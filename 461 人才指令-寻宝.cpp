// ## 2020/10/30 # 江东新风 # 同步马术书大神的更新 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ##2020/08/04 # 江东新风 # 修正寻获宝物时的右下角提示錯誤 ##
// ##2020/08/04 # messi # 語句修正 ##
// ##2020/08/03 # messi # 語句修正 ##
// ## 2020/07/26 ##
/*
@ 만든이: 기마책사
@ UPDATE: '19.1.27    / 최초적용
@ UPDATE: '19.2.13    / 변경내용: 옥새/동작은 수색가능 리스트 제외, 설전/일기토 이벤트 추가

@ 수정자: HoneyBee ('19.2.15, 20.01.05, 20.01.18')
@ 수정사항: 태수가 <군주>인 경우 Message 추가 반영, <학자>에게 설전 화술 추가, <무술가>에게 조형 추가, NULL POINTER 방지조건 추가, 보물 능력 이전 수정

@ 수정자: 기마책사
@ UPDATE: '19.5.4    / 변경내용: 설전 결과에 따른 경험치, 상병 등을 적용

*/

namespace 도시보물수색
{
    //---------------------------------------------------------------------------------------
    // 유저 설정
    const int ACTION_COST = 20;        // 보물수색에 필요한 행동력
    const int GOLD_COST_보물수색 = 300;         // 보물수색에 필요한 금

    const int 보물소재지_발견확률 = 30;            // 보물 소재 도시에서 실행 시 보물 발견 확률
    const float 특기안력_발견확률가중치 = 2.0f;     // 특기_안력 무장이 도시에 있을 시 발견 확률 증가

    //---------------------------------------------------------------------------------------



    class Main
    {

        pk::building@ building;
        pk::force@ force;
        pk::city@ city;
        pk::district@ district;
        pk::person@ kunshu;
        pk::person@ taishu;
        pk::point building_pos;
        uint16 building_gold;
        int building_energy;
        int item_id_found;
        int challange_result;
        array<int> arr_stat = { 70, 75, 80, 85, 90 };

        Main()
        {
            // 보물수색 메뉴 (유저용)
            pk::menu_item 보물수색;
            보물수색.menu = 102;        // 인재메뉴
            보물수색.shortcut = global_menu::shortcut_寻宝;
            보물수색.init = pk::building_menu_item_init_t(init);
            보물수색.is_visible = pk::menu_item_is_visible_t(isVisible_보물수색);
            보물수색.is_enabled = pk::menu_item_is_enabled_t(isEnabled_보물수색);
            보물수색.get_text = pk::menu_item_get_text_t(getText_보물수색);
            보물수색.get_desc = pk::menu_item_get_desc_t(getDesc_보물수색);
            보물수색.handler = pk::menu_item_handler_t(handler_보물수색);
            pk::add_menu_item(보물수색);

        }


        //---------------------------------------------------------------------------------------

        void init(pk::building@ building_t)
        {
            @building = @building_t;
            @force = pk::get_force(building.get_force_id());
            @city = pk::building_to_city(building);
            @kunshu = pk::get_person(pk::get_kunshu_id(building));
            @taishu = pk::get_person(pk::get_taishu_id(building));
            @district = pk::get_district(building.get_district_id());

            building_gold = pk::get_gold(building);
            building_energy = pk::get_energy(building);

            building_pos = building.pos;
        }

        string getText_보물수색()
        {
            return pk::encode("搜寻宝物");
        }

        string getDesc_보물수색()
        {
            if (!pk::is_alive(taishu) or taishu.action_done)
                return pk::encode("太守不在或太守未待命");
            else if (int(district.ap) < ACTION_COST)
                return pk::encode(pk::format("行动力不足 (需要行动力{} )", ACTION_COST));
            else if (building_gold < GOLD_COST_보물수색)
                return pk::encode(pk::format("资金不足 (需要资金{} )", GOLD_COST_보물수색));
            else
                return pk::encode(pk::format("开始搜寻宝物 (使用资金{} )", GOLD_COST_보물수색));
        }

        bool isEnabled_보물수색()
        {
            if (!pk::is_alive(taishu) or taishu.action_done) return false;
            else if (int(district.ap) < ACTION_COST) return false;
            else if (building_gold < GOLD_COST_보물수색) return false;
            return true;
        }

        bool isVisible_보물수색()
        {
            if (pk::is_campaign()) return false;
            if (!pk::is_alive(city)) return false;
            return true;
        }

        //---------------------------------------------------------------------------------------

        bool handler_보물수색()
        {
            if (force.is_player())
            {
                if (pk::choose({ pk::encode(" 是 "), pk::encode(" 否 ") }, pk::encode(pk::format("是否寻找未发现的宝物? \n (使用资金{} )", GOLD_COST_보물수색)), taishu) == 1)
                    return false;
            }

            taishu.action_done = true;

            ch::add_tp(force, 30, force.get_pos());
            pk::add_gold(city, -GOLD_COST_보물수색, true);

            if (force.is_player())
            {
                pk::add_ap(district, -ACTION_COST);
            }


            // 보물수색
            int item_id = -1;
            item_id = get_hidden_item_id(true); // 세력내 미발견 보물검색
            string kunshu_name = pk::decode(pk::get_name(kunshu));
            if (pk::is_valid_item_id(item_id))  // 세력내 미발견 보물 유효
            {
                pk::item@ item = pk::get_item(item_id);
                pk::building@ location = pk::get_building(item.location);

                string item_name = pk::decode(pk::get_name(item));
                string item_location = pk::decode(pk::get_name(location));

                // 미발견 보물이 있는 도시에서 일정 확률로 발견
                if (building.get_id() == item.location and pk::rand_bool(get_chance_item_found(building)))
                {
                    // 아이템 설전/일기토 대전 ('19.2.13)
                    item_id_found = item_id;
                    pk::scene(pk::scene_t(scene_challange));

                    // 보물 획득 성공
                    if (challange_result == 0)
                    {
                        // 보물 발견 처리
                        set_item_found(item);
                        pk::play_se(6);

                        // 보물 발견 메시지
                        if (kunshu.get_id() == taishu.get_id())
                            pk::message_box(pk::encode(pk::format("哈哈.在\x1b[2x{}\x1b[0x寻到了宝物\x1b[1x{}\x1b[0x", item_location, item_name)), taishu);
                        else
                            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x在\x1b[2x{}\x1b[0x得到了\x1b[1x{}\x1b[0x", kunshu_name, item_location, item_name)), taishu);
                    }
                    // 보물 획득 실패
                    else if (challange_result == 1)
                    {
                        pk::play_se(7);

                        if (kunshu.get_id() == taishu.get_id())
                            pk::message_box(pk::encode(pk::format("虽然在\x1b[2x{}\x1b[0x找到了\x1b[1x{}\x1b[0x,但是却失之交臂", item_location, item_name)), taishu);
                        else
                            pk::message_box(pk::encode(pk::format("虽然在\x1b[2x{}\x1b[0x寻找到\x1b[1x{}\x1b[0x,但是未能获得", item_location, item_name)), taishu);
                    }
                }
                else    // 미발견 보물이 없는 도시
                {
                    pk::play_se(6);

                    if (pk::rand_bool(10))       // 일정 확률로 소재+보물명 표시
                        if (kunshu.get_id() == taishu.get_id())
                            pk::message_box(pk::encode(pk::format("貌似传闻在\x1b[2x{}\x1b[0x有\x1b[1x{}\x1b[0x.", item_location, item_name)), taishu);
                        else
                            pk::message_box(pk::encode(pk::format("好像在\x1b[2x{}\x1b[0x见过\x1b[1x{}\x1b[0x.", item_location, item_name)), taishu);
                    else
                    {
                        if (pk::rand_bool(50))  // 일정 확률로 소재만 표시
                        {
                            if (kunshu.get_id() == taishu.get_id())
                                pk::message_box(pk::encode(pk::format("在这个地方好像有\x1b[1x{}\x1b[0x.", item_name)), taishu);
                            else
                                pk::message_box(pk::encode(pk::format("在这个地方好像有\x1b[1x{}\x1b[0x.", item_name)), taishu);
                        }
                        else                         // 영토 내 보물 존재여부만 표시
                        {
                            if (kunshu.get_id() == taishu.get_id())
                                pk::message_box(pk::encode("领地里好像有不错的宝物."), taishu);
                            else
                                pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,领地里好像有不错的宝物.", kunshu_name)), taishu);
                        }
                    }
                }
            }
            else
            {
                pk::play_se(7);

                item_id = get_hidden_item_id(false); // 타세력 미발견 보물검색
                if (pk::is_valid_item_id(item_id) and pk::rand_bool(50))
                {
                    if (kunshu.get_id() == taishu.get_id())
                        pk::message_box(pk::encode("这里好像没有宝物.那么就去别的地方寻找吧."), taishu);
                    else
                        pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,这里好像没有宝物.去别的地方找找怎么样?.", kunshu_name)), taishu);
                }
                else
                {
                    if (kunshu.get_id() == taishu.get_id())
                        pk::message_box(pk::encode("看来没有发现宝物呀."), taishu);
                    else
                        pk::message_box(pk::encode("不管怎样都没有发现宝物."), taishu);
                }
            }

            return true;

        }

        //---------------------------------------------------------------------------------------

        // 미발견 보물 ID 반환 함수
        // in_force =true  : 자세력 검색
        // in_force =false : 타세력 검색
        int get_hidden_item_id(bool in_force)
        {
            int item_id = -1;

            for (int i = 0; i < 보물_끝; i++)
            {
                pk::item@ item = pk::get_item(i);
                if (pk::is_alive(item) and item.status == 보물상태_미등장 and (i != 보물_옥새 and i != 보물_동작))
                {
                    pk::building@ location = pk::get_building(item.location);

                    // NULL POINTER 방지조건 추가
                    if ((location.get_force_id() > -1) and (force.get_id() > -1))
                    {
                        if (in_force and location.get_force_id() == force.get_id())
                        {
                            item_id = i;
                            return item_id;
                        }
                        else if (!in_force and location.get_force_id() != force.get_id())
                        {
                            item_id = i;
                            return item_id;
                        }

                    }

                }
            }

            return item_id;
        }

        // 보물 발견 확률
        int get_chance_item_found(pk::building@ building_t)
        {
            int chance = 보물소재지_발견확률;

            if (!pk::is_alive(building_t)) return 0;
            if (pk::is_valid_person_id(building_t.who_has_skill(특기_안력)))
                chance = pk::min(100, int(특기안력_발견확률가중치 * chance));

            return chance;
        }

        //---------------------------------------------------------------------------------------

        // 설전/일기토
        void scene_challange()
        {
            int mode;
            pk::item@ item = pk::get_item(item_id_found);
            if (item.type == 5) mode = 0;   // 서책: 설전 모드
            else                mode = 1;   // 무기: 일기토 모드

            pk::fade(0);
            pk::background(4);  //배경: 시장
            pk::fade(255);

            // 설전
            if (mode == 0)
            {
                pk::person@ person = pk::get_person(무장_학자);
                person.base_stat[武将能力_智力] = arr_stat[pk::rand(5)];

                // 화술 추가
                person.wajutsu_daikatsu = true;
                person.wajutsu_kiben = true;
                person.wajutsu_mushi = true;
                person.wajutsu_chinsei = true;
                person.wajutsu_gyakujou = true;

                person.update();

                scene_message(person, mode);
                pk::background(-1);
                challange_result = pk::debate(taishu, person, pk::is_player_controlled(taishu), false, true, true).first;
            }
            // 일기토
            else if (mode == 1)
            {
                pk::unit@ dummy_unit;
                pk::person@ person = pk::get_person(무장_무술가);
                person.base_stat[武将能力_武力] = arr_stat[pk::rand(5)];

                // 무장 조형
                person.body[0] = 74;
                person.body[1] = 74;
                person.body[2] = 74;
                person.body[3] = 74;
                person.body[4] = 74;
                person.body[5] = 74;

                person.update();

                scene_message(person, mode);
                pk::background(-1);
                challange_result = pk::duel(dummy_unit, dummy_unit, taishu, null, null, person, null, null, pk::is_player_controlled(taishu), false, 0, true).first;
            }
        }

        void scene_message(pk::person@ person, int mode)
        {
            pk::item@ item = pk::get_item(item_id_found);
            string item_name = pk::decode(pk::get_name(item));
            string mode_name = (mode == 0) ? "文人" : "武者";

            pk::message_box(pk::encode("我听闻你有不错的宝物.能交给我么.我会奉上谢礼的."), taishu);
            pk::message_box(pk::encode(pk::format("呵呵!\x1b[2x{}\x1b[0x吗?大人和我比试一下,赢了我的话就送给你.", item_name)), person);
        }

        // 보물 발견 처리
        void set_item_found(pk::item@ item)
        {
            if (!pk::is_valid_item_id(item.get_id())) return;

            item.status = 보물상태_등장;
            item.location = kunshu.location;
            item.owner = kunshu.get_id();
            item.update();
            string str_location = pk::decode(pk::get_name(pk::get_building(kunshu.service)));
            pk::history_log(pk::get_building(pk::get_person(item.owner).service).pos, pk::get_force(kunshu.get_force_id()).color, pk::encode(pk::format("历经坎坷, \x1b[1x{}\x1b[0x在\x1b[3x{}\x1b[0x寻获宝物\x1b[2x{}\x1b[0x", pk::decode(pk::get_name(kunshu)), str_location, pk::decode(pk::get_name(item)))));


        }


        //---------------------------------------------------------------------------------------
    }

    Main main;
}
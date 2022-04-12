// ## 2020/12/24 # messi # 修正提示错误 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/29 # 江东新风 # 同步马术书大神更新，战役中禁用 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/08 # 氕氘氚 # 加入【国事】菜单 ##
/*
@ 만든이: 기마책사
@ UPDATE: '18.11.17  / 최초적용: 후한황제 천도
@ UPDATE: '18.11.23  / 변경내용: 황제 자동선택 수정, 황후 및 황제 지력 군주에 맞춰서 강제 조정, 군주행동완료 반영
                               후한황제도주 이벤트 추가
@ UPDATE: '18.11.25  / 변경내용: 후한황제도주 시 작위없는군주 제외, 대사 작위 출력오류 수정
@ UPDATE: '18.12.3   / 변경내용: 황제폐위 후 메뉴 초기화 오류 방지 추가

@ 수정자: HoneyBee
@ 내용: 후한황제도주_기준도시수 (조건 변경), 후한황제가 선택하는 군주/세력에서 황건 제외, 황제/황후에게 설전 화술 추가, 황제/황후의 능력 RANDOM 설정 (60 ~ 85)

*/

namespace 후한황제천도
{
    //---------------------------------------------------------------------------------------

    const bool 후한황제천도_사용 = true;
    const int TP_COST_천도 = 1000; 	// 기교P 필요량
    const int ACTION_COST = 50;
    const int GOLD_COST_천도 = 5000;

    const int KEY = pk::hash("殿径");

    //---------------------------------------------------------------------------------------

    const bool 후한황제도주_사용 = true;
    const int 후한황제도주_기준치안 = 65;    // 후한황제 도주 이벤트 발생 기준 치안
    const int 후한황제도주_기준도시수 = 1;   // 후한황제 도주 이벤트 발생 기준 도시수
    const int 후한황제도주_발생확률 = 100;    // 군주가 한실_보통인 경우 도주 확률 (한실_중시는 도주 미발생, 한실_무시는 무조건 도주)

    //---------------------------------------------------------------------------------------


    class Main
    {

        pk::building@ building;
        pk::building@ kunshu_building;
        pk::city@ kunshu_city;
        pk::city@ emperor_city;
        pk::force@ force;
        pk::person@ kunshu;
        pk::person@ emperor;
        pk::person@ support;
        pk::person@ reporter;
        pk::force@ new_force;
        pk::person@ new_kunshu;
        pk::building@ new_building;
        pk::city@ new_city;
<<<<<<< HEAD
        uint16 building_gold;
=======
        int building_gold;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
        bool kunshu_building_check;
        bool emperor_city_check;
        int debate_result;
        array<int> arr_stat = { 65, 70, 75, 80, 85 };

        Main()
        {
            if (후한황제천도_사용)
            {
                pk::menu_item 천도;
                천도.menu = global_menu::菜单_国事;
                //ch::u8debug(pk::format("menu: qian du {}", global_menu::菜单_国事));
                천도.init = pk::building_menu_item_init_t(init);
                천도.is_visible = pk::menu_item_is_visible_t(isVisible_천도);
                천도.is_enabled = pk::menu_item_is_enabled_t(isEnabled_천도);
                천도.get_text = pk::menu_item_get_text_t(getText_천도);
                천도.get_desc = pk::menu_item_get_desc_t(getDesc_천도);
                천도.handler = pk::menu_item_handler_t(handler_천도);
                pk::add_menu_item(천도);
            }

            // 턴 시작 이벤트
            pk::bind(111, pk::trigger111_t(onTurnStart));

        }


        void onTurnStart(pk::force@ force_turn)
        {
            if (후한황제도주_사용)
                event_Emperor_Escape(force_turn);
        }



        //---------------------------------------------------------------------------------------

        void init(pk::building@ building)
        {
            @building = @building;
            @force = pk::get_force(building.get_force_id());
            @kunshu = pk::get_person(pk::get_kunshu_id(building));
            @kunshu_building = pk::get_building(kunshu.service);
            @kunshu_city = pk::building_to_city(kunshu_building);
            building_gold = pk::get_gold(building);

            @emperor = pk::get_person(pk::get_scenario().emperor);

            if (emperor is null) return;
            else if (emperor.mibun != 신분_사망)
            {
                if (emperor.get_id() == 武将_献帝)
                    @support = pk::get_person(武将_皇后);
                else if (emperor.get_id() == 武将_少帝)
                    @support = pk::get_person(武将_宦官);
                else if (emperor.get_id() == 武将_灵帝)
                {
                    switch (pk::rand(2))
                    {
                    case 0: @support = pk::get_person(武将_张让); break;
                    case 1: @support = pk::get_person(武将_蹇硕); break;
                    }
                }

                @emperor_city = pk::building_to_city(pk::hex_object_to_building(pk::get_hex_object(emperor.location)));
                kunshu_building_check = (building.get_id() == kunshu.service);
                emperor_city_check = (emperor_city.get_id() == kunshu.service);


                // 황후 및 황제 지력 강제 변경 ('18.11.18)
                // support.base_stat[武将能力_智力] = pk::max(1, kunshu.base_stat[武将能力_智力] - support_지력);

                // 황후 지력 RANDOM 변경 (65, 70, 75, 80, 85)
                support.base_stat[武将能力_智力] = arr_stat[pk::rand(5)];

                // 화술 추가
                support.wajutsu_daikatsu = true;
                support.wajutsu_kiben = true;
                support.wajutsu_mushi = true;
                support.wajutsu_chinsei = true;
                support.wajutsu_gyakujou = true;

                support.update();

                // emperor.base_stat[武将能力_智力] = pk::max(1, kunshu.base_stat[武将能力_智力] - emperor_지력);

                // 황제 지력 RANDOM 변경 (65, 70, 75, 80, 85)
                emperor.base_stat[武将能力_智力] = arr_stat[pk::rand(5)];

                // 화술 추가
                emperor.wajutsu_daikatsu = true;
                emperor.wajutsu_kiben = true;
                emperor.wajutsu_mushi = true;
                emperor.wajutsu_chinsei = true;
                emperor.wajutsu_gyakujou = true;

                emperor.update();
            }

        }

        string getText_천도()
        {
            return pk::encode("迁都");
        }

        string getDesc_천도()
        {
            if (emperor is null)
                return pk::encode("东汉已经灭亡.");
            else if (!pk::is_protecting_the_emperor(force) or force.title == 爵位_皇帝 or emperor.mibun == 신분_사망)
                return pk::encode("没有拥立汉帝.");
            else if (emperor_city_check)
                return pk::encode("君主和汉帝在同一城市,不可迁都.");
            else if (!kunshu_building_check)
                return pk::encode("只能在君主所在城市实行.");
            else if (pk::is_absent(kunshu) or pk::is_unitize(kunshu) or kunshu.action_done)
                return pk::encode("君主已行动或不在城市中.");
            else if (force.tp < TP_COST_천도)
                return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST_천도));
            else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST)
                return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
            else if (building_gold < GOLD_COST_천도)
                return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST_천도));
            else
                return pk::encode(pk::format("进行迁都. (需{}行动力, {}资金)", TP_COST_천도, GOLD_COST_천도));
        }

        bool isVisible_천도()
        {
            if (pk::is_campaign()) return false;
            if (emperor is null) return false;
            if (!pk::is_protecting_the_emperor(force) or force.title == 爵位_皇帝 or emperor.mibun == 신분_사망) return false;
            return true;
        }

        bool isEnabled_천도()
        {
            if (emperor is null) return false;
            else if (!pk::is_protecting_the_emperor(force) or force.title == 爵位_皇帝 or emperor.mibun == 신분_사망) return false;
            else if (emperor_city_check) return false;
            else if (!kunshu_building_check) return false;
            else if (pk::is_absent(kunshu) or pk::is_unitize(kunshu) or kunshu.action_done) return false;
            else if (force.tp < TP_COST_천도) return false;
            else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST) return false;
            else if (building_gold < GOLD_COST_천도) return false;
            return true;
        }


        bool handler_천도()
        {

            if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("进行迁都吗? \n(需{}技巧点, {}资金)", TP_COST_천도, GOLD_COST_천도)), kunshu) == 1)
                return false;


            //-----------------------------------------------------------------
            // 후한 황제와의 대화
            pk::scene(pk::scene_t(scene_천도));

            //-----------------------------------------------------------------

            if (debate_result == 0)
            {
                // 후한 황제 이동
                pk::move(emperor, kunshu_building, false);

                // 실행 도시 금 소비
                pk::add_gold(kunshu_city, -GOLD_COST_천도, true);

                // 행동력 감소
                auto district = pk::get_district(pk::get_district_id(force, 1));
                pk::add_ap(district, -ACTION_COST);

                // 기교 감소
                ch::add_tp(force, -TP_COST_천도, force.get_pos());

                pk::say(pk::encode("把陛下请来这边."), kunshu, kunshu_city);
                pk::create_effect(0x51, kunshu_building.pos);   //이펙트효과
            }
            else
            {
                // 행동력 감소
                auto district = pk::get_district(pk::get_district_id(force, 1));
                pk::add_ap(district, -ACTION_COST);

                pk::say(pk::encode("没有劝服陛下"), kunshu, kunshu_city);
            }

            // 군주 행동완료 ('18.11.18)
            kunshu.action_done = true;

            return true;

        }


        void scene_천도()
        {
            int pause = int(pk::option["MessagePause"]);
            pk::option["MessagePause"] = 100;
            pk::message_box(pk::encode("需要说服陛下迁都"), kunshu);
            pk::cutin(18);  // 컷인: 걸음걸이
            pk::move_screen(emperor_city.get_pos(), 2000);
            pk::diplomacy(emperor, support, kunshu, pk::diplomacy_t(scene_천도_1));
            pk::move_screen(kunshu_city.get_pos(), 2000);

            if (debate_result == 0)
                pk::cutin(21);  // 컷인: 병사환호

            pk::option["MessagePause"] = 100;
        }

        void scene_천도_1()
        {
            string kunshu_name = pk::decode(pk::get_name(kunshu));
            string city1_name = pk::decode(pk::get_name(emperor_city));
            string city2_name = pk::decode(pk::get_name(kunshu_city));

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,此来有何贵干?", kunshu_name)), emperor);
            pk::message_box(pk::encode(pk::format("近来\x1b[1x{}\x1b[0x逆贼猖獗,恐难安枕.", city1_name)), kunshu);
            pk::message_box(pk::encode("那爱卿以为该如何是好?"), emperor);
            pk::message_box(pk::encode(pk::format("昨日,\x1b[1x{}\x1b[0x上空频有紫气显示,应可迁都于此,可保万全.", city2_name)), kunshu);
            pk::message_box(pk::encode("但此处朕已住得惯了..."), emperor);

            // 군주와 황후와의 설전
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,我觉得贸然迁都不妥.", kunshu_name)), support);
            debate_result = pk::debate(kunshu, support, true, false, false, true).first;

            if (debate_result == 0)  // 군주가 설전 이기면 후한황제와 재설전
            {
                pk::message_box(pk::encode("即便如此也..."), support);
                pk::message_box(pk::encode("容朕再思量一下."), emperor);
                debate_result = pk::debate(kunshu, emperor, true, false, false, true).first;
            }

            if (debate_result == 0)  // 군주가 설전 이김
            {
                pk::message_box(pk::encode("陛下,请尽快准备."), kunshu);
                pk::message_box(pk::encode("好...朕明白你的意思了."), emperor);
            }
            else    // 후한황제가 설전 이김
            {
                pk::message_box(pk::encode("还是不要迁都了."), emperor);
                pk::message_box(pk::encode("明白了."), kunshu);
            }
        }



        //---------------------------------------------------------------------------------------

        void event_Emperor_Escape(pk::force@ force_turn)
        {
            @force = @force_turn;

            if (force.is_player() and pk::is_protecting_the_emperor(force) and force.title != 爵位_皇帝)
            {
                @kunshu = pk::get_person(pk::get_kunshu_id(force));
                @kunshu_building = pk::get_building(kunshu.service);
                @kunshu_city = pk::building_to_city(kunshu_building);
                @emperor = pk::get_person(pk::get_scenario().emperor);
                @emperor_city = pk::building_to_city(pk::hex_object_to_building(pk::get_hex_object(emperor.location)));

                if (emperor.get_id() == 武将_献帝)
                    @support = pk::get_person(武将_皇后);
                else
                    @support = pk::get_person(武将_宦官);

                @ reporter = pk::get_person(무장_문관);

                if (emperor_city.public_order < 후한황제도주_기준치안 and pk::get_city_count(force) <= 후한황제도주_기준도시수 and
                    ((kunshu.kanshitsu == 한실_무시) or (kunshu.kanshitsu == 한실_보통 and pk::rand_bool(후한황제도주_발생확률))))
                {
                    // 가장 강한 세력 검색
                    int force_num = EnemyForceSel(emperor_city);
                    if (force_num > -1)
                    {
                        @ new_force = pk::get_force(force_num);
                        @ new_kunshu = pk::get_person(pk::get_kunshu_id(new_force));
                        @ new_building = pk::get_building(new_kunshu.service);
                        @ new_city = pk::building_to_city(new_building);

                        // 후한 황제 도주씬
                        pk::scene(pk::scene_t(scene_도주));

                        // 후한 황제 이동
                        pk::move(emperor, new_building, false);
                    }
                }
            }

        }

        void scene_도주()
        {
            // 후한황제 밀담
            pk::move_screen(emperor_city.get_pos(), 2000);
            pk::diplomacy(emperor, support, reporter, pk::diplomacy_t(scene_도주_1));

            // 배경 궁성
            scene_도주_2();

            // 도시 이동
            pk::move_screen(new_city.get_pos(), 2000);
            scene_도주_3();

            string emperor_name = pk::decode(pk::get_name(emperor));
            string kunshu_name = pk::decode(pk::get_name(kunshu));
            string new_kunshu_name = pk::decode(pk::get_name(new_kunshu));
            pk::message_box(pk::encode(pk::format("\x1b[1x{}军\x1b[0x拥立了汉帝.", new_kunshu_name)));
            pk::option["MessagePause"] = 100;
        }

        void scene_도주_1()
        {
            string kunshu_name = pk::decode(pk::get_name(kunshu));
            string new_kunshu_name = pk::decode(pk::get_name(new_kunshu));
            string city1_name = pk::decode(pk::get_name(emperor_city));
            string city2_name = pk::decode(pk::get_name(new_city));

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x越来越跋扈了.", kunshu_name)), emperor);
            pk::message_box(pk::encode(pk::format("我看\x1b[1x{}\x1b[0x防御松懈,陛下或可偷溜出去?", city1_name)), support);
            pk::message_box(pk::encode("我们往何处去?"), emperor);
            pk::message_box(pk::encode(pk::format("去依靠\x1b[1x{}\x1b[0x如何? 到\x1b[1x{}\x1b[0x去好了.", new_kunshu_name, city2_name)), support);
            pk::message_box(pk::encode("好主意!快离开这."), emperor);
            pk::message_box(pk::encode("是!微臣会随侍左右."), reporter);
        }
        void scene_도주_2()
        {
            pk::person@ gaurd = pk::get_person(무장_위병);

            pk::background(1);
            pk::message_box(pk::encode("你要去何处?"), gaurd);
            pk::cutin(20);  // 컷인: 노려보기
            pk::message_box(pk::encode("只是出去散步而已.快让开!"), reporter);
            pk::message_box(pk::encode("但是我没有收到报告呀..."), gaurd);
            pk::message_box(pk::encode("放肆!"), support);
            pk::message_box(pk::encode("对不起."), gaurd);
            pk::background(-1);
            pk::fade(100);
        }
        void scene_도주_3()
        {
            string kunshu_name = pk::decode(pk::get_name(kunshu));
            string new_kunshu_name = pk::decode(pk::get_name(new_kunshu));
            string new_kunshu_title = pk::decode(pk::get_name(pk::get_title(new_force.title)));

            pk::background(3);
            pk::fade(255);
            pk::message_box(pk::encode(pk::format("走快点.离\x1b[1x{}\x1b[0x还很远?", new_kunshu_name)), emperor);
            pk::message_box(pk::encode(pk::format("不,陛下.\x1b[1x{}\x1b[0x离这已经很近了.", new_kunshu_name)), reporter);
            pk::message_box(pk::encode(pk::format("你说\x1b[1x{}\x1b[0x是否真的值得信任....", new_kunshu_name)), emperor);
            pk::cutin(12);  // 컷인: 말발굽
            pk::message_box(pk::encode(pk::format("这是什么声音.\x1b[1x{}廓\x1b[0x追来了?", kunshu_name)), emperor);
            pk::fade(0);
            pk::sleep();
            pk::background(-1);

            pk::background(11);
            pk::fade(255);
            pk::play_bgm(17);

            if (new_force.title <= 작위_주자사)
                pk::message_box(pk::encode(pk::format("这不是陛下吗?\x1b[1x{}{}\x1b[0x拜见陛下.", new_kunshu_title, new_kunshu_name)), new_kunshu);
            else
                pk::message_box(pk::encode(pk::format("这不是陛下吗?\x1b[1x{}\x1b[0x拜见陛下.", new_kunshu_name)), new_kunshu);

            pk::message_box(pk::encode(pk::format("见到\x1b[1x{}\x1b[0x你真是万幸.", new_kunshu_name, kunshu_name)), emperor);
            pk::message_box(pk::encode("微臣罪过!让陛下受苦."), new_kunshu);
            pk::message_box(pk::encode("快带陛下到住处."), support);
            pk::message_box(pk::encode("陛下这边请."), new_kunshu);

            pk::cutin(21);  // 컷인: 병사환호
        }

        //--------------------------------------------------------------------------------
        // 최강 적세력 검색 함수
        int EnemyForceSel(pk::city @ city)
        {
            int force_sel = 0;
            int force_city_cnt = 0;

            auto force_list = pk::list_to_array(pk::get_force_list());
            for (int k = 0; k < int(force_list.length); k++)
            {

                pk::force@ force_ = force_list[k];

                pk::building@ building_ = pk::city_to_building(city);


                // 세력이 [황건]일 경우는 제외
                if (force_.kokugou == 국호_황건) continue;

                // 황건적 출신 인물이 [군주]일 경우는 제외 
                if (pk::get_kunshu_id(building_) == 무장_장각 or pk::get_kunshu_id(building_) == 무장_장보 or pk::get_kunshu_id(building_) == 무장_장량
                    or pk::get_kunshu_id(building_) == 무장_관해 or pk::get_kunshu_id(building_) == 무장_주창 or pk::get_kunshu_id(building_) == 무장_배원소
                    or pk::get_kunshu_id(building_) == 무장_장만성 or pk::get_kunshu_id(building_) == 무장_정원지 or pk::get_kunshu_id(building_) == 무장_등무
                    or pk::get_kunshu_id(building_) == 무장_유벽 or pk::get_kunshu_id(building_) == 무장_공도 or pk::get_kunshu_id(building_) == 무장_하의
                    or pk::get_kunshu_id(building_) == 무장_한섬 or pk::get_kunshu_id(building_) == 무장_한충 or pk::get_kunshu_id(building_) == 무장_엄정
                    or pk::get_kunshu_id(building_) == 무장_고승 or pk::get_kunshu_id(building_) == 무장_휴고 or pk::get_kunshu_id(building_) == 무장_손중
                    or pk::get_kunshu_id(building_) == 무장_장연 or pk::get_kunshu_id(building_) == 무장_장개 or pk::get_kunshu_id(building_) == 413
                    or pk::get_kunshu_id(building_) == 무장_파재 or pk::get_kunshu_id(building_) == 무장_변희 or pk::get_kunshu_id(building_) == 무장_요화)
                    continue;


                if (force_.get_force_id() != city.get_force_id() and force_city_cnt < pk::get_city_count(force_))
                {
                    force_city_cnt = pk::get_city_count(force_);
                    force_sel = force_.get_force_id();
                }
            }

            return force_sel;
        }
        //--------------------------------------------------------------------------------

    }

    Main main;
}
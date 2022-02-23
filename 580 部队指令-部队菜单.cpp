// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/02/13 # 江东新风 # 换帅限战斗部队 ##
// ## 2022/01/25 # 江东新风 # 运输队强行军物资损耗，部队强行军入城无代价修复 ##
// ## 2022/01/08 # 江东新风 # 恢复->招募乡勇 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2020/10/30 # 江东新风 # 同步马术书大神的更新 ##  	
// ## 2020/09/30 # 江东新风 # 移动过后无法执行换帅 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/08/09 # 氕氘氚 # 优化回归规则 ##
// ## 2020/08/08 # 氕氘氚 # 修复了換帅取消时可以再次行动的bug ##
/*
@ 만든이: 기마책사
@ UPDATE: '18.11.4   /  최초적용: 계략_공작/도발/고무/대고무/회귀, 부대해산/병합
@ UPDATE: '18.11.8   /  변경내용: 계략_욕설 추가, 계략사용 특기조건 반영
@ UPDATE: '18.11.14  /  변경내용: 게략_기습 추가, 강행군추가
@ UPDATE: '18.11.24  /  변경내용: 계략_도발 통합(욕설삭제)+설전추가, 계략_기습 수상 추가, 부대해산/병합 실행가능 병력기준 변경, NPC부대 지력차계산 추가
@ UPDATE: '18.11.25  /  변경내용: 스크립트 구조변경, 부대설전 버그개선을 위하여 trigger 방식채용
@ UPDATE: '18.11.30  /  변경내용: 계략_기습 버그방지조건 추가, 부대_해산 기준값 오류수정
@ UPDATE: '18.12.3   /  변경내용: 계략_기습 빈칸없는 목표부대 검색 오류수정, 계략_도발 발동오류 개선 및 불러오기 실행 시 초기화 추가
@ UPDATE: '18.12.12  /  변경내용: 계략_기습 기력소모 수정, shift+우클릭 메뉴 무한행동 사용시 오류 수정, 부대_해산/병합 주변적군 확인조건 추가
@ UPDATE: '18.12.21  /  변경내용: 부대_병합 설명수정, 계략_낙석 추가
@ UPDATE: '18.12.23  /  변경내용: 부대_해산/병합 주위의적 검색함수 오류 수정
@ UPDATE: '18.12.30  /  변경내용: 계략_도발 特技_论客 우대 적용, 부대_재편 추가 (주장변경기능), 계략_공작 적시설 탈취 기능 추가


@ 수정자: HoneyBee
@ 내용: 부대묘책메뉴 SCRIPT 추가에 따른 기력 밸런스 재조정 (계략에 소모되는 기력 수치 재조정)

*/

namespace 야전부대메뉴
{

    //---------------------------------------------------------------------------------------
    // 유저 설정 (true = on, false = off)

    // 부대 메뉴 추가여부 설정
    const bool 부대_해산_사용 = true;    // 2천명 이상 부대 강제 해산 ('18.11.9)
    const bool 부대_병합_사용 = true;    // 동일 병과부대 인접 시 병력 인계 ('18.11.6)
    const bool 부대_회복_사용 = true;    // 금을 소비하여 병력 회복 ('18.11.6)
    const bool 부대_재편_사용 = true;    // 부대의 주장 변경 ('18.12.26)
    const bool 메뉴_강행군_사용 = true;   // 기력 추가 소비하여 추가 이동 실시 ('18.11.13)

    // 계략에 대한 특기 옵션
    const bool 계략_특기_설정 = true;        // 계략에 대한 특기 설정 사용
    const bool 백출_반감_설정 = true;        // true= 50% 소비, false = 무조건 1 소비
    const bool 계략_병력_차등 = true;        // 병력수에 따른 계략 소비기력 차등 적용

    // 강행군 실행 조건
    const int ENERGY_MIN_강행군 = 10;       // 강행군을 실행하기 위한 최소 기력
    const float MOVEMENT_강행군 = 0.6f;    // [강행군_이동력]/[부대_이동력] 비율 


    // 부대 실행 조건
    const int GOLD_COST_병합 = 500;   // 아직 사용안하는 조건    
    const int GOLD_COST_회복 = 300;

    //---------------------------------------------------------------------------------------


    class Main
    {
        Main()
        {
            // 부대 메뉴 추가
            add_menu_unit_order();

        }

        pk::unit@ src_unit;
        pk::unit@ dst_unit;
        pk::point src_pos_;
        pk::point dst_pos_;

        pk::unit@ src_unit_debate;
        pk::unit@ dst_unit_debate;

        pk::list<pk::unit@> unit_debate_list;
        int debate_result;



        void add_menu_unit_order()
        {
            //-------------------------------------------------------------



            //-------------------------------------------------------------

            // 부대메뉴 : 해산
            if (부대_해산_사용)
            {
                pk::menu_item 부대_해산;
                부대_해산.menu = global_menu::菜单_部队;
                부대_해산.init = pk::unit_menu_item_init_t(init);
                부대_해산.get_text = pk::menu_item_get_text_t(getText_부대_해산);
                부대_해산.get_desc = pk::menu_item_get_desc_t(getDesc_부대_해산);
                부대_해산.is_enabled = pk::menu_item_is_enabled_t(isEnabled_부대_해산);
                부대_해산.get_targets = pk::unit_menu_item_get_targets_t(getTargets_부대_해산);
                부대_해산.get_chance = pk::unit_menu_item_get_chance_t(getChance_100);
                부대_해산.handler = pk::unit_menu_item_handler_t(handler_부대_해산);
                pk::add_menu_item(부대_해산);
            }

            // 부대메뉴 : 병합
            if (부대_병합_사용)
            {
                pk::menu_item 부대_병합;
                부대_병합.menu = global_menu::菜单_部队;
                부대_병합.init = pk::unit_menu_item_init_t(init);
                부대_병합.get_text = pk::menu_item_get_text_t(getText_부대_병합);
                부대_병합.get_desc = pk::menu_item_get_desc_t(getDesc_부대_병합);
                부대_병합.is_enabled = pk::menu_item_is_enabled_t(isEnabled_부대_병합);
                부대_병합.get_targets = pk::unit_menu_item_get_targets_t(getTargets_부대_병합);
                부대_병합.get_chance = pk::unit_menu_item_get_chance_t(getChance_100);
                부대_병합.handler = pk::unit_menu_item_handler_t(handler_부대_병합);
                pk::add_menu_item(부대_병합);
            }

            // 회복 : 소지금 사용하여 1부대 병력 회복
            if (부대_회복_사용)
            {
                pk::menu_item 부대_회복;
                부대_회복.menu = global_menu::菜单_部队;
                부대_회복.init = pk::unit_menu_item_init_t(init);
                부대_회복.get_text = pk::menu_item_get_text_t(getText_부대_회복);
                부대_회복.get_desc = pk::menu_item_get_desc_t(getDesc_부대_회복);
                부대_회복.is_enabled = pk::menu_item_is_enabled_t(isEnabled_부대_회복);
                부대_회복.get_targets = pk::unit_menu_item_get_targets_t(getTargets_부대_회복);
                부대_회복.get_chance = pk::unit_menu_item_get_chance_t(getChance_100);
                부대_회복.handler = pk::unit_menu_item_handler_t(handler_부대_회복);
                pk::add_menu_item(부대_회복);
            }

            // 재편 : 부대 주장을 변경
            if (부대_재편_사용)
            {
                pk::menu_item 부대_재편;
                부대_재편.menu = global_menu::菜单_部队;
                부대_재편.init = pk::unit_menu_item_init_t(init);
                부대_재편.get_text = pk::menu_item_get_text_t(getText_부대_재편);
                부대_재편.get_desc = pk::menu_item_get_desc_t(getDesc_부대_재편);
                부대_재편.is_enabled = pk::menu_item_is_enabled_t(isEnabled_부대_재편);
                부대_재편.get_targets = pk::unit_menu_item_get_targets_t(getTargets_부대_재편);
                부대_재편.get_chance = pk::unit_menu_item_get_chance_t(getChance_100);
                부대_재편.handler = pk::unit_menu_item_handler_t(handler_부대_재편);
                pk::add_menu_item(부대_재편);
            }

            // 강행군 : 기력을 소비하여 추가 이동
            if (메뉴_강행군_사용)
            {
                pk::menu_item 메뉴_강행군;
                메뉴_강행군.menu = global_menu::菜单_部队;
                메뉴_강행군.init = pk::unit_menu_item_init_t(init);
                메뉴_강행군.get_text = pk::menu_item_get_text_t(getText_메뉴_강행군);
                메뉴_강행군.get_desc = pk::menu_item_get_desc_t(getDesc_메뉴_강행군);
                메뉴_강행군.is_enabled = pk::menu_item_is_enabled_t(isEnabled_메뉴_강행군);
                메뉴_강행군.get_targets = pk::unit_menu_item_get_targets_t(getTargets_메뉴_강행군);
                메뉴_강행군.get_chance = pk::unit_menu_item_get_chance_t(getChance_100);
                메뉴_강행군.handler = pk::unit_menu_item_handler_t(handler_메뉴_강행군);
                pk::add_menu_item(메뉴_강행군);
            }


        }

        //---------------------------------------------------------------------------



        void init(pk::unit@ unit, pk::point src_pos)
        {
            @src_unit = @unit;
            src_pos_ = src_pos;
        }

        int getChance_100(pk::point dst_pos)
        {
            return 100;
        }

        //---------------------------------------------------------------------------
        // 계략 설정 관련 함수
        //---------------------------------------------------------------------------


        int func_range_add()
        {
            if (!계략_특기_설정) return 0;

            if (ch::has_skill(src_unit, 특기_귀모))
                return 1;

            return 0;
        }

        //---------------------------------------------------------------------------




        //---------------------------------------------------------------------------
        // 부대_해산
        //---------------------------------------------------------------------------

        string getText_부대_해산()
        {
            return pk::encode("解散");
        }

        string getDesc_부대_해산()
        {
            if (src_unit.troops < 2000)
                return pk::encode("兵力需 2000 以上");
            else if (count_enemies_around(src_pos_) >= 1)
                return pk::encode("因为周围有敌军,所以无法执行");
            else
                return pk::encode("解散部队 (兵力全消失只回归将帅)");
        }

        bool isVisible_부대_해산()
        {
            if (pk::is_campaign()) return false;
            return true;
        }

        bool isEnabled_부대_해산()
        {
            if (src_unit.troops < 2000) return false;
            else if (count_enemies_around(src_pos_) >= 1) return false;
            return getTargets_부대_해산().length > 0;
        }

        pk::array<pk::point_int> getTargets_부대_해산()
        {
            pk::array<pk::point_int> target_units;

            if (count_enemies_around(src_pos_) < 1)
            {
                target_units.insertLast(pk::point_int(src_pos_, 1));
            }
            return target_units;
        }

        bool handler_부대_해산(pk::point dst_pos)
        {
            pk::unit@ target_unit = pk::get_unit(dst_pos);

            // 행동완료
            src_unit.action_done = true;
            if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
                src_unit.action_done = false;

            // 부대 해산
            pk::say(pk::encode("解散部队"), pk::get_person(target_unit.leader));

            ch::add_troops(target_unit, -target_unit.troops, false, 102);//以全逃兵形式实现
            if (target_unit.troops <= 0)
                pk::kill(target_unit);

            return true;
        }

        // 주위의 적부대, 적건물 수 검색
        int count_enemies_around(pk::point pos)
        {
            int count_enemy = 0;
            int max_range = 5;

            pk::array<pk::point> range = pk::range(pos, 1, max_range);
            for (int i = 0; i < int(range.length); i++)
            {
                pk::unit@ unit_around = pk::get_unit(range[i]);
                if (unit_around !is null and pk::is_enemy(src_unit, unit_around))
                    count_enemy += 1;

                pk::building@ building_around = pk::get_building(range[i]);
                if (building_around !is null and pk::is_enemy(src_unit, building_around))
                    count_enemy += 1;
            }

            return count_enemy;
        }

        bool check_spec_around(pk::unit@ unit)
        {
            int count_enemy = 0;
            int max_range = 2;

            pk::array<pk::point> range = pk::range(pk::get_unit_menu_hex_pos(), 1, max_range);
            for (int i = 0; i < int(range.length); i++)
            {
                pk::hex@ hex = pk::get_hex(range[i]);
                if (hex.has_building)
                {
                    auto building = pk::get_building(range[i]);
                    if (building.get_force_id() == unit.get_force_id())
                    {
                        if (ch::is_valid_spec_id(ch::to_spec_id(building.get_id())))
                            return true;
                    }
                }
            }

            return false;
        }

        //---------------------------------------------------------------------------
        // 부대_병합 (삼국지13 오마쥬)
        //---------------------------------------------------------------------------

        string getText_부대_병합()
        {
            return pk::encode("合并");
        }

        string getDesc_부대_병합()
        {
            if (pk::get_person(src_unit.leader).rank >= 官职_无 and pk::get_person(src_unit.leader).mibun == 身份_一般)
                return pk::encode("官职不符合条件");
            else if (src_unit.troops < 2000)
                return pk::encode("兵力需 2000 以上");
            else if (count_enemies_around(src_pos_) >= 1)
                return pk::encode("因为周围有敌军,所以无法执行");
            else if (getTargets_부대_병합().length == 0)
                return pk::encode("范围内没有同兵种我军部队");
            else
                return pk::encode("并入目标部队");
        }

        bool isVisible_부대_병합()
        {
            if (pk::is_campaign()) return false;
            return true;
        }

        bool isEnabled_부대_병합()
        {
            if (pk::get_person(src_unit.leader).rank >= 官职_无 and pk::get_person(src_unit.leader).mibun == 身份_一般) return false;
            else if (src_unit.troops < 2000) return false;
            else if (count_enemies_around(src_pos_) >= 1) return false;
            return getTargets_부대_병합().length > 0;
        }

        pk::array<pk::point_int> getTargets_부대_병합()
        {
            pk::array<pk::point_int> target_units;

            if (count_enemies_around(src_pos_) < 1)
            {
                auto range = pk::range(src_pos_, 1, 1);
                for (int i = 0; i < int(range.length); i++)
                {
                    auto dst_unit = pk::get_unit(range[i]);
                    if (dst_unit !is null and src_unit.pos != dst_unit.pos and src_unit.get_force_id() == dst_unit.get_force_id() and pk::get_ground_weapon_id(src_unit) == pk::get_ground_weapon_id(dst_unit))
                        target_units.insertLast(pk::point_int(range[i], 1));
                }
            }
            return target_units;
        }

        bool handler_부대_병합(pk::point dst_pos)
        {
            pk::unit@ target_unit = pk::get_unit(dst_pos);

            // 실행 부대 계략 실행 음성 효과, 실행/대상부대 대화
            if (pk::is_in_screen(src_pos_))
            {
                pk::play_se(0x39, src_pos_);
                //pk::wait(1, 1000);
            }

            int troops_re_org = pk::min(src_unit.troops, (pk::get_max_troops(target_unit) - target_unit.troops));
            int energy_re_org = ((troops_re_org * src_unit.energy) + (target_unit.troops * target_unit.energy)) / (troops_re_org + target_unit.troops);

            int food_re_org = pk::min(src_unit.food, (2 * troops_re_org));


            // 실행부대 병력감소, 병량감소
            ch::add_troops(src_unit, -troops_re_org, true);//我方部队减兵力
            pk::add_food(src_unit, -food_re_org, true);

            if (src_unit.troops == 0)
            {
                pk::say(pk::encode("我军部队解散."), pk::get_person(src_unit.leader));
                pk::kill(src_unit);
            }
            else
            {
                pk::say(pk::encode("兵力已达到满编制."), pk::get_person(src_unit.leader));
            }


            // 대상부대 병력증가
            ch::add_troops(target_unit, troops_re_org, true);//目标部队加兵力
            pk::say(pk::encode("从现在开始听我指挥"), pk::get_person(target_unit.leader));
            // 대상부대 병량증가
            pk::add_food(target_unit, food_re_org, true);
            // 대상부대 기력조정
            pk::add_energy(target_unit, (energy_re_org - target_unit.energy), true);

            // 효과 : 기력상승(보라)
            if (pk::is_in_screen(target_unit.pos))
                pk::create_effect(0x4c, target_unit.pos);

            // 행동완료
            src_unit.action_done = true;
            if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
                src_unit.action_done = false;

            return true;
        }



        //---------------------------------------------------------------------------
        // 부대_회복
        //---------------------------------------------------------------------------

        string getText_부대_회복()
        {
            return pk::encode("招募乡勇");
        }

        string getDesc_부대_회복()
        {
            if (pk::get_person(src_unit.leader).rank >= 官职_无 and pk::get_person(src_unit.leader).mibun == 身份_一般)
                return pk::encode("官职不符合条件");
            else if (src_unit.gold < GOLD_COST_회복)
                return pk::encode("金钱不足");
            else if (!check_spec_around(src_unit))
                return pk::encode("仅可在地名建筑周围2格执行");
            else
                return pk::encode("消耗金钱,招募乡勇参战");
        }

        bool isEnabled_부대_회복()
        {
            if (pk::get_person(src_unit.leader).rank >= 官职_无 and pk::get_person(src_unit.leader).mibun == 身份_一般) return false;
            else if (src_unit.gold < GOLD_COST_회복) return false;
            else if (!check_spec_around(src_unit)) return false;
            return getTargets_부대_회복().length > 0;
        }

        bool isVisible_부대_회복()
        {
            if (pk::is_campaign()) return false;
            return true;
        }

        pk::array<pk::point_int> getTargets_부대_회복()
        {
            pk::array<pk::point_int> target_units;
            target_units.insertLast(pk::point_int(src_pos_, 1));

            auto range = pk::range(src_pos_, 1, 2 + func_range_add());
            for (int i = 0; i < int(range.length); i++)
            {
                auto dst_unit = pk::get_unit(range[i]);
                if (dst_unit !is null and src_unit.pos != dst_unit.pos and src_unit.get_force_id() == dst_unit.get_force_id())
                    target_units.insertLast(pk::point_int(range[i], 1));
            }

            return target_units;
        }


        bool handler_부대_회복(pk::point dst_pos)
        {
            pk::unit@ target_unit = pk::get_unit(dst_pos);

            // 실행 부대 계략 실행 음성 효과, 실행/대상부대 대화
            if (pk::is_in_screen(src_pos_))
            {
                pk::play_voice(0x37, pk::get_person(src_unit.leader));  // 음성 : 진정
                pk::play_se(0x39, src_pos_);
                pk::create_effect(0x4a, src_pos_);  // 효과: 기력상승(빨강)
            }
            // 실행 부대 소지금 감소, 병력 회복
            pk::say(pk::encode("招募乡勇参与战斗吧!"), pk::get_person(src_unit.leader));
            pk::add_gold(src_unit, -GOLD_COST_회복, true);
            
            ch::add_troops(target_unit, (GOLD_COST_회복 / 40 * pk::get_person(src_unit.leader).stat[武将能力_魅力]), true);//治疗伤员得换一种实现形式，注意

            // 기교점수
            ch::add_tp(pk::get_force(src_unit.get_force_id()), 20, dst_pos);
            // 능력경험치
            pk::add_stat_exp(src_unit, 武将能力_魅力, 4);
            // 공적
            pk::add_kouseki(src_unit, 100);

            // 행동완료
            src_unit.action_done = true;
            if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
                src_unit.action_done = false;

            return true;
        }



        //---------------------------------------------------------------------------
        // 부대_재편
        //---------------------------------------------------------------------------

        string getText_부대_재편()
        {
            return pk::encode("换帅");
        }

        string getDesc_부대_재편()
        {
            if (src_unit.get_pos() != pk::get_unit_menu_hex_pos())
                return pk::encode("移动过后无法执行换帅.");
            else if (!pk::is_alive(pk::get_person(src_unit.member[1])))
                return pk::encode("没有副将");
            else if (count_enemies_around(src_pos_) >= 1)
                return pk::encode("因为周围有敌军,所以无法执行");
            return pk::encode("重新选择主将");
        }

        bool isVisible_부대_재편()
        {
            if (pk::is_campaign()) return false;
            if (src_unit.type != 部队类型_战斗) return false;
            return true;
        }

        bool isEnabled_부대_재편()
        {
            if (src_unit.get_pos() != pk::get_unit_menu_hex_pos()) return false;
            else if (!pk::is_alive(pk::get_person(src_unit.member[1]))) return false;
            else if (count_enemies_around(src_pos_) >= 1) return false;
            return getTargets_부대_재편().length > 0;
        }

        pk::array<pk::point_int> getTargets_부대_재편()
        {
            pk::array<pk::point_int> target_units;

            if (count_enemies_around(src_pos_) < 1)
            {
                target_units.insertLast(pk::point_int(src_pos_, 1));
            }
            return target_units;
        }

        bool handler_부대_재편(pk::point dst_pos)
        {
            // 주장/부장 리스트
            pk::person@ p0 = pk::get_person(src_unit.member[0]);
            pk::person@ p1 = pk::get_person(src_unit.member[1]);
            pk::person@ p2 = pk::get_person(src_unit.member[2]);

            pk::list<pk::person@> member_list;
            if (pk::is_alive(p0)) member_list.add(p0);
            if (pk::is_alive(p1)) member_list.add(p1);
            if (pk::is_alive(p2)) member_list.add(p2);

            // 지휘병력 확인
            int cmd_old = pk::get_command(p0);
            int cmd_new;

            // 무장 선택
            bool confirm = false;
            pk::list<pk::person@> leader_sel;
            int leader_id;
            pk::person@ leader;
            string leader_name;
            while (!confirm)
            {
                leader_sel = pk::person_selector(pk::encode("选择主将"), pk::encode("选择部队的主将."), member_list, 1, 1);
                if (leader_sel.count == 0)
                {
                    //src_unit.action_done = true;
                    return false; // 취소 종료
                }


                leader_id = leader_sel[0].get_id();      // 선택한 무장번호
                if (leader_id == src_unit.member[0])
                {
                    pk::message_box(pk::encode("未变更主将"));
                }
                else
                {
                    @leader = pk::get_person(leader_id);
                    leader_name = pk::decode(pk::get_name(leader));
                    cmd_new = pk::get_command(leader);
                    confirm = pk::yes_no(pk::encode(pk::format("是否选择\x1b[1x{}\x1b[0x作为新的主将?\n(部队最高指挥兵力从\x1b[1x{}\x1b[0x变成\x1b[1x{}\x1b[0x)", leader_name, cmd_old, cmd_new)));
                }
            }

            // 주장/부장 무장ID swap
            if (leader_id == src_unit.member[2])
            {
                src_unit.member[2] = src_unit.member[0];
                src_unit.member[0] = leader_id;
            }
            else if (leader_id == src_unit.member[1])
            {
                src_unit.member[1] = src_unit.member[0];
                src_unit.member[0] = leader_id;
            }
            else if (leader_id == src_unit.member[0])
            {
                src_unit.member[0] = leader_id;
            }
            src_unit.update();

            // 새로운 주장의 대화표시
            if (pk::is_in_screen(src_pos_))
            {
                pk::say(pk::encode(pk::format("从现在开始由我\x1b[1x{}\x1b[0x指挥", leader_name)), leader);
            }

            // 지휘병력 초과 시 부대 병력 조정
            if (int(src_unit.troops) > int(cmd_new))
                ch::add_troops(src_unit, int(src_unit.troops - cmd_new), true, 102);//兵力超过减兵需要另一种模式

            // 행동완료
            src_unit.action_done = true;
            if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
                src_unit.action_done = false;

            return true;
        }



        //---------------------------------------------------------------------------
        // 메뉴_강행군 (삼국지7 오마쥬)
        //---------------------------------------------------------------------------

        string getText_메뉴_강행군()
        {
            return pk::encode("强行军");
        }

        string getDesc_메뉴_강행군()
        {
            if (pk::get_person(src_unit.leader).rank >= 官职_无 and pk::get_person(src_unit.leader).mibun == 身份_一般)
                return pk::encode("官职不符合条件");
            else if (src_unit.energy < ENERGY_MIN_강행군)
                return pk::encode("气力不足");
            else if (src_unit.type == 部队类型_运输) return pk::encode(pk::format("执行强行军 (必须 {} 气力，将损耗部分物资)", ENERGY_MIN_강행군));
            else
                return pk::encode(pk::format("执行强行军 (必须 {} 气力)", ENERGY_MIN_강행군));
        }

        bool isVisible_메뉴_강행군()
        {
            if (pk::is_campaign()) return false;
            return true;
        }

        bool isEnabled_메뉴_강행군()
        {
            if (pk::get_person(src_unit.leader).rank >= 官职_无 and pk::get_person(src_unit.leader).mibun == 身份_一般) return false;
            else if (src_unit.energy < ENERGY_MIN_강행군) return false;
            return getTargets_메뉴_강행군().length > 0;
        }

        pk::array<pk::point_int> getTargets_메뉴_강행군()
        {
            int movement = int(MOVEMENT_강행군 * src_unit.attr.stat[部队能力_移动]);

            // 이동 목표 지점 획득
            pk::array<pk::point_int> target_pos;
            auto range = pk::get_movable_pos(src_unit, src_pos_, movement); // 이동가능한 경로만 획득
            for (int i = 0; i < int(range.length); i++)
            {
                pk::hex@ hex = pk::get_hex(range[i]);
                target_pos.insertLast(pk::point_int(range[i], 1));
            }
            return target_pos;
        }

        bool handler_메뉴_강행군(pk::point dst_pos)
        {
            // 추가 이동력 설정
            int movement = int(MOVEMENT_강행군 * src_unit.attr.stat[部队能力_移动]);
            int cost_sum = 0;   // 총 이동 cost 더하기
            int cost_next = 0;

            // 부대 위치 지형
            pk::hex@ hex = pk::get_hex(src_unit.pos);
            int terrain_id = hex.terrain;
            // 부대 병과
            int weapon_id = pk::is_water_terrain(terrain_id) ? pk::get_sea_weapon_id(src_unit) : pk::get_ground_weapon_id(src_unit);
            pk::equipment@ equipment = pk::get_equipment(weapon_id);


            // 목표지점까지 이동 경로 획득
            array<pk::point> steps;
            // 중간이동지점
            auto paths = pk::get_path(src_unit, src_unit.pos, dst_pos);

            // 중간이동지점
            for (int i = 0; i < int(paths.length); i++)
            {
                @hex = pk::get_hex(paths[i]);
                terrain_id = hex.terrain;
                weapon_id = pk::is_water_terrain(terrain_id) ? pk::get_sea_weapon_id(src_unit) : pk::get_ground_weapon_id(src_unit);
                @ equipment = pk::get_equipment(weapon_id);

                steps.insertLast(paths[i]); //중간이동지점 추가
                if (i > 0)
                    cost_sum += equipment.movement_cost[terrain_id];

            }

            // 최종이동지점
            @hex = pk::get_hex(dst_pos);
            terrain_id = hex.terrain;
            weapon_id = pk::is_water_terrain(terrain_id) ? pk::get_sea_weapon_id(src_unit) : pk::get_ground_weapon_id(src_unit);
            @ equipment = pk::get_equipment(weapon_id);

            steps.insertLast(dst_pos);  //최종이동지점 추가
            cost_sum += equipment.movement_cost[terrain_id];

            // 부대 강행군 실행
            if (steps.length != 0)
            {
                // 실행 부대 계략 실행 음성 효과
                if (pk::is_in_screen(src_pos_))
                {
                    //pk::play_se(0x39, src_pos_);
                    //pk::create_effect(0x3a, src_pos_);
                    switch (pk::rand(2))
                    {
                    case 0: pk::play_voice(0x1A, pk::get_person(src_unit.leader)); break; // 음성 : 전투_이동
                    case 1: pk::play_voice(0x1B, pk::get_person(src_unit.leader)); break; // 음성 : 전투_이동
                    }
                }

                pk::say(pk::encode("加紧赶路!"), pk::get_person(src_unit.leader));

                // [이동경로상 이동 cost] x 50%] 만큼 기력 소모
                pk::add_energy(src_unit, -int(0.50f * cost_sum), true);
                // 병력의 5% x [이동경로상 이동 cost]/[총 이동력] 만큼 병력 이탈
                ch::add_troops(src_unit, -int(0.05f * src_unit.troops * cost_sum / movement), true, 101);//强行军损伤的伤兵和逃兵可能得其他模式

                if (src_unit.type == 部队类型_运输)
                {
                    //损耗各类物资
                    //金
                    if (src_unit.gold > 1) pk::add_gold(src_unit, -int(0.1f * src_unit.gold), true);
                    //兵粮
                    if (src_unit.food > 1) pk::add_food(src_unit, -int(0.1f * src_unit.food), true);
                    //各类兵器
                    for (int i = 0; i < 兵器_末; ++i)
                    {
                        if (src_unit.stock[i].amount > 1) src_unit.stock[i].amount -= int(0.1f * src_unit.stock[i].amount);
                    }
                }
                pk::move(src_unit, steps); // 전체경로 일괄이동

            }

            // 행동완료
            src_unit.action_done = true;
            if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
                src_unit.action_done = false;


            return true;
        }


    }

    Main main;
}
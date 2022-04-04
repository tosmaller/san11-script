// ## 2020/12/12 # 江东新风 # 修复trace参数类型错误 ##
// ## 2020/10/30 # 江东新风 # 同步马术书大神的更新 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/09/16 # 江东新风 # 修复港口耐久已满亦能进行指令 城墙维修  ##
// ## 2020/08/09 # 氕氘氚 # 加入【军事】菜单 ##
/*
@ 만든이: 기마책사
@ UPDATE: '18.11.6    / 최초적용
@ UPDATE: '18.11.16   / 변경내용: 효과음 최적화
@ UPDATE: '18.11.22   / 변경내용: AI가 성벽보수 실행할 알고리즘 추가, 성벽보수 턴별 1회로 제한
@ UPDATE: '18.12.3    / 변경내용: 실행1회 제한 오류 수정
*/

namespace 军事指令_城墙维修
{
    //---------------------------------------------------------------------------------------

    const int ACTION_COST = 10;
    const int ENERGY_COST_城墙维修 = 15;
    const int GOLD_COST_城墙维修 = 500;

    const int KEY = pk::hash("城墙维修");


    const bool 城墙维修_AI실행 = true;

    //---------------------------------------------------------------------------------------



    class Main
    {
        bool 调试模式 = false;
        //pk::building@ building;
        pk::building@ building_;
        pk::force@ force;
        pk::city@ city;
        pk::person@ taishu;
        pk::point building_pos;

        Main()
        {
            pk::bind(202, pk::trigger202_t(onAIRunningOrder));
            add_menu();
        }

        void add_menu()
        {
            // 城墙维修 메뉴 (유저용)
            pk::menu_item 城墙维修;
            城墙维修.menu = 101;
            // 城墙维修.pos = 8;
            城墙维修.shortcut = global_menu::shortcut_城墙维修;
            城墙维修.init = pk::building_menu_item_init_t(init);
            城墙维修.is_visible = pk::menu_item_is_visible_t(isVisible_城墙维修);
            城墙维修.is_enabled = pk::menu_item_is_enabled_t(isEnabled_城墙维修);
            城墙维修.get_text = pk::menu_item_get_text_t(getText_城墙维修);
            城墙维修.get_desc = pk::menu_item_get_desc_t(getDesc_城墙维修);
            城墙维修.handler = pk::menu_item_handler_t(handler_城墙维修);
            pk::add_menu_item(城墙维修);
        }

        //---------------------------------------------------------------------------------------

        void init(pk::building@ building)
        {
            @building_ = @building;
            @force = pk::get_force(building.get_force_id());
            @city = pk::building_to_city(building);
            @taishu = pk::get_person(pk::get_taishu_id(building));

            building_pos = building.pos;

        }

        string getText_城墙维修()
        {
            return pk::encode("城墙维修"); //chengqiangweixiu
        }

        string getDesc_城墙维修()
        {
            auto person_list = pk::get_idle_person_list(building_);
            int result = check_avaliable(building_, person_list);
            switch (result)
            {
            case 1: return pk::encode("已执行过城墙维修");
            case 2: return pk::encode("没有可执行的武将. ");
            case 3: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
            case 4: return pk::encode("耐久已达最大值.");
            case 5: return pk::encode(pk::format("气力不足.(必须{} 气力)", ENERGY_COST_城墙维修));
            case 6: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST_城墙维修));
            case 0: return pk::encode(pk::format("实行城墙维修.(气力 {}, 资金 {})", ENERGY_COST_城墙维修, GOLD_COST_城墙维修));
            default:
                return pk::encode("");
            }
            return pk::encode("");
        }

        bool isEnabled_城墙维修()
        {
            auto person_list = pk::get_idle_person_list(building_);
            int result = check_avaliable(building_, person_list);
            if (result != 0) return false;
            else return true;
        }

        int check_avaliable(pk::building@ building,pk::list<pk::person@> list)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
        {
            if (base_ex[building.get_id()].repair_done) return 1;
            else if (list.count == 0) return 2;
            else if (int(pk::get_district(building.get_district_id()).ap) < ACTION_COST) return 3;
            else if (int(building.hp) >= pk::get_max_hp(building)) return 4;
            else if (pk::get_energy(building) < ENERGY_COST_城墙维修) return 5;
            else if (pk::get_gold(building) < GOLD_COST_城墙维修) return 6;
            return 0;
        }

        bool isVisible_城墙维修()
        {
            if (pk::is_campaign()) return false;
            if (!pk::is_alive(building_)) return false;
            return true;
        }


        bool handler_城墙维修()
        {
            if (force.is_player())
            {
                if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("实行城墙维修吗? (气力 {}, 资金 {})", ENERGY_COST_城墙维修, GOLD_COST_城墙维修)), taishu) == 1)
                    return false;
            }


            // 실행가능 무장리스트
            pk::list<pk::person@> person_list = pk::get_idle_person_list(building_);
            if (person_list.count == 0) return false;

            // 실행무장 선택하기
            pk::list<pk::person@> person_sel;
            person_list.sort(function(a, b)
            {
                bool a_skill = ch::has_skill(a, 特技_筑城);
                bool b_skill = ch::has_skill(b, 特技_筑城);
                if (a_skill and !b_skill) return true;
                if (!a_skill and b_skill) return false;

                return (a.stat[武将能力_政治] > b.stat[武将能力_政治]); // 무장 정렬 (지력순)
            });
            if (pk::is_player_controlled(building_))
            {
                person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择可执行的武将."), person_list, 1, 1);
                if (person_sel.count == 0) return false; // 미선택 시 취소 종료
            }

            return run_order(person_sel, building_);

        }


        //---------------------------------------------------------------------------------------


        void onAIRunningOrder(pk::ai_context@ context, pk::building@ building, int cmd)
        {
            if (cmd == 据点AI_城墙维修)
            {
                pk::list<pk::person@> person_sel;
                if (run_order_before(context, person_sel, building)) run_order(person_sel, building);
                else if (调试模式) pk::trace("据点AI_城墙维修 不满足");
            }
        }


        bool run_order_before(pk::ai_context@ context, pk::list<pk::person@>& out person_sel, pk::building@ building0)
        {
            pk::list<pk::person@> person_list = pk::get_idle_person_list(building0);
            if (building0.hp > uint16(0.85f * pk::get_max_hp(building0))) return false;//防止不必要的维修
            //无需下列判定，因为ai执行时已有该判定
            /*
            bool engage_enemy_unit = false;
            auto range = pk::range(building0.pos, 1, 5);
            for (int j = 0; j < int(range.length); j++)
            {
                pk::unit@ unit = pk::get_unit(range[j]);
                if (unit !is null and pk::is_enemy(building_AI, unit))
                    engage_enemy_unit = true;
            }
            if (!engage_enemy_unit) return false;*/
            if (check_avaliable(building0, person_list) != 0) return false;
            person_list.sort(function(a, b)//武将选择这块还得加强
            {
                bool a_skill = ch::has_skill(a, 特技_筑城);
                bool b_skill = ch::has_skill(b, 特技_筑城);
                if (a_skill and !b_skill) return true;
                if (!a_skill and b_skill) return false;

                return (a.stat[武将能力_政治] > b.stat[武将能力_政治]); // 무장 정렬 (지력순)
            });
            person_sel.add(person_list[0]);
            return true;
        }


        bool run_order(pk::list<pk::person@> person_sel, pk::building@ building0)
        {
            if (building0 is null) return false;
            if (person_sel[0] is null) return false;


            if (pk::is_in_screen(building0.pos))
            {
                switch (pk::rand(2))
                {
                case 0: pk::play_voice(0x39, person_sel[0]); break;   // 음성 : 설치
                case 1: pk::play_voice(0x3a, person_sel[0]); break;   // 음성 : 설치
                }
                pk::say(pk::encode("快把城墙补一补."), person_sel[0]);
            }

            int hp_heal = int(5 * person_sel[0].stat[武将能力_政治] * (ch::has_skill(person_sel[0], 特技_筑城)?1.5f:1.0f));
          
            base_ex[building0.get_id()].repair_done = true;//设置维修已完成
            
            ch::add_tp(force, 30, building0.pos);
            pk::add_hp(building0, hp_heal, true);
            //pk::trace("before energy:" + pk::get_energy(building0));
            pk::add_energy(building0, -ENERGY_COST_城墙维修, true);
            //pk::trace("after energy:" + pk::get_energy(building0));
            pk::add_gold(building0, -GOLD_COST_城墙维修, true);

            auto district = pk::get_district(building0.get_district_id());
            pk::add_ap(district, -ACTION_COST);

            person_sel[0].action_done = true;

            return true;

        }

    }

    Main main;
}
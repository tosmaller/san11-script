// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/02/01 # 江东新风 # 挑衅成功率参考伪报稍加改动,短期多次挑衅成功率大幅下降 ##
// ## 2022/01/29 # 江东新风 # 挑衅成功率参考扰乱，挑衅目标改为城中所有武将，城中武将过少时成功率大幅下降，论客优势 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/03/02 # 江东新风 # 将特殊地名重建的改写，cpp里调用kill_building都需要这样设定 ##
// ## 2021/01/14 # 江东新风 # 修复参数重名错误 ##
// ## 2021/01/05 # 江东新风 # 为攻城计略添加特殊地名设施争夺 ##
// ## 2020/12/19 # 江东新风 # 修复无目标和气力不足可用奇袭的bug ##  
// ## 2020/10/30 # 江东新风 # 同步马术书大神的更新 ##  	
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/16 # 氕氘氚  ##

namespace 部队指令_部队计略
{

    //---------------------------------------------------------------------------------------
    // 유저 설정 (true = on, false = off)

    // 계략 메뉴 추가여부 설정
    const bool 메뉴_계략_사용 = true;    // Custom 계략 에 대한 Master 설정 (false 시 모든 계략 off)
    const bool 계략_기습_사용 = true;    // 육상부대의 인근 적 부대 기습 : 특기_급습 우대 ('18.11.13)

    const bool 계략_도발_사용 = true;    // 적 태수 도발하여 출성 유도, 도발된 부대의 사기 저하 및 공격 유도 ('18.11.9)
    const bool 계략_도발_설전설정 = true;  //  발도/도발된 부대 간 설전을 벌임 ('18.11.24)
    const bool 계략_도발_설전보기 = true;  //  발도/도발된 부대 간 설전을 볼지 여부를 확인하기

    const bool 계략_공작_사용 = false;    // 적 건물 내구도 감소 ('18.11.9) 计略指令攻城，考虑到不平衡性及和其他功能冲突，故取消
    const bool 계략_공작_탈취 = true;    // 적 시설에 대해 공작 성공 시 시설 탈취 ('18.12.26)

    const bool 계략_낙석_사용 = true;    // 산악지형에서 낙석 실행 ('18.12.21)

    const bool 계략_고무_사용 = true;    // 아군 1부대 기력 회복 ('18.11.6)
    const bool 계략_대고무_사용 = true;    // 아군 복수부대 기력 회복 ('18.11.6)
    const bool 계략_회귀_사용 = true;    // 아군 부대 재행동 : 군주 또는 도독 사용 가능 ('18.11.6)


    // 계략에 대한 특기 옵션
    const bool 계략_특기_설정 = true;        // 계략에 대한 특기 설정 사용
    const bool 백출_반감_설정 = true;        // true= 50% 소비, false = 무조건 1 소비
    const bool 계략_병력_차등 = true;        // 병력수에 따른 계략 소비기력 차등 적용


    // 계략 실행 조건
    const int ENERGY_COST_기습 = 15;
    const int ENERGY_COST_낙석 = 20;
    const int ENERGY_COST_도발 = 15;
    const int ENERGY_COST_도발설전 = 15;
    const int ENERGY_COST_파괴공작 = 15;
    const int CHANCE_공작탈취확률 = 20;    // 적의 시설에 대해 공작 성공 시 시설 탈취 확률 (0 ~ 100 설정)
    const int ENERGY_COST_고무 = 15;
    const int ENERGY_COST_대고무 = 25;
    const int ENERGY_COST_회귀 = 20;

    // 강행군 실행 조건
    const int ENERGY_MIN_강행군 = 10;       // 강행군을 실행하기 위한 최소 기력
    const float MOVEMENT_강행군 = 0.6f;    // [강행군_이동력]/[부대_이동력] 비율 


    //---------------------------------------------------------------------------------------


    class Main
    {
        Main()
        {
            // 부대 경험치 획득 트리거
            unit_debate_list.clear();
            pk::bind(173, pk::trigger173_t(onUnitExpAdd));
            pk::bind(111, pk::trigger111_t(onTurnStart));
            pk::bind(112, pk::trigger112_t(onTurnEnd));
            pk::bind(102, pk::trigger102_t(onGameInit));

            add_menu_stragety();
        }


        pk::unit@ src_unit;
        pk::unit@ dst_unit;
        pk::point old_pos;
        pk::point src_pos_;
        pk::point dst_pos_;

        pk::unit@ src_unit_debate;
        pk::unit@ dst_unit_debate;

        pk::list<pk::unit@> unit_debate_list;
        int debate_result;


        void add_menu_stragety()
        {
            //-------------------------------------------------------------
            if (메뉴_계략_사용)
            {
                // 낙석 : 산악지형에서 낙석 실행
                if (계략_낙석_사용)
                {
                    pk::menu_item 계략_낙석;
                    계략_낙석.menu = 115;
                    계략_낙석.init = pk::unit_menu_item_init_t(init);
                    계략_낙석.get_text = pk::menu_item_get_text_t(getText_계략_낙석);
                    계략_낙석.get_desc = pk::menu_item_get_desc_t(getDesc_계략_낙석);
                    계략_낙석.is_visible = pk::menu_item_is_visible_t(isVisible_계략_낙석);
                    계략_낙석.is_enabled = pk::menu_item_is_enabled_t(isEnabled_계략_낙석);
                    계략_낙석.get_targets = pk::unit_menu_item_get_targets_t(getTargets_계략_낙석);
                    계략_낙석.get_chance = pk::unit_menu_item_get_chance_t(getChance_낙석);
                    계략_낙석.handler = pk::unit_menu_item_handler_t(handler_계략_낙석);
                    pk::add_menu_item(계략_낙석);
                }

                // 기습 : 근처의 적부대를 기습 공격
                if (계략_기습_사용)
                {
                    pk::menu_item 계략_기습;
                    계략_기습.menu = 115;
                    계략_기습.init = pk::unit_menu_item_init_t(init);
                    계략_기습.get_text = pk::menu_item_get_text_t(getText_계략_기습);
                    계략_기습.get_desc = pk::menu_item_get_desc_t(getDesc_계략_기습);
                    계략_기습.is_visible = pk::menu_item_is_visible_t(isVisible_계략_기습);
                    계략_기습.is_enabled = pk::menu_item_is_enabled_t(isEnabled_계략_기습);
                    계략_기습.get_targets = pk::unit_menu_item_get_targets_t(getTargets_계략_기습);
                    계략_기습.get_chance = pk::unit_menu_item_get_chance_t(getChance_100);
                    계략_기습.handler = pk::unit_menu_item_handler_t(handler_계략_기습);
                    pk::add_menu_item(계략_기습);
                }

                // 도발 : 적 거점 태수의 출성을 유도
                if (계략_도발_사용)
                {
                    pk::menu_item 계략_도발;
                    계략_도발.menu = 115;
                    계략_도발.init = pk::unit_menu_item_init_t(init);
                    계략_도발.get_text = pk::menu_item_get_text_t(getText_계략_도발);
                    계략_도발.get_desc = pk::menu_item_get_desc_t(getDesc_계략_도발);
                    계략_도발.is_visible = pk::menu_item_is_visible_t(isVisible_계략_도발);
                    계략_도발.is_enabled = pk::menu_item_is_enabled_t(isEnabled_계략_도발);
                    계략_도발.get_targets = pk::unit_menu_item_get_targets_t(getTargets_계략_도발);
                    계략_도발.get_chance = pk::unit_menu_item_get_chance_t(getChance_도발);
                    계략_도발.handler = pk::unit_menu_item_handler_t(handler_계략_도발);
                    pk::add_menu_item(계략_도발);
                }

                // 파괴 : 적 거점 내구도 파괴
                if (계략_공작_사용)
                {
                    pk::menu_item 계략_파괴공작;
                    계략_파괴공작.menu = 115;
                    계략_파괴공작.init = pk::unit_menu_item_init_t(init);
                    계략_파괴공작.get_text = pk::menu_item_get_text_t(getText_계략_파괴공작);
                    계략_파괴공작.get_desc = pk::menu_item_get_desc_t(getDesc_계략_파괴공작);
                    계략_파괴공작.is_visible = pk::menu_item_is_visible_t(isVisible_계략_파괴공작);
                    계략_파괴공작.is_enabled = pk::menu_item_is_enabled_t(isEnabled_계략_파괴공작);
                    계략_파괴공작.get_targets = pk::unit_menu_item_get_targets_t(getTargets_계략_파괴공작);
                    계략_파괴공작.get_chance = pk::unit_menu_item_get_chance_t(getChance_파괴공작);
                    계략_파괴공작.handler = pk::unit_menu_item_handler_t(handler_계략_파괴공작);
                    pk::add_menu_item(계략_파괴공작);
                }

                // 고무 : 1부대 기력 회복
                if (계략_고무_사용)
                {
                    pk::menu_item 계략_고무;
                    계략_고무.menu = 115;
                    계략_고무.init = pk::unit_menu_item_init_t(init);
                    계략_고무.get_text = pk::menu_item_get_text_t(getText_계략_고무);
                    계략_고무.get_desc = pk::menu_item_get_desc_t(getDesc_계략_고무);
                    계략_고무.is_visible = pk::menu_item_is_visible_t(isVisible_계략_고무);
                    계략_고무.is_enabled = pk::menu_item_is_enabled_t(isEnabled_계략_고무);
                    계략_고무.get_targets = pk::unit_menu_item_get_targets_t(getTargets_계략_고무);
                    계략_고무.get_chance = pk::unit_menu_item_get_chance_t(getChance_100);
                    계략_고무.handler = pk::unit_menu_item_handler_t(handler_계략_고무);
                    pk::add_menu_item(계략_고무);
                }

                // 대고무 : 대상+주위부대 기력 회복
                if (계략_대고무_사용)
                {
                    pk::menu_item 계략_대고무;
                    계략_대고무.menu = 115;
                    계략_대고무.init = pk::unit_menu_item_init_t(init);
                    계략_대고무.get_text = pk::menu_item_get_text_t(getText_계략_대고무);
                    계략_대고무.get_desc = pk::menu_item_get_desc_t(getDesc_계략_대고무);
                    계략_대고무.is_visible = pk::menu_item_is_visible_t(isVisible_계략_대고무);
                    계략_대고무.is_enabled = pk::menu_item_is_enabled_t(isEnabled_계략_대고무);
                    계략_대고무.get_targets = pk::unit_menu_item_get_targets_t(getTargets_계략_대고무);
                    계략_대고무.get_chance = pk::unit_menu_item_get_chance_t(getChance_100);
                    계략_대고무.handler = pk::unit_menu_item_handler_t(handler_계략_대고무);
                    pk::add_menu_item(계략_대고무);
                }

                // 회귀 : 기력 대폭 사용하여 행동완료 아군부대 재행동
                if (계략_회귀_사용)
                {
                    pk::menu_item 계략_회귀;
                    계략_회귀.menu = 115;
                    계략_회귀.init = pk::unit_menu_item_init_t(init);
                    계략_회귀.get_text = pk::menu_item_get_text_t(getText_계략_회귀);
                    계략_회귀.get_desc = pk::menu_item_get_desc_t(getDesc_계략_회귀);
                    계략_회귀.is_visible = pk::menu_item_is_visible_t(isVisible_계략_회귀);
                    계략_회귀.is_enabled = pk::menu_item_is_enabled_t(isEnabled_계략_회귀);
                    계략_회귀.get_targets = pk::unit_menu_item_get_targets_t(getTargets_계략_회귀);
                    계략_회귀.get_chance = pk::unit_menu_item_get_chance_t(getChance_100);
                    계략_회귀.handler = pk::unit_menu_item_handler_t(handler_계략_회귀);
                    pk::add_menu_item(계략_회귀);
                }
            }
        }


        void onUnitExpAdd(pk::unit@ unit, int type)
        {
            if (pk::is_campaign()) return;

            // 부대가 경험치 획득했을 때, 설전할 부대리스트에 있을 시 설전 발동
            if (unit_debate_list.contains(unit))
            {
                pk::scene(pk::scene_t(scene_도발설전)); // 설전 애니메이션 불러오기
                func_debate_result();                 // 부대설전 결과 반영

                unit_debate_list.clear();               // 리스트 초기화
            }

        }
        void onTurnStart(pk::force@ force)
        {
            if (pk::is_campaign()) return;
            unit_debate_list.clear();               // 리스트 초기화
        }
        void onTurnEnd(pk::force@ force)
        {
            if (pk::is_campaign()) return;
            unit_debate_list.clear();               // 리스트 초기화
        }
        void onGameInit()
        {
            if (pk::is_campaign()) return;
            unit_debate_list.clear();               // 리스트 초기화
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
        uint8 func_stragety_cost(int energy_cost)
        {
            if (!계략_특기_설정) return energy_cost;

            uint8 n = uint8(energy_cost);
            if (src_unit.has_skill(특기_백출))
            {
                if (백출_반감_설정)
                    n /= 2; // 기력 소모량 절반 감소
                else
                    n = 1;  // 기력 소모량 무조건 1로
            }

            //병력수에 따른 기력 소모량 차등 - 소부대 기력 소모 증가, 대부대 기력 소모 감소
            if (계략_병력_차등)
            {
                if (src_unit.troops <= 2000)
                    n = uint8(n * pk::min(10.f, 2000.f / src_unit.troops));         // 최대 10배 소모
                else if (src_unit.troops >= 10000)
                    n = uint8(n * pk::max(0.5f, (2 - src_unit.troops / 10000.f)));    // 최소 50% 소모
            }

            return n;
        }

        bool func_priority(pk::point dst_pos)
        {
            if (!계략_특기_설정) return false;

            if (src_unit.has_skill(특기_신산))
                return true;

            pk::unit@ dst_unit = pk::get_unit(dst_pos);
            if (pk::is_alive(dst_unit))
            {
                if (src_unit.has_skill(특기_허실) and (src_unit.attr.stat[部队能力_智力] > dst_unit.attr.stat[部队能力_智力]))
                    return true;
            }
            else if (!pk::is_alive(dst_unit) and pk::get_building(dst_pos) is null and src_unit.has_skill(특기_허실))
                return true;

            return false;
        }

        int func_range_add()
        {
            if (!계략_특기_설정) return 0;

            if (src_unit.has_skill(특기_귀모))
                return 1;

            return 0;
        }

        //---------------------------------------------------------------------------

        bool call_stragety_chance(pk::point dst_pos, int strategy_id)
        {
            int chance = (pk::int_bool(cast<pk::func206_t>(pk::get_func(206))(src_unit, src_pos_, dst_pos, strategy_id))).first;
            return pk::rand_bool(chance);
        }



        //---------------------------------------------------------------------------
        // 落石 계략_낙석
        //---------------------------------------------------------------------------

        int getChance_낙석(pk::point dst_pos)
        {
            return 100;
        }

        string getText_계략_낙석()
        {
            return pk::encode(pk::format("落石 ({})", func_stragety_cost(ENERGY_COST_낙석)));
        }

        string getDesc_계략_낙석()
        {
            if (src_unit.energy < func_stragety_cost(ENERGY_COST_낙석))
                return pk::encode("气力不足");
            else if (getTargets_계략_낙석().length == 0)
                return pk::encode("周围没有敌军");
            else
                return pk::encode("进行落石攻击");
        }

        bool isVisible_계략_낙석()
        {
            if (pk::is_campaign()) return false;
            return true;
        }

        bool isEnabled_계략_낙석()
        {
            if (src_unit.energy < func_stragety_cost(ENERGY_COST_낙석)) return false;
            return getTargets_계략_낙석().length > 0;
        }

        pk::array<pk::point_int> getTargets_계략_낙석()
        {
            pk::array<pk::point_int> targets;
            auto range = pk::range(src_pos_, 1, 1);
            for (int i = 0; i < int(range.length); i++)
            {
                pk::hex@ hex = pk::get_hex(range[i]);
                if (pk::is_enabled_terrain(hex.terrain) and hex.terrain == 지형_산
                    and get_height_diff(src_pos_, range[i]) >= 0
                    and !hex.has_building and (!hex.has_unit or src_unit.pos == range[i]))
                {
                    targets.insertLast(pk::point_int(range[i], 1));
                }
            }
            return targets;
        }

        bool handler_계략_낙석(pk::point dst_pos)
        {
            // 실행부대 기력 감소
            pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_낙석), true);

            if (func_priority(dst_pos) or pk::rand_bool(pk::min(100, pk::max(0, src_unit.attr.stat[部队能力_智力]))))
            {
                // 낙석 생성
                pk::create_building(pk::get_facility(시설_낙석), dst_pos, src_unit.get_force_id());
                pk::building@ building = pk::get_building(dst_pos);
                pk::complete(building);

                // 낙석 공격
                pk::attack(src_unit, pk::get_hex_object(dst_pos));


                if (pk::is_in_screen(src_pos_))
                {
                    switch (pk::rand(4))
                    {
                    case 0: pk::say(pk::encode("成功了"), pk::get_person(src_unit.leader)); break;
                    case 1: pk::say(pk::encode("用石头砸死敌军"), pk::get_person(src_unit.leader)); break;
                    case 2: pk::say(pk::encode("滚石木全都扔下去"), pk::get_person(src_unit.leader)); break;
                    case 3: pk::say(pk::encode("压垮敌方部队"), pk::get_person(src_unit.leader)); break;
                    }
                }

                // 기교점수
                ch::add_tp(pk::get_force(src_unit.get_force_id()), 10, dst_pos);
                // 능력경험치
                pk::add_stat_exp(src_unit, 武将能力_统率, 4);
                // 공적
                pk::add_kouseki(src_unit, 100);
            }
            else
            {
                if (pk::is_in_screen(src_pos_))
                {
                    switch (pk::rand(2))
                    {
                    case 0: pk::say(pk::encode("落石失败了"), pk::get_person(src_unit.leader)); break;
                    case 1: pk::say(pk::encode("没有可用的石头"), pk::get_person(src_unit.leader)); break;
                    }
                }
            }

            // 행동완료
            src_unit.action_done = true;
            if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
                src_unit.action_done = false;

            return true;
        }

        int get_height_diff(const pk::point& in src, const pk::point& in dst)
        {
            return pk::get_height_map(pk::hex_pos_to_height_map_pos(src) + 2).height - pk::get_height_map(pk::hex_pos_to_height_map_pos(dst) + 2).height;
        }







        //---------------------------------------------------------------------------
        // 奇袭 계략_기습 (삼국지10 오마쥬)
        //---------------------------------------------------------------------------

        string getText_계략_기습()
        {
            return pk::encode(pk::format("奇袭 ({})", func_stragety_cost(ENERGY_COST_기습)));
        }

        string getDesc_계략_기습()
        {
            if (src_unit.weapon > 병기_군마 and !pk::is_in_water(src_unit))
                return pk::encode("该兵种无法实行");
            else if (src_unit.weapon == 兵器_走舸 and pk::is_in_water(src_unit))
                return pk::encode("走舸无法实行");
            else if (src_unit.energy < ENERGY_COST_기습)
                return pk::encode("气力不足");
            else if (getTargets_계략_기습().length == 0)
                return pk::encode("周围没有敌军");
            else
                return pk::encode(pk::format("消耗气力执行奇袭 (必须 {} 气力)", ENERGY_COST_기습));
        }

        bool isVisible_계략_기습()
        {
            if (pk::is_campaign()) return false;
            //if (src_unit.weapon > 병기_군마 and !pk::is_in_water(src_unit)) return false;
            //else if (src_unit.weapon == 兵器_走舸 and pk::is_in_water(src_unit)) return false;
            return true;
        }

        bool isEnabled_계략_기습()
        {
            if (src_unit.weapon > 병기_군마 and !pk::is_in_water(src_unit)) return false;
            else if (src_unit.weapon == 兵器_走舸 and pk::is_in_water(src_unit)) return false;
            else if (src_unit.energy < ENERGY_COST_기습) return false;
            return getTargets_계략_기습().length > 0;
        }

        pk::array<pk::point_int> getTargets_계략_기습()
        {
            int movement = int(MOVEMENT_강행군 * src_unit.attr.stat[部队能力_移动]);

            // 이동 목표 지점 획득
            pk::array<pk::point_int> target_pos;
            auto range = pk::get_movable_pos(src_unit, src_pos_, movement); // 이동가능한 경로만 획득
            for (int i = 0; i < int(range.length); i++)
            {
                auto arr = pk::range(range[i], 1, 1);
                for (int j = 0; j < int(arr.length); j++)
                {
                    pk::unit@ target_unit = pk::get_unit(arr[j]);
                    if (target_unit !is null and pk::is_enemy(src_unit, target_unit) and pk::is_valid_pos(getEmptyPos(arr[j])))
                        target_pos.insertLast(pk::point_int(arr[j], 0));
                }
            }
            return target_pos;
        }

        pk::point getEmptyPos(pk::point dst_pos)
        {
            pk::point EmptyPos = -1;
            int pos_range = 0;

            auto arr = pk::range(dst_pos, 1, 1);
            for (int i = 0; i < int(arr.length); i++)
            {
                pk::hex@ hex = pk::get_hex(arr[i]);
                if (pk::is_enabled_terrain(hex.terrain) and !hex.has_building and !hex.has_unit and pos_range <= pk::get_distance(src_pos_, arr[i]))
                {
                    pos_range = pk::get_distance(src_pos_, arr[i]);
                    EmptyPos = arr[i];
                }
            }
            return EmptyPos;
        }

        bool handler_계략_기습(pk::point dst_pos)
        {
            pk::unit@ target_unit = pk::get_unit(dst_pos);

            // 기습 대상부대 주변 멀리있는 빈 칸 획득
            pk::point emptyPos = getEmptyPos(dst_pos);

            array<pk::point> steps;
            if (pk::is_valid_pos(emptyPos))
            {
                // 실행부대 기력 감소
                pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_기습), true);

                if (pk::is_in_screen(src_pos_))
                {
                    pk::play_se(0x39, src_pos_);
                    pk::create_effect(0x3a, src_pos_);
                }

                int stat_delta = 100;
                if (target_unit.leader < 武将_灵帝 or target_unit.leader > 무장_학자)
                    stat_delta = pk::max(0, src_unit.attr.stat[部队能力_智力] - target_unit.attr.stat[部队能力_智力]);

                if (func_priority(dst_pos) or pk::rand_bool(pk::min(100, 40 + pk::max(0, stat_delta)))
                    or (src_unit.has_skill(특기_급습) and !pk::is_in_water(src_unit))
                    or (src_unit.has_skill(특기_강습) and pk::is_in_water(src_unit)))
                {
                    if (pk::is_in_screen(src_pos_))
                    {
                        switch (pk::rand(2))
                        {
                        case 0: pk::play_voice(0x1C, pk::get_person(src_unit.leader)); break; // 음성 : 전투_공격
                        case 1: pk::play_voice(0x1D, pk::get_person(src_unit.leader)); break; // 음성 : 전투_공격
                        }
                    }

                    //---------------------------------------------
                    // 분대 임시 생성하여 공격하는 방식
                    pk::person@ src_leader = pk::get_person(src_unit.leader);
                    pk::person@ attacker = pk::get_person(무장_무관);

                    pk::force@ force = pk::get_force(src_unit.get_force_id());
                    int district_num = pk::get_district_id(force, 1);

                    // 분대 군단 설정
                    pk::set_district(attacker, district_num);
                    attacker.mibun = 身份_一般;
                    attacker.birth = pk::get_year() - 30;
                    attacker.death = pk::min(353, attacker.birth + 99);
                    attacker.appearance = pk::get_year() - 10;
                    // 능력치 : 본대의 80%
                    attacker.base_stat[武将能力_统率] = int(1.0f * src_leader.stat[武将能力_统率]);
                    attacker.base_stat[武将能力_武力] = int(1.0f * src_leader.stat[武将能力_武力]);
                    attacker.base_stat[武将能力_智力] = int(1.0f * src_leader.stat[武将能力_智力]);
                    attacker.base_stat[武将能力_政治] = int(1.0f * src_leader.stat[武将能力_政治]);
                    attacker.base_stat[武将能力_魅力] = int(1.0f * src_leader.stat[武将能力_魅力]);

                    int ground_weapon = pk::get_ground_weapon_id(src_unit);

                    // 능력치 반영.
                    attacker.update();

                    int troops = src_unit.troops;
                    int gold = 0;
                    int food = troops;

                    pk::unit@ atk_unit = pk::create_unit(pk::get_building(pk::get_service(src_unit)), attacker, null, null, troops, ground_weapon, 兵器_走舸, gold, food, emptyPos);
                    pk::set_energy(atk_unit, src_unit.energy);  // 본부대 기력값 반영

                    if (pk::is_in_screen(emptyPos))
                        pk::create_effect(0x4b, emptyPos);

                    pk::attack(atk_unit, target_unit);  // 적 부대에 공격 실행

                    if (pk::rand_bool(30))       // 30% 확률로 적 부대 혼란
                        pk::set_status(target_unit, atk_unit, 部队状态_混乱, 2, true);

                    ch::add_troops(src_unit, -(troops - atk_unit.troops) / 2, true);  // 分队兵力归队 분대 병력 본대 귀환

                    if (!pk::is_alive(atk_unit))
                        pk::reset(attacker);

                    ch::add_troops(atk_unit, -atk_unit.troops, false);
                    if (atk_unit.troops <= 0)
                    {
                        pk::kill(atk_unit);                 // 분대 해산
                        pk::reset(attacker);
                    }

                    //---------------------------------------------

                    if (pk::is_in_screen(src_pos_))
                    {
                        switch (pk::rand(4))
                        {
                        case 0: pk::say(pk::encode("大意了..."), pk::get_person(target_unit.leader)); break;
                        case 1: pk::say(pk::encode("敌人的奇袭?"), pk::get_person(target_unit.leader)); break;
                        case 2: pk::say(pk::encode("到底从何而来的?"), pk::get_person(target_unit.leader)); break;
                        case 3: pk::say(pk::encode("快振作起来."), pk::get_person(target_unit.leader)); break;
                        }
                        switch (pk::rand(4))
                        {
                        case 0: pk::say(pk::encode("成功奇袭敌军"), pk::get_person(src_unit.leader)); break;
                        case 1: pk::say(pk::encode("趁势扰乱敌军后方!"), pk::get_person(src_unit.leader)); break;
                        case 2: pk::say(pk::encode("我们上"), pk::get_person(src_unit.leader)); break;
                        case 3: pk::say(pk::encode("敌军被冲垮了"), pk::get_person(src_unit.leader)); break;
                        }
                    }

                    // 기교점수
                    ch::add_tp(pk::get_force(src_unit.get_force_id()), 20, dst_pos);
                    // 능력경험치
                    pk::add_stat_exp(src_unit, 武将能力_统率, 4);
                    // 공적
                    pk::add_kouseki(src_unit, 100);
                }
                else
                {
                    if (pk::is_in_screen(src_pos_))
                    {
                        switch (pk::rand(2))
                        {
                        case 0: pk::play_voice(0x45, pk::get_person(src_unit.leader)); break; // 음성 : 전투_계략간파
                        case 1: pk::play_voice(0x46, pk::get_person(src_unit.leader)); break; // 음성 : 전투_계략간파
                        }
                        pk::say(pk::encode("奇袭早被我看透了."), pk::get_person(target_unit.leader));
                        pk::say(pk::encode("被敌人发现了吗?"), pk::get_person(src_unit.leader));
                    }
                }
            }

            // 행동완료
            src_unit.action_done = true;
            if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
                src_unit.action_done = false;

            return true;
        }





        //---------------------------------------------------------------------------
        // 鼓舞 계략_고무 (삼국지 조조전 오마쥬)
        //---------------------------------------------------------------------------

        string getText_계략_고무()
        {
            return pk::encode(pk::format("鼓舞({})", func_stragety_cost(ENERGY_COST_고무)));  //guwu
        }

        string getDesc_계략_고무()
        {
            if (pk::get_person(src_unit.leader).rank >= 官职_无 and pk::get_person(src_unit.leader).mibun == 身份_一般)
                return pk::encode("将领官职不符合条件");
            else if (src_unit.energy < func_stragety_cost(ENERGY_COST_고무))
                return pk::encode("气力不足.");
            else if (getTargets_계략_고무().length == 0)
                return pk::encode("范围内没有我军部队");
            else
                return pk::encode("消耗气力, 恢复其他我方部队气力");
        }

        bool isVisible_계략_고무()
        {
            if (pk::is_campaign()) return false;
            if (pk::get_person(src_unit.leader).rank >= 官职_无 and pk::get_person(src_unit.leader).mibun == 身份_一般) return false;
            return true;
        }

        bool isEnabled_계략_고무()
        {
            if (pk::get_person(src_unit.leader).rank >= 官职_无 and pk::get_person(src_unit.leader).mibun == 身份_一般) return false;
            else if (src_unit.energy < func_stragety_cost(ENERGY_COST_고무)) return false;
            return getTargets_계략_고무().length > 0;
        }

        pk::array<pk::point_int> getTargets_계략_고무()
        {
            pk::array<pk::point_int> target_units;
            auto range = pk::range(src_pos_, 1, 1 + func_range_add());
            for (int i = 0; i < int(range.length); i++)
            {
                auto dst_unit = pk::get_unit(range[i]);
                if (dst_unit !is null and src_unit.pos != dst_unit.pos and src_unit.get_force_id() == dst_unit.get_force_id())
                    target_units.insertLast(pk::point_int(range[i], 1));
            }
            return target_units;
        }

        bool handler_계략_고무(pk::point dst_pos)
        {
            pk::unit@ target_unit = pk::get_unit(dst_pos);

            // 실행 부대 계략 실행 음성 효과, 실행/대상부대 대화
            if (pk::is_in_screen(src_pos_))
            {
                pk::play_voice(0x31, pk::get_person(src_unit.leader));  // 음성 : 계략2
                pk::play_se(0x39, src_pos_);
                pk::create_effect(0x3a, src_pos_);
            }
            // 실행 부대 기력 감소
            pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_고무), true);
            pk::say(pk::encode("给我方部队鼓舞士气!"), pk::get_person(src_unit.leader));

            // 대상부대 기력 회복 적용
            pk::add_energy(target_unit, (ENERGY_COST_고무 / 2), true);

            pk::say(pk::encode("感谢!我们会加把劲的!"), pk::get_person(target_unit.leader));

            // 효과 : 기력상승 효과
            if (pk::is_in_screen(target_unit.pos))
                pk::create_effect(0x3d, target_unit.pos);

            // 기교점수
            ch::add_tp(pk::get_force(src_unit.get_force_id()), 30, dst_pos);
            // 능력경험치
            pk::add_stat_exp(src_unit, 武将能力_智力, 4);
            // 공적
            pk::add_kouseki(src_unit, 100);

            // 행동완료
            src_unit.action_done = true;
            if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
                src_unit.action_done = false;

            return true;
        }




        //---------------------------------------------------------------------------
        // 大鼓舞 계략_대고무 (삼국지 조조전 오마쥬)
        //---------------------------------------------------------------------------

        string getText_계략_대고무()
        {
            return pk::encode(pk::format("大鼓舞 ({})", func_stragety_cost(ENERGY_COST_대고무)));  //da guwu
        }

        string getDesc_계략_대고무()
        {
            if (pk::get_person(src_unit.leader).rank >= 官职_无 and pk::get_person(src_unit.leader).mibun == 身份_一般)
                return pk::encode("官职不符合条件.");
            else if (src_unit.energy < func_stragety_cost(ENERGY_COST_대고무))
                return pk::encode("气力不足.");
            else if (getTargets_계략_대고무().length == 0)
                return pk::encode("范围内没有我军部队");
            else
                return pk::encode("耗尽气力, 恢复所选部队及周围部队气力");
        }

        bool isVisible_계략_대고무()
        {
            if (pk::is_campaign()) return false;
            if (pk::get_person(src_unit.leader).rank >= 官职_无 and pk::get_person(src_unit.leader).mibun == 身份_一般) return false;
            return true;
        }

        bool isEnabled_계략_대고무()
        {
            if (pk::get_person(src_unit.leader).rank >= 官职_无 and pk::get_person(src_unit.leader).mibun == 身份_一般) return false;
            else if (src_unit.energy < func_stragety_cost(ENERGY_COST_대고무)) return false;
            return getTargets_계략_대고무().length > 0;
        }

        pk::array<pk::point_int> getTargets_계략_대고무()
        {
            pk::array<pk::point_int> target_units;
            auto range = pk::range(src_pos_, 1, 2 + func_range_add());
            for (int i = 0; i < int(range.length); i++)
            {
                auto dst_unit = pk::get_unit(range[i]);
                if (dst_unit !is null and src_unit.pos != dst_unit.pos and src_unit.get_force_id() == dst_unit.get_force_id())
                    target_units.insertLast(pk::point_int(range[i], 1));
            }
            return target_units;
        }

        bool handler_계략_대고무(pk::point dst_pos)
        {
            pk::unit@ target_unit = pk::get_unit(dst_pos);

            // 실행 부대 계략 실행 음성 효과, 실행/대상부대 대화
            if (pk::is_in_screen(src_pos_))
            {
                pk::play_voice(0x31, pk::get_person(src_unit.leader));  // 음성 : 계략2
                pk::play_se(0x39, src_pos_);
                pk::create_effect(0x3a, src_pos_);
            }
            // 실행 부대 기력 감소
            pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_대고무), true);
            pk::say(pk::encode("擂鼓助威!"), pk::get_person(src_unit.leader));

            // 대상부대 기력 회복 적용
            pk::add_energy(target_unit, (ENERGY_COST_대고무 / 4), true);
            // 대상부대 주위 부대들의 기력 회복 적용
            auto range = pk::range(target_unit.pos, 1, 1);
            for (int i = 0; i < int(range.length); i++)
            {
                auto dst_unit = pk::get_unit(range[i]);
                if (dst_unit !is null and src_unit.pos != dst_unit.pos and src_unit.get_force_id() == dst_unit.get_force_id())
                {
                    pk::add_energy(dst_unit, (ENERGY_COST_대고무 / 4), true);

                    if (pk::is_in_screen(dst_unit.pos))
                    {
                        pk::say(pk::encode("士气旺盛,勇往直前!"), pk::get_person(dst_unit.leader));
                        // 효과 : 기력상승 효과
                        if (pk::is_in_screen(dst_unit.pos))
                            pk::create_effect(0x3d, dst_unit.pos);
                    }
                }
            }

            // 기교점수
            ch::add_tp(pk::get_force(src_unit.get_force_id()), 50, dst_pos);
            // 능력경험치
            pk::add_stat_exp(src_unit, 武将能力_智力, 6);
            // 공적
            pk::add_kouseki(src_unit, 150);

            // 행동완료
            src_unit.action_done = true;
            if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
                src_unit.action_done = false;

            return true;
        }




        //---------------------------------------------------------------------------
        // 攻城 계략_공작
        //---------------------------------------------------------------------------

        int getChance_파괴공작(pk::point dst_pos)
        {
            return 100;
        }

        string getText_계략_파괴공작()
        {
            return pk::encode(pk::format("攻城 ({})", func_stragety_cost(ENERGY_COST_파괴공작)));
        }

        string getDesc_계략_파괴공작()
        {
            if (src_unit.energy < func_stragety_cost(ENERGY_COST_파괴공작))
                return pk::encode("气力不足");
            else if (getTargets_계략_파괴공작().length == 0)
                return pk::encode("范围内没建筑");
            else
                return pk::encode("破坏敌军敌方建筑");
        }

        bool isVisible_계략_파괴공작()
        {
            if (pk::is_campaign()) return false;
            return true;
        }

        bool isEnabled_계략_파괴공작()
        {
            if (src_unit.energy < func_stragety_cost(ENERGY_COST_파괴공작)) return false;
            return getTargets_계략_파괴공작().length > 0;
        }

        pk::array<pk::point_int> getTargets_계략_파괴공작()
        {
            pk::array<pk::point_int> target_building;
            auto range = pk::range(src_pos_, 1, 1 + func_range_add());
            for (int i = 0; i < int(range.length); i++)
            {
                auto dst_building = pk::get_building(range[i]);
                if (dst_building !is null and pk::is_enemy(src_unit, dst_building))
                    target_building.insertLast(pk::point_int(range[i], 1));
            }
            return target_building;
        }


        bool handler_계략_파괴공작(pk::point dst_pos)
        {
            int hp_damage = int(100 + src_unit.attr.stat[部队能力_攻击] + 1.0 * pk::rand(src_unit.attr.stat[部队能力_攻击]));

            pk::building@ target_building = pk::get_building(dst_pos);

            // 실행 부대 계략 실행 음성 효과, 실행/대상부대 대화
            if (pk::is_in_screen(src_pos_))
            {
                pk::play_se(0x39, src_pos_);
                pk::create_effect(0x3a, src_pos_);
                switch (pk::rand(2))
                {
                case 0: pk::play_voice(0x1E, pk::get_person(src_unit.leader)); break;  // 음성 : 전투_시설공격1
                case 1: pk::play_voice(0x1F, pk::get_person(src_unit.leader)); break;  // 음성 : 전투_시설공격2
                }
            }

            // 실행 부대 기력 감소, 대상 거점 내구도 감소
            pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_파괴공작), true);

            pk::person@ taishu = pk::get_person(pk::get_taishu_id(target_building));
            if (!pk::is_alive(taishu) or func_priority(dst_pos) or (pk::rand_bool(pk::min(100, 40 + pk::max(0, src_unit.attr.stat[部队能力_智力] - taishu.stat[武将能力_智力])))))
            {
                pk::say(pk::encode("把墙拆了"), pk::get_person(src_unit.leader));

                int effect_mode = 0;

                // 공격부대 애니메이션 효과 ('18.11.10)
                if (pk::is_in_screen(src_pos_))
                {
                    pk::damage_info info;
                    info.src_pos = src_pos_;
                    info.dst_pos = dst_pos;
                    pk::unit_anim(src_unit, 15, { info });
                    pk::wait(1);
                    pk::stop_unit_anim();
                }

                // 降低目标耐久내구도 대미지
                pk::add_hp(target_building, -hp_damage, true);

                //攻占城市的尝试修正
                /*
                if (target_building.hp == 0)
                {
                    pk::damage_info info;
                    info.hp_damage = hp_damage;
                    info.dst_pos = target_building.get_pos();
                    cast<pk::func209_t>(pk::get_func(209))(info, src_unit, -1, target_building.get_pos(), 0, 2, false);
                    cast<pk::func168_t>(pk::get_func(168))(target_building, src_unit);
                    pk::set_district(target_building, pk::get_district(src_unit.get_district_id()), 0);
                }
                */
                //也要考虑特殊地名设施的处理
                if (target_building.get_id() >= 据点_末 and target_building.hp <= 0)
                {
                    if (pk::is_in_screen(src_pos_))
                    {
                        pk::play_se(0x71, target_building.pos);         //임팩트사운드
                        pk::create_effect(0x5c, target_building.pos);   //임팩트효과
                    }


                    auto dst_pos0 = target_building.pos;
                    auto dst_facility = pk::get_facility(target_building.facility);

                    pk::kill(target_building);

                    ch::特殊地名争夺处理(dst_facility.get_id(), src_unit.get_force_id(), dst_pos0);

                }

                // 일정 확률로 적 시설 탈취 : 논객 우대 ('18.12.26)
                if (계략_공작_탈취 and pk::is_alive(target_building) and !ch::is_valid_spec_id(target_building.get_id() - 100))//府兵所在不会被攻城变换所属
                {
                    if ((src_unit.has_skill(特技_论客) or pk::rand_bool(CHANCE_공작탈취확률)) and target_building.facility >= 시설_진 and target_building.facility <= 시설_성채)
                    {
                        int force_id = src_unit.get_force_id();
                        target_building.init_force = force_id;
                        string force_name = pk::decode(pk::get_name(pk::get_force(force_id)));
                        pk::say(pk::encode(pk::format("现在我们将追随\x1b[1x{}\x1b[0x军", force_name)), pk::get_person(무장_무관), target_building);
                    }
                }

                // 임팩트효과
                if (pk::is_in_screen(src_pos_) and pk::is_alive(target_building))
                {
                    if (pk::is_enemy(target_building, src_unit))    // 공작 일반 성공
                    {
                        pk::play_se(0x71, target_building.pos);         //임팩트사운드
                        pk::create_effect(0x21, target_building.pos);   //임팩트효과
                    }
                    else    // 탈취성공
                    {
                        pk::play_se(6);            //임팩트사운드   
                    }
                }

                // 기교점수
                ch::add_tp(pk::get_force(src_unit.get_force_id()), 30, dst_pos);
                // 능력경험치
                pk::add_stat_exp(src_unit, 武将能力_武力, 5);
                // 공적
                pk::add_kouseki(src_unit, 100);
            }
            else
            {
                pk::say(pk::encode("竟然失败了..."), pk::get_person(src_unit.leader));
            }

            // 행동완료
            src_unit.action_done = true;
            if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
                src_unit.action_done = false;

            return true;
        }





        //---------------------------------------------------------------------------
        // 挑? 계략_도발 (삼국지 조조전 오마쥬)
        //---------------------------------------------------------------------------

        int getChance_도발(pk::point dst_pos)
        {
            return 100;
        }

        string getText_계략_도발()
        {
            return pk::encode(pk::format("挑衅({})", func_stragety_cost(ENERGY_COST_도발)));
        }

        string getDesc_계략_도발()
        {
            if (src_unit.energy < func_stragety_cost(ENERGY_COST_도발))
                return pk::encode("气力不足");
            else if (getTargets_계략_도발().length == 0)
                return pk::encode("范围内没有敌方太守据点");
            else
                return pk::encode("消耗气力挑动敌方将领");
        }

        bool isVisible_계략_도발()
        {
            if (pk::is_campaign()) return false;
            return true;
        }

        bool isEnabled_계략_도발()
        {
            if (src_unit.energy < func_stragety_cost(ENERGY_COST_도발)) return false;
            return getTargets_계략_도발().length > 0;
        }

        pk::array<pk::point_int> getTargets_계략_도발()
        {
            pk::array<pk::point_int> targets;
            auto range = pk::range(src_pos_, 1, 2 + func_range_add());
            for (int i = 0; i < int(range.length); i++)
            {
                auto hex = pk::get_hex(range[i]);
                if (hex.has_building)
                {
                    auto dst_building = pk::get_building(range[i]);
                    if (dst_building !is null and pk::is_enemy(src_unit, dst_building))
                    {
                        if (pk::is_valid_base_id(dst_building.get_id()))
                        {
                            pk::list<pk::person@> list = pk::get_person_list(dst_building, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
                            if (list.count > 0)
                                targets.insertLast(pk::point_int(range[i], 1));
                        }
                    }

                }
                else if (hex.has_unit)
                {
                    auto enemy_unit = pk::get_unit(range[i]);
                    if (enemy_unit !is null and pk::is_enemy(src_unit, enemy_unit))
                        targets.insertLast(pk::point_int(range[i], 1));
                }
            }
            return targets;
        }



        bool handler_계략_도발(pk::point dst_pos)
        {
            pk::set_action_done(src_unit, true);
            // 거점 대상 도발
            auto hex = pk::get_hex(dst_pos);
            if (hex.has_building)
            {
                pk::building@ target_building = pk::get_building(dst_pos);
                int base_id = target_building.get_id();
                if (pk::is_alive(target_building) and pk::is_valid_base_id(base_id))
                {
                    // 실행 부대 계략 실행 음성 효과, 실행/대상부대 대화
                    if (pk::is_in_screen(src_pos_))
                    {
                        pk::play_voice(0x32, pk::get_person(src_unit.leader));  // 음성 : 계략1
                        pk::play_se(0x39, src_pos_);
                        pk::create_effect(0x3a, src_pos_);
                    }

                    // 실행 부대 기력 감소, 적 거점 태수 부대 생성
                    pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_도발), true);
                    //pk::list<pk::person@> member_list = ch::get_member_list(src_unit);
                    
                   // pk::int_bool result0 = STRATEGY_CHANCE::cal_strategy_chance(src_unit, member_list, dst_pos, 计略_伪报);//或许可以参考流言
                   //int rate = result0.first;

                    pk::person@gunshi = ch::get_best_stat_member(target_building,武将能力_智力);
                    int rate = get_provoke_rate(src_unit, gunshi);

                    if (src_unit.has_skill(特技_论客)) rate *= 2;
                    pk::list<pk::person@> list = pk::get_person_list(target_building, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
                    if (list.count == 1) rate /= 10;
                    else if (list.count == 2) rate /= 5;
                    //每一点cd降低15成功率上限
                    rate = pk::min(rate, int(100-base_ex[base_id].provoke_cd *15));
                    //挑衅概率重做，多次挑衅成功率降低（建筑物和部队设置被扰乱被挑衅的cd？随cd减少成功率逐渐恢复），被挑衅方智力加成计算，降低成功率上限？
                    if (pk::rand_bool(rate))
                    {
                        base_ex[base_id].provoke_cd = 5;
                        pk::say(pk::encode("别躲躲藏藏了,快出来吧!"), pk::get_person(src_unit.leader));

                        pk::point NearEmptyPos = func_EmptyPos(target_building);
                        if (pk::is_valid_pos(NearEmptyPos))
                        {
                            int weapon_id = 병기_검;
                            int default_troops = pk::min(5000, int(pk::get_troops(target_building) / 5));    // 기본 출진 병력수 설정
                            int target_troops = default_troops;

                            func_weapon_select(target_building, default_troops, weapon_id, target_troops);  // 병기 선택 함수 호출

                            // 출진 병력에 따른 병량 설정
                            int target_food = pk::min(int(target_troops * 1.5f), int(pk::get_food(target_building) / 2));

                            // 부대 출진에 따른 거점 병력,병량,무기 감소
                            ch::add_troops(target_building, -target_troops, false);
                            pk::add_food(target_building, -target_food, false);

                            if (weapon_id != 병기_검)
                                pk::add_weapon_amount(target_building, weapon_id, -target_troops, false);

                            // 도발된 부대 생성
                            @dst_unit = pk::create_unit(target_building, list[pk::rand(list.count)], null, null, target_troops, weapon_id, 兵器_走舸, 0, target_food, NearEmptyPos);
                            pk::say(pk::encode("我来会会你!"), pk::get_person(dst_unit.leader));

                            pk::set_order(dst_unit, 部队任务_攻击, src_unit.get_pos());

                            if (계략_도발_설전설정)
                            {
                                // 거리가 인접하면 공격 실행, 노병이면 원거리 공격 실행  距离临近时启动攻击，弩兵启动远程攻击  
                                func_fuck_atk(src_unit, dst_unit, 계략_도발_설전설정);

                                // 부대 주장 간 설전 部队主将之间的舌战
                                if (pk::is_alive(src_unit) and pk::is_alive(dst_unit))
                                {
                                    @dst_unit_debate = @dst_unit;
                                    @src_unit_debate = @src_unit;

                                    unit_debate_list.clear();
                                    unit_debate_list.add(dst_unit);
                                }
                            }

                            // 기교점수
                            ch::add_tp(pk::get_force(src_unit.get_force_id()), 30, dst_pos);
                            // 능력경험치
                            pk::add_stat_exp(src_unit, 武将能力_智力, 5);
                            // 공적
                            pk::add_kouseki(src_unit, 100);
                        }
                    }
                    else
                    {
                        if (base_ex[base_id].provoke_cd>0) pk::say(pk::encode("同样的计策难道还妄想我们会多次上当嘛？"), gunshi);
                        pk::say(pk::encode("挑衅失败"), pk::get_person(src_unit.leader));
                    }

                } // 거점 대상 도발
            }
            else if (hex.has_unit)
            {
                // 부대 대상 도발
                @ dst_unit = pk::get_unit(dst_pos);
                if (pk::is_alive(dst_unit))
                {
                    // 실행 부대 계략 실행 음성 효과, 실행/대상부대 대화
                    if (pk::is_in_screen(src_pos_))
                    {
                        pk::play_voice(0x32, pk::get_person(src_unit.leader));  // 음성 : 계략1
                        pk::play_se(0x39, src_pos_);
                        pk::create_effect(0x3a, src_pos_);
                    }

                    // 실행 부대 기력 감소, 적부대 기력 감소
                    pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_도발), true);
                    func_fuck_msg(src_unit, 0);     // 메세지_발동부대

                    // 지력차 계산
                    int stat_delta = 100;
                    if (dst_unit.leader < 武将_灵帝 or dst_unit.leader > 무장_학자)
                        stat_delta = pk::max(0, src_unit.attr.stat[部队能力_智力] - dst_unit.attr.stat[部队能力_智力]);

                    if (func_priority(dst_pos) or src_unit.has_skill(特技_论客) or (pk::rand_bool(pk::min(100, 40 + pk::max(0, stat_delta)))))
                    {
                        func_fuck_msg(dst_unit, 1);  // 메세지_성공_목표부대

                        pk::add_energy(dst_unit, -(src_unit.attr.stat[部队能力_智力] / 5), true);
                        pk::set_order(dst_unit, 部队任务_攻击, src_unit.get_pos());    // 발동부대를 공격 목표로 설정

                        if (계략_도발_설전설정)
                        {
                            // 거리가 인접하면 공격 실행, 노병이면 원거리 공격 실행
                            func_fuck_atk(src_unit, dst_unit, 계략_도발_설전설정);

                            // 부대 주장 간 설전
                            if (pk::is_alive(src_unit) and pk::is_alive(dst_unit))
                            {
                                @dst_unit_debate = @dst_unit;
                                @src_unit_debate = @src_unit;

                                unit_debate_list.clear();
                                unit_debate_list.add(dst_unit);
                            }
                        }


                        // 기교점수
                        ch::add_tp(pk::get_force(src_unit.get_force_id()), 30, dst_pos);
                        // 능력경험치
                        pk::add_stat_exp(src_unit, 武将能力_智力, 5);
                        // 공적
                        pk::add_kouseki(src_unit, 100);
                    }
                    else
                    {
                        func_fuck_msg(dst_unit, 2);  // 메세지_실패_목표부대
                    }
                }

            }

            // 행동완료
            src_unit.action_done = true;
            if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
                src_unit.action_done = false;

            return true;
        }

        int get_provoke_rate(pk::unit@ src, pk::person@ dst)
        {
            int dst_int = dst.stat[武将能力_智力];
            int src_int = src.attr.stat[部队能力_智力];
            if (ch::get_best_member_stat(src, 特技_神算, 武将能力_智力) > dst_int) return 100;

            int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, n = 0;
            b = data_849b24(dst.character);
            c = ch::get_best_member_stat(src, 部队能力_武力) / 20;

            d = src_int * 3 / 10 - dst_int / 5;
            d += b;
            d += c;

            e = a + d + 70;

            f = 100;
            f -= dst_int * 9 / 10;
            f *= src_int * src_int * 100;
            f /= dst_int * dst_int + src_int * src_int;
            f /= 55;
            f -= (100 - src_int) / 10;
            f += a;
            f += b;
            f += c;
            if (src_int < dst_int) f -= (dst_int - src_int) / 3;
            if (f < 1) f = 1;

            n = pk::max(1, pk::min(99, e, f));

            return n;
        }

        /** 성격에 따른 위보 상수 */
        int data_849b24(int character)
        {
            switch (character)
            {
            case 성격_소심: return 3;
            case 성격_냉정: return 1;
            case 성격_대담: return 0;
            case 성격_저돌: return -2;
            }
            return 0;
        }

        void func_fuck_atk(pk::unit@ attacker, pk::unit@ target, bool 효과적용)
        {
            if (효과적용)
            {
                if (pk::get_distance(target.pos, attacker.pos) == 1)
                    pk::attack(target, attacker);
                else if (pk::get_distance(target.pos, attacker.pos) == 2 and target.weapon == 병기_노)
                    pk::attack(target, attacker);
            }
        }

        // 계략_도발용 함수 : 건물과 발동부대 주변 빈 칸 찾기
        pk::point func_EmptyPos(pk::building@ building)
        {
            int max_range = 3;
            auto range = pk::range(building.get_pos(), 1, max_range);

            for (int j = 1; j <= int(max_range); j++)     // 발동부대와의 거리, 가까이부터 멀리
            {
                for (int i = 0; i < int(range.length); i++) // 거점과의 거리, 가까이부터 멀리
                {
                    pk::hex@ hex = pk::get_hex(range[i]);
                    if (!hex.has_building and !hex.has_unit and (pk::get_distance(range[i], src_unit.pos) <= j))
                        return range[i];    // 조건 만족 시 해당 위치 반환
                }
            }
            return -1;
        }

        // 계략_도발용 함수 : 무기 선택 (최적화 아님, 가장 많이 보유한 무기로 설정)
        void func_weapon_select(pk::building@ building, int troops, int& out weapon_sel, int& out troops_sel)
        {
            int weapon_max = 0;

            weapon_sel = 0;
            troops_sel = 0;

            for (int i = 병기_창; i <= 병기_군마; i++)
            {
                int weapon_t = pk::get_weapon_amount(building, i);
                if (2000 <= weapon_t and weapon_max <= weapon_t)
                {
                    weapon_max = weapon_t;
                    weapon_sel = i;
                    troops_sel = pk::min(troops, weapon_max);
                }
            }

            if (weapon_sel == 0)
            {
                troops_sel = troops;
            }
        }

        // 계략_도발 욕설용 함수 : 랜덤 메세지 세트
        void func_fuck_msg(pk::unit@ msg_unit, int msg_case)
        {
            pk::person@ leader = pk::get_person(msg_unit.leader);

            if (msg_case == 0)  // 성공_발동부대
            {
                switch (pk::rand(4))
                {
                case 0: pk::say(pk::encode("你还是太年轻了"), leader); break;
                case 1: pk::say(pk::encode("真是头脑简单"), leader); break;
                case 2: pk::say(pk::encode("真是莽撞..."), leader); break;
                case 3: pk::say(pk::encode("受死..."), leader); break;
                }
            }
            else if (msg_case == 1)  // 성공_목표부대
            {
                switch (pk::rand(4))
                {
                case 0: pk::say(pk::encode("势要取汝之项上人头"), leader); break;
                case 1: pk::say(pk::encode("你在那等着!"), leader); break;
                case 2: pk::say(pk::encode("给我杀了他们!"), leader); break;
                case 3: pk::say(pk::encode("给我冲!"), leader); break;
                }
            }
            else if (msg_case == 2)  // 실패_목표부대
            {
                switch (pk::rand(4))
                {
                case 0: pk::say(pk::encode("无视他."), leader); break;
                case 1: pk::say(pk::encode("别管他."), leader); break;
                case 2: pk::say(pk::encode("当作犬吠..."), leader); break;
                case 3: pk::say(pk::encode("....."), leader); break;
                }
            }
        }

        void scene_도발설전()
        {
            pk::person@ p0 = pk::get_person(src_unit_debate.leader);
            pk::person@ p1 = pk::get_person(dst_unit_debate.leader);
            string p0_name = pk::decode(pk::get_name(p0));
            string p1_name = pk::decode(pk::get_name(p1));

            pk::cutin(20);  // 컷인: 노려보기
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,你能成为我的对手嘛", p1_name)), p0);
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,你才不是我的对手.", p0_name)), p1);
            debate_result = pk::debate(p0, p1, p0.is_player(), p1.is_player(), false, 계략_도발_설전보기).first;
        }

        void func_debate_result()
        {
            // 설전 결과 반영
            if (debate_result == 0)     // 거는 쪽이 이긴 경우
            {
                func_debate_msg(src_unit_debate, 0);   // 메세지_승리
                func_debate_msg(dst_unit_debate, 1);  // 메세지_패전

                pk::add_energy(src_unit_debate, +2 * ENERGY_COST_도발설전, true);
                pk::add_energy(dst_unit_debate, -2 * ENERGY_COST_도발설전, true);

                pk::add_loyalty(pk::get_person(dst_unit_debate.leader), -5);   // 목표부대 주장의 충성도를 낮춤

            }
            else if (debate_result == 1)    // 받는 쪽이 이긴 경우
            {
                func_debate_msg(dst_unit_debate, 0);   // 메세지_승리
                func_debate_msg(src_unit_debate, 1);  // 메세지_패전

                pk::add_energy(dst_unit_debate, +2 * ENERGY_COST_도발설전, true);
                pk::add_energy(src_unit_debate, -2 * ENERGY_COST_도발설전, true);

                pk::add_loyalty(pk::get_person(src_unit_debate.leader), -5);   // 발동부대 주장의 충성도를 낮춤
            }
        }

        // 야전_설전용 함수 : 랜덤 메세지 세트
        void func_debate_msg(pk::unit@ msg_unit, int msg_case)
        {
            pk::person@ leader = pk::get_person(msg_unit.leader);

            if (msg_case == 0)  // 승리부대
            {
                switch (pk::rand(4))
                {
                case 0: pk::say(pk::encode("我们赢了!"), leader); break;
                case 1: pk::say(pk::encode("庆祝胜利!"), leader); break;
                case 2: pk::say(pk::encode("再前进一点!"), leader); break;
                case 3: pk::say(pk::encode("还是我军占了上风.."), leader); break;
                }
            }
            else if (msg_case == 1)  // 패전부대
            {
                switch (pk::rand(4))
                {
                case 0: pk::say(pk::encode("竟然输了.."), leader); break;
                case 1: pk::say(pk::encode("快撤退!"), leader); break;
                case 2: pk::say(pk::encode("我不行了..."), leader); break;
                case 3: pk::say(pk::encode("这种请况.."), leader); break;
                }
            }
            else if (msg_case == 2)  // 실패상황
            {
                switch (pk::rand(4))
                {
                case 0: pk::say(pk::encode("无视他!"), leader); break;
                case 1: pk::say(pk::encode("别管他..."), leader); break;
                case 2: pk::say(pk::encode("当作犬吠..."), leader); break;
                case 3: pk::say(pk::encode("不动如山..."), leader); break;
                }
            }
        }





        //---------------------------------------------------------------------------
        // 回归  계략_회귀 (삼국지 조조전 오마쥬)
        //---------------------------------------------------------------------------

        string getText_계략_회귀()
        {
            return pk::encode(pk::format("回归 ({})", func_stragety_cost(ENERGY_COST_회귀)));
        }

        string getDesc_계략_회귀()
        {
            if (pk::get_person(src_unit.leader).mibun == 身份_一般 and !src_unit.has_skill(특기_지도))
                return pk::encode("不是君主或都督,没有特技指导");
            else if (src_unit.energy < func_stragety_cost(ENERGY_COST_회귀))
                return pk::encode("气力不足");
            else if (getTargets_계략_회귀().length == 0)
                return pk::encode("范围内没有我军完成行动的部队");
            else
                return pk::encode("大幅消耗该部队气力选中部队再行动");
        }

        bool isVisible_계략_회귀()
        {
            if (pk::is_campaign()) return false;
            if (pk::get_person(src_unit.leader).mibun == 身份_一般 and !src_unit.has_skill(특기_지도)) return false;
            return true;
        }

        bool isEnabled_계략_회귀()
        {
            if (pk::get_person(src_unit.leader).mibun == 身份_一般 and !src_unit.has_skill(특기_지도)) return false;
            else if (src_unit.energy < func_stragety_cost(ENERGY_COST_회귀)) return false;
            return getTargets_계략_회귀().length > 0;
        }

        pk::array<pk::point_int> getTargets_계략_회귀()
        {
            pk::person@ src_leader = pk::get_person(src_unit.leader);
            pk::array<pk::point_int> target_units;
            int src_官阶 = 0;
            if (src_leader.mibun != 身分_君主)
                src_官阶 = src_leader.rank / 8;
            auto range = pk::range(src_pos_, 1, 1 + func_range_add());
            for (int i = 0; i < int(range.length); i++)
            {
                auto dst_unit = pk::get_unit(range[i]);
                if (dst_unit !is null and src_unit.pos != dst_unit.pos and src_unit.get_force_id() == dst_unit.get_force_id() and dst_unit.action_done == true and dst_unit.status == 部队状态_通常)
                {
                    pk::person@ dst_leader = pk::get_person(dst_unit.leader);
                    if (dst_leader.mibun == 身分_君主) continue;    //  对君主无效
                    int dst_官阶 = dst_leader.rank / 8;
                    if (src_官阶 >= dst_官阶) continue;    //  只能对官职低与自己的使用
                    target_units.insertLast(pk::point_int(range[i], 1));
                }
            }
            return target_units;
        }

        bool handler_계략_회귀(pk::point dst_pos)
        {
            pk::unit@ target_unit = pk::get_unit(dst_pos);

            // 실행 부대 계략 실행 음성 효과, 실행/대상부대 대화
            if (pk::is_in_screen(src_pos_))
            {
                pk::play_voice(0x37, pk::get_person(src_unit.leader));  // 음성 : 전투_진정
                pk::play_se(0x39, src_pos_);
                pk::create_effect(0x3a, src_pos_);
            }
            // 실행 부대 기력 감소
            pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_회귀), true);
            pk::say(pk::encode("按照我的计策部队再度行动!!"), pk::get_person(src_unit.leader));

            // 재행동 설정
            pk::say(pk::encode("快动身别犹豫了!"), pk::get_person(target_unit.leader));
            target_unit.action_done = false;    // 대상부대 행동재개
            pk::set_remain_movement(target_unit, target_unit.attr.stat[部队能力_移动]);
            // 효과 : 
            if (pk::is_in_screen(target_unit.pos))
                pk::create_effect(0x3a, target_unit.pos);

            // 기교점수
            ch::add_tp(pk::get_force(src_unit.get_force_id()), 30, dst_pos);
            // 능력경험치
            pk::add_stat_exp(src_unit, 武将能力_智力, 4);
            // 공적
            pk::add_kouseki(src_unit, 100);

            // 행동완료
            src_unit.action_done = true;
            if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
                src_unit.action_done = false;

            return true;
        }











        // -----------------------------------------------------------


    }

    Main main;
}
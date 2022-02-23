// ## 2021/12/05 # 江东新风 # 技巧不足时不出现 ##
// ## 2021/09/26 # 江东新风 # 语句优化2 ##
// ## 2021/02/17 # 江东新风 # 语句优化 ##
// ## 2020/11/17 # 江东新风 # trace 信息乱码修复 ##
// ## 2020/10/30 # 江东新风 # 同步马术书大神的更新 ##  
// ## 2020/07/26 ##
/*
※ 주의사항 : 본 스크립트의 기본 COST 변경 적용 시 SCENARIO 파일에 설정된 값을 무시하고 설정됩니다.

@만든이: 기마책사
@Update: '19.1.10  / 최초적용: 기교연구 필요값 설정변경 기능 추가,   매턴 기교연구 우대기능 추가

*/

namespace 기교연구설정
{
    //---------------------------------------------------------------------------------------
    // 유저 설정 (true = on, false = off)

    // 기본 기교연구 설정값 변경 (※ 주의사항 확인 후 사용할 것 !!)
    const bool 기교_GOLD_COST_변경 = false;
    const bool 기교_TP_COST_변경 = false;
    const bool 기교_TIME_COST_변경 = false;

    const uint16 기교LV1_GOLD_COST = 1000;        // 표준: 1000
    const uint16 기교LV1_TP_COST = 1000;        // 표준: 1000
    const uint16 기교LV1_TIME_COST = 3;           // 표준: 3

    const uint16 기교LV2_GOLD_COST = 2000;        // 표준: 2000
    const uint16 기교LV2_TP_COST = 2000;        // 표준: 2000
    const uint16 기교LV2_TIME_COST = 4;           // 표준: 4    

    const uint16 기교LV3_GOLD_COST = 5000;        // 표준: 5000
    const uint16 기교LV3_TP_COST = 3000;        // 표준: 3000
    const uint16 기교LV3_TIME_COST = 6;           // 표준: 6

    const uint16 기교LV4_GOLD_COST = 10000;       // 표준: 10000
    const uint16 기교LV4_TP_COST = 5000;        // 표준: 5000
    const uint16 기교LV4_TIME_COST = 9;           // 표준: 9

    const bool 메시지_LOG창_표시 = false;           // launcher log 창에 변경 정보 표시

    //---------------------------------------------------------------------------------------

    // 일정 확률로 해당 턴에 기교연구 우대 적용
    const bool 기교_COST_우대설정 = true;
    const int 技巧消耗_优惠几率 = 30;             // 0~100 사이값 사용---因为加入了各种不出现情况，所以略微提高触发率


    //---------------------------------------------------------------------------------------

    class Main
    {
        // 유저설정용 기교설정 초기값
        array<array<uint16>> custom_tech_cost = { {기교LV1_GOLD_COST, 기교LV1_TP_COST, 기교LV1_TIME_COST} ,
                                                  {기교LV2_GOLD_COST, 기교LV2_TP_COST, 기교LV2_TIME_COST} ,
                                                  {기교LV3_GOLD_COST, 기교LV3_TP_COST, 기교LV3_TIME_COST} ,
                                                  {기교LV4_GOLD_COST, 기교LV4_TP_COST, 기교LV4_TIME_COST} };
        // 기교설정값 (우대모드 활용)
        array<uint16> tech_cost_gold(기교_끝, 0);
        array<uint16> tech_cost_tp(기교_끝, 0);
        array<uint16> tech_cost_time(기교_끝, 0);



        Main()
        {
            pk::bind(102, pk::trigger102_t(onGameInit));
            pk::bind(111, pk::trigger111_t(onTurnStart));
            pk::bind(112, pk::trigger112_t(onTurnEnd));

            pk::bind(100, pk::trigger100_t(onGameTitle));
        }

        //---------------------------------------------------------------------------------------


        // 타이틀 화면
        void onGameTitle()
        {
            onGameInit();
        }


        // 게임초기화 시 유저설정값 적용
        void onGameInit()
        {
            if (pk::is_campaign()) return;
            // 유저설정값 적용
            if (기교_GOLD_COST_변경 or 기교_TP_COST_변경 or 기교_TIME_COST_변경)
            {
                for (int tech_id = 0; tech_id < 기교_끝; tech_id++)
                {
                    int tech_lv = tech_id % 4;
                    if (기교_GOLD_COST_변경) change_tech_gold(tech_id, custom_tech_cost[tech_lv][0], false);
                    if (기교_TP_COST_변경)   change_tech_tp(tech_id, custom_tech_cost[tech_lv][1], false);
                    if (기교_TIME_COST_변경) change_tech_time(tech_id, custom_tech_cost[tech_lv][2], false);
                }
            }

            // 기교 설정값 백업
            backup_tech_cost();
        }

        //---------------------------------------------------------------------------------------

        // 매턴 종료 시 기교 설정값 원복
        void onTurnEnd(pk::force@ force)
        {
            if (pk::is_campaign()) return;
            reset_tech_cost();
        }

        // 매턴 시작 시 랜덤하게 기교 우대 설정
        void onTurnStart(pk::force@ force)
        {
            if (pk::is_campaign()) return;
            if (!pk::is_normal_force(force)) return;

            if (!기교_COST_우대설정) return;

            // 연구 중인 기교가 있는지 확인
            int tech_rnd = force.researching_tech;
            if (tech_rnd >= 0)
            {
                //已经在研究时不触发
                if (force.is_player())
                {
                    //string tech_name = pk::decode(pk::get_name(pk::get_tech(tech_rnd)));
                    //pk::message_box(pk::encode(pk::format("我本想帮帮你的,原来你已经在研究\x1b[1x{},\x1b[0x了啊，那下次再见吧。", tech_name)),pk::get_person(무장_사마휘));
                    return;
                }
                return;
            }

            // 기교 우대 적용
            if (pk::rand_bool(技巧消耗_优惠几率))
            {
                int tech_group = pk::rand(9);   // 9개 분야 중 랜덤 선택
                int tech_id = get_next_tech_id(force, tech_group);  // 선택 분야 중 미연구 단계의 기교ID 반환
                int adv_type = pk::rand(3);     // 随机优化时间金，技巧，时间 우대할 금/TP/기간 중 랜덤 선택

                if (tech_id < 0)
                {
                    //该领域已研究完，不触发
                    /*
                    if (force.is_player())
                    {
                        pk::array<string> tech_group_name = { "枪兵","戟兵","弓兵","骑兵","练兵","发明","防御","火攻","政治" };
                        string group_name = tech_group_name[tech_group];
                        pk::message_box(pk::encode(pk::format("我本想帮帮你的,\x1b[1x{}\x1b[0x领域已经研究完毕了.下次再见.", group_name)),
                            pk::get_person(무장_사마휘));
                    }*/
                    return;
                }
                else
                {
                    //当技巧明显不足时，不来
                    uint16 cost = pk::get_tech(tech_id).tp_cost;
                    if (force.tp < (cost - 50 - ((adv_type == 1)?cost/2:0))) return;

                    string tech_name = pk::decode(pk::get_name(pk::get_tech(tech_id)));
                    string adv_name;

                    int tech_lv = tech_id % 4;
                    switch (adv_type)
                    {
                    case 0: change_tech_gold(tech_id, tech_cost_gold[tech_id], true); adv_name = "资金";   break;
                    case 1: change_tech_tp(tech_id, tech_cost_tp[tech_id], true); adv_name = "技巧点";   break;
                    case 2: change_tech_time(tech_id, tech_cost_time[tech_id], true); adv_name = "时间";  break;
                    }

                    if (force.is_player())
                    {
                        pk::message_box(pk::encode(pk::format("今天专程来帮你研究\x1b[1x{}\x1b[0x。这次就花费一半\x1b[1x{}\x1b[0x吧。\n要现在就开始研究哦，不然我可就不在了。", tech_name, adv_name)),
                            pk::get_person(무장_사마휘));
                    }
                }
            }
        }



        //---------------------------------------------------------------------------------------

        void change_tech_gold(int tech_id, uint16 gold_cost, bool advantage)
        {
            pk::tech@ tech = pk::get_tech(tech_id);
            uint16 cost = tech.gold_cost;
            tech.gold_cost = gold_cost / (advantage ? 2 : 1);

            if (기교_GOLD_COST_변경 and 메시지_LOG창_표시)
                pk::info(pk::format("技巧{} 需要资金:{}{}", pk::decode(pk::get_name(tech)), cost, gold_cost));
        }

        void change_tech_tp(int tech_id, uint16 tp_cost, bool advantage)
        {
            pk::tech@ tech = pk::get_tech(tech_id);
            uint16 cost = tech.tp_cost;
            tech.tp_cost = tp_cost / (advantage ? 2 : 1);

            if (기교_TP_COST_변경 and 메시지_LOG창_표시)
                pk::info(pk::format("技巧{} 需要技巧点:{}{}", pk::decode(pk::get_name(tech)), cost, tp_cost));
        }

        void change_tech_time(int tech_id, uint16 time_cost, bool advantage)
        {
            pk::tech@ tech = pk::get_tech(tech_id);
            uint16 cost = tech.time_cost;
            tech.time_cost = time_cost / (advantage ? 2 : 1) + (((time_cost % (advantage ? 2 : 1)) == 0) ? 0 : 1);

            if (기교_TIME_COST_변경 and 메시지_LOG창_표시)
                pk::info(pk::format("技巧{} 需要时间:{}{}", pk::decode(pk::get_name(tech)), cost, time_cost));
        }


        //---------------------------------------------------------------------------------------


        int get_next_tech_id(pk::force@ force, int tech_group)
        {
            int tech_id = -1;
            for (int tech_lv = 0; tech_lv < 4; tech_lv++)
            {
                int tech_temp = 4 * tech_group + tech_lv;

                if (!pk::has_tech(force, tech_temp))
                {
                    tech_id = tech_temp;
                    return tech_id;
                }
            }
            return tech_id;
        }


        void backup_tech_cost()
        {
            for (int tech_id = 0; tech_id < 기교_끝; tech_id++)
            {
                pk::tech@ tech = pk::get_tech(tech_id);

                tech_cost_gold[tech_id] = tech.gold_cost;
                tech_cost_tp[tech_id] = tech.tp_cost;
                tech_cost_time[tech_id] = tech.time_cost;
            }
        }

        void reset_tech_cost()
        {
            for (int tech_id = 0; tech_id < 기교_끝; tech_id++)
            {
                pk::tech@ tech = pk::get_tech(tech_id);

                tech.gold_cost = tech_cost_gold[tech_id];
                tech.tp_cost = tech_cost_tp[tech_id];
                tech.time_cost = tech_cost_time[tech_id];
            }
        }

        //---------------------------------------------------------------------------------------

    }

    Main main;
}


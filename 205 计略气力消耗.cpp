// ## 2021/10/03 # 江东新风 # 因兵力少而达到的气力增加上限为当前部队的最大气力 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/09/17 #江东新风#删除重复常量##
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/07/26 ##
namespace STRATEGY_ENERGY_COST
{

    class Main
    {
        Main()
        {
            pk::set_func(205, pk::func205_t(callback));

            pk::bind(100, pk::trigger100_t(onGameTitle));   // 특기 설명 수정을 위해 추가
        }

        int callback(pk::unit@ src, int strategy_id)
        {
            /* 원본 기력 소모량 (백출)
            if (ch::has_skill(src, 특기_백출))
                return 1;
            */

            // 특기 소비 기력
            int n = 0;
            switch (strategy_id)
            {
            case 계략_화계:
                n = 计略耗气_火计;
                break;
            case 계략_소화:
                n = 计略耗气_灭火;
                break;
            case 계략_위보:
                n = 计略耗气_伪报;
                break;
            case 계략_교란:
                n = 计略耗气_扰乱;
                break;
            case 계략_진정:
                n = 计略耗气_镇静;
                break;
            case 계략_복병:
                n = 计略耗气_伏兵;
                break;
            case 계략_동토:
                n = 计略耗气_内讧; // 원본 기력 소모량 (20)
                break;
            case 계략_요술:
                n = 计略耗气_妖术; // 원본 기력 소모량 (50) 
                break;
            case 계략_낙뢰:
                n = 计略耗气_落雷; // 원본 기력 소모량 (50) 
                break;
            }

            //---------------------------------------------------------------------------------------
            //기력 소모량 감소 효과 ('18.10.4)

            if (ch::has_skill(src, 특기_백출))
            {
                if (百出耗气规则 == 0)
                    return 1;
                else if (百出耗气规则 == 1)
                    n /= 2; // 기력 소모량 절반 감소
                else {       // 自定义气力消耗
                    switch (strategy_id)
                    {
                    case 계략_화계:
                        n = 百出耗气_火计; break;
                    case 계략_소화:
                        n = 百出耗气_灭火; break;
                    case 계략_위보:
                        n = 百出耗气_伪报; break;
                    case 계략_교란:
                        n = 百出耗气_扰乱; break;
                    case 계략_진정:
                        n = 百出耗气_镇静; break;
                    case 계략_복병:
                        n = 百出耗气_伏兵; break;
                    case 계략_동토:
                        n = 百出耗气_内讧; break;
                    case 계략_요술:
                        n = 百出耗气_妖术; break;
                    case 계략_낙뢰:
                        n = 百出耗气_落雷; break;
                    }
                }
            }

            if (计略特技减少耗气)
            {
                //  精妙
                if (ch::has_skill(src, 특기_정묘))
                    n = n - 气力减少量_精妙; // 기력 소모량 감소 (-3)
                //  秘计
                if (ch::has_skill(src, 특기_비책))
                    n = n - 气力减少量_秘计; // 기력 소모량 감소 (-5)
                //  火攻/火神 使用 火计/灭火
                if ((ch::has_skill(src, 특기_화공) or ch::has_skill(src, 특기_화신)) and ((strategy_id == 계략_화계) or (strategy_id == 계략_소화)))
                    n /= 2; // 화계 특기자 화계 기력 소모량 절반 감소
                //  埋伏 使用 伏兵
                if ((ch::has_skill(src, 특기_매복)) and (strategy_id == 계략_복병))
                    n /= 2; // 매복 특기자 복병 기력 소모량 절반 감소
                //  言毒 使用 伪报
                if ((ch::has_skill(src, 특기_언독)) and (strategy_id == 계략_위보))
                    n /= 2; // 언독 특기자 위보 기력 소모량 절반 감소
                //  詭计 使用 内讧
                if ((ch::has_skill(src, 특기_궤계)) and (strategy_id == 계략_동토))
                    n /= 2; // 궤계 특기자 동토 기력 소모량 절반 감소
                //  机智 使用 扰乱
                if ((ch::has_skill(src, 특기_기략)) and (strategy_id == 계략_교란))
                    n /= 2; // 기략 특기자 교란 기력 소모량 절반 감소
                //  规律/沉着/明鏡 使用 镇静
                if ((ch::has_skill(src, 특기_규율) or ch::has_skill(src, 특기_침착) or ch::has_skill(src, 특기_명경)) and (strategy_id == 계략_진정))
                    n /= 2; // 규율,침착,명경 특기자 진정 기력 소모량 절반 감소
            }

            //---------------------------------------------------------------------------------------
            //병력수에 따른 기력 소모량 차등 - 소부대 기력 소모 증가, 대부대 기력 소모 감소 ('18.10.11)
            if (兵力影响耗气)
            {
                int max_energy = src.has_tech(技巧_熟练兵)? int(pk::core["部队.熟练兵最大气力"]) : int(pk::core["部队.最大气力"]);//是否有必要？或者直接变成100？
                if (src.troops <= 2000)
                    n = int(pk::min(n * pk::min(10.f, 2000.f / pk::max(1, src.troops)), float(max_energy)));         // 최대 10배 소모
                else if (src.troops >= 10000)
                    n = int(n * pk::max(0.5f, (2 - src.troops / 10000.f)));    // 최소 50% 소모
            }

            n = pk::max(1, n);  //至少消耗1点

            return n;
        }


        void onGameTitle()
        {
            if (百出耗气规则 == 1)
            {
                pk::get_skill(특기_백출).desc = pk::encode("所有计略的消耗气力变为二分之一");
                pk::get_skill(특기_급습).desc = pk::encode("陆上攻击时,50%概率不受反击");
                pk::get_skill(특기_강습).desc = pk::encode("水上攻击时,50%概率不受反击");
            }
        }



    }

    Main main;
}
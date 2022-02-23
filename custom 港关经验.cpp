// ## 2020/12/24 # 江东新风 # 修复trace参数报错 ##
// ## 2020/11/10 # messi # 搬运naver武将在港关及其范围内防守会获得经验 ##
namespace 수비경험치
{
    //----------------------------------------------------------------

    const bool 효과_관문설정 = true;  // 관문 거점에 대해서 효과 적용 여부
    const bool 효과_항구설정 = true;  // 항구 거점에 대해서 효과 적용 여부

    //-----------------------------------------------------------------


    //-----------------------------------------------------------------

    const bool 관문수비경혐치 = true; //관문이나 항구에 소속된 장수는 무력과 통솔 경험치를 얻음.
    const int exp_0 = 2; //통솔
    const int exp_1 = 2; //무력

    //-----------------------------------------------------------------

    class Main
    {
        Main()
        {
            pk::bind(107, pk::trigger107_t(onNewDay));
        }

        void onNewDay()     // 턴별
        {
            // 게임시작시에는 발동안함
            if (pk::get_elapsed_days() <= 0) return;

            // 거점_관문
            for (int i = 건물_관문시작; i < 건물_관문끝; i++)
                func_building_effect(pk::get_building(i), 효과_관문설정);

            // 거점_항구
            for (int i = 건물_항구시작; i < 건물_항구끝; i++)
                func_building_effect(pk::get_building(i), 효과_항구설정);

        }


        //--------------------------------------------------------------

        void func_building_effect(pk::building@ building, bool 효과적용)
        {
            if (효과적용)
            {
                func_관문_수비경험치(building, 관문수비경혐치); // 턴별

            }
        }

        void func_관문_수비경험치(pk::building@ building, bool 효과적용)
        {

            if (효과적용)
            {
                auto list = pk::list_to_array(pk::get_person_list(building, pk::mibun_flags(身份_君主, 身份_太守, 身份_一般)));
                if (0 < list.length)
                {
                    for (int i = 0; i < int(list.length); i++)
                    {
                        pk::person@ person = list[i];
                        if (!pk::is_unitize(person) and !pk::is_absent(person))
                        {
                            person.stat_exp[0] = person.stat_exp[0] + exp_0;
                            person.stat_exp[1] = person.stat_exp[1] + exp_1;

                            if (pk::enemies_around(building))
                            {
                                person.stat_exp[0] = person.stat_exp[0] + 2;
                                person.stat_exp[1] = person.stat_exp[1] + 2;
                            }
                        }
                    }
                }

            }
        }
    }

    Main main;
}
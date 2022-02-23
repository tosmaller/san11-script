// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2020/10/30 # 江东新风 # 同步马术书大神的更新 ##  
// ## 2020/07/26 ##
//此功能和现在ai会做的城墙修复等重复，而且此功能下的上限极高，考虑需平衡
/*
@作者:骑马策士
@ UPDATE: '18.11.06 /最初应用:据点包围效果，据点奏乐效果，据点名声效果，据点修筑效果
@ UPDATE: '18.11.11 /更改内容:适用此事实效果，添加据点化系统反击
@ UPDATE: '18.11.23 /更改内容:增加AI/Player战车设置，据点修复-AI攻城前不启动
@ UPDATE: '18.11.26 /更改内容:据点火计反击NPC部队修正错误
@ UPDATE: '18.12.05 /更改内容:据点化系统反参数添加确认条件，生效建筑类型设置选项
@ UPDATE: '20.08.26 /更改内容:在事件中禁用脚本操作
@ 만든이: 기마책사
@ UPDATE: '18.11.6   / 최초적용: 거점포위효과, 거점주악효과, 거점명성효과, 거점축성효과
@ UPDATE: '18.11.11  / 변경내용: 이팩트 启用效果, 거점화계반격 추가
@ UPDATE: '18.11.23  / 변경내용: AI/Player전차별설정 추가, 거점내구회복 -AI공성전 시 발동안함
@ UPDATE: '18.11.26  / 변경내용: 거점화계반격 NPC부대 에러 수정
@ UPDATE: '18.12.5   / 변경내용: 거점화계반격 태수확인 조건 추가, 효과발동 건물타입 설정옵션 추가
@ UPDATE: '20.8.26   / 변경내용: 캠페인에서는 스크립트 동작 안하도록 수정
*/

namespace 거점전투효과
{
    //---------------------------------------------------------------------------------------
    // 유저 설정 (true = on, false = off)
    const bool 显示气泡框 = true;

    const bool 仅AI_启用 = false;      // true =只适用AI效果，false =用户和AI都适用效果(发动标准)  true =AI만 효과적용, false =유저와 AI 모두 효과 적용 (발동기준)
    const bool 仅Player_启用 = false;  // true =Player应用效果，false =用户和AI都应用效果  true =Player만 효과적용, false =유저와 AI 모두 효과 적용
    //---------------------------------------------------------------------------------------
    const bool 城市据点_适用 = true;  // 是否适用于城市据点 도시 거점에 대해서 효과 적용 여부
    const bool 关卡据点_适用 = true;  // 是否适用于关卡据点 관문 거점에 대해서 효과 적용 여부
    const bool 港口据点_适用 = true;  // 是否适用于港口据点 항구 거점에 대해서 효과 적용 여부
    //---------------------------------------------------------------------------------------

    const bool 据点耐久恢复 = true;   // 按城内武将政治能力的比例恢复耐久  성 내 무장 있을 시 정치력 합산에 비례하여 내구 회복
    const bool 耐久恢复特技 = true;   // 筑城特技耐久恢复优待 特技_筑城 보유 거점 매턴 내구 회복
    const bool 据点包围效果 = true;   // 周围有敌军时减少兵力 주변에 적군 있을 시 병력이탈, 기력감소

    const bool 据点火计反击 = true;   // 성벽에 달라붙을 시 적부대에 화계발동 : 특기_화공, 화신 우대 ('18.11.11)

    const bool 据点奏乐效果 = true;   // 特技_奏乐 보유 거점은 기력 회복 (시상 효과 2배)
    const bool 据点名声效果 = true;   // 특기_명성 보유 거점은 병력 증가

    //---------------------------------------------------------------------------------------


    class Main
    {
        Main()
        {
            pk::bind(107, pk::trigger107_t(onNewDay));
        }

        void onNewDay()     // 턴별
        {
            if (pk::is_campaign()) return;
            // 게임시작시에는 발동안함
            if (pk::get_elapsed_days() <= 0) return;

            // 거점_도시
            for (int i = 건물_도시시작; i < 城市_末; i++)
                func_building_effect(pk::get_building(i), 城市据点_适用);

            // 거점_관문
            for (int i = 건물_관문시작; i < 건물_관문끝; i++)
                func_building_effect(pk::get_building(i), 关卡据点_适用);

            // 거점_항구
            for (int i = 건물_항구시작; i < 건물_항구끝; i++)
                func_building_effect(pk::get_building(i), 港口据点_适用);

        }

        void func_building_effect(pk::building@ building, bool 启用效果)
        {
            if (启用效果)
            {
                // 건물별 적용효과 함수 호출
                // 함수 마지막을 true/false 설정에 따라 효과 on/off 설정 가능
                func_据点_耐久恢复(building, 据点耐久恢复);    // 턴별 
                func_据点_筑城效果(building, 耐久恢复特技);    // 턴별
                func_据点_包围效果(building, 据点包围效果);    // 턴별 
                func_据点_火计反击(building, 据点火计反击);    // 턴별
                func_据点_奏乐效果(building, 据点奏乐效果);    // 월별 (pk::trigger108_t 미사용)
                func_据点_名声效果(building, 据点名声效果);    // 분기별 (pk::trigger109_t 미사용)
            }
        }



        //-------------------------------------------------------------------------------------------------
        // 유저 또는 AI만 적용 시 판단함수
        bool only_AI_unit(pk::unit@ unit)
        {
            if (仅AI_启用 and unit.is_player()) return false;
            return true;
        }
        bool only_AI_building(pk::building@ building)
        {
            if (仅AI_启用 and building.is_player()) return false;
            return true;
        }
        bool only_player_unit(pk::unit@ unit)
        {
            if (仅Player_启用 and !unit.is_player()) return false;
            return true;
        }
        bool only_player_building(pk::building@ building)
        {
            if (仅Player_启用 and !building.is_player()) return false;
            return true;
        }

        //-------------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------

        // 거점 내 내구도 회복 : 정치 합계 x10 ('18.11.10)
        void func_据点_耐久恢复(pk::building@ building, bool 启用效果)
        {
            // 주위 5칸 이내 Player 또는 AI끼리 교전 중인지 확인 ('18.11.23)
            // AI 공성전 시에는 회복 안함!!! 
            // Player 공성전 시에는 회복량 1.5배
            bool engage_AI_unit = false;
            bool engage_player_unit = false;
            auto range = pk::range(building.pos, 1, 5);
            for (int j = 0; j < int(range.length); j++)
            {
                pk::unit@ unit = pk::get_unit(range[j]);
                if (unit !is null and !building.is_player() and !unit.is_player() and pk::is_enemy(building, unit))
                    engage_AI_unit = true;
                if (unit !is null and !building.is_player() and unit.is_player() and pk::is_enemy(building, unit))
                    engage_player_unit = true;
            }

            if (启用效果 and only_AI_building(building) and !engage_AI_unit)
            {
                if (int(building.hp) < int(pk::get_max_hp(building)))
                {
                    if (pk::is_in_screen(building.pos))
                    {
                        pk::person@ taishu = pk::get_person(pk::get_taishu_id(building));
                        if (显示气泡框)
                            pk::say(pk::encode("在敌人攻过来之前,快进行维修!"), taishu);
                    }

                    int build_point = 10 * func_stat_sum(building, 武将能力_政治); // 성 내 무장의 정치 능력 부분 합산

                    if (engage_player_unit)    // player 와 교전 중이면 내구 회복량 1.5배
                        build_point = int(build_point * 1.5f);

                    pk::add_hp(building, build_point, true);
                }

            }
        }
        int func_stat_sum(pk::building@ building, int stat_type)
        {
            //도시 내 무장 능력 반영 (각 무장의 정치 합산) ('18.10.7)
            float add_ratio = 7;    // 합산 보조율 : 무장별 10%
            int stat_sum = 0;

            auto list = pk::list_to_array(pk::get_person_list(building, pk::mibun_flags(身份_君主, 身份_太守, 身份_一般)));
            if (0 < list.length)
            {
                for (int i = 0; i < int(list.length); i++)
                {
                    pk::person@ person = list[i];
                    //출진 또는 부재중 상태가 아닌 장수이며, 태수와 혐오 관계가 아닌 장수만 능력 치 합산
                    if (!pk::is_unitize(person) and !pk::is_absent(person))
                    {
                        stat_sum += int((pk::max(person.stat[stat_type] - 25, 20)) * (add_ratio / 100.f));
                    }
                }
            }
            return stat_sum;
        }


        //-------------------------------------------------------------------------------------------------
        // 如果据点周围有部队，兵力和气力就会减少(18.10.21)。   거점 주변에 적부대 있을 경우 병력 및 기력 감소 ('18.10.21)
        void func_据点_包围效果(pk::building@ building, bool 启用效果)
        {
            if (启用效果 and only_player_building(building))
            {
                if (pk::enemies_around(building))
                {
                    int n = pk::max(pk::get_troops(building), 100);
                    n = int(pk::rand(400) + 40 * sqrt(n / 100));

                    ch::add_troops(building, -n, true,102);   // 因展示效果减少兵力 전시효과로 병력 감소 
                    pk::add_energy(building, -3, true);   // 전시효과로 기력 3 감소
                }

            }
        }

        //-------------------------------------------------------------------------------------------------
        // 거점에 적부대 달라 붙을 경우 화계 발동 ('18.11.11)
        void func_据点_火计反击(pk::building@ building, bool 启用效果)
        {
            if (启用效果 and only_AI_building(building))
            {
                int msg_count = 0;
                int dst_range = (building.get_id() < 城市_末) ? 2 : 1;

                array<pk::point> arr = pk::range(building.pos, dst_range, dst_range);
                for (int j = 0; j < int(arr.length); j++)
                {
                    pk::unit@ unit = pk::get_unit(arr[j]);
                    if (unit !is null and pk::is_enemy(unit, building))
                    {
                        int fire_chance = 0;
                        int fire_timer = 0;
                        int fire_person_id = 0;

                        pk::person@ taishu = pk::get_person(pk::get_taishu_id(building));

                        int stat_delta = 100;
                        if (taishu is null)
                            stat_delta = 0;
                        else if (unit.leader < 武将_灵帝 or unit.leader > 무장_학자)
                            stat_delta = pk::max(0, taishu.stat[武将能力_智力] - unit.attr.stat[部队能力_智力]);

                        // 발동 확률 및 타이머 설정
                        if (taishu is null)
                            fire_chance = 0;
                        else if (pk::is_valid_person_id(building.who_has_skill(특기_화신)))
                        {
                            fire_chance = 100;
                            fire_timer = 1;
                            fire_person_id = building.who_has_skill(특기_화신);
                        }
                        else if (pk::is_valid_person_id(building.who_has_skill(특기_화공)))
                        {
                            fire_chance = pk::min(100, stat_delta + 50);
                            fire_timer = 1;
                            fire_person_id = building.who_has_skill(특기_화공);
                        }
                        else
                        {
                            fire_chance = pk::min(100, stat_delta);
                            fire_person_id = pk::get_taishu_id(building);
                        }


                        // 화계 발동
                        if (pk::rand_bool(fire_chance) and !pk::is_on_fire(unit.pos))
                        {
                            if (显示气泡框 and pk::is_in_screen(building.pos) and msg_count == 0)
                            {
                                msg_count = 1;
                                pk::say(pk::encode("把火燃起来"), pk::get_person(fire_person_id));
                                pk::play_se(0x39, building.pos);
                                switch (pk::rand(3))
                                {
                                case 0: pk::play_voice(0x33, pk::get_person(fire_person_id)); break;   // 음성 : 화계
                                case 1: pk::play_voice(0x34, pk::get_person(fire_person_id)); break;   // 음성 : 화계
                                case 2: pk::play_voice(0x35, pk::get_person(fire_person_id)); break;   // 음성 : 화계
                                }
                            }

                            pk::create_fire(unit.pos, pk::rand(1) + 1 + fire_timer, unit, true);

                            if (显示气泡框 and pk::is_in_screen(building.pos))
                            {
                                switch (pk::rand(4))
                                {
                                case 0: pk::say(pk::encode("哇哇.."), pk::get_person(무장_병사), unit); break;
                                case 1: pk::say(pk::encode("我的脸"), pk::get_person(무장_병사), unit); break;
                                case 2: pk::say(pk::encode("火烧过来了, 快躲开! "), pk::get_person(무장_병사), unit); break;
                                case 3: pk::say(pk::encode("火.着火了!"), pk::get_person(무장_병사), unit); break;
                                }
                            }
                        }

                    }
                }
            }
        }

        //-------------------------------------------------------------------------------------------------
        // 거점 내 特技_奏乐 무장이 소속 시 기력 회복 +5 ~ +10 ('18.10.21)
        void func_据点_奏乐效果(pk::building@ building, bool 启用效果)
        {
            if (启用效果 and only_AI_building(building))
            {
                // 월별 적용, 매월 1일에 발동
                int cheer_point = 0;
                if (pk::is_valid_person_id(building.who_has_skill(特技_奏乐)) and (pk::get_day() == 1))
                {
                    if (pk::get_energy(building) < pk::get_max_energy(building))
                    {
                        if (显示气泡框 and pk::is_in_screen(building.pos))
                        {
                            pk::say(pk::encode("恢复士兵们的士气吧"), pk::get_person(building.who_has_skill(特技_奏乐)));
                            pk::play_se(0x39, building.pos);
                            pk::play_voice(0x37, pk::get_person(building.who_has_skill(特技_奏乐)));   // 음성 : 전투_진정
                        }

                        cheer_point = 5;
                        if (pk::is_valid_person_id(building.who_has_skill(特技_诗想)))
                            cheer_point *= 2;

                        pk::add_energy(building, cheer_point, true);
                    }
                }

            }
        }

        //-------------------------------------------------------------------------------------------------
        // 거점 내 특기_명성 무장이 소속 시 매분기별 병력 증가 : 매력 x 10 ('18.10.22)
        void func_据点_名声效果(pk::building@ building, bool 启用效果)
        {
            if (启用效果 and only_AI_building(building))
            {
                // 분기별 적용, 매분기 첫달 1일에만 발동
                int famous_point = 0;
                if (pk::is_valid_person_id(building.who_has_skill(특기_명성)) and pk::is_first_month_of_quarter(pk::get_month()) and (pk::get_day() == 1))
                {
                    if (pk::get_troops(building) < pk::get_max_troops(building))
                    {
                        if (显示气泡框 and pk::is_in_screen(building.pos))
                        {
                            pk::say(pk::encode("请接受我们成为士兵吧"), pk::get_person(무장_청년), building);
                            pk::play_se(0x39, building.pos);
                            pk::play_voice(0x31, pk::get_person(building.who_has_skill(특기_명성)));   // 음성 : 전투_계략2
                        }

                        famous_point += 10 * (pk::get_person(building.who_has_skill(특기_명성)).stat[武将能力_魅力]);
                        ch::add_troops(building, famous_point, true);
                    }

                }

            }
        }

        //-------------------------------------------------------------------------------------------------
        // 거점 내 特技_筑城 무장이 소속 시 내구도 회복 : 정치 x 5 ('18.10.22)
        void func_据点_筑城效果(pk::building@ building, bool 启用效果)
        {
            if (启用效果 and only_AI_building(building))
            {
                int build_point = 0;
                if (pk::is_valid_person_id(building.who_has_skill(特技_筑城)))
                {
                    if (int(building.hp) < int(pk::get_max_hp(building)))
                    {
                        if (显示气泡框 and pk::is_in_screen(building.pos))
                        {
                            pk::say(pk::encode("快把城墙修补起来"), pk::get_person(building.who_has_skill(特技_筑城)));
                            pk::play_se(0x39, building.pos);
                            switch (pk::rand(2))
                            {
                            case 0: pk::play_voice(0x39, pk::get_person(building.who_has_skill(特技_筑城))); break;   // 음성 : 설치
                            case 1: pk::play_voice(0x3a, pk::get_person(building.who_has_skill(特技_筑城))); break;   // 음성 : 설치
                            }
                        }

                        build_point += 5 * (pk::get_person(building.who_has_skill(特技_筑城)).stat[武将能力_政治]);
                        pk::add_hp(building, build_point, true);
                    }
                }

            }
        }


        //-------------------------------------------------------------------------------------------------

    }

    Main main;
}
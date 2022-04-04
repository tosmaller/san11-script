// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/01/23 # 江东新风 # 加入开启造币谷仓无需相邻时将不摆放位置 ##
// ## 2020/12/16 # messi # 优化语句 ##
// ## 2020/10/30 # 江东新风 # 同步马术书大神的更新 ##
// ## 2020/09/23 # 氕氘氚 # 修复交換内政设施失敗的問題 ##
// ## 2020/07/26 ##
/*
// 만든이: 기마책사
// Update: '18.12.7    / 최초적용: 수입시설_최적건설(조폐 주변 농장을 시장으로, 곡창 주변 시장을 농장으로 변경)
// Update: '19.1.5     / 변경내용: 수입시설_위치이동(멀리 떨어진 시장/농장을 조폐/곡창 주변의 보조시설과 위치 변경), 수입시설_재건축(조폐/곡창 위치에 잘못 지어진 시설 재건축)
// Update: '19.1.9     / 변경내용: 수입시설_위치이동 조건 수정 (군둔농, 대도시 등 추가)
// Update: '19.1.30    / 변경내용: 실행무장 이름 추출 오류 수정
// Update: '19.2.25    / 변경내용: 재건축_세력설정 옵션 설명/스크립트 불일치 수정, 최적건설 및 위치이동 알고리즘에 세력설정 옵션 추가
// Update: '19.11.25   / 변경내용: 재건축 실행무장 선택 알고리즘 개선
// Update: '20.8.24    / 변경내용: core2.dll 신버젼 적용 시 수입시설_위치이동 기능 오류 수정
// Update: '20.9.20    / 변경내용: 수입시설_위치이동 조건 수정 (동작대, 공방, 조선, 생산시설3단 추가), 조폐/곡창 재건축 조건 수정
*/

namespace AI优化_内政设施
{
    //---------------------------------------------------------------------------------------
    // 유저 설정 (true = on, false = off)
    const bool 수입시설_최적건설 = true;     // 시장/농장/조폐/곡창 제대로 짓기 ('18.12.7)
    const int  최적건설_세력설정 = 0;        // 0: 모두 적용, 1: AI세력만 적용, 2: Player세력 전체적용, 3: Player_1군단만 적용

    const bool 수입시설_위치이동 = true;    // 멀리 떨어진 시장/농장을 조폐/곡창 주변의 보조시설과 위치 변경
    const int  위치이동_세력설정 = 0;       // 0: 모두 적용, 1: AI세력만 적용, 2: Player세력 전체적용, 3: Player_1군단만 적용

    //---------------------------------------------------------------------------------------
    const bool 수입시설_재건축 = true;     // 잘못 지어진 위치에 조폐/곡창 다시짓기 ('19.1.4)
    const bool 재건축_유저확인 = true;
    const int  재건축_기준도시금 = 500;
    const int  재건축_세력설정 = 0;        // 0: 모두 적용, 1: AI세력만 적용, 2: Player세력 전체적용, 3: Player_1군단만 적용

    //---------------------------------------------------------------------------------------

    class Main
    {

        Main()
        {
            pk::bind(111, pk::trigger111_t(onTurnStart));
            pk::bind(112, pk::trigger112_t(onTurnEnd));
        }

        void onTurnStart(pk::force@ force)
        {
            if (pk::is_campaign()) return;

            if (수입시설_최적건설)
                optimize_force_city_facility(force);    // 수입관련 내정시설 건설 최적화
        }


        void onTurnEnd(pk::force@ force)
        {
            if (pk::is_campaign()) return;

            if (수입시설_위치이동 or 수입시설_재건축)
                rebuild_force_city_facility(force);     // 시설 위치 변경 또는 재건축 실행
        }

        //---------------------------------------------------------------------------------------

        // 세력 설정 옵션
        bool check_force_option(pk::force@ force, bool enable, int option)
        {
            if (!enable) return false;
            //当造币谷仓无需相邻时就不摆了
            if (造币谷仓无需相邻) return false;
            // 세력설정 = 0: 모두 적용, 1: AI세력만 적용, 2: Player세력 전체적용, 3: Player_1군단만 적용
            if (option == 0) return true;
            if (option == 1 and !force.is_player()) return true;
            if (option == 2 and force.is_player())  return true;
            if (option == 3 and pk::is_player_controlled(force)) return true;

            return false;
        }

        //---------------------------------------------------------------------------------------
        // 수입시설 최적화
        //---------------------------------------------------------------------------------------

        // 세력의 도시
        void optimize_force_city_facility(pk::force@ force)
        {
            // 세력의 도시 리스트
            pk::array<pk::city@> arr_city_list = pk::list_to_array(pk::get_city_list(force));
            for (int i = 0; i < int(arr_city_list.length); i++)
            {
                if (check_force_option(force, 수입시설_최적건설, 최적건설_세력설정))
                    optimize_city_facility(arr_city_list[i]);
            }
        }

        // 도시별 내정시설 검색 및 최적화
        void optimize_city_facility(pk::city@ city)
        {
            int force_id = city.get_force_id();
            if (city is null or !pk::is_valid_force_id(force_id))
                return;

            int sum_facility_hp = 0;
            for (int i = 0; i < int(city.max_devs); i++)
            {
                pk::building@ building = city.dev[i].building;
                if (pk::is_alive(building))
                {
                    int facility_id = building.facility;

                    // 미완성 조폐/곡창 (수입시설 최적화)未完成造币/谷仓(优化收入设施)
                    if (!building.completed and (facility_id == 시설_곡창 or facility_id == 시설_조폐))
                    {
                        change_center_facility(building);   // 주변의 시장/농장 기준수 판단하여 변경 
                    }

                    // 완성된 조폐/곡창 주변 (수입시설 최적화)
                    if (building.completed and (facility_id == 시설_곡창 or facility_id == 시설_조폐))
                    {
                        change_yield_facility(building);    // 미완성 주변 농장/시장 종류 최적화
                        change_assist_facility(building);   // 미완성 주변 보조시설을 시장/농장으로 변경
                    }

                }
            }
        }


        //---------------------------------------------------------------------------------------

        // 미완성 주변 생산시설 변경
        void change_yield_facility(pk::building@ building)
        {
            if (!pk::is_alive(building)) return;
            if (!building.completed) return;

            int facility_id = building.facility;

            // 완성된 곡창 주변 시설 검색
            if (facility_id == 시설_곡창)
            {
                for (int i = 0; i < 방향_끝; i++)
                {
                    pk::point neighbor_pos = pk::get_neighbor_pos(building.pos, i);
                    if (pk::is_valid_pos(neighbor_pos))
                    {
                        pk::building@ target_building = pk::get_building(neighbor_pos);
                        // 미완성 주변 시장을 농장으로 변경
                        if (pk::is_alive(target_building) and !target_building.completed and target_building.facility == 시설_시장)
                        {
                            int count_neighbor = count_neighbor_facility(target_building, 시설_시장)
                                + count_neighbor_facility(target_building, 시설_시장2단)
                                + count_neighbor_facility(target_building, 시설_시장3단);

                            if (count_neighbor <= 3)
                            {
                                target_building.facility = 시설_농장;
                                target_building.update();
                            }
                        }
                    }
                }
            }

            // 완성된 조폐 주변 시설 검색
            if (facility_id == 시설_조폐)
            {
                for (int i = 0; i < 방향_끝; i++)
                {
                    pk::point neighbor_pos = pk::get_neighbor_pos(building.pos, i);
                    if (pk::is_valid_pos(neighbor_pos))
                    {
                        pk::building@ target_building = pk::get_building(neighbor_pos);

                        // 미완성 주변 농장을 시장으로 변경
                        if (pk::is_alive(target_building) and !target_building.completed and target_building.facility == 시설_농장)
                        {
                            int count_neighbor = count_neighbor_facility(target_building, 시설_농장)
                                + count_neighbor_facility(target_building, 시설_농장2단)
                                + count_neighbor_facility(target_building, 시설_농장3단);

                            if (count_neighbor <= 3)
                            {
                                target_building.facility = 시설_시장;
                                target_building.update();
                            }
                        }
                    }
                }
            }

        }

        //---------------------------------------------------------------------------------------

        // 해당건물 주위로 확인하고자 하는 시설 갯수 카운트
        int count_neighbor_facility(pk::building@ building, int facility_id)
        {
            int count_facility = 0;

            for (int j = 0; j < 방향_끝; j++)
            {
                pk::point neighbor_pos = pk::get_neighbor_pos(building.pos, j);
                if (pk::is_valid_pos(neighbor_pos))
                {
                    pk::building@ neighbor_building = pk::get_building(neighbor_pos);
                    if (pk::is_alive(neighbor_building) and neighbor_building.facility == facility_id)
                        count_facility += 1;
                }
            }

            return count_facility;
        }

        //---------------------------------------------------------------------------------------

        // 미완성 주변 보조시설 변경
        void change_assist_facility(pk::building@ building)
        {
            if (!pk::is_alive(building)) return;
            if (!building.completed) return;

            int facility_id = building.facility;

            // 완성된 조폐/곡창 주변의 시장 검색
            if (facility_id == 시설_조폐 or facility_id == 시설_곡창)
            {
                int count_valid_pos = 0;
                int count_completed = 0;
                int count_uncompleted = 0;
                for (int i = 0; i < 방향_끝; i++)
                {
                    pk::point neighbor_pos = pk::get_neighbor_pos(building.pos, i);
                    if (pk::is_valid_pos(neighbor_pos))
                    {
                        count_valid_pos += 1;

                        pk::building@ target_building = pk::get_building(neighbor_pos);
                        if (pk::is_alive(target_building) and 시설_부절태 <= target_building.facility and target_building.facility <= 시설_연병소)
                        {
                            // 주변 보조시설 수 (완성, 미완성)
                            if (target_building.completed)
                                count_completed += 1;
                            else
                                count_uncompleted += 1;

                        }
                    }
                }

                // 조폐/곡창 주변에 보조시설을 과도하게 지을 경우, 미완성 보조시설을 시장/농장으로 변경
                if ((count_completed + count_uncompleted) >= int((count_valid_pos + 1) / 3))
                {
                    for (int i = 0; i < 방향_끝; i++)
                    {
                        pk::point neighbor_pos = pk::get_neighbor_pos(building.pos, i);
                        if (pk::is_valid_pos(neighbor_pos))
                        {
                            pk::building@ target_building = pk::get_building(neighbor_pos);
                            if (pk::is_alive(target_building) and !target_building.completed and 시설_부절태 <= target_building.facility and target_building.facility <= 시설_연병소)
                            {
                                if (facility_id == 시설_조폐)
                                    target_building.facility = 시설_시장;
                                if (facility_id == 시설_곡창)
                                    target_building.facility = 시설_농장;
                            }
                        }
                    }

                }
            }

        }

        //---------------------------------------------------------------------------------------

        // 미완성 중앙 시설 변경
        void change_center_facility(pk::building@ building)
        {
            if (!pk::is_alive(building)) return;
            if (building.completed) return;

            int facility_id = building.facility;

            float count_ref = 3.0f;

            // 미완성 조폐/곡창 주변의 농장 검색
            if (facility_id == 시설_조폐 or facility_id == 시설_곡창)
            {
                float count_farm = 0.f;
                float count_market = 0.f;
                for (int i = 0; i < 방향_끝; i++)
                {
                    pk::point neighbor_pos = pk::get_neighbor_pos(building.pos, i);
                    if (pk::is_valid_pos(neighbor_pos))
                    {
                        pk::building@ target_building = pk::get_building(neighbor_pos);
                        if (pk::is_alive(target_building))
                        {
                            // 주변 농장, 농장2단, 농장3단 검색
                            if (target_building.facility == 시설_농장 and target_building.completed)
                                count_farm += 1.0f;
                            if (target_building.facility == 시설_농장2단)
                                count_farm += 1.25f;
                            if (target_building.facility == 시설_농장3단)
                                count_farm += 1.5f;

                            // 주변 시장, 시장2단, 시장3단 검색
                            if (target_building.facility == 시설_시장 and target_building.completed)
                                count_market += 1.0f;
                            if (target_building.facility == 시설_시장2단)
                                count_market += 1.25f;
                            if (target_building.facility == 시설_시장3단)
                                count_market += 1.5f;
                        }
                    }
                }
                // 농장 기준 수 만족 시 조폐 → 곡창 시설 변경
                if (facility_id == 시설_조폐 and count_farm >= count_ref and count_farm > count_market)
                {
                    building.facility = 시설_곡창;
                    building.update();
                }

                // 시장 기준 수 만족 시 곡창 → 조폐 시설 변경
                if (facility_id == 시설_곡창 and count_market >= count_ref and count_market > count_farm)
                {
                    building.facility = 시설_조폐;
                    building.update();
                }
            }

        }


        //---------------------------------------------------------------------------------------
        // 수입시설_위치이동 / 수입시설_재건축
        //---------------------------------------------------------------------------------------


        // 세력의 도시
        void rebuild_force_city_facility(pk::force@ force)
        {
            // 세력의 도시 리스트
            pk::array<pk::city@> arr_city_list = pk::list_to_array(pk::get_city_list(force));
            for (int i = 0; i < int(arr_city_list.length); i++)
            {
                if (check_force_option(force, 수입시설_위치이동, 위치이동_세력설정))
                    swap_city_facility(arr_city_list[i]);

                if (check_force_option(force, 수입시설_재건축, 재건축_세력설정))
                    rebuild_city_facility(arr_city_list[i]);
            }
        }

        // 도시 내 완성된 생산시설/보조시설 위치 변경 
        void swap_city_facility(pk::city@ city)
        {
            int force_id = city.get_force_id();
            if (city is null or !pk::is_valid_force_id(force_id))
                return;

            if (!check_city_dev_complete(city)) return;   // 건축율 확인

            // 완성된 생산시설/보조시설 간 위치 서로 변경 
            swap_yield_facility(city);


        }

        // 도시별 재건축 확인 및 실행
        void rebuild_city_facility(pk::city@ city)
        {
            int force_id = city.get_force_id();
            if (city is null or !pk::is_valid_force_id(force_id))
                return;

            if (!check_city_dev_complete(city)) return;   // 건축율 확인
            if (city.max_devs <= 10) return;        // 개발지 개수 극소 도시 제외

            // 행동력 및 도시금 조건 확인
            if (int(pk::get_district(city.get_district_id()).ap) > 재건축_기준행동력 and int(city.gold) > 재건축_기준도시금)
            {
                for (int i = 0; i < int(city.max_devs); i++)
                {
                    pk::building@ building = city.dev[i].building;
                    if (pk::is_alive(building))
                    {
                        rebuild_center_facility(city, building);
                    }
                }
            }
        }

        //---------------------------------------------------------------------------------------
        // 수입시설 위치이동 (멀리 떨어진 시장/농장을 조폐/곡창 주변의 보조시설과 위치 변경)

        // 도시의 시설 swapping
        void swap_yield_facility(pk::city@ city)
        {
            if (!pk::is_alive(city)) return;

            pk::building@ src_building;
            pk::building@ dst_building;
            int force_id = city.get_force_id();

            if (pk::has_facility(city, 시설_조폐) and get_facility_count(city, 시설_시장3단) >= 2)
            {
                get_neighbor_assist_facility(city, 시설_조폐, dst_building);
                if (pk::is_alive(dst_building))
                {
                    get_isolated_yield_facility(city, 시설_시장3단, 시설_조폐, src_building);
                    get_neighbor_assist_facility(city, 시설_조폐, dst_building);
                    swap_facility(src_building, dst_building, force_id);    // 건물 SWAP

                    get_isolated_yield_facility(city, 시설_시장2단, 시설_조폐, src_building);
                    get_neighbor_assist_facility(city, 시설_조폐, dst_building);
                    swap_facility(src_building, dst_building, force_id);    // 건물 SWAP

                    get_isolated_yield_facility(city, 시설_시장, 시설_조폐, src_building);
                    get_neighbor_assist_facility(city, 시설_조폐, dst_building);
                    swap_facility(src_building, dst_building, force_id);    // 건물 SWAP
                }
            }

            if (pk::has_facility(city, 시설_곡창) and get_facility_count(city, 시설_농장3단) >= 2)
            {
                get_neighbor_assist_facility(city, 시설_곡창, dst_building);
                if (pk::is_alive(dst_building))
                {
                    get_isolated_yield_facility(city, 시설_농장3단, 시설_곡창, src_building);
                    get_neighbor_assist_facility(city, 시설_곡창, dst_building);
                    swap_facility(src_building, dst_building, force_id);    // 건물 SWAP

                    get_isolated_yield_facility(city, 시설_농장2단, 시설_곡창, src_building);
                    get_neighbor_assist_facility(city, 시설_곡창, dst_building);
                    swap_facility(src_building, dst_building, force_id);    // 건물 SWAP

                    get_isolated_yield_facility(city, 시설_농장, 시설_곡창, src_building);
                    get_neighbor_assist_facility(city, 시설_곡창, dst_building);
                    swap_facility(src_building, dst_building, force_id);    // 건물 SWAP
                }
            }
        }

        // 독립 수입시설 검색
        void get_isolated_yield_facility(pk::city@ city, int facility_id, int neighbor_id, pk::building@& out src_building)
        {
            if (!pk::is_alive(city)) return;

            @src_building = null;
            for (int i = 0; i < int(city.max_devs); i++)
            {
                pk::building@ building = city.dev[i].building;
                if (pk::is_alive(building))
                {
                    if (building.facility == facility_id and building.completed and count_neighbor_facility(building, neighbor_id) == 0)
                    {
                        @src_building = @building;
                        return;
                    }
                }
            }
        }

        // 인접한 보조시설 검색
        void get_neighbor_assist_facility(pk::city@ city, int facility_id, pk::building@& out dst_building)
        {
            if (!pk::is_alive(city)) return;

            @dst_building = null;
            for (int i = 0; i < int(city.max_devs); i++)
            {
                pk::building@ building = city.dev[i].building;
                if (pk::is_alive(building) and building.facility == facility_id)
                {
                    // 주위의 보조 시설 검색
                    for (int j = 0; j < 방향_끝; j++)
                    {
                        pk::point neighbor_pos = pk::get_neighbor_pos(building.pos, j);
                        if (pk::is_valid_pos(neighbor_pos))
                        {
                            pk::building@ building_t = pk::get_building(neighbor_pos);
                            if (pk::is_alive(building_t) and building_t.completed
                                and ((시설_부절태 <= building_t.facility and building_t.facility <= 시설_군둔농) or building_t.facility == 시설_동작대
                                    or building_t.facility == 시설_공방 or building_t.facility <= 시설_조선
                                    or building_t.facility == 시설_병영3단 or building_t.facility == 시설_대장간3단 or building_t.facility == 시설_마구간3단))
                            {
                                /*
                                @dst_building = @building_t;
                                return;
                                */

                                // '20.8.24, 추가
                                for (int k = 0; k < int(city.max_devs); k++)
                                {
                                    pk::building@ building_k = city.dev[k].building;
                                    if (pk::is_alive(building_k))
                                    {
                                        if (building_t.get_id() == building_k.get_id())
                                        {
                                            @dst_building = @building_t;
                                            return;
                                        }
                                    }
                                }

                            }
                        }
                    }
                }
            }
        }

        // 시설 swapping 함수
        void swap_facility(pk::building@ src_building, pk::building@ dst_building, int force_id)
        {
            if (!pk::is_valid_force_id(force_id)) return;
            if (!pk::is_alive(src_building)) return;
            if (!pk::is_alive(dst_building)) return;

            int src_id = src_building.facility;
            if (src_id < 30 or src_id > 60) return;
            int dst_id = dst_building.facility;
            if (dst_id < 30 or dst_id > 60) return;
            pk::point src_pos = src_building.pos;
            pk::point dst_pos = dst_building.pos;
            int src_hp = src_building.hp;
            int dst_hp = dst_building.hp;

            // 신버젼 core2.dll 에러발생으로 기존 시설 제거 ('20.8.24)
            pk::kill(src_building, false);
            pk::kill(dst_building, false);

            pk::create_building(pk::get_facility(src_id), dst_pos, force_id);
            pk::building@ new_src = pk::get_building(dst_pos);
            pk::complete(new_src);
            new_src.hp = src_hp;    // 건물 내구도 재설정 추가 ('20.9.20)
            new_src.update();

            pk::create_building(pk::get_facility(dst_id), src_pos, force_id);
            pk::building@ new_dst = pk::get_building(src_pos);
            pk::complete(new_dst);
            new_dst.hp = dst_hp;    // 건물 내구도 재설정 추가 ('20.9.20)
            new_dst.update();
        }



        //---------------------------------------------------------------------------------------
        // 조폐/곡창 위치 잘못 지어진 경우 다시 짓기

        int 재건축_기준행동력 = 30;     // 수정금지!!
        int answer = 0;     // 0:예,  1:아니오,  2:예(계속),  3:아니오(계속)


        // 중앙시설 위치에 재건축
        void rebuild_center_facility(pk::city@ city, pk::building@ building)
        {
            if (!pk::is_alive(building)) return;

            int count_neighbor = 0;
            int facility_id = building.facility;

            // 시장 안에 시장/조폐가 아니면 다시 짓기
            if (!pk::has_facility(city, 시설_조폐))
            {
                count_neighbor = count_neighbor_facility(building, 시설_시장) + count_neighbor_facility(building, 시설_시장2단) + count_neighbor_facility(building, 시설_시장3단);
                if (count_neighbor >= 3 and facility_id != 시설_시장 and facility_id != 시설_시장2단 and facility_id != 시설_시장3단 and facility_id != 시설_조폐)
                {
                    rebuild_facility(city, building, 시설_조폐);    // 조폐로 재건축
                }
            }

            // 농장 안에 농장/곡창이 아니면 다시 짓기
            if (!pk::has_facility(city, 시설_곡창))
            {
                count_neighbor = count_neighbor_facility(building, 시설_농장) + count_neighbor_facility(building, 시설_농장2단) + count_neighbor_facility(building, 시설_농장3단);
                if (count_neighbor >= 3 and facility_id != 시설_농장 and facility_id != 시설_농장2단 and facility_id != 시설_농장3단 and facility_id != 시설_곡창)
                {
                    rebuild_facility(city, building, 시설_곡창);    // 곡창으로 재건축
                }
            }
        }

        //---------------------------------------------------------------------------------------

        // 시설 재건축 조건 확인 후 실행
        void rebuild_facility(pk::city@ city, pk::building@ building, int rebuild_id)
        {
            // 행동력 확인을 위한 군단 정보 반환
            pk::district@ district = pk::get_district(city.get_district_id());

            // 실행가능 무장 확인
            pk::list<pk::person@> list_idle_person = pk::get_idle_person_list(pk::city_to_building(city));
            if (list_idle_person.count == 0) return;

            // 정치력이 높은 순으로 정렬
            list_idle_person.sort(function(a, b)
            {
                return a.stat[武将能力_政治] > b.stat[武将能力_政治];
            });

            // 시설 내구도
            float hp_delta = float(int(pk::get_facility(rebuild_id).hp));

            // 기준 작업일수 및 최소능력치 계산
            pk::array<pk::person@> arr_idle_person = pk::list_to_array(list_idle_person);
            int min_actors = pk::min(3, pk::max(1, int(arr_idle_person.length) / 2));     // 적정인원 자동선택
            float max_stat = 0.0f;
            for (int i = 0; i < min_actors; i++)    //최대가능 능력치합산
            {
                float stat_weight;
                if (i == 0) stat_weight = 1.5f;
                else        stat_weight = 0.5f;
                max_stat += float(arr_idle_person[i].stat[武将能力_政治]) * stat_weight;
            }
            int   ref_per = int(hp_delta / max_stat) + (((hp_delta % max_stat) > 0) ? 1 : 0); // 건설기간 산정 (올림)
            float ref_stat = hp_delta / float(ref_per);                                      // 기준 합산능력치


            // 적정인원 실행무장 자동선택
            pk::list<pk::person@> person_sel;
            float sum_stat = 0.0f;
            for (int i = 0; i < min_actors; i++)
            {
                float stat_weight;
                if (i == 0) stat_weight = 1.5f;
                else        stat_weight = 0.5f;

                pk::person@ actor_t;
                for (int j = (int(arr_idle_person.length) - 1); j >= 0; j--)
                {
                    if (!person_sel.contains(arr_idle_person[j]))
                    {
                        @actor_t = arr_idle_person[j];
                        if ((float(actor_t.stat[武将能力_政治]) * stat_weight + sum_stat) > ref_stat) break;
                    }
                }
                if (pk::is_alive(actor_t))
                {
                    person_sel.add(actor_t);
                    sum_stat += actor_t.stat[武将能力_政治] * stat_weight;
                }
                if (sum_stat > ref_stat) break;
            }
            pk::array<pk::person@> actors = pk::list_to_array(person_sel);

            // 금 소비 값 (철거된 건물비용 반환 및 재건축 비용)
            int gold_cost = int(pk::get_facility(rebuild_id).gold_cost) - int(pk::get_facility(building.facility).gold_cost);

            // 조건 확인 후 재건축 실행
            if (int(district.ap) >= 재건축_기준행동력 and pk::is_alive(actors[0]) and int(sum_stat) >= 70 and int(city.gold) > pk::max(gold_cost, 재건축_기준도시금))
            {
                if (pk::is_player_controlled(city) and 재건축_유저확인 and answer <= 1)
                {
                    @building_t = @building;
                    pk::scene(pk::scene_t(goto_facility));

                    pk::person@ kunshu = pk::get_person(pk::get_kunshu_id(city));
                    string actor_name;
                    for (int i = 0; i < int(actors.length); i++)
                    {
                        pk::person@ actor_t = actors[i];
                        if (i == 0) actor_name = pk::decode(pk::get_name(actor_t));
                        else        actor_name = actor_name + "," + pk::decode(pk::get_name(actor_t));
                    }
                    string city_name = pk::decode(pk::get_name(city));
                    string facility_name1 = pk::decode(pk::get_facility(building.facility).name);
                    string facility_name2 = pk::decode(pk::get_facility(rebuild_id).name);

                    answer = pk::choose({ pk::encode(" 是 "), pk::encode(" 否 "), pk::encode("是(继续)"), pk::encode("否(继续)") },
                        pk::encode(pk::format("是否要派遣\x1b[1x{}\x1b[0x在\x1b[1x{}\x1b[0x重建\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x?\n (花费 {} 资金)", actor_name, city_name, facility_name1, facility_name2, gold_cost)),
                        kunshu);

                    pk::scene(pk::scene_t(remove_highlights));
                }

                // 재건축 실행
                if ((pk::is_player_controlled(city) and (answer == 0 or answer == 2)) or !pk::is_player_controlled(city))
                {
                    // 다시 지을 건물이 필수시설이면 건설지 추가확보
                    if (check_essential_facility(building.facility))
                        remove_assist_facility(city, building);

                    // 재건축 실행 함수
                    rebuild_command(city, building, actors, rebuild_id);

                    // 철거된 건물비용 반환 및 재건축 비용 차감
                    pk::add_gold(city, -gold_cost, true);
                }
            }
        }

        //---------------------------------------------------------------------------------------

        // 재건축 실행 함수
        void rebuild_command(pk::city@ city, pk::building@ building, pk::array<pk::person@> actors, int rebuild_id)
        {
            // 건축정보
            pk::develop_cmd_info cmd_develop;
            @cmd_develop.base = pk::city_to_building(city);
            for (int i = 0; i < int(actors.length); i++)
            {
                @cmd_develop.actors[i] = actors[i];
            }
            cmd_develop.facility = rebuild_id;
            cmd_develop.pos = building.pos;

            // 기존 건물 제거 (애니메이션 효과 없이)
            pk::kill(building, false);
            pk::district@ district = pk::get_district(city.get_district_id());
            pk::add_ap(district, -10);

            // 건축 실행
            pk::command(cmd_develop);
        }

        //---------------------------------------------------------------------------------------

        void goto_facility()
        {
            pk::point pos = building_t.pos;
            for (int i = 0; i < 4; i++)
            {
                pk::point pos_t;
                pos_t.x = pos.x + 1;
                pos_t.y = pos.y;
                if (pk::is_valid_pos(pos_t))
                    pos.x = pos.x + 1;
            }

            if (!pk::is_in_screen(pos))
                pk::move_screen(pos, 1);

            pk::highlight(building_t.pos, true);
        }
        void remove_highlights()
        {
            pk::remove_all_highlights();
        }

        pk::building@ building_t;

        //---------------------------------------------------------------------------------------


        // 내정시설 건설율 100% 여부 확인
        bool check_city_dev_complete(pk::city@ city)
        {
            if (city.num_devs < city.max_devs)
                return false;

            return true;
        }

        //---------------------------------------------------------------------------------------

        // 필수시설인지 확인
        bool check_essential_facility(int facility_id)
        {
            if (facility_id <= 시설_곡창 or facility_id >= 시설_시장2단)
                return true;
            if (facility_id == 시설_대시장 or facility_id == 시설_어시장)
                return true;

            return false;
        }

        // 건설지 확보를 위한 보조시설 1개 강제철거
        void remove_assist_facility(pk::city@ city, pk::building@ building_self)
        {
            bool removed = false;
            int count_facility = 0;

            if (pk::has_facility(city, 시설_암시장))
            {
                removed = remove_facility(city, building_self, 시설_암시장);
                if (removed) return;
            }
            for (int remove_id = 시설_부절태; remove_id <= 시설_연병소; remove_id++)
            {
                if (pk::has_facility(city, remove_id))
                {
                    removed = remove_facility(city, building_self, remove_id);
                    if (removed) return;
                }
            }
            if (pk::has_facility(city, 시설_군둔농))
            {
                removed = remove_facility(city, building_self, 시설_군둔농);
                if (removed) return;
            }

            count_facility = get_facility_count(city, 시설_곡창);
            if (count_facility > 1)    // 곡창 여러개 지었으면 완성건물은 철거
            {
                for (int i = 0; i < count_facility; i++)
                {
                    removed = remove_facility(city, building_self, 시설_곡창);
                    if (removed) return;
                }
            }
            count_facility = get_facility_count(city, 시설_조폐);
            if (count_facility > 1)    // 조폐 여러개 지었으면 완성건물은 철거
            {
                for (int i = 0; i < count_facility; i++)
                {
                    removed = remove_facility(city, building_self, 시설_조폐);
                    if (removed) return;
                }
            }

            if (pk::has_facility(city, 시설_공방))
            {
                removed = remove_facility(city, building_self, 시설_공방);
                if (removed) return;
            }
            if (pk::has_facility(city, 시설_조선))
            {
                removed = remove_facility(city, building_self, 시설_조선);
                if (removed) return;
            }

        }

        // 완성된 시설 강제철거
        bool remove_facility(pk::city@ city, pk::building@ building_self, int remove_id)
        {
            pk::district@ district = pk::get_district(city.get_district_id());
            int gold_cost = int(pk::get_facility(remove_id).gold_cost);

            for (int i = 0; i < int(city.max_devs); i++)
            {
                pk::building@ building = city.dev[i].building;
                if (pk::is_alive(building))
                {
                    if (building.facility == remove_id and building.completed and building.pos != building_self.pos)
                    {
                        // 강제 철거를 위한 행동력과 철거시설 비용 보전
                        pk::add_ap(district,10);// district.ap += 10;
                        pk::add_gold(city, gold_cost, false);

                        // 강제철거
                        pk::destroy_cmd_info cmd_destroy;
                        @cmd_destroy.building = building;
                        pk::command(cmd_destroy);

                        return true;    // 철거완료
                    }
                }
            }
            return false;   // 철거실패
        }

        // 완성된 시설수 카운트 함수
        int get_facility_count(pk::city@ city, int count_id)
        {
            int count = 0;
            for (int i = 0; i < int(city.max_devs); i++)
            {
                pk::building@ building = city.dev[i].building;
                if (pk::is_alive(building))
                {
                    if (building.facility == count_id)
                        count += 1;
                }
            }
            return count;
        }

        //---------------------------------------------------------------------------------------           
    }



    Main main;
}


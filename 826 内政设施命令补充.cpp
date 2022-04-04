// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/10/18 # 江东新风 # 部分变量汉化 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2020/10/30 # 江东新风 # 同步马术书大神的更新 ##  
/*
@제작자: 기마책사
@Update: '19.11.25  / 최초적용: 내정시설 건설임무정보표시, 흡수합병메뉴, 재료시설건설메뉴, 중앙시설건설메뉴
@Update: '20.8.8    / 수정내용: 재료시설건설메뉴 오류수정, 중앙시설건설메뉴 수정(인접한 조폐/곡창 유무 검색 추가)
@Update: '20.8.21   / 수정내용: 건설임무 잔여기간 표시수식 수정 (정치 낮은 무장에 대한 최소치 보정값 반영)
@Update: '20.9.2    / 수정적용: 내정시설내구도보정 기능 추가, 무장/좌표 자동선택 기능 추가, 최대 건설기간 100일 제한에 따른 무장선택 로직 수정
@Update: '20.9.20   / 수정적용: 거점주변에 적부대 확인
*/

namespace 内政设施命令扩充
{
    //=======================================================================================

    // 내정시설 명력메뉴 유저 설정 (true = on, false = off)
    const bool  건설임무정보_바로표시 = false;    // 내정시설 건설임무정보 표시 (true: 바로 말풍선표시 / false: 메뉴클릭확인)
    const bool  개발지_최적_검색설정 = true;     // 빈개발지 건설위치 또는 흡수합병재료 최적 검색기능 ON/OFF (true 권장)
    const bool  개발지_위치_자동선택 = true;     // 건설 및 흡수합병 대상위치 다수인 경우 (true: 자동선택 / false: 좌표 수동선택 확인창)
    const bool  개발지_무장_자동선택 = true;     // 건설 및 흡수합병 실행무장 다수인 경우 (true: 자동선택 / false: 무장 수동선택 확인창)
    const bool  개발실행_무장수_제한 = true;     // 내정시설 개발 무장수 제한 ON/OFF (true : 미행동 무장수 비례하여 실행무장 제한 / false : 미행동 무장수 상관없이 최대 3명까지 선택)

    //---------------------------------------------------------------------------------------
    // 내정시설 내구도 보정 유저 설정 (true = on, false = off)
    const bool  내정시설_초기내구_추가모드 = false;   // 개발 착수 시 기본 초기내구도에 추가로 내구도 추가 설정
    const bool  내정시설_초기내구_AI만 = false;    // true =AI만 효과적용, false =유저와 AI 모두 효과 적용
    const float 내정시설_초기내구_추가비율 = 0.25f;   // 개발 착수 시 기본 초기내구도 25%에 추가로 더할 내구도 비율 (0.0f~0.75f)

    const bool  내정시설_합병내구_추가모드 = false;   // 흡수합병 시 초기내구도에 추가로 내구도 추가 설정
    const bool  내정시설_합병내구_AI만 = false;    // true =AI만 효과적용, false =유저와 AI 모두 효과 적용
    const float 내정시설_합병내구_추가비율 = 0.50f;   // 개발 착수 시 기본 초기내구도 25%에 추가로 더할 내구도 비율 (0.0f~0.9f)

    const bool  내정시설_매턴내구_보조모드 = false;   // 건설중인 내정시설의 매턴 초기내구도 상승보조 설정
    const bool  내정시설_매턴내구_AI만 = false;    // true =AI만 효과적용, false =유저와 AI 모두 효과 적용
    const int   내정시설_매턴내구_신규개발 = 100;     // 내정건설 시 매턴 내구도 증가량
    const int   내정시설_매턴내구_吸收合并 = 100;     // 흡수합병 시 매턴 내구도 증가량

    //=======================================================================================


    class Main
    {

        Main()
        {
            //---------------------------------------------------------------------------------------
            // 내정시설 내구도 보정
            pk::bind(160, pk::trigger160_t(onBuildingCreate));
            pk::bind(112, pk::trigger112_t(onTurnEnd));

            //---------------------------------------------------------------------------------------
            // 내정시설 명력메뉴 추가
            pk::menu_item item_吸收合并;
            item_吸收合并.menu = 0;        // 상위메뉴
            item_吸收合并.pos = 0;         // 메뉴위치
            item_吸收合并.init = pk::building_menu_item_init_t(init_内政菜单);
            item_吸收合并.get_image_id = pk::menu_item_get_image_id_t(getImageID_green);     // 녹색 메뉴
            item_吸收合并.is_visible = pk::menu_item_is_visible_t(isVisible_吸收合并);
            item_吸收合并.is_enabled = pk::menu_item_is_enabled_t(isEnabled_吸收合并);
            item_吸收合并.get_text = pk::menu_item_get_text_t(getText_吸收合并);
            item_吸收合并.get_desc = pk::menu_item_get_desc_t(getDesc_吸收合并);
            item_吸收合并.handler = pk::menu_item_handler_t(handler_吸收合并);
            pk::add_menu_item(item_吸收合并);

            pk::menu_item item_材料建设;
            item_材料建设.menu = 0;        // 상위메뉴
            item_材料建设.pos = 1;         // 메뉴위치
            item_材料建设.init = pk::building_menu_item_init_t(init_内政菜单);
            item_材料建设.get_image_id = pk::menu_item_get_image_id_t(getImageID_green);     // 녹색 메뉴
            item_材料建设.is_visible = pk::menu_item_is_visible_t(isVisible_材料建设);
            item_材料建设.is_enabled = pk::menu_item_is_enabled_t(isEnabled_材料建设);
            item_材料建设.get_text = pk::menu_item_get_text_t(getText_材料建设);
            item_材料建设.get_desc = pk::menu_item_get_desc_t(getDesc_材料建设);
            item_材料建设.handler = pk::menu_item_handler_t(handler_材料建设);
            pk::add_menu_item(item_材料建设);

            pk::menu_item item_中央建设;
            item_中央建设.menu = 0;        // 상위메뉴
            item_中央建设.pos = 2;         // 메뉴위치
            item_中央建设.init = pk::building_menu_item_init_t(init_内政菜单);
            item_中央建设.get_image_id = pk::menu_item_get_image_id_t(getImageID_green);     // 녹색 메뉴
            item_中央建设.is_visible = pk::menu_item_is_visible_t(isVisible_中央建设);
            item_中央建设.is_enabled = pk::menu_item_is_enabled_t(isEnabled_中央建设);
            item_中央建设.get_text = pk::menu_item_get_text_t(getText_中央建设);
            item_中央建设.get_desc = pk::menu_item_get_desc_t(getDesc_中央建设);
            item_中央建设.handler = pk::menu_item_handler_t(handler_中央建设);
            pk::add_menu_item(item_中央建设);

            pk::menu_item item_任务信息;
            item_任务信息.menu = 0;        // 상위메뉴
            item_任务信息.pos = 2;         // 메뉴위치
            item_任务信息.init = pk::building_menu_item_init_t(init_内政菜单);
            item_任务信息.get_image_id = pk::menu_item_get_image_id_t(getImageID_orange);     // 주황색 메뉴
            item_任务信息.is_visible = pk::menu_item_is_visible_t(isVisible_任务信息);
            item_任务信息.is_enabled = pk::menu_item_is_enabled_t(isEnabled_任务信息);
            item_任务信息.get_text = pk::menu_item_get_text_t(getText_任务信息);
            item_任务信息.get_desc = pk::menu_item_get_desc_t(getDesc_任务信息);
            item_任务信息.handler = pk::menu_item_handler_t(handler_任务信息);
            pk::add_menu_item(item_任务信息);

        }

        //---------------------------------------------------------------------------------------
        // 수정 금지
        pk::building@ building;
        pk::building@ base;
        pk::city@ city;
        string city_name;
        pk::list<pk::person@> list_base_person;
        pk::list<pk::person@> list_idle_person;
        pk::list<pk::building@> target_merge_list;
        pk::array<pk::point> target_dev_pos;
        pk::person@ kunshu;
        pk::person@ taishu;
        pk::district@ district;
        pk::force@ force;

        bool ai_force_ctrl;
        bool district_ctrl;
        bool merge_disable;
        bool center_facility_nearby;
        bool building_hp_full;
        bool enemies_around;
        int city_gold;
        int facility_id;
        int upgrade_id;
        int upgrade_cost;
        int develop_id;
        int develop_cost;
        int center_id;
        int center_cost;

        int ACTION_COST_吸收合并 = 10;
        int ACTION_COST_内政开发 = 20;
        //---------------------------------------------------------------------------------------

        int getImageID_blue()    // 파란색
        {
            return 856;
        }
        int getImageID_green()    // 녹색
        {
            return 889;
        }
        int getImageID_orange()   // 주황색
        {
            return 1249;
        }

        //---------------------------------------------------------------------------------------

        void init_内政菜单(pk::building@ building_t)
        {
            @building = @building_t;
            @city = pk::get_city(pk::get_city_id(building.pos));
            @base = pk::city_to_building(city);
            city_name = pk::decode(pk::get_name(city));
            city_gold = pk::get_gold(base);
            list_base_person = pk::get_person_list(base, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
            list_idle_person = pk::get_idle_person_list(base);

            @kunshu = pk::get_person(pk::get_kunshu_id(base));
            @taishu = pk::get_person(pk::get_taishu_id(base));
            @district = pk::get_district(base.get_district_id());
            @force = pk::get_force(building.get_force_id());

            facility_id = building.facility;
            building_hp_full = (building.hp >= pk::get_facility(facility_id).hp) ? true : false;

            get_upgrade_gold_cost();
            get_merge_target_merge_list();
            get_develop_gold_cost();
            get_center_gold_cost();
            get_dev_pos_nearby();
            check_center_nearby();

            ai_force_ctrl = (int(pk::option["San11Option.FreeOperate"]) == 1) ? true : false;
            district_ctrl = bool(pk::core["sub_district_controls"]);
            merge_disable = bool(pk::core["disable_merge"]);
            enemies_around = pk::enemies_around(base);

            if (건설임무정보_바로표시) report_construct(); // 건설 중인 무장 표시
        }

        //---------------------------------------------------------------------------------------
        //  내정시설_내구도 보정 (건설착수)
        //---------------------------------------------------------------------------------------

        void onBuildingCreate(pk::building@ building, int type)
        {
            if (pk::is_campaign()) return;

            int facility_id = building.facility;
            if (facility_id < 시설_동작대 or facility_id > 시설_마구간3단) return; // 내정시설 아니면 종료
            if (building.completed) return; // 완성된 시설이면 종료

            pk::facility@ facility = pk::get_facility(facility_id);
            uint16 hp_before = building.hp;
            uint16 hp_after = building.hp;

            // 내정시설_초기내구_추가모드
            if (내정시설_초기내구_추가모드 and !(내정시설_초기내구_AI만 and building.is_player()))
            {
                if (시설_동작대 <= facility_id and facility_id <= 시설_군둔농)    // 1단 시설인 경우
                {
                    float hp_add_ratio = pk::min(pk::max(0.f, 내정시설_초기내구_추가비율), 0.75f);  // 입력범위제한 : 0%~75%
                    uint16 hp_add_value = uint16(float(facility.hp) * hp_add_ratio);
                    hp_after = hp_before + hp_add_value;
                    building.hp = hp_after;
                }
            }

            // 내정시설_흡수합병내구_추가모드
            if (내정시설_합병내구_추가모드 and !(내정시설_합병내구_AI만 and building.is_player()))
            {
                int downgrade_id = get_downgrade_id(facility_id);
                if (downgrade_id != -1)    // 2단, 3단 시설인 경우
                {
                    float hp_add_ratio = pk::min(pk::max(0.f, 내정시설_합병내구_추가비율), 0.9f);  // 입력범위제한 : 0%~90%
                    uint16 hp_delta = facility.hp - pk::get_facility(downgrade_id).hp;
                    uint16 hp_add_value = uint16(float(hp_delta) * hp_add_ratio);
                    hp_after = hp_before + hp_add_value;
                    building.hp = hp_after;
                }
            }


            if (调试模式 and (hp_before < hp_after))
            {
                string facility_name = pk::decode(pk::get_name(facility));
                pk::point pos = building.pos;
                pk::info(pk::format("{}({},{}) 耐久追加 {} → {}", facility_name, pos.x, pos.y, hp_before, hp_after));
            }

        }

        //---------------------------------------------------------------------------------------
        //  내정시설_내구도 보정 (매턴)
        //---------------------------------------------------------------------------------------

        void onTurnEnd(pk::force@ force)
        {
            if (pk::is_campaign()) return;
            if (!내정시설_매턴내구_보조모드) return;

            //pk::force@ force = pk::get_force(pk::get_current_turn_force_id());
            if (!pk::is_normal_force(force)) return;
            if (내정시설_매턴내구_AI만 and force.is_player()) return;

            uint16 hp_add_develop = pk::max(0, 내정시설_매턴내구_신규개발);
            uint16 hp_add_merge = pk::max(0, 내정시설_매턴내구_吸收合并);

            array<pk::city@> arr_city_list = pk::list_to_array(pk::get_city_list(force));
            for (int i = 0; i < int(arr_city_list.length); i++)
            {
                pk::city@ city = arr_city_list[i];
                for (int j = 0; j < city.max_devs; j++)
                {
                    int facility_id = -1;
                    pk::building@ building = city.dev[j].building;
                    if (pk::is_alive(building) and !building.completed)
                    {
                        facility_id = building.facility;
                        pk::facility@ facility = pk::get_facility(facility_id);
                        uint16 hp_add_turn = 0;

                        if (get_downgrade_id(facility_id) != -1)    // 2단, 3단 시설인 경우
                            hp_add_turn = hp_add_merge;
                        else
                            hp_add_turn = hp_add_develop;

                        pk::add_hp(building, pk::min(hp_add_turn, (facility.hp - building.hp - 1)), true);
                    }
                }
            }

        }

        //---------------------------------------------------------------------------------------
        //---------------------------------------------------------------------------------------

        // 흡수합병 중인 시설의 이전단계 시설ID 번환
        int get_downgrade_id(int facility_id)
        {
            int downgrade_id = -1;
            switch (facility_id)
            {
            case 시설_시장2단:  downgrade_id = 시설_시장; break;
            case 시설_시장3단:  downgrade_id = 시설_시장2단; break;
            case 시설_농장2단:  downgrade_id = 시설_농장; break;
            case 시설_농장3단:  downgrade_id = 시설_농장2단; break;

            case 시설_병영2단:  downgrade_id = 시설_병영; break;
            case 시설_병영3단:  downgrade_id = 시설_병영2단; break;
            case 시설_대장간2단:  downgrade_id = 시설_대장간; break;
            case 시설_대장간3단:  downgrade_id = 시설_대장간2단; break;
            case 시설_마구간2단:  downgrade_id = 시설_마구간; break;
            case 시설_마구간3단:  downgrade_id = 시설_마구간2단; break;
            }

            return downgrade_id;
        }


        bool check_facility_upgrade(int facility_id)
        {
            if (facility_id == 시설_시장 or facility_id == 시설_시장2단 or
                facility_id == 시설_농장 or facility_id == 시설_농장2단 or
                facility_id == 시설_병영 or facility_id == 시설_병영2단 or
                facility_id == 시설_대장간 or facility_id == 시설_대장간2단 or
                facility_id == 시설_마구간 or facility_id == 시설_마구간2단)
                return true;

            return false;
        }

        // 흡수합병 비용
        void get_upgrade_gold_cost()
        {
            upgrade_id = -1;
            switch (facility_id)
            {
            case 시설_시장:  upgrade_id = 시설_시장2단; break;
            case 시설_시장2단:  upgrade_id = 시설_시장3단; break;
            case 시설_농장:  upgrade_id = 시설_농장2단; break;
            case 시설_농장2단:  upgrade_id = 시설_농장3단; break;

            case 시설_병영:  upgrade_id = 시설_병영2단; break;
            case 시설_병영2단:  upgrade_id = 시설_병영3단; break;
            case 시설_대장간:  upgrade_id = 시설_대장간2단; break;
            case 시설_대장간2단:  upgrade_id = 시설_대장간3단; break;
            case 시설_마구간:  upgrade_id = 시설_마구간2단; break;
            case 시설_마구간2단:  upgrade_id = 시설_마구간3단; break;
            }

            if (upgrade_id >= 0) upgrade_cost = int(pk::get_facility(upgrade_id).gold_cost);
            else upgrade_cost = -1;
        }

        // 주변에 흡수합병 재료 검색
        void get_merge_target_merge_list()
        {
            target_merge_list.clear();

            if (!check_facility_upgrade(facility_id)) return;

            for (int i = 0; i < 방향_끝; i++)
            {
                pk::point neighbor_pos = pk::get_neighbor_pos(building.pos, i);
                if (pk::is_valid_pos(neighbor_pos))
                {
                    pk::building@ target_building = pk::get_building(neighbor_pos);
                    if (pk::is_alive(target_building) and target_building.completed)
                    {
                        if ((facility_id == 시설_시장 or facility_id == 시설_시장2단) and target_building.facility == 시설_시장)    target_merge_list.add(target_building);
                        else if ((facility_id == 시설_농장 or facility_id == 시설_농장2단) and target_building.facility == 시설_농장)    target_merge_list.add(target_building);
                        else if ((facility_id == 시설_병영 or facility_id == 시설_병영2단) and target_building.facility == 시설_병영)    target_merge_list.add(target_building);
                        else if ((facility_id == 시설_대장간 or facility_id == 시설_대장간2단) and target_building.facility == 시설_대장간)  target_merge_list.add(target_building);
                        else if ((facility_id == 시설_마구간 or facility_id == 시설_마구간2단) and target_building.facility == 시설_마구간)  target_merge_list.add(target_building);
                    }
                }
            }
        }

        // 재료건설 비용
        void get_develop_gold_cost()
        {
            develop_id = -1;
            switch (facility_id)
            {
            case 시설_암시장:  develop_id = 시설_암시장; break;

            case 시설_시장:  develop_id = 시설_시장; break;
            case 시설_시장2단:  develop_id = 시설_시장; break;
            case 시설_시장3단:  develop_id = 시설_시장; break;
            case 시설_조폐:  develop_id = 시설_시장; break;
            case 시설_농장:  develop_id = 시설_농장; break;
            case 시설_농장2단:  develop_id = 시설_농장; break;
            case 시설_농장3단:  develop_id = 시설_농장; break;
            case 시설_곡창:  develop_id = 시설_농장; break;

            case 시설_병영:  develop_id = 시설_병영; break;
            case 시설_병영2단:  develop_id = 시설_병영; break;
            case 시설_대장간:  develop_id = 시설_대장간; break;
            case 시설_대장간2단:  develop_id = 시설_대장간; break;
            case 시설_마구간:  develop_id = 시설_마구간; break;
            case 시설_마구간2단:  develop_id = 시설_마구간; break;
            }

            if (develop_id >= 0) develop_cost = int(pk::get_facility(develop_id).gold_cost);
            else develop_cost = -1;
        }


        // 중앙건설 비용
        void get_center_gold_cost()
        {
            center_id = -1;
            switch (facility_id)
            {
            case 시설_시장:  center_id = 시설_조폐; break;
            case 시설_시장2단:  center_id = 시설_조폐; break;
            case 시설_시장3단:  center_id = 시설_조폐; break;
            case 시설_농장:  center_id = 시설_곡창; break;
            case 시설_농장2단:  center_id = 시설_곡창; break;
            case 시설_농장3단:  center_id = 시설_곡창; break;
            }

            if (center_id >= 0) center_cost = int(pk::get_facility(center_id).gold_cost);
            else center_cost = -1;
        }

        // 주변에 조폐/곡창이 있는지 확인
        void check_center_nearby()
        {
            center_facility_nearby = false;
            for (int i = 0; i < 방향_끝; i++)
            {
                pk::point neighbor_pos = pk::get_neighbor_pos(building.pos, i);
                if (pk::is_valid_pos(neighbor_pos))
                {
                    pk::building@ building_t = pk::get_building(neighbor_pos);
                    if (pk::is_alive(building_t) and building_t.completed)
                    {
                        if ((facility_id == 시설_시장 or facility_id == 시설_시장2단 or facility_id == 시설_시장3단) and building_t.facility == 시설_조폐)  center_facility_nearby = true;
                        else if ((facility_id == 시설_농장 or facility_id == 시설_농장2단 or facility_id == 시설_농장3단) and building_t.facility == 시설_곡창)  center_facility_nearby = true;
                    }
                }
            }
        }

        // 주변에 빈 건설지 확인
        void get_dev_pos_nearby()
        {
            pk::array<pk::point> empty_pos;
            for (int i = 0; i < 방향_끝; i++)
            {
                pk::point neighbor_pos = pk::get_neighbor_pos(building.pos, i);
                if (pk::is_valid_pos(neighbor_pos))
                {
                    pk::hex@ hex = pk::get_hex(neighbor_pos);
                    if (!hex.has_building and !hex.has_unit)
                    {
                        for (int j = 0; j < int(city.max_devs); j++)
                        {
                            if (city.dev[j].pos == neighbor_pos)
                                empty_pos.insertLast(neighbor_pos);
                        }
                    }
                }
            }
            target_dev_pos = empty_pos;

        }


        //---------------------------------------------------------------------------------------

        // 개발지 위치 우선순위 정렬
        void sort_dev_pos(int city_dev_id)
        {
            // 건설 가능 개발지 개수 확인
            if (int(target_dev_pos.length) < 2) return;

            if (city_dev_id == develop_id)  // 재료시설
            {
                target_dev_pos.sort(function(a, b)
                {
                    int count_pos_a = main.count_dev_pos(a);
                    int count_pos_b = main.count_dev_pos(b);

                    // 주변에 동일계열의 시설이 있는 건설지를 우선 대상으로 정렬 (흡수합병하기 유리한 위치)
                    // 주변에 동일계열의 시설이 다수인 경우는 제외 (조폐/곡창 건설지일 가능성)
                    if (count_pos_a == 1 and count_pos_b == 0) return true;
                    if (count_pos_a == 0 and count_pos_b == 1) return false;
                    // 주변에 개발지 아닌 좌표가 많은 건설지를 우선 대상으로 정렬 (외곽 위치일 가능성)
                    return main.count_norm_pos(a) > main.count_norm_pos(b);
                });
            }
            else if (city_dev_id == center_id)  // 조폐, 곡창
            {
                target_dev_pos.sort(function(a, b)
                {
                    int count_pos_a = main.count_center_pos(a);
                    int count_pos_b = main.count_center_pos(b);

                    // 주변에 재료시설이 많은 건설지를 우선 대상으로 정렬 (흡수합병하기 유리한 위치)
                    if (count_pos_a > count_pos_b) return true;
                    if (count_pos_a < count_pos_b) return false;
                    // 주변에 개발지 아닌 좌표가 적은 건설지를 우선 대상으로 정렬 (안쪽 위치일 가능성)
                    return main.count_norm_pos(a) < main.count_norm_pos(b);
                });
            }
        }

        // 흡수합병 시설 우선순위 정렬
        void sort_merge_list()
        {
            // 흡수합병 재료 개수 확인
            if (int(target_merge_list.count) < 2) return;

            target_merge_list.sort(function(a, b)
            {
                // 주변에 개발지 아닌 좌표가 적은 건설지를 우선 대상으로 정렬 (안쪽 위치일 가능성)
                return main.count_norm_pos(a.pos) < main.count_norm_pos(b.pos);
            });

        }

        // 주변의 개발지 아닌 좌표수 계산
        int count_norm_pos(pk::point pos_t)
        {
            int count_pos = 0;
            for (int i = 0; i < 방향_끝; i++)
            {
                bool check_dev_pos = false;
                pk::point neighbor_pos = pk::get_neighbor_pos(pos_t, i);
                if (pk::is_valid_pos(neighbor_pos))
                {
                    int j = 0;
                    while (!check_dev_pos and (j < int(city.max_devs)))
                    {
                        if (city.dev[j].pos == neighbor_pos) check_dev_pos = true;
                        j++;
                    }
                }
                if (!check_dev_pos) count_pos++;
            }
            return count_pos;
        }

        // 주변의 upgrade 가능 동일계열 시설수 계산
        int count_dev_pos(pk::point pos_t)
        {
            int count_pos = 0;
            for (int i = 0; i < 방향_끝; i++)
            {
                pk::point neighbor_pos = pk::get_neighbor_pos(pos_t, i);
                if (pk::is_valid_pos(neighbor_pos))
                {
                    pk::building@ building_t = pk::get_building(neighbor_pos);
                    if (pk::is_alive(building_t) and building_t.pos != building.pos)
                    {
                        int f_id_t = building_t.facility;
                        if ((f_id_t == 시설_시장 or f_id_t == 시설_시장2단) and develop_id == 시설_시장) count_pos++;
                        else if ((f_id_t == 시설_농장 or f_id_t == 시설_농장2단) and develop_id == 시설_농장) count_pos++;
                        else if ((f_id_t == 시설_병영 or f_id_t == 시설_병영2단) and develop_id == 시설_병영) count_pos++;
                        else if ((f_id_t == 시설_대장간 or f_id_t == 시설_대장간2단) and develop_id == 시설_대장간) count_pos++;
                        else if ((f_id_t == 시설_마구간 or f_id_t == 시설_마구간2단) and develop_id == 시설_마구간) count_pos++;
                    }
                }
            }
            return count_pos;
        }

        // 주변의 버프 가능한 시설수 계산
        int count_center_pos(pk::point pos_t)
        {
            int count_pos = 0;
            for (int i = 0; i < 방향_끝; i++)
            {
                pk::point neighbor_pos = pk::get_neighbor_pos(pos_t, i);
                if (pk::is_valid_pos(neighbor_pos))
                {
                    pk::building@ building_t = pk::get_building(neighbor_pos);
                    if (pk::is_alive(building_t))
                    {
                        int f_id_t = building_t.facility;
                        if ((f_id_t == 시설_시장 or f_id_t == 시설_시장2단 or f_id_t == 시설_시장3단) and center_id == 시설_조폐) count_pos++;
                        else if ((f_id_t == 시설_농장 or f_id_t == 시설_농장2단 or f_id_t == 시설_농장3단) and center_id == 시설_곡창) count_pos++;
                    }
                }
            }
            return count_pos;
        }


        //---------------------------------------------------------------------------------------
        //---------------------------------------------------------------------------------------
        pk::point target_pos_t;

        void goto_facility()
        {
            pk::point pos = target_pos_t;
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

            pk::highlight(target_pos_t, true);
        }
        void remove_highlights()
        {
            pk::remove_all_highlights();
        }


        //---------------------------------------------------------------------------------------
        // 해당 시설에서 건설 중인 무장 임무보고
        //---------------------------------------------------------------------------------------
        void report_construct()
        {
            if (building.completed) return;     // 건설완료 건물은 표시 제외
            if (list_base_person.count == 0) return;    // 소속 도시의 소속 무장 없는 경우 제외
            if (building.facility < 시설_동작대 or building.facility > 시설_마구간3단) return;

            pk::facility@ facility = pk::get_facility(building.facility);

            string actor_name;
            pk::person@ actor;
            int order = -1;
            float stat_sum = 0.0f;
            pk::array<pk::person@> arr_actors;
            pk::array<pk::person@> arr_base_person = pk::list_to_array(list_base_person);
            for (int i = 0; i < int(arr_base_person.length); i++)
            {
                pk::person@ person_t = arr_base_person[i];
                if (person_t.target[0] == building.get_id())
                {
                    order = person_t.order;
                    if (order == 武将任务_吸收合并)    // 흡수합병
                    {
                        @actor = person_t;
                        actor_name = pk::decode(pk::get_name(actor));
                        stat_sum = float(person_t.stat[武将能力_政治]) * (4.0f / 3.0f + 0.1f);
                    }
                    else if (order == 武将任务_开发)   // 일반개발
                    {
                        arr_actors.insertLast(person_t);
                        @actor = pk::get_person(person_t.target[1]);

                        if (person_t.get_id() == actor.get_id())
                            stat_sum += float(person_t.stat[武将能力_政治]) * 1.5f;
                        else
                            stat_sum += float(person_t.stat[武将能力_政治]) * 0.5f;

                        if (actor_name == "") actor_name = pk::decode(pk::get_name(person_t));
                        else actor_name += "," + pk::decode(pk::get_name(person_t));

                        stat_sum = pk::max(stat_sum, float(int(facility.hp * 0.75f / 9.f)));    // 게임 내에 반영된 최대건설기간 제한값
                    }
                }
            }

            // 임무대상 시설의 잔여 건설내구도 확인
            uint16 hp_delta = uint16(facility.hp - building.hp);
            int remain_turn = int(calc_remain_turn(hp_delta, order, stat_sum));

            // 임무보고
            if (pk::is_alive(actor))
            {
                // 음성
                if (kunshu.get_id() == actor.get_id()) pk::play_voice(0x3E, actor);
                else                                   pk::play_voice(0x3C, actor);

                // 말풍선
                int remain_period = 10 + 10 * remain_turn;
                string msg_actor = (kunshu.get_id() == actor.get_id()) ? "正在执行建造任务" : "正在执行建造任务";
                pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x{}\n剩余任务期间：\x1b[1x{}日\x1b[0x", msg_actor, actor_name, remain_period)), actor, building);
            }

            return;
        }

        // 잔여 건설기간 계산
        float calc_remain_turn(uint16 hp_delta, int order, float stat_sum)
        {
            // 내정시설_내구도_매턴보조 모드 사용 시
            uint16 hp_add_turn = 0;
            if (내정시설_매턴내구_보조모드 and !(내정시설_매턴내구_AI만 and building.is_player()))
            {
                if (order == 武将任务_吸收合并) hp_add_turn = uint16(pk::max(0, 내정시설_매턴내구_吸收合并));
                else if (order == 武将任务_开发) hp_add_turn = uint16(pk::max(0, 내정시설_매턴내구_신규개발));

                stat_sum += float(hp_add_turn); // 내구도 보조량 추가
            }

            // 잔여 임무기간 계산
            float remain_turn = float(hp_delta) / stat_sum;

            return remain_turn;
        }


        //---------------------------------------------------------------------------------------
        //     건설임무정보 표시 메뉴
        //---------------------------------------------------------------------------------------

        string getText_任务信息()
        {
            return pk::encode("任务确认");
        }

        bool isVisible_任务信息()
        {
            if (pk::is_campaign()) return false;
            else if (building.get_id() < 据点_末 or building.completed) return false;
            else if (!ai_force_ctrl and !building.is_player()) return false;   // 컴퓨터 AI 세력
            else if (!district_ctrl and district.no > 1) return false;
            else if (building.facility < 시설_동작대 or building.facility > 시설_마구간3단) return false;
            else if (건설임무정보_바로표시) return false;
            return true;
        }

        bool isEnabled_任务信息()
        {
            if (pk::is_campaign()) return false;
            else if (building.get_id() < 据点_末 or building.completed) return false;
            else if (building.facility < 시설_동작대 or building.facility > 시설_마구간3단) return false;
            return true;
        }

        string getDesc_任务信息()
        {
            if (building.get_id() < 据点_末 or building.completed)
                return pk::encode("不是建设中的内政设施");
            else if (building.facility < 시설_동작대 or building.facility > 시설_마구간3단)
                return pk::encode("不是内政设施");
            return pk::encode("内政设施开发中的武将的任务显示");
        }

        bool handler_任务信息()
        {
            report_construct();
            return true;
        }


        //---------------------------------------------------------------------------------------
        //     흡수합병 메뉴
        //---------------------------------------------------------------------------------------

        string getText_吸收合并()
        {
            return pk::encode("吸收合并");
        }

        bool isVisible_吸收合并()
        {
            if (pk::is_campaign()) return false;
            else if (building.get_id() < 据点_末 or upgrade_id < 0) return false;
            else if (!ai_force_ctrl and !building.is_player()) return false;   // 컴퓨터 AI 세력
            else if (!district_ctrl and district.no > 1) return false;
            return true;
        }

        bool isEnabled_吸收合并()
        {
            if (pk::is_campaign()) return false;
            else if (building.get_id() < 据点_末 or upgrade_id < 0) return false;
            else if (merge_disable) return false;
            else if (!building.completed) return false;
            else if (target_merge_list.count == 0) return false;
            else if (list_idle_person.count < 1) return false;
            else if (int(district.ap) < ACTION_COST_吸收合并)  return false;
            else if (city_gold < upgrade_cost or upgrade_cost < 0) return false;
            else if (!building_hp_full) return false;
            else if (enemies_around) return false;
            return true;
        }

        string getDesc_吸收合并()
        {
            if (building.get_id() < 据点_末 or upgrade_id < 0)
                return pk::encode("不是能吸收合并的内政设施.");
            else if (merge_disable)
                return pk::encode("吸收合并在游戏设定中被禁止");
            else if (!building.completed)
                return pk::encode("设施正在建造中");
            else if (target_merge_list.count == 0)
                return pk::encode("周围没有可以吸收合并的设施");
            else if (list_idle_person.count < 1)
                return pk::encode("没有可执行的武将.");
            else if (int(district.ap) < ACTION_COST_吸收合并)
                return pk::encode(pk::format("行动力不足 (需要{}行动力)", ACTION_COST_吸收合并));
            else if (city_gold < upgrade_cost or upgrade_cost < 0)
                return pk::encode(pk::format("金不足 (需要{}金)", upgrade_cost));
            else if (!building_hp_full)
                return pk::encode("设施耐久不足");
            else if (enemies_around)
                return pk::encode("设施周围有敌军");
            return pk::encode(pk::format("执行吸收合并 (需要{}行动力, {}金)", ACTION_COST_吸收合并, upgrade_cost));
        }


        bool handler_吸收合并()
        {
            if (list_idle_person.count == 0) return true;
            if (target_merge_list.count == 0) return true;

            if (개발지_최적_검색설정) sort_merge_list();

            // 흡수합병 재료 임의선택
            pk::array<pk::building@> arr_target = pk::list_to_array(target_merge_list);
            pk::building@ target = arr_target[0];

            // 흡수합병에 따른 내구도 차이 계산
            pk::facility@ facility_default = pk::get_facility(facility_id);
            pk::facility@ facility_upgrade = pk::get_facility(upgrade_id);
            uint16 hp_delta = uint16(facility_upgrade.hp - facility_default.hp);

            // 내정시설_흡수합병내구_추가모드 사용 시 : 잔여 내구도 차이 계산
            if (내정시설_합병내구_추가모드 and !(내정시설_합병내구_AI만 and building.is_player()))
            {
                if (upgrade_id != -1)    // 2단, 3단 시설인 경우
                {
                    float hp_add_ratio = pk::min(pk::max(0.f, 내정시설_합병내구_추가비율), 0.9f);  // 입력범위제한 : 0%~90%
                    hp_delta -= uint16(float(hp_delta) * hp_add_ratio);
                }
            }

            // 정치력이 높은 순으로 정렬
            list_idle_person.sort(function(a, b)
            {
                return a.stat[武将能力_政治] > b.stat[武将能力_政治];
            });

            // 기준 작업일수 및 최소능력치 계산
            pk::array<pk::person@> arr_idle_person = pk::list_to_array(list_idle_person);
            float stat_weight = (4.0f / 3.0f + 0.1f);
            float max_stat = float(arr_idle_person[0].stat[武将能力_政治]) * stat_weight;

            if (내정시설_매턴내구_보조모드 and !(내정시설_매턴내구_AI만 and building.is_player()))
                max_stat += float(pk::max(0, 내정시설_매턴내구_吸收合并));  // 매턴 내구도 보조량 추가

            int   ref_per = int(float(hp_delta) / max_stat) + (((float(hp_delta) % max_stat) > 0) ? 1 : 0); // 건설기간 산정 (올림)
            float ref_stat = float(hp_delta) / float(ref_per);                                      // 기준 합산능력치


            // 실행무장 자동선택
            pk::person@ actor;
            float sum_stat = 0.0f;

            if (내정시설_매턴내구_보조모드 and !(내정시설_매턴내구_AI만 and building.is_player()))
                sum_stat += float(pk::max(0, 내정시설_매턴내구_吸收合并));  // 매턴 내구도 보조량 추가

            for (int j = (int(arr_idle_person.length) - 1); j >= 0; j--)
            {
                @actor = arr_idle_person[j];
                if ((float(actor.stat[武将能力_政治]) * stat_weight + sum_stat) > ref_stat) break;
            }

            string facility_name = pk::decode(pk::get_name(facility_upgrade));

            // 실행무장 선택창
            pk::list<pk::person@> person_sel;
            person_sel.add(actor);

            bool confirm = false;
            while (!confirm)
            {
                // 예상기간
                @actor = person_sel[0];
                float est_stat = float(actor.stat[武将能力_政治]) * stat_weight;
                int est_turn = int(calc_remain_turn(hp_delta, 武将任务_吸收合并, est_stat));
                int est_period = 10 + 10 * est_turn;
                int est_gold = city_gold - upgrade_cost;

                // 흡수합병 재료가 1개 또는 개발지_자동_선택모드 경우 하이라이트 표시
                if (개발지_위치_자동선택 or int(arr_target.length) == 1)
                {
                    target_pos_t = arr_target[0].pos;
                    pk::scene(pk::scene_t(goto_facility));
                }

                if (개발지_무장_자동선택 or int(arr_idle_person.length) == 1)
                {
                    string actor_name = pk::decode(pk::get_name(actor));
                    bool answer = pk::yes_no(pk::encode(pk::format("让\x1b[1x{}\x1b[0x执行吸收合并吗?\n(需要\x1b[1x{}日\x1b[0x, \x1b[2x{}\x1b[0x金 {}→\x1b[17x{}\x1b[0x)", actor_name, est_period, city_name, city_gold, est_gold)));

                    // 흡수합병 재료가 1개 또는 개발지_자동_선택모드 경우 하이라이트 제거
                    if (개발지_위치_자동선택 or int(arr_target.length) == 1)
                        pk::scene(pk::scene_t(remove_highlights));

                    confirm = true;
                    if (!answer) return true;     // 취소 종료
                }
                else if (!개발지_무장_자동선택 and int(arr_idle_person.length) > 1)
                {
                    string msg_est = (kunshu.get_id() == actor.get_id()) ? pk::format("主公，执行该操作，需要\x1b[1x{}日\x1b[0x，是否执行?", est_period) : pk::format("执行该操作，需要\x1b[1x{}日\x1b[0x，是否执行?", est_period);
                    int answer = pk::choose({ pk::encode("执行"), pk::encode("变更"), pk::encode("取消") },
                        pk::encode(msg_est), actor);

                    // 흡수합병 재료가 1개 또는 개발지_자동_선택모드 경우 하이라이트 제거
                    if (개발지_위치_자동선택 or int(arr_target.length) == 1)
                        pk::scene(pk::scene_t(remove_highlights));

                    if (answer == 0)
                        confirm = true;
                    else if (answer == 1)
                    {
                        confirm = false;
                        person_sel = pk::person_selector(pk::encode("执行武将选择"), pk::encode("选择用于吸收合并的武将。"), list_idle_person, 1, 1, person_sel, 1);
                        if (person_sel.count == 0) return true;
                    }
                    else return true;
                }

                @actor = person_sel[0];
            }


            // 흡수합병 재료 재확인 (재료가 여러개인 경우)
            if (int(arr_target.length) > 1 and !개발지_위치_자동선택)
            {
                int sel = 0;
                int max = int(arr_target.length);
                string msg_confirm = (kunshu.get_id() == actor.get_id()) ? "主公，要吸收合并哪个设施?" : "要吸收合并哪个设施?";

                bool _confirm = false;
                while (!_confirm)
                {
                    target_pos_t = arr_target[sel].pos;
                    pk::scene(pk::scene_t(goto_facility));

                    int answer = pk::choose({ pk::encode("选择"), pk::encode("其次"), pk::encode("迁移"), pk::encode("取消") },
                        pk::encode(msg_confirm), actor);

                    pk::scene(pk::scene_t(remove_highlights));

                    // 선택 결과 처리
                    if (answer == 0) _confirm = true;
                    else if (answer == 1)
                    {
                        if (sel >= (max - 1)) sel = 0;
                        else sel++;
                        _confirm = false;
                    }
                    else if (answer == 2)
                    {
                        if (sel <= 0) sel = (max - 1);
                        else sel--;
                        _confirm = false;
                    }
                    else return true;   // 취소 종료

                    @target = @arr_target[sel]; // 시설선택
                }
            }

            // 말풍선
            string msg_merge = (kunshu.get_id() == actor.get_id()) ? "是否合并" : "是否合并";
            pk::say(pk::encode(pk::format("\x1b[2x{}\x1b[0x{}", facility_name, msg_merge)), actor, building);

            // 흡수합병 명령정보
            pk::merge_cmd_info cmd_merge;
            @cmd_merge.base = @base;
            @cmd_merge.actor = @actor;
            @cmd_merge.src = @target;
            @cmd_merge.dst = @building;
            // 흡수합병 명령실행
            pk::command(cmd_merge);

            pk::combat_text(-upgrade_cost, 3, base.pos); // 흡수합병 실행에 의한 금 감소 표시

            return true;

        }


        //---------------------------------------------------------------------------------------
        //     재료건설 메뉴
        //---------------------------------------------------------------------------------------

        string getText_材料建设()
        {
            return pk::encode("建造" + pk::decode(pk::get_name(pk::get_facility(develop_id))));
        }

        bool isVisible_材料建设()
        {
            if (pk::is_campaign()) return false;
            else if (building.get_id() < 据点_末 or develop_id < 0) return false;
            else if (!ai_force_ctrl and !building.is_player()) return false;   // 컴퓨터 AI 세력
            else if (!district_ctrl and district.no > 1) return false;
            return true;
        }

        bool isEnabled_材料建设()
        {
            if (pk::is_campaign()) return false;
            else if (building.get_id() < 据点_末 or develop_id < 0) return false;
            else if (city.num_devs == city.max_devs) return false;
            else if (target_dev_pos.length == 0) return false;
            else if (list_idle_person.count < 1) return false;
            else if (int(district.ap) < ACTION_COST_内政开发)  return false;
            else if (city_gold < develop_cost or develop_cost < 0) return false;
            else if (enemies_around) return false;
            return true;
        }

        string getDesc_材料建设()
        {
            if (building.get_id() < 据点_末 or develop_id < 0)
                return pk::encode("不是可以建造设施的地方.");
            else if (city.num_devs == city.max_devs)
                return pk::encode("城市周围没有空地");
            else if (target_dev_pos.length == 0)
                return pk::encode("城市周围没有空地");
            else if (list_idle_person.count < 1)
                return pk::encode("没有可执行的武将");
            else if (int(district.ap) < ACTION_COST_内政开发)
                return pk::encode(pk::format("行动力不足 (需要{}行动力)", ACTION_COST_内政开发));
            else if (city_gold < develop_cost or develop_cost < 0)
                return pk::encode(pk::format("金不足(需要{}金)", develop_cost));
            else if (enemies_around)
                return pk::encode("周围有敌人");
            return pk::encode(pk::format("建设材料设施(需要{}行动力,{}金)", ACTION_COST_内政开发, develop_cost));
        }


        bool handler_材料建设()
        {
            return handler_内政开发(develop_id);
        }


        //---------------------------------------------------------------------------------------
        //     조폐/곡창건설 메뉴造币/谷仓建设菜单
        //---------------------------------------------------------------------------------------

        string getText_中央建设()
        {
            return pk::encode("建造" + pk::decode(pk::get_name(pk::get_facility(center_id))));
        }

        bool isVisible_中央建设()
        {
            if (pk::is_campaign()) return false;
            else if (building.get_id() < 据点_末 or center_id < 0) return false;
            else if (!ai_force_ctrl and !building.is_player()) return false;   // 컴퓨터 AI 세력
            else if (!district_ctrl and district.no > 1) return false;
            return true;
        }

        bool isEnabled_中央建设()
        {
            if (pk::is_campaign()) return false;
            else if (building.get_id() < 据点_末 or center_id < 0) return false;
            else if (city.num_devs == city.max_devs) return false;
            else if (target_dev_pos.length == 0) return false;
            else if (list_idle_person.count < 1) return false;
            else if (int(district.ap) < ACTION_COST_内政开发)  return false;
            else if (center_facility_nearby) return false;
            else if (city_gold < center_cost or center_cost < 0) return false;
            else if (enemies_around) return false;
            return true;
        }

        string getDesc_中央建设()
        {
            if (building.get_id() < 据点_末 or center_id < 0)
                return pk::encode("只能在市场或农场周围开发");
            else if (city.num_devs == city.max_devs)
                return pk::encode("城市周围没有空地");
            else if (target_dev_pos.length == 0)
                return pk::encode("城市周围没有空地");
            else if (list_idle_person.count < 1)
                return pk::encode("没有可执行的武将");
            else if (int(district.ap) < ACTION_COST_内政开发)
                return pk::encode(pk::format("行动力不足 (需要{}行动力)", ACTION_COST_内政开发));
            else if (city_gold < center_cost or center_cost < 0)
                return pk::encode(pk::format("金不足(需要{}金)", center_cost));
            else if (center_facility_nearby)
                return pk::encode(pk::format("周围已经有邻近的{}了", pk::decode(pk::get_name(pk::get_facility(center_id)))));
            else if (enemies_around)
                return pk::encode("周围有敌人");
            return pk::encode(pk::format("建设{} (需要{}行动力, {}金)", pk::decode(pk::get_name(pk::get_facility(center_id))), ACTION_COST_内政开发, center_cost));
        }

        bool handler_中央建设()
        {
            return handler_内政开发(center_id);
        }

        //---------------------------------------------------------------------------------------
        // 개발 명령
        bool handler_内政开发(int city_dev_id)
        {
            if (list_idle_person.count == 0) return true;
            if (int(target_dev_pos.length) == 0) return true;

            if (개발지_최적_검색설정) sort_dev_pos(city_dev_id);

            // 개발위치 임의선택
            pk::point target_pos = target_dev_pos[0];

            // 시설 내구도
            pk::facility@ facility_dev = pk::get_facility(city_dev_id);
            string facility_name = pk::decode(pk::get_name(facility_dev));
            string city_name = pk::decode(pk::get_name(city));
            uint16 hp_delta = uint16(facility_dev.hp - facility_dev.hp / 4);

            // 내정시설_초기내구_추가모드 사용 시 : 잔여 내구도 차이값 계산
            if (내정시설_초기내구_추가모드 and !(내정시설_초기내구_AI만 and building.is_player()))
            {
                if (시설_동작대 <= city_dev_id and city_dev_id <= 시설_군둔농)    // 1단 시설인 경우
                {
                    float hp_add_ratio = pk::min(pk::max(0.f, 내정시설_초기내구_추가비율), 0.75f);  // 입력범위제한 : 0%~75%
                    hp_delta -= uint16(float(facility_dev.hp) * hp_add_ratio);
                }
            }

            // 정치력이 높은 순으로 정렬
            list_idle_person.sort(function(a, b)
            {
                return a.stat[武将能力_政治] > b.stat[武将能力_政治];
            });

            // 실행 무장수 선정
            pk::array<pk::person@> arr_idle_person = pk::list_to_array(list_idle_person);
            int idle_cnt = int(arr_idle_person.length);
            int act_mod = (개발실행_무장수_제한) ? 2 : 1;
            int act_add = (city_dev_id == center_id) ? 1 : 0;
            int min_actors = pk::min(3, pk::max(1, pk::min(idle_cnt, (act_add + idle_cnt / act_mod))));     // 적정인원 자동선택

            // 기준 작업일수 및 최소능력치 계산
            float max_stat = 0.0f;
            for (int i = 0; i < min_actors; i++)    //최대가능 능력치합산
            {
                float stat_weight = (i == 0) ? 1.5f : 0.5f;
                max_stat += float(arr_idle_person[i].stat[武将能力_政治]) * stat_weight;
            }
            if (내정시설_매턴내구_보조모드 and !(내정시설_매턴내구_AI만 and building.is_player()))
                max_stat += float(pk::max(0, 내정시설_매턴내구_신규개발));  // 매턴 내구도 보조량 추가

            int   ref_per = int(float(hp_delta) / max_stat) + (((float(hp_delta) % max_stat) > 0) ? 1 : 0); // 건설기간 산정 (올림)
            float ref_stat = float(hp_delta) / float(ref_per);                                      // 기준 합산능력치
            float min_stat = float(hp_delta) / 9.f;     // 건설기간 최대 100일 기준

            bool single_actor_mode = false;  // 일반 선택
            if (ref_stat < min_stat)         // 건설기간 최대 100일 
            {
                single_actor_mode = true;
                min_actors = 1;
            }

            // 적정인원 실행무장 자동선택
            pk::list<pk::person@> person_sel;
            float sum_stat = 0.0f;

            if (내정시설_매턴내구_보조모드 and !(내정시설_매턴내구_AI만 and building.is_player()))
                sum_stat += float(pk::max(0, 내정시설_매턴내구_신규개발));  // 매턴 내구도 보조량 추가

            for (int i = 0; i < min_actors; i++)
            {
                float stat_weight = (i == 0) ? 1.5f : 0.5f;
                pk::person@ actor_t;
                if (single_actor_mode)      // 건설기간 최대 100일 리미트 시 무장선택
                {
                    for (int j = 0; j < int(arr_idle_person.length); j++)
                    {
                        if (!person_sel.contains(arr_idle_person[j]))
                        {
                            @actor_t = arr_idle_person[j];
                            if ((float(actor_t.stat[武将能力_政治]) * stat_weight + sum_stat) > min_stat) break;
                        }
                    }
                }
                else    // 일반적인 무장선택
                {
                    for (int j = (int(arr_idle_person.length) - 1); j >= 0; j--)
                    {
                        if (!person_sel.contains(arr_idle_person[j]))
                        {
                            @actor_t = arr_idle_person[j];
                            if ((float(actor_t.stat[武将能力_政治]) * stat_weight + sum_stat) > ref_stat) break;
                        }
                    }
                }
                if (pk::is_alive(actor_t))
                {
                    person_sel.add(actor_t);
                    sum_stat += actor_t.stat[武将能力_政治] * stat_weight;
                }
                if (sum_stat > ref_stat) break;
            }

            if (调试模式)
                pk::info(pk::format("{}开发, 最多{}名, 最少{}名, 标准能力{}, 选择{}位", facility_name, min_actors, 10 * ref_per, ref_stat, person_sel.count));


            // 실행무장 재지정 확인창
            pk::array<pk::person@> actors = pk::list_to_array(person_sel);
            pk::person@ leader;
            bool confirm = false;
            while (!confirm)
            {
                // 선택무장 능력치 합산, 이름 저장
                sum_stat = 0.0f;
                string actor_name;
                for (int i = 0; i < int(actors.length); i++)
                {
                    pk::person@ actor_t = actors[i];
                    if (i == 0)
                    {
                        sum_stat += float(actor_t.stat[武将能力_政治]) * 1.5f;
                        actor_name = pk::decode(pk::get_name(actor_t));
                    }
                    else
                    {
                        sum_stat += float(actor_t.stat[武将能力_政治]) * 0.5f;
                        actor_name = actor_name + ", " + pk::decode(pk::get_name(actor_t));
                    }
                }

                // 대표무장 선택
                if (person_sel.contains(kunshu)) @leader = kunshu;
                else @leader = actors[0];
                string msg_sel = (kunshu.get_id() == leader.get_id()) ? "主公" : "";

                // 예상기간
                float est_stat = pk::max(sum_stat, float(int(facility_dev.hp * 0.75f / 9.f)));
                int est_turn = int(calc_remain_turn(hp_delta, 武将任务_开发, est_stat));
                int est_period = 10 + 10 * est_turn;
                int est_gold = city_gold - ((city_dev_id == develop_id) ? develop_cost : center_cost);

                // 개발지가 1개 또는 개발지_자동_선택모드 인 경우 하이라이트 표시
                if (개발지_위치_자동선택 or int(target_dev_pos.length) == 1)
                {
                    target_pos_t = target_dev_pos[0];
                    pk::scene(pk::scene_t(goto_facility));
                }

                if (개발지_무장_자동선택 or int(arr_idle_person.length) == 1)
                {
                    bool answer = pk::yes_no(pk::encode(pk::format("要让\x1b[1x{}\x1b[0x执行\x1b[2x{}\x1b[0x的开发吗?\n (需要\x1b[1x{}\x1b[0x日, \x1b[2x{}\x1b[0x金 {}→\x1b[17x{}\x1b[0x)", actor_name, facility_name, est_period, city_name, city_gold, est_gold)));

                    // 개발지가 1개 또는 개발지_자동_선택모드 인 경우 하이라이트 제거
                    if (개발지_위치_자동선택 or int(target_dev_pos.length) == 1)
                        pk::scene(pk::scene_t(remove_highlights));

                    confirm = true;
                    if (!answer) return true;     // 취소 종료
                }
                else if (!개발지_무장_자동선택 and int(arr_idle_person.length) > 1)
                {
                    int answer = pk::choose({ pk::encode("执行"), pk::encode("变更"), pk::encode("取消") },
                        pk::encode(pk::format("要让\x1b[1x{}\x1b[0x执行\x1b[2x{}\x1b[0x 的开发吗{}?\n (需要\x1b[1x{}\x1b[0x日, \x1b[2x{}\x1b[0x金 {}→\x1b[17x{}\x1b[0x)", actor_name, facility_name, msg_sel, est_period, city_name, city_gold, est_gold)),
                        leader);
                    // 개발지가 1개 또는 개발지_자동_선택모드 인 경우 하이라이트 제거
                    if (개발지_위치_자동선택 or int(target_dev_pos.length) == 1)
                        pk::scene(pk::scene_t(remove_highlights));

                    if (answer == 0)
                        confirm = true;
                    else if (answer == 1)
                    {
                        confirm = false;

                        // 실행무장 선택변경
                        person_sel = pk::person_selector(pk::encode("执行武将选择"), pk::encode("选择要开发内政设施的武将"), list_idle_person, 1, pk::min(3, int(arr_idle_person.length)), person_sel, 0);
                        if (person_sel.count == 0) return true;

                    }
                    else return true;
                }

                actors = pk::list_to_array(person_sel);
            }

            if (调试模式)
            {
                for (int i = 0; i < int(target_dev_pos.length); i++)
                {
                    pk::point dev_pos = target_dev_pos[i];
                    pk::info(pk::format("{} 建设可能:: x坐标:{}, y坐标:{}", facility_name, dev_pos.x, dev_pos.y));
                }
            }

            // 개발지 재확인 (빈 개발지가 여러개인 경우)
            if (int(target_dev_pos.length) > 1 and !개발지_위치_자동선택)
            {
                int sel = 0;
                int max = int(target_dev_pos.length);

                bool _confirm = false;
                while (!_confirm)
                {
                    target_pos_t = target_dev_pos[sel];
                    pk::scene(pk::scene_t(goto_facility));

                    string msg_sel = (kunshu.get_id() == leader.get_id()) ? "主公，" : "";
                    int answer = pk::choose({ pk::encode("选择"), pk::encode("其次"), pk::encode("迁移"), pk::encode("取消") },
                        pk::encode(pk::format("{}在这里开发\x1b[1x{}\x1b[0x吗?", msg_sel, facility_name)),
                        leader);

                    pk::scene(pk::scene_t(remove_highlights));

                    // 선택 결과 처리
                    if (answer == 0) _confirm = true;
                    else if (answer == 1)
                    {
                        if (sel >= (max - 1)) sel = 0;
                        else sel++;
                        _confirm = false;
                    }
                    else if (answer == 2)
                    {
                        if (sel <= 0) sel = (max - 1);
                        else sel--;
                        _confirm = false;
                    }
                    else return true;   // 취소 종료

                    target_pos = target_pos_t; // 시설선택
                }

            }

            // 개발 명령정보
            pk::develop_cmd_info cmd_develop;
            @cmd_develop.base = @base;
            for (int i = 0; i < int(actors.length); i++)
            {
                @cmd_develop.actors[i] = actors[i];
            }
            cmd_develop.facility = city_dev_id;
            cmd_develop.pos = target_pos;

            // 개발 명령실행
            pk::command(cmd_develop);

            pk::combat_text(-develop_cost, 3, base.pos); // 개발 실행에 의한 금 감소 표시

            return true;

        }

        //---------------------------------------------------------------------------------------

        bool 调试模式 = false;

    }

    Main main;
}


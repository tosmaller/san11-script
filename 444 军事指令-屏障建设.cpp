// ## 2021/03/09 # 白马叔叔 # 空指针修正 ##
// ## 2020/10/30 # 江东新风 # 同步马术书大神的更新 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/09 # 氕氘氚 # 加入【军事】菜单 ##
/*
@제작자: 기마책사
@Update: '18.12.24  / 최초적용: 도시 주위에 방벽 시설건설
@Update: '19.1.20   / 변경내용: 개발지 회피

*/

namespace 城市屏障建设
{

    // ================ CUSTOMIZE ================

    const int 방벽건설거리 = 3;              // 도시로부터 떨어진 거리 (최소거리=3)
    const int 단위_ACTION_COST = 5;  // 방벽 1개당 행동력 필요량
    const int 단위_TP_COST_방벽 = 30;  // 방벽 1개당 기교 필요량

    // ===========================================

    class Main
    {

        pk::building@ building;
        pk::city@ city;
        pk::point building_pos;
        pk::person@ taishu;
        int building_gold;
        int building_id;
        int force_id;
        pk::force@ force;
        array<pk::point> arr;

        int count_hex;
        int facility_id;
        int answer;

        int GOLD_COST_방벽;
        int ACTION_COST;
        int TP_COST_방벽;

        Main()
        {

            pk::menu_item item;
            item.menu = 101;
            // item.pos = 9;
            item.shortcut = global_menu::shortcut_平障建设;
            item.init = pk::building_menu_item_init_t(init);
            item.is_visible = pk::menu_item_is_visible_t(isVisible);
            item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            item.get_text = pk::menu_item_get_text_t(getText);
            item.get_desc = pk::menu_item_get_desc_t(getDesc);
            item.handler = pk::menu_item_handler_t(handler);
            pk::add_menu_item(item);

        }


        void init(pk::building@ building_t)
        {

            @building = @building_t;
            @city = building.get_id() < 城市_末 ? pk::building_to_city(building) : null;
            building_gold = pk::get_gold(building);
            building_id = building.get_id();
            building_pos = building.pos;

            force_id = building.get_force_id();
            @force = pk::get_force(force_id);

			@taishu = pk::get_taishu_id(building) == -1 ? null : pk::get_person(pk::get_taishu_id(building));

            if (pk::has_tech(force, 기교_석조건축))
                facility_id = 시설_석벽;
            else
                facility_id = 시설_토루;

            // 시설 비용 가져오기
            int facility_gold = pk::get_facility(facility_id).gold_cost;

            // 계산 (설치 갯수에 따라 가변)
            build_defence_wall(false);
            GOLD_COST_방벽 = count_hex * facility_gold;
            ACTION_COST = count_hex * 단위_ACTION_COST;
            TP_COST_방벽 = count_hex * 단위_TP_COST_방벽;

        }

        string getText()
        {
            return pk::encode("屏障建设");
        }

        string getDesc()
        {
			if (taishu is null)
				return pk::encode("无太守无法执行.");
            else if (!pk::is_alive(taishu) or taishu.action_done)
                return pk::encode("太守不在或太守未待命");
            else if (count_hex <= 0)
                return pk::encode("没有可建造的地形");
            else if (int(force.tp) < TP_COST_방벽)
                return pk::encode(pk::format("技巧点不足.(技巧点 {})", TP_COST_방벽));
            else if (int(pk::get_district(pk::get_district_id(force, 1)).ap) < ACTION_COST)
                return pk::encode(pk::format("行动力不足 (需要行动力 {} )", ACTION_COST));
            else if (building_gold < GOLD_COST_방벽)
                return pk::encode(pk::format("资金不足 (需要资金{} )", GOLD_COST_방벽));
            return pk::encode(pk::format("周围建设墙壁 (消耗技巧点{}, 行动力{}, 资金{} )", TP_COST_방벽, ACTION_COST, GOLD_COST_방벽));
        }

        bool isVisible()
        {
            if (pk::is_campaign()) return false;
            if (building_id >= 城市_末) return false;
            return true;
        }


        bool isEnabled()
        {
			if (taishu is null) return false;
            else if (!pk::is_alive(taishu) or taishu.action_done) return false;
            else if (count_hex <= 0) return false;
            else if (int(force.tp) < TP_COST_방벽)  return false;
            else if (int(pk::get_district(pk::get_district_id(force, 1)).ap) < ACTION_COST) return false;
            else if (building_gold < GOLD_COST_방벽) return false;
            return true;
        }

        bool handler()
        {
            if (force.is_player())
            {
                pk::scene(pk::scene_t(scene_방벽));
                if (answer == 1) return false;
            }

            // 음성효과
            if (pk::is_in_screen(building_pos))
            {
                switch (pk::rand(2))
                {
                case 0: pk::play_voice(0x39, taishu); break;   // 음성 : 설치
                case 1: pk::play_voice(0x3a, taishu); break;   // 음성 : 설치
                }
            }
            // 대화표시
            string facility_name = pk::decode(pk::get_facility(facility_id).name);
            pk::say(pk::encode(pk::format("在城周围建设\x1b[1x{}\x1b[0x", facility_name)), taishu);
            taishu.action_done = true;

            // 건설 조건에 맞는 지형에 설치
            build_defence_wall(true);

            // 실행 도시 금 소비
            pk::add_gold(city, -GOLD_COST_방벽, true);

            // 행동력 감소
            auto district = pk::get_district(pk::get_district_id(force, 1));
            pk::add_ap(district, -ACTION_COST);

            // 기교 감소
            ch::add_tp(force, -TP_COST_방벽, force.get_pos());

            return true;

        }

        // 건설 배경화면 호출
        void scene_방벽()
        {
            pk::background(8);

            string city_name = pk::decode(pk::get_name(city));
            string facility_name = pk::decode(pk::get_facility(facility_id).name);
            answer = pk::choose({ pk::encode(" 是 "), pk::encode(" 否 ") },
                pk::encode(pk::format("是否在\x1b[2x{}\x1b[0x周围建设{}个\x1b[1x{}\x1b[0x?\n(消耗技巧点{},行动力{},资金{})", city_name, count_hex, facility_name, TP_COST_방벽, ACTION_COST, GOLD_COST_방벽)),
                taishu);

            pk::background(-1);
        }

        // 방벽 갯수 카운트 및 설치 실행 함수
        void build_defence_wall(bool build_wall)
        {
            count_hex = 0;
            int range = pk::max(3, 방벽건설거리) + ((building_id < 城市_末) ? 1 : 0);
            arr = pk::ring(building_pos, range);
            for (int i = 0; i < int(arr.length); i++)
            {
                pk::point pos = arr[i];
                pk::hex@ hex = pk::get_hex(pos);

                // 6개 방향 비우기
                bool enabled_pos = true;
                for (int dir = 0; dir < 방향_끝; dir++)
                {
                    pk::point chk_pos = pk::get_neighbor_pos(building_pos, dir, range);
                    if (chk_pos == pos)
                        enabled_pos = false;
                }

                if (!hex.has_building and !hex.has_unit and pk::is_enabled_terrain(hex.terrain) and enabled_pos and !check_develop_pos(pos)
                    and building_id < 城市_末 and (hex.terrain <= 지형_습지 or hex.terrain == 지형_황무지))
                {
                    count_hex += 1;

                    if (build_wall)
                    {
                        pk::create_building(pk::get_facility(facility_id), pos, force_id);
                        pk::building@ facility = pk::get_building(pos);
                        pk::complete(facility);
                    }
                }
            }
        }

        bool check_develop_pos(pk::point pos_t)
        {
			if (city is null) return false;
            for (int i = 0; i < city.max_devs; i++)
            {
                if (city.dev[i].pos == pos_t)
                    return true;
            }
            return false;
        }

    }

    Main main;
}
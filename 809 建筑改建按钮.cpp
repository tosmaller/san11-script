// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/07/26 ##
/*
*** Notice
본 스크립트에 대한 재배포, 무단수정을 금지 합니다.
개선을 빙자한 잘라먹기, 덮어씌우기 방식의 별도 패치 역시 거부 합니다.
필요사항 경우 쪽지나 메일을 통해 요청을 주시면 검토 후 반영하겠습니다.

*** 제작
	제르나 -	삼국지 공방		(https://cafe.naver.com/rtkbank)
			삼국지 도원결의	(https://cafe.naver.com/sam10)

*** 환경
	삼국지11 PK - PK2.1

*** History ***
--------------------------------------------------------------------
		작 업 자			| 수정 일자	|		작 업 내 용
--------------------------------------------------------------------
	제르나	- 공방		| 19.01.01	| 초기생성
	제르나	- 공방		| 19.01.02	| v1.1
									| 금 소모 수정
	제르나	- 공방		| 19.01.06	| v2.0
									| AI 사용기능 추가
	제르나	- 공방		| 19.01.29	| v2.1
									| [기타.xml] 설정 미적용 수정
*/

namespace 개축
{
	class Main
	{
		// ----- Set ----- //
		int						action_cost = 30;	// 행동력
		int						gold_cost_add = 100;	// 추가 비용(사용자)

		bool					ai_use = true;	// AI 동작 여부
		bool					ai_use_facility_max = true;	// AI 동작 기준 - 시설 Max
		int						ai_standard_gold = 1500;	// AI 동작 기준 - 금
		int						ai_standard_person = 3;	// AI 동작 기준 - 무장

		pk::building@			building_;
		pk::force@				force_;
		pk::city@				city_;

		pk::list<pk::person@>	list_person;
		array<pk::person@>		arr_person;
		array<pk::person@>		arr_person_sel;

		array<array<int>>		arr_facility =
		{
			{31, 50, 51},
			{32, 52, 53},
			{33, 54, 55},
			{34, 56, 57},
			{35, 58, 59}
		};
		int						facility_type = -1;
		int						facility_level = -1;
		int						facility_gold_cost;
		int						facility_gold_cost2;
		int						facility_time_cost;

		Main()
		{
			pk::bind(111, pk::trigger111_t(callback));
			add_menu();
		}

		//----- 메뉴 설정 -----//
		void add_menu()
		{
			pk::menu_item item;
			item.menu = 0;
			item.pos = 10;
			item.shortcut = "U";
			item.init = pk::building_menu_item_init_t(init);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.is_visible = pk::menu_item_is_visible_t(isVisible);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		//----- 메뉴 설정 처리 -----//
		bool isVisible()
		{
			// [기타.xml] 설정
			if (!check_condition(0))
				return false;

			// 시설 종류
			if (!check_condition(1))
				return false;

			// 건설 완료
			if (!check_condition(2))
				return false;

			return true;
		}

		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building_.get_force_id());
			@city_ = pk::get_city(pk::get_city_id(building_.get_pos()));

			// 도시 무장
			set_data(0);

			// 시설(선택)
			set_data(1);
			if (!check_condition(1))	return;

			// 비용
			set_data(2);
		}

		void set_data(int mode)
		{
			// 도시 무장
			if (mode == 0)
			{
				array<pk::person@> arr_person_temp = pk::list_to_array(pk::get_person_list(pk::city_to_building(city_), pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)));
				list_person.clear();
				for (int i = 0; i < int(arr_person_temp.length); i++)
				{
					if (!arr_person_temp[i].action_done
						and arr_person_temp[i].location == pk::city_to_building(city_).get_id())
						list_person.add(arr_person_temp[i]);
				}
				arr_person = pk::list_to_array(list_person);
			}

			// 시설(선택) - building
			if (mode == 1)
			{
				facility_type = -1;
				facility_level = -1;
				for (int i = 0; i < int(arr_facility.length); i++)
				{
					for (int j = 0; j < 2; j++)
					{
						if (building_.facility == arr_facility[i][j])
						{
							facility_type = i;
							facility_level = j;
							break;
						}
					}
				}
			}

			// 비용
			if (mode == 2)
			{
				facility_gold_cost = pk::get_facility(arr_facility[facility_type][0]).gold_cost + pk::get_facility(arr_facility[facility_type][facility_level + 1]).gold_cost;
				facility_gold_cost2 = pk::get_facility(arr_facility[facility_type][0]).gold_cost;

				if (force_.is_player())
				{
					facility_gold_cost += gold_cost_add;
					facility_gold_cost2 += gold_cost_add;
				}
			}
		}

		bool isEnabled()
		{
			// 행동력
			if (!check_condition(10))
				return false;

			// 도시 금
			if (!check_condition(11))
				return false;

			// 무장 수
			if (!check_condition(12))
				return false;

			return true;
		}

		string getText()
		{
			return pk::encode("改建");
		}

		string getDesc()
		{
			// 행동력
			if (!check_condition(10))
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", action_cost));

			// 도시 금
			if (!check_condition(11))
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", facility_gold_cost));

			// 무장 수
			if (!check_condition(12))
				return pk::encode("没有可执行的武将.");


			return pk::encode(pk::format("重建内政设施. ({}行动力, {}资金)", action_cost, facility_gold_cost));
		}

		bool check_condition(int mode)
		{
			// [기타.xml] 설정
			if (mode == 0)
			{
				if (bool(pk::core["facility_to_level3"]) or bool(pk::core["disable_merge"]))
					return false;
			}

			// 시설 종류
			if (mode == 1)
			{
				if (facility_type < 0 or facility_level < 0)
					return false;
			}

			// 건설 완료
			if (mode == 2)
			{
				if (!building_.completed)
					return false;
			}

			// 행동력
			if (mode == 10)
			{
				if (int(pk::get_district(city_.get_district_id()).ap) < action_cost)
					return false;
			}

			// 도시 금
			if (mode == 11)
			{
				if (city_.gold < facility_gold_cost)
					return false;
				if (!force_.is_player()
					and city_.gold < ai_standard_gold)
					return false;
			}

			// 무장 수
			if (mode == 12)
			{
				if (arr_person.length < 1)
					return false;
				if (!force_.is_player()
					and int(arr_person.length) < ai_standard_person)
					return false;
			}

			// 도시 주변 적 - building
			if (mode == 13)
			{
				if (pk::enemies_around(building_))
					return false;
				if (pk::enemies_around(pk::city_to_building(city_)))
					return false;
			}

			return true;
		}

		//----- 메뉴 동작 처리 -----//
		bool handler()
		{
			// 내정 시설 : return false -> Error
			// 무장 선택
			pk::list<pk::person@> list_temp;
			arr_person_sel = pk::list_to_array(pk::person_selector(pk::encode("选择武将"), pk::encode("选择目标武将"), list_person, 1, 3, list_temp, 1));
			if (arr_person_sel.length < 1) return true;

			// 소요 일수
			set_time_cost();

			// 진행 확인
			bool act_confirm = false;
			act_confirm = pk::yes_no(pk::encode(pk::format("对\x1b[1x{}\x1b[0x进行翻建.\n({}行动力, {}资金, 所需{}天)", pk::decode(pk::get_facility(building_.facility).name), action_cost, facility_gold_cost, facility_time_cost * 10)));
			if (!act_confirm) return true;

			// 변경 명령
			update_building();

			// 금 지출
			pk::add_gold(city_, -facility_gold_cost2, true);

			return true;
		}

		void set_time_cost()
		{
			int need_hp = 0;
			int sum_stat = 0;

			// 개축 내구
			need_hp = pk::get_facility(arr_facility[facility_type][facility_level + 1]).hp;

			// 수행 무장
			for (int i = 0; i < int(arr_person_sel.length); i++)
			{
				sum_stat = sum_stat + arr_person_sel[i].stat[武将能力_政治];
			}
			facility_time_cost = int(need_hp / (sum_stat * 2)) + (need_hp % (sum_stat * 2) == 0 ? 0 : 1);
		}

		void update_building()
		{
			pk::develop_cmd_info cmd_develop;
			@cmd_develop.base = pk::city_to_building(city_);
			for (int i = 0; i < int(arr_person_sel.length); i++)
			{
				@cmd_develop.actors[i] = arr_person_sel[i];
			}
			cmd_develop.facility = arr_facility[facility_type][facility_level + 1];
			cmd_develop.pos = building_.pos;

			pk::kill(building_, false);
			pk::district@ district = pk::get_district(city_.get_district_id());
			pk::add_ap(district, -10);
			bool result = pk::command(cmd_develop);
		}

		//----- 트리거 동작 처리 -----//
		void callback(pk::force@ force)
		{
			if (force.is_player()
				or !check_condition(0)
				or !ai_use)
			{
				return;
			}
			else
			{
				@force_ = force;
				handler_AI();
			}
		}

		void handler_AI()
		{
			// 도시
			array<pk::city@> arr_city_ai = pk::list_to_array(pk::get_city_list(force_));

			for (int i = 0; i < int(arr_city_ai.length); i++)
			{
				@city_ = arr_city_ai[i];

				// 행동력
				if (!check_condition(10))
					continue;

				// 도시 무장
				set_data(0);
				if (!check_condition(12))
					continue;

				// 시설(검색)
				if (!search_facility())
					continue;

				// 비용
				set_data(2);

				// 도시 금
				if (!check_condition(11))
					continue;

				// 무장 선택
				select_person();

				// 변경 명령
				update_building();
				break;
			}
		}

		bool search_facility()
		{
			// 시설 수
			if (ai_use_facility_max and city_.max_devs > city_.num_devs)
				return false;

			// 시설(검색) - city
			for (int i = 0; i < int(city_.max_devs); i++)
			{
				@building_ = city_.dev[i].building;
				if (pk::is_alive(building_))
				{
					if (building_.completed)
						set_data(1);
				}

				if (check_condition(1))
					break;
			}

			if (!check_condition(1) or !check_condition(13) or !check_neighbor_pos())
				return false;
			else
				return true;
		}

		bool check_neighbor_pos()
		{
			for (int j = 0; j < 방향_끝; j++)
			{
				pk::point target_pos = pk::get_neighbor_pos(building_.pos, j);
				if (pk::is_valid_pos(target_pos))
				{
					pk::building@ target_building = pk::get_building(target_pos);
					if (pk::is_alive(target_building)
						and target_building.facility == arr_facility[facility_type][0])
						return false;
				}
			}

			return true;
		}

		void select_person()
		{
			array<pk::person@> arr_person_temp = pk::list_to_array(list_person);
			int temp_id = -1;
			int temp_stat = -1;

			for (int i = 0; i < int(arr_person_temp.length); i++)
			{
				if (temp_stat < int(arr_person_temp[i].stat[武将能力_政治]))
				{
					temp_id = arr_person_temp[i].get_id();
					temp_stat = arr_person_temp[i].stat[武将能力_政治];
				}
			}

			pk::list<pk::person@> list_selected_person;
			list_selected_person.clear();
			list_selected_person.add(pk::get_person(temp_id));

			arr_person_sel = pk::list_to_array(list_selected_person);
		}
	}

	Main main;
}
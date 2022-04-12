// ## 2021/01/17 #江东新风# 势力重新部署时考虑支城的部署 ##
// ## 2020/12/19 #江东新风# 排除5异族城市 ##
// ## 2020/08/01 # messi # 区分?一个势力部署##
/*
	@원본 : 세력재배치.cpp (PK2.1 기본 제공 파일)
	@수정 : masterpiecek
	@도움 : 제르나


	※ '세력재배치.cpp' (PK2.1 기본 제공 파일) 의 자동 재배치 기능에 수동 재배치 기능 추가
	 - 패치 적용 시 기존의 '세력재배치.cpp' 파일은 비활성화 하시기 바랍니다.


	[ 세력재배치 실행방식 및 재배치 대상항목 설명 ]

	1. 재배치 실행 방식
	 1) 'shift + 우클릭' 메뉴 실행
	  - 제르나님 base_menu_세력재배치_2.0.cpp' 실행 방식을 참고함 ( https://cafe.naver.com/rtkbank/17 )
	  - 튜토리얼, 결전제패, 미션 중 메뉴 실행 불가
	  - 시나리오 최초 시작 후 첫 턴이 진행되면 실행 불가 (첫 턴을 진행하지 않고 저장했다가 불러오기 한 경우는 가능)
	 2) '시나리오 시작' 시 자동 실행
	  - '세력재배치.cpp'의 기존 실행 방식을 참고함 (PK2.1 기본 제공 파일)
	  - 유저 설정에서 활성화 여부 설정 가능 (설정 기본값 : 비활성화)
	  - 튜토리얼, 결전제패, 미션 중에는 실행되지 않음

	2. 재배치 가능 항목
	 1) 모든 세력의 도시를 자동 재배치 (PK2.1에서 기본으로 제공하는 '세력재배치.cpp'의 기존 설정 항목)
	 2) 황제의 소속 도시를 자동 재배치 (추가 기능)
	 3) 특정 세력별로 해당 세력 도시의 전부 또는 일부 수동 재배치 (추가 기능)
	 4) 황제의 소속 도시를 수동 재배치 (추가 기능)
*/

namespace 세력재배치_수동재배치추가
{

	///	=========================  < 유저 설정 >  =================================

	const bool 시나리오시작시_재배치메뉴_실행 = false;

	///	===========================================================================

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			relocation_menu_add();
			pk::bind(102, pk::trigger102_t(시나리오시작_재배치메뉴_실행));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void relocation_menu_add()
		{
			pk::menu_item item;
			item.menu = 2;
			item.is_enabled = cast<pk::menu_item_is_enabled_t@>(function() { return main.is_enabled_relocation(); });
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("开局部署势力皇帝"); });
			item.get_desc = cast<pk::menu_item_get_desc_t@>(function()
			{
				if (pk::is_campaign()) return pk::encode("教程,决战称霸中不能重新部署势力.");
				if (pk::is_mission_running()) return pk::encode("任务中无法重新部署势力.");
				if (pk::get_scenario().turn_counter > 0) return pk::encode("游戏已进行,不能再重新部署势力.");
				return pk::encode("重新部署城市及皇帝的位置.");
			});
			item.handler = cast<pk::menu_item_handler_t@>(function() { main.relocation_menu(); return true; });
			pk::add_menu_item(item);
		}

		void 시나리오시작_재배치메뉴_실행()
		{
			if (!시나리오시작시_재배치메뉴_실행) return;
			if (pk::get_scenario().loaded) return;
			relocation_menu();
		}

		void relocation_menu()
		{
			if (!is_enabled_relocation()) return;

			string menu_desc = "请选择要重新部署的对象和方式.\n1.部署对象:\x1b[1x城市\x1b[0x,\x1b[1x皇帝\x1b[0x\n2.部署方式:\x1b[1x自动\x1b[0x,\x1b[1x手动\x1b[0x";
			array<string> answer_arr = { pk::encode("\x1b[1x自动部署城市"), pk::encode("\x1b[1x自动部署皇帝"), pk::encode("\x1b[1x手动部署城市"), pk::encode("\x1b[1x手动部署皇帝"), pk::encode("\x1b[1x确认地图"), pk::encode("结束") };
			int answer = pk::choose(pk::encode(menu_desc), answer_arr);
			if (answer == /*도시 자동 재배치*/0) relocate_city_automatically();
			else if (answer == /*황제 자동 재배치*/1) relocate_emperor_automatically();
			else if (answer == /*도시 수동 재배치*/2) relocate_city_manually();
			else if (answer == /*황제 수동 재배치*/3) relocate_emperor_manually();
			else if (answer == /*지도 확인*/4)        show_map();
			else if (answer == /*종료*/5)             return;
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void relocate_city_automatically()
		{
			string desc = "将所有势力的城市随机重新部署.";
			if (pk::choose(pk::encode(desc), { pk::encode("确定"), pk::encode("取消") }) == /*결정*/0)
			{
				pk::list<pk::force@> force_list = pk::get_force_list();
				force_list.shuffle();

				array<int> city_count, index;
				for (int i = 0; i < force_list.count; i++) city_count.insertLast(pk::get_city_list(force_list[i]).count);
				index = Partitioner(pk::get_scenario().seed).partition(city_count);

				for (int i = 0; i < force_list.count; i++)
				{
					pk::list<pk::city@> a = pk::get_city_list(force_list[i]);
					pk::list<pk::city@> b;

					for (int city_id = 0; city_id < 도시_끝; city_id++)
					{
						if (index[city_id] != i) continue;

						pk::city@ city = pk::get_city(city_id);
						int k = a.index_of(city);
						if (k >= 0) a.remove_at(k);
						else		b.add(city);
					}

					a.shuffle();
					b.shuffle();

					for (int j = 0; j < a.count; j++)
					{
						pk::building@ c = pk::city_to_building(a[j]);
						pk::building@ d = pk::city_to_building(b[j]);

						swap(c, d);
					}
					reset_place_facility(300, 0);
					resetting_of_port_and_gate();
				}
			}

			relocation_menu();
		}

		void relocate_emperor_automatically()
		{
			int emperor_id = pk::get_scenario().emperor;
			if (!pk::is_valid_person_id(emperor_id)) pk::message_box(pk::encode("该剧本中没有皇帝."));
			else
			{
				pk::person@ emperor = pk::get_person(emperor_id);
				string emperor_name = "\x1b[1x" + pk::decode(pk::get_name(emperor)) + "\x1b[0x";
				string emperor_city_name = "\x1b[1x" + pk::decode(pk::get_name(pk::get_city(emperor.service))) + "\x1b[0x";
				string desc = "随机重新部署皇帝所属城市.\n(剧本皇帝:" + emperor_name + ")\n(当前所属城市:" + emperor_city_name + ")";
				if (pk::choose(pk::encode(desc), { pk::encode("确定"), pk::encode("取消") }) == /*결정*/0)
				{
					pk::list<pk::city@> selectable_city_list = selectable_city_list_for_emperor(emperor);
					pk::city@ relocated_city = selectable_city_list[pk::rand(int(selectable_city_list.count))];
					int building_id = pk::city_to_building(relocated_city).get_id();
					pk::set_service(emperor, building_id);
					emperor.location = building_id;
					string relocated_city_name = "\x1b[1x" + pk::decode(pk::get_name(relocated_city)) + "\x1b[0x";
					pk::message_box(pk::encode("皇帝(" + emperor_name + ")所属城市由" + emperor_city_name + "变成了" + relocated_city_name + "."));
				}
			}

			relocation_menu();
		}

		void relocate_city_manually()
		{
			string desc_1 = "选择特定势力手动重新部署该势力的全部或部分城市.";
			if (pk::choose(pk::encode(desc_1), { pk::encode("确定"), pk::encode("取消") }) == /*결정*/0)
			{
				pk::message_box(pk::encode("选择重新部署城市的势力."));
				pk::list<pk::force@> selected_force_list = pk::force_selector(pk::encode("选择势力"), "", pk::get_force_list(), 1, 1);
				if (int(selected_force_list.count) > 0)
				{
					pk::force@ selected_force = selected_force_list[0];
					string selected_force_name = "\x1b[1x" + pk::decode(pk::get_name(selected_force)) + "\x1b[0x";
					pk::message_box(pk::encode("请选择要重新部署的势力的城市.\n(选中的势力:" + selected_force_name + ")"));
					pk::list<pk::city@> force_city_list = pk::get_city_list(selected_force);
					int force_city_count = pk::get_city_count(selected_force);
					array<pk::building@> building_from_arr = pk::list_to_array(pk::city_selector(pk::encode("选择城市"), "", force_city_list, 1, force_city_count));
					if (int(building_from_arr.length) > 0)
					{
						string building_from_names = get_building_names_by_building_arr(building_from_arr);
						string building_from_count_info = (int(building_from_arr.length) > 1) ? "喧柁" : "?";
						pk::message_box(pk::encode("请选择与" + building_from_names + "换位城市" + building_from_count_info));
						pk::list<pk::city@> selectable_city_list = selectable_city_list_for_city_relocation(selected_force);
						array<pk::building@> building_to_arr = pk::list_to_array(pk::city_selector(pk::encode("选择城市"), "", selectable_city_list, int(building_from_arr.length), int(building_from_arr.length)));
						if (int(building_to_arr.length) == int(building_from_arr.length))
						{
							string building_to_names = get_building_names_by_building_arr(building_to_arr);
							string desc_2 = building_from_names + "与" + building_to_names + "互换了位置.";
							if (pk::choose(pk::encode(desc_2), { pk::encode("确定"), pk::encode("取消") }) == /*결정*/0)
							{
								for (int i = 0; i < int(building_from_arr.length); i++)
								{
									pk::building@ building_from = pk::city_to_building(pk::get_city(building_from_arr[i].get_id()));
									pk::building@ building_to = pk::city_to_building(pk::get_city(building_to_arr[i].get_id()));
									swap(building_from, building_to);
								}
								reset_place_facility(300, 0);
								resetting_of_port_and_gate();
							}
						}
					}
				}
			}

			relocation_menu();
		}

		void relocate_emperor_manually()
		{
			int emperor_id = pk::get_scenario().emperor;
			if (!pk::is_valid_person_id(emperor_id)) pk::message_box(pk::encode("该剧本中没有皇帝."));
			else
			{
				pk::person@ emperor = pk::get_person(emperor_id);
				string emperor_name = "\x1b[1x" + pk::decode(pk::get_name(emperor)) + "\x1b[0x";
				string emperor_city_name = "\x1b[1x" + pk::decode(pk::get_name(pk::get_city(emperor.service))) + "\x1b[0x";
				string desc_1 = "手动重新部署皇帝所属城市.\n(剧本皇帝:" + emperor_name + ")\n(当前所属城市:" + emperor_city_name + ")";
				if (pk::choose(pk::encode(desc_1), { pk::encode("确定"), pk::encode("取消") }) == /*결정*/0)
				{
					pk::message_box(pk::encode("选择皇帝(" + emperor_name + ")要部署的城市."));
					pk::list<pk::city@> selectable_city_list = selectable_city_list_for_emperor(emperor);
					pk::list<pk::building@> selected_building_list = pk::city_selector(pk::encode("选择城市"), "", selectable_city_list, 1, 1);
					if (int(selected_building_list.count) > 0)
					{
						pk::building@ selected_building = pk::city_to_building(pk::get_city(selected_building_list[0].get_id()));
						string selected_building_name = "\x1b[1x" + pk::decode(pk::get_name(selected_building)) + "\x1b[0x";
						string desc_2 = "皇帝(" + emperor_name + ")所属城市由" + emperor_city_name + "变成了" + selected_building_name + ".";
						if (pk::choose(pk::encode(desc_2), { pk::encode("确定"), pk::encode("取消") }) == /*결정*/0)
						{
							pk::set_service(emperor, selected_building.get_id());
							emperor.location = selected_building.get_id();
						}
					}
				}
			}

			relocation_menu();
		}

		void show_map()
		{
			pk::city_selector(pk::encode("确认地图"), "", pk::get_city_list(), 0, 0);

			relocation_menu();
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		bool is_enabled_relocation()
		{
			return (!pk::is_campaign() and !pk::is_mission_running() and pk::get_scenario().turn_counter == 0);
		}

		pk::list<pk::city@> selectable_city_list_for_city_relocation(pk::force@ selected_force)
		{
			pk::list<pk::city@> selectable_city_list;
			for (int city_id = 0; city_id < 도시_끝; city_id++)
			{
				pk::city@ city = pk::get_city(city_id);
				if (city.get_force_id() != selected_force.get_id()) selectable_city_list.add(city);
			}

			return selectable_city_list;
		}

		pk::list<pk::city@> selectable_city_list_for_emperor(pk::person@ emperor)
		{
			pk::list<pk::city@> selectable_city_list;
			for (int city_id = 0; city_id < 도시_끝; city_id++)
			{
				pk::city@ city = pk::get_city(city_id);
				if (city_id != emperor.service) selectable_city_list.add(city);
			}
			return selectable_city_list;
		}

		string get_building_names_by_building_arr(array<pk::building@> building_arr)
		{
			array<string> building_name_arr(0);
			for (int i = 0; i < int(building_arr.length); i++)
			{
				string building_name = "\x1b[1x" + pk::decode(pk::get_name(building_arr[i])) + "\x1b[0x";
				building_name_arr.insertLast(building_name);
			}

			return join(building_name_arr, "\x1b[0x,");
		}

		void resetting_of_port_and_gate()
		{
			// 항구, 관문 재설정
			for (int city_id = 0; city_id < 도시_끝; city_id++)
			{
				pk::city@ city = pk::get_city(city_id);
				for (int i = 0; i < 5; i++)
				{
					if (city.gate[i] == -1) continue;

					pk::building@ building = pk::get_building(city.gate[i]);
					if (city.get_force_id() == -1) pk::neutralize(building);
					else if (city.gate[i] == 54 or city.gate[i] == 64 or city.gate[i] == 66 or city.gate[i] == 75 or city.gate[i] == 76)
					{
						//山越2，乌恒12，鲜卑14，南蛮23，西羌24
						pk::neutralize(building);
					}
					else
					{
						pk::set_district(building, city.get_district_id());
						pk::set_gold(building, city.gold / 10);
						pk::set_food(building, city.food / 10);
						pk::set_troops(building, city.troops / 10);
						pk::set_energy(building, city.energy);
						pk::reset_weapon_amount(building);
<<<<<<< HEAD
						pk::set_weapon_amount(building, pk::rand(병기_군마) + 1, city.troops / 10);
=======
						pk::set_weapon_amount(building, pk::rand(兵器_战马) + 1, city.troops / 10);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					}
				}
			}
		}

		void reset_place_facility(int per_length, int order)
		{
			int start = per_length * order;
			int end = start + per_length;
			if (start > int(特殊地名设施.length)) return;
			if (end > int(特殊地名设施.length)) end = int(特殊地名设施.length);
			for (int i = start; i < end; i++)
			{
				pk::point pos;
				pos.x = 特殊地名设施[i][1];
				pos.y = 特殊地名设施[i][2];
				pk::hex@ hex0 = pk::get_hex(pos);
				int force_id = -1;
				pk::city@ area_city = pk::get_city(pk::get_city_id(pos));
				if (pk::is_alive(area_city)) force_id = area_city.get_force_id();
				pk::building@ building0 = pk::get_building(pos);
				building0.init_force = force_id;
			}
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	}	// class main





///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	class Partitioner
	{
		array<int> start_, result_(도시_끝);
		pk::random random_;

		Partitioner(int seed) { random_.seed = seed; }

		/**
		@param count 도시 수 배열
		@return      도시 아이디 별로 city_count 키(인덱스) 값이 들어감
		*/
		array<int> partition(const array<int>& in city_count)
		{
			bool retry = true;

			while (retry)
			{
				start_.resize(도시_끝);
				for (int i = 0; i < 도시_끝; i++)
				{
					result_[i] = -1;
					start_[i] = i;
				}
				shuffle(start_);

				retry = false;
				for (int i = 0; i < int(city_count.length); i++)
				{
					if (spread(start_[0], city_count[i], i) == 0) continue;

					retry = true;
					break;
				}
			}

			return result_;
		}

		/**
		city_id 도시를 기준으로 n 개의 도시만큼 선택
		@return 남은 개수
		*/
		int spread(int city_id, int n, int index)
		{
			// 끝났거나 이미 다른 세력의 도시인 경우 리턴.
			if (n <= 0 or result_[city_id] != -1) return n;

			result_[city_id] = index;
			n -= 1;

			int p = start_.find(city_id);
			if (p >= 0) start_.removeAt(p);

			// 이웃도시 섞기.
			pk::city@ city = pk::get_city(city_id);
			array<int> neighbor_city_id(6);
			for (int i = 0; i < 6; i++) neighbor_city_id[i] = city.neighbor[i];
			shuffle(neighbor_city_id);

			for (int i = 0; i < 6; i++)
			{
				if (neighbor_city_id[i] != -1 and result_[neighbor_city_id[i]] == -1) n = spread(neighbor_city_id[i], n, index);
				if (n == 0) break;
			}

			// 남은 도시 수를 리턴
			return n;
		}

		void shuffle(array<int>& arr)
		{
			int n = arr.length();
			for (int i = 0; i < n; i++)
			{
				int j = random_(n - 1);
				int temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	};

	void swap(pk::building@ a, pk::building@ b)
	{
		SwapContext ctx_a, ctx_b;
		ctx_a.load(a);
		ctx_b.load(b);
		ctx_a.save(b);
		ctx_b.save(a);
		if (a.get_force_id() == -1 or a.get_id() == 54 or a.get_id() == 64 or a.get_id() == 66 or a.get_id() == 75)
			pk::neutralize(a);
		//山越2，乌恒12，鲜卑14，南蛮23，西羌24	
		if (b.get_force_id() == -1 or b.get_id() == 54 or b.get_id() == 64 or b.get_id() == 66 or b.get_id() == 75)
			pk::neutralize(b);
	}

	class SwapContext
	{
		int district_id = -1, gold = 0, food = 0, public_order = 0, troops = 0, energy = 0;
		pk::list<pk::person@> person_list;
		array<int> weapon_amount(병기_끝, 0);

		void load(pk::building@ building)
		{
			pk::city@ city = pk::building_to_city(building);
			district_id = building.get_district_id();
			person_list = pk::get_person_list(building, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
			gold = pk::get_gold(building);
			food = pk::get_food(building);
			public_order = city !is null ? city.public_order : 0;
			troops = pk::get_troops(building);
			energy = pk::get_energy(building);
			for (int i = 0; i < 병기_끝; i++) weapon_amount[i] = pk::get_weapon_amount(building, i);
		}

		void save(pk::building@ building)
		{
			pk::city@ city = pk::building_to_city(building);
			pk::set_district(building, district_id);
			for (int i = 0; i < person_list.count; i++) pk::move(person_list[i], building);
			pk::set_gold(building, gold);
			pk::set_food(building, food);
			if (city !is null) pk::set_public_order(city, public_order);
			pk::set_troops(building, troops);
			pk::set_energy(building, energy);
			for (int i = 0; i < 병기_끝; i++) pk::set_weapon_amount(building, i, weapon_amount[i]);
		}
	};

	///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	Main main;
}
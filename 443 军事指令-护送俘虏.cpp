// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/08/13 #messi#修正語句##
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
	제르나	- 공방		| 18.11.27	| 초기생성
	제르나	- 공방		| 18.11.27	| v1.1
									| 행동력 미감소 현상 수정
	기마책사	- 도원결의	| 18.11.27	| v1.2
									| 행동완료 확인 추가, 인접도시로 후송제한
									| 적군 인접여부 추가, 메세시창을 대화로 변경
	제르나	- 공방		| 18.11.28	| v1.3
									| 타 세력 후송제한
	제르나	- 공방		| 18.12.01	| v1.4
									| 소재 확인 추가
	제르나	- 공방		| 18.12.11	| v1.5
									| 관문, 항구 무장 미선별 현상 수정
									| 관문, 항구 거리 미판별 현상 수정
*/

namespace 포로후송
{
	class Main
	{
		// ----- init -----
		pk::list<pk::person@> list_person_all;
		pk::list<pk::person@> list_person_sel;
		pk::list<pk::city@>   list_city_sel;
		array<pk::building@>  arr_city_target;
		array<pk::person@>    arr_person_sel;

		pk::building@ building_base;
		pk::city@	  city_target;
		pk::force@	  force_;

		int cnt_person_all;
		int cnt_person_sel;
		int max_person_sel;

		int ACTION_COST = 20;
		int standard_gold_cost = 100;

		int base_gold = 0;
		string city_target_name;

		Main()
		{
			add_menu();
		}

		void add_menu()
		{
			pk::menu_item item;
			item.menu = 101;
			item.pos = 10;
			item.shortcut = global_menu::shortcut_护送付虏;
			item.init = pk::building_menu_item_init_t(init);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		void init(pk::building@ building)
		{
			// 출발 거점
			@building_base = building;
			int base_id = building_base.get_id();
			if (base_id >= 건물_도시시작 and base_id < 城市_末)
				base_gold = pk::building_to_city(building_base).gold;
<<<<<<< HEAD
			if (base_id >= 건물_관문시작 and base_id < 건물_관문끝)
				base_gold = pk::building_to_gate(building_base).gold;
			if (base_id >= 건물_항구시작 and base_id < 건물_항구끝)
=======
			if (base_id >= 建筑_关卡开始 and base_id < 건물_관문끝)
				base_gold = pk::building_to_gate(building_base).gold;
			if (base_id >= 건물_항구시작 and base_id < 建筑_港口末)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				base_gold = pk::building_to_port(building_base).gold;

			// 세력
			@force_ = pk::get_force(building_base.get_force_id());

			// 도시 검색
			check_city();

			// 도시 포로
			list_person_all = pk::get_person_list(pk::mibun_flags(신분_포로));
			check_person();

			cnt_person_all = pk::list_to_array(list_person_all).length;
			max_person_sel = pk::min(cnt_person_all, int(base_gold / standard_gold_cost));
		}

		bool isEnabled()
		{
			// 포로
			if (!check_condition(0))	return false;

			// 도시
			if (!check_condition(1))	return false;

			// 행동력
			if (!check_condition(2))	return false;

			// 금
			if (!check_condition(3))	return false;

			// 적군 주위에 있음
			if (!check_condition(4))	return false;

			return true;
		}

		string getText()
		{
			return pk::encode("护送俘虏");
		}

		string getDesc()
		{
			/// 포로
			if (!check_condition(0))
				return pk::encode("没有可押解的俘虏.");

			// 도시
			if (!check_condition(1))
				return pk::encode("没有可以送往的城市.");

			// 행동력
			if (!check_condition(2))
				return pk::encode(pk::format("行动力不足.(行动力 {})", ACTION_COST));

			// 금
			if (!check_condition(3))
				return pk::encode(pk::format("资金不足.(资金{}/{})", base_gold, standard_gold_cost));

			// 적군 주위에 있음
			if (!check_condition(4))
				return pk::encode("附近有敌军,无法执行");

			return pk::encode("将俘虏转移到其他据点.");
		}

		bool check_condition(int mode)
		{
			// 0 : 포로
			if (mode == 0)
			{
				if (cnt_person_all < 1)
					return false;
			}
			// 1 : 도시
			if (mode == 1)
			{
				if (pk::list_to_array(list_city_sel).length <= 0)
					return false;
			}
			// 2 : 행동력
			if (mode == 2)
			{
				if (int(pk::get_district(pk::get_district_id(force_, 1)).ap) < ACTION_COST)
					return false;
			}
			// 3 : 금
			if (mode == 3)
			{
				if (base_gold < standard_gold_cost)
					return false;
			}
			// 4: 적군 주위에 있음
			if (mode == 4)
			{
				if (pk::enemies_around(building_base))
					return false;
			}

			return true;
		}

		bool handler()
		{
			// ----- 대상 선택 -----
			if (!set_move())
				return false;

			// ----- 동작 확인 -----
			if (!check_confirm())
				return false;

			// ----- 메세지 -----
			message();

			// ----- Data 처리 -----
			move();

			return true;
		}

		bool set_move()
		{
			// 무장 선택
			pk::message_box(pk::encode("请选择要押送的武将."));
			list_person_sel = pk::person_selector(pk::encode("选择俘虏"), pk::encode("选择要押送的俘虏"), list_person_all, 1, max_person_sel);
			arr_person_sel = pk::list_to_array(list_person_sel);
			if (arr_person_sel.length < 1) return false;
			cnt_person_sel = arr_person_sel.length;

			// 도시 선택
			pk::message_box(pk::encode("请选择要送往的城市."));

			// 도시거리가 1인 도시 중 선택
			arr_city_target = pk::list_to_array(pk::city_selector(pk::encode("选择目的地"), pk::encode("选择要送往的城市."), list_city_sel, 1, 1));

			if (arr_city_target.length < 1) return false;
			@city_target = pk::get_city(arr_city_target[0].get_id()); // building_to_city() 정상동작 안함
			if (city_target.get_id() == building_base.get_id()) return false;
			city_target_name = city_target.name;

			return true;
		}

		bool check_confirm()
		{
			return pk::yes_no(pk::encode(pk::format("将\x1b[1x{}\x1b[0x名俘虏被送交到\x1b[1x{}\x1b[0x.\n(行动力{}, 资金{})", cnt_person_sel, pk::decode(city_target_name), ACTION_COST, int(standard_gold_cost * cnt_person_sel))));
		}

		void message()
		{
			array<string> message_person =
			{
				pk::encode("嗯... 把他拖到那里去"),
				pk::encode("该死... 随你便吧!"),
				pk::encode("无论关在哪里 \n我的忠心都不容质疑!")
			};
			int i = pk::rand(3);
			pk::say(pk::encode(pk::decode(message_person[i])), arr_person_sel[pk::rand(cnt_person_sel)], building_base);
		}

		void move()
		{
			for (int i = 0; i < cnt_person_sel; i++)
			{
				pk::move(arr_person_sel[i], pk::city_to_building(city_target), false);

				// 이동한 포로무장 행동완료 (기마책사, '18.11.27)
				arr_person_sel[i].action_done = true;
			}

			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);
			pk::add_gold(building_base, -int(standard_gold_cost * cnt_person_sel), true);
		}


		// 무장 필터 추가 (기마책사: '18.11.27)
		// 행동완료, 소재
		void check_person()
		{
			array<pk::person@> arr_person_all = pk::list_to_array(list_person_all);

			list_person_all.clear();   // 무장 검색 리스트 초기화
			for (int i = 0; i < int(arr_person_all.length); i++)
			{
				if (!arr_person_all[i].action_done
					and arr_person_all[i].location == building_base.get_id())
					list_person_all.add(arr_person_all[i]);    // 리스트에 무장 추가
			}
		}

		// 인접도시 검색 추가 (기마책사: '18.11.27)
		void check_city()
		{
			int base_id = building_base.get_id();

			list_city_sel.clear();  // 도시 검색 리스트 초기화
			for (int city_num = 0; city_num < 城市_末; city_num++)
			{
				if (pk::get_building_distance(building_base.get_id(), city_num, building_base.get_force_id()) == 1)
				{
					pk::city@ city = pk::get_city(city_num);
					if (building_base.get_force_id() == city.get_force_id())
						list_city_sel.add(city);  // 리스트에 도시 추가
				}
			}
		}

	}

	Main main;
}
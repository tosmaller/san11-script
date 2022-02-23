// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
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
*/

namespace 재야배치
{
	const int KEY = pk::hash("布置在野武将");

	class Main
	{
		Main()
		{
			add_menu();
		}

		void add_menu()
		{
			pk::menu_item item;
			item.menu = 2;
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		string getText()
		{
			return pk::encode("配置在野武将");
		}

		string getDesc()
		{
			return pk::encode("重新部署在野武将.");
		}

		bool handler()
		{
			//모드 선택
			array<string> btn_mode =
			{
				pk::encode(" 随机部署 "),
				pk::encode(" 手动部署 "),
				pk::encode(" 取消 ")
			};

			int mode = pk::choose(pk::encode("请选择部署方式."), btn_mode);
			if (mode == 0)
			{
				return update_person_auto();
			}
			else if (mode == 1)
			{
				return update_person_manual();
			}
			else
			{
				return false;
			}
		}

		bool update_person_auto()
		{
			if (!pk::yes_no(pk::encode("在野武将会被安排在任意势力中.")))
				return false;

			array<pk::person@> arr_person = pk::list_to_array(pk::get_person_list(pk::mibun_flags(신분_재야)));
			if (arr_person.length < 1) return false;

			int count = 0;
			for (int i = 0; i < int(arr_person.length); i++)
			{
				if (check_num(arr_person[i].get_id()) and check_unemployed(arr_person[i]))
				{
					pk::city@ target_city;
					@target_city = assign_site();
					arr_person[i].service = target_city.get_id();
					arr_person[i].location = target_city.get_id();
					arr_person[i].mibun = 身份_一般;
					arr_person[i].loyalty = 100;
					arr_person[i].district = target_city.district;
					arr_person[i].update();

					count++;
				}
			}

			pk::message_box(pk::encode(pk::format("{}名武将被重新部署了.", count)));

			return true;
		}

		bool update_person_manual()
		{
			// 무장 선택
			pk::message_box(pk::encode("请选择目标武将."));
			pk::list<pk::person@> temp_person_list = pk::get_person_list(pk::mibun_flags(신분_재야));
			array<pk::person@> arr_person = pk::list_to_array(pk::person_selector(pk::encode("选择武将"), pk::encode("选择目标武将"), temp_person_list, 1, pk::list_to_array(temp_person_list).length));
			if (arr_person.length < 1) return false;

			// 도시 선택
			pk::message_box(pk::encode("请选择目标城市."));
			pk::city@ city_target;
			array<pk::building@> arr_building = pk::list_to_array(pk::city_selector(pk::encode("选择武将"), pk::encode("选择目标城市"), pk::get_city_list(), 1, 1));
			if (arr_building.length < 1) return false;
			@city_target = pk::get_city(arr_building[0].get_id());

			// 세력 포함여부
			bool force_check = false;
			if (city_target.district >= 0)
				force_check = pk::yes_no(pk::encode("这个城市已有势力存在.\n是否归属于该势力下?"));

			// 배치
			int count = 0;
			for (int i = 0; i < int(arr_person.length); i++)
			{
				if (check_num(arr_person[i].get_id()) and check_unemployed(arr_person[i]))
				{
					arr_person[i].service = city_target.get_id();
					arr_person[i].location = city_target.get_id();
					if (force_check)
					{
						arr_person[i].mibun = 身份_一般;
						arr_person[i].loyalty = 100;
						arr_person[i].district = city_target.district;
					}
					arr_person[i].update();
					count++;
				}
			}

			pk::message_box(pk::encode(pk::format("{}名武将被重新部署了.", count)));

			return true;
		}

		bool check_num(int number)
		{
			/*
				입력범위
				1.    0 ~  669(역사 무장)
				2.  800 ~  831(고대 무장)
				3.  850 ~  999(신무장)
			*/
			if ((number >= 0 and number <= 669)
				or (number >= 800 and number <= 831)
				or (number >= 850 and number <= 999))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool check_unemployed(pk::person@ target)
		{
			return target.mibun == 신분_재야;
		}

		pk::city@ assign_site()
		{
			pk::force@ force_;
			int city_id;

			array<pk::force@> arr_force = pk::list_to_array(pk::get_force_list(false));
			@force_ = arr_force[pk::rand(arr_force.length)];

			array<pk::city@> arr_city = pk::list_to_array(pk::get_city_list(force_));
			return arr_city[pk::rand(arr_city.length)];
		}
	}

	Main main;
}
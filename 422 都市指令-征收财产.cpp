// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2020/09/21 # 江东新风 # 添加法治效果 ##
// ##2020/08/03 # messi # 語句修正 ##
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
	제르나	- 공방		| 18.11.25	| 초기생성
*/

namespace 징발
{
	const int KEY = pk::hash("町词");

	class Main
	{
		// ----- Setting -----

		// 약소세력 적용 여부
		bool weak_check = true;

		// 약소세력 기준
		int standard_city = 1;			// 최대 도시 수
		int standard_person = 8;		// 최대 장수 수

		// 발동 조건
		int ACTION_COST = 50;	// 행동력
		int standard_public_order = 60;	// 최소 치안 ( 1 - 100 )

		// 이벤트
		bool event_check = true;		// 이벤트 발생 여부
		int event_chance = 10;			// 이벤트 발생 확률 ( 1 - 100 )
		int event_result = -1;			// 이벤트 결과
		float event_rate = 1.5;			// 이벤트 보상 배율

		// 변동 기준치 - 실제 변동량과 다름
		int get_gold_min = 750;			// 최소 금 획득량 기준치
		int get_gold_max = 1000;		// 최대 금 획득량 기준치
		int lose_public_order_min = 15; // 최소 치안 감소량 기준치 ( 1 - 100 )
		int lose_public_order_max = 20; // 최대 치안 감소량 기준치 ( 1 - 100 )

		// ----- init -----
		pk::building@ building_;
		pk::person@ taishu_;
		pk::city@ city_;
		pk::force@ force_;
		int cnt_person;
		int taishu_character;

		Main()
		{
			add_menu();
		}

		void add_menu()
		{
			pk::menu_item item;
			item.menu = 100;
			item.pos = 10;
			item.shortcut = "r";
			item.init = pk::building_menu_item_init_t(init);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.is_visible = pk::menu_item_is_visible_t(isVisible);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		void init(pk::building@ building)
		{
			@building_ = building;
			@city_ = pk::building_to_city(building_);
			@taishu_ = pk::get_person(city_.taishu);
			@force_ = pk::get_force(building_.get_force_id());
			cnt_person = pk::list_to_array(pk::get_person_list(force_, pk::mibun_flags(0, 1, 2, 3))).length;
			taishu_character = taishu_.character;
		}

		bool isEnabled()
		{
			// 약소세력
			if (!check_condition(0) and weak_check)	return false;

			// 행동력
			if (!check_condition(1))	return false;

			// 치안
			if (!check_condition(2))	return false;

			// 태수 행동
			if (!check_condition(3))	return false;

			// 태수 상병
			if (!check_condition(4))	return false;

			return true;
		}

		bool isVisible()
		{
			// 약소세력
			if (!check_condition(0) and weak_check)
				return false;
			else
				return true;
		}

		string getText()
		{
			return pk::encode("征收财产");
		}

		string getDesc()
		{
			// 약소세력
			if (!check_condition(0) and weak_check)
				return pk::encode("只有弱小势力才能执行.");

			// 행동력
			if (!check_condition(1))
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力))", ACTION_COST));

			// 치안
			if (!check_condition(2))
				return pk::encode(pk::format("治安过低. (治安 {}/{})", city_.public_order, standard_public_order));

			// 태수 행동
			if (!check_condition(3))
				return pk::encode("太守已经完成了行动.");

			// 태수 상병
			if (!check_condition(4))
				return pk::encode("太守处于受伤状态.");

			return pk::encode("没收城中百姓的财产.");
		}

		bool check_condition(int mode)
		{
			// 0 : 약소세력
			if (mode == 0)
			{
				if (pk::get_city_count(force_) > standard_city)
					return false;
				if (cnt_person > standard_person)
					return false;
			}
			// 1 : 행동력
			if (mode == 1)
			{
				if (int(pk::get_district(pk::get_district_id(force_, 1)).ap) < ACTION_COST)
					return false;
			}
			// 2 : 치안
			if (mode == 2)
			{
				if (int(city_.public_order) < standard_public_order)
					return false;
			}
			// 3 : 태수 행동 완료
			if (mode == 3)
			{
				if (taishu_.action_done)
					return false;
			}
			// 4 : 태수 상병
			if (mode == 4)
			{
				if (taishu_.shoubyou > 상병_건강)
					return false;
			}

			return true;
		}

		bool handler()
		{
			// ----- 동작 확인 -----
			if (!check_confirm())
				return false;

			// ----- 변수 확인 -----
			if (!check_var())
			{
				pk::message_box(pk::encode("错误:配置发生错误."));
				return false;
			}

			// ----- 메세지, 이벤트 -----
			event_result = -1;

			if (event_check and (int(pk::rand(100)) < event_chance))
				pk::scene(pk::scene_t(scene));
			else
				scene_basic();

			// ----- Data 처리 -----
			data_update();

			return true;
		}

		bool check_confirm()
		{
			return pk::yes_no(pk::encode(pk::format("是否要没收\x1b[1x{}\x1b[0x城中百姓的财产?\n(行动力{})", pk::decode(city_.name), ACTION_COST)));
		}

		bool check_var()
		{
			if (standard_public_order < 1 or standard_public_order > 100)
				return false;
			if (lose_public_order_min < 1 or lose_public_order_min > 100)
				return false;
			if (lose_public_order_max < 1 or lose_public_order_max > 100)
				return false;
			if (event_chance < 1 or event_chance > 100)
				return false;
			return true;
		}

		void scene()
		{
			pk::unit@ dummy_unit;
			pk::person@ dummy_person;

			pk::move_screen(city_.get_pos());
			pk::fade(0);
			pk::sleep();
			pk::background(4);
			pk::fade(255);

			//기본 대사
			scene_basic();

			// 컷인 - 발걸음
			pk::cutin(18);

			// 추가 대사 1
			scene_add_1();

			// 일기토
			event_result = pk::duel(dummy_unit, dummy_unit, taishu_, dummy_person, dummy_person, pk::get_person(무장_무술가), dummy_person, dummy_person, true, false, 0, true).first;

			// 추가 대사 2, 3
			if (event_result == 0)
			{
				scene_add_2();
			}
			else
			{
				scene_add_3();
			}

			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		}

		void scene_basic()
		{
			int num = 0;

			array<string> taishu_lines_arr =
			{
				pk::encode("这是为了国家...\n没有办法."),
				pk::encode("虽然很痛心...\n为了大义要有牺牲的准备."),
				pk::encode("闭嘴,快交上来!\n拒者当斩!"),
				pk::encode("为国之事!民亦乐也!\n快搬走吧!")
			};

			num = pk::rand(2);
			if (taishu_character >= 2) num = num + 2;
			pk::message_box(pk::encode(pk::decode(taishu_lines_arr[num])), taishu_);

			array<string> farmer_lines_arr =
			{
				pk::encode("...真不是人啊..."),
				pk::encode("该死!简直就是地狱...")
			};

			num = pk::rand(2);
			pk::message_box(pk::encode(pk::decode(farmer_lines_arr[num])), pk::get_person(무장_농민));
		}

		void scene_add_1()
		{
			int num = 0;

			array<string> fighter_lines_arr =
			{
				pk::encode("到底要欺压百姓到什么时侯?"),
				pk::encode("无德之君!")
			};

			num = pk::rand(2);
			pk::message_box(pk::encode(pk::decode(fighter_lines_arr[num])), pk::get_person(무장_무술가));

			array<string> taishu_lines_arr =
			{
				pk::encode("你... 是谁?"),
				pk::encode("是谁这么大胆!")
			};

			num = pk::rand(2);
			pk::message_box(pk::encode(pk::decode(taishu_lines_arr[num])), taishu_);
		}

		void scene_add_2()
		{
			int num = 0;

			array<string> taishu_lines_arr =
			{
				pk::encode("胆敢对我动手?"),
				pk::encode("反对国事者则如此."),
				pk::encode("干什么呢!多拿点!"),
				pk::encode("现在不求饶恕.都装上!")
			};

			num = pk::rand(2);
			pk::message_box(pk::encode(pk::decode(taishu_lines_arr[num])), taishu_);

			num = pk::rand(2) + 2;
			pk::message_box(pk::encode(pk::decode(taishu_lines_arr[num])), taishu_);
		}

		void scene_add_3()
		{
			int num = 0;

			array<string> fighter_lines_arr =
			{
				pk::encode("下次就不会再原谅你了."),
				pk::encode("请记住这就是百姓的愤怒.")
			};

			num = pk::rand(2);
			pk::message_box(pk::encode(pk::decode(fighter_lines_arr[num])), pk::get_person(무장_무술가));

			array<string> taishu_lines_arr =
			{
				pk::encode("该死...走着瞧!"),
				pk::encode("切!今天先撤了...")
			};

			num = pk::rand(2);
			pk::message_box(pk::encode(pk::decode(taishu_lines_arr[num])), taishu_);

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x受伤了.", pk::decode(pk::get_name(taishu_)))));
		}

		void data_update()
		{
			if (event_result < 1)
			{
				// 난수 생성
				int gold = get_gold_min + pk::rand(get_gold_max - get_gold_min) + 1;
				int public_order = lose_public_order_min + pk::rand(lose_public_order_max - lose_public_order_min) + 1;

				// 수치 보정 - 치안
				gold = gold * (100 + ((city_.public_order - standard_public_order) / (100 - standard_public_order) * 20)) / 100;

				// 수치 보정 - 태수
				gold = int(gold * ((float(taishu_character) + 9) / 10));
				public_order = pk::min(int(public_order * ((float(taishu_character) + 9) / 10)), 100);

				// 수치 보정 - 이벤트
				if (event_result == 0)
				{
					gold = int(gold * event_rate);
					public_order = pk::min(int(public_order * event_rate), 100);
				}

				// 금, 치안 수정
				pk::add_gold(city_, gold, true);
				ch::add_public_order(city_, -public_order, true);

				pk::message_box(pk::encode(pk::format("获得了\x1b[1x{}\x1b[0x资金.\n治安降低了\x1b[1x{}\x1b[0x.", gold, public_order)));
			}

			// 행동력 감소
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			// 태수 수정
			if (event_result == 1)
				taishu_.shoubyou = pk::max(taishu_.shoubyou, 1 + pk::rand(상병_빈사));
			taishu_.action_done = true;
			taishu_.update();
		}

		void data_update_event()
		{
			// 행동력 감소
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			// 태수 수정
			taishu_.action_done = true;
			taishu_.shoubyou = pk::max(taishu_.shoubyou, 1 + pk::rand(상병_빈사));
			taishu_.update();
		}
	}

	Main main;
}
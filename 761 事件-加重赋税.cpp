// ## 2021/09/01 # 白马叔叔 # 修正语句小失误 ##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/09/21 # 江东新风 # 添加法治效果 ##
// ## 2020/08/01 ##
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
	제르나	- 공방	 	| 18.10.27	| 초기생성
	제르나	- 공방		| 18.10.31	| V 1.1
									| 금, 군량, 치안 수치 조정
									| 발동조건 추가 - 무장수 제한
*/

namespace 추가징수
{
	class Main
	{
		Main()
		{
			pk::bind(112, pk::trigger112_t(callback));
		}

		//Data
		pk::force@ force_;
		pk::person@ kunshu_;
		pk::person@ gunshi_;
		pk::city@ city_;

		//Setting
		int set_standard_city = 1;
		int set_standard_public_order = 80; // 최소값 : 1
		int set_standard_week = 6; // 최소값 : 1
		int set_standard_person = 8; // 최소값 : 1

		int set_get_gold_min = 500;
		int set_get_gold_max = 700;
		int set_get_food_min = 2500;
		int set_get_food_max = 3500;
		int set_lose_public_order_min = 10;
		int set_lose_public_order_max = 15; // 최대값 : set_standard_public_order

		void callback(pk::force@ force)
		{
			if (force.is_player() and force.gunshi >= 0)
			{
				@force_ = force;
				@kunshu_ = pk::get_person(force.kunshu);
				@gunshi_ = pk::get_person(force.gunshi);
				@city_ = pk::building_to_city(pk::get_building(kunshu_.service));
				array<pk::person@> arr_person = pk::list_to_array(pk::get_person_list(force_, pk::mibun_flags(0, 1, 2, 3)));


				if (city_ is null) return;
				if (int(city_.public_order) < set_standard_public_order) return;
				if (pk::get_city_count(force) > set_standard_city) return;
				if (int(arr_person.length) > set_standard_person) return;

				if (pk::get_scenario().turn_counter == 0) return;
				if (pk::get_scenario().turn_counter % set_standard_week != 0) return;

				pk::scene(pk::scene_t(scene));
			}
		}

		void scene()
		{
			pk::move_screen(city_.get_pos());
			pk::fade(0);
			pk::sleep();
			pk::background(2);
			pk::fade(255);

			string kunshu_name = pk::decode(pk::get_name(kunshu_));
			string gunshi_name = pk::decode(pk::get_name(gunshi_));
			pk::person@ farmer = pk::get_person(무장_농민);

			switch (pk::rand(3))
			{
			case 0: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,我们需要更多的税金.", kunshu_name)), gunshi_); break;
			case 1: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,城中的仓库将要见底了.", kunshu_name)), gunshi_); break;
			case 2: pk::message_box(pk::encode("最近的宴会挥霍了很多钱.再征收更多的税金如何?"), gunshi_); break;
			}

			array<string> answers =
			{
				pk::encode("体谅百姓疾苦"),
				pk::encode("剥削百姓")
			};

			int n = pk::choose(pk::encode("是否要多征税收?"), answers);
			int gold;
			int food;
			int public_order;

			switch (n)
			{
				//미징수
			case 0:
				switch (pk::rand(3))
				{
				case 0: pk::message_box(pk::encode("不能再继续压榨百姓了"), kunshu_); break;
				case 1: pk::message_box(pk::encode("全都让百姓们承担确实太过沉重了."), kunshu_); break;
				case 2: pk::message_box(pk::encode("我就当没听过这件事吧."), kunshu_); break;
				}
				switch (pk::rand(3))
				{
				case 0: pk::message_box(pk::encode("不愧是主公啊."), gunshi_); break;
				case 1: pk::message_box(pk::encode("在下知道主公的想法了."), gunshi_); break;
				case 2: pk::message_box(pk::encode("那就在考虑一些别的方案吧."), gunshi_); break;
				}
				break;

				//징수
			case 1:
				switch (pk::rand(4))
				{
				case 0: pk::message_box(pk::encode("我也有相同的想法,那就多征上来些税金吧"), kunshu_); break;
				case 1: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,你果然深知我心.", gunshi_name)), kunshu_); break;
				case 2: pk::message_box(pk::encode("下次可以享受更盛大的宴会了?"), kunshu_); break;
				case 3: pk::message_box(pk::encode("哈哈,百姓也会愿意助我一臂之力的."), kunshu_); break;
				}

				gold = set_get_gold_min + pk::rand(set_get_gold_max - set_get_gold_min) + 1;
				food = set_get_food_min + pk::rand(set_get_food_max - set_get_food_min) + 1;
				public_order = set_lose_public_order_min + pk::rand(set_lose_public_order_max - set_lose_public_order_min) + 1;

				pk::add_gold(city_, gold, true);
				pk::add_food(city_, food, true);
				ch::add_public_order(city_, -public_order, true);

				pk::message_box(pk::encode(pk::format("征收上来了资金\x1b[1x{}\x1b[0x,军粮\x1b[1x{}\x1b[0x.", gold, food)));

				switch (pk::rand(3))
				{
				case 0: pk::message_box(pk::encode("...真不是人啊..."), farmer); break;
				case 1: pk::message_box(pk::encode("等等!这让我怎么活啊..."), farmer); break;
				case 2: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x这种昏庸的人值得我们追随吗...", kunshu_name)), farmer); break;
				}

				pk::message_box(pk::encode(pk::format("治安下降了\x1b[1x{}\x1b[0x.", public_order)));
				pk::history_log(city_.get_pos(), force_.color, pk::encode(pk::format("压榨引起了民众动乱，\x1b[1x{}\x1b[0x的治安下降了", pk::decode(pk::get_name(city_)))));

				break;
			}

			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		}
	}

	Main main;
}
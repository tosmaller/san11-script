// ## 2021/11/09 # 江东新风 # 改进写法，调整强度 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/02/23 #messi# 修正文本 ##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/13 #messi#修正語句##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: 도시 内 无특기 무장들에게 특기를 부여 (도시에 태수가 없으면 실행 불가)

	 => 신분이 군주,태수,일반인 无특기 무장들에게 특기를 부여 (선인_탐색 커맨드를 실행하는 도시에서만 无특기 무장에게 특기가 부여됨)

*/

namespace 선인_탐색
{


	// ================ CUSTOMIZE ================


	const int TP_COST = 1500; 	   // (기교P 필요량: 기본 500 설정)
	const int ACTION_COST = 80;    // (행동력 필요량: 기본 80 설정)

	const int GOLD_ = 3000;    	   // (금 획득량: 기본 3000 설정)
	const int FOOD_ = 15000;       // (병량 획득량: 기본 15000 설정)
	const int TROOPS_ = 10000;     // (병력 획득량: 기본 10000 설정)

	const int select_person = 30;  // 도시에 있는 무특기 무장을 일정확률로 선별하여 당첨된 무장에게만 특기를 부여 (기본 확률: 30%)

	const array<int> skill_arr = {特技_妖术, 特技_明镜, 特技_风水, 特技_沉着, 特技_威压, 特技_规律, 特技_强行, 特技_精妙, 特技_医者, 特技_祈愿, 特技_鬼门, 特技_解毒, 特技_安民 };
	const int KEY = pk::hash("松胎琶哨");


	/*

		[금 획득] 선인을 찾지 못한 대신에 금을 획득
		[병량 획득] 선인을 찾지 못한 대신에 병량을 획득
		[병력 획득] 선인을 찾지 못한 대신에 병력을 획득

		[우길] 도시에 있는 무특기 무장에게 <요술> 특기를 전수
		[좌자] 도시에 있는 무특기 무장에게 <귀문> 특기를 전수
		[북두] 도시에 있는 무특기 무장에게 <백출> 특기를 전수
		[남두] 도시에 있는 무특기 무장에게 <명경> 특기를 전수

	*/


	// ===========================================


	class Main
	{

		pk::building@ building_;
		pk::force@ force_;
		pk::person@ taishu_;
		pk::city@ city_;
<<<<<<< HEAD
		uint16 building_gold;
=======
		int building_gold;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

		Main()
		{
			pk::menu_item item;
			item.menu = 102;
			item.pos = 8;
			item.shortcut = global_menu::shortcut_仙人探索;
			item.init = pk::building_menu_item_init_t(init);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@taishu_ = pk::get_person(pk::get_taishu_id(building));
			@city_ = pk::building_to_city(building);
			building_gold = pk::get_gold(building);
		}


		bool isEnabled()
		{
			if (!pk::is_alive(taishu_)) return false;
			else if (force_.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		string getText()
		{
			return pk::encode("仙人探索");  //선인탐색xianrentansuo
		}

		string getDesc()
		{
			if (!pk::is_alive(taishu_))
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("探索仙人.(技巧P {}, 行动力 {})", TP_COST, ACTION_COST));
		}

		bool handler()
		{

			if (pk::choose(pk::encode("是否要探索仙人?\n(城中如不存在无特技武将,找到仙人时将无法习得特技.)"), { pk::encode("  是  "), pk::encode("  否  ") }) == 1)
				return false;

			pk::person@ sinsen_1 = pk::get_person(무장_우길);
			pk::person@ sinsen_2 = pk::get_person(무장_좌자);
			pk::person@ sinsen_3 = pk::get_person(무장_북두);
			pk::person@ sinsen_4 = pk::get_person(무장_남두);

			if (pk::is_alive(taishu_))
			{

				int n = pk::rand(7);//不包括7
				//n = 6;
				auto person_list = pk::get_person_list(building_, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));


				// 금 획득
				if (n <= 1)
				{

					pk::message_box(pk::encode("可惜没找到仙人,但却筹到了一些金钱."), taishu_);
					pk::message_box(pk::encode(pk::format("城市的资金增加了\x1b[1x{}\x1b[0x.", GOLD_)));
					pk::add_gold(city_, GOLD_, true);

				} // if

				// 병량 획득
				if (n <= 3)
				{

					pk::message_box(pk::encode("可惜没找到仙人,但筹到了一些兵粮."), taishu_);
					pk::message_box(pk::encode(pk::format("城市的兵粮增加了\x1b[1x{}\x1b[0x.", FOOD_)));
					pk::add_food(city_, FOOD_, true);

				} // if

				// 병력 획득
				if (n <= 5)
				{

					pk::message_box(pk::encode("可惜没找到仙人,但却意外遇见一群士兵来投."), taishu_);
					pk::message_box(pk::encode(pk::format("城市的士兵增加了\x1b[1x{}\x1b[0x.", TROOPS_)));
					ch::add_troops(city_, TROOPS_, true);

				} // if

				// 우길
				if (n == 6)
				{

					pk::message_box(pk::encode("神仙, 原来你在这里呀."), taishu_);
					pk::message_box(pk::encode("呵呵, 终于找到这来了啊."), sinsen_1);
					pk::play_se(6);
					pk::message_box(pk::encode("看在你的一片心意, 我送你一份特别的礼物."), sinsen_1);   //그대의 정성을 봐서 내 특별한 선물을 주도록 하겠네
					pk::message_box(pk::encode("谢谢上仙!"), taishu_);

					if (0 < person_list.count)
					{
						for (int k = 0; k < person_list.count; k++)
						{
							pk::person@ person0 = person_list[k];
							// 무특기 무장에게 요술 특기를 전수
							if (person0.skill == -1)
							{								
								if (pk::rand_bool(select_person))
								{
									int skill_id = skill_arr[pk::rand(int(skill_arr.length))];
									if (skill_id == 特技_鬼门 and person0.stat[武将能力_智力] < 88) skill_id = 特技_妖术;
									pk::set_skill(person0, skill_id);//用此新函数可以触发特技变动设定
									string msg = pk::encode("\x1b[1x") + pk::get_name(person0) + pk::encode("\x1b[0x通过仙人探索学会了特技") + pk::get_skill(skill_id).name;
									pk::history_log(person0.pos, pk::get_force(person0.get_force_id()).color, msg);
								}

							} // if

						} // for

					} // if 

				} // if


			} // if	

			// 기교 P 감소
			pk::message_box(pk::encode(pk::format("城市的技巧P减少了\x1b[1x{}\x1b[0x.", TP_COST)));
			ch::add_tp(force_, -TP_COST, force_.get_pos());

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			return true;

		}

	}

	Main main;
}
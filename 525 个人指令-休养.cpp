// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/19 # 江东新风 # 加入行动力消耗防止反复刷休养 ##
// ## 2020/08/08 # 氕氘氚 # 加入【个人】菜单 ##
/*
@ 제작자: HoneyBee
@ 설명: 군주의 행동상태를 미행동으로 변경할 수 있는 SCRIPT

※ 기교P와 금을 소비하여 군주의 행동상태를 미행동으로 회복할 수 있습니다.
- 시장 이벤트 (평범)
- 낚시 이벤트 (평범)
- 농민 이벤트 (BONUS로 병량 획득)


*/

namespace 휴양
{

	// ================ CUSTOMIZE ================
	const int ACTION_COST = 20;
	const int TP_COST = 50; 	    // 기교P 필요량
	const int GOLD_COST = 500;      // 금 필요량

	const int FOOD_사례 = 3000;       // 농민들에게 사례로 받는 병량

	const int KEY = pk::hash("暮泉");

	// ===========================================

	class Main
	{

		pk::building@ building;
		pk::building@ kunshu_building;
		pk::city@ kunshu_city;
		pk::city@ emperor_city;
		pk::force@ force;
		pk::person@ kunshu;
		pk::person@ merchant;
		pk::person@ boy;
		pk::person@ farmer;
		int building_gold;
		pk::random random(pk::rand());

		Main()
		{

			pk::menu_item item;
			item.menu = global_menu::菜单_个人;
			//ch::u8debug(pk::format("menu: xiu yang {}", global_menu::菜单_个人));  
			item.shortcut = "6";
			item.init = pk::building_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(isVisible);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);

		}


		void init(pk::building@ building)
		{

			@building = @building;
			@force = pk::get_force(building.get_force_id());
			@kunshu = pk::get_person(pk::get_kunshu_id(building));
			@merchant = pk::get_person(무장_상인);
			@boy = pk::get_person(무장_소년);
			@farmer = pk::get_person(무장_농민);
			@kunshu_building = pk::get_building(kunshu.service);
			@kunshu_city = pk::building_to_city(kunshu_building);
			building_gold = pk::get_gold(building);
		}

		string getText()
		{
			return pk::encode("休养");
		}

		string getDesc()
		{

			if (kunshu_building.get_id() >= 城市_末)
				return pk::encode("君主不在城市中.");
			else if (kunshu_city.get_id() != kunshu.service)
				return pk::encode("只能在君主所在城市实行.");
			else if (pk::is_absent(kunshu) or pk::is_unitize(kunshu))
				return pk::encode("君主不在城市中.");
			else if (kunshu.action_done == false)
				return pk::encode("君主未行动.");
			else if (force.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("进行休养. (技巧P {} 行动力{})", TP_COST, ACTION_COST));
		}

		bool isVisible()
		{
			if (kunshu_building.get_id() >= 城市_末) return false;
			else if (kunshu_city.get_id() != kunshu.service) return false;
			return true;
		}

		bool isEnabled()
		{
			if (kunshu_building.get_id() >= 城市_末) return false;
			else if (kunshu_city.get_id() != kunshu.service) return false;
			else if (pk::is_absent(kunshu) or pk::is_unitize(kunshu)) return false;
			else if (kunshu.action_done == false) return false;
			else if (force.tp < TP_COST) return false;
			else if (building_gold < GOLD_COST) return false;
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST) return false;
			return true;
		}

		bool handler()
		{

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("要进行休养吗?\n(技巧P {} 行动力{})", TP_COST, ACTION_COST)), kunshu) == 1)
				return false;

			pk::scene(pk::scene_t(scene_휴양));

			// 기교 감소
			ch::add_tp(force, -TP_COST, force.get_pos());

			// 실행 도시 금 소비
			pk::add_gold(kunshu_city, -GOLD_COST, true);

			pk::message_box(pk::encode("君主变成未行动状态."));  //군주가 미행동 상태가 되었습니다

			kunshu.action_done = false;

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force, 1));
			pk::add_ap(district, -ACTION_COST);

			return true;

		}

		void scene_휴양()
		{
			string kunshu_name = pk::decode(pk::get_name(kunshu));

			int n = pk::rand(3);

			if (n == 0)
			{
				pk::background(4);
				pk::fade(255);
				pk::message_box(pk::encode("难得去市场休假呀."), kunshu);
				pk::message_box(pk::encode("先生, 快来看看. 我这新进了一批好东西!"), merchant);
				pk::message_box(pk::encode("质量看起来不错呀."), kunshu);
				pk::message_box(pk::encode("多少钱?"), kunshu);
				pk::message_box(pk::encode("先生真是好眼力!这东西别处可没有.只要500金."), merchant);
				pk::message_box(pk::encode("好.请收好."), kunshu);
				pk::message_box(pk::encode("谢谢惠顾."), merchant);

				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x在市场休养很享受.", kunshu_name)));
			}
			else if (n == 1)
			{
				pk::background(6);
				pk::fade(255);
				pk::message_box(pk::encode("好不容易有空, 去钓鱼好了."), kunshu);
				pk::message_box(pk::encode("我可以和你一起钓鱼吗?"), boy);
				pk::message_box(pk::encode("(好可爱的小孩.)"), kunshu);
				pk::message_box(pk::encode("好, 就在这里一起钓."), kunshu);
				pk::message_box(pk::encode("今天好像没什么鱼.."), boy);
				pk::message_box(pk::encode("看来鱼不出来了."), kunshu);
				pk::message_box(pk::encode("......"), boy);
				pk::message_box(pk::encode("今天很开心, 请收下这500金."), kunshu);
				pk::message_box(pk::encode("哇!好幸运...!"), boy);
				pk::message_box(pk::encode("非常感谢!"), boy);

				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x休养时和少年一起钓鱼.", kunshu_name)));
			}
			else
			{
				pk::background(9);
				pk::fade(255);
				pk::message_box(pk::encode("难得有空.去看看农民好了."), kunshu);
				pk::message_box(pk::encode(pk::format("哇!是 \x1b[1x{}\x1b[0x大人来了!", kunshu_name)), farmer);
				pk::message_box(pk::encode("看样子最近庄稼长得不错."), kunshu);
				pk::message_box(pk::encode("多亏了主公照顾, 收成不错."), farmer);
				pk::message_box(pk::encode("你们辛苦了.请收下这500金, 改善下生活."), kunshu);
				pk::message_box(pk::encode("太感谢了."), farmer);
				pk::message_box(pk::encode("如果大人什么时候需要帮忙,请一定告诉我!"), farmer);
				pk::message_box(pk::encode("如有需要, 我将随大人出去抗敌!"), farmer);
				pk::message_box(pk::encode("哈哈哈!如果以后有需要我会召集民兵."), kunshu);
				pk::message_box(pk::encode("这些土特产请收下."), farmer);
				pk::message_box(pk::encode("谢谢."), kunshu);

				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和农民一起度过了愉快的一天.", kunshu_name)));

				pk::add_food(kunshu_city, FOOD_사례, true);
			}

			pk::fade(0);
			pk::background(-1);
		}



	}

	Main main;
}
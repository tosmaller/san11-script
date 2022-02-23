// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/08 # 氕氘氚 # 加入【国事】菜单 ##
/*
@ 제작자: HoneyBee
@ 내용: 국호를 변경시킬 수 있는 기능

*/

namespace 국호_변경
{

	// ================ CUSTOMIZE ================

	const int TP_COST = 100; 	   // 기교P 필요량
	const int ACTION_COST = 50;    // 행동력 필요량

	const int KEY = pk::hash("各愤室回");

	// ===========================================

	class Main
	{

		pk::building@ building;
		pk::building@ kunshu_building;

		pk::city@ kunshu_city;
		pk::city@ emperor_city;

		pk::force@ force;

		pk::person@ kunshu;
		pk::person@ emperor;
		pk::person@ support;

		Main()
		{

			pk::menu_item item;
			item.menu = global_menu::菜单_国事;
			//ch::u8debug(pk::format("menu: guo hao bian geng {}", global_menu::菜单_国事));
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
			@kunshu_building = pk::get_building(kunshu.service);
			@kunshu_city = pk::building_to_city(kunshu_building);
			@emperor = pk::get_person(pk::get_scenario().emperor);

		}

		string getText()
		{
			return pk::encode("变更国号");
		}

		string getDesc()
		{
			if (pk::is_protecting_the_emperor(force) or force.title != 爵位_皇帝)
				return pk::encode("还未称帝未拥有国号.");
			else if (kunshu_city.get_id() != kunshu.service)
				return pk::encode("只有君主所在的城市才能执行.");
			else if (pk::is_absent(kunshu) or pk::is_unitize(kunshu) or kunshu.action_done)
				return pk::encode("君主已行动或不在.");
			else if (force.tp < TP_COST)
				return pk::encode(pk::format("技巧点不足.(技巧点 {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足. (必要行动力 {} )", ACTION_COST));
			else
				return pk::encode(pk::format("执行变更国号. (使用技巧点 {} )", TP_COST));
		}

		bool isVisible()
		{
			if (pk::is_protecting_the_emperor(force) or force.title != 爵位_皇帝) return false;
			return true;
		}

		bool isEnabled()
		{
			if (pk::is_protecting_the_emperor(force) or force.title != 爵位_皇帝) return false;
			else if (kunshu_city.get_id() != kunshu.service) return false;
			else if (pk::is_absent(kunshu) or pk::is_unitize(kunshu) or kunshu.action_done) return false;
			else if (force.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST) return false;
			return true;
		}

		bool handler()
		{

			if (pk::choose({ pk::encode(" 是 "), pk::encode(" 否 ") }, pk::encode(pk::format("是否要更改国家的国号?\n(使用技巧 {} )", TP_COST)), kunshu) == 1)
				return false;

			pk::message_box(pk::encode("我觉得更改这个国家的国号正是顺应天意!"), kunshu);

			// 국호 변경 
			kokugou_0();

			force.update();

			// 행동력 감소
			auto district = pk::get_district(pk::get_district_id(force, 1));
			pk::add_ap(district, -ACTION_COST);

			// 기교 감소
			ch::add_tp(force, -TP_COST, force.get_pos());

			pk::message_box(pk::encode("国号变更了."));
			pk::create_effect(0x51, kunshu_building.pos);   //이펙트효과

			kunshu.action_done = true;

			return true;

		}

		void kokugou_0()
		{

			array<string> kokugou_list =
			{
				pk::encode(" 魏 "),
				pk::encode(" 吴 "),
				pk::encode(" 蜀 "),
				pk::encode(" 晋 "),
				pk::encode(" 汉 "),
				pk::encode("NEXT"),
			};

			int n = pk::choose(pk::encode("请选择国号."), kokugou_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x魏\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_위;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x吴\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_오;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x蜀\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_촉;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x晋\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_진;
			}
			else if (n == 4)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x汉\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_한;
			}
			else if (n == 5)
			{
				kokugou_1();
			}


		} // kokugou_0()

		void kokugou_1()
		{
			array<string> kokugou_list =
			{
				pk::encode(" 成 "),
				pk::encode(" 楚 "),
				pk::encode(" 燕 "),
				pk::encode(" 齐 "),
				pk::encode("BACK"),
				pk::encode("NEXT"),
			};

			int n = pk::choose(pk::encode("请选择国号."), kokugou_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x成\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_성;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x楚\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_초;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x燕\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_연;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x齐\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_제;
			}
			else if (n == 4)
			{
				kokugou_0();
			}
			else if (n == 5)
			{
				kokugou_2();
			}


		} // kokugou_1()

		void kokugou_2()
		{
			array<string> kokugou_list =
			{
				pk::encode(" 赵 "),
				pk::encode(" 夏 "),
				pk::encode(" 商 "),
				pk::encode(" 周 "),
				pk::encode(" 越 "),
				pk::encode("BACK"),
			};

			int n = pk::choose(pk::encode("请选择国号."), kokugou_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x赵\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_조;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x夏\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_하;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x商\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_상;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x周\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_주;
			}
			else if (n == 4)
			{
				pk::message_box(pk::encode("我宣布这个国家将以\x1b[1x越\x1b[0x作为国号!"), kunshu);
				force.kokugou = 국호_월;
			}
			else if (n == 5)
			{
				kokugou_1();
			}

		} // kokugou_2()

	}

	Main main;
}
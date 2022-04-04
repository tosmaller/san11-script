// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/08 # 氕氘氚 # 加入【国事】菜单 ##
/*
@ 제작자: HoneyBee
@ 내용: 세력의 COLOR를 변경시킬 수 있는 기능

*/

namespace 세력_색상
{

	// ================ CUSTOMIZE ================

	const int TP_COST = 100; 	   // 기교P 필요량
	const int ACTION_COST = 50;    // 행동력 필요량

	const int KEY = pk::hash("症肱哨匠");

	// ===========================================

	class Main
	{

		pk::building@ building_;
		pk::building@ kunshu_building;


		pk::city@ kunshu_city;
		pk::city@ emperor_city;

		pk::force@ force_;

		pk::person@ kunshu_;
		pk::person@ emperor;
		pk::person@ support;

		Main()
		{

			pk::menu_item item;
			item.menu = global_menu::菜单_国事;
			//ch::u8debug(pk::format("menu: shi li yan se {}", global_menu::菜单_国事));      
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

			@building_ = @building;
			@force_ = pk::get_force((building_).get_force_id());
			@kunshu_ = pk::get_person(pk::get_kunshu_id(building_));
			@kunshu_building = pk::get_building(kunshu_.service);
			@kunshu_city = pk::building_to_city(kunshu_building);
			@emperor = pk::get_person(pk::get_scenario().emperor);


		}

		string getText()
		{
			return pk::encode("势力颜色");  //세력색상shiliyanse
		}

		string getDesc()
		{
			if (building_.get_id() != kunshu_.service)
				return pk::encode("只能在君主所在城市或港关实行.");
			else if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_) or kunshu_.action_done)
				return pk::encode("君主不在或已行动.");
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("改变势力的颜色. (必须 {} 技巧P)", TP_COST));
		}

		bool isVisible()
		{
			if (building_.get_id() != kunshu_.service) return false;
			return true;
		}

		bool isEnabled()
		{
			//uint district_ap = pk::get_district(pk::get_district_id(force, 1)).ap
			if (building_.get_id() != kunshu_.service) return false;
			else if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_) or kunshu_.action_done) return false;
			else if (force_.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		bool handler()
		{

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("改变势力的颜色. (必须 {} 技巧P)", TP_COST)), kunshu_) == 1)
				return false;

			// 세력 COLOR 변경 
			color_0();

			force_.update();

			// 행동력 감소
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			// 기교 감소
			ch::add_tp(force_, -TP_COST, force_.get_pos());

			pk::message_box(pk::encode("势力的颜色发生了变化."));
			pk::create_effect(0x51, kunshu_building.pos);   //이펙트효과

			kunshu_.action_done = true;

			return true;

		}

		void color_0()
		{

			array<string> color_list =
			{
				pk::encode("0(曹操)"),  //caocao
				pk::encode("1(刘备)"),  //liubei
				pk::encode("2(孙坚)"),  //sunjian
				pk::encode("3(马腾)"),  //mateng
				pk::encode("4(刘焉)"),  //liuyan
				pk::encode("下一页"),
			};

			int n = pk::choose(pk::encode("请选择势力颜色."), color_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("势力颜色改为 0(曹操)!"), kunshu_);
				force_.color = 0;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("势力颜色改为 1(刘备)!"), kunshu_);
				force_.color = 1;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("势力颜色改为 2(孙坚)!"), kunshu_);
				force_.color = 2;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("势力颜色改为 3(马腾)!"), kunshu_);
				force_.color = 3;
			}
			else if (n == 4)
			{
				pk::message_box(pk::encode("势力颜色改为 4(刘焉)!"), kunshu_);
				force_.color = 4;
			}
			else if (n == 5)
			{
				color_1();
			}


		} // color_0()

		void color_1()
		{
			array<string> color_list =
			{
				pk::encode("5(董卓)"),  //dongzhuo
				pk::encode("6(刘表)"),  //liubiao
				pk::encode("7(袁绍)"),  //yuanshao
				pk::encode("8(袁术)"),  //yuanshu
				pk::encode("上一页"),
				pk::encode("下一页"),
			};

			int n = pk::choose(pk::encode("请选择势力颜色."), color_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("势力颜色改为 5(董卓)!"), kunshu_);
				force_.color = 5;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("势力颜色改为 6(刘表)!"), kunshu_);
				force_.color = 6;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("势力颜色改为 7(袁绍)!"), kunshu_);
				force_.color = 7;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("势力颜色改为 8(袁术)!"), kunshu_);
				force_.color = 8;
			}
			else if (n == 4)
			{
				color_0();
			}
			else if (n == 5)
			{
				color_2();
			}


		} // color_1()

		void color_2()
		{
			array<string> color_list =
			{
				pk::encode("9(张鲁)"),
				pk::encode("10(吕布)"),
				pk::encode("11(公孙瓒)"),
				pk::encode("12(公孙度)"),
				pk::encode("上一页"),
				pk::encode("下一页"),
			};

			int n = pk::choose(pk::encode("请选择势力颜色."), color_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("势力颜色改为 9(张鲁)!"), kunshu_);
				force_.color = 9;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("势力颜色改为 10(吕布)!"), kunshu_);
				force_.color = 10;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("势力颜色改为 11(公孙瓒)!"), kunshu_);
				force_.color = 11;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("势力颜色改为 12(公孙度)!"), kunshu_);
				force_.color = 12;
			}
			else if (n == 4)
			{
				color_1();
			}
			else if (n == 5)
			{
				color_3();
			}


		} // color_2()

		void color_3()
		{
			array<string> color_list =
			{
				pk::encode("13(刘虞)"),
				pk::encode("14(韩馥)"),
				pk::encode("15(司马懿)"),
				pk::encode("16(孔融)"),
				pk::encode("上一页"),
				pk::encode("下一页"),
			};

			int n = pk::choose(pk::encode("请选择势力颜色."), color_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("势力颜色改为 13(刘虞)!"), kunshu_);
				force_.color = 13;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("势力颜色改为 14(韩馥)!"), kunshu_);
				force_.color = 14;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("势力颜色改为 15(司马懿)!"), kunshu_);
				force_.color = 15;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("势力颜色改为 16(孔融)!"), kunshu_);
				force_.color = 16;
			}
			else if (n == 4)
			{
				color_2();
			}
			else if (n == 5)
			{
				color_4();
			}

		} // color_3()

		void color_4()
		{
			array<string> color_list =
			{
				pk::encode("17(刘岱)"),
				pk::encode("18(陶谦)"),
				pk::encode("19(孔伷)"),
				pk::encode("20(丁原)"),
				pk::encode("上一页"),
				pk::encode("下一页"),
			};

			int n = pk::choose(pk::encode("请选择势力颜色."), color_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("势力颜色改为 17(刘岱)!"), kunshu_);
				force_.color = 17;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("势力颜色改为 18(陶谦)!"), kunshu_);
				force_.color = 18;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("势力颜色改为 19(孔伷)!"), kunshu_);
				force_.color = 19;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("势力颜色改为 20(丁原)!"), kunshu_);
				force_.color = 20;
			}
			else if (n == 4)
			{
				color_3();
			}
			else if (n == 5)
			{
				color_5();
			}

		} // color_4()

		void color_5()
		{
			array<string> color_list =
			{
				pk::encode("22(王朗)"),
				pk::encode("23(严白虎)"),
				pk::encode("24(刘繇)"),
				pk::encode("25(张绣)"),
				pk::encode("上一页"),
				pk::encode("下一页"),
			};

			int n = pk::choose(pk::encode("请选择势力颜色."), color_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("势力颜色改为 22(王朗)!"), kunshu_);
				force_.color = 22;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("势力颜色改为 23(严白虎)!"), kunshu_);
				force_.color = 23;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("势力颜色改为 24(刘繇)!"), kunshu_);
				force_.color = 24;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("势力颜色改为 25(张绣)!"), kunshu_);
				force_.color = 25;
			}
			else if (n == 4)
			{
				color_4();
			}
			else if (n == 5)
			{
				color_6();
			}

		} // color_5()

		void color_6()
		{
			array<string> color_list =
			{
				pk::encode("26(孟获)"),
				pk::encode("27(雍闿)"),
				pk::encode("28(张角)"),
				pk::encode("29(何进)"),
				pk::encode("上一页"),
				pk::encode("下一页"),
			};

			int n = pk::choose(pk::encode("请选择势力颜色."), color_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("势力颜色改为 26(孟获)!"), kunshu_);
				force_.color = 26;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("势力颜色改为 27(雍闿)!"), kunshu_);
				force_.color = 27;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("势力颜色改为 28(张角)!"), kunshu_);
				force_.color = 28;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("势力颜色改为 29(何进)!"), kunshu_);
				force_.color = 29;
			}
			else if (n == 4)
			{
				color_5();
			}
			else if (n == 5)
			{
				color_7();
			}

		} // color_6()

		void color_7()
		{
			array<string> color_list =
			{
				pk::encode("31(韩玄)"),
				pk::encode("32(金璇)"),
				pk::encode("33(赵范)"),
				pk::encode("34(刘度)"),
				pk::encode("上一页"),
				pk::encode("下一页"),
			};

			int n = pk::choose(pk::encode("请选择势力颜色."), color_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("势力颜色改为 31(韩玄)!"), kunshu_);
				force_.color = 31;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("势力颜色改为 32(金璇)!"), kunshu_);
				force_.color = 32;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("势力颜色改为 33(赵范)!"), kunshu_);
				force_.color = 33;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("势力颜色改为 34(刘度)!!"), kunshu_);
				force_.color = 34;
			}
			else if (n == 4)
			{
				color_6();
			}
			else if (n == 5)
			{
				color_8();
			}

		} // color_7()

		void color_8()
		{
			array<string> color_list =
			{
				pk::encode("38(钟会)"),
				pk::encode("39(邓艾)"),
				pk::encode("40(毌丘俭)"),
				pk::encode("41(韩遂)"),
				pk::encode("上一页")
			};

			int n = pk::choose(pk::encode("请选择势力颜色."), color_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("势力颜色改为 38(钟会)!"), kunshu_);
				force_.color = 38;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("势力颜色改为 39(邓艾)!"), kunshu_);
				force_.color = 39;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("势力颜色改为 40(毌丘俭)!"), kunshu_);
				force_.color = 40;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("势力颜色改为 41(韩遂)!"), kunshu_);
				force_.color = 41;
			}
			else if (n == 4)
			{
				color_7();
			}

		} // color_8()

	}

	Main main;
}
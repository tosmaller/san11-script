// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/01 # 江东新风 # 三顾下无法触发bug修复，符号不显示错误修复 ##
// ## 2021/09/29 # 江东新风 # 触发时间判断错误修正 ##
// ## 2021/08/16 # 江东新风 # 关掉自动触发，改成菜单触发 ##
// ## 2021/02/15 # 江东新风 # 出征中不发生剧情 ##
// ## 2021/01/29 # 江东新风 # 完善 ##
// ## 2021/01/26 # 江东新风 # 新地图的三顾茅庐剧情 ##
/*
目前看来只有target[0]决定任务目标
//开发命令对应target[0]，建筑id。技巧研究对象建筑，亲善对象是势力，仲介无目标

！！！！！需要注意剧本时间，让非正常时间时三顾按钮消失
*/

namespace 事件_三顾茅庐
{


	// ================ CUSTOMIZE ================
	const bool 事件剧本限定 = true;//默认限定剧本，限定4号剧本三顾茅庐可用，如不限定剧本，则可能在非对应剧本满足条件也会触发，但是会增加事件菜单的占用，以及可能会有未知bug
	// ===========================================


	class Main
	{
		pk::person@ 司马徽;
		pk::person@ 刘备;
		pk::person@ 张飞;
		pk::person@ 关羽;
		pk::person@ 诸葛亮;
		pk::person@ 童子;
		pk::person@ 诸葛均;
		pk::building@ building_;
		pk::force@ force_;
		pk::person@ kunshu_;
		pk::city@ city_;

		string info_事件信息;

		bool show_info = false;

		Main()
		{
			//pk::bind(pk::TRIGGER_TURN_START, pk::trigger111_t(onTurnStart));//关掉自动触发，改成菜单触发
			add_menu_一顾();
			add_menu_二顾();
			add_menu_三顾();
			pk::bind(120, pk::trigger120_t(func_信息显示_事件));//试试通过按钮来显示相关信息
		}

		void func_信息显示_事件()
		{
			if (!show_info) return;


			int line = 11;
			int middle = int(pk::get_resolution().width) / 2;
			int left = middle - 250;
			int right = middle + 250;
			int top = 5;
			int top_start = top + 200;
			pk::point leftdown = pk::point(left + 500, top_start + 28 * line);
			pk::draw_filled_rect(pk::rectangle(pk::point(left + 5, top_start), leftdown), ((0xff / 2) << 24) | 0x777777);

			pk::draw_text(info_事件信息, pk::point(left + 10, top_start), 0xffffffff, FONT_BIG, 0xff000000);

		}

		//------------------------------添加菜单---------------------------------------------//
		void add_menu_一顾()
		{
			pk::menu_item item_一顾;
			item_一顾.menu = global_menu::菜单_事件;
			item_一顾.init = pk::building_menu_item_init_t(init_一顾);
			item_一顾.is_enabled = pk::menu_item_is_enabled_t(isEnabled_一顾);
			item_一顾.is_visible = pk::menu_item_is_visible_t(isVisible_一顾);
			item_一顾.get_text = pk::menu_item_get_text_t(getText_一顾);
			item_一顾.get_desc = pk::menu_item_get_desc_t(getDesc_一顾);
			item_一顾.handler = pk::menu_item_handler_t(handler_一顾);
			pk::add_menu_item(item_一顾);
		}

		void add_menu_二顾()
		{
			pk::menu_item item_二顾;
			item_二顾.menu = global_menu::菜单_事件;
			item_二顾.init = pk::building_menu_item_init_t(init_二顾);
			item_二顾.is_enabled = pk::menu_item_is_enabled_t(isEnabled_二顾);
			item_二顾.is_visible = pk::menu_item_is_visible_t(isVisible_二顾);
			item_二顾.get_text = pk::menu_item_get_text_t(getText_二顾);
			item_二顾.get_desc = pk::menu_item_get_desc_t(getDesc_二顾);
			item_二顾.handler = pk::menu_item_handler_t(handler_二顾);
			pk::add_menu_item(item_二顾);
		}

		void add_menu_三顾()
		{
			pk::menu_item item_三顾;
			item_三顾.menu = global_menu::菜单_事件;
			item_三顾.init = pk::building_menu_item_init_t(init_三顾);
			item_三顾.is_enabled = pk::menu_item_is_enabled_t(isEnabled_三顾);
			item_三顾.is_visible = pk::menu_item_is_visible_t(isVisible_三顾);
			item_三顾.get_text = pk::menu_item_get_text_t(getText_三顾);
			item_三顾.get_desc = pk::menu_item_get_desc_t(getDesc_三顾);
			item_三顾.handler = pk::menu_item_handler_t(handler_三顾);
			pk::add_menu_item(item_三顾);
		}
		//------------------------------一顾茅庐---------------------------------------------//
		void init_一顾(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			//@taishu_ = pk::get_person(pk::get_taishu_id(building));
			@kunshu_ = pk::get_person(force_.kunshu);
			@city_ = pk::building_to_city(building);
			@ 诸葛亮 = pk::get_person(武将_诸葛亮);
			@ 刘备 = pk::get_person(武将_刘备);
			@ 张飞 = pk::get_person(武将_张飞);
			@ 关羽 = pk::get_person(武将_关羽);
		}

		string getText_一顾()
		{
			return pk::encode("三顾茅庐(上)");  //quchu teji
		}

		bool isVisible_一顾()
		{
			if (setting_ex.eve_happened[40]) return false;//40一顾，41二顾，42三顾
			if (pk::get_scenario().no != 4 and 事件剧本限定) return false;//注意如果改过剧本，那这里需要改动
			return true;
		}

		bool isEnabled_一顾()
		{
			/*
			//诸葛亮需未登场
			if (诸葛亮.mibun != 身份_在野 and 诸葛亮.mibun != 身份_未发现) return false;
			//刘备的身份是君主
			else if (force_.kunshu != 武将_刘备) return false;
			//新野所属为刘备军
			else if (pk::get_building(81).get_force_id() != force_.get_id()) return false;
			//关羽所属为刘备军
			else if (关羽.get_force_id() != force_.get_id()) return false;
			//张飞所属为刘备军
			else if (张飞.get_force_id() != force_.get_id()) return false;
			//刘备需可行动
			else if (pk::is_absent(刘备) or pk::is_unitize(刘备)) return false;
			//关羽需可行动
			else if (pk::is_absent(关羽) or pk::is_unitize(关羽)) return false;
			//张飞需可行动
			else if (pk::is_absent(张飞) or pk::is_unitize(张飞)) return false;*/
			return true;
		}

		string getDesc_一顾()
		{
			/*
			if (诸葛亮.mibun != 身份_在野 and 诸葛亮.mibun != 身份_未发现)
				return pk::encode("诸葛亮的身份不是在野或未发现.");
			//刘备的身份是君主
			else if (force_.kunshu != 武将_刘备) return pk::encode("刘备不是所属势力君主.");
			//新野所属为刘备军
			else if (pk::get_building(81).get_force_id() != force_.get_id()) return pk::encode("新野不在所属势力.");
			//关羽所属为刘备军
			else if (关羽.get_force_id() != force_.get_id()) return pk::encode("关羽不在所属势力.");
			//张飞所属为刘备军
			else if (张飞.get_force_id() != force_.get_id()) return pk::encode("张飞不在所属势力.");
			//刘备需可行动
			else if (pk::is_absent(刘备) or pk::is_unitize(刘备)) return pk::encode("刘备不在或已行动.");
			//关羽需可行动
			else if (pk::is_absent(关羽) or pk::is_unitize(关羽)) return pk::encode("关羽不在或已行动.");
			//张飞需可行动
			else if (pk::is_absent(张飞) or pk::is_unitize(张飞)) return pk::encode("张飞不在或已行动.");*/
			return pk::encode("执行三顾茅庐事件.");

		}

		bool handler_一顾()
		{
			bool confirm = pk::yes_no(pk::encode("是否查看\x1b[2x三顾茅庐(上篇)\x1b[0x事件条件?"));
			if (confirm)
			{
				bool start_event = false;
				info_事件信息 = 获取任务条件_一顾();
				show_info = true;
				if (check_all_一顾())
				{
					start_event = pk::yes_no(pk::encode("满足事件条件，是否执行\x1b[2x三顾茅庐(上篇)\x1b[0x?"));
					if (start_event)
					{
						show_info = false;
						pk::scene(pk::scene_t(event_一顾));
					}
				}
				else
				{
					pk::message_box(pk::encode("\x1b[2x三顾茅庐(上篇)\x1b[0x条件不满足，事件无法执行。"));
				}
			}
			show_info = false;
			return true;
		}

		//------------------------------二顾茅庐---------------------------------------------//
		void init_二顾(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			//@taishu_ = pk::get_person(pk::get_taishu_id(building));
			@kunshu_ = pk::get_person(force_.kunshu);
			@city_ = pk::building_to_city(building);
			@ 诸葛亮 = pk::get_person(武将_诸葛亮);
			@ 刘备 = pk::get_person(武将_刘备);
			@ 张飞 = pk::get_person(武将_张飞);
			@ 关羽 = pk::get_person(武将_关羽);
		}

		string getText_二顾()
		{
			return pk::encode("三顾茅庐(中)");  //quchu teji
		}

		bool isVisible_二顾()
		{
			if (pk::get_scenario().no != 4 and 事件剧本限定) return false;//注意如果改过剧本，那这里需要改动
			if (setting_ex.eve_happened[40] and !setting_ex.eve_happened[41]) return true;//40一顾，41二顾，42三顾
			return false;
		}

		bool isEnabled_二顾()
		{
			/*
			forceinfo@ force_t = @force_ex[force_.get_id()];
			if ((pk::get_scenario().turn_counter - force_t.sangu_1_turn_count) < 3) return false;
			//诸葛亮需未登场
			else if (诸葛亮.mibun != 身份_在野 and 诸葛亮.mibun != 身份_未发现) return false;
			//刘备的身份是君主
			else if (force_.kunshu != 武将_刘备) return false;
			//新野所属为刘备军
			else if (pk::get_building(81).get_force_id() != force_.get_id()) return false;
			//关羽所属为刘备军
			else if (关羽.get_force_id() != force_.get_id()) return false;
			//张飞所属为刘备军
			else if (张飞.get_force_id() != force_.get_id()) return false;
			//刘备需可行动
			else if (pk::is_absent(刘备) or pk::is_unitize(刘备)) return false;
			//关羽需可行动
			else if (pk::is_absent(关羽) or pk::is_unitize(关羽)) return false;
			//张飞需可行动
			else if (pk::is_absent(张飞) or pk::is_unitize(张飞)) return false;*/
			return true;
		}

		string getDesc_二顾()
		{
			/*
			forceinfo@ force_t = @force_ex[force_.get_id()];
			if ((pk::get_scenario().turn_counter - force_t.sangu_1_turn_count) < 3) 
				return pk::encode("第一次拜访后经过时间不足.");
			else if (诸葛亮.mibun != 身份_在野 and 诸葛亮.mibun != 身份_未发现)
				return pk::encode("诸葛亮的身份不是在野或未发现.");
			//刘备的身份是君主
			else if (force_.kunshu != 武将_刘备) return pk::encode("刘备不是所属势力君主.");
			//新野所属为刘备军
			else if (pk::get_building(81).get_force_id() != force_.get_id()) return pk::encode("新野不在所属势力.");
			//关羽所属为刘备军
			else if (关羽.get_force_id() != force_.get_id()) return pk::encode("关羽不在所属势力.");
			//张飞所属为刘备军
			else if (张飞.get_force_id() != force_.get_id()) return pk::encode("张飞不在所属势力.");
			//刘备需可行动
			else if (pk::is_absent(刘备) or pk::is_unitize(刘备)) return pk::encode("刘备不在或已行动.");
			//关羽需可行动
			else if (pk::is_absent(关羽) or pk::is_unitize(关羽)) return pk::encode("关羽不在或已行动.");
			//张飞需可行动
			else if (pk::is_absent(张飞) or pk::is_unitize(张飞)) return pk::encode("张飞不在或已行动.");*/
			return pk::encode("执行三顾茅庐事件.");

		}

		bool handler_二顾()
		{
			bool confirm = pk::yes_no(pk::encode("是否查看\x1b[2x三顾茅庐(中篇)\x1b[0x事件条件?"));
			if (confirm)
			{
				bool start_event = false;
				info_事件信息 = 获取任务条件_二顾();
				show_info = true;
				if (check_all_二顾())
				{
					start_event = pk::yes_no(pk::encode("满足事件条件，是否执行\x1b[2x三顾茅庐(中篇)\x1b[0x?"));
					if (start_event)
					{
						show_info = false;
						pk::scene(pk::scene_t(event_二顾));
					}
				}
				else
				{
					pk::message_box(pk::encode("\x1b[2x三顾茅庐(中篇)\x1b[0x条件不满足，事件无法执行。"));
				}
			}
			show_info = false;
			return true;
		}

		//------------------------------三顾茅庐---------------------------------------------//
		void init_三顾(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			//@taishu_ = pk::get_person(pk::get_taishu_id(building));
			@kunshu_ = pk::get_person(force_.kunshu);
			@city_ = pk::building_to_city(building);
			@ 诸葛亮 = pk::get_person(武将_诸葛亮);
			@ 刘备 = pk::get_person(武将_刘备);
			@ 张飞 = pk::get_person(武将_张飞);
			@ 关羽 = pk::get_person(武将_关羽);
		}

		string getText_三顾()
		{
			return pk::encode("三顾茅庐(下)");  //quchu teji
		}

		bool isVisible_三顾()
		{
			if (pk::get_scenario().no != 4 and 事件剧本限定) return false;//注意如果改过剧本，那这里需要改动
			if (setting_ex.eve_happened[40] and setting_ex.eve_happened[41] and !setting_ex.eve_happened[42]) return true;//40一顾，41二顾，42三顾
			return false;
		}

		bool isEnabled_三顾()
		{
			/*
			forceinfo@ force_t = @force_ex[force_.get_id()];
			if ((pk::get_scenario().turn_counter - force_t.sangu_2_turn_count) < 3) return false;
			//诸葛亮需未登场
			else if (诸葛亮.mibun != 身份_在野 and 诸葛亮.mibun != 身份_未发现) return false;
			//刘备的身份是君主
			else if (force_.kunshu != 武将_刘备) return false;
			//新野所属为刘备军
			else if (pk::get_building(81).get_force_id() != force_.get_id()) return false;
			//关羽所属为刘备军
			else if (关羽.get_force_id() != force_.get_id()) return false;
			//张飞所属为刘备军
			else if (张飞.get_force_id() != force_.get_id()) return false;
			//刘备需可行动
			else if (pk::is_absent(刘备) or pk::is_unitize(刘备)) return false;
			//关羽需可行动
			else if (pk::is_absent(关羽) or pk::is_unitize(关羽)) return false;
			//张飞需可行动
			else if (pk::is_absent(张飞) or pk::is_unitize(张飞)) return false;*/
			return true;
		}

		string getDesc_三顾()
		{
			/*
			forceinfo@ force_t = @force_ex[force_.get_id()];
			if ((pk::get_scenario().turn_counter - force_t.sangu_2_turn_count) < 3)
				return pk::encode("第二次拜访后经过时间不足.");
			else if (诸葛亮.mibun != 身份_在野 and 诸葛亮.mibun != 身份_未发现)
				return pk::encode("诸葛亮的身份不是在野或未发现.");
			//刘备的身份是君主
			else if (force_.kunshu != 武将_刘备) return pk::encode("刘备不是所属势力君主.");
			//新野所属为刘备军
			else if (pk::get_building(81).get_force_id() != force_.get_id()) return pk::encode("新野不在所属势力.");
			//关羽所属为刘备军
			else if (关羽.get_force_id() != force_.get_id()) return pk::encode("关羽不在所属势力.");
			//张飞所属为刘备军
			else if (张飞.get_force_id() != force_.get_id()) return pk::encode("张飞不在所属势力.");
			//刘备需可行动
			else if (pk::is_absent(刘备) or pk::is_unitize(刘备)) return pk::encode("刘备不在或已行动.");
			//关羽需可行动
			else if (pk::is_absent(关羽) or pk::is_unitize(关羽)) return pk::encode("关羽不在或已行动.");
			//张飞需可行动
			else if (pk::is_absent(张飞) or pk::is_unitize(张飞)) return pk::encode("张飞不在或已行动.");*/
			return pk::encode("执行三顾茅庐事件.");

		}

		bool handler_三顾()
		{
			bool confirm = pk::yes_no(pk::encode("是否查看\x1b[2x三顾茅庐(下篇)\x1b[0x事件条件?"));
			if (confirm)
			{
				bool start_event = false;
				info_事件信息 = 获取任务条件_三顾();
				show_info = true;
				if (check_all_三顾())
				{
					start_event = pk::yes_no(pk::encode("满足事件条件，是否执行\x1b[2x三顾茅庐(下篇)\x1b[0x?"));
					if (start_event)
					{
						show_info = false;
						pk::scene(pk::scene_t(event_三顾));
					}
				}
				else
				{
					pk::message_box(pk::encode("\x1b[2x三顾茅庐(下篇)\x1b[0x条件不满足，事件无法执行。"));
				}
			}
			show_info = false;
			return true;
		}

		//------------------------------任务条件---------------------------------------------//
		string 获取任务条件_一顾()
		{
			//！！！可以根据刘备是否属于陶谦势力分两种情况？

			string info_事件信息_t = pk::encode("   \x1b[1x三顾茅庐(上篇)触发条件\x1b[0x\n");
			string info_条件0 = (条件0_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("日期迟于208年1月上旬\x1b[0x");
			string info_条件1 = (条件1_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("经过回合数大于12\x1b[0x");
			string info_条件2 = (条件2_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备的身份是君主\x1b[0x");
			string info_条件3 = (条件3_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("新野的所属势力是刘备军\x1b[0x");
			string info_条件4 = (条件4_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("关羽的所属势力是刘备军\x1b[0x");
			string info_条件5 = (条件5_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("张飞的所属势力是刘备军\x1b[0x");
			string info_条件6 = (条件6_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备军的统治城市数小于2\x1b[0x");
			string info_条件7 = (条件7_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("诸葛亮的身份是未发现或在野\x1b[0x");
			string info_条件8 = (条件8_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备、关羽、张飞可参加事件\x1b[0x");
			info_事件信息_t = info_事件信息_t + info_条件0 + info_条件1 + info_条件2 + info_条件3 + info_条件4 + info_条件5 + info_条件6 + info_条件7 + info_条件8;
			return info_事件信息_t;
		}

		string 获取任务条件_二顾()
		{
			//！！！可以根据刘备是否属于陶谦势力分两种情况？

			string info_事件信息_t = pk::encode("   \x1b[1x三顾茅庐(中篇)触发条件\x1b[0x\n");
			string info_条件9 = (条件9_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("三顾茅庐-前篇发生后经过的回合数大于3\x1b[0x");

			string info_条件2 = (条件2_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备的身份是君主\x1b[0x");
			string info_条件3 = (条件3_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("新野的所属势力是刘备军\x1b[0x");
			string info_条件4 = (条件4_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("关羽的所属势力是刘备军\x1b[0x");
			string info_条件5 = (条件5_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("张飞的所属势力是刘备军\x1b[0x");
			string info_条件6 = (条件6_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备军的统治城市数小于2\x1b[0x");
			string info_条件7 = (条件7_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("诸葛亮的身份是未发现或在野\x1b[0x");
			string info_条件8 = (条件8_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备、关羽、张飞可参加事件\x1b[0x");
			info_事件信息_t = info_事件信息_t + info_条件2 + info_条件3 + info_条件4 + info_条件5 + info_条件6 + info_条件7 + info_条件8 + info_条件9;
			return info_事件信息_t;
		}

		string 获取任务条件_三顾()
		{
			//！！！可以根据刘备是否属于陶谦势力分两种情况？

			string info_事件信息_t = pk::encode("   \x1b[1x三顾茅庐(下篇)触发条件\x1b[0x\n");
			string info_条件10 = (条件10_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("三顾茅庐-中篇发生后经过的回合数大于3\x1b[0x");

			string info_条件2 = (条件2_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备的身份是君主\x1b[0x");
			string info_条件3 = (条件3_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("新野的所属势力是刘备军\x1b[0x");
			string info_条件4 = (条件4_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("关羽的所属势力是刘备军\x1b[0x");
			string info_条件5 = (条件5_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("张飞的所属势力是刘备军\x1b[0x");
			string info_条件6 = (条件6_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备军的统治城市数小于2\x1b[0x");
			string info_条件7 = (条件7_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("诸葛亮的身份是未发现或在野\x1b[0x");
			string info_条件8 = (条件8_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备、关羽、张飞可参加事件\x1b[0x");
			info_事件信息_t = info_事件信息_t + info_条件2 + info_条件3 + info_条件4 + info_条件5 + info_条件6 + info_条件7 + info_条件8 + info_条件10;
			return info_事件信息_t;
		}
		//------------------------------条件判定---------------------------------------------//

		bool check_all_一顾()
		{
			if (条件0_check() and 条件1_check() and 条件2_check() and 条件3_check() and 条件4_check() and 条件5_check() and 条件6_check() and 条件7_check() and 条件8_check())
				return true;
			return false;
		}


		bool check_all_二顾()
		{
			if (条件2_check() and 条件3_check() and 条件4_check() and 条件5_check() and 条件6_check() and 条件7_check() and 条件8_check() and 条件9_check())
				return true;
			return false;
		}


		bool check_all_三顾()
		{
			if (条件2_check() and 条件3_check() and 条件4_check() and 条件5_check() and 条件6_check() and 条件7_check() and 条件8_check() and 条件10_check())
				return true;
			return false;
		}


		bool 条件0_check()
		{
			//pk::scenario @ scenario0 = pk::get_scenario();
			//int start_year = scenario0.start_year;
			int current_year = pk::get_year();
			//pk::trace(pk::format("start_year:{},elapsed_years:{}",start_year,pk::get_elapsed_years()));
			if (current_year >= 208) return true;
			return false;
		}

		bool 条件1_check()
		{
			if (pk::get_elapsed_days() > 12) return true;
			return false;
		}

		bool 条件2_check()
		{
			if (pk::get_person(武将_刘备).mibun == 身份_君主) return true;
			return false;
		}

		bool 条件3_check()
		{
			if (刘备.get_force_id() == pk::get_building(81).get_force_id()) return true;//新地图81号据点是新野
			return false;
		}

		bool 条件4_check()
		{
			if (pk::get_person(武将_关羽).get_force_id() == pk::get_person(武将_刘备).get_force_id()) return true;
			return false;
		}

		bool 条件5_check()
		{
			if (pk::get_person(武将_张飞).get_force_id() == pk::get_person(武将_刘备).get_force_id()) return true;
			return false;
		}

		bool 条件6_check()
		{
			if (pk::is_valid_force_id(刘备.get_force_id()) and (pk::get_city_count(pk::get_force(刘备.get_force_id())) <= 2)) return true;
			return false;
		}

		bool 条件7_check()
		{
			if (pk::get_person(武将_诸葛亮).mibun == 身份_未发现 or pk::get_person(武将_诸葛亮).mibun == 身份_在野) return true;
			return false;
		}

		bool 条件8_check()
		{
			if (can_event(关羽) and can_event(刘备) and can_event(张飞)) return true;
			return false;
		}

		bool 条件9_check()
		{
			//pk::trace(pk::format("条件9_check，当前旬数{}：{}", setting_ex.sangu_1_turn_count, pk::get_scenario().turn_counter));
			if (setting_ex.eve_happened[40] and (pk::get_scenario().turn_counter - setting_ex.sangu_1_turn_count) > 2) return true;
			return false;
		}

		bool 条件10_check()
		{
			if (setting_ex.eve_happened[41] and (pk::get_scenario().turn_counter - setting_ex.sangu_2_turn_count) > 2) return true;
			return false;
		}

		bool can_event(pk::person@ person_t)
		{
			if (pk::is_alive(person_t) and !pk::is_absent(person_t) and !pk::is_unitize(person_t)) return true;
			return false;
		}

		//------------------------------三顾茅庐---------------------------------------------//
		void onTurnStart(pk::force@ force)
		{
			if (check_一顾条件(force))
			{
				pk::scene(pk::scene_t(event_一顾));
			}
			if (check_二顾条件(force))
			{
				pk::scene(pk::scene_t(event_二顾));
			}
			if (check_三顾条件(force))
			{
				pk::scene(pk::scene_t(event_三顾));
			}

		}

		/*音效记录
		好事：10，12
		坏事：11，13
		脚步声：17
		战法效果：100+
		*/

		bool check_一顾条件(pk::force@ force)
		{
			//pk::scenario@ scenario0 = pk::get_scenario();
			//pk::trace("三顾pos-2");
			if (force.is_player())
			{
				int start_year = pk::get_scenario().start_year;
				int start_month = pk::get_scenario().start_month;
				//pk::trace(pk::format("start year:{}.month:{}", start_year, start_month));
				if (start_year <= 207 or (start_year == 208 and start_month <= 8))
				{
					//pk::trace("三顾pos-1");
					int current_year = pk::get_year();
					int current_month = pk::get_month();
					//pk::trace(pk::format("current_year:{}.current_month:{}", current_year, current_month));
					if ((current_year == 207 and (current_month <= 12 and current_month >= 10)) or (current_year == 208 and (current_month <= 8 and current_month >= 1)))
					{
						@ 刘备 = pk::get_person(武将_刘备);
						@ 张飞 = pk::get_person(武将_张飞);
						@ 关羽 = pk::get_person(武将_关羽);
						//pk::trace(pk::format("刘备:{}.张飞:{}.关羽:{}", pk::is_absent(刘备), pk::is_absent(张飞), pk::is_absent(关羽)));
						if (!pk::is_absent(刘备) and !pk::is_absent(张飞) and !pk::is_absent(关羽) and !pk::is_unitize(刘备) and !pk::is_unitize(张飞) and !pk::is_unitize(关羽))
						{
							//pk::trace("三顾pos0");
							@ 诸葛亮 = pk::get_person(武将_诸葛亮);
							if (诸葛亮.mibun == 身份_在野 or 诸葛亮.mibun == 身份_未发现)
							{
								//pk::trace("三顾pos1");
								if (!pk::is_ketsuen(诸葛亮, 武将_刘备) and !pk::is_dislike(诸葛亮, 武将_刘备) and !pk::is_dislike(刘备, 武将_诸葛亮) and 诸葛亮.banned_kunshu != 武将_刘备)
								{
									//pk::trace("三顾pos2");
									//还有个支配城市数量需要小于2的设定
									if (pk::get_kunshu_id(pk::get_building(81)) == 武将_刘备)
									{
										//pk::trace("三顾pos3");
										//确保没有发生三顾事件1
										//pk::trace(pk::format("势力id:{},三顾事件1:{}.三顾事件2:{}",force.get_id(), force_t.sangu_1, force_t.sangu_2));
										if (!setting_ex.eve_happened[40]) return true;
									}
								}
							}
						}
					}

				}
			}
			return false;
		}

		void event_一顾()
		{

			@ 刘备 = pk::get_person(武将_刘备);
			@ 张飞 = pk::get_person(武将_张飞);
			@ 关羽 = pk::get_person(武将_关羽);
			@ 童子 = pk::get_person(武将_童子);
			@ 司马徽 = pk::get_person(武将_司马徽);
			@ 诸葛亮 = pk::get_person(武将_诸葛亮);

			pk::move_screen(刘备.get_pos());

			pk::play_bgm(16);
			//play_sfx是用于剧情中的音效
			pk::play_sfx(612);
			pk::message_box(pk::encode("\x1b[1x刘备\x1b[0x大人，您在吗？\n我正好来到这附近，\n就过来看看您……"), 司马徽);
			pk::message_box(pk::encode("喔喔，这不是\x1b[1x水镜\x1b[0x先生吗！\n真是欢迎您的大驾光临。"), 刘备);
			pk::message_box(pk::encode("很好，很好。\n今日是想向您推荐一位人物呀。\n\n在\x1b[2x新野\x1b[0x郊外的\x1b[5x隆中\x1b[0x，有位人称\x1b[1x卧龙\x1b[0x，\n姓\x1b[1x诸葛\x1b[0x，名\x1b[1x亮\x1b[0x，字\x1b[1x孔明\x1b[0x之人……\n\n此人的才智与\x1b[2x周\x1b[0x的\x1b[1x太公望\x1b[0x、\x1b[2x汉\x1b[0x的\x1b[1x张良\x1b[0x，\n可说是并驾齐驱呢。"), 司马徽);
			pk::message_box(pk::encode("什么！竟然会有那样的人物啊！\n先生，真是感谢您的推荐。"), 刘备);
			pk::message_box(pk::encode("呵呵呵。\n很好，很好。"), 司马徽);


			pk::fade(0);
			pk::sleep();
			pk::fade(255);
			//音效
			pk::background(场景_宫廷2);
			pk::play_sfx(11);
			pk::message_box(pk::encode("刚才，\x1b[1x水镜\x1b[0x先生向我推荐，\n在这附近的\x1b[5x隆中\x1b[0x里，有位奇才隐居于此。\n\x1b[1x诸葛亮\x1b[0x，字\x1b[1x孔明\x1b[0x，人称\x1b[1x卧龙\x1b[0x之人。"), 刘备);
			pk::message_box(pk::encode("那是谁啊？\n连听都没听过咧！"), 张飞);
			pk::message_box(pk::encode("\x1b[1x张飞\x1b[0x！安静一点！"), 关羽);
			pk::message_box(pk::encode("这位\x1b[1x诸葛亮\x1b[0x，\n我想请他加入我军，\n但是想听听诸位的意见。"), 刘备);
			pk::message_box(pk::encode("这是好事啊。\n立刻派遣使者，前去邀请过来如何？"), 关羽);
			pk::message_box(pk::encode("不，对方可是人们称之为龙的男人啊。\n应该由我亲自前往邀请，\n才是应有的礼仪吧。\n\x1b[1x关羽\x1b[0x、\x1b[1x张飞\x1b[0x，同我一起前往吧。"), 刘备);
			pk::message_box(pk::encode("不是当真的吧！？\n为何本大爷\x1b[1x张飞\x1b[0x\n非得到\x1b[5x隆中\x1b[0x这种地方去不可……"), 张飞);
			pk::message_box(pk::encode("\x1b[1x张飞\x1b[0x！！\n好啦，快点准备出发了！"), 关羽);
			pk::background(-1);
			pk::point pos0 = pk::point(75,113);
			pk::move_screen(pos0);
			pk::play_bgm(18);
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::background(场景_草庐);
			pk::fade(255);
			//音效			
			auto sfx = pk::play_sfx_loop(614);

			pk::message_box(pk::encode("啧！真麻烦！"), 张飞);
			pk::message_box(pk::encode("\x1b[1x张飞\x1b[0x！\n稍微安静点行吗！"), 关羽);
			pk::message_box(pk::encode("好啦，好啦！"), 张飞);
			pk::message_box(pk::encode("突然来访，真是失礼了。\n请问这里是\x1b[1x诸葛\x1b[0x先生的宅邸吗？"), 刘备);
			pk::message_box(pk::encode("是！是的！\n马上来了！"), 童子);
			//黑屏，脚步声
			pk::fade(0);
			pk::sleep();
			pk::play_sfx(601);
			pk::fade(255);
			pk::message_box(pk::encode("请问您是哪位？"), 童子);
			pk::message_box(pk::encode("哇！是小孩耶！！\n这样的庵里竟然会有小孩！！"), 张飞);
			pk::message_box(pk::encode("（……的确是有哪。\n　有个小孩…）"), 关羽);
			pk::message_box(pk::encode("汉左将军宜城亭侯领豫州牧皇叔刘备"), 刘备);
			pk::message_box(pk::encode("我不记得这许多名字。"), 童子);
			pk::message_box(pk::encode("...你只说刘备来访。"), 刘备);
			pk::message_box(pk::encode("先生现在不在喔！\n今天一大早就出门去了。"), 童子);
			//音效
			pk::play_sfx(613);
			pk::message_box(pk::encode("看吧，俺就知道！"), 张飞);
			pk::message_box(pk::encode("（……知道什么啊？）"), 关羽);
			pk::message_box(pk::encode("没有办法。\n我们改日再访。\n也请代我等向先生问声好。"), 刘备);
			pk::message_box(pk::encode("是！\n我知道了！"), 童子);
			pk::stop_sfx_loop(sfx);
			pk::background(-1);
			pk::move_screen(刘备.get_pos());
			//音效
			pk::play_sfx(12);
			pk::message_box(pk::encode("唉……\n真是的！"), 张飞);

			//事件后处理
			诸葛亮.absence_timer = 36;
			setting_ex.eve_happened[40] = true; //and setting_ex.eve_happened[41] and !setting_ex.eve_happened[42]
			setting_ex.sangu_1_turn_count = uint16(pk::get_scenario().turn_counter);
			//pk::trace(pk::format("事件后，当前旬数{}：{}", setting_ex.sangu_1_turn_count, pk::get_scenario().turn_counter));
			//pk::trace(pk::format("事件后，势力id:{},三顾事件1:{}.事件1旬数:{}，三顾事件2:{}", pk::get_current_turn_force_id(), force_t.sangu_1, force_t.sangu_1_turn_count, force_t.sangu_2));
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		} // void event()

		bool check_二顾条件(pk::force@ force)
		{
			if (force.is_player())
			{
				//用结构体记录三顾事件1已发生，且记录发生时间
				
				//pk::trace(pk::format("事件2前，势力id:{},三顾事件1:{}.事件1旬数:{}，当前旬:{},三顾事件2:{}", pk::get_current_turn_force_id(), force_t.sangu_1, force_t.sangu_1_turn_count, pk::get_scenario().turn_counter, force_t.sangu_2));

				if (setting_ex.eve_happened[40] and !setting_ex.eve_happened[41] and ((pk::get_scenario().turn_counter - setting_ex.sangu_1_turn_count) >= 3))
				{
					@ 刘备 = pk::get_person(武将_刘备);
					@ 张飞 = pk::get_person(武将_张飞);
					@ 关羽 = pk::get_person(武将_关羽);
					if (!pk::is_absent(刘备) and !pk::is_absent(张飞) and !pk::is_absent(关羽) and !pk::is_unitize(刘备) and !pk::is_unitize(张飞) and !pk::is_unitize(关羽))
					{
						@ 诸葛亮 = pk::get_person(武将_诸葛亮);
						if (诸葛亮.mibun == 身份_在野 or 诸葛亮.mibun == 身份_未发现)
						{
							if (!pk::is_ketsuen(诸葛亮, 武将_刘备) and !pk::is_dislike(诸葛亮, 武将_刘备) and !pk::is_dislike(刘备, 武将_诸葛亮) and 诸葛亮.banned_kunshu != 武将_刘备)
							{
								//absence_timer 不在期间，留守计数
								//还有个支配城市数量需要小于2的设定
								if (pk::get_kunshu_id(pk::get_building(81)) == 武将_刘备)
								{
									//上次事件距离此次超过3旬
									if (true)
										return true;
								}
							}
						}
					}
				}
			}
			return false;
		}

		void event_二顾()
		{

			@ 刘备 = pk::get_person(武将_刘备);
			@ 张飞 = pk::get_person(武将_张飞);
			@ 关羽 = pk::get_person(武将_关羽);
			@ 诸葛亮 = pk::get_person(武将_诸葛亮);
			@ 童子 = pk::get_person(武将_童子);
			@ 诸葛均 = pk::get_person(武将_诸葛均);

			pk::move_screen(刘备.get_pos());

			pk::play_bgm(16);
			pk::play_sfx(601);
			pk::message_box(pk::encode("禀报大人！\n近来数日，听说\x1b[1x诸葛亮\x1b[0x大人都留在\n\x1b[5x隆中\x1b[0x的住处里。\n不赶紧前往的话，是否又将错失良机呢？"), pk::get_person(武将_兵士));
			pk::message_box(pk::encode("喔喔，是啊！\n那么，就立刻前往探访吧！"), 刘备);


			pk::fade(0);
			pk::sleep();
			pk::background(场景_宫廷2);
			pk::fade(255);
			
			
			pk::message_box(pk::encode("大家，有好消息喔！\n听说\x1b[1x诸葛\x1b[0x先生好像在家里。"), 刘备);
			//音效
			pk::play_sfx(613);
			pk::message_box(pk::encode("！？\n………………"), 张飞);

			pk::message_box(pk::encode("现在就立刻前往先生的宅邸。\n\x1b[1x关羽\x1b[0x，\x1b[1x张飞\x1b[0x，快点去准备吧。"), 刘备);
			pk::message_box(pk::encode("是的，\n明白了…………嗯！？"), 关羽);
			pk::cutin(CG_眼神);
			pk::message_box(pk::encode("喂，\x1b[1x张飞\x1b[0x！\n你要去哪！！"), 关羽);
			pk::message_box(pk::encode("哇！！\n糟了！被发现啦！！\n\n……不不，\n俺绝对没有想要开溜！\n是真的、绝对、俺保证、绝对！"), 张飞);
			pk::message_box(pk::encode("\x1b[1x张飞\x1b[0x！\n你就乖乖认命吧！！"), 关羽);
			pk::message_box(pk::encode("可恶！！\n……干嘛要为了一个毛头小子，\n费这么大工夫，真是的！"), 张飞);
			pk::message_box(pk::encode("\x1b[1x张飞\x1b[0x，快点跟上来啊。\n不然的话，可要丢下你先走啰！"), 刘备);
			pk::background(-1);
			
			pk::point pos0 = pk::point(75, 113);
			pk::move_screen(pos0);	
			pk::play_bgm(18);
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::background(场景_草庐);
			pk::fade(255);

			//换bgm

			
			auto sfx = pk::play_sfx_loop(608);
			pk::message_box(pk::encode("啊啊……\n真没想到又来到这种地方来了！"), 张飞);
			pk::message_box(pk::encode("\x1b[1x张飞\x1b[0x！！"), 关羽);
			pk::message_box(pk::encode("好啦好啦！\n俺说知道了嘛！"), 张飞);
			pk::message_box(pk::encode("突然冒昧来访，真是失礼了。\n听说\x1b[1x诸葛\x1b[0x先生今日在府里，\n因此前来探访。\n\x1b[1x诸葛\x1b[0x先生是否在家呢？"), 刘备);
			pk::play_sfx(601);
			pk::message_box(pk::encode("嗯！今天好像在呢！\n先生、先生！\n之前来访过的客人来了唷！"), 童子);
			//怎么做到姓名问号
			诸葛均.sei = pk::encode("？");
			诸葛均.mei = pk::encode("？");
			pk::message_box(pk::encode("之前来访过的客人？\n嗯，会是哪一位呢？"), 诸葛均);
			pk::message_box(pk::encode("喔喔，\x1b[1x诸葛\x1b[0x先生！\n没有任何事比得上能够拜见\n您的尊颜更令人感到高兴的了。\n我是前些日子到此拜访过的，\n\x1b[2x新野\x1b[0x的\x1b[1x刘备\x1b[0x。"), 刘备);
			pk::message_box(pk::encode("嗯？先生？\n啊，是指兄长的事情吧。"), 诸葛均);
			诸葛均.sei = pk::encode("诸葛");
			诸葛均.mei = pk::encode("均");
			pk::message_box(pk::encode("我是他的弟弟\x1b[1x诸葛均\x1b[0x。\n真的是很抱歉呀，\n兄长他刚好又出门了……"), 诸葛均);
			//音效
			pk::play_sfx(613);
			pk::message_box(pk::encode("吼！又来了！"), 张飞);
			pk::message_box(pk::encode("（……什么又来了啊？）"), 关羽);
			pk::message_box(pk::encode("是这样啊，那就没办法了。\n那么，今日我就先留下一封书信吧。"), 刘备);
			pk::stop_sfx_loop(sfx);
			pk::cutin(CG_诏书);
			pk::message_box(pk::encode("这是一封写满了我的志向的书信。\n待先生回府之后，麻烦请你交给他了。\n……那么，后会有期了。"), 刘备);
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);

			pk::move_screen(刘备.get_pos());
			//音效
			pk::play_sfx(12);
			pk::message_box(pk::encode("唉……\n真是受不了！"), 张飞);

			//事件后处理
			setting_ex.eve_happened[41] = true;// and !setting_ex.eve_happened[42]
			setting_ex.sangu_2_turn_count = uint16(pk::get_scenario().turn_counter);


		} // void event_二顾()

		bool check_三顾条件(pk::force@ force)
		{
			if (force.is_player())
			{
				//用结构体记录三顾事件2已发生，且记录发生时间
				if (setting_ex.eve_happened[41] and !setting_ex.eve_happened[42] and ((pk::get_scenario().turn_counter - setting_ex.sangu_2_turn_count) >= 3))
				{
					@ 刘备 = pk::get_person(武将_刘备);
					@ 张飞 = pk::get_person(武将_张飞);
					@ 关羽 = pk::get_person(武将_关羽);
					if (!pk::is_absent(刘备) and !pk::is_absent(张飞) and !pk::is_absent(关羽) and !pk::is_unitize(刘备) and !pk::is_unitize(张飞) and !pk::is_unitize(关羽))
					{
						@ 诸葛亮 = pk::get_person(武将_诸葛亮);
						if (诸葛亮.mibun == 身份_在野 or 诸葛亮.mibun == 身份_未发现)
						{
							if (!pk::is_ketsuen(诸葛亮, 武将_刘备) and !pk::is_dislike(诸葛亮, 武将_刘备) and !pk::is_dislike(刘备, 武将_诸葛亮) and 诸葛亮.banned_kunshu != 武将_刘备)
							{
								//absence_timer 不在期间，留守计数
								//还有个支配城市数量需要小于2的设定
								if (pk::get_kunshu_id(pk::get_building(81)) == 武将_刘备)
								{
									return true;
								}
							}
						}
					}
				}
			}
			return false;
		}

		void event_三顾()
		{

			@ 刘备 = pk::get_person(武将_刘备);
			@ 张飞 = pk::get_person(武将_张飞);
			@ 关羽 = pk::get_person(武将_关羽);
			@ 诸葛亮 = pk::get_person(武将_诸葛亮);
			@ 童子 = pk::get_person(武将_童子);
			@ 诸葛均 = pk::get_person(武将_诸葛均);
			pk::person@ 黄月英 = pk::get_person(武将_黄月英);

			pk::move_screen(刘备.get_pos());

			pk::play_bgm(16);
			pk::play_sfx(601);
			pk::message_box(pk::encode("禀报大人！\n\x1b[1x诸葛\x1b[0x大人好像回到\x1b[5x隆中\x1b[0x的宅邸了。"), pk::get_person(武将_文官));
			//音乐
			pk::message_box(pk::encode("喔，是吗！\n这回我有预感，一定会见到面。"), 刘备);


			pk::fade(0);
			pk::sleep();
			pk::fade(255);

			pk::background(场景_宫廷2);
			pk::message_box(pk::encode("\x1b[1x诸葛\x1b[0x先生好像回府了。\n立刻前往\x1b[5x隆中\x1b[0x吧！"), 刘备);
			pk::cutin(CG_激昂);
			pk::message_box(pk::encode("啊！？等、等等啊！！\n还要去啊？\n已经够了吧！"), 张飞);
			pk::message_box(pk::encode("大哥，\n仅只这一回我赞同\x1b[1x张飞\x1b[0x所说的话。\n\n\x1b[1x诸葛亮\x1b[0x想必是因为害怕虚名被戳穿，\n所以刻意不露面吧。\n可别被蒙骗了。"), 关羽);
			pk::message_box(pk::encode("是啊、是啊！\n二哥，再继续说下去啊！"), 张飞);
			pk::message_box(pk::encode("\x1b[1x关羽\x1b[0x，怎么连你也和\x1b[1x张飞\x1b[0x一般见识呢？\n应该要向\x1b[2x周\x1b[0x的\x1b[1x文王\x1b[0x看齐啊。\n\n俗话说事不过三……\n就算是仍旧见不着，\n五次、十次我还是会去的。"), 刘备);
			pk::message_box(pk::encode("嘿，有没有搞错啊？\n不是说有二就有三吗？\n蠢死啦！！俺是绝对不会去的啦！"), 张飞);
			pk::cutin(CG_眼神);
			pk::message_box(pk::encode("\x1b[1x张飞\x1b[0x，给我安份一点！"), 刘备);
			pk::message_box(pk::encode("……算了！\n\x1b[1x关羽\x1b[0x，\n就丢下\x1b[1x张飞\x1b[0x出发了！！"), 刘备);
			pk::message_box(pk::encode("是！"), 关羽);
			pk::message_box(pk::encode("呜哇！！\n二哥，怎么可以这样啊…！\n\n……不，不对不对不对不对，\n这可就正中你的下怀了！\n俺才不去咧！"), 张飞);
			
			pk::fade(0);
			pk::fade(255);
			//音效
			pk::play_sfx(12);
			pk::background(场景_宫廷2);
			pk::message_box(pk::encode("………………………\n…………真的把我丢下了！\n呜哇，真可恶！"), 张飞);
			pk::background(-1);


			pk::point pos0 = pk::point(75, 113);
			pk::move_screen(pos0);
			//换bgm
			pk::play_bgm(18);
			pk::fade(0);
			pk::background(-1);
			pk::background(场景_草庐);
			pk::fade(255);
			auto sfx = pk::play_sfx_loop(614);

			
			
			//bgm鸟叫声
			pk::message_box(pk::encode("大哥们，太慢了啦！！"), 张飞);
			pk::message_box(pk::encode("哈哈哈，\n真没想到他会绕过我们先跑过来了。"), 关羽);
			pk::message_box(pk::encode("真是没辙……那么，嗯，\n我是\x1b[2x新野\x1b[0x的\x1b[1x刘备\x1b[0x。\n请问\x1b[1x诸葛\x1b[0x先生是否在家呢？"), 刘备);

			pk::message_box(pk::encode("嗯，今天在家喔！\n不过他好像正在睡午觉呢，\n要叫他起来吗？"), 童子);
			pk::cutin(7);
			//cg草庐等待
			//音效
			pk::message_box(pk::encode("不不，不碍事的。\n就让我等到他睡醒吧。"), 刘备);
			pk::fade(0);
			pk::sleep();
			pk::fade(255);
			pk::message_box(pk::encode("那个家伙！打算让咱们等到什么时候啊。\n让大哥站在外面等他，\n自己却在里面睡得那么舒坦！！\n可恶！！\n俺要绕到房子后面放火！！"), 张飞);
			pk::message_box(pk::encode("安静点，\x1b[1x张飞\x1b[0x！\n大哥和我不也是在忍耐着吗！"), 关羽);
			pk::cutin(CG_眼神);
			pk::message_box(pk::encode("…………哈啊啊啊啊，\n大梦谁先觉，平生我自知。\n草堂春睡足，窗外日迟迟。\n是谁来了吗？"), 诸葛亮);
			pk::message_box(pk::encode("\x1b[1x刘备\x1b[0x大人自刚才\n就在外面等候着了。"), 童子);
			pk::message_box(pk::encode("原来如此啊。\n嗯，我立刻见他。"), 诸葛亮);
			//换bgm
			pk::stop_sfx_loop(sfx);
			pk::play_bgm(17);
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
			
			pk::background(场景_自室);
			pk::message_box(pk::encode("初次见面。\n我名为\x1b[1x刘备\x1b[0x，字是\x1b[1x玄德\x1b[0x。\n总算能和先生见到面了。"), 刘备);
			pk::message_box(pk::encode("我才是应该为屡次的失礼致歉呢。\n前些日子，已拜读了您留下的书信。\n\n\x1b[1x刘备\x1b[0x大人为民着想之心，\n令我大为感佩。"), 诸葛亮);
			pk::message_box(pk::encode("喔喔，那么，先生！\n我是否能够借助您的智慧呢？"), 刘备);
			pk::message_box(pk::encode("只不过，我还太年轻又无才干，\n想必很难响应您的期待。"), 诸葛亮);
			pk::cutin(CG_眼神);
			pk::message_box(pk::encode("没有必要那么地谦虚。\n像先生如此有才华的人，\n现在若不挺身而出，要等待至何时呢！\n……我实为无能，\n至今仍无法解救众多的臣民，\n不过是存活下来罢了。\n不过，即使是不才的我，\n只要能够借助到先生的力量，\n今后必定能够拯救人民。"), 刘备);
			pk::message_box(pk::encode("…………我明白了。\n那么，虽然是很愚昧的想法，\n还请让我来陈述一下我的想法吧。"), 诸葛亮);
			//still 诸葛向刘备介绍
			pk::cutin(8);
			pk::message_box(pk::encode("造三分天下的鼎足情势，\n进而完成天下统一的大业……\n这就是\x1b[1x诸葛亮\x1b[0x的「三分天下之计」。"));
			pk::message_box(pk::encode("听了先生的高见，\n就彷佛是见到云间的青空般的心境。\n就请您将力量借助给我吧！"), 刘备);
			pk::message_box(pk::encode("……我明白了。\n那么，我愿尽犬马之劳，\n为国事全力以赴。"), 诸葛亮);
			if (黄月英.mibun == 身份_在野 or 黄月英.mibun == 身份_未发现) pk::message_box(pk::encode("\x1b[1x孔明\x1b[0x，且慢。\n我也随相公去吧。"), pk::get_person(武将_黄月英));
			if (诸葛均.mibun == 身份_在野 or 诸葛均.mibun == 身份_未发现) pk::message_box(pk::encode("兄长，\n我也要同行。"), 诸葛均);
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
			pk::move_screen(刘备.get_pos());
			pk::message_box(pk::encode(pk::format("{}年{}月，\n由于「三顾茅庐」的重邀，\n使\x1b[1x刘备\x1b[0x得以迎入\x1b[1x诸葛亮\x1b[0x为军师。\n卧龙终于现身于天下，\n结束漫长的雌伏，\n晚成的大器开始步上通往天下的大道。", pk::get_year(), pk::get_month())));
			//音效
			pk::play_sfx(612);
			pk::message_box(pk::encode("真的是……\n可喜可贺，可喜可贺啊！"), 张飞);

			//三顾后的处理
			//81是新剧本新野的id
			pk::force@ force0 = pk::get_force(刘备.get_force_id());
			pk::district@ district0 = pk::get_district(刘备.get_district_id());
			pk::set_service(诸葛亮, 81);
			pk::set_force(诸葛亮, force0, null, 0);
			pk::set_district(诸葛亮, district0.get_id());
			诸葛亮.location = 81;
			诸葛亮.loyalty = 255;
			诸葛亮.kouseki = 12000;
			诸葛亮.absence_timer = 0;			
			force0.gunshi = 武将_诸葛亮;
			if (黄月英.mibun == 身份_在野 or 黄月英.mibun == 身份_未发现)
			{
				pk::set_service(黄月英, 81);
				pk::set_force(黄月英, force0, null, 0);
				pk::set_district(黄月英, district0.get_id());
				黄月英.location = 81;
				黄月英.loyalty = 255;
				黄月英.kouseki = 2000;
			}

			if (诸葛均.mibun == 身份_在野 or 诸葛均.mibun == 身份_未发现)
			{
				pk::set_service(诸葛均, 81);
				pk::set_force(诸葛均, force0, null, 0);
				pk::set_district(诸葛均, district0.get_id());
				诸葛均.location = 81;
				诸葛均.loyalty = 100;
				诸葛均.kouseki = 500;
			}

			//force_t.sangu_3 = true;
			setting_ex.eve_happened[42] = true;

		} // void event_三顾()


	} // class 

	Main main;
}
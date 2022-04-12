// ## 2021/12/03 # 江东新风 # 修复行动力不增长问题（未设置军团长） ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/01 # 江东新风 # 文字越界错误修复 ##
// ## 2021/09/29 # 江东新风 # 触发时间判断错误修正 ##
// ## 2021/08/20 # 江东新风 # 关掉自动触发，改成菜单触发 ##
// ## 2021/02/15 # 江东新风 # 剧情异常触发的修复 ##
// ## 2021/01/30 # 江东新风 # 音效及动画完善 ##
// ## 2020/12/15 # 江东新风 # 修复对话称呼错误，修复非相同势力归附后武将任务异常 ##
// ## 2020/12/14 # 江东新风 # 剧情发生条件排除了陶谦是玩家而刘备不是玩家的情况 ##
// ## 2020/12/12 # 江东新风 # 新地图的三让徐州剧情 ##
/*
目前看来只有target[0]决定任务目标
//开发命令对应target[0]，建筑id。技巧研究对象建筑，亲善对象是势力，仲介无目标
*/

namespace 事件_三让徐州
{


	// ================ CUSTOMIZE ================
	const bool 事件剧本限定 = true;//默认限定剧本，限定2号剧本群雄割据可用，如不限定剧本，则可能在非对应剧本满足条件也会触发，但是会增加事件菜单的占用，以及可能会有未知bug
	// ===========================================


	class Main
	{
		bool flag_oneforce = false;
		pk::person@ 陶谦;
		pk::person@ 刘备;
		pk::person@ 张飞;
		pk::person@ 糜竺;
		pk::building@ taoqian_building;
		pk::building@ liubei_building;
		pk::force@ taoqian_force;
		pk::force@ liubei_force;

		pk::building@ building_;
		pk::force@ force_;
		pk::person@ kunshu_;
		pk::city@ city_;

		string info_事件信息;

		bool show_info = false;
		Main()
		{
			//pk::bind(pk::EVE_NEW_DAY, pk::trigger107_t(on_new_day));
			add_menu_让徐州();
			pk::bind(120, pk::trigger120_t(func_信息显示_事件));//试试通过按钮来显示相关信息
		}
		
		void func_信息显示_事件()
		{
<<<<<<< HEAD
=======
			/*
			int widget_id = pk::get_focused_widget_id();
			//pk::trace("widget_id" + widget_id);
			if (widget_id == 6293)
			{
				pk::trace("widget_id" + widget_id);
				pk::point rightup = pk::point(int(pk::get_resolution().width) - 330, 75 + (11) * 20);
				pk::point leftdown = pk::point(int(pk::get_resolution().width) - 330 + 32, 75 + (11) * 20 + 40);
				auto rect = pk::rectangle(rightup, leftdown);
				pk::draw_face(FACE_SMALL, 武将_诸葛亮, rect, FACE_R);
			}*/
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			if (!show_info) return;


			int line = 16;
			int middle = int(pk::get_resolution().width) / 2;
			int left = middle - 250;
			int right = middle + 250;
			int top = 5;
			int top_start = top + 100;
			pk::point leftdown = pk::point(left + 500, top_start + 28 * line);
			pk::draw_filled_rect(pk::rectangle(pk::point(left + 5, top_start), leftdown), ((0xff / 2) << 24) | 0x777777);

			pk::draw_text(info_事件信息, pk::point(left + 10, top_start), 0xffffffff, FONT_BIG, 0xff000000);

		}

		//------------------------------添加菜单---------------------------------------------//

		void add_menu_让徐州()
		{
			pk::menu_item item_让徐州;
			item_让徐州.menu = global_menu::菜单_事件;
			item_让徐州.init = pk::building_menu_item_init_t(init_让徐州);
			item_让徐州.is_enabled = pk::menu_item_is_enabled_t(isEnabled_让徐州);
			item_让徐州.is_visible = pk::menu_item_is_visible_t(isVisible_让徐州);
			item_让徐州.get_text = pk::menu_item_get_text_t(getText_让徐州);
			item_让徐州.get_desc = pk::menu_item_get_desc_t(getDesc_让徐州);
			item_让徐州.handler = pk::menu_item_handler_t(handler_让徐州);
			pk::add_menu_item(item_让徐州);
		}

		//------------------------------让徐州---------------------------------------------//
		void init_让徐州(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			//@taishu_ = pk::get_person(pk::get_taishu_id(building));
			@kunshu_ = pk::get_person(force_.kunshu);
			@city_ = pk::building_to_city(building);
			@ 陶谦 = pk::get_person(武将_陶谦);
			@ 刘备 = pk::get_person(武将_刘备);
			@ 张飞 = pk::get_person(武将_张飞);
			@taoqian_force = pk::get_force(陶谦.get_force_id());
			@liubei_force = pk::get_force(刘备.get_force_id());
		}

		string getText_让徐州()
		{
			return pk::encode("三让徐州");  //quchu teji
		}

		bool isEnabled_让徐州()
		{
			return true;
		}

		bool isVisible_让徐州()
		{
			forceinfo@ forcei = @force_ex[force_.get_id()];
			if (forcei.xuzhou) return false;
			if (pk::get_scenario().no != 2 and 事件剧本限定) return false;//注意如果改过剧本，那这里需要改动
			return true;
		}

		string getDesc_让徐州()
		{
			return pk::encode("执行三让徐州事件.");

		}

		bool handler_让徐州()
		{
			bool confirm = pk::yes_no(pk::encode("是否查看\x1b[2x三让徐州\x1b[0x事件条件?"));
			if (confirm)
			{
				bool start_event = false;
				if (刘备.get_force_id() == 陶谦.get_force_id())
				{
					info_事件信息 = 获取任务条件_相同势力(); 
					show_info = true;
<<<<<<< HEAD
					if (true)//check_all_同势力())
=======
					if (check_all_同势力())
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					{
						start_event = pk::yes_no(pk::encode("满足事件条件，是否执行\x1b[2x三让徐州\x1b[0x?"));
						if (start_event)
						{
							show_info = false;
							flag_oneforce = true;
							pk::scene(pk::scene_t(event));
							flag_oneforce = false;
							after_event_1();
						}
					}
					else
					{
						pk::message_box(pk::encode("\x1b[2x三让徐州\x1b[0x条件不满足，事件无法执行。"));
					}

				}
				else
				{
					info_事件信息 = 获取任务条件();
					show_info = true;
					if (check_all())
					{
						start_event = pk::yes_no(pk::encode("满足事件条件，是否执行\x1b[2x三让徐州\x1b[0x?"));
						if (start_event)
						{
							show_info = false;
							@ taoqian_force = pk::get_force(陶谦.get_force_id());
							@ liubei_force = pk::get_force(刘备.get_force_id());
							flag_oneforce = false;
							pk::scene(pk::scene_t(event));
							after_event_2();
						}
					}
					else
					{
						pk::message_box(pk::encode("\x1b[2x三让徐州\x1b[0x条件不满足，事件无法执行。"));
					}					
				}
			}
			show_info = false;
			return true;

		}

		string 获取任务条件()
		{
			//！！！可以根据刘备是否属于陶谦势力分两种情况？

			string info_事件信息_t = pk::encode("      \x1b[1x三让徐州触发条件\x1b[0x\n");
			string info_条件0 = (条件0_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("日期迟于193年7月上旬\x1b[0x");
			string info_条件1 = (条件1_check() ? pk::encode("\n\x1b[2x√"): pk::encode("\n\x1b[29x×")) + pk::encode("经过回合数大于18\x1b[0x");
			string info_条件2 = (条件2_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("势力数大于3\x1b[0x");
			string info_条件3 = (条件3_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备的身份是君主\x1b[0x");
			string info_条件4 = (条件4_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("张飞的所属势力是刘备军\x1b[0x");
			string info_条件5 = (条件5_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备军的统治城市数小于2\x1b[0x");
			string info_条件6 = (条件6_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("陶谦的身份是君主\x1b[0x");
			string info_条件7 = (条件7_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("陶谦军不是己方势力\x1b[0x");
			string info_条件8 = (条件8_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("下邳的所属势力是陶谦军\x1b[0x");
			string info_条件9 = (条件9_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("陶谦军的统治城市数小于2\x1b[0x");
			string info_条件10 = (条件10_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备军与陶谦军有相邻的统治城市\x1b[0x");
			string info_条件11 = (条件11_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备军与陶谦军处于同盟状态\x1b[0x");
			string info_条件12 = (条件12_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备军与陶谦军的关系大于友好\x1b[0x");
			string info_条件13 = (条件13_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备、张飞、陶谦可参加事件\x1b[0x");
			info_事件信息_t = info_事件信息_t + info_条件0 + info_条件1 + info_条件2 + info_条件3 + info_条件4 + info_条件5 + info_条件6 + info_条件7 + info_条件8 + info_条件9 + info_条件10 + info_条件11 + info_条件12 + info_条件13;
			return info_事件信息_t;
		}

		string 获取任务条件_相同势力()
		{
			//！！！可以根据刘备是否属于陶谦势力分两种情况？

			string info_事件信息_t = pk::encode("   \x1b[1x三让徐州(同势力)触发条件\x1b[0x\n");
			string info_条件0 = (条件0_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("日期迟于193年7月上旬\x1b[0x");
			string info_条件1 = (条件1_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("经过回合数大于18\x1b[0x");
			string info_条件2 = (条件2_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("势力数大于3\x1b[0x");
			//string info_条件3 = (条件3_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备的身份是君主\x1b[0x");//改成刘备陶谦同属一势力
			//string info_条件4 = (条件4_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("张飞的所属势力是刘备军\x1b[0x");
			//string info_条件5 = (条件5_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备军的统治城市数小于2\x1b[0x");
			string info_条件6 = (条件6_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("陶谦的身份是君主\x1b[0x");
			//string info_条件7 = (条件7_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("陶谦军不是己方势力\x1b[0x");
			string info_条件8 = (条件8_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("下邳的所属势力是陶谦军\x1b[0x");
			string info_条件9 = (条件9_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("陶谦军的统治城市数小于2\x1b[0x");
			//string info_条件10 = (条件10_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备军与陶谦军有相邻的统治城市\x1b[0x");
			//string info_条件11 = (条件11_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备军与陶谦军处于同盟状态\x1b[0x");
			//string info_条件12 = (条件12_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备军与陶谦军的关系大于友好\x1b[0x");
			string info_条件13 = (条件13_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备、张飞、陶谦可参加事件\x1b[0x");
			string info_条件14 = (条件14_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备、张飞、陶谦所属势力相同\x1b[0x");//
			info_事件信息_t = info_事件信息_t + info_条件0 + info_条件1 + info_条件2 + info_条件6 + info_条件8 + info_条件9 + info_条件13 + info_条件14;
			return info_事件信息_t;
		}

		bool check_all()
		{
			if (条件0_check() and 条件1_check() and 条件2_check() and 条件3_check() and 条件4_check() and 条件5_check() and 条件6_check() and 条件7_check() and 条件8_check() and 条件9_check() and 条件10_check() and 条件11_check() and 条件12_check() and 条件13_check())
				return true;
			return false;
		}

		bool check_all_同势力()
		{
			if (条件0_check() and 条件1_check() and 条件2_check() and 条件4_check() and 条件6_check() and 条件8_check() and 条件9_check() and 条件13_check())
				return true;
			return false;
		}

		bool 条件0_check()
		{
			//pk::scenario @ scenario0 = pk::get_scenario();
			//int start_year = scenario0.start_year;
			//int start_month = scenario0.start_month;
			int current_year = pk::get_year();
			int current_month = pk::get_month();
			if ((current_year == 193 and current_month > 7) or (current_year > 193)) return true;
			return false;
		}

		bool 条件1_check()
		{
			if (pk::get_elapsed_days() > 18) return true;
			return false;
		}

		bool 条件2_check()
		{
			if (pk::get_force_list().count >= 3) return true;
			return false;
		}

		bool 条件3_check()
		{
			if (pk::get_person(武将_刘备).mibun == 身份_君主) return true;
			return false;
		}

		bool 条件4_check()
		{
			if (pk::get_person(武将_张飞).get_force_id() == pk::get_person(武将_刘备).get_force_id()) return true;
			return false;
		}

		bool 条件5_check()
		{
			if (pk::is_valid_force_id(刘备.get_force_id()) and (pk::get_city_count(liubei_force) <= 2)) return true;
			return false;
		}

		bool 条件6_check()
		{
			if (pk::get_person(武将_陶谦).mibun == 身份_君主) return true;
			return false;
		}

		bool 条件7_check()
		{
			if (陶谦.get_force_id() != force_.get_id()) return true;
			return false;
		}

		bool 条件8_check()
		{
			if (陶谦.get_force_id() == pk::get_building(城市_下邳).get_force_id()) return true;
			return false;
		}

		bool 条件9_check()
		{
			if (pk::is_valid_force_id(陶谦.get_force_id()) and (pk::get_city_count(taoqian_force) <= 2)) return true;
			return false;
		}

		bool 条件10_check()
		{
			if (pk::is_valid_force_id(陶谦.get_force_id()) and pk::is_valid_force_id(刘备.get_force_id()) and pk::is_neighbor_force(liubei_force, taoqian_force)) return true;
			return false;
		}

		bool 条件11_check()
		{
			if (pk::is_valid_force_id(陶谦.get_force_id()) and pk::is_valid_force_id(刘备.get_force_id()) and (taoqian_force.ally[liubei_force.get_id()])) return true;
			return false;
		}

		bool 条件12_check()
		{
			if (pk::is_valid_force_id(陶谦.get_force_id()) and pk::is_valid_force_id(刘备.get_force_id()) and (taoqian_force.relations[liubei_force.get_id()] > 50)) return true;
			return false;
		}

		bool 条件13_check()
		{
			if (can_event(陶谦) and can_event(刘备) and can_event(张飞)) return true;
			return false;
		}

		bool 条件14_check()
		{
			if ((pk::get_person(武将_张飞).get_force_id() == pk::get_person(武将_刘备).get_force_id()) and (pk::get_person(武将_陶谦).get_force_id() == pk::get_person(武将_刘备).get_force_id())) return true;
			return false;
		}

		bool can_event(pk::person@ person_t)
		{
			if (pk::is_alive(person_t) and !pk::is_absent(person_t) and !pk::is_unitize(person_t)) return true;
			return false;
		}

		void on_new_day()
		{

			//判断是否满足触发条件
			pk::scenario @ scenario0 = pk::get_scenario();
			int start_year = scenario0.start_year;
			int start_month = scenario0.start_month;
			if (start_year <= 193 or (start_year == 194 and start_month <= 12))
			{
				int current_year = pk::get_year();
				int current_month = pk::get_month();
				if (current_year == 194 and (current_month <= 12 and current_month >= 11))
				{
					@ 陶谦 = pk::get_person(武将_陶谦);
					@ 刘备 = pk::get_person(武将_刘备);
					@ 张飞 = pk::get_person(武将_张飞);
					@ 糜竺 = pk::get_person(武将_糜竺);
					@ taoqian_force = pk::get_force(陶谦.get_force_id());
					if (pk::is_alive(陶谦) and pk::is_alive(刘备) and pk::is_alive(张飞) and pk::is_alive(糜竺) and !(陶谦.is_player() and !刘备.is_player()))
					{
						if (!pk::is_absent(陶谦) and !pk::is_absent(刘备) and !pk::is_absent(张飞) and !pk::is_unitize(陶谦) and !pk::is_unitize(刘备) and !pk::is_unitize(张飞))
						if (陶谦.mibun == 身份_君主 and 陶谦.location < 据点_末 and 刘备.mibun != 身份_俘虏 and 刘备.mibun != 身份_在野)
						{
							if (刘备.get_force_id() == 张飞.get_force_id())
							{
								if (刘备.get_force_id() == 陶谦.get_force_id())
								{
									flag_oneforce = true;
									pk::scene(pk::scene_t(event));
									flag_oneforce = false;
									after_event_1();
								}
								else
								{
									@ taoqian_force = pk::get_force(陶谦.get_force_id());
									@ liubei_force = pk::get_force(刘备.get_force_id());
									//得判断友好度，是否邻近
									if (pk::is_neighbor_force(taoqian_force, liubei_force) and taoqian_force.relations[liubei_force.get_id()] >= 50)
									{
										flag_oneforce = false;
										pk::scene(pk::scene_t(event));
										after_event_2();
									}

								}
							}
						}
					}

				}
			}
		}

		void event()
		{
			@ 陶谦 = pk::get_person(武将_陶谦);
			@ 刘备 = pk::get_person(武将_刘备);
			@ 张飞 = pk::get_person(武将_张飞);
			@ 糜竺 = pk::get_person(武将_糜竺);
			pk::person@ 年轻人 = pk::get_person(武将_若者);
			pk::person@ 农民 = pk::get_person(武将_农民);
			pk::person@ 平民姑娘 = pk::get_person(武将_町娘);
			pk::person@ 老人 = pk::get_person(武将_老人);
			pk::person@ 男童 = pk::get_person(武将_童子);
			pk::person@ 女童 = pk::get_person(武将_童女);


			@ taoqian_building = pk::get_building(陶谦.service);
			@ liubei_building = pk::get_building(刘备.service);
			string building_name = pk::decode(pk::get_name(taoqian_building));
			string building_name2 = pk::decode(pk::get_name(liubei_building));
			string taoqian_name = pk::decode(pk::get_name(陶谦));
			string liubei_name = pk::decode(pk::get_name(刘备));
			string zhangfei_name = pk::decode(pk::get_name(张飞));
			pk::move_screen(taoqian_building.get_pos());

			pk::play_bgm(22);
			pk::message_box(pk::encode(pk::format("于此乱世正处极度混乱之际，\n\x1b[2x{}\x1b[0x的一国之君\x1b[1x{}\x1b[0x病倒，\n其病况可谓不知可否有明日般危笃。", building_name, taoqian_name)));

			pk::fade(0);
			pk::sleep();
			pk::fade(255);
			pk::background(场景_自室);
			pk::open_event_still(10);
			//pk::background(场景_君主死亡);
			pk::message_box(pk::encode("各位.....都已到了吗......\n看来我的余命，\n应该是不长了......"), 陶谦);
			pk::message_box(pk::encode("..........."), 糜竺);
			pk::message_box(pk::encode(pk::format("关于我的身后事……\n就凭我儿子们的力量，\n是无法于此乱世中劈荆斩棘的。\n因此……\n欲将我军托付\n\x1b[2x{}\x1b[0x的\x1b[1x{}\x1b[0x大人……\n各位认为呢？", building_name2, liubei_name)), 陶谦);
			pk::message_box(pk::encode(pk::format("真是英明果断啊！\n信义仁厚、志向远大的\x1b[1x{}\x1b[0x大人，\n势必可领导我军前进吧。", liubei_name)), 糜竺);
			pk::message_box(pk::encode(pk::format("阁下也是那么认为啊……\n这样的话，事情就好办了。\n即刻召唤\x1b[1x{}\x1b[0x大人过来……", liubei_name)), 陶谦);
			pk::fade(0);
			pk::sleep();
			pk::fade(255);
			pk::message_box(pk::encode(pk::format("自使者大人处得知病情，\n赶紧特此前来。\n\x1b[1x{}\x1b[0x大人，您还好吧？", taoqian_name)), 刘备);
			pk::message_box(pk::encode(pk::format("……恐怕就快不久人世了吧。\n其实，就是为了此事，\n而召唤\x1b[1x{}\x1b[0x大人来此的啊。", liubei_name)), 陶谦);
			pk::message_box(pk::encode("？？"), 刘备);
			pk::cutin(20);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，\n请你是否能在我死后，继续领导我军？\n若是能够将我的臣民们，\n托付于你这般秀逸的人才，\n那老朽我也可安心地撒手归西了啊。", liubei_name)), 陶谦);
			pk::message_box(pk::encode(pk::format("！！您千万别这么说啊！\n像我这般的平庸之辈，\n该如何才能继承\x1b[1x{}\x1b[0x大人的伟业呢？\n敬请您还是托付给适任的人才是啊。", taoqian_name)), 刘备);
			pk::message_box(pk::encode("你若是舍我军而去的话，\n我将会死不瞑目的。"), 陶谦);
			pk::message_box(pk::encode("不过，此等大事……\n真是抱歉，\n请容我暂且离开。"), 刘备);
			//场景切换
			pk::close_event_still();
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
			pk::background(场景_宫廷1);
			pk::message_box(pk::encode("喂、喂、大哥、大哥！！\n那么好的事情为啥不要！\n\n是他那边要让出来的，\n干干脆脆地继承过来不就得了嘛！"), 张飞);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x！\n你是想陷我于不义之地吗？", zhangfei_name)), 刘备);
			pk::message_box(pk::encode("啧！啧！\n真是拗不过大哥呐！"), 张飞);
			pk::fade(0);
			pk::sleep();
			pk::fade(255);
			//pk::background(场景_宫廷1);
			pk::play_sfx(12);//落神音效12
			pk::message_box(pk::encode(pk::format("禀报大人。\n\x1b[1x{}\x1b[0x大人已经过世了。", taoqian_name)), 糜竺);
			pk::message_box(pk::encode("！！啊……\n受人们爱戴的君主过世了啊……"), 刘备);
			//请求
			pk::open_event_still(21);
			pk::play_sfx(610);
			//pk::background(场景_命令);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人！！\n容在下代替已过世的\x1b[1x{}\x1b[0x大人请求您。\n\n请您无论如何一定要继承\n\x1b[1x{}\x1b[0x大人的遗志，领导我军。\n唯有那么做才是最佳的供养啊。", liubei_name, taoqian_name, taoqian_name)), 糜竺);
			pk::message_box(pk::encode("你们的心情我很了解。\n不过，恕我无法接受。\n……告辞了。"), 刘备);
			pk::close_event_still();
			pk::fade(0);
			pk::sleep();
			auto sfx = pk::play_sfx_loop(615);
			pk::background(-1);
			pk::background(场景_市场);
			pk::fade(255);
			//场景切换市场，脚步，播放市场音效 落神音效615
			
			pk::cutin(CG_脚步);
			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，请您留步！", liubei_name)), 年轻人);
			pk::message_box(pk::encode("请求您。\n求您千万别弃我们而去。"), 农民);
			pk::message_box(pk::encode("（……实在办不到……）"), 刘备);
			//pk::background(场景_市场);
			pk::cutin(CG_脚步);
			pk::message_box(pk::encode(pk::format("我等众人诚心希望，\n能够由像\x1b[1x{}\x1b[0x大人般的人物，\n来继承\x1b[1x{}\x1b[0x大人的地位。", liubei_name, taoqian_name)), 平民姑娘);
			pk::message_box(pk::encode("我等众人虽为愚民，\n但仍能够分办得出，\n谁才是真正的明君啊。"), 老人);
			pk::message_box(pk::encode("（……毕竟还是办不到……）"), 刘备);
			//pk::background(场景_市场);
			pk::cutin(CG_脚步);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人！请求您！", liubei_name)), 男童);
			pk::message_box(pk::encode("求求您！"), 女童);
			pk::message_box(pk::encode("（……就这事……办不到……）"), 刘备);
			pk::stop_sfx_loop(sfx);
			pk::cutin(CG_眼神);
			pk::message_box(pk::encode("基于大义名份，\n弃这些臣民们于不顾之事，\n绝对办不到！"), 刘备);
			pk::play_bgm(17);
			pk::sleep();
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
			pk::message_box(pk::encode(pk::format("诸位，请听我说！\n从现在开始，\x1b[1x{}\x1b[0x大人的军队，\n将由我\x1b[1x{}\x1b[0x负责领导！\n虽为学艺不精之人，\n但立誓必将全力以赴，保卫诸位！", taoqian_name, liubei_name)), 刘备);
			pk::cutin(CG_气势);//21
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
			pk::message_box(pk::encode(pk::format("{}年{}月，\n继承\x1b[1x{}\x1b[0x遗志之\x1b[1x{}\x1b[0x，\n合并\x1b[2x{}军\x1b[0x，并继承其势力。\n正当群雄们以武力互争领土之时，\n受臣民拥戴，而扩展势力之\x1b[2x{}军\x1b[0x……\n其德望逐日高涨。", pk::get_year(), pk::get_month(), taoqian_name, liubei_name, taoqian_name, liubei_name)));
			//pk::background(场景_灭亡);
			//落神音效612
			//pk::message_box(pk::encode(pk::format("{}年{}月，\x1b[2x{}军\x1b[0x灭亡了，湮灭于历史的长河中。", pk::get_year(), pk::get_month(), taoqian_name)));





			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		} // void event()

		//同属一个势力的情况
		void after_event_1()
		{
			@ 陶谦 = pk::get_person(武将_陶谦);
			@ 刘备 = pk::get_person(武将_刘备);
			@ taoqian_force = pk::get_force(陶谦.get_force_id());
			//taoqian_force.kunshu = 武将_刘备;
			taoqian_force.color = 1;
			taoqian_force.update();
			//陶谦.mibun = 身份_一般;
			//陶谦.update();
			//刘备.mibun = 身份_君主;
			//刘备.update();

			//if (刘备.location < 据点_末)
			//	pk::set_taishu(liubei_building, 武将_刘备);

			
			//仇视问题改动
			//pk::kill(陶谦);
			pk::kill(陶谦, null, taoqian_building, 刘备,2);
			set_loyalty_kouseki(taoqian_force);
			pk::reset_taishu(taoqian_building);
			pk::reset_taishu(liubei_building);
			//pk::kill(陶谦,null,taoqian_building,刘备,0);

		}
		//不属于一个势力的情况
		void after_event_2()
		{
			@ 陶谦 = pk::get_person(武将_陶谦);
			@ 刘备 = pk::get_person(武将_刘备);
			@ taoqian_force = pk::get_force(陶谦.get_force_id());
			@ liubei_force = pk::get_force(刘备.get_force_id());
			//taoqian_force.kunshu = 武将_刘备;
			//taoqian_force.update();
			//此处存在叛乱事件的问题，直接换归属：删除势力，城市和设施换归属，武将换归属
			//原版就是kill，所以参考原版好了
			//势力_依附(liubei_force, taoqian_force);
			pk::kill(陶谦, null, null, null, 2);
			pk::kill(taoqian_force, true, liubei_force);
			set_loyalty_kouseki(liubei_force);
			//pk::kill(陶谦);


			pk::reset_taishu(taoqian_building);
			pk::reset_taishu(liubei_building);

		}

		void set_loyalty_kouseki(pk::force@ force_)
		{
			//pk::force@ force_ = pk::get_force(陶谦.get_force_id());
			// 군주의 성향에 따른 무장 충성도 변화
			auto person_list = pk::get_person_list(force_, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般));

			if (0 < person_list.count)
			{
				for (int j = 0; j < int(person_list.count); j++)
				{
					pk::person@ person_0 = person_list[j];
					//糜竺，陈珪，孙乾，功绩+8000，忠诚100  陈登，糜氏，糜芳，功绩+5000，忠诚100
					if (person_0.get_id() == 武将_糜竺 or person_0.get_id() == 武将_孙乾 or person_0.get_id() == 武将_陈珪)
					{
						if (person_0.loyalty < 85)
							person_0.loyalty == 100;
						else pk::add_loyalty(person_0, 15);
						pk::add_kouseki(person_0, 8000);
					}
					else if (person_0.get_id() == 武将_陈登 or person_0.get_id() == 武将_糜氏 or person_0.get_id() == 武将_糜芳)
					{
						if (person_0.loyalty < 90)
							person_0.loyalty == 100;
						else pk::add_loyalty(person_0, 10);
						pk::add_kouseki(person_0, 5000);
					}
					else pk::add_kouseki(person_0, 5000);

				} // for

			} // if 

		}

		void 势力_依附(pk::force@ force0/*接受方*/, pk::force@ force1/*依附方*/)
		{
			int force0_id = force0.get_id();
			int force0_district = pk::get_person(force0.kunshu).district;
			int force1_id = force1.get_id();

			auto building_list = pk::get_building_list();
			if (0 < building_list.count)
			{
				for (int j = 0; j < int(building_list.count); j++)
				{
					pk::building@ building_0 = building_list[j];
					if (building_0.get_force_id() == force1_id)
					{
						if (building_0.facility > 2)
						{
							building_0.init_force = force0_id;
							building_0.update();
						}
						if (building_0.facility == 2)
						{
							pk::port@ port0 = pk::get_port(j - 52);
							//pk::trace(pk::format("port_id:{},district:{}", port0.get_id(), port0.district));
							port0.district = force0_district;
							port0.update();
						}
						if (building_0.facility == 1)
						{
							pk::gate@ gate0 = pk::get_gate(j - 42);
							gate0.district = force0_district;
							gate0.update();
						}
						if (building_0.facility == 0)
						{
							pk::city@ city0 = pk::get_city(j);
							city0.district = force0_district;
							city0.update();
						}
					}
				} // for
			}
			auto person_list = pk::get_person_list(force1, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般));
			if (0 < person_list.count)
			{
				for (int j = 0; j < int(person_list.count); j++)
				{
					pk::person@ person_0 = person_list[j];
					person_0.district = force0_district;
					if (person_0.mibun == 身份_都督)
					{
						person_0.mibun = 身份_一般;
					}
					if (person_0.order == 武将任务_亲善 or person_0.order == 武将任务_同盟 or person_0.order == 武将任务_摒弃 or person_0.order == 武将任务_停战协定 or person_0.order == 武将任务_劝降 or person_0.order == 武将任务_交换俘虏 or person_0.order == 武将任务_请求援军 or person_0.order == 武将任务_二虎竞食 or person_0.order == 武将任务_二虎竞食)
					{
						person_0.order = 武将任务_返回;
						person_0.target[0] = person_0.service;
					}
					person_0.update();
				} // for						
			} // if 				
			auto unit_list = pk::get_unit_list(force1);
			if (0 < unit_list.count)
			{
				for (int j = 0; j < int(unit_list.count); j++)
				{
					pk::unit@ unit_0 = unit_list[j];
					unit_0.update();
				} // for						
			} // if 			

		}

	} // class 

	Main main;
}
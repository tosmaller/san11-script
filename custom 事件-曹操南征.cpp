// ## 2021/11/03 # 江东新风 # 南征事件是否已发生判断 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/18 # 江东新风 # 新地图的曹操南征剧情 ##
/***CUSTOM
@name 事件-曹操南征
@class 历史事件；曹操南征
@author 江东新风
@contact 
@version 1.1
@date 2021/10/29
@desc 條件:
‧ 劇本開始後 30 日或以上
‧ 曹操君主勢力存在, 支配中原都市 7 個或以上
‧ 曹操君主勢力 除襄平以外 支配河北全部都市
‧ 曹操君主勢力 支配都市 包括 宛 和 許昌
‧ 曹操君主勢力 支配都市 不包括 新野, 廬江, 柴桑, 建業, 吳 和 會稽
‧ 襄陽 江陵 柴桑 建業 中至少一都市的支配勢力和曹操君主勢力不是同盟關係
‧ 曹操 不是在任務中
結果:
‧ 襄陽 江陵 柴桑 建業 中至少一都市的支配勢力和曹操君主勢力的友好度大幅下降
@notice 
***/


namespace 事件_曹操南征
{


	// ================ CUSTOMIZE ================
	const bool 事件剧本限定 = true;//默认限定剧本，限定2号剧本群雄割据可用，如不限定剧本，则可能在非对应剧本满足条件也会触发，但是会增加事件菜单的占用，以及可能会有未知bug
	// ===========================================


	class Main
	{
		pk::person@ 曹操;

		pk::building@ caocao_building;
		pk::force@ force_c;

		pk::building@ building_;
		pk::force@ force_;
		pk::city@ city_;

		string info_事件信息;

		bool show_info = false;
		Main()
		{
			//pk::bind(pk::EVE_NEW_DAY, pk::trigger107_t(on_new_day));
			add_menu_曹操南征();
			pk::bind(120, pk::trigger120_t(func_信息显示_事件));//试试通过按钮来显示相关信息
			pk::bind(102, -1, pk::trigger102_t(callback));//剧本初始化-读档时
		}
		void callback()
		{
			pk::set_eve(5,false);//关闭原本事件的曹操南征
		}

		void func_信息显示_事件()
		{
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

		void add_menu_曹操南征()
		{
			pk::menu_item item_曹操南征;
			item_曹操南征.menu = global_menu::菜单_事件;
			item_曹操南征.init = pk::building_menu_item_init_t(init_曹操南征);
			item_曹操南征.is_enabled = pk::menu_item_is_enabled_t(isEnabled_曹操南征);
			item_曹操南征.is_visible = pk::menu_item_is_visible_t(isVisible_曹操南征);
			item_曹操南征.get_text = pk::menu_item_get_text_t(getText_曹操南征);
			item_曹操南征.get_desc = pk::menu_item_get_desc_t(getDesc_曹操南征);
			item_曹操南征.handler = pk::menu_item_handler_t(handler_曹操南征);
			pk::add_menu_item(item_曹操南征);
		}

		//------------------------------让徐州---------------------------------------------//
		void init_曹操南征(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			//@taishu_ = pk::get_person(pk::get_taishu_id(building));
			@city_ = pk::building_to_city(building);
			@ 曹操 = pk::get_person(武将_曹操);
			@force_c = pk::get_force(曹操.get_force_id());
		}

		string getText_曹操南征()
		{
			return pk::encode("曹操南征");  //quchu teji
		}

		bool isEnabled_曹操南征()
		{
			if (setting_ex.eve_happened[5]) return false;
			return true;
		}

		bool isVisible_曹操南征()
		{
			//if (setting_ex.eve_happened[5]) return false;//注意如果改过剧本，那这里需要改动
			return true;
		}

		string getDesc_曹操南征()
		{
			if (setting_ex.eve_happened[5]) return pk::encode("曹操南征已发生.");
			return pk::encode("执行曹操南征事件.");

		}

		bool handler_曹操南征()
		{
			bool confirm = pk::yes_no(pk::encode("是否查看\x1b[2x曹操南征\x1b[0x事件条件?"));
			if (confirm)
			{
				bool start_event = false;
				info_事件信息 = 获取任务条件();
				show_info = true;
				if (check_all())
				{
					start_event = pk::yes_no(pk::encode("满足事件条件，是否执行\x1b[2x曹操南征\x1b[0x?"));
					if (start_event)
					{
						show_info = false;
						pk::scene(pk::scene_t(event_曹操南征));
						//after_event_2();
					}
				}
				else
				{
					pk::message_box(pk::encode("\x1b[2x曹操南征\x1b[0x条件不满足，事件无法执行。"));
				}
			}
			show_info = false;
			return true;

		}

		string 获取任务条件()
		{

			string info_事件信息_t = pk::encode("      \x1b[1x曹操南征触发条件\x1b[0x\n");
			string info_条件0 = (条件0_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("曹操身份为君主，且可执行剧情\x1b[0x");
			string info_条件1 = (条件1_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("经过回合数大于3\x1b[0x");
			string info_条件2 = (条件2_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("曹操君主势力存在, 支配中原都市7个或以上\x1b[0x");
			string info_条件3 = (条件3_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("曹操君主势力除襄平以外,支配河北全部都市\x1b[0x");
			string info_条件4 = (条件4_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("曹操君主势力支配都市 包括 宛 和 许昌\x1b[0x");
			string info_条件5 = (条件5_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("曹操君主势力支配都市 不包括 新野, 庐江, \n  柴桑, 建业, 吴 和 会稽\x1b[0x");
			string info_条件6 = (条件6_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("襄阳 江陵 柴桑 建业 中至少一都市的支配\n  势力和曹操君主势力不是同盟关系\x1b[0x");

			info_事件信息_t = info_事件信息_t + info_条件0 + info_条件1 + info_条件2 + info_条件3 + info_条件4 + info_条件5 + info_条件6;
			return info_事件信息_t;
		}

		bool check_all()
		{
			if (条件0_check() and 条件1_check() and 条件2_check() and 条件3_check() and 条件4_check() and 条件5_check() and 条件6_check())
				return true;
			return false;
		}

		bool 条件0_check()
		{
			if (曹操.mibun == 身份_君主 and can_event(曹操)) return true;
			return false;
		}

		bool 条件1_check()
		{
			if (pk::get_elapsed_days() > 30) return true;//剧本开始超过3旬
			return false;
		}

		bool 条件2_check()
		{
			// 曹操君主勢力存在, 支配中原都市 7 個或以上
			pk::list<pk::city@> src_citys = get_region_force_city(force_c.get_id(), 地方_中原);
			if (src_citys.count >= 7) return true;
			return false;
		}

		bool 条件3_check()
		{
			//曹操君主勢力 除襄平以外 支配河北全部都市
			pk::list<pk::city@> src_citys = get_region_force_city(force_c.get_id(), 地方_河北);
			pk::list<pk::city@> all_citys = pk::get_city_list(pk::get_region(地方_河北));

			if (src_citys.count == all_citys.count) return true;
			if (src_citys.count +1 == all_citys.count and pk::get_city(城市_襄平).get_force_id() != force_c.get_id()) return true;
			return false;
		}

		bool 条件4_check()
		{
			//曹操君主勢力 支配都市 包括 宛 和 許昌
			if (pk::get_city(城市_许昌).get_force_id() == force_c.get_id() and pk::get_city(城市_宛).get_force_id() == force_c.get_id()) return true;
			return false;
		}

		bool 条件5_check()
		{
			//曹操君主勢力 支配都市 不包括 新野, 廬江, 柴桑, 建業, 吳 和 會稽
			if (pk::get_city(城市_庐江).get_force_id() == force_c.get_id()) return false;
			if (pk::get_city(城市_柴桑).get_force_id() == force_c.get_id()) return false;
			if (pk::get_city(城市_建业).get_force_id() == force_c.get_id()) return false;
			if (pk::get_city(城市_吴).get_force_id() == force_c.get_id()) return false;
			if (pk::get_city(城市_会稽).get_force_id() == force_c.get_id()) return false;
			if (pk::get_building(新据点_新野).get_force_id() == force_c.get_id()) return false;
			return true;
		}

		bool 条件6_check()
		{
			// 襄陽 江陵 柴桑 建業 中至少一都市的支配勢力和曹操君主勢力不是同盟關係,或停战关系
			int src_force_id = force_c.get_id();
			if (pk::is_enemy(pk::get_city(城市_襄阳).get_force_id(), src_force_id)) return true;
			if (pk::is_enemy(pk::get_city(城市_江陵).get_force_id(), src_force_id)) return true;
			if (pk::is_enemy(pk::get_city(城市_柴桑).get_force_id(), src_force_id)) return true;
			if (pk::is_enemy(pk::get_city(城市_建业).get_force_id(), src_force_id)) return true;
			return false;

		}

		bool can_event(pk::person@ person_t)
		{
			if (pk::is_alive(person_t) and !pk::is_absent(person_t) and !pk::is_unitize(person_t)) return true;
			return false;
		}

		pk::list<pk::city@> get_region_force_city(int force_id, int region_id)
		{
			pk::list<pk::city@>list_t = pk::get_city_list(pk::get_force(force_id));
			pk::list<pk::city@>temp;
			for (int i = 0; i < list_t.count; i++)
			{
				if (pk::get_province(list_t[i].province).region == region_id) temp.add(list_t[i]);
			}
			return temp;
		}


		void event_曹操南征()
		{
			setting_ex.eve_happened[5] = true;
			@ 曹操 = pk::get_person(武将_曹操);

			pk::person@ 武将 = pk::get_person(武将_武将);//743
			pk::person@ 卫兵 = pk::get_person(武将_卫兵);//744
			pk::person@ 士兵 = pk::get_person(武将_士兵);//716

			pk::play_bgm(17);
			pk::move_screen(曹操.get_pos());

			pk::message_box(pk::get_msg(pk::msg_param(15742, 曹操, 卫兵)), 曹操);
			pk::message_box(pk::get_msg(pk::msg_param(15743, 卫兵, 曹操)), 卫兵);
			pk::fade(0);
			pk::background(场景_宫廷1);
			pk::sleep();
			pk::fade(255);
			
			pk::message_box(pk::get_msg(pk::msg_param(15744, 曹操,null)), 曹操);
			pk::cutin(CG_眼神);
			pk::message_box(pk::get_msg(pk::msg_param(15745, 曹操, null)), 曹操);
			pk::cutin(CG_气势);
			pk::message_box(pk::get_msg(pk::msg_param(15746, 武将, null)), 武将);
			pk::message_box(pk::get_msg(pk::msg_param(15747, 卫兵, null)), 卫兵);
			pk::message_box(pk::get_msg(pk::msg_param(15748, 士兵, null)), 士兵);
			pk::message_box(pk::get_msg(pk::msg_param(15749, 曹操, null)), 曹操);

			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
			//pk::cutin(CG_气势);
			//pk::cutin(CG_气势);
			pk::open_event_still(24);
			pk::play_sfx(603);

			pk::msg_param msg_param(15750, null, null);
			@msg_param.person[2] = @曹操;
			pk::message_box(pk::get_msg(msg_param));

			pk::close_event_still();


			pk::array<int> dst_force_arr = get_dst_force_id();
			for (int i = 0; i < int(dst_force_arr.length); i++)
			{
				if (force_c.ally[dst_force_arr[i]]) pk::set_ally(force_c, dst_force_arr[i], false);
				else if (force_c.ceasefire_timer[dst_force_arr[i]]!=0) pk::set_ceasefire_timer(force_c, dst_force_arr[i], 0);
				else pk::set_relations(force_c, dst_force_arr[i], -10);
			}
			//襄陽 江陵 柴桑 建業 中至少一都市的支配勢力和曹操君主勢力的友好度大幅下降


		} // void event()

		pk::array<int> get_dst_force_id()
		{
			pk::array<int>arr_t;
			pk::array<int>arr_city_t = {城市_襄阳, 城市_江陵, 城市_柴桑, 城市_建业};
			for (int i = 0; i < int(arr_city_t.length); i++)
			{
				int force_id = pk::get_city(arr_city_t[i]).get_force_id();
				if (arr_t.find(force_id) < 0) arr_t.insertLast(force_id);
			}
			return arr_t;
		}


	} // class 

	Main main;
}
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/02/06 # 江东新风 # 邻近势力条件修改 ##
// ## 2021/11/01 # 江东新风 # 刘表之死剧情完成 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/18 # 江东新风 # 新地图的刘表之死剧情 ##
/***CUSTOM
@name 事件-刘表之死
@class 历史事件；刘表之死
@author 江东新风
@contact
@version 1.1
@date 2021/10/29
@desc 條件：
‧ 選207年劇本
‧ 玩家控制劉備/曹操
‧ 南征已發生
‧ 劉表支配襄陽江夏 (江夏非必要條件)
‧ 劉琦、劉琮及蔡瑁在劉表軍
‧ 劉備支配新野
‧ 諸葛亮在劉備軍
‧ 曹操健在
‧ 維持各勢力格局至208年8月21日
結果：
‧ 劉表死
‧ 劉琮繼位，其勢力與曹操合併，一部分劉表武將下野
‧ 劉琦領部分劉表武將於江夏獨立，其勢力與劉備軍合併，江陵變為空城
如江夏非劉表勢力，則劉琦會於江陵獨立且與劉備軍合併。
@notice
***/


namespace 事件_刘表之死
{


	// ================ CUSTOMIZE ================
	const bool 事件剧本限定 = true;//默认限定剧本，限定2号剧本群雄割据可用，如不限定剧本，则可能在非对应剧本满足条件也会触发，但是会增加事件菜单的占用，以及可能会有未知bug
	// ===========================================


	class Main
	{
		pk::person@ 曹操;
		pk::person@ 刘表;
		pk::person@ 刘备;
		pk::person@ 刘琦;
		pk::person@ 刘琮;
		pk::person@ 蔡瑁;
		pk::person@ 诸葛亮;

		pk::building@ caocao_building;
		pk::force@ force_liubiao;
		pk::force@ force_liubei;
		pk::force@ force_caocao; 

		pk::building@ building_;
		pk::force@ force_;
		pk::city@ city_;

		string info_事件信息;

		bool show_info = false;
		Main()
		{
			//pk::bind(pk::EVE_NEW_DAY, pk::trigger107_t(on_new_day));
			add_menu_刘表之死();
			pk::bind(120, pk::trigger120_t(func_信息显示_事件));//试试通过按钮来显示相关信息
			pk::bind(102, -1, pk::trigger102_t(callback));//剧本初始化-读档时
		}
		void callback()
		{
			pk::set_eve(18, false);//关闭原本事件的刘表之死
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
			pk::point leftdown = pk::point(left + 530, top_start + 28 * line);
			pk::draw_filled_rect(pk::rectangle(pk::point(left + 5, top_start), leftdown), ((0xff / 2) << 24) | 0x777777);

			pk::draw_text(info_事件信息, pk::point(left + 10, top_start), 0xffffffff, FONT_BIG, 0xff000000);

		}

		//------------------------------添加菜单---------------------------------------------//

		void add_menu_刘表之死()
		{
			pk::menu_item item_刘表之死;
			item_刘表之死.menu = global_menu::菜单_事件;
			item_刘表之死.init = pk::building_menu_item_init_t(init_刘表之死);
			item_刘表之死.is_enabled = pk::menu_item_is_enabled_t(isEnabled_刘表之死);
			item_刘表之死.is_visible = pk::menu_item_is_visible_t(isVisible_刘表之死);
			item_刘表之死.get_text = pk::menu_item_get_text_t(getText_刘表之死);
			item_刘表之死.get_desc = pk::menu_item_get_desc_t(getDesc_刘表之死);
			item_刘表之死.handler = pk::menu_item_handler_t(handler_刘表之死);
			pk::add_menu_item(item_刘表之死);
		}

		//------------------------------让徐州---------------------------------------------//
		void init_刘表之死(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			//@taishu_ = pk::get_person(pk::get_taishu_id(building));
			@city_ = pk::building_to_city(building);
			@ 曹操 = pk::get_person(武将_曹操);
			@ 刘表 = pk::get_person(武将_刘表);
			@ 刘备 = pk::get_person(武将_刘备);
			@ 刘琦 = pk::get_person(武将_刘琦);
			@ 刘琮 = pk::get_person(武将_刘琮);
			@ 蔡瑁 = pk::get_person(武将_蔡瑁);
			@ 诸葛亮 = pk::get_person(武将_诸葛亮);

			if (pk::is_alive(刘表) and 刘表.get_force_id() != -1) @force_liubiao = pk::get_force(刘表.get_force_id());
			if (pk::is_alive(刘备) and 刘备.get_force_id() != -1) @force_liubei = pk::get_force(刘备.get_force_id());
			if (pk::is_alive(曹操) and 曹操.get_force_id() != -1) @force_caocao = pk::get_force(曹操.get_force_id());
		}

		string getText_刘表之死()
		{
			return pk::encode("刘表之死");  //quchu teji
		}

		bool isEnabled_刘表之死()
		{
			return true;
		}

		bool isVisible_刘表之死()
		{
			if (setting_ex.eve_happened[18]) return false;//已发生后不显示
			return true;
		}

		string getDesc_刘表之死()
		{
			return pk::encode("执行刘表之死事件.");

		}

		bool handler_刘表之死()
		{
			bool confirm = pk::yes_no(pk::encode("是否查看\x1b[2x刘表之死\x1b[0x事件条件?"));
			if (confirm)
			{
				bool start_event = false;
				info_事件信息 = 获取任务条件();
				show_info = true;
				if (check_all())
				{
					start_event = pk::yes_no(pk::encode("满足事件条件，是否执行\x1b[2x刘表之死\x1b[0x?"));
					if (start_event)
					{
						show_info = false;
						pk::scene(pk::scene_t(event_刘表之死));
						//after_event_2();
					}
				}
				else
				{
					pk::message_box(pk::encode("\x1b[2x刘表之死\x1b[0x条件不满足，事件无法执行。"));
				}
			}
			show_info = false;
			return true;

		}

		string 获取任务条件()
		{
			/*
			劉表之死

條件：
‧ 選207年劇本
‧ 玩家控制劉備/曹操---
‧ 南征已發生
‧ 劉表支配襄陽江夏 (江夏非必要條件)
‧ 劉琦、劉琮及蔡瑁在劉表軍
‧ 劉備支配新野
‧ 諸葛亮在劉備軍
‧ 曹操健在
‧ 維持各勢力格局至208年8月21日
			*/
			string info_事件信息_t = pk::encode("      \x1b[1x刘表之死触发条件\x1b[0x\n");
			string info_条件0 = (条件0_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("曹操,刘备,刘表身份为君主\x1b[0x");
			string info_条件1 = (条件1_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("经过回合数大于18\x1b[0x");
			string info_条件2 = (条件2_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("曹操南征已经发生\x1b[0x");
			string info_条件3 = (条件3_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘表非玩家势力,刘表军支配城市大于1，小于7\x1b[0x");
			string info_条件4 = (条件4_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘琦，刘琮，蔡瑁所属势力为刘表军\x1b[0x");
			string info_条件5 = (条件5_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备军支配城市数量小于3\x1b[0x");
			string info_条件6 = (条件6_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("曹操军占有许昌和宛\x1b[0x");
			string info_条件7 = (条件7_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘表军刘备军相邻，处于停战或同盟状态，\n且均不与曹操停战或同盟\x1b[0x");

			//0-19险恶 20-39不和 40-60中立 61-80友好 81-100亲密
			info_事件信息_t = info_事件信息_t + info_条件0 + info_条件1 + info_条件2 + info_条件3 + info_条件4 + info_条件5 + info_条件6 + info_条件7;
			return info_事件信息_t;
		}

		bool check_all()
		{
			if (条件0_check() and 条件1_check() and 条件2_check() and 条件3_check() and 条件4_check() and 条件5_check() and 条件6_check() and 条件7_check())
				return true;
			return false;
		}


		bool 条件0_check()
		{
			if (曹操.mibun == 身份_君主 and 刘备.mibun == 身份_君主 and 刘表.mibun == 身份_君主) return true;
			return false;
		}

		bool 条件1_check()
		{
			if (pk::get_elapsed_days() >= 180) return true;//剧本开始超过18旬
			return false;
		}

		bool 条件2_check()
		{
			// 曹操南征已经发生
			if (setting_ex.eve_happened[5]) return true;
			return false;
		}

		bool 条件3_check()
		{
			//刘表非玩家势力,刘表军支配城市大于1，小于7
			pk::list<pk::city@> liubiao_citys = pk::get_city_list(force_liubiao);
			if (liubiao_citys.count < 7 and liubiao_citys.count > 1 and !force_liubiao.is_player()) return true;
			return false;
		}

		bool 条件4_check()
		{
			//刘琦，刘琮，蔡瑁所属势力为刘表军
			if (刘琦.get_force_id() == force_liubiao.get_id() and 刘琮.get_force_id() == force_liubiao.get_id() and 蔡瑁.get_force_id() == force_liubiao.get_id()) return true;
			return false;
		}

		bool 条件5_check()
		{
			//刘备军支配城市小于3
			pk::list<pk::city@> liubei_citys = pk::get_city_list(pk::get_force(刘备.get_force_id()));
			if (liubei_citys.count < 3) return true;
			return false;
		}

		bool 条件6_check()
		{
			// 曹操军占有许昌和宛
			if (pk::get_city(城市_许昌).get_force_id() != 曹操.get_force_id()) return false;
			if (pk::get_city(城市_宛).get_force_id() != 曹操.get_force_id()) return false;
			return true;

		}

		bool 条件7_check()
		{
			// 刘表军刘备军相邻，且均不与曹操停战或同盟

			if (!is_neighbor_force(force_liubiao, force_liubei)) return false;
			if (pk::is_enemy(force_liubei, force_liubiao)) return false;
			if (!pk::is_enemy(force_liubei, force_caocao)) return false;
			if (!pk::is_enemy(force_liubiao, force_caocao)) return false;
			return true;

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

		bool is_neighbor_force(pk::force@src, pk::force@dst)
		{
			int src_force_id = src.get_id();
			int dst_force_id = dst.get_id();
			pk::list<pk::building@> list;
			for (int i = 0; i < 据点_末; ++i)
			{
				pk::building@ base = pk::get_building(i);
				if (base.get_force_id() == src_force_id) list.add(base);
			}
			for (int i = 0; i < list.count; ++i)
			{
				for (int j = 0; j < 7; i++)
				{
					int neighbor_base_id = pk::get_neighbor_area_building(list[i].get_id(), j);
					if (pk::is_valid_base_id(neighbor_base_id))
					{
						pk::building@ dst_building = pk::get_building(neighbor_base_id);
						if (dst_building.get_force_id() == dst_force_id) return true;
					}
				}
			}
			return false;
		}

		bool is_neighbor_force(int src_id, int dst_id)
		{
			if (pk::is_neighbor_base(src_id, dst_id)) return true;
			if (pk::is_valid_city_id(src_id) and pk::is_valid_city_id(dst_id))
			{
				pk::city@ city = pk::get_city(src_id);
				for (int i = 0; i < 6; ++i)
				{
					if (city.neighbor[i] == dst_id) return true;
				}
				return false;
			}
			return false;
		}

		void event_刘表之死()
		{
			
			pk::person@ 文官 = pk::get_person(武将_文官);//715
			pk::person@ 使者 = pk::get_person(武将_使者);//742
			pk::person@ 武将 = pk::get_person(武将_武将);//743
			pk::person@ 卫兵 = pk::get_person(武将_卫兵);//744
			pk::force@ liubei_force = pk::get_force(刘备.get_force_id());
			pk::person@ 军师 = pk::get_person(liubei_force.gunshi);

			pk::msg_param msg_param(18519,null,null);
			@msg_param.person[2] = @刘表;
			@msg_param.person[3] = @刘备;
			@msg_param.person[4] = @曹操;
			@msg_param.person[5] = @刘琦;
			@msg_param.person[6] = @刘琮;
			@msg_param.person[7] = @蔡瑁;
			@msg_param.city[0] = @pk::get_city(刘表.service);

			pk::play_bgm(21);
			pk::move_screen(刘表.get_pos());


			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18519, null, null)));
			pk::fade(0);
			pk::background(场景_自室);
			pk::open_event_still(10);
			pk::sleep();
			pk::fade(255);


			//pk::message_box(pk::get_msg(18520, 刘表, null), 刘表);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18520, 刘表, null)), 刘表);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18521, 文官, 刘表)), 文官);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18522, 刘表, 文官)), 刘表);
			pk::cutin(CG_诏书);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18523, 刘表, 文官)), 刘表);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18524, 文官, 刘表)), 文官);
			pk::close_event_still();
			pk::fade(0);
			pk::play_bgm(20);
			pk::sleep();
			pk::fade(255);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18525, 文官, 蔡瑁)), 文官);
			pk::cutin(CG_诏书);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18526, 蔡瑁, 文官)), 蔡瑁);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18527, 文官, 蔡瑁)), 文官);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18528, 蔡瑁, 文官)), 蔡瑁);
			pk::cutin(CG_嘲笑);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18529, 蔡瑁, 文官)), 蔡瑁);
			pk::cutin(CG_一击);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18530, 文官, 蔡瑁)), 文官);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18531, 蔡瑁, 文官)), 蔡瑁);
			pk::fade(0);
			pk::background(场景_宫廷2);
			pk::play_bgm(16);
			pk::sleep();
			pk::fade(255);
			pk::cutin(CG_激昂);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18532, 蔡瑁, 刘琮)), 蔡瑁);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18533, 刘琮, 蔡瑁)), 刘琮);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18534, 蔡瑁, 刘琮)), 蔡瑁);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18535, 刘琮, 蔡瑁)), 刘琮);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18536, 蔡瑁, 刘琮)), 蔡瑁);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18537, 刘琮, 蔡瑁)), 刘琮);
			pk::fade(0);
			pk::background(-1);
			pk::sleep();
			pk::fade(255);
			pk::move_screen(曹操.get_pos());
			pk::fade(0);
			pk::background(场景_宫廷1);
			pk::open_event_still(20);
			pk::sleep();
			pk::fade(255);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18538, 使者, 曹操)), 使者);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18539, 曹操, 使者)), 曹操);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18540, 使者, 曹操)), 使者);
			pk::close_event_still();
			pk::fade(0);
			pk::sleep();
			pk::fade(255);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18541, 曹操, null)), 曹操);
			pk::fade(0);
			pk::background(-1);
			pk::sleep();
			pk::fade(255);
			pk::move_screen(刘备.get_pos());
			pk::fade(0);
			pk::background(场景_宫廷2);
			pk::fade(255);
			pk::cutin(CG_脚步);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18542, 卫兵, 刘备)), 卫兵);
			pk::play_sfx(613);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18543, 刘备, 卫兵)), 刘备);
			
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18544, 军师, 刘备)), 军师);
			pk::cutin(CG_激昂);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18545, 刘备, 军师)), 刘备);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18546, 军师, 刘备)), 军师);
			pk::cutin(CG_脚步);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18547, 武将, 刘备)), 武将);
			pk::fade(0);
			pk::sleep();
			pk::fade(255);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18548, 刘琦, 刘备)), 刘琦);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18549, 刘备, 刘琦)), 刘备);
			pk::play_sfx(610);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18550, 刘琦, 刘备)), 刘琦);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18551, 刘备, 刘琦)), 刘备);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18552, 刘琦, 刘备)), 刘琦);
			pk::fade(0);
			auto sfx = pk::play_sfx_loop(615);
			pk::background(场景_市场);
			pk::sleep();
			pk::fade(255);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18553, pk::get_person(748), 刘备)), pk::get_person(748));
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18554, pk::get_person(733), 刘备)), pk::get_person(733));
			pk::fade(0);
			pk::background(场景_野外);
			pk::sleep();
			pk::fade(255);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18555, pk::get_person(751), 刘备)), pk::get_person(751));
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18556, pk::get_person(752), 刘备)), pk::get_person(752));
			pk::fade(0);
			pk::background(场景_草庐);
			pk::sleep();
			pk::fade(255);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18557, pk::get_person(719), 刘备)), pk::get_person(719));
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18558, pk::get_person(753), 刘备)), pk::get_person(753));
			pk::fade(0);
			pk::stop_sfx_loop(sfx);
			pk::background(场景_宫廷2);
			pk::sleep();
			pk::fade(255);
			pk::play_bgm(17);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18559, 刘琦, 刘备)), 刘琦);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18560, 刘备, 刘琦)), 刘备);
			pk::cutin(CG_眼神);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18561, 刘备, null)), 刘备);
			pk::cutin(CG_气势);
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::open_event_still(24);			
			pk::fade(255);
			pk::play_sfx(603);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18562, null, null)));
			pk::close_event_still();
			//襄陽 江陵 柴桑 建業 中至少一都市的支配勢力和曹操君主勢力的友好度大幅下降
			after_event();

		} // void event()

		/*
‧ 劉表死
‧ 劉琮繼位，其勢力與曹操合併，一部分劉表武將下野 {城市_襄阳, 城市_江陵, 城市_武陵}
‧ 劉琦領部分劉表武將於江夏獨立，其勢力與劉備軍合併，江陵變為空城 {城市_江夏, 城市_江陵, 城市_长沙}
如江夏非劉表勢力，則劉琦會於江陵獨立且與劉備軍合併。
		*/
		void after_event()
		{
			setting_ex.eve_happened[18] = true;//表明事件已发生
			pk::set_district(刘琦, 刘备.get_force_id());
			pk::kill(刘表,null,null,刘琮);
			//城市处理
			//pk::list<pk::city@> city_list0;
			array<int>city_arr0 = { 城市_襄阳, 城市_江陵, 城市_武陵 };
			pk::city@ liucong_city;
			for (int i = 0; i< int(city_arr0.length()); i++)
			{
				pk::city@ city0 = pk::get_city(city_arr0[i]);
				if (city0.get_force_id() == force_liubiao.get_id())
				{
					@liucong_city = @city0;
					pk::set_service(刘琮, liucong_city.get_id());
					break;
				}
					
			}
			//部队处理放在此处，可以确保刘备的势力不会出错
			array<int>person_arr = { 武将_伊籍/*2*/, 武将_马良/*519*/, 武将_诸葛亮/*289*/, 武将_庞统/*557*/,武将_马谡/*511*/, 武将_诸葛均/*283*/, 武将_霍峻, 武将_向朗,武将_魏延/*124*/, 武将_甘宁/*118*/, 武将_黄祖/*175*/, 武将_李严/*603*/, 武将_刘磐/*634*/ };

			for (int i = 0; i < 部队_末; ++i)
			{
				auto unit = pk::get_unit(i);
				if (pk::is_alive(unit))
				{
					if (unit.get_force_id() == force_liubiao.get_id())
					{
						auto person = pk::get_person(unit.leader);
						if (person_arr.find(person.get_id())>0) pk::set_district(person, 刘备.get_district_id());
						else pk::set_district(person, 曹操.get_district_id());
					}
				}
			}

			array<int>city_arr1 = { 城市_江夏, 城市_江陵, 城市_长沙 };
			pk::city@ liuqi_city;
			for (int i = 0; i< int(city_arr1.length()); i++)
			{
				pk::city@ city0 = pk::get_city(city_arr1[i]);
				if (city0.get_force_id() == -1)
				{
					@liuqi_city = @city0;
					pk::set_district(pk::city_to_building(liuqi_city), 刘备.get_district_id());
					pk::set_district(刘琦, 刘备.get_district_id());
					pk::set_service(刘琦, liuqi_city.get_id());
					break;
				}
			}

			//无空城，占刘表城自立
			if (liuqi_city is null)
			{
				for (int i = 0; i< int(city_arr1.length()); i++)
				{
					pk::city@ city0 = pk::get_city(city_arr1[i]);
					if (city0.get_force_id() == force_liubiao.get_id())
					{
						@liuqi_city = @city0;
						pk::set_district(pk::city_to_building(liuqi_city), 刘备.get_district_id());
						pk::set_district(刘琦, 刘备.get_district_id());
						pk::set_service(刘琦, liuqi_city.get_id());
						pk::move(刘琦, pk::city_to_building(liuqi_city));
						pk::list<pk::person@> person_list_t = pk::get_person_list(pk::city_to_building(liuqi_city), pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
						for (int j = 0; j < person_list_t.count; j++)
						{
							if (person_list_t[j].get_id() == 武将_刘琦) continue;
							for (int k = 0; k < int(person_arr.length()); k++)
							{
								if (person_list_t[j].get_id() != person_arr[k])
								{
									pk::move(person_list_t[j], pk::city_to_building(liucong_city));
								}
							}
							
						}
						break;
					}
				}
			}
			
			for (int i = 0; i < int(特殊地名设施.length); i++)
			{
				pk::point pos0;
				pos0.x = 特殊地名设施[i][1];
				pos0.y = 特殊地名设施[i][2];
				if (pk::get_city_id(pos0) == liuqi_city.get_id())
				{
					pk::building@ building_t = pk::get_building(pos0);
					if (!pk::is_alive(building_t)) continue;
					int building_id = building_t.get_id();
					if (building_id > 据点_末 and building_id < 500)
					{
						//pk::trace("id:" + building_t.get_id() + "X,Y" + pos0.x + pos0.y);
						building_t.init_force = 刘备.get_force_id();
						if (person_arr.find(special_ex[i].person) < 0)
						{
							special_ex[i].person = -1;
							special_ex[i].troops = 0;
							special_ex[i].food = 0;
							special_ex[i].gold = 0;
						}
					}
				}
				else if (pk::get_building(pos0).get_force_id() == force_liubiao.get_id())
				{
					pk::building@ building_t = pk::get_building(pos0);
					if (!pk::is_alive(building_t)) continue;
					int building_id = building_t.get_id();
					if (building_id > 据点_末 and building_id < 500)
					{
						if (person_arr.find(special_ex[i].person) > -1)
						{
							special_ex[i].person = -1;
							special_ex[i].troops = 0;
							special_ex[i].food = 0;
							special_ex[i].gold = 0;
						}
					}
				}

			}
			pk::list<pk::person@> person_list0 = pk::get_person_list(force_liubiao,pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
			pk::list<pk::person@> person_list1;
			pk::list<pk::person@> person_list2;
			array<int>person_arr0 = { 武将_伊籍/*2*/, 武将_马良/*519*/, 武将_诸葛亮/*289*/, 武将_庞统/*557*/ };
			array<int>person_arr1 = { 武将_马谡/*511*/, 武将_诸葛均/*283*/ };
			array<int>person_arr2 = { 武将_霍峻, 武将_向朗 };
			array<int>person_arr3 = { 武将_魏延/*124*/, 武将_甘宁/*118*/, 武将_黄祖/*175*/, 武将_李严/*603*/, 武将_刘磐/*634*/ };
			for (int i = 0; i< person_list0.count; i++)
			{
				for (int j = 0; j< int(person_arr0.length()); j++)
				{
					if (person_list0[i].get_id() == person_arr0[j])
					{
						person_list0[i].loyalty = 100;
						person_list0[i].kouseki = 10000;
						pk::set_district(person_list0[i],刘备.get_district_id());
						if (person_list0[i].location != liuqi_city.get_id()) pk::move(person_list0[i], pk::city_to_building(liuqi_city));
						person_list0.remove_at(i);
					}
				}
				for (int j = 0; j< int(person_arr1.length()); j++)
				{
					if (person_list0[i].get_id() == person_arr1[j])
					{
						person_list0[i].loyalty = 100;
						person_list0[i].kouseki = 5000;
						pk::set_district(person_list0[i], 刘备.get_district_id());
						if (person_list0[i].location != liuqi_city.get_id())pk::move(person_list0[i], pk::city_to_building(liuqi_city));
						person_list0.remove_at(i);
					}
				}
				for (int j = 0; j< int(person_arr2.length()); j++)
				{
					if (person_list0[i].get_id() == person_arr2[j])
					{
						//person_list0[i].loyalty = 100;
						person_list0[i].kouseki = 5000;
						pk::set_district(person_list0[i], 刘备.get_district_id());
						if (person_list0[i].location != liuqi_city.get_id())pk::move(person_list0[i], pk::city_to_building(liuqi_city));
						person_list0.remove_at(i);
					}
				}
				for (int j = 0; j< int(person_arr3.length()); j++)
				{
					if (person_list0[i].get_id() == person_arr3[j])
					{
						pk::resign(person_list0[i]);
						pk::set_banned_kunshu(person_list0[i],武将_曹操, 36);
						person_list0.remove_at(i);
					}
				}
			}


			//投刘备 {武将_伊籍, 武将_霍峻, 武将_向朗, 武将_马良/*519*/, 武将_马谡/*511*/, 武将_诸葛亮/*289*/, 武将_诸葛均/*283*/, 武将_庞统/*557*/}
			//三档 10000功绩 100忠诚 伊籍, 马良，诸葛亮，庞统  5000功绩 100忠诚 马谡 诸葛均 其余5000功绩  武将_霍峻, 武将_向朗, 
			//下野-禁仕曹操36旬 {武将_魏延/*124*/, 武将_甘宁/*118*/, 武将_黄祖/*175*/, 武将_李严/*603*/, 武将_刘磐/*634*/}--pk::resign();
			//同血缘筛选，排除俘虏


			pk::kill(force_liubiao, true, force_caocao);

			//资源变化
			pk::building@building0 = pk::get_building(刘琮.service);
			pk::add_gold(building0, 3000);
			pk::add_food(building0, 30000);
			pk::add_troops(building0, 3000);
			pk::add_weapon_amount(building0, 1, 3000);
			pk::add_weapon_amount(building0, 2, 3000);
			pk::add_weapon_amount(building0, 3, 3000);
			pk::add_weapon_amount(building0, 4, 3000);

			pk::building@building1 = pk::get_building(刘备.service);
			pk::add_gold(building1, 1000);
			pk::add_food(building1, 20000);
			pk::add_troops(building1, 10000);
			pk::add_weapon_amount(building1, 1, 2000);
			pk::add_weapon_amount(building1, 2, 2000);
			pk::add_weapon_amount(building1, 3, 2000);
			pk::add_weapon_amount(building1, 4, 21000);
		}

		pk::msg_param set_msg_param(pk::msg_param msg_param, int id, pk::person@ person1, pk::person@ person2)
		{
			msg_param.id = id;
			@msg_param.person[0] = person1;
			@msg_param.person[1] = person2;
			return msg_param;
		}

		pk::array<int> get_dst_force_id()
		{
			pk::array<int>arr_t;
			pk::array<int>arr_city_t = { 城市_襄阳, 城市_江陵, 城市_柴桑, 城市_建业 };
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
// ## 2021/11/22 # 江东新风 # 新地图的孙刘联盟剧情 ##
/***CUSTOM
@name 事件-孙刘联盟
@class 历史事件；孙刘联盟
@author 江东新风
@contact
@version 1.0
@date 2021/11/22
@desc 條件：
‧ 選207年劇本
‧ 三顧、南征、劉表之死已發生
‧ 劉備及諸葛亮於江夏同城待機(經測試，在江陵城亦可)
‧ 孫權佔有江東
    (與孫權友好度，需「中立」以上)
結果：
‧ 可選是否同盟，劉同盟孫不同盟時舌戰，孫自動被諸葛亮打敗
‧ 孫劉結盟，友好度上升，對曹操友好度下降
@notice
***/


namespace 事件_孙刘联盟
{


	// ================ CUSTOMIZE ================
	const bool 事件剧本限定 = true;//默认限定剧本，限定2号剧本群雄割据可用，如不限定剧本，则可能在非对应剧本满足条件也会触发，但是会增加事件菜单的占用，以及可能会有未知bug
	// ===========================================


	class Main
	{
		pk::person@ 曹操;
		pk::person@ 刘备;
		pk::person@ 诸葛亮;
		pk::person@ 孙某;

		pk::person@ 刘表;
		pk::person@ 刘琦;
		pk::person@ 刘琮;
		pk::person@ 蔡瑁;


		pk::building@ caocao_building;
		pk::force@ force_liubiao;
		pk::force@ force_liubei;
		pk::force@ force_caocao;
		pk::force@ force_sun;

		pk::building@ building_;
		pk::force@ force_;
		pk::city@ city_;

		string info_事件信息;

		bool show_info = false;
		Main()
		{
			//pk::bind(pk::EVE_NEW_DAY, pk::trigger107_t(on_new_day));
			add_menu_孙刘联盟();
			pk::bind(120, pk::trigger120_t(func_信息显示_事件));//试试通过按钮来显示相关信息
			pk::bind(102, -1, pk::trigger102_t(callback));//剧本初始化-读档时
		}
		void callback()
		{
			pk::set_eve(19, false);//关闭原本事件的孙刘联盟
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

		void add_menu_孙刘联盟()
		{
			pk::menu_item item_孙刘联盟;
			item_孙刘联盟.menu = global_menu::菜单_事件;
			item_孙刘联盟.init = pk::building_menu_item_init_t(init_孙刘联盟);
			item_孙刘联盟.is_enabled = pk::menu_item_is_enabled_t(isEnabled_孙刘联盟);
			item_孙刘联盟.is_visible = pk::menu_item_is_visible_t(isVisible_孙刘联盟);
			item_孙刘联盟.get_text = pk::menu_item_get_text_t(getText_孙刘联盟);
			item_孙刘联盟.get_desc = pk::menu_item_get_desc_t(getDesc_孙刘联盟);
			item_孙刘联盟.handler = pk::menu_item_handler_t(handler_孙刘联盟);
			pk::add_menu_item(item_孙刘联盟);
		}

		//------------------------------让徐州---------------------------------------------//
		void init_孙刘联盟(pk::building@ building)
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

		string getText_孙刘联盟()
		{
			return pk::encode("孙刘联盟");  //quchu teji
		}

		bool isEnabled_孙刘联盟()
		{
			return true;
		}

		bool isVisible_孙刘联盟()
		{
			//forceinfo force_t(force_.get_id());
			//if (force_t.nanzheng) return false;
			//if (pk::get_scenario().no != 7 and 事件剧本限定) return false;//注意如果改过剧本，那这里需要改动
			if (setting_ex.eve_happened[19]) return false;//已发生后不显示
			return true;
		}

		string getDesc_孙刘联盟()
		{
			return pk::encode("执行孙刘联盟事件.");

		}

		bool handler_孙刘联盟()
		{
			bool confirm = pk::yes_no(pk::encode("是否查看\x1b[2x孙刘联盟\x1b[0x事件条件?"));
			if (confirm)
			{
				bool start_event = false;
				info_事件信息 = 获取任务条件();
				show_info = true;
				if (check_all())
				{
					start_event = pk::yes_no(pk::encode("满足事件条件，是否执行\x1b[2x孙刘联盟\x1b[0x?"));
					if (start_event)
					{
						show_info = false;
						pk::scene(pk::scene_t(event_孙刘联盟));
						//after_event_2();
					}
				}
				else
				{
					pk::message_box(pk::encode("\x1b[2x孙刘联盟\x1b[0x条件不满足，事件无法执行。"));
				}
			}
			show_info = false;
			return true;

		}

		string 获取任务条件()
		{
			/*
條件：
‧ 選207年劇本
‧ 三顧、南征、劉表之死已發生
‧ 劉備及諸葛亮於江夏同城待機(經測試，在江陵城亦可)
‧ 孫權佔有江東
	(與孫權友好度，需「中立」以上)
結果：
‧ 可選是否同盟，劉同盟孫不同盟時舌戰，孫自動被諸葛亮打敗
‧ 孫劉結盟，友好度上升，對曹操友好度下降
			*/
			string info_事件信息_t = pk::encode("      \x1b[1x孙刘联盟触发条件\x1b[0x\n");
			string info_条件0 = (条件0_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("曹操,刘备身份为君主\x1b[0x");
			string info_条件1 = (条件1_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("经过回合数大于18\x1b[0x");
			string info_条件2 = (条件2_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘表之死已经发生超过一月\x1b[0x");
			string info_条件3 = (条件3_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("诸葛亮所属势力为刘备军且为军师\x1b[0x");
			string info_条件4 = (条件4_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("曹操占有许昌和宛，且城市数大于等于5\x1b[0x");
			string info_条件5 = (条件5_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("扬州所属势力是孙家血脉\x1b[0x");
			string info_条件6 = (条件6_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备所在势力与孙家所在势力相邻\x1b[0x");
			string info_条件7 = (条件7_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("孙刘均不与曹操停战或同盟，且孙刘未同盟\x1b[0x");
			string info_条件8 = (条件8_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("孙刘友好度中立以上，且与曹操势力友好度亲密以下\x1b[0x");
			string info_条件9 = (条件9_check() ? pk::encode("\n\x1b[2x√") : pk::encode("\n\x1b[29x×")) + pk::encode("刘备诸葛亮可执行剧情\x1b[0x");

			//0-19险恶 20-39不和 40-60中立 61-80友好 81-100亲密
			info_事件信息_t = info_事件信息_t + info_条件0 + info_条件1 + info_条件2 + info_条件3 + info_条件4 + info_条件5 + info_条件6 + info_条件7 + info_条件8 + info_条件9;
			return info_事件信息_t;
		}

		bool check_all()
		{
			if (条件0_check() and 条件1_check() and 条件2_check() and 条件3_check() and 条件4_check() and 条件5_check() and 条件6_check() and 条件7_check() and 条件8_check() and 条件9_check())
				return true;
			return false;
		}


		bool 条件0_check()
		{
			if (曹操.mibun == 身份_君主 and 刘备.mibun == 身份_君主) return true;
			return false;
		}

		bool 条件1_check()
		{
			if (pk::get_elapsed_days() >= 180) return true;//剧本开始超过18旬
			return false;
		}

		bool 条件2_check()
		{
			// 刘表之死已经发生------还需要加入经过月数
			if (setting_ex.eve_happened[18]) return true;
			return false;
		}

		bool 条件3_check()
		{
			if (force_liubei.gunshi == 武将_诸葛亮 and 诸葛亮.get_force_id() == force_liubei.get_id()) return true;
			return false;
		}

		bool 条件4_check()
		{
			// 曹操军占有许昌和宛
			if (pk::get_city(城市_许昌).get_force_id() != 曹操.get_force_id()) return false;
			if (pk::get_city(城市_宛).get_force_id() != 曹操.get_force_id()) return false;
			pk::list<pk::city@> caocao_citys = pk::get_city_list(force_caocao);
			if (caocao_citys.count < 5) return false;
			return true;
		}

		bool 条件5_check()
		{
			//扬州所属势力是孙家血脉
			int count = 1;
			int force_id = -1;
			int force_id_pre = -1;
			for (int i = 0; i < 城市_末; ++i)
			{
				auto city = pk::get_city(i);
				if (city.province == 州_扬州)
				{
					force_id_pre = force_id;
					force_id = city.get_force_id();
					if (force_id_pre == force_id) count += 1;
				}
			}
			if (count != 5)  return false;
			if (pk::is_valid_normal_force_id(force_id))
			{
				auto force0 = pk::get_force(force_id);
				auto person0 = pk::get_person(force0.kunshu);
				if (person0.ketsuen == 武将_孙坚)
				{
					@孙某 = @person0;
					@force_sun = pk::get_force(force_id);
					return true;
				}
			}
			return false;
		}

		bool 条件6_check()
		{
			// 曹操军占有许昌和宛
			if (force_sun is null) return false;
			if (pk::is_neighbor_force(force_sun,force_liubei)) return true;
			return true;

		}

		bool 条件7_check()
		{
			//孙刘均不与曹操停战或同盟，且孙刘未同盟
			if (force_sun is null) return false;
			if (force_liubei.ally[force_sun.get_id()]) return false;
			if (!pk::is_enemy(force_liubei, force_caocao)) return false;
			if (!pk::is_enemy(force_sun, force_caocao)) return false;
			return true;

		}
		
		bool 条件8_check()
		{
			//孙刘友好度中立以上，且与曹操势力友好度亲密以下
			if (force_sun is null) return false;
			if (force_liubei.relations[force_sun.get_id()] < 40) return false;
			if (force_caocao.relations[force_liubei.get_id()] > 60) return false;
			if (force_caocao.relations[force_sun.get_id()] > 60) return false;
			return true;

		}

		bool 条件9_check()
		{
			//孙刘友好度中立以上，且与曹操势力友好度亲密以下
			if (!can_event(刘备) or !can_event(诸葛亮)) return false;
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


		void event_孙刘联盟()
		{

			pk::person@ 文官 = pk::get_person(武将_文官);//715

			pk::msg_param msg_param(18563, null, null);
			@msg_param.person[2] = @曹操;
			@msg_param.person[3] = @诸葛亮;
			@msg_param.person[4] = @孙某;
			@msg_param.force[0] = @force_liubei;

			pk::play_bgm(16);
			pk::move_screen(刘备.get_pos());

			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18563, null, null)));
			pk::fade(0);
			pk::background(场景_宫廷2);
			
			pk::sleep();
			pk::fade(255);


			//pk::message_box(pk::get_msg(18520, 刘表, null), 刘表);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18564, 刘备, 诸葛亮)), 刘备);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18565, 诸葛亮, 刘备)), 诸葛亮);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18566, 刘备, 诸葛亮)), 刘备);
			pk::cutin(CG_嘲笑);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18567, 诸葛亮, 刘备)), 诸葛亮);
			int choose = 0;
			if (force_liubei.is_player())
			{
				array<string> issue_list =
				{
					pk::get_msg(set_msg_param(msg_param, 18569, null, null)),
					pk::get_msg(set_msg_param(msg_param, 18570, null, null))
				};
				choose = pk::choose(issue_list, pk::get_msg(set_msg_param(msg_param, 18568, null, null)), 刘备);
			}
			if (choose == 1)
			{
				pk::message_box(pk::get_msg(set_msg_param(msg_param, 18571, 刘备, 诸葛亮)), 刘备);
				setting_ex.eve_happened[19] = true;
				return;
			}

			pk::play_sfx(11);
			pk::fade(0);
			pk::background(-1);
			pk::sleep();
			pk::fade(255);
			pk::move_screen(孙某.get_pos());
			pk::fade(0);
			pk::background(场景_宫廷2);
			pk::sleep();
			pk::fade(255);
			pk::cutin(CG_脚步);

			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18573, 文官, 孙某)), 文官);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18574, 孙某, 文官)), 孙某);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18575, 文官, 孙某)), 文官);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18576, 孙某, 文官)), 孙某);


			pk::fade(0);
			pk::sleep();
			pk::open_event_still(21);
			pk::fade(255);
			pk::play_sfx(601);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18577, 诸葛亮, 孙某)), 诸葛亮);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18578, 孙某, 诸葛亮)), 孙某);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18579, 诸葛亮, 孙某)), 诸葛亮);
			pk::close_event_still();
			int choose2 = 0;
			if (force_sun.is_player())
			{
				array<string>  issue_list =
				{
					pk::get_msg(set_msg_param(msg_param, 18581, null, null)),//同盟
					pk::get_msg(set_msg_param(msg_param, 18582, null, null))//不同盟
				};
				choose2 = pk::choose(issue_list, pk::get_msg(set_msg_param(msg_param, 18580, 孙某, null)), 孙某);
			}
			if (choose2 == 1)
			{
				pk::message_box(pk::get_msg(set_msg_param(msg_param, 18583, 孙某, 诸葛亮)), 孙某);
				pk::cutin(CG_嘲笑);
				pk::message_box(pk::get_msg(set_msg_param(msg_param, 18584, 诸葛亮, 孙某)), 诸葛亮);
				int debate_result = pk::debate(孙某, 诸葛亮, true, false, true, true).first;
				if (debate_result == 0)
				{
					pk::fade(0);
					pk::background(场景_宫廷2);
					pk::sleep();
					pk::fade(255);
					pk::message_box(pk::get_msg(set_msg_param(msg_param, 18585, 孙某, 诸葛亮)), 孙某);
					pk::message_box(pk::get_msg(set_msg_param(msg_param, 18586, 诸葛亮, 孙某)), 诸葛亮);
					setting_ex.eve_happened[19] = true;
					return;
				}
				else
				{
					pk::fade(0);
					pk::background(场景_宫廷2);
					pk::sleep();
					pk::fade(255);
					pk::message_box(pk::get_msg(set_msg_param(msg_param, 18587, 诸葛亮, 孙某)), 诸葛亮);
				}
			}

			pk::play_sfx(612);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18588, 孙某, 诸葛亮)), 孙某);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18589, 诸葛亮, 孙某)), 诸葛亮);

			pk::fade(0);
			pk::background(-1);
			pk::sleep();
			pk::fade(255);

			pk::open_event_still(24);
			pk::play_sfx(603);
			pk::message_box(pk::get_msg(set_msg_param(msg_param, 18590, null, null)));
			pk::close_event_still();
			setting_ex.eve_happened[19] = true;

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
			诸葛亮.kouseki += 1000;
			pk::add_relations(force_liubei, force_sun.get_id(), 20);
			pk::set_ally(force_liubei, force_sun.get_id(), true);
			pk::add_relations(force_caocao, force_sun.get_id(), -30);
			pk::add_relations(force_caocao, force_liubei.get_id(), -30);
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
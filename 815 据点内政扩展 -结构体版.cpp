// ## 2022/01/29 # 江东新风 # 空指针报错 ##
// ## 2021/12/06 # 江东新风 # 征兵巡查窗口优化，文本优化 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进,港关治安bug修复 ##
// ## 2021/10/26 # 江东新风 # 折中方案解决据点无太守bug ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/10/10 # 江东新风 # 为非城市据点添加治安设定，并修改相关内容 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2021/02/15 # 江东新风 # 据点内政添加排序，武将未行动bug修复 ##
// ## 2021/01/24 # 江东新风 # 改写据点内政扩展以适应单港关开局 ##
// ## 2021/01/20 # 江东新风 # 以结构体的形式储存据点内政信息 ##
// ## 2020/12/15 # 江东新风 # 针对支城和关隘分别设置菜单名称 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/21 # 江东新风 # 添加法治效果 ##
// ## 2020/08/09 # 氕氘氚 # 添加港guan菜单快捷鍵 ###
// @@ 2020/07/26 @ 江东新风 @@
 /*
@作者: 江东新风
@說明: 据点可以执行征兵和巡察命令
@计划: 换季据点治安减少设定

*/

namespace 据点内政
{
	//KEY是信息保存的緩存zhi，只能用韓文철거금지才能儲存，用其他任何东西都会导致无法保存信息
	const int KEY = pk::hash("据点内政");
	// ================ CUSTOMIZE ================
	const bool 据点_征兵 = true;    // true = on, false = off
	const bool 据点_巡察 = true;    // true = on, false = off
	const int ACTION_COST_征兵 = 20;  // 征兵行动力消耗
	const int ACTION_COST_巡察 = 20;  // 征兵行动力消耗		
	//const int energy_ = 20; 	 // 士气减低
	const int GOLD_COST_征兵 = 300;  //征兵金消耗
	const int GOLD_COST_巡察 = 100;  //征兵金消耗
	const int FOOD_COST_征兵 = 1000;  //远距离征兵粮消耗
	const int Distance_max = 2; //能巡察的最远城市距离，2=20日
///缺少技巧增加		
		//const int FOOD_COST = 2000; //粮消耗

	// ===========================================

	class Main
	{
		pk::building@ building_;
		pk::force@ force_;
		pk::person@ taishu_;
		pk::person@ actor;
		pk::city@ city_;

		int menu_building_id_;

		int check_result_征兵_;
		int check_result_巡察_;
		//int city_id0;
		Main()
		{
			//触发器设定
			pk::bind(111, pk::trigger111_t(onTurnStart));

			//加入非城市据点每季治安降低
			pk::bind(109, pk::trigger109_t(onNewSeason));
			//不确定是否要将据点攻占后的次数重置加入
			//据点菜单
			add_menu();
		}

		void add_menu()
		{
			pk::menu_item 据点;
			int parent;

			据点.menu = 0;
			据点.pos = 1;
			据点.shortcut = "1";
			据点.get_text = pk::menu_item_get_text_t(getText);//cast<pk::menu_item_get_text_t@>(function() { return pk::encode("据点"); });  //yizu guyong
			据点.init = pk::building_menu_item_init_t(init);
			据点.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
			parent = pk::add_menu_item(据点);

			if (据点_征兵)
			{
				pk::menu_item item;
				int parent_征兵;
				item.menu = parent;
				item.shortcut = "1";
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_据点_征兵);
				item.get_text = pk::menu_item_get_text_t(getText_据点_征兵);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_据点_征兵);
				item.handler = pk::menu_item_handler_t(handler_据点_征兵);
				parent_征兵 = pk::add_menu_item(item);

			}
			if (据点_巡察)
			{
				pk::menu_item item;
				item.menu = parent;
				item.shortcut = "2";
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_据点_巡察);
				item.get_text = pk::menu_item_get_text_t(getText_据点_巡察);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_据点_巡察);
				item.handler = pk::menu_item_handler_t(handler_据点_巡察);
				pk::add_menu_item(item);
			}
			if (false)
			{
				pk::menu_item item;
				item.menu = parent;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_据点_巡察);
				item.get_text = pk::menu_item_get_text_t(getText_据点_兵装生产);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_据点_征兵);
				item.handler = pk::menu_item_handler_t(handler_据点_征兵);
				pk::add_menu_item(item);
			}
			if (false)
			{
				pk::menu_item item;
				item.menu = parent;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_据点_巡察);
				item.get_text = pk::menu_item_get_text_t(getText_据点_商人);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_据点_征兵);
				item.handler = pk::menu_item_handler_t(handler_据点_征兵);
				pk::add_menu_item(item);
			}
			if (false)
			{
				pk::menu_item item;
				item.menu = parent;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_据点_巡察);
				item.get_text = pk::menu_item_get_text_t(getText_据点_城墙维修);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_据点_征兵);
				item.handler = pk::menu_item_handler_t(handler_据点_征兵);
				pk::add_menu_item(item);
			}
		}

		/** 获取最近我方城市.且已经计算符合距离要求 */
		pk::building@ src_t;
		int getNearestBase(pk::building@ src)
		{
			int src_id = src.get_id();
			@src_t = @src;
			int force_id0 = src.get_force_id();
			pk::force@ force0 = pk::get_force(force_id0);
			pk::list<pk::city@> city_list = pk::get_city_list(force0);
			// 철수 거점 검색 撤离目标据点搜索
			
			if (city_list.count < 1 )//and src_id < 据点_城市末)
				return -1;      // 마지막 도시인 경우 철수 불가如果这是?的最后一个城市
			else if (city_list.count == 1)
			{
				int distance = pk::get_building_distance(city_list[0].get_id(), src_id, force_id0);
				if (distance <= Distance_max) return city_list[0].get_id();
			}
			else
			{
				city_list.sort(function(a, b)
				{
					int build_dist_a = pk::get_building_distance(a.get_id(), main.src_t.get_id(), a.get_force_id());
					int build_dist_b = pk::get_building_distance(b.get_id(), main.src_t.get_id(), b.get_force_id());

					int pos_dist_a = pk::get_distance(a.pos, main.src_t.pos);
					int pos_dist_b = pk::get_distance(b.pos, main.src_t.pos);

					if (build_dist_a != build_dist_b)
						return (build_dist_a < build_dist_b);

					return (pos_dist_a < pos_dist_b);
				});
				int distance = pk::get_building_distance(city_list[0].get_id(), src.get_id(), force_id0);
				if (distance <= Distance_max) return city_list[0].get_id();
			}
			return -1;
		}

		///-------------触发器部分----------------		

		void onTurnStart(pk::force@ force)
		{
			///回合开始时恢复次数
			for (int i = 城市_末; i < 据点_末; ++i)
			{
				if (pk::get_building(i).get_force_id() == force.get_id())
				{
					BaseInfo@ base_t = @base_ex[i];
					base_t.recruit_done = false;
					base_t.inspections_done = false;
				}
			}
		}

		void onNewSeason()
		{
			for (int building_id = 城市_末; building_id < 据点_末; ++building_id)
			{
				pk::building@ building = pk::get_building(building_id);
				int order_dec = inter::get_season_order_dec(building);
				//pk::trace(pk::format("building_id：{}，public_order：{}，order_dec：{}",building_id, base_t.public_order, order_dec));
				ch::add_public_order(building, order_dec, true);
				
			}
		}
		///菜单的支持函数及处理		
		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@taishu_ = pk::get_person(pk::get_taishu_id(building));

			menu_building_id_ = building_.get_id();

			//加一个修复无港关太守bug
			if (pk::is_valid_base_id(menu_building_id_))
			{

				check_result_征兵_ = check_avaliable_据点_征兵(building);
				check_result_巡察_ = check_avaliable_据点_巡察(building);
				if (pk::get_taishu_id(building) == -1)
				{
					if (pk::get_person_list(building, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)).count > 0)
						pk::reset_taishu(building);
				}
			}
		}

		string getText()
		{
			if (building_.facility == 设施_关隘)
				return pk::encode("关隘");
			else if (building_.facility == 设施_港口)
				return pk::encode("支城");
			else return ("据点");
		}

		bool isMenuVisible()
		{
			if (building_.get_id() < 城市_末 or building_.get_id() >= 据点_末) return false;
			return true;
		}

		int check_avaliable_据点_征兵(pk::building@ building)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			//pk::trace(pk::format("3.1:{}{}{}{}", pk::enemies_around(building), list.count, pk::get_district(building.get_district_id()).ap < ACTION_COST, pk::get_gold(building) < GOLD_COST));			
			if (base_ex[building.get_id()].recruit_done) return 1;
			else if (pk::get_gold(building) < GOLD_COST_征兵) return 2;
			else if (pk::get_troops(building) >= pk::get_max_troops(building)) return 3;
			else if (pk::get_district(building.get_district_id()).ap < ACTION_COST_征兵) return 4;
			else if (pk::get_idle_person_list(building).count < 1) return 5;
			else return 0;
		}

		bool isEnabled_据点_征兵()
		{			
			if (check_result_征兵_ != 0) return false;
			else return true;
		}

		string getDesc_据点_征兵()
		{
			switch (check_result_征兵_)
			{
			case 1: return pk::encode("已执行过征兵.");
			case 2: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST_征兵));
			case 3: return pk::encode("兵力已达到上限.");
			case 4: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST_征兵));
			case 5: return pk::encode("无可执行武将.");
			case 0: return pk::encode(pk::format("执行据点征兵.(金 {}, 行动力 {})", GOLD_COST_征兵, ACTION_COST_征兵));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		int check_avaliable_据点_巡察(pk::building@ building)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			//pk::trace(pk::format("3.1:{}{}{}{}", pk::enemies_around(building), list.count, pk::get_district(building.get_district_id()).ap < ACTION_COST, pk::get_gold(building) < GOLD_COST));			
			if (base_ex[building.get_id()].inspections_done) return 1;
			else if (pk::get_gold(building) < GOLD_COST_巡察) return 2;
			else if (base_ex[building.get_id()].public_order > 99) return 3;
			else if (pk::get_district(building.get_district_id()).ap < ACTION_COST_巡察) return 4;
			else if (pk::get_idle_person_list(building).count < 1) return 5;
			else return 0;
		}

		bool isEnabled_据点_巡察()
		{
			if (check_result_巡察_ != 0) return false;
			else return true;
		}

		string getDesc_据点_巡察()
		{
			switch (check_result_巡察_)
			{
			case 1: return pk::encode("已执行过巡察.");
			case 2: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST_巡察));
			case 3: return pk::encode("治安已达到上限.");
			case 4: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST_巡察));
			case 5: return pk::encode("无可执行武将.");
			case 0: return pk::encode(pk::format("执行据点巡察.(金 {}, 行动力 {})", GOLD_COST_巡察, ACTION_COST_巡察));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		string getText_据点_征兵()
		{
			return pk::encode(" 征兵 ");
		}

		string getText_据点_巡察()
		{
			return pk::encode(" 巡查 ");
		}

		string getText_据点_兵装生产()
		{
			return pk::encode("兵装生产");
		}

		string getText_据点_商人()
		{
			return pk::encode("商人");
		}

		string getText_据点_城墙维修()
		{
			return pk::encode("城墙维修");
		}

		bool handler_据点_征兵()
		{
			pk::recruit_cmd_info cmd_draft;
			if (pk::call_recruit_menu(cmd_draft, building_))
			{
				run_order_征兵(cmd_draft, building_);
				return true;
			}
			return false;
		}

		bool run_order_征兵(pk::recruit_cmd_info cmd_draft, pk::building@building)
		{
			pk::list<pk::person@> person_sel;
			for (int i = 0; i < 3; ++i)
			{
				if (cmd_draft.actors[i] !is null)
				{
					person_sel.add(cmd_draft.actors[i]);
					pk::add_stat_exp(cmd_draft.actors[i],武将能力_魅力,2);
					pk::add_kouseki(cmd_draft.actors[i], 50);
					cmd_draft.actors[i].action_done = true;
				}					
			}
			//治安降低设定，如所属城市是我方城市，则降低該城市治安，如果所属城市非我方，则降低最近我方城市治安
			int troops = inter::get_recruit_num(building, person_sel, 0);//最后一个参数决定是否影响兵役数量，0为影响
			int security = inter::get_recruit_order_dec(building, person_sel, troops);
			int energy = building.energy * building.troops / (building.troops + troops) - building.energy;

			pk::add_gold(building, -GOLD_COST_征兵, true);
			ch::add_troops(building, troops, true);
			pk::add_energy(building, energy, true);
			ch::add_public_order(building, security, true);

			if (pk::is_player_controlled(building))
			{			
				pk::person@ kunshu = pk::get_person(pk::get_kunshu_id(cmd_draft.actors[0]));
				pk::msg_param msg_param(5287, cmd_draft.actors[0], kunshu);
				@msg_param.building[0] = @building;
				msg_param.num[0] = troops;
				pk::play_se(6);
				pk::say(pk::get_msg(msg_param), cmd_draft.actors[0], building);

				pk::msg_param msg_param2(5286, cmd_draft.actors[0], kunshu);
				@msg_param2.hex_object[0] = @building;
				msg_param2.num[0] = building.troops;
				msg_param2.num[1] = troops;
				pk::history_log(building.pos, (pk::get_force(building.get_force_id())).color, pk::get_msg(msg_param2));
			}

			//设成已执行
			BaseInfo@ base_t = @base_ex[building.get_id()];
			base_t.recruit_done = true;

			// 행동력 감소.
			auto district = pk::get_district(building.get_district_id());
			pk::add_ap(district, -ACTION_COST_征兵);
			return true;
		}

		bool handler_据点_巡察()
		{
			pk::inspections_cmd_info cmd_draft;
			if (pk::call_inspections_menu(cmd_draft, building_, base_ex[menu_building_id_].public_order))
			{
				run_order_巡察(cmd_draft, building_);
				return true;
			}
			return false;
		}

		bool run_order_巡察(pk::inspections_cmd_info cmd_draft, pk::building@building)
		{
			pk::list<pk::person@> person_sel;
			for (int i = 0; i < 3; ++i)
			{
				if (cmd_draft.actors[i] !is null)
				{
					person_sel.add(cmd_draft.actors[i]);
					pk::add_stat_exp(cmd_draft.actors[i], 武将能力_统率, 2);
					pk::add_kouseki(cmd_draft.actors[i], 50);
					cmd_draft.actors[i].action_done = true;
				}
			}
			//治安降低设定，如所属城市是我方城市，则降低該城市治安，如果所属城市非我方，则降低最近我方城市治安
			int security = inter::get_inspections_order_inc(building, person_sel);
			pk::add_gold(building, -GOLD_COST_巡察, true);
			ch::add_public_order(building, security, true);

			if (pk::is_player_controlled(building))
			{
				pk::person@ kunshu = pk::get_person(pk::get_kunshu_id(cmd_draft.actors[0]));
				pk::msg_param msg_param(5246, cmd_draft.actors[0], kunshu);
				@msg_param.building[0] = @building;
				msg_param.num[0] = security;
				pk::play_se(6);
				pk::say(pk::get_msg(msg_param), cmd_draft.actors[0], building);

				pk::history_log(building.pos, (pk::get_force(building.get_force_id())).color, pk::encode(pk::format("\x1b[1x{}\x1b[0x的治安增至\x1b[2x{}\x1b[0x(+\x1b[2x{}\x1b[0x)",pk::decode(pk::get_name(building)), base_ex[building.get_id()].public_order, security)));
			}

			//设成已执行
			BaseInfo@ base_t = @base_ex[building.get_id()];
			base_t.inspections_done = true;

			// 행동력 감소.
			auto district = pk::get_district(building.get_district_id());
			pk::add_ap(district, -ACTION_COST_巡察);
			return true;
		}


	}

	Main main;
}
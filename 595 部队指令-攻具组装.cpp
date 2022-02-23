// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/02/02 # 江东新风 # 组装价格上调 ##
// ## 2022/01/12 # 江东新风 # 限制玩家刷攻具 ##
// ## 2022/01/09 # 江东新风 # 组装攻具加入兵力限制 ##
// ## 2021/11/09 # 江东新风 # 改变部队兵种---测试 ##

/*

*/

namespace 部队指令_攻具组装
{

	// ================ CUSTOMIZE ================

	const int ENERGY_COST = 20;
	//const int GOLD_COST = 800;
	const int TROOPS_NEED = 5000;
	const array<int> GOLD_COST = { 2600, 1700, 1500, 1600 ,0 ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0 };
	const array<int> 攻具对应 = {兵器_投石, 兵器_木兽, 兵器_冲车, 兵器_井阑};
	const array<string> 攻具对应文字 = { "投石", "木兽", "冲车", "井阑" };
	// ===========================================

	class Main
	{

		Main()
		{
			// 부대 메뉴 추가
			add_menu_unit_order();
		}

		pk::unit@ src_unit;
		pk::point src_pos_;
		pk::person@ src_leader;
		int check_result_;

		void add_menu_unit_order()
		{
			pk::menu_item 攻具组装;
			攻具组装.menu = 1;//global_menu::菜单_部队;
			攻具组装.init = pk::unit_menu_item_init_t(init);
			攻具组装.get_text = pk::menu_item_get_text_t(getText_攻具组装);
			攻具组装.get_desc = pk::menu_item_get_desc_t(getDesc_攻具组装);
			攻具组装.is_visible = pk::menu_item_is_visible_t(isVisible_攻具组装);
			攻具组装.is_enabled = pk::menu_item_is_enabled_t(isEnabled_攻具组装);
			攻具组装.handler = pk::unit_menu_item_handler_t(handler_攻具组装);
			pk::add_menu_item(攻具组装);

		} // add_menu_unit_order

		void init(pk::unit@ unit, pk::point src_pos)
		{
			@src_unit = @unit;
			src_pos_ = src_pos;
			@src_leader = pk::get_person(src_unit.leader);
			check_result_ = check_avaliable(src_unit, src_pos_);
		}

		//---------------------------------------------------------------------------
		// 攻具组装 
		//---------------------------------------------------------------------------

		string getText_攻具组装()
		{
			return pk::encode(pk::format("组装{}",攻具对应文字[src_unit.weapon]));  
		}

		bool isVisible_攻具组装()
		{
			if (src_unit.weapon < 0 or src_unit.weapon > 兵器_弩) return false;
			else if (src_unit.weapon == 兵器_剑 and !pk::has_tech(src_unit, 技巧_开发投石)) return false;
			else if (src_unit.weapon == 兵器_枪 and !pk::has_tech(src_unit, 技巧_开发木兽)) return false;
			//if (src_unit.weapon > 兵器_剑 and src_unit.weapon < 兵器_战马) return true;
			return true;
		}

		string getDesc_攻具组装()
		{
			switch (check_result_)
			{
			case 1: return pk::encode(pk::format("气力不足 (需要{}气力)", ENERGY_COST));
			case 2: return pk::encode(pk::format("资金不足 (需要{}金)", GOLD_COST[src_unit.weapon]));
			case 3: return pk::encode(pk::format("兵力不足，无法组装 (需要{}兵)", TROOPS_NEED));
			case 0: return pk::encode(pk::format("丢弃原兵器，组装攻城兵器(消耗：金{} 气力{})", GOLD_COST[src_unit.weapon], ENERGY_COST));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		bool isEnabled_攻具组装()
		{
			if (check_result_ != 0) return false;
			else return true;
		}

		int check_avaliable(pk::unit@ unit, pk::point src_pos)
		{
			//pk::trace(pk::format("3.1:{}{}{}{}", pk::enemies_around(building), list.count, pk::get_district(building.get_district_id()).ap < ACTION_COST, pk::get_gold(building) < GOLD_COST));
			if (unit.energy < ENERGY_COST) return 1;
			else if (unit.gold < GOLD_COST[unit.weapon]) return 2;
			else if (unit.troops < TROOPS_NEED) return 3;
			return 0;
		}

		bool handler_攻具组装(pk::point dst_pos)
		{

			pk::point pos = src_unit.get_pos();
			string src_leader_name = pk::decode(pk::get_name(src_leader));

			if (pk::is_in_screen(pos))
			{
				//pk::play_voice(0x37, src_leader);  // 
				//pk::play_se(0x39, pos);
				//pk::create_effect(0x3a, pos);

				switch (pk::rand(2))
				{
				case 0: pk::say(pk::encode("赶紧组装!"), src_leader); break;  
				case 1: pk::say(pk::encode("有此利器，此处攻城必将成功!"), src_leader); break;  
				}
			}
			pk::add_gold(src_unit, -GOLD_COST[src_unit.weapon], true);
			pk::change_weapon(src_unit, 攻具对应[src_unit.weapon]);

			pk::add_energy(src_unit, -ENERGY_COST, true);
			
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
				src_unit.action_done = false;

			return true;
		}


	} // class Main

	Main main;
}
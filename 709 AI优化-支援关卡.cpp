// ## 2022/03/17 # 江东新风 # 数组越界bug修复 ##
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/02/13 # 江东新风 # 为ai特定城市添加关卡支援 ##

namespace AI优化_关卡支援
{
	// ================ CUSTOMIZE ================
	/*
	洛阳-函谷关x- 虎牢关x+
	长安-潼关x+
	宛城-武关x-
	汉中-阳平关x- 大散关y-
	梓潼-葭萌关y- 涪水关x+
	成都-绵竹关y-
	*/
	const array<int> 相关城市 = {新据点_洛阳,新据点_长安,新据点_宛,新据点_汉中, 新据点_梓潼, 新据点_成都};
	const array<array<int>> 城关匹配 = {
		{新据点_洛阳, 新据点_函谷关,新据点_虎牢关},
		{新据点_长安, 新据点_潼关,-1},
		{新据点_宛, 新据点_武关,-1},
		{新据点_汉中, 新据点_阳平关,新据点_大散关},
		{新据点_梓潼, 新据点_葭萌关,新据点_涪水关},
		{新据点_成都, 新据点_绵竹关,-1}
};
	const array<array<bool>> 关卡信息 = {
	/*洛阳-函谷关x- 虎牢关x+*/{true,false,true,true},
	/*长安-潼关x+*/{true,true,false,false},
	/*宛城-武关*/{true,false,false,false},
	/*汉中-阳平关x- 大散关y-*/{true,false,false,false},
	/*梓潼-葭萌关y- 涪水关x+*/{false,false,true,true},
	/*成都-绵竹关y-+*/ {false,false,false,false}
};
	// ===========================================


	class Main
	{
		bool 调试模式 = false;
		pk::building@ building_;
		int check_result_;
		Main()
		{
			pk::bind(202, pk::trigger202_t(onAIRunningOrder));
		}

		void onAIRunningOrder(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 据点AI_支援关卡)
			{
				pk::list<pk::person@> person_sel;
				pk::list<pk::building@> building_sel;
				if (run_order_before(building, person_sel, building_sel)) run_order(building, person_sel, building_sel);
				else if (调试模式) pk::trace("据点AI_支援关卡 不满足");
			}
		}

		bool run_order_before(pk::building@ building0, pk::list<pk::person@>& out person_sel, pk::list<pk::building@>& out building_sel)
		{
			int city_id = building0.get_id();
			if (!pk::is_valid_city_id(city_id)) return false;
			if (pk::get_troops(building0) < 20000) return false;
			int index = 相关城市.find(city_id);
			if (index < 0) return false;
			for (int i = 1; i < 3; ++i)
			{
				if (城关匹配[index][i] != -1)
				{
					pk::building@ building = pk::get_building(城关匹配[index][i]);
					if (pk::get_troops(building) > 8000) continue;
					if (building.get_force_id() != building0.get_force_id()) continue;
					if (is_outside_enemy_around(building, 关卡信息[index][(i - 1) * 2], 关卡信息[index][(i - 1) * 2 + 1])) building_sel.add(building);
				}
			}
			if (building_sel.count == 0) return false;

			if (building_sel.count > 0)
			{
				pk::list<pk::person@> person_list = pk::get_idle_person_list(building0);
				if (person_list.count == 0) return false;
				person_list.sort(function(a, b)
				{
					bool a_win = ch::has_skill(a, 特技_运输);
					bool b_win = ch::has_skill(a, 特技_运输);
					if (a_win and !b_win) return true;
					if (!a_win and b_win) return false;
					int a_stat = (a.stat[武将能力_统率] + a.stat[武将能力_武力] + a.stat[武将能力_智力]);
					int b_stat = (b.stat[武将能力_统率] + b.stat[武将能力_武力] + b.stat[武将能力_智力]);
					return (a_stat < b_stat); 
				});
				person_sel.add(person_list[0]);
				if (building_sel.count > 1 and person_list.count > 1) person_sel.add(person_list[1]);
			}

			return true;
		}

		bool run_order(pk::building@ building0, pk::list<pk::person@> person_sel, pk::list<pk::building@> building_sel)
		{
			if (building0 is null) return false;
			if (building_sel[0] is null) return false;
			if (person_sel[0] is null) return false;

			for (int i = 0; i < building_sel.count; ++i)
			{
				pk::building@ target = @building_sel[i];
				pk::person@ actor = @person_sel[i];
				support_march(building0,target,actor);
			}

			return true;

		}

		void support_march(pk::building@building, pk::building@ target, pk::person@ actor)
		{
			if (target is null) return;
			if (actor is null) return;
			//=============================输送函数===========================//
			pk::com_deploy_cmd_info cmd;
			@cmd.base = @building;
			cmd.type = 部队类型_运输;
			cmd.member[0] = actor.get_id();
			cmd.gold = 0;
			cmd.food = 6000;
			cmd.troops = 6000;
			int i = 0;
			for (int weapon_id = 0; weapon_id < 兵器_末; weapon_id++)
			{
				int weapon_amount = 0;
				if (weapon_id < 兵器_冲车)
					weapon_amount = 1;
				else
					weapon_amount = 0;

				if (weapon_amount > 0)
				{
					cmd.weapon_id[i] = weapon_id;
					cmd.weapon_amount[i] = weapon_amount;
					i++;
				}
			}
			cmd.order = 部队任务_移动;
			cmd.target_pos = target.get_pos();

			int unit_id = pk::command(cmd);
			pk::unit@ unit_cmd = pk::get_unit(unit_id);
			if (pk::is_alive(unit_cmd))
				unit_cmd.action_done = true;  

			if (pk::is_in_screen(building.pos)) pk::say(pk::encode(pk::format("战况紧急，快向{}支援。", pk::decode(pk::get_name(target)))), actor);
			//=============================输送函数结束===========================//
		}

		bool is_outside_enemy_around(pk::building@building, bool x/*x,y*/, bool plus/*+,-*/)
		{
			array<pk::point> range_t = pk::range(building.pos, 1, 3);
			array<pk::point> range;
			for (int i = 0; i < int(range_t.length); i++)
			{
				if (x)
				{
					if (plus)
					{
						if (range_t[i].x > building.pos.x) range.insertLast(range_t[i]);
					}
					else
					{
						if (range_t[i].x < building.pos.x) range.insertLast(range_t[i]);
					}
				}
				else
				{
					if (plus)
					{
						if (range_t[i].y > building.pos.y) range.insertLast(range_t[i]);
					}
					else
					{
						if (range_t[i].y < building.pos.y) range.insertLast(range_t[i]);
					}
				}
			}
			for (int i = 0; i < int(range.length); i++)
			{
				auto hex = pk::get_hex(range[i]);
				if (hex.has_unit)
				{
					pk::unit@ unit = pk::get_unit(range[i]);
					if (pk::is_alive(unit) and pk::is_enemy(building,unit))
						return true;
				}
			}
			return false;
		}
	}

	Main main;
}
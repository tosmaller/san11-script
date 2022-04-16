// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/02/06 # 江东新风 # 府兵带兵数调整为被封武将官职低一级的带兵数##
// ## 2021/01/29 # 江东新风 # 府兵不进攻问题，武将所属变更封地未同步问题，稍微削弱府兵属性##
// ## 2021/01/11 # 江东新风 # 重复分封问题 ##
// ## 2021/01/05 # 江东新风 # 修复前线封地势力bug，玩家运输部队可补给封地 ##
// ## 2021/12/02 # 江东新风 # ai分封和解除分封 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/10/10 # 江东新风 # 初始制作 ##

/*
待处理：玩家府兵资源的输送，建筑被破坏时，封地信息清空
*/
namespace 府兵系统
{
	// ================ CUSTOMIZE ================


	const int ACTION_COST = 10;   	 // 行动力消耗
	const int GOLD_COST = 3000;       // 金消耗
	const int FOOD_COST = 6000;       // 金消耗
	const int TROOP_COST = 3000;       // 金消耗

	const int 封地巡防范围 = 6;//默认6，范围3-10

	int 地名_id = -1;

	const bool 调试模式 = false;
	class Main
	{
		pk::building@ building_;
		pk::force@ force_;
		pk::person@ kunshu_;
		pk::person@ actor;
		pk::city@ city_;
		pk::unit@ src_unit_;
		pk::point src_pos_;
		bool has_moved_ = false;

		string spec_name;
		int check_result_;
		pk::list<pk::building@> able_spec_;
		pk::list<pk::person@> able_person_;

		//array<array<int>> base_info(15, array<int>(5, -1));//最多一个势力存15据点，存（据点id，据点状态（前线，警戒），玩家部队数，敌势力部队数，5格内是否有敌部队）
		Main()
		{
			pk::bind(120, pk::trigger120_t(func_信息显示_地名信息));//数字越大越优先
			pk::bind(111, pk::trigger111_t(onTurnStart));
			pk::bind(202, pk::trigger202_t(onAIRunningOrder));
			pk::bind(152, pk::trigger152_t(onPersonForceChange));
			pk::set_func(63, pk::func63_t(GetBuildingHp));
			add_menu();
			add_unit_menu();
		}

		void add_menu()
		{
			pk::menu_item item;
			item.menu = 105;
			//item.pos = 3;
			item.shortcut = "";
			item.init = pk::building_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(isVisible);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		void add_unit_menu()
		{
			pk::menu_item item;
			item.menu = 1;
			//item.pos = 3;
			item.shortcut = "";
			item.init = cast<pk::unit_menu_item_init_t@>(function(unit, src_pos) { @main.src_unit_ = @unit; main.src_pos_ = src_pos; });
			item.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return (main.src_unit_).type == 部队类型_运输; });
			item.is_enabled = cast<pk::menu_item_is_enabled_t@>(function() { if (main.src_unit_.get_pos() != pk::get_unit_menu_hex_pos()) main.has_moved_ = true; return true;/* main.isEnabled(지원항목_금);*/ });
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("封地补给"); });
			item.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("向相邻的\x1b[1x封地\x1b[0x按1:2比例支援兵力兵粮."); });
			item.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets(1); });
			item.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.unit_handler(dst_pos); });
			//item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		//为府兵建筑增加耐久
		int GetBuildingHp(pk::building@ building, pk::facility@ facility)
		{
			int facility_id = facility.get_id();
			int max_hp = facility.hp;
			if (facility_id >= 设施_阵 and facility_id <= 设施_连弩楼)
			{
				int spec_id = get_spec_id(building);
				if (spec_id != -1)
				{
					max_hp += 300;
					if (pk::is_valid_person_id(special_ex[spec_id].person))
					{
						max_hp += 300;
					}

				}
			}
			return max_hp;
		}

		void func_信息显示_地名信息()
		{
			// 光标指的坐标
			pk::point cursor_pos = pk::get_cursor_hex_pos();
			if (!pk::is_valid_pos(cursor_pos)) return;

			// 光标上指示的建筑物
			pk::building@ building = pk::get_building(cursor_pos);
			if (building is null || building.facility < 3 || building.facility > 7) return;  //特殊地名才显示

			int id = ch::get_特殊地名_id(building, true);
			地名_id = id;
			//pk::trace(pk::format("地名id:{}", id));
			if (id == -1) return;
			//if (!building.is_player()) return;
			//if (!pk::is_player_controlled(building)) return;
			//if (building.get_force_id() != pk::get_current_turn_force_id()) return;


			string building_name = pk::decode(pk::get_name(building));

			string title = pk::format("封地信息(\x1b[1x{}\x1b[0x)", 特殊地名设施名称[id]);//后面改成地名

			int middle = int(pk::get_resolution().width) / 2;
			int left = middle - 200;
			int right = middle + 200;
			int top = 5;
			int bottom = top + 80;

			// pk::draw_rect(pk::rectangle(left, top, right, bottom), 0xff00ccff);
			specialinfo@ spec_t = @special_ex[id];

			pk::draw_text(pk::encode(title), pk::point(left + 5, top + 5), 0xffffffff, FONT_BIG, 0xff000000);

			//BaseInfo@ base_t = @base_ex[building_id];
			int person_id = spec_t.person;
			string person_name = "";
			if (person_id == -1) person_name = "无";
			else person_name = pk::decode(pk::get_name(pk::get_person(person_id)));

			string info_将军 = pk::format("将军: \x1b[1x{}\x1b[0x", person_name);

			pk::draw_text(pk::encode(info_将军), pk::point(left + 10, top + 40 + 军餉维护费::信息行数 * 20), 0xffffffff, FONT_SMALL, 0xff000000);
			if (person_id != -1)
			{
				string info_兵力 = pk::format("府兵: \x1b[1x{}\x1b[0x", spec_t.troops);
				string info_资金 = pk::format("资金: \x1b[1x{}\x1b[0x", spec_t.gold);
				string info_兵粮 = pk::format("兵粮: \x1b[1x{}\x1b[0x", spec_t.food);
				string info_统领 = pk::format("统领: \x1b[1x{}\x1b[0x", 3);//以后再考虑变化
				pk::draw_text(pk::encode(info_兵力), pk::point(middle + 10, top + 40 + 军餉维护费::信息行数 * 20), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::draw_text(pk::encode(info_资金), pk::point(left + 10, top + 40 + (军餉维护费::信息行数 + 1) * 20), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::draw_text(pk::encode(info_兵粮), pk::point(middle + 10, top + 40 + (军餉维护费::信息行数 + 1) * 20), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::draw_text(pk::encode(info_统领), pk::point(middle + 10, top + 20 + 军餉维护费::信息行数 * 20), 0xffffffff, FONT_SMALL, 0xff000000);
				军餉维护费::信息行数 += 2;
			}
			else 军餉维护费::信息行数 += 1;

		}

		//府兵进攻及分封的地方
		void onTurnStart(pk::force@ force)
		{
			if (!pk::is_alive(force)) return;
			//府兵进攻
			for (int i = 0; i < 地名_末; ++i)
			{
				pk::point pos = pk::point(特殊地名设施[i][1], 特殊地名设施[i][2]);
				pk::building@ building0 = pk::get_building(pos);

				if (!pk::is_alive(building0)) continue;
				if (调试模式) pk::trace("府兵 building id:" + building0.get_id());
				if (building0.get_force_id() != force.get_id()) continue;
				specialinfo@ spec_t = @special_ex[i];
				if (spec_t.person != -1)
				{
					//每回合ai府兵资源增加
					if (!building0.is_player() and spec_t.troops < 24000)
					{
						spec_t.troops += ch::randint(200, 400);
						spec_t.gold += ch::randint(200, 400);
						spec_t.food += ch::randint(400, 800);
					}
					pk::point dst_pos = get_enemy_unit_pos(building0);
					if (调试模式) pk::trace("府兵进攻0 building id:" + building0.get_id()+ ch::get_spec_name(building0.get_id()-100) + "dst_pos:" + dst_pos.x + "," + dst_pos.y);
					if (pk::is_valid_pos(dst_pos))
					{
						if (调试模式) pk::trace("府兵进攻 building id:" + building0.get_id() + ch::get_spec_name(building0.get_id() - 100) + "dst_pos:" + dst_pos.x + "," + dst_pos.y);
						func_府兵部队_生成(building0, pk::get_person(spec_t.person), dst_pos, i, spec_t.troops, spec_t.gold, spec_t.food, 3);
					}
				}
			}

		}

		void onPersonForceChange(pk::person@ person, pk::force@ force)
		{
			int person_id = person.get_id();
			for (int i = 0; i < 地名_末; ++i)
			{
				specialinfo@ spec_t = @special_ex[i];
				if (spec_t.person == person_id)
				{
					ch::reset_spec(i);
					person_ex[spec_t.person].spec_id = -1;
				}
			}
		}

		void onAIRunningOrder(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 据点AI_分封)
			{
				if (pk::get_troops(building) <= (15000 + TROOP_COST)) return;
				else if (pk::get_food(building) <= 2 * FOOD_COST) return;
				else if (pk::get_gold(building) <= 2 * GOLD_COST) return;


				pk::list<pk::person@> able_person = get_able_person(pk::get_force(building.get_force_id()));
				pk::list<pk::building@> able_spec = get_city_able_spec_building(building);

				if (run_order_before(building, able_person, able_spec)) run_order(building, able_person[0], get_spec_id(able_spec[0]), false);
				else if (调试模式) pk::trace("据点AI_分封 不满足");
			}
			else if (cmd == 据点AI_解除分封)
			{
				pk::list<pk::person@> spec_person = get_city_spec_person(building);
				if (spec_person.count != 0)
				{
					run_order_解除分封(building, spec_person, false);
				}
			}
		}

		bool run_order_before(pk::building@ building0, pk::list<pk::person@>& able_person, pk::list<pk::building@>& able_spec)
		{
			int check_result = check_avaliable(building0, able_person, able_spec);
			if (check_result != 0) return false;
			//在此处选好目标
			pk::person@ person0 = able_person[0];
			able_person.clear();
			able_person.add(person0);

			pk::building@ building_spec = able_spec[0];
			able_spec.clear();
			able_spec.add(building_spec);

			return true;
		}

		bool run_order(pk::building@ building0, pk::person@ actor, int spec_id, bool is_player = true)
		{
			specialinfo@ spec_t = @special_ex[spec_id];
			if (pk::is_valid_person_id(spec_t.person)) { person_ex[spec_t.person].spec_id = -1; }//清空原来武将的对应记录
			int person_id = actor.get_id();
			if (pk::is_valid_person_id(person_id) and is_player)//清空原封地将领信息
			{
				int actor_spec_id = person_ex[person_id].spec_id;
				if (ch::is_valid_spec_id(actor_spec_id))
				{
					special_ex[actor_spec_id].person = -1;
				}
			}
			spec_t.person = person_id;
			spec_t.gold = is_player ? GOLD_COST : GOLD_COST * 2;
			spec_t.food = is_player ? FOOD_COST : FOOD_COST * 2;
			spec_t.troops = is_player ? TROOP_COST : TROOP_COST * 2;
			person_ex[person_id].spec_id = spec_id;
			// 计算行动力消耗
			auto district = pk::get_district(building0.get_district_id());
			pk::add_ap(district, -ACTION_COST);

			// 计算金消耗
			pk::add_gold(building0, -GOLD_COST, true);
			pk::add_food(building0, -FOOD_COST, true);
			ch::add_troops(building0, -TROOP_COST, true);

			//人物忠诚上升
			pk::add_loyalty(actor, 10);
			spec_name = 特殊地名设施名称[spec_id];


			if (!is_player)
			{
				string t = pk::format("\x1b[2x{}\x1b[0x获得了封地\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(pk::get_person(person_id))),spec_name); //"\x1b[2x" +  + pk::encode("\x1b[0x获得了封地\x1b[1x" + spec_name + "\x1b[0x");
				pk::history_log(building0.pos, pk::get_force(building0.get_force_id()).color, pk::encode(t));
			}
			else pk::scene(pk::scene_t(scen_分封));
			return true;
		}

		bool run_order_解除分封(pk::building@ building0, pk::list<pk::person@> spec_person, bool is_player = true)
		{
			for (int i = 0; i < spec_person.count; ++i)
			{
				int person_id = spec_person[0].get_id();
				int spec_id = person_ex[person_id].spec_id;
				if (!ch::is_valid_spec_id(spec_id)) continue;
				person_ex[person_id].spec_id = -1;
				specialinfo@ spec_t = @special_ex[spec_id];
				spec_t.person = -1;
				int gold = spec_t.gold;
				int food = spec_t.food;
				int troops = spec_t.troops;
				spec_t.gold = 0;
				spec_t.food = 0;
				spec_t.troops = 0;
				pk::building@spec_building = pk::get_building(spec_id + 100);
				spec_building.hp = pk::max(spec_building.hp-300,0);
				pk::add_gold(building0, gold, false);
				pk::add_food(building0, food, false);
				pk::add_troops(building0, troops, false);
				spec_name = 特殊地名设施名称[spec_id];
				string t = pk::format("\x1b[2x{}\x1b[0x被解除了封地\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(pk::get_person(person_id))), spec_name); //"\x1b[2x" +  + pk::encode("\x1b[0x获得了封地\x1b[1x" + spec_name + "\x1b[0x");
				pk::history_log(building0.pos, pk::get_force(building0.get_force_id()).color, pk::encode(t));
			}
			return true;
		}
		//========================================玩家分封=================================================================//

		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@kunshu_ = pk::get_person(pk::get_kunshu_id(building));
			@city_ = pk::building_to_city(building);
			able_spec_ = get_able_building(force_);
			able_person_ = get_able_person(force_);
			if (调试模式) pk::trace("able_spec_" + able_spec_.count + "able_person_" + able_person_.count);
			check_result_ = check_avaliable(building_, able_person_, able_spec_);
		}

		bool isVisible()
		{
			if (building_.get_id() != kunshu_.service) return false;
			return true;
		}

		bool isEnabled()
		{
			if (check_result_ != 0) return false;
			else return true;
		}

		string getDesc()
		{
			switch (check_result_)
			{
			case 1: return pk::encode("没有可分封的武将. ");
			case 2: return pk::encode("没有可分封的封地.");
			case 3: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			case 4: return pk::encode(pk::format("兵力不足.(必须{} 气力)", TROOP_COST));
			case 5: return pk::encode(pk::format("食物不足.(必须{} 兵粮)", FOOD_COST));
			case 6: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));

			case 0: return pk::encode(pk::format("执行分封. (行动力 {}, 金 {}, 粮 {}, 兵 {})", ACTION_COST, GOLD_COST, FOOD_COST, TROOP_COST));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		int check_avaliable(pk::building@ building, pk::list<pk::person@> able_person, pk::list<pk::building@> able_spec)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{

			if (able_person.count == 0) return 1;
			else if (able_spec.count == 0) return 2;
			else if (pk::get_district(building.get_district_id()).ap < ACTION_COST) return 3;
			else if (pk::get_troops(building) <= TROOP_COST) return 4;
			else if (pk::get_food(building) <= FOOD_COST) return 5;
			else if (pk::get_gold(building) <= GOLD_COST) return 6;

			//pk::list<pk::person@> able_person = get_able_person(force_);
			else return 0;
		}

		string getText()
		{
			return pk::encode("分封");//刺杀
		}

		bool handler()
		{
			if (pk::choose({ pk::encode(" 是 "), pk::encode(" 否 ") }, pk::encode(pk::format("是否执行分封。\n(仅可将\x1b[18x未分封\x1b[0x领地封给\x1b[18x统率80以上\x1b[0x武将)\n(消耗行动力\x1b[1x{}\x1b[0x,金\x1b[1x{}\x1b[0x,粮\x1b[1x{}\x1b[0x,兵\x1b[1x{}\x1b[0x)", ACTION_COST, GOLD_COST, FOOD_COST, TROOP_COST)), kunshu_) == 1)
				return false;

			//pk::list<pk::person@> person_list = get_able_person(force_);//获得可以分封的武将数

			pk::list<pk::city@> city_list = get_able_city(force_);//获得可以分封的武将数
			if (city_list.count == 0)
			{
				pk::message_box(pk::encode("没有未分封的封地了。"), kunshu_);
				return false;
			}
			pk::list<pk::person@> pre_sel_person;
			pre_sel_person.add(able_person_[0]);
			//分封武将选擇
			pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("分封武将选择"), pk::encode("选择要分封的武将."), able_person_, 1,1, pre_sel_person, 42);
			if (person_sel.count == 0) return false; // 未选擇时取消结束
			@actor = person_sel[0];
			int person_id = person_sel[0].get_id();

			//!!!!看不到封地名，体验不好，或者做成选项？但是封地数量会太多
			// 选擇目标封地
			pk::list<pk::city@> list_target = pk::city_selector2(pk::encode("选择封地对应城市"), pk::encode("选择封地对应城市."), city_list, 1, 1);
			if (list_target.count == 0) return false; // 未选擇时取消结束

			array<int> spec_arr = get_city_able_spec(list_target[0], force_);

			int spec_id = choose_spec(spec_arr);

			return run_order(building_, actor, spec_id, true);

		}

		void scen_分封()
		{
			pk::play_bgm(17);
			pk::message_box(pk::encode(pk::format("诸位，请听我说！\n从现在开始，我将\x1b[2x{}\x1b[0x分封给我军麾下大将：\x1b[1x{}\x1b[0x，世袭罔替！",spec_name, pk::decode(pk::get_name(actor)))), kunshu_);

			pk::message_box(pk::encode(pk::format("{}待{}恩重如山！\n{}必将效死！", pk::decode(pk::get_msg(pk::msg_param(46, actor, kunshu_))), pk::decode(pk::get_msg(pk::msg_param(0, actor, kunshu_))), pk::decode(pk::get_msg(pk::msg_param(0, actor, kunshu_))))), actor);

			pk::cutin(CG_气势);
			spec_name = "";
		}

		//========================================玩家支援封地=================================================================//
		array<pk::point_int> get_targets(int max)
		{
			array<pk::point_int> targets;
			auto range = pk::range(src_pos_, max == 0 ? 0 : 1, max);
			for (int i = 0; i < int(range.length); i++)
			{
				auto hex = pk::get_hex(range[i]);
				if (hex.has_building)
				{
					auto building = pk::get_building(range[i]);
					if (building.get_force_id() != src_unit_.get_force_id()) continue;
					int spec_id = ch::to_spec_id(building.get_id());
					if (ch::is_valid_spec_id(spec_id))
						targets.insertLast(pk::point_int(range[i], 0));
				}
			}
			return targets;
		}

		bool unit_handler(pk::point dst_pos)
		{
			if (调试模式) pk::trace("supply spec");
			pk::building@ dst_building = pk::get_building(dst_pos);
			int spec_id = ch::to_spec_id(dst_building.get_id());
			string menu_desc = "请决定为" + ch::get_spec_name(spec_id) + "支援" + "的物资数量.";
			int support_limit = pk::min(src_unit_.troops -1, (src_unit_.food -1)/2);
			menu_desc += "\n(支援最大物资值:\x1b[1x" + support_limit + "\x1b[0x)";
			pk::message_box(pk::encode(menu_desc));

			string numberpad_title = pk::encode("支援数量");
			pk::int_bool numberpad_value = pk::numberpad(numberpad_title, 1, support_limit, 1, cast<pk::numberpad_t@>(function(line, original_value, current_value) { return ""; }));
			/* if (!numberpad_value.second)
			{
				pk::set_pos(src_unit, pos_0);
				if (물자지원_실행부대_행동완료) src_unit.action_done = false;
				src_unit.update();
				return false;
			} */
			if (!numberpad_value.second)
			{
				//if (물자지원_실행부대_행동완료 and pos_0 != src_pos_) src_unit.action_done = true;  //移动了
				if (调试模式) pk::trace("has_moved_" + has_moved_);
				if (has_moved_) src_unit_.action_done = true;
				return false;
			}

			pk::person@ src_leader = pk::get_person(src_unit_.leader);
			int support_amount = numberpad_value.first;
			int get_amout = numberpad_value.first;
			string confirm_desc = "是否确定为" + ch::get_spec_name(spec_id) + "提供\x1b[1x" + support_amount + "兵力\x1b[0x吗?";
			int confirm_value = pk::choose(pk::encode(confirm_desc), { pk::encode("确定"), pk::encode("取消") });

			if (confirm_value == 1)
			{
				if (调试模式) pk::trace("has_moved_" + has_moved_);
				if (has_moved_) src_unit_.action_done = true;
				return false;
			}

			pk::add_food(src_unit_, -support_amount*2, true);
			pk::add_troops(src_unit_, -support_amount, true);
			specialinfo@ spec_t = @special_ex[spec_id];
			spec_t.food += support_amount * 2;
			spec_t.troops += support_amount;
			pk::combat_text(support_amount, 1, dst_pos);
			src_unit_.action_done = true;

			return true;
		}
		//================================================支持函数================================================//
		int choose_spec(array<int> spec_arr)
		{
			//一个城市最多8个？
			// 头5+1 中间4+2 尾部 n+1
			int num = int(spec_arr.length);
			int choose_times = (num - 5) / 4;//除法直接去掉小数
			int mod = (num - 5) % 4;//取余数做末尾
			//要获取的是spec_id
			array<string> spec_name_list;
			for (int i = 0; i < pk::min(5,num); ++i)
			{
				spec_name_list.insertLast(pk::encode(get_spec_city_name(spec_arr[i])+特殊地名设施名称[spec_arr[i]]));
			}

			if (num == 6) spec_name_list.insertLast(pk::encode(get_spec_city_name(spec_arr[num-1]) + 特殊地名设施名称[spec_arr[num - 1]]));
			else if (num > 6) spec_name_list.insertLast(pk::encode("下一页"));

			int n = pk::choose(pk::encode("请选择封地."), spec_name_list);
			if (n == 5 and num!=6) return choose_spec_mid(spec_arr,1);
			return spec_arr[n];
		}

		int choose_spec_mid(array<int> spec_arr,int page)
		{
			//一个城市最多8个？
			// 头5+1 中间4+2 尾部 n+1
			int num = int(spec_arr.length);
			int num2 = (num - 1 - 4 * page);
			if (num2 <= 5)
			{
				//最后一页
				//要获取的是spec_id
				array<string> spec_name_list;
				for (int i = (1+page*4); i < (1 + page * 4 + num2); ++i)
				{
					spec_name_list.insertLast(pk::encode(get_spec_city_name(spec_arr[i]) + 特殊地名设施名称[spec_arr[i]]));
				}
				spec_name_list.insertLast(pk::encode("上一页"));
				int n = pk::choose(pk::encode("请选择封地."), spec_name_list);

				if (n == 5 or n == int(spec_name_list.length -1))
				{
					if (page == 1) return choose_spec(spec_arr);
					return choose_spec_mid(spec_arr, page - 1);//如何返回上一页
				}

				int t = n + (page - 1) * 4 + 5;

				return spec_arr[t];
			}
			else//不是最后一页
			{
				//要获取的是spec_id
				array<string> spec_name_list;
				for (int i = (1 + page * 4); i < (1 + page * 4 + 4); ++i)
				{
					spec_name_list.insertLast(pk::encode(get_spec_city_name(spec_arr[i]) + 特殊地名设施名称[spec_arr[i]]));
				}
				spec_name_list.insertLast(pk::encode("上一页"));
				spec_name_list.insertLast(pk::encode("下一页"));

				int n = pk::choose(pk::encode("请选择封地."), spec_name_list);

				//非最后一页情况
				if (n == 4)
				{
					if (page!= 1) return choose_spec_mid(spec_arr, page - 1);
					else return choose_spec(spec_arr);
				}
				if (n == 5) return choose_spec_mid(spec_arr, page + 1);

				int t = n + (page - 1) * 4 + 5;

				return spec_arr[t];
			}

		}

		string get_spec_city_name(int spec_id)
		{
			pk::point pos0 = pk::point(特殊地名设施[spec_id][1], 特殊地名设施[spec_id][2]);
			int city_id = pk::get_city_id(pos0);
			if (city_id == -1) return "";
			return pk::decode(pk::get_name(pk::get_city(city_id)));
		}

		int get_spec_id(pk::building@ building)
		{
			for (int i = 0; i < 地名_末; ++i)
			{
				pk::point pos0 = pk::point(特殊地名设施[i][1], 特殊地名设施[i][2]);
				//pk::building@ building0 = pk::get_building(pos0);
				if (building.pos == pos0)
				{
					return i;
				}
			}
			return -1;
		}

		array<int> get_all_spec()
		{
			array<int> temp;
			for (int i = 0; i < 地名_末; ++i)
			{
				temp.insertLast(i);
			}
			return temp;
		}

		pk::list<pk::building@> get_city_able_spec_building(pk::building@ building)
		{
			pk::list<pk::building@> temp;
			for (int i = 0; i < 地名_末; ++i)
			{
				pk::point pos0 = pk::point(特殊地名设施[i][1], 特殊地名设施[i][2]);
				pk::building@ building0 = pk::get_building(pos0);
				if (building0 is null) continue;
				if (building0.get_force_id() != building.get_force_id()) continue;
				specialinfo@ spec_t = @special_ex[i];
				if (调试模式) pk::trace(pk::format("0 {} {}", get_spec_city_name(i), 特殊地名设施名称[i]));
				if (spec_t.person != -1) continue;
				int city_id = pk::get_city_id(pos0);
				if (调试模式) pk::trace(pk::format("1 {} {}", get_spec_city_name(i), 特殊地名设施名称[i]));
				if (building.get_id() != city_id) continue;
				if (调试模式) pk::trace(pk::format("2 {} {}", get_spec_city_name(i), 特殊地名设施名称[i]));
				temp.add(building0);//insertLast(i);
			}
			return temp;
		}

		array<int> get_city_able_spec(pk::city@ city,pk::force@ force)
		{
			array<int> temp;
			for (int i = 0; i < 地名_末; ++i)
			{
				pk::point pos0 = pk::point(特殊地名设施[i][1], 特殊地名设施[i][2]);
				pk::building@ building0 = pk::get_building(pos0);
				if (building0 is null) continue;
				if (building0.get_force_id() != force.get_id()) continue;
				specialinfo@ spec_t = @special_ex[i];
				if (调试模式) pk::trace(pk::format("0 {} {}", get_spec_city_name(i), 特殊地名设施名称[i]));
				if (spec_t.person != -1) continue;
				int city_id = pk::get_city_id(pos0);
				if (调试模式) pk::trace(pk::format("1 {} {}", get_spec_city_name(i), 特殊地名设施名称[i]));
				if (city.get_id() != city_id) continue;
				if (调试模式) pk::trace(pk::format("2 {} {}", get_spec_city_name(i), 特殊地名设施名称[i]));
				temp.insertLast(i);
			}
			return temp;
		}

		pk::list<pk::city@> get_able_city(pk::force@ force)
		{
			pk::list<pk::city@> temp;
			array<int> id_temp;
			for (int i = 0; i < 地名_末; ++i)
			{
				pk::point pos0 = pk::point(特殊地名设施[i][1], 特殊地名设施[i][2]);
				auto hex = pk::get_hex(pos0);
				if (!hex.has_building) continue;
				pk::building@ building0 = pk::get_building(pos0);

				if (building0.get_force_id() != force.get_id()) continue;
				specialinfo@ spec_t = @special_ex[i];
				if (spec_t.person != -1) continue;
				int city_id = pk::get_city_id(pos0);
				//if (pk::get_city(city_id).get_force_id() != force.get_id()) continue;
				if (id_temp.find(city_id) < 0 and city_id != -1)
				{
					id_temp.insertLast(city_id);
					temp.add(pk::get_city(city_id));
				}
			}
			return temp;
		}

		pk::list<pk::building@> get_able_building(pk::force@ force)
		{
			pk::list<pk::building@> temp;
			for (int i = 0; i < 地名_末; ++i)
			{
				pk::point pos0 = pk::point(特殊地名设施[i][1], 特殊地名设施[i][2]);
				auto hex = pk::get_hex(pos0);
				if (!hex.has_building) continue;
				pk::building@ building0 = pk::get_building(pos0);
				if (building0.get_force_id() != force.get_id()) continue;
				specialinfo@ spec_t = @special_ex[i];
				if (spec_t.person != -1) continue;
				temp.add(building0);
			}
			return temp;
		}

		pk::list<pk::person@> get_able_person(pk::force@ force)
		{
			pk::list<pk::person@> temp;
			pk::list<pk::person@> person_list = pk::get_person_list(force, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般));
			for (int i = 0; i < person_list.count; ++i)
			{
				if (person_list[i].stat[武将能力_统率] > 80)
				{
					if (调试模式) pk::trace(pk::format("spec_id:{},person_id:{}", person_ex[person_list[i].get_id()].spec_id, person_ex[person_list[i].get_id()]));
					if (ch::is_valid_spec_id(person_ex[person_list[i].get_id()].spec_id)) continue;//重复分封的原因，但之前为什么注释掉呢
					temp.add(person_list[i]);
					if (调试模式) pk::trace(pk::format("势力id：{}，武将：{}，统率：{}，spec_id:{}",force.get_id(),pk::decode(pk::get_name(person_list[i])), person_list[i].stat[武将能力_统率], person_ex[person_list[i].get_id()].spec_id));
				}
			}
			if (temp.count > 1)
			{
				temp.sort(function(a, b)
				{
					return a.stat[武将能力_统率] > b.stat[武将能力_统率];  // 按统率排序
				});
			}
			return temp;

		}

		pk::list<pk::person@> get_city_spec_person(pk::building@ building)
		{
			pk::list<pk::person@> temp;
			for (int i = 0; i < 地名_末; ++i)
			{
				specialinfo@ spec_t = @special_ex[i];
				if (spec_t.person == -1) continue;
				pk::point pos0 = pk::point(特殊地名设施[i][1], 特殊地名设施[i][2]);
				int city_id = pk::get_city_id(pos0);
				if (city_id != building.get_id()) continue;
				temp.add(pk::get_person(spec_t.person));

			}
			return temp;

		}

		pk::point get_enemy_unit_pos(pk::building@ building)
		{
			//pk::unit@ dst_unit;
			int range_max = pk::min(10, pk::max(3, 封地巡防范围));
			auto range = pk::range(building.pos, 1, range_max);
			//pk::trace(pk::format("封地范围:{},数量:{}",range_max,range.length));
			for (int i = 0; i < int(range.length); i++)
			{
				auto hex = pk::get_hex(range[i]);
				if (hex.has_unit)
				{
					pk::unit@ unit = pk::get_unit(range[i]);
					//pk::trace(pk::format("1 unit_id:{}", unit.get_id()));
					if (pk::is_alive(unit) and unit.type_id == 11)
					{
						//pk::trace(pk::format("2 unit_id:{}",unit.get_id()));
						int building_id = pk::get_building_id(range[i]);
						if (pk::get_building(building_id).get_force_id() != building.get_force_id()) continue;//不属于己方防区的情况

						if (pk::is_enemy(building, unit))
						{
							//pk::trace(pk::format("3 unit_id:{}", unit.get_id()));
							return unit.pos;
						}
					}
				}

			}
			return pk::point(-1, -1);
		}

		pk::point get_empty_pos(pk::point pos, int distance_min, int distance_max)
		{
			array<pk::point> range_pos_arr = pk::range(pos, distance_min, distance_max);
			for (int arr_index = 0; arr_index < int(range_pos_arr.length); arr_index++)
			{
				pk::point range_pos = range_pos_arr[arr_index];
				if (!pk::is_valid_pos(range_pos)) continue;

				pk::hex@ hex = pk::get_hex(range_pos);
				if (hex.has_building) continue;
				if (hex.has_unit) continue;

				int terrain_id = hex.terrain;
				if (!pk::is_valid_terrain_id(terrain_id)) continue;
				if (!pk::is_enabled_terrain(terrain_id)) continue;

				return range_pos;
			}

			return pk::point(-1, -1);
		}

		//-----------------------府兵部队队生成相关-------------------------------------//
		//基本事件_府兵部队_生成
		void func_府兵部队_生成(pk::building@ building, pk::person@ src_person, pk::point dst_pos, int spec_id, int troops, int gold, int food, int num)
		{
			pk::point unit_pos = get_empty_pos(building.pos, 1, 1);
			if (!pk::is_valid_pos(unit_pos)) return;
			if (!pk::is_in_screen(dst_pos))
			{
				auto hex = pk::get_hex(dst_pos);
				if (hex.has_unit)
				{
					pk::unit@unit0 = pk::get_unit(dst_pos);
					if (!unit0.is_player())
					{
						int chance = pk::rand(100);
						if (chance > 70) num = 0;
						else if (chance > 20) num = 1;
						else num = 2;
					}
				}
			}

			for (int count = 0; count < num; count++)
			{
				if (!func_person_slot_available()) break;//沒有空余武将位则不生成
				if (count != 0) dst_pos = get_enemy_unit_pos(building);
				if (dst_pos.x == -1) break;
				pk::person@ f_person = func_create_person(src_person);
				if (!pk::is_alive(f_person)) { pk::reset(f_person); continue; }
				int src_troops = get_best_troops(troops, num - count);
				if (src_troops < 3000) { pk::reset(f_person); break; }//小于3000不再出兵
				troops -= src_troops;//已出征过一次的兵需要休息，不再再次出征
				pk::unit@ src_unit = func_create_unit(building, f_person, unit_pos, spec_id, num/*统领数*/, src_troops, gold, food);
				//pk::trace("src_unit force" + src_unit.get_force_id() + ",f_person: force:" + f_person.get_id() + f_person.get_force_id());
				if (!pk::is_alive(src_unit)) { pk::kill(src_unit); pk::reset(f_person); continue; }
				if (int(pk::get_movable_pos(src_unit).length) == 0) { kill(src_unit, spec_id); pk::reset(f_person); break; }

				pk::set_order(src_unit, 部队任务_攻击, dst_pos);
				pk::run_order(src_unit);
				//if (!pk::run_order(src_unit)) { kill(src_unit, spec_id); pk::reset(f_person); continue; }
				src_unit.action_done = false;
				if ((pk::get_max_hp(building) - building.hp) > 0)
				{
					pk::set_order(src_unit, 部队任务_修复, building.pos, building.get_id());
				}
				else
				{
					//无需修复的情况
					pk::point next_pos = get_empty_pos(building.pos, 1, 1);
					pk::set_order(src_unit, 部队任务_移动, next_pos);
				}
				pk::run_order(src_unit);

				if (pk::is_in_screen(src_unit.get_pos()))
				{
					switch (pk::rand(3))
					{
					case 0: pk::say(pk::encode("任务完成, 返回封地吧!"), pk::get_person(src_unit.leader)); break;
					case 1: pk::say(pk::encode("兄弟们, 回家吧!"), pk::get_person(src_unit.leader)); break;
					case 2: pk::say(pk::encode("干的不错，回去大人必有赏赐!"), pk::get_person(src_unit.leader)); break;
					}
				}

				kill(src_unit, spec_id);

				pk::reset(f_person);
			}
		}

		void kill(pk::unit@ unit, int spec_id)
		{
			specialinfo@ spec_t = @special_ex[spec_id];
			spec_t.troops += unit.troops;
			spec_t.gold += unit.gold;
			spec_t.food += unit.food;
			pk::kill(unit, true);
		}

		//遍历所有武将，身??死亡或无的都可用来生成
		bool func_person_slot_available()
		{
			for (int person_id = 敌将_开始; person_id < 敌将_末; person_id++)
			{
				pk::person@ person = pk::get_person(person_id);
				if (person.mibun == 身份_无) return true;
				if (person.mibun == 身份_死亡) return true;
			}
			return false;
		}

		//统领武将生成
		pk::person@ func_create_person(pk::person@ person)
		{
			pk::person@ f_person = pk::create_bandit(pk::get_person(武将_卫士));
			f_person.name_read = pk::encode("府兵");
			pk::set_district(f_person, person.get_district_id());

			f_person.base_stat[武将能力_统率] = pk::max(70, pk::min(90, int(person.base_stat[武将能力_统率] * 0.85f + ch::randint(5, 10))));
			f_person.base_stat[武将能力_武力] = pk::max(70, pk::min(90, int(person.base_stat[武将能力_武力] * 0.85f + ch::randint(5, 10))));
			f_person.base_stat[武将能力_智力] = 70 + ch::randint(5, 10);
			for (int heishu_id = 兵种_枪兵; heishu_id < 兵种_末; heishu_id++)
				f_person.tekisei[heishu_id] = person.tekisei[heishu_id] + ch::randint(-2, 1);
			f_person.rank = pk::min(81, person.rank + 8);
			f_person.skill = 特技_突袭;
			f_person.update();

			return f_person;
		}

		//府兵部队生成-改
		pk::unit@ func_create_unit(pk::building@ src_base, pk::person@ f_person, pk::point unit_pos, int spec_id, int num/*统领数*/, int troops, int gold, int food)
		{

			int ground_wpn = get_best_ground_wpn(f_person);//陆上兵器
			int sea_wpn = 兵器_走舸;//水上兵器
			int src_troops = troops;
			int src_gold = int(gold / num);
			int src_food = int(food / num);
			specialinfo@ spec_t = @special_ex[spec_id];
			spec_t.troops -= src_troops;
			spec_t.gold -= src_gold;
			spec_t.food -= src_food;
			pk::unit@ src_unit = pk::create_unit(src_base, f_person, null, null, src_troops, ground_wpn, sea_wpn, src_gold, src_food, unit_pos);

			src_unit.energy = 100;

			return src_unit;
		}

		int get_best_ground_wpn(pk::person@ person)
		{
			/*
			const int 兵器_枪 = 1;
const int 兵器_戟 = 2;
const int 兵器_弩 = 3;
const int 兵器_战马 = 4;
			 */
			int best_weapon = -1;
			int best_heishu = -1;
			for (int weapon_id = 1; weapon_id < 5; weapon_id++)
			{

				if (person.tekisei[weapon_id - 1] > best_heishu)
				{
					best_heishu = person.tekisei[weapon_id - 1];
					best_weapon = weapon_id;
				}
				//pk::trace(pk::format("weapon id :{}", weapon_id));
			}
			return best_weapon;
		}

		int get_best_troops(int base_troops, int num)
		{
			int i = num;
			do
			{
				if (base_troops / i >= 3000)
				{
					if (调试模式) pk::trace(pk::format("{},{},{}",i, base_troops / i,base_troops));
					return pk::min(8000, base_troops / i);//最多8000,最少3000
				}
				i -= 1;
			} while (i >= 1);
			return base_troops;
		}

		/// 范围随机值 :
		int func_数组_范围_随机值(array<int> int_value_arr)
		{
			pk::random random(pk::rand());
			return random(int_value_arr[0], int_value_arr[1]);
		}
	}
	Main main;
}

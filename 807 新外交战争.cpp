// ## 2022/03/28 # 江东新风 # 数组越界修复2，尝试修复复制人 ##
// ## 2022/03/23 # 江东新风 # 数组越界修复 ##
// ## 2021/11/30 # 江东新风 # 基本完工 ##
// ## 2021/11/25 # 江东新风 # 重新新写的外交战争 ##
/*
包含内容
ai的同盟请求--√
玩家的组建联军---√
ai的组建联军--√
ai的联军出征--√
玩家为联盟出征---
朝贡设定---暂不考虑
联军的崩溃---√(时间原因崩溃，成员减少崩溃，盟主势力灭亡，目标势力灭亡)
每旬根据各种数据增减友好度--√
*/
namespace 新外交战争
{
	const int 最低城市数 = 4;//成为被反联盟对象的最低城市数要求
	const int 保护期 = 12;//月数
	const int 联盟最小势力数 = 3;//如果势力数小于此数值，不触发联盟
	const int 联盟持续时间 = 72; //旬数
	const bool 联盟资源奖励 = true;
	const int 联合军出征行动力 = 20;//无论出征多少兵力，只消耗该行动力
	//玩家组建联军相关
	const int ACTION_COST = 40;    // 组建联军行动力消耗
	const int GOLD_COST = 1500;     // 组建联军金消耗

	class Main
	{
		bool 调试模式 = false;

		string leader_kunshu_name;
		string target_kunshu_name;
		string joined_force_name;

		//玩家菜单相关
		pk::building@ building_;
		pk::force@ force_;
		pk::person@ kunshu_;
		int player_alliance_id;
		array<array<int>> player_possible_alliance;
		array<int> player_top_5_force;

		Main()
		{
			pk::bind(107, pk::trigger107_t(onNewDay));
			pk::bind(108, pk::trigger108_t(onNewMonth));
			pk::bind(111, pk::trigger111_t(onTurnStart));
			pk::bind(131, pk::trigger131_t(onForceRemove));
			pk::bind(202, pk::trigger202_t(onAIRunningOrder));
			//pk::bind(102, -1, pk::trigger102_t(onScenInit));//临时测试用
			initMenuItem();
		}

		void initMenuItem()
		{	
			pk::menu_item item;
			item.menu = 103;
			item.pos = 7;
			item.shortcut = global_menu::shortcut_成立联军;
			item.init = pk::building_menu_item_init_t(initMenu);
			item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
			item.get_text = pk::menu_item_get_text_t(getMenuText);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_desc = pk::menu_item_get_desc_t(getMenuDesc);
			// item.get_image_id = pk::menu_item_get_image_id_t(getMenuImageID);
			item.handler = pk::menu_item_handler_t(menuHandler);
			pk::add_menu_item(item);

		}

		void onForceRemove(pk::force@ force)
		{
			int force_id = force.get_id();
			if (!pk::is_valid_normal_force_id(force_id)) return;
			if (setting_ex.alliance_target_id.find(force_id) > -1) { AllianceCollapes(alliance_id, get_member_arr(alliance_id), true, 1); }//一个势力可以又是一个联盟的目标又是另一个盟主
			int alliance_id = force_ex[force_id].alliance_id;
			if (alliance_id > 3) return;
			if (setting_ex.alliance_leader_id.find(force_id) > -1)
			{
				BreakfromAlliance(force_id);
				AllianceCollapes(alliance_id, get_member_arr(alliance_id),true,2);
			}
			return;
		}

		void onTurnStart(pk::force@ force)
		{
			if (pk::is_alive(force))
			{
				int force_id = force.get_id();
				if (force_ex[force_id].attack_delay > 0 and force_ex[force_id].attack_delay < 10) {
					force_ex[force_id].attack_delay -= 1;
					if (调试模式) pk::trace("距离联合军出征还剩下：" + force_ex[force_id].attack_delay + "回合");
				}
			}
		}

		void onScenInit()
		{
			for (int i = 0; i < 城市_末; ++i)
			{
				pk::get_city(i).name = pk::encode(pk::get_old_base_name(i));
			}
			for (int i = 据点_关卡开始; i < 据点_关卡末; ++i)
			{
				pk::gate@ gate = pk::building_to_gate(pk::get_building(i));
				if (pk::is_alive(gate)) gate.name = pk::encode(pk::get_old_base_name(i));
				
			}
			for (int i = 据点_港口开始; i < 据点_港口末; ++i)
			{
				pk::port@ port = pk::building_to_port(pk::get_building(i));
				if (pk::is_alive(port)) port.name = pk::encode(pk::get_old_base_name(i));
			}
		}

		void onNewMonth()//要不要改成每月？
		{
			if (pk::get_elapsed_months() < 保护期) return;
			if (!pk::rand_bool(30)) return;//30概率触发组建联军
			int alliance_id = get_alliance_id();
			if (alliance_id < 0 and alliance_id > 3) return;//已存在4个联盟的情况不再进行
			//联军排除玩家势力，排除已经组成联军的势力
			//已组成联军的势力扩张？
			//寻找区域最大的势力，地图最大的势力
			//城市数4个及以上才会成为目标，武将数*城市数作为判断依据，外交关系作为是否加入联军的依据
			//pk::trace("start");//性能极好，0.001s
			array<int> force_city_count = get_force_city_count();
			array<int> force_person_count = get_force_person_count();
			array<int> force_troop_count = get_force_troop_count();
			array<int> top_5_force = get_top_n(force_city_count, force_person_count, 5);
			//auto top_5_city_force = get_top_n(force_city_count, 5);
			//auto top_5_person_force = get_top_n(force_person_count,5);
			array<array<int>> possible_alliance = get_possible_alliance(top_5_force);
			//挑选前3势力，如果选的不是第一的势力，参与联盟的不要加入靠前的势力
			//搜寻势力所有城市对应的邻近势力，确定可参与联合军的势力数
			//pk::trace("end");
			if (false and 调试模式)
			{
				string t;
				for (int i = 0; i < 非贼势力_末; ++i)
				{
					t += "势力:" + i + ",城市数:" + force_city_count[i] + ",武将数:" + force_person_count[i] + "\n";

				}
				pk::trace(t);
				for (int i = 0; i < int(possible_alliance.length); ++i)
				{
					t += "城市第" + i + "势力:" + pk::decode(pk::get_name(pk::get_force(top_5_force[i]))) + ",城市数:" + force_city_count[top_5_force[i]] + ",武将数:" + force_person_count[top_5_force[i]] + "\n";
					t += "可能敌势力数:" + possible_alliance[i][0];
					for (int j = 1; j < int(possible_alliance[i].length); ++j)
					{
						if (pk::is_valid_normal_force_id(possible_alliance[i][j])) t += "," + j + ":" + pk::decode(pk::get_name(pk::get_force(possible_alliance[i][j])));
						
					}
					t += "\n";
				}
				pk::trace(t);
			}
			//保护期 第一年不发生，最低城市数4个？
			//组成联盟的数量根据盟主的爵位，统政智魅(君主和军师取最高)
			for (int i = 0; i < int(possible_alliance.length); ++i)//排除不满足的势力
			{
				if (possible_alliance[i][0] < 联盟最小势力数)
				{
					if (调试模式) pk::trace("城市第" + i + "势力:" + pk::decode(pk::get_name(pk::get_force(top_5_force[i]))) + "势力数：" + possible_alliance[i][0] + ",i:" + i + ",top_5_forcelength1:" + top_5_force.length + ",possible_alliancelength2:" + possible_alliance.length);
					possible_alliance.removeAt(i);
					top_5_force.removeAt(i);
					i -= 1;//重要移除后数组序号会发生变化
					continue;
				}
				for (int j = 0; j < int(possible_alliance[i].length); ++j)//第一位是数量
				{
					if (possible_alliance[i][j] == -1) { possible_alliance[i].removeAt(j); j -= 1; };
				}
			}
			if (possible_alliance.length > 0)
			{
				int rand_id = pk::rand(int(possible_alliance.length - 1));
				
				array<int> new_alliance = possible_alliance[rand_id];//临时用
				if (new_alliance[0] < 2) return;//没盟友不算联盟--实际前面已判断，仅做保护
				int leader_force_id = get_leader_force(force_city_count, force_person_count, new_alliance);
				int target_force_id = top_5_force[rand_id];//
				FormAlliance(new_alliance, alliance_id, target_force_id, leader_force_id);
			}

		}

		void onNewDay()
		{
			for (int i = 0; i < 4; ++i)
			{				
				if (setting_ex.alliance_remain_time[i] != 255)
					setting_ex.alliance_remain_time[i] -= 1;
				int remain_time = setting_ex.alliance_remain_time[i];
				if (remain_time < (联盟持续时间/2) )
				{
					OutofAlliance(i);
					if (remain_time!=0) AllianceCollapes(i, get_member_arr(i));
					else AllianceCollapes(i, get_member_arr(i), true, 3);
				}
			}
			
			array<int> force_city_count = get_force_city_count();
			array<int> force_person_count = get_force_person_count();
			array<int> force_troop_count = get_force_troop_count();

			for (int i = 0; i < 非贼势力_末-1; ++i)
			{
				pk::force@src = pk::get_force(i);
				if (pk::is_alive(src))
				{
					for (int j = i+1; j < 非贼势力_末; ++j)
					{
						if (i == j) continue;
						pk::force@dst = pk::get_force(j);
						if (pk::is_alive(dst))
						{
							int value1 = get_relation_change(src, dst, force_city_count, force_person_count, force_troop_count);
							int value2 = get_relation_change(dst, src, force_city_count, force_person_count, force_troop_count);
							if (调试模式) pk::trace(pk::decode(pk::get_name(src)) + pk::decode(pk::get_name(dst)) + ",value1:" + value1 + ",value2:" + value2 + ",final" + pk::clamp((value1 + value2)/2, -25, 25));
							pk::add_relations(src, j, pk::clamp((value1 + value2)/2 ,-15, 10));
							allyUnally(src, dst, value1, value2, force_city_count, force_person_count, force_troop_count);
						}
					}
				}			
			}

			for (int force_id = 0; force_id < 非贼势力_末; ++force_id)
			{
				pk::force@force = pk::get_force(force_id);
				if (pk::is_alive(force))
				{
					if (force.is_player())
					{
						if (force_ex[force_id].form_alliance_delay > 0 and force_ex[force_id].form_alliance_delay < 10) {
							force_ex[force_id].form_alliance_delay -= 1;
							if (调试模式) pk::trace("距离组建联合军完成还剩：" + force_ex[force_id].form_alliance_delay + "回合");
						}
						if (force_ex[force_id].form_alliance_delay == 0)
						{
							int alliance_id = force_ex[force_id].alliance_id;
							if (alliance_id < 0 or alliance_id > 3) continue;
							int target_force_id = setting_ex.alliance_target_id[alliance_id];
							if (!pk::is_valid_normal_force_id(target_force_id)) continue;

							array<int> top_5_force = get_top_n(force_city_count, force_person_count, 5);
							force_ex[force_id].alliance_id = 255;//不初始化后面没法操作
							array<int> new_alliance = get_one_possible_alliance(top_5_force, target_force_id, false);
							bool success = true;
							//需要加点成员不愿意加入的设定
							if (调试模式) pk::trace("before 联合军数量" + new_alliance[0]);
							if (调试模式)
							{
								string t;
								t += "势力:" + pk::decode(pk::get_name(pk::get_force(target_force_id))) + ",城市数:" + force_city_count[target_force_id] + ",武将数:" + force_person_count[target_force_id] + "\n";
								t += "可能敌势力数:" + new_alliance[0];
								for (int j = 1; j < (new_alliance[0]+1); ++j)
								{
									if (pk::is_valid_normal_force_id(new_alliance[j])) t += "," + j + ":" + pk::decode(pk::get_name(pk::get_force(new_alliance[j])));

								}
								t += "\n";
								pk::trace(t);
							}
							new_alliance = modify_alliance_member(new_alliance, alliance_id, force, target_force_id);
							if (new_alliance[0] < 2) success = false;
							if (调试模式) pk::trace("after 联合军数量" + new_alliance[0]);
							if (调试模式)
							{
								string t;
								t += "势力:" + pk::decode(pk::get_name(pk::get_force(target_force_id))) + ",城市数:" + force_city_count[target_force_id] + ",武将数:" + force_person_count[target_force_id] + "\n";
								t += "可能敌势力数:" + new_alliance[0];
								for (int j = 1; j < (new_alliance[0] + 1); ++j)
								{
									if (pk::is_valid_normal_force_id(new_alliance[j])) t += "," + j + ":" + pk::decode(pk::get_name(pk::get_force(new_alliance[j])));

								}
								t += "\n";
								pk::trace(t);
							}
							if (success)
							{
								int leader_force_id = get_leader_force(force_city_count, force_person_count, new_alliance);
								if (!pk::is_valid_normal_force_id(leader_force_id)) continue;
								FormAlliance(new_alliance, alliance_id, target_force_id, leader_force_id);
								initPlayerAllianceInfo(alliance_id, force_id);
							}
							else
							{
								string src_force_name = pk::get_name(force);
								pk::message_box(src_force_name + pk::encode("尝试组建的联合军遭遇重重阻碍，最终以失败告终。"));
								initPlayerAllianceInfo(alliance_id, force_id);
							}

						}
					}
				}
			}
		}

		void onAIRunningOrder(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 据点AI_联合军出征)
			{
				if (调试模式) pk::trace("据点" + pk::get_new_base_name(building.get_id()) + "执行出征0");
				//此ai的不同之处在于出征时要出所有能出的部队，另外留守兵力怎么计算
				pk::list<pk::person@> person_sel;
				int target_base_id = -1;
				if (run_order_before(context, building, target_base_id)) run_order(context, building, target_base_id);
				else if (调试模式) pk::trace("据点AI_联合军出征 不满足");
			}
		}

		bool run_order_before(pk::ai_context@ context, pk::building@ building, int& target_base_t)//此处确认目标城市，以及是否可以执行---哪些需要在输入命令时就确定？
		{
			int building_id = building.get_id();
			pk::ai_context_base@ base = context.base[building_id];
			int src_force_id = building.get_force_id();
			int alliance_id = force_ex[src_force_id].alliance_id;
			if (alliance_id < 0 or alliance_id > 3) return false;
			int target_force_id = setting_ex.alliance_target_id[alliance_id];//force_ex[context.force.get_id()].alliance_target_id;
			if (!pk::is_valid_normal_force_id(target_force_id)) return false;//没有目标势力自然不出征
			if (base.enemies_around5) return false;//据点在被打时，就不要执行联合军出征了
			if (building.get_troops() < 20000) return false;//兵力过低时不出征
			if (base.num_officers < 4) return false;//总武将数不足3人时，不出征
			if (base.num_idle_officers == 0) return false;

			//港关据点要不要出征？---要
			//pk::city@ city = pk::is_valid_city_id(building_id)?pk::building_to_city(building):null;
			//array<int> target_base_arr;
			//int target_base_t = -1;
			int target_troops = 1000000;//设定一个大值
			for (int i = 0; i < 据点_末; ++i)
			{
				if (pk::is_neighbor_base(building_id, i))
				{
					pk::building@ dst_base = pk::get_building(i);
					if (dst_base.get_force_id() == target_force_id)
					{
						if (setting_ex.alliance_target_base[alliance_id].find(i) >= 0) { target_base_t = dst_base.get_id(); break; }//如果该城市正好已被设为目标，则无条件攻击该据点

						if (dst_base.get_troops() < target_troops)
						{
							target_troops = dst_base.get_troops();
							target_base_t = dst_base.get_id();
						}
					}
				}
			}
			if (pk::is_valid_base_id(target_base_t)) return true;
			return false;
		}
		//此处出征应考虑是否出现复制武将（影武者）bug
		void run_order(pk::ai_context@ context, pk::building@ building, int target_base_t)
		{
			//对目标城市进行攻击，出征部队数最多10？，出征部队自动优化？(先获取统武总和最高的武将作为主将，然后优化)
			//循环到武将数少于4，或出征后兵力少于1.5w
			int building_id = building.get_id();
			pk::district@ district = pk::get_district(building.get_district_id());
			int ap_backup = context.ap;
			district.ap = 255;
			context.ap = 255;
			pk::ai_context_base@ base = context.base[building_id];
			
			if (context.idle_person.count == 0) return;
			context.idle_person.sort(function(a, b)
			{
				return ((a.stat[武将能力_统率] + a.stat[武将能力_武力]) > (b.stat[武将能力_统率] + b.stat[武将能力_武力])); // 무장 정렬 (지력순)
			});
			int person_count = base.num_officers;
			int i = 0;
			int marched_unit_count = 0;
			array<int> route_type = ai出征优化::get_target_base_route_type(building_id, target_base_t);
			if (调试模式) pk::trace("据点" + pk::get_new_base_name(building_id) + "执行出征");
			while (building.get_troops() > 15000 and person_count > 3 and marched_unit_count < 10 and i < context.idle_person.count)
			{
				
				pk::com_march_cmd_info cmd_draft;
				@cmd_draft.base = @building;
				cmd_draft.type = 部队类型_战斗;
				cmd_draft.member[0] = context.idle_person[i].get_id();
				if (pk::get_gold(building) > 3000) cmd_draft.gold = 1000;
				else cmd_draft.gold = 0;
				if (context.idle_person[i].stat[武将能力_统率] > 79 or context.idle_person[i].stat[武将能力_武力] > 79) cmd_draft.troops = pk::get_command(context.idle_person[i]);
				else cmd_draft.troops = 5000;
				cmd_draft.food = 2 * cmd_draft.troops;
				cmd_draft.order = 部队任务_征服;
				cmd_draft.target_pos = pk::get_building(target_base_t).pos;
				ai出征优化::modify_food(cmd_draft, building_id, target_base_t);
				pk::list<pk::person@> list;
				list.add(context.idle_person[i]);
				cmd_draft.weapon_id[0] = ai出征优化::get_best_weapon(list, 兵器_剑, building, 1/*攻击*/);
				cmd_draft.weapon_amount[0] = get_best_weapon_amount(building, cmd_draft.weapon_id[0], cmd_draft.troops);
				//水军设定还得改---根据连接匹配副将，连接点带海洋需要高级舰船
				ai出征优化::modify_normal_deputy(cmd_draft, list, target_base_t);//需要更新list
				if (pk::is_valid_person_id(cmd_draft.member[1])) 
				{
					auto t_person = pk::get_person(cmd_draft.member[1]);
					int index = context.idle_person.index_of(t_person);
					if (index > -1 and index < context.idle_person.count)
					{
						context.idle_person.remove_at(index); //此处报错(399): __core__(3991): pos >= 0 && pos < self->size
						list.add(t_person);
					}

				}
				if (pk::is_valid_person_id(cmd_draft.member[2]))
				{
					auto t_person = pk::get_person(cmd_draft.member[2]);
					int index = context.idle_person.index_of(t_person);
					if (index > -1 and index < context.idle_person.count)
					{
						context.idle_person.remove_at(index); //此处报错(399): __core__(3991): pos >= 0 && pos < self->size
						list.add(t_person);
					}

				}
				int sea_weapon = 兵器_楼船;
				if (pk::has_tech(context.force, 技巧_投石开发)) sea_weapon = 兵器_斗舰;
				if (get_best_heishu(list, 兵器_楼船) > 适性_B) { cmd_draft.weapon_id[1] = sea_weapon; cmd_draft.weapon_amount[1] = 1; }
				else { cmd_draft.weapon_id[1] = 兵器_走舸; cmd_draft.weapon_amount[1] = 1; }
				marched_unit_count += 1;
				person_count -= 1;
				++i;
				if (cmd_draft.weapon_id[0] == 兵器_剑) continue;
				int unit_id = pk::command(cmd_draft);
				pk::run_order(pk::get_unit(unit_id));

			}
			if (调试模式) pk::trace("据点" + pk::get_new_base_name(building_id) + "执行出征部队数" + marched_unit_count);
			if (marched_unit_count > 0)
			{
				int src_force_id = building.get_force_id();
				force_ex[src_force_id].attack_delay = 255;//攻击完成后将其设为255
				int alliance_id = force_ex[src_force_id].alliance_id;
				int target_force_id = setting_ex.alliance_target_id[alliance_id];
				string force_name = pk::decode(pk::get_name(pk::get_force(target_force_id)));
				string t = pk::encode("遵照联盟约定，我们向\x1b[2x" + force_name + "\x1b[0x发动攻击");
				pk::message_box(t,pk::get_person(pk::get_taishu_id(building)));
			}
			district.ap = pk::max(0,ap_backup - 联合军出征行动力);
			context.ap = pk::max(0, ap_backup - 联合军出征行动力);
		}

		//========================================玩家菜单开始===========================================//

		void initMenu(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@kunshu_ = pk::get_person(pk::get_kunshu_id(building));
			player_alliance_id = get_alliance_id();
		}

		bool isMenuVisible()
		{
			if (!building_.is_player()) return false;//仅给玩家城市
			if (building_.get_id() != kunshu_.service) return false;//仅限君主所在城市
			return true;
		}

		bool isEnabled()
		{
			auto person_list = pk::get_idle_person_list(building_);
			int result = check_avaliable(building_, person_list);
			if (result != 0) return false;
			else return true;
		}

		string getMenuDesc()
		{
			auto person_list = pk::get_idle_person_list(building_);
			int result = check_avaliable(building_, person_list);
			int alliance_id = force_ex[force_.get_id()].alliance_id;
			pk::trace("alliance_id:" + alliance_id );
			//pk::trace("alliance_target_id:" + setting_ex.alliance_target_id[alliance_id]);
			switch (result)
			{
			case 1: return pk::encode("仅能在君主所在据点执行。"); 
			case 2: return pk::encode(pk::format("正在组建对抗{}的联军，剩余{}回合。", pk::decode(pk::get_name(pk::get_force(setting_ex.alliance_target_id[alliance_id]))), force_ex[force_.get_id()].form_alliance_delay)); 
			case 3: return (alliance_id > 3?pk::encode("3"): pk::encode(pk::format("已参加反{}联盟。", pk::decode(pk::get_name(pk::get_force(setting_ex.alliance_target_id[alliance_id])))))); 
			case 4: return pk::encode("联军数量已达上限"); 
			case 5: return pk::encode("没有可执行的武将. "); 
			case 6: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST)); 
			case 7: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST)); 
			case 8: return pk::encode("无可以成立联军的对象"); 
			case 0: return pk::encode(pk::format("成立联军.(消耗：金{}/每势力 行动力{})", GOLD_COST, ACTION_COST)); 
			default:
				return pk::encode(""); 
			}
			return pk::encode("");
		}

		int check_avaliable(pk::building@ building, pk::list<pk::person@> list)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			if (building_.get_id() != kunshu_.service) return 1;
			else if (force_ex[force_.get_id()].form_alliance_delay > 0 and force_ex[force_.get_id()].form_alliance_delay != 255) return 2;
			else if (force_ex[force_.get_id()].alliance_id >= 0 and force_ex[force_.get_id()].alliance_id <= 3) return 3;
			else if (player_alliance_id < 0 or player_alliance_id > 3) return 4;			
			else if (list.count == 0) return 5;
			else if (pk::get_district(building.get_district_id()).ap < ACTION_COST) return 6;
			else if (pk::get_gold(building) < GOLD_COST) return 7;
			array<int> force_city_count = get_force_city_count();
			array<int> force_person_count = get_force_person_count();
			array<int> force_troop_count = get_force_troop_count();
			array<int> top_5_force = get_top_n(force_city_count, force_person_count, 5);
			//auto top_5_city_force = get_top_n(force_city_count, 5);
			//auto top_5_person_force = get_top_n(force_person_count,5);
			array<array<int>> possible_alliance = get_possible_alliance(top_5_force,false);
			bool include = false;
			if (调试模式)
			{
				string t;
				for (int i = 0; i < int(possible_alliance.length); ++i)
				{
					t += "城市第" + i + "势力:" + pk::decode(pk::get_name(pk::get_force(top_5_force[i]))) + ",城市数:" + force_city_count[top_5_force[i]] + ",武将数:" + force_person_count[top_5_force[i]] + "\n";
					t += "可能敌势力数:" + possible_alliance[i][0];
					for (int j = 1; j < int(possible_alliance[i].length); ++j)
					{
						if (pk::is_valid_normal_force_id(possible_alliance[i][j])) t += "," + j + ":" + pk::decode(pk::get_name(pk::get_force(possible_alliance[i][j])));

					}
					t += "\n";
				}
				pk::trace(t);
			}
			for (int i = 0; i < int(possible_alliance.length); ++i)//排除不满足的势力
			{
				if (possible_alliance[i][0] < 联盟最小势力数)
				{
					if (调试模式) pk::trace("城市第" + i + "势力:" + pk::decode(pk::get_name(pk::get_force(top_5_force[i]))) + "势力数：" + possible_alliance[i][0] + ",include" + include + ",i:" + i + ",top_5_forcelength1:" + top_5_force.length + ",possible_alliancelength2:" + possible_alliance.length);
					possible_alliance.removeAt(i);
					top_5_force.removeAt(i);
					i -= 1;//重要移除后数组序号会发生变化
					continue;
				}
				for (int j = 0; j < int(possible_alliance[i].length); ++j)//第一位是数量
				{
					if (j!=0)
						if (possible_alliance[i][j] == force_.get_id()) {include = true; continue; }
					if (possible_alliance[i][j] == -1) { possible_alliance[i].removeAt(j); j -= 1; };
					
				}
				if (!include) {
					if (调试模式) pk::trace("城市第" + i + "势力:" + pk::decode(pk::get_name(pk::get_force(top_5_force[i]))) + "势力数：" + possible_alliance[i][0] + ",include" + include + ",i:" + i + ",length1:" + top_5_force.length + ",length2:" + possible_alliance.length);
					possible_alliance.removeAt(i);
					top_5_force.removeAt(i);
					i -= 1;//重要移除后数组序号会发生变化
				}
				include = false;
			}
			player_possible_alliance = possible_alliance;
			player_top_5_force = top_5_force;
			if (调试模式)
			{
				string t;
				for (int i = 0; i < int(possible_alliance.length); ++i)
				{
					t += "城市第" + i + "势力:" + pk::decode(pk::get_name(pk::get_force(top_5_force[i]))) + ",城市数:" + force_city_count[top_5_force[i]] + ",武将数:" + force_person_count[top_5_force[i]] + "\n";
					t += "可能敌势力数:" + possible_alliance[i][0];
					for (int j = 1; j < int(possible_alliance[i].length); ++j)
					{
						if (pk::is_valid_normal_force_id(possible_alliance[i][j])) t += "," + j + ":" + pk::decode(pk::get_name(pk::get_force(possible_alliance[i][j])));

					}
					t += "\n";
				}
				pk::trace(t);
			}
			if (int(possible_alliance.length) == 0 or int(top_5_force.length) == 0) return 8;
			else return 0;
		}

		string getMenuText()
		{
			return pk::encode("成立联军");
		}

		int getMenuImageID()
		{
			return 889;
		}

		bool menuHandler()
		{
			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("是否要组建联军?\n(消耗：金{}/每势力 行动力{})", GOLD_COST, ACTION_COST)), kunshu_) == 1)
				return false;
			pk::list<pk::force@> force_list1;
			pk::list<pk::force@> force_list2;
			for (int i = 0; i < int(player_top_5_force.length); ++i)
			{
				if (!pk::is_valid_normal_force_id(player_top_5_force[i])) continue;
				force_list1.add(pk::get_force(player_top_5_force[i]));
			}

			pk::list<pk::force@> force_sel1 = pk::force_selector(pk::encode("选择要对抗的势力"), pk::encode("选择要对抗的敌方势力."), force_list1, 1, 1);
			if (force_sel1.count == 0) return false; // 未选择时结束
			pk::force@target_force = force_sel1[0];

			int order = player_top_5_force.find(target_force.get_id());
			int target_force_id = target_force.get_id();
			int src_force_id = force_.get_id();
			for (int i = 1; i < int(player_possible_alliance[order].length); ++i)
			{
				if (!pk::is_valid_normal_force_id(player_possible_alliance[order][i])) continue;
				if (force_.get_id() == player_possible_alliance[order][i]) continue;//不需要选择己方势力
				force_list2.add(pk::get_force(player_possible_alliance[order][i]));
			}
			int max_count = pk::min(pk::get_gold(building_)/GOLD_COST, 4);//最多能邀请的势力个数受到据点资金影响
			pk::list<pk::force@> force_sel2 = pk::force_selector(pk::encode("选择加入联军势力"), pk::encode("选择邀请加入联军的势力," + ((max_count<4)?"金不足,":"") + "最多" + max_count + "个"), force_list2, 1, max_count, force_list2);
			if (force_sel2.count == 0) return false; // 未选择时结束
			
			force_ex[src_force_id].alliance_id = player_alliance_id;
			force_ex[src_force_id].form_alliance_delay = 3;
			setting_ex.alliance_target_id[player_alliance_id] = target_force_id;
			setting_ex.alliance_member_id[player_alliance_id][0] = force_.get_id();
			for (int i = 0; i < pk::min(4, force_sel2.count); ++i)
			{
				setting_ex.alliance_member_id[player_alliance_id][i+1] = force_sel2[i].get_id();
			}		

			int gunshi_id = force_.gunshi;
			if (!pk::is_valid_person_id(gunshi_id)) gunshi_id = 武将_文官;
			pk::person@ actor = pk::get_person(gunshi_id);
			string dst_force_name = pk::decode(pk::get_name(pk::get_person(target_force.kunshu)));
			pk::message_box(pk::encode(pk::format("马上开始筹备反\x1b[2x{}\x1b[0x联盟，预计耗时\x1b[1x{}\x1b[0x旬。", dst_force_name, force_ex[src_force_id].form_alliance_delay)),actor);
			
			pk::add_gold(building_, -force_sel2.count*GOLD_COST);
			pk::get_district(building_.get_district_id()).ap -= ACTION_COST;
			return true;
		}

		//========================================玩家菜单结束===========================================//
		void AllianceCollapes(uint8 alliance_id, array<int> member_arr, bool ignore_count = false, int reason = 0)
		{
			if (!ignore_count and int(member_arr.length) > 0) return;//有成员可以不瓦解
			string dst_kunshu_name = pk::get_name(pk::get_person(pk::get_force(setting_ex.alliance_target_id[alliance_id]).kunshu));
			for (int i = 0; i< int(member_arr.length); ++i)
			{
				force_ex[member_arr[i]].alliance_id = 255;
				force_ex[member_arr[i]].attack_delay = 255;
			}
			for (int i = 0; i< 非贼势力_末; ++i)//保险
			{
				if (force_ex[i].alliance_id == alliance_id)
				{
					force_ex[i].alliance_id = 255;
					force_ex[i].attack_delay = 255;
				}
			}
			setting_ex.alliance_leader_id[alliance_id] = 255;
			setting_ex.alliance_target_id[alliance_id] = 255;
			setting_ex.alliance_remain_time[alliance_id] = 255;
			for (int i = 0; i < 6; ++i) setting_ex.alliance_target_base[alliance_id][i] = 255;
			if (reason == 0) pk::message_box(pk::encode("随着联盟内部矛盾日益加深，成员相继退出，反\x1b[2x")+ dst_kunshu_name + pk::encode("\x1b[0x联盟最终宣告瓦解。"));
			else if(reason == 1) pk::message_box(pk::encode("随着联盟目标完成，反\x1b[2x") + dst_kunshu_name + pk::encode("\x1b[0x联盟也失去其意义，最终解散。"));
			else if (reason == 2) pk::message_box(pk::encode("随着盟主势力覆灭，反\x1b[2x") + dst_kunshu_name + pk::encode("\x1b[0x联盟分崩离析。"));
			else if (reason == 3) pk::message_box(pk::encode("时间流逝，而目标迟迟未完成，反\x1b[2x") + dst_kunshu_name + pk::encode("\x1b[0x联盟的成员逐渐离心，联盟破裂。"));
		}

		array<int> get_member_arr(uint8 alliance_id)
		{
			array<int> member_arr;
			for (uint8 i = 0; i < 非贼势力_末; ++i)
			{
				if (force_ex[i].alliance_id == alliance_id and i != setting_ex.alliance_leader_id[alliance_id]) member_arr.insertLast(i);
			}
			return member_arr;
		}

		void OutofAlliance(int alliance_id)
		{
			int leader_force_id = setting_ex.alliance_leader_id[alliance_id];
			if (!pk::is_valid_normal_force_id(leader_force_id)) return;
			pk::force@ leader_force = pk::get_force(leader_force_id);
			array<int> member_arr = get_member_arr(alliance_id);
			int remain_time = setting_ex.alliance_remain_time[alliance_id];
			if (remain_time <= (联盟持续时间 / 2) and remain_time > (联盟持续时间 / 3))
			{
				for (int i = 0; i < int(member_arr.length); ++i)
				{
					if (leader_force.relations[member_arr[i]] < 60 and pk::rand_bool(50))
					{
						pk::force@ member_force = pk::get_force(member_arr[i]);
						if (!member_force.is_player()) BreakfromAlliance(member_arr[i]);
					}
				}
				
			}
			else if (remain_time <= (联盟持续时间 / 3) and remain_time > (联盟持续时间 / 4))
			{
				for (int i = 0; i < int(member_arr.length); ++i)
				{
					if (leader_force.relations[member_arr[i]] < 70 and pk::rand_bool(50))
					{
						pk::force@ member_force = pk::get_force(member_arr[i]);
						if (!member_force.is_player()) BreakfromAlliance(member_arr[i]);
					}
				}
			}
			else if (remain_time <= (联盟持续时间 / 4) and remain_time > (联盟持续时间 / 6))
			{
				for (int i = 0; i < int(member_arr.length); ++i)
				{
					if (leader_force.relations[member_arr[i]] < 80 and pk::rand_bool(50))
					{
						pk::force@ member_force = pk::get_force(member_arr[i]);
						if (!member_force.is_player()) BreakfromAlliance(member_arr[i]);
					}
				}
			}
			else if (remain_time <= (联盟持续时间 / 6) and remain_time > (联盟持续时间 / 12))
			{
				for (int i = 0; i < int(member_arr.length); ++i)
				{
					if (leader_force.relations[member_arr[i]] < 90 and pk::rand_bool(50))
					{
						pk::force@ member_force = pk::get_force(member_arr[i]);
						if (!member_force.is_player()) BreakfromAlliance(member_arr[i]);
					}
				}
			}
			else if (remain_time <= (联盟持续时间 / 12) and remain_time > 0)
			{
				for (int i = 0; i < int(member_arr.length); ++i)
				{
					if (leader_force.relations[member_arr[i]] < 100 and pk::rand_bool(50))
					{
						pk::force@ member_force = pk::get_force(member_arr[i]);
						if (!member_force.is_player()) BreakfromAlliance(member_arr[i]);
					}
				}
			}
			else if (remain_time == 0)
			{
				for (int i = 0; i < int(member_arr.length); ++i)
				{
					pk::force@ member_force = pk::get_force(member_arr[i]);
					if (!member_force.is_player()) BreakfromAlliance(member_arr[i]);
				}
			}
		}

		void BreakfromAlliance(int force_id, bool is_killed = false)
		{
			int alliance_id = force_ex[force_id].alliance_id;

			force_ex[force_id].alliance_id = 255;
			force_ex[force_id].attack_delay = 255;
			
			if (!is_killed)
			{
				if (!pk::is_valid_normal_force_id(force_id)) return;
				pk::force@ src_force = pk::get_force(force_id);
				array<int> member_arr = get_member_arr(alliance_id);			
				string src_force_name = pk::get_name(src_force);
				string dst_kunshu_name = pk::get_name(pk::get_person(pk::get_force(setting_ex.alliance_target_id[alliance_id]).kunshu));
				for (int i = 0; i<int(member_arr.length); ++i)
				{
					if (i == force_id) continue;
					if (src_force.ally[member_arr[i]]) continue;
					int time = pk::max(0, src_force.ceasefire_timer[member_arr[i]] - (联盟持续时间 - setting_ex.alliance_remain_time[alliance_id]));
					pk::set_ceasefire_timer(src_force, member_arr[i], time);
				}
				pk::message_box("\x1b[1x" + src_force_name + pk::encode("\x1b[0x脱离了反\x1b[2x") + dst_kunshu_name + pk::encode("\x1b[0x联盟"));
			}
	
		}

		void initPlayerAllianceInfo(int alliance_id, int player_force_id)
		{
			for (int i = 0; i < 5; ++i)
			{
				setting_ex.alliance_member_id[alliance_id][i] = 255;
			}
			force_ex[player_force_id].form_alliance_delay = 255;
		}

		array<int> modify_alliance_member(array<int> new_alliance, int alliance_id, pk::force@ force/*己方势力*/, int target_force_id)
		{
			array<int> new_alliance_t;
			for (int i = 1; i < (new_alliance[0]+1); ++i)
			{
				if (调试模式) pk::trace("i:" + i + "数量" + new_alliance[0] + "new_alliance[i]" + new_alliance[i]);
				new_alliance_t.insertLast(new_alliance[i]);
			}
			array<bool> check_arr(int(new_alliance[0] + 1), false);
			check_arr[0] = true;
			for (int i = 1; i < 5; ++i)
			{
				int pos = new_alliance_t.find(setting_ex.alliance_member_id[alliance_id][i]);
				if (调试模式) pk::trace( "i" + i + "setting_ex.alliance_member_id[alliance_id][i]" + setting_ex.alliance_member_id[alliance_id][i]);
				if (pos > -1)
				{
					if (调试模式) pk::trace("pos" + pos + "i" + i + "setting_ex.alliance_member_id[alliance_id][i]" + setting_ex.alliance_member_id[alliance_id][i]);
					check_arr[i] = true;
				}
			}
			for (int i = 1; i < (new_alliance[0]+1); ++i)
			{
				if (!check_arr[i])
				{
					new_alliance.removeAt(i);
					new_alliance[0] -= 1;
					i -= 1;
				}
				else
				{
					//50几率排除友好度不足的势力
					if ((force.relations[new_alliance[i]]+5) < pk::get_force(target_force_id).relations[new_alliance[i]] and pk::rand_bool(50))
					{
						string member_force_name = pk::get_name(pk::get_force(new_alliance[i]));
						pk::message_box(member_force_name +pk::encode("拒绝参与联军。"));
						new_alliance.removeAt(i);
						new_alliance[0] -= 1;
						i -= 1;
					}
				}
			}
			return new_alliance;
		}

		void FormAlliance(array<int> new_alliance, int alliance_id, int target_force_id, int leader_force_id)
		{
			if (!pk::is_valid_normal_force_id(leader_force_id)) return;
			if (!pk::is_valid_normal_force_id(target_force_id)) return;
			if (new_alliance[0] < 2) return;
			if (alliance_id > 3 or alliance_id < 0) return;
			pk::force@ leader_force = pk::get_force(leader_force_id);
			pk::force@ target_force = pk::get_force(target_force_id);

			//是一起攻击一个城市好,还是分散攻击比较好？
			set_alliance_force(new_alliance/*盟军势力*/, alliance_id, target_force_id, leader_force_id);

			leader_kunshu_name = pk::decode(pk::get_name(pk::get_person(leader_force.kunshu)));
			target_kunshu_name = pk::decode(pk::get_name(pk::get_person(target_force.kunshu)));
			joined_force_name = "";
			for (int i = 1; i < int(new_alliance.length); ++i)
			{
				int alliance_force_id = new_alliance[i];
				if (pk::is_valid_normal_force_id(alliance_force_id))
				{
					if (alliance_force_id != leader_force_id)
					{
						if (joined_force_name != "") joined_force_name += ",";
						pk::force@ joined_force = pk::get_force(alliance_force_id);
						//pk::trace("alliance_force_id" + alliance_force_id);
						joined_force_name += pk::decode(pk::get_name(pk::get_person(joined_force.kunshu)));
					}
				}
			}
			pk::scene(pk::scene_t(Scene_FormingAlliance));
			
		}

		// 同盟签订
		void allyUnally(pk::force@ force0, pk::force@ force2, int force0_wantAlly, int force2_wantAlly, array<int> force_city_count, array<int> force_person_count, array<int> force_troop_count)
		{
			int force0_id = force0.get_id();
			int force2_id = force2.get_id();
			int relations = force0.relations[force2_id];

			// 玩家不会自动请求结盟。
			bool force0_want_more = force0_wantAlly > force2_wantAlly;
			bool will_同盟 = (relations >= 100 and pk::is_enemy(force0, force2));
			bool will_破弃 = (relations <= 0 and !pk::is_enemy(force0, force2));
			if (will_破弃)force0_want_more = !force0_want_more;
			pk::force@ force_reqs = force0_want_more ? force0 : force2;
			pk::force@ force_recv = force0_want_more ? force2 : force0;
			
			if (force0.is_player()) { @force_recv = force0; @force_reqs = force2; }
			if (force2.is_player()) { @force_recv = force2; @force_reqs = force0; }

			if (will_同盟 or will_破弃)cmd_AllyUnAlly(force_reqs, force_recv, (will_同盟 ? true : false), force_city_count, force_person_count, force_troop_count);
		}

		void cmd_AllyUnAlly(pk::force@ force0, pk::force@ force2, bool willAlly, array<int> force_city_count, array<int> force_person_count, array<int> force_troop_count)
		{
			pk::int_int temp = get_best_ally_city_distance(force0, force2);
			if (temp.first == -1 or temp.second == -1) return;

			pk::building@ building0 = pk::get_building(temp.second);
			auto person_list = pk::get_idle_person_list(building0);
			if (person_list.count == 0)return;
			pk::person@ actor;

			int force0_id = force0.get_id();
			int force2_id = force2.get_id();
			pk::person@ dst_kunshu = pk::get_person(force2.kunshu);
			if (willAlly)
			{
				if (force2.is_player())
				{
					pk::alliance_cmd_info info; // 破弃
					person_list.sort(function(a, b)
					{
						int sum_a = a.stat[武将能力_统率] + a.stat[武将能力_武力] + a.stat[武将能力_智力] + a.stat[武将能力_政治] + a.stat[武将能力_魅力];
						int sum_b = b.stat[武将能力_统率] + b.stat[武将能力_武力] + b.stat[武将能力_智力] + b.stat[武将能力_政治] + b.stat[武将能力_魅力];
						return sum_a < sum_b;
					});
					@info.actor = person_list[0];
					@info.target = force2;

					//电脑请求玩家只需要考虑钱
					float effect = float(force_city_count[force0_id] / force_city_count[force2_id] * force_person_count[force0_id] / force_person_count[force2_id] * force_troop_count[force0_id] / force_troop_count[force2_id]);
					if (effect != 0 and effect < 1.f) effect = -1.f / effect;
					int gold;
					if (effect > 0) gold = int(effect * 200 + 500);
					else  gold = int(effect * 200 - 2000);
					if (调试模式) pk::trace("effect:" + effect + ",gold:" + gold);
					info.gold = gold;
					pk::command(info);
				}
				else
				{
					//电脑间的同盟算概率
					//如果互相有俘虏，必然失败
					if (ALLIANCE_CHANCE::func_5be0e0(force0_id, force2_id) or ALLIANCE_CHANCE::func_5be0e0(force2_id, force0_id)) return;

					for (int i = 0; i < person_list.count; ++i)
					{
						if (pk::is_dislike(dst_kunshu, person_list[i].get_id())) continue;
						@actor = @person_list[i];
					}
					if (actor !is null)
					{
						pk::alliance_cmd_info info;
						@info.target = force2;
						@info.actor = actor;
						info.gold = 1000;
						pk::command(info);
					}
				}
			}
			else
			{
				pk::annul_alliance_cmd_info info; // 破弃
				person_list.sort(function(a, b)
				{
					int sum_a = a.stat[武将能力_统率] + a.stat[武将能力_武力] + a.stat[武将能力_智力] + a.stat[武将能力_政治] + a.stat[武将能力_魅力];
					int sum_b = b.stat[武将能力_统率] + b.stat[武将能力_武力] + b.stat[武将能力_智力] + b.stat[武将能力_政治] + b.stat[武将能力_魅力];
					return sum_a < sum_b;
				});
				@info.actor = person_list[0];
				@info.target = force2;

				pk::command(info);
			}

		}

		pk::int_int get_best_ally_city_distance(pk::force@ src, pk::force@ dst)
		{
			pk::list<pk::city@> srclist = pk::get_city_list(src);
			//pk::list<pk::city@> dstlist = pk::get_city_list(dst);
			pk::building@ dst_base = pk::get_building(pk::get_person(dst.kunshu).service);
			if (dst_base is null) return pk::int_int(-1,-1);
			pk::city@ dst_city = pk::get_city(pk::get_city_id(dst_base.pos));
			if (srclist.count == 0) return pk::int_int(-1, -1);
			int best_distance = 300;
			int best_city_id = -1;
			for (int i = 0; i < srclist.count; ++i)
			{
				int city_id = srclist[i].get_id();
				int distance = pk::get_city_distance(city_id,dst_city.get_id());
				if (distance < best_distance)
				{
					best_distance = distance;
					best_city_id = city_id;
				}
			}
			return pk::int_int(best_distance, best_city_id);
		}

		int get_relation_change(pk::force@ src, pk::force@ dst, array<int> force_city_count, array<int> force_person_count, array<int> force_troop_count)
		{
			
			int src_id = src.get_id();
			int dst_id = dst.get_id();
			pk::person@ src_kunshu = pk::get_person(src.kunshu);
			//友好度变化---尽量远交近攻
			//邻近与否，威胁程度（城市数，武将数，兵力数比值），是否战争中(战争已有对应增减友好度)，是否拥立汉帝，亲爱，厌恶，相性，同盟主义/敌人的敌人，是否同一联盟，友好度
			float a = (pk::is_neighbor_force(src, dst)?-1.f:0.7f)*15.f ;//邻近与否
			//对方势力越强，越想同盟
			float temp = float(force_city_count[src_id] / pk::max(1,force_city_count[dst_id]) * force_person_count[src_id] / force_person_count[dst_id] * force_troop_count[src_id] / force_troop_count[dst_id]);
			if (temp != 0 and temp < 1.f) temp = -1.f / temp;
			
			float b = pk::clamp(temp/1.5f * 10.f,-20.f, 20.f);//威胁程度（城市数，武将数，兵力数比值）
			float c = (pk::is_protecting_the_emperor(dst) ? 1.f : 0.f) * (src_kunshu.kanshitsu - 1) * 10.f;//是否拥立汉帝
			float d = get_person_influence(src_kunshu,dst.kunshu);//亲爱，厌恶，相性
			float e = get_ally_influence(src,dst_id);
			int final = int((a + b + c + d + e) / 5.f);
			if (final > 0 and src.relations[dst_id] > 60) final = final / 6;
			if (final > 0 and src.relations[dst_id] > 80) final = final / 9;//防止友好度加的太快
			return final;
		}

		float get_ally_influence(pk::force@ src, int dst_id)
		{
			float t = 0.f;
			if (force_ex[src.get_id()].alliance_id == force_ex[dst_id].alliance_id) t = 18.f;
			else if (src.ally[dst_id]) t = 15.f;
			else t = (src.relations[dst_id] - 50.f) / 10.f;
			return t;
		}

		float get_person_influence(pk::person@ src, int dst_id)
		{
			float t;
			if (pk::is_like(src, dst_id)) t = 10.f;
			else if (pk::is_dislike(src, dst_id)) t = -10.f;
			else t = (35 - pk::get_aishou_distance(src, dst_id))/5.f;
			return t;
		}

		int get_alliance_id()
		{
			//int alliance_id = -1;
			array<int> t_arr = { 0,1,2,3 };
			for (int i = 0; i < 非贼势力_末; ++i)
			{
				if (force_ex[i].alliance_id < 0 or force_ex[i].alliance_id > 3) continue;
				int pos = t_arr.find(force_ex[i].alliance_id);
				if (pos > -1) t_arr.removeAt(pos);
			}
			if (int(t_arr.length) > 0) return t_arr[0];
			return -1;
		}

		//延迟3回合出征？这三回合积极征兵，第三回合ai指令调整为出征模式？
		//联盟的每个城市都执行出征命令，排除非邻近城市（或者运输支援出征城市？），如果有多个邻近目标，优先选择兵力*武将数较少的。如果打不下怎么办？
		void conduct_attack(array<int> arr/*盟军势力*/, int target_force_id, int leader_force_id)
		{

		}

		void Scene_FormingAlliance()
		{
			pk::background(11);
			pk::play_se(25); //25?
			pk::message_box(pk::encode("盟主\x1b[1x" + leader_kunshu_name + "\x1b[0x" + "组成了反\x1b[16x" + target_kunshu_name + "\x1b[0x联盟\n\x1b[1x" + joined_force_name + "\x1b[0x" + "已加入联盟."));
			leader_kunshu_name = "";
			target_kunshu_name = "";
			joined_force_name = "";
		}

		int get_leader_force(array<int> arr/*城市数*/, array<int> arr2/*武将数*/, array<int> force_arr)
		{
			int num = force_arr[0] + 1;//int(force_arr.length);
			int force_id = force_arr[1];
			for (int i = 2; i < force_arr[0]; ++i)
			{
				//if (调试模式) pk::trace("i" + i + "force_id" + force_id + "num" + num);
				//if(调试模式) pk::trace("arr[force_id]" + arr[force_id] + "force_arr[i]" + force_arr[i]);
				//if (调试模式) pk::trace("force_arr[i]" + force_arr[i] + "arr[force_arr[i]]" + arr[force_arr[i]]);
				if (arr[force_arr[i]] > arr[force_id]) force_id = force_arr[i];
				else if (arr[force_arr[i]] == arr[force_id])
				{
					if (arr2[force_arr[i]] > arr2[force_id]) force_id = force_arr[i];
				}
			}
			//if (调试模式) pk::trace("force_id" + force_id);
			return force_id;
		}

		//是否要参照原版加入联盟势力武将忠诚低于100的都设置为100
		void set_alliance_force(array<int> arr/*盟军势力*/, int alliance_id, int target_force_id, int leader_force_id)
		{
			setting_ex.alliance_target_id[alliance_id] = target_force_id;
			setting_ex.alliance_leader_id[alliance_id] = leader_force_id;
			setting_ex.alliance_remain_time[alliance_id] = 联盟持续时间;
			//if (调试模式) pk::trace(" int(arr.length)" + int(arr.length));
			//设置两两停战
			for (int i = 1; i < int(arr.length-1); ++i)
			{
				int force_id = arr[i];
				if (pk::is_valid_normal_force_id(force_id))
				{
					for (int j = i+1; j < int(arr.length); ++j)
					{
						//if (调试模式) pk::trace("force_id" + force_id + "arr[j]" + arr[j] + "i" + i + "j" + j);						
						if (pk::is_valid_normal_force_id(arr[j]))
						{
							pk::force@ force = pk::get_force(force_id);
							if (!force.ally[arr[j]]) pk::set_ceasefire_timer(force, arr[j], 联盟持续时间);//不是同盟的，设置2年停战
							if (force.relations[arr[j]] < 80) pk::add_relations(force, arr[j], 20);//友好度设定
							else pk::set_relations(force, arr[j], 100);
						}
					}
				}
			}

			for (int i = 1; i < int(arr.length); ++i)//第一位是数量
			{
				int force_id = arr[i];
				if (pk::is_valid_normal_force_id(force_id))
				{
					pk::force@ force = pk::get_force(force_id);
					force_ex[force_id].attack_delay = 3;//设置联盟攻击延迟
					force_ex[force_id].alliance_id = alliance_id;//设置联盟id
					if (联盟资源奖励)
					{
						for (int j = 0; j < 城市_末; ++j)
						{
							pk::city@ city = pk::get_city(j);
							if (pk::is_alive(city))
							{
								if (city.get_force_id() == force.get_id())
								{
									if (!pk::enemies_around(pk::get_building(j)) and !city.is_player())//参考原版设定防止影响下城
									{
										float rate = 1.f;
										int elapsed_years = pk::get_elapsed_years();
										if (elapsed_years > 10) rate = 4.f;
										else if (elapsed_years > 7) rate = 3.f;
										else if (elapsed_years > 5) rate = 2.5f;
										else if (elapsed_years > 3) rate = 2.f;
										else if (elapsed_years > 2) rate = 1.5f;
										pk::add_gold(city, int(1000 * rate), false);
										pk::add_food(city, int(20000 * rate), false);
										ch::add_troops(city, int(10000 * rate), false);
										for (int k = 兵器_枪; k < 兵器_冲车; ++k) pk::add_weapon_amount(city, k, int(5000 * rate), false);

										if (pk::has_tech(force, 技巧_木兽开发)) pk::add_weapon_amount(city, 兵器_木兽, int(2 * rate), false);
										else pk::add_weapon_amount(city, 兵器_冲车, int(2 * rate), false);
										if (pk::has_tech(force, 技巧_投石开发))
										{
											pk::add_weapon_amount(city, 兵器_投石, int(2 * rate), false);
											pk::add_weapon_amount(city, 兵器_斗舰, int(2 * rate), false);
										}
										else
										{
											pk::add_weapon_amount(city, 兵器_井阑, int(2 * rate), false);
											pk::add_weapon_amount(city, 兵器_楼船, int(2 * rate), false);
										}
										pk::set_energy(city, pk::get_max_energy(city));//据点气力最大
									}

								}
							}
						}
					}
				}
			}						
		}

		//还需要排除已经有联盟的势力
		array<array<int>> get_possible_alliance(array<int> top_n_force, bool exclude_player = true)
		{
			int num = int(top_n_force.length);
			array<array<int>> possible_alliance(num,array<int>(非贼势力_末, -1));
			
			for (int i = 0; i < num; ++i)
			{
				int force_id = top_n_force[i];
				if (pk::is_valid_normal_force_id(force_id))
				{
					pk::force@ force = pk::get_force(force_id);
					if (pk::is_alive(force))
					{
						pk::list<pk::city@> city_list = pk::get_city_list(force);
						if (city_list.count > 3)//最小城市数需求
						{
							possible_alliance[i]= get_one_possible_alliance(top_n_force, force_id, exclude_player);
						}
					}
				}
			}

			return possible_alliance;
		}

		array<int> get_one_possible_alliance(array<int> top_n_force, int force_id, bool exclude_player = true)
		{
			array<int> dst_force_arr(非贼势力_末, -1);
			if (!pk::is_valid_normal_force_id(force_id)) return dst_force_arr;
			pk::force@ force = pk::get_force(force_id);
			if (!pk::is_alive(force)) return dst_force_arr;
			pk::list<pk::city@> city_list = pk::get_city_list(force);
			if (city_list.count == 0) return dst_force_arr;
			int force_count = 0;
			int rank = top_n_force.find(force_id);
			for (int j = 0; j < city_list.count; ++j)
			{
				for (int k = 0; k < 6; ++k)
				{
					int neighbor_id = city_list[j].neighbor[k];
					if (neighbor_id != -1)
					{
						pk::city@ neighbor = pk::get_city(neighbor_id);
						int dst_force_id = neighbor.get_force_id();

						if (pk::is_valid_normal_force_id(dst_force_id))
						{
							if (exclude_player and pk::get_force(dst_force_id).player != -1) continue;
							if (force_ex[dst_force_id].alliance_id != 255) continue;
							//if (pk::is_enemy(city_list[j], neighbor))//还是只排除同盟好了
							if (!force.ally[dst_force_id] and force.relations[dst_force_id] < 80)//盟友或者亲密关系不会加入反联盟
							{
								bool higher = false;//确保不出现大势力创造联盟攻击较小势力
								for (int l = rank; l > -1; --l)
								{
									if (top_n_force[l] == dst_force_id) higher = true;
								}

								if (!higher)
								{
									if (dst_force_arr.find(dst_force_id) < 0)
									{
										dst_force_arr[force_count + 1] = dst_force_id;//第一位存数量
										force_count += 1;
										//pk::trace("加入id" + dst_force_id + ",alliance_id" + force_ex[dst_force_id].alliance_id);
									}
								}//possible_alliance[i].insertLast(dst_force_id);
							}
						}
					}
				}
			}
			dst_force_arr[0] = force_count;
			return dst_force_arr;
		}
/*
		void set_alliance(array<int> top_5_force)
		{
			for (int i = 0; i < int(top_5_force.length); i++)
			{
				int target_force_id = top_5_force[i];

				if (!pk::is_valid_force_id(target_force_id))continue;

				pk::force@ target_force = pk::get_force(target_force_id);

				if (!pk::is_alive(target_force))continue;

				if (target_force_id == force0_id)continue;	// 자기자신 제외 排除己方					

				//if (势力是否交战[force0_id][target_force_id] != 1)continue;	// 연합적과 전쟁중인가

				if (!pk::is_neighbor_force(force0, target_force))continue; // 인접국인가

				// line_bigO++;

				// 나 보다 얼마나 강한가? 
				float 盟主势力对比强弱程度 =
					force_troop_count[target_force_id] /(force_troop_count[force0_id] + force_troop_count[target_force_id]);
				盟主势力对比强弱程度 = 1.0f;//联盟兵力比较 ? 盟主势力对比强弱程度 : 1.0;

				// 우호도
				float relations = pk::max(((100 - force0.relations[target_force_id]) / 100.f), 0.1f);

				float 联盟扩张率 = 0.7f * prob_联盟[target_force_id] * 盟主势力对比强弱程度 * (relations * relations * relations);
				int chosenPossibility = int((联盟扩张率 * leaderEffect) * 100);


				if (getRandom(chosenPossibility, 11)) {

					联合进攻 = target_force_id;
					int result = AllianceAttack(force0, target_force_id);

					联合尝试[force0_id] = 联合尝试拖延_天数;

					return;
				}

			} // end - for				
		}
*/
		bool is_neighbor_2_city(int src, int dst)
		{
			pk::city@ src_city = pk::get_city(src);
			for (int i = 0; i < 6; ++i)
			{
				int neighbor_id = src_city.neighbor[i];
				if (neighbor_id == dst) return true;
				pk::city@ neighbor = pk::get_city(neighbor_id);
				for (int j = 0; j < 6; ++j)
				{
					int neighbor_id2 = neighbor.neighbor[i];
					if (neighbor_id2 == dst) return true;
				}
			}
			return false;
		}

		array<int> bubbleSort(array<int> arr)
		{
			for (int i = 0; i < int(arr.length); i++)
			{
				for (int j = 0; j < int(arr.length) - i; j++)
				{
					if (arr[j] > arr[j + 1])
					{   // 相邻元素两两对比
						int temp = arr[j + 1];        // 元素交换
						arr[j + 1] = arr[j];
						arr[j] = temp;
					}
				}
			}
			return arr;
		}

		array<int> get_top_n(array<int> arr, array<int> arr2, int num)
		{
			array<int>t(num,0);
			array<array<int>> arrt;
			for (int i = 0; i < 非贼势力_末; ++i)
			{
				arrt.insertLast({ i, arr[i], arr2[i] });
			}
			//数组冒泡排序
			for (int i = 0; i < int(arrt.length-1); i++)
			{
				for (int j = 0; j < int(arrt.length-1) - i; j++)
				{
					if (arrt[j][1]* arrt[j][2] < arrt[j + 1][1]*arrt[j + 1][2])
					{   // 相邻元素两两对比
						array<int> temp = arrt[j + 1];        // 元素交换
						arrt[j + 1] = arrt[j];
						arrt[j] = temp;
					}
				}
			}

			for (int i = 0; i < num; ++i)
			{
				t[i] = arrt[i][0];
			}

			return t;
		}

		array<int> get_top_n(array<int> arr,int num)
		{
			array<int>t(num,0);
			array<array<int>> arrt;
			for (int i = 0; i < 非贼势力_末; ++i)
			{
				arrt.insertLast({ i,arr[i] });
			}
			//数组冒泡排序
			for (int i = 0; i < int(arrt.length-1); i++)
			{
				for (int j = 0; j < int(arrt.length-1) - i; j++)
				{
					if (arrt[j][1] > arrt[j + 1][1])
					{   // 相邻元素两两对比
						array<int> temp = arrt[j + 1];        // 元素交换
						arrt[j + 1] = arrt[j];
						arrt[j] = temp;
					}
				}
			}
			for (int i = 0; i < num; ++i)
			{
				t[i] = arrt[i][0];
			}
			return t;
		}

		array<int> get_force_troop_count()
		{
			array<int> 势力兵力数(非贼势力_末, 0);
			for (int i = 0; i < 据点_末; ++i)
			{
				auto base = pk::get_building(i);
				if (pk::is_alive(base))
				{
					int force_id = base.get_force_id();
					if (pk::is_valid_normal_force_id(force_id))
					{
						势力兵力数[force_id] += pk::get_troops(base);
					}
				}
			}
			for (int i = 0; i < 部队_末; ++i)
			{
				auto unit = pk::get_unit(i);
				if (pk::is_alive(unit))
				{
					int force_id = unit.get_force_id();
					if (pk::is_valid_normal_force_id(force_id))
					{
						势力兵力数[force_id] += unit.get_troops();
					}
				}
			}
			return 势力兵力数;
		}

		array<int> get_force_city_count()
		{
			array<int> 势力城市数(非贼势力_末, 0);
			for (int i = 0; i < 城市_末; ++i)
			{
				auto city = pk::get_city(i);
				if (pk::is_alive(city))
				{
					int force_id = city.get_force_id();
					if (pk::is_valid_normal_force_id(force_id))
					{
						势力城市数[force_id] += 1;
					}
				}
			}
			return 势力城市数;
		}

		array<int> get_force_person_count()
		{
			array<int> 势力武将数(非贼势力_末, 0);
			for (int i = 0; i < 非贼武将_末; ++i)
			{
				auto person = pk::get_person(i);
				if (pk::is_alive(person))
				{
					int force_id = person.get_force_id();
					if (pk::is_valid_normal_force_id(force_id))
					{
						势力武将数[force_id] += 1;
					}
				}
			}
			return 势力武将数;
		}

		int get_best_heishu(pk::list<pk::person@> list, int weapon_id)
		{
			if (weapon_id == 兵器_剑) return 0;
			int heishu = 0;
			//if (list[0].get_id() == 武将_典韦) pk::trace("weapon_id" + weapon_id + ",heishu_id" + pk::equipment_id_to_heishu(weapon_id) );
			for (int i = 0; i < list.count; ++i)
			{
				if (list[i] !is null)
				{
					int t = list[i].tekisei[pk::equipment_id_to_heishu(weapon_id)];
					//if (list[0].get_id() == 武将_典韦) pk::trace("weapon_id" + weapon_id + ",heishu_id" + pk::equipment_id_to_heishu(weapon_id) + ",tekisei" + list[i].tekisei[pk::equipment_id_to_heishu(weapon_id)]);
					if (t > heishu) heishu = t;
				}
			}

			return heishu;
		}

		int get_best_weapon_amount(pk::building@ building, int weapon_id, int troops)
		{
			if (weapon_id >= 兵器_剑 and weapon_id <= 兵器_战马)
				return pk::min(pk::get_weapon_amount(building, weapon_id), troops);
			else return 1;		
		}
	}

	Main main;
}

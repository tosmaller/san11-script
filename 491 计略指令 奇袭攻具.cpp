// ## 2022/01/12 # 江东新风 # 无目标对象可执行bug修复 ##
// ## 2021/11/18 # 江东新风 # 为ai添加对应计谋执行 ##
// ## 2021/11/15 # 江东新风 # 使用计略成功率计算方法来计算据点计略成功率，加入失败动画,调整范围,每只部队分别计算成功率 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/30 # 江东新风 # 港关攻心错误修复 ##
// ## 2020/09/23 # 氕氘氚 # 金钱65535上限修复 ##
// ## 2020/07/26 ##


namespace 据点计略_奇袭攻具
{


	// ================ CUSTOMIZE ================


	const int ACTION_COST = 10;    // (행동력 필요량: 기본 40 설정)	
	const int GOLD_COST = 1000;     // (금 사용량: 기본 500 설정)
	const int TROOP_COST = 700;     // 奇袭兵力消耗/每只攻城部队
	const int TROOP_DAMAGE_STD = 900;     // 攻城兵器受伤基数

	const int min_distance = 1;      // 城市默认在此范围加1，从而和港口区分
	const int max_distance = 3;      // 城市默认在此范围加1，从而和港口区分

	const int KEY = pk::hash("奇袭攻具");


	// ===========================================


	class Main
	{
		bool 调试模式 = false;
		pk::building@ building_;
		pk::force@ force_;
		pk::person@ taishu_;
		pk::person@ actor;
		pk::city@ city_;

		Main()
		{
			pk::bind(202, pk::trigger202_t(onAIRunningOrder));
			add_menu();
		}

		void add_menu()
		{
			pk::menu_item item;
			item.menu = 104;
			item.shortcut = "3";
			item.init = pk::building_menu_item_init_t(init);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@taishu_ = pk::get_person(pk::get_taishu_id(building));
			@city_ = pk::building_to_city(building);
		}

		string getText()
		{
			return pk::encode("奇袭攻具");
		}

		bool isEnabled()
		{
			auto person_list = pk::get_idle_person_list(building_);
			int result = check_avaliable(building_, person_list);
			if (result != 0) return false;
			else return true;
		}

		string getDesc()
		{
			auto person_list = pk::get_idle_person_list(building_);
			int result = check_avaliable(building_, person_list);
			switch (result)
			{
			case 1: return pk::encode("周围没有攻城兵器, 无法执行");
			case 2: return pk::encode("没有可执行的武将. ");
			case 3: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			case 4: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			case 5: return pk::encode(pk::format("兵力不足 (必须 {} 兵力)", TROOP_COST));
			case 0: return pk::encode(pk::format("奇袭敌方攻具.(消耗：兵力{} 金{} 行动力{})", TROOP_COST, GOLD_COST, ACTION_COST));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		int check_avaliable(pk::building@ building, pk::list<pk::person@> list)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			if (!is_seige_weapon_around(building)) return 1;
			else if (list.count == 0) return 2;
			else if (pk::get_district(building.get_district_id()).ap < ACTION_COST) return 3;
			else if (pk::get_gold(building) < GOLD_COST) return 4;
			else if (pk::get_troops(building) < TROOP_COST) return 5;
			else return 0;
		}

		bool handler()
		{

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("是否奇袭敌方攻城兵器?\n(削减敌方攻城兵器战力)\n(消耗：兵力{}/每只 金{} 行动力{})", TROOP_COST, GOLD_COST, ACTION_COST)), taishu_) == 1)
				return false;

			// 실행가능 무장리스트
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building_);
			if (person_list.count == 0) return false;

			// 실행무장 선택하기
			pk::list<pk::person@> person_sel;
			person_list.sort(function(a, b)
			{
				return (a.stat[武将能力_智力] > b.stat[武将能力_智力]); // 무장 정렬 (지력순)
			});
			person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择可执行的武将."), person_list, 1, 1);
			if (person_sel.count == 0) return false; // 미선택 시 취소 종료


			return run_order(person_sel, building_);

		}


		void onAIRunningOrder(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 据点AI_计略奇袭攻具)
			{
				pk::list<pk::person@> person_sel;
				if (run_order_before(person_sel, building)) run_order(person_sel, building);
				else if (调试模式) pk::trace(pk::get_new_base_name(building.get_id()) + "据点AI_计略奇袭攻具 不满足");
			}
		}


		bool run_order_before(pk::list<pk::person@>& out person_sel, pk::building@ building0)
		{
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building0);
			if (person_list.count == 0) return false;
			person_list.sort(function(a, b)//武将选择这块还得加强
			{
				return (a.stat[武将能力_智力] > b.stat[武将能力_智力]); // 무장 정렬 (지력순)
			});
			person_sel.add(person_list[0]);
			if (check_avaliable(building0, person_sel) != 0) return false;

			pk::point src_pos = building0.pos;
			array<pk::point> arr = pk::range(building0.get_pos(), min_distance + ((building0.facility == 设施_城市) ? 1 : 0), max_distance + ((building0.facility == 设施_城市) ? 1 : 0));
			int count = 0;
			for (int j = 0; j < int(arr.length); j++)
			{
				pk::unit@ dst_unit = pk::get_unit(arr[j]);

				if (dst_unit is null or !pk::is_enemy(building0, dst_unit)) continue;
				if (dst_unit.weapon < 5 or dst_unit.weapon > 8) continue;
				//成功率里已经有对应特技效果了
				//if (dst_unit.has_skill(특기_침착) or dst_unit.has_skill(특기_명경) or dst_unit.has_skill(특기_통찰) or dst_unit.has_skill(특기_신산)) continue;

				pk::person@ dst_leader = pk::get_person(dst_unit.leader);


				pk::int_bool result0 = STRATEGY_CHANCE::cal_strategy_chance(building0, person_sel, dst_unit.pos, 据点计略_奇袭攻具);//int_bool 成功率 是否被看破
				if (result0.second) continue;
				if (result0.first > 20) count += 1;
			}
			if (count > 0)
			{
				return true;
			}
			return false;
		}


		bool run_order(pk::list<pk::person@> person_sel, pk::building@ building0)
		{
			if (building0 is null) return false;
			if (person_sel.count == 0) return false;

			pk::point src_pos = building0.pos;
			array<pk::point> arr = pk::range(building0.get_pos(), min_distance + ((building0.facility == 设施_城市) ? 1 : 0), max_distance + ((building0.facility == 设施_城市) ? 1 : 0));
			int count = 0;
			int troop_damage = 0;
			//pk::person@ attacker;
			//pk::unit@ atk_unit;
			for (int j = 0; j < int(arr.length); j++)
			{
				auto hex = pk::get_hex(arr[j]);
				if (!hex.has_unit) continue;
				pk::unit@ dst_unit = pk::get_unit(arr[j]);

				if (dst_unit is null or !pk::is_enemy(building0, dst_unit)) continue;
				if (dst_unit.weapon < 5 or dst_unit.weapon > 8) continue;
				//成功率里已经有对应特技效果了
				//if (dst_unit.has_skill(특기_침착) or dst_unit.has_skill(특기_명경) or dst_unit.has_skill(특기_통찰) or dst_unit.has_skill(특기_신산)) continue;

				pk::person@ dst_leader = pk::get_person(dst_unit.leader);


				pk::int_bool result0 = STRATEGY_CHANCE::cal_strategy_chance(building0, person_sel, dst_unit.pos, 据点计略_奇袭攻具);//int_bool 成功率 是否被看破
				//result0.first = 100;
				if (result0.second)//看破
				{
					if (pk::is_in_screen(src_pos))
					{
						pk::create_effect(84, dst_unit.pos);
						pk::play_se(95, dst_unit.pos);
						pk::play_voice(69, dst_leader);
					}
				}
				else if (pk::rand_bool(pk::min(100, result0.first)))//计策成功
				{
					count += 1;//影响部队技计数
					if (count == 1)
					{
						if (pk::is_in_screen(building0.pos))
						{
							pk::play_se(0x39, building0.pos);
							pk::create_effect(0x3a, building0.pos);
							switch (pk::rand(2))
							{
							case 0: pk::play_voice(0x1C, person_sel[0]); break; // 음성 : 전투_공격
							case 1: pk::play_voice(0x1D, person_sel[0]); break; // 음성 : 전투_공격
							}
							/*
							@attacker = pk::create_bandit(person_sel[0]);

							pk::force@ force = pk::get_force(person_sel[0].get_force_id());
							int district_num = pk::get_district_id(force, 1);

							// 분대 군단 설정
							pk::set_district(attacker, district_num);
							attacker.mibun = 身份_一般;
							attacker.birth = pk::get_year() - 30;
							attacker.death = pk::min(353, attacker.birth + 99);
							attacker.appearance = pk::get_year() - 10;
							// 능력치 : 본대의 80%
							attacker.base_stat[武将能力_统率] = int(1.0f * person_sel[0].stat[武将能力_统率]);
							attacker.base_stat[武将能力_武力] = int(1.0f * person_sel[0].stat[武将能力_武力]);
							attacker.base_stat[武将能力_智力] = int(1.0f * person_sel[0].stat[武将能力_智力]);
							attacker.base_stat[武将能力_政治] = int(1.0f * person_sel[0].stat[武将能力_政治]);
							attacker.base_stat[武将能力_魅力] = int(1.0f * person_sel[0].stat[武将能力_魅力]);
							attacker.skill = 特技_驱逐;
							

							// 능력치 반영.
							attacker.update();

							int troops = TROOP_COST;
							int gold = 0;
							int food = troops;

							@atk_unit = pk::create_unit(building0, attacker, null, null, 5000, 兵器_枪, 兵器_走舸, gold, food, building0.pos, false);
							pk::set_energy(atk_unit, 120);  // 본부대 기력값 반영
							*/
							if (pk::is_in_screen(building0.pos))
								pk::create_effect(0x4b, building0.pos);//伏兵的迷雾
							if (pk::is_in_screen(dst_unit.pos))
							{
								switch (pk::rand(4))
								{
								case 0: pk::say(pk::encode("大意了..."), pk::get_person(dst_unit.leader)); break;
								case 1: pk::say(pk::encode("敌人的奇袭?"), pk::get_person(dst_unit.leader)); break;
								case 2: pk::say(pk::encode("可恶，我们的攻城兵器..."), pk::get_person(dst_unit.leader)); break;
								case 3: pk::say(pk::encode("仓促之间，实在难以应对..."), pk::get_person(dst_unit.leader)); break;
								}
							}
							//只针对玩家对象时显示log提示
							if (dst_unit.is_player()) pk::history_log(building0.pos, (pk::get_force(building0.get_force_id())).color, pk::encode(pk::format("\x1b[1x{}\x1b[0x发动了奇袭攻具.", pk::decode(pk::get_name(person_sel[0])))));

						}

					}


					//pk::attack(atk_unit, dst_unit);  // 적 부대에 공격 실행
					pk::point temp_pos = pk::point(dst_unit.pos.x, dst_unit.pos.y-1);
					if (pk::is_in_screen(dst_unit.pos)) attacked(24, temp_pos, dst_unit.pos);
					ch::add_troops(dst_unit, -(TROOP_DAMAGE_STD * 100/ dst_unit.attr.stat[部队能力_防御] + pk::rand(50)), true,14);
					if (dst_unit.troops == 0) pk::kill(dst_unit);
					pk::add_energy(dst_unit, -10, true);
					troop_damage += TROOP_COST * dst_unit.attr.stat[部队能力_攻击] / 100;
					if (pk::get_troops(building0) < troop_damage) break;//兵力不够将不会继续突袭

					if (pk::rand_bool(5))       // 
						pk::set_status(dst_unit, null, 部队状态_混乱, 2, true);

					//  // 分队兵力归队 분대 병력 본대 귀환


				}
				else//计策失败
				{
					if (pk::is_in_screen(dst_unit.pos))
					{
						pk::play_voice(70, dst_leader);
						ch::say_message(7206, /*p0*/dst_leader, /*p1*/null, /*u0*/null, /*p0_u*/dst_unit);//沒用的	
					}
				}
			}

			//if (atk_unit !is null) pk::kill(atk_unit);                 // 분대 해산
			//if (attacker !is null) pk::reset(attacker);

			if (pk::is_in_screen(src_pos))
			{
				if (count > 0)
				{
					if (pk::rand_bool(50)) ch::say_message(7149, /*p0*/person_sel[0], /*p1*/null, /*b0*/null, /*p0_b*/building0);
					else pk::say(pk::encode("机不可失，干掉敌军的攻城兵器吧！"), person_sel[0], building0);
				}
				else
				{
					pk::say(pk::encode("敌人的防备比预计的要森严不少呀."), person_sel[0], building0);//原版san11计略失败不会说话
				}
			}
			//pk::trace("actor name" + pk::decode(pk::get_name(person_sel[0])));
			//兵力损失按计策成功与否计算
			
			// 행동력 감소.
			auto district = pk::get_district(building0.get_district_id());
			pk::add_ap(district, -ACTION_COST);

			// 실행무장 행동완료
			person_sel[0].action_done = true;

			// 금 감소
			pk::add_gold(building0, -GOLD_COST, true);

			ch::add_troops(building0, -troop_damage, true, 14);

			base_ex[building0.get_id()].raid_done = true;

			return true;

		}

		bool attacked(int id, pk::point dst_pos/*攻击方方向*/, pk::point src_pos)
		{
			pk::damage_info info;
			info.src_pos = dst_pos;
			info.dst_pos = src_pos;
			info.troops_damage = 1;
			pk::unit_anim(pk::get_unit(src_pos), id, { info });
			pk::wait(1);
			return true;
		}

		bool is_seige_weapon_around(pk::building@ building)
		{
			pk::point src_pos = building.pos;
			array<pk::point> arr = pk::range(building.get_pos(), min_distance + ((building.facility == 设施_城市) ? 1 : 0), max_distance + ((building.facility == 设施_城市) ? 1 : 0));
			int count = 0;
			for (int j = 0; j < int(arr.length); j++)
			{
				auto hex = pk::get_hex(arr[j]);
				if (!hex.has_unit) continue;
				pk::unit@ dst_unit = pk::get_unit(arr[j]);
				if (dst_unit is null or !pk::is_enemy(building, dst_unit)) continue;
				if (dst_unit.weapon < 5 or dst_unit.weapon > 8) continue;
				return true;
			}
			return false;
		}
	}

	Main main;
}
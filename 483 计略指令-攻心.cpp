// ## 2021/11/18 # 江东新风 # 为ai添加对应计谋执行 ##
// ## 2021/11/15 # 江东新风 # 使用计略成功率计算方法来计算据点计略成功率，加入失败动画,调整范围,每只部队分别计算成功率 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/30 # 江东新风 # 港关攻心错误修复 ##
// ## 2020/09/23 # 氕氘氚 # 金钱65535上限修复 ##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: 도시 근처에 있는 적군 부대의 병력 흡수 효과

※ 계략은 실행무장의 지력 수치에 따라서 결정됩니다.

  EX) 서서 지력 93 => 성공률 93%
  EX) 장비 지력 30 => 성공률 30%

*/

namespace 据点计略_攻心
{


	// ================ CUSTOMIZE ================


	const int ACTION_COST = 30;    // (행동력 필요량: 기본 40 설정)	
	const int GOLD_COST = 1500;     // (금 사용량: 기본 500 설정)

	const int min_distance = 1;      // 城市默认在此范围加1，从而和港口区分
	const int max_distance = 3;      // 城市默认在此范围加1，从而和港口区分

	const int KEY = pk::hash("심공");


	// ===========================================


	class Main
	{
		bool 调试模式 = false;
		pk::building@ building_;
		pk::force@ force_;
		pk::person@ taishu_;
		pk::person@ actor;
		pk::city@ city_;
		int building_gold;

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
			building_gold = pk::get_gold(building);
		}

		string getText()
		{
			return pk::encode("攻心");
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
			case 1: return pk::encode("周围没有敌人, 无法执行");
			case 2: return pk::encode("已执行过该计略");
			case 3: return pk::encode("没有可执行的武将. ");
			case 4: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			case 5: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			case 0: return pk::encode(pk::format("执行攻心.(消耗：金{} 行动力{})", GOLD_COST, ACTION_COST));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		int check_avaliable(pk::building@ building, pk::list<pk::person@> list)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			if (!pk::enemies_around(building)) return 1;
			else if (base_ex[building.get_id()].beguile_done) return 2;
			else if (list.count == 0) return 3;
			else if (pk::get_district(building.get_district_id()).ap < ACTION_COST) return 4;
			else if (pk::get_gold(building) < GOLD_COST) return 5;
			else return 0;
		}

		bool handler()
		{

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("是否执行攻心计谋的操作?\n(使用资金 {} )\n(对敌人部队的兵力具有吸收效果)", GOLD_COST)), taishu_) == 1)
				return false;

			// 실행가능 무장리스트
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building_);
			if (person_list.count == 0) return false;

			// 실행무장 선택하기
			pk::list<pk::person@> person_sel;
			if (pk::is_player_controlled(building_))
			{
				person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择可执行的武将."), person_list, 1, 1);
				if (person_sel.count == 0) return false; // 미선택 시 취소 종료
			}
			else
			{
				person_list.sort(function(a, b)
				{
					return (a.stat[武将能力_智力] > b.stat[武将能力_智力]); // 무장 정렬 (지력순)
				});
			}

			return run_order(person_sel, building_);

		}


		void onAIRunningOrder(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 据点AI_计略攻心)
			{
				pk::list<pk::person@> person_sel;
				if (run_order_before(person_sel, building)) run_order(person_sel, building);
				else if (调试模式) pk::trace("据点AI_计略攻心 不满足");
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

				//成功率里已经有对应特技效果了
				//if (dst_unit.has_skill(특기_침착) or dst_unit.has_skill(특기_명경) or dst_unit.has_skill(특기_통찰) or dst_unit.has_skill(특기_신산)) continue;

				pk::person@ dst_leader = pk::get_person(dst_unit.leader);


				pk::int_bool result0 = STRATEGY_CHANCE::cal_strategy_chance(building0, person_sel, dst_unit.pos, 据点计略_攻心);//int_bool 成功率 是否被看破
				if (result0.second) continue;
				if (result0.first > 40) count += 1;
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
			if (person_sel[0] is null) return false;

			pk::point src_pos = building0.pos;
			array<pk::point> arr = pk::range(building0.get_pos(), min_distance + ((building0.facility == 设施_城市) ? 1 : 0), max_distance + ((building0.facility == 设施_城市) ? 1 : 0));
			int count = 0;
			for (int j = 0; j < int(arr.length); j++)
			{
				pk::unit@ dst_unit = pk::get_unit(arr[j]);

				if (dst_unit is null or !pk::is_enemy(building0, dst_unit)) continue;

				//成功率里已经有对应特技效果了
				//if (dst_unit.has_skill(특기_침착) or dst_unit.has_skill(특기_명경) or dst_unit.has_skill(특기_통찰) or dst_unit.has_skill(특기_신산)) continue;

				pk::person@ dst_leader = pk::get_person(dst_unit.leader);


				pk::int_bool result0 = STRATEGY_CHANCE::cal_strategy_chance(building0, person_sel, dst_unit.pos, 据点计略_攻心);//int_bool 成功率 是否被看破

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
					ch::add_troops(dst_unit, -dst_unit.troops / 10, true);

					ch::add_troops(building0, dst_unit.troops / 10, true);

					if (pk::is_in_screen(dst_unit.get_pos()))
					{
						switch (pk::rand(2))
						{
						case 0: pk::say(pk::encode("请接受我们的投降!"), pk::get_person(무장_병사), dst_unit); break;
						case 1: pk::say(pk::encode("为了家人们谋个生路.."), pk::get_person(무장_병사), dst_unit); break;
						}
					}
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

			if (pk::is_in_screen(src_pos))
			{
				if (count > 0)
				{
					pk::say(pk::encode("哈哈哈! 计谋成功了."), person_sel[0], building0);
				}
				else
				{
					pk::say(pk::encode("很可惜智谋失败了."), person_sel[0], building0);//原版san11计略失败不会说话
				}
			}

			// 행동력 감소.
			auto district = pk::get_district(building0.get_district_id());
			pk::add_ap(district, -ACTION_COST);

			// 실행무장 행동완료
			person_sel[0].action_done = true;

			// 금 감소
			pk::add_gold(building0, -GOLD_COST, true);

			base_ex[building0.get_id()].beguile_done = true;

			return true;

		}

	}

	Main main;
}
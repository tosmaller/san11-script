<<<<<<< HEAD
﻿// ## 2021/11/18 # 江东新风 # 为ai添加对应计谋执行 ##
=======
﻿// ## 2022/03/26 # 江东新风 # run_order相关bug修复 ##
// ## 2021/11/18 # 江东新风 # 为ai添加对应计谋执行 ##
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
// ## 2021/11/15 # 江东新风 # 使用计略成功率计算方法来计算据点计略成功率，加入失败动画,调整范围,每只部队分别计算成功率 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/30 # 江东新风 # 港关计略city指针错误修复 ##
// ## 2020/08/06 #江东新风#修正了周围无可执行对象时可执行计略的bug##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: 도시 근처에 있는 아군 부대에게 진정 효과 (혼란/위보 상태 -> 통상 상태, 진정된 부대 미행동으로 전환)

※ 계략은 실행무장의 지력 수치에 따라서 결정됩니다.

  EX) 서서 지력 93 => 성공률 93%
  EX) 장비 지력 30 => 성공률 30%

*/

namespace 据点计略_镇静
{


	// ================ CUSTOMIZE ================


	const int ACTION_COST = 10;      // (행동력 필요량: 기본 40 설정)	
	const int GOLD_COST = 1000;     	 // (금 사용량: 기본 500 설정)

	const int min_distance = 1;      // 城市默认在此范围加1，从而和港口区分
	const int max_distance = 5;      // 城市默认在此范围加1，从而和港口区分

	const int KEY = pk::hash("镇静");


	// ===========================================


	class Main
	{
		bool 调试模式 = false;
		pk::building@ building_;
		pk::force@ force_;
		pk::person@ taishu_;
		//pk::person@ actor;
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
			item.shortcut = "5";
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
			return pk::encode("镇静");
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
			case 1: return pk::encode("据点周围无可执行计略的友军");
			case 2: return pk::encode("没有可执行的武将. ");
			case 3: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			case 4: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			case 0: return pk::encode(pk::format("执行镇静.(消耗：金{} 行动力{})", GOLD_COST, ACTION_COST));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		int check_avaliable(pk::building@ building, pk::list<pk::person@> list, int type = 0)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			if (type == 1 and !is_src_around(building,list, 1)) return 1;
			else if (type == 0 and !is_src_around(building, list)) return 1;
			else if (list.count == 0) return 2;
			else if (pk::get_district(building.get_district_id()).ap < ACTION_COST) return 3;
			else if (pk::get_gold(building) < GOLD_COST) return 4;
			else return 0;
		}

		bool handler()
		{

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("是否执行镇静?\n(资金 {} 使用)\n(镇静据点周围友方部队)", GOLD_COST)), taishu_) == 1)
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
			if (cmd == 据点AI_计略镇静)
			{
				pk::list<pk::person@> person_sel;
				if (run_order_before(person_sel, building)) run_order(person_sel, building);
				else if (调试模式) pk::trace("据点AI_计略镇静 不满足");
			}
		}


		bool run_order_before(pk::list<pk::person@>& out person_sel, pk::building@ building0)
		{
			//if (!is_src_around(building0)) return false;//3格范围有敌军返回true
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building0);
			if (person_list.count == 0) return false;
			person_list.sort(function(a, b)//武将选择这块还得加强
			{
				return (a.stat[武将能力_智力] > b.stat[武将能力_智力]); // 무장 정렬 (지력순)
			});
			person_sel.add(person_list[0]);
			if (check_avaliable(building0, person_sel,1) != 0) return false;

			return true;
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
<<<<<<< HEAD
				pk::unit@ dst_unit = pk::get_unit(arr[j]);

				if (dst_unit is null or dst_unit.status == 部队状态_通常 or pk::is_enemy(building_, dst_unit)) continue;
=======
				auto hex = pk::get_hex(arr[j]);
				if (!hex.has_unit) continue;
				pk::unit@ dst_unit = pk::get_unit(arr[j]);

				if (dst_unit is null or dst_unit.status == 部队状态_通常 or pk::is_enemy(building0, dst_unit)) continue;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

				//成功率里已经有对应特技效果了
				//if (dst_unit.has_skill(특기_침착) or dst_unit.has_skill(특기_명경) or dst_unit.has_skill(특기_통찰) or dst_unit.has_skill(특기_신산)) continue;

				pk::person@ dst_leader = pk::get_person(dst_unit.leader);


				pk::int_bool result0 = STRATEGY_CHANCE::cal_strategy_chance(building0, person_sel, dst_unit.pos, 据点计略_镇静);//int_bool 成功率 是否被看破

				if (result0.second)//看破--镇静不会触发
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

					pk::set_status(dst_unit, null, 部队状态_通常, 2, true);

					dst_unit.action_done = false;
					dst_unit.update();

					if (pk::is_in_screen(building_.get_pos()))
					{
						ch::say_message(7132, /*p0*/dst_leader, /*p1*/null, /*u0*/null, /*p0_u*/dst_unit);	//快點恢復正常	
					}
				}
				else//计策失败
				{
				}
			}

			if (pk::is_in_screen(src_pos))
			{
				if (count > 0)
				{
					ch::say_message(7122, /*p0*/person_sel[0], /*p1*/null, /*u0*/null, /*p0_u*/building0);	//快點恢復正常	
				}
				else
				{
					ch::say_message(7139, /*p0*/person_sel[0], /*p1*/null, /*u0*/null, /*p0_u*/building0);	//我的聲音似乎\n無法傳達到……
				}
			}

			// 행동력 감소.
			auto district = pk::get_district(building0.get_district_id());
			pk::add_ap(district, -ACTION_COST);

			// 실행무장 행동완료
			person_sel[0].action_done = true;

			// 금 감소
			pk::add_gold(building0, -GOLD_COST, true);

			return true;

		}

		bool is_src_around(pk::building@ building,pk::list<pk::person@> person_list, int type =0)//type 为1时判断成功率
		{
			array<pk::point> arr = pk::range(building.get_pos(), min_distance + ((building.facility == 设施_城市) ? 1 : 0), max_distance + ((building.facility == 设施_城市) ? 1 : 0));
			int count = 0;
			for (int l = 0; l < int(arr.length); l++)
			{
				pk::unit@ dst_unit = pk::get_unit(arr[l]);
				if (dst_unit is null or pk::is_enemy(building, dst_unit)) continue;
				if (dst_unit.status == 部队状态_通常) continue;
				if (type == 1)
				{
					pk::person@ dst_leader = pk::get_person(dst_unit.leader);
					pk::int_bool result0 = STRATEGY_CHANCE::cal_strategy_chance(building, person_list, dst_unit.pos, 据点计略_镇静);//int_bool 成功率 是否被看破
					if (result0.second) continue;
					if (result0.first > 40) count += 1;
				}
				else return true;

			}
			if (count > 1)
			{
				return true;				
			}

			return false;
		}

	}

	Main main;
}
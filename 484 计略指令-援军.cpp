// ## 2022/01/15 # 江东新风 # 一回合一次，消耗相等兵力--ai执行部分暂未写入 ##
// ## 2021/11/15 # 江东新风 # 使用计略成功率计算方法来计算据点计略成功率，加入失败动画 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/09/30 # 江东新风 # 港关计略city指针错误修复 ##
// ## 2020/09/23 # 氕氘氚 # 金钱65535上限修复 ##
// ## 2020/08/06 #江东新风#可选擇支援部队数，根据支援部队数消耗金錢增加，支援多队打八折, 失敗部队打五折##
// ## 2020/08/06 #江东新风#修正了周围无可执行对象时可执行计略的bug，修改效果?只可支援一支部队##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: 도시 근처에 있는 아군 부대에게 병력 지원 효과

※ 계략은 실행무장의 지력 수치에 따라서 결정됩니다.

  EX) 서서 지력 93 => 성공률 93%
  EX) 장비 지력 30 => 성공률 30%

*/

namespace 据点计略_援军
{


	// ================ CUSTOMIZE ================


	const int ACTION_COST = 30;     // (행동력 필요량: 기본 40 설정)	
	const int GOLD_COST = 600;      // (금 사용량: 기본 500 설정)

	const int troops_0 = 1500;       // 원군으로 회복되는 부대의 병력 (기본 1500 설정)

	const int min_distance = 1;      // 도시 근처 최소범위 
	const int max_distance = 5;      // 도시 근처 최대범위  

	const int KEY = pk::hash("援军");


	// ===========================================


	class Main
	{
		bool 调试模式 = false;
		pk::building@ building_;
		int check_result_;
		Main()
		{
			pk::menu_item item;
			item.menu = 104;
			item.shortcut = "4";
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
			auto person_list = pk::get_idle_person_list(building_);
			check_result_ = check_avaliable(building, person_list);
		}

		string getText()
		{
			return pk::encode("援军");
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
			case 1: return pk::encode("周围没有友军, 无法执行");
			case 2: return pk::encode("已执行过该计略");
			case 3: return pk::encode("没有可执行的武将. ");
			case 4: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			case 5: return pk::encode(pk::format("资金不足 (至少 {} 资金)", GOLD_COST));
			case 6: return pk::encode(pk::format("兵力不足 (至少 {} 兵力)", troops_0));
			case 0: return pk::encode(pk::format("支援周围友军.(消耗：(金{}+兵力{})/部队 行动力{})", GOLD_COST, troops_0, ACTION_COST));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		int check_avaliable(pk::building@ building, pk::list<pk::person@> list)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			//pk::trace(pk::format("3.1:{}{}{}{}", pk::enemies_around(building), list.count, pk::get_district(building.get_district_id()).ap < ACTION_COST, pk::get_gold(building) < GOLD_COST));
			if (!is_src_around()) return 1;
			else if (base_ex[building.get_id()].reinforce_done) return 2;
			else if (list.count == 0) return 3;
			else if (pk::get_district(building.get_district_id()).ap < ACTION_COST) return 4;
			else if (pk::get_gold(building) < GOLD_COST) return 5;
			else if (pk::get_troops(building) < troops_0) return 6;
			else return 0;
		}

		bool handler()
		{
			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("支援周围友军.\n(消耗：(金{}+兵力{})/部队 行动力{})", GOLD_COST, troops_0, ACTION_COST)), pk::get_person(武将_文官)) == 1)
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
			

			bool unit_confirm = false;
			pk::list<pk::unit@> src_list;
			src_list = getsrcList();
			pk::unit@selected_unit_;
			pk::list<pk::unit@>unit_sel;
			while (!unit_confirm)
			{
				// 무장 선택 창을 열어서 선택하기
				int max = pk::min(10,int(pk::get_gold(building_)/GOLD_COST),int(pk::get_troops(building_)/troops_0));
				unit_sel = pk::unit_selector(pk::encode("选择要支援的部队"), pk::encode("是否支援该部队."), src_list, 1, max);

				if (unit_sel.count == 0) return false;

				unit_confirm = pk::yes_no(pk::encode(pk::format("是否要支援所选\x1b[1x{}\x1b[0x只部队?", unit_sel.count)));
			}

			return run_order(person_sel, unit_sel,building_);

		}


		void onAIRunningOrder(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 据点AI_计略援军)
			{
				pk::list<pk::person@> person_sel;
				pk::list<pk::unit@>unit_sel;
				if (run_order_before(person_sel, unit_sel, building)) run_order(person_sel, unit_sel, building);
				else if (调试模式) pk::trace("据点AI_计略援军 不满足");
			}
		}

		bool run_order_before(pk::list<pk::person@>& out person_sel, pk::list<pk::unit@>& out unit_sel, pk::building@ building0)
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

				if (dst_unit is null or dst_unit.status == 部队状态_混乱 or !pk::is_enemy(building0, dst_unit)) continue;

				//成功率里已经有对应特技效果了
				//if (dst_unit.has_skill(특기_침착) or dst_unit.has_skill(특기_명경) or dst_unit.has_skill(특기_통찰) or dst_unit.has_skill(특기_신산)) continue;

				pk::person@ dst_leader = pk::get_person(dst_unit.leader);


				pk::int_bool result0 = STRATEGY_CHANCE::cal_strategy_chance(building0, person_sel, dst_unit.pos, 据点计略_扰乱);//int_bool 成功率 是否被看破
				if (result0.second) continue;
				if (result0.first > 10) count += 1;
			}
			if (count > 0)
			{
				return true;
			}
			return false;
		}

		bool run_order(pk::list<pk::person@> person_sel, pk::list<pk::unit@> unit_sel, pk::building@ building0)
		{
			if (building0 is null) return false;
			if (person_sel.count == 0) return false;
			if (unit_sel.count == 0) return false;

			int 支援成功数 = 0;
			pk::unit@selected_unit_;
			for (int i = 0; i < unit_sel.count; i++)
			{
				@selected_unit_ = unit_sel[i];
				pk::int_bool result0 = STRATEGY_CHANCE::cal_strategy_chance(building0, person_sel, selected_unit_.pos, 据点计略_援军);//int_bool 成功率 是否被看破
				if (pk::rand_bool(result0.first))
				{
					支援成功数 += 1;
					ch::add_troops(selected_unit_, troops_0, true);
					if (pk::is_in_screen(building0.get_pos()))
					{
						switch (pk::rand(2))
						{
						case 0: pk::say(pk::encode("援军已经到了, 加油吧!"), pk::get_person(selected_unit_.leader)); break;
						case 1: pk::say(pk::encode("本城里来了援军!"), pk::get_person(selected_unit_.leader)); break;
						}
					}
				}
			}

			if (支援成功数 == unit_sel.count)
			{
				pk::say(pk::encode("哈哈哈! 支援大获成功."), person_sel[0], building0);
			}
			else if ((支援成功数 < unit_sel.count) and (支援成功数 > 0))
			{
				pk::say(pk::encode("如此情况下, 支援比想象的要困难呀."), person_sel[0], building0);
			}
			else if (支援成功数 == 0)
			{
				pk::say(pk::encode("局势过于不利, 支援没有成功."), person_sel[0], building0);
			}


			// 행동력 감소.
			auto district = pk::get_district(building0.get_district_id());
			pk::add_ap(district, -ACTION_COST);

			// 실행무장 행동완료
			person_sel[0].action_done = true;
			int 支援失敗数 = unit_sel.count - 支援成功数;
			int ALL_GOLD_COST = int(支援成功数 * GOLD_COST + 支援失敗数 * 0.5f * GOLD_COST);
			int ALL_TROOPS_COST = int(支援成功数 * troops_0 + 支援失敗数 * 0.5f * troops_0);
			// 금 감소
			pk::add_gold(building0, -ALL_GOLD_COST, true);
			pk::add_troops(building0, -ALL_TROOPS_COST, true);
			base_ex[building0.get_id()].reinforce_done = true;
			return true;
		}

		//==============================================支持函数=================================================//
		bool is_src_around()
		{
			array<pk::point> arr = pk::range(building_.get_pos(), min_distance, max_distance);
			for (int l = 0; l < int(arr.length); l++)
			{
				pk::unit@ dst = pk::get_unit(arr[l]);
				if (dst is null or pk::is_enemy(building_, dst)) continue;
				if (int(dst.troops) >= pk::get_max_troops(dst)) continue;
				return true;
			}
			return false;
		}

		pk::list<pk::unit@> getsrcList()
		{
			pk::list<pk::unit@> list;
			array<pk::point> arr = pk::range(building_.get_pos(), min_distance, max_distance);
			for (int i = 0; i < int(arr.length); i++)
			{
				pk::unit@ dst = pk::get_unit(arr[i]);
				//pk::trace(pk::format("pos0", 1));
				if (dst is null) continue;
				//pk::trace(pk::format("pos1,dst:{}", dst.get_id()));
				if (pk::is_enemy(building_, dst)) continue;
				//pk::trace(pk::format("pos2,dst:{},not enemy", dst.get_id()));
				if (int(dst.troops) >= pk::get_max_troops(dst)) continue;
				list.add(dst);
			}
			return list;
		}

	}

	Main main;
}
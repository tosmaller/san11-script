// ## 2021/11/20 # 江东新风 # 仅限围城下执行，仅限动员本城武将，降低数量，为ai添加对应计谋执行 ##
// ## 2021/11/15 # 江东新风 # 每回合限制1次 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/09/21 # 江东新风 # 无紧急动员对象时按钮为灰色 ##
// ## 2020/08/05 #messi#修改了指令名称和語句文本，平衡了参数##
// ## 2020/08/04 #江东新风#工坊造船的次数不再受命令权限影响##
// ## 2020/08/03 #江东新风#修复进行工坊造船等武将能被恢复行动的bug##
// ## 2020/08/03 #江东新风#修复行动力溢出bug, 改进汉化表述##
// ## 2020/07/26 ##
/*
@ 제작자: HoneyBee
@ 내용: 도시에서 이미 행동을 완료한 무장을 미행동으로 변경 및 도시 명령을 미완료 상태로 변경시킬 수 있는 SCRIPT

※ 효과 참고
- 행동 완료한 무장을 미행동 상태로 변경
- 완료한 도시 명령을 미완료 상태로 변경
- 행동력 회복


== 태수의 관직 및 신분 조건에 따른 미행동 무장 변환 가능 인원 ==

- 无 관직 ~ 평북장군 미만 (최대 3人)
- 평북장군 이상 ~ 후장군 미만 (최대 4人)
- 후장군 이상 ~ 진북장군 미만 (최대 5人)
- 진북장군 이상 ~ 거기장군 미만 (최대 6人)
- 거기장군 이상 or 태수의 신분이 [군주, 도독] (최대 7人)

*/

namespace 紧急动员//명령_권한
{

	// ================ CUSTOMIZE ================

	const int TP_COST = 100; 	   		// 기교P 필요량
	const int GOLD_COST = 500; 	   		// 금 필요량
	const int ACTION_GAIN = 100;    	// 행동력 회복량

	const int 官职条件_0 = 官职_平北将军;	    // 명령권한을 수행하는 태수의 관직 조건
	const int 官职条件_1 = 官职_后将军;
	const int 官职条件_2 = 官职_镇北将军;
	const int 官职条件_3 = 官职_车骑将军;

	const int 最大人数_0 = 2;				// 미행동 무장으로 변환할 수 있는 최대인원 (无 관직 ~ 평북장군 미만)
	const int 最大人数_1 = 4;				// 미행동 무장으로 변환할 수 있는 최대인원 (평북장군 이상 ~ 후장군 미만)
	const int 最大人数_2 = 6;				// 미행동 무장으로 변환할 수 있는 최대인원 (후장군 이상 ~ 진북장군 미만)
	const int 最大人数_3 = 8;				// 미행동 무장으로 변환할 수 있는 최대인원 (진북장군 이상 ~ 거기장군 미만)
	const int 最大人数_4 = 10;				// 미행동 무장으로 변환할 수 있는 최대인원 (거기장군 이상 or 태수의 신분이 [군주, 도독])

	const int KEY = pk::hash("紧急动员");

	// ===========================================

	class Main
	{
		bool 调试模式 = false;
		pk::building@ building_;

		Main()
		{
			pk::bind(107, pk::trigger107_t(onNewDay));

			pk::menu_item item;
			item.menu = 100;
			item.pos = 9;
			item.shortcut = "e";
			item.init = pk::building_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(isVisible);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);

		}

		//新旬恢复，本该按势力分配，不过多占空间，算了
		void onNewDay()
		{
			for (int i = 0; i < 据点_末; ++i)
			{
				base_ex[i].mobilize_done = false;
			}
		}

		void init(pk::building@ building)
		{
			@building_ = @building;
		}

		string getText()
		{
			return pk::encode("紧急动员");
		}

		bool isVisible()
		{
			if (!pk::is_valid_city_id(building_.get_id())) return false;
			return true;
		}

		bool isEnabled()
		{
			auto person_list = getTargetList(building_);
			int result = check_avaliable(building_, person_list);
			if (result != 0) return false;
			else return true;
		}

		string getDesc()
		{
			auto person_list = getTargetList(building_);
			int result = check_avaliable(building_, person_list);
			switch (result)
			{
			case 1: return pk::encode("本回合已执行过紧急动员");
			case 2: return pk::encode("仅限围城状态下使用");
			case 3: return pk::encode("太守不在或正在行动. ");
			case 4: return pk::encode(pk::format("技巧值不足 (必须 {} 技巧值)", TP_COST));
			case 5: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			case 6: return pk::encode("无可执行紧急动员的目标武将");
			case 0: return pk::encode(pk::format("紧急动员.(消耗技巧点{},资金{},使都市已行动武将恢复行动,军团行动力+{})", TP_COST, GOLD_COST, ACTION_GAIN));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		int check_avaliable(pk::building@ building, pk::list<pk::person@> list)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			//pk::trace(pk::format("3.1:{}{}{}{}", pk::enemies_around(building), list.count, pk::get_district(building.get_district_id()).ap < ACTION_GAIN, pk::get_gold(building) < GOLD_COST));
			pk::person@taishu0 = pk::get_person(pk::get_taishu_id(building));
			if (base_ex[building.get_id()].mobilize_done) return 1;
			else if (!pk::enemies_around(building)) return 2;
			else if (pk::is_absent(taishu0) or pk::is_unitize(taishu0) or taishu0.action_done) return 3;
			else if (pk::get_force(building.get_force_id()).tp < TP_COST) return 4;
			else if (pk::get_gold(building) < GOLD_COST) return 5;
			else if (list.count == 0) return 6;
			else return 0;
		}

		bool handler()
		{

			pk::list<pk::person@> person_list = getTargetList(building_);
			pk::list<pk::person@> person_sel;
			pk::person@taishu0 = pk::get_person(pk::get_taishu_id(building_));
			if (pk::choose({ pk::encode(" 是 "), pk::encode(" 否 ") }, pk::encode(pk::format("是否执行紧急动员召回已行动过的武将?\n(消耗技巧点{},资金{},使都市已行动武将恢复行动,军团行动力+{})", TP_COST, GOLD_COST, ACTION_GAIN)), taishu0) == 1)
				return false;
			int max_person = 1;
			if (taishu0.mibun != 身份_君主 and taishu0.mibun != 身份_都督)
			{
				if (taishu0.rank > 官职条件_0) max_person = 最大人数_0;
				else if (taishu0.rank > 官职条件_1) max_person = 最大人数_1;
				else if (taishu0.rank > 官职条件_2) max_person = 最大人数_2;
				else if (taishu0.rank > 官职条件_3) max_person = 最大人数_3;
				else max_person = 最大人数_4;
			}
			else max_person = 最大人数_4;
			person_sel = pk::person_selector(pk::encode("选择目标武将"), pk::encode(pk::format("请选择变更为未行动状态的武将.(最多{}人)", max_person)), person_list, 1, max_person);

			if (person_sel.count == 0) return false; // 미선택 시 취소 종료
			return run_order(person_sel, building_);
		}


		void onAIRunningOrder(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 据点AI_紧急动员)
			{
				pk::list<pk::person@> person_sel;
				if (run_order_before(person_sel, building)) run_order(person_sel, building);
				else if (调试模式) pk::trace("据点AI_紧急动员 不满足");
			}
		}


		bool run_order_before(pk::list<pk::person@>& out person_sel, pk::building@ building0)
		{
			pk::list<pk::person@> person_list = getTargetList(building0);
			if (person_list.count == 0) return false;
			int taishu_id0 = pk::get_taishu_id(building0);
			if (taishu_id0 == -1) return false;
			auto taishu0 = pk::get_person(taishu_id0);
			int max_person = 1;
			if (taishu0.mibun != 身份_君主 and taishu0.mibun != 身份_都督)
			{
				if (taishu0.rank > 官职条件_0) max_person = 最大人数_0;
				else if (taishu0.rank > 官职条件_1) max_person = 最大人数_1;
				else if (taishu0.rank > 官职条件_2) max_person = 最大人数_2;
				else if (taishu0.rank > 官职条件_3) max_person = 最大人数_3;
				else max_person = 最大人数_4;
			}
			else max_person = 最大人数_4;

			if (max_person > person_list.count)
			{
				person_sel = person_list;
			}
			else
			{
				person_list.sort(function(a, b)//武将选择这块还得加强
				{
					return (a.stat[武将能力_智力] > b.stat[武将能力_智力]); // 무장 정렬 (지력순)
				});
				for (int i = 0; i < max_person; i++)
				{
					person_sel.add(person_list[i]);
				}
			}

			if (check_avaliable(building0, person_sel) != 0) return false;

			return true;
		}


		bool run_order(pk::list<pk::person@> person_sel, pk::building@ building0)
		{
			if (building0 is null) return false;
			if (person_sel.count == 0) return false;
			int base_id = building0.get_id();
			if (!pk::is_valid_base_id(base_id)) return false;
			pk::person@ taishu0 = pk::get_person(pk::get_taishu_id(building0));
			pk::force@ force0 = pk::get_force(building0.get_force_id());
			
			base_ex[base_id].mobilize_done = true;
			// 미행동으로 변경.
			for (int i = 0; i < person_sel.count; i++)
			{
				pk::person@ actor = person_sel[i];
				actor.action_done = false;
			}

			// 도시 명령 미완료 상태로 초기화
			
			if (pk::is_valid_city_id(base_id))
			{
				pk::city@ city0 = pk::get_city(base_id);
				city0.drills_ended = false;
				city0.inspections_ended = false;
				city0.merchant_done = false;

				if (has_兵舍(city0)) city0.barracks_counter += 1;

				if (has_厩舍(city0)) city0.blacksmith_counter += 1;

				if (has_锻冶(city0)) city0.stable_counter += 1;
			}
			else//已经判定过了0-86，所以此处是非城市
			{
				base_ex[base_id].recruit_done = false;
				base_ex[base_id].inspections_done = false;
				pk::set_drills_ended(building0,false);
			}


			/*
			if (pk::has_facility(taishu_city, 시설_공방))
			{
				taishu_city.workshop_counter = 1;
			}

			if (pk::has_facility(taishu_city, 시설_조선))
			{
				taishu_city.shipyard_counter = 1;
			}
			*/

			
			if (force0.player != -1)
			{
				pk::message_box(pk::encode("恩!需要紧急动员召集城内的武将,还有些重要任务要托付给他们呢."), taishu0);
			}
			else
			{
				if (pk::is_in_screen(building0.pos))
				{
					pk::say(pk::encode("恩!需要紧急动员召集城内的武将,\n还有些重要任务要托付给他们呢."), taishu0, building0);
				}
				pk::history_log(building0.pos, (pk::get_force(building0.get_force_id())).color, pk::encode(pk::format("\x1b[1x{}\x1b[0x发动了紧急动员.", pk::decode(pk::get_name(taishu0)))));

			}
			// 행동력 회복
			auto district = pk::get_district(building0.get_district_id());
			pk::add_ap(district,ACTION_GAIN);
			//district.ap = pk::min(255, district.ap + ACTION_GAIN);

			// 금 감소 
			pk::add_gold(building0, -GOLD_COST, true);

			// 기교 감소
			ch::add_tp(force0, -TP_COST, building0.get_pos());

			taishu0.action_done = true;

			return true;

		}

		//=========================================支持函数===================================================//
		pk::list<pk::person@> getTargetList(pk::building@ building)
		{
			pk::list<pk::person@> list = pk::get_person_list(building, pk::mibun_flags(身份_一般));//防止动员君主，都督，太守
			pk::list<pk::person@> temp;
			int building_id = building.get_id();
			int force_id = building.get_force_id();
			for (int i = 0; i < list.count; i++)
			{
				pk::person@ person = list[i];
				if (pk::is_alive(person))
				{
					if (person.get_force_id() == force_id)
					{
						if (!pk::is_absent(person) and !pk::is_unitize(person) and (!is_producting(person))
							and (person.action_done == true))
							temp.add(person);
					}
				}
			}
			return temp;
		}

		bool is_producting(pk::person@ person1)
		{
			for (int i = 兵器_冲车; i < 兵器_末; i++)
			{
				if ((person1.order == 2) and (person1.target[1] == i))
					return true;
			}
			return false;
		}

		bool has_兵舍(pk::city@ city)
		{
			if (!pk::is_alive(city)) return false;
			for (int i = 0; i < 30; ++i)
			{
				auto t = city.dev[i].building;
				if (pk::is_alive(t))
					if ((t.facility == 设施_兵舍1级 or t.facility == 设施_兵舍2级 or t.facility == 设施_兵舍3级) && t.completed) return true;
			}
			return false;
		}

		bool has_厩舍(pk::city@ city)
		{
			if (!pk::is_alive(city)) return false;
			for (int i = 0; i < 30; ++i)
			{
				auto t = city.dev[i].building;
				if (pk::is_alive(t))
					if ((t.facility == 设施_厩舍1级 or t.facility == 设施_厩舍2级 or t.facility == 设施_厩舍3级) && t.completed) return true;
			}
			return false;
		}

		bool has_锻冶(pk::city@ city)
		{
			if (!pk::is_alive(city)) return false;
			for (int i = 0; i < 30; ++i)
			{
				auto t = city.dev[i].building;
				if (pk::is_alive(t))
					if ((t.facility == 设施_锻冶1级 or t.facility == 设施_锻冶2级 or t.facility == 设施_锻冶3级) && t.completed) return true;
			}
			return false;
		}
	}

	Main main;
}
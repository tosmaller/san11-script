// ## 2020/08/07 # 氕氘氚 # 规则、代碼优化 ##
/*
@제작자: HoneyBee
@설명: 통솔, 지력이 동시에 높은 무장이 [군주, 도독] 신분이거나 [평북장군] 이상의 관직인 경우에 활용할 수 있는 부대 메뉴


※ COMMANDER TYPE 부대의 효과

(1) 부대 주위 아군 기력 회복
(2) 부대 주위 적군 기력 감소
(3) 부대 주위 아군 병력 회복
(4) 부대 주위 적군 병력 흡수 (심공 효과)


*/

namespace 부대전략메뉴
{

	// ================ CUSTOMIZE ================


	const bool 开启部队战略_玩家 = true; 			// 부대전략메뉴_사용 설정 (true = on / false = off)

	const bool 아군기력회복_사용 = true; 			// 아군기력회복_사용 설정 (true = on / false = off)
	const bool 적군기력감소_사용 = true; 			// 적군기력감소_사용 설정 (true = on / false = off)
	const bool 아군병력회복_사용 = true; 			// 아군병력회복_사용 설정 (true = on / false = off)
	const bool 적군병력흡수_사용 = true; 			// 적군병력흡수_사용 설정 (true = on / false = off)

	const int ENERGY_COST_아군기력회복 = 40;     // 아군기력회복 기력 필요량
	const int ENERGY_COST_적군기력감소 = 40;     // 적군기력감소 기력 필요량
	const int ENERGY_COST_아군병력회복 = 40;     // 아군병력회복 기력 필요량
	const int ENERGY_COST_적군병력흡수 = 40;     // 적군병력흡수 기력 필요량

	const int 兵力条件 = 5000;
	const int 统率条件 = 80;
	const int 智力条件 = 80;


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
		pk::building@ src_building;

		void add_menu_unit_order()
		{
			if (开启部队战略_玩家)
			{
				pk::menu_item 부대;
				int 부대_상위;

				부대.menu = 1;
				부대.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("部队战略"); });
				부대.init = cast<pk::unit_menu_item_init_t@>(function(unit, src_pos) { @main.src_unit = @unit; main.src_pos_ = src_pos; });
				부대.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return (main.src_unit).type == 部队类型_战斗; });
				부대_상위 = pk::add_menu_item(부대);

				// 부대전략메뉴 : 주위 아군 기력 회복
				if (아군기력회복_사용)
				{
					pk::menu_item 아군기력회복;
					아군기력회복.menu = 부대_상위;
					아군기력회복.init = pk::unit_menu_item_init_t(init);
					아군기력회복.get_text = pk::menu_item_get_text_t(getText_아군기력회복);
					아군기력회복.get_desc = pk::menu_item_get_desc_t(getDesc_아군기력회복);
					아군기력회복.is_enabled = pk::menu_item_is_enabled_t(isEnabled_아군기력회복);
					아군기력회복.handler = pk::unit_menu_item_handler_t(handler_아군기력회복);
					pk::add_menu_item(아군기력회복);
				}

				// 부대전략메뉴 : 주위 적군 기력 감소
				if (적군기력감소_사용)
				{
					pk::menu_item 적군기력감소;
					적군기력감소.menu = 부대_상위;
					적군기력감소.init = pk::unit_menu_item_init_t(init);
					적군기력감소.get_text = pk::menu_item_get_text_t(getText_적군기력감소);
					적군기력감소.get_desc = pk::menu_item_get_desc_t(getDesc_적군기력감소);
					적군기력감소.is_enabled = pk::menu_item_is_enabled_t(isEnabled_적군기력감소);
					적군기력감소.handler = pk::unit_menu_item_handler_t(handler_적군기력감소);
					pk::add_menu_item(적군기력감소);
				}

				// 부대전략메뉴 : 주위 아군 병력 회복
				if (아군병력회복_사용)
				{
					pk::menu_item 아군병력회복;
					아군병력회복.menu = 부대_상위;
					아군병력회복.init = pk::unit_menu_item_init_t(init);
					아군병력회복.get_text = pk::menu_item_get_text_t(getText_아군병력회복);
					아군병력회복.get_desc = pk::menu_item_get_desc_t(getDesc_아군병력회복);
					아군병력회복.is_enabled = pk::menu_item_is_enabled_t(isEnabled_아군병력회복);
					아군병력회복.handler = pk::unit_menu_item_handler_t(handler_아군병력회복);
					pk::add_menu_item(아군병력회복);
				}

				// 부대전략메뉴 : 주위 적군 병력 흡수
				if (적군병력흡수_사용)
				{
					pk::menu_item 적군병력흡수;
					적군병력흡수.menu = 부대_상위;
					적군병력흡수.init = pk::unit_menu_item_init_t(init);
					적군병력흡수.get_text = pk::menu_item_get_text_t(getText_적군병력흡수);
					적군병력흡수.get_desc = pk::menu_item_get_desc_t(getDesc_적군병력흡수);
					적군병력흡수.is_enabled = pk::menu_item_is_enabled_t(isEnabled_적군병력흡수);
					적군병력흡수.handler = pk::unit_menu_item_handler_t(handler_적군병력흡수);
					pk::add_menu_item(적군병력흡수);
				}

			}

		} // add_menu_unit_order

		void init(pk::unit@ unit, pk::point src_pos)
		{
			@src_unit = @unit;
			src_pos_ = src_pos;
			@src_leader = pk::get_person(src_unit.leader);
			@src_building = pk::get_building(src_leader.service);
		}



		//---------------------------------------------------------------------------
		// 부대전략메뉴 : 아군기력회복 
		//---------------------------------------------------------------------------

		string getText_아군기력회복()
		{
			return pk::encode(pk::format("士气高涨 ({})", global_func_最终气力消耗(@src_unit, ENERGY_COST_아군기력회복)));
		}

		string getDesc_아군기력회복()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_아군기력회복))
				return pk::encode("气力不足.");
			else if (src_unit.troops < 兵力条件)
				return pk::encode("兵力不足.");
			else if ((src_unit.leader != pk::get_kunshu_id(src_unit)) and (src_unit.leader != pk::get_totoku_id(src_unit)) and (src_leader.rank > 관직_평북장군))
				return pk::encode("身分/官职不符合条件.");
			else if ((src_leader.stat[武将能力_统率] < 统率条件) or (src_leader.stat[武将能力_智力] < 智力条件))
				return pk::encode("统率/智力不符合条件.");
			else
				return pk::encode(pk::format("使用士气高涨.(气力至少 {} 以上)", global_func_最终气力消耗(@src_unit, ENERGY_COST_아군기력회복)));
		}

		bool isVisible_아군기력회복()
		{
			return true;
		}

		bool isEnabled_아군기력회복()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_아군기력회복)) return false;
			else if (src_unit.troops < 兵力条件) return false;
			else if ((src_unit.leader != pk::get_kunshu_id(src_unit)) and (src_unit.leader != pk::get_totoku_id(src_unit)) and (src_leader.rank > 관직_평북장군)) return false;
			else if ((src_leader.stat[武将能力_统率] < 统率条件) or (src_leader.stat[武将能力_智力] < 智力条件)) return false;
			return true;
		}

		bool handler_아군기력회복(pk::point dst_pos)
		{

			global_func_士气高漲(src_pos_, @src_unit, @src_leader);

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -global_func_最终气力消耗(@src_unit, ENERGY_COST_아군기력회복), true);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
				src_unit.action_done = false;

			return true;
		}


		//---------------------------------------------------------------------------
		// 부대전략메뉴 : 적군기력감소
		//---------------------------------------------------------------------------

		string getText_적군기력감소()
		{
			return pk::encode(pk::format("动摇敌军 ({})", global_func_最终气力消耗(@src_unit, ENERGY_COST_적군기력감소)));
		}

		string getDesc_적군기력감소()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_적군기력감소))
				return pk::encode("气力不足.");
			else if (src_unit.troops < 兵力条件)
				return pk::encode("兵力不足.");
			else if ((src_unit.leader != pk::get_kunshu_id(src_unit)) and (src_unit.leader != pk::get_totoku_id(src_unit)) and (src_leader.rank > 관직_평북장군))
				return pk::encode("身分/官职不符合条件.");
			else if ((src_leader.stat[武将能力_统率] < 统率条件) or (src_leader.stat[武将能力_智力] < 智力条件))
				return pk::encode("统率/智力不符合条件");
			else
				return pk::encode(pk::format("使用动摇敌军.(气力至少 {} 以上)", global_func_最终气力消耗(@src_unit, ENERGY_COST_적군기력감소)));
		}

		bool isVisible_적군기력감소()
		{
			return true;
		}

		bool isEnabled_적군기력감소()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_적군기력감소)) return false;
			else if (src_unit.troops < 兵力条件) return false;
			else if ((src_unit.leader != pk::get_kunshu_id(src_unit)) and (src_unit.leader != pk::get_totoku_id(src_unit)) and (src_leader.rank > 관직_평북장군)) return false;
			else if ((src_leader.stat[武将能力_统率] < 统率条件) or (src_leader.stat[武将能力_智力] < 智力条件)) return false;
			return true;
		}

		bool handler_적군기력감소(pk::point dst_pos)
		{

			global_func_敌军动摇(src_pos_, @src_unit, @src_leader);

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -global_func_最终气力消耗(@src_unit, ENERGY_COST_적군기력감소), true);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
				src_unit.action_done = false;

			return true;
		}


		//---------------------------------------------------------------------------
		// 부대전략메뉴 : 아군병력회복 
		//---------------------------------------------------------------------------

		string getText_아군병력회복()
		{
			return pk::encode(pk::format("援军支援 ({})", global_func_最终气力消耗(@src_unit, ENERGY_COST_아군병력회복)));
		}

		string getDesc_아군병력회복()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_아군병력회복))
				return pk::encode("气力不足.");
			else if (src_unit.troops < 兵力条件)
				return pk::encode("兵力不足.");
			else if ((src_unit.leader != pk::get_kunshu_id(src_unit)) and (src_unit.leader != pk::get_totoku_id(src_unit)) and (src_leader.rank > 관직_평북장군))
				return pk::encode("身分/官职不符合条件.");
			else if ((src_leader.stat[武将能力_统率] < 统率条件) or (src_leader.stat[武将能力_智力] < 智力条件))
				return pk::encode("统率/智力不符合条件.");
			else if ((pk::get_troops(src_building) < 援军支援加兵 * 10 or pk::get_food(src_building) < 援军支援加粮 * 20))
				return pk::encode("据点兵力/军粮不足");
			else
				return pk::encode(pk::format("使用援军支援.(气力至少 {} 以上)", global_func_最终气力消耗(@src_unit, ENERGY_COST_아군병력회복)));
		}

		bool isVisible_아군병력회복()
		{
			return true;
		}

		bool isEnabled_아군병력회복()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_아군병력회복)) return false;
			else if (src_unit.troops < 兵力条件) return false;
			else if ((src_unit.leader != pk::get_kunshu_id(src_unit)) and (src_unit.leader != pk::get_totoku_id(src_unit)) and (src_leader.rank > 관직_평북장군)) return false;
			else if ((src_leader.stat[武将能力_统率] < 统率条件) or (src_leader.stat[武将能力_智力] < 智力条件)) return false;
			else if ((pk::get_troops(src_building) < 援军支援加兵 * 10 or pk::get_food(src_building) < 援军支援加粮 * 20)) return false;
			return true;
		}

		bool handler_아군병력회복(pk::point dst_pos)
		{
			global_func_援军支援(src_pos_, @src_unit, @src_leader);

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -global_func_最终气力消耗(@src_unit, ENERGY_COST_아군병력회복), true);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
				src_unit.action_done = false;

			return true;
		}


		//---------------------------------------------------------------------------
		// 부대전략메뉴 : 적군병력흡수 
		//---------------------------------------------------------------------------

		string getText_적군병력흡수()
		{
			return pk::encode(pk::format("煽动敌军 ({})", global_func_最终气力消耗(@src_unit, ENERGY_COST_적군병력흡수)));
		}

		string getDesc_적군병력흡수()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_적군병력흡수))
				return pk::encode("气力不足.");
			else if (src_unit.troops < 兵力条件)
				return pk::encode("兵力不足.");
			else if ((src_unit.leader != pk::get_kunshu_id(src_unit)) and (src_unit.leader != pk::get_totoku_id(src_unit)) and (src_leader.rank > 관직_평북장군))
				return pk::encode("身分/官职不符合条件.");
			else if ((src_leader.stat[武将能力_统率] < 统率条件) or (src_leader.stat[武将能力_智力] < 智力条件))
				return pk::encode("统率/智力不符合条件.");
			else
				return pk::encode(pk::format("使用煽动敌军.(气力至少 {} 以上)", global_func_最终气力消耗(@src_unit, ENERGY_COST_적군병력흡수)));
		}

		bool isVisible_적군병력흡수()
		{
			return true;
		}

		bool isEnabled_적군병력흡수()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_적군병력흡수)) return false;
			else if (src_unit.troops < 兵力条件) return false;
			else if ((src_unit.leader != pk::get_kunshu_id(src_unit)) and (src_unit.leader != pk::get_totoku_id(src_unit)) and (src_leader.rank > 관직_평북장군)) return false;
			else if ((src_leader.stat[武将能力_统率] < 统率条件) or (src_leader.stat[武将能力_智力] < 智力条件)) return false;
			return true;
		}

		bool handler_적군병력흡수(pk::point dst_pos)
		{
			global_func_敌军煽动(src_pos_, @src_unit, @src_leader);

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -global_func_最终气力消耗(@src_unit, ENERGY_COST_적군병력흡수), true);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
				src_unit.action_done = false;

			return true;
		}


	} // class Main

	Main main;

} // namespace
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/08/07 # 氕氘氚 # 规则、代碼优化 ##
// ## 2020/08/05 # messi # 重新设计台詞 ##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: 부대 지력이 적부대의 지력보다 높은 경우와 낮은 경우에 각기 정해진 성공률이 반영되는 SCRIPT


※ 효과 참고
- 적부대의 지력보다 높은 경우 별도 성공률 적용
- 적부대의 지력보다 낮은 경우 별도 성공률 적용
- 범위에 있는 모든 적군 부대에게 계략으로 피해를 줍니다. (단, 특정 특기를 소유한 적부대는 면역)
- 지력 경험치 상승 (묘책을 실행하는 부대에 속한 모든 무장들)
- 귀모계 효과: 범위 내 적군부대 혼란
- 허보계 효과: 범위 내 적군부대 위보
- 신화계 효과: 범위 내 적군부대 화계 + 기력 감소 + 혼란
- 팔진도 효과: 범위 내 [아군부대 기력/상태 회복/미행동 전환] and [적군부대 기력 감소 + 혼란]
- 허허실실 효과: 범위 내 [적군부대 병력 감소 + 혼란] or [적군부대 병력 감소 + 위보]
- 속전고수 효과: 범위 내 [아군부대 병력/기력 회복/미행동 전환] and [적군부대 병량/기력 감소]

*/

namespace 部队指令_部队妙计
{

	// ================ CUSTOMIZE ================

		//	全局设置-部队作战技能

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


		void add_menu_unit_order()
		{
			if (开启部队妙计_玩家)
			{
				pk::menu_item 부대;
				int 부대_상위;

				부대.menu = 1;
				부대.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("妙计"); });  //miaoji
				부대.init = cast<pk::unit_menu_item_init_t@>(function(unit, src_pos) { @main.src_unit = @unit; main.src_pos_ = src_pos; });
				부대.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return (main.src_unit).type == 部队类型_战斗; });
				부대_상위 = pk::add_menu_item(부대);

				// 묘책메뉴 : 귀모계(혼란)
				if (开启_范围混乱)
				{
					pk::menu_item 部队_混乱计;
					部队_混乱计.menu = 부대_상위;
					部队_混乱计.init = pk::unit_menu_item_init_t(init);
					部队_混乱计.get_text = pk::menu_item_get_text_t(getText_部队_混乱计);
					部队_混乱计.get_desc = pk::menu_item_get_desc_t(getDesc_部队_混乱计);
					部队_混乱计.is_enabled = pk::menu_item_is_enabled_t(isEnabled_部队_混乱计);
					部队_混乱计.handler = pk::unit_menu_item_handler_t(handler_部队_混乱计);
					pk::add_menu_item(部队_混乱计);
				}

				// 묘책메뉴 : 허보계(위보)
				if (开启_范围伪报)
				{
					pk::menu_item 部队_伪报计;
					部队_伪报计.menu = 부대_상위;
					部队_伪报计.init = pk::unit_menu_item_init_t(init);
					部队_伪报计.get_text = pk::menu_item_get_text_t(getText_部队_伪报计);
					部队_伪报计.get_desc = pk::menu_item_get_desc_t(getDesc_部队_伪报计);
					部队_伪报计.is_enabled = pk::menu_item_is_enabled_t(isEnabled_部队_伪报计);
					部队_伪报计.handler = pk::unit_menu_item_handler_t(handler_部队_伪报计);
					pk::add_menu_item(部队_伪报计);
				}

				// 묘책메뉴 : 신화계(화계 + 혼란)
				if (开启_神火计)
				{
					pk::menu_item 部队_神火计;
					部队_神火计.menu = 부대_상위;
					部队_神火计.init = pk::unit_menu_item_init_t(init);
					部队_神火计.get_text = pk::menu_item_get_text_t(getText_部队_神火计);
					部队_神火计.get_desc = pk::menu_item_get_desc_t(getDesc_部队_神火计);
					部队_神火计.is_enabled = pk::menu_item_is_enabled_t(isEnabled_部队_神火计);
					部队_神火计.handler = pk::unit_menu_item_handler_t(handler_部队_神火计);
					pk::add_menu_item(部队_神火计);
				}

				// 묘책메뉴 : 팔진도 [아군부대 기력/상태 회복] and [적군부대 기력 감소 + 혼란]
				if (开启_八阵图)
				{
					pk::menu_item 部队_八阵图;
					部队_八阵图.menu = 부대_상위;
					部队_八阵图.init = pk::unit_menu_item_init_t(init);
					部队_八阵图.get_text = pk::menu_item_get_text_t(getText_部队_八阵图);
					部队_八阵图.get_desc = pk::menu_item_get_desc_t(getDesc_部队_八阵图);
					部队_八阵图.is_visible = pk::menu_item_is_visible_t(isVisible_部队_八阵图);
					部队_八阵图.is_enabled = pk::menu_item_is_enabled_t(isEnabled_部队_八阵图);
					部队_八阵图.handler = pk::unit_menu_item_handler_t(handler_部队_八阵图);
					pk::add_menu_item(部队_八阵图);
				}

				// 묘책메뉴 : 허허실실 [적군부대 병력 감소 + 혼란] or [적군부대 병력 감소 + 위보]
				if (开启_虚虚实实)
				{
					pk::menu_item 部队_虚虚实实;
					部队_虚虚实实.menu = 부대_상위;
					部队_虚虚实实.init = pk::unit_menu_item_init_t(init);
					部队_虚虚实实.get_text = pk::menu_item_get_text_t(getText_部队_虚虚实实);
					部队_虚虚实实.get_desc = pk::menu_item_get_desc_t(getDesc_部队_虚虚实实);
					部队_虚虚实实.is_visible = pk::menu_item_is_visible_t(isVisible_部队_虚虚实实);
					部队_虚虚实实.is_enabled = pk::menu_item_is_enabled_t(isEnabled_部队_虚虚实实);
					部队_虚虚实实.handler = pk::unit_menu_item_handler_t(handler_部队_虚虚实实);
					pk::add_menu_item(部队_虚虚实实);
				}

				// 묘책메뉴 : 속전고수 [아군부대 병력/기력 회복] and [적군부대 병량/기력 감소]
				if (开启_速战固守)
				{
					pk::menu_item 部队_速战固守;
					部队_速战固守.menu = 부대_상위;
					部队_速战固守.init = pk::unit_menu_item_init_t(init);
					部队_速战固守.get_text = pk::menu_item_get_text_t(getText_部队_速战固守);
					部队_速战固守.get_desc = pk::menu_item_get_desc_t(getDesc_部队_速战固守);
					部队_速战固守.is_visible = pk::menu_item_is_visible_t(isVisible_部队_速战固守);
					部队_速战固守.is_enabled = pk::menu_item_is_enabled_t(isEnabled_部队_速战固守);
					部队_速战固守.handler = pk::unit_menu_item_handler_t(handler_部队_速战固守);
					pk::add_menu_item(部队_速战固守);
				}

			}

		} // add_menu_unit_order


		void init(pk::unit@ unit, pk::point src_pos)
		{
			@src_unit = @unit;
			src_pos_ = src_pos;
			@src_leader = pk::get_person(src_unit.leader);
		}


		//---------------------------------------------------------------------------
		// 묘책메뉴 : 귀모계(혼란) [대규모 혼란] 
		//---------------------------------------------------------------------------

		string getText_部队_混乱计()
		{
			return pk::encode(pk::format("范围混乱 ({})", global_func_最终气力消耗(@src_unit, ENERGY_COST_范围混乱)));
		}

		string getDesc_部队_混乱计()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_范围混乱))
				return pk::encode("气力不足.");
			else if (src_unit.troops < 部队妙计_兵力条件)
				return pk::encode("兵力不足.");
			else
				return pk::encode(pk::format("使用范围混乱.(气力至少 {} 以上)", global_func_最终气力消耗(@src_unit, ENERGY_COST_范围混乱)));
		}

		bool isVisible_部队_混乱计()
		{
			return true;
		}

		bool isEnabled_部队_混乱计()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_范围混乱)) return false;
			else if (src_unit.troops < 部队妙计_兵力条件) return false;
			return true;
		}

		bool handler_部队_混乱计(pk::point dst_pos)
		{
			global_func_范围混乱(src_pos_, @src_unit, @src_leader);

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -global_func_最终气力消耗(@src_unit, ENERGY_COST_范围混乱), true);

			// 부대 무장들의 지력 경험치 상승
			pk::add_stat_exp(src_unit, 武将能力_智力, 部队妙计_增加经验);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
				src_unit.action_done = false;

			return true;
		}


		//---------------------------------------------------------------------------
		// 묘책메뉴 : 허보계(위보) [대규모 위보] 
		//---------------------------------------------------------------------------

		string getText_部队_伪报计()
		{
			return pk::encode(pk::format("范围伪报 ({})", global_func_最终气力消耗(@src_unit, ENERGY_COST_范围伪报)));
		}

		string getDesc_部队_伪报计()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_范围伪报))
				return pk::encode("气力不足.");
			else if (src_unit.troops < 部队妙计_兵力条件)
				return pk::encode("兵力不足.");
			else
				return pk::encode(pk::format("使用范围伪报.(气力至少 {} 以上)", global_func_最终气力消耗(@src_unit, ENERGY_COST_范围伪报)));
		}

		bool isVisible_部队_伪报计()
		{
			return true;
		}

		bool isEnabled_部队_伪报计()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_范围伪报)) return false;
			else if (src_unit.troops < 部队妙计_兵力条件) return false;
			return true;
		}

		bool handler_部队_伪报计(pk::point dst_pos)
		{
			global_func_范围伪报(src_pos_, @src_unit, @src_leader);

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -global_func_最终气力消耗(@src_unit, ENERGY_COST_范围伪报), true);

			// 부대 무장들의 지력 경험치 상승
			pk::add_stat_exp(src_unit, 武将能力_智力, 部队妙计_增加经验);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
				src_unit.action_done = false;

			return true;
		}


		//---------------------------------------------------------------------------
		// 묘책메뉴 : 신화계(화계 + 혼란) [대규모 화계 + 혼란] 
		//---------------------------------------------------------------------------

		string getText_部队_神火计()
		{
			return pk::encode(pk::format("神火计 ({})", global_func_最终气力消耗(@src_unit, ENERGY_COST_神火计)));
		}

		string getDesc_部队_神火计()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_神火计))
				return pk::encode("气力不足.");
			else if (!ch::has_skill(src_unit, 특기_화신))
				return pk::encode("没有特技火神");
			else if (src_unit.troops < 部队妙计_兵力条件)
				return pk::encode("兵力不足.");
			else
				return pk::encode(pk::format("使用神火计.(气力至少 {} 以上)", global_func_最终气力消耗(@src_unit, ENERGY_COST_神火计)));
		}

		bool isVisible_部队_神火计()
		{
			return true;
		}

		bool isEnabled_部队_神火计()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_神火计)) return false;
			else if (!ch::has_skill(src_unit, 특기_화신)) return false;
			else if (src_unit.troops < 部队妙计_兵力条件) return false;
			return true;
		}

		bool handler_部队_神火计(pk::point dst_pos)
		{
			global_func_神火计(src_pos_, @src_unit, @src_leader);

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -global_func_最终气力消耗(@src_unit, ENERGY_COST_神火计), true);

			// 부대 무장들의 지력 경험치 상승
			pk::add_stat_exp(src_unit, 武将能力_智力, 部队妙计_增加经验);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
				src_unit.action_done = false;

			return true;
		}


		//---------------------------------------------------------------------------
		// 묘책메뉴 : 팔진도 [아군부대 기력/상태 회복] and [적군부대 기력 감소 + 혼란]
		//---------------------------------------------------------------------------

		string getText_部队_八阵图()
		{
			return pk::encode(pk::format("八阵图 ({})", global_func_最终气力消耗(@src_unit, ENERGY_COST_八阵图)));
		}

		string getDesc_部队_八阵图()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_八阵图))
				return pk::encode("气力不足.");
			else if (!ch::has_skill(src_unit, 특기_신산))
				return pk::encode("没有特技神算.");
			else if (src_unit.troops < 部队妙计_兵力条件)
				return pk::encode("兵力不足.");
			else
				return pk::encode(pk::format("使用八阵图.(气力至少 {} 以上)", global_func_最终气力消耗(@src_unit, ENERGY_COST_八阵图)));
		}

		bool isVisible_部队_八阵图()
		{
			return true;
		}

		bool isEnabled_部队_八阵图()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_八阵图)) return false;
			else if (!ch::has_skill(src_unit, 특기_신산)) return false;
			else if (src_unit.troops < 部队妙计_兵力条件) return false;
			return true;
		}

		bool handler_部队_八阵图(pk::point dst_pos)
		{
			global_func_八阵图(src_pos_, @src_unit, @src_leader);

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -global_func_最终气力消耗(@src_unit, ENERGY_COST_八阵图), true);

			// 부대 무장들의 지력 경험치 상승
			pk::add_stat_exp(src_unit, 武将能力_智力, 部队妙计_增加经验);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
				src_unit.action_done = false;

			return true;
		}


		//---------------------------------------------------------------------------
		// 묘책메뉴 : 허허실실 [적군부대 병력 감소 + 혼란] or [적군부대 병력 감소 + 위보]
		//---------------------------------------------------------------------------

		string getText_部队_虚虚实实()
		{
			return pk::encode(pk::format("虚虚实实 ({})", global_func_最终气力消耗(@src_unit, ENERGY_COST_虚虚实实)));
		}

		string getDesc_部队_虚虚实实()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_虚虚实实))
				return pk::encode("气力不足.");
			else if (!ch::has_skill(src_unit, 특기_허실))
				return pk::encode("没有特技虚实.");
			else if (src_unit.troops < 部队妙计_兵力条件)
				return pk::encode("兵力不足.");
			else
				return pk::encode(pk::format("使用虚虚实实.(气力至少 {} 以上)", global_func_最终气力消耗(@src_unit, ENERGY_COST_虚虚实实)));
		}

		bool isVisible_部队_虚虚实实()
		{
			return true;
		}

		bool isEnabled_部队_虚虚实实()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_虚虚实实)) return false;
			else if (!ch::has_skill(src_unit, 특기_허실)) return false;
			else if (src_unit.troops < 部队妙计_兵力条件) return false;
			return true;
		}

		bool handler_部队_虚虚实实(pk::point dst_pos)
		{
			global_func_虚虚实实(src_pos_, @src_unit, @src_leader);

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -global_func_最终气力消耗(@src_unit, ENERGY_COST_虚虚实实), true);

			// 부대 무장들의 지력 경험치 상승
			pk::add_stat_exp(src_unit, 武将能力_智力, 部队妙计_增加经验);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
				src_unit.action_done = false;

			return true;
		}


		//---------------------------------------------------------------------------
		// 묘책메뉴 : 속전고수 [아군부대 병력/기력 회복] and [적군부대 병량/기력 감소]
		//---------------------------------------------------------------------------

		string getText_部队_速战固守()
		{
			return pk::encode(pk::format("速战固守 ({})", global_func_最终气力消耗(@src_unit, ENERGY_COST_速战固守)));
		}

		string getDesc_部队_速战固守()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_速战固守))
				return pk::encode("气力不足.");
			else if (!ch::has_skill(src_unit, 특기_심모))
				return pk::encode("没有特技深谋.");
			else if (src_unit.troops < 部队妙计_兵力条件)
				return pk::encode("兵力不足.");
			else
				return pk::encode(pk::format("使用速战固守.(气力至少 {} 以上)", global_func_最终气力消耗(@src_unit, ENERGY_COST_速战固守)));
		}

		bool isVisible_部队_速战固守()
		{
			return true;
		}

		bool isEnabled_部队_速战固守()
		{
			if (src_unit.energy < global_func_最终气力消耗(@src_unit, ENERGY_COST_速战固守)) return false;
			else if (!ch::has_skill(src_unit, 특기_심모)) return false;
			else if (src_unit.troops < 部队妙计_兵力条件) return false;
			return true;
		}

		bool handler_部队_速战固守(pk::point dst_pos)
		{
			global_func_速战固守(src_pos_, @src_unit, @src_leader);

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -global_func_最终气力消耗(@src_unit, ENERGY_COST_速战固守), true);

			// 부대 무장들의 지력 경험치 상승
			pk::add_stat_exp(src_unit, 武将能力_智力, 部队妙计_增加经验);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
				src_unit.action_done = false;

			return true;
		}


	} // class Main

	Main main;
}
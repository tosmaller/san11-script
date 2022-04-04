// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/08/08 # 氕氘氚 # 提高部队指揮使用条件 ##
/*
@제작자: HoneyBee
@설명: 부대의 주장이 <평북장군> 관직 이상이거나 신분이 [군주 or 도독]인 경우 다른 부대를 지휘할 수 있는 SCRIPT


※ 부대 주장의 신분이나 관직보다 높은 주장의 부대는 지휘할 수 없습니다.
(1) 평북장군이 후장군의 부대를 지휘하지 못함
(2) 진북장군이 거기장군의 부대를 지휘하지 못함
(3) 도독이 다른 군단 도독의 부대를 지휘하지 못함 (EX: 2군단 도독이 3군단 도독의 부대를 지휘하지 못함)
(4) 도독이 군주의 부대를 지휘하지 못함


*/

namespace 부대지휘메뉴
{

	// ================ CUSTOMIZE ================

	const bool 메뉴_부대지휘_사용 = true; 		// 부대지휘 사용 설정 (true = on / false = off)
	const int ENERGY_MIN_부대지휘 = 50;		// 기력 필요량

	const int 관직조건_0 = 관직_평북장군;	    	// 부대지휘를 실행할 수 있는 관직 조건
	const int 관직조건_1 = 관직_후장군;
	const int 관직조건_2 = 관직_진북장군;
	const int 관직조건_3 = 관직_거기장군;

	const int 병력조건 = 10000;					// 병력 8000 이상으로 출진해야 지휘 가능

	const int exp_0 = 10;	    			// 통솔 경험치 획득량
	const int 范围_min = 1;      				// 효과를 받는 최소 범위 
	const int 范围_君主 = 4;
	const int 范围_大都督 = 3;

	/*
		max_범위는 부대 주장의 신분이나 관직 상태에 따라서 달라짐

		max_범위 = 2 (평북장군 이상 ~ 후장군 미만)
		max_범위 = 3 (후장군 이상 ~ 진북장군 미만)
		max_범위 = 4 (진북장군 이상 ~ 거기장군 미만)
		max_범위 = 5 (거기장군 이상 or 신분이 [군주, 도독])
	*/

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
			if (메뉴_부대지휘_사용)
			{
				pk::menu_item 메뉴_부대지휘;
				메뉴_부대지휘.menu = global_menu::菜单_部队;
				메뉴_부대지휘.init = pk::unit_menu_item_init_t(init);
				메뉴_부대지휘.get_text = pk::menu_item_get_text_t(getText_메뉴_부대지휘);
				메뉴_부대지휘.get_desc = pk::menu_item_get_desc_t(getDesc_메뉴_부대지휘);
				메뉴_부대지휘.is_visible = pk::menu_item_is_visible_t(isVisible_메뉴_부대지휘);
				메뉴_부대지휘.is_enabled = pk::menu_item_is_enabled_t(isEnabled_메뉴_부대지휘);
				메뉴_부대지휘.handler = pk::unit_menu_item_handler_t(handler_메뉴_부대지휘);
				pk::add_menu_item(메뉴_부대지휘);
			}

		} // add_menu_unit_order

		void init(pk::unit@ unit, pk::point src_pos)
		{
			@src_unit = @unit;
			src_pos_ = src_pos;
			@src_leader = pk::get_person(src_unit.leader);
		}

		//---------------------------------------------------------------------------
		// 메뉴_부대지휘 (삼국지10 지휘 시스템 참고)
		//---------------------------------------------------------------------------

		string getText_메뉴_부대지휘()
		{
			return pk::encode("部队指挥");  //부대지휘 buduizhihui
		}

		string getDesc_메뉴_부대지휘()
		{
			if (src_leader.mibun != 身份_君主 and src_leader.rank != 관직_대도독)
				return pk::encode("只有君主和大都督可以执行.");
			else if (src_unit.troops < 병력조건)
				return pk::encode("兵力不足.");
			else if (src_unit.energy < ENERGY_MIN_부대지휘)
				return pk::encode("气力不足.");
			else
				return pk::encode(pk::format("使用部队指挥.(气力至少 {} 以上)", ENERGY_MIN_부대지휘));
		}

		bool isVisible_메뉴_부대지휘()
		{
			if (src_leader.mibun != 身份_君主 and src_leader.rank != 관직_대도독) return false;
			return true;
		}

		bool isEnabled_메뉴_부대지휘()
		{
			if (src_leader.mibun != 身份_君主 and src_leader.rank != 관직_대도독) return false;
			else if (src_unit.troops < 병력조건) return false;
			else if (src_unit.energy < ENERGY_MIN_부대지휘) return false;
			return true;
		}

		bool handler_메뉴_부대지휘(pk::point dst_pos)
		{

			pk::point pos = src_unit.get_pos();
			string src_leader_name = pk::decode(pk::get_name(src_leader));

			int 范围_max = 1;

			if (src_leader.mibun == 身份_君主)
			{
				范围_max = 范围_君主;
			}
			else if (src_leader.rank == 관직_대도독)
			{
				范围_max = 范围_大都督;
			}

			array<pk::point> arr = pk::range(pos, 范围_min, 范围_max);

			bool flag_지휘 = false;

			for (int j = 0; j < int(arr.length); j++)
			{
				flag_지휘 = true;

				pk::unit@ dst_unit = pk::get_unit(arr[j]);

				if (dst_unit is null or pk::is_enemy(src_unit, dst_unit)) continue;

				if (dst_unit.action_done == false) continue;

				pk::person@ dst_leader = pk::get_person(dst_unit.leader);

				if (dst_leader.mibun == 身份_君主 || dst_leader.rank == 관직_대도독) continue;

				if (dst_unit !is null)
				{
					dst_unit.action_done = false;
					dst_unit.update();

					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode(pk::format("听从\x1b[1x{}\x1b[0x大人的号令!", src_leader_name)), dst_leader); break;
					case 1: pk::say(pk::encode(pk::format("有\x1b[1x{}\x1b[0x大人指挥就放心了!", src_leader_name)), dst_leader); break;
					}
				}

			}

			if (flag_지휘 and pk::is_in_screen(pos))
			{
				pk::play_voice(0x37, src_leader);  // 음성 : 전투_진정
				pk::play_se(0x39, pos);
				pk::create_effect(0x3a, pos);

				switch (pk::rand(2))
				{
				case 0: pk::say(pk::encode("我来亲自指挥!"), src_leader); break;  //내가 직접 지휘하겠다
				case 1: pk::say(pk::encode("接下来由我指挥!"), src_leader); break;  //지금부터는 내가 지휘하겠다
				}

				flag_지휘 = false;
			}

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -ENERGY_MIN_부대지휘, true);

			// 부대 주장의 통솔 경험치 상승
			//src_leader.stat_exp[0] = src_leader.stat_exp[0] + exp_0; // 통솔
			pk::add_stat_exp(src_leader, 武将能力_统率, exp_0);
			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
				src_unit.action_done = false;

			return true;
		}


	} // class Main

	Main main;
}
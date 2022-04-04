// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2020/11/27 # messi # 搬运NAVER论坛masterpiecek的对盟军使用镇静功能##
/*
	@제작 : masterpiecek
	@요청 : 엑시온
	@내용 : 동맹세력의 부대에게 진정 계략을 사용
*/

namespace 계략_동맹진정
{

	/// ============================================ < 유저 설정 > ============================================

		/// 계략_동맹진정 기본설정
	const string 동맹진정_메뉴_이름 = "镇静盟军";
	const string 동맹진정_메뉴_단축키 = "";

	/// 계략 동맹진정의 실행가능 최대거리 설정
	const int 동맹진정_최대거리 = 2;	// 기존 '진정' 계략의 최대거리 2
	const bool 동맹진정_귀모특기_적용 = true;	// 'true' 설정 시 기존의 계략들과 마찬가지로 귀모 특기 보유 시 거리 +1

	/// 계략 동맹진정 기력 소모값 설정
	// 'true' : 설정한 기력을 고정값으로 소모
	// 'false' : 기존 '진정' 계략의 기력 소모값을 동일하게 적용 ('205 계략 소비 기력.cpp' 파일 참고)
	const bool 동맹진정_기력소모_고정값_적용 = false;
	const int 동맹진정_기력소모_설정값 = 10;	// '0'으로 설정 시 기력소모 없이 실행가능

	/// 계략 동맹진정 성공확률 설정
	// 'true' : 설정한 확률을 고정값으로 적용
	// 'false' : 기존 '진정' 계략의 성공확률을 동일하게 적용 ('206 계략 성공 확률.cpp' 파일 참고)
	const bool 동맹진정_성공확률_고정값_적용 = false;
	const int 동맹진정_성공확률_설정값 = 10;

	/// 계략 동맹진정 행동완료 설정
	// 'true' : 계략 실행 후 부대가 행동완료 상태가 됨
	// 'false' : 계략 실행 후에도 부대가 행동가능 상태로 남음
	const bool 동맹진정_행동완료_적용 = true;

	/// =======================================================================================================

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		pk::unit@ src_unit;
		pk::point src_pos_;
		pk::force@ src_force;
		string menu_cost_info;
		int strategy_range = 동맹진정_최대거리;
		int energy_cost = 동맹진정_기력소모_설정값;
		int success_chance = 동맹진정_성공확률_설정값;

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			pk::menu_item item;
			item.menu = 115;
			item.shortcut = 동맹진정_메뉴_단축키;
			item.pos = 5;
			item.init = pk::unit_menu_item_init_t(init);
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode(동맹진정_메뉴_이름 + "(" + main.energy_cost + ")"); });
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.get_targets = pk::unit_menu_item_get_targets_t(getTargets);
			item.handler = pk::unit_menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void init(pk::unit@ unit, pk::point src_pos)
		{
			@src_unit = @unit;
			src_pos_ = src_pos;
			@src_force = pk::get_force(unit.get_force_id());

			// 최대거리 귀모특기 적용여부 확인
			if (동맹진정_귀모특기_적용 and unit.has_skill(특기_귀모)) strategy_range += 1;

			// 기력소모 고정값 적용여부 확인
			if (!동맹진정_기력소모_고정값_적용) energy_cost = get_energy_cost(unit, 계략_진정);

			menu_cost_info = (energy_cost > 0) ? "士气值\x1b[1x" + energy_cost + "\x1b[0x" : "士气值\x1b[1x无\x1b[0x";
		}

		pk::array<pk::point_int> getTargets()
		{
			pk::array<pk::point_int> targets;
			array<pk::point> ranges = pk::range(src_pos_, 1, strategy_range);
			for (int pos_index = 0; pos_index < int(ranges.length); pos_index++)
			{
				pk::point dst_pos = ranges[pos_index];
				if (!pk::is_valid_pos(dst_pos)) continue;

				pk::unit@ dst_unit = pk::get_unit(dst_pos);
				if (!pk::is_alive(dst_unit)) continue;
				if (pk::is_enemy(src_unit, dst_unit)) continue;	// 적세력 부대 제외
				if (src_unit.get_force_id() == dst_unit.get_force_id()) continue;	// 자세력 부대 제외
				if (dst_unit.status != 部队状态_混乱 and dst_unit.status != 部队状态_伪报) continue;	// 상태이상 부대가 아닌 경우 제외

				targets.insertLast(pk::point_int(dst_pos, /*계략*/2));
			}

			return targets;
		}

		bool isEnabled()
		{
			if (int(src_unit.energy) < energy_cost) return false;
			if (int(getTargets().length) == 0) return false;

			return true;
		}

		string getDesc()
		{
			if (int(src_unit.energy) < energy_cost) return pk::encode("士气不足.(" + menu_cost_info + ")");
			if (int(getTargets().length) == 0) return pk::encode("范围内无目标部队.(" + menu_cost_info + ")");

			return pk::encode("解除同盟势力部队的异常状态.(" + menu_cost_info + ")");
		}

		bool handler(pk::point dst_pos)
		{
			pk::unit@ dst_unit = pk::get_unit(dst_pos);
			if (!pk::is_alive(dst_unit)) return false;

			int strategy_id = 계략_진정;

			// 실해부대 계략 소비기력 차감
			pk::add_energy(src_unit, -energy_cost, false);

			// 계략 실행 (일반시전)
			bool is_success = func_계략효과(dst_unit, strategy_id, /*is_chain_reaction*/false);

			// 일반시전 계략이 성공했고 실행부대가 연환특기를 보유한 경우 추가 계략 실행 (연환발동)
			if (is_success and src_unit.has_skill(특기_연환))
			{
				// 대상부대의 인접부대 검색 (계략실행 가능조건을 충족하는 부대를 검색)
				/// 현재 조건 : 실행부대, 대상부대, 대상부대의 인접부대 각각이 서로 적이 아닐 것(따라서 인접부대는 실행부대와 세력이 동일할 수 있음)
				pk::unit@ dst_neighbor_unit = get_dst_neighbor_unit(src_unit, dst_unit);
				if (pk::is_alive(dst_neighbor_unit)) func_계략효과(dst_neighbor_unit, strategy_id, /*is_chain_reaction*/true);
			}

			// 실행부대 행동완료
			if (동맹진정_행동완료_적용) src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0) src_unit.action_done = false;

			return true;
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		bool func_계략효과(pk::unit@ dst_unit, int strategy_id, bool is_chain_reaction)
		{
			// 실행부대 및 대상부대의 계략관련 무장 확인 (컷인, 대사, 음성 관련)
			/// 참고 : 연환 및 반계 특기 발동 시 기본적으로는 해당특기 보유무장이 계략효과와 관련된 주체가 됨 (컷인의 경우 예외 있음)
			pk::person@ src_person = pk::get_person(src_unit.leader);
			pk::person@ dst_leader = pk::get_person(dst_unit.leader);
			if (is_chain_reaction) @src_person = pk::get_person(src_unit.who_has_skill(특기_연환));

			// 계략 크리티컬 여부 확인
			bool is_critical = pk::rand_bool(get_critical_chance(src_unit, dst_unit.pos, strategy_id));

			// 실행부대의 계략시전효과 : 컷인, 대사, 음성, 효과음, 효과
			func_계략시전효과_실행부대(src_person, is_critical, is_chain_reaction);

			// 성공확률 고정값 적용여부 확인
			if (!동맹진정_성공확률_고정값_적용) success_chance = get_success_chance(src_unit, dst_unit.pos, strategy_id);

			// 계략 성공확률 (진정 계략의 경우 간파여부는 의미가 없음)
			bool is_success = pk::rand_bool(success_chance);

			/// 참고 : 계략이 간파되면 무조건 성공하지 못하므로 계략의 성공여부 자체는 성공확률 충족여부(is_success)만 판단하면 됨
			if (is_success)
			{
				// 대상부대의 계략성공효과 : 대사, 음성, 효과음, 효과, 처리, 로그메시지
				func_계략성공효과_대상부대(src_unit, dst_unit, dst_leader, strategy_id, is_critical);
			}
			else
			{
				// 실행부대의 계략실패효과 : 대사
				func_계략실패효과_실행부대(src_unit, src_person);
			}

			// 실행부대의 계략시전보상 : 능력경험치, 공적 (연환에 의한 계략 시전 시에도 동일)
			func_계략시전보상_실행부대(src_unit, src_force, dst_unit, is_success, is_critical);

			return is_success;
		}

		void func_계략시전효과_실행부대(pk::person@ src_person, bool is_critical, bool is_chain_reaction)
		{
			if (!pk::is_in_screen(src_unit.pos)) return;

			// 실행부대 연환발동 시 추가 대사
			if (is_chain_reaction)
			{
				// 실행부대 연환발동 대사 (연환특기 보유무장)
				say_message(7574, /*p0*/src_person, /*p1*/null, /*u0*/null, /*p0_u*/src_unit);

				// 대기
				pk::wait(1, 500);
			}

			// 실행부대 계략시전 컷인 (계략컷인 적용 시, 크리티컬 발동 시, 플레이어 부대인 경우)
			if (bool(pk::core["strategy_cutin"]) and is_critical and src_unit.is_player())
			{
				// 연환발동 시
				if (is_chain_reaction)
				{
					// 컷인 음성
					pk::play_voice(33, src_person);

					// 컷인 효과음
					pk::play_se(25);

					// 컷인 효과 (연환특기 보유무장)
					int cutin_id = pk::get_cutin_id(src_person);
					pk::face_cutin(cutin_id);
				}

				// 일반시전 및 반계발동 시
				else
				{
					// 컷인 효과 (주장)
					pk::face_cutin(src_unit);	// 음성, 효과음 포함
				}
			}

			// 실행부대 계략시전 대사
			say_message(7122, /*p0*/src_person, /*p1*/null, /*u0*/null, /*p0_u*/src_unit);

			// 실행부대 계략시전 음성
			pk::play_voice(55, src_person);

			// 실행부대 계략시전 효과음
			if (is_critical)pk::play_se(79);
			else			pk::play_se(57, src_unit.pos);

			// 실행부대 계략시전 효과
			int effect_id = is_critical ? 79 : 58;
			pk::create_effect(effect_id, src_unit.pos);

			// 대기
			pk::wait(1, 1000);
		}

		void func_계략성공효과_대상부대(pk::unit@ src_unit, pk::unit@ dst_unit, pk::person@ dst_leader, int strategy_id, bool is_critical)
		{
			if (pk::is_in_screen(dst_unit.pos))
			{
				// 대상부대 계략성공 대사
				say_message(7132, /*p0*/dst_leader, /*p1*/null, /*u0*/null, /*p0_u*/dst_unit);

				// 대상부대 계략성공 효과음
				pk::play_se(2);					// pk::set_status() 함수에 포함된 효과음
				pk::play_se(41, dst_unit.pos);	// pk::set_status() 함수에 포함된 효과음
			}

			// 대상부대 계략성공 효과, 처리
			bool sound = false;
			pk::set_status(dst_unit, src_unit, 部队状态_通常, 1, sound);	// 로그메시지, 효과음(대상부대), 효과 포함

			if (pk::is_in_screen(dst_unit.pos))
			{
				// 대기
				pk::wait(1, 500);
			}
		}

		void func_계략실패효과_실행부대(pk::unit@ src_unit, pk::person@ src_person)
		{
			if (!pk::is_in_screen(src_unit.pos)) return;

			// 실행부대 계략실패 대사
			say_message(7139, /*p0*/src_person, /*p1*/null, /*u0*/null, /*p0_u*/src_unit);

			// 대기
			pk::wait(1, 500);
		}

		void func_계략시전보상_실행부대(pk::unit@ src_unit, pk::force@ src_force, pk::unit@ dst_unit, bool is_success, bool is_critical)
		{
			// 능력 경험치 상승 : 지력
			int stat_exp = (is_success) ? 2 : 1;
			pk::add_stat_exp(src_unit, 武将能力_智力, stat_exp);

			// 공적 상승
			int kouseki_point = (is_success) ? (is_critical ? 100 : 50) : 10;
			pk::add_kouseki(src_unit, kouseki_point);
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

				// 대상부대의 인접좌표에 존재하는 실행부대의 적부대 반환
		pk::unit@ get_dst_neighbor_unit(pk::unit@ src_unit, pk::unit@ dst_unit)
		{
			array<pk::unit@> dst_neighbor_unit_arr(0);
			array<pk::point> ring_pos_arr = pk::ring(dst_unit.pos, 1);
			for (int i = 0; i < int(ring_pos_arr.length); i++)
			{
				pk::point dst_neighbor_pos = ring_pos_arr[i];
				if (!pk::is_valid_pos(dst_neighbor_pos)) continue;

				pk::unit@ dst_neighbor_unit = pk::get_unit(dst_neighbor_pos);
				if (!pk::is_alive(dst_neighbor_unit)) continue;
				if (pk::is_enemy(src_unit, dst_neighbor_unit)) continue;	// 실행부대와 대상부대 인접부대가 적군인 경우 제외
				if (pk::is_enemy(dst_unit, dst_neighbor_unit)) continue;	// 대상부대와 대상부대 인접부대가 적군인 경우 제외
				if (dst_neighbor_unit.status != 部队状态_混乱 and dst_neighbor_unit.status != 部队状态_伪报) continue;	// 상태이상 부대가 아닌 경우 제외

				dst_neighbor_unit_arr.insertLast(dst_neighbor_unit);
			}

			if (int(dst_neighbor_unit_arr.length) == 0) return null;
			return dst_neighbor_unit_arr[pk::rand(int(dst_neighbor_unit_arr.length))];
		}

		// 계략 실행부대의 소비기력 반환
		int get_energy_cost(pk::unit@ src_unit, int strategy_id)
		{
			return cast<pk::func205_t>(pk::get_func(205))(src_unit, strategy_id);
		}

		// 계략 실행부대의 대상부대에 대한 성공확률 반환
		int get_success_chance(pk::unit@ src_unit, pk::point dst_pos, int strategy_id)
		{
			return cast<pk::func206_t>(pk::get_func(206))(src_unit, src_unit.pos, dst_pos, strategy_id).first;
		}

		// 계략 실행부대의 크리티컬 확률 반환
		int get_critical_chance(pk::unit@ src_unit, pk::point dst_pos, int strategy_id)
		{
			return cast<pk::func207_t>(pk::get_func(207))(src_unit, src_unit.pos, dst_pos, strategy_id);
		}

		// 계략 실행부대 및 대상부대의 대사 출력 : msg 폴더의 s11 파일 대사 소환 (계략관련 내용 - S11MSG01.s11 파일)
		void say_message(int msg_id, pk::person@ p0, pk::person@ p1, pk::unit@ u0, pk::unit@ p0_u)
		{
			pk::msg_param msg_param(msg_id, p0, p1);
			@msg_param.unit[0] = @u0;
			pk::say(pk::get_msg(msg_param), p0, p0_u);
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	}

	Main main;
}
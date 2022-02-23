// ## 2021/10/24 #江东新风#改写171trigger##
// ## 2020/07/26 ##
/*
	@제작 : masterpiecek


	[ 전투부대 수송량 제한 ]

	 * 전투부대가 출진 시 수송량이 제한됨 (조건에 따라 변동)
	 * 유저설정은 부대의 조작주체가 플레이어 1군단, 플레이어 위임군단, 컴퓨터인지에 따라 개별설정 가능

	1. 수송제한 항목 : 금, 병량
	 1) 항목별로 조건에 따라 조정된 제한비율만큼 최대 수송량이 제한됨
	  - 최대수송량 = 기존최대수송량 * 제한비율 / 100
	  - 제한비율 : 제한비율 기본값에 조건별 제한비율 조정값을 모두 곱하여 최종 제한비율 결정 (제한비율 최소값 이상 최대값 이하로 결정)
	 2) 메시지 표시 : 초과경고, 초과항목 안내, 최대수송량 정보

	2. 제한비율 조정조건
	 1) 계산조건 : 계산을 통해 조정값(배율)을 도출하는 조건 (각 조건의 최소값 이상 최대값 이하로 결정)
	  (1) 부대병력   : 부대의 병력에 비례하여 조정값 도출   (병력조건 조정값 = 부대병력 / 병력 기준값)
	  (2) 주장능력치 : 주장의 능력치에 비례하여 조정값 도출 (능력조건 조정값 = 주장의 적용능력 능력치 / 능력 기준값)
	  (3) 부대기력   : 부대의 기력에 비례하여 조정값 도출   (기력조건 조정값 = 부대기력 / 기력 기준값)
	 2) 지정조건 : 설정된 고정 조정값(배율)을 적용하는 조건 (각 조건의 조정값을 직접 설정)
	  - 특기, 기교, 계절 항목 추가 및 삭제 가능
	  (1) 무장특기 : 부대 소속 무장이 보유한 특기에 따라 조정값 차등 적용
	  (2) 세력기교 : 연구된 세력 기교에 따라 조정값 차등 적용
	  (3) 계절     : 현재 계절에 따라 조정값 차등 적용

	3. 출진제한 항목 : 공성병기, 함선
	 - 병력이 일정수 미만인 경우 공성병기 부대로 출진불가, 함선을 가지고 출진불가
*/

namespace 전투부대_수송량_제한
{

	///	=============================================  < 유저 설정 >  =====================================================

		/// 항목별 제한적용여부 설정
	const array<array<bool>> 限制输送量开关 =
	{
		/** 금 */ {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false},
		/**병량*/ {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false},
	};

	/// 최대 수송량 초과 시 메시지 표시 설정 (컴퓨터 제외)
	const array<bool> 메시지표시_초과경고 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true };
	const array<bool> 메시지표시_초과항목 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true };
	const array<bool> 메시지표시_최대정보 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true };

	/// 항목별 제한비율 기본값 설정 (0~100%)
	const array<array<float>> 제한비율_기본값 =
	{
		/** 금 */ {/*플레이어 1군단*/30.0f, /*플레이어 위임군단*/30.0f, /*컴퓨터*/60.0f},
		/**병량*/ {/*플레이어 1군단*/50.0f, /*플레이어 위임군단*/50.0f, /*컴퓨터*/80.0f},
	};

	/// 항목별 제한비율 최대값 설정 (최대 100%)
	const array<array<float>> 제한비율_최대값 =
	{
		/** 금 */ {/*플레이어 1군단*/60.0f, /*플레이어 위임군단*/60.0f, /*컴퓨터*/100.0f},
		/**병량*/ {/*플레이어 1군단*/80.0f, /*플레이어 위임군단*/80.0f, /*컴퓨터*/100.0f},
	};

	/// 항목별 제한비율 최소값 설정 (최소 0%)
	const array<array<float>> 제한비율_최소값 =
	{
		/** 금 */ {/*플레이어 1군단*/5.0f, /*플레이어 위임군단*/5.0f, /*컴퓨터*/20.0f},
		/**병량*/ {/*플레이어 1군단*/5.0f, /*플레이어 위임군단*/5.0f, /*컴퓨터*/20.0f},
	};

	/// 제한비율 조정조건 : 부대병력
	// 병력조건 조정값 = 부대병력 / 병력 기준값
	const array<bool> 비율조정_병력조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<float> 비율조정_병력조건_기준값 = {/*플레이어 1군단*/10000.0f, /*플레이어 위임군단*/10000.0f, /*컴퓨터*/5000.f };
	const array<float> 비율조정_병력조건_최소값 = {/*플레이어 1군단*/0.2f, /*플레이어 위임군단*/0.2f, /*컴퓨터*/0.5f };
	const array<float> 비율조정_병력조건_최대값 = {/*플레이어 1군단*/1.5f, /*플레이어 위임군단*/1.5f, /*컴퓨터*/2.0f };

	/// 제한비율 조정조건 : 주장능력치
	// 능력조건 조정값 = 주장의 적용능력 능력치 / 능력 기준값
	const array<bool> 비율조정_능력조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<float> 비율조정_능력조건_기준값 = {/*플레이어 1군단*/60.0f, /*플레이어 위임군단*/60.0f, /*컴퓨터*/40.f };
	const array<float> 비율조정_능력조건_최소값 = {/*플레이어 1군단*/0.2f, /*플레이어 위임군단*/0.2f, /*컴퓨터*/0.5f };
	const array<float> 비율조정_능력조건_최대값 = {/*플레이어 1군단*/1.5f, /*플레이어 위임군단*/1.5f, /*컴퓨터*/2.0f };
	const int 비율조정_능력조건_적용능력 = 무장능력_통솔;

	/// 제한비율 조정조건 : 부대기력
	// 기력조건 조정값 = 부대기력 / 기력 기준값
	const array<bool> 비율조정_기력조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<float> 비율조정_기력조건_기준값 = {/*플레이어 1군단*/60.0f, /*플레이어 위임군단*/60.0f, /*컴퓨터*/30.f };
	const array<float> 비율조정_기력조건_최소값 = {/*플레이어 1군단*/0.2f, /*플레이어 위임군단*/0.2f, /*컴퓨터*/0.5f };
	const array<float> 비율조정_기력조건_최대값 = {/*플레이어 1군단*/1.5f, /*플레이어 위임군단*/1.5f, /*컴퓨터*/2.0f };

	/// 제한비율 조정조건 : 무장특기
	// 특기 추가 및 삭제 가능 : 동일한 형식으로 추가 (특기 이름은 'kr enums.cpp' 참고)
	const array<bool> 비율조정_특기조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 비율조정값_특기 =
	{
		{특기_운반, array<float> = {/*플레이어 1군단*/2.0f, /*플레이어 위임군단*/2.0f, /*컴퓨터*/2.5f} }
	};

	/// 제한비율 조정조건 : 세력기교
	// 기교 추가 및 삭제 가능 : 동일한 형식으로 추가 (기교 이름은 'kr enums.cpp' 참고)
	// 단련기교 및 정예기교는 기교별 해당병기에만 적용되며 육상부대 한정
	const array<bool> 비율조정_기교조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 비율조정값_기교 =
	{
		{기교_목우유마, array<float> = {/*플레이어 1군단*/1.2f, /*플레이어 위임군단*/1.2f, /*컴퓨터*/1.4f} },
		{기교_숙련병,   array<float> = {/*플레이어 1군단*/1.1f, /*플레이어 위임군단*/1.1f, /*컴퓨터*/1.2f} },
		{기교_창병단련, array<float> = {/*플레이어 1군단*/1.2f, /*플레이어 위임군단*/1.2f, /*컴퓨터*/1.4f} },
		{기교_극병단련, array<float> = {/*플레이어 1군단*/1.2f, /*플레이어 위임군단*/1.2f, /*컴퓨터*/1.4f} },
		{기교_노병단련, array<float> = {/*플레이어 1군단*/1.2f, /*플레이어 위임군단*/1.2f, /*컴퓨터*/1.4f} },
		{기교_기병단련, array<float> = {/*플레이어 1군단*/1.2f, /*플레이어 위임군단*/1.2f, /*컴퓨터*/1.4f} },
		{기교_정예창병, array<float> = {/*플레이어 1군단*/1.4f, /*플레이어 위임군단*/1.4f, /*컴퓨터*/1.6f} },
		{기교_정예극병, array<float> = {/*플레이어 1군단*/1.4f, /*플레이어 위임군단*/1.4f, /*컴퓨터*/1.6f} },
		{기교_정예노병, array<float> = {/*플레이어 1군단*/1.4f, /*플레이어 위임군단*/1.4f, /*컴퓨터*/1.6f} },
		{기교_정예기병, array<float> = {/*플레이어 1군단*/1.4f, /*플레이어 위임군단*/1.4f, /*컴퓨터*/1.6f} }
	};

	/// 제한비율 조정조건 : 계절
	// 계절 추가 및 삭제 가능 : 동일한 형식으로 추가 (계절 이름은 'kr enums.cpp' 참고)
	const array<bool> 비율조정_계절조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 비율조정값_계절 =
	{
		{계절_봄,   array<float> = {/*플레이어 1군단*/1.0f, /*플레이어 위임군단*/1.0f, /*컴퓨터*/1.2f} },
		{계절_여름, array<float> = {/*플레이어 1군단*/0.8f, /*플레이어 위임군단*/0.8f, /*컴퓨터*/1.0f} },
		{계절_가을, array<float> = {/*플레이어 1군단*/1.0f, /*플레이어 위임군단*/1.0f, /*컴퓨터*/1.2f} },
		{계절_겨울, array<float> = {/*플레이어 1군단*/0.6f, /*플레이어 위임군단*/0.6f, /*컴퓨터*/0.8f} }
	};

	/// -------------------------------------------------------------------------------------------------------------------

	/// [ 전투부대 출진제한 ]

		/// 전투부대가 공성병기로 출진 시 일정 병력 미만인 경우 검병으로 변경
	const array<bool> 출진제한_적용_병기 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false };
	const array<bool> 알림창표시_병기 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false };
	const array<int>  제한기준병력_병기 = {/*플레이어 1군단*/500, /*플레이어 위임군단*/500, /*컴퓨터*/1000 };

	/// 전투부대가 함선(누선 및 투함)을 가지고 출진 시 일정 병력 미만인 경우 주가로 변경
	const array<bool> 출진제한_적용_함선 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false };
	const array<bool> 알림창표시_함선 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false };
	const array<int>  제한기준병력_함선 = {/*플레이어 1군단*/500, /*플레이어 위임군단*/500, /*컴퓨터*/1000 };

	///	===================================================================================================================

	const int 항목_금 = 0, 항목_병량 = 1;

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		int unit_id = -1;
		int unit_id_병기 = -1, wpn_id_병기 = -1;
		int unit_id_함선 = -1, wpn_id_함선 = -1;
		array<float> 수송량_최대(2, 0.0f);
		array<float> 수송량_초과(2, 0.0f);
		array<float> 거점복원정보(2, 0.0f);

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			pk::bind(170, pk::trigger170_t(부대생성_수송량_제한));
			pk::bind(172, pk::trigger172_t(부대좌표변경_수송량_거점복원));
			pk::bind(171, pk::trigger171_t(부대제거_수송량_거점복원));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void 부대생성_수송량_제한(pk::unit@ unit, int type)	// type - 0 출진, 1 임의
		{
			func_정보_초기화();

			if (type != 0) return;
			if (unit.type != 부대종류_전투) return;

			pk::force@ force = pk::get_force(unit.get_force_id());
			if (!pk::is_alive(force)) return;
			if (!pk::is_normal_force(unit.get_force_id())) return;

			pk::building@ building = pk::get_building(pk::get_service(unit));
			if (!pk::is_alive(building)) return;

			int controller_index = get_controller_index(unit);
			func_출진제한_병기(unit, controller_index);
			func_출진제한_함선(unit, controller_index);

			func_수송량_확인(unit, building, force, controller_index);
			if (!func_초과항목_존재여부()) return;

			func_수송량_조정(unit);
			func_메시지_표시(unit, building, controller_index);
		}

		void 부대좌표변경_수송량_거점복원(pk::unit@ unit, const pk::point& in pos)	// pos - 부대의 이동 후 최종 위치
		{
			if (unit.type != 부대종류_전투) return;

			pk::building@ building = pk::get_building(pk::get_service(unit));
			if (!pk::is_alive(building)) return;
			if (building.pos != unit.pos) return;	// 출진 중인 부대가 아닌 경우 제외
			if (pos == building.pos) return;	// 부대가 이동하지 않은 경우 제외

			func_수송량_거점복원(unit, building);
		}

		/// 참고 : 부대가 출진 후 바로 제거되는 경우에는 부대의 위치를 출진거점으로 인식
		void 부대제거_수송량_거점복원(pk::unit@ unit, pk::hex_object@ dst, int type)	// type - 0 괴멸, 1 진입
		{
			if (type == 0 and unit.troops != 0) return;	// 출진취소로 부대가 제거(괴멸)되는 경우 제외
			if (unit.type != 부대종류_전투) return;

			pk::building@ building_unit_pos = pk::get_building(unit.pos);
			if (!pk::is_alive(building_unit_pos)) return;

			pk::building@ building_unit_service = pk::get_building(pk::get_service(unit));
			if (!pk::is_alive(building_unit_service)) return;
			if (@building_unit_pos != @building_unit_service) return;	// 출진 후 다른 거점으로 바로 진입하는 경우, 출진 후 바로 괴멸되는 경우 예외처리

			func_수송량_거점복원(unit, building_unit_service);
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void func_출진제한_병기(pk::unit@ unit, int controller_index)
		{
			int ground_wpn_id = pk::get_ground_weapon_id(unit);
			int 제한기준병력 = 제한기준병력_병기[controller_index];
			if (!출진제한_적용_병기[controller_index]) return;
			if (int(unit.troops) >= 제한기준병력) return;
			if (pk::equipment_id_to_heishu(ground_wpn_id) != 병종_병기) return;

			unit_id_병기 = unit.get_id();
			wpn_id_병기 = ground_wpn_id;

			func_출진제한_무기변경(unit, ground_wpn_id, 병기_검, /*검 수량*/1, /*육상 병기*/0);
			if (알림창표시_병기[controller_index])
				func_출진제한_메시지_표시(unit, ground_wpn_id, "\x1b[2x陆地\x1b[0x", "\x1b[1x剑\x1b[0x", 제한기준병력);
		}

		void func_출진제한_함선(pk::unit@ unit, int controller_index)
		{
			int sea_wpn_id = pk::get_sea_weapon_id(unit);
			int 제한기준병력 = 제한기준병력_함선[controller_index];
			if (!출진제한_적용_함선[controller_index]) return;
			if (int(unit.troops) >= 제한기준병력) return;
			if (sea_wpn_id == 병기_주가) return;

			unit_id_함선 = unit.get_id();
			wpn_id_함선 = sea_wpn_id;

			func_출진제한_무기변경(unit, sea_wpn_id, 병기_주가, /*주가 수량*/0, /*해상 병기*/1);
			if (알림창표시_함선[controller_index])
				func_출진제한_메시지_표시(unit, sea_wpn_id, "\x1b[2x水上\x1b[0x", "\x1b[1x走舸\x1b[0x", 제한기준병력);
		}

		void func_출진제한_무기변경(pk::unit@ unit, int wpn_before, int wpn_after, int wpn_after_amount, int wpn_type_index)
		{
			unit.stock[wpn_type_index].id = wpn_after;
			unit.stock[wpn_type_index].amount = wpn_after_amount;
			//	pk::set_weapon_amount(unit, wpn_type_index, wpn_before, 0);
			//	pk::set_weapon_amount(unit, wpn_type_index, wpn_after, wpn_after_amount);
			unit.update();
		}

		void func_출진제한_메시지_표시(pk::unit@ unit, int wpn_before, string wpn_type_name, string wpn_after_name, int 제한기준병력)
		{
			pk::play_se(3);
			string unit_name = "\x1b[2x" + pk::decode(pk::get_name(unit)) + "\x1b[0x";
			string wpn_before_name = "\x1b[1x" + pk::decode(pk::get_name(pk::get_equipment(wpn_before))) + "\x1b[0x";
			string change_info = unit_name + "的" + wpn_type_name + "武器" + wpn_before_name + "在" + wpn_after_name + "以\x1b[16x变更\x1b[0x";
			string troops_std_info = "(" + wpn_before_name + " 出征\x1b[2x最低兵力要求为\x1b[0x:\x1b[1x" + 제한기준병력 + "\x1b[0x)";
			pk::message_box(pk::encode(change_info + "\n" + troops_std_info));
		}

		void func_수송량_확인(pk::unit@ unit, pk::building@ building, pk::force@ force, int controller_index)
		{
			// ------------------------------------------------修改与20/7/23  加入了运输量限制开关
			if (限制输送量开关[항목_금][controller_index])	//如果开启限制
			{
				수송량_최대[항목_금] = 10000 * get_limit_ratio(unit, force, 항목_금, controller_index);
				수송량_초과[항목_금] = pk::max(0, unit.gold - int(수송량_최대[항목_금]));
			}
			if (限制输送量开关[항목_병량][controller_index])	//如果开启限制
			{
				수송량_최대[항목_병량] = 50000 * get_limit_ratio(unit, force, 항목_병량, controller_index);
				수송량_초과[항목_병량] = pk::max(0, unit.food - int(수송량_최대[항목_병량]));	// 실수와 정수 간 변환 및 연산 시 주의해야 함
			}
			// ------------------------------------------------
			//	以下是原函数
			// 수송량_최대[항목_금]   = 10000 * get_limit_ratio(unit, force, 항목_금, controller_index);
			// 수송량_최대[항목_병량] = 50000 * get_limit_ratio(unit, force, 항목_병량, controller_index);
			// 수송량_초과[항목_금]   = pk::max(0, unit.gold - int(수송량_최대[항목_금]));
			// 수송량_초과[항목_병량] = pk::max(0, unit.food - int(수송량_최대[항목_병량]));	// 실수와 정수 간 변환 및 연산 시 주의해야 함
		}

		float get_limit_ratio(pk::unit@ unit, pk::force@ force, int item_index, int controller_index)
		{
			float limit_ratio = 제한비율_기본값[item_index][controller_index];
			pk::person@ leader = pk::get_person(unit.leader);
			int leader_stat = leader.stat[비율조정_능력조건_적용능력];
			if (비율조정_병력조건_적용[controller_index]) limit_ratio *= pk::max(비율조정_병력조건_최소값[controller_index], pk::min(비율조정_병력조건_최대값[controller_index], (pk::max(1.f, float(unit.troops)) / 비율조정_병력조건_기준값[controller_index])));
			if (비율조정_능력조건_적용[controller_index]) limit_ratio *= pk::max(비율조정_능력조건_최소값[controller_index], pk::min(비율조정_능력조건_최대값[controller_index], (pk::max(1.f, float(leader_stat)) / 비율조정_능력조건_기준값[controller_index])));
			if (비율조정_기력조건_적용[controller_index]) limit_ratio *= pk::max(비율조정_기력조건_최소값[controller_index], pk::min(비율조정_기력조건_최대값[controller_index], (pk::max(1.f, float(unit.energy)) / 비율조정_기력조건_기준값[controller_index])));
			if (비율조정_특기조건_적용[controller_index]) limit_ratio *= get_dict_value(비율조정값_특기, unit, force, controller_index, /*특기*/0);
			if (비율조정_기교조건_적용[controller_index]) limit_ratio *= get_dict_value(비율조정값_기교, unit, force, controller_index, /*기교*/1);
			if (비율조정_계절조건_적용[controller_index]) limit_ratio *= get_dict_value(비율조정값_계절, unit, force, controller_index, /*계절*/2);

			limit_ratio = pk::min(제한비율_최대값[item_index][controller_index], limit_ratio);
			limit_ratio = pk::max(제한비율_최소값[item_index][controller_index], limit_ratio);
			return pk::min(1.f, (limit_ratio / 100.f));
		}

		float get_dict_value(dictionary dict, pk::unit@ unit, pk::force@ force, int controller_index, int condition_index)
		{
			float n = 1.f;
			bool 조건충족여부 = false;
			array<string> index_name_arr = dict.getKeys();
			for (int i = 0; i < int(index_name_arr.length); i++)
			{
				int index_value = parseInt(index_name_arr[i]);
				if (condition_index == /*특기*/0) 조건충족여부 = unit.has_skill(index_value);
				else if (condition_index == /*기교*/1) 조건충족여부 = pk::has_tech(force, index_value) and check_wpn_for_tech(unit.weapon, index_value);
				else if (condition_index == /*계절*/2) 조건충족여부 = (pk::get_season() == index_value);
				if (!조건충족여부) continue;

				array<float> dict_value_arr = array<float>(dict[index_name_arr[i]]);
				float dict_value = dict_value_arr[controller_index];
				n *= dict_value;
			}

			return n;
		}

		bool check_wpn_for_tech(int weapon_id, int tech_id)
		{
			if ((tech_id == 기교_창병단련 or tech_id == 기교_정예창병) and weapon_id != 병기_창)   return false;
			if ((tech_id == 기교_극병단련 or tech_id == 기교_정예극병) and weapon_id != 병기_극)   return false;
			if ((tech_id == 기교_노병단련 or tech_id == 기교_정예노병) and weapon_id != 병기_노)   return false;
			if ((tech_id == 기교_기병단련 or tech_id == 기교_정예기병) and weapon_id != 병기_군마) return false;

			return true;
		}

		void func_수송량_조정(pk::unit@ unit)
		{
			unit_id = unit.get_id();
			거점복원정보[항목_금] = 수송량_초과[항목_금];
			거점복원정보[항목_병량] = 수송량_초과[항목_병량];
			if (수송량_초과[항목_금] > 0) pk::set_gold(unit, int(수송량_최대[항목_금]));
			if (수송량_초과[항목_병량] > 0) pk::set_food(unit, int(수송량_최대[항목_병량]));
		}

		void func_메시지_표시(pk::unit@ unit, pk::building@ building, int controller_index)
		{
			if (!unit.is_player()) return;

			string building_name = "\x1b[1x" + pk::decode(pk::get_name(building)) + "\x1b[0x";
			array<string> excess_item_arr(0);
			if (수송량_초과[항목_금] > 0) excess_item_arr.insertLast("\x1b[1x资金\x1b[0x");
			if (수송량_초과[항목_병량] > 0) excess_item_arr.insertLast("\x1b[1x兵粮\x1b[0x");

			string warning_msg = "\x1b[0x战斗部队的运输量\n\x1b[29x超出\x1b[0x了辎重水平!\x1b[0x(" + building_name + ")";
			string excess_info = "\x1b[0x超出项目:" + join(excess_item_arr, "\x1b[0x,");
			string max_info = "\x1b[0x<最大运输量(\x1b[2x兵力\x1b[0x" + unit.troops + "要求)>\n";
			max_info += "\x1b[1x资金\x1b[0x" + int(수송량_최대[항목_금]) + ",\x1b[1x兵粮\x1b[0x" + int(수송량_최대[항목_병량]);

			if (메시지표시_초과경고[controller_index]) pk::message_box(pk::encode(warning_msg), pk::get_person(unit.leader));
			if (메시지표시_초과항목[controller_index]) pk::message_box(pk::encode(excess_info), pk::get_person(unit.leader));
			if (메시지표시_최대정보[controller_index]) pk::message_box(pk::encode(max_info), pk::get_person(unit.leader));
		}

		bool func_초과항목_존재여부()
		{
			return (수송량_초과[항목_금] > 0 or 수송량_초과[항목_병량] > 0);
		}

		void func_수송량_거점복원(pk::unit@ unit, pk::building@ building)
		{
			if (unit.get_id() == unit_id_함선) pk::add_weapon_amount(building, wpn_id_함선, 1, false);
			if (unit.get_id() == unit_id_병기) pk::add_weapon_amount(building, wpn_id_병기, 1, false);
			if (unit.get_id() == unit_id)
			{
				pk::add_gold(building, int(거점복원정보[항목_금]));
				pk::add_food(building, int(거점복원정보[항목_병량]));
			}
		}

		void func_정보_초기화()
		{
			unit_id = unit_id_함선 = wpn_id_함선 = unit_id_병기 = wpn_id_병기 = -1;
			for (int i = 0; i < 2; i++) 수송량_최대[i] = 수송량_초과[i] = 거점복원정보[i] = 0.0f;
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		int get_controller_index(pk::unit@ unit)
		{
			return (pk::is_player_controlled(unit) ? /*플레이어 1군단*/0 : (unit.is_player() ? /*플레이어 위임군단*/1 : /*컴퓨터*/2));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	}

	Main main;
}
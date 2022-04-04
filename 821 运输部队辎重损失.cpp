// ## 2021/02/14 #江东新风#加入严整特技##/
// ## 2020/08/06 #messi#調低了参数和?率##
// ## 2020/08/01 ##
/*
	@제작 : masterpiecek


	[ 수송부대 수송량 손실 ]

	 * 새 날마다 수송부대의 수송량이 손실됨 (조건에 따라 변동)
	 * 유저설정은 부대의 조작주체가 플레이어 1군단, 플레이어 위임군단, 컴퓨터인지에 따라 개별설정 가능

	1. 금, 병량, 일반병기 손실
	 1) 수송부대가 항목별로 일정 손실확률 충족 시 조건에 따라 조정된 손실비율만큼 수송량 손실 발생
	  - 손실량 = 수송량 * 손실비율 / 100
	  (1) 손실확률 : 항목별로 고정 확률 적용
	  (2) 손실비율 : 손실비율 기본값에 조건별 손실비율 조정값을 모두 곱하여 최종 손실비율 결정 (손실비율 최소값 이상 최대값 이하로 결정)
	 2) 부대의 말풍선 표시

	2. 공성병기, 함선 손실
	 1) 수송부대가 조건에 따라 조정된 손실확률 충족하는 경우 손실수량 적용단위에 따라 일정수량 손실 발생
	  - 손실량 = 기본손실수량 + 수송량의 적용단위에 따른 추가손실량
	  (1) 손실확률 : 손실확률 기본값에 조건별 손실확률 조정값을 모두 곱하여 최종 손실확률 결정 (손실확률 최소값 이상 최대값 이하로 결정)
	  (2) 손실수량 : 기본 손실수량에서 수송량이 적용단위만큼 늘어날 때마다 추가 손실수량이 1개씩 늘어남
	 2) 부대의 말풍선 표시

	3. 손실비율(금, 병량) 및 손실확률(공성병기, 함선) 조정조건
	 1) 계산조건 : 계산을 통해 조정값(배율)을 도출하는 조건 (각 조건의 최소값 이상 최대값 이하로 결정)
	  (1) 부대병력   : 부대의 병력에 반비례하여 조정값 도출   (병력조건 조정값 = 병력 기준값 / 부대병력)
	  (2) 주장능력치 : 주장의 능력치에 반비례하여 조정값 도출 (능력조건 조정값 = 능력 기준값 / 주장의 적용능력 능력치)
	  (3) 부대기력   : 부대의 기력에 반비례하여 조정값 도출   (기력조건 조정값 = 기력 기준값 / 부대기력)
	 2) 지정조건 : 설정된 고정 조정값(배율)을 적용하는 조건 (각 조건의 조정값을 직접 설정)
	  - 특기, 기교, 계절, 지형 항목 추가 및 삭제 가능
	  (1) 무장특기 : 부대 소속 무장이 보유한 특기에 따라 조정값 차등 적용
	  (2) 세력기교 : 연구된 세력 기교에 따라 조정값 차등 적용
	  (3) 계절     : 현재 계절에 따라 조정값 차등 적용
	  (4) 지형     : 부대가 위치한 지형에 따라 조정값 차등 적용
*/

namespace 수송부대_수송량_손실
{

	///	=============================================  < 유저 설정 >  =====================================================

	/// [ 수송량 손실 : 금, 병량, 일반병기 ]

		/// 항목별 손실적용여부 설정
	const array<bool> 손실적용_금 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false };
	const array<bool> 손실적용_병량 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false };
	const array<bool> 손실적용_일반병기 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false };

	/// 항목별 손실확률 설정
	const array<int> 손실확률_금 = {/*플레이어 1군단*/80, /*플레이어 위임군단*/80, /*컴퓨터*/10 };
	const array<int> 손실확률_병량 = {/*플레이어 1군단*/80, /*플레이어 위임군단*/80, /*컴퓨터*/10 };
	const array<int> 손실확률_일반병기 = {/*플레이어 1군단*/80, /*플레이어 위임군단*/80, /*컴퓨터*/10 };

	/// 손실비율 기본 설정
	const array<float> 손실비율_기본값 = {/*플레이어 1군단*/6.0f, /*플레이어 위임군단*/6.0f, /*컴퓨터*/3.0f };
	const array<float> 손실비율_최대값 = {/*플레이어 1군단*/20.00f, /*플레이어 위임군단*/20.00f, /*컴퓨터*/5.0f };
	const array<float> 손실비율_최소값 = {/*플레이어 1군단*/3.0f, /*플레이어 위임군단*/3.0f, /*컴퓨터*/1.5f };

	/// 손실비율 조정조건 : 부대병력
	// 병력조건 조정값 = 병력 기준값 / 부대병력
	const array<bool> 비율조정_병력조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<float> 비율조정_병력조건_기준값 = {/*플레이어 1군단*/15000.0f, /*플레이어 위임군단*/15000.0f, /*컴퓨터*/7500.f };
	const array<float> 비율조정_병력조건_최소값 = {/*플레이어 1군단*/0.8f, /*플레이어 위임군단*/0.8f, /*컴퓨터*/0.5f };
	const array<float> 비율조정_병력조건_최대값 = {/*플레이어 1군단*/2.0f, /*플레이어 위임군단*/2.0f, /*컴퓨터*/1.5f };

	/// 손실비율 조정조건 : 주장능력치
	// 능력조건 조정값 = 능력 기준값 / 주장의 적용능력 능력치
	const array<bool> 비율조정_능력조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<float> 비율조정_능력조건_기준값 = {/*플레이어 1군단*/60.0f, /*플레이어 위임군단*/60.0f, /*컴퓨터*/30.f };
	const array<float> 비율조정_능력조건_최소값 = {/*플레이어 1군단*/0.8f, /*플레이어 위임군단*/0.8f, /*컴퓨터*/0.5f };
	const array<float> 비율조정_능력조건_최대값 = {/*플레이어 1군단*/2.0f, /*플레이어 위임군단*/2.0f, /*컴퓨터*/1.5f };
	const int 비율조정_능력조건_적용능력 = 武将能力_统率;

	/// 손실비율 조정조건 : 부대기력
	// 기력조건 조정값 = 기력 기준값 / 부대기력
	const array<bool> 비율조정_기력조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<float> 비율조정_기력조건_기준값 = {/*플레이어 1군단*/60.0f, /*플레이어 위임군단*/60.0f, /*컴퓨터*/30.f };
	const array<float> 비율조정_기력조건_최소값 = {/*플레이어 1군단*/0.8f, /*플레이어 위임군단*/0.8f, /*컴퓨터*/0.5f };
	const array<float> 비율조정_기력조건_최대값 = {/*플레이어 1군단*/2.0f, /*플레이어 위임군단*/2.0f, /*컴퓨터*/1.5f };

	/// 손실비율 조정조건 : 무장특기
	// 특기 추가 및 삭제 가능 : 동일한 형식으로 추가 (특기 이름은 'kr enums.cpp' 참고)
	const array<bool> 비율조정_특기조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 비율조정값_특기 =
	{
		{특기_운반, array<float> = {/*플레이어 1군단*/0.5f, /*플레이어 위임군단*/0.5f, /*컴퓨터*/0.25f} }
	};

	/// 손실비율 조정조건 : 세력기교
	// 기교 추가 및 삭제 가능 : 동일한 형식으로 추가 (기교 이름은 'kr enums.cpp' 참고)
	const array<bool> 비율조정_기교조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 비율조정값_기교 =
	{
		{기교_목우유마, array<float> = {/*플레이어 1군단*/0.8f, /*플레이어 위임군단*/0.8f, /*컴퓨터*/0.4f} },
		{기교_숙련병,   array<float> = {/*플레이어 1군단*/0.9f, /*플레이어 위임군단*/0.9f, /*컴퓨터*/0.45f} }
	};

	/// 손실비율 조정조건 : 계절
	// 계절 추가 및 삭제 가능 : 동일한 형식으로 추가 (계절 이름은 'kr enums.cpp' 참고)
	const array<bool> 비율조정_계절조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 비율조정값_계절 =
	{
		{계절_봄,   array<float> = {/*플레이어 1군단*/1.0f, /*플레이어 위임군단*/1.0f, /*컴퓨터*/0.8f} },
		{계절_여름, array<float> = {/*플레이어 1군단*/1.2f, /*플레이어 위임군단*/1.2f, /*컴퓨터*/1.0f} },
		{계절_가을, array<float> = {/*플레이어 1군단*/1.0f, /*플레이어 위임군단*/1.0f, /*컴퓨터*/0.8f} },
		{계절_겨울, array<float> = {/*플레이어 1군단*/1.4f, /*플레이어 위임군단*/1.4f, /*컴퓨터*/1.2f} }
	};

	/// 손실비율 조정조건 : 지형
	// 지형 추가 및 삭제 가능 : 동일한 형식으로 추가 (지형 이름은 'kr enums.cpp' 참고)
	const array<bool> 비율조정_지형조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 비율조정값_지형 =
	{
		{지형_모래,   array<float> = {/*플레이어 1군단*/1.1f, /*플레이어 위임군단*/1.1f, /*컴퓨터*/1.0f} },
		{지형_습지,   array<float> = {/*플레이어 1군단*/1.3f, /*플레이어 위임군단*/1.3f, /*컴퓨터*/1.1f} },
		{지형_독천,   array<float> = {/*플레이어 1군단*/2.0f, /*플레이어 위임군단*/2.0f, /*컴퓨터*/1.4f} },
		{지형_숲,     array<float> = {/*플레이어 1군단*/1.1f, /*플레이어 위임군단*/1.1f, /*컴퓨터*/1.0f} },
		{지형_내,     array<float> = {/*플레이어 1군단*/1.1f, /*플레이어 위임군단*/1.1f, /*컴퓨터*/1.0f} },	// 천
		{지형_바다,   array<float> = {/*플레이어 1군단*/1.3f, /*플레이어 위임군단*/1.3f, /*컴퓨터*/1.1f} },
		{지형_벼랑길, array<float> = {/*플레이어 1군단*/2.0f, /*플레이어 위임군단*/2.0f, /*컴퓨터*/1.4f} },
		{지형_산,     array<float> = {/*플레이어 1군단*/1.5f, /*플레이어 위임군단*/1.5f, /*컴퓨터*/1.2f} }	// 안
	};

	/// -------------------------------------------------------------------------------------------------------------------

	/// [ 수송량 손실 : 공성병기, 함선 ]

		/// 항목별 손실적용여부 설정
	const array<bool> 손실적용_공성병기 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false };
	const array<bool> 손실적용_함선 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false };

	/// 항목별 손실수량 설정
	// 기본값 설명 : 공성병기 및 함선의 수송량이 적용단위인 10개 미만인 경우 최종손실수량은 기본손실수량 1개가 됨,
	//               공성병기 및 함선의 수송량이 10개 이상 20개 미만인 경우 최종손실수량은 기본손실수량 1개에 추가손실수량 1개를 더해 2개가 됨,
	//               공성병기 및 함선의 수송량이 20개 이상 30개 미만인 경우 최종손실수량은 기본손실수량 1개에 추가손실수량 2개를 더해 3개가 됨
	const array<int> 기본손실수량_병기 = {/*플레이어 1군단*/1, /*플레이어 위임군단*/1, /*컴퓨터*/1 };
	const array<int> 기본손실수량_함선 = {/*플레이어 1군단*/1, /*플레이어 위임군단*/1, /*컴퓨터*/1 };
	const array<int> 손실수량_적용단위_병기 = {/*플레이어 1군단*/10, /*플레이어 위임군단*/10, /*컴퓨터*/10 };
	const array<int> 손실수량_적용단위_함선 = {/*플레이어 1군단*/10, /*플레이어 위임군단*/10, /*컴퓨터*/10 };

	/// 손실확률 기본 설정
	const array<float> 손실확률_기본값 = {/*플레이어 1군단*/6.0f, /*플레이어 위임군단*/6.0f, /*컴퓨터*/3.0f };
	const array<float> 손실확률_최대값 = {/*플레이어 1군단*/20.00f, /*플레이어 위임군단*/20.00f, /*컴퓨터*/10.0f };
	const array<float> 손실확률_최소값 = {/*플레이어 1군단*/3.0f, /*플레이어 위임군단*/3.0f, /*컴퓨터*/1.5f };

	/// 손실확률 조정조건 : 부대병력
	// 병력조건 조정값 = 병력 기준값 / 부대병력
	const array<bool> 확률조정_병력조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<float> 확률조정_병력조건_기준값 = {/*플레이어 1군단*/15000.0f, /*플레이어 위임군단*/15000.0f, /*컴퓨터*/7500.f };
	const array<float> 확률조정_병력조건_최소값 = {/*플레이어 1군단*/0.8f, /*플레이어 위임군단*/0.8f, /*컴퓨터*/0.5f };
	const array<float> 확률조정_병력조건_최대값 = {/*플레이어 1군단*/2.0f, /*플레이어 위임군단*/2.0f, /*컴퓨터*/1.5f };

	/// 손실확률 조정조건 : 주장능력치
	// 능력조건 조정값 = 능력 기준값 / 주장의 적용능력 능력치
	const array<bool> 확률조정_능력조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<float> 확률조정_능력조건_기준값 = {/*플레이어 1군단*/60.0f, /*플레이어 위임군단*/60.0f, /*컴퓨터*/30.f };
	const array<float> 확률조정_능력조건_최소값 = {/*플레이어 1군단*/0.8f, /*플레이어 위임군단*/0.8f, /*컴퓨터*/0.5f };
	const array<float> 확률조정_능력조건_최대값 = {/*플레이어 1군단*/2.0f, /*플레이어 위임군단*/2.0f, /*컴퓨터*/1.5f };
	const int 확률조정_능력조건_적용능력 = 武将能力_统率;

	/// 손실확률 조정조건 : 부대기력
	// 기력조건 조정값 = 기력 기준값 / 부대기력
	const array<bool> 확률조정_기력조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<float> 확률조정_기력조건_기준값 = {/*플레이어 1군단*/60.0f, /*플레이어 위임군단*/60.0f, /*컴퓨터*/30.f };
	const array<float> 확률조정_기력조건_최소값 = {/*플레이어 1군단*/0.8f, /*플레이어 위임군단*/0.8f, /*컴퓨터*/0.5f };
	const array<float> 확률조정_기력조건_최대값 = {/*플레이어 1군단*/2.0f, /*플레이어 위임군단*/2.0f, /*컴퓨터*/1.5f };

	/// 손실확률 조정조건 : 무장특기
	// 특기 추가 및 삭제 가능 : 동일한 형식으로 추가 (특기 이름은 'kr enums.cpp' 참고)
	const array<bool> 확률조정_특기조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 확률조정값_특기 =
	{
		{특기_운반, array<float> = {/*플레이어 1군단*/0.5f, /*플레이어 위임군단*/0.5f, /*컴퓨터*/0.25f} }
	};

	/// 손실확률 조정조건 : 세력기교
	// 기교 추가 및 삭제 가능 : 동일한 형식으로 추가 (기교 이름은 'kr enums.cpp' 참고)
	const array<bool> 확률조정_기교조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 확률조정값_기교 =
	{
		{기교_목우유마, array<float> = {/*플레이어 1군단*/0.8f, /*플레이어 위임군단*/0.8f, /*컴퓨터*/0.4f} },
		{기교_숙련병,   array<float> = {/*플레이어 1군단*/0.9f, /*플레이어 위임군단*/0.9f, /*컴퓨터*/0.45f} }
	};

	/// 손실확률 조정조건 : 계절
	// 계절 추가 및 삭제 가능 : 동일한 형식으로 추가 (계절 이름은 'kr enums.cpp' 참고)
	const array<bool> 확률조정_계절조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 확률조정값_계절 =
	{
		{계절_봄,   array<float> = {/*플레이어 1군단*/1.0f, /*플레이어 위임군단*/1.0f, /*컴퓨터*/0.8f} },
		{계절_여름, array<float> = {/*플레이어 1군단*/1.2f, /*플레이어 위임군단*/1.2f, /*컴퓨터*/1.0f} },
		{계절_가을, array<float> = {/*플레이어 1군단*/1.0f, /*플레이어 위임군단*/1.0f, /*컴퓨터*/0.8f} },
		{계절_겨울, array<float> = {/*플레이어 1군단*/1.4f, /*플레이어 위임군단*/1.4f, /*컴퓨터*/1.2f} }
	};

	/// 손실확률 조정조건 : 지형
	// 지형 추가 및 삭제 가능 : 동일한 형식으로 추가 (지형 이름은 'kr enums.cpp' 참고)
	const array<bool> 확률조정_지형조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 확률조정값_지형 =
	{
		{지형_모래,   array<float> = {/*플레이어 1군단*/1.1f, /*플레이어 위임군단*/1.1f, /*컴퓨터*/1.0f} },
		{지형_습지,   array<float> = {/*플레이어 1군단*/1.3f, /*플레이어 위임군단*/1.3f, /*컴퓨터*/1.1f} },
		{지형_독천,   array<float> = {/*플레이어 1군단*/2.0f, /*플레이어 위임군단*/2.0f, /*컴퓨터*/1.4f} },
		{지형_숲,     array<float> = {/*플레이어 1군단*/1.1f, /*플레이어 위임군단*/1.1f, /*컴퓨터*/1.0f} },
		{지형_내,     array<float> = {/*플레이어 1군단*/1.1f, /*플레이어 위임군단*/1.1f, /*컴퓨터*/1.0f} },	// 천
		{지형_바다,   array<float> = {/*플레이어 1군단*/1.3f, /*플레이어 위임군단*/1.3f, /*컴퓨터*/1.1f} },
		{지형_벼랑길, array<float> = {/*플레이어 1군단*/2.0f, /*플레이어 위임군단*/2.0f, /*컴퓨터*/1.4f} },
		{지형_산,     array<float> = {/*플레이어 1군단*/1.5f, /*플레이어 위임군단*/1.5f, /*컴퓨터*/1.2f} }	// 안
	};

	/// -------------------------------------------------------------------------------------------------------------------

		/// 수송량 손실 시 대사표시 설정
	const array<bool> 손실대사_표시 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<string> 손실대사_목록 =
	{
		"快谨慎保管物资",
		"抓紧了解损失的物资"
	};

	///	===================================================================================================================

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			pk::bind(107, pk::trigger107_t(새날_수송량_손실));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void 새날_수송량_손실()
		{
			for (int unit_id = 0; unit_id < 部队_末; unit_id++)
			{
				pk::unit@ unit = pk::get_unit(unit_id);
				if (!pk::is_alive(unit)) continue;
				if (unit.type != 部队类型_运输) continue;
				if (ch::has_skill(unit, 特技_严整)) continue;

				pk::force@ force = pk::get_force(unit.get_force_id());
				if (!pk::is_alive(force)) continue;
				if (!pk::is_normal_force(force)) continue;

				bool lost_supplies = false;
				int controller_index = get_controller_index(unit);
				float loss_ratio = get_loss_ratio(unit, force, controller_index);
				func_금_손실(unit, loss_ratio, controller_index, lost_supplies);
				func_병량_손실(unit, loss_ratio, controller_index, lost_supplies);
				func_일반병기_손실(unit, loss_ratio, controller_index, lost_supplies);

				float loss_chance = get_loss_chance(unit, force, controller_index);
				func_공성병기_손실(unit, loss_chance, controller_index, lost_supplies);
				func_함선_손실(unit, loss_chance, controller_index, lost_supplies);

				func_손실대사_표시(unit, lost_supplies, controller_index);
			}
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		float get_loss_ratio(pk::unit@ unit, pk::force@ force, int controller_index)
		{
			float loss_ratio = 손실비율_기본값[controller_index];
			pk::person@ leader = pk::get_person(unit.leader);
			int leader_stat = leader.stat[비율조정_능력조건_적용능력];
			if (비율조정_병력조건_적용[controller_index]) loss_ratio *= pk::max(비율조정_병력조건_최소값[controller_index], pk::min(비율조정_병력조건_최대값[controller_index], (비율조정_병력조건_기준값[controller_index] / pk::max(1, unit.troops))));
			if (비율조정_능력조건_적용[controller_index]) loss_ratio *= pk::max(비율조정_능력조건_최소값[controller_index], pk::min(비율조정_능력조건_최대값[controller_index], (비율조정_능력조건_기준값[controller_index] / pk::max(1, leader_stat))));
			if (비율조정_기력조건_적용[controller_index]) loss_ratio *= pk::max(비율조정_기력조건_최소값[controller_index], pk::min(비율조정_기력조건_최대값[controller_index], (비율조정_기력조건_기준값[controller_index] / pk::max(1, unit.energy))));
			if (비율조정_특기조건_적용[controller_index]) loss_ratio *= get_dict_value(비율조정값_특기, unit, force, controller_index, /*특기*/0);
			if (비율조정_기교조건_적용[controller_index]) loss_ratio *= get_dict_value(비율조정값_기교, unit, force, controller_index, /*기교*/1);
			if (비율조정_계절조건_적용[controller_index]) loss_ratio *= get_dict_value(비율조정값_계절, unit, force, controller_index, /*계절*/2);
			if (비율조정_지형조건_적용[controller_index]) loss_ratio *= get_dict_value(비율조정값_지형, unit, force, controller_index, /*지형*/3);

			return pk::max(손실비율_최소값[controller_index], pk::min(손실비율_최대값[controller_index], loss_ratio));
		}

		float get_loss_chance(pk::unit@ unit, pk::force@ force, int controller_index)
		{
			float loss_chance = 손실확률_기본값[controller_index];
			pk::person@ leader = pk::get_person(unit.leader);
			int leader_stat = leader.stat[확률조정_능력조건_적용능력];
			if (확률조정_병력조건_적용[controller_index]) loss_chance *= pk::max(확률조정_병력조건_최소값[controller_index], pk::min(확률조정_병력조건_최대값[controller_index], (확률조정_병력조건_기준값[controller_index] / pk::max(1, unit.troops))));
			if (확률조정_능력조건_적용[controller_index]) loss_chance *= pk::max(확률조정_능력조건_최소값[controller_index], pk::min(확률조정_능력조건_최대값[controller_index], (확률조정_능력조건_기준값[controller_index] / pk::max(1, leader_stat))));
			if (확률조정_기력조건_적용[controller_index]) loss_chance *= pk::max(확률조정_기력조건_최소값[controller_index], pk::min(확률조정_기력조건_최대값[controller_index], (확률조정_기력조건_기준값[controller_index] / pk::max(1, unit.energy))));
			if (확률조정_특기조건_적용[controller_index]) loss_chance *= get_dict_value(확률조정값_특기, unit, force, controller_index, /*특기*/0);
			if (확률조정_기교조건_적용[controller_index]) loss_chance *= get_dict_value(확률조정값_기교, unit, force, controller_index, /*기교*/1);
			if (확률조정_계절조건_적용[controller_index]) loss_chance *= get_dict_value(확률조정값_계절, unit, force, controller_index, /*계절*/2);
			if (확률조정_지형조건_적용[controller_index]) loss_chance *= get_dict_value(확률조정값_지형, unit, force, controller_index, /*지형*/3);

			return pk::max(손실확률_최소값[controller_index], pk::min(손실확률_최대값[controller_index], loss_chance));
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
				else if (condition_index == /*기교*/1) 조건충족여부 = pk::has_tech(force, index_value);
				else if (condition_index == /*계절*/2) 조건충족여부 = (pk::get_season() == index_value);
				else if (condition_index == /*지형*/3) 조건충족여부 = (pk::get_hex(unit.pos).terrain == index_value);
				if (!조건충족여부) continue;

				array<float> dict_value_arr = array<float>(dict[index_name_arr[i]]);
				float dict_value = dict_value_arr[controller_index];
				n *= dict_value;
			}

			return n;
		}

		void func_금_손실(pk::unit@ unit, float loss_ratio, int controller_index, bool& out lost_supplies)
		{
			if (!손실적용_금[controller_index]) return;
			if (!pk::rand_bool(손실확률_금[controller_index])) return;

			lost_supplies = true;
			pk::add_gold(unit, -int(unit.gold * loss_ratio / 100.f), true);
		}

		void func_병량_손실(pk::unit@ unit, float loss_ratio, int controller_index, bool& out lost_supplies)
		{
			if (!손실적용_병량[controller_index]) return;
			if (!pk::rand_bool(손실확률_병량[controller_index])) return;

			lost_supplies = true;
			pk::add_food(unit, -int(unit.food * loss_ratio / 100.f), true);
		}

		void func_일반병기_손실(pk::unit@ unit, float loss_ratio, int controller_index, bool& out lost_supplies)
		{
			if (!손실적용_일반병기[controller_index]) return;

			for (int weapon_id = 兵器_枪; weapon_id <= 兵器_战马; weapon_id++)
			{
				if (!pk::rand_bool(손실확률_일반병기[controller_index])) continue;
				lost_supplies = true;
				pk::set_weapon_amount(unit, weapon_id, weapon_id, int(unit.stock[weapon_id].amount * (1.0f - loss_ratio / 100.f)));
			}
		}

		void func_공성병기_손실(pk::unit@ unit, float loss_chance, int controller_index, bool& out lost_supplies)
		{
			if (!손실적용_공성병기[controller_index]) return;

			for (int weapon_id = 兵器_冲车; weapon_id <= 兵器_木兽; weapon_id++)
			{
				if (!pk::rand_bool(int(loss_chance))) continue;
				lost_supplies = true;
				int curr_wpn_ampunt = int(unit.stock[weapon_id].amount);
				int wpn_loss = 기본손실수량_병기[controller_index] + (curr_wpn_ampunt / 손실수량_적용단위_병기[controller_index]);
				pk::set_weapon_amount(unit, weapon_id, weapon_id, pk::max(0, curr_wpn_ampunt - wpn_loss));
			}
		}

		void func_함선_손실(pk::unit@ unit, float loss_chance, int controller_index, bool& out lost_supplies)
		{
			if (!손실적용_함선[controller_index]) return;

			for (int weapon_id = 兵器_走舸; weapon_id <= 兵器_斗舰; weapon_id++)
			{
				if (!pk::rand_bool(int(loss_chance))) continue;
				lost_supplies = true;
				int curr_wpn_ampunt = int(unit.stock[weapon_id].amount);
				int wpn_loss = 기본손실수량_함선[controller_index] + (curr_wpn_ampunt / 손실수량_적용단위_함선[controller_index]);
				pk::set_weapon_amount(unit, weapon_id, weapon_id, pk::max(0, curr_wpn_ampunt - wpn_loss));
			}
		}

		void func_손실대사_표시(pk::unit@ unit, bool lost_supplies, int controller_index)
		{
			if (!lost_supplies) return;
			if (!손실대사_표시[controller_index]) return;
			if (!pk::is_in_screen(unit.pos)) return;

			pk::person@ leader = pk::get_person(unit.leader);
			string dialogue = 손실대사_목록[pk::rand(int(손실대사_목록.length))];
			pk::say(pk::encode(dialogue), leader);
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
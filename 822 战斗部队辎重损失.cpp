// ## 2021/02/14 #江东新风#加入严整特技##/
// ## 2020/12/12 #江东新风#修正参数类型错误##
// ## 2020/09/23 #江东新风#参数汉化，金兵比或粮兵比小于阈值时，不触发损失##
// ## 2020/08/06 #messi#調低了参数和?率##
// ## 2020/08/01 ##
/*
	@제작 : masterpiecek


	[ 전투부대 수송량 손실 ]

	 * 새 날마다 전투부대의 수송량이 손실됨 (조건에 따라 변동)
	 * 유저설정은 부대의 조작주체가 玩家第1军团, 玩家委任军团, AI势力인지에 따라 개별설정 가능

	1. 금, 병량 손실
	 1) 전투부대가 항목별로 일정 손실확률 충족 시 조건에 따라 조정된 손실비율만큼 수송량 손실 발생
	  - 손실량 = 수송량 * 손실비율 / 100
	  (1) 손실확률 : 항목별로 고정 확률 적용
	  (2) 손실비율 : 손실비율 기본값에 조건별 손실비율 조정값을 모두 곱하여 최종 손실비율 결정 (손실비율 최소값 이상 최대값 이하로 결정)
	 2) 부대의 말풍선 표시

	2. 공성병기, 함선 손실
	 1) 전투부대가 조건에 따라 조정된 손실확률 충족하고 부대병력이 기준병력 미만인 경우 공성병기 및 함선의 손실 발생
	  - 공성병기 부대는 기존의 공성병기가 검으로 변경되어 검병이 되고, 함선 보유부대는 보유하고 있던 기존의 함선이 주가로 변경됨
	  (1) 손실확률 : 손실확률 기본값에 조건별 손실확률 조정값을 모두 곱하여 최종 손실확률 결정 (손실확률 최소값 이상 최대값 이하로 결정)
	  (2) 기준병력 : 항목별로 고정 병력 적용
	 2) 효과 적용 : 화면 이동, 메시지 박스 표시

	3. 손실비율(금, 병량) 및 손실확률(공성병기, 함선) 조정조건
	 1) 계산조건 : 계산을 통해 조정값(배율)을 도출하는 조건 (각 조건의 최소값 이상 최대값 이하로 결정)
	  (1) 부대병력   : 부대의 병력에 반비례하여 조정값 도출   (병력조건 조정값 = 병력 기준값 / 부대병력)
	  (2) 주장능력치 : 주장의 능력치에 반비례하여 조정값 도출 (능력조건 조정값 = 능력 기준값 / 주장의 적용능력 능력치)
	  (3) 부대기력   : 부대의 기력에 반비례하여 조정값 도출   (기력조건 조정값 = 기력 기준값 / 부대기력)
	 2) 지정조건 : 설정된 고정 조정값(배율)을 적용하는 조건 (각 조건의 조정값을 직접 설정)
	  - 특기, 기교, 계절, 지형 항목 추가 및 삭제 가능
	  (1) 무장특기 : 부대 소속 무장이 보유한 특기에 따라 조정값 차등 적용
	  (2) 세력기교 : 연구된 세력 기교에 따라 조정값 차등 적용 (단련기교 및 정예기교는 기교별 해당병기에만 적용되며 육상부대 한정)
	  (3) 계절     : 현재 계절에 따라 조정값 차등 적용
	  (4) 지형     : 부대가 위치한 지형에 따라 조정값 차등 적용
*/

namespace 전투부대_수송량_손실
{

	///	=============================================  < 유저 설정 >  =====================================================

	/// [ 수송량 손실 : 금, 병량 ]

		/// 항목별 손실적용여부 설정
	const array<bool> 损失适用_金 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/false };
	const array<bool> 损失适用_兵粮 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/false };

	/// 손실대사 표시 설정
	// 대사 추가 및 삭제 가능 : "则府仅 遇诸 逮肱"의 형태로 추가. 항목("遇诸") 간에는 쉼표(,)로 연결
	const array<bool> 损失台词_显示 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const array<string> 损失台词_目录 =
	{
		"快谨慎保管物资",
		"抓紧了解损失的物资"
	};

	/// 항목별 손실확률 설정
<<<<<<< HEAD
	const array<int> 损失概率_金 = {/*玩家第1军团*/40, /*玩家委任军团*/40, /*AI势力*/10 };
	const array<int> 损失概率_兵粮 = {/*玩家第1军团*/40, /*玩家委任军团*/40, /*AI势力*/10 };
=======
	const array<int> 损失概率_金 = {/*玩家第1军团*/80, /*玩家委任军团*/80, /*AI势力*/10 };
	const array<int> 损失概率_兵粮 = {/*玩家第1军团*/80, /*玩家委任军团*/80, /*AI势力*/10 };
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

	/// 金兵比或粮兵比小于阈值时，不触发损失 by 江东新风
	const array<float> 损失触发阈值_金 = {/*玩家第1军团*/0.2f, /*玩家委任军团*/0.2f, /*AI势力*/0.4f };
	const array<float> 损失触发阈值_兵粮 = {/*玩家第1军团*/2.0f, /*玩家委任军团*/2.0f, /*AI势力*/4.0f };

	/// 손실비율 기본 설정
	const array<float> 损失比率_默认值 = {/*玩家第1军团*/6.0f, /*玩家委任军团*/6.0f, /*AI势力*/3.0f };
	const array<float> 损失比率_最大值 = {/*玩家第1军团*/20.00f, /*玩家委任军团*/20.00f, /*AI势力*/5.0f };
	const array<float> 损失比率_最小值 = {/*玩家第1军团*/3.0f, /*玩家委任军团*/3.0f, /*AI势力*/1.5f };

	/// 손실비율 조정조건 : 부대병력
	// 병력조건 조정값 = 병력 기준값 / 부대병력
	const array<bool> 比率调整_兵力条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const array<float> 比率调整_兵力条件_默认值 = {/*玩家第1军团*/15000.0f, /*玩家委任军团*/15000.0f, /*AI势力*/7500.f };
	const array<float> 比率调整_兵力条件_最小值 = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.5f };
	const array<float> 比率调整_兵力条件_最大值 = {/*玩家第1军团*/2.0f, /*玩家委任军团*/2.0f, /*AI势力*/1.5f };

	/// 손실비율 조정조건 : 주장능력치
	// 능력조건 조정값 = 능력 기준값 / 주장의 적용능력 능력치
	const array<bool> 比率调整_能力条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const array<float> 比率调整_能力条件_默认值 = {/*玩家第1军团*/60.0f, /*玩家委任军团*/60.0f, /*AI势力*/30.f };
	const array<float> 比率调整_能力条件_最小值 = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.5f };
	const array<float> 比率调整_能力条件_最大值 = {/*玩家第1军团*/2.0f, /*玩家委任军团*/2.0f, /*AI势力*/1.5f };
	const int 比率调整_能力条件_适用能力 = 武将能力_统率;

	/// 손실비율 조정조건 : 부대기력
	// 기력조건 조정값 = 기력 기준값 / 부대기력
	const array<bool> 比率调整_气力条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const array<float> 比率调整_气力条件_默认值 = {/*玩家第1军团*/60.0f, /*玩家委任军团*/60.0f, /*AI势力*/30.f };
	const array<float> 比率调整_气力条件_最小值 = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.5f };
	const array<float> 比率调整_气力条件_最大值 = {/*玩家第1军团*/2.0f, /*玩家委任军团*/2.0f, /*AI势力*/1.5f };

	/// 손실비율 조정조건 : 무장특기
	// 특기 추가 및 삭제 가능 : 동일한 형식으로 추가 (특기 이름은 'kr enums.cpp' 참고)
	const array<bool> 比率调整_特技条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const dictionary 比率调整值_特技 =
	{
		{特技_运输, array<float> = {/*玩家第1军团*/0.5f, /*玩家委任军团*/0.5f, /*AI势力*/0.25f} }
	};

	/// 손실비율 조정조건 : 세력기교
	// 기교 추가 및 삭제 가능 : 동일한 형식으로 추가 (기교 이름은 'kr enums.cpp' 참고)
	// 단련기교 및 정예기교는 기교별 해당병기에만 적용되며 육상부대 한정
	const array<bool> 比率调整_技巧条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const dictionary 比率调整值_技巧 =
	{
		{技巧_木牛流马, array<float> = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.4f} },
		{技巧_熟练兵,   array<float> = {/*玩家第1军团*/0.9f, /*玩家委任军团*/0.9f, /*AI势力*/0.45f} },
		{技巧_枪兵锻练, array<float> = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.4f} },
		{技巧_戟兵锻练, array<float> = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.4f} },
		{技巧_弩兵锻练, array<float> = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.4f} },
		{技巧_骑兵锻练, array<float> = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.4f} },
		{技巧_精锐枪兵, array<float> = {/*玩家第1军团*/0.6f, /*玩家委任军团*/0.6f, /*AI势力*/0.3f} },
		{技巧_精锐戟兵, array<float> = {/*玩家第1军团*/0.6f, /*玩家委任军团*/0.6f, /*AI势力*/0.3f} },
		{技巧_精锐弩兵, array<float> = {/*玩家第1军团*/0.6f, /*玩家委任军团*/0.6f, /*AI势力*/0.3f} },
		{技巧_精锐骑兵, array<float> = {/*玩家第1军团*/0.6f, /*玩家委任军团*/0.6f, /*AI势力*/0.3f} }
	};

	/// 손실비율 조정조건 : 계절
	// 계절 추가 및 삭제 가능 : 동일한 형식으로 추가 (계절 이름은 'kr enums.cpp' 참고)
	const array<bool> 比率调整_季节条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const dictionary 比率调整值_季节 =
	{
		{季节_春,   array<float> = {/*玩家第1军团*/1.0f, /*玩家委任军团*/1.0f, /*AI势力*/0.8f} },
		{季节_夏, array<float> = {/*玩家第1军团*/1.2f, /*玩家委任军团*/1.2f, /*AI势力*/1.0f} },
		{季节_秋, array<float> = {/*玩家第1军团*/1.0f, /*玩家委任军团*/1.0f, /*AI势力*/0.8f} },
		{季节_冬, array<float> = {/*玩家第1军团*/1.4f, /*玩家委任军团*/1.4f, /*AI势力*/1.2f} }
	};

	/// 손실비율 조정조건 : 지형
	// 지형 추가 및 삭제 가능 : 동일한 형식으로 추가 (지형 이름은 'kr enums.cpp' 참고)
	const array<bool> 比率调整_地形条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const dictionary 比率调整值_地形 =
	{
		{地形_砂地,   array<float> = {/*玩家第1军团*/1.1f, /*玩家委任军团*/1.1f, /*AI势力*/1.0f} },
		{地形_湿地,   array<float> = {/*玩家第1军团*/1.3f, /*玩家委任军团*/1.3f, /*AI势力*/1.1f} },
		{地形_毒泉,   array<float> = {/*玩家第1军团*/2.0f, /*玩家委任军团*/2.0f, /*AI势力*/1.4f} },
		{地形_森,     array<float> = {/*玩家第1军团*/1.1f, /*玩家委任军团*/1.1f, /*AI势力*/1.0f} },
		{地形_川,     array<float> = {/*玩家第1军团*/1.1f, /*玩家委任军团*/1.1f, /*AI势力*/1.0f} },	// 천
		{地形_海,     array<float> = {/*玩家第1军团*/1.3f, /*玩家委任军团*/1.3f, /*AI势力*/1.1f} },
		{地形_栈道,   array<float> = {/*玩家第1军团*/2.0f, /*玩家委任军团*/2.0f, /*AI势力*/1.4f} },
		{地形_崖,     array<float> = {/*玩家第1军团*/1.5f, /*玩家委任军团*/1.5f, /*AI势力*/1.2f} }	// 안
	};

	/// -------------------------------------------------------------------------------------------------------------------

	/// [ 수송량 손실 : 공성병기, 함선 ]

		/// 항목별 손실적용여부 설정
	const array<bool> 损失适用_兵器 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/false };
	const array<bool> 损失适用_舰艇 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/false };

	/// 항목별 기준병력 설정
	const array<int> 损失基准兵力_兵器 = {/*玩家第1军团*/500, /*玩家委任军团*/500, /*AI势力*/500 };
	const array<int> 损失基准兵力_舰艇 = {/*玩家第1军团*/500, /*玩家委任军团*/500, /*AI势力*/500 };

	/// 항목별 효과 적용 설정
	const array<bool> 画面移动_兵器 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/false };
	const array<bool> 画面移动_舰艇 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/false };
	const array<bool> 通知窗显示_兵器 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/false };
	const array<bool> 通知窗显示_舰艇 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/false };

	/// 손실확률 기본 설정
	const array<float> 损失概率_默认值 = {/*玩家第1军团*/6.0f, /*玩家委任军团*/6.0f, /*AI势力*/3.0f };
	const array<float> 损失概率_最大值 = {/*玩家第1军团*/20.00f, /*玩家委任军团*/20.00f, /*AI势力*/10.0f };
	const array<float> 损失概率_最小值 = {/*玩家第1军团*/3.0f, /*玩家委任军团*/3.0f, /*AI势力*/1.5f };

	/// 손실확률 조정조건 : 부대병력
	// 병력조건 조정값 = 병력 기준값 / 부대병력
	const array<bool> 调整概率_兵力条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const array<float> 调整概率_兵力条件_默认值 = {/*玩家第1军团*/15000.0f, /*玩家委任军团*/15000.0f, /*AI势力*/7500.f };
	const array<float> 调整概率_兵力条件_最小值 = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.5f };
	const array<float> 调整概率_兵力条件_最大值 = {/*玩家第1军团*/2.0f, /*玩家委任军团*/2.0f, /*AI势力*/1.5f };

	/// 손실확률 조정조건 : 주장능력치
	// 능력조건 조정값 = 능력 기준값 / 주장의 적용능력 능력치
	const array<bool> 调整概率_能力条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const array<float> 调整概率_能力条件_默认值 = {/*玩家第1军团*/60.0f, /*玩家委任军团*/60.0f, /*AI势力*/30.f };
	const array<float> 调整概率_能力条件_最小值 = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.5f };
	const array<float> 调整概率_能力条件_最大值 = {/*玩家第1军团*/2.0f, /*玩家委任军团*/2.0f, /*AI势力*/1.5f };
	const int 调整概率_能力条件_适用能力 = 武将能力_统率;

	/// 손실확률 조정조건 : 부대기력
	// 기력조건 조정값 = 기력 기준값 / 부대기력
	const array<bool> 调整概率_气力条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const array<float> 调整概率_气力条件_默认值 = {/*玩家第1军团*/60.0f, /*玩家委任军团*/60.0f, /*AI势力*/30.f };
	const array<float> 调整概率_气力条件_最小值 = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.5f };
	const array<float> 调整概率_气力条件_最大值 = {/*玩家第1军团*/2.0f, /*玩家委任军团*/2.0f, /*AI势力*/1.5f };

	/// 손실확률 조정조건 : 무장특기
	// 특기 추가 및 삭제 가능 : 동일한 형식으로 추가 (특기 이름은 'kr enums.cpp' 참고)
	const array<bool> 调整概率_特技条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const dictionary 调整概率值_特技 =
	{
		{特技_运输, array<float> = {/*玩家第1军团*/0.5f, /*玩家委任军团*/0.5f, /*AI势力*/0.25f} }
	};

	/// 손실확률 조정조건 : 세력기교
	// 기교 추가 및 삭제 가능 : 동일한 형식으로 추가 (기교 이름은 'kr enums.cpp' 참고)
	// 단련기교 및 정예기교는 기교별 해당병기에만 적용되며 육상부대 한정
	const array<bool> 调整概率_技巧条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const dictionary 调整概率值_技巧 =
	{
		{技巧_木牛流马, array<float> = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.4f} },
		{技巧_熟练兵,   array<float> = {/*玩家第1军团*/0.9f, /*玩家委任军团*/0.9f, /*AI势力*/0.45f} },
		{技巧_枪兵锻练, array<float> = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.4f} },
		{技巧_戟兵锻练, array<float> = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.4f} },
		{技巧_弩兵锻练, array<float> = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.4f} },
		{技巧_骑兵锻练, array<float> = {/*玩家第1军团*/0.8f, /*玩家委任军团*/0.8f, /*AI势力*/0.4f} },
		{技巧_精锐枪兵, array<float> = {/*玩家第1军团*/0.6f, /*玩家委任军团*/0.6f, /*AI势力*/0.3f} },
		{技巧_精锐戟兵, array<float> = {/*玩家第1军团*/0.6f, /*玩家委任军团*/0.6f, /*AI势力*/0.3f} },
		{技巧_精锐弩兵, array<float> = {/*玩家第1军团*/0.6f, /*玩家委任军团*/0.6f, /*AI势力*/0.3f} },
		{技巧_精锐骑兵, array<float> = {/*玩家第1军团*/0.6f, /*玩家委任军团*/0.6f, /*AI势力*/0.3f} }
	};

	/// 손실확률 조정조건 : 계절
	// 계절 추가 및 삭제 가능 : 동일한 형식으로 추가 (계절 이름은 'kr enums.cpp' 참고)
	const array<bool> 调整概率_季节条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const dictionary 调整概率值_季节 =
	{
		{季节_春,   array<float> = {/*玩家第1军团*/1.0f, /*玩家委任军团*/1.0f, /*AI势力*/0.8f} },
		{季节_夏, array<float> = {/*玩家第1军团*/1.2f, /*玩家委任军团*/1.2f, /*AI势力*/1.0f} },
		{季节_秋, array<float> = {/*玩家第1军团*/1.0f, /*玩家委任军团*/1.0f, /*AI势力*/0.8f} },
		{季节_冬, array<float> = {/*玩家第1军团*/1.4f, /*玩家委任军团*/1.4f, /*AI势力*/1.2f} }
	};

	/// 손실확률 조정조건 : 지형
	// 지형 추가 및 삭제 가능 : 동일한 형식으로 추가 (지형 이름은 'kr enums.cpp' 참고)
	const array<bool> 调整概率_地形条件_适用 = {/*玩家第1军团*/true, /*玩家委任军团*/true, /*AI势力*/true };
	const dictionary 调整概率值_地形 =
	{
		{地形_砂地,   array<float> = {/*玩家第1军团*/1.1f, /*玩家委任军团*/1.1f, /*AI势力*/1.0f} },
		{地形_湿地,   array<float> = {/*玩家第1军团*/1.3f, /*玩家委任军团*/1.3f, /*AI势力*/1.1f} },
		{地形_毒泉,   array<float> = {/*玩家第1军团*/2.0f, /*玩家委任军团*/2.0f, /*AI势力*/1.4f} },
		{地形_森,     array<float> = {/*玩家第1军团*/1.1f, /*玩家委任军团*/1.1f, /*AI势力*/1.0f} },
		{地形_川,     array<float> = {/*玩家第1军团*/1.1f, /*玩家委任军团*/1.1f, /*AI势力*/1.0f} },	// 천
		{地形_海,   array<float> = {/*玩家第1军团*/1.3f, /*玩家委任军团*/1.3f, /*AI势力*/1.1f} },
		{地形_栈道, array<float> = {/*玩家第1军团*/2.0f, /*玩家委任军团*/2.0f, /*AI势力*/1.4f} },
		{地形_崖,     array<float> = {/*玩家第1军团*/1.5f, /*玩家委任军团*/1.5f, /*AI势力*/1.2f} }	// 안
	};

	///	===================================================================================================================

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		pk::point curr_pos;
		pk::point unit_pos;

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			pk::bind(107, pk::trigger107_t(새날_수송량_손실));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		bool lost_supplies_gold;
		bool lost_supplies_food;
		void 새날_수송량_손실()
		{
			for (int unit_id = 0; unit_id < 部队_末; unit_id++)
			{
				pk::unit@ unit = pk::get_unit(unit_id);
				if (!pk::is_alive(unit)) continue;
				if (unit.type != 部队类型_战斗) continue;
				if (ch::has_skill(unit, 特技_严整)) continue;

				pk::force@ force = pk::get_force(unit.get_force_id());
				if (!pk::is_alive(force)) continue;
				if (!pk::is_normal_force(force)) continue;

				lost_supplies_gold = false;
				lost_supplies_food = false;
				int controller_index = get_controller_index(unit);
				float loss_ratio = get_loss_ratio(unit, force, controller_index);
				//ch::u8debug(pk::format("金损失判断前，部队{}的lost_supplies标志为gold {},food {}", unit.get_id(),lost_supplies_gold, lost_supplies_food));				
				func_金_损失(unit, loss_ratio, controller_index, lost_supplies_gold);
				//ch::u8debug(pk::format("金损失判断后，部队{}的lost_supplies标志为gold {},food {}", unit.get_id(),lost_supplies_gold, lost_supplies_food));
				func_兵粮_损失(unit, loss_ratio, controller_index, lost_supplies_food);
				//ch::u8debug(pk::format("兵粮损失判断后，部队{}的lost_supplies标志为gold {},food {}", unit.get_id(),lost_supplies_gold, lost_supplies_food));
				func_损失台词_显示(unit, lost_supplies_gold, lost_supplies_food, controller_index);

				int ground_wpn_id = pk::get_ground_weapon_id(unit);
				int sea_wpn_id = pk::get_sea_weapon_id(unit);
				float loss_chance = get_loss_chance(unit, force, controller_index);
				func_攻城兵器_损失(unit, ground_wpn_id, loss_chance, controller_index);
				func_舰艇_损失(unit, sea_wpn_id, loss_chance, controller_index);
			}
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		float get_loss_ratio(pk::unit@ unit, pk::force@ force, int controller_index)
		{
			float loss_ratio = 损失比率_默认值[controller_index];
			pk::person@ leader = pk::get_person(unit.leader);
			int leader_stat = leader.stat[比率调整_能力条件_适用能力];
			if (比率调整_兵力条件_适用[controller_index]) loss_ratio *= pk::max(比率调整_兵力条件_最小值[controller_index], pk::min(比率调整_兵力条件_最大值[controller_index], (比率调整_兵力条件_默认值[controller_index] / pk::max(1, unit.troops))));
			if (比率调整_能力条件_适用[controller_index]) loss_ratio *= pk::max(比率调整_能力条件_最小值[controller_index], pk::min(比率调整_能力条件_最大值[controller_index], (比率调整_能力条件_默认值[controller_index] / pk::max(1, leader_stat))));
			if (比率调整_气力条件_适用[controller_index]) loss_ratio *= pk::max(比率调整_气力条件_最小值[controller_index], pk::min(比率调整_气力条件_最大值[controller_index], (比率调整_气力条件_默认值[controller_index] / pk::max(1, unit.energy))));
			if (比率调整_特技条件_适用[controller_index]) loss_ratio *= get_dict_value(比率调整值_特技, unit, force, controller_index, /*특기*/0);
			if (比率调整_技巧条件_适用[controller_index]) loss_ratio *= get_dict_value(比率调整值_技巧, unit, force, controller_index, /*기교*/1);
			if (比率调整_季节条件_适用[controller_index]) loss_ratio *= get_dict_value(比率调整值_季节, unit, force, controller_index, /*계절*/2);
			if (比率调整_地形条件_适用[controller_index]) loss_ratio *= get_dict_value(比率调整值_地形, unit, force, controller_index, /*지형*/3);

			return pk::max(损失比率_最小值[controller_index], pk::min(损失比率_最大值[controller_index], loss_ratio));
		}

		float get_loss_chance(pk::unit@ unit, pk::force@ force, int controller_index)
		{
			float loss_chance = 损失概率_默认值[controller_index];
			pk::person@ leader = pk::get_person(unit.leader);
			int leader_stat = leader.stat[调整概率_能力条件_适用能力];
			if (调整概率_兵力条件_适用[controller_index]) loss_chance *= pk::max(调整概率_兵力条件_最小值[controller_index], pk::min(调整概率_兵力条件_最大值[controller_index], (调整概率_兵力条件_默认值[controller_index] / pk::max(1, unit.troops))));
			if (调整概率_能力条件_适用[controller_index]) loss_chance *= pk::max(调整概率_能力条件_最小值[controller_index], pk::min(调整概率_能力条件_最大值[controller_index], (调整概率_能力条件_默认值[controller_index] / pk::max(1, leader_stat))));
			if (调整概率_气力条件_适用[controller_index]) loss_chance *= pk::max(调整概率_气力条件_最小值[controller_index], pk::min(调整概率_气力条件_最大值[controller_index], (调整概率_气力条件_默认值[controller_index] / pk::max(1, unit.energy))));
			if (调整概率_特技条件_适用[controller_index]) loss_chance *= get_dict_value(调整概率值_特技, unit, force, controller_index, /*특기*/0);
			if (调整概率_技巧条件_适用[controller_index]) loss_chance *= get_dict_value(调整概率值_技巧, unit, force, controller_index, /*기교*/1);
			if (调整概率_季节条件_适用[controller_index]) loss_chance *= get_dict_value(调整概率值_季节, unit, force, controller_index, /*계절*/2);
			if (调整概率_地形条件_适用[controller_index]) loss_chance *= get_dict_value(调整概率值_地形, unit, force, controller_index, /*지형*/3);

			return pk::max(损失概率_最小值[controller_index], pk::min(损失概率_最大值[controller_index], loss_chance));
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
				else if (condition_index == /*지형*/3) 조건충족여부 = (pk::get_hex(unit.pos).terrain == index_value);
				if (!조건충족여부) continue;

				array<float> dict_value_arr = array<float>(dict[index_name_arr[i]]);
				float dict_value = dict_value_arr[controller_index];
				n *= dict_value;
			}

			return n;
		}

		bool check_wpn_for_tech(int weapon_id, int tech_id)
		{
<<<<<<< HEAD
			if ((tech_id == 技巧_枪兵锻练 or tech_id == 技巧_精锐枪兵) and weapon_id != 병기_창)   return false;
			if ((tech_id == 技巧_戟兵锻练 or tech_id == 技巧_精锐戟兵) and weapon_id != 병기_극)   return false;
			if ((tech_id == 技巧_弩兵锻练 or tech_id == 技巧_精锐弩兵) and weapon_id != 병기_노)   return false;
			if ((tech_id == 技巧_骑兵锻练 or tech_id == 技巧_精锐骑兵) and weapon_id != 병기_군마) return false;
=======
			if ((tech_id == 技巧_枪兵锻练 or tech_id == 技巧_精锐枪兵) and weapon_id != 兵器_枪)   return false;
			if ((tech_id == 技巧_戟兵锻练 or tech_id == 技巧_精锐戟兵) and weapon_id != 兵器_戟)   return false;
			if ((tech_id == 技巧_弩兵锻练 or tech_id == 技巧_精锐弩兵) and weapon_id != 兵器_弩)   return false;
			if ((tech_id == 技巧_骑兵锻练 or tech_id == 技巧_精锐骑兵) and weapon_id != 兵器_战马) return false;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

			return true;
		}

		void func_金_损失(pk::unit@ unit, float loss_ratio, int controller_index, bool /*&out*/ lost_supplies)
		{
			lost_supplies_gold = false;
			//ch::u8debug(pk::format("部队{}兵力为{}，金为{}，比例为{}", unit.get_id(), unit.troops, unit.gold, unit.troops / unit.gold));
			if ((unit.gold / unit.troops) <= 损失触发阈值_金[controller_index])
			{
				//ch::u8debug(pk::format("部队{}兵力为{}，金为{}，比例为{}，不触发损失",unit.get_id(), unit.troops, unit.gold, unit.troops / unit.gold));				
				return;
			}
			if (!损失适用_金[controller_index]) return;
			if (!pk::rand_bool(损失概率_金[controller_index])) return;

			lost_supplies_gold = true;
			pk::add_gold(unit, -int(unit.gold * loss_ratio / 100.f), true);
		}

		void func_兵粮_损失(pk::unit@ unit, float loss_ratio, int controller_index, bool /*&out*/ lost_supplies)
		{
			lost_supplies_food = false;
			if ((unit.food / unit.troops) <= 损失触发阈值_兵粮[controller_index])
			{
				return;
			}
			if (!损失适用_兵粮[controller_index]) return;
			if (!pk::rand_bool(损失概率_兵粮[controller_index])) return;

			lost_supplies_food = true;
			pk::add_food(unit, -int(unit.food * loss_ratio / 100.f), true);
		}

		void func_损失台词_显示(pk::unit@ unit, bool lost_supplies1, bool lost_supplies2, int controller_index)
		{
			if (!lost_supplies1 and !lost_supplies2) return;
			if (!损失台词_显示[controller_index]) return;
			if (!pk::is_in_screen(unit.pos)) return;

			pk::person@ leader = pk::get_person(unit.leader);
			string dialogue = 损失台词_目录[pk::rand(int(损失台词_目录.length))];
			pk::say(pk::encode(dialogue), leader);
		}

		void func_攻城兵器_损失(pk::unit@ unit, int ground_wpn_id, float loss_chance, int controller_index)
		{
			if (!损失适用_兵器[controller_index]) return;
			if (pk::equipment_id_to_heishu(ground_wpn_id) != 兵种_兵器) return;
			if (int(unit.troops) >= 损失基准兵力_兵器[controller_index]) return;
			if (!pk::rand_bool(int(loss_chance))) return;

			curr_pos = pk::get_screen_center_hex_pos();
<<<<<<< HEAD
			func_武器_变更(unit, ground_wpn_id, 병기_검, /*검 수량*/1, /*육상 병기*/0);
=======
			func_武器_变更(unit, ground_wpn_id, 兵器_枪, /*검 수량*/1, /*육상 병기*/0);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			if (画面移动_兵器[controller_index]) func_화면_이동(unit);
			if (通知窗显示_兵器[controller_index]) func_메시지박스_표시(unit, ground_wpn_id, "\x1b[2x陆地\x1b[0x", "\x1b[1x剑\x1b[0x");
			pk::scene(cast<pk::scene_t@>(function() { pk::move_screen(main.curr_pos, 500); }));
		}

		void func_舰艇_损失(pk::unit@ unit, int sea_wpn_id, float loss_chance, int controller_index)
		{
			if (!损失适用_舰艇[controller_index]) return;
			if (sea_wpn_id == 兵器_走舸) return;
			if (int(unit.troops) >= 损失基准兵力_舰艇[controller_index]) return;
			if (!pk::rand_bool(int(loss_chance))) return;

			curr_pos = pk::get_screen_center_hex_pos();
			func_武器_变更(unit, sea_wpn_id, 兵器_走舸, /*주가 수량*/0, /*해상 병기*/1);
			if (画面移动_舰艇[controller_index]) func_화면_이동(unit);
			if (通知窗显示_舰艇[controller_index]) func_메시지박스_표시(unit, sea_wpn_id, "\x1b[2x水上\x1b[0x", "\x1b[1x走舸\x1b[0x");
			pk::scene(cast<pk::scene_t@>(function() { pk::move_screen(main.curr_pos, 500); }));
		}

		void func_武器_变更(pk::unit@ unit, int wpn_before, int wpn_after, int wpn_after_amount, int wpn_type_index)
		{
			/// 변경된 스크립트 적용 뒤 기존의 세이브파일로 테스트 시 강제종료 현상이 발생하기도 함
			/// 화면 안에 무기를 변경하는 부대가 존재하는 경우 이미지는 갱신이 안됨 (기존병기 이미지가 남아 있어 겹쳐 보임)
			if (pk::is_in_screen(unit.pos)) pk::scene(cast<pk::scene_t@>(function() { pk::point point(199, 199); pk::move_screen(point, 1); }));
			unit.stock[wpn_type_index].id = wpn_after;
			unit.stock[wpn_type_index].amount = wpn_after_amount;
			// pk::set_weapon_amount(unit, wpn_type_index, wpn_before, 0);
			// pk::set_weapon_amount(unit, wpn_type_index, wpn_after, wpn_after_amount);
			unit.update();
			pk::scene(cast<pk::scene_t@>(function() { pk::move_screen(main.curr_pos, 1); }));
		}

		void func_화면_이동(pk::unit@ unit)
		{
			unit_pos = unit.pos;
			pk::scene(cast<pk::scene_t@>(function() { pk::move_screen(main.unit_pos, 500); }));
			pk::play_se(3);
		}

		void func_메시지박스_표시(pk::unit@ unit, int wpn_before, string wpn_type_name, string wpn_after_name)
		{
			string unit_name = "\x1b[2x" + pk::decode(pk::get_name(unit)) + "\x1b[0x";
			string wpn_before_name = "\x1b[1x" + pk::decode(pk::get_name(pk::get_equipment(wpn_before))) + "\x1b[0x";
			string loss_msg = unit_name + "的兵器 " + wpn_before_name + " \x1b[16x损失\x1b[0x发生";
			string change_info = "(" + wpn_type_name + "兵器" + wpn_before_name + "在" + wpn_after_name + "以发生\x1b[16x变更\x1b[0x)";
			pk::message_box(pk::encode(loss_msg + "\n" + change_info));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		int get_controller_index(pk::unit@ unit)
		{
			return (pk::is_player_controlled(unit) ? /*玩家第1军团*/0 : (unit.is_player() ? /*玩家委任军团*/1 : /*AI势力*/2));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	}

	Main main;
}
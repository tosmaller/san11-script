// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/12/26 # 江东新风 # 关掉trace ##
// ## 2020/12/12 # 江东新风 # 修正Signed/Unsigned mismatch ##
// ## 2020/12/01 # 江东新风 # 配合异族交涉，去掉消灭id不符的建筑的防bug函数，可能会导致未知bug ##
// ## 2020/11/18 # 江东新风 # 不出贼bug修复##
// ## 2020/09/30 # 江东新风 # 出贼文本变量汉化##
// ## 2020/09/23 # 江东新风 # 根城势力和部队武将不一致的问题##
// ## 2020/08/19 # 江东新风 # 原版特技has_skill设定 ##
// ## 2020/08/05 # 氕氘氚 # 可能修复了贼据点与森林模型重合問題 ##
// ## 2020/07/21 # 氕氘氚 # 整合合了原版出贼和加强版出贼  ##
/*


	@제작 : masterpiecek


	[ 根据地_발생 패치 안내 ]

	 ※ 난이도 상승을 위한 기존의 본거지 발생 이벤트 보완 및 신규 이벤트 추가
	  - 패치 적용 시 ‘158 월별 본거지 발생 확률.cpp’ 파일이 적용되지 않습니다.

	1. 基本事件1 : 기존의 본거지 발생 이벤트를 강화 (발생 조건 및 부대 능력 등 강화)
	 * 이벤트 대상 도시가 玩家 세력, AI 세력인 경우에 대해 각각 설정 가능
	 1) 세력 도시별로 매 턴마다 발생 조건을 충족하는 경우,
	  - 이벤트 적용 여부, 이벤트 발생 시기, 시나리오 경과 월수, 세력 도시 수, 세력 도시 치안, 이벤트 발생 확률 조건을 충족
		(세력 도시의 소속 무장이 이벤트 관련 특기를 보유한 경우 이벤트 발생 조건 중 도시 치안의 기준값이 감소)
	  - 도시 구역 내 또는 기존 본거지 근처에 이미 출현한 기존의 贼将 势力部队가 존재하지 않을 것
	 2) 해당 세력 도시 구역 내에 贼将 세력 본거지가 발생
	  - 신규 본거지가 생성된 경우 해당 본거지에서, 신규 본거지가 생성되지 않은 경우 기존의 본거지들 중 한 곳에서 부대가 출현하며,
		나머지 본거지에 대해서는 본거지별로 일정 확률 충족 시 추가로 부대가 출현
	 3) 贼将 세력 본거지에서 해당 贼将 势力部队 출현
	  - 신규 본거지와 기존의 본거지들 중 한 곳에서 부대가 출현하거나, 일정 확률 충족 시 도시 구역 내 모든 본거지에서 부대가 출현

	2. 基本事件2 : 기존의 본거지가 존재함에도 해당 본거지의 처리를 미루는 것을 방지 (신규 이벤트)
	 * 이벤트 대상 도시가 玩家 세력, AI 세력인 경우에 대해 각각 설정 가능
	 1) 세력 도시별로 매 턴마다 발생 조건을 충족하는 경우,
	  - 이벤트 적용 여부, 이벤트 발생 시기, 시나리오 경과 월수, 세력 도시 수 조건을 충족
	  - 도시 구역 내에 이미 발생한 기존의 본거지가 존재할 것
	  - 도시 구역 내 또는 기존 본거지 근처에 이미 출현한 기존의 贼将 势力部队가 존재하지 않을 것
	 2) 해당 도시 구역 내의 贼将 세력 본거지에서 해당 贼将 势力部队 출현
	  - 기존의 본거지들 중 한 곳에서 부대가 출현하며, 나머지 본거지에 대해서는 본거지별로 일정 확률 충족 시 추가로 부대가 출현

	3. 额外事件 1 : 空城 점령 후 본거지를 제거해야 할 부담을 주어 空城의 무혈 입성을 방지 (신규 이벤트)
	 1) 새 날마다 空城에서 발생 조건을 충족하는 경우,
	  - 이벤트 적용 여부, 이벤트 발생 시기, 이벤트 발생 확률 조건을 충족할 것
	 2) 空城 구역 내에 贼将 세력의 본거지가 발생
	  - 해당 空城를 점령 후에도 본거지는 잔류

	4. 额外事件 2 : 본거지 처리를 위해 극소수의 兵力이 출진하는 것을 방지 (신규 이벤트)
	 * 이벤트 대상 도시가 玩家 세력, AI 세력, 空城인 경우에 대해 각각 설정 가능
	 1) 새 날마다 각 도시별로 발생 조건을 충족하는 경우,
	  - 이벤트 적용 여부, 이벤트 발생 시기, 이벤트 발생 확률 조건을 충족할 것
	 2) 해당 도시 구역 내에 존재하는 모든 贼将 세력 본거지의 내구를 일정량 회복


	[ 참고 사항 ]

	1. 이벤트에서 출현하는 贼将 세력 일람
	 1)  盗贼  세력 : 盗贼
	 2) 이민족 세력 : 羌族, 乌丸族, 山越族, 南蛮族

	2. 贼将 세력별 출현 지역
	 1)  盗贼  : 이민족 세력 출현 지역을 제외한 전체 도시
	 2)  羌族  : 무위, 안정, 천수 (량주 지역)
	 3) 乌丸族 : 양평, 북평,  계  (유주 지역)
	 4) 山越族 : 회계, 시상,  오  (양주 지역)
	 5) 南蛮族 : 운남, 건녕, 강주 (익주 지역)

	3. 贼将 세력별 무장 일람
	 1)  盗贼  :   贼将  (头目),   盗贼  (部将)
	 2)  羌族  :  강头目 (头目),  羌武将 (部将)
	 3) 乌丸族 : 오환头目(头目), 乌丸武将(部将)
	 4) 山越族 : 산월头目(头目), 山越武将(部将)
	 5) 南蛮族 : 남만头目(头目), 南蛮武将(部将)
*/

namespace 根据地_发生
{

	///	=============================================  < 유저 설정 >  =====================================================

	const bool 贼兵加强 = false;

	const int 出贼治安 = 80;
	const bool 电脑不出贼 = false;

	/// [ 基本事件相关设定 ]
	//	1. 基本事件1 : 新根据地生成并生成敌军部队
	//	2. 基本事件2 : 在已有根据地生成敌军部队

		/// 事件启用与否设定 : true = 启用事件, false = 关闭事件
	const array<array<bool>> 基本事件_发生条件_事件_是否启用 =
	{
		/**基本事件1*/ {/*玩家势力城市*/true, /*AI势力城市*/true},
		/**基本事件2*/ {/*玩家势力城市*/true, /*AI势力城市*/true}
	};

	/// 设定现有部队确认条件:使用(true)时在市区内或在原有根据地附近已出现的原有的敌军部队存在时不发生任何事件 기존부대확인 조건 설정 : 기능 적용(true) 시 도시구역 내 또는 기존본거지 근처에 이미 출현한 기존의 贼将势力部队가 존재하는 경우 이벤트가 발생하지 않음
	const array<array<bool>> 基本事件_发生条件_现存部队确认 =
	{
		/**基本事件1*/ {/*玩家势力城市*/true, /*AI势力城市*/true},
		/**基本事件2*/ {/*玩家势力城市*/true, /*AI势力城市*/true}
	};

	/// 发生事件条件模式设定 : 启动时必须符合当前时机设定的启动时间模式
	// 事件模式值 : 0(每旬), 1(每月), 2(每季), 3(每年)
	const array<array<int>> 基本事件_发生条件_发生时间模式 =
	{
		/**基本事件1*/ {/*玩家势力城市*/1, /*AI势力城市*/1},
		/**基本事件2*/ {/*玩家势力城市*/1, /*AI势力城市*/2}
	};

	/// 经过月数条件设定 : 事件发生时，剧本的经过的月数需超过预设值
	const array<array<int>> 基本事件_发生条件_经过月数 =
	{
		/**基本事件1*/ {/*玩家势力城市*/4, /*AI势力城市*/8},
		/**基本事件2*/ {/*玩家势力城市*/4, /*AI势力城市*/8}
	};

	/// 势力城市数量条件设定: 势力城市数量必须超过设定值后才发生事件
	const array<array<int>> 基本事件_发生条件_势力城市数 =
	{
		/**基本事件1*/ {/*玩家势力城市*/1, /*AI势力城市*/3},
		/**基本事件2*/ {/*玩家势力城市*/1, /*AI势力城市*/3}
	};

	/// 设定城市治安条件:势力城市的城市治安低于设置值时发生事件 (基本事件1)
	const array<array<int>> 基本事件1_发生条件_城市治安 =
	{
		/** 盗贼  出现地区*/ {/*玩家势力城市*/85, /*AI势力城市*/80},
		/** 羌族  出现地区*/ {/*玩家势力城市*/90, /*AI势力城市*/85},
		/**乌丸族 出现地区*/ {/*玩家势力城市*/90, /*AI势力城市*/85},
		/**山越族 出现地区*/ {/*玩家势力城市*/90, /*AI势力城市*/85},
		/**南蛮族 出现地区*/ {/*玩家势力城市*/90, /*AI势力城市*/85}
	};

	/// 发生概率条件概率倍率设定 : 只有满足势力城市设定的事件发生概率值，事件才能发生(基本事件1)
	// 发生概率 = (100 - 城市治安) * 发生概率倍率
	const array<array<float>> 基本事件1_发生条件_发生概率倍率 =
	{
		/** 盗贼  出现地区*/ {/*玩家势力城市*/2.0f, /*AI势力城市*/1.0f},
		/** 羌族  出现地区*/ {/*玩家势力城市*/2.0f, /*AI势力城市*/1.0f},
		/**乌丸族 出现地区*/ {/*玩家势力城市*/2.0f, /*AI势力城市*/1.0f},
		/**山越族 出现地区*/ {/*玩家势力城市*/2.0f, /*AI势力城市*/1.0f},
		/**南蛮族 出现地区*/ {/*玩家势力城市*/2.0f, /*AI势力城市*/1.0f}
	};

	/// 事件相关的全体特技设置:拥有全体特技在整个城市事件发生条件中减少需求的城市基准治安(基本事件1)
	// 특기의 이름 확인은 'script' 폴더의 ‘kr enums.cpp’ 파일 참고 (특기 없음은 '-1'로 설정)
	const int 基本事件1_相关特技_基准治安减少_全体特技_减少特技 = 特技_威压;
	const array<array<int>> 基本事件1_相关特技_基准治安减少_全体特技_减少量 =
	{
		/** 盗贼  出现地区*/ {/*玩家势力城市*/10, /*AI势力城市*/15},
		/** 羌族  出现地区*/ {/*玩家势力城市*/10, /*AI势力城市*/15},
		/**乌丸族 出现地区*/ {/*玩家势力城市*/10, /*AI势力城市*/15},
		/**山越族 出现地区*/ {/*玩家势力城市*/10, /*AI势力城市*/15},
		/**南蛮族 出现地区*/ {/*玩家势力城市*/10, /*AI势力城市*/15}
	};

	/// 事件相关的异族特技设置:异族势力出现符合的城市拥有异族特技时事件发生条件中减少需求的城市基准治安(基本事件1)
	// 특기의 이름 확인은 'script' 폴더의 ‘kr enums.cpp’ 파일 참고 (특기 없음은 '-1'로 설정)
	const array<int> 基本事件1_相关特技_基准治安减少_异族特技_减少特技 =
	{
		/** 羌族  出现地区*/特技_亲羌,
		/**乌丸族 出现地区*/特技_亲乌,
		/**山越族 出现地区*/特技_亲越,
		/**南蛮族 出现地区*/特技_亲蛮
	};
	const array<array<int>> 基本事件1_相关特技_基准治安减少_异族特技_减少量 =
	{
		/** 羌族  出现地区*/ {/*玩家势力城市*/5, /*AI势力城市*/10},
		/**乌丸族 出现地区*/ {/*玩家势力城市*/5, /*AI势力城市*/10},
		/**山越族 出现地区*/ {/*玩家势力城市*/5, /*AI势力城市*/10},
		/**南蛮族 出现地区*/ {/*玩家势力城市*/5, /*AI势力城市*/10}
	};

	/// 追加功能 : 根据地追加出现设定 (基本事件1)
	// 只要启用(true)时，满足设定的概率值，即使现有根据地存在，也允许新建根据地기능 적용(true) 시 설정된 확률값을 충족하면 기존본거지가 존재해도 신규본거지 생성을 허용
	// (原版为没有根据地时才建立新的根据地)
	const array<bool> 基本事件1_根据地_协同出现_启用 = {/*玩家势力城市*/true, /*AI势力城市*/true };
	const array<int>  基本事件1_根据地_协同出现_概率 = {/*玩家势力城市*/10, /*AI势力城市*/5 };

	/// 追加功能 : 敌军部队协同出现设定
	// 启用时在一处根据地出现敌军部队时，范围内其他根据地将以一定概率也派出敌军部队协同作战
	// (原版为只在一处根据地创建敌军部队后便结束事件)
	const array<array<bool>> 基本事件_敌军部队_协同出现_启用 =
	{
		/**基本事件1*/ {/*玩家势力城市*/true, /*AI势力城市*/true},
		/**基本事件2*/ {/*玩家势力城市*/true, /*AI势力城市*/true}
	};
	const array<array<int>> 基本事件_敌军部队_协同出现_概率 =
	{
		/**基本事件1*/ {/*玩家势力城市*/10, /*AI势力城市*/5},
		/**基本事件2*/ {/*玩家势力城市*/10, /*AI势力城市*/5}
	};

	/// -------------------------------------------------------------------------------------------------------------------

	/// [ 额外事件 1 相关设定 ]
	//	- 空城生成新根据地

		/// 事件是否启用
		// true = 启用事件, false = 关闭事件
	const bool 额外事件1_发生条件_事件_是否启用 = true;

	/// 发生事件条件模式设定:当新一天(1,11,21日)到来时，必须符合当前事件发生时间模式
	// 事件模式值 : 0(每旬), 1(每月), 2(每季), 3(每年)
	const int 额外事件1_发生条件_发生时间模式 = 1;

	/// 发生率条件设定:只有满足设置的发生率值后，事件才会发生
	const int 额外事件1_发生条件_发生概率 = 50;

	/// -------------------------------------------------------------------------------------------------------------------

	/// [ 额外事件 2 相关设定 ]
	//	- 原有根据地耐久恢复

		/// 事件是否启用 : true = 启用事件, false = 关闭事件
	const array<bool> 额外事件2_发生条件_事件_是否启用 = {/*玩家势力城市*/true, /*AI势力城市*/true, /*空城*/true };

	/// 发生事件条件模式设定:当新一天(1,11,21日)到来时，必须符合当前事件发生时间模式
	// 事件模式值 : 0(每旬), 1(每月), 2(每季), 3(每年)
	const array<int> 额外事件2_发生条件_发生时间模式 = {/*玩家势力城市*/0, /*AI势力城市*/1, /*空城*/1 };

	/// 发生率条件设定:只有满足设置的发生率值后，事件才会发生
	const array<int> 额外事件2_发生条件_发生概率 = {/*玩家势力城市*/50, /*AI势力城市*/25, /*空城*/25 };

	/// 根据地耐久恢复量设定:城市区域内根据地耐久恢复量与设定值相等
	const array<int> 额外事件2_根据地내구회복_회복량 = {/*玩家势力城市*/50, /*AI势力城市*/25, /*空城*/25 };

	/// -------------------------------------------------------------------------------------------------------------------

	/// [ 敌势力武将 相关设定 ]

	/*
		[ 范围随机值 ]

		 1) 排列形式 : {第一个(最小值), 第二个(最大值)}

		 2) 返回值   : 返回最小值以上最大值以下
		  - 原本内容 : {75, 85} - 返回75以上85以下的值
		  - 修改举例 : {80, 90} - 返回80以上90以下的值

		 3 )排列必须有两个项目组成
	*/

	/// 敌势力武将能力值: 统率、武力、智力设定 (范围随机值)
	// 预设修改方法 : 范围随机值参考说明
	const array<array<array<int>>> 무장통솔 =
	{
		/**头目*/ {/*贼将*/{75, 85}, /*羌头目*/{80, 90}, /*乌丸头目*/{80, 90}, /*山越头目*/{80, 90}, /*南蛮头目*/{80, 90}},
		/**部将*/ {/*盗贼*/{65, 75}, /*羌武将*/{70, 80}, /*乌丸武将*/{70, 80}, /*山越武将*/{70, 80}, /*南蛮武将*/{70, 80}}
	};
	const array<array<array<int>>> 무장무력 =
	{
		/**头目*/ {/*贼将*/{75, 85}, /*羌头目*/{80, 90}, /*乌丸头目*/{80, 90}, /*山越头目*/{80, 90}, /*南蛮头目*/{80, 90}},
		/**部将*/ {/*盗贼*/{65, 75}, /*羌武将*/{70, 80}, /*乌丸武将*/{70, 80}, /*山越武将*/{70, 80}, /*南蛮武将*/{70, 80}}
	};
	const array<array<array<int>>> 무장지력 =
	{
		/**头目*/ {/*贼将*/{60, 70}, /*羌头目*/{60, 70}, /*乌丸头目*/{60, 70}, /*山越头目*/{60, 70}, /*南蛮头目*/{60, 70}},
		/**部将*/ {/*盗贼*/{50, 60}, /*羌武将*/{50, 60}, /*乌丸武将*/{50, 60}, /*山越武将*/{50, 60}, /*南蛮武将*/{50, 60}}
	};

	/// 敌势力武将适性设定 (范围随机值)
	// 预设修改方法 : 范围随机值参考说明
	// 适性名: 适性_C(0),  适性_B(1),  适性_A(2),  适性_S(3), 适性_S1(4), 适性_S2(5), 适性_S3(6), 适性_S4(7), 适性_S5(8), 适性_S6(9), 适性_S7(10), 适性_S8(11), 适性_S9(12), 适性_SS(13)
	// 适性设置为s以上的请直接用数字
	const array<array<array<int>>> 武将适性 =
	{
		/**头目*/ {/*贼将*/{适性_A, 适性_S}, /*羌头目*/{适性_A, 适性_S}, /*乌丸头目*/{适性_A, 适性_S}, /*山越头目*/{适性_A, 适性_S}, /*南蛮头目*/{适性_A, 适性_S}},
		/**部将*/ {/*盗贼*/{适性_B, 适性_A}, /*羌武将*/{适性_B, 适性_A}, /*乌丸武将*/{适性_B, 适性_A}, /*山越武将*/{适性_B, 适性_A}, /*南蛮武将*/{适性_B, 适性_A}}
	};

	/// 敌势力武将特技设定
	// 特技名请参考script文件夹下的300.1 kr enums-UTF-8.cpp
	const array<array<int>> 武将特技 =
	{
		/**头目*/ {/*贼将*/特技_威压, /*羌头目*/特技_亲羌, /*乌丸头目*/特技_亲乌, /*山越头目*/特技_亲越, /*南蛮头目*/特技_亲蛮},
		/**部将*/ {/*盗贼*/-1, /*羌武将*/-1, /*乌丸武将*/-1, /*山越武将*/-1, /*南蛮武将*/-1}
	};

	/// -------------------------------------------------------------------------------------------------------------------

	/// [ 贼将势力部队 相关设定 ]

		/// 贼将势力部队生成数
	const array<array<array<int>>> 部队数 =
	{ {
			/**玩家 势力城市*/ {/*贼将*/1, /*羌头目*/1, /*乌丸头目*/1, /*山越头目*/1, /*南蛮头目*/1},
			/**玩家 势力城市*/ {/*盗贼*/4, /*羌武将*/4, /*乌丸武将*/4, /*山越武将*/4, /*南蛮武将*/4}
		},{
			/** AI  势力城市*/ {/*贼将*/1, /*羌头目*/1, /*乌丸头目*/1, /*山越头目*/1, /*南蛮头目*/1},
			/** AI  势力城市*/ {/*盗贼*/2, /*羌武将*/2, /*乌丸武将*/2, /*山越武将*/2, /*南蛮武将*/2}
		} };

	/*
		[ 项目随机值 ]

		 1) 排列形式 : {항목1, 항목2, 항목3, ... , 마지막 항목}

		 2) 返回值   : 1개 이상의 배열 전체 항목 중 하나의 항목을 무작위로 결정하여 해당 항목 값을 반환
		  - 原本内容 : {兵器_枪, 兵器_戟} - '창', '극' 두 개의 무기 중 하나를 무작위로 결정
		  - 修改举例 : {兵器_枪, 兵器_弩} - '창', '노' 두 개의 무기 중 하나를 무작위로 결정

		 3) 배열에 항목 추가 및 제거 가능
		  - 原本内容 : {兵器_枪, 兵器_戟}          - '창', '극' 두 개의 무기 중 하나를 무작위로 결정
		  - 修改举例 : {兵器_枪, 兵器_戟, 兵器_弩} - '창', '극', '노' 세 개의 무기 중 하나를 무작위로 결정

		 4) 배열의 항목이 하나인 경우 해당 항목의 값이 그대로 반환됨
		  - 原本内容 : {兵器_枪, 兵器_戟} - '창', '극' 두 개의 무기 중 하나를 무작위로 결정
		  - 修改举例 : {兵器_枪}          - '창'으로 결정

		 5) 특정 항목 값의 반환 확률을 높이려면 같은 항목을 여러개 추가하는 것도 가능
		  - 原本内容 : {兵器_枪, 兵器_戟}          - 1/2의 확률로 '창', 1/2의 확률로 '극'으로 결정
		  - 修改举例 : {兵器_枪, 兵器_枪, 兵器_戟} - 2/3의 확률로 '창', 1/3의 확률로 '극'으로 결정

		 6) 항목 간에는 반드시 ‘쉼표( , )’로 연결되어야 하며, 마지막 항목의 뒤에는 쉼표가 없어야 함
	*/

	/// 贼将势力部队兵器 : 陆上和水上兵器设定 (项目随机值)
	// 预设修改方法 : 项目随机值参考说明
	// 陆上兵器名 : 兵器_剑(0), 兵器_枪(1), 兵器_戟(2), 兵器_弩(3), 兵器_战马(4), 兵器_冲车(5), 兵器_井阑(6), 兵器_投石(7), 兵器_木兽(8)
	// 水上兵器名 : 兵器_走舸(9), 兵器_楼船(10), 兵器_斗舰(11)
	const array<array<array<int>>> 部队陆上兵器 =
	{ {
			/*  贼将  */ {兵器_枪, 兵器_戟},
			/* 羌头目 */ {兵器_战马},
			/*乌丸头目*/ {兵器_战马},
			/*山越头目*/ {兵器_枪, 兵器_戟},
			/*南蛮头目*/ {兵器_枪, 兵器_戟},
		},{
			/*  盗贼  */ {兵器_枪, 兵器_戟},
			/* 羌武将 */ {兵器_战马},
			/*乌丸武将*/ {兵器_战马},
			/*山越武将*/ {兵器_枪, 兵器_戟},
			/*南蛮武将*/ {兵器_枪, 兵器_戟}
		} };
	const array<array<array<int>>> 部队水上兵器 =
	{ {
			/*  贼将  */ {兵器_楼船, 兵器_斗舰},
			/* 羌头目 */ {兵器_楼船, 兵器_斗舰},
			/*乌丸头目*/ {兵器_楼船, 兵器_斗舰},
			/*山越头目*/ {兵器_楼船, 兵器_斗舰},
			/*南蛮头目*/ {兵器_楼船, 兵器_斗舰}
		},{
			/*  盗贼  */ {兵器_走舸, 兵器_楼船},
			/* 羌武将 */ {兵器_走舸, 兵器_楼船},
			/*乌丸武将*/ {兵器_走舸, 兵器_楼船},
			/*山越武将*/ {兵器_走舸, 兵器_楼船},
			/*南蛮武将*/ {兵器_走舸, 兵器_楼船}
		} };

	/// 贼将势力部队兵力设定 (项目随机值)
	// 预设修改方法 : 项目随机值参考说明
	const array<array<array<int>>> 部队兵力 =
	{ {
			/*  贼将  */ {6000, 7000, 8000},
			/* 羌头目 */ {6000, 7000, 8000},
			/*乌丸头目*/ {6000, 7000, 8000},
			/*山越头目*/ {6000, 7000, 8000},
			/*南蛮头目*/ {6000, 7000, 8000}
		},{
			/*  盗贼  */ {4000, 5000, 6000},
			/* 羌武将 */ {4000, 5000, 6000},
			/*乌丸武将*/ {4000, 5000, 6000},
			/*山越武将*/ {4000, 5000, 6000},
			/*南蛮武将*/ {4000, 5000, 6000}
		} };

	/// 贼将势力部队金设定
	const array<array<int>> 부대금 =
	{
		/**头目*/ {/*贼将*/400, /*羌头目*/400, /*乌丸头目*/400, /*山越头目*/400, /*南蛮头目*/400},
		/**部将*/ {/*盗贼*/400, /*羌武将*/400, /*乌丸武将*/400, /*山越武将*/400, /*南蛮武将*/400}
	};

	/// 贼将势力部队兵粮 : 部队兵力倍率
	// 兵粮 = 兵力 * 部队兵粮倍率
	const array<array<float>> 部队兵粮倍率 =
	{
		/**头目*/ {/*贼将*/1.5f, /*羌头目*/1.5f, /*乌丸头目*/1.5f, /*山越头目*/1.5f, /*南蛮头目*/1.5f},
		/**部将*/ {/*盗贼*/1.5f, /*羌武将*/1.5f, /*乌丸武将*/1.5f, /*山越武将*/1.5f, /*南蛮武将*/1.5f}
	};

	/// 贼将势力部队气力设定
	const array<array<int>> 部队气力 =
	{
		/**头目*/ {/*贼将*/100, /*羌头目*/100, /*乌丸头目*/100, /*山越头目*/100, /*南蛮头目*/100},
		/**部将*/ {/*盗贼*/100, /*羌武将*/100, /*乌丸武将*/100, /*山越武将*/100, /*南蛮武将*/100}
	};

	/// -------------------------------------------------------------------------------------------------------------------

	/// [ 其他设定 ]

		/// 根据地消息显示设定
		// 세력의 턴 시작 시 본거지 및 贼将부대 생성 이벤트가 발생하지 않은 도시 중 본거지가 존재하는 도시가 있는 경우 안내(경고) 메시지 표시
	const bool 根据地_介绍消息_显示 = true;

	/// 敌军各势力 出现城市 : 羌族, 乌丸族, 山越族, 南蛮族 出现城市设定
	// 预设修改方法 : 원하는 도시를 '城市_이름'의 형태로 항목을 추가하거나 원하지 않는 도시 항목을 삭제
	//                   (항목 간에는 반드시 쉼표(',')로 연결되어야 하며 마지막 항목 뒤에는 쉼표가 없어야 함)
	// 城市名请参考script文件夹下的300.1 kr enums-UTF-8.cpp
	// 盗贼 세력 출현 도시 : 이민족 세력 출현 지역을 제외한 전체 도시
	// 蛮族出现都市
	const array<array<int>> 敌军出现城市 =
	{
		/*羌族  */ {城市_武威, 城市_安定, 城市_天水},
		/*乌丸族*/ {城市_襄平, 城市_北平, 城市_蓟},
		/*山越族*/ {城市_会稽, 城市_柴桑, 城市_吴},
		/*南蛮族*/ {城市_云南, 城市_建宁, 城市_江州}
	};

	/// 본거지생성 거리 : 贼将势力 본거지 생성 시 出现地区 도시로부터의 최소 및 최대거리 설정
	//  根据地与都市距离范围
	const array<int> 根据地生成_距离 = { 6, 15 };	// {最小, 最大}

	/// 본거지생성 禁止地形 : 贼将势力 본거지 생성 시 생성이 불가능한 지형 설정
	// 预设修改方法 : 원하는 지형을 '地形_이름'의 형태로 항목을 추가하거나 원하지 않는 지형 항목을 삭제
	//                   (항목 간에는 반드시 쉼표(',')로 연결되어야 하며 마지막 항목 뒤에는 쉼표가 없어야 함)
	// 地形名请参考script文件夹下的300.1 kr enums-UTF-8.cpp
	//	根据地不可能出现的地形:		//		毒泉		川		江		海			官道		栈道		岸		山（崖）	小徑
	const array<int> 根据地生成_禁止地形 = { 地形_毒泉, 地形_川, 地形_河, 地形_海, 地形_官道, 地形_栈道, 地形_岸, 地形_崖, 地形_间道, 地形_森 };

	///	===================================================================================================================

	///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

			// bandit_force_id		: 势力_盗贼, 势力_羌, 势力_山越, 势力_乌丸, 势力_盗贼
			// bandit_facility_id	: 设施_根据地1, 设施_根据地2
			// event_index			: 0 基本事件1, 1 基本事件2 / 0 额外事件1, 1 额外事件2
			// controller_index		: 0 玩家, 1 AI
			// bandit_force_index	: 0 盗贼, 1 羌族, 2 乌丸族, 3 山越族, 4 南蛮族
			// bandit_person_index	: 0 头目, 1 部将
			// messagae_type_index	: 0 메시지 박스, 1 로그 메시지
			// history_log_index	: 0 본거지 출현, 1 贼将 부대 출현

	array<bool> 基本事件1_根据地_是否生成(城市_末, false);
	array<bool> 基本事件1_敌军部队_是否生成(城市_末, false);
	array<bool> 基本事件2_敌军部队_是否生成(城市_末, false);
	pk::point pos_for_screen_move;

	/// 이벤트 관련 적장세력 정보 (수정금지)
	array<int> 異族势力 = { 势力_盗贼, 势力_羌, 势力_乌丸, 势力_山越, 势力_南蛮 };//異族势力 42羌,43山越,44乌丸,45南蛮,46盗贼
	array<array<int>> 적장무장 =
	{
		/*头目*/ {武将_贼将, 武将_羌头目, 武将_乌丸头目, 武将_山越头目, 武将_南蛮头目},
		/*部将*/ {武将_盗贼, 武将_羌武将, 武将_乌丸武将, 武将_山越武将, 武将_南蛮武将}
	};
	array<int> 敌军设施 = { 设施_根据地2, 设施_根据地1, 设施_根据地1, 设施_根据地1, 设施_根据地1 };

	///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




	///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			//	檢?是否滿足事件发生时机条件
	bool func_事件_时间条件_是否满足(int mode_index)
	{
		array<bool> 발생시기_모드별_충족여부 = { true, is_first_day_of_month(), is_first_day_of_quarter(), is_first_day_of_year() };

		return 발생시기_모드별_충족여부[mode_index];
	}
	//	檢?势力都市属和游?时間是否滿足
	bool func_基本事件_势力条件_是否满足(pk::force@ force, int event_index, int controller_index)
	{
		if (pk::get_elapsed_months() < 基本事件_发生条件_经过月数[event_index][controller_index]) return false;
		if (pk::get_city_count(force) < 基本事件_发生条件_势力城市数[event_index][controller_index]) return false;

		return true;
	}

	bool func_基本事件1_城市条件_是否满足(pk::city@ city, pk::building@ building, int event_index, int controller_index, int bandit_force_index)
	{
		if (event_index == /*基本事件2*/1) return true;

		int 발생조건_城市治安 = 基本事件1_发生条件_城市治安[bandit_force_index][controller_index];
		if (ch::has_skill(building, 基本事件1_相关特技_基准治安减少_全体特技_减少特技))
			발생조건_城市治安 -= 基本事件1_相关特技_基准治安减少_全体特技_减少量[bandit_force_index][controller_index];
		if (bandit_force_index != /*盗贼*/0)
		{
			if (ch::has_skill(building, 基本事件1_相关特技_基准治安减少_异族特技_减少特技[bandit_force_index - 1]))
				발생조건_城市治安 -= 基本事件1_相关特技_基准治安减少_异族特技_减少量[bandit_force_index - 1][controller_index];
		}
		if (int(city.public_order) >= 발생조건_城市治安) return false;

		int 발생확률 = pk::min(100, int((100 - city.public_order) * 基本事件1_发生条件_发生概率倍率[bandit_force_index][controller_index]));
		if (!pk::rand_bool(발생확률)) return false;

		return true;
	}
	//	执行：基本事件
	void func_基本事件_实行(pk::force@ force, pk::city@ city, pk::building@ building, int bandit_force_id, int bandit_facility_id, int event_index, int controller_index, int bandit_force_index)
	{
		array<pk::building@> old_bandit_base_arr = func_城市区域内_特定势力设施_排列(city.get_id(), bandit_force_id, bandit_facility_id);

		if (基本事件_发生条件_现存部队确认[event_index][controller_index])
		{
			if (func_기존적장부대_是否存在(city, old_bandit_base_arr, bandit_force_id, controller_index)) return;
		}

		if (func_基本事件1_根据地생성조건_충족여부(old_bandit_base_arr, event_index, controller_index))
		{
			func_事件_消息框_显示(force, city, event_index, bandit_force_index);
			pk::building@ new_bandit_base = func_基本事件1_贼将势力根据地_生成(force, city, building, bandit_force_id, bandit_facility_id, bandit_force_index);
			if (pk::is_alive(new_bandit_base)) old_bandit_base_arr.insertAt(0, new_bandit_base);

			基本事件1_根据地_是否生成[city.get_id()] = true;
		}

		if (int(old_bandit_base_arr.length) == 0) return;

		if (!基本事件_敌军部队_协同出现_启用[event_index][controller_index])
		{
			int arr_index = pk::rand(int(old_bandit_base_arr.length));
			if (event_index == /*基本事件1*/0 and int(old_bandit_base_arr.length) > 1) arr_index = 0;
			old_bandit_base_arr = { old_bandit_base_arr[arr_index] };
		}

		for (int arr_index = 0; arr_index < int(old_bandit_base_arr.length); arr_index++)
		{
			if (!func_贼将_空城_是否存在()) break;
			if (func_贼将势力_全部部队数_合计() >= 100) break;
			if (基本事件_敌军部队_协同出现_启用[event_index][controller_index] and !pk::rand_bool(基本事件_敌军部队_协同出现_概率[event_index][controller_index]))
			{
				if (event_index == /*基本事件1*/0 and 基本事件1_敌军部队_是否生成[city.get_id()]) continue;
				else if (event_index == /*基本事件1*/1 and 基本事件2_敌军部队_是否生成[city.get_id()]) continue;
			}

			pk::building@ bandit_base = old_bandit_base_arr[arr_index];
			if (!func_特定坐标周围_오브젝트생성_가능여부(bandit_base.pos, /*distance_min*/1, /*distance_max*/1)) continue;

			if (event_index == /*基本事件1*/0)
			{
				if (基本事件1_根据地_是否生成[city.get_id()] and !基本事件1_敌军部队_是否生成[city.get_id()])
					func_事件_log信息_显示(city, bandit_force_id, bandit_base.pos, bandit_force_index, /*history_log_index*/1);

				基本事件1_敌军部队_是否生成[city.get_id()] = true;
			}
			else if (event_index == /*基本事件2*/1)
			{
				if (!基本事件2_敌军部队_是否生成[city.get_id()])
				{
					func_事件_消息框_显示(force, city, event_index, bandit_force_index);
					func_事件_log信息_显示(city, bandit_force_id, bandit_base.pos, bandit_force_index, /*history_log_index*/1);
				}

				基本事件2_敌军部队_是否生成[city.get_id()] = true;
			}

			func_基本事件_画面移动(force, bandit_base.pos);
			func_基本事件_贼将势力部队_生成(city, building, bandit_base, bandit_force_id, controller_index, bandit_force_index, /*bandit_person_index*/0);
			func_基本事件_贼将势力部队_生成(city, building, bandit_base, bandit_force_id, controller_index, bandit_force_index, /*bandit_person_index*/1);
		}
	}

	bool func_기존적장부대_是否存在(pk::city@ city, array<pk::building@> old_bandit_base_arr, int bandit_force_id, int controller_index)
	{
		if (func_城市区域内_特定势力部队_是否存在(city.get_id(), bandit_force_id)) return true;
		else if (int(old_bandit_base_arr.length) == 0) return false;

		for (int arr_index = 0; arr_index < int(old_bandit_base_arr.length); arr_index++)
		{
			pk::building@ old_bandit_base = old_bandit_base_arr[arr_index];
			if (func_特定坐标周围_特定势力部队_是否存在(old_bandit_base.pos, bandit_force_id, /*distance_min*/1, /*distance_max*/5)) return true;
		}

		return false;
	}

	bool func_基本事件1_根据地생성조건_충족여부(array<pk::building@> old_bandit_base_arr, int event_index, int controller_index)
	{
		if (event_index != /*基本事件1*/0) return false;
		if (int(old_bandit_base_arr.length) == 0) return true;
		if (基本事件1_根据地_协同出现_启用[controller_index] and pk::rand_bool(基本事件1_根据地_协同出现_概率[controller_index])) return true;

		return false;
	}

	pk::building@ func_基本事件1_贼将势力根据地_生成(pk::force@ force, pk::city@ city, pk::building@ building, int bandit_force_id, int bandit_facility_id, int bandit_force_index)
	{
		array<pk::point> new_bandit_base_pos_arr = func_根据地_生成가능좌표_排列(city, building, 根据地生成_距离);
		if (int(new_bandit_base_pos_arr.length) == 0) return null;

		pk::point new_bandit_base_pos = new_bandit_base_pos_arr[pk::rand(int(new_bandit_base_pos_arr.length))];
		if (!pk::is_valid_pos(new_bandit_base_pos)) return null;

		func_基本事件_画面移动(force, new_bandit_base_pos);
		func_特定坐标_效果执行(/*sound_index*/3, /*effect_index*/0x5c, new_bandit_base_pos);

		pk::building@ new_bandit_base = func_特定势力设施_建筑生成(new_bandit_base_pos, bandit_force_id, bandit_facility_id);
		if (!pk::is_alive(new_bandit_base)) return null;

		func_事件_log信息_显示(city, bandit_force_id, new_bandit_base_pos, bandit_force_index, /*history_log_index*/0);

		return new_bandit_base;
	}

	array<pk::point> func_根据地_生成가능좌표_排列(pk::city@ city, pk::building@ building, array<int> distance_arr)
	{
		array<pk::point> candidate_bandit_base_pos_arr(0);

		array<pk::point> range_pos_arr = pk::range(building.pos, /*distance_min*/distance_arr[0], /*distance_max*/distance_arr[1]);
		for (int arr_index = int(range_pos_arr.length) - 1; arr_index >= 0; arr_index--)
		{
			pk::point pos = range_pos_arr[arr_index];
			if (!pk::is_valid_pos(pos)) continue;
			if (pk::get_city_id(pos) != city.get_id()) continue;
			if (!func_特定坐标周围_오브젝트생성_가능여부(pos, /*distance_min*/1,/*distance_max*/1)) continue;

			pk::hex@ hex = pk::get_hex(pos);
			if (!func_특정칸_군사시설건설_가능여부(hex)) continue;

			int terrain_id = hex.terrain;
			if (!pk::is_valid_terrain_id(terrain_id)) continue;
			if (!pk::is_enabled_terrain(terrain_id)) continue;
			if (根据地生成_禁止地形.find(terrain_id) >= 0) continue;

			candidate_bandit_base_pos_arr.insertLast(pos);
		}

		return candidate_bandit_base_pos_arr;
	}

	void func_事件_log信息_显示(pk::city@ city, int bandit_force_id, pk::point event_pos, int bandit_force_index, int history_log_index)
	{
		string str_event_object = (history_log_index == 0) ? "据点" : (history_log_index == 1 ? "部队" : "");
		string str_event_msg = get_city_name(city) + "周边出现了" + get_bandit_force_name_for_log(bandit_force_index) + str_event_object + "!";
		pk::history_log(event_pos, pk::get_force(bandit_force_id).color, pk::encode(str_event_msg));
	}

	void func_事件_消息框_显示(pk::force@ force, pk::city@ city, int event_index, int bandit_force_index)
	{
		if (!force.is_player()) return;
		if (event_index == 1 and (!func_贼将_空城_是否存在() or func_贼将势力_全部部队数_合计() >= 100)) return;

		array<string> str_event_info_arr =
		{
			get_bandit_force_name_for_box(bandit_force_index) + "向" + get_city_name(city) + "觊觎进攻的情报",
			get_city_name(city) + "附近的据点获取的\n" + get_bandit_force_name_for_box(bandit_force_index) + "出现的谍报"
		};
		string str_event_msg = get_kunshu_name(force) + "大人," + str_event_info_arr[pk::rand(int(str_event_info_arr.length))] + "送来了!";
		pk::message_box(pk::encode(str_event_msg), get_message_reporter(force));
	}

	void func_基本事件_画面移动(pk::force@ force, pk::point event_pos)
	{
		if (!force.is_player()) return;

		pos_for_screen_move = event_pos;
		pk::scene(cast<pk::scene_t@>(function() { pk::move_screen(/*main.*/pos_for_screen_move, 500); }));	// 작은 숫자일수록 빠른 이동
	}

	//基本事件_敌军部队_生成
	void func_基本事件_贼将势力部队_生成(pk::city@ city, pk::building@ building, pk::building@ bandit_base, int bandit_force_id, int controller_index, int bandit_force_index, int bandit_person_index)
	{
		for (int count = 0; count < 部队数[controller_index][bandit_person_index][bandit_force_index]; count++)
		{
			if (!func_贼将_空城_是否存在()) break;//沒有空余武将位则不生成
			if (func_贼将势力_全部部队数_合计() >= 100) break;//異族势力部队总数大於100，不生成
			if (!pk::is_normal_force(city.get_force_id())) { func_빈도시구역내_敌军部队제거(city, bandit_force_id); break; }//空城则?除部队，不生成
			if (!func_特定坐标周围_오브젝트생성_가능여부(bandit_base.pos, /*distance_min*/1, /*distance_max*/1)) break;

			pk::person@ bandit_person = func_贼将_武将_生成(bandit_force_id, bandit_force_index, bandit_person_index);
			if (!pk::is_alive(bandit_person)) { pk::reset(bandit_person); continue; }

			pk::unit@ bandit_unit = func_贼将_部队_生成(bandit_base, bandit_person, bandit_force_index, bandit_person_index);
			if (!pk::is_alive(bandit_unit)) { pk::kill(bandit_unit); pk::reset(bandit_person); continue; }
			if (int(pk::get_movable_pos(bandit_unit).length) == 0) { pk::kill(bandit_unit); pk::reset(bandit_person); break; }
			//	if (!func_부대위치좌표_이동가능여부(bandit_unit)) {pk::kill(bandit_unit); pk::reset(bandit_person); continue;}

			int bandit_unit_order = func_敌军部队_최적임무_결정(city, bandit_unit);
			pk::set_order(bandit_unit, bandit_unit_order, building.pos);
			if (!pk::run_order(bandit_unit)) { pk::kill(bandit_unit); pk::reset(bandit_person); continue; }
		}
	}

	//	除去空城范围?的贼军部队
	void func_빈도시구역내_敌军部队제거(pk::city@ city, int bandit_force_id)
	{
		if (pk::is_normal_force(city.get_force_id())) return;	//不是空城，返回

		array<pk::unit@> bandit_unit_arr = pk::list_to_array(pk::get_unit_list(pk::get_force(bandit_force_id)));
		for (int arr_index = 0; arr_index < int(bandit_unit_arr.length); arr_index++)	//遍历該城贼部队，?刪除
		{
			pk::unit@ bandit_unit = bandit_unit_arr[arr_index];
			if (city.get_id() == pk::get_city_id(bandit_unit.pos)) pk::kill(bandit_unit);
		}
	}

	//敌军武将生成
	pk::person@ func_贼将_武将_生成(int bandit_force_id, int bandit_force_index, int bandit_person_index)
	{
		pk::person@ bandit_person = pk::create_bandit(pk::get_person(적장무장[bandit_person_index][bandit_force_index]));

		pk::set_district(bandit_person, bandit_force_id);
		bandit_person.base_stat[武将能力_统率] = func_排列_범위_랜덤값(무장통솔[bandit_person_index][bandit_force_index]);
		bandit_person.base_stat[武将能力_武力] = func_排列_범위_랜덤값(무장무력[bandit_person_index][bandit_force_index]);
		bandit_person.base_stat[武将能力_智力] = func_排列_범위_랜덤값(무장지력[bandit_person_index][bandit_force_index]);
		for (int heishu_id = 兵种_枪兵; heishu_id < 兵种_末; heishu_id++)
			bandit_person.tekisei[heishu_id] = func_排列_범위_랜덤값(武将适性[bandit_person_index][bandit_force_index]);
		bandit_person.skill = 武将特技[bandit_person_index][bandit_force_index];
		bandit_person.update();

		return bandit_person;
	}

	//敌军部队生成
	pk::unit@ func_贼将_部队_生成(pk::building@ bandit_base, pk::person@ bandit_person, int bandit_force_index, int bandit_person_index)
	{
		int ground_wpn = func_排列_항목_랜덤값(部队陆上兵器[bandit_person_index][bandit_force_index]);//陆上兵器
		int sea_wpn = func_排列_항목_랜덤값(部队水上兵器[bandit_person_index][bandit_force_index]);//水上兵器
		int troops = func_排列_항목_랜덤값(部队兵力[bandit_person_index][bandit_force_index]);//兵力
		int gold = 부대금[bandit_person_index][bandit_force_index];//金
		int food = int(troops * 部队兵粮倍率[bandit_person_index][bandit_force_index]);//兵粮
		pk::point emptyPos = getEmptyPos(bandit_base);
		//pk::trace(pk::format("建筑id：{}，武将id：{}，兵力：{}，武器：{}，{}", bandit_base.get_id(), bandit_person.get_id(),troops, ground_wpn, sea_wpn));
		//pk::trace(pk::format("金：{}，粮：{}，坐标x,y：{}，{}", gold, food, emptyPos.x,  emptyPos.y));
		pk::unit@ bandit_unit = pk::create_unit(bandit_base, bandit_person, null, null, troops, ground_wpn, sea_wpn, gold, food, emptyPos);

		bandit_unit.energy = 部队气力[bandit_person_index][bandit_force_index];

		return bandit_unit;
	}

	int func_敌军部队_최적임무_결정(pk::city@ city, pk::unit@ bandit_unit)
	{
		array<pk::point> bandit_unit_movable_pos_arr = pk::get_movable_pos(bandit_unit);
		for (int arr_index = 0; arr_index < int(bandit_unit_movable_pos_arr.length); arr_index++)
		{
			pk::point pos = bandit_unit_movable_pos_arr[arr_index];
			if (!pk::is_valid_pos(pos)) continue;

			for (int dir_index = 0; dir_index < 方向_末; dir_index++)
			{
				pk::point neighbor_pos = pk::get_neighbor_pos(pos, dir_index);
				if (!pk::is_valid_pos(neighbor_pos)) continue;

				pk::unit@ unit = pk::get_unit(neighbor_pos);
				if (pk::is_alive(unit) and pk::is_enemy(bandit_unit, unit) and city.get_force_id() != unit.get_force_id())
					return 部队任务_攻击;

				pk::building@ building = pk::get_building(neighbor_pos);
				if (pk::is_alive(building) and pk::is_enemy(bandit_unit, building) and city.get_force_id() != building.get_force_id())
					return 部队任务_攻击;
			}
		}

		return 부대임무_정복;
	}

	//遍历所有武将，身??死亡或无的都可用来生成
	bool func_贼将_空城_是否存在()
	{
		for (int person_id = 敌将_开始; person_id < 敌将_末; person_id++)
		{
			pk::person@ person = pk::get_person(person_id);
			if (person.mibun == 신분_없음) return true;
			if (person.mibun == 신분_사망) return true;
		}

		return false;
	}

	//異族势力_部队总数_统计
	int func_贼将势力_全部部队数_合计()
	{
		int sum_of_bandit_unit = 0;

		for (int bandit_force_index = 0; bandit_force_index < 5; bandit_force_index++)
		{
			int bandit_force_id = 異族势力[bandit_force_index];
			sum_of_bandit_unit += func_特定势力_部队数_合计(bandit_force_id);
		}

		return sum_of_bandit_unit;
	}

	void func_根据地_介绍消息_显示(pk::force@ force)
	{
		if (!force.is_player()) return;
		if (!根据地_介绍消息_显示) return;

		array<string> str_bandit_base_info_arr(0);
		for (int city_id = 0; city_id < 城市_末; city_id++)
		{
			pk::city@ city = pk::get_city(city_id);
			if (!pk::is_alive(city)) continue;
			if (force.get_id() != city.get_force_id()) continue;
			if (基本事件1_根据地_是否生成[city_id]) continue;
			if (基本事件1_敌军部队_是否生成[city_id]) continue;
			if (基本事件2_敌军部队_是否生成[city_id]) continue;

			int bandit_force_id = get_bandit_force_id_of_city(city_id);
			int bandit_force_index = get_bandit_force_index_by_id(bandit_force_id);
			int bandit_facility_id = 敌军设施[bandit_force_index];
			if (!func_城市区域内_特定势力设施_是否存在(city.get_id(), bandit_force_id, bandit_facility_id)) continue;

			string str_bandit_base_info = get_city_name(city) + "的" + get_bandit_force_name_for_box(bandit_force_index) + "据点";
			str_bandit_base_info_arr.insertLast(str_bandit_base_info);
		}

		if (int(str_bandit_base_info_arr.length) == 0) return;

		string str_all_bandit_base_info = join(str_bandit_base_info_arr, "\x1b[0x,\x1b[0x");
		string str_info_msg = get_kunshu_name(force) + "大人," + str_all_bandit_base_info + "让当地百姓惶恐不安.";
		pk::message_box(pk::encode(str_info_msg), get_message_reporter(force));
	}

	///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			//	获取当前势力是玩家or电脑
	int get_controller_index_of_force(pk::force@ force)
	{
		return pk::is_valid_force_id(force.get_id()) ? (force.is_player() ? 0 : 1) : 2;
	}
	int get_controller_index_of_city(pk::city@ city)
	{
		return pk::is_valid_force_id(city.get_force_id()) ? (city.is_player() ? 0 : 1) : 2;
	}

	int get_bandit_force_index_of_city(int city_id)
	{
		for (int bandit_force_index = 1; bandit_force_index < 5; bandit_force_index++)
		{
			if (敌军出现城市[bandit_force_index - 1].find(city_id) >= 0) return bandit_force_index;
		}

		return 0;
	}

	//	获取当前据点所属贼种类
	int get_bandit_force_id_of_city(int city_id)
	{
		for (int bandit_force_index = 1; bandit_force_index < 5; bandit_force_index++)
		{
			if (敌军出现城市[bandit_force_index - 1].find(city_id) >= 0) return 異族势力[bandit_force_index];
		}

		return 势力_盗贼;
	}
	//获取贼种类对应編号：（array 異族势力的下标）
	int get_bandit_force_index_by_id(int bandit_force_id)
	{
		return 異族势力.find(bandit_force_id);
	}

	int get_bandit_facility_id_by_id(int bandit_force_id)
	{
		return 敌军设施[get_bandit_force_index_by_id(bandit_force_id)];
	}

	string get_kunshu_name(pk::force@ force)
	{
		pk::person@ kunshu = pk::get_person(force.kunshu);
		return "\x1b[1x" + pk::decode(pk::get_name(kunshu)) + "\x1b[0x";
	}
	string get_city_name(pk::city@ city)
	{
		return "\x1b[2x" + pk::decode(city.name) + "\x1b[0x";
	}

	string get_bandit_force_name_for_log(int bandit_force_index)
	{
		array<string> 적장세력이름_로그 = { "贼军", "羌军", "乌丸军", "山越军", "南蛮军" };
		return "\x1b[2x" + 적장세력이름_로그[bandit_force_index] + "\x1b[0x";
	}
	string get_bandit_force_name_for_box(int bandit_force_index)
	{
		array<string> 적장세력이름_박스 = { "贼军", "羌军", "乌丸军", "山越军", "南蛮军" };
		return "\x1b[29x" + 적장세력이름_박스[bandit_force_index] + "\x1b[0x";
	}

	pk::person@ get_message_reporter(pk::force@ force)
	{
		pk::person@ gunshi = pk::get_person(force.gunshi);
		return pk::is_alive(gunshi) ? gunshi : pk::get_person(武将_文官);
	}

	///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

			/// 确认在该城市的区域内是否存在某种势力的部队。
	bool func_城市区域内_特定势力部队_是否存在(int city_id, int force_id)
	{
		for (int unit_id = 0; unit_id < 部队_末; unit_id++)
		{
			pk::unit@ unit = pk::get_unit(unit_id);
			if (!pk::is_alive(unit)) continue;
			if (unit.get_force_id() != force_id) continue;
			if (pk::get_city_id(unit.pos) != city_id) continue;

			return true;
		}

		return false;
	}

	/// 특정 도시 구역 내에 존재하는 특정 세력의 특정 시설에 해당하는 건물 배열 반환
	array<pk::building@> func_城市区域内_特定势力设施_排列(int city_id, int force_id, int facility_id)
	{
		array<pk::building@> building_arr(0);
		for (int building_id = 0; building_id < 建筑_末; building_id++)
		{
			pk::building@ building = pk::get_building(building_id);
			if (!pk::is_alive(building)) continue;
			if (building.get_force_id() != force_id) continue;
			if (building.facility != facility_id) continue;
			if (pk::get_city_id(building.pos) != city_id) continue;

			building_arr.insertLast(building);
		}

		return building_arr;
	}

	/// 특정 도시 구역 내에 특정 세력의 특정 시설 건물이 존재하는지 확인
	bool func_城市区域内_特定势力设施_是否存在(int city_id, int force_id, int facility_id)
	{
		for (int building_id = 0; building_id < 建筑_末; building_id++)
		{
			pk::building@ building = pk::get_building(building_id);
			if (!pk::is_alive(building)) continue;
			if (building.get_force_id() != force_id) continue;
			if (building.facility != facility_id) continue;
			if (pk::get_city_id(building.pos) != city_id) continue;

			return true;
		}

		return false;
	}

	/// 确认在一定距离内的坐标中是否存在可創建对象的坐标 특정 좌표로부터 일정 거리 이내의 좌표 중 오브젝트 생성이 가능한 좌표가 존재하는지 확인
	bool func_特定坐标周围_오브젝트생성_가능여부(pk::point pos, int distance_min, int distance_max)
	{
		array<pk::point> range_pos_arr = pk::range(pos, distance_min, distance_max);
		for (int arr_index = 0; arr_index < int(range_pos_arr.length); arr_index++)
		{
			pk::point range_pos = range_pos_arr[arr_index];
			if (!pk::is_valid_pos(range_pos)) continue;

			pk::hex@ hex = pk::get_hex(range_pos);
			if (hex.has_building) continue;
			if (hex.has_unit) continue;

			int terrain_id = hex.terrain;
			if (!pk::is_valid_terrain_id(terrain_id)) continue;
			if (!pk::is_enabled_terrain(terrain_id)) continue;

			return true;
		}

		return false;
	}

	/// 특정 좌표로부터 일정 거리 이내의 좌표에 특정 세력의 부대가 존재하는지 확인
	bool func_特定坐标周围_特定势力部队_是否存在(pk::point pos, int force_id, int distance_min, int distance_max)
	{
		array<pk::point> range_pos_arr = pk::range(pos, distance_min, distance_max);
		for (int arr_index = 0; arr_index < int(range_pos_arr.length); arr_index++)
		{
			pk::unit@ unit = pk::get_unit(range_pos_arr[arr_index]);
			if (!pk::is_alive(unit)) continue;
			if (unit.get_force_id() != force_id) continue;

			return true;
		}

		return false;
	}
	bool func_特定坐标周围_特定势力部队_是否存在(pk::point pos, pk::force@ force, int distance_min, int distance_max) { return func_特定坐标周围_特定势力部队_是否存在(pos, force.get_id(), distance_min, distance_max); }

	/// 在特定坐标上生成特定势力的特定设施建筑物并完成该建筑物。
	pk::building@ func_特定势力设施_建筑生成(pk::point pos, int force_id, int facility_id)
	{
		pk::building@ building = pk::create_building(pk::get_facility(facility_id), pos, force_id);
		pk::complete(building);
		pk::set_hp(building, pk::get_max_hp(building));
		building.update();

		return building;
	}

	/// 특정 좌표에서 특정 효과 및 특정 효과음 실행
	void func_特定坐标_效果执行(int sound_index, int effect_index, pk::point pos)
	{
		if (!pk::is_in_screen(pos)) return;

		pk::play_se(sound_index);
		pk::create_effect(effect_index, pos);
	}

	/// 특정 세력의 전체 부대 수 반환
	int func_特定势力_部队数_合计(int force_id)
	{
		pk::force@ force = pk::get_force(force_id);
		return int(pk::get_unit_list(force).count);
	}

	/// 특정 칸이 군사시설의 건설이 가능한지 확인
	bool func_특정칸_군사시설건설_가능여부(pk::hex@ hex)
	{
		return (!hex.has_building and !hex.has_unit and hex.can_military and !hex.development);
	}

	/// 부대가 위치한 좌표의 지형이 해당 부대가 이동 가능한 지형인지 확인
	bool func_부대위치좌표_이동가능여부(pk::unit@ unit)
	{
		int terrain_id = pk::get_hex(unit.pos).terrain;
		return cast<pk::func169_t>(pk::get_func(169))(unit, terrain_id);
	}

	/// 范围随机值 : int형 변수 배열(항목 2개)에서 최소값(항목 첫번째 값) 이상 최대값(항목 두번째 값) 이하의 int형 랜덤값을 반환
	int func_排列_범위_랜덤값(array<int> int_value_arr)
	{
		pk::random random(pk::rand());
		return random(int_value_arr[0], int_value_arr[1]);
	}

	/// 项目随机值 : 변수 배열에서 항목 중 하나를 랜덤으로 하나의 값을 선택하여 해당 항목의 변수 값을 반환
	int func_排列_항목_랜덤값(array<int> int_value_arr)
	{
		return int_value_arr[pk::rand(int(int_value_arr.length))];
	}
	float func_排列_항목_랜덤값(array<float> float_value_arr)
	{
		return float_value_arr[pk::rand(int(float_value_arr.length))];
	}

	/// 현재 날짜가 한 달의 초일인지 확인
	bool is_first_day_of_month()
	{
		return (pk::get_day() == 1);
	}

	/// 현재 날짜가 한 분기의 초일인지 확인
	bool is_first_day_of_quarter()
	{
		return (pk::get_day() == 1 and pk::is_first_month_of_quarter());
	}

	/// 현재 날짜가 한 해의 초일인지 확인
	bool is_first_day_of_year()
	{
		return (pk::get_day() == 1 and pk::get_month() == 1);
	}

	/// 현재 게임이 튜토리얼, 결전제패, 미션 중인지 확인
	bool is_normal_game()
	{
		return (!pk::is_campaign() and !pk::is_mission_running());
	}

	pk::point getEmptyPos(pk::building@ building)
	{
		auto arr = pk::range(building.get_pos(), 1, 3);
		for (int i = 0; i < int(arr.length); i++)
		{
			pk::hex@ hex = pk::get_hex(arr[i]);
			int terrain_id = hex.terrain;

			if (!hex.has_building and !hex.has_unit
				and pk::is_valid_terrain_id(terrain_id)
				and pk::is_enabled_terrain(terrain_id)
				and terrain_id != 지형_산 and terrain_id != 地形_岸 and terrain_id != 地形_河 and terrain_id != 地形_川 and terrain_id != 地形_海)
				return arr[i];

		}
		return -1;
	}
	///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	class Main
	{

		Main()
		{
			if (贼兵加强)
			{
				pk::reset_func(158);
				pk::set_func(158, pk::func158_t(func158));

				pk::bind(111, pk::trigger111_t(旬开始_基本事件));
				pk::bind(107, pk::trigger107_t(新天_额外事件1));
				pk::bind(107, pk::trigger107_t(新天_额外事件2));

				pk::bind(111, pk::trigger111_t(旬开始_防止bug));
			}
			else
			{
				pk::set_func(158, pk::func158_t(callback));
			}

		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
				//	原版贼系统
		int callback(pk::city@ city)
		{
			// 게임 시작 후 6개월까지 발생하지 않음.  前6回合不出贼
			if (pk::get_elapsed_months() <= 6)
				return 0;

			// 电脑不出贼  2020/7/17 氕氘氚
			if (!city.is_player() && 电脑不出贼)
				return 0;

			// 중복 발생 금지  不重複出贼
			int city_id = city.get_id();
			// 이 콜백은 도시 별로 호출되기 때문에 맵 상에 건물이 많을 수록 리스트로 접근하면 속도가 느려지므로 배열로 변환.
			array<pk::building@> arr = pk::list_to_array(pk::get_building_list());
			for (int i = 0; i < int(arr.length); i++)
			{
				pk::building@ building = arr[i];
				if (pk::get_city_id(building.get_pos()) == city_id and (building.facility == 设施_根据地1 or building.facility == 设施_根据地2))
					return 0;
			}

			// 치안이 기준치 이상일 경우 발생하지 않음
			if (int(city.public_order) >= 出贼治安 - (ch::has_skill(pk::city_to_building(city), 特技_威压) ? 20 : 0))
				return 0;

			// 기준치 미만일 경우 확률 계산
			return (100 - city.public_order) / 3;
		}

		// -------------------------------------------------------------------------------------------------------------------
		//	强化版贼系统
		int func158(pk::city@ city) { return 0; }

		void 旬开始_基本事件(pk::force@ force)
		{
			//pk::trace(pk::format("start 158,time:{}", pk::time()));
			for (int i = 0; i < 城市_末; i++) 基本事件1_根据地_是否生成[i] = 基本事件1_敌军部队_是否生成[i] = 基本事件2_敌军部队_是否生成[i] = false;

			if (!is_normal_game()) return;
			if (!pk::is_normal_force(force)) return;

			for (int event_index = 0; event_index <= 1; event_index++)	//两种事件
			{
				int controller_index = get_controller_index_of_force(force);	//玩家or电脑
				if (!基本事件_发生条件_事件_是否启用[event_index][controller_index]) return;	//事件关閉 退出
				if (!func_事件_时间条件_是否满足(基本事件_发生条件_发生时间模式[event_index][controller_index])) return;	//不在发动时期
				if (!func_基本事件_势力条件_是否满足(force, event_index, controller_index)) return;	//游?时間未滿or城市数未滿
				//pk::trace(pk::format("{}通过基础判定", 0));
				for (int city_id = 0; city_id < 城市_末; city_id++)	//遍历城市
				{
					//pk::trace(pk::format("城市id：{}，开始判定", city_id));
					pk::city@ city = pk::get_city(city_id);
					if (!pk::is_alive(city)) continue;
					if (force.get_id() != city.get_force_id()) continue;
					if (event_index == /*基本事件2*/1 and (基本事件1_根据地_是否生成[city_id] or 基本事件1_敌军部队_是否生成[city_id])) continue;	//事件已经发生/已经存在贼？？
					//pk::trace(pk::format("城市id：{}，通过前3个判定", city_id));
					pk::building@ building = pk::city_to_building(city);
					int bandit_force_index = get_bandit_force_index_of_city(city_id);
					if (!func_基本事件1_城市条件_是否满足(city, building, event_index, controller_index, bandit_force_index)) continue;	//事件发生条件是否滿足
					//pk::trace(pk::format("城市id：{}，判定城市条件", city_id));
					int bandit_force_id = get_bandit_force_id_of_city(city.get_id());
					int bandit_facility_id = 敌军设施[bandit_force_index];
					func_基本事件_实行(force, city, building, bandit_force_id, bandit_facility_id, event_index, controller_index, bandit_force_index);
					//pk::trace(pk::format("城市id：{}，通过判定，执行基本事件", city_id));
				}
			}

			func_根据地_介绍消息_显示(force);
			//pk::trace(pk::format("finished 158,time:{}", pk::time()));
		}

		void 新天_额外事件1()
		{
			if (!is_normal_game()) return;
			if (!额外事件1_发生条件_事件_是否启用) return;
			if (!func_事件_时间条件_是否满足(额外事件1_发生条件_发生时间模式)) return;

			for (int city_id = 0; city_id < 城市_末; city_id++)
			{
				pk::city@ city = pk::get_city(city_id);
				if (pk::is_normal_force(city.get_force_id())) continue;
				if (!pk::rand_bool(额外事件1_发生条件_发生概率)) continue;

				int bandit_force_id = get_bandit_force_id_of_city(city_id);
				int bandit_force_index = get_bandit_force_index_of_city(city_id);
				int bandit_facility_id = 敌军设施[bandit_force_index];
				if (func_城市区域内_特定势力设施_是否存在(city.get_id(), bandit_force_id, bandit_facility_id)) continue;

				pk::building@ building = pk::city_to_building(city);
				array<pk::point> new_bandit_base_pos_arr = func_根据地_生成가능좌표_排列(city, building, 根据地生成_距离);
				if (int(new_bandit_base_pos_arr.length) == 0) continue;

				pk::point new_bandit_base_pos = new_bandit_base_pos_arr[pk::rand(int(new_bandit_base_pos_arr.length))];
				if (!pk::is_valid_pos(new_bandit_base_pos)) continue;

				pk::building@ new_bandit_base = func_特定势力设施_建筑生成(new_bandit_base_pos, bandit_force_id, bandit_facility_id);
				if (!pk::is_alive(new_bandit_base)) continue;

				func_事件_log信息_显示(city, bandit_force_id, new_bandit_base.pos, bandit_force_index, /*history_log_index*/0);
			}
		}

		void 新天_额外事件2()
		{
			if (!is_normal_game()) return;

			for (int building_id = 0; building_id < 建筑_末; building_id++)
			{
				pk::building@ building = pk::get_building(building_id);
				if (!pk::is_alive(building)) continue;
				if (敌军设施.find(building.facility) < 0) continue;

				pk::city@ city = pk::get_city(pk::get_city_id(building.pos));
				int controller_index = get_controller_index_of_city(city);
				if (!额外事件2_发生条件_事件_是否启用[controller_index]) continue;
				if (!func_事件_时间条件_是否满足(额外事件2_发生条件_发生时间模式[controller_index])) continue;
				if (!pk::rand_bool(额外事件2_发生条件_发生概率[controller_index])) continue;

				pk::add_hp(building, 额外事件2_根据地내구회복_회복량[controller_index], true);
			}
		}

		//	回合开始时防bug函数
		void 旬开始_防止bug(pk::force@ force)
		{
			if (!is_normal_game()) return;

			for (int building_id = 0; building_id < 建筑_末; building_id++)	//遍?建筑
			{
				pk::building@ building = pk::get_building(building_id);
				if (!pk::is_alive(building)) continue;
				if (敌军设施.find(building.facility) < 0) continue;	//不是?据点，跳?

				pk::city@ city = pk::get_city(pk::get_city_id(building.pos));	//所?城市
				int bandit_force_id = get_bandit_force_id_of_city(city.get_id());  //?取?族??
				pk::set_district(building, pk::get_district(bandit_force_id), /*점령占?*/0);	//question

				int bandit_facility_id = get_bandit_facility_id_by_id(bandit_force_id);	//?势力建筑种类
				//if (bandit_facility_id != building.facility) {pk::kill(building); continue;}	//建筑种类錯誤则?灭建筑
			}
			for (int city_id = 0; city_id < 城市_末; city_id++)	//遍历城市
			{
				pk::city@ city = pk::get_city(city_id);
				for (int bandit_force_index = 0; bandit_force_index < 5; bandit_force_index++)
				{
					int bandit_force_id = 異族势力[bandit_force_index];
					func_빈도시구역내_敌军部队제거(city, bandit_force_id);	//?除空城贼兵
				}
			}
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	}

	Main main;
}

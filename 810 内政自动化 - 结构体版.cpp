<<<<<<< HEAD
﻿// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
=======
﻿// ## 2022/03/26 # 江东新风 # 内政执行官死亡问题，关闭按钮 ##
// ## 2022/03/21 # 江东新风 # 任务结束清空临时执行官信息 ##
// ## 2022/03/15 # 江东新风 # 执行官内政试写 ##
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2021/02/16 # 江东新风 # 无法征兵bug修复 ##
// ## 2021/01/20 # 江东新风 # 用结构体储存内政自动化信息，同时将征兵许可等信息取消按势力区分 ##
// ## 2020/12/15 # messi # 内政自动化改为太守自治 ##
// ## 2020/11/17 #messi#优化语句，修正描述错误##
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/07/26 ##

	/// 내정 자동화 항목 (도시별 허용/금지 설정 가능)
	//	- 각 항목별로 모든 도시에 대하여 일괄적으로 허용/금지 여부 설정 가능
	// 1. 순찰 (초기 설정 : 금지)
	// 2. 훈련 (초기 설정 : 금지)
	// 3. 징병 (초기 설정 : 금지)
	// 4. 포상 (초기 설정 : 금지)
	// 5. 수송 (초기 설정 : 금지)

	/// 내정 자동화 명령 실행
	// 1. 플레이어 1군단 도시만 적용
	// 2. 턴종료 시 명령 실행
	// 3. 내정 명령 실행을 위한 기존의 기본 조건을 충족해야 실행 (행동력, 금, 행동가능 무장, 필요 시설 등)
	//	- 단, 행동력의 경우에는 소모 여부 유저설정 가능
	// 4. '허용'으로 설정된 내정 항목에 대해서만 실행
	// 5. 설정한 내정 자동화 기준을 충족하는 경우에만 실행
	// 6. 내정 명령 실행 시 메시지 표시 (메시지 박스, 로그 메시지 - 표시형태 유저 설정 가능)

	/// 내정 자동화 기준 (도시별 기준 설정 가능)
	//	- 각 항목별로 모든 도시에 대하여 일괄적으로 기준 설정 가능
	// 1. 巡察标准_治安		(초기 설정 : 95)	- 도시의 치안이 설정된 순찰기준 치안 이하일 경우 순찰 
	// 2. 訓练标准_气力比率	(초기 설정 : 95(%))	- 도시의 최대 기력 대비 현재 기력의 비율이 설정된 훈련비율 이하일 경우 훈련
	// 3. 征兵标准_治安		(초기 설정 : 95)	- 도시의 치안이 설정된 징병기준 치안 이상일 경우 징병
	// 4. 征兵标准_金		(초기 설정 : 2500)	- 도시의 금이 설정된 징병기준 금 이상일 경우 징병
	// 5. 征兵标准_兵粮比率	(초기 설정 : 150(%))- 도시의 현재 병력 대비 현재 병량의 비율이 설정된 징병기준 병량비율 이상일 경우 징병
	// 6. 褒賞标准_忠诚度	(초기 설정 : 90)	- 무장의 충성도가 설정된 포상기준 충성도 이하일 경우 포상

	/// 내정 자동화 순서 모드 (포상, 수송은 순서 고정)
	//  - 모든 도시에 대하여 일괄적으로 내정 순서 모드 설정 가능
	// 모드 1 : 순찰 - 훈련 - 징병 - 포상 - 수송 (초기 설정)
	// 모드 2 : 순찰 - 징병 - 훈련 - 포상 - 수송
	// 모드 3 : 징병 - 순찰 - 훈련 - 포상 - 수송
	// 모드 4 : 징병 - 훈련 - 순찰 - 포상 - 수송
	// 모드 5 : 훈련 - 징병 - 순찰 - 포상 - 수송
	// 모드 6 : 훈련 - 순찰 - 징병 - 포상 - 수송

	/// 참고 사항
	// 1. 기존에 플레이하던 세이브 파일을 로드한 경우에는 항목별로 초기 설정이 다르게 설정되어 있을 수 있습니다.
	//	- 세이브 파일을 이동하시는 경우 PK2.1 패치 내 SaveData 폴더의 저장파일도 함께 옮겨 보관하셔야 설정이 유지됩니다.
	//	(설정 변경 값들은 해당 폴더의 세이브 파일에 저장되는 것으로 보입니다. '철거금지.cpp' 관련 설정도 마찬가지입니다.)
	// 2. 자세력 도시를 빼앗긴 경우 설정별 초기화 여부
	//	- 내정 자동화 순서 설정값 : 초기화됨 (이후 재탈환 시 초기 설정으로 초기화)
	//	- 내정 자동화 항목별 허용/금지 설정값 : 초기화됨 (이후 재탈환 시 초기 설정으로 초기화)
	//	- 내정 자동화 기준 설정값 : 초기화되지 않음 (이후 재탈환 시 기존 설정값이 반영됨)
	// 3. 도시에 커서를 가져가면 화면 우측 상단에 내정 자동화 관련 정보가 표시됩니다.
	//	- 내정 자동화 순서, 내정 자동화 항목별 허용/금지 여부, 내정 자동화 기준값의 설정이 반영되어 표시됩니다.
	// 4. 수송 자동화는 전방 도시(적 도시와 인접)끼리는 이루어지지 않습니다.
	// 5. 나머지 내용은 아래 CUSTOMIZE 부분을 참고하시기 바랍니다.
	// 6. "澶祸滑张" 해시 값의 KEY 인덱스 번호(uint16)는 50~650 정도까지 사용했습니다.
	//	- 기존 '철거금지.cpp' 파일에서는 0~47 사용하고 있습니다.

namespace 内政自动化
{

	/// ================================== CUSTOMIZE ============================================


		/// 내정 자동화 수행 시 행동력 소모 여부 설정
	const bool 内政自动化_行动力消耗_启用 = true;	// true = 행동력 소모, 행동력 부족 시 미수행

	/// 내정 자동화 수행 시 메시지 박스 표시 설정
	const bool 内政自动化_对话框提示_启用 = true;	// true = 메시지 박스 표시, false = 로그 메시지 표시

	/// 커서를 자세력 1군단 도시에 갖다 대는 경우 화면 우측 상단에 내정 자동화 설정상태를 표시
	const bool 内政自动化_城市上方信息显示_启用 = true;

	///	-----------------------------------------------------------------------------------------

		/// 단축키 설정

		// 순서 모드 설정 메뉴
	const string 上位菜单_顺序 = "z";
	const string 子菜单_顺序_统一变更 = "1";
	const string 子菜单_顺序 = "2";

	// 허용/금지 설정 메뉴
	const string 上位菜单_许可 = "x";
	const string 子菜单_许可_统一变更 = "1";
	const string 子菜单_许可_巡察 = "2";
	const string 子菜单_许可_訓练 = "3";
	const string 子菜单_许可_征兵 = "4";
	const string 子菜单_许可_褒賞 = "5";
	const string 子菜单_许可_运输 = "6";

	// 내정 기준 설정 메뉴
	const string 上位菜单_标准 = "c";
	const string 子菜单_标准_统一变更 = "1";
	const string 子菜单_标准_巡察_治安 = "2";
	const string 子菜单_标准_訓练_气力比率 = "3";
	const string 子菜单_标准_征兵_治安 = "4";
	const string 子菜单_标准_征兵_金 = "5";
	const string 子菜单_标准_征兵兵粮比率 = "6";
	const string 子菜单_标准_褒賞忠诚 = "7";

<<<<<<< HEAD
=======
	const string 上位菜单_生产 = "v";
	const string 子菜单_许可_枪 = "1";
	const string 子菜单_许可_戟 = "2";
	const string 子菜单_许可_弩 = "3";
	const string 子菜单_许可_马 = "4";

	const string 上位菜单_负责 = "b";
	const string 子菜单_负责_征兵 = "1";
	const string 子菜单_负责_巡察 = "2";
	const string 子菜单_负责_訓练 = "3";
	const string 子菜单_负责_生产 = "4";
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	///	-----------------------------------------------------------------------------------------

		/// 수송 내정 자동화 관련 설정

		// 수송부대가 출진할 도시의 현재 병력 대비 수송할 병력 비율
	const float 输送兵力比率 = 0.5f;

	// true = 후방 도시로부터 경계 도시까지 수송 (경계 도시 = 적 도시와 거리 2)
	// false = 후방, 경계 도시로부터 경계, 전방 도시까지 수송 (전방 도시 = 적 도시와 거리 1)
	// true =从后方城市到边境城市的运输（边界城市=距敌方城市2的距离）
	// false =从后方，边界城市到边界，前线城市的运输（前线城市=敌方城市和距离1）
	const bool 运输限制设置_到边境城市 = true;

	// 수송부대가 출진할 도시의 최대 병력 대비 현재 병력의 비율
	// 기본값 설명 : 현재 병력이 최대 병력의 0.5배 이상인 경우 수송
	const float 可运输条件_兵力比例 = 0.5f;

	// 수송부대가 출진할 도시의 현재 병력 대비 현재 병량의 비율
	// 기본값 설명 : 현재 병량이 현재 병력의 1.5배 이상인 경우 수송
	const float 可运输条件_兵粮比例 = 1.5f;

	// 수송 후보 도시의 최대 병력 대비 현재 병력의 비율
	// 기본값 설명 : 수송 후보 도시의 현재 병력이 최대 병력의 0.5배 이하인 경우 수송
	//目标城市中当前兵力与最大兵力的比例
	//默认说明：如果目标城市中的当前兵力少于最大兵力的0.5倍，则进行运输
	const float 可运输条件_兵力条件1 = 0.5f;

	// 수송부대가 출진할 도시의 병력 대비 수송 후보 도시의 병력 비율
	// 기본값 설명 : 수송 후보 도시의 현재 병력이 수송부대가 출진할 도시 현재 병력의 0.5배 이하인 경우 수송
	//运输出发城市中的兵力与目标城市的兵力的比例
	//默认说明：当运输出发城市的当前兵力少于目标城市中当前部队人数的0.5倍时的运输。
	const float 可运输条件_兵力条件2 = 0.5f;


	/// =========================================================================================


<<<<<<< HEAD
	const bool 디버깅 = false;

=======
	const bool 调试模式 = false;
	const bool 开启完全自动内政 = false;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	/// 정보 저장용 해시 값 信息保存緩冲zhi
	// 테스트 결과 pk::hash 함수가 "澶祸滑张" 말고 다른 문장의 해시 값을 지원하지 않는 것으로 추정됨
	// 부득이하게 "澶祸滑张" 문장의 해시 값을 사용하되 인덱스 값이 기존의 '철거금지.cpp' 파일과 충돌하지 않도록 적용
	const int KEY = pk::hash("内政自动化");


	/// 내정 자동화 인덱스용 항목 用於内政自动化索引的项目
	const int 内政自动化_巡察 = 0;
	const int 内政自动化_训练 = 1;
	const int 内政自动化_征兵 = 2;
	const int 内政自动化_褒赏 = 3;
	const int 内政自动化_运输 = 4;
<<<<<<< HEAD
=======
	const int 内政自动化_生产枪 = 5;
	const int 内政自动化_生产戟 = 6;
	const int 内政自动化_生产弩 = 7;
	const int 内政自动化_生产马 = 8;
	const int 内政自动化_生产 = 9;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

	/// 참고 : 도시 메뉴
	// 도시 : 개발, 징병(적용), 순찰(적용), 생산, 거래
	// 군사 : 훈련(적용), 수송(적용)
	// 인재 : 탐색, 등용, 포상(적용)
	// 외교 : 이호경식, 구호탄랑, 유언비어


	class Main
	{
		/*
		/// 도시별 내정 자동화 순서 정보, 포상기준 충성도 저장용 清单 儲存城市内政自动化的默认信息
		// 저장 방법 : 도시별로 순서 값 저장 (초기화 없이 고정된 순서 값 저장)
		array<int> 内政自动化_顺序情报(城市_末, 1);	// 초기 설정 : 순서 모드 1
		array<int> 褒賞标准_忠诚度(城市_末, 90);	// 초기 설정 : 90
		array<int> 巡察标准_治安(城市_末, 95);		// 초기 설정 : 95
		array<int> 訓练标准_气力比率(城市_末, 95);	// 초기 설정 : 95(%)
		array<int> 征兵标准_治安(城市_末, 95);		// 초기 설정 : 95
		array<int> 征兵标准_金(城市_末, 2500);		// 초기 설정 : 2500
		array<int> 征兵标准_兵粮比率(城市_末, 150);	// 초기 설정 : 150(%)

		/// 세력별 내정 자동화 允许与否 정보 저장용 清单 
		// 저장 방법 : 세력별로 모든 도시에 대해 정보를 저장(비트 연산 활용) 每个城市是否允许内政自动化的默认信息
		array<uint64> 巡察_许可(势力_末, uint64(0));	// 초기 설정 : 금지
		array<uint64> 訓练_许可(势力_末, uint64(0));	// 초기 설정 : 금지
		array<uint64> 征兵_许可(势力_末, uint64(0));	// 초기 설정 : 금지
		array<uint64> 褒賞_许可(势力_末, uint64(0));	// 초기 설정 : 금지
		array<uint64> 运输_许可(势力_末, uint64(0));	// 초기 설정 : 금지

		array<bool> 巡察_许可(城市_末, false);	// 초기 설정 : 금지
		array<bool> 訓练_许可(城市_末, false);	// 초기 설정 : 금지
		array<bool> 征兵_许可(城市_末, false);	// 초기 설정 : 금지
		array<bool> 褒賞_许可(城市_末, false);	// 초기 설정 : 금지
		array<bool> 运输_许可(城市_末, false);	// 초기 설정 : 금지*/

///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			/// 내정 자동화 설정 메뉴 配置内政自动化的菜单
			add_menu_城市内政自动化();

			/// 내정 자동화 설정 표시 内政自动化的显示信息
			// 거점에 커서를 갖다 대는 경우 해당 거점의 내정 자동화 설정 정보 표시 当鼠标放在据点上时会在左上角自动显示内政自动化的设定信息
			pk::bind(120, pk::trigger120_t(信息显示_城市内政自动化_信息));

			/// 내정 자동화 실행 트리거 内政自动化的执行程序
			pk::bind(112, pk::trigger112_t(回合结束_城市内政自动化));

			/// 정보 관리용 트리거 情报管理触发器
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		   //增加对应菜单
		void add_menu_城市内政自动化()
		{
			pk::menu_item 内政自动化_上位菜单_顺序;
			内政自动化_上位菜单_顺序.menu = global_menu::菜单_自动化;
			内政自动化_上位菜单_顺序.shortcut = 上位菜单_顺序;
			内政自动化_上位菜单_顺序.init = pk::building_menu_item_init_t(init);
			内政自动化_上位菜单_顺序.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
			内政自动化_上位菜单_顺序.is_visible = pk::menu_item_is_visible_t(isVisible);
			内政自动化_上位菜单_顺序.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("方针顺序"); });
			内政自动化_上位菜单_顺序.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("点击展开或收起"); });
			内政自动化_上位菜单_顺序.handler = pk::menu_item_handler_t(handler_上位菜单_顺序);
			pk::add_menu_item(内政自动化_上位菜单_顺序);

			pk::menu_item 内政自动化_菜单_统一变更_顺序;
			内政自动化_菜单_统一变更_顺序.menu = global_menu::菜单_自动化;
			内政自动化_菜单_统一变更_顺序.shortcut = 子菜单_顺序_统一变更;
			内政自动化_菜单_统一变更_顺序.is_visible = pk::menu_item_is_visible_t(isMenuVisible_顺序);
			内政自动化_菜单_统一变更_顺序.get_text = pk::menu_item_get_text_t(getText_统一变更_顺序);
			内政自动化_菜单_统一变更_顺序.get_desc = pk::menu_item_get_desc_t(getDesc_统一变更_顺序);
			内政自动化_菜单_统一变更_顺序.handler = pk::menu_item_handler_t(handler_统一变更_顺序);
			pk::add_menu_item(内政自动化_菜单_统一变更_顺序);

			pk::menu_item 内政自动化_菜单_顺序;
			内政自动化_菜单_顺序.menu = global_menu::菜单_自动化;
			内政自动化_菜单_顺序.shortcut = 子菜单_顺序;
			内政自动化_菜单_顺序.is_visible = pk::menu_item_is_visible_t(isMenuVisible_顺序);
			内政自动化_菜单_顺序.get_text = pk::menu_item_get_text_t(getText_顺序);
			内政自动化_菜单_顺序.get_desc = pk::menu_item_get_desc_t(getDesc_顺序);
			内政自动化_菜单_顺序.handler = pk::menu_item_handler_t(handler_顺序);
			pk::add_menu_item(内政自动化_菜单_顺序);



			pk::menu_item 内政自动化_上位菜单_许可;
			内政自动化_上位菜单_许可.menu = global_menu::菜单_自动化;
			内政自动化_上位菜单_许可.shortcut = 上位菜单_许可;
			内政自动化_上位菜单_许可.init = pk::building_menu_item_init_t(init);
			内政自动化_上位菜单_许可.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
			内政自动化_上位菜单_许可.is_visible = pk::menu_item_is_visible_t(isVisible);
			内政自动化_上位菜单_许可.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("方针命令"); });
			内政自动化_上位菜单_许可.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("点击展开或收起"); });
			内政自动化_上位菜单_许可.handler = pk::menu_item_handler_t(handler_上位菜单_许可);
			pk::add_menu_item(内政自动化_上位菜单_许可);

			pk::menu_item 内政自动化_菜单_统一变更_许可;
			内政自动化_菜单_统一变更_许可.menu = global_menu::菜单_自动化;
			内政自动化_菜单_统一变更_许可.shortcut = 子菜单_许可_统一变更;
			内政自动化_菜单_统一变更_许可.is_visible = pk::menu_item_is_visible_t(isMenuVisible_许可);
			内政自动化_菜单_统一变更_许可.get_text = pk::menu_item_get_text_t(getText_统一变更_许可);
			内政自动化_菜单_统一变更_许可.get_desc = pk::menu_item_get_desc_t(getDesc_统一变更_许可);
			内政自动化_菜单_统一变更_许可.handler = pk::menu_item_handler_t(handler_统一变更_许可);
			pk::add_menu_item(内政自动化_菜单_统一变更_许可);

			pk::menu_item 内政自动化_菜单_巡察_许可;
			内政自动化_菜单_巡察_许可.menu = global_menu::菜单_自动化;
			内政自动化_菜单_巡察_许可.shortcut = 子菜单_许可_巡察;
			内政自动化_菜单_巡察_许可.is_visible = pk::menu_item_is_visible_t(isMenuVisible_许可);
			内政自动化_菜单_巡察_许可.get_text = pk::menu_item_get_text_t(getText_巡察);
			内政自动化_菜单_巡察_许可.get_desc = pk::menu_item_get_desc_t(getDesc_巡察);
			内政自动化_菜单_巡察_许可.handler = pk::menu_item_handler_t(handler_巡察);
			pk::add_menu_item(内政自动化_菜单_巡察_许可);

			pk::menu_item 内政自动化_菜单_訓练_许可;
			内政自动化_菜单_訓练_许可.menu = global_menu::菜单_自动化;
			内政自动化_菜单_訓练_许可.shortcut = 子菜单_许可_訓练;
			内政自动化_菜单_訓练_许可.is_visible = pk::menu_item_is_visible_t(isMenuVisible_许可);
			内政自动化_菜单_訓练_许可.get_text = pk::menu_item_get_text_t(getText_訓练);
			内政自动化_菜单_訓练_许可.get_desc = pk::menu_item_get_desc_t(getDesc_訓练);
			内政自动化_菜单_訓练_许可.handler = pk::menu_item_handler_t(handler_訓练);
			pk::add_menu_item(内政自动化_菜单_訓练_许可);

			pk::menu_item 内政自动化_菜单_征兵_许可;
			内政自动化_菜单_征兵_许可.menu = global_menu::菜单_自动化;
			内政自动化_菜单_征兵_许可.shortcut = 子菜单_许可_征兵;
			内政自动化_菜单_征兵_许可.is_visible = pk::menu_item_is_visible_t(isMenuVisible_许可);
			内政自动化_菜单_征兵_许可.get_text = pk::menu_item_get_text_t(getText_征兵);
			内政自动化_菜单_征兵_许可.get_desc = pk::menu_item_get_desc_t(getDesc_征兵);
			内政自动化_菜单_征兵_许可.handler = pk::menu_item_handler_t(handler_征兵);
			pk::add_menu_item(内政自动化_菜单_征兵_许可);

			pk::menu_item 内政自动化_菜单_褒賞_许可;
			内政自动化_菜单_褒賞_许可.menu = global_menu::菜单_自动化;
			内政自动化_菜单_褒賞_许可.shortcut = 子菜单_许可_褒賞;
			内政自动化_菜单_褒賞_许可.is_visible = pk::menu_item_is_visible_t(isMenuVisible_许可);
			内政自动化_菜单_褒賞_许可.get_text = pk::menu_item_get_text_t(getText_褒賞);
			内政自动化_菜单_褒賞_许可.get_desc = pk::menu_item_get_desc_t(getDesc_褒賞);
			内政自动化_菜单_褒賞_许可.handler = pk::menu_item_handler_t(handler_褒賞);
			pk::add_menu_item(内政自动化_菜单_褒賞_许可);

			pk::menu_item 内政自动化_菜单_运输_许可;
			内政自动化_菜单_运输_许可.menu = global_menu::菜单_自动化;
			内政自动化_菜单_运输_许可.is_visible = pk::menu_item_is_visible_t(isMenuVisible_许可);
			内政自动化_菜单_运输_许可.shortcut = 子菜单_许可_运输;
			内政自动化_菜单_运输_许可.get_text = pk::menu_item_get_text_t(getText_运输);
			内政自动化_菜单_运输_许可.get_desc = pk::menu_item_get_desc_t(getDesc_运输);
			内政自动化_菜单_运输_许可.handler = pk::menu_item_handler_t(handler_运输);
			pk::add_menu_item(内政自动化_菜单_运输_许可);

<<<<<<< HEAD

=======
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			pk::menu_item 内政自动化_上位菜单_标准;
			内政自动化_上位菜单_标准.menu = global_menu::菜单_自动化;
			内政自动化_上位菜单_标准.shortcut = 上位菜单_标准;
			内政自动化_上位菜单_标准.init = pk::building_menu_item_init_t(init);
			内政自动化_上位菜单_标准.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
			内政自动化_上位菜单_标准.is_visible = pk::menu_item_is_visible_t(isVisible);
			内政自动化_上位菜单_标准.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("自治标准"); });
			内政自动化_上位菜单_标准.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("点击展开或收起"); });
			内政自动化_上位菜单_标准.handler = pk::menu_item_handler_t(handler_上位菜单_标准);
			pk::add_menu_item(内政自动化_上位菜单_标准);

			pk::menu_item 内政自动化_菜单_统一变更_标准;
			内政自动化_菜单_统一变更_标准.menu = global_menu::菜单_自动化;
			内政自动化_菜单_统一变更_标准.shortcut = 子菜单_标准_统一变更;
			内政自动化_菜单_统一变更_标准.is_visible = pk::menu_item_is_visible_t(isMenuVisible_标准);
			内政自动化_菜单_统一变更_标准.get_text = pk::menu_item_get_text_t(getText_统一变更_标准);
			内政自动化_菜单_统一变更_标准.get_desc = pk::menu_item_get_desc_t(getDesc_统一变更_标准);
			内政自动化_菜单_统一变更_标准.handler = pk::menu_item_handler_t(handler_统一变更_标准);
			pk::add_menu_item(内政自动化_菜单_统一变更_标准);

			pk::menu_item 内政自动化_菜单_巡察标准_治安;
			内政自动化_菜单_巡察标准_治安.menu = global_menu::菜单_自动化;
			内政自动化_菜单_巡察标准_治安.shortcut = 子菜单_标准_巡察_治安;
			内政自动化_菜单_巡察标准_治安.is_visible = pk::menu_item_is_visible_t(isMenuVisible_标准);
			内政自动化_菜单_巡察标准_治安.get_text = pk::menu_item_get_text_t(getText_巡察标准_治安);
			内政自动化_菜单_巡察标准_治安.get_desc = pk::menu_item_get_desc_t(getDesc_巡察标准_治安);
			内政自动化_菜单_巡察标准_治安.handler = pk::menu_item_handler_t(handler_巡察标准_治安);
			pk::add_menu_item(内政自动化_菜单_巡察标准_治安);

			pk::menu_item 内政自动化_菜单_訓练标准_气力比率;
			内政自动化_菜单_訓练标准_气力比率.menu = global_menu::菜单_自动化;
			内政自动化_菜单_訓练标准_气力比率.shortcut = 子菜单_标准_訓练_气力比率;
			内政自动化_菜单_訓练标准_气力比率.is_visible = pk::menu_item_is_visible_t(isMenuVisible_标准);
			内政自动化_菜单_訓练标准_气力比率.get_text = pk::menu_item_get_text_t(getText_訓练标准_气力比率);
			内政自动化_菜单_訓练标准_气力比率.get_desc = pk::menu_item_get_desc_t(getDesc_訓练标准_气力比率);
			内政自动化_菜单_訓练标准_气力比率.handler = pk::menu_item_handler_t(handler_訓练标准_气力比率);
			pk::add_menu_item(内政自动化_菜单_訓练标准_气力比率);

			pk::menu_item 内政自动化_菜单_征兵标准_治安;
			内政自动化_菜单_征兵标准_治安.menu = global_menu::菜单_自动化;
			内政自动化_菜单_征兵标准_治安.shortcut = 子菜单_标准_征兵_治安;
			内政自动化_菜单_征兵标准_治安.is_visible = pk::menu_item_is_visible_t(isMenuVisible_标准);
			内政自动化_菜单_征兵标准_治安.get_text = pk::menu_item_get_text_t(getText_征兵标准_治安);
			内政自动化_菜单_征兵标准_治安.get_desc = pk::menu_item_get_desc_t(getDesc_征兵标准_治安);
			内政自动化_菜单_征兵标准_治安.handler = pk::menu_item_handler_t(handler_征兵标准_治安);
			pk::add_menu_item(内政自动化_菜单_征兵标准_治安);

			pk::menu_item 内政自动化_菜单_征兵标准_金;
			内政自动化_菜单_征兵标准_金.menu = global_menu::菜单_自动化;
			内政自动化_菜单_征兵标准_金.shortcut = 子菜单_标准_征兵_金;
			内政自动化_菜单_征兵标准_金.is_visible = pk::menu_item_is_visible_t(isMenuVisible_标准);
			内政自动化_菜单_征兵标准_金.get_text = pk::menu_item_get_text_t(getText_征兵标准_金);
			内政自动化_菜单_征兵标准_金.get_desc = pk::menu_item_get_desc_t(getDesc_征兵标准_金);
			内政自动化_菜单_征兵标准_金.handler = pk::menu_item_handler_t(handler_征兵标准_金);
			pk::add_menu_item(内政自动化_菜单_征兵标准_金);

			pk::menu_item 内政自动化_菜单_征兵标准_兵粮比率;
			内政自动化_菜单_征兵标准_兵粮比率.menu = global_menu::菜单_自动化;
			内政自动化_菜单_征兵标准_兵粮比率.shortcut = 子菜单_标准_征兵兵粮比率;
			内政自动化_菜单_征兵标准_兵粮比率.is_visible = pk::menu_item_is_visible_t(isMenuVisible_标准);
			内政自动化_菜单_征兵标准_兵粮比率.get_text = pk::menu_item_get_text_t(getText_征兵标准_兵粮比率);
			内政自动化_菜单_征兵标准_兵粮比率.get_desc = pk::menu_item_get_desc_t(getDesc_征兵标准_兵粮比率);
			内政自动化_菜单_征兵标准_兵粮比率.handler = pk::menu_item_handler_t(handler_征兵标准_兵粮比率);
			pk::add_menu_item(内政自动化_菜单_征兵标准_兵粮比率);

			pk::menu_item 内政自动化_菜单_褒賞标准_忠诚度;
			内政自动化_菜单_褒賞标准_忠诚度.menu = global_menu::菜单_自动化;
			内政自动化_菜单_褒賞标准_忠诚度.shortcut = 子菜单_标准_褒賞忠诚;
			内政自动化_菜单_褒賞标准_忠诚度.is_visible = pk::menu_item_is_visible_t(isMenuVisible_标准);
			内政自动化_菜单_褒賞标准_忠诚度.get_text = pk::menu_item_get_text_t(getText_褒賞标准_忠诚度);
			内政自动化_菜单_褒賞标准_忠诚度.get_desc = pk::menu_item_get_desc_t(getDesc_褒賞标准_忠诚度);
			内政自动化_菜单_褒賞标准_忠诚度.handler = pk::menu_item_handler_t(handler_褒賞标准_忠诚度);
			pk::add_menu_item(内政自动化_菜单_褒賞标准_忠诚度);
<<<<<<< HEAD
=======
			if (开启完全自动内政)
			{
				//****************************生产菜单开始****************************

				pk::menu_item 内政自动化_上位菜单_生产;
				内政自动化_上位菜单_生产.menu = global_menu::菜单_自动化;
				内政自动化_上位菜单_生产.shortcut = 上位菜单_生产;
				内政自动化_上位菜单_生产.init = pk::building_menu_item_init_t(init);
				内政自动化_上位菜单_生产.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
				内政自动化_上位菜单_生产.is_visible = pk::menu_item_is_visible_t(isVisible);
				内政自动化_上位菜单_生产.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("生产设置"); });
				内政自动化_上位菜单_生产.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("点击展开或收起"); });
				内政自动化_上位菜单_生产.handler = pk::menu_item_handler_t(handler_上位菜单_生产);
				pk::add_menu_item(内政自动化_上位菜单_生产);
				/*
							pk::menu_item 内政自动化_菜单_统一变更_许可;
							内政自动化_菜单_统一变更_许可.menu = global_menu::菜单_自动化;
							内政自动化_菜单_统一变更_许可.shortcut = 子菜单_许可_统一变更;
							内政自动化_菜单_统一变更_许可.is_visible = pk::menu_item_is_visible_t(isMenuVisible_生产);
							内政自动化_菜单_统一变更_许可.get_text = pk::menu_item_get_text_t(getText_统一变更_许可);
							内政自动化_菜单_统一变更_许可.get_desc = pk::menu_item_get_desc_t(getDesc_统一变更_许可);
							内政自动化_菜单_统一变更_许可.handler = pk::menu_item_handler_t(handler_统一变更_许可);
							pk::add_menu_item(内政自动化_菜单_统一变更_许可);
				*/
				pk::menu_item 内政自动化_菜单_枪_许可;
				内政自动化_菜单_枪_许可.menu = global_menu::菜单_自动化;
				内政自动化_菜单_枪_许可.shortcut = 子菜单_许可_枪;
				内政自动化_菜单_枪_许可.is_visible = pk::menu_item_is_visible_t(isMenuVisible_生产);
				内政自动化_菜单_枪_许可.get_text = pk::menu_item_get_text_t(getText_枪许可);
				内政自动化_菜单_枪_许可.get_desc = pk::menu_item_get_desc_t(getDesc_枪许可);
				内政自动化_菜单_枪_许可.handler = pk::menu_item_handler_t(handler_枪许可);
				pk::add_menu_item(内政自动化_菜单_枪_许可);

				pk::menu_item 内政自动化_菜单_戟_许可;
				内政自动化_菜单_戟_许可.menu = global_menu::菜单_自动化;
				内政自动化_菜单_戟_许可.shortcut = 子菜单_许可_戟;
				内政自动化_菜单_戟_许可.is_visible = pk::menu_item_is_visible_t(isMenuVisible_生产);
				内政自动化_菜单_戟_许可.get_text = pk::menu_item_get_text_t(getText_戟许可);
				内政自动化_菜单_戟_许可.get_desc = pk::menu_item_get_desc_t(getDesc_戟许可);
				内政自动化_菜单_戟_许可.handler = pk::menu_item_handler_t(handler_戟许可);
				pk::add_menu_item(内政自动化_菜单_戟_许可);

				pk::menu_item 内政自动化_菜单_弩_许可;
				内政自动化_菜单_弩_许可.menu = global_menu::菜单_自动化;
				内政自动化_菜单_弩_许可.shortcut = 子菜单_许可_弩;
				内政自动化_菜单_弩_许可.is_visible = pk::menu_item_is_visible_t(isMenuVisible_生产);
				内政自动化_菜单_弩_许可.get_text = pk::menu_item_get_text_t(getText_弩许可);
				内政自动化_菜单_弩_许可.get_desc = pk::menu_item_get_desc_t(getDesc_弩许可);
				内政自动化_菜单_弩_许可.handler = pk::menu_item_handler_t(handler_弩许可);
				pk::add_menu_item(内政自动化_菜单_弩_许可);

				pk::menu_item 内政自动化_菜单_马_许可;
				内政自动化_菜单_马_许可.menu = global_menu::菜单_自动化;
				内政自动化_菜单_马_许可.shortcut = 子菜单_许可_马;
				内政自动化_菜单_马_许可.is_visible = pk::menu_item_is_visible_t(isMenuVisible_生产);
				内政自动化_菜单_马_许可.get_text = pk::menu_item_get_text_t(getText_马许可);
				内政自动化_菜单_马_许可.get_desc = pk::menu_item_get_desc_t(getDesc_马许可);
				内政自动化_菜单_马_许可.handler = pk::menu_item_handler_t(handler_马许可);
				pk::add_menu_item(内政自动化_菜单_马_许可);

				//****************************生产菜单结束****************************
				//****************************负责菜单开始****************************

				pk::menu_item 内政自动化_上位菜单_负责;
				内政自动化_上位菜单_负责.menu = global_menu::菜单_自动化;
				内政自动化_上位菜单_负责.shortcut = 上位菜单_负责;
				内政自动化_上位菜单_负责.init = pk::building_menu_item_init_t(init);
				内政自动化_上位菜单_负责.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
				内政自动化_上位菜单_负责.is_visible = pk::menu_item_is_visible_t(isVisible);
				内政自动化_上位菜单_负责.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("内政负责"); });
				内政自动化_上位菜单_负责.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("点击展开或收起"); });
				内政自动化_上位菜单_负责.handler = pk::menu_item_handler_t(handler_上位菜单_负责);
				pk::add_menu_item(内政自动化_上位菜单_负责);
				/*
							pk::menu_item 内政自动化_菜单_统一变更_许可;
							内政自动化_菜单_统一变更_许可.menu = global_menu::菜单_自动化;
							内政自动化_菜单_统一变更_许可.shortcut = 子菜单_许可_统一变更;
							内政自动化_菜单_统一变更_许可.is_visible = pk::menu_item_is_visible_t(isMenuVisible_许可);
							内政自动化_菜单_统一变更_许可.get_text = pk::menu_item_get_text_t(getText_统一变更_许可);
							内政自动化_菜单_统一变更_许可.get_desc = pk::menu_item_get_desc_t(getDesc_统一变更_许可);
							内政自动化_菜单_统一变更_许可.handler = pk::menu_item_handler_t(handler_统一变更_许可);
							pk::add_menu_item(内政自动化_菜单_统一变更_许可);
				*/
				pk::menu_item 内政自动化_菜单_征兵_负责;
				内政自动化_菜单_征兵_负责.menu = global_menu::菜单_自动化;
				内政自动化_菜单_征兵_负责.shortcut = 子菜单_负责_征兵;
				内政自动化_菜单_征兵_负责.is_visible = pk::menu_item_is_visible_t(isMenuVisible_负责);
				内政自动化_菜单_征兵_负责.get_text = pk::menu_item_get_text_t(getText_征兵负责);
				内政自动化_菜单_征兵_负责.get_desc = pk::menu_item_get_desc_t(getDesc_征兵负责);
				内政自动化_菜单_征兵_负责.handler = pk::menu_item_handler_t(handler_征兵负责);
				pk::add_menu_item(内政自动化_菜单_征兵_负责);

				pk::menu_item 内政自动化_菜单_巡察_负责;
				内政自动化_菜单_巡察_负责.menu = global_menu::菜单_自动化;
				内政自动化_菜单_巡察_负责.shortcut = 子菜单_负责_巡察;
				内政自动化_菜单_巡察_负责.is_visible = pk::menu_item_is_visible_t(isMenuVisible_负责);
				内政自动化_菜单_巡察_负责.get_text = pk::menu_item_get_text_t(getText_巡察负责);
				内政自动化_菜单_巡察_负责.get_desc = pk::menu_item_get_desc_t(getDesc_巡察负责);
				内政自动化_菜单_巡察_负责.handler = pk::menu_item_handler_t(handler_巡察负责);
				pk::add_menu_item(内政自动化_菜单_巡察_负责);

				pk::menu_item 内政自动化_菜单_訓练_负责;
				内政自动化_菜单_訓练_负责.menu = global_menu::菜单_自动化;
				内政自动化_菜单_訓练_负责.shortcut = 子菜单_负责_訓练;
				内政自动化_菜单_訓练_负责.is_visible = pk::menu_item_is_visible_t(isMenuVisible_负责);
				内政自动化_菜单_訓练_负责.get_text = pk::menu_item_get_text_t(getText_訓练负责);
				内政自动化_菜单_訓练_负责.get_desc = pk::menu_item_get_desc_t(getDesc_訓练负责);
				内政自动化_菜单_訓练_负责.handler = pk::menu_item_handler_t(handler_訓练负责);
				pk::add_menu_item(内政自动化_菜单_訓练_负责);

				pk::menu_item 内政自动化_菜单_生产_负责;
				内政自动化_菜单_生产_负责.menu = global_menu::菜单_自动化;
				内政自动化_菜单_生产_负责.shortcut = 子菜单_负责_生产;
				内政自动化_菜单_生产_负责.is_visible = pk::menu_item_is_visible_t(isMenuVisible_负责);
				内政自动化_菜单_生产_负责.get_text = pk::menu_item_get_text_t(getText_生产负责);
				内政自动化_菜单_生产_负责.get_desc = pk::menu_item_get_desc_t(getDesc_生产负责);
				内政自动化_菜单_生产_负责.handler = pk::menu_item_handler_t(handler_生产负责);
				pk::add_menu_item(内政自动化_菜单_生产_负责);

				//****************************负责菜单结束****************************
			}

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		}


		bool handler_上位菜单_顺序()
		{
			global_menu::自动化_展开_当前 = (global_menu::自动化_展开_当前 == global_menu::自动化_展开_顺序) ? 0 : global_menu::自动化_展开_顺序;
			return false;
		}

		bool handler_上位菜单_许可()
		{
			global_menu::自动化_展开_当前 = (global_menu::自动化_展开_当前 == global_menu::自动化_展开_许可) ? 0 : global_menu::自动化_展开_许可;
			return false;
		}

<<<<<<< HEAD
=======
		bool handler_上位菜单_生产()
		{
			global_menu::自动化_展开_当前 = (global_menu::自动化_展开_当前 == global_menu::自动化_展开_生产) ? 0 : global_menu::自动化_展开_生产;
			return false;
		}

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		bool handler_上位菜单_标准()
		{
			global_menu::自动化_展开_当前 = (global_menu::自动化_展开_当前 == global_menu::自动化_展开_标准) ? 0 : global_menu::自动化_展开_标准;
			return false;
		}

<<<<<<< HEAD
=======
		bool handler_上位菜单_负责()
		{
			global_menu::自动化_展开_当前 = (global_menu::自动化_展开_当前 == global_menu::自动化_展开_负责) ? 0 : global_menu::自动化_展开_负责;
			return false;
		}

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321


		bool isMenuVisible_顺序()
		{
			if (global_menu::自动化_展开_当前 != global_menu::自动化_展开_顺序) return false;
			return true;
		}

		bool isMenuVisible_许可()
		{
			if (global_menu::自动化_展开_当前 != global_menu::自动化_展开_许可) return false;
			return true;
		}

		bool isMenuVisible_标准()
		{
			if (global_menu::自动化_展开_当前 != global_menu::自动化_展开_标准) return false;
			return true;
		}

<<<<<<< HEAD

=======
		bool isMenuVisible_生产()
		{
			if (global_menu::自动化_展开_当前 != global_menu::自动化_展开_生产) return false;
			return true;
		}

		bool isMenuVisible_负责()
		{
			if (global_menu::自动化_展开_当前 != global_menu::自动化_展开_负责) return false;
			return true;
		}
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321


		//内政自动化显示信息
		void 信息显示_城市内政自动化_信息()
		{
			if (!内政自动化_城市上方信息显示_启用) return;

			// 커서가 가리키는 좌표光标指引的坐标
			pk::point cursor_pos = pk::get_cursor_hex_pos();
			if (!pk::is_valid_pos(cursor_pos)) return;

			// 커서가 가리키는 좌표 상의 건물光标上指示的建筑物
			pk::building@ building = pk::get_building(cursor_pos);
			if (building is null) return;

			// 커서가 가리키는 좌표 상의 도시 对应建筑物是否?城市
			pk::city@ city = pk::building_to_city(building);
			if (city is null) return;

			// 도시의 좌표 조건 확인 确认城市坐标是否有效
			pk::point city_pos = city.get_pos();
			if (!pk::is_valid_pos(city_pos))return;

			// 도시의 세력 및 군단 조건 확인 确认城市是否是玩家的，是否归玩家控制，及是否城市控制势力id是当前回合的执行势力id
			if (!city.is_player()) return;
			if (!pk::is_player_controlled(city)) return;
			if (city.get_force_id() != pk::get_current_turn_force_id()) return;
			// 내정 자동화 설정 정보 표시 显示内政自动化信息
			BaseInfo@ base_t = @base_ex[city.get_id()];
			func_内政自动化_信息显示(building, pk::get_current_turn_force_id(), city.get_id(), base_t.auto_sequence);
		}

		void 回合结束_城市内政自动化(pk::force@ force)
		{
			if (!force.is_player()) return;	// 计算机势力除外 컴퓨터 세력 제외

			array<pk::city@> city_list = pk::list_to_array(pk::get_city_list(force));	// 세력 도시 清单
			for (int i = 0; i < int(city_list.length); i++)
			{
				pk::city@ city = city_list[i];
				pk::building@ building = pk::city_to_building(city);

<<<<<<< HEAD
				if (디버깅)
=======
				if (调试模式)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				{
					string city_name = pk::format("城市\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(city)));
					pk::message_box(pk::encode(pk::format("已确认{}", city_name)));
				}

				if (!pk::is_player_controlled(building))			continue;	// 플레이어 위임군단 도시인 경우 제외
				if (pk::get_idle_person_list(building).count == 0)	continue;	// 행동가능 무장이 없는 경우 제외
				/// 참고 사항
				// cmd 구조체를 사용하여 내정 명령을 실행하는 경우 실행이 불가능한 상황이면 알아서 실행하지 않음 (무장 포상 제외)
				// 따로 실행이 불가능한 경우를 조건으로 추가하지 않아도 됨 (행동력 소비량, 요구 시설, 행동가능 무장 존재, 금 소비량 등)
				BaseInfo@ base_t = @base_ex[city.get_id()];
				func_内政自动化(force, city, base_t.auto_sequence);
			}
		}


		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void func_内政自动化_信息显示(pk::building@ building, int force_id, int city_id, int 内政自动化_顺序)
		{
<<<<<<< HEAD
=======
			
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			string building_name = pk::decode(pk::get_name(building));

			string title = pk::format("太守自治方针(\x1b[1x{}\x1b[0x)", building_name);

			string order_info = pk::format("(方针顺序:\x1b[1x{}\x1b[0x)", 内政自动化_顺序);

			BaseInfo@ base_t = @base_ex[city_id];
			string inspections_enabled = "巡察:" + (base_t.can_inspections ? "\x1b[1x允许" : "\x1b[1x禁止");
			string drills_enabled = "训练:" + (base_t.can_drill ? "\x1b[1x允许" : "\x1b[1x禁止");
			string recruit_enabled = "征兵:" + (base_t.can_recruit ? "\x1b[1x允许" : "\x1b[1x禁止");
			string reward_enabled = "褒奖:" + (base_t.can_reward ? "\x1b[1x允许" : "\x1b[1x禁止");
			string deploy_enabled = "运输:" + (base_t.can_transport ? "\x1b[1x允许" : "\x1b[1x禁止");

			string std_inspections = pk::format("(巡察标准 治安:\x1b[1x{}\x1b[0x)", base_t.inspections_std);
			string std_drills = pk::format("(训练标准 士气比例:\x1b[1x{}\x1b[0x%)", base_t.drill_std);
			string std_recruit_order = pk::format("(征兵标准 治安:\x1b[1x{}\x1b[0x)", base_t.recruit_std_porder);
			string std_recruit_gold = pk::format("(征兵标准 资金:\x1b[1x{}\x1b[0x)", base_t.recruit_std_gold);
			string std_recruit_food = pk::format("(征兵标准 粮兵比例:\x1b[1x{}\x1b[0x%)", base_t.recruit_std_foodtroop);
			string std_reward = pk::format("(褒奖标准 忠诚度:\x1b[1x{}\x1b[0x)", base_t.reward_std);

			array<string> str_内政自动化_顺序(0);
			if (内政自动化_顺序 == 1)	str_内政自动化_顺序 = { inspections_enabled, drills_enabled, recruit_enabled, reward_enabled, deploy_enabled };
			else if (内政自动化_顺序 == 2)	str_内政自动化_顺序 = { inspections_enabled, recruit_enabled, drills_enabled, reward_enabled, deploy_enabled };
			else if (内政自动化_顺序 == 3)	str_内政自动化_顺序 = { recruit_enabled, inspections_enabled, drills_enabled, reward_enabled, deploy_enabled };
			else if (内政自动化_顺序 == 4)	str_内政自动化_顺序 = { recruit_enabled, drills_enabled, inspections_enabled, reward_enabled, deploy_enabled };
			else if (内政自动化_顺序 == 5)	str_内政自动化_顺序 = { drills_enabled, recruit_enabled, inspections_enabled, reward_enabled, deploy_enabled };
			else if (内政自动化_顺序 == 6)	str_内政自动化_顺序 = { drills_enabled, inspections_enabled, recruit_enabled, reward_enabled, deploy_enabled };

			array<string> str_内政标准_顺序(0);
			if (内政自动化_顺序 == 1)	str_内政标准_顺序 = { std_inspections, std_drills, std_recruit_order, std_recruit_gold, std_recruit_food, std_reward };
			else if (内政自动化_顺序 == 2)	str_内政标准_顺序 = { std_inspections, std_recruit_order, std_recruit_gold, std_recruit_food, std_drills, std_reward };
			else if (内政自动化_顺序 == 3)	str_内政标准_顺序 = { std_recruit_order, std_recruit_gold, std_recruit_food, std_inspections, std_drills, std_reward };
			else if (内政自动化_顺序 == 4)	str_内政标准_顺序 = { std_recruit_order, std_recruit_gold, std_recruit_food, std_drills, std_inspections, std_reward };
			else if (内政自动化_顺序 == 5)	str_内政标准_顺序 = { std_drills, std_recruit_order, std_recruit_gold, std_recruit_food, std_inspections, std_reward };
			else if (内政自动化_顺序 == 6)	str_内政标准_顺序 = { std_drills, std_inspections, std_recruit_order, std_recruit_gold, std_recruit_food, std_reward };

			int width = int(pk::get_resolution().width) - 230;
<<<<<<< HEAD

=======
			pk::point left_down = pk::point(int(pk::get_resolution().width) - 10, 75 + (11) * 20 + (开启完全自动内政?42 * 3:0) + 40 +5);
			pk::draw_filled_rect(pk::rectangle(pk::point(width-5, 15), left_down), ((0xff / 2) << 24) | 0x010101);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			pk::draw_text(pk::encode(title), pk::point(width, 20), 0xffffffff, FONT_BIG, 0xff000000);
			pk::draw_text(pk::encode(order_info), pk::point(width, 55), 0xffffffff, FONT_SMALL, 0xff000000);

			int end = int(str_内政自动化_顺序.length);
			for (int i = 0; i < end; i++)
			{
				pk::draw_text(pk::encode(str_内政自动化_顺序[i]), pk::point(width, 75 + i * 20), 0xffffffff, FONT_SMALL, 0xff000000);
			}
			for (int i = 0; i < int(str_内政标准_顺序.length); i++)
			{
				pk::draw_text(pk::encode(str_内政标准_顺序[i]), pk::point(width, 75 + (i + end) * 20), 0xffffffff, FONT_SMALL, 0xff000000);
			}
<<<<<<< HEAD
=======
			if (开启完全自动内政)
			{
				pk::draw_text(pk::encode("征兵执行者："), pk::point(width, 75 + (11) * 20), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::point rightup = pk::point(int(pk::get_resolution().width) - 130, 75 + (11) * 20);
				pk::point leftdown = pk::point(int(pk::get_resolution().width) - 130 + 32, 75 + (11) * 20 + 40);
				auto rect = pk::rectangle(rightup, leftdown);
				if (pk::is_valid_person_id(base_t.recruit_person)) pk::draw_face(FACE_SMALL, base_t.recruit_person, rect, FACE_R);

				pk::draw_text(pk::encode("巡查执行者："), pk::point(width, 75 + (11) * 20 + 42 * 1), 0xffffffff, FONT_SMALL, 0xff000000);
				rightup = pk::point(int(pk::get_resolution().width) - 130, 75 + (11) * 20 + 42 * 1);
				leftdown = pk::point(int(pk::get_resolution().width) - 130 + 32, 75 + (11) * 20 + 42 * 1 + 40);
				rect = pk::rectangle(rightup, leftdown);
				if (pk::is_valid_person_id(base_t.inspections_person)) pk::draw_face(FACE_SMALL, base_t.inspections_person, rect, FACE_R);

				pk::draw_text(pk::encode("训练执行者："), pk::point(width, 75 + (11) * 20 + 42 * 2), 0xffffffff, FONT_SMALL, 0xff000000);
				rightup = pk::point(int(pk::get_resolution().width) - 130, 75 + (11) * 20 + 42 * 2);
				leftdown = pk::point(int(pk::get_resolution().width) - 130 + 32, 75 + (11) * 20 + 42 * 2 + 40);
				rect = pk::rectangle(rightup, leftdown);
				if (pk::is_valid_person_id(base_t.drill_person)) pk::draw_face(FACE_SMALL, base_t.drill_person, rect, FACE_R);

				pk::draw_text(pk::encode("生产执行者："), pk::point(width, 75 + (11) * 20 + 42 * 3), 0xffffffff, FONT_SMALL, 0xff000000);
				rightup = pk::point(int(pk::get_resolution().width) - 130, 75 + (11) * 20 + 42 * 3);
				leftdown = pk::point(int(pk::get_resolution().width) - 130 + 32, 75 + (11) * 20 + 42 * 3 + 40);
				rect = pk::rectangle(rightup, leftdown);
				if (pk::is_valid_person_id(base_t.produce_person)) pk::draw_face(FACE_SMALL, base_t.produce_person, rect, FACE_R);
			}

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		}

		void func_内政自动化(pk::force@ force, pk::city@ city, int 内政自动化_顺序)
		{
<<<<<<< HEAD
			if (内政自动化_顺序 == 1) { func_内政自动化_巡察(force, city);	func_内政自动化_训练(force, city);	func_内政自动化_征兵(force, city); }
			else if (内政自动化_顺序 == 2) { func_内政自动化_巡察(force, city);	func_内政自动化_征兵(force, city);	func_内政自动化_训练(force, city); }
			else if (内政自动化_顺序 == 3) { func_内政自动化_征兵(force, city);	func_内政自动化_巡察(force, city);	func_内政自动化_训练(force, city); }
			else if (内政自动化_顺序 == 4) { func_内政自动化_征兵(force, city);	func_内政自动化_训练(force, city);	func_内政自动化_巡察(force, city); }
			else if (内政自动化_顺序 == 5) { func_内政自动化_训练(force, city);	func_内政自动化_征兵(force, city);	func_内政自动化_巡察(force, city); }
			else if (内政自动化_顺序 == 6) { func_内政自动化_训练(force, city);	func_内政自动化_巡察(force, city);	func_内政自动化_征兵(force, city); }

=======
			if (开启完全自动内政)
			{
				func_内政自动化_执行官征兵(force, city);	
				func_内政自动化_执行官巡察(force, city);	
				func_内政自动化_执行官训练(force, city);
			}
			else
			{
				if (内政自动化_顺序 == 1) { func_内政自动化_巡察(force, city);	func_内政自动化_训练(force, city);	func_内政自动化_征兵(force, city); }
				else if (内政自动化_顺序 == 2) { func_内政自动化_巡察(force, city);	func_内政自动化_征兵(force, city);	func_内政自动化_训练(force, city); }
				else if (内政自动化_顺序 == 3) { func_内政自动化_征兵(force, city);	func_内政自动化_巡察(force, city);	func_内政自动化_训练(force, city); }
				else if (内政自动化_顺序 == 4) { func_内政自动化_征兵(force, city);	func_内政自动化_训练(force, city);	func_内政自动化_巡察(force, city); }
				else if (内政自动化_顺序 == 5) { func_内政自动化_训练(force, city);	func_内政自动化_征兵(force, city);	func_内政自动化_巡察(force, city); }
				else if (内政自动化_顺序 == 6) { func_内政自动化_训练(force, city);	func_内政自动化_巡察(force, city);	func_内政自动化_征兵(force, city); }

			}
		
			func_内政自动化_执行官生产(force, city);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			func_内政自动化_褒赏(force, city);
			func_内政自动化_运输(force, city);
		}

		void func_内政自动化_巡察(pk::force@ force, pk::city@ city)
		{
			pk::building@ building = pk::city_to_building(city);
			pk::district@ district = pk::get_district(building.get_district_id());
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building);

			string city_name = pk::format("顺序模式\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(city)));

<<<<<<< HEAD
			if (디버깅) pk::message_box(pk::encode(pk::format("开始确认{}的巡察运行条件", city_name)));
=======
			if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的巡察运行条件", city_name)));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

			// 기본 조건 확인
			if (内政自动化_行动力消耗_启用 and int(district.ap) < 20)	return;	// 행동력 조건
			if (int(person_list.count) == 0)							return;	// 행동가능 무장 조건
			if (int(city.gold) < 100)									return;	// 도시 금 확인
			if (int(city.public_order) == 100)							return;	// 도시 치안 확인

			// 설정 조건 확인
			BaseInfo@ base_t = @base_ex[city.get_id()];
			if (!base_t.can_inspections)	return;	// 허용/금지 여부
			if (int(city.public_order) > int(base_t.inspections_std))		return;	// 치안 기준

<<<<<<< HEAD
			if (디버깅) pk::message_box(pk::encode(pk::format("{}的巡察运行条件通过", city_name)));
=======
			if (调试模式) pk::message_box(pk::encode(pk::format("{}的巡察运行条件通过", city_name)));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

			/// 순찰 명령 정보
			pk::inspections_cmd_info cmd;

			// 순찰 실행 거점
			@cmd.base = @building;

			// 内政自动化的武将筛选，或许可以加一个特技_调练优先 순찰 실행 무장
			person_list.sort(function(a, b) { return a.stat[int(pk::core["inspection.stat"])] > b.stat[int(pk::core["inspection.stat"])]; });
			pk::list<pk::person@> actors;
			actors.add(person_list[0]);
			if (int(city.public_order) + func_巡察治安增减_计算(city, building, actors) < 100)
			{
				if (int(person_list.count) > 1)
				{
					actors.add(person_list[1]);

					if (int(city.public_order) + func_巡察治安增减_计算(city, building, actors) < 100)
					{
						if (int(person_list.count) > 2) actors.add(person_list[2]);
					}
				}
			}
			int end = int(actors.count);
			array<string> person_name_list(0);
			for (int i = 0; i < end; i++)
			{
				@cmd.actors[i] = @actors[i];
				person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(actors[i]))));
			}

			// 순찰 명령 실행
			if (!内政自动化_行动力消耗_启用)  pk::add_ap(district, 20);
			bool is_inspections_ended = pk::command(cmd);

			// 순찰 실행 메시지
			if (is_inspections_ended)
			{
				string building_name = pk::format("\x1b[1x{}\x1b[0x的", pk::decode(pk::get_name(building)));
				string person_names = join(person_name_list, ",");
				string person_count_info = (end > 1) ? "等" : "";
				string action_info = pk::format("{}名武将\n执行巡察", end);
				string action_message = building_name + person_names + person_count_info + action_info;

				if (内政自动化_对话框提示_启用) pk::message_box(pk::encode(action_message));
				else							pk::history_log(building.pos, force.color, pk::encode(action_message));
			}
		}

<<<<<<< HEAD
=======
		void func_内政自动化_执行官巡察(pk::force@ force, pk::city@ city)
		{
			pk::building@ building = pk::city_to_building(city);
			pk::district@ district = pk::get_district(building.get_district_id());
			//pk::list<pk::person@> person_list = pk::get_idle_person_list(building);

			string city_name = pk::format("顺序模式\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(city)));

			if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的巡察运行条件", city_name)));

			// 기본 조건 확인
			if (内政自动化_行动力消耗_启用 and int(district.ap) < 20)	return;	// 행동력 조건
			//if (int(person_list.count) == 0)							return;	// 행동가능 무장 조건
			if (!pk::is_valid_person_id(base_ex[city.get_id()].inspections_person)) return;
			if (int(city.gold) < 100)									return;	// 도시 금 확인
			if (int(city.public_order) == 100)							return;	// 도시 치안 확인

			// 설정 조건 확인
			BaseInfo@ base_t = @base_ex[city.get_id()];
			if (!base_t.can_inspections)	return;	// 허용/금지 여부
			if (int(city.public_order) > int(base_t.inspections_std))		return;	// 치안 기준

			if (调试模式) pk::message_box(pk::encode(pk::format("{}的巡察运行条件通过", city_name)));

			/// 순찰 명령 정보
			pk::inspections_cmd_info cmd;

			// 순찰 실행 거점
			@cmd.base = @building;


			pk::person@ actor = pk::get_person(base_ex[city.get_id()].inspections_person);

			int stat = actor.base_stat[int(pk::core["inspection.stat"])];
			//执行cmd时会判断执行官是否已行动是否外出且会判断势力，所以没法让部队中的执行官执行任务？或者用文官替代？
			if (pk::is_absent(actor) or pk::is_unitize(actor) or actor.action_done)
			{
				@actor = pk::create_bandit(actor, actor.service, actor.location);
				actor.action_done = false;
				if (actor.mibun == 身份_在野) pk::set_mibun(actor, 身份_一般);
				actor.base_stat[int(pk::core["inspection.stat"])] = uint8(stat * 0.7f);
				//pk::trace("person.mibun"+actor.mibun);
			}
			//actors.add(actor);
			array<string> person_name_list(0);
			//int end = int(actors.count);

			@cmd.actors[0] = @actor;
			person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(actor))));

			// 순찰 명령 실행
			if (!内政自动化_行动力消耗_启用)  pk::add_ap(district, 20);
			bool is_inspections_ended = pk::command(cmd);

			// 순찰 실행 메시지
			if (is_inspections_ended)
			{
				string building_name = pk::format("\x1b[1x{}\x1b[0x的", pk::decode(pk::get_name(building)));
				string person_names = join(person_name_list, ",");
				//string person_count_info = (end > 1) ? "等" : "";
				string action_info = pk::format("{}名武将\n执行巡察", 1);
				string action_message = building_name + person_names + action_info;

				if (内政自动化_对话框提示_启用) pk::message_box(pk::encode(action_message));
				else							pk::history_log(building.pos, force.color, pk::encode(action_message));
			}
			pk::reset(actor);
		}

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		// 计算是参考的104的函数，所以或许有必要直接call '104 순찰 치안 증감.cpp' 참고 (PK2.1 기본 제공 파일 기준)
		int func_巡察治安增减_计算(pk::city@ city, pk::building@ building, pk::list<pk::person@> actors)
		{
			//int porder = cast<pk::func104_t>(pk::get_func(104))(building, actors);
			//return porder;
			
			int n = 0;

			for (int i = 0; i < int(actors.count); i++) n = n + actors[i].stat[int(pk::core["inspection.stat"])];

			n = n / 28 + 2;

			if (pk::enemies_around(building)) n = n / 2;

			return n;
		}

		void func_内政自动化_训练(pk::force@ force, pk::city@ city)
		{
			pk::building@ building = pk::city_to_building(city);
			pk::district@ district = pk::get_district(building.get_district_id());
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building);
			int city_max_energy = (pk::has_tech(city, 기교_숙련병)) ? 120 : 100;

			string city_name = pk::format("城市\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(city)));

<<<<<<< HEAD
			if (디버깅) pk::message_box(pk::encode(pk::format("开始确认{}的训练运行条件", city_name)));
=======
			if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的训练运行条件", city_name)));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

			// 기본 조건 확인
			if (内政自动化_行动力消耗_启用 and int(district.ap) < 20)	return;	// 행동력 조건
			if (int(person_list.count) == 0)							return;	// 행동가능 무장 조건
			if (int(city.energy) == city_max_energy)					return;	// 도시 기력 확인

			// 설정 조건 확인
			BaseInfo@ base_t = @base_ex[city.get_id()];
			if (!base_t.can_drill)	return;	// 허용/금지 여부
<<<<<<< HEAD
			if (디버깅)
=======
			if (调试模式)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			{
				pk::message_box(pk::encode(pk::format("最大士气与当前士气的比率:{}", float(city.energy) / float(city_max_energy))));
				pk::message_box(pk::encode(pk::format("训练值与士气的比率:{}", float(base_t.drill_std) / 100.0f)));
			}
			if (float(city.energy) / float(city_max_energy) > float(base_t.drill_std) / 100.0f) return;	// 기력비율 기준

<<<<<<< HEAD
			if (디버깅) pk::message_box(pk::encode(pk::format("{}的训练运行条件通过", city_name)));
=======
			if (调试模式) pk::message_box(pk::encode(pk::format("{}的训练运行条件通过", city_name)));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

			/// 훈련 명령 정보
			pk::drill_cmd_info cmd;

			// 훈련 실행 거점
			@cmd.base = @building;

			// 훈련 실행 무장
			person_list.sort(function(a, b) { return a.stat[int(pk::core["train.stat"])] > b.stat[int(pk::core["train.stat"])]; });
			pk::list<pk::person@> actors;
			actors.add(person_list[0]);
			if (int(city.energy) + func_訓练气力增减_计算(city, building, actors) < city_max_energy)
			{
				if (int(person_list.count) > 1)
				{
					actors.add(person_list[1]);

					if (int(city.energy) + func_訓练气力增减_计算(city, building, actors) < city_max_energy)
					{
						if (int(person_list.count) > 2) actors.add(person_list[2]);
					}
				}
			}
			int end = int(actors.count);
			array<string> person_name_list(0);
			for (int i = 0; i < end; i++)
			{
				@cmd.actors[i] = @actors[i];
				person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(actors[i]))));
			}

			// 훈련 명령 실행
			if (!内政自动化_行动力消耗_启用) pk::add_ap(district, 20);
			bool is_drills_ended = pk::command(cmd);

			// 훈련 실행 메시지
			if (is_drills_ended)
			{
				string building_name = pk::format("\x1b[1x{}\x1b[0x的", pk::decode(pk::get_name(building)));
				string person_names = join(person_name_list, ",");
				string person_count_info = (end > 1) ? "等" : "";
				string action_info = pk::format("{}名武将\n执行训练", end);
				string action_message = building_name + person_names + person_count_info + action_info;

				if (内政自动化_对话框提示_启用)	pk::message_box(pk::encode(action_message));
				else							pk::history_log(building.pos, force.color, pk::encode(action_message));
			}
		}
<<<<<<< HEAD
=======
		void func_内政自动化_执行官训练(pk::force@ force, pk::city@ city)
		{
			pk::building@ building = pk::city_to_building(city);
			pk::district@ district = pk::get_district(building.get_district_id());
			//pk::list<pk::person@> person_list = pk::get_idle_person_list(building);
			int city_max_energy = (pk::has_tech(city, 기교_숙련병)) ? 150 : 120;

			string city_name = pk::format("城市\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(city)));

			if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的训练运行条件", city_name)));

			// 기본 조건 확인
			if (内政自动化_行动力消耗_启用 and int(district.ap) < 20)	return;	// 행동력 조건
			//if (int(person_list.count) == 0)							return;	// 행동가능 무장 조건
			if (!pk::is_valid_person_id(base_ex[city.get_id()].drill_person)) return;
			if (int(city.energy) == city_max_energy)					return;	// 도시 기력 확인

			// 설정 조건 확인
			BaseInfo@ base_t = @base_ex[city.get_id()];
			if (!base_t.can_drill)	return;	// 허용/금지 여부
			if (调试模式)
			{
				pk::message_box(pk::encode(pk::format("最大士气与当前士气的比率:{}", float(city.energy) / float(city_max_energy))));
				pk::message_box(pk::encode(pk::format("训练值与士气的比率:{}", float(base_t.drill_std) / 100.0f)));
			}
			if (float(city.energy) / float(city_max_energy) > float(base_t.drill_std) / 100.0f) return;	// 기력비율 기준

			if (调试模式) pk::message_box(pk::encode(pk::format("{}的训练运行条件通过", city_name)));

			/// 훈련 명령 정보
			pk::drill_cmd_info cmd;

			// 훈련 실행 거점
			@cmd.base = @building;

			// 훈련 실행 무장
			//person_list.sort(function(a, b) { return a.stat[int(pk::core["train.stat"])] > b.stat[int(pk::core["train.stat"])]; });

			//pk::list<pk::person@> actors;
			pk::person@ actor = pk::get_person(base_ex[city.get_id()].drill_person);		
				
			int stat = actor.base_stat[int(pk::core["train.stat"])];
			//执行cmd时会判断执行官是否已行动是否外出且会判断势力，所以没法让部队中的执行官执行任务？或者用文官替代？
			if (pk::is_absent(actor) or pk::is_unitize(actor) or actor.action_done)
			{
				@actor = pk::create_bandit(actor,actor.service,actor.location);
				actor.action_done = false;
				if (actor.mibun == 身份_在野) pk::set_mibun(actor, 身份_一般);
				actor.base_stat[int(pk::core["train.stat"])] = uint8(stat * 0.7f);
				//pk::trace("person.mibun"+actor.mibun);
			}
			//actors.add(actor);
			array<string> person_name_list(0);
			//int end = int(actors.count);

			@cmd.actors[0] = @actor;
			person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(actor))));
			//pk::trace("train" + cmd.actors[0].get_id() + "actor" + actor.get_id());
			// 훈련 명령 실행
			if (!内政自动化_行动力消耗_启用) pk::add_ap(district, 20);
			bool is_drills_ended = pk::command(cmd);
			//pk::trace("train" + cmd.actors[0].get_id() + "actor" + actor.get_id() + is_drills_ended);
			// 훈련 실행 메시지
			if (is_drills_ended)
			{
				if (actor.get_id() > 999) pk::reset(actor);

				string building_name = pk::format("\x1b[1x{}\x1b[0x的", pk::decode(pk::get_name(building)));
				string person_names = join(person_name_list, ",");
				//string person_count_info = (end > 1) ? "等" : "";
				string action_info = pk::format("{}名武将\n执行训练", 1);
				string action_message = building_name + person_names + action_info;

				if (内政自动化_对话框提示_启用)	pk::message_box(pk::encode(action_message));
				else							pk::history_log(building.pos, force.color, pk::encode(action_message));
			}
			pk::reset(actor);
		}
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

		// '105 훈련 기력c 증감.cpp' 참고 (PK2.1 기본 제공 파일 기준)
		int func_訓练气力增减_计算(pk::city@ city, pk::building@ building, pk::list<pk::person@> actors)
		{
			//int energy = cast<pk::func105_t>(pk::get_func(105))(building, actors);
			//return energy;
			
			int n = 0, max = pk::INT32_MIN;

			for (int i = 0; i < int(actors.count); i++)
			{
				int s = actors[i].stat[int(pk::core["train.stat"])];
				n += s;
				max = pk::max(max, s);
			}

			n = (n + max) / pk::min(building.get_troops() / 2000 + 20, 100) + 3;

			if (building.facility == 시설_도시 and pk::has_facility(city, 시설_연병소)) n = int(n * 1.5f);

			return n;
		}

<<<<<<< HEAD
=======
		void func_内政自动化_执行官生产(pk::force@ force, pk::city@ city)
		{
			pk::building@ building = pk::city_to_building(city);
			pk::district@ district = pk::get_district(building.get_district_id());
			
			//生产设定执行人，一人生产，加2个副将50能力？
			//确定3个兵器是否生产，马匹是否生产，兵器是否生产，舰船是否生产
			//生产按特产及多少来？
			array<bool> weapon_enable = base_ex[building.get_id()].can_produce;//枪戟弩马冲车井阑楼船
			//根据是否开启及兵器量来确定该生产哪个
			//array<int> weapon_count(3, -1);
			int weapon_id = -1;
			int min_weapon_count = 100000;
			if (city.blacksmith_counter > 0)
			{
				//枪戟弩生产选择
				for (int i = 0; i < 3; ++i)
				{
					if (weapon_enable[i])
					{
						int weapon_amount = pk::get_weapon_amount(building, i + 1);
						if (调试模式) pk::trace("weapon_amount" + weapon_amount);
						if (调试模式) pk::trace("city.tokusan[i]" + city.tokusan[i]);
						int weapon_count = weapon_amount / (city.tokusan[i] ? 2 : 1);
						if (weapon_count < min_weapon_count)
						{
							min_weapon_count = weapon_count;
							weapon_id = i + 1;
						}
					}
				}
			}


			//array<int> produce_weapon = weapon_id;
			//if (weapon_enable[3]) produce_weapon.insertLast(3);//马匹是否生产
			if (city.stable_counter > 0)
			{
				if (weapon_enable[3] and pk::get_weapon_amount(building, 兵器_战马) < 97000)
				{
					if (weapon_id != -1)
					{
						if (pk::rand_bool(50)) weapon_id = 兵器_战马;//如果可以生产其他兵器，50%生产战马
					}
					else  weapon_id = 兵器_战马;//如果不可以生产其他兵器，直接生产战马
				}
			}
			if (调试模式) pk::trace("city_id" + city.get_id() + "weapon_id" + weapon_id);

			if (weapon_id != -1)
			{
				//生产枪戟弩

				//pk::list<pk::person@> person_list = pk::get_idle_person_list(building);
				//int city_max_energy = (pk::has_tech(city, 기교_숙련병)) ? 120 : 100;

				string city_name = pk::format("城市\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(city)));

				if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的生产运行条件", city_name)));

				// 기본 조건 확인
				if (内政自动化_行动力消耗_启用 and int(district.ap) < 20)	return;	// 행동력 조건
				//if (int(person_list.count) == 0)							return;	// 행동가능 무장 조건
				//if (int(city.energy) == city_max_energy)					return;	// 도시 기력 확인
				if (pk::get_weapon_amount(building, weapon_id) > 99999) return;

				// 설정 조건 확인
				BaseInfo@ base_t = @base_ex[city.get_id()];
				if (!pk::is_valid_person_id(base_t.produce_person))	return;
				//if (!base_t.can_drill)	return;	// 허용/금지 여부
				if (调试模式)
				{
					pk::message_box(pk::encode(pk::format("自动生产，兵装:{}数量:{}", pk::decode(pk::get_name(pk::get_equipment(weapon_id))), pk::get_weapon_amount(building, weapon_id))));
					pk::message_box(pk::encode(pk::format("{}的生产运行条件通过", city_name)));
				}

				/// 훈련 명령 정보
				pk::outfit_cmd_info cmd;
				//int weapon = 1;//读取设定的武器
				// 훈련 실행 거점
				@cmd.base = @building;
				cmd.weapon = weapon_id;

				// 훈련 실행 무장
				//person_list.sort(function(a, b) { return a.stat[int(pk::core["weapon_produce.stat"])] > b.stat[int(pk::core["weapon_produce.stat"])]; });
				pk::person@ actor = pk::get_person(base_ex[city.get_id()].produce_person);

				int stat = actor.base_stat[int(pk::core["weapon_produce.stat"])];
				//执行cmd时会判断执行官是否已行动是否外出且会判断势力，所以没法让部队中的执行官执行任务？或者用文官替代？
				bool replace = false;
				if (pk::is_absent(actor) or pk::is_unitize(actor) or actor.action_done)
				{
					@actor = pk::create_bandit(actor, actor.service, actor.location);
					actor.action_done = false;
					if (actor.mibun == 身份_在野) pk::set_mibun(actor, 身份_一般);
					actor.base_stat[int(pk::core["weapon_produce.stat"])] = uint8(stat * 0.7f);
					//pk::trace("person.mibun"+actor.mibun);
					replace = true;
				}
				//actors.add(actor);
				array<string> person_name_list(0);
				//int end = int(actors.count);

				@cmd.actors[0] = @actor;
				person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(actor))));
			
				// 훈련 명령 실행

				if (!内政自动化_行动力消耗_启用) pk::add_ap(district, 20);//cmd有消耗行动力，所以如果设定不消耗就在此加回来
				bool is_produce_ended = pk::command(cmd);
				if (调试模式) pk::trace("is_produce_ended" + is_produce_ended);
				// 훈련 실행 메시지
				if (is_produce_ended)
				{
					if (actor.get_id() > 999) pk::reset(actor);
					string building_name = pk::format("\x1b[1x{}\x1b[0x的", pk::decode(pk::get_name(building)));
					string person_names = pk::decode(pk::get_name(actor));//join(person_name_list, ",");
					//string person_count_info = (end > 1) ? "等" : "";
					string action_info = pk::format("{}名武将\n执行生产{}", 1, pk::decode(pk::get_name(pk::get_equipment(weapon_id))));
					string action_message = building_name + person_names + action_info;

					if (内政自动化_对话框提示_启用)	pk::message_box(pk::encode(action_message));
					else							pk::history_log(building.pos, force.color, pk::encode(action_message));
				}
				if (replace) pk::reset(actor);
			}

		}


>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		void func_内政自动化_征兵(pk::force@ force, pk::city@ city)
		{
			pk::building@ building = pk::city_to_building(city);
			pk::district@ district = pk::get_district(building.get_district_id());
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building);

			string city_name = pk::format("城市\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(city)));

<<<<<<< HEAD
			if (디버깅) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件", city_name)));


			// 기본 조건 확인
			//if (디버깅) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--行动力", city_name)));
			if (内政自动化_行动力消耗_启用 and int(district.ap) < 20)	return;	// 행동력 조건
			//if (디버깅) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--人数", city_name)));
			if (int(person_list.count) == 0)							return;	// 행동가능 무장 조건
			//if (디버깅) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--金", city_name)));
			if (int(city.gold) < 300)									return;	// 도시 금 확인
			if (디버깅) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--建筑", city_name)));
			if (!pk::has_facility(city, 시설_병영) and !pk::has_facility(city, 시설_병영2단) and !pk::has_facility(city, 시설_병영3단)) return;	// 도시 시설 확인
			if (디버깅) pk::message_box(pk::encode(pk::format("{}的征兵运行条件--建筑通过", city_name)));
			// 설정 조건 확인
			BaseInfo@ base_t = @base_ex[city.get_id()];
			if (디버깅) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--是否允许征兵：{}", city_name, base_t.can_recruit)));
			if (!base_t.can_recruit)	return;	// 허용/금지 여부
			if (디버깅) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--治安：{}", city_name, int(base_t.recruit_std_porder))));
			if (int(city.public_order) < int(base_t.recruit_std_porder))		return;	// 치안 기준
			if (디버깅) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--金：{}", city_name, int(base_t.recruit_std_gold))));
			if (int(city.gold) < int(base_t.recruit_std_gold))				return;	// 금 기준
			if (디버깅) pk::message_box(pk::encode(pk::format("{}的征兵运行条件--资金通过", city_name)));
			if (디버깅)
=======
			if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件", city_name)));


			// 기본 조건 확인
			//if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--行动力", city_name)));
			if (内政自动化_行动力消耗_启用 and int(district.ap) < 20)	return;	// 행동력 조건
			//if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--人数", city_name)));
			if (int(person_list.count) == 0)							return;	// 행동가능 무장 조건
			//if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--金", city_name)));
			if (int(city.gold) < 300)									return;	// 도시 금 확인
			if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--建筑", city_name)));
			if (!pk::has_facility(city, 시설_병영) and !pk::has_facility(city, 시설_병영2단) and !pk::has_facility(city, 시설_병영3단)) return;	// 도시 시설 확인
			if (调试模式) pk::message_box(pk::encode(pk::format("{}的征兵运行条件--建筑通过", city_name)));
			// 설정 조건 확인
			BaseInfo@ base_t = @base_ex[city.get_id()];
			if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--是否允许征兵：{}", city_name, base_t.can_recruit)));
			if (!base_t.can_recruit)	return;	// 허용/금지 여부
			if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--治安：{}", city_name, int(base_t.recruit_std_porder))));
			if (int(city.public_order) < int(base_t.recruit_std_porder))		return;	// 치안 기준
			if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件--金：{}", city_name, int(base_t.recruit_std_gold))));
			if (int(city.gold) < int(base_t.recruit_std_gold))				return;	// 금 기준
			if (调试模式) pk::message_box(pk::encode(pk::format("{}的征兵运行条件--资金通过", city_name)));
			if (调试模式)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			{
				pk::message_box(pk::encode(pk::format("当前兵粮与当前兵力的比率:{}", float(city.food) / float(city.troops))));
				pk::message_box(pk::encode(pk::format("征兵值与粮兵的比率:{}", float(base_t.recruit_std_foodtroop) / 100.0f)));
			}
			if (float(city.food) / float(city.troops) < float(base_t.recruit_std_foodtroop) / 100.0f) return;	// 병량비율 기준

<<<<<<< HEAD
			if (디버깅) pk::message_box(pk::encode(pk::format("{}的征兵运行条件通过", city_name)));
=======
			if (调试模式) pk::message_box(pk::encode(pk::format("{}的征兵运行条件通过", city_name)));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

			/// 징병 명령 정보
			pk::recruit_cmd_info cmd;

			// 징병 실행 거점
			@cmd.base = @building;


/**/
			// 这是对征兵特技进行处理 징병 실행 무장
			pk::list<pk::person@> actors;
<<<<<<< HEAD
			if (디버깅) pk::message_box(pk::encode(pk::format("{}，建筑id：{}，征兵能力：{}，征兵特技：{}", city_name, building.get_id(), int(pk::core["recruit.stat"]), int(pk::core["recruit.skill"]))));
			
			pk::list<pk::person@> person_list0 = get_cmd_actors(building, int(pk::core["recruit.stat"]), int(pk::core["recruit.skill"]));
			//if (디버깅) pk::message_box(pk::encode(pk::format("数组1：{}，数组2：{}，数组3：{}", person_array[0].get_id(), person_array[1].get_id(), person_array[2].get_id())));

			//pk::list<pk::person@> person_list0 = pk::array_to_list(person_array);
			person_list = person_list0;
			if (디버깅) pk::message_box(pk::encode(pk::format("{}的征兵cmd运行条件通过", city_name)));
=======
			if (调试模式) pk::message_box(pk::encode(pk::format("{}，建筑id：{}，征兵能力：{}，征兵特技：{}", city_name, building.get_id(), int(pk::core["recruit.stat"]), int(pk::core["recruit.skill"]))));
			
			pk::list<pk::person@> person_list0 = get_cmd_actors(building, int(pk::core["recruit.stat"]), int(pk::core["recruit.skill"]));
			//if (调试模式) pk::message_box(pk::encode(pk::format("数组1：{}，数组2：{}，数组3：{}", person_array[0].get_id(), person_array[1].get_id(), person_array[2].get_id())));

			//pk::list<pk::person@> person_list0 = pk::array_to_list(person_array);
			person_list = person_list0;
			if (调试模式) pk::message_box(pk::encode(pk::format("{}的征兵cmd运行条件通过", city_name)));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			actors.add(person_list[0]);
			if (int(city.troops) + func_征兵兵力增减_计算(city, building, actors) < int(city.max_troops))
			{
				if (int(person_list.count) > 1)
				{
					actors.add(person_list[1]);

					if (int(city.troops) + func_征兵兵力增减_计算(city, building, actors) < int(city.max_troops))
					{
						if (int(person_list.count) > 2) actors.add(person_list[2]);
					}
				}
			}



			int end = int(actors.count);
			array<string> person_name_list(0);
			for (int i = 0; i < end; i++)
			{
				@cmd.actors[i] = @actors[i];
				person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(actors[i]))));
			}

			// 징병 명령 실행
			if (!内政自动化_行动力消耗_启用)  pk::add_ap(district, 20);
			bool is_recruit_ended = pk::command(cmd);

			// 징병 실행 메시지
			if (is_recruit_ended)
			{
				string building_name = pk::format("\x1b[1x{}\x1b[0x的", pk::decode(pk::get_name(building)));
				string person_names = join(person_name_list, ",");
				string person_count_info = (end > 1) ? "等" : "";
				string action_info = pk::format("{}名武将\n执行征兵", end);
				string action_message = building_name + person_names + person_count_info + action_info;

				if (内政自动化_对话框提示_启用)	pk::message_box(pk::encode(action_message));
				else							pk::history_log(building.pos, force.color, pk::encode(action_message));
			}
		}

<<<<<<< HEAD
=======
		void func_内政自动化_执行官征兵(pk::force@ force, pk::city@ city)
		{
			pk::building@ building = pk::city_to_building(city);
			pk::district@ district = pk::get_district(building.get_district_id());
			//pk::list<pk::person@> person_list = pk::get_idle_person_list(building);

			string city_name = pk::format("城市\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(city)));

			if (调试模式) pk::message_box(pk::encode(pk::format("开始确认{}的征兵运行条件", city_name)));


			// 기본 조건 확인
			if (内政自动化_行动力消耗_启用 and int(district.ap) < 20)	return;	// 행동력 조건
			//if (int(person_list.count) == 0)							return;	// 행동가능 무장 조건

			if (int(city.gold) < 300)									return;	// 도시 금 확인
			if (!pk::has_facility(city, 시설_병영) and !pk::has_facility(city, 시설_병영2단) and !pk::has_facility(city, 시설_병영3단)) return;	// 도시 시설 확인
			// 설정 조건 확인
			if (!pk::is_valid_person_id(base_ex[city.get_id()].recruit_person)) return;
			BaseInfo@ base_t = @base_ex[city.get_id()];
			if (!base_t.can_recruit)	return;	// 허용/금지 여부
			if (int(city.public_order) < int(base_t.recruit_std_porder))		return;	// 치안 기준
			if (int(city.gold) < int(base_t.recruit_std_gold))				return;	// 금 기준
			if (调试模式)
			{
				pk::message_box(pk::encode(pk::format("当前兵粮与当前兵力的比率:{}", float(city.food) / float(city.troops))));
				pk::message_box(pk::encode(pk::format("征兵值与粮兵的比率:{}", float(base_t.recruit_std_foodtroop) / 100.0f)));
			}
			if (float(city.food) / float(city.troops) < float(base_t.recruit_std_foodtroop) / 100.0f) return;	// 병량비율 기준

			if (调试模式) pk::message_box(pk::encode(pk::format("{}的征兵运行条件通过", city_name)));

			/// 징병 명령 정보
			pk::recruit_cmd_info cmd;

			// 징병 실행 거점
			@cmd.base = @building;


			pk::person@ actor = pk::get_person(base_ex[city.get_id()].recruit_person);

			int stat = actor.base_stat[int(pk::core["recruit.stat"])];
			//执行cmd时会判断执行官是否已行动是否外出且会判断势力，所以没法让部队中的执行官执行任务？或者用文官替代？
			if (pk::is_absent(actor) or pk::is_unitize(actor) or actor.action_done)
			{
				@actor = pk::create_bandit(actor, actor.service, actor.location);
				actor.action_done = false;
				if (actor.mibun == 身份_在野) pk::set_mibun(actor, 身份_一般);
				actor.base_stat[int(pk::core["recruit.stat"])] = uint8(stat * 0.7f);
				//pk::trace("person.mibun"+actor.mibun);
			}
			//actors.add(actor);
			array<string> person_name_list(0);
			//int end = int(actors.count);

			@cmd.actors[0] = @actor;
			person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(actor))));

			// 징병 명령 실행
			if (!内政自动化_行动力消耗_启用)  pk::add_ap(district, 20);
			bool is_recruit_ended = pk::command(cmd);
			//pk::trace("recruit" + cmd.actors[0].get_id() + is_recruit_ended);
			// 징병 실행 메시지
			if (is_recruit_ended)
			{
				if (actor.get_id() > 999) pk::reset(actor);
				string building_name = pk::format("\x1b[1x{}\x1b[0x的", pk::decode(pk::get_name(building)));
				string person_names = join(person_name_list, ",");
				//string person_count_info = (end > 1) ? "等" : "";
				string action_info = pk::format("{}名武将\n执行征兵", 1);
				string action_message = building_name + person_names + action_info;

				if (内政自动化_对话框提示_启用)	pk::message_box(pk::encode(action_message));
				else							pk::history_log(building.pos, force.color, pk::encode(action_message));
			}
			pk::reset(actor);
		}

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		// '100 징병 병력 증감.cpp' 참고 (PK2.1 기본 제공 파일 기준)
		int func_征兵兵力增减_计算(pk::city@ city, pk::building@ building, pk::list<pk::person@> actors)
		{
			//int troops = cast<pk::func100_t>(pk::get_func(100))(building, actors);
			//return troops;
			int n = 0, sum = 0, mul = 100;

			for (int i = 0; i < int(actors.count); i++)
			{
				sum += actors[i].stat[int(pk::core["recruit.stat"])];
				if (ch::has_skill(actors[i], int(pk::core["recruit.skill"]))) mul = 150;
			}

			n = int((1000 + (int(city.public_order) + 20) * sum * 5 / 100) * mul / 100 * func_5c4600(city));

			if (pk::get_scenario().difficulty == 难易度_超级 and !city.is_player()) n *= 2;

			if (pk::enemies_around(building)) n /= 2;

			return n;
		}

		float func_5c4600(pk::city@ city)
		{
			int level1 = 0, level2 = 0;

			for (int i = 0; i < city.max_devs; i++)
			{
				pk::building@ building = city.dev[i].building;
				if (pk::is_alive(building))
				{
					switch (building.facility)
					{
					case 시설_병영: building.completed ? level1++ : 0;		break;
					case 시설_병영2단: building.completed ? level2++ : level1++;	break;
					case 시설_병영3단: building.completed ? 0 : level2++;	break;
					}
				}
			}

			if (int(city.barracks_counter) > level1 + level2)	return 1.5f;
			if (int(city.barracks_counter) > level1)			return 1.2f;
			return 1.f;
		}

		int cmd_stat;
		int cmd_skill;
		pk::list<pk::person@> get_cmd_actors(pk::building@ base, int stat, int skill)
		{
			cmd_stat = stat;
			cmd_skill = skill;
			pk::list<pk::person@> actors;

			// 실행가능 무장 확인
			pk::list<pk::person@> list_idle_person = pk::get_idle_person_list(base);
			if (list_idle_person.count == 0) return actors;
<<<<<<< HEAD
			if (디버깅) pk::message_box(pk::encode(pk::format("排序前，目标数量不为0", 1)));
=======
			if (调试模式) pk::message_box(pk::encode(pk::format("排序前，目标数量不为0", 1)));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			// 능력 높은 순으로 정렬, 특기 우대
			list_idle_person.sort(function(a, b)
			{
				if (main.cmd_skill >= 0)    // 특기 보유 시
				{
					bool a_skill = ch::has_skill(a, main.cmd_skill);
					bool b_skill = ch::has_skill(b, main.cmd_skill);
					if (a_skill and !b_skill) return true;
					if (!a_skill and b_skill) return false;

					return a.stat[main.cmd_stat] > b.stat[main.cmd_stat];
				}

				return a.stat[main.cmd_stat] > b.stat[main.cmd_stat];
			});
<<<<<<< HEAD
			//if (디버깅) pk::message_box(pk::encode(pk::format("排序后，数组1：{}，数组2：{}，数组3：{}", list_idle_person[0].get_id(), list_idle_person[1].get_id(), list_idle_person[2].get_id())));
			if (디버깅) pk::message_box(pk::encode(pk::format("排序后，数组1：{}", list_idle_person[0].get_id())));
=======
			//if (调试模式) pk::message_box(pk::encode(pk::format("排序后，数组1：{}，数组2：{}，数组3：{}", list_idle_person[0].get_id(), list_idle_person[1].get_id(), list_idle_person[2].get_id())));
			if (调试模式) pk::message_box(pk::encode(pk::format("排序后，数组1：{}", list_idle_person[0].get_id())));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

			// 最多三人 최대 3명까지
			//pk::array<pk::person@> arr_idle_person = pk::list_to_array(list_idle_person);
			//int min_actors = pk::min(3, pk::max(1, int(arr_idle_person.length) / 2));
			int min_actors = pk::min(3, list_idle_person.count);
			//int sum_stat = 0;
<<<<<<< HEAD
			if (디버깅) pk::message_box(pk::encode(pk::format("赋值前，数组长度：{}", list_idle_person.count)));
=======
			if (调试模式) pk::message_box(pk::encode(pk::format("赋值前，数组长度：{}", list_idle_person.count)));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			for (int i = 0; i < min_actors; i++)
			{
				actors.add(list_idle_person[i]);
			}

			return actors;
		}

<<<<<<< HEAD

=======
		pk::list<pk::person@> sort_person_list(pk::building@ base, int stat, int skill)
		{
			cmd_stat = stat;
			cmd_skill = skill;
			pk::list<pk::person@> actors;

			// 실행가능 무장 확인
			pk::list<pk::person@> list_person = pk::get_person_list(base,pk::mibun_flags(身份_君主,身份_都督, 身份_太守,身份_一般));
			if (list_person.count == 0) return actors;

			//需要排除其他已经设定过的执行者

			if (调试模式) pk::message_box(pk::encode(pk::format("排序前，目标数量不为0", 1)));
			// 능력 높은 순으로 정렬, 특기 우대
			list_person.sort(function(a, b)
			{
				if (main.cmd_skill >= 0)    // 특기 보유 시
				{
					bool a_skill = ch::has_skill(a, main.cmd_skill);
					bool b_skill = ch::has_skill(b, main.cmd_skill);
					if (a_skill and !b_skill) return true;
					if (!a_skill and b_skill) return false;

					return a.stat[main.cmd_stat] > b.stat[main.cmd_stat];
				}

				return a.stat[main.cmd_stat] > b.stat[main.cmd_stat];
			});
			//if (调试模式) pk::message_box(pk::encode(pk::format("排序后，数组1：{}，数组2：{}，数组3：{}", list_idle_person[0].get_id(), list_idle_person[1].get_id(), list_idle_person[2].get_id())));
			if (调试模式) pk::message_box(pk::encode(pk::format("排序后，数组1：{}", list_person[0].get_id())));

			// 最多三人 최대 3명까지
			//pk::array<pk::person@> arr_idle_person = pk::list_to_array(list_idle_person);
			//int min_actors = pk::min(3, pk::max(1, int(arr_idle_person.length) / 2));

			return list_person;
		}
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

		void func_内政自动化_褒赏(pk::force@ force, pk::city@ city)
		{
			pk::building@ building = pk::city_to_building(city);
			pk::district@ district = pk::get_district(building.get_district_id());

			// 설정 조건 확인
			BaseInfo@ base_t = @base_ex[city.get_id()];
			if (!base_t.can_reward) return;	// 허용/금지 여부

			/// 포상 명령 정보
			pk::reward_cmd_info cmd;

			// 포상 실행 거점
			@cmd.base = @building;

			// 포상 무장
			int recover_ap = 0;
			uint mibun_flags = pk::mibun_flags(身份_都督, 身份_太守, 身份_一般);
			array<pk::person@> person_list = pk::list_to_array(pk::get_person_list(force, mibun_flags));
			for (int i = 0; i < int(person_list.length); i++)
			{
				pk::person@ person = person_list[i];

				// 기본 조건 확인
				if (内政自动化_行动力消耗_启用 and int(district.ap) < (5 * (1 + int((cmd.actors).count))))	break;	// 행동력 조건
				if (city.gold < (100 * (1 + int((cmd.actors).count))))										break;	// 도시 금 확인

				// 此处可能出错 설정 조건 확인
				if (int(person.loyalty) > int(base_t.reward_std)) continue;	// 충성도 기준

				if (!内政自动化_行动力消耗_启用) recover_ap += 5;

				cmd.actors.add(person);
			}

			// 포상 명령 실행
			if (!内政自动化_行动力消耗_启用)  pk::add_ap(district, recover_ap); 
			bool is_reward_ended = pk::command(cmd);

			// 포상 실행 메시지
			if (is_reward_ended)
			{
				int reward_count = int((cmd.actors).count);
				string building_name = pk::decode(pk::get_name(building));
				string action_info = pk::format("对\x1b[1x{}\x1b[0x的\x1b[1x{}\x1b[0x名武将褒奖", building_name, reward_count);

				if (内政自动化_对话框提示_启用)	pk::message_box(pk::encode(action_info));
				else							pk::history_log(building.pos, force.color, pk::encode(action_info));
			}
		}

		void func_内政自动化_运输(pk::force@ force, pk::city@ city)
		{
			pk::building@ building = pk::city_to_building(city);
			pk::district@ district = pk::get_district(building.get_district_id());

			// 기본 조건 확인
			if (内政自动化_行动力消耗_启用 and int(district.ap) < 10)	return;	// 행동력 조건
			if (pk::get_idle_person_list(building).count == 0)			return;	// 행동가능 무장 조건

			// 설정 조건 확인
			BaseInfo@ base_t = @base_ex[city.get_id()];
			if (!base_t.can_transport) return;	// 허용/금지 여부

			// 수송 한계 도시 조건 (수송하는 도시)
			if (运输限制设置_到边境城市)
			{
				if (func_附近_敌城市数(city, 2) > 0) return;
			}
			else
			{
				if (func_附近_敌城市数(city, 1) > 0) return;
			}

			// 인접 도시 확인
			for (int i = 0; i < 6; i++)
			{
				pk::city@ neighbor_city = pk::get_city(city.neighbor[i]);
				if (neighbor_city is null)							continue;	// 비어 있는 인접 도시 번호인 경우 제외
				if (!pk::is_alive(neighbor_city))					continue;
				if (neighbor_city.get_force_id() != force.get_id()) continue;	// 인접 도시가 해당 세력이 아닌 경우 제외

				pk::building@ neighbor_building = pk::city_to_building(neighbor_city);

				// 기본 조건 확인
				if (内政自动化_行动力消耗_启用 and int(district.ap) < 10)	return;	// 행동력 조건
				if (pk::get_idle_person_list(building).count == 0)			return;	// 행동가능 무장 조건

				// 병력 조건 (수송하는 도시)
				if (city.troops < int(city.max_troops * 可运输条件_兵力比例)) return;

				// 병량 조건 (수송하는 도시)
				if (city.food < int(city.troops * 可运输条件_兵粮比例)) return;

				// 수송 한계 도시 조건 (수송받는 도시)
				if (运输限制设置_到边境城市)
				{
					if (func_附近_敌城市数(neighbor_city, 1) > 0) continue;
				}

				// 플레이어 위임군단 도시 제외 (수송받는 도시)
				if (neighbor_building.is_player() and !pk::is_player_controlled(neighbor_building)) continue;

				// 병력 조건 (수송받는 도시)
				if (neighbor_city.troops > int(neighbor_city.max_troops * 可运输条件_兵力条件1)) continue;

				// 병력 조건 (수송하는 도시 vs 수송받는 도시)
				if (neighbor_city.troops > int(city.troops * 可运输条件_兵力条件2)) continue;

				// 근처 적 도시수 조건  (수송하는 도시 vs 수송받는 도시)
				if (!func_附近_敌城市数_条件_是否满足(city, neighbor_city)) continue;

				if (!内政自动化_行动力消耗_启用)  pk::add_ap(district, 10); 

				func_运输部队出征(building, neighbor_building);
			}
		}

		void func_运输部队出征(pk::building@ building, pk::building@ neighbor_building)
		{
			/// 출진 명령 정보
			pk::com_deploy_cmd_info cmd;

			// 출진 거점
			@cmd.base = @building;

			// 출진 부대 종류
<<<<<<< HEAD
			cmd.type = 부대종류_수송;
=======
			cmd.type = 部队类型_运输;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

			// 출진 부대 주장
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building);
			person_list.sort(function(a, b) { return a.stat[武将能力_政治] > b.stat[武将能力_政治]; });
			cmd.member[0] = person_list[0].get_id();

			// 출진 부대 금
			int 수송량_금_최대치 = pk::min(pk::get_gold(building), pk::get_max_gold(neighbor_building) - pk::get_gold(neighbor_building));
			cmd.gold = (pk::get_gold(building) >= 10000) ? pk::min(수송량_금_최대치, (pk::get_gold(building) / 2)) : (pk::get_gold(building) / 4);

			// 출진 부대 병력
			int 수송량_병력_최대치 = pk::min(pk::get_troops(building), pk::get_max_troops(neighbor_building) - pk::get_troops(neighbor_building));
			수송량_병력_최대치 = pk::min(60000, 수송량_병력_최대치);
			int 수송량_병력 = pk::min(수송량_병력_최대치, int(pk::get_troops(building) * 输送兵力比率));
			cmd.troops = 수송량_병력;

			// 출진 부대 병량
			float 병량계수 = (pk::get_food(building) >= int(pk::get_troops(building) * 2.0f)) ? 3.0f : 1.5f;
			int 수송량_병량_최대치 = pk::min(pk::get_food(building), pk::get_max_food(neighbor_building) - pk::get_food(neighbor_building));
			cmd.food = (pk::get_food(building) >= 100000) ? pk::max((pk::get_food(building) / 2), pk::min(수송량_병량_최대치, int(수송량_병력 * 병량계수))) : (pk::get_food(building) / 2);

			// 출진 부대 병기(수송량)
			// 참고 : 0검, 1창, 2극, 3노, 4군마, 5충차, 6정란, 7투석, 8목수, 9주가, 10누선, 11투함
			array<int> 수송량_병기(12, 0);
			for (int i = 0; i < 병기_끝; i++)
			{
				int 수송량_병기_최대치 = pk::get_max_weapon_amount(neighbor_building, i) - pk::get_weapon_amount(neighbor_building, i);
				수송량_병기[i] = pk::min(수송량_병기_최대치, pk::get_weapon_amount(building, i) / 2);
				cmd.weapon_id[i] = i;
				cmd.weapon_amount[i] = 수송량_병기[i];
			}

			// 출진 부대 임무
			cmd.order = 部队任务_移动;

			// 출진 부대 목표 좌표
			cmd.target_pos = neighbor_building.pos;

			// 출진 명령 실행
			int unit_id = pk::command(cmd);

			// 출진 실행 메시지
			if (unit_id != -1)
			{
				pk::unit@ unit = pk::get_unit(unit_id);
				string unit_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(unit)));
				string building_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(building)));
				string neighbor_building_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(neighbor_building)));
				string action_message = pk::format("将{}从{}运输到{}", unit_name, building_name, neighbor_building_name);
				pk::force@ force = pk::get_force(building.get_force_id());

				if (内政自动化_对话框提示_启用)	pk::message_box(pk::encode(action_message));
				else							pk::history_log(building.pos, force.color, pk::encode(action_message));

				// 출진 부대 행동 완료
				unit.action_done = true;
			}
		}

		bool func_附近_敌城市数_条件_是否满足(pk::city@ city, pk::city@ neighbor_city)
		{
			if (func_附近_敌城市数(city, 1) < func_附近_敌城市数(neighbor_city, 1)) return true;
			else if (func_附近_敌城市数(city, 1) > func_附近_敌城市数(neighbor_city, 1)) return false;
			else
			{
				if (func_附近_敌城市数(city, 2) < func_附近_敌城市数(neighbor_city, 2)) return true;
				else if (func_附近_敌城市数(city, 2) > func_附近_敌城市数(neighbor_city, 2)) return false;
				else
				{
					if (func_附近_敌城市数(city, 3) < func_附近_敌城市数(neighbor_city, 3)) return true;
					else if (func_附近_敌城市数(city, 3) > func_附近_敌城市数(neighbor_city, 3)) return false;
					else
					{
						if (func_附近_敌城市数(city, 4) < func_附近_敌城市数(neighbor_city, 4)) return true;
						else if (func_附近_敌城市数(city, 4) > func_附近_敌城市数(neighbor_city, 4)) return false;
						else
						{
							if (func_附近_敌城市数(city, 5) < func_附近_敌城市数(neighbor_city, 5)) return true;
							else if (func_附近_敌城市数(city, 5) > func_附近_敌城市数(neighbor_city, 5)) return false;
							else
							{
								if (func_附近_敌城市数(city, 6) < func_附近_敌城市数(neighbor_city, 6)) return true;
								else if (func_附近_敌城市数(city, 6) > func_附近_敌城市数(neighbor_city, 6)) return false;
								else
								{
									if (func_附近_敌城市数(city, 7) < func_附近_敌城市数(neighbor_city, 7)) return true;
									else if (func_附近_敌城市数(city, 7) > func_附近_敌城市数(neighbor_city, 7)) return false;
									else
									{
										if (func_附近_敌城市数(city, 8) < func_附近_敌城市数(neighbor_city, 8)) return true;
										else																	return false;
									}
								}
							}
						}
					}
				}
			}
		}

		int func_附近_敌城市数(pk::city@ city, int 도시거리)
		{
			int enemy_city_count = 0;

			array<pk::city@> cities = pk::list_to_array(pk::get_city_list());
			for (int i = 0; i < int(cities.length); i++)
			{
				pk::city@ neighbor_city = cities[i];

				// 도시 간 무장의 이동 소요일수 기준
				int distance = pk::get_city_distance(city.get_id(), neighbor_city.get_id());

				if (distance > 도시거리)				continue;
				if (!pk::is_enemy(city, neighbor_city))	continue;

				enemy_city_count++;
			}

			return enemy_city_count;
		}

<<<<<<< HEAD
		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
=======
		///	*************************************菜单相关***********************************************************
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

		int menu_city_id_;	// 건물 메뉴 도시
		int menu_force_id_;	// 건물 메뉴 세력
		pk::building@ building_;

		void init(pk::building@ building)
		{
			@building_ = @building;
			pk::city@ city = pk::building_to_city(building);

			menu_city_id_ = (city !is null) ? city.get_id() : -1;
			menu_force_id_ = building.get_force_id();
		}


		int getImageID_1249()   // 주황색
		{
			return 1249;
		}



		bool isVisible()
		{
			if (building_.get_id() >= 城市_末) return false;
			if (!pk::is_player_controlled(pk::get_city(menu_city_id_))) return false;	// 플레이어 위임군단 도시인 경우 제외

			return menu_city_id_ != -1 and menu_force_id_ == pk::get_current_turn_force_id();
		}

		string getText_统一变更_顺序()
		{
			return pk::encode("批量更改");
		}
		string getDesc_统一变更_顺序()
		{
<<<<<<< HEAD
			return pk::encode("在所有城市中,统一更改所选城市的内定顺序模式.");
=======
			return pk::encode("在所有城市中,统一更改所选城市的内政顺序模式.");
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		}
		bool handler_统一变更_顺序()
		{
			func_统一变更_顺序(); return true;
		}
		void func_统一变更_顺序()
		{
			pk::district@ district = pk::get_district(pk::get_district_id(pk::get_force(menu_force_id_), 1));
			array<pk::building@> selected_cities_arr;
			pk::list<pk::city@> city_list = pk::get_city_list(district);
			string desc_顺序 = pk::encode("选择要改变太守自治方针顺序模式的城市.");
			selected_cities_arr = pk::list_to_array(pk::city_selector(pk::encode("选择城市"), desc_顺序, city_list, 1, int(city_list.count)));
			if (int(selected_cities_arr.length) == 0) return;
			array<string> str_order = { "模式1:巡察-训练-征兵-褒奖-运输", "模式2:巡察-征兵-训练-褒奖-运输", "模式3:征兵-巡察-训练-褒奖-运输", "模式4:征兵-训练-巡察-褒奖-运输", "模式5:训练-征兵-巡察-褒奖-运输", "模式6:训练-巡察-征兵-褒奖-运输" };
			pk::message_box(pk::encode(str_order[0] + "\n" + str_order[1] + "\n" + str_order[2] + "\n" + str_order[3] + "\n" + str_order[4] + "\n" + str_order[5]));
			pk::int_bool set_order = pk::numberpad(pk::encode("顺序模式"), 1, 6, 1, pk::numberpad_t(numberpad_t));
			array<string> city_name_list(0);
			for (int i = 0; i < int(selected_cities_arr.length); i++)
			{
				//	pk::city@ city = pk::building_to_city(selected_cities_arr[i]);	// pk::building_to_city 함수 지원하지 않음
				pk::city@ city = pk::get_city(selected_cities_arr[i].get_id());
				city_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(city))));
				if (set_order.second)
				{
					BaseInfo@ base_t = @base_ex[city.get_id()];
					base_t.auto_sequence = uint8(set_order.first);
					
					//内政自动化_顺序情报[city.get_id()] = set_order.first;
				}
			}
			string city_names = join(city_name_list, ",");
			string city_count_info = (int(selected_cities_arr.length) > 1) ? "等" : "";
			pk::message_box(pk::encode(pk::format("更改城市{}{}{}个自治方针顺序模式\n{}", city_names, city_count_info, int(selected_cities_arr.length), str_order[int(set_order.first) - 1])));
		}

		string getText_顺序()
		{
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			switch (base_t.auto_sequence)
			{
			case 1: return pk::encode("顺序模式:1");
			case 2: return pk::encode("顺序模式:2");
			case 3: return pk::encode("顺序模式:3");
			case 4: return pk::encode("顺序模式:4");
			case 5: return pk::encode("顺序模式:5");
			case 6: return pk::encode("顺序模式:6");
			}
			return pk::encode("");
		}

		string getDesc_顺序()
		{
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			switch (base_t.auto_sequence)
			{
			case 1: return pk::encode("顺序:巡察-训练-征兵-褒奖-运输");
			case 2: return pk::encode("顺序:巡察-征兵-训练-褒奖-运输");
			case 3: return pk::encode("顺序:征兵-巡察-训练-褒奖-运输");
			case 4: return pk::encode("顺序:征兵-训练-巡察-褒奖-运输");
			case 5: return pk::encode("顺序:训练-征兵-巡察-褒奖-运输");
			case 6: return pk::encode("顺序:训练-巡察-征兵-褒奖-运输");
			}
			return pk::encode("");
		}

		bool handler_顺序()
		{
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			if (base_t.auto_sequence == 1)	base_t.auto_sequence = 2;
			else if (base_t.auto_sequence == 2)	base_t.auto_sequence = 3;
			else if (base_t.auto_sequence == 3)	base_t.auto_sequence = 4;
			else if (base_t.auto_sequence == 4)	base_t.auto_sequence = 5;
			else if (base_t.auto_sequence == 5)	base_t.auto_sequence = 6;
			else if (base_t.auto_sequence == 6)	base_t.auto_sequence = 1;
			else												base_t.auto_sequence = 1;
			

			if (menu_force_id_ == pk::get_current_turn_force_id())
			{
				string city_name = pk::decode(pk::get_name(pk::city_to_building(pk::get_city(menu_city_id_))));
				string toggle_msg = pk::encode(pk::format("\x1b[1x{}\x1b[0x的自治方针变更(模式\x1b[1x{}\x1b[0x)\n", city_name, base_t.auto_sequence));
				string order_info = getDesc_顺序();
				pk::message_box(toggle_msg + order_info);
			}

			return true;
		}

		string getText_统一变更_许可()
		{
			return pk::encode("批量更改");
		}
		string getDesc_统一变更_许可()
		{
			return pk::encode("允许/禁止 批量更改所有城市中的所选城市.");
		}
		bool handler_统一变更_许可()
		{
			func_统一变更_许可(); return true;
		}
		void func_统一变更_许可()
		{
			pk::district@ district = pk::get_district(pk::get_district_id(pk::get_force(menu_force_id_), 1));
			array<pk::building@> selected_cities_arr;
			pk::list<pk::city@> city_list = pk::get_city_list(district);
			array<string> answers_项目_arr = { pk::encode("巡察"), pk::encode("训练"), pk::encode("征兵"), pk::encode("褒奖"), pk::encode("运输"), pk::encode("取消") };
			array<string> answers_许可_arr = { pk::encode("批量允许"), pk::encode("批量禁止"), pk::encode("返回") };
			array<string> desc_许可_arr = { "允许", "禁止" };
			string desc_许可 = "";
			int n = pk::choose(pk::encode("选择要批量修改的项目."), answers_项目_arr);
			for (int i = 0; i < 5; i++)
			{
				if (n != i) continue;

				int m = pk::choose(pk::encode("请选择是否允许/禁止."), answers_许可_arr);
				for (int j = 0; j < 2; j++)
				{
					if (m != j) continue;

					desc_许可 = pk::format("选择的城市{}为{}.", pk::decode(answers_项目_arr[n]), desc_许可_arr[m]);
					selected_cities_arr = pk::list_to_array(pk::city_selector(pk::encode("选择城市"), pk::encode(desc_许可), city_list, 1, int(city_list.count)));
					if (int(selected_cities_arr.length) == 0) return;
					array<string> city_name_list(0);
					for (int k = 0; k < int(selected_cities_arr.length); k++)
					{
						//	pk::city@ city = pk::building_to_city(selected_cities_arr[k]);	// pk::building_to_city 함수 지원하지 않음
						pk::city@ city = pk::get_city(selected_cities_arr[k].get_id());
						city_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(city))));
						bool 是否允许禁止 = (m == 0) ? true : false;
						
						set( city.get_id(), 是否允许禁止, n);
						//set(menu_force_id_, city.get_id(), 是否允许禁止, n);	// true : 금지 -> 허용, false : 허용 -> 금지
					}
					string city_names = join(city_name_list, ", ");
					string city_count_info = (int(selected_cities_arr.length) > 1) ? "等" : "";
					pk::message_box(pk::encode(pk::format("{}{}{}个城市的\x1b[1x{}\x1b[0x\x1b[1x{}\x1b[0x", city_names, city_count_info, int(selected_cities_arr.length), pk::decode(answers_项目_arr[n]), desc_许可_arr[m])));
				}
				if (m == 2)	func_统一变更_许可();
			}
			if (n == 5) return;
		}

		string getText_巡察()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_巡察) ? pk::encode("允许巡察") : pk::encode("禁止巡察");
		}
		string getDesc_巡察()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_巡察) ? pk::encode("是否允许城市巡察.") : pk::encode("是否禁止城市巡察.");
		}
		bool handler_巡察()
		{
			toggle(menu_force_id_, menu_city_id_, 内政自动化_巡察);	return true;
		}

		string getText_訓练()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_训练) ? pk::encode("允许训练") : pk::encode("禁止训练");
		}
		string getDesc_訓练()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_训练) ? pk::encode("是否允许城市训练.") : pk::encode("是否禁止城市训练.");
		}
		bool handler_訓练()
		{
			toggle(menu_force_id_, menu_city_id_, 内政自动化_训练);	return true;
		}

		string getText_征兵()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_征兵) ? pk::encode("允许征兵") : pk::encode("禁止征兵");
		}
		string getDesc_征兵()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_征兵) ? pk::encode("是否允许城市征兵.") : pk::encode("是否禁止城市征兵.");
		}
		bool handler_征兵()
		{
			toggle(menu_force_id_, menu_city_id_, 内政自动化_征兵);	return true;
		}

		string getText_褒賞()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_褒赏) ? pk::encode("允许褒奖") : pk::encode("禁止褒奖");
		}
		string getDesc_褒賞()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_褒赏) ? pk::encode("是否允许城市褒奖.") : pk::encode("是否禁止城市褒奖.");
		}
		bool handler_褒賞()
		{
			toggle(menu_force_id_, menu_city_id_, 内政自动化_褒赏);	return true;
		}

		string getText_运输()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_运输) ? pk::encode("允许运输") : pk::encode("禁止运输");
		}
		string getDesc_运输()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_运输) ? pk::encode("是否允许城市运输.") : pk::encode("是否禁止城市运输.");
		}
		bool handler_运输()
		{
			toggle(menu_force_id_, menu_city_id_, 内政自动化_运输);	return true;
		}
<<<<<<< HEAD

=======
		
		//**********************************生产菜单开始*************************************

		string getText_枪许可()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_生产枪) ? pk::encode("允许生产枪") : pk::encode("禁止生产枪");
		}
		string getDesc_枪许可()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_生产枪) ? pk::encode("是否允许城市生产枪.") : pk::encode("是否禁止城市生产枪.");
		}
		bool handler_枪许可()
		{
			toggle(menu_force_id_, menu_city_id_, 内政自动化_生产枪);	return true;
		}

		string getText_戟许可()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_生产戟) ? pk::encode("允许生产戟") : pk::encode("禁止生产戟");
		}
		string getDesc_戟许可()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_生产戟) ? pk::encode("是否允许城市生产戟.") : pk::encode("是否禁止城市生产戟.");
		}
		bool handler_戟许可()
		{
			toggle(menu_force_id_, menu_city_id_, 内政自动化_生产戟);	return true;
		}

		string getText_弩许可()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_生产弩) ? pk::encode("允许生产弩") : pk::encode("禁止生产弩");
		}
		string getDesc_弩许可()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_生产弩) ? pk::encode("是否允许城市生产弩.") : pk::encode("是否禁止城市生产弩.");
		}
		bool handler_弩许可()
		{
			toggle(menu_force_id_, menu_city_id_, 内政自动化_生产弩);	return true;
		}

		string getText_马许可()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_生产马) ? pk::encode("允许生产马") : pk::encode("禁止生产马");
		}
		string getDesc_马许可()
		{
			return isEnabled(menu_force_id_, menu_city_id_, 内政自动化_生产马) ? pk::encode("是否允许城市生产马.") : pk::encode("是否禁止城市生产马.");
		}
		bool handler_马许可()
		{
			toggle(menu_force_id_, menu_city_id_, 内政自动化_生产马);	return true;
		}


		//**********************************生产菜单结束*************************************
		//**********************************负责菜单开始************************************

		string getText_巡察负责()
		{
			return  pk::encode("巡察负责");
		}
		string getDesc_巡察负责()
		{
			return  pk::encode("设定巡察负责人");
		}
		bool handler_巡察负责()
		{
			toggle_charge(menu_force_id_, menu_city_id_, 内政自动化_巡察);	return true;
		}

		string getText_訓练负责()
		{
			return pk::encode("训练负责");;
		}
		string getDesc_訓练负责()
		{
			return  pk::encode("设定训练负责人");
		}
		bool handler_訓练负责()
		{
			toggle_charge(menu_force_id_, menu_city_id_, 内政自动化_训练);	return true;
		}

		string getText_征兵负责()
		{
			return pk::encode("征兵负责");;
		}
		string getDesc_征兵负责()
		{
			return  pk::encode("设定征兵负责人");
		}
		bool handler_征兵负责()
		{
			toggle_charge(menu_force_id_, menu_city_id_, 内政自动化_征兵);	return true;
		}

		string getText_生产负责()
		{
			return pk::encode("生产负责");;
		}
		string getDesc_生产负责()
		{
			return  pk::encode("设定生产负责人");
		}
		bool handler_生产负责()
		{
			toggle_charge(menu_force_id_, menu_city_id_, 内政自动化_生产);	return true;
		}
		//**********************************负责菜单结束************************************
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		string getText_统一变更_标准()
		{
			return pk::encode("批量更改");
		}
		string getDesc_统一变更_标准()
		{
			return pk::encode("在所有城市中,统一更改所选城市的运行标准.");
		}
		bool handler_统一变更_标准()
		{
			func_统一变更_标准(); return true;
		}
		void func_统一变更_标准()
		{
			pk::district@ district = pk::get_district(pk::get_district_id(pk::get_force(menu_force_id_), 1));
			array<pk::building@> selected_cities_arr;
			pk::list<pk::city@> city_list = pk::get_city_list(district);
			array<string> answers_项目_arr = { pk::encode("巡察标准 治安"), pk::encode("训练标准 士气比例"), pk::encode("征兵标准 治安"), pk::encode("征兵标准 资金"), pk::encode("征兵标准 兵粮比例"), pk::encode("褒奖标准 忠诚度") };
			string desc_标准 = "";
			int n = pk::choose(pk::encode("选择要批量更改的项目."), answers_项目_arr);	// @items 선택지 최대 6개
			for (int i = 0; i < 6; i++)
			{
				if (n != i) continue;

				desc_标准 = pk::format("请选择要更改标准的城市.({})", pk::decode(answers_项目_arr[n]));
				selected_cities_arr = pk::list_to_array(pk::city_selector(pk::encode("选择城市"), pk::encode(desc_标准), city_list, 1, int(city_list.count)));
				if (int(selected_cities_arr.length) == 0) return;

				array<int> max_num = { 99, 99, 100, 100000, 1000, 99 };
				array<int> def_num = { 95, 95, 95, 2500, 150, 90 };
				pk::int_bool set_standard = pk::numberpad(pk::encode("内政标准"), 0, max_num[n], def_num[n], pk::numberpad_t(numberpad_t));
				if (set_standard.second)
				{
					array<string> city_name_list(0);
					for (int k = 0; k < int(selected_cities_arr.length); k++)
					{
						//	pk::city@ city = pk::building_to_city(selected_cities_arr[k]);	// pk::building_to_city 함수 지원하지 않음
						pk::city@ city = pk::get_city(selected_cities_arr[k].get_id());
						city_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(city))));
						int city_id = city.get_id();
						BaseInfo@ base_t = @base_ex[city_id];
						if (n == 0) base_t.inspections_std = set_standard.first;
						else if (n == 1) base_t.drill_std = set_standard.first;
						else if (n == 2) base_t.recruit_std_porder = set_standard.first;
						else if (n == 3) base_t.recruit_std_gold = set_standard.first;
						else if (n == 4) base_t.recruit_std_foodtroop = set_standard.first;
						else if (n == 5) base_t.reward_std = set_standard.first;
						
					}
					string city_names = join(city_name_list, ", ");
					string city_count_info_1 = (int(selected_cities_arr.length) > 1) ? "等" : "";
					string city_count_info_2 = (int(selected_cities_arr.length) > 1) ? "批量" : "";
					pk::message_box(pk::encode(pk::format("{}{}{}个城市的\x1b[1x{}\x1b[0x{}变更(\x1b[1x{}\x1b[0x)", city_names, city_count_info_1, int(selected_cities_arr.length), pk::decode(answers_项目_arr[n]), city_count_info_2, set_standard.first)));
				}
			}
		}

		string getText_巡察标准_治安()
		{
			return pk::encode("巡察标准 治安");
		}
		string getDesc_巡察标准_治安()
		{
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			return pk::encode(pk::format("城市治安在{}以下时巡察.", base_t.inspections_std));
		}
		bool handler_巡察标准_治安()
		{
			// 순찰기준 치안 설정
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			pk::int_bool set_public_order = pk::numberpad(pk::encode("内政标准"), 0, 99, base_t.inspections_std, pk::numberpad_t(numberpad_t));

			if (set_public_order.second)
			{
				base_t.inspections_std = set_public_order.first;
				
			}

			return true;
		}

		string getText_訓练标准_气力比率()
		{
			return pk::encode("训练标准 士气比例");
		}
		string getDesc_訓练标准_气力比率()
		{
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			return pk::encode(pk::format("当前士气在城市最高士气的{}%以下时训练.", base_t.drill_std));
		}
		bool handler_訓练标准_气力比率()
		{
			// 훈련기준 기력비율 설정
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			pk::int_bool set_drills = pk::numberpad(pk::encode("内政标准"), 0, 99, base_t.drill_std, pk::numberpad_t(numberpad_t));

			if (set_drills.second)
			{
				base_t.drill_std = set_drills.first;
				
			}

			return true;
		}

		string getText_征兵标准_治安()
		{
			return pk::encode("征兵标准 治安");
		}
		string getDesc_征兵标准_治安()
		{
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			return pk::encode(pk::format("城市治安在{}以上时征兵.", base_t.recruit_std_porder));
		}
		bool handler_征兵标准_治安()
		{
			// 징병기준 치안 설정
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			pk::int_bool set_recruit_order = pk::numberpad(pk::encode("内政标准"), 0, 100, base_t.recruit_std_porder, pk::numberpad_t(numberpad_t));

			if (set_recruit_order.second)
			{
				base_t.recruit_std_porder = set_recruit_order.first;
				
			}
			return true;
		}

		string getText_征兵标准_金()
		{
			return pk::encode("征兵标准 资金");
		}
		string getDesc_征兵标准_金()
		{
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			return pk::encode(pk::format("城市资金在{}以上时征兵.", base_t.recruit_std_gold));
		}
		bool handler_征兵标准_金()
		{
			// 징병기준 금 설정
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			pk::int_bool set_recruit_gold = pk::numberpad(pk::encode("内政标准"), 0, 100000, base_t.recruit_std_gold, pk::numberpad_t(numberpad_t));

			if (set_recruit_gold.second)
			{
				base_t.recruit_std_gold = set_recruit_gold.first;
				
			}

			return true;
		}

		string getText_征兵标准_兵粮比率()
		{
			return pk::encode("征兵标准 兵粮比例");
		}
		string getDesc_征兵标准_兵粮比率()
		{
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			return pk::encode(pk::format("城市当前兵力与当前兵粮的比例为{}%以上时征兵.", base_t.recruit_std_foodtroop));
		}
		bool handler_征兵标准_兵粮比率()
		{
			// 징병기준 병량비율 설정
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			pk::int_bool set_recruit_food = pk::numberpad(pk::encode("内政标准"), 0, 1000, base_t.recruit_std_foodtroop, pk::numberpad_t(numberpad_t));

			if (set_recruit_food.second)
			{
				base_t.recruit_std_foodtroop = set_recruit_food.first;
				
			}

			return true;
		}

		string getText_褒賞标准_忠诚度()
		{
			return pk::encode("褒奖标准 忠诚度");
		}
		string getDesc_褒賞标准_忠诚度()
		{
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			return pk::encode(pk::format("武将的忠诚度在{}以下时褒奖.", base_t.reward_std));
		}
		bool handler_褒賞标准_忠诚度()
		{
			// 포상기준 충성도 설정
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			pk::int_bool set_loyaty = pk::numberpad(pk::encode("内政标准"), 0, 99, base_t.reward_std, pk::numberpad_t(numberpad_t));

			if (set_loyaty.second)
			{
				base_t.reward_std = set_loyaty.first;
				
			}

			return true;
		}

		string numberpad_t(int line, int original_value, int current_value)
		{
			return pk::encode("");
		}

		bool isEnabled(int force_id, int city_id, int 内政自动化_项目)
		{
			//用switch是否更好
			BaseInfo@ base_t = @base_ex[city_id];
			if (内政自动化_项目 == 内政自动化_巡察) return base_t.can_inspections;
			else if (内政自动化_项目 == 内政自动化_训练) return base_t.can_drill;
			else if (内政自动化_项目 == 内政自动化_征兵) return base_t.can_recruit;
			else if (内政自动化_项目 == 内政自动化_褒赏) return base_t.can_reward;
			else if (内政自动化_项目 == 内政自动化_运输) return base_t.can_transport;
<<<<<<< HEAD

=======
			else if (内政自动化_项目 == 内政自动化_生产枪) return base_t.can_produce[兵种_枪兵];
			else if (内政自动化_项目 == 内政自动化_生产戟) return base_t.can_produce[兵种_戟兵];
			else if (内政自动化_项目 == 内政自动化_生产弩) return base_t.can_produce[兵种_弩兵];
			else if (内政自动化_项目 == 内政自动化_生产马) return base_t.can_produce[兵种_骑兵];
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			return false;
		}

		void toggle(int force_id, int city_id, int 内政自动化_项目)
		{
			bool 允许与否 = false;
			string 项目名称 = "";
<<<<<<< HEAD
			array<string> 项目名称_清单 = { "\x1b[1x巡察\x1b[0x", "\x1b[1x训练\x1b[0x", "\x1b[1x征兵\x1b[0x", "\x1b[1x褒奖\x1b[0x", "\x1b[1x运输\x1b[0x" };
			//i只有0-4，为什么会有5
			for (int i = 0; i < 5; i++)
=======
			array<string> 项目名称_清单 = { "\x1b[1x巡察\x1b[0x", "\x1b[1x训练\x1b[0x", "\x1b[1x征兵\x1b[0x", "\x1b[1x褒奖\x1b[0x", "\x1b[1x运输\x1b[0x", "\x1b[1x生产枪\x1b[0x", "\x1b[1x生产戟\x1b[0x", "\x1b[1x生产弩\x1b[0x", "\x1b[1x生产马\x1b[0x" };
			//i只有0-4，为什么会有5
			for (int i = 0; i < 9; i++)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			{
				//说明循环时，当i<num时，i是不会到达num的。
				//pk::trace(pk::format("i:{}", i));
				if (内政自动化_项目 == i)
				{
					允许与否 = isEnabled(force_id, city_id, i);
					项目名称 = 项目名称_清单[i];
				}
			}
			string str_允许与否 = (!允许与否) ? "\x1b[1x允许\x1b[0x" : "\x1b[1x禁止\x1b[0x";	// bool값이 바뀔 예정이므로 반대로 적용

			set(city_id, !允许与否, 内政自动化_项目);

			if (force_id == pk::get_current_turn_force_id())
			{
				string city_name = pk::decode(pk::get_name(pk::city_to_building(pk::get_city(city_id))));
<<<<<<< HEAD
				string toggle_msg = pk::format("将\x1b[1x{}\x1b[0x的自治方针中否允许{}改为:{}", city_name, str_允许与否, 项目名称);
=======
				string toggle_msg = pk::format("将\x1b[1x{}\x1b[0x的自治方针中是否允许{}改为:{}", city_name, 项目名称, str_允许与否);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				pk::message_box(pk::encode(toggle_msg));
			}
		}

		void set(int city_id, bool value, int 内政自动化_项目)
		{
			BaseInfo@ base_t = @base_ex[city_id];
			//pk::trace(pk::format("设置前，项目：{}，巡察：{}，训练：{}，征兵{}，褒奖：{}，运输：{}", 内政自动化_项目,base_t.can_inspections, base_t.can_drill, base_t.can_recruit, base_t.can_reward, base_t.can_transport));
			//此处转化有问题？
			switch (内政自动化_项目)
			{
			case 内政自动化_巡察: base_t.can_inspections = value; break;
			case 内政自动化_训练: base_t.can_drill = value; break;
			case 内政自动化_征兵: base_t.can_recruit = value; break;
			case 内政自动化_褒赏: base_t.can_reward = value; break;
			case 内政自动化_运输: base_t.can_transport = value; break;
<<<<<<< HEAD
=======
			case 内政自动化_生产枪: base_t.can_produce[兵种_枪兵] = value; break;
			case 内政自动化_生产戟: base_t.can_produce[兵种_戟兵] = value; break;
			case 内政自动化_生产弩: base_t.can_produce[兵种_弩兵] = value; break;
			case 内政自动化_生产马: base_t.can_produce[兵种_骑兵] = value; break;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			}
			//pk::trace(pk::format("设置中，项目：{}，巡察：{}，训练：{}，征兵{}，褒奖：{}，运输：{}", 内政自动化_项目, base_t.can_inspections, base_t.can_drill, base_t.can_recruit, base_t.can_reward, base_t.can_transport));
			
			//pk::trace(pk::format("设置后，项目：{}，巡察：{}，训练：{}，征兵{}，褒奖：{}，运输：{}", 内政自动化_项目, base_t.can_inspections, base_t.can_drill, base_t.can_recruit, base_t.can_reward, base_t.can_transport));

		}

<<<<<<< HEAD

=======
		void toggle_charge(int force_id, int city_id, int 内政自动化_项目)
		{
			string 执行武将名称 = "无";
			string 前执行武将名称 = "无";
			string 项目名称 = "";
			array<string> 项目名称_清单 = { "巡察执行官", "训练执行官", "征兵执行官", "生产执行官" };
			int id = (内政自动化_项目 == 内政自动化_生产)?3:内政自动化_项目;
			项目名称 = 项目名称_清单[id];
			int person_id = get_charge(city_id, 内政自动化_项目);
			pk::building@ base = pk::get_building(city_id);
			if (pk::is_valid_person_id(person_id)) 执行武将名称 = pk::decode(pk::get_name(pk::get_person(person_id)));
			前执行武将名称 = 执行武将名称;
			//当前{}执行武将为{}，是否要变更执行官
			bool confirm = pk::yes_no(pk::encode(pk::format("当前{}为\x1b[1x{}\x1b[0x，是否要变更执行官?", 项目名称, 执行武将名称)));
			if (confirm)
			{
				pk::list<pk::person@> person_list;// = pk::get_person_list(pk::get_building(city_id), pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
				//10 巡查，33训练 16征兵，5生产
				int list_type = 1;
				switch (内政自动化_项目)
				{
				case 内政自动化_巡察: {list_type = 10; person_list = sort_person_list(base, int(pk::core["inspection.stat"]), -1); break; }
				case 内政自动化_训练: list_type = 33; person_list = sort_person_list(base, int(pk::core["train.stat"]), -1); break;
				case 内政自动化_征兵: list_type = 16; person_list = sort_person_list(base, int(pk::core["recruit.stat"]), int(pk::core["recruit.skill"])); break;
				case 内政自动化_生产: list_type = 5; person_list = sort_person_list(base, int(pk::core["weapon_produce.stat"]), int(pk::core["weapon_produce.smith_skill"])); break;
				}
				pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("执行官选择"), pk::encode(pk::format("选择{}.", 项目名称)), person_list, 1, 1, person_list,list_type);
				if (person_sel.count == 0) return; // 未选擇时或取消时结束 미선택 시 취소 종료
				person_id = person_sel[0].get_id();
				set_charge(city_id, person_id, 内政自动化_项目);
				if (force_id == pk::get_current_turn_force_id())
				{
					string city_name = pk::decode(pk::get_name(pk::city_to_building(pk::get_city(city_id))));
					string person_name = pk::decode(pk::get_name(pk::get_person(person_id)));
					string toggle_msg = pk::format("将{}由\x1b[1x{}\x1b[0x改为:\x1b[1x{}\x1b[0x", city_name, 前执行武将名称, person_name);
					//如果该武将有其他任职，将自动取消
					BaseInfo@ base_t = @base_ex[city_id];
					if (内政自动化_项目 != 内政自动化_巡察 and base_t.inspections_person == person_id)
					{
						toggle_msg += person_name + "，原巡察官任务取消";
						base_t.inspections_person = -1;
					}
					else if (内政自动化_项目 != 内政自动化_训练 and base_t.drill_person == person_id)
					{
						toggle_msg += person_name + "，原训练官任务取消";
						base_t.drill_person = -1;
					}
					else if (内政自动化_项目 != 内政自动化_征兵 and base_t.recruit_person == person_id)
					{
						toggle_msg += person_name + "，原征兵官任务取消";
						base_t.recruit_person = -1;
					}						
					else if (内政自动化_项目 != 内政自动化_生产 and base_t.produce_person == person_id)
					{
						toggle_msg += person_name + "，原生产官任务取消";
						base_t.produce_person = -1;
					}
						
					pk::message_box(pk::encode(toggle_msg));
				}
			}
		}

		void set_charge(int city_id, int person_id, int 内政自动化_项目)
		{
			BaseInfo@ base_t = @base_ex[city_id];
			//pk::trace(pk::format("设置前，项目：{}，巡察：{}，训练：{}，征兵{}，褒奖：{}，运输：{}", 内政自动化_项目,base_t.can_inspections, base_t.can_drill, base_t.can_recruit, base_t.can_reward, base_t.can_transport));
			//此处转化有问题？
			/*
				int16 recruit_person;//征兵执行官
	int16 inspections_person;//巡查执行官
	int16 drill_person;//训练执行官
	int16 produce_person;//生产执行官
			*/
			switch (内政自动化_项目)
			{
			case 内政自动化_巡察: base_t.inspections_person = person_id; break;
			case 内政自动化_训练: base_t.drill_person = person_id; break;
			case 内政自动化_征兵: base_t.recruit_person = person_id; break;
			case 内政自动化_生产: base_t.produce_person = person_id; break;
			}
			//pk::trace(pk::format("设置中，项目：{}，巡察：{}，训练：{}，征兵{}，褒奖：{}，运输：{}", 内政自动化_项目, base_t.can_inspections, base_t.can_drill, base_t.can_recruit, base_t.can_reward, base_t.can_transport));

			//pk::trace(pk::format("设置后，项目：{}，巡察：{}，训练：{}，征兵{}，褒奖：{}，运输：{}", 内政自动化_项目, base_t.can_inspections, base_t.can_drill, base_t.can_recruit, base_t.can_reward, base_t.can_transport));

		}

		int get_charge(int city_id, int 内政自动化_项目)
		{
			BaseInfo@ base_t = @base_ex[city_id];
			int person_id = -1;
	
			switch (内政自动化_项目)
			{
			case 内政自动化_巡察: person_id = base_t.inspections_person; break;
			case 内政自动化_训练: person_id = base_t.drill_person; break;
			case 内政自动化_征兵: person_id = base_t.recruit_person; break;
			case 内政自动化_生产: person_id = base_t.produce_person; break;
			}
			return person_id;
			//pk::trace(pk::format("设置中，项目：{}，巡察：{}，训练：{}，征兵{}，褒奖：{}，运输：{}", 内政自动化_项目, base_t.can_inspections, base_t.can_drill, base_t.can_recruit, base_t.can_reward, base_t.can_transport));

			//pk::trace(pk::format("设置后，项目：{}，巡察：{}，训练：{}，征兵{}，褒奖：{}，运输：{}", 内政自动化_项目, base_t.can_inspections, base_t.can_drill, base_t.can_recruit, base_t.can_reward, base_t.can_transport));

		}
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	}
	Main main;
 }
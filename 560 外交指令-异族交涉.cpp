<<<<<<< HEAD
=======
// ## 2022/03/22 # 江东新风 # 修正异族请求发生率及保护期 ##
// ## 2022/03/21 # 江东新风 # 异族身份异常bug ##
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/02/07 # 江东新风 # 在野贼兵修复，部分贼兵被异常清除bug ##
// ## 2022/01/29 # 江东新风 # 反复请求bug,势力被灭时清空相关请求 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/09/25 # 江东新风 # ai进攻玩家时会邀请异族贼兵协同进攻，异族外交亲善概率触发赠宝 ##
// ## 2021/02/14 # 江东新风 # 根据地生成距离拉远，确保玩家反应时间 ##
// ## 2021/01/31 # 江东新风 # rand函数错误修复 ##
// ## 2021/01/31 # 江东新风 # 降低请求事件发生概率，贼军和异族概率合并 ##
// ## 2021/01/24 # 江东新风 # 修复港关异族交涉可用的bug ##
// ## 2020/12/26 # 江东新风 # 关掉trace ##
// ## 2020/12/15 # 江东新风 # 称呼优化,添加异族交涉保护期 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/12/06 # 江东新风 # 添加亲善和请求装备选项 ##
// ## 2020/12/04 # 江东新风 # AI请求异族设定 ##
// ## 2020/12/01 # 江东新风 # 修复自己城市可选择bug，修复讨价还价钱未扣除bug ##
// @@ 2020/11/21 @ 江东新风 @ 初次编写 @@
/*
@作者：江东新风
@說明：请求异族对指定势力攻击
@备注： 请求的价格以及出的兵量和玩家所处的阶段相关，大致有5个阶段。
		讨价还价所出价格接近目标预期价格时，可能触发异族首领单挑，获胜则以当前价格直接成交
		交涉时可能出现目标势力军师也恰巧在场的剧情，将触发辩论，如果拒绝，则无事发生，如果
		接受，如果舌战胜利，价格减半，舌战失败，则异族将对玩家势力出兵。
@计划： 四亲专属-交易：  可以进行兵粮交换兵装的选项。
		使者均可-亲善：  直接送钱，送粮改善交情的选项。


*/
namespace 外交指令_异族交涉
{


	// ================ CUSTOMIZE ================


	const int ACTION_COST = 20;   	 // 行动力消耗
	const int GOLD_COST = 100;       // 金消耗
	const int 目标势力军师在场率 = 10; //只有目标军师存在时才计算
	const int 异族请求单挑率 = 30; //只有达到接近预期价格时才会判断概率
	//const int ai请求异族发生率 = 8;
	//const int ai请求贼军发生率 = 5;
<<<<<<< HEAD
	const int ai请求发生率 = 3;//原来为3，为配合协同进攻，适当降低基础概率至2
	const int 不耐烦次数 = 4;  //多次错误异族不耐烦的限制，防止反复循环
	const int 异族请求保护期 = 8;//游戏开始前n个月不发生ai请求，默认4
=======
	const int ai请求发生率 = 1;//原来为3，为配合协同进攻，适当降低基础概率至2
	const int 不耐烦次数 = 4;  //多次错误异族不耐烦的限制，防止反复循环
	const int 异族请求保护期 = 10;//6;//游戏开始前n个月不发生ai请求，默认4
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	const array<int> 协同请求发生率 = { /*初级难度*/15, /*上级难度*/35,/*超级难度*/70 };	// 数字表示发生概率


	const int KEY = pk::hash("异族交涉");//팎뻐
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


	//部队数_年[peroiod][is_player][头目or将][异族id]
	const array<array<array<array<int>>>> 部队数_年 =
	{ {{/*早期 第一年 1 2*/
			/**玩家 势力城市*/ {/*贼将*/1, /*羌头目*/1, /*乌丸头目*/1, /*山越头目*/1, /*南蛮头目*/1},
			/**玩家 势力城市*/ {/*盗贼*/1, /*羌武将*/1, /*乌丸武将*/1, /*山越武将*/1, /*南蛮武将*/1}
		},{
			/** AI  势力城市*/ {/*贼将*/1, /*羌头目*/1, /*乌丸头目*/1, /*山越头目*/1, /*南蛮头目*/1},
			/** AI  势力城市*/ {/*盗贼*/0, /*羌武将*/0, /*乌丸武将*/0, /*山越武将*/0, /*南蛮武将*/0}
		}},{{/*早期 第二年 2 3*/
				/**玩家 势力城市*/ {/*贼将*/1, /*羌头目*/1, /*乌丸头目*/1, /*山越头目*/1, /*南蛮头目*/1},
				/**玩家 势力城市*/ {/*盗贼*/2, /*羌武将*/2, /*乌丸武将*/2, /*山越武将*/2, /*南蛮武将*/2}
			},{
				/** AI  势力城市*/ {/*贼将*/1, /*羌头目*/1, /*乌丸头目*/1, /*山越头目*/1, /*南蛮头目*/1},
				/** AI  势力城市*/ {/*盗贼*/1, /*羌武将*/1, /*乌丸武将*/1, /*山越武将*/1, /*南蛮武将*/1}
			}},	{{/*中期 第三年-第六年 3 5*/
					/**玩家 势力城市*/ {/*贼将*/1, /*羌头目*/1, /*乌丸头目*/1, /*山越头目*/1, /*南蛮头目*/1},
					/**玩家 势力城市*/ {/*盗贼*/4, /*羌武将*/4, /*乌丸武将*/4, /*山越武将*/4, /*南蛮武将*/4}
				},{
					/** AI  势力城市*/ {/*贼将*/1, /*羌头目*/1, /*乌丸头目*/1, /*山越头目*/1, /*南蛮头目*/1},
					/** AI  势力城市*/ {/*盗贼*/2, /*羌武将*/2, /*乌丸武将*/2, /*山越武将*/2, /*南蛮武将*/2}
				}},	{{/*中期 第六年-第十年 5 10*/
						/**玩家 势力城市*/ {/*贼将*/1, /*羌头目*/1, /*乌丸头目*/1, /*山越头目*/1, /*南蛮头目*/1},
						/**玩家 势力城市*/ {/*盗贼*/4, /*羌武将*/4, /*乌丸武将*/4, /*山越武将*/4, /*南蛮武将*/4}
					},{
						/** AI  势力城市*/ {/*贼将*/1, /*羌头目*/1, /*乌丸头目*/1, /*山越头目*/1, /*南蛮头目*/1},
						/** AI  势力城市*/ {/*盗贼*/4, /*羌武将*/4, /*乌丸武将*/4, /*山越武将*/4, /*南蛮武将*/4}
					}},	{{/*后期 十年后 6 15*/
							/**玩家 势力城市*/ {/*贼将*/1, /*羌头目*/1, /*乌丸头目*/1, /*山越头目*/1, /*南蛮头目*/1},
							/**玩家 势力城市*/ {/*盗贼*/4, /*羌武将*/4, /*乌丸武将*/4, /*山越武将*/4, /*南蛮武将*/4}
						},{
							/** AI  势力城市*/ {/*贼将*/1, /*羌头目*/1, /*乌丸头目*/1, /*山越头目*/1, /*南蛮头目*/1},
							/** AI  势力城市*/ {/*盗贼*/2, /*羌武将*/2, /*乌丸武将*/2, /*山越武将*/2, /*南蛮武将*/2}
						}} };

	/*
		[ 项目随机值 ]

		 1) 数组形式 : {항목1, 항목2, 항목3, ... , 마지막 항목}

		 2) 返回值   : 1개 이상의 배열 전체 항목 중 하나의 항목을 무작위로 결정하여 해당 항목 값을 반환
		  - 原本内容 : {兵器_枪, 兵器_戟} - '창', '극' 두 개의 무기 중 하나를 무작위로 결정
		  - 修改举例 : {兵器_枪, 兵器_弩} - '창', '노' 두 개의 무기 중 하나를 무작위로 결정

		 3) 배열에 항목 추가 및 清除 가능
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
	/// 贼将势力部队兵力设定 (项目随机值)
	// 预设修改方法 : 项目随机值参考说明  部队兵力_年[period][头目or将][异族id][is_player][兵力]
	const array<array<array<array<array<int>>>>> 部队兵力_年 =
	{ {{/*早期 第1阶段*/
		{/*  玩家贼将      */ {4500, 5500, 6500}, /*  AI贼将      */ {4500, 5500, 6500}},
		{/*  玩家羌头目    */ {5000, 6000, 7000}, /*  AI羌头目    */ {5000, 6000, 7000}},
		{/*  玩家乌丸头目  */ {5000, 6000, 7000}, /*  AI乌丸头目  */ {5000, 6000, 7000}},
		{/*  玩家山越头目  */ {5000, 6000, 7000}, /*  AI山越头目  */ {5000, 6000, 7000}},
		{/*  玩家南蛮头目  */ {5000, 6000, 7000}, /*  AI南蛮头目  */ {5000, 6000, 7000}},
	},{///玩家实际第一阶段不生成副将部队
		{/*  玩家盗贼      */ {3500, 4000, 4500}, /*  AI盗贼      */ {3500, 4000, 4500}},
		{/*  玩家羌武将    */ {4000, 4500, 5000}, /*  AI羌武将    */ {4000, 4500, 5000}},
		{/*  玩家乌丸武将  */ {4000, 4500, 5000}, /*  AI乌丸武将  */ {4000, 4500, 5000}},
		{/*  玩家山越武将  */ {4000, 4500, 5000}, /*  AI山越武将  */ {4000, 4500, 5000}},
		{/*  玩家南蛮武将  */ {4000, 4500, 5000}, /*  AI南蛮武将  */ {4000, 4500, 5000}},
	}},{{/*早期 第2阶段*/
		{/*  玩家贼将      */ {5500, 6500, 7500}, /*  AI贼将      */ {5500, 6500, 7500}},
		{/*  玩家羌头目    */ {6000, 7000, 8000}, /*  AI羌头目    */ {6000, 7000, 8000}},
		{/*  玩家乌丸头目  */ {6000, 7000, 8000}, /*  AI乌丸头目  */ {6000, 7000, 8000}},
		{/*  玩家山越头目  */ {6000, 7000, 8000}, /*  AI山越头目  */ {6000, 7000, 8000}},
		{/*  玩家南蛮头目  */ {6000, 7000, 8000}, /*  AI南蛮头目  */ {6000, 7000, 8000}},
	},{
		{/*  玩家盗贼      */ {4500, 5000, 5500}, /*  AI盗贼      */ {3500, 4500, 5500}},
		{/*  玩家羌武将    */ {5000, 5500, 6000}, /*  AI羌武将    */ {4000, 5000, 6000}},
		{/*  玩家乌丸武将  */ {5000, 5500, 6000}, /*  AI乌丸武将  */ {4000, 5000, 6000}},
		{/*  玩家山越武将  */ {5000, 5500, 6000}, /*  AI山越武将  */ {4000, 5000, 6000}},
		{/*  玩家南蛮武将  */ {5000, 5500, 6000}, /*  AI南蛮武将  */ {4000, 5000, 6000}},
	}},{{/*中期 第3阶段*/
		{/*  玩家贼将      */ {6500, 7000, 8500}, /*  AI贼将      */ {6500, 7000, 7500}},
		{/*  玩家羌头目    */ {7000, 8000, 9000}, /*  AI羌头目    */ {7000, 7500, 8000}},
		{/*  玩家乌丸头目  */ {7000, 8000, 9000}, /*  AI乌丸头目  */ {7000, 7500, 8000}},
		{/*  玩家山越头目  */ {7000, 8000, 9000}, /*  AI山越头目  */ {7000, 7500, 8000}},
		{/*  玩家南蛮头目  */ {7000, 8000, 9000}, /*  AI南蛮头目  */ {7000, 7500, 8000}},
	},{
		{/*  玩家盗贼      */ {5000, 5500, 6000}, /*  AI盗贼      */ {4500, 5000, 5500}},
		{/*  玩家羌武将    */ {5500, 6000, 6500}, /*  AI羌武将    */ {5000, 5500, 6000}},
		{/*  玩家乌丸武将  */ {5500, 6000, 6500}, /*  AI乌丸武将  */ {5000, 5500, 6000}},
		{/*  玩家山越武将  */ {5500, 6000, 6500}, /*  AI山越武将  */ {5000, 5500, 6000}},
		{/*  玩家南蛮武将  */ {5500, 6000, 6500}, /*  AI南蛮武将  */ {5000, 5500, 6000}},
	}},{{/*中期 第4阶段*/
		{/*  玩家贼将      */ {8000, 9000, 10000},/*  AI贼将      */ {6500, 7000, 7500}},
		{/*  玩家羌头目    */ {8000, 9000, 10000},/*  AI羌头目    */ {7000, 7500, 8000}},
		{/*  玩家乌丸头目  */ {8000, 9000, 10000},/*  AI乌丸头目  */ {7000, 7500, 8000}},
		{/*  玩家山越头目  */ {8000, 9000, 10000},/*  AI山越头目  */ {7000, 7500, 8000}},
		{/*  玩家南蛮头目  */ {8000, 9000, 10000},/*  AI南蛮头目  */ {7000, 7500, 8000}},
	},{
		{/*  玩家盗贼      */ {6000, 6500, 7000}, /*  AI盗贼      */ {4500, 5000, 5500}},
		{/*  玩家羌武将    */ {6000, 6500, 7000}, /*  AI羌武将    */ {5000, 5500, 6000}},
		{/*  玩家乌丸武将  */ {6000, 6500, 7000}, /*  AI乌丸武将  */ {5000, 5500, 6000}},
		{/*  玩家山越武将  */ {6000, 6500, 7000}, /*  AI山越武将  */ {5000, 5500, 6000}},
		{/*  玩家南蛮武将  */ {6000, 6500, 7000}, /*  AI南蛮武将  */ {5000, 5500, 6000}},
	}},{{/*后期 第5阶段*/
		{/*  玩家贼将      */ {10000, 11000, 12000}, /* AI贼将    */ {6500, 7000, 7500}},
		{/*  玩家羌头目    */ {10000, 11000, 12000}, /* AI羌头目  */ {7000, 7500, 8000}},
		{/*  玩家乌丸头目  */ {10000, 11000, 12000}, /* AI乌丸头目*/ {7000, 7500, 8000}},
		{/*  玩家山越头目  */ {10000, 11000, 12000}, /* AI山越头目*/ {7000, 7500, 8000}},
		{/*  玩家南蛮头目  */ {10000, 11000, 12000}, /* AI南蛮头目*/ {7000, 7500, 8000}},
	},{
		{/*  玩家盗贼      */ {7000, 7500, 8000}, /*  AI盗贼      */ {4500, 5000, 5500}},
		{/*  玩家羌武将    */ {7000, 7500, 8000}, /*  AI羌武将    */ {5000, 5500, 6000}},
		{/*  玩家乌丸武将  */ {7000, 7500, 8000}, /*  AI乌丸武将  */ {5000, 5500, 6000}},
		{/*  玩家山越武将  */ {7000, 7500, 8000}, /*  AI山越武将  */ {5000, 5500, 6000}},
		{/*  玩家南蛮武将  */ {7000, 7500, 8000}, /*  AI南蛮武将  */ {5000, 5500, 6000}},
	}} };

	/// 贼将势力部队金设定
	const array<array<int>> 部队金 =
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

	/// [ 敌势力武将 相关设定 ]

	/*
		[ 范围随机值 ]

		 1) 数组形式 : {第一个(最小值), 第二个(最大值)}

		 2) 返回值   : 返回最小值以上最大值以下
		  - 原本内容 : {75, 85} - 返回75以上85以下的值
		  - 修改举例 : {80, 90} - 返回80以上90以下的值

		 3 )数组必须有两个项目组成
	*/

	/// 敌势力武将能力值: 统率、武力、智力设定 (范围随机值)
	// 预设修改方法 : 范围随机值参考说明
	const array<array<array<int>>> 敌将统率 =
	{
		/**头目*/ {/*贼将*/{75, 85}, /*羌头目*/{80, 90}, /*乌丸头目*/{80, 90}, /*山越头目*/{80, 90}, /*南蛮头目*/{80, 90}},
		/**部将*/ {/*盗贼*/{65, 75}, /*羌武将*/{70, 80}, /*乌丸武将*/{70, 80}, /*山越武将*/{70, 80}, /*南蛮武将*/{70, 80}}
	};
	const array<array<array<int>>> 敌将武力 =
	{
		/**头目*/ {/*贼将*/{75, 85}, /*羌头目*/{80, 90}, /*乌丸头目*/{80, 90}, /*山越头目*/{80, 90}, /*南蛮头目*/{80, 90}},
		/**部将*/ {/*盗贼*/{65, 75}, /*羌武将*/{70, 80}, /*乌丸武将*/{70, 80}, /*山越武将*/{70, 80}, /*南蛮武将*/{70, 80}}
	};
	const array<array<array<int>>> 敌将智力 =
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
	// ===========================================
	const array<array<int>> 异族出现城市 =
	{
		/*羌族  */ {城市_武威, 城市_安定, 城市_天水},
		/*乌丸族*/ {城市_襄平, 城市_北平, 城市_蓟},
		/*山越族*/ {城市_会稽, 城市_柴桑, 城市_吴},
		/*南蛮族*/ {城市_云南, 城市_建宁, 城市_江州}
	};
	//  根据地与都市距离范围
	const array<int> 根据地生成_距离 = { 12, 16 };	// {最小, 最大}
	//	根据地不可能出现的地形:		//		毒泉		川		江		海			官道		栈道		岸		山（崖）	小徑
	const array<int> 根据地生成_禁止地形 = { 地形_毒泉, 地形_川, 地形_河, 地形_海, 地形_官道, 地形_栈道, 地形_岸, 地形_崖, 地形_间道, 地形_森 };

	// ===========================================
	const array<array<int>> 请求异族城市 =
	{
		/*羌族  */ {城市_武威, 城市_安定, 城市_天水, 城市_汉中, 城市_长安},
		/*乌丸族*/ {城市_襄平, 城市_北平, 城市_蓟, 城市_南皮, 城市_晋阳},
		/*山越族*/ {城市_会稽, 城市_柴桑, 城市_吴, 城市_建业, 城市_江夏, 城市_庐江},
		/*南蛮族*/ {城市_云南, 城市_建宁, 城市_江州, 城市_永安, 城市_成都}
	};

	const array<array<int>> 异族基础军费 =
	{
		/*贼军  */ {1000, 2000, 3500, 4500, 9000},
		/*羌族  */ {2000, 3000, 4500, 6500, 11000},
		/*乌丸族*/ {2000, 3000, 4500, 6500, 11000},
		/*山越族*/ {1500, 2500, 4000, 6000, 10000},
		/*南蛮族*/ {1500, 2500, 4000, 6000, 10000}
	};

	const array<array<int>> 异族附加军费 =
	{
		/*贼军  */ {500, 1000, 2500, 3500, 4500},
		/*羌族  */ {1500, 2500, 4000, 5000, 6000},
		/*乌丸族*/ {1500, 2500, 4000, 5000, 6000},
		/*山越族*/ {1000, 2000, 3000, 4000, 5000},
		/*南蛮族*/ {1000, 2000, 3000, 4000, 5000}
	};

	const array<array<int>> 异族军费_AI =
	{
		/*贼军  */ {1000, 2000, 3500, 4500, 9000},
		/*羌族  */ {2000, 3000, 4500, 6500, 11000},
		/*乌丸族*/ {2000, 3000, 4500, 6500, 11000},
		/*山越族*/ {1500, 2500, 4000, 6000, 10000},
		/*南蛮族*/ {1500, 2500, 4000, 6000, 10000}
	};

	//数值除以100为buf比例
	const array<array<int>> 军费_季节buf =
	{
		/*羌族  *//*春，夏，秋，冬*/ {100, 110, 80, 120},
		/*乌丸族*//*春，夏，秋，冬*/ {100, 110, 80, 120},
		/*山越族*//*春，夏，秋，冬*/ {90, 130, 80, 110},
		/*南蛮族*//*春，夏，秋，冬*/ {90, 130, 80, 110}
	};

	const array<array<int>> 异族_坐标 =
	{
		/*羌族  *//*x，y*/ {24, 20},
		/*乌丸族*//*x，y*/ {195, 1},
		/*山越族*//*x，y*/ {185, 146},
		/*南蛮族*//*x，y*/ {39, 198}
	};

	const float 军费_四亲buf = 0.8;


	const bool 调试模式 = false;
	class Main
	{

		pk::building@ building_;
		pk::force@ yizu_force;
		pk::force@ target_force;//要攻打的势力
		pk::building@ target_building;	//要攻打的据点
		pk::city@ target_city;	//要攻打的据点
		pk::person@ target_gunshi;
		pk::force@ force_;
		pk::person@ src_kunshu;
		pk::person@ taishu_;
		pk::person@ actor;
		pk::person @dst_kunshu;
		pk::city@ city_;
		int building_gold;
		int expected_gold;
		float season_buf;
		bool flag_deal = false;
		int current_period = 0;
		int duel_result = -1;
		int debate_result = -1;
		bool flag_debate_faied = false;//true时表明和目标势力的军师辩论失败，将会触发蛮族向我方据点出兵。
		int bandit_force_index_t = -1;
		int bandit_force_id_t = -1;
		bool flag_goodwill = false;
		bool flag_equipment = false;
		int weapon_gain_type = -1;
		int weapon_gain = -1;
		int equipment_food_cost = -1;
		int goodwill_gold_cost = -1;

		string target_force_name;
		string src_force_name;
		string yizu_force_name;

		pk::point pos_for_screen_move;

		/// 이벤트 관련 적장세력 정보 (수정금지)
		array<int> 异族势力 = { 势力_盗贼, 势力_羌, 势力_乌丸, 势力_山越, 势力_南蛮 };//异族势力 42羌,43山越,44乌丸,45南蛮,46盗贼
		array<array<int>> 敌部队_武将 =
		{
			/*头目*/ {武将_贼将, 武将_羌头目, 武将_乌丸头目, 武将_山越头目, 武将_南蛮头目},
			/*部将*/ {武将_盗贼, 武将_羌武将, 武将_乌丸武将, 武将_山越武将, 武将_南蛮武将}
		};
		array<int> 敌军设施 = { 设施_根据地2, 设施_根据地1, 设施_根据地1, 设施_根据地1, 设施_根据地1 };

		array<int> 协同请求 = { /*请求方据点id*/-1, /*目标城市id*/-1, /*异族势力id*/势力_盗贼, /*多少旬后出兵*/-1};

		Main()
		{
			add_menu();
			//ai请求贼兵异族的设定
			pk::bind(111, pk::trigger111_t(onTurnStart));
			pk::bind(176, pk::trigger176_t(onAttackAlert));//attack alert 被攻击时提示的位置作为触发器，获取相关信息
			pk::bind(202, pk::trigger202_t(onAIRunningOrder));
			pk::bind(131, pk::trigger131_t(onForceRemove));
		}

		void add_menu()
		{
			pk::menu_item item;
			item.menu = 103;
			item.pos = 8;
			item.shortcut = "";
			item.init = pk::building_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(isVisible);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		void onAttackAlert(pk::unit@ unit)
		{
			if (pk::is_alive(unit))
			{
				if (调试模式) pk::trace(pk::format("attack alert 部队：{},对象id：{}", pk::decode(pk::get_name(unit)), unit.target));
				int unit_force_id = unit.get_force_id();

				//部队所属势力为正常势力，非玩家势力，目标是正常城市
				if (pk::is_normal_force(unit_force_id) and !unit.is_player() and (unit.target > -1 and unit.target < 建筑_城市末))
				{
					pk::building@ target_building = pk::get_building(unit.target);
					//确认目标是玩家
<<<<<<< HEAD
					if (target_building.is_player() and pk::rand_bool(100))//协同请求发生率[pk::get_scenario().difficulty]))
=======
					if (target_building.is_player() and pk::rand_bool(协同请求发生率[pk::get_scenario().difficulty]))
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					{
						//储存相关信息，以便ai和异族交涉
						//需储存目标据点，ai势力id，预计到达时间

						pk::building@ service_building = pk::get_building(pk::get_service(unit));
						if (!pk::is_alive(service_building)) return;
						//下面这种方法获取的city_id可能不是ai的城市，需验证----因为现在只是用来判断是否能请求，所以就不需要验证了
						pk::city@ service_city = pk::get_city(pk::get_city_id(service_building.pos));
						//获取要请求的异族势力
						//异族势力 42羌,43山越,44乌丸,45南蛮,46盗贼
						bool has_异族 = false;
						forceinfo@ force_t = @force_ex[unit_force_id];
						int info_index = -1;
						bool 相同请求 = false;
						//先确认是否请求过，已请求过，就不要多次请求了，出兵后可再次请求
						for (int j = 0; j < 3; j++)
						{
							if (调试模式) pk::trace(pk::format("force_t.aim_city_id：{},j:{}", force_t.aim_city_id[j],j));
							if (force_t.aim_city_id[j] == unit.target)//说明已请求过异族
							{
								相同请求 = true;
								break;
							}
						}
						if (!相同请求)
						{
							for (int j = 0; j < 3; j++)
							{

								if (调试模式) pk::trace(pk::format("force_t.aim_city_id：{},j:{}", force_t.aim_city_id[j],j));
								if (force_t.aim_city_id[j] == -1)//说明结构体位置空着，可以用
								{
									info_index = j;
									break;
								}
							}

							for (int i = 0; i < 4; i++)//需测试4是否被计算---不被计算
							{
								//pk::trace(pk::format("测试4是否被计算：i:{}", i));
								if (异族出现城市[i].find(unit.target) >= 0 and 请求异族城市[i].find(service_city.get_id()) >= 0)
								{
									if (info_index != -1) force_t.yizu_force_id[info_index] = 异族势力[i + 1];
									has_异族 = true;
								}

							}
							if (info_index != -1)
							{
								if (!has_异族) force_t.yizu_force_id[info_index] = 势力_盗贼;
								force_t.ask_base_id[info_index] = pk::get_service(unit);
								force_t.aim_city_id[info_index] = unit.target;
								force_t.wait_timer[info_index] = get_arrive_day(unit, target_building.pos) - 1;//比预计到达时间早一天出兵，可以确保压力同时给上
								//pk::trace(pk::format("协同请求 before update：我方据点id:{},目标城市id：{},异族势力:{},预计抵达天数:{}", force_t.ask_base_id[info_index], force_t.aim_city_id[info_index], force_t.yizu_force_id[info_index], force_t.wait_timer[info_index]));

								if (调试模式) pk::trace(pk::format("协同请求：我方据点id:{},目标城市id：{},异族势力:{},预计抵达天数:{}", force_t.ask_base_id[info_index], force_t.aim_city_id[info_index], force_t.yizu_force_id[info_index], force_t.wait_timer[info_index]));
							}

						}


						
						//AI_异族入侵_执行(city_city.first, city_city.second, 46);
					}
				}
			}
		}

		void onForceRemove(pk::force@ force)
		{
			int force_id = force.get_id();
			if (!pk::is_valid_normal_force_id(force_id)) return;
			forceinfo@ force_t = @force_ex[force_id];
			for (int info_index = 0; info_index < 3; ++info_index)
			{
				force_t.ask_base_id[info_index] = -1;
				force_t.aim_city_id[info_index] = -1;
				force_t.yizu_force_id[info_index] = -1;
				force_t.wait_timer[info_index] = -1;
			}
		}

		void onAIRunningOrder(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			/*
			if (cmd == 据点AI_计略扰乱)
			{
				pk::list<pk::person@> person_sel;
				if (run_order_before(person_sel, building)) run_order(person_sel, building);
				else if (调试模式) pk::trace("据点AI_计略扰乱 不满足");
			}*/
		}

		int get_arrive_day(pk::unit@ src_unit, pk::point dst_pos)
		{
			// 到目标的路径，坐标数组
			auto paths = pk::get_path(src_unit, src_unit.pos, dst_pos);

			// 중간이동지점
			int day_cost = 0;
			int cost_sum = 0;
			int unit_mov = src_unit.max_attr.stat[部队能力_移动];
			for (int i = 0; i < int(paths.length); i++)
			{
				pk::hex@hex = pk::get_hex(paths[i]);
				int terrain_id = hex.terrain;
				int weapon_id = pk::is_water_terrain(terrain_id) ? pk::get_sea_weapon_id(src_unit) : pk::get_ground_weapon_id(src_unit);
				pk::equipment @ equipment = pk::get_equipment(weapon_id);

				if (i > 0)
				{
					if ((unit_mov > cost_sum) and (unit_mov < (cost_sum + equipment.movement_cost[terrain_id])))
					{
						day_cost += 1;
						cost_sum = equipment.movement_cost[terrain_id];
					}
					else if (unit_mov == (cost_sum + equipment.movement_cost[terrain_id]))
					{
						day_cost += 1;
						cost_sum = 0;
					}
					else cost_sum += equipment.movement_cost[terrain_id];
				}
					
			}
			return day_cost;
		}

		void onTurnStart(pk::force@ force)
		{

			if (pk::get_elapsed_months() > 异族请求保护期)
			{
				if (force.is_player())
				{

					current_period = get_current_period(force);
					if (pk::rand_bool(ai请求发生率))
					{
						//这样就是确定请求事件发生时，如果能请求异族，80%概率请求异族，不能请求，则请求贼军
						pk::int_int_int city_city_force = fun_获取AI_进攻及请求_城市_异族(force);
						if (city_city_force.first != -1 and pk::rand_bool(80))
						{
							AI_异族入侵_执行(city_city_force.first, city_city_force.second, city_city_force.third);
							//pk::trace(pk::format("ai提出请求城市id：{}, 目标城市id：{}, 异族势力id:{}", city_city_force.first, city_city_force.second, city_city_force.third));
							return;
						}
						pk::int_int city_city = fun_获取AI_进攻及请求_城市_盗贼(force);
						if (city_city.first != -1)
						{
							AI_异族入侵_执行(city_city.first, city_city.second, 46);
							//pk::trace(pk::format("ai提出请求城市id：{}, 目标城市id：{}, 异族势力id:46", city_city.first, city_city.second));
						}

					}

					for (int i = 0; i < 非贼势力_末; i++)
					{
						forceinfo@ force_t = @force_ex[i];
						for (int j = 0; j < 3; j++)
						{
							if (force_t.wait_timer[j] > 0)
							{
								force_t.wait_timer[j] -= 1;
								//pk::trace(pk::format("协同请求：我方据点id:{},目标城市id：{},异族势力:{},距离出兵还剩{}旬", force_t.ask_base_id[j], force_t.aim_city_id[j], force_t.yizu_force_id[j], force_t.wait_timer[j]));
								
							}
							if (force_t.wait_timer[j] == 0)
							{
								//执行前初始化数据---如放在执行后，如果函数出问题就会跳过清空
								int ask_base_id = force_t.ask_base_id[j];
								int aim_city_id = force_t.aim_city_id[j];
								int yizu_force_id = force_t.yizu_force_id[j];
								force_t.wait_timer[j] = -1;
								AI_异族入侵_执行(ask_base_id, aim_city_id, yizu_force_id,1);

							}
						}
					}
					/*
					if (pk::rand_bool(ai请求贼军发生率))
					{
						pk::int_int city_city = fun_获取AI_进攻及请求_城市_盗贼(force);
						if (city_city.first != -1)
						{
							AI_异族入侵_执行(city_city.first, city_city.second, 46);
<<<<<<< HEAD
							pk::trace(pk::format("ai提出请求城市id：{}, 目标城市id：{}, 异族势力id:46", city_city.first, city_city.second));
=======
							if (调试模式) pk::trace(pk::format("ai提出请求城市id：{}, 目标城市id：{}, 异族势力id:46", city_city.first, city_city.second));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
						}
					}
					if (pk::rand_bool(ai请求异族发生率))
					{
						pk::int_int_int city_city_force = fun_获取AI_进攻及请求_城市_异族(force);
						if (city_city_force.first != -1)
						{
							AI_异族入侵_执行(city_city_force.first, city_city_force.second, city_city_force.third);
<<<<<<< HEAD
							pk::trace(pk::format("ai提出请求城市id：{}, 目标城市id：{}, 异族势力id:{}", city_city_force.first, city_city_force.second, city_city_force.third));
=======
							if (调试模式) pk::trace(pk::format("ai提出请求城市id：{}, 目标城市id：{}, 异族势力id:{}", city_city_force.first, city_city_force.second, city_city_force.third));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
						}
					}*/
				}
			}
		}

		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@taishu_ = pk::get_person(pk::get_taishu_id(building));
			@city_ = pk::building_to_city(building);
			building_gold = pk::get_gold(building);

		}

		//if (异族出现城市[index].find(i) >= 0)
		bool isVisible()
		{
			if (building_.facility > 0) return false;
			if (请求异族城市[0].find(city_.get_id()) < 0 and 请求异族城市[1].find(city_.get_id()) < 0 and 请求异族城市[2].find(city_.get_id()) < 0 and 请求异族城市[3].find(city_.get_id()) < 0) return false;
			return true;
		}

		bool isEnabled()
		{
			if (pk::get_idle_person_list(building_).count == 0) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			else if (building_gold < GOLD_COST) return false;
			return true;
		}

		string getText()
		{
			if (请求异族城市[0].find(city_.get_id()) >= 0)
				return pk::encode("羌族交涉");//
			if (请求异族城市[1].find(city_.get_id()) >= 0)
				return pk::encode("乌丸族交涉");//
			if (请求异族城市[2].find(city_.get_id()) >= 0)
				return pk::encode("山越族交涉");//
			if (请求异族城市[3].find(city_.get_id()) >= 0)
				return pk::encode("南蛮族交涉");//
			return pk::encode("贼军交涉");//备用	
		}

		string getDesc()
		{
			if (pk::get_idle_person_list(building_).count == 0)
				return pk::encode("没有可执行的武将.");
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (需要{}行动力)", ACTION_COST));
			else if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (需要{}金)", GOLD_COST));
			else
				return pk::encode(pk::format("和异族进行交涉. (行动力 {})", ACTION_COST));
		}

		bool handler()
		{
			//初始化常量
			flag_debate_faied = false;
			flag_deal = false;
			current_period = 0;
			duel_result = -1;
			debate_result = -1;
			bandit_force_index_t = -1;
			bandit_force_id_t = -1;
			flag_goodwill = false;
			flag_equipment = false;

			src_force_name = pk::decode(pk::get_name(force_));

			if (pk::choose({ pk::encode(" 是 "), pk::encode(" 否 ") }, pk::encode(pk::format("是否和异族进行交涉？", GOLD_COST)), taishu_) == 1)
				return false;

			// 执行武将
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building_);

			person_list.sort(function(a, b)
			{
				return (a.stat[武将能力_智力] > b.stat[武将能力_智力]); // 무장 정렬 (지력순)
			});
			if (person_list.count == 0) return false;

			//pk::list<pk::force@> force_list = getYizuList();

			//if (force_list.count == 0) return false;

			//pk::list<pk::city@> target_city_list = pk::get_city_list();

			//if (target_city_list.count == 0) return false;			
			bandit_force_index_t = get_bandit_force_index_of_city_2(city_.get_id());
			bandit_force_id_t = get_bandit_force_id_of_city_2(city_.get_id());
			// 실행무장 선택하기
			pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("交涉武将选择"), pk::encode("选择执行异族交涉的武将."), person_list, 1, 1);
			if (person_sel.count == 0) return false; // 未选擇时取消结束
			@actor = person_sel[0];

			@yizu_force = pk::get_force(bandit_force_id_t);
			@dst_kunshu = pk::get_person(yizu_force.kunshu);
			yizu_force_name = pk::decode(pk::get_name(yizu_force));
			@src_kunshu = pk::get_person(force_.kunshu);
			current_period = get_current_period(force_);

			pk::scene(pk::scene_t(scene));


			//flag_deal = false;
			//flag_debate_faied =true;
			if (flag_goodwill)
			{
				pk::add_gold(building_, goodwill_gold_cost, true);
				pk::message_box(pk::encode(pk::format("\x1b[2x{}军\x1b[0x和\x1b[2x{}军\x1b[0x的友好关系上升了。", src_force_name, yizu_force_name)));
			}
			if (flag_equipment)
			{
				pk::add_food(building_, equipment_food_cost, true);
				pk::add_weapon_amount(city_, weapon_gain_type, weapon_gain, true);
				pk::message_box(pk::encode(pk::format("通过相互协助，\x1b[2x{}军\x1b[0x和\x1b[2x{}军\x1b[0x的友好关系上升了。", src_force_name, yizu_force_name)));
			}
			if (flag_deal)
			{
				int target_city_id = target_city.get_id();


				int bandit_facility_id = 敌军设施[bandit_force_index_t];//设施_根据地1;

				int controller_index = get_controller_index_of_force(target_force);	//玩家or电脑

				pk::building@ new_bandit_base = func_基本事件1_贼将势力根据地_生成(target_force, target_city, target_building, bandit_force_id_t, bandit_facility_id, bandit_force_index_t);

				func_基本事件_画面移动(target_force, new_bandit_base.pos);
				//pk::trace(pk::format("执行进攻，目标势力id：{}, 名：{}", target_force.get_id(), target_force_name));
				func_基本事件_贼将势力部队_生成(target_city, target_building, new_bandit_base, bandit_force_id_t, controller_index, bandit_force_index_t, /*bandit_person_index*/0);
				func_基本事件_贼将势力部队_生成(target_city, target_building, new_bandit_base, bandit_force_id_t, controller_index, bandit_force_index_t, /*bandit_person_index*/1);
				if (current_period <= 1) pk::kill(new_bandit_base);

				if (current_period == 4)
				{
					pk::building@ new_bandit_base2 = func_基本事件1_贼将势力根据地_生成(target_force, target_city, target_building, bandit_force_id_t, bandit_facility_id, bandit_force_index_t);
					func_基本事件_画面移动(target_force, new_bandit_base2.pos);
					//pk::trace(pk::format("执行进攻，目标势力id：{}, 名：{}", target_force.get_id(), target_force_name));
					func_基本事件_贼将势力部队_生成(target_city, target_building, new_bandit_base2, bandit_force_id_t, controller_index, bandit_force_index_t, /*bandit_person_index*/0);
					func_基本事件_贼将势力部队_生成(target_city, target_building, new_bandit_base2, bandit_force_id_t, controller_index, bandit_force_index_t, /*bandit_person_index*/1);
				}
			}
			if (flag_debate_faied)
			{

				int bandit_facility_id = 敌军设施[bandit_force_index_t];//设施_根据地1;	
				int controller_index = get_controller_index_of_force(force_);	//玩家or电脑

				pk::building@ new_bandit_base = func_基本事件1_贼将势力根据地_生成(force_, city_, building_, bandit_force_id_t, bandit_facility_id, bandit_force_index_t);

				func_基本事件_画面移动(force_, new_bandit_base.pos);
				//pk::trace(pk::format("执行进攻，目标势力id：{}, 名：{}", force_.get_id(), force__name));
				func_基本事件_贼将势力部队_生成(city_, building_, new_bandit_base, bandit_force_id_t, controller_index, bandit_force_index_t, /*bandit_person_index*/0);
				func_基本事件_贼将势力部队_生成(city_, building_, new_bandit_base, bandit_force_id_t, controller_index, bandit_force_index_t, /*bandit_person_index*/1);
				if (current_period <= 1) pk::kill(new_bandit_base);

				if (current_period == 4)
				{
					pk::building@ new_bandit_base2 = func_基本事件1_贼将势力根据地_生成(force_, city_, building_, bandit_force_id_t, bandit_facility_id, bandit_force_index_t);
					func_基本事件_画面移动(force_, new_bandit_base2.pos);
					//pk::trace(pk::format("执行进攻，目标势力id：{}, 名：{}", force_.get_id(), force__name));
					func_基本事件_贼将势力部队_生成(city_, building_, new_bandit_base2, bandit_force_id_t, controller_index, bandit_force_index_t, /*bandit_person_index*/0);
					func_基本事件_贼将势力部队_生成(city_, building_, new_bandit_base2, bandit_force_id_t, controller_index, bandit_force_index_t, /*bandit_person_index*/1);
				}
			}
			// 计算行动力消耗
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			// 异族外交者设为已行动
			actor.action_done = true;

			// 计算金消耗
			pk::add_gold(building_, -GOLD_COST, true);

			return true;

		}

		void scene()
		{
			if (bandit_force_id_t == 势力_南蛮) pk::play_bgm_file(pk::core_dir + "data/media/BGM/yizu/nanmang.ogg", false, 100);
			//获取异族王庭坐标
			pk::point yizu_pos;
			yizu_pos.x = 异族_坐标[bandit_force_index_t - 1][0];
			yizu_pos.y = 异族_坐标[bandit_force_index_t - 1][1];
			pk::move_screen(yizu_pos);
			pk::fade(0);
			pk::sleep();
			pk::background(场景_宫廷1);  // 배경: 관청
			pk::fade(255);
			if (check_skill(actor, bandit_force_id_t)) pk::message_box(pk::encode("我们老大在里面等你。"), pk::get_person(敌部队_武将[1][bandit_force_index_t]));
			else pk::message_box(pk::encode("那边那个，我们老大让你进来。"), pk::get_person(敌部队_武将[1][bandit_force_index_t]));

			pk::diplomacy(dst_kunshu, null, actor, pk::diplomacy_t(scene_choose));
			//pk::trace(pk::format("4是否达成交易：{}", flag_deal));
			pk::fade(0);
			pk::sleep();
			pk::background(-1);  // 배경: 관청
			pk::fade(255);
			pk::move_screen(city_.pos);
		}

		void scene_choose()
		{
			
			//城市选择
			pk::list<pk::building@> target_building_list = get_Attackable_building_list(yizu_force);
			//if (target_building_list.count == 0) return false; 


			//准备选项
			array<string> issue_list;
			issue_list =
			{
				pk::encode("协议侵略"),
				pk::encode("实行亲善")
			};
			if ((ch::has_skill(actor, 特技_亲乌) and bandit_force_id_t == 势力_乌丸) or (ch::has_skill(actor, 特技_亲羌) and bandit_force_id_t == 势力_羌))
				issue_list.insertLast(pk::encode("请求马匹"));
			if (ch::has_skill(actor, 特技_亲蛮) and bandit_force_id_t == 势力_南蛮)
				issue_list.insertLast(pk::encode("请求戟"));
			if (ch::has_skill(actor, 特技_亲越) and bandit_force_id_t == 势力_山越)
				issue_list.insertLast(pk::encode("请求枪"));

			//pk::trace(pk::format("武将特技id：{},异族势力：{},是否符合：{}", actor.skill, yizu_force_name, (ch::has_skill(actor, 特技_亲越) and bandit_force_id_t == 势力_山越)));
			//for(int i = 0; i < int(issue_list.length); i++)
			//{
			//	pk::trace(pk::format("数组第{}位为：{}", i, pk::decode(issue_list[i])));
			//}

			array<string> reply_list =
			{
				pk::encode("你来我们这里有何贵干呀?"),
				pk::encode("你这憨憨，是想让我们进攻你们自己城市吗？"),
				pk::encode("你们比我们还穷，好意思来送礼嘛。"),
				pk::encode("你傻了? 想清楚你的目的。"),
				pk::encode("别磨蹭了，没什么事就别打扰我!"),
			};
			array<string> reply_list2 =
			{
				pk::encode(pk::format(get_siqin_call(actor.get_id()) + "，什么风把你吹来了。", pk::decode(actor.mei))),
				pk::encode(pk::format("贤{}，莫不是想要我们攻打你们城市？",actor.sex == 0 ? "弟" : "妹")),
				pk::encode(pk::format("贤{}，你们家底也不够呀。",actor.sex == 0 ? "弟" : "妹")),
				pk::encode("你傻了? 想清楚你的目的。"),
				pk::encode("别磨蹭了，没什么事就别打扰我!"),
			};

			int answer = -1;

			int impationt_count = 0;//错误答案的次数，如果老是选择不能执行的选项，异族首领会赶人
			do
			{
				if (check_skill(actor, bandit_force_id_t))
				{
					if (answer == -1) pk::open_message_box(reply_list2[0], dst_kunshu);
					else pk::open_message_box(reply_list2[pk::min(4, answer + 1)], dst_kunshu);
				}
				else
				{
					if (answer == -1) pk::open_message_box(reply_list[0], dst_kunshu);
					else pk::open_message_box(reply_list[pk::min(4, answer + 1)], dst_kunshu);
				}
				answer = pk::choose(issue_list);
				pk::close_message_box();
				if (!check_answer(answer, target_building_list.count, impationt_count))
					impationt_count += 1;
				//pk::trace(pk::format("不耐烦计数：{}", impationt_count));

			} while (!check_answer(answer, target_building_list.count, impationt_count));


			if (impationt_count >= 不耐烦次数)
			{
				pk::message_box(pk::encode("看来你们并没有交涉的意愿，请回吧！"), dst_kunshu);
			}
			else
			{
				if (answer == 0)
				{
					//设定变化，所以下面的攻击目标需要变化获取方式，直接数组
					//pk::list<pk::building@> target_building_list = get_Attackable_building_list(yizu_force);
					pk::list<pk::building@> list_building;
					do
					{
						list_building = pk::building_selector(pk::encode("选择要攻打的据点"), pk::encode("选择要攻打的据点."), target_building_list, 1, 1);
					} while (list_building.count == 0);
					//pk::trace(pk::format("{},{},{}",target_building_list[0].get_id(),target_building_list[1].get_id(),target_building_list[2].get_id()));
					@target_building = list_building[0];
					//pk::trace(pk::format("目标据点id：{}", target_building.get_id()));

					@ target_city = pk::building_to_city(target_building);
					int target_force_id = target_building.get_force_id();
					@ target_force = pk::get_force(target_force_id);
					@ target_gunshi = pk::get_person(target_force.gunshi);
					target_force_name = pk::decode(pk::get_name(target_force));

					//当有可进攻城市的处理
					if (pk::rand_bool(目标势力军师在场率) and target_gunshi !is null)
						scene_reinforcement_2();
					else scene_reinforcement();
				}
				else if (answer == 1)
					scene_goodwill();
				else scene_equipment();
			}

		}

		bool check_answer(int choose, int city_count, int wrong_time)
		{
			if (choose == 0 and city_count > 0)
				return true;
			if (choose == 1 and city_.gold >= 1100)
				return true;
			if (choose == 2)
				return true;
			if (wrong_time >= 不耐烦次数)
				return true;
			return false;
		}
		bool check_skill(pk::person@ person0, int force_id)
		{
			if (ch::has_skill(person0, 特技_亲乌) and force_id == 势力_乌丸)
				return true;
			if (ch::has_skill(person0, 特技_亲羌) and force_id == 势力_羌)
				return true;
			if (ch::has_skill(person0, 特技_亲蛮) and force_id == 势力_南蛮)
				return true;
			if (ch::has_skill(person0, 特技_亲越) and force_id == 势力_山越)
				return true;
			return false;
		}
		//在此处添加赠与宝物的设定
		void scene_goodwill()
		{
			pk::message_box(pk::encode("此番前来是想和贵军改善下关系。"), actor);
			pk::message_box(pk::encode(pk::format("嗯，不错，把你们贡品呈上来吧。", actor.mei)), dst_kunshu);
			pk::int_bool given_money = pk::numberpad(pk::encode("亲善资金"), 1000, city_.gold - 100, 1000, pk::numberpad_t(numberpad_t));
			pk::message_box(pk::encode(pk::format("这里有金钱{}，\n还请笑纳。", given_money.first)), actor);

			goodwill_gold_cost = -given_money.first;
			pk::add_relations(force_, bandit_force_id_t, int(given_money.first / 500));//外交关系是相互的
			int item_id = check_item(dst_kunshu, bandit_force_id_t);
			//pk::trace(pk::format("item_id：{}，relation：{},skill:{}", item_id, force_.relations[bandit_force_id_t], check_skill(actor, bandit_force_id_t)));
			if (pk::rand_bool(30) and force_.relations[bandit_force_id_t] > 90 and check_skill(actor, bandit_force_id_t) and item_id != -1)//只有友好度大于90,且具备对应亲x的武将作为使者才会触发赠宝剧情
			{
				string item_name = pk::decode(pk::get_name(pk::get_item(item_id)));
				string text1 = pk::encode(pk::format("哈哈哈，贵军和我们可真是兄弟之邦呀~这是\x1b[17x{}\x1b[0x，以后阁下只要以此物相邀，哪怕刀山火海，我军也必来相助。", item_name));
				pk::message_box(text1, dst_kunshu);
				pk::play_se(12);
				pk::message_box(pk::encode("感谢大哥！得此符相助，我军必如虎添翼，势不可挡！"), actor);
				//pk::transfer_item();//用这个函数貌似会触发history_log
				pk::get_item(item_id).owner = actor.get_id();
				pk::history_log(actor.get_pos(), pk::get_force(bandit_force_id_t).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x将\x1b[17x{}\x1b[0x赠与了\x1b[1x{}\x1b[0x。",pk::decode(pk::get_name(dst_kunshu)), item_name, pk::decode(pk::get_name(actor)))));
			}
			else
			{
				pk::message_box(pk::encode("哦......"), dst_kunshu);
				pk::message_box(pk::encode(pk::format("好，明白了，\n代我向\x1b[2x{}\x1b[0x大人问声好啊。", pk::decode(pk::get_name(src_kunshu)))), dst_kunshu);
			}

			flag_goodwill = true;
		}

		int check_item(pk::person@person0, int force_id)
		{
			if (force_id == 势力_羌)
			{
				if (pk::get_item(宝物_羌骑兵兵符).owner == person0.get_id())
				{
					return 宝物_羌骑兵兵符;
				}
			}
			if (force_id == 势力_乌丸)
			{
				if (pk::get_item(宝物_乌丸兵兵符).owner == person0.get_id())
				{
					return 宝物_乌丸兵兵符;
				}
				if (pk::get_item(宝物_匈奴兵兵符).owner == person0.get_id())
				{
					return 宝物_匈奴兵兵符;
				}
			}
			if (force_id == 势力_南蛮)
			{
				if (pk::get_item(宝物_蛮族兵兵符).owner == person0.get_id())
				{
					return  宝物_蛮族兵兵符;
				}
			}
			if (force_id == 势力_山越)
			{
				if (pk::get_item(宝物_山越兵兵符).owner == person0.get_id())
				{
					return  宝物_山越兵兵符;
				}
			}
			return -1;
		}

		void scene_equipment()
		{
			int yizu_weapon = -1;
			switch (bandit_force_index_t - 1)
			{
			case 0: yizu_weapon = 4; break;
			case 1: yizu_weapon = 4; break;
			case 2: yizu_weapon = 1; break;
			case 3: yizu_weapon = 2; break;
			default: yizu_weapon = -1; break;
			}
			string weapon_name;
			switch (yizu_weapon)
			{
			case 0: weapon_name = "剑"; break;
			case 1: weapon_name = "枪"; break;
			case 2: weapon_name = "戟"; break;
			case 3: weapon_name = "弩"; break;
			case 4: weapon_name = "马"; break;
			default: weapon_name = "??"; break;
			}

			int weapon_max = pk::min(5000 + current_period * 1000 + 500 * pk::rand(2), int(city_.food / 120) * 100);
			int give_food_min = int(weapon_max * 1.2f);

			//pk::message_box(pk::encode(pk::format("\x1b[2x{}弟\x1b[0x，好久不见。", pk::decode(actor.mei))), dst_kunshu);			
			pk::message_box(pk::encode(pk::format("哈哈，小{}此番前来，是想和大哥你商量件事情。", actor.sex == 0 ? "弟" : "妹")), actor);
			pk::message_box(pk::encode(pk::format(get_siqin_call(actor.get_id()) + "的事便是我的事。", pk::decode(actor.mei))), dst_kunshu);
			pk::message_box(pk::encode(pk::format("我们最近战事频繁，军械吃紧，\n小弟知道大哥你这有不少\x1b[1x好{}\x1b[0x，不知能否支援小{}一些。", weapon_name, actor.sex == 0 ? "弟" : "妹")), actor);
			pk::message_box(pk::encode(pk::format("这样呀，正好我手上有\x1b[1x{}{}\x1b[0x，这便赠与贤{}吧。", weapon_max, weapon_name, actor.sex == 0 ? "弟" : "妹")), dst_kunshu);
			pk::message_box(pk::encode(pk::format("怎么可以让大哥凭空吃那么大亏呢，小{}手上有主公请我送来的一些兵粮，还请笑纳。", actor.sex == 0 ? "弟" : "妹")), actor);
			pk::int_bool given_food = pk::numberpad(pk::encode("赠与兵粮"), give_food_min, city_.food, give_food_min, pk::numberpad_t(numberpad_t));
			if (given_food.first >= give_food_min * 2 and given_food.first >= 4000) pk::message_box(pk::encode(pk::format(get_siqin_call(actor.get_id()) + "真是太客气了。", pk::decode(actor.mei))), dst_kunshu);
			else pk::message_box(pk::encode(pk::format("哈哈，你我不必见外。", pk::decode(actor.mei))), dst_kunshu);

			weapon_gain = weapon_max;
			weapon_gain_type = yizu_weapon;
			equipment_food_cost = -given_food.first;

			pk::add_relations(force_, bandit_force_id_t, 3);
			if (given_food.first >= give_food_min * 2 and given_food.first >= 4000)
				pk::add_relations(force_, bandit_force_id_t, 3);


			flag_equipment = true;
		}

		void scene_reinforcement()
		{
			//pk::list<pk::force@> person_list = pk::get_force_list();
			//pk::list<pk::force@> force_sel = pk::force_selector(pk::encode("执行刺杀武将选择"), pk::encode("选择执行刺杀的武将."), person_list, 1, 1);
			// pk::list<pk::city@> person_list = pk::get_city_list();
			//pk::list<pk::city@> force_sel = pk::city_selector2(pk::encode("执行刺杀武将选择"), pk::encode("选择执行刺杀的武将."), person_list, 1, 1);
			//pk::list<pk::building@> person_list2 = pk::get_building_list();
			//pk::list<pk::building@> force_sel2 = pk::building_selector(pk::encode("执行刺杀武将选择"), pk::encode("选择执行刺杀的武将."), person_list2, 1, 1);

			// 간단한 대화
			pk::message_box(pk::encode("此番前来是想和贵军进行一场交易?"), actor);
			pk::message_box(pk::encode(pk::format("有话快说，别咬文嚼字的.", actor.mei)), dst_kunshu);
			//target_force_name = pk::decode(pk::get_name(target_force));
			int city_id_t = target_city.get_id();
			pk::city @ city_t = pk::get_city(city_id_t);

			string city_name = pk::decode(pk::get_name(city_t));
			//pk::trace(pk::format("目标城市id：{}，城市名：{}", city_id_t, city_name));
			pk::message_box(pk::encode(pk::format("听闻贵军对{}城有意，我军想资助贵军进行战斗?", city_name)), actor);
			pk::message_box(pk::encode("有这等好事?"), dst_kunshu);
			pk::message_box(pk::encode("自然"), actor);
			pk::message_box(pk::encode("我等大老远出兵，人吃马嚼的，可不便宜，你们准备出多少?"), dst_kunshu);

			//讨价还价
			flag_deal = false;
			current_period = get_current_period(force_);
			expected_gold = get_expected_gold();
			pk::int_bool given_money;
			int count_ = 0;
			duel_result = -1;
			do
			{
				given_money = pk::numberpad(pk::encode("军费资助"), 0, city_.gold - 100, 0, pk::numberpad_t(numberpad_t));
				pk::message_box(pk::encode(pk::format("我军愿意出{}金作为军费，你看如何？", given_money.first)), actor);
				if (given_money.first >= 0.9f * expected_gold and given_money.first < expected_gold and pk::rand_bool(异族请求单挑率))
				{
					pk::message_box(pk::encode("这样讨价还价真没意思，要不你和我打一架。打赢了，就以现在的价格成交。"), dst_kunshu);
					bool accept_duel = (pk::choose({ pk::encode(" 是 "), pk::encode(" 否 ") }, pk::encode(pk::format("是否接受单挑？", GOLD_COST)), null) == 0);
					if (accept_duel)
					{
						pk::unit@ dummy_unit;
						pk::person@ dummy_person;
						duel_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, dst_kunshu, dummy_person, dummy_person, true, false, 0, true).first;
						if (duel_result == 0)
						{
							pk::message_box(pk::encode("好汉子，确实有两下子。"), dst_kunshu);
							expected_gold = int(0.9f * expected_gold);
							pk::message_box(pk::encode("承让了。"), actor);
							pk::message_box(pk::encode("行，那就现在这个价格吧。"), dst_kunshu);
						}
						if (duel_result == 1)
						{
							pk::message_box(pk::encode("汉人看来果然不过是绣花枕头，与你们这样的人合作，看来得重新考虑下价格才是。"), dst_kunshu);
							expected_gold = int(1.1f * expected_gold);
						}
					}
					if (!accept_duel)
					{
						pk::message_box(pk::encode("真无趣，这点胆量都没有...\n(看来得重新考虑下价格才是。)"), dst_kunshu);
						expected_gold = int(1.05f * expected_gold);
					}
				}


				if (given_money.first < expected_gold)
					pk::message_box(pk::encode("就这点钱？你在开玩笑吗?"), dst_kunshu);
				count_ += 1;
			} while (given_money.first < expected_gold and count_ < 3);

			if (given_money.first >= expected_gold)
			{
				pk::message_box(pk::encode(pk::format("确定要给我{}金？.", given_money.first)), dst_kunshu);
				pk::message_box(pk::encode("哈哈哈！既然你们这么有诚意，那我们就出兵吧?"), dst_kunshu);
				pk::add_gold(building_, -given_money.first);
				flag_deal = true;

			}
			else
			{
				pk::message_box(pk::encode("看来没有谈下去的必要了，请回吧！"), dst_kunshu);
				flag_deal = false;

			}


		}

		void scene_reinforcement_2()
		{

			flag_deal = false;
			// 간단한 대화
			pk::message_box(pk::encode("此番前来是想和贵军进行一场交易?"), actor);
			pk::message_box(pk::encode(pk::format("有话快说，别咬文嚼字的.", actor.mei)), dst_kunshu);
			//target_force_name = pk::decode(pk::get_name(target_force));
			int city_id_t = target_city.get_id();
			pk::city @ city_t = pk::get_city(city_id_t);

			string city_name = pk::decode(pk::get_name(city_t));
			//pk::trace(pk::format("目标城市id：{}，城市名：{}", city_id_t, city_name));
			pk::message_box(pk::encode(pk::format("听闻贵军对{}城有意，我军想资助贵军进行战斗?", city_name)), actor);
			pk::message_box(pk::encode(pk::format("这样啊...\n正好{}军的军师也在此，你俩分别陈述下利害怎么样?", target_force_name)), dst_kunshu);
			bool accept_debate = (pk::choose({ pk::encode(" 是 "), pk::encode(" 否 ") }, pk::encode(pk::format("是否接受辩论？", GOLD_COST)), null) == 0);
			if (accept_debate)
			{

				debate_result = pk::debate(actor, target_gunshi, true, false, true, true).first;
				if (debate_result == 0)
				{
					pk::message_box(pk::encode(pk::format("看来果然还是帮助{}军比较有利呀...", src_force_name)), dst_kunshu);
					pk::message_box(pk::encode(pk::format("来人，把{}赶出去.", pk::decode(pk::get_name(target_gunshi)))), dst_kunshu);
					flag_debate_faied = false;
					pk::message_box(pk::encode("可恶，这下该如何和主公交代..."), target_gunshi);
				}
				if (debate_result == 1)
				{
					pk::message_box(pk::encode(pk::format("看来果然还是帮助{}军比较有利呀...", target_force_name)), dst_kunshu);
					pk::message_box(pk::encode(pk::format("来人，把{}赶出去.", pk::decode(pk::get_name(actor)))), dst_kunshu);
					flag_debate_faied = true;
					pk::message_box(pk::encode("可恶，这下该如何和主公交代..."), actor);
				}
			}
			if (!accept_debate)
			{
				pk::message_box(pk::encode(pk::format("{}军派来的使者就就只有这点胆量嘛，那这便滚吧...", src_force_name)), dst_kunshu);
			}

			//辩论成功才进入讨价还价阶段，此时就不触发单挑了
			if (!flag_debate_faied and accept_debate)
			{
				pk::message_box(pk::encode("虽然确实和你们交易比较有利，不过我等大老远出兵，人吃马嚼的，成本也不小..."), dst_kunshu);

				//讨价还价
				flag_deal = false;
				current_period = get_current_period(force_);
				//作为舌战胜利的奖励，预期价格降低
				expected_gold = int(get_expected_gold() * 0.5f);
				pk::int_bool given_money;
				int count_ = 0;
				do
				{
					given_money = pk::numberpad(pk::encode("军费资助"), 0, city_.gold - 100, 0, pk::numberpad_t(numberpad_t));
					pk::message_box(pk::encode(pk::format("我军愿意出{}金作为军费，你看如何？", given_money.first)), actor);
					if (given_money.first < expected_gold)
						pk::message_box(pk::encode("就这点钱？你在开玩笑吗?"), dst_kunshu);
					count_ += 1;
				} while (given_money.first < expected_gold and count_ < 3);

				if (given_money.first >= expected_gold)
				{
					pk::message_box(pk::encode(pk::format("确定要给我{}金？.", given_money.first)), dst_kunshu);
					pk::message_box(pk::encode("哈哈哈！既然你们这么有诚意，那我们就出兵吧?"), dst_kunshu);
					pk::add_gold(building_, -given_money.first);
					flag_deal = true;

				}
				else
				{
					pk::message_box(pk::encode("看来没有谈下去的必要了，请回吧！"), dst_kunshu);
					flag_deal = false;
				}
			}
		}

		string numberpad_t(int line, int original_value, int current_value)
		{
			return pk::encode("");
		}



		pk::list<pk::force@> getYizuList()
		{
			pk::list<pk::force@> list;
			for (int i = 42; i < 46; i++)
			{
				pk::force@ yizu_force = pk::get_force(i);
				if (yizu_force !is null)
					list.add(yizu_force);
			}
			return list;
		}


		pk::list<pk::city@> get_Attackable_city_list(pk::force@ src/*异族势力*/)
		{
			pk::list<pk::city@> list;
			int index = get_bandit_index(src);
			for (int i = 0; i < 城市_末; i++)
			{
				pk::city@ dst = pk::get_city(i);
				if (异族出现城市[index - 1].find(i) >= 0)
					list.add(dst);
			}
			return list;
		}

		pk::list<pk::building@> get_Attackable_building_list(pk::force@ src/*异族势力*/)
		{
			pk::list<pk::building@> list;
			int index = get_bandit_index(src);
			for (int i = 0; i < 城市_末; i++)
			{
				pk::building@ dst = pk::get_building(i);
				//pk::trace(pk::format("势力id:{}, index：{},城市id：{}", src.get_id(), index, i));
				//pk::trace(pk::format("index：{},城市id：{}, {}", index, i, 异族出现城市[index - 1]));
				if (异族出现城市[index - 1].find(i) >= 0 and dst.get_force_id() != force_.get_id())
					list.add(dst);
			}
			return list;
		}

		pk::list<pk::building@> getTargetList(pk::building@ src)
		{
			pk::list<pk::building@> list;
			for (int i = 0; i < 建筑_据点末; i++)
			{
				pk::building@ dst = pk::get_building(i);
				int src_id = src.get_id();
				int dst_id = dst.get_id();
				//pk::trace(pk::format("对象据点{}，我方据点{}，相邻据点:{}，距离:{}",pk::decode(pk::get_name(dst)), pk::decode(pk::get_name(src)),pk::is_neighbor_base(src_id, dst_id), pk::get_building_distance(dst_id, src_id, src.get_force_id())));
				if (src_id != dst_id and /*pk::is_neighbor_base(src_id, dst_id)*/pk::get_building_distance(dst_id, src_id, src.get_force_id()) <= 20 and src.get_force_id() != dst.get_force_id())
					list.add(dst);
			}
			return list;
		}
		int get_expected_gold()
		{

			float relation_buf = float(2.5f - yizu_force.relations[force_.get_id()] / 50.f);
			float stat_buf = float(3.0f - (actor.stat[武将能力_政治] - dst_kunshu.stat[武将能力_政治]) / 40.f);

			float skill_buf = 1.0f;
			int bandit_force_index = get_bandit_force_index_of_city(target_city.get_id());
			if (ch::has_skill(actor, 特技_亲羌) and bandit_force_index == 1)
				skill_buf = 军费_四亲buf;
			if (ch::has_skill(actor, 特技_亲乌) and bandit_force_index == 2)
				skill_buf = 军费_四亲buf;
			if (ch::has_skill(actor, 特技_亲越) and bandit_force_index == 3)
				skill_buf = 军费_四亲buf;
			if (ch::has_skill(actor, 特技_亲蛮) and bandit_force_index == 4)
				skill_buf = 军费_四亲buf;
			season_buf = 军费_季节buf[bandit_force_index - 1][pk::get_season()] / 100.f;

			//相性差:羌以马腾相性(70)还是董卓相性(149)为准？和南蛮以孟获(60)为基准，乌丸以公孙家族（142）为准，山越以严白虎(10)为准
			float aishou_buf = 1.0f;
			if (bandit_force_index == 1)
				aishou_buf = float(pk::get_aishou_distance(actor, 武将_马腾) / 100.f + 0.5f);
			if (bandit_force_index == 2)
				aishou_buf = float(pk::get_aishou_distance(actor, 武将_公孙恭) / 100.f + 0.5f);
			if (bandit_force_index == 3)
				aishou_buf = float(pk::get_aishou_distance(actor, 武将_严白虎) / 100.f + 0.5f);
			if (bandit_force_index == 4)
				aishou_buf = float(pk::get_aishou_distance(actor, 武将_孟获) / 100.f + 0.5f);

			//int n = int(skill_buf * season_buf * (异族基础军费 + 异族附加军费 * aishou_buf * relation_buf * stat_buf));
			int n = int(skill_buf * season_buf * (异族基础军费[bandit_force_index - 1][current_period] + 异族附加军费[bandit_force_index - 1][current_period] * aishou_buf * relation_buf * stat_buf));
			//pk::trace(pk::format("所处阶段：{}，异族基础军费：{}，异族附加军费：{}", current_period, 异族基础军费[bandit_force_index - 1][current_period], 异族附加军费[bandit_force_index - 1][current_period]));
			//pk::trace(pk::format("相性buf：{}，友好度buf：{}，能力buf：{}，特技buf：{}，季节buf：{}，预计金：{}", aishou_buf, relation_buf, stat_buf, skill_buf, season_buf, n));

			return n;

		}

		int get_current_period(pk::force@ force)
		{
			int period = 0;
			//目前考虑不管金粮兵装，只管实在的战力，即城数，武将数，兵力数
			int c_months = pk::get_elapsed_months();
			int c_city_count = pk::get_city_count(force);
			int c_troops = pk::get_troops(force);
			//int c_gold = ch::get_force_gold(force);
			//int c_food = ch::get_force_food(force);
			//int c_weapon =  ch::get_force_weapon(force);
			int c_person_count = ch::get_force_person_count(force);



			if (c_months > 24 and (c_city_count > 1 or c_person_count > 20 or c_troops > 60000))
				period = 1;	//对应3队2w
			//超额完成时提前进入下一阶段
			if (c_city_count > 1 and c_person_count > 20 and c_troops > 60000)
				period = 1;
			if (c_months > 36 and (c_city_count > 3 or c_troops > 150000))
				period = 2;//对应1贼窝5队，约4万兵
			if (c_months > 96 and (c_city_count > 5 or c_troops > 300000))
				period = 3;//对应2贼窝10队，约8万兵
			if (c_city_count > 5 and c_troops > 300000 and c_person_count > 80)
				period = 3;//对应2贼窝10队，约8万兵				
			if (c_months > 240 and (c_city_count > 15 or c_troops > 800000))
				period = 4;//对应3贼窝15队，约15万兵
			if (c_city_count > 15 and c_troops > 800000 and c_person_count > 150)
				period = 4;//对应3贼窝15队，约15万兵			
			//pk::trace(pk::format("经过月份：{}，城市数：{}，兵力：{}，武将数：{}，所处阶段：{}", c_months, c_city_count, c_troops, c_person_count, period));
			return period;
		}

		bool check_intelligence(pk::person@ src, pk::person@ dst)
		{
			if (src.stat[武将能力_智力] < dst.stat[武将能力_智力] or (src.stat[武将能力_智力] - dst.stat[武将能力_智力]) <= 10)
				return true;
			return false;
		}

		//	获取当前势力是玩家or电脑
		int get_controller_index_of_force(pk::force@ force)
		{
			return pk::is_valid_force_id(force.get_id()) ? (force.is_player() ? 0 : 1) : 2;
		}
		//-----------------------贼根据地生成相关-------------------------------------//
		/// 在特定坐标上生成特定势力的特定设施建筑物并完成该建筑物。
		pk::building@ func_特定势力设施_建筑生成(pk::point pos, int force_id, int facility_id)
		{
			pk::building@ building = pk::create_building(pk::get_facility(facility_id), pos, force_id);
			pk::complete(building);
			pk::set_hp(building, pk::get_max_hp(building));
			building.update();

			return building;
		}

		pk::building@ func_基本事件1_贼将势力根据地_生成(pk::force@ force, pk::city@ city, pk::building@ building, int bandit_force_id, int bandit_facility_id, int bandit_force_index)
		{
			array<pk::point> new_bandit_base_pos_arr = func_根据地_生成可能坐标_数组(city, building, 根据地生成_距离);
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

		array<pk::point> func_根据地_生成可能坐标_数组(pk::city@ city, pk::building@ building, array<int> distance_arr)
		{
			array<pk::point> candidate_bandit_base_pos_arr(0);

			array<pk::point> range_pos_arr = pk::range(building.pos, /*distance_min*/distance_arr[0], /*distance_max*/distance_arr[1]);
			for (int arr_index = int(range_pos_arr.length) - 1; arr_index >= 0; arr_index--)
			{
				pk::point pos = range_pos_arr[arr_index];
				if (!pk::is_valid_pos(pos)) continue;
				if (pk::get_city_id(pos) != city.get_id()) continue;
				if (!func_特定坐标周围_对象生成_是否可行(pos, /*distance_min*/1,/*distance_max*/1)) continue;

				pk::hex@ hex = pk::get_hex(pos);
				if (!func_特定坐标_军事设施建设_是否可能(hex)) continue;

				int terrain_id = hex.terrain;
				if (!pk::is_valid_terrain_id(terrain_id)) continue;
				if (!pk::is_enabled_terrain(terrain_id)) continue;
				if (根据地生成_禁止地形.find(terrain_id) >= 0) continue;

				candidate_bandit_base_pos_arr.insertLast(pos);
			}

			return candidate_bandit_base_pos_arr;
		}

		/// 确认在一定距离内的坐标中是否存在可創建对象的坐标 특정 좌표로부터 일정 거리 이내의 좌표 중 오브젝트 생성이 가능한 좌표가 존재하는지 확인
		bool func_特定坐标周围_对象生成_是否可行(pk::point pos, int distance_min, int distance_max)
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

		/// 특정 칸이 군사시설의 건설이 가능한지 확인
		bool func_特定坐标_军事设施建设_是否可能(pk::hex@ hex)
		{
			return (!hex.has_building and !hex.has_unit and hex.can_military and !hex.development);
		}

		/// 특정 좌표에서 특정 효과 및 특정 효과음 실행
		void func_特定坐标_效果执行(int sound_index, int effect_index, pk::point pos)
		{
			if (!pk::is_in_screen(pos)) return;

			pk::play_se(sound_index);
			pk::create_effect(effect_index, pos);
		}

		void func_基本事件_画面移动(pk::force@ force, pk::point event_pos)
		{
			//if (!force.is_player()) return;

			pos_for_screen_move = event_pos;
			pk::scene(cast<pk::scene_t@>(function() { pk::move_screen(main.pos_for_screen_move, 500); }));	// 작은 숫자일수록 빠른 이동
		}

		void func_事件_log信息_显示(pk::city@ city, int bandit_force_id, pk::point event_pos, int bandit_force_index, int history_log_index)
		{
			string str_event_object = (history_log_index == 0) ? "据点" : (history_log_index == 1 ? "部队" : "");
			string str_event_msg = get_city_name(city) + "周边出现了" + get_bandit_force_name_for_log(bandit_force_index) + str_event_object + "!";
			pk::history_log(event_pos, pk::get_force(bandit_force_id).color, pk::encode(str_event_msg));
		}

		int get_bandit_force_index_of_city(int city_id)
		{
			for (int bandit_force_index = 1; bandit_force_index < 5; bandit_force_index++)
			{
				if (异族出现城市[bandit_force_index - 1].find(city_id) >= 0) return bandit_force_index;
			}

			return 0;
		}
		//	获取当前据点所属贼种类id-扩充距离版本
		int get_bandit_force_index_of_city_2(int city_id)
		{
			for (int bandit_force_index = 1; bandit_force_index < 5; bandit_force_index++)
			{
				if (请求异族城市[bandit_force_index - 1].find(city_id) >= 0) return bandit_force_index;
			}

			return 0;
		}

		//	获取当前据点所属贼种类
		int get_bandit_force_id_of_city(int city_id)
		{
			for (int bandit_force_index = 1; bandit_force_index < 5; bandit_force_index++)
			{
				if (异族出现城市[bandit_force_index - 1].find(city_id) >= 0) return 异族势力[bandit_force_index];
			}

			return 势力_盗贼;
		}

		//	获取当前据点所属贼种类-扩充距离版本
		int get_bandit_force_id_of_city_2(int city_id)
		{
			for (int bandit_force_index = 1; bandit_force_index < 5; bandit_force_index++)
			{
				if (请求异族城市[bandit_force_index - 1].find(city_id) >= 0) return 异族势力[bandit_force_index];
			}

			return 势力_盗贼;
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

		//-----------------------贼部队生成相关-------------------------------------//
		//基本事件_敌军部队_生成
		void func_基本事件_贼将势力部队_生成(pk::city@ city, pk::building@ building, pk::building@ bandit_base, int bandit_force_id, int controller_index, int bandit_force_index, int bandit_person_index)
		{
			//部队数_年[peroiod][is_player][头目or将][异族id]
			//pk::trace(pk::format("部队数：{}，时期：{}，控制者：{}，人物：{}，势力：{}", 部队数_年[current_period][controller_index][bandit_person_index][bandit_force_index], current_period, controller_index, bandit_person_index, bandit_force_index));

			//for (int count = 0; count < 部队数[controller_index][bandit_person_index][bandit_force_index]; count++)
			for (int count = 0; count < 部队数_年[current_period][controller_index][bandit_person_index][bandit_force_index]; count++)
			{
				//pk::trace(pk::format("是否存在贼将空位：{}，存在部队数：{}，是否可生成：{}", func_贼将_空位_是否存在(), func_贼将势力_全部部队数_合计(), func_特定坐标周围_对象生成_是否可行(bandit_base.pos, /*distance_min*/1, /*distance_max*/1)));
				if (!func_贼将_空位_是否存在()) break;//沒有空余武将位则不生成
				if (func_贼将势力_全部部队数_合计() >= 100) break;//异族势力部队总数大於100，不生成
				if (!pk::is_normal_force(city.get_force_id())) { func_空城区域内_敌军部队清除(city, bandit_force_id); break; }//空城则?除部队，不生成
				if (!func_特定坐标周围_对象生成_是否可行(bandit_base.pos, /*distance_min*/1, /*distance_max*/1)) break;

				pk::person@ bandit_person = func_贼将_武将_生成(bandit_force_id, bandit_force_index, bandit_person_index);
				if (!pk::is_alive(bandit_person)) { pk::reset(bandit_person); continue; }

				//pk::unit@ bandit_unit = func_贼将_部队_生成(bandit_base, bandit_person, bandit_force_index, bandit_person_index);
				pk::unit@ bandit_unit = func_贼将_部队_生成_改(bandit_base, bandit_person, bandit_force_index, bandit_person_index, controller_index, current_period);
				if (!pk::is_alive(bandit_unit)) { pk::kill(bandit_unit); pk::reset(bandit_person); continue; }
				if (int(pk::get_movable_pos(bandit_unit).length) == 0) { pk::kill(bandit_unit); pk::reset(bandit_person); break; }
				//	if (!func_부대위치좌표_이동가능여부(bandit_unit)) {pk::kill(bandit_unit); pk::reset(bandit_person); continue;}

				int bandit_unit_order = func_敌军部队_最佳任务_决策(city, bandit_unit);
				pk::set_order(bandit_unit, bandit_unit_order, building.pos);

				if (!pk::run_order(bandit_unit)) { pk::kill(bandit_unit); pk::reset(bandit_person); continue; }
			}
		}

		//基本事件_敌军部队_生成
		bool func_基本事件_贼将势力部队_生成_请求版(pk::city@ city, pk::building@ building, pk::building@ bandit_base, int bandit_force_id, int controller_index, int bandit_force_index, int bandit_person_index, int request_force_id)
		{
			//部队数_年[peroiod][is_player][头目or将][异族id]
			//pk::trace(pk::format("部队数：{}，时期：{}，控制者：{}，人物：{}，势力：{}", 部队数_年[current_period][controller_index][bandit_person_index][bandit_force_index], current_period, controller_index, bandit_person_index, bandit_force_index));

			//for (int count = 0; count < 部队数[controller_index][bandit_person_index][bandit_force_index]; count++)
			int create_count = 0;
			for (int count = 0; count < 部队数_年[current_period][controller_index][bandit_person_index][bandit_force_index]; count++)
			{
				//pk::trace(pk::format("是否存在贼将空位：{}，存在部队数：{}，是否可生成：{}", func_贼将_空位_是否存在(), func_贼将势力_全部部队数_合计(), func_特定坐标周围_对象生成_是否可行(bandit_base.pos, /*distance_min*/1, /*distance_max*/1)));
				if (!func_贼将_空位_是否存在()) break;//沒有空余武将位则不生成
				if (func_贼将势力_全部部队数_合计() >= 100) break;//异族势力部队总数大於100，不生成
				if (!pk::is_normal_force(city.get_force_id())) { func_空城区域内_敌军部队清除(city, bandit_force_id); break; }//空城则?除部队，不生成
				if (!func_特定坐标周围_对象生成_是否可行(bandit_base.pos, /*distance_min*/1, /*distance_max*/1)) break;

				pk::person@ bandit_person = func_贼将_武将_生成(bandit_force_id, bandit_force_index, bandit_person_index);
				if (!pk::is_alive(bandit_person)) { pk::reset(bandit_person); continue; }

				//pk::unit@ bandit_unit = func_贼将_部队_生成(bandit_base, bandit_person, bandit_force_index, bandit_person_index);
				pk::unit@ bandit_unit = func_贼将_部队_生成_改(bandit_base, bandit_person, bandit_force_index, bandit_person_index, controller_index, current_period);
				if (!pk::is_alive(bandit_unit)) { pk::kill(bandit_unit); pk::reset(bandit_person); continue; }
				if (int(pk::get_movable_pos(bandit_unit).length) == 0) { pk::kill(bandit_unit); pk::reset(bandit_person); continue; }
				//	if (!func_부대위치좌표_이동가능여부(bandit_unit)) {pk::kill(bandit_unit); pk::reset(bandit_person); continue;}

				//int bandit_unit_order = func_敌军部队_最佳任务_决策(city, bandit_unit);
				//pk::set_order(bandit_unit, bandit_unit_order, building.pos);

				bandit_unit.requested_from = request_force_id;
				pk::set_order(bandit_unit, 部队任务_征服, building.pos);
				create_count += 1;
				//pk::set_ceasefire_timer(pk::get_force(bandit_force_id), request_force_id,9);
				if (!pk::run_order(bandit_unit)) { pk::kill(bandit_unit); pk::reset(bandit_person); continue; }
			}
			if (create_count > 0) return true;
			return false;
		}

		//遍历所有武将，身??死亡或无的都可用来生成
		bool func_贼将_空位_是否存在()
		{
			for (int person_id = 敌将_开始; person_id < 敌将_末; person_id++)
			{
				pk::person@ person = pk::get_person(person_id);
				if (person.mibun == 身份_无) return true;
				if (person.mibun == 身份_死亡) return true;
			}

			return false;
		}

		/// 특정 세력의 전체 부대 수 반환
		int func_特定势力_部队数_合计(int force_id)
		{
			pk::force@ force = pk::get_force(force_id);
			return int(pk::get_unit_list(force).count);
		}

		//异族势力_部队总数_统计
		int func_贼将势力_全部部队数_合计()
		{
			int sum_of_bandit_unit = 0;

			for (int bandit_force_index = 0; bandit_force_index < 5; bandit_force_index++)
			{
				int bandit_force_id = 异族势力[bandit_force_index];
				sum_of_bandit_unit += func_特定势力_部队数_合计(bandit_force_id);
			}

			return sum_of_bandit_unit;
		}

		//敌军武将生成
		pk::person@ func_贼将_武将_生成(int bandit_force_id, int bandit_force_index, int bandit_person_index)
		{
			int person_id = 敌部队_武将[bandit_person_index][bandit_force_index];
			pk::person@ bandit_person = pk::create_bandit(pk::get_person(person_id));
<<<<<<< HEAD
			if (person_id == 武将_盗贼) pk::set_mibun(bandit_person,身份_一般); //防止出现在野贼兵
=======
			if (bandit_person.mibun == 身份_在野) pk::set_mibun(bandit_person,身份_一般); //防止出现在野贼兵
			//原版正常出贼都是有所属有所在的，所属为根据地(生成时，实际上打破根据地不会让贼将队消失)，所在为部队
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

			pk::set_district(bandit_person, bandit_force_id);
			bandit_person.base_stat[武将能力_统率] = func_数组_范围_随机值(敌将统率[bandit_person_index][bandit_force_index]);
			bandit_person.base_stat[武将能力_武力] = func_数组_范围_随机值(敌将武力[bandit_person_index][bandit_force_index]);
			bandit_person.base_stat[武将能力_智力] = func_数组_范围_随机值(敌将智力[bandit_person_index][bandit_force_index]);
			for (int heishu_id = 兵种_枪兵; heishu_id < 兵种_末; heishu_id++)
				bandit_person.tekisei[heishu_id] = func_数组_范围_随机值(武将适性[bandit_person_index][bandit_force_index]);
			bandit_person.skill = 武将特技[bandit_person_index][bandit_force_index];
			bandit_person.update();

			return bandit_person;
		}

		/// 范围随机值 : int형 변수 배열(항목 2개)에서 최소값(항목 첫번째 값) 이상 최대값(항목 두번째 값) 이하의 int형 랜덤값을 반환
		int func_数组_范围_随机值(array<int> int_value_arr)
		{
			pk::random random(pk::rand());
			return random(int_value_arr[0], int_value_arr[1]);
		}

		/// 项目随机值 : 변수 배열에서 항목 중 하나를 랜덤으로 하나의 값을 선택하여 해당 항목의 변수 값을 반환
		int func_数组_项目_随机值(array<int> int_value_arr)
		{
			return int_value_arr[pk::rand(int(int_value_arr.length))];
		}
		float func_数组_项目_随机值(array<float> float_value_arr)
		{
			return float_value_arr[pk::rand(int(float_value_arr.length))];
		}

		//	除去空城范围?的贼军部队
		void func_空城区域内_敌军部队清除(pk::city@ city, int bandit_force_id)
		{
			if (pk::is_normal_force(city.get_force_id())) return;	//不是空城，返回

			array<pk::unit@> bandit_unit_arr = pk::list_to_array(pk::get_unit_list(pk::get_force(bandit_force_id)));
			for (int arr_index = 0; arr_index < int(bandit_unit_arr.length); arr_index++)	//遍历該城贼部队，?刪除
			{
				pk::unit@ bandit_unit = bandit_unit_arr[arr_index];
				if (city.get_id() == pk::get_city_id(bandit_unit.pos)) pk::kill(bandit_unit);
			}
		}

		//敌军部队生成
		pk::unit@ func_贼将_部队_生成(pk::building@ bandit_base, pk::person@ bandit_person, int bandit_force_index, int bandit_person_index)
		{
			int ground_wpn = func_数组_项目_随机值(部队陆上兵器[bandit_person_index][bandit_force_index]);//陆上兵器
			int sea_wpn = func_数组_项目_随机值(部队水上兵器[bandit_person_index][bandit_force_index]);//水上兵器
			//部队兵力_年[period][头目or将][异族id][is_player][兵力]
			int troops = func_数组_项目_随机值(部队兵力[bandit_person_index][bandit_force_index]);//兵力
			int gold = 部队金[bandit_person_index][bandit_force_index];//金
			int food = int(troops * 部队兵粮倍率[bandit_person_index][bandit_force_index]);//兵粮
			pk::point emptyPos = getEmptyPos(bandit_base);
			//pk::trace(pk::format("建筑id：{}，武将id：{}，兵力：{}，武器：{}，{}", bandit_base.get_id(), bandit_person.get_id(), troops, ground_wpn, sea_wpn));
			//pk::trace(pk::format("金：{}，粮：{}，坐标x,y：{}，{}", gold, food, emptyPos.x, emptyPos.y));
			pk::unit@ bandit_unit = pk::create_unit(bandit_base, bandit_person, null, null, troops, ground_wpn, sea_wpn, gold, food, emptyPos);

			bandit_unit.energy = 部队气力[bandit_person_index][bandit_force_index];

			return bandit_unit;
		}

		//敌军部队生成-改
		pk::unit@ func_贼将_部队_生成_改(pk::building@ bandit_base, pk::person@ bandit_person, int bandit_force_index, int bandit_person_index, int controller_index, int period)
		{
			int ground_wpn = func_数组_项目_随机值(部队陆上兵器[bandit_person_index][bandit_force_index]);//陆上兵器
			int sea_wpn = func_数组_项目_随机值(部队水上兵器[bandit_person_index][bandit_force_index]);//水上兵器
			//部队兵力_年[period][头目or将][异族id][is_player][兵力]
			int troops = func_数组_项目_随机值(部队兵力_年[period][bandit_person_index][bandit_force_index][controller_index]);//兵力
			int gold = 部队金[bandit_person_index][bandit_force_index];//金
			int food = int(troops * 部队兵粮倍率[bandit_person_index][bandit_force_index]);//兵粮
			pk::point emptyPos = getEmptyPos(bandit_base);
			//pk::trace(pk::format("建筑id：{}，武将id：{}，兵力：{}，武器：{}，{}", bandit_base.get_id(), bandit_person.get_id(),troops, ground_wpn, sea_wpn));
			//pk::trace(pk::format("金：{}，粮：{}，坐标x,y：{}，{}", gold, food, emptyPos.x,  emptyPos.y));
			pk::unit@ bandit_unit = pk::create_unit(bandit_base, bandit_person, null, null, troops, ground_wpn, sea_wpn, gold, food, emptyPos);

			bandit_unit.energy = 部队气力[bandit_person_index][bandit_force_index];

			return bandit_unit;
		}

		int func_敌军部队_最佳任务_决策(pk::city@ city, pk::unit@ bandit_unit)
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
					and terrain_id != 地形_山 and terrain_id != 地形_岸 and terrain_id != 地形_河 and terrain_id != 地形_川 and terrain_id != 地形_海)
					return arr[i];

			}
			return -1;
		}

		//根据异族势力获取异族势力index--排除了盗贼
		int get_bandit_index(pk::force@ force)
		{
			//42羌,43山越,44乌丸,45南蛮,46盗贼
			///*羌武将*/-1, /*乌丸武将*/-1, /*山越武将*/-1, /*南蛮武将*/
			int index = -1;
			int force_id_t = force.get_id();
			if (force_id_t == 42)
				index = 1;
			if (force_id_t == 43)
				index = 3;
			if (force_id_t == 44)
				index = 2;
			if (force_id_t == 45)
				index = 4;
			if (force_id_t == 46)
				index = 0;
			return index;
		}

		pk::city@ func_获取_玩家_可进攻城市(pk::force @player_force)
		{

			pk::list<pk::city@> cityList = pk::get_city_list(player_force);
			pk::list<pk::city@> cityList_f;
			for (int i = 0; i < cityList.count; i++)
			{
				if (请求异族城市[0].find(cityList[i].get_id()) >= 0 or 请求异族城市[1].find(cityList[i].get_id()) >= 0 or 请求异族城市[2].find(cityList[i].get_id()) >= 0 or 请求异族城市[3].find(cityList[i].get_id()) >= 0)
					cityList_f.add(cityList[i]);
			}

			if (cityList_f.count != 0)
			{
				cityList_f.sort(function(a, b)
				{
					if (a.troops != a.troops)
						return a.troops > a.troops;
					pk::list<pk::person@> list_1 = pk::get_person_list(a.get_id(), pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
					pk::list<pk::person@> list_2 = pk::get_person_list(b.get_id(), pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
					if (list_1.count != list_2.count)
						return list_1.count < list_2.count;
					return a.get_id() < b.get_id(); // 先按兵力倒序，再按武将数排序，再按id排序
				});
				//如果符合条件的城市数大于2，取前三随机，如果2个或以内，则直接随机。
				if (cityList_f.count > 2) return cityList_f[pk::rand(3)];
				else return  cityList_f[pk::rand(cityList_f.count)];
			}
			else return null;
		}


		//////////////////////////////////ai请求异族相关////////////////////////////////////////////
		//first 请求城市，second 目标城市
		pk::int_int fun_获取AI_进攻及请求_城市_盗贼(pk::force@ player_force)
		{
			pk::int_int result;
			result.first = -1;
			result.second = -1;
			array<pk::int_int> 目标数组();
			目标数组 = array<pk::int_int>();
			pk::list<pk::city@> List;
			pk::list<pk::city@> List0 = pk::get_city_list(player_force);
			pk::list<pk::city@> List1 = pk::get_city_list();
			//打乱顺序，防止每次都相同的城市
			List1.shuffle();
			int n = 0;
			//按兵力和武将数排序
			List0.sort(function(a, b)
			{
				if (a.troops != a.troops)
					return a.troops < a.troops;
				pk::list<pk::person@> list_1 = pk::get_person_list(a.get_id(), pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
				pk::list<pk::person@> list_2 = pk::get_person_list(b.get_id(), pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
				if (list_1.count != list_2.count)
					return list_1.count < list_2.count;
				return a.get_id() < b.get_id(); // 先按兵力倒序，再按武将数排序，再按id排序
			});

			//获取兵力数量末位的城市，然后满足请求条件的城市取前三个
			for (int i = 0; i < List0.count; i++)
			{
				pk::city @ src = List0[i];
				for (int j = 0; j < List1.count; j++)
				{
					pk::city @ dst = List1[j];
					int src_id = src.get_id();
					int dst_id = dst.get_id();
					//pk::trace(pk::format("ai提出请求城市id：{}, 目标城市id：{}", src.get_id() ,  dst.get_id()));

					if (dst.get_force_id() != src.get_force_id())
					{
						//pk::trace(pk::format("势力不同", src.get_id()));
						//pk::trace(pk::format("城市距离：{}, 是否敌人：{}", pk::get_building_distance(dst_id, src_id, src.get_force_id()) , pk::is_enemy(src, dst)));						
						if (pk::get_building_distance(dst_id, src_id, src.get_force_id()) <= 3 and pk::is_enemy(src, dst))
						{
							目标数组.insertLast(pk::int_int(dst_id, src_id));
							//目标数组[n+1].first = src_id;
							//目标数组[n+1].second = dst_id;
							n = n + 1;
							//pk::trace(pk::format("ai提出请求城市id：{}, 目标城市id：{},符合条件", src_id ,  dst_id));
							if (n > 2)
								break;
						}
					}
				}
			}
			if (目标数组.length > 0)
				result = 目标数组[pk::rand(目标数组.length)];
			return result;
		}

		//请求城市，目标城市，异族势力id
		pk::int_int_int fun_获取AI_进攻及请求_城市_异族(pk::force@ player_force)
		{
			pk::int_int_int result;
			result.first = -1;
			result.second = -1;
			result.third = -1;
			int player_force_id = player_force.get_id();
			array<pk::int_int_int> 目标数组();
			目标数组 = array<pk::int_int_int>();
			pk::list<pk::city@> List;
			pk::list<pk::city@> List0 = pk::get_city_list(player_force);

			pk::list<pk::city@> List1;
			List1.clear();
			int n = 0;
			//按兵力和武将数排序
			List0.sort(function(a, b)
			{
				if (a.troops != a.troops)
					return a.troops < a.troops;
				pk::list<pk::person@> list_1 = pk::get_person_list(a.get_id(), pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
				pk::list<pk::person@> list_2 = pk::get_person_list(b.get_id(), pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
				if (list_1.count != list_2.count)
					return list_1.count < list_2.count;
				return a.get_id() < b.get_id(); // 先按兵力倒序，再按武将数排序，再按id排序
			});

			int index_id = -1;
			//获取兵力数量末位的城市，然后满足请求条件的城市取前三个
			for (int i = 0; i < List0.count; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					//异族势力 42羌,43山越,44乌丸,45南蛮,46盗贼
					if (异族出现城市[j].find(List0[i].get_id()) >= 0)
					{
						pk::city @ src = List0[i];
						int src_id = src.get_id();
						index_id = j;
						//确认可请求的城市里存在敌方城市
						//bool all_player_city = true;
						for (int s = 0; s < int(请求异族城市[j].length); s++)
						{

							pk::city @ dst = pk::get_city(请求异族城市[j][s]);
							if (dst.get_force_id() != player_force_id and pk::is_enemy(src, dst))
							{
								//all_player_city = false;
								List1.add(dst);
							}
						}


						if (List1.count != 0)
						{
							//打乱顺序，防止每次都相同的城市
							List1.shuffle();
							pk::city @ dst = List1[0];
							List1.clear();
							int dst_id = dst.get_id();
							int yizu_force_id = 异族势力[index_id + 1];
							目标数组.insertLast(pk::int_int_int(dst_id, src_id, yizu_force_id));
							n = n + 1;
							//pk::trace(pk::format("ai提出请求城市id：{}, 目标城市id：{},异族势力：{}，符合条件", dst_id, src_id, yizu_force_id));
							if (n > 2)
								break;
						}
					}
				}
				if (n > 2) break;
			}
			if (目标数组.length > 0)
				result = 目标数组[pk::rand(目标数组.length)];
			return result;
		}

		void AI_异族入侵_执行(int src_building_id, int aim_city_id, int yizu_force_id, int type = 0)//加入type，以保证协同进攻时用不同的表述---考虑到attack_alert，将src_city_id改成src_building_id
		{

			pk::city @ aim_city = pk::get_city(aim_city_id);
			pk::building @ aim_building = pk::get_building(aim_city_id);
			pk::force @ aim_force = pk::get_force(aim_city.get_force_id());
			pk::building@ src_building = pk::get_building(src_building_id);
			//pk::city @ src_city = pk::get_city(src_building_id);


			pk::force @ src_force = pk::get_force(src_building.get_force_id());
			pk::force @ yizu_force_t = pk::get_force(yizu_force_id);
			if (src_force.get_id() == aim_force.get_id()) return;//防止自己打自己
			int bandit_force_index = get_bandit_index(yizu_force_t);
			int bandit_facility_id = 敌军设施[bandit_force_index];
			int bandit_force_id = yizu_force_id;

			int controller_index = get_controller_index_of_force(aim_force);	//玩家or电脑

			pk::building@ new_bandit_base = func_基本事件1_贼将势力根据地_生成(aim_force, aim_city, aim_building, bandit_force_id, bandit_facility_id, bandit_force_index);
			if (new_bandit_base !is null)
			{
				func_基本事件_画面移动(aim_force, new_bandit_base.pos);
				//pk::trace(pk::format("执行进攻，目标势力id：{}, 名：{}", aim_force.get_id(), aim_force_name));
				bool success = func_基本事件_贼将势力部队_生成_请求版(aim_city, aim_building, new_bandit_base, bandit_force_id, controller_index, bandit_force_index, /*bandit_person_index*/0, src_force.get_id());
				func_基本事件_贼将势力部队_生成_请求版(aim_city, aim_building, new_bandit_base, bandit_force_id, controller_index, bandit_force_index, /*bandit_person_index*/1, src_force.get_id());
				if (success)
				{
					if (type == 0) pk::history_log(aim_city.pos, src_force.color, pk::encode(pk::format("\x1b[29x{}军\x1b[0x因与\x1b[1x{}军\x1b[0x的协定，开始向\x1b[2x{}\x1b[0x进攻", pk::decode(pk::get_name(yizu_force_t)), pk::decode(pk::get_name(src_force)), pk::decode(pk::get_name(aim_city)))));
					else if (type == 1) pk::history_log(aim_city.pos, src_force.color, pk::encode(pk::format("\x1b[29x{}军\x1b[0x因受\x1b[1x{}军\x1b[0x的委托，开始协同进攻\x1b[2x{}\x1b[0x", pk::decode(pk::get_name(yizu_force_t)), pk::decode(pk::get_name(src_force)), pk::decode(pk::get_name(aim_city)))));

				}
				if (current_period <= 1) pk::kill(new_bandit_base);

				if (current_period >= 3)
				{
					pk::building@ new_bandit_base2 = func_基本事件1_贼将势力根据地_生成(aim_force, aim_city, aim_building, bandit_force_id, bandit_facility_id, bandit_force_index);
					func_基本事件_画面移动(aim_force, new_bandit_base2.pos);
					//pk::trace(pk::format("执行进攻，目标势力id：{}, 名：{}", aim_force.get_id(), aim_force_name));
					func_基本事件_贼将势力部队_生成_请求版(aim_city, aim_building, new_bandit_base2, bandit_force_id, controller_index, bandit_force_index, /*bandit_person_index*/0, src_force.get_id());
					func_基本事件_贼将势力部队_生成_请求版(aim_city, aim_building, new_bandit_base2, bandit_force_id, controller_index, bandit_force_index, /*bandit_person_index*/1, src_force.get_id());
				}

				if (current_period == 4)
				{
					pk::building@ new_bandit_base2 = func_基本事件1_贼将势力根据地_生成(aim_force, aim_city, aim_building, bandit_force_id, bandit_facility_id, bandit_force_index);
					func_基本事件_画面移动(aim_force, new_bandit_base2.pos);
					//pk::trace(pk::format("执行进攻，目标势力id：{}, 名：{}", aim_force.get_id(), aim_force_name));
					func_基本事件_贼将势力部队_生成_请求版(aim_city, aim_building, new_bandit_base2, bandit_force_id, controller_index, bandit_force_index, /*bandit_person_index*/0, src_force.get_id());
					func_基本事件_贼将势力部队_生成_请求版(aim_city, aim_building, new_bandit_base2, bandit_force_id, controller_index, bandit_force_index, /*bandit_person_index*/1, src_force.get_id());
				}
				//军费计算
				if (src_building_id > -1)
				{
					if (src_building_id < 建筑_城市末)
					{
						pk::city @ src_city = pk::get_city(src_building_id);
						if (src_city.gold > 异族军费_AI[bandit_force_index][current_period])
							src_city.gold -= 异族军费_AI[bandit_force_index][current_period];
						else src_city.gold = 100;
					}
					else if (src_building_id < 建筑_关卡末)
					{
						pk::gate @ src_gate = pk::get_gate(src_building_id - 建筑_城市末);
						if (src_gate.gold > 异族军费_AI[bandit_force_index][current_period])
							src_gate.gold -= 异族军费_AI[bandit_force_index][current_period];
						else src_gate.gold = 100;
					}
					else if (src_building_id < 建筑_港口末)
					{
						pk::port @ src_port = pk::get_port(src_building_id - 建筑_关卡末);
<<<<<<< HEAD
						pk::trace("bandit_force_index" + bandit_force_index + "current_period" + current_period + "src_building_id" + src_building_id);
=======
						if (调试模式) pk::trace("bandit_force_index" + bandit_force_index + "current_period" + current_period + "src_building_id" + src_building_id);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
						if (src_port.gold > 异族军费_AI[bandit_force_index][current_period])
							src_port.gold -= 异族军费_AI[bandit_force_index][current_period];
						else src_port.gold = 100;
					}
				}
			}

		}

		bool is_enemy_city_near(pk::city@ src, int distance)
		{
			for (int i = 0; i < 城市_末; i++)
			{
				pk::city @ dst = pk::get_city(i);
				if (dst.get_force_id() != src.get_force_id())
				{
					if (pk::get_distance(src.get_pos(), dst.get_pos()) < distance)
						return true;
				}
			}
			return false;
		}


		string get_siqin_call(int person_id)
		{
			string person_name;
			pk::person@ person0 = pk::get_person(person_id);
			string person_name0;// = (person0.azana is null)? ((person0.sex == 0)? (pk::decode(person0.mei)+"弟"):(pk::decode(person0.mei)+"妹")):(person0.sex == 0)? (pk::decode(person0.azana)+"弟"):(pk::decode(person0.azana)+"妹"));
			if (person0.azana == "")
			{
				if (person0.sex == 0) person_name0 = pk::decode(person0.mei) + "弟";
				else  person_name0 = pk::decode(person0.mei) + "妹";
			}
			else {
				if (person0.sex == 0) person_name0 = pk::decode(person0.azana);
				else  person_name0 = pk::decode(person0.azana);
			}
			switch (person_id)
			{
			case 武将_木鹿大王: person_name = "木鹿大王"; break;
			case 武将_带来洞主: person_name = "带来洞主"; break;
			case 武将_朶思大王: person_name = "朶思大王"; break;
			case 武将_金环三结: person_name = "金环三结"; break;
			case 武将_兀突骨: person_name = "兀突骨"; break;
			case 武将_忙牙长: person_name = "忙牙长"; break;
			case 武将_阿会喃: person_name = "阿会喃"; break;
			case 武将_董荼那: person_name = "董荼那"; break;
			default:  person_name = person_name0; break;
			}
			person_name = "\x1b[2x" + person_name + "\x1b[0x";
			return person_name;
		}

		void check_敌将()
		{
			for (int person_id = 敌将_开始; person_id < 敌将_末; person_id++)
			{
				pk::person@ person = pk::get_person(person_id);
				if (pk::is_alive(person))
				{
					if (person.mibun != 身份_死亡 && person.district < 非贼势力_末) pk::kill(person, null, null, null, 1);   //杀死 还活着但归属非异族军团的
					if (person.mibun == 身份_在野) pk::kill(person, null, null, null, 1);
				}
			}
		}

	}

	Main main;
}
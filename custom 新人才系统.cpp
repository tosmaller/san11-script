/***CUSTOM
@name 新人才系统
@class 人才指令
@author 氕氘氚
@contact 
@version 1.5.6
@date 2021/10/29
@desc *结构体存储调用方式改进。
新版人才系统：
1.选拔（by氕氘氚）
2.AI动态选拔（by氕氘氚）
3.随机人才出现（by氕氘氚）
4.新人才列传生成（by江东新风、白马叔叔）
5.生子系统（by白马叔叔）
@notice 更新时一定要重启！！！
***/

/*
1.5.6   结构体存储调用方式改进
1.5.5   修复单港关存活时的bug
1.5.4   为msg添加结构体参数，并更改func50
1.5.3	修复提拔或生子后太守或父亲能力异常升高的bug
1.5.2	新武将列传一律清空
1.5.1	修复孩子出生年龄和登场时间异常的bug
*/

//## 2022/02/18 # 江东新风 # 人才选拔概率出现历史武将 ##
//## 2022/01/06 # 江东新风 # 头像范围重新设定 ##
//## 2021/12/18 # 江东新风 # 人才选拔展示窗口，信息存储改进 ##
//## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
//## 2021/10/28 # 江东新风 # 修复单港关存活时的bug ##
//## 2021/10/10 # 江东新风 # warning修复 ##
//## 2021/09/26 # 江东新风 # 为msg添加结构体参数，并更改func50 ##
//## 2021/05/07 # 氕氘氚 # 重新添加马神的生子系统 ##
//## 2021/4/27 # 氕氘氚 # 适当降低AI提拔和自动出现的人才数量和强度 ##
//## 2021/4/25 # 氕氘氚 # 修复翘楚概率 ##
//## 2021/4/24 # 氕氘氚 # 修复身份错误，调整费用 ##
//## 2021/4/23 # 氕氘氚 # 暂时删除get_kanji改为get_family_name ##
//## 2021/4/21 # 江东新风 # 修复翘楚bug ##
//## 2021/3/09 # 氕氘氚 # 整合新人才系统 ##
//@@ 2021/3/01 @ 氕氘氚 @ 整合新人才系统 @@
//## 2021/3/05 # 江东新风 # 为人才选拔加入列传信息记录 ##
//## 2021/2/10 # 白马叔叔 # 按文官武官分开生成头像，造型随机 ##
//## 2021/1/16 # 白马叔叔 # 生子事件发生类型可选，细节微调，风神帮忙修复空指针 ##
//## 2021/1/12 # 白马叔叔  # 修复特殊剧本子武将不登场bug，排除有子父母ID，初始化父母、世代、配偶、伤病等，预留子武将直接登场功能 ##
//## 2021/1/3 # 白马叔叔  # 增加新武将出仕事件，新增女名备选库，女名修改为仅备选库，增加武将生子事件，优化算法 ##
//## 2020/12/17 # 氕氘氚 # 修复65535金无法使用的bug ##
//## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/12/07 # 白马叔叔 # 增加备选姓、男名、女名数组，姓只取数组里的值 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
//## 2020/09/02 # 氕氘氚 #  修复出现能力?120的bug##
//## 2020/08/06 # 氕氘氚 # 优化选拔规则、特技匹配 ##
//## 2020/08/05 # 氕氘氚 # 移除港g提拔，修正选拔台詞 ##
//## 2020/08/04 # 氕氘氚 # 人才提拔系统革新 ##
//## 2020/07/30 #messi##



namespace 人才系统
{
    // 全局常量 --------------------------------------------------
    // const bool/int/float 全局常量 = XXX;

	const int TP_COST = 500;//500
	const int ACTION_COST = 50;//50
	const int GOLD_COST = 3000;//3000

	const bool 启用_AI选拔 = true;
	const int 时期_AI选拔 = 2;
	const bool 启用_人才出世 = false;
	const int 时期_人才出世 = 2;
	const bool 启用_武将生子 = true;
	const int 时期_武将生子 = 2;/*2*/
	const int 武将生子概率 = 50;

	const array<int> 人才强度_good = 	{0,		5,		10,		10,		20,		35};
	const array<int> 人才强度_bad = 	{50,	40,		25,		10,		10,		5};
	const array<int> 人才强度_exlt = 	{0,		1,		3,		5,		8,		10};

	const int AI选拔强度 = 3;  // 0-4(极弱、弱、中、强、极强、超级强)

	const int 人才出世强度_200 = 0;  // 0-4(极弱、弱、中、强、极强)
	const int 人才出世强度_300 = 0;  // 0-4(极弱、弱、中、强、极强)
	const int 人才出世强度_400 = 1;  // 0-4(极弱、弱、中、强、极强)
	const int 人才出世强度_500 = 1;  // 0-4(极弱、弱、中、强、极强)
	const int 人才出世强度_600 = 2;  // 0-4(极弱、弱、中、强、极强)
	const int 人才出世强度_700 = 2;  // 0-4(极弱、弱、中、强、极强)

	const int 人才出世数量_200 = 6;
	const int 人才出世数量_300 = 6;
	const int 人才出世数量_400 = 5;
	const int 人才出世数量_500 = 4;
	const int 人才出世数量_600 = 3;
	const int 人才出世数量_700 = 3;

	const array<int> 人才出世_count = 	{200,	300,	400,	500,	600,	700};
	// const array<int> 人才出世_number = 	{6,		6,		5,		4,		3,		3};
	const array<int> 人才出世_number = 	{人才出世数量_200, 人才出世数量_300, 人才出世数量_400, 人才出世数量_500, 人才出世数量_600, 人才出世数量_700};
	const array<int> 人才出世_chance = 	{85,	75,		75,		75,		75,		75};
	// const array<int> 人才出世_good = 	{0,		5,		10,		15,		15,		20};
	// const array<int> 人才出世_bad = 	{50,	50,		45,		45,		30,		30};
	// const array<int> 人才出世_exlt = 	{1,		1,		2,		2,		3,		3};
	const array<int> 人才出世_good = 	{人才强度_good[人才出世强度_200], 人才强度_good[人才出世强度_300], 人才强度_good[人才出世强度_400],
										人才强度_good[人才出世强度_500], 人才强度_good[人才出世强度_600], 人才强度_good[人才出世强度_700] };
	const array<int> 人才出世_bad = 	{人才强度_bad[人才出世强度_200], 人才强度_bad[人才出世强度_300], 人才强度_bad[人才出世强度_400],
										人才强度_bad[人才出世强度_500], 人才强度_bad[人才出世强度_600], 人才强度_bad[人才出世强度_700] };
	const array<int> 人才出世_exlt = 	{人才强度_exlt[人才出世强度_200], 人才强度_exlt[人才出世强度_300], 人才强度_exlt[人才出世强度_400],
										人才强度_exlt[人才出世强度_500], 人才强度_exlt[人才出世强度_600], 人才强度_exlt[人才出世强度_700] };

	const bool 小城可选拔 = false;
	const bool 类型同提拔者 = true;
	const int 女性占比 = 50;
	const int 选拔成功率 = 80;

	/*const int 提拔人才白板概率 = 10;
	const int AI人才白板概率 = 20;
	const int 自动出世白板概率 = 50;
	const int 提拔人才能力倍率 = 100;
	const int AI人才能力倍率 = 85;
	const int 自动出世人才能力倍率 = 75;*/
	const int 人才适性标准 = 4;
	const int 新人才能力倍率 = 110;
	const int 特技白板概率 = 20;
	const int 人才能力上限 = 200;
	const int 人才适性上限 = 3;
	const int 基础欧皇率 = 100;
	const int 基础非酋率 = 10;

    //  可用武将槽
    const bool slot_新武将 = true;		//999-850
    const bool slot_古武将 = true;		//849-800
    const bool slot_史实武将 = true;	//699-0
    //const bool slot_事件武将下 = true;  //799-763
    //const bool slot_事件武将上 = false;  //762-700


    const bool use_designated_name = true;

    //  姓
	const array<string> sei_ = {    "赵", "公孙", "孙", "李", "周", "吴", "郑", "王", "冯", "陈",
							        "司马", "袁", "夏侯", "诸葛", "韩", "杨", "朱", "秦", "许", "何",
							        "吕", "张", "孔", "曹", "严", "华", "金", "魏", "陶", "姜",
                                    "鲁", "马", "刘", "唐", "宋", "徐", "霍", "丁", "陆", "于",
                                    "郭", "关", "姜", "太史", "淳于", "高堂", "皇甫", "蒋义", "成公", "黄",
                                    "高", "胡", "蔡", "荀", "钟", "全", "薛", "辛", "苏", "程",
                                    "田", "董", "邓", "文", "孟", "梁", "范", "白", "萧", "罗"};

	//  男-名
	const array<string> mei_m = {   "海", "嘉", "宁", "超", "统", "真", "建", "允", "休", "智",
									"丰", "艾", "平", "文", "楚", "通", "飞", "康", "良", "明",
									"尚", "青", "德", "进", "岱", "正", "亮", "忠", "仪", "广",
									"云", "延", "威", "昭", "和", "齐", "雄", "信", "义", "志",
									"维", "逊", "柔", "顺", "渊", "定", "济", "攸", "肃", "胜",
									"兰", "睿", "度", "修", "浩", "芝", "成", "朗", "英", "兴",
									"远", "常", "长", "台", "约", "衡", "龙", "让", "伟", "春",
									"夏", "秋", "冬", "祖", "桓", "符", "谋", "昂", "舒", "山",
									"先", "翼", "起", "升", "霸", "孝", "直", "绩", "谦", "才",
									"俊", "方", "彦", "烈", "承", "达", "宣", "高", "至", "业" };

	//  女-名
	const array<string> mei_f = {   "媛", "蕊", "嫣然", "姬", "兰香", "圆圆", "芙蓉", "婉君", "玉华", "雪",
									"香香", "飞燕", "玉环", "若兰", "小小", "灵儿", "丽华", "婕妤", "诗诗", "琬",
                                    "梅", "杏儿", "春华", "秋月", "芳", "玉英", "玉兰", "惠", "佩", "文君",
                                    "妙", "凤", "巧巧", "珠", "雨荷", "如月", "玉珠", "燕燕", "玉", "莺",
                                    "雅", "芝", "慧", "兰", "静", "淑", "翠", "芬", "春", "菊",
                                    "晶", "青", "莹", "珍", "黛", "琳", "彩", "清", "蓉", "卿卿",
                                    "媚", "蝉", "娴", "宓", "娜", "玉奴", "娇", "娉", "灵美", "雨",
                                    "宝珠", "盈盈", "蝶凤", "玉娥", "昭", "琰", "华", "氏", "玲", "琪",
                                    "紫琼", "瑶", "琴", "璃", "瑜", "莲", "莎", "姒", "芸", "美"};

	// 字号的第一个字
	const array<string> azana_0 = { "伯", "仲", "叔", "季", "幼", "早", "孟", "显", "惠", "雅",
									"陆", "德", "仁", "少", "公", "子", "弘", "平", "文", "长",
									"奉", "士", "元", "玄", "守", "兴", "正", "曼", "义", "汉"};


	array<array<int>> 统军技 = {{77, 156, 157}, {1, 2, 27, 28, 29, 72, 73, 77, 113, 114, 126, 128, 156, 157}, {1, 2, 6, 27, 28, 29, 31, 72, 73, 74, 77, 100, 103, 113, 114, 116, 117, 122, 125, 126, 127, 128, 130, 131, 145, 148, 149, 156, 157}, {0, 1, 2, 6, 27, 28, 29, 31, 48, 72, 73, 74, 77, 100, 103, 113, 114, 116, 117, 120, 121, 122, 125, 126, 127, 128, 130, 131, 145, 148, 149, 156, 157}};
	array<array<int>> 破军技 = {{9, 15, 21, 23, 30, 156, 157}, {9, 10, 11, 13, 15, 20, 21, 23, 30, 51, 108, 109, 110, 118, 133, 156, 157, 158, 159}, {9, 10, 11, 12, 13, 15, 19, 20, 21, 23, 30, 51, 108, 109, 110, 118, 122, 124, 127, 133, 134, 135, 145, 148, 149, 156, 157, 158, 159}, {0, 9, 10, 11, 12, 13, 15, 19, 20, 21, 23, 30, 48, 51, 108, 109, 110, 118, 122, 123, 124, 127, 133, 134, 135, 145, 148, 149, 156, 157, 158, 159}};
	array<array<int>> 智略技 = {{16}, {16, 22, 54, 55, 56, 58, 59, 67, 72, 73, 86, 104, 105, 106}, {16, 22, 53, 54, 55, 56, 57, 58, 59, 60, 64, 65, 66, 67, 68, 70, 72, 73, 74, 86, 100, 104, 105, 106, 117, 132, 143, 144, 148, 149}, {16, 22, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 86, 100, 104, 105, 106, 117, 121, 132, 143, 144, 148, 149}};
	array<array<int>> 内政技 = {{77, 82, 83, 85, 97, 98, 101, 138}, {7, 77, 79, 80, 81, 82, 83, 84, 85, 86, 97, 98, 101, 102, 107, 111, 112, 115, 119, 136, 137, 138, 140, 142, 146, 147}, {7, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 96, 97, 98, 101, 102, 103, 107, 111, 112, 115, 119, 136, 137, 138, 139, 140, 141, 142, 146, 147}, {7, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 96, 97, 98, 101, 102, 103, 107, 111, 112, 115, 119, 136, 137, 138, 139, 140, 141, 142, 146, 147}};
	array<array<int>> 辅助技 = {{18, 26, 76, 99, 101}, {7, 8, 18, 26, 33, 52, 75, 76, 99, 101, 107, 142}, {7, 8, 18, 26, 33, 52, 70, 75, 76, 91, 92, 93, 94, 95, 99, 101, 107, 131, 142}, {7, 8, 18, 26, 32, 33, 52, 70, 71, 75, 76, 91, 92, 93, 94, 95, 99, 101, 107, 131, 142}};

	array<array<int>> 枪兵技 = {{34}, {13, 34, 150}, {13, 34, 39, 40, 41, 150}, {13, 34, 39, 40, 41, 42, 150}};
	array<array<int>> 戟兵技 = {{35}, {13, 35}, {13, 35, 39, 40, 41, 151}, {13, 35, 39, 40, 41, 43, 151}};
	array<array<int>> 弩兵技 = {{36}, {36, 50, 129, 152}, {36, 39, 40, 50, 129, 152}, {36, 39, 40, 44, 50, 129, 152}};
	array<array<int>> 骑兵技 = {{3, 37}, {3, 13, 25, 37, 153}, {3, 13, 25, 37, 39, 40, 49, 153}, {3, 13, 25, 37, 39, 40, 45, 49, 153}};
	array<array<int>> 工兵技 = {{24, 154}, {17, 24, 154}, {17, 24, 39, 40, 154}, {17, 24, 39, 40, 46, 154}};
	array<array<int>> 水兵技 = {{38}, {4, 5, 14, 38, 155}, {4, 5, 14, 38, 39, 40, 155}, {4, 5, 14, 38, 39, 40, 47, 155}};

	const bool 调试模式 = false;
	pk::random randint(pk::rand());

	pk::array<bool> flag_has_child(1000, false);    //  flag_has_child[i] == true 说明i号武将有孩子
	int previois_new = 999;
	string title;


    // --------------------------------------------------
    class Main
    {
        // 全局变量 --------------------------------------------------

		pk::building@ building_;
		pk::person@ kunshu_;
		pk::person@ taishu_;
		pk::force@ force_;
		int building_gold;
		int building_food;
		int menu_city_id_;
		pk::func50_t@ prev_callback_50;
        // --------------------------------------------------

        Main()
        {
            // 脚本执行入口

            //	玩家菜单选拔
			menu_select();

            //	根据每个AI势力拥有武将数/城市数大小，自动选拔武将
			if (启用_AI选拔){
				switch (时期_AI选拔){
					case 0:	pk::bind(107, pk::trigger107_t(AI_select));	break;
					case 1:	pk::bind(108, pk::trigger108_t(AI_select));	break;
					case 2:	pk::bind(109, pk::trigger109_t(AI_select));	break;
					case 3:	pk::bind(110, pk::trigger110_t(AI_select));	break;
					default:	break;
				}
			}

            //	根据总体武将数量多少，自动产生一定数量的在野人才
			if (启用_人才出世){
				switch (时期_人才出世){
					case 0:	pk::bind(107, pk::trigger107_t(auto_appear));	break;
					case 1:	pk::bind(108, pk::trigger108_t(auto_appear));	break;
					case 2:	pk::bind(109, pk::trigger109_t(auto_appear));	break;
					case 3:	pk::bind(110, pk::trigger110_t(auto_appear));	break;
					default:	break;
				}
			}

            //	生子系统
			if (启用_武将生子){
				switch (时期_武将生子){
					case 0:	pk::bind(107, pk::trigger107_t(child_birth));	break;
					case 1:	pk::bind(108, pk::trigger108_t(child_birth));	break;
					case 2:	pk::bind(109, pk::trigger109_t(child_birth));	break;
					case 3:	pk::bind(110, pk::trigger110_t(child_birth));	break;
					default:	break;
				}
				pk::bind(110, pk::trigger110_t(child_appear));
			}
			@prev_callback_50 = cast<pk::func50_t@>(pk::get_func(50));
			pk::reset_func(50);
			pk::set_func(50, pk::func50_t(callback_50));
			//pk::set_func(50, pk::func50_t(callback_50));

        }

        // 其他函数
		void callback_50(int person_id, string& str)
		{
			if (prev_callback_50 !is null)
			{
				prev_callback_50(person_id, str);
				return;
			}
			else
			{
				if (is_new_face(person_id))
				{
					str = "";
				}
				return;
			}
		}

		bool is_new_face(int person_id)
		{
			int face_id = pk::get_person(person_id).face;
			if (face_id >= 1200 and face_id <= 1273) return true;
			else if (face_id >= 1300 and face_id <= 1385) return true;
			else if (face_id >= 1400 and face_id <= 1432) return true;
			else if (face_id >= 1600 and face_id <= 1632) return true;
			else return false;
		}


		// 菜单选拔 ==============================================================================================
		void menu_select()
		{
			pk::menu_item item;
			item.menu = 102;
			item.pos = 6;
			item.shortcut = global_menu::shortcut_选拔;
			item.init = pk::building_menu_item_init_t(menu_init);
			item.is_visible = pk::menu_item_is_visible_t(menu_isVisible);
			item.is_enabled = pk::menu_item_is_enabled_t(menu_isEnabled);
			item.get_text = pk::menu_item_get_text_t(menu_getText);
			item.get_desc = pk::menu_item_get_desc_t(menu_getDesc);
			item.handler = pk::menu_item_handler_t(menu_handler);
			pk::add_menu_item(item);
		}

		void menu_init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@kunshu_ = pk::get_person(force_.kunshu);
			building_gold = pk::get_gold(building_);

			if (building_ !is null) @taishu_ = pk::get_person(pk::get_taishu_id(building_));
			else @taishu_ = null;
		}

		//	关闭小城选拔
		bool menu_isVisible()
		{
			pk::city@ city = pk::building_to_city(building_);
            if (not 小城可选拔 and city is null) return false;
			else return true;
		}

		bool menu_isEnabled()
		{
			if (building_.get_id() >= 城市_末) return false;
            if (building_.facility > 시설_도시) return false;
			if (taishu_ is null) return false;
			if (building_gold < GOLD_COST) return false;
            else if (force_.tp < TP_COST) return false;
            else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		string menu_getText()
		{
			return pk::encode("人才选拔");
		}

		string menu_getDesc()
		{
			if (taishu_ is null)
				return pk::encode("太守不在城市中.");
			else if (building_gold < GOLD_COST)
                return pk::encode(pk::format("资金不足.(需要资金 {} )", GOLD_COST));
			else if (force_.tp < TP_COST)
                return pk::encode(pk::format("技巧点不足.(技巧点 {})", TP_COST));
            else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
                return pk::encode(pk::format("行动力不足.(需要行动力 {} )", ACTION_COST));
			else
				return pk::encode(pk::format("进行武人才选拔.(使用技巧点 {} )", TP_COST));
		}

		bool menu_handler()
		{
			int reply = pk::choose(pk::encode("进行人才选拔"), { 	 pk::encode("一般选拔"),
																	pk::encode("严格选拔"),
																	pk::encode(" 取消 "),
																	pk::encode(" <帮助> ") });
			if (reply == 2)	return false;
			else if (reply == 3){
				pk::message_box(pk::encode("人才选拔将随机在城中搜索假想的武将，根据选拔者能力及运气可能选拔到能力高低、特技强度不同的人才."));
				pk::message_box(pk::encode("一般选拔：低失败率、人才属性居中.\n严格选拔：中等失败率、更容易获得高属性武将."));
				pk::message_box(pk::encode("选拔失败时返还1/3技巧点且不消耗金钱."));
				return false;
			}

			string kunshu_name = pk::decode(pk::get_name(kunshu_));
			string taishu_name = pk::decode(pk::get_name(taishu_));

			pk::person@ new_person = getEmptyPerson();
			int 成功率 = 选拔成功率 + (reply == 1 ? -20 : 0) + taishu_.stat[4] / 10 - 8;
			int 欧皇率 = 基础欧皇率 + (reply == 1 ? 基础欧皇率 : 0);
			int 非酋率 = reply == 1 ? 0 : (基础非酋率 - taishu_.stat[4] / 10 + 8);
			int 翘楚率 = reply == 1 ? 7 : 4;

			if (ch::has_skill(taishu_, 특기_안력))
			{
				成功率 += 10;
				欧皇率 += 2;
				非酋率 -= 5;
				翘楚率 += 1;
			}

			成功率 = ch::inRange(成功率, 0, 100);
			欧皇率 = ch::inRange(欧皇率, 0, 50);
			非酋率 = ch::inRange(非酋率, 0, 50);

			if (!pk::rand_bool(成功率) or new_person is null)
			{
				if (reply == 0){
					switch (randint(0, 1))
					{
					case 0:
						pk::message_box(pk::encode("传闻在这个地区有位有名的人才.但是已经去了别的地方."), taishu_); break;
					case 1:
						pk::message_box(pk::encode("已经寻遍了街坊，但没有找到令人满意的人才."), taishu_); break;
					}
				}else{
					pk::message_box(pk::encode("也许是要求太高了，实在无法挑选出足够优秀的人才."), taishu_);
				}

				pk::message_box(pk::encode("人才选拔失败."));
				pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.", TP_COST / 3 * 2)));
				ch::add_tp(force_, -TP_COST / 3 * 2, force_.get_pos());

				auto district = pk::get_district(pk::get_district_id(force_, 1));
				pk::add_ap(district, -ACTION_COST);
			}
			else
			{
				int sex = (pk::rand_bool(女性占比)) ? 1 : 0;
				int luck = initNewPerson(new_person, sex, taishu_, 欧皇率, 非酋率, 翘楚率);		
				init_belong(new_person, building_, false, 身份_在野, 0, 0);
				bool result0 = false;
				if (pk::rand_bool(0)) result0 = init_history_new_person(new_person);//覆盖，产生历史武将
				string new_person_name = pk::decode(pk::get_name(new_person));
				string str_0, str_1;
				if 		(luck <= 1)	str_0 = "平庸的";
				else if (luck <= 2)	str_0 = "";
				else if (luck <= 5)	str_0 = "出色的";
				else				str_0 = "\x1b[18x难得一见\x1b[0x的";
				if	(luck >= 8)	str_1 = "翘楚";
				else 					str_1 = "人才";

				if (ch::has_skill(taishu_, 특기_안력))
					pk::message_box(pk::encode("有我在, 一定能找到出色的人才!"), taishu_);
				if (result0) str_0 = "隐居的";
				pk::message_box(pk::encode(pk::format("在这个地区选拔出了一名{}{} \x1b[1x{}\x1b[0x.", str_0, str_1, new_person_name)), taishu_);
				
				bool result_find = pk::call_find_window(new_person);
				//pk::research_ability_cmd_info cmd_draft;
				//bool result_find = pk::call_research_abillity_window(cmd_draft, building_);
				if (result_find)
				{
					init_belong(new_person, building_, true, 身份_一般, 100, 1000);			
					pk::message_box(pk::encode(pk::format("你好.见到\x1b[1x{}\x1b[0x阁下很高兴.", taishu_name)), new_person);
					pk::message_box(pk::encode("我早就听说了有关阁下的传闻.能否助我军一臂之力?"), taishu_);
					pk::message_box(pk::encode(pk::format("好的,既然\x1b[1x{}\x1b[0x大人了解我的能力,我愿为此献身.", taishu_name)), new_person);
					pk::message_box(pk::encode("来,这是我军的一点心意."), taishu_);
					pk::message_box(pk::encode("竟然给我这么多钱,万分感谢."), new_person);
					pk::message_box(pk::encode("谢谢你能加入我军,期待阁下今后的表现."), taishu_);

					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x加入旗下了.", new_person_name)));

				}
				else
				{
					init_belong(new_person, building_, false, 身份_在野, 0, 0);
				}

				string building_name = pk::decode(pk::get_name(building_));
				pk::history_log(force_.get_pos(), force_.color, pk::encode(pk::format("在\x1b[1x{}\x1b[0x地区选拔出了人才 \x1b[1x{}\x1b[0x", building_name, new_person_name)));

				pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.\n资金减少了\x1b[1x{}\x1b[0x.", TP_COST, GOLD_COST)));
				ch::add_tp(force_, -TP_COST, force_.get_pos());
				pk::add_gold(building_, -GOLD_COST, true);

				auto district = pk::get_district(pk::get_district_id(force_, 1));
				pk::add_ap(district, -ACTION_COST);
				pk::sort(pk::person::type_id);
			}
			return true;
		}

		// AI选拔 ====================================================================================================
		void AI_select()
		{
            auto force_list = pk::list_to_array(pk::get_force_list());
			pk::city@ city_t;
			pk::building@ building_t;
			pk::person@ taishu_t;
			float person_per_city;
			int city_count, person_count;
			int chance = 0, index = -1, counter = 0;;

            for (int f = 0; f < int(force_list.length); f++)
            {
				pk::force@ force_t = force_list[f];
				if (force_t.is_player())	continue;

				auto person_list = pk::list_to_array(pk::get_person_list(force_t, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)));
				city_count = pk::get_city_count(force_t);
				if (city_count == 0) city_count = 1;
				person_count = person_list.length;
				person_per_city = person_count / float(city_count);
				if 		(person_per_city <= 2.0f)		chance = 90;
				else if (person_per_city <= 3.0f)		chance = 70;
				else if (person_per_city <= 4.5f)		chance = 40;
				else if (person_per_city <= 6.0f)		chance = 20;
				else if (person_per_city <= 7.5f)		chance = 10;
				else									chance = 0;

				if (pk::rand_bool(chance)){
					auto city_list = pk::list_to_array(pk::get_city_list(force_t));

					index = pk::rand(city_list.length);
					@city_t = city_list[index];
					@building_t = pk::city_to_building(city_t);
					@taishu_t = pk::get_person(pk::get_taishu_id(building_t));

					pk::person@ new_person = getEmptyPerson();
					if (new_person !is null){
						int sex = (pk::rand_bool(女性占比)) ? 1 : 0;
						initNewPerson(new_person, sex, taishu_t, 10, 10, 5);
						init_belong(new_person, building_t, true, 身份_一般, 100, 1000);
					}
				}
			}
		}

		void auto_appear()
		{
			pk::person@ person;
			pk::city@ city_t;
			int count = 0, index, number, chance, good, bad, exlt;

			for (int i = 0; i < 700; i++)
			{
				@person = pk::get_person(i);
				if (person.mibun == 신분_없음 || person.mibun == 신분_사망)
					count++;
			}
			for (int i = 800; i < 1000; i++)
			{
				@person = pk::get_person(i);
				if (person.mibun == 신분_없음 || person.mibun == 신분_사망)
					count++;
			}

			if 		(count <= 人才出世_count[0])	{index = 0;}
			else if (count <= 人才出世_count[1])	{index = 1;}
			else if (count <= 人才出世_count[2])	{index = 2;}
			else if (count <= 人才出世_count[3])	{index = 3;}
			else if (count <= 人才出世_count[4])	{index = 4;}
			else if (count <= 人才出世_count[5])	{index = 5;}
			else									{index = -1;}

			if (index >= 0){
				number = 人才出世_number[index]; chance = 人才出世_chance[index]; good = 人才出世_good[index]; bad = 人才出世_bad[index]; exlt = 人才出世_exlt[index];
			}else{
				number = 0; chance = 0; good = 0; bad = 0; exlt = 0;
			}

			for (int i = 0; i < number; i++)
			{
				if (pk::rand_bool(chance)){
					pk::person@ new_person = getEmptyPerson();
					if (new_person !is null){
						int sex = (pk::rand_bool(女性占比)) ? 1 : 0;
						initNewPerson(new_person, sex, null, good, bad, exlt);
						init_belong(new_person, pk::get_building(pk::rand(42)), false, 신분_미발견, 0, 0);
					}
				}
			}

		}

		// 生子 ====================================================================================================
		void child_birth()
		{
			if (!pk::rand_bool(武将生子概率))
				return;
			pk::person@ new_child = getEmptyPerson();	//获取空白武将
			int sex = (pk::rand_bool(女性占比)) ? 1 : 0;
			pk::person@ new_child_father;
			pk::list<pk::person@> childs_father_list = getFatherList();
			if (new_child !is null && childs_father_list.count > 0)
			{
				int _count = 0;
				do {
					@new_child_father = childs_father_list[pk::rand(childs_father_list.count - 1)];
					_count++;
				} while (_count < 5 && (new_child_father.service < 0));
				if (new_child_father.service >= 0) {
					//武将信息
					initNewPerson(new_child, sex, new_child_father, 10, 10, 5);

					new_child.sei = new_child_father.sei;				//姓氏同父亲
					new_child.father = new_child_father.get_id();
					new_child.appearance = pk::get_year() + 14;         	//登场年
					new_child.birth = pk::get_year();    					//出生年
					new_child.death = new_child.birth + randint(50, 75);  	//死亡年
					new_child.service = new_child_father.service;	// init_belong里的pk::move无效
					new_child.location = new_child_father.service;
					init_belong(new_child, pk::get_building(new_child_father.service), false, 신분_미등장, 0, 0);

					//msg显示
					pk::play_se(10);
					string child_name = pk::decode(pk::get_name(new_child));
					string father_name = pk::decode(pk::get_name(new_child_father));
					int color = pk::get_force(new_child_father.get_force_id()).color;
					pk::history_log(pk::get_building(new_child_father.service).pos, color,
						pk::encode(pk::format("传闻\x1b[1x{}\x1b[0x之{} \x1b[1x{}\x1b[0x 不久前出生了!",
							father_name, sex == 0 ? "子" : "女", child_name)));

					pk::sort(pk::person::type_id);
				}
			}
		}

		void child_appear()
		{
			pk::list<pk::person@> list_s = pk::get_person_list(pk::mibun_flags(身份_未登场));
			bool is_father = false;		// 1-父亲  0-母亲
			for (int i = 0; i < list_s.count; i++)
			{
				pk::person@ person_childs = list_s[i];
				if (person_childs.appearance == pk::get_year())
				{
					string person_childs_name = pk::decode(pk::get_name(person_childs));
					string person_childs_sex = person_childs.sex == 0 ? "子" : "女";

					if (person_childs.father != -1)
						is_father = true;
					else if (person_childs.mother != -1)
						is_father = false;
					else
						continue;

					pk::person@ childs_parent = pk::get_person(is_father ? person_childs.father : person_childs.mother);
					string childs_parent_name = pk::decode(pk::get_name(childs_parent));
					if (childs_parent.mibun == 身份_君主 || childs_parent.mibun == 身份_都督 || childs_parent.mibun == 身份_太守 || childs_parent.mibun == 身份_一般)
					{
						person_childs.mibun = 身份_一般;
						person_childs.loyalty = 100;
						pk::set_district(person_childs, pk::get_building(childs_parent.service).get_district_id());
						pk::move(person_childs, pk::get_building(childs_parent.service));
						person_childs.update();
						if (childs_parent.is_player())
						{
							pk::fade(0);
							pk::background(person_childs.sex == 0 ? 30 : 32);
							pk::fade(255);
							pk::play_se(10);
							if (is_father)
								switch (person_childs.sex == 0 ? randint(0, 6) : randint(1, 6))
								{
									case 0 : pk::message_box(pk::encode("有我加入,就如同获得百人之力!"), person_childs); break;
									case 1 : pk::message_box(pk::encode("不论何事,请尽管下达命令!"), person_childs); break;
									case 2 : pk::message_box(pk::encode("我将不辱父亲之名,\n鞠躬尽瘁,奋发努力!"), person_childs); break;
									case 3 : pk::message_box(pk::encode("终于到了可以工作的年龄了,\n将不会落人后地努力立功!"), person_childs); break;
									case 4 : pk::message_box(pk::encode("这一日的来临已等候多时!"), person_childs); break;
									case 5 : pk::message_box(pk::encode("我将不辱父亲之名,\n势必立下功劳!"), person_childs); break;
									case 6 : pk::message_box(pk::encode("无论是何事就请您尽管下令吧!"), person_childs); break;
								}
							else
								switch (person_childs.sex == 0 ? randint(0, 6) : randint(1, 6))
								{
									case 0 : pk::message_box(pk::encode("有我加入,就如同获得百人之力!"), person_childs); break;
									case 1 : pk::message_box(pk::encode("不论何事,请尽管下达命令!"), person_childs); break;
									case 2 : pk::message_box(pk::encode("我将不辱母亲之名,\n鞠躬尽瘁,奋发努力!"), person_childs); break;
									case 3 : pk::message_box(pk::encode("终于到了可以工作的年龄了,\n将不会落人后地努力立功!"), person_childs); break;
									case 4 : pk::message_box(pk::encode("这一日的来临已等候多时!"), person_childs); break;
									case 5 : pk::message_box(pk::encode("我将不辱母亲之名,\n势必立下功劳!"), person_childs); break;
									case 6 : pk::message_box(pk::encode("无论是何事就请您尽管下令吧!"), person_childs); break;
								}

						}
						int color = pk::get_force(childs_parent.get_force_id()).color;
						pk::history_log(pk::get_building(childs_parent.service).pos, color, pk::encode(pk::format("\x1b[1x{}\x1b[0x之{} \x1b[1x{}\x1b[0x 已经长大成人,\n于其父的阵营仕官.", childs_parent_name, person_childs_sex, person_childs_name)));
					}
					else //把俘虏也划为在野
					{
						person_childs.mibun = 身份_在野;
						person_childs.update();
						pk::history_log(person_childs.pos, -1, pk::encode(pk::format("\x1b[1x{}\x1b[0x之{} \x1b[1x{}\x1b[0x 已经长大成人,\n目前尚未仕官.", childs_parent_name, person_childs_sex, person_childs_name)));
					}

				}
			}

		}

		/*可生子的武将链表*/
		pk::list<pk::person@> getFatherList()
        {
			pk::list<pk::person@> list;
			pk::person@ pre_father;
			int stat_sum;
			for (int i = 0; i < 1000; i++)
			{
				if (i >= 700 && i < 800)
					continue;
				@pre_father = pk::get_person(i);
				if (pre_father.sex != 0)
					continue;
				if (/*pk::get_scenario().life != 3 &&*/ (pk::get_year() - pre_father.birth < 25 || pk::get_year() - pre_father.birth > 35))
					continue;
				if (!(pre_father.mibun == 身份_都督 || pre_father.mibun == 身份_太守 || pre_father.mibun == 身份_一般))
					continue;
				stat_sum = 0;
				for (int t =0; t < 5; t++)
					stat_sum += int(pre_father.stat[t]);
				if (stat_sum < 300)
					continue;

				list.add(pre_father);

			}
			return list;
		}


    } // end Main

		// 新武将公共函数 ====================================================================================================
        pk::person@ getEmptyPerson()
		{
			pk::person@ person;
            // 子嗣检查
            for (int i = 0; i < 1000; i++)
            {
                flag_has_child[i] = false;
            }
            for (int p = 0; p < 1000; p++)
			{
				@person = pk::get_person(p);
				if (person.generation <= 1)  continue;  //跳过世代小于1的武将
				if (person.father >= 0 && person.father <= 1000) flag_has_child[person.father] = true;
				if (person.mother >= 0 && person.mother <= 1000) flag_has_child[person.mother] = true;
			}

			if (slot_新武将)
			{
				for (int i = 999; i >= /*850*/900; i--)
				{
                    if (testEmptyPerson(i)) return pk::get_person(i);
				}
			}
			if (slot_古武将)
			{
				for (int i = 849; i >= 850; i--)
				{
                    if (testEmptyPerson(i)) return pk::get_person(i);
				}
			}
			if (slot_史实武将)
			{
				for (int i = 699; i >= 0; i--)
				{
                    if (testEmptyPerson(i)) return pk::get_person(i);
				}
			}
			return null;
		}

        bool testEmptyPerson(int id)
        {
            pk::person@ new_person = pk::get_person(id);
            if (new_person.mibun != 신분_없음 && new_person.mibun != 신분_사망)  return false;     //跳过活着的武将id
            if (flag_has_child[id])     return false;   //跳过有孩子的武将id
            switch (new_person.mibun)
            {
            case 신분_없음:
            case 신분_사망:
                return true;
            }
            return false;
        }

		int initNewPerson(pk::person@ new_person, int sex, pk::person@ person/*提拔者*/, int good, int bad, int excellent)
		{
			int buff_stat_min = 0, buff_stat_max = 0, buff_tekisei = 0;
			int best_stat = -1, best_stat_value = -1;
			int stat_sum = 0, luck = 0;
			int person_type = -1, talent_type = -1;

			person_type = randint(0, 5);   //随机一种类型
			if (person !is null){
				if (pk::rand_bool(33) or 类型同提拔者)	person_type = getPersonType(person);
			}

			if (pk::rand_bool(good))
			{
				buff_stat_min += 50;
				buff_stat_max += 25;
				buff_tekisei += 2;
			}
			else if (pk::rand_bool(int( bad / ((100-good)/100.f) )))  // 确保概率准确
			{
				buff_stat_min -= 40;
				buff_stat_max -= 40;
				buff_tekisei -= 2;
			}
			if (pk::rand_bool(excellent))
			{
				person_type = 6;
				buff_stat_min = 0;
				buff_stat_max = 0;
				buff_tekisei = 0;
				luck += 8;
			}

			//	性别
            new_person.sex = sex;
            //  姓名字号
			init_name(new_person, sex);
			//  基本信息
            init_normal(new_person);
            init_relation(new_person);
            init_model(new_person, sex, person_type);
			//	能力值
            stat_sum = init_ability(new_person, person_type, buff_stat_min, buff_stat_max, buff_tekisei);
			best_stat = get_best_stat(new_person);
			best_stat_value = new_person.base_stat[best_stat];

			if (person_type == 6) {
				talent_type = 16;
				title = "翘楚";
			}
			else{
				switch (best_stat)
				{
				/*
				统率	勇将0	帅才8
				武力	勇将1	猛将9
				智力	学者2	谋士10
				政治	学者3	政才11
				魅力	名人4	名流12
				全能			翘楚16
				*/
				case 0:
					if (best_stat_value < 85)	{ talent_type = 0; title = "勇将";}
					else						{ talent_type = 8; title = "帅才";}		break;
				case 1:
					if (best_stat_value < 85)	{ talent_type = 1; title = "勇将";}
					else						{ talent_type = 9; title = "猛将";}		break;
				case 2:
					if (best_stat_value < 85)	{ talent_type = 2; title = "学者";}
					else						{ talent_type = 10; title = "谋士";}	break;
				case 3:
					if (best_stat_value < 85)	{ talent_type = 3; title = "学者";}
					else						{ talent_type = 11; title = "政才";}	break;
				case 4:
					if (best_stat_value < 85)	{ talent_type = 4; title = "名人";}
					else						{ talent_type = 12; title = "名流";}	break;
				default:
												{ talent_type = -1; title = "人才";}	break;
				}
			}

			//	特技
            init_skill(new_person, talent_type);
			
			//pk::trace(pk::format("after int_skill chance：{}", 1));
			new_person.name_read = pk::encode(title);
			new_person.update();
			//pk::trace("人才选拔，智力" + new_person.base_stat[武将能力_智力]);
			if (false) 特技_进修::limit_skill_set(new_person);//, (talent_type > 8 ? 特技_进修::一级特技概率 : 0));//用以替换原版新人才系统特技设定，看效果
			if (best_stat_value >= 70)	luck += 1;
			else if (best_stat_value >= 85)	luck += 2;
			if (stat_sum >= 250)		luck += 1;
			else if (stat_sum >= 290)		luck += 2;
			else if (stat_sum >= 320)		luck += 3;
			else if (stat_sum >= 350)		luck += 4;
			else if (stat_sum >= 380)		luck += 5;

			for (int i = 1; i < 5; i++)
			{
				new_person.base_stat[i] = uint8(new_person.base_stat[i] * 新人才能力倍率 / 100.f);
			}

			return luck;
		}

        void init_name(pk::person@ new_person, int sex)
        {
            new_person.sei = pk::encode(sei_[pk::rand(sei_.length)]);
            if (use_designated_name)
            {
                if (sex == 0)   new_person.mei = pk::encode(mei_m[pk::rand(mei_m.length)]);
                else            new_person.mei = pk::encode(mei_f[pk::rand(mei_f.length)]);
            }
            else{
                new_person.mei = pk::get_family_name(randint(성씨_끝 - 1)).name;
            }
            if (sex == 0 && pk::rand_bool(33))   //部分男性有字号
                new_person.azana = pk::encode(azana_0[pk::rand(azana_0.length)]) + pk::get_family_name(randint(성씨_끝 - 1)).name;
            else
                new_person.azana = "";
            new_person.name_read = "";
            new_person.azana_read = "";
        }

        void init_normal(pk::person@ new_person)
        {
            new_person.appearance = pk::get_year();                 //登场年
			new_person.birth = pk::get_year() - randint(17, 40);    //出生年
			new_person.death = new_person.birth + randint(50, 75);  //死亡年
            new_person.cause_of_death = 0;                          //死因
            new_person.estimated_death = false;                     //死亡预定
            new_person.longevity = false;
			new_person.shoubyou = 0;                                //伤病
            new_person.absence_timer = 0;                           //
            new_person.prisoner_timer = 0;                          //
            new_person.ban_timer = 0;                               //
            new_person.former_force = -1;                           //
            new_person.banned_kunshu = -1;                          //
			new_person.action_done = true;							//

            new_person.birthplace = randint(주_유주, 주_남중);   							//出生地
			new_person.wadai = randint(화제_고사, 화제_시절);    							//特长话题
			new_person.giri = randint(의리_매우낮음, 의리_매우높음); 						//义理
			new_person.ambition = randint(야망_매우낮음, 야망_매우높음);						//野心
			new_person.promotion = randint(기용_능력, 기용_임의);							//任用
			new_person.character = randint(성격_소심, 성격_저돌);    						//性格
			new_person.kanshitsu = randint(한실_무시, 한실_중시);    						//汉室态度
			new_person.strategic_tendency = randint(전략경향_중화통일, 전략경향_현상유지);   //战略倾向
			new_person.local_affiliation = randint(지역집착_지역중시, 지역집착_지역무시);    //地缘执着
        }

        void init_relation(pk::person@ new_person)
        {
			new_person.ketsuen = new_person.get_id();   //血缘
			new_person.generation = 1;                  //世代
			new_person.aishou = randint(0, 150);         //相性
			new_person.father = -1;                     //父亲
			new_person.mother = -1;                     //母亲
			new_person.spouse = -1;                     //配偶
			new_person.gikyoudai = -1;                  //义兄
			for (int i = 0; i < 5; i++)
			{
				new_person.liked[i] = -1;				//喜爱
				new_person.disliked[i] = -1;			//厌恶
			}
        }

        void init_belong(pk::person@ new_person, pk::building@ building, bool office/*仕官*/=true, int mibun=3, int loyalty=100, int kouseki=0)
        {
			new_person.mibun = mibun;        							//身份
			new_person.loyalty = loyalty;           					//忠诚度
			new_person.kouseki = kouseki;          						//功绩
            pk::move(new_person, building);       						//所属、所在
			//结构体信息的更新处		
			personinfo@ person_t = @person_ex[new_person.get_id()];
			person_t.appeared_city = building.get_id();
			if (office)
			{
				person_t.discovered_by = pk::get_taishu_id(building);
				person_t.int_force = building.get_force_id();
				pk::set_district(new_person, building.get_district_id());	//军团
			}
            	
			new_person.update();
        }

        void init_model(pk::person@ new_person, int sex, int person_type)
        {
            if (sex == 0)
			{
				bool wujiang = true;  //类型为武将初始化
				if (person_type == 0 || person_type == 1 || person_type == 6)  //帅才、猛将、翘楚时调用武官头像
					new_person.face = randint(1400, 1485); //头像
				else  //军师、文臣、谋士、名人调用文官头像
				{
					wujiang = false;
					new_person.face = randint(1300, 1373); //头像
				}
				new_person.voice = randint(음성_소심남, 음성_저돌남);  //声音-男
				new_person.tone = randint(말투_장비, 말투_보통남);     //語气-男
				//  武将造型
				new_person.skeleton = 0;
				if (wujiang)  //为武将
				{
					array<int> new_person_body = {75, 76, 84, 85, 93, 94, 102, 103, 111, 112, 120, 121};  //武官的基本造型
					int rd_n = new_person_body[pk::rand(12)];
					new_person.body[0] = rd_n;
					new_person.body[1] = rd_n;
					new_person.body[2] = rd_n;
					new_person.body[3] = rd_n;
					new_person.body[4] = rd_n;
					new_person.body[5] = rd_n;
				}
				else
				{
					array<int> new_person_body = {79, 80, 88, 89, 97, 98, 106, 107, 115, 116, 124, 125};  //文官的基本造型
					int rd_n = new_person_body[pk::rand(12)];
					new_person.body[0] = rd_n;
					new_person.body[1] = rd_n;
					new_person.body[2] = rd_n;
					new_person.body[3] = rd_n;
					new_person.body[4] = rd_n;
					new_person.body[5] = rd_n;
				}
			}
			else
			{
				bool wujiang = true;  //类型为武将初始化
				if (person_type == 0 || person_type == 1 || person_type == 6)  //帅才、猛将、翘楚时调用武官头像
					new_person.face = randint(1700, 1732); //头像
				else  //军师、文臣、谋士、名人调用文官头像
				{
					wujiang = false;
					new_person.face = randint(1600, 1632); //头像
				}
				new_person.voice = randint(음성_냉정녀, 음성_대담녀);   //声音-女
				new_person.tone = randint(말투_고대만족, 말투_보통녀);  //語气-女
				//  武将造型
				new_person.skeleton = 1;  //更换为女性骨架
				if (wujiang)  //为武将
				{
					array<int> new_person_body = {77, 78, 86, 87, 95, 96, 104, 105, 113, 114, 122, 123};  //武官的基本造型
					int rd_n = new_person_body[pk::rand(12)];
					new_person.body[0] = rd_n;
					new_person.body[1] = rd_n;
					new_person.body[2] = rd_n;
					new_person.body[3] = rd_n;
					new_person.body[4] = rd_n;
					new_person.body[5] = rd_n;
				}
				else
				{
					array<int> new_person_body = {81, 82, 90, 91, 99, 100, 108, 109, 117, 118, 126, 127};  //文官的基本造型
					int rd_n = new_person_body[pk::rand(12)];
					new_person.body[0] = rd_n;
					new_person.body[1] = rd_n;
					new_person.body[2] = rd_n;
					new_person.body[3] = rd_n;
					new_person.body[4] = rd_n;
					new_person.body[5] = rd_n;
				}
			}
        }

        int init_ability(pk::person@ new_person, int person_type, int buff_stat_min, int buff_stat_max, int buff_tekisei)
        {
			array<int> range_stat_max = {0,0,0,0,0};
			array<int> range_stat_min = {0,0,0,0,0};
			array<int>  relative_stat = {0,0,0,0,0};
			int stat_sum, stat_min, stat_max;
			int chance_话术 = 0;

            switch (person_type)
			{
			case 0: //帅才
				range_stat_min = {68, 45, 42, 5, 40};
				range_stat_max = {88, 75, 72, 45, 80};
				stat_min = 230;
				stat_max = 310;
				buff_tekisei += 2;
				chance_话术 = 25;
				break;
			case 1: //猛将
				range_stat_min = {60, 70, 25, 5, 40};
				range_stat_max = {80, 90, 65, 45, 80};
				stat_min = 230;
				stat_max = 310;
				buff_tekisei += 2;
				chance_话术 = 25;
				break;
			case 2: //军师
				range_stat_min = {50, 10, 65, 30, 40};
				range_stat_max = {80, 50, 85, 70, 80};
				stat_min = 230;
				stat_max = 310;
				buff_tekisei += 1;
				chance_话术 = 40;
				break;
			case 3: //谋士
				range_stat_min = {25, 10, 80, 40, 40};
				range_stat_max = {65, 50, 90, 80, 80};
				stat_min = 230;
				stat_max = 310;
				buff_tekisei += 1;
				chance_话术 = 50;
				break;
			case 4: //文臣
				range_stat_min = {25, 10, 45, 70, 40};
				range_stat_max = {65, 50, 75, 90, 80};
				stat_min = 230;
				stat_max = 310;
				buff_tekisei += 1;
				chance_话术 = 45;
				break;
			case 5: //名人
				range_stat_min = {30, 30, 30, 30, 40};
				range_stat_max = {80, 80, 80, 80, 80};
				stat_min = 210;
				stat_max = 330;
				buff_tekisei += 1;
				chance_话术 = 30;
				break;
			case 6:	//翘楚
				range_stat_min = {60, 60, 60, 60, 50};
				range_stat_max = {90, 90, 90, 90, 90};
				stat_min = 330;
				stat_max = 380;
				buff_tekisei += 5;
				chance_话术 = 45;
			}
			stat_sum = randint(stat_min + buff_stat_min, stat_max + buff_stat_max);

			//	人才相对五维计算
			int relative_sum = 0;
			for (int i = 0; i < 武将能力_末; i++)
			{
				relative_stat[i] = randint(range_stat_min[i], range_stat_max[i]);
				relative_sum += relative_stat[i];
			}
			//	最终人才能力
			int final_stat = 0;
			for (int i = 0; i < 武将能力_末; i++)
			{
				final_stat = int(relative_stat[i] * stat_sum / relative_sum);
				new_person.base_stat[i] = ch::inRange(final_stat, 1, 人才能力上限);
				new_person.stat_aging[i] = randint(능력성장_초지속형, 능력성장_장비형);
				if (调试模式) ch::debug(2, pk::format("stat_0{}, stat_1{}", relative_stat[i], final_stat));
			}

			//  武将适性基数
			new_person.tekisei[병종_기병] = randint(0, 人才适性标准);
			new_person.tekisei[병종_창병] = randint(0, 人才适性标准);
			new_person.tekisei[병종_극병] = randint(0, 人才适性标准);
			new_person.tekisei[병종_노병] = randint(0, 人才适性标准);
			new_person.tekisei[병종_병기] = randint(0, 人才适性标准);
			new_person.tekisei[兵种_水军] = randint(0, 人才适性标准);
			//  随机适性升级
			int troops_type;
			buff_tekisei += randint(0, 1);
			for (int i = 0; i < buff_tekisei; i++)
			{
				troops_type = randint(병종_창병, 兵种_水军);
				if (new_person.tekisei[troops_type] >= 人才适性上限) continue;
				new_person.tekisei[troops_type] = new_person.tekisei[troops_type] + 1;
			}

			//	话术
			new_person.wajutsu_daikatsu = pk::rand_bool(chance_话术);  //大喝
			new_person.wajutsu_kiben = pk::rand_bool(chance_话术);     //诡辩
			new_person.wajutsu_mushi = pk::rand_bool(chance_话术);     //无视
			new_person.wajutsu_chinsei = pk::rand_bool(chance_话术);   //镇静
			new_person.wajutsu_gyakujou = pk::rand_bool(chance_话术);  //愤怒

			return stat_sum;
        }

        void init_skill(pk::person@ new_person, int person_type)
        {
			int _type, level, index, chance;
			bool excellent = person_type >= 16;
			_type = person_type&(8-1);//整除后取余数
			level = person_type / 8;
			if (pk::rand_bool( level > 0 ? 0 : 特技白板概率 )){
				new_person.skill = -1;
				return;
			}
			chance = randint(1, 100);
			//pk::trace(pk::format("int_skill chance：{},person_type:{}, _type:{}", chance, person_type, _type));
			switch (_type)
			{
				case 0:
					if 		(chance<=40)	new_person.skill = get_兵种特技(@new_person, person_type);
					else if (chance<=80)	new_person.skill = get_战斗特技(@new_person, person_type);
					else if (chance<=85)	new_person.skill = get_智力特技(person_type);
					else					new_person.skill = get_辅助特技(person_type);
					break;
				case 1:
					if 		(chance<=35)	new_person.skill = get_兵种特技(@new_person, person_type);
					else if (chance<=85)	new_person.skill = get_战斗特技(@new_person, person_type);
					else					new_person.skill = get_辅助特技(person_type);
					break;
				case 2:
					if 		(chance<=70)	new_person.skill = get_智力特技(person_type);
					else if (chance<=80)	new_person.skill = get_战斗特技(@new_person, person_type);
					else					new_person.skill = get_辅助特技(person_type);
					break;
				case 3:
					if 		(chance<=60)	new_person.skill = get_内政特技(person_type);
					else					new_person.skill = get_辅助特技(person_type);
					break;
				case 4:
					if 		(chance<=10)	new_person.skill = get_兵种特技(@new_person, person_type);
					else if (chance<=20)	new_person.skill = get_战斗特技(@new_person, person_type);
					else if (chance<=35)	new_person.skill = get_智力特技(person_type);
					else if (chance<=50)	new_person.skill = get_内政特技(person_type);
					else					new_person.skill = get_辅助特技(person_type);
					break;
				default:
					break;
			}
        }

		bool init_history_new_person(pk::person@ person)
		{
			
			int id = pk::rand(183);
			while (!check_duplicate(id))
			{
				id = pk::rand(183);
			}
			person_ex[person.get_id()].select_history_person_id = id;
			int temp_id = 临时id[id];
			//pk::trace("原id:" + id+ "临时id:" + temp_id);
			if (temp_id == -1) return false;
			person.sei = pk::encode(历史武将名字[id][0]);
			person.mei = pk::encode(历史武将名字[id][1]);
			person.name_read = pk::encode("历史");
			for (int i = 0; i < 武将能力_末; ++i)
			{
				person.base_stat[i] = int(1.1f * 历史武将数据_测试[temp_id][i]);
				//pk::trace("i:" + i + ",stat:" + person.base_stat[i]);
			}
			for (int i = 0; i < 兵种_末; ++i)
			{
				person.tekisei[i] = int(1.1f * 历史武将数据_测试[temp_id][i+武将能力_末]);
				//pk::trace("i:" + i + ",stat:" + person.base_stat[i]);
			}
			person.skill = 历史武将数据_测试[temp_id][武将能力_末 + 兵种_末];
			person.update();
			pk::set_face(person, 历史武将数据_测试[temp_id][武将能力_末 + 兵种_末+1]);
			person.sex = 历史武将数据_测试[temp_id][武将能力_末 + 兵种_末 + 2];
			return true;
		}

		bool check_duplicate(uint16 history_person_id)
		{
			for (int i = 0; i < 非贼武将_末; ++i)
			{
				if (history_person_id == person_ex[i].select_history_person_id) return false;
			}
			return true;
		}

		int getPersonType(pk::person@ person)
		{
			float 将帅值 = person.stat[武将能力_统率] * 0.65f + person.stat[武将能力_武力] * 0.30f +  person.stat[武将能力_智力] * 0.15f;
			float 猛将值 = person.stat[武将能力_统率] * 0.30f + person.stat[武将能力_武力] * 0.75f;
			float 军师值 = person.stat[武将能力_统率] * 0.45f + person.stat[武将能力_智力] * 0.65f;
			float 谋士值 = person.stat[武将能力_智力] * 1.00f;
			float 文臣值 = person.stat[武将能力_政治] * 0.55f + person.stat[武将能力_智力] * 0.45f;
			float 名人值 = person.stat[武将能力_魅力] * 1.00f;

			array<float> 类型值 = {将帅值, 猛将值, 军师值, 谋士值, 文臣值, 名人值};
			int type = 0;

			//  选出最高的类型值
			for (int i = 1; i < 6; i++)
			{
				if (类型值[i] > 类型值[type]) type = i;
			}
			return type;
		}

		int get_兵种特技(pk::person@ new_person, int talent_type)
		{
			int level = 0, index = 0;
			level += talent_type / 8;
			//pk::trace(pk::format("兵种特技level：{}", level));
			array<int> score_兵种 = {0,0,0,0,0,0};
			int best_兵种 = 0;
			for (int i = 0; i < 병종_끝; i++)
			{
				// A级范围0-49，S级50-99，6个兵种选score最高zhi获得該兵种战法
				score_兵种[i] = randint((new_person.tekisei[i] - 2) * 50, (new_person.tekisei[i] - 1) * 50 -1);
			}
			for (int i = 1; i < 병종_끝; i++)
			{
				if (score_兵种[best_兵种] <= score_兵种[i])
					best_兵种 = i;
			}

			int chance = randint(1, 20) + (new_person.tekisei[best_兵种] >= 3 ? 5 : 0);
			if (chance <= 10)		level += 0;
			else if (chance <= 20)	level += 1;
			else 					level += 2;
			switch (best_兵种)
			{
			case 0:
				index = pk::rand(枪兵技[level].length);
				return 枪兵技[level][index];
			case 1:
				index = pk::rand(戟兵技[level].length);
				return 戟兵技[level][index];
			case 2:
				index = pk::rand(弩兵技[level].length);
				return 弩兵技[level][index];
			case 3:
				index = pk::rand(骑兵技[level].length);
				return 骑兵技[level][index];
			case 4:
				index = pk::rand(工兵技[level].length);
				return 工兵技[level][index];
			case 5:
				index = pk::rand(水兵技[level].length);
				return 水兵技[level][index];
			default:
				return -1;
			}
			return -1;
		}

		int get_战斗特技(pk::person@ new_face, int talent_type)
		{
			int level = 0, index = 0, ts = new_face.base_stat[0], wl = new_face.base_stat[1];
			level += talent_type / 8;
			if (wl + ts <= 120)			level += 0;
			else if (wl + ts <= 160)	level += 1;
			else						level += 2;
			level = pk::min(level, 3);//上限3
			//pk::trace(pk::format("战斗特技level：{}", level));
			if ( wl > ts )
			{
				if (pk::rand_bool(70))
				{
					index = pk::rand(破军技[level].length);
					return 破军技[level][index];
				}
				else
				{
					index = pk::rand(统军技[level].length);
					return 统军技[level][index];
				}
			}
			else
			{
				if (pk::rand_bool(70))
				{
					index = pk::rand(统军技[level].length);
					return 统军技[level][index];
				}
				else
				{
					index = pk::rand(破军技[level].length);
					return 破军技[level][index];
				}
			}
		}

		int get_智力特技(int talent_type)
		{
			int level = 1, index = 0;
			level += talent_type / 8;
			level += pk::rand_bool(30) ? 1 : 0;
			//pk::trace(pk::format("智力特技level：{}", level));
			index = pk::rand(智略技[level].length);
			return 智略技[level][index];
		}

		int get_内政特技(int talent_type)
		{
			int level = 1, index = 0;
			level += talent_type / 8;
			level += pk::rand_bool(30) ? 1 : 0;
			//pk::trace(pk::format("内政特技level：{}", level));
			index = pk::rand(内政技[level].length);
			return 内政技[level][index];
		}

		int get_辅助特技(int talent_type)
		{
			int level = 1, index = 0;
			level += talent_type / 8;
			level += pk::rand_bool(40) ? 1 : 0;
			level = pk::min(level,3);//上限3
			//pk::trace(pk::format("辅助特技level：{}", level));
			index = pk::rand(辅助技[level].length);
			return 辅助技[level][index];
		}

		//	获得五维最高者
		int get_best_stat(pk::person@ person)
		{
			int best_stat = 0;
			for (int i = 1; i < 5; i++)
			{
				if (person.base_stat[i] > person.base_stat[best_stat]) best_stat = i;
			}
			return best_stat;
		}

		const array <array<int>> 历史武将数据_测试 = {
		{/*王元姬*//*统御*/24,/*武力*/18,/*智力*/80,/*政治*/72,/*魅力*/86,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/0,特技_内助,/*头像*/2205,/*性別*/1},
{/*于夫罗*//*统御*/71,/*武力*/68,/*智力*/22,/*政治*/38,/*魅力*/51,/*枪*/0,/*戟*/0,/*弩*/2,/*骑*/3,/*兵器*/1,/*水军*/0,特技_亲羌,/*头像*/2206,/*性別*/0},
{/*夏侯氏*//*统御*/14,/*武力*/6,/*智力*/52,/*政治*/60,/*魅力*/63,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/0,特技_末,/*头像*/2209,/*性別*/1},
{/*轲比能*//*统御*/77,/*武力*/72,/*智力*/51,/*政治*/57,/*魅力*/66,/*枪*/1,/*戟*/0,/*弩*/3,/*骑*/3,/*兵器*/1,/*水军*/0,特技_亲乌,/*头像*/2213,/*性別*/0},
{/*韩暨*//*统御*/9,/*武力*/6,/*智力*/62,/*政治*/75,/*魅力*/66,/*枪*/0,/*戟*/0,/*弩*/1,/*骑*/1,/*兵器*/2,/*水军*/1,特技_能吏,/*头像*/2214,/*性別*/0},
{/*关银屏*//*统御*/71,/*武力*/72,/*智力*/39,/*政治*/48,/*魅力*/70,/*枪*/2,/*戟*/2,/*弩*/1,/*骑*/1,/*兵器*/0,/*水军*/0,特技_解毒,/*头像*/2216,/*性別*/1},
{/*丘力居*//*统御*/80,/*武力*/69,/*智力*/56,/*政治*/52,/*魅力*/60,/*枪*/1,/*戟*/0,/*弩*/2,/*骑*/3,/*兵器*/3,/*水军*/0,特技_亲乌,/*头像*/2220,/*性別*/0},
{/*强端*//*统御*/60,/*武力*/77,/*智力*/26,/*政治*/43,/*魅力*/41,/*枪*/3,/*戟*/1,/*弩*/0,/*骑*/1,/*兵器*/1,/*水军*/0,特技_亲蛮,/*头像*/2221,/*性別*/0},
{/*奚泥*//*统御*/50,/*武力*/67,/*智力*/3,/*政治*/1,/*魅力*/15,/*枪*/2,/*戟*/1,/*弩*/0,/*骑*/1,/*兵器*/1,/*水军*/0,特技_藤甲,/*头像*/2222,/*性別*/0},
{/*黄乱*//*统御*/60,/*武力*/71,/*智力*/36,/*政治*/25,/*魅力*/50,/*枪*/2,/*戟*/3,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/3,特技_亲越,/*头像*/2225,/*性別*/0},
{/*蔡邕*//*统御*/22,/*武力*/12,/*智力*/86,/*政治*/82,/*魅力*/87,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/0,特技_诗想,/*头像*/2229,/*性別*/0},
{/*士壹*//*统御*/48,/*武力*/36,/*智力*/67,/*政治*/75,/*魅力*/69,/*枪*/0,/*戟*/1,/*弩*/1,/*骑*/1,/*兵器*/0,/*水军*/1,特技_繁殖,/*头像*/2230,/*性別*/0},
{/*士徽*//*统御*/69,/*武力*/66,/*智力*/44,/*政治*/31,/*魅力*/52,/*枪*/1,/*戟*/2,/*弩*/2,/*骑*/1,/*兵器*/0,/*水军*/1,特技_怒发,/*头像*/2231,/*性別*/0},
{/*士燮*//*统御*/53,/*武力*/31,/*智力*/78,/*政治*/90,/*魅力*/89,/*枪*/1,/*戟*/2,/*弩*/1,/*骑*/0,/*兵器*/1,/*水军*/0,特技_贤哲,/*头像*/2235,/*性別*/0},
{/*荀爽*//*统御*/37,/*武力*/8,/*智力*/69,/*政治*/76,/*魅力*/82,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/0,特技_贤哲,/*头像*/2241,/*性別*/0},
{/*徐奕*//*统御*/25,/*武力*/11,/*智力*/60,/*政治*/77,/*魅力*/71,/*枪*/0,/*戟*/0,/*弩*/1,/*骑*/1,/*兵器*/0,/*水军*/1,特技_循吏,/*头像*/2245,/*性別*/0},
{/*徐璆*//*统御*/68,/*武力*/48,/*智力*/62,/*政治*/76,/*魅力*/56,/*枪*/1,/*戟*/1,/*弩*/2,/*骑*/0,/*兵器*/1,/*水军*/2,特技_扫荡,/*头像*/2247,/*性別*/0},
{/*任峻*//*统御*/67,/*武力*/60,/*智力*/62,/*政治*/71,/*魅力*/58,/*枪*/1,/*戟*/2,/*弩*/1,/*骑*/1,/*兵器*/1,/*水军*/0,特技_搬运,/*头像*/2251,/*性別*/0},
{/*秦松*//*统御*/15,/*武力*/17,/*智力*/66,/*政治*/75,/*魅力*/60,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/1,特技_末,/*头像*/2252,/*性別*/0},
{/*薛礼*//*统御*/63,/*武力*/52,/*智力*/38,/*政治*/53,/*魅力*/52,/*枪*/0,/*戟*/1,/*弩*/1,/*骑*/0,/*兵器*/1,/*水军*/2,特技_末,/*头像*/2257,/*性別*/0},
{/*仓慈*//*统御*/68,/*武力*/37,/*智力*/68,/*政治*/84,/*魅力*/83,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/1,/*兵器*/0,/*水军*/0,特技_循吏,/*头像*/2263,/*性別*/0},
{/*曹节*//*统御*/36,/*武力*/4,/*智力*/68,/*政治*/70,/*魅力*/82,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/0,特技_怒发,/*头像*/2264,/*性別*/1},
{/*宗预*//*统御*/56,/*武力*/32,/*智力*/65,/*政治*/66,/*魅力*/70,/*枪*/0,/*戟*/1,/*弩*/1,/*骑*/0,/*兵器*/1,/*水军*/1,特技_论客,/*头像*/2266,/*性別*/0},
{/*苏则*//*统御*/73,/*武力*/52,/*智力*/71,/*政治*/81,/*魅力*/66,/*枪*/2,/*戟*/1,/*弩*/2,/*骑*/3,/*兵器*/1,/*水军*/0,特技_安民,/*头像*/2268,/*性別*/0},
{/*孙河*//*统御*/71,/*武力*/74,/*智力*/45,/*政治*/53,/*魅力*/60,/*枪*/2,/*戟*/1,/*弩*/1,/*骑*/2,/*兵器*/1,/*水军*/2,特技_辅佐,/*头像*/2270,/*性別*/0},
{/*孙奂*//*统御*/70,/*武力*/63,/*智力*/55,/*政治*/61,/*魅力*/62,/*枪*/2,/*戟*/2,/*弩*/0,/*骑*/1,/*兵器*/2,/*水军*/2,特技_治军,/*头像*/2271,/*性別*/0},
{/*孙资*//*统御*/22,/*武力*/13,/*智力*/79,/*政治*/70,/*魅力*/58,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/1,/*兵器*/0,/*水军*/0,特技_料敌,/*头像*/2273,/*性別*/0},
{/*孙邵*//*统御*/17,/*武力*/33,/*智力*/79,/*政治*/83,/*魅力*/71,/*枪*/0,/*戟*/0,/*弩*/1,/*骑*/0,/*兵器*/1,/*水军*/2,特技_末,/*头像*/2274,/*性別*/0},
{/*孙贲*//*统御*/63,/*武力*/67,/*智力*/40,/*政治*/47,/*魅力*/49,/*枪*/2,/*戟*/1,/*弩*/0,/*骑*/1,/*兵器*/2,/*水军*/3,特技_操舵,/*头像*/2277,/*性別*/0},
{/*孙辅*//*统御*/67,/*武力*/65,/*智力*/48,/*政治*/42,/*魅力*/43,/*枪*/2,/*戟*/0,/*弩*/0,/*骑*/1,/*兵器*/2,/*水军*/2,特技_捕缚,/*头像*/2279,/*性別*/0},
{/*张举*//*统御*/63,/*武力*/62,/*智力*/41,/*政治*/25,/*魅力*/50,/*枪*/2,/*戟*/2,/*弩*/2,/*骑*/3,/*兵器*/2,/*水军*/0,特技_末,/*头像*/2287,/*性別*/0},
{/*张纯*//*统御*/71,/*武力*/77,/*智力*/56,/*政治*/43,/*魅力*/35,/*枪*/2,/*戟*/0,/*弩*/2,/*骑*/3,/*兵器*/3,/*水军*/0,特技_末,/*头像*/2291,/*性別*/0},
{/*郑浑*//*统御*/68,/*武力*/32,/*智力*/67,/*政治*/87,/*魅力*/82,/*枪*/2,/*戟*/1,/*弩*/2,/*骑*/2,/*兵器*/1,/*水军*/2,特技_商才,/*头像*/2307,/*性別*/0},
{/*郑泰*//*统御*/50,/*武力*/31,/*智力*/80,/*政治*/74,/*魅力*/71,/*枪*/0,/*戟*/1,/*弩*/1,/*骑*/0,/*兵器*/0,/*水军*/0,特技_富豪,/*头像*/2310,/*性別*/0},
{/*彻里吉*//*统御*/65,/*武力*/63,/*智力*/46,/*政治*/49,/*魅力*/52,/*枪*/3,/*戟*/0,/*弩*/2,/*骑*/3,/*兵器*/0,/*水军*/0,特技_亲羌,/*头像*/2313,/*性別*/0},
{/*土安*//*统御*/52,/*武力*/63,/*智力*/2,/*政治*/2,/*魅力*/17,/*枪*/1,/*戟*/2,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/1,特技_藤甲,/*头像*/2314,/*性別*/0},
{/*蹋顿*//*统御*/82,/*武力*/81,/*智力*/52,/*政治*/34,/*魅力*/55,/*枪*/3,/*戟*/1,/*弩*/3,/*骑*/3,/*兵器*/1,/*水军*/0,特技_亲乌,/*头像*/2318,/*性別*/0},
{/*董白*//*统御*/5,/*武力*/2,/*智力*/43,/*政治*/51,/*魅力*/57,/*枪*/0,/*戟*/0,/*弩*/1,/*骑*/2,/*兵器*/0,/*水军*/0,特技_末,/*头像*/2319,/*性別*/1},
{/*裴潜*//*统御*/63,/*武力*/30,/*智力*/74,/*政治*/75,/*魅力*/51,/*枪*/1,/*戟*/1,/*弩*/0,/*骑*/2,/*兵器*/2,/*水军*/0,特技_亲乌,/*头像*/2325,/*性別*/0},
{/*马日磾*//*统御*/43,/*武力*/25,/*智力*/70,/*政治*/58,/*魅力*/52,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/0,特技_贤哲,/*头像*/2326,/*性別*/0},
{/*潘临*//*统御*/63,/*武力*/76,/*智力*/28,/*政治*/17,/*魅力*/25,/*枪*/2,/*戟*/2,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/3,特技_亲越,/*头像*/2329,/*性別*/0},
{/*步度根*//*统御*/66,/*武力*/74,/*智力*/45,/*政治*/44,/*魅力*/51,/*枪*/1,/*戟*/0,/*弩*/3,/*骑*/3,/*兵器*/0,/*水军*/0,特技_亲乌,/*头像*/2342,/*性別*/0},
{/*步练师*//*统御*/17,/*武力*/10,/*智力*/64,/*政治*/68,/*魅力*/87,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/1,特技_内助,/*头像*/2343,/*性別*/1},
{/*杨彪*//*统御*/22,/*武力*/17,/*智力*/69,/*政治*/75,/*魅力*/72,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/0,特技_诡计,/*头像*/2354,/*性別*/0},
{/*赖恭*//*统御*/53,/*武力*/35,/*智力*/60,/*政治*/69,/*魅力*/64,/*枪*/0,/*戟*/0,/*弩*/1,/*骑*/0,/*兵器*/1,/*水军*/1,特技_末,/*头像*/2357,/*性別*/0},
{/*陆瑁*//*统御*/11,/*武力*/12,/*智力*/71,/*政治*/68,/*魅力*/78,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/1,特技_规律,/*头像*/2363,/*性別*/0},
{/*柳毅*//*统御*/62,/*武力*/55,/*智力*/21,/*政治*/48,/*魅力*/45,/*枪*/1,/*戟*/2,/*弩*/1,/*骑*/2,/*兵器*/2,/*水军*/3,特技_操舵,/*头像*/2372,/*性別*/0},
{/*刘靖*//*统御*/60,/*武力*/35,/*智力*/64,/*政治*/81,/*魅力*/74,/*枪*/0,/*戟*/0,/*弩*/2,/*骑*/1,/*兵器*/1,/*水军*/1,特技_商才,/*头像*/2373,/*性別*/0},
{/*刘胄*//*统御*/64,/*武力*/73,/*智力*/38,/*政治*/28,/*魅力*/40,/*枪*/2,/*戟*/1,/*弩*/1,/*骑*/2,/*兵器*/0,/*水军*/0,特技_乱战,/*头像*/2374,/*性別*/0},
{/*刘放*//*统御*/20,/*武力*/15,/*智力*/80,/*政治*/73,/*魅力*/50,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/0,特技_诡计,/*头像*/2376,/*性別*/0},
{/*凉茂*//*统御*/43,/*武力*/26,/*智力*/67,/*政治*/77,/*魅力*/70,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/1,/*兵器*/1,/*水军*/0,特技_末,/*头像*/2379,/*性別*/0},
{/*卢毓*//*统御*/30,/*武力*/23,/*智力*/71,/*政治*/79,/*魅力*/78,/*枪*/0,/*戟*/0,/*弩*/0,/*骑*/0,/*兵器*/0,/*水军*/0,特技_眼力,/*头像*/2381,/*性別*/0}
		};
		/*
		const array<array<string>> 历史武将名字_测试 = {
		{"王","元姬"},
{"于","夫罗"},
{"夏侯","氏"},
{"轲","比能"},
{"韩","暨"},
{"关","银屏"},
{"丘","力居"},
{"强","端"},
{"奚","泥"},
{"黄","乱"},
{"蔡","邕"},
{"士","壹"},
{"士","徽"},
{"士","燮"},
{"荀","爽"},
{"徐","奕"},
{"徐","璆"},
{"任","峻"},
{"秦","松"},
{"薛","礼"},
{"仓","慈"},
{"曹","节"},
{"宗","预"},
{"苏","则"},
{"孙","河"},
{"孙","奂"},
{"孙","资"},
{"孙","邵"},
{"孙","贲"},
{"孙","辅"},
{"张","举"},
{"张","纯"},
{"郑","浑"},
{"郑","泰"},
{"彻","里吉"},
{"土","安"},
{"蹋","顿"},
{"董","白"},
{"裴","潜"},
{"马","日磾"},
{"潘","临"},
{"步","度根"},
{"步","练师"},
{"杨","彪"},
{"赖","恭"},
{"陆","瑁"},
{"柳","毅"},
{"刘","靖"},
{"刘","胄"},
{"刘","放"},
{"凉","茂"},
{"卢","毓"}
		};*/
		const array<array<int>> 历史武将数据 = {
{/*阿贵*//*统御*/66,/*武力*/73,/*智力*/20,/*政治*/18,/*魅力*/48,/*头像*/2200,/*性別*/0},
{/*尹楷*//*统御*/54,/*武力*/56,/*智力*/42,/*政治*/44,/*魅力*/43,/*头像*/2201,/*性別*/0},
{/*阴夔*//*统御*/22,/*武力*/27,/*智力*/64,/*政治*/75,/*魅力*/65,/*头像*/2202,/*性別*/0},
{/*越吉*//*统御*/68,/*武力*/82,/*智力*/16,/*政治*/20,/*魅力*/25,/*头像*/2203,/*性別*/0},
{/*阎温*//*统御*/36,/*武力*/44,/*智力*/55,/*政治*/57,/*魅力*/62,/*头像*/2204,/*性別*/0},
{/*王元姬*//*统御*/24,/*武力*/18,/*智力*/80,/*政治*/72,/*魅力*/86,/*头像*/2205,/*性別*/1},
{/*于夫罗*//*统御*/71,/*武力*/68,/*智力*/22,/*政治*/38,/*魅力*/51,/*头像*/2206,/*性別*/0},
{/*俄何烧戈*//*统御*/62,/*武力*/75,/*智力*/12,/*政治*/20,/*魅力*/32,/*头像*/2207,/*性別*/0},
{/*郭女王*//*统御*/22,/*武力*/6,/*智力*/70,/*政治*/64,/*魅力*/68,/*头像*/2208,/*性別*/1},
{/*夏侯氏*//*统御*/14,/*武力*/6,/*智力*/52,/*政治*/60,/*魅力*/63,/*头像*/2209,/*性別*/1},
{/*何氏*//*统御*/32,/*武力*/34,/*智力*/65,/*政治*/67,/*魅力*/60,/*头像*/2210,/*性別*/1},
{/*贾信*//*统御*/68,/*武力*/69,/*智力*/51,/*政治*/45,/*魅力*/42,/*头像*/2211,/*性別*/0},
{/*雅丹*//*统御*/60,/*武力*/54,/*智力*/69,/*政治*/51,/*魅力*/40,/*头像*/2212,/*性別*/0},
{/*轲比能*//*统御*/77,/*武力*/72,/*智力*/51,/*政治*/57,/*魅力*/66,/*头像*/2213,/*性別*/0},
{/*韩暨*//*统御*/9,/*武力*/6,/*智力*/62,/*政治*/75,/*魅力*/66,/*头像*/2214,/*性別*/0},
{/*毌丘兴*//*统御*/68,/*武力*/64,/*智力*/50,/*政治*/68,/*魅力*/70,/*头像*/2215,/*性別*/0},
{/*关银屏*//*统御*/71,/*武力*/72,/*智力*/39,/*政治*/48,/*魅力*/70,/*头像*/2216,/*性別*/1},
{/*甘氏*//*统御*/27,/*武力*/5,/*智力*/58,/*政治*/60,/*魅力*/72,/*头像*/2217,/*性別*/1},
{/*环氏*//*统御*/10,/*武力*/7,/*智力*/65,/*政治*/54,/*魅力*/66,/*头像*/2218,/*性別*/1},
{/*桓邻*//*统御*/34,/*武力*/23,/*智力*/64,/*政治*/66,/*魅力*/58,/*头像*/2219,/*性別*/0},
{/*丘力居*//*统御*/80,/*武力*/69,/*智力*/56,/*政治*/52,/*魅力*/60,/*头像*/2220,/*性別*/0},
{/*强端*//*统御*/60,/*武力*/77,/*智力*/26,/*政治*/43,/*魅力*/41,/*头像*/2221,/*性別*/0},
{/*奚泥*//*统御*/50,/*武力*/67,/*智力*/3,/*政治*/1,/*魅力*/15,/*头像*/2222,/*性別*/0},
{/*严氏*//*统御*/16,/*武力*/20,/*智力*/56,/*政治*/45,/*魅力*/62,/*头像*/2223,/*性別*/1},
{/*侯氏*//*统御*/23,/*武力*/22,/*智力*/47,/*政治*/54,/*魅力*/60,/*头像*/2224,/*性別*/1},
{/*黄乱*//*统御*/60,/*武力*/71,/*智力*/36,/*政治*/25,/*魅力*/50,/*头像*/2225,/*性別*/0},
{/*吴匡*//*统御*/64,/*武力*/75,/*智力*/45,/*政治*/53,/*魅力*/61,/*头像*/2226,/*性別*/0},
{/*呼厨泉*//*统御*/65,/*武力*/66,/*智力*/48,/*政治*/40,/*魅力*/57,/*头像*/2227,/*性別*/0},
{/*崔氏*//*统御*/8,/*武力*/25,/*智力*/55,/*政治*/42,/*魅力*/54,/*头像*/2228,/*性別*/1},
{/*蔡邕*//*统御*/22,/*武力*/12,/*智力*/86,/*政治*/82,/*魅力*/87,/*头像*/2229,/*性別*/0},
{/*士壹*//*统御*/48,/*武力*/36,/*智力*/67,/*政治*/75,/*魅力*/69,/*头像*/2230,/*性別*/0},
{/*士徽*//*统御*/69,/*武力*/66,/*智力*/44,/*政治*/31,/*魅力*/52,/*头像*/2231,/*性別*/0},
{/*士匡*//*统御*/58,/*武力*/45,/*智力*/62,/*政治*/57,/*魅力*/55,/*头像*/2232,/*性別*/0},
{/*士廞*//*统御*/50,/*武力*/38,/*智力*/55,/*政治*/68,/*魅力*/60,/*头像*/2233,/*性別*/0},
{/*士祗*//*统御*/63,/*武力*/58,/*智力*/42,/*政治*/52,/*魅力*/43,/*头像*/2234,/*性別*/0},
{/*士燮*//*统御*/53,/*武力*/31,/*智力*/78,/*政治*/90,/*魅力*/89,/*头像*/2235,/*性別*/0},
{/*士武*//*统御*/45,/*武力*/42,/*智力*/57,/*政治*/68,/*魅力*/66,/*头像*/2236,/*性別*/0},
{/*周㬂*//*统御*/68,/*武力*/51,/*智力*/77,/*政治*/55,/*魅力*/64,/*头像*/2237,/*性別*/0},
{/*周善*//*统御*/46,/*武力*/50,/*智力*/44,/*政治*/25,/*魅力*/34,/*头像*/2238,/*性別*/0},
{/*荀恽*//*统御*/18,/*武力*/12,/*智力*/60,/*政治*/66,/*魅力*/62,/*头像*/2239,/*性別*/0},
{/*荀粲*//*统御*/16,/*武力*/14,/*智力*/68,/*政治*/55,/*魅力*/50,/*头像*/2240,/*性別*/0},
{/*荀爽*//*统御*/37,/*武力*/8,/*智力*/69,/*政治*/76,/*魅力*/82,/*头像*/2241,/*性別*/0},
{/*上官子脩*//*统御*/47,/*武力*/51,/*智力*/38,/*政治*/34,/*魅力*/39,/*头像*/2242,/*性別*/0},
{/*蒋显*//*统御*/8,/*武力*/16,/*智力*/60,/*政治*/64,/*魅力*/46,/*头像*/2243,/*性別*/0},
{/*钟进*//*统御*/56,/*武力*/26,/*智力*/38,/*政治*/47,/*魅力*/40,/*头像*/2244,/*性別*/0},
{/*徐奕*//*统御*/25,/*武力*/11,/*智力*/60,/*政治*/77,/*魅力*/71,/*头像*/2245,/*性別*/0},
{/*诸葛融*//*统御*/59,/*武力*/52,/*智力*/55,/*政治*/63,/*魅力*/69,/*头像*/2246,/*性別*/0},
{/*徐璆*//*统御*/68,/*武力*/48,/*智力*/62,/*政治*/76,/*魅力*/56,/*头像*/2247,/*性別*/0},
{/*徐详*//*统御*/54,/*武力*/42,/*智力*/63,/*政治*/73,/*魅力*/70,/*头像*/2248,/*性別*/0},
{/*徐宣*//*统御*/42,/*武力*/41,/*智力*/69,/*政治*/84,/*魅力*/72,/*头像*/2249,/*性別*/0},
{/*审荣*//*统御*/50,/*武力*/57,/*智力*/45,/*政治*/48,/*魅力*/44,/*头像*/2250,/*性別*/0},
{/*任峻*//*统御*/67,/*武力*/60,/*智力*/62,/*政治*/71,/*魅力*/58,/*头像*/2251,/*性別*/0},
{/*秦松*//*统御*/15,/*武力*/17,/*智力*/66,/*政治*/75,/*魅力*/60,/*头像*/2252,/*性別*/0},
{/*石鉴*//*统御*/70,/*武力*/60,/*智力*/73,/*政治*/67,/*魅力*/22,/*头像*/2253,/*性別*/0},
{/*泄归泥*//*统御*/63,/*武力*/69,/*智力*/33,/*政治*/42,/*魅力*/47,/*头像*/2254,/*性別*/0},
{/*薛悌*//*统御*/45,/*武力*/19,/*智力*/65,/*政治*/76,/*魅力*/78,/*头像*/2255,/*性別*/0},
{/*薛兰*//*统御*/60,/*武力*/58,/*智力*/40,/*政治*/52,/*魅力*/55,/*头像*/2256,/*性別*/0},
{/*薛礼*//*统御*/63,/*武力*/52,/*智力*/38,/*政治*/53,/*魅力*/52,/*头像*/2257,/*性別*/0},
{/*鲜于丹*//*统御*/65,/*武力*/67,/*智力*/42,/*政治*/23,/*魅力*/43,/*头像*/2258,/*性別*/0},
{/*全柔*//*统御*/43,/*武力*/40,/*智力*/55,/*政治*/67,/*魅力*/58,/*头像*/2259,/*性別*/0},
{/*千万*//*统御*/67,/*武力*/61,/*智力*/28,/*政治*/30,/*魅力*/54,/*头像*/2260,/*性別*/0},
{/*曹华*//*统御*/31,/*武力*/2,/*智力*/60,/*政治*/51,/*魅力*/65,/*头像*/2261,/*性別*/1},
{/*曹宪*//*统御*/29,/*武力*/5,/*智力*/62,/*政治*/52,/*魅力*/63,/*头像*/2262,/*性別*/1},
{/*仓慈*//*统御*/68,/*武力*/37,/*智力*/68,/*政治*/84,/*魅力*/83,/*头像*/2263,/*性別*/0},
{/*曹节*//*统御*/36,/*武力*/4,/*智力*/68,/*政治*/70,/*魅力*/82,/*头像*/2264,/*性別*/1},
{/*曹彪*//*统御*/55,/*武力*/49,/*智力*/58,/*政治*/41,/*魅力*/68,/*头像*/2265,/*性別*/0},
{/*宗预*//*统御*/56,/*武力*/32,/*智力*/65,/*政治*/66,/*魅力*/70,/*头像*/2266,/*性別*/0},
{/*曹霖*//*统御*/44,/*武力*/51,/*智力*/28,/*政治*/15,/*魅力*/6,/*头像*/2267,/*性別*/0},
{/*苏则*//*统御*/73,/*武力*/52,/*智力*/71,/*政治*/81,/*魅力*/66,/*头像*/2268,/*性別*/0},
{/*孙壹*//*统御*/60,/*武力*/63,/*智力*/47,/*政治*/33,/*魅力*/46,/*头像*/2269,/*性別*/0},
{/*孙河*//*统御*/71,/*武力*/74,/*智力*/45,/*政治*/53,/*魅力*/60,/*头像*/2270,/*性別*/0},
{/*孙奂*//*统御*/70,/*武力*/63,/*智力*/55,/*政治*/61,/*魅力*/62,/*头像*/2271,/*性別*/0},
{/*孙弘*//*统御*/3,/*武力*/12,/*智力*/44,/*政治*/27,/*魅力*/5,/*头像*/2272,/*性別*/0},
{/*孙资*//*统御*/22,/*武力*/13,/*智力*/79,/*政治*/70,/*魅力*/58,/*头像*/2273,/*性別*/0},
{/*孙邵*//*统御*/17,/*武力*/33,/*智力*/79,/*政治*/83,/*魅力*/71,/*头像*/2274,/*性別*/0},
{/*孙松*//*统御*/48,/*武力*/55,/*智力*/41,/*政治*/35,/*魅力*/69,/*头像*/2275,/*性別*/0},
{/*孙霸*//*统御*/34,/*武力*/60,/*智力*/61,/*政治*/63,/*魅力*/71,/*头像*/2276,/*性別*/0},
{/*孙贲*//*统御*/63,/*武力*/67,/*智力*/40,/*政治*/47,/*魅力*/49,/*头像*/2277,/*性別*/0},
{/*孙奋*//*统御*/46,/*武力*/55,/*智力*/36,/*政治*/23,/*魅力*/12,/*头像*/2278,/*性別*/0},
{/*孙辅*//*统御*/67,/*武力*/65,/*智力*/48,/*政治*/42,/*魅力*/43,/*头像*/2279,/*性別*/0},
{/*孙虑*//*统御*/28,/*武力*/12,/*智力*/74,/*政治*/76,/*魅力*/79,/*头像*/2280,/*性別*/0},
{/*孙鲁育*//*统御*/16,/*武力*/8,/*智力*/58,/*政治*/56,/*魅力*/68,/*头像*/2281,/*性別*/1},
{/*戴员*//*统御*/35,/*武力*/32,/*智力*/52,/*政治*/48,/*魅力*/22,/*头像*/2282,/*性別*/0},
{/*卓膺*//*统御*/60,/*武力*/64,/*智力*/38,/*政治*/40,/*魅力*/46,/*头像*/2283,/*性別*/0},
{/*仲长统*//*统御*/8,/*武力*/19,/*智力*/60,/*政治*/69,/*魅力*/46,/*头像*/2284,/*性別*/0},
{/*赵叡*//*统御*/55,/*武力*/60,/*智力*/38,/*政治*/44,/*魅力*/40,/*头像*/2285,/*性別*/0},
{/*张琰*//*统御*/51,/*武力*/39,/*智力*/30,/*政治*/41,/*魅力*/42,/*头像*/2286,/*性別*/0},
{/*张举*//*统御*/63,/*武力*/62,/*智力*/41,/*政治*/25,/*魅力*/50,/*头像*/2287,/*性別*/0},
{/*张氏*//*统御*/18,/*武力*/60,/*智力*/40,/*政治*/53,/*魅力*/66,/*头像*/2288,/*性別*/1},
{/*赵咨*//*统御*/12,/*武力*/36,/*智力*/56,/*政治*/69,/*魅力*/63,/*头像*/2289,/*性別*/0},
{/*赵氏贞*//*统御*/75,/*武力*/80,/*智力*/31,/*政治*/20,/*魅力*/72,/*头像*/2290,/*性別*/1},
{/*张纯*//*统御*/71,/*武力*/77,/*智力*/56,/*政治*/43,/*魅力*/35,/*头像*/2291,/*性別*/0},
{/*张昌蒲*//*统御*/16,/*武力*/20,/*智力*/72,/*政治*/55,/*魅力*/61,/*头像*/2292,/*性別*/1},
{/*赵正*//*统御*/13,/*武力*/17,/*智力*/57,/*政治*/51,/*魅力*/58,/*头像*/2293,/*性別*/0},
{/*张先*//*统御*/56,/*武力*/67,/*智力*/38,/*政治*/21,/*魅力*/30,/*头像*/2294,/*性別*/0},
{/*张羡*//*统御*/70,/*武力*/45,/*智力*/55,/*政治*/66,/*魅力*/58,/*头像*/2295,/*性別*/0},
{/*张泉*//*统御*/48,/*武力*/46,/*智力*/43,/*政治*/35,/*魅力*/41,/*头像*/2296,/*性別*/0},
{/*张达*//*统御*/43,/*武力*/56,/*智力*/31,/*政治*/20,/*魅力*/21,/*头像*/2297,/*性別*/0},
{/*赵达*//*统御*/19,/*武力*/12,/*智力*/66,/*政治*/51,/*魅力*/59,/*头像*/2298,/*性別*/0},
{/*张著*//*统御*/51,/*武力*/58,/*智力*/35,/*政治*/31,/*魅力*/36,/*头像*/2299,/*性別*/0},
{/*张范*//*统御*/10,/*武力*/7,/*智力*/64,/*政治*/72,/*魅力*/78,/*头像*/2300,/*性別*/0},
{/*陈化*//*统御*/18,/*武力*/28,/*智力*/60,/*政治*/74,/*魅力*/69,/*头像*/2301,/*性別*/0},
{/*陈祗*//*统御*/42,/*武力*/25,/*智力*/65,/*政治*/81,/*魅力*/23,/*头像*/2302,/*性別*/0},
{/*陈就*//*统御*/58,/*武力*/65,/*智力*/32,/*政治*/28,/*魅力*/35,/*头像*/2303,/*性別*/0},
{/*陈脩*//*统御*/60,/*武力*/56,/*智力*/38,/*政治*/31,/*魅力*/52,/*头像*/2304,/*性別*/0},
{/*程畿*//*统御*/68,/*武力*/60,/*智力*/75,/*政治*/71,/*魅力*/72,/*头像*/2305,/*性別*/0},
{/*丁固*//*统御*/62,/*武力*/33,/*智力*/70,/*政治*/76,/*魅力*/67,/*头像*/2306,/*性別*/0},
{/*郑浑*//*统御*/68,/*武力*/32,/*智力*/67,/*政治*/87,/*魅力*/82,/*头像*/2307,/*性別*/0},
{/*程咨*//*统御*/53,/*武力*/52,/*智力*/50,/*政治*/47,/*魅力*/55,/*头像*/2308,/*性別*/0},
{/*郑泉*//*统御*/20,/*武力*/27,/*智力*/61,/*政治*/68,/*魅力*/72,/*头像*/2309,/*性別*/0},
{/*郑泰*//*统御*/50,/*武力*/31,/*智力*/80,/*政治*/74,/*魅力*/71,/*头像*/2310,/*性別*/0},
{/*郑胄*//*统御*/58,/*武力*/44,/*智力*/52,/*政治*/61,/*魅力*/57,/*头像*/2311,/*性別*/0},
{/*郑度*//*统御*/24,/*武力*/28,/*智力*/74,/*政治*/50,/*魅力*/21,/*头像*/2312,/*性別*/0},
{/*彻里吉*//*统御*/65,/*武力*/63,/*智力*/46,/*政治*/49,/*魅力*/52,/*头像*/2313,/*性別*/0},
{/*土安*//*统御*/52,/*武力*/63,/*智力*/2,/*政治*/2,/*魅力*/17,/*头像*/2314,/*性別*/0},
{/*邓义*//*统御*/42,/*武力*/22,/*智力*/61,/*政治*/72,/*魅力*/56,/*头像*/2315,/*性別*/0},
{/*董璜*//*统御*/43,/*武力*/37,/*智力*/26,/*政治*/18,/*魅力*/25,/*头像*/2316,/*性別*/0},
{/*董衡*//*统御*/60,/*武力*/59,/*智力*/41,/*政治*/32,/*魅力*/30,/*头像*/2317,/*性別*/0},
{/*蹋顿*//*统御*/82,/*武力*/81,/*智力*/52,/*政治*/34,/*魅力*/55,/*头像*/2318,/*性別*/0},
{/*董白*//*统御*/5,/*武力*/2,/*智力*/43,/*政治*/51,/*魅力*/57,/*头像*/2319,/*性別*/1},
{/*董扶*//*统御*/5,/*武力*/4,/*智力*/67,/*政治*/56,/*魅力*/68,/*头像*/2320,/*性別*/0},
{/*邓方*//*统御*/71,/*武力*/68,/*智力*/59,/*政治*/52,/*魅力*/55,/*头像*/2321,/*性別*/0},
{/*邓龙*//*统御*/54,/*武力*/67,/*智力*/36,/*政治*/26,/*魅力*/33,/*头像*/2322,/*性別*/0},
{/*杜琼*//*统御*/50,/*武力*/38,/*智力*/63,/*政治*/58,/*魅力*/60,/*头像*/2323,/*性別*/0},
{/*杜氏*//*统御*/16,/*武力*/3,/*智力*/50,/*政治*/48,/*魅力*/67,/*头像*/2324,/*性別*/1},
{/*裴潜*//*统御*/63,/*武力*/30,/*智力*/74,/*政治*/75,/*魅力*/51,/*头像*/2325,/*性別*/0},
{/*马日磾*//*统御*/43,/*武力*/25,/*智力*/70,/*政治*/58,/*魅力*/52,/*头像*/2326,/*性別*/0},
{/*潘氏*//*统御*/25,/*武力*/42,/*智力*/69,/*政治*/44,/*魅力*/50,/*头像*/2327,/*性別*/1},
{/*范方*//*统御*/54,/*武力*/58,/*智力*/38,/*政治*/33,/*魅力*/45,/*头像*/2328,/*性別*/0},
{/*潘临*//*统御*/63,/*武力*/76,/*智力*/28,/*政治*/17,/*魅力*/25,/*头像*/2329,/*性別*/0},
{/*费桟*//*统御*/57,/*武力*/69,/*智力*/41,/*政治*/34,/*魅力*/31,/*头像*/2330,/*性別*/0},
{/*毕轨*//*统御*/38,/*武力*/25,/*智力*/42,/*政治*/44,/*魅力*/16,/*头像*/2331,/*性別*/0},
{/*关纯*//*统御*/35,/*武力*/38,/*智力*/70,/*政治*/59,/*魅力*/61,/*头像*/2332,/*性別*/0},
{/*冯楷*//*统御*/65,/*武力*/67,/*智力*/42,/*政治*/44,/*魅力*/43,/*头像*/2333,/*性別*/0},
{/*冯氏*//*统御*/3,/*武力*/1,/*智力*/45,/*政治*/40,/*魅力*/66,/*头像*/2334,/*性別*/1},
{/*傅婴*//*统御*/46,/*武力*/58,/*智力*/41,/*政治*/37,/*魅力*/50,/*头像*/2335,/*性別*/0},
{/*文则*//*统御*/59,/*武力*/53,/*智力*/32,/*政治*/35,/*魅力*/47,/*头像*/2336,/*性別*/0},
{/*赵娥*//*统御*/44,/*武力*/74,/*智力*/66,/*政治*/30,/*魅力*/64,/*头像*/2337,/*性別*/1},
{/*鲍勋*//*统御*/41,/*武力*/35,/*智力*/74,/*政治*/68,/*魅力*/79,/*头像*/2338,/*性別*/0},
{/*鲍韬*//*统御*/57,/*武力*/70,/*智力*/43,/*政治*/42,/*魅力*/56,/*头像*/2339,/*性別*/0},
{/*庞林*//*统御*/54,/*武力*/42,/*智力*/67,/*政治*/69,/*魅力*/58,/*头像*/2340,/*性別*/0},
{/*辅匡*//*统御*/68,/*武力*/71,/*智力*/41,/*政治*/36,/*魅力*/64,/*头像*/2341,/*性別*/0},
{/*步度根*//*统御*/66,/*武力*/74,/*智力*/45,/*政治*/44,/*魅力*/51,/*头像*/2342,/*性別*/0},
{/*步练师*//*统御*/17,/*武力*/10,/*智力*/64,/*政治*/68,/*魅力*/87,/*头像*/2343,/*性別*/1},
{/*迷当大王*//*统御*/64,/*武力*/69,/*智力*/31,/*政治*/42,/*魅力*/55,/*头像*/2344,/*性別*/0},
{/*孟光*//*统御*/12,/*武力*/15,/*智力*/64,/*政治*/69,/*魅力*/36,/*头像*/2345,/*性別*/0},
{/*游楚*//*统御*/61,/*武力*/36,/*智力*/53,/*政治*/62,/*魅力*/70,/*头像*/2346,/*性別*/0},
{/*尤突*//*统御*/62,/*武力*/68,/*智力*/39,/*政治*/37,/*魅力*/33,/*头像*/2347,/*性別*/0},
{/*杨戏*//*统御*/47,/*武力*/26,/*智力*/58,/*政治*/74,/*魅力*/50,/*头像*/2348,/*性別*/0},
{/*羊徽瑜*//*统御*/20,/*武力*/14,/*智力*/71,/*政治*/65,/*魅力*/74,/*头像*/2349,/*性別*/1},
{/*杨氏*//*统御*/30,/*武力*/24,/*智力*/42,/*政治*/48,/*魅力*/62,/*头像*/2350,/*性別*/1},
{/*姚伷*//*统御*/42,/*武力*/30,/*智力*/66,/*政治*/64,/*魅力*/75,/*头像*/2351,/*性別*/0},
{/*杨定*//*统御*/62,/*武力*/60,/*智力*/46,/*政治*/33,/*魅力*/37,/*头像*/2352,/*性別*/0},
{/*羊衜*//*统御*/65,/*武力*/31,/*智力*/79,/*政治*/62,/*魅力*/63,/*头像*/2353,/*性別*/0},
{/*杨彪*//*统御*/22,/*武力*/17,/*智力*/69,/*政治*/75,/*魅力*/72,/*头像*/2354,/*性別*/0},
{/*杨敏*//*统御*/11,/*武力*/8,/*智力*/48,/*政治*/61,/*魅力*/37,/*头像*/2355,/*性別*/0},
{/*杨密*//*统御*/45,/*武力*/33,/*智力*/51,/*政治*/53,/*魅力*/50,/*头像*/2356,/*性別*/0},
{/*赖恭*//*统御*/53,/*武力*/35,/*智力*/60,/*政治*/69,/*魅力*/64,/*头像*/2357,/*性別*/0},
{/*雷绪*//*统御*/65,/*武力*/68,/*智力*/34,/*政治*/13,/*魅力*/22,/*头像*/2358,/*性別*/0},
{/*雷叙*//*统御*/60,/*武力*/63,/*智力*/34,/*政治*/24,/*魅力*/31,/*头像*/2359,/*性別*/0},
{/*来敏*//*统御*/38,/*武力*/34,/*智力*/66,/*政治*/60,/*魅力*/31,/*头像*/2360,/*性別*/0},
{/*李歆*//*统御*/62,/*武力*/75,/*智力*/45,/*政治*/44,/*魅力*/54,/*头像*/2361,/*性別*/0},
{/*陆郁生*//*统御*/22,/*武力*/33,/*智力*/61,/*政治*/50,/*魅力*/67,/*头像*/2362,/*性別*/1},
{/*陆瑁*//*统御*/11,/*武力*/12,/*智力*/71,/*政治*/68,/*魅力*/78,/*头像*/2363,/*性別*/0},
{/*李衡*//*统御*/30,/*武力*/24,/*智力*/63,/*政治*/70,/*魅力*/52,/*头像*/2364,/*性別*/0},
{/*李氏*//*统御*/15,/*武力*/12,/*智力*/66,/*政治*/70,/*魅力*/71,/*头像*/2365,/*性別*/1},
{/*李邵*//*统御*/44,/*武力*/28,/*智力*/68,/*政治*/64,/*魅力*/71,/*头像*/2366,/*性別*/0},
{/*李朝*//*统御*/51,/*武力*/37,/*智力*/57,/*政治*/69,/*魅力*/75,/*头像*/2367,/*性別*/0},
{/*李福*//*统御*/64,/*武力*/41,/*智力*/62,/*政治*/68,/*魅力*/65,/*头像*/2368,/*性別*/0},
{/*李辅*//*统御*/61,/*武力*/64,/*智力*/52,/*政治*/35,/*魅力*/36,/*头像*/2369,/*性別*/0},
{/*李密*//*统御*/14,/*武力*/6,/*智力*/65,/*政治*/71,/*魅力*/58,/*头像*/2370,/*性別*/0},
{/*刘基*//*统御*/33,/*武力*/35,/*智力*/60,/*政治*/69,/*魅力*/79,/*头像*/2371,/*性別*/0},
{/*柳毅*//*统御*/62,/*武力*/55,/*智力*/21,/*政治*/48,/*魅力*/45,/*头像*/2372,/*性別*/0},
{/*刘靖*//*统御*/60,/*武力*/35,/*智力*/64,/*政治*/81,/*魅力*/74,/*头像*/2373,/*性別*/0},
{/*刘胄*//*统御*/64,/*武力*/73,/*智力*/38,/*政治*/28,/*魅力*/40,/*头像*/2374,/*性別*/0},
{/*刘豹*//*统御*/63,/*武力*/60,/*智力*/51,/*政治*/62,/*魅力*/64,/*头像*/2375,/*性別*/0},
{/*刘放*//*统御*/20,/*武力*/15,/*智力*/80,/*政治*/73,/*魅力*/50,/*头像*/2376,/*性別*/0},
{/*刘瑁*//*统御*/51,/*武力*/40,/*智力*/47,/*政治*/44,/*魅力*/42,/*头像*/2377,/*性別*/0},
{/*吕壹*//*统御*/10,/*武力*/19,/*智力*/40,/*政治*/21,/*魅力*/3,/*头像*/2378,/*性別*/0},
{/*凉茂*//*统御*/43,/*武力*/26,/*智力*/67,/*政治*/77,/*魅力*/70,/*头像*/2379,/*性別*/0},
{/*令狐愚*//*统御*/24,/*武力*/38,/*智力*/66,/*政治*/50,/*魅力*/19,/*头像*/2380,/*性別*/0},
{/*卢毓*//*统御*/30,/*武力*/23,/*智力*/71,/*政治*/79,/*魅力*/78,/*头像*/2381,/*性別*/0},
{/*楼班*//*统御*/65,/*武力*/75,/*智力*/39,/*政治*/30,/*魅力*/52,/*头像*/2382,/*性別*/0}
		};
		const array<array<string>> 历史武将名字 = {
{"阿","贵"},
{"尹","楷"},
{"阴","夔"},
{"越","吉"},
{"阎","温"},
{"王","元姬"},
{"于","夫罗"},
{"俄","何烧戈"},
{"郭","女王"},
{"夏侯","氏"},
{"何","氏"},
{"贾","信"},
{"雅","丹"},
{"轲","比能"},
{"韩","暨"},
{"毌丘","兴"},
{"关","银屏"},
{"甘","氏"},
{"环","氏"},
{"桓","邻"},
{"丘","力居"},
{"强","端"},
{"奚","泥"},
{"严","氏"},
{"侯","氏"},
{"黄","乱"},
{"吴","匡"},
{"呼厨","泉"},
{"崔","氏"},
{"蔡","邕"},
{"士","壹"},
{"士","徽"},
{"士","匡"},
{"士","廞"},
{"士","祗"},
{"士","燮"},
{"士","武"},
{"周","㬂"},
{"周","善"},
{"荀","恽"},
{"荀","粲"},
{"荀","爽"},
{"上官","子脩"},
{"蒋","显"},
{"钟","进"},
{"徐","奕"},
{"诸葛","融"},
{"徐","璆"},
{"徐","详"},
{"徐","宣"},
{"审","荣"},
{"任","峻"},
{"秦","松"},
{"石","鉴"},
{"泄","归泥"},
{"薛","悌"},
{"薛","兰"},
{"薛","礼"},
{"鲜","于丹"},
{"全","柔"},
{"千","万"},
{"曹","华"},
{"曹","宪"},
{"仓","慈"},
{"曹","节"},
{"曹","彪"},
{"宗","预"},
{"曹","霖"},
{"苏","则"},
{"孙","壹"},
{"孙","河"},
{"孙","奂"},
{"孙","弘"},
{"孙","资"},
{"孙","邵"},
{"孙","松"},
{"孙","霸"},
{"孙","贲"},
{"孙","奋"},
{"孙","辅"},
{"孙","虑"},
{"孙","鲁育"},
{"戴","员"},
{"卓","膺"},
{"仲","长统"},
{"赵","叡"},
{"张","琰"},
{"张","举"},
{"张","氏"},
{"赵","咨"},
{"赵","氏贞"},
{"张","纯"},
{"张","昌蒲"},
{"赵","正"},
{"张","先"},
{"张","羡"},
{"张","泉"},
{"张","达"},
{"赵","达"},
{"张","著"},
{"张","范"},
{"陈","化"},
{"陈","祗"},
{"陈","就"},
{"陈","脩"},
{"程","畿"},
{"丁","固"},
{"郑","浑"},
{"程","咨"},
{"郑","泉"},
{"郑","泰"},
{"郑","胄"},
{"郑","度"},
{"彻","里吉"},
{"土","安"},
{"邓","义"},
{"董","璜"},
{"董","衡"},
{"蹋","顿"},
{"董","白"},
{"董","扶"},
{"邓","方"},
{"邓","龙"},
{"杜","琼"},
{"杜","氏"},
{"裴","潜"},
{"马","日磾"},
{"潘","氏"},
{"范","方"},
{"潘","临"},
{"费","桟"},
{"毕","轨"},
{"关","纯"},
{"冯","楷"},
{"冯","氏"},
{"傅","婴"},
{"文","则"},
{"赵","娥"},
{"鲍","勋"},
{"鲍","韬"},
{"庞","林"},
{"辅","匡"},
{"步","度根"},
{"步","练师"},
{"迷当","大王"},
{"孟","光"},
{"游","楚"},
{"尤","突"},
{"杨","戏"},
{"羊","徽瑜"},
{"杨","氏"},
{"姚","伷"},
{"杨","定"},
{"羊","衜"},
{"杨","彪"},
{"杨","敏"},
{"杨","密"},
{"赖","恭"},
{"雷","绪"},
{"雷","叙"},
{"来","敏"},
{"李","歆"},
{"陆","郁生"},
{"陆","瑁"},
{"李","衡"},
{"李","氏"},
{"李","邵"},
{"李","朝"},
{"李","福"},
{"李","辅"},
{"李","密"},
{"刘","基"},
{"柳","毅"},
{"刘","靖"},
{"刘","胄"},
{"刘","豹"},
{"刘","放"},
{"刘","瑁"},
{"吕","壹"},
{"凉","茂"},
{"令狐","愚"},
{"卢","毓"},
{"楼","班"}
		};
    Main main;
	const array<int> 临时id = { -1,-1,-1,-1,-1,0,1,-1,-1,2,-1,-1,-1,3,4,-1,5,-1,-1,-1,6,7,8,-1,-1,9,-1,-1,-1,10,11,12,-1,-1,-1,13,-1,-1,-1,-1,-1,14,-1,-1,-1,15,-1,16,-1,-1,-1,17,18,-1,-1,-1,-1,19,-1,-1,-1,-1,-1,20,21,-1,22,-1,23,-1,24,25,-1,26,27,-1,-1,28,-1,29,-1,-1,-1,-1,-1,-1,-1,30,-1,-1,-1,31,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,32,-1,-1,33,-1,-1,34,35,-1,-1,-1,36,37,-1,-1,-1,-1,-1,38,39,-1,-1,40,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,41,42,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,43,-1,-1,44,-1,-1,-1,-1,-1,45,-1,-1,-1,-1,-1,-1,-1,-1,46,47,48,-1,49,-1,-1,50,-1,51,-1 };

} // end namespace

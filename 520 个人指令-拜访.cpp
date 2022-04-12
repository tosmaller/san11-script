<<<<<<< HEAD
﻿// ## 2022/01/06 # 江东新风 # 头像数据重设 ##
=======
﻿// ## 2022/02/15 # 江东新风 # 童子状态头像重设 ##
// ## 2022/01/06 # 江东新风 # 头像数据重设 ##
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
// ## 2021/09/18 # 江东新风 # 更改background路径 ##
// ## 2021/04/08 # 白马叔叔 # 修复拜师经验异常、子女拜师闪退  ##
// ## 2021/03/14 # 白马叔叔 # 新增生子头像无重复功能，修正外出无选项等bug、称谓bug、武将排序  ##
// ## 2021/03/10 # 白马叔叔 # 修正忠诚溢出bug、多处bug、细节调整  ##
// ## 2021/02/08 # 白马叔叔 # 添加部分完成的功能  ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/08/08 # 氕氘氚 # 更改菜单按钮的位置【个人】 ##
// ## 2020/08/06 # 氕氘氚 # 优化生子系统,子女能力与父母相关,取消儿女?定id,改为每个女性最多生子2次##
// ## 2020/08/05 # 江东新风 # 修复港关拜訪bug，限制只能拜訪所在据点武将，修正語句 ##
// ## 2020/08/05 # messi # 修复两个场景不正常显示bug，修正語句，修改参数 ##
// ## 2020/08/04 # 江东新风 # 修复女性君主对話不显示的bug ##
// ## 2020/08/04 # messi # 添加男性君主对配偶的判定，女性君主对配偶的判定还不能讀取 ##
// ## 2020/07/26 ##
/*   
@제작자: HoneyBee    
@설명: 무장을 방문하여 BONUS를 얻는 이벤트 SCRIPT

-> 기교P, 행동력이 필요없고, 오직 군주의 행동상태(미완료)만 필요함   
 

== BONUS 상세 설명 ==
1. 휘하무장을 방문하여 무력 경험치 획득
2. 휘하무장을 방문하여 지력 경험치 획득
3. 휘하무장과 내기를 하여 금을 얻거나 잃음
4. 군주를 휘하무장의 친애무장으로 추가
5. 휘하무장의 혐오무장을 RANDOM으로 1人을 제거
6. 배우자에게 돈을 얻음/잃음
7. 배우자와 부부싸움 이벤트 
8. 군주의 나이가 30세 이상이면 일정확률로 아들/딸이 이벤트로 추가 (아들/딸의 나이는 15세로 설정, 아들/딸 각기 1人씩만 등장)
9. 휘하무장이 군주와 친애관계인 경우 의형제 이벤트 (휘하무장, 군주가 서로 의형제가 없을 경우 발생)
//20岁男子冠礼取字，15岁女子许嫁笄礼取字

*/

namespace 방문
{
	
	
	// ================ CUSTOMIZE ================
	
		
		const int 일기토발생률 = 50; 	 	// 일기토가 발생할 확률 (기본: 50%) 
		const int 설전발생률 = 50;  	 	// 설전이 발생할 확률 (기본: 50%)
		const int 落空概率 = 30;           //  未能遇到在野武将（暂定30%落空）
		const int jinshi_jian = 3;           //  每次禁仕减少回合数(暂定3回合)
		const int jinshi_jia = 5;             //  每次禁仕增加回合数（暂定5回合）
		const int 劝导概率 = 40;            //  触发劝导事件的概率（40%）
		const int zhong_bian = 1;        //  劝降忠诚变动
		const int order_bian = 8;       //  用刑造成的治安变动的惩罚（暂定8点）
		const int stat_award = 3;        //到达满级经验值时，追加能力奖励
		const int 发生率_拜师 = 20;    //孩子拜师事件概率
		
		const int gold_내기 = 500;  	 	// 내기에서 얻거나 잃는 금 수치 (도시 금 수치)
		const int 친애무장_확률 = 50;  	 	// 군주가 휘하무장의 친애무장으로 추가 될 확률 (기본: 50%)
		const int 혐오무장제거_확률 = 30;  	// 선택무장이 자신의 혐오무장을 제거할 확률 (기본: 50%)
		const int gold_waichu = 100;                 // 外出事件中相关金钱
		const int gold_tanbing = 200;             //探病所需金钱
		const int gold_shishi = 300;             //师事所需金钱
		const int gold_quanxiang = 500;     //劝降所需金钱
		const int gold_jianli = 200;              //拜访在野武将见面礼所需金钱
		
		const int gold_배우자 = 800;  	// 배우자가 뺏어가는 금 수치 (도시 금 수치)
		const int bonus_gold = 800;    // 배우자가 보너스로 주는 금 수치 (도시 금 수치)
		
		const int 아들_번호 = 965;			// 신무장 (아들) 번호 설정 (기본: 965)
		const int 딸_번호 = 970;			// 신무장 (딸) 번호 설정 (기본: 970)
		
		const int 아들_얼굴 = 2004;		// 신무장 (아들) 얼굴 설정 (기본: 2004) FACE EDITOR로 확인 및 변경하시면 됩니다.
		const int 딸_얼굴 = 2119;			// 신무장 (딸) 얼굴 설정 (기본: 2119) FACE EDITOR로 확인 및 변경하시면 됩니다.
		
		const int min_stat = 70;        // RANDOM으로 배정되는 아들/딸의 최소 능력치를 설정 (기본 설정 - 70)
		const int max_stat = 99;        // RANDOM으로 배정되는 아들/딸의 최대 능력치를 설정 (기본 설정 - 99)
		
		/* const bool 自动姓名范围 = false;	    // 아들/딸 이름을 자동으로 생성	[이름자동 true일시, 이름수동 false로 지정] */
		const bool 이름수동 = true;        // 아들/딸 이름을 수동(배열 작성)으로 생성 [이름수동 true일시, 이름자동 false로 지정]

		const int exp_무력 = 30;  	    // 무력 경험치
		const int exp_지력 = 30;  	    // 지력 경험치
		const int exp_tong = 30;  	    // 统帅经验值
		const int exp_mei = 30;  	    // 魅力经验值
		const int exp_shi = 15;         // 师事当能力差小于此值时的保底经验值
		const int exp_shi_beilv = 200;      // 师事获得能力差值的经验的倍率，例如倍率100时为1倍，倍率200是为2倍
		const int exp_shao = 15;  	    // 失败时得到的经验值


		//	生子系统
		const int 女性生子上限 = 2;
		const int 孩子能力倍率 = 100;		//?倍率?100时孩子的平均五维和約?280， 暴击时370
		const int 孩子能力上限 = 100;
		const int 孩子适性上限 = 3;
		const int 基础欧皇率 = 15;
		const int 基础非酋率 = 10;
		const array<string> type_name = {"帅才", "猛将", "军师", "谋士", "文臣", "名人", "翘楚"};
		
		const int KEY = pk::hash("雌算");
		
		
	// ===========================================
	 

	// 아들 [이름] (임시로 40개로 설정)
	array<string> mei_남 = { "海", "嘉", "宁", "超", "统", "真", "建", "允", "休", "智", 
											"丰", "艾", "平", "文", "楚", "通", "飞", "康", "良", "明",
											"尚", "青", "德", "进", "岱", "正", "亮", "忠", "仪", "广",
											"云", "延", "威", "昭", "和", "齐", "雄", "信", "义", "志",
											"维", "逊", "柔", "顺", "渊", "定", "济", "攸", "肃", "胜",
											"兰", "睿", "度", "修", "浩", "芝", "成", "朗", "英", "兴",
											"远", "常", "长", "台", "约", "衡", "龙", "让", "伟", "春",							
											"夏", "秋", "冬", "祖", "桓", "符", "谋", "昂", "舒", "山",
											"先", "翼", "起", "升", "霸", "孝", "直", "绩", "谦", "才",
											"俊", "方", "彦", "烈", "承", "达", "宣", "高", "至", "业"}; 
							
												   
	// 딸 [이름] (임시로 10개로 설정)
	array<string> mei_여 = {"媛", "蕊", "嫣然", "姬", "兰香", "圆圆", "芙蓉", "婉君", "玉华", "雪",
											"香香", "飞燕", "玉环", "若兰", "小小", "灵儿", "丽华", "婕妤", "诗诗", "琬",
											"梅", "杏儿", "春华", "秋月", "芳", "玉英", "玉兰", "惠", "佩", "文君",
											"妙", "凤", "巧巧", "珠", "雨荷", "如月", "玉珠", "燕燕", "玉", "莺",
											"雅", "芝", "慧", "兰", "静", "淑", "翠", "芬", "春", "菊",
											"晶", "青", "莹", "珍", "黛", "琳", "彩", "清", "蓉", "卿卿",
											"媚", "蝉", "娴", "宓", "娜", "玉奴", "娇", "娉", "灵美", "雨",
											"宝珠", "盈盈", "蝶凤", "玉娥", "昭", "琰", "华", "氏", "玲", "琪",
											"紫琼", "瑶", "琴", "璃", "瑜", "莲", "莎", "姒", "芸", "美"};
	
	// 字号的第一个字
	array<string> azana_0 = { "伯", "仲", "叔", "季", "幼", "早", "孟", "显", "惠", "雅",
											"陆", "德", "仁", "少", "公", "子", "弘", "平", "文", "长",
											"奉", "士", "元", "玄", "守", "兴", "正", "曼", "义", "汉"};
	
	// EX) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 ... 39 } <- TOTAL 40개 (ARRAY에서 [이름]을 늘릴 때마다 동일한 갯수로 설정하시면 됩니다.)
	int i = 0;   // 배열의 FIRST NUMBER 
	int j = 99;  // 배열의 LAST NUMBER (아들)
	
	int k = 89;   // 배열의 LAST NUMBER (딸)
	int l = 29;   //字号第一个字的数量
	
	array<string> horse1_name = {"棕色马", "红色马", "灰色马", "黄色马", "白色马", "栗色马"};
	
	
	class Main
	{
		
		pk::building@ building_;
		pk::building@ kunshu_building;
		pk::force@ force_;
		pk::person@ kunshu_;
		string kunshu_name;
		pk::person@ selected_person_;
		pk::person@ son_;
		pk::person@ daughter_;
		pk::person@ boy;
		pk::person@ girl;
		pk::person@ baby;
		pk::person@ echen;
		pk::person@ oldman;
		pk::person@ oldwoman;
		pk::person@ bandit_0;
		pk::person@ bandit_1;
		pk::person@ bandit_2;
		pk::person@ farmer_0;
		pk::person@ shangren_0;
		pk::person@ weibing_0;
		pk::person@ person_banned_kunshu;   
		pk::person@ child_baishi;
		pk::person@ child_shifu;
		pk::list<pk::person@> child_baishi_list;
		pk::list<pk::person@> child_shifu_list;
		pk::city@ kunshu_city;
		// pk::city@ city_;
		string selected_person_name;
		pk::list<pk::person@> person_list;
		array<pk::person@> person_sel_arr;
		pk::random random(pk::rand());
		array<int> arr_stat = {65, 70, 75, 80, 85, 90};
		string call_kunshu;
		string call_selected_person;
		string self_selected_person;
		string kunshu_city_name;
		
		int duel_result;
		int debate_result;
		
		int random_n;
		int random_m;
		int building_gold;

		Main()
		{	
			pk::menu_item item;
			item.menu = global_menu::菜单_个人;
			//ch::u8debug(pk::format("menu: baifang {}", global_menu::菜单_个人));
			item.shortcut = "1";
			item.init = pk::building_menu_item_init_t(init);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);		
			pk::bind(112, pk::trigger112_t(callback));
		}

		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@kunshu_ = pk::get_person(pk::get_kunshu_id(building));
			@boy = pk::get_person(무장_소년);
			@girl = pk::get_person(무장_소녀);
			@baby = pk::get_person(무장_아기);
			@echen = pk::get_person(무장_간신);
			@oldman = pk::get_person(무장_노옹);
			@oldwoman = pk::get_person(무장_노파);
			@bandit_0 = pk::get_person(무장_황건적);
			@bandit_1 = pk::get_person(무장_남만두목);
			@bandit_2 = pk::get_person(무장_산월두목);
			@farmer_0 = pk::get_person(무장_농민);
			@shangren_0 = pk::get_person(무장_상인);
			@weibing_0 = pk::get_person(무장_위병);
			@kunshu_building = pk::get_building(kunshu_.service);
			@kunshu_city = pk::building_to_city(kunshu_building);
			// @city_ = pk::building_to_city(building);
			kunshu_name = pk::decode(pk::get_name(kunshu_));
			kunshu_city_name = kunshu_city !is null ? pk::decode(pk::get_name(kunshu_city)) : "";
			building_gold = pk::get_gold(kunshu_building);
		}
		
		string pad_callback(int line, int original_value, int current_value)
        {
            
            return pk::encode("输入数字");  // 텍스트 표시기능인 듯 하나, 사용방법 모르겠음
        }


		bool isEnabled()
		{
			if (building_.get_id() != kunshu_.service) return false;
			else if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_)) return false;
			else if (kunshu_.action_done == true) return false;
			return true;
		}

		string getText()
		{
			return pk::encode("拜访");
		}

		string getDesc()
		{
			if (building_.get_id() != kunshu_.service)
				return pk::encode("只能在君主所在的城市或港关执行.");
			else if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_))
                return pk::encode("君主未在城中.");
			else if (kunshu_.action_done == true)
                return pk::encode("君主已完成行动.");
			else
				return pk::encode("进行拜访武将.");
		}

		bool handler()
		{
			
			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("是否拜访城中武将?"), kunshu_ ) == 1) return false;

			bool person_confirm = false; 
			
			person_list = getTargetList();
			
            while (!person_confirm)
            {
                // 무장 선택 창을 열어서 선택하기
                person_sel_arr = pk::list_to_array(pk::person_selector( pk::encode("武将选择"), pk::encode("是否拜访所在地武将."), person_list, 1, 1) );
                if (person_sel_arr.length == 0)  return false;    //在选人界面返回时返回
				
                // 무장 선택
                @selected_person_ = person_sel_arr[0];
				
                
                // 선택 취소 시 종료
                /* if (!pk::is_alive(selected_person_)) return false;    */    //测试
				
                // 선택된 무장의 이름
                selected_person_name = pk::decode(pk::get_name(selected_person_));
                
                person_confirm = pk::yes_no(pk::encode(pk::format("是否要拜访\x1b[1x{}\x1b[0x?", selected_person_name)));
            }

			pk::scene(pk::scene_t(scene_Event)); 
			
			kunshu_.action_done = true;
			// selected_person_.action_done = true;  //预留是否让选择武将也变为行动完成
			return true;
			
		}
		
		void scene_Event()
        {
			int kunshu_0 = kunshu_.get_id();
			int selected_person_0 = selected_person_.get_id();
			
			//拜访在野武将。想法：无禁仕且选项正确的前提下，概率触发单挑、舌战可以直接加入，给在野追加功绩（变相登庸）；失败则追加禁仕 （失望）
			if (selected_person_.mibun == 신분_재야)
			{
				switch (pk::rand(3))
				{
					case 0: pk::background(7); break;
					case 1: pk::background(0); break;
					case 2: pk::background(39); break;
				}
				pk::fade(255);
				switch (pk::rand(3))
				{
					case 0: pk::message_box(pk::encode(pk::format("（想必\x1b[1x{}\x1b[0x先生就是住在这里了.）", selected_person_name)), kunshu_);  break;
					case 1: pk::message_box(pk::encode(pk::format("（传闻说\x1b[1x{}\x1b[0x就住在这里.）", selected_person_name)), kunshu_);  break;
					case 2: pk::message_box(pk::encode(pk::format("（这里的环境真好啊.）", selected_person_name)), kunshu_);  break;
				}
				switch (pk::rand(2))
				{
					case 0: pk::message_box(pk::encode("请问先生在家吗?"), kunshu_);  break;
					case 1: pk::message_box(pk::encode(pk::format("请问\x1b[1x{}\x1b[0x先生在家吗?", selected_person_name)), kunshu_);  break;
				}
								
				if (pk::rand_bool(落空概率) || building_gold < 1000)   //钱过低（中途要花钱）
				{
					random_n = pk::rand(2);  
					if (random_n == 0)	
					{
						pk::message_box(pk::encode("我家先生出门去了."), boy);
						pk::message_box(pk::encode("不知先生在何处?"), kunshu_);
						pk::message_box(pk::encode("不知.\n你有什么事吗?"), boy);
						pk::message_box(pk::encode(pk::format("如此,先生回来后烦请转告,\n\x1b[1x{}\x1b[0x的\x1b[1x{}\x1b[0x拜访.", kunshu_city_name, kunshu_name)), kunshu_); 
						pk::message_box(pk::encode("好的.\n（真是个奇怪的人...）"), boy); 
						switch (pk::rand(4))
						{
							case 0: pk::say(pk::encode("运气不佳呀."), kunshu_, kunshu_city); break;
							case 1: pk::say(pk::encode("只好改天再来了."), kunshu_, kunshu_city);  break;
							case 2: pk::say(pk::encode("真希望能早点见到啊."), kunshu_, kunshu_city); break;
							case 3: pk::say(pk::encode("童儿,今天有客吗?"), selected_person_, kunshu_city); break;
						}					
					}
					else
					{
						pk::message_box(pk::encode("先生在家.\n你有什么事吗?"), boy);
						pk::message_box(pk::encode(pk::format("我是\x1b[1x{}\x1b[0x的\x1b[1x{}\x1b[0x,\n特来拜见先生的.", kunshu_city_name, kunshu_name)), kunshu_);
						pk::message_box(pk::encode("好,请稍候."), boy);
						pk::sleep();
						switch (pk::rand(3))
						{
							case 0: 	
								pk::message_box(pk::encode("先生说今日不得空.你改日再来吧."), boy);
								pk::message_box(pk::encode("啊这..."), kunshu_);
								break;
							case 1: 
								pk::message_box(pk::encode("先生现在在午睡未醒来."), boy);
								pk::message_box(pk::encode("那我等一会儿吧."), kunshu_);
								pk::message_box(pk::encode("你自便吧."), boy);
								pk::fade(0);
								pk::fade(255);
								pk::message_box(pk::encode("等了很久...")); 	
								pk::message_box(pk::encode("（看来今天见不到了...）"), kunshu_);
							break;
							case 2: 
								pk::message_box(pk::encode(pk::format("先生说不认得什么{}的{},你请回吧.", kunshu_city_name, kunshu_name)), boy); 
								pk::message_box(pk::encode("（我的名气这么低吗...）"), kunshu_);
							break;
						}
						switch (pk::rand(4))
						{
							case 0: pk::say(pk::encode("运气不佳呀."), kunshu_, kunshu_city); break;
							case 1: pk::say(pk::encode("只好改天再来了."), kunshu_, kunshu_city);  break;
							case 2: pk::say(pk::encode("要不要准备些见面礼?"), kunshu_, kunshu_city); break;
							case 3: pk::say(pk::encode("先生有请...哎?人呢?"), boy, kunshu_city); break;
						}
					}
				}
				
				else
				{
					pk::message_box(pk::encode("先生在家.\n你有什么事吗?"), boy);
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的\x1b[1x{}\x1b[0x特来拜见先生.\n这点礼物是我的一点心意.", kunshu_city_name, kunshu_name)), kunshu_);
					pk::message_box(pk::encode("好的,将军请稍候."), boy);
					pk::sleep();
					pk::message_box(pk::encode("先生有请."), boy);
					pk::fade(0);
					pk::background(5);
					pk::fade(255);
					
					switch (pk::rand(6))
					{
						case 0: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,来我这寒舍有何贵干.", kunshu_name)), selected_person_);  break;
						case 1: 
							pk::message_box(pk::encode("「\x1b[1x有朋自远方来,不亦乐乎...\x1b[0x」"), selected_person_);  
							pk::message_box(pk::encode(pk::format("（看来真如传闻一样呢...）\n久闻先生大名,今日有幸得见,我是{}.", kunshu_name)), kunshu_);
							pk::message_box(pk::encode("我不过是一介山野而已,\n您有什么事不妨直说."), selected_person_);
						break;
						case 2: pk::message_box(pk::encode("贵客驾临,失礼失礼.不知您有什么事嘛?"), selected_person_); break;
						case 3: 
							pk::message_box(pk::encode("「\x1b[1x亦余心之所善兮,虽九死其犹未悔...\x1b[0x」"), selected_person_);  
							pk::message_box(pk::encode(pk::format("（\x1b[1x屈原\x1b[0x先生的诗吗...）\n在下{},特来拜会先生.", kunshu_name)), kunshu_);
							pk::message_box(pk::encode("哦,您到了.您有什么事吗?"), selected_person_);  
						break;
						case 4: 
							pk::message_box(pk::encode("「\x1b[1x何彼苍天,不得其所.\n逍遥九州,无所定处...\x1b[0x」"), selected_person_);  
							pk::message_box(pk::encode(pk::format("（自比\x1b[1x孔仲尼\x1b[0x吗...）\n\x1b[1x{}\x1b[0x久慕先生,无缘拜会,今日得瞻,实为万幸.", kunshu_name)), kunshu_);
							pk::message_box(pk::encode(pk::format("您莫非就是\x1b[1x{}\x1b[0x将军吧?\n童儿,快快上茶.", kunshu_name)), selected_person_);  
						break;
						case 5: 
							pk::message_box(pk::encode("「\x1b[1x山谷有贤兮,欲投明主.明主求贤兮,却不知吾...\x1b[0x」"), selected_person_);  
							pk::message_box(pk::encode(pk::format("（看来先生有意出仕...）\n\x1b[1x{}\x1b[0x久慕先生,特来拜会.", kunshu_name)), kunshu_);
							pk::message_box(pk::encode("哦哦,您到了,请坐吧.\n童儿,快上茶."), selected_person_);  
						break;
					}
					array<string> answers =
					{
						pk::encode("匡扶汉室"),    //初定，0与汉室重视有关
						pk::encode("救民水火"),    //初定，1与启用义理有关
						pk::encode("成就霸业"),    //初定，2与野心有关
						pk::encode("才能发挥"),    //初定，3与启用才能有关
						pk::encode("功名声望"),    //初定，4与启用名声有关
						pk::encode("荣华富贵")     //初定，5与义理有关
					};
					/* int n= pk::choose(pk::encode("选择要谈论的话题"), answers);  */  //测试版本1
					int n;           //测试版本2作弊 开始
					if (selected_person_.banned_kunshu != kunshu_0)
					{
						n = pk::choose(pk::encode(pk::format("选择要谈论的话题\n*\x1b[1x{}\x1b[0x对\x1b[1x{}\x1b[0x军似乎没什么成见.*", selected_person_name, kunshu_name)), answers); 
					}
						
					else
					{
						n = pk::choose(pk::encode(pk::format("选择要谈论的话题\n*\x1b[1x{}\x1b[0x对\x1b[1x{}\x1b[0x军似乎存在一些成见.*", selected_person_name, kunshu_name)), answers); 
					}              //测试版本2作弊 结束
					
					if (n == 0)
					{
						pk::message_box(pk::encode("如今汉室倾颓,奸臣窃命,\n我虽名微力薄,也想匡扶汉室,\n希望能得到先生相助."), kunshu_);
						if (selected_person_.kanshitsu == 2 and kunshu_.kanshitsu == 2)
						{
							pk::message_box(pk::encode("将军的诚心与志向令人动容.\n我也何尝不想与国出力呢."), selected_person_);  
							pk::message_box(pk::encode("既如此不如..."), kunshu_);
							pk::message_box(pk::encode("且容我考虑考虑,将军今天就先回吧."), selected_person_);  
							scene_jinshi_0();
						}
						else if (selected_person_.kanshitsu == 2)
						{
							pk::cutin(13); 
							pk::message_box(pk::encode(pk::format("花言巧语之徒,祸乱朝纲之辈!\n人不识你{},你道天下人都不识得吗?!\n童儿,送客!", kunshu_name)), selected_person_);  
							pk::message_box(pk::encode("啊啊...请等一下...容我..."), kunshu_);
							scene_jinshi_1();
							pk::say(pk::encode("看来做人还是要实在些.."), kunshu_, kunshu_city); 
						}
						else
						{
							pk::message_box(pk::encode("\x1b[1x分分合合,盛极而衰\x1b[0x,乃是天命.\n将军只怕徒费心力而已."), selected_person_);  
							pk::message_box(pk::encode("先生所说的诚然是高见,\n但我也有自己的立场,恕难听天由命."), kunshu_);
							pk::say(pk::encode("真的是那样吗?"), kunshu_, kunshu_city); 
						}
					}
					
					else if (n == 1)
					{
						pk::message_box(pk::encode("自\x1b[1x黄巾\x1b[0x乱始,\n贼人四处劫掠烧杀,民众流离失所."), kunshu_);
						pk::message_box(pk::encode("我虽然没什么德才,但一心想拯救天下百姓,让他们重新过上安定的生活,\n愿先生以苍生为念不惜赐教!"), kunshu_);
						if (selected_person_.promotion == 3 and kunshu_.giri >2)
						{
							pk::message_box(pk::encode("将军所想也正是我之所想,\n可是恨力不能."), selected_person_);  
							pk::message_box(pk::encode("既如此不如..."), kunshu_);
							pk::message_box(pk::encode("且容我考虑考虑,将军今天就先回吧."), selected_person_);  
							scene_jinshi_0();
						}
						else if (selected_person_.promotion == 3)
						{
							pk::cutin(13); 
							pk::message_box(pk::encode(pk::format("言行不一之辈!\n盘踞一方为害百姓的贼人正是你等!!\n别人不知你{}!你道天下人都不知吗?", kunshu_name)), selected_person_);  
							pk::message_box(pk::encode("童儿,送客!"), selected_person_);  
							pk::message_box(pk::encode("啊啊...请等一下...\n（到底是哪里不对呢?）"), kunshu_);
							scene_jinshi_1();
							pk::say(pk::encode("可恶,竟然.."), kunshu_, kunshu_city); 
						}
						else
						{
							pk::message_box(pk::encode("\x1b[1x老子云,「天地不仁,以万物为刍狗」.\x1b[0x\n各人命数所在,救得了此时,救不了彼时.\n将军何不顺应天数?"), selected_person_);  
							pk::message_box(pk::encode("天数自然无法违逆,\n但我还是想尽己所能帮助他们."), kunshu_);
							pk::say(pk::encode("\x1b[1x尽人事,知天命.\x1b[0x"), kunshu_, kunshu_city); 
						}
					}
					
					else if (n == 2)
					{
						pk::message_box(pk::encode("如今汉室天命衰微,民怨四起,战乱不断."), kunshu_);
						pk::message_box(pk::encode("我想要终结乱世,成就一番大业,\n不知先生可愿相助?"), kunshu_);
						if (selected_person_.ambition > 2 and kunshu_.strategic_tendency == 전략경향_중화통일)
						{
							pk::cutin(20); 
							pk::message_box(pk::encode("将军之雄心令人动容,\n欲成大事,当如是也."), selected_person_);  
							pk::message_box(pk::encode("既如此不如..."), kunshu_);
							pk::message_box(pk::encode("且容我考虑考虑,将军今天就先回吧."), selected_person_);  
							scene_jinshi_0();
						}
						else if (selected_person_.ambition > 2)
						{
							pk::cutin(17); 
							pk::message_box(pk::encode("恐怕将军之志不在于此吧?\n\x1b[1x无志则不足以成其事.\x1b[0x\n既无此心何不坦然面对自己的内心呢?"), selected_person_);  
							pk::message_box(pk::encode("啊这...\n（被看穿了吗?先生真神人也...）"), kunshu_);
							scene_jinshi_1();
							pk::say(pk::encode("只想守护一方也有错吗.."), kunshu_, kunshu_city); 
						}
						else
						{
							pk::message_box(pk::encode("人各有志,吾只是个山野慵懒之徒,\n喜好种地饮酒交游,\n对定国安邦之事不感兴趣,不劳您下问."), selected_person_); 
							pk::message_box(pk::encode("那先生能否到府上作客?愿奉为上宾."), kunshu_);		
							pk::message_box(pk::encode("人各有志,将军请回吧."), selected_person_);
							pk::say(pk::encode("要不要换个话题."), kunshu_, kunshu_city); 
						}
					}
					
					else if (n == 3)
					{
						pk::message_box(pk::encode("先生怀抱经世奇才,岂可空老于林泉之下?\n或欲经天纬地,或欲\x1b[1x封狼居胥\x1b[0x,可愿加入?"), kunshu_);
						if (selected_person_.promotion == 0 and kunshu_.promotion == 0)   //初定条件有些不够满意
						{
							pk::message_box(pk::encode("我也有意欲投明主,但明主却不知吾.\n听闻将军帐下谋臣如雨,猛将如云,\x1b[1x唯才是举\x1b[0x,各司其职."), selected_person_);
							pk::message_box(pk::encode("既如此,先生不妨..."), kunshu_);
							pk::message_box(pk::encode("且容我考虑考虑,将军今天就先回吧."), selected_person_);  
							scene_jinshi_0();
						}
						else if (selected_person_.promotion == 0)
						{
							pk::cutin(17); 
							pk::message_box(pk::encode("可据闻,贤士有良策却不被采纳,\n良将有高见您却充耳不闻,\n何况我这样的人,恐怕不会受重用吧."), selected_person_);  
							pk::message_box(pk::encode("那些人怎么能和先生相提并论呢?\n先生请听我..."), kunshu_);
							pk::message_box(pk::encode("请回!"), selected_person_);  
							scene_jinshi_1();
							pk::say(pk::encode("到底是谁说的..可恶!"), kunshu_, kunshu_city); 
						}
						else
						{
							pk::message_box(pk::encode("我才疏学浅,哪有那样的本事?\n将军奈何舍美玉而求顽石呢?"), selected_person_); 
							pk::message_box(pk::encode("哪里哪里,先生太过于自谦了."), kunshu_);
							pk::message_box(pk::encode("都是外界的一些虚言而已."), selected_person_); 
							pk::say(pk::encode("下次说点别的吧?"), kunshu_, kunshu_city); 							
						}
					}
					
					else if (n == 4)
					{
						pk::message_box(pk::encode("人生在世唯功名声望耳,\n达官显贵,光耀门庭,封侯拜相,恩荫子孙."), kunshu_);
						pk::message_box(pk::encode("先生一定也是这样想的吧?\n何不加入我军呢."), kunshu_);
						if (selected_person_.promotion == 2 and force_.title < 5)  
						{
							/* string kunshu_forcename = pk::decode(force.title); */
							pk::message_box(pk::encode(pk::format("得蒙\x1b[1x{}\x1b[0x大人知遇,莫敢不从.", kunshu_name)), selected_person_);
							pk::message_box(pk::encode("既如此,先生不妨..."), kunshu_);
							pk::message_box(pk::encode("且容我考虑考虑,将军今天就先回吧."), selected_person_);  
							scene_jinshi_0();
							
						}
						else if (selected_person_.promotion == 2)
						{
							pk::cutin(17); 
							pk::message_box(pk::encode("可是以将军现在的官职,\n尚且没到位高权重之时,\n何况我这样的人,恐怕更没有什么机会吧."), selected_person_);  
							pk::message_box(pk::encode("现今我确是名微言轻,但不能代表今后..."), kunshu_);
							pk::message_box(pk::encode("今后事今后再议也不迟."), selected_person_);  
							pk::message_box(pk::encode("啊,可..."), kunshu_);
							scene_jinshi_1();
							pk::say(pk::encode("等着看吧,今后就到你来求了.."), kunshu_, kunshu_city); 
						}
						else
						{
							pk::message_box(pk::encode("我闲散惯了,无意功名久矣,容他日再见."), selected_person_); 
							pk::message_box(pk::encode("既如此,那不便强求了."), kunshu_);	
							pk::say(pk::encode("似乎不感兴趣呢?"), kunshu_, kunshu_city); 
						}
					}
					
					else
					{
						pk::message_box(pk::encode("\x1b[1x太史公云,\n天下熙熙,皆为利来.天下攘攘,皆为利往.\x1b[0x\n人生所图不过利耳."), kunshu_);
						pk::message_box(pk::encode("只要先生能相助,\n保证今生荣华富贵,享之不尽!"), kunshu_);
						pk::message_box(pk::encode("只是..."), selected_person_);  
						pk::background("media/background/baifang/shangci03-1.png", 0);
						pk::play_se(16);
						pk::sleep(50);
						pk::background("media/background/baifang/shangci03-2.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/shangci03.png", 0);
						pk::sleep(1500);
						pk::background("media/background/baifang/shangci03-2.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/shangci03-1.png", 0);
						pk::sleep(50);
						pk::background(5);
						
						pk::message_box(pk::encode("这金珠玉带还请笑纳."), kunshu_);
						if (selected_person_.giri < 2 and building_gold > 40000 and pk::get_food(kunshu_building) > 200000)
						{
							pk::message_box(pk::encode("何为有此?"), selected_person_); 
							pk::message_box(pk::encode("略表我的心意而已,非聘贤之礼."), kunshu_);
							pk::message_box(pk::encode("如有用得着我的地方,全凭吩咐!"), selected_person_);  

							if ( selected_person_.banned_kunshu == kunshu_0)  //禁仕君主ID是玩家时
							{
								selected_person_.banned_kunshu = -1;
								selected_person_.ban_timer = 0;	
								selected_person_.update();
							}
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x对\x1b[1x{}\x1b[0x军的成见完全消除了.", selected_person_name, kunshu_name))); 
						}
						else if (selected_person_.giri < 2)
						{
							pk::cutin(17); 
							pk::message_box(pk::encode("可依在下看来,\n将军库中早已空虚,举手投足捉襟见肘,\n恐怕你自身尚且难保吧."), selected_person_);  
							pk::message_box(pk::encode("最近确实是有点...不过马上就能...\n先生您先等一下?"), kunshu_);
							pk::message_box(pk::encode("勿复言."), selected_person_);  
							scene_jinshi_1();
							pk::say(pk::encode("难道还要更多?"), kunshu_, kunshu_city); 
						}
						else
						{
							pk::message_box(pk::encode("金银财宝,纸醉金迷,\n不过是过眼烟雨,终究会消散的.\n这样的人又有几个能名垂青史?"), selected_person_); 
							pk::message_box(pk::encode("先生此言差异."), kunshu_);	
							pk::message_box(pk::encode("道不同不相为谋."), selected_person_); 
							pk::say(pk::encode("金银财宝都打动不了吗?"), kunshu_, kunshu_city); 
						}
						pk::add_gold(building_, -500, true);
						building_.update();
						pk::message_box(pk::encode("金珠玉带花了些钱.")); 
					}
					pk::add_gold(building_, -gold_jianli, true);
					building_.update();
					pk::message_box(pk::encode("登门拜访花了些钱.")); 
					/* if (selected_person_.banned_kunshu != -1)     //此处为测试的
					{
						@person_banned_kunshu = pk::get_person(selected_person_.banned_kunshu); 
						string person_banned_kunshu_name = pk::decode(pk::get_name(person_banned_kunshu)); 
						pk::message_box(pk::encode(pk::format("他的禁仕君主是\x1b[1x{}\x1b[0x，禁仕期为\x1b[1x{}\x1b[0x回合.", person_banned_kunshu_name, selected_person_.ban_timer)), kunshu_); 
					}
					else
					{
						pk::message_box(pk::encode("他似乎没有禁仕任何君主."), kunshu_); 
					} */
				}
			}
			
			//俘虏拜访
			else if (selected_person_.mibun == 신분_포로)
			{
				pk::fade(0);
				pk::background("media/background/baifang/jianyu01.png", 0);
				pk::fade(255);
				pk::message_box(pk::encode("哼哼.."), selected_person_);
				
				if (!pk::is_valid_force_id(selected_person_.get_force_id()))     //无归属俘虏判定
				{
					array<string> answers =
					{
						pk::encode("  劝导  "),
						pk::encode("  利诱  ")
					};
					
					/* int n = pk::choose(pk::encode("这里是监狱,\n选择要谈论的话题."), answers);   //测试版本1 */
					int n;           //测试版本2作弊 开始
					if (selected_person_.banned_kunshu != kunshu_0)
					{
						n = pk::choose(pk::encode(pk::format("这里是监狱,\n选择要谈论的话题.\n*\x1b[1x{}\x1b[0x对\x1b[1x{}\x1b[0x军似乎没什么成见.*", selected_person_name, kunshu_name)), answers); 
					}
						
					else
					{
						n = pk::choose(pk::encode(pk::format("这里是监狱,\n选择要谈论的话题.\n*\x1b[1x{}\x1b[0x对\x1b[1x{}\x1b[0x军似乎存在一些成见.*", selected_person_name, kunshu_name)), answers); 
					}              //测试版本2作弊 结束
					if (n == 0)
					{
						switch (pk::rand(3))
						{
							case 0: pk::message_box(pk::encode("将军,岂不闻\x1b[1x良禽择木而栖\x1b[0x?\n以君之才不留有用之身,\n难道觉得不可惜吗?"), kunshu_); break;
							case 1: pk::message_box(pk::encode("在下仰慕将军已久,\n想请将军相辅,\n望将军慎重考虑."), kunshu_); break;
							case 2: pk::message_box(pk::encode("将军是聪明之人,\n如今故主已去旧誓不存,\n不如相助于我共成大业?"), kunshu_); break;
						}
						int random_j = pk::rand(100) + 1;
						if (random_j <= 15)     //预设小概率直接劝导成功
						{
							switch (pk::rand(3))
							{
								case 0: pk::message_box(pk::encode("确如君所言,\n\x1b[1x大丈夫当带三尺剑立不世功\x1b[0x,又怎么能轻易死去呢."), selected_person_); break;
								case 1: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人所说的,\n容我考虑考虑再作答复.", kunshu_name)), selected_person_); break;
								case 2: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人威名远扬,\n{}有幸相随岂有不从之理.", kunshu_name, selected_person_name)), selected_person_);  break;
							}
							pk::message_box(pk::encode("（出乎意料地顺利呀.）\n既如此,我今天就先告辞了."), kunshu_);
							
							pk::add_stat_exp(kunshu_, 4, exp_shao);
							//初定，直接说服魅力经验少许提升
							kunshu_.update();
							scene_jinshi_0();
							
						}
						else if (random_j > 15 and random_j <= 50)      //小概率触发舌战
						{
							pk::message_box(pk::encode("如此也..."), selected_person_);
							pk::cutin(20); 
							pk::message_box(pk::encode("将军请好好听我一言."), kunshu_);
							debate_result = pk::debate(kunshu_, selected_person_, pk::is_player_controlled(kunshu_), false, false, true).first;
							pk::background("media/background/baifang/jianyu01.png", 0);
							if (debate_result == 0)   //舌战胜
							{
								pk::cutin(13); 
								pk::message_box(pk::encode("这下你该明白了吧?"), kunshu_);
								pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人所说的,\n我已经真真切切感受到了,我会给您一个答复的.", kunshu_name)), selected_person_); 
								pk::message_box(pk::encode("既如此,我今天就先告辞了."), kunshu_);
								
								pk::add_stat_exp(kunshu_, 2, exp_shao);
								kunshu_.update();
								pk::add_stat_exp(selected_person_, 2, exp_shao);
								scene_jinshi_0();
								
							}
							else
							{
								pk::message_box(pk::encode("额..完全被你说服了."), kunshu_);
								switch (pk::rand(3))
								{
									case 0: pk::message_box(pk::encode(pk::format("哼哼,\n诚如我之前所知,\x1b[1x{}\x1b[0x军也不过如此.你回去吧.", kunshu_name)), selected_person_);  break;
									case 1: pk::message_box(pk::encode(pk::format("我不认为\x1b[1x{}\x1b[0x能施展我的才华,\n请勿复言!", kunshu_name)), selected_person_); break;
									case 2: pk::message_box(pk::encode(pk::format("虽然我的故主不在了,\n但我还至于要加入\x1b[1x{}\x1b[0x军.", kunshu_name)), selected_person_);  break;
								}

								pk::message_box(pk::encode("..."), kunshu_);
								
								pk::add_stat_exp(kunshu_, 2, exp_shao);
								kunshu_.update();
								pk::add_stat_exp(selected_person_, 2, exp_shao);
								scene_jinshi_1();     //初定，说服失败会被追加禁仕期
		
							}
						}
						else			//中概率被拒绝			
						{
							switch (pk::rand(3))
							{
								case 0: pk::message_box(pk::encode("不必多费唇舌,\n我是绝对不会屈服的!"), selected_person_);break;
								case 1: 
									pk::message_box(pk::encode("\x1b[1x南飞的鸦鹊呀,不知道何处才能停靠呢.\x1b[0x"), selected_person_); 
									pk::message_box(pk::encode("（完全插不上话呢...）"), kunshu_);
								break;
								case 2: pk::message_box(pk::encode("虽然我的故主不在了,\n但我还没想好要加入哪一方."), selected_person_); break;
							}
							
						}
					
					}
					else     //利诱
					{
						if (building_gold < gold_quanxiang)
						{
							pk::message_box(pk::encode("如今库中羞涩,这点东西恐怕诚意不足.\n先回吧!"), kunshu_);
							pk::fade(0);
							pk::background(-1);	
							return;
						}
						pk::background("media/background/baifang/shangci02-1.png", 0);
						pk::play_se(16);
						pk::sleep(50);
						pk::background("media/background/baifang/shangci02-2.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/shangci02.png", 0);
						pk::sleep(1500);
						pk::background("media/background/baifang/shangci02-2.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/shangci02-1.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/jianyu01.png", 0);
						switch (pk::rand(3))
						{
							case 0: pk::message_box(pk::encode(pk::format("在下仰慕将军已久,\n这\x1b[1x{}\x1b[0x金是我的一点心意,\n请您一定收下.", gold_quanxiang)), kunshu_); break;
							case 1: pk::message_box(pk::encode("将军是聪明之人,\n想必我此行的目的您已经知晓,\n区区的薄礼还请笑纳."), kunshu_); break;
							case 2: pk::message_box(pk::encode("人生在世唯名利耳,\n眼前的这些金银不成敬意,\n我保证您今后富贵荣华!"), kunshu_); break;
						}
						int random_k = pk::rand(100) + 1;			
						if (selected_person_.giri == 의리_매우낮음)
						{
							if (random_k <= 80)
							{
								pk::message_box(pk::encode(pk::format("（竟然给这么多...）多谢\x1b[1x{}\x1b[0x大人的厚爱.", kunshu_name)), selected_person_);
								
								scene_jinshi_0();								
							}
							else
							{
								pk::message_box(pk::encode("无功不受禄,请回...\n（咽...）"), selected_person_); 
							}
						} //义理非常低的人非常容易被打动
						
						else if (selected_person_.giri == 의리_낮음)
						{
							if (random_k <= 60)
							{
								pk::message_box(pk::encode(pk::format("多谢\x1b[1x{}\x1b[0x大人的赏赐.", kunshu_name)), selected_person_);
								
								scene_jinshi_0();
							}
							else
							{
								pk::message_box(pk::encode("无功不受禄,请回吧."), selected_person_); 
							}
						}  //义理低的人容易被打动
						
						else if (selected_person_.giri == 의리_보통)
						{
							
							if (random_k <= 35)
							{
								pk::message_box(pk::encode("多谢了."), selected_person_); 
								
								scene_jinshi_0();
							}
							else if (random_k > 95)
							{
								pk::message_box(pk::encode("别看我这样,我也是很重视名节的人."), selected_person_); 
								
								scene_jinshi_1();
							}
							else
							{
								pk::message_box(pk::encode("无功不受禄,请回."), selected_person_); 
							}
						}  //义理中不太容易被打动
						
						else if (selected_person_.giri == 의리_높음)
						{
							if (random_k <= 20)
							{
								pk::message_box(pk::encode("甚好...甚好..."), selected_person_); 
								
								scene_jinshi_0();
							}
							else if (random_k > 90)
							{
								pk::cutin(13); 
								pk::message_box(pk::encode("区区一些钱币,就想换取我的忠心?给我滚!"), selected_person_); 
								
								scene_jinshi_1();
							}
							else
							{
								pk::message_box(pk::encode("无功不受禄."), selected_person_); 
							}
						}  //义理高的人很难被打动
						else
						{
							if (random_k <= 10)
							{
								pk::message_box(pk::encode("如此...也..."), selected_person_); 
								
								scene_jinshi_0();
							}
							else if (random_k > 70)
							{
								pk::cutin(13); 
								pk::message_box(pk::encode("我的忠义岂容你来玷污!"), selected_person_); 
								scene_jinshi_1();
							}							
							else
							{
								pk::message_box(pk::encode("哼."), selected_person_); 
							}
						}  //义理非常高的人很有想法
						pk::add_gold(building_, -gold_quanxiang, true);					
						building_.update();
						pk::message_box(pk::encode("劝诱俘虏花了些钱.")); 
						
					}
					
				}
				else      //有归属的俘虏
				{
					array<string> jianyu_answers;
					if (building_.get_id() < 建筑_城市末)  //为城市时
					{
						jianyu_answers =
						{
							pk::encode("  劝导  "),
							pk::encode("  利诱  "),
							pk::encode("  用刑  ")
						};
					}
					else   //非城市时不可用刑（因为没有治安处罚）
					{
						jianyu_answers =
						{
							pk::encode("  劝导  "),
							pk::encode("  利诱  ")
						};
					}
					
					int n = pk::choose(pk::encode("这里是监狱,\n选择要谈论的话题"), jianyu_answers);
					if (n == 0)
					{
						switch (pk::rand(3))
						{
							case 0: pk::message_box(pk::encode("将军屈身事主却不被重用,\n岂不闻\x1b[1x良禽择木而栖\x1b[0x?"), kunshu_); break;
							case 1: pk::message_box(pk::encode("在下仰慕将军已久,\n想请将军相辅,\n望将军慎重考虑."), kunshu_); break;
							case 2: pk::message_box(pk::encode("将军是聪明之人,\n如今已经是大势所趋,\n不如弃暗投明帮助我?"), kunshu_); break;
						}
						if (pk::rand_bool(劝导概率))
						{
							pk::message_box(pk::encode("如此也..."), selected_person_);
							pk::cutin(20); 
							pk::message_box(pk::encode("将军请好好听我一言."), kunshu_);
							debate_result = pk::debate(kunshu_, selected_person_, pk::is_player_controlled(kunshu_), false, false, true).first;
							pk::background("media/background/baifang/jianyu01.png", 0);
							pk::fade(255);
							if (debate_result == 0)
							{
								random_n = pk::rand(3);
								if (random_n == 0)	
								{
									pk::message_box(pk::encode("关在牢里头脑昏沉,\n刚才思如涌泉,我还要再来一辩!"), selected_person_);
															
									// 再次舌战事件的武将智力调整
									int shentizhuangtai = selected_person_.shoubyou;
									int 원본지력 = selected_person_.base_stat[武将能力_智力];
									bool 원본화술_wajutsu_kiben = selected_person_.wajutsu_daikatsu;
									bool 원본화술_wajutsu_mushi = selected_person_.wajutsu_daikatsu;
									bool 원본화술_wajutsu_chinsei = selected_person_.wajutsu_daikatsu;
									
									// 再次舌战口才追加
									selected_person_.wajutsu_kiben = true;
									selected_person_.wajutsu_mushi = true;
									selected_person_.wajutsu_chinsei = true;
									
									selected_person_.shoubyou = 0;
										
									if (selected_person_.base_stat[武将能力_智力] <= 70)									
									{	
										selected_person_.base_stat[武将能力_智力] = arr_stat[pk::rand(6)];
										selected_person_.update();
									}
									else
									{
										selected_person_.update();
									}
																	
									debate_result = pk::debate(kunshu_, selected_person_, pk::is_player_controlled(kunshu_), false, false, true).first;
									if (debate_result == 0)
									{
										pk::cutin(13); 
										pk::message_box(pk::encode("这下你该明白了吧?"), kunshu_);
										pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人所说的,\n容我考虑考虑再作答复.", kunshu_name)), selected_person_); 
										pk::message_box(pk::encode("那么我今天就先告辞了."), kunshu_);
										pk::add_stat_exp(kunshu_, 2, exp_shao);
										pk::add_stat_exp(selected_person_, 2, exp_shao);
										pk::add_loyalty(selected_person_, -zhong_bian * 2);
										
										kunshu_.update();									

										pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的内心动摇了.", selected_person_name))); 
																			
									}
									else
									{
										pk::message_box(pk::encode("额..完全被你说服了."), kunshu_);
										pk::message_box(pk::encode("我主待我恩重如山,我怎么能听信于你.你回去吧!"), selected_person_);
										pk::message_box(pk::encode("..."), kunshu_);
										pk::add_stat_exp(kunshu_, 2, exp_shao);
										pk::add_stat_exp(selected_person_, 2, exp_shao);
										pk::add_loyalty(selected_person_, zhong_bian * 3);
										
										kunshu_.update();

										pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的忠诚更加坚定了.", selected_person_name))); 
									}
									// 恢复武将原本的智力和健康程度
									selected_person_.base_stat[武将能力_智力] = 원본지력;
									selected_person_.shoubyou = shentizhuangtai;
									
									// 恢复口才
									selected_person_.wajutsu_kiben = 원본화술_wajutsu_kiben;
									selected_person_.wajutsu_mushi = 원본화술_wajutsu_mushi;
									selected_person_.wajutsu_chinsei = 원본화술_wajutsu_chinsei;
									
									selected_person_.update();
								}  //追加舌战
								
								else
								{
									pk::cutin(13); 
									pk::message_box(pk::encode("这下你该明白了吧?"), kunshu_);
									pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人所说的,\n容我考虑考虑再作答复.", kunshu_name)), selected_person_); 
									pk::message_box(pk::encode("那么我今天就先告辞了."), kunshu_);
									pk::add_stat_exp(kunshu_, 2, exp_shao);
									pk::add_stat_exp(selected_person_, 2, exp_shao);
									pk::add_loyalty(selected_person_, -zhong_bian);
										
									kunshu_.update();
									selected_person_.update();
										
									pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的内心有了些许动摇.", selected_person_name))); 
								}  //舌战胜
							}
							
							else
							{
								pk::message_box(pk::encode("额..完全被你说服了."), kunshu_);
								pk::message_box(pk::encode("我主待我恩重如山,我怎么能听信于你.你回去吧!"), selected_person_);
								pk::message_box(pk::encode("..."), kunshu_);
								pk::add_stat_exp(kunshu_, 2, exp_shao);
								pk::add_stat_exp(selected_person_, 2, exp_shao);
								pk::add_loyalty(selected_person_, int(zhong_bian * 1.5));
										
								kunshu_.update();
								selected_person_.update();
										
								pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的忠诚更加坚定了.", selected_person_name)));
							} //舌战负
						}
						else
						{
							switch (pk::rand(3))
							{
								case 0: pk::message_box(pk::encode("不必多费唇舌,\n我主待我恩重如山,我是绝对不会投降的!"), selected_person_);break;
								case 1: pk::message_box(pk::encode("请勿复言,我不是那种不忠不义之徒."), selected_person_); break;
								case 2: pk::message_box(pk::encode("忠臣不仕二主."), selected_person_); break;
							}
						}
											
					}  // 劝降
					
					else if (n == 1)
					{
						if (building_gold < gold_quanxiang)
						{
							pk::message_box(pk::encode("如今库中羞涩,这点东西恐怕诚意不足.\n先回吧!"), kunshu_);
							pk::fade(0);
							pk::background(-1);
							return;
						}
						pk::background("media/background/baifang/shangci02-1.png", 0);
						pk::play_se(16);
						pk::sleep(50);
						pk::background("media/background/baifang/shangci02-2.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/shangci02.png", 0);
						pk::sleep(1500);
						pk::background("media/background/baifang/shangci02-2.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/shangci02-1.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/jianyu01.png", 0);
						switch (pk::rand(3))
						{
							case 0: pk::message_box(pk::encode(pk::format("将军屈身事主却不被重用,\n这\x1b[1x{}\x1b[0x金是我的一点心意,\n请您收下.", gold_quanxiang)), kunshu_); break;
							case 1: pk::message_box(pk::encode("在下仰慕将军已久,\n区区的薄礼还请笑纳,\n请将军慎重考虑."), kunshu_); break;
							case 2: pk::message_box(pk::encode("将军是聪明之人,\n眼前的这些金银不成敬意,\n如果您投降,保证荣华富贵!"), kunshu_); break;
						}
						int j = pk::rand(100) + 1;			
						if (selected_person_.giri == 의리_매우낮음)
						{
							if (j <= 20)
							{
								pk::message_box(pk::encode(pk::format("（竟然给这么多...）多谢\x1b[1x{}\x1b[0x大人的厚爱.", kunshu_name)), selected_person_);
								pk::add_loyalty(selected_person_, -zhong_bian * 2);
							}
							else if (j > 40)
							{
								pk::message_box(pk::encode(pk::format("多谢\x1b[1x{}\x1b[0x大人的赏赐.", kunshu_name)), selected_person_);
								pk::add_loyalty(selected_person_, -zhong_bian);
							}
							else
							{
								pk::message_box(pk::encode("无功不受禄,请回...\n（咽...）"), selected_person_); 
							}
						} //义理非常低的人非常容易被打动
						
						else if (selected_person_.giri == 의리_낮음)
						{
							if (j <= 60)
							{
								pk::message_box(pk::encode(pk::format("多谢\x1b[1x{}\x1b[0x大人的赏赐.", kunshu_name)), selected_person_);
								pk::add_loyalty(selected_person_, -zhong_bian);
							}
							else
							{
								pk::message_box(pk::encode("无功不受禄,请回吧."), selected_person_); 
							}
						}  //义理低的人容易被打动
						
						else if (selected_person_.giri == 의리_보통)
						{
							
							if (j <= 35)
							{
								pk::message_box(pk::encode("多谢了."), selected_person_); 
								pk::add_loyalty(selected_person_, -zhong_bian);
							}
							else if (j > 95)
							{
								pk::message_box(pk::encode("别看我这样,我也是很重视名节的人."), selected_person_); 
								pk::add_loyalty(selected_person_, zhong_bian);
							}
							else
							{
								pk::message_box(pk::encode("无功不受禄,请回."), selected_person_); 
							}
						}  //义理中不太容易被打动
						
						else if (selected_person_.giri == 의리_높음)
						{
							if (j <= 20)
							{
								pk::message_box(pk::encode("甚好...甚好..."), selected_person_); 
								pk::add_loyalty(selected_person_, -zhong_bian);
							}
							else if (j > 90)
							{
								pk::cutin(13); 
								pk::message_box(pk::encode("区区一些钱币,就想换取我的忠心?给我滚!"), selected_person_); 
								pk::add_loyalty(selected_person_, zhong_bian);
							}
							else
							{
								pk::message_box(pk::encode("无功不受禄."), selected_person_); 
							}
						}  //义理高的人很难被打动
						else
						{
							if (j <= 10)
							{
								pk::message_box(pk::encode("如此...也..."), selected_person_); 
								pk::add_loyalty(selected_person_, -zhong_bian);
							}
							else if (j > 10 and j <= 30)
							{
								pk::cutin(13); 
								pk::message_box(pk::encode("我的忠义岂容你来玷污!"), selected_person_); 
								pk::add_loyalty(selected_person_, zhong_bian);
							}
							else if (j > 95)
							{
								pk::cutin(13); 
								pk::message_box(pk::encode("我的忠义岂容你来玷污!"), selected_person_); 
								pk::cutin(13); 
								pk::message_box(pk::encode("区区一些钱币,就想换取我的忠心?给我滚!"), selected_person_); 
								pk::add_loyalty(selected_person_, zhong_bian * 2);
							}
							else
							{
								pk::message_box(pk::encode("哼."), selected_person_); 
							}
						}  //义理非常高的人很有想法
						selected_person_.update();	
						pk::add_gold(building_, -gold_quanxiang, true);					
						building_.update();
						pk::message_box(pk::encode("劝诱俘虏花了些钱.")); 
						
					}  // 利诱
					
					else
					{
						//给卫兵改个名字
						string weibing_0_sei = weibing_0.sei;
						string weibing_0_mei = weibing_0.mei;	
						
						weibing_0.sei = pk::encode("狱");
						weibing_0.mei = pk::encode("卒");
						weibing_0.update();	
						switch (pk::rand(3))
						{
							case 0: 
								pk::message_box(pk::encode("不知好歹的家伙!\n再不投降,就让你吃尽苦头!\n狱卒,用刑!"), kunshu_);
								pk::message_box(pk::encode("是!"), weibing_0);
							break;
							case 1: 
								pk::message_box(pk::encode("不给你些厉害的瞧瞧,\n看来你是不会屈服的..\n来人,给我打到他屈服为止!"), kunshu_); 
								pk::message_box(pk::encode("遵命!\n（你可别怪我哈.）"), weibing_0);
							break;
							case 2: pk::message_box(pk::encode("让你与我为敌,\n来人啊,给我打,打得他皮开肉绽,哈哈..."), kunshu_); break;
						}
						pk::background("media/background/baifang/jianyu02-1.png", 0);
						pk::play_se(5);
						pk::sleep(50);
						pk::background("media/background/baifang/jianyu02-2.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/jianyu02.png", 0);
						pk::sleep(1500);
						pk::background("media/background/baifang/jianyu02-2.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/jianyu02-1.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/jianyu01.png", 0);
						pk::sleep(1000);
						pk::background("media/background/baifang/jianyu02-1.png", 0);
						pk::play_se(5);
						pk::sleep(50);
						pk::background("media/background/baifang/jianyu02-2.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/jianyu02.png", 0);
						pk::sleep(1500);
						pk::background("media/background/baifang/jianyu02-2.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/jianyu02-1.png", 0);
						pk::sleep(50);
						pk::background("media/background/baifang/jianyu01.png", 0);
						int i = 0;

						while(i < 3)
						{
							if (selected_person_.shoubyou == i)
							{
								selected_person_.shoubyou = i + 1;
								selected_person_.update();
								break;
							}
							i++;
						}
						int j = pk::rand(100) + 1;
						if (selected_person_.character == 0)
						{
							if (j <= 80)
							{
								pk::add_loyalty(selected_person_, -zhong_bian);
							}
							else if (j >90)
							{
								pk::add_loyalty(selected_person_, zhong_bian);
							}
						} //胆小性格害怕酷刑
						else if (selected_person_.character == 1)
						{
							random_n = pk::rand(3);
							//pk::trace(pk::format("random_n等于{}", random_n));
							if (random_n == 0)	
							{
								pk::add_loyalty(selected_person_, -zhong_bian);
							}
							else if (random_n == 1)	
							{
								pk::add_loyalty(selected_person_, zhong_bian);
							}
						}  //冷静性格会分析利弊
						else if (selected_person_.character == 2)
						{
							if (j <= 80)
							{
								pk::add_loyalty(selected_person_, zhong_bian);
							}
							else if (j > 90)
							{
								pk::add_loyalty(selected_person_, -zhong_bian);
							}
						}  //大胆性格与胆小性格相反
						else
						{
							int rd_a = ch::randint(-2, 2);
							pk::add_loyalty(selected_person_, rd_a);
						}  //莽撞性格反复无常
						selected_person_.update();
						
						switch (pk::rand(3))
						{
							case 0: pk::message_box(pk::encode("狗贼,\n今日的仇,我必定会报的!"), selected_person_); break;
							case 1: pk::message_box(pk::encode(pk::format("啊啊啊!!\n\x1b[1x{}\x1b[0x匹夫!!啊......\n（已无力言语......）", kunshu_name)), selected_person_);  break;
							case 2: pk::message_box(pk::encode("老贼,你不会有好下场的!!\n啊啊!!...."), selected_person_); break;
						}
						
						pk::message_box(pk::encode(pk::format("报告,\x1b[1x{}\x1b[0x已经昏死过去了.", selected_person_name)), weibing_0); 
						
						//卫兵名字重置
						weibing_0.sei = weibing_0_sei;
						weibing_0.mei = weibing_0_mei;
						weibing_0.update();
							
						if (building_.get_id() < 建筑_城市末)  //为城市时
							pk::add_public_order(kunshu_city, -order_bian, true);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x军残暴的行径,城里的百姓感到十分不安.", kunshu_name))); 
						switch (pk::rand(3))
						{
							case 0: pk::say(pk::encode("\x1b[29x月将升,日将没.\n反乎民,不得活.\x1b[0x"), boy, kunshu_city); break;
							case 1: pk::say(pk::encode("\x1b[29x君非君,臣非臣.\n衣冠楚楚是豺狼.\x1b[0x"), boy, kunshu_city);  break;
							case 2: pk::say(pk::encode("\x1b[29x为人君,不知仁.\x1b[0x"), boy, kunshu_city); break;
						}
						
						
					}  // 用刑
				}
			}  //拜访俘虏
			
			
			else if (!pk::is_fuufu(selected_person_, kunshu_0))
			{
				pk::fade(0);
				pk::background(5); 
				pk::fade(255);

				//呼吸
				// array<string> wangyi_huxi = {/*王异0*/"background/baifang/WangYi0.png", /*王异渐变1*/"background/baifang/WangYi1.png", /*王异渐变2*/"background/baifang/WangYi2.png", /*王异渐变3*/"background/baifang/WangYi3.png", /*王异渐变4*/"background/baifang/WangYi4.png", /*王异渐变5*/"background/baifang/WangYi5.png", /*王异6*/"background/baifang/WangYi6.png"};
				
				/* for (int xunhuan = 0; xunhuan < 3; xunhuan++)
				{
					for (int picA = 0; picA < 7; picA++)
					{
						pk::background(wangyi_huxi[picA]);
						pk::sleep(400);
					}
					for (int picB = 6; picB > -1; picB--)
					{
						pk::background(wangyi_huxi[picB]);
						pk::sleep(400);
					}
				}  */
				//呼吸end
				
				//自动切换说话者头像
				//string bg_select_person = "background/baifang/" + selected_person_0 + ".png" ;
				//string bg_select_person = "background/baifang/" + "WangYi0" + ".png" ;
				//string bg_kunshu = "background/baifang/" + kunshu_0 + ".png" ;
				//pk::background(bg_kunshu);
				//pk::background(bg_select_person);
				
				//自动切换说话者头像end
				
				//称呼变化
				scene_callname();

				array<string> answers =
				{
					pk::encode("  谈话  "),
					pk::encode("  讨论  "),
					pk::encode("  外出  "),
					pk::encode("  师事  "),
					pk::encode("  探病  "),
					pk::encode("  其他  ")
				};

				int n = pk::choose(pk::encode("选择要谈论的话题."), answers);
				
				if (n == 0)  //谈话
				{
					if (pk::rand_bool(50))    //亲爱事件
					{
						if ( (pk::is_like(selected_person_, kunshu_0))    
							and (kunshu_.gikyoudai == -1) 
							and (selected_person_.gikyoudai == -1) 
							and !pk::is_ketsuen(selected_person_, kunshu_0))    //初定，已喜爱的直接触发结义事件
						{
							pk::message_box(pk::encode("主公,我有一事想说于你听.."), selected_person_);
							pk::message_box(pk::encode("主公你我志趣相投."), selected_person_);
							pk::message_box(pk::encode("和我结为异姓兄弟如何?"), selected_person_);
							pk::message_box(pk::encode("这.."), kunshu_);
						
							array<string> jieyi_answers1 =
							{
								pk::encode("正有此意.    "),
								pk::encode("请容我再想想.")
							};
						
							int jieyi_answers = pk::choose(pk::encode("要结为义兄弟吗?"), jieyi_answers1);
						
							if (jieyi_answers == 0)
							{
								pk::message_box(pk::encode("其实,我也正有此意."), kunshu_);
								pk::message_box(pk::encode("好,主公果然和我想到一块去了."), selected_person_);
							
								pk::fade(0);
								pk::background(-1);
							
								pk::background(42); 
								pk::fade(255);
							
								pk::message_box(pk::encode("以后你就是我兄弟了."), kunshu_);
								pk::message_box(pk::encode("能成为主公的兄弟,我很荣幸."), selected_person_);
								
								if (kunshu_.birth <= selected_person_.birth)  //君主大
								{
									kunshu_.gikyoudai = kunshu_0;
									selected_person_.gikyoudai = kunshu_0;
								}
								else  //君主小
								{
									kunshu_.gikyoudai = selected_person_0;
									selected_person_.gikyoudai = selected_person_0;
								}
								//kunshu_.gikyoudai = selected_person_.get_id();
								//selected_person_.gikyoudai = kunshu_.get_id();
							
								kunshu_.update();
								selected_person_.update();
								pk::play_se(10);
							
								pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x结为义兄弟了.", kunshu_name, selected_person_name))); 
								pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x结为义兄弟了.", kunshu_name, selected_person_name)));
							}
							else
							{
								pk::message_box(pk::encode("对不起,请容我再考虑下."), kunshu_);
								pk::message_box(pk::encode("好,那你以后有了决定再说."), selected_person_);
								pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x前往拜访了\x1b[1x{}\x1b[0x.", kunshu_name, selected_person_name)));
							}

						}
						
						else if (pk::is_like(selected_person_, kunshu_0) || pk::is_gikyoudai(kunshu_, selected_person_0))   //亲爱重复和结兄弟，初定语句还可以优化
						{
							switch (pk::rand(2))
							{
								case 0:
									pk::message_box(pk::encode(pk::format("我这正好有一坛美酒,\x1b[1x{}\x1b[0x要不要尝一下.",call_selected_person)), kunshu_);
									pk::message_box(pk::encode(pk::format("哈哈哈,还是{}明白{}.", call_kunshu, self_selected_person)), selected_person_);
									pk::message_box(pk::encode("我正想喝一杯.."), selected_person_);
									pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x一起喝了酒.", kunshu_name, selected_person_name))); 
								break;
								case 1:
									pk::message_box(pk::encode(pk::format("{},现已是日中,留下来一起吃个便饭吧.", call_kunshu)), selected_person_);
									pk::message_box(pk::encode("也好,那我就恭敬不如从命了.哈哈."), kunshu_);
								break;
							}
							if (haveNotfound (building_))  //提供情报1，城中有未发现武将
							{
								pk::message_box(pk::encode(pk::format("说起来,最近此地来了一位有名的人才.\n{}何不求之?", call_kunshu)), selected_person_);
								pk::message_box(pk::encode("喔,\n竟还有这样的人存在啊!"), kunshu_);
							}
							pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x前往拜访了\x1b[1x{}\x1b[0x.", kunshu_name, selected_person_name)));
						}
						
						else
						{
							bool likedscene = false;  //判定武将是否有亲爱空位
							for (int i = 0; i < 5; i++)
							{
								if (selected_person_.liked[i] == -1)
								{
									likedscene = true;
									break;
								}
							}
							if (pk::rand_bool(친애무장_확률) && likedscene)
							{
								pk::fade(255);
								switch (pk::rand(4))
								{
									case 0:pk::message_box(pk::encode("最近还好吗?"), kunshu_); break;
									case 1:pk::message_box(pk::encode("你还好吗?"), kunshu_); break;
									case 2:pk::message_box(pk::encode("哎呀呀,真是稀客呀."), selected_person_); break;
									case 3:pk::message_box(pk::encode("您来啦."), selected_person_); break;
								}
								random_m = pk::rand(4);
								if (random_m == 0)   //单挑事件
								{								
									pk::cutin(17);
									pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,和我切磋一下武艺吧?", call_kunshu)), selected_person_);
									pk::message_box(pk::encode("恩,很有趣的样子."), kunshu_);
									
									duel_result = pk::duel( null, null, kunshu_, null, null, selected_person_, null, null, pk::is_player_controlled(kunshu_), false, 0, true).first;
									
									if (duel_result == 0)
									{										
										pk::message_box(pk::encode(pk::format("果然,还是无法企及{}的武艺呀.", call_kunshu)), selected_person_);
										pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的武艺也不错.\n好久没有那么畅快的单挑了.", call_selected_person)), kunshu_);
										pk::message_box(pk::encode(pk::format("那么,下次还能再找{}挑战吗?", call_kunshu)), selected_person_);
										pk::message_box(pk::encode(pk::format("当然可以,求之不得呢!\n总觉得与{}有一种说不出的亲近感.", call_selected_person)), kunshu_);
										pk::cutin(20); 
										pk::message_box(pk::encode("我也有同样的感觉,能遇到您真是三生有幸呀!"), selected_person_);
										scene_qinai();
										pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x成为了\x1b[1x{}\x1b[0x的亲爱武将.", kunshu_name, selected_person_name)));
									}
									else
									{
										pk::message_box(pk::encode("恩..我输了."), kunshu_);
										pk::message_box(pk::encode(pk::format("这次是{}运气好才赢的.", self_selected_person)), selected_person_);
										pk::message_box(pk::encode(pk::format("别谦逊了.{}的武艺超群.", call_selected_person)), kunshu_);
										pk::message_box(pk::encode("以后再来挑战."), kunshu_);
										pk::message_box(pk::encode("哈哈哈,知道了."), selected_person_);
									}
								}
								else if (random_m == 1)    //舌战事件
								{
									pk::cutin(17);
									pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,和我讨论一番如何?", call_kunshu)), selected_person_);
									pk::message_box(pk::encode("恩,很有趣的样子."), kunshu_);
									debate_result = pk::debate(kunshu_, selected_person_, pk::is_player_controlled(kunshu_), false, false, true).first;
						
									if (debate_result == 0)
									{
										pk::message_box(pk::encode(pk::format("看来还是{}说得比较有道理.", call_kunshu)), selected_person_);
										pk::message_box(pk::encode(pk::format("对局势的看法,{}还是有很多不足呀.", self_selected_person)), selected_person_);
										pk::message_box(pk::encode("不,其实你的意见也有可取之处,\n很好地填补了这里的不足之处."), kunshu_);
										pk::message_box(pk::encode(pk::format("那么,以后还能再找{}讨论其他的吗?", call_kunshu)), selected_person_);
										pk::message_box(pk::encode(pk::format("当然可以,求之不得呢!\n总觉得与{}有一种说不出的亲近感.", call_selected_person)), kunshu_);
										pk::cutin(20); 
										pk::message_box(pk::encode("我也有同样的感觉,能遇到您真是三生有幸呀!"), selected_person_);
										scene_qinai();
										pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x成为了\x1b[1x{}\x1b[0x的亲爱武将.", kunshu_name, selected_person_name)));
									}
									else
									{
										pk::message_box(pk::encode("额..完全被你说服了."), kunshu_);
										pk::message_box(pk::encode(pk::format("其实{}的意见也不无道理.", call_kunshu)), selected_person_);
										pk::message_box(pk::encode("时间不早了,该回去了."), kunshu_);
										pk::message_box(pk::encode("好的,请慢走."), selected_person_);
									}
								}
								else if (random_m == 2)     //救济灾民事件
								{
									pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x怎么一脸愁容呢?", call_selected_person)), kunshu_);
									pk::message_box(pk::encode(pk::format("{}可知附近城外有群流离失所的灾民?\n他们是被此次洪灾冲毁了家园的人.\n{}实在很想帮助他们.", call_kunshu, self_selected_person)), selected_person_);
									pk::message_box(pk::encode("（多么高尚的品格...）\n有什么需要帮助的,尽管开口,我一定会全力相助."), kunshu_);
									pk::message_box(pk::encode("如此真是太感谢了!\n当务之急是解决衣食问题,\n然后重建家园...这般这般..."), selected_person_);
									pk::message_box(pk::encode("可是附近有一伙相当棘手的盗贼,\n四处劫杀百姓,\n就连刚从异地筹集的物资也被劫走了."), selected_person_);
									
									pk::cutin(13); 
									pk::message_box(pk::encode("什么!!\n连灾民的物资都劫,不可原谅!"), kunshu_);
									pk::cutin(20); 
									pk::message_box(pk::encode(pk::format("{},敢不敢与我同去消灭这伙恶贼呢?", call_kunshu)), selected_person_);
									pk::message_box(pk::encode("我也正有此意!马上出发!"), kunshu_);
									
									//给盗贼改个名字
									string bandit_0_sei = bandit_0.sei;
									string bandit_0_mei = bandit_0.mei;
									string bandit_1_sei = bandit_1.sei;
									string bandit_1_mei = bandit_1.mei;
									string bandit_2_sei = bandit_2.sei;
									string bandit_2_mei = bandit_2.mei;
									
									bandit_0.sei = pk::encode("贼");
									bandit_1.sei = pk::encode("贼");
									bandit_2.sei = pk::encode("贼");
									bandit_0.mei = pk::encode("哨兵");
									bandit_1.mei = pk::encode("首领");
									bandit_2.mei = pk::encode("头目");
									bandit_0.update();
									bandit_1.update();
									bandit_2.update();
									
									
									pk::fade(0);
									pk::background("media/background/baifang/shanzhai00.png", 0); 
									pk::fade(255);
									pk::message_box(pk::encode("（惊!!）\n你们是什么人?!"), bandit_0);
									pk::cutin(15);
									pk::message_box(pk::encode("有两个人杀入寨里了...快报告...\n啊啊..."), bandit_0);
									pk::cutin(15);
									pk::message_box(pk::encode("那几个应该就是最后的头目了,\n快消灭他们夺回物资!"), selected_person_);
									
									pk::cutin(19); 
									duel_result = pk::duel( null, null, kunshu_, selected_person_, null, bandit_2, bandit_2, bandit_1, pk::is_player_controlled(kunshu_), false, 2, true).first;
									pk::background("media/background/baifang/shanzhai00.png", 0); 
									if (duel_result == 0)
									{
										pk::message_box(pk::encode("额额...饶命啊...\n我们也是逃难到此,无奈才落草为寇的."), bandit_1);
										pk::cutin(20);
										pk::message_box(pk::encode("今后再不可为非作歹,否则绝不轻饶!"), selected_person_);
										pk::fade(0);
										pk::background(3); 
										pk::fade(255);
										pk::message_box(pk::encode("终于夺回了物资,这下灾民们就有救了."), kunshu_);
										pk::message_box(pk::encode(pk::format("真是太感谢{}了!", call_kunshu)), selected_person_);
										pk::message_box(pk::encode(pk::format("不不,{}这样仁义的人,\n才值得别人感谢和尊敬呀.", call_selected_person)), kunshu_);
										pk::cutin(20); 
										pk::message_box(pk::encode("您真是位仁德谦逊的好人,真是百姓之福气."), selected_person_);
										scene_qinai();
										
										switch (pk::rand(3))
										{
											case 0: pk::say(pk::encode("得救了,有吃的了."), oldman, kunshu_city); break;
											case 1: pk::say(pk::encode(pk::format("{}大人真是大好人啊.", kunshu_name)), farmer_0, kunshu_city);  break;
											case 2: pk::say(pk::encode("总算等来了物资了."), oldwoman, kunshu_city); break;
										}
										pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x成为了\x1b[1x{}\x1b[0x的亲爱武将.", kunshu_name, selected_person_name)));
									}
									else
									{
										pk::message_box(pk::encode("额..我竟然败给了区区贼人.\n只好先杀出去了."), kunshu_);
										pk::cutin(17);
										pk::message_box(pk::encode("小的们,给我追!休让他们走了!"), bandit_2);
										pk::message_box(pk::encode("他们人多势众,只好先回去,再从长计议."), selected_person_);
										pk::message_box(pk::encode("可惜灾民的物资没能夺回来..."), kunshu_);
										pk::message_box(pk::encode("（长叹...）"), selected_person_);
										switch (pk::rand(3))
										{
											case 0: pk::say(pk::encode("明天的太阳啊,何时升起.."), oldman, kunshu_city); break;
											case 1: pk::say(pk::encode(pk::format("连{}大人也救不了我们吗..", kunshu_name)), oldwoman, kunshu_city);  break;
											case 2: pk::say(pk::encode("母亲!!你不要丢下儿去啊.."), farmer_0, kunshu_city); break;
										}
									}
									//重置绑匪原本名字
									bandit_0.sei = bandit_0_sei;
									bandit_0.mei = bandit_0_mei;
									bandit_1.sei = bandit_1_sei;
									bandit_1.mei = bandit_1_mei;
									bandit_2.sei = bandit_2_sei;
									bandit_2.mei = bandit_2_mei;
									bandit_0.update();
									bandit_1.update();
									bandit_2.update();
								}
								else     //猜猜猜事件
								{
									pk::cutin(17);
									pk::message_box(pk::encode(pk::format("{},和玩个游戏如何?", call_kunshu)), selected_person_);
									pk::message_box(pk::encode("哦?很有趣的样子."), kunshu_);

									pk::message_box(pk::encode(pk::format("那么{}就出题了.\n第一问,请问我的性格是哪一种呢?", self_selected_person)), selected_person_);
									array<string> answers_1 =
									{
										pk::encode("胆小"),
										pk::encode("冷静"),
										pk::encode("刚胆"),
										pk::encode("莽撞")
									};							
									int answer_1 = pk::choose(pk::encode("请选择."), answers_1);
									
									pk::message_box(pk::encode("第二问,请问我的义理是哪一种呢?"), selected_person_);
									array<string> answers_2 =
									{
										pk::encode("低  "),
										pk::encode("稍低"),
										pk::encode("普通"),
										pk::encode("稍高"),
										pk::encode("高  ")
									};	
									int answer_2 = pk::choose(pk::encode("请选择."), answers_2);
									
									pk::message_box(pk::encode("第三问,请问我对汉室的态度是哪一种呢?"), selected_person_);
									array<string> answers_3 =
									{
										pk::encode("无视"),
										pk::encode("普通"),
										pk::encode("重视")
									};	
									int answer_3 = pk::choose(pk::encode("请选择."), answers_3);
									
									if (answer_1 == selected_person_.character and answer_2 == selected_person_.giri and answer_3 == selected_person_.kanshitsu)
									{
										pk::message_box(pk::encode("!!......"), selected_person_);
										pk::message_box(pk::encode("怎么样,我说的对吗?"), kunshu_);
										pk::message_box(pk::encode(pk::format("竟然全都答对了,\n惊讶之余又有种莫名的感动.\n没想到{}对{}竟这样了如指掌.", call_kunshu, self_selected_person)), selected_person_);
										pk::message_box(pk::encode("哈哈...\n\x1b[1x知人善任\x1b[0x,是身为人君之本啊."), kunshu_);
										pk::cutin(20); 
										pk::message_box(pk::encode(pk::format("能遇到您这样的{},在阵前赴汤蹈火也不能报答呀!", call_kunshu)), selected_person_);
										scene_qinai();
										pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x成为了\x1b[1x{}\x1b[0x的亲爱武将.", kunshu_name, selected_person_name)));
									}
									else
									{
										pk::message_box(pk::encode(pk::format("答错了...\n看来{}对我的了解还不够呀...", call_kunshu)), selected_person_);
										pk::message_box(pk::encode("是嘛..."), kunshu_);
									}
								}
							}
							
							else
							{
								pk::message_box(pk::encode(pk::format("{},我有个难言之隐..", call_kunshu)), selected_person_);
								pk::message_box(pk::encode("好,你说说看是什么事情."), kunshu_);
								pk::message_box(pk::encode(pk::format("和{}聊天心情变好了.", call_kunshu)), selected_person_);
								pk::message_box(pk::encode("你如果有什么苦衷,尽管和我说."), kunshu_);
								pk::message_box(pk::encode(pk::format("是,{}.", call_kunshu)), selected_person_);
								pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x前往拜访了\x1b[1x{}\x1b[0x.", kunshu_name, selected_person_name)));
							}
						}
					} //亲爱事件

					
					else    //劝解事件
					{
						string dislike_name = "他"; //称呼初始化
						bool havedislike = false; //有厌恶武将初始化
						int i = 0;
						while(i < 5)
						{
							if (selected_person_.disliked[i] != -1)
							{
								bool alldislike = false;  //都厌恶初始化
								for (int ii = 0; ii < 5; ii++)
								{
									if (kunshu_.disliked[ii] == selected_person_.disliked[i])
									{
										alldislike = true;
										break;
									}
								}
								if (!alldislike)  //只劝君主不厌恶的
								{
									havedislike = true;
									dislike_name = pk::decode(pk::get_name(pk::get_person(selected_person_.disliked[i])));
									break;
								}
							}
							i++;
						}
						
						pk::message_box(pk::encode(pk::format("{},{}这有一坛美酒,要不要尝尝?", call_kunshu, self_selected_person)), selected_person_);
						pk::message_box(pk::encode("好!光闻着就很香."), kunshu_);
						if (!havedislike)
						{
							pk::message_box(pk::encode("那我们就不醉不归了."), selected_person_);
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x一起喝了酒.", kunshu_name, selected_person_name)));
							pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x前往拜访了\x1b[1x{}\x1b[0x.", kunshu_name, selected_person_name)));
						}
						else
						{
							pk::message_box(pk::encode(pk::format("说起来..最近和\x1b[1x{}\x1b[0x关系还是不好嘛?", dislike_name)), kunshu_);
							pk::message_box(pk::encode("我和他性格迥异.."), selected_person_);
							pk::message_box(pk::encode("你别怪我说这些.."), kunshu_);
							switch (pk::rand(3))
							{
								case 0: pk::message_box(pk::encode(pk::format("关于那件事,\x1b[1x{}\x1b[0x不是有意的,\n我所知的事情经过是如此这般..", dislike_name)), kunshu_); break;
								case 1: pk::message_box(pk::encode(pk::format("你也许误会\x1b[1x{}\x1b[0x的意思了,\n他当时其实想说的是那样那样...", dislike_name)), kunshu_); break;
								case 2: pk::message_box(pk::encode(pk::format("关于\x1b[1x{}\x1b[0x的那件事,\n与其一直记恨,不如放下彼此成见.\n不失为英雄之气度?", dislike_name)), kunshu_); break;
							}
							

							if (pk::rand_bool(혐오무장제거_확률))
							{
								
								pk::message_box(pk::encode("原来是这样啊,\n看来我确实也有误会的地方."), selected_person_);
								pk::message_box(pk::encode(pk::format("那今后就和\x1b[1x{}\x1b[0x好好相处.", dislike_name)), kunshu_);
								pk::message_box(pk::encode(pk::format("是,{}.", call_kunshu)), selected_person_);

								selected_person_.disliked[i] = -1;
								selected_person_.update();

								pk::message_box(pk::encode(pk::format("经过劝解,\x1b[1x{}\x1b[0x从此不再厌恶\x1b[1x{}\x1b[0x了.", selected_person_name, dislike_name))); 
								pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x劝解了\x1b[1x{}\x1b[0x的误会.", kunshu_name, selected_person_name)));
							}
							else
							{
								pk::message_box(pk::encode(pk::format("{}就不要替他说话了,\n{}确实没法和他交往.", call_kunshu, self_selected_person)), selected_person_);
								pk::message_box(pk::encode("既然你这么决定,我也不好相劝."), kunshu_);
								pk::message_box(pk::encode(pk::format("谢谢你的理解,{}.", call_kunshu)), selected_person_);
							}
						}
						

					} // 好言相劝
					
				} // 谈话

				
				else if (n == 1)  //讨论
				{
					array<string> taolun_list1 =   
					{
						pk::encode("武艺相关的谈话.    "),
						pk::encode("天下大势相关的谈话."),
						pk::encode("兵法探讨的谈话.    ")
					};
					int taolun_list = pk::choose(pk::encode("选择要谈论的话题."), taolun_list1);
					
					if (taolun_list == 0)   //武艺相关的谈话
					{
						pk::message_box(pk::encode(pk::format("我是来和\x1b[1x{}\x1b[0x谈论武艺的.", call_selected_person)), kunshu_);
						pk::message_box(pk::encode(pk::format("{}认为最近最有英名的人是谁?", call_selected_person)), kunshu_);
						pk::message_box(pk::encode("最近么..."), selected_person_);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x与\x1b[1x{}\x1b[0x进行了长时间的交谈.", kunshu_name, selected_person_name))); 
						
						
						if (pk::rand_bool(일기토발생률))
						{
							pk::message_box(pk::encode("哈哈哈,似乎进行了很好的交流."), kunshu_);
							pk::message_box(pk::encode(pk::format("{},和{}切磋一下武艺吧?", call_kunshu, self_selected_person)), selected_person_);
							pk::cutin(17); 
							pk::message_box(pk::encode("恩,很有趣的样子."), kunshu_);
							
							pk::unit@ dummy_unit;
							duel_result = pk::duel(dummy_unit, dummy_unit, kunshu_, null, null, selected_person_, null, null, pk::is_player_controlled(kunshu_), false, 0, true).first;
							
							if (duel_result == 0)
							{
								pk::message_box(pk::encode(pk::format("果然,还是无法企及{}的武艺呀.", call_kunshu)), selected_person_);
								pk::message_box(pk::encode(pk::format("{}的武艺也不错.", call_selected_person)), kunshu_);
								pk::message_box(pk::encode("再接再厉!下次再来挑战吧."), selected_person_);
								pk::message_box(pk::encode("很好.那么回头见."), kunshu_);
							}
							else
							{
								pk::message_box(pk::encode("恩..我输了."), kunshu_);
								pk::message_box(pk::encode(pk::format("这次是{}运气好才赢的.", self_selected_person)), selected_person_);
								pk::message_box(pk::encode(pk::format("别谦逊了.{}的武艺超群.", call_selected_person)), kunshu_);
								pk::message_box(pk::encode("以后再来挑战."), kunshu_);
								pk::message_box(pk::encode("哈哈哈,知道了."), selected_person_);
							}
							pk::add_stat_exp(kunshu_, 1, exp_무력);
							pk::add_stat_exp(selected_person_, 1, exp_무력);
							
							kunshu_.update();
							selected_person_.update();
							
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x的武力经验上升了.", kunshu_name, selected_person_name))); 
						}
						else 
						{
							pk::message_box(pk::encode("哈哈哈,很好的交流."), kunshu_);
							pk::message_box(pk::encode("时候不早了,该回去了."), kunshu_);
							pk::message_box(pk::encode(pk::format("好的,{}请慢走.", call_kunshu)), selected_person_);
						}
						pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x与\x1b[1x{}\x1b[0x切磋了武艺.", kunshu_name, selected_person_name)));
					}
					
					else if (taolun_list == 1)   //天下大势相关的谈话
					{
						pk::message_box(pk::encode(pk::format("此来是想和\x1b[1x{}\x1b[0x谈论下天下大势.", call_selected_person)), kunshu_);
						pk::message_box(pk::encode("君观而今天下如何?"), kunshu_);
						pk::message_box(pk::encode("如今天下..."), selected_person_);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x就天下大势谈论了许久...", kunshu_name, selected_person_name))); 
						
						if (pk::rand_bool(설전발생률))
						{
							pk::message_box(pk::encode("这事我是这样认为的..."), kunshu_);
							pk::message_box(pk::encode(pk::format("{}不这样想.", self_selected_person)), selected_person_);
							pk::cutin(17); 
							pk::message_box(pk::encode("何以见得?"), kunshu_);

							debate_result = pk::debate(kunshu_, selected_person_, pk::is_player_controlled(kunshu_), false, false, true).first;
							
							if (debate_result == 0)
							{
								pk::message_box(pk::encode(pk::format("看来还是{}比较有道理.", call_kunshu)), selected_person_);
								pk::message_box(pk::encode(pk::format("对天下大势的见解{}还是有很多不足呀.", self_selected_person)), selected_person_);
								pk::message_box(pk::encode("不,其实你的意见也有可取之处."), kunshu_);
								pk::message_box(pk::encode("时间不早了,该回去了."), kunshu_);
								pk::message_box(pk::encode(pk::format("好的,{}慢走.", call_kunshu)), selected_person_);
							}
							else
							{
								pk::message_box(pk::encode(pk::format("额..完全被{}说服了.", call_selected_person)), kunshu_);
								pk::message_box(pk::encode(pk::format("其实{}的意见也不无道理.", call_kunshu)), selected_person_);
								pk::message_box(pk::encode("时间不早了,该回去了."), kunshu_);
								pk::message_box(pk::encode(pk::format("好的,{}慢走.", call_kunshu)), selected_person_);
							}
							
							pk::add_stat_exp(kunshu_, 2, exp_지력);
							pk::add_stat_exp(selected_person_, 2, exp_지력);
							
							kunshu_.update();
							selected_person_.update();
							
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x的智力经验上升了.", kunshu_name, selected_person_name))); 
							
						}
						else 
						{
							pk::message_box(pk::encode("这回谈的很尽兴."), kunshu_);
							pk::message_box(pk::encode("时间不早了,该回去了."), kunshu_);
							pk::message_box(pk::encode(pk::format("好的,{}慢走.", call_kunshu)), selected_person_);
						}
						pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x与\x1b[1x{}\x1b[0x讨论了天下大势.", kunshu_name, selected_person_name)));
		
					}
					

					else   //兵法探讨的谈话
					{
						pk::message_box(pk::encode(pk::format("此来是想和\x1b[1x{}\x1b[0x探讨下兵法.", call_selected_person)), kunshu_);
						pk::message_box(pk::encode("君观此局势当如何破解?"), kunshu_);
						pk::message_box(pk::encode("此时应当用..."), selected_person_);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x就兵法演绎了许久...", kunshu_name, selected_person_name))); 
						
						if (pk::rand_bool(설전발생률))
						{
							pk::message_box(pk::encode("此时当\x1b[1x趁其立足未稳,主动出击,攻其不备.\x1b[0x"), kunshu_);
							pk::message_box(pk::encode(pk::format("{}不这样想,当\x1b[1x加强防守,以逸待劳,以静制动.\x1b[0x", self_selected_person)), selected_person_);
							pk::cutin(17); 
							pk::message_box(pk::encode("何以见得?"), kunshu_);

							debate_result = pk::debate(kunshu_, selected_person_, pk::is_player_controlled(kunshu_), false, false, true).first;
							
							if (debate_result == 0)
							{
								pk::message_box(pk::encode(pk::format("看来还是{}比较有道理.", call_kunshu)), selected_person_);
								pk::message_box(pk::encode(pk::format("对兵法的见解,{}还是有很多不足呀.", self_selected_person)), selected_person_);
								pk::message_box(pk::encode("不,其实你的意见也有可取之处."), kunshu_);
								pk::message_box(pk::encode("时间不早了,该回去了."), kunshu_);
								pk::message_box(pk::encode(pk::format("好的,{}慢走.", call_kunshu)), selected_person_);
								
								pk::add_stat_exp(kunshu_, 0, exp_tong);
								pk::add_stat_exp(selected_person_, 0, exp_shao);
							}
							else
							{
								pk::message_box(pk::encode(pk::format("额..完全被{}说服了.", call_selected_person)), kunshu_);
								pk::message_box(pk::encode(pk::format("其实{}的意见也不无道理.", call_kunshu)), selected_person_);
								pk::message_box(pk::encode("时间不早了,该回去了."), kunshu_);
								pk::message_box(pk::encode(pk::format("好的,{}慢走.", call_kunshu)), selected_person_);
								
								pk::add_stat_exp(kunshu_, 0, exp_shao);
								pk::add_stat_exp(selected_person_, 0, exp_tong);
							}
													
							kunshu_.update();
							selected_person_.update();
							
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x的统帅经验上升了.", kunshu_name, selected_person_name))); 
							
						}
						else 
						{
							pk::message_box(pk::encode("这回谈的很尽兴."), kunshu_);
							pk::message_box(pk::encode("时间不早了,该回去了."), kunshu_);
							pk::message_box(pk::encode(pk::format("好的,{}慢走.", call_kunshu)), selected_person_);
						}
						pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x与\x1b[1x{}\x1b[0x讨论了兵法.", kunshu_name, selected_person_name)));
		
					} //兵法探讨
				}  //讨论


				else if (n == 2)   //外出
				{
					pk::message_box(pk::encode("与我同去街上走走如何?"), kunshu_);
					pk::message_box(pk::encode("也好,今天的天气还不错."), selected_person_);
					pk::fade(0);
					pk::background(4); 
					pk::fade(255);
					pk::message_box(pk::encode("街上到处都是一片祥和啊."), selected_person_);
					
					array<string> waichu_list1;
					//君主所属为城市,未交易，且有钱
					if (building_.get_id() < 建筑_城市末)
					{
						if (kunshu_city.merchant_done == false && building_gold >= 500)
						{
							waichu_list1 =
							{
								pk::encode("随意逛逛.  "),
								pk::encode("马匹商人处.")
								// pk::encode("衣服商人处."),
							};
						}
						else
						{
							waichu_list1 =
							{
								pk::encode("随意逛逛.  ")
							};
						}
					}
					else
					{
						waichu_list1 =
						{
							pk::encode("随意逛逛.  ")
						};
					}
					int waichu_list = pk::choose(pk::encode("要上哪儿去呢?"), waichu_list1);
					
					if (waichu_list == 0)
					{
						random_n = pk::rand(5);
						//测试random_n = 3;
						if (random_n == 0)	
						{
							pk::fade(255);
							pk::background(39); 
							pk::cutin(20);
							pk::message_box(pk::encode("看,前面发生了什么?"), selected_person_);
							switch (pk::rand(3))
							{
								case 0: pk::message_box(pk::encode("这位大人,我的宝贝女儿...被贼人给绑架了!"), shangren_0);break;
								case 1: pk::message_box(pk::encode("这位大人,我的小孙女儿...被贼人给绑架了!"), oldman);break;
								case 2: pk::message_box(pk::encode("哪位好心人救救我的孙女儿吧...她被贼人给绑架了!"), oldwoman);break;
							}
							pk::cutin(13); 
							pk::message_box(pk::encode("竟有这种事!!我等这就前去讨伐."), kunshu_);
							
							//给绑匪改个名字
							string bandit_0_sei = bandit_0.sei;
							string bandit_0_mei = bandit_0.mei;
							string bandit_1_sei = bandit_1.sei;
							string bandit_1_mei = bandit_1.mei;
							string bandit_2_sei = bandit_2.sei;
							string bandit_2_mei = bandit_2.mei;
							string echen_sei = echen.sei;
							string echen_mei = echen.mei;
							
							bandit_0.sei = pk::encode("绑匪");
							bandit_1.sei = pk::encode("绑匪");
							bandit_2.sei = pk::encode("绑匪");
							echen.sei = pk::encode("神秘");
							bandit_0.mei = pk::encode("乙");
							bandit_1.mei = pk::encode("甲");
							bandit_2.mei = pk::encode("丙");
							echen.mei = pk::encode("人");
							bandit_0.update();
							bandit_1.update();
							bandit_2.update();
							echen.update();
							

							pk::fade(0);
							pk::background(3); 
							pk::fade(255);
							switch (pk::rand(3))
							{
								case 0:pk::message_box(pk::encode("你们是来交赎金的吗?"), bandit_0);break;
								case 1:pk::message_box(pk::encode("你们是来交赎金的吗?"), bandit_1);break;
								case 2:pk::message_box(pk::encode("你们是来交赎金的吗?"), bandit_2);break;
							}
							pk::cutin(19); 
							duel_result = pk::duel( null, null, kunshu_, selected_person_, null, bandit_1, bandit_0, bandit_2, pk::is_player_controlled(kunshu_), false, 2, true).first;
							pk::background(3);
							if (duel_result == 0)
							{
								pk::message_box(pk::encode("额额...饶命,我这就放人."), bandit_0);
								pk::cutin(20);
								pk::message_box(pk::encode("今后再不可为非作歹,否则绝不轻饶!"), kunshu_);
								pk::message_box(pk::encode("谢谢您把我救了出来,我能回家了."), girl);
								pk::message_box(pk::encode("这次出门真是收获颇丰啊."), selected_person_);
								pk::fade(0);
								pk::fade(255);
								pk::cutin(15);
								pk::message_box(pk::encode("连这点小事都给办砸了,留你们有何用!"), echen);
								pk::message_box(pk::encode("啊啊...你...\n啊......"), bandit_0);
								
								pk::add_stat_exp(kunshu_, 4, exp_mei);
								pk::add_stat_exp(selected_person_, 4, exp_mei);
							
								pk::add_gold(building_, gold_waichu, true);
								kunshu_.update();
								selected_person_.update();	
								building_.update();

								pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x的魅力经验上升了\x1b[1x{}\x1b[0x .", kunshu_name, selected_person_name, exp_mei))); 
								pk::message_box(pk::encode(pk::format("绑架者家属送来慰问,金钱上升了\x1b[1x{}\x1b[0x .", gold_waichu))); 
								switch (pk::rand(3))
								{
									case 0: pk::say(pk::encode("得再找帮可靠的家伙."), echen, kunshu_city); break;
									case 1: pk::say(pk::encode("我当时一点儿都不怕."), girl, kunshu_city); break;
									case 2: pk::say(pk::encode("（?!你是人是鬼?别过来.啊..."), echen, kunshu_city); break;
								}
							}
							else
							{
								pk::message_box(pk::encode("额..我竟然败给了区区贼人."), kunshu_);
								pk::cutin(17);
								pk::message_box(pk::encode("你回去给家属带个话,\n让他带上\x1b[1x双倍赎金\x1b[0x过来赎人.否则休怪我..."), bandit_2);
								pk::message_box(pk::encode("真是气杀我也."), kunshu_);
								pk::message_box(pk::encode("他们人多势众,不妨先回去,再从长计议."), selected_person_);
								pk::message_box(pk::encode("也好也好."), kunshu_);
								
								pk::add_stat_exp(kunshu_, 4, exp_shao);
								pk::add_stat_exp(selected_person_, 4, exp_shao);
							
								pk::add_gold(building_, -gold_waichu, true);
								kunshu_.update();
								selected_person_.update();	
								building_.update();
							
								pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x的魅力经验上升了\x1b[1x{}\x1b[0x .", kunshu_name, selected_person_name, exp_shao)));
								pk::message_box(pk::encode(pk::format("给绑架者家属送去赎金,金钱失去了\x1b[1x{}\x1b[0x .", gold_waichu))); 
								switch (pk::rand(3))
								{
									case 0: pk::say(pk::encode("哇...哇...谁来救救我哇..."), girl, kunshu_city); break;
									case 1: pk::say(pk::encode("嘿嘿.发财了."), bandit_0, kunshu_city); break;
									case 2: pk::say(pk::encode("一群笨蛋,干嘛不要十倍赎金?"), echen, kunshu_city); break;
								}
							}	
							
							//重置绑匪原本名字
							bandit_0.sei = bandit_0_sei;
							bandit_0.mei = bandit_0_mei;
							bandit_1.sei = bandit_1_sei;
							bandit_1.mei = bandit_1_mei;
							bandit_2.sei = bandit_2_sei;
							bandit_2.mei = bandit_2_mei;
							echen.sei = echen_sei;
							echen.mei = echen_mei;
							bandit_0.update();
							bandit_1.update();
							bandit_2.update();
							echen.update();
							
						}
						
						else if (random_n == 1)	
						{
							pk::fade(255);
							pk::background(39); 
							pk::cutin(20);
							pk::message_box(pk::encode("看,前面发生了什么?"), selected_person_);
							
							//给农民改个名字
							string farmer_0_sei = farmer_0.sei;
							string farmer_0_mei = farmer_0.mei;	
							
							farmer_0.sei = pk::encode("可疑");
							farmer_0.mei = pk::encode("之人");
							
							// 事件农民的智商提升
							int 원본지력 = farmer_0.base_stat[武将能力_智力];
							bool 원본화술_wajutsu_kiben = farmer_0.wajutsu_daikatsu;
							bool 원본화술_wajutsu_mushi = farmer_0.wajutsu_daikatsu;
							bool 원본화술_wajutsu_chinsei = farmer_0.wajutsu_daikatsu;

							farmer_0.base_stat[武将能力_智力] = 65 + pk::rand(10);
				
							
							// 农民口才追加
							farmer_0.wajutsu_kiben = true;
							farmer_0.wajutsu_mushi = true;
							farmer_0.wajutsu_chinsei = true;
							farmer_0.update();
							
							pk::message_box(pk::encode("这位大人,\n说来丢脸,犬子在从事诈欺活动呀...\n希望您能前去阻止并说服他!"), oldwoman);
							pk::message_box(pk::encode("好吧,老婆婆您请放心,我一定好言相劝."), kunshu_);
							pk::fade(0);
							pk::background(4); 
							pk::fade(255);
							switch (pk::rand(4))
							{
								case 0: pk::message_box(pk::encode("这几位大姐,\x1b[1x稳赚不赔\x1b[0x的好差事,不看一看吗......"), farmer_0); break;
								case 1:pk::message_box(pk::encode("这位小哥你被\x1b[1x通缉\x1b[0x了你还不知道吗?\n看这份海捕文书上画的是不是你,我跟你说......"), farmer_0); break;
								case 2:pk::message_box(pk::encode("这位好心的大娘,家乡的茶叶滞销了,可以帮忙买一些吗......"), farmer_0); break;
								case 3:pk::message_box(pk::encode("这位老兄,你最近祸事将至了呀!你最近是不是常常......"), farmer_0); break;
							}
							pk::cutin(13); 
							pk::message_box(pk::encode("你就是老婆婆的儿子吧,为什么做这种事情?"), kunshu_);
							pk::message_box(pk::encode("你们是母亲叫来的?"), farmer_0);
							
							//农民名字重置
							farmer_0.sei = farmer_0_sei;
							farmer_0.mei = farmer_0_mei;
							farmer_0.update();
							
							pk::message_box(pk::encode("无论我在做什么,\n这都不干你们的事吧?\n多管闲事!走开别妨碍我."), farmer_0);
							
							debate_result = pk::debate(kunshu_, farmer_0, pk::is_player_controlled(kunshu_), false, false, true).first;
							
							if (debate_result == 0)
							{
								pk::message_box(pk::encode("额额......\n我只是想为母亲分忧而已,\n没想到做了错事!"), farmer_0);
								pk::message_box(pk::encode("您的孝心是好的,\n可\x1b[1x君子爱财取之有道\x1b[0x,\n希望您以后能走正途."), selected_person_);
								pk::fade(0);
								pk::background("media/background/baifang/minju01.png", 0);
								pk::fade(255);
								pk::message_box(pk::encode("母亲,不孝子回来了."), farmer_0);
								pk::message_box(pk::encode(pk::format("真是太感谢\x1b[1x{}\x1b[0x大人和\x1b[1x{}\x1b[0x大人了.", kunshu_name, selected_person_name)), oldwoman);
								
								pk::add_stat_exp(kunshu_, 4, exp_mei);
								pk::add_stat_exp(selected_person_, 4, exp_mei);
							
								pk::add_gold(building_, gold_waichu, true);
								kunshu_.update();
								selected_person_.update();	
								building_.update();

								pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x的魅力经验上升了\x1b[1x{}\x1b[0x .", kunshu_name, selected_person_name, exp_mei))); 
								pk::message_box(pk::encode(pk::format("家属送来感谢金,金钱上升了\x1b[1x{}\x1b[0x .", gold_waichu))); 
								switch (pk::rand(2))
								{
									case 0: pk::say(pk::encode("母亲,我会改正的."), farmer_0, kunshu_city); break;
									case 1: pk::say(pk::encode(pk::format("{}大人真是大好人啊.", kunshu_name)), oldwoman, kunshu_city);  break;
								}
							}
							else
							{
								pk::message_box(pk::encode("无论是\x1b[1x肮脏\x1b[0x还是\x1b[1x干净\x1b[0x,钱都是好的.\n我只是想要养家糊口有什么错!!走开走开."), farmer_0);
								pk::message_box(pk::encode("额额,这么说也有道理...\n没能说服他呢,老婆婆要伤心了呀."), kunshu_);
								pk::message_box(pk::encode("不妨先回去,再从长计议."), selected_person_);
								pk::message_box(pk::encode("也好也好."), kunshu_);
								
								pk::add_stat_exp(kunshu_, 4, exp_shao);
								pk::add_stat_exp(selected_person_, 4, exp_shao);
							
								pk::add_gold(building_, -gold_waichu, true);
								kunshu_.update();
								selected_person_.update();	
								building_.update();
							
								pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x的魅力经验上升了\x1b[1x{}\x1b[0x .", kunshu_name, selected_person_name, exp_shao)));
								pk::message_box(pk::encode(pk::format("给老婆婆送去慰问,金钱失去了\x1b[1x{}\x1b[0x .", gold_waichu))); 
								switch (pk::rand(2))
								{
									case 0: pk::say(pk::encode("养家糊口,天经地义."), farmer_0, kunshu_city); break;
									case 1: pk::say(pk::encode("我的儿啊,你什么时候回来.."), oldwoman, kunshu_city);  break;
								}
							}
							// 恢复原本的智力和健康程度
							farmer_0.base_stat[武将能力_智力] = 원본지력;
							
							// 恢复口才
							farmer_0.wajutsu_kiben = 원본화술_wajutsu_kiben;
							farmer_0.wajutsu_mushi = 원본화술_wajutsu_mushi;
							farmer_0.wajutsu_chinsei = 원본화술_wajutsu_chinsei;
							
							farmer_0.update();
						}
						
						else if (random_n == 2)
						{
							pk::fade(255);
							pk::background(39); 
							pk::cutin(20);
							pk::message_box(pk::encode("看,前面发生了什么?"), selected_person_);
							pk::message_box(pk::encode("行行好吧,\n行行好吧..."), oldwoman);
							pk::message_box(pk::encode("求求这两位大人,\n我们已经好几天没有吃饭了..."), oldman);
							pk::message_box(pk::encode("阿...翁...我...饿..."), girl);
							pk::message_box(pk::encode("（好可怜的孩子..）\n你们是什么人?其他家人呢?"), selected_person_);
							
							switch (pk::rand(3))
							{
								case 0: pk::message_box(pk::encode("我们是逃难过来的.\n儿子服徭役时死了,只留下一孙女.\n求求你们给我孙女些吃的吧"), oldman); break;
								case 1: pk::message_box(pk::encode("我们是逃难过来的.\n儿子被黄巾贼杀了,只留下一孙女.\n求求你们给我孙女些吃的吧"), oldman); break;
								case 2: pk::message_box(pk::encode("我们是逃难过来的.\n儿子服兵役时战死了,只留下一孙女.\n求求你们给我孙女些吃的吧"), oldman); break;
							}
							pk::message_box(pk::encode("我们两老不打紧,可孩子快受不了..\n呜呜呜..."), oldwoman);
							array<string> shishe_answer1 =  
							{
								pk::encode("  是  "),
								pk::encode("  否  ")
							};
		
							int shishe_answer = pk::choose(pk::encode("是否伸出援手?"), shishe_answer1);
							
							if (shishe_answer == 0)
							{
								pk::fade(255);
								pk::play_se(16);
								pk::message_box(pk::encode("这些拿去买些吃的吧."), kunshu_);
								pk::message_box(pk::encode("谢谢大人,谢谢大人..\n呜呜呜..."), oldwoman);
								
								pk::add_stat_exp(kunshu_, 4, exp_shao);
								pk::add_stat_exp(selected_person_, 4, exp_shao);
								pk::add_gold(building_, -gold_waichu, true);
								kunshu_.update();
								selected_person_.update();	
								building_.update();
								pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x的魅力经验上升了\x1b[1x{}\x1b[0x .", kunshu_name, selected_person_name, exp_shao)));
								
								switch (pk::rand(2))
								{
									case 0: pk::say(pk::encode("嗯..嗯..好吃.."), girl, kunshu_city); break;
									case 1: pk::say(pk::encode("孩子慢点吃."), oldwoman, kunshu_city); break;
								}
							}
							else
							{
								pk::message_box(pk::encode("走了走了,这样的人到处都是."), kunshu_);
								pk::message_box(pk::encode("可他们..."), selected_person_);
								switch (pk::rand(3))
								{
									case 0: pk::say(pk::encode("行行好吧.."), oldman, kunshu_city); break;
									case 1: pk::say(pk::encode("孩子!!我的孩啊..呜呜.."), oldwoman, kunshu_city); break;
									case 2: pk::say(pk::encode("（不能不管啊..）老翁这些拿去吧."), selected_person_, kunshu_city); break;
								}
								
							}
						}
						
						/* else if (random_n == 3)	
						{
							
						}  //遇刺事件 */
						/* else if (random_n == 3)  //抓住细作
						{
							pk::message_box(pk::encode("卖糖葫芦啦，一文钱一串~"), pk::get_person(武将_商人));
							pk::message_box(pk::encode("刚到的蜜桃，水灵灵的，只要....~"), pk::get_person(武将_商人));
							pk::message_box(pk::encode("（......总觉的不对劲?）"), kunshu_);
							pk::message_box(pk::encode(pk::format("{}怎么了?", call_kunshu)), selected_person_);
							pk::cutin(20);
							pk::message_box(pk::encode("此人是敌人潜入城中的细作!!\n来人,给我拿下!!"), kunshu_);
							pk::message_box(pk::encode("!!（到底是怎么被发现的?撤!）"), pk::get_person(武将_商人));
							pk::cutin(18);
							pk::message_box(pk::encode("哪里走!!"), pk::get_person(武将_卫士));
							pk::message_box(pk::encode("额...到此为止了吗?"), pk::get_person(武将_商人));
							pk::fade(0);
							pk::fade(255);
							pk::message_box(pk::encode("报告,细作已经拿下!"), pk::get_person(武将_卫士));
							pk::cutin(13);
							pk::message_box(pk::encode("说!!到底是谁指使你来的?"), kunshu_);
							pk::message_box(pk::encode("哼哼."), pk::get_person(武将_商人));
							pk::message_box(pk::encode("打入大牢,细细审问."), kunshu_);
							pk::message_box(pk::encode("是!"), pk::get_person(武将_卫士));
						} */
						
						else 
						{
							switch (pk::rand(2))
							{
								case 0:
									pk::message_box(pk::encode("这位大人,\n新鲜的瓜果,刚从地里摘的尖儿,\n要不要买一点呢?"), oldman);
									pk::message_box(pk::encode("那么要一些吧."), selected_person_);
									pk::message_box(pk::encode("您的瓜一共十文钱."), boy);
									pk::message_box(pk::encode("真是个乖巧的孩子."), selected_person_);
									pk::cutin(12); 
									pk::message_box(pk::encode("不知不觉时候已经不早了呀,走吧回去了."), kunshu_);
									pk::message_box(pk::encode("好的.今天很尽兴."), selected_person_);
								break;
								case 1:
									pk::message_box(pk::encode("这位客官,\n上好的丝绸,多好的样式啊,不看看吗?"), shangren_0);
									pk::message_box(pk::encode("这个不错."), selected_person_);
									pk::message_box(pk::encode("您真有眼光,这可是\x1b[1x蜀锦\x1b[0x呀,\n您看这色彩和花文做工都是上佳的呀."), shangren_0);
									pk::message_box(pk::encode("确实非常好看,那么多少钱呢?"), selected_person_);
									pk::message_box(pk::encode("两百金一匹,给您送到府里您看?"), shangren_0);
									pk::message_box(pk::encode("（额没想到这么贵...）\n我改天再来买."), selected_person_);
									pk::cutin(12); 
									pk::message_box(pk::encode("走了一天,街道平静又充满生气."), kunshu_);
									pk::message_box(pk::encode("是呀.偶尔也该多出来走走."), selected_person_);
								break;
							}
													
						}
					}
					
					else if (waichu_list == 1)
					{
						int horse_list;
						int horse_i = pk::rand(6);
						int horse_j = pk::rand(6);
						while (horse_j == horse_i)  horse_j = pk::rand(6);
						
						//给商人改个名字
						string shangren_0_sei = shangren_0.sei;
						string shangren_0_mei = shangren_0.mei;	
						
						shangren_0.sei = pk::encode("马匹");
						shangren_0.mei = pk::encode("商人");
						shangren_0.update();
						
						pk::message_box(pk::encode("听说最近来了个马匹商人,\n去看看,或许会遇到好马."), kunshu_);
						pk::message_box(pk::encode("是吗?那走吧去看看."), selected_person_);
						pk::cutin(12); 
						pk::message_box(pk::encode("骏马良驹快来看看哟~\n客官,看马吗?"), shangren_0);
						pk::background("media/background/baifang/ma01.png", 0); 
						
						bool confirm = false;
						while (!confirm)
						{
							horse_list = scene_malist1(horse_i, horse_j);  //进入列表

							if (horse_list == 0)
							{
								//读取马的背景图片
								pk::background("media/background/horse/h0.png");
								pk::message_box(pk::encode("这匹看起来很眼熟.."), selected_person_);
								pk::message_box(pk::encode("不错,这匹正是武将都很喜爱的\x1b[1x脚力马\x1b[0x,\n虽说看着普通,但也是匹良驹呀."), shangren_0);
								pk::message_box(pk::encode("负重高,耐力好,不挑吃食,\n正由于这诸多的优点,\n故成为了首选的代步马匹."), shangren_0);
								confirm = pk::yes_no(pk::encode("买这匹吗?"));
								if (confirm)
								{
									selected_person_.horse = 255;
									ch::set_person_horse(selected_person_.get_id(), 255);  //ch::set_person_horse(person_id, 编号)来调用对应模型
									selected_person_.update();
								}
							}
							else if (horse_list == 1)
							{
								int horse_num = horseNum(horse_i);
								confirm = pk::yes_no(pk::encode("买这匹吗?"));
								if (confirm)
								{
									selected_person_.horse = 255;
									ch::set_person_horse(selected_person_.get_id(), horse_num);  //ch::set_person_horse(person_id, 编号)来调用对应模型
									selected_person_.update();
								}
							}
							else if (horse_list == 2)
							{
								int horse_num = horseNum(horse_j);
								confirm = pk::yes_no(pk::encode("买这匹吗?"));
								if (confirm)
								{
									selected_person_.horse = 255;
									ch::set_person_horse(selected_person_.get_id(), horse_num);  //ch::set_person_horse(person_id, 编号)来调用对应模型
									selected_person_.update();
								}
							}

						}
						
						pk::message_box(pk::encode("确实是匹好马,那么多少钱呢?"), selected_person_);
						pk::message_box(pk::encode("五百金."), shangren_0);
						pk::message_box(pk::encode("（额这么贵...可这么好的马...）\n我改日再来买."), selected_person_);
						pk::message_box(pk::encode("此马仅此一匹,迟了就卖与他人了."), shangren_0);
						
						//商人名字重置
						shangren_0.sei = shangren_0_sei;
						shangren_0.mei = shangren_0_mei;
						shangren_0.update();
							
						pk::message_box(pk::encode("买了."), kunshu_);
						pk::message_box(pk::encode("此良驹\x1b[1x可遇不可求\x1b[0x."), kunshu_);
						pk::message_box(pk::encode(pk::format("看得出\x1b[1x{}\x1b[0x也很是喜爱.\n\x1b[1x宝马配英雄\x1b[0x,就将此马赠与{}!", call_selected_person, call_selected_person)), kunshu_);
						pk::cutin(20); 
						pk::message_box(pk::encode(pk::format("那真是太感谢{}了.\n自当骑此马阵前立效以报{}的厚恩.", call_kunshu, call_kunshu)), selected_person_);
						
						pk::add_loyalty(selected_person_, zhong_bian);
						selected_person_.update();
						pk::add_gold(building_, -500, true);
						building_.update();
						kunshu_city.merchant_done = true;  //交易完成
						pk::message_box(pk::encode("买马花了些钱.")); 
						
						switch (pk::rand(3))
						{
							case 0: pk::say(pk::encode("驾!驾!真是匹快马!"), selected_person_, kunshu_city); break;
							case 1: pk::say(pk::encode("起个什么名字好呢?"), selected_person_, kunshu_city);  break;
							case 2: pk::say(pk::encode("清扫爱驹的马舍."), selected_person_, kunshu_city); break;
						}
					}
					pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x一起外出逛了逛.", kunshu_name, selected_person_name)));

				} //外出
				

				else if (n == 3)  //师事
				{
					if (building_gold < gold_shishi)
					{
						pk::message_box(pk::encode("如今库中羞涩,岂可空手登门呀?\n等以后有机会再去吧!"), kunshu_);
						pk::fade(0);
						pk::background(-1);
						return;
					}
					
					if (kunshu_.birth + 12 < selected_person_.birth) //排除向过小的人不耻下问，特别是自己孩子
					{
						pk::message_box(pk::encode("（比我小这么多岁,似乎有些放不下身段呀...）\n还是找其他人请教吧!"), kunshu_);
						pk::fade(0);
						pk::background(-1);
						return;
					}
					
					if (pk::rand_bool(75)) 
					{
						pk::message_box(pk::encode("这是我的一点薄礼."), kunshu_);
						pk::message_box(pk::encode("有什么事吗?您不妨直说."), selected_person_);
						pk::message_box(pk::encode(pk::format("我希望能跟{}学习一番.", call_selected_person)), kunshu_);

						if (pk::is_like(selected_person_, kunshu_0)) 
						{
							random_n = pk::rand(3);
							if ((random_n == 0) or (random_n == 1))
							{
								if(selected_person_.stat[武将能力_武力] > kunshu_.stat[武将能力_武力] )
								{								
									pk::cutin(17);
									pk::message_box(pk::encode("哦?容我考虑考虑,这样好了,如果你单挑能胜过我就答应你."), selected_person_);
									pk::unit@ dummy_unit;
									duel_result = pk::duel(dummy_unit, dummy_unit, kunshu_, null, null, selected_person_, null, null, pk::is_player_controlled(kunshu_), false, 0, true).first;
							
									if (duel_result == 0)
									{
										scene_choose_list_1();										
									}
									else
									{
									pk::message_box(pk::encode(pk::format("额..{}果然厉害,我远不及也.", call_selected_person)), kunshu_);
									pk::message_box(pk::encode("今天就先回去吧."), kunshu_);
									
									pk::add_stat_exp(kunshu_, 1, exp_shao);
									kunshu_.update();
								
									pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的武力经验稍稍上升了\x1b[1x{}\x1b[0x .", kunshu_name, exp_shao))); 
									}
								}
								else if((selected_person_.stat[武将能力_武力] < kunshu_.stat[武将能力_武力] ) and (selected_person_.stat[武将能力_智力] > kunshu_.stat[武将能力_智力]))
								{
									pk::cutin(17);
									pk::message_box(pk::encode("哦?容我考虑考虑,这样好了,如果你舌战能胜过我就答应你."), selected_person_);
									debate_result = pk::debate(kunshu_, selected_person_, pk::is_player_controlled(kunshu_), false, false, true).first;
									
									if (debate_result == 0)
									{
										scene_choose_list_1();
									}
									else
									{
										pk::message_box(pk::encode(pk::format("额..{}果然厉害,我远不及也.", call_selected_person)), kunshu_);
										pk::message_box(pk::encode("今天就先回去吧."), kunshu_);
										pk::add_stat_exp(kunshu_, 2, exp_shao);

										kunshu_.update();
									
										pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的智力经验稍稍上升了\x1b[1x{}\x1b[0x .", kunshu_name, exp_shao))); 
									}
								}
								else
								{
									pk::message_box(pk::encode("虚名而已,我实在没什么能教的.请另寻高明吧."), selected_person_);
									pk::message_box(pk::encode("额,那今天就先回去吧."), kunshu_);
								}
							}
							else
							{
								pk::fade(0);
								pk::fade(255);
								pk::message_box(pk::encode("（......\n先生今天看起来很忙的样子,就不打扰他了...）"), kunshu_);
								pk::message_box(pk::encode("那么我今天就先告辞了."), kunshu_);
							}	
						}
						else
						{
							pk::fade(0);
							pk::fade(255);
							pk::message_box(pk::encode("（......\n先生今天看起来很忙的样子,就不打扰他了...）"), kunshu_);
							pk::message_box(pk::encode("那么我今天就先告辞了."), kunshu_);
						}
					}

					else
					{	
						pk::fade(0);
						pk::fade(255);				
						pk::message_box(pk::encode(pk::format("（......\n今天\x1b[1x{}\x1b[0x先生好像要出门了,只好改天再来了.）", selected_person_name)), kunshu_);

					}

					pk::add_gold(building_, -gold_shishi, true);
					building_.update();
					pk::message_box(pk::encode("买礼物花了些钱.")); 
				} //师事
				
				else if (n == 4)  //探病
				{
					if (building_gold < gold_tanbing)
					{
						pk::message_box(pk::encode("如今库中羞涩,无法买些礼物补品.\n病情再探也无许多益处,先回吧!"), kunshu_);
						pk::fade(0);
						pk::background(-1);
						return;
					}
					if (selected_person_.shoubyou == 0)   //排除健康状态
					{
						switch (pk::rand(2))
						{
							case 0: 
								pk::message_box(pk::encode(pk::format("听闻\x1b[1x{}\x1b[0x生病了,真是辛苦了!\n我买了些\x1b[1x特制的补品\x1b[0x......", call_selected_person)), kunshu_);
							break;
							case 1:
								pk::message_box(pk::encode(pk::format("听闻\x1b[1x{}\x1b[0x受伤了,真是辛苦了!\n我买了些\x1b[1x奇特药\x1b[0x......", call_selected_person)), kunshu_);
							break;
						}
						
						switch (pk::rand(2))
						{
							case 0: 
								pk::cutin(20); 
								pk::message_box(pk::encode(pk::format("完全没有这回事呢!{}身体一直很好.", self_selected_person)), selected_person_); 
							break;
							case 1:
								pk::message_box(pk::encode("前些天确实呢,不过今天已经完全恢复了."), selected_person_); 
							break;
						}
						pk::message_box(pk::encode("那很好.\n那个没事了也能吃,可以\x1b[1x强身健体\x1b[0x."), kunshu_);
						pk::message_box(pk::encode("那我就先告辞了."), kunshu_);
						pk::fade(0);
						pk::fade(255);
						pk::message_box(pk::encode("姑且试试吧,\n气味闻起来有些怪."), selected_person_); 
						
						int j = pk::rand(100) + 1;
						if (j <= 70)
						{							
							switch (pk::rand(3))
							{
								case 0: pk::say(pk::encode("好像没有作用."), selected_person_, kunshu_city); break;
								case 1: pk::say(pk::encode(pk::format("{}是不是被骗了?", call_kunshu)), selected_person_, kunshu_city);  break;
								case 2: pk::say(pk::encode("好像没什么感觉."), selected_person_, kunshu_city); break;
							}
						}
						else if (j > 85)
						{
							int i = 0;
							while(i < 3)
							{
								if (selected_person_.shoubyou == i)
								{
									selected_person_.shoubyou = i + 1;
									selected_person_.update();
									break;
								}
								i++;
							}
							pk::message_box(pk::encode(pk::format("探望不久后,\x1b[1x{}\x1b[0x的状况似乎有些不对.", selected_person_name))); 
							switch (pk::rand(3))
							{
								case 0: pk::say(pk::encode("这东西好像不对劲...呕..."), selected_person_, kunshu_city); break;
								case 1: pk::say(pk::encode(pk::format("{}可把我坑惨了...", call_kunshu)), selected_person_, kunshu_city);  break;
								case 2: pk::say(pk::encode("黄土草灰牛粪各三钱,又做好一副."), shangren_0, kunshu_city); break;
							}
						}
						else
						{
							random_n = pk::rand(4);
							pk::add_stat_exp(selected_person_, random_n, exp_shao);
							selected_person_.update();
							switch (pk::rand(3))
							{
								case 0: pk::say(pk::encode("这东西真奇妙!下次再要一些."), selected_person_, kunshu_city); break;
								case 1: pk::say(pk::encode(pk::format("哇真好用,不知道{}还有吗?", call_kunshu)), selected_person_, kunshu_city);  break;
								case 2: pk::say(pk::encode("感觉格外的神清气爽."), selected_person_, kunshu_city); break;
							}
						}
					}
					else
					{
						pk::fade(0);
						pk::background("media/background/baifang/tanbing01.png", 0);
						pk::fade(255);
						switch (pk::rand(2))
						{
							case 0: 
								pk::message_box(pk::encode(pk::format("听闻\x1b[1x{}\x1b[0x生病了,真是辛苦了!\n我买了些\x1b[1x特制的补品\x1b[0x,注意保重身体.", call_selected_person)), kunshu_);
								pk::message_box(pk::encode(pk::format("谢谢{}的厚爱,自当竭尽全力...咳咳咳...", call_kunshu)), selected_person_);
							break;
							case 1:
								pk::message_box(pk::encode(pk::format("听闻\x1b[1x{}\x1b[0x受伤了,真是辛苦了!\n我买了些\x1b[1x奇特药\x1b[0x,注意保重身体.", call_selected_person)), kunshu_);
								pk::message_box(pk::encode(pk::format("多谢{}的厚爱,我的小伤不足挂齿...咳咳咳...", call_kunshu)), selected_person_);
							break;
						}
						pk::message_box(pk::encode("那您先休息,我这就告辞了"), kunshu_);
						pk::message_box(pk::encode(pk::format("好的,请恕{}不便相送,您慢走...", self_selected_person)), selected_person_);
						int j = pk::rand(100) + 1;
						if (j <= 70)
						{
							selected_person_.shoubyou = 0;
							selected_person_.update();
							pk::message_box(pk::encode(pk::format("探望不久后,\x1b[1x{}\x1b[0x似乎恢复了健康.", selected_person_name))); 
							switch (pk::rand(3))
							{
								case 0: pk::say(pk::encode("真是灵验,已经好多了."), selected_person_, kunshu_city); break;
								case 1: pk::say(pk::encode(pk::format("多亏了{}才好得这么快.", call_kunshu)), selected_person_, kunshu_city);  break;
								case 2: pk::say(pk::encode("神清气爽感觉真好."), selected_person_, kunshu_city); break;
							}
						}
						else if (j > 90)
						{
							int i = 0;
							while(i < 3)
							{
								if (selected_person_.shoubyou == i)
								{
									selected_person_.shoubyou = i + 1;
									selected_person_.update();
									break;
								}
								i++;
							}
							pk::message_box(pk::encode(pk::format("探望不久后,\x1b[1x{}\x1b[0x的情况似乎更严重了.", selected_person_name))); 
							switch (pk::rand(3))
							{
								case 0: pk::say(pk::encode("这东西好像不对劲...呕..."), selected_person_, kunshu_city); break;
								case 1: pk::say(pk::encode(pk::format("{}可把我坑惨了...", call_kunshu)), selected_person_, kunshu_city);  break;
								case 2: pk::say(pk::encode("黄土草灰牛粪各三钱,又做好一副."), shangren_0, kunshu_city); break;
							}
						}
						else
						{
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x似乎没有要好转的样子.", selected_person_name))); 
						}
					}
					
					pk::add_gold(building_, -gold_tanbing, true);	
					building_.update();
					pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x前往探望了\x1b[1x{}\x1b[0x.", kunshu_name, selected_person_name)));
					pk::message_box(pk::encode("探病花了些钱.")); 	

				} //探病
				
				
				else    //其他
				{
					random_n = pk::rand(2);
					if (random_n == 0 && building_gold > gold_내기)  //得够钱
					{
						pk::cutin(17); 
						pk::message_box(pk::encode("来来来,手痒难耐了,来赌两把?"), kunshu_);
						pk::message_box(pk::encode(pk::format("好!我手气正旺,定让{}血本无归!", call_kunshu)), selected_person_);
						pk::message_box(pk::encode("哦?是嘛,鹿死谁手尚未可知."), kunshu_);
						array<string> gambling_answers1 =
							{
								pk::encode("买大"),
								pk::encode("买小")
							};
						
						int gambling_answers = pk::choose(pk::encode("买定离手."), gambling_answers1);
						int gambling = pk::rand(2);
					
						if (gambling_answers == gambling)
						{
							pk::cutin(17); 
							pk::message_box(pk::encode("哈哈赢了!\n来来再玩."), kunshu_);
							pk::message_box(pk::encode("额额不玩了...今天手气败光了."), selected_person_);
							pk::message_box(pk::encode("那我就把赢的金子拿走了."), kunshu_);
							pk::add_gold(building_, gold_내기, true);	
							building_.update();
						}
						else
						{
							pk::message_box(pk::encode("哈哈,手气旺得很!\n想输都输不了."), selected_person_);
							pk::message_box(pk::encode("额额不玩了...今天输光了."), kunshu_);
							pk::message_box(pk::encode("别走嘛,我借你点再玩会儿?"), selected_person_);
							pk::add_gold(building_, -gold_내기, true);	
							building_.update();
						}

					} // 赌钱
					
					else
					{
						pk::message_box(pk::encode(pk::format("我这正好有一坛美酒,\x1b[1x{}\x1b[0x要不要尝一下.",call_selected_person)), kunshu_);
						pk::message_box(pk::encode(pk::format("哈哈哈,还是{}明白{}.", call_kunshu, self_selected_person)), selected_person_);
						pk::message_box(pk::encode("我正想喝一杯.."), selected_person_);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x一起喝了酒.", kunshu_name, selected_person_name))); 
					}  
				}   //其他
				

			} // if (!pk::is_fuufu(kunshu_, selected_person_name))
			
		
			else 
			{
				pk::fade(0);
				pk::background(5); 
				pk::fade(255);
				random_n = pk::rand(8);
				
				// 배우자 지력 RANDOM 변경
				int 원본지력 = selected_person_.base_stat[武将能力_智力];
				bool 원본화술_wajutsu_daikatsu = selected_person_.wajutsu_daikatsu;
				bool 원본화술_wajutsu_kiben = selected_person_.wajutsu_daikatsu;
				bool 원본화술_wajutsu_mushi = selected_person_.wajutsu_daikatsu;
				bool 원본화술_wajutsu_chinsei = selected_person_.wajutsu_daikatsu;
				bool 원본화술_wajutsu_gyakujou = selected_person_.wajutsu_daikatsu;

				selected_person_.base_stat[武将能力_智力] = arr_stat[pk::rand(6)];
	
				
				// 화술 추가
				selected_person_.wajutsu_daikatsu = true;
				selected_person_.wajutsu_kiben = true;
				selected_person_.wajutsu_mushi = true;
				selected_person_.wajutsu_chinsei = true;
				selected_person_.wajutsu_gyakujou = true;
				
				selected_person_.update();
				
				if (random_n == 0)
				{
					if (selected_person_.sex == 성별_여) 
					{
						pk::message_box(pk::encode("夫君,给妾身一些钱改善下生活,好吗!"), selected_person_);
						pk::message_box(pk::encode("可是国家也等着用钱..."), kunshu_);
						pk::message_box(pk::encode("我们好好谈谈如何?"), selected_person_);
				
						debate_result = pk::debate(kunshu_, selected_person_, pk::is_player_controlled(kunshu_), false, false, true).first;
					
						 if (debate_result == 0)
						 {
							pk::message_box(pk::encode("既然夫君这么说,妾身也不好再多做要求了."), selected_person_);
							pk::message_box(pk::encode("等国家有钱了,为夫一定会补偿你的."), kunshu_);
						 }
						 else
						 {
							pk::message_box(pk::encode("夫人这么说确实.."), kunshu_);
							pk::message_box(pk::encode("妾身也是为了我们的家."), selected_person_);
					
							pk::add_gold(building_, -gold_배우자, true);	
							building_.update();
					
							pk::message_box(pk::encode("给了夫人些金子."));
						 }
					}

					else if (selected_person_.sex == 성별_남) 
					{
						pk::message_box(pk::encode("夫人,能给我些钱让我举办宴会与朋友喝酒吗!"), selected_person_);
						pk::message_box(pk::encode("可是国家也等着用钱..."), kunshu_);
						pk::message_box(pk::encode("我们好好谈谈如何?"), selected_person_);
				
						debate_result = pk::debate(kunshu_, selected_person_, pk::is_player_controlled(kunshu_), false, false, true).first;
					
						 if (debate_result == 0)
						 {
							pk::message_box(pk::encode("既然夫人这么说,我就暂时打消这个念头吧."), selected_person_);
							pk::message_box(pk::encode("等国家有钱了,一定会依你所愿举办一场盛大的宴会."), kunshu_);
						 }
						 else
						 {
							pk::message_box(pk::encode("夫君这么说确实.."), kunshu_);
							pk::message_box(pk::encode("我也是为了将士们的团结和斗志啊."), selected_person_);
					
							pk::add_gold(building_, -gold_배우자, true);	
							building_.update();
					
							pk::message_box(pk::encode("给了夫君些金子."));
						 }
					}		 					
				}
				
				else if (random_n == 1)
				{
					if (selected_person_.sex == 성별_여) 
					{
						pk::message_box(pk::encode("夫君,这簪子哪里来的?!"), selected_person_);
						pk::message_box(pk::encode("这..我也是第一次见到.."), kunshu_);
						pk::message_box(pk::encode("我不会放过你的!"), selected_person_);
						
						debate_result = pk::debate(kunshu_, selected_person_, pk::is_player_controlled(kunshu_), false, false, true).first;
							
						if (debate_result == 0)
						{
							pk::message_box(pk::encode("我误会了..."), selected_person_);
							pk::message_box(pk::encode("能消除误会真是万幸."), kunshu_);
							pk::message_box(pk::encode("夫君,这是妾身的一点心意."), selected_person_);
							pk::message_box(pk::encode("哈哈哈!没事就好."), kunshu_);
							
							pk::add_gold(building_, bonus_gold, true);	
							building_.update();
							
							pk::message_box(pk::encode("从夫人那获得了些金子."));
						}
						else
						{
							pk::message_box(pk::encode("果然,我的直觉没错!"), selected_person_);
							pk::message_box(pk::encode("这..."), kunshu_);
							pk::cutin(20); 
							
							kunshu_.shoubyou = 상병_중증;
							kunshu_.update();
							
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x被\x1b[1x{}\x1b[0x打得半死.", kunshu_name, selected_person_name))); 
						}	
					}
					
					if (selected_person_.sex == 성별_남) 
					{
						pk::message_box(pk::encode("夫人,这些首饰哪里来的?!"), selected_person_);
						pk::message_box(pk::encode("这..我前些日子去街上没忍住.."), kunshu_);
						pk::message_box(pk::encode("可你已经有了那么多了..."), selected_person_);
						
						debate_result = pk::debate(kunshu_, selected_person_, pk::is_player_controlled(kunshu_), false, false, true).first;
							
						if (debate_result == 0)
						{
							pk::message_box(pk::encode("之前的都太老旧了,你确实应该买些新的首饰了..."), selected_person_);
							pk::message_box(pk::encode("能得到夫君的理解真是万幸."), kunshu_);
							pk::message_box(pk::encode("夫人,这是我的一点心意."), selected_person_);
							pk::message_box(pk::encode("夫君可真是体谅妾身的辛劳."), kunshu_);
							
							pk::add_gold(building_, bonus_gold, true);	
							building_.update();
							
							pk::message_box(pk::encode("从夫君那获得了些金子."));
						}
						else
						{
							pk::message_box(pk::encode("果然,你又背着我偷偷买了这些东西!"), selected_person_);
							pk::message_box(pk::encode("可是..."), kunshu_);
							pk::cutin(20); 
							
							kunshu_.shoubyou = 상병_중증;
							kunshu_.update();
							
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x受到了\x1b[1x{}\x1b[0x的严厉训斥.", kunshu_name, selected_person_name))); 
						}	
					}
				}


				else if (random_n == 2)
				{
					pk::play_bgm(19);
					if (selected_person_.sex == 성별_여) 
					{
					        pk::message_box(pk::encode("夫君,在外辛苦了,特地为你做了些你爱吃的菜."), selected_person_);
					        pk::message_box(pk::encode("谢谢夫人!"), kunshu_);
					
					        kunshu_.shoubyou = 상병_건강;
					        kunshu_.update();
					
					        pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的健康状态已变为\x1b[1x良好\x1b[0x.", kunshu_name))); 
					}
					else if (selected_person_.sex == 성별_남) 
					{
					        pk::message_box(pk::encode("夫人为国家操劳不堪,特地给你买了些大补的补品."), selected_person_);
					        pk::message_box(pk::encode("谢谢夫君!"), kunshu_);
					
					        kunshu_.shoubyou = 상병_건강;
					        kunshu_.update();
					
					        pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的健康状态已变为\x1b[1x良好\x1b[0x.", kunshu_name))); 
					}
				}
				
				else if (random_n == 3 || random_n == 4) //生子
				{
					pk::person@ child = null, father = null, mother = null;
					int sex = -1, mother_id;
					if (selected_person_.sex == 성별_여)
					{
						mother_id = selected_person_.get_id();
						@child = getEmptyChild(mother_id);
						@father = @kunshu_;
						@mother = @selected_person_;
					}
					else
					{
						mother_id = kunshu_.get_id();
						@child = getEmptyChild(mother_id);
						@father = @selected_person_;
						@mother = @kunshu_;
					}
					
					//生子年龄范围《英雄集结女流之战》可用
					if (child !is null)
					{
						if (pk::get_scenario().no == 7 or pk::get_scenario().no == 13
							or (pk::get_year() >= (kunshu_.birth + 19) and pk::get_year() <= (kunshu_.birth + 49) and pk::get_year() >= (selected_person_.birth + 19) and pk::get_year() <= (selected_person_.birth + 39)))
						{
							sex = random(0,1);
							//ch::u8debug(pk::format("cpp340line714 sex={}", sex));

							pk::fade(0);
							pk::background(5);
							/* pk::background(sex == 성별_남 ? 30 : 32);  */   //测试
							pk::fade(255);
							
							string call_junzhu = kunshu_.sex == 성별_남 ? "夫君" : "夫人";	//夫君/夫人
							string call_peiou = selected_person_.sex == 성별_남 ? "夫君" : "夫人";	//夫君/夫人
							string call_child = sex == 성별_남 ? "儿子" : "女儿";	//?子/女?
							string call_DadMom = kunshu_.sex == 성별_남 ? "父亲" : "母亲";	//父?/母?
							if (kunshu_.sex == 성별_남)
							{
								switch (pk::rand(2))
								{
									case 0: 
										pk::play_bgm(19);
										pk::message_box(pk::encode(pk::format("{},告诉你一个好消息.", call_junzhu)), selected_person_);
										pk::message_box(pk::encode("刚刚大夫替我把脉,说我有喜了!"), selected_person_);
									break;
									case 1: 
										pk::play_bgm(21);
										pk::message_box(pk::encode(pk::format("{},刚刚大夫替我把脉.\n大夫说我...我...\n呜呜呜...", call_junzhu)), selected_person_);
										pk::message_box(pk::encode(pk::format("{}发生什么了?\n你先别哭,无论什么样的病,\n我一定找遍天下名医治好你!", call_peiou)), kunshu_);
										pk::cutin(17);
										pk::message_box(pk::encode("大夫说我...我有喜了!"), selected_person_);
										pk::message_box(pk::encode("哦,吓死我,我还以为...\n嗯?不对!有喜了!?"), kunshu_);
										pk::message_box(pk::encode("哈哈哈,被我骗到了吧?"), selected_person_);
									break;
								}	
								pk::cutin(20);
								pk::message_box(pk::encode("有喜了是真...真的吗?"), kunshu_);
								pk::message_box(pk::encode(pk::format("是真的呢,\n\x1b[1x脉象圆滑流利,如盘走珠,\x1b[0x大夫如是说,\n还说我所怀的是个\x1b[1x{}\x1b[0x呢!", call_child)), selected_person_); 
								pk::message_box(pk::encode(pk::format("太好了!\n我{}有{}了!我们有{}了!\n{},真的太感谢你了!", kunshu_name, call_child, call_child, call_peiou)),  kunshu_);
								//pk::trace(pk::format("剧本编号为{}", pk::get_scenario().no));
								if (pk::get_scenario().no == 7 || pk::get_scenario().no == 13 || pk::get_year() < (selected_person_.birth + 34))   //《英雄集结女流之战》不会，母亲的年龄比较年轻
								{
									pk::play_bgm(19);
									pk::message_box(pk::encode("帮我们的孩子起个名字吧!"), selected_person_);
								}
								else
								{
									pk::message_box(pk::encode("..."), selected_person_);
									pk::message_box(pk::encode("你怎么不高兴了?"), kunshu_);
									pk::message_box(pk::encode("大夫说,以我目前的年龄与身体情况,\n已经不适合生产."), selected_person_);
									pk::message_box(pk::encode("若执意要生产,\n恐怕会有难以预料的危险.\n故而心中不安,你怎么看呢?"), selected_person_);
									array<string> shengzi_1 =
									{
										pk::encode("勿要多想,安心保胎吧"),
										pk::encode("你更重要,只能放弃了")
									};	
									int shengzi_2 = pk::choose(pk::encode("怎么看?"), shengzi_1);
									if (shengzi_2 == 0)
										pk::message_box(pk::encode("那么先帮我们的孩子起个名字吧!"), selected_person_);
									else
									{
										pk::message_box(pk::encode("天意如此,那也只好放弃了."), kunshu_);
										pk::message_box(pk::encode(pk::format("{}...", call_junzhu)), selected_person_);
										pk::fade(0);
										pk::background(-1);
										return;
									}
								}
								
							}
							else
							{
								switch (pk::rand(2))
								{
									case 0: 
										pk::play_bgm(19);
										pk::message_box(pk::encode(pk::format("{},告诉你一个好消息.", call_peiou)), kunshu_);
										pk::message_box(pk::encode("刚刚大夫替我把脉,说我有喜了!"), kunshu_);
									break;
									case 1: 
										pk::play_bgm(21);
										pk::message_box(pk::encode(pk::format("{},刚刚大夫替我把脉.\n大夫说我...我...\n呜呜呜...", call_peiou)), kunshu_);
										pk::message_box(pk::encode(pk::format("{}发生什么了?\n你先别哭,无论什么样的病,\n我一定找遍天下名医治好你!", call_junzhu)), selected_person_);
										pk::cutin(17);
										pk::message_box(pk::encode("大夫说我...我有喜了!"), kunshu_);
										pk::message_box(pk::encode("哦,吓死我,我还以为...\n嗯?不对!有喜了!?"), selected_person_);
										pk::message_box(pk::encode("哈哈哈,被我骗到了吧?"), kunshu_);
									break;
								}	
								pk::cutin(20);
								pk::message_box(pk::encode("有喜了是真...真的吗?"), selected_person_);
								pk::message_box(pk::encode(pk::format("是真的呢,\n\x1b[1x脉象圆滑流利,如盘走珠,\x1b[0x大夫如是说,\n还说我所怀的是个\x1b[1x{}\x1b[0x呢!", call_child)), kunshu_);
								pk::message_box(pk::encode(pk::format("太好了!\n我{}有{}了!我们有{}了!\n{},真的太感谢你了!",selected_person_name, call_child, call_child, call_junzhu)),  selected_person_);
								pk::play_bgm(19);
								pk::message_box(pk::encode("那我们的孩子起个什么名字呢?"), selected_person_);
							}

							setNewChild(@child, sex, @father, @mother, @kunshu_);
							pk::set_district(child, building_.get_district_id()); 
							string child_name = pk::decode(pk::get_name(child));

							/* pk::message_box(pk::encode(pk::format("{},我们的{}\x1b[1x{}\x1b[0x终于长大成人了.", call_junzhu, call_child, child_name)), selected_person_);
						
							pk::message_box(pk::encode(pk::format("{}大人,我已经长大了,可以帮助你了.", call_DadMom)), child);
							pk::message_box(pk::encode("是呀,你已经到了可以成为一名将帅的年龄了."), kunshu_);
							pk::message_box(pk::encode("我不会让你们失望的."), child);

							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x加入了我军.", child_name))); */  //测试
					
							pk::sort(pk::person::type_id);
							/* pk::move(child, building_); */
					
							pk::fade(0);
							pk::background(-1);
						}
						else
						{
							string call_junzhu = kunshu_.sex == 성별_남 ? "夫君" : "夫人";	//夫君/夫人
							pk::message_box(pk::encode(pk::format("{},今天就不要工作了,好好休息可好?", call_junzhu)), selected_person_);
							pk::message_box(pk::encode("偶尔休息下也挺好."), kunshu_);
							pk::fade(0);
							pk::background(-1);
						}
					}
					else
					{
						string call_junzhu = kunshu_.sex == 성별_남 ? "夫君" : "夫人";	//夫君/夫人
						pk::message_box(pk::encode(pk::format("{},今天就不要工作了,好好休息可好?", call_junzhu)), selected_person_);
						pk::message_box(pk::encode("偶尔休息下也挺好."), kunshu_);
					}

				}
				else if (random_n == 5)
				{
				    if (selected_person_.sex == 성별_남) 
					{
						pk::message_box(pk::encode("夫人,现在国家运作的怎么样?"), selected_person_);
						pk::message_box(pk::encode("还有好多问题没法解决."), kunshu_);
						pk::message_box(pk::encode("这是我和兄弟们凑的一些钱财来替国家分忧."), selected_person_);
						pk::message_box(pk::encode("谢谢夫君!"), kunshu_);
						
						pk::add_gold(building_, bonus_gold, true);	
						building_.update();
						
						pk::message_box(pk::encode("从夫君那获得了些金子."));
					}
					if (selected_person_.sex == 성별_여) 
					{
						pk::message_box(pk::encode("夫君现在国家运作的怎么样?"), selected_person_);
						pk::message_box(pk::encode("还有好多问题没法解决."), kunshu_);
						pk::message_box(pk::encode("这是妾身这些年下的一些积蓄."), selected_person_);
						pk::message_box(pk::encode("谢谢夫人!"), kunshu_);
						
						pk::add_gold(building_, bonus_gold, true);	
						building_.update();
						
						pk::message_box(pk::encode("从夫人那获得了些金子."));
					}
				}
				else 
				{
					int no = pk::rand(2);
					if ((no == 0) and (selected_person_.sex == 성별_여))
					{
						pk::message_box(pk::encode("夫君,我们偶尔也可以出去玩玩."), selected_person_);
						pk::message_box(pk::encode("好的,等我这阵忙完,我们出去好好放松下."), kunshu_);
					}
					else if ((no == 0) and (selected_person_.sex == 성별_남))
					{
						pk::message_box(pk::encode("夫人,最近天气不错,何不让我带你领略一下疆土的大好河山."), selected_person_);
						pk::message_box(pk::encode("好的,等我这阵忙完,我们出去好好放松下."), kunshu_);
					}						
					else if ((no != 0) and (selected_person_.sex == 성별_여))
					{
						pk::message_box(pk::encode("夫君,感谢你的陪伴.."), selected_person_);
						pk::message_box(pk::encode("感谢你在背后默默地支持我."), kunshu_);
					}
					else if ((no != 0) and (selected_person_.sex == 성별_남))
					{
						pk::message_box(pk::encode("夫人,多亏有你一直苦苦支撑这个国家.."), selected_person_);
						pk::message_box(pk::encode("这都多亏你的陪伴."), kunshu_);
					}						
				}				
                
				
				
				// 배우자 능력 원본으로 복구 
				selected_person_.base_stat[武将能力_智力] = 원본지력;

				// 화술 추가
				selected_person_.wajutsu_daikatsu = 원본화술_wajutsu_daikatsu;
				selected_person_.wajutsu_kiben = 원본화술_wajutsu_kiben;
				selected_person_.wajutsu_mushi = 원본화술_wajutsu_mushi;
				selected_person_.wajutsu_chinsei = 원본화술_wajutsu_chinsei;
				selected_person_.wajutsu_gyakujou = 원본화술_wajutsu_gyakujou;
				
				selected_person_.update();

			}

			pk::fade(0);
			pk::background(-1);	
				
		}
		
		
		void scene_jinshi_0()    //禁仕期减少
		{
			int kunshu_0 = kunshu_.get_id();
			if ( selected_person_.banned_kunshu != kunshu_0)  //禁仕君主ID不是玩家时
			{
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x对\x1b[1x{}\x1b[0x军似乎没什么成见.", selected_person_name, kunshu_name))); 
			}
			else if ( selected_person_.banned_kunshu == kunshu_0 and selected_person_.ban_timer > jinshi_jian)      //禁仕君主ID是玩家时
			{
				selected_person_.ban_timer = selected_person_.ban_timer - jinshi_jian;
				selected_person_.update();
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x对\x1b[1x{}\x1b[0x军的成见似乎降低了.", selected_person_name, kunshu_name))); 
			}
			else
			{
				selected_person_.banned_kunshu = -1;
				selected_person_.ban_timer = 0;	
				selected_person_.update();
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x对\x1b[1x{}\x1b[0x军的成见似乎完全消除了.", selected_person_name, kunshu_name))); 
			}
		}
		
		void scene_jinshi_1()     //禁仕期增加
		{
			int kunshu_0 = kunshu_.get_id();
			if ( selected_person_.banned_kunshu != kunshu_0)  //初定，说服失败会被追加禁仕期
			{
				selected_person_.banned_kunshu = kunshu_0;
				selected_person_.ban_timer = jinshi_jia;
			}
			else
			{
				selected_person_.ban_timer = selected_person_.ban_timer + jinshi_jia;
			}
			selected_person_.update();
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x对\x1b[1x{}\x1b[0x军的成见似乎加深了.", selected_person_name, kunshu_name))); 
		}
		
		void scene_callname()  //称呼
		{
			int kunshu_0 = kunshu_.get_id();
			int selected_person_0 = selected_person_.get_id();
			
			if (selected_person_.father == kunshu_0 or selected_person_.mother == kunshu_0)    //君主是父亲或者母亲
			{
				string last_name =pk::decode(selected_person_.mei.substr(selected_person_.mei.length() - 2 , selected_person_.mei.length()));   //最后一个字
				if (selected_person_.mei.length() > 2 and last_name != "儿")   //名字是两个字且最后一个字不是儿取最后一个字+儿
				{
					switch (pk::rand(2))
					{
						case 0: call_selected_person = pk::decode(selected_person_.mei.substr(selected_person_.mei.length() - 2 , selected_person_.mei.length())) + "儿";  break;
						case 1: call_selected_person = pk::decode(selected_person_.mei); break;
					}
				}
				else if(selected_person_.mei.length() > 2)
				{
					call_selected_person = pk::decode(selected_person_.mei);
				}
				else   //单字的直接+儿
				{
					call_selected_person = pk::decode(selected_person_.mei) + "儿";
				}
				if (force_.title == 0)
				{
					switch (pk::rand(5))
					{
						case 0: call_kunshu = kunshu_.sex == 성별_남 ? "父亲大人" : "母亲大人"; break;
						case 1: call_kunshu = kunshu_.sex == 성별_남 ? "父亲" : "母亲"; break;
						case 2: call_kunshu = kunshu_.sex == 성별_남 ? "爹爹" : "娘亲"; break;
						case 3: call_kunshu = kunshu_.sex == 성별_남 ? "父皇" : "皇上"; break;
						case 4: call_kunshu = "陛下"; break;
					}
					switch (pk::rand(3))
					{
						case 0: self_selected_person = "孩儿";  break;
						case 1: self_selected_person = selected_person_.sex == 성별_남 ? "孩儿" : "女儿"; break;
						case 2: self_selected_person = "儿臣";  break;
					}
				}
				else if (force_.title == 1)
				{
					switch (pk::rand(4))
					{
						case 0: call_kunshu = kunshu_.sex == 성별_남 ? "父亲大人" : "母亲大人"; break;
						case 1: call_kunshu = kunshu_.sex == 성별_남 ? "父亲" : "母亲"; break;
						case 2: call_kunshu = kunshu_.sex == 성별_남 ? "爹爹" : "娘亲"; break;
						case 3: call_kunshu = kunshu_.sex == 성별_남 ? "父王" : "王上"; break;
					}
					switch (pk::rand(2))
					{
						case 0: self_selected_person = "孩儿";  break;
						case 1: self_selected_person = selected_person_.sex == 성별_남 ? "孩儿" : "女儿"; break;
					}
				}
				else
				{
					switch (pk::rand(3))
					{
						case 0: call_kunshu = kunshu_.sex == 성별_남 ? "父亲大人" : "母亲大人"; break;
						case 1: call_kunshu = kunshu_.sex == 성별_남 ? "父亲" : "母亲"; break;
						case 2: call_kunshu = kunshu_.sex == 성별_남 ? "爹爹" : "娘亲"; break;
					}
					switch (pk::rand(2))
					{
						case 0: self_selected_person = "孩儿";  break;
						case 1: self_selected_person = selected_person_.sex == 성별_남 ? "孩儿" : "女儿"; break;
					}
				}

				switch (pk::rand(2))
				{
					case 0: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,您怎么来了?", call_kunshu)), selected_person_);  break;
					case 1: 
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x给\x1b[1x{}\x1b[0x问安.", self_selected_person, call_kunshu)), selected_person_); 
						pk::message_box(pk::encode("恩,好好好."), kunshu_);
					break;
				}

			}
			
			else if (kunshu_.father == selected_person_0 or kunshu_.mother == selected_person_0)  //访问父亲或者母亲
			{
				if (force_.title == 0)
				{
					call_kunshu = "皇帝";
				}
				else
				{
					if (kunshu_.mei.length() > 2)   //名字是两个字取最后一个字+儿
					{
						switch (pk::rand(3))
						{
							case 0: call_kunshu = pk::decode(kunshu_.mei.substr(kunshu_.mei.length() - 2 , kunshu_.mei.length())) + "儿";  break;
							case 1: call_kunshu = pk::decode(kunshu_.mei); break;
							case 2: call_kunshu = "我儿"; break;
						}
					}
					else   //单字的直接+儿
					{
						switch (pk::rand(2))
						{
							case 0: call_kunshu = pk::decode(kunshu_.mei) + "儿"; break;
							case 1: call_kunshu = "我儿"; break;
						}
					}

				}
				switch (pk::rand(2))
				{
					case 0: call_selected_person = selected_person_.sex == 성별_남 ? "父亲大人" : "母亲大人"; break;
					case 1: call_selected_person = selected_person_.sex == 성별_남 ? "父亲" : "母亲"; break;
				}
				switch (pk::rand(2))
				{
					case 0: self_selected_person = "我";  break;
					case 1: self_selected_person = selected_person_.sex == 성별_남 ? "为父" : "为娘"; break;
				}
				pk::message_box(pk::encode(pk::format("孩儿给\x1b[1x{}\x1b[0x问安.", call_selected_person)), kunshu_); 
				pk::message_box(pk::encode(pk::format("恩,好好好.\n\x1b[1x{}\x1b[0x,你怎么来了?", call_kunshu)), selected_person_);
			}
			
			else if (pk::is_gikyoudai(kunshu_, selected_person_0))  //访问结兄弟
			{
				//刘关张特殊待遇，ID和名字都要对应
				if (kunshu_0 == 武将_刘备 && kunshu_name == "刘备" && selected_person_0 == 武将_关羽 && selected_person_name == "关羽")
				{
					call_selected_person = pk::rand(2) == 1 ? "二弟" : "云长";
					if (force_.title == 0)
					{
						call_kunshu = pk::rand(2) == 1 ? "陛下" : "兄长";
						self_selected_person = pk::rand(2) == 1 ? "臣" : "二弟";
					}
					else
					{
						call_kunshu = pk::rand(2) == 1 ? "兄长" : "大哥";
						self_selected_person = pk::rand(2) == 1 ? "关某" : "我";
					}
				}
				else if (kunshu_0 == 武将_刘备 && kunshu_name == "刘备" && selected_person_0 == 武将_张飞 && selected_person_name == "张飞")
				{
					call_selected_person = pk::rand(2) == 1 ? "三弟" : "翼德";
					if (force_.title == 0)
					{
						call_kunshu = pk::rand(2) == 1 ? "陛下" : "大哥";
						self_selected_person = pk::rand(2) == 1 ? "臣" : "三弟";
					}
					else
					{
						call_kunshu = pk::rand(2) == 1 ? "哥哥" : "大哥";
						self_selected_person = pk::rand(2) == 1 ? "俺老张" : "三弟";
					}
				}
				
				else if (selected_person_.sex == 성별_남)  //男性
				{
					if (kunshu_.gikyoudai == kunshu_0)  //君主是大哥
					{
						call_selected_person = selected_person_.azana != "" ? pk::decode(selected_person_.azana) : "贤弟";
						if (force_.title == 0)
						{
							call_kunshu = "陛下";
							self_selected_person = "臣";
						}
						else
						{
							call_kunshu = kunshu_.sex == 0 ? "兄长" : "姐姐";   //异性结兄弟。。。
							self_selected_person = pk::rand(2) == 1 ? "我" : "小弟";
						}
					}
					else
					{
						call_selected_person = selected_person_.azana != "" ? pk::decode(selected_person_.azana) : "兄长";
						if (force_.title == 0)
						{
							call_kunshu = "陛下";
							self_selected_person = "臣";
						}
						else
						{
							call_kunshu = kunshu_.sex == 0 ? "贤弟" : "妹妹";   //异性结兄弟。。。
							self_selected_person = pk::rand(2) == 1 ? "愚兄" : "我";
						}
					}
						
				}
				else  //女性
				{
					if (kunshu_.gikyoudai == kunshu_0)
					{
						call_selected_person = selected_person_.azana != "" ? pk::decode(selected_person_.azana) : "妹妹";
						if (force_.title == 0)
						{
							call_kunshu = "陛下";
							self_selected_person = "臣";
						}
						else
						{
							call_kunshu = kunshu_.sex == 0 ? "兄长" : "姐姐";   //异性结兄弟。。。
							self_selected_person = pk::rand(2) == 1 ? "我" : "小妹";
						}
					}
					else
					{
						call_selected_person = selected_person_.azana != "" ? pk::decode(selected_person_.azana) : "姐姐";
						if (force_.title == 0)
						{
							call_kunshu = "陛下";
							self_selected_person = "臣";
						}
						else
						{
							call_kunshu = kunshu_.sex == 0 ? "贤弟" : "妹妹";   //异性结兄弟。。。
							self_selected_person = pk::rand(2) == 1 ? "阿姐" : "我";
						}
					}
				}
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,您怎么来了?", call_kunshu)), selected_person_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x最近还好吗?", call_selected_person)), kunshu_); 
			}
			
			else if ((kunshu_.father != -1 and kunshu_.father == selected_person_.father) or (kunshu_.mother != -1 and kunshu_.mother == selected_person_.mother))  //访问的是兄弟姐妹
			{
				if (kunshu_.birth <= selected_person_.birth)  //君主大
				{
					call_selected_person = selected_person_.sex == 성별_남 ? "弟弟" : "妹妹";
					call_kunshu = kunshu_.sex == 성별_남 ? "兄长" : "姐姐";
					self_selected_person = selected_person_.sex == 성별_남 ? "弟弟" : "妹妹";
				}
				else  //君主小
				{
					call_selected_person = selected_person_.sex == 성별_남 ? "兄长" : "姐姐";
					call_kunshu = kunshu_.sex == 성별_남 ? "弟弟" : "妹妹";
					self_selected_person = selected_person_.sex == 성별_남 ? "愚兄" : "阿姐";
				}
			}
			
			else if (kunshu_.ketsuen == selected_person_.ketsuen)  //血缘太复杂了。。
			{
				if (kunshu_.birth <= selected_person_.birth)  //君主大
				{
					call_selected_person = selected_person_.azana != "" ? pk::decode(selected_person_.azana) : "您";
					call_kunshu = force_.title == 0 ? "陛下" : "主公";
					self_selected_person = force_.title == 0 ? "臣" : "我";
				}
				else  //君主小
				{
					call_selected_person = "您";
					call_kunshu = kunshu_.azana != "" ? pk::decode(selected_person_.azana) : "主公";
					self_selected_person = force_.title == 0 ? "臣" : "我";
				}
			}
			
			else if (selected_person_0 == force_.gunshi)  //访问者是军师
			{
				call_selected_person = selected_person_.azana != "" ? pk::decode(selected_person_.azana) : "军师";
				call_kunshu = force_.title == 0 ? "陛下" : "主公";
				self_selected_person = force_.title == 0 ? "臣" : "在下";
				switch (pk::rand(2))
				{
					case 0: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,您怎么来了?", call_kunshu)), selected_person_); break;
					case 1: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x特意来访,是有什么事吗?", call_kunshu)), selected_person_); break;
				}
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x最近还好吗?", call_selected_person)), kunshu_); 
			}
			
			else if (pk::is_like(selected_person_, kunshu_0) || pk::is_like(kunshu_, selected_person_0))  //访问者是亲爱
			{
				call_selected_person = selected_person_.azana != "" ? pk::decode(selected_person_.azana) : "阁下";
				call_kunshu = force_.title == 0 ? "陛下" : "主公";
				self_selected_person = force_.title == 0 ? "臣" : "我";
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,您怎么来了?", call_kunshu)), selected_person_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x最近还好吗?", call_selected_person)), kunshu_); 
			}

			else
			{
				switch(force_.title)
				{
					case 0 : call_kunshu = "陛下"; break;
					case 1 : call_kunshu = "王上"; break;
					case 2 : call_kunshu = pk::encode(kunshu_.sei) + "公"; break;
					default :  call_kunshu = "主公";
				}
				// return call_kunshu;
				call_selected_person = selected_person_.sex == 성별_남 ? "阁下" : "您";
				self_selected_person = selected_person_.sex == 성별_남 ? "我" : "在下";
				switch (pk::rand(2))
				{
					case 0: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,来我这寒舍有何贵干?", call_kunshu)), selected_person_); break;
					case 1: pk::message_box(pk::encode(pk::format("哎呀呀,\x1b[1x{}\x1b[0x大驾光临,有失远迎.", call_kunshu)), selected_person_); break;
				}
			}
		}
		
		int scene_malist1(int horse_i, int horse_j)   //商人买马列表1
		{
			int ma_list;
			array<string> ma_list1 =
			{
				pk::encode("这匹马有些.."),
				pk::encode(pk::format("这匹{}..", horse1_name[horse_i])),
				pk::encode(pk::format("这匹{}..", horse1_name[horse_j]))
			};
			ma_list = pk::choose(pk::encode("相中哪一匹马呢?"), ma_list1);
			return ma_list;
		}
		
		//马的编号
		int horseNum(int num)
		{
			int horse_num = 100;  //初始化为普通马的编号

			if (num == 0)
			{
				horse_num = 100; //普通马的编号
				//读取马的背景图片
				pk::background("media/background/horse/h0.png");
				pk::message_box(pk::encode("这匹看起来很特别.."), selected_person_);
				pk::message_box(pk::encode("您真有眼光!\n这匹\x1b[1x红鬃马\x1b[0x可是难得一遇的良驹呀."), shangren_0);
			}
			else if (num == 1)
			{
				horse_num = 1; //赤兔的编号
				//读取马的背景图片
				pk::background("media/background/horse/h1.png");
				pk::message_box(pk::encode("这匹看起来很特别.."), selected_person_);
				pk::message_box(pk::encode("您真有眼光!\n这匹\x1b[1x汗血马\x1b[0x可是难得一遇的良驹呀."), shangren_0);
				pk::message_box(pk::encode("身形雄壮,流汗如血.\n因而得名「\x1b[1x汗血宝马\x1b[0x」."), shangren_0);
			}
			else if (num == 2)
			{
				horse_num = 6; //的卢的编号
				//读取马的背景图片
				pk::background("media/background/horse/h2.png");
				pk::message_box(pk::encode("这匹看起来很特别.."), selected_person_);
				pk::message_box(pk::encode("您真有眼光!\n这匹\x1b[1x灰影马\x1b[0x可是难得一遇的良驹呀."), shangren_0);
				pk::message_box(pk::encode("尤其善于夜行."), shangren_0);
			}
			else if (num == 3)
			{
				horse_num = 11;   //绝影的编号
				//读取马的背景图片
				pk::background("media/background/horse/h3.png");
				pk::message_box(pk::encode("这匹看起来很特别.."), selected_person_);
				pk::message_box(pk::encode("您真有眼光!\n这匹\x1b[1x黄骠马\x1b[0x可是难得一遇的良驹呀."), shangren_0);
				pk::message_box(pk::encode("即使喂饱了草料,马的肋条常显露在外,\n因此另有别名「\x1b[1x透骨龙\x1b[0x」."), shangren_0);
			}
			else if (num == 4)
			{
				horse_num = 26;   //爪黄飞电的编号
				//读取马的背景图片
				pk::background("media/background/horse/h4.png");
				pk::message_box(pk::encode("这匹看起来很特别.."), selected_person_);
				pk::message_box(pk::encode("您真有眼光!\n这匹\x1b[1x白义马\x1b[0x可是难得一遇的良驹呀."), shangren_0);
				pk::message_box(pk::encode("产于\x1b[1x幽燕\x1b[0x之地,披毛亮白,日行千里."), shangren_0);
			}
			else if (num == 5)
			{
				horse_num = 3;   //大宛马的编号
				//读取马的背景图片
				pk::background("media/background/horse/h5.png");
				pk::message_box(pk::encode("这匹看起来很特别.."), selected_person_);
				pk::message_box(pk::encode("您真有眼光!\n这匹\x1b[1x骅骝马\x1b[0x可是难得一遇的良驹呀."), shangren_0);
				pk::message_box(pk::encode("\x1b[1x庄子云,骐骥骅骝,一日而驰千里.\x1b[0x"), shangren_0);
			}
			return horse_num;
		}
		
		void scene_qinai()   //成为亲爱武将
		{
			int i = 0;
			while(i < 5)
			{
				if (selected_person_.liked[i] == -1)
				{
					selected_person_.liked[i] = force_.kunshu;
					selected_person_.update();
					break;
				}
				i++;
			}
		
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x成为了\x1b[1x{}\x1b[0x的亲爱武将.", kunshu_name, selected_person_name))); 
		}
		
		
		void scene_choose_list_1()  //师事选择菜单1
		{
			pk::message_box(pk::encode("嗯嗯,不错不错,那我遵守承诺教你一些..."), selected_person_);
			pk::message_box(pk::encode("那真是太感谢您了."), kunshu_);
			pk::fade(0);
			pk::background(40); 
			pk::fade(255);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x向\x1b[1x{}\x1b[0x讨教了十天后...", kunshu_name, selected_person_name))); 
			pk::message_box(pk::encode("听先生一席话,我感觉受益匪浅."), kunshu_);
			array<string> list_1 =
			{
				pk::encode("统帅"),
				pk::encode("武力"),						
				pk::encode("智力"),
				pk::encode("政治"),						
				pk::encode("魅力")
			};
			array<string> list_1_0 = {"统帅", "武力", "智力", "政治", "魅力"};
		
			int n = pk::choose(pk::encode("选择提升的属性"), list_1);
			
			int exp_shi_0 = int(selected_person_.stat[n] - kunshu_.stat[n]) > exp_shi ? int(selected_person_.stat[n] - kunshu_.stat[n]) * exp_shi_beilv / 100 : exp_shi;
			//pk::trace(pk::format("exp_shi_0等于{}", exp_shi_0));
			//selected_person_.stat[n] - kunshu_.stat[n]  前需要加上int才能正确运行
			pk::add_stat_exp(kunshu_, n, exp_shi_0);
			kunshu_.update();
			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}{}\x1b[0x经验上升了\x1b[1x{}\x1b[0x .", kunshu_name, list_1_0[n], exp_shi_0))); 
		}
		
		
		pk::list<pk::person@> getTargetList()
		{
			pk::list<pk::person@> list;
			int force_id = force_.get_id();
			int kunshu_0 = kunshu_.get_id();
			for (int i = 0; i < 무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				int person_0 = person.get_id();
				if (pk::is_alive(person))
				{
					if (person.get_force_id() == force_id)
					{
						if ( !pk::is_absent(person) and !pk::is_unitize(person) 
							 and person.mibun != 신분_포로 
						     and person.mibun != 身份_君主
							 and !pk::is_dislike(person, kunshu_0)
							 and !pk::is_dislike(kunshu_, person_0)
							 and person.location == kunshu_.service)
							list.add(person);
					}
					if(person.mibun == 신분_재야
						and !pk::is_dislike(person, kunshu_0)
						and person.location == kunshu_.service)
					{
						list.add(person); 
					}     //拜访在野武将
					if(person.mibun == 신분_포로
						and !pk::is_dislike(person, kunshu_0)
						and person.location == kunshu_.service)     
					{
						list.add(person);
					}   //俘虏拜访
					
				}
			}
			
			//武将排序
			list.sort(function(a, b)
			{
				return a.stat[武将能力_武力] > b.stat[武将能力_武力];   //先按武力排序
			});
			list.sort(function(a, b)
			{
				return a.mibun < b.mibun;//再按身份排序
			});

			return list;
		}


		pk::person@ getEmptyChild(int mother_id)
		{
			int count_children = 0;
			for (int i = 999; i >= 850; i--)
			{
				pk::person@ child = pk::get_person(i);
				if (child.mother == mother_id) count_children++;
				if (count_children >= 女性生子上限)
				{
					//ch::u8debug("already have to many children!");
					return null;
				}
				switch (child.mibun)
				{
				case 신분_없음:
				//case 신분_미등장:
				case 신분_사망:
					return child;
				}
			}
			return null;	
		}


		int setNewChild(pk::person@ child, int sex, pk::person@ father, pk::person@ mother, pk::person@ junzhu)
		{
			
			child.sei = kunshu_.sei; // 姓
			
			//取名
			array<string> name_list1 ={ pk::encode("  随机起  "), pk::encode("  亲自起  ")};
			int name_list = pk::choose(pk::encode("决定要怎么样起名呢?"), name_list1);
			
			if (name_list == 0)
			{
				array<string> name_num1 ={ pk::encode("  单字名  "), pk::encode("  双字名  ")};  

				int name_num = pk::choose(pk::encode("单字还是双字呢?"), name_num1);  
				if (name_num == 0)
				{
					bool next = false;
				
					while (!next)
					{
						child.mei = pk::get_family_name(random(성씨_끝 - 1)).name; // 名
						next = pk::yes_no(pk::encode(pk::format("确定起名为\x1b[1x{}{}\x1b[0x吗?\n*若想选择\x1b[29x否\x1b[0x,可点击滑鼠右键*", pk::decode(child.sei), pk::decode(child.mei))) ); 
					}
					
				}
				else
				{
					bool next_0 = false;
					bool next_1 = false;
					string child_mei_0;
					string child_mei_1;
				
					while (!next_0)
					{
						child_mei_0 = pk::get_family_name(random(성씨_끝 - 1)).name; // 名的第一个字
						next_0 = pk::yes_no(pk::encode(pk::format("确定第一个字为\x1b[1x{}\x1b[0x吗?\n*若想选择\x1b[29x否\x1b[0x,可点击滑鼠右键*", pk::decode(child_mei_0))) ); 
					}
					while (!next_1)
					{
						child_mei_1 = pk::get_family_name(random(성씨_끝 - 1)).name; // 名的第二个字
						next_1 = pk::yes_no(pk::encode(pk::format("确定第二个字为\x1b[1x{}\x1b[0x吗?\n*若想选择\x1b[29x否\x1b[0x,可点击滑鼠右键*", pk::decode(child_mei_1))) ); 
					}
					child.mei = child_mei_0 + child_mei_1;
					
				}
			}
			else
			{
				if (sex == 0)
				{
					int min_value = 0;
					int max_value = j;
					int def_value = 0;
					int pause = int(pk::option["MessagePause"]);
					pk::option["MessagePause"] = 100;
					
					bool next = false;
					
					while (!next)
					{
						pk::background("media/background/baifang/name1.png");
						//pk::message_box(pk::encode("0-9  海,嘉,宁,超,统,真,建,允,休,智\n10-19丰,艾,平,文,楚,通,飞,康,良,明\n20-29尚,青,德,进,岱,正,亮,忠,仪,广\n30-39云,延,威,昭,和,齐,雄,信,义,志\n40-49维,逊,柔,顺,渊,定,济,攸,肃,胜\n50-59兰,睿,度,修,浩,芝,成,朗,英,兴\n60-69远,常,长,台,约,衡,龙,让,伟,春\n70-79夏,秋,冬,祖,桓,符,谋,昂,舒,山\n80-89先,翼,起,升,霸,孝,直,绩,谦,才\n90-99俊,方,彦,烈,承,达,宣,高,至,业"), selected_person_);
						def_value = pk::numberpad( pk::encode("起名"), min_value, max_value, def_value, pk::numberpad_t(pad_callback)).first;
						next = pk::yes_no(pk::encode(pk::format("确定起名为\x1b[1x{}{}\x1b[0x吗?\n*若想选择\x1b[29x否\x1b[0x,可点击滑鼠右键*", pk::decode(child.sei), mei_남[def_value])) ); 
					}
					child.mei = pk::encode(mei_남[def_value]); // 名

					pk::option["MessagePause"] = pause;
				}
				else
				{
					int min_value = 0;
					int max_value = k;
					int def_value = 0;
					int pause = int(pk::option["MessagePause"]);
					pk::option["MessagePause"] = 100;
					
					bool next = false;
					
					while (!next)
					{
						pk::background("media/background/baifang/name2.png");
						//pk::message_box(pk::encode("0-4媛,蕊,嫣然,姬,兰香\n5-9圆圆,芙蓉,婉君,玉华,雪\n10-14香香,飞燕,玉环,若兰,小小\n15-19灵儿,丽华,婕妤,诗诗,琬\n20-24梅,杏儿,春华,秋月,芳\n25-29玉英,玉兰,惠,佩,文君\n30-34妙,凤,巧巧,珠,雨荷\n35-39如月,玉珠,燕燕,玉,莺"), selected_person_);
						def_value = pk::numberpad( pk::encode("起名"), min_value, max_value, def_value, pk::numberpad_t(pad_callback)).first;
						next = pk::yes_no(pk::encode(pk::format("确定起名为\x1b[1x{}{}\x1b[0x吗?\n*若想选择\x1b[29x否\x1b[0x,可点击滑鼠右键*", pk::decode(child.sei), mei_여[def_value])) ); 
					}
					child.mei = pk::encode(mei_여[def_value]); // 名
					
					pk::option["MessagePause"] = pause;
				}
			}
			pk::fade(0);
			pk::background(5); 
			pk::fade(255);
			pk::message_box(pk::encode(pk::format("就叫\x1b[1x{}{}\x1b[0x吧!", pk::decode(child.sei), pk::decode(child.mei))), kunshu_);
			pk::message_box(pk::encode(pk::format("真是个好名字!\n我们的\x1b[1x{}{}\x1b[0x一定会成长为一个不凡之人.", pk::decode(child.sei), pk::decode(child.mei))), selected_person_);
			
			//测试是否要怀胎十月，因为如果君主是女性的话，就要很长一段时间不能动
			if (kunshu_.sex == 0)
			{
				selected_person_.absence_timer = 29;     //怀胎十月
				selected_person_.target[0] = -1;     //预留暂时不知道怎么调出目标是小孩
				selected_person_.order = 40;
				selected_person_.action_done = true;
				selected_person_.update();
				pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x怀孕了,在府中安胎.", selected_person_name)));
			}
			else
			{
				kunshu_.absence_timer = 29;     //怀胎十月
				kunshu_.target[0] = -1;   //预留暂时不知道怎么调出目标是小孩
				kunshu_.order = 40;
				kunshu_.action_done = true;
				kunshu_.update();
				pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x怀孕了,在府中安胎.", kunshu_name)));
			}
			
			//取字
			array<string> azana_list1 ={ pk::encode("  不取字  "), pk::encode("  随机取  "), pk::encode("  手动取  ")};  
			int azana_list = pk::choose(pk::encode("要给孩子取字号吗?"), azana_list1);
			
			if (azana_list == 0)
			{
				child.azana = " ";
			}	
			
			else if (azana_list == 1)
			{
				bool next_2 = false;
				bool next_3 = false;
				string child_azana_0;
				string child_azana_1;
			
				while (!next_2)
				{
					child_azana_0 = pk::get_family_name(random(성씨_끝 - 1)).name; // 字的第一个字
					next_2 = pk::yes_no(pk::encode(pk::format("确定第一个字为\x1b[1x{}\x1b[0x吗?\n*若想选择\x1b[29x否\x1b[0x,可点击滑鼠右键*", pk::decode(child_azana_0))) ); 
				}
				while (!next_3)
				{
					child_azana_1 = pk::get_family_name(random(성씨_끝 - 1)).name; // 字的第二个字
					next_3 = pk::yes_no(pk::encode(pk::format("确定第二个字为\x1b[1x{}\x1b[0x吗?\n*若想选择\x1b[29x否\x1b[0x,可点击滑鼠右键*", pk::decode(child_azana_1))) ); 
				}
				child.azana = child_azana_0 + child_azana_1;
				pk::message_box(pk::encode(pk::format("就叫\x1b[1x{}{}\x1b[0x吧!", pk::decode(child_azana_0), pk::decode(child_azana_1))), kunshu_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}{}{}\x1b[0x,\n听着还不错呢.", pk::decode(child.sei), pk::decode(child_azana_0), pk::decode(child_azana_1))), selected_person_);
			}
			else
			{
				int min_value = 0;
				int max_value_0 = l;
				int def_value_0 = 0;
 				string child_azana_2;
				
				int pause = int(pk::option["MessagePause"]);
				pk::option["MessagePause"] = 100;
				
				bool next_4 = false;
				bool next_5 = false;
				
				while (!next_4)
				{
					pk::message_box(pk::encode("0-9  伯,仲,叔,季,幼,早,孟,显,惠,雅\n10-19陆,德,仁,少,公,子,弘,平,文,长\n20-29奉,士,元,玄,守,兴,正,曼,义,汉"), selected_person_);
					def_value_0 = pk::numberpad( pk::encode("取字"), min_value, max_value_0, def_value_0, pk::numberpad_t(pad_callback)).first;
					next_4 = pk::yes_no(pk::encode(pk::format("确定第一个字为\x1b[1x{}\x1b[0x吗?\n*若想选择\x1b[29x否\x1b[0x,可点击滑鼠右键*", azana_0[def_value_0])) ); 
					
				}
				pk::option["MessagePause"] = pause;
				while (!next_5)
				{
					child_azana_2 = pk::get_family_name(random(성씨_끝 - 1)).name; // 字的第二个字
					next_5 = pk::yes_no(pk::encode(pk::format("确定第二个字为\x1b[1x{}\x1b[0x吗?\n*若想选择\x1b[29x否\x1b[0x,可点击滑鼠右键*", pk::decode(child_azana_2))) ); 
				}
				child.azana = pk::encode(azana_0[def_value_0]) + child_azana_2; 
				pk::message_box(pk::encode(pk::format("就叫\x1b[1x{}\x1b[0x吧!", pk::decode(child.azana))), kunshu_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}{}{}\x1b[0x,\n听着还不错呢.", pk::decode(child.sei), azana_0[def_value_0], pk::decode(child_azana_2))), selected_person_);
			}
			
			//测试结束
			
			

			//child.azana = "";		//字   
			//child.name_read = "";	//姓名讀音
			child.azana_read = "";	//字讀音

			//  基本信息
			child.appearance = pk::get_month() <= 2 ? pk::get_year() + 7 : pk::get_year() + 8;   //登场年     8岁登场测试
			if (pk::get_month() <= 2 or (pk::get_month() == 3 and pk::get_day() <= 11))    //出生年当年出生     测试
			{
				child.birth = pk::get_year();   
			}
			else  //次年出生 测试
			{
				child.birth = pk::get_year() + 1;   
			}
			// child.birth = pk::get_month() <= 2 ? pk::get_year() : pk::get_year() + 1;   //出生年     测试
			child.death = child.birth + 50 + random(20);    //死亡年
			child.ketsuen = father.get_id() == kunshu_.get_id() ? father.ketsuen : mother.ketsuen;   //血缘  修改为君主的血缘
			child.generation = father.get_id() == kunshu_.get_id() ? father.generation + 1 : mother.generation + 1;  //世代  修改为君主世代（世代似乎最多是9）
			child.aishou = random(junzhu.aishou-10, junzhu.aishou+10);   //相性
			child.mibun = 신분_미등장;     //身fen未登场
			child.loyalty = 51;     //忠诚度作为计数器    测试
			child.ban_timer = 10;    //禁仕时间作为计数器   测试
			child.absence_timer = 0;    //不在期间作为计数器   测试
			child.service = kunshu_.service;   //所属为君主当前所在城市
			child.location = kunshu_.service;   //所在为君主当前所在城市
			child.kouseki = 0;  ////功績作为生日计时器 初始化
			child.spouse = -1;    //配偶
			child.gikyoudai = -1;   //义兄弟
			child.shoubyou = 0;   //伤病
			child.estimated_death = false;   //死亡预订
			//生日生成器
			if (pk::get_month() <= 2 or (pk::get_month() == 3 and pk::get_day() <= 11))
			{
				child.kouseki += (pk::get_month() + 9) * 100;
			}
			else if (pk::get_month() >= 4 or (pk::get_month() == 3 and pk::get_day() == 21))
			{
				child.kouseki += (pk::get_month() - 3) * 100;
			}
			if (pk::get_day() == 1)
			{
				child.kouseki += pk::get_day() + random(10, 17);
			}
			else if (pk::get_day() == 11)
			{
				child.kouseki += pk::get_day() + random(10, 17);
			}
			else if (pk::get_day() == 21)
			{
				child.kouseki += pk::get_day()+ random(80, 87);
			}
			//child.kouseki = pk::get_month() <= 2 ? (pk::get_month() + 10) * 100 + pk::get_day() : (pk::get_month() - 2) * 100 + pk::get_day();	//功績    //作为生日计时器
			child.father = father.get_id();
			child.mother = mother.get_id();
			//	?空亲爱厌恶关系
			for (i=0;i<5;i++)
			{
				child.liked[i] = -1;
				child.disliked[i] = -1;
			}
			child.liked[0] = father.get_id(); //亲爱父母
			child.liked[1] = mother.get_id();

			child.birthplace = random(주_유주, 주_남중);     //出生地
			child.wadai = random(화제_고사, 화제_시절);      //特长話題
			child.giri = random(의리_매우낮음, 의리_매우높음);       //义理
			child.ambition = random(야망_매우낮음, 야망_매우높음);   //野心
			child.promotion = random(기용_능력, 기용_임의);  //任用
			child.character = random(성격_소심, 성격_저돌);  //性格
			child.kanshitsu = random(한실_무시, 한실_중시);  //汉室态度
			child.strategic_tendency = random(전략경향_중화통일, 전략경향_현상유지); //战略傾向
			child.local_affiliation = random(지역집착_지역중시, 지역집착_지역무시);  //地缘

			//  与性別相关
			child.sex = sex;  // 0-男，1-女
			
			int father_type = getPersonType(@father), mother_type = getPersonType(@mother);
			int child_type;

			if (pk::rand_bool(50))	//75%和父母类型相同
			{
				if (sex == 0)	child_type = father_type;
				else			child_type = mother_type;
			}
			else if (pk::rand_bool(50))	//75%和父母类型相同
			{
				if (sex == 0)	child_type = mother_type;
				else			child_type = father_type;
			}
			else
				child_type = random(0, 5);
			
			
			array<int> range_能力_max = {0,0,0,0,0};
			array<int> range_能力_min = {0,0,0,0,0};
			int range_适性 = 2;
			int buff_下限 = 0, buff_上限 = 0, buff_适性 = 0, chance_話术 = 0;
			array<int> relative_stat = {0,0,0,0,0};	//相对五维数?
			
			int face_color = 0;	// 欧皇：2，非酋：1，翹楚：4
			int 欧皇率 = 基础欧皇率, 非酋率 = 基础非酋率;
			

			//  看face时刻
			if (pk::rand_bool(欧皇率))  //欧皇
			{
				buff_下限 += 15;
				buff_上限 += 5;
				buff_适性 += 2;
				chance_話术 += 10;
				face_color += 2;
				//ch::u8debug("ou huang");
			}
			if (pk::rand_bool(非酋率))	//非酋
			{
				buff_下限 -= 10;
				buff_上限 -= 10;
				buff_适性 -= 2;
				chance_話术 -= 10;
				face_color += 1;
				//ch::u8debug("fei qiu");
			}
			if (pk::rand_bool(5))  //极低机率全能人才
			{
				child_type = 6;
				face_color += 4;
				//ch::u8debug("qiao chu");
			}
			
			//头像根据类型而定
			if (sex == 0)
			{
				bool wujiang = true;  //类型为武将初始化 
				array<int> face_son;
				if (child_type == 0 || child_type == 1 || child_type == 6)  //帅才、猛将、翘楚时调用武官头像
					face_son = {1407, 1408, 1410, 1415, 1416, 1419, 1420, 1422, 1423, 1424, 1427, 1428, 1431, 1434, 1435, 1438, 1440, 1442, 1446, 1450, 1455, 1459, 1462, 1464, 1466, 1467, 1470, 1476};   //儿子头像
					
				else  //军师、文臣、谋士、名人调用文官头像
				{
					wujiang = false;
					face_son = {1302, 1303, 1304, 1305, 1311, 1314, 1315, 1318, 1319, 1320, 1321, 1322, 1323, 1326, 1327, 1328, 1329, 1332, 1334, 1335, 1339, 1342, 1344};   //儿子头像
				}
				
				//判定头像是否重复
				int child_face = face_son[random(0, face_son.length - 1)];
				int i = 0;
				while(HaveSameFace(child_face) and i < 5)   //随机多次仍重复则跳过，防止死循环
				{
					child_face = face_son[random(0, face_son.length - 1)];
					i++;
				}
				child.face = child_face;
				//child.face = face_son[random(0, face_son.length - 1)];    
				// child.face = random(2025, 2043); //头像
				child.voice = random(음성_소심남, 음성_저돌남);  //声音-男
				child.tone = random(말투_장비, 말투_보통남);     //語气-男
				//  武将造型
				if (wujiang)  //为武将
				{
					array<int> child_body = {75, 76, 84, 85, 93, 94, 102, 103, 111, 112, 120, 121};  //武官的基本造型
					int rd_n = child_body[pk::rand(12)];
					child.body[0] = rd_n;
					child.body[1] = rd_n;
					child.body[2] = rd_n;
					child.body[3] = rd_n;
					child.body[4] = rd_n;
					child.body[5] = rd_n;
				}
				else
				{
					array<int> child_body = {79, 80, 88, 89, 97, 98, 106, 107, 115, 116, 124, 125};  //文官的基本造型
					int rd_n = child_body[pk::rand(12)];
					child.body[0] = rd_n;
					child.body[1] = rd_n;
					child.body[2] = rd_n;
					child.body[3] = rd_n;
					child.body[4] = rd_n;
					child.body[5] = rd_n;
				}
			}
			else
			{
				bool wujiang = true;  //类型为武将初始化 
				array<int> face_daughter;
				if (child_type == 0 || child_type == 1 || child_type == 6)  //帅才、猛将、翘楚时调用武官头像
					face_daughter = {1701, 1702, 1704, 1709, 1711, 1713, 1715, 1716, 1718, 1719, 1721, 1722, 1726, 1727, 1730, 1732};    //女儿头像
				else  //军师、文臣、谋士、名人调用文官头像
				{
					wujiang = false;
					face_daughter = {1600, 1604, 1605, 1607, 1608, 1609, 1610, 1611, 1612, 1614, 1615, 1617, 1618, 1623, 1624, 1625};    //女儿头像
				}
				
				//判定头像是否重复
				int child_face = face_daughter[random(0, face_daughter.length - 1)];
				int i = 0;
				while(HaveSameFace(child_face) and i < 5)   //随机多次仍重复则跳过，防止死循环
				{
					child_face = face_daughter[random(0, face_daughter.length - 1)];
					i++;
				}
				child.face = child_face;
				//child.face = face_daughter[random(0, face_daughter.length - 1)];   
				// child.face = random(2116, 2144); //头像
				child.voice = random(음성_냉정녀, 음성_대담녀);   //声音-女
				child.tone = random(말투_고대만족, 말투_보통녀);  //語气-女
				//  武将造型
				child.skeleton = 1;
				if (wujiang)  //为武将
				{
					array<int> child_body = {77, 78, 86, 87, 95, 96, 104, 105, 113, 114, 122, 123};  //武官的基本造型
					int rd_n = child_body[pk::rand(12)];
					child.body[0] = rd_n;
					child.body[1] = rd_n;
					child.body[2] = rd_n;
					child.body[3] = rd_n;
					child.body[4] = rd_n;
					child.body[5] = rd_n;
				}
				else
				{
					array<int> child_body = {81, 82, 90, 91, 99, 100, 108, 109, 117, 118, 126, 127};  //文官的基本造型
					int rd_n = child_body[pk::rand(12)];
					child.body[0] = rd_n;
					child.body[1] = rd_n;
					child.body[2] = rd_n;
					child.body[3] = rd_n;
					child.body[4] = rd_n;
					child.body[5] = rd_n;
				}
			}
			
			child.name_read = pk::encode(type_name[child_type]);	//姓名讀音显示武将类型

			switch (child_type)
			{
			case 0: //帅才
				range_能力_min = {68, 45, 42, 5, 40};
				range_能力_max = {88, 75, 72, 45, 80};
				range_适性 = 2;
				buff_适性 = 2;
				chance_話术 = 25;
				break;
			case 1: //猛将
				range_能力_min = {60, 70, 25, 5, 40};
				range_能力_max = {80, 90, 65, 45, 80};
				range_适性 = 2;
				buff_适性 = 2;
				chance_話术 = 25;
				break;
			case 2: //军师
				range_能力_min = {50, 10, 65, 30, 40};
				range_能力_max = {80, 50, 85, 70, 80};
				range_适性 = 2;
				buff_适性 = 1;
				chance_話术 = 50;
				break;
			case 3: //謀士
				range_能力_min = {25, 10, 80, 40, 40};
				range_能力_max = {65, 50, 90, 80, 80};
				range_适性 = 2;
				buff_适性 = 1;
				chance_話术 = 50;
				break;
			case 4: //文臣
				range_能力_min = {25, 10, 45, 70, 40};
				range_能力_max = {65, 50, 75, 90, 80};
				range_适性 = 1;
				buff_适性 = 1;
				chance_話术 = 45;
				break;
			case 5: //名人
				range_能力_min = {30, 30, 30, 30, 40};
				range_能力_max = {80, 80, 80, 80, 80};
				range_适性 = 2;
				buff_适性 = 1;
				chance_話术 = 30;
				break;
			case 6:	//翹楚
				range_能力_min = {60, 60, 60, 60, 50};
				range_能力_max = {90, 90, 90, 90, 90};
				range_适性 = 2;
				buff_适性 = 5;
				chance_話术 = 45;
			}


			//	人才相对五维和计算（包括父母能力加成）
			int buff_father = 0, buff_mother = 0;  //父母能力修正
			for (int i = 0; i < 武将能力_末; i++)
			{
				buff_father = father.base_stat[i] / 20;
				buff_mother = mother.base_stat[i] / 20;
				relative_stat[i] = random(range_能力_min[i] + buff_下限, range_能力_max[i] + buff_上限 + buff_father + buff_mother);
			}


			//	 人才能力最终
			int final_stat = 0;
			for (int i = 0; i < 武将能力_末; i++)
			{
				final_stat = int(relative_stat[i] * 孩子能力倍率 / 100.f);
				child.base_stat[i] = pk::min(final_stat, 孩子能力上限);
				child.stat_aging[i] = random(능력성장_초지속형, 능력성장_장비형);
				//ch::u8debug(pk::format("stat_0:{}, stat_1:{}", relative_stat[i], final_stat));
			}

			//  武将适性
			child.tekisei[병종_기병] = random(0, range_适性);
			child.tekisei[병종_창병] = random(0, range_适性);
			child.tekisei[병종_극병] = random(0, range_适性);
			child.tekisei[병종_노병] = random(0, range_适性);
			child.tekisei[병종_병기] = random(0, range_适性);
			child.tekisei[兵种_水军] = random(0, range_适性);
			//  随机适性升级
			buff_适性 += random(0, 1);
			int troops_type;
			for (int i = 0; i < buff_适性; i++)
			{
				troops_type = random(병종_창병, 兵种_水军);
				if (child.tekisei[troops_type] >= 孩子适性上限) continue;    //超出上限则不升级，浪费一级那就算倒?
				child.tekisei[troops_type] = child.tekisei[troops_type] + 1;
			}
			//  話术
			child.wajutsu_daikatsu = pk::rand_bool(chance_話术);  //大喝
			child.wajutsu_kiben = pk::rand_bool(chance_話术);     //詭辯
			child.wajutsu_mushi = pk::rand_bool(chance_話术);     //无視
			child.wajutsu_chinsei = pk::rand_bool(chance_話术);   //?静
			child.wajutsu_gyakujou = pk::rand_bool(chance_話术);  //憤怒


			//	特技
			child.skill = -1;  //特技初始化
			int index, chance = random(1, 100), best_stat=-1;

			switch (child_type)
			{
			case 0: //帅才
				if (chance<=40)
					child.skill = get_兵种特技(@child);
				else if (chance<=75)
					child.skill = get_战斗特技(@child);
				else if (chance<=85)
					child.skill = get_智力特技();
				else if (chance<=90)
					child.skill = get_神技(@child);
				else
					child.skill = get_辅助特技();
				break;
			case 1: //猛将
				if (chance<=35)
					child.skill = get_兵种特技(@child);
				else if (chance<=85)
					child.skill = get_战斗特技(@child);
				else if (chance<=90)
					child.skill = get_神技(@child);
				else
					child.skill = get_辅助特技();
				break;
			case 2: //军师
				if (chance<=15)
					child.skill = get_战斗特技(@child);
				else if (chance<=65)
					child.skill = get_智力特技();
				else if (chance<=70)
					child.skill = get_神技(@child);
				else
					child.skill = get_辅助特技();
				break;
			case 3: //謀士
				if (chance<=70)
					child.skill = get_智力特技();
				else if (chance<=75)
					child.skill = get_神技(@child);
				else
					child.skill = get_辅助特技();
				break;
			case 4: //文臣
				if (chance<=60)
					child.skill = get_内政特技();
				else if (chance<=65)
					child.skill = get_神技(@child);
				else
					child.skill = get_辅助特技();
				break;
			case 5: //名人
				if (chance<=50)
				{
					best_stat = get_best_stat(@child);
					switch (best_stat)
					{
						case 0:		child.skill = get_兵种特技(@child);break;
						case 1:		child.skill = get_战斗特技(@child);break;
						case 2:		child.skill = get_智力特技();break;
						case 3:		child.skill = get_内政特技();break;
						case 4:		child.skill = get_辅助特技();break;
					}
				}
				else if (chance<=60)
					child.skill = get_神技(@child);
				else
					child.skill = get_辅助特技();
				break;
			case 6:	//翹楚
				if (chance<=80)
				{
					best_stat = get_best_stat(@child);
					switch (best_stat)
					{
						case 0:		child.skill = get_兵种特技(@child);break;
						case 1:		child.skill = get_战斗特技(@child);break;
						case 2:		child.skill = get_智力特技();break;
						case 3:		child.skill = get_内政特技();break;
						case 4:		child.skill = get_辅助特技();break;
					}
				}
				else if (chance<=95)
					child.skill = get_神技(@child);
				else
					child.skill = get_辅助特技();
				break;
			default:
				break;
			}

			child.update();

			return face_color;
		}

		int getPersonType(pk::person@ person)
		{
			float 将帅zhi = person.stat[武将能力_统率] * 0.55f + person.stat[武将能力_武力] * 0.30f +  person.stat[武将能力_智力] * 0.30f;
			float 猛将zhi = person.stat[武将能力_统率] * 0.30f + person.stat[武将能力_武力] * 0.75f;
			float 军师zhi = person.stat[武将能力_统率] * 0.45f + person.stat[武将能力_智力] * 0.65f;
			float 謀士zhi = person.stat[武将能力_智力] * 1.00f;
			float 文臣zhi = person.stat[武将能力_政治] * 0.55f + person.stat[武将能力_智力] * 0.45f;
			float 名人zhi = person.stat[武将能力_魅力] * 1.00f;

			array<float> 类型zhi = {将帅zhi, 猛将zhi, 军师zhi, 謀士zhi, 文臣zhi, 名人zhi};
			int type = 0;

			//  选出最高的类型zhi
			for (int i = 1; i < 6; i++)
			{
				if (类型zhi[i] > 类型zhi[type]) type = i;
			}
			return type;
		}
		
		array<int> 枪兵技 = {41, 42, 34, 110, 150};   //斗神；枪神；枪将；猛卒；摧锋
		array<int> 戟兵技 = {41, 43, 35, 110, 151};   //斗神；戟神；戟将；猛卒；重甲
		array<int> 弩兵技 = {44, 36, 50, 108, 129, 152};   //弓神；弓将；射手；狙击；神臂；轻甲
		array<int> 骑兵技 = {45, 49, 37, 25, 3, 109};   //骑神；疾驰；骑将；白马；长驱；猛骑
		array<int> 攻城兵技 = {17, 46, 24};   //攻城；工神；射程
		array<int> 水兵技 = {47, 4, 5, 14, 38};   //水神；推进；操舵；强袭；水将
		
		int get_兵种特技(pk::person@ new_face)
		{
			array<int> score_兵种 = {0,0,0,0,0,0};
			int best_兵种 = 0;
			for (int i = 0; i < 병종_끝; i++)
			{
				// A级范围0-49，S级50-99，6个兵种选score最高zhi获得該兵种战法
				score_兵种[i] = random((new_face.tekisei[i] - 2) * 50, (new_face.tekisei[i] - 1) * 50 -1);
			}
			for (int i = 1; i < 병종_끝; i++)
			{
				if (score_兵种[best_兵种] <= score_兵种[i])
					best_兵种 = i;
			}

			int chance = random(31, 130) - (new_face.tekisei[best_兵种] >= 3 ? 30 : 0);
			switch (best_兵种)
			{
			case 0:	//枪兵
				if (chance <= 10) 		return 특기_투신;
				else if (chance <= 30) return 특기_창신;
				else 					return 특기_창장;
			case 1:	//戟兵
				if (chance <= 10)		return 특기_투신;
				else if (chance <= 30)	return 특기_극신;
				else 					return 특기_극장;
			case 2:	//弩兵
				if (chance <= 30)		return 특기_궁신;
				else if (chance%2 == 0)	return 특기_궁장;
				else 					return 특기_사수;
			case 3:	//骑兵
				if (chance <= 30)
				{
					if 		(chance%2 == 0)	return 특기_기신;
					else 					return 特技_疾驰;
				}else 
				{
					if 		(chance%3 == 0)	return 특기_기장;
					else if (chance%3 == 1)	return 특기_백마;
					else 					return 특기_행군;
				}
			case 4:	//攻城兵
				if (chance <= 30)
				{
					if 		(chance%2 == 0)	return 特技_攻城;
					else 					return 特技_工神;
				}
				else						return 特技_射程;
			case 5:	//水兵
				if (chance <= 30)			return 특기_수신;
				else 
				{
					if 		(chance%4 == 0)	return 특기_추진;
					else if (chance%4 == 1)	return 특기_조타;
					else if (chance%4 == 2)	return 특기_강습;
					else 					return 특기_수장;
				}
			default:
				return -1;
			}
			return -1;
		}

		array<int> 统军技 = {1, 2, 6, 18, 20, 22, 27, 28, 29, 31};
		array<int> 破军技 = {9, 10, 11, 12, 13, 15, 21, 30, 51};

		int get_战斗特技(pk::person@ new_face)
		{
			int 武力 = new_face.base_stat[1], index;

			if (武力 > 75)
			{
				if (pk::rand_bool(70))
				{
					index = random(0, 破军技.length - 1);
					return 破军技[index];
				}
				else
				{
					index = random(0, 统军技.length - 1);
					return 统军技[index];
				}
			}
			else
			{
				if (pk::rand_bool(70))
				{
					index = random(0, 统军技.length - 1);
					return 统军技[index];
				}
				else
				{
					index = random(0, 破军技.length - 1);
					return 破军技[index];
				}
			}
		}

		array<int> 辅助特技 = {7, 8, 26, 32, 33, 52, 69, 75, 76, 91, 92, 93, 94, 99};
		array<int> 智力特技 = {16, 53, 54, 55, 56, 58, 59, 60, 65, 66, 67, 68, 86};
		array<int> 内政特技 = {77, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 95, 96, 97, 98};
		array<int> 神技 = {0, 19, 40, 48, 57, 61, 62, 63, 64, 70, 71};

		int get_智力特技()
		{
			int index = random(0, 智力特技.length - 1);
			return 智力特技[index];
		}

		int get_内政特技()
		{
			int index = random(0, 内政特技.length - 1);
			return 内政特技[index];
		}

		int get_辅助特技()
		{
			int index = random(0, 辅助特技.length - 1);
			return 辅助特技[index];
		}

		int get_神技(pk::person@ new_face)
		{
			int 武力 = new_face.base_stat[1], 智力 = new_face.base_stat[2], index;
			if (武力 >= 智力)
			{
				index = random(0, 3);
				return 神技[index];
			}
			else
			{
				index = random(4, 神技.length - 1);
				return 神技[index];
			}
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
		
		void callback(pk::force@ force)
		{
			if (force.is_player())
			{
				@force_ = force;
				@kunshu_ = pk::get_person(force.kunshu);
				@kunshu_city = pk::building_to_city(pk::get_building(kunshu_.service));

				//测试
				/* if (pk::rand_bool(发生率_拜师))
				{
					pk::scene(pk::scene_t(scene_shishi));	
				} */
				
				//分娩事件
				pk::scene(pk::scene_t(scene_fenmian));
				
				
				//生日事件
				pk::scene(pk::scene_t(scene_shengri));
				
				//孩子拜师事件
				if(pk::rand_bool(发生率_拜师))
				{
					pk::scene(pk::scene_t(scene_baishi));
				}

			}
		}
		
		
		void scene_fenmian()
		{
			string kunshu_name = pk::decode(pk::get_name(kunshu_));
			pk::list<pk::person@> birth_person_list;
			birth_person_list = getpersonList(force_);
			@oldwoman = pk::get_person(무장_노파);
			if (birth_person_list.count > 0)
			{
				for (int i=0; i < int(birth_person_list.count); i++)
				{
					pk::person@ child_birth;
					@child_birth = birth_person_list[i];
					string child_birth_name = pk::decode(pk::get_name(child_birth));
					pk::person@ child_mother;
					@child_mother = pk::get_person(child_birth.mother);
					string child_mother_name = pk::decode(pk::get_name(child_mother));
					pk::person@ child_father;
					@child_father = pk::get_person(child_birth.father);
					string child_father_name = pk::decode(pk::get_name(child_father));
					pk::building@ birth_building;
					pk::city@ birth_city;
					
					//生日
					int child_birth_month = child_birth.kouseki / 100;
					int child_birth_day = child_birth.kouseki % 100;
					int child_birth_xun = 1;  //初始化
					if (child_birth_day >= 21)
					{
						child_birth_xun = 21;
					}
					else if (child_birth_day >= 11)
					{
						child_birth_xun = 11;
					}
					else 
					{
						child_birth_xun = 1;
					}
					
					//生日end
					
					if (child_mother.mibun == 신분_사망 and (child_birth.birth * 360 + child_birth_month * 30 + child_birth_xun) >= (pk::get_year() * 360 + pk::get_month() * 30 + pk::get_day()))  //母亲死亡在胎儿出生前
					{
						pk::move_screen(kunshu_city.get_pos());
						pk::fade(0);
						pk::background(-1);
						pk::play_bgm(22);
						
						if (child_father.mibun == 신분_사망)
						{
							pk::message_box(pk::encode(pk::format("伴随着生母\x1b[1x{}\x1b[0x的死亡,\n\x1b[1x{}\x1b[0x的遗腹子\x1b[1x{}\x1b[0x亦追随母亲而去了!", child_mother_name, child_father_name, child_birth_name)));
							child_birth.mibun = 身份_一般;
							child_birth.update();
							pk::kill(child_birth,null,null,null,2);
						}
						else
						{
							pk::message_box(pk::encode(pk::format("伴随着生母\x1b[1x{}\x1b[0x的死亡,\n尚在腹中的\x1b[1x{}\x1b[0x亦追随母亲而去了!", child_mother_name, child_birth_name)));
							child_birth.mibun = 身份_一般;
							child_birth.update();
							pk::kill(child_birth,null,null,null,2);
							pk::message_box(pk::encode("......................\n.....................\n......................."), child_father);
							pk::message_box(pk::encode(pk::format("在同时遭受了丧妻丧子之痛双重重击后,\n一言未发的\x1b[1x{}\x1b[0x不久便病倒了.", child_father_name)));
							if (child_father.shoubyou < 2)
							{
								child_father.shoubyou = 2;
								child_father.update();
							}
						}
					}
					
					else if (child_mother.mibun == 신분_포로 and child_birth_xun == pk::get_day() and child_birth_month == pk::get_month() and child_birth.birth == pk::get_year())  //生母被俘虏的情形
					{
						pk::play_bgm(22);
						@birth_building = pk::get_building(child_mother.service);
						//@birth_city = pk::building_to_city(birth_building);
						pk::person@ kunshu_fulu;
						@kunshu_fulu = pk::get_person(pk::get_kunshu_id(birth_building));
						string kunshu_fulu_name = pk::decode(pk::get_name(kunshu_fulu));
						pk::person@ shizhe;
						@shizhe = pk::get_person(무장_사자);
						
						pk::move_screen(birth_building.get_pos());
						//pk::move_screen(kunshu_city.get_pos());
						pk::fade(0);
						pk::background(-1);
						
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x如今已是临盆在即,\n而此时的她却身陷囹圄...", child_mother_name)));
						
						pk::fade(255);
						pk::background("media/background/baifang/jianyu01.png");
						
						pk::message_box(pk::encode("要生了...啊!\n求你们让我把孩子生下来..."), child_mother);
						
						//给娃换个头像
						int child_birth_face = child_birth.face;
						child_birth.face = 799;
						child_birth.update();
						
						pk::fade(0);
						pk::fade(255);
						pk::message_box(pk::encode("哦哇~哦哇~"), child_birth);
						pk::message_box(pk::encode(pk::format("（我的孩子,你出来的不是时候啊...）\n狱卒大人,我要求见于你家\x1b[1x主公\x1b[0x!\n请让我面见\x1b[1x{}\x1b[0x大人!", kunshu_fulu_name)), child_mother);
						pk::fade(0);
						pk::fade(255);
						pk::message_box(pk::encode("哦哦,是这样啊.\n你恳求我放了这\x1b[1x婴儿\x1b[0x?"), kunshu_fulu);

						if (kunshu_fulu.stat[4] >= 72)
						{
							pk::message_box(pk::encode(pk::format("明白了!\n我素来便是\x1b[1x治以仁孝\x1b[0x,更何况\x1b[1x竖子无罪\x1b[0x.\n来人,将此儿送还\x1b[1x{}\x1b[0x!", child_father_name)), kunshu_fulu);
							pk::message_box(pk::encode("多谢大人!多谢大人!\n（得救了...）"), child_mother);
						}
						else
						{
							pk::message_box(pk::encode("那我就给你一个机会说服我."), kunshu_fulu);
							debate_result = pk::debate(child_mother, kunshu_fulu, pk::is_player_controlled(child_mother), false, false, true).first;
							pk::background("media/background/baifang/jianyu01.png");
							if (debate_result == 0)   //舌战胜
							{
								pk::message_box(pk::encode(pk::format("原来是这样啊,我明白了.\n来人,将此儿送还\x1b[1x{}\x1b[0x!", child_father_name)), kunshu_fulu);
								pk::message_box(pk::encode("多谢大人!"), child_mother);
							}
							else
							{
								pk::cutin(13);
								pk::message_box(pk::encode("这下你哑口无言了吧！\n来人,把那孩子..."), kunshu_fulu);
								pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人请等一下!!\n我愿以我的微命来换我儿,请大人放过他吧!", kunshu_fulu_name)), child_mother);
								pk::message_box(pk::encode("素闻\x1b[1x以孝治天下者,不害人之亲.\n施仁政于天下者,不绝人之祀.\x1b[0x\n今我已无挂念,唯此儿而已."), child_mother);
								pk::cutin(17);
								pk::message_box(pk::encode("（真烈女也!）哈哈哈哈哈."), kunshu_fulu);
								pk::message_box(pk::encode(pk::format("那只是一个考验而已.\n\x1b[1x竖子无罪\x1b[0x!\n来人,把那孩子送还\x1b[1x{}\x1b[0x!", child_father_name)), kunshu_fulu);
								pk::message_box(pk::encode("?!!\n多谢大人!多谢!多谢大人!..."), child_mother);
							}
						}

						pk::fade(0);
						pk::background(-1);
						pk::fade(255);
						pk::move_screen(kunshu_city.get_pos());
						pk::fade(0);
						pk::background(2);
						pk::fade(255);
						
						pk::message_box(pk::encode("我家主公以仁孝治天下,不忍加罪于此子,\n特令我等前来送还!"), shizhe);
						pk::cutin(20);
						pk::message_box(pk::encode(pk::format("（如此...）\n使者大人,请向你家主公转达\x1b[1x{}\x1b[0x的谢意.\n这些是我的谢礼,请一并转达!", child_father_name)), child_father);
						
						child_birth.service = kunshu_.service;
						child_birth.location = kunshu_.service;
						pk::add_gold(pk::get_building(kunshu_.service), -1500, true);
						pk::get_building(kunshu_.service).update();
						pk::add_gold(birth_building, 1500, true);
						birth_building.update();
						pk::message_box(pk::encode("送上了厚重的谢礼."));
						
						//头像重置
						child_birth.face = child_birth_face;
						child_birth.update();
					}
					
					else if (child_birth_xun == pk::get_day() and child_birth_month == pk::get_month() and child_birth.birth == pk::get_year())
					{
						pk::play_bgm(19);
						@birth_building = pk::get_building(child_mother.service);
						//@birth_city = pk::building_to_city(birth_building);
						string call_child_birth = child_birth.mei.length() > 2 ? pk::decode(child_birth.mei) : pk::decode(child_birth.mei) + "儿";
						child_mother.absence_timer = 0;     //怀胎结束
						child_mother.order = -1;     //怀胎结束
						child_mother.update();
					
						//给娃换个头像
						int child_birth_face = child_birth.face;
						child_birth.face = 799;
						child_birth.update();
						
						pk::move_screen(birth_building.get_pos());
						pk::fade(0);
						pk::sleep();
						pk::background(5);
						pk::fade(255);
						
						pk::message_box(pk::encode("要生了...啊!\n快,快去叫禀告夫君!"), child_mother);
						pk::fade(0);
						pk::fade(255);
						pk::cutin(18);
						pk::message_box(pk::encode("夫人我来了,你怎么样了?"), child_father);
						pk::message_box(pk::encode("我要给贵夫人接生了,您请回避吧!"), oldwoman);
						pk::fade(0);
						pk::fade(255);
						pk::message_box(pk::encode("过去了许久..."));
						pk::sleep();
						int random_n = pk::rand(10);
						int child_mother_0 = child_mother.get_id();  //附带条件女性主君不会难产，《英雄集结女流之战》不会难产  高龄产妇危机
						if(pk::get_scenario().no == 7 || pk::get_scenario().no == 13 || child_mother_0 == kunshu_.get_id() || child_mother.birth + 35 >= pk::get_year() || random_n <= 7)
						{
							pk::message_box(pk::encode("哦哇~哦哇~"), child_birth);
							pk::fade(0);
							pk::fade(255);
							pk::message_box(pk::encode("恭喜恭喜,喜得麟儿!"), oldwoman);
							pk::background("media/background/baifang/fenmian01.png");
							
							pk::message_box(pk::encode(pk::format("夫人你看,\x1b[1x{}\x1b[0x的眼睛鼻子多像你.", call_child_birth)), child_father);
							pk::message_box(pk::encode("他的眉毛跟你一模一样!"), child_mother);
							pk::message_box(pk::encode("夫人辛苦了,接下来你就好好休养吧."), child_father);
							
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x军的\x1b[1x{}\x1b[0x出生了!", kunshu_name, child_birth_name)));
						}
						else 
						{
							pk::play_bgm(22);
							pk::message_box(pk::encode("又过去了几个时辰..."));
							pk::sleep();
							pk::message_box(pk::encode("怎么样了,怎么生产那么久..."), child_father);
							pk::message_box(pk::encode("啊!!......."), child_mother);
							pk::message_box(pk::encode("快!!\n快止住血!"), oldwoman);
							pk::fade(0);
							pk::fade(255);
							pk::message_box(pk::encode("（孩子,对不起,对不起......\n\x1b[1x娘亲\x1b[0x最后还是没能把你生下来...）\n（已没有气息......）"), child_mother);
							pk::cutin(13);
							pk::message_box(pk::encode("夫人!!!!!\n不可能的!!!!!\n快!!快去让大夫过来!!"), child_father);
							pk::fade(0);
							pk::background(-1);
							pk::kill(child_mother,null,null,null,2);
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x军的\x1b[1x{}\x1b[0x难产而死!", kunshu_name, pk::decode(pk::get_name(child_mother)))));
							child_birth.mibun = 身份_一般;
							child_birth.update();
							pk::kill(child_birth,null,null,null,2);
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的孩子\x1b[1x{}\x1b[0x未能出生!", kunshu_name, child_birth_name)));
							
							pk::message_box(pk::encode("......................\n.....................\n......................."), child_father);
							pk::message_box(pk::encode(pk::format("在同时遭受了丧妻丧子之痛双重重击后,\n一言未发的\x1b[1x{}\x1b[0x不久便病倒了.", child_father_name)));
							if (child_father.shoubyou < 2)
							{
								child_father.shoubyou = 2;
								child_father.update();
							}
						}
						
						//头像重置
						child_birth.face = child_birth_face;
						child_birth.update();
						
						pk::fade(0);
						pk::background(-1);
						pk::fade(255);
					}
					
				}
			}
		}
		
		//生日事件
		void scene_shengri()
		{
			pk::list<pk::person@> birthday_person_list;
			birthday_person_list = getpersonList(force_);
			//@oldwoman = pk::get_person(무장_노파);
			if (birthday_person_list.count > 0)
			{
				for (int i=0; i < int(birthday_person_list.count); i++)
				{
					pk::person@ birthday_child;
					@birthday_child = birthday_person_list[i];
					if (birthday_child.birth >= pk::get_year())  continue;  //未到出生年龄的跳过
					if (birthday_child.birth < pk::get_year() - 3)  continue;  //只过1.2.3周岁生日
					pk::person@ birthday_child_father = pk::get_person(birthday_child.father);
					pk::person@ birthday_child_mother = pk::get_person(birthday_child.mother);
					if (birthday_child_father.mibun < 0 || birthday_child_father.mibun > 4 || pk::is_absent(birthday_child_father) || pk::is_unitize(birthday_child_father))  continue;  //父亲非正常身份跳过，父亲出征部队时跳过
					if (birthday_child_mother.mibun < 0 || birthday_child_mother.mibun > 4 || pk::is_absent(birthday_child_mother) || pk::is_unitize(birthday_child_mother))  continue;  //母亲非正常身份跳过
					
					string birthday_child_name = pk::decode(pk::get_name(birthday_child));
					
					//生日
					int birthday_child_month = birthday_child.kouseki / 100;
					int birthday_child_day = birthday_child.kouseki % 100;
					int birthday_child_xun = 1;  //初始化
					if (birthday_child_day >= 21)
					{
						birthday_child_xun = 21;
					}
					else if (birthday_child_day >= 11)
					{
						birthday_child_xun = 11;
					}
					else 
					{
						birthday_child_xun = 1;
					}
					
					//生日end

					if (birthday_child_xun == pk::get_day() and birthday_child_month == pk::get_month() and birthday_child.birth < pk::get_year())
					{
						
						pk::building@ birthday_building;
						pk::city@ birthday_city;
						@birthday_building = pk::get_building(birthday_child.service);
						//@birthday_city = pk::building_to_city(birthday_building);
						string call_birthday_child = birthday_child.mei.length() > 2 ? pk::decode(birthday_child.mei) : pk::decode(birthday_child.mei) + "儿";
						int years_ago = pk::get_year() - birthday_child.birth;
						//给娃换个头像
						int birthday_child_face = birthday_child.face;
						if (pk::get_year() <= birthday_child.birth + 1)
							birthday_child.face = 799;
						else
<<<<<<< HEAD
							birthday_child.face = birthday_child.sex == 0 ? 757 : 758;
=======
							birthday_child.face = birthday_child.sex == 0 ? 753 : 754;//已根据新头像编号更改
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

						birthday_child.update();
						pk::play_bgm(19);
						pk::move_screen(birthday_building.get_pos());
						pk::fade(0);
						pk::sleep();
						pk::background(5);
						pk::fade(255);
						
						pk::message_box(pk::encode(pk::format("（今天是\x1b[1x{}\x1b[0x的生辰了...）", call_birthday_child)), birthday_child_father);
						pk::fade(0);
						pk::fade(255);
						pk::message_box(pk::encode(pk::format("夫人,{}年前你生\x1b[1x{}\x1b[0x时辛苦了.\n今天就我们就好好庆祝一下吧!", years_ago, call_birthday_child)), birthday_child_father);
						
						if (pk::get_year() == birthday_child.birth + 1)
						{
							pk::message_box(pk::encode(pk::format("嗯,愿我们的\x1b[1x{}\x1b[0x平安喜乐!", call_birthday_child)), birthday_child_mother);
							pk::message_box(pk::encode("愿夫人也平安喜乐!"), birthday_child_father);
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,「母-母-」,叫「母-母-」.", call_birthday_child)), birthday_child_mother);
							pk::message_box(pk::encode("哦啊母~哦啊母~"), birthday_child);
							pk::message_box(pk::encode(pk::format("夫君,快看!\x1b[1x{}\x1b[0x会说「母」了!", call_birthday_child)), birthday_child_mother);
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x真乖!\n「爸-爸-」,叫「爸-爸-」.", call_birthday_child)), birthday_child_father);
							pk::message_box(pk::encode("哇啊爸~哇啊爸~"), birthday_child);
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x真乖!", call_birthday_child)), birthday_child_father);
							pk::message_box(pk::encode("「母-母-」,叫「母-母-」."), birthday_child_mother);
							pk::message_box(pk::encode("哦啊母~哦啊母~"), birthday_child);
							
						}
						else
						{
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x快过来!\n\x1b[1x{}\x1b[0x可知道今天是什么日子?", call_birthday_child, call_birthday_child)), birthday_child_father);
							pk::message_box(pk::encode(pk::format("今天是\x1b[1x{}\x1b[0x的生辰! \n爹爹,\x1b[1x{}\x1b[0x说的对吗?", call_birthday_child, call_birthday_child)), birthday_child);
							pk::message_box(pk::encode(pk::format("嗯,对了!愿\x1b[1x{}\x1b[0x平安喜乐!", call_birthday_child)), birthday_child_father);
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x要一直平安喜乐呀!", call_birthday_child)), birthday_child_mother);
							pk::message_box(pk::encode("看,这是什么?\n爹爹给你买的礼物,喜不喜欢呀?"), birthday_child_father);
							pk::message_box(pk::encode("哇哦好好看呀~谢谢爹爹!"), birthday_child);
							pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x真乖!", call_birthday_child)), birthday_child_father);

						}
						pk::fade(0);
						pk::fade(255);
						pk::message_box(pk::encode(pk::format("一家人欢愉地度过了\x1b[1x{}\x1b[0x的生辰!", birthday_child_name)));
						//头像重置
						birthday_child.face = birthday_child_face;
						birthday_child.update();
						
						pk::fade(0);
						pk::background(-1);
						pk::fade(255);
					}
					
				}
			}
		}
		
		//拜师事件
		void scene_baishi()
		{
			string kunshu_name = pk::decode(pk::get_name(kunshu_));
			child_baishi_list = child_personList(force_);
			child_shifu_list = child_shifu_personList(force_);
			if (child_shifu_list.count > 0 and child_baishi_list.count > 0)
			{
				@child_shifu = child_shifu_list[pk::rand(child_shifu_list.count)];
				if (child_shifu is null) return;
				string child_shifu_name = pk::decode(pk::get_name(child_shifu));
				@child_baishi = child_baishi_list[pk::rand(child_baishi_list.count)];
				if (child_baishi is null) return;
				string child_baishi_name = pk::decode(pk::get_name(child_baishi));
				if (child_baishi.liked[2] == -1 and (pk::get_year() > child_baishi.birth + 6) and child_baishi.location == kunshu_.service)
				{
					pk::building@ baishi_building = pk::get_building(child_baishi.service);
					pk::move_screen(baishi_building.get_pos());
					pk::fade(0);
					pk::background(5);
					pk::fade(255);
					
					//给娃换个头像
					int child_baishi_face = child_baishi.face;
					if (pk::get_year() <= child_baishi.birth + 1)
						child_baishi.face = 799;
					else
<<<<<<< HEAD
						child_baishi.face = child_baishi.sex == 0 ? 757 : 758;
=======
						child_baishi.face = child_baishi.sex == 0 ? 753 : 754;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					child_baishi.update();
					string call_kunshu = kunshu_.sex == 0 ? "父亲大人" : "母亲大人";
					pk::message_box(pk::encode(pk::format("哎呀,\x1b[1x{}\x1b[0x已经到了这个年纪了呀!\n得给孩子找个老师了.", child_baishi_name)), kunshu_);
					pk::message_box(pk::encode("主公,别来无恙."), child_shifu);
					pk::message_box(pk::encode("阁下能否当这孩子的老师?"), kunshu_);
					pk::message_box(pk::encode("哦,原来是这样啊."), child_shifu);
					pk::cutin(CG_脚步);
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,\n这位一定就是您常说起的\x1b[1x{}\x1b[0x先生吧!\n\x1b[1x{}\x1b[0x给先生问安!", call_kunshu, child_shifu_name, child_baishi_name)), child_baishi);
					pk::message_box(pk::encode("嗯很好!\n看来公子与我也有些缘分,\n在下一定全力教导!"), child_shifu);
					pk::message_box(pk::encode("是!拜见老师."), child_baishi);
					
					//娃头像重置
					child_baishi.face = child_baishi_face;
					child_baishi.update();
					
					pk::play_se(6);
					pk::fade(0);
					pk::fade(255);
					int beststat = get_best_stat(@child_shifu);
					child_baishi.liked[2] = child_shifu.get_id();
					//pk::add_base_stat(child_baishi, beststat, stat_award);
					child_baishi.base_stat[beststat] = child_baishi.base_stat[beststat] + stat_award;
					child_baishi.update();
					array<string> list_1_0 = {"统帅", "武力", "智力", "政治", "魅力"};
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x拜\x1b[1x{}\x1b[0x为师,自此刻苦的学习.\n\x1b[1x{}值\x1b[0x上升了\x1b[1x{}\x1b[0x.", child_baishi_name, child_shifu_name, list_1_0[beststat], stat_award))); 
				}
			}
		}

		//未登场的、有生日的君主孩子
		pk::list<pk::person@> getpersonList(pk::force@ src_force)
        {
			pk::list<pk::person@> list;
			pk::list<pk::person@> list_t = pk::get_person_list(src_force, pk::mibun_flags(身份_未登场));
			int kunshu_0 = kunshu_.get_id();
			for (int i = 0; i < list_t.count; i++)
			{
                pk::person@ Targetperson = list_t[i];
				if (Targetperson.father != kunshu_0 and Targetperson.mother != kunshu_0)
					continue;  //排除非君主孩子
				
				//生日
				int Targetperson_month = Targetperson.kouseki / 100;
				int Targetperson_day = Targetperson.kouseki % 100;
				//生日end
				if (Targetperson_month < 1 || Targetperson_month > 12)
					continue;  //排除非特殊生日武将
				if (Targetperson_day < 1 || Targetperson_day > 28)
					continue;  //排除非特殊生日武将
				if (Targetperson.father == kunshu_0 || Targetperson.mother == kunshu_0)
					list.add(Targetperson);
			}
			return list;
		}
		
		pk::list<pk::person@> child_personList(pk::force@ src_force)  //孩子
        {
			pk::list<pk::person@> list;
			pk::list<pk::person@> list_t = pk::get_person_list(src_force, pk::mibun_flags(身份_未登场));
			for (int i = 0; i < list_t.count; i++)
			{
                pk::person@ child_baishiperson = list_t[i];
				if (child_baishiperson.liked[2] == -1 and pk::get_year() > child_baishiperson.birth+6 and child_baishiperson.location == kunshu_.service)  //2号喜爱为空，无师状态
					list.add(child_baishiperson); 
			}
			return list;
		}
		
		pk::list<pk::person@> child_shifu_personList(pk::force@ src_force)  //老师
        {
			pk::list<pk::person@> list;
			pk::list<pk::person@> list_t = pk::get_person_list(src_force, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般));
			for (int i = 0; i < list_t.count; i++)
			{
                pk::person@ child_shifuperson = list_t[i];
				int kunshu_0 =kunshu_.get_id();
				int child_shifu_0 = child_shifuperson.get_id();
				if (child_shifuperson.spouse == kunshu_0 || kunshu_.spouse == child_shifu_0)
					continue;  //排除配偶
				if (child_shifuperson.father == kunshu_0 || child_shifuperson.mother == kunshu_0)
					continue;  //排除兄弟
				if (pk::get_year() <= child_shifuperson.birth + 29)
					continue;  //排除低龄
				if (child_shifuperson.location == kunshu_.service and (pk::is_like(child_shifuperson, kunshu_0) or pk::is_like(kunshu_, child_shifu_0) or pk::is_ketsuen(child_shifuperson, kunshu_0)))  //血缘
				{
					for (int j =0; j < 5; j++)
					{
						if (child_shifuperson.base_stat[j] > 80)
							list.add(child_shifuperson); break;
					}
				}

			}
			return list;
		}

		
		//是否有未发现武将
		bool haveNotfound (pk::building@ building_)
		{
			bool havenofound = false;  //初始化
			pk::list<pk::person@> list = pk::get_person_list(building_, pk::mibun_flags(身份_未发现));
			if (list.count > 0) havenofound = true;  //有未发现武将
			return havenofound;
		}
		
		
		//君主孩子头像是否重复
		bool HaveSameFace(int face_0)
		{
			for (int i = 999; i >= 850; i--)
			{
				pk::person@ child = pk::get_person(i);
				if (child.face == face_0)
					return true;  //存在相同头像的人
			}
			return false;  //不存在相同头像的人
		}
	
	}
	

	Main main;
}
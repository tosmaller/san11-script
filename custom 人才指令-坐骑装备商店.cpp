/***CUSTOM
@name 人才指令-坐骑装备商店
@class 人才指令
@author 白马叔叔
@contact 
@version 1.1
@date 2021/10/29
@desc 结构体存储调用方式改进
@notice 
***/
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/09/18 # 江东新风 # 更改background路径 ##
// ## 2021/07/28 # 白马叔叔 # 新手友好提示  ##
// ## 2021/03/14 # 白马叔叔 # 添加是否已拥有皮肤提示、提升稳定性  ##
// ## 2021/03/09 # 白马叔叔 # 增加不买返回选项、增加君主防止无人可赏，排除可授予部队、修改名称、小城不可视  ##
// ## 2021/02/12 # 氕氘氚 # 修复购买后忠诚变0的bug  ##
// ## 2021/02/10 # 白马叔叔 # 细节调整  ##
// @@ 2021/02/05 @ 白马叔叔 @@
//议价，便宜点。不买可以返回。
//每年的某个特定时间有大型马市，然后出现好马的几率提高，甚至必然出现一匹好马
namespace 坐骑装备商店
{
    //---------------------------------------------------------------------------------------
    const int ACTION_COST = 5;                   //需要的行动力
	const int gold_weapon = 500;     //买武器所需金
	const int gold_cloth = 500;        //买其他身体装束所需金
	const int gold_horse = 500;       //买马匹所需金
	const int loyalty_add = 2;          //忠诚提升值
	
	//马匹列表
	array<string> horse1_name = {"红鬃马", "汗血宝马", "灰影马", "黄骠马", "白战马", "骅骝"};
	array<string> horse2_name = {"白龙", "赤兔", "翠龙", "大宛马", "冀州战马",
														"匈奴马", "的卢", "黑月", "虎豹甲骑", "黑豹",
														"花鬃马", "绝影", "匈奴白马", "东胡战马", "青聪马",
														"踏雪乌骓", "乌骓", "五花马", "西凉甲骑", "西羌甲骑",
														"西域甲骑", "陷阵甲骑", "夜照玉狮子", "银甲战马", "雨南马",
														"玉秋", "爪黄飞电", "紫骝马", "骁风赤兔"};
	array<string> weapon1_name = {"长刀", "短刀", "红缨枪"};
	array<string> weapon2_name = {"湛金枪", "草稚枪", "丹凤刀", "长刀", "战斧",
														"短刀", "方天画戟", "凤嘴刀", "红缨枪", "钩廉枪",
														"古锭刀", "火龙枪", "霍去病枪", "金马枪", "锯齿刀",
														"龙骑枪", "龙牙刀", "落日弓", "劈浪枪", "蜻蛉切",
														"三尖两刃刀", "双铁戟", "钢鞭", "铁棍", "铁脊蛇矛",
														"铁胎弓", "玄铁大刀", "无双方天戟", "向阳戟", "涯角枪",
														"子午烈火枪", "子午枪"};
	
	int l1 = 6;    //马匹列表1的数组长度
	int l2 = 29;  //马匹列表2的数组长度
	int l3 = 3;    //武器列表1的数组长度
	int l4 = 32;  //武器列表2的数组长度

    //---------------------------------------------------------------------------------------

    class Main
    {
		pk::building@ building_;
		pk::force@ force_;
		pk::district@ district;
		pk::person@ kunshu_;
		pk::person@ selected_person_;
		pk::person@ shangren_0;
		pk::city@ city;
		string selected_person_name;
		pk::list<pk::person@> person_list;
		array<pk::person@> person_sel_arr;
		pk::random random(pk::rand());
		
		int i, j, m, n, horse_list, ma_list, weapon_list, wu_list;
		string bg_horse;
		string bg_weapon;

		Main()
		{	
			pk::menu_item item;
			item.menu = 102;
			//item.pos = 10;
			//ch::u8debug(pk::format("menu: baifang {}", global_menu::菜单_个人));
			item.shortcut = "B";
			item.init = pk::building_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(isVisible);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);	
		}
		
		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@city = pk::building_to_city(building_);
			@district = pk::get_district(building.get_district_id());
			@kunshu_ = pk::get_person(pk::get_kunshu_id(building));
			@shangren_0 = pk::get_person(무장_상인);
		}
		
		bool isVisible()
        {
            if (pk::is_campaign()) return false;
            if (building_.get_id() >= 城市_末) return false;
            return true;
        }
		
		bool isEnabled()
		{
			if (building_.get_id() >= 城市_末) return false;   //非城市
            else if (building_.facility > 시설_도시) return false;         //非城市
			else if (city.merchant_done) return false;    //已经完成与商人的交易
			else if (getTargetList(building_).count == 0) return false;    //没有可以赠予的武将
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;      //行动力不足
			else if (city.gold < gold_weapon || city.gold < gold_horse || city.gold < gold_cloth) return false;  //金不足
			return true;
		}

		string getText()
		{
			return pk::encode("装备交易");
		}
		
		string getDesc()
		{
			if (building_.get_id() >= 城市_末)
				return pk::encode("这不是一个城市");
            else if (building_.facility > 시설_도시)
				return pk::encode("这不是一个城市");
			else if (city.merchant_done) 
                return pk::encode("商人交易已完成");
			else if (getTargetList(building_).count == 0)
				return pk::encode("没有可选择的武将");
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode("行动力不足");
			else if (city.gold < gold_weapon || city.gold < gold_horse || city.gold < gold_cloth)
				return pk::encode("金不足");
			else
				return pk::encode("与坐骑装备商人进行交易(行动力 5)");
		}
		
		bool handler()
		{
			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("是否与坐骑装备商人进行交易?\n*此为\x1b[29x皮肤系统\x1b[0x,并非游戏道具*"), pk::get_person(무장_문관)) == 1) return false;
			
			pk::scene(pk::scene_t(scene_Event)); 
			
			city.merchant_done = true;  //交易完成
            if (force_.is_player())  pk::add_ap(district, -ACTION_COST);  //减行动力
			return true;
		}
		
		void scene_Event()
		{
			array<string> answers =
			{
				pk::encode("  坐骑商人  "),
				pk::encode("  武器商人  ")
			};
			
			int answers0 = pk::choose(pk::encode("请选择要交易的商人"), answers);
			if (answers0 == 0)
			{
				pk::fade(0);
				pk::background(4); 
				pk::fade(255);
				//给商人改个名字
				string shangren_0_sei = shangren_0.sei;
				string shangren_0_mei = shangren_0.mei;	
				shangren_0.sei = pk::encode("坐骑");
				shangren_0.mei = pk::encode("商人");
				shangren_0.update();
				pk::message_box(pk::encode("今天有几匹很不错的骏马,请随意看看吧~"), shangren_0);
				pk::background("media/background/baifang/ma01.png", 0); 
				
				//随机生成马匹选项
				i = pk::rand(l1);
				j = pk::rand(l1);
				while (j == i)  j = pk::rand(l1);
				m = pk::rand(l2);
				n = pk::rand(l2);
				while (n == m)  n = pk::rand(l2);
				
				//马匹列表
				int random_n = pk::rand(100) + 1;   //1-100随机数
				if (random_n <= 85)  horse_list = 0;  //普通马列表
				else if (random_n <= 95)  horse_list = 1;  //有一个特殊马列表
				else  horse_list = 2;  //有两个特殊马列表
				bool havebuy = true;  //买了的情况
				bool confirm = false;
				for (int ii = 0; ii < 7; ii++)
				{
					if (ii == 6)
					{
						havebuy = false;  //没买
						pk::message_box(pk::encode("看来看去的不买就算了,\n走开走开,不要打搅我做生意."), shangren_0);
						break;
					}
					
					int choose_ma = scene_malist1(i, j, m, n, horse_list);  //进入列表
					if (choose_ma == 0)
					{
						havebuy = false;  //没买
						pk::message_box(pk::encode("（看着衣冠楚楚却是个穷鬼..啧啧..）\n欢迎下次再来!"), shangren_0);
						break;
					}
					else if (choose_ma == 1)
					{
						//读取马的背景图片
						pk::background("media/background/horse/h0.png");
						pk::message_box(pk::encode("这匹正是武将都很喜爱的\x1b[1x脚力马\x1b[0x,\n虽说看着普通,但也是匹良驹呀."), shangren_0);
						pk::message_box(pk::encode("负重高,耐力好,不挑吃食,\n正由于这诸多的优点,\n故成为了首选的代步马匹."), shangren_0);
						pk::message_box(pk::encode(pk::format("一匹只要\x1b[1x{}\x1b[0x金.", gold_horse)), shangren_0);
						confirm = pk::yes_no(pk::encode("买这匹吗?"));
						if (confirm)
						{
							scene_confirm(0);   //引用赠予的场景
							selected_person_.horse = 255;
							ch::set_person_horse(selected_person_.get_id(), 255);
							break;
						}
					}
					else if (choose_ma == 2)
					{
						//读取马的背景图片
						bg_horse = "background/horse/h" + i + ".png";
						pk::background(bg_horse);
						pk::message_box(pk::encode(pk::format("这匹\x1b[1x{}\x1b[0x可是难得一遇的良驹呀.", horse1_name[i])), shangren_0);
						pk::message_box(pk::encode(pk::format("现在一匹只要\x1b[1x{}\x1b[0x金.", gold_horse)), shangren_0);
						confirm = pk::yes_no(pk::encode("买这匹吗?"));
						if (confirm)
						{
							scene_confirm(0);   //引用赠予的场景
							selected_person_.horse = 255;
							ch::set_person_horse(selected_person_.get_id(), horseNum(i));  //ch::set_person_horse(person_id, 编号)来调用对应模型
							break;
						}
					}
					else if (choose_ma == 3)
					{
						//读取马的背景图片
						bg_horse = "background/horse/h" + j + ".png";
						pk::background(bg_horse);
						pk::message_box(pk::encode(pk::format("这匹\x1b[1x{}\x1b[0x可是难得一遇的良驹呀.", horse1_name[j])), shangren_0);
						pk::message_box(pk::encode(pk::format("现在一匹只要\x1b[1x{}\x1b[0x金.", gold_horse)), shangren_0);
						confirm = pk::yes_no(pk::encode("买这匹吗?"));
						if (confirm)
						{
							scene_confirm(0);   //引用赠予的场景
							selected_person_.horse = 255;
							ch::set_person_horse(selected_person_.get_id(), horseNum(j));  //ch::set_person_horse(person_id, 编号)来调用对应模型
							break;
						}
					}
					else if (choose_ma == 4)
					{
						//读取马的背景图片
						bg_horse = "background/horse/" + m + ".png";
						pk::background(bg_horse);
						pk::message_box(pk::encode(pk::format("这匹\x1b[1x{}\x1b[0x可是极其稀有的良驹呀.", horse2_name[m])), shangren_0);
						pk::message_box(pk::encode(pk::format("现在一匹只要\x1b[1x{}\x1b[0x金.", gold_horse)), shangren_0);
						confirm = pk::yes_no(pk::encode("买这匹吗?"));
						if (confirm)
						{
							scene_confirm(0);   //引用赠予的场景
							selected_person_.horse = 255;
							ch::set_person_horse(selected_person_.get_id(), m);  //ch::set_person_horse(person_id, 编号)来调用对应模型
							break;
						}
					}
					else if (choose_ma == 5)
					{
						//读取马的背景图片
						bg_horse = "background/horse/" + n + ".png";
						pk::background(bg_horse);
						pk::message_box(pk::encode(pk::format("这匹\x1b[1x{}\x1b[0x可是极其稀有的良驹呀.", horse2_name[n])), shangren_0);
						pk::message_box(pk::encode(pk::format("现在一匹只要\x1b[1x{}\x1b[0x金.", gold_horse)), shangren_0);
						confirm = pk::yes_no(pk::encode("买这匹吗?"));
						if (confirm)
						{
							scene_confirm(0);   //引用赠予的场景
							selected_person_.horse = 255;
							ch::set_person_horse(selected_person_.get_id(), n);  //ch::set_person_horse(person_id, 编号)来调用对应模型
							break;
						}
					}
				}
				
				if (havebuy)  //买了的情况
				{
					if (selected_person_.get_id() == kunshu_.get_id())   //选择者为君主时
					{
						pk::play_se(6);
						switch (pk::rand(3))
						{
							case 0: pk::say(pk::encode("驾!驾!真是匹好马!"), selected_person_, city); break;
							case 1: pk::say(pk::encode("给它起个什么名字好呢?"), selected_person_, city);  break;
						}
					}
					else
					{
						selected_person_.loyalty = pk::min(255, selected_person_.loyalty + loyalty_add);   //加忠诚
						selected_person_.reward_done = true;      //赏赐完成
						selected_person_.update();
						pk::play_se(6);
						switch (pk::rand(3))
						{
							case 0: pk::say(pk::encode("感谢!"), selected_person_, city); break;
							case 1: pk::say(pk::encode("真是匹好马呀!"), selected_person_, city);  break;
							case 2: pk::say(pk::encode("必将骑此马奋力杀敌!"), selected_person_, city);  break;
						}
					}
					
					pk::add_gold(building_, -gold_horse, true);
					building_.update();
					pk::history_log(city.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x获得了新坐骑", selected_person_name)));
				}
				//商人名字重置
				shangren_0.sei = shangren_0_sei;
				shangren_0.mei = shangren_0_mei;
				shangren_0.update();
			}
			else if (answers0 == 1)
			{
				pk::fade(0);
				pk::background(4); 
				pk::fade(255);
				//给商人改个名字
				string shangren_0_sei = shangren_0.sei;
				string shangren_0_mei = shangren_0.mei;	
				shangren_0.sei = pk::encode("武器");
				shangren_0.mei = pk::encode("商人");
				shangren_0.update();
				pk::message_box(pk::encode("各式兵器品类繁多,请随意看看吧~"), shangren_0);
				//随机生成武器选项
				i = pk::rand(l3);
				j = pk::rand(l3);
				while (j == i)  j = pk::rand(l3);
				m = pk::rand(l4);
				while (m == 3 || m == 5 || m == 8)  m = pk::rand(l3);//排除长刀、短刀、红缨枪的编号
				n = pk::rand(l4);
				while (n == m || n == 3 || n == 5 || n == 8)  n = pk::rand(l4);
				
				//武器列表
				int random_n = pk::rand(100) + 1;   //1-100随机数
				if (random_n <= 85)  weapon_list = 0;  //普通武器列表
				else if (random_n <= 95)  weapon_list = 1;  //有一个特殊武器列表
				else  weapon_list = 2;  //有两个特殊武器列表
				bool havebuy = true;  //买了的情况
				bool confirm = false;
				for (int ii = 0; ii < 7; ii++)
				{
					if (ii == 6)
					{
						havebuy = false;  //没买
						pk::message_box(pk::encode("看来看去的不买就算了,\n走开走开,不要打搅我做生意."), shangren_0);
						break;
					}
					
					int choose_wu = scene_wulist1(i, j, m, n, weapon_list);  //进入列表
					if (choose_wu == 0)
					{
						havebuy = false;  //没买
						pk::message_box(pk::encode("（看着衣冠楚楚却是个穷鬼..啧啧..）\n欢迎下次再来!"), shangren_0);
						break;
					}
					else if (choose_wu == 1)
					{
						//读取武器的背景图片
						pk::background("media/background/weapon/w00.png");
						pk::message_box(pk::encode("这件正是武将都很喜爱的\x1b[1x长枪\x1b[0x,\n虽说看着普通,但却是难得的趁手."), shangren_0);
						pk::message_box(pk::encode("挥砍,穿刺,做工都是一流,\n正由于这诸多的优点,\n故成为了武将首选的兵器."), shangren_0);
						pk::message_box(pk::encode(pk::format("一件只要\x1b[1x{}\x1b[0x金.", gold_weapon)), shangren_0);
						confirm = pk::yes_no(pk::encode("买这件吗?"));
						if (confirm)
						{
							scene_confirm(1);   //引用赠予的场景
							selected_person_.left_weapon = 255;
							selected_person_.right_weapon = 255;
							ch::set_person_weapon(selected_person_.get_id(), 255);
							break;
						}
					}
					else if (choose_wu == 2)
					{
						//读取武器的背景图片
						bg_weapon = "background/weapon/w" + i + ".png";
						pk::background(bg_weapon);
						pk::message_box(pk::encode(pk::format("这件\x1b[1x{}\x1b[0x可是难得一遇的兵器呀.", weapon1_name[i])), shangren_0);
						pk::message_box(pk::encode(pk::format("现在一件只要\x1b[1x{}\x1b[0x金.", gold_weapon)), shangren_0);
						confirm = pk::yes_no(pk::encode("买这件吗?"));
						if (confirm)
						{
							scene_confirm(1);   //引用赠予的场景
							selected_person_.left_weapon = 255;
							selected_person_.right_weapon = 255;
							ch::set_person_weapon(selected_person_.get_id(), weaponNum(i));  //ch::set_person_weapon(person_id, 编号)来调用对应模型
							break;
						}
					}
					else if (choose_wu == 3)
					{
						//读取武器的背景图片
						bg_weapon = "background/weapon/w" + j + ".png";
						pk::background(bg_weapon);
						pk::message_box(pk::encode(pk::format("这件\x1b[1x{}\x1b[0x可是难得一遇的兵器呀.", weapon1_name[j])), shangren_0);
						pk::message_box(pk::encode(pk::format("现在一件只要\x1b[1x{}\x1b[0x金.", gold_weapon)), shangren_0);
						confirm = pk::yes_no(pk::encode("买这件吗?"));
						if (confirm)
						{
							scene_confirm(1);   //引用赠予的场景
							selected_person_.left_weapon = 255;
							selected_person_.right_weapon = 255;
							ch::set_person_weapon(selected_person_.get_id(), weaponNum(j));  //ch::set_person_weapon(person_id, 编号)来调用对应模型
							break;
						}
					}
					else if (choose_wu == 4)
					{
						//读取武器的背景图片
						bg_weapon = "background/weapon/" + m + ".png";
						pk::background(bg_weapon);
						pk::message_box(pk::encode(pk::format("这件\x1b[1x{}\x1b[0x可是极其稀有的兵器呀.", weapon2_name[m])), shangren_0);
						pk::message_box(pk::encode(pk::format("现在一件只要\x1b[1x{}\x1b[0x金.", gold_weapon)), shangren_0);
						confirm = pk::yes_no(pk::encode("买这件吗?"));
						if (confirm)
						{
							scene_confirm(1);   //引用赠予的场景
							selected_person_.left_weapon = 255;
							selected_person_.right_weapon = 255;
							ch::set_person_weapon(selected_person_.get_id(), m);  //ch::set_person_weapon(person_id, 编号)来调用对应模型
							break;
						}
					}
					else if (choose_wu == 5)
					{
						//读取武器的背景图片
						bg_weapon = "background/weapon/" + n + ".png";
						pk::background(bg_weapon);
						pk::message_box(pk::encode(pk::format("这件\x1b[1x{}\x1b[0x可是极其稀有的兵器呀.", weapon2_name[n])), shangren_0);
						pk::message_box(pk::encode(pk::format("现在一件只要\x1b[1x{}\x1b[0x金.", gold_weapon)), shangren_0);
						confirm = pk::yes_no(pk::encode("买这件吗?"));
						if (confirm)
						{
							scene_confirm(1);   //引用赠予的场景
							selected_person_.left_weapon = 255;
							selected_person_.right_weapon = 255;
							ch::set_person_weapon(selected_person_.get_id(), n);  //ch::set_person_weapon(person_id, 编号)来调用对应模型
							break;
						}
					}
				}
				
				if (havebuy)
				{
					if (selected_person_.get_id() == kunshu_.get_id())   //选择者为君主时
					{
						pk::play_se(6);
						switch (pk::rand(3))
						{
							case 0: pk::say(pk::encode("霍!哈!嘿!真乃神兵也!"), selected_person_, city); break;
							case 1: pk::say(pk::encode("用着很顺手."), selected_person_, city);  break;
						}
					}
					else
					{
						selected_person_.loyalty = pk::min(255, selected_person_.loyalty + loyalty_add);   //加忠诚
						selected_person_.reward_done = true;      //赏赐完成
						selected_person_.update();
						pk::play_se(6);
						switch (pk::rand(3))
						{
							case 0: pk::say(pk::encode("感谢!"), selected_person_, city); break;
							case 1: pk::say(pk::encode("真是好武器呀!"), selected_person_, city);  break;
							case 2: pk::say(pk::encode("必将以此利器奋力杀敌!"), selected_person_, city);  break;
						}
					}
					pk::add_gold(building_, -gold_weapon, true);
					building_.update();
					pk::history_log(city.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x获得了新武器", selected_person_name)));
				}
				
				//商人名字重置
				shangren_0.sei = shangren_0_sei;
				shangren_0.mei = shangren_0_mei;
				shangren_0.update();
			}
		}
		
		//商人买马列表1
		int scene_malist1(int i, int j, int m, int n, int horse_list)
		{
			int ma_list;
			if (horse_list == 0)  //普通马列表
			{
				array<string> ma_list1 =
				{
					pk::encode(" 取消 "),
					pk::encode("脚力马"),
					pk::encode(pk::format("  {}  ", horse1_name[i])),
					pk::encode(pk::format("  {}  ", horse1_name[j]))
				};
				ma_list = pk::choose(pk::encode("选一匹吧."), ma_list1);
				return ma_list;
			}
			else if (horse_list == 1)  //有一个特殊马列表
			{
				array<string> ma_list1 =
				{
					pk::encode(" 取消 "),
					pk::encode("脚力马"),
					pk::encode(pk::format("  {}  ", horse1_name[i])),
					pk::encode(pk::format("  {}  ", horse1_name[j])),
					pk::encode(pk::format("  {}  ", horse2_name[m]))
				};
				ma_list = pk::choose(pk::encode("选一匹吧."), ma_list1);
				return ma_list;
			}
			else  //有两个特殊马列表
			{
				array<string> ma_list1 =
				{
					pk::encode(" 取消 "),
					pk::encode("脚力马"),
					pk::encode(pk::format("  {}  ", horse1_name[i])),
					pk::encode(pk::format("  {}  ", horse1_name[j])),
					pk::encode(pk::format("  {}  ", horse2_name[m])),
					pk::encode(pk::format("  {}  ", horse2_name[n]))
				};
				ma_list = pk::choose(pk::encode("选一匹吧."), ma_list1);
				return ma_list;
			}
		}
		
		//普通马的编号
		int horseNum(int num)
		{
			int horse_num = 249 + num;  //"红鬃马", "汗血宝马", "灰影马", "黄骠马", "白战马", "骅骝"
			return horse_num;
		}
		
		//商人买武器列表1
		int scene_wulist1(int i, int j, int m, int n, int weapon_list)
		{
			int wu_list;
			if (weapon_list == 0)  //普通武器列表
			{
				array<string> wu_list1 =
				{
					pk::encode("  取消  "),
					pk::encode("  长枪  "),
					pk::encode(pk::format("  {}  ", weapon1_name[i])),
					pk::encode(pk::format("  {}  ", weapon1_name[j]))
				};
				wu_list = pk::choose(pk::encode("挑一件吧."), wu_list1);
				return wu_list;
			}
			else if (weapon_list == 1)  //有一个特殊武器列表
			{
				array<string> wu_list1 =
				{
					pk::encode("  取消  "),
					pk::encode("  长枪  "),
					pk::encode(pk::format("  {}  ", weapon1_name[i])),
					pk::encode(pk::format("  {}  ", weapon1_name[j])),
					pk::encode(pk::format("  {}  ", weapon2_name[m]))
				};
				wu_list = pk::choose(pk::encode("挑一件吧."), wu_list1);
				return wu_list;
			}
			else  //有两个特殊武器列表
			{
				array<string> wu_list1 =
				{
					pk::encode("  取消  "),
					pk::encode("  长枪  "),
					pk::encode(pk::format("  {}  ", weapon1_name[i])),
					pk::encode(pk::format("  {}  ", weapon1_name[j])),
					pk::encode(pk::format("  {}  ", weapon2_name[m])),
					pk::encode(pk::format("  {}  ", weapon2_name[n]))
				};
				wu_list = pk::choose(pk::encode("挑一件吧."), wu_list1);
				return wu_list;
			}
		}
		
		//普通武器的编号
		int weaponNum(int num)
		{
			int weapon_num = 255;  //初始化为普通枪的编号
			switch (num)
			{
				case 0: weapon_num = 3; break;      //长刀的编号
				case 1: weapon_num = 5; break;      //短刀的编号
				case 2: weapon_num = 8; break;      //红缨枪的编号
			}
			return weapon_num;
		}
		
		//确认授予的对象
		void scene_confirm(int horORwer)
		{
			bool person_confirm = false; 
			person_list = getTargetList(building_);
			
			while (!person_confirm)
			{
				person_sel_arr = pk::list_to_array(pk::person_selector( pk::encode("武将选择"), pk::encode("选择要赠予的武将"), person_list, 1, 1) );
				if (person_sel_arr.length == 0)
					person_confirm = false;
				else
				{
					@selected_person_ = person_sel_arr[0];
					selected_person_name = pk::decode(pk::get_name(selected_person_));
					personinfo@ person_t = @person_ex[selected_person_.get_id()];
					
					//horORwer的数值，马为0，武器为1
					if (horORwer == 0)
					{
						//pk::trace(pk::format("horse_id等于{}", person_t.horse_id));
						//马匹编号为特殊马匹
						if (person_t.horse_id < horse2_name.length)
							pk::message_box(pk::encode(pk::format("大人目前所骑的是 \x1b[1x{}\x1b[0x.", horse2_name[person_t.horse_id])), pk::get_person(무장_문관));
						//马匹编号为普通马匹
						else if (person_t.horse_id > 248 and person_t.horse_id < 255)
							pk::message_box(pk::encode(pk::format("大人目前所骑的是 \x1b[1x{}\x1b[0x.", horse1_name[person_t.horse_id - 249])), pk::get_person(무장_문관));
						//马匹编号为脚力马
						else
							pk::message_box(pk::encode("大人目前所骑的是 \x1b[1x脚力马\x1b[0x."), pk::get_person(무장_문관));
					}
					
					else if (horORwer == 1)
					{
						//pk::trace(pk::format("weapon_id等于{}", person_t.weapon_id));
						if (person_t.weapon_id < weapon2_name.length)
							pk::message_box(pk::encode(pk::format("大人目前所使的是 \x1b[1x{}\x1b[0x", weapon2_name[person_t.weapon_id])), pk::get_person(무장_문관));
						else
							pk::message_box(pk::encode("大人目前所使的是 \x1b[1x长枪\x1b[0x."), pk::get_person(무장_문관));
					}
					
					
					if (selected_person_.get_id() == kunshu_.get_id())   //选择者为君主时
						person_confirm = pk::yes_no(pk::encode("是否要留给自己?"));
					else
						person_confirm = pk::yes_no(pk::encode(pk::format("是否要赠予\x1b[1x{}\x1b[0x?", selected_person_name)));
				}
				
			}
		}
				
		 //选择赠予的目标武将列表
		pk::list<pk::person@> getTargetList(pk::building@ building_)
        {
			pk::list<pk::person@> list;
			pk::list<pk::person@> list_t = pk::get_person_list(building_, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般));
			for (int i = 0; i < list_t.count; i++)
			{
                pk::person@ person = list_t[i];
				if (person.location > 86)  continue;  //排除在外部队
				if (person.reward_done == false)  list.add(person);   //仅限未赏赐的武将
			}
			list.add(kunshu_);    //把君主加上，以免发生无人可赏死循环
			return list;
		}
    }

    Main main;
}
// ## 2022/03/23 # 江东新风 # 战争迷雾开局时可选开关 ##
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/01/06 # 江东新风 # 根据新的头像设定重设美女头像 ##
// ## 2021/10/28 # 江东新风 # 单港关存活设定变更,增加对袁家之战,刘备之活跃的支持 ##
// ## 2021/10/26 # 江东新风 # 打开游戏次数记录##
// ## 2021/10/23 # 白马叔叔 # 新增自动分配145将功能 新增年龄随机增减功能 开局港关存活初始化 ##
// ## 2021/10/06 # 江东新风 # 修复刘备208年初会自动灭亡的bug##
// ## 2021/10/01 # 江东新风 # 超过一年，关闭单港关存活，防bug##
// ## 2021/07/01 # 白马叔叔 # 玩家军团优待，修复军团聚集不行动bug##
// ## 2021/04/12 # 白马叔叔 # 地标附近有军事建筑不再重建##
// ## 2021/04/07 # 白马叔叔 # AI优待，修复AI聚集不行动bug##
// ## 2021/03/02 # 江东新风 # 将特殊地名重建的设定也都全部挪至此处 ##
// ## 2021/02/17 # 江东新风 # 针对三顾剧本玩家直接选刘备剧本的情况作了设定 ##
// ## 2021/01/26 # 江东新风 # 针对三顾茅庐剧本的特殊设定 ##
// ## 2021/01/05 # 江东新风 # 只生成前200地名设施的bug修复 ##
// ## 2021/01/05 # 江东新风 # 只生成前200地名设施的bug修复 ##
// ## 2021/01/03 # 江东新风 # 特殊地名设施争夺的完善, 同时进行开局介绍 ##
namespace 特殊地名设施生成
{
	const int 地名建筑重生_发动时期 = 0;  //0: 每旬,  1: 每月第一旬,  2: 每季第一旬,  3: 每年第一旬 默认 1
	const bool 刘备存活测试 = true;
	const bool 自动分配开启 = true;         //是否开启自动分配145将功能。true为开启，false为关闭
	const bool 年龄增减开启 = true;         //是否开启开局随机增减死亡年龄的功能。true为开启，false为关闭
	const int ending = 670;                      //670是史实登场的武将末。设置为1000则为全武将末
	const bool tips_on = true;                 //是否开启载入存档时，提供游戏tips功能。true为开启，false为关闭
	////////////
	//145将的名字
	array<string> person_145_name = { "成廉", "王沈", "殷观", "崔州平", "石韬", 
		"孟建", "宋忠", "殷署", "吕公", "妫览", "秦宜禄", "隐蕃", "尹礼", "路招", "殷礼", 
		"傅干", "吕建", "于毒", "费观", "朱铄", "聂友", "于糜", "卫兹", "应劭", "卫臻", 
		"刘理", "刘永", "朱绩", "阮籍", "袁春卿", "马延", "王观", "张顗", "韩猛", "王嗣", 
		"王思", "王商", "王必", "王方", "王谋", "刘延", "王连", "邯郸淳", "许汜", "王楷", 
		"韩珩", "王子服", "士孙瑞", "郭石", "郝普", "夏侯荣", "夏侯献", "夏侯儒", "夏侯兰", "李乐", 
		"何曾", "荀正", "金尚", "何宗", "贺达", "丁斐", "姜叙", "赵昂", "韦康", "何桢", 
		"吕乂", "何苗", "杨洪", "周群", "韩起", "周邵", "刘敏", "句扶", "张超", "臧洪", 
		"司马芝", "管承", "韩综", "韩忠", "庞淯", "赵俨", "管统", "韩范", "麹演", "许慈", 
		"许定", "刘先", "魏越", "鲜于辅", "赵昱", "嵇康", "常林", "杜袭", "张裔", "句安", 
		"惠衢", "黄琬", "黄射", "耿纪", "侯音", "陶浚", "公孙晃", "周朝", "陶黄", "高蕃", 
		"耿武", "皇甫闿", "习祯", "胡渊", "伍习", "顾邵", "顾承", "谷利", "胡综", "胡冲", 
		"马隆", "吴敦", "吴范", "伍孚", "蔡勋", "崔勇", "左灵", "史涣", "是仪", "史迹", 
		"射援", "射坚", "谢景", "修允", "周胤", "修则", "习珍", "朱盖", "朱光", "朱赞", 
		"柳隐", "李蒙", "李封", "陆胤", "曹安民", "岑威", "昌豨", "徐琨", "徐商", "段煨"};
		
	array<int> arr_age = {0, 5, 10, 15, 20, 30};  //分别对应选项中的年龄
	
	class Main
	{
		pk::list<pk::person@> person_list;
		bool dead_appear = true;
		bool has_been_changed = false;
		Main()
		{
			pk::bind(102, pk::trigger102_t(callback));
			pk::bind(107, pk::trigger107_t(onNewDay));
			pk::bind(164, pk::trigger164_t(afterkillbuilding));
			pk::bind(110, pk::trigger110_t(onNewYear));
			pk::bind(100, pk::trigger100_t(onGameTitle));
			pk::bind(103, pk::trigger103_t(callback_load));
		}

		void onGameTitle()
		{
			if (pk::get_pk2_2_play_times() <= 3)
			{
				pk::person@ 貂蝉 = pk::get_person(武将_貂蝉);
				pk::play_bgm_file(pk::core_dir + "data/media/voice/diaochan/Diaochan6.ogg", false, 60);
				pk::message_box(pk::encode(pk::format("这是您\x1b[27x第{}次\x1b[0x打开\x1b[28x{}\x1b[0x版本。\n关于此版本，需要占用您一点时间简单介绍下。", pk::get_pk2_2_play_times(),pk::get_mod_version())), 貂蝉);
				pk::message_box(pk::encode("目前因\x1b[18x决战制霸\x1b[0x及\x1b[18x教学模式\x1b[0x和\x1b[1x新地图不兼容\x1b[0x，按钮已被禁用，另外\x1b[18x情报履历\x1b[0x如选择\x1b[18x简单\x1b[0x将\x1b[1x错过重要游戏提示\x1b[0x，也已被禁用。\n \x1b[22x请勿费神\x1b[0x寻找打开方式。"), 貂蝉);
				pk::message_box(pk::encode("如遇到开局军师介绍完闪退，系显卡问题，请更换游戏根目录下补丁。"), 貂蝉);
				pk::message_box(pk::encode("默认情况下\x1b[18x不开启迷雾\x1b[0x。\n如需开启迷雾，请将pk2.2文件夹下的对应core2文件重命名替换原文件。"), 貂蝉);
			}
			if (pk::get_pk2_2_play_times() == 1000)
			{
				pk::person@ 小乔 = pk::get_person(武将_小乔);
				pk::message_box(pk::encode(pk::format("这是您\x1b[27x1000次\x1b[0x打开\x1b[28x{}\x1b[0x版本。\n感谢阁下对韩版的厚爱，祝阁下武运昌隆。",pk::get_mod_version())), 小乔);

			}

		}

		void callback()
		{
			if (!pk::get_scenario().loaded)
			{
				//***************因为为一个武将改剧本麻烦，临时设定*************//
				pk::person@ person_1 = pk::get_person(武将_太史慈);//太史慈
				if (pk::decode(pk::get_name(person_1)) == "太史慈") person_1.skill = 239; //
				pk::person@ person_2 = pk::get_person(武将_乐毅);//乐毅
				if (pk::decode(pk::get_name(person_2)) == "乐毅") person_2.skill = 238; //
				//如果剧本未载入，就是开局时，将三顾茅庐剧本的单港关存活开启，防止还没选择刘备就灭亡
				if (check_scenario())
				{
					pk::set_onebase_survive(true);
					//pk::trace(pk::format("剧本载入前，存活：{}", pk::get_onebase_survive()));
					//pk::set_portgate_survive(true);
					//pk::trace(pk::format("存活：{}", pk::core["单港关存活"]));
				}

			}
			if (pk::get_scenario().loaded)
			{
				//如果剧本已载入，则只有玩家设定里有设定单港关存活的，才将其设定为true。
				if (check_scenario())
				{
					settinginfo@ set0 = @setting_ex;
					if (set0.set_port_gate_survive and set0.enable_port_gate_survive)//251也会有这个开启的情况，需确认
					{
						pk::set_onebase_survive(true);
						//pk::trace(pk::format("剧本载入后存活：{}", pk::get_onebase_survive()));
					}
				}
				else pk::set_onebase_survive(false);
				return;//只要剧本已载入，就跳过
			}
			貂蝉开局介绍();
			if (check_scenario()) 单港关存活剧本特殊处理();
		} //callback()

		void 貂蝉开局介绍()
		{
			set_place_facility(40, 0);
			pk::person@ 貂蝉 = pk::get_person(武将_貂蝉);
			pk::message_box(pk::encode("欢迎来到不一样的三国志。"), 貂蝉);
			set_place_facility(40, 1);
			pk::message_box(pk::encode("请容奴家给您简单介绍下这个世界。"), 貂蝉);
			set_place_facility(40, 2);
			int choose = pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("是否观看简要教程?"), 貂蝉);
			set_place_facility(40, 3);
			if (choose == 1)
			{
				set_place_facility(40, 4);
				set_place_facility(40, 5);
			}
			else
			{
				//set_place_facility(40,3);
				pk::message_box(pk::encode("现在\x1b[27x带有地名的设施\x1b[0x将不会被破坏，耐久降为0后将\x1b[1x自动变更归属\x1b[0x。"), 貂蝉);
				set_place_facility(40, 4);
				pk::message_box(pk::encode("敌方势力可能会派遣高武力武将刺杀我方高威胁低武力武将。"), 貂蝉);
				set_place_facility(40, 5);
				pk::message_box(pk::encode("除了与周边国家保持好关系，还需要注重对异族贼兵的防范，他们可能会和其他势力达成交易突袭我方。"), 貂蝉);
				if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("是否继续观看教程?"), 貂蝉) == 0)
				{
					pk::message_box(pk::encode("没事的时候，可以多用君主拜访下武将，会有意外惊喜哦。"), 貂蝉);
					pk::message_box(pk::encode("武将也会生病的，生病的时候记得照顾好他们，免得造成终生遗憾。"), 貂蝉);
					pk::message_box(pk::encode("有时候，情报履历中会告知一些隐世不出的人才，记得关注哦~"), 貂蝉);
				}
			}
			
			bool fog_open = (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("是否需要挑战\x1b[27x战争迷雾\x1b[0x下游玩?\n(战争迷雾下，敌方部队在视野范围外不可见，难度较高，请谨慎选择。)"), 貂蝉) == 0);
			pk::set_fog_state(fog_open,false);


			if (自动分配开启) person_list = get_145_person();  //开启此自动分配功能时，获取存活145将的列表
			if (person_list.count > 0) //如果有145将登场
			{
				if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("是否按史实归属自动配置 \x1b[1x「145将」\x1b[0x?\n自动配置武将到史实所属君主旗下,\n已手动配置过归属的,则不会影响."), 貂蝉) == 0)
				{
					//pk::trace(pk::format("编号：{}，是否假想：{}", pk::get_scenario().no, pk::get_scenario().virtual));
					if (pk::get_scenario().debut == 登场_史实)  //武将是史实登场
					{
						if (!pk::get_scenario().virtual)
						{
							if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("部分「145将」已超过死亡年龄,\n选「是」则已过死亡年龄的\x1b[1x仍然登场\x1b[0x，\n选「否」则已过死亡年龄的\x1b[29x不登场并死亡\x1b[0x."), 貂蝉) == 1) dead_appear = false;  //非寿命无效的，如英雄集结剧本时才询问，选否则实际已经死亡的直接杀死不再配置
						}
						set_145_person_service();  //执行自动配置145将
					}
					else
					{
						if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("目前武将登场设置为「假想」，\n是否仍然自动配置到所属君主旗下?"), 貂蝉) == 0)
						{
							if (!pk::get_scenario().virtual)
							{
								if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("部分「145将」已超过死亡年龄,\n选「是」则已过死亡年龄的\x1b[1x仍然登场\x1b[0x，\n选「否」则已过死亡年龄的\x1b[29x不登场并死亡\x1b[0x."), 貂蝉) == 1) dead_appear = false;  //非寿命无效的，如英雄集结剧本时才询问，选否则实际已经死亡的直接杀死不再配置
							}
							set_145_person_service();  //执行自动配置145将
						}
					}
				}
				if (has_been_changed) pk::message_box(pk::encode("哼！\n你动过妾身喜爱的「145将」了吧？！"), 貂蝉);
			}
			
			if (年龄增减开启 && !pk::get_scenario().virtual && pk::get_scenario().life != 寿命_假想) //如果开启随机年龄功能并且是史实剧本，不是假想99岁
			{
				pk::message_box(pk::encode("为了增加游戏趣味性,\n您可选择在某一范围内随机改变全体武将的寿命哦~"), 貂蝉);
				array<string> answers =
				{
					pk::encode("  依照史实寿命  "),
					pk::encode(pk::format("  {} 年以内随机增减  ", arr_age[1])),
					pk::encode(pk::format("  {}年以内随机增减  ", arr_age[2])),
					pk::encode(pk::format("  {}年以内随机增减  ", arr_age[3])),
					pk::encode(pk::format("  {}年以内随机增减  ", arr_age[4])),
					pk::encode(pk::format("  {}年以内随机增减  ", arr_age[5]))
				};
				int n = pk::choose(pk::encode("是否要随机增减寿命？\n选「史实寿命」则不增不减,\n选「随机增减」则在范围内随机增加或减少数年."), answers);
				if (n == 0) return;  //如果按照史实寿命
				random_age_within(n);  //执行随机年龄增减n
			}
			
			return;		
		}

		void 单港关存活剧本特殊处理()
		{

			if (pk::get_force(1).player != -1)
			{
				pk::person@ 貂蝉 = pk::get_person(武将_貂蝉);
				pk::message_box(pk::encode("阁下果然英武不凡，敢挑战三顾茅庐的刘备势力~"), 貂蝉);
				pk::message_box(pk::encode("需提醒阁下，本剧本中，所有势力主城被灭，\x1b[27x仍可依靠港关存活\x1b[0x。\n如果后续占领城市后，想要关闭该功能，请于\x1b[2xshift右键\x1b[0x菜单关闭。"), 貂蝉);

				pk::set_onebase_survive(true);
				settinginfo@ set0 = @setting_ex;
				set0.set_port_gate_survive = true;
				set0.enable_port_gate_survive = true;
				//pk::trace("cpp 873 ,pos1");
			}
			else
			{
				pk::person@ 貂蝉 = pk::get_person(武将_貂蝉);
				string scenario_name = pk::decode(pk::get_scenario().name);
				pk::message_box(pk::encode(pk::format("阁下现在选择的是\x1b[1x{}\x1b[0x剧本，这可是非常有意思的剧本呀~", scenario_name)), 貂蝉);
				pk::message_box(pk::encode(pk::format("不过\x1b[1x{}\x1b[0x剧本最有意思的势力应该是刘备势力吧。", scenario_name)), 貂蝉);
				pk::message_box(pk::encode("要提醒的是，刘备在本剧本中初始领地只有小城，难度非常高。"), 貂蝉);
				int choise = pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("是否选择扮演刘备势力?"), 貂蝉);
				if (choise == 1)
				{
					if (刘备存活测试)
					{
						pk::set_onebase_survive(true);
						settinginfo@ set0 = @setting_ex;
						set0.set_port_gate_survive = true;
						set0.enable_port_gate_survive = true;
						//pk::trace("cpp 873 ,pos1");
					}
					return;
				}
				pk::message_box(pk::encode("哈哈，阁下果然英武不凡，敢挑战刘备势力~\n那就预祝阁下武运昌隆吧~"), 貂蝉);
				pk::message_box(pk::encode("对了，还需提醒阁下，本剧本中，所有势力主城被灭，\x1b[27x仍可依靠港关存活\x1b[0x。\n如果后续占领城市后，想要关闭该功能，请于\x1b[2xshift右键\x1b[0x菜单关闭。"), 貂蝉);
				for (int i = 0; i < 非贼势力_末; ++i)
				{
					pk::force@ force0 = pk::get_force(i);
					if (force0.player == 0)
					{
						force0.player = -1;
						force0.update();
					}
				}
				pk::force@ force1 = pk::get_force(1);
				force1.player = 0;
				force1.update();
				pk::set_onebase_survive(true);
				settinginfo@ set0 = @setting_ex;
				set0.set_port_gate_survive = true;
				set0.enable_port_gate_survive = true;
				//pk::trace("cpp 873 ,pos1");
				//pk::trace("cpp 873 ,pos2");
				//settinginfo@ set1() = @setting_ex;
				//pk::trace(pk::format("873,{}{}", set1.set_port_gate_survive, set1.enable_port_gate_survive));
				return;
			}



		}

		void afterkillbuilding(/*pk::unit@ attacker,*/ int force_id, int facility_id, const pk::point& in pos)
		{
			ch::特殊地名争夺处理(facility_id, force_id, pos);
			/*
			if (地名设施.find(facility_id) >= 0)
			{
				//pk::trace(pk::format("目标为特殊地名的设施类型，id为：{}", facility_id));
				pk::point pos0;
				for (int i = 0; i < int(特殊地名设施.length); i++)
				{
					pos0.x = 特殊地名设施[i][1];
					pos0.y = 特殊地名设施[i][2];
					//确保不被改建后的设施左右
					facility_id = 特殊地名设施[i][0];
					if (pos0.x == pos.x and pos0.y == pos.y)
					{
						//pk::trace(pk::format("目标为所在地格为特殊设施类型，x:{},y:{}", facility_id, pos.x, pos.y));

						if (force_id != -1)
						{
							pk::force@ force0 = pk::get_force(force_id);
							if (facility_id == 设施_连弩橹 or facility_id == 设施_箭楼)
								facility_id = pk::has_tech(force0, 技巧_施设强化)? 设施_连弩橹: 设施_箭楼;
							else if (facility_id == 设施_阵 or facility_id == 设施_砦 or facility_id == 设施_城塞)
								facility_id = pk::has_tech(force0, 技巧_设施强化) ? (pk::has_tech(force0, 技巧_城壁强化)? 设施_城塞 : 设施_砦) : 设施_阵;
							pk::trace(pk::format("最终生成设施类型，id为：{}", facility_id));
						}

						pk::facility@ destroy_facility = pk::get_facility(facility_id);
						pk::building@ building0 = pk::create_building(destroy_facility, pos, force_id);
						pk::complete(building0);
					}
				}
			}*/

		}

		void onNewDay()
		{
			change_place_facility(231, 0);
			if (地名建筑重生_发动时期 == 0)
				revive_place_facility(231, 0);
			else if (地名建筑重生_发动时期 == 1 and (pk::get_day() == 1))
				revive_place_facility(231, 0);
			else if (地名建筑重生_发动时期 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
				revive_place_facility(231, 0);
			else if (地名建筑重生_发动时期 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
				revive_place_facility(231, 0);

			//加入对三顾剧本单港关存活的开关，超过一年自动关闭

		}

		void onNewYear()
		{
			//加入对三顾剧本单港关存活的开关，超过一年自动关闭
			if (pk::get_elapsed_days() >= 720)//36旬为1年---这个函数返回值是一旬返回10天
			{
				//pk::trace(pk::format("时间大于1年，关闭单港关存活，目前经过天数：{}", pk::get_elapsed_days()));
				pk::set_onebase_survive(false);
				settinginfo@ set0 = @setting_ex;
				set0.set_port_gate_survive = false;
				set0.enable_port_gate_survive = false;
			}
			return;

		}

		void set_place_facility(int per_length, int order)
		{
			int start = per_length * order;
			int end = start + per_length;
			if (start > int(特殊地名设施.length)) return;
			if (end > int(特殊地名设施.length)) end = int(特殊地名设施.length);
			for (int i = start; i < end; i++)
			{
				pk::point pos;
				pos.x = 特殊地名设施[i][1];
				pos.y = 特殊地名设施[i][2];
				pk::hex@ hex0 = pk::get_hex(pos);
				int force_id = -1;    //原本是-1无归属，改成46
				pk::building@ area_building = pk::get_building(pk::get_building_id(pos));
				if (pk::is_alive(area_building)) force_id = area_building.get_force_id();
				if (hex0.has_building) pk::kill(pk::get_building(pos));
				pk::building@ building0 = pk::create_spec(pk::get_facility(特殊地名设施[i][0]), pos, force_id,i);//改成重建地名建筑专用函数
				pk::complete(building0);
			}
		}

		void revive_place_facility(int per_length, int order)
		{
			int start = per_length * order;
			int end = start + per_length;
			if (start > int(特殊地名设施.length)) return;
			if (end > int(特殊地名设施.length)) end = int(特殊地名设施.length);
			for (int i = start; i < end; i++)
			{
				pk::point pos;
				pos.x = 特殊地名设施[i][1];
				pos.y = 特殊地名设施[i][2];
				pk::hex@ hex0 = pk::get_hex(pos);
				if (hex0.has_building or hex0.has_unit) continue;
				if (military_building_around(pos, 1, 2)) continue;  //2格内是否有军事设施
				int force_id = -1;    //原本是-1无归属，改成46
				//pk::building@ area_building = pk::get_building(pk::get_building_id(pos));
				//if (pk::is_alive(area_building)) force_id = area_building.get_force_id();			
				//if (hex0.has_building) pk::kill(pk::get_building(pos));
				pk::building@ building0 = pk::create_spec(pk::get_facility(特殊地名设施[i][0]), pos, -1,i);//改成重建地名建筑专用函数
				pk::complete(building0);
			}
		}

		void change_place_facility(int per_length, int order)
		{
			int start = per_length * order;
			int end = start + per_length;
			if (start > int(特殊地名设施.length)) return;
			if (end > int(特殊地名设施.length)) end = int(特殊地名设施.length);
			for (int i = start; i < end; i++)
			{
				pk::point pos;
				pos.x = 特殊地名设施[i][1];
				pos.y = 特殊地名设施[i][2];
				pk::hex@ hex0 = pk::get_hex(pos);
				if (hex0.has_unit) continue;
				if (!hex0.has_building) continue;  //地标上没有建筑物，跳过
				pk::city@ area_city = pk::get_city(pk::get_city_id(pos));
				if (area_city.is_player())  //是玩家势力
				{
					if (!pk::is_player_controlled(area_city)) //是玩家的委任军团
					{
						if (!pk::is_in_screen(pos) and (pk::get_building(pos)).get_force_id() == -1)  //pos不在屏幕内且地标建筑无归属时
						{
							pk::building@ spec_building = pk::get_building(pos);
							int building_id = spec_building.get_id();
							int spec_id = ch::to_spec_id(building_id);
							if (!ch::is_valid_spec_id(spec_id)) continue;
							spec_building.init_force = 势力_盗贼;
							//pk::set_district(spec_building, pk::get_district(pk::get_force(势力_盗贼).get_district_id()), /*占领*/势力_盗贼);
							pk::history_log(pos, 势力_盗贼, pk::encode(pk::format("\x1b[1x{}\x1b[0x周围被贼兵占据了.", 特殊地名设施名称[spec_id])));
						}
					}
					continue;
				}
				if (area_city.get_force_id() == -1) continue;  //空城无势力归属跳过
				if (!pk::is_in_screen(pos) and (pk::get_building(pos)).get_force_id() == -1)  //pos不在屏幕内且地标建筑无归属时
				{
					pk::building@ spec_building = pk::get_building(pos);
					int building_id = spec_building.get_id();
					int spec_id = ch::to_spec_id(building_id);
					if (!ch::is_valid_spec_id(spec_id)) continue;

					pk::set_district(spec_building, pk::get_district(pk::get_force(势力_盗贼).get_district_id()), /*占领*/势力_盗贼);
					//pk::history_log(pos, 势力_盗贼, pk::encode(pk::format("\x1b[1x{}\x1b[0x周围被贼兵占据了.", 特殊地名设施名称[spec_id])));
				}
			}
		}

		//附近是否存在军事设施
		bool military_building_around(pk::point pos, int min_distance, int max_distance)
		{
			auto arr = pk::range(pos, min_distance, max_distance);
			for (int i = 0; i < int(arr.length); i++)
			{
				if ((pk::get_hex(arr[i])).has_building)
				{
					auto building_0 = pk::get_building(arr[i]);
					if ((building_0.facility >= 设施_阵 and building_0.facility <= 设施_连弩橹) or (building_0.facility >= 设施_投石台 and building_0.facility <= 设施_根据地2))
						return true;
				}
			}
			return false;
		}

		bool check_scenario()
		{
			if (pk::get_scenario().no == 4)
			{
				string scenario_name = pk::decode(pk::get_scenario().name);
				if (scenario_name == "三顾茅庐" or scenario_name == "刘备之活跃")
					return true;
			}
			else if (pk::get_scenario().no == 9)
			{
				string scenario_name = pk::decode(pk::get_scenario().name);
				if (scenario_name == "袁家之战")
					return true;
			}
			return false;
		}
		
		//按照史实归属配置145将，剧本、姓名都要一致
		void set_145_person_service()
		{
			int person_list_count = person_list.count;
			for (int j = 0; j < person_list_count; j++)
			{
				pk::person@ person_i = person_list[j];
				int person_id = person_i.get_id();
				
				if (!dead_appear && pk::get_scenario().start_year > person_i.death)
				{
					//pk::trace(pk::format("被杀死id：{}，名字：{}，身份：{}", person_id, pk::decode(pk::get_name(person_i)), person_i.mibun));
					pk::kill(person_i,null,null,null,1);  //直接杀死符合条件的人
					continue;
				}
				//pk::trace(pk::format("id：{}，名字：{}，身份：{}", person_id, pk::decode(pk::get_name(person_i)), person_i.mibun));
				int kunshu_id = -1;  //归属的君主id初始化
				switch (person_id)
				{
					case 855 : kunshu_id = 660; break;  //成廉-吕布
					case 856 : kunshu_id = 228; break;  //王沈-司马昭
					case 857 : kunshu_id = 635; break;  //殷观-刘备
					case 858 : kunshu_id = -1; break;  //崔州平-不仕
					case 859 : kunshu_id = pk::get_scenario().start_year < 208 ? 708 : 343; break;  //石韬-司马徽和曹操
					case 860 : kunshu_id = pk::get_scenario().start_year < 208 ? 708 : 343; break;  //孟建-司马徽和曹操
					case 861 : kunshu_id = 636; break;  //宋忠-刘表
					case 862 : kunshu_id = 343; break;  //殷署-曹操
					case 863 : kunshu_id = 636; break;  //吕公-刘表
					case 864 : kunshu_id = 367; break;  //妫览-孙权
					case 865 : kunshu_id = 660; break;  //秦宜禄-吕布
					case 866 : kunshu_id = 328; break;  //隐蕃-曹叡
					case 867 : kunshu_id = 346; break;  //尹礼-臧霸
					case 868 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 328; break;  //路招-曹操和曹叡
					case 869 : kunshu_id = 367; break;  //殷礼-孙权
					case 870 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 328; break;  //傅干-曹操和曹叡
					case 871 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 328; break;  //吕建-曹操和曹叡
					case 872 : kunshu_id = -1; break;  //于毒-山贼无
					case 873 : kunshu_id = pk::get_scenario().start_year < 215 ? 624 : (pk::get_scenario().start_year < 224 ? 635 : 629); break;  //费观-刘璋和刘备父子
					case 874 : kunshu_id = pk::get_scenario().start_year < 227 ? 347: 328; break;  //朱铄-曹丕父子
					case 875 : kunshu_id = 367; break;  //聂友-孙权
					case 876 : kunshu_id = 642; break;  //于糜-刘繇
					case 877 : kunshu_id = 343; break;  //卫兹-曹操
					case 878 : kunshu_id = pk::get_scenario().start_year < 194 ? 189 : 18; break;  //应劭-皇甫嵩（汉）和袁绍
					case 879 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 328; break;  //卫臻-曹操和曹叡
					case 880 : kunshu_id = 629; break;  //刘理-刘禅
					case 881 : kunshu_id = 629; break;  //刘永-刘禅
					case 882 : kunshu_id = 367; break;  //朱绩-孙权
					case 883 : kunshu_id = -1; break;  //阮籍-不仕
					case 884 : kunshu_id = 18; break;  //袁春卿-袁绍
					case 885 : kunshu_id = pk::get_scenario().start_year < 208 ? 17 : 343; break;  //马延-袁尚和曹操
					case 886 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 225; break;  //王观-曹操和司马懿
					case 887 : kunshu_id = 17; break;  //张顗-袁尚
					case 888 : kunshu_id = 18; break;  //韩猛-袁绍
					case 889 : kunshu_id = 629; break;  //王嗣-刘禅
					case 890 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 328; break;  //王思-曹操和曹叡
					case 891 : kunshu_id = 624; break;  //王商-刘璋
					case 892 : kunshu_id = 343; break;  //王必-曹操
					case 893 : kunshu_id = 493; break;  //王方-董卓
					case 894 : kunshu_id = pk::get_scenario().start_year < 215 ? 624 : (pk::get_scenario().start_year < 224 ? 635 : 629); break;  //王谋-刘璋和刘备父子
					case 895 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 347; break;  //刘延-曹操父子
					case 896 : kunshu_id = pk::get_scenario().start_year < 215 ? 624 : (pk::get_scenario().start_year < 224 ? 635 : 629); break;  //王连-刘璋和刘备父子
					case 897 : kunshu_id = pk::get_scenario().start_year < 208 ? 708 : 343; break;  //邯郸淳-司马徽和曹操
					case 898 : kunshu_id = pk::get_scenario().start_year < 199 ? 660 : 636; break;  //许汜-吕布和刘表
					case 899 : kunshu_id = 660; break;  //王楷-吕布
					case 900 : kunshu_id = 13; break;  //韩珩-袁熙
					case 901 : kunshu_id = 491; break;  //王子服-董承
					case 902 : kunshu_id = 189; break;  //士孙瑞-皇甫嵩
					case 903 : kunshu_id = -1; break;  //郭石-山贼无
					case 904 : kunshu_id = pk::get_scenario().start_year < 224 ? 635 : 367; break;  //郝普-刘备和孙权
					case 905 : kunshu_id = 343; break;  //夏侯荣-曹操
					case 906 : kunshu_id = 328; break;  //夏侯献-曹叡
					case 907 : kunshu_id = 328; break;  //夏侯儒-曹叡
					case 908 : kunshu_id = pk::get_scenario().start_year < 208 ? 85 : 635; break;  //夏侯兰-夏侯惇和刘备
					case 909 : kunshu_id = -1; break;  //李乐-山贼无
					case 910 : kunshu_id = 228; break;  //何曾-司马昭
					case 911 : kunshu_id = 16; break;  //荀正-袁术
					case 912 : kunshu_id = 189; break;  //金尚-皇甫嵩
					case 913 : kunshu_id = pk::get_scenario().start_year < 215 ? 624 : (pk::get_scenario().start_year < 224 ? 635 : 629); break;  //何宗-刘璋和刘备父子
					case 914 : kunshu_id = 367; break;  //贺达-孙权
					case 915 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 328; break;  //丁斐-曹操和曹叡
					case 916 : kunshu_id = pk::get_scenario().start_year < 215 ? -1 : 347; break;  //姜叙-韦康和曹丕
					case 917 : kunshu_id = pk::get_scenario().start_year < 215 ? -1 : 343; break;  //赵昂-韦康和曹操
					case 918 : kunshu_id = pk::get_scenario().start_year < 197 ? -1 : 343; break;  //韦康-东汉地方政权和曹操
					case 919 : kunshu_id = 228; break;  //何桢-司马昭
					case 920 : kunshu_id = pk::get_scenario().start_year < 215 ? 624 : (pk::get_scenario().start_year < 224 ? 635 : 629); break;  //吕乂-刘璋和刘备父子
					case 921 : kunshu_id = 91; break;  //何苗-何进
					case 922 : kunshu_id = pk::get_scenario().start_year < 215 ? 624 : (pk::get_scenario().start_year < 224 ? 635 : 629); break;  //杨洪-刘璋和刘备父子
					case 923 : kunshu_id = pk::get_scenario().start_year < 215 ? 624 : (pk::get_scenario().start_year < 224 ? 635 : 629); break;  //周群-刘璋和刘备父子
					case 924 : kunshu_id = 177; break;  //韩起-公孙渊
					case 925 : kunshu_id = 367; break;  //周邵-孙权
					case 926 : kunshu_id = pk::get_scenario().start_year < 224 ? 635 : 629; break;  //刘敏-刘备父子
					case 927 : kunshu_id = pk::get_scenario().start_year < 224 ? 635 : 629; break;  //句扶-刘备父子
					case 928 : kunshu_id = 430; break;  //张超-张邈
					case 929 : kunshu_id = -1; break;  //臧洪-东汉地方政权
					case 930 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 328; break;  //司马芝-曹操和曹叡
					case 931 : kunshu_id = -1; break;  //管承-山贼无
					case 932 : kunshu_id = 367; break;  //韩综-孙权
					case 933 : kunshu_id = 179; break;  //韩忠-公孙康
					case 934 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 347; break;  //庞淯-曹操父子
					case 935 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 328; break;  //赵俨-曹操和曹叡
					case 936 : kunshu_id = 20; break;  //管统-袁谭
					case 937 : kunshu_id = 17; break;  //韩范-袁尚
					case 938 : kunshu_id = -1; break;  //麹演-地方
					case 939 : kunshu_id = pk::get_scenario().start_year < 215 ? 143 : (pk::get_scenario().start_year < 224 ? 635 : 629); break;  //许慈-许靖和刘备父子
					case 940 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 347; break;  //许定-曹操父子
					case 941 : kunshu_id = pk::get_scenario().start_year < 208 ? 636 : (pk::get_scenario().start_year < 221 ? 343 : 347); break;  //刘先-刘表和曹操父子
					case 942 : kunshu_id = 660; break;  //魏越-吕布
					case 943 : kunshu_id = pk::get_scenario().start_year < 200 ? 15 : (pk::get_scenario().start_year < 221 ? 343 : 328); break;  //鲜于辅-阎柔和曹操曹叡
					case 944 : kunshu_id = 484; break;  //赵昱-陶谦
					case 945 : kunshu_id = -1; break;  //嵇康-不仕
					case 946 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 328; break;  //常林-曹操和曹叡
					case 947 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 328; break;  //杜袭-曹操和曹叡
					case 948 : kunshu_id = pk::get_scenario().start_year < 215 ? 624 : (pk::get_scenario().start_year < 224 ? 635 : 629); break;  //张裔-刘璋和刘备父子
					case 949 : kunshu_id = 629; break;  //句安-刘禅
					case 950 : kunshu_id = 16; break;  //惠衢-袁术
					case 951 : kunshu_id = 189; break;  //黄琬-皇甫嵩
					case 952 : kunshu_id = 636; break;  //黄射-刘表
					case 953 : kunshu_id = 343; break;  //耿纪-曹操
					case 954 : kunshu_id = 343; break;  //侯音-曹操
					case 955 : kunshu_id = 368; break;  //陶浚-孙皓
					case 956 : kunshu_id = 178; break;  //公孙晃-公孙恭
					case 957 : kunshu_id = -1; break;  //周朝-山贼无
					case 958 : kunshu_id = 368; break;  //陶黄-孙皓
					case 959 : kunshu_id = 17; break;  //高蕃-袁尚
					case 960 : kunshu_id = 120; break;  //耿武-韩馥
					case 961 : kunshu_id = 263; break;  //皇甫闿-钟会
					case 962 : kunshu_id = 635; break;  //习祯-刘备
					case 963 : kunshu_id = 226; break;  //胡渊-司马炎
					case 964 : kunshu_id = 65; break;  //伍习-郭汜
					case 965 : kunshu_id = 211; break;  //顾邵-孙权
					case 966 : kunshu_id = 965; break;  //顾承-孙权
					case 967 : kunshu_id = 367; break;  //谷利-孙权
					case 968 : kunshu_id = pk::get_scenario().start_year < 201 ? 370 : 367; break;  //胡综-孙策兄弟
					case 969 : kunshu_id = 368; break;  //胡冲-孙皓
					case 970 : kunshu_id = 226; break;  //马隆-司马炎
					case 971 : kunshu_id = 346; break;  //吴敦-臧霸
					case 972 : kunshu_id = 367; break;  //吴范-孙权
					case 973 : kunshu_id = 189; break;  //伍孚-皇甫嵩
					case 974 : kunshu_id = 636; break;  //蔡勋-刘表
					case 975 : kunshu_id = 65; break;  //崔勇-郭汜
					case 976 : kunshu_id = 598; break;  //左灵-李傕
					case 977 : kunshu_id = 343; break;  //史涣-曹操
					case 978 : kunshu_id = 367; break;  //是仪-孙权
					case 979 : kunshu_id = 367; break;  //史迹-孙权
					case 980 : kunshu_id = pk::get_scenario().start_year < 215 ? 624 : (pk::get_scenario().start_year < 224 ? 635 : 629); break;  //射援-刘璋和刘备父子
					case 981 : kunshu_id = pk::get_scenario().start_year < 215 ? 624 : (pk::get_scenario().start_year < 224 ? 635 : 629); break;  //射坚-刘璋和刘备父子
					case 982 : kunshu_id = 367; break;  //谢景-孙权
					case 983 : kunshu_id = 368; break;  //修允-孙皓
					case 984 : kunshu_id = 367; break;  //周胤-孙权
					case 985 : kunshu_id = 367; break;  //修则-孙权
					case 986 : kunshu_id = 635; break;  //习珍-刘备
					case 987 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 347; break;  //朱盖-曹操父子
					case 988 : kunshu_id = 343; break;  //朱光-曹操
					case 989 : kunshu_id = 340; break;  //朱赞-曹真
					case 990 : kunshu_id = 629; break;  //柳隐-刘禅
					case 991 : kunshu_id = 493; break;  //李蒙-董卓
					case 992 : kunshu_id = 660; break;  //李封-吕布
					case 993 : kunshu_id = 367; break;  //陆胤-孙权
					case 994 : kunshu_id = 343; break;  //曹安民-曹操
					case 995 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 328; break;  //岑威-曹操和曹叡
					case 996 : kunshu_id = -1; break;  //昌豨-山贼无
					case 997 : kunshu_id = pk::get_scenario().start_year < 193 ? 365 : (pk::get_scenario().start_year < 201 ? 370 : 367);  //徐琨-孙坚孙策孙权
					case 998 : kunshu_id = pk::get_scenario().start_year < 221 ? 343 : 328; break;  //徐商-曹操和曹叡
					case 999 : kunshu_id = pk::get_scenario().start_year < 193 ? 493 : 343; break;  //段煨-董卓和曹操
				}
				if (kunshu_id == -1) continue;  //属于不仕的或其他，这种不需要配置
				//如果有父亲已经登场的或没登场的，则移动到父亲所属的城市，无视kunshu_id  如果父亲已经死了的，则执行kunshu_id
				int case_0 = -1;  //情形初始化
				if (person_i.father == -1) case_0 = 0;  //执行情形0，该武将没有父亲，直接按照君主登场
				else if (pk::get_person(person_i.father).mibun == 身份_无 || pk::get_person(person_i.father).mibun == 身份_俘虏 || pk::get_person(person_i.father).mibun == 身份_死亡) case_0 = 0;  //执行情形0，该武将有父亲，但父亲已经死亡或者没有被选中登场，直接按照君主登场
				else if (pk::get_person(person_i.father).mibun >= 身份_君主 && pk::get_person(person_i.father).mibun <= 身份_一般) case_0 = 1;  //执行情形1，该武将有父亲，父亲是正常在职武将时，直接照父亲登场
				else if (pk::get_person(person_i.father).mibun == 身份_在野 || pk::get_person(person_i.father).mibun == 身份_未发现 || pk::get_person(person_i.father).mibun == 身份_未登场) case_0 = 1;  //执行情形1，该武将有父亲，但父亲是不是在职武将时，直接移动到父亲所在地
				
				if (case_0 == 0)
				{
					pk::person@ kunshu_0 = pk::get_person(kunshu_id);
					if ((kunshu_0.mibun == 身份_无 && pk::decode(pk::get_name(kunshu_0)) != "司马徽") || kunshu_0.mibun == 身份_俘虏 || kunshu_0.mibun == 身份_死亡) continue;  //如果君主已经死了则跳过，司马徽特殊处理，可以让石韬等弟子跟随
					
					//是正常在职武将时，并且是可以登场的情况下，登场并归属
					auto building_t = pk::get_building(kunshu_0.service);
					if ((kunshu_0.mibun >= 身份_君主 && kunshu_0.mibun <= 身份_一般) && (pk::get_scenario().virtual || (!pk::get_scenario().virtual && pk::get_scenario().start_year >= person_i.appearance))) 
					{
						set_person_district(@person_i, building_t);  //把他设置到历史归属的君主旗下
						pk::trace(pk::format("登场者id：{}，名字：{}，地点：{}，归属君主：{}", person_id, pk::decode(pk::get_name(person_i)), pk::decode(pk::get_name(building_t)), pk::decode(pk::get_name(kunshu_0))));
					}
					else
					{
						pk::move(person_i, building_t);	 //移动到历史归属君主所属据点
						pk::trace(pk::format("移动者id：{}，名字：{}，地点：{}，归属君主：{}", person_id, pk::decode(pk::get_name(person_i)), pk::decode(pk::get_name(building_t)), pk::decode(pk::get_name(kunshu_0))));
					}
				}
				else if (case_0 == 1)
				{
					pk::person@ father_0 = pk::get_person(person_i.father);
					if (father_0.mibun == 身份_无 || father_0.mibun == 身份_俘虏 || father_0.mibun == 身份_死亡) continue;  //如果君主已经死了则跳过
					
					//是正常在职武将时，并且是可以登场的情况下，登场并归属
					auto building_t = pk::get_building(father_0.service);
					if ((father_0.mibun >= 身份_君主 && father_0.mibun <= 身份_一般) && (pk::get_scenario().virtual || (!pk::get_scenario().virtual && pk::get_scenario().start_year >= person_i.appearance))) 
					{
						set_person_district(@person_i, building_t);  //把他设置到父亲的旗下
						pk::trace(pk::format("登场者id：{}，名字：{}，地点：{}，父亲：{}", person_id, pk::decode(pk::get_name(person_i)), pk::decode(pk::get_name(building_t)), pk::decode(pk::get_name(father_0))));
					}
					else
					{
						pk::move(person_i, building_t);	 //移动到父亲所属据点
						pk::trace(pk::format("移动者id：{}，名字：{}，地点：{}，父亲：{}", person_id, pk::decode(pk::get_name(person_i)), pk::decode(pk::get_name(building_t)), pk::decode(pk::get_name(father_0))));
					}
				}
			}
		}
		
		//获取有效的145将列表
		pk::list<pk::person@> get_145_person()
		{
			pk::list<pk::person@> list;
			for (int i = 855; i < 1000; i++)
			{
				pk::person@ person_0 = pk::get_person(i);
				if (person_0 is null) continue;  //继续
				if (person_0.mibun == 身份_无 || person_0.mibun == 身份_死亡 || person_0.mibun == 身份_君主 || person_0.mibun == 身份_都督 || person_0.mibun == 身份_一般 || person_0.mibun == 身份_在野) continue;  //如果身份是这些则跳过，这些身份是被手动配置过的
				if (pk::decode(pk::get_name(person_0)) != person_145_name[i-855])
				{
					has_been_changed = true;  //说明可能被修改过，则标记
					continue;  //姓名没有对上，跳过
				}
				list.add(person_0);
			}
			return list;
		}
		
		void set_person_district(pk::person@ person_i, pk::building@ building_t)
		{
			pk::set_district(person_i, building_t.get_district_id());  //设置到军团
			pk::move(person_i, building_t);	 //移动到据点
			person_i.mibun = 身份_一般;  //身份一般
			person_i.rank = 官职_无;  //官职无
			person_i.loyalty = 100 - int(pk::get_aishou_distance(person_i, pk::get_kunshu_id(building_t)) / 3) - (pk::is_dislike(person_i, pk::get_kunshu_id(building_t)) ? 10 : 0);//是否厌恶君主，忠诚度
			person_i.update();
		}
		
		//随机增减寿命功能
		void random_age_within(int n)
		{
			for (int i = 0; i < ending; i++)
			{
				auto person_0 = pk::get_person(i);
				if (person_0 is null) continue;  //继续
				if (person_0.mibun == 身份_无 || person_0.mibun == 身份_死亡) continue;  //如果身份是这些则跳过
				//下限从（武将登场一年后）或（剧本开局一年后）开始随机，而不是先随机再取最大值，概率略有不同（暂不用此方案）
				//person_0.death = ch::randint(pk::max((pk::get_scenario().start_year + 1), pk::max((person_0.appearance + 1), (person_0.death - arr_age[n]))), person_0.death + arr_age[n]);  
				
				auto random_n = ch::randint(-arr_age[n], arr_age[n]);  //年龄随机增减的数
				//pk::trace(pk::format("名字：{}，增减的年数：{}", pk::decode(pk::get_name(person_0)), random_n));
				//取（原死亡年份+随机数）和（武将登场一年后）和（剧本开局一年后）的最大值，至少是（武将登场一年后）或（剧本开局一年后），年龄可破100岁
				person_0.death = pk::max((pk::get_scenario().start_year + 1), pk::max((person_0.appearance + 1), person_0.death + random_n));
				person_0.update();
			}
		}
		
		void callback_load()
		{
			uint value = pk::get_tick_count();
			pk::get_scenario().seed = value;
			if (tips_on) pk::scene(pk::scene_t(scene_开局tips));  //载入存档开局tips
		}
		
		//貂蝉载入存档的tips
		void scene_开局tips()
		{
	
			pk::person@ 美女 = pk::get_person(武将_町娘);
			//给美女改个名字
			int face_0 = 美女.face;
			string sei_0 = 美女.sei;
			string mei_0 = 美女.mei;
			
			array<int> beauty_face = {武将_貂蝉, 武将_甄氏, 武将_王异, 武将_张春华, 武将_邹氏, 武将_大乔, 武将_小乔, 2205, 武将_黄月英, 武将_孙尚香};                     //头像ID
			array<string> beauty_sei = {pk::encode("貂"), pk::encode("甄"), pk::encode("王"), pk::encode("张"), pk::encode("邹"), pk::encode("大"), pk::encode("小"), pk::encode("王"), pk::encode("黄"), pk::encode("孙")};                //姓
			array<string> beauty_mei = {pk::encode("蝉"), pk::encode("氏"), pk::encode("异"), pk::encode("春华"), pk::encode("氏"), pk::encode("乔"), pk::encode("乔"), pk::encode("元姬"), pk::encode("月英"), pk::encode("尚香")};  //名
			
			int random_m = pk::rand(10, int(pk::get_tick_count()), int(pk::get_tick_count()));
			美女.face = beauty_face[random_m];
			美女.sei = beauty_sei[random_m];
			美女.mei = beauty_mei[random_m];
			美女.update();

			/* random_seed += 1;
			random_seed = random_seed % 7;  //循环加1 */
			int random_seed = pk::rand(16, int(pk::get_tick_count()), int(pk::get_tick_count()));
			switch (random_seed)
			{
				case 0 : pk::message_box(pk::encode("现在\x1b[27x带有地名的设施\x1b[0x将不会被破坏，耐久降为0后将\x1b[1x自动变更归属\x1b[0x。"), 美女); break;
				case 1 : pk::message_box(pk::encode("敌势力会派遣武将来刺杀我方低武力的高威胁武将，拥有「\x1b[27x护卫\x1b[0x」特技的武将有几率帮助抵御，尽量提升与护卫的关系吧！"), 美女); break;
				case 2 : pk::message_box(pk::encode("后方城市还需要注重对「\x1b[27x异族贼兵\x1b[0x」的防范，他们可能会和其他势力达成交易突袭我方。"), 美女); break;
				case 3 : pk::message_box(pk::encode("游戏的「情报履历」要开启并设置为「详细」，避免错过重要情报哦~"), 美女); break;
				case 4 : pk::message_box(pk::encode("「异族贼兵」与其他势力达成协议进攻我方，可不管你城市的治安高低与否哦~他们给的实在太多了。"), 美女); break;
				case 5 : pk::message_box(pk::encode("武将也会生病的，生病的时候记得照顾好他们，免得造成终生遗憾。"), 美女); break;
				case 6 : pk::message_box(pk::encode("部队尽量文武搭配，可以避免因为武将的判断失误，致使周围部队陷入混乱哦~"), 美女); break;
				case 7 : pk::message_box(pk::encode("钱粮搬运途中会因各种意外而损失物资！可以交给拥有「\x1b[27x搬运\x1b[0x」特技的武将，不会造成物资的丢失~"), 美女); break;
				case 8: pk::message_box(pk::encode("现在征兵将消耗\x1b[27x现兵役人口\x1b[0x，人口不足时将招不到兵哦~"), 美女); break;
				case 9: pk::message_box(pk::encode("兵临城下的话，\x1b[27x人口\x1b[0x会大量减少，尽量防止被兵临城下吧~"), 美女); break;
				case 10: pk::message_box(pk::encode("一些看着正常的地格可能潜伏着\x1b[27x伏兵\x1b[0x，所以在其他势力范围内行军时或攻击时，切记需谨慎行事，一旦中伏可能全军覆没。"), 美女); break;
				case 11: pk::message_box(pk::encode("没事不要将\x1b[1x运输队\x1b[0x在敌军境内行动哦~ \n如有必要请务必设置\x1b[2x护卫部队\x1b[0x，否则一旦被敌军突袭，后果不堪设想。"), 美女); break;
				case 12: pk::message_box(pk::encode("当\x1b[27x初次接触\x1b[0x敌方部队时（尤其在一些视野受限的地形）需留个心眼，人家或许潜伏着大量部队，只等你上钩呢。让冷静且智商在线的武将打前锋或许是个好主意。"), 美女); break;
				case 13: pk::message_box(pk::encode("现在敌军进攻时根据情况可能会同时请求一些\x1b[27x异族\x1b[0x势力帮忙，留守兵力太少的话，或许会被异族捡漏哦~"), 美女); break;
				case 14: pk::message_box(pk::encode("不要以为包围敌军城市就万事大吉了，固守的敌军也可能会使出各种\x1b[27x城市计略\x1b[0x与你周旋的，十则围之，可别阴沟里翻船哦~"), 美女); break;
				case 15: pk::message_box(pk::encode("随人口增加(20万,30万,40万,60万)，城市规模会变大，内政用地数量也会增加~所以一定要善待领地内子民呀!~"), 美女); break;
			}
			
			//给美女头像和姓名重置
			美女.face = face_0;
			美女.sei = sei_0;
			美女.mei = mei_0;
			美女.update();
		}
				
		void trace()
		{
			for (int i = 0; i < 670; i++)
			{
				auto person_0 = pk::get_person(i);
				auto age_0 = int(person_0.death) - int(person_0.birth);
				pk::trace(pk::format("id为：{}，年龄为：{}", i, age_0));
			}
		}

	} // class Main
	Main main;
}

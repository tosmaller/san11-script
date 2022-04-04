// ## 2021/09/18 # 江东新风 # 更改background路径 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2021/04/18 # 白马叔叔 # 新增战死无是否影响处死 ##
// ## 2021/04/10 # 江东新风 # 修正刺客被俘军团 ##
// ## 2021/03/06 # 江东新风 # 刺杀隐藏属性失效的bug修复，xml复制黏贴导致不识别错误 ##
// ## 2021/03/02 # 江东新风 # 刺杀分身的bug修复 ##
// ## 2021/02/27 # 江东新风 # 刺杀可能的bug修复 ##
// ## 2021/02/25 # 江东新风 # 修复可能存在的bug ##
// ## 2021/01/31 # 江东新风 # rand函数错误修复 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/11/21 # 江东新风 # 修复姓名未重置的bug ##
// ## 2020/11/16 # 江东新风 # 修复一些描述错误 ##
// ## 2020/11/15 # 江东新风 # 修复刺杀者武器可能被抢的bug，优化代码 ##
// ## 2020/11/14 # 江东新风 # 基本完善 ##
// ## 2020/11/06 @ 江东新风 @ 初次编写 @@
/*
@作者：江东新风
@說明：派出我方武将刺杀敌方武将

*/
namespace 计略指令_刺杀
{


	// ================ CUSTOMIZE ================


	const int ACTION_COST = 20;   	 // 行动力消耗
	const int GOLD_COST = 500;       // 金消耗
	const int 刺杀_可执行距离 = 3;

	// 根据执行武将的智力決定成功率 (智力高於敌军太守 武将智力小於70时成功率? 60%, 大於70时成功率?70%, 大於80时成功率?80%, 大於90时成功率?90%，智力低於敌军太守，成功率?10%)
	const int HIGH_Chance_0 = 60;		   	  // 能力差 - 太守buf 大于10的成功率的计算
	const int HIGH_Chance_1 = 70;		   	  // 能力差 - 太守buf 大于10的成功率的计算
	const int HIGH_Chance_2 = 80;		      // 能力差 - 太守buf 大于20的成功率的计算
	const int HIGH_Chance_3 = 90;		      // 能力差 - 太守buf 大于30的成功率的计算
	const int LOW_Chance = 25;			      // 能力差 - 太守buf 小于0的成功率的计算

	const int 潜入失败_被俘率 = 10;

	const int 刺杀失败_被俘率 = 50;
	const int 刺杀失败_处死率 = 49;


	const bool 刺杀成功_是否处决_选项 = true;	//刺杀成功后是否由玩家决定处死目标			
	const int 刺杀成功_被俘率 = 10;
	const int 刺杀成功_处死率 = 2;
	const int 刺杀成功_君主逃脱率 = 30;

	const bool 好友选最高武力参战 = true;

	const bool 护卫带宝物 = true;

	const int 潜入_被发现率 = 30;
	const array<int> 潜入_被发现率_加成 = {/*胆小*/5, /*冷静*/-5,/*刚胆*/0,/*莽撞*/15 };

	const bool 战死无不处死 = true;    //剧本战死无影响处死。true为战死无时无法处死，false为处死行为与战死设置无关

	const int KEY = pk::hash("刺杀");//팎뻐


// ===========================================


	class Main
	{

		pk::building@ building_;
		pk::building@ target_base;
		pk::force@ force_;
		pk::person@ taishu_;
		pk::person@ actor;
		pk::person@ target_person;
		pk::city@ city_;
		int building_gold;
		bool flag_sneak_success = false;
		bool flag_absent = false;
		bool flag_sneak_failed = false;
		bool flag_ass_success = false;
		bool flag_ass_abandon = false;
		int flag_actor_result = -1;//刺杀者结果，-1：默认值。0：无伤返回，1：负伤返回，2：被俘虏，3：被处决
		int flag_target_result = -1;//被目标结果，-1：默认值。0：无伤逃脱，1：受伤，2：被处死
		int event_result = 0;			// 决斗结果初始化
		//bool flag_ass_success = false; // 刺杀结果初始化
		int debate_result = 0;			// 舌战结果初始化
		int 潜入场景 = 0;
		int 刺杀场景 = 0;
		array<int> 场景id = { 场景_市场, 场景_家中, 场景_庭院, 场景_野外 };
		array<string> 潜入场景id = {/*城门1*/"media/background/assasinate/gate1.bmp", /*城门2*/"media/background/assasinate/gate1.bmp", /*街上1*/"media/background/assasinate/street1.bmp", /*街上2*/"media/background/assasinate/street2.bmp", /*酒馆*/"media/background/assasinate/tavern.bmp" };


		pk::person@ 禁卫;
		pk::person@ 统领;
		pk::person@ 护院;
		pk::person@ 家丁;
		/*
				pk::item@ 三尖刀;     //长柄1
				pk::item@ 大斧;		//长柄2
				pk::item@ 羌骑兵兵符;	//剑3
				pk::item@ 乌丸兵兵符;	//剑4
				pk::item@ 飞刀;			//暗器5
				pk::item@ 手戟;			//暗器6
				pk::item@ 东胡飞弓;		//弓7
				pk::item@ 东胡飞弓2;		//弓8
		*/
		array<array<int>> 宝物信息_暂存 = { {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1} };
		array<int> 宝物id = { 宝物_三尖刀, 宝物_大斧, 宝物_羌骑兵兵符, 宝物_乌丸兵兵符, 宝物_飞刀, 宝物_手戟, 宝物_东胡飞弓, 宝物_东胡飞弓2 };

		Main()
		{
			pk::menu_item item;
			item.menu = 104;
			item.pos = 3;
			item.shortcut = "";
			item.init = pk::building_menu_item_init_t(init);
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
			@taishu_ = pk::get_person(pk::get_taishu_id(building));
			@city_ = pk::building_to_city(building);
			building_gold = pk::get_gold(building);

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
			return pk::encode("刺杀");//刺杀
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
				return pk::encode(pk::format("执行刺杀. (行动力 {})", ACTION_COST));
		}

		bool handler()
		{
			flag_sneak_success = false;
			flag_absent = false;
			flag_sneak_failed = false;
			flag_ass_success = false;
			flag_ass_abandon = false;
			flag_actor_result = -1;//刺杀者结果，-1：默认值。0：无伤返回，1：负伤返回，2：被俘虏，3：被处决
			flag_target_result = -1;//被目标结果，-1：默认值。0：无伤逃脱，1：受伤，2：被处死
			event_result = 0;			// 决斗结果初始化		
			debate_result = 0;			// 舌战结果初始化			

			if (pk::choose({ pk::encode(" 是 "), pk::encode(" 否 ") }, pk::encode(pk::format("确定执行刺杀？\n*\x1b[29x最后即使刺杀成功，我方派出的武将也极有可能被俘虏甚至死亡\x1b[0x*", GOLD_COST)), taishu_) == 1)
				return false;

			// 执行武将
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building_);
			//排除君主
			for (int i = 0; i < person_list.count; i++)
			{
				if (person_list[i].mibun == 身份_君主)
				{
					person_list.remove_at(i);
					break;
				}
			}
			person_list.sort(function(a, b)
			{
				return (a.stat[武将能力_武力] > b.stat[武将能力_武力]); // 무장 정렬 (지력순)
			});
			if (person_list.count == 0)
			{
				pk::message_box(pk::encode("没有可执行的武将了。"), taishu_);
				return false;
			}
			pk::list<pk::building@> building_list = getTargetList(building_);

			if (building_list.count == 0)
			{
				pk::message_box(pk::encode("附近没有敌方据点。"), taishu_);
				return false;
			}


			//执行刺杀武将选擇
			pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("执行刺杀武将选择"), pk::encode("选择执行刺杀的武将."), person_list, 1, 1);
			if (person_sel.count == 0) return false; // 未选擇时取消结束
			@actor = pk::get_person(person_sel[0].get_id());
			// 选擇目标据点

			pk::list<pk::building@> list_target = pk::building_selector(pk::encode("选择目标据点"), pk::encode("选择目标据点."), building_list, 1, 1);
			if (list_target.count == 0) return false; // 未选擇时取消结束
			@target_base = pk::get_building(list_target[0].get_id());

			//刺杀对象武将选擇
			pk::list<pk::person@> person_list2 = pk::get_idle_person_list(target_base);
			pk::list<pk::person@> person_sel2 = pk::person_selector(pk::encode("刺杀对象武将选择"), pk::encode("选择要刺杀的武将."), person_list2, 1, 1);
			if (person_sel.count == 0) return false; // 未选擇时取消结束
			@target_person = pk::get_person(person_sel2[0].get_id());


			//武将任务的设定
			//actor.order = -1;
			//actor.target[6] = 1;
			//actor.absence_timer = 0;

			if (pk::is_absent(target_person))
				flag_absent = true;
			else
			{
				//pk::trace("目标在城中");

				// 根据(执行武将的智力+武力) - (对象武将的智力 + 政治 或 对象武将的统率 + 武力)決定成功率,对象据点太守智力或政治大于90时成功率-10, 都大于 -15。
				pk::person@ taishou = pk::get_person(pk::get_taishu_id(target_base));
				int taishou_buf = 0;
				flag_sneak_success = false;

				if (taishou.stat[武将能力_智力] >= 90 and taishou.stat[武将能力_政治] >= 90)
					taishou_buf = 15;
				else if (taishou.stat[武将能力_智力] >= 90 or taishou.stat[武将能力_政治] >= 90)
					taishou_buf = 10;

				//if ( taishou.has_skill(특기_침착) or taishou.has_skill(특기_명경) or taishou.has_skill(특기_통찰) or taishou.has_skill(특기_신산) ) continue;

				//pk::core["지형"][武将_吕布]["附加单挑防秒率"] = 200;
				//pk::trace(pk::format("吕布防秒率：{}", pk::core["지형"][武将_吕布]["附加单挑防秒率"]));
				//pk::core["지형"][武将_吕布]["附加单挑防秒率"] = 100;
				//pk::trace(pk::format("吕布防秒率：{}", pk::core["지형"][武将_吕布]["附加单挑防秒率"]));				
				//计算能力差
				int DIF = (actor.stat[武将能力_智力] + actor.stat[武将能力_武力]) - pk::max((target_person.stat[武将能力_智力] + target_person.stat[武将能力_政治]), (target_person.stat[武将能力_武力] + target_person.stat[武将能力_统率]));
				// 能力差 - 太守buf 大于零的成功率的计算
				if ((DIF - taishou_buf) > 0)
				{

					if ((DIF - taishou_buf) >= 30 and pk::rand_bool(HIGH_Chance_3))
					{
						flag_sneak_success = true;
					}

					if (((DIF - taishou_buf) >= 20 and (DIF - taishou_buf) < 30) and pk::rand_bool(HIGH_Chance_2))
					{
						flag_sneak_success = true;
					}

					if (((DIF - taishou_buf) >= 10 and (DIF - taishou_buf) < 20) and pk::rand_bool(HIGH_Chance_1))
					{
						flag_sneak_success = true;
					}
					if (((DIF - taishou_buf) < 10) and pk::rand_bool(HIGH_Chance_0))
					{
						flag_sneak_success = true;
					}
				}

				// 能力差 - 太守buf 小于零的成功率计算
				if ((DIF - taishou_buf) <= 0)
				{
					if (pk::rand_bool(LOW_Chance))
					{
						flag_sneak_success = true;
					}
				}
				//pk::trace(pk::format("目标在城中,潜入成功：{}，能力差：{}", flag_sneak_success, DIF - taishou_buf));
			}

			//如果目标不是确实不在，且潜入失败，则随机进行扑空或者被发现的事件，默认被发现率30，根据刺杀者性格调整被发现率
			if (!flag_absent and !flag_sneak_success)
			{
				if (pk::rand_bool(潜入_被发现率 + 潜入_被发现率_加成[actor.character]))
					flag_sneak_failed = true;
				else flag_absent = true;
			}


			if (flag_absent)
			{
				pk::scene(pk::scene_t(scene_sneak_failed));
			}
			else if (flag_sneak_failed)
			{
				pk::scene(pk::scene_t(scene_sneak_failed));
			}
			else if (flag_sneak_success)
			{

				pk::say(pk::encode("虽然防守很是严密，但还难不倒我."), actor, building_);
				event_result = -1;
				刺杀场景 = pk::rand(4);//随机生成刺杀场景
				pk::scene(pk::scene_t(scene));

			}

			//30几率设定刺客为厌恶武将	
			Add_dislike();
			//外交关系变化
			if (flag_actor_result == 2 or flag_actor_result == 3)
			{
				if (flag_target_result == 2)
					pk::add_relations(force_, target_person.get_force_id(), -100);
				else
					pk::add_relations(force_, target_person.get_force_id(), -50);
				pk::force@ dst_force = pk::get_force(target_person.get_force_id());
				pk::history_log(force_.get_id(), force_.color, pk::encode(pk::format("因刺杀\x1b[1x{}\x1b[0x，\x1b[2x{}\x1b[0x军与\x1b[2x{}\x1b[0x军的关系恶化了", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(force_)), pk::decode(pk::get_name(dst_force)))));

			}

			//隐藏武力清空
			person_substrengh_setting(false);
			//姓名重置
			person_setting(false);
			// 计算行动力消耗
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			// 刺杀者设为已行动
			actor.action_done = true;

			// 计算金消耗
			pk::add_gold(building_, -GOLD_COST, true);

			return true;

		}

		void scene_sneak_failed()
		{


			pk::move_screen(target_base.get_pos());

			潜入场景 = pk::rand(5);
			pk::fade(0);
			pk::sleep();
			pk::background(潜入场景id[潜入场景]);
			pk::fade(255);

			if (flag_absent or flag_sneak_failed)
			{
				switch (潜入场景)
				{
				case 0: //城门1
					pk::message_box(pk::encode("老哥辛苦了，喝口水吧~"), actor);
					pk::message_box(pk::encode("感谢，干了一上午了，真是口渴~"), pk::get_person(武将_泥棒));
					pk::message_box(pk::encode(pk::format("向你打听个人，知道\x1b[1x{}\x1b[0x大人住哪里吗？我想前去拜访~", pk::decode(pk::get_name(target_person)))), actor);
					if (target_person.location > 86)
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...真不巧，据说他随军出征了，估计得过段时间才能回来~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_泥棒));
					else if (target_person.location != target_person.service)
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...真不巧，据说他外出公干了，也不知道啥时候能回来呢~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_泥棒));
					else
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...有些日子没见到他了呢，也不知道去哪里了~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_泥棒));
					if (!flag_sneak_failed) pk::message_box(pk::encode("看来是要白跑一趟了...\n留在这里也无济于事，只能先回去了..."), actor);
					else
					{
						pk::message_box(pk::encode("......."));
						pk::message_box(pk::encode("官爷，城门口里有个人，形迹可疑，到处在打探消息。"), pk::get_person(武将_职人));
						pk::message_box(pk::encode("好的，我们这就去看看。"), pk::get_person(武将_卫士));
						pk::message_box(pk::encode("......."));
						pk::message_box(pk::encode("麻烦你跟我们走一趟。"), pk::get_person(武将_卫士));
						pk::message_box(pk::encode("可恶，居然暴露了，只能杀出去了..."), actor);
						scene_sneak_fighte();
					}
					break;
				case 1: //城门2

					pk::message_box(pk::encode("老哥辛苦了，喝口水吧~"), actor);
					pk::message_box(pk::encode("感谢，干了一上午了，真是口渴~"), pk::get_person(武将_泥棒));
					pk::message_box(pk::encode(pk::format("向你打听个人，知道\x1b[1x{}\x1b[0x大人住哪里吗？我想前去拜访~", pk::decode(pk::get_name(target_person)))), actor);
					if (target_person.location > 86)
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...真不巧，据说他随军出征了，估计得过段时间才能回来~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_泥棒));
					else if (target_person.location != target_person.service)
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...真不巧，据说他外出公干了，也不知道啥时候能回来呢~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_泥棒));
					else
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...有些日子没见到他了呢，也不知道去哪里了~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_泥棒));
					if (!flag_sneak_failed) pk::message_box(pk::encode("看来是要白跑一趟了...\n留在这里也无济于事，只能先回去了..."), actor);
					else
					{
						pk::message_box(pk::encode("......."));
						pk::message_box(pk::encode("官爷，城门口里有个人，形迹可疑，到处在打探消息。"), pk::get_person(武将_职人));
						pk::message_box(pk::encode("好的，我们这就去看看。"), pk::get_person(武将_卫士));
						pk::message_box(pk::encode("......."));
						pk::message_box(pk::encode("麻烦你跟我们走一趟。"), pk::get_person(武将_卫士));
						pk::message_box(pk::encode("可恶，居然暴露了，只能杀出去了..."), actor);
						scene_sneak_fighte();
					}
					break;
				case 2: //街上1

					pk::message_box(pk::encode("苏州来的上好的丝绸。数量有限，客官要不要看一看~"), pk::get_person(武将_商人));
					pk::message_box(pk::encode("看起来还真不错，给我来一匹吧~"), actor);
					pk::message_box(pk::encode("好，这就给你打包装好。"), pk::get_person(武将_商人));
					pk::message_box(pk::encode(pk::format("且慢，跟你打听个人，知道\x1b[1x{}\x1b[0x大人住哪里吗？我想前去拜访~", pk::decode(pk::get_name(target_person)))), actor);
					if (target_person.location > 86)
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...真不巧，据说他随军出征了，估计得过段时间才能回来~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_商人));
					else if (target_person.location != target_person.service)
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...真不巧，据说他外出公干了，也不知道啥时候能回来呢~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_商人));
					else
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...有些日子没见到他了呢，也不知道去哪里了~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_商人));
					if (!flag_sneak_failed) pk::message_box(pk::encode("看来是要白跑一趟了...\n留在这里也无济于事，只能先回去了..."), actor);
					else
					{
						pk::message_box(pk::encode("......."));
						pk::message_box(pk::encode("官爷，街上里有个人，形迹可疑，到处在打探消息。"), pk::get_person(武将_职人));
						pk::message_box(pk::encode("好的，我们这就去看看。"), pk::get_person(武将_卫士));
						pk::message_box(pk::encode("......."));
						pk::message_box(pk::encode("麻烦你跟我们走一趟。"), pk::get_person(武将_卫士));
						pk::message_box(pk::encode("可恶，居然暴露了，只能杀出去了..."), actor);
						scene_sneak_fighte();
					}
					break;
				case 3: //街上2

					pk::message_box(pk::encode("上好的大萝卜。今早刚从地里摘来的~"), pk::get_person(武将_泥棒));
					pk::message_box(pk::encode("给我来一斤吧~"), actor);
					pk::message_box(pk::encode("好，这就给你包上。"), pk::get_person(武将_泥棒));
					pk::message_box(pk::encode(pk::format("且慢，跟你打听个人，知道\x1b[1x{}\x1b[0x大人住哪里吗？我想前去拜访~", pk::decode(pk::get_name(target_person)))), actor);
					if (target_person.location > 86)
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...真不巧，据说他随军出征了，估计得过段时间才能回来~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_泥棒));
					else if (target_person.location != target_person.service)
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...真不巧，据说他外出公干了，也不知道啥时候能回来呢~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_泥棒));
					else
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...有些日子没见到他了呢，也不知道去哪里了~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_泥棒));
					if (!flag_sneak_failed) pk::message_box(pk::encode("看来是要白跑一趟了...\n留在这里也无济于事，只能先回去了..."), actor);
					else
					{
						pk::message_box(pk::encode("......."));
						pk::message_box(pk::encode("官爷，街上里有个人，形迹可疑，到处在打探消息。"), pk::get_person(武将_职人));
						pk::message_box(pk::encode("好的，我们这就去看看。"), pk::get_person(武将_卫士));
						pk::message_box(pk::encode("......."));
						pk::message_box(pk::encode("麻烦你跟我们走一趟。"), pk::get_person(武将_卫士));
						pk::message_box(pk::encode("可恶，居然暴露了，只能杀出去了..."), actor);
						scene_sneak_fighte();
					}
					break;
				case 4: //酒馆

					pk::message_box(pk::encode("客官，吃点啥?~"), pk::get_person(武将_酒家娘));
					pk::message_box(pk::encode("掌柜，来一份烧鸡，一叠炒青菜，一壶酒~"), actor);
					pk::message_box(pk::encode("好，这就给你备上。"), pk::get_person(武将_酒家娘));
					pk::message_box(pk::encode(pk::format("且慢，跟你打听个人，知道\x1b[1x{}\x1b[0x大人住哪里吗？我想前去拜访~", pk::decode(pk::get_name(target_person)))), actor);
					if (target_person.location > 86)
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...真不巧，据说他随军出征了，估计得过段时间才能回来~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_酒家娘));
					else if (target_person.location != target_person.service)
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...真不巧，据说他外出公干了，也不知道啥时候能回来呢~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_酒家娘));
					else
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人呀...有些日子没见到他了呢，也不知道去哪里了~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_酒家娘));
					if (!flag_sneak_failed) pk::message_box(pk::encode("看来是要白跑一趟了...\n留在这里也无济于事，只能先回去了..."), actor);
					else
					{
						pk::message_box(pk::encode("......."));
						pk::message_box(pk::encode("官爷，酒馆里有个人，形迹可疑，到处在打探消息。"), pk::get_person(武将_旅人));
						pk::message_box(pk::encode("好的，我们这就去看看。"), pk::get_person(武将_卫士));
						pk::message_box(pk::encode("......."));
						pk::message_box(pk::encode("麻烦你跟我们走一趟。"), pk::get_person(武将_卫士));
						pk::message_box(pk::encode("可恶，居然暴露了，只能杀出去了..."), actor);
						scene_sneak_fighte();
					}
					break;
				}
			}

			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
			pk::move_screen(building_.get_pos());
			//专门再做一个刺杀后结果汇报的场景，根据结果不同汇报不同的台词
			scene_ass_report();

		}

		void scene()
		{


			pk::move_screen(target_base.get_pos());

			pk::fade(0);
			pk::sleep();
			pk::background(场景id[刺杀场景]);
			pk::fade(255);


			//刺杀前场景1--目标对象在闲逛之类的
			scene_before_ass();

			if (target_person.stat[武将能力_魅力] > 90 and actor.giri > 义理_普通 and pk::rand_bool(50))
				scene_ass_abandon();
			else
			{
				// 刺杀前场景对话 2
				scene_before_ass_2();

				// 刺杀，可以考虑多轮
				scene_ass_execute();
				//scene_ass_execute();
				// 刺杀后场景-胜利，失败 2, 3
				if (event_result == 0 and debate_result == 0)
				{
					scene_ass_success();
				}
				else
				{
					//pk::trace(pk::format("单挑结果:{}",event_result));
					scene_ass_falied();
				}
			}

			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
			pk::move_screen(building_.get_pos());
			//专门再做一个刺杀后结果汇报的场景，根据结果不同汇报不同的台词
			scene_ass_report();
		}

		void scene_before_ass()
		{
			//随机多种刺杀场景--要根据武将能力值设定对应场景还是全随机？			
			//int ass_place = 0;
			//ass_place = pk::rand(2);
			switch (刺杀场景)
			{
			case 0: //市场


				pk::message_box(pk::encode("卖糖葫芦啦，一文钱一串~"), pk::get_person(武将_商人));
				pk::message_box(pk::encode("刚到的蜜桃，水灵灵的，只要....~"), pk::get_person(武将_商人));
				pk::message_box(pk::encode("今天的市集也格外热闹呀~"), target_person);
				break;
			case 1: //自家室内

				pk::message_box(pk::encode("不知不觉就工作到现在了呀~"), target_person);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，偶尔也要注意休息呀~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_童子));
				pk::message_box(pk::encode("没办法，每天都有干不完的工作呀..."), target_person);
				break;
			case 2: //庭院

				pk::message_box(pk::encode("鱼国群鳞乐有余，观鱼才觉我非鱼。\n虞诈两忘欣共处，鱼犹如此况人乎。"), target_person);
				pk::message_box(pk::encode("大人此句真是发人深省~"), pk::get_person(武将_童女));
				pk::message_box(pk::encode("有感而发罢了~"), target_person);
				break;
			case 3: //野外

				pk::message_box(pk::encode("看我把这只鹿射下来~"), target_person);
				pk::message_box(pk::encode("哇，大人真是箭法出众！~"), pk::get_person(武将_兵士));
				pk::message_box(pk::encode("哈哈哈~小试牛刀罢了~"), target_person);
				break;
			}
		}

		void scene_before_ass_2()
		{
			int num = 0;

			// 场景-脚步声
			pk::cutin(18);

			// 场景-脚步声
			pk::cutin(20);



			array<string> actor_lines_arr =
			{
				pk::encode(pk::format("{}，今天就是你的死期。", pk::decode(pk::get_name(target_person)))),
				pk::encode(pk::format("{}，明年的今天将是你的忌日。", pk::decode(pk::get_name(target_person))))
			};

			num = pk::rand(2);
			pk::message_box(pk::encode(pk::decode(actor_lines_arr[num])), actor);

			array<string> target_person_lines_arr =
			{
				pk::encode("护卫们，给我上."),
				pk::encode("你... 是谁?"),
				pk::encode("你以为这样就能杀得了我嘛!"),
				pk::encode(pk::format("我\x1b[1x{}\x1b[0x的性命可不会简单的交到你的手里!", pk::decode(pk::get_name(target_person))))
			};

			num = pk::rand(2);
			int target_person_character = target_person.character;
			if (target_person_character >= 2) num = num + 2;
			pk::message_box(pk::encode(pk::decode(target_person_lines_arr[num])), target_person);
		}

		void scene_ass_abandon()
		{
			// 场景-脚步声
			pk::cutin(18);

			// 场景-脚步声
			pk::cutin(20);

			int num = 0;
			array<string> actor_lines_arr =
			{
				pk::encode(pk::format("{}，今天就是你的死期。", pk::decode(pk::get_name(target_person)))),
				pk::encode(pk::format("{}，明年的今天将是你的忌日。", pk::decode(pk::get_name(target_person))))
			};

			num = pk::rand(2);
			pk::message_box(pk::encode(pk::decode(actor_lines_arr[num])), actor);

			array<string> target_person_lines_arr =
			{
				pk::encode("你... 是谁?"),
				pk::encode(pk::format("阁下从何而来?!", pk::decode(pk::get_name(target_person))))
			};
			num = pk::rand(2);
			pk::message_box(pk::encode(pk::decode(target_person_lines_arr[num])), target_person);
			switch (刺杀场景)
			{
			case 0: //市场

				pk::message_box(pk::encode(pk::format("要想刺杀\x1b[1x{}\x1b[0x大人，就先杀了我~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_职人));
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我们帮你挡着！", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_职人));
				pk::message_box(pk::encode(pk::format("没想到居然有这么多百姓为他出头！看来\x1b[1x{}\x1b[0x真是位仁德之人，我不忍杀之。", pk::decode(pk::get_name(target_person)))), actor);
				pk::message_box(pk::encode("看你也是误入歧途受人蛊惑。这次暂且放你一马吧！"), target_person);
				break;
			case 1: //自家室内

				pk::message_box(pk::encode(pk::format("兄弟们，我们拼死也要挡住刺客。就算我们死去，\x1b[1x{}\x1b[0x大人也一定会照顾好我们的妻儿老母~", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_卫士));
				pk::message_box(pk::encode(pk::format("看来\x1b[1x{}\x1b[0x待普通士兵就如亲人一般，更何况天下呢？于心不忍...暂且撤退吧。", pk::decode(pk::get_name(target_person)))), actor);
				pk::message_box(pk::encode("呼！多亏了各位将士。不要去追了免得误伤自己的性命。"), target_person);
				break;
			case 2: //庭院
				pk::message_box(pk::encode(pk::format("不要杀\x1b[1x{}\x1b[0x大人好吗。为什么这么残忍呢，\x1b[1x{}\x1b[0x大人经常陪我一起玩。呜呜呜！", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(target_person)))), pk::get_person(武将_童女));
				pk::message_box(pk::encode("老幼病残我不杀之，既然这么说了就等下次吧。"), actor);
				pk::message_box(pk::encode("哎！不要去追了保护无辜的人。"), target_person);
				break;
			case 3: //野外
				pk::message_box(pk::encode(pk::format("如今乱世百姓都吃不饱穿不暖。\x1b[1x{}\x1b[0x大人经常将肉分发给百姓，将皮毛赠与百姓作为御寒之物。你怎么能下得去手！", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_旅人));
				pk::message_box(pk::encode(pk::format("看来\x1b[1x{}\x1b[0x是位难得的仁慈之人，且留下性命造福百姓吧。", pk::decode(pk::get_name(target_person)))), actor);
				pk::message_box(pk::encode("看来我也没有追他的必要了，让他去行大义之事吧。"), target_person);
				break;
			}
			actor.loyalty -= 5;
			flag_ass_abandon = true;
		}
		void scene_ass_execute()
		{
			pk::unit@ dummy_unit;
			//pk::point emptyPos = getEmptyPos(building_);
			//pk::unit@ unit_temp = pk::create_unit(building_, actor, null, null, 100, 兵器_枪, 兵器_走舸, 0, 100, emptyPos);
			//@dummy_unit = pk::get_unit(0);
			pk::person@ dummy_person;
			pk::person@ helper1;
			pk::person@ helper2;
			person_setting(true);
			if (护卫带宝物) item_setting();


			//对象为文官的场景  duel发起方胜利返回值为0
			if (target_person.mibun == 身份_一般 and target_person.stat[武将能力_武力] < 70)
			{
				if (pk::rand_bool(30))
				{
					actor.shoubyou = 1;
					actor.update();
					pk::message_box(pk::encode("可恶，居然放暗箭... \n 这点伤不算什么...！"), actor);
				}
				person_setting(true);
				person_substrengh_setting(true);
				event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 护院, 家丁, 家丁, true, false, 0, true).first;
				actor.update();
				if (event_result == 0)
				{
					person_setting(true);
					person_substrengh_setting(true);
					actor.hp -= 20 + pk::rand(10);
					pk::int_int Aidperson = getAidperson(getAidpersonList(target_person));
					if (Aidperson.first != -1 and check_strength(actor, pk::get_person(Aidperson.first), 15))
					{
						@ helper1 = pk::get_person(Aidperson.first);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我来帮你", pk::decode(pk::get_name(target_person)))), helper1);
					}
					else
						@ helper1 = 护院;
					if (Aidperson.second != -1 and check_strength(actor, pk::get_person(Aidperson.second), 15))
					{
						@ helper2 = pk::get_person(Aidperson.second);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我会保护你的", pk::decode(pk::get_name(target_person)))), helper2);
					}
					else
						@ helper2 = 家丁;

					if (check_strength(actor, target_person, 15))
					{
						pk::message_box(pk::encode("我们一起上！"), target_person);
						event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, helper1, helper2, target_person, true, false, 0, true).first;
					}
					else
					{
						pk::message_box(pk::encode("靠你们了！"), target_person);
						event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, helper1, helper2, 家丁, true, false, 0, true).first;
						if (event_result == 0)
						{
							pk::message_box(pk::encode("就剩下你一人了！"), actor);
							event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, target_person, dummy_person, dummy_person, true, false, 0, true).first;
						}
					}					//pk::trace(pk::format("单挑结果:{}",event_result));
				}
				if (event_result == 0 and pk::rand_bool(50) and target_person.stat[武将能力_智力] > 85)
				{
					actor.hp = 100;
					pk::message_box(pk::encode("现在就剩你啦！"), actor);
					pk::message_box(pk::encode("好汉且慢，听我一言！"), target_person);
					debate_result = pk::debate(actor, target_person, pk::is_player_controlled(actor), false, true, true).first;
					//event_result += debate_result;
				}

			}

			//对象为武官的场景--未完成  duel发起方胜利返回值为0
			if (target_person.mibun == 身份_一般 and target_person.stat[武将能力_武力] >= 70)
			{
				person_setting(true);
				person_substrengh_setting(true);
				event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 护院, 家丁, 家丁, true, false, 0, true).first;
				if (event_result == 0)
				{
					person_setting(true);
					person_substrengh_setting(true);
					actor.hp -= 20 + pk::rand(10);
					pk::int_int Aidperson = getAidperson(getAidpersonList(target_person));
					if (Aidperson.first != -1 and check_strength(actor, pk::get_person(Aidperson.first), 15))
					{
						@ helper1 = pk::get_person(Aidperson.first);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我来帮你", pk::decode(pk::get_name(target_person)))), helper1);
					}
					else
						@ helper1 = 护院;
					if (Aidperson.second != -1 and check_strength(actor, pk::get_person(Aidperson.second), 15))
					{
						@ helper2 = pk::get_person(Aidperson.second);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我会保护你的", pk::decode(pk::get_name(target_person)))), helper2);
					}
					else
						@ helper2 = 家丁;

					if (check_strength(actor, target_person, 15))
					{
						pk::message_box(pk::encode("我们一起上！"), target_person);
						event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, helper1, helper2, target_person, true, false, 0, true).first;
					}
					else
					{
						pk::message_box(pk::encode("靠你们了！"), target_person);
						event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, helper1, helper2, 家丁, true, false, 0, true).first;
						if (event_result == 0)
						{
							pk::message_box(pk::encode("就剩下你一人了！"), actor);
							event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, target_person, dummy_person, dummy_person, true, false, 0, true).first;
						}

					}					//pk::trace(pk::format("单挑结果:{}",event_result));
				}
			}
			//对象为太守的场景--未完成  duel发起方胜利返回值为0
			if (target_person.mibun == 身份_太守)
			{
				person_setting(true);
				person_substrengh_setting(true);
				event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 护院, 家丁, 家丁, true, false, 0, true).first;
				if (event_result == 0 and pk::rand_bool(50))
				{
					person_setting(true);
					person_substrengh_setting(true);
					pk::message_box(pk::encode("继续给我上！"), target_person);
					pk::message_box(pk::encode("可恶，居然还有！"), actor);
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 护院, 家丁, 家丁, true, false, 0, true).first;
				}
				if (event_result == 0)
				{
					person_setting(true);
					person_substrengh_setting(true);
					actor.hp -= 20 + pk::rand(10);
					pk::int_int Aidperson = getAidperson(getAidpersonList(target_person));
					if (Aidperson.first != -1 and check_strength(actor, pk::get_person(Aidperson.first), 15))
					{
						@ helper1 = pk::get_person(Aidperson.first);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我来帮你", pk::decode(pk::get_name(target_person)))), helper1);
					}
					else
						@ helper1 = 护院;
					if (Aidperson.second != -1 and check_strength(actor, pk::get_person(Aidperson.second), 15))
					{
						@ helper2 = pk::get_person(Aidperson.second);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我会保护你的", pk::decode(pk::get_name(target_person)))), helper2);
					}
					else
						@ helper2 = 家丁;

					if (check_strength(actor, target_person, 15))
					{
						pk::message_box(pk::encode("我们一起上！"), target_person);
						event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, helper1, helper2, target_person, true, false, 0, true).first;
					}
					else
					{
						pk::message_box(pk::encode("靠你们了！"), target_person);
						event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, helper1, helper2, 家丁, true, false, 0, true).first;
						if (event_result == 0)
						{
							pk::message_box(pk::encode("就剩下你一人了！"), actor);
							event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, target_person, dummy_person, dummy_person, true, false, 0, true).first;
						}
					}
					//pk::trace(pk::format("单挑结果:{}",event_result));
				}
			}
			//对象为都督的场景--未完成  duel发起方胜利返回值为0
			if (target_person.mibun == 身份_都督)
			{
				person_setting(true);
				person_substrengh_setting(true);
				event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 护院, 家丁, 家丁, true, false, 0, true).first;
				if (event_result == 0 and pk::rand_bool(80))
				{
					pk::message_box(pk::encode("继续给我上！"), target_person);
					pk::message_box(pk::encode("可恶，居然还有！"), actor);
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 护院, 家丁, 家丁, true, false, 0, true).first;
				}
				if (event_result == 0)
				{
					person_setting(true);
					person_substrengh_setting(true);
					actor.hp -= 20 + pk::rand(10);
					pk::int_int Aidperson = getAidperson(getAidpersonList(target_person));
					if (Aidperson.first != -1 and check_strength(actor, pk::get_person(Aidperson.first), 15))
					{
						@ helper1 = pk::get_person(Aidperson.first);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我来帮你", pk::decode(pk::get_name(target_person)))), helper1);
					}
					else
						@ helper1 = 护院;
					if (Aidperson.second != -1 and check_strength(actor, pk::get_person(Aidperson.second), 15))
					{
						@ helper2 = pk::get_person(Aidperson.second);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我会保护你的", pk::decode(pk::get_name(target_person)))), helper2);
					}
					else
						@ helper2 = 家丁;

					if (check_strength(actor, target_person, 15))
					{
						pk::message_box(pk::encode("我们一起上！"), target_person);
						event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, helper1, helper2, target_person, true, false, 0, true).first;
					}
					else
					{
						pk::message_box(pk::encode("靠你们了！"), target_person);
						event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, helper1, helper2, 家丁, true, false, 0, true).first;
						if (event_result == 0)
						{
							pk::message_box(pk::encode("就剩下你一人了！"), actor);
							event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, target_person, dummy_person, dummy_person, true, false, 0, true).first;

						}
					}
					//pk::trace(pk::format("单挑结果:{}",event_result));
				}
			}
			//对象为君主的场景--未完成  duel发起方胜利返回值为0
			if (target_person.mibun == 身份_君主)
			{
				person_setting(true);
				person_substrengh_setting(true);
				event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 统领, 禁卫, 禁卫, true, false, 0, true).first;
				actor.update();
				if (event_result == 0)
				{
					person_setting(true);
					person_substrengh_setting(true);
					actor.hp -= 5 + pk::rand(10);
					pk::message_box(pk::encode("继续给我上！"), target_person);
					pk::message_box(pk::encode("可恶，居然还有！"), actor);
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 统领, 禁卫, 禁卫, true, false, 0, true).first;
					actor.update();
				}
				if (event_result == 0)
				{
					person_setting(true);
					person_substrengh_setting(true);
					actor.hp -= 5 + pk::rand(10);
					pk::message_box(pk::encode("挡住他！"), target_person);
					pk::message_box(pk::encode("别想跑！"), actor);
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 统领, 禁卫, 统领, true, false, 0, true).first;
					actor.update();
				}
				//随机多加一场
				if (event_result == 0 and pk::rand_bool(50))
				{
					person_setting(true);
					person_substrengh_setting(true);
					actor.hp -= 5 + pk::rand(10);
					pk::message_box(pk::encode("他快不行了，给我上！"), target_person);
					pk::message_box(pk::encode("你跑不了的！"), actor);
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 统领, 禁卫, 统领, true, false, 0, true).first;
					actor.update();
				}
				if (event_result == 0)
				{
					actor.hp -= 5 + pk::rand(10);
					actor.shoubyou = 1;
					actor.update();
					person_setting(true);
					person_substrengh_setting(true);
					pk::message_box(pk::encode("可恶，居然放暗箭... \n 这点伤不算什么...！"), actor);
					pk::int_int Aidperson = getAidperson(getAidpersonList(target_person));
					//pk::trace(pk::format("参战武将:{}，{}",Aidperson.first, Aidperson.second));
					if (Aidperson.first != -1 and check_strength(actor, pk::get_person(Aidperson.first), 15))
					{
						@ helper1 = pk::get_person(Aidperson.first);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我来帮你", pk::decode(pk::get_name(target_person)))), helper1);
					}
					else
						@ helper1 = 统领;
					if (Aidperson.second != -1 and check_strength(actor, pk::get_person(Aidperson.second), 15))
					{
						@ helper2 = pk::get_person(Aidperson.second);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我会保护你的", pk::decode(pk::get_name(target_person)))), helper2);
					}
					else
						@ helper2 = 禁卫;

					if (check_strength(actor, target_person, 15))
					{
						pk::message_box(pk::encode("我们一起上！"), target_person);
						event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, helper1, helper2, target_person, true, false, 0, true).first;
					}
					else
					{
						pk::message_box(pk::encode("靠你们了！"), target_person);
						event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, helper1, helper2, 禁卫, true, false, 0, true).first;
						if (event_result == 0)
						{
							pk::message_box(pk::encode("就剩下你一人了！"), actor);
							event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, target_person, dummy_person, dummy_person, true, false, 0, true).first;
						}
					}
				}
				if (event_result == 0 and pk::rand_bool(80))
				{
					actor.hp = 100;
					pk::message_box(pk::encode("纳命来！"), actor);
					pk::message_box(pk::encode("好汉且慢，听我一言！"), target_person);
					debate_result = pk::debate(actor, target_person, pk::is_player_controlled(actor), false, true, true).first;
					//event_result += debate_result;
				}

			}
			//恢复游戏原来的宝物所属
			if (护卫带宝物) item_restore();

			//写在此处好像因为多线程的原因会提前加载？
			//person_substrengh_setting(false);
		}

		void scene_sneak_fighte()
		{
			pk::unit@ dummy_unit;
			pk::person@ dummy_person;
			//person_setting(false);
			//person_substrengh_setting(true);

			if (护卫带宝物) item_setting();


			//潜入失败与护卫干架
			if (target_person.mibun == 身份_一般 and target_person.stat[武将能力_武力] < 70)
			{
				person_setting(false);
				person_substrengh_setting(true);
				pk::message_box(pk::encode("把他抓起来！"), 护院);
				event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 护院, 护院, 护院, true, false, 0, true).first;
				actor.update();
				if (event_result == 0 and pk::rand_bool(50))
				{
					person_setting(false);
					person_substrengh_setting(true);
					actor.hp -= 20 + pk::rand(10);
					pk::message_box(pk::encode("不要让他跑了！"), 护院);
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 护院, 护院, 护院, true, false, 0, true).first;
				}
			}
			//恢复游戏原来的宝物所属
			if (护卫带宝物) item_restore();

			//恢复附加武力。可以考虑也恢复事件武将能力
			//person_substrengh_setting(false);	

			//根据单挑结果决定是否被俘
			if (event_result == 0)
			{
				pk::message_box(pk::encode("情势不妙，先撤离吧..."), actor);
				pk::message_box(pk::encode("可恶，居然被他逃脱了"), pk::get_person(武将_卫士));
				flag_actor_result = 0;
			}
			else
			{
				pk::message_box(pk::encode("可恶，被包围了..."), actor);
				pk::message_box(pk::encode("绝对不能让刺客逃跑。\n围上去！"), pk::get_person(武将_卫士));
				pk::message_box(pk::encode("我一定要杀出去..."), actor);
				pk::message_box(pk::encode("他已经力竭了，抓住他！"), pk::get_person(武将_卫士));
				pk::message_box(pk::encode("可恶..."), actor);
				actor.former_force = actor.get_force_id();
				pk::set_district(actor, (pk::get_person(pk::get_kunshu_id(actor))).get_district_id());
				actor.mibun = 身份_俘虏;
				actor.location = target_base.get_id();
				pk::set_service(actor, target_base.get_id());
				//actor.service = target_base.get_id();
				flag_actor_result = 2;
			}

		}

		void scene_ass_success()
		{
			flag_ass_success = true;
			if (pk::rand_bool(刺杀成功_君主逃脱率) and target_person.mibun == 身份_君主)
			{
				pk::message_box(pk::encode("可恶，居然趁乱被护卫掩护逃走了..."), actor);
				flag_target_result = 0;
			}
			else
			{
				pk::message_box(pk::encode("你的护卫不过如此。接受制裁吧。"), actor);
				int answer = pk::choose({ pk::encode("处死"), pk::encode("放过") },
					pk::encode(pk::format("请选择是否处死\x1b[1x{}\x1b[0x?", pk::decode(pk::get_name(target_person)))), actor);

				if (战死无不处死 and pk::get_scenario().battle_death == 0 and answer == 0)
				{
					pk::message_box(pk::encode("唔唔...这不好吧...\n我实在下不了手..."), actor);
					answer = 1;
				}

				if (answer == 0)
				{
					pk::message_box(pk::encode("可恶，没想到竟要命丧于此！"), target_person);
					pk::play_se(3);
					pk::history_log(force_.get_id(), force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x被\x1b[2x{}\x1b[0x刺杀了", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))));
					pk::kill(target_person);
					flag_target_result = 2;
				}
				else if (answer == 1)
				{
					pk::message_box(pk::encode("这次就放你一马，希望你能记住这次教训！"), actor);
					int 健康状态 = 2 + pk::rand(1);
					target_person.shoubyou = 健康状态;
					target_person.update();
					flag_target_result = 1;
					pk::history_log(force_.get_id(), force_.color, pk::encode(pk::format("受到我军刺杀，\x1b[1x{}\x1b[0x的健康状态变为\x1b[29x{}\x1b[0x。", pk::decode(pk::get_name(target_person)), get_health_info(健康状态))));
				}
			}

			int actor_result = -1;
			actor_result = pk::rand(100) + 1;
			///根据结果选择对话及处理
			//逃脱
			if (actor_result <= (100 - 刺杀成功_处死率 - 刺杀成功_被俘率))
			{
				pk::message_box(pk::encode("可恶，没想到竟有如此多的护卫埋伏在此..."), actor);
				pk::message_box(pk::encode("情势不妙，先杀出重围吧..."), actor);
				pk::message_box(pk::encode("一定要抓住他"), pk::get_person(武将_兵士));
				int 健康状态 = 0 + pk::rand(3);
				actor.shoubyou = 健康状态;
				actor.update();
				if (健康状态 == 0) flag_actor_result = 0;
				else  flag_actor_result = 1;

			}
			//被俘虏
			else if (actor_result <= (100 - 刺杀成功_被俘率) and actor_result > (100 - 刺杀成功_处死率 - 刺杀成功_被俘率))
			{
				pk::message_box(pk::encode("可恶，侍卫源源不绝..."), actor);
				pk::message_box(pk::encode("绝对不能让刺客逃跑。\n围上去！"), pk::get_person(武将_兵士));
				pk::message_box(pk::encode("我一定要杀出去..."), actor);
				pk::message_box(pk::encode("他已经力竭了，抓住他！"), pk::get_person(武将_兵士));
				pk::message_box(pk::encode("可恶..."), actor);
				//pk::move(actor, target_base, false);
				actor.former_force = actor.get_force_id();
				pk::set_district(actor, (pk::get_person(pk::get_kunshu_id(actor))).get_district_id());
				actor.mibun = 身份_俘虏;
				actor.location = target_base.get_id();
				pk::set_service(actor, target_base.get_id());
				//actor.service = target_base.get_id();

				flag_actor_result = 2;
				//pk::message_box(pk::encode("报告主公，虽然刺杀成功，但事后\x1b[1x{}\x1b[0x大人也被敌军俘虏了。"), pk::get_person(武将_伝令));
			}
			//被处死
			else if (actor_result <= 100 and actor_result > (100 - 刺杀成功_被俘率))
			{
				pk::message_box(pk::encode("可恶，这点伤不算什么..."), actor);
				pk::message_box(pk::encode("你插翅难飞了"), pk::get_person(武将_兵士));
				pk::message_box(pk::encode("...我一定能杀出重围..."), actor);
				pk::message_box(pk::encode("砍死他！"), pk::get_person(武将_兵士));
				pk::message_box(pk::encode("可恶..."), actor);
				pk::play_se(3);
				//pk::history_log(force_.get_id(), force_.color, pk::encode(pk::format("报告主公，虽然刺杀成功，但\x1b[1x{}\x1b[0x未能成功脱离，被敌军处死了" , pk::decode(pk::get_name(actor)) ) ) );	
				pk::kill(actor);
				flag_actor_result = 3;
			}
		}

		void scene_ass_falied()
		{
			flag_ass_success = false;
			int actor_result = -1;
			actor_result = pk::rand(100) + 1;
			if (debate_result == 1)
				actor_result -= 90;//打过护卫但被说服的情况，降低被处死和被俘的几率
			//逃脱
			if (actor_result <= (100 - 刺杀失败_处死率 - 刺杀失败_被俘率))
			{
				//pk::message_box(pk::encode("可恶，没想到竟有如此多的护卫埋伏在此..."), actor);
				//pk::message_box(pk::encode("你的行踪早就被我掌握了。乖乖束手就擒吧！"), target_person);
				//pk::message_box(pk::encode("情势不妙，先杀出重围吧..."), actor);
				pk::message_box(pk::encode(pk::format("{}，你的脑袋就先记在你的脖子上，下次再来取走。", pk::decode(pk::get_name(target_person)))), actor);
				int 健康状态 = 0 + pk::rand(4);
				actor.shoubyou = 健康状态;
				actor.update();
				if (健康状态 == 0) flag_actor_result = 0;
				else  flag_actor_result = 1;

			}
			//被俘虏
			else if (actor_result <= (100 - 刺杀失败_处死率) and actor_result > (100 - 刺杀失败_处死率 - 刺杀失败_被俘率))
			{
				pk::message_box(pk::encode("可恶，侍卫源源不绝..."), actor);
				pk::message_box(pk::encode("没想到吧，我早就知道你潜进来了。乖乖束手就擒吧！"), target_person);
				pk::message_box(pk::encode("只能先杀出去了..."), actor);
				pk::message_box(pk::encode("他已经力竭了，抓住他！"), target_person);
				pk::message_box(pk::encode("可恶..."), actor);
				//pk::move(actor, target_base, false);
				actor.mibun = 身份_俘虏;
				actor.location = target_base.get_id();
				pk::set_service(actor, target_base.get_id());
				//actor.service = target_base.get_id();
				flag_actor_result = 2;
				//pk::message_box(pk::encode(pk::format("报告主公，任务失败，\x1b[1x{}\x1b[0x大人被敌军俘虏了。", pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
			}
			//被处死
			else if (actor_result <= 100 and actor_result > (100 - 刺杀失败_处死率))
			{
				pk::message_box(pk::encode("可恶，这点伤不算什么..."), actor);
				pk::message_box(pk::encode(pk::format("我\x1b[1x{}\x1b[0x的性命可不是那么好取的", pk::decode(pk::get_name(target_person)))), target_person);
				pk::message_box(pk::encode("...我一定能杀出重围..."), actor);
				pk::message_box(pk::encode("砍死他！"), target_person);
				pk::message_box(pk::encode("可恶..."), actor);
				pk::play_se(3);
				pk::history_log(force_.get_id(), force_.color, pk::encode(pk::format("任务失败，\x1b[1x{}\x1b[0x被\x1b[2x{}\x1b[0x处死", pk::decode(pk::get_name(actor)), pk::decode(pk::get_name(target_person)))));
				pk::kill(actor);
				flag_actor_result = 3;
			}

		}

		void scene_ass_report()
		{
			if (flag_absent)
				pk::message_box(pk::encode(pk::format("报告主公，任务失败，目标\x1b[1x{}\x1b[0x未找到。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));
			if (flag_sneak_failed and flag_actor_result == 0)
				pk::message_box(pk::encode(pk::format("报告主公，潜入失败，\x1b[1x{}\x1b[0x大人被护卫发现，所幸安全逃回。", pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
			if (flag_sneak_failed and flag_actor_result == 2)
				pk::message_box(pk::encode(pk::format("报告主公，潜入失败，\x1b[1x{}\x1b[0x大人被护卫发现，不幸被俘。", pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
			if (flag_ass_abandon)
				pk::message_box(pk::encode(pk::format("报告主公，\x1b[1x{}\x1b[0x重情重义，我实在不忍杀之...", pk::decode(pk::get_name(target_person)))), actor);
			if (flag_ass_success and flag_target_result == 0)
			{
				if (flag_actor_result == 0)
					pk::message_box(pk::encode(pk::format("报告主公，任务失败，\x1b[1x{}\x1b[0x在护卫掩护下逃离，\x1b[1x{}\x1b[0x大人安全返回。", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 1)
					pk::message_box(pk::encode(pk::format("报告主公，任务失败，\x1b[1x{}\x1b[0x在护卫掩护下逃离，\x1b[1x{}\x1b[0x大人负伤而回。", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 2)
					pk::message_box(pk::encode(pk::format("报告主公，任务失败，\x1b[1x{}\x1b[0x在护卫掩护下逃离，\x1b[1x{}\x1b[0x大人不幸被俘。", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 3)
					pk::message_box(pk::encode(pk::format("报告主公，任务失败，\x1b[1x{}\x1b[0x在护卫掩护下逃离，\x1b[1x{}\x1b[0x大人不幸被杀。", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));

			}
			if (flag_ass_success and flag_target_result == 1)
			{
				if (flag_actor_result == 0)
					pk::message_box(pk::encode(pk::format("报告主公，任务成功，\x1b[1x{}\x1b[0x被我方刺伤，\x1b[1x{}\x1b[0x大人安全返回。", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 1)
					pk::message_box(pk::encode(pk::format("报告主公，任务成功，\x1b[1x{}\x1b[0x被我方刺伤，\x1b[1x{}\x1b[0x大人负伤而回。", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 2)
					pk::message_box(pk::encode(pk::format("报告主公，任务成功，\x1b[1x{}\x1b[0x被我方刺伤，\x1b[1x{}\x1b[0x大人不幸被俘。", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 3)
					pk::message_box(pk::encode(pk::format("报告主公，任务成功，\x1b[1x{}\x1b[0x被我方刺伤，\x1b[1x{}\x1b[0x大人不幸被杀。", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));

			}
			if (flag_ass_success and flag_target_result == 2)
			{
				if (flag_actor_result == 0)
					pk::message_box(pk::encode(pk::format("报告主公，任务成功，\x1b[1x{}\x1b[0x被我方刺死，\x1b[1x{}\x1b[0x大人安全返回。", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 1)
					pk::message_box(pk::encode(pk::format("报告主公，任务成功，\x1b[1x{}\x1b[0x被我方刺死，\x1b[1x{}\x1b[0x大人负伤而回。", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 2)
					pk::message_box(pk::encode(pk::format("报告主公，任务成功，\x1b[1x{}\x1b[0x被我方刺死，\x1b[1x{}\x1b[0x大人不幸被俘。", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 3)
					pk::message_box(pk::encode(pk::format("报告主公，任务成功，\x1b[1x{}\x1b[0x被我方刺死，\x1b[1x{}\x1b[0x大人不幸被杀。", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));

			}
			if (!flag_ass_success and !flag_absent and !flag_sneak_failed and !flag_ass_abandon)
			{
				if (flag_actor_result == 0)
					pk::message_box(pk::encode(pk::format("报告主公，刺杀失败，\x1b[1x{}\x1b[0x大人安全返回。", pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 1)
					pk::message_box(pk::encode(pk::format("报告主公，刺杀失败，\x1b[1x{}\x1b[0x大人负伤而回。", pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 2)
					pk::message_box(pk::encode(pk::format("报告主公，刺杀失败，\x1b[1x{}\x1b[0x大人不幸被俘。", pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 3)
					pk::message_box(pk::encode(pk::format("报告主公，刺杀失败，\x1b[1x{}\x1b[0x大人不幸被杀。", pk::decode(pk::get_name(actor)))), pk::get_person(武将_伝令));

			}

		}


		//判断是否是义兄弟，夫妻，血缘，亲爱武将
		bool isFriend(pk::person@ person1, pk::person@ person2)
		{
			bool result = false;

			if (pk::is_gikyoudai(person1, person2.get_id()))
				result = true;
			if (pk::is_fuufu(person1, person2.get_id()))
				result = true;
			if (pk::is_ketsuen(person1, person2.get_id()))
				result = true;
			if (pk::is_oyako(person1, person2.get_id()))
				result = true;
			if (pk::is_like(person2, person1.get_id()))
				result = true;
			//君主都督太守被刺杀时，非厌恶武将都可能参战
			if ((person1.mibun == 身份_君主 or person1.mibun == 身份_都督 or person1.mibun == 身份_太守) and !pk::is_dislike(person2, person1.get_id()) and person1.get_id() != person2.get_id())
				result = true;
			return result;
		}

		bool isFriend2(pk::person@ person1, pk::person@ person2)
		{
			bool result = false;

			if (pk::is_gikyoudai(person1, person2.get_id()))
				result = true;
			if (pk::is_fuufu(person1, person2.get_id()))
				result = true;
			if (pk::is_ketsuen(person1, person2.get_id()))
				result = true;
			if (pk::is_oyako(person1, person2.get_id()))
				result = true;
			if (pk::is_like(person2, person1.get_id()))
				result = true;
			return result;
		}
		//判断是否有特殊技能且无厌恶关系
		bool isHuwei(pk::person@ person1, pk::person@ person2)
		{
			bool result = false;

			if (ch::has_skill(person2, 特技_护卫) and !pk::is_dislike(person2, person1.get_id()))
				result = true;
			return result;
		}

		//根据isFriend2以判断是否添加厌恶武将？返回武将列表
		pk::list<pk::person@> getDispersonList(pk::person@ src)
		{
			pk::list<pk::person@> list;
			pk::building@ src_buidling = pk::get_building(src.service);
			pk::list<pk::person@> posible_person_list = pk::get_person_list(pk::mibun_flags(身份_在野, 身份_俘虏, 身份_一般, 身份_君主, 身份_太守, 身份_都督));
			for (int i = 0; i < posible_person_list.count; i++)
			{
				pk::person@ dst = posible_person_list[i];
				if (isFriend2(src, dst))
					list.add(dst);
			}
			return list;
		}


		//根据isFriend以及特殊特技判断是否参与支援？返回武将列表
		pk::list<pk::person@> getAidpersonList(pk::person@ src)
		{
			pk::list<pk::person@> list;
			pk::building@ src_buidling = pk::get_building(src.service);
			pk::list<pk::person@> posible_person_list = pk::get_idle_person_list(src_buidling);
			for (int i = 0; i < posible_person_list.count; i++)
			{
				pk::person@ dst = posible_person_list[i];
				if ((isFriend(src, dst) or isHuwei(src, dst)) and dst.stat[武将能力_武力] > 80)
					list.add(dst);
			}
			//返回按武力排序的支援武将列表
			list.sort(function(a, b)
			{
				return (a.stat[武将能力_武力] > b.stat[武将能力_武力]);
			});
			return list;
		}

		//获取参战武将
		pk::int_int getAidperson(pk::list<pk::person@> list)
		{
			//if (list.count == 0) return;
			int aid1 = -1;
			int aid2 = -1;

			if (list.count == 1)
			{
				aid1 = list[0].get_id();
				//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我来帮你",pk::decode(pk::get_name(target_person))) ), list[1]);
			}
			if (list.count == 2)
			{
				aid1 = list[0].get_id();
				//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我来帮你",pk::decode(pk::get_name(target_person))) ), list[0]);					

				aid2 = list[1].get_id();
				//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我会保护你的",pk::decode(pk::get_name(target_person))) ), list[1]);
			}
			if (list.count > 2)
			{
				if (!好友选最高武力参战)
				{
					//半随机获取列表中的武将
					int aid1_ord = pk::rand(list.count);
					int aid2_ord;
					if (aid1_ord > 0)
						aid2_ord = aid1_ord - 1;
					else
						aid2_ord = aid1_ord + pk::rand(list.count - 1) + 1;

					aid1 = list[aid1_ord].get_id();
					//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我来帮你",pk::decode(pk::get_name(target_person))) ), list[aid1_ord]);	
					aid2 = list[aid2_ord].get_id();
					//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我会保护你的",pk::decode(pk::get_name(target_person))) ), list[aid2_ord]);
					//pk::trace(pk::format("参战武将:{}，{}",aid1, aid2));
				}
				else
				{
					aid1 = list[0].get_id();
					//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我来帮你",pk::decode(pk::get_name(target_person))) ), list[0]);					

					aid2 = list[1].get_id();
					//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我会保护你的",pk::decode(pk::get_name(target_person))) ), list[1]);					
				}
			}

			return pk::int_int(aid1, aid2);
		}
		/*
		void getAidperson(pk::list<pk::person@> list, pk::person@ aid1, pk::person@ aid2)
		{
			//if (list.count == 0) return;
			if (list.count == 1)
			{
				@aid1 = list[1];
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我来帮你",pk::decode(pk::get_name(target_person))) ), aid1);
			}
			if (list.count == 2)
			{
				@aid1 = list[1];
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我来帮你",pk::decode(pk::get_name(target_person))) ), aid1);

				@aid1 = list[2];
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我会保护你的",pk::decode(pk::get_name(target_person))) ), aid2);
			}
			if (list.count > 2)
			{
				//随机获取列表中的武将
				int aid1_ord = pk::rand(list.count - 1);
				int aid2_ord;
				if (aid1_ord > 0)
					aid2_ord = aid1_ord - 1;
				else
					aid2_ord = aid1_ord + pk::rand(list.count - 2) + 1;

				@aid1 = list[aid1_ord];
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我来帮你",pk::decode(pk::get_name(target_person))) ), aid1);
				@aid2 = list[aid2_ord];
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我会保护你的",pk::decode(pk::get_name(target_person))) ), aid2);
				pk::trace(pk::format("参战武将:{}，{}",aid1.get_id(), aid2.get_id()));
			}
		}
		*/

		pk::list<pk::building@> getTargetList(pk::building@ src)
		{
			pk::list<pk::building@> list;
			for (int i = 0; i < 建筑_据点末; i++)
			{
				pk::building@ dst = pk::get_building(i);
				int src_id = src.get_id();
				int dst_id = dst.get_id();
				//pk::trace(pk::format("对象据点{}，我方据点{}，相邻据点:{}，距离:{}",pk::decode(pk::get_name(dst)), pk::decode(pk::get_name(src)),pk::is_neighbor_base(src_id, dst_id), pk::get_building_distance(dst_id, src_id, src.get_force_id())));
				if (src_id != dst_id and /*pk::is_neighbor_base(src_id, dst_id)*/pk::get_building_distance(dst_id, src_id, src.get_force_id()) <= 刺杀_可执行距离 and src.get_force_id() != dst.get_force_id())
					list.add(dst);
			}
			return list;
		}

		bool check_strength(pk::person@ person1, pk::person@ person2, int dif)
		{
			if ((person2.stat[武将能力_武力] > person1.stat[武将能力_武力]) or (int(person1.stat[武将能力_武力] - person2.stat[武将能力_武力]) < dif))
				return true;
			return false;
		}

		void Add_dislike()
		{
			//第一种情况:目标死亡，刺客被俘，则说明刺客身份被曝光。
			if (flag_actor_result == 2 and flag_target_result == 2)
			{
				pk::list<pk::person@> list = getDispersonList(target_person);
				if (list.count > 0)
				{
					for (int i = 0; i < list.count; i++)
					{
						if (pk::rand_bool(30))
						{
							pk::add_dislike(list[i], actor.get_id());
							pk::history_log(force_.get_id(), force_.color, pk::encode(pk::format("因\x1b[1x{}\x1b[0x之死，\x1b[2x{}\x1b[0x将\x1b[2x{}\x1b[0x视为仇敌", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(list[i])), pk::decode(pk::get_name(actor)))));
						}
					}
				}
			}
		}

		void person_setting(bool change)
		{
			@ 禁卫 = pk::get_person(武将_兵士); // 716

			禁卫.sei = pk::encode("禁"); // 성
			禁卫.mei = pk::encode("卫"); // 명
			if (!change)
			{
				禁卫.sei = pk::encode("兵"); // 성
				禁卫.mei = pk::encode("士"); // 명				
			}
			//禁卫.face = 670;
			禁卫.base_stat[武将能力_武力] = 75 + pk::rand(10);

			// 무장 조형
			禁卫.body[0] = 74;
			禁卫.body[1] = 74;
			禁卫.body[2] = 74;
			禁卫.body[3] = 74;
			禁卫.body[4] = 74;
			禁卫.body[5] = 74;


			禁卫.update();
			//-------------------------------------------------------------------//
			@ 统领 = pk::get_person(武将_武将); // 743

			统领.sei = pk::encode("统"); // 성
			统领.mei = pk::encode("领"); // 명
			if (!change)
			{
				统领.sei = pk::encode("武"); // 성
				统领.mei = pk::encode("将"); // 명				
			}
			//统领.face = 747;
			统领.base_stat[武将能力_武力] = 78 + pk::rand(10);

			// 무장 조형
			统领.body[0] = 121;
			统领.body[1] = 119;
			统领.body[2] = 119;
			统领.body[3] = 119;
			统领.body[4] = 119;
			统领.body[5] = 119;


			统领.update();
			//-------------------------------------------------------------------//
			@ 护院 = pk::get_person(武将_卫士); // 744 

			护院.sei = pk::encode("护"); // 성
			护院.mei = pk::encode("院"); // 명
			if (!change)
			{
				护院.sei = pk::encode("卫"); // 성
				护院.mei = pk::encode("士"); // 명				
			}
			护院.face = 武将_卫士;
			护院.base_stat[武将能力_武力] = 70 + pk::rand(10);

			// 무장 조형
			护院.body[0] = 74;
			护院.body[1] = 83;
			护院.body[2] = 83;
			护院.body[3] = 83;
			护院.body[4] = 83;
			护院.body[5] = 83;


			护院.update();
			//-------------------------------------------------------------------//
			@ 家丁 = pk::get_person(武将_武芸者);  // 747

			家丁.sei = pk::encode("家"); // 성
			家丁.mei = pk::encode("丁"); // 명

			if (!change)
			{
				家丁.sei = pk::encode("武芸"); // 성
				家丁.mei = pk::encode("者"); // 명				
			}
			//家丁.face = 751;
			家丁.base_stat[武将能力_武力] = 67 + pk::rand(10);

			// 무장 조형
			家丁.body[0] = 101;
			家丁.body[1] = 124;
			家丁.body[2] = 120;
			家丁.body[3] = 120;
			家丁.body[4] = 120;
			家丁.body[5] = 120;

			家丁.update();
		}

		//武将隐藏武力设定
		bool show_sub_strength = false;
		void person_substrengh_setting(bool setting)
		{
			//设置隐藏武力
			if (setting)
			{
				//加入把显示隐藏武力去掉的设定
				show_sub_strength = pk::get_show_sub_strength();
				pk::set_show_sub_strength(false);
				pk::set_person_sub_strength(禁卫.get_id(), int((actor.stat[武将能力_武力] - 禁卫.base_stat[武将能力_武力]) / 2 + pk::rand(5)));
				pk::set_person_sub_strength(统领.get_id(), int((actor.stat[武将能力_武力] - 统领.base_stat[武将能力_武力]) / 2 + pk::rand(5)));
				pk::set_person_sub_strength(护院.get_id(), int((actor.stat[武将能力_武力] - 护院.base_stat[武将能力_武力]) / 2 + pk::rand(5)));
				pk::set_person_sub_strength(家丁.get_id(), int((actor.stat[武将能力_武力] - 家丁.base_stat[武将能力_武力]) / 2 + pk::rand(5)));
				pk::set_person_sub_zhugong(禁卫.get_id(), 4);
				pk::set_person_sub_zhugong(统领.get_id(), 4);
				pk::set_person_sub_zhugong(护院.get_id(), 2);
				pk::set_person_sub_zhugong(家丁.get_id(), 2);
				pk::set_person_sub_normal_attack_damage(禁卫.get_id(), 4);
				pk::set_person_sub_normal_attack_damage(统领.get_id(), 4);
				pk::set_person_sub_normal_attack_damage(护院.get_id(), 2);
				pk::set_person_sub_normal_attack_damage(家丁.get_id(), 2);
				pk::set_person_sub_anti_seckill(禁卫.get_id(), 200);
				pk::set_person_sub_anti_seckill(统领.get_id(), 200);
				pk::set_person_sub_anti_seckill(护院.get_id(), 200);
				pk::set_person_sub_anti_seckill(家丁.get_id(), 200);

			}
			if (!setting)
			{
				//清空附加武力
				pk::set_show_sub_strength(show_sub_strength);
				pk::set_person_sub_strength(禁卫.get_id(), 0);
				pk::set_person_sub_strength(统领.get_id(), 0);
				pk::set_person_sub_strength(护院.get_id(), 0);
				pk::set_person_sub_strength(家丁.get_id(), 0);
				pk::set_person_sub_zhugong(禁卫.get_id(), 0);
				pk::set_person_sub_zhugong(统领.get_id(), 0);
				pk::set_person_sub_zhugong(护院.get_id(), 0);
				pk::set_person_sub_zhugong(家丁.get_id(), 0);
				pk::set_person_sub_normal_attack_damage(禁卫.get_id(), 0);
				pk::set_person_sub_normal_attack_damage(统领.get_id(), 0);
				pk::set_person_sub_normal_attack_damage(护院.get_id(), 0);
				pk::set_person_sub_normal_attack_damage(家丁.get_id(), 0);
				pk::set_person_sub_anti_seckill(禁卫.get_id(), 0);
				pk::set_person_sub_anti_seckill(统领.get_id(), 0);
				pk::set_person_sub_anti_seckill(护院.get_id(), 0);
				pk::set_person_sub_anti_seckill(家丁.get_id(), 0);
			}
		}



		void item_setting()
		{
			for (int i = 0; i < int(宝物id.length); i++)
			{
				pk::item@ item_t = pk::get_item(宝物id[i]);
				宝物信息_暂存[i][0] = item_t.owner;
				宝物信息_暂存[i][1] = item_t.location;
				宝物信息_暂存[i][2] = item_t.status;
				if (target_person.mibun == 身份_君主 and item_t.owner != actor.get_id())
				{
					if (i % 2 == 0) item_t.owner = 禁卫.get_id();
					if (i % 2 != 0) item_t.owner = 统领.get_id();
				}
				else
				{
					if (pk::rand_bool(30) and item_t.owner != actor.get_id())
					{
						if (i % 2 == 0) item_t.owner = 家丁.get_id();
						if (i % 2 != 0) item_t.owner = 护院.get_id();
					}
				}
			}

		}

		void item_restore()
		{
			for (int i = 0; i < int(宝物id.length); i++)
			{
				pk::item@ item_t = pk::get_item(宝物id[i]);
				item_t.owner = 宝物信息_暂存[i][0];
				item_t.location = 宝物信息_暂存[i][1];
				item_t.status = 宝物信息_暂存[i][2];
			}
		}



		string get_health_info(int health)
		{
			string name;
			switch (health)
			{
			case 0: name = "健康"; break;
			case 1: name = "轻伤"; break;
			case 2: name = "重伤"; break;
			case 3: name = "濒死"; break;
			default: name = "健康"; break;
			}

			return name;
		}

	}

	Main main;
}
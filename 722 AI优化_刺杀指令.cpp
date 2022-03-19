// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/03 # 江东新风 # 尝试修复名字不恢复bug,修复刺杀中断bug ##
// ## 2021/09/26 # 江东新风 # 防秒设定由xml改成直接set ##
// ## 2021/09/18 # 江东新风 # 更改background路径 ##
// ## 2021/08/31 # 白马叔叔 # 略微提升护卫武力，新增游戏难度影响武力，修改护卫概率，玩家不AI刺杀 ##
// ## 2021/04/18 # 白马叔叔 # 新增战死无是否影响处死 ##
// ## 2021/04/10 # 白马叔叔 # 隐藏刺客身份、城中护卫概率前来帮助、刺客场间恢复、俘虏军团修正、伤情影响被俘概率 ##
// ## 2021/03/14 # 江东新风 # 添加防住刺客后的大礼包奖励 ##
// ## 2021/03/06 # 江东新风 # 防秒无效的bug修复，xml复制黏贴导致不识别错误 ##
// ## 2021/02/27 # 江东新风 # 刺杀可能的bug修复 ##
// ## 2021/02/07 # 江东新风 # 平衡性调整 ##
// ## 2021/01/31 # 江东新风 # rand函数错误修复 ##
// ## 2021/01/28 # 江东新风 # 修复场景未随机调用bug ##
// ## 2020/12/12 # 江东新风 # 添加刺杀保护月数和发动时期 ##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/12/06 # 江东新风 # 修复舌战成功仍会被杀死bug ##
// ## 2020/12/06 # 江东新风 # 再次修复舌战无法控制bug ##
// ## 2020/11/21 # 江东新风 # 修复姓名未重置的bug ##
// ## 2020/11/20 # 江东新风 # 修复舌战无法控制bug ##
// ## 2020/11/19 # 江东新风 # 优先刺杀高智武将 ##
// ## 2020/11/15 @ 江东新风 @ 初次编写 @@
namespace AI_刺杀
{
	const int 发生率_通常 = 7;//12
	const int AI_刺杀_可执行距离 = 3;
	const int AI刺杀_发动时期 = 1;        // 0: 每旬,  1: 每月第一旬,  2: 每季第一旬,  3: 每年第一旬 默认 1
	const int AI刺杀_保护月数 = 8;//5

	const int AI_刺杀失败_被俘率 = 6;  //原定6
	const int AI_刺杀失败_处死率 = 0;

	const int hp_add = 15;     //单局恢复体力值
	const int 伤情影响 = 5;     //伤情影响被俘概率

	const int AI_刺杀成功_被俘率 = 3;
	const int AI_刺杀成功_处死率 = 0;

	const bool 战死影响处死 = true;    //剧本战死影响AI处死。true为战死无时AI不处死，高战死高处死，false为处死行为与战死设置无关

	class Main
	{
		pk::force@ force_;
		pk::person@ kunshu_;
		pk::city@ city_;
		pk::building@ target_base;
		pk::person@ actor;
		pk::person@ target_person;
		pk::person@ helper;
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
		array<string> 潜入场景id = {/*城门1*/"media/background/assasinate/gate1.bmp", /*城门2*/"media/background/assasinate/gate2.bmp", /*街上1*/"media/background/assasinate/street1.bmp", /*街上2*/"media/background/assasinate/street2.bmp", /*酒馆*/"media/background/assasinate/tavern.bmp" };

		pk::person@ 禁卫;
		pk::person@ 统领;
		pk::person@ 护院;
		pk::person@ 家丁;

		string actor_sei;
		string actor_mei;
		int actor_face;

		Main()
		{
			pk::bind(111, pk::trigger111_t(on_turn_start));
		}



		void on_turn_start(pk::force@ force)
		{
			if (AI刺杀_发动时期 == 0)
				ass_Set(force);  // 매턴
			else if (AI刺杀_发动时期 == 1 and (pk::get_day() == 1))
				ass_Set(force);  // 매월 1일
			else if (AI刺杀_发动时期 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
				ass_Set(force);  // 매분기 첫달이면서 1일
			else if (AI刺杀_发动时期 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
				ass_Set(force);  // 매년 1월 1일			


		}

		void ass_Set(pk::force@ force)
		{
			if (force.is_player() and pk::get_elapsed_months() >= AI刺杀_保护月数)
			{
				@force_ = force;
				//pk::list<pk::person@> person_list = getAsspersonList();
				if (pk::rand_bool(发生率_通常))
				{
					auto dst_base_list = get_Near_enemy_base_list(force_);
					auto ass_person_list = getAsspersonList(dst_base_list, force_);
					if (ass_person_list.count > 0)
					{
						if (ass_person_list.count >= 2) ass_person_list.shuffle();
						{
							@actor = ass_person_list[0];
							auto target_person_list = getTargetpersonList(force_, actor);
							if (target_person_list.count > 0)
							{
								@target_person = target_person_list[pk::rand(target_person_list.count - 1)];
								@helper = getHelper(@target_person);
								//pk::trace(pk::format("刺杀者：{}, 被刺杀者：{}", pk::decode(pk::get_name(actor)), pk::decode(pk::get_name(target_person))));
								//刺杀者和目标都存在的时候再进行场景

								//刺客改头换面
								actor_sei = actor.sei;
								actor_mei = actor.mei;
								actor_face = actor.face;
								actor.sei = pk::decode("?");
								actor.mei = pk::decode("??");
								actor.face = 2016;
								actor.update();

								pk::scene(pk::scene_t(scene));

								//刺客重置姓名头像						
								actor.sei = actor_sei;
								actor.mei = actor_mei;
								actor.face = actor_face;
								actor.update();

								//参数初始化
								flag_ass_success = false;
								flag_ass_abandon = false;
								flag_actor_result = -1;//刺杀者结果，-1：默认值。0：无伤返回，1：负伤返回，2：被俘虏，3：被处决
								flag_target_result = -1;//被目标结果，-1：默认值。0：无伤逃脱，1：受伤，2：被处死
								event_result = 0;			// 决斗结果初始化
								//bool flag_ass_success = false; // 刺杀结果初始化
								debate_result = 0;			// 舌战结果初始化
								潜入场景 = 0;
								刺杀场景 = 0;
								actor_sei = "";
								actor_mei = "";
								actor_face = -1;
							}
						}
					}
				}
			}
		}

		void scene()
		{

			@target_base = pk::get_building(target_person.service);


			pk::move_screen(target_base.get_pos());
			刺杀场景 = pk::rand(4);
			pk::fade(0);
			pk::sleep();
			pk::background(场景id[刺杀场景]);
			pk::fade(255);

			//pk::background(22,1);

			//pk::cutin(21); 

			//刺杀前场景1--目标对象在闲逛之类的
			scene_before_ass();

			if (target_person.stat[武将能力_魅力] > 100 and actor.giri > 义理_普通 and pk::rand_bool(50))
				scene_ass_abandon();
			else
			{
				// 刺杀前场景对话 2
				scene_before_ass_2();

				// 刺杀，可以考虑多轮
				scene_ass_execute();
				//scene_ass_execute();
				// 刺杀后场景-胜利，失败 2, 3
				if (event_result == 0)
				{
					scene_ass_success();
				}
				else
				{
					//pk::trace(pk::format("单挑结果:{}",event_result));
					scene_ass_falied();
				}
			}
			//姓名重置
			person_setting(false);

			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);

			//pk::move_screen(building_.get_pos());
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
			//actor.loyalty -= 5;
			flag_ass_abandon = true;
		}

		void scene_ass_abandon2()
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

			//设计成护卫类型武将或者有义兄弟高武武将时，触发协助出场对话，吓走刺客（不能直接做成参与的情况，否则刺客基本必跪）

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
			//actor.loyalty -= 5;
			flag_ass_abandon = true;
		}

		void scene_ass_execute()
		{
			pk::unit@ dummy_unit;
			//pk::point emptyPos = getEmptyPos(building_);
			//pk::unit@ unit_temp = pk::create_unit(building_, actor, null, null, 100, 병기_창, 兵器_走舸, 0, 100, emptyPos);
			//@dummy_unit = pk::get_unit(0);
			pk::person@ dummy_person;
			//pk::person@ helper1;	
			//pk::person@ helper2;
			person_setting(true);
			//if (护卫带宝物) item_setting();


			//对象为非君主的场景  duel发起方胜利返回值为0
			if (target_person.mibun != 身份_君主)
			{
				int str_dif = actor.stat[武将能力_武力] - target_person.stat[武将能力_武力];
				//pk::set_person_sub_strength(target_person.get_id(), str_dif+10);
				if (str_dif < 15)
				{
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, target_person, dummy_person, dummy_person, false, true, 0, true).first;

				}
				if (str_dif >= 15 and str_dif < 30)
				{
					person_setting(true);
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 家丁, target_person, dummy_person, false, true, 0, true).first;
				}
				if (str_dif >= 30 and str_dif < 45)
				{
					person_setting(true);
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 家丁, 家丁, target_person, false, true, 0, true).first;
				}
				if (str_dif >= 45)
				{
					person_setting(true);
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 护院, 家丁, 家丁, false, true, 0, true).first;
					/* if (event_result == 0)
					{
						pk::apply_duel_result(0,0);  //同步指定武将的体力伤病, pk::apply_duel_result(0发起方1接受方, 0第一位1第二位2第三位)
						pk::message_box(pk::encode("就剩下你一人了！"), actor);
						event_result += pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, target_person, dummy_person, dummy_person, false, true, 0, true).first;
					} */
				}
				int help_概率 = 60;  //护卫帮助前来护卫的概率
				if (helper !is null)
				{
					help_概率 -= int(pk::get_aishou_distance(helper, target_person.get_id()) / 2);  //减去相性差的1/2
					if (pk::is_ketsuen(helper, target_person.get_id())) help_概率 += 50;  //是血缘关系加20
					if (pk::is_like(helper, target_person.get_id()) || pk::is_like(target_person, helper.get_id()) || pk::is_gikyoudai(helper, target_person.get_id()) || pk::is_oyako(helper, target_person.get_id()) || pk::is_oyako(target_person, helper.get_id()) || pk::is_fuufu(helper, target_person.get_id()) || pk::is_fuufu(target_person, helper.get_id()) || help_概率 > 100) help_概率 = 100;  //但如果亲爱、义兄弟、亲子、夫妻则100%帮忙				
					//pk::trace(pk::format("帮忙概率：{}", help_概率));
				}

				if (event_result == 0 && helper !is null && pk::rand_bool(help_概率))
				{
					pk::apply_duel_result(0, 0);  //同步指定武将的体力伤病, pk::apply_duel_result(0发起方1接受方, 0第一位1第二位2第三位)
					pk::message_box(pk::encode("呼...还好赶上了."), helper);
					pk::message_box(pk::encode("刺客休得猖狂!"), helper);
					pk::add_hp(actor, hp_add);  //场次间隙恢复体力
					event_result += pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, helper, target_person, dummy_person, false, true, 0, true).first;
				}
				if (event_result == 0 && str_dif >= 45)
				{
					pk::apply_duel_result(0, 0);  //同步指定武将的体力伤病, pk::apply_duel_result(0发起方1接受方, 0第一位1第二位2第三位)
					pk::add_hp(actor, hp_add);  //场次间隙恢复体力
					pk::message_box(pk::encode("就剩下你一人了！"), actor);
					event_result += pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, target_person, dummy_person, dummy_person, false, true, 0, true).first;
				}
			}

			//对象为君主的场景--未完成  duel发起方胜利返回值为0
			if (target_person.mibun == 身份_君主)
			{
				int str_dif = actor.stat[武将能力_武力] - target_person.stat[武将能力_武力];
				if (str_dif < 15)
				{
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, target_person, dummy_person, dummy_person, false, true, 0, true).first;
				}
				if (str_dif >= 15 and str_dif < 30)
				{
					person_setting(true);
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 禁卫, target_person, dummy_person, false, true, 0, true).first;
				}
				if (str_dif >= 30 and str_dif < 45)
				{
					person_setting(true);
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 禁卫, 禁卫, target_person, false, true, 0, true).first;
				}
				if (str_dif >= 45)
				{
					person_setting(true);
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, 统领, 禁卫, 禁卫, false, true, 0, true).first;
					/* if (event_result == 0)
					{
						pk::message_box(pk::encode("就剩下你一人了！"), actor);
						pk::apply_duel_result(0,0);  //同步指定武将的体力伤病, pk::apply_duel_result(0发起方1接受方, 0第一位1第二位2第三位)
						event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, target_person, dummy_person, dummy_person, false, true, 0, true).first;
					} */
				}
				int help_概率 = 60;  //护卫帮助前来护卫的概率
				if (helper !is null)
				{
					help_概率 -= int(pk::get_aishou_distance(helper, target_person.get_id()) / 2);  //减去相性差的1/2
					if (pk::is_ketsuen(helper, target_person.get_id())) help_概率 += 50;  //是血缘关系加20
					if (pk::is_like(helper, target_person.get_id()) || pk::is_like(target_person, helper.get_id()) || pk::is_gikyoudai(helper, target_person.get_id()) || pk::is_oyako(helper, target_person.get_id()) || pk::is_oyako(target_person, helper.get_id()) || pk::is_fuufu(helper, target_person.get_id()) || pk::is_fuufu(target_person, helper.get_id()) || help_概率 > 100) help_概率 = 100;  //但如果亲爱、义兄弟、亲子、夫妻则100%帮忙				
					//pk::trace(pk::format("帮忙概率：{}", help_概率));
				}

				if (event_result == 0 && helper !is null && pk::rand_bool(help_概率))
				{
					pk::apply_duel_result(0, 0);  //同步指定武将的体力伤病, pk::apply_duel_result(0发起方1接受方, 0第一位1第二位2第三位)
					pk::add_hp(actor, hp_add);  //场次间隙恢复体力
					pk::message_box(pk::encode("呼...还好赶上了."), helper);
					pk::message_box(pk::encode("刺客休得猖狂!"), helper);
					event_result += pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, helper, target_person, dummy_person, false, true, 0, true).first;
				}
				if (event_result == 0 && str_dif >= 45)
				{
					pk::message_box(pk::encode("就剩下你一人了！"), actor);
					pk::apply_duel_result(0, 0);  //同步指定武将的体力伤病, pk::apply_duel_result(0发起方1接受方, 0第一位1第二位2第三位)
					pk::add_hp(actor, hp_add);  //场次间隙恢复体力
					event_result = pk::duel(dummy_unit, dummy_unit, actor, dummy_person, dummy_person, target_person, dummy_person, dummy_person, false, true, 0, true).first;
				}
				if (event_result == 0 and pk::rand_bool(80))
				{
					actor.hp = 100;
					pk::message_box(pk::encode("纳命来！"), actor);
					pk::message_box(pk::encode("好汉且慢，听我一言！"), target_person);
					debate_result = pk::debate(actor, target_person, false, true, true, true).first;
					//结果为1表示刺杀者被说服
					event_result += debate_result;
				}

			}
			//pk::trace(pk::format("单挑结果：{}", event_result));
			//恢复游戏原来的宝物所属
			//if (护卫带宝物) item_restore();

			//写在此处好像因为多线程的原因会提前加载？
			//person_substrengh_setting(false);
		}

		void scene_ass_success()
		{
			flag_ass_success = true;
			pk::message_box(pk::encode("你的护卫不过如此。接受制裁吧。"), actor);

			//为了降低不适感，直接被刺死的几率稍微下调，修改为剧本战死会影响处死
			int 处死发生率 = 20;
			if (战死影响处死)
			{
				switch (pk::get_scenario().battle_death)
				{
				case 0: 处死发生率 = 0; break;  //无战死为0
				case 2: 处死发生率 = 50; break;  //高战死为50
				default: 处死发生率 = 20;  //上级为20
				}
			}

			bool 处死发生 = pk::rand_bool(处死发生率);
			if (!处死发生)
			{
				pk::message_box(pk::encode("这次就放你一马，希望你能记住这次教训！"), actor);
				int 健康状态 = 2 + pk::rand(1);
				target_person.shoubyou = 健康状态;
				target_person.update();
				flag_target_result = 1;
				pk::history_log(force_.get_id(), force_.color, pk::encode(pk::format("受到敌军刺杀，\x1b[1x{}\x1b[0x的健康状态变为\x1b[29x{}\x1b[0x。", pk::decode(pk::get_name(target_person)), get_health_info(健康状态))));
			}
			else
			{
				pk::message_box(pk::encode("可恶，没想到竟要命丧于此！"), target_person);
				pk::message_box(pk::encode("动手前，报上名来！"), target_person);
				switch (pk::rand(2))
				{
				case 0: pk::message_box(pk::encode(pk::format("姓\x1b[1x{}\x1b[0x名\x1b[1x{}\x1b[0x！", pk::decode(actor_sei), pk::decode(actor_mei))), actor); break;
				case 1: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x是也！", pk::decode(pk::get_name(actor)))), actor); break;
				}
				pk::play_se(5);
				pk::message_box(pk::encode("唔唔..."), target_person);
				pk::history_log(force_.get_id(), force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x被\x1b[2x{}\x1b[0x刺杀了", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))));
				pk::kill(target_person);
				flag_target_result = 2;
			}
			/* if (pk::rand_bool(20))  (!战死无不处死 or pk::get_scenario().battle_death != 0)
			{
				pk::message_box(pk::encode("可恶，没想到竟要命丧于此！"), target_person);
				pk::message_box(pk::encode("动手前，报上名来！"), target_person);
				pk::message_box(pk::encode(pk::format("姓\x1b[1x{}\x1b[0x名\x1b[1x{}\x1b[0x！", pk::decode(actor_sei), pk::decode(actor_mei))), actor);
				pk::play_se(5);
				pk::message_box(pk::encode("唔唔..."), target_person);
				pk::history_log(force_.get_id(), force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x被\x1b[2x{}\x1b[0x刺杀了", pk::decode(pk::get_name(target_person)), pk::decode(pk::get_name(actor)))));
				pk::kill(target_person);
				flag_target_result = 2;
			}
			else
			{
				pk::message_box(pk::encode("这次就放你一马，希望你能记住这次教训！"), actor);
				int 健康状态 = 2 + pk::rand(1);
				target_person.shoubyou = 健康状态;
				target_person.update();
				flag_target_result = 1;
				pk::history_log(force_.get_id(), force_.color, pk::encode(pk::format("受到敌军刺杀，\x1b[1x{}\x1b[0x的健康状态变为\x1b[29x{}\x1b[0x。", pk::decode(pk::get_name(target_person)), get_health_info(健康状态))));
			} */


			int actor_result = -1;
			actor_result = pk::rand(100) + 1;
			///根据结果选择对话及处理
			//逃脱
			if (actor_result <= (100 - AI_刺杀成功_处死率 - AI_刺杀成功_被俘率 - actor.shoubyou * 伤情影响))
			{
				pk::message_box(pk::encode("任务已经成功，这就撤吧..."), actor);
				pk::message_box(pk::encode("一定要抓住他"), pk::get_person(武将_兵士));
				flag_actor_result = 0;

			}
			//被俘虏
			else if (actor_result <= (100 - AI_刺杀成功_处死率) and actor_result > (100 - AI_刺杀成功_处死率 - AI_刺杀成功_被俘率 - actor.shoubyou * 伤情影响))
			{
				pk::message_box(pk::encode("可恶，没想到被包围了..."), actor);
				pk::message_box(pk::encode("绝对不能让刺客逃跑。\n围上去！"), pk::get_person(武将_兵士));
				pk::message_box(pk::encode("..."), actor);
				pk::message_box(pk::encode("他已经力竭了，抓住他！"), pk::get_person(武将_兵士));
				//需要考虑如果ai武将在执行其他任务，这样设定是否会导致bug-20210227
				actor.former_force = actor.get_force_id();
				pk::set_district(actor, (pk::get_person(pk::get_kunshu_id(actor))).get_district_id());
				actor.mibun = 身份_俘虏;
				actor.location = target_base.get_id();
				pk::set_service(actor, target_base.get_id());
				//actor.service = target_base.get_id();

				flag_actor_result = 2;
			}
			//被处死
			else if (actor_result <= 100 and actor_result > (100 - AI_刺杀成功_处死率))
			{
				pk::message_box(pk::encode("居然围了那么多人..."), actor);
				pk::message_box(pk::encode("一定要抓住这个刺客！"), pk::get_person(武将_兵士));
				pk::message_box(pk::encode("...可恶..."), actor);
				pk::message_box(pk::encode("砍死他！"), pk::get_person(武将_兵士));
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
			if (actor_result <= (100 - AI_刺杀失败_处死率 - AI_刺杀失败_被俘率 - actor.shoubyou * 伤情影响))
			{
				pk::message_box(pk::encode(pk::format("{}，你的脑袋就先记在你的脖子上，下次再来取走。", pk::decode(pk::get_name(target_person)))), actor);
				flag_actor_result = 0;

			}
			//被俘虏
			else if (actor_result <= (100 - AI_刺杀失败_处死率) and actor_result > (100 - AI_刺杀失败_处死率 - AI_刺杀失败_被俘率 - actor.shoubyou * 伤情影响))
			{
				pk::message_box(pk::encode("可恶，没想到被包围了..."), actor);
				pk::message_box(pk::encode("绝对不能让刺客逃跑。\n围上去！"), pk::get_person(武将_兵士));
				pk::message_box(pk::encode("..."), actor);
				pk::message_box(pk::encode("他已经力竭了，抓住他！"), pk::get_person(武将_兵士));
				//需要考虑如果ai武将在执行其他任务，这样设定是否会导致bug-20210227
				actor.former_force = actor.get_force_id();
				pk::set_district(actor, (pk::get_person(pk::get_kunshu_id(actor))).get_district_id());
				actor.mibun = 身份_俘虏;
				actor.location = target_base.get_id();
				//actor.service = target_base.get_id();
				pk::set_service(actor, target_base.get_id());
				flag_actor_result = 2;
			}
			//被处死
			else if (actor_result <= 100 and actor_result > (100 - AI_刺杀失败_处死率))
			{
				pk::message_box(pk::encode("居然围了那么多人..."), actor);
				pk::message_box(pk::encode("一定要抓住这个刺客！"), pk::get_person(武将_兵士));
				pk::message_box(pk::encode("...可恶..."), actor);
				pk::message_box(pk::encode("砍死他！"), pk::get_person(武将_兵士));
				pk::kill(actor);
				flag_actor_result = 3;
			}

			//防御成功奖励 target_person

			if (pk::rand_bool(50))
			{
				pk::message_box(pk::encode("呼...呼...呼....\n好险，差点以为要死了呢...还好有你们在呀~"), target_person);
				if (target_person.mibun != 身份_君主) pk::message_box(pk::encode("保护大人的安全是我们应尽的义务！"), 家丁);
				else pk::message_box(pk::encode("保护主公的安全是我们应尽的义务！"), 禁卫);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的统率经验大幅上升。", pk::decode(pk::get_name(target_person)))));
				pk::add_stat_exp(target_person, 武将能力_统率, 90);
			}
			else
			{
				pk::message_box(pk::encode("呼...呼...呼....\n好险，没想到我居然能架住刺客的致命一击。"), target_person);
				pk::message_box(pk::encode("看来以后，身体的锻炼也不能落下呀。"), target_person);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的武力经验大幅上升。", pk::decode(pk::get_name(target_person)))));
				pk::add_stat_exp(target_person, 武将能力_武力, 90);
			}
			//刺客未逃跑时，获得金钱
			if (actor_result > (100 - AI_刺杀失败_处死率 - AI_刺杀失败_被俘率 - actor.shoubyou * 伤情影响))
			{
				int gold_gain = (pk::rand(3) + 1) * 50;
				pk::play_sfx(11);
				pk::message_box(pk::encode(pk::format("从刺客手上搜的一些财物。(金：{})", gold_gain)));
				pk::add_gold(target_base, gold_gain);
			}
			pk::play_sfx(11);
			pk::message_box(pk::encode("获得100技巧点。"));

			ch::add_tp(pk::get_force(target_base.get_force_id()), 100, target_base.get_pos());

		}

		void scene_ass_report()
		{
			if (flag_ass_abandon)
				pk::message_box(pk::encode(pk::format("报告主公，我方\x1b[1x{}\x1b[0x被敌军刺客刺杀，所幸\x1b[2x无恙\x1b[0x，刺客已远遁。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));


			if (flag_ass_success and flag_target_result == 1)
			{
				if (flag_actor_result == 0)
					pk::message_box(pk::encode(pk::format("报告主公，我方\x1b[1x{}\x1b[0x被敌军刺客\x1b[16x刺伤\x1b[0x，刺客已远遁。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 1)
					pk::message_box(pk::encode(pk::format("报告主公，我方\x1b[1x{}\x1b[0x被敌军刺客\x1b[16x刺伤\x1b[0x，刺客负伤而逃。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 2)
					pk::message_box(pk::encode(pk::format("报告主公，我方\x1b[1x{}\x1b[0x被敌军刺客\x1b[16x刺伤\x1b[0x，刺客已被我们俘虏。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 3)
					pk::message_box(pk::encode(pk::format("报告主公，我方\x1b[1x{}\x1b[0x被敌军刺客\x1b[16x刺伤\x1b[0x，刺客被我们乱刀砍死。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));

			}
			if (flag_ass_success and flag_target_result == 2)
			{
				if (flag_actor_result == 0)
					pk::message_box(pk::encode(pk::format("报告主公，我方\x1b[1x{}\x1b[0x被敌军刺客\x1b[29x刺死\x1b[0x，刺客已远遁。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 1)
					pk::message_box(pk::encode(pk::format("报告主公，我方\x1b[1x{}\x1b[0x被敌军刺客\x1b[29x刺死\x1b[0x，刺客负伤而逃。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 2)
					pk::message_box(pk::encode(pk::format("报告主公，我方\x1b[1x{}\x1b[0x被敌军刺客\x1b[29x刺死\x1b[0x，刺客已被我们俘虏。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 3)
					pk::message_box(pk::encode(pk::format("报告主公，我方\x1b[1x{}\x1b[0x被敌军刺客\x1b[29x刺死\x1b[0x，刺客被我们乱刀砍死。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));

			}
			if (!flag_ass_success and !flag_ass_abandon)
			{
				if (flag_actor_result == 0)
					pk::message_box(pk::encode(pk::format("报告主公，我方\x1b[1x{}\x1b[0x被敌军刺客刺杀，所幸\x1b[2x无恙\x1b[0x，刺客已远遁。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 1)
					pk::message_box(pk::encode(pk::format("报告主公，我方\x1b[1x{}\x1b[0x被敌军刺客刺杀，所幸\x1b[2x无恙\x1b[0x，刺客负伤而逃。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 2)
					pk::message_box(pk::encode(pk::format("报告主公，我方\x1b[1x{}\x1b[0x被敌军刺客刺杀，所幸\x1b[2x无恙\x1b[0x，刺客已被我们俘虏。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));
				if (flag_actor_result == 3)
					pk::message_box(pk::encode(pk::format("报告主公，我方\x1b[1x{}\x1b[0x被敌军刺客刺杀，所幸\x1b[2x无恙\x1b[0x，刺客被我们乱刀砍死。", pk::decode(pk::get_name(target_person)))), pk::get_person(武将_伝令));

			}

		}

		bool check_strength(pk::person@ person1, pk::person@ person2, int dif)
		{
			if ((person2.stat[武将能力_武力] > person1.stat[武将能力_武力]) or (int(person1.stat[武将能力_武力] - person2.stat[武将能力_武力]) < dif))
				return true;
			return false;
		}

		pk::list<pk::person@> getAsspersonList(pk::list<pk::building@> assable_base, pk::force@ src_force)
		{
			pk::list<pk::person@> list;
			//只获取符合条件的身份，排除俘虏，未登场，君主，太守，都督
			pk::list<pk::person@> list_t = pk::get_person_list(pk::mibun_flags(身份_一般));
			for (int i = 0; i < list_t.count; i++)
			{
				pk::person@ Assperson = list_t[i];


				//执行刺杀武将的武力必须大于90
				if (Assperson.stat[武将能力_武力] < 90) continue;

				//排除非可执行刺杀据点的武将				
				auto ass_base = pk::get_building(Assperson.service);
				if (!assable_base.contains(ass_base)) continue;


				//排除出征武将，是否要排除执行任务中的武将？
				if (Assperson.location > 86) continue;

				//如果是多人游玩联机，需要排除AI玩家刺杀
				if (Assperson.is_player()) continue;

				//由于可执行刺杀据点的武将肯定是敌方势力的，所以不需要判断敌我
				//排除己方势力
				//if (Assperson.get_force_id() == src_force.get_id()) continue;									

				list.add(Assperson);

			}
			return list;
		}

		//根据刺杀武将来获取可刺杀武将列表，随机生成五个目标，防止占用资源
		pk::list<pk::person@> getTargetpersonList(pk::force@ src_force, pk::person@ dst_person)
		{
			pk::list<pk::person@> list;
			//只获取符合条件的身份，排除俘虏，未登场, -----只取我方势力
			pk::list<pk::person@> list_t = pk::get_person_list(src_force, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
			//打乱顺序
			//list_t.shuffle();
			list_t.sort(function(a, b)
			{
				if (a.stat[武将能力_智力] != b.stat[武将能力_智力])
					return a.stat[武将能力_智力] > b.stat[武将能力_智力];
				if (a.stat[武将能力_政治] != b.stat[武将能力_政治])
					return a.stat[武将能力_政治] > b.stat[武将能力_政治];
				if (a.stat[武将能力_武力] != b.stat[武将能力_武力])
					return a.stat[武将能力_武力] < b.stat[武将能力_武力];
				return a.get_id() < b.get_id(); // 先按武力倒序，再政治排序，再按智力排序，再按id排序
			});
			int n = 0;
			for (int i = 0; i < list_t.count; i++)
			{
				if (n > 5) break;
				pk::person@ Targetperson = list_t[i];
				//排除出征武将，是否要排除执行任务中的武将？
				if (Targetperson.location > 86)
					continue;
				//排除不在城中的武将
				if (Targetperson.location != Targetperson.service)
					continue;
				//被刺杀武将的武力不要大于85，免得刺客送菜
				if (Targetperson.stat[武将能力_武力] > 85)
					continue;
				int src_id = pk::get_building(Targetperson.service).get_id();
				int dst_id = pk::get_building(dst_person.service).get_id();
				if (pk::get_building_distance(dst_id, src_id, Targetperson.get_force_id()) <= AI_刺杀_可执行距离)
				{
					list.add(Targetperson);
					n += 1;
				}
			}

			return list;
		}



		//根据势力获取指定范围内的地方城市列表
		pk::list<pk::building@> get_Near_enemy_base_list(pk::force@ src_force)
		{
			pk::list<pk::building@> list;
			//获取友方城市
			pk::list<pk::building@> src_list;
			for (int i = 0; i < 建筑_据点末; i++)
			{
				auto building_t = pk::get_building(i);
				if (building_t.get_force_id() == src_force.get_id())
					src_list.add(building_t);
			}
			for (int i = 0; i < src_list.count; i++)
			{
				auto src = src_list[i];
				for (int j = 0; j < 建筑_据点末; j++)
				{
					auto dst = pk::get_building(j);
					//排除我方城市
					if (src_list.contains(dst)) continue;
					//排除已纳入城市
					if (list.contains(dst)) continue;
					//确定距离
					int src_id = src.get_id();
					int dst_id = dst.get_id();
					if (pk::get_building_distance(dst_id, src_id, dst.get_force_id()) <= AI_刺杀_可执行距离)
						list.add(dst);
				}
			}

			return list;
		}

		//获取城中护卫武将
		pk::person@ getHelper(pk::person@ target_person)
		{
			pk::building@ target_base = pk::get_building(target_person.service);
			if (!target_base.has_skill(特技_护卫)) return null;
			pk::list<pk::person@> list;
			pk::list<pk::person@> list_0 = pk::get_person_list(target_base, pk::mibun_flags(身份_君主, 身份_太守, 身份_都督, 身份_一般));
			for (int i = 0; i < list_0.count; i++)
			{
				pk::person@ helper_person = list_0[i];
				if (!ch::has_skill(helper_person, 特技_护卫))
					continue;  //武将不是护卫
				if (helper_person.location > 86)
					continue;  //排除出征护卫，是否要排除执行任务中的武将？				
				if (helper_person.location != helper_person.service)
					continue;  //排除不在城中的护卫
				if (helper_person.absence_timer != 0)
					continue;  //排除任务中的护卫
				if (helper_person.stat[武将能力_武力] < 65)
					continue;  //护卫武将的武力要大于65
				if (pk::is_dislike(target_person, helper_person.get_id()) or pk::is_dislike(helper_person, target_person.get_id()))
					continue;  //两者存在厌恶不帮忙
				else
					list.add(helper_person);
			}

			if (list.count > 1)  //多个时取武力最高者
			{
				//武将按武力排序
				list.sort(function(a, b)
				{
					return a.stat[武将能力_武力] > b.stat[武将能力_武力];
				});
				return list[0];
			}
			else if (list.count == 1) return list[0];
			else return null;
		}

		void person_setting(bool change)
		{
			@禁卫 = pk::get_person(武将_兵士); // 716

			禁卫.sei = pk::encode("禁"); // 성
			禁卫.mei = pk::encode("卫"); // 명			
			if (!change)
			{
				禁卫.sei = pk::encode("兵"); // 성
				禁卫.mei = pk::encode("士"); // 명				
			}
			int 难度武力buff = 0;
			if (pk::get_scenario().difficulty == 0) 难度武力buff = 15;  //初级难度加15，初定
			else if (pk::get_scenario().difficulty == 1) 难度武力buff = 6;  //上级难度加6，初定
			//禁卫.face = 670;
			禁卫.base_stat[武将能力_武力] = 68 + 难度武力buff + pk::rand(10);  //游戏难度影响武力

			// 무장 조형
			禁卫.body[0] = 74;
			禁卫.body[1] = 74;
			禁卫.body[2] = 74;
			禁卫.body[3] = 74;
			禁卫.body[4] = 74;
			禁卫.body[5] = 74;
			person_anti_seckill_setting(change);


			禁卫.update();
			//-------------------------------------------------------------------//
			@统领 = pk::get_person(武将_武将); // 743

			统领.sei = pk::encode("统"); // 성
			统领.mei = pk::encode("领"); // 명
			if (!change)
			{
				禁卫.sei = pk::encode("武"); // 성
				禁卫.mei = pk::encode("将"); // 명				
			}
			//统领.face = 747;
			统领.base_stat[武将能力_武力] = 71 + 难度武力buff + pk::rand(10);  //游戏难度影响武力

			// 무장 조형
			统领.body[0] = 121;
			统领.body[1] = 119;
			统领.body[2] = 119;
			统领.body[3] = 119;
			统领.body[4] = 119;
			统领.body[5] = 119;
			person_anti_seckill_setting(change);

			统领.update();
			//-------------------------------------------------------------------//
			@护院 = pk::get_person(武将_卫士); // 744 

			护院.sei = pk::encode("护"); // 성
			护院.mei = pk::encode("院"); // 명
			if (!change)
			{
				护院.sei = pk::encode("卫"); // 성
				护院.mei = pk::encode("士"); // 명				
			}
			护院.face = 武将_卫士;
			护院.base_stat[武将能力_武力] = 65 + 难度武力buff + pk::rand(10);  //游戏难度影响武力

			// 무장 조형
			护院.body[0] = 74;
			护院.body[1] = 83;
			护院.body[2] = 83;
			护院.body[3] = 83;
			护院.body[4] = 83;
			护院.body[5] = 83;
			person_anti_seckill_setting(change);


			护院.update();
			//-------------------------------------------------------------------//
			@家丁 = pk::get_person(武将_武芸者);  // 747

			家丁.sei = pk::encode("家"); // 성
			家丁.mei = pk::encode("丁"); // 명

			if (!change)
			{
				家丁.sei = pk::encode("武芸"); // 성
				家丁.mei = pk::encode("者"); // 명				
			}
			//家丁.face = 751;
			家丁.base_stat[武将能力_武力] = 62 + 难度武力buff + pk::rand(10);  //游戏难度影响武力

			// 무장 조형
			家丁.body[0] = 101;
			家丁.body[1] = 124;
			家丁.body[2] = 120;
			家丁.body[3] = 120;
			家丁.body[4] = 120;
			家丁.body[5] = 120;
			person_anti_seckill_setting(change);

			家丁.update();
		}

		void person_anti_seckill_setting(bool setting)
		{
			//设置隐藏武力
			if (setting)
			{
				if (禁卫 !is null) pk::set_person_sub_anti_seckill(禁卫.get_id(), 200);
				if (统领 !is null) pk::set_person_sub_anti_seckill(统领.get_id(), 200);
				if (护院 !is null) pk::set_person_sub_anti_seckill(护院.get_id(), 200);
				if (家丁 !is null) pk::set_person_sub_anti_seckill(家丁.get_id(), 200);

			}
			if (!setting)
			{
				pk::set_person_sub_anti_seckill(武将_兵士, 0);//禁卫
				pk::set_person_sub_anti_seckill(武将_武将, 0);//统领
				pk::set_person_sub_anti_seckill(武将_卫士, 0);//护院
				pk::set_person_sub_anti_seckill(武将_武芸者, 0);//家丁
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
		//以下为弃用
		/*
				//根据可刺杀武将选出可刺杀据点
				pk::list<pk::building@> getAssbaseList(pk::list<pk::person@> list)
				{
					pk::list<pk::building@> building_list;
					for (int i = 0; i < list.count; i++)
					{
						pk::building@ building_t = pk::get_building(list[i].service);
						//只纳入非重复的据点
						if (!list.contains(building_t))
						   list.add(building_t);
					}
					return list;
				}
		*/
	}
	Main main;
}
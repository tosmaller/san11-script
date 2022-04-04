// ## 2020/12/24 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/23 #江东新风#修复曹子孝的台词bug##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: 부대별 대사 설정
@기타: 매턴마다 부대의 '주장'이 대사를 시전합니다.

*/

namespace 부대별_대사
{

	// ==================CUSTOMIZE =================

	const bool mention_ = true;   // true (대사 시전), false (대사 시전 안함)

// =============================================

	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{
			leader_lines();
		}

		void leader_lines()
		{
			auto list = pk::list_to_array(pk::get_unit_list());


			for (int i = 0; i < int(list.length); i++)
			{

				pk::unit@ src = list[i];
				pk::point pos = src.get_pos();

				// 해당 무장이 PLAYER 소속일 경우에 대사를 시전
				if (mention_ and pk::is_player_controlled(src))
				{

					if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_여포) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(5))
						{
						case 0: pk::say(pk::encode("我,举世无双!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("世上无人能阻档我!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("杂鱼们全都给我滚开!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("全都去死!"), pk::get_person(src.leader)); break;
						case 4: pk::say(pk::encode("人中吕布,马中赤兔!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_여영기) and pk::is_valid_unit_status(部队状态_通常)) // 여령기
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("我就是无双猛将之女\x1b[1x吕玲绮\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("让你们见识下传自父亲的武艺!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("全都去死!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("不想死的全滚开!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_유비) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("我就是中山靖王之后\x1b[1x刘玄德\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("我的双剑出神入化!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("为了天下百姓!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("为了天下大义!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_관우) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(5))
						{
						case 0: pk::say(pk::encode("吾乃美髯公\x1b[1x关羽\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("我对我的武艺很有自信."), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("为了大义!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("青龙偃月刀!"), pk::get_person(src.leader)); break;
						case 4: pk::say(pk::encode("看尔乃插标卖首之徒!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_장비) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(5))
						{
						case 0: pk::say(pk::encode("吾乃燕人\x1b[1x张翼德\x1b[0x是也!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("我才是天下最强的男人!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("不想死的给我滚开!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("看我喝退百万雄师!"), pk::get_person(src.leader)); break;
						case 4: pk::say(pk::encode("百万军中取人首级如探囊取物!!!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_조운) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("吾乃常山\x1b[1x赵子龙\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("能进能退,乃真正法器!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("为将者当尽职尽忠!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("敌羞吾去脱他衣!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_마초) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("吾乃西凉\x1b[1x锦马超\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("都给我闪一边去!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("见识下我的长枪!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("全军突击!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_황충) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("可别小看了老将!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("我的箭百发百中."), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("小不点们都闪开!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("我可不会输给年轻人!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_위연) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("\x1b[1x魏延\x1b[0x参上!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("谁敢杀我!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("敌人...全部...打倒..."), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("全都消失!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_강유) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(5))
						{
						case 0: pk::say(pk::encode("吾乃天水麒麟儿\x1b[1x姜维\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("这么明显的计策!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("为了丞相!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("品尝下我的枪法!"), pk::get_person(src.leader)); break;
						case 4: pk::say(pk::encode("如数奉还!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 673) and pk::is_valid_unit_status(部队状态_通常)) // 관은병
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("我是武圣之女\x1b[1x关银屏\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("父亲传我的武艺\n绝不会辱没!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("我一个人就能解决你们!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("我已经抑制不住了!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_조조) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(5))
						{
						case 0: pk::say(pk::encode("治世之能臣,乱世之奸雄!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("孤之霸道在此示于世间."), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("霸道前容不得汝等!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("让你们见识下我的用兵之道!"), pk::get_person(src.leader)); break;
						case 4: pk::say(pk::encode("霸道是我的宿命.."), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_조홍) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("吾乃\x1b[1x曹子廉\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("知道黄金头奎的传说嘛?"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("无名之辈滚一边去!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("挫折感培养男子汉!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 338) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("吾乃黄须儿\x1b[1x曹子文\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("干掉他们!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("都给我让开!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("给我溃散!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_조인) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("我乃是具有的\x1b[2x天人之勇\x1b[0x\x1b[1x曹子孝\x1b[0x呀!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("现在正是以此身化矛之时!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("就以此身为盾!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("以守为攻伺机而动!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_허저) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("\x1b[1x虎痴\x1b[0x就是俺!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("俺要碾过来了."), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("俺要狠狠惩罚你!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("俺可要把你打飞了!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_전위) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("吾乃恶来\x1b[1x典韦\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("干掉你!!!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("尝尝我这重击!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("这可是很痛的哦!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_하후돈) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("我乃独眼\x1b[1x夏侯惇\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("我可不会手下留情!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("抹杀你!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("把你轰到天边去!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_하후연) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("吾乃白地将军\x1b[1x夏侯渊\x1b[0!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("吾虎步关右,所向无前!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("急速行军,一蹴而就!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("兵贵神速,吾当连夜突袭!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_서황) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(5))
						{
						case 0: pk::say(pk::encode("看我\x1b[1x徐公明\x1b[0x长驱直入!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("我斧头的滋味,\n请好好品尝下!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("截其源,断其粮,贼可擒也!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("吾等可用釜底抽薪之计!"), pk::get_person(src.leader)); break;
						case 4: pk::say(pk::encode("我不打无胜算的战!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_장합) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(5))
						{
						case 0: pk::say(pk::encode("我乃是河间\x1b[1x张邰\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("兵无常势,水无常形."), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("舍身为国,虽死无憾!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("行军打战,需得随机应变!"), pk::get_person(src.leader)); break;
						case 4: pk::say(pk::encode("给他们当头一棒!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_장료) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(5))
						{
						case 0: pk::say(pk::encode("\x1b[1x张文远\x1b[0x在此!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("以武入道,汝可受此一击!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("武之极意,尽在于此!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("马似飞影,枪如霹雳!"), pk::get_person(src.leader)); break;
						case 4: pk::say(pk::encode("辽...来...来..."), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_방덕) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("吾乃\x1b[1x庞令明\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("休想全身而退!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("士之道,唯有克敌前行而已!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("驰骋沙场,杀敌无惧!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_손견) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("我乃是\x1b[2x江东猛虎\x1b[0x \x1b[1x孙坚\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("大业我来铸造!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("接下猛虎之怒!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("宝剑出鞘,踏平贼营!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_손책) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("\x1b[17x小霸王\x1b[0x \x1b[1x孙策\x1b[0x在此!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("属于我们的时代开始了!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("江东子弟何惧于天下?!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("大丈夫行于世,自当志在天下四方!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_손권) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("吾乃江东\x1b[1x孙权\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("不急,吾等必一击制敌!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("待敌动而后发制人!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("父兄的大业由我来守护!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_태사자) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("东莱\x1b[1x太史慈\x1b[0x在此!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("只要有我在,尔等就休想越国境一步!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("驰骋疆场,快意恩仇!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("堂堂正正的一决胜负!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_감녕) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("\x1b[17x锦帆贼\x1b[0x \x1b[1x甘宁在此!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("本大爷要大闹一场!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("本大爷出马,战场可是要沸腾起来了!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("这里是我们的地盘!"), pk::get_person(src.leader)); break;
						}
					}

					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_주태) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("我..是..\x1b[1x周..泰\x1b[0x!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("满身伤痕,皆为吾主而留!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("历战之躯,岂能倒下!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("勇者岂惧皮肉之伤!"), pk::get_person(src.leader)); break;
						}
					}
					/*
						else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_정보) and pk::is_valid_unit_status(部队状态_通常))
						{
							switch (pk::rand(4))
							{
							case 0: pk::say(pk::encode("吉荻 纸矿 \x1b[1x单歧\x1b[0x俱!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("仪待 澶伝诸册待 资泰 茎祸喉!"), pk::get_person(src.leader)); break;
							case 2: pk::say(pk::encode("醍店泰喧打 周昙喉!"), pk::get_person(src.leader)); break;
							case 3: pk::say(pk::encode("册烦文征芽!"), pk::get_person(src.leader)); break;
							}
						}
					*/
					else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_황개) and pk::is_valid_unit_status(部队状态_通常))
					{
						switch (pk::rand(4))
						{
						case 0: pk::say(pk::encode("\x1b[1x黄盖\x1b[0x在此!"), pk::get_person(src.leader)); break;
						case 1: pk::say(pk::encode("为成大义,死不足惜!"), pk::get_person(src.leader)); break;
						case 2: pk::say(pk::encode("且用这满天的火焰,回报主公一家的恩情!"), pk::get_person(src.leader)); break;
						case 3: pk::say(pk::encode("赴汤蹈火,在所不辞!"), pk::get_person(src.leader)); break;
						}
					}
					/*
						else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_원소) and pk::is_valid_unit_status(部队状态_通常))
						{
							switch (pk::rand(4))
							{
							case 0: pk::say(pk::encode("吉荻 诸症除皆待 变粗 \x1b[1x则犄\x1b[0x俱!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("单晃泰专 哉矿 塔恢待 辙晃泰喉\n敷 织 黛张!"), pk::get_person(src.leader)); break;
							case 2: pk::say(pk::encode("醍店泰喧打 周昙喉!"), pk::get_person(src.leader)); break;
							case 3: pk::say(pk::encode("府尺坦否仅 殿府些 抚扶 兎花冼腿准!"), pk::get_person(src.leader)); break;
							}
						}

						else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_안량) and pk::is_valid_unit_status(部队状态_通常))
						{
							switch (pk::rand(4))
							{
							case 0: pk::say(pk::encode("吉荻 纸矿 则犄廓待 帅撂挫摘\n\x1b[1x责校\x1b[0x泰俱!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("单策刃勺矿仅 张张 迹仅俱!"), pk::get_person(src.leader)); break;
							case 2: pk::say(pk::encode("醍店泰喧打 周昙喉!"), pk::get_person(src.leader)); break;
							case 3: pk::say(pk::encode("彦塑喝搞烦!"), pk::get_person(src.leader)); break;
							}
						}

						else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_문추) and pk::is_valid_unit_status(部队状态_通常))
						{
							switch (pk::rand(4))
							{
							case 0: pk::say(pk::encode("吉荻 纸矿 则犄廓待 帅撂挫摘\n\x1b[1x算东\x1b[0x俱!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("仪些 匠遇敷 大仅 诠仅荻!"), pk::get_person(src.leader)); break;
							case 2: pk::say(pk::encode("醍店泰喧打 周昙喉!"), pk::get_person(src.leader)); break;
							case 3: pk::say(pk::encode("桥愤谁东女 订踢砍!"), pk::get_person(src.leader)); break;
							}
						}

						else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_원술) and pk::is_valid_unit_status(部队状态_通常))
						{
							switch (pk::rand(3))
							{
							case 0: pk::say(pk::encode("朝打 平坦俱!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("颜蚕柁 俱鼠!"), pk::get_person(src.leader)); break;
							case 2: pk::say(pk::encode("醍店泰喧打 周昙喉!"), pk::get_person(src.leader)); break;
							}
						}

						else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_공손찬) and pk::is_valid_unit_status(部队状态_通常))
						{
							switch (pk::rand(4))
							{
							case 0: pk::say(pk::encode("吉荻 纸矿 事挫择诸 \x1b[1x皆钟敌\x1b[0x泰俱!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("泰斯团 泊而打 仪待 茶伴砍泰张."), pk::get_person(src.leader)); break;
							case 2: pk::say(pk::encode("醍店泰喧打 周昙喉!"), pk::get_person(src.leader)); break;
							case 3: pk::say(pk::encode("府尺待 征单扶 茶胎打 泰札泰俱!"), pk::get_person(src.leader)); break;
							}
						}

						else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_마등) and pk::is_valid_unit_status(部队状态_通常))
						{
							switch (pk::rand(3))
							{
							case 0: pk::say(pk::encode("吉荻 纸矿 升校待 \x1b[1x挫宪\x1b[0x泰俱!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("泉茶质待 卒遭柁 歧缮茶张."), pk::get_person(src.leader)); break;
							case 2: pk::say(pk::encode("醍店泰喧打 周昙喉!"), pk::get_person(src.leader)); break;
							}
						}

						else if (pk::is_in_screen(pos) and pk::has_leader(src, 무장_마운) and pk::is_valid_unit_status(部队状态_通常)) // 마운록
						{
							switch (pk::rand(4))
							{
							case 0: pk::say(pk::encode("吉荻 纸矿 \x1b[1x挫骚殓\x1b[0x泰俱!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("遍, 恢第 割单径胎荻!"), pk::get_person(src.leader)); break;
							case 2: pk::say(pk::encode("醍店泰喧打 周昙喉!"), pk::get_person(src.leader)); break;
							case 3: pk::say(pk::encode("赜 鼠喉玺芹 萨张 迹冼柁\n歧缮茶牙俱!"), pk::get_person(src.leader)); break;
							}
						}



						// Gold Edition Patch (신무장 대사 예시), 신무장들의 NUMBER를 입력하시면 됩니다. (EX: 천시로 - 852)


						// 이명
						else if (pk::is_in_screen(pos) and pk::has_leader(src, 850) and pk::is_valid_unit_status(部队状态_通常))
						{
							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("菜织勺遇待 卒遭柁 歧缮茶挫!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("缮大喉恢 睬人过 歧张挫喉!"), pk::get_person(src.leader)); break;
							}
						}

						// 천시로
						else if (pk::is_in_screen(pos) and pk::has_leader(src, 852) and pk::is_valid_unit_status(部队状态_通常))
						{
							switch (pk::rand(3))
							{
							case 0: pk::say(pk::encode("泰华泰 骚涂庙寻食待 周砍俱!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("仪待 曾涂些 屋烦敷 织 黛柁鸭?"), pk::get_person(src.leader)); break;
							case 2: pk::say(pk::encode("吉荻 纸矿 \x1b[1x遭待 绩泰\x1b[0x俱!"), pk::get_person(src.leader)); break;
							}
						}

						// 마수미
						else if (pk::is_in_screen(pos) and pk::has_leader(src, 853) and pk::is_valid_unit_status(部队状态_通常))
						{
							switch (pk::rand(3))
							{
							case 0: pk::say(pk::encode("泰华泰 骚涂庙寻食待 周砍俱!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("仪待 曾涂些 屋烦敷 织 黛柁鸭?"), pk::get_person(src.leader)); break;
							case 2: pk::say(pk::encode("吉荻 纸矿 \x1b[1x遭待 绩泰\x1b[0x俱!"), pk::get_person(src.leader)); break;
							}
						}
					*/

				} // if 

			} // for		

		} // leader_lines

	} // class Main

	Main main;

} // namespace
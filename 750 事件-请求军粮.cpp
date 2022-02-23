// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/02/16 # 江东新风 # 语句优化 ##
// ## 2021/01/23 # 江东新风 # 语句优化，为请求兵粮编写了后续剧情 ##
// ## 2020/12/17 # 江东新风 # 语句优化 ##
// ## 2020/09/23 # 江东新风 # 增加请求军粮最低阈值自定义，调高最低阈值为50000 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/09/01 # 江东新风 # 修复技巧增加量???不符的bug ##
// ## 2020/08/03 # 江东新风 # 修复給粮数量固定bug ##
// ## 2020/07/30 # 江东新风 # 給請求军粮设置了上限，设定了請求军粮获得的技巧点数量比例 ##
// @@ 2019/02/21 @ Honeybee @ @@
namespace 군량탄원
{
	const int 发生率_通常 = 10;//非蝗災下，5%几率发生
	const int 发生率_蝗災 = 40;//蝗災下40%几率发生
	const int 请求军粮最低阈值 = 50000;//城市兵粮低于该阈值时，不发生请求军粮事件
	const int 索求上限 = 75000;
	const int 兵粮技巧比 = 125;//多少兵粮获得1技巧点	
	class Main
	{

		Main()
		{
			pk::bind(112, pk::trigger112_t(callback));
		}

		pk::force@ force_;
		pk::person@ kunshu_;
		pk::city@ city_;

		void callback(pk::force@ force)
		{
			if (force.is_player())
			{
				@force_ = force;
				@kunshu_ = pk::get_person(force.kunshu);
				@city_ = pk::building_to_city(pk::get_building(kunshu_.service));
				forceinfo@ forcei0 = @force_ex[force.get_id()];
				//pk::trace("pos0");
				if (forcei0.disaster_relief_time > 0 and forcei0.disaster_relief_time != 255)
				{
					//pk::trace("pos1");
					forcei0.disaster_relief_time -= 1;
					forcei0.update(force.get_force_id());
					if (forcei0.disaster_relief_time == 0) pk::scene(pk::scene_t(scene_灾民报恩));
				}
				else
				{
					//pk::trace("pos2");
					if (city_ is null) return;
					if (city_.food < 请求军粮最低阈值) return;
					//pk::trace("pos3");
					//if (pk::rand(100) > 1 + (city_.inago ? pk::rand(2) : 0)) return;
					if (city_.inago)
					{
						if (pk::rand_bool(发生率_蝗災))
							pk::scene(pk::scene_t(scene_请求军粮));
					};
					if (!city_.inago)
					{
						//pk::trace("pos4");
						if (pk::rand_bool(发生率_通常))
							pk::scene(pk::scene_t(scene_请求军粮));
					}
				}

			}
		}

		void scene_请求军粮()
		{
			pk::move_screen(city_.get_pos());
			pk::fade(0);
			pk::sleep();
			pk::background(2);
			pk::fade(255);

			string kunshu_name = pk::decode(pk::get_name(kunshu_));
			pk::person@ farmer = pk::get_person(무장_농민);


			switch (pk::rand(3))
			{
			case 0: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，请可怜可怜我们吧，我们快饿死了。", kunshu_name)), farmer); break;
			case 1: pk::message_box(pk::encode(pk::format("无论什么，只要能填饱肚子就好... \x1b[1x{}\x1b[0x大人请救救我们吧。", kunshu_name)), farmer); break;
			case 2: pk::message_box(pk::encode("拜托了！救救我的家人吧！他们快饿死了..."), farmer); break;
			}
			array<int> food;
			if ((city_.food / 3) < 索求上限)
			{
				food =
				{
					0,
					city_.food / 5,
					city_.food / 4,
					city_.food / 3
				};
			}
			else if ((city_.food / 3) >= 索求上限)
			{
				food =
				{
					0,
					索求上限 * 3 / 5,
					索求上限 * 3 / 4,
					索求上限
				};
			}

			array<string> answers =
			{
				pk::encode("不救济"),
				pk::encode(pk::format("给予{}粮食", food[1])),
				pk::encode(pk::format("给予{}粮食", food[2])),
				pk::encode(pk::format("给予{}粮食", food[3])),
			};

			int n = pk::choose(pk::encode("是否施舍些粮食给灾民?"), answers);
			int tp;
			forceinfo@ force_t = @force_ex[force_.get_id()];
			int8 good_gain;
			switch (n)
			{
			case 0:
				switch (pk::rand(2))
				{
				case 0: pk::message_box(pk::encode("请原谅我们也没有多余的粮食了。"), kunshu_); break;
				case 1: pk::message_box(pk::encode("我看你们脸色还行呀, 肯定不是灾民吧。"), kunshu_); break;
				}
				switch (pk::rand(3))
				{
				case 0: pk::message_box(pk::encode("...唉，就这样看着我们饿死，大人们都是铁石心肠呀..."), farmer); break;
				case 1: pk::message_box(pk::encode("... 唉，这样的世道..."), farmer); break;
				case 2: pk::message_box(pk::encode(pk::format("...看来真的没法活了...", kunshu_name)), farmer); break;
				}
				
				if (force_t.good_evil > -123) force_t.good_evil -= 5;
				else force_t.good_evil = -128;
				break;

			case 1:
			case 2:
			case 3:
				switch (pk::rand(2))
				{
				case 0: pk::message_box(pk::encode(pk::format("可不能让我的子民受苦，快抓紧准备{}粮食。", food[n])), kunshu_); break;
				case 1: pk::message_box(pk::encode(pk::format("真的太可怜。调拨{}粮食给父老乡亲们。", food[n])), kunshu_); break;
				}
				switch (pk::rand(3))
				{
				case 0: pk::message_box(pk::encode(pk::format("非常感谢.我不会绝不会忘记\x1b[1x{}大人\x1b[0x的这分恩情的。来日必将报答！", kunshu_name)), farmer); break;
				case 1: pk::message_box(pk::encode(pk::format("感谢！\x1b[1x{}大人\x1b[0x你是我们的救命恩人。此恩必报！", kunshu_name)), farmer); break;
				case 2: pk::message_box(pk::encode(pk::format("非常感谢！来日必将结草衔环报答\x1b[1x{}大人\x1b[0x的恩德。", kunshu_name)), farmer); break;
				}
				//tp = (pk::rand(n) + 1) * 100;
				tp = food[n] / 兵粮技巧比 - 20 + pk::rand(40);
				pk::message_box(pk::encode(pk::format("技巧点加了\x1b[1x{}\x1b[0x.", tp)));
				ch::add_tp(force_, tp, city_.get_pos());
				pk::add_food(city_, -food[n], true);
				
				if (n == 1) good_gain = 2;
				else if (n == 2) good_gain = 3;
				else good_gain = 4;
				force_t.good_evil = pk::min(127, force_t.good_evil + good_gain);
				//随机设定农民的回报时间
				if (pk::rand_bool(80)) force_t.disaster_relief_time = ch::randint(6, 12);
				break;
			}


			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		}
		void scene_灾民报恩()
		{
			pk::building@ building0 = pk::get_building(kunshu_.service);
			pk::move_screen(kunshu_.get_pos());
			pk::fade(0);
			pk::sleep();
			pk::background(场景_宫廷1);
			pk::fade(255);
			pk::cutin(CG_脚步);
			string kunshu_name = pk::decode(pk::get_name(kunshu_));
			pk::person@ farmer = pk::get_person(武将_农民);
			pk::message_box(pk::encode(pk::format("终于找到您了，\x1b[1x{}大人\x1b[0x。\n还记得我们嘛？", kunshu_name)), farmer);
			pk::message_box(pk::encode(pk::format("你是?....", kunshu_name)), kunshu_); 
			pk::message_box(pk::encode(pk::format("恩人，你忘了?几个月前您曾给了濒临饿死的我们一些粮食。", kunshu_name)), farmer);
			pk::message_box(pk::encode(pk::format("原来是你们呀~看你们现在气色不错，可是度过难关了？", kunshu_name)), kunshu_);
			pk::message_box(pk::encode(pk::format("是呀，真的多亏了您，否则我们现在早就饿死了吧。\n\n这次来主要就是为了答谢大人您的恩情。我们愿加入\x1b[1x{}大人\x1b[0x的队伍,为您效死！", kunshu_name)), farmer);
			pk::cutin(CG_眼神);
			pk::message_box(pk::encode(pk::format("好好好~我一定会终结这乱世，还大家一个太平盛世！", kunshu_name)), kunshu_);
									
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);

			pk::cutin(CG_欢呼);
			ch::add_troops(building0, ch::randint(1, 3) * 500 + 1000, true);
		}
	}

	Main main;
}
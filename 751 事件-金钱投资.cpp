// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/01/23 # 江东新风 # 语句优化，为请求资金编写了后续剧情 ##
// ## 2020/09/01 # 江东新风 # 修复技巧增加量???不符的bug ##
// ## 2020/08/03 # 江东新风 # 修复投資金額固定bug ##
// ## 2020/07/30 # 江东新风 # 給投資索求设置了上限，设定了投資金錢获得的技巧点数量比例 ##
// @@2019/02/21 @ Honeybee @ @@
namespace 금투자
{
	const int 发生率 = 10;//10%几率发生
	const int 索求上限 = 15000;
	//const int 投资技巧收益 = 30;//进行投资初期的技巧收益

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
				forceinfo@ force_t = @force_ex[force.get_id()];
				if (force_t.investment_time > 0)
				{
					force_t.investment_time -= 1;
					if (force_t.investment_time == 0)
					{
						if (pk::rand_bool(70)) pk::scene(pk::scene_t(scene_投资成功));
						else pk::scene(pk::scene_t(scene_投资失败));
					}
				}
				else
				{
					if (city_ is null) return;
					if (city_.gold < 15000) return;

					if (pk::rand_bool(发生率)) pk::scene(pk::scene_t(scene_金钱投资));						
				}

			}
		}

		void scene_金钱投资()
		{
			pk::move_screen(city_.get_pos());
			pk::fade(0);
			pk::sleep();
			pk::background(2);
			pk::fade(255);

			string kunshu_name = pk::decode(pk::get_name(kunshu_));
			pk::person@ merchant = pk::get_person(무장_상인);


			switch (pk::rand(2))
			{
			case 0: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我生意正在起步阶段，急需投资，您能否赞助我一些资金？", kunshu_name)), merchant); break;
			case 1: pk::message_box(pk::encode(pk::format("好机会来了，我发现一件非常赚钱的买卖，\x1b[1x{}\x1b[0x大人要不要参与投资下?\n如果赚到钱，我会给您分成的。", kunshu_name)), merchant); break;
			}
			pk::message_box(pk::encode(pk::format("生意的具体事项请容我细细禀报。", kunshu_name)), merchant);
			pk::cutin(CG_密告);
			array<int> gold;
			if ((city_.gold / 3) < 索求上限)
			{
				gold =
				{
					0,
					city_.gold / 5,
					city_.gold / 4,
					city_.gold / 3
				};
			}
			else if ((city_.gold / 3) >= 索求上限)
			{
				gold =
				{
					0,
					索求上限 * 3 / 5,
					索求上限 * 3 / 4,
					索求上限
				};
			}

			array<string> answers =
			{
				pk::encode("不投资."),
				pk::encode(pk::format("投资{}资金.", gold[1])),
				pk::encode(pk::format("投资{}资金.", gold[2])),
				pk::encode(pk::format("投资{}资金.", gold[3])),
			};

			int n = pk::choose(pk::encode("是否相信商人进行投资?"), answers);
			//int tp;
			forceinfo@ forcei = @force_ex[force_.get_id()];
			switch (n)
			{
			case 0:
				switch (pk::rand(2))
				{
				case 0: pk::message_box(pk::encode("请原谅我现在没有时间。"), kunshu_); break;
				case 1: pk::message_box(pk::encode("我军资金短缺，实在没这个闲钱。"), kunshu_); break;
				}
				switch (pk::rand(2))
				{
				case 0: pk::message_box(pk::encode("唉,那真是太可惜了..."), merchant); break;
				case 1: pk::message_box(pk::encode("那以后还有机会的时候再来找大人吧。"), merchant); break;
				}
				//ch::add_tp(force_, -投资技巧收益, city_.get_pos());
				break;

			case 1:
			case 2:
			case 3:
				switch (pk::rand(2))
				{
				case 0: pk::message_box(pk::encode(pk::format("嗯，听起来你这个生意貌似不错，我准备投资{}金。", gold[n])), kunshu_); break;
				case 1: pk::message_box(pk::encode(pk::format("正好我手里有笔闲钱，就投资你{}金吧。", gold[n])), kunshu_); break;
				}
				switch (pk::rand(2))
				{
				case 0: pk::message_box(pk::encode("十分感谢，等生意有了起色，我会来找您的。"), merchant); break;
				case 1: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人真是眼光独到呀，我不会让您失望的。", kunshu_name)), merchant); break;
				}
				//tp = gold[n] / 金錢技巧比 - 10 + pk::rand(20);
				//pk::message_box(pk::encode(pk::format("技巧点加了\x1b[1x{}\x1b[0x.", tp)));
				//ch::add_tp(force_, 投资技巧收益, city_.get_pos());
				pk::add_gold(city_, -gold[n], true);
				forcei.good_evil = pk::min(127, forcei.good_evil + 1);
				//随机设定商人的投资的回报时间
				//有概率商人不回来
				if (pk::rand_bool(90))
				{
					forcei.investment_time = ch::randint(8, 16);
					forcei.investment_gold = gold[n];
				}
				forcei.update(force_.get_id());
				break;
			}

			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		}
		void scene_投资成功()
		{
			pk::building@ building0 = pk::get_building(kunshu_.service);
			pk::move_screen(kunshu_.get_pos());
			pk::fade(0);
			pk::sleep();
			pk::background(场景_市场);
			pk::fade(255);
			pk::cutin(CG_脚步);
			string kunshu_name = pk::decode(pk::get_name(kunshu_));
			pk::person@ merchant = pk::get_person(武将_商人);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}大人\x1b[0x。\n还记得我嘛？", kunshu_name)), merchant);
			pk::message_box(pk::encode(pk::format("你是?....", kunshu_name)), kunshu_);
			pk::message_box(pk::encode(pk::format("你忘了?几个月前我曾请求您对我的生意进行投资。", kunshu_name)), merchant);
			pk::message_box(pk::encode(pk::format("原来是你呀~看你现在红光满面，看来生意做得不错呀？", kunshu_name)), kunshu_);
			pk::message_box(pk::encode(pk::format("是呀，真的多亏了您的投资，我才能取得现在的成就。\n\n这次来是为了将投资的收益交还大人。", kunshu_name)), merchant);

			pk::message_box(pk::encode(pk::format("哈哈哈~太客气了，你能有现在的成就我就很开心了！", kunshu_name)), kunshu_);

			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
			forceinfo@ force0 = @force_ex[force_.get_id()];
			int gold_gain = pk::min(int(force0.investment_gold * ch::randfloat(1.1, 1.75)),pk::get_max_gold(building0) - pk::get_gold(building0));
			force0.investment_gold = 0;
			force0.update(force_.get_id());
			pk::message_box(pk::encode(pk::format("商人将投资收益交给我军，{}的金上涨了{}", pk::decode(pk::get_name(building0)), gold_gain)));
			pk::add_gold(building0, gold_gain, true);
		}
		void scene_投资失败()
		{
			pk::building@ building0 = pk::get_building(kunshu_.service);
			pk::move_screen(kunshu_.get_pos());
			pk::fade(0);
			pk::sleep();
			pk::background(场景_市场);
			pk::fade(255);
			pk::cutin(CG_脚步);
			string kunshu_name = pk::decode(pk::get_name(kunshu_));
			pk::person@ merchant = pk::get_person(武将_商人);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}大人\x1b[0x。\n还记得我嘛？", kunshu_name)), merchant);
			pk::message_box(pk::encode(pk::format("你是当初找我投资的商人嘛?", kunshu_name)), kunshu_);
			pk::message_box(pk::encode(pk::format("是呀，多谢您当时对我们生意的投资。", kunshu_name)), merchant);
			pk::message_box(pk::encode(pk::format("看你貌似比之前见到时憔悴了不少，生意不是很顺利嘛？", kunshu_name)), kunshu_);
			pk::message_box(pk::encode(pk::format("是啊，乱世之下，生意难做呀...\n大人您上次投资的钱我们已经亏损了不少，为了防止继续亏损，只能放弃了。\n此处前来是将剩余的钱交换给{}大人，抱歉。", kunshu_name)), merchant);

			pk::message_box(pk::encode(pk::format("哈哈哈~太客气了，你能有现在的成就我就很开心了！", kunshu_name)), kunshu_);

			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);

			forceinfo@ force0 = @force_ex[force_.get_id()];
			int gold_gain = pk::min(int(force0.investment_gold * ch::randfloat(0.4, 0.7)), pk::get_max_gold(building0) - pk::get_gold(building0));
			force0.investment_gold = 0;
			force0.update(force_.get_id());

			pk::message_box(pk::encode(pk::format("商人将投资剩余的钱交给我军，{}的金上涨了{}", pk::decode(pk::get_name(building0)), gold_gain)));
			pk::add_gold(building0, gold_gain, true);
		}
	}

	Main main;
}
// ## 2022/03/18 # 江东新风 # 狗头军师舌战闪退2 ##
// ## 2022/03/17 # 江东新风 # 狗头军师舌战闪退 ##
// ## 2022/02/17 # 江东新风 # 探索结果多样化 ##

namespace SEARCH_RESULT
{
	//---------------------------------------------------------------------------
	//马匹列表
	array<string> horse1_name = { "红鬃马", "汗血宝马", "灰影马", "黄骠马", "白战马", "骅骝" };
	array<string> horse2_name = { "白龙", "赤兔", "翠龙", "大宛马", "冀州战马",
									"匈奴马", "的卢", "黑月", "虎豹甲骑", "黑豹",
									"花鬃马", "绝影", "匈奴白马", "东胡战马", "青聪马",
									"踏雪乌骓", "乌骓", "五花马", "西凉甲骑", "西羌甲骑",
									"西域甲骑", "陷阵甲骑", "夜照玉狮子", "银甲战马", "雨南马",
									"玉秋", "爪黄飞电", "紫骝马", "骁风赤兔" };
	//武器列表
	array<string> weapon1_name = { "长刀", "短刀", "红缨枪" };
	array<string> weapon2_name = { "湛金枪", "草稚枪", "丹凤刀", "长刀", "战斧",
									"短刀", "方天画戟", "凤嘴刀", "红缨枪", "钩廉枪",
									"古锭刀", "火龙枪", "霍去病枪", "金马枪", "锯齿刀",
									"龙骑枪", "龙牙刀", "落日弓", "劈浪枪", "蜻蛉切",
									"三尖两刃刀", "双铁戟", "钢鞭", "铁棍", "铁脊蛇矛",
									"铁胎弓", "玄铁大刀", "无双方天戟", "向阳戟", "涯角枪",
									"子午烈火枪", "子午枪" };
	//---------------------------------------------------------------------------

	class Main
	{
		pk::person@ skin_actor_;
		pk::person@ scen_actor_;
		string skin_name_;
		string bg_dir_;
		int duel_result_;
		Main()
		{
			pk::set_func(65, pk::func65_t(callback));
		}

		bool callback(pk::person@ actor, pk::building@ building)
		{
			if (actor is null) return false;
			if (building is null) return false;
			/*
			20√失败
			13√讨伐盗贼--可单挑，增加治安，武力经验
			1√发现装备皮肤
			旅人汇报其他城市信息---迷雾状态下短暂获取敌方城市信息
			1√发现华佗--治愈武将
			15√发现义勇兵--增加兵力
			24√发现金--获得金钱
			10√村民赠金--获得金钱
			官员不法调查--据说在这个都市中有一些官员徇私枉法，增加治安
			10√丰收庆祝---季节限定？
			2√安置流民---增加人口
			2√投资商人--消耗金钱，增加商业基础值
			2√兴修水利--消耗金钱，增加农业基础值
			增加治安，商业值
			*/

			int result = pk::rand(100);
			if (result < 20) return false;
			else if (result < 44)//24√发现金
			{
				return find_gold(actor, building);
			}
			else if (result < 59)//15√发现义勇兵
			{
				return find_minutemen(actor, building);
			}
			else if (result < 72)//13√讨伐盗贼--可单挑，增加治安，武力经验
			{
				return find_bandit(actor, building);
			}
			else if (result < 82)//10√村民赠金--获得金钱
			{
				return find_gold2(actor, building);
			}
			else if (result < 92)//10√丰收庆祝---季节限定？
			{
				return find_food(actor, building);
			}
			else if (result < 94)//2√安置流民---增加人口
			{
				return find_refugee(actor, building);
			}
			else if (result < 96)//2√投资商人--消耗金钱，增加商业基础值
			{
				return find_investment(actor, building);
			}
			else if (result < 98)//2√兴修水利--消耗金钱，增加农业基础值
			{
				return find_harvest(actor, building);
			}
			else if (result < 99)//1√发现装备皮肤
			{
				return find_doctor(actor, building);
			}
			else if (result < 100)//1√发现华佗--治愈武将
			{
				return find_food(actor, building);
			}
			return true;
		}

		bool find_refugee(pk::person@ actor, pk::building@ building)
		{
			int pop_num = 3000 + pk::rand(2001);
			int gold_cost = pop_num / 10;
			int food_cost = gold_cost * 6;
			if (pk::get_gold(building) < gold_cost) return false;
			if (pk::get_food(building) < food_cost) return false;
			int base_id = building.get_id();

			int confirm = 0;
			if (player_check(actor))
			{
				pk::person@kunshu = pk::get_person(pk::get_kunshu_id(actor));
				confirm = pk::choose({ pk::encode(" 是 "),pk::encode(" 否 ") }, ch::get_call(actor, kunshu) + pk::encode(pk::format("，在探索过程中发现了流离失所的难民，是否要花费\x1b[1x{}\x1b[0x金，\x1b[2x{}\x1b[0x粮进行安置。",gold_cost,food_cost)), actor);

				if (confirm == 0)
				{
					//发现金的音效
					pk::play_se(6);
					string msg = pk::encode(pk::format("\x1b[2x{}\x1b[0x的人口增加\x1b[1x{}\x1b[0x,", pk::decode(pk::get_name(building)), pop_num));
					pk::history_log(actor.get_pos(), pk::get_force(actor.get_force_id()).color, msg);
				}

			}
			if (confirm == 0)
			{
				pk::add_gold(building, -gold_cost, true);
				pk::add_food(building, -food_cost, true);
				ch::add_population(base_id, pop_num);
			}
			
			after_search(actor, building);
			return true;
		}

		bool find_investment(pk::person@ actor, pk::building@ building)
		{
			int gold_cost = 300;
			if (pk::get_gold(building) < gold_cost) return false;
			int base_id = building.get_id();
			uint16 max = (base_id < 城市_末) ? 300 : 150;
			if (base_ex[base_id].revenue_bonus >= max) return false;
			int revenue_gain = 10;
			bool confirm = true;
			if (player_check(actor))
			{
				pk::message_box(pk::encode("虽然人心已经安定下来了，但街上还是不太有活力呀。"), actor);
				confirm = pk::yes_no(pk::encode(pk::format("要投资\x1b[2x{}金\x1b[0x发展商业嘛?", gold_cost)));
				if (confirm)
				{
					//发现金的音效
					pk::play_se(6);
					string msg = pk::encode(pk::format("\x1b[2x{}\x1b[0x的基础金收入增加\x1b[1x{}\x1b[0x,", pk::decode(pk::get_name(actor)), revenue_gain));
					pk::history_log(actor.get_pos(), pk::get_force(actor.get_force_id()).color, msg);
				}

			}
			if (confirm)
			{
				pk::add_gold(building, -gold_cost, true);
				ch::add_revenue_bonus(base_id, revenue_gain);
			}
			after_search(actor, building);
			return true;
		}

		bool find_harvest(pk::person@ actor, pk::building@ building)
		{
			int gold_cost = 300;
			if (pk::get_gold(building) < gold_cost) return false;
			int base_id = building.get_id();
			uint16 max = (base_id < 城市_末) ? 2500 : 1250;
			if (base_ex[base_id].harvest_bonus >= max) return false;
			int harvest_gain = 50;
			bool confirm = true;
			if (player_check(actor))
			{
				pk::message_box(pk::encode("水患如此严重，农民深受其害呀。"), actor);
				confirm = pk::yes_no(pk::encode(pk::format("要投资\x1b[2x{}金\x1b[0x兴修水利嘛?",gold_cost)));
				if (confirm)
				{
					//发现金的音效
					pk::play_se(6);
					string msg = pk::encode(pk::format("\x1b[2x{}\x1b[0x的基础粮收入增加\x1b[1x{}\x1b[0x,", pk::decode(pk::get_name(actor)), harvest_gain));
					pk::history_log(actor.get_pos(), pk::get_force(actor.get_force_id()).color, msg);
				}

			}
			if (confirm)
			{
				pk::add_gold(building, -gold_cost, true);
				ch::add_harvest_bonus(base_id, harvest_gain);
			}
			
			after_search(actor, building);
			return true;
		}

		bool find_skin(pk::person@ actor, pk::building@ building)
		{
			//发现皮肤，属于顶级探索效果
			bool is_horse = pk::rand_bool(45);

			int index = -1;
			if (is_horse) index = pk::rand(horse2_name.length());
			else index = pk::rand(weapon2_name.length());
			
			if (player_check(actor))//玩家控制时有专属剧情，ai就仅获得效果
			{
				@skin_actor_ = @actor;
				skin_name_ = "\x1b[1x" + (is_horse ? horse2_name[index] : weapon2_name[index]) + "\x1b[0x";
				bg_dir_ = "media/background/" + (is_horse ? "horse/" : "weapon/") + index + ".png";
				pk::scene(pk::scene_t(scene_skin));

			}

			if (is_horse)
			{
				skin_actor_.horse = 255;
				ch::set_person_horse(skin_actor_.get_id(), index);
			}
			else
			{
				skin_actor_.left_weapon = 255;
				skin_actor_.right_weapon = 255;
				ch::set_person_weapon(skin_actor_.get_id(), index);
			}
			after_search(actor, building);
			return true;
		}

		bool find_bandit(pk::person@ actor, pk::building@ building)
		{
			//发现山贼
			int porder_gain = ch::randint(2,4);
			bool win = false;
			bool buy = false;
			if (player_check(actor))//玩家执行的情况
			{
				pk::message_box(pk::encode("在连接相邻都市的山路上据说有山贼出没。"), pk::get_person(武将_旅人));
				bool confirm = pk::yes_no(pk::encode("要亲自出马消灭贼寇吗?"));
				int choise = pk::choose({ pk::encode("亲自讨伐"),pk::encode("舌战怀柔"),pk::encode("金钱招安"),pk::encode("置之不理") }, pk::encode("该如何处置这伙山贼呢？"), actor);
				string msg = "";
				if (choise == 0)
				{

					@scen_actor_ = @actor;
					pk::scene(pk::scene_t(scene_duel));
					int duel_result = duel_result_;
					if (duel_result == 0)
					{
						pk::message_box(pk::encode("有我在，必不会让贼人继续横行。"), actor);
						win = true;
						msg = pk::encode(pk::format("\x1b[1x{}\x1b[0x在\x1b[2x{}\x1b[0x讨伐贼兵,治安上升了。", pk::decode(pk::get_name(actor)), pk::decode(pk::get_name(building))));
						pk::history_log(actor.get_pos(), pk::get_force(actor.get_force_id()).color, msg);
					}
					else
					{
						pk::message_box(pk::encode("可恶，形式不利，只能先撤了。"), actor);
						win = false;
						msg = pk::encode(pk::format("\x1b[1x{}\x1b[0x在\x1b[2x{}\x1b[0x讨伐贼兵失败，治安下降了。", pk::decode(pk::get_name(actor)), pk::decode(pk::get_name(building))));
						pk::history_log(actor.get_pos(), pk::get_force(actor.get_force_id()).color, msg);
					}

				}
				else if (choise == 1)//闪退632916
				{
					@scen_actor_ = @actor;
					pk::scene(pk::scene_t(scene_debate));
					int result = duel_result_;
					if (result == 0) 
					{
						pk::message_box(pk::encode("如此以来，山贼应该不会再作乱了。"), actor);
						win = true;
						msg = pk::encode(pk::format("\x1b[1x{}\x1b[0x在\x1b[2x{}\x1b[0x怀柔贼兵,治安上升了。", pk::decode(pk::get_name(actor)), pk::decode(pk::get_name(building))));
						pk::history_log(actor.get_pos(), pk::get_force(actor.get_force_id()).color, msg);
					}
					else
					{
						pk::message_box(pk::encode("可恶，竟然如此难以沟通。"), actor);
						win = false;
						msg = pk::encode(pk::format("\x1b[1x{}\x1b[0x在\x1b[2x{}\x1b[0x怀柔贼兵失败，治安下降了。", pk::decode(pk::get_name(actor)), pk::decode(pk::get_name(building))));
						pk::history_log(actor.get_pos(), pk::get_force(actor.get_force_id()).color, msg);
					}
				}
				else if (choise == 2)
				{
					pk::message_box(pk::encode("收到这些钱，山贼估计能消停点了。"), actor);
					win = false;
					msg = pk::encode(pk::format("\x1b[1x{}\x1b[0x在\x1b[2x{}\x1b[0x怀柔贼兵，治安上升了。", pk::decode(pk::get_name(actor)), pk::decode(pk::get_name(building))));

					buy = true;
				}
			}
			else//电脑执行的时候
			{
				if (actor.stat[武将能力_武力] > 70 or actor.stat[武将能力_智力] > 70) win = true;
				else win = false;
			}
			if (buy) { ch::add_public_order(building, porder_gain, true); pk::add_gold(building, -200,true); }
			else if (win) ch::add_public_order(building, porder_gain, true);
			else ch::add_public_order(building, -porder_gain, true);
			after_search(actor, building);
			return true;
		}

		bool find_minutemen(pk::person@ actor, pk::building@ building)
		{
			//发现义勇兵
			//需要判断城市兵力不足
			int troops_find = 100 + pk::max(pk::rand( actor.stat[武将能力_魅力]),30)*2;

			int building_troops = pk::get_troops(building);
			int max_troops = pk::get_max_troops(building);
			if ((troops_find + building_troops) > max_troops)
			{
				troops_find = max_troops - building_troops;
			}
			if (troops_find > 0)
			{
				if (player_check(actor))
				{
					pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人，我们募集了志同道合\n之人，请让我们成为您的属下。",pk::decode(pk::get_name(actor)))), pk::get_person(武将_武芸者), building);
					
					pk::play_se(6);
					string msg = pk::encode(pk::format("\x1b[1x{}\x1b[0x在\x1b[2x{}\x1b[0x募集到{}义勇兵。", pk::decode(pk::get_name(actor)), pk::decode(pk::get_name(building)), troops_find));
					pk::history_log(actor.get_pos(), pk::get_force(actor.get_force_id()).color, msg);
				}
				pk::add_troops(building,troops_find,true);
				after_search(actor, building);
				return true;
			}
			return false;
		}

		bool find_doctor(pk::person@ actor, pk::building@ building)
		{
			//发现华佗，所属武将全部恢复，属于顶级探索效果
			//需要判断城市是否有受伤武将
			//xx在xx发现了华佗
			pk::list<pk::person@> list = get_injured_person_list(building);
			if (list.count > 0)
			{
				if (player_check(actor))
				{
					bool has_die = pk::get_year() > 208;
					string name = has_die ? "华佗":"华佗传人：华晨";
					pk::person@ doctor = has_die ? pk::get_person(武将_华佗):pk::get_person(武将_医者);

					pk::say(pk::encode(pk::format("在探索过程中偶遇\x1b[1x{}\x1b[0x，\n将他请来了。",name)), actor, building);
					pk::say(pk::encode("就让老夫帮你们看看吧。"), doctor, building);
					//发现金的音效
					pk::play_se(6);
					string msg = pk::encode(pk::format("\x1b[1x{}\x1b[0x在\x1b[2x{}\x1b[0x发现\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(actor)), pk::decode(pk::get_name(building)), name));
					pk::history_log(actor.get_pos(), pk::get_force(actor.get_force_id()).color, msg);

				}
				for (int i = 0; i < list.count; ++i)
				{
					list[i].shoubyou = 0;
					string msg = pk::encode(pk::format("\x1b[1x{}\x1b[0x的伤病复原了。", pk::decode(pk::get_name(list[i]))));
					pk::history_log(actor.get_pos(), pk::get_force(actor.get_force_id()).color, msg);
				}
				after_search(actor, building);
				return true;
			}
			return false;			
		}

		bool find_food(pk::person@ actor, pk::building@ building)
		{
			if (pk::get_season() != 季节_秋) return false;
			int charisma = actor.stat[武将能力_魅力];

			int food_find = pk::max(30, pk::min(80, pk::rand(charisma)))*20;//还需考虑建筑金满的情况
			int building_food = pk::get_food(building);
			int max_food = pk::get_max_food(building);
			if ((food_find + building_food) > max_food)
			{
				food_find = max_food - building_food;
			}
			if (food_find > 0)
			{
				if (player_check(actor))
				{
					
					pk::say(pk::encode("探索过程中参与了当地农户举行的\n丰收庆典。"), actor, building);

					pk::say(pk::encode(pk::format("我们的丰收多亏了\x1b[1x{}\x1b[0x大人平日\n治理有方，一点心意请务必收下。", pk::decode(pk::get_name(actor)))), pk::get_person(武将_农民), building);
					//发现金的音效
					pk::play_se(6);
					string msg = pk::encode(pk::format("\x1b[1x{}\x1b[0x在\x1b[2x{}\x1b[0x收到{}赠粮", pk::decode(pk::get_name(actor)), pk::decode(pk::get_name(building)), food_find));
					pk::history_log(actor.get_pos(), pk::get_force(actor.get_force_id()).color, msg);
				}

				pk::add_food(building, food_find, true);
				after_search(actor, building);
				return true;
			}
			return false;
		}

		bool find_gold(pk::person@ actor, pk::building@ building)
		{
			int charisma = actor.stat[武将能力_魅力];
			
			int gold_find = pk::max(30, pk::min(80, pk::rand(charisma)));//还需考虑建筑金满的情况
			int building_gold = pk::get_gold(building);
			int max_gold = pk::get_max_gold(building);
			if ((gold_find + building_gold) > max_gold)
			{
				gold_find = max_gold - building_gold;
			}
			if (gold_find > 0)
			{
				if (player_check(actor))
				{
					ch::say_message(5050, /*num*/gold_find, /*p0*/actor, /*p1*/null, /*b0*/null, /*p0_b*/building);
					//发现金的音效
					pk::play_se(6);
					string msg = pk::encode(pk::format("\x1b[1x{}\x1b[0x在\x1b[2x{}\x1b[0x发现资金{}", pk::decode(pk::get_name(actor)), pk::decode(pk::get_name(building)), gold_find));
					pk::history_log(actor.get_pos(), pk::get_force(actor.get_force_id()).color, msg);
				}
				//探索人材失败了，\n但却发现了資金{}了。
				// {}在{}发现資金{}
				pk::add_gold(building, gold_find, true);
				after_search(actor, building);
				return true;
			}
			return false;
		}

		bool find_gold2(pk::person@ actor, pk::building@ building)
		{
			int charisma = actor.stat[武将能力_魅力];
			array<int> target = {武将_商人, 武将_农民};
			array<float> rate = { 1.8f, 1.3f };
			int index = pk::rand(2);
			int gold_find = int(pk::max(30, pk::min(80, pk::rand(charisma)))*rate[index]);//还需考虑建筑金满的情况
			int building_gold = pk::get_gold(building);
			int max_gold = pk::get_max_gold(building);
			if ((gold_find + building_gold) > max_gold)
			{
				gold_find = max_gold - building_gold;
			}
			if (gold_find > 0)
			{
				if (player_check(actor))
				{
					pk::say(pk::encode(pk::format("这是我们为了感念\x1b[1x{}\x1b[0x大人平日\n来的照顾，而凑出来的黄金。", pk::decode(pk::get_name(actor)))), pk::get_person(target[index]), building);

					//发现金的音效
					pk::play_se(6);
					string msg = pk::encode(pk::format("\x1b[1x{}\x1b[0x在\x1b[2x{}\x1b[0x收到{}赠金", pk::decode(pk::get_name(actor)), pk::decode(pk::get_name(building)), gold_find));
					pk::history_log(actor.get_pos(), pk::get_force(actor.get_force_id()).color, msg);
				}
				//探索人材失败了，\n但却发现了資金{}了。
				// {}在{}发现資金{}
				pk::add_gold(building, gold_find, true);
				after_search(actor, building);
				return true;
			}
			return false;
		}

		pk::list<pk::person@> get_injured_person_list(pk::building@building)
		{
			int location_id = building.get_id();
			pk::list<pk::person@> list;
			for (int i = 0; i < 非贼武将_末; ++i)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person) and person.location == location_id and person.shoubyou > 0 and pk::check_mibun(person, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)))
				{
					list.add(person);
				}
			}
			return list;
		}

		void after_search(pk::person@ actor, pk::building@ building)
		{
			actor.action_done = true;
			pk::add_kouseki(actor, 50);
			pk::add_stat_exp(actor, 武将能力_政治, 1);
			pk::force@ force = pk::get_force(actor.get_force_id());
			if (pk::is_alive(force)) pk::add_tp(force, 2, actor.pos);
			int ap_cost = get_search_ap_cost(building);
			pk::district@ district = pk::get_district(building.get_district_id());
			pk::add_ap(district, -ap_cost);
		}

		int get_search_ap_cost(pk::building@ building)
		{
			if (!pk::is_alive(building)) return 20;
			pk::city@city = pk::building_to_city(building);
			if (!pk::is_alive(city)) return 20;
			return pk::has_facility(city, 设施_人材府) ? 10 : 20;
		}

		bool player_check(pk::person@ person)
		{
			if (pk::is_alive(person))
			{
				pk::force@ force = pk::get_force(person.get_force_id());
				if (pk::is_alive(force))
				{
					if (force.is_player())
					{
						pk::district@ district = pk::get_district(person.get_district_id());
						if (pk::is_alive(district))
						{
							if (pk::is_player_controlled(district)) return true;
						}
					}
				}
			}
			return false;
		}

		void scene_skin()
		{
			pk::background(bg_dir_);
			pk::message_box(msg5044(skin_actor_, skin_name_), skin_actor_);
			//发现金的音效
			pk::play_se(6);
			string msg = pk::encode(pk::format("\x1b[1x{}\x1b[0x在\x1b[2x{}\x1b[0x发现{}(皮肤)", pk::decode(pk::get_name(skin_actor_)), pk::decode(pk::get_name(pk::get_building(skin_actor_.service))), skin_name_));
			pk::history_log(skin_actor_.get_pos(), pk::get_force(skin_actor_.get_force_id()).color, msg);
		}

		void scene_duel()
		{
			pk::unit@ dummy_unit;
			pk::person@ dummy_person;
			duel_result_ = pk::duel(dummy_unit, dummy_unit, scen_actor_, dummy_person, dummy_person, pk::get_person(武将_贼将), pk::get_person(武将_贼将), dummy_person, true, false, 0, true).first;
		}

		void scene_debate()
		{
			pk::person@ 狗头军师 = pk::get_person(武将_恶臣);
			狗头军师.sei = pk::encode("狗头");
			狗头军师.mei = pk::encode("军师");
			狗头军师.stat[武将能力_智力] = 75;
			//狗头军师.update();
			duel_result_ = pk::debate(scen_actor_, 狗头军师, pk::is_player_controlled(scen_actor_), false, false, true).first;
			狗头军师.sei = pk::encode("恶");
			狗头军师.mei = pk::encode("臣");
			狗头军师.stat[武将能力_智力] = 1;
		}
		string msg5044(pk::person@person, string item_name)
		{
			if (person.get_id() == 무장_여포)
				return pk::encode(pk::format("未能发现人材，\n但却找到了{}。",item_name));
			if (person.get_id() == 무장_장비)
				return pk::encode(pk::format("哪里有什么人材啊。\n不过发现{}啦。",item_name));
			if (person.character == 성격_소심)
				return msg5045(person, item_name);
			if (person.character == 성격_냉정)
				return msg5046(person, item_name);
			if (person.character == 성격_대담)
				return msg5047(person, item_name);
			return msg5048(person, item_name);
		}

		string msg5045(pk::person@person, string item_name)
		{
			switch (pk::rand(3))
			{
			case 0:
				return pk::encode(pk::format("虽然探索人材失败，\n但是得到了{}。",item_name));
			case 1:
				return pk::encode(pk::format("没有探索到所谓的逸才，\n取而代之的是发现了{}。",item_name));
			case 2:
				return pk::encode(pk::format("没有探索到任何的人材，\n却发现了{}。",item_name));
			}
			return "";
		}

		string msg5046(pk::person@person, string item_name)
		{
			switch (pk::rand(3))
			{
			case 0:
				return pk::encode(pk::format("人材探索失败了，\n但发现到{}。",item_name));
			case 1:
				return pk::encode(pk::format("没有见到任何优秀的人材，\n但却得到了{}。",item_name));
			case 2:
				return pk::encode(pk::format("没有见到什么人材，\n是发现到{}了。",item_name));
			}
			return "";
		}

		string msg5047(pk::person@person, string item_name)
		{
			switch (pk::rand(3))
			{
			case 0:
				return pk::encode(pk::format("探索人材虽然失败了，\n但却得到了{}喔。",item_name));
			case 1:
				return pk::encode(pk::format("没有见到什么有骨气的人，\n却发现到{}了。",item_name));
			case 2:
				return pk::encode(pk::format("没有什么适当的人材，\n但却发现了{}。",item_name));
			}
			return "";
		}

		string msg5048(pk::person@person, string item_name)
		{
			switch (pk::rand(3))
			{
			case 0:
				return pk::encode(pk::format("探索人材是失败了，\n但发现到{}了。",item_name));
			case 1:
				return pk::encode(pk::format("没发现任何值得注目的人材，\n但却发现到了{}喔。",item_name));
			case 2:
				return pk::encode(pk::format("虽然没见到所谓的人材，\n但是得到了{}喔。",item_name));
			}
			return "";
		}
	}

	Main main;
}
// ## 2022/02/14 # 江东新风 # 部分常量中文化，激励运筹修复 ##
// ## 2021/10/24 # 江东新风 # stronghold to base ##
// ## 2021/03/12 # 江东新风 # 圣人相性差判断错误bug修复 ##
// ## 2021/02/18 # 江东新风 # 狼骑bug修复 ##
// ## 2021/02/17 # 江东新风 # 狼骑非己方回合不触发 ##
// ## 2021/02/15 # 江东新风 # 强募bug修复，效果微调，伯乐特效添加2 ##
// ## 2020/12/13 # 江东新风 # 新特技-强募 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/27 # 江东新风 # 新特技——狼骑##
// ## 2020/10/27 #氕氘氚# 常量整合##
// ## 2020/09/22 # keehl102 # 添加激励运筹 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/19 #江东新风#新特技-贤哲名称修正##
// ## 2020/08/14 #江东新风#新特技-贤哲##
namespace SIRE新特技
{
	class Main
	{
		pk::list<pk::unit@> _被袭部队;
		float _statEFF = 0.1f;

		Main()
		{
			pk::bind(111, pk::trigger112_t(onTurnStart));//伯乐
			pk::bind(112, pk::trigger112_t(onTurnEnd));//贤哲		
			pk::bind(173, pk::trigger173_t(unit_exp));//激励运筹
			pk::bind(174, pk::trigger174_t(unit_action_done));//激励运筹
			pk::bind(107, pk::trigger107_t(new_day));//激励运筹	
			pk::bind(104, pk::trigger104_t(onScenarioEnd));//狼骑
			pk::bind(172, pk::trigger172_t(onUnitPositionChange));//狼骑
		}

		void unit_exp(pk::unit@ unit, int type)
		{
<<<<<<< HEAD
			if (unit.type != 부대종류_전투 or unit.stock[11].amount != 0)
=======
			if (unit.type != 部队类型_战斗 or unit.stock[11].amount != 0)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				return;

			if (is_strategy_hit(type) and ch::has_skill(unit, 特技_运筹) and pk::rand_bool(运筹触发几率))
				unit.stock[11].amount = 1;
<<<<<<< HEAD
			else if (is_tactics_hit(type) and ch::has_skill(unit, 特技_激励) and pk::rand_bool(100))//激励触发几率))
=======
			else if (is_tactics_hit(type) and ch::has_skill(unit, 特技_激励) and pk::rand_bool(激励触发几率))
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				unit.stock[11].amount = 1;
		}

		void unit_action_done(pk::unit@ unit,int type)//type 2是电脑行动结束，0,1是玩家
		{
<<<<<<< HEAD
			if (unit.type != 부대종류_전투 or unit.stock[11].amount != 1)
=======
			if (type != 2)
			{
				_被袭部队.clear();
			}
			if (unit.type != 部队类型_战斗 or unit.stock[11].amount != 1)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				return;

			unit.stock[11].amount = 2;
			pk::set_remain_movement(unit, unit.attr.stat[部队能力_移动]);
			if (type != 2)//(unit.is_player() and pk::is_player_controlled(unit))
			{
				pk::set_action_done(unit, false);
			}
			else
			{
<<<<<<< HEAD
				pk::trace("is_p:"+ unit.is_player()+ ",p_c:"+ pk::is_player_controlled(unit));
=======
				//pk::trace("is_p:"+ unit.is_player()+ ",p_c:"+ pk::is_player_controlled(unit));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				pk::set_action_done(unit, false);
				pk::run_order(unit);
			}
		}

		void new_day()
		{
			auto list = pk::list_to_array(pk::get_unit_list());
			for (int i = 0; i < int(list.length); i++)
			{
				pk::unit@ unit = list[i];
<<<<<<< HEAD
				if (unit.type == 부대종류_전투)
=======
				if (unit.type == 部队类型_战斗)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					unit.stock[11].amount = 0;
			}
		}

		bool is_tactics_hit(int type)
		{
			switch (type)
			{
			case 经验值_突刺暴击:
			case 经验值_突刺:
			case 经验值_螺旋突暴击:
			case 经验值_螺旋突:
			case 经验值_二段突暴击:
			case 经验值_二段突:
			case 经验值_熊手暴击:
			case 经验值_熊手:
			case 经验值_横扫暴击:
			case 经验值_横扫:
			case 经验值_旋风暴击:
			case 经验值_旋风:
			case 经验值_火矢暴击:
			case 经验值_火矢:
			case 经验值_贯矢暴击:
			case 经验值_贯矢:
			case 经验值_乱射暴击:
			case 经验值_乱射:
			case 经验值_突击暴击:
			case 经验值_突击:
			case 经验值_突破暴击:
			case 经验值_突破:
			case 经验值_突进暴击:
			case 经验值_突进:
			case 经验值_破碎暴击:
			case 经验值_破碎:
			case 经验值_放射暴击:
			case 经验值_放射:
			case 经验值_投石暴击:
			case 经验值_投石:
			case 经验值_猛撞暴击:
			case 经验值_猛撞:
				return true;
			}
			return false;
		}

		bool is_strategy_hit(int type)
		{
			switch (type)
			{
			case 经验值_火计暴击:
			case 经验值_火计:
			case 经验值_灭火暴击:
			case 经验值_灭火:
			case 经验值_伪报暴击:
			case 经验值_伪报:
			case 经验值_扰乱暴击:
			case 经验值_扰乱:
			case 经验值_镇静暴击:
			case 经验值_镇静:
			case 经验值_伏兵暴击:
			case 经验值_伏兵:
			case 经验值_内讧暴击:
			case 经验值_内讧:
			case 经验值_妖术暴击:
			case 经验值_妖术:
			case 经验值_落雷暴击:
			case 经验值_落雷:
				return true;
			}
			return false;
		}

		//剧情需要namespace下的常量，因为无法传参
		pk::person@ 推荐人;
		pk::person@ 被推荐人;
		pk::force@ src_force;
		void onTurnStart(pk::force@ force)
		{
			//强募	
			if (pk::get_day() == 1 and force.is_player())
			{
				pk::list<pk::building@> strongholdList = ch::get_base_list(force);
				if (strongholdList.count > 0)
				{
					for (int i = 0; i < strongholdList.count; i++)
					{
						pk::list<pk::person@> person_list = pk::get_person_list(strongholdList[i], pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));

						if (person_list.count > 0)
						{
							for (int j = 0; j < person_list.count; j++)
							{
								//pk::trace(pk::format("pos1", 1));
								pk::person@ person0 = person_list[j];
								if (!pk::is_absent(person0) and pk::rand_bool(30) and ch::has_skill(person0, 特技_伯乐))
								{
									//pk::trace(pk::format("pos2", 1));
									//获取目标武将
									pk::list<pk::person@> list = pk::get_person_list(pk::mibun_flags(身份_在野, 身份_未发现));
									list.shuffle();
									for (int k = 0; k < list.count; k++)
									{
										pk::person@ person1 = list[k];
										if (pk::get_aishou_distance(person0, person1.get_id()) <= 25)
										{
											//pk::trace(pk::format("pos3", 1));											
											if (person1.is_alive())
											{
												//pk::trace(pk::format("pos4", 1));
												@推荐人 = person0;
												@被推荐人 = person1;
												@src_force = force;
												pk::scene(pk::scene_t(武将推荐));
												break;
												//开启推荐剧情
												//pk::find_talent(person0, person1, pk::get_building(person0.service));
											}
										}
									}

									//pk::say(pk::encode("!"), person0); 
								}
							}
						}
					}
				}

			}

		}

		void 武将推荐()
		{
			//pk::find_talent(src,dst, pk::get_building(src.service));
			pk::person@kunshu0 = pk::get_person(src_force.kunshu);
			pk::move_screen(kunshu0.get_pos());
			string talent = "武力";
			pk::fade(0);
			pk::sleep();
			//确定能力方向
			int max = -1;
			int t = -1;
			for (int i = 0; i < 武将能力_末; i++)
			{
				if (int(被推荐人.stat[i]) > max and 被推荐人.stat[i] >= 70)
				{
					max = 被推荐人.stat[i];
					t = i;
				}
			}
			switch (t)
			{
			case 武将能力_统率: talent = "兵法"; break;
			case 武将能力_武力: talent = "勇武"; break;
			case 武将能力_智力: talent = "谋略"; break;
			case 武将能力_政治: talent = "政略"; break;
			case 武将能力_魅力: talent = "人事"; break;
			}
			pk::background(场景_宫廷1);
			pk::fade(255);
			if (t != -1) pk::message_box(pk::encode(pk::format("主公，有一位名为\x1b[1x{}\x1b[0x的武将，在\x1b[2x{}\x1b[0x方面颇有建树，特向您举荐。",pk::decode(pk::get_name(被推荐人)), talent)), 推荐人);
			else  pk::message_box(pk::encode(pk::format("主公，有一位名为\x1b[1x{}\x1b[0x的武将，在当地颇有名气，特向您举荐。", pk::decode(pk::get_name(被推荐人)))), 推荐人);
			pk::message_box(pk::encode(pk::format("竟有此等人才，快带我去看看。", 1 )), kunshu0);
			if (t != -1) pk::message_box(pk::encode(pk::format("于\x1b[2x{}\x1b[0x方面小有自信，就请多多指教了。", talent)), 被推荐人);
			else pk::message_box(pk::encode(pk::format("能加入\x1b[2x{}\x1b[0x真是荣幸呀。", pk::decode(pk::get_name(kunshu0)))), 被推荐人);
			pk::set_district(被推荐人,推荐人.get_district_id());
			pk::set_service(被推荐人, 推荐人.service);
			pk::set_force(被推荐人, src_force, null, 0);
			被推荐人.location = 推荐人.service;
			被推荐人.loyalty = 100;
			被推荐人.kouseki = 500;
			pk::history_log(kunshu0.get_pos(), src_force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x受到\x1b[1x{}\x1b[0x的举荐，加入\x1b[2x{}\x1b[0x军。", pk::decode(pk::get_name(被推荐人)), pk::decode(pk::get_name(推荐人)), pk::decode(pk::get_name(kunshu0)))));
			pk::fade(0);
			pk::sleep();
			//确定能力方向
			pk::background(-1);
			pk::fade(255);

		}

		//贤哲，强募判定，回合结束时触发，技巧函数貌似要乘2才是遊?中的技巧		
		void onTurnEnd(pk::force@ force)
		{
			//强募	
			if (pk::get_day() == 1)
			{
				pk::list<pk::building@> strongholdList = ch::get_base_list(force);
				if (strongholdList.count > 0)
				{
					for (int i = 0; i < strongholdList.count; i++)
					{
						pk::list<pk::person@> person_list = pk::get_person_list(strongholdList[i], pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));

						if (person_list.count > 0)
						{
							for (int j = 0; j < person_list.count; j++)
							{
								int 强募_troops = 0;
								pk::person@ person0 = person_list[j];
								if (ch::has_skill(person0, 特技_强募))
								{
									int src_public_order = 0;
									src_public_order = int(strongholdList[i].get_id() < 城市_末 ? pk::building_to_city(strongholdList[i]).public_order : 80);
									强募_troops = int((500 + (src_public_order + 20) * person0.stat[武将能力_魅力] / 100) * 强募募兵系数);
									pk::add_troops(strongholdList[i], 强募_troops, true);
									//pk::say(pk::encode("!"), person0); 
								}
							}
						}
					}
				}

			}

			//pk::get_day() == 是每个月一次
			if (pk::get_day() == 1)
			{
				auto person_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)));
				int count_贤哲 = 0;
				if (person_list.length > 0)
				{
					for (int i = 0; i < int(person_list.length); i++)
					{
						pk::person@ person0 = person_list[i];
						if (ch::has_skill(person0, 特技_贤哲))
						{
							count_贤哲 = count_贤哲 + 1;
						}
					}
				}
				if (count_贤哲 != 0)
					ch::add_tp(force, (贤哲增加技巧 * count_贤哲), force.get_pos());
			}
		}


		//此后为狼骑特技		
		void onScenarioEnd()
		{
			_被袭部队.clear();
		}


		void onUnitPositionChange(pk::unit@ unit, const pk::point& in pos)
		{
			//pk::printf(pk::encode("onUnitPositionChange leader=["+unit.leader+"] pos=["+pos+"]  \n"));

			//pk::person@ leader = pk::get_person(unit.leader);
			if (!ch::has_skill(unit, 特技_狼骑)) return;
			if (unit.weapon != 兵器_战马) return;
			if (unit.action_done == true) return;
			if (unit.get_force_id() != pk::get_current_turn_force_id()) return;

			pk::person@ member = pk::get_person(ch::who_has_skill(unit, 特技_狼骑));
			_statEFF = StatEFF(member) * 狼骑伤害系数;
			LoopPath(unit, pos);
			AttackAround(unit, pos); // 마지막 포지션

			//pk::printf(pk::encode("onUnitPositionChange pos=["+pos+"] leader=["+unit.leader+"] unit_pos=["+unit.pos+"] \n"));	


		}

		void LoopPath(pk::unit@ unit, pk::point pos) {

<<<<<<< HEAD
			_被袭部队.clear();
=======
			if(pk::get_remain_movement(unit) < 4) _被袭部队.clear();
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

			auto paths = pk::get_path(unit, unit.pos, pos);
			for (int i = 0; i < int(paths.length); i++) {
				//pk::printf(pk::encode("LoopPath _unitLeader=["+unit.leader+"] pos=["+paths[i]+"] \n"));
				AttackAround(unit, paths[i]);
			}


		}


		void AttackAround(pk::unit@ unit0, pk::point pos0)
		{

			array<pk::point> arr = pk::range(pos0, 1, 1);
			for (int j = 0; j < int(arr.length); j++)
			{
				//pk::printf(pk::encode("AttackAround j=["+j+"] pos0=["+pos0+"] \n"));

				pk::unit@ dst = pk::get_unit(arr[j]);
				if (dst is null)continue;
				if (_被袭部队.contains(dst))continue;
<<<<<<< HEAD

=======
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				if (pk::is_enemy(unit0, dst)) {

					_被袭部队.add(dst);

					pk::play_se(0x71, dst.pos);
					pk::create_effect(0x22, dst.pos);
					pk::add_troops(dst, -int(dst.troops * _statEFF), true);
					MessageAttacked(dst);
					if (dst.troops <= 0)pk::kill(dst, unit0, true);


				}

			}
			//pk::printf(pk::encode("--------------------- AttackAround --------------------- \n"));
		}

		void MessageAttacked(pk::unit@ u0)
		{
			pk::person@ p0 = pk::get_person(u0.leader);
			if (!pk::is_alive(p0))return;
			switch (pk::rand(3))
			{
			case 0: pk::say(pk::encode("那个将领是谁?"), p0); break;
			case 1: pk::say(pk::encode("这就是传说中\x1b[16x狼骑\x1b[0x的威力吗!"), p0); break;
			case 2: pk::say(pk::encode("额啊啊啊..."), p0); break;
			}

		}


		float StatEFF(pk::person@ p0)
		{

			if (!pk::is_alive(p0))return 0.1f;
			float eff = (
				p0.stat[武将能力_统率] +
				p0.stat[武将能力_武力] +
				p0.stat[武将能力_智力] +
				p0.stat[武将能力_政治] +
				p0.stat[武将能力_魅力]
				) / float(武将能力_末 * 100.f);

			//pk::printf(pk::encode("StatEFF eff=["+eff+"]\n"));

			return eff;
		}


	} // end - Main
	Main main;
} // end - namespace		
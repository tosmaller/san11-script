// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/08/30 #白马叔叔# 设定军师之战开局保护月数，修改bug，修改规则 ##
// ## 2021/04/07 #白马叔叔# 尝试修复自己和自己舌战。随机势力是玩家势力时直接返回 ##
// ## 2021/01/17 #江东新风# 修复无玩家时触发导致的bug ##
// ## 2021/01/14 #江东新风# 优化代码，修复错误 ##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/11/12 # messi # 翻译修正##
/*
@제작자: HoneyBee
@설명: 책사(軍師)들간의 <책략전/정략전>


※ 참고사항

1. RANDOM으로 <책략전> or <정략전>이 발동합니다.
2. <책략전>인 경우 무장의 지력 수치를 기반으로 설전이 진행됩니다.
3. <정략전>인 경우 무장의 정치 수치를 기반으로 설전이 진행됩니다.
4. 따라서 지력이 높다고 항상 유리하지는 않음 (정략전은 정치 수치가 높아야 유리함)
5. <책략전>에서 패배하면 패배한 세력의 도시의 [병력, 병장기, 기력] 감소
6. <정략전>에서 패배하면 패배한 세력의 도시의 [금, 군량, 치안] 감소
7. PLAYER 세력은 추가로 <책략전> or <정략전>에서 승리하면 기교P 상승, 패배하면 기교P 감소
8. PLAYER 세력이 <군사>가 없다면, <군주>가 대신 설전을 진행합니다.
/策士(军师)之间的<策略之辩/政略之辩>

※注意

1.随机发动的<策略之辩> or<政略之辩>。
2.<策略之辩>的场合根据武将的智力数值进行舌战。
3.<政略之辩>的场合根据武将的政治数值进行舌战。
4.所以并不总是智力高才占优势(政略之辩必须是政治数值高才有利)
5.在<策略之辩>失败的话，失败的势力的城市的[兵力，兵装，气力]减少
6.在<政略之辩>失败的话，失败的势力的城市的[金，兵粮，治安]减少
7. PLAYER势力进一步在<策略之辩> or<政略之辩>获胜技巧P上升，失败技巧P减少
8. PLAYER势力没有<军师>的情况下，由<君主>代替进行舌战。

*/

namespace 책략전_정략전
{

	// ================ CUSTOMIZE ================

	const int 발동시기 = 2;               					// 1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일
	const int 발동시기_확률 = 10;           					// 발동 확률 설정 (100은 100% 확률로 발생한다는 의미)
	const int 军师之战_保护月数 = 6;                       //军师之战开局的保护月数

	// <책략전>인 경우 설전에서 패배하는 세력의 도시에서 감소하는 [병력, 병장기, 기력] 수치
	const array<int> 병력_감소 = { 5000, 8000, 12000 };	// 감소하는 병력 수치                  兵力
	const array<int> 병장기_감소 = { 5000, 10000, 15000 };	// 감소하는 병장기 수치           兵装
	const array<int> 기력_감소 = { 30, 40, 50 };				// 감소하는 기력 수치                       气力


	// <정략전>인 경우 설전에서 패배하는 세력의 도시에서 감소하는 [금, 군량, 치안] 수치
	const array<int> 금_감소 = { 5000, 10000, 15000 };		// 감소하는 금 수치                      金
	const array<int> 군량_감소 = { 10000, 15000, 20000 };	// 감소하는 군량 수치               粮
	const array<int> 치안_감소 = { 30, 40, 50 };				// 감소하는 치안 수치                       治安


	// <책략전> or <정략전>에서 승리하거나 패배했을 때 PLAYER 세력의 기교P 상승 및 감소 수치 
	// 기교P_증가는 설정한 수치 그대로 증가하지 않음
	// 해당 SCRIPT 문제이거나 PK2.2 문제일 가능성이 있으므로 일단 수치를 낮춰서 반영하세요.
	const int 기교P_증가 = 100;		    // 기교P 증가 수치 		
	const int 기교P_감소 = 200;		    // 기교P 감소 수치 
	//const array<int> 기교P_증가 = { 50, 100, 150 };		    // 기교P 증가 수치 		
	//const array<int> 기교P_감소 = { 500, 700, 900 };		    // 기교P 감소 수치 

// ===========================================


	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		pk::random random(pk::rand());
		pk::list<pk::force@> force_list_enemy;
		int force_num = -1;

		pk::force@ enemy_force;
		pk::force@ player_force;
		pk::person@ enemy_gunshi;
		pk::person@ person_0;

		int debate_result = 1;

		bool result_책략 = false;
		bool result_정치술 = false;

		void callback()
		{
			if (pk::get_player_count() > 0 and pk::get_elapsed_months() >= 军师之战_保护月数)
			{
				if (발동시기 == 1 and (pk::get_day() == 1))
				{
					set_playerForce();
					battleOfIntellectual();  // 매월 1일
				}
				else if (발동시기 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
				{
					set_playerForce();
					battleOfIntellectual();  // 매분기 첫달이면서 1일
				}
				else if (발동시기 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
				{
					set_playerForce();
					battleOfIntellectual();  // 매년 1월 1일
				}
			}
		}


		void set_playerForce()
		{
			pk::list<pk::force@> force_list_0;
			pk::list<pk::force@> force_list = pk::get_force_list(false);  //不包含盗贼
			
			for (int i = 0; i < force_list.count; i++)
			{
				if (force_list[i].is_player() and pk::is_alive(force_list[i]))
					force_list_0.add(force_list[i]);  //抽出全部玩家势力
				else if (!force_list[i].is_player() and pk::is_alive(force_list[i]))
					force_list_enemy.add(force_list[i]);  //抽出全部敌军势力
			}
			
			@player_force = force_list_0[pk::rand(force_list_0.count)];  //随机获取一个幸运势力
			//pk::trace(pk::format("随机抽玩家1名，player_force：{}", player_force.get_force_id()));
			if (player_force.gunshi == -1)
				@person_0 = pk::get_person(player_force.kunshu);
			else
				@person_0 = pk::get_person(player_force.gunshi);
		} // set_playerForce()


		void battleOfIntellectual()
		{
			if (pk::rand_bool(발동시기_확률) and force_list_enemy.count > 0)
			{
				bool has_good_gunshi = false;
				for (int j = 0; j < 8; j++)
				{
					@enemy_force = force_list_enemy[pk::rand(force_list_enemy.count)];  //随机抽取一名敌势力
					if (enemy_force.is_player()) continue;  //随到的是玩家继续
					if (enemy_force.gunshi == -1) continue;  //没有军师就继续					
					@enemy_gunshi = pk::get_person(enemy_force.gunshi);  //获取敌军师
					if (int(person_0.stat[2] - enemy_gunshi.stat[2]) < 5)  //必须加上int才是正确的差值
					{
						has_good_gunshi = true;
						break;  //智力差值较低，军师较好
					}
					if (int(person_0.stat[3] - enemy_gunshi.stat[3]) < 5)  //必须加上int才是正确的差值
					{
						has_good_gunshi = true;
						break;  //政治差值较低，军师较好
					}
				}
				if (!has_good_gunshi) return;  //没有好的军师就直接算了不比
				force_num = enemy_force.get_force_id();  //敌势力的ID
				
				pk::scene(pk::scene_t(scene));  //进行舌战
				
				debate_result_set();  //奖惩执行

			}
			else { return; }

		} // battleOfIntellectual()


		void scene()
		{
			pk::move_screen(person_0.get_pos(), 500);
			pk::fade(0);
			pk::sleep();
			pk::background(5);
			pk::fade(255);

			//int random_n = pk::rand(2); // 0, 1
			//pk::trace(pk::format("智：我{} 敌{}，政：我{} 敌{}", person_0.stat[2], enemy_gunshi.stat[2], person_0.stat[3], enemy_gunshi.stat[3]));
			// <책략전> - 지력으로 설전  智力差值小时用智力来比，必须加上int才是正确的差值
			if (int(person_0.stat[2] - enemy_gunshi.stat[2]) < 5)
			{
				result_책략 = true;

				pk::message_box(pk::encode("呼，能识破我的策略嘛?"), enemy_gunshi);

<<<<<<< HEAD
				debate_result = pk::debate(person_0, enemy_gunshi, true, false, false, true).first;
=======
				debate_result = pk::debate(person_0, enemy_gunshi, true, false, false, true,武将能力_智力).first;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

				if (debate_result == 0)
				{
					pk::play_se(12);
					pk::message_box(pk::encode("呃...比我预想的要出色.."), enemy_gunshi);
					pk::fade(0);
					pk::background(-1);
					pk::fade(255);
					pk::history_log(person_0.get_pos(), player_force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x成功识破敌人的策略", pk::decode(pk::get_name(person_0)))));
				}
				else
				{
					pk::play_se(13);
					pk::message_box(pk::encode("哈哈哈，最终还是没能识破呀."), enemy_gunshi);
					pk::fade(0);
					pk::background(-1);
					pk::fade(255);
					pk::history_log(person_0.get_pos(), player_force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x未能识破敌人策略，兵力 兵装 气力遭受损失", pk::decode(pk::get_name(person_0)))));
				}

			}

			// <정략전> - 정치로 설전 不然就是用政治比
			else
			{
				result_정치술 = true;

<<<<<<< HEAD
				int 원본지력_enemy_gunshi = enemy_gunshi.stat[武将能力_智力];
				enemy_gunshi.stat[武将能力_智力] = enemy_gunshi.stat[武将能力_政治];

				int 원본지력_person_0 = person_0.stat[武将能力_智力];
				person_0.stat[武将能力_智力] = person_0.stat[武将能力_政治];

				pk::message_box(pk::encode("呼，能识破我的政略吗?"), enemy_gunshi);

				debate_result = pk::debate(person_0, enemy_gunshi, true, false, false, true).first;
=======
				//int 원본지력_enemy_gunshi = enemy_gunshi.stat[武将能力_智力];
				//enemy_gunshi.stat[武将能力_智力] = enemy_gunshi.stat[武将能力_政治];

				//int 원본지력_person_0 = person_0.stat[武将能力_智力];
				//person_0.stat[武将能力_智力] = person_0.stat[武将能力_政治];

				pk::message_box(pk::encode("呼，能识破我的政略吗?"), enemy_gunshi);

				debate_result = pk::debate(person_0, enemy_gunshi, true, false, false, true, 武将能力_政治).first;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

				if (debate_result == 0)
				{
					pk::play_se(12);
					pk::message_box(pk::encode("呃...比我预想的要出色.."), enemy_gunshi);
					pk::fade(0);
					pk::background(-1);
					pk::fade(255);
					pk::history_log(person_0.get_pos(), player_force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x成功识破敌人的政略", pk::decode(pk::get_name(person_0)))));
				}
				else
				{
					pk::play_se(13);
					pk::message_box(pk::encode("哈哈哈，我的政治手腕可不是能轻易识破的."), enemy_gunshi);
					pk::fade(0);
					pk::background(-1);
					pk::fade(255);
					pk::history_log(person_0.get_pos(), player_force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x未能识破敌人政略，金 兵粮 治安遭受损失", pk::decode(pk::get_name(person_0)))));
				}

<<<<<<< HEAD
				enemy_gunshi.stat[武将能力_智力] = 원본지력_enemy_gunshi;
				person_0.stat[武将能力_智力] = 원본지력_person_0;
=======
				//enemy_gunshi.stat[武将能力_智力] = 원본지력_enemy_gunshi;
				//person_0.stat[武将能力_智力] = 원본지력_person_0;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

			}
		}

		void debate_result_set()
		{
			if (debate_result == 0)
			{
				ch::add_tp(player_force, 기교P_증가, player_force.get_pos());
				ch::add_tp(enemy_force, -기교P_증가, enemy_force.get_pos());
				//pk::trace(pk::format("胜，force_num：{}", force_num));
				for (int i = 0; i < 建筑_城市末; i++)
				{
					auto city = pk::get_city(i);
					if (city.get_force_id() == force_num)
					{
						if (result_책략)
						{
							ch::add_troops(city, -병력_감소[pk::rand(2)], true, 102);
<<<<<<< HEAD
							pk::add_weapon_amount(city, 병기_창, -병장기_감소[pk::rand(2)], true);
							pk::add_weapon_amount(city, 병기_극, -병장기_감소[pk::rand(2)], true);
							pk::add_weapon_amount(city, 병기_노, -병장기_감소[pk::rand(2)], true);
							pk::add_weapon_amount(city, 병기_군마, -병장기_감소[pk::rand(2)], true);
=======
							pk::add_weapon_amount(city, 兵器_枪, -병장기_감소[pk::rand(2)], true);
							pk::add_weapon_amount(city, 兵器_戟, -병장기_감소[pk::rand(2)], true);
							pk::add_weapon_amount(city, 兵器_弩, -병장기_감소[pk::rand(2)], true);
							pk::add_weapon_amount(city, 兵器_战马, -병장기_감소[pk::rand(2)], true);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
							pk::add_energy(city, -기력_감소[pk::rand(2)], true);

							result_책략 = false;
						}

						if (result_정치술)
						{
							pk::add_gold(city, -금_감소[pk::rand(2)], true);
							pk::add_food(city, -군량_감소[pk::rand(2)], true);
							pk::add_public_order(city, -치안_감소[pk::rand(2)], true);

							result_정치술 = false;
						}
					}
				}
			}
			
			else if (debate_result == 1)
			{
				ch::add_tp(enemy_force, 기교P_감소, enemy_force.get_pos());
				ch::add_tp(player_force, -기교P_감소, player_force.get_pos());
				//pk::trace(pk::format("负，force_num：{}", force_num));
				for (int i = 0; i < 建筑_城市末; i++)
				{
					auto city = pk::get_city(i);
					if (city.get_force_id() == player_force.get_force_id())  //失败方是玩家操作方
					{
						if (result_책략)
						{
							ch::add_troops(city, -병력_감소[pk::rand(3)], true, 102);
<<<<<<< HEAD
							pk::add_weapon_amount(city, 병기_창, -병장기_감소[pk::rand(3)], true);
							pk::add_weapon_amount(city, 병기_극, -병장기_감소[pk::rand(3)], true);
							pk::add_weapon_amount(city, 병기_노, -병장기_감소[pk::rand(3)], true);
							pk::add_weapon_amount(city, 병기_군마, -병장기_감소[pk::rand(3)], true);
=======
							pk::add_weapon_amount(city, 兵器_枪, -병장기_감소[pk::rand(3)], true);
							pk::add_weapon_amount(city, 兵器_戟, -병장기_감소[pk::rand(3)], true);
							pk::add_weapon_amount(city, 兵器_弩, -병장기_감소[pk::rand(3)], true);
							pk::add_weapon_amount(city, 兵器_战马, -병장기_감소[pk::rand(3)], true);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
							pk::add_energy(city, -기력_감소[pk::rand(3)], true);

							result_책략 = false;
						}

						if (result_정치술)
						{
							pk::add_gold(city, -금_감소[pk::rand(3)], true);
							pk::add_food(city, -군량_감소[pk::rand(3)], true);
							pk::add_public_order(city, -치안_감소[pk::rand(3)], true);

							result_정치술 = false;
						}
					}
				}
			}
		}

	} // class Main

	Main main;

} // namespace
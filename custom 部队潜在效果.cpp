// ## 2022/03/28 # 江东新风 # 修复因部队死亡导致的空指针bug ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/01/17 # 江东新风 # 触发后在情报履历提示,发动几率降低 ##
// ## 2020/12/24 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/31 # 江东新风 # 同步honeybee大神的更新##
/*
@제작자: HoneyBee
@설명: 부대에 따라 잠재 효과를 적용하여 긍정적/부정적 효과를 발동 (확률적으로 발동)
@기타: 특정인물에게 특정 효과를 발동하도록 설정 (기존에 제작한 지휘관 효과를 바탕으로 수정)
	 (적군의 병력 및 기력을 감소시키는 효과는 적군 부대의 병력 500 이하, 기력이 10 이하인 경우에는 효과가 적용되지 않음
	  해당 제약 조건은 CUSTOMIZE에서 취향에 맞게 설정하도록 하세요.)


※ 삼국지에서 별로 유명하지 않은 인물이 유명한 인물을 상대로 승리하거나 아군의 실책으로 인해 손해를 입는 묘사를 참고
예시1) <장특>이 <제갈각>을 상대로 승리한 [합비신성] 전투
예시2) <제갈량>의 1차 북벌에서 실책하여 전투를 패배하게 만든 <마속>의 일화


※ 해당 효과는 全 무장에게 적용됩니다. (기본 발동 확률은 1%로 설정)


※ 긍정적 효과를 발휘하면 공적이 상승하고, 부정적 효과를 발휘하면 공적이 하락함
- 부대 주장의 통솔 and 지력이 80미만인 경우에 긍정적 효과를 발휘하면 공적 상승량 2배
- 부대 주장의 통솔 or 지력이 80이상인 경우에 부정적 효과를 발휘하면 공적 감소량 2배


1. 긍정적_기력회복 (아군 기력회복) / 부정적_기력회복 (적군 기력회복)
2. 긍정적_혼란효과 (적군 혼란효과) / 부정적_혼란효과 (아군 혼란효과)
3. 긍정적_진정효과 (아군 진정효과) / 부정적_진정효과 (적군 진정효과)
4. 긍정적_병력회복 (아군 병력회복) / 부정적_병력회복 (적군 병력회복)
5. 긍정적_병력피해 (적군 병력피해) / 부정적_병력피해 (아군 병력피해)
6. 긍정적_기력감소 (적군 기력감소) / 부정적_기력감소 (아군 기력감소)
@说明:根据部队应用潜在效果，发动正面/负面效果(概率性地发动)

@其他:设定特定人物发动特定效果(根据原创制作的指挥官效果修正)
减少敌人兵力和气力的效果，敌人部队兵力在500以下，气力在10以下时，不适用效果
这个限制，请在CUSTOMIZE上按照喜好设定。)

※参照三国志中不太有名的人物战胜有名的人物，或我方失误造成损失的记述
例1)<张特>战胜了<诸葛恪>[合肥神圣]战斗
例2)在<孔明>的第1次北伐中失败，致使战斗失败的<马谡>的逸事

※这个效果，适用于全部武将。(基本的发动概率设定为1%)

※发挥正面的效果功绩上升，发挥负面的效果功绩下降
-部队的主将的统率且智力不足80的情况下发挥正面效果的功绩上升量为2倍
-部队的主将的统率或智力在80以上的场合发挥负面效果的功绩减少量为2倍

1.正面_气力恢复(我方气力恢复)/负面_气力恢复(敌方气力恢复)
2.正面_混乱效果(敌人混乱效果)/负面_混乱效果(我方混乱效果)
3.正面_镇定效果(我方镇定效果)/负面_镇定效果(敌方镇定效果)
4.正面_兵力恢复(我方兵力恢复)/负面_兵力恢复(敌方兵力恢复)
5.正面_兵力损失(敌方部队伤害)/负面_兵力损失(我方部队伤害)
6.正面_气力减少(敌方气力减少)/负面_气力减少(我方气力减少)

*/

namespace 부대_잠재효과
{

	// ================ CUSTOMIZE ================

	const bool 효과_AI만 = false;   // true =AI만 효과적용, false =유저와 AI 모두 효과 적용 

	const int 효과발생확률 = 1;       // 改为默认概率1/1000

	// 긍정적 효과가 발생할 확률을 예시로 50%로 설정 (취향에 맞게 변경하세요.)
	const int 행운확률 = 50;        // (100 - 행운확률) = 불운확률

	const int energy_회복 = 10;    // 기력 회복 수치
	const int energy_감소 = 10;    // 기력 감소 수치

	const int energy_제약조건 = 10;  // 기력 10 이하 아군/적군 부대는 기력 감소 효과가 적용되지 않음 
	const int troops_제약조건 = 500; // 병력 500 이하 아군/적군 부대는 병력 감소 효과가 적용되지 않음 

	const int 공적상승 = 2000;       // 긍정적 효과를 발동할 경우 상승하는 공적 수치 
	const int 공적하락 = 2000;       // 부정적 효과를 발동할 경우 하락하는 공적 수치  


	// 예시로 병력 회복 및 병력 피해는 500, 600, 700, 800, 900 中 한 가지가 선택 발동
	// ARRAY에 원하는 수치로 설정하시면 됩니다. 
	const array<int> troops_회복 = { 500, 600, 700, 800, 900 };
	const array<int> troops_피해 = { 500, 600, 700, 800, 900 };

	// ===========================================


	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}


		void callback()
		{
			commander_Type1();
			commander_Type2();
			commander_Type3();
			commander_Type4();
			commander_Type5();
			commander_Type6();
		}


		// 유저 또는 AI만 적용 시 판단함수
		bool only_AI_unit(pk::unit@ unit)
		{
			if (효과_AI만 and unit.is_player()) return false;
			return true;
		}


		// 기력 회복 
		void commander_Type1()
		{
			auto list = pk::list_to_array(pk::get_unit_list());

			for (int i = 0; i < int(list.length); i++)
			{

				pk::unit@ src = list[i];
				pk::point pos = src.get_pos();

				pk::person@ src_leader = pk::get_person(src.leader);

				if (only_AI_unit(src) and ch::rand_bool_1000(효과발생확률))
				{

					if (pk::rand_bool(행운확률)) // 긍정적 효과 
					{
						array<pk::point> arr = pk::range(pos, 1, 3);

						for (int j = 0; j < int(arr.length); j++)
						{
							pk::unit@ dst = pk::get_unit(arr[j]);

							if (dst is null or pk::is_enemy(src, dst)) continue;

							pk::add_energy(dst, energy_회복, true);
						}

						if ((src_leader.stat[武将能力_统率] < 80) and (src_leader.stat[武将能力_智力] < 80))
						{
							pk::add_kouseki(src, 공적상승 * 2);
						}
						else
						{
							pk::add_kouseki(src, 공적상승);
						}


						// 효과를 발생시키는 부대 
						pk::add_energy(src, energy_회복, true);


						if (pk::is_in_screen(pos))
						{
							pk::play_voice(0x37, pk::get_person(src.leader));
							pk::play_se(0x39, pos);
							pk::create_effect(0x3a, pos);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("请再接再厉!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("大伙一起努力吧!"), pk::get_person(src.leader)); break;
							}
						}
						pk::history_log(src.pos, pk::get_force(src.get_force_id()).color, pk::encode(pk::format("因为\x1b[1x{}\x1b[0x的奋起，友方\x1b[1x士气大振\x1b[0x！", pk::decode(pk::get_name(src)))));

					}
					else // 부정적 효과 
					{
						array<pk::point> arr = pk::range(pos, 1, 3);

						for (int j = 0; j < int(arr.length); j++)
						{
							pk::unit@ dst = pk::get_unit(arr[j]);


							if (dst is null or !pk::is_enemy(src, dst)) continue;


							pk::add_energy(dst, energy_회복, true);

						}

						if ((src_leader.stat[武将能力_统率] >= 80) or (src_leader.stat[武将能力_智力] >= 80))
						{
							pk::add_kouseki(src, -공적하락 * 2);
						}
						else
						{
							pk::add_kouseki(src, -공적하락);
						}

						if (pk::is_in_screen(pos))
						{
							pk::play_voice(0x37, pk::get_person(src.leader));
							pk::play_se(0x39, pos);
							pk::create_effect(0x3a, pos);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("呃...我的失误使敌军士气高涨.."), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("没想到反而激励了敌军.."), pk::get_person(src.leader)); break;
							}
						}
						pk::history_log(src.pos, pk::get_force(src.get_force_id()).color, pk::encode(pk::format("因为\x1b[1x{}\x1b[0x的失误，敌方\x1b[1x士气大振\x1b[0x！", pk::decode(pk::get_name(src)))));
					}

				}

			}

		} // commander_Type1


		// 혼란 효과 
		void commander_Type2()
		{
			auto list = pk::list_to_array(pk::get_unit_list());

			for (int i = 0; i < int(list.length); i++)
			{

				pk::unit@ src = list[i];
				pk::point pos = src.get_pos();

				pk::person@ src_leader = pk::get_person(src.leader);

				if (only_AI_unit(src) and ch::rand_bool_1000(효과발생확률))
				{

					if (pk::rand_bool(행운확률)) // 긍정적 효과 
					{
						array<pk::point> arr = pk::range(pos, 1, 3);

						for (int j = 0; j < int(arr.length); j++)
						{
							pk::unit@ dst = pk::get_unit(arr[j]);

							if (dst is null or dst.status != 部队状态_通常
								or !pk::is_enemy(src, dst))
								continue;

							pk::set_status(dst, src, 部队状态_混乱, 2, true);

						}

						if ((src_leader.stat[武将能力_统率] < 80) and (src_leader.stat[武将能力_智力] < 80))
						{
							pk::add_kouseki(src, 공적상승 * 2);
						}
						else
						{
							pk::add_kouseki(src, 공적상승);
						}

						if (pk::is_in_screen(pos))
						{
							pk::play_voice(0x37, pk::get_person(src.leader));
							pk::play_se(0x39, pos);
							pk::create_effect(0x3a, pos);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("虚则实之，实则虚之."), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("兵法的真谛在于虚实结合!"), pk::get_person(src.leader)); break;
							}
						}
						pk::history_log(src.pos, pk::get_force(src.get_force_id()).color, pk::encode(pk::format("因为\x1b[1x{}\x1b[0x的策略，周围敌军\x1b[1x陷入混乱\x1b[0x！", pk::decode(pk::get_name(src)))));
					}
					else // 부정적 효과 
					{
						array<pk::point> arr = pk::range(pos, 1, 3);

						for (int j = 0; j < int(arr.length); j++)
						{
							pk::unit@ dst = pk::get_unit(arr[j]);

							if (dst is null or dst.status != 部队状态_通常
								or pk::is_enemy(src, dst)) continue;

							pk::set_status(dst, src, 部队状态_混乱, 2, true);

						}

						if ((src_leader.stat[武将能力_统率] >= 80) or (src_leader.stat[武将能力_智力] >= 80))
						{
							pk::add_kouseki(src, -공적하락 * 2);
						}
						else
						{
							pk::add_kouseki(src, -공적하락);
						}

						if (src.status == 部队状态_通常)
						{
							pk::set_status(src, src, 部队状态_混乱, 2, true);
						}

						if (pk::is_in_screen(pos))
						{
							pk::play_voice(0x37, pk::get_person(src.leader));
							pk::play_se(0x39, pos);
							pk::create_effect(0x3a, pos);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("没想到竟犯了大错.."), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("呃..付出了沉重的代价呀.."), pk::get_person(src.leader)); break;
							}
						}
						pk::history_log(src.pos, pk::get_force(src.get_force_id()).color, pk::encode(pk::format("因为\x1b[1x{}\x1b[0x的策略失误，周围友军\x1b[1x陷入混乱\x1b[0x！", pk::decode(pk::get_name(src)))));
					}

				}

			}

		} // commander_Type2


		// 진정 효과 
		void commander_Type3()
		{
			auto list = pk::list_to_array(pk::get_unit_list());

			for (int i = 0; i < int(list.length); i++)
			{

				pk::unit@ src = list[i];
				pk::point pos = src.get_pos();

				pk::person@ src_leader = pk::get_person(src.leader);

				if (only_AI_unit(src) and ch::rand_bool_1000(효과발생확률))
				{

					if (pk::rand_bool(행운확률)) // 긍정적 효과 
					{
						array<pk::point> arr = pk::range(pos, 1, 3);

						for (int j = 0; j < int(arr.length); j++)
						{
							pk::unit@ dst = pk::get_unit(arr[j]);


							if (dst is null or dst.status == 部队状态_通常 or pk::is_enemy(src, dst))
								continue;

							pk::set_status(dst, src, 部队状态_通常, 2, true);

						}

						if ((src_leader.stat[武将能力_统率] < 80) and (src_leader.stat[武将能力_智力] < 80))
						{
							pk::add_kouseki(src, 공적상승 * 2);
						}
						else
						{
							pk::add_kouseki(src, 공적상승);
						}

						pk::set_status(src, src, 部队状态_通常, 2, true);


						if (pk::is_in_screen(pos))
						{
							pk::play_voice(0x37, pk::get_person(src.leader));
							pk::play_se(0x39, pos);
							pk::create_effect(0x3a, pos);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("严整军纪，莫留空隙!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("打起精神!"), pk::get_person(src.leader)); break;
							}
						}
						pk::history_log(src.pos, pk::get_force(src.get_force_id()).color, pk::encode(pk::format("因为\x1b[1x{}\x1b[0x的奋起，周围友军\x1b[1x镇静下来了\x1b[0x！", pk::decode(pk::get_name(src)))));

					}
					else // 부정적 효과
					{
						array<pk::point> arr = pk::range(pos, 1, 3);

						for (int j = 0; j < int(arr.length); j++)
						{
							pk::unit@ dst = pk::get_unit(arr[j]);


							if (dst is null or dst.status == 部队状态_通常
								or !pk::is_enemy(src, dst)) continue;

							pk::set_status(dst, src, 部队状态_通常, 2, true);

						}

						if ((src_leader.stat[武将能力_统率] >= 80) or (src_leader.stat[武将能力_智力] >= 80))
						{
							pk::add_kouseki(src, -공적하락 * 2);
						}
						else
						{
							pk::add_kouseki(src, -공적하락);
						}

						if (pk::is_in_screen(pos))
						{
							pk::play_voice(0x37, pk::get_person(src.leader));
							pk::play_se(0x39, pos);
							pk::create_effect(0x3a, pos);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("没想到让敌军竟还有机会整顿.."), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("因为我的失误，让敌军赢得了整顿的时间.."), pk::get_person(src.leader)); break;
							}
						}
						pk::history_log(src.pos, pk::get_force(src.get_force_id()).color, pk::encode(pk::format("因为\x1b[1x{}\x1b[0x的失误，周围敌军得以\x1b[1x重整旗鼓\x1b[0x！", pk::decode(pk::get_name(src)))));
					}

				}

			}

		} // commander_Type3


		// 병력 회복 
		void commander_Type4()
		{
			auto list = pk::list_to_array(pk::get_unit_list());

			for (int i = 0; i < int(list.length); i++)
			{

				pk::unit@ src = list[i];
				pk::point pos = src.get_pos();

				pk::person@ src_leader = pk::get_person(src.leader);

				if (only_AI_unit(src) and ch::rand_bool_1000(효과발생확률))
				{

					if (pk::rand_bool(행운확률)) // 긍정적 효과 
					{
						array<pk::point> arr = pk::range(pos, 1, 3);

						for (int j = 0; j < int(arr.length); j++)
						{
							pk::unit@ dst = pk::get_unit(arr[j]);


							if (dst is null or pk::is_enemy(src, dst)) continue;

							ch::add_troops(dst, troops_회복[pk::rand(5)], true, 100);

						}

						if ((src_leader.stat[武将能力_统率] < 80) and (src_leader.stat[武将能力_智力] < 80))
						{
							pk::add_kouseki(src, 공적상승 * 2);
						}
						else
						{
							pk::add_kouseki(src, 공적상승);
						}

						// 효과를 발휘하는 부대
						if (ch::rand_bool_1000(효과발생확률))
						{
							ch::add_troops(src, troops_회복[pk::rand(5)], true, 100);
						}

						if (pk::is_in_screen(pos))
						{
							pk::play_voice(0x37, pk::get_person(src.leader));
							pk::play_se(0x39, pos);
							pk::create_effect(0x3a, pos);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("全军，跟我来!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("跟着我，无所畏惧!"), pk::get_person(src.leader)); break;
							}
						}
						pk::history_log(src.pos, pk::get_force(src.get_force_id()).color, pk::encode(pk::format("因为\x1b[1x{}\x1b[0x的活跃，周围友军军力得到\x1b[1x补充\x1b[0x！", pk::decode(pk::get_name(src)))));

					}
					else // 부정적 효과 
					{
						array<pk::point> arr = pk::range(pos, 1, 3);

						for (int j = 0; j < int(arr.length); j++)
						{
							pk::unit@ dst = pk::get_unit(arr[j]);

							if (dst is null or !pk::is_enemy(src, dst)) continue;

							ch::add_troops(dst, troops_회복[pk::rand(5)], true, 100);

						}

						if ((src_leader.stat[武将能力_统率] >= 80) or (src_leader.stat[武将能力_智力] >= 80))
						{
							pk::add_kouseki(src, -공적하락 * 2);
						}
						else
						{
							pk::add_kouseki(src, -공적하락);
						}

						if (pk::is_in_screen(pos))
						{
							pk::play_voice(0x37, pk::get_person(src.leader));
							pk::play_se(0x39, pos);
							pk::create_effect(0x3a, pos);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("敌军的援军到了.."), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("敌军居然已经到了这里.."), pk::get_person(src.leader)); break;
							}
						}
						pk::history_log(src.pos, pk::get_force(src.get_force_id()).color, pk::encode(pk::format("因为\x1b[1x{}\x1b[0x的重大失误，周围敌军军力得以\x1b[1x补充兵力\x1b[0x！", pk::decode(pk::get_name(src)))));
					}

				}

			}

		} // commander_Type4


		// 병력 피해 
		void commander_Type5()
		{
			auto list = pk::list_to_array(pk::get_unit_list());


			for (int i = 0; i < int(list.length); i++)
			{

				pk::unit@ src = list[i];
				pk::point pos = src.get_pos();

				pk::person@ src_leader = pk::get_person(src.leader);

				if (only_AI_unit(src) and ch::rand_bool_1000(효과발생확률))
				{

					if (pk::rand_bool(행운확률)) // 긍정적 효과 
					{
						array<pk::point> arr = pk::range(pos, 1, 3);

						for (int j = 0; j < int(arr.length); j++)
						{
							pk::unit@ dst = pk::get_unit(arr[j]);

							if (dst is null or !pk::is_enemy(src, dst)
								or (troops_제약조건 >= dst.troops)) continue;

							ch::add_troops(dst, -troops_피해[pk::rand(5)], true,100);

							// 병력 0 이 되면 부대 궤멸
							if (dst.troops == 0) { pk::kill(dst); }

						}

						if ((src_leader.stat[武将能力_统率] < 80) and (src_leader.stat[武将能力_智力] < 80))
						{
							pk::add_kouseki(src, 공적상승 * 2);
						}
						else
						{
							pk::add_kouseki(src, 공적상승);
						}

						if (pk::is_in_screen(pos))
						{
							pk::play_voice(0x37, pk::get_person(src.leader));
							pk::play_se(0x39, pos);
							pk::create_effect(0x3a, pos);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("给敌军一些苦头吃!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("击敌以弱，轻而易举."), pk::get_person(src.leader)); break;
							}
						}
						pk::history_log(src.pos, pk::get_force(src.get_force_id()).color, pk::encode(pk::format("因为\x1b[1x{}\x1b[0x的活跃，周围敌军军力损失\x1b[1x少量兵力\x1b[0x！", pk::decode(pk::get_name(src)))));
					}
					else // 부정적 효과  
					{
						array<pk::point> arr = pk::range(pos, 1, 3);

						for (int j = 0; j < int(arr.length); j++)
						{
							pk::unit@ dst = pk::get_unit(arr[j]);

							if (dst is null or pk::is_enemy(src, dst)
								or (troops_제약조건 >= dst.troops)) continue;

							ch::add_troops(dst, -troops_피해[pk::rand(5)], true, 100);

							// 병력 0 이 되면 부대 궤멸
							if (dst.troops == 0) { pk::kill(dst); }

						}

						if ((src_leader.stat[武将能力_统率] >= 80) or (src_leader.stat[武将能力_智力] >= 80))
						{
							pk::add_kouseki(src, -공적하락 * 2);
						}
						else
						{
							pk::add_kouseki(src, -공적하락);
						}

						if (troops_제약조건 < src.troops)
						{
							ch::add_troops(src, -troops_피해[pk::rand(5)], true, 100);	
						}

						if (pk::is_in_screen(pos))
						{
							pk::play_voice(0x37, pk::get_person(src.leader));
							pk::play_se(0x39, pos);
							pk::create_effect(0x3a, pos);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("呃!因为我的失误我军受到了很大的损失.."), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("犯了一个不可饶恕的错误呀.."), pk::get_person(src.leader)); break;
							}
						}
						//因为部队可能提前被杀，所以提前获取信息
						pk::point src_pos = src.pos;
						string src_name = pk::decode(pk::get_name(src));
						int force_color = pk::get_force(src.get_force_id()).color;
						pk::history_log(src_pos, force_color, pk::encode(pk::format("因为\x1b[1x{}\x1b[0x的失误，周围友军军力损失\x1b[1x少量兵力\x1b[0x！", src_name)));
						// 병력 0 이 되면 부대 궤멸
						if (src.troops == 0) { pk::kill(src); }
					}

				}

			}

		} // commander_Type5


		// 기력 감소 
		void commander_Type6()
		{
			auto list = pk::list_to_array(pk::get_unit_list());

			for (int i = 0; i < int(list.length); i++)
			{

				pk::unit@ src = list[i];
				pk::point pos = src.get_pos();

				pk::person@ src_leader = pk::get_person(src.leader);

				if (only_AI_unit(src) and ch::rand_bool_1000(효과발생확률))
				{

					if (pk::rand_bool(행운확률)) // 긍정적 효과 
					{
						array<pk::point> arr = pk::range(pos, 1, 3);

						for (int j = 0; j < int(arr.length); j++)
						{
							pk::unit@ dst = pk::get_unit(arr[j]);

							if (dst is null or !pk::is_enemy(src, dst)
								or (energy_제약조건 >= dst.energy)) continue;

							pk::add_energy(dst, -energy_제약조건, true);

						}

						if ((src_leader.stat[武将能力_统率] < 80) and (src_leader.stat[武将能力_智力] < 80))
						{
							pk::add_kouseki(src, 공적상승 * 2);
						}
						else
						{
							pk::add_kouseki(src, 공적상승);
						}

						if (pk::is_in_screen(pos))
						{
							pk::play_voice(0x37, pk::get_person(src.leader));
							pk::play_se(0x39, pos);
							pk::create_effect(0x3a, pos);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("打掉敌人的士气!"), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("给敌人显示我军的威风!"), pk::get_person(src.leader)); break;
							}
						}
						pk::history_log(src.pos, pk::get_force(src.get_force_id()).color, pk::encode(pk::format("因为\x1b[1x{}\x1b[0x的奋起，周围敌军\x1b[1x气势大降\x1b[0x！", pk::decode(pk::get_name(src)))));

					}
					else // 부정적 효과 
					{
						array<pk::point> arr = pk::range(pos, 1, 3);

						for (int j = 0; j < int(arr.length); j++)
						{
							pk::unit@ dst = pk::get_unit(arr[j]);

							if (dst is null or pk::is_enemy(src, dst)
								or (energy_제약조건 >= dst.energy)) continue;

							pk::add_energy(dst, -energy_감소, true);

						}

						if ((src_leader.stat[武将能力_统率] >= 80) or (src_leader.stat[武将能力_智力] >= 80))
						{
							pk::add_kouseki(src, -공적하락 * 2);
						}
						else
						{
							pk::add_kouseki(src, -공적하락);
						}

						if (energy_제약조건 < src.energy)
						{
							pk::add_energy(src, -energy_감소, true);
						}

						if (pk::is_in_screen(pos))
						{
							pk::play_voice(0x37, pk::get_person(src.leader));
							pk::play_se(0x39, pos);
							pk::create_effect(0x3a, pos);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("因为我的失误，士气大减.."), pk::get_person(src.leader)); break;
							case 1: pk::say(pk::encode("倒被敌军气势给镇住了.."), pk::get_person(src.leader)); break;
							}
						}
						pk::history_log(src.pos, pk::get_force(src.get_force_id()).color, pk::encode(pk::format("因为\x1b[1x{}\x1b[0x的失误，周围友军\x1b[1x气势大降\x1b[0x！", pk::decode(pk::get_name(src)))));
					}

				}

			}

		} // commander_Type6


	} // class Main

	Main main;

} // namespace
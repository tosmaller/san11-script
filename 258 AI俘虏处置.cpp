// ## 2021/11/14 # 江东新风 # 修改俘虏处置的释放条件，应该能使ai更倾向于俘虏武将 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/26 #江东新风#更新prisoner_info→destroy_info##
// ## 2021/02/22 #江东新风#为登用概率添加调用类型##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/23 #江东新风#修正战死为无时不处决的开关逻辑错误的问题##
// ## 2020/09/21 #江东新风#增加战死为无时不处决的开关##
// ## 2020/08/06 #江东新风#仇人被?不再必死，而是增加60%处決几率##
// ## 2020/08/01 ##
namespace AI_JUDGEMENT
{
	const bool 战死为无时不处决 = true;
	class Main
	{
		Main()
		{
			pk::set_func(258, pk::func258a_t(callback));
		}
		//经测试，ai俘虏处置只有一种调用，所以是可以做对应剧情和log的
		int callback(pk::person@ prisoner, const pk::destroy_info& in info)
		{
			pk::person@ judge = pk::get_person(pk::get_kunshu_id(info.attacker));
			pk::district@ district = pk::get_district(prisoner.district);
			pk::force@ force = pk::get_force(district.force);

			if (func_4ae3a0(prisoner, judge) and (pk::get_scenario().battle_death != 战死_无 or !战死为无时不处决))
			{
				if (force.player < 0)
				{
					if (prisoner.mibun == 身份_君主)
					{
						pk::play_se(5);
						pk::message_box(pk::get_msg(pk::msg_param(5929, prisoner, judge)), prisoner);
					}

					if (prisoner.mibun != 身份_君主)
						pk::play_se(3);

					pk::history_log(judge.get_force_id(), force.color, pk::encode(pk::format("成为俘虏的\x1b[1x{}\x1b[0x被\x1b[2x{}\x1b[0x处死", pk::decode(pk::get_name(prisoner)), pk::decode(pk::get_name(judge)))));
				}

				return 俘虏待遇_处决;
			}
			//新增的更改，在func111里加了返回地址，只能查看，不能修改
			if (cast<pk::func111_t>(pk::get_func(111))(prisoner, judge, info.releasable ? 1 : 2, 0, 0))
			{
				if (prisoner.mibun == 身份_君主 && force.player < 0)
				{
					pk::play_se(2);
					pk::message_box(pk::get_msg(pk::msg_param(5881, prisoner, judge)), prisoner);
					pk::history_log(judge.get_force_id(), force.color, pk::encode(pk::format("被捕获的\x1b[1x{}\x1b[0x成为\x1b[2x{}\x1b[0x的属下", pk::decode(pk::get_name(prisoner)), pk::decode(pk::get_name(judge)))));
				}
				return 俘虏待遇_登用;
			}

			if (func_4af5b0(prisoner, judge) and (pk::get_scenario().battle_death != 战死_无 or !战死为无时不处决))
			{
				if (force.player < 0)
				{
					if (prisoner.mibun == 身份_君主)
					{
						pk::play_se(5);
						pk::message_box(pk::get_msg(pk::msg_param(5929, prisoner, judge)), prisoner);
					}

					if (prisoner.mibun != 身份_君主)
						pk::play_se(3);

					pk::history_log(judge.get_force_id(), force.color, pk::encode(pk::format("成为俘虏的\x1b[1x{}\x1b[0x被\x1b[2x{}\x1b[0x处死", pk::decode(pk::get_name(prisoner)), pk::decode(pk::get_name(judge)))));
				}

				return 俘虏待遇_处决;
			}

			if (prisoner.mibun == 身份_君主)
			{
				if (force.player < 0)
				{
					pk::play_se(6);
					pk::message_box(pk::get_msg(pk::msg_param(5917, prisoner, judge)), prisoner);
					pk::history_log(judge.get_force_id(), force.color, pk::encode(pk::format("成为俘虏的\x1b[1x{}\x1b[0x被\x1b[2x{}\x1b[0x释放", pk::decode(pk::get_name(prisoner)), pk::decode(pk::get_name(judge)))));
					pk::add_relations(force, judge.get_force_id(), 50);
				}

				int random = pk::rand(10);
				if (pk::get_aishou_distance(prisoner, judge.get_id()) <= 5 && random < prisoner.giri)
				{
					pk::add_like(prisoner, judge.get_id());
				}

				return 俘虏待遇_释放;
			}

			if (!func_4b0740(info))
			{
				int random = pk::rand(8);
				if (pk::get_aishou_distance(prisoner, judge.get_id()) <= 5 && random < prisoner.giri)
				{
					pk::add_like(prisoner, judge.get_id());
				}

				return 俘虏待遇_释放;
			}

			return 俘虏待遇_俘虏;
		}

		/***/
		bool func_4ae3a0(pk::person@ prisoner, pk::person@ judge)
		{
			if (!pk::is_alive(prisoner) or !pk::is_alive(judge))
				return false;

			// 포로를 혐오하는 경우 처단对仇人必然处決,修改此处，移动到后方，设定?仇人增加大量处決几率
			/*if (pk::is_dislike(judge, prisoner.get_id()))
				return true;
			*/

			if (prisoner.mibun == 身份_君主)
			{
				pk::force@ judge_force = pk::get_force(judge.get_force_id());
				pk::force@ prisoner_force = pk::get_force(prisoner.get_force_id());

				if (!pk::is_alive(judge_force) or !pk::is_alive(prisoner_force))
					return false;

				int random = pk::rand(10);

				// 如果?虏是皇帝，裁決方又拥护汉帝，则会处決?虏포로가 황제이고 내가 후한 황제를 보호중일 경우 처단
				if (prisoner_force.title == 爵位_皇帝 and pk::is_protecting_the_emperor(judge_force) and judge.giri <= random)
					return true;

				// 如果?虏拥护汉帝，裁決方是皇帝，则会处決?虏 내가 황제이고 포로가 후한 황제를 보호중일 경우 처단
				if (pk::is_protecting_the_emperor(prisoner_force) and judge_force.title == 爵位_皇帝 and judge.giri <= random)
					return true;
			}

			return false;
		}

		/***/
		bool func_4af5b0(pk::person@ prisoner, pk::person@ judge)
		{
			int prisoner_id = prisoner.get_id();

			// 의형제인 경우 처단하지 않음(pk::is_gikyoudai 함수를 사용하는 것이 맞다고 생각함)
			//if (prisoner.gikyoudai == judge.gikyoudai) return false;
			if (pk::is_gikyoudai(prisoner, judge.get_id())) return false;

			// 부부인 경우 처단하지 않음
			if (pk::is_fuufu(judge, prisoner_id)) return false;

			// 포로를 친애하는 경우 처단하지 않음
			if (pk::is_like(judge, prisoner_id)) return false;

			int max_stat = 0;
			int avg_stat = 0;
			int death = 10;
			int 仇人处決几率 = 60;

			for (int i = 0; i < 武将能力_末; i++)
			{
				int s = prisoner.stat[i];
				max_stat = pk::max(max_stat, s);
				avg_stat = avg_stat + s;
			}
			avg_stat = avg_stat / 武将能力_末;

			//此处有問題，无战死死亡率高？
			switch (pk::get_scenario().battle_death)
			{
			case 전사_없음: death = 20; break;
			case 전사_표준: death = 10; break;
			case 전사_많음: death = 0; break;
			}

			int d_aishou = pk::get_aishou_distance(prisoner, judge.get_id());

			int n = 0;

			if (prisoner.mibun == 身份_君主)
			{
				if (d_aishou > 70)
					n += 20;
				else if (d_aishou > 50)
					n += 10;
				else if (d_aishou > 30)
					n += 5;
				else if (d_aishou > 10)
					n -= 5;
				else if (d_aishou > 5)
					n -= 10;
				else
					n -= 20;

				n += (judge.ambition + prisoner.ambition) * 8;
				n -= (pk::max(max_stat, 50) + pk::max(avg_stat, 50)) / 2;
				n += (20 - (judge.giri * 4)) * 5;
				n -= (judge.promotion == 기용_의리 ? prisoner.giri * 4 : prisoner.giri * -4);
				n -= death;
				n -= pk::rand(20);
				n += judge.stat[1] > judge.stat[2] ? (judge.stat[1] - judge.stat[2]) : 0;
				if (pk::is_dislike(judge, prisoner.get_id())) n += 仇人处決几率;
				n = pk::max(n, 0);
				n = pk::min(n, 100);

			}
			else
			{
				if (d_aishou > 70)
					n += 20;
				else if (d_aishou > 50)
					n += 10;
				else if (d_aishou > 30)
					n += 5;
				else if (d_aishou > 10)
					n -= 5;
				else if (d_aishou > 5)
					n -= 10;
				else
					n -= 20;

				n += 120 - (pk::max(max_stat, 50) + pk::max(avg_stat, 50)) / 2;
				n -= int(pk::max(prisoner.kouseki, 20000) / 2000 * (judge.promotion == 기용_실적 ? 1.5f : 1));
				n -= prisoner.ambition * (judge.promotion == 기용_의리 ? 5 : 1);
				n -= int(pk::rand(25) * (judge.promotion == 기용_임의 ? 1.5f : 1));
				n -= judge.giri * 10;
				n -= death;
				if (pk::is_dislike(judge, prisoner.get_id())) n += 仇人处決几率;
				n = pk::max(n, 0);
				n = pk::min(n, 100);
			}

			return pk::rand_bool(n);
		}

		/*true 不释放，false 释放**/
		bool func_4b0740(const pk::destroy_info& in info)
		{
			// 멸망시킨경우 모두 해방
			if (!info.releasable) return false;

			// 거점을 공격해 포로를 잡은 경우 攻击据点抓到了俘虏
			if (info.target.is_instance(pk::building::type_id))//原来是判定attacker是不是建筑，感觉不对，改成target了
			{
				// 부대가 잡은게 아니라면 해방 如果不是部队抓的，释放
				if (!info.attacker.is_instance(pk::unit::type_id)) return false;

				pk::building@ building = pk::hex_object_to_building(info.target);//既然攻击者不能是建筑，那这个意义呢？要不改成目标好了，反而更好理解（不过占领后建筑金其实会减少）
				pk::unit@ unit = pk::hex_object_to_unit(info.attacker);
				int pay = 0;

				// 부대 무장들의 봉록 합 武将俸禄计算
				for (int i = 0; i < 3; i++)
				{
					pk::person@ member = pk::get_person(unit.member[i]);
					if (pk::is_alive(member) and member.mibun >= 身份_都督 and member.mibun <= 身份_一般)
					{
						pk::rank@ rank = pk::get_rank(member.rank);
						if (pk::is_alive(rank))
							pay += rank.pay;
					}
				}

				// 부대가 잡고 있는 포로 비용 합 部队抓的俘虏费用
				pay += (pk::get_person_list(pk::get_hex_object_id(unit), pk::mibun_flags(身份_俘虏)).count + 2) * 50;

				pay *= 2;

				// 금이 부족하다면 해방 如果金不够，释放
				// 加入对象是玩家时，更加倾向于不释放
				if (info.target.is_player()) { if (unit.gold + pk::get_gold(building) < int(0.8f * pay)) return false; }
				else if (unit.gold + pk::get_gold(building) < pay) return false;
			}
			// 부대를 공격해 포로를 잡은 경우 攻击部队抓俘虏
			else
			{
				pk::person@ taken_by;
				pk::building@ service;
				int pay = 0;

				if (info.attacker.is_instance(pk::unit::type_id))
					@taken_by = pk::get_person(pk::hex_object_to_unit(info.attacker).leader);
				else if (info.attacker.is_instance(pk::building::type_id))
					@taken_by = pk::get_person(pk::get_taishu_id(pk::hex_object_to_building(info.attacker)));
				else
					@taken_by = pk::get_person(pk::get_kunshu_id(info.attacker));

				if (pk::is_alive(taken_by))
					@service = pk::get_building(taken_by.service);

				// 포로를 잡은 무장의 소속이 없다면 해방
				if (!pk::is_alive(service)) return false;

				// 소속 거점 무장들의 봉록 합
				pk::list<pk::person@> list = pk::get_person_list(service, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般));
				for (int i = 0; i < list.count; i++)
				{
					pk::rank@ rank = pk::get_rank(list[i].rank);
					if (pk::is_alive(rank))
						pay += rank.pay;
				}

				// 소속 거점 포로 비용 합
				pay += (pk::get_person_list(pk::get_hex_object_id(service), pk::mibun_flags(신분_포로)).count + 2) * 50;

				pay *= 2;

				// 소속 거점으로 귀환했을 때 포로를 유지할 수 있는 비용이 부족하다면 해방
				// 加入对象是玩家时，更加倾向于不释放
				if (info.target.is_player()) { if (pk::get_gold(service) < int(0.8f * pay)) return false; }
				else if (pk::get_gold(service) < pay) return false;
			}

			return true;
		}
	}

	Main main;
}
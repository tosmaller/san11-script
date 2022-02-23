// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/08/01 ##


namespace SOLICIT_SURRENDER_CHANCE
{
	int 劝降_保护期 = 18;  //AI 세력의 항복권고 재사용대기시간
	bool AI禁止劝降 = false;

	class Main
	{
		Main()
		{
			pk::set_func(113, pk::func113_t(callback));
			pk::bind(103, pk::trigger103_t(callbackStart));
			pk::bind(111, pk::trigger111_t(callbackTurnStart));
		}

		array<array<int>> surrender_index;

		void callbackStart()
		{
			surrender_index.removeRange(0, surrender_index.length);
		}

		void callbackTurnStart(pk::force@ force)
		{
			pk::person@ kunshu = pk::get_person(force.kunshu);
			pk::solicit_surrender_cmd_info cmdd;
			pk::solicit_cmd_info cmd;

			if (pk::is_iminzoku(kunshu) or pk::is_bandit(kunshu))
				return;
			if (force.player >= 0)
				return;

			for (int i = 0; i<int(surrender_index.length); i++)
			{
				if (force.get_id() == surrender_index[i][0])
				{
					if (surrender_index[i][1] > 0)
					{
						surrender_index[i][1]--;
						return;
					}
					else
						surrender_index.removeAt(i);
				}
			}

			for (int i = 세력_끝 - 1; i >= 0; i--)
			{
				pk::force@ target_force = pk::get_force(i);
				pk::person@ target_kunshu = pk::get_person(target_force.kunshu);
				if (pk::is_alive(target_force) && force.player < 0 && force.get_id() != target_force.get_id() && pk::is_iminzoku(target_kunshu) == false && pk::is_bandit(target_kunshu) == false)
				{
					for (int j = 0; j < 무장_끝; j++)
					{
						pk::person@ person = pk::get_person(j);
						if (person.get_force_id() == force.get_id() && pk::is_alive(person) && kunshu.district == person.district && person.action_done == false && person.order < 0 && pk::get_unit_id(person) < 0)
						{
							@cmd.actor = person;
							@cmd.target = target_force;
							cmd.gold = 0;
							pk::int_bool ib = callback(cmd);
							int random = pk::rand(100);

							if (ib.first == 0 && int(kunshu.stat[武将能力_智力]) > random)
							{
								pk::history_log(pk::get_current_turn_force_id(), force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x势力的\x1b[1x{}\x1b[0x对\x1b[2x{}\x1b[0x势力进行投降劝告", pk::decode(pk::get_name(kunshu)), pk::decode(pk::get_name(person)), pk::decode(pk::get_name(target_kunshu)))));
								@cmdd.actor = person;
								@cmdd.target = target_force;
								cmdd.gold = 0;
								pk::command(cmdd);

								array<int> arr;
								arr.insertLast(force.get_id());
								arr.insertLast(劝降_保护期);
								surrender_index.insertLast(arr);
								return;
							}
						}
					}
				}
			}


		}

		pk::int_bool callback(const pk::solicit_cmd_info& in info)
		{
			bool accurate = true;
			pk::person@ target_kunshu = pk::get_person(info.target.kunshu);
			pk::force@ actor_force = pk::get_force(info.actor.get_force_id());

			if (AI禁止劝降 && !actor_force.is_player()) return pk::int_bool(2, accurate);

			// 1대1 상황일 경우 항상 실패
			if (pk::get_force_list().count <= 2) return pk::int_bool(2, accurate);

			if (!pk::is_alive(actor_force)) return pk::int_bool(2, accurate);

			// 시나리오 개시 2년 미만인 경우 항상 실패
			if (pk::get_elapsed_years() < 2) return pk::int_bool(2, accurate);

			// 실행 무장 세력의 도시수가 목표 세력 도시수의 8배 미만일 경우 항상 실패
			if (pk::get_city_count(actor_force) < pk::get_city_count(info.target) * 8) return pk::int_bool(2, accurate);

			// 실행 무장 세력의 병력수가 목표 세력 병력수의 6배 미만일 경우 항상 실패
			if (pk::get_troops(actor_force) < pk::get_troops(info.target) * 6) return pk::int_bool(2, accurate);

			// 목표 세력의 도시가 3개 이상인 경우 항상 실패
			if (pk::get_city_count(info.target) >= 3) return pk::int_bool(2, accurate);

			if (!pk::is_alive(target_kunshu)) return pk::int_bool(2, accurate);

			// 인접한 도시가 없다면 항상 실패
			if (!pk::is_neighbor_force(actor_force, info.target)) return pk::int_bool(2, accurate);

			// 목표 세력 군주가 실행 무장의 군주를 혐오한다면 항상 실패
			if (pk::is_dislike(target_kunshu, pk::get_kunshu_id(info.actor))) return pk::int_bool(2, accurate);

			// 목표 세력 군주가 실행 무장을 혐오한다면 항상 실패
			if (pk::is_dislike(target_kunshu, info.actor.get_id())) return pk::int_bool(2, accurate);

			int k = info.target.kunshu;

			// 절대 항복하지 않는 군주
			if (k == 무장_손권 or k == 무장_여포 or k == 무장_동탁 or k == 무장_장각 or k == 무장_유비 or k == 무장_관우 or k == 무장_장비 or k == 무장_손견 or k == 무장_손책 or k == 무장_조조) return pk::int_bool(2, true);

			if (pk::get_city_count(info.target) <= 0) return pk::int_bool(2, accurate);

			int n = 0;
			int g = 0;
			int difficulty = pk::get_scenario().difficulty;

			n += (pk::get_city_count(actor_force) / pk::get_city_count(info.target)) / 2;
			n += (pk::get_city_count(actor_force) - pk::get_city_count(info.target)) / 5;
			n -= pk::get_aishou_distance(target_kunshu, info.actor.get_id()) / 7;
			n -= pk::get_aishou_distance(target_kunshu, pk::get_kunshu_id(info.actor));
			n += pk::is_protecting_the_emperor(actor_force) ? 10 : 0;
			n += func_5b3f40(info.actor, target_kunshu, 0, 10, 20);
			n += info.actor.stat[武将能力_魅力];
			n += pk::get_city_count(actor_force) / pk::get_city_count(info.target) * 5;
			n += pk::get_troops(actor_force) / pk::get_troops(info.target) * 5;
			if (difficulty == 难易度_上级)
				n = int(n * 0.8f);
			else if (difficulty == 难易度_超级)
				n = int(n * 0.7f);
			n /= 2;
			n += pk::rand(n, info.gold, info.target.get_id(), info.actor.get_id(), info.actor.service, 0, 0, 0);
			g = target_kunshu.character * 7 - info.target.relations[actor_force.get_id()] / 2;
			g += target_kunshu.ambition * 10;

			if (pk::is_like(target_kunshu, pk::get_kunshu_id(info.actor)))
				n += 30;



			if (n * 2 >= g + 100) accurate = false;

			if (n >= g + 100) return pk::int_bool(0, accurate);

			if (n >= g + 90) return pk::int_bool(1, accurate);

			return pk::int_bool(2, accurate);
		}

		/**
			실행무장과 목표세력 군주의 관계에 따른 보너스
		*/
		int func_5b3f40(pk::person@ actor, pk::person@ target_kunshu, int bad, int good, int best)
		{
			int actor_id = actor.get_id();
			if (pk::is_gikyoudai(target_kunshu, actor_id) or pk::is_fuufu(target_kunshu, actor_id)) return best;
			if (pk::is_like(target_kunshu, actor_id)) return good;
			if (pk::is_dislike(target_kunshu, actor_id)) return bad;
			return 0;
		}
	}

	Main main;
}
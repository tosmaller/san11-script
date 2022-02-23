// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/10/23 #江东新风#同步马术书大神的更新##
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/07/26 ##
namespace ALLIANCE_CHANCE
{
	class Main
	{
		Main()
		{
			pk::set_func(112, pk::func112_t(callback));
		}

		pk::int_bool callback(const pk::alliance_cmd_info& in info)
		{
			bool accurate = true;
			pk::person@ target_kunshu = pk::get_person(info.target.kunshu);

			if (!pk::is_alive(target_kunshu)) return pk::int_bool(2, accurate);

			int actor_force_id = info.actor.get_force_id();
			int target_force_id = info.target.get_id();

			// 포로가 있다면 항상 실패---如果存在对方势力俘虏，总是失败
			if (func_5be0e0(actor_force_id, target_force_id) or func_5be0e0(target_force_id, actor_force_id)) return pk::int_bool(2, accurate);

			// 실행 무장이 논객 특기를 보유하고 있지 않고 목표 세력 군주가 실행 세력 군주를 혐오한다면 항상 실패
			// 如果执行武将没有论客，对方君主厌恶己方执行武将时，总是失败
			if (!ch::has_skill(info.actor, 特技_论客) and pk::is_dislike(target_kunshu, pk::get_kunshu_id(info.actor))) return pk::int_bool(2, accurate);

			int n = 0;
			int g = 0;
			int difficulty = pk::get_scenario().difficulty;

			n += int(func_5b3130(info.actor, target_kunshu, -1, 1, 1.5) * 10);
			n += int(info.gold / 150);
			n -= int(pk::get_aishou_distance(target_kunshu, info.actor.get_id()) / 5);
			n += int(info.actor.stat[武将能力_政治]);
			if (difficulty == 难易度_上级)
				n = int(n * 0.8f);
			else if (difficulty == 难易度_超级)
				n = int(n * 0.7f);

			g = (110 - info.target.relations[actor_force_id]) * 2;

			if (n * 2 > g) accurate = false;

			if (n > g) return pk::int_bool(0, accurate);

			// 실행 무장이 논객 특기를 보유하고 있음
			if (ch::has_skill(info.actor, 特技_论客))
			{
				// 특급 모드 플레이어일 경우에만 20%, 나머지 100% 설전
				// if (difficulty != 难易度_超级 or !info.actor.is_player() or pk::randbool(20)) return pk::int_bool(1, accurate);
				if (!info.actor.is_player() or pk::rand_bool(int(pk::core::skill_constant(info.actor.get_id(), 特技_论客, difficulty)))) return pk::int_bool(1, accurate);
			}

			g = (100 - info.target.relations[actor_force_id]) * 2;

			if (n > g) return pk::int_bool(1, accurate);

			return pk::int_bool(2, accurate);
		}

	}

	Main main;

	/**
		실행무장과 목표세력 군주의 관계에 따른 보너스
	*/
	float func_5b3130(pk::person@ actor, pk::person@ target_kunshu, float bad, float good, float best)
	{
		int actor_id = actor.get_id();
		if (pk::is_gikyoudai(target_kunshu, actor_id) or pk::is_fuufu(target_kunshu, actor_id)) return best;
		if (pk::is_like(target_kunshu, actor_id)) return good;
		if (pk::is_dislike(target_kunshu, actor_id)) return bad;
		return 0;
	}

	/**
		src 세력이 dst 세력 무장을 포로로 잡고 있는지 확인
	*/
	bool func_5be0e0(int src, int dst)
	{
		pk::list<pk::person@> list = pk::get_person_list(pk::mibun_flags(신분_포로));
		for (int i = 0; i < list.count; i++)
		{
			pk::person@ person = list[i];
			if (person.former_force == dst)
			{
				pk::hex_object@ hexobj = pk::get_hex_object(person.location);
				if (pk::is_alive(hexobj) and hexobj.get_force_id() == src)
					return true;
			}
		}
		return false;
	}
}
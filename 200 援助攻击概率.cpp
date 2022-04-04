// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/08/14 #江东新风#将光环系统的援助攻击光环放入此处##
// ## 2020/07/26 ##
namespace ASSIST_CHANCE
{
	//---------------------------------------------------------------------------
	const int 关系援助_仲介 = 50;
	const int 关系援助_亲爱 = 30;
	const int 关系援助_血缘 = 20;
	const int 辅佐援助 = 30;

	//---------------------------------------------------------------------------
	class Main
	{
		Main()
		{
			pk::set_func(200, pk::func200_t(callback));
		}

		int callback(pk::unit@ assister, pk::unit@ attacker, pk::unit@ target)
		{
			pk::person@ assister_leader = pk::get_person(assister.leader);
			pk::person@ assister_deputy1 = pk::get_person(assister.member[1]);
			pk::person@ assister_deputy2 = pk::get_person(assister.member[2]);

			int attacker_leader = attacker.leader;

			// 무장.xml 설정값
			int bonus = pk::core::assist_chance(assister.leader);

			//援助攻击光环判断
			int halo_bonus = 0;
			if ((基础光环) and (辅助攻击光环))
			{
				int dst_id = halo::func_get_highest_halo(assister, 5);
				if (dst_id != -1)
				{
					auto dst = pk::get_unit(dst_id);
					halo_bonus = halo::func_光环效果(dst, 5);
				}
			}


			// 배우자나 의형제일 경우 50%
			if (pk::is_fuufu(assister_leader, attacker_leader) or pk::is_gikyoudai(assister_leader, attacker_leader))
				return int(pk::max(0, pk::min(99, 关系援助_仲介 + bonus)));


			/*
				[원본] 보좌 특기일 경우 혐오무장만 아니면 30% (주장)
				if (ch::has_skill(assister_leader, 특기_보좌) and !pk::is_dislike(assister_leader, attacker_leader))
				{
					return 30 + bonus;
				}
			*/

			// 보좌 특기일 경우 혐오무장만 아니면 30% (주장 or 부장) 
			if (ch::has_skill(assister, 특기_보좌))
			{
				if (!pk::is_dislike(pk::get_person(assister.who_has_skill(특기_보좌)), attacker_leader))
				{
					return int(pk::max(0, pk::min(99, 辅佐援助 + bonus + halo_bonus)));
				}
			}


			// 친애무장일 경우 30%
			if (pk::is_like(assister_leader, attacker_leader))
				return int(pk::max(0, pk::min(99, 关系援助_亲爱 + bonus + halo_bonus)));

			// 혈연이고 혐오무장이 아닐경우 20%
			if (pk::is_ketsuen(assister_leader, attacker_leader) and !pk::is_dislike(assister_leader, attacker_leader))
				return int(pk::max(0, pk::min(99, 关系援助_血缘 + bonus + halo_bonus)));

			return int(pk::max(0, pk::min(99, 0 + bonus + halo_bonus)));
		}
	}

	Main main;
}
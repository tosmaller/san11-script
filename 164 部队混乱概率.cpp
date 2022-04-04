// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/12/21 # messi # 参考恫吓添加智力版恫吓新特技扰敌 ##
// ## 2020/10/23 #江东新风# 同步马术书大神的更新 ##
// ## 2020/10/08 #江东新风#特技_恫吓常量修改##
// ## 2020/08/27 #江东新风#特技_恫吓及自动混乱情況下的治军效果##
// ## 2020/08/15 #江东新风#修复特技_恫吓无效bug##
// ## 2020/08/14 #江东新风#特技_恫吓##
// ## 2020/07/26 ##
namespace UNIT_CONFUSION
{
	class Main
	{
		Main()
		{
			pk::set_func(164, pk::func164_t(callback));
		}

		int callback(pk::unit@ unit)
		{
			//治军判断，不会混乱
			if (ch::has_skill(unit, 特技_治军))
				return 0;
			//donghe 特技，huo取周wei坐biao
			auto arr = pk::ring(unit.pos, 1);
			//ch::u8debug(pk::format("hunluan panduan {} ", unit.get_id()));
			for (int i = 0; i < int(arr.length); i++)
			{

				pk::unit@ dst = pk::get_unit(arr[i]);


				if (dst !is null)
				{
					int dst_id = dst.get_id();
					//ch::u8debug(pk::format("dst {} ", dst_id));
					if (ch::has_skill(dst, 特技_恫吓) and dst.status == 部队状态_通常 and unit.status != 部队状态_混乱 and pk::is_enemy(unit, dst))
					{
						//ch::u8debug(pk::format("dst {} has skill 134 ", dst_id));
						int dst_stat = ch::get_best_member_stat(dst, 特技_恫吓, 武将能力_武力);
						int src_stat = ch::get_best_member_stat(unit, 武将能力_武力);
						if (dst_stat > src_stat)
						{
							//ch::u8debug(pk::format("dst {} src {}", dst_stat, src_stat ));
							//混乱率是武力差
							if (pk::rand_bool(dst_stat - src_stat))
								return 100;
							//pk::set_status(dst, src, 部队状态_混乱, pk::rand(1)+1, true);
						}
					}
				}
				if (dst !is null)
				{
					int dst_id = dst.get_id();
					//ch::u8debug(pk::format("dst {} ", dst_id));
					if (ch::has_skill(dst, 特技_扰敌) and dst.status == 部队状态_通常 and unit.status != 部队状态_混乱 and pk::is_enemy(unit, dst))
					{
						//ch::u8debug(pk::format("dst {} has skill 134 ", dst_id));
						int dst_stat = ch::get_best_member_stat(dst, 特技_扰敌, 武将能力_智力);
						int src_stat = ch::get_best_member_stat(unit, 武将能力_智力);
						if (dst_stat > src_stat)
						{
							//ch::u8debug(pk::format("dst {} src {}", dst_stat, src_stat ));
							//混乱率是武力差
							if (pk::rand_bool(dst_stat - src_stat))
								return 100;
							//pk::set_status(dst, src, 部队状态_混乱, pk::rand(1)+1, true);
						}
					}
				}
			}


			int troops = unit.troops;

			// 병력 수가 1000 이상일 경우 회피
			if (troops >= 1000)
				return 0;

			// 부대가 통상 상태가 아닐 경우 회피
			if (unit.status != 部队状态_通常)
				return 0;

			// 병력 수 8% + 10 확률로 회피
			if (pk::rand_bool(pk::min(90, pk::max(10, troops * 8 / 100 + 10))))
				return 0;

			// 통솔 20% + 20 확률로 회피
			if (pk::rand_bool(unit.attr.stat[部队能力_统率] / 5 + 20))
				return 0;

			pk::int_int_int tuple = func_59b130(unit);
			int sum_own_troops = tuple.first;
			int sum_enemy_troops = tuple.second;
			int max_enemy_troops = tuple.third;

			// 적 부대 중 최대 병력 수가 500 미만일 경우 회피
			if (max_enemy_troops < 500)
				return 0;

			// 적 부대 병력 합이 자세력 부대 병력 수의 20% 미만일 경우 60% 확률로 회피
			if (sum_enemy_troops < sum_own_troops / 5)
			{
				if (pk::rand_bool(60))
					return 0;
			}
			// 적 부대 병력 합이 자세력 부대 병력 수의 50% 미만일 경우 40% 확률로 회피
			else if (sum_enemy_troops < sum_own_troops / 2)
			{
				if (pk::rand_bool(40))
					return 0;
			}

			// 병력 수를 20배한 값이 적 부대 중 최대 병력 수보다 높고, 병력 수를 40배한 값이 적 부대 병력 합보다 높다면 회피
			if (troops * 20 >= max_enemy_troops and troops * 40 >= sum_enemy_troops)
				return 0;

			return 100;
		}

		pk::int_int_int func_59b130(pk::unit@ src)
		{
			// 인접 자세력 부대 병력 합
			int sum_own_troops = 0;
			// 3칸 이하 적 부대 병력 합
			int sum_enemy_troops = 0;
			// 3칸 이하 적 부대 중 최대 병력
			int max_enemy_troops = 0;

			array<pk::point> arr = pk::range(src.get_pos(), 1, 3);
			for (int i = 0; i < int(arr.length); i++)
			{
				pk::unit@ dst = pk::get_unit(arr[i]);
				if (dst is null or dst.status != 部队状态_通常)
					continue;

				if (pk::is_enemy(src, dst))
				{
					int troops = dst.troops;
					sum_enemy_troops = sum_enemy_troops + troops;
					max_enemy_troops = pk::max(max_enemy_troops, troops);
				}
				else if (pk::is_own(src, dst))
				{
					if (pk::is_neighbor_pos(src.get_pos(), dst.get_pos()))
						sum_own_troops = sum_own_troops + dst.troops;
				}
			}

			return pk::int_int_int(sum_own_troops, sum_enemy_troops, max_enemy_troops);
		}
	}

	Main main;
}
// ## 2021/11/10 # 江东新风 # 加入目标是玩家时，要求兵力数更多 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/26 # 江东新风 # 增加影响条件备注 ##
// ## 2020/07/26 ##
namespace AI_ATTACK_TROOPS_MIN_REQUIRED
{
	class Main
	{
		Main()
		{
			pk::set_func(253, pk::func253_t(callback));
		}

		int callback(pk::building@ src, int src_rank, pk::building@ dst, int dst_troops)
		{
/*
根据原始算式，兵力再多也只达到目标据点兵力的2倍，势力越弱，君主性格越大胆，出征兵力就越少。以敌方兵力1000计算
势力排名百分率(强 <-> 弱)
君主性格 100     80     60       40     20      2(最小)

小心	1950	1850	1750	1650	1550	1460
冷静	1800	1700	1600	1500	1400	1310
刚胆	1650	1550	1450	1350	1250	1160
莽撞	1500	1400	1300	1200	1100	1010

*/
//所以君主越小心出兵越多，越莽撞，出兵越少。己方国力排名越强(兵力)，出兵越多，越低，出兵越少。己方国力排名越强

			int a = pk::get_person(pk::get_kunshu_id(src)).character;
			int b = (3 - a) * 15;
			int c = src_rank / 2;
			int d = dst_troops + (dst_troops * (b + c) / 100);
			if (dst.is_player()) d = pk::clamp(int(d*1.5f),d,pk::get_troops(src));
			return pk::max(d, 5000);
		}
	}

	Main main;
}
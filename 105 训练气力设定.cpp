// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2020/10/23 #江东新风#新特技 调练##
// ## 2020/07/26 ##
namespace DRILL
{
	class Main
	{
		Main()
		{
			pk::set_func(105, pk::func105_t(callback));
		}

		int callback(pk::building@ building, const pk::detail::arrayptr<pk::person@>& in actors)
		{
			if (!pk::is_alive(building) or actors[0] is null)
				return 0;

			float n = 0;
			int max = pk::INT32_MIN;
			bool flag = false;

			for (int i = 0; i < int(actors.length); i++)
			{
				pk::person@ actor = actors[i];
				if (pk::is_alive(actor))
				{
					int s = actor.stat[int(pk::core["train.stat"])];
					n += s;
					max = pk::max(max, s);
					if (ch::has_skill(actor, 特技_调练))
						flag = true;
				}
			}
			n = (n + max) / pk::min(building.get_troops() / 2000 + 20, 100) + 3;
			// 연병소가 있다면 1.5배
			if (building.facility == 시설_도시 and pk::has_facility(pk::building_to_city(building), 시설_연병소))
				n = n * 1.5f;
			if (flag)
				n = n * 调练效果倍率 / 100;
			return int(n);
		}
	}

	Main main;
}
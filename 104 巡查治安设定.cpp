// ## 2021/10/10 # 江东新风 # 将函数内容全搬运到305，以便其他cpp调用 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2021/03/05 # 白马叔叔 # 修正巡查特技倍率 ##
// ## 2020/10/27 # 江东新风 # 新te ##
// ## 2020/07/26 ##
namespace INSPECTIONS
{
	class Main
	{
		Main()
		{
			pk::set_func(104, pk::func104_t(callback));
		}

		int callback(pk::building@ building, const pk::detail::arrayptr<pk::person@>& in actors)
		{
			if (actors[0] !is null)//这里判断的话，其他地方调用内政函数还得再内政函数里再判断一次
			{
				pk::list<pk::person@> actor_list;
				for (int i = 0; i < actors.length; i++)
				{
					actor_list.add(actors[i]);
				}

				return inter::get_inspections_order_inc(building, actor_list);
			}
			return 0;
			////以下内容均搬运到305全局函数内政，以方便其他cpp调用
/*
			if (pk::is_alive(building) and actors[0] !is null)
			{
				bool flag = false;
				pk::city@ city = pk::building_to_city(building);
				if (pk::is_alive(city))
				{
					int n = 0;
					for (int i = 0; i < actors.length; i++)
					{
						pk::person@ actor = actors[i];
						if (pk::is_alive(actor))
						{
							n = n + actor.stat[int(pk::core["inspection.stat"])];
							if (ch::has_skill(actor, 特技_巡查))
								flag = true;
						}
					}
					n = n / 28 + 2;
					if (pk::enemies_around(building))
						n = n / 2;
					if (flag)
						n = n * 巡查效果倍率 / 100;
					return int(n);
				}
			}
			return 0;*/
		}		
	}

	Main main;
}
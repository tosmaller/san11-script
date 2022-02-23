// ## 2020/10/01 # 江东新风 ##

namespace 建筑破坏_不损失资源
{
	class Main
	{
		Main()
		{
			pk::set_func(52, pk::func52_t(callback));
		}

		bool callback(pk::city@ city)
		{
			//if (ch::has_skill(pk::city_to_building(city),115)) pk::trace("拥有特技清野-外部");
			return ch::has_skill(pk::city_to_building(city), 特技_清野);
		}

	}

	Main main;
}
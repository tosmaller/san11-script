// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/07/26 ##
namespace UNIT_FOOD_LEFT
{
	class Main
	{
		Main()
		{
			pk::set_func(166, pk::func166_t(callback));
		}

		int callback(int food, int troops)
		{
			float 每旬耗粮 = troops / 1000.f * UNIT_FOOD_USE::基础耗粮;
			return int(food * 10 / 每旬耗粮);
		}
	}

	Main main;
}
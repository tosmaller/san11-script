// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/08/24 # keehl102 # 新版螺旋混乱?率函数 ##
namespace RASEN_CHANCE_2
{
	class Main
	{
		Main()
		{
			pk::set_func(221, pk::func221_t(callback));
		}

		int callback(pk::unit@ attacker, pk::unit@ target, bool critical)
		{
			if (ch::has_skill(target, 特技_治军))
				return 0;
			if (critical or pk::rand_bool(30))
				return pk::rand(2) + (critical ? 1 : 0);
			return 0;
		}
	}

	Main main;
}
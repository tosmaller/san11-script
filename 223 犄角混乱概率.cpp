// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/09/17 # 江东新风 # 完善治军特技 ##
namespace KIKAKU_CHANCE
{
	class Main
	{
		Main()
		{
			pk::set_func(223, pk::func223_t(callback));
		}

		int callback(pk::unit@ attacker, pk::unit@ target)
		{
			if (ch::has_skill(target, 特技_治军))
				return 0;
			if (pk::rand_bool(int(pk::core::skill_constant(attacker, 특기_기각))))
				return 1 + pk::rand(2);
			return 0;
		}
	}

	Main main;
}
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/09/17 # 江东新风 # 完善治军特技 ##
namespace SHISSOU_CHANCE
{
	class Main
	{
		Main()
		{
			pk::set_func(222, pk::func222_t(callback));
		}

		int callback(pk::unit@ attacker, pk::unit@ target)
		{
			//if (ch::has_skill(attacker,49)) //ch::u8debug("攻击方有疾驰");
			if (ch::has_skill(target, 特技_治军))
				return 0;
			if (attacker.attr.stat[部队能力_攻击] > target.attr.stat[部队能力_攻击] and pk::rand_bool(int(pk::core::skill_constant(attacker, 特技_疾驰))))
				return 1 + pk::rand(2);
			return 0;
		}
	}

	Main main;
}
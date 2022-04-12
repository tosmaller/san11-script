// ## 2020/10/01 # 江东新风 ##

namespace 战法_无视地形
{
	class Main
	{
		Main()
		{
			pk::set_func(51, pk::func51_t(callback));
		}

		bool callback(pk::unit@ unit)
		{
			int unit_id = unit.get_id();
			//pk::trace(pk::format("外部测试-巧变判断,部队id{}",unit_id));
			//if (ch::has_skill(unit,125)) pk::trace("拥有特技巧变-外部");
			//else ch::u8debug("未拥有特技巧变-外部");
			return ch::has_skill(unit, 特技_巧变);
		}

	}

	Main main;
<<<<<<< HEAD
}
=======
}
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

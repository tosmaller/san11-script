// ## 2020/10/01 # 江东新风 ##

namespace sire特技_神臂
{
	class Main
	{
		Main()
		{
			pk::set_func(54, pk::func54_t(callback));
		}

		int callback(pk::unit@ unit, int range)
		{
			//if (ch::has_skill(unit, 129)) pk::trace("拥有特技神臂-外部");
			//pk::trace("神臂:" + range);
			//else  pk::trace("未拥有特技神臂-外部");
			if (ch::has_skill(unit, 特技_神臂))
				return (range + 1);
			return range;
		}

	}

	Main main;
}
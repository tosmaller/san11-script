// ## 2020/10/01 # 江东新风 ##

namespace sire特技_百战
{
	class Main
	{
		Main()
		{
			pk::set_func(53, pk::func53_t(callback));
		}

		int callback(pk::unit@ unit, pk::tactics@ tactic, int type)
		{
			int energy_cost = tactic.energy_cost;
			//string 呼叫类型;
			//string name = pk::decode(tactic.name);
			//if (ch::has_skill(unit,127)) pk::trace("拥有特技百战-外部");
			//else  pk::trace("未拥有特技百战-外部");
			/*
			switch (type)
			{
			case 1:
				呼叫类型 = '显示';
				break;
			case 2:
				呼叫类型 = '需求';
				break;
			case 3:
				呼叫类型 = '实际';
				break;
			}
			if (unit is null)
			{
				pk::trace("部队不存在");
			}
			*/
			if (ch::has_skill(unit, 特技_百战))
			{

				//pk::trace(pk::format("战法为{}，拥有百战，{}战法气力消耗为{}",name, 呼叫类型, int (energy_cost/3)));
				energy_cost = int(energy_cost / 3);
			}
			if (ch::has_skill(unit, 特技_蓄势))
				energy_cost += int(0.2f * energy_cost);
			//pk::trace(pk::format("战法为{}，未拥有百战，{}战法气力消耗为{}",name, 呼叫类型, energy_cost));
			return energy_cost;
		}

	}

	Main main;
}
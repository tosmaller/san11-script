// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/07/26 ##
namespace TERRAIN_POISON_DAMAGE
{
	//---------------------------------------------------------------------------------------
	const int 毒泉基础伤害 = 1000;
	//---------------------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(217, pk::func217_t(callback));
			pk::bind(107, pk::trigger107_t(onNewDay));
		}

		int callback(pk::unit@ unit, const pk::point& in pos)
		{
			/* 기존 SCRIPT
				if (ch::has_skill(unit, 특기_해독))
				return 0;
				return 200 + pk::rand(200);
			*/

			// 해독 특기를 보유한 경우에는 독천 피해를 받지 않음
			if (ch::has_skill(unit, 특기_해독))
				return 0;
			if (pk::rand_bool(66)) pk::set_status(unit,null,部队状态_中毒,2);
			// 해독 특기가 없을 경우에 받는 독천 피해
			return 毒泉基础伤害 + pk::rand(毒泉基础伤害 / 2);
		}

		void onNewDay()
		{
			for (int i = 0; i < 部队_末; ++i)
			{
				pk::unit@ unit = pk::get_unit(i);
				if (pk::is_alive(unit))
				{
					if (unit.status == 部队状态_中毒)
					{
						ch::add_troops(unit, -400, true, 9);//只需要减兵，异常状态会自然计数会自动减少
						if (unit.troops == 0) pk::kill(unit);
					}
					else if (unit.status == 部队状态_恐惧) pk::add_energy(unit,-10,true);
				}
			}
		}
	}

	Main main;
}
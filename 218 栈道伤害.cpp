// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/12/22 # 江东新风 # 山战背水特技 ##
// ## 2020/08/16 # 江东新风 # has_skill函数替換 ##
// ## 2020/07/26 ##
namespace TERRAIN_CLIFFROAD_DAMAGE
{
	//---------------------------------------------------------------------------------------
	const int 栈道基础伤害 = 200;
	const int 难所行军踏破栈道伤害 = 20;
	//---------------------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(218, pk::func218_t(callback));
		}

		int callback(pk::unit@ unit, const pk::point& in pos)
		{

			/* 원본 SCRIPT
				if (ch::has_skill(unit, 특기_답파) or unit.has_tech(기교_난소행군))
					return 0;
				return 100 + pk::rand(200);
			*/

			// 수정 SCRIPT (답파 특기 or 난소행군 기교를 보유시 피해를 감소하는 것으로 변경)
			if (ch::has_skill(unit, 特技_山战) and pk::get_hex(pos).terrain == 地形_山)
				return 0;
			if (ch::has_skill(unit, 特技_背水) and pk::get_hex(pos).terrain == 地形_岸)
				return 0;
			if (ch::has_skill(unit, 특기_답파) or unit.has_tech(기교_난소행군))
				return 难所行军踏破栈道伤害 + pk::rand(难所行军踏破栈道伤害);

			// 답파 특기 or 난소행군 기교를 보유하지 않을 경우 받는 피해 
			return 栈道基础伤害 + pk::rand(栈道基础伤害);

		}
	}

	Main main;
}
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/29 #江东新风# 同步马术书大神的更新 ##
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/07/26 ##
namespace BUILDING_FOOD_USE
{
	//---------------------------------------------------------------------------

	const bool 兵粮焚烧 = true;
	const int 基础耗粮 = 25;  //1000人一旬的基础耗粮
	const int 屯田耗粮倍率 = 50;

	//---------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(167, pk::func167_t(callback));
		}

		int callback(pk::building@ building)
		{
			if (!pk::is_alive(building) or !pk::is_valid_force_id(building.get_force_id()))
				return 0;

			float n = 0;

			if (兵粮焚烧 and building.is_on_fire())
			{
				pk::person@ taishu = pk::get_person(pk::get_taishu_id(building));
				int pol = taishu !is null ? taishu.stat[武将能力_政治] : 0;
				n = n + (6.f - (pol / 20.f)) * pk::get_food(building) / 100.f;
			}

			//주변에 적이 있을 시 병량 소비량 2배 증가 ('18.10.14)
			if (pk::enemies_around(building))
				n = n + pk::get_troops(building) * 基础耗粮 / 500;    // 전투부대 수준으로 병량 소비
			else
				n = n + pk::get_troops(building) * 基础耗粮 / 1000;

			if (n <= 0 and pk::get_troops(building) > 0)
				return 1;

			// 둔전 특기 보유시 병량 감소량 반감 
			if (ch::has_skill(building, 특기_둔전))
				return int(n * 屯田耗粮倍率 / 100.f);

			return int(n);
		}
	}

	Main main;
}
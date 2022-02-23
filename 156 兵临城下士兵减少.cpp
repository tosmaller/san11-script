// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/07/26 ##
namespace DESERTION
{
	//---------------------------------------------------------------------------------------
	const bool 兵临城下士兵溃逃 = true;
	//---------------------------------------------------------------------------------------
	class Main
	{
		Main()
		{
			pk::set_func(156, pk::func156_t(callback));
		}

		int callback(pk::building@ building)
		{
			int n = 0;
			if (兵临城下士兵溃逃)
			{
				n = pk::max(building.get_troops(), 100);
				n = int(pk::rand(50) - sqrt(n / 100) * -30);


				//주변에 적이 있을 시 병력 감소 2배 증가 ('18.10.12)
				if (pk::enemies_around(building))
				{
					n *= 2;
					pk::add_energy(building, -3);               // 전시효과로 기력 3 감소
				}
			}
			return -n;
		}
	}

	Main main;
}
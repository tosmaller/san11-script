// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/07/26 ##
namespace TRANSPORT_WEAPON_AMOUNT_LIMIT
{
	class Main
	{
		Main()
		{
			pk::set_func(109, pk::func109_t(callback));
		}

		uint callback(pk::building@ base, int weapon_id)
		{
			if (weapon_id < 兵器_冲车)
				return pk::min(pk::get_weapon_amount(base, weapon_id), 100000);
			return pk::min(pk::get_weapon_amount(base, weapon_id), 100);
		}
	}

	Main main;
}
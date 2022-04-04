// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/11/03 # 江东新风 # 基础资金支出设定 ##

namespace 资金_支出
{

	class Main
	{
		Main()
		{
			pk::set_func(174, pk::func174_t(callback));

		}

		int callback(pk::building@ building)
		{
			if (building is null or !pk::is_valid_force_id(building.get_force_id()) or (building.facility > 2))
				return 0;

			//俸禄
			int pay = 0;
			pk::list<pk::person@> list = pk::get_person_list(building, pk::mibun_flags(身分_都督, 身分_太守, 身分_一般));
			for (int i = 0; i < list.count; i++)
			{
				pk::rank@ rank = pk::get_rank(list[i].rank);
				if (pk::is_alive(rank))
					pay += rank.pay;
			}
			return pay;
		}
	}

	Main main;
}
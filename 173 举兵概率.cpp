// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/07/26 ##
namespace HATAAGE_CHANCE
{
	class Main
	{
		Main()
		{
			pk::set_func(173, pk::func173_t(callback));
		}

		bool callback()
		{
			if (pk::get_elapsed_years() < 1)
				return false;
			if (pk::is_first_month_of_quarter())
				return false;
			int n = pk::get_elapsed_years() / 3 + 3;
			n = pk::min(n, 8);
			return pk::rand_bool(n);
		}
	}

	Main main;
}
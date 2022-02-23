// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/07/26 ##
namespace TRANSPORT_FOOD_LIMIT
{
	class Main
	{
		Main()
		{
			pk::set_func(107, pk::func107_t(callback));
		}

		uint callback(pk::building@ base)
		{
			return pk::min(pk::get_food(base), 500000);
		}
	}

	Main main;
}
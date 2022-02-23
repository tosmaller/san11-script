// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/07/26 ##
namespace TRANSPORT_GOLD_LIMIT
{
	class Main
	{
		Main()
		{
			pk::set_func(106, pk::func106_t(callback));
		}

		uint callback(pk::building@ base)
		{
			return pk::min(pk::get_gold(base), 100000);
		}
	}

	Main main;
}
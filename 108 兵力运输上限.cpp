// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/07/26 ##
namespace TRANSPORT_TROOPS_LIMIT
{
	class Main
	{
		Main()
		{
			pk::set_func(108, pk::func108_t(callback));
		}

		uint callback(pk::building@ base)
		{
			return pk::min(pk::get_troops(base), 60000);
		}
	}

	Main main;
}
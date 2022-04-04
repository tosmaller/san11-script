// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/07/26 ##
namespace EKIBYOU_SHOUBYOU_CHANGE
{
	class Main
	{
		Main()
		{
			pk::set_func(161, pk::func161_t(callback));
		}

		int callback(pk::building@ building, pk::person@ person)
		{
			if (pk::rand_bool(8))
				return 상병_경증;
			else if (pk::rand_bool(2))
				return 상병_중증;
			else
				return -1;
		}
	}

	Main main;
}
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/07/26 ##
namespace SELECT_TOTOKU
{
	class Main
	{
		Main()
		{
			pk::set_func(171, pk::func171_t(callback));
		}

		void callback(pk::list<pk::person@>& list)
		{
			list.sort(function(a, b)
			{
				if ((a.mibun == 身份_君主 and b.mibun != 身份_君主) or (a.mibun != 身份_君主 and b.mibun == 身份_君主))
					return a.mibun == 身份_君主;

				if (pk::get_command(a) != pk::get_command(b))
					return pk::get_command(a) > pk::get_command(b);

				if (a.rank != b.rank)
					return a.rank < b.rank;

				if (a.stat[武将能力_统率] != b.stat[武将能力_统率])
					return a.stat[武将能力_统率] > b.stat[武将能力_统率];

				return a.p < b.p;
			});
		}
	}

	Main main;
}
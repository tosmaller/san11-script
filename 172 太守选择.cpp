// ## 2021/10/28 # 江东新风 # 关闭trace ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/07/26 ##
namespace SELECT_TAISHU
{
	class Main
	{
		Main()
		{
			pk::set_func(172, pk::func172_t(callback));
		}

		void callback(pk::list<pk::person@>& list)
		{
			list.sort(function(a, b)
			{
				if ((a.mibun <= 身份_都督 or b.mibun <= 身份_都督) and a.mibun != b.mibun)
					return a.mibun < b.mibun;

				if (pk::get_command(a) != pk::get_command(b))
					return pk::get_command(a) > pk::get_command(b);

				if (a.stat[武将能力_统率] != b.stat[武将能力_统率])
					return a.stat[武将能力_统率] > b.stat[武将能力_统率];

				if (a.stat[武将能力_武力] != b.stat[武将能力_武力])
					return a.stat[武将能力_武力] > b.stat[武将能力_武力];

				if (a.kouseki != b.kouseki)
					return a.kouseki > b.kouseki;

				return a.get_id() < b.get_id();
			});
		}
	}

	Main main;
}
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/07/26 ##
namespace AP
{
	class Main
	{
		Main()
		{
			pk::set_func(162, pk::func162_t(callback));
		}

		int callback(pk::district@ district)
		{
			int n = 0;

			pk::person@ totoku = pk::get_person(district.totoku);
			n = n + pk::max(totoku.stat[武将能力_统率], totoku.stat[武将能力_魅力]) / 5;
			if (n < 6)
				n = 6;

			int city_ap = pk::clamp(district.query_value(军团属性_城市数),1, 6) * 10;
			n = n + city_ap + 10 + pk::min(city_ap, pk::get_person_list(district, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)).count);

			pk::person@ gunshi = pk::get_person(pk::get_force(district.get_force_id()).gunshi);
			if (pk::is_alive(gunshi))
				n = (int(gunshi.stat[武将能力_智力] - 60) / 2 + 100) * n / 100;

			n = n + district.query_value(军团属性_已建设施数 + 设施_符节台 - 设施_市场1级) * 5;//已建成设施数是从市场1级开始算的。所以这里就是算的已建成符节台数

			if (ch::has_skill(pk::get_force(district.get_force_id()), 特技_变法))
				n += 20;

			return pk::min(255, n);
		}
	}

	Main main;
}
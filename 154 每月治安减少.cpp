// ## 2021/10/10 # 江东新风 # 将函数内容全搬运到305，以便其他cpp调用 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/23 #江东新风#同步马术书大神的更新，添加城市数量惩罚 ##
// ## 2020/08/18 # 氕氘氚 # 添加法治影响 ##
namespace PUBLIC_ORDER_CHANGE
{
	//---------------------------------------------------------------------------------------
	const int 整备政令下降机率 = 50;
	const int 治安下降上限 = 5;
	const int 治安下降基数 = 2;
	const int 电脑治安下降倍率 = 50;
	//---------------------------------------------------------------------------------------
	class Main
	{
		Main()
		{
			pk::set_func(154, pk::func154_t(callback));
		}

		int callback(pk::city@ city)
		{
			return inter::get_season_order_dec(pk::city_to_building(city), 整备政令下降机率, 治安下降上限, 治安下降基数, 电脑治安下降倍率);
			//以下内容均搬运到305全局函数内政，以方便其他cpp调用
			/*
			if (!pk::is_first_month_of_quarter(pk::get_month()))
				return 0;
			pk::force@ force = pk::get_force(city.get_force_id());
			if (force is null)
				return 0;
			// 법령정비 연구 시 50% 확률로 감소하지 않음
			if (pk::has_tech(force, 기교_법령정비) and pk::rand_bool(100 - 整备政令下降机率))
				return 0;
			int n = 90;
			pk::person@ taishu = pk::get_person(city.get_taishu_id());
			if (pk::is_alive(taishu))
				n = n - taishu.stat[武将能力_魅力];
			n = pk::max(n, 1) / 10 + pk::rand(治安下降基数 * 2);
			n = pk::min(n, 治安下降上限);


			//  电脑势力
			if (!city.is_player())
			{
				n = int(n * 电脑治安下降倍率 / 100.f);
			}


			// 동탁이 군주이면 폭정 효과 적용 ('18.10.4)
			if (pk::get_kunshu_id(force) == 무장_동탁)
				n = int(n * 2.f);

			// 이각이 군주이면 폭정 효과 적용
			if (pk::get_kunshu_id(force) == 무장_이각)
				n = int(n * 2.f);

			// 곽사가 군주이면 폭정 효과 적용
			if (pk::get_kunshu_id(force) == 무장_곽사)
				n = int(n * 2.f);

			// 손호가 군주이면 폭정 효과 적용
			if (pk::get_kunshu_id(force) == 무장_손호)
				n = int(n * 2.f);

			n = ch::get_order_change(city, -n);
			return n;
			*/
		}
	}

	Main main;
}
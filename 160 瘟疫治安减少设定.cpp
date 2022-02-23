// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/08/18 # 氕氘氚 # 添加法治影响 ##
namespace EKIBYOU_PUBLIC_ORDER_CHANGE
{
	class Main
	{
		Main()
		{
			pk::set_func(160, pk::func160_t(callback));
		}

		int callback(pk::building@ building)
		{
			return ch::get_order_change(pk::building_to_city(building), -(2 + pk::rand(3)));
		}
	}

	Main main;
}
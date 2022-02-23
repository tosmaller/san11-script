// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/07/26 ##
namespace ITEM_FIND_CHANCE
{
	class Main
	{
		Main()
		{
			pk::set_func(110, pk::func110_t(callback));
		}

		int callback(pk::building@ building, pk::person@ actor, pk::item@ item)
		{
			return pk::max((61 - item.value) / 20, 1);
		}
	}

	Main main;
}
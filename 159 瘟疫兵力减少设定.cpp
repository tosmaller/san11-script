// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/10/29 #氕氘氚# 医者效果添加 ##
// ## 2020/10/23 #江东新风#同步马术书大神的更新
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/07/26 ##
namespace EKIBYOU_TROOPS_CHANGE
{
	class Main
	{
		Main()
		{
			pk::set_func(159, pk::func159_t(callback));
		}

		int callback(pk::building@ building)
		{
			if (!pk::is_campaign())
			{
				if (ch::has_skill(building, 特技_医者, true))
					return -(pk::get_troops(building) * (pk::rand(6) + 5) / 100) / 4;
			}
			return -(pk::get_troops(building) * (pk::rand(6) + 5) / 100);
		}
	}

	Main main;
}
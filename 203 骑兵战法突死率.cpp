// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/10/27 # 氕氘氚 # 突死率清零避免与战法死亡率重复计算 ##
namespace TACTICS_KILL_CHANCE
{
	class Main
	{
		Main()
		{
			pk::set_func(203, pk::func203_t(callback));
		}

		int callback(pk::person@ attacker, pk::person@ target, int tactics_id, bool critical)
		{
			return 0;
		}
	}

	Main main;
}
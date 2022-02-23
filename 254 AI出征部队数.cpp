// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/07/26 ##
namespace AI_ATTACK_TROOPS_AVAILABLE
{
	class Main
	{
		Main()
		{
			pk::set_func(254, pk::func254_t(callback));
		}
		//不是部队数。而且在超级难度或者目标是玩家时，此设定无效
		int callback(pk::building@ building, int troops)
		{
			pk::person@ kunshu = pk::get_person(pk::get_kunshu_id(building));
			int character = building.is_player() ? 性格_冷静 : kunshu.character;
			return troops * (character + 3);
		}
	}

	Main main;
}
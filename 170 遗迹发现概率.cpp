// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/07/26 ##
namespace RUINS_CHANCE
{
	class Main
	{
		Main()
		{
			pk::set_func(170, pk::func170_t(callback));
		}

		int callback(pk::unit@ unit, int facility_id)
		{
			pk::person@ leader = pk::get_person(unit.leader);
			if (leader.kouseki < 10000 or unit.attr.stat[部队能力_魅力] < 80)
				return 0;
			else if (facility_id == 시설_유적 and pk::has_member(unit, 무장_이적))
				return 100;
			else
				return unit.attr.stat[部队能力_魅力] / 2;
		}
	}

	Main main;
}
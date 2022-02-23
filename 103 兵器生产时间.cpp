// ## 2021/10/11 # 江东新风 # 造船和工坊对应特技错误 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2020/10/23 #江东新风#同步马术书大神的更新，添加城市数量惩罚，修复战役模式情况的nullptr错误##
// ## 2020/08/24 # 氕氘氚 # 常量修改 ##
// ## 2020/08/16 #江东新风#has_skill函数替換##
/*
@수정자 : 기마책사
@Update: '20.8.29  // 수정내용: 유저_도시수_패널티 추가, 캠페인에서는 커스텀 세팅 사용 불가하도록 수정
*/

namespace PRODUCE_TIME_COST
{

	//---------------------------------------------------------------------------

	//const bool 生产时间_城市数_惩罚 = true;     // 유저세력에 대해서 도시수에 비례하여 생산기간 디버프 (도시당 5% 감소)

	//---------------------------------------------------------------------------



	class Main
	{
		Main()
		{
			pk::set_func(103, pk::func103_t(callback));
		}

		int callback(const pk::detail::arrayptr<pk::person@>& in actors, int weapon_id)
		{
			if (!pk::is_valid_equipment_id(weapon_id))
				return 0;
			if (weapon_id < 병기_충차)
				return 0;

			int n = 0, sum = 0, max = 0, skill_id = -1;
			bool has_skill = false;

			if (weapon_id == 兵器_冲车 or weapon_id == 兵器_井阑 or weapon_id == 兵器_投石 or weapon_id == 兵器_木兽)
				skill_id = int(pk::core["weapon_produce.workshop_skill"]);
			else if (weapon_id == 兵器_楼船 or weapon_id == 兵器_斗舰)
				skill_id = int(pk::core["weapon_produce.shipyard_skill"]);

			for (int i = 0; i < actors.length; i++)
			{
				pk::person@ actor = actors[i];
				if (pk::is_alive(actor))
				{
					int s = actor.stat[int(pk::core["weapon_produce.stat"])];
					sum = sum + s;
					max = pk::max(max, s);
					if (ch::has_skill(actor, skill_id))
						has_skill = true;
				}
			}

			int top = 生产时长上限;

			if (weapon_id >= 兵器_走舸)
			{
				pk::person@ actor = actors[0];
				if (pk::is_alive(actor))
				{
					pk::building@ building = pk::get_building(actor.service);
					if (pk::is_alive(building))
					{
						pk::city@ city = pk::building_to_city(building);
						if (pk::has_facility(city, 设施_练兵所))
						{
							top = pk::max(生产时长上限 - 2, 3);
						}
					}
				}
			}


			if (n < 2) n = 2;

			n = top - pk::max(sum + max - 200, 24) / 24;
			n = n * 生产时长倍率 / 100;

			// 관련 특기를 가지고 있다면 반감
			if (has_skill)
				n = n * 发明造船倍率 / 100;

			n = pk::max(n, 1);      // overflow 방지용 ('18.11.17)
			return n;
		}
	}

	Main main;
}
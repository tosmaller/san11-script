// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2020/10/23 #江东新风#同步马术书大神的更新，修复战役模式情况的nullptr错误##
// ## 2020/08/24 # 氕氘氚 # 常量修改 ##
// ## 2020/08/16 #江东新风#has_skill函数替換##
/*
@수정자 : 기마책사
@Update: '19.05.4   // 수정내용: 유저_도시수_패널티 추가
@Update: '20.8.29  // 수정내용: 캠페인에서는 커스텀 세팅 사용 불가하도록 수정
*/

namespace PRODUCE
{
	//---------------------------------------------------------------------------

	const bool 玩家_城市数_惩罚 = true;   // 유저세력에 대해서 도시수에 비례하여 생산량 디버프 (도시당 1% 감소)


	//---------------------------------------------------------------------------

	class Main
	{

		Main()
		{
			pk::set_func(102, pk::func102_t(callback));
		}

		int callback(pk::city@ city, const pk::detail::arrayptr<pk::person@>& in actors, int weapon_id)
		{
			if (!pk::is_valid_equipment_id(weapon_id))
				return 0;
<<<<<<< HEAD
			if (weapon_id >= 병기_충차)
=======
			if (weapon_id >= 兵器_冲车)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				return 1;

			int n = 0, sum = 0, max = 0, skill_id = -1;
			int mul = 100, pro = 100;

<<<<<<< HEAD
			if (weapon_id <= 병기_노)
=======
			if (weapon_id <= 兵器_弩)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			{
				skill_id = int(pk::core["weapon_produce.smith_skill"]);
				pro = 制造数量倍率;
			}
<<<<<<< HEAD
			else if (weapon_id == 병기_군마)
=======
			else if (weapon_id == 兵器_战马)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			{
				skill_id = int(pk::core["weapon_produce.stable_skill"]);
				pro = 军马数量倍率;
			}

			for (int i = 0; i < int(actors.length); i++)
			{
				pk::person@ actor = actors[i];
				if (pk::is_alive(actor))
				{
					int s = actor.stat[int(pk::core["weapon_produce.stat"])];
<<<<<<< HEAD
					sum = sum + s;
					max = pk::max(max, s);
					if (ch::has_skill(actor, skill_id) and weapon_id <= 병기_노)
						mul += 能吏生产加成;
					else if (ch::has_skill(actor, skill_id) and weapon_id == 병기_군마)
=======
					//pk::trace("produce stat:" + s);
					sum = sum + s;
					max = pk::max(max, s);
					if (ch::has_skill(actor, skill_id) and weapon_id <= 兵器_弩)
						mul += 能吏生产加成;
					else if (ch::has_skill(actor, skill_id) and weapon_id == 兵器_战马)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
						mul += 繁殖生产加成;
				}
			}

			n = (sum + max + 200) * 5 * mul / 100 * pro / 100;
			n = int(n * func_5c7040(city, weapon_id));


			// 컴퓨터일 경우 특급 난이도에서 2배
			if (pk::get_scenario().difficulty == 难易度_超级 and !city.is_player())
				n = n * 超级电脑生产倍率 / 100;


			// 玩家_城市数_惩罚 ('19.5.4)
			if (玩家_城市数_惩罚 and city.is_player() and !pk::is_campaign())
			{
				pk::force@ force = pk::get_force(city.get_force_id());
				float force_city_count = float(pk::get_city_list(force).count);
				//pk::trace(pk::format("城市数量为{},倍率惩罚{}",force_city_count, (1.f - (force_city_count * 0.01f))));
				n = int(n * (1.f - (force_city_count * 0.01f)));
			}
			//pk::trace(pk::format("3预计生产量为{}",n));

			return n;
		}

		float func_5c7040(pk::city@ city, int weapon_id)
		{
			int level1 = 0, level2 = 0;
<<<<<<< HEAD
			if (weapon_id == 병기_창 or weapon_id == 병기_극 or weapon_id == 병기_노)
=======
			if (weapon_id == 兵器_枪 or weapon_id == 兵器_戟 or weapon_id == 兵器_弩)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			{
				for (int i = 0; i < city.max_devs; i++)
				{
					pk::building@ building = city.dev[i].building;
					if (pk::is_alive(building))
					{
						switch (building.facility)
						{
						case 시설_대장간: building.completed ? level1++ : 0; break;
						case 시설_대장간2단: building.completed ? level2++ : level1++; break;
						case 시설_대장간3단: building.completed ? 0 : level2++; break;
						}
					}
				}
				if (int(city.blacksmith_counter) > level1 + level2)
					return 1.5f;
				if (int(city.blacksmith_counter) > level1)
					return 1.2f;
			}
<<<<<<< HEAD
			else if (weapon_id == 병기_군마)
=======
			else if (weapon_id == 兵器_战马)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			{
				for (int i = 0; i < int(city.max_devs); i++)
				{
					pk::building@ building = city.dev[i].building;
					if (pk::is_alive(building))
					{
						switch (building.facility)
						{
						case 시설_마구간: building.completed ? level1++ : 0; break;
						case 시설_마구간2단: building.completed ? level2++ : level1++; break;
						case 시설_마구간3단: building.completed ? 0 : level2++; break;
						}
					}
				}
				if (int(city.stable_counter) > level1 + level2)
					return 1.5f;
				if (int(city.stable_counter) > level1)
					return 1.2f;
			}
			return 1.f;
		}
	}

	Main main;
}
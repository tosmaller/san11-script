<<<<<<< HEAD
﻿// ## 2021/11/23 # 江东新风 # 港关收入人口影响， ##
=======
﻿// ## 2022/02/17 # 江东新风 # 人口设定调整 ##
// ## 2021/11/23 # 江东新风 # 港关收入人口影响， ##
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/02/17 # 江东新风 # 城市数惩罚设置上限，调整系数 ##
// ## 2021/02/12 # 氕氘氚 # 解决港关非所属时收入的问题##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/23 #江东新风#同步马术书大神的更新，添加城市数量惩罚 ##	
// ## 2020/09/28 # 氕氘氚 # 添加固定收入 ##
/*
@수정자 : 기마책사
@Update : '18.10.15  / 변경내용 : 관문 병량수입 1.5배 적용
@Update : '19.2.26   / 변경내용 : 게임 내 자체적으로 x0.2배 적용되는 문제로 스크립트에서 x0.2배 적용 제외

*/

namespace GATE_HARVEST
{
	const bool  玩家_城市数_惩罚 = true;     // 유저세력에 대해서 도시수에 비례하여 수입 디버프 (도시당 5% 감소)	

	class Main
	{
		Main()
		{
			pk::set_func(153, pk::func153_t(callback));
			if (false/*港关使用固定收入*/) pk::bind(109, pk::trigger109_t(onNewSeason));
		}

		int callback(pk::building@ building, int city_harvest)
		{
			if (!pk::is_alive(building)) return 0;
			if (!pk::is_valid_normal_force_id(building.get_force_id())) return 0;
			int n = 0;
			int base_id = building.get_id();
			if (false)//港关使用固定收入)
			{
				//固定收入在此处为0，改为每回合trigger中增加
				n = 港关固定粮收入;

				float population_multi = 1.f;
				if (开启人口系统)
				{
					float population = 0;
					BaseInfo@ base_t = @base_ex[base_id];
<<<<<<< HEAD
					population = float(pk::clamp(base_t.population, 5000, 50000));//防止意外情况溢出
					population_multi = population / 50000.f;
=======
					population = float(pk::clamp(base_t.population, 5000, 100000));//防止意外情况溢出
					population_multi = population / 80000.f;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					//pk::trace(pk::format("{},population", pk::decode(pk::get_name(city)), population));
				}
				n = int(n * population_multi);

				/**/
			}
			else
			{
				n = int(city_harvest * 0.2f * 港关粮收入倍率 / 100.f);
				//pk::trace("对应城市收入："+ city_revenue + "n:" + n);
				float population_multi = 1.f;
				if (开启人口系统)
				{
					float population = 0;
					BaseInfo@ base_t = @base_ex[base_id];
<<<<<<< HEAD
					population = float(pk::clamp(base_t.population, 5000, 50000));//防止意外情况溢出
					population_multi = population / 20000.f;
=======
					population = float(pk::clamp(base_t.population, 5000, 10000));//防止意外情况溢出
					population_multi = population / 80000.f;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					//pk::trace(pk::format("{},population", pk::decode(pk::get_name(city)), population));
				}
				n = int(n * population_multi);

				//ch::debug(2, pk::format("n={}", n));
				if (关隘粮收入加成 and (base_id >= 建筑_关卡开始) and (base_id < 建筑_关卡末))
					n = int(n * 1.5f);
			}

			// 游戏难度修正
			n = inter::incomes_difficulty_impact(n, building.is_player());
			// 总倍率
			if (building.is_player())
				n = int(n * 玩家粮收入倍率 / 100.f);
			else
				n = int(n * 电脑粮收入倍率 / 100.f);
<<<<<<< HEAD
=======
			n += base_ex[building.get_id()].harvest_bonus;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			// 玩家_城市数_惩罚 ('20.8.29)
			if (玩家_城市数_惩罚 and building.is_player() and !pk::is_campaign())
			{
				pk::force@ force = pk::get_force(building.get_force_id());
				float force_city_count = float(pk::get_city_list(force).count);
				n = int(n * (1.f - pk::min(0.3f, (force_city_count - 3) * 0.015f)));
			}
			n = pk::clamp(n, int(0.1f * city_harvest), int(0.5f * city_harvest));//港关最高收入为对应城市收入的一半
			//pk::trace("兵粮" + n);
			return n;
		}

		void onNewSeason()
        {
            for (int i = 42; i < 87; i++)
			{
				int n = 0;
				pk::building@ building = pk::get_building(i);
				int force_id = building.get_force_id ();
				if (force_id < 0 || force_id >= 42) continue;
				if (!pk::is_alive(building)) continue;

				n = 港关固定粮收入;
				// 游戏难度修正
				n = inter::incomes_difficulty_impact(n, building.is_player());
				// 总倍率
				if (building.is_player())
					n = int(n * 玩家粮收入倍率 / 100.f);
				else
					n = int(n * 电脑粮收入倍率 / 100.f);

				// 加粮食
				pk::add_food(building, n);
			}

        }


	}

	Main main;
}
// ## 2022/03/27 # 江东新风 # 军屯农倍率线性变化 ##
// ## 2022/02/17 # 江东新风 # 探索涨基础粮收入 ##
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/10 # 江东新风 # 人口相关设定 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/02/17 # 江东新风 # 城市数惩罚设置上限，调整系数 ##
// ## 2021/01/29 # 江东新风 # 修复张鲁势力产粮异常 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/23 #江东新风#同步马术书大神的更新，添加城市数量惩罚，修复战役模式情况的nullptr错误##
// ## 2020/10/07 # 氕氘氚 # 修复谷仓未建成就生效、军屯农无效果的bug ##
// ## 2020/09/28 # 氕氘氚 # 谷仓允许不相邻、谷仓对军屯农生效，玩家电脑分别设置倍率 ##
// ## 2020/09/05 # 氕氘氚 #变法bug修复##
// ## 2020/08/19 # 江东新风 #新特技-变法##
// ## 2020/07/26 ##
namespace CITY_HARVEST
{

	//---------------------------------------------------------------------------------------
	// 유저 설정 (true = on, false = off)
	const bool  玩家_城市数_惩罚 = true;     // 유저세력에 대해서 도시수에 비례하여 수입 디버프 (도시당 5% 감소)

	//---------------------------------------------------------------------------------------


	class Main
	{
		Main()
		{
			pk::set_func(151, pk::func151_t(callback));
		}

		int callback(pk::city@ city)
		{
			if (city is null or !pk::is_valid_force_id(city.get_force_id()))
				return 0;

			// 城市基础收入
			int n = city.harvest;
			int population_yield = 0;
			int farm_yield = 0;
			float population = 0;
			if (开启人口系统)
			{
				int tax_rate = 300;//普通税率为每万人收300粮
				BaseInfo@ city_t = @base_ex[city.get_id()];
				population = pk::min(1000000.f, pk::max(5000.f, city_t.population));//防止意外情况溢出
				population_yield = int(sqrt(population / 10000.f) * tax_rate);
				//pk::trace(pk::format("{},population", pk::decode(pk::get_name(city)), population));
			}
			else
			{
				population_yield = city.harvest;
			}

			// 判断有无谷仓
			bool has_谷仓 = false;
			if (造币谷仓无需相邻)
			{
				for (int i = 0; i < city.max_devs; i++)
				{
					pk::building@ t_building = city.dev[i].building;
					if (pk::is_alive(t_building) && t_building.facility == 시설_곡창 && t_building.completed)
					{
						has_谷仓 = true;
						break;
					}
				}
			}

			float basic_yield = 0;
			for (auto i = 0; i < city.max_devs; i++)
			{
				pk::building@ building = city.dev[i].building;
				int facility_id = -1;

				if (pk::is_alive(building))
				{
					facility_id = building.facility;
					switch (facility_id)
					{
					case 设施_农场1级:
					case 设施_军屯农:
						if (!building.completed)
							continue;
						break;
					case 设施_农场2级:
						if (!building.completed)
							facility_id = 设施_农场1级;
						break;
					case 设施_农场3级:
						if (!building.completed)
							facility_id = 设施_农场2级;
						break;
					default:
						continue;
					}
				}

				// 내정시설 별 생산력을 더함.
				pk::facility@ facility = pk::get_facility(facility_id);
				if (pk::is_alive(facility))
				{
					int y = facility.yield;
					basic_yield += y;
					// 军屯农收入计算
					if (facility_id == 设施_军屯农)
					{
						y = int(pk::max(float(city.troops) / 军屯农单倍收入人数, 1.f) * y);
					}
					// 谷仓加成
					if (facility_id == 设施_农场1级 or facility_id == 设施_农场2级 or facility_id == 设施_农场3级
						or (谷仓对军屯农生效 and facility_id == 设施_军屯农))
					{
						if (造币谷仓无需相邻 && has_谷仓 || func_49ed70(building.get_pos(), 시설_곡창))
						{
							if (ch::has_skill(pk::get_building(city.get_id()), 特技_变法)) //变法
								y = int(y * 2.0f);
							else
								y = int(y * 1.5f);
						}
					}

					farm_yield = farm_yield + y;
				}
			}


			if (开启人口系统)
			{
				//每30的市场基础收入需要1万人维持，人数每低于要求1%，产量下降1%,最低倍率0.4，最高1.2
				//pk::trace(pk::format("{},population:{},basic_yield:{}", pk::decode(pk::get_name(city)), population, basic_yield));
				float 人口影响倍率 = basic_yield > 0 ? pk::min(1.1f, pk::max(0.4f, ((population / 10000.f) / (basic_yield / 450.f)))) : 1.0f;
				//pk::trace(pk::format("人口：{}，基础收入：{}，人口影响倍率：{}",population,basic_yield,人口影响倍率));
				farm_yield = int(farm_yield * 人口影响倍率);
			}
			n = population_yield + farm_yield;

			//pk::trace(pk::format("{}:基础产量：{}",pk::decode(pk::get_name(city)),n));
			// 游戏难度修正
			n = inter::incomes_difficulty_impact(n, city.is_player());

			// 治安影响
			n = int(n * pk::max(city.public_order, 50) / 100);

			// 군주가 [조조]인 경우에 1.25배
			if (pk::get_kunshu_id(city) == 武将_曹操)
				n = int(n * 1.15f);

			// 군주가 [장로]인 경우에 1.45배
			if (pk::get_kunshu_id(city) == 武将_张鲁)
				n = int(n * 1.25f);

			n += base_ex[city.get_id()].harvest_bonus;
			// 玩家_城市数_惩罚 ('20.8.29)
			if (玩家_城市数_惩罚 and city.is_player() and !pk::is_campaign())
			{
				pk::force@ force = pk::get_force(city.get_force_id());
				float force_city_count = float(pk::get_city_list(force).count);
				n = int(n * (1.f - pk::min(0.3f, (force_city_count - 3) * 0.015f)));
			}


			// 总倍率
			if (city.is_player())
				n = int(n * 玩家粮收入倍率 / 100.f);
			else
				n = int(n * 电脑粮收入倍率 / 100.f);

			return n;
		}

		/**
			인접 시설 검색.
		*/
		bool func_49ed70(pk::point pos, int facility_id)
		{
			for (int i = 0; i < 방향_끝; i++)
			{
				pk::point neighbor_pos = pk::get_neighbor_pos(pos, i);
				if (pk::is_valid_pos(neighbor_pos))
				{
					pk::building@ building = pk::get_building(neighbor_pos);
					if (pk::is_alive(building) and building.facility == facility_id and building.completed)
						return true;
				}
			}
			return false;
		}
	}

	Main main;
}
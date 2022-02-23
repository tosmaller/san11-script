// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/10 # 江东新风 # 人口相关设定 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/02/17 # 江东新风 # 城市数惩罚设置上限，调整系数 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/07 # 氕氘氚 # 修复造币厂未建成就生效的bug ##
// ## 2020/09/28 # 氕氘氚 # 造币允许不相邻、造币对鱼市场、大市场生效，玩家电脑分别设置倍率 ##
// ## 2020/09/05 # 氕氘氚 #变法bug修复##										 
// ## 2020/08/19 # 江东新风 #新特技-变法##
// ## 2020/08/10 # 氕氘氚 ##
/*
@수정자: 기마책사
@Update: '18.12.22  / 변경내용: 병기차감 스크립트 분리
*/


namespace CITY_REVENUE
{
	const bool  玩家_城市数_惩罚 = true;

	class Main
	{
		Main()
		{
			pk::set_func(150, pk::func150_t(callback));

		}

		int callback(pk::city@ city)
		{
			if (city is null or !pk::is_valid_force_id(city.get_force_id()))
				return 0;

			// 城市基础收入
			int n = city.revenue;
			int population_yield = 0;
			int market_yield = 0;
			float population = 0;
			if (开启人口系统)
			{
				int tax_rate = 20;//普通税率为每万人收20金
				BaseInfo@ city_t = @base_ex[city.get_id()];
				population = pk::min(1000000.f,pk::max(5000.f,city_t.population));//防止意外情况溢出
				population_yield = int(sqrt(population/10000.f) * tax_rate);
				//pk::trace(pk::format("{},population", pk::decode(pk::get_name(city)), population));
			}
			else
			{
				population_yield = city.revenue;
			}


			// 判断有无造币厂
			bool has_造币 = false;
			if (造币谷仓无需相邻)
			{
				for (int i = 0; i < city.max_devs; i++)
				{
					//为了避免遍历城市周围地格，所以城市信息里会存开发土地的信息，如果想要支城能具有影响收入效果，可以参考这个
					pk::building@ t_building = city.dev[i].building;
					if (pk::is_alive(t_building) && t_building.facility == 시설_조폐 && t_building.completed)
					{
						has_造币 = true;
						break;
					}
				}
			}

			float basic_yield = 0;
			for (int i = 0; i < city.max_devs; i++)
			{
				pk::building@ building = city.dev[i].building;
				int facility_id = -1;

				if (pk::is_alive(building))
				{
					facility_id = building.facility;
					switch (facility_id)
					{
					case 设施_市场1级:
					case 设施_大市场:
					case 设施_渔市:
					case 设施_黑市:
						if (!building.completed)
							continue;
						break;
					case 设施_市场2级:
						if (!building.completed)
							facility_id = 设施_市场1级;
						break;
					case 设施_市场3级:
						if (!building.completed)
							facility_id = 设施_市场2级;
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
					// 造币加成
					if (facility_id == 시설_시장 or facility_id == 시설_시장2단 or facility_id == 시설_시장3단
						or (造币对鱼市大市生效 and (facility_id == 시설_대시장 or facility_id == 시설_어시장)))
					{
						if (造币谷仓无需相邻 && has_造币 || func_49ed70(building.get_pos(), 시설_조폐))
						{
							//变法			  
							if (ch::has_skill(pk::get_building(city.get_id()), 特技_变法)) //变法
								y = int(y * 2.0f);
							else
								y = int(y * 1.5f);
						}
					}
					market_yield = market_yield + y;
				}
			}

			if (开启人口系统)
			{
				//每30的市场基础收入需要1万人维持，人数每低于要求1%，产量下降1%,最低倍率0.4，最高1.2
				//pk::trace(pk::format("{},population:{},basic_yield:{}", pk::decode(pk::get_name(city)), population, basic_yield));
				float 人口影响倍率 = basic_yield > 0 ? pk::min(1.1f,pk::max(0.4f,((population / 10000.f) / (basic_yield / 30.f)))):1.0f;
				//pk::trace(pk::format("人口：{}，基础收入：{}，人口影响倍率：{}",population,basic_yield,人口影响倍率));
				market_yield = int(market_yield * 人口影响倍率);
			}
			n = population_yield + market_yield;

			// 游戏难度修正
			n = inter::incomes_difficulty_impact(n, city.is_player());

			// 治安修正
			n = n * pk::max(city.public_order, 50) / 100;
			// 总倍率
			if (city.is_player())
				n = int(n * 玩家金收入倍率 / 100.f);
			else
				n = int(n * 电脑金收入倍率 / 100.f);

			// 玩家_城市数_惩罚 ('20.8.29)
			if (玩家_城市数_惩罚 and city.is_player() and !pk::is_campaign())
			{
				pk::force@ force = pk::get_force(city.get_force_id());
				float force_city_count = float(pk::get_city_list(force).count);
				n = int(n * (1.f - pk::min(0.3f, (force_city_count - 3) * 0.015f)));
			}

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
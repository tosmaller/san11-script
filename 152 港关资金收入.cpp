// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/02/17 # 江东新风 # 城市数惩罚设置上限，调整系数 ##
// ## 2021/02/12 # 氕氘氚 # 解决港关非所属时收入的问题##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/23 #江东新风#同步马术书大神的更新，添加城市数量惩罚 ##
// ## 2020/09/28 # 氕氘氚 # 添加固定收入 ##
/*
@수정자: 기마책사
@Update: '18.12.22  / 변경내용: 병기차감 스크립트 분리
@Update: '19.2.23   / 변경내용: 게임 내에서 x0.2 자동적용되어서 스크립트에서 제외
*/


namespace GATE_REVENUE
{
	const bool  玩家_城市数_惩罚 = true;     // 유저세력에 대해서 도시수에 비례하여 수입 디버프 (도시당 5% 감소)	

	class Main
	{
		Main()
		{
			pk::set_func(152, pk::func152_t(callback));
			if (true) pk::bind(100, pk::trigger100_t(onTitleScreen));
			if (/*港关使用固定收入*/false) pk::bind(108, pk::trigger108_t(onNewMonth));
		}

		void onTitleScreen()
		{
			pk::set_independent_income(true);//开启港关独立收入（会同时让无势力港关有收入，需在后方排除）
		}

		int callback(pk::building@ building, int city_revenue)
		{
			if (!pk::is_alive(building)) return 0;
			if (!pk::is_valid_normal_force_id(building.get_force_id())) return 0;
			int n = 0;
			int base_id = building.get_id();
			if (false)//港关使用固定收入)
			{
				//固定收入在此处为0，改为每回合trigger中增加
				n = 港关固定金收入;
				//ch::debug(2, pk::format("n={}", n));

				float population_multi = 1.f;
				if (开启人口系统)
				{
					float population = 0;
					BaseInfo@ base_t = @base_ex[base_id];
					population = float(pk::clamp(base_t.population, 5000, 50000));//防止意外情况溢出
					population_multi = population / 100000.f;
					//pk::trace(pk::format("{},population", pk::decode(pk::get_name(city)), population));
				}
				n = int(n * population_multi);

			}
			else
			{
				n = int(city_revenue * 0.2f * 港关金收入倍率 / 100.f);
				//pk::trace("对应城市收入："+ city_revenue + "n:" + n);
				float population_multi = 1.f;
				if (开启人口系统)
				{
					float population = 0;
					BaseInfo@ base_t = @base_ex[base_id];
					population = float(pk::clamp(base_t.population, 5000, 50000));//防止意外情况溢出
					population_multi = population / 20000.f;
					//pk::trace(pk::format("{},population", pk::decode(pk::get_name(city)), population));
				}
				n = int(n* population_multi);
				
				//ch::debug(2, pk::format("n={}", n));
				if (港口金收入加成 and (base_id >= 建筑_港口开始) and (base_id < 建筑_港口末))
					n = int(n * 1.5f);
			}

			// 游戏难度修正
			n = inter::incomes_difficulty_impact(n, building.is_player());
			// 总倍率
			if (building.is_player())
				n = int(n * 玩家金收入倍率 / 100.f);
			else
				n = int(n * 电脑金收入倍率 / 100.f);
			/**/

			// 玩家_城市数_惩罚 ('20.8.29)
			if (玩家_城市数_惩罚 and building.is_player() and !pk::is_campaign())
			{
				pk::force@ force = pk::get_force(building.get_force_id());
				float force_city_count = float(pk::get_city_list(force).count);
				n = int(n * (1.f - pk::min(0.3f, (force_city_count - 3) * 0.015f)));
			}
			//pk::trace("对应城市收入2：" + city_revenue + "n:" + n);
			n = pk::clamp(n, int(0.1f * city_revenue), int(0.5f * city_revenue));//港关最高收入为对应城市收入的一半
			//pk::trace("资金"+n);
			//ch::debug(2, pk::format("n t ={}", n));
			return n;
		}


		void onNewMonth()
        {
            for (int i = 42; i < 87; i++)
			{
				int n = 0;
				pk::building@ building = pk::get_building(i);
				int force_id = building.get_force_id();
				if (force_id < 0 || force_id >= 42) continue;
				if (!pk::is_alive(building)) continue;

				n = 港关固定金收入;
				// 游戏难度修正
				n = inter::incomes_difficulty_impact(n, building.is_player());
				// 总倍率
				if (building.is_player())
					n = int(n * 玩家金收入倍率 / 100.f);
				else
					n = int(n * 电脑金收入倍率 / 100.f);

				// 加金
				pk::add_gold(building, n);
			}

        }

		//---------------------------------------------------------------------------------------


	}

	Main main;
}
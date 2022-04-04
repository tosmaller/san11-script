// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/10 # 江东新风 # 治安相关函数也搬运至此 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2021/02/16 # 江东新风 # 一般城市征兵倍率无法修改bug ##
// ## 2021/01/24 # 江东新风 # 将内政函数内容全搬运到此处，以便其他cpp调用 ##
// ## 2020/12/12 # 江东新风 # 修正参数类型错误 ##
// ## 2020/09/28 # 氕氘氚 ##

namespace inter
{
    int incomes_difficulty_impact(int n, bool is_player)
    {
        switch (pk::get_scenario().difficulty)
        {
        case 难易度_超级:
            if (is_player)
                n = int(n * 0.75f);
            else
                n = int(n * 1.25f);
            break;

        case 난이도_초급:
            if (is_player)
                n = int(n * 1.25f);
            break;
        }
        return n;
    }

	//100 征兵数量设定.cpp
	int get_recruit_num(pk::building@ building, pk::list<pk::person@>actor_list, int type = 1/*0表示执行，1表示仅计算，不影响兵役*/, bool 征兵数_城市数_修正 = false, bool 势力名声特技修正 = false, float 名声特技_君主名声 = 1.1f, float 名声特技_都督名声 = 1.15f, float 名声特技_太守名声 = 1.2f, bool 皇帝势力修正 = false)
	{
		if (pk::is_alive(building) and actor_list.count != 0 and actor_list[0] !is null)
		{
			int building_id = building.get_id();
			pk::city@ city;
			if (building_id < 据点_城市末) @city = pk::building_to_city(building);//这样港关征兵应该会出问题才是
			int n = 0;
			int sum = 0;
			int mul = 100;
			for (int i = 0; i < actor_list.count; i++)
			{
				pk::person@ actor = actor_list[i];
				if (pk::is_alive(actor))
				{
					sum += actor.stat[int(pk::core["recruit.stat"])];
					// 명성 특기를 보유하고 있을 경우 50% 상승
					if (ch::has_skill(actor, int(pk::core["recruit.skill"])))
						mul += 名声征兵加成;
				}
			}
			//照顾港关征兵，加了默认值
			float barracks_buf = 1.f;
			int porder = 60;
			if (building_id < 据点_城市末 and pk::is_alive(city))
			{
				barracks_buf = func_5c4600(city);
				porder = city.public_order;
			}
			else if (building_id >= 据点_城市末 and building_id < 据点_末)
			{
				BaseInfo@ base_t = @base_ex[building_id];
				porder = base_t.public_order;
			}

			n = int((1000 + (porder + 20) * sum * 5 / 100) * mul / 100 * barracks_buf);
			// 특급 난이도 컴퓨터일 경우 2배
			if (pk::get_scenario().difficulty == 难易度_超级 and !building.is_player())
				n = n * 超级电脑征兵倍率 / 100;
			// 주변에 적 부대가 존재할 경우 반감
			if (pk::enemies_around(building))
				n = n * 周围敌部队征兵倍率 / 100;


			// -----------------------------------------------------------------------------

			// 세력
			pk::force@ force = pk::get_force(building.get_force_id());

			// 세력의 보유 특기 ('18.10.14)
			if (势力名声特技修正 and !pk::is_campaign())
			{
				// 君主持有名声군주가 특기_명성 보유 시 징병량 증가
				pk::person@ p1 = pk::get_person(pk::get_kunshu_id(building));
				if (pk::is_alive(p1))
				{
					if (ch::has_skill(p1, 특기_명성))
						n = int(n * 名声特技_君主名声);
				}
				// 都督持有名声도독이 특기_명성 보유 시 징병량 증가
				pk::person@ p2 = pk::get_person(pk::get_totoku_id(building));
				if (pk::is_alive(p2))
				{
					if (ch::has_skill(p2, 특기_명성))
						n = int(n * 名声特技_都督名声);
				}
				// 太守持有名声태수가 특기_명성 보유 시 징병량 증가
				pk::person@ p3 = pk::get_person(pk::get_taishu_id(building));
				if (pk::is_alive(p3))
				{
					if (ch::has_skill(p3, 특기_명성))
						n = int(n * 名声特技_太守名声);
				}
			}

			// 후한황제 보호세력 또는 군주가 황제인 세력 징병량 증가 ('18.10.14)
			if (皇帝势力修正 and !pk::is_campaign())
			{

				if (pk::is_protecting_the_emperor(force))
					n = int(n * 1.1f);
				else if (force.title == 爵位_皇帝)
					n = int(n * 1.1f);
			}

			// 대도시 징병량 버프 ('19.4.26)
			if (pk::is_alive(city) and !pk::is_campaign())
			{
				if (!开启人口系统)
				{
					if (pk::is_large_city(city))
						n = int(n * 大都市征兵倍率 / 100.f);
					else
					{
						n = int(n * 一般城市征兵倍率 / 100.f);
					}
				}

			}



			// 유저_도시수_패널티 ('19.5.4)
			if (征兵数_城市数_修正 and building.is_player() and !pk::is_campaign())
			{
				float force_city_count = float(pk::get_city_list(force).count);
				n = int(n * (1.f - (force_city_count * 0.01f)));
			}

			if (开启人口系统)
			{
				if (building_id > -1 and building_id < 据点_末)
				{
					BaseInfo@ base_t = @base_ex[building_id];
					int mil_avil = base_t.mil_pop_av;
					
					if (mil_avil < n)
					{
						if (base_t.population <= 5000) return 0;//参考san14，5000人口无法征兵
						if (mil_avil <= 0)
						{
							n = 200;
							if (type == 0)
							{
								ch::add_population(building_id,-2000); //兵役人口不足时，从总人口强征，副作用较大
								
							}
						}
						else if (mil_avil <= 200)
						{
							n = 200;
							if (type == 0)
							{
								ch::add_mil_pop_av(building_id,-mil_avil);// base_t.mil_pop_av -= mil_avil;
								ch::add_population(building_id, -uint32((200.f - mil_avil) * 10.f));// base_t.population -= uint32((200.f - mil_avil)*10.f);//兵役人口不足时，从总人口强征，副作用较大
								
							}
						}
						else
						{
							n = mil_avil;
							if (type == 0)
							{
								ch::add_mil_pop_av(building_id, -mil_avil);
								
							}
						}
						if (!building.is_player() and n <= 1500) return 1500;//电脑的征兵保底
					}
					else if (mil_avil >= n)
					{
						if (type == 0)
						{
							ch::add_mil_pop_av(building_id, -n);
							
						}
						return n;
					}
				}				
			}
			// -----------------------------------------------------------------------------

			return n;
		}
		return 0;
	}

	//征兵数相关函数，统计兵舍等级
	float func_5c4600(pk::city@ city)
	{
		int level1 = 0, level2 = 0;
		for (int i = 0; i < int(city.max_devs); i++)
		{
			pk::building@ building = city.dev[i].building;
			if (pk::is_alive(building))
			{
				switch (building.facility)
				{
				case 시설_병영: building.completed ? level1++ : 0; break;
				case 시설_병영2단: building.completed ? level2++ : level1++; break;
				case 시설_병영3단: building.completed ? 0 : level2++; break;
				}
			}
		}
		if (int(city.barracks_counter) > level1 + level2)
			return 1.5f;
		if (int(city.barracks_counter) > level1)
			return 1.2f;
		return 1.f;
	}

	//101 征兵治安设定.cpp
	int get_recruit_order_dec(pk::building@ building, pk::list<pk::person@>actor_list, int troops, bool 治安变化_城市数_修正 = false)
	{
		int n = 100;
		for (int i = 0; i < actor_list.count; i++)
		{
			pk::person@ actor = actor_list[i];

			if (pk::is_alive(actor))
				n = n + actor.stat[int(pk::core["recruit.stat"])];
		}
		n = -troops / n;

		/*
		// 군주가 [유비]인 경우에 1/2 감소
			if (pk::get_kunshu_id(actors[0]) == 무장_유비)
				n /= 2;

		// 군주가 [유우]인 경우에 1/2 감소
			if (pk::get_kunshu_id(actors[0]) == 무장_유우)
				n /= 2;
		*/


		// 유저_도시수_패널티 ('19.5.4)
		if (治安变化_城市数_修正 and building.is_player() and !pk::is_campaign())
		{
			pk::force@ force = pk::get_force(building.get_force_id());
			float force_city_count = float(pk::get_city_list(force).count);
			n = int(n * (1.f + (force_city_count * 0.01f)));
		}

		n = ch::get_order_change(building, n);
		return n;
	}

	//104 巡查治安设定.cpp
	int get_inspections_order_inc(pk::building@ building, pk::list<pk::person@>actor_list)
	{
		if (pk::is_alive(building) and actor_list[0] !is null)
		{
			bool flag = false;
			if (building.get_id() < 据点_末)
			{
				int n = 0;
				for (int i = 0; i < actor_list.count; i++)
				{
					pk::person@ actor = actor_list[i];
					if (pk::is_alive(actor))
					{
						n = n + actor.stat[int(pk::core["inspection.stat"])];
						if (ch::has_skill(actor, 特技_巡查))
							flag = true;
					}
				}
				n = n / 28 + 2;
				if (pk::enemies_around(building))
					n = n / 2;
				if (flag)
					n = n * 巡查效果倍率 / 100;

				int max_order = 100;
				int order = 0;
				if (building.get_id() < 城市_末) order = pk::building_to_city(building).public_order;
				else order = base_ex[building.get_id()].public_order;
				if (order + n > max_order)
					n = max_order - order;
				return int(n);
			}
		}
		return 0;
	}

	//154 每月治安减少设定
	/*
		const int 整备政令下降机率 = 50;
	const int 治安下降上限 = 5;
	const int 治安下降基数 = 2;
	const int 电脑治安下降倍率 = 50;
	*/
	int get_season_order_dec(pk::building@ building, int 整备政令下降机率 = PUBLIC_ORDER_CHANGE::整备政令下降机率, int 治安下降上限 = PUBLIC_ORDER_CHANGE::治安下降上限, int 治安下降基数 = PUBLIC_ORDER_CHANGE::治安下降基数, int 电脑治安下降倍率 = PUBLIC_ORDER_CHANGE::电脑治安下降倍率)
	{
		if (!pk::is_first_month_of_quarter(pk::get_month()))
			return 0;
		pk::force@ force = pk::get_force(building.get_force_id());
		if (force is null)
			return 0;
		// 법령정비 연구 시 50% 확률로 감소하지 않음
		if (pk::has_tech(force, 기교_법령정비) and pk::rand_bool(100 - 整备政令下降机率))
			return 0;
		int n = 90;
		pk::person@ taishu = pk::get_person(pk::get_taishu_id(building));
		if (pk::is_alive(taishu))
			n = n - taishu.stat[武将能力_魅力];
		n = pk::max(n, 1) / 10 + pk::rand(治安下降基数 * 2);
		n = pk::min(n, 治安下降上限);


		//  电脑势力
		if (!building.is_player())
		{
			n = int(n * 电脑治安下降倍率 / 100.f);
		}


		// 동탁이 군주이면 폭정 효과 적용 ('18.10.4)
		if (pk::get_kunshu_id(force) == 무장_동탁)
			n = int(n * 2.f);

		// 이각이 군주이면 폭정 효과 적용
		if (pk::get_kunshu_id(force) == 무장_이각)
			n = int(n * 2.f);

		// 곽사가 군주이면 폭정 효과 적용
		if (pk::get_kunshu_id(force) == 무장_곽사)
			n = int(n * 2.f);

		// 손호가 군주이면 폭정 효과 적용
		if (pk::get_kunshu_id(force) == 무장_손호)
			n = int(n * 2.f);

		n = ch::get_order_change(building, -n);
		return n;
	}
}
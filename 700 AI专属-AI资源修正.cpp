// ## 2022/02/17 # 江东新风 # 人口上限修正 ##
// ## 2021/11/03 # 江东新风 # 增长率放入301 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/26 # 江东新风 # 区分是否开启人口系统 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误,加回兵力补偿，但会消耗兵役 ##
// ## 2021/10/10 # 江东新风 # 兵力补偿变成人口及总兵役补偿 ##
// ## 2021/02/14 # 白马叔叔 # 添加资源增加上限。超过某一数值不再增加 ##
// ## 2021/01/12 # 江东新风 # 修复Index out of bounds ##
// ## 2020/12/17 # 江东新风 # 补偿针对势力及城市资源量优化 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/06 #江东新风#关閉了AI加資源的提示，现在玩家不会发现AI在作弊了##
// ## 2020/08/01 ##
/*
@제작자: HoneyBee
@설명: 특정시기마다 적 세력의 자원 ADVANTAGE 효과 SCRIPT

※ 적 세력이 보유자원량이 낮아서 뒷심 부족으로 약화되는 것을 완화할 수 있습니다.


*/

namespace 보상_AI
{

	// ================ CUSTOMIZE ================


		// AI 세력이 보상을 얻는 시기 설정
	const int 发动时期 = 0;              // 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일

	// 구체적인 내용 설정 (AI 세력)
	const bool 金补偿_城市 = true;		  	  // 금 상승설정 (true = on, false = off)
	const int gold_1 = 750;		  // 획득 금 (기본: 5000)
	const int gold_max1 = 30000;    //城市金超过上限不再增加

	const bool 兵粮补偿_城市 = true;		  // 병량 상승설정 (true = on, false = off)
	const int food_1 = 5000;		  // 병량 (기본: 20000)
	const int food_max1 = 150000;		  // 城市粮食超过上限不再增加

	const bool 气力补偿_城市 = true;		  // 기력 상승설정 (true = on, false = off)
	const int energy_1 = 20;		  // 기력 (기본: 50)	

	const bool 兵力补偿_城市 = true;		  // 병력 상승설정 (true = on, false = off)
	const int troops_1 = 1500;		  // 병력 (기본: 20000)
	const int troops_max1 = 50000;		  // 城市兵力超过上限不再增加

	const bool 人口补偿_城市 = true;		  // 병력 상승설정 (true = on, false = off)

	const bool 技巧补偿_城市 = true;		  // 기교P 상승설정 (true = on, false = off)
	const int tp_1 = 120;		      // 기교P (기본: 100)	

	const bool 治安补偿_城市 = true;		  // 치안 상승설정 (true = on, false = off)
	const int public_order_1 = 15;	  // 치안 상승치 (기본: 20)

	const bool 兵装补偿_城市 = true;		  // 병기 상승설정 (true = on, false = off)
	const int 枪_1 = 750;		      // 병기(창) (기본: 20000)		
	const int 戟_1 = 750;		      // 병기(극) (기본: 20000)		
	const int 弩_1 = 750;		      // 병기(노) (기본: 20000)		
	const int 战马_1 = 750;		      // 병기(군마) (기본: 20000)
	const int weapon_max1 = 30000;    // 城市兵装超过上限不再增加

////////////////////////港关加資源////////////////////////////////		
		// AI 세력이 보상을 얻는 시기 설정
		//const int 发动时期_港关 = 0;              // 0: 每旬,  1: 每月首旬,  2: 每季首旬,  3: 每年首旬
	const int 发动几率 = 100;                  //建議默认50
	const bool 无太守也加 = true;          //城关没有太守，也可以增加资源

	// 구체적인 내용 설정 (AI 세력)
	const bool 金补偿_港关 = true;		  	  // 금 상승설정 (true = on, false = off)
	const int gold_2 = 250;		  // 획득 금 (기본: 5000)
	const int gold_max2 = 30000;    //城关金超过上限不再增加

	const bool 兵粮补偿_港关 = true;		  // 병량 상승설정 (true = on, false = off)
	const int food_2 = 1500;		  // 병량 (기본: 20000)
	const int food_max2 = 100000;		  // 城关粮食超过上限不再增加

	const bool 气力补偿_港关 = true;		  // 기력 상승설정 (true = on, false = off)
	const int energy_2 = 10;		  // 기력 (기본: 50)	

	const bool 兵力补偿_港关 = true;		  // 병력 상승설정 (true = on, false = off)
	const int troops_2 = 750;		  // 병력 (기본: 20000)
	const int troops_max2 = 15000;		  // 城关兵力超过上限不再增加

	const bool 人口补偿_港关 = true;		  // 병력 상승설정 (true = on, false = off)

	//const bool 技巧补偿_港关 = true;		  // 기교P 상승설정 (true = on, false = off)
	//const int tp_2 = 50;		      // 기교P (기본: 100)	

	const bool 治安补偿_港关 = true;		  // 치안 상승설정 (true = on, false = off)
	const int public_order_2 = 5;	  // 치안 상승치 (기본: 20)

	const bool 兵装补偿_港关 = true;		  // 병기 상승설정 (true = on, false = off)
	const int 枪_2 = 750;		      // 병기(창) (기본: 20000)		
	const int 戟_2 = 750;		      // 병기(극) (기본: 20000)		
	const int 弩_2 = 750;		      // 병기(노) (기본: 20000)		
	const int 战马_2 = 750;		      // 병기(군마) (기본: 20000)
	const int weapon_max2 = 30000;    // 城关兵装超过上限不再增加


// ===========================================
	const bool 补偿效果开关 = false;//是否显示补偿animation


	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{
			auto force_list = pk::list_to_array(pk::get_force_list());
			for (int i = 0; i < int(force_list.length); i++)
			{
				pk::force@ force = force_list[i];

				if (发动时期 == 0)
				{
					search_city(force);  // 매턴
					search_portgate(force);
				}
				else if (发动时期 == 1 and (pk::get_day() == 1))
				{
					search_city(force);  // 매월 1일
					search_portgate(force);
				}
				else if (发动时期 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
				{
					search_city(force);  // 매분기 첫달이면서 1일
					search_portgate(force);
				}
				else if (发动时期 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
				{
					search_city(force);  // 매년 1월 1일
					search_portgate(force);
				}
			}
		}

		void search_city(pk::force@ force)
		{
			array<pk::city@> arr_city_list = pk::list_to_array(pk::get_city_list(force));

			for (int i = 0; i < int(arr_city_list.length); i++)
				reward_set(arr_city_list[i]);

			if (技巧补偿_城市 and !force.is_player()) ch::add_tp(force, tp_1, force.get_pos());
		}

		void search_portgate(pk::force@ force)
		{
			//pk::building@ portgate;
			pk::list<pk::building@> list;
			for (int i = 42; i <= 86; i++)
			{
				pk::building@ building = pk::get_building(i);
				int building_force_id = building.get_force_id();
				if (building_force_id == force.get_force_id())
				{
					list.add(building);
				}
			}
			array<pk::building@> arr_portgate_list = pk::list_to_array(list);
			if (arr_portgate_list.length != 0)
			{
				for (int i = 0; i < int(arr_portgate_list.length); i++)
				{
					reward_set_portgate(arr_portgate_list[i]);
				}
			}
		}

		void reward_set(pk::city@ city)
		{
			if (!city.is_player() and (city.get_taishu_id() != -1))
			{
				pk::force@ force0 = pk::get_force(city.get_force_id());
				pk::person@ kunshu = pk::get_person(force0.kunshu);
				float flontline = is_frontline_base(pk::city_to_building(city)) ? 1.2f : 0.8f;
				float m = flontline * (0.8f + kunshu.ambition * 0.1f) * 1.2f * get_policy_buf(force0.policy);
				int final_gold = int(gold_1 * m * (pk::get_max_gold(city) - city.gold) / pk::get_max_gold(city));
				int final_food = int(food_1 * m * (pk::get_max_food(city) - city.food) / pk::get_max_food(city));
				int final_troop = int(troops_1 * m * (pk::get_max_troops(city) - city.troops) / pk::get_max_troops(city));
				//pk::trace("gold:"+final_gold+",food:"+final_food+",troops"+final_troop);
				if (金补偿_城市 and city.gold < gold_max1) pk::add_gold(city, final_gold, 补偿效果开关);
				if (兵粮补偿_城市 and city.food < food_max1) pk::add_food(city, final_food, 补偿效果开关);
				if (气力补偿_城市) pk::add_energy(city, energy_1, 补偿效果开关);
				
				if (治安补偿_城市) pk::add_public_order(city, public_order_1, 补偿效果开关);
				if (开启人口系统 and 人口补偿_城市)
				{
					int pop_max = pk::is_large_city(city) ? 750000 : 370000;
					int base_id = city.get_id();
					BaseInfo@ city_t = @base_ex[base_id];

					if (int(city_t.population) < pop_max)
					{
						int 发动倍率 = 0;
						switch (发动时期)
						{
						case 0: 发动倍率 = 1; break;
						case 1: 发动倍率 = 3; break;
						case 2: 发动倍率 = 9; break;
						case 3: 发动倍率 = 36; break;
						default: 发动倍率 = 1; break;
						}
						//if (city.get_id() == 城市_襄阳) pk::trace(pk::format("ai加资源前,襄阳人口：{}", city_t.population));
						float inc_rate = ch::get_pop_inc_rate(base_id, 人口基础增长率 * 发动倍率);
						ch::add_population(base_id, int(city_t.population * inc_rate),true);
						//if (city.get_id() == 城市_襄阳) pk::trace(pk::format("ai加资源后,襄阳人口：{}", city_t.population));
					}
				}
				if (开启人口系统)
				{
					if (兵力补偿_城市 and city.troops < troops_max1) ch::add_troops(city, final_troop, 补偿效果开关, 104);
				}
				else
				{
					if (兵力补偿_城市 and city.troops < troops_max1) ch::add_troops(city, final_troop, 补偿效果开关);
				}
				if (兵装补偿_城市)
				{
					if (city.weapon_amount[1] < weapon_max1) pk::add_weapon_amount(city, 兵器_枪, 枪_1, 补偿效果开关);
					if (city.weapon_amount[2] < weapon_max1) pk::add_weapon_amount(city, 兵器_戟, 戟_1, 补偿效果开关);
					if (city.weapon_amount[3] < weapon_max1) pk::add_weapon_amount(city, 兵器_弩, 弩_1, 补偿效果开关);
					if (city.weapon_amount[4] < weapon_max1) pk::add_weapon_amount(city, 兵器_战马, 战马_1, 补偿效果开关);
				}

				city.update();
			}
		}

		void reward_set_portgate(pk::building@ building)
		{
			bool 发动 = pk::rand_bool(发动几率);
			if (!building.is_player() and ((pk::get_taishu_id(building) != -1) or 无太守也加) and 发动)
			{
				if (building.get_id() < 52)  //为关卡
				{
					pk::gate@ gate_0 = pk::building_to_gate(building);
					if (金补偿_港关 and gate_0.gold < gold_max2) pk::add_gold(building, gold_2, 补偿效果开关);
					if (兵粮补偿_港关 and gate_0.food < food_max2) pk::add_food(building, food_2, 补偿效果开关);
					if (气力补偿_港关) pk::add_energy(building, energy_2, 补偿效果开关);
					
					//if (治安补偿_港关) pk::add_public_order(city, public_order_1, true);
					if (开启人口系统 and 人口补偿_港关)
					{
						int pop_max = 90000;
						int base_id = building.get_id();
						BaseInfo@ base_t = @base_ex[base_id];

						if (int(base_t.population) < pop_max)
						{
							int 发动倍率 = 0;
							switch (发动时期)
							{
							case 0: 发动倍率 = 1; break;
							case 1: 发动倍率 = 3; break;
							case 2: 发动倍率 = 9; break;
							case 3: 发动倍率 = 36; break;
							default: 发动倍率 = 1; break;
							}
							float inc_rate = ch::get_pop_inc_rate(base_id, 人口基础增长率 * 发动倍率);
							ch::add_population(base_id, int(base_t.population * inc_rate), true);
							//ch::add_population(building.get_id(), int(base_t.population * 人口基础增长率 * 发动倍率), true);
						}

					}
					if (开启人口系统)
					{
						if (兵力补偿_港关 and gate_0.troops < troops_max2) ch::add_troops(building, troops_2, 补偿效果开关, 104);
					}
					else
					{
						if (兵力补偿_港关 and gate_0.troops < troops_max2) ch::add_troops(building, troops_2, 补偿效果开关);
					}

					if (兵装补偿_港关)
					{
						if (gate_0.weapon_amount[1] < weapon_max2) pk::add_weapon_amount(building, 兵器_枪, 枪_2, 补偿效果开关);
						if (gate_0.weapon_amount[2] < weapon_max2) pk::add_weapon_amount(building, 兵器_戟, 戟_2, 补偿效果开关);
						if (gate_0.weapon_amount[3] < weapon_max2) pk::add_weapon_amount(building, 兵器_弩, 弩_2, 补偿效果开关);
						if (gate_0.weapon_amount[4] < weapon_max2) pk::add_weapon_amount(building, 兵器_战马, 战马_2, 补偿效果开关);
					}

					gate_0.update();
				}
				else
				{
					pk::port@ port_0 = pk::building_to_port(building);
					if (金补偿_港关 and port_0.gold < gold_max2) pk::add_gold(building, gold_2, 补偿效果开关);
					if (兵粮补偿_港关 and port_0.food < food_max2) pk::add_food(building, food_2, 补偿效果开关);
					if (气力补偿_港关) pk::add_energy(building, energy_2, 补偿效果开关);				
					if (治安补偿_港关) ch::add_public_order(building, public_order_2, 补偿效果开关);
					if (人口补偿_港关)
					{
						int pop_max = 180000;
						BaseInfo@ base_t = @base_ex[building.get_id()];

						if (int(base_t.population) < pop_max)
						{
							int 发动倍率 = 0;
							switch (发动时期)
							{
							case 0: 发动倍率 = 1; break;
							case 1: 发动倍率 = 3; break;
							case 2: 发动倍率 = 9; break;
							case 3: 发动倍率 = 36; break;
							default: 发动倍率 = 1; break;
							}
							ch::add_population(building.get_id(), int(base_t.population * 人口基础增长率 * 发动倍率), true);
						}

					}
					if (兵力补偿_港关 and port_0.troops < troops_max2) ch::add_troops(building, troops_2, 补偿效果开关, 104);
					if (兵装补偿_港关)
					{
						if (port_0.weapon_amount[1] < weapon_max2) pk::add_weapon_amount(building, 兵器_枪, 枪_2, 补偿效果开关);
						if (port_0.weapon_amount[2] < weapon_max2) pk::add_weapon_amount(building, 兵器_戟, 戟_2, 补偿效果开关);
						if (port_0.weapon_amount[3] < weapon_max2) pk::add_weapon_amount(building, 兵器_弩, 弩_2, 补偿效果开关);
						if (port_0.weapon_amount[4] < weapon_max2) pk::add_weapon_amount(building, 兵器_战马, 战马_2, 补偿效果开关);
					}

					port_0.update();
				}
			}
			//港关周围有敌军时，无視条件加兵
			if (!building.is_player() and (pk::get_taishu_id(building) != -1) and !发动 and pk::enemies_around(building))
			{
				ch::add_troops(building, troops_2, 补偿效果开关);
				pk::add_energy(building, energy_2, 补偿效果开关);
				pk::add_weapon_amount(building, 兵器_枪, 枪_2, 补偿效果开关);
				pk::add_weapon_amount(building, 兵器_戟, 戟_2, 补偿效果开关);
				pk::add_weapon_amount(building, 兵器_弩, 弩_2, 补偿效果开关);
				pk::add_weapon_amount(building, 兵器_战马, 战马_2, 补偿效果开关);
			}
		}

		bool is_frontline_base(pk::building@ building)
		{
			for (int i = 0; i < 据点_末; i++)
			{
				if (pk::is_neighbor_base(building.get_id(), i) and pk::is_enemy(building, pk::get_building(i)))
					return true;
			}
			array<pk::point> arr = pk::range(building.pos, 1, 6);
			for (int i = 0; i < int(arr.length); i++)
			{
				pk::unit@ unit0 = pk::get_unit(arr[i]);
				if (unit0 !is null)
				{
					if (pk::is_enemy(building, unit0)) return true;
				}
			}
			return false;
		}
		float get_policy_buf(int policy)
		{
			float buf;
			switch (policy)
			{
			case 方针_中华统一: buf = 1.f; break;
			case 方针_地方统一: buf = 0.9f; break;
			case 方针_州统一: buf = 0.8f; break;
			case 方针_现状维持: buf = 0.7f; break;
			case 方针_吴越割据: buf = 0.9f; break;
			case 方针_巴蜀割据: buf = 0.9f; break;
			default: buf = 0.7f; break;
			}

			return buf;
		}
	} // class Main

	Main main;

} // namespace
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/29 # 江东新风 # 新特技-辎粮##
// ## 2020/10/23 # 江东新风 # 同步马术书大神的更新##
// ## 2020/09/21 # 江东新风 #新增故障追踪选项##
// ## 2020/08/19 # 江东新风 #新特技-后勤##
// ## 2020/08/16 # 江东新风 #has_skill函数替換##
// ## 2020/07/26 ##
namespace UNIT_FOOD_USE
{
	//---------------------------------------------------------------------------

	const bool 兵粮焚烧 = true;
	const int 基础耗粮 = 100;  //1000人一旬的基础耗粮
	const int 运输队耗粮倍率 = 100;
	const int 阵耗粮倍率 = 83;
	const int 砦耗粮倍率 = 67;
	const int 城塞耗粮倍率 = 50;
	const int 屯田耗粮倍率 = 80;
	const bool debug_mod_后勤 = false;

	//---------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(165, pk::func165_t(callback));
		}

		int callback(pk::unit@ unit)
		{
			if (!pk::is_alive(unit))
				return 0;


			int n = 0;
			int m = 100;
			float burn = 0.f;

<<<<<<< HEAD
			if (unit.type == 부대종류_전투)
=======
			if (unit.type == 部队类型_战斗)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			{
				int facility_id = func_49d8e0(unit);
				if (facility_id == 시설_성채)  //城塞
					m = 城塞耗粮倍率;
				else if (facility_id == 시설_요새)  //砦
					m = 砦耗粮倍率;
				else if (facility_id == 시설_진)  //阵
					m = 阵耗粮倍率;
				else
					m = 100;
			}
<<<<<<< HEAD
			if (unit.type == 부대종류_수송)
=======
			if (unit.type == 部队类型_运输)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			{
				m = int(m * 运输队耗粮倍率 / 100.f);
			}


			if (兵粮焚烧 and unit.is_on_fire())
				burn = (6.f - unit.attr.stat[部队能力_政治] / 20.f) * unit.food / 100;


			n = int(unit.troops * m / 100.f * 基础耗粮 / 1000);   // 기본 병량 소비

			if (pk::get_season() == 계절_겨울)      // 겨울철 병량 소비 1.2배 ('18.10.10)
				n = int(n * 1.2f);

			if (pk::get_hex(unit.get_pos()).terrain == 지형_산)
				n = int(n * 1.2f);               // 부대가 산악 행군 시 병량 소비 1.2배 ('18.10.18)

			if (ch::has_skill(unit, 특기_둔전))    //둔전 특기 : 부대 병량 소비 반감 ('18.10.5)
				n = n * 屯田耗粮倍率 / 100;

			if (ch::has_skill(unit, 特技_辎粮))
				n = n * 辎粮耗粮倍率 / 100;


			n = int(n + burn);
			//ch::u8debug(pk::format("开始计算部队id{}的耗量",unit.get_id()));
			//后勤特技----------注意，可能有bug，因为这个可能只是计算过程，会反复算
			if (ch::has_skill(unit, 特技_后勤) and unit.type == 部队类型_战斗)
			{
				//ch::u8debug(pk::format("部队(id{}),武将持有后勤特技",unit.get_id()));
				pk::person@src_leader = pk::get_person(unit.leader);
				pk::building@ src_building = pk::get_building(src_leader.service);
				if (is_in_building_range(unit))
				{
					//老版后勤，耗粮减半由所属据点提供
					//ch::u8debug("in building range");
					if (pk::get_food(src_building) > 20000 and pk::get_food(src_building) > n / 2)
					{
						//ch::u8debug("据点粮食大于20000，触发后勤特效1：耗量减半由据点支出");
						pk::add_food(src_building, -n / 2, true);
						n = n / 2;
					}

					int a = can_supply_amount(unit);
					int b = need_supply_amount(unit);
					if (a > 0 and b > 0)
					{
						//ch::u8debug(pk::format("部队(id{}),据点最大可补给量为{}, 需要兵力补给量为{}.", unit.get_id(), a, b));
						int supply_amount = pk::min(a, b);
						ch::add_troops(src_building, -supply_amount, false);
						//pk::add_food(building, -supply_amount * 2, false);
						pk::add_weapon_amount(src_building, unit.weapon, -supply_amount, false);

						int new_energy = (unit.troops * unit.energy + supply_amount * pk::get_energy(src_building)) / (unit.troops + supply_amount);
						ch::add_troops(unit, supply_amount, true);
						pk::add_energy(unit, new_energy - unit.energy, true);
						//pk::add_food(unit, supply_amount * 2, true);

					}


				}
				else
				{
					//ch::u8debug("附近没有可提供补给的建筑物");
				}

			}

			if (n <= 0 and unit.troops > 0)
				return 1;
			return n;
		}

		/***/
		int func_49d8e0(pk::unit@ unit)
		{
			int force_id = unit.get_force_id();
			pk::force@ force = pk::get_force(force_id);

			if (!pk::is_alive(force))
				return -1;

			int facility_id = 시설_진;

			if (pk::has_tech(force, 기교_성벽강화))
				facility_id = 시설_성채;
			else if (pk::has_tech(force, 기교_시설강화))
				facility_id = 시설_요새;

			array<pk::point> arr = pk::range(unit.get_pos(), 1, pk::get_facility(facility_id).max_range);

			for (int i = 0; i < int(arr.length); i++)
			{
				pk::building@ building = pk::get_building(arr[i]);
				if (pk::is_alive(building) and building.facility == facility_id and building.completed and building.get_force_id() == force_id)
					return facility_id;
			}

			return -1;
		}

		bool is_in_building_range(pk::unit@unit)
		{
			array<pk::point> arr = pk::range(unit.get_pos(), 1, 4);
			for (int i = 0; i < int(arr.length); i++)
			{
				pk::building@ src_building = pk::get_building(arr[i]);
				if (src_building !is null)
				{
					//ch::u8debug(pk::format("部队(id{}),周边有可提供补给的建筑",unit.get_id()));
					if (unit.get_force_id() == src_building.get_force_id())
					{
						if (src_building.facility == 3)
						{
							//ch::u8debug(pk::format("为部队(id{})提供补给的建筑为阵",unit.get_id()));
							if (pk::get_distance(unit.pos, src_building.pos) <= 2) return true;
						}
						if (src_building.facility == 4)
							if (pk::get_distance(unit.pos, src_building.pos) <= 3) return true;
						if (src_building.facility == 5)
							if (pk::get_distance(unit.pos, src_building.pos) <= 4) return true;
					}
				}

			}
			return false;
		}

		int need_supply_amount(pk::unit@ unit)
		{
			pk::person@ leader = pk::get_person(unit.leader);
			int max_troops = pk::get_max_troops(unit);
			int current_troops = unit.troops;
			return pk::min(max_troops * 2 / 100, max_troops - current_troops);
		}

		int can_supply_amount(pk::unit@ unit)
		{
			pk::person@src_leader = pk::get_person(unit.leader);
			pk::building@ building = pk::get_building(src_leader.service);
			int max_food_supply = pk::get_food(building) - 20000;
			int max_troops_supply = pk::get_troops(building) - 10000;
			int n = pk::min(max_food_supply, max_troops_supply);

			if (unit.weapon < 1 or unit.weapon > 4)
			{
				return n;
			}
			else
			{
				int weapon_amount = pk::get_weapon_amount(building, unit.weapon);
				return pk::min(weapon_amount, n);
			}
		}
	}

	Main main;
}
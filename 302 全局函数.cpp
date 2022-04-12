<<<<<<< HEAD
﻿// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
=======
﻿// ## 2022/02/17 # 江东新风 # 部分人口函数移入 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/10/24 # 江东新风 # get_stronghold_list改为get_base_list ##
// ## 2021/10/10 # 江东新风 # 治安增减的非城市据点函数 ##
// ## 2021/03/02 # 江东新风 # 特殊地名重建通用函数，cpp里调用kill_building都需要加上 ##
// ## 2021/02/04 # 江东新风 # 添加设置武器及马匹参数的函数 ##
// ## 2021/01/31 # 江东新风 # rand函数使用错误修复 ##
// ## 2021/01/23 # 江东新风 # get_order_change building版 ##
// ## 2021/01/14 # 江东新风 # 关掉trace ##
// ## 2021/01/13 # 江东新风 # 修复特殊设施闪退bug2 ##
// ## 2021/01/12 # 江东新风 # 修复特殊设施闪退bug ##
// ## 2021/01/11 # 江东新风 # 将特殊设施不被破坏的相关函数移入此处 ##
// ## 2021/01/06 # 江东新风 # 新增get_weapon_name函数 ##
// ## 2021/01/02 # 江东新风 # 新增rand_bool_1000和rand_bool_10000 ##
// ## 2020/12/23 # 江东新风 # 身份中文化 ##
// ## 2020/12/13 # 江东新风 # 添加获取城市港关list的函数 ##
// ## 2020/12/12 # 江东新风 # 修正参数类型错误 ##
// ## 2020/11/30 # 江东新风 # 新函数 get_force_food,get_force_gold, get_force_weapon ##
// ## 2020/10/27 # 江东新风 # 新函数 who_has_skill ##
// ## 2020/10/23 # 氕氘氚 # 取消全部ch::has_skill的组合特技判断 ##
// ## 2020/10/04 # 氕氘氚 # 再次修复宝物特技不生效的bug ##
// ## 2020/10/03 # 氕氘氚 # 修复add_tp在扣技巧时也翻倍的bug ##
// ## 2020/09/23 # 江东新风 # utf8版本u8debug ##
// ## 2020/09/21 # 氕氘氚 #修改函数has_skill, 武将行动时也生效##
// ## 2020/09/15 # 江东新风 #新增针对部队成员计算特技的has_skill版本，用于修复163移动力显示异常的bug##
// ## 2020/08/29 # 氕氘氚 #  添加函数debug
// ## 2020/08/21 # 氕氘氚 #  添加函数set_status
// ## 2020/08/18 # 氕氘氚 # 添加函数get_best_member_stat，添加get_order_change、add_public_order，特技法治影响治安下降 ##

const bool DEBUG_MODE = false;
const int DEBUG_LEVEL = -1;


namespace ch
{
	//  打印調試信息
	void trace(string trace_info)
	{
		if (DEBUG_MODE) pk::trace(trace_info);
	}
	void debug(string debug_info)
	{
		if (0 > DEBUG_LEVEL) pk::trace("[debug] " + debug_info);
	}
	void debug(int level, string debug_info)
	{
		if (level > DEBUG_LEVEL) pk::trace("[debug] " + debug_info);
	}

	//  打印調試信息u8版本
	void u8trace(string trace_info)
	{
		if (DEBUG_MODE) pk::trace(trace_info);
	}
	void u8debug(string debug_info)
	{
		if (0 > DEBUG_LEVEL) pk::trace("[debug] " + debug_info);
	}
	void u8debug(int level, string debug_info)
	{
		if (level > DEBUG_LEVEL) pk::trace("[debug] " + debug_info);
	}


	int randint(int start, int end)
	{
		return start + pk::rand(end - start + 1);
	}

	float randfloat(float start, float end)
	{
		int s = int(start * 100.f), e = int(end * 100.f);
		return (s + pk::rand(e - s + 1)) / 100.f;
	}

	bool rand_bool_1000(int rate)
	{
		rate = pk::max(0, pk::min(999, rate));
		if (int(pk::rand(1000)) < rate)
			return true;
		return false;
	}

	bool rand_bool_10000(int rate)
	{
		rate = pk::max(0, pk::min(9999, rate));
		if (int(pk::rand(10000)) < rate)
			return true;
		return false;
	}

	//  与0比較，至少为0
	int minZero(int value)
	{
		return pk::max(0, value);
	}

	//  设置上下限
	int inRange(int value, int min, int max)
	{
		return pk::max(min, pk::min(max, value));
	}
	//  设置上下限
	float inRange(float value, float min, float max)
	{
		return pk::max(min, pk::min(max, value));
	}

	uint32 set_bit(uint32 i, int pos, bool value)
	{
		uint32 t = value ? 1 : 0;
		i += t << pos;
		return i;
	}

	bool test_bit(uint32 i, int pos)
	{
		return (((i << (31 - pos)) >> 31) == 1);
	}

	bool has_skill(pk::person@ person, int skill_id)
	{
		return pk::has_skill(person, skill_id);
	}

	//新增针对部队成员计算特技的has_skill版本，用于修复163移动力显示异常的bug
	bool has_skill(const pk::detail::arrayptr<pk::person@>& in member, int skill_id)
	{
		return pk::has_skill(member, skill_id);
	}

	bool has_skill(const array<pk::person@>& member, int skill_id)
	{
		return pk::has_skill(member, skill_id);
	}

	bool has_skill(pk::unit@ unit, int skill_id)
	{
		//pk::trace(pk::format("开始判断部队的特技{}持有与否",skill_id));
		if (unit is null)
		{
			//pk::trace("部队不存在");
			return false;
		}
		return unit.has_skill(skill_id);
	}

	bool has_skill(pk::list<pk::person@>list, int skill_id)
	{
		for (int i = 0; i < list.count; i++)
		{
			pk::person@ person = list[i];
			if (pk::has_skill(person, skill_id)) return true;
		}
		return false;
	}
	/**
	 * @param only_not_absent: 只在武将空閑时生效
	*/
	bool has_skill(pk::building@ building, int skill_id, bool only_not_absent = false)
	{
		if (building.facility > 2) return false;
		auto defender_list = pk::get_person_list(building, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
		if (defender_list.count > 0)
		{
			for (int i = 0; i < defender_list.count; i++)
			{
				pk::person@ defender = defender_list[i];
				if ((!only_not_absent) or (!pk::is_unitize(defender) and !pk::is_absent(defender)))
				{
					if (pk::has_skill(defender, skill_id)) return true;
				}
			}
		}
		return false;
	}

	bool has_skill(pk::force@ force, int skill_id)
	{
		if (!pk::is_alive(force)) return false;
		auto person_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)));
		if (person_list.length > 0)
		{
			for (int i = 0; i < int(person_list.length); i++)
			{
				pk::person@ person = person_list[i];
				if (pk::has_skill(person, skill_id)) return true;
			}
		}
		return false;
	}

	int who_has_skill(pk::unit@ unit, int skill_id)
	{
		if (unit is null) return -1;
		if (skill_id < 0 || skill_id > 250)  return -1;
		if (skill_id <= 250)
		{
			for (int m = 0; m < 3; m++)
			{
				if (pk::is_valid_person_id(unit.member[m]))
				{
					pk::person@ member_t = pk::get_person(unit.member[m]);  //隊伍中的武將
					if (member_t is null || !pk::is_alive(member_t)) continue;
					if (has_skill(member_t, skill_id)) return unit.member[m];
				}

			}
		}
		return -1;
	}

	int who_has_skill(pk::building@ building, int skill_id, bool only_not_absent = false)
	{
		if (building.get_id() > 87) return -1;
		auto defender_list = pk::list_to_array(pk::get_person_list(building, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)));
		if (defender_list.length > 0)
		{
			for (int i = 0; i < int(defender_list.length); i++)
			{
				pk::person@ defender = defender_list[i];
				if ((!only_not_absent) or (!pk::is_unitize(defender) and !pk::is_absent(defender)))
				{
					if (pk::has_skill(defender, skill_id)) return defender.get_id();
				}
			}
		}
		return -1;
	}

	int get_best_member_stat(pk::unit@ unit, int stat_type)
	{
		return pk::get_best_member_stat(unit, stat_type);
	}
	int get_best_member_stat(pk::unit@ unit, int skill_id, int stat_type)
	{
		return pk::get_best_member_stat(unit, skill_id, stat_type);
	}

	pk::person@ get_best_stat_member(pk::building@src, int stat_type)
	{
		pk::list<pk::person@>list = pk::get_person_list(src, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
		return get_best_stat_member(list,stat_type);
	}

	pk::person@ get_best_stat_member(pk::list<pk::person@> list, int stat_type)
	{
		if (list.count == 0) return null;
		int best_stat = 0;
		pk::person@ best_member;
		for (int i = 0; i < list.count; ++i)
		{
			if (int(list[i].stat[stat_type]) > best_stat)
			{
				best_stat = list[i].stat[stat_type];
				@best_member = @list[i];
			}
		}
		return best_member;
	}

	int get_best_member_stat(pk::list<pk::person@> list, int stat_type)
	{
		if (list.count == 0) return -1;
		int best_stat = 0;
		for (int i = 0; i < list.count; ++i)
		{
			if (int(list[i].stat[stat_type]) > best_stat) best_stat = list[i].stat[stat_type];
		}
		return best_stat;
	}

	int get_best_member_stat(pk::list<pk::person@> list, int skill_id, int stat_type)
	{
		if (list.count == 0) return -1;
		int best_stat = 0;
		for (int i = 0; i < list.count; ++i)
		{
			if (has_skill(list[i], skill_id))
			{
				if (int(list[i].stat[stat_type]) > best_stat) best_stat = list[i].stat[stat_type];
			}
		}
		return best_stat;
	}

	int get_order_change(pk::city@ city, int value)
	{
		if (value == 0)
			return 0;
		else if (value < 0)
		{
			if (has_skill(pk::city_to_building(city), 特技_法治))
				value /= 2;
		}
		return value;
	}

	int get_order_change(pk::building@ building, int value)
	{
		if (value == 0)
			return 0;
		else if (value < 0)
		{
			if (has_skill(building, 特技_法治))
				value /= 2;
		}
		return value;
	}

	int add_public_order(pk::city@ city, int value, bool effect = false)
	{
		value = get_order_change(city, value);
		return pk::add_public_order(city, value, effect);
	}

	int add_public_order(pk::building@ building, int value, bool effect = false)
	{
		int building_id = building.get_id();
		value = get_order_change(building, value);
		if (building_id > -1 and building_id < 据点_城市末)
		{
			value = get_order_change(building, value);
			return pk::add_public_order(pk::building_to_city(building), value, effect);
		}
		if (building_id >= 据点_城市末 and building_id < 据点_末)
		{
			value = get_order_change(building, value);
			//BaseInfo@ base_t = @base_ex[building_id];
			int p_order = base_ex[building_id].public_order;
			int change_value = ((p_order + value) >= 100) ? (100 - p_order) : (((p_order + value) <= 0) ? (-p_order) : value);
			//int change_value = ((base_t.public_order + value) >= 100) ? (100 - base_t.public_order) : (((base_t.public_order + value) <= 0) ? (-base_t.public_order) : value);
			base_ex[building_id].public_order += change_value;
			//base_t.public_order = base_t.public_order + change_value;
			//
			if (effect) pk::combat_text(change_value, 6, building.pos);//1.value,2.effct_id,3.pos
			return change_value;
		}
		return 0;

	}

	int add_tp(pk::force@ force, int tp, pk::point pos)
	{
		if (tp > 0) return pk::add_tp(force, tp * 2, pos);
		else return pk::add_tp(force, tp, pos);
	}

<<<<<<< HEAD
=======
	int add_revenue_bonus(int base_id, int value)
	{
		if (base_id > -1 and base_id < 据点_末)
		{
			BaseInfo@ base_t = @base_ex[base_id];
			uint16 max = base_id < 城市_末 ? 300 : 150;
			if (base_t.revenue_bonus > max) return 0;
			if (uint16(base_t.revenue_bonus + value) > max) value = max - base_t.revenue_bonus;//人口下限
			base_t.revenue_bonus += value;
			return value;
		}
		return 0;
	}

	int add_harvest_bonus(int base_id, int value)
	{
		if (base_id > -1 and base_id < 据点_末)
		{
			BaseInfo@ base_t = @base_ex[base_id];
			uint16 max = base_id < 城市_末 ? 2500 : 1250;
			if (base_t.harvest_bonus > max) return 0;
			if (uint16(base_t.harvest_bonus + value) > max) value = max - base_t.harvest_bonus;//人口下限
			base_t.harvest_bonus += value;
			return value;
		}
		return 0;
	}

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	int add_population(int base_id, int value, bool include_mil_pop = false)
	{
		if (base_id > -1 and base_id < 据点_末)
		{
			BaseInfo@ base_t = @base_ex[base_id];
<<<<<<< HEAD
			if ((base_t.population + value) < 5000) value = 5000 - base_t.population;//人口下限
			//if (base_id == 城市_襄阳) pk::trace(pk::format("add_population,襄阳人口：{}，value:{}", base_t.population, value));
			base_t.population += value;
			if (include_mil_pop) base_t.mil_pop_all += uint32(value * 0.3f);

=======
			int pre_population = base_t.population;
			if ((pre_population + value) < 5000) value = 5000 - pre_population;//人口下限
			//if (base_id == 城市_襄阳) pk::trace(pk::format("add_population,襄阳人口：{}，value:{}", base_t.population, value));

			base_t.population += value;
			if (include_mil_pop) base_t.mil_pop_all += uint32(value * 0.3f);

			if (value > 0 and base_id < 城市_末)//当人口增长超过阈值时城市升级
			{
				bool upgrade = false;
				int level = 0;
				if (pre_population < 一级城市阈值 and (pre_population + value) >= 一级城市阈值) { upgrade = true; level = 1; }
				if (pre_population < 二级城市阈值 and (pre_population + value) >= 二级城市阈值) { upgrade = true; level = 2; }
				if (pre_population < 三级城市阈值 and (pre_population + value) >= 三级城市阈值) { upgrade = true; level = 3; }
				if (pre_population < 四级城市阈值 and (pre_population + value) >= 四级城市阈值) { upgrade = true; level = 4; }
				if (upgrade) set_interior_land(base_id);
			}
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			return value;
		}
		return 0;
	}

<<<<<<< HEAD
=======
	float get_pop_inc_rate(int base_id, float pre_rate)
	{
		BaseInfo@ base_t = @base_ex[base_id];
		//if (base_id == 城市_襄阳) pk::trace(pk::format("onNewDay 1,襄阳人口：{}", base_t.population));
		int public_order = 0;
		if (base_id < 城市_末) public_order = pk::get_city(base_id).public_order;
		else public_order = base_t.public_order;
		float porder_effect = (public_order - 70) / 30;

		float population_max = base_id < 城市_末 ? (pk::is_large_city(pk::get_city(base_id)) ? 800000.f : 400000.f) : 100000.f;
		float population_t = base_t.population;
		//pk::trace(pk::format("population_max:{},population_t:{}", population_max, population_t));
		float final_rate = 0;
		float buf = 0;

		//掌握人心科技
		if (pk::get_building(base_id).has_tech(技巧_掌握人心)) buf += pre_rate * 0.2f;
		//所以人口增长受到治安及人口上限影响
		if ((population_t / population_max) < 0.25f) final_rate = (pre_rate + 0.02f + buf) * porder_effect;
		else if ((population_t / population_max) < 0.5f) final_rate = (pre_rate + buf) * porder_effect;
		else if ((population_t / population_max) < 0.7f) final_rate = ((pre_rate + buf) / 1.5f) * porder_effect;
		else if ((population_t / population_max) < 0.9f) final_rate = ((pre_rate + buf) / 3.f) * porder_effect;
		else if ((population_t / population_max) < 1.f) final_rate = ((pre_rate + buf) / 10.f) * porder_effect;
		else final_rate = (人口基础增长率 / 20.f) * porder_effect;
		return final_rate;
	}

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	int add_troops(pk::unit@ unit, int value, bool effect = false, int type = -1)
	{
		int n = cast<pk::func58_t>(pk::get_func(58))(unit, value, type);
		if (effect)
			pk::combat_text(n, 1, unit.pos);
		return n;
	}

	int add_troops(pk::city@ city, int value, bool effect = false, int type = -1)
	{
		int n = 0;
		if (type == 104 and value > 0)//104暂定为征兵模式，伴随兵役人口变化
		{
			BaseInfo@ base_t = @base_ex[city.get_id()];
			if (base_t.mil_pop_av >= 0)
			{
				value = pk::min(base_t.mil_pop_av, value);//如果值小于可用兵役，改写value
			}
			n = cast<pk::func59_t>(pk::get_func(59))(city, value, type);
			add_mil_pop_av(city.get_id(), -n);

		}
		else if (type == 105 and value > 0)//105暂定为募兵模式，伴随总兵役人口变化
		{
			BaseInfo@ base_t = @base_ex[city.get_id()];
			if (base_t.population >= 0 and base_t.mil_pop_all >= 0)
			{
				value = pk::min(pk::min(base_t.mil_pop_all, base_t.population), int(0.5f * value)) * 2;//如果值小于总兵役人口，改写value
			}
			n = cast<pk::func59_t>(pk::get_func(59))(city, value, type);
			add_mil_pop_all(city.get_id(), -int(0.5f * n)); //base_t.mil_pop_all -= int(0.5f * n);
			add_population(city.get_id(), -int(0.5f * n)); //base_t.population -= int(0.5f * n);
		}
		else
		{
			n = cast<pk::func59_t>(pk::get_func(59))(city, value, type);
		}

		if (effect)
			pk::combat_text(n, 1, city.pos);
		return n;
	}

	int add_troops(pk::building@ building, int value, bool effect = false, int type = -1)
	{
		int n = 0;
		int building_id = building.get_id();
		if (type == 104 and value > 0)//104暂定为征兵模式，伴随兵役人口变化
		{
			BaseInfo@ base_t = @base_ex[building_id];
			if (base_t.mil_pop_av >= 0)
			{
				value = pk::min(base_t.mil_pop_av, value);//如果值小于可用兵役，改写value
			}
			n = cast<pk::func60_t>(pk::get_func(60))(building, value, type);
			add_mil_pop_all(building_id, -n); //base_t.mil_pop_av -= n;

		}
		else if (type == 105 and value > 0)//104暂定为征兵模式，伴随总兵役人口变化
		{
			BaseInfo@ base_t = @base_ex[building_id];
			if (base_t.mil_pop_av >= 0)
			{
				value = pk::min(base_t.mil_pop_all, value);//如果值小于总兵役人口，改写value
			}
			n = cast<pk::func60_t>(pk::get_func(60))(building, value, type);
			add_mil_pop_all(building_id, -n);
		}
		else
		{
			n = cast<pk::func60_t>(pk::get_func(60))(building, value, type);
		}

		if (effect and n != 0)
			pk::combat_text(n, 1, building.pos);
		return n;
	}

	int add_mil_pop_all(int base_id, int value)//设定总兵役上限不超过人口的1/3
	{
		BaseInfo@ base_a = @base_ex[base_id];
		if (value >= 0)
		{
			if (base_a.population <= uint32((base_a.mil_pop_all + value) * 3)) value = int(base_a.population / 3 - base_a.mil_pop_all);
		}
		else
		{
			if ((base_a.mil_pop_all + value) < 0) value = -base_a.mil_pop_all;
		}
		base_a.mil_pop_all += value;
		return value;
	}

	int add_mil_pop_av(int base_id, int value)//设定现兵役上限不超过人口的1/4
	{
		BaseInfo@ base_a = @base_ex[base_id];
		int pre_value = value;
		if (value >= 0)
		{
			if (base_a.population <= uint32((base_a.mil_pop_av + value) * 4)) value = int(base_a.population / 4 - base_a.mil_pop_av);
		}
		else
		{
			if ((base_a.mil_pop_av + value) < 0) value = -base_a.mil_pop_av;
		}
		base_a.mil_pop_av += value;
		return value;
	}
	/* 设置部队状态
	@param type      0 计略晕, 1 物理晕, 2 自然晕*/
	bool set_status(pk::unit@ unit, pk::unit@ by, int status, int time, int type, bool sound = true)
	{
		if (status == 部队状态_混乱 && type > 0 && has_skill(unit, 特技_治军))     //治军免物理晕
		{
			trace("zhi jun");
			return false;
		}
		pk::set_status(unit, by, status, time, sound);
		return true;
	}

	int get_force_gold(pk::force@ force)
	{
		//分别获取城，港关，部队的金
		int n = 0;
		pk::list<pk::building@> building_list = pk::get_building_list();
		for (int i = 0; i < building_list.count; i++)
		{
			pk::building@ building0 = building_list[i];
			int force0_id = building0.get_force_id();
			if (!pk::is_alive(building0) or force0_id != force.get_id()) continue;
			n += pk::get_gold(building0);
		}
		pk::list<pk::unit@> unit_list = pk::get_unit_list(force);
		for (int i = 0; i < unit_list.count; i++)
		{
			pk::unit@ unit0 = unit_list[i];
			if (!pk::is_alive(unit0)) continue;
			n += unit0.gold;
		}
		return n;
	}

	int get_force_food(pk::force@ force)
	{
		//分别获取城，港关，部队的粮
		int n = 0;
		pk::list<pk::building@> building_list = pk::get_building_list();
		for (int i = 0; i < building_list.count; i++)
		{
			pk::building@ building0 = building_list[i];
			int force0_id = building0.get_force_id();
			if (!pk::is_alive(building0) or force0_id != force.get_id()) continue;
			n += pk::get_food(building0);
		}
		pk::list<pk::unit@> unit_list = pk::get_unit_list(force);
		for (int i = 0; i < unit_list.count; i++)
		{
			pk::unit@ unit0 = unit_list[i];
			if (!pk::is_alive(unit0)) continue;
			n += unit0.food;
		}
		return n;
	}

	//只计算枪戟弩马
	int get_force_weapon(pk::force@ force)
	{
		//分别获取城，港关，部队的粮
		int n = 0;
		pk::list<pk::building@> building_list = pk::get_building_list();
		for (int i = 0; i < building_list.count; i++)
		{
			pk::building@ building0 = building_list[i];
			int force0_id = building0.get_force_id();
			if (!pk::is_alive(building0) or force0_id != force.get_id()) continue;
			n += pk::get_weapon_amount(building0, 1);
			n += pk::get_weapon_amount(building0, 2);
			n += pk::get_weapon_amount(building0, 3);
			n += pk::get_weapon_amount(building0, 4);
		}

		pk::list<pk::unit@> unit_list = pk::get_unit_list(force);
		for (int i = 0; i < unit_list.count; i++)
		{
			pk::unit@ unit0 = unit_list[i];
			if (!pk::is_alive(unit0)) continue;
			if (unit0.weapon == 兵器_枪 or unit0.weapon == 兵器_戟 or unit0.weapon == 兵器_弩 or unit0.weapon == 兵器_战马)
				n += unit0.troops;
		}
		return n;
	}

	//只计算任意
	int get_force_weapon(pk::force@ force, int weapon_id)
	{
		//分别获取城，港关，部队的兵装
		int n = 0;
		pk::list<pk::building@> building_list = pk::get_building_list();
		for (int i = 0; i < building_list.count; i++)
		{
			pk::building@ building0 = building_list[i];
			int force0_id = building0.get_force_id();
			if (!pk::is_alive(building0) or force0_id != force.get_id()) continue;
			n += pk::get_weapon_amount(building0, weapon_id);
		}

		pk::list<pk::unit@> unit_list = pk::get_unit_list(force);
		for (int i = 0; i < unit_list.count; i++)
		{
			pk::unit@ unit0 = unit_list[i];
			if (!pk::is_alive(unit0)) continue;
			if (unit0.weapon == weapon_id)
				n += unit0.troops;
		}
		return n;
	}

	int get_force_person_count(pk::force@ force)
	{
		//分别获取城，港关，部队的粮
		int n = 0;
		pk::list<pk::person@> person_list = pk::get_person_list(force, pk::mibun_flags(身份_一般, 身份_君主, 身份_太守, 身份_都督));
		n = person_list.count;
		return n;
	}

	/*仇视系统的尝试，因为受伤致死的存在，没办法进行了
	vold kill(person@ self, person@ by = null, hex_object@ where = null, person@ successor = null, int type = 0)
	{
		if (by !is null)
		{
			//区分父，母，配偶，义兄弟，子嗣，亲爱

			pk::add_dislike();
		}
		pk::kill(self, by, where, successor, type);
	}

	bool isFriend(pk::person@ person1, pk::person@ person2)
	{
		bool result = false;

		if (pk::is_gikyoudai(person1, person2.get_id()))
			result = true;
		if (pk::is_fuufu(person1, person2.get_id()))
			result = true;
		if (pk::is_ketsuen(person1, person2.get_id()))
			result = true;
		if (pk::is_oyako(person1, person2.get_id()))
			result = true;
		return result;
	}*/

	pk::list<pk::building@> get_base_list(pk::force@ force)
	{
		pk::list<pk::building@> list;
		for (int i = 0; i < 据点_末; i++)
		{
			pk::building@ src = pk::get_building(i);
			if (src.get_force_id() == force.get_id())
				list.add(src);
		}
		return list;
	}

	pk::list<pk::building@> get_base_list()
	{
		pk::list<pk::building@> list;
		for (int i = 0; i < 据点_末; i++)
		{
			pk::building@ src = pk::get_building(i);
			list.add(src);
		}
		return list;
	}

	pk::list<pk::building@> get_base_list(int province_id)
	{
		pk::list<pk::building@> list;
		for (int i = 0; i < 据点_末; i++)
		{
			pk::building@ src = pk::get_building(i);
			if (pk::get_province_id(src.pos) == province_id)
				list.add(src);
		}
		return list;
	}

	string get_weapon_name(int weapon_id)
	{
		string weapon_name;
		switch (weapon_id)
		{
<<<<<<< HEAD
		case 병기_검: weapon_name = "剑"; break;
		case 병기_창: weapon_name = "枪"; break;
		case 병기_극: weapon_name = "戟"; break;
		case 병기_노: weapon_name = "弩"; break;
		case 병기_군마: weapon_name = "战马"; break;
		case 병기_충차: weapon_name = "冲车"; break;
		case 병기_정란: weapon_name = "井阑"; break;
		case 병기_투석: weapon_name = "投石"; break;
		case 병기_목수: weapon_name = "木兽"; break;
=======
		case 兵器_剑: weapon_name = "剑"; break;
		case 兵器_枪: weapon_name = "枪"; break;
		case 兵器_戟: weapon_name = "戟"; break;
		case 兵器_弩: weapon_name = "弩"; break;
		case 兵器_战马: weapon_name = "战马"; break;
		case 兵器_冲车: weapon_name = "冲车"; break;
		case 兵器_井阑: weapon_name = "井阑"; break;
		case 兵器_投石: weapon_name = "投石"; break;
		case 兵器_木兽: weapon_name = "木兽"; break;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		case 兵器_走舸: weapon_name = "走舸"; break;
		case 兵器_楼船: weapon_name = "楼船"; break;
		case 兵器_斗舰: weapon_name = "斗舰"; break;
		default: weapon_name = "??"; break;
		}
		return weapon_name;
	}

<<<<<<< HEAD
=======
	string get_call(pk::person@ src,pk::person@dst)
	{
		pk::msg_param msg_param(46, src, dst);
		return pk::get_msg(msg_param);
	}
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	// 计谋执行部队和目标部队的台词输出:调用msg文件的台词(计谋相关内容- S11MSG01.s11文件)   계략 실행부대 및 대상부대의 대사 출력 : msg 폴더의 s11 파일 대사 소환 (계략관련 내용 - S11MSG01.s11 파일)
	void say_message(int msg_id, pk::person@ p0, pk::person@ p1, pk::unit@ u0, pk::unit@ p0_u)
	{
		pk::msg_param msg_param(msg_id, p0, p1);
		@msg_param.unit[0] = @u0;
		pk::say(pk::get_msg(msg_param), p0, p0_u);
	}

	void say_message(int msg_id, pk::person@ p0, pk::person@ p1, pk::building@ b0, pk::building@ p0_b)
	{
		pk::msg_param msg_param(msg_id, p0, p1);
		@msg_param.building[0] = @b0;
		pk::say(pk::get_msg(msg_param), p0, p0_b);
	}

<<<<<<< HEAD
=======
	void say_message(int msg_id, int num, pk::person@ p0, pk::person@ p1, pk::building@ b0, pk::building@ p0_b)
	{
		pk::msg_param msg_param(msg_id, p0, p1);
		@msg_param.building[0] = @b0;
		msg_param.num[0] = num;
		pk::say(pk::get_msg(msg_param), p0, p0_b);
	}

	void history_log(int msg_id, int num, pk::person@ p0, pk::person@ p1, pk::building@ b0, pk::building@ p0_b)
	{
		pk::msg_param msg_param(msg_id, p0, p1);
		@msg_param.building[0] = @b0;
		msg_param.num[0] = num;
		pk::force@ force = pk::get_force(p0.get_force_id());
		pk::history_log(p0.get_pos(), force.color, pk::get_msg(msg_param));
	}

	bool player_check(pk::person@ person)
	{
		if (pk::is_alive(person))
		{
			pk::force@ force = pk::get_force(person.get_force_id());
			if (pk::is_alive(force))
			{
				if (force.is_player())
				{
					pk::district@ district = pk::get_district(person.get_district_id());
					if (pk::is_alive(district))
					{
						if (pk::is_player_controlled(district)) return true;
					}
				}
			}
		}
		return false;
	}
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

	int get_attacking_near_enemy_base(pk::ai_context@ context, pk::building@ building)
	{
		int building_id = building.get_id();

		if (pk::is_valid_base_id(building_id))
		{
			if (pk::is_valid_city_id(building_id))
			{
				pk::city@city = pk::get_city(building_id);
				//确认邻近城市为目标城市，满足条件执行破坏指令
				for (int i = 0; i < context.target_count; ++i)
				{
					if (context.type[i] == 1)//类型1为攻击
					{
						int target = context.target[i];
						if (pk::is_neighbor_city(city, target)) return target;
					}
				}
			}
			else
			{
				//确认邻近城市为目标城市，满足条件执行破坏指令
				for (int i = 0; i < context.target_count; ++i)
				{
					if (context.type[i] == 1)//类型1为攻击
					{
						int target = context.target[i];
						if (pk::is_neighbor_base(building_id, target)) return target;
					}
				}
			}
		}
		return -1;
	}

	pk::list<pk::person@> get_member_list(pk::unit@ unit)
	{
		pk::list<pk::person@> list;
		for (int i = 0; i < 3; ++i)
		{
			if (pk::is_valid_person_id(unit.member[i]))
			{
				pk::person@ person0 = pk::get_person(unit.member[i]);
				if (pk::is_alive(person0)) list.add(person0);
			}
		}
		return list;
	}

	void 特殊地名争夺处理(int facility_id, int force_id, pk::point pos)
	{
		if (特定地点特定设施可争夺)
		{
			if (地名设施.find(facility_id) >= 0)
			{
				for (int i = 0; i < int(特殊地名设施.length); i++)
				{
					pk::point pos0;
					pos0.x = 特殊地名设施[i][1];
					pos0.y = 特殊地名设施[i][2];
					//确保不被改建后的设施左右
					facility_id = 特殊地名设施[i][0];
					if (pos0 == pos)
					{
						//清空封地信息
						specialinfo@ spec_t = @special_ex[i];
						if (pk::is_valid_person_id(spec_t.person)) person_ex[spec_t.person].spec_id = -1;//清空原来武将的对应记录
						spec_t.person = -1;
						spec_t.troops = -1;
						spec_t.gold = -1;
						spec_t.food = -1;
						if (force_id != -1)
						{
							pk::force@ force0 = pk::get_force(force_id);
							if (facility_id == 设施_连弩橹 or facility_id == 设施_箭楼)
								facility_id = pk::has_tech(force0, 技巧_施设强化) ? 设施_连弩橹 : 设施_箭楼;
							else if (facility_id == 设施_阵 or facility_id == 设施_砦 or facility_id == 设施_城塞)
								facility_id = pk::has_tech(force0, 技巧_设施强化) ? (pk::has_tech(force0, 技巧_城壁强化) ? 设施_城塞 : 设施_砦) : 设施_阵;
							//pk::trace(pk::format("最终生成设施类型，id为：{}", facility_id));
						}
						pk::building@ building0 = pk::create_spec(pk::get_facility(facility_id), pos, force_id, i);
						pk::complete(building0);
						break;
					}
				}
			}
		}
	}

	void 特殊地名争夺处理(pk::building@ target_building, pk::unit@ attacker, int& out destroyed_facility_id, int& out destroy_unit_id, int& out hp_damage, pk::point& out destroyed_building_pos, bool& out destroy_flag)
	{
		if (特定地点特定设施可争夺)
		{
			//destroyed_facility_id = -1;
			//destroy_unit_id = -1;
			//destroyed_building_pos = pk::point(-1,-1);
			//destroy_flag = false;
			//pk::trace(pk::format("hpdamage:{}",hp_damage));
			//pk::trace("全局pos1");
			if (hp_damage >= int(target_building.hp))
			{
				if (地名设施.find(target_building.facility) >= 0)
				{
					//pk::trace(pk::format("全局pos2,facility{}", target_building.facility));
					for (int i = 0; i < int(特殊地名设施.length); i++)
					{
						pk::point pos0;
						pos0.x = 特殊地名设施[i][1];
						pos0.y = 特殊地名设施[i][2];
						if (pos0 == target_building.pos)
						{
							//pk::trace(pk::format("破坏时 x:{},y:{}.", target_building.pos.x, target_building.pos.y));
							if (attacker.get_force_id() < 非贼势力_末)
							{
								destroyed_facility_id = target_building.facility;
								destroyed_building_pos = target_building.pos;
								destroy_unit_id = attacker.get_id();
								destroy_flag = true;
								break;
							}
							else
							{
								hp_damage = 0;
								pk::create_effect(91, target_building.pos);
								pk::set_district(target_building, pk::get_district(attacker.get_district_id()), 0);
								target_building.hp = pk::get_max_hp(target_building);
								break;
							}
						}
					}
				}
			}
			else hp_damage = hp_damage;
		}
	}

	void 特殊地名设施_重建(pk::unit@ unit, int& destroy_unit_id, int& destroyed_facility_id, pk::point& destroyed_building_pos, bool& destroy_flag)
	{
		if (destroy_unit_id != -1 and destroyed_building_pos.x != -1 and destroyed_facility_id != -1 and destroy_flag)
		{
			if (destroy_unit_id == unit.get_id() and 地名设施.find(destroyed_facility_id) >= 0)
			{
				pk::hex@ hex0 = pk::get_hex(destroyed_building_pos);
				if (hex0.has_unit)
				{
					pk::kill(pk::get_unit(destroyed_building_pos));
				}
				if (hex0.has_building)
				{
					pk::kill(pk::get_building(destroyed_building_pos));
				}
				//pk::trace(pk::format("攻击部队id:{}.被摧毁建筑id：{}",destroy_unit.get_id(), destroyed_building.get_id()));
				//pk::trace(pk::format("建筑id:{}.建筑位置：{},{},攻击部队势力：{}",destroyed_building.facility, destroyed_building.pos.x, destroyed_building.pos.y, destroy_unit.get_force_id()));
				//auto pos = destroyed_building.pos;
				//pos.x = pos.x + 1
				pk::unit@ destroy_unit = pk::get_unit(destroy_unit_id);
				pk::facility@ destroy_facility = pk::get_facility(destroyed_facility_id);
				int destroy_force = -1;
				if (destroy_unit.get_force_id() < 非贼势力_末) destroy_force = destroy_unit.get_force_id();
				pk::create_effect(91, destroyed_building_pos);
				//pk::trace(pk::format("攻击部队id:{}. 重建势力id:{}",destroy_unit.get_id(),destroy_force));
				//pk::trace(pk::format("x:{},y:{}.",destroyed_building_pos.x,destroyed_building_pos.y));
				//pk::trace(pk::format("设施类型：{}",destroy_facility.get_id()));
				//pk::trace(pk::format("重建势力id:{}",destroy_force));
				pk::building@ building0 = pk::create_building(destroy_facility, destroyed_building_pos, destroy_force);
				pk::complete(building0);
				//初始化
				destroy_unit_id = -1;
				destroyed_facility_id = -1;
				destroyed_building_pos.x = -1;
				destroyed_building_pos.y = -1;
				destroy_flag = false;
			}
		}
	}

	void 特殊地名设施_重建(pk::unit@ unit, int& destroy_unit_id, int& destroyed_facility_id, pk::point& destroyed_building_pos, bool& destroy_flag, int call_id)
	{
		if (destroy_unit_id != -1 and destroyed_building_pos.x != -1)
		{
			if (destroy_unit_id == unit.get_id() and destroy_flag)
			{
				if (地名设施.find(destroyed_facility_id) >= 0)
				{
					pk::hex@ hex0 = pk::get_hex(destroyed_building_pos);
					if (hex0.has_unit)
					{
						pk::kill(pk::get_unit(destroyed_building_pos));
					}
					if (hex0.has_building)
					{
						pk::kill(pk::get_building(destroyed_building_pos));
					}
					//pk::trace(pk::format("攻击部队id:{}.被摧毁建筑id：{}",destroy_unit.get_id(), destroyed_building.get_id()));
					//pk::trace(pk::format("建筑id:{}.建筑位置：{},{},攻击部队势力：{}",destroyed_building.facility, destroyed_building.pos.x, destroyed_building.pos.y, destroy_unit.get_force_id()));
					//auto pos = destroyed_building.pos;
					//pos.x = pos.x + 1
					pk::unit@ destroy_unit = pk::get_unit(destroy_unit_id);
					pk::facility@ destroy_facility = pk::get_facility(destroyed_facility_id);
					int destroy_force = -1;
					if (destroy_unit.get_force_id() < 非贼势力_末) destroy_force = destroy_unit.get_force_id();
					pk::create_effect(91, destroyed_building_pos);
					//pk::trace(pk::format("攻击部队id:{}. 重建势力id:{}",destroy_unit.get_id(),destroy_force));
					//pk::trace(pk::format("call_id:{}",call_id));
					//pk::trace(pk::format("x:{},y:{}.",destroyed_building_pos.x,destroyed_building_pos.y));
					//pk::trace(pk::format("设施类型：{}",destroy_facility.get_id()));
					//pk::trace(pk::format("重建势力id:{}",destroy_force));

					pk::building@ building0 = pk::create_building(destroy_facility, destroyed_building_pos, destroy_force);
					pk::complete(building0);
					//初始化
					destroy_unit_id = -1;
					destroyed_facility_id = -1;
					destroyed_building_pos.x = -1;
					destroyed_building_pos.y = -1;
					destroy_flag = false;
				}
			}

		}
	}

	int get_特殊地名_id(pk::building@building, bool strict = false)
	{
		if (地名设施.find(building.facility) < 0) return -1;

		//pk::trace(pk::format("全局pos2,facility{}", target_building.facility));
		for (int i = 0; i < int(特殊地名设施.length); i++)
		{
			pk::point pos0;
			pos0.x = 特殊地名设施[i][1];
			pos0.y = 特殊地名设施[i][2];
			if (pos0 == building.pos)
			{
				if (!strict) return i;
				if (特殊地名设施[i][0] == 设施_阵 and 地名设施类型1.find(building.facility) >= 0) return i;
				if (特殊地名设施[i][0] == 设施_箭楼 and 地名设施类型2.find(building.facility) >= 0) return i;
				return -1;
			}
		}
		return -1;
	}

	bool is_valid_spec_id(int spec_id)
	{
		if (spec_id >= 0 and spec_id < 地名_末)
			return true;
		return false;
	}

	int to_spec_id(int building_id)
	{
		if (building_id >= 100 and building_id < (100 + 地名_末))
			return (building_id - 100);
		return -1;
	}

	string get_spec_name(int spec_id)
	{
		return 特殊地名设施名称[spec_id];
	}

	bool reset_spec(int spec_id)
	{
		special_ex[spec_id].person = -1;
		special_ex[spec_id].gold = 0;
		special_ex[spec_id].food = 0;
		special_ex[spec_id].troops = 0;
		pk::building@ spec_building = pk::get_building(spec_id + 100);
		if (int(spec_building.hp) > pk::get_max_hp(spec_building)) spec_building.hp = pk::get_max_hp(spec_building);
		return true;
	}

	void set_person_weapon(int person_id, int weapon_id)
	{
		if (!pk::is_valid_person_id(person_id)) return;
		pk::set_person_weapon(person_id, weapon_id);
		person_ex[person_id].weapon_id = weapon_id;
	}
	
	void set_person_horse(int person_id, int horse_id)
	{
		if (!pk::is_valid_person_id(person_id)) return;
		pk::set_person_horse(person_id, horse_id);
		person_ex[person_id].horse_id = horse_id;
	}

<<<<<<< HEAD
=======
	void set_interior_land(int city_id)
	{
		int level = get_city_level(city_id);
		//内政用地升级[city_id][level - 1][];
		for (int i = 1; i <= level; ++i)
		{
			set_interior_land(city_id, i);
		}
	}

	void set_interior_land(int city_id, int level)
	{
		if (level > 0 and level <= 4)
		{
			//int population = base_ex[city_id].population;

			//内政用地升级[city_id][level - 1][];
			for (int j = 0; j < 2; ++j)
			{
				int index = 2 * j;
				pk::point pos1 = pk::point(内政用地升级[city_id][level - 1][index], 内政用地升级[city_id][level - 1][index + 1]);
				int result = pk::add_interior_land(pos1);
				//如果此地兵舍，厩舍，锻造厂，需刷新计数-----实际上是city_dev的building信息没更新，另外，如果城市人口减少导致内政用地减少，此信息目前只能在读档后更新
				if (false) pk::trace(pk::get_new_base_name(city_id) + "level:" + level + "x:" + pos1.x + "y:" + pos1.y + ",result" + result + "index" + index);
			}
		}

	}

	int get_city_level(int city_id)
	{
		int population = base_ex[city_id].population;
		int level = 0;
		if (population > 四级城市阈值) level = 4;
		else if (population > 三级城市阈值) level = 3;
		else if (population > 二级城市阈值) level = 2;
		else if (population > 一级城市阈值) level = 1;
		return level;
	}

	string get_level_string(int level)
	{
		string level_name;
		switch (level)
		{
		case 0: level_name = "微"; break;
		case 1: level_name = "小"; break;
		case 2: level_name = "中"; break;
		case 3: level_name = "大"; break;
		case 4: level_name = "巨"; break;
		}
		return level_name;
	}
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	/*
	const int 据点AI_开发 = 1;
const int 据点AI_征兵 = 2;
const int 据点AI_生产 = 3;
const int 据点AI_巡察 = 4;
const int 据点AI_交易 = 5; // ?
const int 据点AI_训练 = 6;
const int 据点AI_防御 = 7;
const int 据点AI_攻击 = 8;
const int 据点AI_设置 = 9;
const int 据点AI_关卡运输 = 10; // 都市关卡间运输도시 관문 간 수송?
const int 据点AI_都市运输 = 11; // 都市间运输도시 간 수송
const int 据点AI_移动 = 12;
const int 据点AI_召唤 = 13;
const int 据点AI_人才探索 = 14;
const int 据点AI_武将登用 = 15;
const int 据点AI_褒奖 = 16;
const int 据点AI_二虎竞食 = 17;
const int 据点AI_驱虎吞狼 = 18;
const int 据点AI_流言 = 19;
const int 据点AI_他势力武将登用 = 20;
const int 据点AI_拆除 = 21;
const int 据点AI_计略 = 22;
const int 据点AI_最小人才探索 = 23; // 武将 수가 难易度 값 不足时(预计失败时无条件不做). 인공지능.xml 참고 / 무장 수가 난이도 값 미만일 때만(실패 예상 시 무조건 안함). 인공지능.xml 참고
const int 据点AI_最小武将登用 = 24; // 무장 수가 난이도 값 미만일 때만(성공 예상 시 무조건 시도). 인공지능.xml 참고
const int 据点AI_最小他势力武将登用 = 25; // 무장 수가 3명 미만일 때만. 인공지능.xml 참고
const int 据点AI_交易2 = 26; // ?
const int 据点AI_吸收合并 = 27;
	*/

	string get_cmd_name(int cmd)
	{
		string cmd_name;
		switch (cmd)
		{
		case 1: cmd_name = "据点AI_开发 "; break;
		case 2: cmd_name = "据点AI_征兵 "; break;
		case 3: cmd_name = "据点AI_生产 "; break;
		case 4: cmd_name = "据点AI_巡察 "; break;
		case 5: cmd_name = "据点AI_交易 "; break;
		case 6: cmd_name = "据点AI_训练 "; break;
		case 7: cmd_name = "据点AI_防御 "; break;
		case 8: cmd_name = "据点AI_攻击 "; break;
		case 9: cmd_name = "据点AI_设置 "; break;
		case 10: cmd_name = "据点AI_关卡运输 "; break;
		case 11: cmd_name = "据点AI_都市运输 "; break;
		case 12: cmd_name = "据点AI_移动 "; break;
		case 13: cmd_name = "据点AI_召唤 "; break;
		case 14: cmd_name = "据点AI_人才探索 "; break;
		case 15: cmd_name = "据点AI_武将登用 "; break;
		case 16: cmd_name = "据点AI_褒奖 "; break;
		case 17: cmd_name = "据点AI_二虎竞食 "; break;
		case 18: cmd_name = "据点AI_驱虎吞狼 "; break;
		case 19: cmd_name = "据点AI_流言 "; break;
		case 20: cmd_name = "据点AI_他势力武将登用 "; break;
		case 21: cmd_name = "据点AI_拆除 "; break;
		case 22: cmd_name = "据点AI_计略 "; break;
		case 23: cmd_name = "据点AI_最小人才探索 "; break;
		case 24: cmd_name = "据点AI_最小武将登用 "; break;
		case 25: cmd_name = "据点AI_最小他势力武将登用 "; break;
		case 26: cmd_name = "据点AI_交易2 "; break;
		case 27: cmd_name = "据点AI_吸收合并 "; break;
		default: cmd_name = "??"; break;
		}
		return cmd_name;
	}
<<<<<<< HEAD
}
=======

	string get_stat_name(int stat_type)
	{
		switch (stat_type)
		{
		case 武将能力_统率:return "统率";
		case 武将能力_武力:return "武力";
		case 武将能力_智力:return "智力";
		case 武将能力_政治:return "政治";
		case 武将能力_魅力:return "魅力";
		}
		return "";
	}

	string get_tekisei_name(int tekisei_type)
	{
		switch (tekisei_type)
		{
		case 兵种_枪兵:return "枪兵";
		case 兵种_戟兵:return "戟兵";
		case 兵种_弩兵:return "弩兵";
		case 兵种_骑兵:return "骑兵";
		case 兵种_兵器:return "兵器";
		case 兵种_水军:return "水军";
		}
		return "";
	}

	const array<array<array<int>>> 内政用地升级 = {
{/*襄平*/{/*级别1*//*x*/179,/*y*/21,/*级别1*//*x*/180,/*y*/22},{/*级别2*//*x*/180,/*y*/21,/*级别2*//*x*/179,/*y*/20},{/*级别3*//*x*/178,/*y*/20,/*级别3*//*x*/177,/*y*/19},{/*级别4*//*x*/176,/*y*/20,/*级别4*//*x*/175,/*y*/20}},
{/*北平*/{/*级别1*//*x*/150,/*y*/30,/*级别1*//*x*/150,/*y*/27},{/*级别2*//*x*/149,/*y*/26,/*级别2*//*x*/148,/*y*/26},{/*级别3*//*x*/147,/*y*/26,/*级别3*//*x*/146,/*y*/27},{/*级别4*//*x*/150,/*y*/26,/*级别4*//*x*/149,/*y*/25}},
{/*蓟*/{/*级别1*//*x*/123,/*y*/24,/*级别1*//*x*/130,/*y*/6},{/*级别2*//*x*/130,/*y*/7,/*级别2*//*x*/129,/*y*/5},{/*级别3*//*x*/129,/*y*/6,/*级别3*//*x*/128,/*y*/5},{/*级别4*//*x*/128,/*y*/6,/*级别4*//*x*/127,/*y*/4}},
{/*南皮*/{/*级别1*//*x*/141,/*y*/31,/*级别1*//*x*/140,/*y*/33},{/*级别2*//*x*/129,/*y*/35,/*级别2*//*x*/130,/*y*/36},{/*级别3*//*x*/129,/*y*/36,/*级别3*//*x*/130,/*y*/37},{/*级别4*//*x*/129,/*y*/37,/*级别4*//*x*/130,/*y*/38}},
{/*平原*/{/*级别1*//*x*/123,/*y*/56,/*级别1*//*x*/124,/*y*/57},{/*级别2*//*x*/125,/*y*/57,/*级别2*//*x*/126,/*y*/58},{/*级别3*//*x*/123,/*y*/55,/*级别3*//*x*/124,/*y*/56},{/*级别4*//*x*/125,/*y*/56,/*级别4*//*x*/126,/*y*/57}},
{/*晋阳*/{/*级别1*//*x*/101,/*y*/41,/*级别1*//*x*/100,/*y*/41},{/*级别2*//*x*/99,/*y*/41,/*级别2*//*x*/98,/*y*/41},{/*级别3*//*x*/97,/*y*/40,/*级别3*//*x*/101,/*y*/40},{/*级别4*//*x*/100,/*y*/40,/*级别4*//*x*/99,/*y*/40}},
{/*邺*/{/*级别1*//*x*/114,/*y*/43,/*级别1*//*x*/114,/*y*/42},{/*级别2*//*x*/114,/*y*/41,/*级别2*//*x*/114,/*y*/40},{/*级别3*//*x*/114,/*y*/39,/*级别3*//*x*/115,/*y*/38},{/*级别4*//*x*/113,/*y*/43,/*级别4*//*x*/113,/*y*/42}},
{/*北海*/{/*级别1*//*x*/170,/*y*/47,/*级别1*//*x*/170,/*y*/48},{/*级别2*//*x*/170,/*y*/49,/*级别2*//*x*/170,/*y*/50},{/*级别3*//*x*/169,/*y*/46,/*级别3*//*x*/169,/*y*/47},{/*级别4*//*x*/169,/*y*/48,/*级别4*//*x*/169,/*y*/49}},
{/*广陵*/{/*级别1*//*x*/182,/*y*/96,/*级别1*//*x*/182,/*y*/95},{/*级别2*//*x*/182,/*y*/94,/*级别2*//*x*/182,/*y*/93},{/*级别3*//*x*/181,/*y*/96,/*级别3*//*x*/181,/*y*/95},{/*级别4*//*x*/181,/*y*/94,/*级别4*//*x*/181,/*y*/93}},
{/*下邳*/{/*级别1*//*x*/158,/*y*/64,/*级别1*//*x*/158,/*y*/67},{/*级别2*//*x*/157,/*y*/67,/*级别2*//*x*/159,/*y*/67},{/*级别3*//*x*/155,/*y*/65,/*级别3*//*x*/155,/*y*/66},{/*级别4*//*x*/155,/*y*/67,/*级别4*//*x*/156,/*y*/68}},
{/*寿春*/{/*级别1*//*x*/148,/*y*/92,/*级别1*//*x*/147,/*y*/91},{/*级别2*//*x*/147,/*y*/92,/*级别2*//*x*/147,/*y*/93},{/*级别3*//*x*/147,/*y*/94,/*级别3*//*x*/147,/*y*/95},{/*级别4*//*x*/147,/*y*/96,/*级别4*//*x*/147,/*y*/97}},
{/*濮阳*/{/*级别1*//*x*/142,/*y*/68,/*级别1*//*x*/143,/*y*/68},{/*级别2*//*x*/144,/*y*/68,/*级别2*//*x*/143,/*y*/67},{/*级别3*//*x*/141,/*y*/67,/*级别3*//*x*/142,/*y*/67},{/*级别4*//*x*/144,/*y*/67,/*级别4*//*x*/145,/*y*/67}},
{/*陈留*/{/*级别1*//*x*/113,/*y*/73,/*级别1*//*x*/113,/*y*/72},{/*级别2*//*x*/111,/*y*/73,/*级别2*//*x*/112,/*y*/73},{/*级别3*//*x*/111,/*y*/72,/*级别3*//*x*/110,/*y*/73},{/*级别4*//*x*/110,/*y*/74,/*级别4*//*x*/110,/*y*/75}},
{/*许昌*/{/*级别1*//*x*/94,/*y*/96,/*级别1*//*x*/95,/*y*/97},{/*级别2*//*x*/95,/*y*/96,/*级别2*//*x*/95,/*y*/95},{/*级别3*//*x*/94,/*y*/95,/*级别3*//*x*/93,/*y*/94},{/*级别4*//*x*/93,/*y*/93,/*级别4*//*x*/94,/*y*/94}},
{/*汝南*/{/*级别1*//*x*/113,/*y*/102,/*级别1*//*x*/115,/*y*/112},{/*级别2*//*x*/114,/*y*/113,/*级别2*//*x*/114,/*y*/112},{/*级别3*//*x*/113,/*y*/112,/*级别3*//*x*/113,/*y*/111},{/*级别4*//*x*/112,/*y*/112,/*级别4*//*x*/112,/*y*/111}},
{/*洛阳*/{/*级别1*//*x*/76,/*y*/79,/*级别1*//*x*/82,/*y*/79},{/*级别2*//*x*/75,/*y*/79,/*级别2*//*x*/75,/*y*/80},{/*级别3*//*x*/83,/*y*/79,/*级别3*//*x*/83,/*y*/80},{/*级别4*//*x*/74,/*y*/79,/*级别4*//*x*/83,/*y*/78}},
{/*宛*/{/*级别1*//*x*/87,/*y*/95,/*级别1*//*x*/87,/*y*/96},{/*级别2*//*x*/86,/*y*/97,/*级别2*//*x*/85,/*y*/97},{/*级别3*//*x*/86,/*y*/94,/*级别3*//*x*/87,/*y*/94},{/*级别4*//*x*/82,/*y*/98,/*级别4*//*x*/83,/*y*/98}},
{/*长安*/{/*级别1*//*x*/52,/*y*/81,/*级别1*//*x*/54,/*y*/81},{/*级别2*//*x*/53,/*y*/80,/*级别2*//*x*/55,/*y*/80},{/*级别3*//*x*/52,/*y*/80,/*级别3*//*x*/56,/*y*/80},{/*级别4*//*x*/53,/*y*/79,/*级别4*//*x*/55,/*y*/79}},
{/*上庸*/{/*级别1*//*x*/51,/*y*/96,/*级别1*//*x*/52,/*y*/97},{/*级别2*//*x*/53,/*y*/97,/*级别2*//*x*/54,/*y*/98},{/*级别3*//*x*/55,/*y*/98,/*级别3*//*x*/56,/*y*/98},{/*级别4*//*x*/53,/*y*/93,/*级别4*//*x*/54,/*y*/94}},
{/*安定*/{/*级别1*//*x*/30,/*y*/66,/*级别1*//*x*/31,/*y*/65},{/*级别2*//*x*/31,/*y*/64,/*级别2*//*x*/32,/*y*/65},{/*级别3*//*x*/31,/*y*/63,/*级别3*//*x*/32,/*y*/64},{/*级别4*//*x*/31,/*y*/62,/*级别4*//*x*/32,/*y*/63}},
{/*天水*/{/*级别1*//*x*/3,/*y*/67,/*级别1*//*x*/4,/*y*/69},{/*级别2*//*x*/4,/*y*/68,/*级别2*//*x*/4,/*y*/67},{/*级别3*//*x*/4,/*y*/66,/*级别3*//*x*/4,/*y*/64},{/*级别4*//*x*/4,/*y*/63,/*级别4*//*x*/4,/*y*/62}},
{/*武威*/{/*级别1*//*x*/5,/*y*/34,/*级别1*//*x*/5,/*y*/33},{/*级别2*//*x*/3,/*y*/28,/*级别2*//*x*/3,/*y*/29},{/*级别3*//*x*/4,/*y*/29,/*级别3*//*x*/4,/*y*/30},{/*级别4*//*x*/5,/*y*/29,/*级别4*//*x*/5,/*y*/30}},
{/*建业*/{/*级别1*//*x*/193,/*y*/120,/*级别1*//*x*/193,/*y*/126},{/*级别2*//*x*/194,/*y*/121,/*级别2*//*x*/194,/*y*/126},{/*级别3*//*x*/194,/*y*/120,/*级别3*//*x*/194,/*y*/127},{/*级别4*//*x*/193,/*y*/127,/*级别4*//*x*/194,/*y*/128}},
{/*吴*/{/*级别1*//*x*/153,/*y*/138,/*级别1*//*x*/153,/*y*/141},{/*级别2*//*x*/152,/*y*/142,/*级别2*//*x*/154,/*y*/142},{/*级别3*//*x*/151,/*y*/138,/*级别3*//*x*/152,/*y*/138},{/*级别4*//*x*/154,/*y*/138,/*级别4*//*x*/155,/*y*/138}},
{/*会稽*/{/*级别1*//*x*/144,/*y*/170,/*级别1*//*x*/145,/*y*/168},{/*级别2*//*x*/145,/*y*/169,/*级别2*//*x*/145,/*y*/170},{/*级别3*//*x*/142,/*y*/170,/*级别3*//*x*/143,/*y*/170},{/*级别4*//*x*/144,/*y*/171,/*级别4*//*x*/145,/*y*/171}},
{/*庐江*/{/*级别1*//*x*/140,/*y*/119,/*级别1*//*x*/141,/*y*/119},{/*级别2*//*x*/141,/*y*/120,/*级别2*//*x*/141,/*y*/121},{/*级别3*//*x*/147,/*y*/115,/*级别3*//*x*/147,/*y*/116},{/*级别4*//*x*/148,/*y*/115,/*级别4*//*x*/148,/*y*/116}},
{/*柴桑*/{/*级别1*//*x*/110,/*y*/140,/*级别1*//*x*/110,/*y*/141},{/*级别2*//*x*/110,/*y*/142,/*级别2*//*x*/111,/*y*/142},{/*级别3*//*x*/110,/*y*/143,/*级别3*//*x*/115,/*y*/138},{/*级别4*//*x*/115,/*y*/139,/*级别4*//*x*/115,/*y*/140}},
{/*江夏*/{/*级别1*//*x*/99,/*y*/119,/*级别1*//*x*/100,/*y*/120},{/*级别2*//*x*/101,/*y*/119,/*级别2*//*x*/102,/*y*/120},{/*级别3*//*x*/100,/*y*/116,/*级别3*//*x*/101,/*y*/116},{/*级别4*//*x*/102,/*y*/117,/*级别4*//*x*/103,/*y*/117}},
{/*河内*/{/*级别1*//*x*/73,/*y*/54,/*级别1*//*x*/72,/*y*/55},{/*级别2*//*x*/71,/*y*/54,/*级别2*//*x*/70,/*y*/53},{/*级别3*//*x*/70,/*y*/52,/*级别3*//*x*/70,/*y*/51},{/*级别4*//*x*/70,/*y*/50,/*级别4*//*x*/70,/*y*/49}},
{/*襄阳*/{/*级别1*//*x*/74,/*y*/125,/*级别1*//*x*/75,/*y*/124},{/*级别2*//*x*/75,/*y*/123,/*级别2*//*x*/75,/*y*/122},{/*级别3*//*x*/75,/*y*/121,/*级别3*//*x*/75,/*y*/120},{/*级别4*//*x*/72,/*y*/125,/*级别4*//*x*/73,/*y*/125}},
{/*江陵*/{/*级别1*//*x*/85,/*y*/142,/*级别1*//*x*/88,/*y*/141},{/*级别2*//*x*/84,/*y*/143,/*级别2*//*x*/89,/*y*/140},{/*级别3*//*x*/85,/*y*/141,/*级别3*//*x*/87,/*y*/140},{/*级别4*//*x*/84,/*y*/142,/*级别4*//*x*/88,/*y*/140}},
{/*长沙*/{/*级别1*//*x*/107,/*y*/153,/*级别1*//*x*/107,/*y*/154},{/*级别2*//*x*/107,/*y*/155,/*级别2*//*x*/107,/*y*/156},{/*级别3*//*x*/106,/*y*/153,/*级别3*//*x*/106,/*y*/154},{/*级别4*//*x*/107,/*y*/152,/*级别4*//*x*/108,/*y*/152}},
{/*武陵*/{/*级别1*//*x*/78,/*y*/149,/*级别1*//*x*/82,/*y*/149},{/*级别2*//*x*/79,/*y*/148,/*级别2*//*x*/81,/*y*/148},{/*级别3*//*x*/78,/*y*/151,/*级别3*//*x*/79,/*y*/151},{/*级别4*//*x*/80,/*y*/152,/*级别4*//*x*/79,/*y*/152}},
{/*桂阳*/{/*级别1*//*x*/93,/*y*/181,/*级别1*//*x*/102,/*y*/195},{/*级别2*//*x*/103,/*y*/194,/*级别2*//*x*/103,/*y*/193},{/*级别3*//*x*/102,/*y*/194,/*级别3*//*x*/101,/*y*/194},{/*级别4*//*x*/101,/*y*/193,/*级别4*//*x*/100,/*y*/194}},
{/*零陵*/{/*级别1*//*x*/72,/*y*/190,/*级别1*//*x*/73,/*y*/189},{/*级别2*//*x*/71,/*y*/189,/*级别2*//*x*/75,/*y*/189},{/*级别3*//*x*/70,/*y*/190,/*级别3*//*x*/76,/*y*/190},{/*级别4*//*x*/69,/*y*/190,/*级别4*//*x*/77,/*y*/190}},
{/*永安*/{/*级别1*//*x*/40,/*y*/128,/*级别1*//*x*/48,/*y*/133},{/*级别2*//*x*/47,/*y*/132,/*级别2*//*x*/49,/*y*/132},{/*级别3*//*x*/48,/*y*/132,/*级别3*//*x*/49,/*y*/131},{/*级别4*//*x*/50,/*y*/131,/*级别4*//*x*/50,/*y*/132}},
{/*汉中*/{/*级别1*//*x*/30,/*y*/93,/*级别1*//*x*/20,/*y*/93},{/*级别2*//*x*/19,/*y*/92,/*级别2*//*x*/21,/*y*/92},{/*级别3*//*x*/19,/*y*/91,/*级别3*//*x*/20,/*y*/92},{/*级别4*//*x*/20,/*y*/91,/*级别4*//*x*/21,/*y*/91}},
{/*梓潼*/{/*级别1*//*x*/12,/*y*/108,/*级别1*//*x*/13,/*y*/107},{/*级别2*//*x*/13,/*y*/106,/*级别2*//*x*/14,/*y*/107},{/*级别3*//*x*/13,/*y*/105,/*级别3*//*x*/14,/*y*/106},{/*级别4*//*x*/13,/*y*/104,/*级别4*//*x*/14,/*y*/105}},
{/*江州*/{/*级别1*//*x*/36,/*y*/146,/*级别1*//*x*/37,/*y*/151},{/*级别2*//*x*/36,/*y*/151,/*级别2*//*x*/35,/*y*/150},{/*级别3*//*x*/35,/*y*/149,/*级别3*//*x*/35,/*y*/148},{/*级别4*//*x*/35,/*y*/147,/*级别4*//*x*/35,/*y*/146}},
{/*成都*/{/*级别1*//*x*/20,/*y*/132,/*级别1*//*x*/19,/*y*/130},{/*级别2*//*x*/20,/*y*/131,/*级别2*//*x*/20,/*y*/130},{/*级别3*//*x*/17,/*y*/130,/*级别3*//*x*/18,/*y*/130},{/*级别4*//*x*/19,/*y*/129,/*级别4*//*x*/20,/*y*/129}},
{/*建宁*/{/*级别1*//*x*/35,/*y*/171,/*级别1*//*x*/15,/*y*/179},{/*级别2*//*x*/16,/*y*/180,/*级别2*//*x*/15,/*y*/178},{/*级别3*//*x*/16,/*y*/179,/*级别3*//*x*/17,/*y*/179},{/*级别4*//*x*/16,/*y*/178,/*级别4*//*x*/17,/*y*/178}},
{/*云南*/{/*级别1*//*x*/15,/*y*/194,/*级别1*//*x*/14,/*y*/194},{/*级别2*//*x*/14,/*y*/195,/*级别2*//*x*/13,/*y*/196},{/*级别3*//*x*/13,/*y*/195,/*级别3*//*x*/13,/*y*/194},{/*级别4*//*x*/12,/*y*/196,/*级别4*//*x*/12,/*y*/195}}

	};
}//namespace end
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321


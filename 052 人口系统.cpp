// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/02/12 # 江东新风 # 丰收不起效bug，伤兵恢复下限 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/10 # 江东新风 # 人口设定扩展到非城市据点，蝗灾，疫病，丰收，掌握人心的影响 ##
// ## 2021/10/07 # 江东新风 # 基础人口，人口增长，总兵役人口，可用兵役等设定 ##
// ## 2021/01/19 # 江东新风 # 构思 ##
namespace 人口系统
{
	/// ============================================ < 人口自定义项-放到301 > ============================================ ///
	

	/// ========================================================================================================= ///
	const array<int> 各城市人口 = { 91650, 56550, 334100, 455000, 559000, 383500, 1671150, 364000, 461500, 62400, 282100, 346450, 656500, 385450, 303550, 1741350, 289250, 445250, 250900, 52650, 162500, 199550, 487500, 461500, 479700, 117000, 430300, 253500, 123500, 147550, 500500, 490750, 110500, 118950, 296400, 130000, 136500, 279500, 78000, 975000, 247000, 305500};
	const bool 调试模式 = false;
	class Main
	{
		Main()
		{
			pk::bind(102, pk::trigger102_t(onGameStart));
			pk::bind(107, pk::trigger107_t(onNewDay));//测试时107，实际运行改109
			pk::bind(107, pk::trigger107_t(onNewDay2));//作为人口削减的地方，每旬如城市周边3格有敌部队，人口减2%
			//pk::bind(108, pk::trigger108_t(onNewMonth));
			pk::bind(120, pk::trigger120_t(func_信息显示_人口信息), 999);//数字越大越优先
			pk::bind(171, pk::trigger171_t(onUnitRemove));
			pk::set_func(64, pk::func64_t(callback));

		}

		int callback(pk::city@ city)
		{
			int city_id = city.get_id();
			int population = base_ex[city_id].population;
			//pk::trace("城市" + pk::get_new_base_name(city_id) + "人口" + population);
			if (population > 600000) return 2;
			else if (population > 200000) return 1;
			else return 0;
		}

		//开局时设定基础人口数据
		void onGameStart()
		{
			if (pk::get_scenario().loaded or !开启人口系统) return;
			for (int city_id = 0; city_id < 城市_末; ++city_id)
			{
				BaseInfo@ city_t = @base_ex[city_id];
				city_t.population = 三顾城市人口[city_id][0];
				city_t.mil_pop_all = 三顾城市人口[city_id][1];
				city_t.mil_pop_av = 三顾城市人口[city_id][2];
				//if (city_id == 城市_襄阳) pk::trace(pk::format("onGameStart,襄阳人口：{}", city_t.population));
			}
			for (int base_id = 城市_末; base_id < 据点_末; ++base_id)
			{
				BaseInfo@ base_t = @base_ex[base_id];
				base_t.population = 15000;
				base_t.mil_pop_all = 5000;
				base_t.mil_pop_av = 1000;
				
			}
		} //onGameStart()

		void onNewDay()
		{
			if (!开启人口系统) return;
			//人口增长
			for (int base_id = 0; base_id < 据点_末; ++base_id)
			{
				BaseInfo@ base_t = @base_ex[base_id];
				//if (base_id == 城市_襄阳) pk::trace(pk::format("onNewDay 1,襄阳人口：{}", base_t.population));
				int public_order = 0;
				if (base_id < 城市_末) public_order = pk::get_city(base_id).public_order;
				else public_order = base_t.public_order;
				float porder_effect = (public_order - 70) / 30;

				float population_max = base_id < 城市_末? (pk::is_large_city(pk::get_city(base_id)) ? 800000.f : 400000.f):200000.f;
				float population_t = base_t.population;
				//pk::trace(pk::format("population_max:{},population_t:{}", population_max, population_t));
				float final_rate = 0;
				float buf = 0;

				//掌握人心科技
				if (pk::get_building(base_id).has_tech(技巧_掌握人心)) buf += 人口基础增长率 * 0.2f;
				//所以人口增长受到治安及人口上限影响
				if ((population_t /population_max) < 0.25f) final_rate = (人口基础增长率 + 0.02f + buf) * porder_effect;
				else if ((population_t /population_max) < 0.5f) final_rate = (人口基础增长率 + buf) * porder_effect;
				else if ((population_t /population_max) < 0.7f) final_rate = ((人口基础增长率 + buf )/1.5f) * porder_effect;
				else if ((population_t /population_max) < 0.9f) final_rate = ((人口基础增长率 + buf) / 3.f) * porder_effect;
				else if ((population_t /population_max) < 1.f) final_rate = ((人口基础增长率 + buf) / 10.f) * porder_effect;
				else final_rate = (人口基础增长率 / 20.f) * public_order / 100;

				//城市专属
				if (base_id < 城市_末)
				{
					pk::city@ city0 = pk::get_city(base_id);
					bool 丰收状态 = city0.housaku;
					bool 疫病状态 = city0.ekibyou;
					bool 蝗灾状态 = city0.inago;
					if (city0.housaku) final_rate += 人口基础增长率;//丰收状态基础增长率翻倍
					else if (city0.ekibyou) final_rate -= 人口基础增长率 * 2.0f;//按每旬算
					else if (city0.inago) final_rate -= 人口基础增长率 * 1.5f;//按每旬算
				}

				int temp = int(base_t.population * (1 + final_rate));
				int temp2 = int((temp - base_t.population) * 0.2f + population_t/600.f);//总兵役人口算增量,额外加上每600人多一个兵役人口
				//if (pk::is_first_month_of_quarter()) pk::trace(pk::format("{},temp:{},人口：{},增长率：{},base_t.population / population_max:{}", pk::decode(pk::get_name(city0)), temp, base_t.population,final_rate, population_t /population_max));
				//base_t.population = uint32(temp);
				ch::add_population(base_id, int(base_t.population* final_rate));
				ch::add_mil_pop_all(base_id, temp2);
				if (调试模式 and base_id == 城市_永安) pk::trace(pk::format("onNewDay 2,永安人口：{},总兵役人口：{}", base_t.population, base_t.mil_pop_all));
				
				//if (base_id == 城市_襄阳) pk::trace(pk::format("onNewDay 3,襄阳人口：{}", base_t.population));
				//if (pk::is_first_month_of_quarter() and base_id < 城市_末) pk::trace(pk::format("{},人口：{}，金收入：{},粮收入：{}",pk::decode(pk::get_name(pk::get_city(base_id))), base_t.population, get_city_revenue(base_id), get_city_harvest(base_id)));
			}

			//伤兵恢复
			for (int base_id = 0; base_id < 据点_末; ++base_id)
			{
				BaseInfo@ base_t = @base_ex[base_id];
				if (base_t.wounded > 0)
				{
					pk::building@ building0 = pk::get_building(base_id);
					int heal_num = int(base_t.wounded * 0.2f);					
					if (ch::has_skill(building0,特技_医者)) heal_num= int(base_t.wounded * 0.3f);
					if (调试模式) pk::trace(pk::format("伤兵恢复，wounded:{},heal_num：{}", base_t.wounded, heal_num));
					heal_num = pk::min(base_t.wounded, pk::max(30, heal_num));//设定伤兵最低恢复数
					base_t.wounded -= uint32(heal_num);
					ch::add_troops(building0, heal_num, true);
					
				}
			}

			for (int province_id = 0; province_id < 州_末; ++province_id)
			{
				//人口平均分配到城市，或据点？
				pk::list<pk::building@> province_base = ch::get_base_list(province_id);
				pk::list<pk::city@> province_city = pk::get_city_list(pk::get_province(province_id));
				float count = province_city.count + (province_base.count - province_city.count) * 0.5f;
				settinginfo@ set_t = @setting_ex;
				float per_deserter = set_t.province_deserter[province_id] * 0.5f / count;//每旬分配总量的50%
				for (int i = 0; i < province_base.count; ++i)
				{
					int base_id = province_base[i].get_id();
					if (base_id < 城市_末)
					{
						set_t.province_deserter[province_id] -= uint32(per_deserter);
						BaseInfo@ base_t = @base_ex[base_id];
						ch::add_population(base_id, uint32(per_deserter * 0.4f));
						//base_t.population += uint32(per_deserter*0.4f);
						ch::add_mil_pop_all(base_id, uint32(per_deserter * 0.6f));
						if (调试模式 and base_id == 城市_永安) pk::trace(pk::format("deserter,永安人口：{},总兵役人口：{}", base_t.population, base_t.mil_pop_all));
					}
					else
					{
						set_t.province_deserter[province_id] -= uint32(0.5f*per_deserter);
						BaseInfo@ base_t = @base_ex[base_id];
						ch::add_population(base_id, uint32(0.5f * per_deserter * 0.4f));
						ch::add_mil_pop_all(base_id, uint32(0.5f * per_deserter * 0.6f));	
						if (调试模式 and base_id == 城市_永安) pk::trace(pk::format("deserter 2,永安人口：{},总兵役人口：{}", base_t.population, base_t.mil_pop_all));
					}
				}

			}
		}

		void onNewDay2()
		{
			if (!开启人口系统) return;
			for (int base_id = 0; base_id < 据点_末; ++base_id)
			{
				BaseInfo@ base_t = @base_ex[base_id];
				if (base_t.return_timer >= 0 and base_t.return_timer != 255)
				{
					if (!base_t.return_buf)
					{
						if (base_id < 据点_城市末 and pk::get_city(base_id).num_devs > 9 and pk::get_city(base_id).public_order > 89)
						{
							base_t.return_timer = 5;//此时作为buf计数
							base_t.return_buf = true;
						}
						if (base_id >= 据点_城市末 and base_id < 据点_末)
						{
							if (base_t.public_order > 89)
							{
								base_t.return_timer = 5;//此时作为buf计数
								base_t.return_buf = true;
							}
							//小城以什么判断呢？也引入治安概念？
						}
					}

					// + (city_t.return_buf?0.08f:0.f)
					if (base_t.return_buf)
					{
						//if (base_id == 城市_襄阳) pk::trace(pk::format("onNewDay2 return_buf,襄阳人口：{}", base_t.population));
						int temp = int(base_t.population + (base_t.return_pop * 0.3f));//每回合返回总返回人口的30%
						int temp2 = int((temp - base_t.population) * 0.3f);//总兵役人口算增量
						base_t.return_pop -= uint32(base_t.return_pop * 0.3f);
						//base_t.population = uint32(temp);
						ch::add_population(base_id, uint32(base_t.return_pop * 0.3f));
						ch::add_mil_pop_all(base_id, temp2);
						//base_t.mil_pop_all += uint32(temp2);
					}
					base_t.return_timer -= 1;
					
				}
			}
			for (int base_id = 0; base_id < 据点_末; ++base_id)
			{
				//每旬总兵役向可用兵役转化，基础值在1200?受到兵舍等级(0.2,0.4,0.7)，太守魅力(0.3)，都市名声影响(0.5)？
				pk::building@ building0 = pk::get_building(base_id);
				int taishu_id = pk::get_taishu_id(building0);
				if (taishu_id != -1)
				{
					pk::person@ taishu = pk::get_person(taishu_id);
					float inf_a = 1.0f;
					if (taishu.stat[武将能力_魅力] < 35) inf_a -= 0.3f;
					else if (taishu.stat[武将能力_魅力] < 45) inf_a -= 0.2f;
					else if (taishu.stat[武将能力_魅力] < 55) inf_a -= 0.1f;
					else if (taishu.stat[武将能力_魅力] < 85) inf_a = inf_a;
					else if (taishu.stat[武将能力_魅力] < 95) inf_a += 0.1f;
					else if (taishu.stat[武将能力_魅力] < 105) inf_a += 0.2f;
					else  inf_a += 0.3f;

					if (ch::has_skill(building0,特技_名声,true)) inf_a += 0.5f;

					if (base_id < 城市_末)
					{
						pk::city@ city0 = pk::get_city(base_id);
						float level = func_5c4600(city0);
						if (level == 1.5f) inf_a += 0.7f;
						else if (level == 1.2f)  inf_a += 0.4f;
						else if (level == 1.0f)  inf_a += 0.2f;
						else inf_a = inf_a;
					
					}
					else
					{
						inf_a += 0.2f;//小城默认兵舍为1级
					}

					int exp_mil = int(800.f * inf_a);//总兵役向可用兵役转化公式
					//pk::trace(pk::format("{}:inf_a:{},exp_mil：{}", pk::decode(pk::get_name(building0)), inf_a, exp_mil));
					BaseInfo@ base_t = @base_ex[base_id];
					if (调试模式) pk::trace(pk::format("before base_t.mil_pop_all:{},base_t.mil_pop_av：{}", base_t.mil_pop_all, base_t.mil_pop_av));
					if (exp_mil >= int(base_t.mil_pop_all))
					{
						exp_mil = base_t.mil_pop_all;
						base_t.mil_pop_all = 0;
						ch::add_mil_pop_av(base_id, exp_mil); //base_t.mil_pop_av += exp_mil;
					}
					else
					{
						ch::add_mil_pop_all(base_id, -exp_mil); //base_t.mil_pop_all -= exp_mil;
						ch::add_mil_pop_av(base_id, exp_mil); //base_t.mil_pop_av += exp_mil;
					}
					if (调试模式) pk::trace(pk::format("after base_t.mil_pop_all:{},base_t.mil_pop_av：{}", base_t.mil_pop_all, base_t.mil_pop_av));
					

					//境内敌军影响人口及总兵役
					int has_enemy_type = has_enemy(pk::get_building(base_id));
					float enemy_inf = has_enemy_type == 2 ? -0.03f : (has_enemy_type == 1 ? -0.02f : 0.f);
					if (has_enemy_type != 0)
					{
						//BaseInfo@ base_t = @base_ex[base_id];
						int temp = int(base_t.population *  enemy_inf);//境内敌军对人口的影响
						int temp2 = int(temp * 0.15f);
						ch::add_population(base_id,temp);
						ch::add_mil_pop_all(base_id, temp2);;//境内敌军也会小幅度影响总兵役人口
						//pk::trace(pk::format("enemy_inf:{},影响后人口：{},影响总兵役量：{}", enemy_inf, temp, temp2));				
					}				
					//if (base_id == 城市_襄阳) pk::trace(pk::format("onNewDay2 enemy_inf,襄阳人口：{}", base_t.population));
				}


			}
		}

		void func_信息显示_人口信息()
		{
			// 光标指的坐标
			if (!开启人口系统) return;
			pk::point cursor_pos = pk::get_cursor_hex_pos();
			if (!pk::is_valid_pos(cursor_pos)) return;

			// 光标上指示的建筑物
			pk::building@ building = pk::get_building(cursor_pos);
			if (building is null || building.facility > 2) return;  //城港官才显示

			//if (!building.is_player()) return;
			//if (!pk::is_player_controlled(building)) return;
			//if (building.get_force_id() != pk::get_current_turn_force_id()) return;


			string building_name = pk::decode(pk::get_name(building));

			string title = pk::format("据点信息(\x1b[1x{}\x1b[0x)", building_name);

			int middle = int(pk::get_resolution().width) / 2;
			int left = middle - 200;
			int right = middle + 200;
			int top = 5 + (pk::get_player_count() == 0 ? 100:0);
			int bottom = top + 80;
			int base_id = building.get_id();
			// pk::draw_rect(pk::rectangle(left, top, right, bottom), 0xff00ccff);
			BaseInfo@ base_t = @base_ex[base_id];

			//if (base_id == 城市_襄阳) pk::trace(pk::format("func_信息显示_人口信息 0,襄阳人口：{}", base_t.population));
			if (base_t.return_timer > 0 and base_t.return_timer != 255)
			{
				//pk::trace(pk::format("return time:{}", base_t.return_timer));
				title = title + "(\x1b[2x新攻占\x1b[0x)";
				if (调试模式) title += base_t.return_pop;
			}

			if (false)
			{
				//pk::trace(pk::format("return time:{}", base_t.return_timer));
				settinginfo@ set_t = @setting_ex;

				title = title + "州逃兵:" +"\x1b[2x"+ set_t.province_deserter[pk::get_province_id(building.pos)] + "\x1b[0x";
			}

			pk::draw_text(pk::encode(title), pk::point(left + 5, top + 5), 0xffffffff, FONT_BIG, 0xff000000);

			if (false and base_id >= 据点_城市末 and base_id < 据点_末)
			{
				//BaseInfo@ base_t = @base_ex[base_id];
				string info_治安 = pk::format("治安: \x1b[1x{}\x1b[0x", base_t.public_order);
				pk::draw_text(pk::encode(info_治安), pk::point(left + 10, top + 40 + 军餉维护费::信息行数 * 20), 0xffffffff, FONT_SMALL, 0xff000000);
				军餉维护费::信息行数 += 1;
			}

			if (base_id < 据点_末)
			{
				//if (base_id == 城市_襄阳) pk::trace(pk::format("func_信息显示_人口信息,襄阳人口：{}", base_t.population));
				//BaseInfo@ base_t = @base_ex[base_id];
				string info_人口 = pk::format("人口: \x1b[1x{}\x1b[0x", base_t.population);
				string info_可用兵役 = pk::format("可用兵役: \x1b[1x{}\x1b[0x", base_t.mil_pop_av);
				string info_总兵役 = pk::format("总兵役: \x1b[1x{}\x1b[0x", base_t.mil_pop_all);
				string info_伤兵 = pk::format("伤兵:\x1b[16x{}\x1b[0x", base_t.wounded);
				pk::draw_text(pk::encode(info_人口), pk::point(left + 10, top + 40 + 军餉维护费::信息行数 * 20), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::draw_text(pk::encode(info_总兵役), pk::point(middle + 10, top + 40 + 军餉维护费::信息行数 * 20), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::draw_text(pk::encode(info_可用兵役), pk::point(left + 10, top + 40 + (军餉维护费::信息行数 + 1) * 20), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::draw_text(pk::encode(info_伤兵), pk::point(middle + 10, top + 40 + (军餉维护费::信息行数 + 1) * 20), 0xffffffff, FONT_SMALL, 0xff000000);
				军餉维护费::信息行数 += 2;
			}

		}

		void onUnitRemove(pk::unit@ unit,pk::hex_object@ dst, int type)
		{
			//pk::trace(pk::format("onUnitRemove0", 1));
			if (dst is null) return;
			if (type == 1)
			{
				pk::building@ dst_building = dst.get_type_id() == pk::building::type_id ? dst : null;//将hex_object转为建筑指针
				//pk::trace(pk::format("onUnitRemove0,id:{}", dst_building.get_id()));
				if (!pk::is_alive(dst_building)) return;
				//pk::trace(pk::format("onUnitRemove1", 1));
				int base_id = dst_building.get_id();
				int unit_id = unit.get_id();


				BaseInfo@ base_t = @base_ex[base_id];
				unitinfo@ unit_t = @unit_ex[unit_id];
				//if (base_id == 城市_襄阳) pk::trace(pk::format("onUnitRemove,襄阳伤兵：{}", base_t.wounded));
				base_t.wounded += pk::max(0, unit_t.wounded);
				unit_t.wounded = 0;
				
				
			}
			if (type == 0)
			{
				//pk::trace("remove type 0");
				pk::building@ dst_building = dst.get_type_id() == pk::building::type_id ? dst : null;
				pk::unit@ dst_unit = dst.get_type_id() == pk::unit::type_id ? dst : null;
				//分为部队击破部队，据点击破部队，其他建筑击破部队，同时需区分是否友军,（区分是否异族势力）
				if (dst_unit !is null)
				{
					//unit 是被灭方
					//pk::trace(pk::format("{}击破{}",pk::decode(pk::get_name(dst_unit)), pk::decode(pk::get_name(unit))));
					unitinfo@ unit_t = @unit_ex[unit.get_id()];
					int unit_wounded = unit_t.wounded;
					unit_t.wounded = 0;
					
					//pk::trace(pk::format("{}击破{}, 伤兵：{}", pk::decode(pk::get_name(dst_unit)), pk::decode(pk::get_name(unit)), unit_wounded));
					if (pk::is_enemy(unit, dst_unit))
					{

						unitinfo dst_t(dst_unit.get_id());
						dst_t.wounded += uint16(0.5f * unit_wounded);
						dst_t.update(dst_unit.get_id());
						int unit_service = pk::get_service(unit);
						if (unit_service > -1 and unit_service<据点_末)
						{
							BaseInfo@ base_t = @base_ex[unit_service];
							base_t.wounded += uint16(0.5f * unit_wounded);
							
						}
					}
					else
					{
						int unit_service = pk::get_service(unit);
						if (unit_service != -1)
						{
							BaseInfo@ base_t = @base_ex[unit_service];
							base_t.wounded += uint16(unit_wounded);
							
						}
					}
				}
				if (dst_building !is null)
				{
					//unit 是被灭方
					unitinfo@ unit_t = @unit_ex[unit.get_id()];
					int unit_wounded = unit_t.wounded;
					unit_t.wounded = 0;
					
					if (pk::is_enemy(unit, dst_building))
					{
						int dst_building_id = dst_building.get_id();
						if (pk::is_valid_base_id(dst_building_id))
						{
							BaseInfo@ dst_t = @base_ex[dst_building.get_id()];
							dst_t.wounded += uint16(0.5f * unit_wounded);
						}
						int unit_service = pk::get_service(unit);
						if (pk::is_valid_base_id(unit_service))
						{
							BaseInfo@ base_t = @base_ex[unit_service];
							base_t.wounded += uint16(0.5f * unit_wounded);
							
						}
					}
					else
					{
						int unit_service = pk::get_service(unit);
						if (pk::is_valid_base_id(unit_service))
						{
							BaseInfo@ base_t = @base_ex[unit_service];
							base_t.wounded += uint16(unit_wounded);
							
						}
					}
				}
			}

		}

		int has_enemy(pk::building@ base)
		{
			// 1格内敌部队数
			bool has_enemy_units1 = false;
			// 3格内敌部队数
			bool has_enemy_units3 = false;


			auto range = pk::range(base.get_pos(), 1, 4 + (base.facility == 设施_都市 ? 1 : 0));
			for (int i = 0; i < int(range.length); i++)
			{
				auto unit = pk::get_unit(range[i]);
				if (pk::is_alive(unit))
				{
					int distance = pk::get_distance(base.get_pos(), range[i]);
					if (pk::is_enemy(base, unit))
					{
						if (distance <= 1)
						{
							has_enemy_units1 = true;
							break;
						}
						if (distance <= 3)
						{
							has_enemy_units3 = true;
						}
					}
					else
					{
					}
				}
			}

			if (has_enemy_units1) return 2;
			if (has_enemy_units3) return 1;
			return 0;
		}

		//可以通过返回值判断是几级兵舍
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
			if (level1 > 0) return 1.f;
			return 0.f;
		}

		int get_city_revenue(int city_id)
		{
			return cast<pk::func150_t>(pk::get_func(150))(pk::get_city(city_id));
		}

		int get_city_harvest(int city_id)
		{
			return cast<pk::func151_t>(pk::get_func(151))(pk::get_city(city_id));
		}
	} // class Main

	Main main;
	const array<array<int>> 三顾城市人口 = {
		/*襄平*/{/*人口*/78490,/*可用兵役*/23547,/*兵役*/5000},
		/*北平*/{/*人口*/85220,/*可用兵役*/25566,/*兵役*/6000},
		/*蓟*/{/*人口*/213510,/*可用兵役*/64053,/*兵役*/11000},
		/*南皮*/{/*人口*/216440,/*可用兵役*/64932,/*兵役*/13000},
		/*平原*/{/*人口*/98390,/*可用兵役*/29517,/*兵役*/11000},
		/*晋阳*/{/*人口*/135290,/*可用兵役*/40587,/*兵役*/7000},
		/*邺*/{/*人口*/261660,/*可用兵役*/78498,/*兵役*/14000},
		/*北海*/{/*人口*/189410,/*可用兵役*/56823,/*兵役*/12000},
		/*下邳*/{/*人口*/131100,/*可用兵役*/39330,/*兵役*/16000},
		/*小沛*/{/*人口*/77800,/*可用兵役*/23340,/*兵役*/10000},
		/*寿春*/{/*人口*/216130,/*可用兵役*/64839,/*兵役*/12000},
		/*濮阳*/{/*人口*/157810,/*可用兵役*/47343,/*兵役*/16000},
		/*陈留*/{/*人口*/217300,/*可用兵役*/65190,/*兵役*/14000},
		/*许昌*/{/*人口*/308820,/*可用兵役*/92646,/*兵役*/15000},
		/*汝南*/{/*人口*/161300,/*可用兵役*/48390,/*兵役*/13000},
		/*洛阳*/{/*人口*/219530,/*可用兵役*/65859,/*兵役*/16000},
		/*宛*/{/*人口*/169900,/*可用兵役*/50970,/*兵役*/12000},
		/*长安*/{/*人口*/260590,/*可用兵役*/78177,/*兵役*/13000},
		/*上庸*/{/*人口*/114800,/*可用兵役*/34440,/*兵役*/8000},
		/*安定*/{/*人口*/86790,/*可用兵役*/26037,/*兵役*/5000},
		/*天水*/{/*人口*/103900,/*可用兵役*/31170,/*兵役*/5000},
		/*武威*/{/*人口*/137100,/*可用兵役*/41130,/*兵役*/5000},
		/*建业*/{/*人口*/269590,/*可用兵役*/80877,/*兵役*/9000},
		/*吴*/{/*人口*/182200,/*可用兵役*/54660,/*兵役*/6000},
		/*会稽*/{/*人口*/189000,/*可用兵役*/56700,/*兵役*/9000},
		/*庐江*/{/*人口*/91930,/*可用兵役*/27579,/*兵役*/12000},
		/*柴桑*/{/*人口*/132710,/*可用兵役*/39813,/*兵役*/6000},
		/*江夏*/{/*人口*/130300,/*可用兵役*/39090,/*兵役*/5000},
		/*新野*/{/*人口*/151000,/*可用兵役*/45300,/*兵役*/6000},
		/*襄阳*/{/*人口*/264000,/*可用兵役*/79200,/*兵役*/12000},
		/*江陵*/{/*人口*/279800,/*可用兵役*/83940,/*兵役*/8000},
		/*长沙*/{/*人口*/206260,/*可用兵役*/61878,/*兵役*/8000},
		/*武陵*/{/*人口*/155000,/*可用兵役*/46500,/*兵役*/6000},
		/*桂阳*/{/*人口*/113500,/*可用兵役*/34050,/*兵役*/4000},
		/*零陵*/{/*人口*/183880,/*可用兵役*/55164,/*兵役*/5000},
		/*永安*/{/*人口*/77190,/*可用兵役*/23157,/*兵役*/10000},
		/*汉中*/{/*人口*/190480,/*可用兵役*/57144,/*兵役*/13000},
		/*梓潼*/{/*人口*/96580,/*可用兵役*/28974,/*兵役*/5000},
		/*江州*/{/*人口*/115390,/*可用兵役*/34617,/*兵役*/11000},
		/*成都*/{/*人口*/303100,/*可用兵役*/90930,/*兵役*/14000},
		/*建宁*/{/*人口*/174110,/*可用兵役*/52233,/*兵役*/11000},
		/*云南*/{/*人口*/93200,/*可用兵役*/27960,/*兵役*/9000}
	};
}

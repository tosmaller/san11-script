// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/29 # 江东新风 # 为兵力增减伤兵系统新增 ##
namespace unit_add_troops_cal
{
	const bool 调试模式 = false;
	class Main
	{
		Main()
		{
			pk::set_func(58, pk::func58_t(callback0));
			pk::set_func(59, pk::func59_t(callback1));
			pk::set_func(60, pk::func60_t(callback2));
		}

		int callback0(pk::unit@ src,int value, int rettype)
		{
			if (!pk::is_alive(src)) return 0;
			int unit_troops = src.troops;
			int max_troops = pk::get_max_troops(src);
			int cur_troops = 0;
			int change_value = 0;
			if (unit_troops + value > 0)
			{
				cur_troops = max_troops;
				if (max_troops > unit_troops + value)
					cur_troops = unit_troops + value;
			}
			else
			{
				cur_troops = 0;
			}
			if (pk::is_alive(src))
			{
				change_value = cur_troops - unit_troops;
				if (change_value < 0)
				{
					if (rettype == 6 or rettype == 14 or rettype == 100) //6建筑自动攻击, 14部队伤害函数，100就是默认0.1死 0.1伤 0.8逃
					{
						unitinfo@ unit_t = @unit_ex[src.get_id()];
						if (调试模式) pk::trace(pk::format("value:{},wounded:{}", value, uint16(-value / 10)));
						unit_t.wounded += uint16(-value / 10);
						
						settinginfo@ set_t = @setting_ex;
						set_t.province_deserter[pk::get_province_id(src.pos)] += uint32(-value * 8 / 10);
						if (调试模式) pk::trace(pk::format("province_id:{},deserter:{}", pk::get_province_id(src.pos), set_t.province_deserter[pk::get_province_id(src.pos)]));
					}
					if (rettype == 3 or rettype == 5 or rettype == 9 or rettype == 11 or rettype == 12) //火伤害，地形伤害等 0.4死 0.2伤 0.4逃
					{
						unitinfo@ unit_t = @unit_ex[src.get_id()];
						if (调试模式) pk::trace(pk::format("value:{},wounded:{}", value, uint16(-value * 2 / 10)));
						unit_t.wounded += uint16(-value * 2 / 10);
						
						settinginfo@ set_t = @setting_ex;
						set_t.province_deserter[pk::get_province_id(src.pos)] += uint32(-value * 4 / 10);
						if (调试模式) pk::trace(pk::format("province_id:{},deserter:{}", pk::get_province_id(src.pos), set_t.province_deserter[pk::get_province_id(src.pos)]));
					}
					if (rettype == 2 or rettype == 10 or rettype == 13 or rettype == 102) //2,10单挑兵力损失,13 缺粮减兵, 102 0.05死 0.95逃
					{
						settinginfo@ set_t = @setting_ex;
						set_t.province_deserter[pk::get_province_id(src.pos)] += uint32(-value * 9.5f / 10.f);
						if (调试模式) pk::trace(pk::format("province_id:{},deserter:{}", pk::get_province_id(src.pos), set_t.province_deserter[pk::get_province_id(src.pos)]));
					}

				}

				pk::set_troops(src, cur_troops);				
			}
			return change_value;
		}

		int callback1(pk::city@ src, int value, int rettype)
		{
			if (!pk::is_alive(src)) return 0;
			int city_troops = src.troops;
			int max_troops = pk::get_max_troops(src);
			int cur_troops = 0;
			int change_value = 0;
			if (city_troops + value > 0)
			{
				cur_troops = max_troops;
				if (max_troops > city_troops + value)
					cur_troops = city_troops + value;
			}
			else
			{
				cur_troops = 0;
			}
			if (pk::is_alive(src))
			{
				pk::set_troops(src, cur_troops);
				change_value = cur_troops - city_troops;
				if (change_value < 0)
				{
					if (rettype == 0 or rettype == 100)//rettype == 0 兵舍被打破后的兵力变化
					{
						BaseInfo@ base_t = @base_ex[src.get_id()];
						if (调试模式) pk::trace(pk::format("value:{},wounded:{}", change_value, uint32(-change_value / 10)));
						base_t.wounded += uint16(-change_value / 10);
						
						settinginfo@ set_t = @setting_ex;
						set_t.province_deserter[pk::get_province_id(src.pos)] += uint32(-change_value * 8 / 10);
						if (调试模式) pk::trace(pk::format("province_id:{},deserter:{}", pk::get_province_id(src.pos), set_t.province_deserter[pk::get_province_id(src.pos)]));
					}
					if (rettype == 101) //火，水，瘟疫等 0.4死 0.2伤 0.4逃
					{
						BaseInfo@ base_t = @base_ex[src.get_id()];
						if (调试模式) pk::trace(pk::format("value:{},wounded:{}", value, uint16(-value * 2 / 10)));
						base_t.wounded += uint16(-value * 2 / 10);
						
						settinginfo@ set_t = @setting_ex;
						set_t.province_deserter[pk::get_province_id(src.pos)] += uint32(-value * 4 / 10);
						if (调试模式) pk::trace(pk::format("province_id:{},deserter:{}", pk::get_province_id(src.pos), set_t.province_deserter[pk::get_province_id(src.pos)]));
					}
					if (rettype == 102) //102 0.05死 0.95逃
					{
						settinginfo@ set_t = @setting_ex;
						set_t.province_deserter[pk::get_province_id(src.pos)] += uint32(-value * 9.5f / 10.f);
						if (调试模式) pk::trace(pk::format("province_id:{},deserter:{}", pk::get_province_id(src.pos), set_t.province_deserter[pk::get_province_id(src.pos)]));
					}
				}

				if (cur_troops == 0) pk::set_energy(src, 0);
			}
			return (cur_troops - city_troops);
		}

		int callback2(pk::building@ src, int value, int rettype)
		{
			if (!pk::is_alive(src)) return 0;
			int building_troops = src.get_troops();
			int max_troops = pk::get_max_troops(src);
			int cur_troops = 0;
			int change_value = 0;
			if (building_troops + value > 0)
			{
				cur_troops = max_troops;
				if (max_troops > building_troops + value)
					cur_troops = building_troops + value;
			}
			else
			{
				cur_troops = 0;
			}
			if (pk::is_alive(src))
			{
				pk::set_troops(src, cur_troops);
				change_value = cur_troops - building_troops;
				if (change_value < 0)
				{
					if (rettype == 11 or rettype == 8 or rettype == 100)//rettype == 11 攻击建筑伤害 209伤害函数的后续处理
					{
						BaseInfo@ base_t = @base_ex[src.get_id()];
						//pk::trace(pk::format("value:{},wounded:{}", change_value, uint32(-change_value / 10)));
						base_t.wounded += uint16(-change_value / 10);
						
						settinginfo@ set_t = @setting_ex;
						set_t.province_deserter[pk::get_province_id(src.pos)] += uint32(-change_value * 8 / 10);
						//pk::trace(pk::format("province_id:{},deserter:{}", pk::get_province_id(src.pos), set_t.province_deserter[pk::get_province_id(src.pos)]));
					}
					if (rettype == 5 or rettype == 6 or rettype == 9 or rettype == 101) //火，水，瘟疫等 0.4死 0.2伤 0.4逃
					{
						BaseInfo@ base_t = @base_ex[src.get_id()];
						//pk::trace(pk::format("value:{},wounded:{}", value, uint16(-value * 2 / 10)));
						base_t.wounded += uint16(-value * 2 / 10);
						
						settinginfo@ set_t = @setting_ex;
						set_t.province_deserter[pk::get_province_id(src.pos)] += uint32(-value * 4 / 10);
						//pk::trace(pk::format("province_id:{},deserter:{}", pk::get_province_id(src.pos), set_t.province_deserter[pk::get_province_id(src.pos)]));
					}
					if (rettype == 7 or rettype == 102) //102 0.05死 0.95逃
					{
						settinginfo@ set_t = @setting_ex;
						set_t.province_deserter[pk::get_province_id(src.pos)] += uint32(-value * 9.5f / 10.f);
						//pk::trace(pk::format("province_id:{},deserter:{}", pk::get_province_id(src.pos), set_t.province_deserter[pk::get_province_id(src.pos)]));
					}

				}

				if (cur_troops == 0) pk::set_energy(src, 0);
			}
			//pk::trace(pk::format("cur_troops:{},building_troops:{},change_value:{}", cur_troops, building_troops, change_value));
			return (cur_troops - building_troops);
		}
	}

	Main main;
}
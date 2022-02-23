// ## 2021/02/17 # 江东新风 # 无所属武将光环气力异常修复 ##
// ## 2021/01/31 # 江东新风 # rand函数错误修复 ##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/11/23 #江东新风# 配合光环可视化修改常量取得方式 ##
// ## 2020/11/02 #江东新风# 修复气力光环台词bug ##
// ## 2020/10/26 #氕氘氚# 修复武干没有最小光环效果的bug, 改了下军魂倍率 ##
// ## 2020/09/23 #江东新风# 修复气力光环台词重复显示bug ##
// ## 2020/09/21 #江东新风# 修复刚达到基础值时不获得最小光环效果的bug ##
// ## 2020/08/16 #江东新风# 刪除部分常量 ##
// ## 2020/08/14 #江东新风#将光环系统的具体执行部分全放入系统内置函数裡，仅保留光环相关的计算部分在此##
// ## 2020/08/10 #江东新风#配合k神内存修改器，现在可以拥有250新特技了##
// ## 2020/08/04 #江东新风#新增特技战神，整合636兵圣特技，兵圣，战神，武干，军魂支持?定宝物##
// ## 2020/08/01 #江东新风#修复了出征界面部队能力不同步的問題##
// @@ 2020/07/15 @ryan_knight_12@江东新风?譯自RK的sire光环系统@@
namespace halo
{
	///////////////////////////////////////////////////////////////////////////////
	/////////////////////////////				///////////////////////////////////
	/////////////////////////////	CUSTOMIZE	///////////////////////////////////
	/////////////////////////////				///////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	///基础光环0，战法暴击率1，暴击伤害2，计策成功3，计策识破4，辅助攻击5，气力回复6
	const int 光环基础范围 = 1; //

	const bool debug_mode = false;//开?debug模式

	int func_光环范围(pk::unit@unit, int 光环类型)//获取部队光环范围
	{
		int 光环基础范围 = int(pk::core["光环.基础光环基本范围"]);
		int 基础光环相关属性 = int(pk::core["光环.基础光环关联属性"]);
		int 战法暴击率光环相关属性 = int(pk::core["光环.暴击率光环关联属性"]);
		int 暴击伤害光环相关属性 = int(pk::core["光环.暴击伤害光环关联属性"]);
		int 计策成功率光环相关属性 = int(pk::core["光环.计略成功率关联属性"]);
		int 计策识破率光环相关属性 = int(pk::core["光环.计略识破率关联属性"]);
		int 辅助攻击光环相关属性 = int(pk::core["光环.辅助攻击关联属性"]);
		int 气力回复光环相关属性 = int(pk::core["光环.气力回复关联属性"]);
		int 一级基础光环需求 = int(pk::core["光环.基础光环阈值"]);
		int 二级基础光环需求 = int(pk::core["光环.基础光环阈值2"]);
		int 三级基础光环需求 = int(pk::core["光环.基础光环阈值3"]);
		int 战法暴击率光环属性需求 = int(pk::core["光环.暴击率光环阈值"]);
		int 暴击伤害光环属性需求 = int(pk::core["光环.暴击伤害光环阈值"]);
		int 计策成功率光环属性需求 = int(pk::core["光环.计略成功率阈值"]);
		int 计策识破率光环属性需求 = int(pk::core["光环.计略识破率阈值"]);
		int 辅助攻击光环属性需求 = int(pk::core["光环.辅助攻击阈值"]);
		int 气力回复光环属性需求 = int(pk::core["光环.气力回复阈值"]);

		int n = -1;
		if (unit !is null)
		{
			pk::person@dst_leader = pk::get_person(unit.leader);
			if (dst_leader is null)
				return -1;
			if (int(dst_leader.stat[基础光环相关属性]) < 一级基础光环需求)
				n = -1;
			if (int(dst_leader.stat[基础光环相关属性]) >= 一级基础光环需求)
				n = 光环基础范围;
			if (int(dst_leader.stat[基础光环相关属性]) >= 二级基础光环需求)
				n += 1;
			if (int(dst_leader.stat[基础光环相关属性]) >= 三级基础光环需求)
				n += 1;

			if ((ch::has_skill(dst_leader, 特技_武干)) and (int(dst_leader.stat[基础光环相关属性]) >= 一级基础光环需求))
			{
				n += 1;
			}
			else if ((ch::has_skill(dst_leader, 特技_武干)) and (int(dst_leader.stat[基础光环相关属性]) < 一级基础光环需求))
			{
				n = 光环基础范围;
			}

			if (光环类型 == 0)
				return n;
			else if ((光环类型 == 1) and int(dst_leader.stat[战法暴击率光环相关属性]) >= 战法暴击率光环属性需求)
				return n;
			else if ((光环类型 == 2) and int(dst_leader.stat[暴击伤害光环相关属性]) >= 暴击伤害光环属性需求)
				return n;
			else if ((光环类型 == 3) and int(dst_leader.stat[计策成功率光环相关属性]) >= 计策成功率光环属性需求)
				return n;
			else if ((光环类型 == 4) and int(dst_leader.stat[计策识破率光环相关属性]) >= 计策识破率光环属性需求)
				return n;
			else if ((光环类型 == 5) and int(dst_leader.stat[辅助攻击光环相关属性]) >= 辅助攻击光环属性需求)
				return n;
			else if ((光环类型 == 6) and int(dst_leader.stat[气力回复光环相关属性]) >= 气力回复光环属性需求)
				return n;
			else return -1;
		}
		return n;


	}

	int func_光环效果(pk::unit@unit, int 光环类型)//获取部队光环效果,需要目标武将
	{
		int 基础光环相关属性 = int(pk::core["光环.基础光环关联属性"]);
		int 战法暴击率光环相关属性 = int(pk::core["光环.暴击率光环关联属性"]);
		int 暴击伤害光环相关属性 = int(pk::core["光环.暴击伤害光环关联属性"]);
		int 计策成功率光环相关属性 = int(pk::core["光环.计略成功率关联属性"]);
		int 计策识破率光环相关属性 = int(pk::core["光环.计略识破率关联属性"]);
		int 辅助攻击光环相关属性 = int(pk::core["光环.辅助攻击关联属性"]);
		int 气力回复光环相关属性 = int(pk::core["光环.气力回复关联属性"]);
		int 一级基础光环需求 = int(pk::core["光环.基础光环阈值"]);
		int 二级基础光环需求 = int(pk::core["光环.基础光环阈值2"]);
		int 三级基础光环需求 = int(pk::core["光环.基础光环阈值3"]);
		int 战法暴击率光环属性需求 = int(pk::core["光环.暴击率光环阈值"]);
		int 暴击伤害光环属性需求 = int(pk::core["光环.暴击伤害光环阈值"]);
		int 计策成功率光环属性需求 = int(pk::core["光环.计略成功率阈值"]);
		int 计策识破率光环属性需求 = int(pk::core["光环.计略识破率阈值"]);
		int 辅助攻击光环属性需求 = int(pk::core["光环.辅助攻击阈值"]);
		int 气力回复光环属性需求 = int(pk::core["光环.气力回复阈值"]);
		int 基础光环系数 = int(pk::core["光环.基础光环效果系数"]);
		int 战法暴击率光环系数 = int(pk::core["光环.暴击率光环效果系数"]);
		int 暴击伤害光环系数 = int(pk::core["光环.暴击伤害光环效果系数"]);
		int 计策成功率光环系数 = int(pk::core["光环.计略成功率效果系数"]);
		int 计策识破率光环系数 = int(pk::core["光环.计略识破率效果系数"]);
		int 辅助攻击光环系数 = int(pk::core["光环.辅助攻击效果系数"]);
		int 气力回复光环系数 = int(pk::core["光环.气力回复效果系数"]);
		int 基础光环最小 = int(pk::core["光环.基础光环效果最小值"]);
		int 战法暴击率光环最小 = int(pk::core["光环.暴击率光环效果最小值"]);
		int 暴击伤害光环最小 = int(pk::core["光环.暴击伤害光环效果最小值"]);
		int 计策成功率光环最小 = int(pk::core["光环.计略成功率效果最小值"]);
		int 计策识破率光环最小 = int(pk::core["光环.计略识破率效果最小值"]);
		int 辅助攻击光环最小 = int(pk::core["光环.辅助攻击效果最小值"]);
		int 气力回复光环最小 = int(pk::core["光环.气力回复效果最小值"]);
		int 基础光环最大 = int(pk::core["光环.基础光环效果最大值"]);
		int 战法暴击率光环最大 = int(pk::core["光环.暴击率光环效果最大值"]);
		int 暴击伤害光环最大 = int(pk::core["光环.暴击伤害光环效果最大值"]);
		int 计策成功率光环最大 = int(pk::core["光环.计略成功率效果最大值"]);
		int 计策识破率光环最大 = int(pk::core["光环.计略识破率效果最大值"]);
		int 辅助攻击光环最大 = int(pk::core["光环.辅助攻击效果最大值"]);
		int 气力回复光环最大 = int(pk::core["光环.气力回复效果最大值"]);

		int n = 0;
		int ntemp = -1;
		float m = 1.f;
		pk::person@dst_leader = pk::get_person(unit.leader);

		if (ch::has_skill(unit, 特技_军魂))
			m = 军魂效果倍率 / 100.f;
		if (光环类型 == 0 and int(dst_leader.stat[基础光环相关属性]) >= 一级基础光环需求 or ch::has_skill(unit, 特技_武干))
		{
			ntemp = int(((pk::max(int(dst_leader.stat[基础光环相关属性]) - 一级基础光环需求, 0)) * 基础光环系数 / 100 + 基础光环最小) * m);
			n = pk::min(pk::max(ntemp, 基础光环最小), 基础光环最大);
			//ch::u8debug(pk::format("武将属性:{},ntemp为{}", int(dst_leader.stat[基础光环相关属性]), ntemp));
		}
		else if (光环类型 == 1 and int(dst_leader.stat[战法暴击率光环相关属性]) >= 战法暴击率光环属性需求)
		{
			ntemp = int(((int(dst_leader.stat[战法暴击率光环相关属性]) - 战法暴击率光环属性需求) * 战法暴击率光环系数 / 100 + 战法暴击率光环最小) * m);
			n = pk::min(pk::max(ntemp, 战法暴击率光环最小), 战法暴击率光环最大);
		}
		else if (光环类型 == 2 and int(dst_leader.stat[暴击伤害光环相关属性]) >= 暴击伤害光环属性需求)
		{
			ntemp = int(((int(dst_leader.stat[暴击伤害光环相关属性]) - 暴击伤害光环属性需求) * 暴击伤害光环系数 / 100 + 暴击伤害光环最小) * m);
			n = pk::min(pk::max(ntemp, 暴击伤害光环最小), 暴击伤害光环最大);
		}
		else if (光环类型 == 3 and int(dst_leader.stat[计策成功率光环相关属性]) >= 计策成功率光环属性需求)
		{

			ntemp = int(((int(dst_leader.stat[计策成功率光环相关属性]) - 计策成功率光环属性需求) * 计策成功率光环系数 / 100 + 计策成功率光环最小) * m);
			n = pk::min(pk::max(ntemp, 计策成功率光环最小), 计策成功率光环最大);
		}
		else if (光环类型 == 4 and int(dst_leader.stat[计策识破率光环相关属性]) >= 计策识破率光环属性需求)
		{
			ntemp = int(((int(dst_leader.stat[计策识破率光环相关属性]) - 计策识破率光环属性需求) * 计策识破率光环系数 / 100 + 计策识破率光环最小) * m);
			n = pk::min(pk::max(ntemp, 计策识破率光环最小), 计策识破率光环最大);
		}
		else if (光环类型 == 5 and int(dst_leader.stat[辅助攻击光环相关属性]) >= 辅助攻击光环属性需求)
		{
			ntemp = int(((int(dst_leader.stat[辅助攻击光环相关属性]) - 辅助攻击光环属性需求) * 辅助攻击光环系数 / 100 + 辅助攻击光环最小) * m);
			n = pk::min(pk::max(ntemp, 辅助攻击光环最小), 辅助攻击光环最大);
		}
		else if (光环类型 == 6 and int(dst_leader.stat[气力回复光环相关属性]) >= 气力回复光环属性需求)
		{
			ntemp = int(float((int(dst_leader.stat[气力回复光环相关属性]) - 气力回复光环属性需求) * 气力回复光环系数 / 100 + 气力回复光环最小) * m);
			n = pk::min(pk::max(ntemp, 气力回复光环最小), 气力回复光环最大);
			//pk::trace(pk::format("ntemp：{}，最终值：{}, 气力回复光环相关属性: {}, 武将属性：{}，气力回复光环属性需求: {}, 气力回复光环系数:{}", ntemp, n, 气力回复光环相关属性,dst_leader.stat[气力回复光环相关属性], 气力回复光环属性需求, 气力回复光环系数));
		}

		return n;
	}

	bool isbetter(pk::unit@src, pk::unit@dst, int 光环类型)//比較两部队主将官职和光环效果，前者大返回true，否则返回false
	{
		pk::person@src_leader = pk::get_person(src.leader);
		pk::person@dst_leader = pk::get_person(dst.leader);
		if (src_leader.rank < dst_leader.rank) //光环取官官职数值小者（君主为0，官职越大值越小）
			return true;
		else if (src_leader.rank > dst_leader.rank)
			return false;
		else
		{
			if (func_光环效果(src, 光环类型) >= func_光环效果(dst, 光环类型))
				return true;
			else return false;
		}
	}

	int func_get_highest_halo(pk::unit@src, int 光环类型)//获取附近有光环的友方部队中官职光环效果最高的部队id
	{
		int n = -1;
		if (func_光环范围(src, 光环类型) != -1)
			n = src.get_id();
		//int dst_id1 = -1;
		//int dst_id2 = -1;
		//获取附近友方部队列表
		/*
		pk::point pos = src.pos;
		array<pk::point> arr = pk::range(pos, 光环基础范围, 4);
		pk::list<pk::unit@> dst_list;
		for (int j = 0; j < int(arr.length); j++)
		{
			pk::unit@ dst = pk::get_unit(arr[j]);
			//pk::person@ dst_leader = dst.leader;
			if (!pk::is_enemy(src, dst))
			{
				dst_list.add(dst);
			}
		}
		*/
		if (src is null) return -1;
		pk::force@ force = pk::get_force(src.get_force_id());
		//array<pk::unit@> unit_list = pk::list_to_array(pk::get_unit_list(force));
		auto dst_list = pk::list_to_array(pk::get_unit_list(force));

		if (dst_list.length == 0)
			return -1;

		for (int i = 0; i < int(dst_list.length); i++)
		{

			int range = func_光环范围(dst_list[i], 光环类型);
			int distance = pk::get_distance(src.pos, dst_list[i].pos);

			//if (src.get_force_id() != dst_list[i].get_force_id()) continue;
			if (distance > range)
				continue;
			else if (dst_list[i].status != 部队状态_通常)
				continue;
			else
			{
				//如果id?空，则直接取代,否则取官职光环效果最高的部队id
				if (n == -1)
					n = dst_list[i].get_id();
				else
				{
					pk::unit@prev_halo = pk::get_unit(n);
					if (isbetter(prev_halo, dst_list[i], 光环类型))
						continue;
					else
						n = dst_list[i].get_id();
				}

			}

		}
		return n;
	}


	class Main
	{
		Main()
		{
			pk::bind(112, pk::trigger112_t(onTurnEnd));//气力回复
		}

		///气力回复光环		
		void onTurnEnd(pk::force@ force)
		{
			if ((!基础光环) or (!气力回复光环))
				return;
			auto dst_list = pk::list_to_array(pk::get_unit_list(force));
			if (dst_list.length != 0)
			{
				for (int i = 0; i < int(dst_list.length); i++)
				{
					int dst_id = func_get_highest_halo(dst_list[i], 6);
					if (dst_id != -1)
					{
						auto dst = pk::get_unit(dst_id);
						pk::person@ dst_leader = pk::get_person(dst.leader);
						//pk::unit@ dst = pk::get_unit(dst_id);
						int energy = func_光环效果(dst, 6);
						
						int max_energy;
						if (pk::has_tech(dst, 技巧_熟练兵)) max_energy = int(pk::core["部队.熟练兵最大气力"]) - dst_list[i].energy;
						else  max_energy = int(pk::core["部队.最大气力"]) - dst_list[i].energy;
						energy = pk::min(energy, max_energy);
						//pk::say(pk::encode(pk::format("见识下我\x1b[1x{}\x1b[0x的魅力吧.", pk::decode(pk::get_name(dst_leader)))),dst_leader);
						pk::add_energy(dst_list[i], energy, true);
						if (pk::rand_bool(30))
						{
							if (pk::is_in_screen(dst_list[i].pos))
							{
								pk::person@ 兵士 = pk::get_person(武将_兵士);
								switch (pk::rand(3))
								{
								case 0: pk::say(pk::encode(pk::format("能跟随着\x1b[1x{}\x1b[0x大人真是太棒了.", pk::decode(pk::get_name(dst_leader)))), 兵士); break;
								case 1: pk::say(pk::encode(pk::format("有\x1b[1x{}\x1b[0x大人在，我们无所畏惧.", pk::decode(pk::get_name(dst_leader)))), 兵士); break;
								case 2: pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x阁下真是我辈的榜样呀.", pk::decode(pk::get_name(dst_leader)))), 兵士); break;
								}
							}
						}

					}
				}
			}
		}
	}

	Main main;
} // end - namespace
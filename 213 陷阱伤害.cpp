// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/03/02 # 江东新风 # 将特殊地名争夺移入873 ##
// ## 2021/01/13 # 江东新风 # 修复特殊设施闪退bug2 ##
// ## 2021/01/12 # 江东新风 # 修复特殊设施闪退bug ##
// ## 2021/01/11 # 江东新风 # 修复异族伤害来源的特殊建筑破坏 ##
// ## 2021/01/05 # 江东新风 # 添加陷阱时特殊建筑不被破坏 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/08/12 # 氕氘氚 # 11:20 ##
namespace TRAP_DAMAGE
{

	//---------------------------------------------------------------------------


	//---------------------------------------------------------------------------

	class Main
	{

		Main()
		{
			pk::set_func(213, pk::func213_t(callback));
			//pk::bind(173, pk::trigger173_t(onUnitExpAdd));
		}

		void callback(pk::damage_info& info, int trap, pk::unit@ attacker, pk::hex_object@ target, bool critical)
		{
			int n = 0;
			int bakuyaku = 0; // 部队火伤_练成炸药附加 폭약연성 기교 추가 피해량
			//if (true)
			//{
			//	pk::unit@ target_unit = pk::hex_object_to_unit(target);
			//	pk::trace(pk::format("213,pos2,部队id:{}", target_unit.get_id()));
			//}

			if (target.is_instance(pk::unit::type_id))  //部队
			{
				bakuyaku = 部队火伤_练成炸药附加;
				switch (trap)
				{
				case 시설_불씨:  //火种
					n = 部队火伤_火种 + pk::rand(部队火伤_火种 / 3);
					break;
				case 시설_화구:  //火球
					n = 部队火伤_火球 + pk::rand(部队火伤_火球 / 3);
					break;
				case 시설_화선:  //火船
					n = 部队火伤_火船 + pk::rand(部队火伤_火船 / 2);
					break;
				case 시설_화염종:  //火焰种
					n = 部队火伤_火焰种 + pk::rand(部队火伤_火焰种 / 2);
					break;
				case 시설_화염구:  //火焰球
					n = 部队火伤_火焰球 + pk::rand(部队火伤_火焰球 / 2);
					break;
				case 시설_업화종:  //业火种
					n = 部队火伤_业火种 + pk::rand(部队火伤_业火种 / 2);
					break;
				case 시설_업화구:  //业火球
					n = 部队火伤_业火球 + pk::rand(部队火伤_业火球 / 2);
					break;
				case 시설_낙석:  //落石
					n = 部队落石伤害 + pk::rand(部队落石伤害 / 2);
					break;
				default:
					n = 0 + pk::rand(0);
					break;
				}
			}
			else  //建筑
			{
				bakuyaku = 建筑火伤_练成炸药附加;
				switch (trap)
				{
				case 시설_불씨:  //火种
					n = 建筑火伤_火种 + pk::rand(建筑火伤_火种 / 2);
					break;
				case 시설_화구:  //火球
					n = 建筑火伤_火球 + pk::rand(建筑火伤_火球 / 2);
					break;
				case 시설_화선:  //火船
					n = 建筑火伤_火船 + pk::rand(建筑火伤_火船 / 2);
					break;
				case 시설_화염종:  //火焰种
					n = 建筑火伤_火焰种 + pk::rand(建筑火伤_火焰种 / 2);
					break;
				case 시설_화염구:  //火焰球
					n = 建筑火伤_火焰球 + pk::rand(建筑火伤_火焰球 / 2);
					break;
				case 시설_업화종:  //业火种
					n = 建筑火伤_业火种 + pk::rand(建筑火伤_业火种 / 2);
					break;
				case 시설_업화구:  //业火球
					n = 建筑火伤_业火球 + pk::rand(建筑火伤_业火球 / 2);
					break;
				case 시설_낙석:  //落石
					n = 建筑落石伤害 + pk::rand(建筑落石伤害);
					break;
				case 시설_제방:  //提防
					n = pk::hex_object_to_building(target).hp;
					n = pk::max(n - pk::max(n - pk::rand(40) - 980, 1), 0);
					break;
				default:
					n = 0 + pk::rand(0);
					break;
				}
			}


			pk::unit@ target_unit = pk::hex_object_to_unit(target);
			if (target_unit !is null)
			{
				if (trap == 시설_낙석)  //落石
				{
					n = int(batt::unit_def_skill_impact(float(n), target_unit));
				}
				else  //火陷陷?
				{
					if (pk::is_alive(attacker))  //练成炸药
					{
						if (attacker.has_tech(기교_폭약연성))
						{
							n += 部队火伤_练成炸药附加;
							info.atk_tech = 기교_폭약연성;
						}
						if (ch::has_skill(attacker, 특기_화신))  //火神
							info.atk_skill = 특기_화신;
						else if (ch::has_skill(attacker, 특기_화공))  //火攻
							info.atk_skill = 특기_화공;
					}
					if (ch::has_skill(target_unit, 특기_등갑))  //藤甲
						info.def_skill = 특기_등갑;
					else if (ch::has_skill(target_unit, 특기_화신))  //火神
						info.def_skill = 특기_화신;
					else if (ch::has_skill(target_unit, 특기_화공))  //火攻
						info.def_skill = 특기_화공;

					n = batt::get_fire_final_damage(n, attacker, target_unit);
				}
				if (ch::has_skill(target_unit, 특기_답파))  //踏破
				{
					n = int(n * 踏破陷伤倍率 / 100.f);
					info.def_skill = 특기_답파;
				}
			}



			if (target.is_instance(pk::unit::type_id))
				info.troops_damage = n;
			else
			{
				//攻击落石时，第一次是部队对落石的攻击伤害，正常，落石对建筑的伤害，异常。。。
				//pk::trace(pk::format("陷阱pos1，n：{}",n));
				pk::building@ building = pk::hex_object_to_building(target);
				//ch::特殊地名争夺处理(building, attacker, destroyed_facility_id, destroy_unit_id, n, destroyed_building_pos, destroy_flag);
				//pk::trace(pk::format("陷阱pos2，n：{}", n));
				info.hp_damage = n;
			}


			//提防未恢复不能造成伤害
			if (target.is_instance(pk::building::type_id))
			{
				pk::building@ building = pk::hex_object_to_building(target);
				if (building.facility == 시설_제방 and not building.completed)
					info.hp_damage = 0;
			}
		}//callback
		/*
		//经验结算时添加
		int destroy_unit_id = -1;
		int destroyed_facility_id = -1;
		pk::point destroyed_building_pos = pk::point(-1, -1);
		bool destroy_flag = false;
		void onUnitExpAdd(pk::unit@ unit, int type)
		{
			//pk::trace(pk::format("攻击部队id:{}",destroy_unit.get_id()));	
			//pk::trace(pk::format("经验获得部队id：{}",unit.get_id()));
			ch::特殊地名设施_重建(unit, destroy_unit_id, destroyed_facility_id, destroyed_building_pos, destroy_flag, 213);
		}*/
	}

	Main main;
}
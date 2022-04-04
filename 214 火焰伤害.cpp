// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2021/03/02 # 江东新风 # 将特殊地名争夺移入873 ##
// ## 2021/02/12 # 氕氘氚 # 修复火焰秒杀城防的bug ##
// ## 2021/01/13 # 江东新风 # 修复特殊设施闪退bug2 ##
// ## 2021/01/12 # 江东新风 # 修复特殊设施闪退bug ##
// ## 2021/01/11 # 江东新风 # 修复异族伤害来源的特殊建筑破坏 ##
// ## 2021/01/05 # 江东新风 # 添加火焰时特殊建筑不被破坏 ##
// ## 2020/08/12 # 氕氘氚 # 11:20 ##
namespace FIRE_DAMAGE
{
	//---------------------------------------------------------------------------


	//---------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(214, pk::func214_t(callback));
			//pk::bind(173, pk::trigger173_t(onUnitExpAdd));
		}

		void callback(pk::damage_info& info, pk::unit@ attacker, pk::hex_object@ target, bool critical)
		{
			pk::force@ target_force = pk::get_force(target.get_force_id());
			int atk = 0;

			// 基础伤害
			if (target.is_instance(pk::unit::type_id))
			{
				atk = 部队火伤_练成炸药附加 + pk::rand(部队火伤_练成炸药附加 / 2);
				info.troops_damage = atk;
				pk::unit@ target_unit = pk::hex_object_to_unit(target);
				//pk::trace(pk::format("214,部队id:{}", target_unit.get_id()));
			}
			else
			{
				atk = 建筑火伤_练成炸药附加 + pk::rand(建筑火伤_练成炸药附加 / 2);
				info.hp_damage = atk;
			}

			if (bool(pk::core["fix_explosive_bug"]) ? pk::is_alive(attacker) and attacker.has_tech(기교_폭약연성) : pk::is_alive(target_force) and target.has_tech(기교_폭약연성))
			{
				info.troops_damage += atk;
				info.atk_tech = 기교_폭약연성;
				pk::unit@ target_unit = pk::hex_object_to_unit(target);
				//pk::trace(pk::format("214,pos2,部队id:{}", target_unit.get_id()));
			}

			// 技巧特技修正
			pk::unit@ target_unit = pk::hex_object_to_unit(target);
			if (target_unit !is null)

				if (pk::is_alive(attacker))
				{
					if (ch::has_skill(attacker, 특기_화신))  //火神
						info.atk_skill = 특기_화신;
					else if (ch::has_skill(attacker, 특기_화공))  //火攻
						info.atk_skill = 특기_화공;
				}
			if (target_unit !is null)
			{
				info.troops_damage = batt::get_fire_final_damage(info.troops_damage, attacker, target_unit);	//计算伤害修正

				if (ch::has_skill(target_unit, 특기_등갑))  //藤甲
					info.def_skill = 특기_등갑;
				else if (ch::has_skill(target_unit, 특기_화신))  //火神
					info.def_skill = 특기_화신;
				else if (ch::has_skill(target_unit, 특기_화공))  //火攻
					info.def_skill = 특기_화공;
			}


			if (target.is_instance(pk::building::type_id))
			{
				pk::building@ building = pk::hex_object_to_building(target);
				//if (building.facility > 2)
				//{
                //    ch::特殊地名争夺处理(building, attacker, destroyed_facility_id, destroy_unit_id, atk, destroyed_building_pos, destroy_flag);
                //    info.hp_damage = atk;
				//}
			    // 提防防止伤害
				if (building.facility == 시설_제방 and !building.completed)
					info.hp_damage = 0;
			}

			info.tactics = -1;
			if (pk::is_alive(attacker))
				info.src_pos = attacker.get_pos();
			info.dst_pos = target.get_pos();
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
			ch::特殊地名设施_重建(unit, destroy_unit_id, destroyed_facility_id, destroyed_building_pos, destroy_flag, 214);
		}*/
	}

	Main main;
}
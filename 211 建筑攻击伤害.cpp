// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/27 # 氕氘氚 # 新特技效果 ##
// ## 2020/09/30 # 氕氘氚 # 修复城市对攻城兵器无伤害的bug ##
// ## 2020/08/12 # 氕氘氚 # 22:45 ##
// 修改与 2020/7/14

namespace BUILDING_ATTACK_DAMAGE
{

	//---------------------------------------------------------------------------------------
	// 유저 설정 (true = on, false = off)

	//---------------------------------------------------------------------------------------


	class Main
	{

		Main()
		{
			pk::set_func(211, pk::func211_t(callback));
		}
		/*----------------------------------------
			건물의 공격(반격) 시 병력 대미지를 계산합니다.
			@param[out] damage	대미지 정보
			@param building		건물
			@param target		목표
		----------------------------------------*/
		void callback(pk::damage_info& info, pk::building@ attacker, pk::hex_object@ target)
		{
			// 사용하지 않음
			if (target.is_instance(pk::building::type_id))
				pk::assert(false);

			int facility_id = attacker.facility;
			pk::unit@ target_unit = pk::hex_object_to_unit(target);
			int troops_atk = 0, atk = 0, def = 0, troops = 0, city_def = 0;
			int target_def = target_unit.attr.stat[部队能力_防御];
			float troops_damage = 0;

			if (target_unit.weapon == 兵器_戟 && ch::has_skill(target_unit, 特技_重甲))
				target_def = target_def + 重甲防御提升量;
			if (target_unit.weapon >= 兵器_冲车 && target_unit.weapon <= 兵器_投石 && ch::has_skill(target_unit, 特技_重器))
				target_def = int(target_def * (100 + 重器防御提升比例) / 100.f);
			target_def = pk::max(255, target_def);

			switch (facility_id)
			{
			case 设施_城市:
			case 设施_关卡:
			case 设施_港口:
				//pk::trace(pk::decode(pk::get_name(target_unit)) + "before atk:" + atk + " city_def:" + city_def + " troops:" + troops);
				batt::get_city_def(troops_atk, atk, city_def, troops, attacker);
				//pk::trace("after atk:" + atk + " city_def:" + city_def + " troops:" + troops);
				troops_damage = batt::get_troops_damage(atk, troops, troops_atk, 0, target_def, batt::get_command(target_unit));
				//pk::trace("damage:" + troops_damage);
				if (pk::equipment_id_to_heishu(target_unit.weapon) == 兵种_兵器)
					troops_damage *= 建筑对攻城兵器伤害倍率 / 100.f;
				if (facility_id == 设施_关卡) troops_damage *= 1.5f;//关卡输出优待
				break;

			case 设施_阵:
			case 设施_砦:
			case 设施_城塞:
			case 设施_箭楼:
			case 设施_连弩楼:
			case 设施_投石台:
				troops_damage = batt::get_troops_damage(attacker, target_unit);
				if (pk::equipment_id_to_heishu(target_unit.weapon) == 병종_병기 and attacker.facility != 시설_투석대)
					troops_damage *= 建筑对攻城兵器伤害倍率 / 100.f;
				break;

			default:
				// 시설.자동공격 대미지
				troops_damage = batt::get_troops_damage(attacker, target_unit);
				if (pk::equipment_id_to_heishu(target_unit.weapon) == 병종_병기)
					troops_damage *= 建筑对攻城兵器伤害倍率 / 100.f;
				break;
			}

			// 防御建筑减免伤害  디버프
			int debuffer = batt::get_def_buff_building_id(target_unit);

			troops_damage = batt::get_troops_final_damage(troops_damage, attacker, target_unit, debuffer);
			info.troops_damage = int(troops_damage);

			// 병력 피해량에 따라 피격대상 기력감소, 공격측 기력상승 효과 발생 ('18.10.7) 
			// 气力变化
			if (气力增减设定)
			{
				if (info.troops_damage >= 300)
					info.energy_damage = int((info.troops_damage - 300) / 100);
			}

			info.src_pos = attacker.get_pos();
			info.dst_pos = target.get_pos();
		}


	}

	Main main;
}
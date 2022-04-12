// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/27 # 氕氘氚 # 新特技效果 ##
// ## 2020/09/06 # 氕氘氚 # 修复金刚不論伤害均生效的bug ##																   
// ## 2020/08/12 # 氕氘氚 # 22:45 ##

namespace ASSIST_DAMAGE
{
	//---------------------------------------------------------------------------------------
	// 유저 설정 (true = on, false = off)

	//---------------------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(210, pk::func210_t(callback));
		}
		/*----------------------------------------
			공격 피해량을 계산합니다.
			@param[out] damage	대미지 정보
			@param unit			부대
			@param tactics_id	#전법Id
			@param target_pos	목표 좌표
		----------------------------------------*/
		void callback(pk::damage_info& info, pk::unit@ attacker, const pk::point& in target_pos)
		{
			int force_id = attacker.get_force_id();
			pk::force@ force = pk::get_force(force_id);
			int weapon_id = attacker.weapon;
			int command = batt::get_command(attacker);
			pk::unit@ target_unit = pk::get_unit(target_pos);
			pk::building@ target_building = pk::get_building(target_pos);
			int atk = attacker.attr.stat[部队能力_攻击];
			int def = target_unit.attr.stat[部队能力_防御];
			int troops_atk = 0;
			int buffed = 0;

			if (pk::is_neighbor_pos(attacker.get_pos(), target_pos))
				troops_atk = 10;

			// 设施攻击加成
			if (batt::has_atk_buff_building(attacker.get_pos(), force_id))
			{
				buffed = 1;
				info.buffed = true;
			}

			// 防御建筑减免伤害  디버프
			int debuffer = batt::get_def_buff_building_id(target_unit);
			if (debuffer >= 0)
			{
				info.debuffer = debuffer;
			}

			// 크리티컬
			info.critical = false;

			float 정예기교_공격력 = 精锐兵伤害系数;
			float 단련기교_공격력 = 锻炼兵伤害系数;


			if (target_unit !is null)
			{
				float troops_damage = 0;
				// 特技攻防修正
				if (attacker.weapon == 兵器_枪 && pk::get_distance(attacker.pos, target_unit.pos) == 1 && ch::has_skill(attacker, 特技_摧锋))
					def = int(def * (100 - 摧锋破甲比例) / 100.f);
				if (attacker.weapon == 兵器_弩 && pk::get_distance(attacker.pos, target_unit.pos) == 1 && ch::has_skill(attacker, 特技_轻甲))
					atk = int(atk * (100 + 轻甲攻防提升比例) / 100.f);
				if (target_unit.weapon == 兵器_弩 && pk::get_distance(attacker.pos, target_unit.pos) == 1 && ch::has_skill(target_unit, 特技_轻甲))
					def = int(def * (100 + 轻甲攻防提升比例) / 100.f);
				if (target_unit.weapon == 兵器_戟 && ch::has_skill(target_unit, 特技_重甲))
					def = int(def + 重甲防御提升量);
				if (target_unit.weapon >= 兵器_冲车 && target_unit.weapon <= 兵器_投石 && ch::has_skill(target_unit, 特技_重器))
					def = int(def * (100 + 重器防御提升比例) / 100.f);
				def = pk::max(255, def);

				int atk_ability = 0;
				for (int m = 0; m < 3; m++)
				{
					if (pk::is_valid_person_id(attacker.member[m]))
					{
						pk::person@ member_t = pk::get_person(attacker.member[m]);  //队伍中的武将
						atk_ability += member_t.stat[武将能力_武力];
					}
				}

				troops_damage = batt::get_troops_damage(atk, command, troops_atk, buffed, def, batt::get_command(target_unit), atk_ability);

				switch (weapon_id)
				{
<<<<<<< HEAD
				case 병기_창:
=======
				case 兵器_枪:
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					if (pk::has_tech(force, 기교_정예창병))
						troops_damage *= 정예기교_공격력;
					else if (pk::has_tech(force, 기교_창병단련))
						troops_damage *= 단련기교_공격력;
					break;

<<<<<<< HEAD
				case 병기_극:
=======
				case 兵器_戟:
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					if (pk::has_tech(force, 기교_정예극병))
						troops_damage *= 정예기교_공격력;
					else if (pk::has_tech(force, 기교_극병단련))
						troops_damage *= 단련기교_공격력;
					break;

<<<<<<< HEAD
				case 병기_노:
=======
				case 兵器_弩:
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					if (pk::has_tech(force, 기교_정예노병))
						troops_damage *= 정예기교_공격력;
					else if (pk::has_tech(force, 기교_노병단련))
						troops_damage *= 단련기교_공격력;
					break;

<<<<<<< HEAD
				case 병기_군마:
=======
				case 兵器_战马:
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					if (pk::has_tech(force, 기교_정예기병))
						troops_damage *= 정예기교_공격력;
					else if (pk::has_tech(force, 기교_기병단련))
						troops_damage *= 단련기교_공격력;
					break;
				}

				info.troops_damage = int(troops_damage);
				if (batt::damage_avoided(info, attacker, target_unit, -1))
					troops_damage = 0;

				info.food_damage = batt::get_food_captured(attacker, target_unit);
				info.food_heal = info.food_damage;

				troops_damage = batt::get_troops_final_damage(troops_damage, attacker, target_unit, info.debuffer);
				info.troops_damage = int(troops_damage);

				if (ch::has_skill(attacker, 특기_소탕))
					info.energy_damage = pk::max(info.energy_damage, int(pk::core::skill_constant(attacker, 특기_소탕))); // 5
				if (ch::has_skill(attacker, 特技_威风))
					info.energy_damage = pk::max(info.energy_damage, int(pk::core::skill_constant(attacker, 特技_威风))); // 20

			}
			else if (target_building !is null and pk::is_general_type(target_building))
			{
				pk::assert(false);
			}
			if (target_building !is null)
			{
				pk::assert(false);
			}

			// 최소 병력 대미지
			if (info.troops_damage != 0)
				info.troops_damage = pk::max(int(info.troops_damage / 2.f), int(command / 20.f));
			info.troops_damage = int(info.troops_damage);

			// 최소 내구 대미지
			if (info.hp_damage != 0)
				info.hp_damage = pk::max(int(info.hp_damage / 2.f), int(command / 200.f));
			info.hp_damage = int(info.hp_damage);

			info.src_pos = attacker.get_pos();
			info.dst_pos = target_pos;
			info.type = 3; // ?
		}



	}

	Main main;
}
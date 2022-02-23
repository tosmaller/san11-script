// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/03/02 # 江东新风 # 将特殊地名争夺移入873 ##
// ## 2021/01/13 # 江东新风 # 修复特殊设施闪退bug2 ##
// ## 2021/01/12 # 江东新风 # 修复特殊设施闪退bug ##
// ## 2021/01/11 # 江东新风 # 修复异族伤害来源的特殊建筑破坏 ##
// ## 2021/01/05 # 江东新风 # 添加撞击时特殊建筑不被破坏 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/08/13 # 氕氘氚 # 23:40 ##
namespace COLLISION_DAMAGE
{
	class Main
	{
		Main()
		{
			pk::set_func(212, pk::func212_t(callback));
			//pk::bind(173, pk::trigger173_t(onUnitExpAdd));
		}

		void callback(pk::damage_info& info, pk::unit@ attacker, pk::hex_object@ sub_target)
		{
			int n = 0;

			// ?
			n = int(attacker.attr.stat[部队能力_攻击] * ch::randint(50, 150) / 100.f);
			info._20 = -pk::max(n, 1);

			if (sub_target.is_instance(pk::unit::type_id))
			{
				n = attacker.attr.stat[部队能力_攻击] * 2 - pk::hex_object_to_unit(sub_target).attr.stat[部队能力_防御];
				n = int(batt::unit_def_skill_impact(float(n), pk::hex_object_to_unit(sub_target)));
				if (n < 20) n = 20;
				info.troops_damage = pk::max(n + pk::rand(n), 1);

				// 공격/피격부대 병력 차에 따른 추가 효과 : 병력차의 5% 이내 랜덤 ('18.10.14)
				if (attacker.troops >= pk::hex_object_to_unit(sub_target).troops)
					info.troops_damage += pk::max(pk::rand((attacker.troops - pk::hex_object_to_unit(sub_target).troops) / 20), 0);

			}
			else
			{
				pk::building@ building = pk::hex_object_to_building(sub_target);
				if (!pk::is_trap_type(building))
				{
					n = attacker.attr.stat[部队能力_攻击] + pk::rand(attacker.attr.stat[部队能力_攻击] / 2);
					n = n / 2;
					n = pk::max(n, 1);
					//ch::特殊地名争夺处理(building, attacker, destroyed_facility_id, destroy_unit_id, n, destroyed_building_pos, destroy_flag);
					info.hp_damage = n;
				}
				else if (building.facility == 시설_제방 and !building.completed)
				{
					info.hp_damage = 0;
				}
			}

			info.tactics = -1;
			info.src_pos = attacker.get_pos();
			info.dst_pos = sub_target.get_pos();
		} //callback
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
			ch::特殊地名设施_重建(unit, destroy_unit_id, destroyed_facility_id, destroyed_building_pos, destroy_flag, 212);
		}*/
	}

	Main main;
}
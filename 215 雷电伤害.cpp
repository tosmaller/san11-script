// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/03/02 # 江东新风 # 将特殊地名争夺移入873 ##
// ## 2021/01/12 # 江东新风 # 修复特殊设施闪退bug ##
// ## 2021/01/11 # 江东新风 # 修复异族伤害来源的特殊建筑破坏 ##
// ## 2021/01/13 # 江东新风 # 修复特殊设施闪退bug2 ##
// ## 2021/01/05 # 江东新风 # 添加雷电时特殊建筑不被破坏 ##
// ## 2020/08/13 # 氕氘氚 # 23:40 ##
namespace LIGHTNING_DAMAGE
{
	//---------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(215, pk::func215_t(callback));
			//pk::bind(173, pk::trigger173_t(onUnitExpAdd));
		}

		void callback(pk::damage_info& info, pk::unit@ attacker, pk::hex_object@ target, bool critical)
		{
			if (target.is_instance(pk::unit::type_id))
			{
				info.troops_damage = 部队落雷伤害 + pk::rand(部队落雷伤害_1);

				//병력 피해의 2% 만큼 기력감소 ('18.10.11)
				info.energy_damage = info.troops_damage / 50;
			}
			else
			{
				int n = 建筑落雷伤害 + pk::rand(建筑落雷伤害_1);
				// 회복중인 제방일 경우 무효
				pk::building@ building = pk::hex_object_to_building(target);
				//ch::特殊地名争夺处理(building, attacker, destroyed_facility_id, destroy_unit_id, n, destroyed_building_pos, destroy_flag);
				info.hp_damage = n;
				if (building.facility == 시설_제방 and !building.completed)
					info.hp_damage = 0;
			}

			info.tactics = -1;
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
			ch::特殊地名设施_重建(unit, destroy_unit_id, destroyed_facility_id, destroyed_building_pos, destroy_flag, 215);
		}*/
	}

	Main main;
}
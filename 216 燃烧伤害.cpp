// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/03/02 # 江东新风 # 将特殊地名争夺移入873 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/08/12 # 氕氘氚 # 11:20 ##
namespace TERRAIN_FIRE_DAMAGE
{
	//---------------------------------------------------------------------------
	// 这个燃烧伤害其实是穿过火焰地形造成的伤害    
	const bool 着火减气 = true;

	//---------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(216, pk::func216_t(callback));
		}

		int callback(pk::unit@ unit, const pk::point& in pos)
		{
			// 스크립트 방식 수정, "伴砍_丙皆" 조건 추가 ('18.10.7)
			int n = int(ch::randint(2, 5) / 100.f * unit.troops + pk::rand(300));

			n = batt::fire_def_skill_impact(n, unit);

			//pk::trace(pk::format("部队id:{}，x:{},y:{}",unit.get_id(), pos.x, pos.y));
			//병력 피해의 1% 만큼 기력감소 气力减少受伤害的1% ('18.10.11)
			if (着火减气)
				pk::add_energy(unit, -n / 100);
			//pk::trace(pk::format("燃烧重建，x:{},y:{}",pos.x,pos.y));
			//特殊地名争夺处理_燃烧(pos, n);            
			return n;
		}
		/*
				void 特殊地名争夺处理_燃烧(pk::point pos, int &out hp_damage)
				{
					if (特定地点特定设施可争夺)
					{
						pk::building@ target_building = pk::get_building(pos);
						if (target_building !is null)
						{

							if (hp_damage >= int(target_building.hp))
							{
								if (地名设施.find(target_building.facility) >= 0)
								{
									for (int i = 0; i < int(特殊地名设施.length); i++)
									{
										pk::point pos0;
										pos0.x = 特殊地名设施[i][1];
										pos0.y = 特殊地名设施[i][2];
										if (pos0 == target_building.pos)
										{
											hp_damage = 0;
											pk::create_effect(91, target_building.pos);
											//pk::building@ building0 = pk::create_building(pk::get_facility(building.facility), pos,destroy_unit.get_force_id());
											//pk::set_district(target_building, pk::get_district(-1),0);
											//pk::trace(pk::format("燃烧重建",1));
											target_building.init_force = -1;
											target_building.hp = pk::get_max_hp(target_building);
											break;
										}
									}
								}
							}
						}
					}
				}
		*/
	}

	Main main;
}
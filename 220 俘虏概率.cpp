// ## 2022/03/28 # 江东新风 # 俘虏概率调整 ##
// ## 2022/03/17 # 江东新风 # 府兵空指针bug ##
// ## 2022/02/15 # 江东新风 # 府兵无法俘虏 ##
// ## 2021/10/28 # 江东新风 # 单港关存活设定变更 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2021/03/12 # 江东新风 # 用retfunc解决单港关俘虏释放后在野问题 ##
// ## 2021/02/18 # 江东新风 # 尝试修复单港关bug ##
// ## 2021/02/17 # 江东新风 # 修复空指针错误 ##
// ## 2021/02/08 # 江东新风 # 解决单港关，部队被灭武将在野的问题，修正任务外出武将的所属问题 ##
// ## 2021/01/24 # 江东新风 # 解决单港关，最后城被攻陷的武将变在野问题 ##
// ## 2021/01/14 # 江东新风 # 关掉trace ##
// ## 2020/12/29 # 江东新风 # 临时将普通武将的最终捕获率调整为原来的50% ##
// ## 2020/12/26 # 江东新风 # 关掉trace ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/15 #江东新风#大幅降低君主被俘虏概率，临时武将(农民兵，突袭者)不可捕获##
// ## 2020/08/16 #江东新风#has_skill函数替換##
namespace CAPTURE_CHANCE
{
	array<array<int>> 单城被灭武将;
	int base_id;
	int kunshu_id;
	class Main
	{
		Main()
		{
			pk::set_func(220, pk::func220_t(callback));
			pk::set_func(55, pk::func55_t(callback2));
		}

		void callback(const pk::destroy_info& in info, pk::list<pk::person@>& captured, pk::list<pk::person@>& escaped, bool tactics_bonus)
		{
			//pk::trace(pk::format("计算俘虏概率",1));

			pk::hex_object@ attacker = info.attacker;
			pk::hex_object@ target = info.target;

			int attacker_force_id = attacker.get_force_id();
			int target_force_id = target.get_force_id();
			pk::building@ attacker_building = attacker.get_type_id() == pk::building::type_id ? attacker : null;
			pk::unit@ attacker_unit = attacker.get_type_id() == pk::unit::type_id ? attacker : null;
			pk::building@ target_building = target.get_type_id() == pk::building::type_id ? target : null;
			pk::unit@ target_unit = target.get_type_id() == pk::unit::type_id ? target : null;
			int target_border_radius = pk::is_alive(target_building) and target_building.facility == 시설_도시 ? 2 : 1;
			pk::point attacker_pos = attacker.get_pos();
			pk::point target_pos = target.get_pos();
			bool in_border = pk::is_in_range(target_pos, target_border_radius, attacker_pos);
			bool cant_capture = !pk::is_valid_normal_force_id(attacker_force_id) or !pk::is_valid_normal_force_id(target_force_id);
			bool penalty = pk::get_scenario().difficulty == 难易度_超级 and attacker.is_player() and not target.is_player();
			bool has_hobaku_skill = false;
			int hobaku_chance = 0;
			int target_building_id_t = -1;
			pk::list<pk::person@> prisoner_list;
			//为单港关存活加的城市数量计数


			// 공격 오브젝트의 포박 특기 보유 확인确认攻击对象的特技
			if (pk::is_alive(attacker_unit))
			{
				has_hobaku_skill = in_border and ch::has_skill(attacker_unit, 특기_포박);
				hobaku_chance = int(pk::core::skill_constant(attacker_unit, 특기_포박));
			}
			else if (pk::is_alive(attacker_building))
			{
				int building_id = attacker_building.get_id();
				int hex_object_id = pk::building_id_to_hex_object_id(building_id);
				pk::list<pk::person@> list = pk::get_person_list(hex_object_id, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
				pk::filter_list(list, 무장속성_세력, attacker_force_id, COMPARE_EQ);

				if (list.size == 0)
					cant_capture = true;

				if (in_border)
				{
					for (int i = 0; i < list.size; i++)
					{
						if (ch::has_skill(list[i], 특기_포박))
						{
							has_hobaku_skill = true;
							hobaku_chance = pk::max(hobaku_chance, int(pk::core::skill_constant(list[i].get_id(), 특기_포박)));
						}
					}
				}
			}

			// 목표 오브젝트 무장 확인确认目标对象的武将
			if (pk::is_alive(target_unit))
			{
				for (int i = 0; i < 3; i++)
				{
					pk::person@ member = pk::get_person(target_unit.member[i]);
					if (pk::is_alive(member))
						prisoner_list.push_back(member);
				}
			}
			else if (pk::is_alive(target_building))
			{
				int building_id = target_building.get_id();
				target_building_id_t = building_id;
				int hex_object_id = pk::building_id_to_hex_object_id(building_id);
				prisoner_list = pk::get_person_list(hex_object_id, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
				pk::filter_list(prisoner_list, 무장속성_세력, target_force_id, COMPARE_EQ);

			}

			int bordered_unit_count = 0;
			int terrain_id = -1;

			if (pk::is_valid_pos(target_pos))
			{
				array<pk::point>@ range = pk::range(target_pos, 0, target_border_radius);
				for (int i = 0; i < int(range.length); i++)
				{
					pk::unit@ unit = pk::get_unit(range[i]);
					if (pk::is_alive(unit) and unit.get_force_id() == attacker_force_id)
						bordered_unit_count++;
				}
				terrain_id = pk::get_hex(target_pos).terrain;
			}


			for (int i = 0; i < prisoner_list.size; i++)
			{
				pk::person@ person = prisoner_list[i];

				if (cant_capture)
				{
					escaped.push_back(person);
					continue;
				}

				if (not info.releasable)
				{
					captured.push_back(person);
					continue;
				}

				if (pk::has_item(person, 보물종류_명마) or ch::has_skill(person, 특기_강운))
				{
					escaped.push_back(person);
					continue;
				}

				int n = 0;
				n += int(pk::core["person.max_stat"]) + 20 - pk::max(person.stat[武将能力_武力], person.stat[武将能力_智力]);
				n /= 3;
				if (bordered_unit_count > 0)
					n *= 100 * (ch::has_skill(person, 특기_철벽) ? 1 : bordered_unit_count);
				if (terrain_id == 地形_湿地 or terrain_id == 地形_毒泉)
					n = int(n * 1.5f);
				n /= 100;
				if (tactics_bonus)
					n += 10;
				n /= penalty ? 2 : 1;
				n += has_hobaku_skill ? hobaku_chance : 0;


				//君主捕缚率为十分之一
				if (pk::is_kunshu(person))
					n /= 10;
				//临时武将(农民兵，突袭者，府兵)不可捕缚
				if (is_临时武将(person))
					n = 0;

				n = int(pk::max(0, pk::min(n, 100)));
				//pk::trace(pk::format("person id:{};capture chance:{}",person.get_id(),n));
				if (pk::is_alive(attacker_unit) and pk::get_person(attacker_unit.leader).name_read == pk::encode("府兵"))//限制府兵捕获武将
				{
					//pk::trace("是府兵，无法俘虏");
					escaped.push_back(person);
				}
				else
				{
					//pk::trace("是府兵，俘虏概率计算");
					if (pk::rand_bool(n))
						captured.push_back(person);
					else
						escaped.push_back(person);
				}

			}
		}

		bool callback2(pk::person@ target_person, bool pre_return)
		{
			//true是返回所属，false是在野,
			int stat = pk::add_stat_exp(target_person, 武将能力_智力, 5);
			if (!pre_return and pk::get_onebase_survive())
			{
				int target_force_id = target_person.get_force_id();
				if (target_force_id < 非贼势力_末 and target_force_id > -1)
				{
					int location_id = target_person.location;
					if (location_id > 86) return true;
					else if (location_id > -1 and location_id < 87)
					{
						//此时用service可以避免返回武将所在和所属不符的问题
						pk::building@ target_building = pk::get_building(target_person.service);
						if (pk::is_alive(target_building))
						{
							int target_building_id = target_building.get_id();
							for (int building_id = 0; building_id < 据点_末; building_id++)
							{
								pk::building@building0 = pk::get_building(building_id);
								if (building0.get_force_id() == target_force_id and building_id != target_building_id) return true;
							}
						}
					}

				}

			}
			return pre_return;
		}

		bool is_临时武将(pk::person@ person)
		{
			if (pk::is_alive(person))
			{
				//@members[j] = @member_t;
				if (person.name_read == pk::encode("农民兵"))
					return true;
				if (person.name_read == pk::encode("突袭者"))
					return true;
				if (person.name_read == pk::encode("府兵"))
					return true;
			}
			return false;
		}

	}

	Main main;
}
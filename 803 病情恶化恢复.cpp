// ## 2021/01/17 # 江东新风 # 名将优待只考虑最大能力值(不计算伤病的能力) ##
// ## 2021/01/17 # 江东新风 # 修复部队势力指针误用建筑势力指针的bug(君主病死的来源?),名将优待设定 ##
// ## 2020/12/25 # 氕氘氚 # 修复势力空指针错误 ##
// ## 2020/12/25 # 氕氘氚 # 修复君主死亡继承问题，修正：势力的最后一个君主不会病死 ##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/12/04 # 氕氘氚 # 调整概率 ##
// @@ 2020/10/29 @ 氕氘氚 @@


namespace 病情恶化恢复
{
	const int 患病概率_0 = 1;   //每月（千分之n）  40岁及以下
	const int 患病概率_1 = 4;  //41-55岁
	const int 患病概率_2 = 6;  //56-70岁
	const int 患病概率_3 = 10;  //71岁及以上
	const int 恶化概率 = 4;	//每旬
	const int 病死概率 = 10;	//每旬
	const bool 开启患病 = true;
	const bool 开启恶化 = true;
	const bool 开启病死 = true;
	const bool 名将优待设定 = true;

	class Main
	{

		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));  //每旬
		}

		void callback()
		{
			pk::building@ building;
			pk::force@ force;

			for (int i = 0; i < 87; i++)
			{
				@building = pk::get_building(i);
				@force = pk::get_force(building.get_force_id());
				if (force is null) continue;

				bool has_doctor = ch::has_skill(building, 特技_医者, true);

				auto person_list = pk::list_to_array(pk::get_person_list(building, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)));

				for (int j = 0; j < int(person_list.length); j++)
				{
					pk::person@ person = person_list[j];
					if (!pk::is_unitize(person))
					{
						switch (person.shoubyou)
						{
						case 0:
							if (func_患病判断(has_doctor, pk::get_age(person)))  //每月检查
							{
								person.shoubyou = ch::randint(1, 2);
								show_message_sick(person);
							}
							break;
						case 1:
						case 2:
							if (has_doctor)
							{
								person.shoubyou = person.shoubyou - 1;
							}
							else if (func_恶化判断(person))
							{
								person.shoubyou = person.shoubyou + 1;
								show_message_worse(person);
							}
							break;
						case 3:
							if (has_doctor)
							{
								person.shoubyou = person.shoubyou - 1;
							}
							else if (func_病死判断(person))
							{
								if (force.kunshu != person.get_id())  // 防止君主病死
								{
									pk::person@ 君主 = pk::get_person(force.kunshu);
									pk::kill(person, null, null, 君主, 2);
									show_message_death(person);
								}
							}
							else
							{
								person.death = person.death - ch::randint(1, 3);
							}
						default:
							break;
						}
					}
				}
			}

			auto unit_list = pk::list_to_array(pk::get_unit_list());
			pk::unit@ unit;
			pk::person@ person;
			pk::force@ unit_force;

			for (int i = 0; i < int(unit_list.length); i++)
			{
				@unit = unit_list[i];
				@unit_force = pk::get_force(unit.get_force_id());
				for (int m = 0; m < 3; m++)
				{
					@person = pk::get_person(unit.member[m]);  //隊伍中的武將
					if (person is null || !pk::is_alive(person)) continue;
					bool has_doctor = ch::has_skill(building, 特技_医者, true);

					switch (person.shoubyou)
					{
					case 0:
						if (func_患病判断(has_doctor, pk::get_age(person)))  //每月检查
						{
							person.shoubyou = 2;
							show_message_sick(person);
						}
						break;
					case 1:
					case 2:
						if (has_doctor)
						{
							person.shoubyou = person.shoubyou - 1;
						}
						else if (func_恶化判断(person))
						{
							person.shoubyou = person.shoubyou + 1;
							show_message_worse(person);
						}
						break;
					case 3:
						if (has_doctor)
						{
							person.shoubyou = person.shoubyou - 1;
						}
						else if (func_病死判断(person))
						{
							if (unit_force !is null)
							{
								if (unit_force.kunshu != person.get_id())  // 防止君主病死
								{
									pk::kill(person, null, unit, null, 2);
									show_message_death(person);
								}
							}

						}
						else
						{
							person.death = person.death - ch::randint(1, 3);
						}
					default:
						break;
					}
				}
			}

		}

		bool func_患病判断(bool has_doctor, int age)
		{
			if (开启患病 && pk::get_day() == 1 && !has_doctor)
			{
				if (pk::get_scenario().life == 3)	return (ch::randint(0, 999) < 患病概率_0);
				else if (age <= 40)					return (ch::randint(0, 999) < 患病概率_0);
				else if (age >= 41 && age <= 55)	return (ch::randint(0, 999) < 患病概率_1);
				else if (age >= 56 && age <= 70)	return (ch::randint(0, 999) < 患病概率_2);
				else								return (ch::randint(0, 999) < 患病概率_3);
			}
			else
				return false;
		}

		bool func_恶化判断(pk::person@ person)
		{
			if (开启恶化)
			{
				if (名将优待设定)
				{
					if (person.max_stat[武将能力_武力] > 90) return (ch::randint(0, 999) <= int(恶化概率 * 10 / 8));
					else if (person.max_stat[武将能力_统率] > 90) return (ch::randint(0, 999) <= int(恶化概率 * 10 / 6));
					else if (person.max_stat[武将能力_智力] > 90) return (ch::randint(0, 999) <= int(恶化概率 * 10 / 4));
					else if (person.max_stat[武将能力_政治] > 90) return (ch::randint(0, 999) <= int(恶化概率 * 10 / 4));
					else if (person.max_stat[武将能力_魅力] > 90) return (ch::randint(0, 999) <= int(恶化概率 * 10 / 4));
				}
				return pk::rand_bool(恶化概率);
			}
			return false;
		}

		bool func_病死判断(pk::person@ person)
		{
			if (pk::get_scenario().battle_death == 0 or !开启病死) return false;
			if (名将优待设定)
			{
				if (person.max_stat[武将能力_武力] > 90) return (ch::randint(0, 999) <= int(病死概率 * 10 / 8));
				else if (person.max_stat[武将能力_统率] > 90) return (ch::randint(0, 999) <= int(病死概率 * 10 / 6));
				else if (person.max_stat[武将能力_智力] > 90) return (ch::randint(0, 999) <= int(病死概率 * 10 / 4));
				else if (person.max_stat[武将能力_政治] > 90) return (ch::randint(0, 999) <= int(病死概率 * 10 / 4));
				else if (person.max_stat[武将能力_魅力] > 90) return (ch::randint(0, 999) <= int(病死概率 * 10 / 4));
			}
			return pk::rand_bool(病死概率);
		}

		void show_message_death(pk::person@ person)
		{
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[29x不治身亡了！", pk::decode(pk::get_name(person)))));
		}

		void show_message_worse(pk::person@ person)
		{
			pk::force@ force = pk::get_force(person.get_force_id());
			int color = (force is null) ? -1 : force.color;
			pk::history_log(person.pos, color, pk::encode(pk::format("\x1b[1x{}\x1b[22x的健康状态每况愈下...", pk::decode(pk::get_name(person)))));
		}

		void show_message_sick(pk::person@ person)
		{
			pk::force@ force = pk::get_force(person.get_force_id());
			int color = (force is null) ? -1 : force.color;
			pk::history_log(person.pos, color, pk::encode(pk::format("\x1b[1x{}\x1b[24x似乎染上了疾病...", pk::decode(pk::get_name(person)))));
		}

	}

	Main main;
}
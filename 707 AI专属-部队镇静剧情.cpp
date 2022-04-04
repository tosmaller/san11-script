// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/31 # 江东新风 # 同步honeybee大神的更新##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: 통솔 or 지력 수치에 따라 일정확률로 AI 부대가 진정(혼란 회복) 및 행동가능 상태가 되는 SCRIPT
@기타: 혼란으로 AI 부대가 속수무책으로 당하는 것을 완화 (확률설정에 따라서 혼란류 특기/전법의 가치가 다소 하락할 수 있음)

※ 해당 SCRIPT 효과로 혼란에서 회복하는 AI 부대는 대신 기력이 소모됨


*/

namespace 진정_AI
{

	// ================ CUSTOMIZE ================


		///  TEST를 위한 목적으로 존재하는 변수이며, 효과_AI만 = true로 설정하는 것을 권장 
	const bool 효과_AI만 = true;  			// true =AI만 효과적용, false =유저와 AI 모두 효과 적용 


	const int 진정확률_70 = 5;      		// 진정될 확률 기본 5% (통솔 or 지력 70대 수치)
	const int 진정확률_80 = 10;      		// 진정될 확률 기본 10% (통솔 or 지력 80대 수치)
	const int 진정확률_90 = 15;      		// 진정될 확률 기본 15% (통솔 or 지력 90대 수치)

	// 70대 통솔, 지력 설정
	const int 통솔_70 = 70;      			// 최소 통솔 
	const int 통솔_79 = 79;      			// 최대 통솔  
	const int 지력_70 = 70;      			// 최소 지력
	const int 지력_79 = 79;      			// 최대 지력  

	// 80대 통솔, 지력 설정
	const int 통솔_80 = 80;      			// 최소 통솔 
	const int 통솔_89 = 89;      			// 최대 통솔  
	const int 지력_80 = 80;      			// 최소 지력
	const int 지력_89 = 89;      			// 최대 지력  

	// 90대 이상 통솔, 지력 설정
	const int 통솔_90 = 90;      			// 최소 통솔 
	const int 지력_90 = 90;      			// 최소 지력

	// AI부대가 혼란에서 회복하면서 소모되는 기력 설정
	const int 기력소모 = 5;      			// 소모되는 기력 수치 기본 5 


// ===========================================


	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{
			effect_진정();
		}

		// 유저 또는 AI만 적용 시 판단함수
		bool only_AI_unit(pk::unit@ unit)
		{
			if (unit.is_player()) return false;
			return true;
		}

		void effect_진정()
		{
			auto list = pk::list_to_array(pk::get_unit_list());

			for (int i = 0; i < int(list.length); i++)
			{
				pk::unit@ src_unit = list[i];
				pk::person@ src_leader = pk::get_person(src_unit.leader);

				if (only_AI_unit(src_unit))
				{
					// 통솔 70대 or 지력 70대인 경우
					if (((src_leader.stat[武将能力_统率] >= 통솔_70) and (src_leader.stat[武将能力_统率] <= 통솔_79))
						or ((src_leader.stat[武将能力_智力] >= 지력_70) and (src_leader.stat[武将能力_智力] <= 지력_79)))
					{

						// 확률 적용 
						if (pk::rand_bool(진정확률_70))
						{
							if (src_unit is null or src_unit.status == 部队状态_通常) continue;

							pk::set_status(src_unit, null, 部队状态_通常, 2, true);

							pk::add_energy(src_unit, -기력소모, true);

							src_unit.action_done = false;
							src_unit.update();

							if (pk::is_in_screen(src_unit.get_pos()))
							{
								switch (pk::rand(2))
								{
								case 0: pk::say(pk::encode("不要让敌人看见破绽!"), pk::get_person(src_unit.leader)); break;
								case 1: pk::say(pk::encode("还好冷静下来了..."), pk::get_person(src_unit.leader)); break;
								}
							}

						}

					} // 통솔 70대 or 지력 70대인 경우

					// 통솔 80대 or 지력 80대인 경우
					if (((src_leader.stat[武将能力_统率] >= 통솔_80) and (src_leader.stat[武将能力_统率] <= 통솔_89))
						or ((src_leader.stat[武将能力_智力] >= 지력_80) and (src_leader.stat[武将能力_智力] <= 지력_89)))
					{

						// 확률 적용 
						if (pk::rand_bool(진정확률_80))
						{
							if (src_unit is null or src_unit.status == 部队状态_通常) continue;

							pk::set_status(src_unit, null, 部队状态_通常, 2, true);

							pk::add_energy(src_unit, -기력소모, true);

							src_unit.action_done = false;
							src_unit.update();

							if (pk::is_in_screen(src_unit.get_pos()))
							{
								switch (pk::rand(2))
								{
								case 0: pk::say(pk::encode("不要让敌人看见破绽!"), pk::get_person(src_unit.leader)); break;
								case 1: pk::say(pk::encode("还好冷静下来了..."), pk::get_person(src_unit.leader)); break;
								}
							}

						}

					} // 통솔 80대 or 지력 80대인 경우

					// 통솔 90대 or 지력 90대 이상인 경우
					if ((src_leader.stat[武将能力_统率] >= 통솔_90) or (src_leader.stat[武将能力_智力] >= 지력_90))
					{

						// 확률 적용 
						if (pk::rand_bool(진정확률_90))
						{
							if (src_unit is null or src_unit.status == 部队状态_通常) continue;

							pk::set_status(src_unit, null, 部队状态_通常, 2, true);

							pk::add_energy(src_unit, -기력소모, true);

							src_unit.action_done = false;
							src_unit.update();

							if (pk::is_in_screen(src_unit.get_pos()))
							{
								switch (pk::rand(2))
								{
								case 0: pk::say(pk::encode("不要让敌人看见破绽!"), pk::get_person(src_unit.leader)); break;
								case 1: pk::say(pk::encode("还好冷静下来了..."), pk::get_person(src_unit.leader)); break;
								}
							}

						}

					} // 통솔 90대 or 지력 90대 이상인 경우

				}

			} // for 

		} // effect_진정()

	} // class Main

	Main main;

} // namespace
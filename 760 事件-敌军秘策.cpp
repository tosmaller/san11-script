// ## 2021/01/17 # 江东新风 # 修复无玩家时也触发的bug，提升性能 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@내용: 일정확률로 발생하는 적군의 비책으로 인한 출진한 모든 아군 부대 피해 효과
@기타: 난이도 상승 목적을 위해 제작한 SCRIPT

*/

namespace 적군_비책
{


	// ================ CUSTOMIZE ================

	const int whenToActivate = 12;   // 몇개월이 지난 시점부터 적군 비책이 발동할지 (기본 12개월 설정)

	const int 적군비책_발동시기 = 3;        // 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일,  4: 랜덤 턴
	const int 적군비책_발동확률_턴 = 5;     // 발동시기를 랜덤으로 설정했을 경우: 턴별 발동 확률
	const int 적군비책_발동확률 = 5;       // 0,1,2,3 설정일 경우 적군 비책 발동 확률 (기본 5% 설정) 

// ===========================================


	class Main
	{

		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{
			if (pk::get_player_count() == 0) return;
			if (적군비책_발동시기 == 0)
				secret_plan();  // 매턴
			else if (적군비책_발동시기 == 1 and (pk::get_day() == 1))
				secret_plan();  // 매월 1일
			else if (적군비책_발동시기 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
				secret_plan();  // 매분기 첫달이면서 1일
			else if (적군비책_발동시기 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
				secret_plan();  // 매년 1월 1일
			else if (적군비책_발동시기 == 4 and pk::rand_bool(적군비책_발동확률_턴))
				secret_plan();  // 랜덤 턴에서 발동
		}

		void secret_plan()
		{

			if (pk::rand_bool(적군비책_발동확률) and pk::get_elapsed_months() > whenToActivate)
			{
				auto list = pk::list_to_array(pk::get_unit_list());

				pk::person@ reporter_ = pk::get_person(무장_전령);

				int n = pk::rand(3);

				for (int i = 0; i < int(list.length); i++)
				{

					pk::unit@ src = list[i];

					if (!src.is_player()) continue;

					// 모든 아군 부대 혼란 피해
					if (n == 0)
					{
						pk::set_status(src, null, 部队状态_混乱, 3, true);

						if (pk::is_in_screen(src.pos))
						{
							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("咳,这是敌军的计谋吗!"), pk::get_person(src.leader), src); break;
							case 1: pk::say(pk::encode("是落入敌人的圈套里了吗!"), pk::get_person(src.leader), src); break;
							}
						}
					}

					// 모든 아군 부대 위보 피해
					if (n == 1)
					{
						pk::set_status(src, null, 部队状态_伪报, 3, true);

						if (pk::is_in_screen(src.pos))
						{
							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("城池因敌军奇袭而处于危险之中!"), pk::get_person(src.leader), src); break;
							case 1: pk::say(pk::encode("赶紧吧!快回到各自的据点内!"), pk::get_person(src.leader), src); break;
							}
						}
					}

					// 모든 아군 부대 화공 피해
					if (n == 2 and !pk::is_on_fire(src.pos))
					{
						pk::create_fire(src.pos, pk::rand(1) + 1 + 1, src, true);

						if (pk::is_in_screen(src.pos))
						{
							switch (pk::rand(4))
							{
							case 0: pk::say(pk::encode("哇哇哇.."), pk::get_person(무장_병사), src); break;
							case 1: pk::say(pk::encode("我的眼睛"), pk::get_person(무장_병사), src); break;
							case 2: pk::say(pk::encode("快去避火"), pk::get_person(무장_병사), src); break;
							case 3: pk::say(pk::encode("火.着火了!"), pk::get_person(무장_병사), src); break;
							}
						}
					}

				} // for

				pk::message_box(pk::encode("主公,听说我军被敌人的计谋狠狠地摆了一道!"), reporter_);

			} // if

		} // void secret_plan()

	} // class 

	Main main;
}
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: 도시 근처에서 전투 中이며, 특정 특기를 보유한 인물이 도시에 있을 경우 5% 확률로 수성전 책략을 사용 가능 (도시 근처에서 발생)

	 (1) 도시에 (간파 or 규율 or 침착 or 통찰 or 명경) 특기를 소유하고 있는 무장이 있을 경우 도시 근처에 존재하는 아군 부대의 상태 회복 효과 (미행동 부대로 전환)
	 (2) 도시에 (허실 or 심모 or 귀모 or 기략 or 매복 or 연환) 특기를 소유하고 있는 무장이 있을 경우 도시 근처에 존재하는 적군 부대에게 혼란 효과
	 (3) 도시에 (신산 or 반계 or 비책 or 언독 or 궤계) 특기를 소유하고 있는 무장이 있을 경우 도시 근처에 존재하는 적군 부대에게 위보 효과

*/

namespace 수성_책략
{

	// ================ CUSTOMIZE ================

	const int 수성책략_발동시기 = 1;       // 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일,  4: 랜덤 턴
	const int 수성책략_발동확률_턴 = 5;     // 발동시기를 랜덤으로 설정했을 경우: 턴별 발동 확률
	const int 수성책략_발동확률 = 5;       // 0,1,2,3 설정일 경우 수성전 책략 발동 확률 (기본 5% 설정)	

	const int min_distance = 1;      // 도시 근처 최소범위 
	const int max_distance = 3;      // 도시 근처 최대범위  

// ===========================================


	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{
			if (수성책략_발동시기 == 0)
				defence_Plan();  // 매턴
			else if (수성책략_발동시기 == 1 and (pk::get_day() == 1))
				defence_Plan();  // 매월 1일
			else if (수성책략_발동시기 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
				defence_Plan();  // 매분기 첫달이면서 1일
			else if (수성책략_발동시기 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
				defence_Plan();  // 매년 1월 1일
			else if (수성책략_발동시기 == 4 and pk::rand_bool(수성책략_발동확률_턴))
				defence_Plan();  // 랜덤 턴에서 발동
		}

		// 수성 책략 
		void defence_Plan()
		{

			auto list = pk::list_to_array(pk::get_city_list());

			for (int i = 0; i < int(list.length); i++)
			{

				pk::city@ city_ = list[i];

				pk::building@ building_ = pk::city_to_building(city_);

				pk::person@ taishu_ = pk::get_person(pk::get_taishu_id(building_));

				pk::force@ force_ = pk::get_force(building_.get_force_id());


				if (pk::is_alive(taishu_) and pk::enemies_around(building_))
				{

					auto person_list = pk::list_to_array(pk::get_person_list(building_, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)));

					if (pk::rand_bool(수성책략_발동확률))
					{

						if (0 < person_list.length)
						{

							for (int j = 0; j < int(person_list.length); j++)
							{

								pk::person@ person_0 = person_list[j];


								// 해당 특기를 보유한 무장이 도시에 있다면 상태 회복 효과 발휘 
								if ((!pk::is_absent(person_0) and !pk::is_unitize(person_0))
									and (person_0.skill == 특기_간파 or person_0.skill == 특기_규율 or person_0.skill == 특기_침착 or person_0.skill == 특기_통찰 or person_0.skill == 특기_명경))
								{

									array<pk::point> arr = pk::range(building_.get_pos(), min_distance, max_distance);

									for (int k = 0; k < int(arr.length); k++)
									{

										pk::unit@ dst = pk::get_unit(arr[k]);

										if (dst is null or dst.status == 部队状态_通常 or pk::is_enemy(building_, dst)) continue;

										pk::set_status(dst, null, 部队状态_通常, 1, true);

										dst.action_done = false;
										dst.update();

										if (pk::is_in_screen(building_.get_pos()))
										{
											switch (pk::rand(2))
											{
											case 0: pk::say(pk::encode("莫留空隙!"), pk::get_person(dst.leader)); break;
											case 1: pk::say(pk::encode("做好防守!"), pk::get_person(dst.leader)); break;
											}
										}

									}


								} // if 

								// 해당 특기를 보유한 무장이 도시에 있다면 혼란 효과 발휘 
								else if ((!pk::is_absent(person_0) and !pk::is_unitize(person_0))
									and (person_0.skill == 특기_허실 or person_0.skill == 특기_심모 or person_0.skill == 특기_귀모 or person_0.skill == 특기_기략 or person_0.skill == 특기_매복 or person_0.skill == 특기_연환))
								{

									array<pk::point> arr = pk::range(building_.get_pos(), min_distance, max_distance);

									for (int l = 0; l < int(arr.length); l++)
									{

										pk::unit@ dst = pk::get_unit(arr[l]);

										if (dst is null or dst.status != 部队状态_通常 or !pk::is_enemy(building_, dst)) continue;

										if (dst.has_skill(특기_침착) or dst.has_skill(특기_명경) or dst.has_skill(특기_통찰) or dst.has_skill(특기_신산)) continue;

										pk::set_status(dst, null, 部队状态_混乱, 2, true);

										if (pk::is_in_screen(building_.get_pos()))
										{
											switch (pk::rand(2))
											{
											case 0: pk::say(pk::encode("是敌人的突袭.."), pk::get_person(dst.leader)); break;
											case 1: pk::say(pk::encode("如此狼狈不堪.."), pk::get_person(dst.leader)); break;
											}
										}

									}

								} // if 

								// 해당 특기를 보유한 무장이 도시에 있다면 위보 효과 발휘 
								else if ((!pk::is_absent(person_0) and !pk::is_unitize(person_0))
									and (person_0.skill == 특기_신산 or person_0.skill == 특기_반계 or person_0.skill == 특기_비책 or person_0.skill == 특기_언독 or person_0.skill == 특기_궤계))
								{

									array<pk::point> arr = pk::range(building_.get_pos(), min_distance, max_distance);

									for (int m = 0; m < int(arr.length); m++)
									{

										pk::unit@ dst = pk::get_unit(arr[m]);

										if (dst is null or dst.status != 部队状态_通常 or !pk::is_enemy(building_, dst)) continue;

										if (dst.has_skill(특기_규율) or dst.has_skill(특기_명경) or dst.has_skill(특기_통찰) or dst.has_skill(특기_신산)) continue;

										pk::set_status(dst, null, 部队状态_伪报, 2, true);

										if (pk::is_in_screen(building_.get_pos()))
										{
											switch (pk::rand(2))
											{
											case 0: pk::say(pk::encode("后方被袭击了!?"), pk::get_person(dst.leader)); break;
											case 1: pk::say(pk::encode("后方陷落了!?"), pk::get_person(dst.leader)); break;
											}
										}

									}

								} // if 

							} // for 

						} // if 

					} // if (확률)

				} // if 

			} // for

		} // defence_Plan

	} // class Main

	Main main;

} // namespace
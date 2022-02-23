// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/07/15 # 白马叔叔 # 修复筑城建设中bug ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/31 # 江东新风 # 同步honeybee大神的更新##
/*
@제작자: HoneyBee
@설명: 통솔, 정치가 동시에 높은 무장이 특정 특기를 소유한 경우에 발휘하는 부대 효과


※ 보조 부대효과 설명 (확률적 발동)

(1) <부호> 특기를 소유 시 주위에 있는 아군 부대 금 상승
(2) <징세> 특기를 소유 시 주위에 있는 적군 부대 금 감소 (적군 부대가 부호 or 징세 특기를 소유 시 면역)
(3) <미도> 특기를 소유 시 주위에 있는 아군 부대 병량 상승
(4) <징수> 특기를 소유 시 주위에 있는 적군 부대 병량 감소 (적군 부대가 미도 or 징수 특기를 소유 시 면역)
(5) <축성> 특기를 소유 시 주위에 있는 아군 건물 내구 회복
(6) <인정> 특기를 소유 시 주위에 있는 아군 부대 충성도 상승 (통솔 + 매력이 동시에 높은 무장인 경우)
(7) <기원> 특기를 소유 시 주위에 있는 아군 부대 RANDOM 효과 (통솔 + 매력이 동시에 높은 무장인 경우)
	- [금 or 병량 or 기력 상승] or [상태이상 회복]
e
@说明:统率，政治同时高的武将拥有特定技术的情况下所发挥的部队效果

※辅助部队效果的说明(概率性发动)

(1)拥有<富豪>特技时周围的友方部队的金钱上升
(2)拥有<征税>特技时周围的敌方部队的金钱减少(敌人部队拥有富豪or征税特技时免疫)
(3)拥有<米道>特技时周围的友方部队的军粮上升
(4)拥有<征收>特技时周围的敌方部队的军粮减少(敌人部队拥有米道or征收特技时免疫)
(5)拥有<筑城>特技时周围的友方建筑物的耐久恢复
(6)拥有<仁政>特技时周围的我方部队的忠诚上升(统率+魅力同时高的武将)
(7)拥有<祈愿>特技时周围的友方部队RANDOM效果(统率+魅力同时高的武将)
[金or军粮or气力上升]or[状态异常恢复]
*/

namespace 보조_부대효과
{

	// ================ CUSTOMIZE ================


	const bool 仅AI有效 = false;  						// true =AI만 효과적용, false =유저와 AI 모두 효과 적용 
	const int 效果概率 = 15;      						    // 효과가 발휘되는 확률 (기본 15%)


	// ARRAY에 원하는 수치로 설정하시면 됩니다. 

	const array<int> 金上升_0 = { 300, 400, 500 };      		// [통솔 80대, 정치 80대]
	const array<int> 金上升_1 = { 400, 500, 600 };      		// [통솔 80 or 90대 (and) 정치 80 or 90대]
	const array<int> 金上升_2 = { 500, 600, 700 };      		// [통솔 90 이상 (and) 정치 90 이상]

	const array<int> 金减少_0 = { 300, 400, 500 };      		// [통솔 80대, 정치 80대]
	const array<int> 金减少_1 = { 400, 500, 600 };      		// [통솔 80 or 90대 (and) 정치 80 or 90대]
	const array<int> 金减少_2 = { 500, 600, 700 };      		// [통솔 90 이상 (and) 정치 90 이상]

	const array<int> 兵粮上升_0 = { 2000, 3000, 4000 };      // [통솔 80대, 정치 80대]
	const array<int> 兵粮上升_1 = { 3000, 4000, 5000 };      // [통솔 80 or 90대 (and) 정치 80 or 90대]
	const array<int> 兵粮上升_2 = { 4000, 5000, 6000 };      // [통솔 90 이상 (and) 정치 90 이상]

	const array<int> 兵粮减少_0 = { 2000, 3000, 4000 };      // [통솔 80대, 정치 80대]
	const array<int> 兵粮减少_1 = { 3000, 4000, 5000 };      // [통솔 80 or 90대 (and) 정치 80 or 90대]
	const array<int> 兵粮减少_2 = { 4000, 5000, 6000 };      // [통솔 90 이상 (and) 정치 90 이상]

	const array<int> 耐久恢复_0 = { 300, 400, 500 };      	// [통솔 80대, 정치 80대]
	const array<int> 耐久恢复_1 = { 400, 500, 600 };      	// [통솔 80 or 90대 (and) 정치 80 or 90대]
	const array<int> 耐久恢复_2 = { 500, 600, 700 };      	// [통솔 90 이상 (and) 정치 90 이상]

	const array<int> 忠诚度上升_0 = { 3, 4, 5 };      		// [통솔 80대, 매력 80대]
	const array<int> 忠诚度上升_1 = { 4, 5, 6 };      		// [통솔 80 or 90대 (and) 매력 80 or 90대]
	const array<int> 忠诚度上升_2 = { 6, 7, 8 };      		// [통솔 90 이상 (and) 매력 90 이상]

	const array<int> 气力上升_0 = { 5, 10, 15 };      		// [통솔 80대, 매력 80대]
	const array<int> 气力上升_1 = { 10, 15, 20 };      		// [통솔 80 or 90대 (and) 매력 80 or 90대]
	const array<int> 气力上升_2 = { 15, 20, 25 };      		// [통솔 90 이상 (and) 매력 90 이상]


	const int min_统率_0 = 80;      						// 최소 통솔 80 이상
	const int min_统率_1 = 90;      						// 최소 통솔 90 이상

	const int min_政治_0 = 80;      						// 최소 정치 80 이상
	const int min_政治_1 = 90;      						// 최소 정치 90 이상

	const int min_魅力_0 = 80;      						// 최소 매력 80 이상
	const int min_魅力_1 = 90;      						// 최소 매력 90 이상

	const int min_范围 = 1;      							// 효과를 받는 최소 범위 
	const int max_范围 = 3; 								// 효과를 받는 최대 범위 


// ===========================================


	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{
			auxiliary_effect();
		}

		// 유저 또는 AI만 적용 시 판단함수
		bool only_AI_unit(pk::unit@ unit)
		{
			if (仅AI有效 and unit.is_player()) return false;
			return true;
		}

		bool flag_금상승 = false;
		bool flag_금감소 = false;
		bool flag_병량상승 = false;
		bool flag_병량감소 = false;
		bool flag_내구회복 = false;
		bool flag_충성도상승 = false;
		bool flag_기력상승 = false;
		bool flag_진정 = false;

		bool flag_금상승0 = false;
		bool flag_금상승1 = false;
		bool flag_금상승2 = false;

		bool flag_병량상승0 = false;
		bool flag_병량상승1 = false;
		bool flag_병량상승2 = false;

		bool flag_기력상승0 = false;
		bool flag_기력상승1 = false;
		bool flag_기력상승2 = false;

		bool flag_주장_충성도상승0 = false;
		bool flag_주장_충성도상승1 = false;
		bool flag_주장_충성도상승2 = false;

		bool flag_부장1_충성도상승0 = false;
		bool flag_부장1_충성도상승1 = false;
		bool flag_부장1_충성도상승2 = false;

		bool flag_부장2_충성도상승0 = false;
		bool flag_부장2_충성도상승1 = false;
		bool flag_부장2_충성도상승2 = false;


		void auxiliary_effect()
		{

			int n = pk::rand(4);

			auto list = pk::list_to_array(pk::get_unit_list());

			for (int i = 0; i < int(list.length); i++)
			{

				pk::unit@ src = list[i];
				pk::person@ src_leader = pk::get_person(src.leader);
				pk::point pos = src.get_pos();

				pk::person@ src_deputy1 = pk::get_person(src.member[1]);
				pk::person@ src_deputy2 = pk::get_person(src.member[2]);

				if (only_AI_unit(src))
				{

					array<pk::point> arr = pk::range(pos, min_范围, max_范围);

					for (int j = 0; j < int(arr.length); j++)
					{
						pk::unit@ dst = pk::get_unit(arr[j]);
						pk::building@ dst_building = pk::get_building(arr[j]);

						if (pk::rand_bool(效果概率))
						{
							// <부호> 특기를 소유 시 주위에 있는 아군 부대 금 상승 
							if (src.has_skill(특기_부호) and (dst !is null))
							{
								// [통솔 80대, 정치 80대]
								if (((src_leader.stat[武将能力_统率] >= min_统率_0) and (src_leader.stat[武将能力_统率] < min_统率_1))
									and ((src_leader.stat[武将能力_政治] >= min_政治_0) and (src_leader.stat[武将能力_政治] < min_政治_1)))
								{
									flag_금상승 = true;
									flag_금상승0 = true;
									if (!pk::is_enemy(src, dst))
									{
										pk::add_gold(dst, 金上升_0[pk::rand(3)], true); // 금상승 
									}
								}

								// [통솔 80 or 90대 (and) 정치 80 or 90대]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_政治] >= min_政治_0 and src_leader.stat[武将能力_政治] < min_政治_1))
									or ((src_leader.stat[武将能力_政治] >= min_政治_1) and (src_leader.stat[武将能力_统率] >= min_统率_0 and src_leader.stat[武将能力_统率] < min_统率_1)))
								{
									flag_금상승 = true;
									flag_금상승1 = true;
									if (!pk::is_enemy(src, dst))
									{
										pk::add_gold(dst, 金上升_1[pk::rand(3)], true); // 금상승 
									}
								}

								// [통솔 90 이상 (and) 정치 90 이상]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_政治] >= min_政治_1)))
								{
									flag_금상승 = true;
									flag_금상승2 = true;
									if (!pk::is_enemy(src, dst))
									{
										pk::add_gold(dst, 金上升_2[pk::rand(3)], true); // 금상승 
									}
								}

							} // if (src.has_skill(특기_부호))


							// <징세> 특기를 소유 시 주위에 있는 적군 부대 금 감소 
							// (적군 부대가 부호 or 징세 특기를 소유 시 면역) 
							if (src.has_skill(특기_징세) and (dst !is null))
							{
								// [통솔 80대, 정치 80대]
								if (((src_leader.stat[武将能力_统率] >= min_统率_0) and (src_leader.stat[武将能力_统率] < min_统率_1))
									and ((src_leader.stat[武将能力_政治] >= min_政治_0) and (src_leader.stat[武将能力_政治] < min_政治_1)))
								{
									flag_금감소 = true;
									if (pk::is_enemy(src, dst))
									{
										if (!dst.has_skill(특기_징세) and !dst.has_skill(특기_부호))
										{
											pk::add_gold(dst, -金减少_0[pk::rand(3)], true); // 금감소 
										}
									}
								}

								// [통솔 80 or 90대 (and) 정치 80 or 90대]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_政治] >= min_政治_0 and src_leader.stat[武将能力_政治] < min_政治_1))
									or ((src_leader.stat[武将能力_政治] >= min_政治_1) and (src_leader.stat[武将能力_统率] >= min_统率_0 and src_leader.stat[武将能力_统率] < min_统率_1)))
								{
									flag_금감소 = true;
									if (pk::is_enemy(src, dst))
									{
										if (!dst.has_skill(특기_징세) and !dst.has_skill(특기_부호))
										{
											pk::add_gold(dst, -金减少_1[pk::rand(3)], true); // 금감소 
										}
									}
								}

								// [통솔 90 이상 (and) 정치 90 이상]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_政治] >= min_政治_1)))
								{
									flag_금감소 = true;
									if (pk::is_enemy(src, dst))
									{
										if (!dst.has_skill(특기_징세) and !dst.has_skill(특기_부호))
										{
											pk::add_gold(dst, -金减少_2[pk::rand(3)], true); // 금감소 
										}
									}
								}

							} // if (src.has_skill(특기_징세)) 


							// <미도> 특기를 소유 시 주위에 있는 아군 부대 병량 상승 
							if (src.has_skill(특기_미도) and (dst !is null))
							{
								// [통솔 80대, 정치 80대]
								if (((src_leader.stat[武将能力_统率] >= min_统率_0) and (src_leader.stat[武将能力_统率] < min_统率_1))
									and ((src_leader.stat[武将能力_政治] >= min_政治_0) and (src_leader.stat[武将能力_政治] < min_政治_1)))
								{
									flag_병량상승 = true;
									flag_병량상승0 = true;
									if (!pk::is_enemy(src, dst))
									{
										pk::add_food(dst, 兵粮上升_0[pk::rand(3)], true); // 병량상승 
									}
								}

								// [통솔 80 or 90대 (and) 정치 80 or 90대]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_政治] >= min_政治_0 and src_leader.stat[武将能力_政治] < min_政治_1))
									or ((src_leader.stat[武将能力_政治] >= min_政治_1) and (src_leader.stat[武将能力_统率] >= min_统率_0 and src_leader.stat[武将能力_统率] < min_统率_1)))
								{
									flag_병량상승 = true;
									flag_병량상승1 = true;
									if (!pk::is_enemy(src, dst))
									{
										pk::add_food(dst, 兵粮上升_1[pk::rand(3)], true); // 병량상승 
									}
								}

								// [통솔 90 이상 (and) 정치 90 이상]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_政治] >= min_政治_1)))
								{
									flag_병량상승 = true;
									flag_병량상승2 = true;
									if (!pk::is_enemy(src, dst))
									{
										pk::add_food(dst, 兵粮上升_2[pk::rand(3)], true); // 병량상승 
									}
								}

							} // if (src.has_skill(특기_미도))


							// <징수> 특기를 소유 시 주위에 있는 적군 부대 병량 감소 
							// (적군 부대가 미도 or 징수 특기를 소유 시 면역) 
							if (src.has_skill(특기_징수) and (dst !is null))
							{
								// [통솔 80대, 정치 80대]
								if (((src_leader.stat[武将能力_统率] >= min_统率_0) and (src_leader.stat[武将能力_统率] < min_统率_1))
									and ((src_leader.stat[武将能力_政治] >= min_政治_0) and (src_leader.stat[武将能力_政治] < min_政治_1)))
								{
									flag_병량감소 = true;
									if (pk::is_enemy(src, dst))
									{
										if (!dst.has_skill(특기_징수) and !dst.has_skill(특기_미도))
										{
											pk::add_food(dst, -兵粮减少_0[pk::rand(3)], true); // 병량감소 
										}
									}
								}

								// [통솔 80 or 90대 (and) 정치 80 or 90대]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_政治] >= min_政治_0 and src_leader.stat[武将能力_政治] < min_政治_1))
									or ((src_leader.stat[武将能力_政治] >= min_政治_1) and (src_leader.stat[武将能力_统率] >= min_统率_0 and src_leader.stat[武将能力_统率] < min_统率_1)))
								{
									flag_병량감소 = true;
									if (pk::is_enemy(src, dst))
									{
										if (!dst.has_skill(특기_징수) and !dst.has_skill(특기_미도))
										{
											pk::add_food(dst, -兵粮减少_1[pk::rand(3)], true); // 병량감소 
										}
									}
								}

								// [통솔 90 이상 (and) 정치 90 이상]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_政治] >= min_政治_1)))
								{
									flag_병량감소 = true;
									if (pk::is_enemy(src, dst))
									{
										if (!dst.has_skill(특기_징수) and !dst.has_skill(특기_미도))
										{
											pk::add_food(dst, -兵粮减少_2[pk::rand(3)], true); // 병량감소 
										}
									}
								}

							} // if (src.has_skill(특기_징수)) 


							// <축성> 특기를 소유 시 주위에 있는 아군 건물 내구 회복 
							if (src.has_skill(特技_筑城) and (dst_building !is null))
							{
								// [통솔 80대, 정치 80대]
								if (((src_leader.stat[武将能力_统率] >= min_统率_0) and (src_leader.stat[武将能力_统率] < min_统率_1))
									and ((src_leader.stat[武将能力_政治] >= min_政治_0) and (src_leader.stat[武将能力_政治] < min_政治_1)))
								{
									flag_내구회복 = true;
									if (!pk::is_enemy(src, dst_building))
									{
										pk::add_hp(dst_building, 耐久恢复_0[pk::rand(3)], true); // 내구회복
										if (int(dst_building.hp) >= pk::get_max_hp(dst_building) && !dst_building.completed) pk::complete(dst_building);  //如果耐久足够 就完成了
									}
								}

								// [통솔 80 or 90대 (and) 정치 80 or 90대]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_政治] >= min_政治_0 and src_leader.stat[武将能力_政治] < min_政治_1))
									or ((src_leader.stat[武将能力_政治] >= min_政治_1) and (src_leader.stat[武将能力_统率] >= min_统率_0 and src_leader.stat[武将能力_统率] < min_统率_1)))
								{
									flag_내구회복 = true;
									if (!pk::is_enemy(src, dst_building))
									{
										pk::add_hp(dst_building, 耐久恢复_1[pk::rand(3)], true); // 내구회복
										if (int(dst_building.hp) >= pk::get_max_hp(dst_building) && !dst_building.completed) pk::complete(dst_building);  //如果耐久足够 就完成了
									}
								}

								// [통솔 90 이상 (and) 정치 90 이상]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_政治] >= min_政治_1)))
								{
									flag_내구회복 = true;
									if (!pk::is_enemy(src, dst_building))
									{
										pk::add_hp(dst_building, 耐久恢复_2[pk::rand(3)], true); // 내구회복 
										if (int(dst_building.hp) >= pk::get_max_hp(dst_building) && !dst_building.completed) pk::complete(dst_building);  //如果耐久足够 就完成了
									}
								}

							} // if (src.has_skill(特技_筑城))


							// <인정> 특기를 소유 시 주위에 있는 아군 부대 충성도 상승 
							if (src.has_skill(특기_인정) and (dst !is null))
							{

								pk::person@ dst_leader = pk::get_person(dst.leader);

								// [통솔 80대, 매력 80대]
								if (((src_leader.stat[武将能力_统率] >= min_统率_0) and (src_leader.stat[武将能力_统率] < min_统率_1))
									and ((src_leader.stat[武将能力_魅力] >= min_魅力_0) and (src_leader.stat[武将能力_魅力] < min_魅力_1)))
								{

									flag_충성도상승 = true;
									flag_주장_충성도상승0 = true;

									if (pk::is_alive(src_deputy1) and !pk::is_alive(src_deputy2))
									{
										flag_부장1_충성도상승0 = true;
									}
									else if (pk::is_alive(src_deputy1) and pk::is_alive(src_deputy2))
									{
										flag_부장1_충성도상승0 = true;
										flag_부장2_충성도상승0 = true;
									}

									if (!pk::is_enemy(src, dst))
									{

										pk::add_loyalty(dst_leader, 忠诚度上升_0[pk::rand(3)]); // 충성도 상승 

										if ((dst.member[1] != -1) and (dst.member[2] == -1))
										{
											pk::person@ dst_deputy1 = pk::get_person(dst.member[1]);
											pk::add_loyalty(dst_deputy1, 忠诚度上升_0[pk::rand(3)]); // 충성도 상승 
										}
										else if ((dst.member[1] != -1) and (dst.member[2] != -1))
										{
											pk::person@ dst_deputy1 = pk::get_person(dst.member[1]);
											pk::person@ dst_deputy2 = pk::get_person(dst.member[2]);
											pk::add_loyalty(dst_deputy1, 忠诚度上升_0[pk::rand(3)]); // 충성도 상승 
											pk::add_loyalty(dst_deputy2, 忠诚度上升_0[pk::rand(3)]); // 충성도 상승 
										}
									}
								}

								// [통솔 80 or 90대 (and) 매력 80 or 90대]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_魅力] >= min_魅力_0 and src_leader.stat[武将能力_魅力] < min_魅力_1))
									or ((src_leader.stat[武将能力_魅力] >= min_魅力_1) and (src_leader.stat[武将能力_统率] >= min_统率_0 and src_leader.stat[武将能力_统率] < min_统率_1)))
								{

									flag_충성도상승 = true;
									flag_주장_충성도상승1 = true;

									if (pk::is_alive(src_deputy1) and !pk::is_alive(src_deputy2))
									{
										flag_부장1_충성도상승1 = true;
									}
									else if (pk::is_alive(src_deputy1) and pk::is_alive(src_deputy2))
									{
										flag_부장1_충성도상승1 = true;
										flag_부장2_충성도상승1 = true;
									}

									if (!pk::is_enemy(src, dst))
									{

										pk::add_loyalty(dst_leader, 忠诚度上升_1[pk::rand(3)]); // 충성도 상승 

										if ((dst.member[1] != -1) and (dst.member[2] == -1))
										{
											pk::person@ dst_deputy1 = pk::get_person(dst.member[1]);
											pk::add_loyalty(dst_deputy1, 忠诚度上升_1[pk::rand(3)]); // 충성도 상승 
										}
										else if ((dst.member[1] != -1) and (dst.member[2] != -1))
										{
											pk::person@ dst_deputy1 = pk::get_person(dst.member[1]);
											pk::person@ dst_deputy2 = pk::get_person(dst.member[2]);
											pk::add_loyalty(dst_deputy1, 忠诚度上升_1[pk::rand(3)]); // 충성도 상승 
											pk::add_loyalty(dst_deputy2, 忠诚度上升_1[pk::rand(3)]); // 충성도 상승 
										}
									}
								}

								// [통솔 90 이상 (and) 매력 90 이상]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_魅力] >= min_魅力_1)))
								{

									flag_충성도상승 = true;
									flag_주장_충성도상승2 = true;

									if (pk::is_alive(src_deputy1) and !pk::is_alive(src_deputy2))
									{
										flag_부장1_충성도상승2 = true;
									}
									else if (pk::is_alive(src_deputy1) and pk::is_alive(src_deputy2))
									{
										flag_부장1_충성도상승2 = true;
										flag_부장2_충성도상승2 = true;
									}

									if (!pk::is_enemy(src, dst))
									{

										pk::add_loyalty(dst_leader, 忠诚度上升_2[pk::rand(3)]); // 충성도 상승 

										if ((dst.member[1] != -1) and (dst.member[2] == -1))
										{
											pk::person@ dst_deputy1 = pk::get_person(dst.member[1]);
											pk::add_loyalty(dst_deputy1, 忠诚度上升_2[pk::rand(3)]); // 충성도 상승 
										}
										else if ((dst.member[1] != -1) and (dst.member[2] != -1))
										{
											pk::person@ dst_deputy1 = pk::get_person(dst.member[1]);
											pk::person@ dst_deputy2 = pk::get_person(dst.member[2]);
											pk::add_loyalty(dst_deputy1, 忠诚度上升_2[pk::rand(3)]); // 충성도 상승 
											pk::add_loyalty(dst_deputy2, 忠诚度上升_2[pk::rand(3)]); // 충성도 상승 
										}
									}
								}

							} // if (src.has_skill(특기_인정))


							// <기원> 특기를 소유 시 주위에 있는 아군 부대 RANDOM 효과  
							if (src.has_skill(특기_기원) and (dst !is null))
							{
								// [통솔 80대, 매력 80대]
								if (((src_leader.stat[武将能力_统率] >= min_统率_0) and (src_leader.stat[武将能力_统率] < min_统率_1))
									and ((src_leader.stat[武将能力_魅力] >= min_魅力_0) and (src_leader.stat[武将能力_魅力] < min_魅力_1)))
								{

									// 금상승 
									if (n == 0)
									{
										flag_금상승 = true;
										flag_금상승0 = true;

										if (!pk::is_enemy(src, dst))
										{
											pk::add_gold(dst, 金上升_0[pk::rand(3)], true); // 금상승 
										}
									}

									// 병량상승 
									else if (n == 1)
									{
										flag_병량상승 = true;
										flag_병량상승0 = true;

										if (!pk::is_enemy(src, dst))
										{
											pk::add_food(dst, 兵粮上升_0[pk::rand(3)], true); // 병량상승 
										}
									}

									// 기력상승 
									else if (n == 2)
									{
										flag_기력상승 = true;
										flag_기력상승0 = true;

										if (!pk::is_enemy(src, dst))
										{
											pk::add_energy(dst, 气力上升_0[pk::rand(3)], true); // 기력상승 
										}
									}

									// 상태이상 회복
									else if (n == 3)
									{
										flag_진정 = true;

										if (dst.status != 部队状态_通常 and !pk::is_enemy(src, dst))
										{
											pk::set_status(dst, src, 部队状态_通常, 2, true); // 상태이상 회복  
										}
									}

								}

								// [통솔 80 or 90대 (and) 매력 80 or 90대]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_魅力] >= min_魅力_0 and src_leader.stat[武将能力_魅力] < min_魅力_1))
									or ((src_leader.stat[武将能力_魅力] >= min_魅力_1) and (src_leader.stat[武将能力_统率] >= min_统率_0 and src_leader.stat[武将能力_统率] < min_统率_1)))
								{

									// 금상승 
									if (n == 0)
									{
										flag_금상승 = true;
										flag_금상승1 = true;

										if (!pk::is_enemy(src, dst))
										{
											pk::add_gold(dst, 金上升_1[pk::rand(3)], true); // 금상승 
										}
									}

									// 병량상승 
									else if (n == 1)
									{
										flag_병량상승 = true;
										flag_병량상승1 = true;

										if (!pk::is_enemy(src, dst))
										{
											pk::add_food(dst, 兵粮上升_1[pk::rand(3)], true); // 병량상승 
										}
									}

									// 기력상승 
									else if (n == 2)
									{
										flag_기력상승 = true;
										flag_기력상승1 = true;

										if (!pk::is_enemy(src, dst))
										{
											pk::add_energy(dst, 气力上升_1[pk::rand(3)], true); // 기력상승 
										}
									}

									// 상태이상 회복
									else if (n == 3)
									{
										flag_진정 = true;

										if (dst.status != 部队状态_通常 and !pk::is_enemy(src, dst))
										{
											pk::set_status(dst, src, 部队状态_通常, 2, true); // 상태이상 회복  
										}
									}

								}

								// [통솔 90 이상 (and) 정치 90 이상]
								else if (((src_leader.stat[武将能力_统率] >= min_统率_1) and (src_leader.stat[武将能力_魅力] >= min_魅力_1)))
								{

									// 금상승 
									if (n == 0)
									{
										flag_금상승 = true;
										flag_금상승2 = true;

										if (!pk::is_enemy(src, dst))
										{
											pk::add_gold(dst, 金上升_2[pk::rand(3)], true); // 금상승 
										}
									}

									// 병량상승 
									else if (n == 1)
									{
										flag_병량상승 = true;
										flag_병량상승2 = true;

										if (!pk::is_enemy(src, dst))
										{
											pk::add_food(dst, 兵粮上升_2[pk::rand(3)], true); // 병량상승 
										}
									}

									// 기력상승 
									else if (n == 2)
									{
										flag_기력상승 = true;
										flag_기력상승2 = true;

										if (!pk::is_enemy(src, dst))
										{
											pk::add_energy(dst, 气力上升_2[pk::rand(3)], true); // 기력상승 
										}
									}

									// 상태이상 회복
									else if (n == 3)
									{
										flag_진정 = true;

										if (dst.status != 部队状态_通常 and !pk::is_enemy(src, dst))
										{
											pk::set_status(dst, src, 部队状态_通常, 2, true); // 상태이상 회복  
										}
									}

								}

							} // if (src.has_skill(특기_기원))

						} // 效果概率

					} // for 


					if (flag_금상승)
					{
						if (flag_금상승0)
						{
							pk::add_gold(src, 金上升_0[pk::rand(3)], true); // 금상승 
						}

						if (flag_금상승1)
						{
							pk::add_gold(src, 金上升_1[pk::rand(3)], true); // 금상승 
						}

						if (flag_금상승2)
						{
							pk::add_gold(src, 金上升_2[pk::rand(3)], true); // 금상승 
						}

						if (pk::is_in_screen(pos))
						{
							pk::say(pk::encode("补充资金吧!"), src_leader);
						}

						flag_금상승 = false;
					}

					if (flag_금감소 and pk::is_in_screen(pos))
					{
						pk::say(pk::encode("夺取敌军的资金吧!"), src_leader);
						flag_금감소 = false;
					}

					if (flag_병량상승)
					{
						if (flag_병량상승0)
						{
							pk::add_food(src, 兵粮上升_0[pk::rand(3)], true); // 병량상승 
						}

						if (flag_병량상승1)
						{
							pk::add_food(src, 兵粮上升_1[pk::rand(3)], true); // 병량상승 
						}

						if (flag_병량상승2)
						{
							pk::add_food(src, 兵粮上升_2[pk::rand(3)], true); // 병량상승 
						}

						if (pk::is_in_screen(pos))
						{
							pk::say(pk::encode("补充兵粮吧!"), src_leader);
						}

						flag_병량상승 = false;
					}

					if (flag_기력상승)
					{
						if (flag_기력상승0)
						{
							pk::add_energy(src, 气力上升_0[pk::rand(3)], true); // 기력상승 
						}

						if (flag_기력상승1)
						{
							pk::add_energy(src, 气力上升_1[pk::rand(3)], true); // 기력상승 
						}

						if (flag_기력상승2)
						{
							pk::add_energy(src, 气力上升_2[pk::rand(3)], true); // 기력상승 
						}

						if (pk::is_in_screen(pos))
						{
							pk::say(pk::encode("将士们更有干劲了!"), src_leader);
						}

						flag_기력상승 = false;
					}

					if (flag_병량감소 and pk::is_in_screen(pos))
					{
						pk::say(pk::encode("夺取敌军的兵粮吧!"), src_leader);
						flag_병량감소 = false;
					}

					if (flag_내구회복 and pk::is_in_screen(pos))
					{
						pk::say(pk::encode("把建筑修起来吧!"), src_leader);
						flag_내구회복 = false;
					}

					if (flag_진정)
					{
						if (src.status != 部队状态_通常)
						{
							pk::set_status(src, src, 部队状态_通常, 2, true); // 상태이상 회복 
						}

						if (pk::is_in_screen(pos))
						{
							pk::say(pk::encode("军队平静下来了!"), src_leader);
						}

						flag_진정 = false;
					}

					if (flag_충성도상승)
					{

						if (flag_주장_충성도상승0)
						{
							pk::add_loyalty(src_leader, 忠诚度上升_0[pk::rand(3)]); // 충성도 상승 
							flag_주장_충성도상승0 = false;
						}

						if (flag_주장_충성도상승1)
						{
							pk::add_loyalty(src_leader, 忠诚度上升_1[pk::rand(3)]); // 충성도 상승 
							flag_주장_충성도상승1 = false;
						}

						if (flag_주장_충성도상승2)
						{
							pk::add_loyalty(src_leader, 忠诚度上升_2[pk::rand(3)]); // 충성도 상승 
							flag_주장_충성도상승2 = false;
						}

						if (flag_부장1_충성도상승0)
						{
							pk::add_loyalty(src_deputy1, 忠诚度上升_0[pk::rand(3)]); // 충성도 상승 
							flag_부장1_충성도상승0 = false;
						}

						if (flag_부장1_충성도상승1)
						{
							pk::add_loyalty(src_deputy1, 忠诚度上升_1[pk::rand(3)]); // 충성도 상승
							flag_부장1_충성도상승1 = false;
						}

						if (flag_부장1_충성도상승2)
						{
							pk::add_loyalty(src_deputy1, 忠诚度上升_2[pk::rand(3)]); // 충성도 상승 
							flag_부장1_충성도상승2 = false;
						}

						if (flag_부장2_충성도상승0)
						{
							pk::add_loyalty(src_deputy2, 忠诚度上升_0[pk::rand(3)]); // 충성도 상승
							flag_부장2_충성도상승0 = false;
						}

						if (flag_부장2_충성도상승1)
						{
							pk::add_loyalty(src_deputy2, 忠诚度上升_1[pk::rand(3)]); // 충성도 상승
							flag_부장2_충성도상승1 = false;
						}

						if (flag_부장2_충성도상승2)
						{
							pk::add_loyalty(src_deputy2, 忠诚度上升_2[pk::rand(3)]); // 충성도 상승
							flag_부장2_충성도상승2 = false;
						}

						if (pk::is_in_screen(pos))
						{
							pk::say(pk::encode("将士们更加忠诚了!"), src_leader);
						}

						flag_충성도상승 = false;

					}


				} // (only_AI_unit(src))

			} // for 

		} // auxiliary_effect

	} // class Main

	Main main;

} // namespace
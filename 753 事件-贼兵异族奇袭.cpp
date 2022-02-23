// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/11/16 # 江东新风 # 调整保护期，防止开局袭击 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/31 # 江东新风 # 同步honeybee大神的更新##
// ## 2020/10/27 # 江东新风 # 新特技——戍防##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@내용: 도적/이민족이 도시 근처에서 기습 출현
@기타: 난이도 상승 목적을 위해 제작한 SCRIPT

	  [도적 출현] => 치안 80 미만일 때 30% 확률로 발생
	  [이민족 출현] => 치안 80 미만일 때 30% 확률로 발생

	  190년 ~ 210년까지 女 도적 [정강, 여정] 등장 (TOTAL WAR 삼국지 참고)

*/

namespace 事件_贼兵异族奇袭
{


	//===================== CUSTOMIZE ========================

	const int cnt = 5; 				// 도적 일반 부대 생성 숫자  (기본 설정 5부대 생성) 
	const int cnt2 = 5; 			// 이민족 일반 부대 생성 숫자 (기본 설정 5부대 생성) 

	const int 발동시기 = 2;        	    // 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일,  4: 랜덤 턴 --默认2
	const int 발동확률_턴 = 30;         // 발동시기를 랜덤으로 설정했을 경우: 턴별 발동 확률--默认 30

	const int 도적발생확률 = 30; 		// 도적 발생 확률 (기본 설정 30%) 默认30
	const int 이민족발생확률 = 30; 		// 이민족 발생 확률 (기본 설정 30%)

	const int 정강여정등장확률 = 10; 		// 정강, 여정이 등장할 확률 (기본 설정 10%)

	const int 保护期 = 6;            // 게임 start 후 몇 개월 간 발동 방지
	//const int 최소도시수 = 1;       		// 설정 도시수 3개 이하 세력에게는 발동 안함 (도시 4개 ~ 6개 사이일 때 발생)
	const int 최대도시수 = 4;       		// 설정 도시수 7개 이상 세력에게는 발동 안함 (강제 종료 ERROR 방지를 위한 설정)

	const bool 플레이어만 = true;       // true = 플레이어만 적용, false = AI도 적용
	const bool 특기보정 = true;        // true = 특기 적용, false = 특기 어드밴티지 없음 (위압, 친만, 친강, 친월, 친오)

	const int safety_1 = 80; 		// 도적 발생 치안 (기본 설정 80 미만)
	const int safety_2 = 85; 		// 이민족 발생 치안 (기본 설정 85 미만)


	// 난이도에 따른 금, 군량 요구 설정        (난이도 설정 한 가지만 true로 설정해야 함)
	const bool 난이도_하 = true;	    // 금, 군량을 각각 1/3을 요구한다.
	const bool 난이도_상 = false;	    // 금, 군량을 각각 절반을 요구한다.

	const int troops_0 = 10000;		// 정강, 여정, 이민족 두목 부대 병력 설정 (최대 14000 = 정동장군 관직과 동일)
	const int troops_1 = 8000;		// 도적 부대 병력 설정 (최대 14000 = 정동장군 관직과 동일)
	const int troops_2 = 8000;		// 이민족 부대 병력 설정 (최대 14000 = 정동장군 관직과 동일)

	const int min_distance = 3;     // 부대 생성 최소 위치
	const int max_distance = 6;     // 부대 생성 최대 위치

//========================================================



	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{

			// 매턴
			if (발동시기 == 0)
			{
				bandit_Type();    // 도적
				iminzoku_Type1(); // 강족
				iminzoku_Type2(); // 오환족
				iminzoku_Type3(); // 산월족
				iminzoku_Type4(); // 남만족				
			}

			// 매월 1일
			else if (발동시기 == 1 and (pk::get_day() == 1))
			{
				bandit_Type(); // 도적
				iminzoku_Type1(); // 강족
				iminzoku_Type2(); // 오환족
				iminzoku_Type3(); // 산월족
				iminzoku_Type4(); // 남만족				
			}

			// 매분기 첫달이면서 1일	
			else if (발동시기 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
			{
				bandit_Type();    // 도적
				iminzoku_Type1(); // 강족
				iminzoku_Type2(); // 오환족
				iminzoku_Type3(); // 산월족
				iminzoku_Type4(); // 남만족 
			}

			// 매년 1월 1일
			else if (발동시기 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
			{
				bandit_Type();    // 도적
				iminzoku_Type1(); // 강족
				iminzoku_Type2(); // 오환족
				iminzoku_Type3(); // 산월족
				iminzoku_Type4(); // 남만족
			}

			// 랜덤 턴에서 발동
			else if (발동시기 == 4 and pk::rand_bool(발동확률_턴))
			{
				bandit_Type();    // 도적
				iminzoku_Type1(); // 강족
				iminzoku_Type2(); // 오환족
				iminzoku_Type3(); // 산월족
				iminzoku_Type4(); // 남만족
			}

		}


		void bandit_Type() // 일반 도적
		{

			pk::random random(pk::rand());

			auto list = pk::list_to_array(pk::get_city_list());

			for (int i = 0; i < int(list.length); i++)
			{

				pk::city@ city_ = list[i];

				pk::building@ building_ = pk::city_to_building(city_);

				pk::person@ bandit_0 = pk::get_person(무장_적장);
				pk::person@ reporter_ = pk::get_person(무장_전령);
				pk::person@ taishu_ = pk::get_person(pk::get_taishu_id(building_));

				//pk::trace(pk::format("城市id：{}，开始计算", i));
				if (ch::has_skill(building_, 特技_戍防)) continue;

				if (플레이어만 and !building_.is_player()) continue;

				if (!pk::is_player_controlled(building_)) continue;
				//pk::trace(pk::format("城市id：{}，通过前3个条件", i));
				if (특기보정 and pk::is_valid_person_id(building_.who_has_skill(특기_위압))) continue;

				if (pk::get_elapsed_months() < 保护期) continue;
				//pk::trace(pk::format("城市id：{}，通过前5个条件", i));
				//if (최소도시수 >= pk::get_city_count(pk::get_force(building_.get_force_id())) ) continue;

				if (최대도시수 <= pk::get_city_count(pk::get_force(building_.get_force_id()))) continue;

				//pk::trace(pk::format("城市id：{}，通过前6个条件", i));
				//pk::trace(pk::format("城市id：{}，治安：{}，发生概率:{}, 周围无敌军:{}", i,city_.public_order, 도적발생확률, !pk::enemies_around(building_)));
				if (city_.public_order < safety_1 and pk::rand_bool(도적발생확률) and (!pk::enemies_around(building_)))
				{
					//pk::trace(pk::format("城市id：{}，通过前7个条件", i));
					pk::message_box(pk::encode("主公, \x1b[17x盗贼\x1b[0x向这里攻过来了!"), reporter_);
					pk::message_box(pk::encode("快把城里的金银和粮食交出来!"), bandit_0);
					pk::message_box(pk::encode("不然我就把你们都杀了!"), bandit_0);

					array<string> answers =
					{
						pk::encode("将金银和粮食献上."),
						pk::encode("拒绝上交.")
					};

					int n = pk::choose(pk::encode("是否将资金和粮食交给盗贼?"), answers);

					if (n == 0)
					{
						pk::message_box(pk::encode("我会把资金和粮食献上的,请你们撤退吧!"), taishu_);
						pk::message_box(pk::encode("哈哈哈,谢谢."), bandit_0);
						pk::message_box(pk::encode("就当你运气不错!"), bandit_0);

						if (난이도_하)
						{
							pk::add_gold(city_, -city_.gold / 3, true);
							pk::add_food(city_, -city_.food / 3, true);
						}

						if (난이도_상)
						{
							pk::add_gold(city_, -city_.gold / 2, true);
							pk::add_food(city_, -city_.food / 2, true);
						}

					} // n == 0

					else if (n == 1)
					{

						pk::message_box(pk::encode("你以为这里是什么地方!"), taishu_);
						pk::message_box(pk::encode("我会教你什么是规矩!"), taishu_);
						pk::message_box(pk::encode("可恶!不要后悔!"), bandit_0);

						if (pk::rand_bool(정강여정등장확률))
						{

							// 정강
							if (pk::get_scenario().start_year > 189 and pk::get_scenario().start_year < 211)
							{

								pk::person@ bandit = pk::create_bandit(pk::get_person(554));

								pk::set_district(bandit, 세력_도적);

								bandit.sex = 성별_여;

								bandit.face = 2102;
								bandit.sei = pk::encode("吕");  // 성
								bandit.mei = pk::encode("母");  // 명
								bandit.mibun = 身份_一般;
								bandit.loyalty = 150;
								bandit.rank = 관직_정동장군;
								bandit.birth = pk::get_year() - 30;
								bandit.death = pk::min(353, bandit.birth + 99);
								bandit.appearance = pk::get_year() - 10;

								bandit.skill = 특기_연전; // 특기

								// 능력치 수정.
								bandit.base_stat[武将能力_统率] = 85;
								bandit.base_stat[武将能力_武力] = 85;
								bandit.base_stat[武将能力_智力] = 60;
								bandit.base_stat[武将能力_政治] = 60;
								bandit.base_stat[武将能力_魅力] = 60;

								bandit.tekisei[병종_창병] = 적성_S;

								// 능력치 반영.
								bandit.update();

								int gold = 1000;
								int food = int(troops_1 * 1.3f);


								// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
								pk::point emptyPos = getEmptyPos(building_);

								if (!pk::is_valid_pos(emptyPos) or !building_.is_player())
									break;

								pk::unit@ unit = pk::create_unit(building_, bandit, null, null, troops_0, 병기_창, 兵器_走舸, gold, food, emptyPos);

								// 부대 생성이 실패한 경우 무장 삭제
								if (!pk::is_alive(unit))
								{
									pk::remove(unit);
									continue;
								}

								// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
								unit.energy = 100;

								pk::set_order(unit, 部队任务_攻击, building_.get_pos());


								// 임무 실행(부대를 생성한 턴에 행동을 시작하지 않는다면 생략해도 됨.)
								if (!pk::run_order(unit))
									// 임무에 문제가 있다면 부대 삭제.
									pk::remove(unit);

							}

							// 여정
							if (pk::get_scenario().start_year > 189 and pk::get_scenario().start_year < 211)
							{

								pk::person@ bandit = pk::create_bandit(pk::get_person(554));

								pk::set_district(bandit, 세력_도적);

								bandit.sex = 성별_여;

								bandit.face = 2132;
								bandit.sei = pk::encode("迟");  // 성
								bandit.mei = pk::encode("昭平");  // 명
								bandit.mibun = 身份_一般;
								bandit.loyalty = 150;
								bandit.rank = 관직_정동장군;
								bandit.birth = pk::get_year() - 30;
								bandit.death = pk::min(353, bandit.birth + 99);
								bandit.appearance = pk::get_year() - 10;

								bandit.skill = 특기_창장; // 특기

								// 능력치 수정.
								bandit.base_stat[武将能力_统率] = 80;
								bandit.base_stat[武将能力_武力] = 80;
								bandit.base_stat[武将能力_智力] = 60;
								bandit.base_stat[武将能力_政治] = 60;
								bandit.base_stat[武将能力_魅力] = 60;

								bandit.tekisei[병종_창병] = 적성_S;

								// 능력치 반영.
								bandit.update();

								int gold = 1000;
								int food = int(troops_1 * 1.3f);


								// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
								pk::point emptyPos = getEmptyPos(building_);

								if (!pk::is_valid_pos(emptyPos) or !building_.is_player())
									break;

								pk::unit@ unit = pk::create_unit(building_, bandit, null, null, troops_0, 병기_창, 兵器_走舸, gold, food, emptyPos);

								// 부대 생성이 실패한 경우 무장 삭제
								if (!pk::is_alive(unit))
								{
									pk::remove(unit);
									continue;
								}

								// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
								unit.energy = 100;

								pk::set_order(unit, 部队任务_攻击, building_.get_pos());


								// 임무 실행(부대를 생성한 턴에 행동을 시작하지 않는다면 생략해도 됨.)
								if (!pk::run_order(unit))
									// 임무에 문제가 있다면 부대 삭제.
									pk::remove(unit);

							}

						}


						// 일반 도적 
						for (int j = 0; j < cnt; j++)
						{

							pk::person@ bandit = pk::create_bandit(pk::get_person(무장_적장));

							pk::set_district(bandit, 세력_도적);
							bandit.mibun = 身份_一般;
							bandit.loyalty = 150;
							bandit.rank = 관직_정동장군;
							bandit.birth = pk::get_year() - 30;
							bandit.death = pk::min(353, bandit.birth + 99);
							bandit.appearance = pk::get_year() - 10;

							// 능력치 수정.
							bandit.base_stat[武将能力_统率] = 70;
							bandit.base_stat[武将能力_武力] = 70;
							bandit.base_stat[武将能力_智力] = 40;
							bandit.base_stat[武将能力_政治] = 40;
							bandit.base_stat[武将能力_魅力] = 40;

							bandit.tekisei[병종_창병] = random(0, 3);

							// 능력치 반영.
							bandit.update();

							int gold = 1000;
							int food = int(troops_1 * 1.3f);


							// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
							pk::point emptyPos = getEmptyPos(building_);

							if (!pk::is_valid_pos(emptyPos) or !building_.is_player())
								break;

							pk::unit@ unit = pk::create_unit(building_, bandit, null, null, troops_1, 병기_창, 兵器_走舸, gold, food, emptyPos);

							// 부대 생성이 실패한 경우 무장 삭제
							if (!pk::is_alive(unit))
							{
								pk::remove(unit);
								continue;
							}

							// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
							unit.energy = 100;

							pk::set_order(unit, 部队任务_攻击, building_.get_pos());


							// 임무 실행(부대를 생성한 턴에 행동을 시작하지 않는다면 생략해도 됨.)
							if (!pk::run_order(unit))
								// 임무에 문제가 있다면 부대 삭제.
								pk::remove(unit);

						} // for


					} // n == 1	

				} // if

			} // for

		} // bandit_Type()


		void iminzoku_Type1() // 이민족 (강족)
		{

			pk::random random(pk::rand());

			array<pk::int> cities = { 도시_무위, 도시_안정, 도시_천수 };

			for (int i = 0; i < int(cities.length); i++)
			{

				pk::city@ city_ = pk::get_city(cities[i]);

				pk::building@ building_ = pk::city_to_building(city_);

				pk::person@ bandit_0 = pk::get_person(무장_강두목);
				pk::person@ reporter_ = pk::get_person(무장_전령);
				pk::person@ taishu_ = pk::get_person(pk::get_taishu_id(building_));

				if (플레이어만 and !building_.is_player()) continue;

				if (!pk::is_player_controlled(building_)) continue;

				if (특기보정 and pk::is_valid_person_id(building_.who_has_skill(특기_친강))) continue;

				if (pk::get_elapsed_months() < 保护期) continue;

				//if (최소도시수 >= pk::get_city_count(pk::get_force(building_.get_force_id())) ) continue;

				if (최대도시수 <= pk::get_city_count(pk::get_force(building_.get_force_id()))) continue;

				if (city_.public_order < safety_2 and pk::rand_bool(이민족발생확률) and (!pk::enemies_around(building_)))
				{

					pk::message_box(pk::encode("主公,\x1b[17x羌族\x1b[0x向这里攻过来了!"), reporter_);
					pk::message_box(pk::encode("快把城里的金银和粮食交出来!"), bandit_0);
					pk::message_box(pk::encode("不然我就把你们都杀了!"), bandit_0);

					array<string> answers =
					{
						pk::encode("将金银和粮食献上."),
						pk::encode("拒绝上交.")
					};

					int n = pk::choose(pk::encode("是否将资金和粮食交给羌族?"), answers);

					if (n == 0)
					{
						pk::message_box(pk::encode("我会把资金和粮食献上的,请你们撤退吧!"), taishu_);
						pk::message_box(pk::encode("哈哈哈,谢谢."), bandit_0);
						pk::message_box(pk::encode("就当你运气不错!"), bandit_0);

						if (난이도_하)
						{
							pk::add_gold(city_, -city_.gold / 3, true);
							pk::add_food(city_, -city_.food / 3, true);
						}

						if (난이도_상)
						{
							pk::add_gold(city_, -city_.gold / 2, true);
							pk::add_food(city_, -city_.food / 2, true);
						}

					} // n == 0

					else if (n == 1)
					{

						pk::message_box(pk::encode("你以为这里是什么地方!"), taishu_);
						pk::message_box(pk::encode("我会教你什么是规矩!"), taishu_);
						pk::message_box(pk::encode("可恶!不要后悔!"), bandit_0);

						// 강두목
						for (int j = 0; j < 1; j++)
						{

							pk::person@ bandit = pk::create_bandit(pk::get_person(무장_강두목));

							pk::set_district(bandit, 세력_강);
							bandit.mibun = 身份_一般;
							bandit.loyalty = 150;
							bandit.rank = 관직_정동장군;
							bandit.birth = pk::get_year() - 30;
							bandit.death = pk::min(353, bandit.birth + 99);
							bandit.appearance = pk::get_year() - 10;

							// 능력치 수정.
							bandit.base_stat[武将能力_统率] = 85;
							bandit.base_stat[武将能力_武力] = 85;
							bandit.base_stat[武将能力_智力] = 60;
							bandit.base_stat[武将能力_政治] = 60;
							bandit.base_stat[武将能力_魅力] = 60;

							bandit.tekisei[병종_기병] = 적성_S;

							bandit.skill = 특기_비장;

							// 능력치 반영.
							bandit.update();

							int gold = 1000;
							int food = int(troops_2 * 1.3f);


							// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
							pk::point emptyPos = getEmptyPos(building_);

							if (!pk::is_valid_pos(emptyPos) or !building_.is_player())
								break;

							pk::unit@ unit = pk::create_unit(building_, bandit, null, null, troops_0, 병기_군마, 兵器_走舸, gold, food, emptyPos);

							// 부대 생성이 실패한 경우 무장 삭제
							if (!pk::is_alive(unit))
							{
								pk::remove(unit);
								continue;
							}

							// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
							unit.energy = 100;

							pk::set_order(unit, 부대임무_정복, building_.get_pos());


							// 임무 실행(부대를 생성한 턴에 행동을 시작하지 않는다면 생략해도 됨.)
							if (!pk::run_order(unit))
								// 임무에 문제가 있다면 부대 삭제.
								pk::remove(unit);


						} // for

						// 일반 부대
						for (int k = 0; k < cnt2; k++)
						{

							pk::person@ bandit = pk::create_bandit(pk::get_person(무장_강장수));

							pk::set_district(bandit, 세력_강);
							bandit.mibun = 身份_一般;
							bandit.loyalty = 150;
							bandit.rank = 관직_정동장군;
							bandit.birth = pk::get_year() - 30;
							bandit.death = pk::min(353, bandit.birth + 99);
							bandit.appearance = pk::get_year() - 10;

							// 능력치 수정.
							bandit.base_stat[武将能力_统率] = 80;
							bandit.base_stat[武将能力_武力] = 80;
							bandit.base_stat[武将能力_智力] = 60;
							bandit.base_stat[武将能力_政治] = 60;
							bandit.base_stat[武将能力_魅力] = 60;

							bandit.tekisei[병종_기병] = 적성_S;

							bandit.skill = 특기_친강;

							// 능력치 반영.
							bandit.update();

							int gold = 1000;
							int food = int(troops_2 * 1.3f);


							// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
							pk::point emptyPos = getEmptyPos(building_);

							if (!pk::is_valid_pos(emptyPos) or !building_.is_player())
								break;

							pk::unit@ unit = pk::create_unit(building_, bandit, null, null, troops_2, 병기_군마, 兵器_走舸, gold, food, emptyPos);

							// 부대 생성이 실패한 경우 무장 삭제
							if (!pk::is_alive(unit))
							{
								pk::remove(unit);
								continue;
							}

							// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
							unit.energy = 100;

							pk::set_order(unit, 부대임무_정복, building_.get_pos());


							// 임무 실행(부대를 생성한 턴에 행동을 시작하지 않는다면 생략해도 됨.)
							if (!pk::run_order(unit))
								// 임무에 문제가 있다면 부대 삭제.
								pk::remove(unit);


						} // for

					} // n == 1	

				} // if

			} // for

		} // iminzoku_Type1()


		void iminzoku_Type2() // 이민족 (오환족)
		{

			pk::random random(pk::rand());

			array<pk::int> cities = { 도시_양평, 도시_북평, 도시_계 };

			for (int i = 0; i < int(cities.length); i++)
			{

				pk::city@ city_ = pk::get_city(cities[i]);

				pk::building@ building_ = pk::city_to_building(city_);

				pk::person@ bandit_0 = pk::get_person(무장_오환두목);
				pk::person@ reporter_ = pk::get_person(무장_전령);
				pk::person@ taishu_ = pk::get_person(pk::get_taishu_id(building_));


				if (플레이어만 and !building_.is_player()) continue;

				if (!pk::is_player_controlled(building_)) continue;

				if (특기보정 and pk::is_valid_person_id(building_.who_has_skill(특기_친오))) continue;

				if (pk::get_elapsed_months() < 保护期) continue;

				//if (최소도시수 >= pk::get_city_count(pk::get_force(building_.get_force_id())) ) continue;

				if (최대도시수 <= pk::get_city_count(pk::get_force(building_.get_force_id()))) continue;

				if (city_.public_order < safety_2 and pk::rand_bool(이민족발생확률) and (!pk::enemies_around(building_)))
				{

					pk::message_box(pk::encode("主公,\x1b[17x乌丸族\x1b[0x向这里攻过来了!"), reporter_);
					pk::message_box(pk::encode("快把城里的金银和粮食交出来!"), bandit_0);
					pk::message_box(pk::encode("不然我就把你们都杀了!"), bandit_0);

					array<string> answers =
					{
						pk::encode("将金银和粮食献上."),
						pk::encode("拒绝上交.")
					};

					int n = pk::choose(pk::encode("是否将资金和粮食交给乌丸族?"), answers);

					if (n == 0)
					{
						pk::message_box(pk::encode("我会把资金和粮食献上的,请你们撤退吧!"), taishu_);
						pk::message_box(pk::encode("哈哈哈,谢谢."), bandit_0);
						pk::message_box(pk::encode("就当你运气不错!"), bandit_0);

						if (난이도_하)
						{
							pk::add_gold(city_, -city_.gold / 3, true);
							pk::add_food(city_, -city_.food / 3, true);
						}

						if (난이도_상)
						{
							pk::add_gold(city_, -city_.gold / 2, true);
							pk::add_food(city_, -city_.food / 2, true);
						}

					} // n == 0

					else if (n == 1)
					{

						pk::message_box(pk::encode("你以为这里是什么地方!"), taishu_);
						pk::message_box(pk::encode("我会教你什么是规矩!"), taishu_);
						pk::message_box(pk::encode("可恶!不要后悔!"), bandit_0);

						// 오환 두목 
						for (int j = 0; j < 1; j++)
						{

							pk::person@ bandit = pk::create_bandit(pk::get_person(무장_오환두목));

							pk::set_district(bandit, 세력_오환);
							bandit.mibun = 身份_一般;
							bandit.loyalty = 150;
							bandit.rank = 관직_정동장군;
							bandit.birth = pk::get_year() - 30;
							bandit.death = pk::min(353, bandit.birth + 99);
							bandit.appearance = pk::get_year() - 10;

							// 능력치 수정.
							bandit.base_stat[武将能力_统率] = 85;
							bandit.base_stat[武将能力_武力] = 85;
							bandit.base_stat[武将能力_智力] = 60;
							bandit.base_stat[武将能力_政治] = 60;
							bandit.base_stat[武将能力_魅力] = 60;

							bandit.tekisei[병종_기병] = 적성_S;

							bandit.skill = 特技_疾驰;

							// 능력치 반영.
							bandit.update();

							int gold = 1000;
							int food = int(troops_2 * 1.3f);


							// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
							pk::point emptyPos = getEmptyPos(building_);

							if (!pk::is_valid_pos(emptyPos) or !building_.is_player())
								break;

							pk::unit@ unit = pk::create_unit(building_, bandit, null, null, troops_0, 병기_군마, 兵器_走舸, gold, food, emptyPos);

							// 부대 생성이 실패한 경우 무장 삭제
							if (!pk::is_alive(unit))
							{
								pk::remove(unit);
								continue;
							}

							// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
							unit.energy = 100;

							pk::set_order(unit, 부대임무_정복, building_.get_pos());


							// 임무 실행(부대를 생성한 턴에 행동을 시작하지 않는다면 생략해도 됨.)
							if (!pk::run_order(unit))
								// 임무에 문제가 있다면 부대 삭제.
								pk::remove(unit);

						} // for


						// 일반 부대 
						for (int k = 0; k < cnt2; k++)
						{

							pk::person@ bandit = pk::create_bandit(pk::get_person(무장_오환장수));

							pk::set_district(bandit, 세력_오환);
							bandit.mibun = 身份_一般;
							bandit.loyalty = 150;
							bandit.rank = 관직_정동장군;
							bandit.birth = pk::get_year() - 30;
							bandit.death = pk::min(353, bandit.birth + 99);
							bandit.appearance = pk::get_year() - 10;

							// 능력치 수정.
							bandit.base_stat[武将能力_统率] = 80;
							bandit.base_stat[武将能力_武力] = 80;
							bandit.base_stat[武将能力_智力] = 60;
							bandit.base_stat[武将能力_政治] = 60;
							bandit.base_stat[武将能力_魅力] = 60;

							bandit.tekisei[병종_기병] = 적성_S;

							bandit.skill = 특기_친오;

							// 능력치 반영.
							bandit.update();

							int gold = 1000;
							int food = int(troops_2 * 1.3f);


							// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
							pk::point emptyPos = getEmptyPos(building_);

							if (!pk::is_valid_pos(emptyPos) or !building_.is_player())
								break;

							pk::unit@ unit = pk::create_unit(building_, bandit, null, null, troops_2, 병기_군마, 兵器_走舸, gold, food, emptyPos);

							// 부대 생성이 실패한 경우 무장 삭제
							if (!pk::is_alive(unit))
							{
								pk::remove(unit);
								continue;
							}

							// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
							unit.energy = 100;

							pk::set_order(unit, 부대임무_정복, building_.get_pos());


							// 임무 실행(부대를 생성한 턴에 행동을 시작하지 않는다면 생략해도 됨.)
							if (!pk::run_order(unit))
								// 임무에 문제가 있다면 부대 삭제.
								pk::remove(unit);


						} // for

					} // n == 1

				} // if

			} // for

		} // iminzoku_Type2()


		void iminzoku_Type3() // 이민족 (산월족)
		{

			pk::random random(pk::rand());

			array<pk::int> cities = { 도시_회계, 도시_오, 도시_시상 };

			for (int i = 0; i < int(cities.length); i++)
			{

				pk::city@ city_ = pk::get_city(cities[i]);

				pk::building@ building_ = pk::city_to_building(city_);

				pk::person@ bandit_0 = pk::get_person(무장_산월두목);
				pk::person@ reporter_ = pk::get_person(무장_전령);
				pk::person@ taishu_ = pk::get_person(pk::get_taishu_id(building_));


				if (플레이어만 and !building_.is_player()) continue;

				if (!pk::is_player_controlled(building_)) continue;

				if (특기보정 and pk::is_valid_person_id(building_.who_has_skill(특기_친월))) continue;

				if (pk::get_elapsed_months() < 保护期) continue;

				//if (최소도시수 >= pk::get_city_count(pk::get_force(building_.get_force_id())) ) continue;

				if (최대도시수 <= pk::get_city_count(pk::get_force(building_.get_force_id()))) continue;


				if (city_.public_order < safety_2 and pk::rand_bool(이민족발생확률) and (!pk::enemies_around(building_)))
				{

					pk::message_box(pk::encode("主公,\x1b[17x山越族\x1b[0x向这里攻过来了!"), reporter_);
					pk::message_box(pk::encode("快把城里的金银和粮食交出来!"), bandit_0);
					pk::message_box(pk::encode("不然我就把你们都杀了!"), bandit_0);

					array<string> answers =
					{
						pk::encode("将金银和粮食献上."),
						pk::encode("拒绝上交.")
					};

					int n = pk::choose(pk::encode("是否将资金和粮食交给山越族?"), answers);

					if (n == 0)
					{
						pk::message_box(pk::encode("我会把资金和粮食献上的,请你们撤退吧!"), taishu_);
						pk::message_box(pk::encode("哈哈哈,谢谢."), bandit_0);
						pk::message_box(pk::encode("就当你运气不错!"), bandit_0);

						if (난이도_하)
						{
							pk::add_gold(city_, -city_.gold / 3, true);
							pk::add_food(city_, -city_.food / 3, true);
						}

						if (난이도_상)
						{
							pk::add_gold(city_, -city_.gold / 2, true);
							pk::add_food(city_, -city_.food / 2, true);
						}

					} // n == 0

					else if (n == 1)
					{

						pk::message_box(pk::encode("你以为这里是什么地方!"), taishu_);
						pk::message_box(pk::encode("我会教你什么是规矩!"), taishu_);
						pk::message_box(pk::encode("可恶!不要后悔!"), bandit_0);

						// 산월 두목
						for (int j = 0; j < 1; j++)
						{

							pk::person@ bandit = pk::create_bandit(pk::get_person(무장_산월두목));

							pk::set_district(bandit, 세력_산월);
							bandit.mibun = 身份_一般;
							bandit.loyalty = 150;
							bandit.rank = 관직_정동장군;
							bandit.birth = pk::get_year() - 30;
							bandit.death = pk::min(353, bandit.birth + 99);
							bandit.appearance = pk::get_year() - 10;

							// 능력치 수정.
							bandit.base_stat[武将能力_统率] = 85;
							bandit.base_stat[武将能力_武力] = 85;
							bandit.base_stat[武将能力_智力] = 60;
							bandit.base_stat[武将能力_政治] = 60;
							bandit.base_stat[武将能力_魅力] = 60;

							bandit.tekisei[병종_극병] = 적성_S;

							bandit.skill = 特技_威风;

							// 능력치 반영.
							bandit.update();

							int gold = 1000;
							int food = int(troops_2 * 1.3f);


							// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
							pk::point emptyPos = getEmptyPos(building_);

							if (!pk::is_valid_pos(emptyPos) or !building_.is_player())
								break;

							pk::unit@ unit = pk::create_unit(building_, bandit, null, null, troops_0, 병기_극, 兵器_走舸, gold, food, emptyPos);

							// 부대 생성이 실패한 경우 무장 삭제
							if (!pk::is_alive(unit))
							{
								pk::remove(unit);
								continue;
							}

							// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
							unit.energy = 100;

							pk::set_order(unit, 부대임무_정복, building_.get_pos());


							// 임무 실행(부대를 생성한 턴에 행동을 시작하지 않는다면 생략해도 됨.)
							if (!pk::run_order(unit))
								// 임무에 문제가 있다면 부대 삭제.
								pk::remove(unit);


						} // for

						// 일반 부대 
						for (int k = 0; k < cnt2; k++)
						{

							pk::person@ bandit = pk::create_bandit(pk::get_person(무장_산월장수));

							pk::set_district(bandit, 세력_산월);
							bandit.mibun = 身份_一般;
							bandit.loyalty = 150;
							bandit.rank = 관직_정동장군;
							bandit.birth = pk::get_year() - 30;
							bandit.death = pk::min(353, bandit.birth + 99);
							bandit.appearance = pk::get_year() - 10;

							// 능력치 수정.
							bandit.base_stat[武将能力_统率] = 80;
							bandit.base_stat[武将能力_武力] = 80;
							bandit.base_stat[武将能力_智力] = 60;
							bandit.base_stat[武将能力_政治] = 60;
							bandit.base_stat[武将能力_魅力] = 60;

							bandit.tekisei[병종_극병] = 적성_S;

							bandit.skill = 특기_친월;

							// 능력치 반영.
							bandit.update();

							int gold = 1000;
							int food = int(troops_2 * 1.3f);


							// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
							pk::point emptyPos = getEmptyPos(building_);

							if (!pk::is_valid_pos(emptyPos) or !building_.is_player())
								break;

							pk::unit@ unit = pk::create_unit(building_, bandit, null, null, troops_2, 병기_극, 兵器_走舸, gold, food, emptyPos);

							// 부대 생성이 실패한 경우 무장 삭제
							if (!pk::is_alive(unit))
							{
								pk::remove(unit);
								continue;
							}

							// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
							unit.energy = 100;

							pk::set_order(unit, 부대임무_정복, building_.get_pos());


							// 임무 실행(부대를 생성한 턴에 행동을 시작하지 않는다면 생략해도 됨.)
							if (!pk::run_order(unit))
								// 임무에 문제가 있다면 부대 삭제.
								pk::remove(unit);


						} // for

					} // n == 1

				} // if

			} // for

		} // iminzoku_Type3()


		void iminzoku_Type4() // 이민족 (남만족)
		{

			pk::random random(pk::rand());

			array<pk::int> cities = { 도시_운남, 도시_건녕, 도시_강주 };

			for (int i = 0; i < int(cities.length); i++)
			{

				pk::city@ city_ = pk::get_city(cities[i]);

				pk::building@ building_ = pk::city_to_building(city_);

				pk::person@ bandit_0 = pk::get_person(무장_남만두목);
				pk::person@ reporter_ = pk::get_person(무장_전령);
				pk::person@ taishu_ = pk::get_person(pk::get_taishu_id(building_));


				if (플레이어만 and !building_.is_player()) continue;

				if (!pk::is_player_controlled(building_)) continue;

				if (특기보정 and pk::is_valid_person_id(building_.who_has_skill(특기_친만))) continue;

				if (pk::get_elapsed_months() < 保护期) continue;

				//if (최소도시수 >= pk::get_city_count(pk::get_force(building_.get_force_id())) ) continue;

				if (최대도시수 <= pk::get_city_count(pk::get_force(building_.get_force_id()))) continue;

				if (city_.public_order < safety_2 and pk::rand_bool(이민족발생확률) and (!pk::enemies_around(building_)))
				{

					pk::message_box(pk::encode("主公,\x1b[17x南蛮族\x1b[0x向这里攻过来了!"), reporter_);
					pk::message_box(pk::encode("快把城里的金银和粮食交出来!"), bandit_0);
					pk::message_box(pk::encode("不然我就把你们都杀了!"), bandit_0);

					array<string> answers =
					{
						pk::encode("将金银和粮食献上."),
						pk::encode("拒绝上交.")
					};

					int n = pk::choose(pk::encode("是否将资金和粮食交给南蛮族?"), answers);

					if (n == 0)
					{
						pk::message_box(pk::encode("我会把资金和粮食献上的,请你们撤退吧!"), taishu_);
						pk::message_box(pk::encode("哈哈哈,谢谢."), bandit_0);
						pk::message_box(pk::encode("就当你运气不错!"), bandit_0);

						if (난이도_하)
						{
							pk::add_gold(city_, -city_.gold / 3, true);
							pk::add_food(city_, -city_.food / 3, true);
						}

						if (난이도_상)
						{
							pk::add_gold(city_, -city_.gold / 2, true);
							pk::add_food(city_, -city_.food / 2, true);
						}

					} // n == 0

					else if (n == 1)
					{

						pk::message_box(pk::encode("你以为这里是什么地方!"), taishu_);
						pk::message_box(pk::encode("我会教你什么是规矩!"), taishu_);
						pk::message_box(pk::encode("可恶!不要后悔!"), bandit_0);

						// 남만 두목 
						for (int j = 0; j < cnt2; j++)
						{

							pk::person@ bandit = pk::create_bandit(pk::get_person(무장_남만두목));

							pk::set_district(bandit, 세력_남만);
							bandit.mibun = 身份_一般;
							bandit.loyalty = 150;
							bandit.rank = 관직_정동장군;
							bandit.birth = pk::get_year() - 30;
							bandit.death = pk::min(353, bandit.birth + 99);
							bandit.appearance = pk::get_year() - 10;

							// 능력치 수정.
							bandit.base_stat[武将能力_统率] = 85;
							bandit.base_stat[武将能力_武力] = 85;
							bandit.base_stat[武将能力_智力] = 60;
							bandit.base_stat[武将能力_政治] = 60;
							bandit.base_stat[武将能力_魅力] = 60;

							bandit.tekisei[병종_극병] = 적성_S;

							bandit.skill = 특기_난전;

							// 능력치 반영.
							bandit.update();

							int gold = 1000;
							int food = int(troops_2 * 1.3f);


							// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
							pk::point emptyPos = getEmptyPos(building_);

							if (!pk::is_valid_pos(emptyPos) or !building_.is_player())
								break;

							pk::unit@ unit = pk::create_unit(building_, bandit, null, null, troops_0, 병기_극, 兵器_走舸, gold, food, emptyPos);

							// 부대 생성이 실패한 경우 무장 삭제
							if (!pk::is_alive(unit))
							{
								pk::remove(unit);
								continue;
							}

							// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
							unit.energy = 100;

							pk::set_order(unit, 부대임무_정복, building_.get_pos());


							// 임무 실행(부대를 생성한 턴에 행동을 시작하지 않는다면 생략해도 됨.)
							if (!pk::run_order(unit))
								// 임무에 문제가 있다면 부대 삭제.
								pk::remove(unit);


						} // for

						// 일반 부대
						for (int k = 0; k < cnt2; k++)
						{

							pk::person@ bandit = pk::create_bandit(pk::get_person(무장_남만장수));

							pk::set_district(bandit, 세력_남만);
							bandit.mibun = 身份_一般;
							bandit.loyalty = 150;
							bandit.rank = 관직_정동장군;
							bandit.birth = pk::get_year() - 30;
							bandit.death = pk::min(353, bandit.birth + 99);
							bandit.appearance = pk::get_year() - 10;

							// 능력치 수정.
							bandit.base_stat[武将能力_统率] = 80;
							bandit.base_stat[武将能力_武力] = 80;
							bandit.base_stat[武将能力_智力] = 60;
							bandit.base_stat[武将能力_政治] = 60;
							bandit.base_stat[武将能力_魅力] = 60;

							bandit.tekisei[병종_극병] = 적성_S;

							bandit.skill = 특기_친만;

							// 능력치 반영.
							bandit.update();

							int gold = 1000;
							int food = int(troops_2 * 1.3f);


							// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
							pk::point emptyPos = getEmptyPos(building_);

							if (!pk::is_valid_pos(emptyPos) or !building_.is_player())
								break;

							pk::unit@ unit = pk::create_unit(building_, bandit, null, null, troops_2, 병기_극, 兵器_走舸, gold, food, emptyPos);

							// 부대 생성이 실패한 경우 무장 삭제
							if (!pk::is_alive(unit))
							{
								pk::remove(unit);
								continue;
							}

							// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
							unit.energy = 100;

							pk::set_order(unit, 부대임무_정복, building_.get_pos());


							// 임무 실행(부대를 생성한 턴에 행동을 시작하지 않는다면 생략해도 됨.)
							if (!pk::run_order(unit))
								// 임무에 문제가 있다면 부대 삭제.
								pk::remove(unit);


						} // for

					} // n == 1	

				} // if

			} // for

		} // iminzoku_Type4()


		pk::point getEmptyPos(pk::building@ building_)
		{
			auto arr = pk::range(building_.get_pos(), min_distance, max_distance);
			for (int i = 0; i < int(arr.length); i++)
			{
				pk::hex@ hex = pk::get_hex(arr[i]);
				int terrain_id = hex.terrain;

				if (!hex.has_building and !hex.has_unit
					and pk::is_valid_terrain_id(terrain_id)
					and pk::is_enabled_terrain(terrain_id)
					and terrain_id != 지형_산)
					return arr[i];

			}
			return -1;
		}

	} // class Main

	Main main;

} // namespace
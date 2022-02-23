// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2020/07/26 ##
/*
	@제작 : masterpiecek
	@내용 : 对先前AI势力在各主要地区城市之間物資运输不暢的地方进行改进기존 AI세력의 전방지역 도시들 간에 물자 수송이 원활하게 이루어지지 않던 부분을 개선
*/

namespace AI도시_물자수송
{

	/// ============================================ < 유저 설정 > ============================================

	/// [ 수송기본조건 설정 ]

		// 도시 조작주체별 기능 적용여부 설정
	const array<bool> AI_도시물자수송_적용 = {/*플레이어 위임군단*/true, /*컴퓨터*/true };

	// 현재 시나리오의 경과월수가 기준값 미만인 경우 수송하지 않음
	const int 수송기본조건_경과월수 = 6;

	// 세력의 도시수가 기준값 미만인 경우 수송하지 않음
	const int 수송기본조건_세력도시수 = 3;

	/// -------------------------------------------------------------------------------------------------------

	/// [ 수송가능조건 설정 ] : 수송부대가 출진할 도시의 조건

		// 수송부대가 출진할 도시로부터 기준거리 이내의 도시구역에 적부대가 존재하는 경우 적부대수 및 적병사수가 기준값 이상이라면 수송하지 않음
	const array<bool> 수송가능조건_적부대조건_적용 = {/*플레이어 위임군단*/true, /*컴퓨터*/true };
	const int 수송가능조건_기준적부대거리 = 12;
	const int 수송가능조건_기준적부대수 = 2;
	const int 수송가능조건_기준적병사수 = 4000;

	// 수송부대가 출진할 도시의 최대병력 대비 현재병력의 비율(%)이 기준값 미만인 경우 제외
	const array<bool> 수송가능조건_병력조건_적용 = {/*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<int> 수송가능조건_병력조건_비율 = {/*플레이어 위임군단*/50, /*컴퓨터*/50 };

	// 수송부대가 출진할 도시의 현재병력 대비 현재병량의 비율(%)이 기준값 미만인 경우 제외
	const array<bool> 수송가능조건_병량조건_적용 = {/*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<int> 수송가능조건_병량조건_비율 = {/*플레이어 위임군단*/150, /*컴퓨터*/150 };

	/// -------------------------------------------------------------------------------------------------------

	/// [ 수송필요조건 설정 ] : 수송을 받을 도시의 조건

		// 수송을 받을 도시로부터 기준거리 이내의 도시구역에 적부대가 존재하는 경우 적부대 및 적병사수가 기준값 이상이라면 수송받지 않음
	const array<bool> 수송필요조건_적부대조건_적용 = {/*플레이어 위임군단*/true, /*컴퓨터*/true };
	const int 수송필요조건_기준적부대거리 = 12;
	const int 수송필요조건_기준적부대수 = 3;
	const int 수송필요조건_기준적병사수 = 6000;

	// 수송을 받을 도시의 최대병력 대비 현재병력의 비율이 기준값을 초과하는 경우 수송받지 않음
	const array<bool> 수송필요조건_병력조건1_적용 = {/*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<int> 수송필요조건_병력조건1_비율 = {/*플레이어 위임군단*/50, /*컴퓨터*/50 };

	// 수송부대가 출진할 도시의 현재병력 대비 수송을 받을 도시의 현재병력의 비율이 기준값을 초과하는 경우 수송받지 않음
	const array<bool> 수송필요조건_병력조건2_적용 = {/*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<int> 수송필요조건_병력조건2_비율 = {/*플레이어 위임군단*/50, /*컴퓨터*/50 };

	// 수송을 받을 도시가 역병 재해가 발생했거나 발생할 예정인 경우 수송받지 않음 (도시상태가 전방인 경우 조건 적용여부 설정)
	const array<bool> 수송필요조건_역병조건_적용 = {/*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<bool> 역병조건_전방도시_적용제외 = {/*플레이어 위임군단*/true, /*컴퓨터*/true };

	// 수송을 받을 도시가 메뚜기 재해가 발생했거나 발생할 예정인 경우 수송받지 않음 (도시상태가 전방인 경우 조건 적용여부 설정)
	const array<bool> 수송필요조건_메뚜기조건_적용 = {/*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<bool> 메뚜기조건_전방도시_적용제외 = {/*플레이어 위임군단*/true, /*컴퓨터*/true };

	/// -------------------------------------------------------------------------------------------------------

	/// [ 수송부대 관련 설정 ]

		// 수송부대의 주장 결정 시 기준 충성도 미만의 무장은 배제
	const int 수송부대_주장후보_기준충성도 = 85;

	// 수송부대 주장 결정 시 후보무장 중 운반특기 보유무장을 최우선 조건으로 설정할지 여부
	const bool 수송부대주장_운반특기_반영 = true;

	// 수송부대 주장 결정 시 후보무장 중 능력치가 높은 무장을 우선 선택함에 있어 적용할 무장능력 설정
	const int 수송부대주장_적용능력 = 武将能力_政治;

	// 수송부대가 출진할 도시의 수송항목별 수송비율(%) 설정
	const array<int> 수송비율_금 = {/*플레이어 위임군단*/50, /*컴퓨터*/50 };
	const array<int> 수송비율_병력 = {/*플레이어 위임군단*/50, /*컴퓨터*/50 };
	const array<int> 수송비율_병량 = {/*플레이어 위임군단*/50, /*컴퓨터*/50 };
	const array<int> 수송비율_일반병기 = {/*플레이어 위임군단*/50, /*컴퓨터*/50 };
	const array<int> 수송비율_공성병기 = {/*플레이어 위임군단*/50, /*컴퓨터*/50 };
	const array<int> 수송비율_함선 = {/*플레이어 위임군단*/50, /*컴퓨터*/50 };

	// 수송부대가 출진할 도시가 전방상태인 경우 수송항목별 최소유지량 설정
	const array<int> 최소유지량_전방_금 = {/*플레이어 위임군단*/2000, /*컴퓨터*/2000 };
	const array<int> 최소유지량_전방_병력 = {/*플레이어 위임군단*/20000, /*컴퓨터*/20000 };
	const array<int> 최소유지량_전방_병량 = {/*플레이어 위임군단*/80000, /*컴퓨터*/80000 };
	const array<int> 최소유지량_전방_일반병기 = {/*플레이어 위임군단*/10000, /*컴퓨터*/10000 };
	const array<int> 최소유지량_전방_공성병기 = {/*플레이어 위임군단*/3, /*컴퓨터*/3 };
	const array<int> 최소유지량_전방_함선 = {/*플레이어 위임군단*/3, /*컴퓨터*/3 };

	// 수송부대가 출진할 도시가 경계상태인 경우 수송항목별 최소유지량 설정
	const array<int> 최소유지량_경계_금 = {/*플레이어 위임군단*/2000, /*컴퓨터*/2000 };
	const array<int> 최소유지량_경계_병력 = {/*플레이어 위임군단*/10000, /*컴퓨터*/10000 };
	const array<int> 최소유지량_경계_병량 = {/*플레이어 위임군단*/40000, /*컴퓨터*/40000 };
	const array<int> 최소유지량_경계_일반병기 = {/*플레이어 위임군단*/5000, /*컴퓨터*/5000 };
	const array<int> 최소유지량_경계_공성병기 = {/*플레이어 위임군단*/0, /*컴퓨터*/0 };
	const array<int> 최소유지량_경계_함선 = {/*플레이어 위임군단*/0, /*컴퓨터*/0 };

	/// =======================================================================================================

		/// 도시상태 색인
	const int 도시상태_후방 = 0, 도시상태_경계 = 1, 도시상태_전방 = 2;

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

				// AI 스크립트 함수 체인을 위한 이전 핸들러 저장용
		pk::func261_t@ prev_callback_261;

		/**
			[ 추가 고려사항 ]

			특정 관문 및 항구가 수송부대의 이동경로에 포함되어 있는데 적거점이라면 수송에 제약을 받게 됨
			(지형의 이동가능 설정에 따라 달라짐)
		*/

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			// 261함수 이전 핸들러 저장, 함수 초기화, 함수 재정의
			@prev_callback_261 = cast<pk::func261_t@>(pk::get_func(261));
			pk::reset_func(261);
			pk::set_func(261, pk::func261_t(func261));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

				/**
				@param  force : AI 세력
				@return 자신이 처리했다면 true. 컴퓨터에게 맡기려면 false.
				*/
				/// 참고 : AI 스크립트 함수(261)를 사용하는 경우에는 플레이어 1군단이 이미 배제된 상태임
				/// 참고 : 261함수는 전체 AI 스크립트이기 때문에 수송부대를 출진시킨 후 이전 핸들러를 무조건 체인해주어야 함
		bool func261(pk::force@ force)
		{
			supplies_transport_of_AI_city(force);

			if (prev_callback_261 is null) return false;
			return prev_callback_261(force);
		}

		void supplies_transport_of_AI_city(pk::force@ force)
		{
			// 이벤트 발생을 위한 기본조건 확인 : 경과월수, 세력도시수 등
			int controller_index = get_controller_index(force);
			if (!func_수송기본조건_충족여부(force, controller_index)) return;

			for (int city_id = 0; city_id < 도시_끝; city_id++)
			{
				pk::city@ city = pk::get_city(city_id);
				if (!pk::is_alive(city)) continue;
				if (city.get_force_id() != force.get_id()) continue;

				// 기본조건 : // 플레이어 1군단 도시 제외
				if (pk::is_player_controlled(city)) continue;

				// 수송부대가 출진할 도시가 수송이 가능한지 조건 확인
				pk::building@ building = pk::city_to_building(city);
				if (!func_수송가능조건_충족여부(city, building, controller_index)) continue;

				// 수송부대가 출진할 도시의 자세력 인접도시 중 수송받을 후보도시 검색
				array<pk::city@> neighbor_city_arr = func_수송받을도시_후보검색(city, controller_index);
				if (int(neighbor_city_arr.length) == 0) continue;

				// 수송받을 후보도시 중 우선순위에 따라 결정
				neighbor_city_arr.sort(function(a, b) { return main.func_도시간_우선순위_비교(a, b); });
				pk::city@ neighbor_city = neighbor_city_arr[0];
				if (!pk::is_alive(neighbor_city)) continue;

				// 수송부대가 출진할 도시로부터 수송받을 도시로 수송부대 출진
				pk::building@ neighbor_building = pk::city_to_building(neighbor_city);
				func_수송부대_출진(city, building, neighbor_building, controller_index);
			}
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

				// 이벤트 발생을 위한 기본조건 확인
		bool func_수송기본조건_충족여부(pk::force@ force, int controller_index)
		{
			// 기본조건 : 일반게임 여부 확인 (결전제패, 튜토리얼, 미션 진행 중이 아닐 것)
			if (!is_normal_game()) return false;

			// 기본조건 : 일반세력 여부 확인 (적장세력 제외)
			if (!pk::is_normal_force(force)) return false;

			// 설정조건 : 기능적용 여부 확인
			if (!AI_도시물자수송_적용[controller_index]) return false;

			// 설정조건 : 현재 시나리오의 경과월수가 기준값 미만인 경우 수송하지 않음
			if (pk::get_elapsed_months() < 수송기본조건_경과월수) return false;

			// 설정조건 : 세력의 도시수가 기준값 미만인 경우 수송하지 않음
			if (pk::get_city_count(force) < 수송기본조건_세력도시수) return false;

			return true;
		}

		// 수송부대가 출진할 도시가 수송이 가능한지 조건 확인确认运输队触发的城市是否滿足条件
		bool func_수송가능조건_충족여부(pk::city@ city, pk::building@ building, int controller_index)
		{
			// 기본조건 : 수송부대가 출진할 도시의 도시상태가 후방(도시거리 2 이내에 적도시가 없는 경우)인 경우 제외
			int city_status = get_city_status(city);
			if (city_status == 도시상태_후방) return false;

			// 기본조건 : 도시에 행동가능한 무장이 없는 경우 제외
			if (pk::get_idle_person_list(building).count == 0) return false;

			// 기본조건 : 도시의 수송명령 실행을 위한 행동력이 충분하지 않은 경우 제외
			/// 참고 : 수송 명령의 행동력 소모량은 10
			if (!ap_is_enough(city, 10, 시설_군사부)) return false;

			// 설정조건 : 수송부대가 출진할 도시로부터 일정거리 이내의 도시구역에 일정수 이상의 적부대 및 적병력이 존재하는 경우 제외
			if (수송가능조건_적부대조건_적용[controller_index])
			{
				int enemy_units = func_도시구역_적부대수(city, 수송가능조건_기준적부대거리);
				int enemy_troops = func_도시구역_적병사수(city, 수송가능조건_기준적부대거리);
				if (enemy_units >= 수송가능조건_기준적부대수 and enemy_troops >= 수송가능조건_기준적병사수) return false;
			}

			// 설정조건 : 수송부대가 출진할 도시의 최대병력 대비 현재병력의 비율이 기준값 미만인 경우 제외
			int city_troops = int(city.troops);
			int city_max_troops = int(city.max_troops);
			if (수송가능조건_병력조건_적용[controller_index])
			{
				/// 참고 : 0이 될 수 있는 항목(병력)은 나누기 시 분모가 0이 되지 않도록 주의
				if (city_troops < city_max_troops * 수송가능조건_병력조건_비율[controller_index] / 100) return false;
			}

			// 설정조건 : 수송부대가 출진할 도시의 현재병력 대비 현재병량의 비율이 기준값 미만인 경우 제외
			int city_food = int(city.food);
			if (수송가능조건_병량조건_적용[controller_index])
			{
				/// 참고 : 0이 될 수 있는 항목(병력)은 나누기 시 분모가 0이 되지 않도록 주의
				if (city_food < city_troops * 수송가능조건_병량조건_비율[controller_index] / 100) return false;
			}

			return true;
		}

		// 在运输部队出发的城市接壤的自势力城市中搜寻可运输的候选城市수송부대가 출진할 도시의 자세력 인접도시 중 수송받을 후보도시를 검색
		array<pk::city@> func_수송받을도시_후보검색(pk::city@ city, int controller_index)
		{
			array<pk::city@> neighbor_city_arr(0);
			for (int dir_index = 0; dir_index < 6; dir_index++)
			{
				pk::city@ neighbor_city = pk::get_city(city.neighbor[dir_index]);
				if (!pk::is_alive(neighbor_city)) continue;
				if (neighbor_city.get_force_id() != city.get_force_id()) continue;

				// 수송을 받을 도시의 조건 확인
				if (!func_수송필요조건_충족여부(city, neighbor_city, controller_index)) continue;

				neighbor_city_arr.insertLast(neighbor_city);
			}

			return neighbor_city_arr;
		}

		// 수송을 받을 도시의 조건 확인
		bool func_수송필요조건_충족여부(pk::city@ city, pk::city@ neighbor_city, int controller_index)
		{
			// 기본조건 : 수송을 받을 도시의 도시상태가 후방(도시거리 2 이내에 적도시가 없는 경우)인 경우 제외
			int neighbor_city_status = get_city_status(neighbor_city);
			if (neighbor_city_status == 도시상태_후방) return false;

			// 기본조건 : 수송부대가 출진할 도시의 도시상태가 경계, 수송받을 도시의 도시상태가 전방인 경우 제외
			int city_status = get_city_status(city);
			if (city_status == 도시상태_경계 and neighbor_city_status == 도시상태_전방) return false;

			// 기본조건 : 수송을 받을 도시로 향하고 있는 수송부대가 이미 존재하는 경우 제외
			if (func_특정도시목표_출진수송부대_존재여부(neighbor_city)) return false;

			// 설정조건 : 수송을 받을 도시로부터 일정거리 이내의 도시구역에 일정수 이상의 적부대 및 적병력이 존재하는 경우 제외
			if (수송필요조건_적부대조건_적용[controller_index])
			{
				int enemy_units = func_도시구역_적부대수(neighbor_city, 수송필요조건_기준적부대거리);
				int enemy_troops = func_도시구역_적병사수(neighbor_city, 수송필요조건_기준적부대거리);
				if (enemy_units >= 수송필요조건_기준적부대수 and enemy_troops >= 수송필요조건_기준적병사수) return false;
			}

			// 설정조건 : 수송을 받을 도시의 최대병력 대비 현재병력의 비율이 기준값을 초과하는 경우 제외
			int neighbor_city_troops = int(neighbor_city.troops);
			int neighbor_city_max_troops = int(neighbor_city.max_troops);
			if (수송필요조건_병력조건1_적용[controller_index])
			{
				/// 참고 : 0이 될 수 있는 항목(병력)은 나누기 시 분모가 0이 되지 않도록 주의
				if (neighbor_city_troops > neighbor_city_max_troops * 수송필요조건_병력조건1_비율[controller_index] / 100) return false;
			}

			// 설정조건 : 수송부대가 출진할 도시의 현재병력 대비 수송을 받을 도시의 현재병력의 비율이 기준값을 초과하는 경우 제외
			int city_troops = int(city.troops);
			if (수송필요조건_병력조건2_적용[controller_index])
			{
				/// 참고 : 0이 될 수 있는 항목(병력)은 나누기 시 분모가 0이 되지 않도록 주의
				if (neighbor_city_troops > city_troops * 수송필요조건_병력조건2_비율[controller_index] / 100) return false;
			}

			// 설정조건 : 수송을 받을 도시가 역병 재해가 발생했거나 발생할 예정인 경우 제외
			if (수송필요조건_역병조건_적용[controller_index])
			{
				// 도시상태가 전방인 경우 적용여부 확인
				if (neighbor_city_status == 도시상태_경계 or (!역병조건_전방도시_적용제외[controller_index] and neighbor_city_status == 도시상태_전방))
				{
					// 역병 재해가 발생한 경우
					if (neighbor_city.ekibyou) return false;

					// 역병 재해가 발생할 예정인 경우
					/// 참고 : 재해가 발생예정인 경우 다음 분기(계절)가 되면 발생함
					if (neighbor_city.next_ekibyou and next_turn_is_next_quarter()) return false;
				}
			}

			// 설정조건 : 수송을 받을 도시가 메뚜기 재해가 발생했거나 발생할 예정인 경우 제외
			if (수송필요조건_메뚜기조건_적용[controller_index])
			{
				// 도시상태가 전방인 경우 적용여부 확인
				if (neighbor_city_status == 도시상태_경계 or (!메뚜기조건_전방도시_적용제외[controller_index] and neighbor_city_status == 도시상태_전방))
				{
					// 메뚜기 재해가 발생한 경우
					if (neighbor_city.inago) return false;

					// 메뚜기 재해가 발생할 예정인 경우
					/// 참고 : 재해가 발생예정인 경우 다음 분기(계절)가 되면 발생함
					if (neighbor_city.next_inago and next_turn_is_next_quarter()) return false;
				}
			}

			// 도시상태가 전방으로 같은 경우
			if (city_status == 도시상태_전방 and neighbor_city_status == 도시상태_전방)
			{
				// 기본조건 : 인접 적도시수가 수송부대가 출진할 도시가 수송받을 도시의 일정배수를 초과하는 경우 제외
				if (func_도시주변_적도시수(city, 1) > func_도시주변_적도시수(neighbor_city, 1) * 2) return false;

				// 기본조건 : 인접 적도시 병사수가 수송부대가 출진할 도시가 수송받을 도시의 일정배수를 초과하는 경우 제외
				else if (func_인접적도시_병사수(city) > func_인접적도시_병사수(neighbor_city) * 2) return false;
			}

			// 도시상태가 경계로 같은 경우 : 도시거리별 적도시수 비교
			if (city_status == 도시상태_경계 and neighbor_city_status == 도시상태_경계)
			{
				for (int distance = 2; distance <= 8; distance++)
				{
					// 기본조건 : 도시거리별로 도시의 주변 적도시수가 수송부대가 출진할 도시가 수송받을 도시의 일정배수를 초과하는 경우 제외
					if (func_도시주변_적도시수(city, distance) > func_도시주변_적도시수(neighbor_city, distance) * 2) return false;
				}
			}

			return true;
		}

		/**
		@return 'city_a'가 'city_b'보다 전력보강 우선순위가 높은지 여부
		*/
		// 수송받을 도시 간 전력보강 우선순위 비교
		bool func_도시간_우선순위_비교(pk::city@ city_a, pk::city@ city_b)
		{
			// 도시상태 : 경계상태보다 전방상태를 우선
			int a_city_status = get_city_status(city_a);
			int b_city_status = get_city_status(city_b);
			if (a_city_status != b_city_status) return a_city_status > b_city_status;

			// 도시상태가 전방으로 같은 경우
			if (a_city_status == 도시상태_전방 and b_city_status == 도시상태_전방)
			{
				// 인접 적도시수 : 많을수록 우선
				int a_around_enenmy_cities = func_도시주변_적도시수(city_a, 1);
				int b_around_enenmy_cities = func_도시주변_적도시수(city_b, 1);
				if (a_around_enenmy_cities != b_around_enenmy_cities)
					return a_around_enenmy_cities > b_around_enenmy_cities;

				// 도시구역 내의 거점(관문 및 항구) 중 적거점의 병력 : 많을수록 우선
				int a_area_enemy_base_troops = func_도시구역_적거점_병력(city_a);
				int b_area_enemy_base_troops = func_도시구역_적거점_병력(city_b);
				if (a_area_enemy_base_troops != b_area_enemy_base_troops)
					return a_area_enemy_base_troops > b_area_enemy_base_troops;

				// 인접 적도시의 병사수 : 많을수록 우선
				int a_neighbor_enemy_city_troops = func_인접적도시_병사수(city_a);
				int b_neighbor_enemy_city_troops = func_인접적도시_병사수(city_b);
				if (a_neighbor_enemy_city_troops != b_neighbor_enemy_city_troops)
					return a_neighbor_enemy_city_troops > b_neighbor_enemy_city_troops;
			}

			// 도시상태가 경계로 같은 경우
			if (a_city_status == 도시상태_경계 and b_city_status == 도시상태_경계)
			{
				// 도시주변 적도시수 : 많을수록 우선 (도시거리별 판단, 도시거리 2부터 비교)
				for (int distance = 2; distance <= 8; distance++)
				{
					int a_around_enenmy_cities = func_도시주변_적도시수(city_a, distance);
					int b_around_enenmy_cities = func_도시주변_적도시수(city_b, distance);
					if (a_around_enenmy_cities != b_around_enenmy_cities)
						return a_around_enenmy_cities > b_around_enenmy_cities;
				}
			}

			// 도시병력 : 적을수록 우선
			int a_troops = city_a.troops;
			int b_troops = city_b.troops;
			return a_troops < b_troops;
		}

		// 수송부대 출진
		/// 참고 : cmd_info 구조체를 활용해 AI의 거점명령 실행 시 해당명령의 행동력은 자동 차감됨 (즉, 군단의 행동력이 부족하면 실행하지 않음)
		void func_수송부대_출진(pk::city@ city, pk::building@ building, pk::building@ neighbor_building, int controller_index)
		{
			// 컴퓨터 출진명령 생성
			pk::com_deploy_cmd_info cmd;

			// 출진거점
			@cmd.base = @building;

			// 출진부대 종류
			cmd.type = 부대종류_수송;

			// 출진부대 주장
			cmd.member[0] = get_unit_leader_id(building);

			// 출진부대 병력
			int city_status = get_city_status(city);
			cmd.troops = get_troops_transport_volume(building, neighbor_building, city_status, controller_index);

			// 출진부대 금
			cmd.gold = get_gold_transport_volume(building, neighbor_building, city_status, controller_index);

			// 출진부대 병량
			cmd.food = get_food_transport_volume(building, neighbor_building, city_status, controller_index);

			// 출진부대 병기
			for (int weapon_id = 0; weapon_id < 병기_끝; weapon_id++)
			{
				cmd.weapon_id[weapon_id] = weapon_id;
				cmd.weapon_amount[weapon_id] = get_wpn_transport_volume(building, neighbor_building, weapon_id, city_status, controller_index);
			}

			// 출진부대 임무
			cmd.order = 部队任务_移动;

			// 출진부대 대상좌표
			cmd.target_pos = neighbor_building.pos;

			// 출진명령 실행
			int unit_id = pk::command(cmd);

			// 출진부대 행동완료 처리
			/// 참고 : cmd_info 구조체를 활용해 AI의 출진명령 실행 시 출진한 부대의 행동완료 설정 필수
			if (unit_id != -1) pk::get_unit(unit_id).action_done = true;
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

				// 일반게임여부 반환 : 결전제패, 튜토리얼, 미션 진행 중이 아닐 것
		bool is_normal_game()
		{
			return (!pk::is_campaign() and !pk::is_mission_running());
		}

		// 세력의 조작주체 색인 반환
		int get_controller_index(pk::force@ force)
		{
			return (force.is_player() ? /*플레이어 위임군단*/0 : /*컴퓨터*/1);
		}

		/**
			[ 참고 ] 도시상태 : 거리는 도시 간 무장의 이동 소요 턴수(소요일수 10일) 기준

			1. 후방 : 도시거리 2 내에 적세력의 도시가 없는 경우 (도시거리 3 부터 적세력의 도시가 존재)
			2. 경계 : 도시거리 1 내에 적세력의 도시가 없는 경우 (도시거리 2 부터 적세력의 도시가 존재)
			3. 전방 : 도시거리 1 내에 적세력의 도시가 있는 경우
		*/
		// 도시의 도시상태 반환
		int get_city_status(pk::city@ city)
		{
			if (func_도시주변_적도시수(city, /*distance*/2) == 0) return 도시상태_후방;
			else if (func_도시주변_적도시수(city, /*distance*/1) == 0) return 도시상태_경계;
			else                                                       return 도시상태_전방;
		}

		// 명령실행을 위한 행동력 충분여부 확인
		/// 참고 : 도시의 내정시설 중 군사부 존재 시 군사메뉴의 모든 명령의 행동력 소모가 반감
		bool ap_is_enough(pk::city@ city, int needed_ap, int buff_facility_id)
		{
			if (pk::has_facility(city, buff_facility_id)) needed_ap /= 2;
			pk::district@ district = pk::get_district(city.get_district_id());
			return (int(district.ap) >= needed_ap);
		}

		// 다음 턴이 다음 분기(계절)인지 확인
		/// 참고 : 계절의 시작 날짜는 1/1(봄), 4/1(여름), 7/1(가을), 10/1(겨울), 따라서 각각의 직전 턴은 12/21, 3/21, 6/21, 9/21이 됨
		bool next_turn_is_next_quarter()
		{
			return (pk::get_month() % 3 == 0 and pk::get_day() == 21);
		}

		// 도시구역 중 도시로부터 일정거리(좌표거리) 이내의 적부대수 반환
		int func_도시구역_적부대수(pk::city@ city, int distance)
		{
			int enemy_units = 0;
			array<pk::point> range_arr = pk::range(city.get_pos(), 1, distance);
			for (int pos_index = 0; pos_index < int(range_arr.length); pos_index++)
			{
				pk::point range_pos = range_arr[pos_index];
				int area_city_id = pk::get_city_id(range_pos);
				if (city.get_id() != area_city_id) continue;

				pk::unit@ unit = pk::get_unit(range_pos);
				if (!pk::is_alive(unit)) continue;
				if (!pk::is_enemy(city, unit)) continue;

				enemy_units++;
			}

			return enemy_units;
		}

		// 도시구역 중 도시로부터 일정거리(좌표거리) 이내의 적병사수 반환
		int func_도시구역_적병사수(pk::city@ city, int distance)
		{
			int enemy_troops = 0;
			array<pk::point> range_arr = pk::range(city.get_pos(), 1, distance);
			for (int pos_index = 0; pos_index < int(range_arr.length); pos_index++)
			{
				pk::point range_pos = range_arr[pos_index];
				int area_city_id = pk::get_city_id(range_pos);
				if (city.get_id() != area_city_id) continue;

				pk::unit@ unit = pk::get_unit(range_pos);
				if (!pk::is_alive(unit)) continue;
				if (!pk::is_enemy(city, unit)) continue;

				enemy_troops += unit.troops;
			}

			return enemy_troops;
		}

		// 특정 도시를 수송 목표로 하고 있는 도시 세력의 수송부대가 존재하는지 여부
		bool func_특정도시목표_출진수송부대_존재여부(pk::city@ city)
		{
			for (int unit_id = 0; unit_id < 部队_末; unit_id++)
			{
				pk::unit@ unit = pk::get_unit(unit_id);
				if (!pk::is_alive(unit)) continue;

				// 부대의 종류가 수송부대가 아닌 경우 제외
				if (unit.type != 부대종류_수송) continue;

				// 부대의 임무가 이동이 아닌 경우 제외
				if (unit.order != 部队任务_移动) continue;

				// 부대의 세력이 기준도시의 세력과 동일하지 않은 경우 제외
				if (unit.get_force_id() != city.get_force_id()) continue;

				// 부대의 목표좌표가 기준도시의 좌표가 아닌 경우 제외
				if (unit.target_pos != city.get_pos()) continue;

				return true;
			}

			return false;
		}

		// 도시로부터 일정거리(도시거리) 이내의 적도시수 반환
		/// 참고 : 도시거리는 무장의 도시 간 이동 턴수(소요일수 10일) 기준
		int func_도시주변_적도시수(pk::city@ city, int distance)
		{
			int enemy_cities = 0;
			for (int city_id = 0; city_id < 도시_끝; city_id++)
			{
				pk::city@ enemy_city = pk::get_city(city_id);
				if (!pk::is_alive(enemy_city)) continue;

				// 검색기준 도시 제외
				if (city.get_id() == city_id) continue;

				// 검색기준 도시와 확인대상 도시가 적이 아닌 경우 제외
				if (!pk::is_enemy(city, enemy_city)) continue;

				// 도시 간 거리가 일정거리를 초과하는 경우 제외
				int city_distance = pk::get_city_distance(city.get_id(), city_id);
				if (city_distance > distance) continue;

				enemy_cities++;
			}

			return enemy_cities;
		}

		// 인접 적도시의 병사수 확인
		/// 참고 : 도시거리가 2(인접하지 않은 도시)이지만 실제로는 거리가 가까워서 공격을 올 수 있는 거리의 도시도 존재 (허창 - 신야)
		/// 참고 : 도시거리가 1(인접한 도시)이지만 실제로는 거리가 멀어서 공격을 오기 힘든 거리의 도시도 존재 (시상 - 건업)
		int func_인접적도시_병사수(pk::city@ city)
		{
			int enemy_troops = 0;
			for (int dir_index = 0; dir_index < 6; dir_index++)
			{
				pk::city@ neighbor_city = pk::get_city(city.neighbor[dir_index]);
				if (!pk::is_alive(neighbor_city)) continue;
				if (!pk::is_enemy(city, neighbor_city)) continue;

				enemy_troops += int(neighbor_city.troops);
			}

			return enemy_troops;
		}

		/**
			[ 참고 ] 도시별 관문 및 항구와의 좌표거리 (최단거리 기준)

			 1. 양평 : 안평항(15)
			 2. 진양 :   호관(18), 서하항(28)
			 3.   업 : 백마항(10)
			 4. 평원 : 고당항( 5)
			 5. 북해 : 임제항(15), 창양항(16)
			 6. 하비 : 해릉항(10), 강도항(19)
			 7. 복양 : 돈구항( 6)
			 8. 진류 : 관도항(10)
			 9. 낙양 : 맹진항( 6), 호로관(11), 해현항(18)
			10. 장안 :   동관( 8), 신풍항(11), 함곡관(15),   무관(20), 하양항(21)
			11. 한중 : 양평관( 6)
			12. 자동 : 가맹관(11),   검각(14)
			13. 성도 : 면죽관( 8), 부수관(13)
			14. 영안 : 무현항( 8)
			15. 상용 : 방릉항( 6)
			16. 신야 : 호양항( 6)
			17. 양양 : 중려항(11)
			18. 강릉 : 강진항( 9), 한진항(15), 오림항(17)
			19. 무릉 : 동정항(10), 공안항(12)
			20. 장사 : 나현항(20)
			21. 강하 : 하구항( 7)
			22. 시상 : 구강항( 7), 육구항(10), 파양항(20), 노릉항(40)
			23. 수춘 : 유수항(18)
			24. 여강 : 환구항( 6)
			25. 건업 : 무호항( 6), 호림항(19)
			26.   오 : 곡아항(10)
			27. 회계 : 구장항( 8)
		*/
		// 도시구역 내의 거점(관문 및 항구) 중 적거점의 병력 확인
		/// 참고 : 도시구역의 거점이지만 거리가 너무 멀거나 전략적 요충지가 아닌 거점은 제외함
		int func_도시구역_적거점_병력(pk::city@ city)
		{
			int enemy_troops = 0;
			for (int i = 0; i < 5; i++)
			{
				int building_id = city.gate[i];
				if (building_id == -1) continue;

				// 도시구역의 거점이지만 거리가 너무 멀거나 전략적 요충지가 아닌 거점 확인
				if (func_제외대상_도시구역거점_해당여부(city, building_id)) continue;

				pk::building@ building = pk::get_building(building_id);
				if (!pk::is_enemy(city, building)) continue;

				int building_troops = pk::get_troops(building);
				enemy_troops += building_troops;
			}

			return enemy_troops;
		}

		// 도시구역의 거점이지만 거리가 너무 멀거나 전략적 요충지가 아닌 거점 확인
		bool func_제외대상_도시구역거점_해당여부(pk::city@ city, int building_id)
		{
			int city_id = city.get_id();
			if (city_id == 건물_진양 and building_id == 건물_서하항) return true;
			if (city_id == 건물_하비 and building_id == 건물_강도항) return true;
			if (city_id == 건물_낙양 and building_id == 건물_해현항) return true;
			if (city_id == 건물_장안 and building_id == 건물_하양항) return true;
			if (city_id == 건물_시상 and building_id == 건물_파양항) return true;
			if (city_id == 건물_시상 and building_id == 건물_노릉항) return true;

			return false;
		}

		// 수송부대 주장 결정 (주장후보 무장 정렬)
		int get_unit_leader_id(pk::building@ building)
		{
			array<pk::person@> person_arr = pk::list_to_array(pk::get_idle_person_list(building));
			person_arr.sort(function(a, b)
			{
				// 우선순위1 - 무장의 특기 : 운반특기를 보유하고 있는 무장을 우선
				if (수송부대주장_운반특기_반영)
				{
					bool a_has_skill = ch::has_skill(a, 특기_운반);
					bool b_has_skill = ch::has_skill(b, 특기_운반);
					if (a_has_skill and !b_has_skill) return true;
					if (!a_has_skill and b_has_skill) return false;
				}

				// 우선순위2 - 무장의 특정 무장능력 : 높을수록 우선
				int a_stat = a.stat[수송부대주장_적용능력];
				int b_stat = b.stat[수송부대주장_적용능력];
				if (a_stat != b_stat) return a_stat > b_stat;

				// 우선순위3 - 무장의 의리 : 높을수록 우선
				int a_giri = a.giri;
				int b_giri = b.giri;
				return a_giri > b_giri;
			});

			// 설정조건 : 수송부대 주장후보 무장 중 충성도가 기준값 이상인 무장만을 선택
			int leader_id = -1;
			for (int person_index = 0; person_index < int(person_arr.length); person_index++)
			{
				pk::person@ leader = person_arr[person_index];
				if (leader.loyalty < 수송부대_주장후보_기준충성도) continue;

				leader_id = leader.get_id();

				/// 참고 : 주장 결정 후 for 구문을 종료하지 않으면 충성도 기준을 충족하는 후순위 무장이 결정됨
				break;
			}

			return leader_id;
		}

		// 금 수송량 결정
		int get_gold_transport_volume(pk::building@ building, pk::building@ neighbor_building, int city_status, int controller_index)
		{
			// 기본조건 : 기존 수송부대 금 최대수송량
			int gold = 100000;

			// 기본조건 : 수송받을 도시의 금 재고 확인 (재고 최대치까지의 여유 확인)
			gold = pk::min(gold, pk::get_max_gold(neighbor_building) - pk::get_gold(neighbor_building));

			// 설정조건 : 수송부대가 출진할 도시의 금 재고 최소유지량 확인
			array<int> 최소유지량_금 = (city_status == 도시상태_전방) ? 최소유지량_전방_금 : 최소유지량_경계_금;
			gold = pk::min(gold, pk::max(0, pk::get_gold(building) - 최소유지량_금[controller_index]));

			// 설정조건 : 수송부대가 출진할 도시의 금 재고 중 수송할 비율 확인
			gold = pk::min(gold, pk::get_gold(building) * 수송비율_금[controller_index] / 100);

			return gold;
		}

		// 병량 수송량 결정
		int get_food_transport_volume(pk::building@ building, pk::building@ neighbor_building, int city_status, int controller_index)
		{
			// 기본조건 : 기존 수송부대 병량 최대수송량
			int food = 500000;

			// 기본조건 : 수송받을 도시의 병량 재고 확인 (재고 최대치까지의 여유 확인)
			food = pk::min(food, pk::get_max_food(neighbor_building) - pk::get_food(neighbor_building));

			// 설정조건 : 수송부대가 출진할 도시의 병량 재고 최소유지량 확인
			array<int> 최소유지량_병량 = (city_status == 도시상태_전방) ? 최소유지량_전방_병량 : 최소유지량_경계_병량;
			food = pk::min(food, pk::max(0, pk::get_food(building) - 최소유지량_병량[controller_index]));

			// 설정조건 : 수송부대가 출진할 도시의 병량 재고 중 수송할 비율 확인
			food = pk::min(food, pk::get_food(building) * 수송비율_병량[controller_index] / 100);

			return food;
		}

		// 병력 수송량 결정
		int get_troops_transport_volume(pk::building@ building, pk::building@ neighbor_building, int city_status, int controller_index)
		{
			// 기본조건 : 기존 수송부대 병력 최대수송량
			int troops = 60000;

			// 기본조건 : 수송받을 도시의 병력 재고 확인 (재고 최대치까지의 여유 확인)
			troops = pk::min(troops, pk::get_max_troops(neighbor_building) - pk::get_troops(neighbor_building));

			// 설정조건 : 수송부대가 출진할 도시의 병력 재고 최소유지량 확인
			array<int> 최소유지량_병력 = (city_status == 도시상태_전방) ? 최소유지량_전방_병력 : 최소유지량_경계_병력;
			troops = pk::min(troops, pk::max(0, pk::get_troops(building) - 최소유지량_병력[controller_index]));

			// 설정조건 : 수송부대가 출진할 도시의 병력 재고 중 수송할 비율 확인
			troops = pk::min(troops, pk::get_troops(building) * 수송비율_병력[controller_index] / 100);

			return troops;
		}

		// 병기 수송량 결정
		int get_wpn_transport_volume(pk::building@ building, pk::building@ neighbor_building, int weapon_id, int city_status, int controller_index)
		{
			// 기본조건 : 기존 수송부대 병기 최대수송량
			int wpn_amount = (weapon_id < 병기_충차) ? 100000 : 100;

			// 기본조건 : 수송받을 도시의 병기 재고 확인 (재고 최대치까지의 여유 확인)
			wpn_amount = pk::min(wpn_amount, pk::get_max_weapon_amount(neighbor_building, weapon_id) - pk::get_weapon_amount(neighbor_building, weapon_id));

			// 설정조건 : 수송부대가 출진할 도시의 병기 재고 최소유지량 확인
			array<int> 최소유지량_병기(0);
			if (weapon_id < 병기_충차) 최소유지량_병기 = (city_status == 도시상태_전방) ? 최소유지량_전방_일반병기 : 최소유지량_경계_일반병기;
			else if (weapon_id < 兵器_走舸) 최소유지량_병기 = (city_status == 도시상태_전방) ? 최소유지량_전방_공성병기 : 최소유지량_경계_공성병기;
			else                            최소유지량_병기 = (city_status == 도시상태_전방) ? 최소유지량_전방_함선 : 최소유지량_경계_함선;
			wpn_amount = pk::min(wpn_amount, pk::max(0, pk::get_weapon_amount(building, weapon_id) - 최소유지량_병기[controller_index]));

			// 설정조건 : 수송부대가 출진할 도시의 병기 재고 중 수송할 비율 확인
			int 수송비율_병기 = 0;
			if (weapon_id < 병기_충차) 수송비율_병기 = 수송비율_일반병기[controller_index];
			else if (weapon_id < 兵器_走舸) 수송비율_병기 = 수송비율_공성병기[controller_index];
			else                            수송비율_병기 = 수송비율_함선[controller_index];
			wpn_amount = pk::min(wpn_amount, pk::get_weapon_amount(building, weapon_id) * 수송비율_병기 / 100);

			return wpn_amount;
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	}

	Main main;
}
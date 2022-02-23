// ## 2021/01/14 # 江东新风 # 修复优化失效bug ##
// ## 2020/11/18 # 江东新风 # 兵种优化时排除建设部队 ##
// ## 2020/09/23 # 江东新风 #修复触发特技优化出征兵种时，语句显示不正确的问题##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: 부대의 주장의 능력에 따른 출진병종 최적화 설정

@수정자: masterpiecek님
@수정사항: Modified Code는 "/// 东荻 吉遭" 주석 표기를 참고하시기 바랍니다.
(1) 거점에 병기가 가득 차 있는 경우 '변경 전' 병기의 반환이 제대로 되지 않는 문제 해결
	(부대좌표이동 트리거에서 출진한 것을 확인한 다음에 '변경 전' 병기를 반환)
(2) 거점에서 부대를 출진하려다가 취소하는 경우에도 '변경 후' 병기가 차감되어 버리는 문제 해결
	(부대좌표이동 트리거에서 출진한 것을 확인한 다음에 '변경 후' 병기를 차감)


※ 초심자 유저 분들이 쉽게 CUSTOMIZING 할 수 있도록 가중치 등 복잡한 요소를 배제하고 단순하게 CODING 작업한 SCRIPT


※ 해당 SCRIPT 사용자 분들은 <호위장군조운>님의 [출진부대최적화] SCRIPT를 함께 사용하지 않도록 주의하시기 바랍니다.

===> <호위장군조운>님의 [출진부대최적화] SCRIPT를 사용하시는 분들은 536번째 line에 적성을 반영하는 함수에
===> pk::get_tekisei(unit); 로 되어 있는 부분이 pk::get_tekisei(unit, weapon); 로
===> 수정되어야 무기에 따른 적성이 제대로 반영됩니다. (by masterpiecek님)


※ <기마책사>님의 [출진무장최적화] SCRIPT는 함께 사용해도 상관없습니다.

※ 저의 SCRIPT에서 특기 배분은 다음과 같은 요소를 감안하여 설정한 것입니다.
  (1) 병종과 관련된 특기를 최우선적으로 감안
  (2) 병기 우선순위 [창병(나선첨, 혼란 활용), 극병(방어력, 다수공격), 기병(공격력), 노병(화시, 간접공격(범용성)) > 간접병기(공성전) > 직접병기(공성전)]
  (3) 대표적인 특기 보유 무장의 병종적성 감안
  (4) 설정된 특기가 없다면 무장의 병종적성에서 가장 적성이 좋은 병종으로 출진

  저는 개인적으로 이와 같은 우선순위를 감안하여 특기를 각 병종에 MATCHING 설정하였습니다.


*/

namespace AI_出征兵种优化
{


	// ============================================== CUSTOMIZE =========================================================


		///    ※ 효과_AI만은 TEST를 위해서 설정한 CUSTOMIZE이므로, true로 설정하기를 권장 		
	const bool 효과_AI만 = true;  // true =AI만 효과적용, false =유저와 AI 모두 효과 적용 


	// 병장기_재고 = 0으로 설정 시 <병장기>가 무제한으로 병종 최적화 출진
	const int 병장기_재고 = 5000; 	// 병종 최적화 가능한 병장기 재고량 (기본 5000 이상이 되어야 병종 최적화 출진)


	// 병기_재고 = 0으로 설정 시 <병기>가 무제한으로 병종 최적화 출진
	const int 병기_재고 = 1; 		// 병종 최적화 가능한 병기 재고량 (기본 1 이상이 되어야 병종 최적화 출진)


// ======================================================================================================================

	const int 出征兵种优化_触发器_优先顺序 = 99; // 값이 높을수록 먼저 실행 (출진부대최적화.cpp 크고 출진무장최적화.cpp 보다 낮게 설정 필요)

	bool 开启 = true;
	class Main
	{
		/// 추가 내용
		int unit_id;
		int wpn_id_add, wpn_amount_add;
		int wpn_id_subtract, wpn_amount_subtract;

		Main()
		{
			pk::bind(170, 出征兵种优化_触发器_优先顺序, pk::trigger170_t(onUnitCreate));

			/// 추가 내용
			pk::bind(172, 出征兵种优化_触发器_优先顺序, pk::trigger172_t(RestoreUnitWeapon));

			// 강제종료 ERROR 방지 TRIGGER
			pk::bind(106, pk::trigger106_t(Load_Game));
			pk::bind(112, pk::trigger112_t(onTurnEnd));
			pk::bind(111, pk::trigger111_t(onTurnStart));
		}

		// 유저 또는 AI만 적용 시 판단함수
		bool only_AI_unit(pk::unit@ unit)
		{
			if (효과_AI만 and unit.is_player()) return false;
			return true;
		}

		void onUnitCreate(pk::unit@ src, int type)
		{
			if (!开启) return;
			pk::person@ src_leader = pk::get_person(src.leader);
			pk::building@ building_ = pk::get_building(pk::get_service(src));
			if (!pk::is_alive(building_)) return;

			int weapon_id = pk::get_ground_weapon_id(src);
			int best_weapon_id = getBestWeaponId(src_leader);
			string weapon_name = get_weapon_name(best_weapon_id);

			// 이민족, 도적 제외
			if (!pk::is_normal_force(src.get_force_id())) return;

			// 수송 부대 제외
			if (src.type != 부대종류_전투) return;

			// 取消对建设部队的优化
			if (src.order == 部队任务_设置) return;

			// 부대, 건물 존재 여부
			if (!pk::is_alive(src)) return;
			if (!pk::is_alive(building_)) return;


			/// 추가 내용
			bool isOptimized = false;



			/// ===================== ▼ 여기서부터 특기 배치 CUSTOMIZE =====================



			// 특기 및 적성에 따른 출진
			if (only_AI_unit(src))
			{

				// 창병 출진 (창병 특기 및 나선첨 활용 감안)
				// 초심자 분들은 ↓ 에서 특기를 취향에 맞게 수정하시면 됩니다.
				if (
					src_leader.skill == 특기_창장
					or src_leader.skill == 특기_창신
					or src_leader.skill == 특기_신장
					or src_leader.skill == 특기_투신
					or src_leader.skill == 특기_용장
					or src_leader.skill == 특기_구축
					or src_leader.skill == 특기_패왕
					or src_leader.skill == 특기_맹장
					or src_leader.skill == 특기_난전
					)
					// 초심자 분들은 ↑ 에서 특기를 취향에 맞게 수정하시면 됩니다.	



				{
					if (CheckWeaponAmount_병장기(building_, 병기_창))
					{
						/// 추가 내용
						isOptimized = true;

						//	ReturnUnitWeapon(src, weapon_id);								  // 거점 병장기/병기 반환

						//	pk::add_weapon_amount(building_, 병기_창, -src.troops, false);   	  // 거점 병장기 차감
						pk::set_weapon_amount(src, 0, 병기_창, src.troops);           		  // 부대 병종 지정

						string weapon_name_skill = get_weapon_name(병기_창);
						pk::say(pk::encode(pk::format("我们部队以\x1b[17x{}\x1b[0x出征!", weapon_name_skill)), src_leader);
					}
				}



				// 극병 출진 (극병 특기 및 다수 공격, 방어력 감안)
				// 초심자 분들은 ↓ 에서 특기를 취향에 맞게 수정하시면 됩니다.
				else if (
					src_leader.skill == 특기_극장
					or src_leader.skill == 특기_극신
					or src_leader.skill == 특기_위풍
					or src_leader.skill == 특기_철벽
					or src_leader.skill == 특기_금강
					or src_leader.skill == 특기_등갑
					or src_leader.skill == 특기_불굴
					or src_leader.skill == 특기_호위
					or src_leader.skill == 특기_연전
					or src_leader.skill == 특기_포박
					or src_leader.skill == 특기_친만
					or src_leader.skill == 특기_친월
					)
					// 초심자 분들은 ↑ 에서 특기를 취향에 맞게 수정하시면 됩니다.



				{
					if (CheckWeaponAmount_병장기(building_, 병기_극))
					{
						/// 추가 내용
						isOptimized = true;

						//	ReturnUnitWeapon(src, weapon_id);								  // 거점 병장기/병기 반환

						//	pk::add_weapon_amount(building_, 병기_극, -src.troops, false);   	  // 거점 병장기 차감
						pk::set_weapon_amount(src, 0, 병기_극, src.troops);           		  // 부대 병종 지정
						string weapon_name_skill = get_weapon_name(병기_극);
						pk::say(pk::encode(pk::format("我们部队以\x1b[17x{}\x1b[0x出征!", weapon_name_skill)), src_leader);
					}
				}



				// 기병 출진 (기병 특기 및 공격력 감안)
				// 초심자 분들은 ↓ 에서 특기를 취향에 맞게 수정하시면 됩니다.
				else if (
					src_leader.skill == 특기_기장
					or src_leader.skill == 특기_기신
					or src_leader.skill == 특기_질주
					or src_leader.skill == 특기_백마
					or src_leader.skill == 특기_비장
					or src_leader.skill == 특기_친강
					or src_leader.skill == 특기_친오
					)
					// 초심자 분들은 ↑ 에서 특기를 취향에 맞게 수정하시면 됩니다.		 



				{
					if (CheckWeaponAmount_병장기(building_, 병기_군마))
					{
						/// 추가 내용
						isOptimized = true;

						//	ReturnUnitWeapon(src, weapon_id);								  // 거점 병장기/병기 반환

						//	pk::add_weapon_amount(building_, 병기_군마, -src.troops, false);   	  // 거점 병장기 차감
						pk::set_weapon_amount(src, 0, 병기_군마, src.troops);           	  // 부대 병종 지정
						string weapon_name_skill = get_weapon_name(병기_군마);
						pk::say(pk::encode(pk::format("我们部队以\x1b[17x{}\x1b[0x出征!", weapon_name_skill)), src_leader);
					}
				}



				// 노병 출진 (화시 및 간접공격(범용성, <노>가 가장 좋은 적성이면 노병 출진))
				// 초심자 분들은 ↓ 에서 특기를 취향에 맞게 수정하시면 됩니다.
				else if (
					best_weapon_id == 병기_노
					or src_leader.skill == 특기_궁장
					or src_leader.skill == 특기_궁신
					or src_leader.skill == 특기_사수
					)
					// 초심자 분들은 ↑ 에서 특기를 취향에 맞게 수정하시면 됩니다.	



				{
					if (CheckWeaponAmount_병장기(building_, 병기_노))
					{
						/// 추가 내용
						isOptimized = true;

						//	ReturnUnitWeapon(src, weapon_id);								  // 거점 병장기/병기 반환

						//	pk::add_weapon_amount(building_, 병기_노, -src.troops, false);   	  // 거점 병장기 차감
						pk::set_weapon_amount(src, 0, 병기_노, src.troops);           	  	  // 부대 병종 지정
						string weapon_name_skill = get_weapon_name(병기_노);
						pk::say(pk::encode(pk::format("我们部队以\x1b[17x{}\x1b[0x出征!", weapon_name_skill)), src_leader);
					}
				}



				// 병기 출진 (공성전 감안)
				// 초심자 분들은 ↓ 에서 특기를 취향에 맞게 수정하시면 됩니다. (투석 출진 - 투석개발O)
				else if (
					src_leader.skill == 특기_공성
					or src_leader.skill == 특기_공신
					or src_leader.skill == 특기_사정
					)
					// 초심자 분들은 ↑ 에서 특기를 취향에 맞게 수정하시면 됩니다.



				{

					if (pk::has_tech(pk::get_force(src.get_force_id()), 기교_투석개발))
					{
						if (CheckWeaponAmount_병기(building_, 병기_투석))
						{
							/// 추가 내용
							isOptimized = true;

							//	ReturnUnitWeapon(src, weapon_id);								  // 거점 병장기/병기 반환

							//	pk::add_weapon_amount(building_, 병기_투석, -1, false);   	  		  // 거점 병장기 차감
							pk::set_weapon_amount(src, 0, 병기_투석, 1);           	  			  // 부대 병종 지정
							string weapon_name_skill = get_weapon_name(병기_투석);
							pk::say(pk::encode(pk::format("我们部队以\x1b[17x{}\x1b[0x出征!", weapon_name_skill)), src_leader);
						}
					}
					else
					{
						if (CheckWeaponAmount_병기(building_, 병기_정란))
						{
							/// 추가 내용
							isOptimized = true;

							//	ReturnUnitWeapon(src, weapon_id);								  // 거점 병장기/병기 반환

							//	pk::add_weapon_amount(building_, 병기_정란, -1, false);   	  		  // 거점 병장기 차감
							pk::set_weapon_amount(src, 0, 병기_정란, 1); 						  // 부대 병종 지정
							string weapon_name_skill = get_weapon_name(병기_정란);
							pk::say(pk::encode(pk::format("我们部队以\x1b[17x{}\x1b[0x出征!", weapon_name_skill)), src_leader);
						}
					}

				} // if







				/// ===================== ▲ 여기까지 특기 배치 CUSTOMIZE =====================







				// 위에서 설정된 특기가 없다면 무장의 병종적성에 가장 좋은 병종으로 출진
				// 초심자 분들은 여기부터는 수정하실 필요가 없습니다.
				else
				{
					if (best_weapon_id >= 병기_충차 and best_weapon_id <= 병기_목수)
					{
						if (CheckWeaponAmount_병기(building_, best_weapon_id))
						{
							/// 추가 내용
							isOptimized = true;

							//	ReturnUnitWeapon(src, weapon_id);										// 거점 병장기/병기 반환

							//	pk::add_weapon_amount(building_, best_weapon_id, -1, false);   	  		// 거점 병장기 차감
							pk::set_weapon_amount(src, 0, best_weapon_id, 1); 						// 부대 병종 지정

							pk::say(pk::encode(pk::format("我们部队以\x1b[17x{}\x1b[0x出征!!", weapon_name)), src_leader);
						}
					}

					else
					{
						if (CheckWeaponAmount_병장기(building_, best_weapon_id))
						{
							/// 추가 내용
							isOptimized = true;

							//	ReturnUnitWeapon(src, weapon_id);										// 거점 병장기/병기 반환

							//	pk::add_weapon_amount(building_, best_weapon_id, -src.troops, false);   // 거점 병장기 차감
							pk::set_weapon_amount(src, 0, best_weapon_id, src.troops); 				// 부대 병종 지정

							pk::say(pk::encode(pk::format("我们部队以\x1b[17x{}\x1b[0x出征!!", weapon_name)), src_leader);
						}
					}

				}

			}

			/// 추가 내용
			int new_weapon_id = pk::get_ground_weapon_id(src);
			if (isOptimized)
			{
				unit_id = src.get_id();
				wpn_id_add = weapon_id;
				wpn_id_subtract = new_weapon_id;
				wpn_amount_add = (weapon_id >= 병기_충차 and weapon_id <= 병기_목수) ? 1 : src.troops;
				wpn_amount_subtract = (new_weapon_id >= 병기_충차 and new_weapon_id <= 병기_목수) ? 1 : src.troops;
			}

		} // onUnitCreate



		/// ** 초심자 분들은 SCRIPT에 대해서 잘 모르시면 아래 함수는 수정하지 않도록 주의하시기 바랍니다. **
		/// ** 초심자 분들은 위에서 설정한 병종에 따른 특기만 취향에 맞게 수정하는 것을 권장 ** 

		// Tester님 <외교전쟁>의 함수 참고
		int getBestWeaponId(pk::person@ person)
		{

			// 최고 적성
			int best_tekisei = 적성_C;
			int best_weapon_id = 병기_창;
			for (int i = 0; i < 병기_목수; i++)
			{
				int tekisei = i == 병기_검 ? 0 : person.tekisei[pk::equipment_id_to_heishu(i)];
				if (best_tekisei < tekisei)best_weapon_id = i;

				best_tekisei = pk::max(tekisei, best_tekisei);

			}

			// 최고 적성이 C or 최고 병종이 <노> or 무특기인 경우 노병으로 설정
			if (best_tekisei == 적성_C)
				return best_weapon_id = 병기_노;
			else
				return best_weapon_id;
		}

		// 출진하는 병장기/병기 NAME [기마책사님 출진무장최적화 함수 참고]
		string get_weapon_name(int weapon_id)
		{
			string weapon_name;
			switch (weapon_id)
			{
			case 병기_검: weapon_name = "剑兵"; break;
			case 병기_창: weapon_name = "枪兵"; break;
			case 병기_극: weapon_name = "戟兵"; break;
			case 병기_노: weapon_name = "弩兵"; break;
			case 병기_군마: weapon_name = "骑兵"; break;
			case 병기_충차: weapon_name = "冲车"; break;
			case 병기_정란: weapon_name = "井阑"; break;
			case 병기_투석: weapon_name = "投石"; break;
			case 병기_목수: weapon_name = "木兽"; break;
			case 병기_주가: weapon_name = "走舸"; break;
			case 병기_누선: weapon_name = "楼船"; break;
			case 병기_투함: weapon_name = "斗舰"; break;
			default: weapon_name = "??"; break;
			}
			return weapon_name;
		}

		bool CheckWeaponAmount_병장기(pk::building@ building, int weapon_id)
		{
			if (pk::get_weapon_amount(building, weapon_id) >= 병장기_재고) return true;
			return false;
		}

		bool CheckWeaponAmount_병기(pk::building@ building, int weapon_id)
		{
			if (pk::get_weapon_amount(building, weapon_id) >= 병기_재고) return true;
			return false;
		}

		/*
		// 소속거점으로 병장기/병기 반환
		void ReturnUnitWeapon(pk::unit@ unit, int weapon_id)
		{

			if (!pk::is_alive(unit)) return;
			pk::building@ building_ = pk::get_building(pk::get_service(unit));
			if (!pk::is_alive(building_)) return;

			if (병기_창 <= weapon_id and weapon_id <= 병기_군마)
			{
				pk::add_weapon_amount(building_, weapon_id, unit.troops, false); 	// 거점 병장기 반환
			}
			else if (병기_충차 <= weapon_id and weapon_id <= 병기_목수)
			{
				pk::add_weapon_amount(building_, weapon_id, 1, false);   			// 거점 병기 반환
			}

		}
		*/

		/// 추가 내용
		void RestoreUnitWeapon(pk::unit@ unit, const pk::point& in pos)
		{
			if (!pk::is_alive(unit))							return;
			if (unit.type == 부대종류_수송)						return;		// 수송 부대 제외
			if (pk::get_hex(pos).has_building) return;	                // 실제로 출진하지 않은 경우(출진 취소) 제외

			pk::building@ building = pk::get_building(pk::get_service(unit));
			if (!pk::is_alive(building))	return;
			if (building.pos != unit.pos)	return;		// 부대의 이동 전 위치가 소속 거점이 아닌 경우 제외

			if (unit.get_id() == unit_id)
			{
				// 거점에 변경 전 병기 반환
				pk::add_weapon_amount(building, wpn_id_add, wpn_amount_add, false);

				// 거점에 변경 후 병기 차감
				pk::add_weapon_amount(building, wpn_id_subtract, -wpn_amount_subtract, false);
			}

			unit_id = -1;	// 부대 번호 초기화
		}



		/// *** 아래는 강제종료 ERROR 방지를 위한 FUNCTION (masterpiecek님 부대제거 SCRIPT 참고) ***
		/// *** 为了防止强制终止，执行地形移动限制的功能来排除非正常地格的部队 ***

		void Load_Game(int file_id)
		{

			array<pk::unit@> unit_list = pk::list_to_array(pk::get_unit_list());

			for (int i = 0; i < int(unit_list.length); i++)
			{
				pk::unit@ unit = unit_list[i];

				int terrain_id = pk::get_hex(unit.pos).terrain;

				if (cast<pk::func169_t>(pk::get_func(169))(unit, terrain_id)) return;
				pk::info(pk::format("强制清除部队：原因--部队id：{}，地格id，{}", unit.get_id(), terrain_id));

				pk::kill(unit);
			}

		}

		void onTurnEnd(pk::force@ force)
		{

			array<pk::unit@> unit_list = pk::list_to_array(pk::get_unit_list(force));

			for (int i = 0; i < int(unit_list.length); i++)
			{
				pk::unit@ unit = unit_list[i];

				int terrain_id = pk::get_hex(unit.pos).terrain;

				if (cast<pk::func169_t>(pk::get_func(169))(unit, terrain_id)) return;
				pk::info(pk::format("强制清除部队：原因--部队id：{}，地格id，{}", unit.get_id(), terrain_id));

				pk::kill(unit);
			}

		}

		void onTurnStart(pk::force@ force)
		{

			array<pk::unit@> unit_list = pk::list_to_array(pk::get_unit_list(force));

			for (int i = 0; i < int(unit_list.length); i++)
			{
				pk::unit@ unit = unit_list[i];

				int terrain_id = pk::get_hex(unit.pos).terrain;

				if (cast<pk::func169_t>(pk::get_func(169))(unit, terrain_id)) return;
				pk::info(pk::format("强制清除部队：原因--部队id：{}，地格id，{}", unit.get_id(), terrain_id));

				pk::kill(unit);
			}

		}


	} // class Main


	Main main;


} // namespace
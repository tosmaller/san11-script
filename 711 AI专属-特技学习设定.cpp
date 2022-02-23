// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2020/12/24 # 江东新风 # 修复trace参数报错 ##
// ## 2020/12/17 # 江东新风 # ai学习特技所需能力随游戏设定最大能力变化 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
//## 2020/09/21 #江东新风#配合250特技，已有100以上特技的武将不进行特技学习##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: AI 세력 무장들에게 확률에 따라서 능력에 어울리는 특기로 자동 변경시키는 SCRIPT
說明:AI势力的武将們在每个季度的第一个一月一日，以一定?率(30%)自动更改成与能力匹配的特技SCRIPT。
※AI势力武装特技变更条件(优先顺序)
1.以原本?准，已经拥有?好的特技的武将不属於特技变更物件(EX:呂布，張飞，关羽，項借，呂玲綺，尉迟恭，諸葛亮，曹操，周瑜，黃忠等)
2.武将无特技的情況下
??武力超過90(最高优先顺序)
(1) 兵种适性 [枪兵]S以上，从以下特技随机 [枪将，枪神，神将，?神，勇将，驅逐，猛将，乱战]一項
(2) 兵种适性 [戟兵]S以上，从以下特技随机 [枪将，枪神，神将，?神，勇将，驅逐，猛将，乱战，威风]一項
(3) 兵种适性 [弩兵]S以上，从以下特技随机 [弓将，弓神]一項
(4) 兵种适性 [骑兵]S以上，从以下特技随机 [骑将，骑神，飞将，疾馳]一項
??智力超過90(中等优先顺序)
(5)从以下特技随机 [虚实，鬼謀，百出，深謀 ，连环，言毒，机略]一項
??政治超過90(最低优先顺序)
(6)从以下特技随机 [筑城，能吏，繁殖，指导 ，米道，富豪，征稅，征收，仁政，风水]一項
3.武将有特技情況下
??武力超過90(最高优先顺序)
==> [霸王，枪将，枪神，神将，?神，勇将，驅逐，猛将，乱战，戟将，戟神，威风，攻城，工神，骑将，骑神，飞将]
只有在沒有具备上述特技的情況下，才能更改特技(包括枪兵、戟兵、弩兵、骑兵)。
(1) 兵种适性 [枪兵]S以上，从以下特技随机 [枪将，枪神，神将，?神，勇将，驅逐，猛将，乱战]一項
(2) 兵种适性 [戟兵]S以上，从以下特技随机 [枪将，枪神，神将，?神，勇将，驅逐，猛将，乱战，威风]一項
(3) 兵种适性 [弩兵]S以上，从以下特技随机 [弓将，弓神]一項
(4) 兵种适性 [骑兵]S以上，从以下特技随机 [骑将，骑神，飞将，疾馳]一項
(5) [枪将，戟将，弓将，骑将]有一定?率(30%)将升级?[枪神，戟神，弓神，骑神]
??智力超過90(中等优先顺序)
(6)当掌握的特技不属於 [虚实，鬼謀，百出，深謀 ，连环，言毒，机略，神算，火神，鬼門]时，从以下特技随机 [虚实，鬼謀，百出，深謀，连环，言毒，机略]一項
??政治超過90(最低优先顺序)
(7)当掌握的特技不属於 [筑城，能吏，繁殖，指导 ，米道，富豪，征稅，征收，仁政，风水]时，从以下特技随机[筑城，能吏，繁殖，指导，米道，富豪，征稅，征收，仁政，风水]一項


※ AI 세력 무장 특기 변경 조건 (우선순위)


1. 원본 기준으로 이미 좋은 특기를 소유한 무장은 특기 변경 대상에서 제외 (EX: 여포, 장비, 관우, 항적, 여령기, 위지공, 제갈량, 조조, 주유, 황충 등..)


[2. 无 특기인 무장의 경우]
- 무력 90 이상인 경우 (최우선 순위) -
(1) 병종적성 [창병]이 S 이상이면 [창장, 창신, 신장, 투신, 용장, 구축, 맹장, 난전] 中 한 가지 특기가 RANDOM으로 적용
(2) 병종적성 [극병]이 S 이상이면 [극장, 극신, 신장, 투신, 용장, 구축, 맹장, 난전, 위풍] 中 한 가지 특기가 RANDOM으로 적용
(3) 병종적성 [노병]이 S 이상이면 [궁장, 궁신] 中 한 가지 특기가 RANDOM으로 적용
(4) 병종적성 [기병]이 S 이상이면 [기장, 기신, 질주, 비장] 中 한 가지 특기가 RANDOM으로 적용

- 지력 90 이상인 경우 (중간 우선 순위) -
(5) [허실, 귀모, 백출, 심모, 언독, 연환, 기략] 中 한 가지 특기가 RANDOM으로 적용

- 정치 90 이상인 경우 (마지막 우선 순위) -
(6) [축성, 능리, 번식, 지도, 미도, 부호, 징세, 징수, 인정, 풍수] 中 한 가지 특기가 RANDOM으로 적용


[3. 이미 특기를 소유하고 있는 무장의 경우]
- 무력 90 이상인 경우 (최우선 순위) -
==> [패왕, 창장, 창신, 신장, 투신, 용장, 구축, 맹장, 난전, 극장, 극신, 위풍, 궁장, 궁신, 기장, 기신, 질주, 비장]
==> 위 사항에 해당되는 특기를 소유하고 있지 않은 경우에만 특기를 변경하도록 기본 설정됨 (창병, 극병, 노병, 기병을 전부 감안)
(1) 병종적성 [창병]이 S 이상이면 [창장, 창신, 신장, 투신, 용장, 구축, 맹장, 난전] 中 한 가지 특기가 RANDOM으로 적용
(2) 병종적성 [극병]이 S 이상이면 [극장, 극신, 신장, 투신, 용장, 구축, 맹장, 난전, 위풍] 中 한 가지 특기가 RANDOM으로 적용
(3) 병종적성 [노병]이 S 이상이면 [궁장, 궁신] 中 한 가지 특기가 RANDOM으로 적용
(4) 병종적성 [기병]이 S 이상이면 [기장, 기신, 질주, 비장] 中 한 가지 특기가 RANDOM으로 적용
(5) [창장, 극장, 궁장, 기장] 특기이면 일정 확률로 [창신, 극신, 궁신, 기신]으로 특기가 변경

- 지력 90 이상인 경우 (중간 우선 순위) -
(6) [허실, 귀모, 백출, 심모, 언독, 연환, 기략, 신산, 화신, 귀문]에 해당되지 않는 특기를 소유한 무장이면
	[허실, 귀모, 백출, 심모, 언독, 연환, 기략] 中 한 가지 특기가 RANDOM으로 적용

- 정치 90 이상인 경우 (마지막 우선 순위) -
(7) [축성, 능리, 번식, 지도, 미도, 부호, 징세, 징수, 인정, 풍수]에 해당되지 않는 특기를 소유한 무장이면
	[축성, 능리, 번식, 지도, 미도, 부호, 징세, 징수, 인정, 풍수] 中 한 가지 특기가 RANDOM으로 적용


*/

namespace 특기변경_AI
{

	// ================ CUSTOMIZE ================

		///    ※ 효과_AI만은 TEST를 위해서 설정한 CUSTOMIZE이므로, true로 설정하기를 권장 		
	const bool 효과_AI만 = true;  		// true =AI만 효과적용, false =유저와 AI 모두 효과 적용 

	// 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일
	const int 발동시기 = 2;

	// 발동시기가 0 ~ 3 설정일 경우에 발동 확률
	const int 발동확률 = 30;           // 발동 시 건물별로 발동 확률 설정

	// AI 세력 무장의 특기가 부여되기 위해 필요한 최소 능력
	const int 무력_90 = 90;           // 무력 90 이상인 무장
	const int 지력_90 = 90;           // 지력 90 이상인 무장
	const int 정치_90 = 90;           // 정치 90 이상인 무장

	// 최소 능력 조건을 만족하는 무장인 경우 将 계열 -> 神 계열 특기로 변경되는 확률 
	// (창장,극장,궁장,기장 -> 창신,극신,궁신,기신)
	const int 특기강화확률 = 30;         // 기본 30% 설정

// ===========================================


	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{
			// 매턴 每旬
			if (발동시기 == 0)
				change_skills();

			// 매월 1일 每月第一旬
			if (발동시기 == 1 and (pk::get_day() == 1))
				change_skills();

			// 매분기 첫달이면서 1일 每季度第一旬
			if (발동시기 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
				change_skills();

			// 매년 1월 1일 每年第一旬
			if (발동시기 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
				change_skills();
		}


		// 유저 또는 AI만 적용 시 판단함수 (건물) 判断只对玩家或AI适用的函数
		bool only_AI_building(pk::building@ building)
		{
			if (효과_AI만 and building.is_player()) return false;
			return true;
		}

		void change_skills()
		{

			auto list = pk::list_to_array(pk::get_building_list());

			pk::random random(pk::rand());

			for (int i = 0; i < int(list.length); i++)
			{

				pk::building@ building = list[i];

				pk::person@ taishu = pk::get_person(pk::get_taishu_id(building));

				auto mibun_list = pk::list_to_array(pk::get_person_list(building, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)));

				if (pk::is_alive(taishu) and only_AI_building(building))
				{

					if (0 < mibun_list.length)
					{

						for (int j = 0; j < int(mibun_list.length); j++)
						{

							pk::person@ person_0 = mibun_list[j];


							// 무장이 출진 or 부재 상태이면 제외 判断武将是否已执行或不在城中
							if (pk::is_unitize(person_0) or pk::is_absent(person_0)) continue;


							// 원본 기준으로 이미 좋은 특기를 소유한 무장은 제외 已有高级特技的特定武将除外,已有100以上特技的武将也除外
							// 취향에 따라서 특기가 자동적으로 변경되는 것이 싫은 무장을 이곳에 추가/삭제하셔도 됩니다. 玩家可以在此处添加不变更特技的武将ID
							// ------------------ ↓ ------------------
							if (
								person_0.get_id() == 무장_여포
								or person_0.get_id() == 무장_장비
								or person_0.get_id() == 무장_관우
								or person_0.get_id() == 무장_마초
								or person_0.get_id() == 무장_황충
								or person_0.get_id() == 무장_항적
								or person_0.get_id() == 무장_여영기
								or person_0.get_id() == 무장_관색
								or person_0.get_id() == 무장_조창
								or person_0.get_id() == 무장_위지공
								or person_0.get_id() == 무장_제갈량
								or person_0.get_id() == 무장_곽가
								or person_0.get_id() == 무장_조조
								or person_0.get_id() == 무장_순유
								or person_0.get_id() == 무장_여상_태공망
								or person_0.get_id() == 무장_장량_자방
								or person_0.get_id() == 무장_손빈
								or person_0.get_id() == 무장_방통
								or person_0.get_id() == 무장_주유
								or person_0.get_id() == 무장_육손
								or person_0.get_id() == 무장_오기
								or person_0.get_id() == 무장_종회
								or person_0.get_id() == 무장_제갈각
								or person_0.get_id() == 무장_가충
								or person_0.get_id() == 무장_마속
								or person_0.get_id() == 무장_황월영
								or person_0.get_id() == 무장_양대안
								or person_0.get_id() == 무장_백기
								or person_0.get_id() == 무장_손책
								or person_0.get_id() == 무장_부첨
								or person_0.get_id() == 무장_손례
								or person_0.get_id() == 무장_사마가
								or person_0.get_id() == 무장_화웅
								or person_0.skill > 100
								) continue;
							// ------------------ ↑ ------------------


							if (pk::rand_bool(발동확률))
							{

								// 무특기 무장인 경우 沒有特技的
								if (person_0.skill == -1)
								{

									// 무력 90이상인 무장 (최우선 순위) 武力90以上的武将
									if (int(person_0.base_stat[武将能力_武力]) >= int(무력_90 * int(pk::core["person.max_stat"]) / 100))
									{

										// 창병 적성이 S 이상인 경우 (적성 최우선 순위) 枪兵适性S以上
										// 창장, 창신, 신장, 투신, 용장, 구축, 맹장, 난전 枪将，枪神，神将，?神，勇将，驅逐，猛将，乱战
										if (person_0.tekisei[병종_창병] >= 적성_S)
										{
											int n = pk::rand(8);

											if (n == 0)
												person_0.skill = 특기_창장;

											if (n == 1)
												person_0.skill = 특기_창신;

											if (n == 2)
												person_0.skill = 특기_신장;

											if (n == 3)
												person_0.skill = 특기_투신;

											if (n == 4)
												person_0.skill = 특기_용장;

											if (n == 5)
												person_0.skill = 특기_구축;

											if (n == 6)
												person_0.skill = 특기_맹장;

											if (n == 7)
												person_0.skill = 특기_난전;

											person_0.update();

										} // 창병 적성이 S 이상인 경우


										// 극병 적성이 S 이상인 경우 戟兵适性S以上
										// 극장, 극신, 신장, 투신, 용장, 구축, 맹장, 난전, 위풍 戟将，戟神，神将，斗神，勇将，驅逐，猛将，乱战，威风
										// <기마책사>님의 [야전부대전법확장] SCRIPT 감안 설정 [나선첨 활용 有]
										else if (person_0.tekisei[병종_극병] >= 적성_S)
										{
											int n = pk::rand(9);

											if (n == 0)
												person_0.skill = 특기_극장;

											if (n == 1)
												person_0.skill = 특기_극신;

											if (n == 2)
												person_0.skill = 특기_신장;

											if (n == 3)
												person_0.skill = 특기_투신;

											if (n == 4)
												person_0.skill = 특기_용장;

											if (n == 5)
												person_0.skill = 특기_구축;

											if (n == 6)
												person_0.skill = 특기_맹장;

											if (n == 7)
												person_0.skill = 특기_난전;

											if (n == 8)
												person_0.skill = 特技_威风;

											person_0.update();

										} // 극병 적성이 S 이상인 경우 戟兵适性S以上


										// 노병 적성이 S 이상인 경우 弩兵适性S以上
										// 궁장, 궁신		弓将，弓神								
										else if (person_0.tekisei[병종_노병] >= 적성_S)
										{
											int q = pk::rand(2);

											if (q == 0)
												person_0.skill = 특기_궁장;

											if (q == 1)
												person_0.skill = 특기_궁신;

											person_0.update();

										} // 노병 적성이 S 이상인 경우


										// 기병 적성이 S 이상인 경우 骑兵适性S以上
										// 기장, 기신, 질주, 비장	骑将，骑神，飞将，疾馳									
										else if (person_0.tekisei[병종_기병] >= 적성_S)
										{
											int q = pk::rand(4);

											if (q == 0)
												person_0.skill = 특기_기장;

											if (q == 1)
												person_0.skill = 특기_기신;

											if (q == 2)
												person_0.skill = 特技_疾驰;

											if (q == 3)
												person_0.skill = 특기_비장;

											person_0.update();

										} // 기병 적성이 S 이상인 경우	骑兵适性S以上								

									} // 무력 90이상인 무장 (최우선 순위) 武力90以上的武将


									// 지력 90이상인 무장 
									// 허실, 귀모, 백출, 심모, 언독, 연환, 기략
									else if (int(person_0.base_stat[武将能力_智力]) >= int(지력_90 * int(pk::core["person.max_stat"]) / 100))
									{
										int n = pk::rand(7);

										if (n == 0)
											person_0.skill = 특기_허실;

										if (n == 1)
											person_0.skill = 특기_귀모;

										if (n == 2)
											person_0.skill = 특기_백출;

										if (n == 3)
											person_0.skill = 특기_심모;

										if (n == 4)
											person_0.skill = 특기_언독;

										if (n == 5)
											person_0.skill = 특기_연환;

										if (n == 6)
											person_0.skill = 특기_기략;

										person_0.update();

									} // 지력 90이상인 무장


									// 정치 90이상인 무장 
									// 축성, 능리, 번식, 지도, 미도, 부호, 징세, 징수, 인정, 풍수
									else if (int(person_0.base_stat[武将能力_政治]) >= int(정치_90 * int(pk::core["person.max_stat"]) / 100))
									{
										int n = pk::rand(10);

										if (n == 0)
											person_0.skill = 特技_筑城;

										if (n == 1)
											person_0.skill = 특기_능리;

										if (n == 2)
											person_0.skill = 특기_번식;

										if (n == 3)
											person_0.skill = 특기_지도;

										if (n == 4)
											person_0.skill = 특기_미도;

										if (n == 5)
											person_0.skill = 특기_징세;

										if (n == 6)
											person_0.skill = 특기_징수;

										if (n == 7)
											person_0.skill = 특기_징수;

										if (n == 8)
											person_0.skill = 특기_인정;

										if (n == 9)
											person_0.skill = 특기_풍수;

										person_0.update();

									} // 정치 90이상인 무장									

								} // 무특기 무장인 경우 沒有特技的武将


								// 특기를 소유한 무장인 경우 有特技的武将
								else
								{

									// 무력 90이상인 무장 (최우선 순위)武力超過90(最高优先顺序)
									if (int(person_0.base_stat[武将能力_武力]) >= int(무력_90 * int(pk::core["person.max_stat"]) / 100))
									{

										// 창병 적성이 S 이상인 경우 (적성 최우선 순위)兵种适性 [枪兵]S以上，
										// 패왕, 창장, 창신, 신장, 투신, 용장, 구축, 맹장, 난전, 극장, 극신, 위풍이 아닌 특기를 소유한 경우 (창병, 극병 감안)从以下特技随机 [枪将，枪神，神将，?神，勇将，驅逐，猛将，乱战]一項
										// 궁장, 궁신이 아닌 특기를 소유한 경우 (노병 감안)
										// 기장, 기신, 질주, 비장이 아닌 특기를 소유한 경우 (기병 감안)
										if ((person_0.tekisei[병종_창병] >= 적성_S) and
											(person_0.skill != 특기_패왕
												and person_0.skill != 특기_창장
												and person_0.skill != 특기_창신
												and person_0.skill != 특기_신장
												and person_0.skill != 특기_투신
												and person_0.skill != 특기_용장
												and person_0.skill != 특기_구축
												and person_0.skill != 특기_맹장
												and person_0.skill != 특기_난전
												and person_0.skill != 특기_극장
												and person_0.skill != 특기_극신
												and person_0.skill != 特技_威风
												and person_0.skill != 특기_궁장
												and person_0.skill != 특기_궁신
												and person_0.skill != 특기_기장
												and person_0.skill != 특기_기신
												and person_0.skill != 特技_疾驰
												and person_0.skill != 특기_비장)
											)
										{
											int n = pk::rand(8);

											if (n == 0)
												person_0.skill = 특기_창장;

											if (n == 1)
												person_0.skill = 특기_창신;

											if (n == 2)
												person_0.skill = 특기_신장;

											if (n == 3)
												person_0.skill = 특기_투신;

											if (n == 4)
												person_0.skill = 특기_용장;

											if (n == 5)
												person_0.skill = 특기_구축;

											if (n == 6)
												person_0.skill = 특기_맹장;

											if (n == 7)
												person_0.skill = 특기_난전;

											person_0.update();

										} // 창병 적성이 S 이상인 경우


										// 극병 적성이 S 이상인 경우 
										// 패왕, 창장, 창신, 신장, 투신, 용장, 구축, 맹장, 난전, 극장, 극신, 위풍이 아닌 특기를 소유한 경우 (창병, 극병 감안)
										// 궁장, 궁신이 아닌 특기를 소유한 경우 (노병 감안)
										// 기장, 기신, 질주, 비장이 아닌 특기를 소유한 경우 (기병 감안)
										// <기마책사>님의 [야전부대전법확장] SCRIPT 감안 설정 [나선첨 활용 有]
										else if ((person_0.tekisei[병종_극병] >= 적성_S) and
											(person_0.skill != 특기_패왕
												and person_0.skill != 특기_창장
												and person_0.skill != 특기_창신
												and person_0.skill != 특기_신장
												and person_0.skill != 특기_투신
												and person_0.skill != 특기_용장
												and person_0.skill != 특기_구축
												and person_0.skill != 특기_맹장
												and person_0.skill != 특기_난전
												and person_0.skill != 특기_극장
												and person_0.skill != 특기_극신
												and person_0.skill != 特技_威风
												and person_0.skill != 특기_궁장
												and person_0.skill != 특기_궁신
												and person_0.skill != 특기_기장
												and person_0.skill != 특기_기신
												and person_0.skill != 特技_疾驰
												and person_0.skill != 특기_비장)
											)
										{
											int n = pk::rand(9);

											if (n == 0)
												person_0.skill = 특기_극장;

											if (n == 1)
												person_0.skill = 특기_극신;

											if (n == 2)
												person_0.skill = 특기_신장;

											if (n == 3)
												person_0.skill = 특기_투신;

											if (n == 4)
												person_0.skill = 특기_용장;

											if (n == 5)
												person_0.skill = 특기_구축;

											if (n == 6)
												person_0.skill = 특기_맹장;

											if (n == 7)
												person_0.skill = 특기_난전;

											if (n == 8)
												person_0.skill = 特技_威风;

											person_0.update();

										} // 극병 적성이 S 이상인 경우


										// 노병 적성이 S 이상인 경우 	
										// 패왕, 창장, 창신, 신장, 투신, 용장, 구축, 맹장, 난전, 극장, 극신, 위풍이 아닌 특기를 소유한 경우 (창병, 극병 감안)
										// 궁장, 궁신이 아닌 특기를 소유한 경우 (노병 감안)
										// 기장, 기신, 질주, 비장이 아닌 특기를 소유한 경우 (기병 감안)										
										else if ((person_0.tekisei[병종_노병] >= 적성_S) and
											(person_0.skill != 특기_패왕
												and person_0.skill != 특기_창장
												and person_0.skill != 특기_창신
												and person_0.skill != 특기_신장
												and person_0.skill != 특기_투신
												and person_0.skill != 특기_용장
												and person_0.skill != 특기_구축
												and person_0.skill != 특기_맹장
												and person_0.skill != 특기_난전
												and person_0.skill != 특기_극장
												and person_0.skill != 특기_극신
												and person_0.skill != 特技_威风
												and person_0.skill != 특기_궁장
												and person_0.skill != 특기_궁신
												and person_0.skill != 특기_기장
												and person_0.skill != 특기_기신
												and person_0.skill != 特技_疾驰
												and person_0.skill != 특기_비장)
											)
										{
											int q = pk::rand(2);

											if (q == 0)
												person_0.skill = 특기_궁장;

											if (q == 1)
												person_0.skill = 특기_궁신;

											person_0.update();

										} // 노병 적성이 S 이상인 경우


										// 기병 적성이 S 이상인 경우 	
										// 패왕, 창장, 창신, 신장, 투신, 용장, 구축, 맹장, 난전, 극장, 극신, 위풍이 아닌 특기를 소유한 경우 (창병, 극병 감안)
										// 궁장, 궁신이 아닌 특기를 소유한 경우 (노병 감안)
										// 기장, 기신, 질주, 비장이 아닌 특기를 소유한 경우 (기병 감안)
										else if ((person_0.tekisei[병종_기병] >= 적성_S) and
											(person_0.skill != 특기_패왕
												and person_0.skill != 특기_창장
												and person_0.skill != 특기_창신
												and person_0.skill != 특기_신장
												and person_0.skill != 특기_투신
												and person_0.skill != 특기_용장
												and person_0.skill != 특기_구축
												and person_0.skill != 특기_맹장
												and person_0.skill != 특기_난전
												and person_0.skill != 특기_극장
												and person_0.skill != 특기_극신
												and person_0.skill != 特技_威风
												and person_0.skill != 특기_궁장
												and person_0.skill != 특기_궁신
												and person_0.skill != 특기_기장
												and person_0.skill != 특기_기신
												and person_0.skill != 特技_疾驰
												and person_0.skill != 특기_비장)
											)
										{
											int q = pk::rand(4);

											if (q == 0)
												person_0.skill = 특기_기장;

											if (q == 1)
												person_0.skill = 특기_기신;

											if (q == 2)
												person_0.skill = 特技_疾驰;

											if (q == 3)
												person_0.skill = 특기_비장;

											person_0.update();

										} // 기병 적성이 S 이상인 경우	


										// 창장 특기를 소유한 무장의 특기를 [창신]으로 변경
										else if (person_0.skill == 특기_창장 and pk::rand_bool(특기강화확률))
										{
											person_0.skill = 특기_창신;
											person_0.update();
										}

										// 극장 특기를 소유한 무장의 특기를 [극신]으로 변경
										else if (person_0.skill == 특기_극장 and pk::rand_bool(특기강화확률))
										{
											person_0.skill = 특기_극신;
											person_0.update();
										}

										// 궁장 특기를 소유한 무장의 특기를 [궁신]으로 변경
										else if (person_0.skill == 특기_궁장 and pk::rand_bool(특기강화확률))
										{
											person_0.skill = 특기_궁신;
											person_0.update();
										}

										// 기장 특기를 소유한 무장의 특기를 [기신]으로 변경
										else if (person_0.skill == 특기_기장 and pk::rand_bool(특기강화확률))
										{
											person_0.skill = 특기_기신;
											person_0.update();
										}

									} // 무력 90이상인 무장 (최우선 순위)


									// 지력 90이상인 무장 智力超過90(中等优先顺序)
									//当掌握的特技不属於 [虚实，鬼謀，百出，深謀 ，连环，言毒，机略，神算，火神，鬼門]时，从以下特技随机 [虚实，鬼謀，百出，深謀，连环，言毒，机略]一項 허실, 귀모, 백출, 심모, 언독, 연환, 기략, 신산, 화신, 귀문이 아닌 특기를 소유한 경우	
									else if ((int(person_0.base_stat[武将能力_智力]) >= int(지력_90 * int(pk::core["person.max_stat"]) / 100)) and
										(person_0.skill != 특기_허실
											and person_0.skill != 특기_귀모
											and person_0.skill != 특기_백출
											and person_0.skill != 특기_심모
											and person_0.skill != 특기_언독
											and person_0.skill != 특기_연환
											and person_0.skill != 특기_기략
											and person_0.skill != 특기_신산
											and person_0.skill != 특기_화신
											and person_0.skill != 특기_귀문
											and person_0.skill != 특기_패왕 /*以下?追加武力特技判断，部分特技可考虑刪除*/
											and person_0.skill != 특기_창장/*枪将*/
											and person_0.skill != 특기_창신
											and person_0.skill != 특기_신장
											and person_0.skill != 특기_투신
											and person_0.skill != 특기_용장
											and person_0.skill != 특기_구축
											and person_0.skill != 특기_맹장
											and person_0.skill != 특기_난전/*乱战*/
											and person_0.skill != 특기_극장/*戟将*/
											and person_0.skill != 특기_극신
											and person_0.skill != 特技_威风
											and person_0.skill != 특기_궁장/*弓将*/
											and person_0.skill != 특기_궁신
											and person_0.skill != 특기_기장/*骑将*/
											and person_0.skill != 특기_기신
											and person_0.skill != 特技_疾驰
											and person_0.skill != 특기_비장)
										)
									{
										int n = pk::rand(7);

										if (n == 0)
											person_0.skill = 특기_허실;

										if (n == 1)
											person_0.skill = 특기_귀모;

										if (n == 2)
											person_0.skill = 특기_백출;

										if (n == 3)
											person_0.skill = 특기_심모;

										if (n == 4)
											person_0.skill = 특기_언독;

										if (n == 5)
											person_0.skill = 특기_연환;

										if (n == 6)
											person_0.skill = 특기_기략;

										person_0.update();

									} // 지력 90이상인 무장


									// 정치 90이상인 무장 政治超過90(最低优先顺序) 智力政治双高时，不会将智力特技变?政治特技
									// 축성, 능리, 번식, 지도, 미도, 부호, 징세, 징수, 인정, 풍수가 아닌 특기를 소유한 경우  增加判断不是智力特技，当掌握的特技不属於 [筑城，能吏，繁殖，指导 ，米道，富豪，征稅，征收，仁政，风水]时，从以下特技随机[筑城，能吏，繁殖，指导，米道，富豪，征稅，征收，仁政，风水]一項
									else if ((int(person_0.base_stat[武将能力_政治]) >= int(정치_90 * int(pk::core["person.max_stat"]) / 100)) and
										(person_0.skill != 特技_筑城
											and person_0.skill != 특기_능리
											and person_0.skill != 특기_번식
											and person_0.skill != 특기_지도
											and person_0.skill != 특기_미도
											and person_0.skill != 특기_징세
											and person_0.skill != 특기_징수
											and person_0.skill != 특기_인정
											and person_0.skill != 특기_풍수
											and person_0.skill != 특기_허실
											and person_0.skill != 특기_귀모
											and person_0.skill != 특기_백출
											and person_0.skill != 특기_심모
											and person_0.skill != 특기_언독
											and person_0.skill != 특기_연환
											and person_0.skill != 특기_기략
											and person_0.skill != 특기_신산
											and person_0.skill != 특기_화신
											and person_0.skill != 특기_귀문
											and person_0.skill != 특기_패왕/*以下?武力特技*/
											and person_0.skill != 특기_창장
											and person_0.skill != 특기_창신
											and person_0.skill != 특기_신장
											and person_0.skill != 특기_투신
											and person_0.skill != 특기_용장
											and person_0.skill != 특기_구축
											and person_0.skill != 특기_맹장
											and person_0.skill != 특기_난전
											and person_0.skill != 특기_극장
											and person_0.skill != 특기_극신
											and person_0.skill != 特技_威风
											and person_0.skill != 특기_궁장
											and person_0.skill != 특기_궁신
											and person_0.skill != 특기_기장
											and person_0.skill != 특기_기신
											and person_0.skill != 特技_疾驰
											and person_0.skill != 특기_비장)
										)
									{
										int n = pk::rand(10);

										if (n == 0)
											person_0.skill = 特技_筑城;

										if (n == 1)
											person_0.skill = 특기_능리;

										if (n == 2)
											person_0.skill = 특기_번식;

										if (n == 3)
											person_0.skill = 특기_지도;

										if (n == 4)
											person_0.skill = 특기_미도;

										if (n == 5)
											person_0.skill = 특기_징세;

										if (n == 6)
											person_0.skill = 특기_징수;

										if (n == 7)
											person_0.skill = 특기_징수;

										if (n == 8)
											person_0.skill = 특기_인정;

										if (n == 9)
											person_0.skill = 특기_풍수;

										person_0.update();

									} // 정치 90이상인 무장

								} // 특기를 소유한 무장인 경우

							} // if 발동확률

						} // for

					} // if 

				} // if 

			} // for 

		} // change_skills

	} // class Main

	Main main;

} // namespace
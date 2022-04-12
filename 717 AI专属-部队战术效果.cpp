// ## 2021/02/12 # 氕氘氚 # 适配kre参数自定义 ##
// ## 2021/01/01 # 江东新风 # 战神特技也能大杀四方 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/07 # 江东新风 # 修正发动几率遗漏bug, 部队状态非通常状态下无法发动 ##
// ## 2020/08/07 # 氕氘氚 # 规则、代碼优化 ##
// ## 2020/08/05 # messi # 重新设计台詞 ##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: 통솔, 무력이 동시에 높은 무장이 특정 특기를 소유한 경우에 일정확률로 발휘하는 부대 효과


※ CHAMPION TYPE 부대의 효과

(1) 특기 <앙양>을 소유 시 주위에 있는 적군 부대 기력 흡수 (<앙양> 특기를 보유한 적군부대는 면역)
(2) 특기 <위풍>을 소유 시 주위에 있는 적군 부대 기력 감소 및 자신보다 무력이 낮은 적부대를 일정확률로 혼란 (<위풍> 특기를 보유한 적군부대는 면역)
(3) 특기 [창장 or 창신]을 소유한 창병부대 주위에 있는 적군 기병부대 병력 및 기력 감소 (단, 해당 특기 소유자의 무력 이상인 적군 무장의 부대는 면역)
(4) 특기 [극장 or 극신 or 친월 or 친만]를 소유한 극병부대 주위에 있는 적군 창병부대 병력 및 기력 감소 (단, 해당 특기 소유자의 무력 이상인 적군 무장의 부대는 면역)
(5) 특기 [질주 or 기장 or 기신 or 친강 or 친오]를 소유한 기병부대 주위에 있는 적군 극병부대 병력 및 기력 감소 (단, 해당 특기 소유자의 무력 이상인 적군 무장의 부대는 면역)
(6) 특기 [패왕 or 용장 or 맹장 or 신장 or 비장 or 투신]을 소유한 부대 주위에 있는
	적군부대 병력,기력 감소 및 자신보다 무력이 낮은 적부대를 일정확률로 혼란
   ([패왕 or 용장 or 맹장 or 신장 or 비장 or 투신] 특기를 보유한 적군부대는 면역)


*/

namespace 부대전술효과_AI
{

	// ================ CUSTOMIZE ================


		// 이미 부대전술메뉴가 존재하므로 효과는 AI만으로 설정하는 것을 권장
	const bool 开启战术发动 = true;
	const bool 仅AI有效 = true;  			// true =AI만 효과적용, false =유저와 AI 모두 효과 적용 
	const int 发动时机 = 0;        	   	   		// 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일
	const int 发动机率 = 10;      			// 효과가 발휘되는 확률 (기본 10%)
	const int 气力条件 = 80;


// ===========================================


	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{
			// 발동시기 매턴
			if (发动时机 == 0)
			{
				effect_战术();
			}
			// 매월 1일
			else if (发动时机 == 1 and (pk::get_day() == 1))
			{
				effect_战术();
			}
			// 매분기 첫달이면서 1일	
			else if (发动时机 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
			{
				effect_战术();
			}

		}

		// 유저 또는 AI만 적용 시 판단함수
		bool only_AI_unit(pk::unit@ unit)
		{
			if (仅AI有效 and unit.is_player()) return false;
			return true;
		}

		void effect_战术()
		{
			if (!开启战术发动) return;

			auto list = pk::list_to_array(pk::get_unit_list());

			for (int i = 0; i < int(list.length); i++)
			{
				if (!pk::rand_bool(发动机率)) continue;
				pk::unit@ src = list[i];
				if (src.status != 部队状态_通常) continue;
				pk::person@ src_leader = pk::get_person(src.leader);
				pk::point pos = src.get_pos();

				if (only_AI_unit(src)  && src.energy > 气力条件)
				{
					// 특기 <앙양>을 소유 시 주위에 있는 적군 부대 기력 흡수 (단, 해당 특기 소유자의 무력 이상인 적군 무장의 부대는 면역)
					if (src.has_skill(특기_앙양))
					{
						global_func_昂扬奋战(pos, @src, @src_leader);
					}

					// 특기 <위풍>을 소유 시 주위에 있는 적군 부대 기력 감소 및 일정확률로 혼란 (단, 해당 특기 소유자의 무력 이상인 적군 무장의 부대는 면역)
					else if (src.has_skill(特技_威风))
					{
						global_func_威风屈敌(pos, @src, @src_leader);
					}

					// 특기 [창장 or 창신]을 소유한 창병부대 주위에 있는 적군 기병부대 병력 및 기력 감소 (단, 해당 특기 소유자의 무력 이상인 적군 무장의 부대는 면역)
					else if ((src.has_skill(특기_창장) or src.has_skill(특기_창신) or src.has_skill(특기_투신))
<<<<<<< HEAD
						and src.type_id == 병기_창)
=======
						and src.type_id == 兵器_枪)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					{
						global_func_枪兵冲锋(pos, @src, @src_leader);
					}

					// 특기 [극장 or 극신 or 친월 or 친만]을 소유한 극병부대 주위에 있는 적군 창병부대 병력 및 기력 감소 (단, 해당 특기 소유자의 무력 이상인 적군 무장의 부대는 면역)
					else if ((src.has_skill(특기_극장) or src.has_skill(특기_극신) or src.has_skill(특기_친월) or src.has_skill(특기_친만))
<<<<<<< HEAD
						and src.type_id == 병기_극)
=======
						and src.type_id == 兵器_戟)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					{
						global_func_戟兵破甲(pos, @src, @src_leader);
					}

					// 특기 [기장 or 기신]을 소유한 기병부대 주위에 있는 적군 극병부대 병력 및 기력 감소 (단, 해당 특기 소유자의 무력 이상인 적군 무장의 부대는 면역)
					else if ((src.has_skill(特技_疾驰) or src.has_skill(특기_기장) or src.has_skill(특기_기신) or src.has_skill(특기_친강) or src.has_skill(특기_친오))
<<<<<<< HEAD
						and src.type_id == 병기_군마)
=======
						and src.type_id == 兵器_战马)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					{
						global_func_骑兵突袭(pos, @src, @src_leader);
					}

					// 특기 [패왕 or 용장 or 맹장 or 신장 or 비장 or 투신]을 소유한 부대 주위에 있는 적군부대 병력 감소 및 혼란 (단, 해당 특기 소유자의 무력 이상인 적군 무장의 부대는 면역)
					else if (src.has_skill(특기_패왕) or src.has_skill(특기_용장) or src.has_skill(특기_맹장)
						or src.has_skill(특기_신장) or src.has_skill(특기_비장) or src.has_skill(特技_战神))
					{
						global_func_大杀四方(pos, @src, @src_leader);
					}

				} // (only_AI_unit(src))

			} // for 

		} // effect_战术

	} // class Main

	Main main;

} // namespace
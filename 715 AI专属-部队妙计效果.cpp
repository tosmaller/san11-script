// ## 2021/03/03 # 江东新风 # 兵力条件 ##
// ## 2021/02/16 # 江东新风 # 调整默认发动几率为10% ##
// ## 2021/02/12 # 氕氘氚 # 适配kre参数自定义 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/08/07 # 氕氘氚 # 规则、代碼优化 ##
// ## 2020/08/05 # messi # 重新设计台詞 ##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: AI 세력의 부대도 일정확률로 부대묘책효과를 발휘하는 SCRIPT
@기타: 난이도 상승 목적으로 제작한 SCRIPT


※ 효과 발동 조건
- 특정 특기를 소유한 AI 세력의 부대에서 발동
- 신화계(화신), 팔진도(신산), 허허실실(허실), 심모(속전고수) 효과 발동 = (주유, 제갈량, 조조, 사마의가 AI 세력일 경우를 기본적으로 감안)
- 부대의 기력이 일정 조건 이상이 충족되어야 발동 (기본 80으로 설정, AI 부대는 기력 소모하지 않고 단지 조건만 충족하면 됨)
- 효과가 발동될 확률은 기본 30%로 설정
- 부대 지력이 적부대의 지력보다 높은 경우와 낮은 경우에 각기 정해진 성공률이 반영
- 신화계 효과: 범위 내 적군부대 화계 + 기력 감소 + 혼란
- 팔진도 효과: 범위 내 [아군부대 기력/상태 회복] and [적군부대 기력 감소 + 혼란]
- 허허실실 효과: 범위 내 [적군부대 병력 감소 + 혼란] or [적군부대 병력 감소 + 위보]
- 속전고수 효과: 범위 내 [아군부대 병력/기력 회복] and [적군부대 병량/기력 감소]


*/

namespace 부대묘책효과_AI
{

	// ================ CUSTOMIZE ================


		///  TEST를 위한 목적으로 존재하는 변수이며, 효과_AI만 = true로 설정하는 것을 권장 
	const bool 仅AI有效 = true;  				// true =AI만 효과적용, false =유저와 AI 모두 효과 적용 

	const int 发动时机 = 0;        	   	   		// 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일

	const int 发动机率 = 100;      					// 효과가 발동되는 확률 (기본 10%)

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
				effect_妙计();
			}
			// 매월 1일
			else if (发动时机 == 1 and (pk::get_day() == 1))
			{
				effect_妙计();
			}
			// 매분기 첫달이면서 1일	
			else if (发动时机 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
			{
				effect_妙计();
			}
		}

		// 유저 또는 AI만 적용 시 판단함수
		bool only_AI_unit(pk::unit@ unit)
		{
			if (仅AI有效 and unit.is_player()) return false;
			return true;
		}


		void effect_妙计()
		{
			auto list = pk::list_to_array(pk::get_unit_list());

			for (int i = 0; i < int(list.length); i++)
			{
				if (!pk::rand_bool(发动机率)) 	continue;

				pk::unit@ src_unit = list[i];
				pk::person@ src_leader = pk::get_person(src_unit.leader);
				pk::point pos = src_unit.get_pos();
				//pk::trace(pk::format("{}的部队妙计判断",pk::decode(pk::get_name(src_unit))));

				if (only_AI_unit(src_unit) && src_unit.status == 部队状态_通常 && src_unit.energy >= 气力条件 && src_unit.troops >= 部队妙计_兵力条件)
				{
					//pk::trace(pk::format("{}的部队可以执行部队妙计",pk::decode(pk::get_name(src_unit))));

					if (开启_神火计 && src_unit.has_skill(특기_화신))
					{
						global_func_神火计(pos, @src_unit, @src_leader);
					}

					if (开启_八阵图 && src_unit.has_skill(특기_신산))
					{
						global_func_八阵图(pos, @src_unit, @src_leader);
					}

					if (开启_虚虚实实 && src_unit.has_skill(특기_허실))
					{
						global_func_虚虚实实(pos, @src_unit, @src_leader);
						//pk::trace(pk::format("{}的执行了虚虚实实",pk::decode(pk::get_name(src_unit))));
					}

					if (开启_速战固守 && src_unit.has_skill(특기_심모))
					{
						global_func_速战固守(pos, @src_unit, @src_leader);
					}

				}
			}
		}

	} // class Main

	Main main;

} // namespace

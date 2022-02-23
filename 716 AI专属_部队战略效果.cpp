// ## 2021/02/12 # 氕氘氚 # 适配kre参数自定义 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/08/07 # 氕氘氚 # 规则、代碼优化 ##
/*
@제작자: HoneyBee
@설명: 통솔, 지력이 동시에 높은 무장이 [군주, 도독] 신분이거나 [평북장군] 이상의 관직인 경우에 일정확률로 발휘하는 부대 효과


※ COMMANDER TYPE 부대의 효과

(1) 부대 주위 아군 기력 회복
(2) 부대 주위 적군 기력 감소
(3) 부대 주위 아군 병력 회복
(4) 부대 주위 적군 병력 흡수 (심공 효과)


*/


namespace 부대전략효과_AI
{

	// ================ CUSTOMIZE ================


		// 이미 부대전략메뉴가 존재하므로 효과는 AI만으로 설정하는 것을 권장
	const bool 仅AI有效 = true;  			// true =AI만 효과적용, false =유저와 AI 모두 효과 적용 

	const int 发动时机 = 0;        	   	   		// 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일

	const int 发动机率 = 10;      			// 효과가 발휘되는 확률 (기본 10%)

	const int 气力条件 = 80;

	const int 관직조건 = 관직_평북장군;			// 평북장군 이상의 관직



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
				effect_战略();
			}
			// 매월 1일
			else if (发动时机 == 1 and (pk::get_day() == 1))
			{
				effect_战略();
			}
			// 매분기 첫달이면서 1일	
			else if (发动时机 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
			{
				effect_战略();
			}

		}

		// 유저 또는 AI만 적용 시 판단함수
		bool only_AI_unit(pk::unit@ unit)
		{
			if (仅AI有效 and unit.is_player()) return false;
			return true;
		}


		// 신분이 [군주/도독]이거나 관직이 [평북장군]이상인 부대의 무장이 통솔 80대 + 지력 80대
		void effect_战略()
		{
			auto list = pk::list_to_array(pk::get_unit_list());

			for (int i = 0; i < int(list.length); i++)
			{
				if (!pk::rand_bool(发动机率)) 	continue;

				pk::unit@ src = list[i];
				pk::person@ src_leader = pk::get_person(src.leader);
				pk::point pos = src.get_pos();

				int n = pk::rand(4);

				if (only_AI_unit(src) && src.energy > 气力条件)
				{

					if ((src.leader == pk::get_kunshu_id(src)) or (src.leader == pk::get_totoku_id(src))
						or (src_leader.rank <= 관직조건))
					{

						// 부대 주위 아군 기력 회복
						if (n == 0)
						{
							global_func_士气高漲(pos, @src, @src_leader);

						} // 부대 주위 아군 기력 회복


						// 부대 주위 적군 기력 감소
						if (n == 1)
						{
							global_func_敌军动摇(pos, @src, @src_leader);

						} // 부대 주위 적군 기력 감소


						// 부대 주위 아군 병력 회복 
						if (n == 2)
						{
							global_func_援军支援(pos, @src, @src_leader);

						} // 부대 주위 아군 병력 회복 


						// 부대 주위 적군 병력 흡수 (심공 효과)
						if (n == 3)
						{
							global_func_敌军煽动(pos, @src, @src_leader);

						} // 부대 주위 적군 병력 감소 (심공 효과)  

					} // if (신분 조건)

				} // (only_AI_unit(src))

			} // for 

		} // effect_战略

	} // class Main

	Main main;

} // namespace
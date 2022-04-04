// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/07/26 ##
/*
*** Notice
본 스크립트에 대한 재배포, 무단수정을 금지 합니다.
개선을 빙자한 잘라먹기, 덮어씌우기 방식의 별도 패치 역시 거부 합니다.
필요사항 경우 쪽지나 메일을 통해 요청을 주시면 검토 후 반영하겠습니다.

*** 제작
	제르나 -	삼국지 공방		(https://cafe.naver.com/rtkbank)
			삼국지 도원결의	(https://cafe.naver.com/sam10)

*** 환경
	삼국지11 PK - PK2.1

*** History ***
--------------------------------------------------------------------
		작 업 자			| 수정 일자	|		작 업 내 용
--------------------------------------------------------------------
	제르나	- 공방		| 18.11.27	| 초기생성
*/

namespace 행동가능
{
	class Main
	{
		// 플레이어
		bool	player_check = true;// 사용 여부
		int		player_rate = 100;	// 적용 확률 (1 - 100)

		// 인공지능
		bool	ai_check = true;
		int		ai_rate = 100;		// 적용 확률 (1 - 100)

		Main()
		{
			pk::bind(111, pk::trigger111_t(callback));
		}

		void callback(pk::force@ force)
		{
			pk::force@ force_ = force;
			int rate;

			if (force_.is_player())
			{
				if (!player_check)	return;
				rate = player_rate;
			}
			else
			{
				if (!ai_check)	return;
				rate = ai_rate;
			}

			array<pk::unit@> arr_unit;
			arr_unit = pk::list_to_array(pk::get_unit_list(force_));
			if (arr_unit.length < 1) return;

			for (int i = 0; i < int(arr_unit.length); i++)
			{
				if (arr_unit[i].status == 部队状态_通常)
				{
					if (int(pk::rand(100)) < rate)
					{
						arr_unit[i].action_done = false;
						arr_unit[i].update();
					}
				}
			}
		}
	}

	Main main;
}
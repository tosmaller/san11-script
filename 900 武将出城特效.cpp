// ## 2021/10/24 #江东新风#改写171trigger##
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
	제르나	- 공방		| 19.02.21	| 초기생성
	제르나	- 공방		| 19.02.21	| v1.1
									| 전법/계략 실패 시 효과 분리
	제르나	- 공방		| 19.02.24	| v2.0
									| 부대 생성 시 컷인 출력 추가
	제르나	- 공방		| 19.02.27	| v3.0
									| 부대 생성 시 초상 변경 추가
	제르나	- 공방		| 19.03.11	| v3.1
									| [초상 - 부대 생성] 1턴 1회 옵션 추가
*/

namespace 무장강조
{
	class Main
	{
		// ----- set ----- //
		// mode - 0:不使用미사용, 1:玩家1军团플레이어(군단), 2:玩家全部플레이어(전체), 3:所有势力전세력
		int		new_mode = 1;			// 暴击图 - 部队生成 컷인 - 부대 생성
		int		mod_mode = 0;			// 肖像 - 部队生成/解散 초상 - 부대 생성/해산
		int		exp_mode = 0;			// 暴击图= 部队行动 컷인 - 부대 행동

		// new
		bool	use_otoc = true;			// 1旬一次 1턴 1회

		// mod

		// exp
		bool	use_attack = false; 		// 普通攻击통상공격
		bool	use_skill = true;			// 战法전법
		bool	use_tactics = true;			// 计謀계략

		// ----- --- ----- //

		// ----- data ----- //
		array<int> arr_PersonId;			// 1턴 컷인 출력
		// ----- ---- ----- //

		Main()
		{
			pk::bind(112, pk::trigger112_t(callback_112));
			pk::bind(170, pk::trigger170_t(callback_170));
			pk::bind(171, pk::trigger171_t(callback_171));
			pk::bind(173, pk::trigger173_t(callback_173));
		}

		// 턴 종료
		void callback_112(pk::force@ force)
		{
			// 컷인 로그 삭제
			if (use_otoc)
				arr_PersonId.removeRange(0, int(arr_PersonId.length));
		}

		// 부대 생성
		void callback_170(pk::unit@ unit, int type)
		{
			if (type == 0)
			{
				// 초상 - 부대 생성
				if (func_check_set(mod_mode, unit))
					func_face(unit, 0);

				// 컷인 - 부대 생성
				if (func_check_set(new_mode, unit))
				{
					if (use_otoc)
					{
						bool check = true;
						for (int i = 0; i < int(arr_PersonId.length); i++)
						{
							if (arr_PersonId[i] == unit.leader)
								check = false;
						}

						if (check)
						{
							arr_PersonId.insertLast(unit.leader);
							func_cutin(unit, 0, -1);
						}
					}
					else
					{
						func_cutin(unit, 0, -1);
					}
				}
			}
		}

		// 부대 해산
		void callback_171(pk::unit@ unit, pk::hex_object@ dst, int type)
		{
			// 초상 - 부대 해산
			if (func_check_set(mod_mode, unit))
				func_face(unit, 1);
		}

		// 부대 행동
		void callback_173(pk::unit@ unit, int type)
		{
			// 컷인 - 부대 행동
			if (func_check_set(exp_mode, unit))
				func_cutin(unit, 1, type);
		}

		bool func_check_set(int set_mode, pk::unit@ unit)
		{
			if (set_mode == 0)
				return false;
			else if (set_mode == 1 and (!unit.is_player() or unit.get_district_id() != pk::get_district_id(pk::get_force(unit.get_force_id()), 1)))
				return false;
			else if (set_mode == 2 and !unit.is_player())
				return false;

			return true;
		}

		void func_cutin(pk::unit@ unit, int func_mode, int exp_type)
		{
			// 컷인 - 부대 생성
			if (func_mode == 0)
			{
				pk::face_cutin(unit);
			}
			// 컷인 - 부대 행동
			else if (func_mode == 1)
			{
				if (use_attack
					and exp_type >= 0 and exp_type < 6)
				{
					pk::face_cutin(unit);
				}
				if (use_skill)
				{
					if (exp_type >= 6 and exp_type < 54)
					{
						// 크리티컬
						if (exp_type % 3 == 0)
							pk::face_cutin(unit);
						// 일반
						if (exp_type % 3 == 1)
							pk::face_cutin(unit);
						// 실패
						if (exp_type % 3 == 2)
							pk::play_se(4);
					}
				}
				if (use_tactics)
				{
					if (exp_type >= 54 and exp_type < 81)
					{
						// 크리티컬
						if (exp_type % 3 == 0)
							pk::face_cutin(unit);
						// 일반
						if (exp_type % 3 == 1)
							pk::face_cutin(unit);
						// 실패
						if (exp_type % 3 == 2)
							pk::play_se(4);
					}
				}
			}
		}

		void func_face(pk::unit@ unit, int func_mode)
		{
			if (func_mode == 0)
			{
				for (int i = 0; i < 3; i++)
				{
					if (unit.member[i] > -1)
					{
						pk::person@ target = pk::get_person(unit.member[i]);
						if (target.face >= 0 and target.face < 200)
						{
							target.face = target.face + 2200;
							target.update();
						}
					}
				}
			}
			else if (func_mode == 1)
			{
				for (int i = 0; i < 3; i++)
				{
					if (unit.member[i] > -1)
					{
						pk::person@ target = pk::get_person(unit.member[i]);
						if (target.face >= 2200 and target.face < 2400)
						{
							target.face = target.face - 2200;
							target.update();
						}
					}
				}
			}
		}
	}

	Main main;
}
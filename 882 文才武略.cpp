// ## 2020/12/06 # 江东新风 # 修正文本异常 ##
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
	제르나	- 공방		| 18.11.24	| 초기생성
	제르나	- 공방		| 18.11.28	| v1.1
									| 무장 신분/상병 변경 버그 수정
*/

namespace 文才武略
{
	const int KEY = pk::hash("가상대전");

	class Main
	{
		array<string> team_name =
		{
			"玩家",
			"电脑"
		};

		array<array<pk::person@>> player(2, array<pk::person@>(3));
		array<array<int>> back_mibun(2, array<int>(3));
		array<array<int>> back_shoubyou(2, array<int>(3));

		array<int> team_count(2, 1);

		int mode = 0;
		int min_person = 1;
		int max_person = 1;

		Main()
		{
			add_menu();
		}

		void add_menu()
		{
			pk::menu_item item;
			item.menu = 2;
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		bool isEnabled()
		{
			return true;
		}

		string getText()
		{
			return pk::encode("文才武略");
		}

		string getDesc()
		{
			return pk::encode("举行比武,辩论大会.");
		}

		bool handler()
		{
			//모드 선택
			array<string> btn_mode =
			{
				pk::encode(" 单挑 "),
				pk::encode(" 舌战 "),
				pk::encode(" 取消 ")
			};

			mode = pk::choose(pk::encode("选择要比试的类型."), btn_mode);
			if (mode == 2) return false;
			if (mode == 0) max_person = 3;
			if (mode == 1) max_person = 1;

			//무장 설정
			if (!set_team()) return false;

			//대전 진행
			pk::scene(pk::scene_t(scene));

			//무장 복구
			restore_person();

			return true;
		}

		bool set_team()
		{
			for (int i = 0; i < 2; i++)
			{
				array<pk::person@> arr_select;
				pk::person@ dummy_person;

				pk::message_box(pk::encode(pk::format("请选择\x1b[1x[{}]\x1b[0x阵营的武将.", team_name[i])));
				arr_select = pk::list_to_array(pk::person_selector(pk::encode("选择武将"), pk::encode(pk::format("选择\x1b[1x{}\x1b[0x阵营的武将", team_name[i])), pk::get_person_list(pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般, 신분_재야, 신분_포로, 신분_미등장, 신분_미발견, 신분_사망)), min_person, max_person));
				team_count[i] = arr_select.length;
				if (team_count[i] == 0) return false;

				for (int j = 0; j < team_count[i]; j++)
				{
					@player[i][j] = arr_select[j];

					back_mibun[i][j] = player[i][j].mibun;
					back_shoubyou[i][j] = player[i][j].shoubyou;
				}

				for (int k = team_count[i]; k < 3; k++)
				{
					@player[i][k] = dummy_person;
				}
			}

			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < team_count[i]; j++)
				{
					player[i][j].mibun = 身份_一般;
					player[i][j].shoubyou = 상병_건강;
					player[i][j].update();
				}
			}

			return true;
		}


		void scene()
		{
			pk::unit@ dummy_unit;
			string winner;

			if (mode == 0)
				winner = team_name[pk::duel(dummy_unit, dummy_unit, player[0][0], player[0][1], player[0][2], player[1][0], player[1][1], player[1][2], true, false, 0, true).first];
			else if (mode == 1)
				winner = team_name[pk::debate(player[0][0], player[1][0], true, false, false, true).first];

			pk::message_box(pk::encode(pk::format("\x1b[1x[{}]\x1b[0x阵营胜利.", winner)));
		}

		void restore_person()
		{
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < team_count[i]; j++)
				{
					player[i][j].mibun = back_mibun[i][j];
					player[i][j].shoubyou = back_shoubyou[i][j];
					player[i][j].update();
				}
			}
		}
	}

	Main main;
}
// ## 2020/07/26 ##
/*
	@제작 : masterpiecek
	@도움 : 기마책사


	[ 부대정보표시 ]

	1. 부대가 위치한 좌표의 정보를 표시
	 1) 해당 좌표 부대의 현재 부대 방향
	 2) 부대가 위치한 좌표의 지형 고도 정보
	  * 정보 표시 모드 설정 가능 : 커서 모드(커서를 가리킨 경우에만 표시), 지속 모드(항상 표시)

	2. 부대와 인접한 좌표의 정보를 표시
	 1) 부대 인접 좌표 기준 부대의 피격 방향
	 2) 부대 인접 좌표의 지형 고도 정보
	  * 마우스 커서를 부대 위에 올린 경우에만 정보 표시

	3. 일반 좌표의 정보를 표시 : 일반 좌표의 지형 고도 정보를 표시
	 * 마우스 커서를 부대 위에 올린 경우에만 정보 표시

	※ 표시 정보별 목적
	 - 부대 방향 및 피격 방향 : 부대 방향에 따라 공격 대미지가 달라지는 유저 패치를 사용하는 경우
	 - 지형의 고도 : 공격 대미지 및 전법 성공 확률 등이 좌표 간 높이 차이에 따라 달라짐
*/

namespace 부대정보표시
{

	/// ================================== < 유저 설정 > ============================================

		// 글자 색상 설정 시 참고 : 0(흰색), 1(파랑), 2(초록), 3(주황), 4(노랑), 5(빨강)

	/// ---------------------------------------------------------------------------------------------

	/// [ 부대위치좌표의 정보표시 관련 설정 ]
	//	- 커서모드와 지속모드 중 선택 가능

		/// 부대위치좌표 정보표시 모드 설정
		// 모드 1 - 커서모드 : 좌표에 존재하는 부대 위에 커서를 가져가면 해당 좌표에 정보를 표시
		// 모드 2 - 지속모드 : 좌표에 존재하는 모든 부대의 정보를 각 부대가 위치한 좌표에 항상 표시
	const int 부대위치좌표_정보표시_모드 = 1;

	/// 부대가 위치한 좌표에서의 부대방향 표시 설정 : 북서, 북, 북동, 남서, 남, 남동
	const bool 부대위치좌표_부대방향_표시 = true;
	const int  부대위치좌표_부대방향_색상 = 0;

	/// 부대가 위치한 좌표의 고도(높이) 표시 설정 : 숫자가 클수록 높은 고도
	const bool 부대위치좌표_지형고도_표시 = true;
	const int  부대위치좌표_지형고도_색상 = 0;

	/// ---------------------------------------------------------------------------------------------

	/// [ 부대인접좌표의 정보표시 관련 설정 ]
	//	- 커서모드로만 표시

		/// 부대와 인접한 좌표로부터의 부대의 피격방향 표시 설정 : 전면, 전측, 후측, 후면
	const bool 부대인접좌표_피격방향_표시 = true;
	const int  부대인접좌표_피격방향_색상_전면 = 0;
	const int  부대인접좌표_피격방향_색상_전측 = 4;
	const int  부대인접좌표_피격방향_색상_후측 = 3;
	const int  부대인접좌표_피격방향_색상_후면 = 5;

	/// 부대와 인접한 좌표의 고도(높이) 표시 설정 : 숫자가 클수록 높은 고도
	const bool 부대인접좌표_지형고도_표시 = true;
	const int  부대인접좌표_지형고도_색상_높음 = 5;	// 부대인접좌표 고도 > 부대위치좌표 고도
	const int  부대인접좌표_지형고도_색상_낮음 = 1;	// 부대인접좌표 고도 < 부대위치좌표 고도
	const int  부대인접좌표_지형고도_색상_같음 = 0;	// 부대인접좌표 고도 = 부대위치좌표 고도

/// ---------------------------------------------------------------------------------------------

/// [ 일반좌표의 지형고도 정보 표시 관련 설정 ]
//	- 커서모드로만 표시

	/// 맵 상의 일반 좌표의 고도(높이) 표시 설정 : 숫자가 클수록 높은 고도
	const bool 일반좌표_지형고도_표시 = false;
	const int  일반좌표_지형고도_색상 = 0;

	/// =============================================================================================

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

				// 표시좌표 색인 : 0(부대위치좌표), 1(부대인접좌표), 2(일반좌표)        - pos_index
				// 글자색상 색인 : 0(흰색), 1(파랑), 2(초록), 3(주황), 4(노랑), 5(빨강) - clr_index
				// 방향 번호     : 0(북서), 1(북), 2(북동), 3(남서), 4(남), 5(남동)     - direction_id
				// 피격방향 색인 : 0(전면), 1(전측), 2(후측), 3(후면)                   - atk_dir_index

		array<string> 글자_색상 = { "\x1b[0x", "\x1b[1x", "\x1b[2x", "\x1b[16x", "\x1b[17x", "\x1b[29x" };
		array<string> 부대방향_이름 = { "西北", "北", "东北", "西南", "南", "东南" };
		array<string> 피격방향_이름 = { "前方", "前侧", "后侧", "后方" };
		array<int> 피격방향_색상 = { 부대인접좌표_피격방향_색상_전면, 부대인접좌표_피격방향_색상_전측, 부대인접좌표_피격방향_색상_후측, 부대인접좌표_피격방향_색상_후면 };
		array<array<array<int>>> 피격방향_정보 =
		{
			/**북서*/{ /*전면*/{방향_남동}, /*전측*/{방향_북동,방향_남  }, /*후측*/{방향_북,  방향_남서}, /*후면*/{방향_북서} },
			/** 북 */{ /*전면*/{방향_남  }, /*전측*/{방향_남서,방향_남동}, /*후측*/{방향_북서,방향_북동}, /*후면*/{방향_북  } },
			/**북동*/{ /*전면*/{방향_남서}, /*전측*/{방향_남,  방향_북서}, /*후측*/{방향_남동,방향_북  }, /*후면*/{방향_북동} },
			/**남서*/{ /*전면*/{방향_북동}, /*전측*/{방향_남동,방향_북  }, /*후측*/{방향_남,  방향_북서}, /*후면*/{방향_남서} },
			/** 남 */{ /*전면*/{방향_북  }, /*전측*/{방향_북서,방향_북동}, /*후측*/{방향_남서,방향_남동}, /*후면*/{방향_남  } },
			/**남동*/{ /*전면*/{방향_북서}, /*전측*/{방향_북,  방향_남서}, /*후측*/{방향_북동,방향_남  }, /*후면*/{방향_남동} }
		};

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			pk::bind(120, pk::trigger120_t(게임화면그리기_부대위치좌표_정보표시));
			pk::bind(120, pk::trigger120_t(게임화면그리기_부대인접좌표_정보표시));
			pk::bind(120, pk::trigger120_t(게임화면그리기_일반좌표_정보표시));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void 게임화면그리기_부대위치좌표_정보표시()
		{
			if (!is_normal_game()) return;

			if (부대위치좌표_정보표시_모드 == /*커서모드*/1)
			{
				pk::point cursor_pos = pk::get_cursor_hex_pos();
				if (!pk::is_valid_pos(cursor_pos)) return;

				pk::unit@ cursor_unit = pk::get_unit(cursor_pos);
				if (!pk::is_alive(cursor_unit)) return;

				func_부대위치좌표_정보표시(cursor_unit);
			}
			else if (부대위치좌표_정보표시_모드 == /*지속모드*/2)
			{
				for (int unit_id = 0; unit_id < 部队_末; unit_id++)
				{
					pk::unit@ unit = pk::get_unit(unit_id);
					if (!pk::is_alive(unit)) continue;

					func_부대위치좌표_정보표시(unit);
				}
			}
		}

		void 게임화면그리기_부대인접좌표_정보표시()
		{
			if (!is_normal_game()) return;

			pk::point cursor_pos = pk::get_cursor_hex_pos();
			if (!pk::is_valid_pos(cursor_pos)) return;

			pk::unit@ cursor_unit = pk::get_unit(cursor_pos);
			if (!pk::is_alive(cursor_unit)) return;

			for (int direction_id = 0; direction_id < 방향_끝; direction_id++)
			{
				pk::point neighbor_pos = pk::get_neighbor_pos(cursor_unit.pos, direction_id);
				if (!pk::is_valid_pos(neighbor_pos)) continue;

				pk::hex@ hex = pk::get_hex(neighbor_pos);
				if (hex.has_building) continue;

				int terrain_id = hex.terrain;
				if (!pk::is_valid_terrain_id(terrain_id)) continue;
				if (terrain_id == 지형_미정) continue;

				func_부대인접좌표_정보표시(cursor_unit, neighbor_pos);
			}
		}

		void 게임화면그리기_일반좌표_정보표시()
		{
			if (!is_normal_game()) return;

			pk::point cursor_pos = pk::get_cursor_hex_pos();
			if (!pk::is_valid_pos(cursor_pos)) return;

			int terrain_id = pk::get_hex(cursor_pos).terrain;
			if (!pk::is_valid_terrain_id(terrain_id)) return;
			if (terrain_id == 지형_미정) return;

			func_일반좌표_정보표시(cursor_pos);
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void func_부대위치좌표_정보표시(pk::unit@ unit)
		{
			string unit_dir_info = 부대위치좌표_부대방향_표시 ? get_unit_dir_info(unit) : "";
			string pos_height_info = 부대위치좌표_지형고도_표시 ? get_pos_height_info(unit, unit.pos, /*부대위치좌표*/0) : "";
			string unit_pos_info = unit_dir_info + "\n" + pos_height_info;
			draw_text_on_screen(unit_pos_info, unit.pos);
		}

		void func_부대인접좌표_정보표시(pk::unit@ cursor_unit, pk::point neighbor_pos)
		{
			string atk_dir_info = 부대인접좌표_피격방향_표시 ? get_atk_dir_info(cursor_unit, neighbor_pos) : "";
			string pos_height_info = 부대인접좌표_지형고도_표시 ? get_pos_height_info(cursor_unit, neighbor_pos, /*부대인접좌표*/1) : "";
			string neighbor_pos_info = atk_dir_info + "\n" + pos_height_info;
			draw_text_on_screen(neighbor_pos_info, neighbor_pos);
		}

		void func_일반좌표_정보표시(pk::point cursor_pos)
		{
			string pos_height_info = 일반좌표_지형고도_표시 ? get_pos_height_info(null, cursor_pos, /*일반좌표*/2) : "";
			draw_text_on_screen("\n" + pos_height_info, cursor_pos);
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		string get_unit_dir_info(pk::unit@ unit) { return 글자_색상[부대위치좌표_부대방향_색상] + 부대방향_이름[unit.direction]; }

		string get_atk_dir_info(pk::unit@ unit, pk::point neighbor_pos)
		{
			int atk_dir = pk::get_direction(neighbor_pos, unit.pos);
			for (int atk_dir_index = 0; atk_dir_index < /*피격방향_끝*/4; atk_dir_index++)
			{
				if (피격방향_정보[unit.direction][atk_dir_index].find(atk_dir) < 0) continue;

				return 글자_색상[피격방향_색상[atk_dir_index]] + 피격방향_이름[atk_dir_index];
			}

			return "";
		}

		string get_pos_height_info(pk::unit@ unit, pk::point pos, int pos_index)
		{
			return "\x1b[0x(" + get_height_clr(unit, pos, pos_index) + get_pos_height(pos) + "\x1b[0x)\x1b[0x";
		}

		void draw_text_on_screen(string str, pk::point pos)
		{
			pk::draw_text(pk::encode(str), pk::hex_pos_to_screen_pos(pos, 2, 5.f), 0xffffffff, FONT_SMALL, 0xff000000);
		}

		string get_height_clr(pk::unit@ unit, pk::point pos, int pos_index)
		{
			if (pos_index == /*부대위치좌표*/0) return 글자_색상[부대위치좌표_지형고도_색상];
			else if (pos_index == /*부대인접좌표*/1)
			{
				if (get_pos_height(pos) > get_pos_height(unit.pos)) return 글자_색상[부대인접좌표_지형고도_색상_높음];
				else if (get_pos_height(pos) < get_pos_height(unit.pos)) return 글자_색상[부대인접좌표_지형고도_색상_낮음];
				else if (get_pos_height(pos) == get_pos_height(unit.pos)) return 글자_색상[부대인접좌표_지형고도_색상_같음];
			}
			else if (pos_index == /*일반좌표*/2) return 글자_색상[일반좌표_지형고도_색상];

			return "";
		}

		int get_pos_height(pk::point pos) { return pk::get_height_map(pk::hex_pos_to_height_map_pos(pos) + 2).height; }

		bool is_normal_game() { return (!pk::is_campaign() and !pk::is_mission_running()); }

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	}

	Main main;
}
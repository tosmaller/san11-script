// ## 2020/08/06 ##
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
	제르나	- 공방		| 18.11.22	| 초기생성

*/

/*
*** Guide ***
1. mode 선택	 : 상단부 set - 도시, 관문, 항구의 모드 설정 (주석)
2. mode 1	 : 하단부 set_data() - 도시 각각의 data 입력 (주석)
   mode 2, 3 : 상단부 set - mode에 맞는 변수 설정 (주석)
*/

namespace 거점내구
{
	class Main
	{
		// ----- Set ----- //
		//건물
		pk::building@ building_;

		//도시
		int		city_hp_mode = 2;			// 0:미사용, 1:사용(개별), 2:사용(배율), 3:사용(증가)
		float	city_hp_rate = 2;			// mode 2 - 전 도시 적용 배율 ex) 1.2
		int		city_hp_rise = 2000;		// mode 3 - 전 도시 증가 수치 ex) 1000
		pk::city@ city_;

		//관문
		int		gate_hp_mode = 2;			// 0:미사용, 1:사용(개별), 2:사용(배율), 3:사용(증가)
		float	gate_hp_rate = 2;			// mode 2 - 전 관문 적용 배율 ex) 1.2
		int		gate_hp_rise = 2000;		// mode 3 - 전 관문 증가 수치 ex) 1000
		pk::gate@ gate_;

		//항구
		int		port_hp_mode = 2;			// 0:미사용, 1:사용(개별), 2:사용(배율), 3:사용(증가)
		float	port_hp_rate = 2;			// mode 2 - 전 항구 적용 배율 ex) 1.2
		int		port_hp_rise = 2000;		// mode 3 - 전 항구 증가 수치 ex) 1000
		pk::port@ port_;



		// ----- act ----- //
		Main()
		{
			pk::bind(102, pk::trigger102_t(callback));
		}

		void callback()
		{
			//시나리오 시작여부 확인
			if (pk::get_scenario().loaded) return;

			//사용여부 확인
			if (city_hp_mode + gate_hp_mode + port_hp_mode == 0) return;

			//update
			update_data();

			return;
		}

		// ----- update ----- //
		// 도시
		void update_data()
		{
			//Mode 2, 3
			//도시
			if (city_hp_mode == 2 or city_hp_mode == 3)
			{
				for (int i = 건물_도시시작; i < 城市_末; i++)
				{
					int city_num = i;
					@city_ = pk::get_city(city_num);
					@building_ = pk::get_building(i);
					if (city_hp_mode == 2)
					{
						city_.max_hp = int(city_.max_hp * city_hp_rate);
						building_.hp = int(building_.hp * city_hp_rate);
					}
					if (city_hp_mode == 3)
					{
						city_.max_hp = city_.max_hp + city_hp_rise;
						building_.hp = building_.hp + city_hp_rise;
					}
					city_.update();
					building_.update();
				}
			}

			//관문
			if (gate_hp_mode == 2 or gate_hp_mode == 3)
			{
<<<<<<< HEAD
				for (int j = 건물_관문시작; j < 건물_관문끝; j++)
				{
					int gate_num = j - 건물_관문시작;
=======
				for (int j = 建筑_关卡开始; j < 건물_관문끝; j++)
				{
					int gate_num = j - 建筑_关卡开始;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					@gate_ = pk::get_gate(gate_num);
					@building_ = pk::get_building(j);
					if (gate_hp_mode == 2)
					{
						gate_.max_hp = int(gate_.max_hp * gate_hp_rate);
						building_.hp = int(building_.hp * gate_hp_rate);
					}
					if (gate_hp_mode == 3)
					{
						gate_.max_hp = gate_.max_hp + gate_hp_rise;
						building_.hp = building_.hp + gate_hp_rise;
					}
					gate_.update();
					building_.update();
				}
			}

			//항구
			if (port_hp_mode == 2 or port_hp_mode == 3)
			{
<<<<<<< HEAD
				for (int k = 건물_항구시작; k < 건물_항구끝; k++)
=======
				for (int k = 건물_항구시작; k < 建筑_港口末; k++)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				{
					int port_num = k - 건물_항구시작;
					@port_ = pk::get_port(port_num);
					@building_ = pk::get_building(k);
					if (port_hp_mode == 2)
					{
						port_.max_hp = int(port_.max_hp * port_hp_rate);
						building_.hp = int(building_.hp * port_hp_rate);
					}
					if (port_hp_mode == 3)
					{
						port_.max_hp = port_.max_hp + port_hp_rise;
						building_.hp = building_.hp + port_hp_rise;
					}
					port_.update();
					building_.update();
				}
			}


			//Mode 1
			if (city_hp_mode == 1 or gate_hp_mode == 1 or port_hp_mode == 1)
			{
				array<array<int>> arr_data_get;
				arr_data_get = set_data();
				int building_num;
				int building_hp;

				for (int l = 0; l < int(arr_data_get.length); l++)
				{
					building_num = arr_data_get[l][0];
					building_hp = arr_data_get[l][1];

					//도시
					if (building_num >= 건물_도시시작 and building_num < 城市_末 and city_hp_mode == 1)
					{
						int city_num = building_num;
						@city_ = pk::get_city(city_num);
						@building_ = pk::get_building(building_num);
						city_.max_hp = building_hp;
						building_.hp = building_hp;
						city_.update();
						building_.update();
					}

					//관문
<<<<<<< HEAD
					if (building_num >= 건물_관문시작 and building_num < 건물_관문끝 and gate_hp_mode == 1)
					{
						int gate_num = building_num - 건물_관문시작;
=======
					if (building_num >= 建筑_关卡开始 and building_num < 건물_관문끝 and gate_hp_mode == 1)
					{
						int gate_num = building_num - 建筑_关卡开始;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
						@gate_ = pk::get_gate(gate_num);
						@building_ = pk::get_building(building_num);
						gate_.max_hp = building_hp;
						building_.hp = building_hp;
						gate_.update();
						building_.update();
					}

					//항구
<<<<<<< HEAD
					if (building_num >= 건물_항구시작 and building_num < 건물_항구끝 and port_hp_mode == 1)
=======
					if (building_num >= 건물_항구시작 and building_num < 建筑_港口末 and port_hp_mode == 1)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					{
						int port_num = building_num - 건물_항구시작;
						@port_ = pk::get_port(port_num);
						@building_ = pk::get_building(building_num);
						port_.max_hp = building_hp;
						building_.hp = building_hp;
						port_.update();
						building_.update();
					}
				}
			}
		}

		// ----- set data ----- //
		array<array<int>> set_data()
		{
			array<array<int>> arr_data_set;

			// *** 양식 : {거점번호, 최대내구}
			// *** 거점번호는 아래 주석 참조
			/*
			arr_data_set =
			{
				{0, 4500},
				{42, 8000},
				{52, 3000}
			};
			*/

			return arr_data_set;
		}
	}

	Main main;
}

/*
*** 거점번호 ***
<도시>
양평 = 0; 북평 = 1; 계 = 2; 남피 = 3; 평원 = 4; 진양 = 5; 업 = 6; 북해 = 7; 하비 = 8; 소패 = 9;
수춘 = 10; 복양 = 11; 진류 = 12; 허창 = 13; 여남 = 14; 낙양 = 15; 완 = 16; 장안 = 17; 상용 = 18; 안정 = 19;
천수 = 20; 무위 = 21; 건업 = 22; 오 = 23; 회계 = 24; 여강 = 25; 시상 = 26; 강하 = 27; 신야 = 28; 양양 = 29;
강릉 = 30; 장사 = 31; 무릉 = 32; 계양 = 33; 영릉 = 34; 영안 = 35; 한중 = 36; 자동 = 37; 강주 = 38; 성도 = 39;
건녕 = 40; 운남 = 41;

<관문>
호관 = 42; 호로관 = 43;동관 = 44; 함곡관 = 45; 무관 = 46; 양평관 = 47; 검각 = 48; 가맹관 = 49;부수관 = 50; 면죽관 = 51;

<항구>
안평항 = 52; 고당항 = 53; 서하항 = 54; 백마항 = 55; 창양항 = 56; 임제항 = 57; 해릉항 = 58; 강도항 = 59; 유수항 = 60; 돈구항 = 61;
관도항 = 62; 맹진항 = 63; 해현항 = 64; 신풍항 = 65; 하양항 = 66; 방릉항 = 67; 무호항 = 68; 호림항 = 69; 곡아항 = 70; 구장항 = 71;
환구항 = 72; 구강항 = 73; 육구항 = 74; 파양항 = 75; 노릉항 = 76; 하구항 = 77; 호양항 = 78; 중려항 = 79; 오림항 = 80; 한진항 = 81;
강진항 = 82; 나현항 = 83; 동정항 = 84; 공안항 = 85; 무현항 = 86;
*/
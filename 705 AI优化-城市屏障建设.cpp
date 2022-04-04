// ## 2020/12/31 #江东新风# 简单翻译 ##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@내용: 일정확률로 AI 세력의 도시방벽 자동 건설
@기타: 기마책사님의 도시방벽건설을 참고하여 제작한 SCRIPT

*/

namespace 城市屏障建设_AI
{


	// ================ CUSTOMIZE ================

	const int 屏障建设距离 = 3;          // 도시로부터 떨어진 거리 (최소거리=3)

	const int 城市屏障建设_发动时期 = 1;        // 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일,  4: 랜덤 턴
	const int 城市屏障建设_发动概率_城市 = 30;    // 0,1,2,3 설정일 경우 발동확률
	const int 城市屏障建设_发动概率_旬 = 10;     // 4 설정일 경우 발동확률

// ===========================================


	class Main
	{

		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{

			pk::array<pk::force@> force_list = pk::list_to_array(pk::get_force_list());

			for (int i = 0; i < int(force_list.length); i++)
			{

				pk::force@ force = force_list[i];

				if (城市屏障建设_发动时期 == 0)
					build_defence_wall(force);  // 매턴
				else if (城市屏障建设_发动时期 == 1 and (pk::get_day() == 1))
					build_defence_wall(force);  // 매월 1일
				else if (城市屏障建设_发动时期 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
					build_defence_wall(force);  // 매분기 첫달이면서 1일
				else if (城市屏障建设_发动时期 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
					build_defence_wall(force);  // 매년 1월 1일
				else if (城市屏障建设_发动时期 == 4 and pk::rand_bool(城市屏障建设_发动概率_旬))
					build_defence_wall(force);  // 랜덤 턴에서 발동
			}

		}

		void build_defence_wall(pk::force@ force)
		{

			int facility_id;
			int building_id;
			int force_id;
			pk::point building_pos;
			array<pk::point> arr;

			pk::array<pk::city@> city_list = pk::list_to_array(pk::get_city_list(force));

			for (int i = 0; i < int(city_list.length); i++)
			{

				pk::city@ city_ = city_list[i];

				pk::building@ building_ = pk::city_to_building(city_);

				building_id = building_.get_id();

				building_pos = building_.pos;

				force_id = building_.get_force_id();

				pk::force@ force0 = pk::get_force(force_id);

				if (pk::has_tech(force0, 기교_석조건축))
					facility_id = 시설_석벽;
				else
					facility_id = 시설_토루;

				if (building_.is_player())
					continue;

				if (pk::rand_bool(城市屏障建设_发动概率_城市))
				{

					int count_hex = 0;
					int range = pk::max(3, 屏障建设距离) + ((building_id < 城市_末) ? 1 : 0);
					arr = pk::ring(building_pos, range);
					for (int j = 0; j < int(arr.length); j++)
					{
						pk::point pos = arr[j];
						pk::hex@ hex = pk::get_hex(pos);

						// 6개 방향 비우기
						bool enabled_pos = true;
						for (int dir = 0; dir < 방향_끝; dir++)
						{
							pk::point chk_pos = pk::get_neighbor_pos(building_pos, dir, range);
							if (chk_pos == pos)
								enabled_pos = false;
						}

						if (!hex.has_building and !hex.has_unit and pk::is_enabled_terrain(hex.terrain) and enabled_pos and !check_develop_pos(pos)
							and building_id < 城市_末 and (hex.terrain <= 지형_습지 or hex.terrain == 지형_황무지))
						{
							count_hex += 1;

							pk::create_building(pk::get_facility(facility_id), pos, force_id);
							pk::building@ facility = pk::get_building(pos);
							pk::complete(facility);

						}

					}

				} // if


			} // for


		} // void build_defence_wall()


		bool check_develop_pos(pk::point pos_t)
		{

			auto list = pk::list_to_array(pk::get_city_list());

			for (int i = 0; i < int(list.length); i++)
			{

				pk::city@ city_ = list[i];

				for (int j = 0; j < int(city_.max_devs); j++)
				{
					if (city_.dev[j].pos == pos_t)
						return true;
				}

			}
			return false;
		}

	} // class 

	Main main;
}
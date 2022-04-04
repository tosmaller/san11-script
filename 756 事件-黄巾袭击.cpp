// ## 2022/03/21 # 江东新风 # 部队异常消除bug ##
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/08/19 # 江东新风 # 袭击报告加入据点名称提示 ##
// ## 2020/12/12 # 江东新风 # 修复Signed/Unsigned mismatch ##
// ## 2020/10/27 # 江东新风 # 新特技——戍防##
// ## 2020/09/30 # 氕氘氚 # 常量名简体化，难度调整 ##
/*
@제작자: HoneyBee
@설명: 일정확률로 황건적 기습부대 출현 (도시 근처에서 발생)
@기타: 난이도 상승 목적을 위해 제작한 SCRIPT

*/

namespace 황건_습격
{

	// ================ CUSTOMIZE ================

	const int 部队数 = 4; 			 	 // 황건적 부대 생성 수치 (기본 5부대 생성 설정)
	const int 兵力 = 8000;		 // 황건적 부대 병력 설정 (최대 14000 = 정동장군 관직과 동일)

	const int min_distance = 4;      // 황건적 부대 생성 최소 위치
	const int max_distance = 7;      // 황건적 부대 생성 최대 위치

	const int 黄巾袭击_发动时期 = 2;        // 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일
	const int 黄巾袭击_发动概率_基本 = 10;  // 基本的发动概率
	const int 黄巾袭击_发动概率_都市 = 30;    // 在发动的基础上，每座城市被袭击的概率

	const bool 黄巾袭击_仅对玩家 = true;         // true = 黄巾袭击_仅对玩家 적용, false = AI도 적용
	const bool 黄巾袭击_特技保护 = true;          // true = 특기 적용, false = 특기 어드밴티지 없음 (신산 or 통찰)

	const int 黄巾袭击_难度 = 0;	      // 0-33%, 1-50%, 2-75%, 3-100%

	const int 黄巾袭击_最少城市 = 3;       		 // 설정 도시수 3개 이하 세력에게는 발동 안함 (도시 4개 ~ 6개 사이일 때 발생)
	const int 黄巾袭击_最多城市 = 7;       		 // 설정 도시수 7개 이상 세력에게는 발동 안함 (강제 종료 ERROR 방지를 위한 설정)

	const int 黄巾袭击_保护月数 = 12;    // 몇개월이 지난 시점부터 황건적이 등장할지 (기본 12개월 설정)

// ===========================================


	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{
			if (黄巾袭击_发动时期 == 0)
				yellow_Set();  // 매턴
			else if (黄巾袭击_发动时期 == 1 and (pk::get_day() == 1))
				yellow_Set();  // 매월 1일
			else if (黄巾袭击_发动时期 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
				yellow_Set();  // 매분기 첫달이면서 1일
			else if (黄巾袭击_发动时期 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
				yellow_Set();  // 매년 1월 1일
		}

		void yellow_Set()
		{
			if (!pk::rand_bool(黄巾袭击_发动概率_基本)) return;
			auto list = pk::list_to_array(pk::get_city_list());

			for (int i = 0; i < int(list.length); i++)
			{

				pk::city@ city_ = list[i];

				pk::building@ building_ = pk::city_to_building(city_);

				pk::force@ force_ = pk::get_force(building_.get_force_id());

				pk::person@ bandit_0 = pk::get_person(무장_황건적);
				pk::person@ reporter_ = pk::get_person(무장_전령);
				pk::person@ taishu_ = pk::get_person(pk::get_taishu_id(building_));


				if (ch::has_skill(building_, 特技_戍防)) continue;

				if (黄巾袭击_仅对玩家 and !building_.is_player()) continue;

				if (!pk::is_player_controlled(building_)) continue;

				if (黄巾袭击_最少城市 >= pk::get_city_count(pk::get_force(building_.get_force_id()))) continue;

				if (黄巾袭击_最多城市 <= pk::get_city_count(pk::get_force(building_.get_force_id()))) continue;

				if (pk::get_elapsed_months() < 黄巾袭击_保护月数) continue;
				//pk::trace("满足月数条件：月数:" + pk::get_elapsed_months());
				if (黄巾袭击_特技保护 and ch::has_skill(building_, 특기_신산) or ch::has_skill(building_, 특기_통찰)) continue;


				// 세력이 [황건]일 경우 황건적이 습격하지 않음
				if (force_.kokugou == 국호_황건) continue;

				// 황건적 출신 인물이 [군주]일 경우 황건적이 습격하지 않음 
				if (pk::get_kunshu_id(building_) == 무장_장각 or pk::get_kunshu_id(building_) == 무장_장보 or pk::get_kunshu_id(building_) == 무장_장량
					or pk::get_kunshu_id(building_) == 무장_관해 or pk::get_kunshu_id(building_) == 무장_주창 or pk::get_kunshu_id(building_) == 무장_배원소
					or pk::get_kunshu_id(building_) == 무장_장만성 or pk::get_kunshu_id(building_) == 무장_정원지 or pk::get_kunshu_id(building_) == 무장_등무
					or pk::get_kunshu_id(building_) == 무장_유벽 or pk::get_kunshu_id(building_) == 무장_공도 or pk::get_kunshu_id(building_) == 무장_하의
					or pk::get_kunshu_id(building_) == 무장_한섬 or pk::get_kunshu_id(building_) == 무장_한충 or pk::get_kunshu_id(building_) == 무장_엄정
					or pk::get_kunshu_id(building_) == 무장_고승 or pk::get_kunshu_id(building_) == 무장_휴고 or pk::get_kunshu_id(building_) == 무장_손중
					or pk::get_kunshu_id(building_) == 무장_장연 or pk::get_kunshu_id(building_) == 무장_장개 or pk::get_kunshu_id(building_) == 413
					or pk::get_kunshu_id(building_) == 무장_파재 or pk::get_kunshu_id(building_) == 무장_변희 or pk::get_kunshu_id(building_) == 무장_요화)
					continue;


				if (pk::rand_bool(黄巾袭击_发动概率_都市) and (!pk::enemies_around(building_)))
				{

					pk::message_box(pk::encode(pk::format("报告主公, \x1b[17x黄巾军\x1b[0x已经打到\x1b[2x{}\x1b[0x城下了!",pk::decode(pk::get_name(building_)))), reporter_);
					pk::message_box(pk::encode("把城里的金子和粮食全都交出来!"), bandit_0);
					pk::message_box(pk::encode("不然我们马上打你一顿!"), bandit_0);

					array<string> answers =
					{
						pk::encode("  交  "),
						pk::encode(" 不 交 ")
					};

					int n = pk::choose(pk::encode("要把金和粮食交给黄巾军吗?"), answers);

					if (n == 0)
					{
						pk::message_box(pk::encode("我把钱粮都交给你们, 饶了我!"), taishu_);
						pk::message_box(pk::encode("哈哈哈!算你识相."), bandit_0);
						pk::message_box(pk::encode("这次就放过你!"), bandit_0);

						switch (黄巾袭击_难度)
						{
						case 0:
							pk::add_gold(city_, -city_.gold / 3, true);
							pk::add_food(city_, -city_.food / 3, true);
							break;
						case 1:
							pk::add_gold(city_, -city_.gold / 2, true);
							pk::add_food(city_, -city_.food / 2, true);
							break;
						case 2:
							pk::add_gold(city_, -city_.gold / 4 * 3, true);
							pk::add_food(city_, -city_.food / 4 * 3, true);
							break;
						case 3:
							pk::add_gold(city_, -city_.gold, true);
							pk::add_food(city_, -city_.food, true);
							break;
						}

					} // n == 0

					else if (n == 1)
					{

						pk::message_box(pk::encode("竟敢在此放肆!"), taishu_);
						pk::message_box(pk::encode("让你们有来无回!"), taishu_);
						pk::message_box(pk::encode("希望你不要后悔!"), bandit_0);


						for (int j = 0; j < 部队数; j++)
						{

							pk::person@ yellow_turban = pk::create_bandit(pk::get_person(무장_황건적));

							if (!pk::is_alive(yellow_turban)) continue;

							pk::set_district(yellow_turban, 세력_도적);
							yellow_turban.mibun = 身份_一般;
							yellow_turban.loyalty = 150;
							yellow_turban.rank = 관직_정동장군;
							yellow_turban.birth = pk::get_year() - 30;
							yellow_turban.death = pk::min(353, yellow_turban.birth + 99);
							yellow_turban.appearance = pk::get_year() - 10;

							// 능력치 수정.
							yellow_turban.base_stat[武将能力_统率] = 70;
							yellow_turban.base_stat[武将能力_武力] = 70;
							yellow_turban.base_stat[武将能力_智力] = 40;
							yellow_turban.base_stat[武将能力_政治] = 40;
							yellow_turban.base_stat[武将能力_魅力] = 40;
							yellow_turban.tekisei[병종_창병] = 적성_S;

							// 능력치 반영.
							yellow_turban.update();

							int gold = 2000;
							int food = int(兵力 * 1.3f);


							// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
							pk::point emptyPos = getEmptyPos(building_);

							if (!pk::is_valid_pos(emptyPos) or !building_.is_player()) break;

							pk::unit@ unit = pk::create_unit(building_, yellow_turban, null, null, 兵力, 兵器_枪, 兵器_走舸, gold, food, emptyPos);

							// 부대 생성이 실패한 경우 무장 삭제
							if (!pk::is_alive(unit))
							{
								pk::kill(unit);
								continue;
							}

							// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
							unit.energy = 100;

							pk::set_order(unit, 부대임무_정복, building_.get_pos());


							// 임무 실행(부대를 생성한 턴에 행동을 시작하지 않는다면 생략해도 됨.)
							if (!pk::run_order(unit))
								// 임무에 문제가 있다면 부대 삭제.
								pk::kill(unit);

						} // for

					} // n == 1		

				} // if

			} // for

		} // yellow_Set()


		pk::point getEmptyPos(pk::building@ building_)
		{
			auto arr = pk::range(building_.get_pos(), min_distance, max_distance);
			for (int i = 0; i < int(arr.length); i++)
			{
				pk::hex@ hex = pk::get_hex(arr[i]);
				int terrain_id = hex.terrain;

				if (!hex.has_building and !hex.has_unit
					and pk::is_valid_terrain_id(terrain_id)
					and pk::is_enabled_terrain(terrain_id)
					and terrain_id != 지형_산)
					return arr[i];

			}
			return -1;
		}

	} // class Main

	Main main;

} // namespace
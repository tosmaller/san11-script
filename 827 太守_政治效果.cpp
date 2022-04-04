// ## 2022/03/27 # 江东新风 # 默认关闭功能 ##
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/01/13 #江东新风# 改良算法，修复错误 ##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/11/12 # messi # 翻译修正##
// ## 2020/10/31 # 江东新风 # 同步honeybee大神的更新##
/*
@제작자: HoneyBee
@설명: 태수의 [정치, 지력]이 동시에 높거나, [정치, 매력]이 동시에 높은 경우에 발동하는 효과


※ 효과 발동조건

(1) 태수의 정치, 지력이 동시에 80대 이상인 경우 도시 [병력, 병장기, 기력] 상승 / 도시 근처 적군 [병력, 기력] 감소
(2) 태수의 정치, 매력이 동시에 80대 이상인 경우 도시 [금, 군량, 치안] 상승 / 도시 근처 적군 [금, 군량] 감소


@说明:太守的[政治，智力]同时高，或[政治，魅力]同时高的情况下发动效果

※效果发动条件

(1)太守的政治、智力同时在80以上的情况的城市[兵力、兵器、气力]上升/城市附近敌人[兵力、气力]减少
(2)太守的政治，魅力同时在80以上的情况的城市[金，兵粮，治安]上升/城市附近敌人[金，兵粮]减少
*/

namespace 太守_政治效果
{

	// ================ CUSTOMIZE ================
	const bool 开启太守政治效果 = false;//默认关闭，和军师之战部分冲突
	const int 发动时期 = 2;               					// 1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일
	const int 发动时期_概率 = 15;           					// 발동 확률 설정 (100은 100% 확률로 발생한다는 의미) 

	const int energy_限制条件 = 10;  						// 기력 10 이하 적군 부대는 기력 감소 효과가 적용되지 않음 
	const int troops_限制条件 = 500; 						// 병력 500 이하 적군 부대는 병력 감소 효과가 적용되지 않음 


	// 태수의 정치, 지력이 동시에 높은 경우 증가하는 [병력, 병장기, 기력] 수치
	const array<int> 兵力_上升 = { 4000, 5000, 6000 };		// 증가하는 병력 수치 
	const array<int> 兵器_上升 = { 4000, 5000, 6000 };		// 증가하는 병장기 수치 
	const array<int> 气力_上升 = { 10, 20, 30 };				// 증가하는 기력 수치


	// 태수의 정치, 매력이 동시에 높은 경우 증가하는 [금, 군량, 치안] 수치
	const array<int> 金_上升 = { 1000, 2000, 3000 };			// 증가하는 금 수치 
	const array<int> 兵粮_上升 = { 2000, 4000, 6000 };		// 증가하는 군량 수치 
	const array<int> 治安_上升 = { 10, 20, 30 };				// 증가하는 치안 수치


	// 태수의 정치, 지력이 동시에 높은 경우 도시 주위에 적 부대가 있을 경우 적군의 병력, 기력 감소 
	const array<int> 兵力_减少 = { 500, 700, 900 };		    // 감소하는 병력 수치 
	const array<int> 气力_减少 = { 10, 20, 30 };				// 감소하는 기력 수치 


	// 태수의 정치, 매력이 동시에 높은 경우 도시 주위에 적 부대가 있을 경우 적군의 금, 군량 감소 
	const array<int> 金_减少 = { 300, 400, 500 };		    // 감소하는 금 수치 
	const array<int> 兵粮_减少 = { 1000, 1500, 2000 };		// 감소하는 군량 수치 


	const int 政治_条件 = 80;      						// 정치 80 이상
	const int 智力_条件 = 80;      						// 지력 80 이상
	const int 魅力_条件 = 80;      						// 매력 80 이상

	const int min_范围 = 1;      							// 효과를 받는 최소 범위 
	const int max_范围 = 3; 								// 효과를 받는 최대 범위 


// ===========================================


	class Main
	{
		Main()
		{
			if(开启太守政治效果) pk::bind(107, pk::trigger107_t(callback));
		}

		pk::random random(pk::rand());

		pk::building@ building;
		pk::city@ city;

		pk::person@ taishu;
		pk::point building_pos;

		pk::unit@ dst;
		pk::person@ dst_leader;

		bool flag_정치지력 = false;
		bool flag_정치매력 = false;

		void callback()
		{
			if (发动时期 == 1 and (pk::get_day() == 1))
			{
				political_effect();  // 매월 1일
			}
			else if (发动时期 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
			{
				political_effect();  // 매분기 첫달이면서 1일
			}
			else if (发动时期 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
			{
				political_effect();  // 매년 1월 1일
			}
		}


		void political_effect()
		{

			//auto building_list = pk::list_to_array(pk::get_building_list());

			for (int i = 0; i < 据点_末; i++)
			{

				@building = pk::get_building(i);
				if (i < 城市_末) @city = pk::building_to_city(building);
				@taishu = pk::get_person(pk::get_taishu_id(building));

				if (pk::rand_bool(发动时期_概率) and pk::is_alive(taishu))
				{

					// [태수의 정치 80 이상, 지력 80 이상]
					if (((taishu.stat[武将能力_政治] >= 政治_条件) and (taishu.stat[武将能力_智力] >= 智力_条件)))
					{
						ch::add_troops(building, 兵力_上升[pk::rand(3)], true);
						pk::add_weapon_amount(building, 兵器_枪, 兵器_上升[pk::rand(3)], true);
						pk::add_weapon_amount(building, 兵器_戟, 兵器_上升[pk::rand(3)], true);
						pk::add_weapon_amount(building, 兵器_弩, 兵器_上升[pk::rand(3)], true);
						pk::add_weapon_amount(building, 兵器_战马, 兵器_上升[pk::rand(3)], true);
						pk::add_energy(building, 气力_上升[pk::rand(3)], true);

						flag_정치지력 = true;
					}

					// [태수의 정치 80 이상, 매력 80 이상]
					if (((taishu.stat[武将能力_政治] >= 政治_条件) and (taishu.stat[武将能力_魅力] >= 魅力_条件)))
					{
						pk::add_gold(building, 金_上升[pk::rand(3)], true);
						pk::add_food(building, 兵粮_上升[pk::rand(3)], true);
						if (i < 城市_末) pk::add_public_order(city, 治安_上升[pk::rand(3)], true);

						flag_정치매력 = true;
					}

					set_enemy_unit(building, building.get_pos());

				} // if (发动时期_概率)

			} // for 

		} // political_effect()


		void set_enemy_unit(pk::building@ building, pk::point building_pos)
		{

			array<pk::point> arr = pk::range(building_pos, min_范围, max_范围);

			for (int j = 0; j < int(arr.length); j++)
			{

				@dst = pk::get_unit(arr[j]);

				if (dst is null or !pk::is_enemy(building, dst)) continue;

				@dst_leader = pk::get_person(dst.leader);

				// [태수의 정치 80 이상, 지력 80 이상]
				if (((taishu.stat[武将能力_政治] >= 政治_条件) and (taishu.stat[武将能力_智力] >= 智力_条件)))
				{
					if (energy_限制条件 < dst.energy)
					{
						pk::add_energy(dst, -气力_减少[pk::rand(3)], true);

					}

					if (troops_限制条件 < dst.troops)
					{
						ch::add_troops(dst, -兵力_减少[pk::rand(3)], true);
					}

					pk::say(pk::encode("军心不稳了.."), dst_leader);
				}

				// [태수의 정치 80 이상, 매력 80 이상]
				if (((taishu.stat[武将能力_政治] >= 政治_条件) and (taishu.stat[武将能力_魅力] >= 魅力_条件)))
				{
					pk::add_gold(dst, -金_减少[pk::rand(3)], true);
					pk::add_food(dst, -兵粮_减少[pk::rand(3)], true);

					pk::say(pk::encode("什么，金粮竟然消失了!"), dst_leader);
				}

			} // for 

			if (flag_정치지력 and pk::is_in_screen(building.get_pos()))
			{
				pk::say(pk::encode("迷惑于我的政治鼓动术吧!"), taishu);
				flag_정치지력 = false;
			}

			if (flag_정치매력 and pk::is_in_screen(building.get_pos()))
			{
				pk::say(pk::encode("迷惑于我的政治鼓动术吧!"), taishu);
				flag_정치매력 = false;
			}

		} // set_enemy_unit(pk::building@ building, pk::point building_pos)


	} // class Main

	Main main;

} // namespace
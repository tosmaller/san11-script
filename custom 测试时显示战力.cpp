<<<<<<< HEAD
/***CUSTOM
@name 测试时显示战力
@class 测试
@author 江东新风
@contact 
@version 1.0
@date 2020/08/14
@desc 看海时了解战力变化,只显示排名前5势力，按城池数排序
@notice 
***/
// ## 2020/08/14 # 江东新风 # 测试时了解战力变化,只显示排名前5势力，按城池数排序 ##
=======
﻿// ## 2020/08/14 # 江东新风 # 测试时了解战力变化,只显示排名前5势力，按城池数排序 ##
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
// @@ 2020/08/13 @ 江东新风 @@

namespace 测试时显示战力
{
	//---------------------------------------------------------------------------------------
	const bool 显示边匡 = false;

	//---------------------------------------------------------------------------------------

	class Main
	{
		array<array<int>> all_force_stats;
		Main()
		{
			//跑图时显示势力战力
			pk::bind(107, pk::trigger107_t(on_new_day));
			pk::bind(120, pk::trigger120_t(func_信息显示_势力战力信息));


		}


		void func_信息显示_势力战力信息()
		{
			//只在观看ai作战时显示
			if (pk::get_player_count() > 0) return;

			if (all_force_stats.length < 1) return;

			int middle = int(pk::get_resolution().width) / 2;
			int left = middle - 200;
			int right = middle + 200;
			int top = 5;
			int bottom = top + 80;

			for (int i = 0; i < int(all_force_stats.length); i++)
			{
				// 显示信息
				pk::force@ force0 = pk::get_force(all_force_stats[i][0]);
				string force_name = pk::decode(pk::get_name(force0));
				string info = pk::format("势力(\x1b[1x{}\x1b[0x)", force_name);
				string info城市 = pk::format("城市数:\x1b[2x{}\x1b[0x", all_force_stats[i][1]);
				string info武将 = pk::format("武将数:\x1b[2x{}\x1b[0x", all_force_stats[i][2]);
				string info兵力 = pk::format("兵力数:\x1b[2x{}\x1b[0x", all_force_stats[i][3]);
				pk::draw_text(pk::encode(info), pk::point(left + 5, top + 5 + 25 * i), 0xffffffff, FONT_BIG, 0xff000000);
				pk::draw_text(pk::encode(info城市), pk::point(left + 155, top + 5 + 25 * i), 0xffffffff, FONT_BIG, 0xff000000);
				pk::draw_text(pk::encode(info武将), pk::point(left + 285, top + 5 + 25 * i), 0xffffffff, FONT_BIG, 0xff000000);
				pk::draw_text(pk::encode(info兵力), pk::point(left + 435, top + 5 + 25 * i), 0xffffffff, FONT_BIG, 0xff000000);
			}

		}

		void on_new_day()
		{
			if (pk::get_player_count() > 0) return;
			all_force_stats = get_force_stats();
		}

		array<array<int>> get_force_stats()
		{
			array<array<int>> force_stats;
			pk::list<pk::force@> force_list = pk::get_force_list(false);
			force_list.sort(function(a, b)
			{
				int stat_a = pk::get_city_count(a);
				int stat_b = pk::get_city_count(b);
				return (stat_a > stat_b);     // 지휘-능력순
			});
			int end = pk::min(5, force_list.count);
			for (int i = 0; i < end; i++)
			{
				pk::force@ force0 = force_list[i];
				array<int> per_force_stats;// = (-1, -1, -1, -1);
					//string c_force_name = pk::decode(pk::get_name(pk::get_force(i)));
				int c_city_count = pk::get_city_count(force0);
				int c_troops = pk::get_troops(force0);
				//int c_gold = ch::get_force_gold(force);
				//int c_food = ch::get_force_food(force);
				//int c_weapon =  ch::get_force_weapon(force0);
				int c_person_count = ch::get_force_person_count(force0);
				//array<int> temp = {i,c_city_count,c_person_count,c_troops,c_weapon};
				per_force_stats = { force0.get_force_id(),c_city_count,c_person_count,c_troops };
				force_stats.insertLast(per_force_stats);

			}
			return force_stats;
		}
	}

	Main main;
}
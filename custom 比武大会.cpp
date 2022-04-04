// ## 2021/07/28 # 白马叔叔 # 添加奖励提示、选人防止意外取消  ##
// @@ 2021/01/21 @ 白马叔叔 @@

namespace 比武大会
{
    //---------------------------------------------------------------------------------------
    const int fail_gold = 200; //场败者金奖励
	const int succeed_gold = 500; //场胜者金奖励
	const int winner_gold = 2000; //冠军金奖励
	const int winner_tp = 200; //冠军tp奖励

    //---------------------------------------------------------------------------------------

    class Main
    {

        Main()
        {
			pk::bind(107, pk::trigger107_t(callback));			
		}
		
		//事件
		void callback()
		{
			//比武大会日期 （12月21日）
			//if (pk::rand_bool(100))  //测试
			if (pk::get_month() == 12 and pk::get_day() == 21)
			{
				pk::scene(pk::scene_t(scene_tournament));
			}
		}

		void scene_tournament()
		{
			if (pk::get_player_count() > 1)  return;  //多势力直接返回
			
			pk::list<pk::force@> force_list = pk::get_force_list(false);
			if (force_list.count == 1) return;  //只有一个返回
			int force_count = force_list.count;
			
			force_list.sort(function(a, b)
			{
				int stat_a = pk::get_city_count(a);
				int stat_b = pk::get_city_count(b);
				return (stat_a > stat_b);     // 지휘-능력순
			});	
			
			if (pk::get_player_count() == 0)
			{
				if (force_list.count < 3)
					pk::history_log(0, -1, pk::encode("因与会势力数不足,此次年度比武大会未举办."));
				else
				{
					pk::force@ force0_winner;
					string force0_name;
					pk::building@ building0;
					
					if (pk::rand_bool(50))  @ force0_winner = force_list[0];  //50概率为第一名最终获胜
					else if (pk::rand_bool(80))  @ force0_winner = force_list[1];  //40概率第二名获胜
					else  @ force0_winner = force_list[2];  //第三名获胜
					force0_name = pk::decode(pk::get_name(force0_winner));
					@ building0 = pk::get_building(pk::get_person(pk::get_kunshu_id(force0_winner)).service);
					//pk::message_box(pk::encode(pk::format("在年度比武大会中,各方激烈角逐.\n\x1b[1x{}军\x1b[0x取得了最后的胜利!", force0_name))); 
					pk::add_gold(building0, winner_gold, true);
					ch::add_tp(force0_winner, winner_tp, force0_winner.get_pos());
					building0.update();
					pk::history_log(force0_winner.get_pos(), force0_winner.color, pk::encode(pk::format("\x1b[1x{}军\x1b[0x在比武大会中取胜.", force0_name)));
				}
				

			}
				
			else
			{
				pk::force @ force_;
				pk::list<pk::force@> force_list1;
				for (int i = 0; i < force_count; i++)
				{
					if (pk::is_player_controlled(force_list[i]))
					{
						@force_ = force_list[i];
						continue;  //跳过玩家势力，只取敌方势力
					}
						
					force_list1.add(force_list[i]);
				}
				int winner_final = 0;
				pk::message_box(pk::encode("主公,今年的比武大会即将开始."), pk::get_person(무장_문관));
				bool	confirm = pk::yes_no(pk::encode("是否参加?")); 
				if (confirm) 
				{
					pk::force@ force_winner;
					string winner_name;
					pk::building@ winner_building;
					int end = pk::min(7, force_list1.count);	
					if (end >= 4)
					{
						array<int> lots_ = {-1, -1, -1, -1, -1, -1, -1, -1}; //3轮pk生成初始签
						array<int> forceid_1 = {-1, -1, -1, -1, -1, -1, -1, -1};  //势力id轮次
						array<string> forcename_1 = {"轮空", "轮空", "轮空", "轮空", "轮空", "轮空", "轮空", "轮空"};

						//-1为该轮轮空
						for (int j = 0; j <= end; j++)
						{
							lots_[j] = j; // ==end为玩家签
						}
						//打乱签，5次
						for (int k = 0; k < 5; k++)
						{
							int random_1 = pk::rand(8);
							int random_2 = pk::rand(8);
							int cache_0 = lots_[random_1];
							lots_[random_1] = lots_[random_2];
							lots_[random_2] = cache_0;
						}
						//势力id赋值
						for (int m = 0; m < 8; m++)
						{
							if (lots_[m] == -1)   continue; //轮空
							forceid_1[m] = lots_[m] == end ? force_.get_id() : force_list1[lots_[m]].get_id();
						}
						//势力名称赋值
						for (int l = 0; l < 8; l++)
						{
							if (lots_[l] == -1)   continue; //轮空
							forcename_1[l] = lots_[l] == end ? pk::decode(pk::get_name(force_)) + "军" : pk::decode(pk::get_name(force_list1[lots_[l]])) + "军";  //势力名
						}
						pk::message_box(pk::encode(pk::format("首轮 \x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x , \x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x ,\n     \x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x , \x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x.", forcename_1[0], forcename_1[1], forcename_1[2], forcename_1[3], forcename_1[4], forcename_1[5], forcename_1[6], forcename_1[7])));
						pk::message_box(pk::encode(pk::format("第一场\x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x.", forcename_1[0], forcename_1[1])));
						int winner_1 = getWinner(forceid_1[0], forceid_1[1]);
						
						pk::message_box(pk::encode(pk::format("第二场\x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x.", forcename_1[2], forcename_1[3])));
						int winner_2 = getWinner(forceid_1[2], forceid_1[3]);
						
						pk::message_box(pk::encode(pk::format("第三场\x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x.", forcename_1[4], forcename_1[5])));
						int winner_3 = getWinner(forceid_1[4], forceid_1[5]);
						
						pk::message_box(pk::encode(pk::format("第四场\x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x.", forcename_1[6], forcename_1[7])));
						int winner_4 = getWinner(forceid_1[6], forceid_1[7]);
						
						string winner_1_name = winner_1 == -1 ? "轮空" : pk::decode(pk::get_name(pk::get_force(winner_1))) + "军";
						string winner_2_name = winner_2 == -1 ? "轮空" : pk::decode(pk::get_name(pk::get_force(winner_2))) + "军";
						string winner_3_name = winner_3 == -1 ? "轮空" : pk::decode(pk::get_name(pk::get_force(winner_3))) + "军";
						string winner_4_name = winner_4 == -1 ? "轮空" : pk::decode(pk::get_name(pk::get_force(winner_4))) + "军";
						
						pk::message_box(pk::encode(pk::format("次轮 \x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x , \x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x .", winner_1_name, winner_2_name, winner_3_name, winner_4_name)));
						
						pk::message_box(pk::encode(pk::format("第一场 \x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x .", winner_1_name, winner_2_name)));
						int winner_5 = getWinner(winner_1, winner_2);
						
						pk::message_box(pk::encode(pk::format("第二场 \x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x .", winner_3_name, winner_4_name)));
						int winner_6 = getWinner(winner_3, winner_4);
						
						pk::message_box(pk::encode(pk::format("决胜轮 \x1b[1x{}军\x1b[0x 对 \x1b[1x{}军\x1b[0x .", pk::decode(pk::get_name(pk::get_force(winner_5))), pk::decode(pk::get_name(pk::get_force(winner_6))))));
						winner_final = getWinner(winner_5, winner_6);

					}
					else if (end >= 2)
					{
						array<int> lots_ = {-1, -1, -1, -1}; //3轮pk生成初始签
						array<int> forceid_1 = {-1, -1, -1, -1};  //势力id轮次
						array<string> forcename_1 = {"轮空", "轮空", "轮空", "轮空"};
						
						//-1为该轮轮空
						for (int j = 0; j <= end; j++)
						{
							lots_[j] = j; // ==end为玩家签
						}
						//打乱签，3次
						for (int k = 0; k < 3; k++)
						{
							int random_1 = pk::rand(4);
							int random_2 = pk::rand(4);
							int cache_0 = lots_[random_1];
							lots_[random_1] = lots_[random_2];
							lots_[random_2] = cache_0;
						}
						//势力id赋值
						for (int m = 0; m < 4; m++)
						{
							if (lots_[m] == -1)   continue; //轮空
							forceid_1[m] = lots_[m] == end ? force_.get_id() : force_list1[lots_[m]].get_id();
						}
						//势力名称赋值
						for (int l = 0; l < 4; l++)
						{
							if (lots_[l] == -1)   continue; //轮空
							forcename_1[l] = lots_[l] == end ? pk::decode(pk::get_name(force_)) + "军" : pk::decode(pk::get_name(force_list1[lots_[l]])) + "军";  //势力名
						}
						pk::message_box(pk::encode(pk::format("首轮 \x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x , \x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x .", forcename_1[0], forcename_1[1], forcename_1[2], forcename_1[3])));
						pk::message_box(pk::encode(pk::format("第一场\x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x.", forcename_1[0], forcename_1[1])));
						int winner_1 = getWinner(forceid_1[0], forceid_1[1]);
						
						pk::message_box(pk::encode(pk::format("第二场\x1b[1x{}\x1b[0x 对 \x1b[1x{}\x1b[0x.", forcename_1[2], forcename_1[3])));
						int winner_2 = getWinner(forceid_1[2], forceid_1[3]);
						
						pk::message_box(pk::encode(pk::format("决胜轮 \x1b[1x{}军\x1b[0x 对 \x1b[1x{}军\x1b[0x .", pk::decode(pk::get_name(pk::get_force(winner_1))), pk::decode(pk::get_name(pk::get_force(winner_2))))));
						winner_final = getWinner(winner_1, winner_2);

					}
					else
					{
						pk::message_box(pk::encode(pk::format("\x1b[1x{}军\x1b[0x 对 \x1b[1x{}军\x1b[0x .", pk::decode(pk::get_name(force_)), pk::decode(pk::get_name(force_list1[0])))));
						winner_final = getWinner(force_.get_id(), force_list1[0].get_id());
					}
					@ force_winner = pk::get_force(winner_final);
					winner_name = pk::decode(pk::get_name(force_winner));
					@ winner_building = pk::get_building(pk::get_person(pk::get_kunshu_id(force_winner)).service);

					pk::message_box(pk::encode(pk::format("在年度比武大会中,各方激烈角逐.\n\x1b[1x{}军\x1b[0x取得了最后的胜利!\n赢取冠军奖金与技巧p.", winner_name))); 
					ch::add_tp(force_winner, winner_tp, force_winner.get_pos());
					pk::add_gold(winner_building, winner_gold, true);
					winner_building.update();
					pk::history_log(force_winner.get_pos(), force_winner.color, pk::encode(pk::format("\x1b[1x{}军\x1b[0x在比武大会中取胜.", winner_name)));
				}
				else
				{
					if (force_list1.count < 2)
						pk::history_log(0, -1, pk::encode("因与会势力数不足,此次年度比武大会未举办."));
					else
					{
						pk::force@ force0_winner;
						string force0_name;
						pk::building@ building0;
						
						if (pk::rand_bool(50))  @ force0_winner = force_list1[0];  //50概率为第一名最终获胜
						else if (pk::rand_bool(80))  @ force0_winner = force_list1[1];  //40概率第二名获胜
						else  @ force0_winner = force_list1[2];  //第三名获胜
						force0_name = pk::decode(pk::get_name(force0_winner));
						@ building0 = pk::get_building(pk::get_person(pk::get_kunshu_id(force0_winner)).service);
						pk::message_box(pk::encode(pk::format("在年度比武大会中,各方激烈角逐.\n\x1b[1x{}军\x1b[0x取得了最后的胜利!\n赢取冠军奖金与技巧p.", force0_name))); 
						pk::add_gold(building0, winner_gold, true);
						ch::add_tp(force0_winner, winner_tp, force0_winner.get_pos());
						building0.update();
						pk::history_log(force0_winner.get_pos(), force0_winner.color, pk::encode(pk::format("\x1b[1x{}军\x1b[0x在比武大会中取胜.", force0_name)));
					}
					
				}
			}
			
		}
		
		//势力可选取武将list
		pk::list<pk::person@> personList(pk::force@ force_2)
		{
			pk::list<pk::person@> list_t = pk::get_person_list(force_2, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
			list_t.sort(function(a, b)
            {
				return a.stat[1] > b.stat[1];// 按武力排序
            });
			return list_t;
		}
		
		//获取晋级势力
		int getWinner(int force_id1, int force_id2)
		{
			pk::force@ force1 = pk::get_force(force_id1);
			pk::force@ force2 = pk::get_force(force_id2);
			string force_id1_name = force_id1 != -1 ? pk::decode(pk::get_name(force1)) : "";
			string force_id2_name = force_id2 != -1 ? pk::decode(pk::get_name(force2)) : "";
			if (force_id1 == -1 and force_id2 == -1)  return -1;  //两者都是空势力，进入下一回比赛，晋级为空势力
			else if (force_id1 == -1)
			{
				
				pk::message_box(pk::encode(pk::format("\x1b[1x{}军\x1b[0x轮空无奖励.\n晋级下一轮.", force_id2_name)), pk::get_person(무장_문관));
				return force_id2;
			}
			else if (force_id2 == -1)
			{
				pk::message_box(pk::encode(pk::format("\x1b[1x{}军\x1b[0x轮空无奖励.\n晋级下一轮.", force_id1_name)), pk::get_person(무장_문관));
				return force_id1;
			}
			else  //均不等于，进入pk
			{
				pk::list<pk::person@> force1_list = personList(force1);
				pk::list<pk::person@> force2_list = personList(force2);
				pk::person@ person_a1;
				pk::person@ person_a2;
				pk::person@ person_a3;
				pk::person@ person_b1;
				pk::person@ person_b2;
				pk::person@ person_b3;
				if (pk::is_player_controlled(force1))  //1是玩家
				{
					array<pk::person@> person_sel_arr;
					for (int jj = 0; jj < 3; jj++)
					{
						person_sel_arr = pk::list_to_array(pk::person_selector( pk::encode("武将选择"), pk::encode("请选择参赛的武将.  （最多3人）"), personList(force1), 1, 3) );
						if (person_sel_arr.length != 0) break;  //当有选择武将时，跳过，循环3次以免发生死循环
					}
					
					@person_a1 = person_sel_arr[0];
					@person_a2 = person_sel_arr.length > 1 ? person_sel_arr[1] : null;
					@person_a3 = person_sel_arr.length > 2 ? person_sel_arr[2] : null;
					@person_b1 = force2_list[0];
					@person_b2 = force2_list.count > 1 ? force2_list[1] : null;
					@person_b3 = force2_list.count > 2 ? force2_list[2] : null;
					
					pk::fade(0);
					pk::background(57);
					pk::fade(255);
					
					pk::unit@ dummy_unit;
					int duel_result = pk::duel(dummy_unit, dummy_unit, person_a1, person_a2, person_a3, person_b1, person_b2, person_b3, pk::is_player_controlled(force1), pk::is_player_controlled(force2), 0, true).first;
					//pk::trace(pk::format("比武结果为{}", duel_result));
					pk::building@ building1 = pk::get_building(pk::get_person(pk::get_kunshu_id(force1)).service);
					pk::building@ building2 = pk::get_building(pk::get_person(pk::get_kunshu_id(force2)).service);

					if (duel_result == 0)
					{
						pk::play_se(12);
						pk::message_box(pk::encode("恭喜主公!"), pk::get_person(무장_문관));
						pk::message_box(pk::encode("我军胜出,获得本轮奖金!"), pk::get_person(무장_문관));
						pk::add_gold(building1, succeed_gold, true);
						pk::add_gold(building2, fail_gold, true);
						building1.update();
						building2.update();
						return force_id1;
					}
					else
					{
						pk::play_se(13);
						pk::message_box(pk::encode("很遗憾...主公还请再接再厉!"), pk::get_person(무장_문관)); 
						pk::add_gold(building2, succeed_gold, true);
						pk::add_gold(building1, fail_gold, true);
						building1.update();
						building2.update();
						return force_id2;
					}
				}
				else if (pk::is_player_controlled(force2))  //2是玩家
				{
					array<pk::person@> person_sel_arr;
					for (int jj = 0; jj < 3; jj++)
					{
						person_sel_arr = pk::list_to_array(pk::person_selector( pk::encode("武将选择"), pk::encode("请选择参赛的武将.  （最多3人）"), personList(force2), 1, 3) );
						if (person_sel_arr.length != 0) break;  //当有选择武将时，跳过，循环3次以免发生死循环
					}
					
					@person_b1 = person_sel_arr[0];
					@person_b2 = person_sel_arr.length > 1 ? person_sel_arr[1] : null;
					@person_b3 = person_sel_arr.length > 2 ? person_sel_arr[2] : null;
					@person_a1 = force1_list[0];
					@person_a2 = force1_list.count > 1 ? force1_list[1] : null;
					@person_a3 = force1_list.count > 2 ? force1_list[2] : null;
					
					pk::fade(0);
					pk::background(57);
					pk::fade(255);
					
					pk::unit@ dummy_unit;
					int duel_result = pk::duel(dummy_unit, dummy_unit, person_a1, person_a2, person_a3, person_b1, person_b2, person_b3, pk::is_player_controlled(force1), pk::is_player_controlled(force2), 0, true).first;
					
					pk::building@ building1 = pk::get_building(pk::get_person(pk::get_kunshu_id(force1)).service);
					pk::building@ building2 = pk::get_building(pk::get_person(pk::get_kunshu_id(force2)).service);

					if (duel_result == 1)
					{
						pk::play_se(12);
						pk::message_box(pk::encode("恭喜主公!"), pk::get_person(무장_문관));
						pk::message_box(pk::encode("我军胜出,获得本轮奖金!"), pk::get_person(무장_문관));
						pk::add_gold(building2, succeed_gold, true);
						pk::add_gold(building1, fail_gold, true);
						building1.update();
						building2.update();
						return force_id2;
					}
					else
					{
						pk::play_se(13);
						pk::message_box(pk::encode("很遗憾...主公还请再接再厉!"), pk::get_person(무장_문관)); 
						pk::add_gold(building1, succeed_gold, true);
						pk::add_gold(building2, fail_gold, true);
						building1.update();
						building2.update();
						return force_id1;
					}
				}
				else  //均是电脑
				{
					@person_a1 = force1_list[0];
					@person_a2 = force1_list.count > 1 ? force1_list[1] : null;
					@person_a3 = force1_list.count > 2 ? force1_list[2] : null;
					
					@person_b1 = force2_list[0];
					@person_b2 = force2_list.count > 1 ? force2_list[1] : null;
					@person_b3 = force2_list.count > 2 ? force2_list[2] : null;
					
					pk::fade(0);
					pk::background(57);
					pk::fade(255);
					
					if (pk::rand_bool(50))
					{
						switch (pk::rand(4))
						{
							case 0: pk::message_box(pk::encode("（此次的对手就是他吗?）"), pk::get_person(pk::get_kunshu_id(force1))); break;
							case 1: pk::message_box(pk::encode(pk::format("（传说中的\x1b[1x{}军\x1b[0x,就是此次的对手了吗?）", force_id1_name)), pk::get_person(pk::get_kunshu_id(force2))); break;
							case 2: pk::message_box(pk::encode("（此次我军势不可挡!）"), pk::get_person(pk::get_kunshu_id(force2))); break;
							case 3: pk::message_box(pk::encode(pk::format("（觉悟吧!\x1b[1x{}军\x1b[0x!）", force_id2_name)), pk::get_person(pk::get_kunshu_id(force1))); break;
						}
					}
							
					pk::unit@ dummy_unit;
					int duel_result = pk::duel(dummy_unit, dummy_unit, person_a1, person_a2, person_a3, person_b1, person_b2, person_b3, pk::is_player_controlled(force1), pk::is_player_controlled(force2), 0, true).first;
					
					pk::building@ building1 = pk::get_building(pk::get_person(pk::get_kunshu_id(force1)).service);
					pk::building@ building2 = pk::get_building(pk::get_person(pk::get_kunshu_id(force2)).service);

					if (duel_result == 0)
					{
						pk::message_box(pk::encode(pk::format("\x1b[1x{}军\x1b[0x胜出.", force_id1_name))); 
						pk::add_gold(building1, succeed_gold, true);
						pk::add_gold(building2, fail_gold, true);
						building1.update();
						building2.update();
						return force_id1;
					}
					else
					{
						pk::message_box(pk::encode(pk::format("\x1b[1x{}军\x1b[0x胜出.", force_id2_name))); 
						pk::add_gold(building2, succeed_gold, true);
						pk::add_gold(building1, fail_gold, true);
						building1.update();
						building2.update();
						return force_id2;
					}
				}
				
			}

		}
    }

    Main main;
}
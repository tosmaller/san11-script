// ## 2021/10/21 # 江东新风 # 改进贼将杀死的设定 ##
// ## 2021/10/01 # 江东新风 # 无kre情况下修复沉着类特技错误 ##
// ## 2021/07/09 # 白马叔叔 # AI探索未发现人才、小城的未发现移动到大城、杀死不该活着的贼、杀死没武将的势力 ##
// @@ 2021/04/18 @ 白马叔叔 @@

namespace 辅助补丁
{
    //---------------------------------------------------------------------------------------
    const int begin_month = 4; //从哪月之后才开始探索，权衡玩家偷鸡，也不能浪费太多回合
	const int 城市_无 = -1;
    //---------------------------------------------------------------------------------------

    class Main
    {

        Main()
        {
			pk::bind(107, pk::trigger107_t(callback));
			pk::bind(108, pk::trigger108_t(callback_month));
			pk::bind(103, pk::trigger103_t(callback2));//无kre情况下修复沉着类特技错误
		}

		void callback2()
		{
			for (int i = 0; i < 城市_末; ++i)
			{
				for (int j = 0; j < 6; ++j)
				{
					pk::get_city(i).set_neighbor(j, 城市相邻[i][j]);
					int neighbor = 城市相邻[i][j];
					if (neighbor != -1)
					{
						bool wrong = true;
						for (int k = 0; k < 6; ++k)
						{
							if (城市相邻[neighbor][k] == i) wrong = false;
						}
						if (wrong) pk::trace(pk::decode(pk::get_name(pk::get_city(i))) + "和" + pk::decode(pk::get_name(pk::get_city(neighbor))));
					}

				}

			}
			if (pk::get_scenario().loaded) return;

			//ch::u8debug("start setting");			
			pk::person@ person_1 = pk::get_person(武将_关平);
			//特技id
			if (person_1.skill == -1) person_1.skill = 特技_沉着; 

			pk::person@ person_2 = pk::get_person(武将_张春华);
			//特技id
			if (person_2.skill == -1) person_2.skill = 特技_沉着; 

			//pk::person@ person_3 = pk::get_person(武将_蒋义渠);
			//特技id
			//if (person_3.skill == -1) person_3.skill = 特技_沉着; 
			////////////////////////////////////////////////
		}



		//事件-AI把未发现人才探索出来
		void callback()
		{
			//经过月份超过4个月保护期、当前月份超过第4个月
			if (pk::get_elapsed_months() >= begin_month && pk::get_month() >= begin_month)
				pk::scene(pk::scene_t(scene_explore_person));
			
			//在小城登场的武将移动到所属城市，设定在1月11日手动发生
			if (pk::get_month() == 1 && pk::get_day() == 11)
				pk::scene(pk::scene_t(scene_move_person));
		}
		
		
		void callback_month()
		{
			//杀死变成普通武将的临时武将
			pk::scene(pk::scene_t(scene_kill_person));
			
			//杀死已经没有城市、没有港关的势力
			pk::scene(pk::scene_t(scene_kill_force));
		}
		
		
		//粗糙点就是有未发现直接变成在野。再细一些就是有无眼力、有无政治高、而且没有任务中、外出的
		void scene_explore_person()
		{
			for (int i = 0; i < 建筑_城市末; i++)
			{
				auto building_t = pk::get_building(i);
				
				if (building_t.is_player())
					continue;  //跳过玩家地盘
				if (building_t.get_force_id() == -1)
					continue;  //跳过空城无势力
				if (pk::get_taishu_id(building_t) == -1)
					continue;  //跳过无太守
				
				auto list_0 = pk::get_person_list(building_t, pk::mibun_flags(身份_未发现));  //只取未发现身份的
				
				if (list_0.count == 0) continue;  //跳过没有未发现武将
				
				bool 有眼力或政治高 = false;   //有眼力或者有政治高的人才
				auto list_1 = pk::get_person_list(building_t, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));  //只取未发现身份的
				
				if (building_t.has_skill(특기_안력))
					有眼力或政治高 = true;  //城市有眼力特技，直接判定true省计算
				else
				{
					for (int j = 0; j < list_1.count; j++)
					{
						auto person_1 = list_1[j];
						if (!pk::is_absent(person_1) && !pk::is_unitize(person_1) && person_1.stat[2] > 69)
						{
							有眼力或政治高 = true;  //人物非离开，非出征，政治超过69
							break;
						}
					}
				}
				
				if (有眼力或政治高)
				{
					for (int k = 0; k < list_0.count; k++)
					{
						auto person_2 = list_0[k];
						if (pk::is_absent(person_2)) break;  //人物不在场直接结束（三顾触发后，使刘表不能找出诸葛均、黄月英）
						else
						{
							person_2.mibun = 身份_在野;
							//pk::trace(pk::format("被AI探索的人：{}", pk::decode(pk::get_name(person_2))));
							person_2.update();
							break;
						}
					}
				}
			}
		}
		
		//移动武将到城市，以免永远找不到
		void scene_move_person()
		{
			for (int i = 42; i < 建筑_据点末; i++)
			{
				auto building_t = pk::get_building(i);
				auto list_0 = pk::get_person_list(building_t, pk::mibun_flags(身份_未发现));  //只取未发现身份的
				auto list_count = list_0.count;
				if (list_count == 0) continue;  //跳过没有未发现武将
				for (int j = 0; j < list_count; j++)
				{
					auto person = list_0[j];
					if (person.appearance < pk::get_year())  //登场年得小于当前年，比方登场是207，要晚一年，在208才会自动出现
					{
						//pk::trace(pk::format("被移动到城市的人：{}", pk::decode(pk::get_name(person))));
						pk::move(person, pk::get_building(pk::get_city_id(building_t.pos)), false);
					}
				}
			}
		}
		
		//杀死变成普通武将的临时武将
		void scene_kill_person()
		{
			for (int person_id = 敌将_开始; person_id < 敌将_末; person_id++)
			{
				pk::person@ person = pk::get_person(person_id);
				if (pk::is_alive(person))
				{
					if (person.mibun != 身份_死亡 && person.district < 非贼势力_末) pk::kill(person, null, null, null, 1);   //杀死 还活着但归属非异族军团的
					if (person.mibun == 身份_在野) pk::kill(person, null, null, null, 1);
				}
				if (person.mibun == 身份_死亡)
				{
<<<<<<< HEAD
					pk::trace(pk::format("重置贼将 id:{}", person_id));
=======
					//pk::trace(pk::format("重置贼将 id:{}", person_id));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					pk::reset(person);
				}
				
			}
		}
		
		
		//杀死已经没有城市、没有港关的势力
		void scene_kill_force()
		{
			pk::list<pk::force@> force_list = pk::get_force_list(false);
			if (force_list.count == 1) return;  //只有一个返回
			for (int i = 0; i < force_list.count; i++)
			{
				if (ch::get_force_person_count(force_list[i]) <= 0) pk::kill(force_list[i], false, null);   //杀死已经没有武将仍不破灭的势力
			}
		}

		array<array<int>> 城市相邻 = {
			/*襄平*/{城市_北平,城市_无,城市_无,城市_无,城市_无,城市_无},
			/*北平*/{城市_襄平,城市_蓟,城市_南皮,城市_无,城市_无,城市_无},
			/*蓟*/{城市_北平,城市_南皮,城市_晋阳,城市_无,城市_无,城市_无},
			/*南皮*/{城市_北平,城市_蓟,城市_平原,城市_邺,城市_无,城市_无},
			/*平原*/{城市_南皮,城市_邺,城市_北海,城市_濮阳,城市_无,城市_无},
			/*晋阳*/{城市_蓟,城市_邺,城市_新野,城市_无,城市_无,城市_无},
			/*邺*/{城市_南皮,城市_平原,城市_晋阳,城市_濮阳,城市_陈留,城市_新野},
			/*北海*/{城市_平原,城市_小沛,城市_无,城市_无,城市_无,城市_无},
			/*下邳*/{城市_庐江,城市_小沛,城市_寿春,城市_建业,城市_无,城市_无},
			/*小沛*/{城市_下邳,城市_寿春,城市_濮阳,城市_北海,城市_无,城市_无},
			/*寿春*/{城市_下邳,城市_小沛,城市_汝南,城市_濮阳,城市_庐江,城市_陈留},
			/*濮阳*/{城市_平原,城市_邺,城市_寿春,城市_陈留,城市_小沛,城市_无},
			/*陈留*/{城市_邺,城市_寿春,城市_濮阳,城市_许昌,城市_洛阳,城市_宛},
			/*许昌*/{城市_陈留,城市_汝南,城市_洛阳,城市_宛,城市_襄阳,城市_无},
			/*汝南*/{城市_寿春,城市_许昌,城市_江夏,城市_襄阳,城市_无,城市_无},
			/*洛阳*/{城市_陈留,城市_许昌,城市_宛,城市_长安,城市_新野,城市_无},
			/*宛*/{城市_许昌,城市_洛阳,城市_长安,城市_上庸,城市_襄阳,城市_陈留},
			/*长安*/{城市_洛阳,城市_宛,城市_安定,城市_汉中,城市_新野,城市_无},
			/*上庸*/{城市_宛,城市_襄阳,城市_汉中,城市_无,城市_无,城市_无},
			/*安定*/{城市_长安,城市_天水,城市_武威,城市_无,城市_无,城市_无},
			/*天水*/{城市_安定,城市_武威,城市_汉中,城市_无,城市_无,城市_无},
			/*武威*/{城市_安定,城市_天水,城市_无,城市_无,城市_无,城市_无},
			/*建业*/{城市_下邳,城市_吴,城市_庐江,城市_下邳,城市_无,城市_无},
			/*吴*/{城市_建业,城市_会稽,城市_柴桑,城市_庐江,城市_无,城市_无},
			/*会稽*/{城市_吴,城市_无,城市_无,城市_无,城市_无,城市_无},
			/*庐江*/{城市_寿春,城市_建业,城市_柴桑,城市_江夏,城市_吴,城市_下邳},
			/*柴桑*/{城市_吴,城市_庐江,城市_江夏,城市_江陵,城市_长沙,城市_无},
			/*江夏*/{城市_庐江,城市_柴桑,城市_襄阳,城市_江陵,城市_长沙,城市_汝南},
			/*新野*/{城市_长安,城市_洛阳,城市_邺,城市_晋阳,城市_无,城市_无},
			/*襄阳*/{城市_江夏,城市_汝南,城市_江陵,城市_上庸,城市_宛,城市_许昌},
			/*江陵*/{城市_柴桑,城市_江夏,城市_襄阳,城市_长沙,城市_武陵,城市_永安},
			/*长沙*/{城市_柴桑,城市_江陵,城市_武陵,城市_桂阳,城市_江夏,城市_无},
			/*武陵*/{城市_江陵,城市_长沙,城市_零陵,城市_永安,城市_无,城市_无},
			/*桂阳*/{城市_长沙,城市_零陵,城市_无,城市_无,城市_无,城市_无},
			/*零陵*/{城市_武陵,城市_桂阳,城市_无,城市_无,城市_无,城市_无},
			/*永安*/{城市_江陵,城市_武陵,城市_江州,城市_无,城市_无,城市_无},
			/*汉中*/{城市_长安,城市_上庸,城市_天水,城市_梓潼,城市_无,城市_无},
			/*梓潼*/{城市_汉中,城市_江州,城市_成都,城市_无,城市_无,城市_无},
			/*江州*/{城市_永安,城市_梓潼,城市_成都,城市_建宁,城市_无,城市_无},
			/*成都*/{城市_梓潼,城市_江州,城市_建宁,城市_无,城市_无,城市_无},
			/*建宁*/{城市_成都,城市_云南,城市_江州,城市_无,城市_无,城市_无},
			/*云南*/{城市_建宁,城市_无,城市_无,城市_无,城市_无,城市_无}


		};
    }

    Main main;
}
// @@ 2021/08/29 @ 白马叔叔 @@

//另辟途径给清谈登庸一个便利，花费300金探访在野武将清谈登庸
//城市有可行动的清谈特技人才，即可派遣清谈武将舌战说服在野人才加入我方，无视厌恶、禁仕、义兄弟、夫妻关系，舌战胜则加入
//清谈登庸成功后，清谈武将会得到相应功绩；如果在野人才厌恶君主，则初始忠诚会较低
//AI和玩家军团亦会使用清谈登庸

namespace 人才指令清谈
{
    //---------------------------------------------------------------------------------------
	const int GOLD_COST = 300;    // 金花费
	const int tp_get = 25;               // 技巧p获得数
	const int 功绩获得 = 200;          //正常登庸武将获得的功绩
	
	const bool AI清谈开启 = true;  //AI清谈开关
	
    //---------------------------------------------------------------------------------------
	
    class Main
    {
		pk::building@ building_;
		pk::city@ city_;
		pk::force@ force_;
		pk::district@ district;
		pk::person@ target_person;
		pk::person@ actor_;
		pk::person@ kunshu_;
		string target_name;
		string actor_name;
		string kunshu_name;
		int ACTION_COST;
		
        Main()
        {
			pk::menu_item item_清谈;
			item_清谈.menu = 102;
			item_清谈.pos = 10;
			item_清谈.shortcut = "T";
			item_清谈.init = pk::building_menu_item_init_t(init);
			item_清谈.is_visible = pk::menu_item_is_visible_t(isVisible);
			item_清谈.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item_清谈.get_text = pk::menu_item_get_text_t(getText);
			item_清谈.get_desc = pk::menu_item_get_desc_t(getDesc);
			item_清谈.handler = pk::menu_item_handler_t(handler_清谈);
			pk::add_menu_item(item_清谈);
			
			if (AI清谈开启)
				pk::bind(107, pk::trigger107_t(AI_清谈));
		}
		
		void init(pk::building@ building)
		{
			@building_ = @building;
			@city_ = pk::building_to_city(building);
			@force_ = pk::get_force(building.get_force_id());
			@district = pk::get_district(pk::get_district_id(force_, 1));
			@kunshu_ = pk::get_person(pk::get_kunshu_id(building));
			ACTION_COST = get_hrcmd_ap(city_);
		}
		
		int get_hrcmd_ap(pk::city@ city_t)
        {
            if (pk::is_alive(city_t))
            {
                if (pk::has_facility(city_t, 시설_인재부))
                    return 10;// 行动力花费，有人才府行动力减半
            }
            return 20;
        }
		
		bool isVisible()
        {
            if (pk::is_campaign()) return false;  //决战称霸不可见
            if (building_.get_id() >= 城市_末) return false;  //城关不可见，仅限城市
            return true;
        }
		
		bool isEnabled()
		{
			if (!building_.has_skill(特技_清谈)) return false;  //没有清谈
			else if (int(pk::get_district(pk::get_district_id(force_, 1)).ap) < ACTION_COST) return false;  //行动力不足
			else if (pk::get_gold(building_) < GOLD_COST) return false;  //金不足
			else if (getTargetList(building_).count < 1) return false;  //没有在野
			else if (getActorList(building_).count < 1) return false;      //没有可执行的武将
			return true;
		}

		string getText()
		{
			return pk::encode("清谈登庸");
		}
		
		string getDesc()
		{
			if (!building_.has_skill(特技_清谈))
				return pk::encode("城市里没有清谈特技武将");  //没有清谈
			else if (int(pk::get_district(pk::get_district_id(force_, 1)).ap) < ACTION_COST)
				return pk::encode(pk::format("行动力不足（行动力 {}）", ACTION_COST));  //行动力不足
			else if (pk::get_gold(building_) < GOLD_COST)
				return pk::encode(pk::format("金不足（金 {}）", GOLD_COST));  //金不足
			else if (getTargetList(building_).count < 1)
				return pk::encode("没有在野武将");  //没有在野
			else if (getActorList(building_).count < 1)
				return pk::encode("没有可执行的武将");      //没有可执行的武将
			return pk::encode("与在野武将进行舌战（清谈登庸）");	
		}
		
		bool handler_清谈()
		{
			//对象武将选择
			pk::list<pk::person@> person_sel_0 = pk::person_selector(pk::encode("对象武将选择"), pk::encode("请选择欲清谈登庸的武将"), getTargetList(building_), 1, 1);
			if (person_sel_0.count == 0) return false; // 未选择结束
			
			//清谈武将选择
			pk::list<pk::person@> person_sel_1 = pk::person_selector(pk::encode("清谈武将选择"), pk::encode("请选择执行清谈登庸的武将"), getActorList(building_), 1, 1);
			if (person_sel_1.count == 0) return false; // 未选择结束
			
			@target_person = person_sel_0[0];
			target_name = pk::decode(pk::get_name(target_person));
			@actor_ = person_sel_1[0];
			actor_name = pk::decode(pk::get_name(actor_));
			kunshu_name = pk::decode(pk::get_name(kunshu_));
			
			pk::scene(pk::scene_t(scene_清谈));   //清谈场景
			
			pk::add_ap(district, -ACTION_COST);
			pk::add_gold(building_, -GOLD_COST, true);
			actor_.action_done = true;
			actor_.update();
			
			return true;
		}
		
		//清谈执行
		void scene_清谈()
		{
			pk::fade(0);
			pk::background(5);
			pk::fade(255);
			
			pk::message_box(pk::encode("您请坐吧.\n童儿,快上茶."), target_person);
			pk::message_box(pk::encode(pk::format("这\x1b[1x{}\x1b[0x金是一点薄礼.\n此次奉我主\x1b[1x{}\x1b[0x大人之命，\n特请先生出山相助.", GOLD_COST, kunshu_name)), actor_);
			pk::message_box(pk::encode(pk::format("很抱歉，对\x1b[1x{}\x1b[0x大人仕官一事，\n恕难从命......", kunshu_name)), target_person);
			if (pk::yes_no(pk::encode(pk::format("要以清谈舌战说服吗？\n挑战者  \x1b[1x{}\x1b[0x：智力：{}\n受挑战者\x1b[1x{}\x1b[0x：智力：{}", actor_name, actor_.stat[武将能力_智力], target_name, target_person.stat[武将能力_智力]))))
			{
				if (pk::debate(actor_, target_person, pk::is_player_controlled(actor_), false, false, true).first == 0)
				{
					pk::set_district(target_person, building_.get_district_id());  //设置到军团
					target_person.mibun = 身份_一般;  //身份一般
					//target_person.loyalty = pk::is_dislike(target_person, kunshu_.get_id()) ? 70 : 100;  //是否厌恶君主，忠诚度
					target_person.loyalty = 100 - int(pk::get_aishou_distance(target_person, pk::get_kunshu_id(building_)) / 5) - (pk::is_dislike(target_person, kunshu_.get_id()) ? 20 : 0);//是否厌恶君主，忠诚度
					target_person.action_done = true;
					target_person.update();
					pk::add_kouseki(actor_, 功绩获得);  //清谈武将功绩增加
					
					pk::play_se(6);
					switch (pk::rand(3))
					{
						case 0 : pk::message_box(pk::encode(pk::format("我明白了。\n就将我等之力，献给\x1b[1x{}\x1b[0x大人吧。", kunshu_name)), target_person); break;
						case 1 : pk::message_box(pk::encode(pk::format("我\x1b[1x{}\x1b[0x虽然不才，\n但愿加入\x1b[2x{}军\x1b[0x略尽一份薄力。", target_name, kunshu_name)), target_person); break;
						case 2 : pk::message_box(pk::encode(pk::format("就尽陪末座，\n为\x1b[2x{}军\x1b[0x略尽一番心意吧。", kunshu_name)), target_person); break;
					}
					pk::message_box(pk::encode("如此真是太好了。"), actor_);					
					ch::add_tp(force_, tp_get, building_.get_pos());  //技巧获得
					pk::fade(0);
					pk::background(-1);
					pk::fade(255);
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人成功地登庸了\x1b[1x{}\x1b[0x大人。", actor_name, target_name)), pk::get_person(武将_文官));
					pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x清谈成功登庸\x1b[1x{}\x1b[0x", actor_name, target_name)));
				}
				else
				{
					pk::play_se(7);
					pk::message_box(pk::encode("今天先到这里吧。"), target_person);
					pk::message_box(pk::encode("唔唔..."), actor_);
					pk::fade(0);
					pk::background(-1);
					pk::fade(255);
					pk::message_box(pk::encode(pk::format("真是非常抱歉，\n登用\x1b[1x{}\x1b[0x一事失败了......", target_name)), actor_);					
					pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x清谈登庸\x1b[1x{}\x1b[0x失败", actor_name, target_name)));
				}
			}
			else
			{
				pk::play_se(7);
				pk::message_box(pk::encode(pk::format("登用\x1b[1x{}\x1b[0x一事失败了......", target_name)), pk::get_person(武将_文官));
				pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x清谈登庸\x1b[1x{}\x1b[0x失败", actor_name, target_name)));
			}
		}
		
		void AI_清谈()
		{
			if (pk::get_day() == 11) pk::scene(pk::scene_t(scene_AI_清谈));
		}
		
		//AI清谈执行
		void scene_AI_清谈()
		{
			for (int i = 0; i < 城市_末; i++)
			{
				auto building_t = pk::get_building(i);
				auto district_t = pk::get_district(building_t.get_district_id());
				if (district_t is null) continue;  //如果是空城跳过
				if (district_t.is_player() && district_t.no == 1) continue;  //如果是玩家第一军团跳过
				if (district_t.ap < 20) continue;  //行动力不足
				if (pk::get_gold(building_t) < GOLD_COST) continue;  //金不足
				pk::list<pk::person@> target_list = getTargetList(building_t);  //目标列表
				if (target_list.count < 1) continue;  //没有在野
				pk::list<pk::person@> actor_list = getActorList(building_t);  //目标列表
				if (actor_list.count < 1) continue;      //没有可执行的武将
				
				auto target_t = target_list[0];
				auto actor_t = actor_list[0];
				bool debate_result = false;
				
				int dif = actor_t.stat[武将能力_智力] - target_t.stat[武将能力_智力];
				if (dif >= 15) debate_result = true;
				else if (dif <= -20) debate_result = false;
				else debate_result = pk::rand_bool(50 + dif);
				//pk::trace(pk::format("清谈城市序号：{}，debate_result：{}，target_t：{}", i, debate_result, pk::decode(pk::get_name(target_t))));
				if (debate_result)
				{
					pk::set_district(target_t, building_t.get_district_id());  //设置到军团
					//pk::move(target_t, building_t);	 //移动到据点
					target_t.mibun = 身份_一般;  //身份一般
					target_t.loyalty = 100 - int(pk::get_aishou_distance(target_t, pk::get_kunshu_id(building_t)) / 5) - (pk::is_dislike(target_t, pk::get_kunshu_id(building_t)) ? 20 : 0);//是否厌恶君主，忠诚度
					target_t.action_done = true;
					target_t.update();
					actor_t.action_done = true;
					actor_t.update();
					pk::add_kouseki(actor_t, 功绩获得);  //清谈武将功绩增加
					district_t.ap -= 20;
					pk::add_gold(building_t, -GOLD_COST, false);
					ch::add_tp(pk::get_force(building_t.get_force_id()), tp_get, building_t.get_pos());  //技巧获得
				}
				
			}
		}
		
		//获取可行动清谈武将列表
		pk::list<pk::person@> getActorList(pk::building@ building_t)
		{
			pk::list<pk::person@> list_a;
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building_t);
			for (int i = 0; i < person_list.count; i++)
			{
				if (ch::has_skill(person_list[i], 特技_清谈))
					list_a.add(person_list[i]);
			}
			list_a.sort(function(a, b)
			{
				return (a.stat[武将能力_智力] > b.stat[武将能力_智力]);  //按武将智力降序
			});
			return list_a;
		}	
		
		//获取目标在野武将列表
		pk::list<pk::person@> getTargetList(pk::building@ building_t)
		{
			pk::list<pk::person@> list_z;
			pk::list<pk::person@> person_list = pk::get_person_list(building_t, pk::mibun_flags(身份_在野));
			for (int i = 0; i < person_list.count; i++)
			{
				if (person_list[i].district < 0)
				{
					if (person_list[i].gikyoudai == -1 or person_list[i].gikyoudai == person_list[i].get_id())
						list_z.add(person_list[i]);  //没有结兄弟或者自己是大哥才加入（防止玩家流放小弟被AI登庸）
					else if (building_t.is_player())
						list_z.add(person_list[i]);  //玩家势力正常清谈登庸在野武将
				}
			}
			return list_z;
		}

    }

    Main main;
}
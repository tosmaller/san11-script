// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/02/11 # 江东新风 # 小幅削弱募兵 ##
// ## 2021/12/24 # 江东新风 # 人口相关设定，关闭ai自动募兵，改为特定条件募兵 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/04/18 # 白马叔叔 # AI募兵、算法优化、调整关小城募兵 ##																					
// ## 2021/03/09 # 白马叔叔 # 无太守修正 ##
// ## 2020/12/12 # 江东新风# 修复trace参数类型错误 ##
// ## 2020/10/31 # 江东新风 # 同步honeybee大神的更新##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/09 # 氕氘氚 # 加入【军事】菜单 ##
// ## 2020/08/02 # 氕氘氚 # 募兵公式 2000+(20~70)*魅力 ##
/*
@제작자: HoneyBee
@설명: 모병을 실행  (매력 수치에 따라서 모병되는 병력이 달라짐)

*/

namespace 募兵
{

	// ================ CUSTOMIZE ================

	const int TP_COST = 100; 	 // 기교P 필요량
	const int ACTION_COST = 20;  // 행동력 필요량
	const int GOLD_COST = 600;
	const int energy_ = 10; 	 // 기력 상승 수치
	const int 募兵倍率 = 100;

	bool AI募兵开启 = false;          //AI募兵事件开关。true为开，false为关。
	const int AI募兵概率 = 10;      //AI募兵事件概率，默认10															  
	// ===========================================

	const int KEY = pk::hash("募兵");

	class Main
	{
		bool 调试模式 = false;
		pk::building@ building_;
		pk::force@ force_;
		pk::person@ taishu_;
		pk::city@ city_;


		Main()
		{
			pk::bind(202, pk::trigger202_t(onAIRunningOrder));
			add_menu();
			//pk::bind(107, pk::trigger107_t(callback));				 
		}

		void add_menu()
		{
			pk::menu_item item;
			item.menu = 101;
			// item.pos = 5;
			item.shortcut = "4";
			item.init = pk::building_menu_item_init_t(init);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@taishu_ = pk::get_taishu_id(building) == -1 ? null : pk::get_person(pk::get_taishu_id(building));
			@city_ = pk::building_to_city(building);
		}

		string getText()
		{
			return pk::encode("募兵");
		}

		bool isEnabled()
		{
			auto person_list = pk::get_idle_person_list(building_);
			int result = check_avaliable(building_, person_list);
			if (result != 0) return false;
			else return true;
		}

		int check_avaliable(pk::building@ building, pk::list<pk::person@> list)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			if (base_ex[building.get_id()].enlist_done) return 1;
			else if (list.count == 0) return 2;
			else if (pk::get_district(building.get_district_id()).ap < ACTION_COST) return 3;			
			else if (pk::get_troops(building) >= pk::get_max_troops(building)) return 4;
			else if (pk::get_force(building.get_force_id()).tp < TP_COST) return 5;
			else if (pk::get_gold(building) < GOLD_COST) return 6;
			else if (开启人口系统 and base_ex[building.get_id()].mil_pop_all <= 0) return 7;
			else return 0;
		}

		string getDesc()
		{
			auto person_list = pk::get_idle_person_list(building_);
			int result = check_avaliable(building_, person_list);
			switch (result)
			{
			case 1: return pk::encode("已执行过募兵");
			case 2: return pk::encode("没有可执行的武将. ");
			case 3: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			case 4: return pk::encode("兵力已达最大值.");
			case 5: return pk::encode(pk::format("技巧不足.(必须{} 气力)", TP_COST));
			case 6: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			case 7: return pk::encode("总兵役人口不足");
			case 0: return pk::encode(pk::format("张榜募集士兵.(消耗：金{} 技巧点{}, 行动力{})", GOLD_COST, TP_COST, ACTION_COST));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		bool handler()
		{
			if (开启人口系统)
			{
				int choise = pk::choose(pk::encode("是否开始募集士兵?\n(兵源来自总兵役人口及人口)"), { pk::encode(" 是 "), pk::encode(" 否 ") });
				if (choise == 1)
					return false;
				else
				{
					if (base_ex[building_.get_id()].mil_pop_all < 1500)
						if (pk::choose(pk::encode(pk::format("目前总兵役人口不足({})，募集效率较低，是否继续?", base_ex[building_.get_id()].mil_pop_all)), { pk::encode(" 是 "), pk::encode(" 否 ") })==1)	return false;
				}
			}
			else
			{
				if (pk::choose(pk::encode("是否开始募集士兵?"), { pk::encode(" 是 "), pk::encode(" 否 ") }) == 1)
					return false;
			}

			pk::list<pk::person@> person_list = pk::get_idle_person_list(building_);
			if (person_list.count == 0) return false;

			// 실행무장 선택하기
			pk::list<pk::person@> person_sel;
			person_list.sort(function(a, b)
			{
				return (a.stat[武将能力_魅力] > b.stat[武将能力_魅力]); // 무장 정렬 (지력순)
			});

			person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择可执行的武将."), person_list, 1, 1);
			if (person_sel.count == 0) return false; // 미선택 시 취소 종료

			return run_order(person_sel, building_);
		}

		//======================================================AI募兵==============================================//


		void onAIRunningOrder(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 据点AI_募兵)
			{
				pk::list<pk::person@> person_sel;
				if (run_order_before(context, person_sel, building)) run_order(person_sel, building);
				else if (调试模式) pk::trace("据点AI_募兵 不满足");
			}
		}


		bool run_order_before(pk::ai_context@ context, pk::list<pk::person@>& out person_sel, pk::building@ building0)
		{
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building0);

			if (!context.base[building0.get_id()].enemies_around5 and pk::get_food(building0) < int(pk::get_troops(building0) * 1.8f)) return false;//无敌人威胁时，少粮不募兵
			if (check_avaliable(building0, person_list) != 0) {return false; }
			person_list.sort(function(a, b)//武将选择这块还得加强
			{
				return (a.stat[武将能力_魅力] > b.stat[武将能力_魅力]); // 무장 정렬 (지력순)
			});
			person_sel.add(person_list[0]);
			return true;
		}


		bool run_order(pk::list<pk::person@> person_sel, pk::building@ building0)
		{
			if (building0 is null) return false;
			if (person_sel[0] is null) return false;


			string actor_name = pk::decode(pk::get_name(person_sel[0]));
			pk::person@ young_man = pk::get_person(무장_청년);


			if (pk::is_player_controlled(building0))
			{
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人, 我们前来相助.请接受我们吧.", actor_name)), young_man);
				pk::message_box(pk::encode("哈哈哈!托各位的福, 我感觉得到了千军万马."), person_sel[0]);
			}
			else
			{
				if (pk::is_in_screen(building0.pos))
				{
					pk::say(pk::encode("敌军犯境,事态紧急!快张榜募兵!"), person_sel[0], building0);
					pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,我们前来相助.", actor_name)), young_man, building0);
				}

				pk::history_log(building0.pos, (pk::get_force(building0.get_force_id())).color, pk::encode(pk::format("\x1b[1x{}\x1b[0x在城中张榜募兵.", actor_name)));
			}

			int troops_ = 1500 + 20 * person_sel[0].stat[武将能力_魅力];
			troops_ += ch::randint(10,40) * person_sel[0].stat[武将能力_魅力];
			troops_ = int(troops_ * 募兵倍率 / 100.f);

			// 기교 감소 및 병력, 기력 상승.

			ch::add_tp(pk::get_force(building0.get_force_id()), -TP_COST, building0.get_pos());
			if (开启人口系统) ch::add_troops(building0, troops_, true,105);//募兵消耗总兵役人口
			else  ch::add_troops(building0, troops_, true);
			pk::add_energy(building0, energy_, true);
			if (pk::is_player_controlled(building0))
			{
				string t = "成功募集了壮丁\x1b[1x" + troops_ + "\x1b[0x名, 士气增加\x1b[1x" + energy_ + "\x1b[0x点。\n消耗\x1b[1x" + GOLD_COST + "\x1b[0x金,\x1b[1x" + TP_COST + "\x1b[0x技巧";
				pk::message_box(pk::encode(t), person_sel[0]);
			}

			// 행동력 감소.
			auto district = pk::get_district(building0.get_district_id());
			pk::add_ap(district, -ACTION_COST);

			base_ex[building0.get_id()].enlist_done = true;
			person_sel[0].action_done == true;

			return true;

		}

		
		//AI募兵事件
		void callback()
		{
			if (AI募兵开启)
				pk::scene(pk::scene_t(scene_AI_recruit));
		}
		
		//AI募兵功能
		void scene_AI_recruit()
		{
			for (int i = 0; i < 建筑_据点末; i++)
			{
				auto building_t = pk::get_building(i);
				
				if (building_t.is_player())
					continue;  //跳过玩家地盘
				if (!pk::enemies_around(building_t) and i < 建筑_城市末)
					continue;  //跳过非交战的城市（关、小城不跳过）
				if (pk::get_troops(building_t) > 40000 and i >= 建筑_城市末)
					continue;  //跳过兵力过高的关、小城
				if (building_t.get_force_id() == -1)
					continue;  //跳过空城无势力
				if (pk::get_taishu_id(building_t) == -1)
					continue;  //跳过无太守
				if ((pk::get_force(building_t.get_force_id())).tp < TP_COST)
					continue;  //跳过没有足够技巧
				
				int AI实际概率 = prob_recruit(building_t);
				if (!pk::rand_bool(AI实际概率))
					continue;  //不触发
				
				//pk::trace(pk::format("i等于{}", i));
				pk::person@ taishu_0 = pk::get_taishu_id(building_t) != -1 ? pk::get_person(pk::get_taishu_id(building_t)) : pk::get_person(武将_文官);
				string taishu_name = pk::decode(pk::get_name(taishu_0));
				pk::person@ young_man = pk::get_person(무장_청년);
				
				if (pk::is_in_screen(building_t.pos)) pk::say(pk::encode("敌军犯境,事态紧急!快张榜募兵!"), taishu_0, building_t);
				if (pk::is_in_screen(building_t.pos)) pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,我们前来相助.", taishu_name)), young_man, building_t);
				
				int troops_ = 2000 + 20 * taishu_0.stat[武将能力_魅力];
				troops_ += pk::rand(50) * taishu_0.stat[武将能力_魅力];
				troops_ = int(troops_ * 募兵倍率 / 100.f);
				ch::add_troops(building_t, troops_, true);

				ch::add_tp(pk::get_force(building_t.get_force_id()), -TP_COST, (pk::get_force(building_t.get_force_id())).get_pos());
				
				pk::history_log(building_t.pos, (pk::get_force(building_t.get_force_id())).color, pk::encode(pk::format("\x1b[1x{}\x1b[0x在城中张榜募兵.", taishu_name)));
			}
		}
		
		//AI募兵概率计算
		int prob_recruit(pk::building@ building_t)
		{
			int recruit_0 = AI募兵概率;
			auto arr = pk::range(building_t.get_pos(), 1, 4);
			bool player_attacking = false;  //玩家在攻城，初始化
			
			for (int i = 0; i < int(arr.length); i++)
			{
				if ((pk::get_hex(arr[i])).has_unit)
				{
					if ((pk::get_unit(arr[i])).is_player())
					{
						player_attacking = true;
						break;  //玩家在攻城
					}
				}
			}
			
			if (player_attacking)
				recruit_0 = pk::min((recruit_0 + 20), 100);   //为玩家攻城时，概率大提升
			
			//现存兵力计算是否要募兵
			if (pk::get_troops(building_t) < 5000)
				recruit_0 = pk::min((recruit_0 + 10), 100);   //兵力低于5000时，概率小提升
			else if (pk::get_troops(building_t) < 15000)
				recruit_0 = pk::min((recruit_0 + 5), 100);   //兵力低于15000时，概率微提升
			
			return recruit_0;
		}

	}
	Main main;
}
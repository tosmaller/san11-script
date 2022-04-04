// ## 2021/12/03 # 江东新风 # 概率计算参考流言，添加被俘情况，ai也会在条件满足时破坏 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/11/06 # 江东新风 # 临时增加破坏可执行距离设定 ##
// ## 2020/09/30 # 江东新风 # 港关计略city指针错误修复 ##
// ## 2020/09/23 # 氕氘氚 # 金钱65535上限修复 ##
// ## 2020/08/30 #江东新风#UTF8改写##
// ## 2020/07/26 ##
/*
@作者：江东新风
@說明：参照三国志10的破坏指令，消耗行动力，破坏敌军城市耐久

*/
namespace stratage_destroy
{


	// ================ CUSTOMIZE ================


	const int ACTION_COST = 20;   	 // 行动力消耗
	const int GOLD_COST = 1000;       // 金消耗
	const float hp_percent = 0.05f;   //基础耐久損失比例
	const int hp_rand = 100;         // 随机耐久損失增量
	const int 破坏_可执行距离 = 2;

	const int KEY = pk::hash("破坏");

	const bool 调试模式 = false;
// ===========================================


	class Main
	{

		pk::building@ building_;
		int check_result_;

		Main()
		{
			pk::bind(202, pk::trigger202_t(onAIRunningOrder));
			add_menu();

		}

		void add_menu()
		{
			pk::menu_item item;
			item.menu = 104;
			item.pos = 3;
			item.shortcut = "R";
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
			auto person_list = pk::get_idle_person_list(building_);
			check_result_ = check_avaliable(building_, person_list);
		}

		string getText()
		{
			return pk::encode("破坏");//破坏
		}

		bool isEnabled()
		{
			if (check_result_ != 0) return false;
			else return true;
		}

		string getDesc()
		{
			switch (check_result_)
			{
			case 1: return pk::encode("没有可执行的武将");
			case 2: return pk::encode(pk::format("行动力不足 (需要{}行动力)", ACTION_COST));
			case 3: return pk::encode(pk::format("资金不足 (需要{}金)", GOLD_COST));
			case 0: return pk::encode(pk::format("对敌方据点执行破坏.(消耗：金{} 行动力{})", GOLD_COST, ACTION_COST));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		int check_avaliable(pk::building@ building, pk::list<pk::person@> list)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			//pk::trace(pk::format("3.1:{}{}{}{}", pk::enemies_around(building), list.count, pk::get_district(building.get_district_id()).ap < ACTION_COST, pk::get_gold(building) < GOLD_COST));
			if (list.count == 0) return 1;
			else if (pk::get_district(building.get_district_id()).ap < ACTION_COST) return 2;
			else if (pk::get_gold(building) < GOLD_COST) return 3;
			return 0;
		}

		bool handler()
		{
			pk::person@ taishu_ = pk::get_person(pk::get_taishu_id(building_));
			if (pk::choose({ pk::encode(" 是 "), pk::encode(" 否 ") }, pk::encode(pk::format("是否执行破坏？\n(资金 {} 使用)\n(对敌军城市的耐久造成破坏效果)", GOLD_COST)), taishu_) == 1)
				return false;

			// 执行武将
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building_);
			if (person_list.count == 0) return false;
			pk::list<pk::building@> building_list = getTargetList(building_);
			
			if (building_list.count == 0)
			{
				pk::message_box(pk::encode("可执行范围内不存在可以破坏的敌军据点。"));
				return false;
			}

			// 选擇目标据点
			pk::list<pk::building@> list_target = pk::building_selector(pk::encode("选择目标据点"), pk::encode("选择目标据点."), building_list, 1, 1);
			if (list_target.count == 0) return false; // 未选擇时取消结束
			pk::building@target = list_target[0];

			person_list.sort(function(a, b)
			{
				return (a.stat[武将能力_智力] > b.stat[武将能力_智力]); // 무장 정렬 (지력순)
			});
			//武将选擇
			pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择可执行的武将."), person_list, 1, 1);
			if (person_sel.count == 0) return false; // 未选擇时取消结束
			pk::person@actor = person_sel[0];

			//run_order

			//actor.order = -1;
			//actor.target[6] = 1;
			//actor.absence_timer = 0;

			return run_order(person_sel, building_, target);

		}
		//==========================================成功率及结果========================================//
		//结果 0 成功未发现 1 成功被发现 2 失败 3 失败被捕 4 返回原概率计算
		int get_destroy_result(pk::person@ actor, pk::building@ target_building, int seed = -1)
		{
			if (!pk::is_alive(actor) || !pk::is_alive(target_building))
				return 2;
			int target_force_id = target_building.get_force_id();
			pk::force@ target_force = pk::get_force(target_force_id);
			if (!pk::is_alive(target_force))
				return 2;
			if (seed < 0)
				seed = pk::rand(10);
			int wisdom_effect = pk::get_wisdom_effect(actor, target_building, seed);
			if (wisdom_effect < -1)
				return 2;
			int defender_id = target_force.gunshi;
			int target_taishu = pk::get_taishu_id(target_building);
			if (!pk::is_valid_person_id(defender_id))
			{
				if (pk::is_valid_person_id(target_taishu))
					defender_id = target_taishu;
				else
					defender_id = target_force.kunshu;
			}

			int actor_service = actor.service;
			int actor_id = actor.get_id();
			int target_building_id = target_building.get_id();

			if (wisdom_effect / 12 > pk::rand(100, seed, actor_id, defender_id, target_building_id, actor_service, 0, 0))
				return ((wisdom_effect < 60)?1:0);                             // 这个返回1或0，是两种成功的形式
			if (pk::is_kunshu(actor) || pk::has_skill(actor, 特技_强运) || pk::has_item(actor, 宝物种类_名马))// 32 特技_强运 0确认是否有宝物坐骑
				return 2;

			return ((wisdom_effect + 9 <= pk::rand(10, actor_service, defender_id, actor_id, target_building_id, seed, 0, 0))?1:0) + 2;// 这个返回2或3，即失败或失败被擒
				
		}
		//============================================AI相关========================================//

		void onAIRunningOrder(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 据点AI_计略破坏)
			{
				int target_id = ch::get_attacking_near_enemy_base(context, building);
				if (pk::is_valid_base_id(target_id))
				{
					pk::building@ target_building = pk::get_building(target_id);
					pk::list<pk::person@> person_sel;
					if (run_order_before(context, person_sel, building, target_building)) run_order(person_sel, building, target_building);
				}
				else if (调试模式) pk::trace("据点AI_计略破坏 不满足");
			}
		}

		bool run_order_before(pk::ai_context@ context, pk::list<pk::person@>& person_sel, pk::building@ building0, pk::building@ target_building)
		{
			//判断城下部队数再破坏
			pk::ai_context_base@ base = context.base[target_building.get_id()];
			if (base.num_own_units < 1) return false;
			int max_hp = pk::get_max_hp(target_building);
			if (target_building.hp > uint16(0.5f*max_hp)) return false;//耐久过高不破坏
			array<pk::point> arr = pk::range(target_building.get_pos(), 1 + ((target_building.facility == 设施_城市) ? 1 : 0), 3 + ((target_building.facility == 设施_城市) ? 1 : 0));
			int count = 0;
			int src_force_id = building0.get_force_id();
			for (int j = 0; j < int(arr.length); j++)
			{
				pk::unit@ unit = pk::get_unit(arr[j]);
				if (unit is null) continue;
				if (src_force_id == unit.get_force_id()) count += 1;
			}
			if (count < 2) return false;//己方部队在敌军城下不超过2只，不破坏

			pk::list<pk::person@> person_list = pk::get_idle_person_list(building0);
			if (person_list.count == 0) return false;
			person_list.sort(function(a, b)
			{
				return (a.stat[武将能力_智力] > b.stat[武将能力_智力]);
			});
			person_sel.add(person_list[0]);
			if (check_avaliable(building0, person_sel) != 0) return false;

			int destroy_result = get_destroy_result(person_sel[0], target_building);
			if (destroy_result == 0 or destroy_result == 1) return true;

			return false;
		}

		bool run_order(pk::list<pk::person@> person_sel, pk::building@ src_building, pk::building@ target_building)
		{
			/**/
			int min_distance = 0;
			int max_distance = 2;
			if (target_building is null) return false;
			if (person_sel[0] is null) return false;

			int destroy_result = get_destroy_result(person_sel[0], target_building);
			switch (destroy_result)
			{
			case 0: destroy_success_undetect(person_sel[0], target_building); break;
			case 1: destroy_success_detect(person_sel[0], target_building); break;
			case 2: destroy_failed(person_sel[0], target_building); break;
			case 3: destroy_failed_capture(person_sel[0], target_building); break;
			default:
				break;
			}

			auto district = pk::get_district(src_building.get_district_id());
			pk::add_ap(district, -ACTION_COST);

			person_sel[0].action_done = true;

			pk::add_gold(src_building, -GOLD_COST, true);

			//base_ex[building0.get_id()].perturb_done = true;

			return true;

		}

		//============================================支持函数========================================//
		void destroy_success_undetect(pk::person@ actor, pk::building@ target_building)
		{
			if (!pk::is_alive(actor)) return;
			if (!pk::is_alive(target_building)) return;
			int src_force_id = actor.get_force_id();
			pk::force@src_force = pk::get_force(src_force_id);
			if (!pk::is_alive(src_force)) return;			
			int dst_force_id = target_building.get_force_id();
			pk::force@dst_force = pk::get_force(dst_force_id);
			if (!pk::is_alive(dst_force)) return;
			if (src_force.is_player())
			{
				if (pk::is_in_screen(target_building.get_pos()))
				{
					int dst_taishu_id = pk::get_taishu_id(target_building);
					pk::person@taishu = pk::get_person(dst_taishu_id);
					if (!pk::is_alive(taishu))
						@taishu = pk::get_person(武将_士兵);
					pk::say(pk::encode("城墙似乎遭到了恶意破坏。"), taishu);
				}

				string actor_name = pk::decode(pk::get_name(actor));
				string base_name = pk::get_new_base_name(target_building.get_id());
				pk::history_log(target_building.pos, dst_force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x成功对\x1b[1x{}\x1b[0x的城墙执行了破坏.", actor_name, base_name)));

			}
			if (dst_force.is_player())
			{
				if (pk::is_in_screen(target_building.get_pos()))
				{
					int dst_taishu_id = pk::get_taishu_id(target_building);
					pk::person@taishu = pk::get_person(dst_taishu_id);
					if (!pk::is_alive(taishu))
						@taishu = pk::get_person(武将_士兵);
					pk::say(pk::encode("城墙似乎遭到了恶意破坏。"), taishu);
				}

				//string actor_name = pk::decode(pk::get_name(actor));
				string base_name = pk::get_new_base_name(target_building.get_id());
				pk::history_log(target_building.pos, dst_force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x的城墙似乎遭到了破坏.", base_name)));
			}

			int hp_base = int(hp_percent * target_building.hp);
			int hp_damage = hp_base + pk::rand(hp_rand);
			pk::add_hp(target_building, -hp_damage, true);

			pk::add_stat_exp(actor, 武将能力_智力, 5);
			pk::add_kouseki(actor, 150);
			//after_person_done_order_5B9600((pk::person*)actor, 0);

			if (pk::is_alive(src_force))
				pk::add_tp(src_force, 30, actor.pos);
		}

		void destroy_success_detect(pk::person@ actor, pk::building@ target_building)
		{
			if (!pk::is_alive(actor)) return;
			if (!pk::is_alive(target_building)) return;
			int src_force_id = actor.get_force_id();
			pk::force@src_force = pk::get_force(src_force_id);
			if (!pk::is_alive(src_force)) return;
			int dst_force_id = target_building.get_force_id();
			pk::force@dst_force = pk::get_force(dst_force_id);
			if (!pk::is_alive(dst_force)) return;
			if (src_force.is_player())
			{
				if (pk::is_in_screen(target_building.get_pos()))
				{
					int dst_taishu_id = pk::get_taishu_id(target_building);
					pk::person@taishu = pk::get_person(dst_taishu_id);
					if (!pk::is_alive(taishu))
						@taishu = pk::get_person(武将_士兵);

					pk::say(pk::encode("似乎有细作对城墙进行了破坏。"), taishu);
				}

				string actor_name = pk::decode(pk::get_name(actor));
				string base_name = pk::get_new_base_name(target_building.get_id());
				pk::history_log(target_building.pos, dst_force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x成功对\x1b[1x{}\x1b[0x的城墙执行了破坏.", actor_name, base_name)));

			}
			if (dst_force.is_player())
			{
				string src_force_name = pk::decode(pk::get_name(dst_force));
				if (pk::is_in_screen(target_building.get_pos()))
				{
					int dst_taishu_id = pk::get_taishu_id(target_building);
					pk::person@taishu = pk::get_person(dst_taishu_id);
					if (!pk::is_alive(taishu))
						@taishu = pk::get_person(武将_士兵);
					//string src_force_name = pk::decode(pk::get_name(dst_force));
					pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x似乎在破坏我方城墙。", src_force_name)), taishu);
				}

				//string actor_name = pk::decode(pk::get_name(actor));
				string base_name = pk::get_new_base_name(target_building.get_id());
				pk::history_log(target_building.pos, dst_force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x的城墙遭到了{}的破坏.", base_name, src_force_name)));
			}

			int hp_base = int(hp_percent * target_building.hp);
			int hp_damage = hp_base + pk::rand(hp_rand);
			pk::add_hp(target_building, -hp_damage, true);

			pk::add_relations(src_force, dst_force_id,-8);
			pk::add_stat_exp(actor, 武将能力_智力, 5);
			pk::add_kouseki(actor, 150);
			//after_person_done_order_5B9600((pk::person*)actor, 0);

			if (pk::is_alive(src_force))
				pk::add_tp(src_force, 30, actor.pos);
		}

		void destroy_failed(pk::person@ actor, pk::building@ target_building)
		{
			if (!pk::is_alive(actor)) return;
			if (!pk::is_alive(target_building)) return;
			int src_force_id = actor.get_force_id();
			pk::force@src_force = pk::get_force(src_force_id);
			if (!pk::is_alive(src_force)) return;
			int dst_force_id = target_building.get_force_id();
			pk::force@dst_force = pk::get_force(dst_force_id);
			if (!pk::is_alive(dst_force)) return;
			if (src_force.is_player())
			{
				string actor_name = pk::decode(pk::get_name(actor));
				string base_name = pk::get_new_base_name(target_building.get_id());
				pk::history_log(target_building.pos, dst_force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x对\x1b[1x{}\x1b[0x的城墙破坏任务失败了.", actor_name, base_name)));

			}
			pk::add_relations(src_force, dst_force_id, -8);
			pk::add_stat_exp(actor, 武将能力_智力, 1);
			pk::add_kouseki(actor, 20);
		}

		void destroy_failed_capture(pk::person@ actor, pk::building@ target_building)
		{		
			if (!pk::is_alive(actor)) return;
			if (!pk::is_alive(target_building)) return;
			int taishu_id = pk::get_taishu_id(target_building);
			if (!pk::is_valid_person_id(taishu_id))
			{
				destroy_failed(actor, target_building);//无太守时不会被捕
				return;
			}
			int src_force_id = actor.get_force_id();
			pk::force@src_force = pk::get_force(src_force_id);
			if (!pk::is_alive(src_force)) return;
			int dst_force_id = target_building.get_force_id();
			pk::force@dst_force = pk::get_force(dst_force_id);
			if (!pk::is_alive(dst_force)) return;
			pk::person@taishu = pk::get_person(taishu_id);
			if (src_force.is_player())
			{
				string actor_name = pk::decode(pk::get_name(actor));
				string base_name = pk::get_new_base_name(target_building.get_id());
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x在对\x1b[2x{}\x1b[0x城墙进行破坏时被敌人发现而被捕了。", actor_name, base_name)), pk::get_person(pk::get_kunshu_id(actor)));
			}
			if (dst_force.is_player())
			{
				string actor_name = pk::decode(pk::get_name(actor));
				string base_name = pk::get_new_base_name(target_building.get_id());
				pk::message_box(pk::encode(pk::format("抓到在\x1b[1x{}\x1b[0x执行城墙破坏的名为\x1b[2x{}\x1b[0x的人了。", base_name, actor_name)), taishu);
			}
			pk::set_prisoner(actor,taishu);
		}


		pk::list<pk::building@> getTargetList(pk::building@ src)
		{
			pk::list<pk::building@> list;
			for (int i = 0; i < 建筑_据点末; i++)
			{
				pk::building@ dst = pk::get_building(i);
				int src_id = src.get_id();
				int dst_id = dst.get_id();
				//pk::trace(pk::format("对象据点{}，我方据点{}，相邻据点:{}，距离:{}",pk::decode(pk::get_name(dst)), pk::decode(pk::get_name(src)),pk::is_neighbor_base(src_id, dst_id), pk::get_building_distance(dst_id, src_id, src.get_force_id())));
				if (src_id != dst_id and /*pk::is_neighbor_base(src_id, dst_id)*/pk::get_building_distance(dst_id, src_id, src.get_force_id()) <= 破坏_可执行距离 and src.get_force_id() != dst.get_force_id())
					list.add(dst);
			}
			return list;
		}

	}

	Main main;
}
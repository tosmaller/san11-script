// ## 2021/10/29 # 江东新风 # 非超级难度不显示调查 ##
// ## 2021/03/17 # 江东新风 # 排除空白据点 ##
// ## 2020/12/13 # 江东新风 # 新特技_窥探 ##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/11/15 # messi # 优化界面，只有执行城市距离2以内的城市才能被侦查 ##
// ## 2020/11/03 # signalwolf # 套用“破坏“特技构建“调查”特技
/*
@作者：signalwolf
@說明：配合信息隐藏，可以通过调查获取指定据点信息

*/
namespace stratage_investigate
{


	// ================ CUSTOMIZE ================


	const int ACTION_COST = 20;   	 // 行动力消耗
	const int GOLD_COST = 100;       // 金消耗
	const int 调查_可执行距离 = 2;

	// 根据执行武将的智力決定成功率 (智力高於敌军太守 武将智力小於70时成功率? 60%, 大於70时成功率?70%, 大於80时成功率?80%, 大於90时成功率?90%，智力低於敌军太守，成功率?10%)
	const int HIGH_Chance_60 = 60;		   	  // 智力高於敌军太守 武将智力小於70时成功率? 60%
	const int HIGH_Chance_70 = 70;		      // 智力高於敌军太守 武将智力大於70时成功率? 70%
	const int HIGH_Chance_80 = 75;		      // 智力高於敌军太守 武将智力大於80时成功率? 80%
	const int HIGH_Chance_90 = 85;		      // 智力高於敌军太守 武将智力大於90时成功率? 90%
	const int LOW_Chance = 30;			      // 智力低於敌军太守，成功率?30%
	const int Without_Taishou_Chance = 100;

	const int KEY = pk::hash("调查");//팎뻐


// ===========================================


	class Main
	{

		pk::building@ building_;
		pk::building@ target;
		pk::force@ force_;
		pk::person@ taishu_;
		pk::person@ actor;
		pk::city@ city_;
		int building_gold;

		Main()
		{
			pk::menu_item item;
			item.menu = 104;
			item.pos = 3;
			item.shortcut = "I";
			item.init = pk::building_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(isVisible);
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
			@taishu_ = pk::get_person(pk::get_taishu_id(building));
			@city_ = pk::building_to_city(building);
			building_gold = pk::get_gold(building);
		}

		//获取各种城市列表
		pk::building@ building;
		pk::list<pk::building@> list_src_base;
		pk::list<pk::building@> list_dst_base;
		pk::list<pk::building@> list_district_base;    // 군단공략가능 거점
		pk::list<pk::building@> list_force_base;       // 아군 거점
		pk::list<pk::building@> list_enemy_base;       // 적 거점
		pk::array<int> arr_tekisei(병종_끝, 适性_C);

		void get_base_available()
		{
			// 리스트 초기화
			//list_drafts.clear();
			//list_patrol.clear();
			//list_drills.clear();

			//list_outfit0.clear();
			//list_outfit1.clear();
			//list_outfit2.clear();
			//list_outfit3.clear();

			list_district_base.clear();
			list_force_base.clear();
			list_enemy_base.clear();

			// 군단 도시 리스트에서 조건 확인
			for (int base_id = 0; base_id < 据点_末; base_id++)
			{
				pk::building@ base = pk::get_building(base_id);
				/*
				if (district.get_id() == base.get_district_id())
				{
					// 군단내정
					if (func_base_drafts(base, false) and !list_drafts.contains(base))  list_drafts.add(base);
					if (func_base_patrol(base, false) and !list_patrol.contains(base))  list_patrol.add(base);
					if (func_base_drills(base, false) and !list_drills.contains(base))  list_drills.add(base);

					// 군단생산
					if (func_base_outfit(base, weapon_outfit[0], false) and !list_outfit0.contains(base))  list_outfit0.add(base);
					if (func_base_outfit(base, weapon_outfit[1], false) and !list_outfit1.contains(base))  list_outfit1.add(base);
					if (func_base_outfit(base, weapon_outfit[2], false) and !list_outfit2.contains(base))  list_outfit2.add(base);
					if (func_base_outfit(base, weapon_outfit[3], false) and !list_outfit3.contains(base))  list_outfit3.add(base);

					// 군단공략
					if (func_unit_march(base, null, false) and !list_district_base.contains(base)) list_district_base.add(base);
				}
			}

			for (int target_id = 0; target_id < 据点_末; target_id++)
			{
				pk::building@ target = pk::get_building(target_id);
				if (building.get_force_id() == target.get_force_id() and !list_force_base.contains(target))
					list_force_base.add(target);
			}*/

				for (int target_id = 0; target_id < 据点_末; target_id++)
				{
					pk::building@ target = pk::get_building(target_id);
					if (pk::is_enemy(building, target))
					{
						pk::array<pk::building@> arr_force_base = pk::list_to_array(list_force_base);
						for (int i = 0; i < int(arr_force_base.length); i++)
						{
							pk::building@ base0 = arr_force_base[i];
							if (pk::is_neighbor_base(base0.get_id(), target.get_id()) and !list_enemy_base.contains(target))
								list_enemy_base.add(target);
						}
					}
				}

			}
		}

		bool isVisible()
		{
			if (pk::get_scenario().difficulty != 难易度_超级) return false;
			return true;
		}

		bool isEnabled()
		{
			if (pk::get_idle_person_list(building_).count == 0) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			else if (building_gold < GOLD_COST) return false;
			return true;
		}

		string getText()
		{
			return pk::encode("调查");//调查
		}

		string getDesc()
		{
			if (pk::get_idle_person_list(building_).count == 0)
				return pk::encode("没有可执行的武将.");
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (需要{}行动力)", ACTION_COST));
			else if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (需要{}金)", GOLD_COST));
			else
				return pk::encode(pk::format("执行调查. (行动力 {})", ACTION_COST));
		}

		bool handler()
		{

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("是否执行调查？\n(资金 {} 使用)\n(对敌军城市的信息进行调查)", GOLD_COST)), taishu_) == 1)
				return false;

			// 执行武将
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building_);
			if (person_list.count == 0) return false;
			pk::list<pk::building@> building_list = getTargetList(building_);

			if (building_list.count == 0) return false;

			// 실행무장 선택하기
			if (pk::is_player_controlled(building_))
			{
				list_src_base.clear();
				list_dst_base.clear();

				// 목표 적 거점
				list_dst_base = list_enemy_base;

				// 选擇目标据点

				pk::list<pk::building@> list_target = pk::building_selector(pk::encode("选择目标据点"), pk::encode("选择目标据点."), building_list, 1, 1);
				if (list_target.count == 0) return false; // 未选擇时取消结束
				@target = pk::get_building(list_target[0].get_id());

				//武将选擇
				pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择可执行的武将."), person_list, 1, 1);
				if (person_sel.count == 0) return false; // 未选擇时取消结束
				@actor = pk::get_person(person_sel[0].get_id());
			}
			else
			{
				person_list.sort(function(a, b)
				{
					return (a.stat[武将能力_智力] > b.stat[武将能力_智力]); // 무장 정렬 (지력순)
				});
				@actor = pk::get_person(person_list[0].get_id());
			}
			actor.order = -1;
			//actor.target[6] = 1;
			actor.absence_timer = 0;

			// 根据执行武将的智力決定成功率 (智力高於敌军太守 武将智力小於70时成功率? 60%, 大於70时成功率?70%, 大於80时成功率?80%, 大於90时成功率?90%，智力低於敌军太守，成功率?10%)
			pk::person@ taishou = pk::get_person(pk::get_taishu_id(target));
			bool flag_investigate_success = false;

			//if ( taishou.has_skill(특기_침착) or taishou.has_skill(특기_명경) or taishou.has_skill(특기_통찰) or taishou.has_skill(특기_신산) ) continue;


			// 智力高於敌军太守 武将智力小於70时成功率? 60%, 大於70时成功率?70%, 大於80时成功率?80%, 大於90时成功率?90%
			if (actor.stat[武将能力_智力] > taishou.stat[武将能力_智力])
			{

				if ((actor.stat[武将能力_智力] < 70) and pk::rand_bool(HIGH_Chance_60 + 窥探调查成功率提升))
				{
					flag_investigate_success = true;
				}

				if ((actor.stat[武将能力_智力] >= 70 and actor.stat[武将能力_智力] < 80) and pk::rand_bool(HIGH_Chance_70 + 窥探调查成功率提升))
				{
					flag_investigate_success = true;
				}

				if ((actor.stat[武将能力_智力] >= 80 and actor.stat[武将能力_智力] < 90) and pk::rand_bool(HIGH_Chance_80 + 窥探调查成功率提升))
				{
					flag_investigate_success = true;
				}

				if ((actor.stat[武将能力_智力] >= 90) and pk::rand_bool(HIGH_Chance_90 + 窥探调查成功率提升))
				{
					flag_investigate_success = true;
				}

			}

			// 智力低於敌军太守，成功率?10%
			if (actor.stat[武将能力_智力] <= taishou.stat[武将能力_智力])
			{
				if (pk::rand_bool(LOW_Chance + 窥探调查成功率提升))
				{
					flag_investigate_success = true;
				}
			}

			if (flag_investigate_success)
			{

				pk::say(pk::encode("哈哈哈! 计谋成功了."), actor, building_);
				int base_hp = target.hp;
				int base_troop = pk::get_troops(target);
<<<<<<< HEAD
				int weapon_qty1 = pk::get_weapon_amount(target, 병기_창);
				int weapon_qty2 = pk::get_weapon_amount(target, 병기_극);
				int weapon_qty3 = pk::get_weapon_amount(target, 병기_노);
				int weapon_qty4 = pk::get_weapon_amount(target, 병기_군마);
=======
				int weapon_qty1 = pk::get_weapon_amount(target, 兵器_枪);
				int weapon_qty2 = pk::get_weapon_amount(target, 兵器_戟);
				int weapon_qty3 = pk::get_weapon_amount(target, 兵器_弩);
				int weapon_qty4 = pk::get_weapon_amount(target, 兵器_战马);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				string city_name = pk::decode(pk::get_name(target));
				pk::message_box(pk::encode(pk::format("据细作来报:\x1b[2x{}\x1b[0x耐久是\x1b[1x{}\x1b[0x,士兵数量是\x1b[1x{}\x1b[0x,\n           枪的数量是\x1b[1x{}\x1b[0x,戟的数量是\x1b[1x{}\x1b[0x,\n           弓的数量是\x1b[1x{}\x1b[0x,马的数量是\x1b[1x{}\x1b[0x.", city_name, base_hp, base_troop, weapon_qty1, weapon_qty2, weapon_qty3, weapon_qty4)));
			}
			else
			{
				pk::say(pk::encode("很可惜计谋失败了."), actor, building_);
			}


			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			// 실행무장 행동완료
			actor.action_done = true;

			// 금 감소
			pk::add_gold(building_, -GOLD_COST, true);

			return true;

		}
		pk::list<pk::building@> getTargetList(pk::building@ src)
		{
			pk::list<pk::building@> list;
			for (int i = 0; i < 建筑_据点末; i++)
			{
				pk::building@ dst = pk::get_building(i);
				int src_id = src.get_id();
				int dst_id = dst.get_id();
				//pk::trace(pk::format("对象据点{}，我方据点{}，相邻据点:{}，距离:{}", pk::decode(pk::get_name(dst)), pk::decode(pk::get_name(src)), pk::is_neighbor_base(src_id, dst_id), pk::get_building_distance(dst_id, src_id, src.get_force_id())));
				if (src_id != dst_id and /*pk::is_neighbor_base(src_id, dst_id)*/dst.get_force_id() != -1 and pk::get_building_distance(dst_id, src_id, src.get_force_id()) <= 调查_可执行距离 and src.get_force_id() != dst.get_force_id())
					list.add(dst);
			}
			return list;
		}

	}

	Main main;
}
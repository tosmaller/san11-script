// ## 2021/12/10 # 江东新风 # 祭祀消耗减少，限制1年一次 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/09/21 # 江东新风 # 添加法治效果 ##
// ## 2020/08/08 # 氕氘氚 # 加入【国事】菜单 ##
/*
@제작자: HoneyBee
@설명: 도시에서 제사를 지내어 치안 상승 및 기교P를 대량으로 획득하는 SCRIPT

※ 그동안 제가 제작한 SCRIPT의 상당수가 기교P를 많이 요구하는 경우가 많아서
  기교P를 단번에 대량으로 획득할 수 있는 SCRIPT를 제작하였습니다.

*/

namespace 国事指令_祭祀
{
	// ================ CUSTOMIZE ================

	const int ACTION_COST = 30;     // (행동력 필요량: 기본 80 설정)	
	const int GOLD_COST = 3000;    	    // (금 사용량: 기본 5000 설정)

	// TP는 상승치는 설정된 값의 절반만큼 상승합니다. (EX: 1000으로 설정해도 500만 상승, 500으로 설정해도 250만 상승)
	const int TP_ = 2000;    	    // (제사를 지내고 얻는 기교P 획득량: 기본 2000 설정)
	const int safety_ = 50;    	    // (제사를 지내고 상승하는 치안 상승치: 기본 50 설정)

	// 특기_조합을 true로 설정 시, 특기_비조합을 false로 설정해야 됨.
	const bool 특기_조합 = true;      // 拥有祈愿、风水、仁政特技的武将们必须同属一个城市才可行   기원, 풍수, 인정 특기를 소유한 무장들이 한 도시에 함께 소속되어 있어야 실행 가능

	const int KEY = pk::hash("祭祀");

	// ===========================================


	class Main
	{

		pk::building@ building_;
		pk::person@ kunshu_;
		int check_result_;

		Main()
		{
			add_menu();
			pk::bind(110, pk::trigger110_t(onNewYear));
		}

		void add_menu()
		{
			pk::menu_item item;
			item.menu = global_menu::菜单_国事;
			//ch::u8debug(pk::format("menu: ji si {}", global_menu::菜单_国事));
			item.init = pk::building_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(isVisible);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		void onNewYear()
		{
			for (int i = 0; i < 非贼势力_末; ++i) force_ex[i].jisi_done = false;
		}

		void init(pk::building@ building)
		{
			@building_ = @building;
			check_result_ = check_avaliable(building);
		}

		bool isVisible()
		{
			if (building_.get_id() >= 城市_末) return false;
			return true;
		}

		string getText()
		{
			return pk::encode("祭祀");  //jisi
		}

		int check_avaliable(pk::building@ building)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			int building_id = building.get_id();		
			
			if (!pk::is_valid_city_id(building_id) or !pk::is_valid_normal_force_id(building.get_force_id())) return 1;
			@kunshu_ = pk::get_person(pk::get_kunshu_id(building));
			if (building_id != kunshu_.service or pk::is_absent(kunshu_) or pk::is_unitize(kunshu_) or kunshu_.action_done) return 2;
			else if (force_ex[building.get_force_id()].jisi_done) return 3;
			else if (int(pk::get_district(building.get_district_id()).ap) < ACTION_COST) return 4;
			else if (pk::get_gold(building) < GOLD_COST) return 5;
			else if (!check_skill(building)) return 6;
			return 0;
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
			case 1: return pk::encode("仅可在己方城市执行");
			case 2: return pk::encode("君主不在该城市中或已行动. ");
			case 3: return pk::encode("今年已执行过祭祀. ");
			case 4: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			case 5: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			case 6: return pk::encode("城市需同时有未行动的持有祈愿,风水,仁政特技的武将。");
			case 0: return pk::encode(pk::format("进行祭祀. (消耗行动力{} 金{})", ACTION_COST, GOLD_COST));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		bool handler()
		{
			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("是否要进行祭祀?\n(消耗行动力{} 金{})\n(获得大量技巧P)", ACTION_COST, GOLD_COST)), kunshu_) == 1)
				return false;
			auto actor_person_list = get_skill_person(building_);
			pk::scene(pk::scene_t(scene_제사));

			ch::add_tp(pk::get_force(building_.get_force_id()), TP_, building_.get_pos());

			ch::add_public_order(building_, safety_, true);

			auto district = pk::get_district(building_.get_district_id());
			pk::add_ap(district, -ACTION_COST);

			pk::add_gold(building_, -GOLD_COST, true);
			force_ex[building_.get_force_id()].jisi_done = true;
			kunshu_.action_done = true;
			for (int i = 0; i < actor_person_list.count; ++i)
			{
				actor_person_list[i].action_done = true;
			}
			return true;
		}

		void scene_제사()
		{
			pk::background(2);

			pk::person@ reporter_ = pk::get_person(무장_문관);

			pk::message_box(pk::encode("主公,又到了祭祀的时候了."), reporter_);
			pk::message_box(pk::encode("好...祈祷今年能风调雨顺!"), kunshu_);
			pk::message_box(pk::encode("真诚的祝祷上天会听见的."), reporter_);
			pk::message_box(pk::encode("我将斋戒沐浴以示诚意."), kunshu_);
			pk::message_box(pk::encode("祈求上天让百姓丰衣足食吧."), kunshu_);

			pk::background(-1);
		}

		bool check_skill(pk::building@ building)
		{
			int building_id = building.get_id();
			uint mibun_flag = pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般);
			array<bool> has_skill_check(3, false);
			array<uint8> skill_info = {特技_祈愿, 特技_风水, 特技_仁政};
			for (int i = 0; i < 非贼武将_末; ++i)
			{
				pk::person@person = pk::get_person(i);
				if (pk::is_alive(person) and person.service == building_id and pk::check_mibun(person, mibun_flag))
				{
					for (int j = 0; j < 3; ++j)
					{
						if (pk::is_absent(person) or pk::is_unitize(person) or person.action_done) continue;
						if (!has_skill_check[j] and ch::has_skill(person, skill_info[j])) has_skill_check[j] = true;
					}			
				}
			}
			for (int i = 0; i < 3; ++i) { if (!has_skill_check[i]) return false; }
			return true;
		}

		pk::list<pk::person@> get_skill_person(pk::building@ building)
		{
			int building_id = building.get_id();
			uint mibun_flag = pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般);
			array<bool> has_skill_check(3, false);
			array<uint8> skill_info = { 特技_祈愿, 特技_风水, 特技_仁政 };
			pk::list<pk::person@> temp;
			for (int i = 0; i < 非贼武将_末; ++i)
			{
				pk::person@person = pk::get_person(i);
				if (pk::is_alive(person) and person.service == building_id and pk::check_mibun(person, mibun_flag))
				{
					for (int j = 0; j < 3; ++j)
					{
						if (pk::is_absent(person) or pk::is_unitize(person) or person.action_done) continue;
						if (!has_skill_check[j] and ch::has_skill(person, skill_info[j])) {
							has_skill_check[j] = true;
							temp.add(person);
						} 
					}
				}
			}
			//for (int i = 0; i < 3; ++i) { if (!has_skill_check[i]) return false; }
			return temp;
		}
	}

	Main main;
}
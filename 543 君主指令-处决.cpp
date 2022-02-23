// ## 2021/02/25 # 江东新风 # 处决系统改写，此处不用了 ##
// ## 2021/02/16 # 江东新风 # 处决系统不再仇视自己 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/09 # 氕氘氚 # 加入【君主】菜单 ##
// ## 2020/08/06 #配合仇視系统，如果处決武将，将会根据义理以一定?率将君主視?厌恶武将#
// ## 2020/07/26 ##
namespace 처형
{

	// ========= CUSTOMIZE ==========

		// 기교P 상승량은 설정된 수치의 절반으로 계산됩니다. (EX: 200 = 100P)
	const int TP_COST = 200; 	 // 기교P 상승량 (1人 = 100P 상승) 
	const int ACTION_COST = 30;  // 행동력 필요량
	//仇视系统改写，此处不用了
	//const bool 处決仇視开关 = false;  //是否开?处決时将君主視?厌恶武将
	//bool 처형_인간관계_의형제 = true; // 무장 사망시 의형제무장의 혐오무장 등록
	//bool 처형_인간관계_배우자 = true; // 무장 사망시 배우자무장의 혐오무장 등록
	//bool 처형_인간관계_혈연 = true; // 무장 사망시 혈연무장의 혐오무장 등록
	//bool 처형_인간관계_후계자 = true; // 군주 사망시 후임군주의 혐오무장 등록


		// ==============================

	class Main
	{

		pk::building@ building_;
		pk::force@ force_;

		Main()
		{
			pk::menu_item item;
			item.menu = 105;
			item.pos = 6;
			item.shortcut = "W";
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
		}

		bool isVisible()
		{
			return !pk::is_campaign();
		}

		bool isEnabled()
		{
			return getTargetList().count > 0 and pk::get_district(pk::get_district_id(force_, 1)).ap >= ACTION_COST;
		}

		string getText()
		{
			return pk::encode("处决");  //chujue
		}

		string getDesc()
		{
			if (getTargetList().count == 0)
				return pk::encode("没有可处决的武将.");
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("处决武将.(行动力 {})", ACTION_COST));
		}

		bool handler()
		{
			pk::list<pk::person@> target_list = getTargetList();
			pk::list<pk::person@> temp_list;

			target_list.sort(function(a, b)
			{
				if (a.mibun != b.mibun)
					return a.mibun > b.mibun;
				if (a.loyalty != b.loyalty)
					return a.loyalty < b.loyalty;
				return a.p < b.p;
			});

			// 처형할 무장 선택.
			while (true)
			{
				temp_list = pk::person_selector(pk::encode("选择武将"), pk::encode("请选择要处决的武将."), target_list, 1, target_list.count, temp_list);
				if (temp_list.count == 0)
					return false;
				if (pk::yes_no(pk::encode("真的要处决嘛?")))
					break;
				// 취소를 눌렀을 때 한 명일 경우 선택 복구하지 않음.
				if (temp_list.count == 1)
					temp_list.clear();
			}

			target_list = temp_list;

			pk::person@ kunshu = pk::get_person(force_.kunshu);
			int force_id = force_.get_force_id();
			array<int> death_count(세력_끝, 0);

			// 기교 감소.
			ch::add_tp(force_, target_list.count * TP_COST, force_.get_pos());

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			// 처형.
			for (int i = 0; i < target_list.count; i++)
			{
				pk::person@ target = target_list[i];
				pk::play_se(5);
				pk::message_box(pk::get_msg(pk::msg_param(5929, target, kunshu)), target);
				int target_force_id = target.get_force_id();
				if (pk::is_valid_force_id(target_force_id))
					death_count[target_force_id]++;
				pk::kill(target, kunshu, pk::get_hex_object(target.location), null, 2);
				//仇视系统改写，此处不用了
				//if (处決仇視开关)
				//	setDislike(target);

			}

			// 자세력 무장 충성도 감소.
			int turn_counter = pk::get_scenario().turn_counter;
			// 155번 월별 충성도 감소 함수를 위해 1월로 변경함.
			pk::get_scenario().turn_counter = turn_counter + (13 - pk::get_month()) * 3;
			for (int i = 0; i < 据点_末; i++)
			{
				pk::building@ building = pk::get_building(i);
				if (pk::is_alive(building) and building.get_force_id() == force_id)
				{
					temp_list = pk::get_person_list(building, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般));
					for (int j = 0; j < temp_list.count; j++)
						pk::add_loyalty(temp_list[j], cast<pk::func155_t@>(pk::get_func(155, true))(building, temp_list[j]));
				}
			}
			pk::get_scenario().turn_counter = turn_counter;

			// 처형시킨 세력과의 우호도 악화.
			for (int i = 0; i < 세력_끝; i++)
			{
				if (i != force_id)
					pk::add_relations(force_, i, death_count[i] > 0 ? -100 : 0);
			}

			return true;
		}

		pk::list<pk::person@> getTargetList()
		{
			pk::list<pk::person@> list;
			int force_id = force_.get_id();
			for (int i = 0; i < 무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person))
				{
					if (person.get_force_id() == force_id)
					{
						if (!pk::is_absent(person) and !pk::is_unitize(person) and person.mibun != 身份_君主 and person.mibun != 신분_포로)
							list.add(person);
					}
					else if (person.mibun == 신분_포로)
					{
						pk::hex_object@ hexobj = pk::get_hex_object(person.location);
						if (pk::is_alive(hexobj) and hexobj.get_force_id() == force_id)
							list.add(person);
					}
				}
			}
			return list;
		}

		//仇视系统改写，此处不用了
		/*
		//lose_person_array 0死亡人?，1,对应势力,2,死亡人身?，3,好友
		void setDislike(pk::person@ lose_person)
		{
			pk::person@ kunshu = pk::get_person(force_.kunshu);
			for (int i = 0; i < 무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person) && person.get_id() != lose_person.get_id() && isFriend(lose_person, person) && force_.kunshu != i)
				{
					if (pk::rand_bool(pk::min((person.giri * 20 + 25), 100)))
					{
						pk::add_dislike(person, kunshu.get_id());
						pk::history_log(pk::get_current_turn_force_id(), pk::get_force(person.get_force_id()).color, pk::encode(pk::format("\x1b[1x{}\x1b[0x将\x1b[2x{}\x1b[0x视为厌恶武将", pk::decode(pk::get_name(person)), pk::decode(pk::get_name(kunshu)))));
					}
				}
			}

		}

		//判断是否是义兄弟，夫妻，血缘
		bool isFriend(pk::person@ person1, pk::person@ person2)
		{
			bool result = false;

			if (pk::is_gikyoudai(person1, person2.get_id()) && 처형_인간관계_의형제)
				result = true;
			if (pk::is_fuufu(person1, person2.get_id()) && 처형_인간관계_배우자)
				result = true;
			if (pk::is_ketsuen(person1, person2.get_id()) && 처형_인간관계_혈연)
				result = true;
			if (pk::is_oyako(person1, person2.get_id()) && 처형_인간관계_혈연)
				result = true;

			return result;
		}*/
	}

	Main main;
}
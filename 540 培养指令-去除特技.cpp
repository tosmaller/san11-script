// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/09 # 氕氘氚 # 加入【培养】菜单 ##
/*
@제작자: HoneyBee
@설명: 도시 内 무장들의 특기를 무특기로 변경 (도시에 태수가 없으면 실행 불가)

	 => 신분이 군주,태수,일반인 무장들의 특기를 모두 변경 (특기_제거 커맨드를 실행하는 도시에서만 변경됨)

*/

namespace 특기_제거
{


	// ================ CUSTOMIZE ================

	const int TP_COST = 50; 	   // (기교P 필요량: 기본 50 설정 / 1人 기준)
	const int ACTION_COST = 50;    // (행동력 필요량: 기본 50 설정)	
	const int GOLD_COST = 2000;    // (금 필요량: 기본 2000 설정)

	const int KEY = pk::hash("伴砍坦祸");

	// ===========================================


	class Main
	{

		pk::building@ building_;
		pk::force@ force_;
		pk::person@ taishu_;
		pk::city@ city_;
		uint16 building_gold;
		pk::list<pk::person@> person_list;

		Main()
		{
			pk::menu_item item;
			item.menu = global_menu::菜单_培养;
			//ch::u8debug(pk::format("menu: qu chu te ji {}", global_menu::菜单_培养));
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
			@taishu_ = pk::get_person(pk::get_taishu_id(building));
			@city_ = pk::building_to_city(building);
			building_gold = pk::get_gold(building);

		}


		bool isEnabled()
		{
			if (!pk::is_alive(taishu_)) return false;
			else if (force_.tp < TP_COST) return false;
			else if (building_gold < GOLD_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		string getText()
		{
			return pk::encode("去除特技");  //quchu teji
		}

		string getDesc()
		{
			if (!pk::is_alive(taishu_))
				return pk::encode(pk::format("没有可执行的武将. (行动力 {})", ACTION_COST));
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			else
				return pk::encode(pk::format("去除武将已有的特技.(技巧P {}/人, 行动力 {})", TP_COST, ACTION_COST));
		}

		bool handler()
		{
			pk::list<pk::person@> target_list = getTargetList();
			pk::list<pk::person@> temp_list;

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("给武将新的机会\n给他们什么?\n(去除武将已有的特技?)"), taishu_) == 1)
				return false;

			bool taishu_confirm = false;

			while (!taishu_confirm)
			{

				temp_list = pk::person_selector(pk::encode("选择武将"), pk::encode(pk::format("请选择要去除特技的武将.({}名)", force_.tp / TP_COST)), target_list, 1, pk::min(target_list.count, force_.tp / TP_COST), temp_list);

				if (temp_list.count == 0)
					return false;

				if (pk::yes_no(pk::encode("真的要删除特技吗?")))
					break;

				// 취소를 눌렀을 때 한 명일 경우 선택 복구하지 않음.
				if (temp_list.count == 1)
					temp_list.clear();
			}

			target_list = temp_list;

			// 기교 및 금 감소
			ch::add_tp(force_, target_list.count * -TP_COST, force_.get_pos());
			pk::add_gold(city_, -GOLD_COST, true);

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);


			// 특기 제거.
			for (int i = 0; i < target_list.count; i++)
			{
				pk::person@ target = target_list[i];

				// 특기를 보유한 무장들을 무특기로 변경
				if (target.skill != -1)
				{
					target.skill = -1;
				}

			}

			pk::message_box(pk::encode("删除了所有选择武将的特技."));

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
						if (!pk::is_absent(person) and !pk::is_unitize(person) and person.mibun != 신분_포로 and person.skill != -1)
							list.add(person);
					}
				}
			}
			return list;
		}

	}

	Main main;
}
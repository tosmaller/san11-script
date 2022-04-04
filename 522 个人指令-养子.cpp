// ## 2020/12/11 # 白马叔叔 # 赐姓功能、称谓优化 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ##2020/09/16 # 江东新风 # 使用条件对君主行动状态进行判定 ##
// ## 2020/08/08 # 氕氘氚 ##
/*
@제작자: HoneyBee
@설명: 특정 무장을 군주의 양자/양녀로 삼을 수 있는 SCRIPT


※ 양자/양녀로 삼을 수 있는 무장의 조건
- <군주>보다 나이가 어린 무장
- 혈연관계가 아닌 무장
- 의형제가 아닌 무장
- 부부가 아닌 무장
- 혐오관계가 아닌 무장


*/

namespace 양자
{


	// ================ CUSTOMIZE ================

	const int ACTION_COST = 50;     	// (행동력 필요량: 기본 50 설정)	
	const int TP_COST = 100;    	   	// (기교P 필요량: 기본 100 설정)

	bool 혈연_설정 = true;    	   			// (군주의 혈연으로 설정할지 여부, true(on), false(off))
	bool 세대_설정 = true;    	   			// (군주의 2세로 설정할지 여부, true(on), false(off))

	const int KEY = pk::hash("泉大");

	// ===========================================


	class Main
	{

		pk::building@ building_;
		pk::building@ kunshu_building;
		pk::force@ force_;
		pk::person@ kunshu_;
		pk::person@ gunshi_;
		pk::person@ foster_son;
		pk::person@ support;
		pk::city@ kunshu_city;
		pk::city@ city_;
		string foster_son_name;
		pk::list<pk::person@> person_list;
		array<pk::person@> person_sel_arr;

		Main()
		{
			pk::menu_item item;
			item.menu = global_menu::菜单_个人;
			//ch::u8debug(pk::format("menu: yang zi {}", global_menu::菜单_个人));
			item.shortcut = "3";
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
			@kunshu_ = pk::get_person(pk::get_kunshu_id(building));
			@gunshi_ = pk::get_person(force_.gunshi);
			@kunshu_building = pk::get_building(kunshu_.service);
			@kunshu_city = pk::building_to_city(kunshu_building);
			@city_ = pk::building_to_city(building);

			@support = pk::is_alive(gunshi_) ? gunshi_ : pk::get_person(무장_문관);
		}

		bool isVisible()
		{
			if (kunshu_.service != building_.get_id()) return false;
			return true;
		}

		bool isEnabled()
		{
			if (building_.get_id() != kunshu_.service) return false;
			else if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_) or kunshu_.action_done) return false;
			else if (force_.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		string getText()
		{
			return pk::encode("养子");
		}

		string getDesc()
		{
			if (building_.get_id() != kunshu_.service)
				return pk::encode("只能在君主所在城市或港关实行.");
			else if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_) or kunshu_.action_done)
				return pk::encode("君主已行动或不在城市中.");
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("收养义子. (行动力 {})", ACTION_COST));
		}

		bool handler()
		{

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("要收养义子吗?\n(技巧P {} )", TP_COST)), kunshu_) == 1)
				return false;


			bool person_confirm = false;

			person_list = getTargetList();

			while (!person_confirm)
			{
				// 무장 선택 창을 열어서 선택하기
				person_sel_arr = pk::list_to_array(pk::person_selector(pk::encode("武将选择"), pk::encode("请选择做为养子的武将."), person_list, 1, 1));

				// 무장 선택
				@foster_son = person_sel_arr[0];

				// 선택 취소 시 종료
				if (!pk::is_alive(foster_son)) return false;

				// 선택된 무장의 이름
				foster_son_name = pk::decode(pk::get_name(foster_son));

				person_confirm = pk::yes_no(pk::encode(pk::format("收养\x1b[1x{}\x1b[0x做为养子吗?", foster_son_name)));
			}

			// 혈연 설정
			if (혈연_설정) foster_son.ketsuen = kunshu_.ketsuen;

			// 세대 설정
			if (세대_설정) foster_son.generation = kunshu_.generation + 1;

			// 부모 설정
			if (kunshu_.sex == 성별_남)
				foster_son.father = force_.kunshu;
			else if (kunshu_.sex == 성별_여)
				foster_son.mother = force_.kunshu;

			foster_son.update();

			pk::scene(pk::scene_t(scene_Event));

			if (foster_son.sex == 성별_남)
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x成为了养子.", foster_son_name)));
			else if (foster_son.sex == 성별_여)
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x成为了养女.", foster_son_name)));

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			// 기교P 감소
			ch::add_tp(force_, -TP_COST, force_.get_pos());

			kunshu_.action_done = true;

			return true;

		}

		void scene_Event()
		{
			if (foster_son.get_id() == gunshi_.get_id()) @support = pk::get_person(무장_문관);

			pk::diplomacy(kunshu_, support, foster_son, pk::diplomacy_t(scene_Event_1));
			pk::move_screen(kunshu_city.get_pos(), 2000);
		}

		void scene_Event_1()
		{
			string call_kunshu = "主公";
			switch (force_.title)
			{
			case 0: call_kunshu = "陛下"; break;
			case 1: call_kunshu = "王上"; break;
			default:  call_kunshu = "主公"; break;
			}
			string self_kunshu = "我";
			switch (force_.title)
			{
			case 0: self_kunshu = "朕"; break;
			case 1: self_kunshu = "孤王"; break;
			default:  self_kunshu = "我"; break;
			}
			string self_foster_son = "我";
			if (foster_son.base_stat[1] >= 75)
			{
				self_foster_son = "末将";
			}
			else if (foster_son.base_stat[2] >= 75 and force_.title == 0)
			{
				self_foster_son = "微臣";
			}
			else
			{
			}
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,您传唤\x1b[1x{}\x1b[0x?", call_kunshu, self_foster_son)), foster_son);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,你来了?", foster_son_name)), kunshu_);

			if (foster_son.sex == 성별_남)
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x叫你过来是想收你为养子.", self_kunshu)), kunshu_);
			else if (foster_son.sex == 성별_여)
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x叫你过来是想收你为养女.", self_kunshu)), kunshu_);

			pk::message_box(pk::encode(pk::format("对{}这样卑微的人来说,\n这简直和做梦一样...", self_foster_son)), foster_son);
			pk::message_box(pk::encode(pk::format("以后我也会将{}当作亲生父母一样侍奉.", call_kunshu)), foster_son);
			pk::message_box(pk::encode("非常好."), kunshu_);

			if (foster_son.sex == 성별_남)
			{
				pk::message_box(pk::encode(pk::format("现在开始你就是{}的儿子了.", self_kunshu)), kunshu_);
				pk::message_box(pk::encode(pk::format("恭喜\x1b[1x{}\x1b[0x收\x1b[1x{}\x1b[0x为养子.", call_kunshu, foster_son_name)), support);
			}
			else if (foster_son.sex == 성별_여)
			{
				pk::message_box(pk::encode(pk::format("现在开始你就是{}的女儿了.", self_kunshu)), kunshu_);
				pk::message_box(pk::encode(pk::format("恭喜\x1b[1x{}\x1b[0x收\x1b[1x{}\x1b[0x为养女.", call_kunshu, foster_son_name)), support);
			}

			//赐名
			if (force_.title <= 1)
			{
				bool	confirm = pk::yes_no(pk::encode("是否给养子女赐\x1b[1x姓\x1b[0x?"));
				if (confirm == true)
				{
					foster_son.azana_read = foster_son.sei + foster_son.mei;
					foster_son.sei = kunshu_.sei;
					foster_son.update();
					string foster_son_name_new = pk::decode(pk::get_name(foster_son));
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x被赐名为\x1b[1x{}\x1b[0x.", foster_son_name, foster_son_name_new)));
					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode("..."), foster_son); break;
					case 1: pk::message_box(pk::encode(pk::format("谢{}赐姓!", call_kunshu)), foster_son); break;
					}

				}
			}

		}


		pk::list<pk::person@> getTargetList()
		{
			pk::list<pk::person@> list;
			int force_id = force_.get_id();
			int kunshu_0 = kunshu_.get_id();
			for (int i = 0; i < 무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person))
				{
					if (person.get_force_id() == force_id)
					{
						if (!pk::is_absent(person) and !pk::is_unitize(person)
							and person.mibun != 신분_포로 and (person.birth > kunshu_.birth)
							and !pk::is_fuufu(person, kunshu_0)
							and !pk::is_gikyoudai(person, kunshu_0)
							and !pk::is_ketsuen(person, kunshu_0)
							and !pk::is_dislike(person, kunshu_0))
							list.add(person);
					}
				}
			}
			return list;
		}

	}

	Main main;
}
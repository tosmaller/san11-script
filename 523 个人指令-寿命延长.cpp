// ## 2021/12/09 # 江东新风 # 修复港关bug，写法改进 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/08 # 氕氘氚 # 加入【个人】菜单 ##
/*
@제작자: HoneyBee
@설명: 무장의 수명을 연장시킬 수 있는 SCRIPT (수명은 5년 ~ 10년 사이로 RANDOM으로 증가)

	 ※ 삼국지연의에서 관로(북두, 남두)의 일화 및 제갈량의 수명연장 기도 일화 참고

	 (북두, 남두를 발견하지 못하면 대신에 도시 치안 상승)

*/

namespace 寿命延长
{


	// ================ CUSTOMIZE ================

	const int TP_COST = 500; 	   // (기교P 필요량: 기본 500 설정)
	const int ACTION_COST = 80;    // (행동력 필요량: 기본 80 설정)	

	const int 南斗北斗未发现概率 = 50;     // 북두, 남두를 만나지 못할 확률 (기본 설정 - 50%)

	const int safety_ = 50;        // 북두, 남두를 만나지 못한 대신에 치안 상승

	const int KEY = pk::hash("织袓噌择");

	// ===========================================


	class Main
	{

		pk::building@ building_;
		pk::person@ taishu_;
		int check_result_;
		int debate_result;
		array<int> arr_stat = { 70, 75, 80, 85, 90 };

		Main()
		{
			add_menu();
		}

		void add_menu()
		{
			pk::menu_item item;
			item.menu = global_menu::菜单_个人;
			//ch::u8debug(pk::format("menu: shou ming yan chang {}", global_menu::菜单_个人));
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
			check_result_ = check_avaliable(building);
		}

		string getText()
		{
			return pk::encode("寿命延长");  //shouming yanchang
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
			case 1: return pk::encode("无太守无法执行");
			case 2: return pk::encode("太守不在或已行动. ");
			case 3: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			case 5: return pk::encode(pk::format("技巧不足.(必须{} 气力)", TP_COST));
			case 0: return pk::encode(pk::format("进行寿命延长.(技巧P {}, 行动力 {})", TP_COST, ACTION_COST));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		int check_avaliable(pk::building@ building)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			int taishu_id = pk::get_taishu_id(building);
			if (!pk::is_valid_person_id(taishu_id)) return 1;
			@taishu_ = pk::get_person(taishu_id);
			if (pk::is_absent(taishu_) or pk::is_unitize(taishu_) or taishu_.action_done) return 2;
			else if (pk::get_district(building.get_district_id()).ap < ACTION_COST) return 3;
			else if (pk::get_force(building.get_force_id()).tp < TP_COST) return 4;
			else return 0;
		}

		bool handler()
		{
			pk::force@ force_ = pk::get_force(building_.get_force_id());

			pk::list<pk::person@> target_list = getTargetList(force_);
			pk::list<pk::person@> temp_list;
			pk::random random(pk::rand());

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("进行寿命延长吗?"), taishu_) == 1)
				return false;


			if (pk::rand_bool(南斗北斗未发现概率))
			{

				pk::message_box(pk::encode("可惜没找到仙人, 但是对百姓的诉求有了进一步的体会."), taishu_);

				// 기교 감소 및 치안 증가
				ch::add_tp(force_, -TP_COST, building_.get_pos());
				pk::message_box(pk::encode(pk::format("城市的治安 \x1b[1x{}\x1b[0x因此加了.", safety_)));
				ch::add_public_order(building_, safety_, true);
			}
			else
			{
				debate_result == -1;
				pk::scene(pk::scene_t(scene_선인));

				if (debate_result == 0)
				{

					bool kunshu_confirm = false;

					while (!kunshu_confirm)
					{

						temp_list = pk::person_selector(pk::encode("选择武将"), pk::encode(pk::format("请选择要延长寿命的武将.({}寿命)", force_.tp / TP_COST)), target_list, 1, 1, temp_list);

						if (temp_list.count == 0)
							return false;

						if (pk::yes_no(pk::encode("要延长寿命吗?")))
							break;

						// 취소를 눌렀을 때 한 명일 경우 선택 복구하지 않음.
						if (temp_list.count == 1)
							temp_list.clear();
					}

					target_list = temp_list;

					// 기교 감소
					ch::add_tp(force_, -TP_COST, building_.get_pos());

					// 수명연장.
					for (int i = 0; i < target_list.count; i++)
					{
						pk::person@ target = target_list[i];

						target.death = target.death + random(5, 10);
					}

					pk::message_box(pk::encode("选择的武将寿命加了."));

				}
				else
				{
					// 기교 감소
					ch::add_tp(force_, -TP_COST, building_.get_pos());
				}

			}

			// 행동력 감소.
			auto district = pk::get_district(building_.get_district_id());
			pk::add_ap(district, -ACTION_COST);

			return true;

		}

		void scene_선인()
		{

			pk::move_screen(building_.get_pos());
			pk::fade(0);
			pk::sleep();
			pk::background(23);
			pk::fade(255);

			pk::person@ sinsen_1 = pk::get_person(무장_북두);
			pk::person@ sinsen_2 = pk::get_person(무장_남두);

			sinsen_1.wajutsu_daikatsu = true;
			sinsen_1.wajutsu_kiben = true;
			sinsen_1.wajutsu_mushi = true;
			sinsen_1.wajutsu_chinsei = true;
			sinsen_1.wajutsu_gyakujou = true;

			sinsen_2.wajutsu_daikatsu = true;
			sinsen_2.wajutsu_kiben = true;
			sinsen_2.wajutsu_mushi = true;
			sinsen_2.wajutsu_chinsei = true;
			sinsen_2.wajutsu_gyakujou = true;


			//sinsen_1.base_stat[武将能力_智力] = pk::max(1, taishu_.base_stat[武将能力_智力] - sinsen1_지력);
			sinsen_1.base_stat[武将能力_智力] = arr_stat[pk::rand(5)];
			sinsen_1.update();


			//sinsen_2.base_stat[武将能力_智力] = pk::max(1, taishu_.base_stat[武将能力_智力] - sinsen2_지력);
			sinsen_2.base_stat[武将能力_智力] = arr_stat[pk::rand(5)];
			sinsen_2.update();

			pk::message_box(pk::encode("神仙你在这儿呀."), taishu_);
			pk::message_box(pk::encode("你来这是为了逆天改命?"), sinsen_2);
			pk::message_box(pk::encode("你知道我为什么来这."), taishu_);
			pk::message_box(pk::encode("这是命中注定的谁也改变不了."), sinsen_1);
			pk::message_box(pk::encode("但我一定要试一试, 请神仙帮帮我."), taishu_);

			debate_result = pk::debate(taishu_, sinsen_1, true, false, false, true).first;

			if (debate_result == 0)
			{
				pk::message_box(pk::encode("希望神仙能理解."), taishu_);
				pk::message_box(pk::encode("是位意志坚定的小友.."), sinsen_2);
				debate_result = pk::debate(taishu_, sinsen_2, true, false, false, true).first;
			}

			if (debate_result == 0)
			{
				pk::message_box(pk::encode("我猜你们两位已经理解了."), taishu_);
				pk::message_box(pk::encode("看在你那么诚恳, 我答应你的请求."), sinsen_2);
			}
			else
			{
				pk::message_box(pk::encode("你该明白了.."), sinsen_2);
				pk::message_box(pk::encode("命运果然不可抗拒吗.."), taishu_);
			}

			pk::move_screen(building_.get_pos());
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);

		}


		pk::list<pk::person@> getTargetList(pk::force@ force)
		{
			pk::list<pk::person@> list;
			int force_id = force.get_id();
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
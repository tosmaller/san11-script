// ## 2022/01/25 # 江东新风 # 目标排除民兵 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/08/08 # 氕氘氚 # 加入【国事】菜单 ##
// ## 2020/08/07 #江东新风#限制君主退位需君主未行动,傳位后新旧君主设?已行动#
// ## 2020/08/07 #江东新风#退位设置金錢花銷，行动力消耗，港口无法退位#
// ## 2020/07/26 ##
/*
@ 만든이: 기마책사
@ UPDATE: '18.11.22  / 최초적용 : 세력 군주 변경

@ 수정자: HoneyBee
@ 수정 사항: SCRIPT 명칭을 '양위'로 변경, 군주가 소속된 도시의 태수를 新 군주로 재설정,
		  양위 시에 新 군주의 성향에 따른 무장들의 충성도 변화

*/

namespace 양위
{
	//---------------------------------------------------------------------------------------

	const int GOLD_COST = 800;//退位仪式所需金錢
	const int ACTION_COST = 10;
	const int KEY = pk::hash("泉卒");

	//---------------------------------------------------------------------------------------


	class Main
	{

		pk::building@ building;
		pk::building@ kunshu_building;
		pk::force@ force;
		pk::person@ gunshi;
		pk::person@ old_kunshu;
		pk::person@ new_kunshu;
		bool kunshu_building_check;
		string old_kunshu_name;
		string new_kunshu_name;
		
		array<pk::person@> person_sel_arr;


		Main()
		{
			pk::menu_item item;
			item.menu = global_menu::菜单_国事;
			//ch::u8debug(pk::format("menu: tui wei {}", global_menu::菜单_国事));
			item.init = pk::building_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(isVisible);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}


		//------------------------------------------------------------

		void init(pk::building@ building)
		{
			@building = @building;

			@force = pk::get_force(building.get_force_id());

			@gunshi = pk::get_person(force.gunshi);
			if (!pk::is_alive(gunshi))
				@gunshi = pk::get_person(무장_문관);

			@old_kunshu = pk::get_person(pk::get_kunshu_id(force));
			@kunshu_building = pk::get_building(old_kunshu.service);

			kunshu_building_check = (building.get_id() == old_kunshu.service);

			

		}

		string getText()
		{
			return pk::encode("退位");
		}

		string getDesc()
		{
			if (!kunshu_building_check) return pk::encode("只能在君主所在城市实行.");
			else if (kunshu_building.get_id() > 42) return pk::encode("穷乡僻壤无法传位.");
			else if (pk::is_absent(old_kunshu) or pk::is_unitize(old_kunshu) or old_kunshu.action_done == true)
				return pk::encode("君主不在或已行动.");
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else if (pk::get_gold(kunshu_building) < GOLD_COST) return pk::encode("无法负担退位花销.");
			return pk::encode("请选择要继位的武将");
		}

		bool isVisible()
		{
			if (!kunshu_building_check) return false;
			return true;
		}
		bool isEnabled()
		{
			if (kunshu_building.get_id() > 42) return false;
			else if (pk::is_absent(old_kunshu) or pk::is_unitize(old_kunshu) or old_kunshu.action_done == true)  return false;
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST) return false;
			else if (pk::get_gold(kunshu_building) < GOLD_COST) return false;
			return true;
		}

		bool handler()
		{

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("是否决定退位并选择继承人?"), old_kunshu) == 1)
				return false;

			bool kunshu_confirm = false;

			while (!kunshu_confirm)
			{
				// 무장 선택 창을 열어서 선택하기
				pk::list<pk::person@> person_list = get_possible_person_list(force);
				@new_kunshu = @pk::person_selector(pk::encode("武将选择"), pk::encode("选选择要成为新君主的武将"), person_list, 1, 1)[0];

				// 새로운 군주
				//@new_kunshu = person_sel_arr[0];

				// 선택 취소 시 종료
				if (!pk::is_alive(new_kunshu)) return false;

				// 군주 이름
				old_kunshu_name = pk::decode(pk::get_name(old_kunshu));
				new_kunshu_name = pk::decode(pk::get_name(new_kunshu));

				// 새 군주 확정
				kunshu_confirm = pk::yes_no(pk::encode(pk::format("选择\x1b[1x{}\x1b[0x做为新君主?", new_kunshu_name)));
			}

			// 군사를 새 군주로 선택했을 경우 대화씬에 문관을 사용
			if (new_kunshu.get_id() == gunshi.get_id())
			{
				@gunshi = pk::get_person(무장_문관);
				force.gunshi = old_kunshu.get_id();  // 기존 군주를 군사로 변경
				force.update();
			}

			// 군주 변경을 위한 이벤트 화면
			pk::scene(pk::scene_t(scene));


			// 세력 군주 및 선택 무장 신분 변경
			force.kunshu = new_kunshu.get_id();
			pk::get_district(pk::get_district_id(force, 1)).ap = pk::max(0, pk::get_district(pk::get_district_id(force, 1)).ap - ACTION_COST);
			force.update();
			old_kunshu.mibun = 身份_一般;
			old_kunshu.update();
			new_kunshu.mibun = 身份_君主;
			new_kunshu.update();

			// 태수 재설정
			int new_kunshu_ = new_kunshu.get_id();
			pk::set_taishu(kunshu_building, new_kunshu_);


			// 군주의 성향에 따른 무장 충성도 변화
			auto city_list = pk::list_to_array(pk::get_city_list());

			for (int i = 0; i < int(city_list.length); i++)
			{

				pk::city@ city_ = city_list[i];

				pk::building@ building_ = pk::city_to_building(city_);

				pk::person@ taishu = pk::get_person(pk::get_taishu_id(building_));

				if (!building_.is_player())
					continue;


				auto mibun_list = pk::list_to_array(pk::get_person_list(building_, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般)));

				if (pk::is_alive(taishu))
				{

					if (new_kunshu.ambition == 야망_매우높음 or new_kunshu.ambition == 야망_높음)
					{

						if (0 < mibun_list.length)
						{

							for (int j = 0; j < int(mibun_list.length); j++)
							{
								pk::person@ person_0 = mibun_list[j];

								if (pk::is_gikyoudai(person_0, new_kunshu_) or pk::is_fuufu(person_0, new_kunshu_) or pk::is_like(person_0, new_kunshu_))
									continue;

								// 新 군주를 혐오하는 무장의 충성도 변화
								if (person_0.disliked[0] == new_kunshu.get_id() or person_0.disliked[1] == new_kunshu.get_id()
									or person_0.disliked[2] == new_kunshu.get_id() or person_0.disliked[3] == new_kunshu.get_id()
									or person_0.disliked[4] == new_kunshu.get_id())
								{
									pk::add_loyalty(person_0, -50);
								}

								// 야망 매우높음/높음 충성도 변화
								if (person_0.ambition == 야망_매우높음 or person_0.ambition == 야망_높음)
								{
									pk::add_loyalty(person_0, 0);
								}

								// 야망 보통 충성도 변화
								else if (person_0.ambition == 야망_보통)
								{
									pk::add_loyalty(person_0, -5);
								}

								// 야망 매우낮음/낮음 충성도 변화
								else if (person_0.ambition == 야망_낮음 or person_0.ambition == 야망_매우낮음)
								{
									pk::add_loyalty(person_0, -15);
								}

							} // for

						} // if 

					} // if 

					else if (new_kunshu.ambition == 야망_보통)
					{

						if (0 < mibun_list.length)
						{

							for (int j = 0; j < int(mibun_list.length); j++)
							{
								pk::person@ person_0 = mibun_list[j];

								if (pk::is_gikyoudai(person_0, new_kunshu_) or pk::is_fuufu(person_0, new_kunshu_) or pk::is_like(person_0, new_kunshu_))
									continue;

								// 新 군주를 혐오하는 무장의 충성도 변화
								if (person_0.disliked[0] == new_kunshu.get_id() or person_0.disliked[1] == new_kunshu.get_id()
									or person_0.disliked[2] == new_kunshu.get_id() or person_0.disliked[3] == new_kunshu.get_id()
									or person_0.disliked[4] == new_kunshu.get_id())
								{
									pk::add_loyalty(person_0, -50);
								}

								// 야망 매우높음/높음 충성도 변화
								if (person_0.ambition == 야망_매우높음 or person_0.ambition == 야망_높음)
								{
									pk::add_loyalty(person_0, -5);
								}

								// 야망 보통 충성도 변화
								else if (person_0.ambition == 야망_보통)
								{
									pk::add_loyalty(person_0, 0);
								}

								// 야망 매우낮음/낮음 충성도 변화 or 新 군주를 혐오하는 무장 충성도 변화
								else if (person_0.ambition == 야망_낮음 or person_0.ambition == 야망_매우낮음)
								{
									pk::add_loyalty(person_0, -5);
								}

							} // for

						} // if 

					} // if 

					else if (new_kunshu.ambition == 야망_낮음 or new_kunshu.ambition == 야망_매우낮음)
					{

						if (0 < mibun_list.length)
						{

							for (int j = 0; j < int(mibun_list.length); j++)
							{
								pk::person@ person_0 = mibun_list[j];

								if (pk::is_gikyoudai(person_0, new_kunshu_) or pk::is_fuufu(person_0, new_kunshu_) or pk::is_like(person_0, new_kunshu_))
									continue;

								// 新 군주를 혐오하는 무장의 충성도 변화
								if (person_0.disliked[0] == new_kunshu.get_id() or person_0.disliked[1] == new_kunshu.get_id()
									or person_0.disliked[2] == new_kunshu.get_id() or person_0.disliked[3] == new_kunshu.get_id()
									or person_0.disliked[4] == new_kunshu.get_id())
								{
									pk::add_loyalty(person_0, -50);
								}

								// 야망 매우높음/높음 충성도 변화
								if (person_0.ambition == 야망_매우높음 or person_0.ambition == 야망_높음)
								{
									pk::add_loyalty(person_0, -15);
								}

								// 야망 보통 충성도 변화
								else if (person_0.ambition == 야망_보통)
								{
									pk::add_loyalty(person_0, -5);
								}

								// 야망 매우낮음/낮음 충성도 변화
								else if (person_0.ambition == 야망_낮음 or person_0.ambition == 야망_매우낮음)
								{
									pk::add_loyalty(person_0, 0);
								}

							} // for

						} // if 

					} // if 

				} // if 

			} // for 

			old_kunshu.loyalty = 100;
			old_kunshu.action_done = true;
			new_kunshu.action_done = true;
			pk::add_gold(kunshu_building, -GOLD_COST, true);



			return true;
		}

		void scene()
		{
			int pause = int(pk::option["MessagePause"]);
			pk::option["MessagePause"] = 100;

			pk::move_screen(kunshu_building.get_pos(), 2000);
			pk::fade(0);
			pk::sleep();
			pk::background(2);  // 배경: 관청
			pk::fade(255);

			pk::message_box(pk::encode(pk::format("请找\x1b[1x{}\x1b[0x过来.", new_kunshu_name)), old_kunshu);
			pk::message_box(pk::encode("遵命!"), gunshi);
			pk::cutin(18);  // 컷인: 걸음걸이
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x已经来了.", new_kunshu_name)), gunshi);

			pk::diplomacy(old_kunshu, gunshi, new_kunshu, pk::diplomacy_t(scene1));

			pk::background(22);  // 배경: 거병
			pk::message_box(pk::encode(pk::format("从现在开始, \x1b[1x{}\x1b[0x将带领诸位.", new_kunshu_name)), new_kunshu);
			pk::cutin(21);  // 컷인: 병사환호

			pk::option["MessagePause"] = 100;
		}

		void scene1()
		{

			// 간단한 대화
			pk::message_box(pk::encode("陛下此番找我有何吩咐?"), new_kunshu);
			pk::message_box(pk::encode(pk::format("多年征战, 我已身心俱疲.所以我决定将势力转交给你.我相信你一定有能力带领好大家的.", new_kunshu_name)), old_kunshu);
			pk::message_box(pk::encode("不...这如何使得?"), new_kunshu);
			pk::message_box(pk::encode("我意已决!"), old_kunshu);
			pk::message_box(pk::encode("好...必将不负所托!"), new_kunshu);

		}

		pk::list<pk::person@> get_possible_person_list(pk::force@ force)
		{
			pk::list<pk::person@> list;
			int force_id = force.get_id();
			for (int i = 0; i < 非贼武将_末; ++i)
			{
				pk::person@ person0 = pk::get_person(i);
				if (pk::is_alive(person0))
				{					
					if (person0.get_force_id() == force_id and (person0.mibun == 身份_都督 or person0.mibun == 身份_太守 or person0.mibun == 身份_一般))
					{
						if (person0.name_read != pk::encode("农民兵")) list.add(person0);
					}
				}
			}
			return list;
		}
	}

	Main main;
}
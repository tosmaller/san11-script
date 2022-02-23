// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/08 # 氕氘氚 # 加入【国事】菜单 ##
/*
@ 제작자: HoneyBee
@ 내용: 후한황제를 폐위시키고, 세력의 군주가 황제가 됨.
//---------------------------------------------
@ 수정자: 기마책사
@ UPDATE: '18.12.3   / 변경내용: 황제폐위 후 메뉴 초기화 오류 방지 추가
*/

namespace 폐위
{

	// ================ CUSTOMIZE ================

	const int TP_COST = 5000; 	   // 기교P 필요량
	const int ACTION_COST = 100;   // 행동력 필요량

	const int KEY = pk::hash("榷卒");

	// ===========================================

	class Main
	{

		pk::building@ building;
		pk::building@ kunshu_building;

		pk::city@ kunshu_city;
		pk::city@ emperor_city;

		pk::force@ force;

		pk::person@ kunshu;
		pk::person@ emperor;
		pk::person@ support;

		Main()
		{

			pk::menu_item item;
			item.menu = global_menu::菜单_国事;
			//ch::u8debug(pk::format("menu: fei chu {}", global_menu::菜单_国事));    
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

			@building = @building;
			@force = pk::get_force(building.get_force_id());
			@kunshu = pk::get_person(pk::get_kunshu_id(building));
			@kunshu_building = pk::get_building(kunshu.service);
			@kunshu_city = pk::building_to_city(kunshu_building);
			@emperor = pk::get_person(pk::get_scenario().emperor);

			if (emperor is null) return;
			else
			{
				// 황제의 SUPPORT(황후,환관)을 설정 
				if (emperor.get_id() == 武将_献帝)
					@support = pk::get_person(武将_皇后);
				else if (emperor.get_id() == 武将_少帝)
					@support = pk::get_person(武将_宦官);
				else if (emperor.get_id() == 武将_灵帝)
				{
					switch (pk::rand(2))
					{
					case 0: @support = pk::get_person(武将_张让); break;
					case 1: @support = pk::get_person(武将_蹇硕); break;
					}
				}
				@emperor_city = pk::building_to_city(pk::hex_object_to_building(pk::get_hex_object(emperor.location)));
			}

		}

		string getText()
		{
			return pk::encode("废黜");  //feichu
		}

		string getDesc()
		{
			if (emperor is null)
				return pk::encode("东汉灭亡了");
			if (!pk::is_protecting_the_emperor(force) or force.title == 爵位_皇帝)
				return pk::encode("没有拥立东汉皇帝.");
			else if (emperor_city.get_id() != kunshu.service)
				return pk::encode("君主需和东汉皇帝在同一城市.");
			else if (kunshu_city.get_id() != kunshu.service)
				return pk::encode("只能在君主所在城市实行.");
			else if (pk::is_absent(kunshu) or pk::is_unitize(kunshu) or kunshu.action_done)
				return pk::encode("君主已行动或不在城市中.");
			else if (force.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("废黜东汉皇帝. (技巧P{})", TP_COST));
		}

		bool isVisible()
		{
			if (emperor is null) return false;
			else if (!pk::is_protecting_the_emperor(force) or force.title == 爵位_皇帝) return false;
			return true;
		}

		bool isEnabled()
		{
			if (emperor is null) return false;
			else if (!pk::is_protecting_the_emperor(force) or force.title == 爵位_皇帝) return false;
			else if (emperor_city.get_id() != kunshu.service) return false;
			else if (kunshu_city.get_id() != kunshu.service) return false;
			else if (pk::is_absent(kunshu) or pk::is_unitize(kunshu) or kunshu.action_done) return false;
			else if (force.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST) return false;
			return true;
		}

		bool handler()
		{

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("废黜东汉皇帝?\n(技巧P{})", TP_COST)), kunshu) == 1)
				return false;

			string kunshu_name = pk::decode(pk::get_name(kunshu));

			// 후한 황제와의 대화
			pk::scene(pk::scene_t(scene_폐위));

			// 후한 황제 제거
			pk::kill(emperor, kunshu, kunshu_building, null, 0);

			// 군주의 작위를 황제로 설정
			force.title = 爵位_皇帝;

			// 국호 설정
			kokugou_0();

			force.update();

			int kunshu_0 = kunshu.get_id();


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

					if (0 < mibun_list.length)
					{

						for (int j = 0; j < int(mibun_list.length); j++)
						{
							pk::person@ person_0 = mibun_list[j];

							if (pk::is_gikyoudai(person_0, kunshu_0) or pk::is_fuufu(person_0, kunshu_0) or pk::is_like(person_0, kunshu_0))
								continue;

							// 新 황제를 혐오하는 무장의 충성도 변화
							if (person_0.disliked[0] == kunshu.get_id() or person_0.disliked[1] == kunshu.get_id()
								or person_0.disliked[2] == kunshu.get_id() or person_0.disliked[3] == kunshu.get_id()
								or person_0.disliked[4] == kunshu.get_id())
							{
								pk::add_loyalty(person_0, -50);
							}

							// 한실_중시 충성도 변화
							if (person_0.kanshitsu == 한실_중시)
							{
								pk::add_loyalty(person_0, -15);
							}

							// 한실_보통 충성도 변화
							else if (person_0.kanshitsu == 한실_보통)
							{
								pk::add_loyalty(person_0, 0);
							}

							// 한실_무시 충성도 변화
							else if (person_0.kanshitsu == 한실_무시)
							{
								pk::add_loyalty(person_0, 15);
							}

						} // for

					} // if 

				} // if 

			} // for

			// 행동력 감소
			auto district = pk::get_district(pk::get_district_id(force, 1));
			pk::add_ap(district, -ACTION_COST);

			// 기교 감소
			ch::add_tp(force, -TP_COST, force.get_pos());

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x称帝了.", kunshu_name)));
			pk::create_effect(0x51, kunshu_building.pos);   //이펙트효과

			kunshu.action_done = true;

			return true;

		}


		void scene_폐위()
		{
			int pause = int(pk::option["MessagePause"]);
			pk::option["MessagePause"] = 100;
			pk::message_box(pk::encode("我得借此机会废黜皇帝, 夺取江山."), kunshu);
			pk::cutin(18);  // 컷인: 걸음걸이
			pk::move_screen(emperor_city.get_pos(), 2000);
			pk::diplomacy(emperor, support, kunshu, pk::diplomacy_t(scene_폐위_1));
			pk::move_screen(kunshu_city.get_pos(), 2000);

			pk::message_box(pk::encode("从今天起朕就是这个国家的皇帝!"), kunshu);
			pk::cutin(21);  // 컷인: 병사환호

			pk::option["MessagePause"] = 100;

		}

		void scene_폐위_1()
		{

			string kunshu_name = pk::decode(pk::get_name(kunshu));

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x, 今天来有何贵干?", kunshu_name)), emperor);
			pk::message_box(pk::encode("听好! 子无, 国乃至此也!"), kunshu);
			pk::message_box(pk::encode("什么? 你什么意思?"), emperor);
			pk::message_box(pk::encode("你已经不配做这个皇帝了!"), kunshu);
			pk::message_box(pk::encode("以后我来治理这个国家!"), kunshu);
			pk::message_box(pk::encode("...原来你也是个叛贼..."), emperor);
			pk::message_box(pk::encode(pk::format("陛下, 如果你忤逆了\x1b[1x{}\x1b[0x, 将会酿成大祸.", kunshu_name)), support);
			pk::message_box(pk::encode("............"), emperor);

		}

		void kokugou_0()
		{

			array<string> kokugou_list =
			{
				pk::encode("魏"),
				pk::encode("吴"),
				pk::encode("蜀"),
				pk::encode("晋"),
				pk::encode("成"),
				pk::encode("下一页"),
			};

			int n = pk::choose(pk::encode("请选择国号."), kokugou_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x魏\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_위;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x吴\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_오;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x蜀\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_촉;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x晋\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_진;
			}
			else if (n == 4)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x成\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_한;
			}
			else if (n == 5)
			{
				kokugou_1();
			}


		} // kokugou_0()

		void kokugou_1()
		{
			array<string> kokugou_list =
			{
				pk::encode("汉"),
				pk::encode("夏"),
				pk::encode("商"),
				pk::encode("周"),
				pk::encode("上一页"),
				pk::encode("下一页"),
			};

			int n = pk::choose(pk::encode("请选择国号."), kokugou_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x汉\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_성;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x夏\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_초;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x商\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_연;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x周\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_제;
			}
			else if (n == 4)
			{
				kokugou_0();
			}
			else if (n == 5)
			{
				kokugou_2();
			}


		} // kokugou_1()

		void kokugou_2()
		{
			array<string> kokugou_list =
			{
				pk::encode("秦"),
				pk::encode("新"),
				pk::encode("燕"),
				pk::encode("赵"),
				pk::encode("齐"),
				pk::encode("上一页"),
			};

			int n = pk::choose(pk::encode("请选择国号."), kokugou_list);

			if (n == 0)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x秦\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_조;
			}
			else if (n == 1)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x新\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_하;
			}
			else if (n == 2)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x燕\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_상;
			}
			else if (n == 3)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x赵\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_주;
			}
			else if (n == 4)
			{
				pk::message_box(pk::encode("朕选择\x1b[1x齐\x1b[0x做为国号!"), kunshu);
				force.kokugou = 국호_월;
			}
			else if (n == 5)
			{
				kokugou_1();
			}

		} // kokugou_2()

	}

	Main main;
}
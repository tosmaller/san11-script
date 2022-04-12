// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/09/21 # 江东新风 # 添加法治效果 ##
// ## 2020/08/09 # 江东新风 # 拜見完君主行动状态设?已行动，限制多次拜見 ##
// ## 2020/08/08 # 氕氘氚 # 加入【国事】菜单 ##
/*
@제작자: HoneyBee
@설명: 후한황제 알현 기능
@기타: 후한황제를 알현하여 추가적인 ADVANTAGE를 얻습니다.
----------------------------------------------
@수정자: 기마책사
@UPDATE: '18.12.3   / 변경내용: 후한황제 자동선택, 알현 화면전환 적용, 황제폐위 후 메뉴 초기화 오류 방지 추가
*/

namespace 알현
{


	// ======================================================== CUSTOMIZE ==========================================================


		/*

		  시나리오에 따라서 후한황제를 [武将_灵帝, 武将_少帝, 武将_献帝] or 기타 인물들 中 1人으로 선택하여 결정 (아래는 헌제로 기본 설정됨)

		  1. 황건봉기(영제)
		  2. 반동탁연합, 군웅할거, 관도대전, 삼고초려, 유비입촉, 여포토벌전, 원가분열, 한중쟁탈전, 삼대군벌(헌제)
		  3. 영웅집결, 하진포위망, 여인천하(후한황제 无)
		  4. 기타 유저 시나리오는 후한황제의 설정에 따라서 달라지므로, VAN EDITOR로 후한황제가 누구로 설정되어 있는지 확인하시기 바랍니다.


		  < 이벤트 내용 >

		  1. 동탁, 이각, 곽사가 [군주]일 경우
		  = 도시 3개 미만으로 보유 시 후한황제를 [알현]하면 금, 병량, 병력, 병기(군마) 상승하는 이벤트 발생
		  = 도시 3개 이상 보유 시 후한황제를 [알현]하면 관직 상승하는 이벤트 발생
		  = 도시 5개 이상 보유 시 후한황제를 [알현]하면 병력, 병기(군마), 기력 상승하는 이벤트 발생

		  2. 기타 인물들이 [군주]일 경우
		  = 후한황제를 [알현]하면 RANDOM으로 후한황제에게 금 이벤트 발생 (금을 줄지 말지 결정)
		  = 후한황제를 [알현]하면 RANDOM으로 후한황제에게 병량 이벤트 발생 (병량을 줄지 말지 결정)
		  = 후한황제를 [알현]하면 RANDOM으로 후한황제에게 병력 이벤트 발생 (병력을 줄지 말지 결정)
		  = 후한황제를 [알현]하면 RANDOM으로 작위 상승 이벤트 발생

		*/


	const int TP_COST = 500; // 기교P 수치 결정
	const int ACTION_COST = 100;  // 행동력 필요량
	const int gold_ = 10000; // 후한황제가 요구하는 금액 설정
	const int food_ = 20000; // 후한황제가 요구하는 병량 설정 
	const int troops_ = 10000; // 후한황제가 요구하는 병력 설정 

	const int energy_1 = 50; // 후한황제에게서 작위 대신에 얻는 이익 (기력 상승)
	const int safety_1 = 50; // 후한황제에게서 작위 대신에 얻는 이익 (치안 상승)
<<<<<<< HEAD
	const int weapon_10 = 10000; // 후한황제에게서 작위 대신에 얻는 이익 (병기_창)
	const int weapon_11 = 10000; // 후한황제에게서 작위 대신에 얻는 이익 (병기_극)
	const int weapon_12 = 10000; // 후한황제에게서 작위 대신에 얻는 이익 (병기_노)
=======
	const int weapon_10 = 10000; // 후한황제에게서 작위 대신에 얻는 이익 (兵器_枪)
	const int weapon_11 = 10000; // 후한황제에게서 작위 대신에 얻는 이익 (兵器_戟)
	const int weapon_12 = 10000; // 후한황제에게서 작위 대신에 얻는 이익 (兵器_弩)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

	const int gold_2 = 10000; // 동탁, 이각, 곽사가 <군주>인 경우, 알현으로 얻는 이익 설정 (금)
	const int food_2 = 20000; // 동탁, 이각, 곽사가 <군주>인 경우, 알현으로 얻는 이익 설정 (병량)
	const int troops_2 = 10000; // 동탁, 이각, 곽사가 <군주>인 경우, 알현으로 얻는 이익 설정 (병력)
<<<<<<< HEAD
	const int weapon_2 = 10000; // 동탁, 이각, 곽사가 <군주>인 경우, 알현으로 얻는 이익 설정 (병기_군마)
	const int energy_2 = 50; // 동탁, 이각, 곽사가 <군주>인 경우, 알현으로 얻는 이익 설정 (병기_군마)
=======
	const int weapon_2 = 10000; // 동탁, 이각, 곽사가 <군주>인 경우, 알현으로 얻는 이익 설정 (兵器_战马)
	const int energy_2 = 50; // 동탁, 이각, 곽사가 <군주>인 경우, 알현으로 얻는 이익 설정 (兵器_战马)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	const int city_1 = 3; // 동탁, 이각, 곽사가 <군주>인 경우, 작위가 '공' 미만 이벤트 조건 (도시 보유)
	const int city_2 = 5; // 동탁, 이각, 곽사가 <군주>인 경우, 작위가 '공' 이상 이벤트 조건 (도시 보유)

	const int gold_3 = 10000; // 알현으로 작위를 얻음으로서 잃는 (금)의 수량 설정 


// =============================================================================================================================


	const int KEY = pk::hash("甩渊");


	class Main
	{

		pk::building@ building_;
		pk::building@ kunshu_building;
		pk::force@ force_;
		pk::person@ kunshu_;
		pk::person@ reporter_;
		pk::city@ city_;

		pk::person@ emperor;
		pk::person@ queen;
		pk::city@ emperor_city;

		Main()
		{
			pk::menu_item item;
			item.menu = global_menu::菜单_国事;
			//ch::u8debug(pk::format("menu: bai jian {}", global_menu::菜单_国事));
			//item.pos = 7;
			//item.shortcut = "7";
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
			@reporter_ = pk::get_person(무장_문관);
			@kunshu_building = pk::get_building(kunshu_.service);
			@city_ = pk::building_to_city(pk::get_building(kunshu_.service));

			@queen = pk::get_person(武将_皇后);

			@emperor = pk::get_person(pk::get_scenario().emperor);

			if (emperor is null) return;
			else
			{
				@emperor_city = pk::building_to_city(pk::hex_object_to_building(pk::get_hex_object(emperor.location)));
			}
		}

		bool isVisible()
		{
			if (kunshu_building.get_id() >= 城市_末) return false;
			else if (emperor is null) return false;
			else if (!pk::is_protecting_the_emperor(force_) or force_.title == 爵位_皇帝) return false;
			return true;
		}

		bool isEnabled()
		{
			if (emperor is null) return false;
			else if (emperor_city.get_id() != kunshu_.service) return false;
			else if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_) or kunshu_.action_done) return false;
			return pk::is_protecting_the_emperor(force_) and force_.tp > TP_COST and pk::is_alive(kunshu_) and pk::get_district(pk::get_district_id(force_, 1)).ap >= ACTION_COST;
		}

		string getText()
		{
			return pk::encode("拜见");  //baijian
		}

		string getDesc()
		{
			if (kunshu_building.get_id() >= 城市_末)
				return pk::encode("君主不在城市中.");
			else if (emperor_city.get_id() != kunshu_.service)
				return pk::encode("君主需和东汉皇帝在同一城市.");
			else if (emperor is null)
				return pk::encode("东汉已经灭亡");
			else if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_) or kunshu_.action_done)
				return pk::encode("君主已行动或不在城市中.");
			else if (!pk::is_alive(kunshu_))
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (!pk::is_protecting_the_emperor(force_))
				return pk::encode(pk::format("不是拥护汉室的势力.(技巧P {})", TP_COST));
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("拜见皇帝陛下.(技巧P {})", TP_COST));
		}

		bool handler()
		{


			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("是否要拜见皇帝陛下? \n(使用技巧P{},行动力{})", TP_COST, ACTION_COST)), reporter_) == 1)
				return false;

			//-----------------------------------------------------------------
			// 후한 황제와의 대화
			pk::scene(pk::scene_t(scene));

			//-----------------------------------------------------------------

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);
			kunshu_.action_done = true;

			return true;
		}

		void scene()
		{
			int pause = int(pk::option["MessagePause"]);
			pk::option["MessagePause"] = 100;
			pk::message_box(pk::encode("确实是要拜见一下陛下了"), kunshu_);
			pk::cutin(18);  // 컷인: 걸음걸이
			pk::move_screen(emperor_city.get_pos(), 2000);
			pk::diplomacy(emperor, queen, kunshu_, pk::diplomacy_t(scene1));
			pk::move_screen(city_.get_pos(), 2000);
			pk::option["MessagePause"] = 100;
		}

		void scene1()
		{
			string kunshu_name = pk::decode(pk::get_name(kunshu_));
			//auto district = pk::get_district(pk::get_district_id(force_, 1));


			// 군주가 동탁 or 이각 or 곽사일 경우 (1)
			if (pk::get_city_count(force_) < city_1 and pk::get_kunshu_id(building_) == 무장_동탁 or pk::get_kunshu_id(building_) == 무장_이각 or pk::get_kunshu_id(building_) == 무장_곽사)
			{
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,你来有什么事?", kunshu_name)), emperor);
				pk::message_box(pk::encode("陛下,近日逆贼猖獗需有陛下声讨之檄文."), kunshu_);
				pk::message_box(pk::encode("好...知道了."), emperor);
				pk::message_box(pk::encode("当然,此檄文天下周知.\n(皇帝在我手里,真舒服.)"), kunshu_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x以皇帝的檄文掠夺百姓, 使百姓苦不堪言.", kunshu_name)));

				int safety_ = 100;
				safety_ = safety_ - pk::rand(50);

				pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.", TP_COST)));
				ch::add_tp(force_, -TP_COST, force_.get_pos());
				pk::message_box(pk::encode(pk::format("金钱增加了\x1b[1x{}\x1b[0x.", gold_2)));
				pk::add_gold(city_, gold_2, true);
				pk::message_box(pk::encode(pk::format("粮食增加了\x1b[1x{}\x1b[0x.", food_2)));
				pk::add_food(city_, food_2, true);
				pk::message_box(pk::encode(pk::format("士兵增加了\x1b[1x{}\x1b[0x.", troops_2)));
				ch::add_troops(city_, troops_2, true);
				pk::message_box(pk::encode(pk::format("武器军备增加了\x1b[1x{}\x1b[0x.", weapon_2)));
<<<<<<< HEAD
				pk::add_weapon_amount(city_, 병기_군마, weapon_2, true);
=======
				pk::add_weapon_amount(city_, 兵器_战马, weapon_2, true);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				pk::message_box(pk::encode(pk::format("治安减少了\x1b[1x{}\x1b[0x.", safety_)));
				ch::add_public_order(city_, -safety_, true);

				// 행동력 감소.
				//pk::add_ap(district, -ACTION_COST);


			}

			// 군주가 동탁 or 이각 or 곽사일 경우 (2)
			else if (pk::get_city_count(force_) >= city_1 and force_.title >= 작위_대사마 and pk::get_kunshu_id(building_) == 무장_동탁 or pk::get_kunshu_id(building_) == 무장_이각 or pk::get_kunshu_id(building_) == 무장_곽사)
			{
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,你来有什么事?", kunshu_name)), emperor);
				pk::message_box(pk::encode("陛下,剿灭逆贼,特来报告."), kunshu_);
				pk::message_box(pk::encode("好...辛苦了,朕要以讨伐逆贼之功来提升你的爵位"), emperor);
				pk::message_box(pk::encode("很好.感谢陛下恩典."), kunshu_);
				pk::message_box(pk::encode("............."), emperor);
				pk::message_box(pk::encode("(皇帝捏在手心真舒服.)"), kunshu_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x借着铲除敌人的幌子进一步提高了爵位.", kunshu_name)));

				if (force_.title == 작위_없음)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为州刺史.", kunshu_name)));
					force_.title = 작위_주자사;
					force_.update();
				}
				else if (force_.title == 작위_주자사)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为州牧.", kunshu_name)));
					force_.title = 작위_주목;
					force_.update();
				}
				else if (force_.title == 작위_주목)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为羽林中郎将.", kunshu_name)));
					force_.title = 작위_우림중랑장;
					force_.update();
				}
				else if (force_.title == 작위_우림중랑장)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为五官中郎将.", kunshu_name)));
					force_.title = 작위_오관중랑장;
					force_.update();
				}
				else if (force_.title == 작위_오관중랑장)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为大将军.", kunshu_name)));
					force_.title = 작위_대장군;
					force_.update();
				}
				else if (force_.title == 작위_대장군)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为大司马.", kunshu_name)));
					force_.title = 작위_대사마;
					force_.update();
				}
				else if (force_.title == 작위_대사마)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为公.", kunshu_name)));
					force_.title = 작위_공;
					force_.update();
				}

				pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.", TP_COST)));
				ch::add_tp(force_, -TP_COST, force_.get_pos());

				// 행동력 감소.
				//pk::add_ap(district, -ACTION_COST);

			}

			// 군주가 동탁 or 이각 or 곽사일 경우 (3)
			else if (pk::get_city_count(force_) >= city_2 and force_.title <= 작위_공 and pk::get_kunshu_id(building_) == 무장_동탁 or pk::get_kunshu_id(building_) == 무장_이각 or pk::get_kunshu_id(building_) == 무장_곽사)
			{
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,你来有什么事?", kunshu_name)), emperor);
				pk::message_box(pk::encode("陛下近来龙体是否安泰."), kunshu_);
				pk::message_box(pk::encode("朕恭甚安..无需挂念.."), emperor);
				pk::message_box(pk::encode("好,那在下就告退了."), kunshu_);
				pk::message_box(pk::encode("............."), emperor);
				pk::message_box(pk::encode("(我指掌天下的日子快到了.)"), kunshu_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x军威势日盛.", kunshu_name)));

				pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.", TP_COST)));
				ch::add_tp(force_, -TP_COST, force_.get_pos());
				pk::message_box(pk::encode(pk::format("士兵增加了\x1b[1x{}\x1b[0x.", troops_2)));
				ch::add_troops(city_, troops_2, true);
				pk::message_box(pk::encode(pk::format("气力增加了\x1b[1x{}\x1b[0x.", energy_2)));
				pk::add_energy(city_, energy_2, true);
				pk::message_box(pk::encode(pk::format("武器军备增加了\x1b[1x{}\x1b[0x.", weapon_2)));
<<<<<<< HEAD
				pk::add_weapon_amount(city_, 병기_군마, weapon_2, true);
=======
				pk::add_weapon_amount(city_, 兵器_战马, weapon_2, true);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

				// 행동력 감소.
				//pk::add_ap(district, -ACTION_COST);

			}


			int n = pk::rand(4);

			if (pk::get_kunshu_id(building_) == 무장_동탁 or pk::get_kunshu_id(building_) == 무장_이각 or pk::get_kunshu_id(building_) == 무장_곽사)
			{
				//return false;
				return;
			}

			else if (n == 0)
			{
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,可以为振兴汉室捐赠些钱财吗?", kunshu_name)), emperor);

				array<string> answers =
				{
					pk::encode("捐钱"),
					pk::encode("不捐钱")
				};

				int a = pk::choose(pk::encode("是否给汉室皇帝捐赠些钱财?"), answers);
				int energy_ = 100;

				switch (a)
				{
				case 0:
					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode("陛下,以后有什么需要之处尽可提出来."), kunshu_); break;
					case 1: pk::message_box(pk::encode("陛下,请接受我的一片心意."), kunshu_); break;
					}
					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x果然是忠臣啊.", kunshu_name)), emperor); break;
					case 1: pk::message_box(pk::encode("此恩不忘."), emperor); break;
					}
					pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.", TP_COST)));
					ch::add_tp(force_, -TP_COST, force_.get_pos());
					pk::message_box(pk::encode(pk::format("士气上升到了\x1b[1x{}\x1b[0x.", energy_)));
					pk::set_energy(building_, energy_);
					pk::message_box(pk::encode(pk::format("金钱减少了\x1b[1x{}\x1b[0x.", gold_)));
					pk::add_gold(city_, -gold_, true);

					// 행동력 감소.
					//pk::add_ap(district, -ACTION_COST);
					break;

				case 1:
					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode("陛下,现在这个时候吗?请想想国家各个方面都需要钱财."), kunshu_); break;
					case 1: pk::message_box(pk::encode("陛下,此时请赐予善政,勤俭持政."), kunshu_); break;
					}
					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode(pk::format("..\x1b[1x{}\x1b[0x..这时才需要体现忠心啊..", kunshu_name)), emperor); break;
					case 1: pk::message_box(pk::encode("......"), emperor); break;
					}
					pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.", TP_COST)));
					ch::add_tp(force_, -TP_COST, force_.get_pos());
					energy_ = energy_ - pk::rand(50);
					pk::message_box(pk::encode(pk::format("士气减少了\x1b[1x{}\x1b[0x.", energy_)));
					pk::add_energy(building_, -energy_);

					// 행동력 감소.
					//pk::add_ap(district, -ACTION_COST);
					break;
				}

			}

			if (n == 1)
			{
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,可否替朕给百姓发些口粮?", kunshu_name)), emperor);

				array<string> answers =
				{
					pk::encode("给百姓散发粮食"),
					pk::encode("不给百姓散发粮食")
				};

				int a = pk::choose(pk::encode("是否开仓振民?"), answers);
				int safety_ = 100;

				switch (a)
				{
				case 0:
					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode("为宣扬陛下的恩泽,我即刻就给百姓施粮."), kunshu_); break;
					case 1: pk::message_box(pk::encode("陛下真是明君啊."), kunshu_); break;
					}
					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode(pk::format("感谢!\x1b[1x{}\x1b[0x果然是我大汉的忠臣.", kunshu_name)), emperor); break;
					case 1: pk::message_box(pk::encode("此恩不忘."), emperor); break;
					}
					pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.", TP_COST)));
					ch::add_tp(force_, -TP_COST, force_.get_pos());
					pk::message_box(pk::encode(pk::format("治安变为了\x1b[1x{}\x1b[0x了.", safety_)));
					pk::set_public_order(city_, safety_);
					pk::message_box(pk::encode(pk::format("粮食减少了\x1b[1x{}\x1b[0x.", food_)));
					pk::add_food(city_, -food_, true);

					// 행동력 감소.
					//pk::add_ap(district, -ACTION_COST);
					break;

				case 1:
					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode("陛下,现在是分发粮食的时候吗? 请认清现实."), kunshu_); break;
					case 1: pk::message_box(pk::encode("陛下,百姓不一定非要我的救济才能活的更好."), kunshu_); break;
					}
					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode(pk::format("...\x1b[1x{}\x1b[0x..这时才需要体现忠心啊..", kunshu_name)), emperor); break;
					case 1: pk::message_box(pk::encode("......"), emperor); break;
					}
					pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.", TP_COST)));
					ch::add_tp(force_, -TP_COST, force_.get_pos());

					safety_ = safety_ - pk::rand(50);
					pk::message_box(pk::encode(pk::format("治安减少了\x1b[1x{}\x1b[0x.", safety_)));
					ch::add_public_order(city_, -safety_);

					// 행동력 감소.
					//pk::add_ap(district, -ACTION_COST);
					break;
				}
			}

			if (n == 2)
			{
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,是否可以抽调兵力来守卫行宫?", kunshu_name)), emperor);

				array<string> answers =
				{
					pk::encode("抽调兵力"),
					pk::encode("不抽调兵力")
				};

				int a = pk::choose(pk::encode("是否抽调兵力?"), answers);
				int hp_ = 1000;

				switch (a)
				{
				case 0:
					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode("快挑选一批勇猛的士兵保护陛下的寝宫."), kunshu_); break;
					case 1: pk::message_box(pk::encode("挑选了一批侍卫来侍奉陛下."), kunshu_); break;
					}
					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode(pk::format("感谢!\x1b[1x{}\x1b[0x果然是忠臣啊.", kunshu_name)), emperor); break;
					case 1: pk::message_box(pk::encode("此恩不忘."), emperor); break;
					}
					pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.", TP_COST)));
					ch::add_tp(force_, -TP_COST, force_.get_pos());
					pk::message_box(pk::encode(pk::format("耐久变为\x1b[1x{}\x1b[0x.", hp_)));
					pk::add_hp(building_, hp_, true);
					pk::message_box(pk::encode(pk::format("士兵减少了\x1b[1x{}\x1b[0x.", troops_)));
					ch::add_troops(city_, -troops_, true);

					// 행동력 감소.
					//pk::add_ap(district, -ACTION_COST);
					break;

				case 1:
					switch (pk::rand(3))
					{
					case 0: pk::message_box(pk::encode("陛下,现在国家正是用人之际,请恕在下莫难从命."), kunshu_); break;
					case 1: pk::message_box(pk::encode("陛下,此时请赐予善政,励精图治."), kunshu_); break;
					case 2: pk::message_box(pk::encode("兵权在我手上!陛下不要再说了."), kunshu_); break;
					}
					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x..这时才能体现忠心啊.", kunshu_name)), emperor); break;
					case 1: pk::message_box(pk::encode("......"), emperor); break;
					}
					pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.", TP_COST)));
					ch::add_tp(force_, -TP_COST, force_.get_pos());
					hp_ = hp_ - pk::rand(500);
					pk::message_box(pk::encode(pk::format("耐久减少了\x1b[1x{}\x1b[0x..", hp_)));
					pk::add_hp(building_, -hp_, true);

					// 행동력 감소.
					//pk::add_ap(district, -ACTION_COST);
					break;

				}
			}

			if (n == 3)
			{
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,此来有何事禀告?", kunshu_name)), emperor);
				pk::message_box(pk::encode("陛下,在下已剿灭逆贼,特来禀告."), kunshu_);
				pk::message_box(pk::encode("爱卿辛苦了.你剿贼得力,朕当重赏."), emperor);
				pk::message_box(pk::encode("此行冒昧了.这就将剿贼所得之金献于陛下!"), kunshu_);
				pk::message_box(pk::encode("有劳了."), emperor);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x因剿灭逆贼而声名鹊起.", kunshu_name)));

				pk::message_box(pk::encode(pk::format("金钱减少了\x1b[1x{}\x1b[0x.", gold_3)));
				pk::add_gold(city_, -gold_3, true);


				if (force_.title == 작위_없음)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为州刺史.", kunshu_name)));
					force_.title = 작위_주자사;
					force_.update();
					//return true;
					return;
				}
				else if (force_.title == 작위_주자사)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为州牧.", kunshu_name)));
					force_.title = 작위_주목;
					force_.update();
					//return true;
					return;
				}
				else if (force_.title == 작위_주목)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为羽林中郎将.", kunshu_name)));
					force_.title = 작위_우림중랑장;
					force_.update();
					//return true;
					return;
				}
				else if (force_.title == 작위_우림중랑장)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为五官中郎将.", kunshu_name)));
					force_.title = 작위_오관중랑장;
					force_.update();
					//return true;
					return;
				}
				else if (force_.title == 작위_오관중랑장)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为大将军.", kunshu_name)));
					force_.title = 작위_대장군;
					force_.update();
					//return true;
					return;
				}
				else if (force_.title == 작위_대장군)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为大司马.", kunshu_name)));
					force_.title = 작위_대사마;
					force_.update();
					//return true;
					return;
				}
				else if (force_.title == 작위_대사마)
				{
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的爵位升为公.", kunshu_name)));
					force_.title = 작위_공;
					force_.update();
					//return true;
					return;
				}
				else if (force_.title <= 작위_공)
				{
					pk::message_box(pk::encode(pk::format("治安增加了\x1b[1x{}\x1b[0x.", safety_1)));
					ch::add_public_order(city_, safety_1, true);
					pk::message_box(pk::encode(pk::format("士气增加了\x1b[1x{}\x1b[0x.", energy_1)));
					pk::add_energy(city_, energy_1, true);
					pk::message_box(pk::encode(pk::format("枪的库存增加了\x1b[1x{}\x1b[0x.", weapon_10)));
<<<<<<< HEAD
					pk::add_weapon_amount(city_, 병기_창, weapon_10, true);
					pk::message_box(pk::encode(pk::format("戟的库存增加了\x1b[1x{}\x1b[0x.", weapon_11)));
					pk::add_weapon_amount(city_, 병기_극, weapon_11, true);
					pk::message_box(pk::encode(pk::format("弩的库存增加了\x1b[1x{}\x1b[0x.", weapon_12)));
					pk::add_weapon_amount(city_, 병기_노, weapon_12, true);
=======
					pk::add_weapon_amount(city_, 兵器_枪, weapon_10, true);
					pk::message_box(pk::encode(pk::format("戟的库存增加了\x1b[1x{}\x1b[0x.", weapon_11)));
					pk::add_weapon_amount(city_, 兵器_戟, weapon_11, true);
					pk::message_box(pk::encode(pk::format("弩的库存增加了\x1b[1x{}\x1b[0x.", weapon_12)));
					pk::add_weapon_amount(city_, 兵器_弩, weapon_12, true);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				}

				pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.", TP_COST)));
				ch::add_tp(force_, -TP_COST, force_.get_pos());

				// 행동력 감소.
				//pk::add_ap(district, -ACTION_COST);

			} // if

			//return true;
			return;

		}


	}// class Main

	Main main;

} //namespace
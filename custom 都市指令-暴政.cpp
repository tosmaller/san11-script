// ## 2022/03/27 # 江东新风 # 暴政惩罚增加 ##
// ## 2020/12/27 # 江东新风 # 只在君主所在城市可见 ##
// ## 2020/11/06 # 江东新风 # 焚城后治安设为50 ##
// ## 2020/10/31 # 江东新风 # 同步honeybee大神的更新##
/*
@제작자: HoneyBee
@설명: 폭정을 행할 수 있는 메뉴 SCRIPT

※ 동탁, 손호 등.. 삼국지의 유명 폭군들의 CONCEPT을 반영한 SCRIPT입니다.

(1) 이벤트 - 금 or 군량을 획득
(2) 도시 초토화 - 군주가 소속된 도시를 공백지로 변경 (세력의 도시가 2개 이상인 경우만 가능)
(3) 도시 초토화를 사용하면 후한황제를 옹립한 세력의 도시일 경우, 후한황제를 해당 도시에 두고 이동함 (참고사항)
(4) 폭정을 행한 횟수에 따라서 군주의 상성, 야망, 말투가 변경됨
(5) 폭정을 행한 횟수에 따라서 군주의 별명이 추가 및 변경됨 (소인 -> 악인 -> 폭군)

@说明:可以进行暴政的菜单SCRIPT

※董卓、孙皓等。是反映了三国志的有名暴君的概念的SCRIPT。

(1)事件-获得黄金or兵粮
(2)焚城-君主所属的都市空白地变更(势力的都市有2个以上的情况才有可能)
(3)如果这座城市被摧毁了，如果是拥立皇帝的势力的城市，就自由地把皇帝放在那个城市移动(注)
(4)根据实行暴政的次数，君主的城，野心，言语有所改变
(5)根据施行暴政的次数，君主的称号追加和变更(小人- >恶人- >暴君)
*/

namespace 暴政
{

	// ================ CUSTOMIZE ================

	const int TP_COST = 100; 	  // 기교P 필요량
	const int ACTION_COST = 100;  // 행동력 필요량

	const int safety_ = 30;       // 폭정을 행하고 감소하는 치안 수치 
	const int city_hp = 100;      // 초토화된 도시의 내구 수치
	const int public_order = 50; //焚城后治安

	const array<int> food_ = { 5000, 7500, 10000 }; 	  // 백성들에게 약탈한 군량 수치 
	const array<int> gold_ = { 1000, 1500, 2000 }; 	  // 백성들에게 약탈한 금 수치 
	int pop_dec_ = 2000;

// ===========================================

	const int KEY = pk::hash("폭정");

	class Main
	{

		pk::building@ building_;
		pk::force@ force_;
		pk::person@ kunshu_;
		pk::person@ farmer_;
		pk::person@ merchant_;
		pk::person@ officer_;
		pk::person@ civilian_;
		pk::city@ city_;

		Main()
		{
			pk::menu_item item;
			item.menu = 100;
			item.pos = 8;
			item.shortcut = "8";
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
			@farmer_ = pk::get_person(무장_농민);
			@officer_ = pk::get_person(무장_무관);
			@merchant_ = pk::get_person(무장_상인);
			@civilian_ = pk::get_person(무장_문관);
			@city_ = pk::building_to_city(building);
		}

		bool isVisible()
		{
			//pk::trace("kunshu_.service" + kunshu_.service + "building_.get_id()" + building_.get_id());
			if (kunshu_.service != building_.get_id()) return false;
			return true;
		}

		bool isEnabled()
		{
			if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_)) return false;
			else if (kunshu_.action_done == true) return false;
			else if (force_.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		string getText()
		{
			return pk::encode("暴政");
		}

		string getDesc()
		{
			if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_))
				return pk::encode("君主不在.");
			else if (kunshu_.action_done == true)
				return pk::encode("君主已行动.");
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧不足.(需要{}技巧)", TP_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足(需要{}行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("实施暴政.(需要{}技巧, {}行动力)", TP_COST, ACTION_COST));
		}

		bool handler()
		{

			if (pk::choose(pk::encode("要实施暴政吗?"), { pk::encode(" 是 "), pk::encode(" 否 ") }) == 1)
				return false;


			array<string> select_list =
			{
				pk::encode("事件"),    // 0
				pk::encode("焚城"),     // 1
				pk::encode("取消")       // 2
			};

			int select_command = pk::choose(pk::encode("请选择要实施的暴政."), select_list);


			if (select_command == 0)
			{
				pk::scene(pk::scene_t(scene_Event));
				if (pk::is_in_screen(city_.get_pos())) pk::say(pk::encode("唉，这地方是待不下去了..."), pk::get_person(武将_农民), city_);
			}
			else if (select_command == 1)
			{
				if (pk::choose(pk::encode("要让城市变为一片焦土嘛?"),
					{ pk::encode(" 是 "), pk::encode(" 否 ") }) == 0)
				{
					if (pk::get_city_count(force_) >= 2)
					{
						pk::scene(pk::scene_t(scene_BurnedTheCity));
					}
					else
					{
						pk::encode("需要至少两座城市.");
						return false;
					}
				}
				else
				{
					pk::encode("暴政取消了.");
					return false;
				}
			}
			else if (select_command == 2)
			{
				pk::encode("取消执行暴政.");
				return false;
			}

			if ((kunshu_.azana_read != pk::encode("小人")) and (kunshu_.azana_read != pk::encode("恶人"))
				and (kunshu_.azana_read != pk::encode("暴君")))
			{
				kunshu_.azana_read = pk::encode("小人");
				kunshu_.aishou = 140; // 상성을 140으로 변경 
				kunshu_.update();
			}

			else if (kunshu_.azana_read == pk::encode("小人"))
			{
				kunshu_.azana_read = pk::encode("恶人");
				kunshu_.ambition = 야망_매우높음;
				kunshu_.update();
			}

			else if (kunshu_.azana_read == pk::encode("恶人"))
			{
				kunshu_.azana_read = pk::encode("暴君");
				kunshu_.tone = 말투_난폭;
				kunshu_.update();
			}

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			// 군주 행동 완료 
			kunshu_.action_done == true;

			return true;

		}


		void scene_Event()
		{

			int n = pk::rand(2);

			if (n == 0)
			{
				pk::fade(0);
				pk::sleep();
				pk::background(16);
				pk::fade(255);

				pk::message_box(pk::encode("额，你们在干什么?"), kunshu_);
				pk::message_box(pk::encode("近来庄稼长得好，村民们都在庆祝呢."), farmer_);
				pk::message_box(pk::encode("什么?!据我所知最近的干旱情况非常严重..."), kunshu_);
				pk::message_box(pk::encode("显然，你们这些家伙不是百姓，是盗贼!"), kunshu_);
				pk::message_box(pk::encode("这..? 这算什么..!"), farmer_);

				pk::background(14);
				pk::message_box(pk::encode("立即把在这里举行庆典的敌人全部杀死，确保粮食!"), kunshu_);
				pk::message_box(pk::encode("遵命!"), officer_);
				pk::message_box(pk::encode("呃.. 简直不是人.."), farmer_);

				int food_num = food_[pk::rand(3)];

				pk::message_box(pk::encode(pk::format("兵粮增加了\x1b[1x{}\x1b[0x.", food_num)));
				pk::add_food(city_, food_num, true);
				pk::add_tp(force_, -TP_COST, force_.get_pos());
				pk::add_public_order(city_, -safety_, true);
				ch::add_population(city_.get_id(), -pop_dec_);
				

				pk::fade(0);
				pk::sleep();
				pk::background(-1);
				pk::fade(255);
			}

			if (n == 1)
			{
				pk::fade(0);
				pk::sleep();
				pk::background(4);
				pk::fade(255);

				pk::message_box(pk::encode("街上好像很热闹."), kunshu_);
				pk::message_box(pk::encode("哈哈哈!请进。"), merchant_);
				pk::message_box(pk::encode("生意好吗?"), kunshu_);
				pk::message_box(pk::encode("你看，这里客人很多，生意很好。"), merchant_);
				pk::message_box(pk::encode("嗯. .最近有人说这条街上有盗贼。.."), kunshu_);
				pk::message_box(pk::encode("你小子!你一定是盗贼的头儿，这儿的家伙都是是你这们一伙人假扮的客人!!"), kunshu_);
				pk::message_box(pk::encode("这..? 这算什么..!"), merchant_);

				pk::background(14);
				pk::message_box(pk::encode("当场把这条街的家伙都杀光，没收全部财产!"), kunshu_);
				pk::message_box(pk::encode("遵命!"), officer_);
				pk::message_box(pk::encode("嗯...如果这世上有恶魔，必为主上.."), merchant_);

				int gold_num = gold_[pk::rand(3)];

				pk::message_box(pk::encode(pk::format("金因此增加了\x1b[1x{}\x1b[0x。", gold_num)));
				pk::add_gold(city_, gold_num, true);
				pk::add_tp(force_, -TP_COST, force_.get_pos());
				pk::add_public_order(city_, -safety_, true);
				ch::add_population(city_.get_id(),-pop_dec_);

				pk::fade(0);
				pk::sleep();
				pk::background(-1);
				pk::fade(255);
			}

		} // scene_Event()

		void scene_BurnedTheCity()
		{
			pk::fade(0);
			pk::sleep();
			pk::background(2);
			pk::fade(255);

			pk::message_box(pk::encode("这个城镇已是毫无价值的地方了。"), kunshu_);
			pk::message_box(pk::encode("与其给敌人，不如把它全部烧掉。"), kunshu_);
			pk::message_box(pk::encode("主公，那样会招怨百姓的.."), civilian_);
			pk::message_box(pk::encode("哼!那些鼠目寸光的家伙怎么能懂国家大计呢?!"), kunshu_);
			pk::message_box(pk::encode("但是..."), civilian_);
			pk::message_box(pk::encode("闭嘴!再阻挠一下，我必先斩断你的脖子!"), kunshu_);
			pk::message_box(pk::encode("..."), civilian_);

			//pk::background(21);
			//pk::background("media/background/huoshaoluoyang2.png", 1);
			pk::message_box(pk::encode("哈哈哈!全部给我烧掉!"), kunshu_);

			pk::set_hp(city_, city_hp);
			pk::set_public_order(city_, public_order);

			pk::neutralize(building_);

			pk::add_tp(force_, -TP_COST, force_.get_pos());
			int city_id = city_.get_id();
			int pre_pop = base_ex[city_id].population;
			ch::add_population(city_id, int(-pre_pop*0.3f));

			pk::fade(0);
			pk::sleep();
			pk::background(-1);

			pk::fade(255);
		}

	}

	Main main;
}
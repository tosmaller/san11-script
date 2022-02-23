// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2020/12/20 # 江东新风 # 尝试默认关闭地格显示 ##
// ## 2020/09/23 # keehl102 # 新增战斗测试功能，所有城市兵力兵装全满 ##
namespace Shift_우클릭_메뉴
{
	class Main
	{
		Main()
		{
			pk::add_menu_item(OptionMenu("San11Option.AlwaysActive", "后台时不停止").create_menu_item());
			pk::add_menu_item(OptionMenu("San11Option.FreeOperate", "控制电脑势力").create_menu_item());
			pk::add_menu_item(OptionMenu("San11Option.EnableInfiniteAction", "部队无限行动").create_menu_item());
			pk::add_menu_item(create_menu_item(function() { return pk::encode("无限行动力"); }, function() { return main.infinite_action_ ? 1249 : 889; }, function() { main.infinite_action_ = !main.infinite_action_; (main.infinite_action_ ? pk::set_ap_rate(0.f) : pk::set_ap_rate(1.f)); pk::get_district(pk::get_district_id(pk::get_force(pk::get_current_turn_force_id()), 1)).ap = 255; return true; }));
			pk::add_menu_item(create_menu_item(function() { return pk::encode("战斗测试"); }, function() { return main.battle_test_ ? 1249 : 889; }, function() { main.battle_test_ = !main.battle_test_; return true; }));
			pk::add_menu_item(OptionMenu("San11Option.ShowHex", "显示地格地形").create_menu_item());
			pk::add_menu_item(OptionMenu("San11Option.ShowHexInfo", "显示地格信息").create_menu_item());
			pk::add_menu_item(create_menu_item(function() { return pk::encode("地形编辑"); }, function() { return pk::core::is_editing_terrain() ? 1249 : 889; }, function() { pk::core::toggle_terrain_editor(); return true; }));
			pk::add_menu_item(OptionMenu("San11Option.ShowArea", "显示区域").create_menu_item());
			pk::add_menu_item(create_menu_item(function() { return pk::encode("地形纹理reload"); }, function() { return 889; }, function() { pk::core::load_terrain_texture(); return true; }));
			pk::add_menu_item(create_menu_item(function() { return pk::encode("FPS"); }, function() { return pk::core::is_show_fps() ? 1249 : 889; }, function() { pk::core::toggle_show_fps(); return true; }));
			pk::bind(107, pk::trigger107_t(new_day));
		}

		bool battle_test_ = false;
		bool infinite_action_ = false;
		void new_day()
		{
			if (battle_test_)
			{
				for (int i = 0; i < 도시_끝; i++)
				{
					pk::city@ city = pk::get_city(i);
					if (!pk::is_alive(city) or city.get_force_id() == -1)
						continue;

					ch::add_troops(city, 100000);
					pk::add_weapon_amount(city, 1, 100000);
					pk::add_weapon_amount(city, 2, 100000);
					pk::add_weapon_amount(city, 3, 100000);
					pk::add_weapon_amount(city, 4, 100000);
					pk::add_gold(city, 100000);
					pk::add_food(city, 100000);
				}
			}
		}
	}

	Main main;

	pk::menu_item create_menu_item(pk::menu_item_get_text_t@ get_text, pk::menu_item_get_image_id_t@ get_image_id, pk::menu_item_handler_t@ handler)
	{
		pk::menu_item item;
		item.menu = 2;
		item.get_text = get_text;
		item.get_image_id = get_image_id;
		item.handler = handler;
		return item;
	}

	class OptionMenu
	{
		string key_;
		string text_;
		int value_;

		OptionMenu(string key, string text)
		{
			key_ = key;
			text_ = text;
			pk::bind(100, pk::trigger100_t(title_screen));
			pk::bind(102, pk::trigger102_t(game_init));
		}

		void title_screen()
		{
			value_ = int(pk::option[key_]);
			pk::printf("{} {}\n", key_, value_);
		}

		void game_init()
		{
			pk::option[key_] = value_;
			pk::option["San11Option.ShowHex"] = 0;
			pk::option["San11Option.ShowHexInfo"] = 0;
			pk::option["San11Option.ShowArea"] = 0;

		}

		pk::menu_item create_menu_item()
		{
			pk::menu_item item;
			item.menu = 2;
			item.get_text = pk::menu_item_get_text_t(get_text);
			item.get_image_id = pk::menu_item_get_image_id_t(get_image_id);
			item.handler = pk::menu_item_handler_t(handler);
			return item;
		}

		string get_text()
		{
			return pk::encode(text_);
		}

		int get_image_id()
		{
			return value_ != 0 ? 1249 : 889;
		}

		bool handler()
		{
			//main.infinite_action_ ? float(pk::core["ap_rate"]) = 0.f : float(pk::core["ap_rate"]) = 1.f;
			
			value_ = 1 - value_;
			pk::option[key_] = value_;
			return true;
		}
	}
}
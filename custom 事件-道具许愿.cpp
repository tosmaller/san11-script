/*

概述：集齐特定道具召唤仙人许愿
1 游戏开始时设置五件道具作为任务道具，道具设置方式
	（1）默认。默认时【宝物_雌雄一对剑】，【宝物_方天画戟】，【宝物_孙子兵法】，【宝物_绝影】，【宝物_太平要术】
	（2）随机。
	（3）自定义(待定)。
2 五件任务道具都同属与同一势力且拥有者都在同一城市，触发许愿效果
3 可向仙人许一个愿望
	3.1 势力。
  	(1) 势力随机一半城市金粮全满
 	 	(2) 势力随机一半城市枪，戟，弩全满
  	(3) 势力随机一半城市军马全满
  	(4) 势力随机一半城市兵器全满
  3.2 军势
   （1）地图上非本势力部队全部眩晕或混乱5回合
   （2）地图上非本势力部队全部溃灭
  3.3 武将
   （1) 选势力内一武将五维低于100的变为100，适性低于S的变为S，寿命 + 10
   （2）势力内全部武将随机一项五维上升1
   （3）势力内全部武将随机一项适性上升一级
  3.4 特殊
   （1）劝降敌部队身份为普通的武将（必成功）
   （2）劝降敌部队身份为太守的武将（80%成功）
   （3）劝降敌部队身份为军团长的武将（50%成功）
	 （4）劝降任一势力（30%成功）
	 （5）任意一武将直接死亡
 */

namespace 道具许愿
{
    //---------------------------------------------------------------------------------------
		const int KEY_任务道具 = pk::hash("KEY_任务道具");

		bool 展开面板 = false;

    class Main
    {

    	pk::random random(pk::rand());

    	pk::building@ building_;
    	pk::building@ king_building;
    	pk::person@ 小乔 = pk::get_person(武将_小乔);

      Main()
      {
      	add_menu_道具许愿();
      	pk::bind(106, pk::trigger106_t(onReadSave));
      	pk::bind(111, pk::trigger111_t(onTurnStart));
			}
			pk::item@ 任务道具1;
			pk::item@ 任务道具2;
			pk::item@ 任务道具3;
			pk::item@ 任务道具4;
			pk::item@ 任务道具5;

	    // -------------- trigger ----------------
	    void onReadSave(int file_id)
	    {
	    	展开面板 = checkout_event_open();
	    }

	    void onTurnStart(pk::force@ force)
	    {
	    	check_item_complute(force);
	    	// if (force.is_player())
	    	// {
	    	// 	check_item_setting();
	    	// }
	    }

	    void add_menu_道具许愿()
			{
				pk::menu_item item_道具许愿;
				item_道具许愿.menu = global_menu::菜单_事件;
				item_道具许愿.init = pk::building_menu_item_init_t(init_道具许愿);
				item_道具许愿.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
				item_道具许愿.is_enabled = pk::menu_item_is_enabled_t(isEnabled_道具许愿);
				item_道具许愿.is_visible = pk::menu_item_is_visible_t(isVisible_道具许愿);
				item_道具许愿.get_text = pk::menu_item_get_text_t(getText_道具许愿);
				item_道具许愿.get_desc = pk::menu_item_get_desc_t(getDesc_道具许愿);
				item_道具许愿.handler = pk::menu_item_handler_t(handler_道具许愿);
				pk::add_menu_item(item_道具许愿);

				pk::menu_item item;

				item.menu = global_menu::菜单_事件;
				item.init = pk::building_menu_item_init_t(init_任务道具选项);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_查看任务道具);
				item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("1.查看任务道具"); });
				item.get_desc = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("查看设置的任务道具"); });
				item.handler = pk::menu_item_handler_t(handler_查看任务道具);
				pk::add_menu_item(item);

				item.menu = global_menu::菜单_事件;
				item.init = pk::building_menu_item_init_t(init_任务道具选项);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_检测任务道具);
				item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("2.检测任务道具"); });
				item.get_desc = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("检查已收集的任务道具"); });
				item.handler = pk::menu_item_handler_t(handler_检测任务道具);
				pk::add_menu_item(item);

				item.menu = global_menu::菜单_事件;
				item.init = pk::building_menu_item_init_t(init_任务道具选项);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_终止事件);
				item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("3.终止事件"); });
				item.get_desc = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("终止道具收集事件"); });
				item.handler = pk::menu_item_handler_t(handler_终止事件);
				pk::add_menu_item(item);

				item.menu = global_menu::菜单_事件;
				item.init = pk::building_menu_item_init_t(init_任务道具选项);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_开启事件);
				item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("1.开启道具收集事件"); });
				item.get_desc = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("开启道具收集事件"); });
				item.handler = pk::menu_item_handler_t(handler_开启事件);
				pk::add_menu_item(item);

				item.menu = global_menu::菜单_事件;
				item.init = pk::building_menu_item_init_t(init_任务道具选项);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_查看事件说明);
				item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("2.查看事件说明"); });
				item.get_desc = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("查看事件说明"); });
				item.handler = pk::menu_item_handler_t(handler_查看事件说明);
				pk::add_menu_item(item);
			}

	    // -------------- function ----------------
	    void init_道具许愿(pk::building@ building)
			{
				@building_ = building;
				pk::force@ force_ = pk::get_force(building_.get_force_id());
				pk::person@ kunshu_ = pk::get_person(pk::get_kunshu_id(building_));
				@king_building = pk::get_building(kunshu_.service);
			}

			int getImageID_1249()   // 주황색
			{
				return 1249;
			}

			string getText_道具许愿()
			{
				bool open = checkout_event_open();
				return pk::encode(pk::format("道具收集事件\x1b[1x{}\x1b[0x", open ? '(已开启)' : '(未开启)' ));
			}

			bool isEnabled_道具许愿()
			{
				return true;
			}

			bool isVisible_道具许愿()
			{
				if (building_.get_id() != king_building.get_id()) return false;
				return true;
			}

			string getDesc_道具许愿()
			{
				return pk::encode("收集任务道具可许愿.");
			}

			bool handler_道具许愿()
			{
				展开面板 = 展开面板 ? false : true;
				return false;
			}

			void init_任务道具选项(pk::building@ building)
			{

			}

			// 查看任务道具
			bool isMenuVisible_查看任务道具()
			{
				return 展开面板 and checkout_event_open();
			}
			bool handler_查看任务道具()
			{

				pk::list<pk::item@> items = get_items();

				if (items.count > 0)
				{
					string str;
					for (int i = 0; i < items.count; i += 1)
					{
						str += pk::format(",\x1b[27x{}\x1b[0x", pk::decode(pk::get_name(items[i])));
					}

					pk::message_box(pk::encode(pk::format("当前任务道具分别是{}", str)), 小乔);
				}
				return false;
			}

			// 检测任务道具
			bool isMenuVisible_检测任务道具()
			{
				return 展开面板 and checkout_event_open();
			}
			bool handler_检测任务道具()
			{
				int count = 0;
				string str = "已收集任务道具有";

				pk::list<pk::item@> items = get_items();

				for (int i = 0; i < items.count; i += 1)
				{
					pk::person@ owner = pk::get_person(items[i].owner);
					if (owner !is null and owner.get_force_id() == king_building.get_force_id())
					{
						count += 1;
						str += pk::format(",\x1b[27x{}\x1b[0x", pk::decode(pk::get_name(items[i])));
					}
				}

				pk::message_box(pk::encode(count == 0 ? '还没有收集任务道具呢，快去收集吧' : str), 小乔);
				return false;
			}

			// 终止事件
			bool isMenuVisible_终止事件()
			{
				return 展开面板 and checkout_event_open();
			}
			bool handler_终止事件()
			{

				pk::list<pk::item@> items = get_items();
				int count = 0;

				for (int i = 0; i < items.count; i += 1)
				{
					pk::person@ owner = pk::get_person(items[i].owner);
					if (owner !is null and owner.get_force_id() == king_building.get_force_id())
					{
						count += 1;
					}
				}

				bool confirm = pk::yes_no(pk::encode(pk::format("确定终止\x1b[1x道具收集\x1b[0x事件吗？ \n您已收集\x1b[2x{}\x1b[0x件任务道具了?", count)));
				if (confirm)
				{
					pk::message_box(pk::encode("已终止\x1b[27x道具收集\x1b[0x事件呢，可以随时开启哦。"), 小乔);
					pk::store(KEY_任务道具, 0, 0);
				}
				return false;
			}

			// 开启事件
			bool isMenuVisible_开启事件()
			{
				return 展开面板 and !checkout_event_open();
			}
			bool handler_开启事件()
			{
				item_settin();
				return false;
			}

			// 事件说明
			bool isMenuVisible_查看事件说明()
			{
				return 展开面板 and !checkout_event_open();
			}
			bool handler_查看事件说明()
			{
				pk::message_box(pk::encode("收集五件特定任务道具后，则可召唤\x1b[27x仙人\x1b[0x许愿哦。"), 小乔);
				return false;
			}

			//---------------------------- Common Function----------------------------

			bool checkout_event_open()
			{
				const int setting = int(pk::load(KEY_任务道具, 0, 0));
				bool open = setting == 1 or setting == 2;
				return open;
			}

			pk::list<pk::item@> get_items()
			{
				pk::list<pk::item@> items;
				if (checkout_event_open())
				{
					pk::item@ _任务道具1 = pk::get_item(uint32(pk::load(KEY_任务道具, 1, -1)));
					pk::item@ _任务道具2 = pk::get_item(uint32(pk::load(KEY_任务道具, 2, -1)));
					pk::item@ _任务道具3 = pk::get_item(uint32(pk::load(KEY_任务道具, 3, -1)));
					pk::item@ _任务道具4 = pk::get_item(uint32(pk::load(KEY_任务道具, 4, -1)));
					pk::item@ _任务道具5 = pk::get_item(uint32(pk::load(KEY_任务道具, 5, -1)));
					if (_任务道具1 !is null)
					{
						items.add(_任务道具1);
					}
					if (_任务道具2 !is null)
					{
						items.add(_任务道具2);
					}
					if (_任务道具3 !is null)
					{
						items.add(_任务道具3);
					}
					if (_任务道具4 !is null)
					{
						items.add(_任务道具4);
					}
					if (_任务道具5 !is null)
					{
						items.add(_任务道具5);
					}
				}
				return items;
			}
	    void item_settin()
	    {
	    	array<string> select =
				{
					pk::encode("默认"),
					pk::encode("随机"),
					// pk::encode("自定义"),
				};

				int n= pk::choose(pk::encode("请设置任务道具"), select);

				if (n == 0)
				{
					@任务道具1 = pk::get_item(宝物_雌雄一对剑);
					@任务道具2 = pk::get_item(宝物_方天画戟);
					@任务道具3 = pk::get_item(宝物_孙子兵法);
					@任务道具4 = pk::get_item(宝物_绝影);
					@任务道具5 = pk::get_item(宝物_太平要术);
					pk::store(KEY_任务道具, 0, 1);
					pk::store(KEY_任务道具, 1, 任务道具1.get_id());
					pk::store(KEY_任务道具, 2, 任务道具2.get_id());
					pk::store(KEY_任务道具, 3, 任务道具3.get_id());
					pk::store(KEY_任务道具, 4, 任务道具4.get_id());
					pk::store(KEY_任务道具, 5, 任务道具5.get_id());
					pk::message_box(pk::encode("设置成功，请到游戏中体验吧。\n 小提示：可使用【查看任务道具】菜单查看设置的的道具哦"), 小乔);
				}
				else if (n == 1)
				{
					array<int> old_items = {};
					array<int> new_items = {};
					for (int l = 0; l <= 42; l++)
					{
						old_items.insertLast(l);
					}
					for (int l = 50; l <= 98; l++)
					{
						new_items.insertLast(l);
					}
					string str;
					for (int i = 1; i <= 5 ; i += 1)
					{
						int item_id;
						if (pk::rand_bool(50)) {
							int index = pk::rand(old_items.length);
							item_id = old_items[index];
							old_items.removeAt(index);
						}
						else
						{
							int index = pk::rand(new_items.length);
							item_id = new_items[index];
							new_items.removeAt(index);
						}
						pk::trace("任务道具:" + item_id);
						str += ("," + item_id);
						switch(i)
						{
							case 1:
								@任务道具1 = pk::get_item(item_id);
								break;
							case 2:
								@任务道具2 = pk::get_item(item_id);
								break;
							case 3:
								@任务道具3 = pk::get_item(item_id);
								break;
							case 4:
								@任务道具4 = pk::get_item(item_id);
								break;
							case 5:
								@任务道具5 = pk::get_item(item_id);
								break;
						}
					}

					// int id_1 = items[pk::rand(items.length)];
					// pk::trace("任务道具:" + items.length + "," +id_1);
					// @任务道具1 = pk::get_item(id_1);
					// items.removeAt(id_1);

					// int id_2 = items[pk::rand(items.length)];
					// pk::trace("任务道具:" + items.length + "," +id_2);
					// @任务道具2 = pk::get_item(id_2);
					// items.removeAt(id_2);

					// int id_3 = items[pk::rand(items.length)];
					// pk::trace("任务道具:" + items.length + "," +id_3);
					// @任务道具3 = pk::get_item(id_3);
					// items.removeAt(id_3);

					// int id_4 = items[pk::rand(items.length)];
					// pk::trace("任务道具:" + items.length + "," +id_4);
					// @任务道具4 = pk::get_item(id_4);
					// items.removeAt(id_4);

					// int id_5 = items[pk::rand(items.length)];
					// pk::trace("任务道具:" + items.length + "," +id_5);
					// @任务道具5 = pk::get_item(id_5);
					// items.removeAt(id_5);

					pk::store(KEY_任务道具, 0, 2);
					pk::store(KEY_任务道具, 1, 任务道具1.get_id());
					pk::store(KEY_任务道具, 2, 任务道具2.get_id());
					pk::store(KEY_任务道具, 3, 任务道具3.get_id());
					pk::store(KEY_任务道具, 4, 任务道具4.get_id());
					pk::store(KEY_任务道具, 5, 任务道具5.get_id());
					pk::message_box(pk::encode("设置成功，请到游戏中体验吧。\n 小提示：可使用【查看任务道具】菜单查看设置的的道具哦"), 小乔);
					pk::trace("任务道具:" + str);
				}
				else if (n == 2)
				{

				}
	    }

	    void check_item_complute(pk::force@ force)
	    {

	    }

	}
   Main main;
}

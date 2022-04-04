/***CUSTOM
@name 联机专用_据点转让
@class 联机；外交
@author 氕氘氚
@contact 
@version 1.0.0
@date 2021/7/25
@desc 可以将据点转让给盟友
@notice 
***/



namespace 联机专用_据点转让
{
    // 常量 --------------------------------------------------
    // const bool/int/float 全局常量 = XXX;
	const int TP_COST = 200;
	const int ACTION_COST = 50;
    
    // --------------------------------------------------
    class Main
    {
        // 全局变量 --------------------------------------------------
        
		pk::building@ building_;
		pk::person@ kunshu_;
		pk::force@ force_;
        pk::list<pk::district@> friend_list;

        // --------------------------------------------------

        Main()
        {
            // 脚本执行入口
            menu_set();
        }
        
        // 其他函数
        void menu_set()
		{
			pk::menu_item item;
			item.menu = 103;
			item.pos = 10;
			item.init = pk::building_menu_item_init_t(menu_init);
			item.is_visible = pk::menu_item_is_visible_t(menu_isVisible);
			item.is_enabled = pk::menu_item_is_enabled_t(menu_isEnabled);
			item.get_text = pk::menu_item_get_text_t(menu_getText);
			item.get_desc = pk::menu_item_get_desc_t(menu_getDesc);
			item.handler = pk::menu_item_handler_t(menu_handler);
			
            pk::add_menu_item(item);
		}

        void menu_init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@kunshu_ = pk::get_person(force_.kunshu);
            friend_list.clear();
		}

		bool menu_isVisible()
		{
            if (pk::get_player_count() < 2) return false;//玩家超过2个才显现
            for (int i=0; i<군단_끝; i++){
                pk::district@ district = pk::get_district(i);
                if (!pk::is_enemy(force_, district) && district.get_force_id() != force_.get_id()){
                    friend_list.push_back(district);
                }
            }
            if (friend_list.count > 0)
			    return true;
            else
                return false;
		}

		bool menu_isEnabled()
		{
            if (force_.tp < TP_COST) return false;
            else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		string menu_getText()
		{
			return pk::encode("据点转让");
		}

		string menu_getDesc()
		{
			if (force_.tp < TP_COST)
                return pk::encode(pk::format("技巧点不足.(技巧点 {})", TP_COST));
            if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
                return pk::encode(pk::format("行动力不足.(需要行动力 {} )", ACTION_COST));
			else
				return pk::encode(pk::format("进行据点转让.(使用技巧点 {} )", TP_COST));
		}

		bool menu_handler()
		{
            pk::list<pk::person@> list = pk::get_person_list(building_, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
            if (list.count > 0){
                pk::message_box(pk::encode("据点内不能有我势力武将."));
                return false;
            }
            pk::list<pk::district@> district_sel = pk::district_selector(pk::encode("选择军团"), pk::encode("请选择军团"), friend_list, 1, 1);
            if (district_sel.count == 0) return false;
            pk::district@ district_t = district_sel[0];
            
            if (pk::choose(pk::encode("确定要转让据点吗?"), { pk::encode(" 是 "), pk::encode(" 否 ") }) == 0){
                pk::set_district(building_, district_t, 0);	
                return true;
            }
			return false;
        }
        
        
    } // end Main
    
    Main main;
    
} // end namespace

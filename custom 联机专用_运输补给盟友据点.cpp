<<<<<<< HEAD
=======
// ## 2022/03/17 # 江东新风 # 限制对空白据点的补给 ##
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
/***CUSTOM
@name 联机专用_运输补给盟友据点
@class 联机；运输
@author 氕氘氚
@contact 
@version 1.0.0
@date 2021/7/25
@desc 运输队可以将全部兵力资源补给到盟友据点。
@notice 
***/
<<<<<<< HEAD
=======



>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
namespace 联机专用_运输补给盟友据点
{
    // 常量 --------------------------------------------------
    // const bool/int/float 全局常量 = XXX;
    const int 资源损失比例 = 90;

    // --------------------------------------------------
    class Main
    {
        pk::unit@ unit_;
        pk::building@ building_;
        pk::point pos_;
        pk::list<pk::district@> friend_list;

        Main()
        {
            // 脚本执行入口
            menu_set();
        }
        
        // 其他函数
        void menu_set()
		{
			pk::menu_item item;
			item.menu = 1;
			item.pos = 10;
			item.init = pk::unit_menu_item_init_t(menu_init);
			item.is_visible = pk::menu_item_is_visible_t(menu_isVisible);
			item.is_enabled = pk::menu_item_is_enabled_t(menu_isEnabled);
			item.get_text = pk::menu_item_get_text_t(menu_getText);
			item.get_desc = pk::menu_item_get_desc_t(menu_getDesc);
			item.handler = pk::unit_menu_item_handler_t(menu_handler);
			
            pk::add_menu_item(item);
		}

        void menu_init(pk::unit@ unit, pk::point pos)
		{
			@unit_ = @unit;
			pos_ = pos;
		}

		bool menu_isVisible()
		{
            pk::array<pk::point> points = pk::range(pos_, 1, 1);
            for (int i=0;i<6;i++) {
                @building_ = pk::get_building(points[i]);
                if (building_ is null || building_.facility > 2)
                    continue;
<<<<<<< HEAD
=======
                if (building_.get_force_id() == -1) continue;//加入空白据点限制
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
                if (!pk::is_enemy(unit_, building_) && unit_.get_force_id() != building_.get_force_id()){
                    return true;
                }
            }
            return false;
		}

		bool menu_isEnabled()
		{
			return true;
		}

		string menu_getText()
		{
			return pk::encode("补给盟友据点");
		}

		string menu_getDesc()
		{
            return pk::encode(pk::format("部队的全部兵力资源将进入盟友据点，武将返回(会有部分资源损失)", ""));
		}

		bool menu_handler(pk::point pos)
		{
            int troops = unit_.troops;
            int energy = unit_.energy;
            int gold = unit_.gold * 资源损失比例 / 100;
            int food = unit_.food * 资源损失比例 / 100;
            array<int> stock(12, 0);
            for (int i=0;i<12;i++)
                stock[i] = unit_.stock[i].amount * 资源损失比例 / 100;
            
            if (building_.facility == 0){
                pk::city@ city = pk::building_to_city(building_);
                city.energy = (city.troops * city.energy + troops * energy) / (city.troops + troops);
                city.troops = city.troops + troops;
                city.gold = city.gold + gold;
                city.food = city.food + food;
                for (int i=0;i<12;i++)
                    city.weapon_amount[i] = city.weapon_amount[i] + stock[i];
            }
            else if (building_.facility == 1){
                pk::gate@ gate = pk::building_to_gate(building_);
                gate.energy = (gate.troops * gate.energy + troops * energy) / (gate.troops + troops);
                gate.troops = gate.troops + troops;
                gate.gold = gate.gold + gold;
                gate.food = gate.food + food;
                for (int i=0;i<12;i++)
                    gate.weapon_amount[i] = gate.weapon_amount[i] + stock[i];
            }
            else if (building_.facility == 2){
                pk::port@ port = pk::building_to_port(building_);
                port.energy = (port.troops * port.energy + troops * energy) / (port.troops + troops);
                port.troops = port.troops + troops;
                port.gold = port.gold + gold;
                port.food = port.food + food;
                for (int i=0;i<12;i++)
                    port.weapon_amount[i] = port.weapon_amount[i] + stock[i];
            }
            pk::kill(unit_);
            return true;
        }
        
        
    } // end Main
    
    Main main;
    
} // end namespace

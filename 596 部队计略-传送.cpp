// ## 2021/12/05 # 江东新风 # 传送失败的情况 ##
// ## 2021/11/09 # 江东新风 # 解决传送动画问题 ##
// @@ 2020/11/21 @ 江东新风 @ 初次编写 @@
/*   
@作者：江东新风
@說明：张角或拥有太平要术武将专属，可传送己方部队
@备注： 
@计划： 传送的失败剧情以及特殊剧情
		
				
*/
namespace 部队计略_传送
{
	
	
	// ================ CUSTOMIZE ================
		
		
		const int ENERGY_COST_传送 = 25;   	 // 气力消耗


		// 计略特技兵力相关设定
		const bool 计略_特技_设定  = true;        // 启用百出优待
		const bool 百出_减半_设定  = true;        // true= 50% 减少, false = 均为1
		const bool 计略_兵力_区分  = true;        // 随兵力调整耗气
		const int 传送_可执行距离 = 40;
		
		const int KEY = pk::hash("计略传送");//팎뻐

	
	
	
	class Main
	{
		
		pk::force@ force;
		pk::unit@ src_unit;
		pk::point src_pos_;
		pk::person@ src_leader;


		pk::point pos_for_screen_move;


		
		Main()
		{	
			pk::menu_item item;
			item.menu = 115;
			item.pos = 8;
			//item.shortcut = "";
			item.init = pk::unit_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(isVisible);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.get_targets = pk::unit_menu_item_get_targets_t(getTargets);
			item.get_chance = pk::unit_menu_item_get_chance_t(getChance_传送);			
			item.handler = pk::unit_menu_item_handler_t(handler);
			pk::add_menu_item(item);
		
		}
		
        //-------------------计略相关功能-----------------------------------------
 		
        uint8 func_stragety_cost(int energy_cost)
        {
            if (!计略_特技_设定) return energy_cost;
            
            uint8 n = uint8(energy_cost);
            if (src_unit.has_skill(特技_百出))
            {
                if (百出_减半_设定)
                    n /= 2; // 기력 소모량 절반 감소
                else
                    n = 1;  // 기력 소모량 무조건 1로
            }
            
            //병력수에 따른 기력 소모량 차등 - 소부대 기력 소모 증가, 대부대 기력 소모 감소
            if (计略_兵力_区分)
            {
                if (src_unit.troops <= 2000)
                    n = uint8(n * pk::min(10.f, 2000.f/src_unit.troops));         // 최대 10배 소모
                else if (src_unit.troops >= 10000)
                    n = uint8(n * pk::max(0.5f, (2-src_unit.troops/10000.f)));    // 최소 50% 소모
            }
            
            return n;
        }

        int func_range_add()
		{
            if (!计略_特技_设定) return 0;
            
            if (src_unit.has_skill(特技_鬼谋))
                return 1;
            
            return 0;
        }
        
       
        bool call_stragety_chance(pk::point dst_pos, int strategy_id)
        {
            int chance = (pk::int_bool(cast<pk::func206_t>(pk::get_func(206))(src_unit, src_pos_, dst_pos, strategy_id))).first;
            return pk::rand_bool(chance);
        }
		
        pk::int_bool call_stragety_chance2(pk::point dst_pos, int strategy_id)
        {
            //int chance = (pk::int_bool(cast<pk::func206_t>(pk::get_func(206))(src_unit, src_pos_, dst_pos, strategy_id))).first;
			//bool 看破 = (pk::int_bool(cast<pk::func206_t>(pk::get_func(206))(src_unit, src_pos_, dst_pos, strategy_id))).second;
            return pk::int_bool(cast<pk::func206_t>(pk::get_func(206))(src_unit, src_pos_, dst_pos, strategy_id));
        }		
		
		void init(pk::unit@ unit, pk::point src_pos)
		{
			@src_unit = @unit;
			src_pos_ = src_pos;
			@src_leader = pk::get_person(src_unit.leader);
			@force = pk::get_force(src_unit.get_force_id());
			
		}

        int getChance_传送(pk::point dst_pos)
        {
            return call_stragety_chance2(dst_pos, 计略_落雷).first;
        }		
		//if (敌军出现城市[index].find(i) >= 0)
		bool isVisible()
		{
            if (src_leader.get_id() == 403 and pk::decode(pk::get_name(src_leader)) == "张角")
				return true;
			return false;
			
		}
		
		bool isEnabled()
		{
			if (src_unit.energy < func_stragety_cost(ENERGY_COST_传送)) return false;
			return getTargets().length > 0;
		}

		string getText()
		{
			return pk::encode("传送");//备用	
		}

		string getDesc()
		{
            if (src_unit.energy < func_stragety_cost(ENERGY_COST_传送))
                return pk::encode("气力不足");
            else if (getTargets().length == 0)
                return pk::encode("周围没有目标");
            else
                return pk::encode("进行传送");
		}
		
        pk::array<pk::point_int> getTargets()
        {
			pk::array<pk::point_int> target_units;
			auto range = pk::range(src_pos_, 1, 2 + func_range_add());
			for (int i = 0; i < int(range.length); i++)
			{
				auto dst_unit = pk::get_unit(range[i]);
				if (dst_unit !is null and src_unit.pos != dst_unit.pos and src_unit.get_force_id() == dst_unit.get_force_id())
					target_units.insertLast(pk::point_int(range[i], 1));
			}
			return target_units;
		}

		bool handler(pk::point dst_pos)
		{
			pk::list<pk::building@> building_list = getTargetList(dst_pos);

			if (building_list.count == 0)
			{
				pk::message_box(pk::encode("周围没有合适的目标。"), src_leader);
				return false;
			}
			pk::list<pk::building@> list_target = pk::building_selector(pk::encode("传送的大致位置"), pk::encode("目标位置."), building_list, 1, 1);
			if (list_target.count == 0) return false; // 未选擇时取消结束
			pk::building@ target_base = list_target[0];
			bool wrong_result = pk::rand_bool(10);
			if (wrong_result) @target_base = pk::get_building(pk::rand(据点_末));//10概率成功时传到错误目标

            pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_传送), true);
            //直接按照落雷计算成功率
            if (call_stragety_chance(dst_pos, 计略_落雷))
            {
				pk::play_se(120);
				pk::special_cutin(126,1000);
				
				pk::unit@ target_unit = pk::get_unit(dst_pos);
			
				if (pk::is_in_screen(src_pos_))
				{
					pk::create_effect(0x3a, src_pos_);
				}

				pk::say(pk::encode("这就是\x1b[29x奇门遁甲\x1b[0x!"), pk::get_person(src_unit.leader));
            
				// 传送的执行
				pk::point target_pos = getEmptyPos(target_base);//pk::get_neighbor_pos(src_unit.pos, 1, 5);
				pk::person @ unit_leader = pk::get_person(target_unit.leader);

				pk::create_effect(58, dst_pos);
	
				if (pk::is_in_screen(target_unit.pos))
				{
					pk::play_se(81, target_unit.pos);
					pk::create_effect(63, target_unit.pos);
					pk::wait(1200);
					
					//pk::wait(300);
				}
				/*
				auto range = pk::range(target_unit.pos, 1, 1);
				for (int i = 0; i < int(range.length); i++)
				{
					pk::create_effect(64, range[i]);
				}*/

				if (pk::is_in_screen(target_pos)) pk::create_effect(75, target_pos);
				pk::set_pos(target_unit, target_pos);
				pk::update_unit(target_unit);
				//pk::move(target_unit, steps);
				pk::play_se(340);//354,367
            
				if (!wrong_result) { if (pk::is_in_screen(target_unit.pos)) pk::say(pk::encode("哇!"), pk::get_person(target_unit.leader)); }
				else
				{
					if (pk::is_in_screen(target_unit.pos)) pk::say(pk::encode("这里是哪里？"), pk::get_person(target_unit.leader));
					if (pk::is_in_screen(src_pos_)) pk::say(pk::encode("传送应该是成功了...\n(但是好像哪里不对...)"), pk::get_person(src_unit.leader));
				}
				
            
				// 效果 : 气力上升效果
				//if (pk::is_in_screen(target_unit.pos)) pk::create_effect(63, target_unit.pos);//63妖术 64落雷 68类妖术 75迷雾
                
                
            
                ch::add_tp(pk::get_force(src_unit.get_force_id()), 10, dst_pos);
     
                pk::add_stat_exp(src_unit, 武将能力_智力, 4);
        
                pk::add_kouseki(src_unit, 100);
            }
            else
            {
				
                if (pk::is_in_screen(src_pos_))
                {
                    switch(pk::rand(2))
                    {
                        case 0: pk::say(pk::encode("传送失败了"), pk::get_person(src_unit.leader)); break;
                        case 1: pk::say(pk::encode("天威难测"), pk::get_person(src_unit.leader)); break;
                    }
                }
            }
            
            // 행동완료
            src_unit.action_done = true;
            if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
                src_unit.action_done = false;
            
            return true;
			
		}
		pk::list<pk::building@> getTargetList(pk::point src_pos)
		{
			pk::list<pk::building@> list;
			for (int i = 0; i < 建筑_据点末; i++)
			{
				pk::building@ dst = pk::get_building(i);
				if (pk::get_distance(src_pos, dst.get_pos()) <= 传送_可执行距离)
					list.add(dst);
			}
			return list;
		}
		pk::point getEmptyPos(pk::building@ building)
		{
			auto arr = pk::range(building.get_pos(), 1, 4);
			for (int i = 0; i < int(arr.length); i++)
			{
				pk::hex@ hex = pk::get_hex(arr[i]);
				int terrain_id = hex.terrain;

				if (!hex.has_building and !hex.has_unit
					and pk::is_valid_terrain_id(terrain_id)
					and pk::is_enabled_terrain(terrain_id)
					and terrain_id != 地形_山 and terrain_id != 地形_岸 and terrain_id != 地形_河 and terrain_id != 地形_川 and terrain_id != 地形_海)
					return arr[i];

			}
			return -1;
		}
	}

	Main main;
}
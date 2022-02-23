/***CUSTOM
@name 辅佐效果加强
@class 
@author 江东新风
@contact 
@version 1.0
@date 2020/12/24
@desc 加强辅佐
@notice 
***/
// ## 2020/12/24 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/07/26 ##
namespace 辅佐修改
{
	///////////////////////////////////////////////////////////////////////////////
	/////////////////////////////				///////////////////////////////////
	/////////////////////////////	CUSTOMIZE	///////////////////////////////////
	/////////////////////////////				///////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	const bool 辅佐修改 = true;//总开关
	const int 技巧上升量 = 25; //
	const int 治安上升量 = 3; //



	class Main
	{


		Main()
		{
			pk::bind(112, pk::trigger112_t(onTurnEnd));//气力回复

		}


		//回合结束时触发		
		void onTurnEnd(pk::force@ force)
		{
			if (辅佐修改)
			{
				auto city_list = pk::list_to_array(pk::get_city_list(force));
				bool 增加技巧 = false;
				for (int i = 0; i < int(city_list.length); i++)
				{
					pk::building@building = pk::city_to_building(city_list[i]);
					if (building.has_skill(26))	//특기_보좌
					{
						pk::add_public_order(city_list[i], 治安上升量, true);
						增加技巧 = true;
					}
				}
				if (增加技巧)
					ch::add_tp(force, 技巧上升量, force.get_pos());
			}
		}


	} // end - Main
	Main main;
} // end - namespace
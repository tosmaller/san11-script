// ## 2021/10/26 # 江东新风 # 写法改进 ##
// ## 2021/01/28 # 白马叔叔 # 智力从高到低排序 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/11/19 # 白马叔叔 # 稍稍优化语句，方便理解 ##
// ## 2020/10/30 # 江东新风 # 同步马术书大神的更新 ##
// ## 2020/09/21 # 江东新风 # 添加法治效果 ##
// ## 2020/08/01 # 氕氘氚 # 稍加强舌战难度，设置市价季节上下限，治安下降 ##
/*
@ 만든이: 기마책사 作者：骑马买书
@ UPDATE: '19.1.19    / 최초적용 : 상인과 설전하여 시세를 흥정 最初版本：和商人討价还价
@ UPDATE: '19.5.1     / 변경내용 : 결과에 따른 경험치, 상병 등을 적용 变更内容：舌战结果的经验??賞等
*/

namespace 도시시세흥정
{
	//---------------------------------------------------------------------------------------
    
	const int ACTION_COST = 20;
    const int 治安减少 = 6;
    const array<int> 市价下限 = {3, 3, 4, 2};
    const array<int> 市价上限 = {7, 7, 9, 7};
    
    //---------------------------------------------------------------------------------------
    
    
	
	class Main
	{
		
		pk::building@ building;
		pk::force@ force;
		pk::city@ city;
        pk::district@ district;
        pk::point building_pos;
		pk::person@ taishu;
		pk::person@ actor;
		pk::person@ merchant;
        uint16 building_gold;
        uint8 currency;
        int debate_result;
        array<int> arr_stat = {65, 70, 70, 75, 75, 75, 80, 80, 85, 90};

		Main()
		{
            // 討价还价菜单(玩家用) 시세흥정 메뉴 (유저용)
			pk::menu_item 시세흥정;
			시세흥정.menu = 100;        // 도시메뉴
			시세흥정.pos = 6;
			시세흥정.shortcut = "6";
			시세흥정.init = pk::building_menu_item_init_t(init);
			시세흥정.is_visible = pk::menu_item_is_visible_t(isVisible_시세흥정);
			시세흥정.is_enabled = pk::menu_item_is_enabled_t(isEnabled_시세흥정);
			시세흥정.get_text = pk::menu_item_get_text_t(getText_시세흥정);
			시세흥정.get_desc = pk::menu_item_get_desc_t(getDesc_시세흥정);
			시세흥정.handler = pk::menu_item_handler_t(handler_시세흥정);
			pk::add_menu_item(시세흥정);
            
		}

        
        //---------------------------------------------------------------------------------------
        
		void init(pk::building@ building_t)
		{
			@building = @building_t;
			@force = pk::get_force(building.get_force_id());
            @city = pk::building_to_city(building);
            @taishu = pk::get_person(pk::get_taishu_id(building));
            @district = pk::get_district(building.get_district_id());
            
            building_gold = pk::get_gold(building);
            building_pos = building.pos;
            currency = city.rate/10;
            
            @merchant = pk::get_person(무장_상인);
            merchant.base_stat[武将能力_智力] = arr_stat[pk::rand(10)];
            merchant.update();
		}

		string getText_시세흥정()
		{
			return pk::encode("讨价还价");
		}

		string getDesc_시세흥정()
		{
            if (pk::enemies_around(building))
                return pk::encode("周围存在敌人.无法执行");
            else if (city.merchant_done) 
                return pk::encode("已经完成与商人的交易");
            else if (pk::get_idle_person_list(building).count == 0)
                return pk::encode("没有可行动的武将");
            else if (int(district.ap) < ACTION_COST)
                return pk::encode(pk::format("行动力不足 (需要行动力{} )", ACTION_COST));
			else
				return pk::encode(pk::format("与商人讨价还价 (消耗行动力{} )", ACTION_COST));
		}

		bool isEnabled_시세흥정()
		{ 
            if (pk::enemies_around(building)) return false;
            else if (city.merchant_done) return false;
            else if (pk::get_idle_person_list(building).count == 0) return false;
            else if (int(district.ap) < ACTION_COST) return false;
			return true;
		}
        
        bool isVisible_시세흥정()
        {
			if (pk::is_campaign()) return false;
            if (!pk::is_alive(city)) return false;
            return true;
        }

		bool handler_시세흥정()
		{
            // 可执行武将名单 실행가능 무장리스트
            pk::list<pk::person@> person_list = pk::get_idle_person_list(building);
			person_list.sort(function(a, b)
            {
				return a.stat[2] > b.stat[2];  // 按智力排序
            });
			if (person_list.count == 0) return false;
            
            bool deal = false;
            int answer = -1;
            int season = pk::get_season();
			if (pk::is_player_controlled(building))
            {
                while(!deal)
                {
                    answer = pk::choose({ pk::encode("降低粮价"), pk::encode("提高粮价"), pk::encode(" 否 ") }, 
                                          pk::encode(pk::format("是否与商人讨价还价? \n(现在行情 粮食{}=资金1)\n*降低粮价利于\x1b[29x买粮\x1b[0x,提高粮价利于\x1b[29x卖粮\x1b[0x*", currency)),
                                          taishu );
                    
                    if (answer == 2) return false;  // 取消时终止 취소 종료
                    
                    else if (answer == 1 and currency <= uint8(市价下限[season]))
                    {
                        pk::message_box(pk::encode("不能再提高价格了"), taishu);
                        deal = false;
                    }
                    else if (answer == 0 and currency >= uint8(市价上限[season]))
                    {
                        pk::message_box(pk::encode("不能再降低价格了"), taishu);
                        deal = false;
                    }
                    else
                        deal = true;
                }
            }
            
            // 执行武将选擇 실행무장 선택하기
            if (pk::is_player_controlled(building))
            {
               pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择进行讨价还价的武将."), person_list, 1, 1);
               if (person_sel.count == 0) return false; // 未选擇时或取消时结束 미선택 시 취소 종료
               @actor = @person_sel[0];
            }
            else
            {
                person_list.sort(function(a, b)
                {
                    return (a.stat[武将能力_智力] > b.stat[武将能力_智力]) ; // 武将排序(按智力) 무장 정렬 (지력순)
                });
                @actor = @person_list[0];
            }
            
            // 和商人討价还价상인과 흥정
            pk::scene(pk::scene_t(scene_시세흥정));

            // ?除行动力 행동력 차감
            if (force.is_player())
                pk::add_ap(district, -ACTION_COST);
            
            // 将执行武将和交易设置成完成 실행무장 행동완료 및 도시거래 완료
            city.merchant_done = true;
            actor.action_done = true;
           
            // 舌战胜利后更改市价 설전 승리한 경우 시세 변경
            if (debate_result == 0)
            {
                if (answer == 0)
                {
                    city.rate = city.rate + 10;
                    // city.public_order = city.public_order - 治安减少;
                    ch::add_public_order(city, -治安减少, true);
                    pk::message_box(pk::encode("农民和粮商们纷纷表示不满.治安下降."), taishu);
                    pk::say(pk::encode("就决定降低粮食的价格了."), actor, city);
                }
                else if (answer == 1)
                {
                    city.rate = city.rate - 10;
                    // city.public_order = city.public_order - 治安减少;
                    ch::add_public_order(city, -治安减少, true);
                    pk::message_box(pk::encode("百姓们纷纷表示不满.治安下降."), taishu);
                    pk::say(pk::encode("就决定提高粮食的价格了."), actor, city);
                }
				//????致?退 pk::add_stat_exp(actor, 武将能力_智力, 10);  //获取的智力经验
            }
            else
            {
                pk::say(pk::encode("对不起.讨价还价失败了."), actor, city);
            }
            
			return true;
		}
        
        
        void scene_시세흥정()
		{
			debate_result = pk::debate(actor, merchant, pk::is_player_controlled(actor), false, true, true).first;/*发起武将，应战武将，发起方控制与否，应战方控制与否，是否获得经验，是否观看*/
		}
        
        

	}

	Main main;
}
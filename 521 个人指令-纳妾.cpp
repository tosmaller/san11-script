// ## 2021/03/10 # 白马叔叔 # 修正妾室数量上限判定、无军师的情况，添加固定CP，修正下标越界 ##
// ## 2021/01/27 # 白马叔叔 # 增加妾室数量上限判定 ##
// ## 2020/12/22 # messi # 纠正错字 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ##2020/09/16 # 江东新风 # 使用条件对君主行动状态进行判定 ##
// ##2020/08/08 # 氕氘氚 ##
// ##2020/07/30 # 江东新风 # 修正納妾君主不在可执行的bug ##
/**
원본 제작자 : HoneyBee
수정자 : 바바부부
설명 : 특정 무장을 군주의 배우자로 삼을 수 있음. 양자 스크립트 개조

※ 배우자로 삼을 수 있는 무장의 조건
- 혈연관계가 아닌 무장
- 의형제가 아닌 무장
- 부부가 아닌 무장
- 배우자가 존재하지 않는 무장
- 혐오관계가 아닌 무장
*/

namespace 중혼
{	
	// ================ CUSTOMIZE ================		
		const int ACTION_COST = 0;     	// (행동력 필요량: 기본 50 설정)	
		const int TP_COST = 400;    	   	// (기교P 필요량: 기본 100 설정)		
		const int KEY = pk::hash("柱焚");
		const int 王身份上限 = 5;            //王身份妾的上限
		const int 公身份上限 = 4;            //公身份妾的上限
		const int 其他身份上限 = 3;         //公以下身份妾的上限
	// ===========================================
	
		array<int> couple_m = {660, 289, 370, 245, 347, 225, 635, 568, 602, 395};    //CP男ID
		array<int> couple_w = {424, 165, 387, 266, 300, 418, 375, 248, 371, 505};    //CP女ID
		
		//CP男名字
		array<string> couple_m_name = { "吕布", "诸葛亮", "孙策", "周瑜", "曹丕", "司马懿", "刘备", "孟获", "陆逊", "赵云"};
		//CP女名字
		array<string> couple_w_name = { "貂蝉", "黄月英", "大乔", "小乔", "甄氏", "张春华", "孙尚香", "祝融", "孙氏", "马云騄"};
	
	class Main
	{		
		pk::building@ building_;
		pk::building@ kunshu_building;
		pk::force@ force_;
		pk::person@ kunshu_;
		pk::person@ gunshi_;
		pk::person@ foster_spouse;
		pk::person@ support;
		pk::person@ cp_;
		pk::city@ kunshu_city;
		pk::city@ city_;
		string kunshu_name;
		string spouse_name;
		string cp_name;
		pk::list<pk::person@> person_list;
		array<pk::person@> person_sel_arr;
		int spouse_cp;

		Main()
		{	
			pk::menu_item item;
			item.menu = global_menu::菜单_个人;
			//ch::u8debug(pk::format("menu: na qie {}", global_menu::菜单_个人));
			item.shortcut = "2";
			item.init = pk::building_menu_item_init_t(init);
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
			@kunshu_building = pk::get_building(kunshu_.service);
			@kunshu_city = pk::building_to_city(kunshu_building);
			@city_ = pk::building_to_city(building);
			kunshu_name = pk::decode(pk::get_name(kunshu_));
		}

		bool isEnabled()
		{		
			if (building_.get_id() != kunshu_.service)
				return false;
	        else if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_) or kunshu_.action_done)
				return false;
			else if (force_.tp < TP_COST)
				return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return false;
			else if (!naqie(@ kunshu_))
				return false;
			else if (getTargetList().count == 0)
				return false;
			return true;
		}

		string getText()
		{
			return pk::encode("纳妾");
		}

		string getDesc()
		{
			if (building_.get_id() != kunshu_.service)
				return pk::encode("只能在君主所在城市或港关实行.");
            else if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_) or kunshu_.action_done)
               return pk::encode("君主已行动或不在城市中.");
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须{}行动力)", ACTION_COST));
			else if (!naqie(@ kunshu_))
				return pk::encode("无法纳妾.（当前爵位的妾室数达到上限）");
			else if (getTargetList().count == 0)
				return pk::encode("无法纳妾.（没有适合的人选）");
			else
				return pk::encode(pk::format("执行纳妾.(技巧P {})", TP_COST));
		}

		bool handler()
		{
			if (pk::choose({ pk::encode(" 是 "), pk::encode(" 否 ") }, pk::encode(pk::format("是否要纳妾?\n(使用{}技巧P)", TP_COST)), kunshu_ ) == 1)
			{
				return false;
			}
			
			bool person_confirm = false; 
			
			person_list = getTargetList();
            
            while (!person_confirm)
            {
                // 무장 선택 창을 열어서 선택하기
                person_sel_arr = pk::list_to_array(pk::person_selector( pk::encode("选择武将"), pk::encode("请选择要结婚的对象."), person_list, 1, 1) );
                
                // 무장 선택
                @foster_spouse = person_sel_arr[0];
                
                // 선택 취소 시 종료
                if (!pk::is_alive(foster_spouse))
				{
					return false;
				}
                
                // 선택된 무장의 이름
                spouse_name = pk::decode(pk::get_name(foster_spouse));
                
                person_confirm = pk::yes_no(pk::encode(pk::format("确定选择\x1b[1x{}\x1b[0x作为配偶吗?", spouse_name)));
            }
			int foster_spouse_0 = foster_spouse.get_id();
			spouse_cp = spouseCP(@foster_spouse);
			//pk::trace(pk::format("CP武将ID是:{}",spouse_cp));
			
			if (spouse_cp == -1)
			{
				pk::scene(pk::scene_t(scene_Event_0));  //无官配CP的情况
				//배우자 설정
				foster_spouse.spouse = force_.kunshu;
				foster_spouse.update();
			}
			else
				pk::scene(pk::scene_t(scene_Event_1)); //有官配CP的情况

			//pk::scene(pk::scene_t(scene_Event));
			
			//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x已成为配偶.", spouse_name)));

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);
			
			// 기교P 감소
			ch::add_tp(force_, -TP_COST, force_.get_pos());
			
			kunshu_.action_done = true;
			
			return true;
		}
		
		void scene_Event_0()
        {
			@support = pk::get_person(무장_문관);  //默认是文官
			if (force_.gunshi != -1)
			{
				if (foster_spouse.get_id() != force_.gunshi)
				{
					@gunshi_ = pk::get_person(force_.gunshi);
					@support = pk::is_alive(gunshi_) ? gunshi_ : pk::get_person(무장_문관);
				}
			}
			pk::diplomacy(kunshu_, support, foster_spouse, pk::diplomacy_t(scene_Event_2));
		}
		
		void scene_Event_1()
        {
			@support = pk::get_person(무장_문관);
			@cp_ = pk::get_person(spouse_cp);   //官配CP武将
			pk::diplomacy(kunshu_, cp_, foster_spouse, pk::diplomacy_t(scene_Event_3));
			
			array<string> answers_0 =
			{
				pk::encode(" 让步 "),
				pk::encode("不让步")
			};	
			int answer_ = pk::choose(pk::encode("是否让步成全二人?"), answers_0);
			
			if (answer_ == 0)  //选择让步
			{
				scene_kekon_1();
				if (!pk::is_like(foster_spouse, kunshu_.get_id()))    //选择武将非亲爱君主的情况，有无空位无所谓
				{
					pk::add_like(foster_spouse, kunshu_.get_id());    //添加亲爱
					pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x成为了\x1b[1x{}\x1b[0x的亲爱武将.", kunshu_name, spouse_name)));
				}
				if (pk::is_dislike(cp_, kunshu_.get_id()))    //CP武将厌恶君主的情况，不再厌恶
				{
					pk::remove_dislike(cp_, kunshu_.get_id());   //移除厌恶
					pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x不再厌恶\x1b[1x{}\x1b[0x了.", cp_name, kunshu_name)));
				}
				else if (!pk::is_like(cp_, kunshu_.get_id()))    //CP武将非亲爱君主的情况，有空位就亲爱君主
				{
					pk::add_like(cp_, kunshu_.get_id());    //添加亲爱
					pk::history_log(building_.pos, force_.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x成为了\x1b[1x{}\x1b[0x的亲爱武将.", kunshu_name, cp_name)));
				}
			}
			else   //不让步
			{
				pk::message_box(pk::encode("即便如此,我也无法拱手相让!"), kunshu_);
				pk::message_box(pk::encode(pk::format("那我们便当着\x1b[1x{}\x1b[0x的面,\n展示一下彼此,如何?", spouse_name)), cp_);
				int duel_result = 0;
				if (kunshu_.stat[1] <= cp_.stat[1])   //君主武力低
				{
					pk::message_box(pk::encode("很好!\n那这次就以\x1b[1x单挑\x1b[0x的方式来一决高下吧."), kunshu_);
					duel_result = pk::duel( null, null, kunshu_, null, null, cp_, null, null, pk::is_player_controlled(kunshu_), false, 0, true).first;
				}
				
				else if (kunshu_.stat[2] <= cp_.stat[2])   //君主智力低
				{
					pk::message_box(pk::encode("很好!\n那这次就以\x1b[1x舌战\x1b[0x的方式来一决高下吧."), kunshu_);
					duel_result = pk::debate(kunshu_, cp_, pk::is_player_controlled(kunshu_), false, false, true).first;
				}
				
				else  //都高
				{
					pk::message_box(pk::encode("很好!\n那这次就以\x1b[1x猜「骑戟枪」\x1b[0x的游戏来一决高下吧."), kunshu_);
					pk::message_box(pk::encode("就由我来说明\x1b[1x猜「骑戟枪」\x1b[0x的游戏规则.\n二人每轮同时各出\x1b[1x骑戟枪\x1b[0x中的一项."), support);
					pk::message_box(pk::encode("依据「骑克戟」「戟克枪」「枪克骑」之规则判胜负.\n所出相同则判平.最终以三局两胜!"), support);
					int kunshu_wins = 0;  //君主胜场数
					int cp_wins = 0;  //CP武将胜场数
					bool won = false;  //是否决出胜负初始化
					while (!won)
					{
						array<string> jyanken_1 =
						{
							pk::encode(" 骑 "),
							pk::encode(" 戟 "),
							pk::encode(" 枪 ")
						};	
						int rd_n = pk::choose(pk::encode("这次出哪个呢?"), jyanken_1);
						int rd_m = pk::rand(3);
						int jyanken_result = jyanken(rd_n, rd_m);
						if (jyanken_result == -1)
						{
							pk::message_box(pk::encode(pk::format("主公此轮所出为\x1b[1x{}\x1b[0x,\n\x1b[1x{}\x1b[0x大人此轮所出为\x1b[1x{}\x1b[0x,\n此轮{}大人胜.",getjyankenName (rd_n), cp_name, getjyankenName (rd_m), cp_name)), support);
							cp_wins += 1;  //失败则CP胜场+1
						}
						else if (jyanken_result == 1)
						{
							pk::message_box(pk::encode(pk::format("主公此轮所出为\x1b[1x{}\x1b[0x,\n\x1b[1x{}\x1b[0x大人此轮所出为\x1b[1x{}\x1b[0x,\n此轮主公胜.",getjyankenName (rd_n), cp_name, getjyankenName (rd_m))), support);
							kunshu_wins += 1;  //君主胜场+1
						}
						else
							pk::message_box(pk::encode(pk::format("主公此轮所出为\x1b[1x{}\x1b[0x,\n\x1b[1x{}\x1b[0x大人此轮所出为\x1b[1x{}\x1b[0x,\n此轮平.",getjyankenName (rd_n), cp_name, getjyankenName (rd_m))), support);
						if (kunshu_wins >= 2)
						{
							duel_result = 0;
							pk::message_box(pk::encode("主公最终胜出!"), support);
							won = true;
						}
						else if (cp_wins >= 2)
						{
							duel_result = 1;
							pk::message_box(pk::encode(pk::format("{}大人最终胜出!", cp_name)), support);
							won = true;
						}
					}
				}
				
				if (duel_result == 0)  //君主胜利
				{
					pk::message_box(pk::encode("唔..唔..."), cp_);
					pk::cutin(CG_激昂);
					pk::message_box(pk::encode("这下你无话可说了吧!"), kunshu_);
					pk::message_box(pk::encode("..."), cp_);
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x...", cp_name)), foster_spouse);
					scene_kekon_0();
				}
				else
				{
					pk::message_box(pk::encode("唔..唔...\n（只好做顺水人情了...）"), kunshu_);
					scene_kekon_1();
				}
			}
		}
		
		
		/* void scene_Event()
        {
			int foster_spouse_0 = foster_spouse.get_id();
			@support = pk::get_person(무장_문관);  //默认是文官
			if (force_.gunshi != -1)  
			{
				if (foster_spouse.get_id() != force_.gunshi)
				{
					@gunshi_ = pk::get_person(force_.gunshi);
					@support = pk::is_alive(gunshi_) ? gunshi_ : pk::get_person(무장_문관);
				}
			}
			int spouse_cp = spouseCP(foster_spouse_0);
			if (spouse_cp == -1)  //无官配CP的情况
				pk::diplomacy(kunshu_, support, foster_spouse, pk::diplomacy_t(scene_Event_1));
			else  //有官配CP的情况
			{
				@cp_ = pk::get_person(spouseCP(foster_spouse.get_id()));   //官配CP武将
				pk::diplomacy(kunshu_, cp_, foster_spouse, pk::diplomacy_t(scene_Event_2));
			}
			
			//pk::move_screen(kunshu_city.get_pos(), 2000); 본거지로 화면 이동
		} */
		
		//无官配CP的情况
        void scene_Event_2()
        {
			pk::message_box(pk::encode("主公,是您叫我么?"), foster_spouse);
			
			if (foster_spouse.sex == 성별_남)
			{
				pk::message_box(pk::encode(pk::format("哈哈!\x1b[1x{}\x1b[0x终于来了.", spouse_name)), kunshu_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,\n我希望阁下能成为我的夫君.", spouse_name)), kunshu_);
			}
			else if (foster_spouse.sex == 성별_여)
			{
				pk::message_box(pk::encode(pk::format("唔!\x1b[1x{}\x1b[0x,终与来了么?", spouse_name)), kunshu_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,\n我希望卿能成为我的妻子.", spouse_name)), kunshu_);
			}
			
			pk::message_box(pk::encode("像我这样普通的人能有此荣幸!真像做梦一样呀..."), foster_spouse);
			pk::message_box(pk::encode("以后我会更加努力的侍奉主公."), foster_spouse);
			
			if (foster_spouse.sex == 성별_남)
			{
				pk::message_box(pk::encode("哈哈哈!"), kunshu_);
				pk::message_box(pk::encode("真是愉快的一天!\n抓紧准备婚礼吧."), kunshu_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,\n恭喜阁下成为主公的夫君.", spouse_name)), support);
			}
			else if (foster_spouse.sex == 성별_여)
			{
				pk::message_box(pk::encode("哈哈哈!"), kunshu_);
				pk::message_box(pk::encode("谢谢卿能同意成为我的妻子!\n那么快准备婚礼吧."), kunshu_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,\n恭喜阁下成为主公的妻子.", spouse_name)), support);
			}
        }
		
		//有官配CP的情况
		void scene_Event_3()
        {
			cp_name = pk::decode(pk::get_name(cp_));
			pk::message_box(pk::encode("主公,是您叫我么?"), foster_spouse);
			
			if (foster_spouse.sex == 성별_남)
			{
				pk::message_box(pk::encode(pk::format("哈哈!\x1b[1x{}\x1b[0x终于来了.", spouse_name)), kunshu_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,\n我希望阁下能侍奉于我.", spouse_name)), kunshu_);
				pk::message_box(pk::encode("如此...\n可我..."), foster_spouse);
				pk::message_box(pk::encode("请容许辞退."), foster_spouse);
				pk::message_box(pk::encode("在下对主公,仅是部下侍奉主君耳,\n并无其他非分之想."), foster_spouse);
				pk::message_box(pk::encode(pk::format("我只钟情于\x1b[1x{}\x1b[0x一人.", cp_name)), foster_spouse);
				pk::message_box(pk::encode(pk::format("事实上,\x1b[1x{}\x1b[0x与我情投意合.", spouse_name)), cp_);
				pk::message_box(pk::encode(pk::format("无论如何,请主公成全.\n成全我与\x1b[1x{}\x1b[0x成亲!", cp_name)), foster_spouse);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x...", spouse_name)), cp_);
				pk::message_box(pk::encode("望主公成全!"), cp_);
			}
			else if (foster_spouse.sex == 성별_여)
			{
				pk::message_box(pk::encode(pk::format("唔!\x1b[1x{}\x1b[0x,终与来了么?", spouse_name)), kunshu_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,\n我希望卿能成为我的妾室.", spouse_name)), kunshu_);
				pk::message_box(pk::encode("怎么会?!...可我..."), foster_spouse);
				pk::message_box(pk::encode("请先容我细陈."), foster_spouse);
				pk::message_box(pk::encode(pk::format("承蒙主公厚爱,\n然而\x1b[1x{}\x1b[0x并无非分之想.", spouse_name)), foster_spouse);
				pk::message_box(pk::encode(pk::format("其实我的心早已许给了\x1b[1x{}\x1b[0x.", cp_name)), foster_spouse);
				pk::message_box(pk::encode(pk::format("事实上,\x1b[1x{}\x1b[0x与我情投意合.", spouse_name)), cp_);
				pk::message_box(pk::encode(pk::format("无论如何,请主公成全.\n成全我与\x1b[1x{}\x1b[0x成亲!", spouse_name)), cp_);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x...", cp_name)), foster_spouse);
				pk::message_box(pk::encode("望主公成全!"), foster_spouse);
			}
			
        }
		
		//君主不让步结婚场景
		void scene_kekon_0()
		{
			pk::message_box(pk::encode("哈哈哈!"), kunshu_);
			pk::message_box(pk::encode("那就立刻进行婚仪吧!"), kunshu_);
			pk::fade(0);
			pk::background(场景_结婚);
			pk::fade(255);
			pk::play_bgm(19);
			pk::message_box(pk::encode("臣等一同,衷心祝福."), support);
			pk::message_box(pk::encode("没有比像这样接受大家的祝福.\n还要令人高兴的事了."), kunshu_);
			pk::message_box(pk::encode("..."), foster_spouse);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,\n今后还请多多指教.", spouse_name)), kunshu_);
			pk::message_box(pk::encode("我...才是..."), foster_spouse);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x进行婚仪,结为夫妻了.", kunshu_name, spouse_name))); 
			
			if (pk::is_like(foster_spouse, kunshu_.get_id()))    //选择武将亲爱君主的情况
				pk::remove_like(foster_spouse, kunshu_.get_id());    //移除亲爱
				
			else if (!pk::is_dislike(foster_spouse, kunshu_.get_id()))    //选择武将非厌恶君主的情况，有无空位无所谓
				pk::add_dislike(foster_spouse, kunshu_.get_id());    //增加厌恶
				
			if (pk::is_like(cp_, kunshu_.get_id()))    //CP武将亲爱君主的情况
				pk::remove_like(cp_, kunshu_.get_id());   //移除亲爱
				
			else if (!pk::is_dislike(cp_, kunshu_.get_id()))    //CP武将非厌恶君主的情况，有无空位无所谓
				pk::add_dislike(cp_, kunshu_.get_id());    //添加厌恶
			
			foster_spouse.spouse = force_.kunshu;
			foster_spouse.update();
		}
		
		//让步赐婚场景
		void scene_kekon_1()
		{
			pk::message_box(pk::encode("既是如此,那我便成全你们吧!"), kunshu_);
			pk::message_box(pk::encode(pk::format("那就立刻进行\x1b[1x{}\x1b[0x与\x1b[1x{}\x1b[0x之婚仪吧!", spouse_name, cp_name)), kunshu_);
			pk::message_box(pk::encode("能遇到您这样开明的主公,\n我二人必将会更加地尽忠效命!"), foster_spouse);
			pk::fade(0);
			pk::background(场景_结婚);
			pk::fade(255);
			pk::play_bgm(19);
			pk::message_box(pk::encode(pk::format("两位真是相配的夫妇啊.\n今后也请夫妇一同,\n助\x1b[1x{}\x1b[0x大人完成霸业.",kunshu_name)), support);
			pk::message_box(pk::encode("感激不尽,\n承蒙促成此良缘,\n无可多言来表达感激之意."), foster_spouse);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,\n今后还请多多指教.", cp_name)), foster_spouse);
			pk::message_box(pk::encode("我才是,这还请多多指教了."), cp_);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x和\x1b[1x{}\x1b[0x进行婚仪,结为夫妻了.", spouse_name, cp_name))); 
			foster_spouse.spouse = cp_.get_id();
			foster_spouse.update();
			cp_.spouse = foster_spouse.get_id();
			cp_.update();
		}
		
		pk::list<pk::person@> getTargetList()
		{
			pk::list<pk::person@> list;
			int force_id = force_.get_id();
			int kunshu_0 = kunshu_.get_id();
			for (int i = 0; i < 무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person))
				{
					if (person.get_force_id() == force_id)
					{
						if
						(
							!pk::is_absent(person)
							and person.sex != kunshu_.sex
							and person.spouse == -1
							and !pk::is_unitize(person)
							and person.mibun != 신분_포로
							and !pk::is_fuufu(person, kunshu_0)
							and !pk::is_gikyoudai(person, kunshu_0)
							and !pk::is_ketsuen(person, kunshu_0)
							and !pk::is_dislike(person, kunshu_0)
						)
						list.add(person);
					}
				}
			}
			return list;
		}
		
		//判定还能不能纳妾
		bool naqie(pk::person@ kunshu_)
		{
			bool cannaqie = false;  //初始化
			if (force_.title == 0)  cannaqie = true;  //皇帝无限制
			else if (force_.title == 1)  //王
			{
				if (qiesnum(@ kunshu_) < 王身份上限)  cannaqie = true;  //超过不能再纳妾
			}
			else if (force_.title == 2)  //公
			{
				if (qiesnum(@ kunshu_) < 公身份上限)  cannaqie = true;  //超过不能再纳妾
			}
			else  //比公低的其他
			{
				if (qiesnum(@ kunshu_) < 其他身份上限)  cannaqie = true;  //超过不能再纳妾
			}
			return cannaqie;
		}
		
		//妾的数量
		int qiesnum(pk::person@ kunshu_)
		{
			int num = 0;  //初始化
			pk::list<pk::person@> list_s = pk::get_person_list(pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般, 身份_在野, 身份_俘虏, 身份_未登场, 身份_未发现));
			for (int l = 0; l < list_s.count; l++)
			{
				pk::person@ person_spouse = list_s[l];
				if (person_spouse.spouse == kunshu_.get_id() && kunshu_.spouse != person_spouse.get_id())  num += 1; //区分正室妻子和妾室
			}
			return num;
		}
		
		//官配组合
		int spouseCP(pk::person@ foster_spouse)
		{
			int spouse_id = foster_spouse.get_id();
			int cp_id = -1;  //纳妾的固定CP武将ID初始化，默认无
			if (foster_spouse.sex == 0)  //男
			{
				for (int i = 0; i < int(couple_m.length); i++)
				{
					if (couple_m[i] == spouse_id && couple_m_name[i] == pk::decode(pk::get_name(foster_spouse)))  //选择武将存在于数组中，并且名字对得上
					{
						pk::person@ person_cp = pk::get_person(couple_w[i]);
						//pk::trace(pk::format("cp为{}", couple_w[i]));
						//同一个势力，名字对的上，未婚，身份是都督太守一般。有CP为真
						if (person_cp.get_force_id() == foster_spouse.get_force_id() && couple_w_name[i] == pk::decode(pk::get_name(person_cp)) && person_cp.spouse == -1 && (person_cp.mibun > 身份_君主 and person_cp.mibun < 身份_俘虏))
						{
							cp_id = couple_w[i];   //提取纳妾男武将的CP女武将ID
							break;
						}
					}
				}
			}
			else
			{
				for (int i = 0; i < int(couple_w.length); i++)
				{
					if (couple_w[i] == spouse_id && couple_w_name[i] == pk::decode(pk::get_name(foster_spouse)))  //选择武将存在于数组中，并且名字对得上
					{
						pk::person@ person_cp = pk::get_person(couple_m[i]);
						//pk::trace(pk::format("cp为{}", couple_m[i]));
						//同一个势力，名字对的上，未婚，身份是都督太守一般。有CP为真
						if (person_cp.get_force_id() == foster_spouse.get_force_id() && couple_m_name[i] == pk::decode(pk::get_name(person_cp)) && person_cp.spouse == -1 && (person_cp.mibun > 身份_君主 and person_cp.mibun < 身份_俘虏))
						{
							cp_id = couple_m[i];   //提取纳妾女武将的CP男武将ID
							break;
						}
					}
				}
			}
			return cp_id;
		}
		
		//猜拳判胜负, 0克1,1克2,2克0
		int jyanken (int rd_n, int rd_m)
		{
			//-1为负，0为平，1为胜
			if (rd_n == rd_m)
				return 0;
			else if (rd_n == (rd_m - 1) or rd_n == (rd_m + 2))
				return 1;
			else
				return -1;
		}
		
		//猜拳的名字
		string getjyankenName (int rd_)
		{
			string jyankenName;
			if (rd_ == 0)
				jyankenName = pk::encode("骑");
			else if (rd_ == 1)
				jyankenName = pk::encode("戟");
			else
				jyankenName = pk::encode("枪");
			return jyankenName;
		}
		
	}

	Main main;
}
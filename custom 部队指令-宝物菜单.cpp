// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/03/02 # 江东新风 # 将特殊地名重建的改写，cpp里调用kill_building都需要这样设定 ##
// ## 2021/01/05 # 江东新风 # 为宝物指令添加特殊地名争夺 ##
// ## 2020/12/24 # 江东新风 # 修复trace参数报错 ##
// ## 2020/11/12 # messi # 删掉使用条件中的白象和役兽部队2##
// ## 2020/10/31 # 江东新风 # 同步honeybee大神的更新，无法施展任意宝物技时隐藏##
/*
@제작자: HoneyBee
@설명: 특정 보물을 소유한 경우 고유한 효과를 발동하는 부대 메뉴
	 (적군의 병력 및 기력, 건물의 내구를 감소시키는 효과는
	  적군 부대의 병력 500 이하, 기력이 10 이하, 건물의 내구 100이하인 경우에는 효과가 적용되지 않음
	  해당 제약 조건은 CUSTOMIZE에서 취향에 맞게 설정하도록 하세요.)


== 효과 설명 ==
1. 광풍맹수 - 맹수부대 ITEM 소유한 부대가 사용할 수 있는 부대메뉴
		   (적군병력,기력감소 / 일정확률로 아군도 기력 및 병력피해)

2. 전상광돌 - 백상 or 코끼리병 ITEM 소유한 부대가 사용할 수 있는 부대메뉴
		   (적군병력,건물피해 / 일정확률로 아군도 건물 및 병력피해)

3. 태평요술 - 태평요술서 ITEM 소유한 부대가 사용할 수 있는 부대메뉴
		   (적군병력 or 적군병량 특정수치로 변환 / 일정확률로 아군도 병력 or 병량 특정수치로 변환)
@说明:拥有特定宝物时，发动特殊效果的部队菜单
(敌人的兵力和气力，减少建筑物耐久的效果，
敌人部队的兵力在500以下，气力在10以下，建筑物耐久在100以下时，不适用效果。
这个限制，请在CUSTOMIZE上按照喜好设定。)

==说明效果==

1.狂风猛兽-猛兽部队宝物持有的部队可以使用的部队菜单
(敌军兵力，气力下降/一定概率我方产生气力和兵力的损失)

2.正相官石-白象or大象兵宝物持有的部队可以使用的部队菜单
(敌军兵力，建筑物受害/一定概率我方产生建筑物或兵力的受害)

3.太平道术-太平道术中宝物持有的部队可以使用的部队菜单

(敌军兵力or敌军兵粮变换成特定数值/以一定概率我方部队兵力or兵粮也变换成特定数值)


*/

namespace 部队指令_宝物菜单
{

	// ================ CUSTOMIZE ================


	const bool 보물효과메뉴_사용 = true; 							// 보물효과메뉴_사용 설정 (true = on / false = off)

	// 계략에 대한 특기 옵션
	const bool 계략_특기_설정 = true;           				// 계략에 대한 특기 설정 사용
	const bool 백출_반감_설정 = true;           				// true= 50% 소비, false = 무조건 1 소비
	const bool 계략_병력_차등 = true;           				// 병력수에 따른 계략 소비기력 차등 적용

	const int energy_제약조건 = 10;  							// 기력 10 이하 적군 부대는 기력 감소 효과가 적용되지 않음 
	const int troops_제약조건 = 500; 							// 병력 500 이하 적군 부대는 병력 감소 효과가 적용되지 않음
	const int building_hp_제약조건 = 100; 						// 내구 100 이하 적군 건물은 내구 감소 효과가 적용되지 않음	

	// 맹수부대 아이템을 소유한 경우 사용 가능
	pk::item@ item_맹수부대1 = pk::get_item(66); 				// 맹수부대1 ITEM NUMBER를 입력
	pk::item@ item_맹수부대2 = pk::get_item(96); 				// 맹수부대2 ITEM NUMBER를 입력
	const bool 광풍맹수_사용 = true; 							// 광풍맹수_사용 설정 (true = on / false = off)
	const int ENERGY_COST_광풍맹수 = 40;       				// 광풍맹수 기력 최소 필요량
	const array<int> 적군병력감소_광풍맹수 = { 700, 800, 900 };  	 	// 광풍맹수로 인한 적군병력 감소
	const int 적군사기감소_광풍맹수 = 20;     						// 광풍맹수로 인한 적군사기 감소
	const int 아군피해확률_광풍맹수 = 30;                       	// 광풍맹수를 사용 시 주위에 존재하는 아군도 피해받을 확률
	const int 아군사기감소_광풍맹수 = 10;     						// 광풍맹수로 인한 아군사기 감소
	const array<int> 아군병력감소_광풍맹수 = { 300, 400, 500 };   	// 광풍맹수로 인한 아군병력 감소
	const int min_범위_광풍맹수 = 1;      						// 효과를 받는 최소 범위 
	const int max_범위_광풍맹수 = 2; 							// 효과를 받는 최대 범위 

	// 백상 or 코끼리병 아이템을 소유한 경우 사용 가능
	pk::item@ item_백상 = pk::get_item(64); 					// 백상 ITEM NUMBER를 입력
	pk::item@ item_巨象兵兵符 = pk::get_item(91); 				// 코끼리병 ITEM NUMBER를 입력
	const bool 전상광돌_사용 = true; 							// 전상광돌_사용 설정 (true = on / false = off)
	const int ENERGY_COST_전상광돌 = 40;       				// 전상광돌 기력 최소 필요량
	const array<int> 적군병력감소_전상광돌 = { 600, 700, 800 };   	// 전상광돌로 인한 적군병력 감소
	const array<int> 적군건물피해_전상광돌 = { 500, 600, 700 };   	// 전상광돌로 인한 적군건물 내구피해
	const int 아군피해확률_전상광돌 = 30;                       	// 전상광돌을 사용 시 주위에 존재하는 아군도 피해받을 확률
	const array<int> 아군병력감소_전상광돌 = { 300, 400, 500 };   	// 전상광돌로 인한 아군병력 감소
	const array<int> 아군건물피해_전상광돌 = { 500, 600, 700 };   	// 전상광돌로 인한 아군건물 내구피해
	const int min_범위_전상광돌 = 1;      						// 효과를 받는 최소 범위 
	const int max_범위_전상광돌 = 2; 							// 효과를 받는 최대 범위 

	// 태평요술서 아이템을 소유한 경우 사용 가능
	pk::item@ item_태평요술서 = pk::get_item(보물_태평요술서); 	    // 태평요술서 ITEM NUMBER를 입력
	const bool 태평요술_사용 = true; 						    // 태평요술_사용 설정 (true = on / false = off)
	const int ENERGY_COST_태평요술 = 40;       				// 태평요술 기력 최소 필요량
	const int 적군병력변환_태평요술 = 3000;    						// 태평요술로 인한 적군병력 변환
	const int 적군병량변환_태평요술 = 3000;    						// 태평요술로 인한 적군병량 변환
	const int 아군피해확률_태평요술 = 30;                           // 태평요술을 사용 시 주위에 존재하는 아군도 피해받을 확률
	const int 아군병력변환_태평요술 = 3000;    						// 태평요술로 인한 아군병력 변환
	const int 아군병량변환_태평요술 = 3000;    						// 태평요술로 인한 아군병량 변환
	const int min_범위_태평요술 = 1;      					    // 효과를 받는 최소 범위 
	const int max_범위_태평요술 = 2; 						    // 효과를 받는 최대 범위 

	const int 병력제한 = 5000;                                 // 병력이 5000 미만이면 사용 불가 

// ===========================================


	class Main
	{
		Main()
		{
			// 부대 메뉴 추가
			add_menu_unit_order();
		}

		pk::force@ force;
		pk::unit@ src_unit;
		pk::unit@ dst_unit;
		pk::point src_pos_;
		pk::person@ src_leader;
		pk::person@ dst_leader;
		pk::damage_info info;
		pk::building@ dst_building;

		bool flag_광풍맹수 = false;
		bool flag_전상광돌 = false;
		bool flag_전상광돌_부대 = false;
		bool flag_전상광돌_건물 = false;
		bool flag_태평요술 = false;

		void add_menu_unit_order()
		{
			if (보물효과메뉴_사용)
			{
				pk::menu_item 부대;
				int 부대_상위;

				부대.menu = 1;
				부대.init = pk::unit_menu_item_init_t(init);
				부대.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("宝物技"); });
				부대.is_visible = pk::menu_item_is_visible_t(isVisible_부대);
				부대_상위 = pk::add_menu_item(부대);

				// 광풍맹수
				if (광풍맹수_사용)
				{
					pk::menu_item 광풍맹수;
					광풍맹수.menu = 부대_상위;
					광풍맹수.init = pk::unit_menu_item_init_t(init);
					광풍맹수.get_text = pk::menu_item_get_text_t(getText_광풍맹수);
					광풍맹수.get_desc = pk::menu_item_get_desc_t(getDesc_광풍맹수);
					광풍맹수.is_enabled = pk::menu_item_is_enabled_t(isEnabled_광풍맹수);
					광풍맹수.handler = pk::unit_menu_item_handler_t(handler_광풍맹수);
					pk::add_menu_item(광풍맹수);
				}

				// 전상광돌
				if (전상광돌_사용)
				{
					pk::menu_item 전상광돌;
					전상광돌.menu = 부대_상위;
					전상광돌.init = pk::unit_menu_item_init_t(init);
					전상광돌.get_text = pk::menu_item_get_text_t(getText_전상광돌);
					전상광돌.get_desc = pk::menu_item_get_desc_t(getDesc_전상광돌);
					전상광돌.is_enabled = pk::menu_item_is_enabled_t(isEnabled_전상광돌);
					전상광돌.handler = pk::unit_menu_item_handler_t(handler_전상광돌);
					pk::add_menu_item(전상광돌);
				}

				// 태평요술
				if (태평요술_사용)
				{
					pk::menu_item 태평요술;
					태평요술.menu = 부대_상위;
					태평요술.init = pk::unit_menu_item_init_t(init);
					태평요술.get_text = pk::menu_item_get_text_t(getText_태평요술);
					태평요술.get_desc = pk::menu_item_get_desc_t(getDesc_태평요술);
					태평요술.is_enabled = pk::menu_item_is_enabled_t(isEnabled_태평요술);
					태평요술.handler = pk::unit_menu_item_handler_t(handler_태평요술);
					pk::add_menu_item(태평요술);
				}

			}

		} // add_menu_unit_order

		void init(pk::unit@ unit, pk::point src_pos)
		{
			@src_unit = @unit;
			src_pos_ = src_pos;
			@src_leader = pk::get_person(src_unit.leader);
			@force = pk::get_force(src_unit.get_force_id());
		}


		//---------------------------------------------------------------------------
		// 계략 설정 관련 함수
		//---------------------------------------------------------------------------
		uint8 func_stragety_cost(int energy_cost)
		{
			if (!계략_특기_설정) return energy_cost;

			uint8 n = uint8(energy_cost);
			if (src_unit.has_skill(특기_백출))
			{
				if (백출_반감_설정)
					n /= 2; // 기력 소모량 절반 감소
				else
					n = 1;  // 기력 소모량 무조건 1로
			}

			//병력수에 따른 기력 소모량 차등 - 소부대 기력 소모 증가, 대부대 기력 소모 감소
			if (계략_병력_차등)
			{
				if (src_unit.troops <= 2000)
					n = uint8(n * pk::min(10.f, 2000.f / src_unit.troops));         // 최대 10배 소모
				else if (src_unit.troops >= 10000)
					n = uint8(n * pk::max(0.5f, (2 - src_unit.troops / 10000.f)));    // 최소 50% 소모
			}

			return n;
		}

		bool isVisible_부대()
		{
			if (pk::is_campaign()) return false;
			if ((item_맹수부대1.owner == src_leader.get_id()) or (item_맹수부대2.owner == src_leader.get_id())) return true;
			if ((item_백상.owner == src_leader.get_id()) or (item_巨象兵兵符.owner == src_leader.get_id())) return true;
			if ((item_태평요술서.owner == src_leader.get_id())) return true;
			return false;
		}

		//---------------------------------------------------------------------------
		// 부대소환메뉴 : 광풍맹수 
		//---------------------------------------------------------------------------



		string getText_광풍맹수()
		{
			return pk::encode(pk::format("狂风猛兽 ({})", func_stragety_cost(ENERGY_COST_광풍맹수)));
		}

		string getDesc_광풍맹수()
		{
			if (src_unit.energy < func_stragety_cost(ENERGY_COST_광풍맹수))
				return pk::encode("部队气力不足.");
			else if ((item_맹수부대1.owner != src_leader.get_id()) and (item_맹수부대2.owner != src_leader.get_id()))
				return pk::encode("未拥有宝物-猛兽部队兵符.");
			else if (src_unit.troops < 병력제한)
				return pk::encode("兵力不足.");
			else
				return pk::encode(pk::format("使用狂风猛兽.(需要至少{}气力)", func_stragety_cost(ENERGY_COST_광풍맹수)));
		}

		bool isVisible_광풍맹수()
		{
			return true;
		}

		bool isEnabled_광풍맹수()
		{
			if (src_unit.energy < func_stragety_cost(ENERGY_COST_광풍맹수)) return false;
			else if ((item_맹수부대1.owner != src_leader.get_id())) return false;
			else if (src_unit.troops < 병력제한) return false;
			return true;
		}

		bool handler_광풍맹수(pk::point dst_pos)
		{
			array<pk::point> arr = pk::range(src_pos_, min_범위_광풍맹수, max_범위_광풍맹수);

			for (int j = 0; j < int(arr.length); j++)
			{
				@dst_unit = pk::get_unit(arr[j]);

				if (dst_unit is null) continue;

				@dst_leader = pk::get_person(dst_unit.leader);

				pk::play_voice(0x1C, src_leader);
				pk::create_effect(0x31, src_pos_);
				pk::create_effect(0x24, dst_unit.pos);
				pk::play_se(0x31, dst_unit.pos);

				// 적군인 경우 
				if (pk::is_enemy(src_unit, dst_unit))
				{
					flag_광풍맹수 = true;
					if (troops_제약조건 < dst_unit.troops) {
						ch::add_troops(dst_unit, -적군병력감소_광풍맹수[pk::rand(3)], true, 100);
					};

					if (energy_제약조건 < dst_unit.energy) {
						pk::add_energy(dst_unit, -적군사기감소_광풍맹수, true);
					};

					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode("哇，好多猛兽！"), dst_leader); break;
					case 1: pk::say(pk::encode("呃...快跑吧..!"), dst_leader); break;
					}
				}

				// 아군인 경우 
				if (pk::rand_bool(아군피해확률_광풍맹수) and !pk::is_enemy(src_unit, dst_unit))
				{
					if (troops_제약조건 < dst_unit.troops) {
						ch::add_troops(dst_unit, -아군병력감소_광풍맹수[pk::rand(3)], true, 100);
					};

					if (energy_제약조건 < dst_unit.energy) {
						pk::add_energy(dst_unit, -아군사기감소_광풍맹수, true);
					};

					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode("猛兽失控了..!"), dst_leader); break;
					case 1: pk::say(pk::encode("不好！猛兽开始攻击我方部队了..！"), dst_leader); break;
					}
				}

				// 병력 0 이 되면 부대 궤멸
				if (dst_unit.troops == 0) { pk::kill(dst_unit); }

			} // for 

			if (flag_광풍맹수)
			{
				switch (pk::rand(2))
				{
				case 0: pk::say(pk::encode("让我的猛兽好好饱餐一顿!"), src_leader); break;
				case 1: pk::say(pk::encode("哈哈~ 敌军被吓跑了!"), src_leader); break;
				}
				flag_광풍맹수 = false;
			}

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_광풍맹수), true);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
			{
				src_unit.action_done = false;
			}

			return true;

		} // 광풍맹수 END 

//------------------------------------------------------------------------------------------

		//---------------------------------------------------------------------------
		// 부대소환메뉴 : 전상광돌 
		//---------------------------------------------------------------------------

		string getText_전상광돌()
		{
			return pk::encode(pk::format("巨象冲击 ({})", func_stragety_cost(ENERGY_COST_전상광돌)));
		}

		string getDesc_전상광돌()
		{
			if (src_unit.energy < func_stragety_cost(ENERGY_COST_전상광돌))
				return pk::encode("部队气力不足");
			else if ((item_백상.owner != src_leader.get_id()) and (item_巨象兵兵符.owner != src_leader.get_id()))
				return pk::encode("未拥有宝物-巨象兵兵符");
			else if (src_unit.troops < 병력제한)
				return pk::encode("兵力不足");
			else
				return pk::encode(pk::format("使用巨象冲击.(至少需要{}气力)", func_stragety_cost(ENERGY_COST_전상광돌)));
		}

		bool isVisible_전상광돌()
		{
			return true;
		}

		bool isEnabled_전상광돌()
		{
			if (src_unit.energy < func_stragety_cost(ENERGY_COST_전상광돌)) return false;
			else if ((item_巨象兵兵符.owner != src_leader.get_id())) return false;
			else if (src_unit.troops < 병력제한) return false;
			return true;
		}

		bool handler_전상광돌(pk::point dst_pos)
		{
			array<pk::point> arr = pk::range(src_pos_, min_범위_전상광돌, max_범위_전상광돌);

			for (int j = 0; j < int(arr.length); j++)
			{
				@dst_unit = pk::get_unit(arr[j]);

				pk::play_voice(0x1C, src_leader);
				pk::create_effect(0x4e, src_pos_);

				if (dst_unit !is null) { flag_전상광돌_부대 = true; }

				// 적군인 경우 
				if (flag_전상광돌_부대 and pk::is_enemy(src_unit, dst_unit))
				{
					flag_전상광돌 = true;

					@dst_leader = pk::get_person(dst_unit.leader);

					pk::create_effect(0x24, dst_unit.pos);
					pk::play_se(0x28, dst_unit.pos);

					if (troops_제약조건 < dst_unit.troops) {
						ch::add_troops(dst_unit, -적군병력감소_전상광돌[pk::rand(3)], true, 100);
					};

					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode("这么会有这么大的动物，好可怕..!"), dst_leader); break;
					case 1: pk::say(pk::encode("好恐怖的大象呀!"), dst_leader); break;
					}

					if (dst_unit.troops == 0) { pk::kill(dst_unit); }

					flag_전상광돌_부대 = false;
				}

				// 아군인 경우 
				if (pk::rand_bool(아군피해확률_전상광돌) and flag_전상광돌_부대 and !pk::is_enemy(src_unit, dst_unit))
				{
					flag_전상광돌 = true;

					@dst_leader = pk::get_person(dst_unit.leader);

					pk::create_effect(0x24, dst_unit.pos);
					pk::play_se(0x28, dst_unit.pos);

					if (troops_제약조건 < dst_unit.troops) {
						ch::add_troops(dst_unit, -아군병력감소_전상광돌[pk::rand(3)], true, 100);
					};

					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode("不好，大象攻击友军了..!"), dst_leader); break;
					case 1: pk::say(pk::encode("快撤!"), dst_leader); break;
					}

					if (dst_unit.troops == 0) { pk::kill(dst_unit); }

					flag_전상광돌_부대 = false;
				}

			} // for 

			for (int j = 0; j < int(arr.length); j++)
			{
				@dst_building = pk::get_building(arr[j]);

				pk::play_voice(0x1C, src_leader);
				pk::create_effect(0x4e, src_pos_);

				if (dst_building !is null) { flag_전상광돌_건물 = true; }

				// 적세력 건물인 경우 
				if (flag_전상광돌_건물 and pk::is_enemy(src_unit, dst_building))
				{
					flag_전상광돌 = true;

					pk::create_effect(0x5c, dst_building.pos);

					if (building_hp_제약조건 < dst_building.hp) {
						pk::add_hp(dst_building, -적군건물피해_전상광돌[pk::rand(3)], true);
					};

					if (dst_building.hp == 0)
					{
						auto dst_pos0 = dst_building.pos;
						auto dst_facility = pk::get_facility(dst_building.facility);

						pk::kill(dst_building);
						ch::特殊地名争夺处理(dst_facility.get_id(), src_unit.get_force_id(), dst_pos0);
					}

					flag_전상광돌_건물 = false;
				}

				// 아군세력 건물인 경우 
				if (pk::rand_bool(아군피해확률_전상광돌) and flag_전상광돌_건물 and !pk::is_enemy(src_unit, dst_building))
				{
					flag_전상광돌 = true;

					pk::create_effect(0x5c, dst_building.pos);

					if (building_hp_제약조건 < dst_building.hp) {
						pk::add_hp(dst_building, -아군건물피해_전상광돌[pk::rand(3)], true);
					};

					if (dst_building.hp == 0)
					{
						auto dst_pos0 = dst_building.pos;
						auto dst_facility = pk::get_facility(dst_building.facility);

						pk::kill(dst_building);
						if (特定地点特定设施可争夺)
						{
							if (地名设施.find(dst_facility.get_id()) >= 0)
							{
								for (int i = 0; i < int(特殊地名设施.length); i++)
								{
									pk::point pos0;
									pos0.x = 特殊地名设施[i][1];
									pos0.y = 特殊地名设施[i][2];
									if (pos0 == dst_pos0)
									{
										pk::building@ building0 = pk::create_building(dst_facility, dst_pos0, src_unit.get_force_id());
										pk::complete(building0);
										break;
									}
								}
							}
						}
					}

					flag_전상광돌_건물 = false;
				}

			} // for 

			if (flag_전상광돌)
			{
				switch (pk::rand(2))
				{
				case 0: pk::say(pk::encode("全给我碾碎吧!"), src_leader); break;
				case 1: pk::say(pk::encode("这就是圣象之力!"), src_leader); break;
				}
				flag_전상광돌 = false;
			}

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_전상광돌), true);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
			{
				src_unit.action_done = false;
			}

			return true;

		} // 전상광돌 END 

//------------------------------------------------------------------------------------------

		//---------------------------------------------------------------------------
		// 부대소환메뉴 : 태평요술 
		//---------------------------------------------------------------------------

		string getText_태평요술()
		{
			return pk::encode(pk::format("太平道术 ({})", func_stragety_cost(ENERGY_COST_태평요술)));
		}

		string getDesc_태평요술()
		{
			if (src_unit.energy < func_stragety_cost(ENERGY_COST_태평요술))
				return pk::encode("部队气力不足");
			else if ((item_태평요술서.owner != src_leader.get_id()))
				return pk::encode("未拥有宝物-太平道术.");
			else if (src_unit.troops < 병력제한)
				return pk::encode("兵力不足.");
			else
				return pk::encode(pk::format("使用太平道术.(需要至少{}气力)", func_stragety_cost(ENERGY_COST_태평요술)));
		}

		bool isVisible_태평요술()
		{
			return true;
		}

		bool isEnabled_태평요술()
		{
			if (src_unit.energy < func_stragety_cost(ENERGY_COST_태평요술)) return false;
			else if ((item_태평요술서.owner != src_leader.get_id())) return false;
			else if (src_unit.troops < 병력제한) return false;
			return true;
		}

		bool handler_태평요술(pk::point dst_pos)
		{
			array<pk::point> arr = pk::range(src_pos_, min_범위_태평요술, max_범위_태평요술);

			for (int j = 0; j < int(arr.length); j++)
			{
				@dst_unit = pk::get_unit(arr[j]);

				pk::play_voice(0x1C, src_leader);
				pk::create_effect(0x4e, src_pos_);

				int n = pk::rand(2);

				if (dst_unit is null) continue;

				// 적군인 경우 
				if (pk::is_enemy(src_unit, dst_unit))
				{
					flag_태평요술 = true;

					@dst_leader = pk::get_person(dst_unit.leader);

					pk::create_effect(0x59, dst_unit.pos);
					pk::play_se(0x59, dst_unit.pos);

					if (n == 0)
					{
						if (적군병력변환_태평요술 < dst_unit.troops) {
							pk::set_troops(dst_unit, 적군병력변환_태평요술);
						};
						switch (pk::rand(2))
						{
						case 0: pk::say(pk::encode("我们的士兵都凭空消失了？！"), dst_leader); break;
						case 1: pk::say(pk::encode("这是什么妖法!"), dst_leader); break;
						}
					}
					else if (n == 1)
					{
						if (적군병량변환_태평요술 < dst_unit.food) {
							pk::set_food(dst_unit, 적군병량변환_태평요술);
						};
						switch (pk::rand(2))
						{
						case 0: pk::say(pk::encode("不好，我方的兵粮都消失了..!"), dst_leader); break;
						case 1: pk::say(pk::encode("好诡异的妖法!"), dst_leader); break;
						}
					}
				}

				// 아군인 경우 
				if (pk::rand_bool(아군피해확률_태평요술) and !pk::is_enemy(src_unit, dst_unit))
				{
					@dst_leader = pk::get_person(dst_unit.leader);

					pk::create_effect(0x59, dst_unit.pos);
					pk::play_se(0x59, dst_unit.pos);

					if (n == 0)
					{
						if (아군병력변환_태평요술 < dst_unit.troops) {
							pk::set_troops(dst_unit, 아군병력변환_태평요술);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("不好，我方的士兵也消失了..!"), dst_leader); break;
							case 1: pk::say(pk::encode("仙术失控了..."), dst_leader); break;
							}
						}
					}
					else if (n == 1)
					{
						if (아군병량변환_태평요술 < dst_unit.food) {
							pk::set_food(dst_unit, 아군병량변환_태평요술);

							switch (pk::rand(2))
							{
							case 0: pk::say(pk::encode("不好，我方的兵粮也消失了..!"), dst_leader); break;
							case 1: pk::say(pk::encode("仙术竟然失控了!"), dst_leader); break;
							}
						}
					}
				}

			} // for 

			if (flag_태평요술)
			{
				switch (pk::rand(2))
				{
				case 0: pk::say(pk::encode("哈哈哈~这可是仙家的手段!"), src_leader); break;
				case 1: pk::say(pk::encode("见识下太平道术的威力吧!"), src_leader); break;
				}
				flag_태평요술 = false;
			}

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_태평요술), true);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
			{
				src_unit.action_done = false;
			}

			return true;

		} // 태평요술 END 


	} // class Main

	Main main;

} // namespace
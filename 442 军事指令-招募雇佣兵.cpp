// ## 2021/07/04 # 白马叔叔 # 雇佣可以提升异族关系、雇佣会减少异族兵力、一回合仅雇佣一次、修复雇佣不减钱bug ##
// ## 2021/04/18 # 白马叔叔 # AI雇佣军、优化算法 ##
// ## 2020/12/28 # 江东新风 # 在敲鱼协助下解决委任退却闪退问题 ##
// ## 2020/12/22 # messi # 增加可能造成闪退操作的提示 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/09 # 氕氘氚 # 加入【军事】菜单 ##
// ## 2020/08/05 # 氕氘氚 # 修复雇佣兵被捕閃退問題，禁止雇佣武将入城 ##
/*
@제작자: HoneyBee
@설명: 이민족 특기를 보유한 무장이 소속된 도시에서 이민족을 용병으로 고용 가능 (50% 확률로 고용 가능)

	 -> 이민족 고용에 실패하면 대신에 병량을 획득하고 금은 소비되지 않습니다.

*/

namespace 이민족_용병
{

	// ================ CUSTOMIZE ================

	const bool 강족_활성화 = true;    // true = on, false = off
	const bool 오환족_활성화 = true;   // true = on, false = off
	const bool 산월족_활성화 = true;   // true = on, false = off
	const bool 남만족_활성화 = true;   // true = on, false = off

	const bool 이름자동 = false;	   // 무장 이름을 자동으로 생성	[이름자동 true일시, 이름수동 false로 지정]

	const int TP_COST = 50; 		// 기교P 수치 결정
	const int ACTION_COST = 15;     // 행동력 필요량
	const int GOLD_COST = 3000;     // 금 필요량
	const int ADD_FOOD = 10000;     // 이민족 용병 대신에 획득하는 병량

	const int 고용실패확률 = 10;    		// 이민족 용병 고용이 실패할 확률 (기본 설정 - 50%)

	const int min_stat = 60;        // 이민족 용병의 최소 능력치를 설정 (기본 설정 - 60)
	const int max_stat = 85;        // 이민족 용병의 최대 능력치를 설정 (기본 설정 - 85)

	const int min_distance = 1;      // 용병 부대 생성 최소 위치
	const int max_distance = 6;      // 용병 부대 생성 최대 위치
	const int troops = 10000;		 // 용병 부대 병력 설정 (최대 14000 = 정동장군 관직과 동일)

	const int 临时武将起始 = 780;
	const int 临时武将终止 = 799;
	
	bool AI雇佣开启 = true;          //AI雇佣事件开关。true为开，false为关。
	const int AI雇佣失败概率 = 90;  //AI雇佣事件，不雇佣概率的。默认90
	
	const int 异族关系提升值 = 3;   //玩家每次雇佣可以提升的关系指

	/*

		이민족 무장 이름을 ARRAY로 미리 SETTING 하시기 바랍니다. (아래는 임시로 넣은 성명입니다.)
		아래 성명에서 RANDOM으로 출력하여 이민족 무장의 이름이 정해집니다.

		EX) 조조, 조비, 조견, 유비, 유견, 유탁, 손견, 손탁, 손포 등..

	*/

	// 이민족 무장 [성] (임시로 40개 설정)
	array<string> sei_ = { "乞伏", "秃发", "拓跋", "贺", "独孤", "赫连", "细封", "费听", "野利", "颇超",
						   "呼厨", "米擒", "姜包", "云", "巴", "乌", "陶", "马", "沙", "哈",
						   "沐", "郑", "木", "董" };

	// 이민족 무장 [이름] (임시로 40개로 설정)
	array<string> mei_ = { "香", "火", "白", "风", "梅", "超", "休", "落", "沐", "爱",
						   "羽", "台", "智", "福", "羊", "初", "艾", "孙", "牛", "瑞",
						   "江", "墨", "皮", "印" };
						   
	array<int> banditcity_id = {76, 64, 54, 75, 66};  //西羌（76卢陵） 乌桓（64解县） 山越（54西河） 南蛮（75鄱阳） 鲜卑（贼66夏阳）
	array<int> banditforce_id = {42, 44, 43, 45, 46}; //西羌42 乌丸44 山越43 南蛮45 贼46
	array<int> banditface_id = {681, 679, 683, 685, 749};  //西羌  乌丸  山越  南蛮  贼


	// EX) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 ... 39 } <- TOTAL 40개 (ARRAY에서 [성], [이름]을 늘릴 때마다 동일한 갯수로 설정하시면 됩니다.)
	int i = 0;   // 배열의 FIRST NUMBER 
	int j = 23;  // 배열의 LAST NUMBER




	const int KEY = pk::hash("遭质恢遭");  //용병고용 掩 フ꽥 묘Р 콩愈

// ===========================================


	class Main
	{

		pk::building@ building_;
		pk::building@ taishu_building;
		pk::force@ force;
		pk::person@ taishu;
		pk::city@ city;
<<<<<<< HEAD
		uint16 building_gold;
=======
		int building_gold;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

		// 건물 메뉴 도시
		int menu_city_id_;

		// 건물 메뉴 세력
		int menu_force_id_;

		Main()
		{

			pk::menu_item item;
			int parent;

			item.menu = 101;
			// item.pos = 7;
			item.shortcut = "6";
			item.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("异族雇佣"); });  //yizu guyong
			item.init = pk::building_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
			parent = pk::add_menu_item(item);

			if (강족_활성화)
			{
				item.menu = parent;
				item.shortcut = "1";
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_강족);
				item.get_text = pk::menu_item_get_text_t(getText_강족);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_강족);
				item.handler = pk::menu_item_handler_t(handler_강족);
				pk::add_menu_item(item);
			}

			if (오환족_활성화)
			{
				item.menu = parent;
				item.shortcut = "2";
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_오환족);
				item.get_text = pk::menu_item_get_text_t(getText_오환족);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_오환족);
				item.handler = pk::menu_item_handler_t(handler_오환족);
				pk::add_menu_item(item);
			}

			if (산월족_활성화)
			{
				item.menu = parent;
				item.shortcut = "3";
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_산월족);
				item.get_text = pk::menu_item_get_text_t(getText_산월족);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_산월족);
				item.handler = pk::menu_item_handler_t(handler_산월족);
				pk::add_menu_item(item);
			}

			if (남만족_활성화)
			{
				item.menu = parent;
				item.shortcut = "4";
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_남만족);
				item.get_text = pk::menu_item_get_text_t(getText_남만족);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_남만족);
				item.handler = pk::menu_item_handler_t(handler_남만족);
				pk::add_menu_item(item);
			}
			pk::bind(107, pk::trigger107_t(callback));
			
		}


		int getImageID_1249()   // 주황색
		{
			return 1249;
		}


		void init(pk::building@ building)
		{

			@building_ = @building;
			@force = pk::get_force(building.get_force_id());
			@taishu = pk::get_person(pk::get_taishu_id(building));
			@city = pk::building_to_city(building);
			building_gold = pk::get_gold(building);

			if (pk::is_alive(taishu))
				@taishu_building = pk::get_building(taishu.service);
			
			menu_city_id_ = -1;

			if (city !is null)
				menu_city_id_ = city.get_id();
			menu_force_id_ = building.get_force_id();
			
		}

		bool isMenuVisible()
		{
			if (building_.get_id() >= 据点_末) return false;
			return menu_city_id_ != -1 and menu_force_id_ == pk::get_current_turn_force_id();
		}

		//	各族菜单文字
		string getText_강족()
		{
			return pk::encode("(羌族)雇佣");
		}
		string getText_오환족()
		{
			return pk::encode("(乌丸族)雇佣");
		}
		string getText_산월족()
		{
			return pk::encode("(山越族)雇佣");
		}
		string getText_남만족()
		{
			return pk::encode("(南蛮族)雇佣");
		}

		//	各族	menu_item_get_desc_t	和	isEnabled
		string getDesc_강족()
		{
			if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			else if (!taishu_building.has_skill(특기_친강))
				return pk::encode("城市需拥有亲羌特技的武将.");
			else if (pk::get_troops(pk::get_building(banditcity_id[0])) * 2 < troops)
				return pk::encode("羌族兵力不足, 无法雇佣.");
			else if ((pk::building_to_port(pk::get_building(banditcity_id[0]))).drills_ended)
				return pk::encode("已经进行过雇佣.");   //以异族城的训练完成的值作为判定此回合是否雇佣的参数
			else if (getEmptyPos(taishu_building) == -1)
				return pk::encode("没有空地生成外援.");
			else if (force.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("雇佣羌族.(使用 {} 技巧P)", TP_COST));
		}
		bool isEnabled_강족()
		{
			if (building_gold < GOLD_COST) return false;
			else if (!taishu_building.has_skill(특기_친강)) return false;
			else if (pk::get_troops(pk::get_building(banditcity_id[0])) * 2 < troops) return false;
			else if ((pk::building_to_port(pk::get_building(banditcity_id[0]))).drills_ended) return false;
			else if (getEmptyPos(taishu_building) == -1) return false;
			else if (force.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST) return false;
			return true;
		}


		string getDesc_오환족()
		{
			if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			else if (!taishu_building.has_skill(특기_친오))
				return pk::encode("城市需拥有亲乌特技的武将.");
			else if (pk::get_troops(pk::get_building(banditcity_id[1])) * 2 < troops)
				return pk::encode("乌丸族兵力不足, 无法雇佣.");
			else if ((pk::building_to_port(pk::get_building(banditcity_id[1]))).drills_ended)
				return pk::encode("已经进行过雇佣.");   //以异族城的训练完成的值作为判定此回合是否雇佣的参数
			else if (getEmptyPos(taishu_building) == -1)
				return pk::encode("没有空地生成外援.");
			else if (force.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力))", ACTION_COST));
			else
				return pk::encode(pk::format("雇佣乌丸族.(使用 {} 技巧P)", TP_COST));
		}
		bool isEnabled_오환족()
		{
			if (building_gold < GOLD_COST) return false;
			else if (!taishu_building.has_skill(특기_친오)) return false;
			else if (pk::get_troops(pk::get_building(banditcity_id[1])) * 2 < troops) return false;
			else if ((pk::building_to_port(pk::get_building(banditcity_id[1]))).drills_ended) return false;
			else if (getEmptyPos(taishu_building) == -1) return false;
			else if (force.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST) return false;
			return true;
		}


		string getDesc_산월족()
		{
			if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			else if (!taishu_building.has_skill(특기_친월))
				return pk::encode("城市需拥有亲越特技的武将.");
			else if (pk::get_troops(pk::get_building(banditcity_id[2])) * 2 < troops)
				return pk::encode("山越族兵力不足, 无法雇佣.");
			else if ((pk::building_to_port(pk::get_building(banditcity_id[2]))).drills_ended)
				return pk::encode("已经进行过雇佣.");   //以异族城的训练完成的值作为判定此回合是否雇佣的参数
			else if (getEmptyPos(taishu_building) == -1)
				return pk::encode("没有空地生成外援.");
			else if (force.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("雇佣山越族.(使用 {} 技巧P)", TP_COST));
		}
		bool isEnabled_산월족()
		{
			if (building_gold < GOLD_COST) return false;
			else if (!taishu_building.has_skill(특기_친월)) return false;
			else if (pk::get_troops(pk::get_building(banditcity_id[2])) * 2 < troops) return false;
			else if ((pk::building_to_port(pk::get_building(banditcity_id[2]))).drills_ended) return false;
			else if (getEmptyPos(taishu_building) == -1) return false;
			else if (force.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST) return false;
			return true;
		}


		string getDesc_남만족()
		{
			if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			else if (!ch::has_skill(taishu_building, 특기_친만))
				return pk::encode("城市需拥有亲蛮特技的武将.");
			else if (pk::get_troops(pk::get_building(banditcity_id[3])) * 2 < troops)
				return pk::encode("南蛮族兵力不足, 无法雇佣.");
			else if ((pk::building_to_port(pk::get_building(banditcity_id[3]))).drills_ended)
				return pk::encode("已经进行过雇佣.");   //以异族城的训练完成的值作为判定此回合是否雇佣的参数
			else if (getEmptyPos(taishu_building) == -1)
				return pk::encode("没有空地生成外援.");
			else if (force.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("雇佣南蛮族.(使用 {} 技巧P)", TP_COST));
		}
		bool isEnabled_남만족()
		{
			if (building_gold < GOLD_COST) return false;
			else if (!ch::has_skill(taishu_building, 특기_친만)) return false;
			else if (pk::get_troops(pk::get_building(banditcity_id[3])) * 2 < troops) return false;
			else if ((pk::building_to_port(pk::get_building(banditcity_id[3]))).drills_ended) return false;
			else if (getEmptyPos(taishu_building) == -1) return false;
			else if (force.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST) return false;
			return true;
		}



		pk::random random(pk::rand());

		//----------------------------------------------------------------------------------------------------------
		// handler
		//----------------------------------------------------------------------------------------------------------



		//	羌族
		bool handler_강족()
		{
			//if (pk::choose(pk::encode("是否要雇佣羌族部队?\n*\x1b[29x请勿将雇佣的部队委任攻击,撤退!回合内该部队只能在自己可移动的格子内执行相关战斗指令!\x1b[0x*"), { pk::encode("  是  "), pk::encode("  否  ") }) == 1)
			if (pk::choose(pk::encode("是否要雇佣羌族部队?"), { pk::encode("  是  "), pk::encode("  否  ") }) == 1)
				return true;

			pk::person@ iminzoku = getEmptyPerson_雇佣兵();

			if (pk::rand_bool(고용실패확률) or iminzoku is null)
			{
				failure_event(무장_강두목, 681);
			}
			else
			{
				int district_num = pk::get_district_id(force, 1);
				pk::set_district(iminzoku, district_num);

				int iminzoku_id = iminzoku.get_id();
				setTempPerson(iminzoku_id, 성별_남, 681, 0, 71);
				iminzoku.tekisei[병종_기병] = 적성_S;

<<<<<<< HEAD
				bool success = createUnit(iminzoku_id, 병기_군마);
=======
				bool success = createUnit(iminzoku_id, 兵器_战马);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				if (success)
					success_event(iminzoku_id, 0);	//	对話以及資源减除
				else
					pk::history_log(force.get_pos(), force.color, pk::encode("雇佣兵生成失败"));

			}

			return true;
		}



		bool handler_오환족()	//乌丸
		{

			//if (pk::choose(pk::encode("是否要雇佣乌丸族部队?\n*\x1b[29x请勿将雇佣的部队委任攻击,撤退!回合内该部队只能在自己可移动的格子内执行相关战斗指令!\x1b[0x*"), { pk::encode("  是  "), pk::encode("  否  ") }) == 1)
			if (pk::choose(pk::encode("是否要雇佣乌丸族部队?"), { pk::encode("  是  "), pk::encode("  否  ") }) == 1)
				return true;

			pk::person@ iminzoku = getEmptyPerson_雇佣兵();

			if (pk::rand_bool(고용실패확률) or iminzoku is null)
			{
				failure_event(무장_오환두목, 679);
			}
			else
			{
				int district_num = pk::get_district_id(force, 1);
				pk::set_district(iminzoku, district_num);

				int iminzoku_id = iminzoku.get_id();
				setTempPerson(iminzoku_id, 성별_남, 679, 0, 71);

				iminzoku.tekisei[병종_기병] = 적성_S;
				iminzoku.update();

<<<<<<< HEAD
				bool success = createUnit(iminzoku_id, 병기_군마);
=======
				bool success = createUnit(iminzoku_id, 兵器_战马);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				if (success)
					success_event(iminzoku_id, 1);	//	对話以及資源减除
				else
					pk::history_log(force.get_pos(), force.color, pk::encode("雇佣兵生成失败"));

			}

			return true;
		}



		bool handler_산월족()	//山越
		{

			//if (pk::choose(pk::encode("是否要雇佣山越族部队?\n*\x1b[29x请勿将雇佣的部队委任攻击,撤退!回合内该部队只能在自己可移动的格子内执行相关战斗指令!\x1b[0x*"), { pk::encode("  是  "), pk::encode("  否  ") }) == 1)
			if (pk::choose(pk::encode("是否要雇佣山越族部队?"), { pk::encode("  是  "), pk::encode("  否  ") }) == 1)
				return true;

			pk::person@ iminzoku = getEmptyPerson_雇佣兵();

			if (pk::rand_bool(고용실패확률) or iminzoku is null)
			{
				failure_event(무장_산월두목, 683);
			}
			else
			{
				int district_num = pk::get_district_id(force, 1);
				pk::set_district(iminzoku, district_num);

				int iminzoku_id = iminzoku.get_id();
				setTempPerson(iminzoku_id, 성별_남, 683, 2, 72);

				iminzoku.tekisei[병종_극병] = 적성_S;
				iminzoku.update();

<<<<<<< HEAD
				bool success = createUnit(iminzoku_id, 병기_극);
=======
				bool success = createUnit(iminzoku_id, 兵器_戟);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				if (success)
					success_event(iminzoku_id, 2);	//	对話以及資源减除
				else
					pk::history_log(force.get_pos(), force.color, pk::encode("雇佣兵生成失败"));

			}

			return true;
		}



		bool handler_남만족()	//南蛮
		{

			//if (pk::choose(pk::encode("是否要雇佣南蛮族部队?\n*\x1b[29x请勿将雇佣的部队委任攻击,撤退!回合内该部队只能在自己可移动的格子内执行相关战斗指令!\x1b[0x*"), { pk::encode("  是  "), pk::encode("  否  ") }) == 1)
			if (pk::choose(pk::encode("是否要雇佣南蛮族部队?"), { pk::encode("  是  "), pk::encode("  否  ") }) == 1)
				return true;

			pk::person@ iminzoku = getEmptyPerson_雇佣兵();

			if (pk::rand_bool(고용실패확률) or iminzoku is null)
			{
				failure_event(무장_남만두목, 685);
			}
			else
			{
				int district_num = pk::get_district_id(force, 1);
				pk::set_district(iminzoku, district_num);

				int iminzoku_id = iminzoku.get_id();
				setTempPerson(iminzoku_id, 성별_남, 685, 2, 72);

				iminzoku.tekisei[병종_극병] = 적성_S;

<<<<<<< HEAD
				bool success = createUnit(iminzoku_id, 병기_극);
=======
				bool success = createUnit(iminzoku_id, 兵器_戟);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				if (success)
					success_event(iminzoku_id, 3);	//	对話以及資源减除
				else
					pk::history_log(force.get_pos(), force.color, pk::encode("雇佣兵生成失败"));

			}

			return true;
		}



		//	 招募失敗时发生的事件
		void failure_event(int toumu_id, int toumu_face)
		{
			pk::person@ toumu = pk::get_person(toumu_id);
			toumu.face = toumu_face;
			toumu.update();
			pk::person@ taishu_0 = taishu is null ? pk::get_person(武将_文官) : taishu;
			string taishu_name = pk::decode(pk::get_name(taishu_0));

			pk::message_box(pk::encode(pk::format("很抱歉, 我们现在帮不了\x1b[1x{}\x1b[0x.", taishu_name)), toumu);
			pk::message_box(pk::encode("... 那只好作罢了."), taishu_0);
			pk::message_box(pk::encode(pk::format("作为补偿我们支援些兵粮给 \x1b[1x{}\x1b[0x.", taishu_name)), toumu);
			pk::message_box(pk::encode("好, 也算没白来一次."), taishu_0);

			pk::message_box(pk::encode("雇佣失败了."));

			// 兵粮补偿
			pk::message_box(pk::encode(pk::format("粮食加了\x1b[1x{}\x1b[0x.", ADD_FOOD)));
			pk::add_food(city, ADD_FOOD, true);

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force, 1));
			district.ap -= ACTION_COST;
			pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.", TP_COST)));
			ch::add_tp(force, -TP_COST, force.get_pos());
		}


		//	 招募成功后发生的事件
		void success_event(int iminzoku_id, int num)
		{
			pk::person@ iminzoku = pk::get_person(iminzoku_id);
			string iminzoku_name = pk::decode(pk::get_name(iminzoku));
			pk::person@ taishu_0 = taishu is null ? pk::get_person(武将_文官) : taishu;
			string taishu_name = pk::decode(pk::get_name(taishu_0));

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x, 远道而来辛苦了.", iminzoku_name)), taishu_0);
			pk::message_box(pk::encode(pk::format("我们乐意为\x1b[1x{}\x1b[0x而战!", taishu_name)), iminzoku);
			pk::message_box(pk::encode("这些是答谢你们的帮助的."), taishu_0);
			pk::message_box(pk::encode("哇!给了我们这么多金子!太感谢了!"), iminzoku);
			pk::message_box(pk::encode(pk::format("我们不会忘记\x1b[1x{}\x1b[0x的恩情.", iminzoku_name)), taishu_0);

			//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x加入了我军.", iminzoku_name)));

			// 기교 및 금 감소
			pk::message_box(pk::encode(pk::format("技巧点减少了\x1b[1x{}\x1b[0x.", TP_COST)));
			ch::add_tp(force, -TP_COST, force.get_pos());
			pk::message_box(pk::encode(pk::format("资金减少了\x1b[1x{}\x1b[0x.", GOLD_COST)));
			pk::add_gold(city, -GOLD_COST, true);
			//和对应异族的关系提升，兵力减少，雇佣完成
			pk::add_relations(force, banditforce_id[num], 异族关系提升值);
			pk::add_troops(pk::get_building(banditcity_id[num]), -(troops / 2), false);
			(pk::building_to_port(pk::get_building(banditcity_id[num]))).drills_ended = true;  //训练已完成（雇佣已进行）

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force, 1));
			district.ap -= ACTION_COST;
		}


		//	生成部队
		bool createUnit(int iminzoku_id, int weapon)
		{
			pk::person@ iminzoku = pk::get_person(iminzoku_id);
			int gold = 1000;
			int food = int(troops * 1.3f);

			// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
			pk::point emptyPos = getEmptyPos(taishu_building);

			if (!pk::is_valid_pos(emptyPos)) return false;

			pk::unit@ unit = pk::create_unit(taishu_building, iminzoku, null, null, troops, weapon, 병기_주가, gold, food, emptyPos);

			// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
			unit.energy = 100;

			// 부대 이동 불가 시 부대 및 무장 삭제 후 중단
			if (pk::get_movable_pos(unit).length == 0) { pk::kill(unit);	pk::reset(iminzoku);	return false; }

			// 부대 생성 실패 시 부대 및 무장 삭제 후 중단
			if (!pk::is_alive(unit)) { pk::kill(unit); pk::reset(iminzoku);	return false; }

			return true;
		}


		pk::point getEmptyPos(pk::building@ building)
		{
			auto arr = pk::range(building.get_pos(), min_distance, max_distance);
			for (int i = 0; i < int(arr.length); i++)
			{
				pk::hex@ hex = pk::get_hex(arr[i]);
				int terrain_id = hex.terrain;

				if (!hex.has_building and !hex.has_unit
					and pk::is_valid_terrain_id(terrain_id)
					and pk::is_enabled_terrain(terrain_id)
					and terrain_id != 지형_산)
					return arr[i];

			}
			return -1;
		}


		pk::person@ getEmptyPerson_雇佣兵()
		{
			for (int i = 临时武将起始; i <= 临时武将终止; i++)
			{
				pk::person@ new_face = pk::get_person(i);
				switch (new_face.mibun)
				{
				case 신분_없음:
				case 신분_미등장:
				case 신분_사망:
					return new_face;
				}
			}
			return null;
		}

		void setTempPerson(int new_person_id, int sex, int face, int skeleton, int body)
		{
			pk::person@ new_person = pk::get_person(new_person_id);
			new_person.name_read = pk::encode("雇佣兵");
			new_person.sex = sex;

			if (이름자동)
			{
				new_person.sei = pk::get_family_name(random(성씨_끝 - 1)).name; // 성
				new_person.mei = pk::get_family_name(random(성씨_끝 - 1)).name; // 명
			}
			else
			{
				new_person.sei = pk::encode(sei_[random(i, j)]); // 성
				new_person.mei = pk::encode(mei_[random(i, j)]); // 명
			}

			//敲鱼提出的，将异族所属设定一个不存在的大值能解决委任闪退
			new_person.service = 建筑_末 - 1;
			new_person.face = face;
			new_person.mibun = 身份_一般;
			new_person.loyalty = 100;
			new_person.birth = pk::get_year() - 30;
			new_person.death = pk::min(353, new_person.birth + 99);
			new_person.appearance = pk::get_year() - 10;
			new_person.rank = 관직_정동장군;
			new_person.kouseki = 1000;
			new_person.shoubyou = 0;   //伤病
			new_person.estimated_death = false;   //死亡预订

			new_person.birthplace = random(주_끝 - 1);
			new_person.skill = random(특기_끝) - 1;
			new_person.wadai = random(화제_끝 - 1);
			new_person.giri = random(의리_끝 - 1);
			new_person.ambition = random(야망_끝 - 1);
			new_person.promotion = random(기용_끝 - 1);
			new_person.character = random(성격_끝 - 1);
			new_person.voice = new_person.sex == 성별_남 ? random(음성_소심남, 음성_저돌남) : random(음성_냉정녀, 음성_대담녀);
			new_person.tone = new_person.sex == 성별_남 ? random(말투_장비, 말투_보통남) : random(말투_고대만족, 말투_보통녀);
			new_person.kanshitsu = random(한실_끝 - 1);
			new_person.strategic_tendency = random(전략경향_끝 - 1);
			new_person.local_affiliation = random(지역집착_끝 - 1);

			// 造型
			new_person.skeleton = skeleton;
			new_person.body[0] = body;
			new_person.body[1] = body;
			new_person.body[2] = body;
			new_person.body[3] = body;
			new_person.body[4] = body;
			new_person.body[5] = body;

			//	亲爱厌恶关系
			for (i = 0;i < 5;i++)
			{
				new_person.liked[i] = -1;
				new_person.disliked[i] = -1;
			}

			// 能力
			for (int i = 0; i < 武将能力_末; i++)
			{
				new_person.base_stat[i] = random(min_stat, max_stat);
				new_person.stat_aging[i] = random(능력성장_초지속형, 능력성장_장비형);
			}
			// 适性
			for (int i = 0; i < 병종_끝; i++)
			{
				new_person.tekisei[i] = random(适性_C, 适性_A);
			}

			//  話术
			new_person.wajutsu_daikatsu = pk::rand_bool(30);  //大喝
			new_person.wajutsu_kiben = pk::rand_bool(30);     //詭辯
			new_person.wajutsu_mushi = pk::rand_bool(30);     //无視
			new_person.wajutsu_chinsei = pk::rand_bool(30);   //?静
			new_person.wajutsu_gyakujou = pk::rand_bool(30);  //憤怒

			new_person.update();
		}
		
		
		//AI招募雇佣兵事件
		void callback()
		{
			if (AI雇佣开启)
				pk::scene(pk::scene_t(scene_AI_hire));
		}
		
		
		//AI雇佣
		void scene_AI_hire()
		{
			for (int i = 0; i < 建筑_城市末; i++)
			{
				int weapon_0 = -1;
				int num = 0;
				//pk::trace(pk::format("i等于{}", i));
				auto building_t = pk::get_building(i);
				
				if (building_t.is_player())
					continue;  //跳过玩家地盘
				if (!pk::enemies_around(building_t))
					continue;  //跳过非交战
				if (building_t.get_force_id() == -1)
					continue;  //跳过空城无势力
				
				/* if (pk::get_taishu_id(building_t) == -1)
					continue;  //跳过无太守 */
				
				if (building_t.has_skill(특기_친강) && pk::get_troops(pk::get_building(banditcity_id[0])) * 2 > troops)
				{
					weapon_0 = 4;
					num = 0;
				}
				else if (building_t.has_skill(특기_친오) && pk::get_troops(pk::get_building(banditcity_id[1])) * 2 > troops)
				{
					weapon_0 = 4;
					num = 1;
				}
				else if (building_t.has_skill(특기_친월) && pk::get_troops(pk::get_building(banditcity_id[2])) * 2 > troops)
				{
					weapon_0 = 2;
					num = 2;
				}
				else if (building_t.has_skill(특기_친만) && pk::get_troops(pk::get_building(banditcity_id[3])) * 2 > troops)
				{
					weapon_0 = 2;
					num = 3;
				}
					
				//if (building_t.has_skill(특기_친월) or building_t.has_skill(특기_친만))  weapon_0 = 2;  //亲越亲蛮 兵器为戟
				//if (building_t.has_skill(특기_친강) or building_t.has_skill(특기_친오)) 	weapon_0 = 4;  //亲羌亲乌 兵器为军马
				//pk::trace(pk::format("weapon_0：{}，亲越：{},亲蛮：{}", weapon_0, building_t.has_skill(특기_친월), building_t.has_skill(특기_친만)));
				//pk::trace(pk::format("weapon_0：{}，亲羌：{},亲乌：{}", weapon_0, building_t.has_skill(특기_친강), building_t.has_skill(특기_친오)));
				if (weapon_0 == -1)
					continue;  //跳过没有四亲或者兵力不足
				if (pk::get_gold(building_t) < GOLD_COST)
					continue;  //跳过资金不足

				auto emptyPos = getEmptyPos(building_t);
				if (emptyPos == -1)
					continue;  //跳过没有空白区域
				if ((pk::get_force(building_t.get_force_id())).tp < TP_COST)
					continue;  //跳过没有足够技巧
				
				
				//int AI实际失败 = is_player_attacking(building_t) ? (AI雇佣失败概率 / 2) : AI雇佣失败概率;
				//AI实际失败 = need_to_hire(building_t) ? pk::max((AI实际失败 - 20), 0) : AI实际失败;
				int AI实际失败 = prob_change_hire(building_t);
				//pk::trace(pk::format("AI实际失败等于{}", AI实际失败));
				if (pk::rand_bool(AI实际失败))
					continue;  //概率不触发
				
				/* if (!need_to_hire(building_t))
					continue;  //跳过不需要雇佣 */
				
				
				pk::person@ iminzoku = getEmptyPerson_雇佣兵();

				if (iminzoku !is null)
				{
					int district_num = building_t.get_district_id();
					pk::set_district(iminzoku, district_num);

					int iminzoku_id = iminzoku.get_id();
					setTempPerson(iminzoku_id, 성별_남, banditface_id[num], 0, 71);

					iminzoku.tekisei[weapon_0 - 1] = 적성_S;  //适性比兵器的数值小1
					iminzoku.update();
					
					//生成部队
					//pk::point emptyPos = getEmptyPos(building_t);
					if (!pk::is_valid_pos(emptyPos)) continue;
					
					pk::unit@ unit = pk::create_unit(building_t, iminzoku, null, null, troops, weapon_0, 병기_주가, 1000, 10000, emptyPos);
					unit.energy = 100;

					// 부대 이동 불가 시 부대 및 무장 삭제 후 중단
					if (pk::get_movable_pos(unit).length == 0) { pk::kill(unit);	pk::reset(iminzoku);	continue; }

					// 부대 생성 실패 시 부대 및 무장 삭제 후 중단
					if (!pk::is_alive(unit)) { pk::kill(unit); pk::reset(iminzoku);	continue; }
					
					string iminzoku_name = pk::decode(pk::get_name(iminzoku));
					pk::person@ taishu_0 = pk::get_taishu_id(building_t) != -1 ? pk::get_person(pk::get_taishu_id(building_t)) : pk::get_person(武将_文官);
					string taishu_name = pk::decode(pk::get_name(taishu_0));
					
					if (pk::is_in_screen(emptyPos)) pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x, 远道而来辛苦了.", iminzoku_name)), taishu_0, building_t);
					if (pk::is_in_screen(emptyPos)) pk::say(pk::encode(pk::format("我雇佣军起誓，定不负\x1b[1x{}\x1b[0x大人所托!", taishu_name)), iminzoku, unit);

					// 기교 및 금 감소
					ch::add_tp(pk::get_force(building_t.get_force_id()), -TP_COST, (pk::get_force(building_t.get_force_id())).get_pos());
					//是否在屏幕中 显示特效
					if (pk::is_in_screen(emptyPos)) pk::add_gold(building_t, -GOLD_COST, true);
					pk::add_troops(pk::get_building(banditcity_id[num]), -(troops / 2), false);
					
					pk::history_log(emptyPos, (pk::get_force(building_t.get_force_id())).color, pk::encode(pk::format("\x1b[1x{}队\x1b[0x受到雇佣而加入战场.", iminzoku_name)));
				}
			}
		}
		
		//是否玩家在攻城
		/* bool is_player_attacking(pk::building@ building)
		{
			auto arr = pk::range(building.get_pos(), min_distance, max_distance);
			for (int i = 0; i < int(arr.length); i++)
			{
				if ((pk::get_hex(arr[i])).has_unit)
				{
					if ((pk::get_unit(arr[i])).is_player()) return true;  //坐标上的部队是玩家的
				}
			}
			return false;
		} */
		
		//是否需要雇佣（部队数、兵力比较）
		/* bool need_to_hire(pk::building@ building)
		{
			auto arr = pk::range(building.get_pos(), min_distance, max_distance);
			int num_self = 0, num_enemy = 0, troops_self = 0, troops_enemy = 0;
			for (int i = 0; i < int(arr.length); i++)
			{
				if ((pk::get_hex(arr[i])).has_unit)
				{
					auto unit_0 = pk::get_unit(arr[i]);
					if (unit_0.get_force_id() == building.get_force_id())
					{
						num_self += 1;  //部队数+1
						troops_self += unit_0.troops;  //兵力+
					}
					else if (unit_0.get_force_id() != building.get_force_id())
					{
						num_enemy += 1;  //部队数+1
						troops_enemy += unit_0.troops;  //兵力+
					}
				}
			}
			
			//是否需要雇佣（部队数、兵力比较）
			if ((troops_enemy >= 30000 or num_enemy >= 6) and ((troops_enemy * 2) > troops_self or (num_enemy * 2) > num_self))
				return true;  //兵力超过30000部队数超过6支，涉嫌拔城全力还击
			if ((troops_enemy <= 30000 and num_enemy < 6) and ((troops_enemy + 15000) > troops_self or (num_enemy + 2) > num_self))
				return true;  //兵力未超过部队未超过，稍稍动员

			return false;
		} */
		
		//AI雇佣概率修正
		int prob_change_hire(pk::building@ building)
		{
			int hire = AI雇佣失败概率;
			auto arr = pk::range(building.get_pos(), min_distance, max_distance);
			int num_self = 0, num_enemy = 0, troops_self = 0, troops_enemy = 0;
			bool player_attacking = false;  //玩家在攻城，初始化
			
			for (int i = 0; i < int(arr.length); i++)
			{
				if ((pk::get_hex(arr[i])).has_unit)
				{
					auto unit_0 = pk::get_unit(arr[i]);
					if (!player_attacking && unit_0.is_player())
						player_attacking = true;  //玩家在攻城
					if (unit_0.get_force_id() == building.get_force_id())
					{
						num_self += 1;  //部队数+1
						troops_self += unit_0.troops;  //兵力+
					}
					else if (unit_0.get_force_id() != building.get_force_id())
					{
						num_enemy += 1;  //部队数+1
						troops_enemy += unit_0.troops;  //兵力+
					}
				}
			}
			
			if (player_attacking)
				hire = pk::max((hire - 20), 0);   //为玩家攻城时，失败概率大减少
			
			//是否需要雇佣（部队数、兵力比较）
			if ((troops_enemy >= 30000 or num_enemy >= 6) and ((troops_enemy * 2) > troops_self or (num_enemy * 2) > num_self))
				hire = pk::max((hire - 10), 0);  //兵力超过30000部队数超过6支，涉嫌拔城全力还击
			else if ((troops_enemy <= 15000 and num_enemy < 4 and building.hp > 4000) and ((troops_enemy * 2) > troops_self or (num_enemy * 2) > num_self))
				hire = pk::min((hire + 10), 100);  //兵力未超过部队未超过，稍稍增加失败概率
			
			return hire;
		}

	}

	Main main;
}
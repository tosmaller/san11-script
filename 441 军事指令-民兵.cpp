// ## 2021/12/06 # 江东新风 # 写法改进，ai用法改进 ##			
// ## 2021/04/11 # 白马叔叔 # AI民兵 ##											 
// ## 2021/03/06 # 江东新风 # 民兵削弱 ##
// ## 2020/12/28 # 江东新风 # 在敲鱼协助下解决委任退却闪退问题 ##
// ## 2020/12/22 # messi # 增加可能造成闪退操作的提示 ##
// ## 2020/12/15 # 江东新风 # 民兵不携带钱，携带粮食降低，民兵带粮从城里出一半 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/10 # 氕氘氚 # 修复金大於65535时计算出錯的bug ##
// ## 2020/08/09 # 氕氘氚 # 加入【军事】菜单 ##
// ## 2020/08/04 # 氕氘氚 # 修复召喚农民兵后在任武将数显示問題 ##
/*
@제작자: HoneyBee
@설명: 치안 80미만인 도시에서 소환할 수 있는 농민병 부대 SCRIPT

※ 농민병은 기본적으로 능력이 약하도록 설정하였기에, 긴급한 순간에 임시로 사용하는 것을 권해드립니다.
  또한, 농민병 무장은 일회용이므로, 부대가 괴멸하면 자동 소멸합니다.

*/

namespace 民兵_召唤
{

	// ================ CUSTOMIZE ================

	const bool 이름자동 = false;	     // 무장 이름을 자동으로 생성	[이름자동 true일시, 이름수동 false로 지정]

	const int ACTION_COST = 10;      // 행동력 필요량
	const int GOLD_COST = 100;       // 금 필요량

	const int min_stat = 30;        // 농민병의 최소 능력치를 설정 (기본 설정 - 30)
	const int max_stat = 50;        // 농민병의 최대 능력치를 설정 (기본 설정 - 50)

	const int 治安需求 = 80;           // 농민병을 소환할 수 있는 치안 조건 (기본: 80미만)

	const int min_distance = 1;      // 농민병 부대 생성 최소 위치
	const int max_distance = 6;      // 농민병 부대 생성 최대 위치
	const int TROOPS = 3000;		 // 농민병 부대 병력 설정

	const int 临时武将起始 = 780;
	const int 临时武将终止 = 799;

	//AI民兵事件
	const bool AI民兵开启 = true;     //AI民兵功能开启，true为开，false为关
	const int AI民兵基础概率 = 15;  //AI民兵功能发生的基本概率，非实际概率。默认15																 

	/*

		이민족 무장 이름을 ARRAY로 미리 SETTING 하시기 바랍니다. (아래는 임시로 넣은 성명입니다.)
		아래 성명에서 RANDOM으로 출력하여 이민족 무장의 이름이 정해집니다.

		EX) 조조, 조비, 조견, 유비, 유견, 유탁, 손견, 손탁, 손포 등..

	*/

	// 이민족 무장 [성] (임시로 40개 설정)
	array<string> sei_ = { "赵", "钱", "孙", "李", "周", "吴", "郑", "王", "冯", "陈",
						   "楚", "卫", "江", "沈", "韩", "杨", "朱", "秦", "许", "何",
						   "吕", "张", "孔", "曹", "严", "华", "金", "魏", "陶", "姜",
						   "鲁", "马", "刘", "唐", "宋", "徐", "霍", "丁", "陆", "于" };

	// 이민족 무장 [이름] (임시로 40개로 설정)
	array<string> mei_ = { "香", "梅", "白", "超", "风", "火", "落", "沐", "休", "智",
						   "牛", "艾", "爱", "初", "楚", "福", "呱", "鸽", "鬼", "海",
						   "噩", "后", "狐", "江", "久", "铃", "墨", "木", "码", "皮",
						   "曲", "睿", "瑞", "孙", "团", "台", "熊", "羽", "羊", "猪" };


	// EX) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 ... 39 } <- TOTAL 40개 (ARRAY에서 [성], [이름]을 늘릴 때마다 동일한 갯수로 설정하시면 됩니다.)
	int i = 0;   // 배열의 FIRST NUMBER 
	int j = 39;  // 배열의 LAST NUMBER

	array<int> farmer_skill = { 特技_怒发, 特技_扫讨, 特技_乱战, 特技_掎角, 特技_劫财, 特技_截粮, 特技_看破 , 特技_言毒, 特技_机略, 特技_诡计};
	const int KEY = pk::hash("民兵_召唤");

	///下面内容只是为了防止kre报错，实际不起效，所以kre修改也不起效
	const int 치안_80 = 80;           // 농민병을 소환할 수 있는 치안 조건 (기본: 80미만)
	const int troops = 3000;		 // 농민병 부대 병력 설정

	// ===========================================
	const bool 调试模式 = false;

	class Main
	{

		pk::building@ building_;

		int check_result_;

		pk::random random(pk::rand());

		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
			add_menu();
		}

		void add_menu()
		{
			pk::menu_item item;
			item.menu = 101;
			// item.pos = 6;
			item.shortcut = "5";
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
			check_result_ = check_avaliable(building);
		}

		bool isMenuVisible()
		{
			if (building_.get_id() >= 城市_末) return false;
			if (building_.facility != 设施_城市) return false;
			return true;
		}

		//---------------------------------------------------------------------------
		// 농민병
		//---------------------------------------------------------------------------

		string getText()
		{
			return pk::encode("农民兵");
		}

		string getDesc()
		{
			switch (check_result_)
			{
			case 1: return pk::encode("这不是一个城市");
			case 2: return pk::encode(pk::format("粮食不足.(需要{} 粮食)", TROOPS));
			case 3: return pk::encode("城市治安过高无发召唤农民兵.");
			case 4: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			case 5: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			case 6: return pk::encode("本回合召唤次数达上限.");
			case 7: return pk::encode("农民兵武将数量超上限.");
			case 8: return pk::encode("没有召唤农民兵的地点.");
			case 0: return pk::encode(pk::format("召唤农民兵.(消耗\x1b[1x{}\x1b[0x资金,\x1b[1x{}\x1b[0x粮食,\x1b[1x{}\x1b[0x行动力)", GOLD_COST, TROOPS/2, ACTION_COST));
			default:
				return pk::encode("");
			}
			return pk::encode("");

		}

		bool isEnabled()
		{
			if (check_result_ != 0) return false;
			else return true;
		}

		int check_avaliable(pk::building@ building)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			//pk::trace(pk::format("3.1:{}{}{}{}", pk::enemies_around(building), list.count, pk::get_district(building.get_district_id()).ap < ACTION_COST, pk::get_gold(building) < GOLD_COST));			
			if (building.get_id() >= 城市_末 or building.facility != 设施_城市) return 1;
			pk::city@city = pk::building_to_city(building);
			if (int(city.food) < int(TROOPS / 2)) return 2;
			else if (city.public_order >= 治安需求) return 3;
			else if (pk::get_district(building.get_district_id()).ap < ACTION_COST) return 4;
			else if (pk::get_gold(building) < GOLD_COST) return 5;
			else if (base_ex[building.get_id()].farmer_count == 0) return 6;
			else if (getEmptyPerson_农民() is null) return 7;
			else if (getEmptyPos(building) == -1) return 8;
			else return 0;
		}

		bool handler()
		{

			//if (pk::choose(pk::encode("是否要召集农民兵?\n*\x1b[29x请勿将召集的部队委任攻击,撤退!回合内该部队只能在自己可移动的格子内执行相关战斗指令!\x1b[0x*"), { pk::encode(" 是 "), pk::encode(" 否 ") }) == 1) return false;
			if (pk::choose(pk::encode("是否要召集农民兵?"), { pk::encode(" 是 "), pk::encode(" 否 ") }) == 1) return false;

			return run_order(building_,true);
		}

		void onAIRunningOrder(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 据点AI_民兵)
			{
				if (run_order_before(building)) run_order(building, false);
				else if (调试模式) pk::trace("据点AI_民兵 不满足");
			}
		}

		bool run_order_before(pk::building@ building0)
		{
			if (check_avaliable(building0) != 0) return false;
			return true;
		}

		bool run_order(pk::building@building, bool is_player = true)
		{
			pk::person@ farmer = getEmptyPerson_农民();//前面判断过了

			auto emptyPos = getEmptyPos(building);
			pk::unit@farmer_unit = create_农民兵部队(building, farmer, emptyPos);

			if (farmer_unit is null) return false;

			pk::person@ taishu = pk::get_taishu_id(building) != -1 ? pk::get_person(pk::get_taishu_id(building)) : pk::get_person(武将_文官);
			string taishu_name = pk::decode(pk::get_name(taishu));

			if (is_player)
			{
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人我们会站出来助你一臂之力的!", taishu_name)), farmer);
				pk::message_box(pk::encode("哈哈.. 在这种紧急情况各位能来,真的太感谢了!"), taishu);
				pk::message_box(pk::encode(pk::format("好了, 大家来为 \x1b[1x{}\x1b[0x大人效劳!", taishu_name)), farmer);
				auto district = pk::get_district(building.get_district_id());
				pk::add_ap(district, -ACTION_COST);
			}
			else
			{
				if (pk::is_in_screen(emptyPos)) pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人我们会站出来助你一臂之力的!", taishu_name)), farmer, farmer_unit);
				if (pk::is_in_screen(emptyPos)) pk::say(pk::encode("哈哈.. 在这种紧急情况各位能来,真的太感谢了!"), taishu, building);
				pk::history_log(emptyPos, (pk::get_force(building.get_force_id())).color, pk::encode(pk::format("\x1b[1x农民队\x1b[0x受到\x1b[1x{}\x1b[0x召唤而加入战场.", taishu_name)));
			}
			base_ex[building.get_id()].farmer_count -= 1;
			pk::add_gold(building, -GOLD_COST, true);
			pk::add_food(building, -TROOPS, true);
			return true;
		}

		//=============================================支持函数==============================================//
		pk::unit@ create_农民兵部队(pk::building@building_t,pk::person@farmer_1, pk::point emptyPos)
		{
			int district_num = building_t.get_district_id();
			pk::set_district(farmer_1, district_num);

			int farmer_id = farmer_1.get_id();
			int kunshu_id = (pk::get_force(building_t.get_force_id())).kunshu;
			bool enhance = kunshu_id == 武将_张角;
			setFarmer(farmer_id, kunshu_id);

			int weapon = random(兵器_剑, 兵器_弩);
			int gold = 0;
			int final_troops = TROOPS+(enhance?500:0);
			int food = int(final_troops * 1.0f);

			if (!pk::is_valid_pos(emptyPos)) return null;

			pk::unit@ unit = pk::create_unit(building_t, farmer_1, null, null, final_troops, weapon, 兵器_走舸, gold, food, emptyPos);
			unit.energy = 100;

			// 부대 이동 불가 시 부대 및 무장 삭제 후 중단
			if (pk::get_movable_pos(unit).length == 0) { pk::kill(unit);pk::reset(farmer_1); return null;}

			// 부대 생성 실패 시 부대 및 무장 삭제 후 중단
			if (!pk::is_alive(unit)) { pk::kill(unit); pk::reset(farmer_1); return null;}
			return unit;
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

		pk::person@ getEmptyPerson_农民()
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

		void setFarmer(int farmer_id, int liked0_id)
		{
			pk::person@ farmer = pk::get_person(farmer_id);
			farmer.name_read = pk::encode("农民兵");
			farmer.sex = 성별_남;

			if (이름자동)
			{
				farmer.sei = pk::get_family_name(random(성씨_끝 - 1)).name; // 성
				farmer.mei = pk::get_family_name(random(성씨_끝 - 1)).name; // 명
			}
			else
			{
				farmer.sei = pk::encode(sei_[random(i, j)]); // 성
				farmer.mei = pk::encode(mei_[random(i, j)]); // 명
			}

			farmer.service = 建筑_末 - 1;
			farmer.face = 武将_农民;
			farmer.mibun = 身份_一般;
			farmer.loyalty = 100;
			farmer.birth = pk::get_year() - 30;
			farmer.death = pk::min(353, farmer.birth + 99);
			farmer.appearance = pk::get_year() - 10;
			farmer.kouseki = 1000;

			farmer.birthplace = random(주_끝 - 1);
			if (pk::rand_bool(40)) farmer.skill = farmer_skill[pk::rand(farmer_skill.length)];
			else farmer.skill = -1;
			farmer.wadai = random(화제_끝 - 1);
			farmer.giri = random(의리_끝 - 1);
			farmer.ambition = random(야망_끝 - 1);
			farmer.promotion = random(기용_끝 - 1);
			farmer.character = random(성격_끝 - 1);
			farmer.voice = random(음성_소심남, 음성_저돌남);
			farmer.tone = random(말투_장비, 말투_보통남);
			farmer.kanshitsu = random(한실_끝 - 1);
			farmer.strategic_tendency = random(전략경향_끝 - 1);
			farmer.local_affiliation = random(지역집착_끝 - 1);

			//	?空亲爱厌恶关系
			for (i = 0;i < 5;i++)
			{
				farmer.liked[i] = -1;
				farmer.disliked[i] = -1;
			}
			farmer.liked[0] = liked0_id;  //亲爱君主

			// 能力?
			for (int i = 0; i < 武将能力_末; i++)
			{
				farmer.base_stat[i] = random(min_stat, max_stat);
				farmer.stat_aging[i] = random(능력성장_초지속형, 능력성장_장비형);
			}

			// 适性
			for (int i = 0; i < 병종_끝; i++)
			{
				farmer.tekisei[i] = random(适性_C, 적성_B);
			}


			// 무장 조형
			farmer.skeleton = 0;
			farmer.body[0] = 71;
			farmer.body[1] = 71;
			farmer.body[2] = 71;
			farmer.body[3] = 71;
			farmer.body[4] = 71;
			farmer.body[5] = 71;


			//  話术
			farmer.wajutsu_daikatsu = pk::rand_bool(30);  //大喝
			farmer.wajutsu_kiben = pk::rand_bool(30);     //詭辯
			farmer.wajutsu_mushi = pk::rand_bool(30);     //无視
			farmer.wajutsu_chinsei = pk::rand_bool(30);   //?静
			farmer.wajutsu_gyakujou = pk::rand_bool(30);  //憤怒

			farmer.update();
		}
		
		//AI民兵事件
		void callback()
		{
			if (AI民兵开启)
				pk::scene(pk::scene_t(scene_AI_farmer));
		}
		
		//AI民兵功能
		void scene_AI_farmer()
		{
			for (int i = 0; i < 建筑_城市末; i++)
			{
				int weapon_0 = -1;
				//pk::trace(pk::format("i等于{}", i));
				auto building_t = pk::get_building(i);
				
				if (building_t.is_player())
					continue;  //跳过玩家地盘
				if (!pk::enemies_around(building_t))
					continue;  //跳过非交战
				if (building_t.get_force_id() == -1)
					continue;  //跳过空城无势力
				if (pk::get_taishu_id(building_t) == -1)
					continue;  //跳过无太守
				if ((pk::building_to_city(building_t).public_order) >= 治安需求)
					continue;  //跳过治安过高

				if (!pk::rand_bool(AI民兵基础概率))
					continue;  //概率不触发

				if (pk::get_gold(building_t) < GOLD_COST)
					continue;  //跳过资金不足

				run_order(building_, false);

			}
		}
	}

	Main main;
}
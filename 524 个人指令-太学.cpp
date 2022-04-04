// ## 2021/01/31 # 江东新风 # rand函数错误修复 ##
// ## 2020/12/31 # 江东新风 # 加强太学，君主不再已行动 ##
// ## 2020/12/15 # messi # 修复文本乱码错误，增加太学几十道题库 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/08 # 氕氘氚 # 加入【个人】菜单 ##
/*
@제작자: HoneyBee
@설명: 삼국지 관련 QUIZ를 풀어서 맞히면 TP 획득, 틀리면 TP 감소하는 SCRIPT
	  [QUIZ의 내용은 正史, 연의, 창작물 등을 참고.]

	 (군주가 소속된 도시에서 '지도' 특기를 소유하고 있는 무장이 존재해야 실행 가능)

*/

namespace 학습관
{


	// ================ CUSTOMIZE ================

	const int ACTION_COST = 50;     	// (행동력 필요량: 기본 50 설정)	
	const int GOLD_COST = 500;    		// (금 사용량: 기본 500 설정)

	// TP는 상승치는 설정된 값의 절반만큼 상승합니다. (EX: 1000으로 설정해도 500만 상승, 500으로 설정해도 250만 상승)
	const int TP_상승 = 100;    	        // (정답인 경우 기교P 획득량: 기본 100 설정)
	const int TP_감소 = 50;    	   	    // (오답인 경우 기교P 감소량: 기본 50 설정)

	const int KEY = pk::hash("怖人开");

	// ===========================================


	class Main
	{

		pk::building@ building_;
		pk::building@ kunshu_building;
		pk::force@ force_;
		pk::person@ kunshu_;
		pk::city@ kunshu_city;
		pk::city@ city_;
		int building_gold;
		int quiz_result;

		Main()
		{
			pk::menu_item item;
			item.menu = global_menu::菜单_个人;
			//ch::u8debug(pk::format("menu: tai xue {}", global_menu::菜单_个人));
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
			@force_ = pk::get_force(building.get_force_id());
			@kunshu_ = pk::get_person(pk::get_kunshu_id(building));
			@kunshu_building = pk::get_building(kunshu_.service);
			@kunshu_city = pk::building_to_city(kunshu_building);
			@city_ = pk::building_to_city(building);
			building_gold = pk::get_gold(building);
		}


		bool isEnabled()
		{
			if (building_.get_id() != kunshu_.service) return false;
			else if (!kunshu_building.has_skill(특기_지도)) return false;
			else if (building_gold < GOLD_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		string getText()
		{
			return pk::encode("太学");  //학습관
		}

		string getDesc()
		{
			if (building_.get_id() != kunshu_.service)
				return pk::encode("只能在君主所在城市或港关实行.");
			else if (!kunshu_building.has_skill(특기_지도))
				return pk::encode("城市需拥有指导特技的武将.");
			else if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("进入太学学习. (行动力 {})", ACTION_COST));
		}

		bool handler()
		{

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("是否要进入太学学习?\n(消耗金{} 获得技巧点)", GOLD_COST)), kunshu_) == 1)
				return false;

			pk::scene(pk::scene_t(scene_Quiz));

			pk::person@ examiner = pk::get_person(무장_문관);

			// 정답
			if (quiz_result == 0)
			{

				pk::message_box(pk::encode("主公你知识真渊博."), examiner);
				pk::message_box(pk::encode("哈哈哈, 这对我来说只是牛刀小试!"), kunshu_);

				// 기교P 획득
				ch::add_tp(force_, TP_상승, force_.get_pos());
			}

			// 오답
			if (quiz_result == 1)
			{
				pk::message_box(pk::encode("这问题确实比较棘手."), examiner);
				pk::message_box(pk::encode("以后我可得更加努力.."), kunshu_);

				// 기교P 감소
				ch::add_tp(force_, -TP_감소, force_.get_pos());
			}

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			// 금 감소
			pk::add_gold(building_, -GOLD_COST, true);

			//kunshu_.action_done = true;

			return true;

		}


		void scene_Quiz()
		{

			pk::person@ examiner = pk::get_person(무장_문관);

			pk::message_box(pk::encode("主公,我要出题了."), examiner);  //붚띕,겟傾뼙잖古.
			pk::message_box(pk::encode("准备好了嘛?"), examiner);
			pk::message_box(pk::encode("当然."), kunshu_);

			int q = pk::rand(50);

			if (q == 0)
			{
				array<string> quiz_list =
				{
					pk::encode("孙坚"),
					pk::encode("袁绍"),
					pk::encode("曹操"),
					pk::encode("刘备")
				};

				int answer = pk::choose(pk::encode("十八路诸侯伐董时, 总盟主是谁?"), quiz_list);

				if (answer == 1)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 1)
			{
				array<string> quiz_list =
				{
					pk::encode("刘表"),
					pk::encode("孙权"),
					pk::encode("刘琦"),
					pk::encode("周瑜")
				};

				int answer = pk::choose(pk::encode("对诸葛亮用上屋抽梯而求活命之术的是谁?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 2)
			{
				array<string> quiz_list =
				{
					pk::encode("魏延"),
					pk::encode("姜维"),
					pk::encode("关羽"),
					pk::encode("赵云")
				};

				int answer = pk::choose(pk::encode("除诸葛亮外,以下蜀将谁使用过空城计?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 3)
			{
				array<string> quiz_list =
				{
					pk::encode("新野"),
					pk::encode("赤壁"),
					pk::encode("博望"),
					pk::encode("长坂")
				};

				int answer = pk::choose(pk::encode("诸葛亮初出茅庐第一把火烧了何处?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 4)
			{
				array<string> quiz_list =
				{
					pk::encode("郭嘉"),
					pk::encode("荀攸"),
					pk::encode("郭图"),
					pk::encode("许攸")
				};

				int answer = pk::choose(pk::encode("官渡之战, 献计偷袭乌巢的是?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 5)
			{
				array<string> quiz_list =
				{
					pk::encode("华雄"),
					pk::encode("庞德"),
					pk::encode("曹仁"),
					pk::encode("乐进")
				};

				int answer = pk::choose(pk::encode("棺战关羽的是?"), quiz_list);

				if (answer == 1)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 6)
			{
				array<string> quiz_list =
				{
					pk::encode("公孙瓒"),
					pk::encode("马超"),
					pk::encode("赵云"),
					pk::encode("马腾")
				};

				int answer = pk::choose(pk::encode("号称白马将军的是?"), quiz_list);

				if (answer == 0)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 7)
			{
				array<string> quiz_list =
				{
					pk::encode("釜底抽薪"),
					pk::encode("调虎离山"),
					pk::encode("借刀杀人"),
					pk::encode("十面埋伏")
				};

				int answer = pk::choose(pk::encode("孔明入川擒张任用了什么计?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 8)
			{
				array<string> quiz_list =
				{
					pk::encode("徐庶"),
					pk::encode("曹操"),
					pk::encode("诸葛亮"),
					pk::encode("司马懿")
				};

				int answer = pk::choose(pk::encode("不得慕虚名而处实祸出自谁言?"), quiz_list);

				if (answer == 1)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 9)
			{
				array<string> quiz_list =
				{
					pk::encode("诸葛亮"),
					pk::encode("贾诩"),
					pk::encode("郭嘉"),
					pk::encode("荀彧")
				};

				int answer = pk::choose(pk::encode("后人休把留侯比,临末无颜见汉君.描述的谁?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 10)
			{
				array<string> quiz_list =
				{
					pk::encode("甘宁"),
					pk::encode("太史慈"),
					pk::encode("周泰"),
					pk::encode("黄盖")
				};

				int answer = pk::choose(pk::encode("群英会里, 谁被周瑜用来吓蒋干?"), quiz_list);

				if (answer == 1)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 11)
			{
				array<string> quiz_list =
				{
					pk::encode("  12  "),
					pk::encode("  13  "),
					pk::encode("  14  "),
					pk::encode("  15  ")
				};

				int answer = pk::choose(pk::encode("刘表死时次子刘琮多大?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 12)
			{
				array<string> quiz_list =
				{
					pk::encode("杜门"),
					pk::encode("休门"),
					pk::encode("伤门"),
					pk::encode("惊门")
				};

				int answer = pk::choose(pk::encode("八门金锁阵除了死门, 还有个门入则亡?"), quiz_list);

				if (answer == 0)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 13)
			{
				array<string> quiz_list =
				{
					pk::encode("董卓"),
					pk::encode("曹操"),
					pk::encode("袁绍"),
					pk::encode("孙坚")
				};

				int answer = pk::choose(pk::encode("哪位君主的属下提出过 挟天子以令诸侯 ?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 14)
			{
				array<string> quiz_list =
				{
					pk::encode("鲁肃"),
					pk::encode("周瑜"),
					pk::encode("孙权"),
					pk::encode("诸葛亮")
				};

				int answer = pk::choose(pk::encode("为东吴提出三分天下论的是?"), quiz_list);

				if (answer == 0)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 15)
			{
				array<string> quiz_list =
				{
					pk::encode("武库令"),
					pk::encode("骑都卫"),
					pk::encode("骑校卫"),
					pk::encode("武库丞")
				};

				int answer = pk::choose(pk::encode("曹操演义登场时官居何职?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 16)
			{
				array<string> quiz_list =
				{
					pk::encode("张飞 赵云"),
					pk::encode("赵云 刘备"),
					pk::encode("刘备 关羽"),
					pk::encode("关羽 张飞")
				};

				int answer = pk::choose(pk::encode("火烧博望坡诱敌的是?"), quiz_list);

				if (answer == 1)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 17)
			{
				array<string> quiz_list =
				{
					pk::encode("曹性"),
					pk::encode("曹休"),
					pk::encode("张辽"),
					pk::encode("夏侯渊")
				};

				int answer = pk::choose(pk::encode("赤壁之战,谁一箭射落黄盖救了曹操?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 18)
			{
				array<string> quiz_list =
				{
					pk::encode("程普"),
					pk::encode("祖茂"),
					pk::encode("黄盖"),
					pk::encode("韩当")
				};

				int answer = pk::choose(pk::encode("孙坚手下四将谁是辽西令支人?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 19)
			{
				array<string> quiz_list =
				{
					pk::encode("曹仁"),
					pk::encode("张辽"),
					pk::encode("徐晃"),
					pk::encode("张邰")
				};

				int answer = pk::choose(pk::encode("渭水之战,与曹操分兵击之意见不谋而合的是?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 20)
			{
				array<string> quiz_list =
				{
					pk::encode("顾雍"),
					pk::encode("陆逊"),
					pk::encode("步骘"),
					pk::encode("孙邵")
				};

				int answer = pk::choose(pk::encode("孙权登基后,第一位丞相?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 21)
			{
				array<string> quiz_list =
				{
					pk::encode("公嗣"),
					pk::encode("公瑾"),
					pk::encode("禅祀"),
					pk::encode("阿斗")
				};

				int answer = pk::choose(pk::encode("后主刘禅的字是什么?"), quiz_list);

				if (answer == 0)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 22)
			{
				array<string> quiz_list =
				{
					pk::encode("孙策"),
					pk::encode("乐进"),
					pk::encode("程普"),
					pk::encode("王朗")
				};

				int answer = pk::choose(pk::encode("没有与太史慈进行过单挑的人?"), quiz_list);

				if (answer == 1)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 23)
			{
				array<string> quiz_list =
				{
					pk::encode("火凤燎原"),
					pk::encode("龙狼传"),
					pk::encode("三国战纪"),
					pk::encode("横山光辉")
				};

				int answer = pk::choose(pk::encode("三国志12中合作追加的漫画是?"), quiz_list);

				if (answer == 1)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 24)
			{
				array<string> quiz_list =
				{
					pk::encode("张郃"),
					pk::encode("曹仁"),
					pk::encode("曹洪"),
					pk::encode("满宠")
				};

				int answer = pk::choose(pk::encode("在合肥中活跃的魏国武将?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 25)
			{
				array<string> quiz_list =
				{
					pk::encode("严白虎"),
					pk::encode("杨弘"),
					pk::encode("王凌"),
					pk::encode("曹豹")
				};

				int answer = pk::choose(pk::encode("在淮南地区引起骚乱的武将是?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 26)
			{
				array<string> quiz_list =
				{
					pk::encode("成济"),
					pk::encode("王经"),
					pk::encode("荀顗"),
					pk::encode("王业")
				};

				int answer = pk::choose(pk::encode("杀害魏皇帝曹髦的武将是谁?"), quiz_list);

				if (answer == 0)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 27)
			{
				array<string> quiz_list =
				{
					pk::encode("隐影"),
					pk::encode("暗影"),
					pk::encode("绝影"),
					pk::encode("爪黄飞电")
				};

				int answer = pk::choose(pk::encode("意味着速度快得连影子都看不到的名马?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 28)
			{
				array<string> quiz_list =
				{
					pk::encode("225年"),
					pk::encode("226年"),
					pk::encode("227年"),
					pk::encode("228年")
				};

				int answer = pk::choose(pk::encode("石亭之战发生的时期是?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 29)
			{
				array<string> quiz_list =
				{
					pk::encode("金刚"),
					pk::encode("铁壁"),
					pk::encode("护卫"),
					pk::encode("戟将")
				};

				int answer = pk::choose(pk::encode("三国志11中典韦的特技是?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 30)
			{
				array<string> quiz_list =
				{
					pk::encode("冯氏"),
					pk::encode("郑氏"),
					pk::encode("刘氏"),
					pk::encode("甄氏")
				};

				int answer = pk::choose(pk::encode("袁术的后宫是谁?"), quiz_list);

				if (answer == 0)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 31)
			{
				array<string> quiz_list =
				{
					pk::encode("王莽"),
					pk::encode("董卓"),
					pk::encode("曹操"),
					pk::encode("丁仪")
				};

				int answer = pk::choose(pk::encode("不属于东汉的人物是?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 32)
			{
				array<string> quiz_list =
				{
					pk::encode("大将军"),
					pk::encode("骠骑将军"),
					pk::encode("车骑将军"),
					pk::encode("右将军")
				};

				int answer = pk::choose(pk::encode("张飞在蜀汉担任的最终官职的什么?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 33)
			{
				array<string> quiz_list =
				{
					pk::encode("枪"),
					pk::encode("刀"),
					pk::encode("剑"),
					pk::encode("弓")
				};

				int answer = pk::choose(pk::encode("演义中夏侯惇最常用的武器是什么?"), quiz_list);

				if (answer == 0)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 34)
			{
				array<string> quiz_list =
				{
					pk::encode("子桓"),
					pk::encode("子廉"),
					pk::encode("子孝"),
					pk::encode("子和")
				};

				int answer = pk::choose(pk::encode("曹纯的字是什么?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 35)
			{
				array<string> quiz_list =
				{
					pk::encode("孟坦"),
					pk::encode("刘延"),
					pk::encode("韩福"),
					pk::encode("秦琪")
				};

				int answer = pk::choose(pk::encode("在过五关斩六将中没被关羽所斩的武将是?"), quiz_list);

				if (answer == 1)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 36)
			{
				array<string> quiz_list =
				{
					pk::encode("100回"),
					pk::encode("110回"),
					pk::encode("120回"),
					pk::encode("130回")
				};

				int answer = pk::choose(pk::encode("三国演义共分多少回?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 37)
			{
				array<string> quiz_list =
				{
					pk::encode("10回合"),
					pk::encode("20回合"),
					pk::encode("30回合"),
					pk::encode("40回合")
				};

				int answer = pk::choose(pk::encode("张郃与赵云单挑最多坚持了多少回合?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 38)
			{
				array<string> quiz_list =
				{
					pk::encode("黑泉"),
					pk::encode("柔泉"),
					pk::encode("灭泉"),
					pk::encode("哑泉")
				};

				int answer = pk::choose(pk::encode("喝了就会变成哑巴而死的泉水是?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 39)
			{
				array<string> quiz_list =
				{
					pk::encode("武侯陵"),
					pk::encode("高陵"),
					pk::encode("惠陵"),
					pk::encode("蒋陵")
				};

				int answer = pk::choose(pk::encode("刘备的墓陵叫什么?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 40)
			{
				array<string> quiz_list =
				{
					pk::encode("晋"),
					pk::encode("明"),
					pk::encode("元"),
					pk::encode("清")
				};

				int answer = pk::choose(pk::encode("罗贯中是哪个时代的人?"), quiz_list);

				if (answer == 1)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 41)
			{
				array<string> quiz_list =
				{
					pk::encode("刘表"),
					pk::encode("刘焉"),
					pk::encode("刘璋"),
					pk::encode("张鲁")
				};

				int answer = pk::choose(pk::encode("杨怀,高沛是谁的手下?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 42)
			{
				array<string> quiz_list =
				{
					pk::encode("董袭"),
					pk::encode("潘璋"),
					pk::encode("徐盛"),
					pk::encode("太史慈")
				};

				int answer = pk::choose(pk::encode("不是江东十二虎臣的是谁?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 43)
			{
				array<string> quiz_list =
				{
					pk::encode("249年"),
					pk::encode("250年"),
					pk::encode("251年"),
					pk::encode("252年")
				};

				int answer = pk::choose(pk::encode("发生高平陵事变的年份是?"), quiz_list);

				if (answer == 0)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 44)
			{
				array<string> quiz_list =
				{
					pk::encode("褒谷"),
					pk::encode("河谷"),
					pk::encode("箕谷"),
					pk::encode("祁山")
				};

				int answer = pk::choose(pk::encode("曹真与赵云两军对峙的地点所在?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 45)
			{
				array<string> quiz_list =
				{
					pk::encode("郭淮"),
					pk::encode("徐质"),
					pk::encode("邓艾"),
					pk::encode("牛金")
				};

				int answer = pk::choose(pk::encode("235年击退马岱的武将是谁?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 46)
			{
				array<string> quiz_list =
				{
					pk::encode("俞涉"),
					pk::encode("方悦"),
					pk::encode("潘凤"),
					pk::encode("鲍忠")
				};

				int answer = pk::choose(pk::encode("谁没被华雄杀死?"), quiz_list);

				if (answer == 1)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 47)
			{
				array<string> quiz_list =
				{
					pk::encode("郝萌"),
					pk::encode("成廉"),
					pk::encode("高顺"),
					pk::encode("曹性")
				};

				int answer = pk::choose(pk::encode("不属于吕布八健将的是谁?"), quiz_list);

				if (answer == 2)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 48)
			{
				array<string> quiz_list =
				{
					pk::encode("义封"),
					pk::encode("君理"),
					pk::encode("文博"),
					pk::encode("公伟")
				};

				int answer = pk::choose(pk::encode("朱儁的字是什么?"), quiz_list);

				if (answer == 3)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

			if (q == 49)
			{
				array<string> quiz_list =
				{
					pk::encode("严颜"),
					pk::encode("泠苞"),
					pk::encode("马超"),
					pk::encode("雷铜")
				};

				int answer = pk::choose(pk::encode("张飞入蜀俘虏又放了的是谁?"), quiz_list);

				if (answer == 1)
					quiz_result = 0;
				else
					quiz_result = 1;
			}

		}

	}

	Main main;
}
// ## 2022/03/21 # 江东新风 # 结束时清空临时武将 ##
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/01/17 # messi # 修复部队菜单初始化时会生成武将的bug ##
// ## 2020/12/24 # messi # 只有专属武将召唤时才有可能成功 ##
// ## 2020/12/24 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/31 # 江东新风 # 同步honeybee大神的更新，无法施展任意召唤技时隐藏##
/*
@제작자: HoneyBee
@설명: 특정 보물을 소유한 경우 이미 사망한 무장을 소환하여 효과를 발동하는 부대 메뉴


※ 삼국지연의에서 <관우>, <장비>, <제갈량>의 혼령이 등장한 묘사를 참고하여 반영한 SCRIPT
- (예시) 삼국지연의 85회에서 <유비>에게 나타난 [관우, 장비]의 혼령
- (예시) 삼국지연의 94회에서 <관흥>을 돕는 <관우>의 혼령 (다수 묘사 中 한 가지 예시)
- (예시) 삼국지연의 116회에서 <종회>에게 나타난 <제갈량>의 혼령


== 메뉴 효과 설명 ==
1. 관우 소환 - 적장부상상태, 적군사기를 특정 수치로 변경 (<관우>가 아닌 부대의 주장이 청룡언월도를 소유한 경우 메뉴 사용 가능)
2. 장비 소환 - 적군위보, 적군사기를 특정 수치로 변경 (<장비>가 아닌 부대의 주장이 장팔사모를 소유한 경우 메뉴 사용 가능)
3. 제갈량 소환 - 아군상태이상회복, 적군혼란상태로 변경 (<제갈량>이 아닌 부대의 주장이 병법24편을 소유한 경우 메뉴 사용 가능)
4. 면역되는 특기가 없도록 설정 (일단 발동하면 100% 효과 적용됨)


== 효과 발동 확률 ==
1. 기본적으로 40% 확률로 발동
2. 관우 소환의 경우 부대의 주장이 [관평, 관색, 관흥]이라면 70% 확률로 발동
3. 장비 소환의 경우 부대의 주장이 [장포, 장소]라면 70% 확률로 발동
4. 제갈량 소환의 경우 부대의 주장이 [제갈첨]이라면 70% 확률로 발동

@说明:拥有特定宝物时，召唤已死亡的武将发动效果的部队菜单

※三国演义中，参考<关羽>、<张飞>、<诸葛亮>的灵魂登场的记述而反映的SCRIPT
-(例)三国志演义第85回，<刘>所示[关羽，张飞]之魂
-(例)三国志演义94回中，帮助关兴的关羽的灵魂(多数描写中或例示)
-(例)三国志演义第116回中，<宗>中所示的<孔明>之魂

==菜单的效果的说明==
1.关羽召唤-击伤敌军、将敌军的气力更为特定数值(部队主将不是“关羽”、拥有青龙偃月刀时，可以使用菜单)
2.张飞召唤-敌军伪报，将敌军的气力变更为特定数值(部队主将不是张飞，拥有丈八蛇矛时，可以使用菜单)
3.诸葛亮召唤-己方状态异常回复，敌方混乱(部队主将不是诸葛亮，拥有兵法24篇时，可以使用菜单)
4.没有免疫技术的设定(一旦发动，100%的效果适用)

==效果发动概率==
1.基本上40%的概率发动
2.召唤关羽时，如果部队的主将是[关平、关索、关兴]，则有70%的概率发动。
3.召唤张飞时，如果部队的主将是[张苞，张绍]，则有70%的概率发动
4.召唤诸葛亮时，如果部队的主将是[诸葛瞻，姜维]，则有70%的概率发动。
*/

namespace 부대소환메뉴
{

	// ================ CUSTOMIZE ================


	const bool 소환메뉴_사용 = true; 			// 소환메뉴_사용 설정 (true = on / false = off)

	// 계략에 대한 특기 옵션
	const bool 계략_특기_설정 = true;           // 계략에 대한 특기 설정 사용
	const bool 백출_반감_설정 = true;           // true= 50% 소비, false = 무조건 1 소비
	const bool 계략_병력_차등 = true;           // 병력수에 따른 계략 소비기력 차등 적용

	// 관우 소환 메뉴 관련 CUSTOMIZE
	const bool 관우소환_사용 = true; 			// 관우소환_사용 설정 (true = on / false = off)
	const int ENERGY_COST_관우소환 = 40;       // 관우소환 기력 필요량
	const int 적군사기설정_관우소환 = 50;     		// (예시) 적군사기가 50보다 큰 경우 적군사기를 50으로 변경

	// 장비 소환 메뉴 관련 CUSTOMIZE
	const bool 장비소환_사용 = true; 			// 장비소환_사용 설정 (true = on / false = off)
	const int ENERGY_COST_장비소환 = 40;       // 장비소환 기력 필요량 
	const int 적군사기설정_장비소환 = 50;     		// (예시) 적군사기가 50보다 큰 경우 적군사기를 50으로 변경

	// 제갈량 소환 메뉴 관련 CUSTOMIZE
	const bool 제갈량소환_사용 = true; 			// 제갈량소환_사용 설정 (true = on / false = off)
	const int ENERGY_COST_제갈량소환 = 40;      // 제갈량소환 기력 필요량 

	const int 병력조건 = 5000;	    			// 소환메뉴를 실행할 수 있는 최소 병력 조건

	const int min_범위 = 1;      				// 효과를 받는 최소 범위 
	const int max_범위 = 2; 					// 효과를 받는 최대 범위

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
		pk::point src_pos_;
		pk::person@ src_leader;
		pk::person@ person_关羽;
		pk::person@ person_张飞;
		pk::person@ person_诸葛亮;
		pk::person@ tempPerson;
		pk::item@ item_青龙偃月刀;
		pk::item@ item_丈八蛇矛;
		pk::item@ item_兵法二十四篇;

		bool 관우소환_발동 = false;
		bool 장비소환_발동 = false;
		bool 제갈량소환_발동 = false;

		bool 발동성공 = false;
		bool 발동실패 = false;


		void add_menu_unit_order()
		{
			if (소환메뉴_사용)
			{
				pk::menu_item 부대;
				int 부대_상위;

				부대.menu = 1;
				부대.init = pk::unit_menu_item_init_t(init);
				부대.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("召唤"); });
				부대.is_visible = pk::menu_item_is_visible_t(isVisible_부대);
				부대_상위 = pk::add_menu_item(부대);

				// 관우소환
				if (관우소환_사용)
				{
					pk::menu_item 관우소환;
					관우소환.menu = 부대_상위;
					관우소환.init = pk::unit_menu_item_init_t(init);
					관우소환.get_text = pk::menu_item_get_text_t(getText_관우소환);
					관우소환.get_desc = pk::menu_item_get_desc_t(getDesc_관우소환);
					관우소환.is_enabled = pk::menu_item_is_enabled_t(isEnabled_관우소환);
					관우소환.handler = pk::unit_menu_item_handler_t(handler_관우소환);
					pk::add_menu_item(관우소환);
				}

				// 장비소환
				if (장비소환_사용)
				{
					pk::menu_item 장비소환;
					장비소환.menu = 부대_상위;
					장비소환.init = pk::unit_menu_item_init_t(init);
					장비소환.get_text = pk::menu_item_get_text_t(getText_장비소환);
					장비소환.get_desc = pk::menu_item_get_desc_t(getDesc_장비소환);
					장비소환.is_enabled = pk::menu_item_is_enabled_t(isEnabled_장비소환);
					장비소환.handler = pk::unit_menu_item_handler_t(handler_장비소환);
					pk::add_menu_item(장비소환);
				}

				// 제갈량소환
				if (제갈량소환_사용)
				{
					pk::menu_item 제갈량소환;
					제갈량소환.menu = 부대_상위;
					제갈량소환.init = pk::unit_menu_item_init_t(init);
					제갈량소환.get_text = pk::menu_item_get_text_t(getText_제갈량소환);
					제갈량소환.get_desc = pk::menu_item_get_desc_t(getDesc_제갈량소환);
					제갈량소환.is_enabled = pk::menu_item_is_enabled_t(isEnabled_제갈량소환);
					제갈량소환.handler = pk::unit_menu_item_handler_t(handler_제갈량소환);
					pk::add_menu_item(제갈량소환);
				}

			}

		} // add_menu_unit_order

		void init(pk::unit@ unit, pk::point src_pos)
		{
			@src_unit = @unit;
			src_pos_ = src_pos;
			@src_leader = pk::get_person(src_unit.leader);
			@force = pk::get_force(src_unit.get_force_id());
			@person_关羽 = pk::get_person(무장_관우);
			@person_张飞 = pk::get_person(무장_장비);
			@person_诸葛亮 = pk::get_person(무장_제갈량);

			@item_青龙偃月刀 = pk::get_item(보물_청룡언월도);
			@item_丈八蛇矛 = pk::get_item(보물_사모);
			@item_兵法二十四篇 = pk::get_item(보물_병법24편);
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
			if (person_关羽.mibun == 신분_사망 and item_青龙偃月刀.owner == src_leader.get_id()) return true;
			if (person_张飞.mibun == 신분_사망 and item_丈八蛇矛.owner == src_leader.get_id()) return true;
			if (person_诸葛亮.mibun == 신분_사망 and item_兵法二十四篇.owner == src_leader.get_id()) return true;
			return false;
		}

		//---------------------------------------------------------------------------
		// 부대소환메뉴 : 관우소환 (적장부상상태, 적군사기를 특정수치로 변경)
		//---------------------------------------------------------------------------

		string getText_관우소환()
		{
			return pk::encode(pk::format("召唤关羽 ({})", func_stragety_cost(ENERGY_COST_관우소환)));
		}

		string getDesc_관우소환()
		{
			if (src_unit.energy < func_stragety_cost(ENERGY_COST_관우소환))
				return pk::encode("部队气力不足");
			else if (person_关羽.mibun != 신분_사망)
				return pk::encode("关羽未死");
			else if (item_青龙偃月刀.owner != src_leader.get_id())
				return pk::encode("未拥有青龙偃月刀");
			else if (src_unit.troops < 병력조건)
				return pk::encode("兵力不足");
			else
				return pk::encode(pk::format("召唤关羽英灵(消耗{}气力)", func_stragety_cost(ENERGY_COST_관우소환)));
		}

		bool isVisible_관우소환()
		{
			return true;
		}

		bool isEnabled_관우소환()
		{
			if (src_unit.energy < func_stragety_cost(ENERGY_COST_관우소환)) return false;
			else if (person_关羽.mibun != 신분_사망) return false;
			else if (item_青龙偃月刀.owner != src_leader.get_id()) return false;
			else if (src_unit.troops < 병력조건) return false;
			return true;
		}

		bool handler_관우소환(pk::point dst_pos)
		{

			관우소환_발동 = true;
			summon_effect(src_unit);

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_관우소환), true);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
			{
				src_unit.action_done = false;
			}

			return true;
		}


		//---------------------------------------------------------------------------
		// 부대소환메뉴 : 장비소환 (적군위보, 적군사기를 특정수치로 변경)
		//---------------------------------------------------------------------------

		string getText_장비소환()
		{
			return pk::encode(pk::format("召唤张飞 ({})", func_stragety_cost(ENERGY_COST_장비소환)));
		}

		string getDesc_장비소환()
		{
			if (src_unit.energy < func_stragety_cost(ENERGY_COST_장비소환))
				return pk::encode("部队气力不足");
			else if (person_张飞.mibun != 신분_사망)
				return pk::encode("张飞未死");
			else if (item_丈八蛇矛.owner != src_leader.get_id())
				return pk::encode("未拥有丈八蛇矛");
			else if (src_unit.troops < 병력조건)
				return pk::encode("兵力不足");
			else
				return pk::encode(pk::format("召唤张飞英灵 (消耗{}气力)", func_stragety_cost(ENERGY_COST_장비소환)));
		}

		bool isVisible_장비소환()
		{
			return true;
		}

		bool isEnabled_장비소환()
		{
			if (src_unit.energy < func_stragety_cost(ENERGY_COST_장비소환)) return false;
			else if (person_张飞.mibun != 신분_사망) return false;
			else if (item_丈八蛇矛.owner != src_leader.get_id()) return false;
			else if (src_unit.troops < 병력조건) return false;
			return true;
		}

		bool handler_장비소환(pk::point dst_pos)
		{

			장비소환_발동 = true;
			summon_effect(src_unit);

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_장비소환), true);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
			{
				src_unit.action_done = false;
			}

			return true;
		}


		//---------------------------------------------------------------------------
		// 부대소환메뉴 : 제갈량소환 (아군상태이상회복, 적군혼란, 적군사기를 특정수치로 변경)
		//---------------------------------------------------------------------------

		string getText_제갈량소환()
		{
			return pk::encode(pk::format("召唤诸葛亮({})", func_stragety_cost(ENERGY_COST_제갈량소환)));
		}

		string getDesc_제갈량소환()
		{
			if (src_unit.energy < func_stragety_cost(ENERGY_COST_제갈량소환))
				return pk::encode("部队气力不足.");
			else if (person_诸葛亮.mibun != 신분_사망)
				return pk::encode("诸葛亮未死");
			else if (item_兵法二十四篇.owner != src_leader.get_id())
				return pk::encode("未拥有兵法二十四篇");
			else if (src_unit.troops < 병력조건)
				return pk::encode("兵力不足");
			else
				return pk::encode(pk::format("召唤诸葛亮英灵.(消耗{}气力)", func_stragety_cost(ENERGY_COST_제갈량소환)));
		}

		bool isVisible_제갈량소환()
		{
			return true;
		}

		bool isEnabled_제갈량소환()
		{
			if (src_unit.energy < func_stragety_cost(ENERGY_COST_제갈량소환)) return false;
			else if (person_诸葛亮.mibun != 신분_사망) return false;
			else if (item_兵法二十四篇.owner != src_leader.get_id()) return false;
			else if (src_unit.troops < 병력조건) return false;
			return true;
		}

		bool handler_제갈량소환(pk::point dst_pos)
		{

			제갈량소환_발동 = true;
			summon_effect(src_unit);

			// 실행 부대 기력 감소
			pk::add_energy(src_unit, -func_stragety_cost(ENERGY_COST_제갈량소환), true);

			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
			{
				src_unit.action_done = false;
			}

			return true;
		}


		void summon_effect(pk::unit@ unit)
		{

			string 관우_name = pk::decode(pk::get_name(person_关羽));
			string 장비_name = pk::decode(pk::get_name(person_张飞));
			string 제갈량_name = pk::decode(pk::get_name(person_诸葛亮));

			string 장익덕 = "张翼德";
			string 와룡 = "卧龙";

			int 관우소환_발동확률 = 0;
			int 장비소환_발동확률 = 0;
			int 제갈량소환_발동확률 = 0;

			if (관우소환_발동)
			{
				if (src_leader.get_id() == 무장_관평 or src_leader.get_id() == 무장_관흥
					or src_leader.get_id() == 무장_관색)
				{
					관우소환_발동확률 = 40;
				}

				if (pk::rand_bool(관우소환_발동확률))
				{
					tempPerson.face = 武将_关羽;

					int n = 2;

					set_person(n);

					pk::point emptyPos = getEmptyPos();
					pk::unit@ ghost_unit = pk::create_unit(pk::get_building(pk::get_service(src_unit)), tempPerson, null, null, 1, 兵器_枪, 兵器_走舸, 1, 1, emptyPos);
					pk::set_energy(ghost_unit, 100);
					ghost_unit.action_done = true;

					if (pk::is_in_screen(emptyPos)) { pk::create_effect(0x4b, emptyPos); }

					pk::play_voice(0x37, src_leader);
					pk::play_se(0x39, src_pos_);
					pk::create_effect(0x3a, src_pos_);

					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode("小子们! 见识我的青龙偃月刀吧!"), person_关羽); break;
					case 1: pk::message_box(pk::encode("以义传魂，以武入圣!"), person_关羽); break;
					}

					발동성공 = true;
					pk::say(pk::encode("再会了..."), tempPerson);
					pk::kill(ghost_unit);
					pk::reset(tempPerson);
				}
				else { 발동실패 = true; }

			} // if (관우소환_발동)

			if (장비소환_발동)
			{
				if (src_leader.get_id() == 무장_장포2 or src_leader.get_id() == 무장_장소2)
				{
					장비소환_발동확률 = 40;
				}

				if (pk::rand_bool(장비소환_발동확률))
				{
					tempPerson.face = 武将_张飞;

					int n = 1;

					set_person(n);

					pk::point emptyPos = getEmptyPos();
					pk::unit@ ghost_unit = pk::create_unit(pk::get_building(pk::get_service(src_unit)), tempPerson, null, null, 1, 兵器_枪, 兵器_走舸, 1, 1, emptyPos);
					pk::set_energy(ghost_unit, 100);
					ghost_unit.action_done = true;

					if (pk::is_in_screen(emptyPos)) { pk::create_effect(0x4b, emptyPos); }

					pk::play_voice(0x37, src_leader);
					pk::play_se(0x39, src_pos_);
					pk::create_effect(0x3a, src_pos_);

					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode("小子们!见识下丈八蛇矛吧!"), person_张飞); break;
					case 1: pk::message_box(pk::encode(pk::format("燕人\x1b[1x{}\x1b[0x在此!", 장익덕)), person_张飞); break;
					}

					발동성공 = true;
					pk::say(pk::encode("再会了..."), tempPerson);
					pk::kill(ghost_unit);
				}
				else { 발동실패 = true; }

			} // if (장비소환_발동)

			if (제갈량소환_발동)
			{
				if (src_leader.get_id() == 무장_제갈첨 or src_leader.get_id() == 무장_강유)
				{
					제갈량소환_발동확률 = 40;
				}

				if (pk::rand_bool(제갈량소환_발동확률))
				{
					tempPerson.face = 武将_诸葛亮;

					int n = 45;

					set_person(n);

					pk::point emptyPos = getEmptyPos();
					pk::unit@ ghost_unit = pk::create_unit(pk::get_building(pk::get_service(src_unit)), tempPerson, null, null, 1, 兵器_枪, 兵器_走舸, 1, 1, emptyPos);
					pk::set_energy(ghost_unit, 100);
					ghost_unit.action_done = true;

					if (pk::is_in_screen(emptyPos)) { pk::create_effect(0x4b, emptyPos); }

					pk::play_voice(0x37, src_leader);
					pk::play_se(0x39, src_pos_);
					pk::create_effect(0x3a, src_pos_);

					switch (pk::rand(2))
					{
					case 0: pk::message_box(pk::encode("斗转星移，七星借命."), person_诸葛亮); break;
					case 1: pk::message_box(pk::encode("太极生两仪，两仪生四象，四象生八卦."), person_诸葛亮); break;
					}

					발동성공 = true;


					pk::say(pk::encode("再会了..."), tempPerson);
					pk::kill(ghost_unit);

				}
				else { 발동실패 = true; }

			} // if (제갈량소환_발동)


			array<pk::point> arr = pk::range(src_pos_, min_범위, max_범위);

			for (int j = 0; j < int(arr.length); j++)
			{
				if (관우소환_발동 and 발동성공)
				{
					pk::unit@ dst = pk::get_unit(arr[j]);

					if (dst is null or !pk::is_enemy(src_unit, dst)) continue;

					pk::person@ dst_leader = pk::get_person(dst.leader);

					if (dst_leader.shoubyou == 상병_건강)
					{
						switch (pk::rand(2))
						{
						case 0: dst_leader.shoubyou = 상병_경증; dst_leader.update(); break;
						case 1: dst_leader.shoubyou = 상병_중증; dst_leader.update(); break;
						}
					}

					if (dst.troops > 적군사기설정_관우소환)
					{
						pk::set_energy(dst, 적군사기설정_관우소환);
					}

					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode(pk::format("这是 \x1b[1x{}\x1b[0x 的英灵吗？!", 관우_name)), dst_leader); break;
					case 1: pk::say(pk::encode(pk::format("不好，武圣\x1b[1x{}\x1b[0x居然显灵了!", 관우_name)), dst_leader); break;
					}

				} // if (관우소환_발동 and 발동성공)

				if (장비소환_발동 and 발동성공)
				{
					pk::unit@ dst = pk::get_unit(arr[j]);

					if (dst is null or !pk::is_enemy(src_unit, dst)) continue;

					pk::person@ dst_leader = pk::get_person(dst.leader);

					if (dst.troops > 적군사기설정_장비소환)
					{
						pk::set_energy(dst, 적군사기설정_장비소환);
					}

					if (dst.status == 部队状态_通常)
					{
						pk::set_status(dst, src_unit, 部队状态_伪报, 2, true);
					}

					switch (pk::rand(3))
					{
					case 0: pk::say(pk::encode(pk::format("哇，\x1b[1x{}\x1b[0x杀过来了!", 장비_name)), dst_leader); break;
					case 1: pk::say(pk::encode(pk::format("!! 这是\x1b[1x万人敌{}\x1b[0x?!", 장비_name)), dst_leader); break;
					case 2: pk::say(pk::encode("好恐怖的声音..!"), dst_leader);  break;
					}

				} // if (장비소환_발동 and 발동성공)

				if (제갈량소환_발동 and 발동성공)
				{
					pk::unit@ dst = pk::get_unit(arr[j]);

					if (dst is null) continue;

					// 적군인 경우 
					if (pk::is_enemy(src_unit, dst))
					{
						pk::person@ dst_leader = pk::get_person(dst.leader);

						if (dst.status == 部队状态_通常)
						{
							pk::set_status(dst, src_unit, 部队状态_混乱, 2, true);
						}

						switch (pk::rand(3))
						{
						case 0: pk::say(pk::encode(pk::format("中计了，\x1b[1x{}\x1b[0x竟还活着?!", 제갈량_name)), dst_leader); break;
						case 1: pk::say(pk::encode(pk::format("这! 是\x1b[1x{}\x1b[0x显灵了?!", 제갈량_name)), dst_leader); break;
						case 2: pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x复活了?!", 와룡)), dst_leader); break;
						}
					}

					// 아군인 경우 
					if (!pk::is_enemy(src_unit, dst))
					{
						pk::person@ dst_leader = pk::get_person(dst.leader);

						if (dst.status != 部队状态_通常)
						{
							pk::set_status(dst, src_unit, 部队状态_通常, 2, true);
						}

						switch (pk::rand(3))
						{
						case 0: pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x来帮我们了嘛!", 와룡)), dst_leader); break;
						case 1: pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x的英灵出现了!", 와룡)), dst_leader); break;
						case 2: pk::say(pk::encode(pk::format("有\x1b[1x{}\x1b[0x大人在，我们赢定了!", 와룡)), dst_leader); break;
						}
					}

				} // if (제갈량소환_발동 and 발동성공)

			} // for 


			if (관우소환_발동 and 발동성공)
			{
				if (src_leader.get_id() == 무장_관평 or src_leader.get_id() == 무장_관흥
					or src_leader.get_id() == 무장_관색)
				{
					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode("父亲大人回来了!"), src_leader);  break;
					case 1: pk::say(pk::encode("父亲~!"), src_leader);  break;
					}
				}
				else
				{
					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode("关圣显灵了"), src_leader); break;
					case 1: pk::say(pk::encode(pk::format("有\x1b[1x{}\x1b[0x将军在，我们攻无不克!", 관우_name)), src_leader); break;
					}
				}

				관우소환_발동 = false;
				발동성공 = false;
			}

			if (장비소환_발동 and 발동성공)
			{
				if (src_leader.get_id() == 무장_장포2 or src_leader.get_id() == 무장_장소2)
				{
					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode("父亲大人来帮我了!"), src_leader);  break;
					case 1: pk::say(pk::encode("谢谢您，父亲大人!"), src_leader);  break;
					}
				}
				else
				{
					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x将军的英灵来救我了..!", 장비_name)), src_leader); break;
					case 1: pk::say(pk::encode(pk::format("有\x1b[1x{}\x1b[0x帮助，我们必能杀出重围!", 장비_name)), src_leader); break;
					}
				}

				장비소환_발동 = false;
				발동성공 = false;
			}

			if (제갈량소환_발동 and 발동성공)
			{
				if (src_leader.get_id() == 무장_제갈첨)
				{
					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode("父亲的英灵，显现了!"), src_leader);  break;
					case 1: pk::say(pk::encode("父亲, 谢谢!"), src_leader);  break;
					}
				}
				else
				{
					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x的英灵来帮我了..!", 와룡)), src_leader); break;
					case 1: pk::say(pk::encode(pk::format("有\x1b[1x{}\x1b[0x英灵的帮助，我军必当胜利!", 와룡)), src_leader); break;
					}
				}

				제갈량소환_발동 = false;
				발동성공 = false;
			}

			if (발동실패)
			{
				switch (pk::rand(2))
				{
				case 0: pk::say(pk::encode("看来我的诚意还不够.."), src_leader);  break;
				case 1: pk::say(pk::encode("召唤失败了嘛..!"), src_leader);  break;
				}

				관우소환_발동 = false;
				장비소환_발동 = false;
				제갈량소환_발동 = false;
				발동실패 = false;
			}

		} // void summon_effect(pk::unit@ unit)

		void set_person(int n)
		{
			@tempPerson = pk::create_bandit(pk::get_person(744));
			int district_num = pk::get_district_id(force, 1);

			pk::set_district(tempPerson, district_num);
			tempPerson.mibun = 身份_一般;
			tempPerson.birth = pk::get_year() - 30;
			tempPerson.death = pk::min(353, tempPerson.birth + 99);
			tempPerson.appearance = pk::get_year() - 10;
			tempPerson.tekisei[병종_창병] = 적성_S;

			// 관우 부대 능력 설정
			if (n == 2)
			{
				tempPerson.sei = pk::encode("关"); // 성
				tempPerson.mei = pk::encode("羽"); // 명
				tempPerson.base_stat[武将能力_统率] = 95;
				tempPerson.base_stat[武将能力_武力] = 97;
				tempPerson.base_stat[武将能力_智力] = 75;
				tempPerson.base_stat[武将能力_政治] = 62;
				tempPerson.base_stat[武将能力_魅力] = 93;
				tempPerson.skill = 특기_신장;
			}

			// 장비 부대 능력 설정
			if (n == 1)
			{
				tempPerson.sei = pk::encode("张"); // 성
				tempPerson.mei = pk::encode("飞"); // 명
				tempPerson.base_stat[武将能力_统率] = 85;
				tempPerson.base_stat[武将能力_武力] = 98;
				tempPerson.base_stat[武将能力_智力] = 30;
				tempPerson.base_stat[武将能力_政治] = 22;
				tempPerson.base_stat[武将能力_魅力] = 45;
				tempPerson.skill = 특기_투신;
			}

			// 제갈량 부대 능력 설정
			if (n == 45)
			{
				tempPerson.sei = pk::encode("诸葛"); // 성
				tempPerson.mei = pk::encode("亮"); // 명
				tempPerson.base_stat[武将能力_统率] = 92;
				tempPerson.base_stat[武将能力_武力] = 38;
				tempPerson.base_stat[武将能力_智力] = 100;
				tempPerson.base_stat[武将能力_政治] = 95;
				tempPerson.base_stat[武将能力_魅力] = 92;
				tempPerson.skill = 특기_신산;
			}

			tempPerson.update();

		}

		pk::point getEmptyPos()
		{
			array<pk::point> arr = pk::range(src_pos_, min_범위, max_범위);

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


	} // class Main

	Main main;

} // namespace
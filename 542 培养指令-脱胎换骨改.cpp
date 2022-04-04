// ## 2022/03/11 # 江东新风 # 大幅改写写法，缩减内容，适性脱胎换骨调整 ##
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/09/21 # 江东新风 # 修正港口脱胎换骨菜单无效的bug ##
// ## 2020/08/09 # 氕氘氚 # 加入【培养】菜单 ##
/*
@제작자: HoneyBee
@설명: 능력치 50 미만인 무장의 능력치를 RANDOM으로 재설정 및 병종적성을 교환할 수 있는 SCRIPT

※ 무장 능력치를 30 ~ 69 사이의 수치로 재설정

*/

namespace 脱胎换骨
{

	// ================ CUSTOMIZE ================
	const bool 能力培养_开启 = true;
	const bool 适性培养_开启 = true;

	const int TP_COST = 100; 		// 기교P 수치 결정
	const int ACTION_COST = 70;     // 행동력 필요량
	const int GOLD_COST = 1500;     // 금 필요량

	const int stat_limit = 50;      // 환골탈태 가능한 무장 최대 능력치 제한 설정 (기본 설정 - 50)
	const int tekisei_limit = 4;  //适性s及以下可以脱胎换骨

	const int min_stat = 30;        // 변경 가능한 최소 능력치를 설정 (기본 설정 - 30)
	const int max_stat = 69;        // 변경 가능한 최대 능력치를 설정 (기본 설정 - 69)

	const int min_tekisei_exp = 0;        //随机数250为1级,1000就是4级
	const int max_tekisei_exp = 1000;       

	const int 适性交换成功率 = 50;        // 적성이 교환될 확률 설정 (기본 설정 - 50)

	array<string> temp_sei = { "沐","天","碧","季","麒","梅" };
	array<string> temp_mei = { "雨","照","落","子","剩","西" };
	array<string> desc_stat_msg = { "兵法","武技","经论","政论","修身法" };
	array<string> desc_tekisei_msg = { "枪阵","戟阵","弩阵","骑阵","攻城法","水战法" };
	array<int> temp_face = {2001,2004,2052,2055,2083,765};

	const int KEY = pk::hash("脱胎换骨");

	// ===========================================


	class Main
	{
		bool 调试模式 = false;
		pk::building@ building_;
		int check_result_;
		int change_type = 0;

		pk::person@ scen_actor_;
		pk::person@ 仙人;
		pk::person@ 弟子;
		int debate_result_;
		int debate_type_;
		Main()
		{
			add_menu();
		}

		void add_menu()
		{

			pk::menu_item item_脱胎換骨;
			item_脱胎換骨.menu = global_menu::菜单_培养;
			//ch::u8debug(pk::format("menu: qu chu te ji {}", global_menu::菜单_培养));
			item_脱胎換骨.init = pk::building_menu_item_init_t(init);
			item_脱胎換骨.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
			item_脱胎換骨.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("脱胎换骨"); });
			item_脱胎換骨.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("点击展开或收起"); });
			// item_脱胎換骨.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
			item_脱胎換骨.handler = pk::menu_item_handler_t(handler_脱胎換骨);
			pk::add_menu_item(item_脱胎換骨);

			pk::menu_item item;

			if (能力培养_开启)
			{
				item.menu = global_menu::菜单_培养;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_stat);
				item.get_text = pk::menu_item_get_text_t(getText_stat);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_stat);
				item.handler = pk::menu_item_handler_t(handler_stat);
				pk::add_menu_item(item);
			}

			if (适性培养_开启)
			{
				item.menu = global_menu::菜单_培养;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_tekisei);
				item.get_text = pk::menu_item_get_text_t(getText_tekisei);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_tekisei);
				item.handler = pk::menu_item_handler_t(handler_tekisei);
				pk::add_menu_item(item);
			}
		}

		void init(pk::building@ building)
		{
			@building_ = @building;
			check_result_ = check_avaliable(building);
		}

		int getImageID_1249()   // 주황색
		{
			return 1249;
		}


		bool isMenuVisible()
		{
			if (global_menu::培养_展开_当前 != global_menu::培养_展开_脱胎換骨) return false;
			if (!pk::is_valid_base_id(building_.get_id())) return false;
			return building_.get_force_id() == pk::get_current_turn_force_id();
		}

		bool handler_脱胎換骨()
		{
			global_menu::培养_展开_当前 = (global_menu::培养_展开_当前 == global_menu::培养_展开_脱胎換骨) ? 0 : global_menu::培养_展开_脱胎換骨;
			return false;
		}

		//---------------------------------------------------------------------------
		// 能力脱胎换骨
		//---------------------------------------------------------------------------

		string getText_stat()
		{
			return pk::encode("(能力)变化");
		}

		string getDesc_stat()
		{
			switch (check_result_)
			{
			case 1: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			case 2: return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			case 3: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			case 0: return pk::encode(pk::format("让(能力)脱胎换骨.(技巧P {} )", TP_COST));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		bool isEnabled_stat()
		{
			if (check_result_ != 0) return false;
			else return true;
		}

		int check_avaliable(pk::building@ building)//之所以加入list是为了ai调用时不用重复计算，玩家菜单稍微多点操作问题不大
		{
			//pk::trace(pk::format("3.1:{}{}{}{}", pk::enemies_around(building), list.count, pk::get_district(building.get_district_id()).ap < ACTION_COST, pk::get_gold(building) < GOLD_COST));
			if (pk::get_gold(building) < GOLD_COST) return 1;
			else if (pk::get_force(building.get_force_id()).tp < TP_COST) return 2;
			else if (pk::get_district(building.get_district_id()).ap < ACTION_COST) return 3;
			else return 0;
		}

		bool handler_stat()
		{
			int choise = pk::choose({ pk::encode("  统率  "), pk::encode("  武力  "), pk::encode("  智力  "), pk::encode("  政治  "), pk::encode("  魅力  "), pk::encode("  取消  ") }, pk::encode("请选择要脱胎换骨的能力类型。"), pk::get_person(武将_文官));
			if (choise == 5) return false;
			int stat_type = choise;
			pk::list<pk::person@> person_list = getTargetList_能力(building_.get_force_id(), stat_type);
			if (person_list.count == 0) return false;


			// 실행무장 선택하기
			pk::list<pk::person@> person_sel;
			person_list.sort(function(a, b)
			{
				int a_stat = a.stat[武将能力_统率] + a.stat[武将能力_武力] + a.stat[武将能力_智力] + a.stat[武将能力_政治] + a.stat[武将能力_魅力];
				int b_stat = b.stat[武将能力_统率] + b.stat[武将能力_武力] + b.stat[武将能力_智力] + b.stat[武将能力_政治] + b.stat[武将能力_魅力];
				return (a_stat > b_stat); 
			});

			person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode(pk::format("选择要改变({})的武将.", ch::get_stat_name(stat_type))), person_list, 1, 1, person_list,37);
			if (person_sel.count == 0) return false; // 미선택 시 취소 종료


			return run_order(person_sel, building_, stat_type,0);
		}

		bool run_order(pk::list<pk::person@> person_sel, pk::building@ building0, int type_id,int change_type)
		{
			if (building0 is null) return false;
			if (person_sel[0] is null) return false;
			pk::person@ actor = @person_sel[0];
			pk::person@ person0 = pk::get_person(武将_左慈);
			pk::person@ person_1 = @get_debate_person(actor, type_id, change_type);
			int debate_result = 1;
			bool win = false;
			//******************************剧情对话********************************
			if (ch::player_check(actor))
			{
				string desc_info = "";
				if (change_type == 0) desc_info = desc_stat_msg[type_id];
				else desc_info = desc_tekisei_msg[type_id];
				pk::message_box(pk::encode(pk::format("上仙, 我听闻您这收藏有一卷仙家{}, 特来请教.", desc_info)), actor);
				pk::message_box(pk::encode(pk::format("不知能否将这{}传于我.", desc_info)), actor);
				pk::message_box(pk::encode(pk::format("仙家的{}, 凡人难以领悟.", desc_info)), person0);
				pk::message_box(pk::encode("反而可能会产生很大的副作用."), person0);
				pk::message_box(pk::encode("即便如此我也想试一试!"), actor);
				pk::message_box(pk::encode("也行...且让你试他一试."), person0);
				pk::message_box(pk::encode(pk::format("你若能与我弟子论{}胜之, 我便传你.", desc_info)), person0);
				pk::message_box(pk::encode("感谢."), actor);
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x你在何处!",pk::decode(pk::get_name(person_1)))), person0);
				pk::message_box(pk::encode("师傅,我正在进行修行."), person_1);
				pk::message_box(pk::encode(pk::format("你跟这人切磋下{}吧..", desc_info)), person0);
				pk::message_box(pk::encode("好...这位真能成为我的对手嘛?"), person_1);
				if (change_type == 1 or type_id == 武将能力_武力)//改变的是武力或者适性时用单挑
				{
					@scen_actor_ = @actor;
					@弟子 = @person_1;
					debate_result_ = 1;
					pk::scene(pk::scene_t(scen_duel)); //单挑不写成剧情的话，会在结束后闪退
					debate_result = debate_result_;
				}
				else
				{
					@scen_actor_ = @actor;
					@弟子 = @person_1;
					debate_result_ = 1;
					debate_type_ = type_id;
					scen_debate();
					debate_result = debate_result_;
				}
				
				if (debate_result == 0)
				{
					pk::message_box(pk::encode("我输了, 你比我想象的要出色不少."), person_1);
					pk::message_box(pk::encode("竟能战胜我徒弟...这人确有可取之处."), person0);
					pk::message_box(pk::encode(pk::format("哈哈哈,我赢了~ 请传授我仙家{}.", desc_info)), actor);
					pk::message_box(pk::encode("确定吗？即使这可能会有副作用."), person0);
					pk::message_box(pk::encode("我确定."), actor);
					pk::message_box(pk::encode("好, 那就跟我来吧."), person0);

					if (change_type == 0) pk::message_box(pk::encode(pk::format("选中武将{}已变更.", ch::get_stat_name(type_id))));
					else pk::message_box(pk::encode(pk::format("选中武将{}适性已变更.", ch::get_tekisei_name(type_id))));
					win = true;
				}
				else
				{
					pk::message_box(pk::encode("果然很弱呀."), person_1);
					pk::message_box(pk::encode(pk::format("连这孩子也打不过, 向你传授{}也枉然.", desc_info)), person0);
					pk::message_box(pk::encode("真是可惜..."), actor);
					pk::message_box(pk::encode("虽未达成心愿, 但还是要谢谢你."), actor);
					win = false;
				}
				pk::reset(person_1);
			}
			else//电脑执行的时候
			{
				if (change_type == 0)
				{
					if (actor.stat[type_id] > 30 and pk::rand_bool(50)) win = true;
					else win = false;
				}
				else
				{
					if (actor.stat[武将能力_武力] > 90 and pk::rand_bool(50)) win = true;
					else win = false;
				}
			}

			//******************************剧情对话完********************************
			if (win)
			{
				if (change_type == 0)
				{
					actor.base_stat[type_id] = ch::randint(min_stat, max_stat);
					actor.update();
				}
				else
				{
					int exp = ch::randint(min_tekisei_exp, max_tekisei_exp);
					int level = exp / 250;
					uint8 remain_exp = exp - level * 250;
					actor.tekisei[type_id] = level;
					actor.heishu_exp[type_id] = remain_exp;
					//actor.update();
				}
			}
			ch::add_tp(pk::get_force(building0.get_force_id()), -TP_COST, building0.get_pos());
			// 행동력 감소.
			auto district = pk::get_district(building0.get_district_id());
			pk::add_ap(district, -ACTION_COST);
			// 실행무장 행동완료
			person_sel[0].action_done = true;
			// 금 감소
			pk::add_gold(building0, -GOLD_COST, true);
			return true;

		}

		pk::list<pk::person@> getTargetList_能力(int force_id, int stat_type)
		{
			pk::list<pk::person@> list;
			//int force_id = force.get_id();
			if (pk::is_valid_force_id(force_id))
			{
				for (int i = 0; i < 非贼武将_末; i++)
				{
					pk::person@ person = pk::get_person(i);
					if (pk::is_alive(person))
					{
						if (person.get_force_id() == force_id)
						{
							if (!pk::is_absent(person) and !pk::is_unitize(person) and person.mibun != 身份_俘虏 and (person.stat[stat_type] < stat_limit))
								list.add(person);
						}
					}
				}
			}
			return list;
		}

		pk::person@ get_debate_person(pk::person@dst, int stat_type,int change_type)
		{
			//pk::person@ person_1 = pk::get_person(무장_남); // 천시로 
			pk::person@ temp_person = pk::create_bandit(pk::get_person(武将_童子));
			temp_person.sei = pk::encode(temp_sei[stat_type]); // 성
			temp_person.mei = pk::encode(temp_mei[stat_type]); // 명
			temp_person.face = temp_face[stat_type];;
			temp_person.voice = 音声_刚胆男;
			if (change_type == 0)
			{
				temp_person.base_stat[stat_type] = dst.base_stat[stat_type] + 5 + pk::rand(10);
				temp_person.stat[stat_type] = dst.stat[stat_type] + 5 + pk::rand(10);
			}
			else
			{
				temp_person.base_stat[武将能力_武力] = dst.base_stat[武将能力_武力] + pk::rand(10);
				temp_person.stat[武将能力_武力] = dst.stat[武将能力_武力] + pk::rand(10);
			}
			// 무장 조형
			temp_person.body[0] = 74;
			temp_person.body[1] = 74;
			temp_person.body[2] = 74;
			temp_person.body[3] = 74;
			temp_person.body[4] = 74;
			temp_person.body[5] = 74;

			temp_person.wajutsu_daikatsu = true;
			temp_person.wajutsu_kiben = true;
			temp_person.wajutsu_mushi = true;
			temp_person.wajutsu_chinsei = true;
			temp_person.wajutsu_gyakujou = true;

			temp_person.update();
			return temp_person;			
		}

		//---------------------------------------------------------------------------
		// 适性脱胎换骨
		//---------------------------------------------------------------------------

		string getText_tekisei()
		{
			return pk::encode("(适性)变化");
		}

		string getDesc_tekisei()
		{
			switch (check_result_)
			{
			case 1: return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			case 2: return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			case 3: return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			case 0: return pk::encode(pk::format("让(适性)脱胎换骨.(技巧P {} )", TP_COST));
			default:
				return pk::encode("");
			}
			return pk::encode("");
		}

		bool isEnabled_tekisei()
		{
			if (check_result_ != 0) return false;
			else return true;
		}

		bool handler_tekisei()
		{
			int choise = pk::choose({ pk::encode("  枪兵  "), pk::encode("  戟兵  "), pk::encode("  弩兵  "), pk::encode("  骑兵  "), pk::encode("  兵器  "), pk::encode("  水军  ") }, pk::encode("请选择要脱胎换骨的适性类型。"), pk::get_person(武将_文官));
			//if (choise == 5) return false;
			int tekisei_type = choise;
			pk::list<pk::person@> person_list = getTargetList_适性(building_.get_force_id(), tekisei_type);
			if (person_list.count == 0) return false;


			// 실행무장 선택하기
			pk::list<pk::person@> person_sel;
			person_list.sort(function(a, b)
			{
				int a_stat = a.tekisei[兵种_枪兵] + a.tekisei[兵种_戟兵] + a.tekisei[兵种_弩兵] + a.tekisei[兵种_骑兵] + a.tekisei[兵种_兵器] + a.tekisei[兵种_水军];
				int b_stat = b.tekisei[兵种_枪兵] + b.tekisei[兵种_戟兵] + b.tekisei[兵种_弩兵] + b.tekisei[兵种_骑兵] + b.tekisei[兵种_兵器] + b.tekisei[兵种_水军];
				return (a_stat < b_stat);
			});

			person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode(pk::format("选择要改变({}适性)的武将.", ch::get_tekisei_name(tekisei_type))), person_list, 1, 1, person_list, 37);
			if (person_sel.count == 0) return false; // 미선택 시 취소 종료


			return run_order(person_sel, building_, tekisei_type,1);
		}

		pk::list<pk::person@> getTargetList_适性(int force_id, int tekisei_type)
		{
			pk::list<pk::person@> list;
			//int force_id = force.get_id();
			if (pk::is_valid_force_id(force_id))
			{
				for (int i = 0; i < 非贼武将_末; i++)
				{
					pk::person@ person = pk::get_person(i);
					if (pk::is_alive(person))
					{
						if (person.get_force_id() == force_id)
						{
							if (!pk::is_absent(person) and !pk::is_unitize(person) and person.mibun != 身份_俘虏 and (person.tekisei[tekisei_type] < tekisei_limit))
								list.add(person);
						}
					}
				}
			}
			return list;
		}
		//---------------------------------------------------------------------------
		// 适性脱胎换骨结束
		//---------------------------------------------------------------------------
		void scen_duel()
		{
			bool show_sub_strength = pk::get_show_sub_strength();
			pk::set_show_sub_strength(false);
			pk::set_person_sub_strength(scen_actor_.get_id(), 30);
			pk::unit@dummy_unit;
			debate_result_ = pk::duel(dummy_unit, dummy_unit, scen_actor_, null, null, 弟子, null, null, pk::is_player_controlled(scen_actor_), false, 0, true).first;
			pk::set_show_sub_strength(show_sub_strength);
			pk::set_person_sub_strength(scen_actor_.get_id(), 0);
			return;
		}

		void scen_debate()
		{
			debate_result_ = pk::debate(scen_actor_, 弟子, pk::is_player_controlled(scen_actor_), false, false, true, debate_type_).first;
			return;
		}
	}

	Main main;
}
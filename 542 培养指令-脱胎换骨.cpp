// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/09/21 # 江东新风 # 修正港口脱胎换骨菜单无效的bug ##
// ## 2020/08/09 # 氕氘氚 # 加入【培养】菜单 ##
/*
@제작자: HoneyBee
@설명: 능력치 50 미만인 무장의 능력치를 RANDOM으로 재설정 및 병종적성을 교환할 수 있는 SCRIPT

※ 무장 능력치를 30 ~ 69 사이의 수치로 재설정

*/

namespace 환골탈태
{

	// ================ CUSTOMIZE ================

	const bool 통솔변화_활성화 = true;    // true = on, false = off
	const bool 무력변화_활성화 = true;    // true = on, false = off
	const bool 지력변화_활성화 = true;    // true = on, false = off
	const bool 정치변화_활성화 = true;    // true = on, false = off
	const bool 매력변화_활성화 = true;    // true = on, false = off
	const bool 적성교환_활성화 = true;    // true = on, false = off

	const int TP_COST = 100; 		// 기교P 수치 결정
	const int ACTION_COST = 70;     // 행동력 필요량
	const int GOLD_COST = 1500;     // 금 필요량

	const int stat_limit = 50;      // 환골탈태 가능한 무장 최대 능력치 제한 설정 (기본 설정 - 50)

	const int min_stat = 30;        // 변경 가능한 최소 능력치를 설정 (기본 설정 - 30)
	const int max_stat = 69;        // 변경 가능한 최대 능력치를 설정 (기본 설정 - 69)

	const int 적성교환성공률 = 50;        // 적성이 교환될 확률 설정 (기본 설정 - 50)

	const int KEY = pk::hash("환골탈태");

	// ===========================================


	class Main
	{

		pk::building@ building_;
		pk::building@ taishu_building;
		pk::force@ force_;
		pk::person@ taishu;
		pk::person@ actor;
		pk::person@ person0; // 좌자
		//pk::city@ city;
		uint16 building_gold;
		int debate_result;
		int duel_result;
		array<int> arr_stat = { 10, 20, 30, 40, 50 };

		// 건물 메뉴 도시
		int menu_building_id_;

		// 건물 메뉴 세력
		int menu_force_id_;

		Main()
		{

			int parent;

			pk::menu_item item_脱胎換骨;
			item_脱胎換骨.menu = global_menu::菜单_培养;
			//ch::u8debug(pk::format("menu: qu chu te ji {}", global_menu::菜单_培养));
			item_脱胎換骨.init = pk::building_menu_item_init_t(init);
			item_脱胎換骨.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
			item_脱胎換骨.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("脱胎换骨"); });
			item_脱胎換骨.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("点击展开或收起"); });
			// item_脱胎換骨.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
			item_脱胎換骨.handler = pk::menu_item_handler_t(handler_脱胎換骨);
			parent = pk::add_menu_item(item_脱胎換骨);


			pk::menu_item item;

			if (통솔변화_활성화)
			{
				item.menu = global_menu::菜单_培养;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_통솔);
				item.get_text = pk::menu_item_get_text_t(getText_통솔);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_통솔);
				item.handler = pk::menu_item_handler_t(handler_통솔);
				pk::add_menu_item(item);
			}

			if (무력변화_활성화)
			{
				item.menu = global_menu::菜单_培养;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_무력);
				item.get_text = pk::menu_item_get_text_t(getText_무력);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_무력);
				item.handler = pk::menu_item_handler_t(handler_무력);
				pk::add_menu_item(item);
			}

			if (지력변화_활성화)
			{
				item.menu = global_menu::菜单_培养;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_지력);
				item.get_text = pk::menu_item_get_text_t(getText_지력);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_지력);
				item.handler = pk::menu_item_handler_t(handler_지력);
				pk::add_menu_item(item);
			}

			if (정치변화_활성화)
			{
				item.menu = global_menu::菜单_培养;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_정치);
				item.get_text = pk::menu_item_get_text_t(getText_정치);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_정치);
				item.handler = pk::menu_item_handler_t(handler_정치);
				pk::add_menu_item(item);
			}

			if (매력변화_활성화)
			{
				item.menu = global_menu::菜单_培养;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_매력);
				item.get_text = pk::menu_item_get_text_t(getText_매력);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_매력);
				item.handler = pk::menu_item_handler_t(handler_매력);
				pk::add_menu_item(item);
			}

			if (적성교환_활성화)
			{
				item.menu = global_menu::菜单_培养;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_적성);
				item.get_text = pk::menu_item_get_text_t(getText_적성);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_적성);
				item.handler = pk::menu_item_handler_t(handler_적성);
				pk::add_menu_item(item);
			}

		}

		void init(pk::building@ building)
		{

			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@taishu = pk::get_person(pk::get_taishu_id(building));
			//@city = pk::building_to_city(building);
			building_gold = pk::get_gold(building);

			@person0 = pk::get_person(무장_좌자);
			person0.update();

			if (pk::is_alive(taishu))
				@taishu_building = pk::get_building(taishu.service);

			menu_building_id_ = -1;

			if (building !is null)
				menu_building_id_ = building.get_id();
			menu_force_id_ = building.get_force_id();

		}



		int getImageID_1249()   // 주황색
		{
			return 1249;
		}


		bool isMenuVisible()
		{
			if (global_menu::培养_展开_当前 != global_menu::培养_展开_脱胎換骨) return false;
			if (building_.get_id() >= 据点_末) return false;
			return menu_building_id_ != -1 and menu_building_id_ < 87 and menu_force_id_ == pk::get_current_turn_force_id();
		}

		bool handler_脱胎換骨()
		{
			global_menu::培养_展开_当前 = (global_menu::培养_展开_当前 == global_menu::培养_展开_脱胎換骨) ? 0 : global_menu::培养_展开_脱胎換骨;
			return false;
		}

		//---------------------------------------------------------------------------
		// 통솔변화
		//---------------------------------------------------------------------------

		string getText_통솔()
		{
			return pk::encode("(统率)变化");
		}

		string getDesc_통솔()
		{
			if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("让(统率)脱胎换骨.(技巧P {} )", TP_COST));
		}

		bool isEnabled_통솔()
		{
			if (building_gold < GOLD_COST) return false;
			else if (force_.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		bool handler_통솔()
		{

			pk::random random(pk::rand());
			pk::list<pk::person@> person_list = getTargetList_통솔();

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("要让(统率)脱胎换骨嘛?"), taishu) == 1) return false;

			// 실행무장 선택하기
			if (pk::is_player_controlled(building_))
			{
				pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择要改变(统率)的武将."), person_list, 1, 1);
				if (person_sel.count == 0) return false; // 미선택 시 취소 종료
				@actor = pk::get_person(person_sel[0].get_id());
			}
			else
			{
				person_list.sort(function(a, b)
				{
					return (a.stat[武将能力_统率] > b.stat[武将能力_统率]); // 무장 정렬 (지력순)
				});
				@actor = pk::get_person(person_list[0].get_id());
			}

			pk::person@ person_1 = pk::get_person(무장_남); // 천시로 

			person_1.sei = pk::encode("沐"); // 성
			person_1.mei = pk::encode("雨"); // 명
			person_1.face = 2311;
			person_1.update();

			// 통솔변경 대화 
			pk::message_box(pk::encode("\x1b[1x左慈\x1b[0x先生我听闻这有一部传自神仙的兵法, 特来请教."), actor);
			pk::message_box(pk::encode("不知能否将这兵法传于我."), actor);
			pk::message_box(pk::encode("神仙的兵法, 凡人难以领悟."), person0);
			pk::message_box(pk::encode("反而可能会产生很大的副作用."), person0);
			pk::message_box(pk::encode("即便如此我也想试一试!"), actor);
			pk::message_box(pk::encode("也行...且让你试他一试."), person0);
			pk::message_box(pk::encode("你若能与我弟子论兵法胜之, 我便传你."), person0);
			pk::message_box(pk::encode("感谢."), actor);
			pk::message_box(pk::encode("\x1b[1x沐雨\x1b[0x你在何处!"), person0);
			pk::message_box(pk::encode("师傅,我正在进行训练."), person_1);
			pk::message_box(pk::encode("你跟这人谈谈兵法."), person0);
			pk::message_box(pk::encode("好...这位真能成为我的对手嘛?"), person_1);

			pk::scene(pk::scene_t(scene_설전1));

			if (debate_result == 0)
			{
				pk::message_box(pk::encode("我输了, 你比我想象的要出色不少."), person_1);
				pk::message_box(pk::encode("竟能战胜我徒弟这人比预计的要出色呀."), person0);
				pk::message_box(pk::encode("哈哈哈, 现在请传授我神仙兵法."), actor);
				pk::message_box(pk::encode("这可是约定好的."), actor);
				pk::message_box(pk::encode("确定嘛, 即使这可能会有副作用."), person0);
				pk::message_box(pk::encode("我确定."), actor);
				pk::message_box(pk::encode("好, 那就到这儿来."), person0);

				actor.base_stat[0] = random(min_stat, max_stat);
				pk::message_box(pk::encode("选中武将统率已变更."));

				actor.update();

			}
			else
			{
				pk::message_box(pk::encode("果然很弱呀."), person_1);
				pk::message_box(pk::encode("连这孩子也打不过, 向你传授兵法也枉然."), person0);
				pk::message_box(pk::encode("真是可惜..."), actor);
				pk::message_box(pk::encode("虽未达成心愿, 但还是要谢谢你."), actor);
				pk::message_box(pk::encode("好你也辛苦了."), person0);
				pk::message_box(pk::encode("人虽笨了点, 心肠倒不错."), person_1);
			}

			actor.action_done = true;

			// 기교 및 금 감소
			ch::add_tp(force_, -TP_COST, force_.get_pos());
			pk::add_gold(building_, -GOLD_COST, true);

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			return true;
		}


		//---------------------------------------------------------------------------
		// 무력변화
		//---------------------------------------------------------------------------

		string getText_무력()
		{
			return pk::encode("(武力)变化");  //허稿 좝稿 쐼뱐	甘稿
		}

		string getDesc_무력()
		{
			if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("让(武力)脱胎换骨.(技巧P {} )", TP_COST));
		}

		bool isEnabled_무력()
		{
			if (building_gold < GOLD_COST) return false;
			else if (force_.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		bool handler_무력()
		{

			pk::random random(pk::rand());
			pk::list<pk::person@> person_list = getTargetList_무력();

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("要让(武力)脱胎换骨嘛?"), taishu) == 1) return false;

			// 실행무장 선택하기
			if (pk::is_player_controlled(building_))
			{
				pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择要改变(武力)的武将."), person_list, 1, 1);
				if (person_sel.count == 0) return false; // 미선택 시 취소 종료
				@actor = pk::get_person(person_sel[0].get_id());
			}
			else
			{
				person_list.sort(function(a, b)
				{
					return (a.stat[武将能力_武力] > b.stat[武将能力_武力]); // 무장 정렬 (무력순)
				});
				@actor = pk::get_person(person_list[0].get_id());
			}

			pk::person@ person_1 = pk::get_person(무장_남); // 천시로 

			person_1.sei = pk::encode("天"); // 성
			person_1.mei = pk::encode("照"); // 명
			person_1.face = 2311;
			person_1.update();

			// 무력변경 대화 
			pk::message_box(pk::encode("\x1b[1x左慈\x1b[0x先生我听闻这有一部传自神仙的武技, 特来请教."), actor);  //
			pk::message_box(pk::encode("不知能否将这武技传于我."), actor);
			pk::message_box(pk::encode("神仙的武技, 凡人难以领悟."), person0);
			pk::message_box(pk::encode("反而可能会产生很大的副作用."), person0);
			pk::message_box(pk::encode("即便如此, 我也想试一试."), actor);
			pk::message_box(pk::encode("也行...且让你试他一试."), person0);
			pk::message_box(pk::encode("你若能与我弟子比武胜之, 我便传你."), person0);
			pk::message_box(pk::encode("感谢."), actor);
			pk::message_box(pk::encode("\x1b[1x天照\x1b[0x你在何处!"), person0);
			pk::message_box(pk::encode("师傅,我正在进行训练."), person_1);
			pk::message_box(pk::encode("你和这人切磋下武技"), person0);
			pk::message_box(pk::encode("好...这位真能成为我的对手吗?"), person_1);

			pk::scene(pk::scene_t(scene_일기토));

			if (duel_result == 0)
			{
				pk::message_box(pk::encode("我输了, 你比我想象的要出色不少."), person_1);
				pk::message_box(pk::encode("竟能战胜我徒弟, 这人比预计的要出色呀."), person0);
				pk::message_box(pk::encode("哈哈哈, 现在请传授我神仙武技."), actor);
				pk::message_box(pk::encode("这可是约定好的."), actor);
				pk::message_box(pk::encode("确定嘛, 即使这可能会有副作用."), person0);
				pk::message_box(pk::encode("我确定."), actor);
				pk::message_box(pk::encode("好, 那就到这儿来."), person0);

				actor.base_stat[1] = random(min_stat, max_stat);
				pk::message_box(pk::encode("选中武将武力已变更."));

				actor.update();
			}
			else
			{
				pk::message_box(pk::encode("果然很弱呀."), person_1);
				pk::message_box(pk::encode("我\x1b[1x天照的武技\x1b[0x凡人是难以企及的."), person_1);
				pk::message_box(pk::encode("连这孩子也打不过, 向你传授武技也枉然."), person0);
				pk::message_box(pk::encode("真是可惜.."), actor);
				pk::message_box(pk::encode("虽未达成心愿, 但还是要谢谢你."), actor);
				pk::message_box(pk::encode("好, 你也辛苦了."), person0);
				pk::message_box(pk::encode("人虽笨了点, 心肠倒不错."), person_1);
			}

			actor.action_done = true;

			// 기교 및 금 감소
			ch::add_tp(force_, -TP_COST, force_.get_pos());
			pk::add_gold(building_, -GOLD_COST, true);

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			return true;
		}


		//---------------------------------------------------------------------------
		// 지력변화
		//---------------------------------------------------------------------------

		string getText_지력()
		{
			return pk::encode("(智力)变化");
		}

		string getDesc_지력()
		{
			if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("让(智力)脱胎换骨.(技巧P {} )", TP_COST));
		}

		bool isEnabled_지력()
		{
			if (building_gold < GOLD_COST) return false;
			else if (force_.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		bool handler_지력()
		{

			pk::random random(pk::rand());
			pk::list<pk::person@> person_list = getTargetList_지력();

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("要让(智力)脱胎换骨吗?"), taishu) == 1) return false;

			// 실행무장 선택하기
			if (pk::is_player_controlled(building_))
			{
				pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择要改变(智力)的武将."), person_list, 1, 1);
				if (person_sel.count == 0) return false; // 미선택 시 취소 종료
				@actor = pk::get_person(person_sel[0].get_id());
			}
			else
			{
				person_list.sort(function(a, b)
				{
					return (a.stat[武将能力_智力] > b.stat[武将能力_智力]); // 무장 정렬 (지력순)
				});
				@actor = pk::get_person(person_list[0].get_id());
			}

			pk::person@ person_2 = pk::get_person(무장_여); // 마수미 

			person_2.sei = pk::encode("碧"); // 성
			person_2.mei = pk::encode("落"); // 명 biluo
			person_2.face = 2312;
			person_2.update();

			// 지력변경 대화 
			pk::message_box(pk::encode("\x1b[1x左慈\x1b[0x先生我听闻这有传自神仙的谋略, 特来请教."), actor);
			pk::message_box(pk::encode("不知能否将这谋略传于我."), actor);
			pk::message_box(pk::encode("神仙的谋略, 凡人难以领悟."), person0);
			pk::message_box(pk::encode("反而可能会产生很大的副作用."), person0);
			pk::message_box(pk::encode("即便如此, 我也想试一试."), actor);
			pk::message_box(pk::encode("也行...且让你试他一试."), person0);
			pk::message_box(pk::encode("你若能与我弟子斗智胜之, 我便传你."), person0);  //且좝
			pk::message_box(pk::encode("感谢."), actor);
			pk::message_box(pk::encode("\x1b[1x碧落\x1b[0x你在何处!"), person0);
			pk::message_box(pk::encode("师傅, 这次要教我什么?"), person_2);
			pk::message_box(pk::encode("你和这人比试下谋略."), person0);  //且	좝
			pk::message_box(pk::encode("好...这位真能成为我的对手吗?"), person_2);

			pk::scene(pk::scene_t(scene_설전2));

			if (debate_result == 0)
			{
				pk::message_box(pk::encode("我输了, 你比我想象的要出色不少."), person_2);
				pk::message_box(pk::encode("竟能战胜我徒弟, 这人比预计的要出色呀."), person0);
				pk::message_box(pk::encode("哈哈哈, 现在请传授我神仙谋略."), actor);
				pk::message_box(pk::encode("这可是约定好的."), actor);
				pk::message_box(pk::encode("确定嘛, 即使这可能会有副作用."), person0);
				pk::message_box(pk::encode("我确定."), actor);
				pk::message_box(pk::encode("好, 那就到这儿来."), person0);

				actor.base_stat[2] = random(min_stat, max_stat);
				pk::message_box(pk::encode("选中武将智力已变更."));

				actor.update();
			}
			else
			{
				pk::message_box(pk::encode("果然很弱呀."), person_2);
				pk::message_box(pk::encode("我\x1b[1x碧落的智略\x1b[0x凡人是难以企及的."), person_2);
				pk::message_box(pk::encode("连这孩子也打不过, 向你传授谋略也枉然."), person0);
				pk::message_box(pk::encode("真是可惜.."), actor);
				pk::message_box(pk::encode("虽未达成心愿, 但还是要谢谢你."), actor);
				pk::message_box(pk::encode("好你也辛苦了."), person0);
				pk::message_box(pk::encode("人虽笨了点, 心肠倒不错."), person_2);
			}

			actor.action_done = true;

			// 기교 및 금 감소
			ch::add_tp(force_, -TP_COST, force_.get_pos());
			pk::add_gold(building_, -GOLD_COST, true);

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			return true;
		}


		//---------------------------------------------------------------------------
		// 정치변화
		//---------------------------------------------------------------------------

		string getText_정치()
		{
			return pk::encode("(政治)变化");
		}

		string getDesc_정치()
		{
			if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("让(政治)脱胎换骨.(技巧P {} )", TP_COST));
		}

		bool isEnabled_정치()
		{
			if (building_gold < GOLD_COST) return false;
			else if (force_.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		bool handler_정치()
		{

			pk::random random(pk::rand());
			pk::list<pk::person@> person_list = getTargetList_정치();

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("要让(政治)脱胎换骨吗?"), taishu) == 1) return false;

			// 실행무장 선택하기
			if (pk::is_player_controlled(building_))
			{
				pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择要改变(政治)的武将."), person_list, 1, 1);
				if (person_sel.count == 0) return false; // 미선택 시 취소 종료
				@actor = pk::get_person(person_sel[0].get_id());
			}
			else
			{
				person_list.sort(function(a, b)
				{
					return (a.stat[武将能力_政治] > b.stat[武将能力_政治]); // 무장 정렬 (정치순)
				});
				@actor = pk::get_person(person_list[0].get_id());
			}

			pk::person@ person_2 = pk::get_person(무장_여); // 마수미 

			person_2.sei = pk::encode("云"); // 성
			person_2.mei = pk::encode("台"); // 명
			person_2.face = 2312;
			person_2.update();

			// 정치변경 대화 
			pk::message_box(pk::encode("\x1b[1x左慈\x1b[0x先生我听闻这有一部传自神仙的政略, 特来请教."), actor);
			pk::message_box(pk::encode("不知能否将这政略传于我."), actor);
			pk::message_box(pk::encode("神仙的政略, 凡人难以领悟."), person0);
			pk::message_box(pk::encode("反而可能会产生很大的副作用."), person0);
			pk::message_box(pk::encode("即便如此, 我也想试一试."), actor);
			pk::message_box(pk::encode("也行...且让你试他一试."), person0);
			pk::message_box(pk::encode("你若能与我弟子论政胜之, 我便传你."), person0);
			pk::message_box(pk::encode("感谢."), actor);
			pk::message_box(pk::encode("\x1b[1x云台\x1b[0x你在何处!"), person0);
			pk::message_box(pk::encode("师傅, 这次要教我什么?"), person_2);
			pk::message_box(pk::encode("你和这人进行论政."), person0);
			pk::message_box(pk::encode("好...这位真能成为我的对手吗?"), person_2);

			pk::scene(pk::scene_t(scene_설전2));

			if (debate_result == 0)
			{
				pk::message_box(pk::encode("我输了, 你比我想象的要出色不少."), person_2);
				pk::message_box(pk::encode("竟能战胜我徒弟, 这人比预计的要出色呀."), person0);
				pk::message_box(pk::encode("哈哈哈, 现在请传授我神仙政略."), actor);
				pk::message_box(pk::encode("这可是约定好的."), actor);
				pk::message_box(pk::encode("确定嘛, 即使这可能会有副作用."), person0);
				pk::message_box(pk::encode("我确定."), actor);
				pk::message_box(pk::encode("好, 那就到这儿来."), person0);

				actor.base_stat[3] = random(min_stat, max_stat);
				pk::message_box(pk::encode("选中武将政治已变更."));

				actor.update();
			}
			else
			{
				pk::message_box(pk::encode("果然很弱呀."), person_2);
				pk::message_box(pk::encode("我\x1b[1x云台的政略\x1b[0x凡人是难以企及的."), person_2);
				pk::message_box(pk::encode("连这孩子也打不过, 向你传授政略也枉然."), person0);
				pk::message_box(pk::encode("真是可惜.."), actor);
				pk::message_box(pk::encode("虽未达成心愿, 但还是要谢谢你."), actor);
				pk::message_box(pk::encode("好你也辛苦了."), person0);
				pk::message_box(pk::encode("人虽笨了点, 心肠倒不错."), person_2);
			}

			actor.action_done = true;

			// 기교 및 금 감소
			ch::add_tp(force_, -TP_COST, force_.get_pos());
			pk::add_gold(building_, -GOLD_COST, true);

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			return true;
		}


		//---------------------------------------------------------------------------
		// 매력변화
		//---------------------------------------------------------------------------

		string getText_매력()
		{
			return pk::encode("(魅力)变化");
		}

		string getDesc_매력()
		{
			if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("让(魅力)脱胎换骨.(技巧P {} )", TP_COST));
		}

		bool isEnabled_매력()
		{
			if (building_gold < GOLD_COST) return false;
			else if (force_.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		bool handler_매력()
		{

			pk::random random(pk::rand());
			pk::list<pk::person@> person_list = getTargetList_매력();

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("要让(魅力)脱胎换骨吗??"), taishu) == 1) return false;

			// 실행무장 선택하기
			if (pk::is_player_controlled(building_))
			{
				pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择要改变(魅力)的武将."), person_list, 1, 1);
				if (person_sel.count == 0) return false; // 미선택 시 취소 종료
				@actor = pk::get_person(person_sel[0].get_id());
			}
			else
			{
				person_list.sort(function(a, b)
				{
					return (a.stat[武将能力_魅力] > b.stat[武将能力_魅力]); // 무장 정렬 (매력순)
				});
				@actor = pk::get_person(person_list[0].get_id());
			}

			pk::person@ person_2 = pk::get_person(무장_여); // 마수미 

			person_2.sei = pk::encode("幻"); // 성
			person_2.mei = pk::encode("异羽"); // 명
			person_2.face = 2312;
			person_2.update();

			// 매력변경 대화 
			pk::message_box(pk::encode("\x1b[1x左慈\x1b[0x先生我听闻这有一部传自神仙的修身之法, 特来请教."), actor);  //빎슥큭흰
			pk::message_box(pk::encode("不知能否将这修身之法传于我."), actor);
			pk::message_box(pk::encode("神仙的修身之法, 凡人难以领悟.."), person0);
			pk::message_box(pk::encode("反而可能会产生很大的副作用."), person0);
			pk::message_box(pk::encode("即便如此, 我也想试一试."), actor);
			pk::message_box(pk::encode("也行...且让你试他一试."), person0);
			pk::message_box(pk::encode("你若能与我弟子论德胜之, 我便传你."), person0);  //管翠
			pk::message_box(pk::encode("感谢."), actor);
			pk::message_box(pk::encode("\x1b[1x异羽\x1b[0x你在何处!"), person0);
			pk::message_box(pk::encode("师傅, 这次要教我什么?"), person_2);
			pk::message_box(pk::encode("你和这人进行论德."), person0);
			pk::message_box(pk::encode("好...这位真能成为我的对手吗?"), person_2);

			pk::scene(pk::scene_t(scene_설전2));

			if (debate_result == 0)
			{
				pk::message_box(pk::encode("我输了, 你比我想象的要出色不少."), person_2);
				pk::message_box(pk::encode("竟能战胜我徒弟, 这人比预计的要出色呀."), person0);
				pk::message_box(pk::encode("哈哈哈, 现在请传授我神仙修身之法."), actor);
				pk::message_box(pk::encode("这可是约定好的."), actor);
				pk::message_box(pk::encode("确定嘛, 即使这可能会有副作用."), person0);
				pk::message_box(pk::encode("我确定."), actor);
				pk::message_box(pk::encode("好, 那就到这儿来."), person0);

				actor.base_stat[4] = random(min_stat, max_stat);
				pk::message_box(pk::encode("选中武将魅力已变更."));

				actor.update();
			}
			else
			{
				pk::message_box(pk::encode("果然很弱呀."), person_2);
				pk::message_box(pk::encode("和我\x1b[1x异羽\x1b[0x \x1b[1x论德\x1b[0x真是不自量力."), person_2);
				pk::message_box(pk::encode("连这孩子也打不过, 向你传授修身之法也枉然."), person0);
				pk::message_box(pk::encode("真是可惜.."), actor);
				pk::message_box(pk::encode("虽未达成心愿, 但还是要谢谢你."), actor);
				pk::message_box(pk::encode("好你也辛苦了."), person0);
				pk::message_box(pk::encode("人虽笨了点, 心肠倒不错."), person_2);
			}

			actor.action_done = true;

			// 기교 및 금 감소
			ch::add_tp(force_, -TP_COST, force_.get_pos());
			pk::add_gold(building_, -GOLD_COST, true);

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			return true;
		}


		//---------------------------------------------------------------------------
		// 적성교환
		//---------------------------------------------------------------------------

		string getText_적성()
		{
			return pk::encode("(适性)变换");
		}

		string getDesc_적성()
		{
			if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} 资金)", GOLD_COST));
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else
				return pk::encode(pk::format("进行(适性)变换.(技巧P {} )", TP_COST));
		}

		bool isEnabled_적성()
		{
			if (building_gold < GOLD_COST) return false;
			else if (force_.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		bool handler_적성()
		{

			pk::random random(pk::rand());
			pk::list<pk::person@> person_list = getTargetList_적성();

			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("要变换武将适性吗?"), taishu) == 1) return false;

			// 실행무장 선택하기
			if (pk::is_player_controlled(building_))
			{
				pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择要变换(适性)的武将."), person_list, 1, 1);
				if (person_sel.count == 0) return false; // 미선택 시 취소 종료
				@actor = pk::get_person(person_sel[0].get_id());
			}
			else
			{
				person_list.sort(function(a, b)
				{
					return (a.stat[武将能力_统率] > b.stat[武将能力_统率]); // 무장 정렬 (통솔순)
				});
				@actor = pk::get_person(person_list[0].get_id());
			}

			pk::person@ person_1 = pk::get_person(무장_남); // 천시로 

			person_1.sei = pk::encode("梅"); // 성
			person_1.mei = pk::encode("起"); // 명
			person_1.face = 2311;
			person_1.update();

			// 통솔변경 대화 
			pk::message_box(pk::encode("\x1b[1x左慈\x1b[0x先生, 我听闻对所有的兵种都运用自如, 特来请教."), actor);
			pk::message_box(pk::encode("请教我如何指挥兵种."), actor);
			pk::message_box(pk::encode("神仙的方法凡人不一定能学会.."), person0);
			pk::message_box(pk::encode("也可能毫无作用."), person0);
			pk::message_box(pk::encode("即便如此, 我也想试一试."), actor);
			pk::message_box(pk::encode("也行...且让你试他一试."), person0);
			pk::message_box(pk::encode("你若能与我弟子论战胜之, 我便传你."), person0);  //管沫
			pk::message_box(pk::encode("感谢."), actor);
			pk::message_box(pk::encode("\x1b[1x梅起\x1b[0x你在何处!"), person0);
			pk::message_box(pk::encode("师傅,我正在进行训练."), person_1);
			pk::message_box(pk::encode("你和这人进行论战."), person0);
			pk::message_box(pk::encode("好...这位真能成为我的对手吗?"), person_1);

			pk::scene(pk::scene_t(scene_설전1));

			if (debate_result == 0)
			{
				pk::message_box(pk::encode("我输了, 你比我想象的要出色不少."), person_1);
				pk::message_box(pk::encode("竟能战胜我徒弟, 这人比预计的要出色呀."), person0);
				pk::message_box(pk::encode("哈哈哈! 现在神仙可以教我了!"), actor);
				pk::message_box(pk::encode("这可是约定好的."), actor);
				pk::message_box(pk::encode("记着, 这可能对你无效."), person0);
				pk::message_box(pk::encode("我确定."), actor);
				pk::message_box(pk::encode("好, 那就到这儿来."), person0);

				pk::scene(pk::scene_t(scene_병종적성));
				pk::message_box(pk::encode("选中武将适性变换了."));
			}
			else
			{
				pk::message_box(pk::encode("果然很弱呀."), person_1);
				pk::message_box(pk::encode("连这孩子也打不过, 我无法教你."), person0);
				pk::message_box(pk::encode("真是可惜.."), actor);
				pk::message_box(pk::encode("虽未达成心愿, 但还是要谢谢你."), actor);
				pk::message_box(pk::encode("好你也辛苦了."), person0);
				pk::message_box(pk::encode("人虽笨了点, 心肠倒不错."), person_1);
			}

			actor.action_done = true;

			// 기교 및 금 감소
			ch::add_tp(force_, -TP_COST, force_.get_pos());
			pk::add_gold(building_, -GOLD_COST, true);

			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			return true;
		}

		void scene_병종적성()
		{

			array<string> 적성_list =
			{
				pk::encode("(枪兵)适性"),
				pk::encode("(戟兵)适性"),
				pk::encode("(弩兵)适性"),
				pk::encode("(骑兵)适性"),
				pk::encode("(兵器)适性"),
				pk::encode("(水军)适性")
			};

			int n = pk::choose(pk::encode("请选择适性."), 적성_list);

			if (n == 0)
			{
				창병적성_교환();
			}
			else if (n == 1)
			{
				극병적성_교환();
			}
			else if (n == 2)
			{
				노병적성_교환();
			}
			else if (n == 3)
			{
				기병적성_교환();
			}
			else if (n == 4)
			{
				병기적성_교환();
			}
			else if (n == 5)
			{
				수군적성_교환();
			}

		}

		void 창병적성_교환()
		{

			array<string> 창병적성_list =
			{
				pk::encode("枪兵 <=> 戟兵"),
				pk::encode("枪兵 <=> 弩兵"),
				pk::encode("枪兵 <=> 骑兵"),
				pk::encode("枪兵 <=> 兵器"),
				pk::encode("枪兵 <=> 水军")
			};

			int n = pk::choose(pk::encode("请选择要交换的适性."), 창병적성_list);

			if (n == 0)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_창병적성 = actor.tekisei[병종_창병];
					int old_극병적성 = actor.tekisei[병종_극병];

					actor.tekisei[병종_창병] = old_극병적성;
					actor.tekisei[병종_극병] = old_창병적성;

					actor.update();
				}
			}
			else if (n == 1)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_창병적성 = actor.tekisei[병종_창병];
					int old_노병적성 = actor.tekisei[병종_노병];

					actor.tekisei[병종_창병] = old_노병적성;
					actor.tekisei[병종_노병] = old_창병적성;

					actor.update();
				}
			}
			else if (n == 2)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_창병적성 = actor.tekisei[병종_창병];
					int old_기병적성 = actor.tekisei[병종_기병];

					actor.tekisei[병종_창병] = old_기병적성;
					actor.tekisei[병종_기병] = old_창병적성;

					actor.update();
				}
			}
			else if (n == 3)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_창병적성 = actor.tekisei[병종_창병];
					int old_병기적성 = actor.tekisei[병종_병기];

					actor.tekisei[병종_창병] = old_병기적성;
					actor.tekisei[병종_병기] = old_창병적성;

					actor.update();
				}
			}
			else if (n == 4)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_창병적성 = actor.tekisei[병종_창병];
					int old_수군적성 = actor.tekisei[兵种_水军];

					actor.tekisei[병종_창병] = old_수군적성;
					actor.tekisei[兵种_水军] = old_창병적성;

					actor.update();
				}
			}

		}

		void 극병적성_교환()
		{

			array<string> 극병적성_list =
			{
				pk::encode("戟兵 <=> 枪兵"),
				pk::encode("戟兵 <=> 弩兵"),
				pk::encode("戟兵 <=> 骑兵"),
				pk::encode("戟兵 <=> 兵器"),
				pk::encode("戟兵 <=> 水军")
			};

			int n = pk::choose(pk::encode("请选择要交换的适性."), 극병적성_list);

			if (n == 0)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_극병적성 = actor.tekisei[병종_극병];
					int old_창병적성 = actor.tekisei[병종_창병];

					actor.tekisei[병종_극병] = old_창병적성;
					actor.tekisei[병종_창병] = old_극병적성;

					actor.update();
				}
			}
			else if (n == 1)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_극병적성 = actor.tekisei[병종_극병];
					int old_노병적성 = actor.tekisei[병종_노병];

					actor.tekisei[병종_극병] = old_노병적성;
					actor.tekisei[병종_노병] = old_극병적성;

					actor.update();
				}
			}
			else if (n == 2)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_극병적성 = actor.tekisei[병종_극병];
					int old_기병적성 = actor.tekisei[병종_기병];

					actor.tekisei[병종_극병] = old_기병적성;
					actor.tekisei[병종_기병] = old_극병적성;

					actor.update();
				}
			}
			else if (n == 3)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_극병적성 = actor.tekisei[병종_극병];
					int old_병기적성 = actor.tekisei[병종_병기];

					actor.tekisei[병종_극병] = old_병기적성;
					actor.tekisei[병종_병기] = old_극병적성;

					actor.update();
				}
			}
			else if (n == 4)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_극병적성 = actor.tekisei[병종_극병];
					int old_수군적성 = actor.tekisei[兵种_水军];

					actor.tekisei[병종_극병] = old_수군적성;
					actor.tekisei[兵种_水军] = old_극병적성;

					actor.update();
				}
			}

		}

		void 노병적성_교환()
		{

			array<string> 노병적성_list =
			{
				pk::encode("弩兵 <=> 枪兵"),
				pk::encode("弩兵 <=> 戟兵"),
				pk::encode("弩兵 <=> 骑兵"),
				pk::encode("弩兵 <=> 兵器"),
				pk::encode("弩兵 <=> 水军")
			};

			int n = pk::choose(pk::encode("请选择要交换的适性."), 노병적성_list);

			if (n == 0)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_노병적성 = actor.tekisei[병종_노병];
					int old_창병적성 = actor.tekisei[병종_창병];

					actor.tekisei[병종_노병] = old_창병적성;
					actor.tekisei[병종_창병] = old_노병적성;

					actor.update();
				}
			}
			else if (n == 1)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_노병적성 = actor.tekisei[병종_노병];
					int old_극병적성 = actor.tekisei[병종_극병];

					actor.tekisei[병종_노병] = old_극병적성;
					actor.tekisei[병종_극병] = old_노병적성;

					actor.update();
				}
			}
			else if (n == 2)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_노병적성 = actor.tekisei[병종_노병];
					int old_기병적성 = actor.tekisei[병종_기병];

					actor.tekisei[병종_노병] = old_기병적성;
					actor.tekisei[병종_기병] = old_노병적성;

					actor.update();
				}
			}
			else if (n == 3)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_노병적성 = actor.tekisei[병종_노병];
					int old_병기적성 = actor.tekisei[병종_병기];

					actor.tekisei[병종_노병] = old_병기적성;
					actor.tekisei[병종_병기] = old_노병적성;

					actor.update();
				}
			}
			else if (n == 4)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_노병적성 = actor.tekisei[병종_노병];
					int old_수군적성 = actor.tekisei[兵种_水军];

					actor.tekisei[병종_노병] = old_수군적성;
					actor.tekisei[兵种_水军] = old_노병적성;

					actor.update();
				}
			}

		}

		void 기병적성_교환()
		{

			array<string> 기병적성_list =
			{
				pk::encode("骑兵 <=> 枪兵"),
				pk::encode("骑兵 <=> 戟兵"),
				pk::encode("骑兵 <=> 弩兵"),
				pk::encode("骑兵 <=> 兵器"),
				pk::encode("骑兵 <=> 水军")
			};

			int n = pk::choose(pk::encode("请选择要交换的适性."), 기병적성_list);

			if (n == 0)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_기병적성 = actor.tekisei[병종_기병];
					int old_창병적성 = actor.tekisei[병종_창병];

					actor.tekisei[병종_기병] = old_창병적성;
					actor.tekisei[병종_창병] = old_기병적성;

					actor.update();
				}
			}
			else if (n == 1)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_기병적성 = actor.tekisei[병종_기병];
					int old_극병적성 = actor.tekisei[병종_극병];

					actor.tekisei[병종_기병] = old_극병적성;
					actor.tekisei[병종_극병] = old_기병적성;

					actor.update();
				}
			}
			else if (n == 2)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_기병적성 = actor.tekisei[병종_기병];
					int old_노병적성 = actor.tekisei[병종_노병];

					actor.tekisei[병종_기병] = old_노병적성;
					actor.tekisei[병종_노병] = old_기병적성;

					actor.update();
				}
			}
			else if (n == 3)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_기병적성 = actor.tekisei[병종_기병];
					int old_병기적성 = actor.tekisei[병종_병기];

					actor.tekisei[병종_기병] = old_병기적성;
					actor.tekisei[병종_병기] = old_기병적성;

					actor.update();
				}
			}
			else if (n == 4)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_기병적성 = actor.tekisei[병종_기병];
					int old_수군적성 = actor.tekisei[兵种_水军];

					actor.tekisei[병종_기병] = old_수군적성;
					actor.tekisei[兵种_水军] = old_기병적성;

					actor.update();
				}
			}

		}

		void 병기적성_교환()
		{

			array<string> 병기적성_list =
			{
				pk::encode("兵器 <=> 枪兵"),
				pk::encode("兵器 <=> 戟兵"),
				pk::encode("兵器 <=> 弩兵"),
				pk::encode("兵器 <=> 骑兵"),
				pk::encode("兵器 <=> 水军")
			};

			int n = pk::choose(pk::encode("请选择要交换的适性."), 병기적성_list);

			if (n == 0)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_병기적성 = actor.tekisei[병종_병기];
					int old_창병적성 = actor.tekisei[병종_창병];

					actor.tekisei[병종_병기] = old_창병적성;
					actor.tekisei[병종_창병] = old_병기적성;

					actor.update();
				}
			}
			else if (n == 1)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_병기적성 = actor.tekisei[병종_기병];
					int old_극병적성 = actor.tekisei[병종_병기];

					actor.tekisei[병종_병기] = old_극병적성;
					actor.tekisei[병종_극병] = old_병기적성;

					actor.update();
				}
			}
			else if (n == 2)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_병기적성 = actor.tekisei[병종_병기];
					int old_노병적성 = actor.tekisei[병종_노병];

					actor.tekisei[병종_병기] = old_노병적성;
					actor.tekisei[병종_노병] = old_병기적성;

					actor.update();
				}
			}
			else if (n == 3)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_병기적성 = actor.tekisei[병종_병기];
					int old_기병적성 = actor.tekisei[병종_기병];

					actor.tekisei[병종_병기] = old_기병적성;
					actor.tekisei[병종_기병] = old_병기적성;

					actor.update();
				}
			}
			else if (n == 4)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_병기적성 = actor.tekisei[병종_병기];
					int old_수군적성 = actor.tekisei[兵种_水军];

					actor.tekisei[병종_병기] = old_수군적성;
					actor.tekisei[兵种_水军] = old_병기적성;

					actor.update();
				}
			}

		}

		void 수군적성_교환()
		{

			array<string> 수군적성_list =
			{
				pk::encode("水军 <=> 枪兵"),
				pk::encode("水军 <=> 戟兵"),
				pk::encode("水军 <=> 弩兵"),
				pk::encode("水军 <=> 骑兵"),
				pk::encode("水军 <=> 兵器")
			};

			int n = pk::choose(pk::encode("请选择要交换的适性."), 수군적성_list);

			if (n == 0)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_수군적성 = actor.tekisei[兵种_水军];
					int old_창병적성 = actor.tekisei[병종_창병];

					actor.tekisei[兵种_水军] = old_창병적성;
					actor.tekisei[병종_창병] = old_수군적성;

					actor.update();
				}
			}
			else if (n == 1)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_수군적성 = actor.tekisei[兵种_水军];
					int old_극병적성 = actor.tekisei[병종_병기];

					actor.tekisei[兵种_水军] = old_극병적성;
					actor.tekisei[병종_극병] = old_수군적성;

					actor.update();
				}
			}
			else if (n == 2)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_수군적성 = actor.tekisei[兵种_水军];
					int old_노병적성 = actor.tekisei[병종_노병];

					actor.tekisei[兵种_水军] = old_노병적성;
					actor.tekisei[병종_노병] = old_수군적성;

					actor.update();
				}
			}
			else if (n == 3)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_수군적성 = actor.tekisei[병종_병기];
					int old_기병적성 = actor.tekisei[兵种_水军];

					actor.tekisei[兵种_水军] = old_기병적성;
					actor.tekisei[병종_기병] = old_수군적성;

					actor.update();
				}
			}
			else if (n == 4)
			{
				if (pk::rand_bool(적성교환성공률))
				{
					int old_수군적성 = actor.tekisei[兵种_水军];
					int old_병기적성 = actor.tekisei[병종_병기];

					actor.tekisei[兵种_水军] = old_병기적성;
					actor.tekisei[병종_병기] = old_수군적성;

					actor.update();
				}
			}

		}

		void scene_설전1()
		{

			pk::person@ person_1 = pk::get_person(무장_남); // 천시로 

			person_1.sei = pk::encode("沐"); // 성
			person_1.mei = pk::encode("雨"); // 명
			person_1.face = 2311;
			person_1.voice = 음성_대담남;
			person_1.base_stat[武将能力_武力] = arr_stat[pk::rand(5)];
			person_1.base_stat[武将能力_智力] = arr_stat[pk::rand(5)];

			// 무장 조형
			person_1.body[0] = 74;
			person_1.body[1] = 74;
			person_1.body[2] = 74;
			person_1.body[3] = 74;
			person_1.body[4] = 74;
			person_1.body[5] = 74;

			person_1.wajutsu_daikatsu = true;
			person_1.wajutsu_kiben = true;
			person_1.wajutsu_mushi = true;
			person_1.wajutsu_chinsei = true;
			person_1.wajutsu_gyakujou = true;

			person_1.update();

			debate_result = pk::debate(actor, person_1, pk::is_player_controlled(actor), false, false, true).first;
		}

		void scene_설전2()
		{

			pk::person@ person_2 = pk::get_person(무장_여); // 마수미 

			person_2.sei = pk::encode("天"); // 성
			person_2.mei = pk::encode("照"); // 명
			person_2.face = 2312;
			person_2.voice = 음성_냉정녀;
			person_2.base_stat[武将能力_智力] = arr_stat[pk::rand(5)];

			// 무장 조형
			person_2.body[0] = 90;
			person_2.body[1] = 90;
			person_2.body[2] = 90;
			person_2.body[3] = 90;
			person_2.body[4] = 90;
			person_2.body[5] = 90;

			person_2.wajutsu_daikatsu = true;
			person_2.wajutsu_kiben = true;
			person_2.wajutsu_mushi = true;
			person_2.wajutsu_chinsei = true;
			person_2.wajutsu_gyakujou = true;

			person_2.update();

			debate_result = pk::debate(actor, person_2, pk::is_player_controlled(actor), false, false, true).first;
		}

		void scene_일기토()
		{

			pk::person@ person_1 = pk::get_person(무장_남); // 천시로 

			person_1.sei = pk::encode("碧"); // 성
			person_1.mei = pk::encode("落"); // 명
			person_1.face = 2311;
			person_1.voice = 음성_대담남;
			person_1.base_stat[武将能力_武力] = arr_stat[pk::rand(5)];
			person_1.base_stat[武将能力_智力] = arr_stat[pk::rand(5)];

			// 무장 조형
			person_1.body[0] = 74;
			person_1.body[1] = 74;
			person_1.body[2] = 74;
			person_1.body[3] = 74;
			person_1.body[4] = 74;
			person_1.body[5] = 74;

			person_1.wajutsu_daikatsu = true;
			person_1.wajutsu_kiben = true;
			person_1.wajutsu_mushi = true;
			person_1.wajutsu_chinsei = true;
			person_1.wajutsu_gyakujou = true;

			person_1.update();

			pk::unit@ dummy_unit;
			duel_result = pk::duel(dummy_unit, dummy_unit, actor, null, null, person_1, null, null, pk::is_player_controlled(actor), false, 0, true).first;
		}

		pk::list<pk::person@> getTargetList_통솔()
		{
			pk::list<pk::person@> list;
			int force_id = force_.get_id();
			for (int i = 0; i < 무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person))
				{
					if (person.get_force_id() == force_id)
					{
						if (!pk::is_absent(person) and !pk::is_unitize(person)
							and person.mibun != 신분_포로 and (person.stat[0] < stat_limit))
							list.add(person);
					}
				}
			}
			return list;
		}

		pk::list<pk::person@> getTargetList_무력()
		{
			pk::list<pk::person@> list;
			int force_id = force_.get_id();
			for (int i = 0; i < 무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person))
				{
					if (person.get_force_id() == force_id)
					{
						if (!pk::is_absent(person) and !pk::is_unitize(person)
							and person.mibun != 신분_포로 and (person.stat[1] < stat_limit))
							list.add(person);
					}
				}
			}
			return list;
		}

		pk::list<pk::person@> getTargetList_지력()
		{
			pk::list<pk::person@> list;
			int force_id = force_.get_id();
			for (int i = 0; i < 무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person))
				{
					if (person.get_force_id() == force_id)
					{
						if (!pk::is_absent(person) and !pk::is_unitize(person)
							and person.mibun != 신분_포로 and (person.stat[2] < stat_limit))
							list.add(person);
					}
				}
			}
			return list;
		}

		pk::list<pk::person@> getTargetList_정치()
		{
			pk::list<pk::person@> list;
			int force_id = force_.get_id();
			for (int i = 0; i < 무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person))
				{
					if (person.get_force_id() == force_id)
					{
						if (!pk::is_absent(person) and !pk::is_unitize(person)
							and person.mibun != 신분_포로 and (person.stat[3] < stat_limit))
							list.add(person);
					}
				}
			}
			return list;
		}

		pk::list<pk::person@> getTargetList_매력()
		{
			pk::list<pk::person@> list;
			int force_id = force_.get_id();
			for (int i = 0; i < 무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person))
				{
					if (person.get_force_id() == force_id)
					{
						if (!pk::is_absent(person) and !pk::is_unitize(person)
							and person.mibun != 신분_포로 and (person.stat[4] < stat_limit))
							list.add(person);
					}
				}
			}
			return list;
		}

		pk::list<pk::person@> getTargetList_적성()
		{
			pk::list<pk::person@> list;
			int force_id = force_.get_id();
			for (int i = 0; i < 무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person))
				{
					if (person.get_force_id() == force_id)
					{
						if (!pk::is_absent(person) and !pk::is_unitize(person) and person.mibun != 신분_포로)
							list.add(person);
					}
				}
			}
			return list;
		}

	}

	Main main;
}
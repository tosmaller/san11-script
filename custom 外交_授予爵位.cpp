// ## 2020/11/28 # messi # 搬运NAVER论坛masterpiece拥立献帝的势力可以对比自己爵位低的君主授予爵位##
/*
	@제작 : masterpiecek
	@요청 : 엑시온
	@내용 : 군주가 한실 황제를 옹립하고 있는 경우 외교메뉴 실행을 통해 타세력 군주에게 자신의 작위보다 낮은 작위를 수여 가능
			(황제가 소속된 도시에서 작위 수여식을 진행하여 이후 양 세력 간 우호도가 상승)
			君主拥立汉室皇帝时，可以通过实行外交菜单，给其他势力君主授予比自己爵位低的爵位
		   （在皇帝所在的城市举行爵位授予仪式，之后两个势力之间的友好度上升）
*/

/**
	[[[ 작위수여 외교 이벤트 개요 ]]]

	0. 거점의 외교메뉴 사용 - 메뉴를 실행한 거점에서 작위를 수여

	1. 기본 조건
	 1) 외교메뉴를 실행하는 건물이 거점(도시, 관문, 항구)일 것
	 2) 한실 황제를 옹립하고 있을 것
	 3) 황제 소속 도시 주변에 적부대가 없을 것 (거점 주변 3칸 이내 - pk::enemies_around(building) 함수)
	 4) 실행세력의 군주가 행동가능 상태일 것 - pk::is_idle(person); 함수 제대로 작동 안하는 것으로 보임
	 5) 실행세력의 군주가 출진중이 아닐 것
	 6) 실행세력의 군주 작위가 기준 이상일 것
	 7) 작위 수여가 가능한 타세력 군주가 존재할 것 - 선택가능 타세력 군주 목록으로 검색하되 이후 타세력 군주 선택 시 활용
	 8) 실행군단의 행동력이 소모값 이상일 것
	 9) 실행세력의 기교P가 소모값 이상일 것

	2. 이벤트 실행여부 최초 확인 메시지
	 - 황제 소속 도시(작위 수여식을 실행거점), 행동력 및 기교P 소모값 확인

	3. 타세력 군주 선택 - pk::person_selector() 함수 사용 (조건 충족 여부 자체는 기본 조건 확인 시 미리 확인)
	 1) 대상세력 군주가 출진중이거나 부재중이 아닐 것
	 2) 대상세력 군주의 작위가 실행세력 군주의 작위보다 낮을 것 (한 단계 낮은 경우도 수여할 수 있는 작위가 없으므로 제외)
	 3) 대상세력 군주 소속거점 주변에 적부대가 없을 것 (거점 주변 3칸 이내 - pk::enemies_around(building) 함수)

	4. 수여할 작위 결정 - pk::choose() 함수 사용

	5. 이벤트 실행여부 최종 확인 메시지
	 - 황제 소속 도시(작위 수여식을 실행거점), 대상세력 군주, 수여 작위 확인

	6. 이벤트 선처리
	 1) 행동력 소모 - district.ap 사용 (행동력의 경우 소모는 바로 되지만 표시가 딜레이가 있음
	 2) 기교P 소모 - pk::add_tp() 함수 사용

	7. 작위수여 이벤트 - pk::scene(), pk::background(), pk::cutin(), pk::play_bgm() 함수 사용
	 1) 실행세력의 군주가 황제 소속 도시에 소속되어 있지 않은 경우 군주 출발 장면
	 2) 황제가 소속된 거점에서 실행세력 군주의 황제 알현 : 작위 수여 건의 및 승낙
	 3) 대상세력 군주의 거점으로 황제의 관인을 파견하여 대상세력 군주에게 작위 수여

	8. 이벤트 후처리
	 1) 실행세력 군주 행동완료 설정 - person.action_done 사용
	 2) 실행세력 군주 소속도시 황제의 소속 도시로 변경 (소속 도시가 다른 경우)
	 2) 대상세력 군주의 작위 변경
	 3) 각 세력 간의 우호도 증가 (메시지박스 표시) - pk::add_relations() 함수 사용
	 [授予爵位外交活动概要]



0. 使用据点外交菜单-在实行菜单的据点授予爵位



1. 基本条件

1）实行外交菜单的建筑物将是据点（城市、关隘、港口）

2）拥立汉室皇帝

3）皇帝所属城市周边不会有敌人部队（据点周围3格以内-pk:enemies around（building）函数）

4）执行势力的君主可能是行动状态—pk:is idle（person）；函数似乎不正常运行

5）执行势力的君主不在出阵中

6）执行势力的君主爵位超过标准

7）存在可授予爵位的其他势力君主-可选择的其他势力君主目录，之后选择其他势力君主时使用

8）执行军团的行动力可能超过消耗值

9）执行势力的技巧P可能超过消耗值



2. 首次确认事件是否运行

-皇帝所属城市（执行爵位授予仪式据点），确认行动力及技巧P消耗值



3. 其他势力君主选择-pk:person selector（）函数使用（条件满足与否本身在确认基本条件时提前确认）

1）对象势力君主不是在出阵或不在

2）目标势力君主的爵位低于执行势力君主的爵位（没有一个等级低的情况也可以授予的爵位）

3）目标势力君主所属据点周边不会有敌人部队（据点周边3格以内-pk:enemies around（building）函数）



4. 随机决定-pk：使用choose（）函数



5. 活动运行与否最终确认信息

-皇帝所属城市（执行爵位仪式据点），对象势力君主，授予爵位确认



6. 事件预处理

1）行动力消耗-district.ap 使用（行动力的情况下，消耗是可以的，但是显示有延迟

2）技巧P消耗-pk:adtp（）函数使用



7. 爵位授予活动-pk：scene（），pk：background（），pk：cutin（），pk：play bgm（）函数使用

1）执行势力的君主不属于皇帝所属城市时，君主出发场面

2）在皇帝所属的据点里，执行势力君主的皇帝拜见：爵位授予建议及承诺

3）派遣皇帝官人到对方势力君主的据点，授予对方势力君主爵位



8. 事件处理

1）执行势力君主行动完成设定-person.action_done n.使用

2）变更为执行势力君主所属城市皇帝所属城市（所属城市不同）

2）目标势力君主的爵位变更

3）增加各势力之间的友好度（信息框显示）-pk:add relations（）函数使用
*/

namespace 외교_작위수여
{

	/// ========================= 유저 설정 =========================

		/// 작위수여 기본설정
		// 작위 순서 : (황제) > 왕 > 공 > 대사마 > 대장군 > 오관중랑장 > 우림중랑장 > 주목 > 주자사 > (없음)
	const string 작위수여_메뉴_단축키 = "";
	const string 작위수여_메뉴_이름 = "授予爵位";
	const int ACTION_COST = 30;	// 设置0时无行动力消耗
	const int 작위수여_소모값_기교P = 50;	// 设置0时无技巧点数消耗
	const int 작위수여_변화값_우호도 = 25;	// 设置0时友好度无变化

	/// 작위수여 대상세력의 동맹여부 제한조건
	const bool 작위수여_대상세력_동맹한정 = false;	// 设置“true”只可授予同盟势力君主爵位

	/// 작위수여 실행군주의 최소작위 제한조건
	const int 작위수여_실행군주_최소작위 = 작위_대장군;	// '작위_이름' 형태로 입력以“爵位名字”形态输入

	/// 작위수여 상승단계 제한조건
	const int 작위수여_대상군주_상승가능_최대단계수 = 2;	// 수여대상 군주의 작위가 설정값만큼의 단계만 상승 가능授予对象君主的爵位只能上升到设定值的阶段

	/// 작위수여 도시거리 제한조건
	// 참고 : 황제의 거점 또는 수여대상 군주의 소속거점이 도시가 아닌 경우에는 해당 거점구역의 도시를 기준으로 거리판단
	const bool 작위수여_거리제한_적용 = false;	// 设置“true”时，不可授予皇帝城市超过一定距离的街头据点中的君主
	const int 작위수여_거리제한_거리 = 5;	// 意味着武装城市间移动所需的回合数，超过设置值时不可颁发

/// =============================================================

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		pk::scenario@ scenario;
		pk::force@ menu_force, selected_force;
		pk::district@ menu_district;
		pk::building@ emperor_base, menu_base, base_of_menu_kunshu, base_of_selected_kunshu;
		pk::person@ emperor, kunshu_of_menu_base, selected_kunshu;
		int selectable_title_id_min, selectable_title_id_max, selected_title_id;
		string emperor_name, emperor_base_name, menu_force_name, kunshu_name_of_menu_base, base_name_of_menu_kunshu;
		string selected_kunshu_name, selected_force_name, base_name_of_selected_kunshu, selected_title_name;
		string ap_info = "行动力\x1b[1x" + ACTION_COST + "\x1b[0x";
		string tp_info = "技巧点\x1b[1x" + 작위수여_소모값_기교P + "\x1b[0x";
		string setting_ally = 작위수여_대상세력_동맹한정 ? "\x1b[1x限定同盟\x1b[0x" : "\x1b[1x无关同盟\x1b[0x";
		string setting_level = "最高授予\x1b[1x" + 작위수여_대상군주_상승가능_최대단계수 + "\x1b[0x阶";
		string setting_distance = "最大城市距离\x1b[1x" + 작위수여_거리제한_거리 + "\x1b[0x";

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			pk::menu_item item;
			item.menu = 103;
			item.shortcut = 작위수여_메뉴_단축키;
			item.init = pk::building_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(isVisible);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode(작위수여_메뉴_이름); });
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void init(pk::building@ building)
		{
			@scenario = pk::get_scenario();
			@emperor = pk::get_person(scenario.emperor);
			emperor_name = "\x1b[1x" + pk::decode(pk::get_name(emperor)) + "\x1b[0x";
			@emperor_base = pk::get_building(emperor.service);
			emperor_base_name = "\x1b[1x" + pk::decode(pk::get_name(emperor_base)) + "\x1b[0x";
			@menu_force = pk::get_force(building.get_force_id());
			menu_force_name = "\x1b[2x" + pk::decode(pk::get_name(menu_force)) + "军\x1b[0x";
			@menu_district = pk::get_district(building.get_district_id());
			@menu_base = @building;
			@kunshu_of_menu_base = pk::get_person(pk::get_kunshu_id(building));
			kunshu_name_of_menu_base = "\x1b[1x" + pk::decode(pk::get_name(kunshu_of_menu_base)) + "\x1b[0x";
			@base_of_menu_kunshu = pk::get_building(kunshu_of_menu_base.service);
			base_name_of_menu_kunshu = "\x1b[1x" + pk::decode(pk::get_name(base_of_menu_kunshu)) + "\x1b[0x";
		}

		bool isVisible()
		{
			if (menu_base.get_id() >= 据点_末)
				return false;

			if (menu_force.get_id() != pk::get_current_turn_force_id())
				return false;

			return true;
		}

		bool isEnabled()
		{
			if (!pk::is_protecting_the_emperor(menu_force))
				return false;

			if (pk::enemies_around(emperor_base))
				return false;

			if (kunshu_of_menu_base.action_done)
				return false;

			if (pk::is_unitize(kunshu_of_menu_base))
				return false;

			if (menu_force.title > 작위수여_실행군주_최소작위)
				return false;

			if (int(get_selectable_kunshu_list().count) == 0)
				return false;

			if (int(menu_district.ap) < ACTION_COST)
				return false;

			if (int(menu_force.tp) < 작위수여_소모값_기교P)
				return false;

			return true;
		}

		string getDesc()
		{
			if (!pk::is_protecting_the_emperor(menu_force))
				return pk::encode("只有拥立汉室皇帝的势力才能实行." + menu_cost_info(true));

			if (pk::enemies_around(emperor_base))
				return pk::encode("皇帝所属的据点周围存在敌方部队." + menu_cost_info(true));;

			if (kunshu_of_menu_base.action_done)
				return pk::encode("君主已经完成行动." + menu_cost_info(true));

			if (pk::is_unitize(kunshu_of_menu_base))
				return pk::encode("君主正在外出征." + menu_cost_info(true));

			if (menu_force.title > 작위수여_실행군주_최소작위)
				return pk::encode("君主的爵位低于可执行的等级." + menu_cost_info(true));

			if (int(get_selectable_kunshu_list().count) == 0)
				return pk::encode("没有可以授予爵位的其他势力君主." + menu_cost_info(true));

			if (int(menu_district.ap) < ACTION_COST)
				return pk::encode("行动力不足." + menu_cost_info(true));

			if (int(menu_force.tp) < 작위수여_소모값_기교P)
				return pk::encode("技巧点不足." + menu_cost_info(true));

			return pk::encode("授予比自己爵位低的其他君主爵位." + menu_cost_info(true));
		}

		bool handler()
		{
			func_작위수여_실행();

			return true;
		}

		void func_작위수여_실행()
		{
			string check_desc = "确定要授予其他势力的君主爵位吗?\n" + menu_cost_info(false);
			check_desc += "\n(" + setting_ally + "," + setting_level + "," + setting_distance + ")";
			int check_value = pk::choose(pk::encode(check_desc), { pk::encode("确定"), pk::encode("取消") });
			if (check_value == /*취소*/1) return;

			string selector_title = pk::encode("选择君主");
			string selector_desc = pk::encode("请选择授予爵位的其他势力君主.(" + setting_ally + "," + setting_distance + ")");
			array<pk::person@> selected_person_arr = pk::list_to_array(pk::person_selector(selector_title, selector_desc, get_selectable_kunshu_list(), 1, 1));
			if (int(selected_person_arr.length) > 0)
			{
				@selected_kunshu = selected_person_arr[0];
				@base_of_selected_kunshu = pk::get_building(selected_kunshu.service);
				@selected_force = pk::get_force(selected_kunshu.get_force_id());
				selected_kunshu_name = "\x1b[1x" + pk::decode(pk::get_name(selected_kunshu)) + "\x1b[0x";
				base_name_of_selected_kunshu = "\x1b[1x" + pk::decode(pk::get_name(base_of_selected_kunshu)) + "\x1b[0x";
				selected_force_name = "\x1b[2x" + pk::decode(pk::get_name(selected_force)) + "军\x1b[0x";
				selectable_title_id_min = pk::max(menu_force.title + 1, selected_force.title - 작위수여_대상군주_상승가능_최대단계수);
				selectable_title_id_max = selected_force.title - 1;
				string choose_desc1 = selected_force_name + "君主" + selected_kunshu_name;
				choose_desc1 += "(当前爵位\x1b[1x" + pk::decode(pk::get_title(selected_force.title).name) + "\x1b[0x)";
				choose_desc1 += "请选择要授予的爵位.";
				pk::message_box(pk::encode(choose_desc1));
				string choose_desc2 = "";
				array<string> choose_arr(0);
				array<int> choose_title_id_arr(0);
				get_choose_title_info(choose_desc2, choose_arr, choose_title_id_arr);
				int choose_value = pk::choose(pk::encode(choose_desc2), choose_arr);
				if (int(choose_title_id_arr.length) != 6 and choose_value != int(choose_arr.length) - 1)
				{
					selected_title_id = choose_title_id_arr[choose_value];
					selected_title_name = pk::decode(pk::get_title(selected_title_id).name);
					string confirm_desc = "确定要对" + selected_force_name + "的君主" + selected_kunshu_name + "\x1b[0x授予";
					confirm_desc += "\x1b[1x" + selected_title_name + "\x1b[0x的爵位吗?\n" + menu_cost_info(false);
					int confirm_value = pk::choose(pk::encode(confirm_desc), { pk::encode("确定"), pk::encode("取消") });
					if (confirm_value == /*결정*/0)
					{
						func_작위수여_선처리();
						pk::scene(pk::scene_t(scene_작위수여_이벤트));
						func_작위수여_후처리();

						return;
					}
				}
			}

			func_작위수여_실행();
		}

		void func_작위수여_선처리()
		{
			pk::add_tp(menu_force, -작위수여_소모값_기교P, menu_base.pos);
			pk::add_ap(menu_district, -ACTION_COST);
		}

		void scene_작위수여_이벤트()
		{
			pk::move_screen(base_of_menu_kunshu.pos, 1000);
			if (@emperor_base == @base_of_menu_kunshu)
			{
				pk::message_box(pk::encode("快移步到皇帝陛下所在的宫殿吧."), kunshu_of_menu_base);
				pk::cutin(18);
			}
			else
			{
				pk::message_box(pk::encode("快去皇帝陛下所在的" + emperor_base_name + "吧."), kunshu_of_menu_base);
				pk::cutin(12);
				pk::move_screen(emperor_base.pos, 1000);
			}

			change_background(34);
			pk::message_box(pk::encode("陛下,臣有要事要表奏."), kunshu_of_menu_base);
			pk::message_box(pk::encode("爱卿" + kunshu_name_of_menu_base + ".因何事而来?"), emperor);
			pk::message_box(pk::encode("最近听说" + selected_kunshu_name + "在百姓间传闻有很高威望的样子."), kunshu_of_menu_base);
			pk::message_box(pk::encode("所以,请给" + selected_kunshu_name + "授予\x1b[1x" + selected_title_name + "\x1b[0x爵位\n您看怎么样?"), kunshu_of_menu_base);
			pk::cutin(20);
			pk::message_box(pk::encode("嗯...就这样吧.现在就向" + selected_kunshu_name + "所在的" + base_name_of_selected_kunshu + "派遣使者授予官印吧."), emperor);
			pk::message_box(pk::encode("谨遵照办,陛下."), kunshu_of_menu_base);
			change_background(-1);

			pk::move_screen(base_of_selected_kunshu.pos, 1000);
			pk::person@ reporter = pk::get_person(무장_문관);
			reporter.sei = pk::encode("文");
			reporter.mei = pk::encode("官");
			pk::message_box(pk::encode(selected_kunshu_name + "大人,陛下的敕使来了.听说" + emperor_name + "将要授予您更高的爵位."), reporter);

			pk::play_bgm(12);
			change_background(2);
			pk::cutin(14);
			pk::message_box(pk::encode(selected_kunshu_name + ",将你任命为\x1b[1x" + selected_title_name + "\x1b[0x.\n今后也要为\x1b[2x汉室\x1b[0x贡献自己的一份力量啊."), pk::get_person(무장_사자));
			string selected_kunshu_msg = "";
			switch (pk::rand(2))
			{
			case 0: selected_kunshu_msg = "万分感谢.敬请期待在下今后的表现.";	break;
			case 1: selected_kunshu_msg = "万分感谢.今后也请期待在下的活跃.";	break;
			}
			pk::message_box(pk::encode(selected_kunshu_msg), selected_kunshu);
			pk::play_se(12);
			change_background(41);
			pk::message_box(pk::encode(pk::get_year() + "年" + pk::get_month() + "月." + selected_kunshu_name + "从" + emperor_name + "那里获得了\x1b[2x" + selected_title_name + "\x1b[0x的爵位."));
		}

		void func_작위수여_후처리()
		{
			kunshu_of_menu_base.action_done = true;
			if (emperor.service != kunshu_of_menu_base.service) kunshu_of_menu_base.service = kunshu_of_menu_base.location = emperor.service;
			selected_force.title = selected_title_id;
			selected_force.update();
			if (작위수여_변화값_우호도 > 0)
			{
				string relations_msg = menu_force_name + "提升了与" + selected_force_name + "友好关系";
				pk::message_box(pk::encode(relations_msg));
				//	pk::history_log(menu_base.pos, menu_force.color, pk::encode(relations_msg));
				pk::add_relations(menu_force, selected_force.get_id(), 작위수여_변화값_우호도);
			}
			pk::scene(cast<pk::scene_t@>(function() { pk::move_screen(main.menu_base.pos, 1000); }));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		string menu_cost_info(bool is_menu_desc)
		{
			string cost_info = "(";
			if (ACTION_COST > 0)  cost_info += (작위수여_소모값_기교P > 0) ? ap_info + ", " + tp_info : ap_info;
			else if (ACTION_COST == 0) cost_info += (작위수여_소모값_기교P > 0) ? tp_info : "\x1b[1x无消耗值\x1b[0x";
			if (!is_menu_desc and (ACTION_COST > 0 or 작위수여_소모값_기교P > 0)) cost_info += "消耗";

			return cost_info + ")";
		}

		pk::list<pk::person@> get_selectable_kunshu_list()
		{
			pk::list<pk::person@> kunshu_list;
			for (int person_id = 0; person_id < 무장_끝; person_id++)
			{
				pk::person@ kunshu = pk::get_person(person_id);
				if (!pk::is_alive(kunshu)) continue;
				if (kunshu.mibun != 身份_君主) continue;
				if (pk::is_unitize(kunshu)) continue;
				if (pk::is_absent(kunshu)) continue;

				int kunshu_force_id = kunshu.get_force_id();
				if (kunshu_force_id == menu_force.get_id()) continue;
				if (작위수여_대상세력_동맹한정 and pk::is_enemy(menu_base, kunshu)) continue;

				pk::building@ kunshu_base = pk::get_building(kunshu.service);
				int city_distance = get_city_distance(emperor_base, kunshu_base);
				if (작위수여_거리제한_적용 and city_distance > 작위수여_거리제한_거리) continue;

				pk::force@ kunshu_force = pk::get_force(kunshu_force_id);
				if (kunshu_force.title <= menu_force.title + 1) continue;
				if (pk::enemies_around(kunshu_base)) continue;

				kunshu_list.add(kunshu);
			}

			return kunshu_list;
		}

		/// 작위번호 : 0황제 - 1왕 - 2공 - 3대사마 - 4대장군 - 5오관중랑장 - 6우림중랑장 - 7주목 - 8주자사
		// 참고 : 황제를 옹립하고 있는 경우 실행세력 군주의 최대작위는 '왕'이므로 타세력 군주에게 수여할 수 있는 최대 작위는 '공'이 됨
		// 참고 : 수여가능 최대 작위가 '공', 최소 작위가 '주목'이므로 수여가능한 작위의 최대 개수는 6개인데,
		//		  이 경우에는 'pk::choose()' 함수의 선택지 최대값과 일치하므로 '취소' 선택지는 넣지 않음
		void get_choose_title_info(string& choose_desc2, array<string>& choose_arr, array<int>& choose_title_id_arr)
		{
			int menu_force_title_id = menu_force.title;
			choose_arr.resize(0);
			choose_desc2 = "顺序：王";
			for (int title_id = 작위_공; title_id <= 작위_주자사; title_id++)
			{
				pk::title@ title = pk::get_title(title_id);
				string title_name = pk::decode(title.name);
				bool givable_title = (title_id >= selectable_title_id_min and title_id <= selectable_title_id_max);
				string title_clr = (menu_force.title == title_id ? "\x1b[17x" : (givable_title ? "\x1b[1x" : (selected_force.title == title_id ? "\x1b[2x" : "\x1b[0x")));
				choose_desc2 += (title_id == 6 ? " " : " ") + "\x1b[29x>\x1b[0x " + title_clr + title_name + "\x1b[0x";
				if (!givable_title) continue;

				choose_arr.insertLast(pk::encode("\x1b[1x" + title_name + "\x1b[0x"));
				choose_title_id_arr.insertLast(title_id);
			}
			if (int(choose_arr.length) < 6) choose_arr.insertLast(pk::encode("取消"));
			choose_desc2 += "\n(\x1b[17x执行君主爵位\x1b[0x,\x1b[1x可授予爵位\x1b[0x,\x1b[2x目标君主爵位\x1b[0x)";
		}

		void change_background(int background_id)
		{
			pk::fade(0);
			pk::background(-1);
			if (background_id != -1) pk::background(background_id);
			pk::fade(255);
		}

		int get_city_distance(pk::building@ base_1, pk::building@ base_2)
		{
			int city_1_id = (base_1.get_id() < 城市_末) ? base_1.get_id() : pk::get_city_id(base_1.pos);
			int city_2_id = (base_2.get_id() < 城市_末) ? base_2.get_id() : pk::get_city_id(base_2.pos);
			return pk::get_city_distance(city_1_id, city_2_id);
		}

		string pad_callback(int line, int original_value, int current_value) { return ""; }

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	}

	Main main;
}
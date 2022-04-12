// ## 2021/10/29 # 江东新风 # 非超级难度不显示派驻细作 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/03/17 # 白马叔叔 # 排除空白据点 ##													   
// ## 2021/03/10 # 白马叔叔 # 修复细作等级下标越界 ##
// ## 2021/01/24 # 江东新风 # 修复港关细作系统的bug ##
// ## 2021/01/23 # 江东新风 # 细作系统完善，随时间推移提升或降低等级，可短期强化，维护费设定,bug修复 ##
// ## 2021/01/20 # 江东新风 # 开始编写细作系统 ##
// ## 2020/12/27 # 江东新风 # bug修复,参考keehl102大神更新在马术书大神基础上更改 ##
// ## 2020/12/26 # 江东新风 # 同盟间势力也不可见 ##
// ## 2020/12/24 # 江东新风 # 修复trace参数报错 ##
// ## 2020/12/13 # 江东新风 # 新特技_窥探 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错-未完成 ##
// ## 2020/11/24 #江东新风# 参考马术书更改，尝试添加shift-右键开关 ##
// ## 2020/08/30 #氕氘氚#  ##
// ## 2020/08/24 #keehl102###
/*
@修者:马术书
@Update:“18.10.21 /变更内容:部队/建筑物的视野调整功能，特殊视力增加功能
@Update:“18.11.26 /变更内容:添加信息显示/隐藏选项菜单(shift+ ooclick)
@Update:“18.12.22 /变更内容:特急难易度时增加信息显示选项，人数在1以下时退出时不隐藏信息
@Update:“19.1.4 /变更内容:修正为在生成详细建筑物时显示信息
@Update:“19.1.30 /小时内关门/港口的兵器信息无法打开，(视野外的关门/港口的兵器信息明显改变)
*/
namespace 信息隐藏
{
	// 건물 정보
	const uint open_building_info_flags = 4;
	const uint closed_building_info_flags = 0;
	// 군단 정보
	const uint open_district_info_flags = 0;
	const uint closed_district_info_flags = 0;
	// 세력 정보
	const uint open_force_info_flags = 0;
	const uint closed_force_info_flags = 0;
	// 무장 정보(충성도는 정렬하면 금방 알 수 있기 때문에 자세력이나 적세력 무장 모두 숨김)
	const uint open_person_info_flags = 1;
	const uint closed_person_info_flags = 0;
	// 부대 정보
	const uint open_unit_info_flags = uint(-1);
	const uint closed_unit_info_flags = 0;

	// 부대/건물 시야 추가량 지정 ('18.10.21)
	const int 追加视野_部队 = 0;     // 부대의 기본 시야에 추가 시야값 지정
	const int 追加视野_建筑 = 0;    // 건물의 기본 시야에 추가 시야값 지정
	const int 추가시야_특기 = 2;    // 특기_안력 장수 소속의 거점 또는 부대의 시야 증가
	const bool 특급정보표시 = true;   // 难易度_超级에서 계략/전법/일기토 확률 표시

	/// ================ 派驻细作相关自定义 ================
	const int ACTION_COST = 20;   	 // 行动力消耗
	const int GOLD_COST = 300;       // 金消耗
	const int 细作_可执行距离 = 2;


	class Main
	{
		uint building_flags_1_;
		dictionary visible_pos_;
		pk::list<pk::building@> building_list;  // '19.1.30 추가 (병장기 표시용)

		bool visible = false;   // 정보표시/숨김 옵션 처리를 위한 변수 추가 ('18.11.26)

		///------------派驻细作相关常量------------------///
		pk::building@ building_;
		pk::building@ target;
		pk::force@ force_;
		pk::person@ taishu_;
		pk::person@ actor;
		pk::city@ city_;
		int building_gold;
		//pk::list<pk::building@> spy_building_list;

		Main()
		{
			// 이 파일의 시나리오 초기화 이벤트는 세력 재배치 이후에 발생해야함.
			pk::bind(102, /*-1*/100, pk::trigger102_t(onGameInit));
			pk::bind(111, pk::trigger111_t(onTurnStart));
			pk::bind(112, pk::trigger112_t(onTurnEnd));
			pk::bind(132, pk::trigger132_t(onForceLeaveControl));
			pk::bind(140, pk::trigger140_t(onDistrictCreate));
			pk::bind(152, pk::trigger152_t(onPersonForceChange));
			pk::bind(160, pk::trigger160_t(onBuildingCreate));
			//pk::bind(161, pk::trigger161_t(building_destroy));
			pk::bind(170, pk::trigger170_t(onUnitCreate));
			//pk::bind(171, pk::trigger171_t(unit_destroy));
			pk::bind(172, pk::trigger172_t(onUnitPosChange));
			//细作信息显示
			pk::bind(120, pk::trigger120_t(func_信息显示_据点信息));

			pk::bind(107, pk::trigger107_t(func_细作信息_更新));

			// shift+우클릭 메뉴 추가 ('18.11.26)
			add_menu();

			//添加派驻细作菜单
			add_menu_spy();
		}

		// 정보표시/정보숨김 옵션 ('18.11.26)
		void add_menu()
		{
			pk::menu_item item;
			item.menu = 2;
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.get_image_id = pk::menu_item_get_image_id_t(getImageID);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}
		string getText()
		{
			return (visible) ? pk::encode("显示信息") : pk::encode("关闭信息");
		}
		string getDesc()
		{
			return pk::encode("更改信息显示状态");
		}
		int getImageID()
		{
			return (visible) ? 889 : 1249;
		}
		bool handler()
		{
			visible = !visible;

			//PersonInfo test1(100);
			//pk::trace(pk::format("after3:finished_jili:{},yunchou:{}", test1.finished_jili, test1.finished_yunchou));

			auto force = pk::get_force(pk::get_current_turn_force_id());
			onTurnStart(force);
<<<<<<< HEAD

=======
			pk::set_fog_state(visible);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			return true;
		}
		////--------------------------派驻细作--------------------------------/////       
		void add_menu_spy()
		{
			pk::menu_item item;
			item.menu = 104;
			//item.pos = 3;
			//item.shortcut = "I";
			item.init = pk::building_menu_item_init_t(init_spy);
			item.is_visible = pk::menu_item_is_visible_t(isVisible_spy);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_spy);
			item.get_text = pk::menu_item_get_text_t(getText_spy);
			item.get_desc = pk::menu_item_get_desc_t(getDesc_spy);
			item.handler = pk::menu_item_handler_t(handler_spy);
			pk::add_menu_item(item);
		}

		void init_spy(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@taishu_ = pk::get_person(pk::get_taishu_id(building));
			//@city_ = pk::building_to_city(building);
			building_gold = pk::get_gold(building);
		}

		bool isVisible_spy()
		{
			if (pk::get_scenario().difficulty != 难易度_超级) return false;
			return true;
		}

		bool isEnabled_spy()
		{
			if (pk::get_idle_person_list(building_).count == 0) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			else if (building_gold < GOLD_COST) return false;
			return true;
		}

		string getText_spy()
		{
			return pk::encode("细作");
		}

		string getDesc_spy()
		{
			if (pk::get_idle_person_list(building_).count == 0)
				return pk::encode("没有可执行的武将.");
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (需要{}行动力)", ACTION_COST));
			else if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (至少需要{}金)", GOLD_COST));
			else
				return pk::encode(pk::format("派驻细作. (行动力 {})", ACTION_COST));
		}

		bool handler_spy()
		{
			return spy_execute();
		}


		/** 시야 범위
		@return
			최소, 최대 범위
		*/
		pk::int_int getSightRange(pk::object@ obj)
		{
			if (obj.is_instance(pk::unit::type_id))
			{
				return pk::int_int(1, 2);
			}
			else if (obj.is_instance(pk::building::type_id))
			{
				// pk::hex_object 타입으로 들어온 경우가 있을 수 있으므로 한번 변환
				auto building = pk::hex_object_to_building(obj);
				auto facility = pk::get_facility(building.facility);
				if (facility !is null)
					return pk::int_int(facility.min_range, facility.max_range + (building.facility == 시설_도시 ? 1 : 0));
			}
			return pk::int_int(0, 0);
		}

		/** 좌표를 키로 변환(visible_pos 테이블의 키로 사용됨) */
		string makeKey(const pk::point& in pos)
		{
			return pk::format("{}_{}", pos.x, pos.y);
		}

		/** 업데이트가 필요한 상황인지 판단
		@details
			난이도가 특급이고 플레이어턴인 경우
		*/
		bool needUpdate()
		{
			if (pk::get_scenario().difficulty != 难易度_超级) return false;
			auto force = pk::get_force(pk::get_current_turn_force_id());
			return force !is null and force.is_player();
		}

		/** 시야를 밝힘 */
		void update(int force_id, const pk::point& in pos, int min, int max)
		{
			//所以0是可见，-1是不可见？
			visible_pos_[makeKey(pos)] = 0;
			//获取对应的范围，将范围内的信息设为可见？
			auto range = pk::range(pos, min, max);
			for (int i = 0; i < int(range.length); i++)
			{
				visible_pos_[makeKey(range[i])] = 0;
				auto obj = pk::get_hex_object(range[i]);
				if (obj !is null and obj.get_force_id() != force_id)
				{
					//pk::core::info(obj, -1, obj.is_instance(pk::unit::type_id) ? open_unit_info_flags : open_building_info_flags);

					if (obj.is_instance(pk::unit::type_id))
						pk::core::info(obj, -1, open_unit_info_flags | uint(pk::core::info(obj, -1)));
					else
						pk::core::info(obj, -1, uint(-1) | uint(pk::core::info(obj, -1)));

					auto person_list = pk::get_person_list(pk::get_hex_object_id(obj), uint(-1));
					for (int j = 0; j < person_list.count; j++)
						pk::core::info(person_list[j], -1, ((visible) ? -1 : open_person_info_flags));

					// 리스트에 시야 내 건물 추가  在列表中添加视野符号('19.1.30)
					if (obj.is_instance(pk::building::type_id))
					{
						pk::building@ building = pk::hex_object_to_building(obj);
						if (!building_list.contains(building))
							building_list.add(building);
					}
				}
			}
		}

		/***/
		void onTurnStart(pk::force@ force)
		{
			// 시야 초기화
			visible_pos_.deleteAll();

			// 건물리스트 초기화 ('19.1.30)
			building_list.clear();

			if (!needUpdate())
				return;

			// 정보표시/숨김 옵션을 고려하여 모든 정보 초기화
			pk::core::info(pk::building::type_id, ((visible) ? -1 : 0));
			pk::core::info(pk::district::type_id, ((visible) ? -1 : 0));
			pk::core::info(pk::force::type_id, ((visible) ? -1 : 0));
			pk::core::info(pk::person::type_id, ((visible) ? -1 : 0));
			pk::core::info(pk::unit::type_id, ((visible) ? -1 : 0));

			//回合开始时，确定是否是新的一月，概率提升间谍等级？也有概率降低，onturnstart 会被其他地方调用，所以可能还是有必要on_new_day


			int force_id = force.get_id();

			for (int i = 0; i < 건물_끝; i++)
			{
				auto building = pk::get_building(i);

				// 部队/建筑视野追加量设定 부대/건물 시야 추가량 지정 ('18.10.21)
				int 视野_特技效果 = (ch::has_skill(building, 特技_窥探)) ? 窥探视野增加 : 0;

				//同势力的情况
				if (building.get_force_id() == force_id)
				{
					pk::core::info(building, -1, open_building_info_flags);
					auto range = getSightRange(building);
					// 部队/建筑视野追加量设定 부대/건물 시야 추가량 지정 ('18.10.21)
					update(force_id, building.get_pos(), range.first, range.second + 追加视野_建筑 + 视野_特技效果);

					///在该列表中的建筑将设为可见
					// 在列表中添加视野符号 리스트에 시야 내 건물 추가 ('19.1.30)
					if (!building_list.contains(building))
						building_list.add(building);
				}
				/*
				int building_id = building.get_id();
				if (building_id < 据点_末)
				{
					if (pk::is_valid_force_id(force_id))
					{
						//BaseInfo@ base_t = @base_ex[building_id];
						//pk::trace(pk::format("force_id:{},building_id:{}", force_id, building_id));
						//pk::trace(pk::format("spy_this:{},spy1:{},spy2:{},spy3:{}", base_t.has_spy[force_id], base_t.has_spy[1], base_t.has_spy[2], base_t.has_spy[3]));

						if (base_t.spy_lv[force_id] > 0 and !building_list.contains(building))
							building_list.add(building);
					}

				}*/
					
			}

			for (int i = 0; i < 군단_끝; i++)
			{
				auto district = pk::get_district(i);
				if (district.get_force_id() == force_id)
					pk::core::info(district, -1, open_district_info_flags);
			}

			for (int i = 0; i < 세력_끝; i++)
			{
				auto force0 = pk::get_force(i);
				if (force0.get_force_id() == force_id)
					pk::core::info(force0, -1, open_force_info_flags);
			}

			for (int i = 0; i < 무장_끝; i++)
			{
				auto person = pk::get_person(i);
				if (person.get_force_id() == force_id)
					pk::core::info(person, -1, ((visible) ? -1 : open_person_info_flags));
			}

			for (int i = 0; i < 部队_末; i++)
			{
				auto unit = pk::get_unit(i);

				// 부대/건물 시야 추가량 지정 ('18.10.21)
				int 视野_特技效果 = (ch::has_skill(unit, 特技_窥探)) ? 窥探视野增加 : 0;

				if (unit.get_force_id() == force_id)
				{
					pk::core::info(unit, -1, open_unit_info_flags);
					auto range = getSightRange(unit);
					// 부대/건물 시야 추가량 지정 ('18.10.21)
					update(force_id, unit.get_pos(), range.first, range.second + 追加视野_部队 + 视野_特技效果);
				}
			}

			// 隐藏所有建筑物信息并隐藏视野外的建筑物信息 건물정보 모두 보이게 한 후 시야 밖 건물정보 감추기 ('19.1.30)
			if (!visible)
			{
				pk::core::info(pk::building::type_id, -1);
				for (int i = 0; i < 건물_끝; i++)
				{
					pk::building@ building = pk::get_building(i);
					//此处把不包含的据点都隐藏
					if (!building_list.contains(building) /*and building.get_id() != 0*/)
					{
						pk::core::info(building, 0, (closed_building_info_flags == uint(-1)));
						pk::core::info(building, 1, (closed_building_info_flags == uint(-1)));
						pk::core::info(building, 2, (closed_building_info_flags == uint(-1)));
						pk::core::info(building, 3, (closed_building_info_flags == uint(-1)));
						pk::core::info(building, 4, (closed_building_info_flags == uint(-1)));
						pk::core::info(building, 5, (closed_building_info_flags == uint(-1)));

						if (i < 据点_末)
							pk::core::info(building, 6, (closed_building_info_flags == uint(-1)));
					}
					
				}
				//细作系统，加一个根据级别显示列表内的部分内容
				for (int i = 0; i < 据点_末; i++)
				{
					pk::building@ building = pk::get_building(i);
					//0兵力，1金，2粮，3耐久，4气力，5治安，6兵器
					//1级显示金粮，2级显示耐久气力，3级显示兵力，治安，兵器
					BaseInfo@ base_t = @base_ex[i];
					//pk::trace(pk::format("spy level: {}", base_t.spy_lv[force_id]));
					if (base_t.spy_lv[force_id] > 2)
					{
						pk::core::info(building, 0, true);						
						pk::core::info(building, 6, true);
					}
					if (base_t.spy_lv[force_id] > 1)
					{
						pk::core::info(building, 1, true);
						pk::core::info(building, 2, true);
					}
					if (base_t.spy_lv[force_id] > 0)
					{						
						pk::core::info(building, 3, true);
						pk::core::info(building, 4, true);
						pk::core::info(building, 5, true);
					}

				}
			}

		}

		/***/
		void onTurnEnd(pk::force@ force)
		{
			if (!needUpdate())
				return;

			if (pk::get_player_count() > 1)
			{
				// 세력, 군단, 무장은 컴퓨터 턴에서 확인할 수 없으므로 초기화 시키지 않음.
				for (int i = 0; i < 건물_끝; i++)
				{
					auto building = pk::get_building(i);
					pk::core::info(building, -1, 0);
				}

				for (int i = 0; i < 部队_末; i++)
				{
					auto unit = pk::get_unit(i);
					pk::core::info(unit, -1, 0);
				}
			}

		}

		/***/
		void onGameInit()
		{
			//pk::trace("信息隐藏");
			building_flags_1_ = uint(-1);
			if (pk::get_scenario().loaded)
				onTurnStart(pk::get_force(pk::get_current_turn_force_id()));

			if (pk::get_scenario().difficulty < 难易度_超级 or (특급정보표시 and pk::get_scenario().difficulty == 难易度_超级))
			{
				pk::core::info(-1, 1);
				pk::core::info(-2, 1);
				pk::core::info(-3, 1);
			}
		}

		/***/
		void onUnitCreate(pk::unit@ unit, int type)
		{
			if (!needUpdate())
				return;

			pk::core::info(unit, -1, unit.get_force_id() == pk::get_current_turn_force_id() ? open_unit_info_flags : ((visible) ? -1 : closed_unit_info_flags));
		}

		/***/
		void onUnitPosChange(pk::unit@ unit, const pk::point& in pos)
		{
			if (!needUpdate())
				return;

			// 부대/건물 시야 추가량 지정 ('18.10.21)
			int 视野_特技效果 = (ch::has_skill(unit, 特技_窥探)) ? 窥探视野增加 : 0;

			auto range = getSightRange(unit);
			if (unit.get_force_id() == pk::get_current_turn_force_id())
				update(unit.get_force_id(), pos, range.first, range.second + 追加视野_部队 + 视野_特技效果);
			else
				pk::core::info(unit, -1, visible_pos_.exists(makeKey(pos)) ? open_unit_info_flags : ((visible) ? -1 : closed_unit_info_flags));
		}

		void building_destroy(pk::building@ building, int type)
		{
			pk::core::info(building, -1, closed_building_info_flags);
		}

		void unit_destroy(pk::unit@ unit, int type)
		{
			if (!needUpdate())
				return;
			pk::core::info(unit, -1, closed_unit_info_flags);
		}

		/***/
		void onBuildingCreate(pk::building@ building, int type)
		{
			if (!needUpdate())
				return;

			// 부대/건물 시야 추가량 지정 ('18.10.21)
			int 视野_特技效果 = (ch::has_skill(building, 特技_窥探)) ? 窥探视野增加 : 0;

			auto range = getSightRange(building);
			if (building.get_force_id() == pk::get_current_turn_force_id())
			{
				pk::core::info(building, -1, open_building_info_flags);
				update(building.get_force_id(), building.get_pos(), range.first, range.second + 追加视野_建筑 + 视野_特技效果);
			}
			else
				pk::core::info(building, -1, visible_pos_.exists(makeKey(building.get_pos())) ? open_building_info_flags : ((visible) ? -1 : closed_building_info_flags));
		}

		/***/
		void onDistrictCreate(pk::district@ district, int type)
		{
			if (!needUpdate())
				return;

			pk::core::info(district, -1, district.get_force_id() == pk::get_current_turn_force_id() ? open_district_info_flags : ((visible) ? -1 : closed_district_info_flags));
		}

		/***/
		void onPersonForceChange(pk::person@ person, pk::force@ force)
		{
			if (!needUpdate())
				return;

			if (force !is null) pk::core::info(person, -1, force.get_force_id() == pk::get_current_turn_force_id() ? open_person_info_flags : ((visible) ? -1 : closed_person_info_flags));
		}

		/***/
		void onForceLeaveControl(pk::force@ force)
		{
			// 모든 세력을 위임한 경우
			if (pk::get_player_count() == 1)
			{
				// 모든 정보를 보이는 상태로 초기화 将所有信息重置为可见状态
				pk::core::info(pk::building::type_id, -1);
				pk::core::info(pk::district::type_id, -1);
				pk::core::info(pk::force::type_id, -1);
				pk::core::info(pk::person::type_id, -1);
				pk::core::info(pk::unit::type_id, -1);
			}
			else
			{
				onTurnEnd(force);
			}
		}

		///-----------------------细作的执行部分-------------------------////
		bool spy_execute()
		{
			//pk::trace("pos0");
			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("是否发布细作相关指令？\n(获取敌军城市信息)", GOLD_COST)), taishu_) == 1)
				return false;
			//pk::trace("pos1");
			
			pk::list<pk::building@> spyable_building_list = getTargetList(building_);
			//pk::trace("pos2");
			if (spyable_building_list.count == 0) return false;



			// 选擇目标据点

			pk::list<pk::building@> list_target = pk::building_selector(pk::encode("选择目标据点"), pk::encode("选择目标据点."), spyable_building_list, 1, 1);
			if (list_target.count == 0) return false; // 未选擇时取消结束
			@target = list_target[0];
			string target_name = pk::decode(pk::get_name(target));
			int target_id = target.get_id();
			BaseInfo@ base_t = @base_ex[target_id];
			//选择要执行的操作。两种，高级间谍和低级？高级的刺探情报更给力(情报提升更快)，初始费用更高，维护费更高？低级的刺探情报能力弱。
			//加强细作运作是花钱快速提升细作等级，有概率被发现降级
			//根据所选据点以及己方据点金钱决定展示的内容，
			//有高级细作的就展示1.换派低级细作，2.加强细作运作，3.撤回细作
			//有低级细作的就展示1.换派高级细作，2.加强细作运作，3.撤回细作
			//无细作的就展示    1.派驻高级细作，2.派驻低级细作，3.加强细作运作，4.撤回细作
			//用一个bool存46个势力是否对该城执行过派驻细作，另一个存是否对该城执行过加强运作
			//回合开始时，如果是执行派驻细作的，则细作等级设置为1，如果执行过加强运作的，决定加强运作的结果，同时将bool改成false
			//目标城市选择时，执行过加强运作和执行派驻细作的城市将不会出现在列表里

			int answer = 0;
			int spy_lv = base_t.spy_lv[building_.get_force_id()];
			if (spy_lv == 0) answer = pk::choose(pk::encode("请选择执行的操作"),{ pk::encode("派驻细作"), pk::encode("取消") });
			if (spy_lv > 0 and spy_lv < 3) answer = pk::choose(pk::encode("请选择执行的操作"), { pk::encode("加强细作运作"), pk::encode("撤回细作"), pk::encode("取消") });
			if (spy_lv == 3) answer = pk::choose(pk::encode("请选择执行的操作"), { pk::encode("撤回细作"), pk::encode("取消") });


			if (answer == 1 and (spy_lv == 0 or spy_lv == 3)) return false;  // 取消时返回
			if (answer == 2) return false;  // 取消时返回

			if (answer == 0)
			{	
				//派驻细作
				if (spy_lv == 0)
				{
					base_t.send_spy[building_.get_force_id()] = true;
					//auto force = pk::get_force(pk::get_current_turn_force_id());
					pk::message_box(pk::encode(pk::format("已经向\x1b[1x{}\x1b[0x派驻细作，不久后定有消息传来。", target_name)), pk::get_person(武将_传令));
					//取消直接升级细作，则会在下回合开始时开始显示信息
					//onTurnStart(force); 
					pk::add_gold(building_, -GOLD_COST, true);
				}
				//加强细作运作
				if (spy_lv > 0 and spy_lv < 3)
				{
					base_t.enhance_spy[building_.get_force_id()] = true;

					//auto force = pk::get_force(pk::get_current_turn_force_id());
					pk::message_box(pk::encode(pk::format("已经向\x1b[1x{}\x1b[0x的细作发布加强运作的命令，应该有望刺探到更多信息。", target_name)), pk::get_person(武将_传令));
					pk::add_gold(building_, -(GOLD_COST * 2), true);
					//onTurnStart(force); 
				}
				if (spy_lv == 3)
				{
					base_t.spy_lv[building_.get_force_id()] = 0;
					//auto force = pk::get_force(pk::get_current_turn_force_id());
					pk::message_box(pk::encode(pk::format("已经向\x1b[1x{}\x1b[0x的细作发布了撤回的信息。", target_name)), pk::get_person(武将_传令));
					//取消直接升级细作，则会在下回合开始时开始显示信息
					//onTurnStart(force); 
					pk::add_gold(building_, -int(GOLD_COST/2), true);
				}

			}

			if (answer == 1)
			{
				base_t.spy_lv[building_.get_force_id()] = 0;
				//auto force = pk::get_force(pk::get_current_turn_force_id());
				pk::message_box(pk::encode(pk::format("已经向\x1b[1x{}\x1b[0x的细作发布了撤回的信息。", target_name)), pk::get_person(武将_传令));
				pk::add_gold(building_, -int(GOLD_COST / 2), true);
			}


			// 행동력 감소.
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);

			// 실행무장 행동완료
			//actor.action_done = true;

			// 금 감소
			

			return true;

		}

		pk::list<pk::building@> getTargetList(pk::building@ src)
		{
			pk::list<pk::building@> list;
			for (int i = 0; i < 建筑_据点末; i++)
			{
				pk::building@ dst = pk::get_building(i);
				int src_city_id = pk::get_city_id(src.pos);
				int dst_city_id = pk::get_city_id(dst.pos);
				int src_id = src.get_id();
				int dst_id = dst.get_id();
				//pk::trace(pk::format("对象据点{}，我方据点{}，相邻据点:{}，距离:{}",pk::decode(pk::get_name(dst)), pk::decode(pk::get_name(src)),pk::is_neighbor_base(src_id, dst_id), pk::get_building_distance(dst_id, src_id, src.get_force_id())));
				if (src_id != dst_id and /*pk::is_neighbor_base(src_id, dst_id)*/ src.get_force_id() != dst.get_force_id() and dst.get_force_id() != -1 and pk::get_city_distance(src_city_id, dst_city_id) <= 细作_可执行距离)
				{
					BaseInfo@ base_t = @base_ex[dst_id];
					if (!base_t.send_spy[src.get_force_id()] and !base_t.send_spy[src.get_force_id()]) list.add(dst);
				}					
			}
			return list;
		}

		void func_细作信息_更新()
		{
			for (int j = 0; j < 势力_末; ++j)
			{
				pk::force@ force0 = pk::get_force(j);
				if (force0.is_player())
				{
					int count = 0;
					for (int i = 0; i < 建筑_据点末; ++i)
					{
						pk::building@ building0 = pk::get_building(i);
						//只有在据点不是玩家的，且为玩家势力时，才执行操作
						if (building0.get_force_id() != j)
						{
							BaseInfo@ base_t = @base_ex[i];
							if (base_t.send_spy[j] or base_t.spy_lv[j] > 0) count += 1;
							if (base_t.send_spy[j])
							{
								base_t.spy_lv[j] = 1;
								base_t.send_spy[j] = false;
								pk::history_log(building0.pos, force0.color, pk::encode(pk::format("细作成功潜入了{},并传回了一些消息。", pk::decode(pk::get_name(building0)))));
							}
							else if (base_t.enhance_spy[j])
							{
								if (pk::rand_bool(85))
								{
									base_t.spy_lv[j] += 1;
									base_t.enhance_spy[j] = false;
									pk::history_log(building0.pos, force0.color, pk::encode(pk::format("{}的细作通过加强运作,传回了更多军情。", pk::decode(pk::get_name(building0)))));
								}
								else
								{
									base_t.spy_lv[j] -= 1;
									base_t.enhance_spy[j] = false;
									pk::history_log(building0.pos, force0.color, pk::encode(pk::format("{}的细作的运作引起了敌军注意,\n细作网络遭到破坏。", pk::decode(pk::get_name(building0)))));

								}
							}
							else
							{
								if (base_t.spy_lv[j] == 1 and pk::rand_bool(15))
								{
									base_t.spy_lv[j] += 1;
									pk::history_log(building0.pos, force0.color, pk::encode(pk::format("长期运作下，{}的细作等级提升。", pk::decode(pk::get_name(building0)))));
								}
								else if (base_t.spy_lv[j] == 2 and pk::rand_bool(10))
								{
									base_t.spy_lv[j] += 1;
									pk::history_log(building0.pos, force0.color, pk::encode(pk::format("长期运作下，{}的细作等级进一步提升。", pk::decode(pk::get_name(building0)))));
								}
								else if (base_t.spy_lv[j] == 3 and pk::rand_bool(5))
								{
									base_t.spy_lv[j] -= 1;
									pk::history_log(building0.pos, force0.color, pk::encode(pk::format("{}的细作受到敌方察觉，\n细作网络遭到破坏。", pk::decode(pk::get_name(building0)))));
								}
							}
						}
					}
					//维护费支付
					if ((pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
					{
						if (count > 0)
						{
							pk::person@ kunshu0 = pk::get_person(force0.kunshu);
							pk::building@ building1 = pk::get_building(kunshu0.service);
							int pay = count * 100;
							if (pay <= pk::get_gold(building1))
							{
								pk::add_gold(building1,-pay,true);
								pk::history_log(building1.pos, force0.color, pk::encode(pk::format("君主所属的{}向各地细作支付了{}金的运作费。", pk::decode(pk::get_name(building1)),pay)));

							}
							else
							{
								for (int i = 0; i < 建筑_据点末; ++i)
								{
									BaseInfo@ base_t = @base_ex[i];
									if (base_t.spy_lv[j] > 0)
									{
										base_t.spy_lv[j] -= 1;
									}
								}
								
								pk::history_log(building1.pos, force0.color, pk::encode(pk::format("因不足以维持细作运作，多地细作等级下降。", pay)));

							}
						}
					}
				}
				

			}

		}

		void func_信息显示_据点信息()
		{
<<<<<<< HEAD
			军餉维护费::信息行数 = 0;
=======
			据点信息行数 = 0;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			// 光标指的坐标
			pk::point cursor_pos = pk::get_cursor_hex_pos();
			if (!pk::is_valid_pos(cursor_pos)) return;

			// 光标上指示的建筑物
			pk::building@ building = pk::get_building(cursor_pos);
			if (building is null || building.facility > 2) return;  //城港关才显示


			// 确认城市是否是玩家的，及是否城市控制势力id是当前回合的执行势力id
			if (building.is_player()) return;
			// if (!pk::is_player_controlled(building)) return;
			if (building.get_force_id() == pk::get_current_turn_force_id()) return;

			string building_name = pk::decode(pk::get_name(building));

			string title = pk::format("据点信息(\x1b[1x{}\x1b[0x)", building_name);

			BaseInfo@ base_t = @base_ex[building.get_id()];
			//string 细作与否 = base_t.has_spy[pk::get_current_turn_force_id()] ? "是" : "否";
			string info_细作;
			if (pk::get_current_turn_force_id() >= 0)
				info_细作 = pk::format("细作级别: \x1b[1x{}\x1b[0x", base_t.spy_lv[pk::get_current_turn_force_id()]);
			//string info_人口 = pk::format("人口: \x1b[1x{}\x1b[0x", base_t.population);

			int middle = int(pk::get_resolution().width) / 2;
			int left = middle - 200;
			int right = middle + 200;
			int top = 5;
			int bottom = top + 80;

			// pk::draw_rect(pk::rectangle(left, top, right, bottom), 0xff00ccff);
			pk::draw_text(pk::encode(title), pk::point(left + 5, top + 5), 0xffffffff, FONT_BIG, 0xff000000);
<<<<<<< HEAD
			pk::draw_text(pk::encode(info_细作), pk::point(left + 10, top + 40 + 军餉维护费::信息行数 * 20), 0xffffffff, FONT_SMALL, 0xff000000);
			军餉维护费::信息行数 += 1;
=======
			pk::draw_text(pk::encode(info_细作), pk::point(left + 10, top + 40 + 据点信息行数 * 20), 0xffffffff, FONT_SMALL, 0xff000000);
			据点信息行数 += 1;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			//pk::draw_text(pk::encode(info_人口), pk::point(middle + 10, top + 40), 0xffffffff, FONT_SMALL, 0xff000000);

		}
	}


	Main main;
}
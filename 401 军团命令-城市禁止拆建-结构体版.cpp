// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2021/01/31 # 江东新风 # rand函数错误修复 ##
// ## 2021/01/20 # 江东新风 # 将禁止拆建的信息保存到结构体 ##
// ## 2021/01/14 # 江东新风 # 整合ai据点设定内容，包括人才移动和低兵力禁止出征 ##
// ## 2020/12/24 # 江东新风 # 修复trace参数报错 ##
// ## 2020/08/09 # 氕氘氚 # 修正无法点击的bug，加入【军团命令】菜单 ##
// ## 2020/08/07 #messi#改成利於理解的表述##
// ## 2020/07/26 ##
namespace 철거금지
{
	//------ai优化据点命令设定的新增内容-------------------------------------------------------------------------
	// 유저 설정 (true = on, false = off)
	const bool 玩家_委任军团_武将转移_人才设定联动 = true;   // 根据委任军团配置菜单中的人才许可与否决定是否禁用后方武将移动위임군단 설정 메뉴의 인재 허가/금지에 따라 이동/소환 명령 연동하기
	const bool 玩家_委任军团_武将移动_总是禁止 = true;   // 人才设定联动:只有在false时才有效，禁止委任军团武将移动召唤命令인재설정연동=false 일 경우에만 동작, 위임군단의 이동/소환 명령 사용금지 설정

	const bool AI据点_低兵力防御_禁止部队出征 = true;        // 如果据点兵力不足标准兵力，战斗部队将被禁止出征进行防御 거점 병력이 기준병력 미만이 될 경우 전투부대 방어 출진금지
	const int  AI据点_低兵力防御_兵力标准_城市 = 5000;  // 거점병력 설정된 기준값 미만이 될 경우 방어 출진금지됨
	const int  AI据点_低兵力防御_兵力标准_港关 = 3000;  // 거점병력 설정된 기준값 미만이 될 경우 방어 출진금지됨

	const bool AI据点_低兵力攻略_禁止部队出征 = true;        // 据点兵力不足标准兵力时，战斗部队攻略禁止出征 거점 병력이 기준병력 미만이 될 경우 전투부대 공략 출진금지
	const int  AI据点_低兵力攻略_兵力标准_城市 = 5000;  // 거점병력 설정된 기준값 미만이 될 경우 공략 출진금지됨
	const int  AI据点_低兵力攻略_兵力标准_港关 = 3000;  // 거점병력 설정된 기준값 미만이 될 경우 공략 출진금지됨

	//---------------------------------------------------------------------------------------	

	const int KEY = pk::hash("철거금지");
	int counter = 0;

	class Main
	{

		Main()
		{
			pk::bind(102, pk::trigger102_t(onGameInit));
			//pk::bind(105, pk::trigger105_t(onSave));
			pk::bind(111, pk::trigger111_t(onTurnStart));
			pk::bind(112, pk::trigger112_t(onTurnEnd));

			@func256_ = cast<pk::func256_t@>(pk::get_func(256));
			pk::reset_func(256);
			pk::set_func(256, pk::func256_t(func256));

			@func257_ = cast<pk::func257_t@>(pk::get_func(257));
			pk::reset_func(257);
			pk::set_func(257, pk::func256_t(func257));

			add_menu();
		}

		void add_menu()
		{
			pk::menu_item item;
			item.menu = global_menu::菜单_军团;
			item.shortcut = global_menu::shortcut_禁止拆建;
			item.init = pk::building_menu_item_init_t(init);
			item.is_visible = pk::menu_item_is_visible_t(isVisible);
			// item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_1);//測試用，根据isEnabled_1判断是否已经执行過
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.get_image_id = pk::menu_item_get_image_id_t(getImageID);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		bool isEnabled(int force_id, int city_id)
		{
			return (enabled_[force_id] & (uint64(1) << city_id)) != 0;  //或?是指判?enabled那?右?第?位不?0的意思
		}

		void set(int force_id, int city_id, bool value)
		{
			if (value)
				enabled_[force_id] = enabled_[force_id] | uint64(1) << city_id;//把右数第几位变成1
			else
				enabled_[force_id] = enabled_[force_id] & ~(uint64(1) << city_id);//把右数敌几位变成0
		}

		void toggle(int force_id, int city_id)//按jian的开关设置，如果是开就变成关，如果是关就变成开
		{
			BaseInfo@ base_t = @base_ex[city_id];

			if (base_t.can_build[force_id])
			{
				base_t.can_build[force_id] = false;
				
				//set(force_id, city_id, false);
				if (force_id == pk::get_current_turn_force_id())
				{
					for (int facility_id = 0; facility_id < 시설_끝; facility_id++)
					{
						min_[city_id][facility_id] = int(pk::core["ai.develop.min_facility"][city_id][facility_id]);
						pk::core["ai.develop.min_facility"][city_id][facility_id] = 0;
					}
				}
			}
			else
			{
				base_t.can_build[force_id] = true;
				
				if (force_id == pk::get_current_turn_force_id())
				{
					for (int facility_id = 0; facility_id < 시설_끝; facility_id++)
					{
						pk::core["ai.develop.min_facility"][city_id][facility_id] = min_[city_id][facility_id];
					}
				}
			}
		}

		void onGameInit()
		{
			for (int i = 0; i < 势力_末; i++)
				enabled_[i] = uint64(pk::load(KEY, i, uint64(-1)));
			// 불러오기를 한 경우 턴 시작 트리거를 건너뛰므로 한 번 실행해줌.如果已加載剧本，则跳過回合触发器，因此将执行一次
			if (pk::get_scenario().loaded)
				onTurnStart(pk::get_force(pk::get_current_turn_force_id()));
		}
/*
		void onSave(int file_id)
		{
			for (int i = 0; i < 势力_末; i++)
				pk::store(KEY, i, enabled_[i]);
		}*/

		void onTurnStart(pk::force@ force)
		{
			// 최소시설수는 철거뿐만 아니라 개발, 공격 명령에도 관련되어 있음.最小设施数不仅与?chai除有关，且与发展和进攻命令有关
			if (force.is_player())
			{
				int force_id = force.get_id();
				for (int city_id = 0; city_id < 도시_끝; city_id++)
				{
					BaseInfo@ base_t = @base_ex[city_id];
					if (pk::get_city(city_id).get_force_id() == force_id)
					{
						if (!base_t.can_build[force_id])
						{
							//enabled_[force_id] = enabled_[force_id] | uint64(1) << city_id;//測試用，回合开始时恢复次数
							for (int facility_id = 0; facility_id < 시설_끝; facility_id++)
							{
								min_[city_id][facility_id] = int(pk::core["ai.develop.min_facility"][city_id][facility_id]);
								pk::core["ai.develop.min_facility"][city_id][facility_id] = 0;
							}
						}
					}
					else
					{
						// 도시를 빼앗긴 경우 리셋 都市被夺走时重置
						base_t.can_build[force_id] = true;
						
					}
				}
			}
			onTurnStart_据点命令设定(force);
		}

		void onTurnEnd(pk::force@ force)
		{
			if (force.is_player())
			{
				int force_id = force.get_id();
				for (int city_id = 0; city_id < 도시_끝; city_id++)
				{
					if (pk::get_city(city_id).get_force_id() == force_id)
					{
						BaseInfo@ base_t = @base_ex[city_id];
						//if ((enabled_[force_id] & 1 << city_id) == 0)
						if (!base_t.can_build[force_id])
						{
							for (int facility_id = 0; facility_id < 시설_끝; facility_id++)
							{
								pk::core["ai.develop.min_facility"][city_id][facility_id] = min_[city_id][facility_id];
							}
						}
					}
				}
			}
		}

		void func256(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 거점AI_철거)
			{
				pk::city@ city = pk::building_to_city(building);
				BaseInfo@ base_t = @base_ex[city.get_id()];
				//pk::trace(pk::format("据点id：{}，是否可拆建：{}", building.get_id(), base_t.can_build[city.get_force_id()]));
				if (city !is null and !base_t.can_build[city.get_force_id()])
				{
					//pk::trace(pk::format("据点id：{}，将行动力设为0", building.get_id(), base_t.can_build[city.get_force_id()]));
					prev_ap_ = context.ap;
					context.ap = 0;
				}
			}
			callback_256(context, building, cmd);
			if (func256_ !is null)
				func256_(context, building, cmd);
		}

		void func257(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			if (cmd == 거점AI_철거)
			{
				pk::city@ city = pk::building_to_city(building);
				BaseInfo@ base_t = @base_ex[city.get_id()];
				if (city !is null and !base_t.can_build[city.get_force_id()])
				{
					context.ap = prev_ap_;
				}
			}
			callback_257(context, building, cmd);
			if (func257_ !is null)
				func257_(context, building, cmd);
		}

		void init(pk::building@ building)
		{
			menu_city_id_ = -1;
			pk::city@ city = pk::building_to_city(building);
			if (city !is null)
				menu_city_id_ = city.get_id();
			menu_force_id_ = building.get_force_id();
		}

		bool isVisible()
		{
			return menu_city_id_ != -1 and menu_force_id_ == pk::get_current_turn_force_id();
		}
		bool isEnabled_1()//測試用，根据isEnabled_1判断是否已经执行過
		{
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			return base_t.can_build[menu_force_id_];
		}
		string getText()
		{
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			return base_t.can_build[menu_force_id_] ? pk::encode("允许拆建") : pk::encode("禁止拆建");//根据开关決定按鍵文本
		}

		string getDesc()
		{
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			return base_t.can_build[menu_force_id_] ? pk::encode("当前势力军团城市允许自由建设拆除.") : pk::encode("当前势力军团城市禁止自由建设拆除.");//根据开关決定描述
		}

		int getImageID()
		{
			BaseInfo@ base_t = @base_ex[menu_city_id_];
			return base_t.can_build[menu_force_id_] ? 889 : 1249;//根据开关決定按jian色
		}

		bool handler()
		{
			toggle(menu_force_id_, menu_city_id_);
			//pk::add_interior_land(pk::point(83,142));
			//output850name();
			return true;
		}

		// 철거 가능 여부是否可以拆除，按势力
		array<uint64> enabled_(势力_末, uint64(-1));
		// 인공지능 최소시설수AI最小设施数
		array<array<int>> min_(도시_끝, array<int>(시설_끝, -2));
		//
		pk::func256_t@ func256_;
		//
		pk::func257_t@ func257_;
		//
		int prev_ap_;
		// 건물 메뉴 도시建筑菜单都市
		int menu_city_id_;
		// 건물 메뉴 세력建筑菜单势力
		int menu_force_id_;

		//------ai优化据点命令设定的新增内容-------------------------------------------------------------------------

		int temp_ap;

		array<array<bool>> arr_cmd_canceled(据点_末, array<bool>(据点AI_末, false));//为了防止溢出，此处28得改成和新的命令id上限一致
		array<int> arr_building_ap(据点_末, 0);

		void callback_256(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			//pk::trace(pk::format("256 ai building_id:{},cmd:{}", pk::decode(pk::get_name(building)),ch::get_cmd_name(cmd)));
			if (!pk::is_campaign() and building !is null and building.get_id() < 据点_末)
			{
				int base_id = building.get_id();
				pk::district@ district = pk::get_district(building.get_district_id());
				arr_building_ap[base_id] = context.ap;  // 行动力 저장

				// 플레이어 위임군단 무장이동금지 설정
				if ((cmd == 据点AI_移动 or cmd == 据点AI_召唤) and !arr_cmd_canceled[base_id][cmd])
				{
					if (building.is_player() and !pk::is_player_controlled(building))
					{
						if (玩家_委任军团_武将转移_人才设定联动 and !district.personnel)
							cancel_cmd_256(context, building, cmd);   // 위임군단 인재금지 시 무장이동/소환 취소
						else if (玩家_委任军团_武将移动_总是禁止)
							cancel_cmd_256(context, building, cmd);
					}
				}
				// 거점 방어 출진 금지설정
				if (cmd == 据点AI_防御 and !arr_cmd_canceled[base_id][cmd])
				{
					if (AI据点_低兵力防御_禁止部队出征)
					{
						int min_troops;
						if (base_id < 城市_末)
							min_troops = pk::max(0, AI据点_低兵力防御_兵力标准_城市);
						else if (建筑_关卡开始 <= base_id and base_id < 建筑_港口末)
							min_troops = pk::max(0, AI据点_低兵力防御_兵力标准_港关);

						if (pk::get_troops(building) < min_troops)
							cancel_cmd_256(context, building, cmd);
					}
				}
				// 거점 공략 출진 금지설정
				if (cmd == 据点AI_攻击 and !arr_cmd_canceled[base_id][cmd])
				{
					if (AI据点_低兵力攻略_禁止部队出征)
					{
						int min_troops;
						if (base_id < 城市_末)
							min_troops = pk::max(0, AI据点_低兵力攻略_兵力标准_城市);
						else if (建筑_关卡开始 <= base_id and base_id < 建筑_港口末)
							min_troops = pk::max(0, AI据点_低兵力攻略_兵力标准_港关);

						if (pk::get_troops(building) < min_troops)
							cancel_cmd_256(context, building, cmd);
					}
				}
			}

		}

		//---------------------------------------------------------------------------------------

		// 257함수 : AI 명령실행 후 설정변경
		void callback_257(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			int base_id = building.get_id();

			// 명령 取消完毕 후 行动力 복원처리
			if (arr_cmd_canceled[base_id][cmd] and context.ap == 0)
			{
				context.ap = temp_ap;   // 行动力 복원
				arr_cmd_canceled[base_id][cmd] = false;

				if (调试模式) pk::info(pk::format("---据点_AI: {}, 行动力({}), {} 取消完毕", pk::decode(pk::get_name(building)), context.ap, get_cmd_info(cmd)));
			}

			// 行动力 감소한 경우, 명령실행한 것으로 간주
			if (context.ap < arr_building_ap[base_id])
			{
				if (调试模式) pk::info(pk::format("---据点_AI: {}, 行动力({}→{}), {} 实行完成", pk::decode(pk::get_name(building)), arr_building_ap[base_id], context.ap, get_cmd_info(cmd)));
			}

		}


		// 명령 취소를 위한 行动力 강제변경 함수
		void cancel_cmd_256(pk::ai_context@ context, pk::building@ building, int cmd)
		{
			temp_ap = context.ap;
			context.ap = 0;     // 行动力 백업 후 0 으로 처리

			int base_id = building.get_id();
			arr_cmd_canceled[base_id][cmd] = true;

			if (调试模式) pk::info(pk::format("---据点_AI: {}, 行动力({}), {} 取消设定", pk::decode(pk::get_name(building)), temp_ap, get_cmd_info(cmd)));
		}

		// 턴시작 시 명령금지설정 정보 초기화
		void onTurnStart_据点命令设定(pk::force@ force)
		{
			if (pk::is_campaign()) return;

			for (int i = 0; i < 据点_末; i++)
			{
				for (int j = 0; j < 28; j++)
					arr_cmd_canceled[i][j] = false;
			}

		}

		//---------------------------------------------------------------------------------------

		string get_cmd_info(int cmd)
		{
			string str_cmd;
			switch (cmd)
			{
			case 1: str_cmd = "开发"; break;
			case 2: str_cmd = "征兵"; break;
			case 3: str_cmd = "生产"; break;
			case 4: str_cmd = "巡查"; break;
			case 5: str_cmd = "交易"; break;
			case 6: str_cmd = "训练"; break;
			case 7: str_cmd = "防御"; break;
			case 8: str_cmd = "攻击"; break;
			case 9: str_cmd = "设置"; break;
			case 10: str_cmd = "港关运输"; break;
			case 11: str_cmd = "城市运输"; break;
			case 12: str_cmd = "移动"; break;
			case 13: str_cmd = "召唤"; break;
			case 14: str_cmd = "人才探索"; break;
			case 15: str_cmd = "武将登用"; break;
			case 16: str_cmd = "褒奖"; break;
			case 17: str_cmd = "二虎竞食"; break;
			case 18: str_cmd = "驱虎吞狼"; break;
			case 19: str_cmd = "流言"; break;
			case 20: str_cmd = "他势力武将登用"; break;
			case 21: str_cmd = "拆除"; break;
			case 22: str_cmd = "计谋"; break;
			case 23: str_cmd = "最小人才探索"; break;
			case 24: str_cmd = "最小人才登用"; break;
			case 25: str_cmd = "最小他势力武将登用"; break;
			case 26: str_cmd = "交易2"; break;
			case 27: str_cmd = "吸收合并"; break;
			}
			return str_cmd;
		}

		//---------------------------------------------------------------------------------------

		bool 调试模式 = false;
	}


	Main main;
}
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/02/13 # 江东新风 # ai针对ai的战术调整 ##
// ## 2021/11/15 # 江东新风 # 改善ai人才积极性 ##
// ## 2021/10/18 # 江东新风 # 部分变量汉化 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/11/10 # messi # 同步泽纳ai后方低于xx数量武将不会擅自移动 ##
/*
*** Notice
본 스크립트에 대한 재배포, 무단수정을 금지 합니다.
개선을 빙자한 잘라먹기, 덮어씌우기 방식의 별도 패치 역시 거부 합니다.
필요사항 경우 쪽지나 메일을 통해 요청을 주시면 검토 후 반영하겠습니다.

*** 제작
 keehl102 -	삼국지 도원결의	(https://cafe.naver.com/sam10)
	제르나 -	삼국지 공방		(https://cafe.naver.com/rtkbank)
			삼국지 도원결의	(https://cafe.naver.com/sam10)

*** 환경
	삼국지11 PK - PK2.1

*** History ***
--------------------------------------------------------------------
		작 업 자			| 수정 일자	|		작 업 내 용
--------------------------------------------------------------------
	keehl102- 도원결의		| 18.03.07	| 초기생성
	제르나	- 공방		| 19.01.01	| v1.0
									| 위임도시 무장이동 금지
	제르나	- 공방		| 20.09.17	| v1.1
									| 인코딩 변경
	제르나	- 공방		| 20.10.18	| v1.2
									| AI 후방지역 이동제한
*/

namespace AI_BUILDING_SET_ORDER
{
	class Main
	{
		// 수정: 제르나
		// 내용: AI 후방지역 무장이동 제한 - 도시 대 무장수 일정 % 미만의 경우 이동안함
		bool	use_minimum_officer = true;
		int		minimum_officer_rate = 25;		// 비율: 1 ~ 100% ex) 총무장 20명, 도시 2개 상황에서 50% 설정 시 무장 수 5명 (20 / 2 * 0.50) 이하의 후방 도시는 이동안함
		bool 调试模式 = true;
		Main()
		{
			pk::set_func(255, pk::func255_t(callback));
		}

		void callback(pk::ai_context@ context, pk::building@ building)
		{
			int building_id = building.get_id();
			pk::ai_context_base@ base = context.base[building_id];
			if (调试模式)
			{
				for (int i = 0; i < 据点_末; ++i)
				{
					pk::ai_context_base@ baset = context.base[i];
					string t = "ai_据点："+ pk::get_new_base_name(building_id) + ":" + pk::get_new_base_name(i)+"据点状态：" + baset.status + ",己方部队数：" + baset.num_own_units + ",敌方部队数：" + baset.num_enemy_units + ",玩家部队数:" + baset.num_player_units;
					pk::trace(t);
				}
				for (int i = 0; i < context.target_count; ++i)
				{
					//string t = "target_count:" + context.target_count;
					//type 1 攻击？ 2 防御？
					int target = context.target[i];
					string t = pk::get_new_base_name(building_id) + "，目标" + i + ":" + pk::get_new_base_name(target) + ",type:" + context.type[i] + ",已出征部队：";
					for (int j = 0; j < 16; ++j)
					{
						int unit_id = context.get_march_unit_id(i,j);
						if (pk::is_valid_unit_id(unit_id))
						{
							string tt = pk::decode(pk::get_name(pk::get_unit(unit_id)));
							t += j + ":" + tt + ",";
						}
					}

					pk::trace(t);
					/*
					pk::ai_target_info@ target_info = context._2164[i];
					if (target_info.target != -1)
					{
						string t = "target_count:" + context.target_count + "目标" + pk::get_new_base_name(target_info.target) + "类型：" + target_info.type;
						for (int j = 0; j < 16; ++j)
						{
							if (target_info.march_unit_id[j] != -1) t += "部队" + j + ":" + target_info.march_unit_id[j];
						}
						pk::trace(t);
					}*/
				}
			}


			// 거점 주변 3칸 안에 적 있음
			if (base.enemies_around3)
			{
				//可以直接在此处限制据点无脑防御
				pk::int_int temp = get_around_enemy_troops(building);//troops_all, player_troops
				bool has_player = (temp.second > 0 or pk::is_in_screen(building.pos));
				bool enhanced_defence = (has_player or pk::rand_bool(20));
				int src_troops = pk::get_troops(building);
				if (enhanced_defence)
				{
					if (开启人口系统) { if (base_ex[building_id].mil_pop_all > 1000) context.push_cmd(据点AI_募兵); }
					else context.push_cmd(据点AI_募兵);
					context.push_cmd(据点AI_城墙维修);

					//还得考虑耐久不足的情况倾巢而出，以及如何在不使用防御指令的情况下，通过募兵及修复耐久等方式抵抗
					if ((src_troops - 8000) > temp.first and (src_troops - 8000) > (temp.second * 1.5f))
					{
						//兵力足够时的守城方式
						context.push_cmd(据点AI_防御);
						if (enhanced_defence)
							context.push_cmd(据点AI_部队奇袭攻具);
						context.push_cmd(据点AI_计略奇袭攻具);
						context.push_cmd(据点AI_计略攻心);
						if (pk::rand_bool(50))context.push_cmd(据点AI_计略扰乱);
						else context.push_cmd(据点AI_计略伪报);
						context.push_cmd(据点AI_计略火攻);
						context.push_cmd(据点AI_计略待劳);
					}
					else
					{
						if (调试模式) pk::trace("src_troops - 8000" + (src_troops - 8000) + "temp.first" + temp.first + "temp.second * 1.5f" + (temp.second * 1.5f));
						if (pk::is_in_screen(building.pos)) pk::say(pk::encode("敌军来势汹汹，暂时固守待变。"), pk::get_person(pk::get_taishu_id(building)));
						context.push_cmd(据点AI_部队奇袭攻具);
						context.push_cmd(据点AI_计略奇袭攻具);
						float hp_rate = float(building.hp) / float(pk::get_max_hp(building));
						float bonus = (pk::get_city_list(context.force).count == 1) ? 0.1f : 0.f;
						if (hp_rate < (0.35f + bonus))//城墙耐久临界和攻城兵器数量挂钩
						{
							bool 反击 = false;
							if (pk::get_troops(building) > 30000) 反击 = true;
							if (hp_rate < (0.2f + bonus / 2) and pk::get_troops(building) > 10000) 反击 = true;
							if (hp_rate < (0.07f + bonus / 3) and pk::get_troops(building) > 5000) 反击 = true;
							if (反击)
							{
								if (pk::is_in_screen(building.pos))
								{
									switch (pk::rand(2))
									{
									case 0: pk::say(pk::encode("城墙即将坍塌，只能背水一战了。"), pk::get_person(pk::get_taishu_id(building))); break;
									case 1: pk::say(pk::encode("敌军也已疲敝，正面决战吧。"), pk::get_person(pk::get_taishu_id(building))); break;
									}

								}
								context.push_cmd(据点AI_防御);
							}

						}
						context.push_cmd(据点AI_计略攻心);
						if (pk::rand_bool(50))context.push_cmd(据点AI_计略扰乱);
						else context.push_cmd(据点AI_计略伪报);
						context.push_cmd(据点AI_计略火攻);
						context.push_cmd(据点AI_紧急动员);
						context.push_cmd(据点AI_民兵);
						context.push_cmd(据点AI_计略鼓舞);
						context.push_cmd(据点AI_计略镇静);
						context.push_cmd(据点AI_计略待劳);
					}
				}
				else context.push_cmd(据点AI_防御);

				if (building_id >= 建筑_城市开始 and building_id < 建筑_城市末)
				{
					context.push_cmd(据点AI_征兵);
					context.push_cmd(据点AI_巡察);
					context.push_cmd(据点AI_训练);
					context.push_cmd(据点AI_生产);
					context.push_cmd(据点AI_拆除);
				}
				return;
			}

			// 명령 가능한 무장 없음
			if (base.num_idle_officers == 0)
				return;

			// 도시
			if (building_id >= 建筑_城市开始 and building_id < 建筑_城市末)
			{
				pk::district@ district = pk::get_district(building.get_district_id());
				int force_id = context.force.get_id();
					
				if (force_ex[force_id].attack_delay == 0) context.push_cmd(据点AI_联合军出征);

				//if (building.is_player()) pk::trace(pk::format("据点id：{}，所属军团行动力：{},据点状态：{}", building_id, district.ap, base.status));
				//据点状态_后方：距离前线2以上；据点状态_警戒： 距离前线1；据点状态_前线：前线；据点状态_战斗：区域内有敌军
				switch (base.status)
				{
				case 据点状态_后方:
					if (!building.is_player()) context.push_cmd(据点AI_解除分封);
					context.push_cmd(据点AI_巡察);
					context.push_cmd(据点AI_褒奖);
					if (need_officer(context, building))
					{
						int count = get_order_count_need(context, building, 据点AI_人才探索);
						for (int i = 0; i < count; ++i) context.push_cmd(据点AI_人才探索);
						count = get_order_count_need(context, building, 据点AI_武将登用);
						for (int i = 0; i < count; ++i) context.push_cmd(据点AI_武将登用);
						count = get_order_count_need(context, building, 据点AI_他势力武将登用);
						for (int i = 0; i < count; ++i) context.push_cmd(据点AI_他势力武将登用);
					}
					//context.push_cmd(据点AI_最小人才探索);
					//context.push_cmd(据点AI_最小武将登用);
					//context.push_cmd(据点AI_最小他势力武将登用);
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_交易);
					context.push_cmd(据点AI_计略);
					context.push_cmd(据点AI_生产);
					context.push_cmd(据点AI_都市运输);
					context.push_cmd(据点AI_关卡运输);
					if (check_can_move(base,district)) context.push_cmd(据点AI_移动);
					context.push_cmd(据点AI_交易2);
					context.push_cmd(据点AI_征兵);
					context.push_cmd(据点AI_训练);
					context.push_cmd(据点AI_拆除);
					context.push_cmd(据点AI_开发);
					context.push_cmd(据点AI_吸收合并);
					context.push_cmd(据点AI_关卡运输);
					context.push_cmd(据点AI_人才探索);
					context.push_cmd(据点AI_武将登用);
					context.push_cmd(据点AI_他势力武将登用);
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_计略);
					break;

				case 据点状态_警戒:
					if (!building.is_player()) context.push_cmd(据点AI_解除分封);
					context.push_cmd(据点AI_攻击);
					context.push_cmd(据点AI_都市运输);
					context.push_cmd(据点AI_关卡运输);
					// 수정: 제르나	- 공방
					// 내용: 위임도시 무장이동 금지
					if (!building.is_player()) { context.push_cmd(据点AI_移动); }
					if (need_officer(context, building))
					{
						int count = get_order_count_need(context, building, 据点AI_人才探索);
						for (int i = 0; i < count; ++i) context.push_cmd(据点AI_人才探索);
						count = get_order_count_need(context, building, 据点AI_武将登用);
						for (int i = 0; i < count; ++i) context.push_cmd(据点AI_武将登用);
						count = get_order_count_need(context, building, 据点AI_他势力武将登用);
						for (int i = 0; i < count; ++i) context.push_cmd(据点AI_他势力武将登用);
					}
					//context.push_cmd(据点AI_最小人才探索);
					//context.push_cmd(据点AI_最小武将登用);
					//context.push_cmd(据点AI_最小他势力武将登用);
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_征兵);
					context.push_cmd(据点AI_巡察);
					context.push_cmd(据点AI_计略);
					context.push_cmd(据点AI_交易2);
					context.push_cmd(据点AI_交易);
					context.push_cmd(据点AI_生产);
					context.push_cmd(据点AI_训练);
					context.push_cmd(据点AI_拆除);
					//if (building.is_player()) pk::trace(pk::format("警戒据点id：{}，开发前，所属军团行动力：{},据点状态：{}", building_id, district.ap, base.status));
					context.push_cmd(据点AI_开发);
					//if (building.is_player()) pk::trace(pk::format("警戒据点id：{}，开发后，所属军团行动力：{},据点状态：{}", building_id, district.ap, base.status));
					context.push_cmd(据点AI_吸收合并);
					context.push_cmd(据点AI_关卡运输);
					context.push_cmd(据点AI_人才探索);
					context.push_cmd(据点AI_武将登用);
					context.push_cmd(据点AI_他势力武将登用);
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_计略);
					break;

				case 据点状态_前线:
					context.push_cmd(据点AI_攻击);
					if (pk::is_valid_base_id(ch::get_attacking_near_enemy_base(context, building))) context.push_cmd(据点AI_计略破坏);
					context.push_cmd(据点AI_支援关卡);
					context.push_cmd(据点AI_设置);
					context.push_cmd(据点AI_褒奖);
					if (need_officer(context, building))
					{
						int count = get_order_count_need(context, building, 据点AI_人才探索);
						for (int i = 0; i < count; ++i) context.push_cmd(据点AI_人才探索);
						count = get_order_count_need(context, building, 据点AI_武将登用);
						for (int i = 0; i < count; ++i) context.push_cmd(据点AI_武将登用);
						count = get_order_count_need(context, building, 据点AI_他势力武将登用);
						for (int i = 0; i < count; ++i) context.push_cmd(据点AI_他势力武将登用);
					}
					if (!building.is_player()) context.push_cmd(据点AI_分封);
					//context.push_cmd(据点AI_最小人才探索);
					//context.push_cmd(据点AI_最小武将登用);
					//context.push_cmd(据点AI_最小他势力武将登用);
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_征兵);
					context.push_cmd(据点AI_巡察);
					context.push_cmd(据点AI_训练);
					context.push_cmd(据点AI_计略);
					context.push_cmd(据点AI_交易2);
					context.push_cmd(据点AI_交易);
					context.push_cmd(据点AI_生产);
					context.push_cmd(据点AI_拆除);
					context.push_cmd(据点AI_开发);
					context.push_cmd(据点AI_吸收合并);
					context.push_cmd(据点AI_都市运输);
					// 수정: 제르나	- 공방
					// 내용: 위임도시 무장이동 금지
					if (!building.is_player()) { context.push_cmd(据点AI_移动); }
					context.push_cmd(据点AI_关卡运输);
					context.push_cmd(据点AI_人才探索);
					context.push_cmd(据点AI_武将登用);
					context.push_cmd(据点AI_他势力武将登用);
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_计略);
					break;

				case 据点状态_战斗:
					context.push_cmd(据点AI_防御);
					context.push_cmd(据点AI_分封);
					context.push_cmd(据点AI_支援关卡);
					context.push_cmd(据点AI_拆除);
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_征兵);
					context.push_cmd(据点AI_训练);
					context.push_cmd(据点AI_交易);
					context.push_cmd(据点AI_巡察);
					context.push_cmd(据点AI_生产);
					context.push_cmd(据点AI_计略);
					context.push_cmd(据点AI_交易2);
					context.push_cmd(据点AI_褒奖);
					if (need_officer(context, building))
					{
						int count = get_order_count_need(context, building, 据点AI_人才探索);
						for (int i = 0; i < count; ++i) context.push_cmd(据点AI_人才探索);
						count = get_order_count_need(context, building, 据点AI_武将登用);
						for (int i = 0; i < count; ++i) context.push_cmd(据点AI_武将登用);
						count = get_order_count_need(context, building, 据点AI_他势力武将登用);
						for (int i = 0; i < count; ++i) context.push_cmd(据点AI_他势力武将登用);
					}
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_计略);
					break;
				}
			}
			// 관문, 항구
			else
			{
			pk::district@ district = pk::get_district(building.get_district_id());
			//pk::trace(pk::format("据点id：{}，所属军团行动力：{}",building_id, district.ap));
				switch (base.status)
				{
				case 据点状态_后方:
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_都市运输);
					context.push_cmd(据点AI_关卡运输);
					// 수정: 제르나	- 공방
					// 내용: 위임도시 무장이동 금지
					if (!building.is_player()) { context.push_cmd(据点AI_移动); }
					context.push_cmd(据点AI_训练);
					context.push_cmd(据点AI_计略);
					context.push_cmd(据点AI_人才探索);
					context.push_cmd(据点AI_武将登用);
					context.push_cmd(据点AI_他势力武将登用);
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_计略);
					break;

				case 据点状态_警戒:
					context.push_cmd(据点AI_攻击);
					context.push_cmd(据点AI_都市运输);
					context.push_cmd(据点AI_关卡运输);
					// 수정 : 제르나	- 공방
					// 내용: 위임도시 무장이동 금지
					if (!building.is_player()) { context.push_cmd(据点AI_移动); }
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_训练);
					context.push_cmd(据点AI_计略);
					context.push_cmd(据点AI_人才探索);
					context.push_cmd(据点AI_武将登用);
					context.push_cmd(据点AI_他势力武将登用);
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_计略);
					break;

				case 据点状态_前线:
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_攻击);
					context.push_cmd(据点AI_设置);
					context.push_cmd(据点AI_训练);
					context.push_cmd(据点AI_都市运输);
					// 수정 : 제르나	- 공방
					// 내용: 위임도시 무장이동 금지
					if (!building.is_player()) { context.push_cmd(据点AI_移动); }
					context.push_cmd(据点AI_关卡运输);
					context.push_cmd(据点AI_计略);
					context.push_cmd(据点AI_人才探索);
					context.push_cmd(据点AI_武将登用);
					context.push_cmd(据点AI_他势力武将登用);
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_计略);
					break;

				case 据点状态_战斗:
					context.push_cmd(据点AI_防御);
					context.push_cmd(据点AI_训练);
					context.push_cmd(据点AI_都市运输);
					// 수정 : 제르나	- 공방
					// 내용: 위임도시 무장이동 금지
					if (!building.is_player()) { context.push_cmd(据点AI_移动); }
					context.push_cmd(据点AI_都市运输);
					context.push_cmd(据点AI_褒奖);
					context.push_cmd(据点AI_计略);
					break;
				}
			}
		}

		bool check_can_move(pk::ai_context_base@ base, pk::district@ district)
		{
			if (!district.is_player())
			{
				if (use_minimum_officer)
				{
					int	num_officer_total = pk::get_person_list(district, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)).count;
					int	num_officer_building = base.num_officers;
					int num_building = pk::get_city_list(district).count;

					if (num_officer_building > 0)
					{
						int	base_officer_num = int(float(num_officer_total) / float(num_building) * float(minimum_officer_rate) / 100);
						if (num_officer_building > base_officer_num)
							return true;
					}
				}
				else
				{
					return true;
				}
			}
			return false;
		}

		bool is_target_base(pk::ai_context@ context, int src_building_id, int dst_building_id)
		{
			pk::ai_context_base@ base = context.base[src_building_id];
			for (int i = 0; i < context.target_count; ++i)
			{
				if (context.type[i] == 1)//类型1为攻击
				{
					int target = context.target[i];
					if (target == dst_building_id) return true;
				}
			}
			return false;
				
		}

		//本该改最小武将登用那块更合理，不过那样改动更大。既然效果一样，就在这里改好了
		bool need_officer(pk::ai_context@ context, pk::building@ building)
		{
			int building_id = building.get_id();
			pk::ai_context_base@ base = context.base[building_id];
			//原设定
			/*
			int need_num = 2 * pk::get_scenario().difficulty + 3;//难易度0-2
			if (base.num_officers < need_num) return true;
			return false;
			*/
			//还是只对城市有效好了？
			int city_id = pk::get_city_id(building.get_pos());
			pk::city@ city = pk::get_city(city_id);

			int office_need = 7 + 2 * pk::get_scenario().difficulty;//难易度0-2
			int max_dst_officer = office_need;
			//前线和警戒据点根据敌方武将数设定登用人才上限
			if (city !is null)
			{
				for (int i = 0; i < 6; ++i)
				{
					int dst_city_id = city.neighbor[i];
					if (pk::is_valid_city_id(dst_city_id))
					{
						pk::city@ dst_city = pk::get_city(dst_city_id);
						if (pk::is_enemy(city, dst_city))
						{
							int person_count = pk::get_person_list(pk::city_to_building(dst_city), pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)).count;
							if (person_count > max_dst_officer) max_dst_officer = person_count;
						}
					}
				}
			}

			switch (base.status)
			{
			case 据点状态_后方: return (int(base.num_officers) < office_need); 
			case 据点状态_警戒: return (int(base.num_officers) < max_dst_officer);
			case 据点状态_前线: return (int(base.num_officers) < max_dst_officer); 
			case 据点状态_战斗: return (int(base.num_officers) < max_dst_officer); 
			default:
				break;
			}
			return false;
		}

		//研究发现，每次push一个指令都只会执行1次，所以针对未发现武将多，或者灭势力俘虏多的情况，加强对应指令数量
		int get_order_count_need(pk::ai_context@ context, pk::building@ building,int ai_order_id)
		{
			if (ai_order_id == 据点AI_人才探索)
			{
				int target_count = get_undetect_person_list(building).count;
				return pk::min(8,pk::max(0, int(ceil(target_count * 0.5f))));
			}
			else if (ai_order_id == 据点AI_武将登用)
			{
				int target_count = get_ai_employ_person_list(building).count;
				return pk::max(0, int(ceil(target_count * 0.5f)));
			}
			else if (ai_order_id == 据点AI_他势力武将登用)
			{
				int target_count = get_ai_employ_enemy_person_list(building).count;
				return pk::min(8,pk::max(0, int(ceil(target_count * 0.3f))));
			}
			return 1;
		}
		//下面三个函数都是完全按照ai获取对应武将对象的函数复刻的
		pk::list<pk::person@> get_undetect_person_list(pk::building@ building)
		{
			pk::list<pk::person@> list;
			for (int i = 0; i < 非贼武将_末; ++i)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person) and pk::is_undetect_person(person,building)) list.add(person);
			}
			return list;
		}

		pk::list<pk::person@> get_ai_employ_person_list(pk::building@ building)
		{
			pk::list<pk::person@> list;
			int src_force_id = building.get_force_id();
			int src_city_id = pk::get_city_id(building.pos);
			for (int i = 0; i < 非贼武将_末; ++i)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person) and person.get_force_id() != src_force_id)
				{
					bool is_same_base = false;
					if (person.mibun == 身份_在野)//在野也是有所属所在的
					{
						pk::building@ service_building = pk::get_building(person.service);
						if (pk::is_alive(service_building)) is_same_base = (service_building.get_id() == src_city_id);
					}
					else if (person.mibun == 身份_俘虏)
					{
						is_same_base = (person.location == building.get_id());
					}
					if (is_same_base) list.add(person);
				}
			}
			return list;
		}

		pk::list<pk::person@> get_ai_employ_enemy_person_list(pk::building@ building)
		{
			pk::list<pk::person@> list;
			int src_force_id = building.get_force_id();
			int src_city_id = pk::get_city_id(building.pos);
			auto src_force = pk::get_force(src_force_id);
			for (int i = 0; i < 非贼武将_末; ++i)
			{
				pk::person@ person = pk::get_person(i);
				if (pk::is_alive(person))
				{
					if (person.mibun == 身份_一般 or person.mibun == 身份_太守 or person.mibun == 身份_都督)
					{
						if (!pk::is_absent(person))
						{
							int dst_force_id = person.get_force_id();
							if (pk::is_valid_normal_force_id(dst_force_id) and dst_force_id != src_force_id and !src_force.ally[dst_force_id])//正常势力，非己方势力，非盟友势力
							{
								if (!pk::is_valid_pos(person.get_pos())) continue;
								int dst_city_id = pk::get_city_id(person.get_pos());
								if (person.loyalty > 90)
								{
									if (src_city_id == dst_city_id) { list.add(person); continue; }
									if (pk::has_neighbor(pk::building_to_city(building), dst_city_id)) { list.add(person); continue; }
								}
								else
								{
									if (src_city_id == dst_city_id) { list.add(person); continue; }
									int city_distance = pk::get_city_distance(src_city_id, dst_city_id);
									if (city_distance < 3) { list.add(person); continue; }
									if (city_distance == 3 and pk::rand_bool(25)) { list.add(person); continue; }
								}
							}
						}
					}
				}
			}
			return list;
		}

		pk::int_int get_around_enemy_troops(pk::building@ building, int range_max = 5)
		{
			int bonus = building.facility == 设施_城市 ? 1 : 0;
			array<pk::point> range_pos_arr = pk::range(building.pos, 1+ bonus, range_max +bonus);
			int troops_all = 0;
			int player_troops = 0;
			for (int arr_index = 0; arr_index < int(range_pos_arr.length); arr_index++)
			{
				pk::point range_pos = range_pos_arr[arr_index];
				if (!pk::is_valid_pos(range_pos)) continue;

				pk::hex@ hex = pk::get_hex(range_pos);
				if (!hex.has_unit) continue;
				pk::unit@ dst_unit = pk::get_unit(range_pos);
				if (dst_unit.type != 部队类型_战斗) continue; //排除运输部队
				int modify_troops = dst_unit.troops;
				if (check_elite_unit(dst_unit)) modify_troops = int(dst_unit.troops * 1.5f);
				if (dst_unit.get_force_id() == building.get_force_id()) modify_troops = -modify_troops;//己方部队作为减法，也就意味着最终算出来的是战力差
				if (!dst_unit.is_player()) player_troops += modify_troops;
				troops_all += modify_troops;
			}

			return pk::int_int(troops_all, player_troops);
		}

		bool check_elite_unit(pk::unit@unit)
		{
			pk::equipment@ equipment = pk::get_equipment(unit.weapon);
			uint8 elite_atk = uint8(equipment.stat[兵器能力_攻击] * 1.15f);
			uint8 elite_def = uint8(equipment.stat[兵器能力_防御] * 1.15f);
			if (elite_atk > unit.attr.stat[部队能力_攻击]) return true;
			if (elite_def > unit.attr.stat[部队能力_防御]) return true;
			return false;
		}
	}

	Main main;
}

// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/28 # 江东新风 # 单港关存活设定变更 ##
// ## 2021/10/10 # 江东新风 # 据点攻占后的人口增减 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/02/18 # 江东新风 # 调整吸收比例为魅力相关，最大20% ##
// ## 2021/02/18 # 江东新风 # 尝试修复单港关bug ##
// ## 2021/02/08 # 江东新风 # 单港关时，势力灭亡条件的修正，城市被占领后留有港关也可存活 ##
// ## 2021/01/24 # 江东新风 # 关闭单城市灭亡 ##
// ## 2021/01/14 # 江东新风 # 关掉trace ##
// ## 2021/01/05 # 江东新风 # 特殊地名设施争夺的完善2 ##
// ## 2020/12/26 # 江东新风 # 关掉trace ##
// ## 2020/12/25 # 江东新风 # 据点灭亡修正 ##
// ## 2020/12/23 # 江东新风 # 加入据点灭亡开关 ##
// ## 2020/12/17 # 江东新风 # 实验添加据点为势力最后据点时，势力灭亡，配合港口势力不灭亡的设定 ##
// ## 2020/08/14 # 江东新风 # 新特技-安民 ##
// ## 2020/08/05 # messi # 調低?領据点获取物資的参数 ##
// ## 2020/08/01 ##
/*
	@원본 : 168 거점 점령 후처리.cpp (PK2.1 기본 제공 파일)
	@수정 : masterpiecek

	- 유저설정 기본값은 PK2.1 원본과 동일한 상태입니다.


	[ 거점 점령 후처리 대상 및 모드 설명 ]

	1. 일반세력 공격부대가 점령한 거점의 후처리 : 거점의 자원 (금, 병량, 병력, 병기)
	 - 모드 1 : 吸收比例 魅力反应 - 거점 공격부대 주장의 매력이 반영된 자원 吸收比例을 설정
	 - 모드 2 : 吸收比例 直接设定 - 거점의 전체 자원 중 흡수할 고정比例을 직접 설정
	  * 거점을 점령한 공격부대가 플레이어 세력인지 컴퓨터 세력인지에 따라 각각 설정 가능
	  * 에볼루션킷 기존 설정 : 모드 2
	  * PK2.1 기존 설정 : 모드 1

	2. 일반세력 공격부대가 점령한 도시의 후처리 : 도시의 内政设施
	 * 모드별로 일정比例 또는 일정수의 内政设施을 무작위로 파괴하고 나머지를 공격부대 세력이 흡수하는 형태
	 - 모드 1 : 吸收个数 魅力反应 - 거점 공격부대 주장의 매력이 높을수록 파괴되지 않고 흡수하는 内政设施 수 증가
	 - 모드 2 : 파괴比例 直接设定 - 전체 内政设施 중 설정한 파괴比例의 개수만큼 파괴
	 - 모드 3 : 파괴확률 개별적용 - 설정한 파괴확률을 각 内政设施별로 판단하여 충족 시 파괴
	  * 거점을 점령한 공격부대가 플레이어 세력인지 컴퓨터 세력인지에 따라 각각 설정 가능
	  * 에볼루션킷 기존 설정 : 모드 1
	  * PK2.1 기존 설정 : 모드 1

	3. 일반세력 공격부대가 점령한 도시의 후처리 : 도시구역 내 군사시설 (공격부대 세력의 군사시설은 제외)
	 1) 城市势力 군사시설 : 점령한 도시 세력의 군사시설
	 2) 第三方势力 군사시설 : 공격부대 세력도 점령한 도시 세력도 아닌 第三方势力의 군사시설 처리
	  * 모드별로 일정 확률로 군사시설을 공격부대 세력으로 흡수하거나 파괴하고 나머지 군사시설은 기존의 세력으로 잔류시키는 형태
	  - 모드 1 : 处理概率 反应距离 - 도시로부터의 거리에 따라 시설의 흡수 및 파괴 확률 차등 적용
	  - 모드 2 : 处理概率 直接设定 - 전체 군사시설 중 설정한 吸收比例의 개수만큼 흡수, 파괴比例의 개수만큼 파괴
	  - 모드 3 : 处理概率 개별적용 - 설정한 吸收概率, 파괴확률을 각 군사시설별로 판단하여 충족 시 흡수 및 파괴
	   * 거점을 점령한 공격부대가 플레이어 세력인지 컴퓨터 세력인지에 따라 각각 설정 가능
	   * 에볼루션킷 기존 설정 : 모드 1 (단, 城市势力 군사시설만 적용)
	   * PK2.1 기존 설정 : 모드 2 (단, 城市势力 군사시설만 적용)

	4. 적장세력 공격부대가 점령한 도시의 후처리 : 内政设施, 군사시설(城市势力 및 第三方势力 군사시설)
	 * 에볼루션킷 기존 설정 : 内政设施 전부 파괴, 城市势力 군사시설 전부 파괴, 第三方势力 군사시설 그대로 잔류
	 * PK2.1 기존 설정 : 内政设施 전부 파괴, 城市势力 군사시설 전부 파괴, 第三方势力 군사시설 그대로 잔류

	5. 도시구역 내 其他设施의 후처리 : 함정 및 장벽 등
	 * 에볼루션킷 기존 설정 : 其他设施은 그대로 잔류
	 * PK2.1 기존 설정 : 其他设施은 그대로 잔류
*/

namespace KANRAKU
{

	/// ============================================== < 유저 설정 > ==============================================

	/// [ 거점 점령 후처리 : 거점자원 ]

		/// 资源吸收比例 모드
		// 모드 1 : 吸收比例 魅力反应 (PK2.1 원본과 동일모드)
		// 모드 2 : 吸收比例 直接设定 (에볼루션킷 원본과 동일모드)
	const array<int>
		资源吸收比例_模式 = {/*플레이어*/1, /*컴퓨터*/1 },

		/// 모드 1 : 吸收比例 魅力反应
		// 기본값 설명 : 매력이 80인 경우 80의 10%가 8이므로 전체 거점자원의 8%를 흡수 (吸收比例 최소 5%, 최대 100%)
		资源吸收比例_魅力反应比例 = {/*플레이어*/12, /*컴퓨터*/12 },
		资源吸收比例_最大吸收比例 = {/*플레이어*/20, /*컴퓨터*/20 },
		资源吸收比例_最小吸收比例 = {/*플레이어*/5, /*컴퓨터*/5 },

		/// 모드 2 : 吸收比例 直接设定
		// 기본값 설명 : 거점의 모든 자원에 대하여 자원별로 70%를 흡수
		资源吸收比例_金 = {/*플레이어*/30, /*컴퓨터*/30 },
		资源吸收比例_兵粮 = {/*플레이어*/30, /*컴퓨터*/30 },
		资源吸收比例_兵力 = {/*플레이어*/30, /*컴퓨터*/30 },
		资源吸收比例_兵器 = {/*플레이어*/30, /*컴퓨터*/30 },

		/// -----------------------------------------------------------------------------------------------------------

		/// [ 도시 점령 후처리 : 도시의 内政设施 ]

			/// 内政设施 처리모드
			// 모드 1 : 吸收个数 魅力反应 (PK.1, 에볼루션킷 원본과 동일모드)
			// 모드 2 : 파괴比例 直接设定
			// 모드 3 : 파괴확률 개별적용
		内政设施_处理模式 = {/*플레이어*/1, /*컴퓨터*/1 },

		/// 모드 1 : 吸收个数 魅力反应
		// 기본값 설명 : 매력이 70인 경우 70의 5%가 3.5인데 반올림하여 4개의 시설을 파괴하지 않고 흡수 (최소 0개, 최대 15개 흡수 가능)
		内政设施_吸收个数_魅力反应比例 = {/*플레이어*/5, /*컴퓨터*/5 },
		内政设施_吸收个数_최소 = {/*플레이어*/0, /*컴퓨터*/0 },
		内政设施_吸收个数_최대 = {/*플레이어*/15, /*컴퓨터*/15 },

		/// 모드 2 : 파괴比例 直接设定
		// 기본값 설명 : 전체 内政设施 수가 8개인 경우 8개의 60%가 4.8인데 반올림하여 5개를 파괴
		内政设施_破坏比例 = {/*플레이어*/60, /*컴퓨터*/60 },

		/// 모드 3 : 파괴확률 개별적용
		// 기본값 설명 : 각각의 内政设施별로 60%의 확률을 적용하여 확률을 충족하는 시설을 파괴
		内政设施별_破坏概率 = {/*플레이어*/60, /*컴퓨터*/60 },

		/// -----------------------------------------------------------------------------------------------------------

		/// [ 도시 점령 후처리 : 도시구역 내 군사시설 ]

			/// 군사시설 처리모드
			// 모드 1 : 处理概率 反应距离 (에볼루션킷 원본과 동일모드 - 단, 城市势力 군사시설만 적용)
			// 모드 2 : 处理概率 直接设定 (PK2.1 원본과 동일모드 - 단, 城市势力 군사시설만 적용)
			// 모드 3 : 处理概率 개별적용
		城市势力_军事设施_处理模式 = {/*플레이어*/1, /*컴퓨터*/1 },
		第三方势力_军事设施_处理模式 = {/*플레이어*/3, /*컴퓨터*/3 };

	/// 모드 1 : 处理概率 反应距离
	/* 기본값 설명 : 점령된 城市势力의 군사시설은 도시로부터 거리가 4이하인 경우 75%의 확률로 공격부대 세력으로 흡수하고 5%의 확률로 파괴,
					 거리가 4초과 8이하인 경우 50%의 확률로 공격부대 세력으로 흡수하고 30%의 확률로 파괴,
					 거리가 8초과인 경우 30%의 확률로 공격부대 세력으로 흡수하고 50%의 확률로 파괴
					 (공격부대 세력도 城市势力도 아닌 第三方势力 군사시설도 동일하게 설정된 상태)
					 (흡수되거나 파괴되지 않은 시설은 기존의 세력으로 잔류) */
	const array<array<int>>
		城市势力_军事设施_标准距离 = { /*플레이어*/{4, 8}, /*컴퓨터*/{4, 8} },
		城市势力_军事设施_按距离_吸收概率 = { /*플레이어*/{75, 50, 30}, /*컴퓨터*/{75, 50, 30} },
		城市势力_军事设施_按距离_破坏概率 = { /*플레이어*/{5, 30, 50}, /*컴퓨터*/{5, 30, 50} },
		第三方势力_军事设施_标准距离 = { /*플레이어*/{4, 8}, /*컴퓨터*/{4, 8} },
		第三方势力_军事设施_按距离_吸收概率 = { /*플레이어*/{75, 50, 30}, /*컴퓨터*/{75, 50, 30} },
		第三方势力_军事设施_按距离_破坏概率 = { /*플레이어*/{5, 30, 50}, /*컴퓨터*/{5, 30, 50} };

	/// 모드 2 : 处理概率 直接设定
	/* 기본값 설명 : 점령된 城市势力의 군사시설은 흡수(0%)하지 않고 모두 파괴(100%),
					 공격부대 세력도 城市势力도 아닌 第三方势力 군사시설은 흡수(0%) 및 파괴(0%)하지 않고 모두 기존의 세력으로 잔류
					 (比例에 따른 흡수 및 파괴할 시설의 개수는 반올림하여 결정) */
	const array<int>
		城市势力_军事设施_吸收比例 = {/*플레이어*/0, /*컴퓨터*/0 },
		城市势力_军事设施_破坏比例 = {/*플레이어*/100, /*컴퓨터*/100 },
		第三方势力_军事设施_吸收比例 = {/*플레이어*/0, /*컴퓨터*/0 },
		第三方势力_军事设施_破坏比例 = {/*플레이어*/0, /*컴퓨터*/0 },

		/// 모드 3 : 处理概率 개별적용
		/* 기본값 설명 : 점령된 城市势力의 군사시설은 각 군사시설별로 20%의 확률로 흡수하고 80%의 확률로 파괴,
						 공격부대 세력도 城市势力도 아닌 第三方势力 군사시설은 흡수 및 파괴하지 않고 모두 기존의 세력으로 잔류 */
		城市势力_按军事设施_吸收概率 = {/*플레이어*/20, /*컴퓨터*/20 },
		城市势力_按军事设施_破坏概率 = {/*플레이어*/80, /*컴퓨터*/80 },
		第三方势力_按军事设施_吸收概率 = {/*플레이어*/0, /*컴퓨터*/0 },
		第三方势力_按军事设施_破坏概率 = {/*플레이어*/0, /*컴퓨터*/0 };

	/// -----------------------------------------------------------------------------------------------------------

	/// [ 기타 설정 ]

		/// 적장세력이 도시점령 시 内政设施 및 군사시설(城市势力의 군사시설, 第三方势力의 군사시설) 처리 설정
		// 内政设施이 파괴되지 않도록 한 경우 内政设施은 세력없음으로 잔류하며 이후 점령한 세력으로 다시 흡수됨
	const bool 敌军势力_城市占领时_内政设施_破坏 = true;
	const bool 敌军势力_城市占领时_军事设施_破坏_城市势力 = true;
	const bool 敌军势力_城市占领时_军事设施_破坏_第三方势力 = false;

	/// 점령된 도시의 함정, 장벽 파괴여부 설정
	const bool 城市占领时_其他设施_破坏_陷阱 = false;
	const bool 城市占领时_其他设施_破坏_障碍物 = false;

	/// ===========================================================================================================

	const bool 디버깅 = false;
	//const bool 港关独存_测试 = true;

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

				// facility_index : 0(城市势力), 1(第三方势力)
		array<array<array<int>>> 军事设施_标准距离 = { 城市势力_军事设施_标准距离, 第三方势力_军事设施_标准距离 };
		array<array<array<int>>> 军事设施_按距离_吸收概率 = { 城市势力_军事设施_按距离_吸收概率, 第三方势力_军事设施_按距离_吸收概率 };
		array<array<array<int>>> 军事设施_按距离_破坏概率 = { 城市势力_军事设施_按距离_破坏概率, 第三方势力_军事设施_按距离_破坏概率 };
		array<array<int>> 军事设施_处理模式 = { 城市势力_军事设施_处理模式, 第三方势力_军事设施_处理模式 };
		array<array<int>> 军事设施_吸收比例 = { 城市势力_军事设施_吸收比例, 第三方势力_军事设施_吸收比例 };
		array<array<int>> 军事设施_破坏比例 = { 城市势力_军事设施_破坏比例, 第三方势力_军事设施_破坏比例 };
		array<array<int>>按军事设施_吸收概率 = { 城市势力_按军事设施_吸收概率, 第三方势力_按军事设施_吸收概率 };
		array<array<int>>按军事设施_破坏概率 = { 城市势力_按军事设施_破坏概率, 第三方势力_按军事设施_破坏概率 };

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main() { pk::set_func(168, pk::func168_t(callback)); }

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

				/**
				@param base     : 점령한 거점
				@param attacker : 공격 부대
				*/
		void callback(pk::building@ base, pk::unit@ attacker)
		{
			if (!pk::is_alive(base)) return;
			if (!pk::is_alive(attacker)) return;

			pk::person@ attacker_leader = pk::get_person(attacker.leader);
			pk::force@ attacker_force = pk::get_force(attacker.get_force_id());
			pk::force@ base_force = pk::get_force(base.get_force_id());

			if (pk::get_onebase_survive())
			{
				//测试最后据点灭亡势力,只有一座城市时也要灭亡
				//攻占后设定的触发在俘虏之后
				//因为势力灭亡的设定被抹去了，所以当城市数只有一个时被攻占也意味着势力灭亡，需要处理
				if (base.facility == 0 and base.get_force_id() != -1)
				{
					if (pk::get_city_list(base_force).count < 2)
					{
						//bool has_base = false;
						int base_num = 0;
						for (int building_id = 城市_末; building_id < 据点_末; building_id++)
						{
							pk::building@ building0 = pk::get_building(building_id);
							if (building0.get_force_id() == base_force.get_id())
							{
								base_num += 1;

							}
							//pk::trace(pk::format("据点攻占后，城市，剩余据点数{},据点id{}",base_num, building_id));
						}
						if (base_num < 1) pk::kill(base_force, false, attacker_force);
							
					}
				}
				if ((base.facility == 1 or base.facility == 2) and base.get_force_id() != -1)
				{
					if (pk::get_city_list(base_force).count < 1)
					{
						//bool has_base = false;
						int base_num = 0;
						for (int building_id = 城市_末; building_id < 据点_末; building_id++)
						{
							pk::building@ building0 = pk::get_building(building_id);
							if (building0.get_force_id() == base_force.get_id())
							{
								base_num += 1;

							}
							//pk::trace(pk::format("据点攻占后，城市，剩余据点数{},据点id{}", base_num, building_id));
						}
						if (base_num < 1) pk::kill(base_force, false, attacker_force);
					}
				}
			}

			int charisma = (pk::is_alive(attacker_leader)) ? attacker_leader.stat[武将能力_魅力] : 20;
			
			if (开启人口系统)
			{
				int base_id = base.get_id();
				//攻击方将领魅力的影响
				float leader_eff;
				if (charisma < 30) leader_eff = -0.3f;
				else if (charisma < 40) leader_eff = -0.2f;
				else if (charisma < 50) leader_eff = -0.1f;
				else if (charisma < 80) leader_eff = 0;
				else if (charisma < 90) leader_eff = 0.1f;
				else if (charisma < 100) leader_eff = 0.2f;
				else leader_eff = 0.3f;

				if (base_id > -1 and base_id < 据点_城市末)
				{
					BaseInfo@ base_t = @base_ex[base_id];
					if (base_t.population > 10000)
					{
						base_t.return_pop = uint32(pk::min(800000.f, base_t.return_pop + base_t.population * (0.5f - leader_eff) *0.6f));//返回人口的设定,最多返还60%
						base_t.population = int(base_t.population * (0.5f+ leader_eff));
					}
					else
					{
						//if (base_id == 城市_襄阳) pk::trace(pk::format("城市占领,襄阳人口：{}", base_t.population));
						base_t.population = 5000;//人口保底5000
					}
					uint32 mil_pop_all = uint32(pk::max(0.f, pk::min(base_t.mil_pop_all - 1000 * (1.f - leader_eff), base_t.mil_pop_all * (0.7f + leader_eff))));
					uint32 mil_pop_av = uint32(pk::max(0.f, pk::min(base_t.mil_pop_av - 1000 * (1.f - leader_eff), base_t.mil_pop_av * (0.7f + leader_eff))));

					
					base_t.mil_pop_all = mil_pop_all;
					base_t.mil_pop_av = mil_pop_av;
					//添加据点被攻占后标志，达成条件后可产生人口恢复buf
					// 9回合城市建筑量达7个，触发buf，人口回归
					base_t.return_timer = 9;
					base_t.return_buf = false;
					
				}
				else if (base_id >= 据点_关卡开始 and base_id < 据点_据点末)
				{
					//BaseInfo@ base_t = @base_ex[base_id];
				}
				
			}
			
			func_디버깅(attacker, attacker_force, attacker_leader, base, base_force);
			func_점령거점_후처리_거점자원(base, attacker, charisma);

			pk::city@ city = pk::building_to_city(base);
			if (!pk::is_alive(city)) return;

			pk::list<pk::building@> 内政设施_待处理_列表;
			pk::list<pk::building@> 城市势力_军事设施_待处理_列表;
			pk::list<pk::building@> 第三方势力_军事设施_待处理_列表;

			for (int building_id = 0; building_id < 건물_끝; building_id++)
			{
				pk::building@ building = pk::get_building(building_id);
				if (!pk::is_alive(building)) continue;
				if (!is_target_facility(city, attacker, building)) continue;

				if (pk::is_normal_force(attacker.get_force_id()))
				{
					if (pk::get_facility_type(building) == 设施类型_内政设施)
					{
						内政设施_待处理_列表.add(building);
					}
					else if (pk::get_facility_type(building) == 设施类型_军事设施)
					{
						if (building.get_force_id() == city.get_force_id())
						{
							//需要将特殊地名设施排除在列表外
							bool except0 = false;
							if (特定地点特定设施可争夺)
							{
								//pk::trace(pk::format("pos 1,building_id:{}",building_id ));
								if (地名设施.find(building.facility) >= 0)
								{
									//pk::trace(pk::format("pos 2,building_id:{}",building_id ));
									for (int i = 0; i < int(特殊地名设施.length); i++)
									{

										pk::point pos0;
										pos0.x = 特殊地名设施[i][1];
										pos0.y = 特殊地名设施[i][2];
										if (pos0 == building.pos)
										{
											//pk::trace(pk::format("pos 3,building_id:{}",building_id ));
											pk::set_district(building, pk::get_district(attacker.get_district_id()), /*占领*/0);
											ch::reset_spec(i);
											except0 = true;
											break;
										}
									}
								}
							}
							if (!except0) 城市势力_军事设施_待处理_列表.add(building);
						}
						else
						{
							//需要将特殊地名设施排除在列表外
							bool except0 = false;
							if (特定地点特定设施可争夺)
							{
								//pk::trace(pk::format("pos 1,building_id:{}",building_id ));
								if (地名设施.find(building.facility) >= 0)
								{
									//pk::trace(pk::format("pos 2,building_id:{}",building_id ));
									for (int i = 0; i < int(特殊地名设施.length); i++)
									{

										pk::point pos0;
										pos0.x = 特殊地名设施[i][1];
										pos0.y = 特殊地名设施[i][2];
										if (pos0 == building.pos)
										{
											//pk::trace(pk::format("pos 3,building_id:{}",building_id ));
											//pk::set_district(building, 1);
											except0 = true;
											break;
										}
									}
								}
							}
							if (!except0) 第三方势力_军事设施_待处理_列表.add(building);
						}
					}
				}

				func_占领城市后处理_敌军势力_城市占领时(city, attacker, building);
				func_占领城市后处理_其他设施(building);
			}

			func_占领城市后处理_内政设施(city, attacker, charisma, 内政设施_待处理_列表);
			func_占领城市后处理_军事设施(city, attacker, charisma, 城市势力_军事设施_待处理_列表, /*城市势力*/0);
			func_占领城市后处理_军事设施(city, attacker, charisma, 第三方势力_军事设施_待处理_列表, /*第三方势力*/1);
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

				/// 초기설정 : 일반세력 공격부대가 일반세력의 거점을 점령한 경우 기본적으로 거점의 자원(금, 병량, 병력, 병기)을 모두 흡수함 (설정 가능)
				// 일반세력 공격부대가 빈거점을 점령하는 경우는 해당사항 없음
				// 적장세력 공격부대가 일반세력의 거점을 점령하는 경우는 거점의 모든 자원이 손실됨
		void func_점령거점_후처리_거점자원(pk::building@ base, pk::unit@ attacker, int charisma)
		{
			int gold_ratio = 0, food_ratio = 0, troops_ratio = 0, wpn_amount_ratio = 0;

			if (资源吸收比例_模式[get_index(attacker)] == /*吸收比例 魅力反应*/1)
			{
				int charisma_ratio = 资源吸收比例_魅力反应比例[get_index(attacker)];
				int min_ratio = 资源吸收比例_最小吸收比例[get_index(attacker)];
				int max_ratio = 资源吸收比例_最大吸收比例[get_index(attacker)];
				int resource_ratio = pk::min(max_ratio, pk::max((charisma * charisma_ratio / 100), min_ratio));
				gold_ratio = food_ratio = troops_ratio = wpn_amount_ratio = resource_ratio;
			}
			else if (资源吸收比例_模式[get_index(attacker)] == /*吸收比例 直接设定*/2)
			{
				gold_ratio = 资源吸收比例_金[get_index(attacker)];
				food_ratio = 资源吸收比例_兵粮[get_index(attacker)];
				troops_ratio = 资源吸收比例_兵力[get_index(attacker)];
				wpn_amount_ratio = 资源吸收比例_兵器[get_index(attacker)];
			}

			pk::set_gold(base, (pk::get_gold(base) * gold_ratio / 100));
			pk::set_food(base, (pk::get_food(base) * food_ratio / 100));
			pk::set_troops(base, (pk::get_troops(base) * troops_ratio / 100));
			for (int i = 0; i < 병기_끝; i++)
				pk::set_weapon_amount(base, i, (pk::get_weapon_amount(base, i) * wpn_amount_ratio / 100));
		}

		/// 초기설정 : 기본적으로 内政设施은 도시를 점령한 공격부대의 세력으로 흡수됨 (파괴 가능, 타세력 흡수 설정 불가)
		// 건설 중이던 미완성 内政设施은 168 함수가 호출될 시점에 이미 파괴된 후임 (설정 불가)
		// 동작대 관련 : 파괴된 미완성 内政设施이 동작대인 경우 보물 동작은 자동 파괴됨, 완성된 동작대는 자동 흡수함
		// 세력의 최후도시를 점령하여 해당 세력을 멸망시키고 포획한 세력 군주를 처단한 경우 168 함수가 호출될 시점에 이미 모든 内政设施이 파괴된 후임 (설정 불가)
		// 적장세력 공격부대가 일반세력 도시를 점령한 경우 도시의 内政设施은 기본적으로 세력 없음으로 잔류 (파괴 가능)
		void func_占领城市后处理_内政设施(pk::city@ city, pk::unit@ attacker, int charisma, pk::list<pk::building@> facility_list)
		{
			if (!pk::is_normal_force(attacker.get_force_id())) return;

			int destroy_count = int(facility_list.count);
			if (destroy_count == 0) return;

			//安民
			if (ch::has_skill(attacker, 136)) return;


			if (内政设施_处理模式[get_index(attacker)] == /*吸收个数 魅力反应*/1)
			{
				int min_count = 内政设施_吸收个数_최소[get_index(attacker)];
				int max_count = 内政设施_吸收个数_최대[get_index(attacker)];
				int charisma_count = get_round_value(charisma * pk::min(内政设施_吸收个数_魅力反应比例[get_index(attacker)], 100) / 100.f);
				destroy_count = destroy_count - pk::max(min_count, pk::min(charisma_count, max_count));
			}
			else if (内政设施_处理模式[get_index(attacker)] == /*파괴比例 直接设定*/2)
			{
				destroy_count = get_round_value(destroy_count * pk::min(内政设施_破坏比例[get_index(attacker)], 100) / 100.f);
			}

			facility_list.shuffle();
			array<pk::building@> building_arr = pk::list_to_array(facility_list);
			for (int i = 0; i < destroy_count; i++)
			{
				pk::building@ building = building_arr[i];
				if (内政设施_处理模式[get_index(attacker)] == /*파괴확률 개별적용*/3)
				{
					if (!pk::rand_bool(内政设施별_破坏概率[get_index(attacker)])) continue;
				}
				pk::kill(building, false);
			}
		}

		/// 초기설정 : 기본적으로 군사시설은 기존의 세력으로 잔류 (미완성 시설 포함, 본거지 포함, 파괴 가능, 타세력 흡수 설정 가능)
		// 세력의 최후도시를 점령하여 멸망시킨 경우 도시구역 내 해당 세력의 모든 군사시설이 파괴됨 (타세력 흡수 설정 가능)
		// 세력의 최후도시를 점령하여 해당 세력을 멸망시키고 포획한 세력 군주를 처단한 경우 168 함수가 호출될 시점에 이미 그 세력의 모든 군사시설이 파괴된 후임 (설정 불가)
		// 적장세력 공격부대가 도시를 점령 시 본거지는 기본적으로 파괴됨 (적장세력을 포함한 타세력 흡수 설정 불가, 세력 없음으로 잔류 가능)
		void func_占领城市后处理_军事设施(pk::city@ city, pk::unit@ attacker, int charisma, pk::list<pk::building@> facility_list, int facility_index)
		{
			if (!pk::is_normal_force(attacker.get_force_id())) return;

			if (int(facility_list.count) == 0) return;

			int absorb_count = 0, destroy_count = 0;

			if (军事设施_处理模式[facility_index][get_index(attacker)] == /*处理概率 直接设定*/2)
			{
				absorb_count = get_round_value(int(facility_list.count) * pk::min(军事设施_吸收比例[facility_index][get_index(attacker)], 100) / 100.f);
				destroy_count = get_round_value(int(facility_list.count) * pk::min(军事设施_破坏比例[facility_index][get_index(attacker)], 100) / 100.f);
			}

			facility_list.shuffle();
			array<pk::building@> building_arr = pk::list_to_array(facility_list);
			for (int i = 0; i < int(building_arr.length); i++)
			{
				pk::building@ building = building_arr[i];
				if (军事设施_处理模式[facility_index][get_index(attacker)] == /*处理概率 反应距离*/1)
				{
					int distance = pk::get_distance(city.get_pos(), building.pos);
					int distance1 = 军事设施_标准距离[facility_index][get_index(attacker)][/*标准距离1*/0];
					int distance2 = 军事设施_标准距离[facility_index][get_index(attacker)][/*标准距离2*/1];
					int absorb_chance1 = 军事设施_按距离_吸收概率[facility_index][get_index(attacker)][/*标准距离1이하*/0];
					int absorb_chance2 = 军事设施_按距离_吸收概率[facility_index][get_index(attacker)][/*标准距离1초과2이하*/1];
					int absorb_chance3 = 军事设施_按距离_吸收概率[facility_index][get_index(attacker)][/*标准距离2초과*/2];
					int destroy_chance1 = 军事设施_按距离_破坏概率[facility_index][get_index(attacker)][/*标准距离1이하*/0];
					int destroy_chance2 = 军事设施_按距离_破坏概率[facility_index][get_index(attacker)][/*标准距离1초과2이하*/1];
					int destroy_chance3 = 军事设施_按距离_破坏概率[facility_index][get_index(attacker)][/*标准距离2초과*/2];
					int absorb_chance = distance <= distance2 ? (distance <= distance1 ? absorb_chance1 : absorb_chance2) : absorb_chance3;
					int destroy_chance = distance <= distance2 ? (distance <= distance1 ? destroy_chance1 : destroy_chance2) : destroy_chance3;
					if (pk::rand_bool(absorb_chance))  absorb_building(attacker, building);
					else if (pk::rand_bool(destroy_chance))	pk::kill(building, false);
				}
				else if (军事设施_处理模式[facility_index][get_index(attacker)] == /*处理概率 直接设定*/2)
				{
					if (i < absorb_count)                 absorb_building(attacker, building);
					else if (i < absorb_count + destroy_count) pk::kill(building, false);
				}
				else if (军事设施_处理模式[facility_index][get_index(attacker)] == /*处理概率 개별적용*/3)
				{
					if (pk::rand_bool(按军事设施_吸收概率[facility_index][get_index(attacker)])) absorb_building(attacker, building);
					else if (pk::rand_bool(按军事设施_破坏概率[facility_index][get_index(attacker)])) pk::kill(building, false);
				}
			}
		}

		void func_占领城市后处理_敌军势力_城市占领时(pk::city@ city, pk::unit@ attacker, pk::building@ building)
		{
			if (pk::is_normal_force(attacker.get_force_id())) return;

			if (pk::get_facility_type(building) == 设施类型_内政设施)
			{
				if (敌军势力_城市占领时_内政设施_破坏) pk::kill(building, false);
			}
			else if (pk::get_facility_type(building) == 设施类型_军事设施)
			{
				if (building.get_force_id() == city.get_force_id())
				{
					if (敌军势力_城市占领时_军事设施_破坏_城市势力) pk::kill(building, false);
				}
				else
				{
					if (敌军势力_城市占领时_军事设施_破坏_第三方势力) pk::kill(building, false);
				}
			}
		}

		/// 초기설정 : 기본적으로 함정 및 장애물은 모두 잔류 (미완성 시설 포함, 파괴 가능, 타세력 흡수 설정 불가)
		void func_占领城市后处理_其他设施(pk::building@ building)
		{
			if (pk::get_facility_type(building) == 设施类型_陷阱)
			{
				if (城市占领时_其他设施_破坏_陷阱) pk::kill(building, false);
			}
			else if (building.facility == 시설_토루 or building.facility == 시설_석벽)
			{
				if (城市占领时_其他设施_破坏_障碍物) pk::kill(building, false);
			}
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		array<int> 城市占领时_处理_除外设施 =
		{
			시설_도시, 시설_관문, 시설_항구,			// 고정시설
			시설_본거지1, 시설_본거지2,					// 군사시설
			시설_산악, 시설_장성, 시설_유적, 시설_묘,	// 장애물
			시설_낙석, 시설_제방, 시설_얕은여,			// 함정
			시설_동작대									// 内政设施
		};

		bool is_target_facility(pk::city@ city, pk::unit@ attacker, pk::building@ building)
		{
			if (pk::get_city_id(building.pos) != city.get_id()) return false;
			if (building.get_force_id() == attacker.get_force_id()) return false;
			if (城市占领时_处理_除外设施.find(building.facility) >= 0) return false;

			return true;
		}

		int get_index(pk::unit@ unit) { return (unit.is_player() ? /*플레이어*/0 : /*컴퓨터*/1); }

		/// round 연산자를 직접 사용하는 것은 불가능한 것으로 보임
		// 소수를 반올림한 정수값 반환용 함수
		int get_round_value(float float_value)
		{
			return (float_value - int(float_value) < (float_value >= 0 ? 0.5f : -0.5f)) ? int(float_value) : int(float_value) + 1;
		}

		void absorb_building(pk::unit@ unit, pk::building@ building)
		{
			pk::set_district(building, pk::get_district(unit.get_district_id()), /*점령*/0);
		}

		void func_디버깅(pk::unit@ attacker, pk::force@ attacker_force, pk::person@ attacker_leader, pk::building@ base, pk::force@ base_force)
		{
			if (!디버깅) return;

			string attacker_force_name = (pk::is_alive(attacker_force)) ? "\x1b[1x" + pk::decode(pk::get_name(attacker_force)) + "\x1b[0x" : "诠冼";
			string attacker_name = "\x1b[1x" + pk::decode(pk::get_name(attacker)) + "\x1b[0x";
			string attacker_leader_name = (pk::is_alive(attacker_leader)) ? "\x1b[1x" + pk::decode(pk::get_name(attacker_leader)) + "\x1b[0x" : "诠冼";
			string base_force_name = (pk::is_alive(base_force)) ? "\x1b[1x" + pk::decode(pk::get_name(base_force)) + "\x1b[0x" : "诠冼";
			string base_name = "\x1b[1x" + pk::decode(pk::get_name(base)) + "\x1b[0x";

			string title = "'168 普织 尽附\n(168 祸樽 樽讲 变点沙.cpp 烦腿)";
			string attacker_info = "皆芽 勺遇 :\n症肱(" + attacker_force_name + "), 勺遇(" + attacker_name + "), 茶择(" + attacker_leader_name + ")";
			string base_info = "樽讲扶 祸樽 :\n症肱(" + base_force_name + "), 祸樽(" + base_name + ")";

			pk::message_box(pk::encode(title));
			pk::message_box(pk::encode(attacker_info));
			pk::message_box(pk::encode(base_info));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	}

	Main main;
}
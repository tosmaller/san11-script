// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/11/15 # 江东新风 # 修复能力公式bug ##
// ## 2021/11/06 # 江东新风 # 移动力bug修复 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2020/12/20 # 江东新风 # 添加父母子女关系的血缘加成 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/27 # 氕氘氚 # 删除势力加成效果 ##
// ## 2020/10/04 # 氕氘氚 # 修复宝物特技强行不生效的bug ##
// ## 2020/10/02 # 氕氘氚 # 修复步兵移动力大于骑兵的bug ##
// ## 2020/10/01 #江东新风#降低移动力浮动范围##
// ## 2020/09/15 #江东新风#修复部队移动力显示异常的bug##
// ## 2020/08/14 #江东新风#将光环系统的基础光环放入此处##
// ## 2020/08/01 ##
/*
@ 수정자: 기마책사
@ Update: '18.10.21  / 变更内容: 武力统率修正变更，移动力_兵力影响设定，移动力_适性影响设定，移动力_统率影响设定，移动力_气力影响设定，移动力_季节影响设定，势力效果设定
@ Update: '18.11.6   / 变更内容: 玩家自定义选項添加
@ Update: '18.12.1   / 变更内容: 运输部队移动力調整，随兵力变化，不含适性
@ Update: '18.12.27  / 变更内容: 添加领土设定功能
*/

namespace UNIT_ATTR
{

	//---------------------------------------------------------------------------------------
	// 유저 설정 (true = on, false = off)

	const bool 部队攻防武力统率影响修正 = true;   //武力统率修正变更 true: 부장능력 포함하는 신규 수식 적용, false: 기존 방식 적용

	const bool 移动力_兵力影响 = true;   // 병력수에 따라 이동력 증감
	const bool 移动力_适性影响 = true;   // 적성에 따라 이동력 증감
	const bool 移动力_统率影响 = true;   // 통솔력에 따라 이동력 증감
	const bool 移动力_气力影响 = true;   // 기력에 따라 이동력 증감
	const bool 移动力_季节影响 = true;   // 계절에 따라 이동력 증감
	const bool 移动力_领土影响 = true;   // 계절에 따라 이동력 증감

	const int 适性等级修正比率 = 10;
	const int 精锐加攻 = 15;
	const int 精锐加防 = 15;
	const int 剑兵攻防倍率 = 60;
	const int 运输队攻倍率 = 40;
	const int 运输队防倍率 = 33;
	const int 混乱攻防倍率 = 80;
	const int 建设政治占比 = 70;
	const int 建设修正参数 = 50;
	const int 攻武占比 = 50;
	const int 攻统占比 = 50;
	const int 防武占比 = 20;
	const int 防统占比 = 80;

	const int 精锐步兵加移 = 6;
	const int 良马产出加移 = 4;
	const int 精锐骑兵加移 = 2;
	const int 車轴强化加移 = 4;
	const int 木牛流马加移 = 4;
	const int 运输队加移 = 5;

	const int 仲介修正比率 = 15;
	const int 血缘修正比率 = 15;
	const int 亲爱修正比率 = 10;
	const int 正常修正比率 = 5;

	//---------------------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(163, pk::func163_t(callback));
		}

		void callback(pk::unit_attr& attr, const pk::detail::arrayptr<pk::person@>& in member, int weapon_id, uint troops, int type, int status, bool navy, bool shoubyou)
		{

			pk::person@ leader = member[0];


			if (!pk::is_alive(leader))
				return;


			pk::force@ force = pk::get_force(leader.get_force_id());

			// 주장이 소속된 부대 또는 건물 반환主将所属部队或建筑物归还 ('18.10.10)
			int unit_energy = 0;
			pk::building@ building = pk::hex_object_to_building(pk::get_hex_object(leader.location));
			pk::unit@ unit;
			if (!pk::is_alive(building))    // 주장이 건물에 소속되어 있지 않으면 부대 기력값 사용
			{
				@unit = pk::get_unit(pk::get_unit_id(leader));
				unit_energy = unit.energy;
			}
			else                            //主将如果所属在某一建筑，则出征时使用建筑物的气力 주장이 건물 소속되어 출진 부대 편성 중이면 거점 기력값 사용
			{
				unit_energy = pk::get_energy(building);
			}



			if (!pk::is_alive(force))
				return;
			if (!pk::is_valid_equipment_id(weapon_id) or !pk::is_valid_unit_type(type) or !pk::is_valid_unit_status(status))
				return;



			// 부장이 없다면	如果沒有副将
			if ((member[1] is null) and (member[2] is null))
			{
				for (int i = 0; i < 武将能力_末; i++)	// 只发揮其主将能力주장 능력치 그대로 사용
					attr.stat[i] = shoubyou ? leader.stat[i] : leader.max_stat[i];
			}
			// 부장이 있다면
			else
			{
				// 如果有厌恶关系则不进行修正 혐오 관계가 있다면 보정하지 않음
				if (func_4960d0(leader, member[1]) or func_4960d0(leader, member[2]) or func_4960d0(member[1], member[2]))
				{
					for (int i = 0; i < 武将能力_末; i++)
						attr.stat[i] = shoubyou ? leader.stat[i] : leader.max_stat[i];
				}
				// 无厌恶关系时进行能力修正혐오 관계가 없다면 보정
				else
				{
					for (int i = 0; i < 武将能力_末; i++)
					{
						int a = 0, b = 0;
						int leader_stat = shoubyou ? leader.stat[i] : leader.max_stat[i];

						// 统率武力按武将关系修正 통솔, 무력은 무장 관계에 따라 보정
						if (i <= 武将能力_武力)
						{

							if (部队攻防武力统率影响修正)
							{
								// 新的修飾方式：主将+副将1修正+副将2修正신규 보정 수식 : 주장 + 부장1보조 + 부장2보조   ('18.10.9)
								if (member[1] !is null)     // 新函数：副将1修正量 신규 함수 : 부장1 보조량
									a = func_sub_stat(leader, leader_stat, member[1], (shoubyou ? member[1].stat[i] : member[1].max_stat[i]));
								if (member[2] !is null)     // 新函数：副将2修正量 신규 함수 : 부장2 보조량
									b = func_sub_stat(leader, leader_stat, member[2], (shoubyou ? member[2].stat[i] : member[2].max_stat[i]));
								//下面公式本来ab直接相加而不是取最大，导致主将越弱，差值越大，能力越大，不合理，改了
								attr.stat[i] = pk::min(255, (leader_stat + pk::max(a, b)));   // 超過255以255计算。255 이하로 합산
							}

							else
							{
								// 现有的修正方式：MAX(主将&副将2)(主将&副将2)기존 보정 방법 : MAX[(주장&부장1),(주장&부장2)]
								if (member[1] !is null)
									a = func_495ff0(leader, leader_stat, member[1], shoubyou ? member[1].stat[i] : member[1].max_stat[i]);
								if (member[2] !is null)
									b = func_495ff0(leader, leader_stat, member[2], shoubyou ? member[2].stat[i] : member[2].max_stat[i]);
								attr.stat[i] = pk::max(a, b);   // 최대값
							}

						}
						// 智力政治魅力最大zhi 지력, 정치, 매력은 최대값
						else
						{
							if (member[1] !is null)
								a = pk::max(leader_stat, shoubyou ? member[1].stat[i] : member[1].max_stat[i]);
							if (member[2] !is null)
								b = pk::max(leader_stat, shoubyou ? member[2].stat[i] : member[2].max_stat[i]);

							attr.stat[i] = pk::max(a, b);   // 최대값
						}

					}
				}
			}


			// 适性最大值적성은 최대값
			for (int i = 0; i < 병종_끝; i++)
			{
				int n = 0;
				for (int j = 0; j < member.length; j++)
				{
					if (member[j] !is null)
						n = pk::max(n, member[j].tekisei[i]);
				}
				attr.tekisei[i] = n;
			}

			pk::equipment@ equipment = pk::get_equipment(weapon_id);

			float apt = float(pk::core["tekisei"][attr.tekisei[pk::equipment_id_to_heishu(weapon_id)]]["unit_stat"]); // (attr.tekisei[pk::equipment_id_to_heishu(weapon_id)] + 7) * 0.1;
			//int 适性 = pk::get_tekisei(unit, weapon_id);
			//apt = 1.f + 适性等级修正比率 * (适性 - 3) / 100.f;  //新的适性修正??
			float sts = 1.f;	//攻防建百分比
			float str = 1.f;	//攻百分比
			float ldr = 1.f;	//防建百分比
			int atk = equipment.stat[兵器能力_攻击];
			int def = equipment.stat[兵器能力_防御];
			int mov = equipment.stat[兵器能力_移动];


			// int defskill_불굴 = 특기_불굴;  //不屈
			// int defskill_금강 = 특기_금강;  //金刚
			// int defskill_철벽 = 특기_철벽;  //铁壁

			if (weapon_id == 병기_검 or (weapon_id == 兵器_走舸 and type == 부대종류_수송))	//剑，走?，运输
				apt = 剑兵攻防倍率 / 100.f;

			if (status == 部队状态_混乱)//混乱
				sts = 混乱攻防倍率 / 100.f;

			if (type == 부대종류_수송)//运输
			{
				str = 运输队攻倍率 / 100.f;
				ldr = 运输队防倍率 / 100.f;
			}

			if (weapon_id <= 병기_군마)
			{
				int tech_id = -1;

				if (weapon_id == 병기_창)//枪
				{
					tech_id = 기교_정예창병;//精锐枪兵
				}
				if (weapon_id == 병기_극)//戟
				{
					tech_id = 기교_정예극병;//精锐戟兵
				}
				if (weapon_id == 병기_노)//弩
				{
					tech_id = 기교_정예노병;//精锐弩兵
				}
				if (weapon_id == 병기_군마)//战马
				{
					tech_id = 기교_정예기병;//精锐骑兵
				}

				if (pk::has_tech(force, tech_id))
				{
					atk = atk + 精锐加攻;
					def = def + 精锐加防;
				}

			}

			// ------------------------------- //
			// 공격/방어력 추가 효과 적용 시 작성할 위치使用攻击防御追加效果时的位置 //
			// ------------------------------- //


			//ch::u8debug("开始计算能力");
			attr.stat[部队能力_攻击] = int(pk::min(255.f, pk::max(1.f, ((攻武占比 / 100.f * attr.stat[部队能力_武力] + 攻统占比 / 100.f * attr.stat[部队能力_统率]) / 1.0f * atk * apt * 0.01f) * str * sts)));
			attr.stat[部队能力_防御] = int(pk::min(255.f, pk::max(1.f, ((防武占比 / 100.f * attr.stat[部队能力_武力] + 防统占比 / 100.f * attr.stat[部队能力_统率]) / 1.0f * def * apt * 0.01f) * ldr * sts)));
			attr.stat[部队能力_智力] = int((attr.stat[部队能力_智力]) / 1.f);
			attr.stat[部队能力_建设] = int(pk::min(255.f, pk::max(1.f, (attr.stat[部队能力_政治] * 建设政治占比 / 100.f + 建设修正参数) * ldr * sts)));

			//光环系统设定-基础光环
			if (基础光环)
			{
				if (leader.location < 1086 and leader.location > 86)
				{
					//ch::u8debug("开始计算光环");
					pk::unit@ src = pk::get_unit(pk::get_unit_id(leader));
					//pk::force@ force = pk::get_force(leader.get_force_id());
					int dst_id = halo::func_get_highest_halo(src, 0);
					/*
					if (src.get_id() == 0 and dst_id == -1 and halo::func_光环范围(src,0) != -1)
					{
						//ch::u8debug("the first unit with halo");
						dst_id = 0;
					}*/
					if (dst_id != -1)
					{
						pk::unit@ dst = pk::get_unit(dst_id);
						int effect = halo::func_光环效果(dst, 0);
						attr.stat[部队能力_攻击] = pk::min(255, (attr.stat[部队能力_攻击] + effect));
						attr.stat[部队能力_防御] = pk::min(255, (attr.stat[部队能力_防御] + effect));
					}

				}
			}

			//--------------------------------------------------------------------------------
			// 부대 조건에 따른 이동력 가변 효과特殊条件下移动力可变的效果

			// 부대별 기력, 통솔력, 적성, 병력수 효과 이동력 계산용 변수 추가 ('18.10.7)
			float mov_var = mov;

			// 병력수에 따른 이동력 보정 : 기본 이동력 +1, 3000명 증가 시마다 이동력 감소 ('18.10.9)
			if (移动力_兵力影响)
			{
				if (type == 부대종류_수송)
					mov_var = mov_var + 1 - pk::min(5, (troops / 6000));    // 수송부대 이동력 감소 리미터 ('18.11.24)
				else
					mov_var = mov_var + 1 - pk::min(10, (troops / 3000));    // 전투부대 이동력 감소 리미터
			}

			// 병과 적성에 따른 이동력 보정 S급=+10%, C급=-20%('18.10.8)
			if (移动力_适性影响)
			{
				if (type == 부대종류_전투)    // 전투부대에 한하여 적성 고려 추가 ('18.11.26)
					mov_var = mov_var * (1.f + 0.05f * pk::min(2, (attr.tekisei[pk::equipment_id_to_heishu(weapon_id)] - 적성_A)));
			}

			// 부대별 통솔력에 따른 이동력 변동 85% ~ 105% 변동 : 통솔력 75일 때 ±0% ('18.10.7)
			if (移动力_统率影响)
				mov_var = mov_var * (0.95f + 0.10f * (leader.stat[武将能力_统率] / 100.f));

			// 부대별 기력에 따른 이동력 80% ~ 110% 변동 : 기력 80일 때 100% ('18.10.10)
			if (移动力_气力影响)
				mov_var = mov_var * (0.90 + 0.15f * (unit_energy / 120.f));

			// 계절에 따른 이동력 증감 효과 ('18.10.9)
			if (移动力_季节影响 and !navy)//改成只影响陆军好了，否则对水军影响过大
			{
				switch (pk::get_season())            // pk::get_season()= 0:봄, 1:여름, 2:가을, 3:겨울
				{
				case 季节_夏: mov_var -= 2; break;    // 여름에 이동력 -2
				case 季节_冬: mov_var -= 4; break;    // 겨율에 이동력 -4
				}
			}

			// 부대의 좌표 영토에 따른 이동력 효과 : 아군영토에서 이동력 상승, 적군영토에서 이동력 감소 ('18.12.27)
			if (移动力_领土影响 and pk::is_alive(unit))
			{
				pk::building@ area_building = pk::get_building(pk::get_building_id(unit.pos));
				if (pk::is_alive(area_building))
				{
					if (unit.get_force_id() == area_building.get_force_id())
						mov_var = mov_var * 1.05f;   // 아군영토에서 이동력 10%증가 
					else if (pk::is_enemy(unit, area_building))
						mov_var = mov_var * 0.95f;   // 적군영토에서 이동력 10% 감소 
				}
			}

			// 부대별 최종 가변 이동력各部队最终可变移动力
			
			mov = int(mov_var);

			//--------------------------------------------------------------------------------
			// 기존 스크립트 수정 : 기교 및 세력/장수별 특징 반영

			if (type == 부대종류_전투)
			{
				switch (weapon_id)
				{
				case 병기_창:
					if (pk::has_tech(force, 기교_정예창병))
						mov = mov + 精锐步兵加移;
					break;

				case 병기_극:
					if (pk::has_tech(force, 기교_정예극병))
						mov = mov + 精锐步兵加移;
					if (ch::has_skill(unit, 特技_重甲))
						mov = mov - 重甲移动力减少;
					break;

				case 병기_노:
					if (pk::has_tech(force, 기교_정예노병))
						mov = mov + 精锐步兵加移;
					break;

				case 병기_군마:
					if (pk::has_tech(force, 기교_양마산출))
						mov = mov + 良马产出加移;
					if (pk::has_tech(force, 기교_정예기병))   // 정예기병 이동력 하향 ('18.10.5)
						mov = mov + 精锐骑兵加移;
					break;

				case 병기_충차:
					if (pk::has_tech(force, 기교_차축강화))   // 차축강화 이동력 상향 ('18.10.5)
						mov = mov + 車轴强化加移;
					break;

				case 병기_정란:
					if (pk::has_tech(force, 기교_차축강화))    // 차축강화 이동력 상향 ('18.10.5)
						mov = mov + 車轴强化加移;
					break;

				case 병기_투석:
					if (pk::has_tech(force, 기교_차축강화))    // 차축강화 이동력 상향 ('18.10.5)
						mov = mov + 車轴强化加移;
					break;

				case 병기_목수:
					if (pk::has_tech(force, 기교_차축강화))
						mov = mov + 車轴强化加移;
					break;
				}


				if (weapon_id <= 병기_노)
				{
					/* 宝物的强行用has_skill无法判断，得用pk::core::skill_constant才行
					有强行时返回5， 没有强行时返回0
					*/
					//pk::trace(pk::format("leader:{},mov-before:{}.inc:{}", member[0].get_id(),mov, int(pk::core::skill_constant(member, 특기_강행))));
					mov = mov + (ch::has_skill(member, 特技_强行) ? int(pk::core::skill_constant(member, 特技_强行)):0); // 5

				}
				else if (weapon_id == 병기_군마)
				{
					int a_强行 = ch::has_skill(member, 特技_强行) ? int(pk::core::skill_constant(member, 特技_强行)):0; // 5
					int a_长驱 = ch::has_skill(member, 特技_长驱) ? int(pk::core::skill_constant(member, 特技_长驱)):0; // 3
					mov = mov + pk::max(a_长驱, a_强行);
				}

			}
			else if (type == 부대종류_수송)   // 수송부대 输送部队
			{
				if (pk::has_tech(force, 기교_목우유마))
					mov = mov + 木牛流马加移;

				mov = mov + 运输队加移;
				if (ch::has_skill(member, 특기_운반))
					mov = mov + int(pk::core::skill_constant(member, 특기_운반)); // 5
			}

			if (navy)   //수상 부대인 경우
			{
				if (ch::has_skill(member, 특기_조타))
					mov = mov + int(pk::core::skill_constant(member, 특기_조타)); // 4
			}

			if (unit !is null) { if (unit.status == 部队状态_恐惧) mov_var /= 2.f; }

			attr.stat[部队能力_移动] = mov;

			// 自定义修正函数
			func_部队能力设定_自定义(attr, member, weapon_id, troops, type, status, navy, shoubyou);
		}

		/**
			관계에 따른 능력 보정  -- 기존 수식 : 주장과 부장1, 주장과 부장2 의 각 보정 결과 중 최대값 -- 사용안함
		*/
		int func_495ff0(pk::person@ leader, int leader_stat, pk::person@ deputy, int deputy_stat)
		{
			if (leader_stat > deputy_stat)
				return leader_stat;

			int leader_id = leader.get_id();
			int deputy_id = deputy.get_id();

			if (pk::is_gikyoudai(leader, deputy_id) or pk::is_fuufu(leader, deputy_id))
				return deputy_stat;

			if (pk::is_like(leader, deputy_id) or pk::is_like(deputy, leader_id))
				return leader_stat + (deputy_stat - leader_stat) / 2;

			if (pk::is_ketsuen(leader, deputy_id))
				return leader_stat + (deputy_stat - leader_stat) / 3;

			if (pk::is_dislike(leader, deputy_id) or pk::is_dislike(deputy, leader_id))
				return leader_stat;

			return leader_stat + (deputy_stat - leader_stat) / 4;
		}

		/**
			서로 상대방을 혐오하는 관계가 있는지 확인
		*/
		bool func_4960d0(pk::person@ a, pk::person@ b)
		{
			if (a !is null and b !is null)
				return pk::is_dislike(a, b.get_id()) or pk::is_dislike(b, a.get_id());
			return false;
		}

		// --------------------------------------------------------------------------------------
		//**  관계에 따른 능력 보정  -- 신규 수식 : 각 부장의 능력 보조량만 계산 후 주장+부장1+부장2 최종 합산 **/
		int func_sub_stat(pk::person@ leader, int leader_stat, pk::person@ deputy, int deputy_stat)
		{

			int leader_id = leader.get_id();
			int deputy_id = deputy.get_id();

			int stat_add = 0;

			// 혐오 관계 무장인 경우 : 보조 없음  厌恶
			if (pk::is_dislike(leader, deputy_id) or pk::is_dislike(deputy, leader_id))
				stat_add = 0;
			// 의형제 또는 부부 관계인 경우 : 부장 능력의 30% 보조  仲介
			else if (pk::is_gikyoudai(leader, deputy_id) or pk::is_fuufu(leader, deputy_id))
				stat_add = int(pk::max(仲介修正比率 / 100.f * deputy_stat, 1.f * (deputy_stat - leader_stat)));
			// 혈연 관계인 경우 : 부장 능력의 30% 보조  血缘，添加父母子女关系
			else if (pk::is_ketsuen(leader, deputy_id) or pk::is_oyako(leader, deputy_id))
				stat_add = int(血缘修正比率 / 100.f * deputy_stat);

			// 주장/부장이 친애 무장인 경우 : 부장 능력의 20% 보조  亲爱
			else if (pk::is_like(leader, deputy_id) or pk::is_like(deputy, leader_id))
				stat_add = int(亲爱修正比率 / 100.f * deputy_stat);
			// 기타 관계 : 부장 능력의 10% 보조  其他
			else
				stat_add = int(正常修正比率 / 100.f * deputy_stat);

			return stat_add;
		}
		// --------------------------------------------------------------------------------------


		/**自定义扩展设定
		 * 自定义设定请写在此函数中，不建议直接修改上面的callback。
		 * 根据需要添加内容，如绑定势力、武将的特殊加成效果等等。
		*/
		void func_部队能力设定_自定义(pk::unit_attr& attr, const pk::detail::arrayptr<pk::person@>& in member, int weapon_id, uint troops, int type, int status, bool navy, bool shoubyou)
		{
			return;
		}

	}

	Main main;

	void cal_rough_stat(pk::unit_attr& attr, array<pk::person@> member, int weapon_id, uint troops, int type, int status, bool navy, bool shoubyou)
	{
		pk::person@ leader = member[0];


		if (!pk::is_alive(leader))
			return;


		pk::force@ force = pk::get_force(leader.get_force_id());

		// 주장이 소속된 부대 또는 건물 반환主将所属部队或建筑物归还 ('18.10.10)
		int unit_energy = 0;
		pk::building@ building = pk::hex_object_to_building(pk::get_hex_object(leader.location));
		pk::unit@ unit;
		if (!pk::is_alive(building))    // 주장이 건물에 소속되어 있지 않으면 부대 기력값 사용
		{
			@unit = pk::get_unit(pk::get_unit_id(leader));
			unit_energy = unit.energy;
		}
		else                            //主将如果所属在某一建筑，则出征时使用建筑物的气力 주장이 건물 소속되어 출진 부대 편성 중이면 거점 기력값 사용
		{
			unit_energy = pk::get_energy(building);
		}



		if (!pk::is_alive(force))
			return;
		if (!pk::is_valid_equipment_id(weapon_id) or !pk::is_valid_unit_type(type) or !pk::is_valid_unit_status(status))
			return;



		// 부장이 없다면	如果沒有副将
		if ((member[1] is null) and (member[2] is null))
		{
			for (int i = 0; i < 武将能力_末; i++)	// 只发揮其主将能力주장 능력치 그대로 사용
				attr.stat[i] = shoubyou ? leader.stat[i] : leader.max_stat[i];
		}
		// 부장이 있다면
		else
		{
			// 如果有厌恶关系则不进行修正 혐오 관계가 있다면 보정하지 않음
			if (main.func_4960d0(leader, member[1]) or main.func_4960d0(leader, member[2]) or main.func_4960d0(member[1], member[2]))
			{
				for (int i = 0; i < 武将能力_末; i++)
					attr.stat[i] = shoubyou ? leader.stat[i] : leader.max_stat[i];
			}
			// 无厌恶关系时进行能力修正혐오 관계가 없다면 보정
			else
			{
				for (int i = 0; i < 武将能力_末; i++)
				{
					int a = 0, b = 0;
					int leader_stat = shoubyou ? leader.stat[i] : leader.max_stat[i];

					// 统率武力按武将关系修正 통솔, 무력은 무장 관계에 따라 보정
					if (i <= 武将能力_武力)
					{

						if (部队攻防武力统率影响修正)
						{
							// 新的修飾方式：主将+副将1修正+副将2修正신규 보정 수식 : 주장 + 부장1보조 + 부장2보조   ('18.10.9)
							if (member[1] !is null)     // 新函数：副将1修正量 신규 함수 : 부장1 보조량
								a = main.func_sub_stat(leader, leader_stat, member[1], (shoubyou ? member[1].stat[i] : member[1].max_stat[i]));
							if (member[2] !is null)     // 新函数：副将2修正量 신규 함수 : 부장2 보조량
								b = main.func_sub_stat(leader, leader_stat, member[2], (shoubyou ? member[2].stat[i] : member[2].max_stat[i]));
							//下面公式本来ab直接相加而不是取最大，导致主将越弱，差值越大，能力越大，不合理，改了
							attr.stat[i] = pk::min(255, (leader_stat + pk::max(a, b)));   // 超過255以255计算。255 이하로 합산
						}

						else
						{
							// 现有的修正方式：MAX(主将&副将2)(主将&副将2)기존 보정 방법 : MAX[(주장&부장1),(주장&부장2)]
							if (member[1] !is null)
								a = main.func_495ff0(leader, leader_stat, member[1], shoubyou ? member[1].stat[i] : member[1].max_stat[i]);
							if (member[2] !is null)
								b = main.func_495ff0(leader, leader_stat, member[2], shoubyou ? member[2].stat[i] : member[2].max_stat[i]);
							attr.stat[i] = pk::max(a, b);   // 최대값
						}

					}
					// 智力政治魅力最大zhi 지력, 정치, 매력은 최대값
					else
					{
						if (member[1] !is null)
							a = pk::max(leader_stat, shoubyou ? member[1].stat[i] : member[1].max_stat[i]);
						if (member[2] !is null)
							b = pk::max(leader_stat, shoubyou ? member[2].stat[i] : member[2].max_stat[i]);

						attr.stat[i] = pk::max(a, b);   // 최대값
					}

				}
			}
		}


		// 适性最大值적성은 최대값
		for (int i = 0; i < 병종_끝; i++)
		{
			int n = 0;
			for (int j = 0; j < int(member.length); j++)
			{
				if (member[j] !is null)
					n = pk::max(n, member[j].tekisei[i]);
			}
			attr.tekisei[i] = n;
		}

		pk::equipment@ equipment = pk::get_equipment(weapon_id);

		float apt = float(pk::core["tekisei"][attr.tekisei[pk::equipment_id_to_heishu(weapon_id)]]["unit_stat"]); // (attr.tekisei[pk::equipment_id_to_heishu(weapon_id)] + 7) * 0.1;
		//int 适性 = pk::get_tekisei(unit, weapon_id);
		//apt = 1.f + 适性等级修正比率 * (适性 - 3) / 100.f;  //新的适性修正??
		float sts = 1.f;	//攻防建百分比
		float str = 1.f;	//攻百分比
		float ldr = 1.f;	//防建百分比
		int atk = equipment.stat[兵器能力_攻击];
		int def = equipment.stat[兵器能力_防御];
		int mov = equipment.stat[兵器能力_移动];


		// int defskill_불굴 = 특기_불굴;  //不屈
		// int defskill_금강 = 특기_금강;  //金刚
		// int defskill_철벽 = 특기_철벽;  //铁壁

		if (weapon_id == 병기_검 or (weapon_id == 兵器_走舸 and type == 부대종류_수송))	//剑，走?，运输
			apt = 剑兵攻防倍率 / 100.f;

		if (status == 部队状态_混乱)//混乱
			sts = 混乱攻防倍率 / 100.f;

		if (type == 부대종류_수송)//运输
		{
			str = 运输队攻倍率 / 100.f;
			ldr = 运输队防倍率 / 100.f;
		}

		if (weapon_id <= 병기_군마)
		{
			int tech_id = -1;

			if (weapon_id == 병기_창)//枪
			{
				tech_id = 기교_정예창병;//精锐枪兵
			}
			if (weapon_id == 병기_극)//戟
			{
				tech_id = 기교_정예극병;//精锐戟兵
			}
			if (weapon_id == 병기_노)//弩
			{
				tech_id = 기교_정예노병;//精锐弩兵
			}
			if (weapon_id == 병기_군마)//战马
			{
				tech_id = 기교_정예기병;//精锐骑兵
			}

			if (pk::has_tech(force, tech_id))
			{
				atk = atk + 精锐加攻;
				def = def + 精锐加防;
			}

		}

		// ------------------------------- //
		// 공격/방어력 추가 효과 적용 시 작성할 위치使用攻击防御追加效果时的位置 //
		// ------------------------------- //


		//ch::u8debug("开始计算能力");
		attr.stat[部队能力_攻击] = int(pk::min(255.f, pk::max(1.f, ((攻武占比 / 100.f * attr.stat[部队能力_武力] + 攻统占比 / 100.f * attr.stat[部队能力_统率]) / 1.0f * atk * apt * 0.01f) * str * sts)));
		attr.stat[部队能力_防御] = int(pk::min(255.f, pk::max(1.f, ((防武占比 / 100.f * attr.stat[部队能力_武力] + 防统占比 / 100.f * attr.stat[部队能力_统率]) / 1.0f * def * apt * 0.01f) * ldr * sts)));
		attr.stat[部队能力_智力] = int((attr.stat[部队能力_智力]) / 1.f);
		attr.stat[部队能力_建设] = int(pk::min(255.f, pk::max(1.f, (attr.stat[部队能力_政治] * 建设政治占比 / 100.f + 建设修正参数) * ldr * sts)));

		//光环系统设定-基础光环
		if (基础光环)
		{
			if (leader.location < 1086 and leader.location > 86)
			{
				//ch::u8debug("开始计算光环");
				pk::unit@ src = pk::get_unit(pk::get_unit_id(leader));
				//pk::force@ force = pk::get_force(leader.get_force_id());
				int dst_id = halo::func_get_highest_halo(src, 0);
				/*
				if (src.get_id() == 0 and dst_id == -1 and halo::func_光环范围(src,0) != -1)
				{
					//ch::u8debug("the first unit with halo");
					dst_id = 0;
				}*/
				if (dst_id != -1)
				{
					pk::unit@ dst = pk::get_unit(dst_id);
					int effect = halo::func_光环效果(dst, 0);
					attr.stat[部队能力_攻击] = pk::min(255, (attr.stat[部队能力_攻击] + effect));
					attr.stat[部队能力_防御] = pk::min(255, (attr.stat[部队能力_防御] + effect));
				}

			}
		}

	}

}


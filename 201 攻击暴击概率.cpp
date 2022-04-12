// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2021/02/15 # 江东新风 # 战神自带战将效果无需组合 ##
// ## 2020/12/22 # 江东新风 # 山战背水特技 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/08/29 # 江东新风 # 战将bug修复 ##
// ## 2020/08/23 # 江东新风 # 遊侠bug修复 ##
// ## 2020/08/19 # 江东新风 # 原版特技has_skill设定 ##
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/08/14 #江东新风#将光环系统的战法暴击光环放入此处，新特技步将骁将布阵战将遊侠##
// ## 2020/07/26 ##
namespace ATTACK_CRITICAL_CHANCE
{
	//---------------------------------------------------------------------------
	const int 基础暴击率 = 0;
	const int 武力暴击需求 = 80;
	const int 武力暴击下限 = 2;
	const int 武力暴击上限 = 5;
	const float 武力暴击系数 = 0.2;
	const int 适性增加暴击 = 1;
	const int 关系暴击_仲介 = 4;
	const int 关系暴击_亲爱 = 2;
	const int 关系暴击_厌恶 = 5;
	//---------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(201, pk::func201_t(callback));
		}

		bool callback(pk::unit@ attacker, pk::hex_object@ target, int tactics_id, bool ranged)
		{
			pk::unit@ target_unit = pk::hex_object_to_unit(target);
			int target_strength = int(pk::core["person.max_stat"]) + 1; // 무력 변수 
			int target_lead = int(pk::core["person.max_stat"]) + 1; // 통솔 변수 

			if (target_unit !is null)
			{
				//布阵不被暴击判断
				if (ch::has_skill(target_unit, 特技_布阵))
				{
					return false;
				}
				target_strength = ch::get_best_member_stat(target_unit, 武将能力_武力);
				target_lead = ch::get_best_member_stat(target_unit, 武将能力_统率);
			}

			// 전법
			if (pk::is_valid_tactics_id(tactics_id))
			{
				if (ch::has_skill(attacker, 特技_霸王)) return true;
				if (ch::get_best_member_stat(attacker, 特技_勇将, 武将能力_武力) > target_strength) return true;
				if (ch::get_best_member_stat(attacker, 特技_勇将, 武将能力_统率) > target_lead) return true;

				//战将
				if (target_unit !is null and (ch::has_skill(attacker, 特技_战将) or ch::has_skill(attacker, 特技_战神)))
				{
					if (attacker.attr.stat[部队能力_攻击] > target_unit.attr.stat[部队能力_防御])
					{
<<<<<<< HEAD
						pk::trace(pk::format("战将判断，攻击：{}，防御：{}", attacker.attr.stat[部队能力_攻击], target_unit.attr.stat[部队能力_防御]));
=======
						//pk::trace(pk::format("战将判断，攻击：{}，防御：{}", attacker.attr.stat[部队能力_攻击], target_unit.attr.stat[部队能力_防御]));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
						return true;
					}
						
				}


				//遊侠
				if (ch::has_skill(attacker, 特技_游侠) and target_unit !is null)
				{
					if ((attacker.troops * 2) < target_unit.troops)
						return true;
				}

				switch (pk::get_weapon_id(attacker))
				{
<<<<<<< HEAD
				case 병기_창:
=======
				case 兵器_枪:
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					if (ch::has_skill(attacker, 특기_투신)) return true;
					if (ch::has_skill(attacker, 특기_창신)) return true;
					if (ch::get_best_member_stat(attacker, 特技_枪将, 武将能力_武力) > target_strength) return true;
					if (ch::get_best_member_stat(attacker, 特技_枪将, 武将能力_统率) > target_lead) return true;
					if (ch::get_best_member_stat(attacker, 特技_神将, 武将能力_武力) > target_strength) return true;
					if (ch::get_best_member_stat(attacker, 特技_神将, 武将能力_统率) > target_lead) return true;
					if (ch::get_best_member_stat(attacker, 特技_飞将, 武将能力_武力) > target_strength) return true;
					if (ch::get_best_member_stat(attacker, 特技_飞将, 武将能力_统率) > target_lead) return true;
					//步将
					if (ch::get_best_member_stat(attacker, 151, 部队能力_武力) > target_strength) return true;

					break;

<<<<<<< HEAD
				case 병기_극:
=======
				case 兵器_戟:
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					if (ch::has_skill(attacker, 특기_투신)) return true;
					if (ch::has_skill(attacker, 특기_극신)) return true;
					if (ch::get_best_member_stat(attacker, 특기_극장, 武将能力_武力) > target_strength) return true;
					if (ch::get_best_member_stat(attacker, 특기_극장, 武将能力_统率) > target_lead) return true;
					if (ch::get_best_member_stat(attacker, 特技_神将, 武将能力_武力) > target_strength) return true;
					if (ch::get_best_member_stat(attacker, 特技_神将, 武将能力_统率) > target_lead) return true;
					if (ch::get_best_member_stat(attacker, 特技_飞将, 武将能力_武力) > target_strength) return true;
					if (ch::get_best_member_stat(attacker, 特技_飞将, 武将能力_统率) > target_lead) return true;
					//步将
					if (ch::get_best_member_stat(attacker, 151, 部队能力_武力) > target_strength) return true;
					break;

<<<<<<< HEAD
				case 병기_노:
=======
				case 兵器_弩:
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					if (ch::has_skill(attacker, 특기_궁신)) return true;
					if (ch::get_best_member_stat(attacker, 특기_궁장, 武将能力_武力) > target_strength) return true;
					if (ch::get_best_member_stat(attacker, 특기_궁장, 武将能力_统率) > target_lead) return true;
					if (ch::get_best_member_stat(attacker, 特技_神将, 武将能力_武力) > target_strength) return true;
					if (ch::get_best_member_stat(attacker, 特技_神将, 武将能力_统率) > target_lead) return true;
					if (ch::get_best_member_stat(attacker, 特技_飞将, 武将能力_武力) > target_strength) return true;
					if (ch::get_best_member_stat(attacker, 特技_飞将, 武将能力_统率) > target_lead) return true;
					//骁将
					if (ch::get_best_member_stat(attacker, 152, 部队能力_武力) > target_strength) return true;
					break;

<<<<<<< HEAD
				case 병기_군마:
=======
				case 兵器_战马:
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					if (ch::has_skill(attacker, 특기_기신)) return true;
					if (ch::get_best_member_stat(attacker, 특기_기장, 武将能力_武力) > target_strength) return true;
					if (ch::get_best_member_stat(attacker, 특기_기장, 武将能力_统率) > target_lead) return true;
					if (ch::get_best_member_stat(attacker, 特技_神将, 武将能力_武力) > target_strength) return true;
					if (ch::get_best_member_stat(attacker, 特技_神将, 武将能力_统率) > target_lead) return true;
					if (ch::get_best_member_stat(attacker, 特技_飞将, 武将能力_武力) > target_strength) return true;
					if (ch::get_best_member_stat(attacker, 特技_飞将, 武将能力_统率) > target_lead) return true;
					//骁将
					if (ch::get_best_member_stat(attacker, 152, 部队能力_武力) > target_strength) return true;
					break;

<<<<<<< HEAD
				case 병기_충차:
				case 병기_정란:
				case 병기_투석:
				case 병기_목수:
=======
				case 兵器_冲车:
				case 兵器_井阑:
				case 兵器_投石:
				case 兵器_木兽:
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					if (ch::has_skill(attacker, 特技_攻城)) return true;
					break;

				case 兵器_走舸:
				case 兵器_楼船:
				case 兵器_斗舰:
					if (ch::has_skill(attacker, 特技_水神)) return true;
					if (ch::get_best_member_stat(attacker, 特技_水将, 武将能力_武力) > target_strength) return true;
					if (ch::get_best_member_stat(attacker, 特技_水将, 武将能力_统率) > target_lead) return true;
					break;
				}
			}
			// 공격, 일제
			else
			{
				if (ch::get_best_member_stat(attacker, 特技_神将, 武将能力_武力) > target_strength) return true;
				if (ch::get_best_member_stat(attacker, 特技_神将, 武将能力_统率) > target_lead) return true;
				if (ch::get_best_member_stat(attacker, 특기_구축, 武将能力_武力) > target_strength) return true;
				if (ch::get_best_member_stat(attacker, 특기_구축, 武将能力_统率) > target_lead) return true;

<<<<<<< HEAD
				if (pk::get_weapon_id(attacker) == 병기_군마)
=======
				if (pk::get_weapon_id(attacker) == 兵器_战马)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				{
					if (ranged and ch::has_skill(attacker, 특기_백마)) return true;
				}
			}

			// 공성 특기 보유, 건물 목표
			if (ch::has_skill(attacker, 特技_攻城) and target.is_instance(pk::building::type_id)) return true;

			// 부대 목표, 숲에서 공격, 난전 특기 보유
			if (target_unit !is null and pk::get_hex(attacker.get_pos()).terrain == 지형_숲 and ch::has_skill(attacker, 특기_난전)) return true;

			//山战
			if (target_unit !is null and pk::get_hex(attacker.pos).terrain == 地形_山 and ch::has_skill(attacker, 特技_山战)) return true;

			//背水
			if (target_unit !is null and pk::get_hex(attacker.pos).terrain == 地形_岸 and ch::has_skill(attacker, 特技_背水)) return true;


			int n = 基础暴击率, np = 0;
			pk::person@ leader = pk::get_person(attacker.leader);

			int 武力 = attacker.attr.stat[部队能力_武力];
			if (武力 >= 武力暴击需求)
			{
				np = int((武力 - 武力暴击需求) * 武力暴击系数 + 武力暴击下限);
				n = n + pk::min(武力暴击上限, np);
			}

			n = n + pk::get_tekisei(attacker) * 适性增加暴击;	//适性加成

			for (int i = 0; i < 2; i++)
			{
				int deputy = attacker.deputy[i];
				if (pk::is_valid_person_id(deputy))
				{
					if (pk::is_gikyoudai(leader, deputy) or pk::is_fuufu(leader, deputy))
						n = n + 关系暴击_仲介;
					else if (pk::is_like(leader, deputy))
						n = n + 关系暴击_亲爱;
					else if (pk::is_dislike(leader, deputy))
						n = n - 关系暴击_厌恶;
				}
			}

			int chance = 0;
			if (基础光环 and 战法暴击光环)
			{
				int dst_id = halo::func_get_highest_halo(attacker, 1);
				if (dst_id != -1)
				{
					auto dst = pk::get_unit(dst_id);
					chance = halo::func_光环效果(dst, 1);
				}
			}


			return pk::rand_bool(pk::min(100, pk::max(n + chance, 0)));
		}
	}

	Main main;
}
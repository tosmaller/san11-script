// ## 2021/11/15 # 江东新风 # 改造函数以适应妙计计算 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/12/24 # 江东新风 # 因倾国改为奇谋，所以取消性别判断 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/08/19 # 江东新风 # 原版特技has_skill设定 ##
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/08/14 #江东新风#将光环系统的计略成功&识破光环放入此处，新特技??料敌##
// ## 2020/07/26 ##
namespace STRATEGY_CHANCE
{
	//---------------------------------------------------------------------------------------
	const int 计略成功率下限 = 1;
	const int 计略成功率上限 = 99;
	//const bool 傾国无性別限制 = false;
	const int 混乱状态成功率增加 = 10;

	//---------------------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(206, pk::func206_t(callback));
		}

		pk::int_bool callback(pk::unit@ src, const pk::point& in src_pos, const pk::point& in dst_pos, int strategy_id)
		{
			pk::list<pk::person@> member_list;
			for (int i = 0; i < 3; i++)
			{
				if (pk::is_valid_person_id(src.member[i]))
				{
					pk::person@ person = pk::get_person(src.member[i]);
					if (pk::is_alive(person)) member_list.add(person);
				}			
			}
			return cal_strategy_chance(src, member_list, dst_pos,strategy_id);
		}

		bool is_miaoji(int strategy_id)
		{
			if (strategy_id > 19 and strategy_id < 30) return true;
			return false;
		}

		/** 시설에 따른 화계 상수 */
		int func_5af510(int facility_id)
		{
			switch (facility_id)
			{
			case 시설_도시:
			case 시설_관문:
			case 시설_성채:
			case 시설_석벽:
			case 시설_석병팔진:
			case 시설_동작대:
				return 10;

			case 시설_항구:
			case 시설_연노로:
			case 시설_투석대:
			case 시설_태고대:
			case 시설_군악대:
			case 시설_본거지1:
			case 시설_유적:
			case 시설_묘:
			case 시설_농장:
			case 시설_공방:
			case 시설_조선:
			case 시설_조폐:
			case 시설_곡창:
			case 시설_암시장:
			case 시설_군둔농:
			case 시설_농장2단:
			case 시설_농장3단:
				return 30;

			case 시설_진:
				return 35;

			case 시설_요새:
				return 25;

			case 시설_궁노:
			case 시설_본거지2:
			case 시설_시장:
			case 시설_병영:
			case 시설_대장간:
			case 시설_마구간:
			case 시설_부절태:
			case 시설_군사부:
			case 시설_인재부:
			case 시설_외교부:
			case 시설_계략부:
			case 시설_연병소:
			case 시설_대시장:
			case 시설_어시장:
			case 시설_시장2단:
			case 시설_시장3단:
			case 시설_병영2단:
			case 시설_병영3단:
			case 시설_대장간2단:
			case 시설_대장간3단:
			case 시설_마구간2단:
			case 시설_마구간3단:
				return 40;

			case 시설_토루:
			case 시설_제방:
				return 20;

			case 시설_불씨:
			case 시설_화염종:
			case 시설_화구:
			case 시설_화염구:
			case 시설_화선:
			case 시설_업화구:
			case 시설_업화종:
				return 100;

			case 시설_낙석:
			case 시설_얕은여:
			case 시설_산악:
			case 시설_장성:
				return 0;
			}
			return 0;
		}

		/** 지형에 따른 화계 상수 */
		int data_849a60(int terrain_id)
		{
			switch (terrain_id)
			{
			case 지형_초원: return 40;
			case 지형_땅: return 40;
			case 지형_모래: return 40;
			case 지형_습지: return 10;
			case 지형_독천: return 10;
			case 지형_숲: return 50;
			case 지형_내: return 0;
			case 지형_강: return 0;
			case 지형_바다: return 0;
			case 지형_황무지: return 35;
			case 지형_대로: return 30;
			case 지형_벼랑길: return 15;
			case 지형_다리: return 20;
			case 지형_여울: return 10;
			case 지형_물가: return 0;
			case 지형_산: return 0;
			case 지형_도시: return 0;
			case 지형_관문: return 0;
			case 지형_항구: return 0;
			case 지형_샛길: return 10;
			}
			return 0;
		}

		/** 성격에 따른 복병 상수 */
		int data_849af4(int character)
		{
			switch (character)
			{
			case 성격_소심: return 3;
			case 성격_냉정: return -2;
			case 성격_대담: return 0;
			case 성격_저돌: return 1;
			}
			return 0;
		}

		/** 성격에 따른 진정 상수 */
		int data_849b04(int character)
		{
			switch (character)
			{
			case 성격_소심: return -5;
			case 성격_냉정: return 5;
			case 성격_대담: return 0;
			case 성격_저돌: return -10;
			}
			return 0;
		}

		/** 성격에 따른 교란 상수 */
		int data_849b14(int character)
		{
			switch (character)
			{
			case 성격_소심: return -2;
			case 성격_냉정: return 0;
			case 성격_대담: return 1;
			case 성격_저돌: return 3;
			}
			return 0;
		}

		/** 성격에 따른 위보 상수 */
		int data_849b24(int character)
		{
			switch (character)
			{
			case 성격_소심: return 3;
			case 성격_냉정: return 1;
			case 성격_대담: return 0;
			case 성격_저돌: return -2;
			}
			return 0;
		}
	}

	Main main;

	pk::int_bool cal_strategy_chance(pk::hex_object@ src, pk::list<pk::person@>member, const pk::point& in dst_pos, int strategy_id)
	{
		if (!pk::is_alive(src))
			return pk::int_bool(0, false);
		pk::building@ src_building = src.get_type_id() == pk::building::type_id ? src : null;
		pk::unit@ src_unit = src.get_type_id() == pk::unit::type_id ? src : null;


		int src_int = 1; 
		if (src_unit !is null) src_int = src_unit.attr.stat[部队能力_智力];  //施法部队智力
		else if (src_building !is null) src_int = member[0].stat[武将能力_智力];  //执行者智力

		int dst_int = 0;  //目标部队智力
		pk::unit@ dst = pk::get_unit(dst_pos);
		int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, n = 0;

		// 목표 부대 존재
		if (dst !is null)
		{
			// 낙뢰가 아닌 경우 목표 부대가 통찰 특기를 가지고 있다면 항상 실패
			if (strategy_id != 计略_落雷 and pk::is_enemy(src, dst) and ch::has_skill(dst, 特技_洞察)) return pk::int_bool(0, true);

			dst_int = dst.attr.stat[部队能力_智力];
			if (dst.status == 部队状态_混乱)
				a = 混乱状态成功率增加;

			//计策成功率光环，计策识破率光环
			if ((基础光环) and (计策成功率光环))
			{
				int dst_chance = 0;
				int dst2_chance = 0;
				if (src_unit !is null)//只有部队才适用光环
				{
					int dst_id = halo::func_get_highest_halo(src, 3);//计策成功率光环
					if (dst_id != -1)
					{
						auto dst1 = pk::get_unit(dst_id);
						dst_chance = halo::func_光环效果(dst1, 3);
					}
				}
				int dst2_id = halo::func_get_highest_halo(pk::get_unit(dst_pos), 4);//计策率识破光环
				if (dst2_id != -1)
				{
					auto dst2 = pk::get_unit(dst2_id);
					dst2_chance = halo::func_光环效果(dst2, 4);
				}
				a += dst_chance;
				a -= dst2_chance;
			}


		}

		switch (strategy_id)
		{
			//火计
		case 计略_火计:
		case 据点计略_火攻:
			b = pk::get_building(dst_pos) !is null ? main.func_5af510(pk::get_building(dst_pos).facility) : main.data_849a60(pk::get_hex(dst_pos).terrain);
			if (b == 100) return pk::int_bool(100, false);

			c = 100;
			c -= dst_int * 9 / 10;
			c /= 2;
			c *= src_int * src_int * 100;
			c /= dst_int * dst_int + src_int * src_int;
			c /= 55;
			c -= (100 - src_int) / 10;
			c += a;
			c += b / 2;
			c -= 5;
			if (src_int < dst_int) c -= (dst_int - src_int) / 3;
			if (c < 1) c = 1;

			d = a + src_int * 3 / 10 - dst_int / 5 + 55;
			if (dst is null) d += 10;

			n = pk::max(1, pk::min(99, c, d));

			if (dst !is null)
			{
				if (ch::has_skill(dst, 特技_火神)) return pk::int_bool(0, true);
				if (ch::get_best_member_stat(dst, 特技_看破, 武将能力_智力) > src_int) return pk::int_bool(0, true);
				if (ch::get_best_member_stat(dst, 特技_神算, 武将能力_智力) > src_int) return pk::int_bool(0, true);

				if (ch::get_best_member_stat(member, 特技_火攻, 武将能力_智力) > dst_int)
				{
					if (ch::has_skill(dst, 特技_料敌))
						return pk::int_bool(50, false);
					return pk::int_bool(100, false);
				}
				if (ch::get_best_member_stat(member, 特技_虚实, 武将能力_智力) > dst_int)
				{
					if (ch::has_skill(dst, 特技_料敌))
						return pk::int_bool(50, false);
					return pk::int_bool(100, false);
				}
				if (ch::get_best_member_stat(member, 特技_火神, 武将能力_智力) > dst_int)
				{
					if (ch::has_skill(dst, 特技_料敌))
						return pk::int_bool(50, false);
					return pk::int_bool(100, false);
				}
				if (ch::get_best_member_stat(member, 特技_神算, 武将能力_智力) > dst_int)
				{
					if (ch::has_skill(dst, 特技_料敌))
						return pk::int_bool(50, false);
					return pk::int_bool(100, false);
				}
			}
			break;

			//灭火
		case 计略_灭火:
			if (ch::get_best_member_stat(member, 特技_火攻, 武将能力_智力) > dst_int) return pk::int_bool(100, false);
			if (ch::get_best_member_stat(member, 特技_虚实, 武将能力_智力) > dst_int) return pk::int_bool(100, false);
			if (ch::get_best_member_stat(member, 特技_火神, 武将能力_智力) > dst_int) return pk::int_bool(100, false);
			if (ch::get_best_member_stat(member, 特技_神算, 武将能力_智力) > dst_int) return pk::int_bool(100, false);

			b = int(sqrt(pk::max(10, src_int - 5)) * 11);

			n = pk::max(1, pk::min(99, b));
			break;

			//伪报
		case 计略_伪报:
		case 妙计_大伪报:
		case 据点计略_伪报:
			if (dst is null) return pk::int_bool(0, false);

			if (ch::has_skill(dst, 特技_规律)) return pk::int_bool(0, true);
			if (ch::has_skill(dst, 特技_明镜)) return pk::int_bool(0, true);

			if (ch::get_best_member_stat(member, 特技_言毒, 武将能力_智力) > dst_int)
			{
				if (ch::has_skill(dst, 特技_料敌))
					return pk::int_bool(50, false);
				return pk::int_bool(100, false);
			}
			if (ch::get_best_member_stat(member, 特技_虚实, 武将能力_智力) > dst_int)
			{
				if (ch::has_skill(dst, 特技_料敌))
					return pk::int_bool(50, false);
				return pk::int_bool(100, false);
			}
			if (ch::get_best_member_stat(member, 特技_神算, 武将能力_智力) > dst_int)
			{
				if (ch::has_skill(dst, 特技_料敌))
					return pk::int_bool(50, false);
				return pk::int_bool(100, false);
			}

			if (ch::get_best_member_stat(dst, 特技_看破, 武将能力_智力) > src_int) return pk::int_bool(0, true);
			if (ch::get_best_member_stat(dst, 特技_神算, 武将能力_智力) > src_int) return pk::int_bool(0, true);

			b = main.data_849b24(pk::get_person(dst.leader).character);
			c = ch::get_best_member_stat(member, 部队能力_魅力)/ 20;

			d = src_int * 3 / 10 - dst_int / 5;
			d += b;
			d += c;

			e = a + d + 70;

			f = 100;
			f -= dst_int * 9 / 10;
			f *= src_int * src_int * 100;
			f /= dst_int * dst_int + src_int * src_int;
			f /= 55;
			f -= (100 - src_int) / 10;
			f += a;
			f += b;
			f += c;
			if (src_int < dst_int) f -= (dst_int - src_int) / 3;
			if (f < 1) f = 1;

			n = pk::max(1, pk::min(99, e, f));
			break;

			//扰乱
		case 计略_扰乱:
		case 妙计_大混乱:
		case 据点计略_扰乱:
			if (dst is null) return pk::int_bool(0, false);

			if (ch::has_skill(dst, 特技_沉着)) return pk::int_bool(0, true);
			if (ch::has_skill(dst, 特技_明镜)) return pk::int_bool(0, true);

			if (ch::get_best_member_stat(member, 特技_机略, 武将能力_智力) > dst_int)
			{
				if (ch::has_skill(dst, 特技_料敌))
					return pk::int_bool(50, false);
				return pk::int_bool(100, false);
			}
			if (ch::get_best_member_stat(member, 特技_虚实, 武将能力_智力) > dst_int)
			{
				if (ch::has_skill(dst, 特技_料敌))
					return pk::int_bool(50, false);
				return pk::int_bool(100, false);
			}
			if (ch::get_best_member_stat(member, 特技_神算, 武将能力_智力) > dst_int)
			{
				if (ch::has_skill(dst, 特技_料敌))
					return pk::int_bool(50, false);
				return pk::int_bool(100, false);
			}

			if (ch::get_best_member_stat(dst, 特技_看破, 武将能力_智力) > src_int) return pk::int_bool(0, true);
			if (ch::get_best_member_stat(dst, 特技_神算, 武将能力_智力) > src_int) return pk::int_bool(0, true);

			b = main.data_849b14(pk::get_person(dst.leader).character);
			c = dst.attr.stat[部队能力_防御] / 20;

			d = src_int * 3 / 10 - dst_int / 5;
			d += b;
			d += c;

			e = a + d + 70;

			f = 100;
			f -= dst_int * 9 / 10;
			f *= src_int * src_int * 100;
			f /= dst_int * dst_int + src_int * src_int;
			f /= 45;
			f -= (100 - src_int) / 10;
			f += a;
			f += b;
			f += c;
			if (src_int < dst_int) f -= (dst_int - src_int) / 3;
			if (f < 1) f = 1;

			n = pk::max(1, pk::min(99, e, f));
			break;

			//镇静
		case 计略_镇静:
		case 据点计略_镇静:
		case 据点计略_鼓舞:
		case 据点计略_援军:
			if (dst is null) return pk::int_bool(0, false);

			if (ch::get_best_member_stat(member, 特技_虚实, 武将能力_智力) > dst_int) return pk::int_bool(100, false);
			if (ch::get_best_member_stat(member, 特技_神算, 武将能力_智力) > dst_int) return pk::int_bool(100, false);

			b = int(sqrt(pk::max(10, src_int - 5)) * 11 + main.data_849b04(pk::get_person(dst.leader).character));

			n = pk::max(1, pk::min(99, b));
			break;

			//伏兵
		case 计略_伏兵:
		case 据点计略_奇袭攻具:
			if (dst is null) return pk::int_bool(0, false);

			if (ch::get_best_member_stat(member, 特技_虚实, 武将能力_智力) > dst_int)
			{
				if (ch::has_skill(dst, 特技_料敌))
					return pk::int_bool(50, false);
				return pk::int_bool(100, false);
			}
			if (ch::get_best_member_stat(member, 特技_神算, 武将能力_智力) > dst_int)
			{
				if (ch::has_skill(dst, 特技_料敌))
					return pk::int_bool(50, false);
				return pk::int_bool(100, false);
			}

			if (ch::get_best_member_stat(dst, 特技_看破, 武将能力_智力) > src_int) return pk::int_bool(0, true);
			if (ch::get_best_member_stat(dst, 特技_神算, 武将能力_智力) > src_int) return pk::int_bool(0, true);

			b = main.data_849af4(pk::get_person(dst.leader).character);

			if (src_unit !is null) c = int(src_unit.attr.stat[部队能力_攻击] - dst.attr.stat[部队能力_防御]) / 10;

			d = src_int * 3 / 10;
			d -= dst_int / 3;
			d += b;
			d += c;

			e = a + d + 80;

			f = 100;
			f -= dst_int * 9 / 10;
			f *= src_int * src_int * 100;
			f /= dst_int * dst_int + src_int * src_int;
			f /= 50;
			f -= (100 - src_int) / 10;
			f += a;
			f += b;
			f += c;
			if (src_int < dst_int) f -= (dst_int - src_int) / 3;
			if (strategy_id == 据点计略_奇袭攻具)
			{
				if (!pk::is_in_water(dst) and ch::has_skill(member, 特技_突袭)) { f += 20; e += 20; }
				if (pk::is_in_water(dst) and ch::has_skill(member, 特技_强袭)) { f += 20; e += 20; }
			}

			if (f < 1) f = 1;

			n = pk::max(1, pk::min(99, e, f));
			break;

			//内讧
		case 计略_内讧:
		case 据点计略_攻心:
		case 据点计略_待劳:						   
			if (dst is null) return pk::int_bool(0, false);

			if (ch::get_best_member_stat(member, 特技_诡计, 武将能力_智力) > dst_int)
			{
				if (ch::has_skill(dst, 特技_料敌))
					return pk::int_bool(50, false);
				return pk::int_bool(100, false);
			}
			if (ch::get_best_member_stat(member, 特技_虚实, 武将能力_智力) > dst_int)
			{
				if (ch::has_skill(dst, 特技_料敌))
					return pk::int_bool(50, false);
				return pk::int_bool(100, false);
			}
			if (ch::get_best_member_stat(member, 特技_神算, 武将能力_智力) > dst_int)
			{
				if (ch::has_skill(dst, 特技_料敌))
					return pk::int_bool(50, false);
				return pk::int_bool(100, false);
			}

			if (ch::get_best_member_stat(dst, 特技_看破, 武将能力_智力) > src_int) return pk::int_bool(0, true);
			if (ch::get_best_member_stat(dst, 特技_神算, 武将能力_智力) > src_int) return pk::int_bool(0, true);

			b = main.data_849b14(pk::get_person(dst.leader).character);
			c = src_int * 3 / 10 - dst_int * 4 / 10 + b;
			e = a + c + 70;

			f = 100;
			f -= dst_int * 9 / 10;
			f *= src_int * src_int * 100;
			f /= dst_int * dst_int + src_int * src_int;
			f /= 55;
			f -= (100 - src_int) / 10;
			f += a;
			f += b;
			if (src_int < dst_int) f -= (dst_int - src_int) / 3;
			if (f < 1) f = 1;

			n = pk::max(1, pk::min(99, e, f));
			break;

			//妖术
		case 计略_妖术:
			if (dst is null) return pk::int_bool(0, false);

			b = main.data_849b14(pk::get_person(dst.leader).character);
			c = 100 - dst_int * 9 / 10;

			d = src_int / 10;
			d += a;
			d += b;
			d += c;

			if (dst_int > src_int)
			{
				b -= (100 - src_int) / 10;
				b += c;
				d = a + b - 5;
			}

			n = pk::max(1, pk::min(99, d));

			if (ch::has_skill(dst, 特技_料敌))
				return pk::int_bool((n + pk::core::strategy_chance(member[0].get_id())) / 2, false);
			return pk::int_bool(n + pk::core::strategy_chance(member[0].get_id()), false);

			//落雷
		case 计略_落雷:
		case 妙计_神火计:
		case 妙计_八阵图:
		case 妙计_虚虚实实:
		case 妙计_速战固守:
			b = int(src_int * 0.6f);
			c = a + b + 15;
			n = pk::max(1, pk::min(99, c));
			if (ch::has_skill(dst, 特技_料敌))
				return pk::int_bool((n + pk::core::strategy_chance(member[0].get_id())) / 2, false);
			return pk::int_bool(n + pk::core::strategy_chance(member[0].get_id()), false);
		}

		if (ch::has_skill(member, 特技_倾国) and dst !is null /*and (!pk::has_female_member(dst) or 傾国无性別限制)*/)
			n *= 2;

		int n_ = n + (pk::is_valid_person_id(member[0].get_id())?pk::core::strategy_chance(member[0].get_id()):0);

		if (main.is_miaoji(strategy_id))
		{
			n_ = int(n * 0.8f);//妙计成功率正常情况下为普通计略的0.8
			if (ch::has_skill(member, 特技_谋略)) n_ += 谋略成功率上升;
		}

		//料敌对最终成功率的影响
		if (dst !is null)
		{
			if (ch::has_skill(dst, 特技_料敌))
				n_ /= 2;
		}

		n_ = pk::max(计略成功率下限, pk::min(计略成功率上限, n_));  //自定义计略成功率上下限			
		return pk::int_bool(n_, false);
	}
}
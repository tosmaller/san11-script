// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/08/19 # 江东新风 # 原版特技has_skill设定 ##
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/07/26 ##
namespace STRATEGY_CRITICAL_CHANCE
{
	//---------------------------------------------------------------------------------------
	const int 计略暴击率下限 = 3;
	//---------------------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(207, pk::func207_t(callback));
		}

		int callback(pk::unit@ src, const pk::point& in src_pos, const pk::point& in dst_pos, int strategy_id)
		{
			if (!pk::is_alive(src) or !pk::is_valid_pos(src_pos) or !pk::is_valid_pos(dst_pos))
				return 0;

			int src_int = src.attr.stat[部队能力_智力];
			int dst_int = 0;

			pk::unit@ dst = pk::get_unit(dst_pos);
			if (dst !is null)
				dst_int = dst.attr.stat[部队能力_智力];

			if (strategy_id >= 0 and strategy_id <= 계략_동토 and strategy_id != 계략_소화)
			{
				if (ch::has_skill(src, 특기_신산)) return 100;
				if (ch::has_skill(src, 특기_심모)) return 100;
				// 모계 특기 보유자 지력이 목표 부대 지력보다 높음
				if (ch::get_best_member_stat(src, 특기_묘계, 武将能力_智力) > dst_int) return 100;
				// 비책 특기 보유자 지력이 목표 부대 지력보다 낮음
				int n = ch::get_best_member_stat(src, 특기_비책, 武将能力_智力);
				if (n > -1 and n < dst_int) return 100;
			}

			switch (strategy_id)
			{
			case 계략_화계:
				return pk::max(计略暴击率下限, (src_int - dst_int / 2) / 10);
			case 계략_소화:
				return 0;
			case 계략_위보:
				return pk::max(计略暴击率下限, (src_int - dst_int / 2 + data_849b58(pk::get_person(src.leader).character)) / 10);
			case 계략_교란:
				return pk::max(计略暴击率下限, (src_int - dst_int / 2 + data_849b48(pk::get_person(src.leader).character)) / 10);
			case 계략_진정:
				return pk::max(计略暴击率下限, (src_int) / 10);
			case 계략_복병:
				if (ch::has_skill(src, 특기_매복)) return 100;
				return pk::max(计略暴击率下限, (src_int - dst_int / 2 + fukuhei_crit_weapon_factor(pk::get_weapon_id(src, src_pos))) / 10);
			case 계략_동토:
				return pk::max(计略暴击率下限, (src_int - dst_int / 2 + data_849b34(pk::get_person(src.leader).giri)) / 10);
			case 계략_요술:
			case 계략_낙뢰:
				return pk::max(计略暴击率下限, (src_int) / 12);
			}

			return 0;
		}

		/** 의리에 따른 동토 상수 */
		int data_849b34(int giri)
		{
			switch (giri)
			{
			case 의리_매우낮음: return 10;
			case 의리_낮음: return 5;
			case 의리_보통: return 0;
			case 의리_높음: return -5;
			case 의리_매우높음: return -10;
			}
			return 0;
		}

		/** 병기에 따른 복병 상수 */
		int fukuhei_crit_weapon_factor(int weapon_id)
		{
			switch (weapon_id)
			{
			case 병기_검: return 5;
			case 병기_창: return 10;
			case 병기_극: return 10;
			case 병기_노: return 5;
			}
			return 0;
		}

		/** 성격에 따른 교란 상수 */
		int data_849b48(int character)
		{
			switch (character)
			{
			case 성격_소심: return -5;
			case 성격_냉정: return 0;
			case 성격_대담: return 5;
			case 성격_저돌: return 10;
			}
			return 0;
		}

		/** 성격에 따른 위보 상수 */
		int data_849b58(int character)
		{
			switch (character)
			{
			case 성격_소심: return 10;
			case 성격_냉정: return 5;
			case 성격_대담: return 0;
			case 성격_저돌: return -5;
			}
			return 0;
		}
	}

	Main main;
}
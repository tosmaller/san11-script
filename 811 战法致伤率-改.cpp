// ## 2022/03/17 # 江东新风 # critical数组越界bug修复##
// ## 2022/02/25 # 江东新风 # 函数大改，解决战法使自己致伤问题##
// ## 2021/10/13 # 江东新风 # 匹配新209函数参数##
// ## 2021/02/17 # 江东新风 # 致伤函数重写 ##
// ## 2021/02/17 # 江东新风 # 伤病状态能力更新 ##
// ## 2021/02/17 # 江东新风 # 狙击概率修复 ##
// ## 2020/12/05 # 氕氘氚 # 调整概率计算规则 ##
// @@ 2020/10/26 @ 氕氘氚 @@


namespace 战法_受伤_概率
{
	const bool 枪兵战法致伤 = true;
	const bool 戟兵战法致伤 = false;
	const bool 弩兵战法致伤 = true;
	const bool 骑兵战法致伤 = true;
	const bool 井栏战法致伤 = true;
	const bool 投石战法致伤 = true;
	const bool 木兽战法致伤 = true;
	const bool 水军战法致伤 = true;

	const bool 溅射伤害致伤 = true;

	/// ---------------------------------------------------------------------------------------------

	const int 基本致伤率_突刺 = 0;
	const int 基本致伤率_突刺_暴击 = 0;
	const int 基本致伤率_螺旋突 = 1;
	const int 基本致伤率_螺旋突_暴击 = 2;
	const int 基本致伤率_二段突 = 1;
	const int 基本致伤率_二段突_暴击 = 2;

	/// ---------------------------------------------------------------------------------------------

	const int 基本致伤率_熊手 = 0;
	const int 基本致伤率_熊手_暴击 = 0;
	const int 基本致伤率_横扫 = 0;
	const int 基本致伤率_横扫_暴击 = 0;
	const int 基本致伤率_旋风 = 0;
	const int 基本致伤率_旋风_暴击 = 0;

	/// ---------------------------------------------------------------------------------------------

	const int 基本致伤率_火矢 = 2;
	const int 基本致伤率_火矢_暴击 = 4;
	const int 基本致伤率_贯箭 = 2;
	const int 基本致伤率_贯箭_暴击 = 4;
	const int 基本致伤率_乱射 = 2;
	const int 基本致伤率_乱射_暴击 = 4;

	/// ---------------------------------------------------------------------------------------------

	const int 基本致伤率_突击 = 3;
	const int 基本致伤率_突击_暴击 = 5;
	const int 基本致伤率_突破 = 3;
	const int 基本致伤率_突破_暴击 = 5;
	const int 基本致伤率_突进 = 4;
	const int 基本致伤率_突进_暴击 = 6;

	/// ---------------------------------------------------------------------------------------------

	const int 基本致伤率_火箭 = 2;
	const int 基本致伤率_火箭_暴击 = 4;

	const int 基本致伤率_投石 = 4;
	const int 基本致伤率_投石_暴击 = 6;

	const int 基本致伤率_放射 = 2;
	const int 基本致伤率_放射_暴击 = 4;

	/// ---------------------------------------------------------------------------------------------

	const int 基本致伤率_舰船火矢 = 2;
	const int 基本致伤率_舰船火矢_暴击 = 4;

	const int 基本致伤率_舰船猛撞 = 2;
	const int 基本致伤率_舰船猛撞_暴击 = 4;

	const int 基本致伤率_舰船投石 = 3;
	const int 基本致伤率_舰船投石_暴击 = 5;

	/// ---------------------------------------------------------------------------------------------

	const int 战死_无 = 0;
	const int 战死_标准 = 2;
	const int 战死_高战死 = 5;

	const int 受伤_性格_小心 = -3;
	const int 受伤_性格_冷静 = -1;
	const int 受伤_性格_刚胆 = 0;
	const int 受伤_性格_莽撞 = 2;

	const bool 开启受伤对话 = true;

	const array<string> 伤病类型 =
	{
		"健康",
		"\x1b[18x轻伤\x1b[0x",
		"\x1b[17x重伤\x1b[0x",
		"\x1b[16x濒死\x1b[0x"
	};

	const bool 调试模式 = false;
	/// =============================================================================================

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		array<bool> 兵种战法致伤_启用(병기_끝);
		array<array<int>> 基本致伤率(전법_끝, array<int>(0));
		array<int> 致伤率_战死设定(3);
		array<int> 致伤率_性格影响(4);

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			init();

			pk::bind(179, pk::trigger179_t(部队攻击操作后));//在攻心函数前，包括弩兵(去除原弩兵致伤判定)
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void 部队攻击操作后(pk::unit@ dst_unit, pk::unit@ unit, int troops_damage, int critical, int tactics)
		{
			bool result = pk::is_alive(dst_unit);
			int tactic_eff = 0;
			if (调试模式) pk::trace("部队攻击操作后 pos1");
			if (pk::is_alive(dst_unit))
			{
				if (调试模式) pk::trace("部队攻击操作后 pos2");
				//result = func_598330(dst_unit);
				pk::person@ dst_person = func_598330(dst_unit);//排除队伍中的血路和幸运武将
				//s11::Person* dst_person = s11::fxx::func_495f80(dst_unit);
				if (dst_person !is null)
				{
					if (调试模式) pk::trace("部队攻击操作后 pos3");
					//spdlog::get("core")->info("info:{},tactic_id:{}", 1, 11);
					//int character = dst_person.character;
					pk::person@ leader = pk::get_person(unit.leader);
					//int stat_eff = func_5975d0(dst_person, leader);//获取能力差效果
					//int rate = character + stat_eff + (critical != 0 ? 1 : 0) + tactic_eff - 1;
					int rate = pk::is_valid_tactics_id(tactics)?func_最终致伤率(unit, leader, dst_person, (dst_unit.leader != dst_person.get_id() ? 1 : 0), tactics, critical>0?1:0):0;

					//result = pk::rand_bool(character + stat_eff + (critical != 0) + tactic_eff - 1);
					if (pk::rand_bool(rate))
					{
						if (调试模式) pk::trace("部队攻击操作后 pos4");
						int kunshu_id = pk::get_kunshu_id(dst_person);
						pk::person@ kunshu = pk::get_person(kunshu_id);
						if (pk::is_alive(kunshu))
						{
							pk::msg_param msg_param(7462, dst_person, kunshu);
							@msg_param.unit[0] = @dst_unit;
							pk::say(pk::get_msg(msg_param), dst_person, dst_unit);// 不過是這種程度的傷！
						}
						func_597530(troops_damage, dst_person, dst_unit);//设置伤病
					}
				}
			}
			return;
		}

		pk::person@ func_598330(pk::unit@ dst_unit)//排除队伍中的血路和幸运武将
		{
			if (ch::has_skill(dst_unit, 特技_血路)) return null;
			pk::list<pk::person@> list;
			int count = 0;
			for (int i = 0; i < 3; ++i)
			{
				if (pk::is_valid_person_id(dst_unit.member[i])) count += 1;
			}

			for (int i = 0; i < count; ++i)
			{
				pk::person@ member = pk::get_person(dst_unit.member[i]);
				if (pk::is_alive(member) and !ch::has_skill(member, 特技_强运)) list.add(member);
			}
			if (list.count > 0)
			{
				list.shuffle();
				pk::person@ person = @list[0];
				list.clear();
				return person;
			}
			return null;
		}

		int func_5975d0(pk::person@ src, pk::person@ dst)//获取能力差效果
		{
			uint8 dst_max_stat = pk::max(dst.stat[武将能力_统率], dst.stat[武将能力_武力]);
			uint8 src_max_stat = pk::max(src.stat[武将能力_统率], src.stat[武将能力_武力]);
			uint8 stat_dif = dst_max_stat - src_max_stat;

			if (stat_dif <= 0)
				return -2;
			if (stat_dif > 6) return 0;
			if (stat_dif > 12) return 1;
			return -1;
		}

		void func_597530(int troops_damage, pk::person@ dst, pk::unit@ dst_unit)//设置伤病
		{
			bool damage_eff; // eax
			int shoubyou; // esi
			if (troops_damage <= 1500)
				damage_eff = pk::rand_bool(80);
			else
				damage_eff = pk::rand_bool(60);
			shoubyou = dst.shoubyou + 2 - (damage_eff ? 1 : 0);
			if (shoubyou >= 3)
				shoubyou = 3;
			dst.shoubyou = shoubyou;//设置伤病

			pk::msg_param msg_param(8303, dst, null);
			pk::force@ force = pk::get_force(dst.get_force_id());
			msg_param.str[0] = pk::encode(伤病类型[shoubyou]);
			pk::history_log(dst.get_pos(), force.color, pk::get_msg(msg_param));
			return;
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		int func_最终致伤率(pk::unit@ attacker, pk::person@ attacker_leader, pk::person@ def_member, int member_pos, int tactics_id_, int critical)
		{
			int n = 0;
			if (member_pos > 0) n -= 2;  //副将更不容易受伤
			
			n += 基本致伤率[tactics_id_][critical];
			n += 致伤率_战死设定[pk::get_scenario().battle_death];
			n += 致伤率_性格影响[def_member.character];

			// 特技修正致伤率
			if (tactics_id_ >= 战法_突刺 && tactics_id_ <= 战法_旋风 && ch::has_skill(attacker, 特技_猛卒))
				n += 猛卒击伤概率;
			if (batt::is_弓箭战法(tactics_id_) && ch::has_skill(attacker, 特技_狙击))
				n += 狙击击伤概率;
			if (tactics_id_ >= 战法_突击 && tactics_id_ <= 战法_突进 && ch::has_skill(attacker, 特技_猛骑))
				n += 猛骑击伤概率;

			// 能力值修正致伤率
			n -= func_5975d0(attacker_leader, def_member);
			/*
			if (def_member.stat[武将能力_武力] >= 90) n -= 3;
			else if (def_member.stat[武将能力_武力] >= 80) n -= 2;
			else if (def_member.stat[武将能力_武力] >= 70) n -= 1;
			if (def_member.stat[武将能力_智力] >= 90) n -= 2;
			else if (def_member.stat[武将能力_智力] >= 80) n -= 1;
			*/

			// ch::u8debug(2, pk::format("击伤概率{}%", n));
			return ch::inRange(n, 0, 100);
		}

		int func_致伤率_特技()
		{
			return 0;
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void init()
		{
			兵种战法致伤_启用[兵器_剑] = false;
			兵种战法致伤_启用[兵器_枪] = 枪兵战法致伤;
			兵种战法致伤_启用[兵器_戟] = 戟兵战法致伤;
			兵种战法致伤_启用[兵器_弩] = 弩兵战法致伤;
			兵种战法致伤_启用[兵器_战马] = 骑兵战法致伤;
			兵种战法致伤_启用[兵器_冲车] = false;
			兵种战法致伤_启用[兵器_井阑] = 井栏战法致伤;
			兵种战法致伤_启用[兵器_投石] = 投石战法致伤;
			兵种战法致伤_启用[兵器_木兽] = 木兽战法致伤;
			兵种战法致伤_启用[兵器_走舸] = 水军战法致伤;
			兵种战法致伤_启用[兵器_楼船] = 水军战法致伤;
			兵种战法致伤_启用[兵器_斗舰] = 水军战法致伤;

			基本致伤率[전법_돌출] = { 基本致伤率_突刺, 基本致伤率_突刺_暴击 };
			基本致伤率[전법_나선첨] = { 基本致伤率_螺旋突, 基本致伤率_螺旋突_暴击 };
			基本致伤率[전법_이단첨] = { 基本致伤率_二段突, 基本致伤率_二段突_暴击 };
			基本致伤率[전법_갈퀴] = { 基本致伤率_熊手, 基本致伤率_熊手_暴击 };
			基本致伤率[전법_횡치] = { 基本致伤率_横扫, 基本致伤率_横扫_暴击 };
			基本致伤率[전법_선풍] = { 基本致伤率_旋风, 基本致伤率_旋风_暴击 };
			基本致伤率[전법_화시] = { 基本致伤率_火矢, 基本致伤率_火矢_暴击 };
			基本致伤率[전법_관시] = { 基本致伤率_贯箭, 基本致伤率_贯箭_暴击 };
			基本致伤率[전법_난사] = { 基本致伤率_乱射, 基本致伤率_乱射_暴击 };
			基本致伤率[전법_돌격] = { 基本致伤率_突击, 基本致伤率_突击_暴击 };
			基本致伤率[전법_돌파] = { 基本致伤率_突破, 基本致伤率_突破_暴击 };
			基本致伤率[전법_돌진] = { 基本致伤率_突进, 基本致伤率_突进_暴击 };
			基本致伤率[전법_공성화시] = { 基本致伤率_火箭, 基本致伤率_火箭_暴击 };
			基本致伤率[전법_공성파쇄] = { 0, 0 };
			基本致伤率[전법_공성방사] = { 基本致伤率_放射, 基本致伤率_放射_暴击 };
			基本致伤率[전법_공성투석] = { 基本致伤率_投石, 基本致伤率_投石_暴击 };
			基本致伤率[전법_함선화시] = { 基本致伤率_舰船火矢, 基本致伤率_舰船火矢_暴击 };
			基本致伤率[전법_함선격돌] = { 基本致伤率_舰船猛撞, 基本致伤率_舰船猛撞_暴击 };
			基本致伤率[전법_함선투석] = { 基本致伤率_舰船投石, 基本致伤率_舰船投石_暴击 };

			致伤率_战死设定[0] = 战死_无;
			致伤率_战死设定[1] = 战死_标准;
			致伤率_战死设定[2] = 战死_高战死;

			致伤率_性格影响[성격_소심] = 受伤_性格_小心;
			致伤率_性格影响[성격_냉정] = 受伤_性格_冷静;
			致伤率_性格影响[성격_대담] = 受伤_性格_刚胆;
			致伤率_性格影响[성격_저돌] = 受伤_性格_莽撞;
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	}

	Main main;
}

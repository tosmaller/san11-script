// ## 2021/10/13 #江东新风#匹配新209函数参数##
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
	const array<string> 受伤类型 =
	{
		"受了轻伤",
		"受了重伤",
		"重伤濒死"
	};
	const array<string> 受伤对话 =
	{
		"可恶...",
		"这点伤不算什么...",
		"被摆了一道嘛..."
	};





	/// =============================================================================================

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

				// 병기 번호 : 0(검), 1(창), 2(극), 3(노), 4(군마), 5(충차), 6(정란), 7(투석), 8(목수), 9(주가), 10(누선), 11(투함)
				// 전법 번호 : 0(돌출), 1(나선첨), 2(이단첨), 3(갈퀴), 4(선풍), 5(횡치), 6(화시), 7(관시), 8(난사), 9(돌격), 10(돌파), 11(돌진), 12(공성화시), 13(공성파쇄), 14(공성방사), 15(공성투석), 16(함선화시), 17(함선격돌), 18(함선투석)

		pk::func209_t@ prev_callback_;
		pk::unit@ attacker_;
		pk::unit@ target_unit_;
		array<int> target_unit_id_arr(0);
		pk::person@ attacker_leader;
		pk::person@ def_member;
		int tactics_id_;
		int critical_;

		array<bool> 兵种战法致伤_启用(병기_끝);
		array<array<int>> 基本致伤率(전법_끝, array<int>(0));
		array<int> 致伤率_战死设定(3);
		array<int> 致伤率_性格影响(4);

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			init();
			//func_설정반영();

			@prev_callback_ = cast<pk::func209_t@>(pk::get_func(209));
			pk::reset_func(209);
			pk::set_func(209, pk::func209_t(func209));

			pk::bind(173, pk::trigger173_t(部队经验获得_战法致伤率));
			//pk::bind(171, pk::trigger171_t(部队清除_战法死亡率));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void func209(pk::damage_info& info, pk::unit@ attacker, int tactics_id, const pk::point& in target_pos, int type, int critical, bool ambush, int rettype)
		{
			prev_callback_(info, attacker, tactics_id, target_pos, type, critical, ambush, rettype);

			if (pk::is_campaign()) return;
			if (pk::is_mission_running()) return;

			pk::unit@ target_unit = pk::get_unit(target_pos);
			if (target_unit is null) return;
			// if (type != 0) return;  //主动攻击
			if (!pk::is_valid_tactics_id(tactics_id)) return;
			if (!info.tactics_hit) return;
			if (!兵种战法致伤_启用[attacker.weapon]) return;
			if (!溅射伤害致伤 && attacker_ !is null) return;
			if (target_unit.has_skill(特技_护卫)) return;

			@attacker_ = @attacker;
			@attacker_leader = pk::get_person(attacker.leader);
			if (target_unit_id_arr.find(target_unit.get_id()) < 0) target_unit_id_arr.insertLast(target_unit.get_id());
			tactics_id_ = tactics_id;
			critical_ = (info.critical) ? 1 : 0;
		}


		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		//这处理的是被攻击部队的经验值获得
		void 部队经验获得_战法致伤率(pk::unit@ unit, int type)
		{
			if (attacker_ is null) return;
			if (target_unit_id_arr.find(unit.get_id()) < 0) return;

			//部队变量储存
			@target_unit_ = @unit;
			//@target_leader_ = pk::get_person(unit.leader);
			//计算致伤率
			func_致伤执行();
			//部队变量清理
			target_unit_id_arr.removeAt(target_unit_id_arr.find(target_unit_.get_id()));

			if (int(target_unit_id_arr.length) == 0)
			{
				// 在被攻击目标的负伤全部处理完后，清空攻击者
				@attacker_ = null;
			}

		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		void func_致伤执行()
		{
			//pk::trace("pos1");
			for (int m = 0; m < 3; m++)
			{
				int n = 0;
				@def_member = pk::get_person(target_unit_.member[m]);
				if (def_member is null || !pk::is_alive(def_member)) continue;
				if (ch::has_skill(def_member, 特技_强运)) continue;
				//pk::trace(pk::format("武将id:{},致伤率:{}", def_member, func_最终致伤率(attacker_, attacker_leader, def_member, m, tactics_id_, critical_)));
				if (pk::rand_bool(func_最终致伤率(attacker_, attacker_leader, def_member, m, tactics_id_, critical_)) && def_member.shoubyou < 3)
				{
					ch::u8debug(2, pk::format("负伤！击伤概率{}%", func_最终致伤率(attacker_, attacker_leader, def_member, m, tactics_id_, critical_)));
					def_member.shoubyou = def_member.shoubyou + 1;
					def_member.update();
					if (开启受伤对话 && (attacker_.is_player() || def_member.is_player()))
					{
						pk::force@ force = pk::get_force(target_unit_.get_force_id());
						if (开启受伤对话) pk::history_log(target_unit_.pos, force.color, pk::encode(
							pk::format("\x1b[1x{}\x1b[0x在战斗中\x1b[27x{}\x1b[0x！",
								pk::decode(pk::get_name(def_member)), 受伤类型[def_member.shoubyou - 1])));
					}
					break;
				}
			}
		}
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
			if (def_member.stat[武将能力_武力] >= 90) n -= 3;
			else if (def_member.stat[武将能力_武力] >= 80) n -= 2;
			else if (def_member.stat[武将能力_武力] >= 70) n -= 1;
			if (def_member.stat[武将能力_智力] >= 90) n -= 2;
			else if (def_member.stat[武将能力_智力] >= 80) n -= 1;

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
			兵种战法致伤_启用[병기_검] = false;
			兵种战法致伤_启用[병기_창] = 枪兵战法致伤;
			兵种战法致伤_启用[병기_극] = 戟兵战法致伤;
			兵种战法致伤_启用[병기_노] = 弩兵战法致伤;
			兵种战法致伤_启用[병기_군마] = 骑兵战法致伤;
			兵种战法致伤_启用[병기_충차] = false;
			兵种战法致伤_启用[병기_정란] = 井栏战法致伤;
			兵种战法致伤_启用[병기_투석] = 投石战法致伤;
			兵种战法致伤_启用[병기_목수] = 木兽战法致伤;
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

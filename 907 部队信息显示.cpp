// ## 2021/11/03 # 江东新风 # 伤兵显示开关 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/26 # 江东新风 # 伤兵数字显示 ##
// ## 2021/10/11 # 江东新风 # 修正截粮不显示 ##
// ## 2021/01/09 # 江东新风 # 显示部队特技信息，添加半透明底色 ##
// ## 2020/08/14 # 氕氘氚 # 14:50 ##
// @@ 2020/08/13 @ 氕氘氚 @@

namespace 部队信息显示
{
	//---------------------------------------------------------------------------------------
	const bool 显示边匡 = false;

	//---------------------------------------------------------------------------------------

	class Main
	{

		Main()
		{
			// 当鼠标放在部队上时会在上方显示相关信息
			pk::bind(120, pk::trigger120_t(func_信息显示_部队信息));			

		}


		void func_信息显示_部队信息()
		{
			// 光标指的坐标
			pk::point cursor_pos = pk::get_cursor_hex_pos();
			if (!pk::is_valid_pos(cursor_pos)) return;


			// 光标上指示的部队
			pk::unit@ unit = pk::get_unit(cursor_pos);
			if (unit is null) return;

			bool is_player = unit.is_player();

			//


			// 显示信息
			string unit_name = pk::decode(pk::get_name(unit));

			string title = pk::format("部队信息(\x1b[1x{}\x1b[0x)", unit_name);
			unitinfo@ unit_t = @unit_ex[unit.get_id()];
			string info_wounded = pk::format("伤兵:\x1b[16x{}\x1b[0x", unit_t.wounded);

			string info_攻击力 = pk::format("攻击力: \x1b[2x{}\x1b[0x", unit.attr.stat[部队能力_攻击]);
			string info_防御力 = pk::format("防御力: \x1b[2x{}\x1b[0x", unit.attr.stat[部队能力_防御]);
			string info_智力 = pk::format("智力: \x1b[2x{}\x1b[0x", unit.attr.stat[部队能力_智力]);
			//特技显示,为了能画底色，所以将参数计算上移
			string info_部队特技信息 = get_unit_skill_name(unit);
			int 基础光环范围 = halo::func_光环范围(unit, 0);

			int middle = int(pk::get_resolution().width) / 2;
			int left = middle - 200;
			int right = middle + 200;
			int top = 5;
			int bottom = top + 80;

			pk::point leftdown;
			if (基础光环范围 > 0) leftdown = pk::point(left + 380, top + 125 + skill_line * 20);
			else leftdown = pk::point(left + 380, top + 60 + skill_line * 20);
			pk::draw_filled_rect(pk::rectangle(pk::point(left + 5, top + 5), leftdown), ((0xff / 2) << 24) | 0x777777);
			// pk::draw_rect(pk::rectangle(left, top, right, bottom), 0xff00ccff);
			pk::draw_text(pk::encode(title), pk::point(left + 5, top + 5), 0xffffffff, FONT_BIG, 0xff000000);
			if (开启伤兵系统) pk::draw_text(pk::encode(info_wounded), pk::point(left + 270, top + 13), 0xffffffff, FONT_SMALL, 0xff000000);
			pk::draw_text(pk::encode(info_攻击力), pk::point(left + 10, top + 40), 0xffffffff, FONT_SMALL, 0xff000000);
			pk::draw_text(pk::encode(info_防御力), pk::point(left + 140, top + 40), 0xffffffff, FONT_SMALL, 0xff000000);
			pk::draw_text(pk::encode(info_智力), pk::point(left + 270, top + 40), 0xffffffff, FONT_SMALL, 0xff000000);



			if (基础光环范围 > 0)
			{
				int 战法暴击光环范围 = halo::func_光环范围(unit, 1);
				int 暴击伤害光环范围 = halo::func_光环范围(unit, 2);
				int 计策成功率光环范围 = halo::func_光环范围(unit, 3);
				int 计策识破率光环范围 = halo::func_光环范围(unit, 4);
				int 辅助攻击光环范围 = halo::func_光环范围(unit, 5);
				int 气力回复光环范围 = halo::func_光环范围(unit, 6);

				string info_光环范围 = (基础光环范围 > 0) ? pk::format("光环范围: \x1b[2x{}\x1b[0x", 基础光环范围) : "光环范围: \x1b[29x无\x1b[0x";
				string info_基础光环 = (基础光环范围 > 0) ? pk::format("基础光环: \x1b[2x+{}\x1b[0x", halo::func_光环效果(unit, 0)) : "基础光环: \x1b[29x无\x1b[0x";
				string info_战法暴击光环 = (战法暴击光环范围 > 0) ? pk::format("战法暴击: \x1b[2x+{}%\x1b[0x", halo::func_光环效果(unit, 1)) : "战法暴击: \x1b[29x无\x1b[0x";
				string info_暴击伤害光环 = (暴击伤害光环范围 > 0) ? pk::format("暴击伤害: \x1b[2x+{}%\x1b[0x", halo::func_光环效果(unit, 2)) : "暴击伤害: \x1b[29x无\x1b[0x";
				string info_计策成功率光环 = (计策成功率光环范围 > 0) ? pk::format("计策成功: \x1b[2x+{}%\x1b[0x", halo::func_光环效果(unit, 3)) : "计策成功: \x1b[29x无\x1b[0x";
				string info_计策识破率光环 = (计策识破率光环范围 > 0) ? pk::format("计策识破: \x1b[2x+{}%\x1b[0x", halo::func_光环效果(unit, 4)) : "计策识破: \x1b[29x无\x1b[0x";
				string info_辅助攻击光环 = (辅助攻击光环范围 > 0) ? pk::format("辅助攻击: \x1b[2x+{}%\x1b[0x", halo::func_光环效果(unit, 5)) : "辅助攻击: \x1b[29x无\x1b[0x";
				string info_气力回复光环 = (气力回复光环范围 > 0) ? pk::format("气力回复: \x1b[2x+{}\x1b[0x", halo::func_光环效果(unit, 6)) : "气力回复: \x1b[29x无\x1b[0x";


				pk::draw_text(pk::encode(info_光环范围), pk::point(left + 10, top + 65), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::draw_text(pk::encode(info_基础光环), pk::point(left + 140, top + 65), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::draw_text(pk::encode(info_战法暴击光环), pk::point(left + 10, top + 85), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::draw_text(pk::encode(info_暴击伤害光环), pk::point(left + 140, top + 85), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::draw_text(pk::encode(info_计策成功率光环), pk::point(left + 10, top + 105), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::draw_text(pk::encode(info_计策识破率光环), pk::point(left + 140, top + 105), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::draw_text(pk::encode(info_辅助攻击光环), pk::point(left + 270, top + 85), 0xffffffff, FONT_SMALL, 0xff000000);
				pk::draw_text(pk::encode(info_气力回复光环), pk::point(left + 270, top + 105), 0xffffffff, FONT_SMALL, 0xff000000);

			}

			//特技显示
			//string info_部队特技信息 = get_unit_skill_name(unit);
			pk::draw_text(info_部队特技信息, pk::point(left + 10, top + ((基础光环范围 > 0) ? 125 : 60)), 0xffffffff, FONT_SMALL, 0xff000000);


		}

		int skill_line;
		string get_unit_skill_name(pk::unit@ unit)
		{
			string name_list = pk::encode("部队特技: ");
			int n = 0;
			skill_line = 1;
			//单特技只到159
			for (int i = 0; i <= 159; i++)
			{
				if (ch::has_skill(unit, i))
				{
					n += 1;
					if (n == 8)
					{
						name_list += "\n          ";
						skill_line += 1;
						n = 1;
					}
					//pk::trace(pk::format("has skill: {}",i));
					name_list += pk::get_skill(i).name + ' ';
				}
			}
			if (n == 0) name_list += pk::encode("无");
			return name_list;
		}


	}

	Main main;
}
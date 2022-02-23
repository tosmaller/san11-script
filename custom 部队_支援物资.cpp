// ## 2021/04/11 # 白马叔叔 # 调整行动结束判定规则、农民兵削弱##
// ## 2021/03/22 # 白马叔叔 # 修正不补给和取消补给时不判定是否行动结束、新增补给损失设定##
// ## 2020/11/27 # messi # 搬运NAVER论坛masterpiecek的支援物资功能##
/*
	@제작 : masterpiecek
	@요청 : 엑시온
	@내용 : 부대가 인접한 부대에게 부대메뉴 실행을 통해 물자를 지원
*/

namespace 부대_물자지원
{

	/// ============================== 유저 설정 ==============================

		/// 메뉴구성 : 물자지원(상위메뉴) - 금지원, 병량지원(하위메뉴)
	const string 물자지원_메뉴_이름 = "支援物资";
	const string 물자지원_메뉴_단축키 = "";
	const string 금지원_메뉴_이름 = "资金支援";
	const string 금지원_메뉴_단축키 = "";
	const string 병량지원_메뉴_이름 = "兵粮支援";
	const string 병량지원_메뉴_단축키 = "";

	const bool 물자지원_대상부대_자세력한정 = false;	// 'false'로 설정 시 동맹세력 부대에게도 지원가능（false可向同支援物资）
	const bool 물자지원_실행부대_행동완료 = true;	// 'false'로 설정 시 지원실행 후에도 부대가 행동가능（设定" false "后部队仍可行动）
	const int 물자지원_소모값_기력 = 0;	// '0'으로 설정 시 기력소모 없이 실행가능气力消耗
	const int 물자지원_금지원_최대값 = 5000;	// 참고 : 부대의 보유 가능 최대 금은 '10000'参考:部队可拥有的最大金为“10000”
	const int 물자지원_병량지원_최대값 = 20000;	// 참고 : 부대의 보유 가능 최대 병량은 '50000'参考:部队最多可拥有兵粮50000'
	
	const bool 물자지원_loss = true;  //支援物资是否会损失。相比于运输部队支援效果差一些
	const int 물자지원_get_percent = 90; //支援物资最后所得百分比。设置90实际所得为90%

/// =======================================================================

	const int 지원항목_금 = 0, 지원항목_병량 = 1;

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		pk::unit@ src_unit;
		pk::point src_pos_;
		pk::point pos_0;
		string menu_cost_info = (물자지원_소모값_기력 > 0) ? "\x1b[1x" + 물자지원_소모값_기력 + "\x1b[0x士气值" : "无需\x1b[1x士气值\x1b[0x";
		string target_setting = 물자지원_대상부대_자세력한정 ? "" : "\x1b[1x同势力部队\x1b[0x";

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			pk::menu_item 물자지원;
			물자지원.menu = 1;
			물자지원.shortcut = 물자지원_메뉴_단축키;
			물자지원.init = cast<pk::unit_menu_item_init_t@>(function(unit, src_pos) { @main.src_unit = @unit; main.src_pos_ = src_pos; });
			물자지원.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return (main.src_unit).type == 部队类型_战斗; });
			물자지원.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode(물자지원_메뉴_이름); });
			물자지원.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("相邻的\x1b[1x己方部队\x1b[0x向" + main.target_setting + "支援物资."); });
			int 상위메뉴 = pk::add_menu_item(물자지원);

			pk::menu_item 금지원;
			금지원.menu = 상위메뉴;
			금지원.shortcut = 금지원_메뉴_단축키;
			금지원.is_enabled = cast<pk::menu_item_is_enabled_t@>(function() { return main.isEnabled(지원항목_금); });
			금지원.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode(금지원_메뉴_이름); });
			금지원.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return main.getDesc(지원항목_금); });
			금지원.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets_arr(지원항목_금); });
			//금지원.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.handler(지원항목_금, dst_pos); });
			금지원.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.handler(지원항목_금, dst_pos); });
			pk::add_menu_item(금지원);

			pk::menu_item 병량지원;
			병량지원.menu = 상위메뉴;
			병량지원.shortcut = 병량지원_메뉴_단축키;
			병량지원.is_enabled = cast<pk::menu_item_is_enabled_t@>(function() { return main.isEnabled(지원항목_병량); });
			병량지원.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode(병량지원_메뉴_이름); });
			병량지원.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return main.getDesc(지원항목_병량); });
			병량지원.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.get_targets_arr(지원항목_병량); });
			병량지원.handler = cast<pk::unit_menu_item_handler_t@>(function(dst_pos) { return main.handler(지원항목_병량, dst_pos); });
			pk::add_menu_item(병량지원);
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		bool isEnabled(int menu_index)
		{
			if (is_临时武将(src_unit))
				return false;
			
			if (get_src_supplies(menu_index) == 0)
				return false;

			if (int(get_targets_arr(지원항목_금).length) == 0)
				return false;

			if (물자지원_소모값_기력 > 0 and int(src_unit.energy) < 물자지원_소모값_기력)
				return false;

			return true;
		}
		string getDesc(int menu_index)
		{
			if (is_临时武将(src_unit))
				return pk::encode("此类部队无法提供物资支援.");
			
			if (get_src_supplies(menu_index) == 0)
				return pk::encode("部队中没有" + get_support_item(menu_index) + ".(" + menu_cost_info + ")");

			if (int(get_targets_arr(지원항목_병량).length) == 0)
				return pk::encode("没有" + target_setting + "在\x1b[1x己方部队\x1b[0x周围可提供" + get_support_item(menu_index) + "\x1b[0x.(" + menu_cost_info + ")");

			if (물자지원_소모값_기력 > 0 and int(src_unit.energy) < 물자지원_소모값_기력)
				return pk::encode("无需士气值.(" + menu_cost_info + ")");

			return pk::encode("相邻的\x1b[1x己方部队\x1b[0x向" + target_setting + "提供" + get_support_item(menu_index) + "物资支援.(" + menu_cost_info + ")");
		}

		pk::array<pk::point_int> get_targets_arr(int menu_index)
		{
			pos_0 = src_unit.pos;
			int point_type = (menu_index == 지원항목_금) ? /*초록색 화살표*/3 : /*주황색 화살표*/4;
			pk::array<pk::point_int> targets;
			array<pk::point> rings = pk::ring(src_pos_, 1);
			for (int pos_index = 0; pos_index < int(rings.length); pos_index++)
			{
				pk::point dst_pos = rings[pos_index];
				pk::unit@ dst_unit = pk::get_unit(dst_pos);
				if (!pk::is_alive(dst_unit)) continue;
				if ((menu_index == 지원항목_금) ? (dst_unit.gold == 10000) : (dst_unit.food == 50000)) continue;
				if (물자지원_대상부대_자세력한정 ? (src_unit.get_force_id() != dst_unit.get_force_id()) : pk::is_enemy(src_unit, dst_unit)) continue;

				targets.insertLast(pk::point_int(dst_pos, point_type));
			}

			return targets;
		}

		bool handler(int menu_index, pk::point dst_pos)
		{
			pk::unit@ dst_unit = pk::get_unit(dst_pos);
			string menu_desc = "请决定为" + get_name(dst_unit) + "支援" + get_support_item(menu_index) + "的物资数量.";
			menu_desc += "\n(支援最大物资值:\x1b[1x" + get_supply_max(menu_index) + "\x1b[0x" + (물자지원_loss ? ", 实际所得稍有减少)" : ")");
			pk::message_box(pk::encode(menu_desc));

			string numberpad_title = pk::encode("支援数量");
			int support_limit = pk::min(get_src_supplies(menu_index), get_supply_max(menu_index));
			pk::int_bool numberpad_value = pk::numberpad(numberpad_title, 1, support_limit, 1, cast<pk::numberpad_t@>(function(line, original_value, current_value) { return ""; }));
			/* if (!numberpad_value.second)
			{
				pk::set_pos(src_unit, pos_0);
				if (물자지원_실행부대_행동완료) src_unit.action_done = false;
				src_unit.update();
				return false;
			} */
			if (!numberpad_value.second)
			{
				if (물자지원_실행부대_행동완료 and pos_0 != src_pos_) src_unit.action_done = true;  //移动了
				return false;
			}
			
			pk::person@ src_leader = pk::get_person(src_unit.leader);
			pk::person@ dst_leader = pk::get_person(dst_unit.leader);
			int support_amount = numberpad_value.first;
			int get_amout = 물자지원_loss ? (numberpad_value.first * (물자지원_get_percent + int((pk::get_best_member_stat(src_unit, 2) - 100) / 5)) / 100) : numberpad_value.first;
			string confirm_desc = "是否确定为" + get_name(dst_unit) + "提供\x1b[1x" + support_amount + get_support_item(menu_index) + "\x1b[0x吗?";
			confirm_desc += "\n(" + menu_cost_info + (물자지원_소모값_기력 > 0 ? "消耗" : "") + (물자지원_loss ? ", 实际所得稍有减少)" : ")");
			int confirm_value = pk::choose(pk::encode(confirm_desc), { pk::encode("确定"), pk::encode("取消") });
			/* if (confirm_value == 1)
			{
				pk::set_pos(src_unit, pos_0);
				if (물자지원_실행부대_행동완료) src_unit.action_done = false;
				src_unit.update();
				return false;
			} */
			if (confirm_value == 1)
			{
				if (물자지원_실행부대_행동완료 and pos_0 != src_pos_) src_unit.action_done = true;  //移动了
				return false;
			}

			if (물자지원_소모값_기력 > 0) pk::add_energy(src_unit, -물자지원_소모값_기력, true);
			
			array<string> thanks_msg_arr =
			{
				get_name(src_leader) + "托您的福勉强活下来了.",
				"万分感谢," + get_name(src_leader) + "!",
				get_name(src_leader) + "真是雪中送炭啊!",
				"真是解了燃眉之急."
			};
			string thanks_msg = thanks_msg_arr[pk::rand(int(thanks_msg_arr.length))];
			if (pk::is_in_screen(dst_unit.pos)) pk::say(pk::encode(thanks_msg), dst_leader, dst_unit);
			if (pk::is_in_screen(src_unit.pos)) pk::say(pk::encode("事态紧急,不必在意损失!快!"), src_leader, src_unit);
			if (menu_index == 지원항목_금) { pk::add_gold(src_unit, -support_amount, true); pk::add_gold(dst_unit, get_amout, true); }
			else if (menu_index == 지원항목_병량) { pk::add_food(src_unit, -support_amount, true); pk::add_food(dst_unit, get_amout, true); }

			if (물자지원_실행부대_행동완료) src_unit.action_done = true;

			return true;
		}
		
		//部队是否是农民、突袭者、雇佣兵
		bool is_临时武将(pk::unit@ unit_selected)
		{
			pk::person@ person = pk::get_person(unit_selected.leader);
			if (pk::is_alive(person))
			{
				if (person.name_read == pk::encode("农民兵"))
					return true;
				if (person.name_read == pk::encode("突袭者"))
					return true;
				if (person.name_read == pk::encode("雇佣兵"))
					return true;
			}
			return false;
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		string get_name(pk::unit@ unit) { return "\x1b[1x" + pk::decode(pk::get_name(unit)) + "\x1b[0x"; }
		string get_name(pk::person@ person) { return "\x1b[1x" + pk::decode(pk::get_name(person)) + "\x1b[0x"; }
		int get_src_supplies(int menu_index) { return (menu_index == 지원항목_금) ? src_unit.gold : src_unit.food; }
		string get_support_item(int menu_index) { return (menu_index == 지원항목_금) ? "\x1b[1x资金\x1b[0x" : "\x1b[1x兵粮\x1b[0x"; }
		int get_supply_max(int menu_index) { return (menu_index == 지원항목_금) ? 물자지원_금지원_최대값 : 물자지원_병량지원_최대값; }

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	}

	Main main;
}
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/02/02 # 江东新风 # 购买价格上调 ##
// ## 2021/04/18 # 白马叔叔 # AI兵装购买、优化算法、调整关下城购买兵装 ##																								
// ## 2021/01/31 # 江东新风 # rand函数错误修复 ##
// ## 2021/01/12 # 江东新风 # 加入特色都市设定，价格微调 ##
// ## 2021/01/07 # 江东新风 # 兵装买卖重做,商才价格优待#
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/10 #messi#全篇校对語句##
// ## 2020/08/09 # 氕氘氚 # 添加港?的兵器买卖，把菜单放到【买卖】下，修复金大於65535时计算出錯的bug##
// ## 2020/08/08 # 氕氘氚 # 把菜单放到【城市】下##
/*
@제작자: HoneyBee
@설명: 창,극,노,군마를 매매 (태수의 정치력이 관여)

*/

namespace 买卖指令_兵装买卖
{


	// ================ CUSTOMIZE ================


	const bool 枪_购买_启用 = true;   // true = on, false = off
	const bool 戟_购买_启用 = true;   // true = on, false = off
	const bool 弩_购买_启用 = true;   // true = on, false = off
	const bool 战马_购买_启用 = true;  // true = on, false = off

	const bool 枪_贩卖_启用 = true;   // true = on, false = off
	const bool 戟_贩卖_启用 = true;   // true = on, false = off
	const bool 弩_贩卖_启用 = true;   // true = on, false = off
	const bool 战马_贩卖_启用 = true;  // true = on, false = off

	const int TP_COST = 30; 	   // (기교P 필요량: 기본 30 설정)
	const int ACTION_COST = 50;    // (행동력 필요량: 기본 50 설정)


	const int KEY = pk::hash("兵装买卖");

	const bool AI兵装购买开启 = false;    //AI兵装购买开关。true为开，false为关。
	const int AI兵装购买概率 = 15;      //AI兵装购买概率，默认15
	const int AI兵装购买数 = 8000;       //AI兵装购买数，默认8000
	const int AI兵装购买金 = 1600;       //AI兵装购买兵装所需金, 默认1600															  

	/*

		(1) 창,극,노 (능리,부호,논객) 특기 보유 시 1.5배 획득량 증가
		(2) 군마 (번식,부호,논객) 특기 보유 시 1.5배 획득량 증가

	*/


	// ===========================================


	class Main
	{

		pk::building@ building_;
		pk::force@ force_;
		pk::person@ taishu_;
		pk::city@ city_;
		pk::gate@ gate_;
		pk::port@ port_;
		int building_gold;

		// 건물 메뉴 도시
		int menu_city_id_;

		// 건물 메뉴 세력
		int menu_force_id_;

		Main()
		{

			pk::menu_item item_兵装购买;
			item_兵装购买.menu = global_menu::菜单_买卖;
			// item_兵装购买.pos = 8;
			item_兵装购买.init = pk::building_menu_item_init_t(init);
			item_兵装购买.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
			item_兵装购买.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("兵装购买"); });
			item_兵装购买.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("点击展开或收起"); });
			item_兵装购买.handler = pk::menu_item_handler_t(handler_兵装购买);
			pk::add_menu_item(item_兵装购买);

			pk::menu_item item;

			pk::bind(107, pk::trigger107_t(callback));
			if (枪_购买_启用)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_枪购买);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_枪_购买);
				item.get_text = pk::menu_item_get_text_t(getText_枪_购买);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_枪_购买);
				item.handler = pk::menu_item_handler_t(handler_枪_购买);
				pk::add_menu_item(item);
			}

			if (戟_购买_启用)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_戟购买);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_戟_购买);
				item.get_text = pk::menu_item_get_text_t(getText_戟_购买);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_戟_购买);
				item.handler = pk::menu_item_handler_t(handler_戟_购买);
				pk::add_menu_item(item);
			}

			if (弩_购买_启用)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_弩购买);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_弩_购买);
				item.get_text = pk::menu_item_get_text_t(getText_弩_购买);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_弩_购买);
				item.handler = pk::menu_item_handler_t(handler_弩_购买);
				pk::add_menu_item(item);
			}

			if (战马_购买_启用)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_马购买);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_战马_购买);
				item.get_text = pk::menu_item_get_text_t(getText_战马_购买);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_战马_购买);
				item.handler = pk::menu_item_handler_t(handler_战马_购买);
				pk::add_menu_item(item);
			}


			pk::menu_item item_兵装贩卖;
			item_兵装贩卖.menu = global_menu::菜单_买卖;
			// item_兵装贩卖.pos = 8;
			item_兵装贩卖.init = pk::building_menu_item_init_t(init);
			item_兵装贩卖.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
			item_兵装贩卖.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("兵装贩卖"); });
			item_兵装贩卖.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("点击展开或收起"); });
			item_兵装贩卖.handler = pk::menu_item_handler_t(handler_兵装贩卖);
			pk::add_menu_item(item_兵装贩卖);


			if (枪_贩卖_启用)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_枪贩卖);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_枪_贩卖);
				item.get_text = pk::menu_item_get_text_t(getText_枪_贩卖);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_枪_贩卖);
				item.handler = pk::menu_item_handler_t(handler_枪_贩卖);
				pk::add_menu_item(item);
			}

			if (戟_贩卖_启用)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_戟贩卖);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_戟_贩卖);
				item.get_text = pk::menu_item_get_text_t(getText_戟_贩卖);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_戟_贩卖);
				item.handler = pk::menu_item_handler_t(handler_戟_贩卖);
				pk::add_menu_item(item);
			}

			if (弩_贩卖_启用)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_弩贩卖);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_弩_贩卖);
				item.get_text = pk::menu_item_get_text_t(getText_弩_贩卖);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_弩_贩卖);
				item.handler = pk::menu_item_handler_t(handler_弩_贩卖);
				pk::add_menu_item(item);
			}

			if (战马_贩卖_启用)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_马贩卖);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_战马_贩卖);
				item.get_text = pk::menu_item_get_text_t(getText_战马_贩卖);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_战马_贩卖);
				item.handler = pk::menu_item_handler_t(handler_战马_贩卖);
				pk::add_menu_item(item);
			}

		}

		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@taishu_ = pk::get_person(pk::get_taishu_id(building));
			if ((building_.get_id() <= 41) and (building_.get_id() >= 0))
				@city_ = pk::building_to_city(building);
			else if ((building_.get_id() <= 51) and (building_.get_id() >= 42))
				@gate_ = pk::building_to_gate(building);
			else if ((building_.get_id() <= 86) and (building_.get_id() >= 52))
				@port_ = pk::building_to_port(building);
			building_gold = pk::get_gold(building);


			menu_city_id_ = -1;

			if (city_ !is null)
				menu_city_id_ = city_.get_id();
			menu_force_id_ = building.get_force_id();
		}


		int getImageID_1249()   // 주황색
		{
			return 1249;
		}



		// 불필요한 건물에서의 메뉴 비활성화 조건 추가
		bool isMenuVisible()
		{
			if (building_.get_id() >= 据点_末) return false;
			return /*menu_city_id_ != -1 and*/ menu_force_id_ == pk::get_current_turn_force_id();
		}


		bool isMenuVisible_枪购买()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵装购买) return false;
			return true;
		}

		bool isMenuVisible_戟购买()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵装购买) return false;
			return true;
		}

		bool isMenuVisible_弩购买()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵装购买) return false;
			return true;
		}

		bool isMenuVisible_马购买()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵装购买) return false;
			return true;
		}

		bool isMenuVisible_枪贩卖()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵装贩卖) return false;
			return true;
		}

		bool isMenuVisible_戟贩卖()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵装贩卖) return false;
			return true;
		}

		bool isMenuVisible_弩贩卖()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵装贩卖) return false;
			return true;
		}

		bool isMenuVisible_马贩卖()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵装贩卖) return false;
			return true;
		}


		bool handler_兵装购买()
		{
			global_menu::买卖_展开_当前 = (global_menu::买卖_展开_当前 == global_menu::买卖_展开_兵装购买) ? 0 : global_menu::买卖_展开_兵装购买;
			return false;
		}


		bool handler_兵装贩卖()
		{
			global_menu::买卖_展开_当前 = (global_menu::买卖_展开_当前 == global_menu::买卖_展开_兵装贩卖) ? 0 : global_menu::买卖_展开_兵装贩卖;
			return false;
		}

		//---------------------------------------------------------------------------
		// 总函数
		//---------------------------------------------------------------------------


		bool buy_weapon(int weapon_id)
		{
			string weapon_name = ch::get_weapon_name(weapon_id);
			if (pk::choose(pk::encode(pk::format("是否从商人那里购买{}?", weapon_name)), { pk::encode("  是  "), pk::encode("  否  ") }) == 1)
				return false;
			// 执行武将选择
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building_);

			person_list.sort(function(a, b)
			{
				return (a.stat[武将能力_智力] > b.stat[武将能力_智力]); // 무장 정렬 (지력순)
			});
			if (person_list.count == 0) return false;

			// 실행무장 선택하기
			pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("交易武将选择"), pk::encode("选择执行兵器购买的武将."), person_list, 1, 1);
			if (person_sel.count == 0) return false; // 未选擇时取消结束
			pk::person@actor = person_sel[0];

			string actor_name = pk::decode(pk::get_name(actor));
			pk::person@ merchant = pk::get_person(무장_상인);

			//系数计算，调整只需处理price_max和price_min, 按价格差来最后浮动过于变态，还是按数量差
			float weapon_num_max = 8000.f;
			float weapon_num_min = 1400.f;
			//float price_min = 0.0875f;
			float per_stat_buf = (weapon_num_max - weapon_num_min) * 0.35f * 1.f / 70.f;
			float per_store_buf = (weapon_num_max - weapon_num_min) * 0.35f;
			float per_skill_buf = (weapon_num_max - weapon_num_min) * 0.15f;
			float per_rate_buf = (weapon_num_max - weapon_num_min) * 0.15f * 1.f / 40.f;
			float per_specialty_buf = (weapon_num_max - weapon_num_min) * 0.2f;

			//买武器，库存越多越便宜。智力越高，有特技，金粮比高有优势
			//非特产城市无buf，特产都市为0.15的价差，特产都市所属地据点为都市的一半
			float specialty_buf = 0.0f;
			if (building_.facility == 0)
			{
				int weapon_heishu = pk::equipment_id_to_heishu(weapon_id);
				if (pk::building_to_city(building_).tokusan[weapon_heishu]) specialty_buf = per_specialty_buf;
			}
			else if (building_.facility == 1 or building_.facility == 2)
			{

				int weapon_heishu = pk::equipment_id_to_heishu(weapon_id);
				if (pk::get_city(pk::get_city_id(building_.pos)).tokusan[weapon_heishu]) specialty_buf = 0.5f * per_specialty_buf;
			}
			float stat_buf = per_stat_buf * (pk::max(0, actor.stat[武将能力_智力] - 50));
			float store_buf = per_store_buf * pk::get_weapon_amount(building_, weapon_id) / pk::get_max_weapon_amount(building_, weapon_id);
			float skill_buf = (ch::has_skill(actor, 特技_商才) ? per_skill_buf : 0.0f);
			float rate_buf = (pk::get_city(pk::get_city_id(building_.pos)).rate - 30) * per_rate_buf;
			//每件武器的价格
			float muti = 1000.f / (weapon_num_min + (stat_buf + store_buf + skill_buf + rate_buf) + specialty_buf/*特产buf是特殊的*/);//调整基础价格，

			int PER_GOLD_COST = int(1000 * muti);

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x阁下运气不错,我这正好有一批不错的{}。\n每千份{}算你\x1b[2x{}\x1b[0x金，阁下要多少呢？", actor_name, weapon_name, weapon_name, PER_GOLD_COST)), merchant);  //마침 좋은 병기가 재고로 남아있었는데 \x1b[1x{}\x1b[0x님께서는 운이 좋으시군요.
			int count_max = pk::min(int(pk::get_gold(building_) / muti), 10000 + 5000 * pk::rand(3), pk::get_max_weapon_amount(building_, weapon_id) - pk::get_weapon_amount(building_, weapon_id));
			int count = 1;
			pk::int_bool deal0;
			deal0 = pk::numberpad(pk::encode("购买数量"), 1, count_max, 1, pk::numberpad_t(numberpad_t));
			if (!deal0.second) return false;
			count = deal0.first;

			int GOLD_COST = pk::max(1, int(muti * count));
			//int weapon_ = 1;

			//pk::message_box(pk::encode(pk::format("城内{}库存增加了\x1b[1x{}\x1b[0x个.", weapon_name, count))); //병기 (충차)가 \x1b[1x{}\x1b[0x만큼 증가했습니다.
			pk::message_box(pk::encode(pk::format("要购买{}件{}吗?价格算你{}吧。", count, weapon_name, GOLD_COST)), merchant);  //마침 좋은 병기가 재고로 남아있었는데 \x1b[1x{}\x1b[0x님께서는 운이 좋으시군요.

			pk::add_weapon_amount(building_, weapon_id, count, true);


			// 기교 및 금 감소
			//pk::message_box(pk::encode(pk::format("技巧P减少了\x1b[1x{}\x1b[0x.", TP_COST))); //기교 포인트가 \x1b[1x{}\x1b[0x만큼 감소했습니다.
			ch::add_tp(force_, -TP_COST, force_.get_pos());
			//pk::message_box(pk::encode(pk::format("资金减少了\x1b[1x{}\x1b[0x.", GOLD_COST)));  //도시의 금이 \x1b[1x{}\x1b[0x만큼 감소했습니다.
			pk::add_gold(building_, -GOLD_COST, true);

			// 행동력 감소.
			actor.action_done = true;
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);
			return true;
		}

		bool sale_weapon(int weapon_id)
		{

			string weapon_name = ch::get_weapon_name(weapon_id);
			if (pk::choose(pk::encode(pk::format("是否要将一些城内库存的{}卖给商人?", weapon_name)), { pk::encode("  是  "), pk::encode("  否  ") }) == 1)
				return false;
			// 执行武将选择
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building_);

			person_list.sort(function(a, b)
			{
				return (a.stat[武将能力_智力] > b.stat[武将能力_智力]); // 무장 정렬 (지력순)
			});
			if (person_list.count == 0) return false;

			// 실행무장 선택하기
			pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("交易武将选择"), pk::encode("选择执行兵器购买的武将."), person_list, 1, 1);
			if (person_sel.count == 0) return false; // 未选擇时取消结束
			pk::person@actor = person_sel[0];

			string actor_name = pk::decode(pk::get_name(actor));
			pk::person@ merchant = pk::get_person(무장_상인);

			//系数计算，调整只需处理price_max和price_min
			float weapon_num_max = 10000.f;
			float weapon_num_min = 4000.f;
			float per_stat_buf = (weapon_num_max - weapon_num_min) * 0.35f * 1.f / 70.f;
			float per_store_buf = (weapon_num_max - weapon_num_min) * 0.35f;
			float per_skill_buf = (weapon_num_max - weapon_num_min) * 0.15f;
			float per_rate_buf = (weapon_num_max - weapon_num_min) * 0.15f * 1.f / 40.f;
			float per_specialty_buf = (weapon_num_max - weapon_num_min) * 0.2f;

			//卖武器，库存越少越贵。智力越高，有特技，金粮比高有优势
			//非特产城市无buf，特产都市为0.15的价差，特产都市所属地据点为都市的一半
			float specialty_buf = 0.0f;
			if (building_.facility == 0)
			{
				int weapon_heishu = pk::equipment_id_to_heishu(weapon_id);
				if (pk::building_to_city(building_).tokusan[weapon_heishu]) specialty_buf = per_specialty_buf;
			}
			else if (building_.facility == 1 or building_.facility == 2)
			{

				int weapon_heishu = pk::equipment_id_to_heishu(weapon_id);
				if (pk::get_city(pk::get_city_id(building_.pos)).tokusan[weapon_heishu]) specialty_buf = 0.5f * per_specialty_buf;
			}
			float stat_buf = per_stat_buf * (pk::max(0, actor.stat[武将能力_智力] - 50));
			float store_buf = per_store_buf * pk::get_weapon_amount(building_, weapon_id) / pk::get_max_weapon_amount(building_, weapon_id);
			float skill_buf = (ch::has_skill(actor, 特技_商才) ? per_skill_buf : 0.0f);
			float rate_buf = (pk::get_city(pk::get_city_id(building_.pos)).rate - 30) * per_rate_buf;
			//pk::trace(pk::format("weapon:{},specialty_buf:{}",weapon_id, specialty_buf));
			float multi = get_weapon_cost(weapon_id) / (weapon_num_max - (stat_buf + store_buf + skill_buf + rate_buf) + specialty_buf/*特产buf是特殊的*/);
			//每1000份价格 
			int PER_GOLD_GAIN = int(1000 * multi);


			pk::message_box(pk::encode(pk::format("真巧呀，\x1b[1x{}\x1b[0x阁下。我正准备进一点{}。\n每千份{}我可以出\x1b[2x{}\x1b[0x金，阁下要卖多少呢？", actor_name, weapon_name, weapon_name, PER_GOLD_GAIN)), merchant);  //마침 좋은 병기가 재고로 남아있었는데 \x1b[1x{}\x1b[0x님께서는 운이 좋으시군요.
			//最大贩卖量取城市武器量，最大限制量和贩卖数量不超过城市最大容纳金
			int count_max = pk::min(pk::get_weapon_amount(building_, weapon_id), 40000, int((pk::get_max_gold(building_) - pk::get_gold(building_)) / multi));
			int count = 1;
			pk::int_bool deal0;
			deal0 = pk::numberpad(pk::encode("出售数量"), 1, count_max, 1, pk::numberpad_t(numberpad_t));
			if (!deal0.second) return false;
			count = deal0.first;

			int GOLD_GAIN = int(multi * count);

			// 기교 및 금 감소
			pk::message_box(pk::encode(pk::format("要出售{}件{}吗?价格算你{}吧。", count, weapon_name, GOLD_GAIN)), merchant);  //마침 좋은 병기가 재고로 남아있었는데 \x1b[1x{}\x1b[0x님께서는 운이 좋으시군요.

			//pk::message_box(pk::encode(pk::format("据点金增加了\x1b[1x{}\x1b[0x.", GOLD_GAIN))); //기교 포인트가 \x1b[1x{}\x1b[0x만큼 감소했습니다.
			pk::add_gold(building_, GOLD_GAIN, true);
			//pk::message_box(pk::encode(pk::format("城内\x1b[2x{}\x1b[0x库存减少了\x1b[1x{}\x1b[0x个.", weapon_name, count)));  //도시의 금이 \x1b[1x{}\x1b[0x만큼 감소했습니다.
			pk::add_weapon_amount(building_, weapon_id, -count, true);

			// 행동력 감소.
			actor.action_done = true;
			auto district = pk::get_district(pk::get_district_id(force_, 1));
			pk::add_ap(district, -ACTION_COST);
			return true;
		}

		string numberpad_t(int line, int original_value, int current_value)
		{
			return pk::encode("");
		}

		bool isEnabled_buy_weapon(int weapon_id)
		{
			//int GOLD_COST = int(get_weapon_cost(weapon_id) * GOLD_COST_muti);
			if (pk::get_idle_person_list(building_).count == 0) return false;
			else if (force_.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			else if (building_gold < 1) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		string getDesc_buy_weapon(int weapon_id)
		{
			//int GOLD_COST = int(get_weapon_cost(weapon_id) * GOLD_COST_muti);
			string weapon_name = ch::get_weapon_name(weapon_id);
			if (pk::get_idle_person_list(building_).count == 0)
				return pk::encode("没有可执行的武将。");
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));   //기교P가 부족합니다
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));  //행동력이 부족합니다
			else if (building_gold < 1)
				return pk::encode("城市资金不足");  //금이 부족합니다 
			else
				return pk::encode(pk::format("购买{}.(技巧P {}, 行动力 {})", weapon_name, TP_COST, ACTION_COST));  //충차를 구매합니다
		}

		bool isEnabled_sale_weapon(int weapon_id)
		{
			if (pk::get_idle_person_list(building_).count == 0) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			else if (pk::get_weapon_amount(building_, weapon_id) < 1) return false;
			return true;
		}

		string getDesc_sale_weapon(int weapon_id)
		{
			string weapon_name = ch::get_weapon_name(weapon_id);
			if (pk::get_idle_person_list(building_).count == 0)
				return pk::encode("没有可执行的武将。");
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));
			else if (pk::get_weapon_amount(building_, weapon_id) < 1)
				return pk::encode(pk::format("据点没有{}。", weapon_name));
			else
				return pk::encode(pk::format("贩卖{}。(技巧P {}, 行动力 {})", weapon_name, TP_COST, ACTION_COST));
		}

		//---------------------------------------------------------------------------
		// 枪_购买
		//---------------------------------------------------------------------------

		bool isEnabled_枪_购买()
		{
			return isEnabled_buy_weapon(兵器_枪);
		}

		string getText_枪_购买()
		{
			return pk::encode("购买枪");  //mai qiang
		}

		string getDesc_枪_购买()
		{
			return getDesc_buy_weapon(兵器_枪);
		}

		bool handler_枪_购买()
		{
			return buy_weapon(兵器_枪);
		}


		//---------------------------------------------------------------------------
		// 戟_购买
		//---------------------------------------------------------------------------

		bool isEnabled_戟_购买()
		{
			return isEnabled_buy_weapon(兵器_戟);
		}

		string getText_戟_购买()
		{
			return pk::encode("购买戟");  //ji goumai
		}

		string getDesc_戟_购买()
		{
			return getDesc_buy_weapon(兵器_戟);
		}

		bool handler_戟_购买()
		{
			return buy_weapon(兵器_戟);
		}


		//---------------------------------------------------------------------------
		// 弩_购买
		//---------------------------------------------------------------------------

		bool isEnabled_弩_购买()
		{
			return isEnabled_buy_weapon(兵器_弩);
		}

		string getText_弩_购买()
		{
			return pk::encode("购买弩");  //nu goumai
		}

		string getDesc_弩_购买()
		{
			return getDesc_buy_weapon(兵器_弩);
		}

		bool handler_弩_购买()
		{
			return buy_weapon(兵器_弩);
		}


		//---------------------------------------------------------------------------
		// 战马_购买
		//---------------------------------------------------------------------------

		bool isEnabled_战马_购买()
		{
			return isEnabled_buy_weapon(兵器_战马);
		}

		string getText_战马_购买()
		{
			return pk::encode("购买军马");  //junma goumai
		}

		string getDesc_战马_购买()
		{
			return getDesc_buy_weapon(兵器_战马);
		}

		bool handler_战马_购买()
		{
			return buy_weapon(兵器_战马);
		}


		//---------------------------------------------------------------------------
		// 枪_贩卖
		//---------------------------------------------------------------------------

		bool isEnabled_枪_贩卖()
		{
			return isEnabled_sale_weapon(兵器_枪);
		}

		string getText_枪_贩卖()
		{
			return pk::encode("贩卖枪");  //qiang fanmai
		}

		string getDesc_枪_贩卖()
		{
			return getDesc_sale_weapon(兵器_枪);
		}

		bool handler_枪_贩卖()
		{
			return sale_weapon(兵器_枪);
		}


		//---------------------------------------------------------------------------
		// 戟_贩卖
		//---------------------------------------------------------------------------

		bool isEnabled_戟_贩卖()
		{
			return isEnabled_sale_weapon(兵器_戟);
		}

		string getText_戟_贩卖()
		{
			return pk::encode("贩卖戟");
		}

		string getDesc_戟_贩卖()
		{
			return getDesc_sale_weapon(兵器_戟);
		}

		bool handler_戟_贩卖()
		{
			return sale_weapon(兵器_戟);
		}


		//---------------------------------------------------------------------------
		// 弩_贩卖
		//---------------------------------------------------------------------------

		bool isEnabled_弩_贩卖()
		{
			return isEnabled_sale_weapon(兵器_弩);
		}

		string getText_弩_贩卖()
		{
			return pk::encode("贩卖弩");  //nu fanmai
		}

		string getDesc_弩_贩卖()
		{
			return getDesc_sale_weapon(兵器_弩);
		}

		bool handler_弩_贩卖()
		{
			return sale_weapon(兵器_弩);
		}


		//---------------------------------------------------------------------------
		// 战马_贩卖
		//---------------------------------------------------------------------------

		bool isEnabled_战马_贩卖()
		{
			return isEnabled_sale_weapon(兵器_战马);
		}

		string getText_战马_贩卖()
		{
			return pk::encode("贩卖军马");
		}

		string getDesc_战马_贩卖()
		{
			return getDesc_sale_weapon(兵器_战马);
		}

		bool handler_战马_贩卖()
		{
			return sale_weapon(兵器_战马);
		}

		//---------------------------------------------------------------------------

		int get_weapon_cost(int weapon_id)
		{
			int weapon_cost;
			switch (weapon_id)
			{
			case 兵器_剑: weapon_cost = 0; break;
			case 兵器_枪: weapon_cost = 700; break;
			case 兵器_戟: weapon_cost = 700; break;
			case 兵器_弩: weapon_cost = 700; break;
			case 兵器_战马: weapon_cost = 700; break;
			case 兵器_冲车: weapon_cost = 1500; break;
			case 兵器_井阑: weapon_cost = 1600; break;
			case 兵器_投石: weapon_cost = 1800; break;
			case 兵器_木兽: weapon_cost = 1700; break;
			case 兵器_走舸: weapon_cost = 0; break;
			case 兵器_楼船: weapon_cost = 800; break;
			case 兵器_斗舰: weapon_cost = 800; break;
			default: weapon_cost = 0; break;
			}
			return weapon_cost;
		}
		
		//AI兵装购买事件
		void callback()
		{
			if (AI兵装购买开启)
				pk::scene(pk::scene_t(scene_AI_equipment));
		}
		
		//AI兵装购买功能
		void scene_AI_equipment()
		{
			for (int i = 0; i < 建筑_据点末; i++)
			{
				auto building_t = pk::get_building(i);
				
				if (building_t.is_player())
					continue;  //跳过玩家地盘
				if (!pk::enemies_around(building_t) and i < 建筑_城市末)
					continue;  //跳过非交战的城市（关、小城不跳过）
				if (pk::get_weapon_amount (building_t, 1) > 40000 and pk::get_weapon_amount (building_t, 2) > 40000 and pk::get_weapon_amount (building_t, 3) > 40000 and pk::get_weapon_amount (building_t, 4) > 40000 and i >= 建筑_城市末)
					continue;  //跳过兵装过多的关、小城
				
				if (building_t.get_force_id() == -1)
					continue;  //跳过空城无势力
				if (pk::get_taishu_id(building_t) == -1)
					continue;  //跳过无太守
				
				//int AI实际概率 = prob_equipment(building_t);
				if (!pk::rand_bool(AI兵装购买概率))
					continue;  //不触发
				if ((pk::get_force(building_t.get_force_id())).tp < TP_COST)
					continue;  //跳过没有足够技巧
				
				//最少的一种兵装，需要计算量，不如直接1-4随机好了，以下弃用
				int equipment_id = which_equipment_need(building_t);
				
				//int equipment_id = pk::rand(4) + 1;
				//pk::trace(pk::format("i等于{},equipment_id等于{}", i, equipment_id));
				
				pk::person@ taishu_0 = pk::get_taishu_id(building_t) != -1 ? pk::get_person(pk::get_taishu_id(building_t)) : pk::get_person(武将_文官);
				string taishu_name = pk::decode(pk::get_name(taishu_0));
				pk::person@ merchant = pk::get_person(무장_상인);
				string weapon_name = ch::get_weapon_name(equipment_id);

				if (pk::is_in_screen(building_t.pos)) pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x来不及生产了吗?先向商人买一些吧.", weapon_name)), taishu_0, building_t);
				if (pk::is_in_screen(building_t.pos)) pk::say(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,我这正好有一批不错的{}.", taishu_name, weapon_name)), merchant, building_t);
				
				if (i < 建筑_城市末)  //城市因为会自己生产，所以就设定为增加较少
				{
					pk::add_weapon_amount(building_t, equipment_id, AI兵装购买数, true);
					pk::add_gold(building_t, -AI兵装购买金, true);
				}
				else  //关、小城没有生产功能，所以就设定为增加较多
				{
					pk::add_weapon_amount(building_t, int(equipment_id * 1.5), AI兵装购买数, true);
					pk::add_gold(building_t, -int(AI兵装购买金 * 1.5), true);
				}
				
				ch::add_tp(pk::get_force(building_t.get_force_id()), -TP_COST, (pk::get_force(building_t.get_force_id())).get_pos());
				
				pk::history_log(building_t.pos, (pk::get_force(building_t.get_force_id())).color, pk::encode(pk::format("\x1b[1x{}\x1b[0x在城中采购\x1b[1x{}\x1b[0x.", taishu_name, weapon_name)));
			}
		}
		
		//获取城市哪种兵装数最少
		int which_equipment_need(pk::building@ building_t)
		{
			int less_id = 1;
			for (int i = 2; i < 5; i++)
			{
				if (pk::get_weapon_amount (building_t, i) < pk::get_weapon_amount (building_t, less_id))
					less_id = i;
			}
			return less_id;
		}
		
	}
	Main main;
}
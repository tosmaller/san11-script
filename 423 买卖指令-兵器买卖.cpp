// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/02/02 # 江东新风 # 购买价格上调 ##
// ## 2021/01/31 # 江东新风 # rand函数错误修复 ##
// ## 2021/01/14 # 江东新风 # 修复参数错误 ##
// ## 2021/01/12 # 江东新风 # 加入特色都市设定，价格微调 ##
// ## 2021/01/07 # 江东新风 # 兵器买卖重做,商才价格优待##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/09 # 氕氘氚 # 添加港?的兵器买卖，把菜单放到【买卖】下，修复金大於65535时计算出錯的bug##
// ## 2020/08/08 # 氕氘氚 #把菜单放到【城市】下##
// ## 2020/08/07 #messi#全篇校对語句##
/*
@제작자: HoneyBee
@설명: 충차,정란,투석,목수,누선,투함을 매매 (병기(함선)를 판매하여 금을 획득할 시 태수의 정치력이 관여)
准备改成购买时数量可选，特殊特技有折扣
*/

namespace 병기_매매
{


	// ================ CUSTOMIZE ================


	const bool 冲车_购买许可 = true;   // true = on, false = off
	const bool 井阑_购买许可 = true;   // true = on, false = off
	const bool 投石_购买许可 = true;   // true = on, false = off
	const bool 木兽_购买许可 = true;   // true = on, false = off
	const bool 楼船_购买许可 = true;   // true = on, false = off
	const bool 斗舰_购买许可 = true;   // true = on, false = off

	const bool 冲车_贩卖许可 = true;   // true = on, false = off
	const bool 井阑_贩卖许可 = true;   // true = on, false = off
	const bool 投石_贩卖许可 = true;   // true = on, false = off
	const bool 木兽_贩卖许可 = true;   // true = on, false = off
	const bool 楼船_贩卖许可 = true;   // true = on, false = off
	const bool 斗舰_贩卖许可 = true;   // true = on, false = off

	const int TP_COST = 30; 	    // (기교P 필요량: 기본 30 설정)
	const int ACTION_COST = 50;     // (행동력 필요량: 기본 50 설정)
	const float GOLD_COST_muti = 2.1f;     // (구매 시 금 필요량: 기본 2000 설정) 根据每个兵器价格调整

	const int KEY = pk::hash("兵器买卖");


	/*
		※ 병기 구매: (발명,부호,논객) 특기 보유 시 2배 병기 획득량 증가
		※ 병기 판매: (발명,부호,논객) 특기 보유 시 1.5배 금 획득량 증가
		※ 함선 구매: (조선,부호,논객) 특기 보유 시 2배 함선 획득량 증가
		※ 함선 판매: (조선,부호,논객) 특기 보유 시 1.5배 금 획득량 증가
	*/


	// ===========================================


	class Main
	{

		pk::building@ building_;
		pk::force@ force_;
		pk::person@ taishu_;
		int building_gold;


		// 건물 메뉴 세력
		int menu_force_id_;

		Main()
		{

			pk::menu_item item_兵器购买;
			item_兵器购买.menu = global_menu::菜单_买卖;
			// item_兵器购买.pos = 8;
			item_兵器购买.init = pk::building_menu_item_init_t(init);
			item_兵器购买.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
			item_兵器购买.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("兵器购买"); });
			item_兵器购买.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("点击展开或收起"); });
			item_兵器购买.handler = pk::menu_item_handler_t(handler_兵器购买);
			pk::add_menu_item(item_兵器购买);


			pk::menu_item item;
			if (冲车_购买许可)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_冲車购买);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_冲车_购买);
				item.get_text = pk::menu_item_get_text_t(getText_冲车_购买);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_冲车_购买);
				item.handler = pk::menu_item_handler_t(handler_冲车_购买);
				pk::add_menu_item(item);
			}

			if (井阑_购买许可)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_井栏购买);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_井阑_购买);
				item.get_text = pk::menu_item_get_text_t(getText_井阑_购买);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_井阑_购买);
				item.handler = pk::menu_item_handler_t(handler_井阑_购买);
				pk::add_menu_item(item);
			}

			if (投石_购买许可)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_投石购买);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_投石_购买);
				item.get_text = pk::menu_item_get_text_t(getText_投石_购买);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_投石_购买);
				item.handler = pk::menu_item_handler_t(handler_投石_购买);
				pk::add_menu_item(item);
			}

			if (木兽_购买许可)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_木兽购买);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_木兽_购买);
				item.get_text = pk::menu_item_get_text_t(getText_木兽_购买);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_木兽_购买);
				item.handler = pk::menu_item_handler_t(handler_木兽_购买);
				pk::add_menu_item(item);
			}

			if (楼船_购买许可)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_楼船购买);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_楼船_购买);
				item.get_text = pk::menu_item_get_text_t(getText_楼船_购买);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_楼船_购买);
				item.handler = pk::menu_item_handler_t(handler_楼船_购买);
				pk::add_menu_item(item);
			}

			if (斗舰_购买许可)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_斗船购买);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_斗舰_购买);
				item.get_text = pk::menu_item_get_text_t(getText_斗舰_购买);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_斗舰_购买);
				item.handler = pk::menu_item_handler_t(handler_斗舰_购买);
				pk::add_menu_item(item);
			}


			pk::menu_item item_兵器贩卖;
			item_兵器贩卖.menu = global_menu::菜单_买卖;
			// item_兵器贩卖.pos = 8;
			item_兵器贩卖.init = pk::building_menu_item_init_t(init);
			item_兵器贩卖.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
			item_兵器贩卖.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("兵器贩卖"); });
			item_兵器贩卖.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("点击展开或收起"); });
			item_兵器贩卖.handler = pk::menu_item_handler_t(handler_兵器贩卖);
			pk::add_menu_item(item_兵器贩卖);


			if (冲车_贩卖许可)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_冲車贩卖);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_冲车_贩卖);
				item.get_text = pk::menu_item_get_text_t(getText_冲车_贩卖);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_冲车_贩卖);
				item.handler = pk::menu_item_handler_t(handler_冲车_贩卖);
				pk::add_menu_item(item);
			}

			if (井阑_贩卖许可)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_井栏贩卖);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_井阑_贩卖);
				item.get_text = pk::menu_item_get_text_t(getText_井阑_贩卖);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_井阑_贩卖);
				item.handler = pk::menu_item_handler_t(handler_井阑_贩卖);
				pk::add_menu_item(item);
			}

			if (投石_贩卖许可)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_投石贩卖);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_投石_贩卖);
				item.get_text = pk::menu_item_get_text_t(getText_投石_贩卖);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_投石_贩卖);
				item.handler = pk::menu_item_handler_t(handler_投石_贩卖);
				pk::add_menu_item(item);
			}

			if (木兽_贩卖许可)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_木兽贩卖);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_木兽_贩卖);
				item.get_text = pk::menu_item_get_text_t(getText_木兽_贩卖);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_木兽_贩卖);
				item.handler = pk::menu_item_handler_t(handler_木兽_贩卖);
				pk::add_menu_item(item);
			}

			if (楼船_贩卖许可)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_楼船贩卖);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_楼船_贩卖);
				item.get_text = pk::menu_item_get_text_t(getText_楼船_贩卖);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_楼船_贩卖);
				item.handler = pk::menu_item_handler_t(handler_楼船_贩卖);
				pk::add_menu_item(item);
			}

			if (斗舰_贩卖许可)
			{
				item.menu = global_menu::菜单_买卖;
				item.init = pk::building_menu_item_init_t(init);
				item.is_visible = pk::menu_item_is_visible_t(isMenuVisible_斗船贩卖);
				item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_斗舰_贩卖);
				item.get_text = pk::menu_item_get_text_t(getText_斗舰_贩卖);
				item.get_desc = pk::menu_item_get_desc_t(getDesc_斗舰_贩卖);
				item.handler = pk::menu_item_handler_t(handler_斗舰_贩卖);
				pk::add_menu_item(item);
			}

		}

		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@taishu_ = pk::get_person(pk::get_taishu_id(building));


			building_gold = pk::get_gold(building);

			menu_force_id_ = building.get_force_id();
		}



		int getImageID_1249()   // 주황색
		{
			return 1249;
		}



		// 불필요한 건물에서의 메뉴 비활성화 조건 추가
		bool isMenuVisible()
		{
			return true;
			// if (building_.get_id() >= 据点_末) return false;
			// return /*menu_building_id_ != -1 and*/ menu_force_id_ == pk::get_current_turn_force_id();
		}

		// -------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------



		bool isMenuVisible_冲車购买()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵器购买) return false;
			if (pk::has_tech(force_, 기교_목수개발)) return false;
			return true;
		}

		bool isMenuVisible_井栏购买()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵器购买) return false;
			if (pk::has_tech(force_, 기교_투석개발)) return false;
			return true;
		}

		bool isMenuVisible_投石购买()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵器购买) return false;
			if (!pk::has_tech(force_, 기교_투석개발)) return false;
			return true;
		}

		bool isMenuVisible_木兽购买()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵器购买) return false;
			if (!pk::has_tech(force_, 기교_목수개발)) return false;
			return true;
		}

		bool isMenuVisible_楼船购买()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵器购买) return false;
			if (pk::has_tech(force_, 기교_투석개발)) return false;
			return true;
		}

		bool isMenuVisible_斗船购买()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵器购买) return false;
			if (!pk::has_tech(force_, 기교_투석개발)) return false;
			return true;
		}


		bool isMenuVisible_冲車贩卖()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵器贩卖) return false;
			if (pk::has_tech(force_, 기교_목수개발)) return false;
			return true;
		}

		bool isMenuVisible_井栏贩卖()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵器贩卖) return false;
			if (pk::has_tech(force_, 기교_투석개발)) return false;
			return true;
		}

		bool isMenuVisible_投石贩卖()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵器贩卖) return false;
			if (!pk::has_tech(force_, 기교_투석개발)) return false;
			return true;
		}

		bool isMenuVisible_木兽贩卖()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵器贩卖) return false;
			if (!pk::has_tech(force_, 기교_목수개발)) return false;
			return true;
		}

		bool isMenuVisible_楼船贩卖()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵器贩卖) return false;
			if (pk::has_tech(force_, 기교_투석개발)) return false;
			return true;
		}

		bool isMenuVisible_斗船贩卖()
		{
			if (global_menu::买卖_展开_当前 != global_menu::买卖_展开_兵器贩卖) return false;
			if (!pk::has_tech(force_, 기교_투석개발)) return false;
			return true;
		}







		bool handler_兵器购买()
		{
			global_menu::买卖_展开_当前 = (global_menu::买卖_展开_当前 == global_menu::买卖_展开_兵器购买) ? 0 : global_menu::买卖_展开_兵器购买;
			return false;
		}


		bool handler_兵器贩卖()
		{
			global_menu::买卖_展开_当前 = (global_menu::买卖_展开_当前 == global_menu::买卖_展开_兵器贩卖) ? 0 : global_menu::买卖_展开_兵器贩卖;
			return false;
		}




		// -------------------------------------------------------------------------------------------------------
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
			float cost_rate_max = 3.f;
			float cost_rate_min = 1.f;
			//float price_min = 0.0875f;
			float per_stat_buf = (cost_rate_max - cost_rate_min) * 0.35f * 1.f / 70.f;
			float per_store_buf = (cost_rate_max - cost_rate_min) * 0.35f;
			float per_skill_buf = (cost_rate_max - cost_rate_min) * 0.15f;
			float per_rate_buf = (cost_rate_max - cost_rate_min) * 0.15f * 1.f / 40.f;
			float per_specialty_buf = (cost_rate_max - cost_rate_min) * 0.2f;

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
			float store_buf = per_store_buf * pk::max(0, pk::min(15, 15 - pk::get_weapon_amount(building_, weapon_id))) / 15.f;
			float skill_buf = (ch::has_skill(actor, 特技_商才) ? per_skill_buf : 0.0f);
			float rate_buf = (pk::get_city(pk::get_city_id(building_.pos)).rate - 30) * per_rate_buf;
			//每件攻具的价格倍率
			float muti = cost_rate_max - (stat_buf + store_buf + skill_buf + rate_buf) - specialty_buf;
			int PER_GOLD_COST = int(get_weapon_cost(weapon_id)*1.5f * muti);

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x阁下运气不错,我这正好有一些{}。\n单价\x1b[2x{}\x1b[0x，阁下要多少呢？", actor_name, weapon_name, PER_GOLD_COST)), merchant);  //마침 좋은 병기가 재고로 남아있었는데 \x1b[1x{}\x1b[0x님께서는 운이 좋으시군요.
			int count_max = pk::min(pk::rand(3) + 2, pk::get_gold(building_) / PER_GOLD_COST, pk::get_max_weapon_amount(building_, weapon_id) - pk::get_weapon_amount(building_, weapon_id));
			int count = 1;
			pk::int_bool deal0;
			deal0 = pk::numberpad(pk::encode("购买数量"), 1, count_max, 1, pk::numberpad_t(numberpad_t));
			if (!deal0.second) return false;
			count = deal0.first;

			int GOLD_COST = PER_GOLD_COST * count;
			//int weapon_ = 1;

			// 태수가 아래 해당되는 특기를 보유 시 병기 획득량 2배 증가 (상인과의 거래를 유리하게 이끌어낸다는 설정)
			//if (taishu_.skill == 특기_발명 or taishu_.skill == 특기_부호 or taishu_.skill == 特技_论客)
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

			//系数计算，调整只需处理price_max和price_min, 按价格差来最后浮动过于变态，还是按数量差
			float cost_rate_max = 1.1f;
			float cost_rate_min = 0.3f;
			//float price_min = 0.0875f;
			float per_stat_buf = (cost_rate_max - cost_rate_min) * 0.35f * 1.f / 70.f;
			float per_store_buf = (cost_rate_max - cost_rate_min) * 0.35f;
			float per_skill_buf = (cost_rate_max - cost_rate_min) * 0.15f;
			float per_rate_buf = (cost_rate_max - cost_rate_min) * 0.15f * 1.f / 40.f;
			float per_specialty_buf = (cost_rate_max - cost_rate_min) * 0.2f;

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
			float store_buf = per_store_buf * pk::max(0, pk::min(10, 10 - pk::get_weapon_amount(building_, weapon_id))) / 10.f;
			float skill_buf = (ch::has_skill(actor, 特技_商才) ? per_skill_buf : 0.0f);
			float rate_buf = (pk::get_city(pk::get_city_id(building_.pos)).rate - 30) * per_rate_buf;
			//每件攻具的价格倍率
			float muti = cost_rate_min + (stat_buf + store_buf + skill_buf + rate_buf) - specialty_buf;
			int PER_GOLD_GAIN = int(get_weapon_cost(weapon_id) * muti);

			pk::message_box(pk::encode(pk::format("真巧呀，\x1b[1x{}\x1b[0x阁下。我正准备进一点{}。\n单价\x1b[2x{}\x1b[0x，阁下要卖多少呢？", actor_name, weapon_name, PER_GOLD_GAIN)), merchant);  //마침 좋은 병기가 재고로 남아있었는데 \x1b[1x{}\x1b[0x님께서는 운이 좋으시군요.
			int count_max = pk::min(pk::get_weapon_amount(building_, weapon_id), 5 + pk::rand(3), int(floor((pk::get_max_gold(building_) - pk::get_gold(building_)) / PER_GOLD_GAIN)));
			int count = 1;
			pk::int_bool deal0;
			deal0 = pk::numberpad(pk::encode("出售数量"), 1, count_max, 1, pk::numberpad_t(numberpad_t));
			if (!deal0.second) return false;
			count = deal0.first;

			int GOLD_GAIN = PER_GOLD_GAIN * count;
			//int weapon_ = 1;

			// 태수가 아래 해당되는 특기를 보유 시 병기 획득량 2배 증가 (상인과의 거래를 유리하게 이끌어낸다는 설정)
			//if (taishu_.skill == 특기_발명 or taishu_.skill == 특기_부호 or taishu_.skill == 特技_论客)

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
			int GOLD_COST = int(get_weapon_cost(weapon_id) * GOLD_COST_muti);
			if (pk::get_idle_person_list(building_).count == 0) return false;
			else if (force_.tp < TP_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			else if (building_gold < GOLD_COST) return false;
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;
			return true;
		}

		string getDesc_buy_weapon(int weapon_id)
		{
			int GOLD_COST = int(get_weapon_cost(weapon_id) * GOLD_COST_muti);
			string weapon_name = ch::get_weapon_name(weapon_id);
			if (pk::get_idle_person_list(building_).count == 0)
				return pk::encode("没有可执行的武将。");
			else if (force_.tp < TP_COST)
				return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST));   //기교P가 부족합니다
			else if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", ACTION_COST));  //행동력이 부족합니다
			else if (building_gold < GOLD_COST)
				return pk::encode(pk::format("资金不足 (必须 {} )", GOLD_COST));  //금이 부족합니다 
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
		// 冲车_购买
		//---------------------------------------------------------------------------

		bool isEnabled_冲车_购买()
		{
			return isEnabled_buy_weapon(兵器_冲车);
		}

		string getText_冲车_购买()
		{
			return pk::encode("购买冲车");  //chongche(충차)구매
		}

		string getDesc_冲车_购买()
		{
			return getDesc_buy_weapon(兵器_冲车);
		}

		bool handler_冲车_购买()
		{
			return buy_weapon(兵器_冲车);
		}


		//---------------------------------------------------------------------------
		// 井阑_购买
		//---------------------------------------------------------------------------

		bool isEnabled_井阑_购买()
		{
			return isEnabled_buy_weapon(兵器_井阑);
		}

		string getText_井阑_购买()
		{
			return pk::encode("购买井阑");  //jinglan(정란)구매
		}

		string getDesc_井阑_购买()
		{
			return getDesc_buy_weapon(兵器_井阑);
		}

		bool handler_井阑_购买()
		{

			return buy_weapon(兵器_井阑);

		}


		//---------------------------------------------------------------------------
		// 投石_购买
		//---------------------------------------------------------------------------

		bool isEnabled_投石_购买()
		{
			return isEnabled_buy_weapon(兵器_投石);
		}

		string getText_投石_购买()
		{
			return pk::encode("购买投石"); //toushi
		}

		string getDesc_投石_购买()
		{
			return getDesc_buy_weapon(兵器_投石);
		}

		bool handler_投石_购买()
		{
			return buy_weapon(兵器_投石);
		}


		//---------------------------------------------------------------------------
		// 木兽_购买
		//---------------------------------------------------------------------------

		bool isEnabled_木兽_购买()
		{
			return isEnabled_buy_weapon(兵器_木兽);
		}

		string getText_木兽_购买()
		{
			return pk::encode("购买木兽");  //mushou
		}

		string getDesc_木兽_购买()
		{
			return getDesc_buy_weapon(兵器_木兽);
		}

		bool handler_木兽_购买()
		{
			return buy_weapon(兵器_木兽);
		}


		//---------------------------------------------------------------------------
		// 楼船_购买
		//---------------------------------------------------------------------------

		bool isEnabled_楼船_购买()
		{
			return isEnabled_buy_weapon(兵器_楼船);
		}

		string getText_楼船_购买()
		{
			return pk::encode("购买楼船");  //louchuan
		}

		string getDesc_楼船_购买()
		{
			return getDesc_buy_weapon(兵器_楼船);
		}

		bool handler_楼船_购买()
		{
			return buy_weapon(兵器_楼船);
		}


		//---------------------------------------------------------------------------
		// 斗舰_购买
		//---------------------------------------------------------------------------

		bool isEnabled_斗舰_购买()
		{
			return isEnabled_buy_weapon(兵器_斗舰);
		}

		string getText_斗舰_购买()
		{
			return pk::encode("购买斗舰");  //doujian
		}

		string getDesc_斗舰_购买()
		{
			return getDesc_buy_weapon(兵器_斗舰);
		}

		bool handler_斗舰_购买()
		{
			return buy_weapon(兵器_斗舰);
		}


		//---------------------------------------------------------------------------
		// 冲车_贩卖
		//---------------------------------------------------------------------------

		bool isEnabled_冲车_贩卖()
		{
			return isEnabled_sale_weapon(兵器_冲车);
		}

		string getText_冲车_贩卖()
		{
			return pk::encode("贩卖冲车");  //mai chongche
		}

		string getDesc_冲车_贩卖()
		{
			return getDesc_sale_weapon(兵器_冲车);
		}

		bool handler_冲车_贩卖()
		{
			return sale_weapon(兵器_冲车);
		}


		//---------------------------------------------------------------------------
		// 井阑_贩卖
		//---------------------------------------------------------------------------

		bool isEnabled_井阑_贩卖()
		{
			return isEnabled_sale_weapon(兵器_井阑);
		}

		string getText_井阑_贩卖()
		{
			return pk::encode("贩卖井阑");
		}

		string getDesc_井阑_贩卖()
		{
			return getDesc_sale_weapon(兵器_井阑);
		}

		bool handler_井阑_贩卖()
		{

			return sale_weapon(兵器_井阑);

		}


		//---------------------------------------------------------------------------
		// 投石_贩卖
		//---------------------------------------------------------------------------

		bool isEnabled_投石_贩卖()
		{
			return isEnabled_sale_weapon(兵器_投石);
		}


		string getText_投石_贩卖()
		{
			return pk::encode("贩卖投石");  //mai toushi
		}

		string getDesc_投石_贩卖()
		{
			return getDesc_sale_weapon(兵器_投石);
		}

		bool handler_投石_贩卖()
		{
			return sale_weapon(兵器_投石);
		}


		//---------------------------------------------------------------------------
		// 木兽_贩卖
		//---------------------------------------------------------------------------

		bool isEnabled_木兽_贩卖()
		{
			return isEnabled_sale_weapon(兵器_木兽);
		}

		string getText_木兽_贩卖()
		{
			return pk::encode("贩卖木兽");
		}

		string getDesc_木兽_贩卖()
		{
			return getDesc_sale_weapon(兵器_木兽);
		}

		bool handler_木兽_贩卖()
		{

			return sale_weapon(兵器_木兽);

		}


		//---------------------------------------------------------------------------
		// 楼船_贩卖
		//---------------------------------------------------------------------------

		bool isEnabled_楼船_贩卖()
		{
			return isEnabled_sale_weapon(兵器_楼船);
		}


		string getText_楼船_贩卖()
		{
			return pk::encode("贩卖楼船");
		}

		string getDesc_楼船_贩卖()
		{
			return getDesc_sale_weapon(兵器_楼船);
		}

		bool handler_楼船_贩卖()
		{

			return sale_weapon(兵器_楼船);

		}


		//---------------------------------------------------------------------------
		// 斗舰_贩卖
		//---------------------------------------------------------------------------

		bool isEnabled_斗舰_贩卖()
		{
			return isEnabled_sale_weapon(兵器_斗舰);
		}


		string getText_斗舰_贩卖()
		{
			return pk::encode("贩卖斗舰");  //doujian
		}

		string getDesc_斗舰_贩卖()
		{
			return getDesc_sale_weapon(兵器_斗舰);
		}

		bool handler_斗舰_贩卖()
		{

			return sale_weapon(兵器_斗舰);

		}

		int get_weapon_cost(int weapon_id)
		{
			int weapon_cost;
			switch (weapon_id)
			{
<<<<<<< HEAD
			case 병기_검: weapon_cost = 0; break;
			case 병기_창: weapon_cost = 700; break;
			case 병기_극: weapon_cost = 700; break;
			case 병기_노: weapon_cost = 700; break;
			case 병기_군마: weapon_cost = 700; break;
			case 병기_충차: weapon_cost = 1500; break;
			case 병기_정란: weapon_cost = 1600; break;
			case 병기_투석: weapon_cost = 1800; break;
			case 병기_목수: weapon_cost = 1700; break;
=======
			case 兵器_剑: weapon_cost = 0; break;
			case 兵器_枪: weapon_cost = 700; break;
			case 兵器_戟: weapon_cost = 700; break;
			case 兵器_弩: weapon_cost = 700; break;
			case 兵器_战马: weapon_cost = 700; break;
			case 兵器_冲车: weapon_cost = 1500; break;
			case 兵器_井阑: weapon_cost = 1600; break;
			case 兵器_投石: weapon_cost = 1800; break;
			case 兵器_木兽: weapon_cost = 1700; break;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			case 兵器_走舸: weapon_cost = 0; break;
			case 兵器_楼船: weapon_cost = 800; break;
			case 兵器_斗舰: weapon_cost = 800; break;
			default: weapon_cost = 0; break;
			}
			return weapon_cost;
		}

	}

	Main main;
}
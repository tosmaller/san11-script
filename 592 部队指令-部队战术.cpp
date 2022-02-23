// ## 2021/10/01 # 江东新风 # 关闭大杀四方动画 ##
// ## 2021/09/26 # 江东新风 # 战术的动画效果测试，暂不更新，必须写到对应指令cpp下 ##
// ## 2021/01/01 # 江东新风 # 战神特技也能大杀四方 ##
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/08/07 # 氕氘氚 # 规则、代碼优化 ##
// ## 2020/08/05 # 江东新风 # 兵种不符时，无法发动对应特技 ##
// ## 2020/08/05 # messi # 重新设计台詞 ##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: 통솔, 무력이 동시에 높은 무장이 특정 특기를 소유한 경우에 활용할 수 있는 부대 메뉴


※ CHAMPION TYPE 부대의 효과

(1) 특기 <앙양>을 소유 시 주위에 있는 적군 부대 기력 흡수 (<앙양> 특기를 보유한 적군부대는 면역)
(2) 특기 <위풍>을 소유 시 주위에 있는 적군 부대 기력 감소 및 자신보다 무력이 낮은 적부대를 일정확률로 혼란 (<위풍> 특기를 보유한 적군부대는 면역)
(3) 특기 [창장 or 창신]을 소유한 창병부대 주위에 있는 적군 기병부대 병력 및 기력 감소 (단, 해당 특기 소유자의 무력 이상인 적군 무장의 부대는 면역)
(4) 특기 [극장 or 극신 or 친월 or 친만]를 소유한 극병부대 주위에 있는 적군 창병부대 병력 및 기력 감소 (단, 해당 특기 소유자의 무력 이상인 적군 무장의 부대는 면역)
(5) 특기 [질주 or 기장 or 기신 or 친강 or 친오]를 소유한 기병부대 주위에 있는 적군 극병부대 병력 및 기력 감소 (단, 해당 특기 소유자의 무력 이상인 적군 무장의 부대는 면역)
(6) 특기 [패왕 or 용장 or 맹장 or 신장 or 비장 or 투신]을 소유한 부대 주위에 있는
	적군부대 병력,기력 감소 및 자신보다 무력이 낮은 적부대를 일정확률로 혼란
   ([패왕 or 용장 or 맹장 or 신장 or 비장 or 투신] 특기를 보유한 적군부대는 면역)


*/

namespace 부대전술메뉴
{

	// ================ CUSTOMIZE ================


		//	全局设置-部队作战技能

	// ===========================================


	class Main
	{
		Main()
		{
			// 부대 메뉴 추가
			add_menu_unit_order();
		}

		pk::unit@ src_unit;
		pk::point src_pos_;
		pk::person@ src_leader;

		bool flag_기력흡수 = false;
		bool flag_기력감소 = false;
		bool flag_창병효과 = false;
		bool flag_극병효과 = false;
		bool flag_기병효과 = false;
		bool flag_용장계효과 = false;

		void add_menu_unit_order()
		{
			if (开启_部队战术_玩家)
			{
				pk::menu_item 부대;
				int 부대_상위;

				부대.menu = 1;
				부대.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("部队战术"); });
				부대.init = cast<pk::unit_menu_item_init_t@>(function(unit, src_pos) { @main.src_unit = @unit; main.src_pos_ = src_pos; });
				부대.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return (main.src_unit).type == 部队类型_战斗; });
				부대_상위 = pk::add_menu_item(부대);

				if (开启_昂扬奋战)
				{
					pk::menu_item 앙양전술;
					앙양전술.menu = 부대_상위;
					앙양전술.init = pk::unit_menu_item_init_t(init);
					앙양전술.get_text = pk::menu_item_get_text_t(getText_앙양전술);
					앙양전술.get_desc = pk::menu_item_get_desc_t(getDesc_앙양전술);
					앙양전술.is_enabled = pk::menu_item_is_enabled_t(isEnabled_앙양전술);
					앙양전술.handler = pk::unit_menu_item_handler_t(handler_앙양전술);
					pk::add_menu_item(앙양전술);
				}

				if (开启_威风屈敌)
				{
					pk::menu_item 위풍전술;
					위풍전술.menu = 부대_상위;
					위풍전술.init = pk::unit_menu_item_init_t(init);
					위풍전술.get_text = pk::menu_item_get_text_t(getText_威风屈敌);
					위풍전술.get_desc = pk::menu_item_get_desc_t(getDesc_威风屈敌);
					위풍전술.is_enabled = pk::menu_item_is_enabled_t(isEnabled_威风屈敌);
					위풍전술.handler = pk::unit_menu_item_handler_t(handler_威风屈敌);
					pk::add_menu_item(위풍전술);
				}

				if (开启_枪兵冲锋)
				{
					pk::menu_item 창군전술;
					창군전술.menu = 부대_상위;
					창군전술.init = pk::unit_menu_item_init_t(init);
					창군전술.get_text = pk::menu_item_get_text_t(getText_창군전술);
					창군전술.get_desc = pk::menu_item_get_desc_t(getDesc_창군전술);
					창군전술.is_enabled = pk::menu_item_is_enabled_t(isEnabled_창군전술);
					창군전술.handler = pk::unit_menu_item_handler_t(handler_창군전술);
					pk::add_menu_item(창군전술);
				}

				if (开启_戟兵破甲)
				{
					pk::menu_item 극군전술;
					극군전술.menu = 부대_상위;
					극군전술.init = pk::unit_menu_item_init_t(init);
					극군전술.get_text = pk::menu_item_get_text_t(getText_극군전술);
					극군전술.get_desc = pk::menu_item_get_desc_t(getDesc_극군전술);
					극군전술.is_enabled = pk::menu_item_is_enabled_t(isEnabled_극군전술);
					극군전술.handler = pk::unit_menu_item_handler_t(handler_극군전술);
					pk::add_menu_item(극군전술);
				}

				if (开启_骑兵突袭)
				{
					pk::menu_item 기마전술;
					기마전술.menu = 부대_상위;
					기마전술.init = pk::unit_menu_item_init_t(init);
					기마전술.get_text = pk::menu_item_get_text_t(getText_기마전술);
					기마전술.get_desc = pk::menu_item_get_desc_t(getDesc_기마전술);
					기마전술.is_enabled = pk::menu_item_is_enabled_t(isEnabled_기마전술);
					기마전술.handler = pk::unit_menu_item_handler_t(handler_기마전술);
					pk::add_menu_item(기마전술);
				}

				if (开启_大杀四方)
				{
					pk::menu_item 용장전술;
					용장전술.menu = 부대_상위;
					용장전술.init = pk::unit_menu_item_init_t(init);
					용장전술.get_text = pk::menu_item_get_text_t(getText_용장전술);
					용장전술.get_desc = pk::menu_item_get_desc_t(getDesc_용장전술);
					용장전술.is_enabled = pk::menu_item_is_enabled_t(isEnabled_용장전술);
					용장전술.handler = pk::unit_menu_item_handler_t(handler_용장전술);
					pk::add_menu_item(용장전술);
				}

			}

		} // add_menu_unit_order

		void init(pk::unit@ unit, pk::point src_pos)
		{
			@src_unit = @unit;
			src_pos_ = src_pos;
			@src_leader = pk::get_person(src_unit.leader);
		}


		//---------------------------------------------------------------------------
		// 부대전술메뉴 : 앙양전술 
		//---------------------------------------------------------------------------

		string getText_앙양전술()
		{
			return pk::encode(pk::format("昂扬奋战 ({})", ENERGY_COST_昂扬奋战));  //angyang zhanhsu
		}

		string getDesc_앙양전술()
		{
			if (src_unit.energy < ENERGY_COST_昂扬奋战)
				return pk::encode("气力不足.");
			else if (src_unit.troops < 部队战术_兵力条件)
				return pk::encode("兵力不足.");
			else if (!ch::has_skill(src_unit, 특기_앙양))
				return pk::encode("没有特技昂扬.");
			else if ((src_leader.stat[武将能力_统率] < 部队战术_统率条件) or (src_leader.stat[武将能力_武力] < 部队战术_武力条件))
				return pk::encode("统率/武力不符合条件.");
			else
				return pk::encode(pk::format("使用昂扬奋战战术.(气力至少 {} 以上)", ENERGY_COST_昂扬奋战));
		}

		bool isVisible_앙양전술()
		{
			return true;
		}

		bool isEnabled_앙양전술()
		{
			if (src_unit.energy < ENERGY_COST_昂扬奋战) return false;
			else if (src_unit.troops < 部队战术_兵力条件) return false;
			else if (!ch::has_skill(src_unit, 특기_앙양)) return false;
			else if ((src_leader.stat[武将能力_统率] < 部队战术_统率条件) or (src_leader.stat[武将能力_武力] < 部队战术_武力条件)) return false;
			return true;
		}

		bool handler_앙양전술(pk::point dst_pos)
		{
			global_func_昂扬奋战(src_pos_, @src_unit, @src_leader);

			global_func_发动完成结算(@src_unit, ENERGY_COST_昂扬奋战);

			//pk::scene(pk::scene_t(scen_昂扬奋战));

			return true;
		}

		void scen_昂扬奋战()
		{
			//pk::fade(0);
			//pk::background(-1);
			pk::background("media/animation/109-1.png");
			//pk::play_se(16);
			pk::sleep(300);
			pk::background("media/animation/110-1.png");
			pk::sleep(300);
			pk::background("media/animation/111-1.png");
			pk::sleep(300);
			pk::background("media/animation/112-1.png");
			pk::sleep(300);
			//pk::background(5);
		}


		//---------------------------------------------------------------------------
		// 부대전술메뉴 : 위풍전술 
		//---------------------------------------------------------------------------

		string getText_威风屈敌()
		{
			return pk::encode(pk::format("威风屈敌 ({})", ENERGY_COST_威风屈敌));  //위풍전술
		}

		string getDesc_威风屈敌()
		{
			if (src_unit.energy < ENERGY_COST_威风屈敌)
				return pk::encode("气力不足.");
			else if (src_unit.troops < 部队战术_兵力条件)
				return pk::encode("兵力不足.");
			else if (!ch::has_skill(src_unit, 特技_威风))
				return pk::encode("没有特技威风.");
			else if ((src_leader.stat[武将能力_统率] < 部队战术_统率条件) or (src_leader.stat[武将能力_武力] < 部队战术_武力条件))
				return pk::encode("统率/武力不符合条件.");
			else
				return pk::encode(pk::format("使用威风屈敌战术.(气力至少 {} 以上)", ENERGY_COST_威风屈敌));
		}

		bool isVisible_威风屈敌()
		{
			return true;
		}

		bool isEnabled_威风屈敌()
		{
			if (src_unit.energy < ENERGY_COST_威风屈敌) return false;
			else if (src_unit.troops < 部队战术_兵力条件) return false;
			else if (!ch::has_skill(src_unit, 特技_威风)) return false;
			else if ((src_leader.stat[武将能力_统率] < 部队战术_统率条件) or (src_leader.stat[武将能力_武力] < 部队战术_武力条件)) return false;
			return true;
		}

		bool handler_威风屈敌(pk::point dst_pos)
		{
			global_func_威风屈敌(src_pos_, @src_unit, @src_leader);

			global_func_发动完成结算(@src_unit, ENERGY_COST_威风屈敌);

			return true;
		}


		//---------------------------------------------------------------------------
		// 부대전술메뉴 : 창군전술 
		//---------------------------------------------------------------------------

		string getText_창군전술()
		{
			return pk::encode(pk::format("枪兵冲锋 ({})", ENERGY_COST_枪兵冲锋));  //qiangjiang
		}

		string getDesc_창군전술()
		{
			if (src_unit.energy < ENERGY_COST_枪兵冲锋)
				return pk::encode("气力不足.");
			else if (src_unit.weapon != 兵器_枪)
				return pk::encode("兵种不符, 无法发动!");
			else if (src_unit.troops < 部队战术_兵力条件)
				return pk::encode("兵力不足.");
			else if (!ch::has_skill(src_unit, 특기_창장) and !ch::has_skill(src_unit, 특기_창신) and !ch::has_skill(src_unit, 특기_투신))
				return pk::encode("没有枪兵系特技.");
			else if ((src_leader.stat[武将能力_统率] < 部队战术_统率条件) or (src_leader.stat[武将能力_武力] < 部队战术_武力条件))
				return pk::encode("统率/武力不符合条件.");
			else
				return pk::encode(pk::format("使用枪兵冲锋战术(敌骑军气力/兵力减少).(气力至少 {} 以上)", ENERGY_COST_枪兵冲锋));
		}

		bool isVisible_창군전술()
		{
			return true;
		}

		bool isEnabled_창군전술()
		{
			if (src_unit.energy < ENERGY_COST_枪兵冲锋) return false;
			else if (src_unit.weapon != 兵器_枪) return false;
			else if (src_unit.troops < 部队战术_兵力条件) return false;
			else if (!ch::has_skill(src_unit, 특기_창장) and !ch::has_skill(src_unit, 특기_창신) and !ch::has_skill(src_unit, 특기_투신)) return false;
			else if ((src_leader.stat[武将能力_统率] < 部队战术_统率条件) or (src_leader.stat[武将能力_武力] < 部队战术_武力条件)) return false;
			return true;
		}

		bool handler_창군전술(pk::point dst_pos)
		{
			global_func_枪兵冲锋(src_pos_, @src_unit, @src_leader);

			global_func_发动完成结算(@src_unit, ENERGY_COST_枪兵冲锋);
			
			//pk::scene(pk::scene_t(scen_枪兵冲锋));

			return true;
		}

		void scen_枪兵冲锋()
		{
			//pk::fade(0);
			//pk::background(-1);
			pk::background("media/animation/9-1.png");
			//pk::play_se(16);
			pk::sleep(300);
			pk::background("media/animation/10-1.png");
			pk::sleep(300);
			pk::background("media/animation/11-1.png");
			pk::sleep(300);
			pk::background("media/animation/12-1.png");
			pk::sleep(300);
			//pk::background(5);
		}

		//---------------------------------------------------------------------------
		// 부대전술메뉴 : 극군전술 
		//---------------------------------------------------------------------------

		string getText_극군전술()
		{
			return pk::encode(pk::format("戟兵破甲 ({})", ENERGY_COST_戟兵破甲));  //jibing
		}

		string getDesc_극군전술()
		{
			if (src_unit.energy < ENERGY_COST_戟兵破甲)
				return pk::encode("气力不足.");
			else if (src_unit.weapon != 兵器_戟)
				return pk::encode("兵种不符, 无法发动!");
			else if (src_unit.troops < 部队战术_兵力条件)
				return pk::encode("兵力不足.");
			else if (!ch::has_skill(src_unit, 특기_극장) and !ch::has_skill(src_unit, 특기_극신) and !ch::has_skill(src_unit, 특기_투신) and !ch::has_skill(src_unit, 특기_친월) and !ch::has_skill(src_unit, 특기_친만))
				return pk::encode("没有戟兵系特技.");
			else if ((src_leader.stat[武将能力_统率] < 部队战术_统率条件) or (src_leader.stat[武将能力_武力] < 部队战术_武力条件))
				return pk::encode("统率/武力不符合条件.");
			else
				return pk::encode(pk::format("使用戟兵破甲战术(敌枪兵气力/兵力减少).(气力至少 {} 以上)", ENERGY_COST_戟兵破甲));
		}

		bool isVisible_극군전술()
		{
			return true;
		}

		bool isEnabled_극군전술()
		{
			if (src_unit.energy < ENERGY_COST_戟兵破甲) return false;
			else if (src_unit.weapon != 兵器_戟) return false;
			else if (src_unit.troops < 部队战术_兵力条件) return false;
			else if (!ch::has_skill(src_unit, 특기_극장) and !ch::has_skill(src_unit, 특기_극신) and !ch::has_skill(src_unit, 특기_투신) and !ch::has_skill(src_unit, 특기_친월) and !ch::has_skill(src_unit, 특기_친만)) return false;
			else if ((src_leader.stat[武将能力_统率] < 部队战术_统率条件) or (src_leader.stat[武将能力_武力] < 部队战术_武力条件)) return false;
			return true;
		}

		bool handler_극군전술(pk::point dst_pos)
		{
			global_func_戟兵破甲(src_pos_, @src_unit, @src_leader);

			global_func_发动完成结算(@src_unit, ENERGY_COST_戟兵破甲);

			return true;
		}


		//---------------------------------------------------------------------------
		// 부대전술메뉴 : 기마전술 
		//---------------------------------------------------------------------------

		string getText_기마전술()
		{
			return pk::encode(pk::format("骑兵突袭 ({})", ENERGY_COST_骑兵突袭));  //qibing zhanshu
		}

		string getDesc_기마전술()
		{
			if (src_unit.energy < ENERGY_COST_骑兵突袭)
				return pk::encode("气力不足.");
			else if (src_unit.weapon != 兵器_战马)
				return pk::encode("兵种不符, 无法发动!");
			else if (src_unit.troops < 部队战术_兵力条件)
				return pk::encode("兵力不足.");
			else if (!ch::has_skill(src_unit, 特技_疾驰) and !ch::has_skill(src_unit, 특기_기장) and !ch::has_skill(src_unit, 특기_기신) and !ch::has_skill(src_unit, 특기_친강) and !ch::has_skill(src_unit, 특기_친오))
				return pk::encode("没有骑兵系特技.");
			else if ((src_leader.stat[武将能力_统率] < 部队战术_统率条件) or (src_leader.stat[武将能力_武力] < 部队战术_武力条件))
				return pk::encode("统率/武力不符合条件.");
			else
				return pk::encode(pk::format("使用骑兵突袭战术(敌戟军气力/兵力减少).(气力至少 {} 以上)", ENERGY_COST_骑兵突袭));
		}

		bool isVisible_기마전술()
		{
			return true;
		}

		bool isEnabled_기마전술()
		{
			if (src_unit.energy < ENERGY_COST_骑兵突袭) return false;
			else if (src_unit.weapon != 兵器_战马) return false;
			else if (src_unit.troops < 部队战术_兵力条件) return false;
			else if (!ch::has_skill(src_unit, 特技_疾驰) and !ch::has_skill(src_unit, 특기_기장) and !ch::has_skill(src_unit, 특기_기신) and !ch::has_skill(src_unit, 특기_친강) and !ch::has_skill(src_unit, 특기_친오)) return false;
			else if ((src_leader.stat[武将能力_统率] < 部队战术_统率条件) or (src_leader.stat[武将能力_武力] < 部队战术_武力条件)) return false;
			return true;
		}

		bool handler_기마전술(pk::point dst_pos)
		{
			global_func_骑兵突袭(src_pos_, @src_unit, @src_leader);

			global_func_发动完成结算(@src_unit, ENERGY_COST_骑兵突袭);
			//pk::scene(pk::scene_t(scen_骑兵突袭));
			return true;
		}

		void scen_骑兵突袭()
		{
			//pk::fade(0);
			//pk::background(-1);
			pk::background("media/animation/17-1.png");
			//pk::play_se(16);
			pk::sleep(300);
			pk::background("media/animation/18-1.png");
			pk::sleep(300);
			pk::background("media/animation/19-1.png");
			pk::sleep(300);
			pk::background("media/animation/20-1.png");
			pk::sleep(300);
			//pk::background(5);
		}
		//---------------------------------------------------------------------------
		// 부대전술메뉴 : 용장전술 
		//---------------------------------------------------------------------------

		string getText_용장전술()
		{
			return pk::encode(pk::format("大杀四方 ({})", ENERGY_COST_大杀四方));  //yongjiang zhanshu
		}

		string getDesc_용장전술()
		{
			if (src_unit.energy < ENERGY_COST_大杀四方)
				return pk::encode("气力不足.");
			else if (src_unit.troops < 部队战术_兵力条件)
				return pk::encode("兵力不足.");
			else if (!ch::has_skill(src_unit, 특기_패왕) and !ch::has_skill(src_unit, 특기_용장) and !ch::has_skill(src_unit, 특기_맹장) and !ch::has_skill(src_unit, 특기_신장) and !ch::has_skill(src_unit, 특기_비장) and !ch::has_skill(src_unit, 특기_투신) and !ch::has_skill(src_unit, 特技_战神))
				return pk::encode("没有猛将系特技.");
			else if ((src_leader.stat[武将能力_统率] < 部队战术_统率条件) or (src_leader.stat[武将能力_武力] < 部队战术_武力条件))
				return pk::encode("统率/武力不符合条件.");
			else
				return pk::encode(pk::format("依靠猛将大杀四方的战术(敌军气力/兵力减少/混乱).(气力至少 {} 以上)", ENERGY_COST_大杀四方));
		}

		bool isVisible_용장전술()
		{
			return true;
		}

		bool isEnabled_용장전술()
		{
			if (src_unit.energy < ENERGY_COST_大杀四方) return false;
			else if (src_unit.troops < 部队战术_兵力条件) return false;
			else if (!ch::has_skill(src_unit, 특기_패왕) and !ch::has_skill(src_unit, 특기_용장) and !ch::has_skill(src_unit, 특기_맹장) and !ch::has_skill(src_unit, 특기_신장) and !ch::has_skill(src_unit, 특기_비장) and !ch::has_skill(src_unit, 特技_战神)) return false;
			else if ((src_leader.stat[武将能力_统率] < 部队战术_统率条件) or (src_leader.stat[武将能力_武力] < 部队战术_武力条件)) return false;
			return true;
		}

		bool handler_용장전술(pk::point dst_pos)
		{
			global_func_大杀四方(src_pos_, @src_unit, @src_leader);

			global_func_发动完成结算(@src_unit, ENERGY_COST_大杀四方);
			
			//pk::scene(pk::scene_t(scen_大杀四方));
			/*变换兵种
			src_unit.weapon = 兵器_木兽;
			src_unit.stock[0].id = 兵器_木兽;
			pk::create_effect(59, src_unit.pos);//59,75
			//src_unit.update();
			array<pk::point>steps;
			steps.insertLast(src_unit.pos);
			pk::move(src_unit, steps);
			src_unit.stock[0].amount = 1;*/
			return true;
		}

		void scen_大杀四方()
		{
			//pk::fade(255);

			  //pk::background(-1);
			pk::play_bgm_file(pk::core_dir + "data/media/se/se_h001_spear.ogg", false, 100);
			pk::background("media/animation/9-1.png");

			pk::sleep(500);
			pk::background("media/animation/10-1.png");
			pk::sleep(500);
			pk::background("media/animation/11-1.png");
			pk::sleep(500);
			pk::background("media/animation/12-1.png");
			pk::sleep(500);
			//pk::background(5);
		}
	} // class Main

	Main main;

} // namespace
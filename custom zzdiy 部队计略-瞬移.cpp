/*

娱乐之作
效果：可移动到全地图任意合理位置
条件：兵力至少5000，气力至少30，且队伍中存在智力大于90的武将
 */
namespace 部队计略_瞬移
{

	const int 瞬移气力消耗 = 30;

	class Main {
		Main() {

		  unit_debate_list.clear();
			// pk::bind(173, pk::trigger173_t(onUnitExpAdd));
          pk::bind(111, pk::trigger111_t(onTurnStart));
          pk::bind(112, pk::trigger112_t(onTurnEnd));
          pk::bind(102, pk::trigger102_t(onGameInit));

          add_menu_stragety();
		}

		 pk::unit@ src_unit;
		 pk::point src_pos_;
		 pk::list<pk::unit@> unit_debate_list;

		void add_menu_stragety() {
			pk::menu_item item;
			item.menu = 115;
			// item.shortcut = 동맹진정_메뉴_단축키;
			item.pos = 116;
			item.init = pk::unit_menu_item_init_t(init);
			item.get_text = pk::menu_item_get_text_t(getText_瞬移);
			item.get_desc = pk::menu_item_get_desc_t(getDesc_瞬移);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_瞬移);
			item.is_visible = pk::menu_item_is_visible_t(isVisible_瞬移);
			item.get_targets = pk::unit_menu_item_get_targets_t(getTargets_瞬移);
			item.handler = pk::unit_menu_item_handler_t(handler_瞬移);
			pk::add_menu_item(item);
		}

		void onUnitExpAdd(pk::unit@ unit, int type)
    {
        if (pk::is_campaign()) return;
        unit_debate_list.clear();
    }
    void onTurnStart(pk::force@ force)
    {
        if (pk::is_campaign()) return;
        unit_debate_list.clear();               // 리스트 초기화
    }
    void onTurnEnd(pk::force@ force)
    {
        if (pk::is_campaign()) return;
        unit_debate_list.clear();               // 리스트 초기화
    }
    void onGameInit()
    {
        if (pk::is_campaign()) return;
        unit_debate_list.clear();               // 리스트 초기화
    }

    void init(pk::unit@ unit, pk::point src_pos)
    {
        @src_unit = @unit;
        src_pos_ = src_pos;
    }

    // ---------------------------------------------------------

		uint8 get_strtegy_energy_cost(int energy_cost)
    {
        uint8 n = uint8(energy_cost);
        if (src_unit.has_skill(特技_百出))
        {
            if (百出耗气规则 >= 1)
                n /= 2;
            else
                n = 1;
        }

        if (兵力影响耗气)
        {
            if (src_unit.troops <= 2000)
                n = uint8(n * pk::min(10.f, 2000.f / src_unit.troops));
            else if (src_unit.troops >= 10000)
                n = uint8(n * pk::max(0.5f, (2 - src_unit.troops / 10000.f)));
        }

        return n;
    }


		string getText_瞬移()
    {
        return pk::encode(pk::format("瞬移({})", get_strtegy_energy_cost(瞬移气力消耗)));
    }

    string getDesc_瞬移()
    {
        if (src_unit.energy < get_strtegy_energy_cost(瞬移气力消耗))
            return pk::encode("气力不足");
        else
            return pk::encode("移动到全地图任意位置");
    }

    bool isEnabled_瞬移()
    {
        if (src_unit.energy < get_strtegy_energy_cost(瞬移气力消耗)) return false;
        return true;
    }

    bool unit_stat(pk::unit@ unit)
    {
      if (unit is null) return false;
      for (int m = 0; m < 3; m++)
      {
        if (pk::is_valid_person_id(unit.member[m]))
        {
          pk::person@ member_t = pk::get_person(unit.member[m]);  //隊伍中的武將
          if (member_t is null || !pk::is_alive(member_t)) continue;
          if (member_t.stat[武将能力_智力] >= 90)  // 等于0，则代表今年为本命年，附带效果由另一个方法决定
          {
            return true;
          }
        }
      }
      return false;
    }

    bool isVisible_瞬移()
    {
        if (pk::is_campaign()) return false;
        return unit_stat(src_unit);
    }

    pk::array<pk::point_int> getTargets_瞬移()
    {
				pk::array<pk::point_int> targets;
				array<pk::point> rings = pk::range(src_pos_, 1, 500);
				for (int i = 0; i < int(rings.length); i++)
				{
          pk::hex@ hex = pk::get_hex(rings[i]);
          int terrain_id = hex.terrain;

          pk::hex_object@ hex_object = pk::get_hex_object(rings[i]);
          if (hex_object is null and terrain_id < 地形_未定) {
              targets.insertLast(pk::point_int(rings[i], 1));
          }
				}

				return targets;
    }

    bool handler_瞬移(pk::point dst_pos)
		{

			// 부대 무장들의 지력 경험치 상승
			// pk::add_stat_exp(src_unit, 무장능력_지력, 部队妙计_增加经验);
      pk::create_effect(0x40, dst_pos);
      pk::wait(1000);
			pk::set_pos(src_unit, dst_pos);
      // src_unit.update(); // 过时方法，使用update_unit
      pk::update_unit(src_unit);
      pk::say(pk::encode("这才是仙人的力量"), pk::get_person(src_unit.leader));

      pk::add_energy(src_unit, -get_strtegy_energy_cost(瞬移气力消耗), false);
			// 행동완료
			src_unit.action_done = true;
			if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
				src_unit.action_done = false;

			return true;
		}
	}
	Main main;
}

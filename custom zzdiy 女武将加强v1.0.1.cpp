/*
  作者：黑店小小二

  1.0 貂蝉新增神术【离魂】
  1.0.1 修复貂蝉神术对女武将起效bug
*/

namespace 女武将加强 {

  const int KEY_离魂 = pk::hash("神术_离魂");

  const int KEY_离魂_索引_武将起始 = 0;

  const int 计略气力消耗_通用 = 30;
  const int 兵力条件_通用 = 5000;

  const int 兵力条件_离魂 = 2000;

  class Main
  {
    pk::random random(pk::rand());

    Main() {
      add_menu_unit_order();
      pk::bind(112, pk::trigger112_t(onTurnEnd));
    }

    pk::unit@ src_unit;
    pk::person@ src_leader;
    pk::person@ person_貂蝉;
    pk::point src_pos_;

    bool 开启_离魂 = true;

    void add_menu_unit_order()
    {

      pk::menu_item 菜单;
      int 菜单_神术;

      菜单.menu = 1;
      菜单.init = pk::unit_menu_item_init_t(init);
      菜单.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("神术"); });
      菜单.is_visible = pk::menu_item_is_visible_t(isVisible_神术);
      菜单_神术 = pk::add_menu_item(菜单);

      if (开启_离魂) {
        pk::menu_item 神术_离魂;
        神术_离魂.menu = 菜单_神术;
        神术_离魂.init = pk::unit_menu_item_init_t(init);
        神术_离魂.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称(pk::encode("离魂"), 计略气力消耗_通用); });
        神术_离魂.get_desc = pk::menu_item_get_desc_t(getDesc_神术_离魂);
        神术_离魂.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return main.isVisible_神术_名称(武将_貂蝉); });
        神术_离魂.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_离魂);
        神术_离魂.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.getTargets_神术_目标部队(3); });
        神术_离魂.handler = pk::unit_menu_item_handler_t(handler_神术_离魂);
        pk::add_menu_item(神术_离魂);
      }
    }

    void init(pk::unit@ unit, pk::point src_pos)
    {
      @src_unit = @unit;
      @src_leader = pk::get_person(src_unit.leader);
      @person_貂蝉 = pk::get_person(武将_貂蝉);
      src_pos_ = src_pos;
    }

    bool isVisible_神术()
    {
      if (pk::is_campaign()) return false;
      array<int> 神术技能武将= {
        person_貂蝉.get_id()
      };
      if (神术技能武将.find(src_leader.get_id()) >= 0) return true;
      return false;
    }

    // ------------------ 通用 ------------------

    string getText_神术_名称(string 神术名称, int 气力消耗)
    {
      return pk::encode(pk::format("{} ({})", 神术名称, 气力消耗));
    }

    bool isVisible_神术_名称(int 武将Id)
    {
      return src_leader.get_id() == 武将Id;
    }

    pk::array<pk::point_int> getTargets_神术_目标()
    {
        pk::array<pk::point_int> targets;
        array<pk::point> rings = pk::range(src_pos_, 1, 3);
        for (int pos_index = 0; pos_index < int(rings.length); pos_index++)
        {
          pk::point dst_pos = rings[pos_index];
          targets.insertLast(pk::point_int(dst_pos, 1));
        }

        return targets;
    }

    pk::array<pk::point_int> getTargets_神术_目标部队(int range)
    {
        pk::array<pk::point_int> targets;
        array<pk::point> rings = pk::range(src_pos_, 1, range);
        for (int i = 0; i < int(rings.length); i++)
        {
          pk::point dst_pos = rings[i];
          pk::unit@ dst = pk::get_unit(dst_pos);
          if (dst !is null and dst.get_force_id() != src_unit.get_force_id() and pk::get_person(dst.leader).sex == 性别_男)
          {
            targets.insertLast(pk::point_int(dst_pos, 1));
          }
        }

        return targets;
    }

    // --------------------------- 时机处理 ----------------
    void onTurnEnd(pk::force@ force)
    {
      int force_id = force.get_id();
      const int unit_id = uint64(pk::load(KEY_离魂, KEY_离魂_索引_武将起始 + 武将_貂蝉, uint64(-1)));
      if (unit_id >= 0)
      {
        pk::unit@ dst = pk::get_unit(unit_id);
        ch::add_troops(dst, dst.troops, true);
        pk::say(pk::encode("士兵好像回来了，但总感觉不一样..."), pk::get_person(dst.leader));
        pk::store(KEY_离魂, KEY_离魂_索引_武将起始 + 武将_貂蝉, -1);
      }
      else
      {
        pk::store(KEY_离魂, KEY_离魂_索引_武将起始 + 武将_貂蝉, -1);
      }
    }

    // --------------------------- 离魂 ---------------------
    string getDesc_神术_离魂()
    {
      const int id = uint64(pk::load(KEY_离魂, KEY_离魂_索引_武将起始 + 武将_貂蝉, uint64(-1)));
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_离魂)
        return pk::encode(pk::format("兵力不足,兵力至少{}。", 兵力条件_离魂));
      else if (int(src_unit.troops) == pk::get_max_troops(src_unit))
        return pk::encode("兵力已达上限.");
      else if (id >= 0)
        return pk::encode("本回合已经使用过");
      else
        return pk::encode(pk::format("消耗气力{}，不视为行动，获得目标部队一半兵力，回合结束后该部队兵力变为2倍", 计略气力消耗_通用));
    }

    bool isEnabled_神术_离魂()
    {
      const int id = uint64(pk::load(KEY_离魂, KEY_离魂_索引_武将起始 + 武将_貂蝉, uint64(-1)));
      if (src_unit.energy < 计略气力消耗_通用) return false;
      else if (src_unit.troops < 兵力条件_离魂) return false;
      else if (int(src_unit.troops) == pk::get_max_troops(src_unit)) return false;
      else if (id >= 0) return false;
      return true;
    }

    bool handler_神术_离魂(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(126,1000);
      pk::wait(1000);

      pk::unit@ target_unit = pk::get_unit(dst_pos);
      const float target_troops = 0.5 * target_unit.troops;

      if (pk::is_in_screen(src_unit.pos))
      {
        pk::create_effect(58, src_unit.pos);
      }

      if (pk::is_in_screen(dst_pos))
      {
        pk::create_effect(63, dst_pos);
        pk::wait(2000);
        ch::add_troops(target_unit, -int(target_troops), true);
        pk::say(pk::encode("怎么回事，快拦住士兵们！"), pk::get_person(target_unit.leader));
      }

      ch::add_troops(src_unit, int(target_troops), true);
      pk::wait(1000);
      pk::say(pk::encode("一场好戏，嗯哼哼。"), person_貂蝉);

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 10);

      pk::store(KEY_离魂, KEY_离魂_索引_武将起始 + 武将_貂蝉, target_unit.get_id());

      person_貂蝉.update();

      // src_unit.action_done = true;
      // if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
      //   src_unit.action_done = false;

      return true;
    }
  }

  Main main;
}

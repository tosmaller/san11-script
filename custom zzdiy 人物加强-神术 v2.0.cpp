
/*
作者：黑店小小二
pk2.2 版本要求，至少为2.4.220319
v2.0 2022/03/21 庞统新增神术【火凤连环】,贾诩新增神术【乱武完杀】, 法正新增神术【奇谋诡策】
v1.9 2022/03/18 司马懿新增神术【狼顾权变】，修复郭嘉神术bug
v1.8 2022/03/01 诸葛亮新增神术【神鬼八阵】
v1.7 2022/02/28 袁术新增神术【妄尊仲帝】
v1.6 2022/02/25 郭嘉新增神术【神鬼奇谋】
v1.5 2022/02/22 调整写法，优化代码体积
v1.4 2022/02/19 修复【黄天泰平】劝降君主，都督后的处理
v1.3 2022/02/17 修复对城池耐久归0归属变更bug，张角新增神术【黄天泰平】,调整【神雷灭世】动画效果，曹操新增神术【天下归心】
v1.2 2022/02/14 修复措施耐久归0归属变更bug
v1.1 2022/02/13 修复周瑜神术对关卡跟港口无效bug，张角新增神术【神雷灭世】
v1.0 2022/02/12 新增周瑜神术【业火焚天】

描述：针对高智武将，设置了大招【神术】，旨在增加游戏娱乐性
 */

namespace 人物加强 {
  const int 临时武将起始 = 780;
  const int 临时武将终止 = 799;

  const int KEY_神鬼八阵_一击必杀 = pk::hash("神术_神鬼八阵_一击必杀");
  const int KEY_神鬼八阵_回合数 = pk::hash("神术_神鬼八阵_回合数");
  const int KEY_神鬼八阵_一击必杀_索引_武将起始 = 0;

  const int KEY_狼顾权变_失去技能回合数 = pk::hash("神术_狼顾权变_失去技能回合数");
  const int KEY_狼顾权变_失去技能 = pk::hash("神术_狼顾权变_失去技能");

  const int KEY_火凤连环 = pk::hash("神术_火凤连环");

  const int KEY_乱武完杀 = pk::hash("神术_乱武完杀");

  const int KEY_奇谋诡策 = pk::hash("神术_奇谋诡策");

  const int 计略气力消耗_通用 = 60;
  const int 兵力条件_通用 = 5000;

  const int 计略气力消耗_黄天泰平 = 100;
  const int 计略气力消耗_乱武完杀 = 100;

  pk::func209_t@ prev_callback_209;
  pk::func58_t@ prev_callback_58;

  array<string> 已影响城市; // 存储处理过的城市

  class Main
  {
    pk::random random(pk::rand());
    Main() {
      add_menu_unit_order();

      @prev_callback_209 = cast<pk::func209_t@>(pk::get_func(209));
      pk::reset_func(209);
      pk::set_func(209, pk::func209_t(func209));

      @prev_callback_58 = cast<pk::func58_t@>(pk::get_func(58));
      pk::reset_func(58);
      pk::set_func(58, pk::func58_t(func58));

      pk::bind(108, pk::trigger108_t(onMonthStart));
      pk::bind(111, pk::trigger111_t(onTurnStart));
      pk::bind(112, pk::trigger111_t(onTurnEnd));
      pk::bind(171, pk::trigger171_t(onUnitRemove));
      pk::bind(174, pk::trigger174_t(onUnitActionDone));
      pk::bind(179, pk::trigger179_t(onUnitAfterAttack));
    }

    pk::unit@ src_unit;
    pk::person@ src_leader;
    pk::person@ person_周瑜;
    pk::person@ person_张角;
    pk::person@ person_张宝;
    pk::person@ person_张梁;
    pk::person@ person_曹操;
    pk::person@ person_郭嘉;
    pk::person@ person_袁术;
    pk::person@ person_诸葛亮;
    pk::person@ person_司马懿;
    pk::person@ person_庞统;
    pk::person@ person_贾诩;
    pk::person@ person_法正;
    pk::point src_pos_;
    pk::force@ src_force;

    bool 开启_业火焚天 = true; // 周瑜大招
    bool 开启_神雷灭世 = true; // 张角大招
    bool 开启_黄天泰平 = true; // 张角三兄弟大招
    bool 开启_天下归心 = true; // 曹操大招
    bool 开启_神鬼奇谋 = true; // 郭嘉大招
    bool 开启_妄尊仲帝 = true; // 袁术大招
    bool 开启_神鬼八阵 = true; // 诸葛亮大招
    bool 开启_狼顾权变 = true; // 司马懿大招
    bool 开启_火凤连环 = true; // 庞统大招
    bool 开启_乱武完杀 = true; // 贾诩大招
    bool 开启_奇谋诡策 = true; // 法正大招

    bool 妄尊仲帝_启用状态 = true;
    bool 神鬼八阵_启用状态 = true;

    void add_menu_unit_order()
    {

      pk::menu_item 菜单;
      int 菜单_神术;

      菜单.menu = 1;
      菜单.init = pk::unit_menu_item_init_t(init);
      菜单.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("神术"); });
      菜单.is_visible = pk::menu_item_is_visible_t(isVisible_神术);
      菜单_神术 = pk::add_menu_item(菜单);

      if (开启_业火焚天) {
        pk::menu_item 神术_业火焚天;
        神术_业火焚天.menu = 菜单_神术;
        神术_业火焚天.init = pk::unit_menu_item_init_t(init);
        神术_业火焚天.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称('业火焚天', 计略气力消耗_通用); });
        神术_业火焚天.get_desc = pk::menu_item_get_desc_t(getDesc_神术_业火焚天);
        神术_业火焚天.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return main.isVisible_神术_名称(武将_周瑜); });
        神术_业火焚天.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_业火焚天);
        神术_业火焚天.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.getTargets_神术_目标(3); });
        神术_业火焚天.handler = pk::unit_menu_item_handler_t(handler_神术_业火焚天);
        pk::add_menu_item(神术_业火焚天);
      }
      if (开启_神雷灭世) {
        pk::menu_item 神术_神雷灭世;
        神术_神雷灭世.menu = 菜单_神术;
        神术_神雷灭世.init = pk::unit_menu_item_init_t(init);
        神术_神雷灭世.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称("神雷灭世", 计略气力消耗_通用); });
        神术_神雷灭世.get_desc = pk::menu_item_get_desc_t(getDesc_神术_神雷灭世);
        神术_神雷灭世.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return main.isVisible_神术_名称(武将_张角); });
        神术_神雷灭世.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_神雷灭世);
        神术_神雷灭世.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.getTargets_神术_目标(3); });
        神术_神雷灭世.handler = pk::unit_menu_item_handler_t(handler_神术_神雷灭世);
        pk::add_menu_item(神术_神雷灭世);
      }
      if (开启_黄天泰平) {
        pk::menu_item 神术_黄天泰平;
        神术_黄天泰平.menu = 菜单_神术;
        神术_黄天泰平.init = pk::unit_menu_item_init_t(init);
        神术_黄天泰平.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称("黄天泰平", 计略气力消耗_黄天泰平); });
        神术_黄天泰平.get_desc = pk::menu_item_get_desc_t(getDesc_神术_黄天泰平);
        神术_黄天泰平.is_visible = pk::menu_item_is_visible_t(isVisible_神术_黄天泰平);
        神术_黄天泰平.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_黄天泰平);
        神术_黄天泰平.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.getTargets_神术_目标(3); });
        神术_黄天泰平.handler = pk::unit_menu_item_handler_t(handler_神术_黄天泰平);
        pk::add_menu_item(神术_黄天泰平);
      }
      if (开启_天下归心) {
        pk::menu_item 神术_天下归心;
        神术_天下归心.menu = 菜单_神术;
        神术_天下归心.init = pk::unit_menu_item_init_t(init);
        神术_天下归心.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称("天下归心", 计略气力消耗_通用); });
        神术_天下归心.get_desc = pk::menu_item_get_desc_t(getDesc_神术_天下归心);
        神术_天下归心.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return main.isVisible_神术_名称(武将_曹操); });
        神术_天下归心.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_天下归心);
        神术_天下归心.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.getTargets_神术_目标(3); });
        神术_天下归心.handler = pk::unit_menu_item_handler_t(handler_神术_天下归心);
        pk::add_menu_item(神术_天下归心);
      }
      if (开启_神鬼奇谋) {
        pk::menu_item 神术_神鬼奇谋;
        神术_神鬼奇谋.menu = 菜单_神术;
        神术_神鬼奇谋.init = pk::unit_menu_item_init_t(init);
        神术_神鬼奇谋.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称("神鬼奇谋", 计略气力消耗_通用); });
        神术_神鬼奇谋.get_desc = pk::menu_item_get_desc_t(getDesc_神术_神鬼奇谋);
        神术_神鬼奇谋.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return main.isVisible_神术_名称(武将_郭嘉); });
        神术_神鬼奇谋.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_神鬼奇谋);
        神术_神鬼奇谋.handler = pk::unit_menu_item_handler_t(handler_神术_神鬼奇谋);
        pk::add_menu_item(神术_神鬼奇谋);
      }
      if (开启_妄尊仲帝) {
        pk::menu_item 神术_妄尊仲帝;
        神术_妄尊仲帝.menu = 菜单_神术;
        神术_妄尊仲帝.init = pk::unit_menu_item_init_t(init);
        神术_妄尊仲帝.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_妄尊仲帝(); });
        神术_妄尊仲帝.get_desc = pk::menu_item_get_desc_t(getDesc_神术_妄尊仲帝);
        神术_妄尊仲帝.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return main.isVisible_神术_名称(武将_袁术); });
        神术_妄尊仲帝.handler = pk::unit_menu_item_handler_t(handler_神术_妄尊仲帝);
        pk::add_menu_item(神术_妄尊仲帝);
      }
      if (开启_神鬼八阵) {
        pk::menu_item 神术_神鬼八阵;
        神术_神鬼八阵.menu = 菜单_神术;
        神术_神鬼八阵.init = pk::unit_menu_item_init_t(init);
        神术_神鬼八阵.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称("神鬼八阵", 计略气力消耗_通用); });
        神术_神鬼八阵.get_desc = pk::menu_item_get_desc_t(getDesc_神术_神鬼八阵);
        神术_神鬼八阵.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return main.isVisible_神术_名称(武将_诸葛亮); });
        神术_神鬼八阵.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_神鬼八阵);
        神术_神鬼八阵.handler = pk::unit_menu_item_handler_t(handler_神术_神鬼八阵);
        pk::add_menu_item(神术_神鬼八阵);
      }
      if (开启_狼顾权变) {
        pk::menu_item 神术_狼顾权变;
        神术_狼顾权变.menu = 菜单_神术;
        神术_狼顾权变.init = pk::unit_menu_item_init_t(init);
        神术_狼顾权变.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称("狼顾权变", 计略气力消耗_通用); });
        神术_狼顾权变.get_desc = pk::menu_item_get_desc_t(getDesc_神术_狼顾权变);
        神术_狼顾权变.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return main.isVisible_神术_名称(武将_司马懿); });
        神术_狼顾权变.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_狼顾权变);
        // 神术_狼顾权变.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.getTargets_狼顾权变_目标部队位置(3); });
        神术_狼顾权变.handler = pk::unit_menu_item_handler_t(handler_神术_狼顾权变);
        pk::add_menu_item(神术_狼顾权变);
      }
      if (开启_火凤连环) {
        pk::menu_item 神术_火凤连环;
        神术_火凤连环.menu = 菜单_神术;
        神术_火凤连环.init = pk::unit_menu_item_init_t(init);
        神术_火凤连环.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称("火凤连环", 计略气力消耗_通用); });
        神术_火凤连环.get_desc = pk::menu_item_get_desc_t(getDesc_神术_火凤连环);
        神术_火凤连环.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return main.isVisible_神术_名称(武将_庞统); });
        神术_火凤连环.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_火凤连环);
        神术_火凤连环.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.getTargets_神术_目标(3); });
        神术_火凤连环.handler = pk::unit_menu_item_handler_t(handler_神术_火凤连环);
        pk::add_menu_item(神术_火凤连环);
      }
      if (开启_乱武完杀) {
        pk::menu_item 神术_乱武完杀;
        神术_乱武完杀.menu = 菜单_神术;
        神术_乱武完杀.init = pk::unit_menu_item_init_t(init);
        神术_乱武完杀.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称("乱武完杀", 计略气力消耗_乱武完杀); });
        神术_乱武完杀.get_desc = pk::menu_item_get_desc_t(getDesc_神术_乱武完杀);
        神术_乱武完杀.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return main.isVisible_神术_名称(武将_贾诩); });
        神术_乱武完杀.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_乱武完杀);
        神术_乱武完杀.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.getTargets_神术_目标(3); });
        神术_乱武完杀.handler = pk::unit_menu_item_handler_t(handler_神术_乱武完杀);
        pk::add_menu_item(神术_乱武完杀);
      }
      if (开启_奇谋诡策) {
        pk::menu_item 神术_奇谋诡策;
        神术_奇谋诡策.menu = 菜单_神术;
        神术_奇谋诡策.init = pk::unit_menu_item_init_t(init);
        神术_奇谋诡策.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称("奇谋诡策", 计略气力消耗_通用); });
        神术_奇谋诡策.get_desc = pk::menu_item_get_desc_t(getDesc_神术_奇谋诡策);
        神术_奇谋诡策.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return main.isVisible_神术_名称(武将_法正); });
        神术_奇谋诡策.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_奇谋诡策);
        神术_奇谋诡策.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.getTargets_神术_目标(3); });
        神术_奇谋诡策.handler = pk::unit_menu_item_handler_t(handler_神术_奇谋诡策);
        pk::add_menu_item(神术_奇谋诡策);
      }
    }
    // ----------- 基础 ------------------
    void init(pk::unit@ unit, pk::point src_pos)
    {
      @src_unit = @unit;
      @src_leader = pk::get_person(src_unit.leader);
      @person_周瑜 = pk::get_person(武将_周瑜);
      @person_张角 = pk::get_person(武将_张角);
      @person_张宝 = pk::get_person(武将_张宝);
      @person_张梁 = pk::get_person(武将_张梁);
      @person_曹操 = pk::get_person(武将_曹操);
      @person_郭嘉 = pk::get_person(武将_郭嘉);
      @person_袁术 = pk::get_person(武将_袁术);
      @person_诸葛亮 = pk::get_person(武将_诸葛亮);
      @person_司马懿 = pk::get_person(武将_司马懿);
      @person_庞统 = pk::get_person(武将_庞统);
      @person_贾诩 = pk::get_person(武将_贾诩);
      @person_法正 = pk::get_person(武将_法正);
      @src_force = pk::get_force(unit.get_force_id());
      src_pos_ = src_pos;
    }

    bool isVisible_神术()
    {
      if (pk::is_campaign()) return false;
      array<int> 神术技能武将= {
        person_周瑜.get_id(),
        person_张角.get_id(),
        person_张宝.get_id(),
        person_张梁.get_id(),
        person_曹操.get_id(),
        person_郭嘉.get_id(),
        person_袁术.get_id(),
        person_诸葛亮.get_id(),
        person_司马懿.get_id(),
        person_庞统.get_id(),
        person_贾诩.get_id(),
        person_法正.get_id()
      };
      if (神术技能武将.find(src_leader.get_id()) >= 0) return true;
      return false;
    }

    // ------------------ 通用 ------------------

    string getText_神术_名称(string& 神术名称, int 气力消耗 = 0)
    {
      return pk::encode(pk::format("{} ({})", 神术名称, 气力消耗));
    }

    bool isVisible_神术_名称(int 武将Id)
    {
      return src_leader.get_id() == 武将Id;
    }
    void 历史日志(pk::person@ person, string name, string result)
    {
      string person_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(person)));
      string action_name = pk::format("\x1b[27x{}\x1b[0x", name);

      string str = pk::format("{}由于{}的效果，{}", person_name, action_name, result);

      pk::unit@ unit = pk::get_unit(pk::hex_object_id_to_unit_id(person.location));
      pk::building@ building = pk::get_building(pk::hex_object_id_to_building_id(person.location));
      pk::point persion_pos;
      if (unit !is null)
      {
        persion_pos = unit.pos;
      }
      else
      {
        persion_pos = building.pos;
      }
      pk::history_log(persion_pos, pk::get_force(person.get_force_id()).color, pk::encode(str));
    }
    void 历史日志(pk::unit@ unit, string name, string result)
    {
      string unit_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(unit)));
      string action_name = pk::format("\x1b[27x{}\x1b[0x", name);

      string str = pk::format("{}队由于{}的效果，{}", unit_name, action_name, result);

      pk::history_log(unit.pos, pk::get_force(unit.get_force_id()).color, pk::encode(str));
    }
    void 历史日志(pk::building@ building, string name, string result)
    {
      string building_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(building)));
      string action_name = pk::format("\x1b[27x{}\x1b[0x", name);

      string str = pk::format("{}由于{}的效果，{}", building_name, action_name, result);

      pk::history_log(building.pos, pk::get_force(building.get_force_id()).color, pk::encode(str));
    }

    pk::array<pk::point_int> getTargets_神术_目标(int range)
    {
        pk::array<pk::point_int> targets;
        array<pk::point> rings = pk::range(src_pos_, 1, range);
        for (int pos_index = 0; pos_index < int(rings.length); pos_index++)
        {
          pk::point dst_pos = rings[pos_index];
          targets.insertLast(pk::point_int(dst_pos, 1));
        }

        return targets;
    }

    pk::array<pk::point_int> getTargets_神术_目标部队位置(int range)
    {
        pk::array<pk::point_int> targets;
        array<pk::point> rings = pk::range(src_pos_, 1, range);
        for (int i = 0; i < int(rings.length); i++)
        {
          pk::point dst_pos = rings[i];
          pk::unit@ dst = pk::get_unit(dst_pos);
          if (dst !is null)
          {
            targets.insertLast(pk::point_int(dst_pos, 1));
          }
        }

        return targets;
    }

    pk::list<pk::unit@> getTargets_神术_目标部队(int range)
    {
        pk::list<pk::unit@> targets;
        array<pk::point> rings = pk::range(src_pos_, 1, range);
        for (int i = 0; i < int(rings.length); i++)
        {
          pk::point dst_pos = rings[i];
          pk::unit@ dst = pk::get_unit(dst_pos);
          if (dst !is null)
          {
            targets.add(dst);
          }
        }

        return targets;
    }

    pk::list<pk::unit@> getTargets_势力_目标部队(pk::force@ force, int range)
    {
        pk::list<pk::unit@> targets;
        array<pk::point> rings = pk::range(src_pos_, 1, range);
        for (int i = 0; i < int(rings.length); i++)
        {
          pk::point dst_pos = rings[i];
          pk::unit@ dst = pk::get_unit(dst_pos);
          if (dst !is null and dst.get_force_id() == force.get_force_id())
          {
            targets.add(dst);
          }
        }

        return targets;
    }


    void fun_处理城市(pk::building@ building, pk::point pos, int hp_damage)
    {
      if (building.facility == 设施_城市 or building.facility == 设施_关隘 or building.facility == 设施_港口)
      {
        pk::city@ city = pk::building_to_city(building); // 城市
        pk::gate@ gate = pk::building_to_gate(building); // 关卡
        pk::port@ port = pk::building_to_port(building); // 港口
        if (city !is null and 已影响城市.find(city.name) < 0)
        {
          已影响城市.insertLast(city.name);
          pk::add_energy(building, -30, true); //`减气
          pk::add_gold(city, city.gold > 10000 ? int(-0.5 * city.gold) : -5000, true); // 大于1万减50%，否则减5000
          pk::add_food(city, city.food > 10000 ? int(-0.5 * city.food) : -5000, true); // 大于1万最后减50%，否则减5000
        }
        if (gate !is null and 已影响城市.find(gate.name) < 0) {
          已影响城市.insertLast(gate.name);
          pk::add_energy(building, -30, true);
          pk::add_gold(building, gate.gold > 10000 ? int(-0.5 * gate.gold) : -5000, true);
          pk::add_food(building, gate.food > 10000 ? int(-0.5 * gate.food) : -5000, true);

        }
        if (port !is null and 已影响城市.find(port.name) < 0) {
          已影响城市.insertLast(port.name);
          pk::add_energy(building, -30, true);
          pk::add_gold(building, port.gold > 10000 ? int(-0.5 * port.gold) : -5000, true);
          pk::add_food(building, port.food > 10000 ? int(-0.5 * port.food) : -5000, true);
        }
        if (building.hp - hp_damage <= 0) // 伤害能破城，那么保留一点hp由部队使用火焰破坏，可以达到攻城效果
        {
            pk::add_hp(building, 1 - building.hp, true);
            pk::create_fire(pos, 1, src_unit, true); // 点火烧城
        } else {
          // 伤害不足以破城，保留
          pk::add_hp(building, -hp_damage, true);
          if (pk::is_in_screen(building.pos))
          {
            switch (pk::rand(4))
            {
            case 0: pk::say(pk::encode("哇哇哇.."), pk::get_person(武将_兵士), building); break;
            case 1: pk::say(pk::encode("我的身体"), pk::get_person(武将_兵士), building); break;
            case 2: pk::say(pk::encode("快去避火"), pk::get_person(武将_兵士), building); break;
            case 3: pk::say(pk::encode("火. 着火了!"), pk::get_person(武将_兵士), building); break;
            }
          }
        }
      } else {
        // 非城池建筑
        pk::add_hp(building, -hp_damage, true);
        if (pk::is_in_screen(building.pos))
        {
            pk::play_se(0x71, building.pos);         //임팩트사운드
            pk::create_effect(0x5c, building.pos);   //임팩트효과
        }
        auto dst_pos0 = building.pos;
        auto dst_facility = pk::get_facility(building.facility);
        pk::kill(building);
        ch::特殊地名争夺处理(dst_facility.get_id(), person_周瑜.get_force_id(), dst_pos0);
      }

      building.update();
    }

    bool unit_has_person(pk::unit@ unit, pk::person@ person)
    {
      for (int m = 0; m < 3; m++)
      {
        if (pk::is_valid_person_id(unit.member[m]))
        {
          pk::person@ member = pk::get_person(unit.member[m]);
          if (member.get_id() == person.get_id()) {
            return true;
          }
        }
      }
      return false;
    }

    void 减少武将寿命(pk::person@ person, int 减少值)
    {
      person.death -= 减少值;
      if (person.death - person.birth <= 0) { // 生命小于等于0，死亡预定
        person.estimated_death = true;
        pk::kill(src_unit);
        pk::wait(2000);
        pk::kill(person);
      }
    }

    // ------------------------------------------------
    void onMonthStart()
    {
      const bool use = bool(pk::load(KEY_神鬼八阵_回合数, 0, false));
      if(use) pk::store(KEY_神鬼八阵_回合数, 0, true);;
    }
    void onTurnStart(pk::force@ force)
    {
      if (person_袁术 is null)
      {
        @person_袁术 = pk::get_person(武将_袁术);
      }
      if (force.get_id() == person_袁术.get_force_id())
      {
        神术_妄尊仲帝_补兵处理();
      }
    }
    void onTurnEnd(pk::force@ force)
    {
      if (person_司马懿 is null)
      {
        @person_司马懿 = pk::get_person(武将_司马懿);
      }
      if (force.get_id() == person_司马懿.get_force_id())
      {
        神术_狼顾权变_特技处理();
      }
      if (force.get_id() == person_庞统.get_force_id())
      {
        火凤连环清除();
      }
      if (force.get_id() == person_法正.get_force_id())
      {
        奇谋诡策清除();
      }
    }
    void onUnitRemove(pk::unit@ unit,pk::hex_object@ dst, int type)
    {
      const bool has_effect = bool(pk::load(KEY_神鬼八阵_一击必杀, KEY_神鬼八阵_一击必杀_索引_武将起始 + unit.get_id(), false));
      if (has_effect)
      {
        pk::store(KEY_神鬼八阵_一击必杀, KEY_神鬼八阵_一击必杀_索引_武将起始 + unit.get_id(), false);
      }
      if (开启_乱武完杀)
      {
        乱武完杀部队溃灭处理(unit, dst, type);
      }
    }

    void onUnitActionDone(pk::unit@ unit,int type)
    {
      const bool has_effect = bool(pk::load(KEY_神鬼八阵_一击必杀, KEY_神鬼八阵_一击必杀_索引_武将起始 + unit.get_id(), false));
      if (has_effect)
      {
        pk::store(KEY_神鬼八阵_一击必杀, KEY_神鬼八阵_一击必杀_索引_武将起始 + unit.get_id(), false);
      }
    }

    void onUnitAfterAttack(pk::unit@ dst_unit, pk::unit@ unit, int troops_damage, int critical, int tactics)
    {
      if (开启_奇谋诡策)
      {
        神术_奇谋诡策_伤害处理(dst_unit, unit);
      }
    }

    void func209(pk::damage_info& info, pk::unit@ attacker, int tactics_id, const pk::point& in target_pos, int type, int critical, bool ambush, int rettype)
    {
      prev_callback_209(info, attacker, tactics_id, target_pos, type, critical, ambush, rettype);
      if (开启_神鬼八阵)
      {
        神术_神鬼八阵_伤害处理(info, attacker, target_pos);
      }
    }

    int func58(pk::unit@ unit,int value, int rettype)
    {
      int troops = prev_callback_58(unit, value, rettype);
      if (unit.leader == 武将_袁术 and 妄尊仲帝_启用状态)
      {
        troops = 神术_妄尊仲帝_伤害处理(unit, troops, rettype);
      }
      if (开启_火凤连环)
      {
        火凤连环伤害(unit, value, rettype);
      }
      if (开启_乱武完杀)
      {
        乱武完杀部队受伤处理(unit, value, rettype);
      }
      return int(troops);
    }

    // ------------------- 业火焚天------------------

    string getDesc_神术_业火焚天()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("使用业火焚天.(气力至少 {} 以上)", 计略气力消耗_通用));
    }
    bool isEnabled_神术_业火焚天()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      else if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void func_业火焚天(pk::point src_pos, int range) {
      array<pk::point> arr = pk::range(src_pos, range, range);
      for (int l = 0; l < int(arr.length); l++)
      {

        pk::hex_object@ hex_obj = pk::get_hex_object(arr[l]);

        pk::unit@ dst = pk::get_unit(arr[l]);
        pk::building@ building = pk::get_building(arr[l]);

        if (dst !is null) {
          if (!pk::is_enemy(src_unit, dst)) continue;
          if (dst.has_skill(特技_火神)) {
            pk::create_effect(0x55, arr[l]);
            pk::say(pk::encode("火神在此，有何俱焉"), pk::get_person(dst.who_has_skill(特技_火神)));
            continue;
          }
          if (dst.has_skill(特技_洞察)) {
            pk::create_effect(0x55, arr[l]);
            pk::say(pk::encode("区区小计，也要丢人现眼吗"), pk::get_person(dst.who_has_skill(特技_洞察)));
            continue;
          }
          pk::set_status(dst, src_unit, 部队状态_伪报, pk::rand(2) + 3, true);
          历史日志(dst, '神术_业火焚天', '陷入伪报');
          pk::create_fire(arr[l], pk::rand(2) + 3, src_unit, true); //火计
          pk::add_energy(dst, -30, true); //减气
          ch::add_troops(dst, -random(500, 1500), true); // 随机减兵500~1500
          历史日志(dst, '神术_业火焚天', '受到了巨大伤害');
          if (dst.troops <= 0) {
            pk::kill(dst, src_unit);
          }
          if (pk::is_in_screen(hex_obj.pos))
          {
            switch (pk::rand(4))
            {
            case 0: pk::say(pk::encode("哇哇哇.."), pk::get_person(武将_兵士), dst); break;
            case 1: pk::say(pk::encode("我的身体"), pk::get_person(武将_兵士), dst); break;
            case 2: pk::say(pk::encode("快去避火"), pk::get_person(武将_兵士), dst); break;
            case 3: pk::say(pk::encode("火. 着火了!"), pk::get_person(武将_兵士), dst); break;
            }
          }
        }

        if (building !is null) {
          if (!pk::is_enemy(src_unit, building)) continue;
          if (building.has_skill(特技_火神) or building.has_skill(特技_洞察)) continue;
          int hp_damage = 建筑火伤_练成炸药附加 + pk::rand(建筑火伤_练成炸药附加) + pk::rand(5000);
          历史日志(building, '神术_业火焚天', '受到了巨大伤害');
          fun_处理城市(building, arr[l], hp_damage);

          building.update();
        }

        if (dst is null and building is null) {
          pk::create_fire(arr[l], pk::rand(2) + 3, src_unit, true);
        }
      }
    }

    bool handler_神术_业火焚天(pk::point dst_pos)
    {
      // pk::special_cutin(121,1000); // 单火焰遮罩
      pk::special_cutin(122,1000); // 全火遮罩
      // pk::special_cutin(123,1000); // 焚烧遮罩
      // pk::special_cutin(124,1000); // 山遮罩
      // pk::special_cutin(125,1000); // 水遮罩
      // pk::special_cutin(126,1000); // 雷电遮罩

      for (int l = 0; l < 11; l++) {
        func_业火焚天(dst_pos, l);
      }

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 10);

      pk::say(pk::encode("感受下这红莲业火吧!"), person_周瑜);

      // pk::wait(100);

      pk::say(pk::encode("怎么感觉有点虚弱呢，难道是天谴吗。。。"), person_周瑜);

      减少武将寿命(src_leader, random(3, 10));
      src_leader.update();

      已影响城市.removeRange(0, 已影响城市.length() - 1); // 清除处理过的

      src_unit.action_done = true;
      if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
        src_unit.action_done = false;

      return true;
    }


    //-------------------------神雷灭世---------------
    string getDesc_神术_神雷灭世()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("使用神雷灭世.(气力至少 {} 以上)", 计略气力消耗_通用));
    }


    bool isEnabled_神术_神雷灭世()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      else if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }


    void func_神雷灭世(pk::point src_pos, int range) {
      array<pk::point> arr = pk::range(src_pos, range, range);
      for (int l = 0; l < int(arr.length); l++)
      {

        pk::create_effect(0x40, arr[l]); // 落雷动画

        pk::hex_object@ hex_obj = pk::get_hex_object(arr[l]);

        pk::unit@ dst = pk::get_unit(arr[l]);
        pk::building@ building = pk::get_building(arr[l]);

        if (dst !is null) {
          if (!pk::is_enemy(src_unit, dst)) continue;
          if (dst.has_skill(特技_鬼门)) {
            pk::create_effect(0x55, arr[l]);
            pk::say(pk::encode("区区雷电，不过尔尔"), pk::get_person(dst.who_has_skill(特技_鬼门)));
            continue;
          }
          pk::set_status(dst, src_unit, 部队状态_混乱, pk::rand(2) + 3, true); // 雷电麻痹
          历史日志(dst, '神术_神雷灭世', '陷入混乱');
          if (pk::rand_bool(50)) { // 50%概率着火
            pk::create_fire(arr[l], pk::rand(2), src_unit, true); //火计
          }
          pk::add_energy(dst, -30, true); //减气
          ch::add_troops(dst, -(部队落雷伤害 + pk::rand(部队落雷伤害_1) + pk::rand(2000)), true); // 在雷电伤害基础上随机加2000
          历史日志(dst, '神术_神雷灭世', '受到了巨大伤害');
          if (dst.troops <= 0) {
            pk::kill(dst, src_unit);
          }
          if (pk::is_in_screen(hex_obj.pos))
          {
            switch (pk::rand(4))
            {
            case 0: pk::say(pk::encode("哇哇哇.."), pk::get_person(武将_兵士), dst); break;
            case 1: pk::say(pk::encode("我的身体"), pk::get_person(武将_兵士), dst); break;
            case 2: pk::say(pk::encode("这是什么力量..."), pk::get_person(武将_兵士), dst); break;
            case 3: pk::say(pk::encode("难道苍天死了吗..."), pk::get_person(武将_兵士), dst); break;
            }
          }
        }

        if (building !is null) {
          if (!pk::is_enemy(src_unit, building)) continue;
          if (building.has_skill(特技_鬼门)) continue;
          int hp_damage = 建筑落雷伤害 + pk::rand(建筑落雷伤害_1) + pk::rand(2000);
          历史日志(building, '神术_神雷灭世', '受到了巨大伤害');
          fun_处理城市(building, arr[l], hp_damage);
          building.update();
        }

        if (dst is null and building is null) {
          if (pk::rand_bool(50)) { // 50%概率着火
            pk::create_fire(arr[l], pk::rand(2), src_unit, true); //火计
          }
        }
      }
    }

    bool handler_神术_神雷灭世(pk::point dst_pos)
    {
      pk::special_cutin(127,1000); // 雷电遮罩
      pk::play_se(121);

      for (int l = 0; l < 9; l++) {
        func_神雷灭世(dst_pos, l);
        if (l % 3 == 2)
        {
          pk::wait(1000);
        }
      }

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 10);

      pk::say(pk::encode("呼风唤雨，驱雷策电!道施所向，皆由我控!"), person_张角);

      // pk::wait(100);

      pk::say(pk::encode("黄天既覆，苍生何存......"), person_张角);
      减少武将寿命(src_leader, random(3, 10));
      src_leader.update();

      src_unit.action_done = true;
      if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
        src_unit.action_done = false;

      return true;
    }

    // ----------- 黄天泰平 ----------
    string getDesc_神术_黄天泰平()
    {
      if (src_unit.energy < 计略气力消耗_黄天泰平)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("使用黄天泰平.(气力至少 {} 以上, 本部队混乱，蛊惑3格内部队)", 计略气力消耗_黄天泰平));
    }

    bool isVisible_神术_黄天泰平()
    {
      return src_leader.get_id() == person_张角.get_id() or src_leader.get_id() == person_张宝.get_id() or src_leader.get_id() == person_张梁.get_id();
    }

    bool isEnabled_神术_黄天泰平()
    {
      if (src_unit.energy < 计略气力消耗_黄天泰平) return false;
      else if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void func_黄天泰平(pk::point src_pos, int range) {
      array<pk::point> arr = pk::range(src_pos, range, range);
      for (int l = 0; l < int(arr.length); l++)
      {

        pk::create_effect(0x3a, arr[l]); // 妖术动画

        pk::unit@ dst = pk::get_unit(arr[l]);
        if (dst !is null) {
          if (!pk::is_enemy(src_unit, dst)) continue;
          if (dst.has_skill(特技_仁政)) {
            pk::create_effect(0x55, arr[l]);
            pk::say(pk::encode("夫仁政，必自经界始"), pk::get_person(dst.who_has_skill(特技_仁政)));
            continue;
          }
          for (int m = 0; m < 3; m++)
          {
            if (pk::is_valid_person_id(dst.member[m]))
            {
              pk::person@ member_t = pk::get_person(dst.member[m]);  //隊伍中的武將
              if (member_t is null || !pk::is_alive(member_t)) continue;
              if (member_t.mibun == 身份_君主)
              {
                // 君主，万分之一的概率直接收编
                if (ch::rand_bool_10000(1))
                {
                  pk::say(pk::encode("黄天有道，吾愿降。。。"), member_t);
                  pk::force@ king_force = pk::get_force(member_t.get_force_id());
                  pk::force@ attack_force = pk::get_force(src_leader.get_force_id());
                  pk::wait(1000);
                  pk::kill(king_force, true, attack_force);
                }
                continue;
              }
              if (member_t.mibun == 身份_都督)
              {
                // 都督，千分之一的概率直接收编
                if (ch::rand_bool_1000(1))
                {
                  pk::say(pk::encode("黄天有道，吾愿降。。。"), member_t);
                  // 历史日志(dst, '神术_黄天泰平', '的蛊惑，投靠');
                  int member_district_id = member_t.get_district_id();
                  int attack_district_id = src_leader.get_district_id();

                  int member_force = member_t.get_force_id();
                  pk::district@ member_district = pk::get_district(member_district_id);

                  auto building_list = pk::get_building_list();
                  if (0 < building_list.count)
                  {
                    for (int j = 0; j < int(building_list.count); j++)
                    {
                      pk::building@ building_0 = building_list[j];
                      if (building_0.get_district_id() == member_district_id)
                      {
                        if (building_0.facility > 2)
                        {
                          building_0.init_force = src_leader.get_force_id();
                          building_0.update();
                        }
                        if (building_0.facility == 2)
                        {
                          pk::port@ port0 = pk::get_port(j - 52);
                          port0.district = attack_district_id;
                          port0.update();
                        }
                        if (building_0.facility == 1)
                        {
                          pk::gate@ gate0 = pk::get_gate(j - 42);
                          gate0.district = attack_district_id;
                          gate0.update();
                        }
                        if (building_0.facility == 0)
                        {
                          pk::city@ city0 = pk::get_city(j);
                          city0.district = attack_district_id;
                          city0.update();
                        }
                      }
                    }
                  }
                  auto person_list = pk::get_person_list(member_district, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般));
                  if (0 < person_list.count)
                  {
                    for (int j = 0; j < int(person_list.count); j++)
                    {
                      pk::person@ person_0 = person_list[j];
                      // person_0.district = attack_district_id;
                      pk::set_district(person_0, attack_district_id);
                      if (person_0.mibun == 身份_都督)
                      {
                        person_0.mibun = 身份_一般;
                      }
                      if (person_0.order == 武将任务_亲善 or person_0.order == 武将任务_同盟 or person_0.order == 武将任务_摒弃 or person_0.order == 武将任务_停战协定 or person_0.order == 武将任务_劝降 or person_0.order == 武将任务_交换俘虏 or person_0.order == 武将任务_请求援军 or person_0.order == 武将任务_二虎竞食 or person_0.order == 武将任务_二虎竞食)
                      {
                        person_0.order = 武将任务_返回;
                        person_0.target[0] = person_0.service;
                      }
                      person_0.update();
                    }
                  }
                  auto unit_list = pk::get_unit_list(member_district);
                  if (0 < unit_list.count)
                  {
                    for (int j = 0; j < int(unit_list.count); j++)
                    {
                      pk::unit@ unit_0 = unit_list[j];
                      unit_0.update();
                    }
                  }

                }
                continue;
              }
              if (pk::rand_bool(5))
              { // 5%概率武将直接叛变
                历史日志(member_t, '神术_黄天泰平', '的蛊惑，投靠别的势力了');
                pk::set_district(member_t, src_leader.get_district_id());
                member_t.mibun = 身份_一般;
              }
              else if (pk::rand_bool(20))
              {
                // 20% 概率下野
                历史日志(member_t, '神术_黄天泰平', '的蛊惑，下野了');
                switch (pk::rand(4))
                {
                  case 0: pk::say(pk::encode("良禽折木而栖"), member_t); break;
                  case 1: pk::say(pk::encode("果然还是不适合"), member_t); break;
                  case 2: pk::say(pk::encode("再见了..."), member_t); break;
                  case 3: pk::say(pk::encode("世界那么大，我想去看看"), member_t); break;
                }

                if (m == 0) // 主将在野，部队解散
                {
                  pk::kill(dst);
                }
                pk::resign(member_t);
                member_t.update();
              }
              else if (pk::rand_bool(50))
              {
                // 50% 概率减忠1~3
                pk::add_loyalty(member_t, -random(1, 3));
                pk::say(pk::encode("君主大人好像也不是很好。。。"), member_t);
              }
            }
          }

        }
      }
    }

    bool handler_神术_黄天泰平(pk::point dst_pos)
    {

      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      for (int l = 0; l < 3; l++) {
        func_黄天泰平(dst_pos, l);
      }

      pk::add_energy(src_unit, -计略气力消耗_黄天泰平, true);

      pk::set_status(src_unit, null, 部队状态_混乱, 1, true); // 自己眩晕一回合

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("救苍生于水火，平天下于乱世!"), src_leader);

      减少武将寿命(src_leader, random(5, 10));
      src_leader.update();

      src_unit.action_done = true;
      if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
        src_unit.action_done = false;

      return true;
    }

    // ----------- 天下归心 ----------
    string getDesc_神术_天下归心()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("使用天下归心.(气力至少 {} 以上, 本部队伪报，俘虏3格内部队的女性武将)", 计略气力消耗_通用));
    }

    bool isEnabled_神术_天下归心()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      else if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void func_天下归心(pk::point src_pos, int range) {
      array<pk::point> arr = pk::range(src_pos, range, range);
      for (int l = 0; l < int(arr.length); l++)
      {

        pk::create_effect(0x3a, arr[l]); // 妖术动画

        pk::unit@ dst = pk::get_unit(arr[l]);
        if (dst !is null) {
          if (!pk::is_enemy(src_unit, dst)) continue;
          if (dst.has_skill(特技_内助)) {
            pk::create_effect(0x55, arr[l]);
            pk::say(pk::encode("内助，非细事也"), pk::get_person(dst.who_has_skill(特技_内助)));
            continue;
          }
          for (int m = 0; m < 3; m++)
          {
            if (pk::is_valid_person_id(dst.member[m]))
            {
              pk::person@ member_t = pk::get_person(dst.member[m]);  //隊伍中的武將
              if (member_t is null || !pk::is_alive(member_t) || member_t.mibun == 身份_君主) continue;

              int rang = 0;
              if (member_t.sex == 性别_女)
              {
                rang += 20;
                if (member_t.spouse != -1) { // 已有配偶
                  rang += 40;
                }
                if (pk::rand_bool(rang)) {
                  历史日志(member_t, '神术_天下归心', '被俘虏了');
                  member_t.former_force = member_t.get_force_id();
                  pk::set_district(member_t, (pk::get_person(pk::get_kunshu_id(member_t))).get_district_id());
                  member_t.mibun = 身份_俘虏;
                  member_t.location = src_unit.get_id();
                  pk::building@ building = pk::get_building(pk::get_service(src_unit));
                  pk::set_service(member_t, building.get_id());
                  pk::say(pk::encode("恶贼！"), member_t);
                  if (m == 0) // 主将被俘，部队解散
                  {
                    pk::kill(dst);
                  }
                  else
                  {
                    if (m == 1) {
                      dst.member[1] = dst.member[2];
                      dst.member[2] = -1;
                      m -= 1;
                    }
                    if (m == 2) {
                      dst.member[2] = -1;
                    }
                    pk::update_unit(dst);
                  }
                  if (member_t.spouse != -1) {
                    pk::person@ target = pk::get_person(member_t.spouse);
                    member_t.spouse = -1;
                    target.spouse = -1;

                    member_t.update();
                    target.update();

                    pk::say(pk::encode("曹贼我与你势不两立！"), target);
                    pk::add_dislike(target, person_曹操.get_id());
                  }
                }
              }
            }
          }

        }
      }
    }

    bool handler_神术_天下归心(pk::point dst_pos)
    {

      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      for (int l = 0; l < 3; l++) {
        func_天下归心(dst_pos, l);
        // pk::wait(1000);
      }

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::set_status(src_unit, null, 部队状态_伪报, 1, true); // 自己眩晕一回合

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("汝妻子吾自养之，汝勿虑也"), src_leader);

      // pk::wait(100);
      减少武将寿命(src_leader, random(5, 10));
      src_leader.update();

      src_unit.action_done = true;
      if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
        src_unit.action_done = false;

      return true;
    }

    // ----------------- 神鬼奇谋 ---------------
    string getDesc_神术_神鬼奇谋()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("消耗气力{}，距离最近十只地方部队眩晕，最近十只已方部队回兵", 计略气力消耗_通用));
    }

    bool isEnabled_神术_神鬼奇谋()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      else if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void func_神鬼奇谋_处理敌方部队(int max_unit = 10)
    {
      array<pk::point> target_unit_list;
      array<pk::point> arr = pk::range(src_pos_, 1, 500);
      for (int i = 0; i < int(arr.length); i++)
      {
        if (int(target_unit_list.length) < max_unit)
        {
          pk::point dst_pos = arr[i];
          if (pk::is_valid_pos(dst_pos)) {
            pk::unit@ dst = pk::get_unit(dst_pos);
            if (dst !is null and dst.get_force_id() != src_unit.get_force_id())
            {
              target_unit_list.insertLast(dst_pos);
            }
          }
        } else
        {
          break;
        }
      }

      for (int i = 0; i < int(target_unit_list.length); i++)
      {
        pk::point dst_pos = target_unit_list[i];
        if (pk::is_valid_pos(dst_pos)) {
          pk::unit@ dst = pk::get_unit(dst_pos);
          pk::set_status(dst, src_unit, 部队状态_混乱, 2 + pk::rand(3), true); // 混乱2~5回合
          历史日志(dst, '神术_神鬼奇谋', '陷入混乱');
          if (int(dst.troops) > int(0.5 * pk::get_max_troops(dst))) { // 兵力超过最大兵力一半
            ch::add_troops(dst, -int(0.5 * pk::get_max_troops(dst)), true);
            历史日志(dst, '神术_神鬼奇谋', '受到了巨大伤害');
          }
          pk::say(pk::encode("这，这是发生什么"), pk::get_person(dst.leader));
        }
      }
    }

    void func_神鬼奇谋_处理已方部队(int max_unit = 10)
    {

      array<pk::point> target_unit_list;
      array<pk::point> arr = pk::range(src_pos_, 1, 500);
      for (int i = 0; i < int(arr.length); i++)
      {
        if (int(target_unit_list.length) < max_unit)
        {
          pk::point dst_pos = arr[i];
          if (pk::is_valid_pos(dst_pos)) {
            pk::unit@ dst = pk::get_unit(dst_pos);
            if (dst !is null and dst.get_force_id() == src_unit.get_force_id())
            {
              target_unit_list.insertLast(dst_pos);
            }
          }
        } else
        {
          break;
        }
      }

      for (int i = 0; i < int(target_unit_list.length); i++)
      {
        pk::point dst_pos = target_unit_list[i];
        if (pk::is_valid_pos(dst_pos)) {
          pk::unit@ dst = pk::get_unit(dst_pos);
          pk::add_energy(dst, 5 + pk::rand(10), true);
          if (int(dst.troops) < int(0.5 * pk::get_max_troops(dst))) { // 兵力小于最大兵力一半
            ch::add_troops(dst, int(0.5 * pk::get_max_troops(dst)), true);
            历史日志(dst, '神术_神鬼奇谋', '补充了大量兵士');
          }
          pk::say(pk::encode("先生真乃世之奇士也"), pk::get_person(dst.leader));
        }
      }
    }

    void func_神鬼奇谋()
    {
      func_神鬼奇谋_处理敌方部队(10);
      func_神鬼奇谋_处理已方部队(10);
    }

    bool handler_神术_神鬼奇谋(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      func_神鬼奇谋();

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("策谋本天成，妙手偶得之!"), src_leader);

      减少武将寿命(src_leader, random(0, 1));
      src_leader.update();

      src_unit.action_done = true;
      if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
        src_unit.action_done = false;

      return true;
    }


    // --------------- 妄尊仲帝 ---------------
    string getText_神术_妄尊仲帝()
    {
      return pk::encode(pk::format("妄尊仲帝({})", 妄尊仲帝_启用状态 ? '已开启' : '已关闭'));
    }
    string getDesc_神术_妄尊仲帝()
    {
      return pk::encode("伤害减半，且可用金钱代替（可关闭）。每回合回兵势力数%");
    }

    bool isEnabled_神术_妄尊仲帝()
    {
      return true;
    }

    bool handler_神术_妄尊仲帝(pk::point dst_pos)
    {
      // pk::play_se(120);
      // pk::special_cutin(126,1000); // 妖术遮罩
      妄尊仲帝_启用状态 = !妄尊仲帝_启用状态;

      return true;
    }

    int 神术_妄尊仲帝_伤害处理(pk::unit@ unit, int value, int rettype)
    {
      int troops = value;
      if (troops < 0 and (rettype == 14 or rettype == 100))
      {
        if (unit.gold > 0)
        {
          troops = int(0.5f * troops);
          pk::add_gold(unit, troops, true);
          troops += unit.gold;
          troops = troops > 0 ? 0 : troops;
          // pk::wait(300);
          pk::say(pk::encode("大胆！冒犯天威，大逆不道！！"), person_袁术);
          历史日志(unit, '神术_妄尊仲帝', '减少了大量伤害');
        }
        else
        {
          pk::say(pk::encode("可恶！就差……一步了……"), person_袁术);
        }
      }
      return troops;
    }

    void 神术_妄尊仲帝_补兵处理()
    {
      pk::unit@ 袁术部队 = pk::get_unit(pk::hex_object_id_to_unit_id(person_袁术.location));
      if (袁术部队 !is null)
      {
        int troops = int(pk::get_force_list().count * 袁术部队.troops / 100);
        ch::add_troops(袁术部队, troops, true);
        pk::say(pk::encode("乱世之中，必出枭雄！哈哈哈"), person_袁术);
        历史日志(袁术部队, '神术_妄尊仲帝', '补充了大量兵士');
      }
    }

    // --------- 神鬼八阵 -----------
    string getDesc_神术_神鬼八阵()
    {
      const bool use = bool(pk::load(KEY_神鬼八阵_回合数, 0, false));
      if (use)
        return pk::encode("本月已经使用过.");
      else if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode("每月限一次,3格内友军本旬获得一击必杀能力，敌军眩晕3~8旬");
    }

    bool isEnabled_神术_神鬼八阵()
    {
      const bool use = bool(pk::load(KEY_神鬼八阵_回合数, 0, false));
      if(use) return false;
      return true;
    }

    bool handler_神术_神鬼八阵(pk::point dst_pos)
    {
      pk::store(KEY_神鬼八阵_回合数, 0, true);
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      pk::force@ force = pk::get_force(src_unit.get_force_id());
      pk::list<pk::unit@> target_unit_list = getTargets_神术_目标部队(3);
      for (int i = 0; i < target_unit_list.count; i++)
      {
        pk::unit@ target_unit = target_unit_list[i];
        if (target_unit.get_force_id() == src_unit.get_force_id())
        {
          历史日志(target_unit, '神术_神鬼八阵', '获得一击必杀能力');
          pk::set_status(target_unit, src_unit, 部队状态_引诱, 1, true);
          pk::store(KEY_神鬼八阵_一击必杀, KEY_神鬼八阵_一击必杀_索引_武将起始 + target_unit.get_id(), true);
        }
        else
        {
          pk::set_status(target_unit, src_unit, 部队状态_混乱, 3 + pk::rand(5), true);
        }
      }
      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 50);

      pk::say(pk::encode("奇正相生，循环无端!"), src_leader);

      return true;

    }

    void 神术_神鬼八阵_伤害处理(pk::damage_info& info, pk::unit@ attacker, const pk::point& in target_pos)
    {
      const bool has_effect = bool(pk::load(KEY_神鬼八阵_一击必杀, KEY_神鬼八阵_一击必杀_索引_武将起始 + attacker.get_id(), false));
      if (has_effect)
      {
        pk::building@ building = pk::get_building(target_pos);
        if (building !is null and building.facility != 设施_城市 and building.facility != 设施_关隘 and building.facility != 设施_港口)
        {
          info.hp_damage = building.hp;
        }
        else
        {
          info.troops_damage = info.dst_troops;
        }
      }
    }


    // ----------- 狼顾权变 --------------------
    string getDesc_神术_狼顾权变()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (getTargets_狼顾权变_目标部队位置(3).length == 0)
        return pk::encode("周围没有目标.");
      else
        return pk::encode("每回合限一次,临近3格内敌方武将特技失效3回合,并可令已方一名武将习得其中一个特技。");
    }

    bool isEnabled_神术_狼顾权变()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      if (getTargets_狼顾权变_目标部队位置(3).length == 0) return false;
      return true;
    }

    pk::array<pk::point_int> getTargets_狼顾权变_目标部队位置(int range)
    {
        pk::array<pk::point_int> targets;
        array<pk::point> rings = pk::range(src_pos_, 1, range);
        for (int i = 0; i < int(rings.length); i++)
        {
          pk::point dst_pos = rings[i];
          pk::unit@ dst = pk::get_unit(dst_pos);
          if (dst !is null and dst.get_force_id() != src_unit.get_force_id())
          {
            array<string> skill_names = get_skill_name(dst);
            if (skill_names.length > 0)
            {
              targets.insertLast(pk::point_int(dst_pos, 1));
            }

          }
        }

        return targets;
    }

    array<int> get_skill_id(pk::unit@ unit)
    {
      array<pk::point> rings = pk::range(unit.get_pos(), 1, 3);
      array<int> unit_ids;
      for (int i = 0; i < int(rings.length); i++)
      {
        pk::point target_pos = rings[i];
        pk::unit@ dst = pk::get_unit(target_pos);
        if (dst !is null && dst.get_force_id() != src_unit.get_force_id())
        {
          for (int m = 0; m < 3; m++)
          {
            if (pk::is_valid_person_id(dst.member[m]))
            {
              pk::person@ member_t = pk::get_person(dst.member[m]);
              if (member_t is null || !pk::is_alive(member_t)) continue;
              if (member_t.skill >= 0) unit_ids.insertLast(member_t.skill);
            }
          }
        }
      }

      return unit_ids;
    }

    array<string> get_skill_name(pk::unit@ unit)
    {
      array<string> skill_names;
      for (int m = 0; m < 3; m++)
      {
        if (pk::is_valid_person_id(unit.member[m]))
        {
          pk::person@ member_t = pk::get_person(unit.member[m]);
          if (member_t is null || !pk::is_alive(member_t)) continue;
          if (member_t.skill >= 0)
          {
            pk::skill@ skill = pk::get_skill(member_t.skill);
            skill_names.insertLast(skill.name);
          }
        }
      }
      return skill_names;
    }

    void clear_skill(pk::unit@ unit)
    {
      array<pk::point> rings = pk::range(unit.get_pos(), 1, 3);
      for (int i = 0; i < int(rings.length); i++)
      {
        pk::point target_pos = rings[i];
        pk::unit@ dst = pk::get_unit(target_pos);
        if (dst !is null && dst.get_force_id() != src_unit.get_force_id())
        {
          for (int m = 0; m < 3; m++)
          {
            if (pk::is_valid_person_id(dst.member[m]))
            {
              pk::person@ member_t = pk::get_person(dst.member[m]);
              if (member_t is null || !pk::is_alive(member_t)) continue;
              if (member_t.skill >= 0)
              {
                int times = int(pk::load(KEY_狼顾权变_失去技能回合数, member_t.get_id(), 0));
                pk::store(KEY_狼顾权变_失去技能回合数, member_t.get_id(), times + 3);
                pk::store(KEY_狼顾权变_失去技能, member_t.get_id(), member_t.skill);
                pk::set_skill(member_t, -1);
                pk::say(pk::encode("我，我这是怎么了！！!"), member_t);
                member_t.update();
              }
            }
          }
        }
      }
    }

    int choose_skill(array<int> skill_ids)
    {
        int num = int(skill_ids.length);
        array<string> skill_name_list;
        for (int i = 0; i < pk::min(5, num); ++i)
        {
          pk::skill@ skill = pk::get_skill(skill_ids[i]);
          skill_name_list.insertLast(skill.name);
        }

        if (num == 6) skill_name_list.insertLast(pk::get_skill(skill_ids[num]).name);
        else if (num > 6) skill_name_list.insertLast(pk::encode("下一页"));

        int n = pk::choose(pk::encode("请选择要获得的特技."), skill_name_list);
        if (n == 5 and num != 6) return choose_skill_mid(skill_ids, 1);
        return skill_ids[n];
    }

    int choose_skill_mid(array<int> skill_ids, int page)
    {
        int num = int(skill_ids.length);
        int num2 = (num - 1 - 4 * page);
        if (num2 <= 5)
        {
          //最后一页
          array<string> skill_name_list;
          for (int i = (1 + page * 4); i < (1 + page * 4 + num2); ++i)
          {
            skill_name_list.insertLast(pk::get_skill(skill_ids[i]).name);
          }
          skill_name_list.insertLast(pk::encode("上一页"));
          int n = pk::choose(pk::encode("请选择要获得的特技."), skill_name_list);

          if (n == 5 or n == int(skill_name_list.length - 1))
          {
            if (page == 1) return choose_skill(skill_ids);
            return choose_skill_mid(skill_ids, page - 1);//如何返回上一页
          }

          int t = n + (page - 1) * 4 + 5;

          return skill_ids[t];
        }
        else//不是最后一页
        {
            array<string> skill_name_list;
            for (int i = (1 + page * 4); i < (1 + page * 4 + 4); ++i)
            {
              skill_name_list.insertLast(pk::get_ability(skill_ids[num]).name);
            }
            skill_name_list.insertLast(pk::encode("上一页"));
            skill_name_list.insertLast(pk::encode("下一页"));

            int n = pk::choose(pk::encode("请选择要获得的特技."), skill_name_list);

            //非最后一页情况
            if (n == 4)
            {
              if (page != 1) return choose_skill_mid(skill_ids, page - 1);
              else return choose_skill(skill_ids);
            }
            if (n == 5) return choose_skill_mid(skill_ids, page + 1);

            int t = n + (page - 1) * 4 + 5;

            return skill_ids[t];
        }
    }

    void 神术_狼顾权变_特技处理()
    {
      for (int i = 0; i < 可用武将_末; i++)
      {
        int times = int(pk::load(KEY_狼顾权变_失去技能回合数, i, 0));
        if (times > 1)
        {
          times -= 1;
          pk::store(KEY_狼顾权变_失去技能回合数, i, times);
        } else if (times == 1)
        {
          const int person_skill_id = int(pk::load(KEY_狼顾权变_失去技能, i, 0));
          pk::set_skill(pk::get_person(i), person_skill_id);
          pk::say(pk::encode("总算回复了，恐怖如斯啊！！!"), pk::get_person(i));
        }
      }
    }

    void func_狼顾权变(pk::point dst_pos)
    {

      pk::unit@ unit = pk::get_unit(dst_pos);
      array<int> skill_ids = get_skill_id(unit);
      // array<string> skill_names = get_skill_name(unit);
      clear_skill(unit);
      // pk::wait(1000);
      int skill_id = choose_skill(skill_ids);
      string desc = pk::encode(pk::format("请选择获得特技\x1b[1x{}\x1b的武将", pk::get_skill(skill_id).name));
      auto person_list = pk::get_person_list(src_force, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
      bool person_confirm = false;
      pk::person@ monster;
      string monster_name = "";
      while (!person_confirm)
      {
        // 무장 선택 창을 열어서 선택하기
        auto person_sel = pk::person_selector(pk::encode("选择武将"), desc, person_list, 1, 1);
      if (person_sel.count == 0) return;
        // 무장 선택
        @monster = @person_sel[0];

        // 선택된 무장의 이름
        monster_name = pk::decode(pk::get_name(monster));

        person_confirm = pk::yes_no(pk::encode(pk::format("是否让\x1b[1x{}\x1b[0x特技变为\x1b[1x{}\x1b?", monster_name, pk::get_skill(skill_id).name)));
      }

      pk::say(pk::encode("精晓奇谋，诡变万策!"), src_leader);
      pk::set_skill(monster, skill_id);
      monster.update();
      string mibun_text;
      switch(person_司马懿.mibun)
      {
        case 身份_君主:
          mibun_text = '主公';
        case 身份_都督:
          mibun_text = '都督';
        default:
           mibun_text = '仲达';
      }
      pk::say(pk::encode(pk::format("{}真乃世之奇才也", mibun_text)), monster);

      历史日志(monster, '神术_狼顾权变', pk::format("获得特技\x1b[1x{}\x1b[0x", pk::decode(pk::get_skill(skill_id).name)));
    }

    bool handler_神术_狼顾权变(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩
      pk::wait(1000);
      func_狼顾权变(dst_pos);

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      return true;
    }

    // --------- 神术_火凤连环
    string getDesc_神术_火凤连环()
    {
      const int skill_id = int(pk::load(KEY_火凤连环, 0, 0));
      if (skill_id > 0)
        return pk::encode("本回合已经使用过.");
      else if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else
        return pk::encode("每回合限一次,3格内敌方任一部队受到部队伤害时，其他部队同样受伤。");
    }
    bool isEnabled_神术_火凤连环()
    {
      const int skill_id = int(pk::load(KEY_火凤连环, 0, 0));
      if(skill_id > 0) return false;
      if (src_unit.energy < 计略气力消耗_通用) return false;
      return true;
    }

    void 设置连环状态部队(pk::point dst_pos)
    {
      array<pk::point> rings = pk::range(dst_pos, 0, 3);
      array<int> unit_ids;
      for (int i = 0; i < int(rings.length); i++)
      {
        pk::point target_pos = rings[i];
        pk::unit@ dst = pk::get_unit(target_pos);
        if (dst !is null && dst.get_force_id() != src_unit.get_force_id())
        {
          unit_ids.insertLast(dst.get_id());
        }
      }

      pk::store(KEY_火凤连环, 0, int(unit_ids.length));
      for (int i = 0; i < int(unit_ids.length); i++)
      {
        pk::store(KEY_火凤连环, i + 1, unit_ids[i]);
        pk::unit@ dst = pk::get_unit(unit_ids[i]);
        pk::set_status(dst, src_unit, 部队状态_混乱, pk::rand(2), true);
        历史日志(dst, '神术_火凤连环', '陷入混乱');
      }
    }

    void 火凤连环伤害(pk::unit@ unit, int value, int rettype)
    {
      pk::trace('伤害类型：' + rettype);
      array<int> unit_ids;
      int length = int(pk::load(KEY_火凤连环, 0, 0));
      if (length > 0)
      {
        for (int i = 0; i < length; i++)
        {
          int unit_id = int(pk::load(KEY_火凤连环, i + 1, 10000));
          if (unit_id < 部队_末)
          {
            pk::unit@ dst = pk::get_unit(unit_id);
            if (unit.get_id() != unit_id)
            {
              if (value < 0 and rettype == 14)
              {
                ch::add_troops(dst, value, true);
                历史日志(dst, '神术_火凤连环', '受到了巨大伤害');
                pk::say(pk::encode("该死的，这是怎么回事"), pk::get_person(dst.leader));
                if (dst.troops <= 0) {
                  pk::kill(dst);
                }
              }
            }
          }
        }
      }
    }

    void 火凤连环清除()
    {
      int length = int(pk::load(KEY_火凤连环, 0, 0));
      if (length > 0)
      {
        pk::store(KEY_火凤连环, 0, 0);
        for (int i = 0; i < length; i++)
        {
          pk::store(KEY_火凤连环, i + 1, 10000);
        }
      }
    }

    bool handler_神术_火凤连环(pk::point dst_pos)
    {

      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩
      设置连环状态部队(dst_pos);

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      src_unit.action_done = true;
      if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
        src_unit.action_done = false;
      return true;
    }

    // --------- 神术_乱武完杀 ------------
    string getDesc_神术_乱武完杀()
    {
      int unit_length = get_乱武完杀部队().length;
      if (getEmptyPerson() is null)
        return pk::encode("暂无多余部队.");
      else if (unit_length > 0)
        return pk::encode(pk::format("还有{}支小分队存在", unit_length));
      else if (src_unit.energy < 计略气力消耗_乱武完杀)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode("指定位置，分兵5支100兵力小队。小队被攻击或破坏时，对周围造成巨大伤害");
    }
    bool isEnabled_神术_乱武完杀()
    {
      if (getEmptyPerson() is null) return false;
      if (get_乱武完杀部队().length > 0) return false;
      if (src_unit.energy < 计略气力消耗_乱武完杀) return false;
      if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    bool valid_pos(pk::point dst_pos)
    {
      pk::hex@ hex = pk::get_hex(dst_pos);
      int terrain_id = hex.terrain;
      return pk::is_valid_pos(dst_pos) and !hex.has_building and !hex.has_unit
        and pk::is_valid_terrain_id(terrain_id)
        and pk::is_enabled_terrain(terrain_id)
        and terrain_id != 지형_산 and terrain_id != 地形_岸 and terrain_id != 地形_河 and terrain_id != 地形_川 and terrain_id != 地形_海;
    }

    pk::person@ getEmptyPerson()
    {
      for (int i = 临时武将起始; i <= 临时武将终止; i++)
      {
        pk::person@ new_face = pk::get_person(i);
        switch (new_face.mibun)
        {
        case 身分_无:
        case 身分_未登场:
        case 身分_死亡:
          return new_face;
        }
      }
      return null;
    }

    void setTempPerson(int person_id, int index)
    {
      pk::person@ farmer = pk::get_person(person_id);
      // pk::set_district(farmer, person_贾诩.get_district_id());

      farmer.name_read = pk::encode("贾诩小队");
      farmer.sex = 性别_男;
      farmer.sei = pk::encode("贾");
      farmer.mei = pk::encode(pk::format("诩{}", index));
      farmer.service = 建筑_末 - 1;
      farmer.face = 武将_卫兵;
      farmer.mibun = 身份_一般;
      farmer.loyalty = 100;
      farmer.birth = pk::get_year() - 30;
      farmer.death = pk::min(353, farmer.birth + 99);
      farmer.appearance = pk::get_year() - 10;
      farmer.kouseki = 1000;
      farmer.birthplace = person_贾诩.birthplace;
      farmer.skill = -1;
      farmer.wadai = person_贾诩.wadai;
      farmer.giri = person_贾诩.giri;
      farmer.ambition = person_贾诩.ambition;
      farmer.promotion = person_贾诩.promotion;
      farmer.character = person_贾诩.character;
      farmer.voice = person_贾诩.voice;
      farmer.tone = person_贾诩.tone;
      farmer.kanshitsu = person_贾诩.kanshitsu;
      farmer.strategic_tendency = person_贾诩.strategic_tendency;
      farmer.local_affiliation = person_贾诩.local_affiliation;

      //  ?空亲爱厌恶关系
      for (int i = 0;i < 5;i++)
      {
        farmer.liked[i] = person_贾诩.liked[i];
        farmer.disliked[i] = person_贾诩.disliked[i];
      }

      // 能力?
      for (int i = 0; i < 武将能力_末; i++)
      {
        farmer.base_stat[i] = random(10, 30);
        farmer.stat_aging[i] = random(能力成长_超持续型, 能力成长_张飞型);
      }

      // 适性
      for (int i = 0; i < 兵种_末; i++)
      {
        farmer.tekisei[i] = person_贾诩.tekisei[i];
      }

      // 무장 조형
      farmer.skeleton = person_贾诩.skeleton;
      farmer.body[0] = person_贾诩.body[0];
      farmer.body[1] = person_贾诩.body[1];
      farmer.body[2] = person_贾诩.body[2];
      farmer.body[3] = person_贾诩.body[3];
      farmer.body[4] = person_贾诩.body[4];
      farmer.body[5] = person_贾诩.body[5];

      //  話术
      farmer.wajutsu_daikatsu = person_贾诩.wajutsu_daikatsu;  //大喝
      farmer.wajutsu_kiben = person_贾诩.wajutsu_kiben;     //詭辯
      farmer.wajutsu_mushi = person_贾诩.wajutsu_mushi;     //无視
      farmer.wajutsu_chinsei = person_贾诩.wajutsu_chinsei;   //?静
      farmer.wajutsu_gyakujou = person_贾诩.wajutsu_gyakujou;  //憤怒

      farmer.update();
    }

    array<int> get_乱武完杀部队()
    {
      int max_unit = int(pk::load(KEY_乱武完杀, 0, 0));
      array<int> unit_ids;
      for (int i = 0; i < max_unit; i++)
      {
        int unit_id = int(pk::load(KEY_乱武完杀, i + 1, 10000));
        if (unit_id < 部队_末)
        {
          unit_ids.insertLast(unit_id);
        }
      }
      return unit_ids;
    }
    void remove_乱武完杀部队(int unit_id)
    {
      int max_unit = int(pk::load(KEY_乱武完杀, 0, 0));
      for (int i = 0; i < max_unit; i++)
      {
        int effect_unit_id = int(pk::load(KEY_乱武完杀, i + 1, 10000));
        if (unit_id == effect_unit_id)
        {
          pk::store(KEY_乱武完杀, i + 1, 10000);
        }
      }
    }

    void 乱武完杀处理(pk::point dst_pos, int max_unit)
    {
      array<pk::point> rings = pk::range(dst_pos, 0, 100);
      array<int> unit_ids;
      for (int i = 0; i < int(rings.length); i++)
      {
        if (unit_ids.length >= 5) break;
        pk::point target_pos = rings[i];
        if (!valid_pos(target_pos)) continue;
        if (getEmptyPerson() is null) break;
        pk::person@ temp_person = getEmptyPerson();
        pk::set_district(temp_person, person_贾诩.get_district_id());
        setTempPerson(temp_person.get_id(), unit_ids.length + 1);
        pk::unit@ unit = pk::create_unit(pk::get_building(person_贾诩.service), temp_person, null, null, 100, src_unit.weapon, 兵器_走舸, 0, 100, target_pos);
        unit.energy = 10;
        unit_ids.insertLast(unit.get_id());
        pk::trace('完杀小队' + i + "队：" + unit.get_id());
      }

      pk::store(KEY_乱武完杀, 0, unit_ids.length);
      for (int i = 0; i < int(unit_ids.length); i++)
      {
        pk::store(KEY_乱武完杀, i + 1, unit_ids[i]);
      }
    }

    void 乱武完杀部队受伤处理(pk::unit@ unit, int value, int rettype)
    {
      pk::trace('伤害类型：' + rettype + " 部队：" + unit.get_id());
      if (value < 0)
      {
        if (rettype >= 0 and rettype != 13 and int(unit.troops) > -value)
        {
          array<int> 完杀小队 = get_乱武完杀部队();
          if (完杀小队.find(unit.get_id()) >= 0)
          {
            pk::trace('完杀小队受伤：' + unit.get_id());
            array<pk::point> rings = pk::range(unit.get_pos(), 1, 1);
            for (int i = 0; i < int(rings.length); i++)
            {
              pk::point dst_pos = rings[i];
              pk::unit@ dst = pk::get_unit(dst_pos);
              if (dst !is null and dst.get_force_id() != person_贾诩.get_force_id())
              {
                ch::add_troops(dst, int(random(1, 2) * value), true);
                历史日志(dst, '神术_乱武完杀', '受到了反伤');
                pk::say(pk::encode("这是反伤吗？"), pk::get_person(dst.leader));
                if (dst.troops <= 0) {
                  pk::kill(dst, src_unit);
                }
              }
            }
          }
        }
      }
    }

    void 乱武完杀部队溃灭处理(pk::unit@ unit, pk::hex_object@ hex_object, int type)
    {
      array<int> 完杀小队 = get_乱武完杀部队();
      if (完杀小队.find(unit.get_id()) >= 0)
      {
        pk::trace('溃灭部队：' + unit.get_id());
        if (type == 0)
        {
          array<pk::point> rings = pk::range(unit.get_pos(), 1, 1);
          for (int i = 0; i < int(rings.length); i++)
          {
            pk::point dst_pos = rings[i];
            pk::unit@ dst = pk::get_unit(dst_pos);
            if (dst !is null and dst.get_force_id() != person_贾诩.get_force_id())
            {
              ch::add_troops(dst, -int(random(10, 100) * 100), true);
              历史日志(dst, '神术_乱武完杀', '受到了巨大伤害');
              pk::say(pk::encode("不好，中计了！"), pk::get_person(dst.leader));
              if (dst.troops <= 0) {
                pk::kill(dst, src_unit);
              }
            }
          }
        }
        remove_乱武完杀部队(unit.get_id());
      }
    }
    bool handler_神术_乱武完杀(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      int max_unit = 5;
      乱武完杀处理(dst_pos, max_unit);
      ch::add_troops(src_unit, max_unit * -100, true);

      pk::add_energy(src_unit, -计略气力消耗_乱武完杀, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("吾之所好，杀人诛心。"), src_leader);

      src_unit.action_done = true;
      if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
        src_unit.action_done = false;
      return true;
    }

    // ------------ 神术_奇谋诡策 -----------
    string getDesc_神术_奇谋诡策()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode("令3格内我方部队气力回满，兵力回一半，并且本回合攻击敌方部队时一定概率斩将");
    }
    bool isEnabled_神术_奇谋诡策()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void fun_奇谋诡策(pk::point dst_pos)
    {
      array<pk::point> rings = pk::range(dst_pos, 0, 3);
      for (int i = 0; i < int(rings.length); i++)
      {
        pk::point target_pos = rings[i];
        if (!pk::is_valid_pos(target_pos)) continue;
        pk::unit@ dst = pk::get_unit(target_pos);
        if (dst !is null)
        {
          pk::trace("部队势力：" + dst.get_force_id() + ";法正势力：" + person_法正.get_force_id());
          if (dst.get_force_id() == person_法正.get_force_id())
          {
            // KEY_奇谋诡策
            pk::store(KEY_奇谋诡策, dst.get_id(), true);
            pk::add_energy(dst, int(pk::core["部队.熟练兵最大气力"]), true);
            ch::add_troops(dst, dst.troops, true);
            pk::say(pk::encode("多谢孝直达人"), pk::get_person(dst.leader));
            历史日志(dst, '神术_奇谋诡策', '回复了气力跟兵士');
          }
          else
          {
            pk::set_status(dst, src_unit, 部队状态_伪报, pk::rand(2), true);
            历史日志(dst, '神术_奇谋诡策', '陷入了伪报');
          }
        }
      }
    }

    void 神术_奇谋诡策_伤害处理(pk::unit@ dst_unit, pk::unit@ unit)
    {
      const bool 奇谋诡策部队 = bool(pk::load(KEY_奇谋诡策, unit.get_id(), false));
      if (奇谋诡策部队 and dst_unit !is null)
      {
        for (int m = 0; m < 3; m++)
        {
          if (pk::is_valid_person_id(dst_unit.member[m]))
          {
            const int rand = random(0, 100);
            pk::person@ member_t = pk::get_person(dst_unit.member[m]);
            if (member_t is null || !pk::is_alive(member_t)) continue;
            if (rand <= 50) { // 50%受伤
              if (member_t.shoubyou != 3)
              {
                member_t.shoubyou += 1;
              }
              else if (pk::rand_bool(50))
              {
                pk::kill(member_t, pk::get_person(unit.leader), null, null, 2);
                if (m == 0) pk::kill(dst_unit);
              }
            }
            else if (rand > 50 and rand <= 80 and member_t.mibun != 身份_君主)
            {
              member_t.former_force = member_t.get_force_id();
              // pk::set_district(member_t, (pk::get_person(pk::get_kunshu_id(member_t))).get_district_id());
              member_t.mibun = 身份_俘虏;
              member_t.location = src_unit.get_id();
              pk::building@ building = pk::get_building(pk::get_service(src_unit));
              pk::set_service(member_t, building.get_id());
              pk::say(pk::encode("这是妖术！"), member_t);
              if (m == 0) // 主将被俘，部队解散
              {
                pk::kill(dst_unit);
              }
              else
              {
                if (m == 1) {
                  dst_unit.member[1] = dst_unit.member[2];
                  dst_unit.member[2] = -1;
                  m -= 1;
                }
                if (m == 2) {
                  dst_unit.member[2] = -1;
                }
                pk::update_unit(dst_unit);
              }
            }
            else
            {
              pk::kill(member_t, pk::get_person(unit.leader), null, null, 2);
              if (m == 0) pk::kill(dst_unit);
            }
          }
        }
      }
    }

    void 奇谋诡策清除()
    {
      for (int i = 0; i < 部队_末; i++)
      {
        pk::store(KEY_奇谋诡策, i, false);
      }
    }

    bool handler_神术_奇谋诡策(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      fun_奇谋诡策(dst_pos);

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("虚实不定，避重就轻，以眩远近。"), src_leader);

      src_unit.action_done = true;
      if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
        src_unit.action_done = false;
      return true;
    }
  }
  Main main;
}

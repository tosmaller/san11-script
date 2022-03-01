
/*
作者：黑店小小二
v1.8 2022/03/01 诸葛亮新增神术【神鬼八阵】
v1.7 2022/02/28 袁术新增神术【妄尊仲帝】
v1.6 2022/02/25 郭嘉新增神术【十胜十败】
v1.5 2022/02/22 调整写法，优化代码体积
v1.4 2022/02/19 修复【黄天泰平】劝降君主，都督后的处理
v1.3 2022/02/17 修复对城池耐久归0归属变更bug，张角新增神术【黄天泰平】,调整【神雷灭世】动画效果，曹操新增神术【天下归心】
v1.2 2022/02/14 修复措施耐久归0归属变更bug
v1.1 2022/02/13 修复周瑜神术对关卡跟港口无效bug，张角新增神术【神雷灭世】
v1.0 2022/02/12 新增周瑜神术【业火焚天】

描述：针对高智武将，设置了大招【神术】，旨在增加游戏娱乐性
 */

namespace 人物加强 {
  const int KEY_神鬼八阵 = pk::hash("神术_神鬼八阵");
  const int KEY_神鬼八阵_索引_武将起始 = 0;

  const int 计略气力消耗_通用 = 60;
  const int 兵力条件_通用 = 5000;

  const int 计略气力消耗_黄天泰平 = 100;

  const int 神术_十胜十败_影响部队 = 0;

  pk::func209_t@ prev_callback_;

  array<string> 已影响城市; // 存储处理过的城市

  class Main
  {
    pk::random random(pk::rand());
    Main() {
      add_menu_unit_order();

      @prev_callback_ = cast<pk::func209_t@>(pk::get_func(209));
      pk::reset_func(209);
      pk::set_func(209, pk::func209_t(func209));

      pk::bind(111, pk::trigger111_t(onTurnBegin));
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
    pk::point src_pos_;

    bool 开启_业火焚天 = true; // 周瑜大招
    bool 开启_神雷灭世 = true; // 张角大招
    bool 开启_黄天泰平 = true; // 张角三兄弟大招
    bool 开启_天下归心 = true; // 曹操大招
    bool 开启_十胜十败 = true; // 郭嘉大招
    bool 开启_妄尊仲帝 = true; // 袁术大招
    bool 开启_神鬼八阵 = true; // 诸葛亮大招

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
      if (开启_十胜十败) {
        pk::menu_item 神术_十胜十败;
        神术_十胜十败.menu = 菜单_神术;
        神术_十胜十败.init = pk::unit_menu_item_init_t(init);
        神术_十胜十败.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称("十胜十败", 计略气力消耗_通用); });
        神术_十胜十败.get_desc = pk::menu_item_get_desc_t(getDesc_神术_十胜十败);
        神术_十胜十败.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return main.isVisible_神术_名称(武将_郭嘉); });
        神术_十胜十败.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_十胜十败);
        // 神术_十胜十败.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return main.getTargets_神术_目标(0); });
        神术_十胜十败.handler = pk::unit_menu_item_handler_t(handler_神术_十胜十败);
        pk::add_menu_item(神术_十胜十败);
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
        神术_神鬼八阵.handler = pk::unit_menu_item_handler_t(handler_神术_神鬼八阵);
        pk::add_menu_item(神术_神鬼八阵);
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
        person_诸葛亮.get_id()
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

    pk::array<pk::point_int> getTargets_神术_目标部队(int range)
    {
        pk::array<pk::point_int> targets;
        array<pk::point> rings = pk::range(src_pos_, 1, range);
        for (int i = 0; i < int(rings.length); i++)
        {
          pk::point dst_pos = rings[i];
          pk::unit@ dst = pk::get_unit(dst_pos);
          if (dst !is null and dst.get_force_id() != src_unit.get_force_id())
          {
            targets.insertLast(pk::point_int(dst_pos, 1));
          }
        }

        return targets;
    }

    pk::array<pk::point_int> getTargets_势力_目标部队(pk::force@ force, int range)
    {
        pk::array<pk::point_int> targets;
        array<pk::point> rings = pk::range(src_pos_, 1, range);
        for (int i = 0; i < int(rings.length); i++)
        {
          pk::point dst_pos = rings[i];
          pk::unit@ dst = pk::get_unit(dst_pos);
          if (dst !is null and dst.get_force_id() == force.get_force_id())
          {
            targets.insertLast(pk::point_int(dst_pos, 1));
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

    void func209(pk::damage_info& info, pk::unit@ attacker, int tactics_id, const pk::point& in target_pos, int type, int critical, bool ambush, int rettype)
    {

      pk::unit@ dst = pk::get_unit(target_pos);
      if (dst !is null) {
        switch(dst.leader)
        {
          case 武将_袁术:
            神术_妄尊仲帝_伤害处理(info);
            break;
        }
      }
      prev_callback_(info, attacker, tactics_id, target_pos, type, critical, ambush, rettype);
    }

    void onTurnStart(pk::force@ force)
    {
      switch(force.get_force_id())
      {
        case 武将_袁术.get_force_id():
          神术_妄尊仲帝_补兵处理();
          break;
      }
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
          pk::set_status(dst, dst, 部队状态_伪报, pk::rand(2) + 3, true);
          pk::create_fire(arr[l], pk::rand(2) + 3, dst, true); //火计
          pk::add_energy(dst, -30, true); //减气
          ch::add_troops(dst, -random(500, 1500), true); // 随机减兵500~1500
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
          if (pk::rand_bool(50)) { // 50%概率着火
            pk::create_fire(arr[l], pk::rand(2), dst, true); //火计
          }
          pk::add_energy(dst, -30, true); //减气
          ch::add_troops(dst, -(部队落雷伤害 + pk::rand(部队落雷伤害_1) + pk::rand(2000)), true); // 在雷电伤害基础上随机加2000
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
          fun_处理城市(building, arr[l], hp_damage);
          building.update();
        }

        if (dst is null and building is null) {
          if (pk::rand_bool(50)) { // 50%概率着火
            pk::create_fire(arr[l], pk::rand(2), dst, true); //火计
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
                pk::set_district(member_t, src_leader.get_district_id());
                member_t.mibun = 身份_一般;
              }
              else if (pk::rand_bool(20))
              {
                // 20% 概率下野
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

    // ----------------- 十胜十败 ---------------
    string getDesc_神术_十胜十败()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("消耗气力{}，距离最近十只地方部队眩晕，最近十只已方部队回兵", 计略气力消耗_通用));
    }

    bool isEnabled_神术_十胜十败()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      else if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void fun_十胜十败_处理敌方部队(int range)
    {
      int max_unit = 10;
      pk::force@ force = pk::get_force(src_unit.get_force_id());
      array<pk::unit@> force_unit_list = pk::list_to_array(pk::get_unit_list(force));
      array<pk::unit@> target_unit_list = pk::list_to_array(pk::get_unit_list());
      if ((int(target_unit_list.length) - int(force_unit_list.length)) < max_unit)
      {
        for (int i = 0; i < int(target_unit_list.length); i++)
        {
          pk::unit@ target_unit = target_unit_list[i];
          if (target_unit.get_force_id() != src_unit.get_force_id()) {
            pk::set_status(target_unit, src_unit, 部队状态_混乱, 2 + pk::rand(3), true); // 混乱2~5回合
            if (int(target_unit.troops) > int(0.5 * pk::get_max_troops(target_unit))) { // 兵力超过最大兵力一半
              ch::add_troops(target_unit, -int(0.5 * pk::get_max_troops(target_unit)), true);
            }
            pk::say(pk::encode("这，这是发生什么"), pk::get_person(target_unit.leader));
          }
        }
      }
      else
      {
        array<pk::point> arr = pk::range(src_pos_, range, range);
        for (int i = 0; i < int(arr.length); i++)
        {
          pk::point dst_pos = arr[i];
          if (!pk::is_valid_pos(dst_pos)) {
            max_unit = 0;
            return;
          }
          pk::unit@ dst = pk::get_unit(dst_pos);
          if (dst !is null and dst.get_force_id() != src_unit.get_force_id() and max_unit > 0)
          {
            pk::set_status(dst, src_unit, 部队状态_混乱, 2 + pk::rand(3), true); // 混乱2~5回合
            if (int(dst.troops) > int(0.5 * pk::get_max_troops(dst))) { // 兵力超过最大兵力一半
              ch::add_troops(dst, -int(0.5 * pk::get_max_troops(dst)), true);
            }
            pk::say(pk::encode("这，这是发生什么"), pk::get_person(dst.leader));
            max_unit -= 1;
          }
        }
        if (max_unit > 0)
        {
          fun_十胜十败_处理敌方部队(++range);
        }
      }
    }

    void fun_十胜十败_处理已方部队(int range)
    {
      int max_unit = 10;
      pk::force@ force = pk::get_force(src_unit.get_force_id());
      array<pk::unit@> force_unit_list = pk::list_to_array(pk::get_unit_list(force));
      if (int(force_unit_list.length) < max_unit)
      {
        for (int i = 0; i < int(force_unit_list.length); i++)
        {
          pk::unit@ target_unit = force_unit_list[i];
          // pk::set_status(target_unit, src_unit, 部队状态_混乱, 2 + pk::rand(3), true);
          pk::add_energy(target_unit, 5 + pk::rand(10), true);
          if (int(target_unit.troops) < int(0.5 * pk::get_max_troops(target_unit))) { // 兵力超过最大兵力一半
            ch::add_troops(target_unit, int(0.5 * pk::get_max_troops(target_unit)), true);
          }
          pk::say(pk::encode("先生真乃世之奇士也"), pk::get_person(target_unit.leader));
        }
      }
      else
      {
        array<pk::point> arr = pk::range(src_pos_, range, range);
        for (int i = 0; i < int(arr.length); i++)
        {
          pk::point dst_pos = arr[i];
          if (!pk::is_valid_pos(dst_pos)) {
            max_unit = 0;
            return;
          }
          pk::unit@ dst = pk::get_unit(dst_pos);
          if (dst !is null and dst.get_force_id() != src_unit.get_force_id() and max_unit > 0)
          {
            pk::set_status(dst, src_unit, 部队状态_混乱, 2 + pk::rand(3), true); // 混乱2~5回合
            if (int(dst.troops) > int(0.5 * pk::get_max_troops(dst))) { // 兵力超过最大兵力一半
              ch::add_troops(dst, -int(0.5 * pk::get_max_troops(dst)), true);
            }
            pk::say(pk::encode("这，这是发生什么"), pk::get_person(dst.leader));
            max_unit -= 1;
          }
        }
        if (max_unit > 0)
        {
          fun_十胜十败_处理已方部队(++range);
        }
      }
    }

    void func_十胜十败()
    {
      fun_十胜十败_处理敌方部队(1);
      fun_十胜十败_处理已方部队(1);
    }

    bool handler_神术_十胜十败(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      func_十胜十败();

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
      pk::say(pk::encode("策谋本天成，妙手偶得之!"), src_leader);

      return true;
    }

    void 神术_妄尊仲帝_伤害处理(pk::damage_info& info)
    {
      info.troops_damage = int(0.5 * info.troops_damage);
      if (妄尊仲帝_启用状态)
      {
        if (dst.gold > 0)
        {
          int troops_damage = info.troops_damage;
          info.troops_damage -= dst.gold;
          pk::wait(300);
          pk::add_gold(dst, -troops_damage, true);
          pk::say(pk::encode("大胆！冒犯天威，大逆不道！！"), person_袁术);
        }
        else
        {
          pk::say(pk::encode("可恶！就差……一步了……"), person_袁术);
        }
      }
      else
      {
        pk::say(pk::encode("我袁家人何以至此！"), src_leader);
      }
    }

    void 神术_妄尊仲帝_补兵处理()
    {
      auto force_list = pk::list_to_array(pk::get_force_list());
      int troops = int(force_list.length / 100 * src_unit.troops);
      pk::unit 袁术部队 = pk::get_unit(person_袁术.location);
      ch::add_troops(袁术部队, troops, true);
      pk::say(pk::encode("乱世之中，必出枭雄！哈哈哈"), person_袁术);
    }

    // --------- 神鬼八阵 -----------

    string getText_神术_神鬼八阵()
    {
      return pk::encode(pk::format("神鬼八阵"));
    }
    string getDesc_神术_神鬼八阵()
    {
      return pk::encode("伤害50%为0，已方3格内部队本回合获得一击必杀能力");
    }

    bool isEnabled_神术_神鬼八阵()
    {
      return true;
    }

    bool handler_神术_神鬼八阵(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      pk::force@ force = pk::get_force(src_unit.get_force_id());
      pk::list<pk::unit@> target_unit_list = getTargets_势力_目标部队(force, 3);
      for (int i = 0; i < int(target_unit_list.length); i++)
      {
        pk::unit@ target_unit = target_unit_list[i];
        pk::store(KEY_神鬼八阵, KEY_神鬼八阵_索引_武将起始 + i + 1, target_unit_list_id);
      }
      pk::say(pk::encode("八阵图!"), src_leader);

      return true;

    }

    void 神术_神鬼八阵_伤害处理(pk::damage_info& info)
    {
      if (开启_神鬼八阵)
      {
        if (pk::rand_bool(50)) {
          info.troops_damage = 0;
        }
      }

    }

  }
  Main main;
}


/*
作者：黑店小小二
v1.5 2022/02/22 调整写法，优化代码体积
v1.4 2022/02/19 修复【黄天泰平】劝降君主，都督后的处理
v1.3 2022/02/17 修复对城池耐久归0归属变更bug，张角新增神术【黄天泰平】,调整【神雷灭世】动画效果，曹操新增神术【天下归心】
v1.2 2022/02/14 修复措施耐久归0归属变更bug
v1.1 2022/02/13 修复周瑜神术对关卡跟港口无效bug，张角新增神术【神雷灭世】
v1.0 2022/02/12 新增周瑜神术【业火焚天】
 */

namespace 人物加强 {
  const int 计略气力消耗_通用 = 30;
  const int 兵力条件_通用 = 5000;

  const int 计略气力消耗_业火焚天 = 60;
  const int 计略气力消耗_神雷灭世 = 60;
  const int 计略气力消耗_黄天泰平 = 100;
  const int 计略气力消耗_天下归心 = 60;

  array<string> 已影响城市; // 存储处理过的城市

  class Main
  {
    pk::random random(pk::rand());
    Main() {
      add_menu_unit_order();
    }

    pk::unit@ src_unit;
    pk::person@ src_leader;
    pk::person@ person_周瑜;
    pk::person@ person_张角;
    pk::person@ person_张宝;
    pk::person@ person_张梁;
    pk::person@ person_曹操;
    pk::point src_pos_;

    bool 开启_业火焚天 = true; // 周瑜大招
    bool 开启_神雷灭世 = true; // 张角大招
    bool 开启_黄天泰平 = true; // 张角三兄弟大招
    bool 开启_天下归心 = true; // 曹操大招
    bool 开启_七进七出 = true; // 赵云大招

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
        神术_业火焚天.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称(pk::encode("业火焚天"), 计略气力消耗_通用); });
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
        神术_神雷灭世.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_名称(pk::encode("神雷灭世"), 计略气力消耗_通用); });
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
        神术_黄天泰平.get_text = pk::menu_item_get_text_t(getText_神术_黄天泰平);
        神术_黄天泰平.get_desc = pk::menu_item_get_desc_t(getDesc_神术_黄天泰平);
        神术_黄天泰平.is_visible = pk::menu_item_is_visible_t(isVisible_神术_黄天泰平);
        神术_黄天泰平.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_黄天泰平);
        神术_黄天泰平.get_targets = pk::unit_menu_item_get_targets_t(getTargets_神术_黄天泰平);
        神术_黄天泰平.handler = pk::unit_menu_item_handler_t(handler_神术_黄天泰平);
        pk::add_menu_item(神术_黄天泰平);
      }
      if (开启_天下归心) {
        pk::menu_item 神术_天下归心;
        神术_天下归心.menu = 菜单_神术;
        神术_天下归心.init = pk::unit_menu_item_init_t(init);
        神术_天下归心.get_text = pk::menu_item_get_text_t(getText_神术_天下归心);
        神术_天下归心.get_desc = pk::menu_item_get_desc_t(getDesc_神术_天下归心);
        神术_天下归心.is_visible = pk::menu_item_is_visible_t(isVisible_神术_天下归心);
        神术_天下归心.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_天下归心);
        神术_天下归心.get_targets = pk::unit_menu_item_get_targets_t(getTargets_神术_天下归心);
        神术_天下归心.handler = pk::unit_menu_item_handler_t(handler_神术_天下归心);
        pk::add_menu_item(神术_天下归心);
      }
    }

    void init(pk::unit@ unit, pk::point src_pos)
    {
      @src_unit = @unit;
      @src_leader = pk::get_person(src_unit.leader);
      @person_周瑜 = pk::get_person(武将_周瑜);
      @person_张角 = pk::get_person(武将_张角);
      @person_张宝 = pk::get_person(武将_张宝);
      @person_张梁 = pk::get_person(武将_张梁);
      @person_曹操 = pk::get_person(武将_曹操);
      src_pos_ = src_pos;
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

    bool isVisible_神术()
    {
      if (pk::is_campaign()) return false;
      array<int> 神术技能武将= {
        person_周瑜.get_id(),
        person_张角.get_id(),
        person_张宝.get_id(),
        person_张梁.get_id(),
        person_曹操.get_id(),
      };
      if (神术技能武将.find(src_leader.get_id()) >= 0) return true;
      return false;
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

    // ------------------- 业火焚天------------------

    string getDesc_神术_业火焚天()
    {
      if (src_unit.energy < 计略气力消耗_业火焚天)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 部队妙计_兵力条件)
        return pk::encode("兵力不足,兵力至少5000.");
      else
        return pk::encode(pk::format("使用业火焚天.(气力至少 {} 以上)", 计略气力消耗_业火焚天));
    }
    bool isEnabled_神术_业火焚天()
    {
      if (src_unit.energy < 计略气力消耗_业火焚天) return false;
      else if (src_unit.troops < 部队妙计_兵力条件) return false;
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
          pk::add_troops(dst, -random(500, 1500), true); // 随机减兵500~1500
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
        // pk::wait(1000);
      }

      pk::add_energy(src_unit, -计略气力消耗_业火焚天, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 10);

      pk::say(pk::encode("感受下这红莲业火吧!"), person_周瑜);

      // pk::wait(100);

      pk::say(pk::encode("怎么感觉有点虚弱呢，难道是天谴吗。。。"), person_周瑜);
      person_周瑜.death -= random(3, 10);
      if (person_周瑜.death - person_周瑜.birth <= 0) { // 生命小于等于0，死亡
        person_周瑜.estimated_death = true; // 死亡预定
        pk::kill(src_unit);
        pk::wait(2000);
        pk::kill(person_周瑜); // 防止死亡预定因为队伍在外无法及时处理，补充设置武将死亡
      }
      person_周瑜.update();

      已影响城市.removeRange(0, 已影响城市.length() - 1); // 清除处理过的

      src_unit.action_done = true;
      if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
        src_unit.action_done = false;

      return true;
    }


    //-------------------------神雷灭世---------------
    //
    string getDesc_神术_神雷灭世()
    {
      if (src_unit.energy < 计略气力消耗_神雷灭世)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 部队妙计_兵力条件)
        return pk::encode("兵力不足,兵力至少5000.");
      else
        return pk::encode(pk::format("使用神雷灭世.(气力至少 {} 以上)", 计略气力消耗_神雷灭世));
    }


    bool isEnabled_神术_神雷灭世()
    {
      if (src_unit.energy < 计略气力消耗_神雷灭世) return false;
      else if (src_unit.troops < 部队妙计_兵力条件) return false;
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
          pk::add_troops(dst, -(部队落雷伤害 + pk::rand(部队落雷伤害_1) + pk::rand(2000)), true); // 在雷电伤害基础上随机加2000
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

      pk::add_energy(src_unit, -计略气力消耗_神雷灭世, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 10);

      pk::say(pk::encode("呼风唤雨，驱雷策电!道施所向，皆由我控!"), person_张角);

      // pk::wait(100);

      pk::say(pk::encode("黄天既覆，苍生何存......"), person_张角);
      person_张角.death -= random(3, 10);
      if (person_张角.death - person_张角.birth <= 0) { // 生命小于等于0，死亡预定
        person_张角.estimated_death = true;
        pk::kill(src_unit);
        pk::wait(2000);
        pk::kill(person_张角);
      }
      person_张角.update();

      src_unit.action_done = true;
      if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
        src_unit.action_done = false;

      return true;
    }

    // ----------- 黄天泰平 ----------
    string getText_神术_黄天泰平()
    {
      return pk::encode(pk::format("黄天泰平 ({})", 计略气力消耗_黄天泰平));
    }

    string getDesc_神术_黄天泰平()
    {
      if (src_unit.energy < 计略气力消耗_黄天泰平)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 部队妙计_兵力条件)
        return pk::encode("兵力不足,兵力至少5000.");
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
      else if (src_unit.troops < 部队妙计_兵力条件) return false;
      return true;
    }


    pk::array<pk::point_int> getTargets_神术_黄天泰平()
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

                  // pk::set_district(member_t, src_leader.get_district_id());
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
        // pk::wait(1000);
      }

      pk::add_energy(src_unit, -计略气力消耗_黄天泰平, true);

      pk::set_status(src_unit, null, 部队状态_混乱, 1, true); // 自己眩晕一回合

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("救苍生于水火，平天下于乱世!"), src_leader);

      // pk::wait(100);

      src_leader.death -= random(5, 15);
      pk::say(pk::encode("逆天而行，必遭天谴呐!"), src_leader);
      if (src_leader.death - src_leader.birth <= 0) { // 生命小于等于0，死亡预定
        src_leader.estimated_death = true;
        pk::kill(src_unit);
        pk::wait(2000);
        pk::kill(src_leader);
      }
      src_leader.update();

      src_unit.action_done = true;
      if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
        src_unit.action_done = false;

      return true;
    }

    // ----------- 天下归心 ----------
    string getText_神术_天下归心()
    {
      return pk::encode(pk::format("天下归心 ({})", 计略气力消耗_天下归心));
    }

    string getDesc_神术_天下归心()
    {
      if (src_unit.energy < 计略气力消耗_天下归心)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 部队妙计_兵力条件)
        return pk::encode("兵力不足,兵力至少5000.");
      else
        return pk::encode(pk::format("使用天下归心.(气力至少 {} 以上, 本部队伪报，俘虏3格内部队的女性武将)", 计略气力消耗_天下归心));
    }

    bool isVisible_神术_天下归心()
    {
      return src_leader.get_id() == person_曹操.get_id();
    }

    bool isEnabled_神术_天下归心()
    {
      if (src_unit.energy < 计略气力消耗_天下归心) return false;
      else if (src_unit.troops < 部队妙计_兵力条件) return false;
      return true;
    }


    pk::array<pk::point_int> getTargets_神术_天下归心()
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

      pk::add_energy(src_unit, -计略气力消耗_天下归心, true);

      pk::set_status(src_unit, null, 部队状态_伪报, 1, true); // 自己眩晕一回合

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("汝妻子吾自养之，汝勿虑也"), src_leader);

      // pk::wait(100);

      src_leader.death -= random(5, 10);
      pk::say(pk::encode("身体不行啊!"), src_leader);
      if (person_曹操.death - person_曹操.birth <= 0) { // 生命小于等于0，死亡预定
        src_leader.estimated_death = true;
        pk::kill(src_unit);
        pk::wait(2000);
        pk::kill(src_leader);
      }
      src_leader.update();

      src_unit.action_done = true;
      if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
        src_unit.action_done = false;

      return true;
    }
  }
  Main main;
}

 /*

  20220503 修复自定义武将头像不正确bug.

 */

namespace 神术数据结构体 {
  const int KEY = pk::hash("神术");
  const int KEY_索引_追加_武将起始 = 5000;   // 同 000 文件
  const int KEY_索引_追加_部队起始 = 30000;  // 同 000 文件

  pk::menu_item 菜单;
  int 菜单_神术;

  pk::unit@ src_unit;
  pk::person@ src_leader;
  pk::point src_pos_;

  array<string> 已影响城市; // 存储处理过的城市
  array<string> 临时武将 = { pk::encode("农民兵"), pk::encode("突袭者"), pk::encode("府兵"), pk::encode("雇佣兵"), pk::encode("贾诩小队") };

  array<int> 神术技能武将= {
    武将_周瑜,
    武将_张角,
    武将_张宝,
    武将_张梁,
    武将_曹操,
    武将_郭嘉,
    武将_袁术,
    武将_诸葛亮,
    武将_司马懿,
    武将_庞统,
    武将_贾诩,
    武将_法正,
    武将_荀攸,
    武将_陆逊,
    武将_荀彧,
  };

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
    pk::force@ person_force = pk::get_force(person.get_force_id());
    int color = person_force is null ? -1 : person_force.color;

    pk::history_log(persion_pos, color, pk::encode(str));
  }
  void 历史日志(pk::unit@ unit, string name, string result)
  {
    string unit_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(unit)));
    string action_name = pk::format("\x1b[27x{}\x1b[0x", name);

    string str = pk::format("{}队由于{}的效果，{}", unit_name, action_name, result);

    pk::force@ unit_force = pk::get_force(unit.get_force_id());
    int color = unit_force is null ? -1 : unit_force.color;
    pk::history_log(unit.pos, color, pk::encode(str));
  }
  void 历史日志(pk::building@ building, string name, string result)
  {
    string building_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(building)));
    string action_name = pk::format("\x1b[27x{}\x1b[0x", name);

    string str = pk::format("{}由于{}的效果，{}", building_name, action_name, result);
    pk::force@ building_force = pk::get_force(building.get_force_id());
    int color = building_force is null ? -1 : building_force.color;
    pk::history_log(building.pos, color, pk::encode(str));
  }

  string getText_神术_名称(string& 神术名称, int 气力消耗 = 0)
  {
    return pk::encode(pk::format("{} ({})", 神术名称, 气力消耗));
  }

  bool isVisible_神术_名称(array<int> 武将列表, int leader_Id)
  {
    return 武将列表.find(leader_Id) >= 0;
  }

  bool isVisible_神术_名称(int 武将Id, int leader_Id)
  {
    return leader_Id == 武将Id;
  }

  bool isVisible_神技_名称(pk::unit@ unit, int weapon_id, int level)
  {
    return pk::get_tekisei(unit, weapon_id) >= level;
  }

  pk::array<pk::point_int> getTargets_神术_目标(int range, pk::point src_pos_)
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

  pk::array<pk::point_int> getTargets_神术_目标部队位置(int range, pk::point src_pos_)
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

  pk::list<pk::unit@> getTargets_神术_目标部队(int range, pk::point src_pos_)
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

  pk::list<pk::unit@> getTargets_势力_目标部队(pk::force@ force, int range, pk::point src_pos_)
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
      if (building.get_id() >= 据点_末 and building.hp <= 0)
      {
        auto dst_pos0 = building.pos;
        auto dst_facility = pk::get_facility(building.facility);
        pk::kill(building, true);
        ch::特殊地名争夺处理(dst_facility.get_id(), src_unit.get_force_id(), dst_pos0);
      }
    }

    building.update();
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

  void 部队行动(pk::unit@ unit)
  {
    unit.action_done = true;
    if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
      unit.action_done = false;
  }

  void 设置部队伤害值(pk::unit@ attacker, int troops_damage)
  {
    for (int i = 0; i < 3; i++)
    {
      if (pk::is_valid_person_id(attacker.member[i]))
      {
        sc_personinfo@ sc_person = @person_sc[attacker.member[i]];
        sc_person.troops_damage += troops_damage;
      }
    }
  }

  void 设置部队杀敌值(pk::unit@ attacker)
  {
    for (int i = 0; i < 3; i++)
    {
      if (pk::is_valid_person_id(attacker.member[i]))
      {
        sc_personinfo@ sc_person = @person_sc[attacker.member[i]];
        sc_person.kill_unit += 1;
      }
    }
  }

  class Main
  {
    pk::func209_t@ prev_callback_209;
    pk::func212_t@ prev_callback_212;
    pk::func213_t@ prev_callback_213;
    pk::func214_t@ prev_callback_214;
    pk::func215_t@ prev_callback_215;

    Main()
    {
      pk::bind(102, 0, pk::trigger102_t(剧本初始化_结构体_信息读取));
      pk::bind(105, pk::trigger105_t(儲存_结构体_信息储存));

      pk::bind(151, pk::trigger151_t(武将死亡));
      pk::bind(171, pk::trigger171_t(部队清除));
      pk::bind(175, pk::trigger175_t(部队溃灭));

      pk::bind(120, pk::trigger120_t(部队特殊信息显示));

      @prev_callback_209 = cast<pk::func209_t@>(pk::get_func(209));
      pk::reset_func(209);
      pk::set_func(209, pk::func209_t(func209));

      @prev_callback_212 = cast<pk::func212_t@>(pk::get_func(212));
      pk::reset_func(212);
      pk::set_func(212, pk::func212_t(func212));

      @prev_callback_213 = cast<pk::func213_t@>(pk::get_func(213));
      pk::reset_func(213);
      pk::set_func(213, pk::func213_t(func213));

      @prev_callback_214 = cast<pk::func214_t@>(pk::get_func(214));
      pk::reset_func(214);
      pk::set_func(214, pk::func214_t(func214));

      @prev_callback_215 = cast<pk::func215_t@>(pk::get_func(215));
      pk::reset_func(215);
      pk::set_func(215, pk::func215_t(func215));


      添加神术菜单();
    }

    void 添加神术菜单()
    {
      菜单.menu = 1;
      菜单.init = pk::unit_menu_item_init_t(init);
      菜单.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("神术"); });
      菜单.is_visible = pk::menu_item_is_visible_t(isVisible_神术);
      菜单_神术 = pk::add_menu_item(菜单);
    }

    void init(pk::unit@ unit, pk::point src_pos)
    {
      @src_unit = @unit;
      @src_leader = pk::get_person(src_unit.leader);
      src_pos_ = src_pos;
    }

    bool isVisible_神术()
    {
      if (pk::is_campaign()) return false;
      if (神术技能武将.find(src_leader.get_id()) >= 0) return true;
      return false;
    }

    void 剧本初始化_结构体_信息读取()
    {
      //重新开始游戏时，初始化数据
      if (!pk::get_scenario().loaded)
      {
        for (int i = 0; i < 武将_末; ++i)
        {
          person_sc[i].狼顾权变_失去技能回合 = 游戏最大时间;
          person_sc[i].狼顾权变_技能失效编号 = -1;
          person_sc[i].帷幄奇策_技能获得回合 = 游戏最大时间;
          person_sc[i].特技_霸王 = false;
          person_sc[i].特技_百战 = false;
          person_sc[i].特技_巧变 = false;
          person_sc[i].特技_激励 = false;
          person_sc[i].神鬼八阵_使用 = false;
          person_sc[i].火凤连环_使用 = false;
          person_sc[i].kill_unit = 0;
          person_sc[i].troops_damage = 0;
          person_sc[i].kill_destroyed = 0;
          person_sc[i].kill_unit_destroyed = 0;
        }

        for (int i = 0; i < 部队_末; i++)
        {
          unit_sc[i].神鬼八阵效果 = false;
          unit_sc[i].火凤连环影响 = false;
          unit_sc[i].乱武完杀部队 = false;
          unit_sc[i].奇谋诡策影响 = false;
          unit_sc[i].帷幄奇策_禁法回合 = 游戏最大时间;
        }
      }
      if (pk::get_scenario().loaded)
      {
        for (int i = 0; i < 武将_末; i++)
        {
          for (int j = 0; j <= (神术武将结构体_uint32数 - 1); j++)
            sc_person_info_temp[j][i] = uint32(pk::load(KEY, (KEY_索引_追加_武将起始 + (i * 神术武将结构体_uint32数 + j)), 0));
          sc_personinfo person_t(i);
          person_sc[i] = person_t;
        }
        for (int i = 0; i < 部队_末; i++)
        {
          for (int j = 0; j <= (神术部队结构体_uint32数 - 1); j++)
            sc_unit_info_temp[j][i] = uint32(pk::load(KEY, (KEY_索引_追加_部队起始 + (i * 神术部队结构体_uint32数 + j)), 0));
          sc_unitinfo unit_t(i);
          unit_sc[i] = unit_t;
        }
      }
    }

    void 儲存_结构体_信息储存(int file_id)
    {
      for (int i = 0; i < 武将_末; i++)
      {
        person_sc[i].update(i);
        for (int j = 0; j <= (神术武将结构体_uint32数 - 1); j++)
          pk::store(KEY, (KEY_索引_追加_武将起始 + (i * 神术武将结构体_uint32数 + j)), sc_person_info_temp[j][i]);
      }
      for (int i = 0; i < 部队_末; i++)
      {
        unit_sc[i].update(i);
        for (int j = 0; j <= (神术部队结构体_uint32数 - 1); j++)
          pk::store(KEY, (KEY_索引_追加_部队起始 + (i * 神术部队结构体_uint32数 + j)), sc_unit_info_temp[j][i]);
      }
    }

    void 武将死亡(pk::person@ dead, pk::person@ by, int type, int rettype)
    {
      sc_personinfo@ sc_person = @person_sc[dead.get_id()];
      sc_person.狼顾权变_失去技能回合 = 游戏最大时间;
      sc_person.狼顾权变_技能失效编号 = -1;
      sc_person.帷幄奇策_技能获得回合 = 游戏最大时间;
      sc_person.特技_霸王 = false;
      sc_person.特技_百战 = false;
      sc_person.特技_巧变 = false;
      sc_person.特技_激励 = false;
      sc_person.神鬼八阵_使用 = false;
      sc_person.火凤连环_使用 = false;
      sc_person.kill_unit = 0;
      sc_person.troops_damage = 0;
      sc_person.kill_destroyed = 0;
      sc_person.kill_unit_destroyed = 0;
    }

    void 部队清除(pk::unit@ unit, pk::hex_object@ dst, int type)
    {
      pk::unit@ dst_unit = (dst !is null and dst.get_type_id() == pk::unit::type_id) ? dst : null;
      for (int i = 0; i < 3; i++)
      {
        if (pk::is_valid_person_id(unit.member[i]))
        {
          pk::person@ member = pk::get_person(unit.member[i]);
          sc_personinfo@ sc_person = @person_sc[unit.member[i]];
          if (临时武将.find(member.name_read) >= 0)
          {
            sc_person.狼顾权变_失去技能回合 = 游戏最大时间;
            sc_person.狼顾权变_技能失效编号 = -1;
            sc_person.帷幄奇策_技能获得回合 = 游戏最大时间;
            sc_person.特技_霸王 = false;
            sc_person.特技_百战 = false;
            sc_person.特技_巧变 = false;
            sc_person.特技_激励 = false;
            sc_person.神鬼八阵_使用 = false;
            sc_person.火凤连环_使用 = false;
            sc_person.kill_unit = 0;
            sc_person.troops_damage = 0;
            sc_person.kill_destroyed = 0;
            sc_person.kill_unit_destroyed = 0;
          }
          else if (type == 0)
          {
            sc_person.kill_destroyed += 1;
            if (dst_unit !is null) sc_person.kill_unit_destroyed += 1;
          }
        }
      }
    }

    void 部队溃灭(pk::unit@ attacker, pk::unit@ target)
    {
      for (int i = 0; i < 3; i++)
      {
        if (pk::is_valid_person_id(attacker.member[i]))
        {
          sc_personinfo@ sc_person = @person_sc[attacker.member[i]];
          sc_person.kill_unit += 1;
        }
      }
    }

    void 部队特殊信息显示()
    {
      pk::point cursor_pos = pk::get_cursor_hex_pos();
      if (!pk::is_valid_pos(cursor_pos)) return;

      // 光标上指示的部队
      pk::unit@ unit = pk::get_unit(cursor_pos);
      if (unit is null) return;

      bool is_player = unit.is_player();

      int current = pk::get_elapsed_days();

      sc_unitinfo@ sc_unit = @unit_sc[unit.get_id()];

      string unit_name = pk::decode(pk::get_name(unit));

      string title = pk::format("部队特殊信息(\x1b[1x{}\x1b[0x)", unit_name);

      string title2 = "武将特殊信息";

      string 神鬼八阵效果 = pk::format("(带神鬼八阵效果: \x1b[1x{}\x1b[0x)", sc_unit.神鬼八阵效果 ? '一击必杀' : '否');

      string 火凤连环影响 = pk::format("(被火凤连环部队: \x1b[1x{}\x1b[0x)", pk::encode(sc_unit.火凤连环影响 ? '是' : '否'));

      string 奇谋诡策影响 = pk::format("(受奇谋诡策影响: \x1b[1x{}\x1b[0x)", pk::encode(sc_unit.奇谋诡策影响 ? '是' : '否'));

      bool times = int(sc_unit.帷幄奇策_禁法回合) > 0 and current > int(sc_unit.帷幄奇策_禁法回合 - 10) and current - int(sc_unit.帷幄奇策_禁法回合 - 10) <= 30;

      string 帷幄奇策影响 = pk::format("(受帷幄奇策影响: \x1b[1x{}\x1b[0x)", pk::encode(times ? '是' : '否'));

      array<bool> 效果影响列表 = { sc_unit.神鬼八阵效果, sc_unit.火凤连环影响, sc_unit.奇谋诡策影响, times };

      array<string> 效果内容列表 = { 神鬼八阵效果, 火凤连环影响, 奇谋诡策影响, 帷幄奇策影响 };

      array<string> 展示文案;

      for (int i = 0; i < int(效果影响列表.length); i++)
      {
        if (效果影响列表[i]) 展示文案.insertLast(效果内容列表[i]);
      }
      int len1 = int(展示文案.length);

      pk::list<pk::person@> member = ch::get_member_list(unit);
      sc_personinfo@ sc_person_leader = @person_sc[unit.leader];

      int width = int(pk::get_resolution().width) - 280;
      pk::point left_down = pk::point(int(pk::get_resolution().width) - 10, 75 + 3 * 20 + 40 + 5 + int(member.count) * 80) + 20;
      pk::draw_filled_rect(pk::rectangle(pk::point(width - 5, 15), left_down), ((0xff / 2) << 24) | 0x010101);
      pk::draw_text(pk::encode(title), pk::point(width, 20), 0xffffffff, FONT_BIG, 0xff000000);

      for (int i = 0; i < len1; i++)
      {
        pk::draw_text(pk::encode(展示文案[i]), pk::point(width, 75 + i * 20), 0xffffffff, FONT_SMALL, 0xff000000);
      }

      if (len1 == 0) pk::draw_text(pk::encode('(无)'), pk::point(width, 75), 0xffffffff, FONT_SMALL, 0xff000000);

      int person_offset_top =  95 + (len1 > 0 ? len1 : 1) * 20;
      pk::draw_text(pk::encode(title2), pk::point(width, person_offset_top), 0xffffffff, FONT_BIG, 0xff000000);

      for (int i = 0; i < int(member.count); i++)
      {
        int offset_top = person_offset_top + i * 60 + 10;
        pk::person@ person = member[i];
        int person_id = person.get_id();
        sc_personinfo@ sc_person = @person_sc[person_id];

        pk::point rightup = pk::point(width + 10, offset_top + (i + 1) * 40);
        pk::point leftdown = pk::point(width + 42, offset_top + 40 + (i + 1) * 40);
        auto rect = pk::rectangle(rightup, leftdown);

        pk::draw_face(FACE_SMALL, person.face, rect, FACE_L);

        string 武将姓名 = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(person)));
        string 武将杀敌数 = pk::format("击杀部队数：\x1b[1x{}\x1b[0x", sc_person.kill_unit);
        string 武将杀兵数 = pk::format("击杀士兵数：\x1b[1x{}\x1b[0x", sc_person.troops_damage);
        string 武将被杀数 = pk::format("被击杀次数：\x1b[1x{}\x1b[0x(部队:\x1b[19x{}\x1b[0x)", sc_person.kill_destroyed, sc_person.kill_unit_destroyed);

        int face_top = offset_top + (i + 1) * 20 + (i * 20);

        pk::draw_text(pk::encode(武将姓名), pk::point(width + 70, face_top + 0), 0xffffffff, FONT_SMALL, 0xff000000);
        pk::draw_text(pk::encode(武将杀敌数), pk::point(width + 70, face_top + 20), 0xffffffff, FONT_SMALL, 0xff000000);
        pk::draw_text(pk::encode(武将杀兵数), pk::point(width + 70, face_top + 40), 0xffffffff, FONT_SMALL, 0xff000000);
        pk::draw_text(pk::encode(武将被杀数), pk::point(width + 70, face_top + 60), 0xffffffff, FONT_SMALL, 0xff000000);
      }

    }

    void func209(pk::damage_info& info, pk::unit@ attacker, int tactics_id, const pk::point& in target_pos, int type, int critical, bool ambush, int rettype)
    {
      prev_callback_209(info, attacker, tactics_id, target_pos, type, critical, ambush, rettype);
      if (rettype != 15 and attacker !is null)
      {
        for (int i = 0; i < 3; i++)
        {
          if (pk::is_valid_person_id(attacker.member[i]))
          {
            sc_personinfo@ sc_person = @person_sc[attacker.member[i]];
            sc_person.troops_damage += info.troops_damage;
          }
        }
      }
    }

    void func212(pk::damage_info& info, pk::unit@ attacker, pk::hex_object@ sub_target)
    {
      prev_callback_212(info, attacker, sub_target);
      if (attacker !is null)
      {
        for (int i = 0; i < 3; i++)
        {
          if (pk::is_valid_person_id(attacker.member[i]))
          {
            pk::person@ member = pk::get_person(attacker.member[i]);
            if (临时武将.find(member.name_read) < 0)
            {
              sc_personinfo@ sc_person = @person_sc[attacker.member[i]];
              sc_person.troops_damage += info.troops_damage;
            }
          }
        }
      }
    }

    void func213(pk::damage_info& info, int trap, pk::unit@ attacker, pk::hex_object@ target, bool critical)
    {
      prev_callback_213(info, trap, attacker, target, critical);
      if (attacker !is null)
      {
        for (int i = 0; i < 3; i++)
        {
          if (pk::is_valid_person_id(attacker.member[i]))
          {
            pk::person@ member = pk::get_person(attacker.member[i]);
            if (临时武将.find(member.name_read) < 0)
            {
              sc_personinfo@ sc_person = @person_sc[attacker.member[i]];
              sc_person.troops_damage += info.troops_damage;
            }
          }
        }
      }
    }

    void func214(pk::damage_info& info, pk::unit@ attacker, pk::hex_object@ target, bool critical)
    {
      prev_callback_214(info, attacker, target, critical);
      if (attacker !is null)
      {
        for (int i = 0; i < 3; i++)
        {
          if (pk::is_valid_person_id(attacker.member[i]))
          {
            pk::person@ member = pk::get_person(attacker.member[i]);
            if (临时武将.find(member.name_read) < 0)
            {
              sc_personinfo@ sc_person = @person_sc[attacker.member[i]];
              sc_person.troops_damage += info.troops_damage;
            }
          }
        }
      }
    }

    void func215(pk::damage_info& info, pk::unit@ attacker, pk::hex_object@ target, bool critical)
    {
      prev_callback_215(info, attacker, target, critical);
      if (attacker !is null)
      {
        for (int i = 0; i < 3; i++)
        {
          if (pk::is_valid_person_id(attacker.member[i]))
          {
            pk::person@ member = pk::get_person(attacker.member[i]);
            if (临时武将.find(member.name_read) < 0)
            {
              sc_personinfo@ sc_person = @person_sc[attacker.member[i]];
              sc_person.troops_damage += info.troops_damage;
            }
          }
        }
      }
    }

  }
  Main main;
}



const int 神术武将结构体_uint32数 = 10;
const int 神术部队结构体_uint32数 = 5;

const int 游戏最大时间 = 1000 * 12 * 3 * 10;

array<array<uint32>> sc_unit_info_temp(神术部队结构体_uint32数, array<uint32>(部队_末, uint32(0)));
array<array<uint32>> sc_person_info_temp(神术武将结构体_uint32数, array<uint32>(武将_末, uint32(0)));

array<sc_personinfo> person_sc(武将_末);
array<sc_unitinfo> unit_sc(部队_末);

class sc_unitinfo {
  int 神鬼八阵影响序号 = 0;
  int 火凤连环影响序号 = 1;
  int 乱武完杀部队序号 = 2;
  int 奇谋诡策影响序号 = 3;
  int 帷幄奇策影响序号 = 4;

  bool 神鬼八阵效果;
  bool 火凤连环影响;
  bool 乱武完杀部队;
  bool 奇谋诡策影响;
  uint32 帷幄奇策_禁法回合 = 游戏最大时间;

  //初始化
  sc_unitinfo(int id)
  {
    get_info(id);
  }

  sc_unitinfo() {}

  void get_info(int id)
  {
    fromInt32_0(sc_unit_info_temp[神鬼八阵影响序号][id]);
    fromInt32_1(sc_unit_info_temp[火凤连环影响序号][id]);
    fromInt32_2(sc_unit_info_temp[乱武完杀部队序号][id]);
    fromInt32_3(sc_unit_info_temp[奇谋诡策影响序号][id]);
    fromInt32_4(sc_unit_info_temp[帷幄奇策影响序号][id]);
  }

  void update(int id)
  {
    sc_unit_info_temp[神鬼八阵影响序号][id] = toInt32_0();
    sc_unit_info_temp[火凤连环影响序号][id] = toInt32_1();
    sc_unit_info_temp[乱武完杀部队序号][id] = toInt32_2();
    sc_unit_info_temp[奇谋诡策影响序号][id] = toInt32_3();
    sc_unit_info_temp[帷幄奇策影响序号][id] = toInt32_4();
  }

  uint32 toInt32_0(void)
  {
    uint8 神鬼八阵效果_值 = 神鬼八阵效果 ? 1 : 0;
    uint32 x = 神鬼八阵效果_值;
    return x;
  }

  uint32 toInt32_1(void)
  {
    uint8 火凤连环影响_值 = 火凤连环影响 ? 1 : 0;
    uint32 x = 火凤连环影响_值;
    return x;
  }

  uint32 toInt32_2(void)
  {
    uint8 乱武完杀部队_值 = 乱武完杀部队 ? 1 : 0;
    uint32 x = 乱武完杀部队_值;
    return x;
  }

  uint32 toInt32_3(void)
  {
    uint8 奇谋诡策影响_值 = 奇谋诡策影响 ? 1 : 0;
    uint32 x = 奇谋诡策影响_值;
    return x;
  }

  uint32 toInt32_4(void)
  {
    uint32 帷幄奇策_禁法回合_值 = 帷幄奇策_禁法回合;
    uint32 x = 帷幄奇策_禁法回合_值;
    return x;
  }

  void fromInt32_0(uint32 x)
  {
    神鬼八阵效果 = (((x << 31) >> 31) == 1);
  }

  void fromInt32_1(uint32 x)
  {
    火凤连环影响 = (((x << 31) >> 31) == 1);
  }

  void fromInt32_2(uint32 x)
  {
    乱武完杀部队 = (((x << 31) >> 31) == 1);
  }

  void fromInt32_3(uint32 x)
  {
    奇谋诡策影响 = (((x << 31) >> 31) == 1);
  }

  void fromInt32_4(uint32 x)
  {
    帷幄奇策_禁法回合 = ((x << 0) >> 0);
  }
}

class sc_personinfo {
  uint16 person_id_t;

  int 狼顾权变时长序号 = 0;
  int 狼顾权变技能序号 = 1;
  int 帷幄奇策时长序号 = 2;
  int 帷幄奇策技能序号 = 3;
  int 神鬼八阵效果序号 = 4;
  int 火凤连环效果序号 = 5;

  int 击破部队效果序号 = 6;
  int 士兵伤害效果序号 = 7;
  int 部队溃灭效果序号 = 8; // 部队被其他部队击破数
  int 部队被破效果序号 = 9; // 部队总被击破数

  int32 狼顾权变_失去技能回合 = 游戏最大时间;
  int16 狼顾权变_技能失效编号 = -1;
  int32 帷幄奇策_技能获得回合 = 游戏最大时间;
  bool 特技_霸王;
  bool 特技_百战;
  bool 特技_巧变;
  bool 特技_激励;
  bool 神鬼八阵_使用 = false;
  bool 火凤连环_使用 = false;

  int32 kill_unit = 0; // 击破部队数
  int32 troops_damage = 0; // 士兵伤害数
  int32 kill_destroyed = 0; // 被击溃总次数
  int32 kill_unit_destroyed = 0; // 被部队溃灭次数

  //初始化
  sc_personinfo(int person_id)
  {
    person_id_t = person_id;
    get_info(person_id);
  }

  //下面这种写法用于定义全局变量，先声明，然后用get_info获取信息后使用
  sc_personinfo() {}

  void get_info(int person_id)
  {
    fromInt32_0(sc_person_info_temp[狼顾权变时长序号][person_id]);
    fromInt32_1(sc_person_info_temp[狼顾权变技能序号][person_id]);
    fromInt32_2(sc_person_info_temp[帷幄奇策时长序号][person_id]);
    fromInt32_3(sc_person_info_temp[帷幄奇策技能序号][person_id]);
    fromInt32_4(sc_person_info_temp[神鬼八阵效果序号][person_id]);
    fromInt32_5(sc_person_info_temp[火凤连环效果序号][person_id]);
    fromInt32_6(sc_person_info_temp[击破部队效果序号][person_id]);
    fromInt32_7(sc_person_info_temp[士兵伤害效果序号][person_id]);
    fromInt32_8(sc_person_info_temp[部队溃灭效果序号][person_id]);
    fromInt32_9(sc_person_info_temp[部队被破效果序号][person_id]);
  }

  void update(int person_id)
  {
    sc_person_info_temp[狼顾权变时长序号][person_id] = toInt32_0();
    sc_person_info_temp[狼顾权变技能序号][person_id] = toInt32_1();
    sc_person_info_temp[帷幄奇策时长序号][person_id] = toInt32_2();
    sc_person_info_temp[帷幄奇策技能序号][person_id] = toInt32_3();
    sc_person_info_temp[神鬼八阵效果序号][person_id] = toInt32_4();
    sc_person_info_temp[火凤连环效果序号][person_id] = toInt32_5();
    sc_person_info_temp[击破部队效果序号][person_id] = toInt32_6();
    sc_person_info_temp[士兵伤害效果序号][person_id] = toInt32_7();
    sc_person_info_temp[部队溃灭效果序号][person_id] = toInt32_8();
    sc_person_info_temp[部队被破效果序号][person_id] = toInt32_9();
  }

  uint32 toInt32_0(void)
  {
    uint32 狼顾权变_失去技能回合_值 = 狼顾权变_失去技能回合;
    uint32 x = 狼顾权变_失去技能回合_值;
    return x;
  }

  uint32 toInt32_1(void)
  {
    uint32 狼顾权变_技能失效编号_值 = 狼顾权变_技能失效编号;
    uint32 x = 0;
    x = 狼顾权变_技能失效编号_值;
    return x;
  }

  uint32 toInt32_2(void)
  {
    uint32 帷幄奇策_技能获得回合_值 = 帷幄奇策_技能获得回合;
    uint32 x = 帷幄奇策_技能获得回合_值;
    return x;
  }

  uint32 toInt32_3(void)
  {
    uint8 特技_霸王_值 = 特技_霸王 ? 1 : 0;
    uint8 特技_百战_值 = 特技_百战 ? 1 : 0;
    uint8 特技_巧变_值 = 特技_巧变 ? 1 : 0;
    uint8 特技_激励_值 = 特技_激励 ? 1 : 0;
    uint32 x = 特技_霸王_值 + (特技_百战_值 << 8) + (特技_巧变_值 << 16) + (特技_激励_值 << 24);
    return x;
  }

  uint32 toInt32_4(void)
  {
    uint8 神鬼八阵使用_值 = 神鬼八阵_使用 ? 1 : 0;
    uint32 x = 神鬼八阵使用_值;
    return x;
  }

  uint32 toInt32_5(void)
  {
    uint8 火凤连环使用_值 = 火凤连环_使用 ? 1 : 0;
    uint32 x = 火凤连环使用_值;
    return x;
  }

  uint32 toInt32_6(void)
  {
    uint32 kill_unit_值 = kill_unit;
    uint32 x = kill_unit_值;
    return x;
  }

  uint32 toInt32_7(void)
  {
    uint32 troops_damage_值 = troops_damage;
    uint32 x = troops_damage_值;
    return x;
  }
  uint32 toInt32_8(void)
  {
    uint32 kill_destroyed_值 = kill_destroyed;
    uint32 x = kill_destroyed_值;
    return x;
  }
  uint32 toInt32_9(void)
  {
    uint32 kill_unit_destroyed_值 = kill_unit_destroyed;
    uint32 x = kill_unit_destroyed_值;
    return x;
  }

  void fromInt32_0(uint32 x)
  {
    狼顾权变_失去技能回合 = ((x << 0) >> 0);
  }

  void fromInt32_1(uint32 x)
  {
    狼顾权变_技能失效编号 = ((x << 16) >> 16);
  }

  void fromInt32_2(uint32 x)
  {
    帷幄奇策_技能获得回合 = ((x << 0) >> 0);
  }

  void fromInt32_3(uint32 x)
  {
    特技_霸王 = ((x << 24) >> 24) == 1;
    特技_百战 = ((x << 16) >> 24) == 1;
    特技_巧变 = ((x << 8) >> 24) == 1;
    特技_激励 = ((x << 0) >> 24) == 1;
  }

  void fromInt32_4(uint32 x)
  {
    神鬼八阵_使用 = (((x << 31) >> 31) == 1);
  }

  void fromInt32_5(uint32 x)
  {
    火凤连环_使用 = (((x << 31) >> 31) == 1);
  }

  void fromInt32_6(uint32 x)
  {
    kill_unit = ((x << 0) >> 0);
  }

  void fromInt32_7(uint32 x)
  {
    troops_damage = ((x << 0) >> 0);
  }

  void fromInt32_8(uint32 x)
  {
    kill_destroyed = ((x << 0) >> 0);
  }

  void fromInt32_9(uint32 x)
  {
    kill_unit_destroyed = ((x << 0) >> 0);
  }
}

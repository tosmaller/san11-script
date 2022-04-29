/*
描述：封地强化，围绕封地扩展3个功能
1 封地传送：部队可通过封地相互传送到其他封地旁边
2 封地补兵：可直接在城市内为封地补充兵力
3 封地散兵：封地被敌军攻破后，出现一只兵力为封地士兵数的部队(待实现)

*/

namespace 封地强化 {
  const int 计略气力消耗 = 30;
  const int 兵力条件_通用 = 5000;
  const int 封地兵力值不补给 = 10000;

  class Main
  {
    pk::random random(pk::rand());

    int 封地补给行动力 = 30;
    int 封地补给技巧值 = 200;

    Main() {
      add_menu_unit_order();
      add_city_military_order();

      // pk::bind(164, -1, pk::trigger164_t(afterkillbuilding));
      // pk::bind2(163, pk::trigger163_t(beforeKillbuilding), 1);
      // pk::bind2(164, pk::trigger164_t(afterkillbuilding), 1);
    }

    pk::unit@ src_unit;
    pk::person@ src_leader;
    pk::force@ force_;
    pk::point src_pos_;
    pk::building@ src_building;
    pk::city@ city_;


    bool 开启_传送阵 = true;

    void add_menu_unit_order()
    {
      pk::menu_item 传送阵;
      传送阵.menu = global_menu::菜单_部队;
      传送阵.init = pk::unit_menu_item_init_t(unit_init);
      传送阵.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_名称('传送阵', 计略气力消耗); });
      传送阵.get_desc = pk::menu_item_get_desc_t(getDesc_传送阵);
      传送阵.is_visible = pk::menu_item_is_visible_t(isVisible_传送阵);
      传送阵.is_enabled = pk::menu_item_is_enabled_t(isEnabled_传送阵);
      传送阵.handler = pk::unit_menu_item_handler_t(handler_传送阵);
      pk::add_menu_item(传送阵);
    }

    void add_city_military_order()
    {
      pk::menu_item 封地补给;
      封地补给.menu = 101;
      封地补给.init = pk::building_menu_item_init_t(city_init);
      封地补给.get_text = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("封地补给"); });
      封地补给.is_enabled = pk::menu_item_is_enabled_t(isEnabled_封地补给);
      封地补给.get_desc = pk::menu_item_get_desc_t(getDesc_封地补给);
      封地补给.handler = pk::menu_item_handler_t(handler_封地补给);
      pk::add_menu_item(封地补给);
    }

    array<int> get_spec(pk::force@ force)
    {
      array<int> temp;
      for (int i = 0; i < 地名_末; ++i)
      {
        pk::point pos0 = pk::point(特殊地名设施[i][1], 特殊地名设施[i][2]);
        pk::building@ building0 = pk::get_building(pos0);
        if (building0 is null) continue;
        if (building0.get_force_id() != force.get_id()) continue;
        specialinfo@ spec_t = @special_ex[i];
        if (spec_t.person == -1) continue;
        temp.insertLast(i);
      }
      return temp;
    }

    pk::list<pk::city@> get_able_city(pk::force@ force)
    {
      pk::list<pk::city@> temp;
      array<int> id_temp;
      for (int i = 0; i < 地名_末; ++i)
      {
        pk::point pos0 = pk::point(特殊地名设施[i][1], 特殊地名设施[i][2]);
        auto hex = pk::get_hex(pos0);
        if (!hex.has_building) continue;
        pk::building@ building0 = pk::get_building(pos0);

        if (building0.get_force_id() != force.get_id()) continue;
        specialinfo@ spec_t = @special_ex[i];
        if (spec_t.person == -1) continue;
        if (pk::is_neighbor_pos(src_pos_, pos0)) continue;
        int city_id = pk::get_city_id(pos0);
        if (id_temp.find(city_id) < 0 and city_id != -1)
        {
          id_temp.insertLast(city_id);
          temp.add(pk::get_city(city_id));
        }
      }
      return temp;
    }

    // --------- 封地传送 -----------

    array<int> get_city_spec(pk::city@ city, pk::force@ force)
    {
      array<int> temp;
      for (int i = 0; i < 地名_末; ++i)
      {
        pk::point pos0 = pk::point(特殊地名设施[i][1], 特殊地名设施[i][2]);
        pk::building@ building0 = pk::get_building(pos0);
        if (building0 is null) continue;
        if (building0.get_force_id() != force.get_id()) continue;
        specialinfo@ spec_t = @special_ex[i];
        if (spec_t.person == -1) continue;
        if (pk::is_neighbor_pos(src_pos_, pos0)) continue;
        int city_id = pk::get_city_id(pos0);
        if (city.get_id() != city_id) continue;
        temp.insertLast(i);
      }
      return temp;
    }

    bool checkout_force_spec(pk::force@ force)
    {
      array<int> spec_arr = get_spec(force);
      return spec_arr.length > 1;
    }

    void unit_init(pk::unit@ unit, pk::point src_pos)
    {
      @src_unit = @unit;
      @src_leader = pk::get_person(src_unit.leader);
      @force_ = pk::get_force(unit.get_force_id());
      src_pos_ = src_pos;
    }

    bool isVisible_传送阵()
    {
      return checkout_force_spec(force_);
    }

    string getText_名称(string 名称, int 气力消耗)
    {
      return pk::encode(pk::format("{} ({})", 名称, 气力消耗));
    }
    string getDesc_传送阵()
    {
      if (src_unit.energy < 计略气力消耗)
        return pk::encode("气力不足.");
      else if (src_unit.gold < 1000)
        return pk::encode("金钱不足.");
      else if (!checkout_force_spec(force_))
        return pk::encode("没有可传送的封地");
      else if (!is_near_spec()) {
        return pk::encode("没有在封地附近");
      }
      return pk::encode(pk::format("消耗气力{}，金钱1000，部队可选择传送到势力已分封的封地旁", 计略气力消耗));

    }

    bool isEnabled_传送阵()
    {
      if (src_unit.energy < 计略气力消耗) return false;
      else if (src_unit.gold < 1000) return false;
      else if (!checkout_force_spec(force_)) return false;
      else if (!is_near_spec()) return false;
      return true;
    }

    bool is_near_spec()
    {
      array<int> spec_arr = get_spec(force_);
      bool near = false;
      for(int i = 0; i < int(spec_arr.length); i += 1)
      {
        pk::point pos0 = pk::point(特殊地名设施[spec_arr[i]][1], 特殊地名设施[spec_arr[i]][2]);
        if (pk::is_neighbor_pos(src_pos_, pos0))
        {
          near = true;
          break;
        }
      }
      return near;
    }

    int choose_spec(array<int> spec_arr)
    {
      //一个城市最多8个？
      // 头5+1 中间4+2 尾部 n+1
      int num = int(spec_arr.length);
      int choose_times = (num - 5) / 4;//除法直接去掉小数
      int mod = (num - 5) % 4;//取余数做末尾
      //要获取的是spec_id
      array<string> spec_name_list;
      for (int i = 0; i < pk::min(5,num); ++i)
      {
        pk::point pos0 = pk::point(特殊地名设施[spec_arr[i]][1], 特殊地名设施[spec_arr[i]][2]);
        if (pk::is_neighbor_pos(pos0, src_unit.get_pos())) continue;
        spec_name_list.insertLast(pk::encode(get_spec_city_name(spec_arr[i])+特殊地名设施名称[spec_arr[i]]));
      }

      if (num == 6) spec_name_list.insertLast(pk::encode(get_spec_city_name(spec_arr[num-1]) + 特殊地名设施名称[spec_arr[num - 1]]));
      else if (num > 6) spec_name_list.insertLast(pk::encode("下一页"));

      int n = pk::choose(pk::encode("请选择传送目标封地."), spec_name_list);
      if (n == 5 and num!=6) return choose_spec_mid(spec_arr,1);
      return spec_arr[n];
    }

    int choose_spec_mid(array<int> spec_arr,int page)
    {
      //一个城市最多8个？
      // 头5+1 中间4+2 尾部 n+1
      int num = int(spec_arr.length);
      int num2 = (num - 1 - 4 * page);
      if (num2 <= 5)
      {
        //最后一页
        //要获取的是spec_id
        array<string> spec_name_list;
        for (int i = (1+page*4); i < (1 + page * 4 + num2); ++i)
        {
          pk::point pos0 = pk::point(特殊地名设施[spec_arr[i]][1], 特殊地名设施[spec_arr[i]][2]);
          if (pk::is_neighbor_pos(pos0, src_unit.get_pos())) continue;
          spec_name_list.insertLast(pk::encode(get_spec_city_name(spec_arr[i]) + 特殊地名设施名称[spec_arr[i]]));
        }
        spec_name_list.insertLast(pk::encode("上一页"));
        int n = pk::choose(pk::encode("请选择封地."), spec_name_list);

        if (n == 5 or n == int(spec_name_list.length -1))
        {
          if (page == 1) return choose_spec(spec_arr);
          return choose_spec_mid(spec_arr, page - 1);//如何返回上一页
        }

        int t = n + (page - 1) * 4 + 5;

        return spec_arr[t];
      }
      else//不是最后一页
      {
        //要获取的是spec_id
        array<string> spec_name_list;
        for (int i = (1 + page * 4); i < (1 + page * 4 + 4); ++i)
        {
          pk::point pos0 = pk::point(特殊地名设施[spec_arr[i]][1], 特殊地名设施[spec_arr[i]][2]);
          if (pk::is_neighbor_pos(pos0, src_unit.get_pos())) continue;
          spec_name_list.insertLast(pk::encode(get_spec_city_name(spec_arr[i]) + 特殊地名设施名称[spec_arr[i]]));
        }
        spec_name_list.insertLast(pk::encode("上一页"));
        spec_name_list.insertLast(pk::encode("下一页"));

        int n = pk::choose(pk::encode("请选择封地."), spec_name_list);

        //非最后一页情况
        if (n == 4)
        {
          if (page!= 1) return choose_spec_mid(spec_arr, page - 1);
          else return choose_spec(spec_arr);
        }
        if (n == 5) return choose_spec_mid(spec_arr, page + 1);

        int t = n + (page - 1) * 4 + 5;

        return spec_arr[t];
      }
    }

    string get_spec_city_name(int spec_id)
    {
      pk::point pos0 = pk::point(特殊地名设施[spec_id][1], 特殊地名设施[spec_id][2]);
      int city_id = pk::get_city_id(pos0);
      if (city_id == -1) return "";
      return pk::decode(pk::get_name(pk::get_city(city_id)));
    }

    pk::point getEmptyPos(pk::building@ building)
    {
      auto arr = pk::range(building.get_pos(), 1, 1);
      for (int i = 0; i < int(arr.length); i++)
      {
        pk::hex@ hex = pk::get_hex(arr[i]);
        int terrain_id = hex.terrain;

        if (!hex.has_building and !hex.has_unit
          and pk::is_valid_terrain_id(terrain_id)
          and pk::is_enabled_terrain(terrain_id)
          and terrain_id != 地形_山 and terrain_id != 地形_岸 and terrain_id != 地形_河 and terrain_id != 地形_川 and terrain_id != 地形_海)
          return arr[i];

      }
      return -1;
    }

    bool run_order(int spec_id)
    {
      pk::point pos0 = pk::point(特殊地名设施[spec_id][1], 特殊地名设施[spec_id][2]);
      pk::play_se(120);
      pk::special_cutin(126,1000);
      pk::wait(1000);
      pk::building@ target_building = pk::get_building(pos0);
      pk::point target_pos = getEmptyPos(target_building);
      if (pk::rand_bool(90))
      {
        if (pk::is_in_screen(src_unit.pos))
        {
          pk::play_se(81, src_unit.pos);
          pk::create_effect(63, src_unit.pos);
          pk::wait(1200);
        }

        if (pk::is_in_screen(target_pos)) pk::create_effect(75, target_pos);
        pk::set_pos(src_unit, target_pos);
        pk::update_unit(src_unit);


        string unit_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(src_unit)));
        string action_name = pk::format("\x1b[27x{}\x1b[0x", "传送阵的影响");
        int 低智伤害 = src_unit.attr.stat[部队能力_智力] > 100 ? 0 : 100 - src_unit.attr.stat[部队能力_智力];
        int 低防伤害 = src_unit.attr.stat[部队能力_防御] > 100 ? 0 : 100 - src_unit.attr.stat[部队能力_防御];
        int troops = pk::rand_bool(50) ? 0 : int((低智伤害 + 低防伤害) * src_unit.troops / 100);
        ch::add_troops(src_unit, -troops, true);
        if (troops > 0)
        {
          pk::history_log(src_unit.pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("{}队由于{}，大量士兵迷失在次元空间里", unit_name, action_name)));
        }

        bool 眩晕防御 = src_unit.has_skill(特技_飞将)
        or src_unit.has_skill(特技_遁走)
        or src_unit.has_skill(特技_护卫)
        or src_unit.has_skill(特技_鬼门)
        or src_unit.has_skill(特技_神算)
        or src_unit.has_skill(特技_枪神)
        or src_unit.has_skill(特技_戟神)
        or src_unit.has_skill(特技_弓神)
        or src_unit.has_skill(特技_骑神)
        or src_unit.has_skill(特技_工神)
        or src_unit.has_skill(特技_水神)
        or src_unit.has_skill(特技_霸王)
        or src_unit.has_skill(特技_兵圣);

        if (pk::rand_bool(50) and !眩晕防御)
        {
          pk::set_status(src_unit, null, 部队状态_混乱, pk::rand(1), true);
          pk::history_log(src_unit.pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("{}队由于{}，陷入了昏迷", unit_name, action_name)));
        }
        pk::say(pk::encode("仙人法器，妙用无穷"), src_leader);
        return true;
      }
      else
      {
        pk::say(pk::encode("怎么可能失败了..."), src_leader);
        return false;
      }
    }

    bool handler_传送阵(pk::point dst_pos)
    {
      pk::list<pk::city@> city_list = get_able_city(force_);//获得可以分封的武将数
      pk::list<pk::city@> list_target = pk::city_selector2(pk::encode("选择封地对应城市"), pk::encode("选择封地对应城市."), city_list, 1, 1);
      if (list_target.count == 0) return false; // 未选擇时取消结束

      array<int> spec_arr = get_city_spec(list_target[0], force_);

      int spec_id = choose_spec(spec_arr);
      bool result = run_order(spec_id);

      pk::add_energy(src_unit, -计略气力消耗, true);
      pk::add_gold(src_unit, -1000, true);
      pk::add_stat_exp(src_unit, 武将能力_智力, result ? 10 : 0);

      src_unit.action_done = true;
      if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
        src_unit.action_done = false;

      return true;
    }

    // ----------------- 封地补给 -----------
    void city_init(pk::building@ building)
    {
      @src_building = @building;
      @force_ = pk::get_force(building.get_force_id());
      @city_ = pk::building_to_city(building);
    }

    array<int> checkout_city_spec(pk::city@ city, pk::force@ force)
    {
      array<int> temp;
      for (int i = 0; i < 地名_末; ++i)
      {
        pk::point pos0 = pk::point(特殊地名设施[i][1], 特殊地名设施[i][2]);
        pk::building@ building0 = pk::get_building(pos0);
        if (building0 is null) continue;
        if (building0.get_force_id() != force.get_id()) continue;
        specialinfo@ spec_t = @special_ex[i];
        if (spec_t.person == -1) continue;
        if (spec_t.troops >= 封地兵力值不补给) continue;
        int city_id = pk::get_city_id(pos0);
        if (city.get_id() != city_id) continue;
        temp.insertLast(i);
      }
      return temp;
    }

    array<string> checkout_city_spec_name(pk::city@ city, pk::force@ force)
    {
      array<string> temp;
      for (int i = 0; i < 地名_末; ++i)
      {
        pk::point pos0 = pk::point(特殊地名设施[i][1], 特殊地名设施[i][2]);
        pk::building@ building0 = pk::get_building(pos0);
        if (building0 is null) continue;
        if (building0.get_force_id() != force.get_id()) continue;
        specialinfo@ spec_t = @special_ex[i];
        if (spec_t.person == -1) continue;
        if (spec_t.troops >= 封地兵力值不补给) continue;
        int city_id = pk::get_city_id(pos0);
        if (city.get_id() != city_id) continue;
        temp.insertLast(pk::encode(特殊地名设施名称[i]));
      }
      return temp;
    }

    bool isEnabled_封地补给()
    {
      if (int(checkout_city_spec(city_, force_).length) == 0) return false;
      if (int(pk::get_district(pk::get_district_id(force_, 1)).ap) < 封地补给行动力) return false;
      if (int(force_.tp) < 封地补给技巧值) return false;
      return true;
    }
    string getDesc_封地补给()
    {
      if (int(checkout_city_spec(city_, force_).length) == 0)
        return pk::encode(pk::format("当前城市没有兵力不足{}的封地", 封地兵力值不补给));
      else if (int(pk::get_district(pk::get_district_id(force_, 1)).ap) < 封地补给行动力)
        return pk::encode(pk::format("行动力不足 (必须 {} 行动力)", 封地补给行动力));
      else if (int(force_.tp) < 封地补给技巧值)
        return pk::encode(pk::format("技巧值不足 (必须 {} 技巧)", 封地补给技巧值));
      else
        return pk::encode(pk::format("城市所属封地补给。(技巧P {}, 行动力 {})", 封地补给技巧值, 封地补给行动力));
    }

    bool handler_封地补给()
    {
      array<string> spec_name_list = checkout_city_spec_name(city_, force_);
      spec_name_list.insertLast(pk::encode("取消"));
      int n = pk::choose(pk::encode("请选择要补给的封地."), spec_name_list);

      if (n == int(spec_name_list.length - 1)) return false;

      array<int> spec_list = checkout_city_spec(city_, force_);
      specialinfo@ spec_t = @special_ex[spec_list[n]];
      int support_limit = pk::min(封地兵力值不补给 - spec_t.troops, pk::min(city_.troops - 1, (city_.food -1) / 2));

      string numberpad_title = pk::encode("补给数值");
      pk::int_bool numberpad_value = pk::numberpad(numberpad_title, 1, support_limit, 1, cast<pk::numberpad_t@>(function(line, original_value, current_value) { return ""; }));

      if (!numberpad_value.second) return false;

      int support_amount = numberpad_value.first;
      int get_amout = numberpad_value.first;
      string confirm_desc = pk::encode(pk::format("是否确定为\x1b[16x【{}】\x1b[0x补充\x1b[1x{}兵力\x1b[0x吗?", pk::decode(spec_name_list[n]), support_amount));
      int confirm_value = pk::choose(confirm_desc, { pk::encode("确定"), pk::encode("取消") });

      if (confirm_value == 1) return true;

      pk::add_food(city_, -support_amount * 2, true);
      ch::add_troops(city_, -support_amount, true);
      spec_t.food += support_amount * 2;
      spec_t.troops += support_amount;
      pk::point dst_pos = pk::point(特殊地名设施[spec_list[n]][1], 特殊地名设施[spec_list[n]][2]);
      pk::combat_text(support_amount, 1, dst_pos);

      auto district = pk::get_district(src_building.get_district_id());
      pk::add_ap(district, -封地补给行动力);

      ch::add_tp(force_, -封地补给技巧值, force_.get_pos());

      return true;
    }

    // ---------- 封地攻占 ----------
    pk::point get_empty_pos(pk::point pos, int distance_min, int distance_max)
    {
      array<pk::point> range_pos_arr = pk::range(pos, distance_min, distance_max);
      for (int arr_index = 0; arr_index < int(range_pos_arr.length); arr_index++)
      {
        pk::point range_pos = range_pos_arr[arr_index];
        if (!pk::is_valid_pos(range_pos)) continue;

        pk::hex@ hex = pk::get_hex(range_pos);
        if (hex.has_building) continue;
        if (hex.has_unit) continue;

        int terrain_id = hex.terrain;
        if (!pk::is_valid_terrain_id(terrain_id)) continue;
        if (!pk::is_enabled_terrain(terrain_id)) continue;

        return range_pos;
      }

      return pk::point(-1, -1);
    }

    //遍历所有武将，身??死亡或无的都可用来生成
    bool func_person_slot_available()
    {
      for (int person_id = 敌将_开始; person_id < 敌将_末; person_id++)
      {
        pk::person@ person = pk::get_person(person_id);
        if (person.mibun == 身份_无) return true;
        if (person.mibun == 身份_死亡) return true;
      }
      return false;
    }
    pk::person@ func_create_person(pk::person@ person)
    {
      pk::person@ f_person = pk::create_bandit(pk::get_person(武将_卫士));
      f_person.name_read = pk::encode("府兵");
      pk::set_district(f_person, person.get_district_id());

      f_person.base_stat[武将能力_统率] = pk::max(70, pk::min(90, int(person.base_stat[武将能力_统率] * 0.85f + ch::randint(5, 10))));
      f_person.base_stat[武将能力_武力] = pk::max(70, pk::min(90, int(person.base_stat[武将能力_武力] * 0.85f + ch::randint(5, 10))));
      f_person.base_stat[武将能力_智力] = 70 + ch::randint(5, 10);
      for (int heishu_id = 兵种_枪兵; heishu_id < 兵种_末; heishu_id++)
        f_person.tekisei[heishu_id] = person.tekisei[heishu_id] + ch::randint(-2, 1);
      f_person.rank = pk::min(81, person.rank + 8);
      f_person.skill = 特技_突袭;
      f_person.update();

      return f_person;
    }

    pk::unit@ func_create_unit(pk::point pos, pk::person@ f_person, pk::point unit_pos, int spec_id, int num/*统领数*/, int troops, int gold, int food, int force_id)
    {

      int ground_wpn = get_best_ground_wpn(f_person);//陆上兵器
      int sea_wpn = 兵器_走舸;//水上兵器
      int src_troops = troops;
      int src_gold = int(gold / num);
      int src_food = int(food / num);
      specialinfo@ spec_t = @special_ex[spec_id];
      spec_t.troops -= src_troops;
      spec_t.gold -= src_gold;
      spec_t.food -= src_food;
      int city_id = pk::get_city_id(pos);
      pk::building@ src_base = pk::city_to_building(pk::get_city(city_id));
      pk::unit@ src_unit = pk::create_unit(src_base, f_person, null, null, src_troops, ground_wpn, sea_wpn, src_gold, src_food, unit_pos);

      src_unit.energy = 100;

      return src_unit;
    }

    int get_best_ground_wpn(pk::person@ person)
    {
      int best_weapon = -1;
      int best_heishu = -1;
      for (int weapon_id = 1; weapon_id < 5; weapon_id++)
      {

        if (person.tekisei[weapon_id - 1] > best_heishu)
        {
          best_heishu = person.tekisei[weapon_id - 1];
          best_weapon = weapon_id;
        }
      }
      return best_weapon;
    }

    void kill(pk::unit@ unit, int spec_id)
    {
      specialinfo@ spec_t = @special_ex[spec_id];
      spec_t.troops += unit.troops;
      spec_t.gold += unit.gold;
      spec_t.food += unit.food;
      pk::kill(unit, true);
    }

    pk::point get_enemy_unit_pos(pk::point pos, int force_id)
    {
      //pk::unit@ dst_unit;
      int range_max = 5;
      auto range = pk::range(pos, 1, range_max);
      for (int i = 0; i < int(range.length); i++)
      {
        auto hex = pk::get_hex(range[i]);
        if (hex.has_unit)
        {
          pk::unit@ unit = pk::get_unit(range[i]);
          if (pk::is_alive(unit) and unit.type_id == 11)
          {
            int building_id = pk::get_building_id(range[i]);
            if (pk::get_building(building_id).get_force_id() != force_id) continue;//不属于己方防区的情况
            pk::force@ src_force = pk::get_force(force_id);
            if (unit.get_force_id() != force_id and src_force.ally[unit.get_force_id()])
            {
              return unit.pos;
            }
          }
        }

      }
      return pk::point(-1, -1);
    }

    void func_府兵部队_生成(pk::point pos, pk::person@ src_person, pk::point dst_pos, int spec_id, int troops, int gold, int food, int force_id)
    {
      pk::point unit_pos = get_empty_pos(pos, 1, 2);
      if (!pk::is_valid_pos(unit_pos)) return ;
      if (!func_person_slot_available()) return ; //沒有空余武将位则不生成
      pk::person@ f_person = func_create_person(src_person);
      if (!pk::is_alive(f_person)) { pk::reset(f_person); return; }
      pk::unit@ src_unit = func_create_unit(pos, f_person, unit_pos, spec_id, 1, troops, gold, food, force_id);
      if (int(pk::get_movable_pos(src_unit).length) == 0)
      {
        kill(src_unit, spec_id);
        pk::reset(f_person);
      }

      pk::set_order(src_unit, 部队任务_歼灭, dst_pos);
      pk::say(pk::encode("刚破坏本大爷家!"), pk::get_person(src_unit.leader));
      pk::core::add_hidden_skill(src_unit.leader, 特技_强运);
      pk::run_order(src_unit);

    }

    void afterkillbuilding(/*pk::unit@ attacker,*/ int force_id, int facility_id, const pk::point& in pos)
    {
      if (特定地点特定设施可争夺)
      {
        if (地名设施.find(facility_id) >= 0)
        {
          for (int i = 0; i < int(特殊地名设施.length); i++)
          {
            pk::point pos0;
            pos0.x = 特殊地名设施[i][1];
            pos0.y = 特殊地名设施[i][2];
            facility_id = 特殊地名设施[i][0];
            if (pos0 == pos)
            {
              specialinfo@ spec_t = @special_ex[i];
              if (spec_t.person >= 0)
              {
                pk::point dst_pos = get_enemy_unit_pos(pos, force_id);
                func_府兵部队_生成(pos, pk::get_person(spec_t.person), dst_pos, i, spec_t.troops, spec_t.gold, spec_t.food, force_id);
              }
            }
          }
        }
      }
    }
  }

  Main main;
}

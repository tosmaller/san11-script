

namespace 传送阵 {
  const int 计略气力消耗 = 30;
  const int 兵力条件_通用 = 5000;

  class Main
  {
    pk::random random(pk::rand());

    Main() {
      add_menu_unit_order();
    }

    pk::unit@ src_unit;
    pk::person@ src_leader;
    pk::force@ force_;
    pk::point src_pos_;


    bool 开启_传送阵 = true;

    void add_menu_unit_order()
    {
      pk::menu_item 传送阵;
      传送阵.menu = global_menu::菜单_部队;
      传送阵.init = pk::unit_menu_item_init_t(init);
      传送阵.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_名称('传送阵', 计略气力消耗); });
      传送阵.get_desc = pk::menu_item_get_desc_t(getDesc_传送阵);
      传送阵.is_visible = pk::menu_item_is_visible_t(isVisible_传送阵);
      传送阵.is_enabled = pk::menu_item_is_enabled_t(isEnabled_传送阵);
      传送阵.handler = pk::unit_menu_item_handler_t(handler_传送阵);
      pk::add_menu_item(传送阵);
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

    void init(pk::unit@ unit, pk::point src_pos)
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
  }

  Main main;
}

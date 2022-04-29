
/*
地点事件-天雷渊

触发时机
  1 每年3月，随机在无城池关卡港口区域出现一圈落雷，持续3个月？

效果
  1 生成回合破坏区域内全部部队跟建筑
  2 每回合给予所处区域部队落雷伤害
  3 每回合令区域内武将受伤，且受伤不断叠加，直至死亡
  4 连续待2个月不死亡武将，获得隐藏特技【鬼门】

*/

namespace 地点事件_天雷渊 {
  class Main
  {
    // 随机数方法
    pk::random random(pk::rand());
    pk::point event_pos;
    bool in_event_time = false;
    array<pk::effect_handle> effects;

    Main() {
      pk::bind(102,  pk::trigger102_t(load));
      pk::bind2(108, pk::trigger108_t(onMonthStart), 10);
      pk::bind2(107, pk::trigger107_t(onNewDay), 9);
      pk::bind(104, pk::trigger104_t(onScenarioEnd));
      // pk::bind2(107, pk::trigger107_t(onNewDay), 9);
    }

    void 历史日志(pk::person@ person, string name, string result)
    {
      string person_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(person)));
      string action_name = pk::format("\x1b[27x{}\x1b[0x", name);

      string str = pk::format("{}通过了{}的试炼，{}", person_name, action_name, result);

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

    void onMonthStart()
    {
      地点事件();
      // if (pk::get_month() == 3) 地点事件();
    }

    void onNewDay()
    {
      if (in_event_time) 地点处理事件();
    }

    void load()
    {
      if (pk::get_scenario().loaded)
      {
        init_event();
      }
    }

    void onScenarioEnd()
    {
      for (int i = 0; i < int(effects.length); i++)
      {
        pk::remove(effects[i]);
      }
    }

    void init_event()
    {
      in_event_time = setting_event.天雷试炼标记;
      if (in_event_time)
      {
        event_pos = pk::point(setting_event.试炼坐标_X, setting_event.试炼坐标_Y);
        pk::trace(setting_event.试炼坐标_X + ':' + setting_event.试炼坐标_Y);
        array<pk::point> ranges = pk::range(event_pos, 0, 3);
        for (int l = 0; l < int(ranges.length); l++)
        {
          if (pk::is_valid_pos(ranges[l]))
          {
            pk::point pos = ranges[l];
            pk::effect_handle effect = pk::create_effect(0x45, pos);
            effects.insertLast(effect);
          }
        }
      }
    }

    pk::point get_pos()
    {
      pk::point center_pos = pk::point(100, 100);
      array<pk::point> range = pk::range(center_pos, 0, 100);
      array<pk::point> result;
      for (int l = 0; l < int(range.length); l++)
      {
        pk::hex@ hex = pk::get_hex(range[l]);
        int terrain_id = hex.terrain;
        pk::building@ building = pk::get_building(range[l]);
        if (pk::is_valid_terrain_id(terrain_id) and terrain_id <= 地形_山 and building !is null and ch::to_spec_id(building.get_id()) == -1)
        {
          array<pk::point> cur_range = pk::range(range[l], 0, 6);
          bool check = true;
          for (int i = 0; i < int(cur_range.length); i++)
          {
            pk::hex@ cur_hex = pk::get_hex(cur_range[i]);
            int cur_terrain_id = cur_hex.terrain;
            pk::building@ cur_building = pk::get_building(cur_range[i]);
            if (!pk::is_valid_terrain_id(cur_terrain_id) or cur_terrain_id > 地形_山 or (cur_building !is null and ch::to_spec_id(building.get_id()) >= 0))
            {
              check = false;
              break;
            }
          }
          if (check) result.insertLast(range[l]);
        }
      }
      return result[pk::rand(int(result.length))];
    }

    void 清除地点事件()
    {
      if (in_event_time)
      {
        in_event_time = false;
        array<pk::point> range = pk::range(pos, 0, 3);
        for (int l = 0; l < int(effects.length); l++)
        {
          pk::remove(effects[l]);
        }
      }
    }

    bool is_person_in_thunder(pk::point pos)
    {
      bool result = false;
      array<pk::point> ranges = pk::range(event_pos, 0, 3);
      for (int l = 0; l < int(ranges.length); l++)
      {
        if (ranges[l].x == pos.x and ranges[l].y == pos.y)
        {
          result = true;
          break;
        }
      }
      return result;
    }

    void 地点事件()
    {
      if (!setting_event.天雷试炼标记)
      {
        event_pos = get_pos();
        if (pk::is_valid_pos(event_pos))
        {
          in_event_time = true;
          pk::scene(cast<pk::scene_t@>(function() { pk::move_screen(main.event_pos, 500); }));
          pk::message_box(pk::encode('据说，今年的\x1b[1x天雷渊\x1b[0x试炼之地开启了'));
          setting_event.天雷试炼标记 = true;
          setting_event.试炼坐标_X = event_pos.x;
          setting_event.试炼坐标_Y = event_pos.y;
          for (int l = 0; l < 3; l++)
          {
            create_effect(l);
            pk::wait(500);
          }
        }
      }
    }

    void create_effect(int range)
    {
      array<pk::point> ranges = pk::range(event_pos, range, range);
      for (int l = 0; l < int(ranges.length); l++)
      {
        if (pk::is_valid_pos(ranges[l]))
        {
          pk::point pos = ranges[l];
          pk::create_effect(0x40, pos);
          pk::hex@ hex = pk::get_hex(pos);
          if (hex.has_unit) {
            pk::unit@ unit = pk::get_unit(pos);
            pk::say(pk::encode('糟了！怎么会选中这个地方作为试炼地。。'), pk::get_person(unit.leader));
            pk::kill(unit);
          };
          if (hex.has_building) pk::kill(pk::get_building(pos));
          pk::effect_handle effect = pk::create_effect(0x45, pos);
          effects.insertLast(effect);
        }
      }
    }

    bool effect_callback(pk::effect_handle& effect_handle, int delta)
    {
      return true;
    }

    void 地点处理事件()
    {
      array<pk::point> ranges = pk::range(event_pos, 0, 3);
      for (int l = 0; l < int(ranges.length); l++)
      {
        if (pk::is_valid_pos(ranges[l]))
        {
          pk::point pos = ranges[l];
          pk::create_effect(0x40, pos);
          pk::hex@ hex = pk::get_hex(pos);
          if (hex.has_building) pk::kill(pk::get_building(pos));
          if (hex.has_unit)
          {
            pk::unit@ unit = pk::get_unit(pos);
            ch::add_troops(unit, -ch::randint(1, 1000));
            if (unit.troops == 0)
            {
              pk::say(pk::encode('还是不行啊'), pk::get_person(unit.leader));
              pk::kill(unit);
            }
            else
            {
              pk::say(pk::encode('顶住！'), pk::get_person(unit.leader));
              for (int i = 0; i < 3; i++)
              {
                pk::person@ member = pk::get_person(unit.member[i]);
                if (member is null || !pk::is_alive(member)) continue;
                event_personinfo@ person_t = @person_event[member.get_id()];
                pk::trace('天雷时长区域' + is_person_in_thunder(pk::point(person_t.武将坐标_X, person_t.武将坐标_Y)));
                if (is_person_in_thunder(pk::point(person_t.武将坐标_X, person_t.武将坐标_Y)))
                {
                  person_t.天雷试炼时长 += 1;
                  if (person_t.天雷试炼时长 < 6) person_t.天雷试炼时长 += 1;
                  else if(!ch::has_skill(member, 特技_鬼门))
                  {
                    pk::core::add_hidden_skill(member.get_id(), 特技_鬼门);
                    历史日志(member, '天雷渊', pk::format("习得特技【\x1b[1x{}\x1b[0x】", pk::decode(pk::get_skill(特技_鬼门).name)));
                    pk::message_box(pk::encode(pk::format('{}通过了\x1b[1x天雷渊\x1b[0x的试炼，习得了特技【\x1b[1x{}\x1b[0x】', pk::decode(pk::get_name(member)), pk::decode(pk::get_skill(特技_鬼门).name))));
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  Main main;
}

/*
作者：黑店小小二
一击必杀
 */

namespace 人物一击必杀 {
  pk::func209_t@ prev_callback_209;


  class Main
  {
    pk::random random(pk::rand());
    array<int> 一击必杀能力部队 = { 武将_刘禅, 武将_刘备 };
    Main() {
      @prev_callback_209 = cast<pk::func209_t@>(pk::get_func(209));
      pk::reset_func(209);
      pk::set_func(209, pk::func209_t(func209));

      pk::bind(170, pk::trigger170_t(onUnitCreate));
    }

    pk::unit@ src_unit;

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

      string str = pk::format("{}队由于{}，{}", unit_name, action_name, result);

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

    void onUnitCreate(pk::unit@ unit, int type)
    {
      if (type == 0)
      {
        for (int m = 0; m < 3; m++)
        {
          if (pk::is_valid_person_id(unit.member[m]))
          {
            pk::person@ member_t = pk::get_person(unit.member[m]);  //隊伍中的武將
            if (member_t is null or !pk::is_alive(member_t)) continue;
            if (一击必杀能力部队.find(member_t.get_id()) >= 0)
            {
              历史日志(unit, '开挂', pk::format("获得\x1b[27x{}\x1b[0x能力！", '一击必杀'));
            }
          }
        }
      }
    }

    void func209(pk::damage_info& info, pk::unit@ attacker, int tactics_id, const pk::point& in target_pos, int type, int critical, bool ambush, int rettype)
    {
      prev_callback_209(info, attacker, tactics_id, target_pos, type, critical, ambush, rettype);
      一击必杀_伤害处理(info, attacker, target_pos);
    }

    void 一击必杀_伤害处理(pk::damage_info& info, pk::unit@ attacker, const pk::point& in target_pos)
    {
      bool 一击必杀效果 = false;
      for (int m = 0; m < 3; m++)
      {
        if (pk::is_valid_person_id(attacker.member[m]))
        {
          if (一击必杀能力部队.find(attacker.member[m]) >= 0)
          {
            一击必杀效果 = true;
            break;
          }
        }
      }
      if (一击必杀效果)
      {
        pk::building@ building = pk::get_building(target_pos);
        pk::unit@ unit = pk::get_unit(target_pos);
        if (building !is null and building.facility != 设施_城市 and building.facility != 设施_关隘 and building.facility != 设施_港口)
        {
          info.hp_damage = building.hp;
        }
        else if (unit !is null)
        {
          info.troops_damage = info.dst_troops;
        }
        info.critical = true;
      }
    }
  }
  Main main;
}

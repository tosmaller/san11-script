
/*
水土不服事件
条件：
1 武将武力低于70；
2 所在城市归属州与出身地不符
3 武将在当前所属州待的总时长不足3年（即108个回合，总时长，非连续时长）
3 月初5%概率出现水土不服情况
结果：
  健康 -> 轻伤
  已受伤 -> 加重，最多重症(考虑游戏内本身武将有一定几率病症加重，这里控制到重症)
*/

namespace 事件_水土不服 {

  class Main
  {
    // 随机数方法
    pk::random random(pk::rand());
    Main() {
      pk::bind(108, pk::trigger108_t(onMonthBegin));
      pk::bind(112, pk::trigger111_t(onTurnEnd));
    }

    // 武将的历史日志
    void 历史日志(pk::person@ person, string name, string result)
    {
      string person_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(person)));
      string action_name = pk::format("{}", name);

      string str = pk::format("{}由于{}，{}", person_name, action_name, result);

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

    void onTurnEnd(pk::force@ force)
    {
      if (force.get_id() < 非贼势力_末)
      {
        pk::list<pk::person@> person_list = pk::get_person_list(force, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般, 身份_俘虏));
        for (int i = 0; i < person_list.count; ++i )
        {
          pk::person@ person_s = person_list[i];
          event_personinfo@ person_t = @person_event[i];
          int province_id = pk::get_province_id(person_s.get_pos());
          person_t.set_地区时长(province_id, person_t.get_地区时长(province_id) + 1);
        }
      }
    }

    void onMonthBegin()
    {
      for (int i = 0; i < 非贼武将_末; i++)
      {
        pk::person@ person_s = pk::get_person(i);
        event_personinfo@ person_t = @person_event[i];
        array<int> mibun_list = { 身分_无, 身分_未登场, 身分_未发现, 身分_死亡, 身分_末 };
        if (person_s is null
          or !pk::is_alive(person_s)
          or (mibun_list.find(person_s.mibun) >= 0))
          or pk::is_valid_pos(person_s.get_pos())
            continue;
        int province_id = pk::get_province_id(person_s.get_pos());
        if (person_t.get_地区时长(province_id) >= 3) continue;
        if (person_s.base_stat[武将能力_武力] < 70
          and pk::rand_bool(5)
          and person_s.shoubyou < 伤病_重症
          and pk::get_province_id(person_s.get_pos()) != person_s.birthplace)
        {
          person_s.shoubyou += 1;
          array<string> status = { 伤病_健康, 伤病_轻症, 伤病_重症, 伤病_濒死 };
          string action = pk::format("在\x1b[1x{}\x1b[0x水土不服", pk::decode(pk::get_province(province_id).name));
          历史日志(person_s, action, pk::format("\x1b[27x{}\x1b[0x", '染上了疾病'));
        }
      }
    }
  }

  Main main;
}

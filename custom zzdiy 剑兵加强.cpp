
/*
剑兵加强

剑兵击破非运输部队
  ·运输部队存在兵装大于剑兵兵力的类别（枪戟弩骑），随机获得其中一项
剑兵击破武装部队
  ·兵力小于武装部队，变为跟其一样的兵装
*/

namespace 剑兵加强 {

  class Main
  {
    // 随机数方法
    pk::random random(pk::rand());
    Main() {
      pk::bind(171, pk::trigger171_t(部队清除));
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

    string get_weapon_unit_name(int weapon_id)
    {
      string weapon_name;
      switch (weapon_id)
      {
      case 兵器_枪: weapon_name = "枪兵"; break;
      case 兵器_戟: weapon_name = "戟兵"; break;
      case 兵器_弩: weapon_name = "弩兵"; break;
      case 兵器_战马: weapon_name = "骑兵"; break;
      }
      return weapon_name;
    }

    void 部队清除(pk::unit@ target, pk::hex_object@ dst, int type)
    {
      if (type == 0)
      {
        pk::unit@ attacker = pk::hex_object_to_unit(dst);
        if (attacker !is null and attacker.weapon == 兵器_剑)
        {
          if (target.type == 部队类型_运输)
          {
            array<int> weapons;
            for (int i = 兵器_枪; i <= 兵器_战马; i++)
            {
              if (int(target.stock[i].amount) >= int(attacker.troops))
                weapons.insertLast(i);
            }
            if (int(weapons.length) > 0)
            {
              int _weapon = random(0, int(weapons.length) - 1);
              int weapon_id = weapons[_weapon];
              attacker.weapon = weapon_id;
              attacker.stock[0].id = weapon_id;
              pk::update_unit(attacker);
              pk::say(pk::encode(pk::format("哈哈哈，感谢老铁送来的\x1b[1x{}\x1b[0x", ch::get_weapon_name(weapon_id))), pk::get_person(attacker.leader));
              历史日志(attacker, '剑兵加强', pk::format("变为了【\x1b[1x{}\x1b[0x】", get_weapon_unit_name(weapon_id)));
            }
          }
          else
          {
            pk::trace('兵力：' + attacker.troops + ';兵力：' + target.troops);
            if (int(attacker.troops) <= int(target.troops))
            {
              int weapon_id = target.weapon;
              attacker.weapon = weapon_id;
              attacker.stock[0].id = weapon_id;
              pk::update_unit(attacker);
              pk::say(pk::encode(pk::format("看我变化自如，感谢老铁送来的\x1b[1x{}\x1b[0x", ch::get_weapon_name(weapon_id))), pk::get_person(attacker.leader));
              历史日志(attacker, '剑兵加强', pk::format("变为了【\x1b[1x{}\x1b[0x】", get_weapon_unit_name(weapon_id)));
            }
          }
        }

      }
    }


  }

  Main main;
}

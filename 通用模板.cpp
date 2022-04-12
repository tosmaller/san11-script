
/*
开始前请分析好动作的步骤
比如奏乐，效果是每回合回复5气力

这里拆分为发动时机 + 目标 + 条件 + 效果。
奏乐规范描述的话则是，回合开始时，有奏乐效果的部队，回复气力5
其中【回合开始时】是触发时机
【有奏乐效果的部队】是目标
【回复气力5】是效果
该技能无需条件，后面再讲
一一对应后，则是一个完整的效果
*/

namespace 通用模板 {

  class Main
  {
    // 随机数方法
    pk::random random(pk::rand());

    // 常用参数
    pk::unit@ 部队;
    pk::person@ 部队主将;
    pk::point 部队位置;
    pk::force@ 势力;

    Main() {
      pk::bind(108, pk::trigger108_t(每月开始时));
      pk::bind(111, pk::trigger111_t(回合开始时));
      pk::bind(112, pk::trigger111_t(回合结束时));
      pk::bind(171, pk::trigger171_t(部队消失时));
      pk::bind(174, pk::trigger174_t(部队行动结束后));
    }
    // ------------------ 通用 这里可以不用看，忽略，使用时会说明------------------

    // 武将的历史日志
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

    // 部队的历史日志
    void 历史日志(pk::unit@ unit, string name, string result)
    {
      string unit_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(unit)));
      string action_name = pk::format("\x1b[27x{}\x1b[0x", name);

      string str = pk::format("{}队由于{}的效果，{}", unit_name, action_name, result);

      pk::force@ unit_force = pk::get_force(unit.get_force_id());
      int color = unit_force is null ? -1 : unit_force.color;
      pk::history_log(unit.pos, color, pk::encode(str));
    }

    // 建筑的历史日志
    void 历史日志(pk::building@ building, string name, string result)
    {
      string building_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(building)));
      string action_name = pk::format("\x1b[27x{}\x1b[0x", name);

      string str = pk::format("{}由于{}的效果，{}", building_name, action_name, result);
      pk::force@ building_force = pk::get_force(building.get_force_id());
      int color = building_force is null ? -1 : building_force.color;
      pk::history_log(building.pos, color, pk::encode(str));
    }

    // 获取指定范围内所有的位置
    pk::array<pk::point_int> getTargets_目标(int range, pk::point 指定位置 = 部队位置)
    {
        pk::array<pk::point_int> targets;
        array<pk::point> rings = pk::range(指定位置, 1, range);
        for (int pos_index = 0; pos_index < int(rings.length); pos_index++)
        {
          pk::point dst_pos = rings[pos_index];
          targets.insertLast(pk::point_int(dst_pos, 1));
        }

        return targets;
    }

    // 获取指定范围内所有的部队的位置
    pk::array<pk::point_int> getTargets_目标部队位置(int range, pk::point 指定位置 = 部队位置)
    {
        pk::array<pk::point_int> targets;
        array<pk::point> rings = pk::range(指定位置, 1, range);
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

    // 获取指定范围内所有的部队
    pk::list<pk::unit@> getTargets_目标部队(int range, pk::point 指定位置 = 部队位置)
    {
        pk::list<pk::unit@> targets;
        array<pk::point> rings = pk::range(指定位置, 1, range);
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

    // 获取指定范围内某个势力所有的部队
    pk::list<pk::unit@> getTargets_势力_目标部队(pk::force@ 势力, int range, pk::point 指定位置 = 部队位置)
    {
        pk::list<pk::unit@> targets;
        array<pk::point> rings = pk::range(指定位置, 1, range);
        for (int i = 0; i < int(rings.length); i++)
        {
          pk::point dst_pos = rings[i];
          pk::unit@ dst = pk::get_unit(dst_pos);
          if (dst !is null and dst.get_force_id() == 势力.get_force_id())
          {
            targets.add(dst);
          }
        }

        return targets;
    }

    pk::list<pk::unit@> 获取全地图部队()
    {
      return pk::get_unit_list();
    }

    // 部队是否有某个武将
    bool 部队是否有某个武将(pk::unit@ unit, pk::person@ person)
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
        // pk::kill(src_unit);
        pk::wait(2000);
        pk::kill(person);
      }
    }

    // 兵力变化值，正数则回复兵力，负数则扣减兵力
    void 部队兵力变化值(pk::unit@ 部队, int 兵力变化值)
    {
      ch::add_troops(部队,兵力变化值, true);
    }

    // 气力变化值，正数则回复气力，负数则扣减气力
    void 部队气力变化值(pk::unit@ 部队, int 气力变化值)
    {
      pk::add_energy(部队, 气力变化值, true);
    }

    // 金钱变化值，正数则增加金钱，负数则扣减金钱
    void 部队金钱变化值(pk::unit@ 部队, int 金钱变化值)
    {
      pk::add_gold(部队, 金钱变化值, true);
    }

    // 粮食变化值，正数则增加粮食，负数则扣减粮食
    void 部队粮食变化值(pk::unit@ 部队, int 粮食变化值)
    {
      pk::add_food(部队, 粮食变化值, true);
    }

    // 特技编号可以查看kre的特技列表，0-飞将，以此类推
    bool 部队是否拥有指定特技(pk::unit@ 部队, int 特技编号)
    {
      return ch::has_skill(部队, 特技编号);
    }

    // 传入部队跟势力，返回传入的部队是否为传入的势力
    bool 部队是否为某个势力(pk::unit@ 部队, pk::force@ 当前势力)
    {
      return 部队.get_force_id() == 当前势力.get_id();
    }

    // 获取部队的属性，比如兵力，气力，金钱，粮食，状态
    int 部队属性(pk::unit@ 部队, string arg)
    {
      dictionary 部队属性;
      部队属性.set('兵力', 部队.troops);
      部队属性.set('气力', 部队.energy);
      部队属性.set('金钱', 部队.gold);
      部队属性.set('粮食', 部队.food);
      部队属性.set('状态', 部队.status);
      return int(部队属性[arg]);
    }

    pk::point 获取部队位置(pk::unit@ 部队)
    {
      return 部队.get_pos();
    }

    // ------------------------------------------------
    void 每月开始时()
    {

    }
    void 回合开始时(pk::force@ 当前势力)
    {
      // 奏乐的触发时机是回合开始，所以先在这里这样写
      // 括号里的是参数，这里用来控制轮到有奏乐特技的武将的势力是才执行加气动作
      奏乐(当前势力);
    }
    void 回合结束时(pk::force@ 当前势力)
    {
    }
    void 部队消失时(pk::unit@ 部队, pk::hex_object@ dst, int 部队消失类型)
    {

    }

    void 部队行动结束后(pk::unit@ 部队,int 行动结束类型)
    {

    }

    // 方法模板
    void 效果名称()
    {

    }


    // 下面开始写奏乐效果，复制上面的模板，效果名称改为自己想要的，不要重复
    // 【pk::force@ 当前势力】 设置参数类型跟参数名称
    void 奏乐(pk::force@ 当前势力)
    {
      // 奏乐的效果是拥有此技能的部队，每回合回气5；

      // 第一步：获取全地图的部队

      // 这里，【pk::list<pk::unit@>】是类型，我们常用的还有int 跟bool, 【全地图部队】是变量名，理解为容器，【获取全地图部队()】是函数，理解为数学里的函数，执行后会返回内容
      pk::list<pk::unit@> 全地图部队 = 获取全地图部队();

      // 第二步，带特技奏乐的部队，并回复气力

      // 这里我们用到一个循环，跟高中数学流程图一样的内容，不理解的先看看
      for (int i = 0; i < 全地图部队.count; i++)
      {
        // 全地图部队[i]这一句可以获取具体的部队，可以不用理解，记住就行
        pk::unit@ 临时部队 = 全地图部队[i];

        // 这里调用上面写的【部队是否拥有指定特技】函数，这个函数会返回true跟false
        // 【特技_奏乐】来自与300 S11KP常量.cpp 内置的全局常量，不清楚可以查询，这里也可以直接写特技序号
        //
        if (部队是否拥有指定特技(临时部队, 特技_奏乐) and 部队是否为某个势力(临时部队, 当前势力))
        {
          // 部队有这个特技，则执行到此处

          // 这里标识部队回复气力20，到这一步，基本完成了
          // 部队气力变化值(临时部队, 20);

          // 也可以加个代价，兵力减少20，这样的话我们先判断部队的兵力是否大于20，不大于20的也没效果
          if (部队属性(临时部队, '兵力') > 20) // 这里判断兵力是否大于20，兵力是内置属性，这里要加上引号
          {
            部队兵力变化值(临时部队, -20);
            部队气力变化值(临时部队, 20);

            // 以上就是完整效果了，我们再加个日志窗口的输出记录

            // 这里传三个参数，第一个参数是部队/武将/建筑
            // 第二个参数可以传动作名称
            // 第三个参数可以传动作结果
            // 像下面这样传，会输出 xx部队由于奏乐的效果，回复气力20
<<<<<<< HEAD
            历史日志(临时部队, '奏乐的效果', '回复气力20');
=======
            历史日志(临时部队, '奏乐', '回复气力20');
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
          }
        }
      }
    }
  }

  Main main;
}

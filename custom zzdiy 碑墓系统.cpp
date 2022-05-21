/*
碑墓系统：
时机
  武将死亡时
条件
  满足任一条件
  1 武将功绩 大于 3万
  2 身份为君主，都督，太守
  3 有任一五维达到95+
  4 有任一适性达到 极以上
结果
  在所属城市周围2~6歌随机一格，根据不同身份生成不同建筑
  1 只满足功绩大于3万，建筑为【落石】
  2 满足五维或者适性，建筑为随机【阵，砦，城塞，弓橹，箭楼，连弩橹，连弩楼，土垒，石壁，投石台】中一种
  3 身份为君主，建筑为【石兵八阵】,身份为都督，建筑为【军乐台】, 身份为太守，建筑为【太鼓台】

附加盗墓事件
  碑墓建筑被破坏时，根据相性，所属等条件
  触发以下任意结果
    1 冤魂索命
    2 传授经验
    3 夺舍
    4 复活
 */

namespace 碑墓系统 {
  const int KEY = pk::hash("碑墓系统");
  pk::random random(pk::rand());

  int get_tomb_building(pk::building@ building)
  {
    int tomb_id = -1;
    for (int i = 0; i < 碑墓建筑_末; i++)
    {
      if (info_tomb[i].buildingId == building.get_id())
      {
        tomb_id = i;
        break;
      }
    }
    if (tomb_id > -1)
    {
      tomb_info@ tomb_info = @info_tomb[tomb_id];
      if (!info_tomb[tomb_id].name.isEmpty())
      {
        return tomb_id;
      }
    }
    return -1;
  }

  class Main {
    file f;
    pk::func209_t@ prev_callback_209;
    pk::unit@ attack_unit;
    int break_tomb_id;
    Main()
    {
      pk::bind(102, -1, pk::trigger102_t(剧本初始化_结构体_信息读取));
      pk::bind(105, pk::trigger105_t(存档));
      pk::bind(106, pk::trigger105_t(读档));
      pk::bind(151, pk::trigger151_t(武将死亡));
      pk::bind(120, pk::trigger120_t(建筑信息显示));
      // pk::bind(163, pk::trigger163_t(建筑破坏前));
      pk::bind2(161, pk::trigger161_t(killbuilding), 1);

      // debug
      // pk::bind(179, pk::trigger179_t(afterAttack));

      @prev_callback_209 = cast<pk::func209_t@>(pk::get_func(209));
      pk::reset_func(209);
      pk::set_func(209, pk::func209_t(func209));
    }

    void 历史日志(pk::person@ leader, pk::person@ person, pk::point pos)
    {
      string leader_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(leader)));
      string person_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(person)));
      string str = pk::format("{}听闻{}逝世，感其功绩，为其在\x1b[27x（{}, {}）\x1b[0x修建了墓地", leader_name, person_name, pos.x, pos.y);
      if (leader.get_id() == person.get_id())
      {
        str = pk::format("{}后世子孙因其功绩，为其在\x1b[27x（{}, {}）\x1b[0x修建了墓地", leader_name, pos.x, pos.y);
      }

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

    string get_path(string fileName)
    {
      return '../pk2.2/Koei/San11/SaveData/' + fileName;
    }

    void 存档(int file_id) //儲存
    {
      string result;
      for (int i = 0; i < 碑墓建筑_末; i++)
      {
        info_tomb[i].update(i);
        for (int j = 0; j <= (碑墓信息结构体_uint32数 - 1); j++)
          pk::store(KEY, (i * 碑墓信息结构体_uint32数 + j), tomb_info_temp[j][i]);

        if (!info_tomb[i].name.isEmpty())
        {
          result += pk::format(
            """
              <builging id="{}">
                <name value="{}"/>
                <title value="{}"/>
                <mibun value="{}"/>
                <ptitle value="{}"/>
              </builging>
            """,
            info_tomb[i].buildingId,
            info_tomb[i].name,
            info_tomb[i].title,
            info_tomb[i].mibun,
            info_tomb[i].ptitle
          );
        }
      }
      string str = file_id > 9 ? '' : '0';
      string path = get_path('Save0' + str + file_id + '.data');
      f.open(path, "w");
      f.writeString(result);
      f.close();
    }
    void 读档 (int file_id) {
      string str = file_id > 9 ? '' : '0';
      string path = get_path('Save0' + str + file_id + '.data');
      if (f.open(path, "r") >= 0)
      {
        string content = f.readString(f.getSize());
        array<array<string>> info = get_info(content);
        for (int i = 0; i < 碑墓建筑_末; i++)
        {
          for (int j = 0; j <= (碑墓信息结构体_uint32数 - 1); j++)
            tomb_info_temp[j][i] = uint32(pk::load(KEY, (i * 碑墓信息结构体_uint32数 + j), 0));

          tomb_info tomb_t(i);
          info_tomb[i].buildingId = tomb_t.buildingId;
          info_tomb[i].五维能力 = tomb_t.五维能力;
          info_tomb[i].兵种适性 = tomb_t.兵种适性;
          for (int a = 0; a <= int(info.length - 1); a++)
          {
            if (tomb_t.buildingId == parseInt(info[a][0]))
            {
              info_tomb[i].set_info(info[a]);
            }
          }
        }
      }
    }
    void 剧本初始化_结构体_信息读取()
    {
      // 重新开始游戏时，初始化数据
      if (!pk::get_scenario().loaded)
      {
        for (int i = 0; i < 碑墓建筑_末; ++i)
        {
          info_tomb[i].buildingId = -1;
          info_tomb[i].name = '';
          info_tomb[i].title = '';
          info_tomb[i].mibun = '';
          info_tomb[i].ptitle = '';
          info_tomb[i].face = -1;
          for (int ab = 0; ab < 武将能力_末; ab++)
          {
            info_tomb[i].五维能力[ab] = 0;
          }
          for (int ts = 0; ts < 兵种_末; ts++)
          {
            info_tomb[i].兵种适性[ts] = 0;
          }
        }
      }
    }

    array<array<string>> get_info(string content)
    {
      array<array<string>> result;
      array<string> str = content.split('</builging>');
      for (int i = 0; i < int(str.length - 1); i++)
      {
        string info = str[i];
        if (!info.isEmpty())
        {
          string id = info.split('id="')[1].split('">')[0];
          string name = info.split('name value="')[1].split('"/>')[0];
          string title = info.split('title value="')[1].split('"/>')[0];
          string mibun = info.split('mibun value="')[1].split('"/>')[0];
          string ptitle = info.split('ptitle value="')[1].split('"/>')[0];
          result.insertLast({ id, name, title, mibun, ptitle });
        }
      }
      return result;
    }

    void 武将死亡(pk::person@ dead, pk::person@ by, int type, int rettype)
    {
      if (dead !is null)
      {
        array<int> mibuns = { 身份_君主, 身份_都督, 身份_太守 };
        bool can_set_tonb = false;
        int 设施;
        int dead_type = -1;
        if (dead.kouseki > 3 * 10000)
        {
          can_set_tonb = true;
          设施 = 设施_落石;
        }
        else if (mibuns.find(dead.mibun) >= 0)
        {
          switch(dead.mibun)
          {
            case 身份_君主:
              设施 = 设施_石兵八阵;
              break;
            case 身份_都督:
              设施 = 设施_军乐台;
              break;
            case 身份_太守:
              设施 = 设施_太鼓台;
              break;
          }
          dead_type = 3;
          can_set_tonb = true;
        }
        else
        {
          for (int i = 0; i < 武将能力_末; i++)
          {
            if (dead.stat[i] >= 95) can_set_tonb = true;dead_type = 1;break;
          }
          if (!can_set_tonb)
          {
            for (int i = 0; i < 兵种_末; i++)
            {
              if (dead.tekisei[i] > 适性_S) can_set_tonb = true;dead_type = 2;break;
            }
          }

          设施 = random(设施_阵, 设施_投石台);
        }
        pk::trace('武将' + pk::decode(pk::get_name(dead)) + '是否修建碑墓：' + can_set_tonb + '类型：' + dead_type);
        if (can_set_tonb)
        {
          pk::building@ building = pk::get_building(dead.service);
          array<pk::point> mult_pos;
          array<pk::point> rings = pk::range(building.get_pos(), 2, 9);
          array<int> building_terrain = { 地形_草地, 地形_砂地, 地形_官道 };
          for (int i = 0; i < int(rings.length); i++)
          {
            pk::point dst_pos = rings[i];
            pk::hex@ hex = pk::get_hex(dst_pos);
            if (hex.has_unit or hex.has_building or building_terrain.find(hex.terrain) < 0) continue;
            if (hex.can_military and hex.can_trap)
              mult_pos.insertLast(dst_pos);
          }
          pk::trace('可修建地形长度：' + int(mult_pos.length));
          if (int(mult_pos.length) > 0)
          {
            pk::point res_pos = mult_pos[random(mult_pos.length - 1)];
            pk::info('pos-{' + res_pos.x + ',' + res_pos.y + '}' + ';设施:' + 设施);
            pk::facility@ facility = pk::get_facility(设施);
            pk::building@ target_building = pk::create_building(facility, res_pos, dead.get_force_id());
            pk::add_hp(target_building, facility.hp);
            pk::complete(target_building);
            int tomb_id = -1;
            for (int i = 0; i < 碑墓建筑_末; i++)
            {
              if(info_tomb[i].buildingId == -1 and info_tomb[i].name.isEmpty())
              {
                tomb_id = i;
                break;
              }
            }
            if (tomb_id > -1)
            {
              string dead_name = get_dead_name(dead);
              tomb_info@ tomb_info = @info_tomb[tomb_id];
              tomb_info.buildingId = target_building.get_id();
              tomb_info.name = pk::decode(pk::get_name(dead));
              tomb_info.title = pk::decode(dead.name_read);
              tomb_info.mibun = dead.mibun;
              tomb_info.ptitle = pk::decode(pk::encode(dead_name));
              tomb_info.face = dead.face;
              array<uint8> 五维;
              array<uint8> 适性;
              for (int i = 0; i < 武将能力_末; i++)
              {
                五维.insertLast(dead.stat[i]);
              }
              tomb_info.五维能力 = 五维;
              for (int i = 0; i < 兵种_末; i++)
              {
                适性.insertLast(dead.tekisei[i]);
              }
              tomb_info.兵种适性 = 适性;
              历史日志(pk::get_person(pk::get_force(dead.get_force_id()).kunshu), dead, res_pos);
            }
          }
        }
      }
    }

    void 建筑信息显示()
    {
      pk::point cursor_pos = pk::get_cursor_hex_pos();
      if (!pk::is_valid_pos(cursor_pos)) return;
      pk::building@ building = pk::get_building(cursor_pos);
      if (building !is null)
      {
        int tomb_id = get_tomb_building(building);
        if (tomb_id > -1)
        {
          tomb_info@ tomb_info = @info_tomb[tomb_id];
          // tomb_info.mibun = dead.mibun;
          // pk::trace(pk::format('五维： {}，{}，{}，{}，{}', tomb_info.五维能力[0], tomb_info.五维能力[1], tomb_info.五维能力[2], tomb_info.五维能力[3], tomb_info.五维能力[4]));

          // pk::trace(pk::format('适性： {}，{}，{}，{}，{}', tomb_info.兵种适性[0], tomb_info.兵种适性[1], tomb_info.兵种适性[2], tomb_info.兵种适性[3], tomb_info.兵种适性[4], tomb_info.兵种适性[5]));

          string name = pk::format('\x1b[1x{}\x1b[0x之墓', tomb_info.name);
          string title = pk::format('生前称号： \x1b[1x{}\x1b[0x', tomb_info.title);
          string ptitle = pk::format('谥号： \x1b[1x{}\x1b[0x', tomb_info.ptitle);

          int width = int(pk::get_resolution().width) - 250;
          pk::point left_down = pk::point(int(pk::get_resolution().width) - 10, 75 + 40 + 5 + 80) + 20;
          pk::draw_filled_rect(pk::rectangle(pk::point(width - 5, 15), left_down), ((0xff / 2) << 24) | 0x010101);
          pk::draw_text(pk::encode(name), pk::point(width, 20), 0xffffffff, FONT_BIG, 0xff000000);

          pk::point rightup = pk::point(width + 10, 60);
          pk::point leftdown = pk::point(width + 42, 100);
          auto rect = pk::rectangle(rightup, leftdown);
          pk::draw_face(FACE_SMALL, tomb_info.face, rect, FACE_L);

          pk::draw_text(pk::encode(title), pk::point(width + 60, 60), 0xffffffff, FONT_SMALL, 0xff000000);
          pk::draw_text(pk::encode(ptitle), pk::point(width + 60, 85), 0xffffffff, FONT_SMALL, 0xff000000);
        }
      }
    }

    void func209(pk::damage_info& info, pk::unit@ attacker, int tactics_id, const pk::point& in target_pos, int type, int critical, bool ambush, int rettype)
    {
      prev_callback_209(info, attacker, tactics_id, target_pos, type, critical, ambush, rettype);
      pk::building@ building = pk::get_building(target_pos);
      if (building !is null)
      {
        int tomb_id = get_tomb_building(building);
        pk::trace('碑墓序号：' + tomb_id + ';碑墓hp:' + building.hp + ";伤害值：" + info.hp_damage);

        if (tomb_id > -1 and int(info.hp_damage) >= int(building.hp))
        {
          break_tomb_id = tomb_id;
          @attack_unit = attacker;
        }
      }
    }

    void killbuilding(pk::building@ building, int type)
    {
      if (type == 0 and building !is null)
      {
        int tomb_id = get_tomb_building(building);
        pk::trace('碑墓序号：' + break_tomb_id);
        pk::trace('建筑序号：' + tomb_id);
        if (tomb_id > -1 and int(break_tomb_id) == int(tomb_id))
        {
          // 处理攻击此建筑部队
          /*
          1 冤魂索命
          2 传授经验
          3 夺舍
          4 复活*/
          // @attack_unit = attacker;
          tomb_info@ tomb_info = @info_tomb[tomb_id];
          pk::trace('破坏建筑者：' + pk::decode(pk::get_name(attack_unit)));
          pk::person@ leader = pk::get_person(attack_unit.leader);
          int event1_chance = 10; // 1 冤魂索命
          int event2_chance = 10; // 2 夺舍
          int event3_chance = 5; // 3 复活
          int event4_chance = 25; // 4 传授经验
          int chance = 5;//random(100);
          if (chance <= event1_chance)
          {
            pk::message_box(pk::encode('墓地破坏后，飘出了浓浓青烟，烟中慢慢浮现出一个令人胆颤的身影...'));
            pk::message_box(pk::encode('何人扰我休眠?'));
            pk::say(pk::encode("这...这是怎么回事"), leader);
            pk::wait(1000);
            pk::message_box(pk::encode('来都来了，就留下来陪我吧'));
            pk::say(pk::encode("不..不好...救...救我..."), leader);
            pk::wait(1000);
            pk::kill(leader);
            pk::kill(attack_unit);
            pk::message_box(pk::encode('随着声音越来越小，一切仿佛又回归平静...'));
          }
        }
      }
    }


    //
    void 建筑破坏前(pk::building@ building, bool voices)
    {
      for (int i = 0; i < 碑墓建筑_末; i++)
      {
        pk::trace('建筑破坏前：' + building.get_id() + ',' + i);
        // if (info_tomb[i].buildingId == building.get_id() and !info_tomb[i].name.isEmpty())
        // {

        //   tomb_info@ tomb_info = @info_tomb[i];
        //   tomb_info.clear();
        //   break;
        // }
      }
    }
    void afterAttack(pk::unit@ target, pk::unit@ attacker, int troops_damage, int critical, int tactics)
    {
      if (tactics == 战法_突进)
      {
        pk::kill(pk::get_person(target.leader), pk::get_person(attacker.leader));
      }
    }

    string get_dead_name(pk::person@ dead)
    {
      string dead_name;
      array<string> king = { '' };
      array<string> wen = {
        '正', '成', '忠', '端', '定', '简', '懿', '肃', '毅', '宪', '庄', '敬', '裕', '节', '义', '靖', '穆', '昭', '恪', '恭', '襄', '清', '修', '康', '洁', '敏', '达', '通', '介', '安', '烈', '和', '僖', '荣', '愍', '思'
      };
      array<string> wu = {
        '宁', '毅', '敏', '惠', '襄', '顺', '肃', '靖', '信', '康', '壮', '恒', '愍', '烈', '勇', '僖'
      };
      array<string> zhong = {
        '文', '武', '定', '烈', '简', '肃', '毅', '敬', '宪', '节', '贞', '靖', '襄', '敏', '安', '僖', '穆', '介', '威', '端', '壮', '宣', '裕', '果', '勇', '愍', '刚', '惠', '悫', '怀', '清'
      };
      if (pk::is_kunshu(dead))
      {
        if (pk::get_force(dead.get_force_id()).title == 爵位_皇帝)
        {
          string name = pk::decode(pk::encode(dead.sei) + pk::encode(dead.mei));
          if (name == '曹操') dead_name = '武皇帝';
          if (name == '孙权') dead_name = '大皇帝';
          if (name == '刘备') dead_name = '昭烈皇帝';
          if (name == '司马懿') dead_name = '宣皇帝';
        }
      }
      return dead_name;
    }
  }
  Main main;
}

const int 碑墓信息结构体_uint32数 = 15;
const int 碑墓建筑_末 = 4000;

array<array<uint32>> tomb_info_temp(碑墓信息结构体_uint32数, array<uint32>(碑墓建筑_末, uint32(0)));
array<tomb_info> info_tomb(碑墓建筑_末);

// 建筑id, 墓主姓名，墓主五维（2个位置），墓主适性，墓主称号，墓主身份，墓主谥号
class tomb_info {
  int ID序号 = 0;
  int 适性序号1 = 1;
  int 适性序号2 = 2;
  int 头像序号 = 3;
  int 五维序号1 = 4;
  int 五维序号2 = 5;

  int buildingId;
  int face;
  array<uint8> 五维能力(武将能力_末, 0);
  array<uint8> 兵种适性(兵种_末, 0);

  string name;
  string title;
  string mibun;
  string ptitle;

  //初始化
  tomb_info(int id)
  {
    get_info(id);
  }

  tomb_info() {}

  void get_info(int id)
  {
    fromInt32_0(tomb_info_temp[ID序号][id]);
    fromInt32_1(tomb_info_temp[适性序号1][id]);
    fromInt32_2(tomb_info_temp[适性序号2][id]);
    fromInt32_3(tomb_info_temp[头像序号][id]);
    fromInt32_4(tomb_info_temp[五维序号1][id]);
    fromInt32_5(tomb_info_temp[五维序号2][id]);
  }

  void update(int id)
  {
    tomb_info_temp[ID序号][id] = toInt32_0();
    tomb_info_temp[适性序号1][id] = toInt32_1();
    tomb_info_temp[适性序号2][id] = toInt32_2();
    tomb_info_temp[头像序号][id] = toInt32_3();
    tomb_info_temp[五维序号1][id] = toInt32_4();
    tomb_info_temp[五维序号2][id] = toInt32_5();
  }

  void set_info(array<string> info)
  {
    name = info[1];
    title = info[2];
    mibun = info[3];
    ptitle = info[4];
  }

  void clear()
  {
    array<uint8> 五维能力_t(武将能力_末, 0);
    array<uint8> 兵种适性_t(兵种_末, 0);
    buildingId = -1;
    face = -1;
    五维能力 = 五维能力_t;
    兵种适性 = 兵种适性_t;
    name = '';
    title = '';
    mibun = '';
    ptitle = '';
  }

  uint32 toInt32_0(void)
  {
    uint32 x = buildingId;
    return x;
  }

  uint32 toInt32_1(void)
  {
    uint32 x = 兵种适性[兵种_枪兵] + (兵种适性[兵种_戟兵] << 8) + (兵种适性[兵种_弩兵] << 16);
    return x;
  }

  uint32 toInt32_2(void)
  {
    uint32 x = 兵种适性[兵种_骑兵] + (兵种适性[兵种_兵器] << 8) + (兵种适性[兵种_水军] << 16);
    return x;
  }

  uint32 toInt32_3(void)
  {
    uint32 x = face;
    return x;
  }

  uint32 toInt32_4(void)
  {
    uint32 x = 五维能力[武将能力_统率] + (五维能力[武将能力_武力] << 8);
    return x;
  }

  uint32 toInt32_5(void)
  {
    uint32 x = 五维能力[武将能力_智力] + (五维能力[武将能力_政治] << 8) + (五维能力[武将能力_魅力] << 16);
    return x;
  }

  void fromInt32_0(uint32 x)
  {
    buildingId = x;
  }

  void fromInt32_1(uint32 x)
  {
    兵种适性[兵种_枪兵] = x << 24 >> 24;
    兵种适性[兵种_戟兵] = x << 16 >> 24;
    兵种适性[兵种_弩兵] = x << 8 >> 24;
  }

  void fromInt32_2(uint32 x)
  {
    兵种适性[兵种_骑兵] = x << 24 >> 24;
    兵种适性[兵种_兵器] = x << 16 >> 24;
    兵种适性[兵种_水军] = x << 8 >> 24;
  }

  void fromInt32_3(uint32 x)
  {
    face = x;
  }

  void fromInt32_4(uint32 x)
  {
    五维能力[武将能力_统率] = x << 24 >> 24;
    五维能力[武将能力_武力] = x << 16 >> 24;
  }

  void fromInt32_5(uint32 x)
  {
    五维能力[武将能力_智力] = x << 24 >> 24;
    五维能力[武将能力_政治] = x << 16 >> 24;
    五维能力[武将能力_魅力] = x << 8 >> 24;
  }
}


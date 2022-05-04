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

附加盗墓事件
  碑墓建筑被破坏时，根据相性，所属等条件
  触发以下任意结果
    1 冤魂索命
    2 传道
    3 夺舍
    4 复活
 */

namespace 碑墓系统 {
  const int KEY = pk::hash("碑墓系统");
  class Main {
    file f;

    pk::func63_t@ prev_callback_63;
    Main()
    {
      pk::bind(102, -1, pk::trigger102_t(剧本初始化_结构体_信息读取));
      pk::bind(105, pk::trigger105_t(存档));
      pk::bind(106, pk::trigger105_t(读档));
      pk::bind(151, pk::trigger151_t(武将死亡));
      pk::bind(120, pk::trigger120_t(建筑信息显示));
      pk::bind(163, pk::trigger163_t(建筑破坏前));

      // @prev_callback_63 = cast<pk::func63_t@>(pk::get_func(63));
      // pk::reset_func(63);
      // pk::set_func(63, pk::func63_t(func63));
    }

    void 历史日志(pk::person@ leader, pk::person@ person, pk::point pos)
    {
      string leader_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(leader)));
      string person_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(person)));

      string str = pk::format("{}听闻{}逝世，感其功绩，为其在\x1b[27x（{}, {}）\x1b[0x修建了墓地", leader_name, person_name, pos.x, pos.y);

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
          info_tomb[i].buildingId = 0;
          info_tomb[i].name = '';
          info_tomb[i].title = '';
          info_tomb[i].mibun = '';
          info_tomb[i].ptitle = '';
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
        pk::building@ building = pk::get_building(dead.service);

        array<pk::point> mult_pos;
        array<pk::point> rings = pk::range(building.get_pos(), 2, 6);
        for (int i = 0; i < int(rings.length); i++)
        {
          pk::point dst_pos = rings[i];
          pk::hex@ hex = pk::get_hex(dst_pos);
          if (hex.has_unit or hex.has_building or hex.terrain >= 地形_未定 or hex.terrain == 地形_荒地) continue;
          if (hex.can_military and hex.can_trap)
            mult_pos.insertLast(dst_pos);
        }
        pk::point res_pos = mult_pos[pk::rand(int(mult_pos.length))];
        // 设施_落石 设施_石壁 设施_土垒
        pk::building@ target_building = pk::create_building(pk::get_facility(设施_落石), res_pos, dead.get_force_id());
        pk::add_hp(target_building, 100);
        int tomb_id;
        for (int i = 0; i < 碑墓建筑_末; i++)
        {
          if(info_tomb[i].buildingId == 0 and info_tomb[i].name.isEmpty())
          {
            tomb_id = i;
            break;
          }
        }
        tomb_info@ tomb_info = @info_tomb[tomb_id];
        tomb_info.buildingId = target_building.get_id();
        tomb_info.name = pk::decode(pk::get_name(dead));
        tomb_info.title = pk::decode(dead.name_read);
        tomb_info.mibun = dead.mibun;
        tomb_info.ptitle = pk::decode(pk::encode("大忠"));
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

    void 建筑信息显示()
    {
      pk::point cursor_pos = pk::get_cursor_hex_pos();
      if (!pk::is_valid_pos(cursor_pos)) return;
      pk::building@ building = pk::get_building(cursor_pos);
      int tomb_id;
      if (building !is null)
      {
        for (int i = 0; i < 碑墓建筑_末; i++)
        {
          if (info_tomb[i].buildingId == building.get_id())
          {
            tomb_id = i;
            break;
          }
        }
        if (!info_tomb[tomb_id].name.isEmpty())
        {
          tomb_info@ tomb_info = @info_tomb[tomb_id];
        // tomb_info.mibun = dead.mibun;
          string title = pk::format('{} \x1b[1x{}\x1b[0x之墓', tomb_info.title, tomb_info.name);
          string ptitle = pk::format('谥号： \x1b[1x{}\x1b[0x', tomb_info.ptitle);

          int width = int(pk::get_resolution().width) - 180;
          pk::point left_down = pk::point(int(pk::get_resolution().width) - 10, 75 + 40 + 5 + 80) + 20;
          pk::draw_filled_rect(pk::rectangle(pk::point(width - 5, 15), left_down), ((0xff / 2) << 24) | 0x010101);
          pk::draw_text(pk::encode(title), pk::point(width, 20), 0xffffffff, FONT_BIG, 0xff000000);
          pk::draw_text(pk::encode(ptitle), pk::point(width, 75), 0xffffffff, FONT_SMALL, 0xff000000);
        }
      }
    }

    // int func63(pk::building@ building, pk::facility@ facility)
    // {
    //   prev_callback_63(building, facility);

    //   int facility_id = facility.get_id();
    //   int max_hp = facility.hp;
    //   if (facility_id >= 设施_阵 and facility_id <= 设施_连弩楼)
    //   {
    //     int spec_id = get_spec_id(building);
    //     if (spec_id != -1)
    //     {
    //       max_hp += SPEC_HP_BONUS;
    //       if (pk::is_valid_person_id(special_ex[spec_id].person))
    //       {
    //         max_hp += SPEC_HP_BONUS;
    //       }

    //     }
    //   }
    //   return max_hp;
    // }
    //
    void 建筑破坏前(pk::building@ building, bool voices)
    {
      for (int i = 0; i < 碑墓建筑_末; i++)
      {
        tomb_info@ tomb_info = @info_tomb[i];
        if (tomb_info.buildingId == building.get_id() and !tomb_info.name.isEmpty())
        {
          tomb_info.clear();
          break;
        }
      }
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
  int 五维序号 = 1;
  int 适性序号 = 2;

  int buildingId;
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
    fromInt32_1(tomb_info_temp[五维序号][id]);
    fromInt32_2(tomb_info_temp[适性序号][id]);
  }

  void update(int id)
  {
    tomb_info_temp[ID序号][id] = toInt32_0();
    tomb_info_temp[五维序号][id] = toInt32_1();
    tomb_info_temp[适性序号][id] = toInt32_2();
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
    buildingId = 0;
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
    uint32 x = 0;
    for (int i = 0; i < 武将能力_末; ++i)
    {
      uint8 temp = 五维能力[i];
      x += temp << (i * 4);
    }
    return x;
  }

  uint32 toInt32_2(void)
  {
    uint32 x = 0;
    for (int i = 0; i < 兵种_末; ++i)
    {
      uint8 temp = 兵种适性[i];
      x += temp << (i * 4);
    }
    return x;
  }


  void fromInt32_0(uint32 x)
  {
    buildingId = x;
  }

  void fromInt32_1(uint32 x)
  {
    for (int i = 0; i < 武将能力_末; ++i)
    {
      五维能力[i] = ((x << (武将能力_魅力 * 4 - i * 4)) >> 武将能力_魅力 * 4);
    }
  }

  void fromInt32_2(uint32 x)
  {
    for (int i = 0; i < 兵种_末; ++i)
    {
      兵种适性[i] = ((x << (兵种_水军 * 4 - i * 4)) >> 兵种_水军 * 4);
    }
  }
}


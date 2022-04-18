
/*
事件数据结构体
v1.0 2022/4/4
*/

namespace 事件数据结构体 {

  const int KEY = pk::hash("自定义事件");

  /// KEY index 的类型是 (uint16),所以最高支持65535,
  const int KEY_索引_追加_武将起始 = 0;          // 索引追加武将结构体 uint32数量为30个，武将共1100个。总计33000
  const int KEY_索引_追加_部队起始 = 33000;         // 索引追加部队结构体 uint32数量为20个，部队共1000个。总计20000
  const int KEY_索引_追加_势力起始 = 53000;         // 索引追加势力结构体 uint32数量为20个，势力共46个。总计920
  const int KEY_索引_追加_设定起始 = 53920;         // 索引追加游戏基础设定结构体 uint32数量为135个

  class Main
  {
    Main() {
      pk::bind(102, -1, pk::trigger102_t(剧本初始化_结构体_信息读取));
      pk::bind(105, pk::trigger105_t(儲存_结构体_信息储存));
    }
    void 儲存_结构体_信息储存(int file_id) //儲存
    {
      for (int i = 0; i < 武将_末; i++)
      {
        person_event[i].update(i);
        for (int j = 0; j <= (事件武将结构体_uint32数 - 1); j++)
          pk::store(KEY, (KEY_索引_追加_武将起始 + (i * 事件武将结构体_uint32数 + j)), event_person_info_temp[j][i]);

      }
      for (int i = 0; i < 部队_末; i++)
      {
        unit_event[i].update(i);
        for (int j = 0; j <= (事件部队结构体_uint32数 - 1); j++)
          pk::store(KEY, (KEY_索引_追加_部队起始 + (i * 事件部队结构体_uint32数 + j)), event_unit_info_temp[j][i]);
      }

      setting_ex.update();
      for (int j = 0; j <= (事件设定结构体_uint32数 - 1); j++)
      {
        pk::store(KEY, (KEY_索引_追加_设定起始 + (j)), event_setting_info_temp[j]);
      }
    }
      void 剧本初始化_结构体_信息读取()
    {
      //重新开始游戏时，初始化数据
      if (!pk::get_scenario().loaded)
      {

        for (int i = 0; i < 武将_末; ++i)
        {
          person_event[i].幽州时长 = 0;
          person_event[i].冀州时长 = 0;
          person_event[i].青徐时长 = 0;
          person_event[i].兖豫时长 = 0;
          person_event[i].司隶时长 = 0;
          person_event[i].京兆时长 = 0;
          person_event[i].凉州时长 = 0;
          person_event[i].扬州时长 = 0;
          person_event[i].荆北时长 = 0;
          person_event[i].荆南时长 = 0;
          person_event[i].益州时长 = 0;
          person_event[i].南中时长 = 0;
        }

      }
      if (pk::get_scenario().loaded)
      {
        for (int i = 0; i < 武将_末; i++)
        {
          for (int j = 0; j <= (事件武将结构体_uint32数 - 1); j++)
            event_person_info_temp[j][i] = uint32(pk::load(KEY, (KEY_索引_追加_武将起始 + (i * 事件武将结构体_uint32数 + j)), 0));

          event_personinfo person_t(i);
          person_event[i] = person_t;
        }
        for (int i = 0; i < 部队_末; i++)
        {
          for (int j = 0; j <= (事件部队结构体_uint32数 - 1); j++)
            event_unit_info_temp[j][i] = uint32(pk::load(KEY, (KEY_索引_追加_部队起始 + (i * 事件部队结构体_uint32数 + j)), 0));

          event_unitinfo unit_t(i);
          unit_event[i] = unit_t;
        }
      }

      settinginfo setting_t();
      setting_event = setting_t;
    }
  }

  Main main;
}

const int 事件武将结构体_uint32数 = 10;
const int 事件部队结构体_uint32数 = 10;
const int 事件设定结构体_uint32数 = 0;

const int 事件最大时间 = 352 * 12 * 3 * 10;

array<array<uint32>> event_unit_info_temp(事件部队结构体_uint32数, array<uint32>(部队_末, uint32(0)));
array<array<uint32>> event_person_info_temp(事件武将结构体_uint32数, array<uint32>(武将_末, uint32(0)));
array<uint32> event_setting_info_temp(设定结构体_uint32数, uint32(0));

array<event_personinfo> person_event(武将_末);
array<event_unitinfo> unit_event(部队_末);
settinginfo setting_event;

class event_unitinfo {
  int 影响序号0 = 0;
  int 影响序号1 = 1;
  int 影响序号2 = 2;
  int 影响序号3 = 3;
  int 影响序号4 = 4;

  bool 影响0;
  bool 影响1;
  bool 影响2;
  bool 影响3;
  int32 影响4_时间 = 事件最大时间;

  //初始化
  event_unitinfo(int id)
  {
    get_info(id);
  }

  event_unitinfo() {}

  void get_info(int id)
  {
    fromInt32_0(event_unit_info_temp[影响序号0][id]);
    fromInt32_1(event_unit_info_temp[影响序号1][id]);
    fromInt32_2(event_unit_info_temp[影响序号2][id]);
    fromInt32_3(event_unit_info_temp[影响序号3][id]);
    fromInt32_4(event_unit_info_temp[影响序号4][id]);
  }

  void update(int id)
  {
    event_unit_info_temp[影响序号0][id] = toInt32_0();
    event_unit_info_temp[影响序号1][id] = toInt32_1();
    event_unit_info_temp[影响序号2][id] = toInt32_2();
    event_unit_info_temp[影响序号3][id] = toInt32_3();
    event_unit_info_temp[影响序号4][id] = toInt32_4();
  }

  uint32 toInt32_0(void)
  {
    uint8 影响序号0_值 = 影响0 ? 1 : 0;
    uint32 x = 影响序号0_值;
    return x;
  }

  uint32 toInt32_1(void)
  {
    uint8 影响序号1_值 = 影响1 ? 1 : 0;
    uint32 x = 影响序号1_值;
    return x;
  }

  uint32 toInt32_2(void)
  {
    uint8 影响序号2_值 = 影响2 ? 1 : 0;
    uint32 x = 影响序号2_值;
    return x;
  }

  uint32 toInt32_3(void)
  {
    uint8 影响序号3_值 = 影响3 ? 1 : 0;
    uint32 x = 影响序号3_值;
    return x;
  }

  uint32 toInt32_4(void)
  {
    uint32 影响4_时间_值 = 影响4_时间;
    uint32 x = 影响4_时间_值;
    return x;
  }

  void fromInt32_0(uint32 x)
  {
    影响0 = (((x << 31) >> 31) == 1);
  }

  void fromInt32_1(uint32 x)
  {
    影响1 = (((x << 31) >> 31) == 1);
  }

  void fromInt32_2(uint32 x)
  {
    影响2 = (((x << 31) >> 31) == 1);
  }

  void fromInt32_3(uint32 x)
  {
    影响3 = (((x << 31) >> 31) == 1);
  }

  void fromInt32_4(uint32 x)
  {
    影响4_时间 = x;
  }
}

class event_personinfo {
  uint16 person_id_t;

  int 幽州冀州时长序号 = 0;
  int 青徐兖豫时长序号 = 1;
  int 司隶京兆时长序号 = 2;
  int 凉州扬州时长序号 = 3;
  int 荆北荆南时长序号 = 4;
  int 益州南中时长序号 = 5;

  uint16 幽州时长 = 0;
  uint16 冀州时长 = 0;

  uint16 青徐时长 = 0;
  uint16 兖豫时长 = 0;

  uint16 司隶时长 = 0;
  uint16 京兆时长 = 0;

  uint16 凉州时长 = 0;
  uint16 扬州时长 = 0;

  uint16 荆北时长 = 0;
  uint16 荆南时长 = 0;

  uint16 益州时长 = 0;
  uint16 南中时长 = 0;


  //初始化
  event_personinfo(int person_id)
  {
    person_id_t = person_id;
    get_info(person_id);
  }

  //下面这种写法用于定义全局变量，先声明，然后用get_info获取信息后使用
  event_personinfo() {}

  void get_info(int person_id)
  {
    fromInt32_0(event_person_info_temp[幽州冀州时长序号][person_id]);
    fromInt32_1(event_person_info_temp[青徐兖豫时长序号][person_id]);
    fromInt32_2(event_person_info_temp[司隶京兆时长序号][person_id]);
    fromInt32_3(event_person_info_temp[凉州扬州时长序号][person_id]);
    fromInt32_4(event_person_info_temp[荆北荆南时长序号][person_id]);
    fromInt32_5(event_person_info_temp[益州南中时长序号][person_id]);
  }

  void update(int person_id)
  {
    event_person_info_temp[幽州冀州时长序号][person_id] = toInt32_0();
    event_person_info_temp[青徐兖豫时长序号][person_id] = toInt32_1();
    event_person_info_temp[司隶京兆时长序号][person_id] = toInt32_2();
    event_person_info_temp[凉州扬州时长序号][person_id] = toInt32_3();
    event_person_info_temp[荆北荆南时长序号][person_id] = toInt32_4();
    event_person_info_temp[益州南中时长序号][person_id] = toInt32_5();
  }

  int get_地区时长(int province_id)
  {
    int result;
    switch(province_id)
    {
      case 0: result = 幽州时长;
        break;
      case 1: result = 冀州时长;
        break;
      case 2: result = 青徐时长;
        break;
      case 3: result = 兖豫时长;
        break;
      case 4: result = 司隶时长;
        break;
      case 5: result = 京兆时长;
        break;
      case 6: result = 凉州时长;
        break;
      case 7: result = 扬州时长;
        break;
      case 8: result = 荆北时长;
        break;
      case 9: result = 荆南时长;
        break;
      case 10: result = 益州时长;
        break;
      case 11: result = 南中时长;
        break;
    }
    return result;
  }

  void set_地区时长(int province_id, int result)
  {
    switch(province_id)
    {
      case 0: 幽州时长 = result;
        break;
      case 1: 冀州时长 = result;
        break;
      case 2: 青徐时长 = result;
        break;
      case 3: 兖豫时长 = result;
        break;
      case 4: 司隶时长 = result;
        break;
      case 5: 京兆时长 = result;
        break;
      case 6: 凉州时长 = result;
        break;
      case 7: 扬州时长 = result;
        break;
      case 8: 荆北时长 = result;
        break;
      case 9: 荆南时长 = result;
        break;
      case 10: 益州时长 = result;
        break;
      case 11: 南中时长 = result;
        break;
    }
  }

  uint32 toInt32_0(void)
  {
    uint16 幽州时长_值 = 幽州时长;
    uint16 冀州时长_值 = 冀州时长;
    uint32 x = 幽州时长_值 + (冀州时长_值 << 16);
    return x;
  }

  uint32 toInt32_1(void)
  {
    uint16 青徐时长_值 = 青徐时长;
    uint16 兖豫时长_值 = 兖豫时长;
    uint32 x = 青徐时长_值 + (兖豫时长_值 << 16);
    return x;
  }

  uint32 toInt32_2(void)
  {
    uint16 司隶时长_值 = 司隶时长;
    uint16 京兆时长_值 = 京兆时长;
    uint32 x = 司隶时长_值 + (京兆时长_值 << 16);
    return x;
  }

  uint32 toInt32_3(void)
  {
    uint16 凉州时长_值 = 凉州时长;
    uint16 扬州时长_值 = 扬州时长;
    uint32 x = 凉州时长_值 + (扬州时长_值 << 16);
    return x;
  }

  uint32 toInt32_4(void)
  {
    uint16 荆北时长_值 = 荆北时长;
    uint16 荆南时长_值 = 荆南时长;
    uint32 x = 荆北时长_值 + (荆南时长_值 << 16);
    return x;
  }

  uint32 toInt32_5(void)
  {
    uint16 益州时长_值 = 益州时长;
    uint16 南中时长_值 = 南中时长;
    uint32 x = 益州时长_值 + (南中时长_值 << 16);
    return x;
  }

  void fromInt32_0(uint32 x)
  {
    幽州时长 = ((x << 16) >> 16);
    冀州时长 = (x >> 16);
  }

  void fromInt32_1(uint32 x)
  {
    青徐时长 = ((x << 16) >> 16);
    兖豫时长 = (x >> 16);
  }

  void fromInt32_2(uint32 x)
  {
    司隶时长 = ((x << 16) >> 16);
    京兆时长 = (x >> 16);
  }

  void fromInt32_3(uint32 x)
  {
    凉州时长 = ((x << 16) >> 16);
    扬州时长 = (x >> 16);
  }

  void fromInt32_4(uint32 x)
  {
    荆北时长 = ((x << 16) >> 16);
    荆南时长 = (x >> 16);
  }

  void fromInt32_5(uint32 x)
  {
    益州时长 = ((x << 16) >> 16);
    南中时长 = (x >> 16);
  }
}

class event_settinginfo
{
  //初始化
  event_settinginfo()
  {
    get_info();
  }

  void get_info()
  {
    //pk::trace(pk::format("get_info,temp:{}", setting_ex_info_temp[0]));
  }

  void update()
  {
  }
}

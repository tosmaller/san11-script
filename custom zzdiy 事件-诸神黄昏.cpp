
/*
诸神黄昏事件
条件：
1 每年每季度10%概率发生一次
2 随机令某一个五维能力高于90的武将，按（当前能力 - 90）* 3 + 25 的几率触发，（最高99%）
结果：
  1 武将死亡
  2 该五维历经3次天劫而武将不死，则该项五维 + 3，且永远不再历劫
*/

namespace 事件_诸神黄昏 {

  class Main
  {
    // 随机数方法
    pk::random random(pk::rand());
    Main() {
      pk::bind(108, pk::trigger108_t(onSeasonBegin));
    }

    void 历史日志(pk::person@ person, string result, bool msg)
    {
      string person_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(person)));

      string str = pk::format("{}{}", person_name, result);

      array<string> result_msg = {
        pk::format("{}应劫而亡", person_name),
        pk::format("天雷降下，{}应声而倒", person_name),
        pk::format("天地不仁，以万物为刍狗。{}吐血三升而死...", person_name),
        pk::format("{}力抗天劫，不克而亡", person_name),
        pk::format("{}忽感厄运将至，盘膝而坐，遂无声息", person_name),

        pk::format("{}忽拔剑四顾，悲从心起，自刎而亡...", person_name),
        pk::format("{}忽然爆体而亡！", person_name),
        pk::format("{}出门大笑三声，大哭三声，以头触底而亡", person_name),
        pk::format("{}摔落下马，折颈而死", person_name),
        pk::format("{}遭刺客伏击，死于乱刃之下...", person_name),

        pk::format("{}行至山路，突遭山洪，卷入乱流之中...", person_name),
        pk::format("{}家中失火，灰飞烟灭，尸骨无存！", person_name),
        pk::format("{}被家奴下毒杀害！", person_name),
        pk::format("{}误入奸细据点，被围杀致死！", person_name),
        pk::format("听说{}被恶鬼索命，死于枯井之中", person_name),

        pk::format("{}误食丹药而亡...", person_name),
        pk::format("{}争风吃醋，死于情敌之手！", person_name),
        pk::format("{}病入膏肓，不治而亡！", person_name),
        pk::format("{}误入遗迹，不知所踪！", person_name),
        pk::format("{}睡梦中死去...", person_name),

        pk::format("{}宿醉坠入河中而亡...", person_name),
        pk::format("{}生无可恋，自绝于家中！", person_name),
        pk::format("{}车架翻入山崖，尸骨无存！", person_name),
        pk::format("{}摔倒街头，被人群踩踏而死！", person_name),
        pk::format("{}被发现悬尸城门！", person_name)
      };

      if (msg) str = result_msg[random(result_msg.length - 1)];

      pk::unit@ unit = pk::get_unit(pk::hex_object_id_to_unit_id(person.location));
      pk::building@ building = pk::get_building(pk::hex_object_id_to_building_id(person.location));
      pk::point persion_pos;
      if (unit !is null) {
        persion_pos = unit.pos;
      }
      else
      {
        persion_pos = building.pos;
      }
      pk::force@ person_force = pk::get_force(person.get_force_id());
      int color = person_force is null ? -1 : person_force.color;

      // pk::history_log(persion_pos, color, pk::encode(str));
      pk::message_box(pk::encode(str));
    }

    void onSeasonBegin()
    {
      pk::message_box(pk::encode('开始新一轮的劫难\n这次是谁要应劫\n让我来占卜一下'), pk::get_person(武将_左慈));
      if (pk::rand_bool(90))
      {
        array<int> ablelist = { 武将能力_统率, 武将能力_武力, 武将能力_智力, 武将能力_政治 };
        int cur_stat = random(ablelist.length - 1);
        string msg;
        switch(cur_stat) {
          case 0:
            msg = pk::encode('青龙在东，仁德不存，应在\x1b[18x统御首将\x1b[0x');
            break;
          case 1:
            msg = pk::encode('白虎在西，杀人盈野，应在\x1b[16x强挚之士\x1b[0x');
            break;
          case 2:
            msg = pk::encode('朱雀在南，妄估天意，应在\x1b[21x帷幄幕僚\x1b[0x');
            break;
          case 3:
            msg = pk::encode('玄武在北，灾祸不息，应在\x1b[24x领属治吏\x1b[0x');
            break;
        }
        pk::message_box(msg, pk::get_person(武将_左慈));
        for (int i = 0; i < 非贼武将_末; i++)
        {
          pk::person@ person_s = pk::get_person(i);
          event_personinfo@ person_t = @person_event[i];
          array<int> mibun_list = { 身分_无, 身分_未登场, 身分_未发现, 身分_死亡, 身分_末 };
          if (person_s is null
            or !pk::is_alive(person_s)
            or mibun_list.find(person_s.mibun) >= 0
            or !pk::is_valid_pos(person_s.get_pos())
            or person_s.stat[cur_stat] < 90
            or person_t.get_天罚时长(cur_stat) > 3
            )
            continue;
          int chance = pk::min((person_s.stat[cur_stat] - 90) * 3 + 25, 99);
          if (pk::rand_bool(chance))
          {
            历史日志(person_s, '', true);
            pk::kill(person_s);
          }
          else
          {
            if (person_t.get_天罚时长(cur_stat) == 2 and int(person_s.base_stat[cur_stat]) < int(pk::core["person.max_stat"]))
            {
              person_s.base_stat[cur_stat] = person_s.base_stat[cur_stat] + 3;
              person_s.update();
              array<string> dif_name = { '\x1b[18x统御首将\x1b[0x', '\x1b[16x强挚之士\x1b[0x', '\x1b[21x帷幄幕僚\x1b[0x', '\x1b[24x领属治吏\x1b[0x' };
              array<string> able_names = { '统率', '武力', '智力', '政治' };
              string person_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(person_s)));
              string chable_name = pk::format("{}", pk::decode(pk::encode(dif_name[cur_stat])));
              string able_name = pk::format("\x1b[24x{}\x1b[0x", pk::decode(pk::encode(able_names[cur_stat])));
              string his_msg = pk::format('{}历经三次{}天劫不死，获天道赐福，{}+3', person_name, chable_name, able_name);

              pk::message_box(pk::encode(his_msg));
            }
            else
            {
              person_t.set_天罚时长(cur_stat, person_t.get_天罚时长(cur_stat) + 1);
            }
          }
        }
        pk::message_box(pk::encode('人力不可逆天\n万物皆为蝼蚁'), pk::get_person(武将_左慈));
      }
      else
      {
        pk::message_box(pk::encode('劫息难平，诸事皆吉，本次\x1b[24x无人应劫\x1b[0x'), pk::get_person(武将_左慈));
      }
    }

  }

  Main main;
}

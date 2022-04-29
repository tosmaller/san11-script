
/*
城市灾害事件

  类型一 4-6月，所有城市有概率触发火灾（5圈火焰）
  类型二 7-9月，有堤坝城市有概率堤坝破坏
  类型三 10-12月，所有城市有概率触发雷电，随机破坏内政设施跟城市
*/

namespace 城市灾害事件 {

  class Main
  {
    // 随机数方法
    pk::random random(pk::rand());
    bool AI_EFFECT = false;
    int 城市治安最低值 = 90;
    pk::list<pk::city@> citys = pk::get_city_list();

    Main() {
      pk::bind2(109, pk::trigger109_t(onSeasonStart), 5);
    }

    void 历史日志(pk::city@ city, string name, string result)
    {
      string city_name = pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(city)));
      string action_name = pk::format("\x1b[27x{}\x1b[0x", name);

      string str = pk::format("{}遭遇{}，{}", city_name, action_name, result);
      pk::force@ city_force = pk::get_force(city.get_force_id());
      int color = city_force is null ? -1 : city_force.color;
      pk::history_log(city.pos, color, pk::encode(str));
    }

    int season()
    {
      int month = pk::get_month();
      if (month >= 1 and month <= 3) return 1;
      if (month >= 4 and month <= 6) return 2;
      if (month >= 7 and month <= 9) return 3;
      if (month >= 10 and month <= 12) return 4;
      return 0;
    }

    void onSeasonStart()
    {
      int season_id = season();
      switch(season_id)
      {
        case 2:
          summer();
          break;
        case 3:
          autumn();
          break;
        case 4:
          winter();
          break;
      }
    }

    void summer()
    {
      for(int i = 0; i < citys.count; i++)
      {
        pk::city@ city = citys[i];
        if (int(city.public_order) < 城市治安最低值)
        {
          int chance = 5 + int((城市治安最低值 - city.public_order) / 5);
          if (pk::rand_bool(chance))
          {
            array<pk::point> range = pk::range(city.get_pos(), 0, 3);
            for (int l = 0; l < int(range.length); l++)
            {
              pk::create_fire(range[l], 2 + pk::rand(2), null, true); //火计
              pk::person@ taishu = pk::get_person(city.get_taishu_id());
              pk::say(pk::encode('着火了，快来人啊'), pk::get_person(city.get_taishu_id()));
              历史日志(city, '夏季干燥失火', '遭受了巨大损失');
            }
          }
        }
      }
    }
    void autumn()
    {
      // 设施_堤防
      pk::list<pk::building@> building_list = pk::get_building_list();
      for(int i = 0; i < building_list.count; i++)
      {
        pk::building@ building = building_list[i];
        if (building.facility == 设施_堤防)
        {
          pk::city@ city = pk::get_city(pk::get_city_id(building.get_pos()));
          pk::trace('城市id:' + city.get_id());
          if (int(city.public_order) < 城市治安最低值)
          {
            int chance = 5 + int((城市治安最低值 - city.public_order) / 5);
            if (pk::rand_bool(chance))
            {
              pk::point pos = building.get_pos();
              pk::kill(building, true);
              pk::create_building(pk::get_facility(设施_堤防), pos, -1);
            }

            pk::say(pk::encode('堤坝破损了，快，快修好'), pk::get_person(city.get_taishu_id()));
            历史日志(city, '秋季江河泛滥', '遭受了巨大损失');
          }
        }
      }
    }
    void winter()
    {
      for(int i = 0; i < citys.count; i++)
      {
        pk::city@ city = citys[i];
        if (int(city.public_order) < 城市治安最低值)
        {
          int chance = 5 + int((城市治安最低值 - city.public_order) / 5);
          if (pk::rand_bool(100))
          {
            pk::create_effect(0x40, city.get_pos());
            int hp = ch::randint(1, int(pk::get_hp(city) * 0.1));
            int troops = ch::randint(1, int(pk::get_troops(city) * 0.1));
            pk::combat_text(-hp, 0, city.get_pos());
            pk::combat_text(-troops, 1, city.get_pos());
            for (int d = 0; d < 30; d++)
            {
              auto t = city.dev[d].building;
              if (pk::is_alive(t) and pk::rand_bool(50))
              {
                pk::create_effect(0x40, city.dev[d].pos);
                pk::kill(t, true);
              }
            }
            pk::say(pk::encode('打雷喽，下雨咯，快收衣服啊'), pk::get_person(city.get_taishu_id()));
            历史日志(city, '冬季雷雨天气', '遭受了巨大损失');
          }
        }
      }
    }
  }

  Main main;
}

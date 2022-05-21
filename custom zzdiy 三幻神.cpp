namespace 神技 {

  class Main
  {
    pk::random random(pk::rand());

    pk::unit@ src_unit;
    pk::point unit_pos;
    pk::force@ unit_force;

    bool 战法_神戟乱舞 = true;

    int 战法消耗 = 30;

    Main() {
      add_menu_unit_order();
    }

    void add_menu_unit_order()
    {
      if (战法_神戟乱舞) {
        pk::menu_item 战法_神戟乱舞;
        战法_神戟乱舞.menu = 113;
        战法_神戟乱舞.init = pk::unit_menu_item_init_t(init);
        战法_神戟乱舞.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称('神戟乱舞', main.战法消耗); });
        战法_神戟乱舞.get_desc = pk::menu_item_get_desc_t(getDesc_战法_神戟乱舞);
        // 战法_神戟乱舞.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神技_名称(main.src_unit, 兵种_戟兵, 4); });
        战法_神戟乱舞.is_enabled = pk::menu_item_is_enabled_t(isEnabled_战法_神戟乱舞);
        // 战法_神戟乱舞.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return 神术数据结构体::getTargets_神术_目标(业火焚天_计略范围, src_pos_); });
        战法_神戟乱舞.handler = pk::unit_menu_item_handler_t(handler_战法_神戟乱舞);
        pk::add_menu_item(战法_神戟乱舞);
      }
    }

    pk::list<pk::unit@> get_near_unit()
    {
      return 神术数据结构体::getTargets_势力_目标部队(unit_force, 3, unit_pos);
    }

    void init(pk::unit@ unit, pk::point src_pos)
    {
      @src_unit = @unit;
      @unit_force = pk::get_force(unit.get_force_id());
      unit_pos = src_pos;
    }

    string getDesc_战法_神戟乱舞()
    {
      if (int(src_unit.energy) < int(战法消耗))
        return pk::encode("气力不足.");
      if (get_near_unit().count < 2)
        return pk::encode("已方部队数量不足2.");
      else
         return pk::encode(pk::format("耗气{},解放已方3格内任意2只部队，击溃3格内所有敌方部队跟建筑", 战法消耗));
    }

    bool isEnabled_战法_神戟乱舞()
    {
      if (int(src_unit.energy) < int(战法消耗)) return false;
      else if (get_near_unit().count < 2) return false;
      return true;
    }

    bool handler_战法_神戟乱舞(pk::point pos)
    {
      pk::list<pk::unit@> src_list = get_near_unit();
      pk::list<pk::unit@> select_unit = pk::unit_selector(pk::encode("选择要解放的部队"), pk::encode("是否解放选择的部队."), src_list, 2, 2);
      if (select_unit.count < 2) return false;

      bool unit_confirm = pk::yes_no(pk::encode("是否确认解放所选2只部队?"));

      if (unit_confirm)
      {
        for (int i = 0; i < select_unit.count; i++)
        {
          pk::unit@ tem_unit = select_unit[i];
          pk::create_effect(0x4c, tem_unit.get_pos());
          pk::wait(100);
          pk::kill(tem_unit, true);
        }
        pk::play_se(25);//使用暴击图音效
        pk::face_cutin(src_unit);
        pk::say(pk::encode("看我神戟乱舞"), pk::get_person(src_unit.leader));

        // pk::array<pk::point> get_dst_pos;
        // for (int i = 0; i < 方向_末; i++)
        // {
        //   get_dst_pos.insertLast(pk::get_neighbor_pos(unit_pos, i, 2));
        // }

        if (pk::is_in_screen(unit_pos))
        {
          pk::damage_info info;
          info.src_pos = unit_pos;
          info.dst_pos = pk::point(unit_pos.x + 1, unit_pos.y + 1);
          info.troops_damage = 1;
          pk::unit_anim(src_unit, 7, { info });
          pk::wait(1);
        }
        pk::list<pk::unit@> target_list = 神术数据结构体::getTargets_神术_目标部队(3, unit_pos);

        for (int i = 0; i < target_list.count; i++)
        {
          pk::unit@ target_unit = target_list[i];
          if (target_unit.get_force_id() != unit_force.get_force_id())
          {
            pk::create_effect(0x65, target_unit.get_pos());
            pk::create_effect(0x23, target_unit.get_pos());
            pk::wait(50);
            pk::kill(target_unit);
          }
        }
        pk::stop_unit_anim();
      }


      // pk::add_energy(src_unit, -战法消耗, true);
      // pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      神术数据结构体::部队行动(src_unit);
      return true;
    }
  }

  Main main;


}

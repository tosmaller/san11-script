// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/10/24 #江东新风#改写171trigger##
// ## 2020/08/04 # 氕氘氚 # 修复临时武将被捕閃退的bug、临时武将可以入城的bug ##
// @@ 2020/08/03 @ 氕氘氚 @@

/*
部队回城时进行檢验：
1. 农民兵：解散部队、兵装，武将初始化。
2. 雇佣兵：武将初始化。

部队溃灭时进行檢验：
    初始化临时武将。


临时武将范围：780~799
    农民/雇佣：780~799
    贼、奇袭、突袭：1000~1099
*/


namespace Unit_Clear_Event
{
    // --------------------------------------------------------------------------------------

    const int 临时武将起始 = 780;
    const int 临时武将终止 = 799;


    // --------------------------------------------------------------------------------------


    class Main
    {
        Main()
        {

            pk::bind(171, pk::trigger171_t(event_return));


        }


        void event_return(pk::unit@ unit, pk::hex_object@ dst, int type)
        {
            int leader_id = unit.leader;
            pk::person@ leader = pk::get_person(leader_id);

            //ch::u8debug("return");
            //ch::u8debug(pk::format("type: {}", type));
            //ch::u8debug(pk::format("person_id: {}", leader_id));
            //ch::u8debug(pk::format("person_location: {}", leader.location));

            if (type == 1)  //进入据点
            {
                if (leader_id >= 临时武将起始 && leader_id <= 临时武将终止)
                {
                    //ch::u8debug(pk::format("unit_pos: ({},{})", unit.get_pos().x, unit.get_pos().y));
                    //ch::u8debug(pk::format("target: {}  type:{}", unit.target, unit.target_type));
                    //ch::u8debug(pk::format("target_pos: ({},{})", unit.target_pos.x, unit.target_pos.y));
                    // pk::building@ building = pk::get_building(unit.target_pos);
                    //ch::u8debug(pk::format("building_id:{}", building.get_id()));
                    //ch::u8debug(pk::format("building_troops: {}", building.troops));


                    if (leader.name_read == pk::encode("农民兵"))   //农民兵
                    {
                        //ch::u8debug("nong min bing");
                        // //  兵力、金、粮复位
                        // int troops = unit.troops;
                        // int weapon = unit.weapon;
                        // ch::add_troops(building, -troops);
                        // pk::add_weapon_amount(building, weapon, -troops);
                        // pk::add_gold(building, -unit.gold);
                        // pk::add_food(building, -unit.food);

                        // //  气力复原
                        // int previous_energy = ((building.troops + troops) * pk::get_energy(building) - troops * unit.energy) / building.troops;
                        // pk::set_energy(building, previous_energy);

                        //ch::u8debug(pk::format("troops: {}", unit.troops));

                        pk::reset(leader);

                    }

                    else if (leader.name_read == pk::encode("雇佣兵"))   //雇佣兵
                    {
                        //ch::u8debug("gu yong bing");
                        pk::reset(leader);
                    }

                    else if (leader.name_read == pk::encode("突袭者"))   //突袭者
                    {
                        //ch::u8debug("di jun tu xi");
                        pk::reset(leader);
                    }
                }
                else if (leader_id >= 1000 && leader_id <= 1099)    //贼武将
                {
                    //ch::u8debug("zei wu jiang");
                    pk::reset(leader);
                }

            }
            else        //部队溃灭
            {
                if (leader_id >= 临时武将起始 && leader_id <= 临时武将终止)
                {
                    pk::reset(leader);
                }
                else if (leader_id >= 1000 && leader_id <= 1099)    //贼武将（防止贼兵異族被捕）
                {
                    pk::reset(leader);
                }
            }

        }



    }

    Main main;
}


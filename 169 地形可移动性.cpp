// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2020/08/05 #江东新风#增加农民兵不能移入城池港关的设定###
// ## 2020/07/26 ##
/*
@ 수정한이: 기마책사
@ UPDATE: '18.11.5
*/

namespace TERRAIN_ENABLED
{
    // 기교필요여부설정 Customize ('18.11.5)
    const bool 정예기교필요 = true;  // 정예기교필요 = true, 불필요 = false 


    class Main
    {
        Main()
        {
            pk::set_func(169, pk::func169_t(callback));
        }

        bool callback(pk::unit@ unit, int terrain_id)
        {
            //pk::trace(pk::format("unit moveable, terrain_id:{},core:{}",terrain_id, bool(pk::core["terrain"][terrain_id]["movabale"])));
            if (bool(pk::core["terrain"][terrain_id]["movabale"]) == false)
                return false;

            int weapon_id = pk::is_water_terrain(terrain_id) ? pk::get_sea_weapon_id(unit) : pk::get_ground_weapon_id(unit);
            pk::equipment@ equipment = pk::get_equipment(weapon_id);
            //pk::trace(pk::format("unit moveable, equipment !is null:{},equipment.movement_cost[terrain_id]:{}", (equipment !is null), equipment.movement_cost[terrain_id]));
            if (equipment !is null and equipment.movement_cost[terrain_id] == 255)
                return false;
            //pk::trace(pk::format("unit moveable, terrain_id:{},weapon_id:{},required 难所行军：{}",terrain_id,weapon_id, bool(pk::core["terrain"][terrain_id]["required_nanshokougun"])));
            if (bool(pk::core["terrain"][terrain_id]["required_nanshokougun"]) and !unit.has_tech(技巧_难所行军))
                return false;
            if ((is_农民兵(unit)) and (terrain_id == 16 or terrain_id == 17 or terrain_id == 18)) return false;



            // 산악 지형 이동에 대해서 부대는 난소행군 필요
            // 전투병과의 산악 지형 이동에 대해서 정예기교 필요여부 설정 가능 ('18.11.5)
            // 기병부대는 병종적성 S이상 + 정예기병 연구 시 산악 이동 가능
            // 수송부대는 난소행군+목우유마 연구 시 산악 이동 가능
            // 난소행군 연구 시 특기_답파 또는 무장_등애는 부대유형 관계없이 산악 이동 가능
            if (bool(pk::core["terrain"][terrain_id]["required_nanshokougun"]) and unit.has_tech(技巧_难所行军) and equipment !is null)
            {
                if ((terrain_id == 地形_崖) and (equipment.movement_cost[地形_崖] != 255) and (!pk::is_in_water(unit)))
                {
                    if ((pk::get_ground_weapon_id(unit) == 兵器_剑) and (unit.type == 部队类型_战斗)) // <검>은 난소행군 연구 시 기본적으로 이동 가능
                        return true;

                    else if ((pk::get_ground_weapon_id(unit) == 兵器_枪)) // <창>은 난소행군 연구 시 기본적으로 이동 가능 
                        return true;

                    else if ((pk::get_ground_weapon_id(unit) == 兵器_戟)) // <극>은 난소행군 연구 시 기본적으로 이동 가능 
                        return true;

                    else if ((pk::get_ground_weapon_id(unit) == 兵器_弩)) // <노>는 난소행군 연구 시 기본적으로 이동 가능 
                        return true;

                    else if ((pk::get_ground_weapon_id(unit) == 兵器_战马) and pk::get_tekisei(unit) >= 적성_S)
                        if (!정예기교필요)
                            return true;
                        else if (정예기교필요 and unit.has_tech(기교_정예기병))
                            return true;
                        else
                            return false;

                    else if ((unit.type == 部队类型_运输) and pk::has_tech(pk::get_force(unit.get_force_id()), 기교_목우유마))
                        return true;

                    else if (ch::has_skill(unit, 특기_답파))
                        return true;

                    else if (unit.leader == 무장_등애 or unit.member[1] == 무장_등애 or unit.member[2] == 무장_등애)
                        return true;

                    else
                        return false;
                }
            }


            return true;
        }

        bool is_农民兵(pk::unit@ unit)
        {
            //pk::array<pk::person@> members(3, null);
            for (int j = 0; j < 3; j++)
            {
                pk::person@ member_t = pk::get_person(unit.member[j]);
                if (pk::is_alive(member_t))
                {
                    //@members[j] = @member_t;
                    if (member_t.name_read == pk::encode("农民兵"))
                        return true;
                }
            }
            return false;
        }



    }

    Main main;
}
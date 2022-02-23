// ## 2021/11/08 # 江东新风 # 更改trigger来确保金变化正确显示 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/28 # 江东新风 # 将小城治安显示 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/10/10 # 江东新风 #加入信息行数设定，以便多个cpp调用##
// ## 2021/02/05 # 江东新风 #调整兵装维护费基于的数量单位，从1000变为10000##
// ## 2021/01/23 # 江东新风 #调整特级难度的维护费到合理范围，未设置kre##
// ## 2020/12/01 # 江东新风 #再次修复报告出现乱码问题##
// ## 2020/11/21 # 江东新风 #修复报告再次出现乱码问题##
// ## 2020/10/27 # 氕氘氚 # 修复乱码问题 ##
// ## 2020/08/19 # 氕氘氚 # 添加军餉不足时溃逃的事件 ##
// ## 2020/08/19 # 江东新风 #尝試修复非玩家城市出现物資損坏报告的問題##
// ## 2020/08/10 # 氕氘氚 ##


namespace 军餉维护费
{
    // -------------------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------
    // 유저 설정 (true = on, false = off)
    const bool 兵器维护费设置 = true;      // 병기 보유량에 비례하여 금 소비, 금 없을 시 병기 차감
    const bool 士兵逃走设置 = true;
    const bool 兵器减少设置 = true;       // 병기차감설정
    const int 兵器减少时期 = 1;        // 0: 매턴,  1: 매월 1일
    //特级难度兵装2，兵器3，军队城内10.城外13是比较合理的范围
    const int 兵装基础维护费 = 10;  //10000件兵装一月的基础维护费，军马维护费2倍
    const int 兵器基础维护费 = 1;  //冲車井栏一月的维护费

    const int 金收入倍率 = 100;
    const bool 军队维护费设置 = true;
    const int 军队基础维护费_城内 = 40;  //10000人一月的基础维护费
    const int 军队基础维护费_出征 = 60;

    //---------------------------------------------------------------------------------------
    int 信息行数 = 0;


    // -------------------------------------------------------------------------------------------------------

    class Main
    {
        Main()
        {
            // 当鼠标放在据点上时会在上方显示金錢消耗的相关信息
            pk::bind(120, 1,pk::trigger120_t(信息显示_据点信息));

            pk::bind(107, pk::trigger107_t(func_每旬兵器損坏));
            pk::bind(107, pk::trigger107_t(func_每月减除军费));

            // pk::bind(105, pk::trigger105_t());
        }




        //显示信息
        void 信息显示_据点信息()
        {
            // 光标指的坐标
            pk::point cursor_pos = pk::get_cursor_hex_pos();
            if (!pk::is_valid_pos(cursor_pos)) return;

            // 光标上指示的建筑物
            pk::building@ building = pk::get_building(cursor_pos);
            if (building is null || building.facility > 2) return;  //城港官才显示


            // 确认城市是否是玩家的，及是否城市控制势力id是当前回合的执行势力id
            if (!building.is_player()) return;
            if (!pk::is_player_controlled(building)) return;
            if (building.get_force_id() != pk::get_current_turn_force_id()) return;
            int 信息行数 = 0;
            // 显示信息
            func_信息显示_据点信息(building);
        }


        void func_信息显示_据点信息(pk::building@ building)
        {

            string building_name = pk::decode(pk::get_name(building));

            string title = pk::format("据点信息(\x1b[1x{}\x1b[0x)", building_name);

            int gold_estimate = pk::get_gold(building);
            int 军餉 = get_军餉(building), 维护费 = get_维护费(building);


            string info_军餉 = pk::format("军饷: \x1b[1x{}\x1b[0x", 军餉);
            string info_维护费 = pk::format("兵装维护费: \x1b[1x{}\x1b[0x", 维护费);
            if (gold_estimate < 军餉)
            {
                info_军餉 = pk::format("军饷: \x1b[29x{}\x1b[0x", 军餉);
            }
            if (gold_estimate < 军餉 + 维护费)
            {
                info_维护费 = pk::format("兵装维护费: \x1b[29x{}\x1b[0x", 维护费);
            }

            int middle = int(pk::get_resolution().width) / 2;
            int left = middle - 200;
            int right = middle + 200;
            int top = 5;
            int bottom = top + 80;

            // pk::draw_rect(pk::rectangle(left, top, right, bottom), 0xff00ccff);

            pk::draw_text(pk::encode(title), pk::point(left + 5, top + 5), 0xffffffff, FONT_BIG, 0xff000000);

            int base_id = building.get_id();
            if (base_id >= 据点_城市末 and base_id < 据点_末)
            {
                //BaseInfo@ base_t = @base_ex[base_id];
                string info_治安 = pk::format("治安: \x1b[1x{}\x1b[0x", base_ex[base_id].public_order/*base_t.public_order*/);
                pk::draw_text(pk::encode(info_治安), pk::point(middle + 50, top + 13), 0xffffffff, FONT_SMALL, 0xff000000);
                //信息行数 += 1;
            }

            pk::draw_text(pk::encode(info_军餉), pk::point(left + 10, top + 40 + 信息行数 * 20), 0xffffffff, FONT_SMALL, 0xff000000);
            pk::draw_text(pk::encode(info_维护费), pk::point(middle + 10, top + 40 + 信息行数 * 20), 0xffffffff, FONT_SMALL, 0xff000000);
            信息行数 += 1;
        }


        void func_每月减除军费()
        {
            if (pk::get_day() == 1)
            {
                int n, force_id;
                bool effect = false;
                float maintenance_type = 1.0f;

                for (int i = 0; i < 据点_末; i++)
                {
                    pk::building@ building = pk::get_building(i);
                    force_id = building.get_force_id();
                    if (force_id < 0) continue;

                    n = get_军餉(building) + get_维护费(building) + 1;
                    effect = (pk::is_in_screen(building.pos)) ? true : false;
                    pk::add_gold(building, -n, effect);

                    if (士兵逃走设置)
                    {
                        func_士兵溃逃(building, get_军餉(building));
                    }
                    if (兵器减少设置 && 兵器减少时期 == 1)
                    {
                        func_兵器損坏(building, n, maintenance_type);
                    }


                }
            }
 
        }


        void func_每旬兵器損坏()
        {
            if (兵器减少设置 && 兵器减少时期 == 0)
            {
                float maintenance_type = 0.3f;

                for (int i = 건물_도시시작; i < 据点_末; i++)
                {
                    pk::building@ building = pk::get_building(i);
                    if (building.get_force_id() < 0) continue;
                    func_兵器損坏(building, (get_军餉(building) + get_维护费(building)), maintenance_type);
                }
            }
        }


        void func_兵器損坏(pk::building@ building, int weapon_pay, float type)
        {

            int gold_estimate = pk::get_gold(building);
            if (gold_estimate < weapon_pay)
            {
                int weapon_qty1 = pk::get_weapon_amount(building, 병기_창);
                int weapon_qty2 = pk::get_weapon_amount(building, 병기_극);
                int weapon_qty3 = pk::get_weapon_amount(building, 병기_노);
                int weapon_qty4 = pk::get_weapon_amount(building, 병기_군마);
                int weapon_sum = weapon_qty1 + weapon_qty2 + weapon_qty3 + weapon_qty4;
                if (weapon_sum == 0) return;

                pk::add_weapon_amount(building, 병기_창, int((gold_estimate - weapon_pay) * 1.0f * weapon_qty1 / weapon_sum * type), true);
                pk::add_weapon_amount(building, 병기_극, int((gold_estimate - weapon_pay) * 1.0f * weapon_qty2 / weapon_sum * type), true);
                pk::add_weapon_amount(building, 병기_노, int((gold_estimate - weapon_pay) * 1.0f * weapon_qty3 / weapon_sum * type), true);
                pk::add_weapon_amount(building, 병기_군마, int((gold_estimate - weapon_pay) * 1.0f * weapon_qty4 / weapon_sum * type), true);

                if (building.is_player())
                {
                    pk::person@ person_병사 = pk::get_person(무장_병사);
                    string building_name = pk::decode(pk::get_name(building));
                    pk::message_box(pk::encode(pk::format("报告主公,\x1b[2x{}\x1b[0x的军备军械维护费不足,部分兵装出现了损坏的情况.", building_name)), person_병사);
                }
            }
        }


        void func_士兵溃逃(pk::building@ building, int troops_pay)
        {

            int gold_estimate = pk::get_gold(building);
            int building_troops = pk::get_troops(building);
            if (building_troops == 0) return;
            if (gold_estimate < troops_pay)
            {
                int deserter = pk::max(1, building_troops * ch::randint(10, 30) / 100);
                ch::add_troops(building, -deserter, true, 102);
                pk::add_energy(building, -40, true);

                if (building.is_player())
                {
                    pk::person@ person_병사 = pk::get_person(무장_병사);
                    string building_name = pk::decode(pk::get_name(building));
                    pk::message_box(pk::encode(pk::format("报告主公,因\x1b[2x{}\x1b[0x无法支付军饷, 部分士兵乘夜出逃了.", building_name)), person_병사);
                }
            }
        }



        int get_军餉(pk::building@ building)
        {
            if (军队维护费设置)
            {
                int sum_unit_troops = 0, n = 0;
                pk::force@ force = pk::get_force(building.get_force_id()); // 도시의 세력
                array<pk::unit@> unit_arr = pk::list_to_array(pk::get_unit_list(force)); // 도시 세력의 부대 목록
                for (int i = 0; i < int(unit_arr.length); i++)
                {
                    pk::unit@ unit = unit_arr[i];
                    if (building.get_id() == pk::get_service(unit)) // 도시 세력 부대의 소속이 해당 도시라면
                        sum_unit_troops += int(unit.troops); // 해당 부대의 병사수 더함
                }

                n = int(building.troops * 军队基础维护费_城内 / 10000.f + sum_unit_troops * 军队基础维护费_出征 / 10000.f);

                // 避免电脑資金不足
                if (AI_防負收入(building)) return 0;

                return n;
            }
            return 0;
        }


        int get_维护费(pk::building@ building)
        {
            if (兵器维护费设置)
            {
                int n = 0;
                // 富豪降低维护费
                float rich_rate = pk::is_valid_person_id(building.who_has_skill(特技_富豪)) ? 1.5f : 1.0f;  // 일반=1.0  특기부호=1.5 반환

                // 일반병기 유지비 차감, 병기 부족 시 수입 대신 병기 보유량 감소
                int weapon_qty1 = pk::get_weapon_amount(building, 병기_창);
                int weapon_qty2 = pk::get_weapon_amount(building, 병기_극);
                int weapon_qty3 = pk::get_weapon_amount(building, 병기_노);
                int weapon_qty4 = pk::get_weapon_amount(building, 병기_군마);
                int weapon_sum = pk::max(1, weapon_qty1 + weapon_qty2 + weapon_qty3 + weapon_qty4 * 2);
                n += int(兵装基础维护费 / 10000.f * weapon_sum / rich_rate);

                // 공성병기 및 전함은 유지비 차감만 적용 
                n += int(兵器基础维护费 * (pk::get_weapon_amount(building, 병기_충차)) / rich_rate);
                n += int(兵器基础维护费 * (pk::get_weapon_amount(building, 병기_정란)) / rich_rate);
                n += int(兵器基础维护费 * 1.5f * (pk::get_weapon_amount(building, 병기_목수)) / rich_rate);
                n += int(兵器基础维护费 * 2.0f * (pk::get_weapon_amount(building, 병기_투석)) / rich_rate);
                n += int(兵器基础维护费 * 4.0f * (pk::get_weapon_amount(building, 兵器_楼船)) / rich_rate);
                n += int(兵器基础维护费 * 6.0f * (pk::get_weapon_amount(building, 兵器_斗舰)) / rich_rate);

                // 避免电脑資金不足
                // int income;
                // if (building.facility == 시설_도시)
                // {
                //     pk::city@ city = pk::building_to_city(building);
                //     income = cast<pk::func150_t>(pk::get_func(150))(@city);
                // }    
                // else if (building.facility == 시설_도시)
                // {
                //     pk::gate@ gate = pk::building_to_gate(building);
                //     income = cast<pk::func150_t>(pk::get_func(152))(@building, );
                // }
                // else if (building.facility == 시설_도시)
                // {
                //     pk::port@ port = pk::building_to_port(building);
                //     income = cast<pk::func150_t>(pk::get_func(150))(@building);
                // }

                // 避免电脑資金不足
                if (AI_防負收入(building)) return 0;

                return n;

            }
            return 0;

        }


        bool AI_防負收入(pk::building@ building)
        {
            if (!building.is_player())
            {
                if (pk::get_gold(building) <= 5000)
                {
                    return true;
                }
            }
            return false;
        }


    }

    Main main;

}

// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/30 #江东新风#同步马术书大神的更新##
// ## 2020/08/03 #messi#改进表述##
// ## 2020/08/02 #江东新风#增加回合末及Save&Load时的重疊物檢測以防閃退##
// ## 2020/07/26 ##
/*
※ 주의사항 : 본 스크립트 적용 시 지형.xml 에서 적용된 일부 산악/물가 지형 설정이 무시됩니다.

@만든이: 기마책사
@Update: '18.12.22   / 최초적용: 거점 주변의 [지형_산]과 [지형_물가]를 [지형_미정]으로 일괄 변경함
@Update: '18.12.23   / 변경내용: [지형_미정] 에 있는 부대 강제해산 추가
@Update: '19.1.2     / 변경내용: 지방별 고도제한 조건 추가, 벼랑길주변 낭떠러지 [지형_미정]으로 일괄 변경
@Update: '19.2.7     / 변경내용: [지형_미정] 에 있는 건물 강제철거 추가
@Update: '19.2.15    / 변경내용: 고도제한 판정 기준좌표 수정 (칸 모서리에서 가운데로)
@Update: '20.8.29    / 변경내용: 캠페인에서는 커스텀설정 비활성화
*/

namespace 지형이동제한
{
    //---------------------------------------------------------------------------------------
    // 유저 설정 (true = on, false = off)

    const bool 거점주변지형_변경설정 = true;     // 거점 주변의 [지형_산]과 [지형_물가]를 [지형_미정]으로 변경함

    // 거점주변지형_변경설정=true 일 때 :
    const bool 도시주변지형_변경설정 = true;     // 거점 주변의 [지형_산]과 [지형_물가]를 [지형_미정]으로 변경함
    const bool 관문주변지형_변경설정 = true;     // 거점 주변의 [지형_산]과 [지형_물가]를 [지형_미정]으로 변경함
    const bool 항구주변지형_변경설정 = true;     // 거점 주변의 [지형_산]과 [지형_물가]를 [지형_미정]으로 변경함

    const int  도시주변지형_변경범위 = 7;
    const int  관문주변지형_변경범위 = 5;
    const int  항구주변지형_변경범위 = 5;

    //---------------------------------------------------------------------------------------
    const bool 제한지형_부대해산설정 = true;    // 주의: true 로 설정 권장 (변경된 지형의 부대가 이동 시 튕김)
    //---------------------------------------------------------------------------------------

    const bool 고지대지형_변경설정 = true;     // 지형 높이가 기준 이상인 [지형_산]을 [지형_미정]으로 변경함
    const int  고지대지형_기준높이_서촉남만 = 200;      // 주의사항 : 180 이하 설정 불가
    const int  고지대지형_기준높이_기타지역 = 130;      // 주의사항 : 100 이하 설정 불가

    //---------------------------------------------------------------------------------------

    const bool 벼랑길주변_변경설정 = true;     // 벼랑길 주변 벼랑길보다 고도 낮은 낭떠러지 [지형_산]을 [지형_미정]으로 변경   

    //---------------------------------------------------------------------------------------

    class Main
    {

        Main()
        {
            pk::bind(102, pk::trigger102_t(onGameInit));
            //pk::bind(105, pk::trigger105_t(remove_invalid_object1));
            //pk::bind(106, pk::trigger106_t(remove_invalid_object1));
            pk::bind(112, pk::trigger112_t(remove_invalid_object3));
            pk::bind(112, pk::trigger112_t(onTurnEnd));
            //pk::bind(111, pk::trigger111_t(remove_invalid_object3));

        }

        void onGameInit()
        {
            if (!pk::is_campaign())
            {
                if (거점주변지형_변경설정)
                {
                    if (도시주변지형_변경설정)
                    {
                        for (int num = 건물_도시시작; num < 城市_末; num++)
                        {
                            disable_terrain_around_building(num, 도시주변지형_변경범위);
                        }
                    }

                    if (관문주변지형_변경설정)
                    {
                        for (int num = 建筑_关卡开始; num < 건물_관문끝; num++)
                        {
                            disable_terrain_around_building(num, 관문주변지형_변경범위);
                        }
                    }

                    if (항구주변지형_변경설정)
                    {
                        for (int num = 건물_항구시작; num < 建筑_港口末; num++)
                        {
                            disable_terrain_around_building(num, 항구주변지형_변경범위);
                        }
                    }
                }

                if (고지대지형_변경설정)
                {
                    disable_terrain_high_pos();
                }

                if (벼랑길주변_변경설정)
                {
                    disable_terrain_nearby_cliff();
                }
            }
            remove_invalid_object();

        }


        void onTurnEnd(pk::force@ force)
        {
            if (pk::is_campaign()) return;
            remove_invalid_object();
        }

        //---------------------------------------------------------------------------


        void disable_terrain_around_building(int building_id, int around_range)
        {
            pk::building@ building = pk::get_building(building_id);
            pk::array<pk::point> range = pk::range(building.pos, 1, around_range);
            for (int j = 0; j < int(range.length); j++)
            {
                pk::hex@ hex = pk::get_hex(range[j]);
                if (hex.terrain == 지형_산 or hex.terrain == 지형_물가)
                {
                    hex.terrain = 지형_미정;
                }
            }

        }

        void disable_terrain_high_pos()
        {
            for (int pos_x = 0; pos_x < 200; pos_x++)
            {
                for (int pos_y = 0; pos_y < 200; pos_y++)
                {
                    pk::point pos;
                    pos.x = pos_x;
                    pos.y = pos_y;

                    pk::hex@ hex = pk::get_hex(pos);

                    // 지방별 기준높이
                    int region_id = pk::get_region_id(pk::get_city(pk::get_city_id(pos)));
                    int ref_height = 0;
                    if (region_id >= 지방_파촉)
                        ref_height = pk::max(180, 고지대지형_기준높이_서촉남만);
                    else
                        ref_height = pk::max(100, 고지대지형_기준높이_기타지역);

                    // 해당 좌표 높이 제한
                    int pos_height = pk::get_height_map(pk::hex_pos_to_height_map_pos(pos) + 2).height;
                    if (hex.terrain == 지형_산 and pos_height >= ref_height)
                    {
                        hex.terrain = 지형_미정;
                    }
                }
            }
        }

        void disable_terrain_nearby_cliff()
        {
            for (int pos_x = 0; pos_x < 200; pos_x++)
            {
                for (int pos_y = 0; pos_y < 200; pos_y++)
                {
                    pk::point pos;
                    pos.x = pos_x;
                    pos.y = pos_y;

                    pk::hex@ hex = pk::get_hex(pos);
                    if (hex.terrain == 지형_벼랑길)
                    {
                        int pos_height = pk::get_height_map(pk::hex_pos_to_height_map_pos(pos) + 2).height;
                        for (int dir = 0; dir < 방향_끝; dir++)
                        {
                            pk::point neighbor_pos = pk::get_neighbor_pos(pos, dir);
                            if (pk::is_valid_pos(neighbor_pos))
                            {
                                pk::hex@ neighbor_hex = pk::get_hex(neighbor_pos);
                                int neighbor_height = pk::get_height_map(pk::hex_pos_to_height_map_pos(neighbor_pos) + 2).height;
                                if (neighbor_hex.terrain == 지형_산 and (pos_height >= neighbor_height))
                                {
                                    neighbor_hex.terrain = 지형_미정;
                                }
                            }
                        }
                    }
                }
            }
        }


        void remove_invalid_object()
        {
            for (int pos_x = 0; pos_x < 200; pos_x++)
            {
                for (int pos_y = 0; pos_y < 200; pos_y++)
                {
                    pk::point pos;
                    pos.x = pos_x;
                    pos.y = pos_y;

                    pk::hex@ hex = pk::get_hex(pos);
                    if (hex.terrain == 지형_미정 and hex.has_unit)
                        pk::kill(pk::get_unit(pos));

                    if (hex.terrain == 지형_미정 and hex.has_building)
                        pk::kill(pk::get_building(pos));
                    //增加当建筑物和单位重疊的处理by江东新风

                    //if (hex.has_unit and hex.has_building)
                        // pk::kill(pk::get_unit(pos));
                }
            }

        }
        //失?的方法
        /*
        void remove_invalid_object2(int file_id)
        {
            pk::history_log(pk::get_current_turn_force_id(), pk::get_force(pk::get_current_turn_force_id()).color, pk::encode(pk::format("\x1b[1x{}\x1b[0x势力开始检查位置重叠." , pk::decode(pk::get_name(pk::get_force(pk::get_current_turn_force_id()))) ) ) );

            array<pk::building@> arr = pk::list_to_array(pk::get_building_list());
            for (int i = 0; i < int(arr.length); i++)
            {
                pk::building@ building0 = arr[i];
                pk::point pos = building0.pos;

                    pk::hex@ hex = pk::get_hex(pos);
                    if (hex.has_unit)
                    {
                        pk::unit @dst = pk::get_unit(pos);
                        pk::kill(dst);
                        pk::history_log(pk::get_current_turn_force_id(), pk::get_force(pk::get_current_turn_force_id()).color, pk::encode(pk::format("因为位置重叠, 将\x1b[1x{}\x1b[0x移除." , pk::decode(pk::get_name(dst)) ) ) );
                    }
                    else
                    {
                        //pk::history_log(pk::get_current_turn_force_id(), pk::get_force(pk::get_current_turn_force_id()).color, pk::encode("未检测到重叠单位." ) );
                    }

            }

        }

        void remove_invalid_object1(int file_id)
        {
            //pk::history_log(pk::get_current_turn_force_id(), pk::get_force(pk::get_current_turn_force_id()).color, pk::encode(pk::format("\x1b[1x{}\x1b[0x势力开始检查位置重叠." , pk::decode(pk::get_name(pk::get_force(pk::get_current_turn_force_id()))) ) ) );

            array<pk::unit@> arr = pk::list_to_array(pk::get_unit_list());
            for (int i = 0; i < int(arr.length); i++)
            {
                pk::unit@ dst = arr[i];
                pk::point pos = dst.pos;
                pk::hex@ hex = pk::get_hex(pos);
                    if (hex.has_building)
                    {
                        //pk::unit @dst = pk::get_unit(pos);
                        pk::history_log(pk::get_current_turn_force_id(), pk::get_force(pk::get_current_turn_force_id()).color, pk::encode(pk::format("因为位置重叠, 将\x1b[1x{}\x1b[0x移除." , pk::decode(pk::get_name(dst)) ) ) );
                        pk::kill(dst);
                    }
                    else
                    {
                        //pk::history_log(pk::get_current_turn_force_id(), pk::get_force(pk::get_current_turn_force_id()).color, pk::encode("未检测到重叠单位." ) );
                    }

            }

        }
        */
        void remove_invalid_object3(pk::force@ force)
        {
            array<pk::unit@> arr = pk::list_to_array(pk::get_unit_list());
            for (int i = 0; i < int(arr.length); i++)
            {
                pk::unit@ dst = arr[i];
                pk::point pos = dst.pos;
                pk::hex@ hex = pk::get_hex(pos);
                if (hex.has_building)
                {
                    pk::history_log(dst.get_pos(), force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x内部士兵与主将出现了分歧,士兵们解甲归田了.", pk::decode(pk::get_name(dst)))));
                    pk::kill(dst);
                }

            }

        }
        /*
                void remove_invalid_object4(pk::force@ force)
                {
                        pk::point pos;
                        pos.x = 130;
                        pos.y = 48;
                        pk::hex@ hex = pk::get_hex(pos);
                        if (hex.has_building)
                        {
                            //ch::u8debug("hex.has_building works");
                        }
                        if (hex.has_unit)
                        {
                            //ch::u8debug("hex.has_unit works");
                        }

                        pk::unit @dst = pk::get_unit(pos);

                        if (dst !is null)
                        {
                            //ch::u8debug("get_unit works");
                        }
                        else if  (dst is null)
                        {
                            //ch::u8debug("get_unit didn't work");
                        }

                        pk::building@building1 = pk::get_building(pos);
                        pk::kill(building1);



                }
        */




    }

    Main main;
}


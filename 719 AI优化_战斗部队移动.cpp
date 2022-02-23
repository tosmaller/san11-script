// ## 2021/10/10 # 江东新风 # warning修复 ##
// ## 2021/10/04 # 江东新风 # 部队任务_收复改为修复 ##
// ## 2020/11/17 # 江东新风 # trace 信息乱码修复 ##
// ## 2020/10/31 # 江东新风 # 同步马术书大神的更新，本地化##
/*
// 작성자: 기마책사
// Update: '20.10.24    / 최초내용 : 노병/간접병기부대 근접교전 회피기동 추가, 공성전 공성부대 좌표확보 기동 추가
// Update: '20.10.30    / 수정내용 : 공성기동 일반부대 확인조건 오류수정

*/


namespace AI优化_战斗部队移动
{

    //=======================================================================================

    // 교전 시 적부대 근접하지 않는 위치로 회피기동
    const bool 间接兵器_回避行动 = true;        // true=적용, false=미적용 (노병,정란,투석: 회피기동)
    const bool 冲车部队_回避行动 = true;        // true=적용, false=미적용 (충차: 회피기동)

    // 거점 공성전 시 기동최적화
    const bool 도시거점_공성전기동 = true;       // 도시 거점 주변 공성병기(1선:충차/목수, 2선:노병/정란/투석)기동. 이동할 좌표 확보하여 공성공격
    const bool 관문항구_공성전기동 = true;       // 관문/항구 주변 공성병기(1선:충차/목수, 2선:노병/정란/투석)기동. 이동할 좌표 확보하여 공성공격
    const int  거점공성_기동부대수 = 3;          // 거점공격용 공성병기 기동 부대수 설정 (턴당/거점당, 최소=1~최대=5)

    const bool 공성부대_우선_기동 = true;       // 거점 주변 공성병기(충차,목수,정란,투석)가 있을 경우 일반부대보다 먼저 이동하여 공성공격

    //=======================================================================================

    class Main
    {

        Main()
        {
            pk::bind(111, pk::trigger111_t(onTurnStart));
        }

        pk::func261_t@ prev_callback_261;

        //---------------------------------------------------------------------------------------

        // AI세력 스크립트 (부대행동 우선순위를 위하여 261함수 대신 턴시작 트리거 사용)
        void onTurnStart(pk::force@ force)
        {
            int force_id = force.get_id();
            if (!pk::is_campaign())
            {
                // 이민족 세력은 제외, 난이도 특급 시에만 동작
                if (pk::is_normal_force(force_id) and pk::get_scenario().difficulty == 难易度_超级 and !force.is_player())
                {
                    // 관문항구 공성전
                    if (관문항구_공성전기동)
                    {
                        for (int j = 건물_관문시작; j < 건물_항구끝; j++)
                        {
                            pk::building@ dst_base = pk::get_building(j);
                            if (pk::enemies_around(dst_base) and pk::is_enemy(dst_base, force))
                            {
                                func_siege_formation(dst_base, force);
                            }
                        }
                    }
                    // 도시거점 공성전
                    if (도시거점_공성전기동)
                    {
                        for (int j = 건물_도시시작; j < 城市_末; j++)
                        {
                            pk::building@ dst_base = pk::get_building(j);
                            if (pk::enemies_around(dst_base) and pk::is_enemy(dst_base, force))
                            {
                                func_siege_formation(dst_base, force);
                                if (공성부대_우선_기동)
                                    func_siege_prior_atk(dst_base, force);
                            }
                        }
                    }

                    // 회피 기동 스크립트
                    if (冲车部队_回避行动 or 间接兵器_回避行动)
                    {
                        pk::array<pk::unit@> arr_unit_force = pk::list_to_array(pk::get_unit_list(force));
                        if (冲车部队_回避行动)
                        {
                            for (int i = 0; i < int(arr_unit_force.length); i++)
                                func_siegeram_evasion(arr_unit_force[i]);
                        }
                        if (间接兵器_回避行动)
                        {
                            for (int i = 0; i < int(arr_unit_force.length); i++)
                                func_archer_evasion(arr_unit_force[i]);
                        }
                    }
                }
            }
        }


        // 병과 속성 확인
        bool is_dir_weapon(int weapon_id) { return ((병기_검 <= weapon_id and weapon_id <= 병기_군마) and weapon_id != 병기_노); }
        bool is_siege_weapon(int weapon_id) { return (병기_충차 <= weapon_id and weapon_id <= 병기_목수); }
        bool is_siegeram_weapon(int weapon_id) { return (병기_충차 == weapon_id or weapon_id == 병기_목수); }
        bool is_siegerng_weapon(int weapon_id) { return (병기_정란 == weapon_id or weapon_id == 병기_투석); }
        bool is_ship_weapon(int weapon_id) { return (weapon_id == 兵器_楼船 or weapon_id == 兵器_斗舰); }



        //=======================================================================================
        //      간접병기부대 회피기동(노병,정란,투석) , 충차부대 회피기동(충차)
        //---------------------------------------------------------------------------------------

        //----- 간접병기부대 회피기동 실행함수
        void func_archer_evasion(pk::unit@ unit)
        {
            if (!check_valid_evasion_unit(unit)) return;
            if (unit.weapon >= 兵器_走舸 or unit.weapon == 병기_군마) return;   // 수군, 기병 제외

            pk::unit@ enemy = get_neighbor_enemy_unit(unit);
            if (!pk::is_alive(enemy)) return;  // 주변에 접촉된 적부대 없음

            int max_range = get_atk_range(unit).second;
            if (max_range <= 1) return;     // 간접병과 부대 아님

            // 간접공성병기인 경우, 이동가능범위 내에 공성가능한 거점 있는지 확인
            if (is_siegerng_weapon(unit.weapon))
            {
                pk::building@ target_base = get_siege_target_base(unit);
                if (pk::is_alive(target_base))
                {
                    atk_target_base(unit, target_base);
                    return;
                }
            }

            pk::point prev_pos = unit.pos;
            pk::point evasion_pos = get_evasion_pos(unit, max_range);   // 회피기동 후보좌표 검색(최대 사거리 내)
            if (evasion_pos == -1 or !pk::is_valid_pos(evasion_pos))
                evasion_pos = get_evasion_pos(unit, max_range + 2);   // 회피기동 후보좌표 재검색 (최대 사거리 밖)
            if (evasion_pos == -1 or !pk::is_valid_pos(evasion_pos)) return;    // 유효좌표 없으면 회피기동 불가

            bool is_moved = unit_movement_control(unit, evasion_pos, /*is_func262*/false);
            if (!is_moved) return;

            run_order_combat_unit(prev_pos, unit, enemy, 3);
        }

        //---------------------------------------------------------------------------------------
        //----- 충차부대 회피기동 실행함수
        void func_siegeram_evasion(pk::unit@ unit)
        {
            if (!check_valid_evasion_unit(unit)) return;
            if (unit.weapon != 병기_충차) return;   // 충차만 대상

            pk::unit@ enemy = get_neighbor_enemy_unit(unit);
            if (!pk::is_alive(enemy)) return;  // 주변에 접촉된 적부대 없음

            // 이동가능범위 내에 공성가능한 거점 있는지 확인
            pk::building@ target_base = get_siege_target_base(unit);
            if (pk::is_alive(target_base))
            {
                atk_target_base(unit, target_base);
                return;
            }

            int max_range = 5;
            pk::point prev_pos = unit.pos;
            pk::point evasion_pos = get_evasion_pos(unit, max_range);   // 회피기동 후보좌표 (최대 5칸밖 조회)
            if (evasion_pos == -1 or !pk::is_valid_pos(evasion_pos)) return;

            bool is_moved = unit_movement_control(unit, evasion_pos, /*is_func262*/false);
            if (!is_moved) return;

            run_order_combat_unit(prev_pos, unit, enemy, 3);
        }

        //---------------------------------------------------------------------------------------

        bool check_valid_evasion_unit(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return false;
            if (unit.action_done) return false;   // 행동완료 부대면 종료
            if (unit.order == 부대임무_퇴각) return false;
            if (unit.status != 部队状态_通常) return false;  // 정상부대가 아니면 종료
            if (pk::is_player_controlled(unit)) return false;   // 플레이어 제외
            return true;
        }

        void atk_target_base(pk::unit@ unit, pk::building@ target_base)
        {
            pk::set_order(unit, 部队任务_攻击, target_base.pos);
            pk::run_order(unit);
            unit.action_done = true;
        }

        //---------------------------------------------------------------------------------------
        // 주변의 일반전투부대 명령 실행
        void run_order_combat_unit(pk::point prev_pos, pk::unit@ unit, pk::unit@ enemy, int range)
        {
            pk::list<pk::unit@> list_unit_combat_evasion;

            pk::array<pk::point> arr_range = pk::range(prev_pos, 1, range);
            for (int i = 0; i < int(arr_range.length); i++)
            {
                pk::point pos_t = arr_range[i];
                pk::unit@ unit_atk = pk::get_unit(pos_t);
                if (pk::is_alive(unit_atk) and !unit_atk.action_done and is_dir_weapon(unit_atk.weapon)
                    and unit_atk.get_id() != unit.get_id() and unit_atk.get_force_id() == unit.get_force_id()
                    and !pk::is_player_controlled(unit_atk) and unit_atk.target_type == 부대임무대상_부대)
                {
                    pk::unit@ target = pk::get_unit(unit_atk.target);
                    if (pk::is_alive(target) and target.get_id() == enemy.get_id() and pk::is_enemy(target, unit_atk) and !list_unit_combat_evasion.contains(unit_atk))
                        list_unit_combat_evasion.add(unit_atk);
                }
            }
            if (list_unit_combat_evasion.count == 0) return;

            list_unit_combat_evasion.sort(function(a, b)
            {
                return ((a.attr.stat[部队能力_攻击] + a.attr.stat[部队能力_防御]) > (b.attr.stat[部队能力_攻击] + b.attr.stat[部队能力_防御]));
            });

            pk::unit@ unit_combat = list_unit_combat_evasion[0];
            if (!unit_combat.action_done)
            {
                pk::run_order(unit_combat);
                unit_combat.action_done = true;
            }
        }


        //---------------------------------------------------------------------------------------
        // 인접 좌표에 적부대 접촉 여부 확인
        pk::unit@ get_neighbor_enemy_unit(pk::unit@ unit)
        {
            for (int i = 0; i < 방향_끝; i++)
            {
                pk::point neighbor_pos = pk::get_neighbor_pos(unit.pos, i);
                if (pk::is_valid_pos(neighbor_pos))
                {
                    pk::unit@ unit_t = pk::get_unit(neighbor_pos);
                    if (pk::is_alive(unit_t) and unit_t.get_id() != unit.get_id() and pk::is_enemy(unit, unit_t) and is_dir_weapon(unit_t.weapon))
                        return unit_t;
                }
            }
            return null;
        }
        //---------------------------------------------------------------------------------------
        // 회피기동 후보좌표 확인 함수
        pk::unit@ unit_temp;
        int unit_max_range;
        pk::point get_evasion_pos(pk::unit@ unit, int range = 1)
        {
            @unit_temp = unit;
            unit_max_range = range;

            pk::array<pk::point> arr_evasion_pos;
            pk::array<pk::point> arr_range = pk::get_movable_pos(unit);  // 이동가능좌표
            for (int j = 0; j < int(arr_range.length); j++)
            {
                bool is_engaged = false;
                pk::point pos = arr_range[j];
                pk::hex@ hex = pk::get_hex(pos);
                int dist_mov = pk::get_distance(unit.pos, pos);
                int dist_ref = pk::max(1, range - 1);
                if (dist_mov <= dist_ref and !hex.has_unit and !hex.has_building)
                {
                    for (int i = 0; i < 방향_끝; i++)
                    {
                        pk::point neighbor_pos = pk::get_neighbor_pos(pos, i);  // 인접좌표 검색
                        if (!is_engaged and pk::is_valid_pos(neighbor_pos))
                        {
                            pk::unit@ unit_t = pk::get_unit(neighbor_pos);
                            if (pk::is_alive(unit_t) and unit_t.get_id() != unit.get_id() and pk::is_enemy(unit, unit_t))
                                is_engaged = true;  // 인접좌표에 적부대 있는 경우
                            if (pk::is_on_fire(neighbor_pos))
                                is_engaged = true;  // 화계 좌표 제외
                        }
                    }
                    if (!is_engaged) arr_evasion_pos.insertLast(pos);   // 회피기동 후보좌표 추가
                }
            }
            if (int(arr_evasion_pos.length) == 0) return -1;

            // 현재 좌표에서 가장 먼 위치로
            arr_evasion_pos.sort(function(a, b)
            {
                int dist_a = pk::get_distance(a, main.unit_temp.pos);
                int dist_b = pk::get_distance(b, main.unit_temp.pos);
                if (dist_a != dist_b) return (dist_a < dist_b);

                bool pos_t_a = pk::is_valid_pos(main.get_atk_target_pos(main.unit_temp, a, main.unit_max_range));
                bool pos_t_b = pk::is_valid_pos(main.get_atk_target_pos(main.unit_temp, b, main.unit_max_range));
                if (pos_t_a and !pos_t_b) return true;
                if (!pos_t_a and pos_t_b) return false;

                return (dist_a <= dist_b);
            });

            pk::point evasion_pos = arr_evasion_pos[0];
            return evasion_pos;
        }

        //---------------------------------------------------------------------------------------

        pk::point set_atk_target(pk::unit@ unit, pk::point evasion_pos, int max_range)
        {
            pk::hex_object@ target = get_target_obj(unit, evasion_pos, max_range);
            if (pk::is_alive(target))
            {
                pk::set_order(unit, 部队任务_攻击, target.pos);
                return target.pos;
            }
            return -1;
        }

        pk::hex_object@ get_target_obj(pk::unit@ unit, pk::point evasion_pos, int max_range)
        {
            if (unit.weapon == 병기_충차) return null;

            pk::point pos_t = get_atk_target_pos(unit, evasion_pos, max_range);
            if (pos_t != -1 and pk::is_valid_pos(pos_t))
            {
                pk::hex_object@ target = pk::get_hex_object(pos_t);
                if (pk::is_alive(target))
                    return target;
            }
            return null;
        }

        // 회피 기동 후 공격 목표 설정
        pk::point get_atk_target_pos(pk::unit@ unit, pk::point evasion_pos, int max_range)
        {
            pk::point target_pos = -1;
            if (!pk::is_alive(unit)) return -1;
            if (max_range < 1) return -1;
            if (unit.weapon == 병기_충차) return -1;

            pk::array<pk::point> arr_target_pos;
            pk::array<pk::point> arr_atk_range = pk::range(evasion_pos, 1, max_range);
            for (int i = 0; i < int(arr_atk_range.length); i++)
            {
                pk::point pos_t = arr_atk_range[i];
                pk::hex_object@ target = pk::get_hex_object(pos_t);
                if (pk::is_alive(target) and pk::is_enemy(target, unit) and pk::get_hex_object_id(unit) != pk::get_hex_object_id(target))
                    arr_target_pos.insertLast(pos_t);
            }
            if (int(arr_target_pos.length) == 0) return -1;

            // 목표 정렬
            if (int(arr_target_pos.length) > 1)
            {
                arr_target_pos.sort(function(a, b)
                {
                    pk::unit@ unit_a = pk::get_unit(a);
                    pk::unit@ unit_b = pk::get_unit(b);
                    if (pk::is_alive(unit_a) and !pk::is_alive(unit_b)) return true;
                    if (!pk::is_alive(unit_a) and pk::is_alive(unit_b)) return false;
                    if (pk::is_alive(unit_a) and pk::is_alive(unit_a)) return (unit_a.troops < unit_b.troops);

                    pk::building@ building_a = pk::get_building(a);
                    pk::building@ building_b = pk::get_building(b);
                    if (pk::is_alive(building_a) and pk::is_alive(building_b)) return (building_a.hp < building_b.hp);

                    return false;
                });
            }
            target_pos = arr_target_pos[0];

            return target_pos;
        }

        //---------------------------------------------------------------------------------------

        pk::building@ get_siege_target_base(pk::unit@ unit)
        {
            if (!pk::is_alive(unit) or !is_siege_weapon(unit.weapon)) return null;

            int max_atk_range = get_atk_range(unit).second;

            pk::list<pk::building@> list_target_base;
            pk::array<pk::point> range = pk::get_movable_pos(unit);  // 이동가능좌표
            for (int i = 0; i < int(range.length); i++)
            {
                pk::point pos = range[i];
                pk::hex@ hex = pk::get_hex(pos);
                pk::array<pk::point> arr_t = pk::range(pos, 1, max_atk_range);   // 주변좌표
                for (int j = 0; j < int(arr_t.length); j++)
                {
                    pk::building@ target_base = pk::get_building(arr_t[j]);
                    if (!hex.has_building and !hex.has_unit and pk::is_alive(target_base)
                        and pk::is_enemy(unit, target_base) and target_base.get_id() < 据点_末
                        and !list_target_base.contains(target_base))
                    {
                        list_target_base.add(target_base);
                    }
                }
            }
            if (list_target_base.count == 0) return null;

            // 목표 거점 정렬
            list_target_base.sort(function(a, b)
            {
                bool city_a = (a.get_id() < 城市_末);
                bool city_b = (b.get_id() < 城市_末);
                if (city_a and !city_b) return true;
                if (!city_a and city_b) return false;

                bool gate_a = (건물_관문시작 <= a.get_id() and a.get_id() < 건물_항구끝);
                bool gate_b = (건물_관문시작 <= b.get_id() and b.get_id() < 건물_항구끝);
                if (gate_a and !gate_b) return true;
                if (!gate_a and gate_b) return false;

                return (a.hp < b.hp);
            });

            pk::building@ target_base = list_target_base[0];
            return target_base;
        }


        //---------------------------------------------------------------------------------------

        // 사거리 --- 미사용
        bool is_valid_range_atk_pos(pk::unit@ src_unit, pk::point target_pos)
        {
            if (!pk::is_alive(src_unit) or !pk::is_valid_pos(target_pos)) return false;

            pk::hex@ hex = pk::get_hex(target_pos);
            if (!hex.has_unit) return false;

            int atk_range = get_archer_range(src_unit);

            if (pk::get_distance(src_unit.pos, target_pos) > atk_range) return false;

            if (hex.terrain == 지형_숲 and src_unit.weapon == 병기_노)
            {
                if (src_unit.has_skill(특기_사수)) return true;
                else return false;
            }
            else return true;
        }

        int get_archer_range(pk::unit@ src_unit)
        {
            int atk_range = 1;
            if (src_unit.weapon == 병기_노)
                atk_range = 2 + ((src_unit.has_tech(기교_강노)) ? 1 : 0);
            else if (src_unit.weapon == 병기_군마)
                atk_range = 1 + ((src_unit.has_tech(기교_기사) or src_unit.has_skill(특기_백마)) ? 1 : 0);

            return atk_range;
        }

        //---------------------------------------------------------------------------------------

        // 부대 공격 최소,최대 사거리 함수 -- 신규 추가, 사용가능한 전법의 세팅조건에 따라 계산
        pk::int_int get_atk_range(pk::unit@ unit)
        {
            pk::int_int atk_range = pk::int_int(1, 1);
            if (!pk::is_alive(unit) or unit.weapon == 병기_검) return atk_range;

            int weapon_id = unit.weapon;
            pk::equipment@ epq = pk::get_equipment(unit.weapon);
            string eqp_name = pk::decode(pk::get_name(epq));
            string info = pk::format("{}", eqp_name);

            int min_range = 5;
            int max_range = 1;

            for (int j = 0; j < 전법_끝; j++)
            {
                if (epq.tactics[j])
                {
                    pk::tactics@ tts = pk::get_tactics(j);
                    string tts_name = pk::decode(pk::get_name(tts));
                    info += pk::format(" {}({},{})", tts_name, tts.min_range, tts.max_range);
                    min_range = pk::min(min_range, tts.min_range);
                    max_range = pk::max(max_range, tts.max_range);
                }
            }
            if (weapon_id == 병기_노 and unit.has_tech(기교_강노)) max_range += 1;
            if (weapon_id == 병기_군마 and (unit.has_tech(기교_기사) or unit.has_skill(특기_백마))) max_range = pk::max(2, max_range);
            if ((weapon_id == 병기_정란 or weapon_id == 병기_투석) and unit.has_skill(特技_射程)) max_range += 1;

            atk_range = pk::int_int(min_range, max_range);
            info += pk::format(": 最小({}),最大({})", min_range, max_range);

            //if (调试模式) pk::info(info);

            return atk_range;
        }


        //=======================================================================================
        //          부대 기동 제어 함수
        //---------------------------------------------------------------------------------------

        bool unit_movement_control(pk::unit@ unit, pk::point evasion_pos, bool is_func262 = false)
        {
            pk::hex@ hex = pk::get_hex(evasion_pos);
            if (hex.has_unit or hex.has_building) return false;

            bool is_moved = false;
            int max_atk_range = get_atk_range(unit).second;
            int move_distance = pk::get_distance(unit.pos, evasion_pos);

            if (unit.order == 부대임무_퇴각) return false;

            // 회피기동 처리
            if (move_distance == 1)  // 회피기동 이동거리 = 1
            {
                // 회피기동 직접 이동 처리
                pk::array<pk::point> evasion_path = { unit.pos, evasion_pos };	// 이동경로
                pk::move(unit, evasion_path);
                if (unit.pos == evasion_pos) is_moved = true;
            }
            else if (move_distance > 1) // 회피기동 이동거리 > 1
            {
                // 기존 부대임무 정보 백업
                int prev_order = unit.order;
                int prev_target = unit.target;
                int prev_target_type = unit.target_type;
                pk::point prev_target_pos = unit.target_pos;

                // 부대임무 업데이트를 통해 유닛 회피 컨트롤
                if (unit.pos != evasion_pos)
                {
                    pk::set_order(unit, 部队任务_移动, evasion_pos);   // 회피기동 목적지로 이동명령
                    pk::run_order(unit);
                    unit.order = prev_order;
                    unit.target = prev_target;
                    unit.target_type = prev_target_type;
                    unit.target_pos = prev_target_pos;

                    if (unit.pos == evasion_pos) is_moved = true;
                }
            }

            if (is_moved)
            {
                // 이동처리 후 공격 가능 시 공격
                if (!unit.action_done and (unit.weapon == 병기_노 or unit.weapon == 병기_군마) and max_atk_range > 1)
                {
                    pk::hex_object@ target = get_target_obj(unit, evasion_pos, max_atk_range);
                    if (pk::is_alive(target))
                    {
                        pk::attack(unit, target);
                        unit.action_done = true;  // 부대행동 완료로 설정
                    }
                }
                if (!unit.action_done and unit.weapon != 병기_충차 and unit.order != 部队任务_攻击)
                {
                    pk::point target_pos = set_atk_target(unit, evasion_pos, max_atk_range + (move_distance - 1));   // 부대임무 공격으로 변경
                    if (!is_func262 and target_pos != -1 and pk::is_valid_pos(target_pos)
                        and pk::get_distance(target_pos, unit.pos) <= max_atk_range and is_siegerng_weapon(unit.weapon))
                    {
                        pk::run_order(unit);
                        unit.action_done = true;  // 부대행동 완료로 설정
                    }
                }
                unit.action_done = true;  // 부대행동 완료로 설정

                if (调试模式)
                {
                    pk::person@ leader = pk::get_person(unit.leader);
                    string unit_name = (pk::is_alive(leader)) ? pk::decode(pk::get_name(leader)) : "??";
                    pk::info(pk::format("野战移动: {}({}) 移动", unit_name, get_weapon_name(unit.weapon)));
                }
            }

            return is_moved;
        }



        //=======================================================================================
        //      도시/관문/항구 공성전 기동 (공성병기 우선기동) 城市/关卡/港口 攻城战移动(攻城兵器优先移动)  
        //---------------------------------------------------------------------------------------

        void func_siege_prior_atk(pk::building@ dst_base, pk::force@ force)
        {
            if (!pk::is_alive(dst_base) or !pk::is_normal_force(force)) return;
            if (!pk::enemies_around(dst_base)) return;
            string base_name = pk::decode(pk::get_name(dst_base));

            // 목표거점 주변 공성부대 검색
            get_siege_units_nearby(dst_base, force, 3);    // 검색거리=3 으로 제한

            if (list_siege_unit_around.count == 0) return;   // 인접한 공성병기 있음

            pk::array<pk::unit@> arr_siege_unit = pk::list_to_array(list_siege_unit_around);
            for (int i = 0; i < int(arr_siege_unit.length); i++)
            {
                pk::unit@ siege_unit = arr_siege_unit[i];

                if (!siege_unit.action_done)
                {
                    pk::person@ leader = pk::get_person(siege_unit.leader);
                    string siege_unit_name = (pk::is_alive(leader)) ? pk::decode(pk::get_name(leader)) : "??";
                    string weapon_name = get_weapon_name(siege_unit.weapon);
                    pk::info(pk::format("任务执行前，攻城活动: {}({}) 优先攻击 {}，已行动:{}", siege_unit_name, weapon_name, base_name, siege_unit.action_done));
                    pk::set_order(siege_unit, 部队任务_攻击, dst_base.pos);
                    pk::run_order(siege_unit);
                    siege_unit.action_done = true;
                    pk::info(pk::format("任务执行后，攻城活动: {}({}) 优先攻击 {}，已行动:{}", siege_unit_name, weapon_name, base_name, siege_unit.action_done));

                    if (调试模式)
                    {
                        pk::info(pk::format("攻城活动: {}({}) 优先攻击 {}", siege_unit_name, weapon_name, base_name));
                    }
                }
            }
        }

        // 거점 주변의 공성병기 부대 검색
        pk::list<pk::unit@> list_siege_unit_around;
        void get_siege_units_nearby(pk::building@ dst_base, pk::force@ force, int search_range)
        {
            if (!pk::is_alive(dst_base) or dst_base.get_id() >= 据点_末) return;

            list_siege_unit_around.clear();

            int dist_add = (dst_base.facility == 시설_도시) ? 1 : 0;

            pk::array<pk::point> arr_range = pk::range(dst_base.pos, 1, (search_range + dist_add));
            for (int i = 0; i < int(arr_range.length); i++)
            {
                pk::point pos_t = arr_range[i];
                if (pk::is_valid_pos(pos_t) and (pk::get_building_id(pos_t) == dst_base.get_id() or pk::get_distance(pos_t, dst_base.pos) <= 4))
                {
                    pk::unit@ unit = pk::get_unit(pos_t);
                    if (pk::is_alive(unit) and unit.get_force_id() == force.get_id() and !pk::is_player_controlled(unit) and !pk::is_in_water(unit) and !unit.action_done and unit.status == 部队状态_通常)
                    {
                        if (is_siege_weapon(unit.weapon) and !list_siege_unit_around.contains(unit))
                            list_siege_unit_around.add(unit);
                    }
                }
            }
        }

        // 부대임무 공격목표가 해당 거점인지 확인
        bool is_target_base(pk::unit@ unit, pk::building@ dst_base)
        {
            if (!pk::is_alive(unit) or !pk::is_alive(dst_base)) return false;
            if (unit.target_type == 부대임무대상_거점)
            {
                pk::building@ base_t = pk::get_building(unit.target);
                if (pk::is_alive(base_t) and base_t.get_id() == dst_base.get_id())
                    return true;
            }
            return false;
        }

        //=======================================================================================
        //      도시/관문/항구 공성전 기동 (충차/목수, 정란/투석 공격위치 최적화)
        //---------------------------------------------------------------------------------------

        void func_siege_formation(pk::building@ dst_base, pk::force@ force)
        {
            if (!pk::is_alive(dst_base) or !pk::is_normal_force(force)) return;
            if (!pk::enemies_around(dst_base)) return;

            bool cmd_1 = true;
            bool cmd_2 = true;
            int unit_count = 0;
            int push_count = 0;
            int max_count = pk::max(1, pk::min(5, 거점공성_기동부대수));
            while (push_count < max_count and unit_count < max_count)
            {
                push_count++;
                if (cmd_1 and PushSiegeRamFormation(dst_base, force)) unit_count++;
                else cmd_1 = false;

                if (cmd_2 and PushSiegeTwrFormation(dst_base, force)) unit_count++;
                else cmd_2 = false;
            }
        }

        // 직접공성병기(충차/목수) 공성 진형
        bool PushSiegeRamFormation(pk::building@ dst_base, pk::force@ force)
        {
            // 목표거점 주변의 거리별 일반/공성부대 검색
            get_atk_units_nearby(dst_base, force, 4);    // 검색거리 제한

            if (list_siegeram_unit.count == 0) return false;   // 주변에 공성병기 없음

            pk::person@ kunshu = pk::get_person(pk::get_kunshu_id(force));
            string force_name = pk::decode(pk::get_name(kunshu));
            string base_name = pk::decode(pk::get_name(dst_base));
            if (调试模式) pk::info(pk::format("攻城活动: {}→{}, 邻近攻城部队数={}, 邻近一般部队数={}", force_name, base_name, list_siegeram_unit.count, list_combat_unit_n1.count));

            // 공성부대 선택
            sort_list_siegeram_unit();     // 리스트 정렬
            pk::unit@ siege_unit = list_siegeram_unit[0];

            // 목표거점 주변의 빈좌표 검색
            int num_empty_pos = count_empty_neighbor_pos(dst_base, siege_unit, /*distance*/1);

            // 1선의 일반부대 회피기동 (빈 좌표 없을 경우)
            if (list_combat_unit_n1.count > 0 and num_empty_pos == 0)
            {
                sort_list_combat_unit_n1();    // 리스트 정렬
                pk::unit@ combat_unit = list_combat_unit_n1[0];
                pk::point dst_pos = combat_unit.pos;   // 현재 좌표 저장 (공성부대가 이동할 자리)
                pk::point evasion_pos = get_evasion_pos(combat_unit, /*max_range*/4);   // 후퇴기동 후보좌표
                if (evasion_pos == -1 or !pk::is_valid_pos(evasion_pos)) return false;    // 유효좌표 없으면 후퇴기동 불가

                string combat_unit_name = pk::decode(pk::get_name(pk::get_person(combat_unit.leader)));
                if (调试模式) pk::info(pk::format("攻城活动: {} 后退坐标({},{})", combat_unit_name, evasion_pos.x, evasion_pos.y));

                bool is_moved = unit_movement_control(combat_unit, evasion_pos, /*is_func262*/false);
                if (!is_moved) return false;     // 이동 못했으면 종료

                // 목표거점 주변의 빈좌표 재검색
                num_empty_pos = count_empty_neighbor_pos(dst_base, siege_unit, /*distance*/1);
            }

            // 공성부대 거점공격 攻城部队据点攻击
            if (!siege_unit.action_done and num_empty_pos > 0)
            {
                pk::person@ leader = pk::get_person(siege_unit.leader);
                string siege_unit_name = (pk::is_alive(leader)) ? pk::decode(pk::get_name(leader)) : "??";
                string weapon_name = get_weapon_name(siege_unit.weapon);
                pk::info(pk::format("任务执行前，攻城活动: {}({}) 接近攻城 {}，已行动:{}", siege_unit_name, weapon_name, base_name, siege_unit.action_done));
                pk::set_order(siege_unit, 部队任务_攻击, dst_base.pos);
                pk::run_order(siege_unit);
                pk::info(pk::format("任务执行后，攻城活动: {}({}) 接近攻城 {}，已行动:{}", siege_unit_name, weapon_name, base_name, siege_unit.action_done));
                siege_unit.action_done = true;


                if (调试模式)
                {
                    pk::info(pk::format("攻城活动: {}({}) {} 接近攻城", siege_unit_name, weapon_name, base_name));
                }
                return true;
            }
            return false;
        }

        // 간접공성병기(정란/투석) 공성 진형
        bool PushSiegeTwrFormation(pk::building@ dst_base, pk::force@ force)
        {
            // 목표거점 주변의 거리별 일반/공성부대 검색
            get_atk_units_nearby(dst_base, force, 4);    // 검색거리 제한

            if (list_range_unit.count == 0) return false;   // 주변에 간접병기 없음

            pk::person@ kunshu = pk::get_person(pk::get_kunshu_id(force));
            string force_name = pk::decode(pk::get_name(kunshu));
            string base_name = pk::decode(pk::get_name(dst_base));
            if (调试模式) pk::info(pk::format("攻城活动: {}→{}, 间接攻城部队数={}, 普通部队数={}", force_name, base_name, list_range_unit.count, list_combat_unit_n2.count));

            // 공성부대 선택
            sort_list_range_unit();     // 리스트 정렬
            pk::unit@ siege_unit = list_range_unit[0];

            // 목표거점 주변의 빈좌표 검색
            int num_empty_pos = count_empty_neighbor_pos(dst_base, siege_unit, /*distance*/2);

            // 2선의 일반부대 회피기동
            if (list_combat_unit_n2.count > 0 and num_empty_pos == 0)
            {
                sort_list_combat_unit_n2();    // 리스트 정렬
                pk::unit@ combat_unit = list_combat_unit_n2[0];
                pk::point dst_pos = combat_unit.pos;   // 현재 좌표 저장 (공성부대가 이동할 자리)
                pk::point evasion_pos = get_evasion_pos(combat_unit, /*max_range*/4);   // 후퇴기동 후보좌표
                if (evasion_pos == -1 or !pk::is_valid_pos(evasion_pos)) return false;    // 유효좌표 없으면 후퇴기동 불가

                string combat_unit_name = pk::decode(pk::get_name(pk::get_person(combat_unit.leader)));
                if (调试模式) pk::info(pk::format("攻城活动: {} 后退坐标({},{})", combat_unit_name, evasion_pos.x, evasion_pos.y));

                bool is_moved = unit_movement_control(combat_unit, evasion_pos, /*is_func262*/false);
                if (!is_moved) return false;     // 이동 못했으면 종료

                // 목표거점 주변의 빈좌표 재검색
                num_empty_pos = count_empty_neighbor_pos(dst_base, siege_unit, /*distance*/2);
            }

            // 공성부대 거점공격
            if (!siege_unit.action_done and num_empty_pos > 0)
            {
                pk::set_order(siege_unit, 部队任务_攻击, dst_base.pos);
                pk::run_order(siege_unit);
                siege_unit.action_done = true;

                if (调试模式)
                {
                    pk::person@ leader = pk::get_person(siege_unit.leader);
                    string siege_unit_name = (pk::is_alive(leader)) ? pk::decode(pk::get_name(leader)) : "??";
                    string weapon_name = get_weapon_name(siege_unit.weapon);

                    pk::info(pk::format("攻城活动: {}({}) 对{}执行间接攻城", siege_unit_name, weapon_name, base_name));
                }
                return true;
            }
            return false;
        }



        //---------------------------------------------------------------------------------------

        pk::list<pk::unit@> list_combat_unit_n1;
        pk::list<pk::unit@> list_combat_unit_n2;
        pk::list<pk::unit@> list_siegeram_unit;
        pk::list<pk::unit@> list_range_unit;

        // 거점 주변의 부대 검색 (미행동 및 정상상태여야 하고, 공성부대는 직접병기만)
        void get_atk_units_nearby(pk::building@ dst_base, pk::force@ force, int search_range)
        {
            if (!pk::is_alive(dst_base) or !pk::is_normal_force(force) or dst_base.get_id() >= 据点_末) return;

            list_combat_unit_n1.clear();
            list_combat_unit_n2.clear();
            list_siegeram_unit.clear();
            list_range_unit.clear();

            int dist_add = (dst_base.facility == 시설_도시) ? 1 : 0;

            pk::array<pk::point> arr_range = pk::range(dst_base.pos, 1, (search_range + dist_add));
            for (int i = 0; i < int(arr_range.length); i++)
            {
                pk::point pos_t = arr_range[i];
                if (pk::is_valid_pos(pos_t) and (pk::get_building_id(pos_t) == dst_base.get_id() or pk::get_distance(pos_t, dst_base.pos) <= 4))
                {
                    pk::unit@ unit = pk::get_unit(pos_t);
                    if (pk::is_alive(unit) and unit.get_force_id() == force.get_id() and !pk::is_player_controlled(unit) and !pk::is_in_water(unit) and !unit.action_done and unit.status == 部队状态_通常)
                    {
                        pk::int_int unit_range = get_atk_range(unit);
                        int min_range = unit_range.first;
                        int max_range = unit_range.second;
                        int distance = pk::get_distance(unit.pos, dst_base.pos) - dist_add;
                        // 충차/목수, 근거리/원거리
                        if (is_siegeram_weapon(unit.weapon))
                        {
                            if (distance > 1 and !list_siegeram_unit.contains(unit))   // 1선 제외
                                list_siegeram_unit.add(unit);
                        }
                        // 간접병기 : 노/정란/투석 (사정거리에 있는 부대는 리스트에서 제외)
                        if (is_siegerng_weapon(unit.weapon) or unit.weapon == 병기_노)
                        {
                            if (!(min_range <= distance and distance <= max_range) and !list_range_unit.contains(unit))
                                list_range_unit.add(unit);
                        }
                        // 일반부대 (창극노기)
                        if (unit.weapon <= 병기_군마)
                        {
                            if (distance == 1 and !list_combat_unit_n1.contains(unit))  // 1선
                                list_combat_unit_n1.add(unit);
                            else if (distance == 2 and distance > max_range and !list_combat_unit_n2.contains(unit))    // 2선
                                list_combat_unit_n2.add(unit);
                        }
                    }
                }
            }
        }

        //---------------------------------------------------------------------------------------

        // 일반전투부대 리스트 정렬 (노병 우선, 노병부대는 강한 부대 우선, 근접부대는 약한 부대 우선)
        void sort_list_combat_unit_n1()
        {
            if (list_combat_unit_n1.count <= 1) return;

            list_combat_unit_n1.sort(function(a, b)
            {
                bool skill_a = (a.has_skill(特技_工神) or a.has_skill(特技_攻城));
                bool skill_b = (b.has_skill(特技_工神) or b.has_skill(特技_攻城));

                int wpn_a = pk::get_ground_weapon_id(a);
                int wpn_b = pk::get_ground_weapon_id(b);
                if (wpn_a == 병기_노 and wpn_b != 병기_노) return true;
                if (wpn_a != 병기_노 and wpn_b == 병기_노) return false;
                if (wpn_a == 병기_노 and wpn_b == 병기_노)
                {
                    if (skill_a and !skill_b) return true;
                    if (!skill_a and skill_b) return false;
                    return (a.attr.stat[部队能力_攻击] > b.attr.stat[部队能力_攻击]);
                }

                return (a.attr.stat[部队能力_攻击] < b.attr.stat[部队能力_攻击]);
            });
        }

        // 일반전투부대 리스트 정렬 (근접부대 우선)
        void sort_list_combat_unit_n2()
        {
            if (list_combat_unit_n2.count <= 1) return;

            list_combat_unit_n2.sort(function(a, b)
            {
                int wpn_a = pk::get_ground_weapon_id(a);
                int wpn_b = pk::get_ground_weapon_id(b);
                if (wpn_a != 병기_노 and wpn_b != 병기_노) return (a.attr.stat[部队能力_攻击] < b.attr.stat[部队能力_攻击]);
                if (wpn_a == 병기_노 and wpn_b != 병기_노) return false;
                if (wpn_a != 병기_노 and wpn_b == 병기_노) return true;

                return (a.attr.stat[部队能力_攻击] < b.attr.stat[部队能力_攻击]);
            });
        }

        // 직접병기 공성부대 리스트 정렬 (특기 우선, 공격력 우선)
        void sort_list_siegeram_unit()
        {
            if (list_siegeram_unit.count <= 1) return;

            list_siegeram_unit.sort(function(a, b)
            {
                bool skill_a = (a.has_skill(特技_工神) or a.has_skill(特技_攻城));
                bool skill_b = (b.has_skill(特技_工神) or b.has_skill(特技_攻城));

                if (skill_a and !skill_b) return true;
                if (!skill_a and skill_b) return false;

                return (a.attr.stat[部队能力_攻击] > b.attr.stat[部队能力_攻击]);
            });
        }


        // 간접병기부대 리스트 정렬 (공성병기 우선, 특기 우선, 공격력 우선)
        void sort_list_range_unit()
        {
            if (list_range_unit.count <= 1) return;

            list_range_unit.sort(function(a, b)
            {
                bool siege_a = main.is_siegerng_weapon(a.weapon);
                bool siege_b = main.is_siegerng_weapon(b.weapon);
                if (siege_a and !siege_b) return true;
                if (!siege_a and siege_b) return false;

                bool skill_a = (a.has_skill(特技_工神) or a.has_skill(特技_攻城));
                bool skill_b = (b.has_skill(特技_工神) or b.has_skill(特技_攻城));
                if (skill_a and !skill_b) return true;
                if (!skill_a and skill_b) return false;

                return (a.attr.stat[部队能力_攻击] > b.attr.stat[部队能力_攻击]);
            });
        }

        //---------------------------------------------------------------------------------------
        // 주변에 비어있는 육상 좌표 개수 반환
        int count_empty_neighbor_pos(pk::building@ dst_base, pk::unit@ unit, int distance = 1)
        {
            if (!pk::is_alive(dst_base) or !pk::is_alive(unit)) return -1;

            int dist_add = (dst_base.facility == 시설_도시) ? 1 : 0;
            int dist_search = pk::max(1, pk::min(2, distance));

            pk::array<pk::point> arr_range = pk::get_movable_pos(unit);
            pk::array<pk::point> arr_empty_pos;
            for (int i = 0; i < int(arr_range.length); i++)
            {
                pk::point pos_t = arr_range[i];
                int dist_pos = pk::get_distance(pos_t, dst_base.pos) - dist_add;
                pk::hex@ hex = pk::get_hex(pos_t);
                if (dist_search == dist_pos and !hex.has_building and !hex.has_unit and !pk::is_water_terrain(hex.terrain))
                    arr_empty_pos.insertLast(pos_t);
            }

            if (调试模式) pk::info(pk::format("攻城活动: 空坐标数={}", int(arr_empty_pos.length)));

            return int(arr_empty_pos.length);
        }


        //---------------------------------------------------------------------------------------

        string get_weapon_name(int weapon_id)
        {
            string weapon_name;
            switch (weapon_id)
            {
            case 병기_검: weapon_name = "剑兵"; break;
            case 병기_창: weapon_name = "枪兵"; break;
            case 병기_극: weapon_name = "戟兵"; break;
            case 병기_노: weapon_name = "弩兵"; break;
            case 병기_군마: weapon_name = "骑兵"; break;
            case 병기_충차: weapon_name = "冲车"; break;
            case 병기_정란: weapon_name = "井阑"; break;
            case 병기_투석: weapon_name = "投石"; break;
            case 병기_목수: weapon_name = "木兽"; break;
            case 兵器_走舸: weapon_name = "水军"; break;
            case 兵器_楼船: weapon_name = "水军"; break;
            case 兵器_斗舰: weapon_name = "水军"; break;
            default: weapon_name = "??"; break;
            }
            return weapon_name;
        }

        string get_order_info(int order)
        {
            string name;
            switch (order)
            {
            case 0: name = "部队任务_待命"; break;
            case 1: name = "部队任务_移动"; break;
            case 2: name = "部队任务_设置"; break;
            case 3: name = "部队任务_攻击"; break;
            case 4: name = "部队任务_撤退"; break;
            case 5: name = "部队任务_拦截"; break;
            case 6: name = "部队任务_护卫"; break;
            case 7: name = "部队任务_攻城"; break;
            case 8: name = "部队任务_修复"; break;
            case 9: name = "部队任务_征服"; break;
            case 10: name = "部队任务_补给"; break;
            case 11: name = "部队任务_歼灭"; break;
            case 12: name = "部队任务_追随"; break;
            case 13: name = "部队任务_末"; break;
            default: name = "部队任务_没有"; break;
            }

            return name;
        }

        //---------------------------------------------------------------------------------------


        bool 调试模式 = false;




    };

    Main main;
}
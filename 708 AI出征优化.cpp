// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/11/14 # 江东新风 # 基本完善 ##
// ## 2021/11/10 # 江东新风 # 高效的ai出征优化 ##

namespace ai出征优化
{
    // ================ CUSTOMIZE ================
    const bool 开启ai出征优化 = true;//是否开启ai出征优化

        // 如果据点存在未出征的高能力武将，则主将更换将被应用 거점에 미출진한 높은 능력의 무장 있는 경우 주장 변경 적용함
    const bool 优化出征武将_武将变更 = true;    // true=적용,  false=미적용

    // 能力值低于基准值的武将将被变更 기준값 이하 무장이 주장인 경우 변경
    const int 武将变更_能力基准值 = 70;

    // 出征武将能力比较权重(无预设范围限制)출진무장 능력 비교용 가중치 (설정값 범위제약 없음)
    const float 武将变更_统率权重 = 0.8f;
    const float 武将变更_武力权重 = 1.0f;
    const float 武将变更_智力权重 = 0.5f;

    // 무장변경 기준이하 적성 고려 ('19.2.9)
    const bool 武将变更_启用适性标准 = true;      // 기준이하의 적성 시 변경 적용
    const int  武将变更_适性基准值 = 1;           // 0:适性_C,   1:적성_B,   2:적성_A,   3:적성_S 이상
    const float 武将变更_适性权重 = 0.3f;

    // 무장변경 특기 보유자 우선 고려 ('20.9.16)
    const bool 武将变更_启用特技加权 = true;    //true=启用적용,  false=不启用미적용
    // 건설편제 판단 조건 ('19.2.15)
    const bool 建设编制_除外设定 = true;      // 건설편제 조건 만족 시 AI부대 무장변경 안함 (特技_筑城 보유 또는 금/병력 기준 판단)
    const int 建设编制_持有金标准 = 1500;     // 기준 소지금 이상 & 기준 병력 이하일 경우 건설부대로 취급하여 무장변경 안함
    const int 建设编制_兵力标准 = 3000;     // 기준 소지금 이상 & 기준 병력 이하일 경우 건설부대로 취급하여 무장변경 안함

    //---------------------------------------------------------------------------------------
    // 对主将/副将能力的比较以能力优先重编주장/부장 능력 비교하여 능력 우선으로 재편함
    const bool 优化出征武将_指挥重组 = false;     // true= 启用적용,  false= 不启用미적용
    const bool 指挥重组_时刻检查 = false;         // true= 部队移动时检查부대이동 시 마다 점검,  false= 部队出征时检查부대출진 때만 점검
    const bool 指挥重组_无视官职 = true;         // 无视官职指挥兵力，以统/武优先重组主将 관직에 따른 지휘가능병력 무시하고, 통/무 능력 우선으로 주장 재편

    //---------------------------------------------------------------------------------------

    //*******************部队奇袭攻具相关*****************************//

    const int min_distance = 1;      // 城市默认在此范围加1，从而和港口区分
    const int max_distance = 3;      // 城市默认在此范围加1，从而和港口区分
    //**************************************************************//
    const bool 调试模式 = false;
    // ===========================================
    class Main
    {
       
        Main()
        {
            pk::bind(201, pk::trigger201_t(onAIMarch));
            pk::bind(202, pk::trigger202_t(onAIRunningOrder));
        }

        void onAIMarch(pk::ai_context@ context, pk::com_march_cmd_info& info, int type/*0防御，1攻击，2建设，3运输*/)
        {
            //pk::trace("start trace");
            int src_id = (info.base is null) ? -1 : info.base.get_id();
            if (调试模式)
            {
                string t = "主将:" + pk::decode(pk::get_name(pk::get_person(info.member[0]))) + ",副将1:" + (info.member[1] == -1 ? "无" : pk::decode(pk::get_name(pk::get_person(info.member[1])))) + ",副将2:" + (info.member[2] == -1 ? "无" : pk::decode(pk::get_name(pk::get_person(info.member[2]))));
                t += "出征据点：" + src_id + ",目标: " + info.target + "x:" + info.target_pos.x + ", y:" + info.target_pos.y + ",类型:" + type + ",兵粮：" + info.food + ",兵力：" + info.troops;
                pk::trace(t);
            }

            pk::hex_object@ target = pk::get_hex_object(info.target_pos);
            pk::building@ target_building;
            pk::unit@ target_unit;
            if (target !is null)
            {
                @ target_building = target.get_type_id() == pk::building::type_id ? target : null;
                @ target_unit = target.get_type_id() == pk::unit::type_id ? target : null;
            }

            auto member_list = get_member_list(info);

            if (type == 0 or type == 1)//防御和攻击
            {
                //只根据特技优化兵种好了，前期电脑已经根据带兵量，能力，适性进行过最佳选择
                //modify_normal_leader(info, member_list);
                int final_weapon = get_best_weapon(member_list, info.weapon_id[0], info.base, type);
                int pre_weapon = info.weapon_id[0];
                if (final_weapon != pre_weapon)
                {
                    if (调试模式) 
                    {
                        string t = "主将：" + pk::decode(pk::get_name(member_list[0])) + "兵种由" + pk::decode(pk::get_equipment(pre_weapon).name) + "变为" + pk::decode(pk::get_equipment(final_weapon).name);
                        //t += "\n变化前" + ",max:" + max_skill_score[pre_weapon] + ",all:" + all_skill_score[pre_weapon] + "\n变化后" + ",max:" + max_skill_score[final_weapon] + ",all:" + all_skill_score[final_weapon];
                        pk::trace(t);
                    }

                    info.weapon_id[0] = final_weapon;                    
                    if (final_weapon > 0 and final_weapon < 5)
                    {
                        int base_weapon_amount = pk::get_weapon_amount(info.base, info.weapon_id[0]);
                        if (base_weapon_amount < info.troops) info.troops = base_weapon_amount;
                    }
                    
                }

            }

            if (type == 3) modify_transport_leader(info, member_list);

            if (target_building !is null and type == 1)
            {
                int dst_id = target_building.get_id();                
                //兵粮优化
                if (pk::is_valid_base_id(dst_id)) modify_food(info, src_id, dst_id);
            }
            //auto x = context.num_orders;
            //auto x1 = context.order;
            //info.member[0] = 武将_吕布;
            return;
        }


        void onAIRunningOrder(pk::ai_context@ context, pk::building@ building, int cmd)
        {
            if (cmd == 据点AI_部队奇袭攻具)
            {
                pk::list<pk::person@> person_sel;
                pk::list<pk::unit@> unit_sel;
                if (run_order_before(person_sel, building, unit_sel)) run_order(person_sel, building, unit_sel);
                else if (调试模式) pk::trace(pk::get_new_base_name(building.get_id()) + "据点AI_部队奇袭攻具 不满足");
            }
        }


        bool run_order_before(pk::list<pk::person@>& out person_sel, pk::building@ building0, pk::list<pk::unit@>& out unit_sel)
        {
            if (pk::get_troops(building0) < 12000) return false;
            pk::list<pk::person@> person_list = pk::get_idle_person_list(building0);
            if (person_list.count == 0) return false;
            pk::list<pk::unit@> dst_list = get_around_seige_unit(building0);
            if (dst_list.count == 0) return false;
            dst_list.sort(function(a, b)
            {
                return (a.troops < b.troops);
            });
            unit_sel.add(dst_list[0]);
            if (pk::get_weapon_amount(building0, 兵器_弩) > 3000)
            {
                person_list.sort(function(a, b)
                {
                    float apt_a = 1.f + 10.f * (a.tekisei[兵种_弩兵] - 3) / 100.f;
                    float apt_b = 1.f + 10.f * (b.tekisei[兵种_弩兵] - 3) / 100.f;
                    float stat_a = (UNIT_ATTR::攻武占比 * a.stat[武将能力_武力] + UNIT_ATTR::攻统占比 * a.stat[武将能力_统率]) * apt_a;
                    float stat_b = (UNIT_ATTR::攻武占比 * b.stat[武将能力_武力] + UNIT_ATTR::攻统占比 * b.stat[武将能力_统率]) * apt_b;
                    return (stat_a > stat_b);
                });
                person_sel.add(person_list[0]);
            }
            else
            {
                person_list.sort(function(a, b)
                {
                    float apt_a = main.get_weapon_tekisei_buf(a);
                    float apt_b = main.get_weapon_tekisei_buf(b);
                    float stat_a = (UNIT_ATTR::攻武占比 * a.stat[武将能力_武力] + UNIT_ATTR::攻统占比 * a.stat[武将能力_统率]) * apt_a;
                    float stat_b = (UNIT_ATTR::攻武占比 * b.stat[武将能力_武力] + UNIT_ATTR::攻统占比 * b.stat[武将能力_统率]) * apt_b;
                    return (stat_a > stat_b);
                });
                person_sel.add(person_list[0]);
            }

            return true;
        }

        float get_weapon_tekisei_buf(pk::person@ person)
        {
            int best_weapon = 0;
            for (int i = 兵种_枪兵; i <= 兵种_骑兵; ++i)
            {
                if (person.tekisei[i] > person.tekisei[best_weapon]) best_weapon = i;
            }
            float buf = 1.f + 10.f * (person.tekisei[best_weapon] - 3) / 100.f;
            return buf;
        }

        bool run_order(pk::list<pk::person@> person_sel, pk::building@ building0, pk::list<pk::unit@>& unit_sel)
        {
            if (调试模式)pk::trace(pk::get_new_base_name(building0.get_id()) + "开始执行AI_部队奇袭攻具0");
            if (building0 is null) return false;
            if (调试模式)pk::trace(pk::get_new_base_name(building0.get_id()) + "开始执行AI_部队奇袭攻具1");
            if (person_sel.count == 0) return false;
            if (调试模式)pk::trace(pk::get_new_base_name(building0.get_id()) + "开始执行AI_部队奇袭攻具2");
            if (unit_sel.count == 0) return false;
            if (调试模式)pk::trace(pk::get_new_base_name(building0.get_id()) + "开始执行AI_部队奇袭攻具3");
            if (调试模式)pk::trace(pk::decode(pk::get_name(unit_sel[0])) + "--攻击改目标");
            // 출진 명령 정보 생성.
            pk::com_deploy_cmd_info cmd;
            @cmd.base = @building0;
            cmd.type = 部队类型_战斗;
            cmd.member[0] = person_sel[0].get_id();
            cmd.gold = 0;
            cmd.food = 1000;
            cmd.troops = 3000;
            cmd.order = 部队任务_拦截;
            int weapon_id = (pk::get_weapon_amount(building0, 兵器_弩) > 3000)? 兵器_弩: get_best_weapon(person_sel, 兵器_弩, building0, 0/*0为防御，1位攻击*/);
            cmd.weapon_id[0] = weapon_id;
            cmd.weapon_id[1] = 兵器_走舸;
            cmd.weapon_amount[0] = 3000;
            cmd.weapon_amount[1] = 3000;
            cmd.target_pos = unit_sel[0].get_pos();
            int unit_id = pk::command(cmd);
            pk::unit@ unit_cmd = pk::get_unit(unit_id);
            //if (pk::is_alive(unit_cmd))
            //    unit_cmd.action_done = (!거점철수_원거리이동);
            return true;

        }

        bool is_seige_weapon_around(pk::building@ building)
        {
            pk::point src_pos = building.pos;
            array<pk::point> arr = pk::range(building.get_pos(), min_distance + ((building.facility == 设施_城市) ? 1 : 0), max_distance + ((building.facility == 设施_城市) ? 1 : 0));
            int count = 0;
            for (int j = 0; j < int(arr.length); j++)
            {
                auto hex = pk::get_hex(arr[j]);
                if (!hex.has_unit) continue;
                pk::unit@ dst_unit = pk::get_unit(arr[j]);
                if (dst_unit is null or !pk::is_enemy(building, dst_unit)) continue;
                if (dst_unit.weapon < 5 or dst_unit.weapon > 8) continue;
                return true;
            }
            return false;
        }

        pk::list<pk::unit@> get_around_seige_unit(pk::building@ building)
        {
            pk::point src_pos = building.pos;
            array<pk::point> arr = pk::range(building.get_pos(), min_distance + ((building.facility == 设施_城市) ? 1 : 0), max_distance + ((building.facility == 设施_城市) ? 1 : 0));
            int count = 0;
            pk::list<pk::unit@> list;
            for (int j = 0; j < int(arr.length); j++)
            {
                auto hex = pk::get_hex(arr[j]);
                if (!hex.has_unit) continue;
                pk::unit@ dst_unit = pk::get_unit(arr[j]);
                if (dst_unit is null or !pk::is_enemy(building, dst_unit)) continue;
                if (dst_unit.weapon < 5 or dst_unit.weapon > 8) continue;
                list.add(dst_unit);
            }
            return list;
        }

        int get_best_weapon2(pk::person@person)
        {
            int best_weapon = 0;
            int best_heishu = 0;
            for (int i = 兵种_枪兵; i <= 兵种_骑兵; ++i)
            {
                if (person.tekisei[i] >= person.tekisei[best_heishu]) best_heishu = i;
            }
            best_weapon = best_heishu + 1;
            return best_weapon;
        }
        //===================================================优化函数==========================================================//


        int t_weapon_heishu;
        void modify_normal_leader(pk::com_march_cmd_info& info, pk::list<pk::person@> member_list)
        {
            auto idle_person_list = pk::get_idle_person_list(info.base);
            if (idle_person_list.count == 0) return;//理论上不会触发，因为老早就判断过了
            if (!check_leader_attr(member_list,info.weapon_id[0])) return;//水军的情况未考虑
            //筛选最佳权重武将
            t_weapon_heishu = pk::equipment_id_to_heishu(info.weapon_id[0]);
            idle_person_list.sort(function(a, b)
            {
                int stat_a = main.get_stat_sum(a, true) + main.get_stat_tekisei(a, main.t_weapon_heishu) + main.get_stat_skill(a, main.t_weapon_heishu);
                int stat_b = main.get_stat_sum(b, true) + main.get_stat_tekisei(b, main.t_weapon_heishu) + main.get_stat_skill(b, main.t_weapon_heishu);
                return (stat_a > stat_b);
            });
            pk::person@ new_leader = @idle_person_list[0];
            pk::person@ old_leader = pk::get_person(info.member[0]);
            int stat_new_leader = get_stat_sum(new_leader, true) + get_stat_tekisei(new_leader,info.weapon_id[0]);     // 武将能力适性加权值
            int stat_old_leader = get_stat_sum(old_leader, false) + get_stat_member_tekisei(member_list,info.weapon_id[0]) + int(武将变更_智力权重 * ch::get_best_member_stat(member_list, 武将能力_智力));

            if (stat_old_leader < stat_new_leader) info.member[0] = new_leader.get_id();
        }

        void modify_transport_leader(pk::com_march_cmd_info& info, pk::list<pk::person@> member_list)
        {
            if (ch::has_skill(member_list, 特技_运输)) return;
            auto idle_person_list = pk::get_idle_person_list(info.base);
            if (idle_person_list.count == 0) return;//理论上不会触发，因为老早就判断过了
            if (ch::has_skill(idle_person_list, 特技_运输)) return;

            idle_person_list.sort(function(a, b)
            {
                bool a_skill = ch::has_skill(a, 特技_运输);
                bool b_skill = ch::has_skill(b, 特技_运输);
                if (a_skill and !b_skill) return true;
                if (!a_skill and b_skill) return false;
                return (float(a.stat[武将能力_政治]) / float(a.stat[武将能力_武力])) > (float(b.stat[武将能力_政治]) / float(b.stat[武将能力_武力]));
            });

            info.member[0] = idle_person_list[0].get_id();
        }
        //===================================================支持函数==========================================================//
        int get_next_id(array<int> arr)
        {
            for (int i = 0; i < int(arr.length); ++i)
            {
                if (arr[i] == -1) return i;
            }
            return -1;
        }

        array<int> get_skill_by_route(array<int> route_type)
        {
            array<int> arr;
            if (route_type.find(路径_间道) > 0) arr.insertLast(特技_踏破);
            if (route_type.find(路径_毒泉) > 0) arr.insertLast(特技_解毒);
            if (route_type.find(路径_海洋) > 0)
            {
                arr.insertLast(特技_操舵);
                arr.insertLast(特技_推进);
                arr.insertLast(特技_水将);
            }
            return arr;
        }

        bool check_weapon(pk::list<pk::person@> list, int weapon_id)
        {
            if (weapon_id >= 兵器_冲车 and weapon_id <= 兵器_木兽)
            {
                if (get_best_heishu(list, weapon_id) < 适性_A) return true;
                return false;
            }
            return true;
        }

        int get_best_heishu(pk::list<pk::person@> list, int weapon_id)
        {
            if (weapon_id == 兵器_剑) return 0;
            int heishu = 0;
            if (调试模式 and list[0].get_id() == 武将_夏侯渊) pk::trace("weapon_id" + weapon_id + ",heishu_id" + pk::equipment_id_to_heishu(weapon_id));
            for (int i = 0; i < list.count; ++i)
            {
                if (list[i] !is null)
                {
                    if (调试模式 and list[0].get_id() == 武将_夏侯渊) pk::trace("weapon_id" + weapon_id + ",heishu_id" + pk::equipment_id_to_heishu(weapon_id) + ",tekisei" + list[i].tekisei[pk::equipment_id_to_heishu(weapon_id)]);
                    int t = list[i].tekisei[pk::equipment_id_to_heishu(weapon_id)];
                    if (t > heishu) heishu = t;
                }              
            }
           
            return heishu;
        }

        int get_best_heishu(pk::list<pk::person@> list)
        {
            int heishu = 0;
            for (int j = 1; j < 兵器_末; ++j)
            {
                for (int i = 0; i < list.count; ++i)
                {
                    if (list[i] !is null)
                    {
                        int t = list[i].tekisei[pk::equipment_id_to_heishu(j)];
                        if (t > heishu) heishu = t;
                    }
                }
            }

            return heishu;
        }

        pk::int_int_int get_dif(array<int> score, int end = -1)//
        {
            if (end == -1) end = int(score.length);
            int best_score = 0;
            int id = 0;
            //int second_score = 0;
            for (int i = 0; i < end; i++)
            {
                if (score[i] > best_score)
                {
                    best_score = score[i];
                    id = i;
                }
            }
            int dif = 255;
            for (int i = 0; i < end; i++)
            {
                if ((best_score - score[i]) < dif)
                {
                    dif = best_score - score[i];
                }
            }
            return pk::int_int_int(id, best_score, dif);
        }

        bool is_best_score(array<int> score, int best_score, int id)//
        {
            if (best_score == score[id]) return true;
            return false;
        }

        array<bool> get_array_with_dif(array<int> score, int best_score, int dif)//
        {
            array<bool> arr(兵器_末, false);
            for (int i = 0; i < 兵器_走舸; i++)
            {
                if ((best_score - score[i]) <= dif) arr[i] = true;
            }
            
            return arr;
        }

        int cal_best_weapon(array<int> heishu_score, array<bool> can_march, array<bool> check_dif, int pre_weapon, pk::list<pk::person@> list)
        {
            pk::force@ force0 = pk::get_force(list[0].get_force_id());
            array<int> tech_score(兵器_末, 0);//已研究科技评分
            array<int> all_skill_score(兵器_末, 0);//总特技评分
            array<int> max_skill_score(兵器_末, 0);//最佳特技评分
            array<int> all_score(兵器_末, 0);//兵器和适性的总评分
            //科技评分的获取，只有可出征且满足适性差的才有评分
            for (int i = 0; i < 兵器_走舸; i++)
            {
                if (can_march[i] and check_dif[i])
                {
                    tech_score[i] += get_tech_score(force0, i);
                    all_score[i] += tech_score[i] + heishu_score[i];
                    if (调试模式 and list[0].get_id() == 武将_夏侯渊) pk::trace("tech_score[i]:" + tech_score[i] + ",all_score:" + all_score[i] + ",heishu_score:" + heishu_score[i]);
                }
            }
            array<int> all_single_skill = get_all_single_skill(list);
            //特技评分的获取，只有可出征且满足适性差的才有评分
            if (int(all_single_skill.length) > 0)
            {
                for (int i = 0; i < int(all_single_skill.length); i++)
                {
                    int skill_id = all_single_skill[i];
                    if (skill_id != -1)
                    {
                        for (int j = 0; j < 兵器_走舸; j++)//跳过剑兵
                        {
                            if (can_march[j] and check_dif[j])
                            {
                                if (调试模式 and list[0].get_id() == 武将_夏侯渊) pk::trace("skill_id:"+skill_id+",weapon_id:"+j + ",score:"+ skill_score[skill_id][j]);
                                all_skill_score[j] += skill_score[skill_id][j];
                                all_score[j] += skill_score[skill_id][j];
                                if (skill_score[skill_id][j] > max_skill_score[j]) max_skill_score[j] = skill_score[skill_id][j];
                            }
                        }
                    }

                }
            }

            auto max_skill_dif = get_dif(max_skill_score, 兵器_走舸);//排除水军适性
            if (max_skill_dif.third > 1) return max_skill_dif.first;//最佳特技评分差>1	选最佳特技兵种
            auto all_dif = get_dif(all_score, 兵器_走舸);//排除水军适性
            if (调试模式) pk::trace(pk::decode(pk::get_name(list[0])) + "(id, best_score, dif)" + all_dif.first + all_dif.second + all_dif.third);//(id, best_score, dif)

            if (all_dif.third >= 1)
            {
                return all_dif.first;
            }
            else//差距为0
            {
                //攻城兵器有其他最优选择时 优先选择弩兵
                if (pre_weapon >= 兵器_冲车 and pre_weapon <= 兵器_木兽)
                {
                    if (is_best_score(all_score, all_dif.second, 兵器_弩)) return 兵器_弩;
                }
                return all_dif.first;
            }
        }

        int get_tech_score(pk::force@ force, int weapon_id)
        {
            if (force is null) return 0;
            switch (weapon_id)
            {
            case 兵器_剑: return 0;
            case 兵器_枪:
            {
                if (pk::has_tech(force, 技巧_精锐枪兵)) return 4;
                else if (pk::has_tech(force, 技巧_奇袭)) return 3;
                else if (pk::has_tech(force, 技巧_兵粮袭击)) return 2;
                else if (pk::has_tech(force, 技巧_枪兵锻练)) return 1;
                else return 0;
            }
            case 兵器_戟:
            {
                if (pk::has_tech(force, 技巧_精锐戟兵)) return 4;
                else if (pk::has_tech(force, 技巧_大盾)) return 3;
                else if (pk::has_tech(force, 技巧_矢盾)) return 2;
                else if (pk::has_tech(force, 技巧_戟兵锻练)) return 1;
                else return 0;
            }
            case 兵器_弩:
            {
                if (pk::has_tech(force, 技巧_精锐弩兵)) return 4;
                else if (pk::has_tech(force, 技巧_强弩)) return 3;
                else if (pk::has_tech(force, 技巧_应射)) return 2;
                else if (pk::has_tech(force, 技巧_弩兵锻练)) return 1;
                else return 0;
            }
            case 兵器_战马:
            {
                if (pk::has_tech(force, 技巧_精锐骑兵)) return 4;
                else if (pk::has_tech(force, 技巧_骑射)) return 3;
                else if (pk::has_tech(force, 技巧_良马产出)) return 2;
                else if (pk::has_tech(force, 技巧_骑兵锻练)) return 1;
                else return 0;
            }
            case 兵器_冲车: return 0;
            case 兵器_井阑: return 0;
            case 兵器_投石: 
            {
                if (pk::has_tech(force, 技巧_霹雳)) return 4;
                else return 0;
            }
            case 兵器_木兽: return 0;
            case 兵器_走舸: return 0;
            case 兵器_楼船: return 0;
            case 兵器_斗舰:
            {
                if (pk::has_tech(force, 技巧_霹雳)) return 4;
                else return 0;
            }
            default:
                return 0;
            }
            return 0;
        }

        bool CheckWeaponAmount(pk::building@ building, int weapon_id)
        {
            if (weapon_id == 兵器_剑 or weapon_id == 兵器_走舸) return true;
            if (weapon_id > 兵器_战马)
            {
                if (pk::get_weapon_amount(building, weapon_id) >= 1) return true;
            }
            else
            {
                if (pk::get_weapon_amount(building, weapon_id) >= 5000) return true;
            }
            return false;
        }

        array<int> get_all_single_skill(pk::person@ person)
        {
            array<int> arr;
            //单特技只到159
            for (int i = 0; i <= 159; i++)
            {
                if (ch::has_skill(person, i)) arr.insertLast(i);
            }
            return arr;
        }

        array<int> get_all_single_skill(pk::list<pk::person@> list)
        {
            array<int> arr;
            //单特技只到159
            for (int i = 0; i <= 159; i++)
            {
                if (ch::has_skill(list, i)) arr.insertLast(i);
            }
            return arr;
        }

        //确认是否需要变更
        bool check_leader_attr(pk::list<pk::person@> list,int weapon_id)
        {
            int stat_ref = int(武将变更_能力基准值 * (武将变更_统率权重 + 武将变更_武力权重 + 武将变更_智力权重));

            pk::person@ leader_t = list[0];

            // 비교에 사용할 지력값 변경 : 개인지력 → 부대지력 ('19.2.15)
            //int stat_chk = get_stat_sum(leader_t, true);
            int stat_chk = get_stat_sum(leader_t, false) + int(武将变更_智力权重 * ch::get_best_member_stat(list, 武将能力_智力));

            bool chk_tekisei = true;
            if (武将变更_启用适性标准)
            {
                //chk_tekisei = (leader_t.tekisei[weapon_heishu] > pk::max(0, pk::min(3, 武将变更_适性基准值))); //개인적성
                chk_tekisei = (get_tekisei(list,weapon_id) > pk::max(0, pk::min(3, 武将变更_适性基准值)));           //부대적성
            }
            if (stat_chk > stat_ref and chk_tekisei) return false;

            return true;
        }

        pk::list<pk::person@> get_member_list(pk::com_march_cmd_info& info)
        {
            pk::list<pk::person@> list;
            for (int i = 0; i < 3; ++i)
            {
                if (pk::is_valid_person_id(info.member[i]))
                {
                    pk::person@ person0 = pk::get_person(info.member[i]);
                    if (pk::is_alive(person0)) list.add(person0);
                }
            }
            return list;
        }

        // 武将特技权重换算무장 특기 가중치 환산
        int get_stat_skill(pk::person@ person_t, int weapon_heishu)
        {
            int stat = 0;
            if (武将变更_启用特技加权)
            {
                if (ch::has_skill(person_t, 特技_飞将)) stat = pk::max(5, stat);
                if (ch::has_skill(person_t, 特技_霸王)) stat = pk::max(5, stat);
                if (ch::has_skill(person_t, 特技_勇将)) stat = pk::max(3, stat);

                if (weapon_heishu == 兵种_枪兵)
                {
                    if (ch::has_skill(person_t, 特技_神将)) stat = pk::max(4, stat);
                    if (ch::has_skill(person_t, 特技_斗神)) stat = pk::max(4, stat);
                    if (ch::has_skill(person_t, 特技_枪神)) stat = pk::max(4, stat);
                    if (ch::has_skill(person_t, 特技_枪将)) stat = pk::max(3, stat);
                    if (ch::has_skill(person_t, 特技_猛者)) stat = pk::max(3, stat);
                }
                else if (weapon_heishu == 兵种_戟兵)
                {
                    if (ch::has_skill(person_t, 特技_神将)) stat = pk::max(4, stat);
                    if (ch::has_skill(person_t, 特技_斗神)) stat = pk::max(4, stat);
                    if (ch::has_skill(person_t, 特技_戟神)) stat = pk::max(4, stat);
                    if (ch::has_skill(person_t, 特技_戟将)) stat = pk::max(3, stat);
                    if (ch::has_skill(person_t, 特技_猛者)) stat = pk::max(3, stat);
                    if (ch::has_skill(person_t, 特技_威风)) stat = pk::max(3, stat);
                }
                else if (weapon_heishu == 兵种_弩兵)
                {
                    if (ch::has_skill(person_t, 特技_神将)) stat = pk::max(3, stat);
                    if (ch::has_skill(person_t, 特技_弓神)) stat = pk::max(5, stat);
                    if (ch::has_skill(person_t, 特技_弓将)) stat = pk::max(4, stat);
                    if (ch::has_skill(person_t, 特技_射手)) stat = pk::max(3, stat);
                    if (ch::has_skill(person_t, 特技_威风)) stat = pk::max(3, stat);
                }
                else if (weapon_heishu == 兵种_骑兵)
                {
                    if (ch::has_skill(person_t, 特技_神将)) stat = pk::max(3, stat);
                    if (ch::has_skill(person_t, 特技_骑神)) stat = pk::max(5, stat);
                    if (ch::has_skill(person_t, 特技_骑将)) stat = pk::max(4, stat);
                    if (ch::has_skill(person_t, 特技_白马)) stat = pk::max(4, stat);
                    if (ch::has_skill(person_t, 特技_疾驰)) stat = pk::max(4, stat);
                    if (ch::has_skill(person_t, 特技_猛者)) stat = pk::max(3, stat);
                }
                else if (weapon_heishu == 兵种_兵器)
                {
                    if (ch::has_skill(person_t, 特技_工神)) stat = pk::max(5, stat);
                    if (ch::has_skill(person_t, 特技_攻城)) stat = pk::max(4, stat);
                    if (ch::has_skill(person_t, 特技_射程)) stat = pk::max(4, stat);
                    if (ch::has_skill(person_t, 特技_奏乐)) stat = pk::max(2, stat);
                }
            }
            return stat;
        }


        // 무장 능력치 계산식
        int get_stat_sum(pk::person@ person_t, bool include_wisdom)
        {
            int stat = int(武将变更_统率权重 * person_t.stat[武将能力_统率]
                + 武将变更_武力权重 * person_t.stat[武将能力_武力]);

            if (include_wisdom)
                stat += int(武将变更_智力权重 * person_t.stat[武将能力_智力]);

            return stat;
        }

        // 무장 적성 능력가중치 환산
        int get_stat_tekisei(pk::person@ person_t, int weapon_heishu)
        {
            int stat = 0;
            int tekisei = person_t.tekisei[weapon_heishu];
            if (武将变更_启用适性标准)
            {
                int stat_base = (tekisei > pk::max(0, pk::min(3, 武将变更_适性基准值))) ? 30 : 0;
                stat = int(武将变更_适性权重 * (stat_base + 15 * pk::min(3, tekisei)));
            }
            return stat;
        }

        int get_stat_member_tekisei(pk::list<pk::person@> list, int weapon_id)
        {
            int stat = 0;
            int best_tekisei = get_tekisei(list,weapon_id);

            if (武将变更_启用适性标准)
            {
                int stat_base = (best_tekisei > pk::max(0, pk::min(3, 武将变更_适性基准值))) ? 30 : 0;
                stat = int(武将变更_适性权重 * (stat_base + 15 * pk::min(3, best_tekisei)));
            }
            return stat;
        }

        int get_tekisei(pk::list<pk::person@> list, int weapon_id)
        {
            int heishu_id = pk::equipment_id_to_heishu(weapon_id);
            int best_tekisei = 0;
            for (int i = 0; i < list.count; ++i)
            {
                if (list[i] !is null)
                {
                    if (list[i].tekisei[heishu_id] > best_tekisei) best_tekisei = list[i].tekisei[heishu_id];
                }
            }
            return best_tekisei;
        }

        array<array<int>> skill_score = {
            /*000飞将*/{/*剑*/0,/*枪*/4,/*戟*/4,/*弩*/4,/*马*/5,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*001遁走*/{/*剑*/0,/*枪*/1,/*戟*/2,/*弩*/1,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*002强行*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/2,/*马*/2,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*003长驱*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*004推进*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/2,/*楼船*/2,/*斗舰*/2},
            /*005操舵*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/2,/*楼船*/2,/*斗舰*/2},
            /*006踏破*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*007运输*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*008用毒*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/2,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*009扫讨*/{/*剑*/0,/*枪*/1,/*戟*/2,/*弩*/2,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/2,/*木兽*/0,/*走舸*/0,/*楼船*/1,/*斗舰*/2},
            /*010威风*/{/*剑*/0,/*枪*/2,/*戟*/3,/*弩*/3,/*马*/2,/*冲车*/0,/*井阑*/0,/*投石*/3,/*木兽*/0,/*走舸*/0,/*楼船*/2,/*斗舰*/3},
            /*011昂扬*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/2,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/1},
            /*012连战*/{/*剑*/0,/*枪*/3,/*戟*/3,/*弩*/3,/*马*/3,/*冲车*/0,/*井阑*/3,/*投石*/3,/*木兽*/3,/*走舸*/0,/*楼船*/3,/*斗舰*/3},
            /*013急袭*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*014强袭*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/1,/*楼船*/1,/*斗舰*/1},
            /*015乱战*/{/*剑*/0,/*枪*/2,/*戟*/1,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*016待伏*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*017攻城*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/2,/*马*/0,/*冲车*/3,/*井阑*/4,/*投石*/5,/*木兽*/4,/*走舸*/0,/*楼船*/0,/*斗舰*/1},
            /*018掎角*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*019擒拿*/{/*剑*/0,/*枪*/1,/*戟*/2,/*弩*/1,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*020精妙*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*021强夺*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*022攻心*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/0,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/1,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*023驱逐*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/0,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*024射程*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/3,/*投石*/3,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*025骑射*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/2,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*026辅佐*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/1,/*马*/0,/*冲车*/0,/*井阑*/1,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*027不屈*/{/*剑*/0,/*枪*/0,/*戟*/1,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*028金刚*/{/*剑*/0,/*枪*/0,/*戟*/2,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*029铁壁*/{/*剑*/0,/*枪*/0,/*戟*/1,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*030怒发*/{/*剑*/0,/*枪*/0,/*戟*/1,/*弩*/1,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*031藤甲*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*032幸运*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*033血路*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*034枪将*/{/*剑*/0,/*枪*/4,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*035戟将*/{/*剑*/0,/*枪*/0,/*戟*/4,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*036弓将*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/4,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*037骑将*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/4,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*038水将*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/3,/*楼船*/4,/*斗舰*/4},
            /*039勇将*/{/*剑*/0,/*枪*/4,/*戟*/4,/*弩*/4,/*马*/4,/*冲车*/4,/*井阑*/4,/*投石*/4,/*木兽*/4,/*走舸*/3,/*楼船*/4,/*斗舰*/4},
            /*040神将*/{/*剑*/0,/*枪*/4,/*戟*/4,/*弩*/4,/*马*/4,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*041斗神*/{/*剑*/0,/*枪*/5,/*戟*/5,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*042枪神*/{/*剑*/0,/*枪*/6,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*043戟神*/{/*剑*/0,/*枪*/0,/*戟*/6,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*044弓神*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/6,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*045骑神*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/6,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*046工神*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/4,/*井阑*/5,/*投石*/5,/*木兽*/4,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*047水神*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/4,/*楼船*/6,/*斗舰*/6},
            /*048霸王*/{/*剑*/0,/*枪*/6,/*戟*/6,/*弩*/5,/*马*/6,/*冲车*/5,/*井阑*/5,/*投石*/5,/*木兽*/5,/*走舸*/5,/*楼船*/5,/*斗舰*/5},
            /*049疾驰*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/3,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*050射手*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/2,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*051猛者*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/0,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*052护卫*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*053火攻*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/1,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/0},
            /*054言毒*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*055机略*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*056诡计*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*057虚实*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*058妙计*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*059秘计*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*060看破*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*061洞察*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*062火神*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/2,/*马*/0,/*冲车*/0,/*井阑*/1,/*投石*/0,/*木兽*/2,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*063神算*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*064百出*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*065鬼谋*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*066连环*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*067深谋*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*068反计*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*069奇谋*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*070妖术*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*071鬼门*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*072规律*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*073沉着*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*074明镜*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*075奏乐*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/2,/*马*/1,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/1,/*楼船*/1,/*斗舰*/1},
            /*076诗想*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/1,/*马*/0,/*冲车*/0,/*井阑*/1,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*077筑城*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*078屯田*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*079名声*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*080能吏*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*081繁殖*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*082发明*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*083造船*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*084指导*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*085眼力*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*086论客*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*087积蓄*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*088劝农*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*089度支*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*090平准*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*091亲乌*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/3,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*092亲羌*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/3,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*093亲越*/{/*剑*/0,/*枪*/0,/*戟*/3,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*094亲蛮*/{/*剑*/0,/*枪*/0,/*戟*/3,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*095威压*/{/*剑*/0,/*枪*/3,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*096仁政*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*097风水*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*098祈愿*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*099内助*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*100八卦*/{/*剑*/0,/*枪*/2,/*戟*/3,/*弩*/2,/*马*/2,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*101窥探*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*102内治*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*103戍防*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*104韬略*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*105谋略*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*106军略*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*107强募*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*108狙击*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/3,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*109猛骑*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/3,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*110猛卒*/{/*剑*/0,/*枪*/3,/*戟*/3,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*111严整*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*112辎粮*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*113山战*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/3,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*114背水*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/2,/*马*/2,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*115清野*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*116拱戍*/{/*剑*/0,/*枪*/2,/*戟*/3,/*弩*/2,/*马*/2,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*117布阵*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/2,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/1,/*楼船*/1,/*斗舰*/1},
            /*118要击*/{/*剑*/0,/*枪*/1,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*119商才*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*120兵圣*/{/*剑*/0,/*枪*/3,/*戟*/4,/*弩*/3,/*马*/3,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*121兵神*/{/*剑*/0,/*枪*/0,/*戟*/1,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*122兵心*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/2,/*马*/2,/*冲车*/2,/*井阑*/2,/*投石*/2,/*木兽*/2,/*走舸*/2,/*楼船*/2,/*斗舰*/2},
            /*123战神*/{/*剑*/0,/*枪*/4,/*戟*/4,/*弩*/4,/*马*/4,/*冲车*/0,/*井阑*/2,/*投石*/2,/*木兽*/1,/*走舸*/0,/*楼船*/2,/*斗舰*/2},
            /*124战将*/{/*剑*/0,/*枪*/4,/*戟*/4,/*弩*/4,/*马*/4,/*冲车*/0,/*井阑*/2,/*投石*/2,/*木兽*/1,/*走舸*/0,/*楼船*/2,/*斗舰*/2},
            /*125巧变*/{/*剑*/0,/*枪*/2,/*戟*/1,/*弩*/2,/*马*/2,/*冲车*/0,/*井阑*/1,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*126游侠*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*127百战*/{/*剑*/0,/*枪*/3,/*戟*/4,/*弩*/4,/*马*/3,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/2},
            /*128蓄势*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/2,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*129神臂*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/3,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*130治军*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/2,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*131坚城*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*132料敌*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/1,/*楼船*/1,/*斗舰*/1},
            /*133陷阵*/{/*剑*/0,/*枪*/2,/*戟*/1,/*弩*/2,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*134恫吓*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/1,/*马*/2,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*135破竹*/{/*剑*/0,/*枪*/2,/*戟*/4,/*弩*/4,/*马*/3,/*冲车*/0,/*井阑*/1,/*投石*/4,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/4},
            /*136安民*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/2,/*井阑*/2,/*投石*/2,/*木兽*/2,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*137循吏*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*138贤哲*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*139变法*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*140法治*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*141后勤*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*142医者*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*143扰敌*/{/*剑*/0,/*枪*/0,/*戟*/1,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*144运筹*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*145激励*/{/*剑*/0,/*枪*/3,/*戟*/3,/*弩*/3,/*马*/3,/*冲车*/0,/*井阑*/2,/*投石*/3,/*木兽*/2,/*走舸*/0,/*楼船*/2,/*斗舰*/3},
            /*146伯乐*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*147清谈*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*148军魂*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/2,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/2},
            /*149武干*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/2,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/2},
            /*150摧锋*/{/*剑*/0,/*枪*/4,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*151重甲*/{/*剑*/0,/*枪*/0,/*戟*/4,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*152轻甲*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/3,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*153掠阵*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/4,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*154重器*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/3,/*投石*/3,/*木兽*/3,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*155水师*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*156调练*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*157巡查*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*158劫财*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*159截粮*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*160*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*161*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*162*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*163*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*164*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*165*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*166*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*167*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*168*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*169*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*170*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*171*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*172*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*173*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*174*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*175*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*176*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*177*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*178*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*179*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*180飞军*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/3,/*马*/3,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*181闪击*/{/*剑*/0,/*枪*/1,/*戟*/2,/*弩*/1,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*182步将*/{/*剑*/0,/*枪*/3,/*戟*/3,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*183骁将*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/3,/*马*/3,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*184骁神*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/4,/*马*/4,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*185毒师*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/3,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/1,/*楼船*/1,/*斗舰*/0},
            /*186骑驱*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/4,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*187卫戍*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*188剽掠*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*189义从*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*190天骄*/{/*剑*/0,/*枪*/4,/*戟*/4,/*弩*/4,/*马*/4,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/4,/*斗舰*/4},
            /*191谋圣*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/2,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/2},
            /*192才媛*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/2,/*马*/1,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*193仙君*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*194先师*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*195圣王*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*196王佐*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*197和戍*/{/*剑*/0,/*枪*/0,/*戟*/3,/*弩*/0,/*马*/3,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*198圣人*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*199统军*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/2,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/2},
            /*200魏武*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/2,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*201仁德*/{/*剑*/0,/*枪*/1,/*戟*/2,/*弩*/1,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*202吴侯*/{/*剑*/0,/*枪*/2,/*戟*/3,/*弩*/2,/*马*/2,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*203黄天*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*204暴君*/{/*剑*/0,/*枪*/1,/*戟*/2,/*弩*/2,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/2,/*木兽*/0,/*走舸*/0,/*楼船*/1,/*斗舰*/2},
            /*205名门*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*206白马*/{/*剑*/0,/*枪*/2,/*戟*/4,/*弩*/4,/*马*/5,/*冲车*/0,/*井阑*/1,/*投石*/4,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/4},
            /*207蛮王*/{/*剑*/0,/*枪*/1,/*戟*/4,/*弩*/1,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*208武冠*/{/*剑*/0,/*枪*/6,/*戟*/4,/*弩*/4,/*马*/4,/*冲车*/4,/*井阑*/4,/*投石*/4,/*木兽*/4,/*走舸*/4,/*楼船*/4,/*斗舰*/4},
            /*209无双*/{/*剑*/0,/*枪*/4,/*戟*/4,/*弩*/4,/*马*/6,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*210武圣*/{/*剑*/0,/*枪*/4,/*戟*/6,/*弩*/4,/*马*/4,/*冲车*/0,/*井阑*/2,/*投石*/2,/*木兽*/1,/*走舸*/0,/*楼船*/2,/*斗舰*/2},
            /*211咆哮*/{/*剑*/0,/*枪*/2,/*戟*/1,/*弩*/2,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*212龙骑*/{/*剑*/0,/*枪*/5,/*戟*/5,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*213固守*/{/*剑*/0,/*枪*/2,/*戟*/3,/*弩*/2,/*马*/2,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/1,/*斗舰*/1},
            /*214奇袭*/{/*剑*/0,/*枪*/2,/*戟*/3,/*弩*/3,/*马*/2,/*冲车*/0,/*井阑*/0,/*投石*/3,/*木兽*/0,/*走舸*/0,/*楼船*/2,/*斗舰*/3},
            /*215虎痴*/{/*剑*/0,/*枪*/4,/*戟*/4,/*弩*/4,/*马*/4,/*冲车*/0,/*井阑*/3,/*投石*/3,/*木兽*/3,/*走舸*/0,/*楼船*/3,/*斗舰*/3},
            /*216恶来*/{/*剑*/0,/*枪*/0,/*戟*/4,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*217啖睛*/{/*剑*/0,/*枪*/3,/*戟*/4,/*弩*/4,/*马*/5,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/0,/*楼船*/1,/*斗舰*/2},
            /*218稳固*/{/*剑*/0,/*枪*/1,/*戟*/2,/*弩*/1,/*马*/2,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*219白衣*/{/*剑*/0,/*枪*/2,/*戟*/2,/*弩*/0,/*马*/2,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/1,/*走舸*/0,/*楼船*/2,/*斗舰*/2},
            /*220儒帅*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/4,/*马*/0,/*冲车*/0,/*井阑*/2,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*221锦帆*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/3,/*楼船*/5,/*斗舰*/5},
            /*222冢虎*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/1,/*井阑*/1,/*投石*/1,/*木兽*/1,/*走舸*/1,/*楼船*/1,/*斗舰*/1},
            /*223卧龙*/{/*剑*/0,/*枪*/2,/*戟*/3,/*弩*/2,/*马*/2,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*224周郎*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/2,/*马*/0,/*冲车*/0,/*井阑*/1,/*投石*/0,/*木兽*/2,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*225天妒*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*226斗智*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*227凤雏*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*228令君*/{/*剑*/0,/*枪*/40,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*229门神*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*230破虏*/{/*剑*/0,/*枪*/1,/*戟*/1,/*弩*/1,/*马*/1,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/1,/*楼船*/1,/*斗舰*/1},
            /*231*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*232*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*233*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*234*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*235*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*236*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*237*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*238*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*239*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*240*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*241*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*242*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*243*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*244*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*245*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*246*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*247*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*248*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0},
            /*249*/{/*剑*/0,/*枪*/0,/*戟*/0,/*弩*/0,/*马*/0,/*冲车*/0,/*井阑*/0,/*投石*/0,/*木兽*/0,/*走舸*/0,/*楼船*/0,/*斗舰*/0}
        };
    }
    Main main;

    void modify_normal_deputy(pk::com_march_cmd_info& info, pk::list<pk::person@> member_list, int dst_base_id = -1)
    {
        if (member_list.count > 2) return;//已有三名副将直接不优化---相信电脑的优化
        auto list = pk::get_idle_person_list(info.base);
        if (list.count < 7) return; //武将数小于7，直接单队出征不优化副将
        int base_id = info.base.get_id();

        pk::person@ leader = pk::get_person(info.member[0]);
        uint8 best_wisdom = 0;
        uint8 best_strength = 0;
        array<int> member_arr(3, -1);
        for (int i = 0; i < member_list.count; ++i)
        {
            member_arr[i] = member_list[i].get_id();
            if (member_list[i].stat[武将能力_智力] > best_wisdom) best_wisdom = member_list[i].stat[武将能力_智力];
            if (member_list[i].stat[武将能力_武力] > best_strength) best_strength = member_list[i].stat[武将能力_武力];
        }

        int need_type = -1;
        if (best_wisdom > 75)
        {
            if (best_strength < 60)
                need_type = 武将能力_武力;
        }
        else need_type = 武将能力_智力;
        array<int> skill_arr;
        if (dst_base_id != -1)
        {
            //根据到达目标据点的经过的路径类型选择副将
            array<int> route_type = get_target_base_route_type(base_id, dst_base_id);
            skill_arr = main.get_skill_by_route(route_type);
        }
        int next_id = main.get_next_id(member_arr);
        for (int i = 0; i < list.count; ++i)
        {
            if (next_id > 3 or next_id == -1) break;

            if (member_arr.find(list[i].get_id()) > 0) continue;//排除已有武将

            if (pk::rand_bool(70))//70概率优化能力，30优化特技
            {
                if (need_type > -1 and need_type < 武将能力_末)
                {
                    if (need_type == 武将能力_智力)
                    {
                        if (list[i].stat[need_type] > (best_wisdom + 10) and list[i].stat[武将能力_统率] < 70 and list[i].stat[武将能力_智力] > 70)
                        {
                            info.member[next_id] = list[i].get_id();
                            member_arr[next_id] = list[i].get_id();
                            next_id = main.get_next_id(member_arr);
                            need_type = -1;
                        }
                    }
                    else if (need_type == 武将能力_武力)
                    {
                        if (list[i].stat[need_type] > (best_strength + 20) and list[i].stat[武将能力_智力] < 70)
                        {
                            info.member[next_id] = list[i].get_id();
                            member_arr[next_id] = list[i].get_id();
                            next_id = main.get_next_id(member_arr);
                            need_type = -1;
                        }
                    }
                }

            }
            else
            {
                if (skill_arr.length > 0)
                {
                    for (int j = 0; j < int(skill_arr.length); ++j)
                    {
                        if (ch::has_skill(list[i], skill_arr[j]))
                        {
                            info.member[next_id] = list[i].get_id();
                            member_arr[next_id] = list[i].get_id();
                            next_id = main.get_next_id(member_arr);
                            break;
                        }
                    }
                }
            }

        }

    }

    void modify_food(pk::com_march_cmd_info& info, int src_id, int dst_id)
    {
        int days = pk::get_unit_distance(src_id, dst_id, info.weapon_id[0]);
        if (days > 70)
        {
            int ex_days = days - 70;
            float multi = (ex_days / 200.f * 2.f);
            int food_inc = int((multi + 2.f) * info.troops) - info.food;
            if (food_inc < 0) food_inc = 0;
            info.food += food_inc;
            if (调试模式) pk::trace("预计天数：" + days + ",额外天数：" + ex_days + "额外兵粮:" + food_inc + ",最终兵粮：" + info.food);
        }
    }

    //只变更陆军
    int get_best_weapon(pk::list<pk::person@> list, int pre_weapon, pk::building@ base, int type)
    {
        //先获取所有已有特技，然后根据特技获得适合兵种的评分
        //再加入适性影响
        //副将补充适性和智力？根据需要出征兵力和部队数决定多少副将？
        pk::person@ person = @list[0];

        array<int> heishu_score(兵器_末, 0);//适性评分，是否可以参考副将补全
        array<int> tech_score(兵器_末, 0);//已研究科技评分
        array<bool> can_march(兵器_末, false);//是否有足够兵器可出征
        array<int> all_skill_score(兵器_末, 0);//总评分
        array<int> max_skill_score(兵器_末, 0);//最佳评分
        //确认是否可出征对应兵器，如果对应兵器不可出征，评分直接为0
        pk::force@ force0 = pk::get_force(person.get_force_id());
        for (int i = 0; i < 兵器_走舸; i++)
        {
            if (main.CheckWeaponAmount(base, i)) can_march[i] = true;
            if (i > 4 and i < 9)
            {
                //攻城兵器不在防御时出动
                if (type == 0) can_march[i] = false;
            }
            if (can_march[i]) heishu_score[i] += main.get_best_heishu(list, i);
            if (调试模式 and person.get_id() == 武将_夏侯渊) pk::trace("weapon_id" + i + "heishu_score" + heishu_score[i]);
            //all_skill_score[i] += heishu_score;
        }

        //pk::trace("pos1");
        //适性例外情况，不做特技判断
        int pre_heishu = main.get_best_heishu(list, pre_weapon);
        auto heishu_dif = main.get_dif(heishu_score, 兵器_走舸);//排除水军适性
        if (heishu_dif.second <= 适性_S)
        {
            if (调试模式) pk::trace(pk::decode(pk::get_name(person)) + "适性_S(id, best_score, dif)" + heishu_dif.first + heishu_dif.second + heishu_dif.third);//(id, best_score, dif)
            if (heishu_dif.third > 0 and heishu_dif.first != 0 and can_march[heishu_dif.first]) return heishu_dif.first;//当适性小于s时，优先选择最高适性
            if (heishu_dif.third == 0)
            {
                array<bool> check_dif = main.get_array_with_dif(heishu_score, heishu_dif.second, 1);
                if (!check_dif[pre_weapon])//出征兵器非最佳适性
                {
                    return main.cal_best_weapon(heishu_score, can_march, check_dif, pre_weapon, list);
                }
            }
        }
        else//适性大于s的情况
        {
            if (调试模式) pk::trace(pk::decode(pk::get_name(person)) + "(id, best_score, dif)" + heishu_dif.first + heishu_dif.second + heishu_dif.third);//(id, best_score, dif)
            if (heishu_dif.third > 1) return heishu_dif.first;//当适性大于s时，第一第二适性差超过1级（如s和s2），也优先选择高适性
            else
            {
                //筛选适性差<=1的兵种
                array<bool> check_dif = main.get_array_with_dif(heishu_score, heishu_dif.second, 1);

                return main.cal_best_weapon(heishu_score, can_march, check_dif, pre_weapon, list);
            }

        }

        return pre_weapon;
    }

    //获取目标据点的经过连接点类型数组
    array<int> get_target_base_route_type(int src, int dst)
    {
        array<int> arr;
        pk::int_int temp;
        int route_type = -1;
        int i = 30;//设置上限，防止死循环
        while (src != dst and i > 0)
        {
            temp = get_next_base_id(src, dst);
            route_type = temp.second;
            src = temp.first;
            if (arr.find(route_type) < 0) arr.insertLast(route_type);
            --i;
        }
        return arr;
    }

    pk::int_int get_next_base_id(int src, int dst)
    {
        pk::building@ building0 = pk::get_building(src);
        int min_distance = 999;
        int temp_id = -1;
        int route_type = -1;
        for (int i = 0; i < 6; i++)
        {
            int neighbor_base_id = pk::get_neighbor_area_building(src, i);
            if (neighbor_base_id >= 0 && neighbor_base_id <= 据点_末)
            {
                if (neighbor_base_id == dst) return pk::int_int(dst, pk::get_neighbor_area_route(src, i));
                int base_distance = pk::get_building_distance(neighbor_base_id, dst);
                if (base_distance < min_distance)
                {
                    temp_id = neighbor_base_id;
                    min_distance = base_distance;
                    route_type = pk::get_neighbor_area_route(src, i);
                }

            }
        }
        if (调试模式 and pk::get_building_distance(temp_id, dst) >= pk::get_building_distance(src, dst))
        {
            string t = "temp_id:" + pk::get_new_base_name(temp_id) + pk::get_building_distance(temp_id, dst) + ",src:" + pk::get_new_base_name(src) + pk::get_building_distance(src, dst) + ",dst:" + pk::get_new_base_name(dst);
            pk::trace(t);
        }
        return pk::int_int(temp_id, route_type);
    }

}
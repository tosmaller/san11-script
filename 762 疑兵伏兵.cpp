// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/01/25 # 江东新风 # 加入难度倍率修正 ##
// ## 2021/11/04 # 江东新风 # 独立文件 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/10/14 # 江东新风 #复刻码神的疑兵伏兵系统，加入踩地格触发伏兵,加入突袭运输队##


namespace 疑兵伏兵
{
    // ================ CUSTOMIZE ================
    const int 运输队袭击发生率 = 3;
    const array<int>难度倍率修正 = { /*初级难度*/5, /*上级难度*/3,/*超级难度*/1 };
    // ===========================================

    //---------------------下方变量不支持改动---------------------------------------------------
    bool 触发伏兵 = false;
    pk::unit@ f_unit;
    pk::person@ f_person;
    pk::person@ f_deputy1;
    pk::person@ f_deputy2;
    int f_skill_1 = -1;
    int f_skill_2 = -1;
    int f_skill_3 = -1;
    int f_food = 0;
    int f_gold = 0;
    int f_troops = 0;
    bool 触发地格伏兵 = false;
    //------------------------------------------------------------------------------------------
    bool 调试模式 = false;
    class Main
    {
        Main()
        {
            pk::bind(177, pk::trigger177_t(unit_order_complete));//玩家命令结束，包括委任命令
            pk::bind(174, pk::trigger174_t(unit_action_done));
            pk::bind(170, pk::trigger170_t(unit_create));
           
        }

        void unit_create(pk::unit@ unit, int type)
        {
            unit_ex[unit.get_id()].has_contact = false;
        }
        //==================================突袭运输部队================================================//

        void unit_action_done(pk::unit@ unit,int type)
        {
            if (开启疑兵伏兵)
            {
                if (unit.type == 部队类型_运输)
                {
                    //1）AI和玩家非同盟/停战关系
                    //2）运输队周边3格内没有玩家的战斗部队，或者有战斗部队但总兵力小于3000
                    //3）运输队周边3格内没有同属AI势力的战斗部队
                    if (pk::is_player_controlled(unit))
                    {
                        //pk::trace("209 0");
                        pk::city@ dst_city = pk::get_city(pk::get_city_id(unit.pos));
                        if (pk::is_enemy(dst_city, unit))//非敌方势力不触发伏兵
                        {
                            //pk::trace("209 1");
                            
                            if (pk::get_troops(dst_city) > 18000)
                            {
                                if (!pk::has_member(unit, 武将_诸葛亮) and !pk::has_member(unit, 武将_张良))
                                {
                                    //int dst_force_id = dst_city.get_force_id();
                                    if (!check_around(unit))
                                    {
                                        //pk::trace("209 2");
                                        if (pk::rand_bool(运输队袭击发生率))//此处调成100可百分百触发地格伏兵
                                        {
                                            //pk::trace("209 3");
                                            //触发运输伏兵 = true;
                                            pk::list<pk::person@> fubing_person = get_fubing_person(dst_city.get_force_id(), dst_city.get_id(), 1, -1, 兵种_骑兵);
                                            if (fubing_person.count > 0) func_transport_fubing_event(unit, fubing_person);
                                        }
                                    }
                                }
                            }

                        }
                    }
                }
            }

        }

        void unit_order_complete(pk::unit@ unit)
        {
            if (开启疑兵伏兵)
            {
                if (!触发伏兵)//
                {
                    if (pk::is_player_controlled(unit) and unit.type == 部队类型_战斗)
                    {
                        pk::city@ dst_city = pk::get_city(pk::get_city_id(unit.pos));
                        if (pk::is_enemy(dst_city, unit))//非敌方势力不触发伏兵
                        {
                            if (pk::get_troops(dst_city) > 18000)
                            {
                                if (!pk::has_member(unit, 武将_诸葛亮) and !pk::has_member(unit, 武将_张良))
                                {
                                    if (!is_near_building(unit))
                                    {
                                        int rate = cal_src_yibing_rate(unit, dst_city) / 难度倍率修正[pk::get_scenario().difficulty];;
                                        if (pk::rand_bool(rate))//此处调成100可百分百触发地格伏兵
                                        {
                                            触发地格伏兵 = true;
                                            pk::list<pk::person@> fubing_person = get_fubing_person(dst_city.get_force_id(), dst_city.get_id(), 2);
                                            if (fubing_person.count > 0) func_hex_fubing_event(unit, fubing_person);
                                        }
                                    }

                                }
                            }
                        }

                    }

                }
                if (调试模式) pk::trace("触发伏兵,personid,地格" + 触发伏兵+ f_person.get_id()+ 触发地格伏兵);
                if (触发伏兵 and f_person !is null and !触发地格伏兵)//同时触发地格伏兵时，会出现一击打死对面，而未触发疑兵
                {
                    触发地格伏兵 = false;
                    触发伏兵 = false;//因为执行过程中部队可能死亡而反复触发伏兵，所以执行前初始化
                    int food = f_food;
                    int gold = f_gold;
                    int troops = f_troops;
                    f_food = 0;
                    f_gold = 0;
                    f_troops = 0;
                    pk::core::remove_hidden_skill(f_person.get_id(), 特技_强运);
                    //f_person.skill = f_skill_1;
                    if (f_deputy1 !is null)  pk::core::remove_hidden_skill(f_deputy1.get_id(), 特技_强运); //f_deputy1.skill = f_skill_2;
                    if (f_deputy2 !is null)  pk::core::remove_hidden_skill(f_deputy2.get_id(), 特技_强运); //f_deputy2.skill = f_skill_3;
                    func_yibing_event(unit, f_person, f_deputy1, f_deputy2, food, gold, troops);
                    @f_person = null;
                    @f_deputy1 = null;
                    @f_deputy2 = null;
                    f_skill_1 = -1;
                    f_skill_2 = -1;
                    f_skill_3 = -1;
                }
                触发伏兵 = false;
                @f_person = null;
                f_food = 0;
                f_gold = 0;
                f_troops = 0;
                @f_person = null;
                @f_deputy1 = null;
                @f_deputy2 = null;
                f_skill_1 = -1;
                f_skill_2 = -1;
                f_skill_3 = -1;
            }

        }
        //==================================================疑兵支持函数=======================================//
        bool check_around(pk::unit@ unit)
        {
            array<pk::point> range_pos_arr = pk::range(unit.pos, 1, 3);
            for (int arr_index = 0; arr_index < int(range_pos_arr.length); arr_index++)
            {
                pk::point range_pos = range_pos_arr[arr_index];
                if (!pk::is_valid_pos(range_pos)) continue;

                pk::hex@ hex = pk::get_hex(range_pos);
                if (hex.has_unit or hex.has_building)
                {
                    if (hex.has_unit)
                    {
                        pk::unit@ dst_unit = pk::get_unit(range_pos);
                        if (dst_unit.get_force_id() == unit.get_force_id())
                        {
                            if (dst_unit.type == 部队类型_战斗 and dst_unit.troops >= 3000) return true;
                        }
                        //if (dst_unit.get_force_id() == dst_force_id)
                        //{
                        //    if (dst_unit.type == 部队类型_战斗) return true;
                        //}
                    }
                    else if (hex.has_building)
                    {
                        pk::building@ building0 = pk::get_building(range_pos);

                        if ((building0.facility == 设施_港口 or building0.facility == 设施_关卡) and building0.get_force_id() == unit.get_force_id())
                        {
                            if (pk::get_distance(unit.pos, range_pos) <= 3) return true;
                        }
                        if ((building0.facility == 设施_阵 or building0.facility == 设施_砦 or building0.facility == 设施_城塞 or building0.facility == 设施_箭楼 or building0.facility == 设施_连弩楼 or building0.facility == 设施_投石台) and building0.get_force_id() == unit.get_force_id())
                        {
                            if (pk::get_distance(unit.pos, range_pos) <= 2) return true;
                        }
                    }
                }

            }

            return false;
        }


        void func_transport_fubing_event(pk::unit@attacker, pk::list<pk::person@> fubing_person, int troops = 8000, int gold = 1000, int food = 16000)
        {
            //加点音效。另外此处部队没有完全灭亡该怎么生成？
            pk::message_box(pk::encode("兄弟们，把物资全抢过来！"), fubing_person[0]);
            pk::message_box(pk::encode("没想到居然被伏击了！"), pk::get_person(attacker.leader));
            pk::person@ f_person = fubing_person[0];
            //pk::set_status(attacker, null, 部队状态_混乱, 2);
            func_伏兵部队_生成(pk::get_building(f_person.service), fubing_person, attacker, troops, 兵器_战马, gold, food, 1, 2, false);
            //func_create_unit(pk::get_building(f_person.service), f_person, src_pos, 伏兵部队数, 8000, target_unit.gold, target_unit.food);
            return;
        }

        void func_hex_fubing_event(pk::unit@attacker, pk::list<pk::person@> fubing_person, int troops = 8000, int gold = 1000, int food = 16000)
        {
            //加点音效。另外此处部队没有完全灭亡该怎么生成？
            pk::message_box(pk::encode("哈哈哈，没想到吧！"), fubing_person[0]);
            pk::message_box(pk::encode("没想到此处竟有敌人埋伏！"), pk::get_person(attacker.leader));
            pk::person@ f_person = fubing_person[0];
            pk::set_status(attacker, null, 部队状态_混乱, 2);
            func_伏兵部队_生成(pk::get_building(f_person.service), fubing_person, attacker, troops, -1, gold, food, 2,1, true);
            //func_create_unit(pk::get_building(f_person.service), f_person, src_pos, 伏兵部队数, 8000, target_unit.gold, target_unit.food);
            return;
        }

        void func_yibing_event(pk::unit@attacker, pk::person@ target_person, pk::person@ second_person = null, pk::person@ third_person = null, int troops = 8000, int gold = 1000, int food = 16000)
        {
            //需要传入部队数，金，粮，部队灭亡时，各种数据都清空了
            //加点音效。另外此处部队没有完全灭亡该怎么生成？
            pk::message_box(pk::encode("你中计了！"), target_person);
            if (pk::is_kunshu(pk::get_person(attacker.leader))) pk::message_box(pk::encode("一时大意，竟中了敌人的奸计，回去该如何向将士们交代！"), pk::get_person(attacker.leader));
            else pk::message_box(pk::encode("一时大意，竟中了敌人的奸计，回去该如何向主公交代！"), pk::get_person(attacker.leader));
            pk::person@ f_person = target_person;
            pk::set_status(attacker, null, 部队状态_混乱, 2);
            pk::list<pk::person@> fubing_person = get_fubing_person(f_person.get_force_id(), f_person.service, 伏兵部队数 - 1, f_person.get_id());
            func_伏兵部队_生成(pk::get_building(f_person.service), fubing_person, attacker, troops, -1, gold, food, 伏兵部队数, 0, true, f_deputy1, f_deputy2);
            //func_create_unit(pk::get_building(f_person.service), f_person, src_pos, 伏兵部队数, 8000, target_unit.gold, target_unit.food);
            return;
        }

        void func_伏兵部队_生成(pk::building@ building, pk::list<pk::person@> fubing_person, pk::unit@attacker, int troops, int weapon, int gold, int food, int num,int type, bool tactic = false, pk::person@ second_person = null, pk::person@ third_person = null)
        {
            if (!pk::is_alive(fubing_person[0])) return;

            for (int count = 0; count < num; count++)
            {
                int src_troops = troops;// get_best_troops(troops, num - count);
                pk::point src_pos = get_empty_pos(attacker.pos, 1, 1);

                pk::unit@ src_unit;
                if (count == 0)
                {
                    //AI_出征兵种优化::开启 = false;//改造出征优化后，无需此设定
                    if (调试模式) pk::trace(pk::format("weapon{}", weapon));
                    @src_unit = func_create_unit_1(type, building, fubing_person[0], src_pos, num, troops, weapon, gold, food, second_person, third_person);
                    //AI_出征兵种优化::开启 = true;//改造出征优化后，无需此设定
                    fubing_person.remove_at(0);
                }
                else
                {
                    if (fubing_person.count != 0)
                    {
                        //兵力还得重新算
                        @src_unit = func_create_unit_2(pk::get_building(fubing_person[0].service), fubing_person[0], src_pos);
                        fubing_person.remove_at(0);
                    }
                }

                if (!pk::is_alive(src_unit)) { pk::kill(src_unit); continue; }
                //if (int(pk::get_movable_pos(src_unit).length) == 0) { pk::kill(src_unit); break; }
                int unit_id = src_unit.get_id();
                unitinfo@ unit_t= @unit_ex[unit_id];
                unit_t.has_contact = true;//无论是否成功，此时都是已接触

                pk::set_order(src_unit, 部队任务_攻击, attacker.pos);
                if (!tactic) pk::attack(src_unit, attacker);
                else pk::run_order(src_unit); //pk::run_order(src_unit);

                src_unit.action_done = true;

            }
        }

        pk::list<pk::person@> get_fubing_person(int force_id, int building_id, int num, int src_id = -1, int weapon = -1)
        {
            //如果配置的伏兵数量大于1，那么新增加的伏兵部队主将的筛选规则如下：
            //4.1）和疑兵部队主将属于同一军团----范围是否太大，要不加上距离限制(疑兵部队所属城市或邻近城市)，不然从很远地方过来也打乱了ai的配置
            //4.2）身份正常（非俘虏死亡），没有正在执行任务
            //4.3）按武将统帅自高到低选择
            pk::list<pk::person@> person_list = pk::get_person_list(pk::get_force(force_id), pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
            if (person_list.count > 1)
            {
                if (weapon != -1)
                {
                    person_list.sort(function(a, b)
                    {
                        if (a.tekisei[兵种_骑兵] != b.tekisei[兵种_骑兵])
                            return a.tekisei[兵种_骑兵] > b.tekisei[兵种_骑兵];
                        return a.stat[武将能力_统率] > b.stat[武将能力_统率];  // 按统率排序
                    });
                }
                else
                {
                    person_list.sort(function(a, b)
                    {
                        return a.stat[武将能力_统率] > b.stat[武将能力_统率];  // 按统率排序
                    });
                }

            }
            pk::list<pk::person@> temp;
            int count = 0;
            if (src_id != -1) temp.add(pk::get_person(src_id));
            for (int i = 0; i < person_list.count; ++i)
            {
                if (src_id == person_list[i].get_id()) continue;
                if (pk::is_absent(person_list[i]) or pk::is_unitize(person_list[i])) continue;
                int person_building = person_list[i].service;
                //(疑兵部队所属城市或邻近城市)
                if (pk::is_neighbor_base(person_building, building_id) or building_id == person_building)
                {
                    //AI会从运输队所在城市所属的军团里选择一名武将向玩家运输队发动突袭，武将选择标准：统帅小于80，武力不低于75，骑兵适性不低于B，如果没有合适的武将则不发动突袭。突袭的部队种类是骑兵，部队出现的地点是运输队周边最近的格子，部队出现后会立刻发动攻击。
                    if (weapon == -1 or (person_list[i].stat[武将能力_统率] < 80 and person_list[i].stat[武将能力_武力] > 75))
                    {
                        temp.add(person_list[i]);
                        count += 1;
                        if (count >= num) break;//满足数量也不再寻找
                    }
                }

            }
            return temp;
        }
        //被灭部队的重新生成，不需要扣兵力，直接取数值
        pk::unit@ func_create_unit_1(int type, pk::building@ src_base, pk::person@ f_person, pk::point unit_pos, int num/*统领数*/, int troops, int weapon, int gold, int food, pk::person@ second_person = null, pk::person@ third_person = null)
        {
            pk::city@ src_city = pk::building_to_city(src_base);
            int ground_wpn;
            if (weapon == -1) ground_wpn = get_best_ground_wpn(f_person);//陆上兵器
            else ground_wpn = weapon;
            if (调试模式) pk::trace(pk::format("ground_wpn{}", ground_wpn));
            int sea_wpn = src_city.weapon_amount[兵器_斗舰] > 0 ? 兵器_斗舰 : (pk::building_to_city(src_base).weapon_amount[兵器_楼船] > 0 ? 兵器_楼船 : 兵器_走舸);//水上兵器
            //int src_troops = troops;//是否根据据点情况额外算
            int src_gold = gold;
            int src_food = food;
            int src_troops = troops;
            if (type != 0)
            {
                ch::add_troops(src_base, -src_troops);
                pk::add_gold(src_base, -src_gold);
                pk::add_food(src_base, -src_food);
            }

            pk::unit@ src_unit = pk::create_unit(src_base, f_person, second_person, third_person, src_troops, ground_wpn, sea_wpn, src_gold, src_food, unit_pos);
            if (src_unit !is null) unit_ex[src_unit.get_id()].has_contact = true;
            //src_unit.energy = 100;

            return src_unit;
        }
        //剩余部队的生成
        pk::unit@ func_create_unit_2(pk::building@ src_base, pk::person@ f_person, pk::point unit_pos, pk::person@ second_person = null, pk::person@ third_person = null)
        {
            pk::city@ src_city = pk::building_to_city(src_base);
            int ground_wpn = get_best_ground_wpn(f_person);//陆上兵器
            int sea_wpn = pk::get_weapon_amount(src_base, 兵器_斗舰) > 0 ? 兵器_斗舰 : (pk::get_weapon_amount(src_base, 兵器_楼船) > 0 ? 兵器_楼船 : 兵器_走舸);//水上兵器
            //int src_troops = troops;//是否根据据点情况额外算
            int src_gold = 1000;
            int src_food = 16000;
            int src_troops = 8000;
            if (pk::get_troops(src_base) < (10000 + src_troops)) return null;//防止据点被掏空
            if (pk::get_gold(src_base) < (1000)) src_gold = 0;
            if (pk::get_food(src_base) < (10000)) src_food = 5000;
            ch::add_troops(src_base, -src_troops);
            pk::add_gold(src_base, -src_gold);
            pk::add_food(src_base, -src_food);
            pk::unit@ src_unit = pk::create_unit(src_base, f_person, null, null, src_troops, ground_wpn, sea_wpn, src_gold, src_food, unit_pos);
            if (src_unit !is null) unit_ex[src_unit.get_id()].has_contact = true;
            //src_unit.energy = 100;

            return src_unit;
        }

        pk::point get_empty_pos(pk::point pos, int distance_min, int distance_max)
        {
            array<pk::point> range_pos_arr = pk::range(pos, distance_min, distance_max);
            for (int arr_index = 0; arr_index < int(range_pos_arr.length); arr_index++)
            {
                pk::point range_pos = range_pos_arr[arr_index];
                if (!pk::is_valid_pos(range_pos)) continue;

                pk::hex@ hex = pk::get_hex(range_pos);
                if (hex.has_building) continue;
                if (hex.has_unit) continue;

                int terrain_id = hex.terrain;
                if (!pk::is_valid_terrain_id(terrain_id)) continue;
                if (!pk::is_enabled_terrain(terrain_id)) continue;

                return range_pos;
            }

            return pk::point(-1, -1);
        }

        int get_best_able_ground_wpn(pk::person@ person, array<int>not_avaliable)
        {
            /*
            const int 兵器_枪 = 1;
const int 兵器_戟 = 2;
const int 兵器_弩 = 3;
const int 兵器_战马 = 4;
             */
            int best_weapon = -1;
            int best_heishu = -1;
            for (int weapon_id = 1; weapon_id < 5; weapon_id++)
            {

                if (person.tekisei[weapon_id - 1] > best_heishu and not_avaliable.find(weapon_id) < 0)
                {
                    best_heishu = person.tekisei[weapon_id - 1];
                    best_weapon = weapon_id;
                }
                //pk::trace(pk::format("weapon id :{}", weapon_id));
            }
            return best_weapon;
        }

        int get_best_ground_wpn(pk::person@ person)
        {
            /*
            const int 兵器_枪 = 1;
const int 兵器_戟 = 2;
const int 兵器_弩 = 3;
const int 兵器_战马 = 4;
             */
            int best_weapon = -1;
            int best_heishu = -1;
            for (int weapon_id = 1; weapon_id < 5; weapon_id++)
            {

                if (person.tekisei[weapon_id - 1] > best_heishu)
                {
                    best_heishu = person.tekisei[weapon_id - 1];
                    best_weapon = weapon_id;
                }
                //pk::trace(pk::format("weapon id :{}", weapon_id));
            }
            return best_weapon;
        }

    }


    /*----------------------------------------
        공격 피해량을 계산합니다.
        @param[out] damage	대미지 정보
        @param unit			부대
        @param tactics_id	#전법Id战术Id
        @param target_pos	목표 좌표
        @param type			0 공격, 1 반격, 2 반격
        @param critical		0 치명타 확률 계산, 1 치명타 성공 강제, 2 치명타 실패 강제
        @param ambush		0 통상, 1 복병
    ----------------------------------------*/
    bool is_yibing_triggered(pk::damage_info& info, pk::unit@ attacker, int tactics_id, pk::unit@ target_unit, int type, int critical, bool ambush, int rettype)
    {

        // 部队閒战斗  부대간 전투 (병력)
        if (target_unit !is null)
        {
            info.dst_troops = target_unit.troops;
            //疑兵判断
            if (开启疑兵伏兵)
            {
                //0玩家普攻，2玩家战法，还需测试齐攻的触发，15委任计算，16委任执行，15,16是ai的攻击和玩家的委任都触发
                if (rettype == 0 or rettype == 1 or rettype == 2 or rettype == 16)
                {
                    //pk::trace("209 0");
                    //AI势力互相之间不会发生任何疑兵事件，玩家委托的军团同理
                    if (pk::is_player_controlled(attacker))
                    {
                        //pk::trace("209 1");
                        //以下AI部队可成为疑兵（剑枪戟弩骑，3种水军）
                        //if (target_unit.weapon == 兵器_剑 or target_unit.weapon == 兵器_枪 or target_unit.weapon == 兵器_戟 or target_unit.weapon == 兵器_弩 or target_unit.weapon == 兵器_战马 or target_unit.weapon == 兵器_走舸 or target_unit.weapon == 兵器_楼船 or target_unit.weapon == 兵器_斗舰)
                        if (target_unit.weapon != 兵器_冲车 and target_unit.weapon != 兵器_木兽 and target_unit.weapon != 兵器_井阑 and target_unit.weapon != 兵器_投石)
                        {
                            //由陆地部队攻击水军，或者水军攻击陆地部队，不会触发疑兵
                            if ((pk::is_in_water(attacker) and pk::is_in_water(target_unit)) or (!pk::is_in_water(attacker) and !pk::is_in_water(target_unit)))
                            {
                                
                                //需要判断标记位
                                int target_unit_id = target_unit.get_id();
                                unitinfo@ unit_t = @unit_ex[target_unit_id];
                                if (调试模式) pk::trace("209 2" + unit_t.has_contact);
                                //unit_t.has_contact == true;
                                if (!unit_t.has_contact)//需要部队生成时改成true，或者直接默认false就是未接触
                                {
                                    if (!pk::has_member(attacker, 武将_诸葛亮) and !pk::has_member(attacker, 武将_张良))
                                    {
                                        unit_t.has_contact = true;//无论是否成功，此时都是已接触
                                        if (调试模式) pk::trace("209 3" + unit_t.has_contact);
                                        if (!is_near_building(attacker))//这个判断需遍历大量地格，所以放最后
                                        {
                                            int rate = cal_yibing_rate(attacker, target_unit)/难度倍率修正[pk::get_scenario().difficulty];
                                            if (pk::rand_bool(rate))//此处调成100可百分百触发疑兵伏兵
                                            {
                                                //直接触发？（码神版本），还是行动结束时触发？还是经验获得时？
                                                if (调试模式) pk::trace("209 4");
                                                //func_yibing_event(attacker, target_unit);
                                                触发伏兵 = true;
                                                //@f_unit = @target_unit;
                                                @f_person = pk::get_person(target_unit.leader);
                                                //f_skill_1 = f_person.skill;
                                                //f_person.skill = 特技_强运;//需要设定连击概率0(触发器的形式下，不需要)，捕获率0，负伤0，死亡0
                                                pk::core::add_hidden_skill(target_unit.leader, 特技_强运);
                                                if (target_unit.deputy[0] != -1)
                                                {
                                                    @f_deputy1 = pk::get_person(target_unit.deputy[0]);
                                                    //f_skill_2 = f_deputy1.skill;
                                                    //f_deputy1.skill = 特技_强运;//需要设定连击概率0，捕获率0，负伤0，死亡044
                                                    pk::core::add_hidden_skill(target_unit.deputy[0], 特技_强运);
                                                }
                                                if (target_unit.deputy[1] != -1)
                                                {
                                                    @f_deputy2 = pk::get_person(target_unit.deputy[1]);
                                                    //f_skill_3 = f_deputy2.skill;
                                                    //f_deputy2.skill = 特技_强运;//需要设定连击概率0，捕获率0，负伤0，死亡0
                                                    pk::core::add_hidden_skill(target_unit.deputy[1], 特技_强运);
                                                }
                                                f_food = target_unit.food;
                                                f_gold = target_unit.gold;
                                                f_troops = target_unit.troops;
                                                target_unit.food = 0;
                                                target_unit.gold = 0;//连钱粮都不让玩家收
                                                info.troops_damage = target_unit.troops;
                                                info.tactics = tactics_id;
                                                info.src_pos = attacker.get_pos();
                                                info.dst_pos = target_unit.pos;
                                                return true;
                                            }
                                        }

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    int cal_yibing_rate(pk::unit@attacker, pk::unit@target_unit)
    {
        //10）AI部队成为疑兵并非预设，而是在被首次攻击后动态计算，概率因子如下（以下涉及到ID时数据参照血色衣冠）
        //10.1）基础触发率，10%，可配置
        //10.2）攻击方主将性格猪突，+5%
        //10.3）AI部队在自势力领土上作战，+5%
        //10.4）AI部队被攻击时处于以下3类地形，湿地，森林，栈道，+5%
        //10.5）AI部队被攻击时位于西南2州（益州ID：0A，南中ID：0B，共下辖西南6城），+5%
        //10.6）如果开了位置系统，那么玩家从后侧翼攻击AI，+10%，从后背攻击AI，+20%。这里考虑的逻辑是玩家首次遭遇战，从后部攻击AI虽然有很高的受益，但潜在的也会中了AI部队门户大开的诱敌之计。高风险高收益
        //10.7）玩家部队智力抵消触发概率，（玩家部队智力-阈值）/2，即为抵消值，缺省阈值90，举例来说，如果阈值设定为50，玩家部队智力100，则可降低触发概率25%，可配置
        //10.8）AI部队智力加成触发概率，（AI部队智力-阈值）/2，即为加成值，缺省阈值70，举例来说，如果阈值设定为50，AI部队智力100，则可提升触发概率25%，可配置
        //const int 疑兵基础概率 = 10;
        //const int 减少疑兵概率智力下限 = 90;//攻击方
        //const int 增加疑兵概率智力下限 = 70;//被攻击方
        int rate = 疑兵基础概率;
        rate += pk::get_person(attacker.leader).character == 性格_莽撞 ? 5 : 0;
        rate += pk::get_building(pk::get_building_id(target_unit.pos)).get_force_id() == target_unit.get_force_id() ? 5 : 0;
        pk::hex@dst_hex = pk::get_hex(target_unit.pos);
        int terrain_id = dst_hex.terrain;
        rate += (terrain_id == 地形_湿地 or terrain_id == 地形_森 or terrain_id == 地形_栈道) ? 5 : 0;
        int dst_province_id = pk::get_province_id(target_unit.pos);
        rate += (dst_province_id == 州_益州 or dst_province_id == 州_南中) ? 5 : 0;
        if (交战朝向修正)
        {
            int attack_dir = batt::get_unit_direction(attacker, target_unit);
            int dir_buf = 0;
            switch (attack_dir)
            {
            case 2: dir_buf = 10; break;  // 후측 대미지 가중치
            case 3: dir_buf = 20; break;  // 후면 대미지 가중치
            }
            rate += dir_buf;
        }

        rate -= pk::max(0, (attacker.attr.stat[部队能力_智力] - 减少疑兵概率智力下限));
        rate += pk::max(0, (target_unit.attr.stat[部队能力_智力] - 增加疑兵概率智力下限));
        if (调试模式) pk::trace(pk::format("rate:{}", rate));
        return rate;
    }
    //此函数只计算因己方因素导致的疑兵概率
    int cal_src_yibing_rate(pk::unit@attacker, pk::city@ dst_city)
    {
        //10）AI部队成为疑兵并非预设，而是在被首次攻击后动态计算，概率因子如下（以下涉及到ID时数据参照血色衣冠）
        //10.1）基础触发率，10%，可配置
        //10.2）攻击方主将性格猪突，+5%
        //10.3）AI部队在自势力领土上作战，+5%
        //10.4）AI部队被攻击时处于以下3类地形，湿地，森林，栈道，+5%
        //10.5）AI部队被攻击时位于西南2州（益州ID：0A，南中ID：0B，共下辖西南6城），+5%
        //10.6）如果开了位置系统，那么玩家从后侧翼攻击AI，+10%，从后背攻击AI，+20%。这里考虑的逻辑是玩家首次遭遇战，从后部攻击AI虽然有很高的受益，但潜在的也会中了AI部队门户大开的诱敌之计。高风险高收益
        //10.7）玩家部队智力抵消触发概率，（玩家部队智力-阈值）/2，即为抵消值，缺省阈值90，举例来说，如果阈值设定为50，玩家部队智力100，则可降低触发概率25%，可配置
        //10.8）AI部队智力加成触发概率，（AI部队智力-阈值）/2，即为加成值，缺省阈值70，举例来说，如果阈值设定为50，AI部队智力100，则可提升触发概率25%，可配置

        int rate = 疑兵基础概率;
        rate += pk::get_person(attacker.leader).character == 性格_莽撞 ? 5 : 0;
        pk::hex@dst_hex = pk::get_hex(attacker.pos);
        int terrain_id = dst_hex.terrain;
        rate += (terrain_id == 地形_湿地 or terrain_id == 地形_森 or terrain_id == 地形_栈道 or terrain_id == 地形_崖) ? 5 : 0;
        int dst_province_id = pk::get_province_id(attacker.pos);
        rate += (dst_province_id == 州_益州 or dst_province_id == 州_南中) ? 5 : 0;

        rate -= pk::max(0, (attacker.attr.stat[部队能力_智力] - 减少疑兵概率智力下限));
        if (调试模式) pk::trace(pk::format("hex fubing rate:{}", rate));
        return rate;
    }

    //玩家防御作战时免疫疑兵-伏兵（条件：玩家部队在自势力城市5格以内，港关3格以内）。
    //玩家在前线有阵地时免疫疑兵-伏兵（条件：玩家部队两格内有属于自势力的防御性建筑物，包括阵，砦，城塞，箭楼，连弩楼，投石台）合理使用这两项可以有效控制疑兵的影响
    bool is_near_building(pk::unit @ src_unit)
    {
        //或者城市用get_city_id()
        pk::point pos = src_unit.pos;
        int src_force_id = src_unit.get_force_id();
        int area_city_id = pk::get_city_id(pos);
        if (pk::get_city(area_city_id).get_force_id() == src_force_id) return true;
        array<pk::point> range_pos_arr = pk::range(pos, 1, 3);//直接搜索3格，因为港关是3格，排除城市，因为城市范围太大
        for (int arr_index = 0; arr_index < int(range_pos_arr.length); arr_index++)
        {
            pk::point range_pos = range_pos_arr[arr_index];
            if (!pk::is_valid_pos(range_pos)) continue;
            pk::hex@ hex = pk::get_hex(range_pos);
            if (!hex.has_building) continue;
            pk::building@ building0 = pk::get_building(range_pos);

            if ((building0.facility == 设施_港口 or building0.facility == 设施_关卡) and building0.get_force_id() == src_force_id)
            {
                if (pk::get_distance(pos, range_pos) <= 3) return true;
            }
            if ((building0.facility == 设施_阵 or building0.facility == 设施_砦 or building0.facility == 设施_城塞 or building0.facility == 设施_箭楼 or building0.facility == 设施_连弩楼 or building0.facility == 设施_投石台) and building0.get_force_id() == src_force_id)
            {
                if (pk::get_distance(pos, range_pos) <= 2) return true;
            }
        }

        return false;
    }


    Main main;
}



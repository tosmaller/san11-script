// ## 2022/03/19 # 江东新风 # 游戏自带选择窗口选择能力研究 ##
// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/02/03 # 江东新风 # 重复研究bug修复 ##
// ## 2021/11/08 # 江东新风 # 完善 ##
// ## 2021/11/06 # 江东新风 # 自动研究能力的设定 ##


/*
作者： 江东新风
简介： 忘记研究能力时提醒玩家研究能力
计划： 忘记培养武将特技或能力时也提醒
*/
namespace 自动研究
{
    // ================ CUSTOMIZE ================
    const bool 开启自动研究 = true;//是否开启自动研究
    // ===========================================
    //array<bool> research_info(非贼势力_末, false);
    class Main
    {
        pk::building@ research_building_;
        Main()
        {
            pk::bind(112, pk::trigger112_t(onTurnEnd));
        }

        void onTurnEnd(pk::force@ force)
        {
            if (开启自动研究 and pk::is_alive(force) and force.is_player())
            {
                if (true)//research_info[force.get_id()])//判断放到回合开始时，因为回合结束时研究时间会自动减一，导致即将研究完的能力会被认为已经研究完
                {
                    if (force.researching_ability != -1 or force.researching_ability_timer != 0) return; //排除玩家回合中研究的情况
                    if (pk::get_district(pk::get_district_id(force, 1)).ap < 20 and !force_ex[force.get_id()].all_ability_researched)
                    {
                        pk::message_box(pk::encode("主公似乎忘记了研究能力。\n目前行动力不足，请下回合务必注意。"), pk::get_person(武将_文官));
                        return;
                    }
                    /*
                    for (int i = 0; i < 255; i++)
                    {
                        pk::ability@ ability = pk::get_ability(i);
                        if (pk::is_alive(ability))
                        {
                            pk::trace(pk::format("能力id：{},名称:{}",i,pk::decode(pk::get_name(ability))));
                        }
                    }*/
                    int building_id = -1;
                    int kunshu_service = pk::get_person(force.kunshu).service;
                    if (kunshu_service != -1)
                    {
                        if (pk::get_gold(pk::get_building(kunshu_service)) > 300) building_id = kunshu_service;
                    }
                    if (building_id == -1)
                    {
                        pk::list<pk::building@> src_base_list = ch::get_base_list(force);
                        if (src_base_list.count > 0)
                        {
                            for (int i = 0; i < src_base_list.count; i++)
                            {
                                if (pk::get_gold(src_base_list[i]) > 300)
                                {
                                    building_id = src_base_list[i].get_id();
                                    break;
                                }
                            }
                        }
                    }
                    if (building_id > -1 and building_id < 据点_末)
                    {
                        //势力特殊能力上限10个，但实际剧本开始时所有势力随机生成5个，位置不固定
                        //0-47为固定能力 
                        //搜索可研究能力
                        array<int> can_ability;
                        for (int i = 0; i < 势力能力_基础能力末; i++)
                        {
                            if (force.ability_researched[i]) continue;//已研究的能力无需研究
                            pk::ability@ ability = pk::get_ability(i);
                            bool can_research = true;
                            for (int j = 0; j < 3; j++)
                            {
                                if (ability.required_ability[j] == -1) continue;
                                if (force.ability_researched[ability.required_ability[j]] == false) { can_research = false; break; }
                            }
                            if (can_research) can_ability.insertLast(i);
                        }
                        for (int i = 0; i < 10; i++)
                        {
                            if (force.sp_ability[i] == -1) continue;
                            if (force.sp_ability_researched[i]) continue;//已研究的能力无需研究
                            pk::ability@ ability = pk::get_ability(force.sp_ability[i]);
                            bool can_research = true;
                            for (int j = 0; j < 3; j++)
                            {
                                if (ability.required_ability[j] == -1) continue;
                                if (force.ability_researched[ability.required_ability[j]] == false) { can_research = false; break; }
                            }
                            if (can_research) can_ability.insertLast(force.sp_ability[i]);
                        }
                        /*
                        for (int i = 0; i < int(can_ability.length); i++)
                        {
                            pk::ability@ ability = pk::get_ability(can_ability[i]);
                            string name = pk::decode(ability.name);
                            pk::trace(pk::format("当前可研究能力：{}", name));
                        }*/
                        if (int(can_ability.length) > 0)
                        {
                            //bool confirm = pk::yes_no(pk::encode("当前并未进行能力研究，是否要选择研究的能力?"));//回合结束时无法调用这个
                            pk::person@ 文官 = pk::get_person(武将_文官);
                            if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("当前并未进行能力研究，是否要选择研究的能力?"), 文官) == 0)
                            {
                                @research_building_ = pk::get_building(building_id);
                                
                                pk::scene(pk::scene_t(research_choose));
                                /*
                                //pk::research_ability_cmd_info cmd_draft;
                                //@cmd_draft.base = pk::get_building(building_id);
                                //if (pk::call_research_abillity_window(cmd_draft, pk::get_building(building_id)))
                                {
                                    int ability_id = choose_ability(can_ability);
                                    if (ability_id > -1 and ability_id < 势力能力_末)
                                    {

                                        pk::play_se(6);
                                        pk::message_box(pk::encode(pk::format("好的，马上开始\x1b[2x{}\x1b[0x的研究。", pk::decode(pk::get_ability(ability_id).name))), 文官);
                                        
                                        pk::research_ability_cmd_info cmd_draft;
                                        @cmd_draft.base = pk::get_building(building_id);
                                        cmd_draft.ability = ability_id;
                                        pk::command(cmd_draft);
                                        pk::ability@ ability = pk::get_ability(ability_id);
                                        string name = pk::decode(ability.name);
                                        //pk::trace(pk::format("{}自动研究能力：{}", pk::get_new_base_name(building_id), name));
                                    }
                                }*/
     
                            }


                        }
                        else
                        {
                            force_ex[force.get_id()].all_ability_researched = true;
                        }
                        //pk::trace(pk::format("当前正在研究能力id：{},剩余时间：{}",force.researching_tech, force.researching_tech_timer));
                    }

                }
            }
        }

        void research_choose()
        {
            pk::research_ability_cmd_info cmd_draft;
            @cmd_draft.base = @research_building_;
            if (pk::call_research_abillity_window(cmd_draft, research_building_))
            {
                int ability_id = cmd_draft.ability;
                if (ability_id > -1 and ability_id < 势力能力_末)
                {

                    //pk::play_se(6);
                    //pk::message_box(pk::encode(pk::format("好的，马上开始\x1b[2x{}\x1b[0x的研究。", pk::decode(pk::get_ability(ability_id).name))), pk::get_person(武将_文官));

                    //pk::research_ability_cmd_info cmd_draft;
                    //@cmd_draft.base = pk::get_building(building_id);
                    //cmd_draft.ability = ability_id;
                    pk::command(cmd_draft);
                    pk::ability@ ability = pk::get_ability(ability_id);
                    string name = pk::decode(ability.name);
                    pk::trace(pk::format("{}自动研究能力：{}", pk::get_new_base_name(research_building_.get_id()), name));
                }
            }
            //ability_id_ = pk::call_research_abillity_window(cmd_draft, pk::get_building(building_id))
        }

        int choose_ability(array<int> ability_arr)
        {
            //一个城市最多8个？
            // 头5+1 中间4+2 尾部 n+1
            int num = int(ability_arr.length);
            //要获取的是ability_id
            array<string> ability_name_list;
            for (int i = 0; i < pk::min(5, num); ++i)
            {
                ability_name_list.insertLast(pk::get_ability(ability_arr[i]).name);
            }

            if (num == 6) ability_name_list.insertLast(pk::get_ability(ability_arr[num]).name);
            else if (num>6) ability_name_list.insertLast(pk::encode("下一页"));
            pk::person@ 文官 = pk::get_person(武将_文官);
            int n = pk::choose(ability_name_list, pk::encode("请选择要研究的能力."), 文官);
            if (n == 5 and num != 6) return choose_ability_mid(ability_arr, 1);
            return ability_arr[n];
        }

        int choose_ability_mid(array<int> ability_arr, int page)
        {
            //一个城市最多8个？
            // 头5+1 中间4+2 尾部 n+1
            pk::person@ 文官 = pk::get_person(武将_文官);
            int num = int(ability_arr.length);
            int num2 = (num - 1 - 4 * page);
            if (num2 <= 5)
            {
                //最后一页
                //要获取的是ability_id
                array<string> ability_name_list;
                for (int i = (1 + page * 4); i < (1 + page * 4 + num2); ++i)
                {
                    ability_name_list.insertLast(pk::get_ability(ability_arr[i]).name);
                }
                ability_name_list.insertLast(pk::encode("上一页"));
                int n = pk::choose( ability_name_list, pk::encode("请选择要研究的能力."), 文官);

                if (n == 5 or n == int(ability_name_list.length - 1))
                {
                    if (page == 1) return choose_ability(ability_arr);
                    return choose_ability_mid(ability_arr, page - 1);//如何返回上一页
                }

                int t = n + (page - 1) * 4 + 5;

                return ability_arr[t];
            }
            else//不是最后一页
            {
                //要获取的是ability_id
                array<string> ability_name_list;
                for (int i = (1 + page * 4); i < (1 + page * 4 + 4); ++i)
                {
                    ability_name_list.insertLast(pk::get_ability(ability_arr[num]).name);
                }
                ability_name_list.insertLast(pk::encode("上一页"));
                ability_name_list.insertLast(pk::encode("下一页"));

                int n = pk::choose(ability_name_list, pk::encode("请选择要研究的能力."), 文官);

                //非最后一页情况
                if (n == 4)
                {
                    if (page != 1) return choose_ability_mid(ability_arr, page - 1);
                    else return choose_ability(ability_arr);
                }
                if (n == 5) return choose_ability_mid(ability_arr, page + 1);

                int t = n + (page - 1) * 4 + 5;

                return ability_arr[t];
            }

        }


    }

    Main main;
}
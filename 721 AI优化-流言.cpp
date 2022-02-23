// ## 2021/11/05 # 江东新风 # 电脑承诺，不首先使用流言 ##

namespace AI优化_流言
{
    // ================ CUSTOMIZE ================
    const int 报复范围 = 2;//城市距离多少的ai会来报复
    // ===========================================

    class Main
    {
        Main()
        {
            pk::bind(165, pk::trigger165_t(callback));
            pk::bind(108, pk::trigger108_t(onNewMonth));
            pk::bind(111, pk::trigger111_t(onTurnStart));
            pk::set_func(61, pk::func61_t(rumor_result));
        }

        int rumor_result(pk::building@ target_base, pk::person@ actor)
        {
            //结果 0 成功未发现 1 成功被发现 2 失败 3 失败被捕 4 返回原概率计算
            if (!actor.is_player()) return 4;
            if (base_ex[target_base.get_id()].anti_rumor > 1) return 2;
            return 4;
        }

        void onNewMonth()
        {
            for (int i = 0; i < 据点_末; ++i)
            {
                if (base_ex[i].anti_rumor > 2) base_ex[i].anti_rumor -= 2;
                else base_ex[i].anti_rumor = 0;
            }
        }

        void callback(pk::building@ target_base/*目标据点*/, pk::person@ actor/*流言者*/, int type)
        {
            //pk::trace(pk::format("target_force_id{}，actor_force_id{}，type：{}", target_force_id/*流言者*/, actor_force_id/*被流言者*/, type));
            if (type == 0 or type == 1 or type == 2 or type == 3)//0 成功未发现 1 成功被发现 2 失败逃脱 3 失败被捕
            {
                if (pk::is_alive(actor) and pk::is_alive(target_base))
                {
                    pk::force@ actor_force = pk::get_force(actor.get_force_id());
                    if (pk::is_alive(actor_force))
                    {
                        int target_force_id = target_base.get_force_id();
                        if (actor_force.is_player())
                        {
                            base_ex[target_base.get_id()].anti_rumor += 1;//玩家对ai使用流言会导致对应据点警戒等级上升
                            force_ex[target_force_id].rumor_timer[actor_force.player] += 1;
                            setting_ex.rumor_timer += 2;
                            //pk::trace(pk::format("{}被{}流言计数:{},报复计数:{}", pk::decode(pk::get_name(pk::get_force(target_force_id))), pk::decode(pk::get_name(actor_force)), force_ex[target_force_id].rumor_timer[actor_force.player], setting_ex.rumor_timer));
                        }
                    }
                }
            }
        }

        void onTurnStart(pk::force@ force)
        {
            if (pk::is_alive(force) and setting_ex.rumor_timer > 0 and !force.is_player())
            {
                bool done = false;
                pk::list<pk::city@> city_list = pk::get_city_list(force);
                if (city_list.count > 0)
                {

                    for (int i = 0; i < city_list.count; ++i)
                    {
                        pk::building@ building = pk::city_to_building(city_list[i]);
                        for (int j = 0; j < 城市_末; ++j)
                        {
                            pk::building@ player_building = pk::get_building(j);
                            if (pk::is_alive(player_building) and pk::is_player_controlled(player_building))
                            {
                                if (pk::get_city_distance(building.get_id(), j) <= 2)//范围2
                                {
                                    pk::list<pk::person@> person_list = pk::get_idle_person_list(building);
                                    //pk::trace("满足条件，执行报复");
                                    for (int k = 0; k < person_list.count; ++k)
                                    {

                                        if (pk::cal_rumor_result(person_list[k], player_building) < 2)
                                        {
                                            
                                            setting_ex.rumor_timer -= 1;
                                            pk::trace(pk::format("满足条件，执行报复,报复武将id：{},报复计数：{}", person_list[k].get_id(), setting_ex.rumor_timer));
                                            pk::rumor_cmd_info cmd_draft;
                                            @cmd_draft.base = building;
                                            @cmd_draft.actor = person_list[k];
                                            @cmd_draft.target = player_building;
                                            pk::command(cmd_draft);
                                            if (setting_ex.rumor_timer == 0)
                                            {
                                                done = true;
                                                break;
                                            }
                                        }
                                    }
                                    if (done) break;
                                }
                            }

                        }
                    }
                }
            }
        }
    }

    Main main;
}
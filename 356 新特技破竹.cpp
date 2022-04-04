// ## 2021/10/03 # 江东新风 # 军团长部队溃灭，周围三格己方部队士气下降50改回30 ##
// ## 2021/09/03 # 江东新风 # 将破竹从209移至此，同时添加君主部队溃灭，周围五格己方部队士气下降50，军团长部队溃灭，周围三格己方部队士气下降30 ##

namespace 新特技_破竹
{
    class Main
    {
        Main()
        {
            pk::bind(175, pk::trigger175_t(callback));
        }

        void callback(pk::unit@ attacker, pk::unit@ target)//测试有效
        {
            //破竹
            if (ch::has_skill(attacker, 特技_破竹))
            {
                auto arr = pk::range(target.pos, 1, 2);
                for (int i = 0; i < int(arr.length); i++)
                {
                    pk::unit@ dst = pk::get_unit(arr[i]);
                    if (dst !is null and dst.get_force_id() == target.get_force_id())
                    {
                        pk::add_energy(dst, -破竹气力减少, true);
                        pk::point pos = arr[i];
                        if (pk::is_in_screen(pos))
                        {
                            pk::play_se(0x39, pos);
                            pk::create_effect(60, pos);

                            switch (pk::rand(2))
                            {
                            case 0: pk::say(pk::encode("局势...急转直下..."), pk::get_person(dst.leader)); break;
                            case 1: pk::say(pk::encode("这是...一面倒的屠杀吗..."), pk::get_person(dst.leader)); break;
                            }
                        }
                    }

                }
            }
            //君主队溃灭，周边五格气力大衰减
            if (target.leader == pk::get_kunshu_id(target))
            {
                auto arr = pk::range(target.pos, 1, 5);
                for (int i = 0; i < int(arr.length); i++)
                {
                    pk::unit@ dst = pk::get_unit(arr[i]);
                    if (dst !is null and dst.get_force_id() == target.get_force_id())
                    {
                        pk::add_energy(dst, -50, true);
                        pk::point pos = arr[i];
                        if (pk::is_in_screen(pos))
                        {
                            pk::play_se(0x39, pos);
                            pk::create_effect(60, pos);

                            switch (pk::rand(2))
                            {
                            case 0: pk::say(pk::encode("呃!主公居然..."), pk::get_person(dst.leader)); break;
                            case 1: pk::say(pk::encode("不...要完蛋了吗..."), pk::get_person(dst.leader)); break;
                            }
                        }
                    }

                }
            }
            else if (target.leader == pk::get_totoku_id(target))//都督队溃灭，周边三格气力大衰减
            {
                auto arr = pk::range(target.pos, 1, 3);
                for (int i = 0; i < int(arr.length); i++)
                {
                    pk::unit@ dst = pk::get_unit(arr[i]);
                    if (dst !is null and dst.get_force_id() == target.get_force_id())
                    {
                        pk::add_energy(dst, -30, true);
                        pk::point pos = arr[i];
                        if (pk::is_in_screen(pos))
                        {
                            pk::play_se(0x39, pos);
                            pk::create_effect(60, pos);

                            switch (pk::rand(2))
                            {
                            case 0: pk::say(pk::encode("呃!都督...竟然战败了嘛..."), pk::get_person(dst.leader)); break;
                            case 1: pk::say(pk::encode("...要完蛋了吗..."), pk::get_person(dst.leader)); break;
                            }
                        }
                    }
                }
            }
        }

    }

    Main main;
}
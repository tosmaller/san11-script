// ## 2021/01/30 # 江东新风 # 将图片和动画音效分开 ##
// ## 2021/01/29 # 江东新风 # 更改playse为playsfx ##
// ## 2021/01/20 # 江东新风 # 同步白马叔叔更新，加一个连续播放2，少点几次 ##
// ## 2020/07/26 ##
/*
@ 만든이: 기마책사
@ UPDATE: '18.11.25  / 최초적용
@ UPDATE: '18.12.25  / 변경내용: 효과음듣기추가
*/

namespace 컷인배경테스트
{
    //---------------------------------------------------------------------------------------

    const int KEY_컷인 = pk::hash("酉胎");
    const int KEY_배경 = pk::hash("齿回");

    //---------------------------------------------------------------------------------------



    class Main
    {
        int menu_num;

        int min_value;
        int max_value;
        int def_value;
        int cur_value;

        Main()
        {
            add_menu();
            add_menu2();
        }

        void add_menu()
        {
            pk::menu_item item;
            item.menu = 2;
            item.get_text = pk::menu_item_get_text_t(getText);
            item.get_desc = pk::menu_item_get_desc_t(getDesc);
            item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            item.handler = pk::menu_item_handler_t(handler);
            menu_num = pk::add_menu_item(item);

        }

        void add_menu2()
        {
            pk::menu_item item2;
            item2.menu = 2;
            item2.get_text = pk::menu_item_get_text_t(getText2);
            item2.get_desc = pk::menu_item_get_desc_t(getDesc2);
            item2.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            item2.handler = pk::menu_item_handler_t(handler2);
            menu_num = pk::add_menu_item(item2);

        }

        //------------------------------------------------------------

        string pad_callback(int line, int original_value, int current_value)
        {

            return pk::encode("输入数字");  // 텍스트 표시기능인 듯 하나, 사용방법 모르겠음
        }

        //------------------------------------------------------------

        string getText()
        {
            return pk::encode("图片鉴赏");
        }

        string getDesc()
        {
            return pk::encode("观看游戏内相关图片");
        }

        string getText2()
        {
            return pk::encode("音效动画");
        }

        string getDesc2()
        {
            return pk::encode("查阅游戏相关音效及动画");
        }

        bool isEnabled()
        {
            return true;
        }


        bool handler()
        {
            int answer = 0;
            answer = pk::choose(pk::encode("请选择要鉴赏的项目"),
                { pk::encode("背景视图"), pk::encode("鉴赏视图"), pk::encode("插图"),  pk::encode("取消") });
            if (answer == 3) return true;  // 종료

            if (answer == 0)
                pk::scene(pk::scene_t(scene_背景));

            if (answer == 1)
                pk::scene(pk::scene_t(scene_鉴赏));

            if (answer == 2)
                pk::scene(pk::scene_t(scene_插图));

            return true;
        }

        bool handler2()
        {
            int answer = 0;
            answer = pk::choose(pk::encode("请选择执行的模式"),
                { pk::encode("试听音效"), pk::encode("测试语音"), pk::encode("测试动画"), pk::encode("取消") });
            if (answer == 3) return true;  // 종료

            if (answer == 0)
                pk::scene(pk::scene_t(play_sound_effect2));
            if (answer == 1)
                play_voice();
            if (answer == 2)
                play_effect();
                
            return true;
        }

        void scene_背景()
        {
            int answer = 0;
            answer = pk::choose(pk::encode("请选择背景视图查看方式"),
                { pk::encode("连续视图"), pk::encode("输入数字"), pk::encode("取消") });
            if (answer == 2) return;  // 종료


            min_value = 0;
            max_value = 62;
            def_value = 0;

            int pause = int(pk::option["MessagePause"]);
            pk::option["MessagePause"] = 100;

            bool next = true;

            if (answer == 0)
            {
                for (int num = min_value; num <= max_value; num++)
                {
                    if (next)
                    {
                        pk::fade(0);
                        pk::sleep();
                        //open event still 查看的是游戏鉴赏视图的内容  到49为止
                        //pk::close_event_still(),关闭鉴赏视图，不需要带数字
                        //pk::open_event_still(num);
                       // pk::open_event_still(num);
                        //pk::sleep();
                        //pk::close_event_still();
                        pk::background(num);
                        pk::fade(255);

                        next = pk::yes_no(pk::encode(pk::format("背景视图\x1b[1x{}\x1b[0x运行完成\n是否要继续浏览?", num)));

                    }
                }
            }
            if (answer == 1)
            {
                while (next)
                {
                    def_value = pk::numberpad(pk::encode("背景序号"), min_value, max_value, def_value, pk::numberpad_t(pad_callback)).first;

                    pk::fade(0);
                    pk::sleep();
                    pk::background(def_value);
                    pk::fade(255);

                    next = pk::yes_no(pk::encode(pk::format("背景视图\x1b[1x{}\x1b[0x运行完成\n是否要继续浏览?", def_value)));

                }
            }

            pk::fade(0);
            pk::sleep();
            pk::background(-1);
            pk::fade(255);

            pk::option["MessagePause"] = pause;
        }


        void scene_插图()
        {
            int answer = 0;
            answer = pk::choose(pk::encode("请选择插图视图查看方式"),
                { pk::encode("连续视图"), pk::encode("输入数字"), pk::encode("取消") });
            if (answer == 2) return;  // 종료

            min_value = 0;
            max_value = 22;
            def_value = 0;

            int pause = int(pk::option["MessagePause"]);
            pk::option["MessagePause"] = 100;

            bool next = true;

            if (answer == 0)
            {
                for (int num = min_value; num <= max_value; num++)
                {
                    if (next)
                    {
                        pk::cutin(num);
                        next = pk::yes_no(pk::encode(pk::format("插图\x1b[1x{}\x1b[0x运行完成\n是否要继续浏览?", num)));

                    }
                }
            }
            if (answer == 1)
            {
                while (next)
                {
                    def_value = pk::numberpad(pk::encode("指定序号"), min_value, max_value, def_value, pk::numberpad_t(pad_callback)).first;

                    pk::cutin(def_value);
                    next = pk::yes_no(pk::encode(pk::format("插图\x1b[1x{}\x1b[0x运行完成\n是否要继续浏览?", def_value)));
                }
            }

            pk::option["MessagePause"] = pause;
        }

        void scene_鉴赏()
        {
            int answer = 0;
            answer = pk::choose(pk::encode("请选择鉴赏视图查看方式"),
                { pk::encode("连续视图"), pk::encode("输入数字"), pk::encode("取消") });
            if (answer == 2) return;  // 종료


            min_value = 0;
            max_value = 49;
            def_value = 0;

            int pause = int(pk::option["MessagePause"]);
            pk::option["MessagePause"] = 100;

            bool next = true;

            if (answer == 0)
            {
                for (int num = min_value; num <= max_value; num++)
                {
                    if (next)
                    {
                        //pk::fade(0);
                        //pk::sleep();
                        //open event still 查看的是游戏鉴赏视图的内容  到49为止
                        //pk::close_event_still(),关闭鉴赏视图，不需要带数字
                        pk::open_event_still(num);
                       // pk::open_event_still(num);
                        //pk::sleep();
                        //pk::close_event_still();
                        //pk::fade(255);

                        next = pk::yes_no(pk::encode(pk::format("背景视图\x1b[1x{}\x1b[0x运行完成\n是否要继续浏览?", num)));
                        pk::close_event_still();

                    }
                }
            }
            if (answer == 1)
            {
                while (next)
                {
                    def_value = pk::numberpad(pk::encode("背景序号"), min_value, max_value, def_value, pk::numberpad_t(pad_callback)).first;

                    pk::fade(0);
                    pk::sleep();
                    pk::open_event_still(def_value);
                    pk::fade(255);

                    next = pk::yes_no(pk::encode(pk::format("背景视图\x1b[1x{}\x1b[0x运行完成\n是否要继续浏览?", def_value)));
                    pk::close_event_still();
                }
            }

            pk::fade(0);
            pk::sleep();
            pk::background(-1);
            pk::fade(255);

            pk::option["MessagePause"] = pause;
        }

        void play_sound_effect()
        {
            int answer = 0;
            answer = pk::choose(pk::encode("请选择音效的听音方式"),
                { pk::encode("连续听音"), pk::encode("输入数字"), pk::encode("取消") });
            if (answer == 2) return;  // 종료

            min_value = 0;
            max_value = 1255;
            def_value = 0;

            int pause = int(pk::option["MessagePause"]);
            pk::option["MessagePause"] = 100;

            bool next = true;

            if (answer == 0)
            {
                for (int num = min_value; num <= max_value; num++)
                {
                    if (next)
                    {
                        pk::play_se(num);
                        next = pk::yes_no(pk::encode(pk::format("音效\x1b[1x{}\x1b[0x运行完成\n是否要继续听下去?", num)));
                    }
                }
            }
            if (answer == 1)
            {
                while (next)
                {
                    def_value = pk::numberpad(pk::encode("音效编号"), min_value, max_value, def_value, pk::numberpad_t(pad_callback)).first;

                    pk::play_se(def_value);
                    next = pk::yes_no(pk::encode(pk::format("音效\x1b[1x{}\x1b[0x运行完成\n是否要继续听下去?", def_value)));
                }
            }

            pk::option["MessagePause"] = pause;
        }

        void play_sound_effect2()
        {
            int answer = 0;
            answer = pk::choose(pk::encode("请选择音效的听音方式"),
                { pk::encode("连续听音"), pk::encode("输入数字"), pk::encode("取消") });
            if (answer == 2) return;  // 종료

            min_value = 0;
            max_value = 1255;
            def_value = 0;

            int pause = int(pk::option["MessagePause"]);
            pk::option["MessagePause"] = 100;

            bool next = true;

            if (answer == 0)
            {
                for (int num = min_value; num <= max_value; num++)
                {
                    if (next)
                    {
                        pk::play_sfx(num);
                        next = pk::yes_no(pk::encode(pk::format("音效\x1b[1x{}\x1b[0x运行完成\n是否要继续听下去?", num)));
                    }
                }
            }
            if (answer == 1)
            {
                while (next)
                {
                    def_value = pk::numberpad(pk::encode("音效编号"), min_value, max_value, def_value, pk::numberpad_t(pad_callback)).first;

                    pk::play_sfx(def_value);
                    next = pk::yes_no(pk::encode(pk::format("音效\x1b[1x{}\x1b[0x运行完成\n是否要继续听下去?", def_value)));
                }
            }

            pk::option["MessagePause"] = pause;
        }

        void play_effect()
        {
            int answer = 0;
            answer = pk::choose(pk::encode("请选择动画效果的播放方式"),
                { pk::encode("连续播放"), pk::encode("输入数字"), pk::encode("取消") });
            if (answer == 2) return;  // 종료

            min_value = 0;
            max_value = 255;
            def_value = 0;

            int pause = int(pk::option["MessagePause"]);
            pk::option["MessagePause"] = 100;

            bool next = true;
            pk::point pos = pk::screen_pos_to_hex_pos(pk::get_cursor_screen_pos());
            //pk::effect_handle eft;
            if (answer == 0)
            {
                for (int num = min_value; num <= max_value; num++)
                {
                    if (next)
                    {

                        auto eft = pk::create_effect(num, pos);
                        //pk::trace(pk::format("valid:{}", pk::create_effect(num, pos).valid));
                        //pk::wait(1, 1000);
                        next = pk::yes_no(pk::encode(pk::format("效果\x1b[1x{}\x1b[0x运行完成\n是否要继续听下去?", num)));
                        pk::remove(eft);
                        //pk::remove(pk::create_effect(num, pos));
                        //pk::trace(pk::format("valid2:{}", eft.valid));
                    }
                }
            }
            if (answer == 1)
            {
                min_value = 0;
                while (next)
                {
                    def_value = pk::numberpad(pk::encode("效果编号"), min_value, max_value, def_value, pk::numberpad_t(pad_callback)).first;

                    pk::create_effect(def_value, pos);
                    next = pk::yes_no(pk::encode(pk::format("效果\x1b[1x{}\x1b[0x运行完成\n是否要继续听下去?", def_value)));
                    pk::remove(pk::create_effect(def_value, pos));
                }
            }

            pk::option["MessagePause"] = pause;
        }

        void play_voice()
        {
            pk::list<pk::person@> person_list = pk::get_person_list(pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般, 身份_在野, 身份_未发现, 身份_未登场));
            array<pk::person@> person_sel_arr;
            //person_list = getTargetList();
            person_sel_arr = pk::list_to_array(pk::person_selector(pk::encode("武将选择"), pk::encode("武将语音."), person_list, 1, 1));
            pk::person@selected_person_ = person_sel_arr[0];

            int answer = 0;
            answer = pk::choose(pk::encode("请选择语音的播放方式"),
                { pk::encode("连续播放"),  pk::encode("连续播放2"),pk::encode("输入数字"), pk::encode("取消") });
            if (answer == 3) return;  // 종료

            min_value = 0;
            max_value = 70;
            def_value = 0;

            int pause = int(pk::option["MessagePause"]);
            pk::option["MessagePause"] = 100;

            bool next = true;


            if (answer == 0)
            {
                for (int num = min_value; num <= max_value; num++)
                {
                    if (next)
                    {
                        bool confirm = true;
                        while (confirm)
                        {
                            pk::play_voice(num, selected_person_);
                            confirm = pk::yes_no(pk::encode("是否要\x1b[1x重听\x1b[0x一次?（多语音随机）"));
                        }

                        next = pk::yes_no(pk::encode(pk::format("语音\x1b[1x{}\x1b[0x运行完成\n是否要继续听下去?", num)));
                    }
                }
            }

            if (answer == 1)
            {
                for (int num = min_value; num <= max_value; num++)
                {
                    if (next)
                    {
                        pk::play_voice(num, selected_person_);
                        next = pk::yes_no(pk::encode(pk::format("语音\x1b[1x{}\x1b[0x运行完成\n是否要继续听下去?", num)));
                    }                 
                }
            }

            if (answer == 2)
            {
                while (next)
                {
                    def_value = pk::numberpad(pk::encode("语音编号"), min_value, max_value, def_value, pk::numberpad_t(pad_callback)).first;
                    bool confirm = true;
                    while (confirm)
                    {
                        pk::play_voice(def_value, selected_person_);
                        confirm = pk::yes_no(pk::encode("是否要\x1b[1x重听\x1b[0x一次?（多语音随机）"));
                    }

                    next = pk::yes_no(pk::encode(pk::format("语音\x1b[1x{}\x1b[0x运行完成\n是否要继续听下去?", def_value)));

                }
            }

            pk::option["MessagePause"] = pause;
        }
    }

    Main main;
}
// ## 2020/07/26 ##
/*
@ 만든이: 기마책사
@ UPDATE: '18.12.27  / 최초적용
*/

namespace BGM플레이어
{

    class Main
    {
        int menu_num;
        int min_value;
        int max_value;
        int def_value;

        Main()
        {
            add_menu();
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


        //------------------------------------------------------------

        string pad_callback(int line, int original_value, int current_value)
        {
            return pk::encode("输入数字");  // 텍스트 표시기능인 듯 하나, 사용방법 모르겠음
        }

        //------------------------------------------------------------

        string getText()
        {
            return pk::encode("BGM Player");
        }

        string getDesc()
        {
            return pk::encode("播放选中的BGM");
        }

        bool isEnabled()
        {
            return true;
        }

        bool handler()
        {
            min_value = 1;
            max_value = 24;
            def_value = 1;

            int pause = int(pk::option["MessagePause"]);
            pk::option["MessagePause"] = 100;

            bool next = true;
            while (next)
            {
                pk::int_bool numpad = pk::numberpad(pk::encode("BGM号码"), min_value, max_value, def_value, pk::numberpad_t(pad_callback));
                if (!numpad.second)
                {
                    pk::option["MessagePause"] = pause;
                    return false;   // 취소종료
                }

                def_value = numpad.first;

                pk::scene(pk::scene_t(scene_bgm));

                next = pk::yes_no(pk::encode(pk::format("BGM正在播放第\x1b[1x{}首\x1b[0x.\n选择其他BGM?", def_value)));
            }

            pk::option["MessagePause"] = pause;

            return true;
        }

        void scene_bgm()
        {
            pk::play_bgm(def_value - 1, true, 100);    // 선택한 BGM 재생
        }

    }

    Main main;
}
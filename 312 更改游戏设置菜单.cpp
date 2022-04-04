// ## 2021/10/28 # 江东新风 # 单港关存活设定变更 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2021/03/06 # 江东新风 # 修复xml复制黏贴导致不识别错误 ##
// ## 2021/01/26 # 江东新风 # 加入单港关存活开关 ##
// ## 2021/01/16 # 江东新风 # 修复因测试引起的bug ##
// ## 2020/08/01 ##
/*
@ 만든이: 기마책사
@ UPDATE: '19.5.26   / 최초적용 : 게임 설정변경 메뉴 (사실/가상, 난이도 등)
@ UPDATE: '19.11.10  / 변경적용 : 초특기연전 설정변경 시 특기설명 변경되도록 수정
*/

namespace 플레이설정변경메뉴
{

    class Main
    {

        Main()
        {
            add_menu();
        }


        void add_menu()
        {
            pk::menu_item menu_top;
            menu_top.menu = 2;
            menu_top.init = pk::menu_item_init_t(init);
            menu_top.get_text = pk::menu_item_get_text_t(getText_상위메뉴);
            menu_top.get_desc = pk::menu_item_get_desc_t(getDesc_상위메뉴);
            menu_num = pk::add_menu_item(menu_top);


            pk::menu_item menu_난이도;
            menu_난이도.menu = menu_num;
            menu_난이도.get_text = pk::menu_item_get_text_t(getText_난이도);
            menu_난이도.get_desc = pk::menu_item_get_desc_t(getDesc_난이도);
            menu_난이도.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            menu_난이도.handler = pk::menu_item_handler_t(handler_난이도);
            pk::add_menu_item(menu_난이도);

            pk::menu_item menu_전사빈도;
            menu_전사빈도.menu = menu_num;
            menu_전사빈도.get_text = pk::menu_item_get_text_t(getText_전사빈도);
            menu_전사빈도.get_desc = pk::menu_item_get_desc_t(getDesc_전사빈도);
            menu_전사빈도.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            menu_전사빈도.handler = pk::menu_item_handler_t(handler_전사빈도);
            pk::add_menu_item(menu_전사빈도);

            pk::menu_item menu_요술낙뢰;
            menu_요술낙뢰.menu = menu_num;
            menu_요술낙뢰.get_text = pk::menu_item_get_text_t(getText_요술낙뢰);
            menu_요술낙뢰.get_desc = pk::menu_item_get_desc_t(getDesc_요술낙뢰);
            menu_요술낙뢰.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            menu_요술낙뢰.handler = pk::menu_item_handler_t(handler_요술낙뢰);
            pk::add_menu_item(menu_요술낙뢰);

            pk::menu_item menu_적부대알림;
            menu_적부대알림.menu = menu_num;
            menu_적부대알림.get_text = pk::menu_item_get_text_t(getText_적부대알림);
            menu_적부대알림.get_desc = pk::menu_item_get_desc_t(getDesc_적부대알림);
            menu_적부대알림.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            menu_적부대알림.handler = pk::menu_item_handler_t(handler_적부대알림);
            pk::add_menu_item(menu_적부대알림);

            pk::menu_item menu_위임군단제어;
            menu_위임군단제어.menu = menu_num;
            menu_위임군단제어.get_text = pk::menu_item_get_text_t(getText_위임군단제어);
            menu_위임군단제어.get_desc = pk::menu_item_get_desc_t(getDesc_위임군단제어);
            menu_위임군단제어.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            menu_위임군단제어.handler = pk::menu_item_handler_t(handler_위임군단제어);
            pk::add_menu_item(menu_위임군단제어);

            pk::menu_item menu_산악이동;
            menu_산악이동.menu = menu_num;
            menu_산악이동.get_text = pk::menu_item_get_text_t(getText_산악이동);
            menu_산악이동.get_desc = pk::menu_item_get_desc_t(getDesc_산악이동);
            menu_산악이동.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            menu_산악이동.handler = pk::menu_item_handler_t(handler_산악이동);
            pk::add_menu_item(menu_산악이동);

            pk::menu_item menu_물가이동;
            menu_물가이동.menu = menu_num;
            menu_물가이동.get_text = pk::menu_item_get_text_t(getText_물가이동);
            menu_물가이동.get_desc = pk::menu_item_get_desc_t(getDesc_물가이동);
            menu_물가이동.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            menu_물가이동.handler = pk::menu_item_handler_t(handler_물가이동);
            pk::add_menu_item(menu_물가이동);

            pk::menu_item menu_일기토교대;
            menu_일기토교대.menu = menu_num;
            menu_일기토교대.get_text = pk::menu_item_get_text_t(getText_일기토교대);
            menu_일기토교대.get_desc = pk::menu_item_get_desc_t(getDesc_일기토교대);
            menu_일기토교대.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            menu_일기토교대.handler = pk::menu_item_handler_t(handler_일기토교대);
            pk::add_menu_item(menu_일기토교대);

            pk::menu_item menu_초특기연전;
            menu_초특기연전.menu = menu_num;
            menu_초특기연전.get_text = pk::menu_item_get_text_t(getText_초특기연전);
            menu_초특기연전.get_desc = pk::menu_item_get_desc_t(getDesc_초특기연전);
            menu_초특기연전.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            menu_초특기연전.handler = pk::menu_item_handler_t(handler_초특기연전);
            pk::add_menu_item(menu_초특기연전);

            pk::menu_item menu_기사공격숲제외;
            menu_기사공격숲제외.menu = menu_num;
            menu_기사공격숲제외.get_text = pk::menu_item_get_text_t(getText_기사공격숲제외);
            menu_기사공격숲제외.get_desc = pk::menu_item_get_desc_t(getDesc_기사공격숲제외);
            menu_기사공격숲제외.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            menu_기사공격숲제외.handler = pk::menu_item_handler_t(handler_기사공격숲제외);
            pk::add_menu_item(menu_기사공격숲제외);

            pk::menu_item menu_내정시설3단;
            menu_내정시설3단.menu = menu_num;
            menu_내정시설3단.get_text = pk::menu_item_get_text_t(getText_내정시설3단);
            menu_내정시설3단.get_desc = pk::menu_item_get_desc_t(getDesc_내정시설3단);
            menu_내정시설3단.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            menu_내정시설3단.handler = pk::menu_item_handler_t(handler_내정시설3단);
            pk::add_menu_item(menu_내정시설3단);

            pk::menu_item menu_单港关存活;
            menu_单港关存活.menu = menu_num;
            menu_单港关存活.is_visible = pk::menu_item_is_visible_t(is_visible_单港关存活);
            menu_单港关存活.get_image_id = pk::menu_item_get_image_id_t(get_image_id);
            menu_单港关存活.get_text = pk::menu_item_get_text_t(getText_单港关存活);
            menu_单港关存活.get_desc = pk::menu_item_get_desc_t(getDesc_单港关存活);
            menu_单港关存活.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
            menu_单港关存活.handler = pk::menu_item_handler_t(handler_单港关存活);
            pk::add_menu_item(menu_单港关存活);
        }

        int menu_num;
        pk::scenario@ scenario;

        pk::person@ kunshu;


        //---------------------------------------------------------------------------------------

        void init()
        {
            @scenario = pk::get_scenario();
            @kunshu = pk::get_person(pk::get_force(pk::get_current_turn_force_id()).kunshu);
        }

        string getText_상위메뉴()
        {
            return pk::encode("更改游戏设置");
        }

        string getDesc_상위메뉴()
        {
            return pk::encode("在游戏中更改设置");
        }

        bool isEnabled()
        {
            return true;
        }


        //---------------------------------------------------------------------------------------
        // 난이도 설정 메뉴
        //---------------------------------------------------------------------------------------

        string getText_난이도()
        {
            return pk::encode("设置难度");
        }

        string getDesc_난이도()
        {
            return pk::encode("在游戏中更改难度设置");
        }

        bool handler_난이도()
        {
            pk::array<string> arr_settings = { "初级", "上级", "超级" };

            int value = scenario.difficulty;
            int answer = pk::choose({ pk::encode("初级"), pk::encode("上级"), pk::encode("超级"), pk::encode("取消") },
                pk::encode(pk::format("当前难度为\x1b[1x{}\x1b[0x.\n是否要更换难度?", arr_settings[value])), kunshu);

            if (answer >= 3) return false;

            scenario.difficulty = answer;
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x难度设置完毕!!", arr_settings[answer])), kunshu);

            return true;
        }


        //---------------------------------------------------------------------------------------
        // 전사 설정 메뉴
        //---------------------------------------------------------------------------------------

        string getText_전사빈도()
        {
            return pk::encode("战死频率");
        }

        string getDesc_전사빈도()
        {
            return pk::encode("更改游戏中的战死频率");
        }

        bool handler_전사빈도()
        {
            pk::array<string> arr_settings = { "无", "标准", "频繁" };

            int value = scenario.battle_death;
            int answer = pk::choose({ pk::encode("无"), pk::encode("标准"), pk::encode("频繁"), pk::encode("取消") },
                pk::encode(pk::format("目前战死频率为\x1b[1x{}\x1b[0x.\n是否要更改战死频率?", arr_settings[value])), kunshu);

            if (answer >= 3) return false;

            scenario.battle_death = answer;
            pk::message_box(pk::encode(pk::format("战死频率\x1b[1x{}\x1b[0x设置完毕!!", arr_settings[answer])), kunshu);

            return true;
        }


        //---------------------------------------------------------------------------------------
        // 요술낙뢰 설정 메뉴
        //---------------------------------------------------------------------------------------

        string getText_요술낙뢰()
        {
            return pk::encode("妖术落雷");
        }

        string getDesc_요술낙뢰()
        {
            return pk::encode("更改妖术落雷的使用设置");
        }

        bool handler_요술낙뢰()
        {
            pk::array<string> arr_settings = { "有效", "无效" };

            int value = scenario.magic;
            int answer = pk::choose({ pk::encode("有效"), pk::encode("无效"), pk::encode("取消") },
                pk::encode(pk::format("目前妖术落雷为\x1b[1x{}\x1b[0x.\n是否要更改妖术落雷的设置?", arr_settings[value])), kunshu);

            if (answer >= 2) return false;

            scenario.magic = answer;
            pk::message_box(pk::encode(pk::format("使用妖术落雷\x1b[1x{}\x1b[0x设置完毕!!", arr_settings[answer])), kunshu);

            return true;
        }


        //---------------------------------------------------------------------------------------
        // 적부대 출진 알림 설정 메뉴
        //---------------------------------------------------------------------------------------

        string getText_적부대알림()
        {
            return pk::encode("敌方部队出阵提示");
        }

        string getDesc_적부대알림()
        {
            return pk::encode("更改敌方部队出阵提示设置");
        }

        bool handler_적부대알림()
        {
            pk::array<string> arr_settings = { "打开", "关闭" };

            bool value = bool(pk::core["attack_alert"]);

            int answer = pk::choose({ pk::encode("打开"), pk::encode("关闭"), pk::encode("取消") },
                pk::encode(pk::format("当前敌方部队出阵提示为\x1b[1x{}\x1b[0x.\n是否更改提示设置?", ((value) ? arr_settings[0] : arr_settings[1]))), kunshu);

            if (answer >= 2) return false;

            else if (answer == 0) pk::core["attack_alert"] = true;
            else if (answer == 1) pk::core["attack_alert"] = false;

            value = bool(pk::core["attack_alert"]);

            pk::message_box(pk::encode(pk::format("敌方部队出阵提示\x1b[1x{}\x1b[0x!!\n此更改仅在此次游戏中生效，如需永久更改请使用KRE", ((value) ? arr_settings[0] : arr_settings[1]))), kunshu);

            return true;
        }


        //---------------------------------------------------------------------------------------
        // 위임군단제어 메뉴
        //---------------------------------------------------------------------------------------

        string getText_위임군단제어()
        {
            return pk::encode("控制委任军团");
        }

        string getDesc_위임군단제어()
        {
            return pk::encode("更改委任军团控制设置");
        }

        bool handler_위임군단제어()
        {
            pk::array<string> arr_settings = { "允许", "禁止" };

            bool value = bool(pk::core["sub_district_controls"]);

            int answer = pk::choose({ pk::encode("允许"), pk::encode("禁止"), pk::encode("取消") },
                pk::encode(pk::format("当前委任军团控制设置为\x1b[1x{}\x1b[0x.\n是否更改配置?", ((value) ? arr_settings[0] : arr_settings[1]))), kunshu);

            if (answer >= 2) return false;
            else if (answer == 0) pk::core["sub_district_controls"] = true;
            else if (answer == 1) pk::core["sub_district_controls"] = false;

            pk::message_box(pk::encode(pk::format("委任军团控制\x1b[1x{}\x1b[0x设置完毕!!\n此更改仅在此次游戏中生效，如需永久更改请使用KRE", arr_settings[answer])), kunshu);

            return true;
        }


        //---------------------------------------------------------------------------------------
        // 산악 이동 허용 메뉴
        //---------------------------------------------------------------------------------------

        string getText_산악이동()
        {
            return pk::encode("山地移动");
        }

        string getDesc_산악이동()
        {
            return pk::encode("更改山地移动设置");
        }

        bool handler_산악이동()
        {
            pk::array<string> arr_settings = { "允许", "禁止" };

            bool value = bool(pk::core["terrain"][地形_崖]["movabale"]);

            int answer = pk::choose({ pk::encode("允许"), pk::encode("禁止"), pk::encode("取消") },
                pk::encode(pk::format("目前山地移动为\x1b[1x{}\x1b[0x.\n是否更改设置?", ((value) ? arr_settings[0] : arr_settings[1]))), kunshu);

            //bool(pk::core["지형"][지형_산]["이동가능"]) = false;
            if (answer >= 2) return false;
            else if (answer == 0) pk::set_terrain_movable(地形_崖,true);//pk::core["terrain"][地形_崖]["movabale"] = true;
            else if (answer == 1) pk::set_terrain_movable(地形_崖, false); //pk::core["terrain"][地形_崖]["movabale"] = false;

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x山地移动设置完毕!!\n默认为DATA文件夹其他地形.需要修改xml\n需要在xml确定不同兵种的移动力", arr_settings[answer])), kunshu);

            return true;
        }


        //---------------------------------------------------------------------------------------
        // 물가 이동 허용 메뉴
        //---------------------------------------------------------------------------------------

        string getText_물가이동()
        {
            return pk::encode("水岸移动");
        }

        string getDesc_물가이동()
        {
            return pk::encode("更改水岸移动设置");
        }

        bool handler_물가이동()
        {
            pk::array<string> arr_settings = { "允许", "禁止" };

            bool value = bool(pk::core["terrain"][地形_岸]["movabale"]);

            int answer = pk::choose({ pk::encode("允许"), pk::encode("禁止"), pk::encode("取消") },
                pk::encode(pk::format("目前水岸移动为\x1b[1x{}\x1b[0x.\n是否更改设置?", ((value) ? arr_settings[0] : arr_settings[1]))), kunshu);

            if (answer >= 2) return false;
            else if (answer == 0) pk::core["terrain"][地形_岸]["movabale"] = true;
            else if (answer == 1) pk::core["terrain"][地形_岸]["movabale"] = false;

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x水岸移动设置完毕!!\n默认为DATA文件夹其他地形.需要修改xml\n需要在xml确定不同兵种的移动力", arr_settings[answer])), kunshu);

            return true;
        }


        //---------------------------------------------------------------------------------------
        // 일기토 교대 설정 메뉴
        //---------------------------------------------------------------------------------------

        string getText_일기토교대()
        {
            return pk::encode("单挑轮换");
        }

        string getDesc_일기토교대()
        {
            return pk::encode("更改单挑轮换设置");
        }

        bool handler_일기토교대()
        {
            pk::array<string> arr_settings = { "允许", "禁止" };
            bool value = bool(pk::core["duel.one_on_one"]);

            int answer = pk::choose({ pk::encode("允许"), pk::encode("禁止"), pk::encode("取消") },
                pk::encode(pk::format("当前单挑轮换设置为\x1b[1x{}轮换\x1b[0x.\n是否更改配置?", ((value) ? arr_settings[1] : arr_settings[0]))), kunshu);

            if (answer >= 2) return false;
            else if (answer == 0) pk::core["duel.one_on_one"] = false;
            else if (answer == 1) pk::core["duel.one_on_one"] = true;

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x单挑轮换设置完毕!!\n此更改仅在此次游戏中生效，如需永久更改请使用KRE", arr_settings[answer])), kunshu);

            return true;
        }


        //---------------------------------------------------------------------------------------
        // 초특기 연전 설정 메뉴
        //---------------------------------------------------------------------------------------

        string getText_초특기연전()
        {
            return pk::encode("战法连击");
        }

        string getDesc_초특기연전()
        {
            return pk::encode("更改战法连击的设置");
        }

        bool handler_초특기연전()
        {
            pk::array<string> arr_settings = { "有效", "无效" };
            bool value = bool(pk::core["tactics_rensen"]);

            int answer = pk::choose({ pk::encode("有效"), pk::encode("无效"), pk::encode("取消") },
                pk::encode(pk::format("当前设置为\x1b[1x连战对战法{}\x1b[0x.\n是否更改配置?", ((value) ? arr_settings[1] : arr_settings[0]))), kunshu);

            if (answer >= 2) return false;
            else if (answer == 0)
            {
                pk::core["tactics_rensen"] = true;
                pk::get_skill(특기_연전).desc = pk::encode("普通和战法攻击时有50%的概率再攻击一次");
            }
            else if (answer == 1)
            {
                pk::core["tactics_rensen"] = false;
                pk::get_skill(특기_연전).desc = pk::encode("普通攻击时有50%的概率再攻击一次");
            }

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x战法连击设置完毕!!\n此更改仅在此次游戏中生效，如需永久更改请使用KRE", arr_settings[answer])), kunshu);

            return true;
        }


        //---------------------------------------------------------------------------------------
        // 기사공격숲제외 건설 설정 메뉴
        //---------------------------------------------------------------------------------------

        string getText_기사공격숲제외()
        {
            return pk::encode("骑兵仅骑射森林外");
        }

        string getDesc_기사공격숲제외()
        {
            return pk::encode("更改骑兵仅骑射森林外建筑的设置");
        }

        bool handler_기사공격숲제외()
        {
            pk::array<string> arr_settings = { "打开", "关闭" };

            bool value = bool(pk::core["kisha_no_forest"]);

            int answer = pk::choose({ pk::encode("打开"), pk::encode("关闭"), pk::encode("取消") },
                pk::encode(pk::format("当前骑兵无法骑射森林中部队为\x1b[1x{}\x1b[0x.\n是否更改配置?", ((value) ? arr_settings[0] : arr_settings[1]))), kunshu);

            if (answer >= 2) return false;

            else if (answer == 0) pk::core["kisha_no_forest"] = true;
            else if (answer == 1) pk::core["kisha_no_forest"] = false;

            value = bool(pk::core["kisha_no_forest"]);

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x骑兵无法骑射森林中部队!!\n此更改仅在此次游戏中生效，如需永久更改请使用KRE", ((value) ? arr_settings[0] : arr_settings[1]))), kunshu);

            return true;
        }


        //---------------------------------------------------------------------------------------
        // 내정시설3단 건설 설정 메뉴
        //---------------------------------------------------------------------------------------

        string getText_내정시설3단()
        {
            return pk::encode("内政设施3级");
        }

        string getDesc_내정시설3단()
        {
            return pk::encode("更改内政设施3级建设");
        }

        bool handler_내정시설3단()
        {
            pk::array<string> arr_settings = { "打开", "关闭" };

            bool value = bool(pk::core["facility_to_level3"]);

            int answer = pk::choose({ pk::encode("打开"), pk::encode("关闭"), pk::encode("取消") },
                pk::encode(pk::format("当前内政设施3级建设为\x1b[1x{}\x1b[0x.\n是否更改设置?", ((value) ? arr_settings[0] : arr_settings[1]))), kunshu);

            if (answer >= 2) return false;

            else if (answer == 0) pk::core["facility_to_level3"] = true;
            else if (answer == 1) pk::core["facility_to_level3"] = false;

            value = bool(pk::core["facility_to_level3"]);

            pk::message_box(pk::encode(pk::format("已\x1b[1x{}\x1b[0x内政设施3级建设!!\n此更改仅在此次游戏中生效，如需永久更改请使用KRE", ((value) ? arr_settings[0] : arr_settings[1]))), kunshu);

            return true;
        }



        //---------------------------------------------------------------------------------------

        string getText_单港关存活()
        {
            //if (pk::get_scenario().no == 4)
            //settinginfo@ set0 = @setting_ex;
            //if (set0.set_port_gate_survive and set0.enable_port_gate_survive) return pk::encode("单港关存活(on)");
            if (pk::get_onebase_survive()) return pk::encode("单港关存活(on)");
            else return pk::encode("单港关存活(off)");
        }

        string getDesc_单港关存活()
        {
            //settinginfo@ set0 = @setting_ex;
            //pk::trace(pk::format("{}{}", set0.set_port_gate_survive, set0.enable_port_gate_survive));
            //if (set0.set_port_gate_survive and set0.enable_port_gate_survive) return pk::encode("关闭单港关存活，目前为开启");
            if (pk::get_onebase_survive()) return pk::encode("关闭单港关存活，目前为开启");
            else return pk::encode("开启单港关存活，目前为关闭");
        }

        int get_image_id()
        {
            return pk::get_onebase_survive() ? 1249 : 889;
        }

        bool is_visible_单港关存活()
        {
            return (pk::get_scenario().no == 4);
        }

        bool handler_单港关存活()
        {

            bool value = pk::get_onebase_survive();
            pk::set_onebase_survive(!value);
            settinginfo@ set0 = @setting_ex;
            set0.set_port_gate_survive = !value;
            set0.enable_port_gate_survive = !value;

            return true;
        }
    }

    Main main;
}
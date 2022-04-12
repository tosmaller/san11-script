// ## 2021/09/26 # 江东新风 # 事件手动触发的一级菜单 ##
// ## 2021/01/18 # 江东新风 # 禁止拆建拼音改简体 ##
// ## 2020/12/15 # messi # 内政自动化改为太守自治 ##
// ## 2020/08/11 # 氕氘氚 # 修复太守不在时点击买卖可能出现的指針錯誤 ##
// ## 2020/08/08 # 氕氘氚 # 新增三个一级菜单 ##


namespace global_menu
{
    // ======================================================================

    const bool  군단명령_군단거점출병 = true;    // 출진조건 만족하는 거점들을 일괄출진 메뉴 활성화
    const bool  군단명령_징병순찰훈련 = true;    // 징병,순찰,훈련 등을 일괄실행 하는 메뉴군 활성화
    const bool  군단명령_군단병장생산 = true;    // 병장생산(단야,구사,공방,조선)을 일괄실행하는 메뉴 활성화
    const bool  군단명령_군단선택메뉴 = true;    // [군단장] 메뉴 사용여부 : 군단장 소속 거점으로 화면이동

    const bool  군단명령_위임군단적용 = true;   // 对委任军团有效
    const bool  군단명령_도시실행확인 = false;   // 显示詢問确认执行提示

    const string shortcut_军团 = "q";
    const string shortcut_买卖 = "w";
    const string shortcut_国事 = "e";
    const string shortcut_个人 = "r";
    const string shortcut_培养 = "t";
    const string shortcut_自动化 = "y";

    const string shortcut_军团长 = "q";
    const string shortcut_军团军事 = "w";
    const string shortcut_据点内政 = "e";
    const string shortcut_军团内政 = "r";
    const string shortcut_军团生chan = "t";
    const string shortcut_禁止拆建 = "y";

    const string shortcut_护送付虏 = "Q";
    const string shortcut_平障建设 = "W";
    const string shortcut_城墙维修 = "E";
    const string shortcut_焚城 = "R";

    const string shortcut_选拔 = "Q";
    const string shortcut_寻宝 = "W";
    const string shortcut_仙人探索 = "E";
    const string shortcut_连续探索 = "R";

    const string shortcut_成立联军 = "8";

    // ======================================================================

    int 菜单_军团 = -1;
    int 菜单_国事 = -1;
    int 菜单_个人 = -1;
    int 菜单_培养 = -1;
    int 菜单_买卖 = -1;
    int 菜单_自动化 = -1;
	int 菜单_事件 = -1;					   

    int 菜单_部队 = -1;

    int 买卖_展开_当前 = 0;
    const int 买卖_展开_兵器购买 = 1;
    const int 买卖_展开_兵器贩卖 = 2;
    const int 买卖_展开_兵装购买 = 3;
    const int 买卖_展开_兵装贩卖 = 4;

    int 培养_展开_当前 = 0;
    const int 培养_展开_强化战术 = 1;
    const int 培养_展开_脱胎換骨 = 2;

    int 自动化_展开_当前 = 0;
    const int 自动化_展开_顺序 = 1;
    const int 自动化_展开_许可 = 2;
    const int 自动化_展开_标准 = 3;
<<<<<<< HEAD


=======
    const int 自动化_展开_生产 = 4;
    const int 自动化_展开_负责 = 5;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
    // ======================================================================


    class Main
    {

        pk::building@ building_;
        pk::force@ force_;
        pk::person@ taishu_;
        pk::person@ kunshu_;
        pk::city@ city_;
        pk::gate@ gate_;
        pk::port@ port_;
        pk::district@ district_;

        int menu_city_id_;
        int menu_force_id_;


        Main()
        {
            if (군단명령_징병순찰훈련 or 군단명령_군단병장생산 or 군단명령_군단거점출병)
            {
                pk::menu_item menu_item_军团;
                menu_item_军团.menu = 0;
                menu_item_军团.pos = 0;
                menu_item_军团.shortcut = shortcut_军团;
                menu_item_军团.init = pk::building_menu_item_init_t(init_菜单_军团);
                menu_item_军团.is_visible = pk::menu_item_is_visible_t(isVisible_菜单_军团);
                menu_item_军团.is_enabled = pk::menu_item_is_enabled_t(isEnabled_菜单_军团);
                menu_item_军团.get_text = pk::menu_item_get_text_t(getText_菜单_军团);
                menu_item_军团.get_desc = pk::menu_item_get_desc_t(getDesc_菜单_军团);
                菜单_军团 = pk::add_menu_item(menu_item_军团);
            }

            pk::menu_item menu_item_买卖;
            menu_item_买卖.menu = 0;
            menu_item_买卖.pos = 7;
            menu_item_买卖.shortcut = shortcut_买卖;
            menu_item_买卖.init = pk::building_menu_item_init_t(init_菜单_买卖);
            menu_item_买卖.is_visible = pk::menu_item_is_visible_t(isVisible_菜单_买卖);
            menu_item_买卖.is_enabled = pk::menu_item_is_enabled_t(isEnabled_菜单_买卖);
            menu_item_买卖.get_text = pk::menu_item_get_text_t(getText_菜单_买卖);
            menu_item_买卖.get_desc = pk::menu_item_get_desc_t(getDesc_菜单_买卖);
            菜单_买卖 = pk::add_menu_item(menu_item_买卖);

            pk::menu_item menu_item_国事;
            menu_item_国事.menu = 0;
            menu_item_国事.pos = 8;
            menu_item_国事.shortcut = shortcut_国事;
            menu_item_国事.init = pk::building_menu_item_init_t(init_菜单_国事);
            menu_item_国事.is_visible = pk::menu_item_is_visible_t(isVisible_菜单_国事);
            menu_item_国事.is_enabled = pk::menu_item_is_enabled_t(isEnabled_菜单_国事);
            menu_item_国事.get_text = pk::menu_item_get_text_t(getText_菜单_国事);
            menu_item_国事.get_desc = pk::menu_item_get_desc_t(getDesc_菜单_国事);
            菜单_国事 = pk::add_menu_item(menu_item_国事);
            //ch::u8debug(pk::format("guo shi menu {}", 菜单_国事));

            pk::menu_item menu_item_个人;
            menu_item_个人.menu = 0;
            menu_item_个人.pos = 9;
            menu_item_个人.shortcut = shortcut_个人;
            menu_item_个人.init = pk::building_menu_item_init_t(init_菜单_个人);
            menu_item_个人.is_visible = pk::menu_item_is_visible_t(isVisible_菜单_个人);
            menu_item_个人.is_enabled = pk::menu_item_is_enabled_t(isEnabled_菜单_个人);
            menu_item_个人.get_text = pk::menu_item_get_text_t(getText_菜单_个人);
            menu_item_个人.get_desc = pk::menu_item_get_desc_t(getDesc_菜单_个人);
            菜单_个人 = pk::add_menu_item(menu_item_个人);
            //ch::u8debug(pk::format("ge ren menu {}", 菜单_个人));

            pk::menu_item menu_item_培养;
            menu_item_培养.menu = 0;
            menu_item_培养.pos = 10;
            menu_item_培养.shortcut = shortcut_培养;
            menu_item_培养.init = pk::building_menu_item_init_t(init_菜单_培养);
            menu_item_培养.is_visible = pk::menu_item_is_visible_t(isVisible_菜单_培养);
            menu_item_培养.is_enabled = pk::menu_item_is_enabled_t(isEnabled_菜单_培养);
            menu_item_培养.get_text = pk::menu_item_get_text_t(getText_菜单_培养);
            menu_item_培养.get_desc = pk::menu_item_get_desc_t(getDesc_菜单_培养);
            菜单_培养 = pk::add_menu_item(menu_item_培养);

            pk::menu_item menu_item_事件;
            menu_item_事件.menu = 0;
            menu_item_事件.pos = 10;
            menu_item_事件.init = pk::building_menu_item_init_t(init_菜单_事件);
            menu_item_事件.is_visible = pk::menu_item_is_visible_t(isVisible_菜单_事件);
            menu_item_事件.is_enabled = pk::menu_item_is_enabled_t(isEnabled_菜单_事件);
            menu_item_事件.get_text = pk::menu_item_get_text_t(getText_菜单_事件);
            menu_item_事件.get_desc = pk::menu_item_get_desc_t(getDesc_菜单_事件);
            菜单_事件 = pk::add_menu_item(menu_item_事件);

            pk::menu_item menu_item_自动化;
            menu_item_自动化.menu = 0;
            menu_item_自动化.pos = 11;
            menu_item_自动化.shortcut = shortcut_自动化;
            menu_item_自动化.init = pk::building_menu_item_init_t(init_菜单_自动化);
            menu_item_自动化.is_visible = pk::menu_item_is_visible_t(isVisible_菜单_自动化);
            menu_item_自动化.is_enabled = pk::menu_item_is_enabled_t(isEnabled_菜单_自动化);
            menu_item_自动化.get_text = pk::menu_item_get_text_t(getText_菜单_自动化);
            menu_item_自动化.get_desc = pk::menu_item_get_desc_t(getDesc_菜单_自动化);
            菜单_自动化 = pk::add_menu_item(menu_item_自动化);

            // =============================================================================================
            // 部队菜单======================================================================================

            pk::menu_item menu_item_部队;

            menu_item_部队.menu = 1;
            menu_item_部队.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("部队"); });  //budui
            菜单_部队 = pk::add_menu_item(menu_item_部队);


        }

        // =============================================================================================

        void init_菜单_军团(pk::building@ building)
        {
            @building_ = @building;
            @force_ = pk::get_force(building.get_force_id());
            if (building_.facility <= 시설_항구)
            {
                @district_ = pk::get_district(building.get_district_id());
            }
        }

        bool isVisible_菜单_军团()
        {
            if (pk::is_campaign()) return false;
            if (building_.get_id() > 41) return false;
            if (!군단명령_위임군단적용 and district_.no > 1) return false;
            return true;
        }

        string getText_菜单_军团()
        {
            return pk::encode("军团命令");
        }

        bool isEnabled_菜单_军团()
        {
            if (building_.get_id() > 41) return false;
            return true;
        }

        string getDesc_菜单_军团()
        {
            return pk::encode("执行军团命令");
        }

        // =============================================================================================

        void init_菜单_国事(pk::building@ building)
        {
            @building_ = @building;
            @force_ = pk::get_force(building.get_force_id());
            @taishu_ = pk::get_person(pk::get_taishu_id(building));
            @kunshu_ = pk::get_person(force_.kunshu);

            if ((building_.get_id() <= 41) and (building_.get_id() >= 0))
                @city_ = pk::building_to_city(building);
        }

        bool isVisible_菜单_国事()
        {
            if (pk::is_campaign()) return false;
            if (building_.get_id() > 41) return false;
            if (building_.get_id() != kunshu_.service) return false;
            return true;
        }

        string getText_菜单_国事()
        {
            return pk::encode("国事");
        }

        bool isEnabled_菜单_国事()
        {
            if (building_.get_id() >= 据点_末) return false;
            if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_)) return false;
            if (kunshu_.action_done == true) return false;
            return true;
        }

        string getDesc_菜单_国事()
        {
            if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_))
                return pk::encode("君主未在城中.");
            else if (kunshu_.action_done == true)
                return pk::encode("君主已完成行动.");
            else
                return pk::encode("执行国事相关指令.");
        }

        // =============================================================================================

        void init_菜单_个人(pk::building@ building)
        {
            @building_ = @building;
            @force_ = pk::get_force(building.get_force_id());
            @taishu_ = pk::get_person(pk::get_taishu_id(building));
            @kunshu_ = pk::get_person(force_.kunshu);

            if ((building_.get_id() <= 41) and (building_.get_id() >= 0))
                @city_ = pk::building_to_city(building);
            else if ((building_.get_id() <= 51) and (building_.get_id() >= 42))
                @gate_ = pk::building_to_gate(building);
            else if ((building_.get_id() <= 86) and (building_.get_id() >= 52))
                @port_ = pk::building_to_port(building);
        }

        bool isVisible_菜单_个人()
        {
            if (pk::is_campaign()) return false;
            if (building_.get_id() >= 据点_末) return false;
            if (building_.get_id() != kunshu_.service) return false;
            return true;
        }

        string getText_菜单_个人()
        {
            return pk::encode("个人");
        }

        bool isEnabled_菜单_个人()
        {
            if (building_.get_id() >= 据点_末) return false;
            if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_)) return false;
            // if (kunshu_.action_done == true) return false;   //和修养矛盾
            return true;
        }

        string getDesc_菜单_个人()
        {
            if (pk::is_absent(kunshu_) or pk::is_unitize(kunshu_))
                return pk::encode("君主未在城中.");
            // else if (kunshu_.action_done == true)
            //     return pk::encode("君主已完成行动.");
            else
                return pk::encode("执行君主个人行动.");
        }

        // =============================================================================================

        void init_菜单_培养(pk::building@ building)
        {
            @building_ = @building;
            @taishu_ = pk::get_person(pk::get_taishu_id(building));
        }

        bool isVisible_菜单_培养()
        {
            if (pk::is_campaign()) return false;
            if (building_.get_id() >= 据点_末) return false;
            return true;
        }

        string getText_菜单_培养()
        {
            return pk::encode("培养");
        }

        bool isEnabled_菜单_培养()
        {
            if (building_.get_id() >= 据点_末) return false;
            return true;
        }

        string getDesc_菜单_培养()
        {
            return pk::encode("执行人才培养指令.");
        }

        // =============================================================================================

        void init_菜单_事件(pk::building@ building)
        {
            @building_ = @building;
            @taishu_ = pk::get_person(pk::get_taishu_id(building));
            @kunshu_ = pk::get_person(force_.kunshu);
        }

        bool isVisible_菜单_事件()
        {
            if (pk::is_campaign()) return false;
            if (building_.get_id() >= 据点_末) return false;
            if (building_.get_id() != kunshu_.service) return false;
            return true;
        }

        string getText_菜单_事件()
        {
            return pk::encode("事件");
        }

        bool isEnabled_菜单_事件()
        {
            if (building_.get_id() >= 据点_末) return false;
            return true;
        }

        string getDesc_菜单_事件()
        {
            return pk::encode("决定触发事件.");
        }
        // =============================================================================================

        void init_菜单_买卖(pk::building@ building)
        {
            @building_ = @building;
            @taishu_ = pk::get_person(pk::get_taishu_id(building));

            if ((building_.get_id() <= 41) and (building_.get_id() >= 0))
                @city_ = pk::building_to_city(building);
            else if ((building_.get_id() <= 51) and (building_.get_id() >= 42))
                @gate_ = pk::building_to_gate(building);
            else if ((building_.get_id() <= 86) and (building_.get_id() >= 52))
                @port_ = pk::building_to_port(building);
        }

        bool isVisible_菜单_买卖()
        {
            if (pk::is_campaign()) return false;
            if (building_.get_id() >= 据点_末) return false;
            return true;
        }

        string getText_菜单_买卖()
        {
            return pk::encode("买卖");
        }

        bool isEnabled_菜单_买卖()
        {
            if (building_.get_id() >= 据点_末) return false;
            if (taishu_ is null || pk::is_absent(taishu_)) return false;
            return true;
        }

        string getDesc_菜单_买卖()
        {
            if (taishu_ is null || pk::is_absent(taishu_))
                return pk::encode("太守不在空闲状态.");
            else
                return pk::encode("执行买卖操作.");
        }

        // =============================================================================================

        void init_菜单_自动化(pk::building@ building)
        {
            @building_ = @building;
            pk::city@ city_ = pk::building_to_city(building);

            menu_city_id_ = (city_ !is null) ? city_.get_id() : -1;
            menu_force_id_ = building.get_force_id();
        }

        bool isVisible_菜单_自动化()
        {
            if (pk::is_campaign()) return false;
            if (building_.get_id() >= 城市_末) return false;
            if (!pk::is_player_controlled(pk::get_city(menu_city_id_))) return false;
            return menu_city_id_ != -1 and menu_force_id_ == pk::get_current_turn_force_id();
        }

        string getText_菜单_自动化()
        {
            return pk::encode("太守自治");
        }

        bool isEnabled_菜单_自动化()
        {
            return true;
        }

        string getDesc_菜单_自动化()
        {
            return pk::encode("太守自治方针选项");
        }

        // =============================================================================================


    }

    Main main;
}


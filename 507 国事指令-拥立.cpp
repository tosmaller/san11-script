// ## 2020/10/29 # 江东新风 # 同步马术书大神更新，战役中禁用 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/08 # 氕氘氚 # 加入【国事】菜单 ##
/*
@제작자: 기마책사
@Update: '18.12.3  / 최초적용: 폐위된 후한황제를 복위시키고 옹립함
@Update: '19.7.8   / 변경내용: 폐황제 붕어메시지 오류 수정
@UPDATE: '20.8.29   / 변경내용: 캠페인에서는 스크립트 비활성화

*/

namespace 폐황제옹립
{

    // ================ CUSTOMIZE ================

    const int TP_COST_옹립 = 3000; 	   // 기교P 필요량
    const int ACTION_COST = 100;   // 행동력 필요량

    const int KEY = pk::hash("层裟");

    // ===========================================

    class Main
    {

        pk::building@ building;
        pk::building@ kunshu_building;

        pk::city@ kunshu_city;
        pk::city@ emperor_city;
        pk::point emeperor_pos;

        pk::force@ force;

        pk::person@ kunshu;
        pk::person@ emperor;
        pk::person@ support;

        Main()
        {

            pk::menu_item item;
            item.menu = global_menu::菜单_国事;
            //ch::u8debug(pk::format("menu: yong li {}", global_menu::菜单_国事));       
            item.init = pk::building_menu_item_init_t(init_옹립);
            item.is_visible = pk::menu_item_is_visible_t(isVisible_옹립);
            item.is_enabled = pk::menu_item_is_enabled_t(isEnabled_옹립);
            item.get_text = pk::menu_item_get_text_t(getText_옹립);
            item.get_desc = pk::menu_item_get_desc_t(getDesc_옹립);
            item.handler = pk::menu_item_handler_t(handler_옹립);
            pk::add_menu_item(item);

        }


        void init_옹립(pk::building@ building_t)
        {

            @building = @building_t;
            @force = pk::get_force(building.get_force_id());
            @kunshu = pk::get_person(pk::get_kunshu_id(building));
            @kunshu_building = pk::get_building(kunshu.service);
            @kunshu_city = pk::building_to_city(kunshu_building);
            @emperor = pk::get_person(pk::get_scenario().emperor);
            @support = pk::get_person(武将_宦官);
        }

        string getText_옹립()
        {
            return pk::encode("拥立");  //yonngli
        }

        string getDesc_옹립()
        {
            if (emperor !is null and emperor.mibun != 신분_사망)
                return pk::encode("汉帝未废, 拥立不了");
            else if (kunshu.kanshitsu != 한실_중시)
                return pk::encode("只有重视汉室的君主才能实行");
            else if (pk::is_protecting_the_emperor(force) or force.title == 爵位_皇帝)
                return pk::encode("拥立着东汉皇帝或已经是皇帝了.");
            else if (kunshu_city.get_id() != kunshu.service)
                return pk::encode("只能在君主所在城市实行.");
            else if (pk::is_absent(kunshu) or pk::is_unitize(kunshu) or kunshu.action_done)
                return pk::encode("君主已行动或不在城市中.");
            else if (force.tp < TP_COST_옹립)
                return pk::encode(pk::format("技巧P不足.(技巧P {})", TP_COST_옹립));
            else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST)
                return pk::encode(pk::format("行动力不足 (必须 {} 行动力))", ACTION_COST));
            else
                return pk::encode(pk::format("拥立汉帝. (技巧P {} )", TP_COST_옹립));

        }

        bool isVisible_옹립()
        {
            if (pk::is_campaign()) return false;
            if (emperor !is null and emperor.mibun != 신분_사망) return false;
            else if (kunshu.kanshitsu != 한실_중시) return false;
            return true;
        }

        bool isEnabled_옹립()
        {
            if (emperor !is null and emperor.mibun != 신분_사망) return false;
            else if (kunshu.kanshitsu != 한실_중시) return false;
            else if (pk::is_protecting_the_emperor(force) or force.title == 爵位_皇帝) return false;
            else if (kunshu_city.get_id() != kunshu.service) return false;
            else if (pk::is_absent(kunshu) or pk::is_unitize(kunshu) or kunshu.action_done) return false;
            else if (force.tp < TP_COST_옹립) return false;
            else if (pk::get_district(pk::get_district_id(force, 1)).ap < ACTION_COST) return false;
            else
                return true;
        }

        bool handler_옹립()
        {
            if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode(pk::format("拥立汉帝吗?\n(技巧P {} )", TP_COST_옹립)), kunshu) == 1)
                return false;

            // 후한 황제와의 대화
            pk::scene(pk::scene_t(scene_옹립));

            // 후한 황제 복원 및 이동
            emperor.mibun = 신분_없음;
            emperor.update();
            pk::move(emperor, kunshu_building, false);

            // 행동력 감소
            auto district = pk::get_district(pk::get_district_id(force, 1));
            pk::add_ap(district, -ACTION_COST);

            // 기교 감소
            ch::add_tp(force, -TP_COST_옹립, force.get_pos());

            string kunshu_name = pk::decode(pk::get_name(kunshu));
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x拥立了汉帝.", kunshu_name)));
            pk::create_effect(0x51, kunshu_building.pos);   //이펙트효과

            kunshu.action_done = true;

            return true;

        }


        void scene_옹립()
        {
            int pause = int(pk::option["MessagePause"]);
            pk::option["MessagePause"] = 100;
            pk::message_box(pk::encode("废帝何在. 请到我这里."), kunshu);
            pk::cutin(12);  // 컷인: 말발굽
            pk::move_screen(kunshu_city.get_pos(), 2000);

            // 재야 황제 알현
            scene_옹립_1();

            pk::move_screen(kunshu_city.get_pos(), 2000);

            pk::say(pk::encode("快把陛下请过来!"), kunshu, kunshu_city);
            pk::cutin(21);  // 컷인: 병사환호

            pk::option["MessagePause"] = pause;

        }

        void scene_옹립_1()
        {
            string kunshu_name = pk::decode(pk::get_name(kunshu));
            string kunshu_title = pk::decode(pk::get_name(pk::get_title(force.title)));
            string kunshu_city_name = pk::decode(pk::get_name(kunshu_city));

            pk::fade(0);
            pk::sleep();

            pk::background(9);  //배경: 농장
            pk::fade(255);
            pk::message_box(pk::encode("请问你见过长这样的人吗? "), kunshu);
            pk::message_box(pk::encode("...不清楚..."), pk::get_person(무장_농민));
            pk::message_box(pk::encode("有点像我在市场里见到的一个人..."), pk::get_person(무장_청년));
            pk::message_box(pk::encode("那我去市场看看, 也许会有线索."), kunshu);
            pk::message_box(pk::encode("是, 主公, 这边请."), pk::get_person(무장_문관));
            pk::fade(0);
            pk::background(-1);

            pk::background(4);  //배경: 시장
            pk::fade(255);
            pk::message_box(pk::encode("你可曾见过这样的人?"), kunshu);
            pk::message_box(pk::encode("这位呀.去那边那家便可. 但是有件事..."), pk::get_person(무장_부호));
            pk::message_box(pk::encode("谢谢你. \n快点跟上."), kunshu);
            pk::message_box(pk::encode("额."), pk::get_person(무장_무관));
            pk::message_box(pk::encode("什么事那么急."), pk::get_person(무장_부호));
            pk::fade(0);
            pk::background(-1);

            pk::background(7);  //배경: 집앞
            pk::fade(255);
            pk::message_box(pk::encode(pk::format("这不是\x1b[1x{}\x1b[0x吗? 你是怎么找来的?", kunshu_name)), support);
            pk::message_box(pk::encode("为了侍奉陛下,寻访多时...那陛下在何处?"), kunshu);
            pk::message_box(pk::encode("这边. 请跟我来."), support);
            pk::fade(0);
            pk::background(-1);

            pk::background(5);  //배경: 집
            pk::fade(255);


            if (emperor is null)
            {
                pk::get_scenario().emperor = 武将_献帝;
                @emperor = pk::get_person(pk::get_scenario().emperor);
            }

            pk::message_box(pk::encode(pk::format("这不是陛下? 臣\x1b[1x{}\x1b[0x参见陛下! ", kunshu_name)), kunshu);
            pk::message_box(pk::encode(pk::format("见到\x1b[1x{}\x1b[0x真是万幸. ", kunshu_name)), emperor);
            pk::message_box(pk::encode("罪过"), kunshu);
            pk::message_box(pk::encode(pk::format("以后将由\x1b[1x{}\x1b[0x侍奉陛下.必不会再使陛下再遭此厄运!", kunshu_name)), kunshu);
            pk::message_box(pk::encode("大恩不忘!只有你才是真正的忠臣呀"), emperor);
            pk::message_box(pk::encode(pk::format("陛下, 请准备一下. 我将在\x1b[1x{}\x1b[0x侍奉陛下.", kunshu_city_name)), kunshu);
            pk::background(-1);
        }


    }

    Main main;
}
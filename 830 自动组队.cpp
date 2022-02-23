// ## 2021/03/19 # 江东新风 # 记忆组队及组队调整基本完善 ##
// ## 2021/03/19 # 江东新风 # 组队调整的设定 ##
/*
   玩家主动调整的设想
   按钮 调整组队 
   1.调整现有组队优先级：选择兵种，选择主将，展示队伍组成，选择后询问调整后的优先级
   2.新增部队组成 选择兵种，选择主将，选择副将，组成后选择希望放置的优先级。
*/

namespace 自动_组队
{
    const bool 采用记忆组队 = true;

    /*
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
*/
    array<array<int>> 记忆组队数组;
    bool 调试模式 = false;
	class Main
	{
		Main()
		{
			pk::set_func(56, pk::func56_t(callback));
            pk::bind(108, pk::trigger108_t(每月记录部队));
            //pk::bind(107, pk::trigger107_t(每月记录部队));
            add_menu();
		}

        void add_menu()
        {
            // 组队调整 메뉴 (유저용)
            pk::menu_item 组队调整;
            组队调整.menu = 101;
            // 组队调整.pos = 8;
            //组队调整.shortcut = global_menu::shortcut_组队调整;
            组队调整.init = pk::building_menu_item_init_t(init);
            组队调整.is_visible = pk::menu_item_is_visible_t(isVisible_组队调整);
            组队调整.is_enabled = pk::menu_item_is_enabled_t(isEnabled_组队调整);
            组队调整.get_text = pk::menu_item_get_text_t(getText_组队调整);
            组队调整.get_desc = pk::menu_item_get_desc_t(getDesc_组队调整);
            组队调整.handler = pk::menu_item_handler_t(handler_组队调整);
            pk::add_menu_item(组队调整);
        }

        //---------------------------------------------------------------------------------------
        pk::building@ building_;
        pk::force@ force_;
        void init(pk::building@ building)
        {
            @building_ = @building;
            @force_ = pk::get_force(building.get_force_id());
        }

        string getText_组队调整()
        {
            return pk::encode("组队调整"); //chengqiangweixiu
        }

        string getDesc_组队调整()
        {
            return pk::encode("调整组队的优先级或新增默认组队");
        }

        bool isEnabled_组队调整()
        {
            return true;
        }

        bool isVisible_组队调整()
        {
            if (pk::is_campaign()) return false;
            if (!pk::is_alive(building_)) return false;
            if (!building_.is_player()) return false;
            return true;
        }

        bool handler_组队调整()
        {
            int choose = pk::choose(pk::encode("请选择要执行的操作"), { pk::encode("调整组队优先级"), pk::encode(" 新增组队设定 "), pk::encode("取消") });

            if (choose == 0) handler_调整组队优先级();
            else if (choose == 1) handler_新增组队设定();
            else return false;
            return true;

        }

        void handler_调整组队优先级()
        {
            array<array<int>> temp_arr;
            for (int i = 0; i < 300; ++i)
            {
                autoarmyinfo@ army_info = @autoarmy_ex[i];
                if (army_info.member1 == -1) break;//-1表示已经到末尾
                if (army_info.member1 == 0 and army_info.member2 == 0) break;
                array<int> arr_t = { army_info.member1, army_info.member2, army_info.member3, army_info.ground_weapon, army_info.sea_weapon, army_info.freq };
                temp_arr.insertLast(arr_t);
            }
            记忆组队数组 = temp_arr;
            //1.调整现有组队优先级：选择兵种，选择主将，展示队伍组成，选择后询问调整后的优先级
            int weapon_choose = pk::choose(pk::encode("请选择要对象兵种"), { pk::encode("枪兵"), pk::encode("戟兵"), pk::encode("弩兵"), pk::encode("骑兵")});
            pk::list<pk::person@ >person_list = get_target_person_list(weapon_choose + 1, 记忆组队数组);
            if (person_list.count > 0)
            {
                pk::list<pk::person@> person_sel = get_person_sel(person_list);
                array<array<int>> target_weapon_unit_arr = get_target_weapon_unit_arr(weapon_choose + 1, 记忆组队数组);//此数组用于排序优先级
                if (person_sel.count == 0)
                {
                    int final_choose = pk::choose(pk::encode("当前并未选择主将，是否继续选择？"), { pk::encode(" 是 "), pk::encode(" 否 ") });
                    if (final_choose == 0) person_sel = get_person_sel(person_list);
                    else return;
                }
                else
                {
                    int leader_id = person_sel[0].get_id();
                    //选择好主将的情况，罗列该主将对应的部队组成，展示目前优先级，选择要更改后的优先级
                    array<array<int>> target_unit_arr = get_target_unit_arr(leader_id, target_weapon_unit_arr);
                    array<string> unit_info_list;
                    if (调试模式)
                    {
                        for (int i = 0; i < int(target_unit_arr.length); ++i)
                        {
                            for (int j = 0; j < int(target_unit_arr[i].length); ++j)
                            {
                                pk::trace(pk::format("target_unit_arr.{}:j:{},{}", i, j, target_unit_arr[i][j]));
                            }

                        }
                        for (int i = 0; i < int(target_weapon_unit_arr.length); ++i)
                        {
                            for (int j = 0; j < int(target_weapon_unit_arr[i].length); ++j)
                            {
                                pk::trace(pk::format("target_weapon_unit_arr.{}:j:{},{}", i, j, target_weapon_unit_arr[i][j]));
                            }

                        }
                        for (int i = 0; i < int(记忆组队数组.length); ++i)
                        {
                            for (int j = 0; j < int(记忆组队数组[i].length); ++j)
                            {
                                pk::trace(pk::format("记忆组队数组.{}:j:{},{}", i, j, 记忆组队数组[i][j]));
                            }

                        }
                    }
  
                    //选择数量不定的制作比较麻烦，就做6个或10个的情况
                    if (int(target_unit_arr.length) < 6)
                    {
                        for (int i = 0; i < int(target_unit_arr.length); ++i)
                        {
                            unit_info_list.insertLast(pk::encode(pk::format("{}，{}，{}，{}，优先级：{}", get_weapon_name(target_unit_arr[i][3]), pk::decode(pk::get_name(pk::get_person(target_unit_arr[i][0]))), pk::decode(pk::get_name(pk::get_person(target_unit_arr[i][1]))), pk::decode(pk::get_name(pk::get_person(target_unit_arr[i][2]))), target_unit_arr[i][7])));
                        }
                        for (int i = 0; i < int(unit_info_list.length); ++i)
                        {
                            pk::trace(pk::format("listpos:{},{}", i,pk::decode(unit_info_list[i])));
                        }
                        
                        unit_choose_0(unit_info_list, target_unit_arr, target_weapon_unit_arr);

                    }
                    else if (int(target_unit_arr.length) > 6)
                    {
                        for (int i = 0; i < int(target_unit_arr.length); ++i)
                        {
                            if (i != 5 and i != int(target_unit_arr.length-1)) unit_info_list.insertLast(pk::encode(pk::format("{}，{}，{}，{}，优先级：{}", get_weapon_name(target_unit_arr[i][3]), pk::decode(pk::get_name(pk::get_person(target_unit_arr[i][0]))), pk::decode(pk::get_name(pk::get_person(target_unit_arr[i][1]))), pk::decode(pk::get_name(pk::get_person(target_unit_arr[i][2]))), target_unit_arr[i][7])));
                            if (i == 5) unit_info_list.insertLast(pk::encode("NEXT"));
                            if (i == int(target_unit_arr.length - 1)) unit_info_list.insertLast(pk::encode("BACK"));
                        }
                        for (int i = 0; i < int(unit_info_list.length); ++i)
                        {
                            pk::trace(pk::format("listpos:{},{}", i, pk::decode(unit_info_list[i])));
                        }
                        unit_choose_0(unit_info_list, target_unit_arr, target_weapon_unit_arr);

                    }


                    
                }
            }
            else
            {
                int final_choose = pk::choose( pk::encode("所选兵种并无储存的部队，是否继续选择？"), { pk::encode(" 是 "), pk::encode(" 否 ")});
                if (final_choose == 0) handler_调整组队优先级();
                else return;
            }
        }

        void handler_新增组队设定()
        {
            //选择武将
            int weapon_choose = pk::choose(pk::encode("请选择要对象兵种"), { pk::encode("枪兵"), pk::encode("戟兵"), pk::encode("弩兵"), pk::encode("骑兵") });
            string weapon_name = get_weapon_name(weapon_choose + 1);
            pk::list<pk::person@ >person_list = pk::get_person_list(force_, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
            pk::list<pk::person@ >person_sel = pk::person_selector(pk::encode(pk::format("选择{}主将", weapon_name)), pk::encode(pk::format("选择{}的主将", weapon_name)), person_list, 1, 1, person_list,37);
            if (person_sel.count == 0) return;
            int leader_id = person_sel[0].get_id();

            pk::list<pk::person@ >person_list2 = get_deputy_list(person_sel[0]);
            pk::list<pk::person@ >person_sel2 = pk::person_selector(pk::encode(pk::format("选择{}副将", weapon_name)), pk::encode(pk::format("选择{}的两位副将", weapon_name)), person_list2, 0, 2, person_list2, 37);
            int deputy1 = -1;
            int deputy2 = -1;
            if (person_sel2.count > 0) deputy1 = person_sel2[0].get_id();
            if (person_sel2.count == 2) deputy2 = person_sel2[1].get_id();

            //选择优先级
            array<array<int>> temp_arr;
            for (int i = 0; i < 300; ++i)
            {
                autoarmyinfo@ army_info = @autoarmy_ex[i];
                if (army_info.member1 == -1) break;//-1表示已经到末尾
                if (army_info.member1 == 0 and army_info.member2 == 0) break;
                array<int> arr_t = { army_info.member1, army_info.member2, army_info.member3, army_info.ground_weapon, army_info.sea_weapon, army_info.freq };
                temp_arr.insertLast(arr_t);
            }
            记忆组队数组 = temp_arr;

            array<array<int>> target_weapon_unit_arr = get_target_weapon_unit_arr(weapon_choose + 1, 记忆组队数组);//

            int same_pos = -1;
            for (int i = 0; i < int(target_weapon_unit_arr.length); ++i)
            {
                if (leader_id == target_weapon_unit_arr[i][0] and (weapon_choose + 1) == target_weapon_unit_arr[i][3])
                {
                    if ((deputy1 == target_weapon_unit_arr[i][1] and deputy2 == target_weapon_unit_arr[i][2]) or (deputy2 == target_weapon_unit_arr[i][1] and deputy1 == target_weapon_unit_arr[i][2]))
                    {
                        same_pos = i;
                    }
                }
            }
            if (same_pos != -1)
            {
                pk::message_box(pk::encode(pk::format("记录里存在相同的组队。\n{}，{}，{}，{}，优先级：{}。请问是否调整此队伍的优先级。", get_weapon_name(target_weapon_unit_arr[same_pos][3]), pk::decode(pk::get_name(pk::get_person(target_weapon_unit_arr[same_pos][0]))), pk::decode(pk::get_name(pk::get_person(target_weapon_unit_arr[same_pos][1]))), pk::decode(pk::get_name(pk::get_person(target_weapon_unit_arr[same_pos][2]))), same_pos)));
                reset_priority(same_pos, target_weapon_unit_arr);
            }
            else
            {
                //设定优先级
                array<int> temp1 = {leader_id, deputy1, deputy2, weapon_choose + 1 , 兵器_走舸, 1};
                set_priority(target_weapon_unit_arr, temp1);
            }

        }

        //----------------------------组队调整的支持函数------------------------------------------

        pk::list<pk::person@ > get_target_person_list(int weapon, array<array<int>> arr)
        {
            pk::list<pk::person@ > temp_list;
            for (int i = 0; i < int(arr.length); ++i)
            {
                if (arr[i][3] == weapon)
                {
                    temp_list.add(pk::get_person(arr[i][0]));
                }
            }
            return temp_list;
        }

        array<array<int>> get_target_weapon_unit_arr(int weapon, array<array<int>> arr)
        {
            array<array<int>> temp_arr;
            for (int i = 0; i < int(arr.length); ++i)
            {
                if (arr[i][3] == weapon)
                {
                    pk::trace(pk::format("arraypos:{},{}", i, arr[i][3]));
                    array<int> temp = arr[i];
                    temp.insertLast(i+1);//插入数组原来的序号，方便后面排序
                    temp_arr.insertLast(temp);
                }
            }
            pk::trace(pk::format("end arr", 1));
            return temp_arr;
        }

        pk::list<pk::person@ > get_person_sel(pk::list<pk::person@ > person_list)
        {
            pk::list<pk::person@ >person_list_t = pk::person_selector(pk::encode("选择对象主将"), pk::encode("选择需要调整的主将."), person_list, 1, 1);
            return person_list_t;
        }
        //进行此函数时，传入的数组应该已经限定好武器了
        array<array<int>> get_target_unit_arr(int leader_id, array<array<int>> arr)
        {
            array<array<int>> temp_arr;
            for (int i = 0; i < int(arr.length); ++i)
            {
                if (arr[i][0] == leader_id)
                {
                    pk::trace(pk::format("array2pos:{},{}", i, arr[i][3]));
                    array<int> temp = arr[i];
                    temp.insertLast(i);//插入在选择好武器数组的位置序号，方便后面寻找
                    temp_arr.insertLast(temp);
                }
            }
            pk::trace(pk::format("end arr2", 1));
            return temp_arr;
        }

        string get_weapon_name(int weapon_id)
        {
            string weapon_name;
            switch (weapon_id)
            {
            case 병기_검: weapon_name = "剑兵"; break;
            case 병기_창: weapon_name = "枪兵"; break;
            case 병기_극: weapon_name = "戟兵"; break;
            case 병기_노: weapon_name = "弩兵"; break;
            case 병기_군마: weapon_name = "骑兵"; break;
            case 병기_충차: weapon_name = "冲车"; break;
            case 병기_정란: weapon_name = "井阑"; break;
            case 병기_투석: weapon_name = "投石"; break;
            case 병기_목수: weapon_name = "木兽"; break;
            case 兵器_走舸: weapon_name = "水军"; break;
            case 兵器_楼船: weapon_name = "水军"; break;
            case 兵器_斗舰: weapon_name = "水军"; break;
            default: weapon_name = "??"; break;
            }
            return weapon_name;
        }

        void unit_choose_0(array<string> unit_info_arr, array<array<int>> arr, array<array<int>> arr2)
        {
            if (调试模式) pk::trace(pk::format("choose0", 2));
            int n = pk::choose(pk::encode("请选择要更改优先级的队伍"), unit_info_arr);
            if (n!=5)
            {
                reset_priority(arr[n][7], arr2);
            }
            else
            {
                if (arr.length < 7)
                {
                    reset_priority(arr[n][7], arr2);
                }
                else
                {
                    unit_choose_1(unit_info_arr, arr, arr2);
                }
            }
            pk::trace(pk::format("choose0-2", 2));
        }

        void unit_choose_1(array<string> unit_info_arr, array<array<int>> arr, array<array<int>> arr2)
        {
            if (调试模式) pk::trace(pk::format("choose1", 2));
            int n = pk::choose(pk::encode("请选择要更改优先级的队伍"), unit_info_arr);
            if ((n+6)!= int(unit_info_arr.length)) //不是最后一项的处理
            {
                reset_priority(arr[n+6][7], arr2);
            }
            else
            {
                //最后一项的处理
                unit_choose_0(unit_info_arr,arr, arr2);
            }
            pk::trace(pk::format("choose1-2", 2));
        }

        void reset_priority(int pos, array<array<int>> arr2)
        {
            pk::message_box(pk::encode(pk::format("当前部队的优先级为:{},频次为：{}。\n请选择输入调整后的优先级。", pos, arr2[pos][5])));
            int def_value = pk::numberpad(pk::encode("优先级"), 1, arr2.length, pos, pk::numberpad_t(pad_callback)).first;

            //优先级和当前优先级相同的情况的处理

            //调整优先级,实际上是获取n-1位的频次，然后选择的项目的频次变为目标优先级项目的频次+1，然后重新排序
            int id = arr2[pos][6]-1;//获取所在序号
            int id2 = arr2[def_value - 1][6]-1;//获取目标所在序号
            if (调试模式) pk::trace(pk::format("id:{},id2{},数组长度：{}", id, id2, 记忆组队数组.length));
            记忆组队数组[id][5] = 记忆组队数组[id2][5];//选择的项目的频次变为目标优先级项目的频次 然后插入到目标上方

            if (id > id2)
            {
                array<int> temp = 记忆组队数组[id];
                记忆组队数组.removeAt(id);
                记忆组队数组.insertAt(id2, temp);
            }
            else if (id < id2)
            {
                array<int> temp = 记忆组队数组[id];
                记忆组队数组.insertAt(id2, temp);
                记忆组队数组.removeAt(id);
            }

            for (int i = 0; i < int(记忆组队数组.length); ++i)
            {
                if (i >= 300) break;
                autoarmyinfo@ army_info = @autoarmy_ex[i];
                army_info.member1 = 记忆组队数组[i][0];
                army_info.member2 = 记忆组队数组[i][1];
                army_info.member3 = 记忆组队数组[i][2];
                army_info.ground_weapon = 记忆组队数组[i][3];
                army_info.sea_weapon = 记忆组队数组[i][4];
                army_info.freq = 记忆组队数组[i][5];
            }
            bool confirm = pk::yes_no(pk::encode("设定完成，是否继续下一个设定？"));
            if (confirm) handler_组队调整();
        }

        void set_priority(array<array<int>> arr2, array<int> insert_arr)
        {
            pk::message_box(pk::encode("请输入优先级。"));
            int def_value = pk::numberpad(pk::encode("优先级"), 1, pk::max(arr2.length, 1), 1, pk::numberpad_t(pad_callback)).first;

            //优先级和当前优先级相同的情况的处理

            //调整优先级,实际上是获取n-1位的频次，然后选择的项目的频次变为目标优先级项目的频次+1，然后重新排序
            int id = 0;
            if (arr2.length() > 0) id = arr2[def_value - 1][6] - 1;//获取目标所在序号

            记忆组队数组.insertAt(id, insert_arr); //直接插入

            //信息同步
            for (int i = 0; i < int(记忆组队数组.length); ++i)
            {
                if (i >= 300) break;
                autoarmyinfo@ army_info = @autoarmy_ex[i];
                army_info.member1 = 记忆组队数组[i][0];
                army_info.member2 = 记忆组队数组[i][1];
                army_info.member3 = 记忆组队数组[i][2];
                army_info.ground_weapon = 记忆组队数组[i][3];
                army_info.sea_weapon = 记忆组队数组[i][4];
                army_info.freq = 记忆组队数组[i][5];
            }
            bool confirm = pk::yes_no(pk::encode("设定完成，是否继续下一个设定？"));
            if (confirm) handler_新增组队设定();
        }

        pk::list<pk::person@ > get_deputy_list(pk::person@leader)
        {
            int force_id = leader.get_force_id();
            int leader_id = leader.get_id();
            pk::list<pk::person@ > temp_list;
            for (int i = 0; i < 非贼武将_末; ++i)
            {
                if (leader_id != i)
                {
                    pk::person@ deputy = pk::get_person(i);
                    if (deputy.get_force_id() == force_id)
                    {
                        temp_list.add(deputy);
                    }
                }
            }
            return temp_list;
        }

        string pad_callback(int line, int original_value, int current_value)
        {

            return pk::encode("输入数字");  // 
        }
        //---------------------------------------------------------------------------------------


        //int weapon_heishu = -1;

		//pk::int_int_int callback(int person_id)
		//pk::int_int_int_int_int_int callback(int leader, int member1, int member2, int troops, int ground_weapon, int sea_weapon)
        void callback(pk::march_cmd_info& in info)
        {
            if (info.member[0] == -1) return;
            if (info.base is null) return;
            pk::trace(pk::format("pos1,info.member[0]:{},info.member[1]:{}", info.member[0], info.member[1]));
            pk::building@base = @info.base;

            pk::list<pk::person@> idle_person_list = pk::get_idle_person_list(base);//pk::get_person_list(base, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
            if (idle_person_list.count == 0) return;
            int weapon_heishu = pk::equipment_id_to_heishu(info.weapon_id[0]);
            pk::int_int_int info1_t = pk::int_int_int(-1, -1, -1);//暂存搜索到的信息
            if (采用记忆组队)
            {
                int leader_id = -1;
                int member_id1 = -1;
                int member_id2 = -1;
                bool finished = false;
                for (int i = 0; i < int(记忆组队数组.length); ++i)
                {
                    //pk::trace(pk::format("pos0,i:{},数组武器：{}，编制武器：{} ", i, 记忆组队数组[i][3], info2.second));
                    if (记忆组队数组[i][0] == -1) break; //无主将时说明到末尾，退出循环
                    if (记忆组队数组[i][3] != info.weapon_id[0]) continue;//如果部队兵装不符，不进入判断
                    //pk::trace(pk::format("pos1,i:{}", i));
                    leader_id = 记忆组队数组[i][0];
                    for (int j = 0; j < idle_person_list.count; ++j)
                    {
                        //pk::trace(pk::format("pos2,i:{},j:{}", i,j));
                        if (idle_person_list[j].get_id() == leader_id)
                        {
                            //开始搜索副将
                            int person_count = 记忆组队数组[i][1] == -1 ? 1 : (记忆组队数组[i][2] == -1 ? 2 : 3);
                            //pk::trace(pk::format("pos2,i:{},j:{},找到主将id：{},部队人数：{}", i, j, leader_id,person_count));
                            if (person_count == 1)
                            {
                                finished = true;
                                info1_t.first = leader_id;
                                break;//退出到记忆组队数组那个循环
                            }

                            member_id1 = 记忆组队数组[i][1];
                            member_id2 = 记忆组队数组[i][2];
                            int 副将计数 = 0;
                            int member1_t = -1;
                            for (int k = 0; k < idle_person_list.count; ++k)
                            {
                                //pk::trace(pk::format("pos2,i:{},j:{},k:{},目标副将id：{}，{},目前的id：{}", i, j,k, member_id1, member_id2, idle_person_list[k].get_id()));
                                if (idle_person_list[k].get_id() == leader_id) continue;//跳过重复的

                                if (idle_person_list[k].get_id() == member_id1 or idle_person_list[k].get_id() == member_id2)
                                {
                                    if (person_count == 2)
                                    {
                                        finished = true;
                                        info1_t.first = idle_person_list[j].get_id();
                                        info1_t.second = idle_person_list[k].get_id();
                                        //pk::trace(pk::format("找到一个副将退出循环", 1));
                                        break;//退出寻找副将的循环
                                    }
                                    else
                                    {
                                        //两个副将的情况
                                        副将计数 += 1;
                                        if (副将计数 == 1)
                                        {
                                            member1_t = idle_person_list[k].get_id();
                                            continue;
                                        }
                                        else
                                        {
                                            info1_t.first = idle_person_list[j].get_id();
                                            info1_t.second = member1_t;
                                            info1_t.third = idle_person_list[k].get_id();
                                            finished = true;
                                            break;//退出寻找副将的循环
                                        }

                                    }
                                }
                            }

                            if (finished) break;//退出寻找主将的循环
                        }
                    }
                    if (finished) break;
                }

                if (info1_t.first != -1)
                {
                    //temp.leader = info1_t.first;
                    //temp.deputy1 = info1_t.second;
                    //temp.deputy2 = info1_t.third;
                    info.member[0] = info1_t.first;
                    info.member[1] = info1_t.second;
                    info.member[2] = info1_t.third;
                    //info.member = member;
                    //pk::trace(pk::format("上传武将id，武将1,{}。武将2：{}，武将3：{}", info1_t.first, info1_t.second, info1_t.third));
                    //pk::trace(pk::format("接受武将id，武将1,{}。武将2：{}，武将3：{}", info1.first, info1.second, info1.third));
                    return;
                }
                else return;

                //还需根据获取的武将组合配给兵力
            }
        }

		void callback22(pk::int_int_int & info1, pk::int_int_int & info2)
		{
			pk::person@ leader = pk::get_person(info1.first);
			pk::building@ base = pk::get_building(leader.service);
            pk::list<pk::person@> idle_person_list = pk::get_idle_person_list(base);//pk::get_person_list(base, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
			if (idle_person_list.count == 0) return;
            int weapon_heishu = pk::equipment_id_to_heishu(info2.second);

            //武将数据的暂存数组
            pk::int_int_int info1_t = pk::int_int_int(-1,-1,-1);
            pk::int_int_int info2_t = pk::int_int_int(-1,-1,-1);
			//根据特技组合选择组队？
			//根据武将id组合选择组队？
			//官职高的必定是主将
            if (采用记忆组队)
            {
                int leader_id = -1; 
                int member_id1 = -1;
                int member_id2 = -1;
                bool finished = false;
                for (int i = 0; i < int(记忆组队数组.length); ++i)
                {
                    //pk::trace(pk::format("pos0,i:{},数组武器：{}，编制武器：{} ", i, 记忆组队数组[i][3], info2.second));
                    if (记忆组队数组[i][0] == -1) break; //无主将时说明到末尾，退出循环
                    if (记忆组队数组[i][3] != info2.second) continue;//如果部队兵装不符，不进入判断
                    //pk::trace(pk::format("pos1,i:{}", i));
                    leader_id = 记忆组队数组[i][0];
                    for (int j = 0; j < idle_person_list.count; ++j)
                    {
                        //pk::trace(pk::format("pos2,i:{},j:{}", i,j));
                        if (idle_person_list[j].get_id() == leader_id)
                        {
                            
                            //开始搜索副将
                            int person_count = 记忆组队数组[i][1] == -1 ? 1 : (记忆组队数组[i][2] == -1? 2:3);
                            //pk::trace(pk::format("pos2,i:{},j:{},找到主将id：{},部队人数：{}", i, j, leader_id,person_count));
                            if (person_count == 1)
                            {
                                finished = true;
                                info1_t.first = leader_id;
                                break;//退出到记忆组队数组那个循环
                            }

                            member_id1 = 记忆组队数组[i][1];
                            member_id2 = 记忆组队数组[i][2];
                            int 副将计数 = 0;
                            int member1_t = -1;
                            for (int k = 0; k < idle_person_list.count; ++k)
                            {
                                //pk::trace(pk::format("pos2,i:{},j:{},k:{},目标副将id：{}，{},目前的id：{}", i, j,k, member_id1, member_id2, idle_person_list[k].get_id()));
                                if (idle_person_list[k].get_id() == leader_id) continue;//跳过重复的

                                if (idle_person_list[k].get_id() == member_id1 or idle_person_list[k].get_id() == member_id2)
                                {
                                    if (person_count == 2)
                                    {
                                        finished = true;
                                        info1_t.first = idle_person_list[j].get_id();
                                        info1_t.second = idle_person_list[k].get_id();
                                        //pk::trace(pk::format("找到一个副将退出循环", 1));
                                        break;//退出寻找副将的循环
                                    }
                                    else
                                    {
                                        //两个副将的情况
                                        副将计数 += 1;
                                        if (副将计数 == 1) 
                                        {
                                            member1_t = idle_person_list[k].get_id();
                                            continue;
                                        }
                                        else
                                        {
                                            info1_t.first = idle_person_list[j].get_id();
                                            info1_t.second = member1_t;
                                            info1_t.third = idle_person_list[k].get_id();
                                            finished = true;
                                            break;//退出寻找副将的循环
                                        }

                                    }
                                }
                            }

                            if (finished) break;//退出寻找主将的循环
                        }
                    }
                    if (finished) break;
                }

                if (info1_t.first != -1)
                {
                    info1 = info1_t;
                    //pk::trace(pk::format("上传武将id，武将1,{}。武将2：{}，武将3：{}", info1_t.first, info1_t.second, info1_t.third));
                    //pk::trace(pk::format("接受武将id，武将1,{}。武将2：{}，武将3：{}", info1.first, info1.second, info1.third));
                    return;
                }
                else return;

                //还需根据获取的武将组合配给兵力
            }



			//pk::trace(pk::format("outside:leader:{}, member1:{}, member2:{}, troops:{}, ground_weapon:{}, sea_weapon:{}", info1.first, info1.second, info1.third, info2.first, info2.second, info2.third));
			//pk::trace(pk::format("outside:leader:{}, member1:{}, member2:{}, troops:{}, ground_weapon:{}, sea_weapon:{}", leader, member1, member2, troops, ground_weapon, sea_weapon));
			
			//info1.first = 武将_关羽;
			//info1.second = 武将_张飞;
			//info1.third = 武将_诸葛亮;
			//return;
			return;
		}

        
        void 每月记录部队()
        {
            if (pk::is_campaign()) return;
            if (pk::get_player_count() < 1) return;
            if (调试模式) pk::trace("start autoarmy");
            pk::force@ player_force;
            for (int i = 0; i < 非贼势力_末; ++i)
            {
                if (pk::get_force(i).player == 0)  @player_force = pk::get_force(i);             
            }

            pk::list<pk::unit@> unit_list = pk::get_unit_list(player_force);
            array<array<int>> temp_arr;
            for (int i = 0; i < 300; ++i)
            {
                autoarmyinfo@ army_info = @autoarmy_ex[i];
                if (army_info.member1 == -1) break;//-1表示已经到末尾
                if (army_info.member1 == 0 and army_info.member2 == 0) break;
                array<int> arr_t = {army_info.member1, army_info.member2, army_info.member3, army_info.ground_weapon, army_info.sea_weapon, army_info.freq};
                temp_arr.insertLast(arr_t);
            }
            记忆组队数组 = temp_arr;


            for (int i = 0; i < unit_list.count; ++i)
            {
                bool same_unit = false;
                if (unit_list[i].type == 部队类型_战斗)
                {                    
                    for (int j = 0; j < int(记忆组队数组.length); ++j)
                    {
                        //pk::trace(pk::format("same juge :,unit weapon:{}.记忆组队数组weapon:{}", unit_list[i].weapon, 记忆组队数组[j][3]));
                        //暂时只考虑陆地兵种
                        if (unit_list[i].weapon == 记忆组队数组[j][3])
                        {
                            //pk::trace(pk::format("same juge :,unit member1:{}.member2{}.member3{}", unit_list[i].member[0], unit_list[i].member[1], unit_list[i].member[1]));
                            //pk::trace(pk::format("same juge :,记忆组队数组 member1:{}.member2{}.member3{}", 记忆组队数组[j][0], 记忆组队数组[j][1], 记忆组队数组[j][2]));
                            //相同的情况
                            if (unit_list[i].member[0] == 记忆组队数组[j][0] and ((unit_list[i].member[1] == 记忆组队数组[j][1] and unit_list[i].member[2] == 记忆组队数组[j][2]) or (unit_list[i].member[1] == 记忆组队数组[j][2] and unit_list[i].member[2] == 记忆组队数组[j][1])))
                            {
                                //pk::trace("pos same");
                                记忆组队数组[j][5] += 1;
                                //当场排序
                                if (j != 0)
                                {
                                    for (int k = (j - 1); k > -1; --k)
                                    {
                                        if (记忆组队数组[j][5] < 记忆组队数组[k][5] and (k+1) != j)
                                        {
                                            array<int> temp = 记忆组队数组[j];
                                            记忆组队数组[j] = 记忆组队数组[k+1];
                                            记忆组队数组[k+1] = temp;
                                            break;
                                        }
                                    }
                                }

                                same_unit = true;
                                //pk::trace(pk::format("same :,member1:{}.member2{}", unit_list[i].member[0], unit_list[i].member[1]));
                                break;//退出记忆组队数组的循环
                            }
                        }

                    }
                }
                if (!same_unit and (unit_list[i].weapon > 0 and unit_list[i].weapon < 9))
                {
                    array<int> unit_infomation = { unit_list[i].member[0], unit_list[i].member[1], unit_list[i].member[2], unit_list[i].weapon, 兵器_走舸, 1 };

                    //pk::trace(pk::format("not same i:{},member1:{}.member2{}.weapon:{}", i, unit_list[i].member[0], unit_list[i].member[1], unit_list[i].weapon));
                    记忆组队数组.insertLast(unit_infomation);
                }
            }

            //for (int i = 0; i < 记忆组队数组.length; ++i)
            //{
            //    pk::trace(pk::format("id:{},member1:{}.member2{}, weapon:{}, freq:{}", i, 记忆组队数组[i][0], 记忆组队数组[i][1], 记忆组队数组[i][3], 记忆组队数组[i][5]));
            //}

            array<array<int>> temp_arr2;
            for (int i = 0; i < int(记忆组队数组.length); ++i)
            {
                if (i >= 300) break;
                autoarmyinfo@ army_info = @autoarmy_ex[i];
                army_info.member1 = 记忆组队数组[i][0];
                army_info.member2 = 记忆组队数组[i][1];
                army_info.member3 = 记忆组队数组[i][2];
                army_info.ground_weapon = 记忆组队数组[i][3];
                army_info.sea_weapon = 记忆组队数组[i][4];
                army_info.freq = 记忆组队数组[i][5];
            }
            if (调试模式) pk::trace("end autoarmy");
        }


        /*
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
        int get_stat_tekisei(pk::person@ person_t)
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

        // 부대 적성 능력가중치 환산
        int get_stat_unit_tekisei(pk::unit@ unit_t)
        {
            int stat = 0;
            int tekisei = pk::get_tekisei(unit_t, unit_t.weapon);
            if (武将变更_启用适性标准)
            {
                int stat_base = (tekisei > pk::max(0, pk::min(3, 武将变更_适性基准值))) ? 30 : 0;
                stat = int(武将变更_适性权重 * (stat_base + 15 * pk::min(3, tekisei)));
            }
            return stat;
        }

        // 武将特技权重换算무장 특기 가중치 환산
        int get_stat_skill(pk::person@ person_t)
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
        }*/
	}

	Main main;
}
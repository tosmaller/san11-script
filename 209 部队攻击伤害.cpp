// ## 2021/11/05 # 江东新风 # 删除无效的螺旋突设定，部分内容汉化 ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/10/21 # 江东新风 #通过rettype改进劫财设置##
// ## 2021/10/14 # 江东新风 #复刻码神的疑兵伏兵系统，加入踩地格触发伏兵,加入突袭运输队##
// ## 2021/10/13 # 江东新风 #匹配新209函数参数##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/21 # 江东新风 # 将破竹移至356 ##
// ## 2021/09/03 # 江东新风 # 君主部队溃灭，周围五格己方部队士气下降50，军团长部队溃灭，周围三格己方部队士气下降30 ##
// ## 2021/02/17 # 江东新风 # 去掉此处的战法致伤 ##
// ## 2021/02/14 # 江东新风 # 修复重甲等防御技不生效bug,重甲50%免疫普通弓箭 ##
// ## 2021/02/14 # 江东新风 # 修复蓄势不生效bug ##
// ## 2021/01/13 # 江东新风 # 修复特殊设施闪退bug2 ##
// ## 2021/01/12 # 江东新风 # 修复特殊设施闪退bug ##
// ## 2021/01/11 # 江东新风 # 修复异族伤害来源的特殊建筑破坏 ##
// ## 2021/01/05 # 江东新风 # 特殊地名设施争夺的完善2 ##
// ## 2021/01/03 # 江东新风 # 特殊地名设施争夺的完善 ##
// ## 2020/12/16 # 江东新风 # 预写的特定设施争夺 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/11/17 # 江东新风 # trace 信息乱码修复 ##
// ## 2020/10/29 # 江东新风 # 同步马术书大神的更新，优化ai战法 ##
// ## 2020/10/27 # 氕氘氚 # 添加战法击伤判定，删除势力效果，新特技效果 ##
// ## 2020/10/12 # 江东新风 # 狙击无视护卫bug修复 ##
// ## 2020/09/23 # 江东新风 # sire新特技狙击 ##
// ## 2020/09/21 # 江东新风 # 同步氢神cp949的两次更新 ##
// ## 2020/09/16 # 氕氘氚 # 修复攻城伤害異常低的bug ##
// ## 2020/09/06 # 氕氘氚 # 修复金刚不論伤害均生效的bug ##
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/08/16 # 氕氘氚 # 刪除部分常量 ##
// ## 2020/08/14 #江东新风#将光环系统的暴击伤害光环放入此处, 新特技破竹##
/*
@ 수정이 : 기마책사
@ UPDATE : '18.10.21  / 변경내용: 데미지 보정효과 적용
@ UPDATE : '18.11.8   / 변경내용: 유저설정 옵션 추가, 전략거점 크리티컬보정추가, 거점방어버프추가
@ UPDATE : '18.11.27  / 변경내용: 교전면향에 대미지보정 추가
@ UPDATE : '19.2.16   / 변경내용: 전법에 대한 불굴/금강 발동 시 전법실패 처리 + 방어막 이팩트 표시 + 반격대미지 적용되게 수정
@ UPDATE : '19.2.21   / 변경내용: 커스텀 대화표시가 교전 이후에 표시되도록 경험치획득트리거 방식으로 변경, 나선첨혼란확률 강제적용 옵션 추가
@ UPDATE : '19.3.3    / 변경내용: 후방피격시 혼란효과 기능 추가
@ UPDATE : '19.6.17   / 변경내용: 후방피격시 혼란효과 반격적용 버그 수정
@ UPDATE : '19.11.21  / 변경내용: 초특기연전 활성화 시 근접전법의 원거리발동 버그 수정, AI부대 전법최적화 옵션 추가
@ UPDATE : '20.4.11   / 변경내용: 전투병과의 공성파쇄 데미지 감소
@ UPDATE : '20.8.25   / 변경내용: 나선첨혼란확률 옵션OFF 처리 (pk2.1 신버젼에서 버그 수정됨)
@ UPDATE : '20.8.29   / 변경내용: 캠페인에서는 커스텀 세팅 사용 불가하도록 수정
@ UPDATE : '20.10.6   / 변경내용: 초특기연전 사거리 확인조건 수정, 전법최적화 확인조건 수정 (노병만제외)
@ UPDATE : '20.10.17  / 변경내용: 초특기연전 발동함수 오류 수정 (특기_사수, 숲에서 전법취소 현상), AI部队_弩兵攻击优化 추가 (통상활공격을 화시로 변경)
*/

namespace UNIT_ATTACK_DAMAGE
{
    //---------------------------------------------------------------------------------------
    // 유저 설정 (true = on, false = off)
    const bool 显示对话框 = true;   // 커스텀 효과 발동 시 대화표시 여부 (false 권장)


    //---------------------------------------------------------------------------------------
    // 부대 후방피격시 혼란효과 : 교전방향설정=true 시 활성화되는 추가 옵션  交战朝向修正true时才会生效
    const bool 后方攻击_启用混乱 = true;   // 背后攻击混乱 부대 후방피격시 일정확률로 혼란효과 (true=적용, false=미적용)
    const int  后方攻击_混乱概率 = 10;     // 背后攻击混乱?率 后方攻击_启用混乱=true 시 혼란 확률값 (0~100)

    // 초특기연전 활성화 시 근접전법이 원거리에서 발동되는 버그 수정 (true 권장)
    const bool 초특기연전_발동설정 = true;     // 초특기연전 활성화 시 근접전법의 원거리발동 버그 수정

    // AI부대 전법 옵션
    const bool AI部队_战法优化 = false;    // AI部队交战时选择有效战法 AI부대 교전 시 효과적인 전법을 선택
    const bool AI部队_弩兵攻击优化 = true;   // AI部队交战时将弩兵普通间接攻击变更为火矢  AI부대 교전 시 노병 통상간접 공격을 화시로 변경

    //下面内容只是为了防止kre报错，实际不起效，所以kre修改也不起效
    // 나선첨혼란확률 강제적용 (204 나선첨 혼란 확률.cpp 미동작 보완)
    const bool 나선첨혼란확률_강제적용 = true;   // 螺旋突混乱 적용 시 참고사항 : 혼란→통상으로 상태 강제처리 시 애니메이션 어색할 수 있음
    const int  나선첨_크리티컬혼란확률 = 100;    // 螺旋突混乱暴击?率 기본값=100, 크리티컬일 때 혼란확률값
    const int  나선첨_전법성공혼란확률 = 30;     // 螺旋突混乱一般?率 기본값=30, 크리티컬 아닐 때 혼란확률값 

    // 부대 후방피격시 혼란효과 : 교전방향설정=true 시 활성화되는 추가 옵션  交战朝向修正true时才会生效
    const bool 후방피격시_혼란적용 = true;   // 背后攻击混乱 부대 후방피격시 일정확률로 혼란효과 (true=적용, false=미적용)
    const int  후방피격시_혼란확률 = 10;     // 背后攻击混乱?率 후방피격시_혼란적용=true 시 혼란 확률값 (0~100)

    // AI부대 전법 옵션
    const bool AI부대_전법최적화 = false;    // AI부대 교전 시 효과적인 전법을 선택
    const bool AI부대_노병화시전법 = true;   // AI부대 교전 시 노병 통상간접 공격을 화시로 변경


    //array<pk::point> 地名坐标 = {pk::point(83, 133), pk::point(84, 133)};
    //---------------------------------------------------------------------------------------


    class Main
    {
        Main()
        {
            pk::set_func(209, pk::func209_t(callback));

            pk::bind(173, pk::trigger173_t(onUnitExpAdd));

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
        void callback(pk::damage_info& info, pk::unit@ attacker, int tactics_id, const pk::point& in target_pos, int type, int critical, bool ambush, int rettype)
        {
            if (attacker is null) return;
            int force_id = attacker.get_force_id();
            pk::force@ force = pk::get_force(force_id);
            int weapon_id = attacker.weapon;
            int command = batt::get_command(attacker);
            pk::unit@ target_unit = pk::get_unit(target_pos);
            pk::building@ target_building = pk::get_building(target_pos);
            pk::hex_object@ target_obj = pk::get_hex_object(target_pos);
            int atk = 0;
            int def = 0;
            int troops_atk = 0;
            int buffed = 0;
            //pk::trace(pk::format("attacker:{}, tactics_id:{}, target_pos,x:{},y:{}, type:{}, critical:{}, ambush:{}", pk::decode(pk::get_name(attacker)), tactics_id, target_pos.x, target_pos.y, type, critical, ambush));
            if (调试模式)
            {
                pk::trace(pk::format("attacker:{},x:{},y:{}, tactics_id:{}, target_pos,x:{},y:{}", pk::decode(pk::get_name(attacker)), attacker.pos.x, attacker.pos.y, tactics_id, target_pos.x, target_pos.y));
                pk::trace(pk::format("209rettype:{}", rettype));
                if (rettype == 0) pk::trace("209玩家普攻");//不区分建筑部队
                if (rettype == 1) pk::trace("209玩家齐攻");
                if (rettype == 2) pk::trace("209玩家战法对部队");
                if (rettype == 3) pk::trace("209玩家战法对建筑");
                if (rettype == 4) pk::trace("209受到反击");
                if (rettype == 5) pk::trace("209触发连击");
                if (rettype == 7) pk::trace("209横扫波及");
                if (rettype == 8) pk::trace("209旋风波及");
                if (rettype == 10) pk::trace("209贯射波及");
                if (rettype == 12) pk::trace("209乱射波及");//不区分建筑部队
                if (rettype == 14) pk::trace("209投石波及");
                if (rettype == 15) pk::trace("209委任计算");
                if (rettype == 16) pk::trace("209委任执行");
            }

            //劫财信息
            //gold_heal = 0;
            //gold_damage = 0;
            //@gold_unit = null;
            //@gaingold_unit = null;
            //---------------------------------------------------------------------------------------
            // 경험치획득 처리용 변수 초기화 ('19.2.21)
            @msg_unit = null;
            @msg_person = null;
            msg_string = "";

            @back_unit = null;
            //---------------------------------------------------------------------------------------
            //ch::debug(pk::format("unit action_done is {}", attacker.action_done));
            // 基本攻击力
            if (ambush)
                atk = pk::max(attacker.attr.stat[部队能力_智力] - 5, 1);
            else
                atk = attacker.attr.stat[部队能力_攻击];


            // AI部队战法优化 ('20.10.13)----这个ai计算时也会被调用了，可能出问题
            if (!pk::is_campaign() and !pk::is_player_controlled(attacker) and !ambush and type == 0)
            {
                if (AI部队_弩兵攻击优化 and attacker.weapon == 兵器_弩)    // 노병 간접통상 → 전법화시로 최적화
                    tactics_id = func_fire_on_arrow(attacker, target_obj, tactics_id);

                if (AI部队_战法优化 and target_unit !is null)     // 근접 전법 최적화
                    tactics_id = func_dir_tactics_opt(attacker, target_unit, tactics_id);
            }



            // 基本伤害、战法成功与否
            if (pk::is_valid_tactics_id(tactics_id))
            {
                troops_atk = pk::get_tactics(tactics_id).troops_atk;
                info.tactics_hit = pk::rand_bool(cast<pk::func202_t>(pk::get_func(202))(attacker, attacker.get_pos(), pk::get_hex_object(target_pos), tactics_id));
                //sire的战法成功与否在这里---也就是激励的设定
            }
            else if (pk::is_neighbor_pos(attacker.get_pos(), target_pos))
            {
                troops_atk = 10;
            }


            // 设施攻击加成
            if (batt::has_atk_buff_building(attacker.get_pos(), force_id))
            {
                buffed = 1;
                info.buffed = true;
            }

            // 防御建筑减免伤害  디버프
            int debuffer = batt::get_def_buff_building_id(target_unit);
            if (debuffer >= 0)
            {
                info.debuffer = debuffer;
            }

            /*// 是否暴击  크리티컬
            if (ch::has_skill(target_unit, 特技_布阵))  //布阵强制防暴击
                critical = 2;
            */
            if (critical == 0)
            {
                if (pk::is_valid_tactics_id(tactics_id) and not info.tactics_hit)
                    info.critical = false;
                else if (type == 0)
                    info.critical = cast<pk::func201_t@>(pk::get_func(201))(attacker, pk::get_hex_object(target_pos), tactics_id, !pk::is_neighbor_pos(attacker.get_pos(), target_pos));
            }
            else if (critical == 1)
            {
                info.critical = true;
            }
            else
                info.critical = false;

            float 정예기교_공격력 = 精锐兵伤害系数;
            float 단련기교_공격력 = 锻炼兵伤害系数;
            float 크리티컬_공격력 = 暴击伤害系数;

            if (ch::has_skill(attacker, 特技_战神))
            {
                크리티컬_공격력 += 战神暴击系数;
            }


            //光环系统-暴击伤害光环
            if ((基础光环) and (暴击伤害光环))
            {
                int dst_id = halo::func_get_highest_halo(attacker, 2);
                if (dst_id != -1)
                {
                    int percent = 0;
                    auto dst = pk::get_unit(dst_id);
                    percent += halo::func_光环效果(dst, 2);
                    크리티컬_공격력 += percent / 100.f;
                }
            }




            // 部队閒战斗  부대간 전투 (병력)
            if (target_unit !is null)
            {
                info.dst_troops = target_unit.troops;

                bool fubing = false;
                if (开启疑兵伏兵)
                {
                    fubing = 疑兵伏兵::is_yibing_triggered(info, attacker, tactics_id, target_unit, type, critical, ambush, rettype);
                    if (fubing) return;
                }

                float troops_damage;
                def = target_unit.attr.stat[部队能力_防御];

                // 特技攻防修正
                if (attacker.weapon == 兵器_枪 && pk::get_distance(attacker.pos, target_unit.pos) == 1 && ch::has_skill(attacker, 特技_摧锋))
                    def = int(def * (100 - 摧锋破甲比例) / 100.f);
                if (attacker.weapon == 兵器_弩 && pk::get_distance(attacker.pos, target_unit.pos) == 1 && ch::has_skill(attacker, 特技_轻甲))
                    atk = int(atk * (100 + 轻甲攻防提升比例) / 100.f);
                if (target_unit.weapon == 兵器_弩 && pk::get_distance(attacker.pos, target_unit.pos) == 1 && ch::has_skill(target_unit, 特技_轻甲))
                    def = int(def * (100 + 轻甲攻防提升比例) / 100.f);
                if (target_unit.weapon == 兵器_戟 && ch::has_skill(target_unit, 特技_重甲))
                    def = int(def + 重甲防御提升量);                   
                if (target_unit.weapon >= 兵器_冲车 && target_unit.weapon <= 兵器_投石 && ch::has_skill(target_unit, 特技_重器))
                    def = int(def * (100 + 重器防御提升比例) / 100.f);
                def = pk::min(255, def);

                //ch::debug(pk::format("atk_troops:{}, def_troops:{}", attacker.troops, target_unit.troops));
                int atk_ability = 0;
                for (int m = 0; m < 3; m++)
                {
                    if (pk::is_valid_person_id(attacker.member[m]))
                    {
                        pk::person@ member_t = pk::get_person(attacker.member[m]);  //队伍中的武将
                        atk_ability += member_t.stat[武将能力_武力];
                    }
                }
                //pk::trace(pk::format("伤害计算传入前def{}", def));
                troops_damage = batt::get_troops_damage(atk, command, troops_atk, buffed, def, batt::get_command(target_unit), atk_ability);


                switch (weapon_id)
                {
                case 兵器_枪:
                    if (pk::has_tech(force, 기교_정예창병))
                        troops_damage *= 정예기교_공격력; // 1.15f
                    else if (pk::has_tech(force, 기교_창병단련))
                        troops_damage *= 단련기교_공격력; // 1.10f;

                    // 위압 특기 (위압)
                    if (兵种特技特殊效果 and ch::has_skill(attacker, 特技_威压))
                    {
                        troops_damage *= 兵种特技伤害系数;

                        if (rettype != 15 and 显示对话框 and pk::is_player_controlled(attacker))
                        {
                            //pk::say(pk::encode("날카로운 창끝을 받아라"), pk::get_person(attacker.leader));
                            @msg_unit = @attacker;
                            @msg_person = pk::get_person(attacker.leader);
                            msg_string = "接受锋利的尖刃罢";
                        }
                    }

                    break;

                case 兵器_戟:
                    if (pk::has_tech(force, 기교_정예극병))
                        troops_damage *= 정예기교_공격력; // 1.15f
                    else if (pk::has_tech(force, 기교_극병단련))
                        troops_damage *= 단련기교_공격력; // 1.10f;

                    // 이민족 특기 (친만, 친월)
                    if (兵种特技特殊效果 and (ch::has_skill(attacker, 특기_친만) or ch::has_skill(attacker, 특기_친월)))
                    {
                        troops_damage *= 兵种特技伤害系数;

                        if (rettype != 15 and 显示对话框 and pk::is_player_controlled(attacker))
                        {
                            if (ch::has_skill(attacker, 특기_친만))
                            {
                                @msg_unit = @attacker;
                                @msg_person = pk::get_person(attacker.leader);
                                msg_string = "这就是南蛮军的威力";
                            }
                            else if (ch::has_skill(attacker, 특기_친월))
                            {
                                @msg_unit = @attacker;
                                @msg_person = pk::get_person(attacker.leader);
                                msg_string = "这就是山越军的威力";
                            }
                        }
                    }

                    break;

                case 兵器_弩:
                    if (pk::has_tech(force, 기교_정예노병))
                        troops_damage *= 정예기교_공격력; // 1.15f
                    else if (pk::has_tech(force, 기교_노병단련))
                        troops_damage *= 단련기교_공격력; // 1.10f;

                    break;

                case 兵器_战马:
                    if (pk::has_tech(force, 기교_정예기병))
                        troops_damage *= 정예기교_공격력; // 1.15f
                    else if (pk::has_tech(force, 기교_기병단련))
                        troops_damage *= 단련기교_공격력; // 1.10f;

                    // 이민족 특기 (친강, 친오) : 근접 기병공격만 적용 ('18.10.28)
                    if (兵种特技特殊效果 and (ch::has_skill(attacker, 특기_친강) or ch::has_skill(attacker, 특기_친오)) and (pk::get_distance(attacker.pos, target_unit.pos) <= 1))
                    {
                        troops_damage *= 兵种特技伤害系数;

                        if (rettype != 15 and 显示对话框 and pk::is_player_controlled(attacker))
                        {
                            if (ch::has_skill(attacker, 특기_친강))
                            {
                                @msg_unit = @attacker;
                                @msg_person = pk::get_person(attacker.leader);
                                msg_string = "这就是羌族骑兵的威力";
                            }
                            else if (ch::has_skill(attacker, 특기_친오))
                            {
                                @msg_unit = @attacker;
                                @msg_person = pk::get_person(attacker.leader);
                                msg_string = "这就是乌丸骑兵的威力";
                            }
                        }
                    }

                    // 백마_특기 : 화살공격력 상향 ('18.10.28), 반격에 미적용 ('18.11.18)
                    if (兵种特技特殊效果 and ch::has_skill(attacker, 특기_백마) and !ch::has_skill(target_unit, 특기_백마) and (pk::get_distance(attacker.pos, target_unit.pos) > 1) and type == 0)
                    {
                        if (pk::has_tech(force, 기교_기사))  // 기교_기사 개발 시 데미지 증가
                            troops_damage *= 兵种特技伤害系数;
                        else
                            troops_damage *= 1.1f;

                        if (rettype != 15 and 显示对话框 and pk::is_player_controlled(attacker))
                        {
                            @msg_unit = @attacker;
                            @msg_person = pk::get_person(attacker.leader);
                            msg_string = "这就是白马骑兵阵的威力";

                        }
                    }

                    break;

                case 兵器_走舸:
                case 兵器_楼船:
                case 兵器_斗舰:
                    if (ch::has_skill(attacker, 특기_조타) or ch::has_skill(attacker, 특기_추진))
                        troops_damage *= 操舵推进伤害系数;
                    break;
                }


                if (pk::is_in_water(attacker) and (attacker.type == 部队类型_战斗))
                {
                    if (pk::is_valid_tactics_id(tactics_id) and pk::equipment_id_to_heishu(target_unit.weapon) == 병종_병기)
                        troops_damage *= 兵器水上伤害系数;
                }

                if (兵种特技特殊效果 and ch::has_skill(attacker, 特技_解毒))
                {
                    if (batt::is_弓箭战法(tactics_id) && !ch::has_skill(target_unit, 特技_解毒))
                    {
                        troops_damage *= 兵种特技伤害系数;

                        if (rettype != 15 and 显示对话框 and pk::is_player_controlled(attacker))
                        {
                            @msg_unit = @attacker;
                            @msg_person = pk::get_person(attacker.leader);
                            msg_string = "让你见识下毒箭的威力";
                        }
                    }
                }


                // 전략거점 점유 세력은 크리티컬 공격력 버프 적용 ('18.11.5)
                if (战略据点修正)
                {
                    if (batt::get_tactics_city_buff(attacker, weapon_id))
                        크리티컬_공격력 += 0.05f;
                }

                if (type == 0)  // 主动攻击
                {
                    if (info.critical)  //暴击
                        troops_damage *= 크리티컬_공격력; // 1.15f;
                    info.troops_damage = int(troops_damage);
                    if (batt::damage_avoided(info, attacker, target_unit, tactics_id))
                        troops_damage = 0;

                    // 兵粮袭击
                    info.food_damage = batt::get_food_captured(attacker, target_unit);
                    info.food_heal = info.food_damage;
                }
                else  //  反击
                {
                    if (type == 1)
                        troops_damage *= 0.9f;
                    else if (type == 2)
                        troops_damage *= 0.8f;


                    if (ch::has_skill(attacker, 特技_陷阵))
                        troops_damage *= 陷阵伤害倍率 / 100.f;

                    // 반격이기 때문에 target_unit은 공격을 실행한 부대를 뜻함.
                    if (pk::is_in_water(target_unit))
                    {
                        if (ch::has_skill(target_unit, 특기_강습) and pk::rand_bool(int(pk::core::skill_constant(target_unit, 특기_강습))))
                        {   //强袭
                            troops_damage = 0;
                            info.def_skill = 특기_강습;
                        }
                    }
                    else
                    {   //急袭
                        if (ch::has_skill(target_unit, 특기_급습) and (pk::get_hex(target_unit.pos).terrain == 지형_숲 or pk::rand_bool(int(pk::core::skill_constant(target_unit, 특기_급습)))))
                        {
                            troops_damage = 0;
                            info.def_skill = 특기_급습;
                        }
                    }
                }

                //蓄势特技
                if (pk::is_valid_tactics_id(tactics_id) && ch::has_skill(attacker, 特技_蓄势))
                    troops_damage = troops_damage * (100 + 蓄势增加伤害) / 100.f;

                // 最终伤害修正
                //pk::trace(pk::format("troops_damage before {}", troops_damage));
                troops_damage = batt::get_troops_final_damage(troops_damage, attacker, target_unit, info.debuffer);
                //pk::trace(pk::format("troops_damage after {}", troops_damage));
                //特技重甲免疫弓箭伤害
                if (pk::get_distance(attacker.pos, target_unit.pos) > 1 && (!pk::is_valid_tactics_id(tactics_id) || not info.tactics_hit))
                {
                    if (ch::has_skill(target_unit, 特技_重甲) and pk::rand_bool(50))
                    {
                        info.def_tech = 기교_화살방패;
                        troops_damage = 0;
                    }
                }

                info.troops_damage = int(troops_damage);

                // 原破竹所在位置

                // 背后攻击一定机率混乱  후방피격시 혼란효과 적용
                int damage_dir = batt::get_unit_direction(attacker, target_unit);
                if (后方攻击_启用混乱 and type == 0 and damage_dir == 3 and int(troops_damage) > 0)
                    @back_unit = target_unit;

                // 气力增减（包括特技）
                info.energy_heal = batt::get_energy_heal(troops_damage, attacker, target_unit, type);
                info.energy_damage = batt::get_energy_damage(troops_damage, attacker, target_unit, type);

                // 劫财设定
                if (rettype != 15)
                {
                    if (ch::has_skill(attacker, 特技_劫财))
                    {
                        gold_heal = batt::get_gold_captured(attacker, target_unit);
                        gold_damage = -gold_heal;
                        @gaingold_unit = @attacker;
                        @gold_unit = @target_unit;
                        //pk::add_gold(attacker, gold_heal, true);
                        //pk::add_gold(target_unit, gold_damage, true);
                    }
                }

                // 부대공격 실패 시 메시지 추가 ('19.2.17)
                if (rettype != 15 and type == 0 and troops_damage == 0 and !pk::is_campaign())
                {
                    @msg_unit = @attacker;
                    @msg_person = pk::get_person(attacker.leader);
                    msg_string = "攻击行不通嘛";
                }

            }
            // 攻城（兵力）
            else if (target_building !is null and pk::is_general_type(target_building))
            {
                info.dst_troops = pk::get_troops(target_building);

                int facility_id = target_building.facility;
                int /*troops_atk, */dst_atk, dst_def, dst_troops;

                // 据点防御力计算
                batt::get_city_def(troops_atk, dst_atk, dst_def, dst_troops, target_building);
                // 基本杀伤
                float troops_damage = batt::get_troops_damage(atk, command, troops_atk, buffed, dst_def, dst_troops);

                if (attacker.weapon == 兵器_井阑)  troops_damage *= 井栏攻击据点兵力 / 100.f;
                else if (attacker.weapon == 兵器_投石)  troops_damage *= 投石攻击据点兵力 / 100.f;
                else if (attacker.weapon == 兵器_战马)  troops_damage *= 骑兵攻击据点兵力 / 100.f;

                if (facility_id == 시설_도시)  troops_damage *= 部队防御倍率_城 / 100.f;
                else if (facility_id == 시설_관문)  troops_damage *= 部队防御倍率_关 / 100.f;
                else if (facility_id == 시설_항구)  troops_damage *= 部队防御倍率_港 / 100.f;

                if (attacker.has_tech(기교_운제))  //云梯
                {
                    if (attacker.weapon >= 兵器_剑 and attacker.weapon < 兵器_战马)
                        troops_damage *= 云梯步兵攻城倍率 / 100.f;
                    else
                        troops_damage *= 云梯其余攻城倍率 / 100.f;
                }


                if (info.critical)  //暴击
                    troops_damage *= 크리티컬_공격력;


                // 특기 발동 시 전법 유효 사거리 및 지형조건 확인, 초특기연전 전법사용 패치용 ('19.11.10)
                if (초특기연전_발동설정 and attacker.has_skill(특기_연전) and pk::is_valid_tactics_id(tactics_id) and !pk::is_campaign())
                {
                    if (pk::is_alive(target_unit) and !func_tactics_valid_target(attacker, target_unit, tactics_id))
                    {
                        info.critical = false;      // 통상공격
                        info.tactics_hit = false;   // 전법실패
                        info.troops_damage = 0;     // 데미지무효
                        info.energy_damage = 0;     // 데미지무효
                        info.food_damage = 0;       // 데미지무효
                        info.energy_heal = 0;       // 데미지무효
                        info.food_heal = 0;         // 데미지무효
                    }
                }

                // 最终伤害修正
                troops_damage = batt::get_troops_final_damage(troops_damage, attacker, target_building);
                info.troops_damage = int(troops_damage);

            }
            // 攻城（耐久）
            if (target_building !is null)
            {
                int facility_id = target_building.facility;
                int hp_atk = 0;
                float hp_damage = 0;

                if (pk::is_valid_tactics_id(tactics_id))    //攻城战法
                    hp_atk = pk::get_tactics(tactics_id).hp_atk;
                else if (pk::is_neighbor_pos(attacker.get_pos(), target_pos))   //直接攻击
                    hp_atk = 15;
                else    //間接攻击
                    hp_atk = 5;


                // 基本耐久伤害
                hp_damage = batt::get_hp_damage(attacker.weapon, atk, command, hp_atk, buffed);

                // 暴击
                if (info.critical)
                    hp_damage *= 크리티컬_공격력;

                // 조건별 보정
                if (facility_id >= 시설_불씨 and facility_id <= 시설_업화종)
                    hp_damage *= 耐久防御倍率_陷 / 100.f;
                else if (facility_id >= 시설_시장 and facility_id <= 시설_조선3단)
                    hp_damage *= 耐久防御倍率_内政 / 100.f;
                else if (facility_id == 시설_제방)
                    hp_damage *= 耐久防御倍率_提防 / 100.f;
                else if (attacker.weapon != 兵器_冲车 and attacker.weapon != 兵器_木兽)
                {
                    switch (facility_id)
                    {
                    case 시설_도시:     hp_damage *= 耐久防御倍率_城 / 100.f;   break;
                    case 시설_관문:     hp_damage *= 耐久防御倍率_关 / 100.f;   break;
                    case 시설_항구:     hp_damage *= 耐久防御倍率_港 / 100.f;   break;
                    case 시설_토루:     hp_damage *= 耐久防御倍率_土垒 / 100.f; break;
                    case 시설_석벽:     hp_damage *= 耐久防御倍率_石墙 / 100.f; break;
                    case 시설_진:       hp_damage *= 耐久防御倍率_阵 / 100.f;   break;
                    case 시설_요새:     hp_damage *= 耐久防御倍率_砦 / 100.f;   break;
                    case 시설_성채:     hp_damage *= 耐久防御倍率_城塞 / 100.f; break;
                    }

                }

                // 非攻城兵种获得破碎特技后的削弱 ('20.4.11)
                if (attacker.weapon <= 兵器_战马 and tactics_id == 전법_공성파쇄)
                    hp_damage *= 0.3f;

                //  骑兵攻城能力削弱
                if (facility_id <= 시설_항구 and attacker.weapon == 兵器_战马)
                    hp_damage *= 骑兵攻击据点耐久 / 100.f;
                // 防御技巧对攻城武器的防御加强
                if (facility_id <= 시설_항구 and (attacker.weapon >= 兵器_冲车 and attacker.weapon <= 兵器_木兽))
                {
                    if (target_building.has_tech(技巧_城壁强化))
                        hp_damage *= 耐久防御倍率_强化城墙 / 100.f;
                    else if (target_building.has_tech(技巧_设施强化))
                        hp_damage *= 耐久防御倍率_强化设施 / 100.f;
                }


                if (facility_id == 시설_제방 and not target_building.completed)
                    hp_damage = 0;

                if (attacker.has_tech(기교_운제))  //云梯
                {
                    if (attacker.weapon >= 兵器_剑 and attacker.weapon < 兵器_战马)
                        hp_damage *= 云梯步兵攻城倍率 / 100.f;
                    else
                        hp_damage *= 云梯其余攻城倍率 / 100.f;
                }


                hp_damage = batt::get_hp_final_damage(hp_damage, attacker, target_building);
                int final_hp_damage = int(hp_damage);

                //因为已经加入了建筑破坏后trigger，所以此处去掉
                //ch::特殊地名争夺处理(target_building, attacker, destroyed_facility_id, destroy_unit_id, final_hp_damage, destroyed_building_pos, destroy_flag);

                info.hp_damage = final_hp_damage;
            }

            info.tactics = tactics_id;
            info.src_pos = attacker.get_pos();
            info.dst_pos = target_pos;


            // 螺旋突眩晕概率('19.2.21)--keehl102大佬已改写相关函数，此处不用

            //战法_受伤_概率::func_致伤判断(info, attacker, tactics_id, target_pos, type, critical, ambush);

        }

        //------------------------------------------------------------------------------------------
        // 确认战法启动时的射程/地形是否符合有效条件전법 발동 시 사거리/지형 유효 조건인지 확인 ('19.11.10)
        bool func_tactics_valid_target(pk::unit@ attacker, pk::unit@ target_unit, int tactics_id)
        {
            int combat_range = pk::get_distance(attacker.pos, target_unit.pos);
            int terrain_id = pk::get_hex(target_unit.pos).terrain;

            pk::tactics@ tactics_t = pk::get_tactics(tactics_id);
            int  max_range = tactics_t.max_range;
            int  min_range = tactics_t.min_range;
            if (max_range == 1 and combat_range > max_range) return false;
            /*
            bool terrain_valid;
            if (terrain_id == 지형_숲 and attacker.has_skill(특기_사수)
                and ((전법_화시 <= tactics_id and tactics_id <= 전법_난사) or tactics_id == 전법_공성화시 or tactics_id == 전법_함선화시))
                terrain_valid = true;
            else
                terrain_valid = tactics_t.terrain[terrain_id];
            if (!terrain_valid) return false;
            */
            return true;
        }

        //------------------------------------------------------------------------------------------
        // AI部队战法变更函数 AI부대 전법 변경 함수 ('19.11.21)
        int func_dir_tactics_opt(pk::unit@ attacker, pk::unit@ target_unit, int tactics_id)
        {
            int tactics_opt = tactics_id;   // 기본값은 입력값
            if (!pk::is_alive(target_unit)) return tactics_id;
            if (attacker.weapon > 兵器_战马 or attacker.weapon == 兵器_弩) return tactics_id;
            if (pk::get_current_turn_force_id() != attacker.get_force_id()) return tactics_id;

            int atk_dist = pk::get_distance(attacker.pos, target_unit.pos); // 교전거리
            int atk_dir = pk::get_direction(attacker.pos, target_unit.pos); // 공격방향
            int def_dir = pk::get_direction(target_unit.pos, attacker.pos); // 피격방향

            pk::point pos_b1 = pk::get_neighbor_pos(attacker.pos, def_dir, 1);
            pk::point pos_n1 = pk::get_neighbor_pos(target_unit.pos, atk_dir, 1);
            pk::point pos_n2 = pk::get_neighbor_pos(target_unit.pos, atk_dir, 2);

            pk::unit@ unit_n1 = (pk::is_valid_pos(pos_n1)) ? pk::get_unit(pos_n1) : null;
            pk::unit@ unit_n2 = (pk::is_valid_pos(pos_n2)) ? pk::get_unit(pos_n2) : null;
            bool enemy_n1 = (pk::is_alive(unit_n1)) ? pk::is_enemy(attacker, unit_n1) : false;
            bool enemy_n2 = (pk::is_alive(unit_n2)) ? pk::is_enemy(attacker, unit_n2) : false;

            pk::hex@ hex_n1 = (pk::is_valid_pos(pos_n1)) ? pk::get_hex(pos_n1) : null;
            pk::hex@ hex_n2 = (pk::is_valid_pos(pos_n2)) ? pk::get_hex(pos_n2) : null;
            bool empty_n1 = (hex_n1 !is null) ? (!hex_n1.has_unit and !hex_n1.has_building) : false;
            bool empty_n2 = (hex_n2 !is null) ? (!hex_n2.has_unit and !hex_n2.has_building) : false;

            bool fire_n0 = pk::is_on_fire(target_unit.pos);
            bool fire_n1 = (pk::is_valid_pos(pos_n1)) ? pk::is_on_fire(pos_n1) : false;
            bool fire_n2 = (pk::is_valid_pos(pos_n2)) ? pk::is_on_fire(pos_n2) : false;
            bool fire_b1 = (pk::is_valid_pos(pos_b1)) ? pk::is_on_fire(pos_b1) : false;

            if (pk::is_valid_tactics_id(tactics_id) and (atk_dist == 1))    // 근접공격, 통상/일제공격 제외
            {
                //----- 창병 전법 (나선첨, 이단첨, 돌출)
                // 전법_나선첨 조건 확인 (전법사용가능, 기력충족, 근접, 대상부대가 통상상태)
                if (check_tactics_enable(attacker, target_unit, 전법_나선첨) and target_unit.status == 部队状态_通常)
                    tactics_opt = 전법_나선첨;

                // 전법_이단첨 조건 확인 (전법사용가능, 기력충족, 근접, 충돌대상부대 유효)
                else if (check_tactics_enable(attacker, target_unit, 전법_이단첨) and empty_n1 and (enemy_n2 or fire_n2))
                    tactics_opt = 전법_이단첨;

                // 전법_돌출 조건 확인 (전법사용가능, 기력충족, 근접, 충돌대상부대 유효)
                else if (check_tactics_enable(attacker, target_unit, 전법_돌출) and (empty_n1 or enemy_n1 or fire_n1))
                    tactics_opt = 전법_돌출;

                //----- 기병 전법 (돌파, 돌진, 돌격)
                // 전법_돌파 조건 확인 (전법사용가능, 기력충족, 근접, 돌파후 위치 빈칸)
                else if (check_tactics_enable(attacker, target_unit, 전법_돌파) and empty_n1 and !enemy_n2 and !fire_n1)
                    tactics_opt = 전법_돌파;

                // 전법_돌진 조건 확인 (전법사용가능, 기력충족, 근접, 돌진후 위치 빈칸)
                else if (check_tactics_enable(attacker, target_unit, 전법_돌진) and empty_n1 and !fire_n1 and (empty_n2 or fire_n2))
                    tactics_opt = 전법_돌진;

                // 전법_돌격 조건 확인 (전법사용가능, 기력충족, 근접)
                else if (check_tactics_enable(attacker, target_unit, 전법_돌격) and !fire_n0)
                    tactics_opt = 전법_돌격;

                //----- 극병 전법 (횡치, 선풍, 갈퀴)
                // 전법_횡치 조건 확인 (전법사용가능, 기력충족, 근접, 인접한 적부대 확인)
                else if (check_tactics_enable(attacker, target_unit, 전법_횡치) and (count_neighbor_enemies(attacker, target_unit, 전법_횡치) >= 1))
                    tactics_opt = 전법_횡치;

                // 전법_선풍 조건 확인 (전법사용가능, 기력충족, 근접, 인접한 적부대 확인)
                else if (check_tactics_enable(attacker, target_unit, 전법_선풍) and (count_neighbor_enemies(attacker, target_unit, 전법_선풍) >= 3))
                    tactics_opt = 전법_선풍;

                // 전법_갈퀴 조건 확인 (전법사용가능, 기력충족, 근접)
                else if (check_tactics_enable(attacker, target_unit, 전법_갈퀴) and !fire_b1)
                    tactics_opt = 전법_갈퀴;

            }

            // 로그창에 표시
            if (调试模式 and (tactics_id != tactics_opt))
            {
                string leader_name = pk::decode(pk::get_name(pk::get_person(attacker.leader)));
                pk::info(pk::format("{}部队战法: {} → {}", leader_name, get_tactics_name(tactics_id), get_tactics_name(tactics_opt)));
            }

            return tactics_opt;
        }

        // 弩兵通常使用普通攻击优化为使用火矢 노병 통상활공격 → 화시 최적화
        int func_fire_on_arrow(pk::unit@ attacker, pk::hex_object@ target_obj, int tactics_id)
        {
            int tactics_opt = tactics_id;   // 기본값은 입력값
            if (!pk::is_alive(target_obj)) return tactics_id;
            if (attacker.weapon != 兵器_弩) return tactics_id;
            if (pk::get_current_turn_force_id() != attacker.get_force_id()) return tactics_id;
            int atk_dist = pk::get_distance(attacker.pos, target_obj.pos); // 교전거리

            if (atk_dist > 1 and !pk::is_valid_tactics_id(tactics_id))
            {
                int tekisei = pk::get_tekisei(attacker, attacker.weapon);
                pk::equipment@ atk_eqp = pk::get_equipment(attacker.weapon);
                pk::tactics@ fire_arrow = pk::get_tactics(전법_화시);
                pk::hex@ hex_dst = pk::get_hex(target_obj.pos);

                // 화시전법사용가능여부 : 병과, 기력, 적성, 지형 체크
                bool chk_eqp = atk_eqp.tactics[전법_화시];
                bool chk_tekisei = tekisei >= fire_arrow.tekisei;
                bool chk_energy = (attacker.energy >= fire_arrow.energy_cost);
                bool chk_range = (int(fire_arrow.min_range) <= atk_dist and atk_dist <= int(fire_arrow.max_range));
                bool chk_terrain = fire_arrow.terrain[hex_dst.terrain];
                if (chk_eqp and chk_tekisei and chk_energy and chk_range and chk_terrain)
                    tactics_opt = 전법_화시;
            }
            return tactics_opt;
        }


        bool check_tactics_enable(pk::unit@ src, pk::unit@ dst, int tactics_id)
        {
            bool tactics_en = false;
            int tekisei = pk::get_tekisei(src, src.weapon);
            pk::equipment@ atk_eqp = pk::get_equipment(src.weapon);
            pk::tactics@ tactics = pk::get_tactics(tactics_id);
            pk::hex@ hex_dst = pk::get_hex(dst.pos);

            // 전법사용가능여부 : 병과, 기력, 적성, 지형 체크
            tactics_en = (atk_eqp.tactics[tactics_id] and (src.energy >= tactics.energy_cost) and (tekisei >= tactics.tekisei) and tactics.terrain[hex_dst.terrain]);

            return tactics_en;
        }

        int count_neighbor_enemies(pk::unit@ src, pk::unit@ dst, int tactics_id)
        {
            int count = 0;
            for (int i = 0; i < 방향_끝; i++)
            {
                pk::point pos_t = pk::get_neighbor_pos(src.pos, i);
                pk::unit@ unit_t = (pk::is_valid_pos(pos_t)) ? pk::get_unit(pos_t) : null;
                if (pk::is_valid_pos(pos_t) and pk::is_alive(unit_t) and pk::is_enemy(src, unit_t) and unit_t.get_id() != dst.get_id())
                {
                    if (tactics_id == 전법_횡치 and pk::get_distance(dst.pos, unit_t.pos) == 1)
                        count++;
                    else if (tactics_id == 전법_선풍)
                        count++;
                }
            }
            return count;
        }

        bool check_splash_valid(pk::unit@ src, pk::unit@ dst)
        {
            int count_force = 0;
            int count_enemy = 0;
            for (int i = 0; i < 방향_끝; i++)
            {
                pk::point pos_t = pk::get_neighbor_pos(dst.pos, i);
                pk::unit@ unit_t = (pk::is_valid_pos(pos_t)) ? pk::get_unit(pos_t) : null;
                pk::hex@ hex_t = pk::get_hex(pos_t);
                if (pk::is_valid_pos(pos_t) and pk::is_alive(unit_t) and unit_t.get_id() != dst.get_id())
                {
                    if (!pk::is_enemy(src, unit_t)) count_force++;
                    if (pk::is_enemy(src, unit_t))
                    {
                        if (hex_t.terrain != 지형_숲) count_enemy++;
                        if (hex_t.terrain == 지형_숲 and src.has_skill(특기_사수)) count_enemy++;
                    }
                }
            }
            if (count_enemy >= pk::max(2, (count_force + 1))) return true;
            return false;
        }

        string get_tactics_name(int tactics_id)
        {
            pk::array<string> str_tactics = { "돌출", "나선첨", "이단첨", "갈퀴", "횡치", "선풍", "화시", "관시", "난사", "돌격", "돌파", "돌진", "공성화시", "공성파쇄", "공성방사", "공성투석", "함선화시", "함선격돌", "함선투석" };
            if (tactics_id < 0 or tactics_id >= 전법_끝) return "통상";
            return str_tactics[tactics_id];
        }


        //------------------------------------------------------------------------------------------
        // 부대 경험치 획득 시 추가처리 
        //------------------------------------------------------------------------------------------

        // 전용 메시지 처리용 변수
        pk::unit@   msg_unit;
        pk::person@ msg_person;
        string      msg_string = "";

        // 후방피격 혼란처리용 변수
        pk::unit@ back_unit;

        // 追加劫财的部队信息
        pk::unit@ gold_unit;
        pk::unit@ gaingold_unit;
        int gold_heal;
        int gold_damage;

        // 경험치 획득 시 처리 함수
        void onUnitExpAdd(pk::unit@ unit, int type)
        {
            // 커스텀 대화표시
            if (显示对话框 and pk::is_alive(unit) and pk::is_alive(msg_unit) and unit.get_id() == msg_unit.get_id())
            {
                pk::say(pk::encode(msg_string), msg_person);
            }
            @msg_unit = null;
            @msg_person = null;
            msg_string = "";

            //pk::trace(pk::format("攻击部队id:{}.劫财量：{}",unit.get_id(), gold_heal));
            if (gold_heal != 0 and gaingold_unit.get_id() == unit.get_id())
            {
                pk::add_gold(unit, gold_heal, true);
                pk::add_gold(gold_unit, gold_damage, true);
                gold_heal = 0;
                gold_damage = 0;
                @gold_unit = null;
                @gaingold_unit = null;
            }

            //pk::trace(pk::format("攻击部队id:{}",destroy_unit.get_id()));	
            //pk::trace(pk::format("经验获得部队id：{}",unit.get_id()));

            //已经有建筑物破坏恢复trigger下，不需要这个
            //ch::特殊地名设施_重建(unit, destroy_unit_id, destroyed_facility_id, destroyed_building_pos, destroy_flag, 209);

            // 후방피격 혼란처리 背后攻击混乱 添加治军
            if (后方攻击_启用混乱 and pk::is_alive(unit) and pk::is_alive(back_unit))
            {
                if (pk::rand_bool(后方攻击_混乱概率) and back_unit.status == 部队状态_通常)
                {
                    if (!ch::has_skill(back_unit, 130))
                    {
                        //ch::debug("back");
                        pk::set_status(back_unit, unit, 部队状态_混乱, 1, true);
                        pk::say(pk::encode("呼.成功的扰乱了敌方."), pk::get_person(unit.leader));
                    }
                }
            }
            @back_unit = null;

        }
 
        bool 调试模式 = false;

    }

    Main main;
}



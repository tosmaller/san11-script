// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/12/25 # 江东新风 # 高度差伤害异常修复 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文 ##
// ## 2021/05/09 # 氕氘氚 # 修正KRE无法识别兵种克制关系的问题 ##
// ## 2021/02/18 # 江东新风 # 耐久伤害添加坚城影响 ##
// ## 2020/12/12 # 江东新风 # 修正参数类型错误 ##
// ## 2020/11/16 # 氕氘氚 # 修改伤害计算公式 ##
// ## 2020/10/27 # 氕氘氚 # 新特技判断等 ##
// ## 2020/09/23 # 江东新风 # utf8版本u8debug ##
// ## 2020/08/16 # 氕氘氚 #  ##
// @@ 2018/10/21 @ 기마책사 @@

// 修正因子开关 ----------------------------------------------------------------
const bool 气力影响设定 = true;   // 根据气力修正伤害 기력에 비례하여 데미지 증감 보정 효과
const bool 气力增减设定 = true;   // 攻击加气，被攻减气 교전 시 데미지에 따라 공격부대 기력증가, 피격부대 기력감소
const bool 地形特征修正 = true;   // 地形特征、地势高低修正伤害 교전 시 지형종류 및 지형고저 영향 반영여부
const bool 周边部队加成 = true;   // 周围部队中有我军君主、都督或是有神算、虚实特技时修正伤害 주변 아군부대가 군주 또는 도독이거나, 특기_신산, 특기_허실 보유 시 보정효과
const bool 交战朝向修正 = true;   // 部队攻击方向修正伤害 교전 시 부대 공격 방향에 따른 데미지 보정효과 적용
const bool 战略据点修正 = true;   // 势力占有特定城市时对应兵种暴击伤害增加 병과별 특정한 전략거점 점유한 세력의 부대는 크리티컬 공격력 증가 
const bool 兵种特技特殊效果 = true;   // 兵种特技伤害修正 병과별 특기에 따른 데미지 보정 효과 적용
const bool 据点能力合算 = true;   // 据点中能力合算，true时所有武将能力合算，否则仅太守能力生效 true: 거점 내 무장들의 능력치 합산 기능 적용, false: 기존처럼 태수 능력만 적용
const bool 据点防御buff = true;   // 与我方据点相接的部队防御增加（效果同当前科技的防御设施） 거점(도시,관문,항구)에 인접한 부대의 데미지 방어 버프 설정
const bool 部队伤害上限 = true;
const bool 新版伤害公式 = true;

// 基础设定 ----------------------------------------------------------------
const int 部队基础伤害倍率 = 80;   // （基本）攻击力总体比例

const float 锻炼兵伤害系数 = 1.1;
const float 精锐兵伤害系数 = 1.15;
const float 暴击伤害系数 = 1.15;
const float 士气影响系数 = 0.2;

const float 兵种特技伤害系数 = 1.15;
const float 操舵推进伤害系数 = 1.15;
const float 兵器水上伤害系数 = 0.8;

const int 骑兵攻击据点兵力 = 80;
const int 骑兵攻击据点耐久 = 50;
const int 井栏攻击据点兵力 = 180;
const int 投石攻击据点兵力 = 120;
const int 云梯步兵攻城倍率 = 140;  //  剑枪戟弩
const int 云梯其余攻城倍率 = 120;

// 兵力防御倍率 ----------------------------------------------------------------
const int 部队防御倍率_城 = 55;
const int 部队防御倍率_关 = 45;
const int 部队防御倍率_港 = 60;

const int 周边部队防御倍率_阵 = 85;
const int 周边部队防御倍率_砦 = 75;
const int 周边部队防御倍率_城塞 = 65;

// 耐久防御倍率 ----------------------------------------------------------------
const int 耐久防御倍率_城 = 70;
const int 耐久防御倍率_关 = 60;
const int 耐久防御倍率_港 = 80;
const int 耐久防御倍率_阵 = 80;
const int 耐久防御倍率_砦 = 70;
const int 耐久防御倍率_城塞 = 60;
const int 耐久防御倍率_土垒 = 90;
const int 耐久防御倍率_石墙 = 70;
const int 耐久防御倍率_陷 = 160;
const int 耐久防御倍率_内政 = 110;
const int 耐久防御倍率_提防 = 70;

const int 耐久防御倍率_强化设施 = 90;
const int 耐久防御倍率_强化城墙 = 80;

// 建筑攻击力 ----------------------------------------------------------------
const int 建筑基础伤害倍率 = 80;   // 전체적인 대미지량을 비율로 조절
const int 建筑对攻城兵器伤害倍率 = 80;  //投石台除外
const int 防御强化建筑伤害倍率 = 200;
const int 基础攻击力_自动攻击 = 150;
const int 基础攻击力_城市 = 200;
const int 基础攻击力_关 = 400;
const int 基础攻击力_港口 = 200;
const int 基础攻击力_阵 = 100;
const int 基础攻击力_砦 = 200;
const int 基础攻击力_城塞 = 300;
const int 基础攻击力_箭楼 = 400;
const int 基础攻击力_连弩楼 = 600;
const int 基础攻击力_投石台 = 900;




namespace batt
{
    // ---------------------------------------------------------------------------------------
    const bool 显示对話 = false;

    const float 攻防光环_皇帝 = 0.15;
    const float 攻防光环_王 = 0.1;
    const float 攻防光环_公 = 0.1;
    const float 攻防光环_君主 = 0.05;
    const float 攻防光环_都督 = 0.05;
    const float 攻防光环_神算 = 0.05;
    const float 攻防光环_虚实 = 0.05;

    const float 方位伤害修正_正前 = 0.95;
    const float 方位伤害修正_侧前 = 1.0;
    const float 方位伤害修正_侧后 = 1.05;
    const float 方位伤害修正_正后 = 1.1;

	/*  克制倍率 = 兵种克制关系[攻击方][防守方]   */
    const array<array<float>> 兵种克制关系 =
    {
        /*枪     戟     弩     马      車     井     石     船*/
        /*枪*/{1.00f, 0.80f, 1.10f, 1.20f, 0.90f, 0.90f, 0.90f, 0.90f},
        /*戟*/{1.10f, 1.00f, 1.10f, 0.80f, 0.80f, 0.80f, 0.80f, 0.80f},
        /*弩*/{1.10f, 0.80f, 1.00f, 1.10f, 0.80f, 0.80f, 0.80f, 1.00f},
        /*马*/{0.80f, 1.10f, 1.20f, 1.00f, 0.80f, 0.80f, 0.80f, 0.80f},
        /*車*/{1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f},
        /*井*/{1.00f, 0.80f, 1.00f, 1.10f, 1.00f, 1.00f, 1.00f, 1.00f},
        /*石*/{0.90f, 0.90f, 0.90f, 0.90f, 1.20f, 1.20f, 1.20f, 1.10f},
        /*船*/{1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f}
    };

    // ---------------------------------------------------------------------------------------

    bool is_弓箭战法(int tactics_id)
    {
        if (tactics_id >= 战法_火矢 && tactics_id <= 战法_乱射 || tactics_id == 战法_攻城火矢 || tactics_id == 战法_舰艇火矢)
            return true;
        else
            return false;
    }

    // ---------------------------------------------------------------------------------------

    // 防御建筑类型
    int get_def_buff_building_id(pk::unit@ target_unit)
    {
        if (target_unit !is null)
        {
            pk::force@ target_force = pk::get_force(target_unit.get_force_id());
            int facility_id = 시설_진;
            if (pk::has_tech(target_force, 기교_시설강화))
                facility_id = 시설_요새;
            if (pk::has_tech(target_force, 기교_성벽강화))
                facility_id = 시설_성채;
            if (has_def_buff_building(target_unit.get_pos(), 1, pk::get_facility(facility_id).max_range, target_unit.get_force_id()))
                return facility_id;
        }
        return -1;
    }

    // 部队战斗朝向判断（返回0正，1正侧，2后侧，3后）
    int get_unit_direction(pk::unit@ attacker, pk::unit@ target_unit)
    {
        int dir_unit = target_unit.direction;    // 피격부대의 현재방향
        int dir_atk = pk::get_direction(target_unit.pos, attacker.pos);  // 교전시 피격방향

        int damage_dir = -1;

        switch (dir_unit)
        {
        case 방향_북서:
            if (dir_atk == 방향_북서)                      damage_dir = 0;   // 전면
            else if (dir_atk == 방향_북 or dir_atk == 방향_남서)  damage_dir = 1;   // 전측
            else if (dir_atk == 방향_북동 or dir_atk == 방향_남)  damage_dir = 2;   // 후측
            else if (dir_atk == 방향_남동)                      damage_dir = 3;   // 후면
            break;

        case 방향_북:
            if (dir_atk == 방향_북)                        damage_dir = 0;   // 전면
            else if (dir_atk == 방향_북서 or dir_atk == 방향_북동) damage_dir = 1;   // 전측 
            else if (dir_atk == 방향_남서 or dir_atk == 방향_남동) damage_dir = 2;   // 후측 
            else if (dir_atk == 방향_남)                        damage_dir = 3;   // 후면
            break;

        case 방향_북동:
            if (dir_atk == 방향_북동)                      damage_dir = 0;   // 전면
            else if (dir_atk == 방향_북 or dir_atk == 방향_남동)  damage_dir = 1;   // 전측
            else if (dir_atk == 방향_북서 or dir_atk == 방향_남)  damage_dir = 2;   // 후측
            else if (dir_atk == 방향_남서)                      damage_dir = 3;   // 후면
            break;

        case 방향_남서:
            if (dir_atk == 방향_남서)                      damage_dir = 0;   // 전면   
            else if (dir_atk == 방향_북서 or dir_atk == 방향_남)  damage_dir = 1;   // 전측
            else if (dir_atk == 방향_북 or dir_atk == 방향_남동)  damage_dir = 2;   // 후측
            else if (dir_atk == 방향_북동)                      damage_dir = 3;   // 후면
            break;

        case 방향_남:
            if (dir_atk == 방향_남)                        damage_dir = 0;   // 전면
            else if (dir_atk == 방향_남서 or dir_atk == 방향_남동) damage_dir = 1;   // 전측
            else if (dir_atk == 방향_북서 or dir_atk == 방향_북동) damage_dir = 2;   // 후측
            else if (dir_atk == 방향_북)                        damage_dir = 3;   // 후면
            break;

        case 방향_남동:
            if (dir_atk == 방향_남동)                      damage_dir = 0;   // 전면
            else if (dir_atk == 방향_남 or dir_atk == 방향_북동)  damage_dir = 1;   // 전측
            else if (dir_atk == 방향_남서 or dir_atk == 방향_북)  damage_dir = 2;   // 후측
            else if (dir_atk == 방향_북서)                      damage_dir = 3;   // 후면
            break;

        }

        return damage_dir;

    }

    // 朝向伤害修正
    float get_direction_buff(int damage_dir)
    {
        float damage_buf = 1.f;

        switch (damage_dir)
        {
        case 0: damage_buf = 方位伤害修正_正前; break;  // 전면 대미지 가중치
        case 1: damage_buf = 方位伤害修正_侧前; break;  // 전측 대미지 가중치
        case 2: damage_buf = 方位伤害修正_侧后; break;  // 후측 대미지 가중치
        case 3: damage_buf = 方位伤害修正_正后; break;  // 후면 대미지 가중치
        }

        return damage_buf;
    }

    // 拥有特定都市可以增加兵种战力
    bool get_tactics_city_buff(pk::unit@ attacker, int weapon_id)
    {
        int kunshu_id = pk::get_kunshu_id(attacker);
        switch (weapon_id)
        {
        case 병기_창:
            if (kunshu_id == pk::get_kunshu_id(pk::get_building(도시_복양)))
                return true;
            else if (kunshu_id == pk::get_kunshu_id(pk::get_building(도시_완)))
                return true;
            break;

        case 병기_극:
            if (kunshu_id == pk::get_kunshu_id(pk::get_building(도시_운남)))
                return true;
            else if (kunshu_id == pk::get_kunshu_id(pk::get_building(도시_장사)))
                return true;
            break;

        case 병기_노:
            if (kunshu_id == pk::get_kunshu_id(pk::get_building(도시_강릉)))
                return true;
            else if (kunshu_id == pk::get_kunshu_id(pk::get_building(도시_회계)))
                return true;
            break;

        case 병기_군마:
            if (kunshu_id == pk::get_kunshu_id(pk::get_building(도시_양평)))
                return true;
            else if (kunshu_id == pk::get_kunshu_id(pk::get_building(도시_무위)))
                return true;
            break;
        }
        return false;
    }

    // 周边友军的加成效果
    float get_ally_buff(pk::unit@ unit, bool show_dialog)
    {
        float neighbor_buf = 0.f;
        int neighbor_range = 1;

        pk::point pos = unit.get_pos();

        array<pk::point> arr = pk::range(pos, 1, neighbor_range);
        for (int j = 0; j < int(arr.length); j++)
        {
            pk::unit@ search_unit = pk::get_unit(arr[j]);
            if (search_unit !is null and !pk::is_enemy(unit, search_unit))  // 주변부대가 아군인 경우
            {
                // 아군부대장의 신분에 따른 버프 
                if (search_unit.leader == pk::get_kunshu_id(unit))  // 君主 아군부대가 군주이면 : 작위에 따라 버프량 및 대화 다르게 ('18.10.28)
                {
                    switch (pk::get_force(unit.get_force_id()).title)
                    {
                    case 爵位_皇帝:  //皇帝
                        neighbor_buf += 攻防光环_皇帝;   // 15% 버프
                        break;
                    case 작위_왕:  //王
                        neighbor_buf += 攻防光环_王;   // 10% 버프
                        break;
                    case 작위_공:  //公
                        neighbor_buf += 攻防光环_公;   // 5% 버프
                        break;
                    default:
                        neighbor_buf += 攻防光环_君主;   // 5% 버프
                        break;
                    }
                    if (显示对話 and show_dialog and pk::is_player_controlled(unit))
                    {
                        pk::say(pk::encode("与主公在一起"), pk::get_person(unit.leader));
                    }
                }
                else if (search_unit.leader == pk::get_totoku_id(unit))  //同军团都督 아군부대가 같은 군단 도독이면 ('18.10.28)
                {
                    neighbor_buf += 攻防光环_都督;   // 5% 버프

                    if (显示对話 and show_dialog and pk::is_player_controlled(unit))
                    {
                        pk::say(pk::encode("与都督同在"), pk::get_person(search_unit.leader));
                    }
                }

                // 아군부대장의 특기에 따른 버프
                if (ch::has_skill(search_unit, 특기_신산))  //拥有神算特技 아군부대가 특기_신산 보유 시 버프 ('18.10.28)
                {
                    neighbor_buf += 攻防光环_神算;   // 10% 버프

                    if (显示对話 and show_dialog and pk::is_player_controlled(unit))
                    {
                        pk::say(pk::encode("发动神算阵法"), pk::get_person(search_unit.leader));
                    }

                }
                if (ch::has_skill(search_unit, 특기_허실))  //虚实 아군부대가 특기_허실 보유 시 버프 ('18.10.28)
                {
                    neighbor_buf += 攻防光环_虚实;   // 5% 버프

                    if (显示对話 and show_dialog and pk::is_player_controlled(unit))
                    {
                        pk::say(pk::encode("展开虚虚实实的战略"), pk::get_person(search_unit.leader));
                    }
                }

            }
        }

        return neighbor_buf;
    }

    // 地势高差计算
    int get_height_diff(const pk::point& in src, const pk::point& in dst)
    {
        return pk::get_height_map(pk::hex_pos_to_height_map_pos(src) + 2).height - pk::get_height_map(pk::hex_pos_to_height_map_pos(dst) + 2).height;
    }

    // 地形对特定兵种的影响
    float get_terrain_buff(pk::unit @ unit)
    {
        int weapon_id = unit.weapon;
        float terrain_buf = 0.f;
        pk::hex@ unit_hex = pk::get_hex(unit.get_pos());    // 부대의 좌표
        switch (unit_hex.terrain)                           // 좌표의 지형
        {
        case 지형_초원:
            if (weapon_id == 병기_창)   terrain_buf = 0.05f;
            else if (weapon_id == 병기_군마)  terrain_buf = 0.10f;
            break;
        case 지형_땅:
            if (weapon_id == 병기_창)   terrain_buf = 0.05f;
            else if (weapon_id == 병기_노)   terrain_buf = 0.05f;
            else if (weapon_id == 병기_군마)  terrain_buf = 0.05f;
            break;
        case 지형_모래:
            if (weapon_id == 병기_창)   terrain_buf = 0.05f;
            else if (weapon_id == 병기_군마)  terrain_buf = 0.05f;
            break;
        case 지형_습지:
            if (weapon_id == 병기_창)   terrain_buf = -0.05f;
            else if (weapon_id == 병기_극)   terrain_buf = 0.10f;
            else if (weapon_id == 병기_노)   terrain_buf = -0.05f;
            else if (weapon_id == 병기_군마)  terrain_buf = -0.10f;
            break;
        case 지형_독천:
            if (weapon_id == 병기_창)   terrain_buf = -0.10f;
            else if (weapon_id == 병기_극)   terrain_buf = 0.05f;
            else if (weapon_id == 병기_노)   terrain_buf = -0.10f;
            else if (weapon_id == 병기_군마)  terrain_buf = -0.15f;
            break;
        case 지형_숲:
            if (weapon_id == 병기_창)   terrain_buf = 0.10f;
            else if (weapon_id == 병기_극)   terrain_buf = 0.05f;
            else if (weapon_id == 병기_노)   terrain_buf = 0.15f;
            else if (weapon_id == 병기_군마)  terrain_buf = -0.10f;
            break;
        case 지형_황무지:
            if (weapon_id == 병기_창)   terrain_buf = -0.05f;
            else if (weapon_id == 병기_군마)  terrain_buf = 0.05f;
            break;
        case 지형_대로:
            if (weapon_id == 병기_창)   terrain_buf = 0.05f;
            else if (weapon_id == 병기_극)   terrain_buf = 0.05f;
            else if (weapon_id == 병기_노)   terrain_buf = 0.05f;
            else if (weapon_id == 병기_군마)  terrain_buf = 0.05f;
            break;
        case 지형_산:
            if (weapon_id == 병기_창)   terrain_buf = 0.15f;
            else if (weapon_id == 병기_극)   terrain_buf = 0.15f;
            else if (weapon_id == 병기_노)   terrain_buf = 0.15f;
            else if (weapon_id == 병기_군마)  terrain_buf = 0.10f;
            break;
        }

        // 특기_매복 보유부대는 병종 관계없이 숲/산/샛길에서 15% 추가 어드밴티지 ('18.10.14)
        if (ch::has_skill(unit, 특기_매복) and (unit_hex.terrain == 지형_숲 or unit_hex.terrain == 지형_산 or unit_hex.terrain == 지형_샛길))
            terrain_buf += 0.15f;
        // 특기_해독 보유부대는 병종 관계없이 독천에서 15% 추가 어드밴티지 ('18.10.17)
        else if (ch::has_skill(unit, 특기_해독) and (unit_hex.terrain == 지형_독천))
            terrain_buf += 0.15f;
        // 특기_답파 보유부대는 병종 관계없이 독천에서 15% 추가 어드밴티지 ('18.10.17)
        else if (ch::has_skill(unit, 특기_답파) and (unit_hex.terrain == 지형_벼랑길 or unit_hex.terrain == 지형_산 or unit_hex.terrain == 지형_샛길))
            terrain_buf += 0.15f;

        return terrain_buf;
    }

    // 免伤特技/技巧效果，返回是否免伤  （func_5af230）
    bool damage_avoided(pk::damage_info& info, pk::unit@ attacker, pk::unit@ target, int tactics_id)
    {
        // 전법여부 판정 부분 위로 스크립트 이동 : 통상공격/전법공격 구분 없이 방어 특기 발동 ('18.10.9)
        if (ch::has_skill(target, 특기_불굴))  //不屈
        {
            if (int(target.troops) < int(pk::core::skill_constant(target, 특기_불굴)) and pk::rand_bool(int(pk::core::skill_constant(target, 특기_불굴, 1))))
            {
                info.def_skill = 특기_불굴;

                if (pk::is_valid_tactics_id(tactics_id))
                    info.tactics_hit = false;

                return true;
            }
        }

        if (ch::has_skill(target, 특기_금강))  //金刚
        {
            if (info.troops_damage < (int(pk::core::skill_constant(target, 특기_금강))) and pk::rand_bool(int(pk::core::skill_constant(target, 특기_금강, 1))))
            {
                info.def_skill = 특기_금강;
                if (pk::is_valid_tactics_id(tactics_id))
                    info.tactics_hit = false;

                return true;
            }
        }

        if (ch::has_skill(target, 特技_八卦))
        {
            if (pk::is_valid_tactics_id(tactics_id) and pk::rand_bool(八卦发动几率))
            {
                info.def_skill = 특기_철벽;
                info.tactics_hit = false;
                return true;
            }
        }

        // 방어특기 없을 경우 공격자 전법여부 확인 : 전법인 경우 false 반환 후 함수 종료, 평타인 경우 방어자 극병 기교효과 적용
        if (pk::is_valid_tactics_id(tactics_id))
            return false;
        else
        {
            pk::point attacker_pos = attacker.get_pos();
            pk::point target_pos = target.get_pos();

            if (target.weapon == 병기_극)
            {
                if (target.has_tech(기교_화살방패))
                {
                    if (!pk::is_neighbor_pos(attacker_pos, target_pos) and pk::rand_bool(矢盾发动机率))
                    {
                        info.def_tech = 기교_화살방패;
                        return true;
                    }
                }
                if (target.has_tech(기교_큰방패))
                {
                    if (pk::is_neighbor_pos(attacker_pos, target_pos) and pk::rand_bool(大盾发动机率))
                    {
                        info.def_tech = 기교_큰방패;
                        return true;
                    }
                }
            }
        }

        return false;
    }

    // 是否存在增加攻击的建筑（見data03号）   （func_5aed80）
    bool has_atk_buff_building(const pk::point& in pos, int force_id)
    {
        array<pk::point> range = pk::range(pos, 1, int(pk::core["facility.max_range"]));
        for (int i = 0; i < int(range.length); i++)
        {
            pk::building@ building = pk::get_building(range[i]);
            if (pk::is_alive(building) and building.completed and building.get_force_id() == force_id)
            {
                if (bool(pk::core["facility"][building.facility]["damage_buff"]) and pk::get_distance(pos, range[i]) <= int(pk::get_facility(building.facility).max_range))
                    return true;
            }
        }
        return false;
    }

    // 是否存在防御建筑 （func_5aedc0）
    bool has_def_buff_building(const pk::point& in pos, int min, int max, int force_id)
    {
        array<pk::point> range = pk::range(pos, min, max);
        for (int i = 0; i < int(range.length); i++)
        {
            pk::building@ building = pk::get_building(range[i]);
            if (pk::is_alive(building) and (building.facility == 시설_진 or building.facility == 시설_요새 or building.facility == 시설_성채) and building.completed and building.get_force_id() == force_id)
                return true;
            if (据点防御buff)
            {
                if (pk::is_alive(building) and (building.facility == 시설_도시 or building.facility == 시설_관문 or building.facility == 시설_항구) and building.get_force_id() == force_id)
                    return true;
            }

        }
        return false;
    }

    // 据点防御能力计算    （func_5af0e0）
    void get_city_def(int& out src_atk, int& out dst_atk, int& out dst_def, int& out dst_troops, pk::building@ building)
    {
        src_atk = 10;
        float dst_atk_t = 0;
        float dst_def_t = 0;
        float dst_troops_t = 0;
        pk::person@ taishu = pk::get_person(pk::get_taishu_id(building));
        if (pk::is_alive(taishu))
        {
            //태수 능력 반영 (기본)
            dst_atk_t = pk::max(taishu.stat[武将能力_武力] - 25, 40);
            dst_def_t = pk::max(taishu.stat[武将能力_统率] - 25, 40);
            dst_troops_t = pk::min(pk::get_troops(building), pk::get_command(taishu));

            //도시 내 "寻杓_腿至" 인 무장 능력 반영 (각 무장의 무력, 통솔, 지휘 부분 합산) ('18.10.7)
            if (据点能力合算)
            {
                float ilban_support = 3.5f;    // 태수 외 무장 능력 치 합산 보조율 : 무장별 5%
                auto ilban_list = pk::list_to_array(pk::get_person_list(building, pk::mibun_flags(身份_一般)));
                if (0 < ilban_list.length)
                {
                    for (int i = 0; i < int(ilban_list.length); i++)
                    {
                        pk::person@ ilban = ilban_list[i];
                        //출진 또는 부재중 상태가 아닌 장수이며, 태수와 혐오 관계가 아닌 장수만 능력 치 합산
                        if (!pk::is_unitize(ilban) and !pk::is_absent(ilban) and !pk::is_dislike(taishu, ilban.get_id()) and !pk::is_dislike(ilban, taishu.get_id()))
                        {
                            dst_atk_t += (pk::max(ilban.stat[武将能力_武力] - 25, 20)) * (ilban_support / 100.f);
                            dst_def_t += (pk::max(ilban.stat[武将能力_统率] - 25, 20)) * (ilban_support / 100.f);
                            dst_troops_t += (pk::min(pk::get_troops(building), pk::get_command(ilban))) * (ilban_support / 100.f);
                        }
                    }
                }
            }
            dst_atk = int(dst_atk_t);
            dst_def = int(dst_def_t);
            dst_troops = int(dst_troops_t);
        }
        else
        {
            dst_atk = 30;
            dst_def = 30;
            //dst_troops = pk::min(pk::get_troops(building), 7000);
            dst_troops = pk::min(pk::get_troops(building), 5000); //지휘병력 상한치 하향
            if (dst_troops != 0)
                dst_troops = pk::max(dst_troops, 500);
        }
    }

    // 兵粮袭击效果 （func_5aecc0） + 新增截粮特技效果
    int get_food_captured(pk::unit@ attacker, pk::unit@ target)
    {
        int a = 0;
        if (pk::is_alive(attacker) and pk::is_alive(target))
        {
            if (attacker.weapon == 병기_창 and attacker.has_tech(기교_병량습격))
                a += int(ch::randfloat(1, 2) * attacker.attr.stat[部队能力_攻击]);
            if (ch::has_skill(attacker, 特技_截粮))
                a += int(截粮系数 * target.food / 100.f);
            if (a != 0)
            {
                int b = pk::max(attacker.troops / 2, 1);
                int c = target.food;
                int d = pk::get_max_food(attacker) - attacker.food;
                return pk::min(a, b, c, d);
            }
            return 0;
        }
        return 0;
    }

    // 截取金钱效果 （func_5aecc0） + 新增劫财特技效果
    int get_gold_captured(pk::unit@ attacker, pk::unit@ target)
    {
        int a = 0;
        if (pk::is_alive(attacker) and pk::is_alive(target))
        {
            if (ch::has_skill(attacker, 特技_劫财))
            {
                a += int(劫财系数 * target.gold / 100.f);
                int b = pk::max(attacker.troops / 2, 1);
                int c = target.gold;
                int d = pk::get_max_gold(attacker) - attacker.gold;
                return pk::min(a, b, c, d);
            }
            return 0;
        }
        return 0;
    }
    // 获取指揮兵力数   （func_5aee10）
    int get_command(pk::unit@ unit)
    {
        pk::person@ leader = pk::get_person(unit.leader);
        if (pk::is_alive(leader))
            return pk::min(pk::get_command(leader), unit.troops);
        return unit.troops;
    }

    // 核心：兵力伤害量计算函数 （func_5aee60）
    float get_troops_damage(int src_atk, int src_troops, int tactics_atk, int buffed, int dst_def, int dst_troops, int atk_ability = 0)
    {
        if (!新版伤害公式)
        {
            float n = 0;
            int a = 0;
            int b = 0;
            int c = 0;

            a = pk::max(src_atk, 40);
            a = a * a;

            b = pk::max(dst_def, 40);
            b = b * b;

            n = sqrt(tactics_atk * 64);
            n = n + pk::max((src_troops - dst_troops) / 2000, 0);
            n = n + pk::max((a - b) / 300, 0);
            n = n + 50;
            n = n * (buffed + 10);

            a = (src_troops / 100 + 300) * (src_atk + 50) * (src_atk + 50) / 100;
            b = (dst_troops / 100 + 300) * (dst_def + 50) * (dst_def + 50) / 100;
            c = a + b;
            if (a >= b)
                c = a * 100 / c;
            else
                c = pk::max(100 - (b * 100 / c), 1);
            n = n * c;

            a = pk::max(src_troops / 4, 1);
            n = n * pk::min(sqrt(a), 40.f);

            n = n / 2100;
            n = n + src_troops / 200;

            //ch::u8debug(pk::format("{}  {}  {}  {}  {}  {}  damage:{}", src_atk, src_troops, tactics_atk, buffed, dst_def, dst_troops, n));
            return n;
        }
        else    // 新版兵力伤害量计算公式
        {
            return get_troops_damage_new(src_atk, src_troops, tactics_atk, buffed, dst_def, dst_troops, atk_ability);
        }

    }

    // 新：兵力伤害量计算函数
    float get_troops_damage_new(int src_atk, int src_troops, int tactics_atk, int buffed, int dst_def, int dst_troops, int atk_ability)
    {
        float n = 0, a, b, c, d, k, t, atk, def;
        atk = 10.f + sqrt(float(src_atk));
        def = 10.f + sqrt(float(dst_def));
        t = atk / def;
        //pk::trace(pk::format("伤害计算，troop{},atk{}，def{}，t{}", src_troops,atk,def,t));
        a = sqrt(tactics_atk) * sqrt(pk::max(1000, src_troops)) * 2.f;
        b = pk::min(src_troops, 5000) / 30.f;
        c = sqrt(src_troops) * 6.f;
        d = atk_ability / def * 10.f;

        n += a + (b + c) * t;

        n *= (10 + buffed) / 10.f;

        n *= t;
        n += d;
        
        //ch::u8debug(pk::format("{}  {}  {}  {}  {}  {}  damage:{}", src_atk, src_troops, tactics_atk, buffed, dst_def, atk_ability, n));
        return n;
    }

    // 核心：耐久伤害量计算函数 （合?  func_5aeff0 非兵器 & func_5af050 兵器）
    float get_hp_damage(int weapon, int src_atk, int src_troops, int tactics_atk, int buffed)
    {
        if (weapon == 병기_충차 or weapon == 병기_목수)
        {
            src_troops = pk::max(src_troops, 1);
            float a = sqrt(src_atk * src_atk / 15.f);
            float b = sqrt(src_troops);
            float c = tactics_atk * 4 + 100;
            float d = buffed + 10;
            return a * pk::min(b, 40.f) * c * d / 10000 + (src_troops / 25) + b;
        }
        else
        {
            src_troops = pk::max(src_troops, 1);
            float a = sqrt(src_atk * src_atk / 15.f);
            float b = sqrt(src_troops);
            float c = tactics_atk * 4 + 100;
            float d = buffed + 10;
            return a * b * c * d / 10000;
        }


    }

    // 核心：建筑攻击伤害计算函数   （func_5af370）
    int get_troops_damage(pk::building@ attacker, pk::unit@ target)
    {
        int atk = 0;
        int bonus = 0;

        switch (attacker.facility)
        {
        case 시설_도시:     // 개방형 거점 : 약한 수준으로 적용 ('18.10.14)
            atk = 基础攻击力_城市;
            bonus = 基础攻击力_城市;
            break;
        case 시설_관문:     // 폐쇄형 거점 : 강한 수준으로 적용 ('18.10.14)
            atk = 基础攻击力_关;
            bonus = 基础攻击力_关;
            break;
        case 시설_항구:     // 개방형 거점 : 약한 수준으로 적용 ('18.10.14)
            atk = 基础攻击力_港口;
            bonus = 基础攻击力_港口;
            break;
        case 시설_진:
            atk = 基础攻击力_阵;
            bonus = 基础攻击力_阵;
            break;
        case 시설_요새:
            atk = 基础攻击力_砦;
            bonus = int(基础攻击力_砦 * 0.9f);
            break;
        case 시설_성채:
            atk = 基础攻击力_城塞;
            bonus = int(基础攻击力_城塞 * 0.8f);
            break;
        case 시설_궁노:
            atk = 基础攻击力_箭楼;
            bonus = int(基础攻击力_箭楼 * 1.0f);
            break;
        case 시설_연노로:
            atk = 基础攻击力_连弩楼;
            bonus = int(基础攻击力_连弩楼 * 0.8f);
            break;
        case 시설_투석대:
            atk = 基础攻击力_投石台;
            bonus = int(基础攻击力_投石台 * 0.6f);
            break;
        default:
            // 시설.자동공격 대미지
            atk = 基础攻击力_自动攻击;
            bonus = 基础攻击力_自动攻击;
            break;
        }

        float max_hp = pk::max(pk::get_max_hp(attacker), 1);
        float hp = pk::max(attacker.hp, max_hp / 2);

        return int(hp / max_hp * (152 - int(target.attr.stat[部队能力_防御])) * atk / 132 + pk::rand(bonus));
    }

    // 兵种索引
    int weapon_to_index(int weapon)
    {
        if (weapon >= 兵器_枪 && weapon <= 兵器_冲车) return weapon - 1;
        else if (weapon == 兵器_木兽) return 4;
        else if (weapon == 兵器_井阑 || weapon == 兵器_投石) return weapon - 1;
        else if (weapon >= 兵器_走舸 && weapon <= 兵器_斗舰) return 7;
        else return -1;
    }

    // 修正：兵种克制   【布阵】
    float weapon_restriction(float troops_damage, pk::unit@ attacker, pk::unit@ target_unit)
    {
        int attacker_weapon = attacker.weapon;
        int target_weapon = target_unit.weapon;
        int atk_index = weapon_to_index(attacker_weapon);
        int tar_index = weapon_to_index(target_weapon);
        //ch::u8debug(pk::format("        attacker_weapon:{}, target_weapon:{}", attacker_weapon, target_weapon));
        if (atk_index == -1 || tar_index == -1)
            return troops_damage;
        //ch::u8debug(pk::format("        atk_index:{}, tar_index:{}", atk_index, tar_index));
        float res = 兵种克制关系[atk_index][tar_index];
        if (res < 1.f && ch::has_skill(attacker, 特技_布阵) || res > 1.f && ch::has_skill(target_unit, 特技_布阵))
            res = 1.f;
        //ch::u8debug(pk::format("        res:{}", res));
        return troops_damage * res;
    }

    // 修正：气力
    float energy_impact(float troops_damage, int attacker_energy, int target_energy)
    {
        if (气力影响设定)
        {
            troops_damage = troops_damage * (100 + (attacker_energy - 100) * 士气影响系数) / 100.f;
            troops_damage = troops_damage * (100 - (target_energy - 100) * 士气影响系数) / 100.f;
        }
        return troops_damage;
    }

    // 修正：防御设施
    float defence_building_impact(float troops_damage, int debuffer)
    {
        if (debuffer == 시설_진)
            return troops_damage * 周边部队防御倍率_阵 / 100.f;
        else if (debuffer == 시설_요새)
            return troops_damage * 周边部队防御倍率_砦 / 100.f;
        else if (debuffer == 시설_성채)
            return troops_damage * 周边部队防御倍率_城塞 / 100.f;
        else
            return troops_damage;
    }

    // 修正：地形、地势高低
    float terrain_height_impact(float troops_damage, pk::unit@ attacker, pk::unit@ target_unit)
    {
        if (地形特征修正)
        {
            troops_damage *= (1.f + get_terrain_buff(attacker)) * (1.f - get_terrain_buff(target_unit));
            troops_damage *= (1.f + pk::max(0.01f * get_height_diff(attacker.get_pos(), target_unit.get_pos()),-0.99f));//防止伤害为正
        }
        return troops_damage;
    }

    // 修正：交战朝向
    float direction_impact(float troops_damage, pk::unit@ attacker, pk::unit@ target_unit)
    {
        if (交战朝向修正)
        {
            int damage_dir = get_unit_direction(attacker, target_unit);
            return troops_damage * get_direction_buff(damage_dir);
        }
        return troops_damage;
    }

    // 修正：友军身分特技光环
    float ally_impact(float troops_damage, pk::unit@ attacker, pk::unit@ target_unit)
    {
        if (周边部队加成)
        {
            troops_damage *= (1.f + get_ally_buff(attacker, true)) * (1.f - get_ally_buff(target_unit, false));
        }
        return troops_damage;
    }
    float ally_impact(float troops_damage, pk::unit@ attacker)
    {
        if (周边部队加成)
        {
            troops_damage *= (1.f + get_ally_buff(attacker, true));
        }
        return troops_damage;
    }

    // 修正：部队攻/防特技          【藤甲】【兵圣】【要击】【拱戍】【水师】
    float skill_impact(float troops_damage, pk::unit@ attacker, pk::unit@ target_unit)
    {
        // 藤甲
        if (ch::has_skill(target_unit, 특기_등갑))
        {
            troops_damage *= 藤甲受伤倍率 / 100.f;
        }

        // 兵圣
        if (ch::has_skill(target_unit, 特技_兵圣))
        {
            troops_damage *= 兵圣受伤倍率 / 100.f;
        }

        // 要击
        if (ch::has_skill(attacker, 特技_要击) && target_unit.status != 部队状态_通常)
        {
            troops_damage *= 要击伤害倍率 / 100.f;
        }

        // 拱戍
        if (ch::has_skill(attacker, 特技_拱戍))
        {
            pk::building@ area_building = pk::get_building(pk::get_building_id(attacker.pos));
            if (pk::is_alive(area_building) && attacker.get_force_id() == area_building.get_force_id())
            {
                troops_damage *= 拱戍攻防倍率 / 100.f;
            }
        }
        if (ch::has_skill(target_unit, 特技_拱戍))
        {
            pk::building@ area_building = pk::get_building(pk::get_building_id(target_unit.pos));
            if (pk::is_alive(area_building) && target_unit.get_force_id() == area_building.get_force_id())
            {
                troops_damage /= 拱戍攻防倍率 / 100.f;
            }
        }

        // 水师
        if (pk::is_in_water(target_unit))
        {
            if (ch::has_skill(target_unit, 特技_水师))
                troops_damage *= (100 - 水师减少伤害比例) / 100.f;
        }

        return troops_damage;
    }
    // 修正：部队攻/守军防特技      【坚城】
    float skill_impact(float troops_damage, pk::unit@ attacker, pk::building@ target_building)
    {
        // 坚城
        if (ch::has_skill(target_building, 特技_坚城))
        {
            troops_damage *= 坚城受伤倍率 / 100.f;
        }

        return troops_damage;
    }

    // 修正：部队防御特技           【藤甲】【兵圣】【拱戍】【水师】
    float unit_def_skill_impact(float troops_damage, pk::unit@ defender)
    {
        // 藤甲
        if (ch::has_skill(defender, 특기_등갑))
        {
            troops_damage *= 藤甲受伤倍率 / 100.f;
        }

        // 兵圣
        if (ch::has_skill(defender, 特技_兵圣))
        {
            troops_damage *= 兵圣受伤倍率 / 100.f;
        }

        // 拱戍
        if (ch::has_skill(defender, 特技_拱戍))
        {
            pk::building@ area_building = pk::get_building(pk::get_building_id(defender.pos));
            if (pk::is_alive(area_building) && defender.get_force_id() == area_building.get_force_id())
            {
                troops_damage /= 拱戍攻防倍率 / 100.f;
            }
        }

        // 水师
        if (pk::is_in_water(defender))
        {
            if (ch::has_skill(defender, 特技_水师))
                troops_damage *= (100 - 水师减少伤害比例) / 100.f;
        }
        return troops_damage;
    }

    // 核心：（部队vs部队） 兵种克制+气力+防御设施+朝向+地形地势高低+友军身分特技光环+攻防特技+伤害倍率+难度修正
    float get_troops_final_damage(float troops_damage, pk::unit@ attacker, pk::unit@ target_unit, int debuffer = -1 /*, pk::unit@ msg_unit, pk::person@ msg_person, string@ msg_string*/)
    {
        //ch::u8debug(pk::format("start--leader:{}--target:{}, damage:{}",attacker.leader, target_unit.leader, troops_damage));
        // 兵种克制
        troops_damage = weapon_restriction(troops_damage, attacker, target_unit);
        //ch::u8debug(pk::format("    restriction damage:{}", troops_damage));
        // 设施防御加成
        troops_damage = defence_building_impact(troops_damage, debuffer);
        //ch::u8debug(pk::format("    defence_building damage:{}", troops_damage));
        // 交战朝向
        troops_damage = direction_impact(troops_damage, attacker, target_unit);
        // 友军光环
        troops_damage = ally_impact(troops_damage, attacker, target_unit);
        //ch::u8debug(pk::format("    ally_impact damage:{}", troops_damage));
        // 地形地势
        troops_damage = terrain_height_impact(troops_damage, attacker, target_unit);
        //ch::u8debug(pk::format("    terrain damage:{}", troops_damage));
        // 气力影响
        troops_damage = energy_impact(troops_damage, attacker.energy, target_unit.energy);
        //ch::u8debug(pk::format("    energy_impact damage:{}", troops_damage));
        // 特技
        troops_damage = skill_impact(troops_damage, attacker, target_unit);
        //ch::u8debug(pk::format("    skill_impact damage:{}", troops_damage));
        // 难度伤害修正
        if (attacker.is_player())
            troops_damage *= float(pk::core["damage_penalty"][pk::get_scenario().difficulty]);
        // 伤害倍率
        troops_damage *= 部队基础伤害倍率 / 100.f;
        //ch::u8debug(pk::format("    troops_damage damage:{}", troops_damage));
        // 不超過兵力数的80%
        if (部队伤害上限)
            troops_damage = pk::min(troops_damage, attacker.troops * 0.80f);
        //ch::u8debug(pk::format("    final damage:{}", troops_damage));
        return troops_damage;
    }
    // 核心：（部队vs守军） 气力+攻防特技+伤害倍率+难度修正
    float get_troops_final_damage(float troops_damage, pk::unit@ attacker, pk::building@ target_building /*, pk::unit@ msg_unit, pk::person@ msg_person, string@ msg_string*/)
    {
        // 气力影响
        troops_damage = energy_impact(troops_damage, attacker.energy, target_building.energy);
        // 友军光环
        troops_damage = ally_impact(troops_damage, attacker);
        // 特技
        troops_damage = skill_impact(troops_damage, attacker, target_building);
        // 难度伤害修正
        if (attacker.is_player())
            troops_damage *= float(pk::core["damage_penalty"][pk::get_scenario().difficulty]);
        // 伤害倍率
        troops_damage *= 部队基础伤害倍率 / 100.f;
        // 不超過兵力数的80%
        if (部队伤害上限)
            troops_damage = pk::min(troops_damage, attacker.troops * 0.80f);
        return troops_damage;
    }
    // 核心：（建筑vs部队） 气力+防御设施+攻防特技+伤害倍率+难度修正
    float get_troops_final_damage(float troops_damage, pk::building@ attacker, pk::unit@ target_unit, int debuffer = -1 /*, pk::unit@ msg_unit, pk::person@ msg_person, string@ msg_string*/)
    {
        // 设施防御加成
        troops_damage = defence_building_impact(troops_damage, debuffer);
        // 气力影响
        troops_damage = energy_impact(troops_damage, attacker.energy, target_unit.energy);
        // 防御特技
        troops_damage = unit_def_skill_impact(troops_damage, target_unit);
        // 难度伤害修正
        if (attacker.is_player())
            troops_damage *= float(pk::core["damage_penalty"][pk::get_scenario().difficulty]);
        // 技巧防御强化修正
        if (attacker.has_tech(기교_방어강화) and attacker.facility >= 시설_도시 and attacker.facility <= 시설_성채)
            troops_damage *= 防御强化建筑伤害倍率 / 100.f;
        // 伤害倍率
        troops_damage *= 建筑基础伤害倍率 / 100.f;
        return troops_damage;
    }

    // 核心：（耐久杀伤）
    float get_hp_final_damage(float hp_damage, pk::unit@ attacker, pk::building@ target_building /*, pk::unit@ msg_unit, pk::person@ msg_person, string@ msg_string*/)
    {
        if (attacker.is_player())
            hp_damage *= float(pk::core["damage_penalty"][pk::get_scenario().difficulty]);
        // 气力影响
        hp_damage = energy_impact(hp_damage, attacker.energy, target_building.energy);
        // 友军光环
        hp_damage = ally_impact(hp_damage, attacker);
        // 伤害倍率
        hp_damage *= 部队基础伤害倍率 / 100.f;
        // 特技影响
        hp_damage = skill_impact(hp_damage, attacker, target_building);
        return hp_damage;
    }


    // 气力增加 (type==0主动，1、2反击)
    int get_energy_heal(float troops_damage, pk::unit@ attacker, pk::unit@ target_unit, int type)
    {
        int energy_heal = 0;
        if (type == 0)  // 主动攻击
        {
            if (ch::has_skill(attacker, 특기_앙양))      //昂扬 특기 앙양 효과 반영, 반격에는 적용 안됨 ('18.10.21)
                energy_heal = 昂扬攻击加气;
        }
        if (气力增减设定)
        {
            if (pk::is_enemy(attacker, target_unit))  // 防止?讧时气力增加
            {
                energy_heal += int(pk::min(5.f, (troops_damage / 200.f)));   //광역 전법데미지에 의한 기력 과도상승 리미터 적용 
            }
        }
        return energy_heal;
    }

    // 气力减少 (type==0主动，1、2反击)
    int get_energy_damage(float troops_damage, pk::unit@ attacker, pk::unit@ target_unit, int type)
    {
        int energy_damage = 0;
        if (type == 0)  // 主动攻击
        {
            if (ch::has_skill(attacker, 특기_소탕))      //扫蕩
                energy_damage = pk::max(energy_damage, int(pk::core::skill_constant(attacker, 특기_소탕))); // 5

            if (ch::has_skill(attacker, 特技_威风))      //威风 소탕+위풍 중첩 효과로 조정 ('18.10.9)
                energy_damage = pk::max(energy_damage, int(pk::core::skill_constant(attacker, 特技_威风))); // 20
        }
        if (气力增减设定)
        {
            if (ch::has_skill(target_unit, 특기_호위))  // 护衛 특기_호위 보유 부대는 병력피해 있어도 기력 감소 없음 ('18.10.14)
                energy_damage = 0;
            if (pk::is_enemy(attacker, target_unit))  // 防止?讧时气力增减
            {
                if (troops_damage >= 500)
                {
                    energy_damage += int((troops_damage - 500) / 100.f);
                }
            }
            else    // ?讧时 동토(아군끼리 교전) 시 기력회복 없고, 데미지량에 비례하여 기력 데미지 발생 ('18.10.25)
            {
                energy_damage += int(troops_damage / 100.f);
            }
        }
        return energy_damage;
    }




}
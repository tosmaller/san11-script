// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2021/12/02 #江东新风#变量汉化##
// ## 2021/10/24 #江东新风#改写171trigger##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/12/05 # 江东新风 # 修复颜色乱码 ##
// ## 2020/11/20 # messi # 优化了部队选择目标语句 ##	
// ## 2020/10/30 # 江东新风 # 同步马术书大神的更新 ##				   
/*
// 만든이: 기마책사
// Update: '19.2.1     / 최초내용: 전투병과부대 인접한 공격목표부대 재지정하는 스크립트 작성
// Update: '19.2.19    / 변경내용: 가중치옵션(방어력,신분), 건설편제 제외옵션, 계략부대 제외옵션, 적거점 점령허용 옵션 반영
// Update: '19.2.27    / 변경내용: NPC부대 제외조건 추가
// Update: '19.3.1     / 변경내용: 부대목표변경_병력가중치적용 옵션 추가
// Update: '19.3.4     / 변경내용: null point error 오류 수정
// Update: '19.3.9     / 변경내용: 궤멸직전 부대 소속거점으로 복귀 기능 추가
// Update: '19.3.25    / 변경내용: 주변 거점 교전 시 거점으로 지원 이동
// Update: '20.8.26    / 변경내용: 캠페인에서는 스크립트 효과 발동되지 않도록 수정, 难易度_超级에서만 동작하도록 수정
// Update: '20.9.8     / 변경내용: 계략특기 우대부대 집중공격을 위한 가중치 조건 추가, 간접공성병기 교전조건 추가, 임무 업데이트 범위제한(검색리소스 개선)
// Update: '20.9.16    / 변경내용: 적거점점령/아군거점수성지원 알고리즘 조건 추가
// Update: '20.9.19    / 변경내용: 아군거점수성지원 알고리즘 수정, 进入部队_兵力超过返回 기능 추가, 거점원군 파병부대의 거점진입 방지 추가
// Update: '20.9.21    / 변경내용: 进入部队_兵力超过返回 병기초과 확인조건 추가, 목표거점 영토에 있을 경우에만 초과여부 확인 (거점원군 출진고려)
*/

namespace 野战部队交战优化
{
    //=======================================================================================
    // 유저 설정 (true = on, false = off)

    // 부대/거점 공격목표 변경 시기 설정 (3: 턴시작/중간 모두, 2: 턴중간 변동조건마다, 1: 턴시작 시에만,  0: 기능OFF)
    //部队/据点攻击目标变更时间设定(3:旬开始/中间，2:每个中间变动条件，1:开始时，0:功能OFF)  
    const int  更新_周期设定 = 3;       // 警告:如果将更新周期设定为2或3，游戏可能会变慢。  주의 : 업데이트 주기를 2 또는 3으로 설정 시 유닛이 많아지면 게임이 느려질 수 있음

    const bool 目标部队变更 = true;    // 设定攻击目标部队时，是否设定目标部队(不影响据点目标的变更)   공격목표부대 재설정 시 타켓부대 설정여부 (거점목표 변경에는 영향 안줌)
    const bool 配置对话框 = true;    // 切换攻击目标是否显示对话 공격목표 변경내용 대화표시여부

    const int  部队目标变更_势力条件 = 0;     // 0:电脑AI和player _委任军团，1:player _委任军团，2:电脑AI, 3:均未使用  0: 컴퓨터AI 와 플레이어_위임군단 모두,  1: 플레이어_위임군단만, 2: 컴퓨터AI만,  3: 모두 미적용
    const bool 部队目标变更_适用玩家军团 = false;   // 与势力条件无关，设定是否适用，忽略玩家的任务指定，建议false   세력조건과 별개로 적용여부 설정, 유저의 임무지정 무시되므로 false 권장

    // 加权值设定附加选项(基本是根据兵种/适性来判断)   가중치 추가옵션 설정 (기본적으로는 병과/적성만으로 판단함)
    const bool 部队目标变更_攻击权重值适用 = false;   // 目标对象部队攻击力加权值启用 목표대상의 부대공격력을 가중치 추가 적용
    const bool 部队目标变更_防御权重值适用 = true;   // 目标对象部队防御力加权值启用 목표대상의 부대방어력을 가중치 추가 적용
    const bool 部队目标变更_身份权重值适用 = true;   // 目标对象部队身份加权值启用 如果目标是君主/督导部队，则添加加权值(被动效果。使用cpp内部队溃灭效果时推荐设置)。   목표대상이 군주/도독 부대인 경우 가중치 추가 적용 (패시브효과.cpp 내 부대궤멸효과 사용 시 추천 설정)
    const bool 部队目标变更_兵力权重值适用 = true;   // 增加对目标对象和任务部队兵力差异的加权值(提高攻击容易溃散的小部队的可能性)  목표대상과 임무부대의 병력차이에 대한 가중치 추가 적용 (궤멸시키기 쉬운 소부대를 공격할 가능성 높임)
    const bool 部队目标变更_计略权重值适用 = false;   // 目标对象是拥有计谋特长的部队时追加加权值  목표대상이 계략특기 보유부대인 경우 가중치 추가 적용

    // 交战可能时建设编制判断标准 교전가능 시 건설편제 판단 기준
    const bool 建设编制_除外设定 = true;
    const int  建设编制_标准持有金 = 1000;     // 如果基准金上涨，就不能命令电脑建设 기준값이 높아지면 컴퓨터 건설명령불가
    const int  建设编制_标准兵力 = 3000;     // 基准持有金以上及基准兵力以下的，按建设编制处理  기준 소지금 이상 및 기준병력 이하인 경우 건설편제로 취급
    const bool 建设编制_任务优待 = true;     // 设定部队任务的，允许设置陷阱设施;  부대임무가 설치로 설정된 경우 함정 외 시설 허용

    // 可能交战时计谋部队判断标准 교전가능 시 계략부대 판단 기준
    const bool  计谋部队_除外设定 = true;
    const bool  计谋部队_特技优待 = true;     // 拥有计谋系列特长的部队优待(神算，虚实，百出，鬼谋，连环无条件优待)   계략계열 특기 보유한 부대 우대 (신산,허실,백출,귀모,연환은 무조건 우대)
    const float 计谋部队_智力攻防比 = 1.5f;      // 如果部队智力超过部队攻防平均对比标准，就被视为计谋部队   부대지력이 부대공방평균 대비 기준 이상이면 계략부대로 취급

    // 可能交战时木兽/井阑/投石部队部队战斗设定 교전가능 시 목수/정란/투석 부대 부대전투 설정 ('20.8.30)
    const bool  攻城兵器_部队战斗_参与设定 = true; // 设定攻城兵器部队是否参与部队间战斗 공성병기부대가 부대간 전투에 참여할지 설정
    const int   攻城兵器_部队战斗_标准气力 = 60;   // 攻城兵器部队参与部队间战斗标准(气力以上时参与)   공성병기부대가 부대간 전투에 참여기준 (기력 이상인 경우 참여)

    //---------------------------------------------------------------------------------------

    // 若交战中周边据点兵力不足予以支援 교전 중인 주변 거점 병력 부족 시 지원
    const bool 我军据点_守城支援 = true;   // 近距离战斗部队周边交战据点兵力进入守城支援   근접 전투 부대 주변의 교전 중인 거점에 병력 적을 시 거점 진입하여 수성 지원
    const bool 城市据点_守城支援 = true;   // 我军据点_守城支援 true:激活是否支援城市据点  설정 true 인 경우: 도시거점 수성지원 여부 활성화 
    const bool 港关据点_守城支援 = true;   // 我军据点_守城支援 true:激活是否支援港关据点  설정 true 인 경우: 관문항구 수성지원 여부 활성화  

    // 允许敌方渗透部队攻占据点 적지 침투 부대의 거점공략 허용
    const bool 敌据点_占领许可 = true;   // 部队处于敌区时，周边无可交战部队时，以攻占据点为任务  부대가 적지에 있을 시 주변에 교전가능한 부대가 없을 경우 거점 공략으로 임무설정됨
    const bool 敌据点_占领性格启用 = true;   // 容许占领时根据部队主将性格的行动(true=大胆性格实行占领;false=不以性格进行占领)  점령 허용 시 부대 주장의 성격에 따른 행동 (true=대담,저돌만 점령실행  / false=성격 관계없이 점령실행)

    // 进入据点超过兵力时回到所属据点  거점 진입 시 병력 초과인 경우 소속거점으로 복귀 
    const bool  进入部队_兵力超过返回 = true;   // 进入据点前确认兵力是否超过限度后再设定返回目标   거점으로 부대진입 전 병력한도 초과여부 확인 후 귀환목표 재설정
    const float 进入部队_超过限度标准 = 1.0f;   // 据点最大兵力对比兵力/兵器(据点+部队)设定超过限度标准(0.0f~1.0f)   거점최대병력 대비 병력/병기(거점+부대) 한도초과 기준설정 (0.0f~1.0f)
    const bool  进入部队_兵器超过返回 = true;   // 进入据点前确认是否超过武器限制后重新设定返回目标  거점으로 부대진입 전 병기한도 초과여부 확인 후 귀환목표 재설정
    const float 进入部队_兵器超过_据点兵力标准 = 0.5f;   // 相对据点最大兵力而言，目前兵力不足标准则忽略兵器超支(0.0 ~1.0f)。  거점최대병력 대비 현재병력이 기준 이하일 경우 병기 초과 무시 (0.0f~1.0f)

    //---------------------------------------------------------------------------------------
    const bool 极小兵力部队_返回据点许可 = true;    // 溃灭前返回AI部队所属据点功能(玩家部队未使用)   궤멸직전 AI부대 소속거점으로 복귀 기능 (유저부대 미적용)
    const int  极小兵力部队_返回兵力标准 = 500;     // 未达到定点基准的兵力返回   소속거점 복귀 기준 병력, 기준 미만 시 거점 복귀함


    //=======================================================================================



    class Main
    {

        Main()
        {

            pk::bind(102, pk::trigger102_t(onGameInit));
            pk::bind(111, pk::trigger111_t(onTurnStart));

            pk::bind(160, pk::trigger160_t(onBuildingCreate));
            pk::bind(161, pk::trigger161_t(onBuildingDestroyed));

            pk::bind(170, pk::trigger170_t(onUnitCreate));
            pk::bind(171, pk::trigger171_t(onUnitEliminated));
            pk::bind(172, pk::trigger172_t(onUnitPosChange));
            pk::bind(173, pk::trigger173_t(onUnitExpAdd));

        }
        //---------------------------------------------------------------------------

        pk::unit@ src_unit;
        pk::unit@ dst_unit_old;
        pk::unit@ dst_unit_new;

        pk::list<pk::unit@> list_candidate_unit;
        array<int> arr_target_unit_id(部队_末, -1);
        array<int> arr_target_base_id(部队_末, -1);

        array<float> heishu_weight(6, 0.0f);
        float heishu_null = 0.0f;
        float heishu_weak = 0.5f;
        float heishu_normal = 1.0f;
        float heishu_strong = 1.5f;
        int order_update_range = 10;  // 创建/移除建筑物或创建/移除/移动时任务更新的部队搜索范围('20.9.7)  건물 생성/제거 또는 유닛 생성/제거/이동 시 임무업데이트 대상부대 검색범위 ('20.9.7)

        //---------------------------------------------------------------------------
        // 触发动作 트리거 동작부 
        //---------------------------------------------------------------------------

        // 游戏初始化时更新 게임 초기화 시 업데이트
        void onGameInit()
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().no >= 16) return;
            if (pk::get_scenario().difficulty < 难易度_超级) return;
            clear_target_info();
            pk::force@ force = pk::get_force(pk::get_current_turn_force_id());
            onTurnStart(force);
        }

        // 回合开始时更新 턴 시작 시 업데이트
        void onTurnStart(pk::force@ force)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < 难易度_超级) return;

            if (!pk::is_normal_force(force)) return;
            optimize_current_force_target();    // 通用函数调用 공통함수호출
        }

        // 建筑物生成时更新 건물 생성 시 업데이트
        void onBuildingCreate(pk::building@ building, int type)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < 难易度_超级) return;

            optimize_pos_range_target(building.pos);    // 通用函数调用 공통함수호출 ('20.9.7)
        }

        // 破坏建筑时更新 건물 파괴 시 업데이트
        void onBuildingDestroyed(pk::building@ building, int type)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < 难易度_超级) return;

            optimize_pos_range_target(building.pos);    // 通用函数调用 공통함수호출 ('20.9.7)
        }

        // 创建部队时更新 부대 생성 시 업데이트
        void onUnitCreate(pk::unit@ unit, int type)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < 难易度_超级) return;

            // 重置生成部队的攻击目标 생성된 부대의 공격목표값 초기화
            arr_target_unit_id[unit.get_id()] = -1;
            arr_target_base_id[unit.get_id()] = -1;
        }

        // 部队崩溃时更新 부대 궤멸 시 업데이트
        void onUnitEliminated(pk::unit@ unit, pk::hex_object@ dst, int type)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < 难易度_超级) return;

            // 重置溃败部队的攻击目标 궤멸된 부대의 공격목표값 초기화
            arr_target_unit_id[unit.get_id()] = -1;
            arr_target_base_id[unit.get_id()] = -1;

            optimize_pos_range_target(unit.pos);    // 通用函数调用 공통함수호출 ('20.9.7)
        }

        // 获得部队经验值时 부대 경험치 획득 시
        void onUnitExpAdd(pk::unit@ unit, int type)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < 难易度_超级) return;
            if (!pk::is_alive(unit)) return;

            optimize_pos_range_target(unit.pos);    // 通用函数调用 공통함수호출 ('20.9.7)
        }

        // 部队位置变动(含战法移动) 부대 위치 변동 시 (전법에 의한 이동 포함)
        void onUnitPosChange(pk::unit@ unit_t, const pk::point& in pos)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < 难易度_超级) return;

            if (!check_valid_unit(unit_t)) return;

            int force_id = pk::get_current_turn_force_id();
            if (!pk::is_normal_force(force_id)) return;
            pk::force@ force = pk::get_force(force_id);

            optimize_pos_range_target(pos);    // 通用函数调用 공통함수호출 ('20.9.7)

        }

        //---------------------------------------------------------------------------
        // 기준 좌표로부터 일정 거리 내에 있는 현재 턴 세력 부대 최적화 (공통함수 추가, '20.9.7)
        void optimize_pos_range_target(pk::point pos_t)

        {
            if (更新_周期设定 != 2 and 更新_周期设定 != 3) return;

            if (!pk::is_valid_pos(pos_t)) return;

            int force_id = pk::get_current_turn_force_id();
            if (!pk::is_normal_force(force_id)) return;
            pk::force@ force = pk::get_force(force_id);

            // 해당 좌표로부터 범위 내 현재 턴 부대검색
            pk::array<pk::point> arr_range = pk::range(pos_t, 1, order_update_range);
            for (int i = 0; i < int(arr_range.length); i++)
            {
                pk::unit@ unit = pk::get_unit(arr_range[i]);

                if (check_valid_unit(unit) and check_force_option(unit))
                {
                    if (!unit.action_done and unit.get_force_id() == force_id)
                        optimize_combat_unit_target(unit);     // 부대목표 재설정 함수    
                }
            }
        }

        // 현재 턴 세력 부대 최적화 (공통함수)
        void optimize_current_force_target()
        {
            if (更新_周期设定 != 1 and 更新_周期设定 != 3) return;

            int force_id = pk::get_current_turn_force_id();
            if (!pk::is_normal_force(force_id)) return;
            pk::force@ force = pk::get_force(force_id);

            // 세력 전체 미행동 부대 
            pk::array<pk::unit@> arr_unit_list = pk::list_to_array(pk::get_unit_list());
            if (int(arr_unit_list.length) == 0) return;
            for (int i = 0; i < int(arr_unit_list.length); i++)
            {
                pk::unit@ unit = arr_unit_list[i];
                if (check_valid_unit(unit) and check_force_option(unit))
                {
                    if (!unit.action_done and unit.get_force_id() == force_id)
                        optimize_combat_unit_target(unit);     // 부대목표 재설정 함수    
                }
            }
        }

        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------

        // 부대목표 재설정 함수
        void optimize_combat_unit_target(pk::unit@ unit)
        {
            // 정상부대가 아니면 종료
            if (!pk::is_alive(unit)) return;
            if (unit.status != 部队状态_通常) return;

            // 부대 목표
            @dst_unit_old = pk::hex_object_to_unit(pk::get_hex_object(unit.target_pos));




            // ------ 공격 가능한 적부대 검색함수 호출 -------------
            // 주의 :: def_force_base_nearby, atk_enemy_base_nearby, retreat_exceed_unit 함수보다 먼저 사용할 것
            get_list_candidate_unit(unit);
            int candidate_count = list_candidate_unit.count;
            if (candidate_count == 0)   // 부대가 원군도착인 경우 거점주변 추가탐색
            {
                search_enemy_on_reinforce_engage(unit);
                candidate_count = list_candidate_unit.count;
            }// --------------------------------------------

            // 개발자 디버깅용 로그 출력
            if (调试模式)
            {
                string src_name = (pk::is_alive(unit)) ? pk::decode(pk::get_name(pk::get_person(unit.leader))) + "대" : "확인불가";
                string dst_name = (pk::is_alive(dst_unit_old)) ? pk::decode(pk::get_name(pk::get_person(dst_unit_old.leader))) + "대" : "확인불가";
                pk::info(pk::format("{} 邻近部队数={}", src_name, candidate_count));
            }

            // 거점점령, 수성지원, 점령군 초과병력귀환


            if (我军据点_守城支援 and def_force_base_nearby(unit)) return;  // 주변의 교전 중인 아군 거점 지원
            if (敌据点_占领许可 and atk_enemy_base_nearby(unit)) return;  // 부대좌표로 요격/원정 여부 판단 : 적지원정이면 거점공략 설정
            if ((进入部队_兵力超过返回 or 进入部队_兵器超过返回) and retreat_exceed_unit(unit)) return;  // 거점 병력/병기 초과 시 타거점으로 진로변경

            // 궤멸직전 부대 거점 복귀 ('19.3.9)
            if (极小兵力部队_返回据点许可 and retreat_battlefield(unit)) return;


            // AI건설편제인 경우 임무 미변경
            if (建设编制_除外设定 and (check_construction_unit(unit))) return;

            // 계략부대인 경우 임무 미변경
            if (计谋部队_除外设定 and (func_strategy_unit_type(unit) > 0)) return;

            // 공격목표부대 변경
            if (目标部队变更 and candidate_count > 0)
                func_change_target_unit(unit);  // 주변 적부대 공격설정

        }


        //---------------------------------------------------------------------------

        // 극소병력 부대 야전 이탈 명령
        bool retreat_battlefield(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return false;
            if (pk::is_player_controlled(unit)) return false;

            if (int(unit.troops) < 极小兵力部队_返回兵力标准)
            {
                int unit_id = unit.get_id();
                int service_id = pk::get_service(unit);
                pk::building@ base = pk::get_building(pk::get_service(unit));
                if (pk::is_alive(base) and arr_target_base_id[unit_id] != service_id and pk::get_distance(base.pos, unit.pos) <= 25)
                {
                    arr_target_base_id[unit_id] = service_id;
                    pk::set_order(unit, 部队任务_移动, base.pos);
                    return true;
                }
            }

            return false;
        }

        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------

        // 공격 가능한 적부대 검색함수
        void get_list_candidate_unit(pk::unit@ unit)
        {
            // 리스트 초기화
            if (!pk::is_alive(unit)) return;
            list_candidate_unit.clear();

            int weapon_id = unit.weapon;
            int weapon_type = get_weapon_type(weapon_id);   // 0:검, 1:전투, 2:충차, 3:공성기타, 4:수군

            // 충차는 검색대상에서 제외
            if (weapon_id == 병기_충차) return;
            // 목수/정란/투석 검색대상에서 제외여부
            if (!攻城兵器_部队战斗_参与设定 and weapon_type == 3) return;
            if (攻城兵器_部队战斗_参与设定 and weapon_type == 3 and (unit.energy < 攻城兵器_部队战斗_标准气力)) return;

            // 공격 사거리 설정 --- 함수 변경 ('20.9.13)
            pk::int_int atk_range = get_atk_range(unit);
            int min = (weapon_id == 병기_충차) ? 0 : atk_range.first;
            int max = (weapon_id == 병기_충차) ? 0 : atk_range.second;

            pk::array<pk::point> range = pk::get_movable_pos(unit);  // 이동가능좌표
            for (int i = 0; i < int(range.length); i++)
            {
                pk::point pos = range[i];
                pk::hex@ hex = pk::get_hex(pos);
                pk::array<pk::point> arr_t = pk::range(pos, min, max);   // 주변좌표
                for (int j = 0; j < int(arr_t.length); j++)
                {
                    pk::unit@ target_unit = pk::get_unit(arr_t[j]);
                    if (!hex.has_building and !hex.has_unit and pk::is_alive(target_unit)
                        and pk::is_enemy(unit, target_unit) and !list_candidate_unit.contains(target_unit))
                    {
                        list_candidate_unit.add(target_unit);   // 적부대 공격 가능 시 리스트 추가
                    }
                }
            }
        }

        // 부대임무가 자세력 거점으로 이동인 경우, 거점주변에 적부대 있는지 추가 확인
        void search_enemy_on_reinforce_engage(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return;
            if (list_candidate_unit.count > 0) return;
            if (unit.type != 부대종류_전투) return;
            if (unit.weapon > 병기_군마) return;

            pk::building@ service_base = pk::get_building(pk::get_service(unit));
            if (!pk::is_alive(service_base)) return;

            pk::building@ base_p = pk::get_building(pk::get_building_id(unit.pos));
            if (!pk::is_alive(base_p)) return;

            int service_id = service_base.get_id();
            int base_p_id = base_p.get_id();
            if (base_p_id == service_id or unit.get_force_id() != base_p.get_force_id()) return;    // 소속거점 영토이거나 타세력 영토인 경우

            if (unit.target_type == 부대임무대상_거점)
            {
                pk::building@ base_t = pk::get_building(unit.target);
                if (pk::is_alive(base_t))
                {
                    int base_t_id = base_t.get_id();
                    // 목표거점과 부대좌표거점이 같거나, 부대 이동거리 내에 목표거점이 존재하는 경우
                    if (base_t_id < 据点_末 and (base_t_id == base_p_id or check_base_in_movable_range(unit, base_t)))
                    {
                        pk::array<pk::point> range = pk::range(base_p.pos, 1, 12);  // 거점 주변 검색
                        for (int i = 0; i < int(range.length); i++)
                        {
                            pk::point pos_i = range[i];
                            pk::unit@ unit_i = pk::get_unit(pos_i);
                            if (pk::is_alive(unit_i) and pk::get_building_id(pos_i) == base_p_id
                                and pk::is_enemy(unit, unit_i) and !list_candidate_unit.contains(unit_i))
                            {
                                list_candidate_unit.add(unit_i);   // 적부대 공격 가능 시 리스트 추가
                            }
                        }
                    }
                }
            }
        }


        // 공격목표부대 변경 함수
        void func_change_target_unit(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return;

            @src_unit = @unit;
            int target_count = list_candidate_unit.count;   // 후보 부대수
            if (target_count == 0) return;

            // 병과종류
            int weapon_id = src_unit.weapon;
            int weapon_type = get_weapon_type(weapon_id);   // 0:검, 1:전투, 2:충차, 3:공성기타, 4:수군

            // 임무변경 대상부대
            bool combat_update = false;
            if (weapon_type == 1 or weapon_type >= 4) combat_update = true;
            else if (weapon_type == 3 and 攻城兵器_部队战斗_参与设定 and (unit.energy >= 攻城兵器_部队战斗_标准气力)) combat_update = true;
            else combat_update = false;

            if (combat_update)
            {
                // 병종상성 설정
                set_heishu_weight(weapon_id);

                // 병종상성, 적성, 방어력 가중치 적용한 목표부대 능력치 정렬
                list_candidate_unit.sort(function(a, b)
                {
                    float stat_a = main.get_attr_weight(a);
                    float stat_b = main.get_attr_weight(b);
                    return (stat_a > stat_b);
                });

                // 목표부대 설정
                pk::array<pk::unit@> arr_candidate_unit = pk::list_to_array(list_candidate_unit);
                @dst_unit_new = arr_candidate_unit[0];

                // 개발자 디버깅용 로그 출력
                if (调试模式)
                {
                    string src_name = (pk::is_alive(unit)) ? pk::decode(pk::get_name(pk::get_person(unit.leader))) + "部" : "无法确认";
                    string dst_name = (pk::is_alive(dst_unit_new)) ? pk::decode(pk::get_name(pk::get_person(dst_unit_new.leader))) + "部" : "无法确认";
                    pk::info(pk::format("{} 任务目标:{}", src_name, dst_name));
                }

                if (pk::is_alive(dst_unit_new))
                {
                    int src_id = src_unit.get_id();
                    int dst_id = dst_unit_new.get_id();

                    // 새 목표부대로 변경된 경우 공격임무 재설정
                    if (arr_target_unit_id[src_id] != dst_id)
                    {
                        // 목표 재설정 정보 저장
                        arr_target_unit_id[src_id] = dst_id;
                        arr_target_base_id[src_id] = -1;

                        // 공격임무 재설정
                        int old_order = src_unit.order;
                        if (old_order == 부대임무_물림 or old_order == 부대임무_섬멸)
                            pk::set_order(src_unit, old_order, dst_unit_new.pos);
                        else
                            pk::set_order(src_unit, 部队任务_攻击, dst_unit_new.pos);

                        // 부대임무변경 대화 표시
                        if (配置对话框 and pk::is_in_screen(src_unit.pos))

                            say_change_target_unit();
                    }
                }
            }
        }



        void say_change_target_unit()
        {
            string str_f0 = pk::decode(pk::get_name(pk::get_force(src_unit.get_force_id())));
            string str_p0 = pk::decode(pk::get_name(pk::get_person(src_unit.leader)));
            string str_w0 = get_weapon_name(src_unit.weapon);
            string str_t0 = get_tekisei_name(pk::get_tekisei(src_unit));
            //string str_f1 = pk::decode(pk::get_name(pk::get_force(dst_unit_old.get_force_id())));
            //string str_p1 = pk::decode(pk::get_name(pk::get_person(dst_unit_old.leader)));
            //string str_w1 = get_weapon_name(dst_unit_old.weapon);
            //string str_t1 = get_tekisei_name(pk::get_tekisei(dst_unit_old));
            string str_f2 = pk::decode(pk::get_name(pk::get_force(dst_unit_new.get_force_id())));
            string str_p2 = pk::decode(pk::get_name(pk::get_person(dst_unit_new.leader)));
            string str_w2 = get_weapon_name(dst_unit_new.weapon);
            string str_t2 = get_tekisei_name(pk::get_tekisei(dst_unit_new));
            string str_color = get_heishu_color(dst_unit_new.weapon);

            //pk::trace("开始显示706语言");
            string str_line0 = pk::format("\x1b[2x{}军\x1b[1x{}队\x1b[17x({}{})\x1b[0x任务变更!!", str_f0, str_p0, str_w0, str_t0);
            string str_line1 = pk::format("开始向\x1b[2x{}军\x1b[1x{}队{}{}{}\x1b[0x发起攻击", str_f2, str_p2, str_color, str_w2, str_t2);

            pk::person@ leader = pk::get_person(src_unit.leader);
            pk::say(pk::encode(pk::format("{}\n{}", str_line0, str_line1)), leader);
        }

        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------

        // ----- 敌据点_占领许可  
        // 공격부대 : 현재 좌표구역의 거점 세력 확인 : true=원정 또는 원군 / false=요격 판단
        bool atk_enemy_base_nearby(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return false;
            if (unit.type != 부대종류_전투) return false;

            int candidate_count = list_candidate_unit.count;
            if (candidate_count > 4) return false;

            pk::building@ service_base = pk::get_building(pk::get_service(unit));
            pk::building@ target_base = pk::get_building(pk::get_building_id(unit.pos));
            if (pk::is_alive(service_base) and pk::is_alive(target_base))
            {
                int unit_id = unit.get_id();
                int service_id = service_base.get_id();
                int target_id = target_base.get_id();
                pk::person@ leader = pk::get_person(unit.leader);

                // 적 거점 공략 (근접 교전 이후 주변에 부대 없을 시에만 동작, 최초 출진 시에는 미동작)
                bool is_enemy_base = pk::is_enemy(unit, target_base);
                bool is_candidate_base = (candidate_count == 0 or (is_weak_base(target_base) and candidate_count > 0));
                if (is_enemy_base and is_candidate_base and (arr_target_base_id[unit_id] != target_id and arr_target_unit_id[unit_id] != -1))
                {

                    if ((敌据点_占领性格启用 and (leader.character == 성격_대담 or leader.character == 성격_저돌)) or !敌据点_占领性格启用)
                    {
                        // 목표 재설정 정보 저장
                        arr_target_base_id[unit_id] = target_id;
                        // 적지이면 적거점 공격 재설정
                        pk::set_order(unit, 部队任务_攻击, target_base.pos);
                        // 목표변경 메시지
                        if (配置对话框 and pk::is_in_screen(unit.pos))
                        {
                            string target_name = pk::decode(pk::get_name(target_base));
                            switch (pk::rand(3))
                            {
                            case 0: pk::say(pk::encode(pk::format("向\x1b[2x{}\x1b[0x发起进攻吧", target_name)), leader); break;
                            case 1: pk::say(pk::encode(pk::format("向\x1b[2x{}\x1b[0x突击！", target_name)), leader); break;
                            case 2: pk::say(pk::encode(pk::format("攻击\x1b[2x{}\x1b[0x吧", target_name)), leader); break;
                            }
                        }

                        return true;
                    }
                }
            }

            return false;
        }


        //---------------------------------------------------------------------------
        // ----- 我军据点_守城支援 
        // 방어부대 : 주변의 병력부족한 교전거점으로 이동
        bool def_force_base_nearby(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return false;
            if (unit.type != 부대종류_전투) return false;    // 전투 병과 아니면 제외

            pk::building@ service = pk::get_building(pk::get_service(unit));
            if (!pk::is_alive(service)) return false;

            int candidate_count = list_candidate_unit.count;
            if (candidate_count > 1) return false;

            // 주변의 목표거점 검색
            pk::list<pk::building@> list_target_base;
            pk::array<pk::point> arr = pk::get_movable_pos(unit);
            for (int i = 0; i < int(arr.length); i++)
            {
                pk::building@ base = pk::get_building(arr[i]);
                if (pk::is_alive(base))




                {
                    int base_id = base.get_id();
                    bool is_force_base = (unit.get_force_id() == base.get_force_id());
                    bool is_force_city = (城市据点_守城支援 and is_force_base and (건물_도시시작 <= base_id and base_id < 城市_末));
                    bool is_force_gate = (港关据点_守城支援 and is_force_base and (건물_관문시작 <= base_id and base_id < 건물_항구끝));
                    if (is_force_base and pk::enemies_around(base))
                    {
                        if (is_force_gate and is_weak_base(base)) list_target_base.add(base);
                        if (is_force_city and is_weak_base(base)) list_target_base.add(base);
                    }
                }
            }

            if (list_target_base.count == 0) return false;

            // 목표 거점이 다수인 경우 우선순위 정렬 (도시가 관문항구보다 우선, 병력 부족 거점 우선, 병력 충분 시 내구도 부족 거점 우선
            list_target_base.sort(function(a, b)
            {
                bool city_a = (건물_도시시작 <= a.get_id() and a.get_id() < 城市_末);
                bool city_b = (건물_도시시작 <= b.get_id() and b.get_id() < 城市_末);
                if (city_a and !city_b) return true;
                if (!city_a and city_b) return false;
                int troops_a = pk::get_troops(a);
                int troops_b = pk::get_troops(b);
                if (troops_a > 3000 and troops_b > 3000) return (int(a.hp) < int(b.hp));
                return (troops_a < troops_b);
            });

            pk::building@ target_base = list_target_base[0];
            pk::person@ leader = pk::get_person(unit.leader);

            if (pk::is_alive(target_base) and pk::is_alive(leader))
            {
                string leader_name = pk::decode(pk::get_name(leader));
                string target_name = pk::decode(pk::get_name(target_base));
                int target_id = target_base.get_id();
                int unit_id = unit.get_id();
                int max_atk_range = get_atk_range(unit).second;
                pk::array<int> num_unit_around = count_unit_around(unit, target_base);  //[0]근접, [1]간접

                // 아군 거점으로 이동
                if (arr_target_base_id[unit_id] != target_id)
                {
                    // 근접전투부대 이거나, 간접전투부대면서 주변에 지원가능한 근접전투부대가 없는 경우
                    if (max_atk_range <= 1 or (max_atk_range > 1 and num_unit_around[0] < 1))
                    {
                        // 목표 재설정 정보 저장
                        arr_target_base_id[unit_id] = target_id;
                        // 아군 거점 이동 재설정
                        pk::set_order(unit, 部队任务_攻击, target_base.pos);  // 部队任务_移动 → 部队任务_攻击 으로 수정 ('20.9.13)
                        // 목표변경 메시지
                        if (配置对话框 and pk::is_in_screen(unit.pos))
                        {
                            switch (pk::rand(3))
                            {
                            case 0: pk::say(pk::encode(pk::format("去支援\x1b[2x{}\x1b[0x吧", target_name)), leader); break;
                            case 1: pk::say(pk::encode(pk::format("我部将支援\x1b[2x{}\x1b[0x", target_name)), leader); break;
                            case 2: pk::say(pk::encode(pk::format("\x1b[2x{}\x1b[0x需要支援", target_name)), leader); break;
                            }
                        }

                        if (调试模式)
                            pk::info(pk::format("{} 任务目标:{}", leader_name, target_name));

                        return true;
                    }
                }
            }
            return false;
        }

        // 부대주변의 자세력 근접/간접 공격 부대수 확인
        pk::array<int> count_unit_around(pk::unit@ unit, pk::building@ target_base)
        {
            pk::array<int> count_unit(2, 0);    // [0]근접, [1]간접
            pk::array<pk::point> arr = pk::get_movable_pos(unit, target_base.pos, (unit.attr.stat[部队能力_移动] / 2));
            for (int i = 0; i < int(arr.length); i++)
            {
                pk::unit@ unit_t = pk::get_unit(arr[i]);
                if (pk::is_alive(unit_t) and unit_t.type == 부대종류_전투)
                    if (unit.get_id() != unit_t.get_id() and unit.get_force_id() == unit_t.get_force_id())
                    {
                        if (get_atk_range(unit_t).second > 1)
                            count_unit[1]++;
                        else count_unit[0]++;
                    }
            }
            return count_unit;
        }

        // 취약 거점 판단 조건
        bool is_weak_base(pk::building@ base)
        {
            if (!pk::is_alive(base)) return false;
            int base_id = base.get_id();
            if (base_id < 0 or base_id >= 据点_末) return false;
            bool is_city = (건물_도시시작 <= base_id and base_id < 城市_末);
            int base_troops = pk::get_troops(base);
            int base_hp = int(base.hp);
            int base_max_hp = int(pk::get_max_hp(base));

            if (base_troops < 500 or base_hp < 700) return true;
            else if (!is_city and (base_troops <= 1000 and base_hp <= pk::max(1500, int(base_max_hp * 0.75f)))) return true;
            else if (!is_city and (base_troops <= 2000 and base_hp <= pk::max(1000, int(base_max_hp * 0.50f)))) return true;
            else if (is_city and (base_troops <= 3000 and base_hp <= pk::max(2000, int(base_max_hp * 0.50f)))) return true;
            else if (is_city and (base_troops <= 5000 and base_hp <= pk::max(1500, int(base_max_hp * 0.25f)))) return true;

            return false;
        }

        //---------------------------------------------------------------------------
        // ----- 교전 후 거점 부대 진입 시 초과병력/병기 부대 귀환 설정
        // 교전부대 : 현재 좌표구역의 거점 세력 확인 : true=원정 또는 원군 / false=요격 판단
        bool retreat_exceed_unit(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return false;
            int unit_id = unit.get_id();
            int leader_id = unit.leader;
            pk::person@ leader = pk::get_person(leader_id);
            string unit_name = pk::decode(pk::get_name(leader));

            int candidate_count = list_candidate_unit.count;
            if (candidate_count > 0) return false;

            pk::building@ service_base = pk::get_building(pk::get_service(unit));
            pk::building@ pos_area_base = pk::get_building(pk::get_building_id(unit.pos));
            if (!pk::is_alive(service_base)) return false;
            if (!pk::is_alive(pos_area_base)) return false;
            int service_id = service_base.get_id();
            int pos_area_id = pos_area_base.get_id();

            if (!pk::enemies_around(pos_area_base))
            {
                // 거점 진입 시 부대+거점 병력 한도 설정
                bool is_service_base = (service_id == pos_area_id);
                bool is_force_base = (unit.get_force_id() == pos_area_base.get_force_id());
                bool is_exceed_unit = (is_force_base and !is_service_base and check_exceed_unit(pos_area_base, unit, true));

                // 병력 한도 초과하는 경우 소속 거점으로 퇴각
                if (is_exceed_unit and (arr_target_base_id[unit_id] == pos_area_id and arr_target_unit_id[unit_id] != -1))
                {
                    // 목표 재설정 정보 저장
                    arr_target_base_id[unit_id] = service_id;
                    // 소속 거점으로 돌아감
                    pk::set_order(unit, 部队任务_攻击, service_base.pos);
                    // 목표변경 메시지
                    if (配置对话框 and pk::is_in_screen(unit.pos))
                    {
                        string old_order = pk::format("不必执行原目标:\x1b[2x{}\x1b[0x\n", pk::decode(pk::get_name(pos_area_base)));
                        string target_name = pk::decode(pk::get_name(service_base));
                        switch (pk::rand(3))
                        {
                        case 0: pk::say(pk::encode(pk::format("{}向\x1b[2x{}\x1b[0x退却吧", old_order, target_name)), leader); break;
                        case 1: pk::say(pk::encode(pk::format("{}向\x1b[2x{}\x1b[0x返回", old_order, target_name)), leader); break;
                        case 2: pk::say(pk::encode(pk::format("{}转向\x1b[2x{}\x1b[0x", old_order, target_name)), leader); break;
                        }
                    }
                    if (调试模式) pk::info(pk::format("部队返回 目标据点: {}", pk::decode(pk::get_name(service_base))));
                }
            }

            // 부대임무 정보 조회
            bool is_order_retreat = (unit.order == 부대임무_퇴각 or (unit.order == 부대임무_물림 and candidate_count == 0));

            // 부대임무 목표거점 병력 한도 초과하는 경우 인근의 병력부족 거점으로 퇴각
            if (is_order_retreat or unit.target_type == 부대임무대상_거점)
            {
                pk::building@ base_t = (is_order_retreat) ? service_base : pk::get_building(unit.target);
                if (pk::is_alive(base_t) and base_t.get_id() < 据点_末)
                {
                    // 거점진입 대상 거점의 부대진입 시 병력 한도 확인
                    string base_t_name = pk::decode(pk::get_name(base_t));
                    int target_id = base_t.get_id();
                    bool is_in_movable_pos = check_base_in_movable_range(unit, base_t); // 부대 이동거리 내에 목표거점이 존재하는 경우
                    bool is_in_city_area = (pk::get_city_id(base_t.pos) == pk::get_city_id(unit.pos));  // 부대위치가 목표거점과 같은 도시영역인 경우
                    bool is_force_base = (unit.get_force_id() == base_t.get_force_id());
                    bool is_exceed_unit = (is_force_base and (is_in_city_area or is_in_movable_pos) and check_exceed_unit(base_t, unit, true));
                    if (调试模式) pk::info(pk::format("---{}대 {}:{}", unit_name, get_order_info(unit.order), base_t_name));

                    // 병력 한도 초과하는 경우 병력 적은 인근 거점으로 퇴각
                    if (is_exceed_unit)
                    {
                        // 거점 검색 함수
                        int dst_id = get_neighbor_base_retreat(base_t, unit);
                        pk::building@ dst_base = pk::get_building(dst_id);
                        if (!pk::is_alive(dst_base)) return false;

                        // 목표 재설정 정보 저장
                        arr_target_base_id[unit_id] = dst_id;
                        // 부대임무 재설정 : 새로운 목표 거점으로 이동
                        pk::set_order(unit, 部队任务_攻击, dst_base.pos);
                        // 목표변경 메시지
                        if (配置对话框 and pk::is_in_screen(unit.pos))
                        {
                            string old_order = pk::format("\x1b[2x{}\x1b[0x负责部队\n", pk::decode(pk::get_name(base_t)));
                            string target_name = pk::decode(pk::get_name(dst_base));
                            switch (pk::rand(3))
                            {
                            case 0: pk::say(pk::encode(pk::format("{}向\x1b[2x{}\x1b[0x进军吧", old_order, target_name)), leader); break;
                            case 1: pk::say(pk::encode(pk::format("{}改为 \x1b[2x{}\x1b[0x", old_order, target_name)), leader); break;
                            case 2: pk::say(pk::encode(pk::format("{}目标变更为\x1b[2x{}\x1b[0x", old_order, target_name)), leader); break;
                            }
                        }
                        if (调试模式) pk::info(pk::format("部队返回目标据点: {}", pk::decode(pk::get_name(dst_base))));
                    }
                }
            }

            return false;
        }

        // 가까운 병력 부족 거점을 얻음
        pk::building@ src_t;
        int get_neighbor_base_retreat(pk::building@ src_base, pk::unit@ unit)
        {
            if (!pk::is_alive(unit) or !pk::is_alive(src_base)) return -1;
            int dst_id = -1;
            int best_dst = -1;
            int best_distance = 0;
            int src_id = src_base.get_id();
            pk::list<pk::building@> dst_list;
            @src_t = @src_base;
            pk::force@ force = pk::get_force(src_base.get_force_id());

            // 철수 거점 검색
            if (pk::get_city_list(force).count <= 1 and src_id < 城市_末)
                return -1;      // 마지막 도시인 경우 철수 불가
            else
            {
                for (int i = 0; i < 据点_末; i++)
                {
                    pk::building@ dst = pk::get_building(i);
                    dst_id = dst.get_id();
                    bool is_exceed_unit = check_exceed_unit(dst, unit, true);
                    if (!is_exceed_unit and src_id != dst_id and src_base.get_force_id() == dst.get_force_id())
                    {
                        best_dst = dst_id;
                        dst_list.add(dst);  // 철수가능 거점리스트 추가
                    }
                }
            }

            // 출진가능 거점리스트 정렬 (거점 거리 오름차순, 좌표거리 오름차순)
            if (dst_list.count == 0)
                best_dst = -1;
            else
            {
                dst_list.sort(function(a, b)
                {
                    int build_dist_a = pk::get_building_distance(a.get_id(), main.src_t.get_id(), a.get_force_id());
                    int build_dist_b = pk::get_building_distance(b.get_id(), main.src_t.get_id(), b.get_force_id());
                    int pos_dist_a = pk::get_distance(a.pos, main.src_t.pos);
                    int pos_dist_b = pk::get_distance(b.pos, main.src_t.pos);
                    if (build_dist_a != build_dist_b) return (build_dist_a < build_dist_b);
                    return (pos_dist_a < pos_dist_b);
                });
                best_dst = dst_list[0].get_id();
            }
            return best_dst;
        }


        // 거점+부대 병력/병기 한도 초과 여부 확인
        bool check_exceed_unit(pk::building@ base, pk::unit@ unit, bool isforce = true)
        {
            if (!pk::is_alive(base) or !pk::is_alive(unit)) return false;
            if (base.get_id() >= 据点_末) return false;
            if (isforce and base.get_force_id() != unit.get_force_id()) return false;
            if (!isforce and !pk::is_enemy(base, unit)) return false;

            float exceed_limit = pk::max(0.0f, pk::min(1.0f, 进入部队_超过限度标准));
            float weapon_ignore = pk::max(0.0f, pk::min(1.0f, 进入部队_兵器超过_据点兵力标准));
            bool is_ignore = (float(pk::get_troops(base)) / float(pk::get_max_troops(base)) <= weapon_ignore);

            bool is_exceed_troops = false;
            bool is_exceed_weapon = false;
            if (unit.type == 부대종류_전투)
            {
                if (进入部队_兵力超过返回)
                {
                    int base_limit = int(float(pk::get_max_troops(base)) * exceed_limit);
                    int exceed_troops = pk::max(0, ((pk::get_troops(base) + unit.troops) - base_limit));
                    is_exceed_troops = (exceed_troops > 0);
                }
                if (进入部队_兵器超过返回 and !is_ignore)
                {
                    int gnd_wpn_id = pk::get_ground_weapon_id(unit);
                    int sea_wpn_id = pk::get_sea_weapon_id(unit);
                    int unit_gnd_wpn_amt = (is_siege_weapon(gnd_wpn_id)) ? 1 : unit.troops;
                    int unit_sea_wpn_amt = (sea_wpn_id == 兵器_走舸) ? 0 : 1;
                    int base_gnd_wpn_amt = pk::get_weapon_amount(base, gnd_wpn_id);
                    int base_sea_wpn_amt = pk::get_weapon_amount(base, sea_wpn_id);
                    int exceed_gnd_wpn_amt = pk::max(0, (base_gnd_wpn_amt + unit_gnd_wpn_amt) - int(float(pk::get_max_weapon_amount(base, gnd_wpn_id)) * exceed_limit));
                    int exceed_sea_wpn_amt = pk::max(0, (base_sea_wpn_amt + unit_sea_wpn_amt) - int(float(pk::get_max_weapon_amount(base, sea_wpn_id)) * exceed_limit));
                    is_exceed_weapon = (exceed_gnd_wpn_amt > 0 or exceed_sea_wpn_amt > 0);
                }
            }
            else if (unit.type == 부대종류_수송)
            {
                if (进入部队_兵力超过返回)
                {
                    int exceed_troops = pk::max(0, ((pk::get_troops(base) + unit.troops) - pk::get_max_troops(base)));
                    is_exceed_troops = (exceed_troops >= 2000);  // 부대병력 2000 이상 증발 시 
                }
                if (进入部队_兵器超过返回 and !is_ignore)
                {
                    pk::array<int> arr_exceed_wpn_amt(병기_끝, 0);
                    for (int i = 0; i < 병기_끝; i++)
                    {
                        if (!is_exceed_weapon and !is_ignore)
                        {
                            int unit_wpn_amt = unit.stock[i].amount;
                            int base_wpn_amt = pk::get_weapon_amount(base, i);
                            arr_exceed_wpn_amt[i] = pk::max(0, (unit_wpn_amt + unit_wpn_amt) - int(float(pk::get_max_weapon_amount(base, i)) * exceed_limit));
                            if (arr_exceed_wpn_amt[i] >= ((is_siege_weapon(i) or is_ship_weapon(i)) ? 1 : 2000))
                                is_exceed_weapon = true;  // 공성/함선병기 1이상 또는 전투병기 2000 이상 증발 시
                        }
                    }
                }
            }

            if (调试模式)
            {
                string unit_name = pk::decode(pk::get_name(pk::get_person(unit.leader)));
                string base_name = pk::decode(pk::get_name(base));
                if (is_exceed_troops or is_exceed_weapon)
                    pk::info(pk::format("---{}/{}进入导致库存超额", base_name, unit_name));
            }
            bool is_exceed_unit = (is_exceed_troops or is_exceed_weapon);
            return is_exceed_unit;
        }

        // 부대의 이동범위 내에 목표거점이 위치하는지 확인
        bool check_base_in_movable_range(pk::unit@ unit, pk::building@ base)
        {
            if (!pk::is_alive(unit) or !pk::is_alive(base)) return false;

            pk::array<pk::point> arr_range = pk::get_movable_pos(unit);;
            for (int i = 0; i < int(arr_range.length); i++)
            {
                if (base.pos == arr_range[i]) return true;
            }
            return false;
        }

        bool is_siege_weapon(int weapon_id) { return (병기_충차 <= weapon_id and weapon_id <= 병기_목수); }
        bool is_ship_weapon(int weapon_id) { return (兵器_楼船 == weapon_id or weapon_id == 兵器_斗舰); }


        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------

        // 정상적인 부대인지 확인
        bool check_valid_unit(pk::unit@ unit_t)
        {
            if (!pk::is_alive(unit_t))      return false;
            if (unit_t.type != 부대종류_전투)	return false;	// 수송 부대 제외

            int force_id = unit_t.get_force_id();
            pk::force@ force = pk::get_force(force_id);
            if (!pk::is_alive(force) or !pk::is_normal_force(force_id)) return false;  // 소속 세력 확인  
            if (!pk::is_alive(pk::get_building(pk::get_service(unit_t))))	return false;	// 소속거점 확인불가 제외

            int leader_id = unit_t.leader;
            if ((leader_id >= 670 and leader_id < 800) or (leader_id >= 1000 and leader_id < 1100)) return false;

            return true;
        }


        // 세력 확인 함수
        bool check_force_option(pk::unit@ unit_t)
        {
            // 플레이어 군단 미적용 (유저의 임무지정 무시하면서 목표재설정하므로 미적용 권장)
            if (!pk::is_alive(unit_t)) return false;
            if (pk::is_player_controlled(unit_t))
            {
                if (部队目标变更_适用玩家军团) return true;
                else return false;
            }

            // 0: 컴퓨터AI 와 플레이어_위임군단 모두,  1: 플레이어_위임군단만, 2: 컴퓨터AI만,  3: 모두 미적용
            if (部队目标变更_势力条件 == 0 and !pk::is_player_controlled(unit_t)) return true;
            else if (部队目标变更_势力条件 == 1 and unit_t.is_player() and !pk::is_player_controlled(unit_t)) return true;
            else if (部队目标变更_势力条件 == 2 and !unit_t.is_player()) return true;
            else if (部队目标变更_势力条件 == 3) return false;

            return false;
        }

        // 건설편제 여부 판단 함수
        bool check_construction_unit(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return false;
            if (pk::is_player_controlled(unit)) return false; // 유저군단은 무조건 제외

            if (unit.has_skill(特技_筑城) and unit.gold >= 建设编制_标准持有金) return true;  // 축성 특기

            if (unit.type == 부대종류_전투 and pk::get_member_count(unit) <= 1)   // 1인대
            {
                if (unit.gold >= 建设编制_标准持有金 and unit.troops <= 建设编制_标准兵力)
                    return true;
            }
            if (建设编制_任务优待 and unit.order == 부대임무_설치 and unit.target_type == 부대임무대상_좌표)
            {
                int facility_id = unit.target;
                pk::facility@ facility = pk::get_facility(facility_id);
                if (pk::is_alive(facility) and int(unit.gold) >= int(facility.gold_cost))
                {
                    if (시설_진 <= facility_id and facility_id <= 시설_석병팔진)
                        return true;
                }
            }

            return false;
        }

        // 계략부대 여부 판단 함수
        int func_strategy_unit_type(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return -1;
            if (pk::is_player_controlled(unit)) return -1; // 유저군단은 무조건 제외

            // 무조건 우대 특기
            if (unit.has_skill(특기_신산) or unit.has_skill(특기_허실) or unit.has_skill(특기_백출)
                or unit.has_skill(특기_귀모) or unit.has_skill(특기_연환))
                return 3;

            // 보유 특기 확인
            for (int i = 0; i < 3; i++)
            {
                pk::person@ member = pk::get_person(unit.member[i]);
                if (pk::is_alive(member))
                {
                    int skill_id = member.skill;
                    pk::skill@ skill = pk::get_skill(skill_id);

                    // 신/장 계열 공격특기인 경우 계략부대 아닌 것으로 판정 (지력공방비 무시)
                    if (skill_id >= 특기_창장 and skill_id <= 특기_맹장) return 0;

                    // 계략특기 우대
                    if (计谋部队_特技优待 and pk::is_alive(skill))
                    {
                        if (skill.type == 특기종류_계략) return 2;
                    }



                }
            }

            // 지력 대 공방 비율 판단 (일반부대)
            float stat_ratio = float(2.0f * unit.attr.stat[部队能力_智力] / (unit.attr.stat[部队能力_攻击] + unit.attr.stat[部队能力_防御]));
            if (stat_ratio >= 计谋部队_智力攻防比) return 1;


            return 0;
        }


        //---------------------------------------------------------------------------

        // 부대 공격 최소,최대 사거리 함수 -- 신규 추가, 사용가능한 전법의 세팅조건에 따라 계산 ('20.9.13)
        pk::int_int get_atk_range(pk::unit@ unit)
        {
            pk::int_int atk_range = pk::int_int(1, 1);
            if (!pk::is_alive(unit) or unit.weapon == 병기_검) return atk_range;

            int weapon_id = unit.weapon;
            pk::equipment@ epq = pk::get_equipment(unit.weapon);
            string eqp_name = pk::decode(pk::get_name(epq));
            string info = pk::format("{}", eqp_name);

            int min_range = 5;
            int max_range = 1;

            for (int j = 0; j < 전법_끝; j++)
            {
                if (epq.tactics[j])
                {
                    pk::tactics@ tts = pk::get_tactics(j);
                    string tts_name = pk::decode(pk::get_name(tts));
                    info += pk::format(" {}({},{})", tts_name, tts.min_range, tts.max_range);
                    min_range = pk::min(min_range, tts.min_range);
                    max_range = pk::max(max_range, tts.max_range);
                }
            }
            if (weapon_id == 병기_노 and unit.has_tech(기교_강노)) max_range += 1;
            if (weapon_id == 병기_군마 and (unit.has_tech(기교_기사) or unit.has_skill(특기_백마))) max_range = pk::max(2, max_range);
            if ((weapon_id == 병기_정란 or weapon_id == 병기_투석) and unit.has_skill(特技_射程)) max_range += 1;

            atk_range = pk::int_int(min_range, max_range);
            info += pk::format(": 最小({}),最大({})", min_range, max_range);

            //if (调试模式) pk::info(info);

            return atk_range;
        }

        // 부대 공격 최대 사거리 함수
        int get_max_atk_range(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return 1;

            int weapon_id = unit.weapon;
            int max_range = 0;

            if (weapon_id == 병기_노)     max_range = (unit.has_tech(기교_강노)) ? 3 : 2;
            else if (weapon_id == 병기_군마)   max_range = (unit.has_tech(기교_기사) or unit.has_skill(특기_백마)) ? 2 : 1;
            else if (weapon_id == 병기_정란)   max_range = ((unit.has_skill(特技_射程)) ? 1 : 0) + (pk::get_tactics(전법_공성화시).max_range);
            else if (weapon_id == 병기_투석)   max_range = ((unit.has_skill(特技_射程)) ? 1 : 0) + (pk::get_tactics(전법_공성투석).max_range);
            else if (weapon_id >= 兵器_走舸)   max_range = 2;
            else                             max_range = 1;     // 검, 창, 극, 목수 등

            return max_range;
        }


        // 목표부대 가중치 계산 함수 (상성, 적성, 방어력, 신분 고려)
        float get_attr_weight(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return 0.0f;
            int weapon_id = unit.weapon;
            int heishu_id = pk::equipment_id_to_heishu(weapon_id);
            int tekisei = pk::min(적성_S, pk::get_tekisei(unit));
            int unit_atk = unit.attr.stat[部队能力_攻击];
            int unit_def = unit.attr.stat[部队能力_防御];
            int unit_skill = func_strategy_unit_type(unit);

            if (heishu_id < 0 or heishu_id >= 6) return 0.0f;

            float relative_stat = 0.0f;
            float weight = main.heishu_weight[heishu_id];

            // 상성, 적성 가중치 적용
            relative_stat = pk::max(0.f, weight) + float(적성_끝 - tekisei) / float(적성_끝);

            // 공격력 가중치 적용
            if (部队目标变更_攻击权重值适用) relative_stat += float(unit_atk) / 128.0f;
            // 방여력 가중치 적용
            if (部队目标变更_防御权重值适用) relative_stat += float(unit_def) / 128.0f;




            // 군주/도독 신분 가중치 적용
            if (部队目标变更_身份权重值适用)
            {
                if (unit.leader == pk::get_kunshu_id(unit)) relative_stat += 0.50f;
                else if (unit.leader == pk::get_totoku_id(unit)) relative_stat += 0.25f;


            }

            // 병력 가중치 적용 ('19.3.1)
            if (部队目标变更_兵力权重值适用)
            {
                if (int(unit.troops) <= 5000)
                    relative_stat += float(5000 - unit.troops) / 10000.f;
            }

            // 계략우대 특기 보유 가중치 적용 ('20.8.30)
            if (部队目标变更_计略权重值适用 and (unit_skill > 1)) relative_stat += 0.4f * float(unit_skill);

            return relative_stat;
        }

        // 병종상성 설정함수
        void set_heishu_weight(int weapon_id)
        {
            int heishu_id = pk::equipment_id_to_heishu(weapon_id);

            if (heishu_id == 병종_창병)
            {
                heishu_weight[병종_창병] = heishu_normal;
                heishu_weight[병종_극병] = heishu_weak;
                heishu_weight[병종_노병] = heishu_normal;
                heishu_weight[병종_기병] = heishu_strong;
                heishu_weight[병종_병기] = heishu_strong;
                heishu_weight[兵种_水军] = heishu_null;
            }
            else if (heishu_id == 병종_극병)
            {
                heishu_weight[병종_창병] = heishu_strong;
                heishu_weight[병종_극병] = heishu_normal;
                heishu_weight[병종_노병] = heishu_normal;
                heishu_weight[병종_기병] = heishu_weak;
                heishu_weight[병종_병기] = heishu_strong;
                heishu_weight[兵种_水军] = heishu_null;
            }
            else if (heishu_id == 병종_노병)
            {
                heishu_weight[병종_창병] = heishu_normal;
                heishu_weight[병종_극병] = heishu_weak;
                heishu_weight[병종_노병] = heishu_normal;
                heishu_weight[병종_기병] = heishu_strong;
                heishu_weight[병종_병기] = heishu_strong;
                heishu_weight[兵种_水军] = heishu_normal;
            }
            else if (heishu_id == 병종_기병)
            {
                heishu_weight[병종_창병] = heishu_weak;
                heishu_weight[병종_극병] = heishu_strong;
                heishu_weight[병종_노병] = heishu_strong;
                heishu_weight[병종_기병] = heishu_normal;
                heishu_weight[병종_병기] = heishu_strong;
                heishu_weight[兵种_水军] = heishu_null;
            }
            else if (heishu_id == 병종_병기)
            {
                heishu_weight[병종_창병] = heishu_normal;
                heishu_weight[병종_극병] = heishu_normal;
                heishu_weight[병종_노병] = heishu_normal;
                heishu_weight[병종_기병] = heishu_normal;
                heishu_weight[병종_병기] = heishu_normal;
                heishu_weight[兵种_水军] = heishu_null;
            }
            else if (heishu_id == 兵种_水军)
            {
                heishu_weight[병종_창병] = heishu_normal;
                heishu_weight[병종_극병] = heishu_normal;
                heishu_weight[병종_노병] = heishu_normal;
                heishu_weight[병종_기병] = heishu_normal;
                heishu_weight[병종_병기] = heishu_weak;
                heishu_weight[兵种_水军] = heishu_strong;
            }
            else
            {
                heishu_weight[병종_창병] = heishu_weak;
                heishu_weight[병종_극병] = heishu_weak;
                heishu_weight[병종_노병] = heishu_weak;
                heishu_weight[병종_기병] = heishu_weak;
                heishu_weight[병종_병기] = heishu_normal;
                heishu_weight[兵种_水军] = heishu_null;
            }
        }


        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------

        int get_weapon_type(int weapon_id)
        {
            int weapon_type = -1;
            if (weapon_id == 병기_검)                          weapon_type = 0;
            else if (weapon_id >= 병기_창 and weapon_id <= 병기_군마)  weapon_type = 1;
            else if (weapon_id == 병기_충차)                         weapon_type = 2;
            else if (weapon_id >= 병기_정란 and weapon_id <= 병기_목수) weapon_type = 3;
            else if (weapon_id == 兵器_走舸)                         weapon_type = 4;
            else if (weapon_id > 兵器_走舸)                         weapon_type = 5;

            return weapon_type;
        }

        string get_weapon_name(int weapon_id)
        {
            string weapon_name;
            switch (weapon_id)
            {
            case 병기_검: weapon_name = "剑兵"; break;
            case 병기_창: weapon_name = "枪兵"; break;
            case 병기_극: weapon_name = "戟兵"; break;
            case 병기_노: weapon_name = "弓兵"; break;
            case 병기_군마: weapon_name = "骑兵"; break;
            case 병기_충차: weapon_name = "冲车"; break;
            case 병기_정란: weapon_name = "井栏"; break;
            case 병기_투석: weapon_name = "投石"; break;
            case 병기_목수: weapon_name = "木兽"; break;
            case 兵器_走舸: weapon_name = "走舸"; break;
            case 兵器_楼船: weapon_name = "楼船"; break;
            case 兵器_斗舰: weapon_name = "斗舰"; break;
            default: weapon_name = "??"; break;
            }
            return weapon_name;
        }


        string get_tekisei_name(int tekisei)
        {
            string tekisei_name;
            switch (tekisei)
            {
            case 适性_C: tekisei_name = "C"; break;
            case 적성_B: tekisei_name = "B"; break;
            case 적성_A: tekisei_name = "A"; break;
            case 적성_S: tekisei_name = "S"; break;
            default: tekisei_name = "↑"; break;
            }
            return tekisei_name;
        }


        string get_heishu_color(int weapon_id)
        {
            string text_color;
            float weight = main.heishu_weight[pk::equipment_id_to_heishu(weapon_id)];

            if (weapon_id == 병기_검)      text_color = "\x1b[2x"; // 녹색
            else if (weight == heishu_null) text_color = "\x1b[29x"; // 빨강
            else if (weight == heishu_weak) text_color = "\x1b[16x"; // 주황
            else if (weight == heishu_normal) text_color = "\x1b[17x"; // 노랑
            else if (weight == heishu_strong) text_color = "\x1b[2x"; // 녹색
            else                              text_color = "\x1b[0x"; // 흰색

            return text_color;
        }

        //---------------------------------------------------------------------------

        void clear_target_info()
        {
            for (int i = 0; i < 部队_末; i++)
            {
                arr_target_unit_id[i] = -1;
                arr_target_base_id[i] = -1;
            }
        }

        //---------------------------------------------------------------------------------------

        string get_order_info(int order)
        {
            string name;
            switch (order)
            {
            case 0: name = "부대임무_대기"; break;
            case 1: name = "部队任务_移动"; break;
            case 2: name = "부대임무_설치"; break;
            case 3: name = "部队任务_攻击"; break;
            case 4: name = "부대임무_퇴각"; break;
            case 5: name = "부대임무_물림"; break;
            case 6: name = "부대임무_호위"; break;
            case 7: name = "부대임무_공성"; break;
            case 8: name = "부대임무_수복"; break;
            case 9: name = "부대임무_정복"; break;
            case 10: name = "부대임무_보급"; break;
            case 11: name = "부대임무_섬멸"; break;
            case 12: name = "부대임무_추종"; break;
            case 13: name = "부대임무_끝"; break;
            default: name = "부대임무_없음"; break;
            }

            return name;
        }


        bool 调试模式 = false;
    }


    Main main;
}


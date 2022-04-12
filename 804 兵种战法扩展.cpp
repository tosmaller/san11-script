// ## 2020/07/26 ##
/*
@만든이: 기마책사
@Update: '19.3.2   / 최초적용: 기교개발에 따른 병과별 추가전법 교차사용
@Update: '19.5.4   / 변경내용: 타이틀화면 초기화 조건 추가

*/

namespace 야전부대전법확장
{
    //---------------------------------------------------------------------------------------
    // 유저 설정
    const bool 야전부대전법_기교설정 = true;  // 기교에 따라 병과별 추가전법 교차사용 (정예공성, 정예전투 사용가능)

    const bool 야전부대전법_정예공성 = true;  // 운제 & 정예기교 개발시 전투병과의 파쇄 전법 추가
    const bool 야전부대전법_정예전투 = true;  // 정예기교 개발 시 전투병과의 전투용 전법추가


    //---------------------------------------------------------------------------------------


    class Main
    {

        Main()
        {
            pk::bind(102, pk::trigger102_t(onGameInit));
            pk::bind(111, pk::trigger111_t(onTurnStart));
            pk::bind(112, pk::trigger112_t(onTurnEnd));

            pk::bind(100, pk::trigger100_t(onGameTitle));
        }

        //---------------------------------------------------------------------------------------

        // 스크립트 설정용 변수 (수정금지)
        pk::array<bool> 창병전법(전법_끝, false);
        pk::array<bool> 극병전법(전법_끝, false);
        pk::array<bool> 노병전법(전법_끝, false);
        pk::array<bool> 기병전법(전법_끝, false);
        pk::array<bool> 충차전법(전법_끝, false);
        pk::array<bool> 목수전법(전법_끝, false);
        pk::array<bool> 정란전법(전법_끝, false);
        pk::array<bool> 투석전법(전법_끝, false);
        pk::array<bool> 주가전법(전법_끝, false);
        pk::array<bool> 누선전법(전법_끝, false);
        pk::array<bool> 투함전법(전법_끝, false);

        //---------------------------------------------------------------------------------------

        // 게임 초기화
        void onGameInit()
        {
            change_tactics_name();

            pk::force@ force = pk::get_force(pk::get_current_turn_force_id());

            onTurnStart(force);     // 턴 시작 함수 실행
        }

        // 매턴 시작 시 : 병과별 전법설정값 백업, 세력 조건에 따른 병과별 전법설정값
        void onTurnStart(pk::force@ force)
        {
            reset_tactics_info();   // 초기화
            //backup_tactics_info();  // 백업

            if (야전부대전법_기교설정)
                custom_tech_tactics(force); // 커스텀 기교전법 설정
        }

        // 매턴 종료 시 병과별 전법설정값 원복
        void onTurnEnd(pk::force@ force)
        {
            reset_tactics_info();   // 초기화
        }


        // 타이틀 화면
        void onGameTitle()
        {
            backup_tactics_info();  // 백업
        }

        //---------------------------------------------------------------------------------------

        // 전법 명칭 변경 함수
        void change_tactics_name()
        {
            pk::get_tactics(전법_공성화시).name = pk::encode("火箭");
        }

        //---------------------------------------------------------------------------------------

        // 커스텀 기교전법 설정 함수
        void custom_tech_tactics(pk::force@ force)
        {
            if (!pk::is_normal_force(force)) return;

            // 정예기교 : 타병과 전법 사용가능
            if (야전부대전법_정예전투)
            {
                // 정예창병 : 횡치 사용가능
                if (pk::has_tech(force, 기교_정예창병))
<<<<<<< HEAD
                    pk::get_equipment(병기_창).tactics[전법_횡치] = true;

                // 정예극병 : 나선첨 사용가능
                if (pk::has_tech(force, 기교_정예극병))
                    pk::get_equipment(병기_극).tactics[전법_나선첨] = true;

                // 정예노병 : 공성화시 사용가능
                if (pk::has_tech(force, 기교_정예노병))
                    pk::get_equipment(병기_노).tactics[전법_공성화시] = true;

                // 정예기병 : 화시 사용가능
                if (pk::has_tech(force, 기교_정예기병))
                    pk::get_equipment(병기_군마).tactics[전법_화시] = true;
=======
                    pk::get_equipment(兵器_枪).tactics[전법_횡치] = true;

                // 정예극병 : 나선첨 사용가능
                if (pk::has_tech(force, 기교_정예극병))
                    pk::get_equipment(兵器_戟).tactics[전법_나선첨] = true;

                // 정예노병 : 공성화시 사용가능
                if (pk::has_tech(force, 기교_정예노병))
                    pk::get_equipment(兵器_弩).tactics[전법_공성화시] = true;

                // 정예기병 : 화시 사용가능
                if (pk::has_tech(force, 기교_정예기병))
                    pk::get_equipment(兵器_战马).tactics[전법_화시] = true;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
            }

            // 기교_운제 & 정예기교 : 공성파쇄 사용가능
            if (야전부대전법_정예공성 and pk::has_tech(force, 기교_운제))
            {
                if (pk::has_tech(force, 기교_정예창병))
<<<<<<< HEAD
                    pk::get_equipment(병기_창).tactics[전법_공성파쇄] = true;

                if (pk::has_tech(force, 기교_정예극병))
                    pk::get_equipment(병기_극).tactics[전법_공성파쇄] = true;

                if (pk::has_tech(force, 기교_정예노병))
                    pk::get_equipment(병기_노).tactics[전법_공성파쇄] = true;

                if (pk::has_tech(force, 기교_정예기병))
                    pk::get_equipment(병기_군마).tactics[전법_공성파쇄] = true;
=======
                    pk::get_equipment(兵器_枪).tactics[전법_공성파쇄] = true;

                if (pk::has_tech(force, 기교_정예극병))
                    pk::get_equipment(兵器_戟).tactics[전법_공성파쇄] = true;

                if (pk::has_tech(force, 기교_정예노병))
                    pk::get_equipment(兵器_弩).tactics[전법_공성파쇄] = true;

                if (pk::has_tech(force, 기교_정예기병))
                    pk::get_equipment(兵器_战马).tactics[전법_공성파쇄] = true;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
            }

        }


        //---------------------------------------------------------------------------------------

        // 창극노기 전법 설정 백업
        void backup_tactics_info()
        {
            for (int i = 0; i < 전법_끝; i++)
            {
<<<<<<< HEAD
                창병전법[i] = pk::get_equipment(병기_창).tactics[i];
                극병전법[i] = pk::get_equipment(병기_극).tactics[i];
                노병전법[i] = pk::get_equipment(병기_노).tactics[i];
                기병전법[i] = pk::get_equipment(병기_군마).tactics[i];
                충차전법[i] = pk::get_equipment(병기_충차).tactics[i];
                목수전법[i] = pk::get_equipment(병기_목수).tactics[i];
                정란전법[i] = pk::get_equipment(병기_정란).tactics[i];
                투석전법[i] = pk::get_equipment(병기_투석).tactics[i];
=======
                창병전법[i] = pk::get_equipment(兵器_枪).tactics[i];
                극병전법[i] = pk::get_equipment(兵器_戟).tactics[i];
                노병전법[i] = pk::get_equipment(兵器_弩).tactics[i];
                기병전법[i] = pk::get_equipment(兵器_战马).tactics[i];
                충차전법[i] = pk::get_equipment(兵器_冲车).tactics[i];
                목수전법[i] = pk::get_equipment(兵器_木兽).tactics[i];
                정란전법[i] = pk::get_equipment(兵器_井阑).tactics[i];
                투석전법[i] = pk::get_equipment(兵器_投石).tactics[i];
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
                주가전법[i] = pk::get_equipment(兵器_走舸).tactics[i];
                누선전법[i] = pk::get_equipment(兵器_楼船).tactics[i];
                투함전법[i] = pk::get_equipment(兵器_斗舰).tactics[i];
            }
        }

        // 창극노기 전법 설정 가져오기
        void reset_tactics_info()
        {
            for (int i = 0; i < 전법_끝; i++)
            {
<<<<<<< HEAD
                pk::get_equipment(병기_창).tactics[i] = 창병전법[i];
                pk::get_equipment(병기_극).tactics[i] = 극병전법[i];
                pk::get_equipment(병기_노).tactics[i] = 노병전법[i];
                pk::get_equipment(병기_군마).tactics[i] = 기병전법[i];
                pk::get_equipment(병기_충차).tactics[i] = 충차전법[i];
                pk::get_equipment(병기_목수).tactics[i] = 목수전법[i];
                pk::get_equipment(병기_정란).tactics[i] = 정란전법[i];
                pk::get_equipment(병기_투석).tactics[i] = 투석전법[i];
=======
                pk::get_equipment(兵器_枪).tactics[i] = 창병전법[i];
                pk::get_equipment(兵器_戟).tactics[i] = 극병전법[i];
                pk::get_equipment(兵器_弩).tactics[i] = 노병전법[i];
                pk::get_equipment(兵器_战马).tactics[i] = 기병전법[i];
                pk::get_equipment(兵器_冲车).tactics[i] = 충차전법[i];
                pk::get_equipment(兵器_木兽).tactics[i] = 목수전법[i];
                pk::get_equipment(兵器_井阑).tactics[i] = 정란전법[i];
                pk::get_equipment(兵器_投石).tactics[i] = 투석전법[i];
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
                pk::get_equipment(兵器_走舸).tactics[i] = 주가전법[i];
                pk::get_equipment(兵器_楼船).tactics[i] = 누선전법[i];
                pk::get_equipment(兵器_斗舰).tactics[i] = 투함전법[i];
            }
        }

        //---------------------------------------------------------------------------------------
    }

    Main main;
}


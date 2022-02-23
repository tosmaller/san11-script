// ## 2021/10/29 #江东新风#张飞原设定未考虑到能力成长为开眼，故将智力上限降低到60##
// ## 2020/08/11 #messi#修正李平特技无的bug##
// ## 2020/07/26 ##
/*
@ 제작자: HoneyBee
@ 내용: 무장들의 설정을 변경하는 용도의 SCRIPT


※ 특정 무장들의 설정을 변경
- <장비>의 공적에 따라 <장비>의 지력이 자동 상승 (지력 70 미만인 경우)
- 225년 (남만정벌 시나리오)연도부터 <이엄>의 이름이 <이평>으로 자동 변경


*/


namespace 무장설정변경
{

	// ================ CUSTOMIZE ================

	const int 지력상승_장비 = 5; 	 	// <장비>의 지력 상승치
	const int 공적조건_1 = 6000; 	 	// 1번째 공적조건 (기본: 6000 이상)
	const int 공적조건_2 = 8000; 	 	// 2번째 공적조건 (기본: 8000 이상)
	const int 공적조건_3 = 10000; 	 	// 3번째 공적조건 (기본: 10000 이상)
	const int 공적조건_4 = 12000; 	 	// 4번째 공적조건 (기본: 12000 이상)

	const int 이평_연도 = 225; 	 	// <이평>으로 이름이 변경되는 연도 (기본: 225년)


// ===========================================



	class Main
	{

		pk::person@ 장비 = pk::get_person(무장_장비);
		pk::person@ 이엄 = pk::get_person(무장_이엄);



		Main()
		{
			pk::bind(111, pk::trigger111_t(onTurnStart));
		}

		void onTurnStart(pk::force@ force)
		{
			//set_character(force);
		}

		void set_character(pk::force@ force)
		{

			// <장비>의 지력 상승	
			if ((장비.base_stat[武将能力_智力] < 60) and (장비.kouseki >= 공적조건_1))
			{
				장비.base_stat[武将能力_智力] = 장비.base_stat[武将能力_智力] + 지력상승_장비;
			}

			if ((장비.base_stat[武将能力_智力] < 60) and (장비.kouseki >= 공적조건_2))
			{
				장비.base_stat[武将能力_智力] = 장비.base_stat[武将能力_智力] + 지력상승_장비;
			}

			if ((장비.base_stat[武将能力_智力] < 60) and (장비.kouseki >= 공적조건_3))
			{
				장비.base_stat[武将能力_智力] = 장비.base_stat[武将能力_智力] + 지력상승_장비;
			}

			if ((장비.base_stat[武将能力_智力] < 60) and (장비.kouseki >= 공적조건_4))
			{
				장비.base_stat[武将能力_智力] = 장비.base_stat[武将能力_智力] + 지력상승_장비;
			}

			장비.update();

			//-------------------------------------------------------------------------------------			

						// <이엄>의 이름 변경 (from 이엄 to 이평)	
			if (pk::get_year() >= 이평_연도)
			{
				이엄.mei = pk::encode("平"); // 이름 (평)
				이엄.update();
			}


		} //set_character(force)

	} // class Main

	Main main;
}
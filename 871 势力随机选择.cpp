// ## 2020/12/11 # 江东新风 # 修正变量重复导致报错的问题 ##
// ## 2020/07/26 ##
/*
@ 제작자: HoneyBee
@ 내용: 모든 공식 시나리오에서 세력을 랜덤 선택할 수 있는 SCRIPT

※ 반드시 GAME을 진행하기 前, 해당 기능을 사용하시기 바랍니다.
  (이미 한창 GAME이 진행된 상태에서 사용하는 것은 권장하지 않음)

*/


namespace 랜덤세력_선택
{

	const int KEY = pk::hash("症肱弘桑松俳");

	class Main
	{

		Main()
		{
			pk::menu_item item;
			item.menu = 2;
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}

		string getText()
		{
			return pk::encode("势力随机选择");
		}

		string getDesc()
		{
			return pk::encode("执行势力随机选择");
		}

		bool handler()
		{

			//pk::force@ new_force;
			pk::person@ 초선 = pk::get_person(무장_초선);
			pk::list<pk::force@> force_list = pk::get_force_list();

			for (int i = 0; i < force_list.count; i++)
			{
				pk::force@ force = force_list[i];

				// 황건봉기 시나리오
				if (pk::get_scenario().no == 0)
				{
					int n = pk::rand(8);

					// 손견 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();
					}

					// 마등 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(3);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();
					}

					// 유언 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(4);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();
					}

					// 동탁 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(5);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();
					}

					// 공손찬 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(11);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();
					}

					// 장각 세력 
					if ((n == 5) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(28);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();
					}

					// 하진 세력 
					if ((n == 6) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(29);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();
					}

					// 정원 세력 
					if ((n == 7) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(30);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();
					}

				}

				// 반동탁연합 시나리오
				if (pk::get_scenario().no == 1)
				{
					int n = pk::rand(19);

					// 조조 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(0);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();
					}

					// 유비 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(1);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();
					}

					// 손견 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 마등 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(3);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유언 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(4);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 동탁 세력 
					if ((n == 5) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(5);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유표 세력 
					if ((n == 6) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(6);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 원소 세력 
					if ((n == 7) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(7);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 원술 세력 
					if ((n == 8) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(8);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장로 세력 
					if ((n == 9) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(9);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손찬 세력 
					if ((n == 10) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(11);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손도 세력 
					if ((n == 11) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(12);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유우 세력 
					if ((n == 12) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(13);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 한복 세력 
					if ((n == 13) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(14);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공융 세력 
					if ((n == 14) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(16);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유대 세력 
					if ((n == 15) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(17);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 도겸 세력 
					if ((n == 16) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(18);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공주 세력 
					if ((n == 17) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(19);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장양 세력 
					if ((n == 18) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(20);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

				}

				// 군웅할거 시나리오
				if (pk::get_scenario().no == 2)
				{
					int n = pk::rand(19);

					// 조조 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(0);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유비 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(1);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 손책 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 마등 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(3);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유장 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(4);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 이각 세력 
					if ((n == 5) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(5);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

						pk::person@ kunshu = pk::get_person(new_force.kunshu);
						string kunshu_name = pk::decode(pk::get_name(kunshu));
					}

					// 유표 세력 
					if ((n == 6) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(6);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 원소 세력 
					if ((n == 7) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(7);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

						pk::person@ kunshu = pk::get_person(new_force.kunshu);
						string kunshu_name = pk::decode(pk::get_name(kunshu));

					}

					// 원술 세력 
					if ((n == 8) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(8);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장로 세력 
					if ((n == 9) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(9);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 여포 세력 
					if ((n == 10) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(10);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손찬 세력 
					if ((n == 11) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(11);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손도 세력 
					if ((n == 12) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(12);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공융 세력 
					if ((n == 13) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(16);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 도겸 세력 
					if ((n == 14) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(18);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장양 세력 
					if ((n == 15) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(20);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 왕랑 세력 
					if ((n == 16) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(22);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 엄백호 세력 
					if ((n == 17) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(23);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유요 세력 
					if ((n == 18) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(24);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

				}

				// 관도대전 시나리오
				if (pk::get_scenario().no == 3)
				{
					int n = pk::rand(10);

					// 조조 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(0);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유비 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(1);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 손책 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 마등 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(3);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유장 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(4);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유표 세력 
					if ((n == 5) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(6);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 원소 세력 
					if ((n == 6) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(7);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장로 세력 
					if ((n == 7) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(9);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손도 세력 
					if ((n == 8) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(12);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장수 세력 
					if ((n == 9) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(25);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

				}

				// 삼고초려 시나리오
				if (pk::get_scenario().no == 4)
				{
					int n = pk::rand(12);

					// 조조 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(0);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유비 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(1);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 손권 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 마등 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(3);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유장 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(4);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유표 세력 
					if ((n == 5) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(6);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장로 세력 
					if ((n == 6) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(9);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손강 세력 
					if ((n == 7) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(12);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 한현 세력 
					if ((n == 8) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(31);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 금선 세력 
					if ((n == 9) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(32);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 조범 세력 
					if ((n == 10) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(33);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유도 세력 
					if ((n == 11) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(34);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

				}

				// 유비입촉 시나리오
				if (pk::get_scenario().no == 5)
				{
					int n = pk::rand(7);

					// 조조 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(0);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유비 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(1);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 손권 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 마초 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(3);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유장 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(4);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장로 세력 
					if ((n == 5) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(9);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손공 세력 
					if ((n == 6) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(12);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

				}

				// 남만정벌 시나리오
				if (pk::get_scenario().no == 6)
				{
					int n = pk::rand(5);

					// 조조 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(0);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유선 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(1);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 손권 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손공 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(12);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 맹획 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(26);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

				}

				// 영웅집결 시나리오
				if (pk::get_scenario().no == 7)
				{
					int n = pk::rand(37);

					// 조조 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(0);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유비 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(1);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 손견 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 마등 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(3);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유언 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(4);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 동탁 세력 
					if ((n == 5) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(5);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유표 세력 
					if ((n == 6) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(6);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 원소 세력 
					if ((n == 7) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(7);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 원술 세력 
					if ((n == 8) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(8);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장로 세력 
					if ((n == 9) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(9);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 여포 세력 
					if ((n == 10) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(10);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손찬 세력 
					if ((n == 11) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(11);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손도 세력 
					if ((n == 12) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(12);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유우 세력 
					if ((n == 13) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(13);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 한복 세력 
					if ((n == 14) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(14);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 사마의 세력 
					if ((n == 15) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(15);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공융 세력 
					if ((n == 16) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(16);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유대 세력 
					if ((n == 17) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(17);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 도겸 세력 
					if ((n == 18) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(18);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공주 세력 
					if ((n == 18) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(19);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 정원 세력 
					if ((n == 20) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(20);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 왕랑 세력 
					if ((n == 21) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(22);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 엄백호 세력 
					if ((n == 22) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(23);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유요 세력 
					if ((n == 23) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(24);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장수 세력 
					if ((n == 24) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(25);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 맹획 세력 
					if ((n == 25) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(26);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 옹개 세력 
					if ((n == 26) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(27);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장각 세력 
					if ((n == 27) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(28);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 하진 세력 
					if ((n == 28) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(29);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 한현 세력 
					if ((n == 29) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(31);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 금선 세력 
					if ((n == 30) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(32);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 조범 세력 
					if ((n == 31) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(33);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유도 세력 
					if ((n == 32) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(34);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 종회 세력 
					if ((n == 33) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(38);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 등애 세력 
					if ((n == 34) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(39);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 관구검 세력 
					if ((n == 35) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(40);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 한수 세력 
					if ((n == 36) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(41);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

				}

				// 여포토벌전 시나리오
				if (pk::get_scenario().no == 8)
				{
					int n = pk::rand(14);

					// 조조 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(0);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유비 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(1);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 손책 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 마등 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(3);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유장 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(4);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유표 세력 
					if ((n == 5) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(6);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 원소 세력 
					if ((n == 6) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(7);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 원술 세력 
					if ((n == 7) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(8);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장로 세력 
					if ((n == 8) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(9);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 여포 세력 
					if ((n == 9) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(10);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손찬 세력 
					if ((n == 10) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(11);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손도 세력 
					if ((n == 11) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(12);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장양 세력 
					if ((n == 12) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(20);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장수 세력 
					if ((n == 13) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(25);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

				}

				// 원가분열 시나리오
				if (pk::get_scenario().no == 9)
				{
					int n = pk::rand(10);

					// 조조 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(0);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유비 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(1);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 손권 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 마등 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(3);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유장 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(4);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유표 세력 
					if ((n == 5) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(6);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 원상 세력 
					if ((n == 6) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(7);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장로 세력 
					if ((n == 7) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(9);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손도 세력 
					if ((n == 8) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(12);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 원담 세력 
					if ((n == 9) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(40);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

				}

				// 한중쟁탈전 시나리오
				if (pk::get_scenario().no == 10)
				{
					int n = pk::rand(5);

					// 조조 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(0);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유비 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(1);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 손권 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손공 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(12);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 맹획 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(26);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

				}

				// 하진포위망 시나리오
				if (pk::get_scenario().no == 11)
				{
					int n = pk::rand(22);

					// 조조 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(0);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유비 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(1);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 손권 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 마등 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(3);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유언 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(4);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 동탁 세력 
					if ((n == 5) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(5);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유표 세력 
					if ((n == 6) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(6);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 원술 세력 
					if ((n == 7) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(8);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장로 세력 
					if ((n == 8) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(9);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손찬 세력 
					if ((n == 9) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(11);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공융 세력 
					if ((n == 10) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(16);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 도겸 세력 
					if ((n == 11) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(18);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장연 세력 
					if ((n == 12) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(21);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 관해 세력 
					if ((n == 13) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(28);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 하진 세력 
					if ((n == 14) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(29);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 정원 세력 
					if ((n == 15) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(30);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 진규 세력 
					if ((n == 16) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(35);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 순욱 세력 
					if ((n == 17) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(36);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 누규 세력 
					if ((n == 18) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(38);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 구성 세력 
					if ((n == 18) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(39);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 방회 세력 
					if ((n == 20) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(40);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 엄안 세력 
					if ((n == 21) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(41);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

				}

				// 삼대군벌 시나리오
				if (pk::get_scenario().no == 12)
				{
					int n = pk::rand(15);

					// 장흠 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 마등 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(3);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유언 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(4);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 동탁 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(5);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유표 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(6);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 원소 세력 
					if ((n == 5) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(7);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 원술 세력 
					if ((n == 6) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(8);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장로 세력 
					if ((n == 7) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(9);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손찬 세력 
					if ((n == 8) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(11);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 공손도 세력 
					if ((n == 9) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(12);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유우 세력 
					if ((n == 10) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(13);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장양 세력 
					if ((n == 11) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(20);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 엄백호 세력 
					if ((n == 12) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(23);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유요 세력 
					if ((n == 13) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(24);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유벽 세력 
					if ((n == 14) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(28);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

				}

				// 여인천하 시나리오
				if (pk::get_scenario().no == 13)
				{
					int n = pk::rand(22);

					// 변씨 세력 
					if ((n == 0) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(0);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 미씨 세력 
					if ((n == 1) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(1);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 손상향 세력 
					if ((n == 2) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(2);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 마운록 세력 
					if ((n == 3) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(3);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 채씨 세력 
					if ((n == 4) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(6);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 유씨 세력 
					if ((n == 5) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(7);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 손씨 세력 
					if ((n == 6) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(8);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 초선 세력 
					if ((n == 7) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(10);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 장춘화 세력 
					if ((n == 8) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(15);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 추씨 세력 
					if ((n == 9) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(25);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 축융 세력 
					if ((n == 10) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(26);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 손노반 세력 
					if ((n == 11) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(31);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 채염 세력 
					if ((n == 12) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(32);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 번씨 세력 
					if ((n == 13) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(33);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 신헌영 세력 
					if ((n == 14) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(34);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 하후영녀 세력 
					if ((n == 15) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(35);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 견씨 세력 
					if ((n == 16) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(36);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 포삼랑 세력 
					if ((n == 17) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(37);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 황월영 세력 
					if ((n == 18) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(38);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 왕이 세력 
					if ((n == 18) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(39);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 서씨 세력 
					if ((n == 20) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(40);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

					// 대교 세력 
					if ((n == 21) and force.is_player())
					{
						pk::force@ new_force = pk::get_force(41);
						force.player = -1;
						force.update();

						new_force.player = 0;
						new_force.update();

					}

				}

			} // for 

			pk::message_box(pk::encode("势力变更完成!"), 초선);
			pk::message_box(pk::encode("如果选择的势力非同一势力,下个回合后将会以变更的势力重新开始.请谨慎考虑~"), 초선);


			return true;

		} // bool handler

	} // class Main

	Main main;
}
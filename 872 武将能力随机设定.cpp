// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/07/26 ##
/*
@ 제작자: HoneyBee
@ 내용: 모든 무장들의 능력을 RANDOM으로 재설정할 수 있는 SCRIPT


※ 무장의 능력에서 변경되는 요소
- 무장의 특기
- 무장의 능력치 (통솔, 무력, 지력, 정치, 매력)
- 무장의 성장 TYPE (초지속형 등..)
- 무장의 병종적성

*/


namespace 무장능력_랜덤설정
{

	const int KEY = pk::hash("钦肱弘桑樟单");

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
			return pk::encode("随机设置武将能力");
		}

		string getDesc()
		{
			return pk::encode("随机更改武将能力.");
		}

		bool handler()
		{
			if (pk::choose(pk::encode("武将的能力是否全部随机更改?"), { pk::encode(" 是 "), pk::encode(" 否 ") }) == 1) return false;

			pk::random random(pk::rand());
			pk::person@ 초선 = pk::get_person(무장_초선);

			auto building_list = pk::list_to_array(pk::get_building_list());

			for (int i = 0; i < int(building_list.length); i++)
			{
				pk::building@ building = building_list[i];

				pk::person@ taishu = pk::get_person(pk::get_taishu_id(building));

				auto mibun_list = pk::list_to_array(pk::get_person_list(building, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般, 신분_재야, 신분_포로, 신분_미등장, 신분_미발견)));

				if (pk::is_alive(taishu))
				{

					for (int j = 0; j < int(mibun_list.length); j++)
					{
						pk::person@ person_0 = mibun_list[j];

						for (int k = 0; k < 武将能力_末; k++)
						{
							person_0.base_stat[k] = random(1, 100);
							person_0.stat_aging[k] = random(능력성장_초지속형, 능력성장_장비형);
						}

						for (int k = 0; k < 병종_끝; k++)
						{
							person_0.tekisei[k] = random(0, 3);
						}

						person_0.skill = random(특기_비장, 특기_내조);

						person_0.update();

					} // for 

				} // if 

			} // for 


			pk::message_box(pk::encode("武将的能力变更了!"), 초선);
			pk::message_box(pk::encode("以崭新的心情游玩游戏把~"), 초선);


			return true;

		} // bool handler

	} // class Main

	Main main;
}
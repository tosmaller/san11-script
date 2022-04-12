<<<<<<< HEAD
﻿// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
=======
﻿// ## 2022/03/15 # 江东新风 # 修改写法，防止出错 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
// ## 2020/11/20 # 江东新风 # 经验值上升的提示信息改为history_log ##
// ## 2020/08/16 # 江东新风 #has_skill函数替換##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@수정자: 기마책사님
@설명: 내조 특기를 보유한 무장이 부부이고, 부부가 같은 부대에 속해 있다면 매턴마다 부대에 있는 부부인 무장들의 능력 경험치 획득 효과
@기타: 내조 특기 효과가 정상적으로 반영되지 않아서 제작한 SCRIPT

	 유비, 감씨 => 내조 효과 발동  (둘다 서로 배우자로 설정되어 있으므로 발동)
	 유비, 미씨 => 내조 효과 미발동 (미씨의 배우자는 유비이지만, 유비의 배우자가 감씨이므로 발동 안됨)

	 ※ 부부인 무장들만 능력 경험치가 상승함
<<<<<<< HEAD

*/

namespace 내조_효과
=======
@修正者:骑马的策士
@说明:拥有贤内助特长的武装是夫妻，夫妻同属一个部队的话，每一次都有在部队中夫妻武装获得能力经验的效果。
@其他:贤内助特技效果不能正常反映而制作的脚本

刘备，甘氏=>发动贤内助效果(两个都设定为配偶而发动)
刘备，美氏=内助效果未发(刘备的配偶是刘备，但刘备的配偶是甘氏，所以不能发动)

※夫妻武装的能力经验值上升
*/

namespace 内助_效果
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
{

	// ================ CUSTOMIZE ================

	const int exp_0 = 2;    	 // 통솔 경험치 추가 획득량统率经验值
	const int exp_1 = 2;         // 무력 경험치 추가 획득량武力经验值
	const int exp_2 = 2;         // 지력 경험치 추가 획득량智力经验值
	const int exp_3 = 2;         // 정치 경험치 추가 획득량政治经验值
	const int exp_4 = 2;         // 매력 경험치 추가 획득량魅力经验值

<<<<<<< HEAD
	const int 내조효과_발동시기 = 0;    // 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일
=======
	const int 发动时期 = 0;    // 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

// ===========================================


	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{
<<<<<<< HEAD
			if (내조효과_발동시기 == 0)
				spousal_Support();  // 매턴
			else if (내조효과_발동시기 == 1 and (pk::get_day() == 1))
				spousal_Support();  // 매월 1일
			else if (내조효과_발동시기 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
				spousal_Support();  // 매분기 첫달이면서 1일
			else if (내조효과_발동시기 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
=======
			if (发动时期 == 0)
				spousal_Support();  // 매턴
			else if (发动时期 == 1 and (pk::get_day() == 1))
				spousal_Support();  // 매월 1일
			else if (发动时期 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
				spousal_Support();  // 매분기 첫달이면서 1일
			else if (发动时期 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				spousal_Support();  // 매년 1월 1일
		}

		void spousal_Support()
		{
<<<<<<< HEAD

			auto list = pk::list_to_array(pk::get_unit_list());

			for (int i = 0; i < int(list.length); i++)
			{

				pk::unit@ src = list[i];

				pk::person@ src_leader = pk::get_person(src.leader);
				pk::person@ src_deputy1 = pk::get_person(src.member[1]);
				pk::person@ src_deputy2 = pk::get_person(src.member[2]);

				int deputy1 = src.member[1];
				int deputy2 = src.member[2];

				bool fuufu_exp01 = false;
				bool fuufu_exp02 = false;
				bool fuufu_exp03 = false;
				bool fuufu_exp04 = false;

				if (ch::has_skill(src, 특기_내조) and pk::is_alive(src_deputy1) and !pk::is_alive(src_deputy2))
				{
					// 주장, 부장1이 부부인 경우  
					if (pk::is_fuufu(src_leader, deputy1))
						fuufu_exp01 = true;
				}
				else if (ch::has_skill(src, 특기_내조) and pk::is_alive(src_deputy1) and pk::is_alive(src_deputy2))
				{
					// 주장, 부장1이 부부인 경우  
					if (pk::is_fuufu(src_leader, deputy1))
						fuufu_exp02 = true;

					// 주장, 부장2가 부부인 경우  
					else if (pk::is_fuufu(src_leader, deputy2))
						fuufu_exp03 = true;

					// 부장1, 부장2가 부부인 경우  
					else if (pk::is_fuufu(src_deputy1, deputy2))
						fuufu_exp04 = true;
				}

				// 능력 경험치 상승
				if (fuufu_exp01)
				{
					src_leader.stat_exp[0] = src_leader.stat_exp[0] + exp_0; // 통솔
					src_leader.stat_exp[1] = src_leader.stat_exp[1] + exp_1; // 무력
					src_leader.stat_exp[2] = src_leader.stat_exp[2] + exp_2; // 지력
					src_leader.stat_exp[3] = src_leader.stat_exp[3] + exp_3; // 정치
					src_leader.stat_exp[4] = src_leader.stat_exp[4] + exp_4; // 매력

					src_deputy1.stat_exp[0] = src_deputy1.stat_exp[0] + exp_0; // 통솔
					src_deputy1.stat_exp[1] = src_deputy1.stat_exp[1] + exp_1; // 무력
					src_deputy1.stat_exp[2] = src_deputy1.stat_exp[2] + exp_2; // 지력
					src_deputy1.stat_exp[3] = src_deputy1.stat_exp[3] + exp_3; // 정치
					src_deputy1.stat_exp[4] = src_deputy1.stat_exp[4] + exp_4; // 매력

					src_leader.update();
					src_deputy1.update();

					string src_leader_name = pk::decode(pk::get_name(src_leader));
					string src_deputy1_name = pk::decode(pk::get_name(src_deputy1));
					int force_id = src_leader.get_force_id();
					pk::force@ force_ = pk::get_force(force_id);
					pk::history_log(force_id, force_.color, pk::encode(pk::format("因特技内助，\x1b[1x{}\x1b[0x与\x1b[2x{}\x1b[0x的能力经验上升了。", src_leader_name, src_deputy1_name)));

					//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的经验上升了.", src_leader_name)));
					//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的经验上升了.", src_deputy1_name)));

				}

				if (fuufu_exp02)
				{
					src_leader.stat_exp[0] = src_leader.stat_exp[0] + exp_0; // 통솔
					src_leader.stat_exp[1] = src_leader.stat_exp[1] + exp_1; // 무력
					src_leader.stat_exp[2] = src_leader.stat_exp[2] + exp_2; // 지력
					src_leader.stat_exp[3] = src_leader.stat_exp[3] + exp_3; // 정치
					src_leader.stat_exp[4] = src_leader.stat_exp[4] + exp_4; // 매력

					src_deputy1.stat_exp[0] = src_deputy1.stat_exp[0] + exp_0; // 통솔
					src_deputy1.stat_exp[1] = src_deputy1.stat_exp[1] + exp_1; // 무력
					src_deputy1.stat_exp[2] = src_deputy1.stat_exp[2] + exp_2; // 지력
					src_deputy1.stat_exp[3] = src_deputy1.stat_exp[3] + exp_3; // 정치
					src_deputy1.stat_exp[4] = src_deputy1.stat_exp[4] + exp_4; // 매력

					src_leader.update();
					src_deputy1.update();

					string src_leader_name = pk::decode(pk::get_name(src_leader));
					string src_deputy1_name = pk::decode(pk::get_name(src_deputy1));
					int force_id = src_leader.get_force_id();
					pk::force@ force_ = pk::get_force(force_id);
					pk::history_log(force_id, force_.color, pk::encode(pk::format("因特技内助，\x1b[1x{}\x1b[0x与\x1b[2x{}\x1b[0x的能力经验上升了。", src_leader_name, src_deputy1_name)));
					//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的能力经验上升了.", src_leader_name)));
					//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的能力经验上升了.", src_deputy1_name)));

				}

				if (fuufu_exp03)
				{
					src_leader.stat_exp[0] = src_leader.stat_exp[0] + exp_0; // 통솔
					src_leader.stat_exp[1] = src_leader.stat_exp[1] + exp_1; // 무력
					src_leader.stat_exp[2] = src_leader.stat_exp[2] + exp_2; // 지력
					src_leader.stat_exp[3] = src_leader.stat_exp[3] + exp_3; // 정치
					src_leader.stat_exp[4] = src_leader.stat_exp[4] + exp_4; // 매력

					src_deputy2.stat_exp[0] = src_deputy2.stat_exp[0] + exp_0; // 통솔
					src_deputy2.stat_exp[1] = src_deputy2.stat_exp[1] + exp_1; // 무력
					src_deputy2.stat_exp[2] = src_deputy2.stat_exp[2] + exp_2; // 지력
					src_deputy2.stat_exp[3] = src_deputy2.stat_exp[3] + exp_3; // 정치
					src_deputy2.stat_exp[4] = src_deputy2.stat_exp[4] + exp_4; // 매력

					src_leader.update();
					src_deputy2.update();

					string src_leader_name = pk::decode(pk::get_name(src_leader));
					string src_deputy2_name = pk::decode(pk::get_name(src_deputy2));
					int force_id = src_leader.get_force_id();
					pk::force@ force_ = pk::get_force(force_id);
					pk::history_log(force_id, force_.color, pk::encode(pk::format("因特技内助，\x1b[1x{}\x1b[0x与\x1b[2x{}\x1b[0x的能力经验上升了。", src_leader_name, src_deputy2_name)));
					//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的能力经验上升了.", src_leader_name)));
					//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的能力经验上升了.", src_deputy2_name)));

				}

				if (fuufu_exp04)
				{
					src_deputy1.stat_exp[0] = src_deputy1.stat_exp[0] + exp_0; // 통솔
					src_deputy1.stat_exp[1] = src_deputy1.stat_exp[1] + exp_1; // 무력
					src_deputy1.stat_exp[2] = src_deputy1.stat_exp[2] + exp_2; // 지력
					src_deputy1.stat_exp[3] = src_deputy1.stat_exp[3] + exp_3; // 정치
					src_deputy1.stat_exp[4] = src_deputy1.stat_exp[4] + exp_4; // 매력

					src_deputy2.stat_exp[0] = src_deputy2.stat_exp[0] + exp_0; // 통솔
					src_deputy2.stat_exp[1] = src_deputy2.stat_exp[1] + exp_1; // 무력
					src_deputy2.stat_exp[2] = src_deputy2.stat_exp[2] + exp_2; // 지력
					src_deputy2.stat_exp[3] = src_deputy2.stat_exp[3] + exp_3; // 정치
					src_deputy2.stat_exp[4] = src_deputy2.stat_exp[4] + exp_4; // 매력

					src_deputy1.update();
					src_deputy2.update();

					string src_deputy1_name = pk::decode(pk::get_name(src_deputy1));
					string src_deputy2_name = pk::decode(pk::get_name(src_deputy2));

					int force_id = src_deputy1.get_force_id();
					pk::force@ force_ = pk::get_force(force_id);
					pk::history_log(force_id, force_.color, pk::encode(pk::format("因特技内助，\x1b[1x{}\x1b[0x与\x1b[2x{}\x1b[0x的能力经验上升了。", src_deputy1, src_deputy2_name)));
					//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的能力经验上升了.", src_deputy1_name)));
					//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x的能力经验上升了.", src_deputy2_name)));

				}

			}


		} // callback
=======
			for (int i = 0; i < 部队_末; ++i)
			{
				pk::unit@ src = pk::get_unit(i);
				if (pk::is_alive(src))
				{
					pk::list<pk::person@> list = ch::get_member_list(src);
					if (list.count < 2) continue;
					array<bool> has_skill_check = { false,false,false };
					//array<bool> exp_bonus_check = { false,false,false };
					for (int j = 0; j < list.count; ++j)
					{
						has_skill_check[j] = ch::has_skill(list[j], 特技_内助);
					}
					if (list.count == 2)
					{
						if (!pk::is_fuufu(list[0], list[1].get_id())) continue;
						if (!has_skill_check[0] and !has_skill_check[1]) continue;
						add_list_stat_exp(list);
					} 
					if (list.count == 3)
					{
						pk::list<pk::person@> exp_list;
						if (pk::is_fuufu(list[0], list[1].get_id()) and (has_skill_check[0] or has_skill_check[1]))
						{
							exp_list.add(list[0]);
							exp_list.add(list[1]);
							add_list_stat_exp(exp_list);
							exp_list.clear();
						}
						if (pk::is_fuufu(list[0], list[2].get_id()) and (has_skill_check[0] or has_skill_check[2]))
						{
							exp_list.add(list[0]);
							exp_list.add(list[2]);
							add_list_stat_exp(exp_list);
							exp_list.clear();
						}
						if (pk::is_fuufu(list[1], list[2].get_id()) and (has_skill_check[1] or has_skill_check[2]))
						{
							exp_list.add(list[1]);
							exp_list.add(list[2]);
							add_list_stat_exp(exp_list);
							exp_list.clear();
						}
					}
					//
				}
			}
		}

		void add_list_stat_exp(pk::list<pk::person@> list)
		{
			for (int i = 0; i < list.count; ++i)
			{
				for (int j = 0; j < 武将能力_末; ++j)
				{
					pk::add_stat_exp(list[i],j,2);
					list[i].update();
				}
			}

			string src_deputy1_name = pk::decode(pk::get_name(list[0]));
			string src_deputy2_name = pk::decode(pk::get_name(list[1]));

			int force_id = list[0].get_force_id();
			pk::force@ force = pk::get_force(force_id);
			pk::history_log(force_id, force.color, pk::encode(pk::format("因特技内助，\x1b[1x{}\x1b[0x与\x1b[2x{}\x1b[0x的能力经验上升了。", src_deputy1_name, src_deputy2_name)));
		}
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

	} // class Main

	Main main;

} // namespace
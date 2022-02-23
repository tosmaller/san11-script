// ## 2021/09/26 # 江东新风 # 无势力时不可用 ##
// ## 2021/02/17 # 江东新风 # 更改复活的设定，原来的复活方式默认隐藏 ##
// ## 2020/11/06 # 江东新风 # 复活时去除伤病状态 ##
// ## 2020/07/31 # messi # 汉化 ##

/*
*** Notice
본 스크립트에 대한 재배포, 무단수정을 금지 합니다.
개선을 빙자한 잘라먹기, 덮어씌우기 방식의 별도 패치 역시 거부 합니다.
필요사항 경우 쪽지나 메일을 통해 요청을 주시면 검토 후 반영하겠습니다.

*** 제작
	제르나 -	삼국지 공방		(https://cafe.naver.com/rtkbank)
			삼국지 도원결의	(https://cafe.naver.com/sam10)

*** 환경
	삼국지11 PK - PK2.1

*** History ***
--------------------------------------------------------------------
		작 업 자			| 수정 일자	|		작 업 내 용
--------------------------------------------------------------------
	제르나	- 공방		| 18.11.11	| 초기생성
	제르나	- 공방		| 18.11.12	| v1.1
									| 메뉴명 변경
									| 빈 슬롯 확인 조건 추가
	제르나	- 공방		| 18.11.14	| v1.2
									| 모든 사망무장 소생 기능 추가
									| 임의 선택기능의 선별 회수 증가
	제르나	- 공방		| 18.11.24	| v2.0
									| 수동선별 선택 방식 변경 (선택지 > 무장선택)
	제르나	- 공방		| 18.11.24	| v2.1
									| 전체/임의 선별 내부처리 개편
	기마책사	- 도원결의		| 18.11.28	| v2.2
									| 사망무장 중 소생유효무장 필터링 추가
	제르나	- 공방		| 18.12.11	| v2.3
									| 전체/임의 선별 내부처리 개편
									| 수동 선별 복수처리 기능 추가
	제르나	- 공방		| 19.11.11	| v3.0
									| 자동화 기능 추가
	제르나	- 공방		| 20.03.23	| v3.1
									| 배우자 유지/초기화 설정 추가
*/

namespace 复活
{
	class Main
	{
		int		use_auto = 0;		// 부활 자동화(주기)
												// 0: 미사용,	1: 매월,	2: 계절
		bool	use_auto_message = false;	// 자동화 메세지
												// true: 보임, false: 숨김
		int		use_menu = 1;		// 사용자 메뉴
												// 0: 미사용,	1: 사용
		bool	use_keep_spouse = false;	// 배우자 유지/초기화
												// true: 유지, false: 초기화
		bool	admin_mode = false;//将此处改完true，则使用原来的复活设定

		Main()
		{
			pk::bind(108, pk::trigger108_t(t108_NewMonth));
			pk::bind(109, pk::trigger109_t(t109_NewSeason));
			func_AddMenu();
		}

		// ↓↓↓↓↓↓↓↓ 부활 자동기능 ↓↓↓↓↓↓↓↓
		void t108_NewMonth()
		{
			if (use_auto == 1)
				func_AutoRevival();
		}

		void t109_NewSeason()
		{
			if (use_auto == 2)
				func_AutoRevival();
		}

		void func_AutoRevival()
		{
			pk::person@ target_;
			pk::person@ NPC = pk::get_person(무장_남두);
			array<pk::person@>	arr_person = pk::list_to_array(func_ListOfDead());

			if (int(arr_person.length) > 0)
			{
				@target_ = arr_person[pk::rand(arr_person.length)];
				func_Revival(target_);
				if (use_auto_message)
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人请随我来", pk::decode(pk::get_name(target_)))), pk::get_person(무장_화타));
			}
		}

		// ↑↑↑↑↑↑↑↑ 부활 자동기능 ↑↑↑↑↑↑↑↑

		// ↓↓↓↓↓↓↓↓ 부활	수동메뉴 ↓↓↓↓↓↓↓↓
		void func_AddMenu()
		{
			pk::menu_item item;
			item.menu = 2;
			item.is_visible = pk::menu_item_is_visible_t(Menu_Visible);
			item.get_text = cast<pk::menu_item_get_text_t@>(function() { return pk::encode("复活武将"); });
			item.get_desc = cast<pk::menu_item_get_desc_t@>(function() { return pk::encode("复活死亡的武将."); });
			item.handler = pk::menu_item_handler_t(Menu_handler);
			pk::add_menu_item(item);
		}

		bool Menu_Visible()
		{
			if (admin_mode)
			{
				if (use_menu == 1)
					return true;
				else
					return false;
			}
			else
			{
				if (pk::get_current_turn_force_id() == -1) return false;
				else if (pk::get_force(pk::get_current_turn_force_id()).player == -1)  return false;
				else return true;
			}

		}

		bool Menu_handler()
		{
			string target_name;
			pk::person@ target_;
			pk::person@ NPC = pk::get_person(무장_남두);

			if (admin_mode)
			{
				int mode = 0;

				//선별 모드
				array<string> btn_mode =
				{
					pk::encode("全部复活"),
					pk::encode("随机筛选"),
					pk::encode("手动筛选"),
					pk::encode("取消")
				};

				mode = pk::choose(pk::encode("请选择武将筛选的方式."), btn_mode);

				pk::list<pk::person@>	list_dead = func_ListOfDead();
				array<pk::person@>		arr_person;
				arr_person = pk::list_to_array(list_dead);

				if (arr_person.length <= 0)
				{
					pk::message_box(pk::encode("当前没有武将死亡..."), NPC);
					return false;
				}

				if (mode == 0)
				{
					for (int i = 0; i < int(arr_person.length); i++)
					{
						@target_ = arr_person[i];
						target_name = pk::decode(pk::get_name(target_));

						func_Revival(target_);
					}

					if (arr_person.length == 1)
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,请稍作休息随我来", target_name)), NPC);
					else
						pk::message_box(pk::encode(pk::format("{}名武将被我复活了.", arr_person.length)), NPC);

					return true;
				}
				else if (mode == 1)
				{
					@target_ = arr_person[pk::rand(arr_person.length)];
					target_name = pk::decode(pk::get_name(target_));

					func_Revival(target_);
					pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,请稍作休息随我来", target_name)), NPC);
					return true;
				}
				else if (mode == 2)
				{
					pk::message_box(pk::encode("原来如此,阁下要复活谁?"), NPC);

					array<pk::person@> arr_select;
					arr_select = pk::list_to_array(pk::person_selector(pk::encode("选择武将"), pk::encode("选择目标武将"), list_dead, 1, arr_person.length)); // 리스트 변경 (기마책사, '18.11.28)
					if (arr_select.length == 0) return false;

					for (int i = 0; i < int(arr_select.length); i++)
					{
						@target_ = arr_select[i];
						target_name = pk::decode(pk::get_name(target_));
						func_Revival(target_);
					}

					if (arr_select.length == 1)
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人,请稍作休息随我来", target_name)), NPC);
					else
						pk::message_box(pk::encode(pk::format("{}名武将被我复活了.", arr_select.length)), NPC);

					return true;


				}
			}
			else
			{
				pk::person@kunshu0 = pk::get_person(pk::get_force(pk::get_current_turn_force_id()).kunshu);
				string talent;
				int max = -1;
				int t = -1;
				for (int i = 0; i < 武将能力_末; i++)
				{
					if (int(kunshu0.stat[i]) > max and kunshu0.stat[i] >= 70)
					{
						max = kunshu0.stat[i];
						t = i;
					}
				}
				switch (t)
				{
				case 武将能力_统率: talent = "统率"; break;
				case 武将能力_武力: talent = "武力"; break;
				case 武将能力_智力: talent = "智力"; break;
				case 武将能力_政治: talent = "政治"; break;
				case 武将能力_魅力: talent = "魅力"; break;
				}
				//确定寿元
				int shouyuan = -1;
				if (pk::get_scenario().no == 7 or pk::get_scenario().no == 13) shouyuan = -1;
				else shouyuan = int(kunshu0.death - pk::get_year());
				if (t == -1 and shouyuan < 6)
				{
					pk::message_box(pk::encode("愚蠢的凡人呐...\n复活武将的代价不是你能承受的起的！\n回去吧！"), NPC);
					return false;
				}
				int mode;
				//무장 수동 선택
				pk::message_box(pk::encode("愚蠢的凡人呐,就是汝想要复活武将吗?"), NPC);
				//pk::message_box(pk::encode("生死皆有定数。起死回生要付出的代价可能远超你的想象！\n即便如此，你也要坚持吗？"), NPC);
				array<string> btn_mode =
				{
					pk::encode("任何后果都能承受"),
					pk::encode("容我再回去再想想"),
				};
				mode = pk::choose(btn_mode, pk::encode("生死皆有定数。起死回生要付出的\x1b[29x代价可能远超你的想象\x1b[0x！\n即便如此，你也要坚持吗？"), NPC);
				if (mode == 1) return false;
				else
				{
					pk::list<pk::person@>	list_dead = func_ListOfDead();
					array<pk::person@>		arr_person;
					arr_person = pk::list_to_array(list_dead);
					pk::message_box(pk::encode("好，既然你的意志如此坚决，我便帮你这一次！"), NPC);
					//251剧本，还有女流剧本，无寿命，可以考虑属性永久降低，将最强的能力减去5点
					pk::message_box(pk::encode("现在，便告诉我你要复活的武将吧。"), NPC);
					array<pk::person@> arr_select;
					arr_select = pk::list_to_array(pk::person_selector(pk::encode("选择武将"), pk::encode("选择要复活的武将"), list_dead, 1, 1)); //
					if (arr_select.length == 0) return false;
					@target_ = arr_select[0];
					target_name = pk::decode(pk::get_name(target_));
					func_Revival(target_);
					if (pk::get_scenario().no == 7 or pk::get_scenario().no == 13)
					{
						func_能力变化(kunshu0, target_name, t);
					}
					else
					{
						if (t > -1 and shouyuan > 5)
						{
							if (pk::rand_bool(50))
							{
								func_寿元变化(kunshu0, target_name);
							}
							else
							{
								func_能力变化(kunshu0, target_name, t);
							}
						}
						else if (t < 0)
						{
							func_寿元变化(kunshu0, target_name);
						}
						else func_能力变化(kunshu0, target_name, t);						
						
					}
					


					return true;
				}

			}


			return false;
		}

		void func_寿元变化(pk::person@ actor, string target_name)
		{
			//pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x已被我复活。\n这抽取你的\x1b[2x5年寿元\x1b[0x作为代价吧。", target_name)), pk::get_person(무장_남두));
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x已被我复活。\n不过这茫茫人海，能否找到他就看你的造化了。\n你这便去吧。", target_name)), pk::get_person(무장_남두));
			pk::message_box(pk::encode(pk::format("是错觉嘛。\n感觉好像突然苍老了好多...", target_name)), actor);
			actor.death -= 5;
			actor.update();
		}

		void func_能力变化(pk::person@ actor, string target_name, int ability)
		{
			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x已被我复活。\n不过这茫茫人海，能否找到他就看你的造化了。\n你这便去吧。", target_name)), pk::get_person(무장_남두));
			
			switch (ability)
			{
			case 武将能力_统率: pk::say(pk::encode(pk::format("是错觉嘛。\n怎么感觉思维没有那么清晰了...", target_name)), actor); break;
			case 武将能力_武力: pk::say(pk::encode(pk::format("是错觉嘛。\n怎么感觉体力差了一些...", target_name)), actor); break;
			case 武将能力_智力: pk::say(pk::encode(pk::format("是错觉嘛。\n怎么感觉脑袋没那么灵光了...", target_name)), actor); break;
			case 武将能力_政治: pk::say(pk::encode(pk::format("是错觉嘛。\n怎么感觉处理政务没那么顺手了...", target_name)), actor); break;
			case 武将能力_魅力: pk::say(pk::encode(pk::format("是错觉嘛。\n怎么感觉好像耐心差了一些...", target_name)), actor); break;
			}
			
			actor.base_stat[ability] = actor.base_stat[ability] - 5;
			actor.update();
		}

		// ↑↑↑↑↑↑↑↑ 부활 수동메뉴 ↑↑↑↑↑↑↑↑

		pk::list<pk::person@> func_ListOfDead()
		{
			array<pk::person@>		arr_dead_all = pk::list_to_array(pk::get_person_list(pk::mibun_flags(신분_사망)));
			pk::list<pk::person@>	list_dead;

			for (int i = 0; i < int(arr_dead_all.length); i++)
			{
				if (func_CheckNum(arr_dead_all[i].get_id())
					and func_CheckDeath(arr_dead_all[i])
					and pk::decode(pk::get_name(arr_dead_all[i])) != "")
					list_dead.add(arr_dead_all[i]); // 리스트 추가
			}

			return list_dead;
		}

		bool func_CheckNum(int number)
		{
			/*
				입력범위
				1.    0 ~  669(역사 무장)
				2.  800 ~  831(고대 무장)
				3.  850 ~  999(신무장)
			*/
			if ((number >= 0 and number <= 669)
				or (number >= 800 and number <= 831)
				or (number >= 850 and number <= 999))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool func_CheckDeath(pk::person@ target)
		{
			return target.mibun == 신분_사망;
		}

		void func_Revival(pk::person@ target)
		{
			int city = pk::rand(도시_끝);

			if (target.death <= pk::get_year() + 5)
			{
				target.death = pk::get_year() + 5 + pk::rand(6);
			}
			if (target.spouse > 0)
			{
				// 배우자
				pk::person@	spouse = pk::get_person(target.spouse);
				if (spouse.is_alive())
				{
					if (use_keep_spouse)
					{
						if (spouse.spouse != target.get_id())
							target.spouse = -1;
					}
					else
					{
						if (spouse.spouse == target.get_id())
						{
							spouse.spouse = -1;
							spouse.update();

						}
						target.spouse = -1;
					}
				}
				else
				{
					target.spouse = -1;
				}
			}
			target.mibun = 신분_재야;
			target.location = city;
			target.service = city;
			target.shoubyou = 伤病_健康;
			target.update();
		}
	}

	Main main;
}
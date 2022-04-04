// ## 2021/11/08 # 江东新风 # KRE报错问题 ##
// ## 2021/10/19 # 江东新风 # 修复选择时的空指针bug ##
// ## 2021/02/23 # 白马叔叔 # 君主妾室设为不可合离 ##
// ## 2020/09/21 # 江东新风 # ch::add_tp替换 ##
// ## 2020/08/09 # 氕氘氚 # 加入【君主】菜单 ##
// ## 2020/08/06 #配合仇視系统，如果处決武将，将会根据义理以一定?率将君主視?厌恶武将#
// ## 2020/07/30 #江东新风 # 修复离婚可以离两次bug，修复同时把君主当成亲爱武将和厌恶武将bug ##
// @@ 2019/02/21 @ 초기생성 @ @@
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
	제르나	- 공방		| 19.02.21	| 초기생성
*/

namespace 결별
{
	class Main
	{
		// ----- Setting -----
		bool					loyalty_target_use = true;	// 충성도 변화 사용 (대상)
		bool					loyalty_spouse_use = false;	// 충성도 변화 사용 (배우자)
		int						loyalty_target_rate = 5;	// 충성도 변화 계수 (대상)
		int						loyalty_spouse_rate = 5;	// 충성도 변화 계수 (배우자)
		bool					dislike_target_use = true;	// 혐오대상 등록 사용 (대상)
		bool					dislike_spouse_use = false;	// 혐오대상 등록 사용 (배우자)
		int						TP_COST = 500;				// 필요 기교 포인트 (기본값 : 500, 혼인과 동일)

		pk::building@			building_;
		pk::force@				force_;
		pk::person@				kunshu_;

		pk::list<pk::person@>	list_target_all;
		pk::person@	target_;
		pk::person@	spouse_;
		string		target_name;
		string		spouse_name;

		Main()
		{
			add_menu();
		}

		//----- 메뉴 설정 -----//
		void add_menu()
		{
			pk::menu_item item;
			item.menu = 105;
			item.pos = 5;
			item.shortcut = "6";
			item.init = pk::building_menu_item_init_t(init);
			item.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item.get_text = pk::menu_item_get_text_t(getText);
			item.get_desc = pk::menu_item_get_desc_t(getDesc);
			item.handler = pk::menu_item_handler_t(handler);
			pk::add_menu_item(item);
		}


		//----- 메뉴 설정 처리 -----//
		bool isvisible()
		{
			return true;
		}

		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building_.get_force_id());
			@kunshu_ = pk::get_person(force_.kunshu);
			
			pk::list<pk::person@> list_t = pk::get_person_list(force_, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
			list_target_all.clear();
			for (int i = 0; i < list_t.count; i++)
			{
				if (list_t[i].spouse == -1)
					continue;    //没有配偶的跳过
				if (list_t[i].spouse == kunshu_.get_id() && kunshu_.spouse != list_t[i].get_id())
					continue;    //君主的妾室跳过
				else
					list_target_all.add(list_t[i]);
			}
		}

		bool isEnabled()
		{
			//기교 포인트
			if (!check_condition(0))	return false;

			//무장 수
			if (!check_condition(1))	return false;

			return true;
		}

		string getText()
		{
			return pk::encode("和离");
		}

		string getDesc()
		{
			//기교 포인트
			if (!check_condition(0))
				return pk::encode(pk::format("技巧点不足.(必要技巧点:{})", TP_COST));

			//무장 수
			if (!check_condition(1))
				return pk::encode("没有要和离的目标武将.");

			return pk::encode(pk::format("请问是否要解除武将的夫妻关系吗.(需要技巧点:{})", TP_COST));
		}

		bool check_condition(int mode)
		{
			//行动力檢測
			if (mode == 0)
			{
				if (int(force_.tp) < TP_COST) return false;
			}
			//对象数量檢測
			if (mode == 1)
			{
				array<pk::person@> arr_person = pk::list_to_array(list_target_all);
				if (arr_person.length < 1) return false;
			}

			return true;
		}

		//----- 메뉴 동작 처리 -----//
		bool handler()
		{
			// - 무장 선택 -
			pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("选择武将"), pk::encode("请选择要和离的双方目标"), list_target_all, 1, 1);
			if (person_sel.count <= 0) return false;
			@target_ = @person_sel[0];
			@spouse_ = pk::get_person(target_.spouse);
			target_name = pk::decode(pk::get_name(target_));
			spouse_name = pk::decode(pk::get_name(spouse_));

			// - 무장 선택 확인 -
			bool act_confirm = false;
			act_confirm = pk::yes_no(pk::encode(pk::format("是否要解除\x1b[1x{}\x1b[0x与\x1b[1x{}\x1b[0x之间的夫妻关系?\n(技巧点{})", target_name, spouse_name, TP_COST)));
			if (!act_confirm) return false;

			// - Data 처리 -
			person_update();
			ch::add_tp(force_, -TP_COST, force_.get_pos());

			// - message 표시 -
			pk::scene(pk::scene_t(message));

			return true;
		}

		void person_update()
		{
			// 무장 충성도忠诚降低
			if (loyalty_target_use and !check_target_kunshu())
				target_.loyalty = pk::max(0, int(target_.loyalty - target_.giri * loyalty_target_rate));
			if (loyalty_spouse_use and !check_spouse_kunshu())
				spouse_.loyalty = pk::max(0, int(spouse_.loyalty - spouse_.giri * loyalty_spouse_rate));

			// 혐오 대상厌恶武将
			if (dislike_target_use and !check_target_kunshu())
			{
				if (pk::is_like(target_, kunshu_.get_id()))
					pk::remove_like(target_, kunshu_.get_id());

				pk::add_dislike(target_, kunshu_.get_id());
			}

			if (dislike_spouse_use and !check_spouse_kunshu())
			{
				if (pk::is_like(spouse_, kunshu_.get_id()))
					pk::remove_like(spouse_, kunshu_.get_id());

				pk::add_dislike(spouse_, kunshu_.get_id());
			}

			// 배우자
			if (target_.get_id() == spouse_.spouse)
			{
				spouse_.spouse = -1;
				target_.spouse = -1;

				target_.update();
				spouse_.update();
			}
		}


		void message()
		{
			string dialogue_01 = "";

			if (check_spouse_kunshu())
				dialogue_01 = "彼此";
			else if (target_.sex == 성별_남)
				dialogue_01 = "和妻子";
			else
				dialogue_01 = "和夫君";

			pk::fade(0);
			pk::sleep(100);
			pk::background(35);
			pk::play_se(24);
			pk::fade(255);

			// 군주
			if (check_target_kunshu())
			{
				pk::message_box(pk::encode(pk::format("我为了天下大义{}告别.\n今后我将专注于天下.", dialogue_01)), kunshu_);
				pk::message_box(pk::encode("..."), spouse_);
			}

			// 무장
			else
			{
				pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x,为了天下安定{}分别如何?", target_name, dialogue_01)), kunshu_);
				pk::message_box(pk::encode("如果是主公的命令...我也无可奈何."), target_);
			}

			pk::fade(0);
			pk::sleep(0);
			pk::background(-1);
			pk::fade(255);
		}

		bool check_target_kunshu()
		{
			if (target_.get_id() == kunshu_.get_id())
				return true;
			else
				return false;
		}

		bool check_spouse_kunshu()
		{
			if (spouse_.get_id() == kunshu_.get_id())
				return true;
			else
				return false;
		}
	}

	Main main;
}
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/10/27 # 氕氘氚 # 修复乱码问题 ##
// ## 2020/10/05 # 氕氘氚 # 对话中添加城市名称 ##
// ## 2020/09/21 # 江东新风 # 添加法治效果 ##
/*
@제작자: HoneyBee
@내용: 일정확률로 발생하는 적군의 계략으로 인한 아군 도시 피해
@기타: 난이도 상승 목적을 위해 제작한 SCRIPT

*/

namespace 적군_계략
{


	// ================ CUSTOMIZE ================

	const int 적군계략_발동시기 = 2;        // 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일,  4: 랜덤 턴
	const int 적군계략_발동확률_턴 = 5;      // 발동시기를 랜덤으로 설정했을 경우: 턴별 발동 확률
	const int 적군계략_발동확률 = 5;        // 0,1,2,3 설정일 경우 적군 계략 확률 (기본 5% 설정)

	const int whenToActivate = 12;    // 몇개월이 지난 시점부터 적군 계략이 발동할지 (기본 12개월 설정)

	// ===========================================


	class Main
	{

		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{
			if (적군계략_발동시기 == 0)
				enemy_Plan();  // 매턴
			else if (적군계략_발동시기 == 1 and (pk::get_day() == 1))
				enemy_Plan();  // 매월 1일
			else if (적군계략_발동시기 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
				enemy_Plan();  // 매분기 첫달이면서 1일
			else if (적군계략_발동시기 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
				enemy_Plan();  // 매년 1월 1일
			else if (적군계략_발동시기 == 4 and pk::rand_bool(적군계략_발동확률_턴))
				enemy_Plan();  // 랜덤 턴에서 발동
		}

		void enemy_Plan()
		{

			auto list = pk::list_to_array(pk::get_city_list());

			pk::person@ reporter_ = pk::get_person(무장_문관);

			int n = pk::rand(7);


			for (int i = 0; i < int(list.length); i++)
			{

				pk::city@ city_ = list[i];

				pk::building@ building_ = pk::city_to_building(city_);

				pk::force@ force_ = pk::get_force(building_.get_force_id());

				if (!building_.is_player())
					continue;

				if (pk::is_valid_person_id(building_.who_has_skill(특기_신산)) or pk::is_valid_person_id(building_.who_has_skill(특기_통찰)))
					continue;

				if (pk::get_elapsed_months() < whenToActivate) continue;


				if (pk::rand_bool(적군계략_발동확률))
				{

					// 도시 내구 피해
					if (n == 0)
					{
						pk::message_box(pk::encode(pk::format("主公, 有人破坏了\x1b[1x{}\x1b[0x的城墙!", pk::decode(city_.name))), reporter_);
						pk::add_hp(city_, -city_.max_hp / 3, true);
						pk::message_box(pk::encode("城墙的耐久下降了."));

					}

					// 도시 치안 피해
					if (n == 1)
					{
						pk::message_box(pk::encode(pk::format("主公, 有人在\x1b[1x{}\x1b[0x散播谣言!", pk::decode(city_.name))), reporter_);
						ch::add_public_order(city_, -city_.public_order / 3, true);
						pk::message_box(pk::encode("城市的治安下降了."));
					}

					// 도시 금 피해
					if (n == 2)
					{
						pk::message_box(pk::encode(pk::format("主公, 有人洗劫了\x1b[1x{}\x1b[0x的钱库!!", pk::decode(city_.name))), reporter_);
						pk::add_gold(city_, -city_.gold / 3, true);
						pk::message_box(pk::encode("城市的资金减少了."));
					}

					// 도시 병량 피해
					if (n == 3)
					{
						pk::message_box(pk::encode(pk::format("主公, 有人在\x1b[1x{}\x1b[0x的粮仓放火!", pk::decode(city_.name))), reporter_);
						pk::add_food(city_, -city_.food / 3, true);
						pk::message_box(pk::encode("城市的粮食减少了."));
					}

					// 도시 기력 피해
					if (n == 4)
					{
						pk::message_box(pk::encode(pk::format("主公, 有人在\x1b[1x{}\x1b[0x散播流言降低我军士气!", pk::decode(city_.name))), reporter_);
						pk::add_energy(city_, -city_.energy / 3, true);
						pk::message_box(pk::encode("城市驻军的士气下降了."));
					}

					// 도시 병장기 피해
					if (n == 5)
					{
						pk::message_box(pk::encode(pk::format("主公, 有人烧毁了\x1b[1x{}\x1b[0x的军械库!", pk::decode(city_.name))), reporter_);
						pk::add_weapon_amount(city_, 兵器_枪, -city_.weapon_amount[1] / 3, true);
						pk::add_weapon_amount(city_, 兵器_戟, -city_.weapon_amount[2] / 3, true);
						pk::add_weapon_amount(city_, 兵器_弩, -city_.weapon_amount[3] / 3, true);
						pk::message_box(pk::encode("城市的兵装减少了."));
					}

					// 도시 군마 피해
					if (n == 6)
					{
						pk::message_box(pk::encode(pk::format("主公, 有人在\x1b[1x{}\x1b[0x带着一群战马逃了!", pk::decode(city_.name))), reporter_);
						pk::add_weapon_amount(city_, 兵器_战马, -city_.weapon_amount[4] / 3, true);
						pk::message_box(pk::encode("城市的战马减少了."));
					}


				} // if


			} // for


		} // void enemy_Plan()



	} // class 

	Main main;
}
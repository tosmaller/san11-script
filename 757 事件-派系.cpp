// ## 2020/12/12 # 江东新风 # 修复Signed/Unsigned mismatch ##
// ## 2020/09/21 # 江东新风 # 添加法治效果 ##
// ##2020/08/20 # messi # 全面优化語句,調低触发条件，平衡浮动?加入了派系支持者增加忠诚的设定 ##
// ##2020/08/01 # messi # ?整?? ##
/*
@제작자: HoneyBee
@설명: 파벌 이벤트
@기타: GAME START 하고서 1년이 경과 后, 일정확률로 파벌 이벤트 발생 및 무장들의 충성도 변화 (의리, 야망 TYPE을 기준으로 무장들의 충성도 변화)

*/

namespace 파벌_이벤트
{

	// ================ CUSTOMIZE ================

	const bool 기존_발동 = true;      // Turn을 종료하면 이벤트 발동 (기존_발동 true일 경우, 계절_발동 false로 설정)
	const bool 계절_발동 = false;       // 새계절이 되는 시기에 이벤트 발동 (계절_발동 true일 경우, 기존_발동 false로 설정)

	const int chance_ = 5;  		// 파벌 이벤트 발생 확률 (기본 5% 설정)
	const int city_count = 6;   	// 도시 보유 수 6개 미만 세력에게는 파벌 이벤트가 발생하지 않음
	const int 보호기간 = 24;           // 게임 start 후 몇 개월간 파벌 이벤트 금지

// ===========================================


	class Main
	{
		Main()
		{
			if (기존_발동)
			{
				pk::bind(112, pk::trigger112_t(callback_0));
			}

			if (계절_발동)
			{
				pk::bind(109, pk::trigger109_t(callback_1));
			}
		}

		pk::force@ force_;
		pk::person@ kunshu_;
		pk::person@ gunshi_;
		pk::city@ city_;


		void callback_0(pk::force@ force)
		{

			if (force.is_player())
			{
				@force_ = force;
				@kunshu_ = pk::get_person(force.kunshu);
				@gunshi_ = pk::get_person(force.gunshi);
				@city_ = pk::building_to_city(pk::get_building(kunshu_.service));

				if (pk::get_elapsed_months() < 보호기간) return;

				if (city_ is null) return;

				if (pk::get_city_count(pk::get_force(city_.get_force_id())) < city_count) return;

				if (pk::rand_bool(chance_))

					pk::scene(pk::scene_t(scene));
			}

		} // callback()


		void callback_1()
		{

			auto force_list = pk::list_to_array(pk::get_force_list());

			for (int k = 0; k < int(force_list.length); k++)
			{
				pk::force@ force_ = force_list[k];

				if (force_.is_player())
				{

					@kunshu_ = pk::get_person(force_.kunshu);
					@gunshi_ = pk::get_person(force_.gunshi);
					@city_ = pk::building_to_city(pk::get_building(kunshu_.service));

					if (int(pk::get_elapsed_months()) < 보호기간) return;

					if (city_ is null) return;

					if (pk::get_city_count(pk::get_force(city_.get_force_id())) < city_count) return;

					if (pk::rand_bool(chance_))

						pk::scene(pk::scene_t(scene));
				}

			}

		}

		void scene()
		{

			pk::move_screen(city_.get_pos());
			pk::fade(0);
			pk::sleep();
			pk::background(2);
			pk::fade(255);

			pk::person@ reporter = pk::is_alive(gunshi_) ? gunshi_ : pk::get_person(무장_문관);
			string kunshu_name = pk::decode(pk::get_name(kunshu_));

			pk::message_box(pk::encode("主公,最近有传闻说臣子之间出现了派系活动."), reporter);
			pk::message_box(pk::encode("哎..到底是因为什么原因产生了派系呢?"), kunshu_);
			pk::message_box(pk::encode("那是.."), reporter);

			int num = pk::rand(4);

			if (num == 0)
			{

				pk::message_box(pk::encode("听说是关于势力方针的内容出现了分歧.主公可趁这个机会确定一下有关势力方针的内容."), reporter);
				pk::message_box(pk::encode("的确!是时候让大臣们知道我们的势力该如何前进了."), kunshu_);

				array<string> answers =
				{
					pk::encode("稳定内治"),
					pk::encode("对外征服")
				};

				int n = pk::choose(pk::encode("请选择今后的势力方针."), answers);


				if (n == 0) // 내치안정		
				{

					pk::message_box(pk::encode("我认为目前暂时稳定内治是最重要的."), kunshu_);
					pk::message_box(pk::encode("是,那么我把主公的话传达给臣子们."), reporter);
					pk::message_box(pk::encode(pk::format("关于今后势力方针的决定,不同的臣子们对\x1b[1x{}\x1b[0x大人表达了支持和不满.", kunshu_name)));

					auto city_list = pk::list_to_array(pk::get_city_list());

					for (int i = 0; i < int(city_list.length); i++)
					{
						pk::city@ city_ = city_list[i];

						pk::building@ building_ = pk::city_to_building(city_);

						if (!building_.is_player())
							continue;

						pk::person@ taishu = pk::get_person(pk::get_taishu_id(building_));


						if (pk::is_alive(taishu))
						{

							auto mibun_list = pk::list_to_array(pk::get_person_list(building_, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般)));

							if (0 < mibun_list.length)
							{
								for (int j = 0; j < int(mibun_list.length); j++)
								{

									pk::person@ person_0 = mibun_list[j];

									// 야망 매우높음/높음 충성도 변화 
									if (person_0.ambition == 야망_매우높음 or person_0.ambition == 야망_높음)
									{
										pk::add_loyalty(person_0, -10);
									}

									// 야망 보통 충성도 변화 
									else if (person_0.ambition == 야망_보통)
									{
										pk::add_loyalty(person_0, 0);
									}

									// 야망 매우낮음/낮음 충성도 변화 
									else if (person_0.ambition == 야망_낮음 or person_0.ambition == 야망_매우낮음)
									{
										pk::add_loyalty(person_0, 10);
									}


								} // for

							} // if 

						} // if	

					} // for 

					pk::message_box(pk::encode("由于对势力今后发展的倾向不同而产生了分歧和忠诚的浮动."));
				}


				else if (n == 1) // 세력정복		
				{

					pk::message_box(pk::encode("现在正是扩张领土征服势力的时期."), kunshu_);
					pk::message_box(pk::encode("是,那么我把主公的话传达给臣子们."), reporter);
					pk::message_box(pk::encode(pk::format("关于今后势力方针的决定,不同的臣子们对\x1b[1x{}\x1b[0x大人表达了支持和不满.", kunshu_name)));

					auto city_list = pk::list_to_array(pk::get_city_list());

					for (int i = 0; i < int(city_list.length); i++)
					{
						pk::city@ city_ = city_list[i];

						pk::building@ building_ = pk::city_to_building(city_);

						if (!building_.is_player())
							continue;

						pk::person@ taishu = pk::get_person(pk::get_taishu_id(building_));

						if (pk::is_alive(taishu))
						{

							auto mibun_list = pk::list_to_array(pk::get_person_list(building_, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般)));

							if (0 < mibun_list.length)
							{
								for (int j = 0; j < int(mibun_list.length); j++)
								{
									pk::person@ person_0 = mibun_list[j];

									// 야망 매우높음/높음 충성도 변화 
									if (person_0.ambition == 야망_매우높음 or person_0.ambition == 야망_높음)
									{
										pk::add_loyalty(person_0, 10);
									}

									// 야망 보통 충성도 변화 
									else if (person_0.ambition == 야망_보통)
									{
										pk::add_loyalty(person_0, 0);
									}

									// 야망 매우낮음/낮음 충성도 변화 
									else if (person_0.ambition == 야망_낮음 or person_0.ambition == 야망_매우낮음)
									{
										pk::add_loyalty(person_0, -10);
									}


								} // for

							} // if 	

						} // if	

					} // for 

					pk::message_box(pk::encode("由于对势力今后发展的倾向不同而产生了分歧和忠诚的浮动."));

				}


			} // num 0								


			if (num == 1)
			{

				pk::message_box(pk::encode("之前地方爆发了叛乱,对于参与地方叛乱的百姓将要如何处置?"), reporter);
				pk::message_box(pk::encode("你是说上次讨伐镇压叛乱,然后将支持叛乱的百姓抓起来的那件事吧.."), kunshu_);

				array<string> answers =
				{
					pk::encode("释放俘虏"),
					pk::encode("斩首俘虏")
				};

				int n = pk::choose(pk::encode("请选择俘虏待遇."), answers);


				if (n == 0) // 포로해방		
				{

					pk::message_box(pk::encode("这些人虽然支持叛乱,但是却是因为天下不太平才会这样的,我之过也,就放了他们吧."), kunshu_);
					pk::message_box(pk::encode("是,那么我把主公的话传达给臣子们."), reporter);
					pk::message_box(pk::encode(pk::format("关于对俘虏待遇问题措施,不同的臣子们对\x1b[1x{}\x1b[0x大人表达了支持和不满.", kunshu_name)));

					auto city_list = pk::list_to_array(pk::get_city_list());

					for (int i = 0; i < int(city_list.length); i++)
					{
						pk::city@ city_ = city_list[i];

						pk::building@ building_ = pk::city_to_building(city_);

						if (!building_.is_player())
							continue;

						pk::person@ taishu = pk::get_person(pk::get_taishu_id(building_));

						if (pk::is_alive(taishu))
						{

							auto mibun_list = pk::list_to_array(pk::get_person_list(building_, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般)));

							if (0 < mibun_list.length)
							{
								for (int j = 0; j < int(mibun_list.length); j++)
								{
									pk::person@ person_0 = mibun_list[j];

									// 의리 매우높음/높음 충성도 변화
									if (person_0.giri == 의리_매우높음 or person_0.giri == 의리_높음)
									{
										pk::add_loyalty(person_0, 10);
									}

									// 의리 보통 충성도 변화 
									else if (person_0.giri == 의리_보통)
									{
										pk::add_loyalty(person_0, 0);
									}

									// 의리 매우낮음/낮음 충성도 변화
									else if (person_0.giri == 의리_낮음 or person_0.giri == 의리_매우낮음)
									{
										pk::add_loyalty(person_0, -10);
									}


								} // for

							} // if


						} // if	

					} // for 

					pk::message_box(pk::encode("根据对参与叛乱的百姓待遇问题上产生了分歧和忠诚的浮动."));

				}


				else if (n == 1) // 포로참수			
				{

					pk::message_box(pk::encode("为了维护威严,稳定政权,就决定杀一儆百吧."), kunshu_);
					pk::message_box(pk::encode("是,那么我把主公的话传达给臣子们."), reporter);
					pk::message_box(pk::encode(pk::format("关于对俘虏待遇问题措施,不同的臣子们对\x1b[1x{}\x1b[0x大人表达了支持和不满.", kunshu_name)));

					auto city_list = pk::list_to_array(pk::get_city_list());

					for (int i = 0; i < int(city_list.length); i++)
					{
						pk::city@ city_ = city_list[i];

						pk::building@ building_ = pk::city_to_building(city_);

						if (!building_.is_player())
							continue;

						pk::person@ taishu = pk::get_person(pk::get_taishu_id(building_));

						if (pk::is_alive(taishu))
						{

							auto mibun_list = pk::list_to_array(pk::get_person_list(building_, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般)));

							if (0 < mibun_list.length)
							{
								for (int j = 0; j < int(mibun_list.length); j++)
								{
									pk::person@ person_0 = mibun_list[j];

									// 의리 매우높음/높음 충성도 변화
									if (person_0.giri == 의리_매우높음 or person_0.giri == 의리_높음)
									{
										pk::add_loyalty(person_0, -10);
									}

									// 의리 보통 충성도 변화 
									else if (person_0.giri == 의리_보통)
									{
										pk::add_loyalty(person_0, 0);
									}

									// 의리 매우낮음/낮음 충성도 변화 
									else if (person_0.giri == 의리_낮음 or person_0.giri == 의리_매우낮음)
									{
										pk::add_loyalty(person_0, 10);
									}


								} // for

							} // if


						} // if	

					} // for 

					pk::message_box(pk::encode("根据对参与叛乱的百姓待遇问题上产生了分歧和忠诚的浮动."));

				}

			} // num 1	

			if (num == 2)
			{

				pk::message_box(pk::encode("有传闻说平时主张汉室复兴的人想肃清反对派.我认为主公对这件事认真对待为妙."), reporter);
				pk::message_box(pk::encode("嗯..听上去这是一个相当严重的情况."), kunshu_);

				array<string> answers =
				{
					pk::encode("确认传闻真伪"),
					pk::encode("认为其是谣言")
				};

				int n = pk::choose(pk::encode("是否要相信这个传闻呢?"), answers);


				if (n == 0) // 소문 진위 파악
				{

					pk::message_box(pk::encode("不能放过在内部制造纷争的人.马上把他们全部找出来罢职."), kunshu_);
					pk::message_box(pk::encode("是,那么就按照主公的命令执行."), reporter);
					pk::message_box(pk::encode(pk::format("在内部制造纷乱的臣子们被\x1b[1x{}\x1b[0x大人所驱逐.", kunshu_name)));
					pk::message_box(pk::encode("反对派表达了忠心!另外,支持被驱逐大臣们的人开始产生了不满情绪.."));

					auto city_list = pk::list_to_array(pk::get_city_list());

					for (int i = 0; i < int(city_list.length); i++)
					{
						pk::city@ city_ = city_list[i];

						pk::building@ building_ = pk::city_to_building(city_);

						if (!building_.is_player())
							continue;

						pk::person@ taishu = pk::get_person(pk::get_taishu_id(building_));

						if (pk::is_alive(taishu))
						{

							auto mibun_list = pk::list_to_array(pk::get_person_list(building_, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般)));

							if (0 < mibun_list.length)
							{
								for (int j = 0; j < int(mibun_list.length); j++)
								{
									pk::person@ person_0 = mibun_list[j];

									// 야망 매우높음/높음 충성도 변화 
									if (person_0.ambition == 야망_매우높음 or person_0.ambition == 야망_높음)
									{
										pk::add_loyalty(person_0, 10);
									}

									// 야망 보통 충성도 변화
									else if (person_0.ambition == 야망_보통)
									{
										pk::add_loyalty(person_0, 0);
									}

									// 야망 매우낮음/낮음 충성도 변화 
									else if (person_0.ambition == 야망_낮음 or person_0.ambition == 야망_매우낮음)
									{
										pk::add_loyalty(person_0, -10);
									}


								} // for

							} // if


						} // if	

					} // for 

					pk::message_box(pk::encode("根据对驱逐大臣这件事情产生了分歧和忠诚的浮动."));

				}

				else if (n == 1) // 헛소문으로 결론		
				{

					pk::message_box(pk::encode("既然没有特别的物证,也就不必受传闻的左右了吧."), kunshu_);
					pk::message_box(pk::encode("好的,这是明智的判断."), reporter);
					pk::message_box(pk::encode(pk::format("主张汉室复兴的臣子们对\x1b[1x{}\x1b[0x表示忠心,反对派表示不满.", kunshu_name)));

					auto city_list = pk::list_to_array(pk::get_city_list());

					for (int i = 0; i < int(city_list.length); i++)
					{
						pk::city@ city_ = city_list[i];

						pk::building@ building_ = pk::city_to_building(city_);

						if (!building_.is_player())
							continue;

						pk::person@ taishu = pk::get_person(pk::get_taishu_id(building_));

						if (pk::is_alive(taishu))
						{

							auto mibun_list = pk::list_to_array(pk::get_person_list(building_, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般)));

							if (0 < mibun_list.length)
							{
								for (int j = 0; j < int(mibun_list.length); j++)
								{
									pk::person@ person_0 = mibun_list[j];

									// 야망 매우높음/높음 충성도 변화 
									if (person_0.ambition == 야망_매우높음 or person_0.ambition == 야망_높음)
									{
										pk::add_loyalty(person_0, -10);
									}

									// 야망 보통 충성도 변화 
									else if (person_0.ambition == 야망_보통)
									{
										pk::add_loyalty(person_0, 0);
									}

									// 야망 매우낮음/낮음 충성도 변화 
									else if (person_0.ambition == 야망_낮음 or person_0.ambition == 야망_매우낮음)
									{
										pk::add_loyalty(person_0, 10);
									}


								} // for

							} // if


						} // if	

					} // for 

					pk::message_box(pk::encode("根据对处理传闻这件事情产生了分歧和忠诚的浮动."));

				}


			} // num 2	

			if (num == 3)
			{

				pk::message_box(pk::encode("听闻最近百姓生活窘迫,就是否返还这段时间征收的税金的意见出现了分歧.主公应对此事予以确认."), reporter);
				pk::message_box(pk::encode("不错,最近因为战争和盗贼的掠夺,生活困难的百姓也变多了.."), kunshu_);

				array<string> answers =
				{
					pk::encode("退还百姓的税金"),
					pk::encode("拒绝还税于民")
				};

				int n = pk::choose(pk::encode("是否要将税金还给百姓?"), answers);


				if (n == 0) // 세금을 백성들에게 돌려준다.		
				{

					pk::message_box(pk::encode("就将税金还给百姓吧"), kunshu_);
					pk::message_box(pk::encode("是,您果然是位贤明的君主.那我就把主公的话转告大臣们."), reporter);
					pk::message_box(pk::encode(pk::format("对此,收到此事通知的大臣们对\x1b[1x{}\x1b[0x表达了忠心和不满.", kunshu_name)));

					auto city_list = pk::list_to_array(pk::get_city_list());

					for (int i = 0; i < int(city_list.length); i++)
					{
						pk::city@ city_ = city_list[i];

						pk::building@ building_ = pk::city_to_building(city_);

						if (!building_.is_player())
							continue;

						pk::person@ taishu = pk::get_person(pk::get_taishu_id(building_));

						if (pk::is_alive(taishu))
						{

							auto mibun_list = pk::list_to_array(pk::get_person_list(building_, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般)));

							if (0 < mibun_list.length)
							{
								for (int j = 0; j < int(mibun_list.length); j++)
								{
									pk::person@ person_0 = mibun_list[j];

									// 의리 매우높음/높음 충성도 변화 
									if (person_0.giri == 의리_매우높음 or person_0.giri == 의리_높음)
									{
										pk::add_loyalty(person_0, 10);
									}

									// 의리 보통 충성도 변화 
									else if (person_0.giri == 의리_보통)
									{
										pk::add_loyalty(person_0, 0);
									}

									// 의리 매우낮음/낮음 충성도 변화 
									else if (person_0.giri == 의리_낮음 or person_0.giri == 의리_매우낮음)
									{
										pk::add_loyalty(person_0, -10);
									}


								} // for

							} // if


						} // if	

					} // for 

					pk::message_box(pk::encode("根据是否还税于民这件事产生了分歧和忠诚的浮动."));
					pk::message_box(pk::encode("城市的资金减少了一半."));
					pk::add_gold(city_, -city_.gold / 2, true);

				}

				else if (n == 1) // 세금을 백성들에게 돌려주지 않는다.			
				{

					pk::message_box(pk::encode("目前正处于战争期间,没有多余的钱财来赈济百姓啊."), kunshu_);
					pk::message_box(pk::encode("好的,那我去把主公的话转告大臣们."), reporter);
					pk::message_box(pk::encode(pk::format("大臣们收到此消息后,表达了对\x1b[1x{}\x1b[0x大人的支持和不满.", kunshu_name)));

					auto city_list = pk::list_to_array(pk::get_city_list());

					for (int i = 0; i < int(city_list.length); i++)
					{
						pk::city@ city_ = city_list[i];

						pk::building@ building_ = pk::city_to_building(city_);

						if (!building_.is_player())
							continue;

						pk::person@ taishu = pk::get_person(pk::get_taishu_id(building_));

						if (pk::is_alive(taishu))
						{

							auto mibun_list = pk::list_to_array(pk::get_person_list(building_, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般)));

							if (0 < mibun_list.length)
							{
								for (int j = 0; j < int(mibun_list.length); j++)
								{
									pk::person@ person_0 = mibun_list[j];

									// 의리 매우높음/높음 충성도 변화 (태수)
									if (person_0.giri == 의리_매우높음 or person_0.giri == 의리_높음)
									{
										pk::add_loyalty(person_0, -10);
									}

									// 의리 보통 충성도 변화 (태수)
									else if (person_0.giri == 의리_보통)
									{
										pk::add_loyalty(person_0, 0);
									}

									// 의리 매우낮음/낮음 충성도 변화 (태수)
									else if (person_0.giri == 의리_낮음 or person_0.giri == 의리_매우낮음)
									{
										pk::add_loyalty(person_0, 10);
									}


								} // for

							} // if


						} // if	

					} // for 

					pk::message_box(pk::encode("根据是否还税于民这件事产生了分歧和忠诚的浮动."));
					pk::message_box(pk::encode("城市的治安降低了."));
					ch::add_public_order(city_, -20, true);

				}


			} // num 3


			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);


		} // void scene()


	} // class Main

	Main main;
}
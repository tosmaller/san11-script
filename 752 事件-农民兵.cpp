// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2020/08/13 #messi#修正語句##
// ## 2020/08/03 # 氕氘氚 # 添加常量 ##
namespace 농민병
{
	// ----------------------------------------------------------------------------------------------
	const int 发生率 = 30;
	const int 兵力条件 = 10000;
	const int 治安条件 = 80;
	const int 兵力增加_min = 1000;
	const int 兵力增加_max = 6000;
	const int 治安增加_min = 10;
	const int 治安增加_max = 20;

	// ----------------------------------------------------------------------------------------------


	class Main
	{

		Main()
		{
			pk::bind(112, pk::trigger112_t(callback));
		}

		pk::force@ force_;
		pk::person@ kunshu_;
		pk::city@ city_;

		void callback(pk::force@ force)
		{
			if (force.is_player())
			{
				@force_ = force;
				@kunshu_ = pk::get_person(force.kunshu);
				@city_ = pk::building_to_city(pk::get_building(kunshu_.service));

				if (city_ is null) return;
				if (city_.troops >= 兵力条件) return;
				if (city_.public_order >= 治安条件) return;

				if (pk::rand_bool(发生率))

					pk::scene(pk::scene_t(scene));
			}
		}

		void scene()
		{
			pk::move_screen(city_.get_pos());
			pk::fade(0);
			pk::sleep();
			pk::background(2);
			pk::fade(255);

			string kunshu_name = pk::decode(pk::get_name(kunshu_));
			pk::person@ peasant = pk::get_person(무장_농민);

			switch (pk::rand(2))
			{
			case 0: pk::message_box(pk::encode(pk::format("我们被盗贼所威胁.\x1b[1x{}\x1b[0x大人请收下我们.", kunshu_name)), peasant); break;
			case 1: pk::message_box(pk::encode(pk::format("最近生活很困难.请接受我们成为士兵吧.", kunshu_name)), peasant); break;
			}

			array<string> answers =
			{
				pk::encode("不将这些农民收为士兵."),
				pk::encode("将这些农民收为士兵."),
			};

			int n = pk::choose(pk::encode("是否将这些农民收为士兵?"), answers);

			int troops = 兵力增加_min + pk::rand(兵力增加_max - 兵力增加_min);
			int safety = 治安增加_min + pk::rand(治安增加_max - 治安增加_min);

			switch (n)
			{
			case 0:
				pk::message_box(pk::encode("没有必要非得让大家上战场.抓紧去消灭盗贼吧."), kunshu_);

				switch (pk::rand(2))
				{
				case 0: pk::message_box(pk::encode(pk::format("是..\x1b[1x{}\x1b[0x大人,我明白你的意思了.", kunshu_name)), peasant); break;
				case 1: pk::message_box(pk::encode(pk::format("那么\x1b[1x{}\x1b[0x大人,我们相信你.请尽快消灭盗贼吧.", kunshu_name)), peasant); break;
				}
				pk::message_box(pk::encode(pk::format("治安增加了\x1b[1x{}\x1b[0x.", safety)));
				pk::add_public_order(city_, safety, true);
				break;

			case 1:
				pk::message_box(pk::encode("谢谢各位.我会接受大家的诚意的."), kunshu_);

				switch (pk::rand(2))
				{
				case 0: pk::message_box(pk::encode(pk::format("为了\x1b[1x{}\x1b[0x大人,我们会献出生命的.", kunshu_name)), peasant); break;
				case 1: pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x大人果然是位贤名的君主.我们会尽忠的.", kunshu_name)), peasant); break;
				}
				pk::message_box(pk::encode(pk::format("兵力增加了 \x1b[1x{}\x1b[0x.", troops)));
				ch::add_troops(city_, troops, true);
				break;
			}

			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		}
	}

	Main main;
}
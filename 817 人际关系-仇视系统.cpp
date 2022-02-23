// ## 2021/10/07 # 江东新风 # 修复处决会仇视自己bug，同时武将同时和处决者及被处决者都有关系时，50%仇视 ##
// ## 2021/02/25 # 江东新风 # 处决系统改写 ##
// ## 2021/01/14 # 江东新风 # 修复颜色空指针错误 ##
// ## 2021/01/13 # 江东新风 # 修复势力空指针错误 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ##2020/08/01 # 江东新风 # 增加了是否存在好友的判断。 ##
// ##2020/08/01 # 江东新风 # 修复了继承者滿地图认仇，认仇添加几率=义理(0~4)*20+25。 ##
// @@2020/07/25 @ 像风一樣的存在(바람과같은존재) @ @@

namespace 仇视系统
{
	class Main
	{
		Main()
		{
			pk::bind(151, pk::trigger151_t(killperson));			
		}

		//存在的问题是tigger的触发是在死亡之前，这意味着，认仇的提示会在武将死亡的信息之前
		void killperson(pk::person@ self, pk::person@ by/*, pk::hexobject@ where, pk::person@ succ, bool sound*/, int type, int rettype)
		{
			//if (by !is null) pk::trace(pk::format("武将死亡trigger测试，死亡武将：{}，执行武将：{},死亡类型：{}，返回类型：{}", self.get_id(), by.get_id(), type, rettype));
			//else  pk::trace(pk::format("武将死亡trigger测试，死亡武将：{}，死亡类型：{}，返回类型：{}", self.get_id(), type, rettype));

			//分两种情况，一种是战场死亡，一种是处决
			//战场死亡仇视敌方部队武将，战场死亡有两种可能，一种是script设定死亡，一种是原游戏的骑兵突死
			//处决的话，直接仇视君主
			if ((rettype == 6 or rettype == 10) and by !is null)
			{
				for (int i = 0; i < 可用武将_末; i++)
				{
					pk::person@ friend = pk::get_person(i);
					if (pk::is_alive(friend) && i != self.get_id() && i != by.get_id() && isFriend(self, friend))
					{
						//分为执行者是其亲爱武将和不是，分别处理
						if (pk::is_like(friend, by.get_id())) remove_like(friend, by);
						else if (isFriend(friend, by))
						{
							if (pk::rand_bool(50))
							{
								set_dislike(friend, by);//如果目标武将和执行者及被处决者都有关系，则概率设定仇视
							}
						}
						else
						{
							set_dislike(friend, by);
						}
					}
				}
			}
			else if (type == 1 and by !is null)
			{
				for (int i = 0; i < 可用武将_末; i++)
				{
					pk::person@ friend = pk::get_person(i);
					if (pk::is_alive(friend) && i != self.get_id() && i != by.get_id() && isFriend(self, friend))
					{
						//分为执行者是其亲爱武将和不是，分别处理
						if (pk::is_like(friend, by.get_id())) remove_like(friend, by);
						else if (isFriend(friend, by))
						{
							if (pk::rand_bool(50)) set_dislike(friend, by);//如果目标武将和执行者及被处决者都有关系，则概率设定仇视
						}
						else set_dislike(friend, by);
					}
				}
			}

		}

		void set_dislike(pk::person@src, pk::person@dst)
		{
			if (true)//(pk::rand_bool(pk::min((src.giri * 20 + 25), 100)))
			{
				pk::add_dislike(src, dst.get_id());
				int color = -1;
				int force_id0 = src.get_force_id();
				if (pk::is_valid_force_id(force_id0)) color = pk::get_force(force_id0).color;

				pk::history_log(src.get_pos(), color, pk::encode(pk::format("\x1b[1x{}\x1b[0x将\x1b[2x{}\x1b[0x视为仇人", pk::decode(pk::get_name(src)), pk::decode(pk::get_name(dst)))));

			};
		}

		void remove_like(pk::person@src, pk::person@dst)
		{
			if (pk::rand_bool(pk::min((src.giri * 20 + 25), 100)))
			{
				pk::remove_like(src, dst.get_id());
				int color = -1;
				int force_id0 = src.get_force_id();
				if (pk::is_valid_force_id(force_id0)) color = pk::get_force(force_id0).color;

				pk::history_log(src.get_pos(), color, pk::encode(pk::format("\x1b[1x{}\x1b[0x不再将\x1b[2x{}\x1b[0x视为朋友", pk::decode(pk::get_name(src)), pk::decode(pk::get_name(dst)))));

			};
		}


		//判断是否是义兄弟，夫妻，血缘
		bool isFriend(pk::person@ person1, pk::person@ person2)
		{
			bool result = false;

			if (pk::is_gikyoudai(person1, person2.get_id()))
				result = true;
			if (pk::is_fuufu(person1, person2.get_id()))
				result = true;
			if (pk::is_ketsuen(person1, person2.get_id()))
				result = true;
			if (pk::is_oyako(person1, person2.get_id()))
				result = true;

			return result;
		}


	}

	Main main;
}
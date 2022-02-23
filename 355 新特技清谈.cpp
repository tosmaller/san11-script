// ## 2021/10/01 #江东新风#仍有bug，关闭##
// ## 2021/08/21 #江东新风#重写清谈，测试无bug##

namespace sire特技_清谈
{
	class Main
	{
		Main()
		{
			pk::set_func(57,pk::func57_t(callback));
		}

		bool callback(pk::person@ target, pk::person@ actor)
		{
			//触发点在登用结果判定为失败之后，失败后跳转此处进行处理，如果此处返回值为true，会将登用结果改为true，也即是最终为登用成功
			if (ch::has_skill(actor, 特技_清谈))
			{
				return (func_清谈处理(target, actor));
			}
			return false;
		}

		bool func_清谈处理(pk::person@ target, pk::person@ actor)
		{
			if (actor.is_player())
			{
				pk::person@kunshu0 = pk::get_person(pk::get_kunshu_id(actor));
				pk::message_box(pk::encode(pk::format("很抱歉，对\x1b[1x{}\x1b[0x大人仕官一事，\n恕难从命......", pk::decode(pk::get_name(kunshu0)))), target);
				if (pk::yes_no(pk::encode(pk::format("要以舌战说服吗？\n挑战者 \x1b[1x{}\x1b[0x：智力：{}\n被挑战者\x1b[1x{}\x1b[0x：智力：{}", pk::decode(pk::get_name(actor)), actor.stat[武将能力_智力], pk::decode(pk::get_name(target)), target.stat[武将能力_智力]))))
				{
					pk::int_int_bool result = pk::debate(actor, target, pk::is_player_controlled(actor), false, true, true);
					//pk::trace(pk::format("first:{},second:{},third:{}",result.first, result.second, result.third));
					if (result.first == 0)
					{
						//pk::trace("win");
						return true;
					}
					else
					{
						//pk::trace("lose");
						return false;
					}
				}
				else
				{
					//pk::trace("no debate");
					return false;
					//pk::message_box(pk::encode(pk::format("真是非常抱歉，\n登用\x1b[1x{}\x1b[0x一事失败了......", pk::decode(pk::get_name(target)))), kunshu);
				}
			}
			else
			{
				int dif = actor.stat[武将能力_智力] - target.stat[武将能力_智力];
				if (dif >= 5) return true;
				else if (dif >= 0) return pk::rand_bool(50 + dif);
				else if (dif <= -5) return false;
				else return pk::rand_bool(50 + dif);
			}

		}
	}

	Main main;
}
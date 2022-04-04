// ## 2021/11/30 # 江东新风 ##

namespace 内政设施建造速度
{
	class Main
	{
		Main()
		{
			pk::set_func(62, pk::func62_t(callback));
		}

		int callback(const pk::detail::arrayptr<pk::person@>& in actors, int facility_id)
		{
			int all_politics = 0;
			uint8 best_politics = 0;
			for (int i = 0; i < 3; ++i)
			{
				pk::person@ actor = actors[i];
				if (pk::is_alive(actor))
				{
					all_politics += actor.stat[武将能力_政治];
					if (best_politics < actor.stat[武将能力_政治])
						best_politics = actor.stat[武将能力_政治];
				}
			}

			pk::facility@ facility = pk::get_facility(facility_id);
			int final_stat = all_politics / 2 + best_politics;
			int speed = pk::max(facility.hp/10, final_stat);
			if (ch::has_skill(actors, 特技_循吏)) speed *= 2;
			//pk::trace("speed:"+speed + "facility_id" + facility_id + "final_stat" + final_stat);
			return speed;
		}

	}

	Main main;
}
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2021/01/24 # 江东新风 # 将函数内容全搬运到305，以便其他cpp调用 ##
// ## 2020/10/23 #江东新风#同步马术书大神的更新，修复战役模式情况的nullptr错误##
// ## 2020/08/18 # 氕氘氚 # 添加法治影响 ##
// ## 2020/08/03 # messi # 征兵部件耐久公共版 ##
/*
@수정자 : 기마책사
@Update: '19.05.4   // 수정내용: 유저_도시수_패널티 추가
@Update: '20.8.29  // 수정내용: 캠페인에서는 커스텀 세팅 사용 불가하도록 수정
*/

namespace RECRUIT_PUBLIC_ORDER_CHANGE
{
	//---------------------------------------------------------------------------

	const bool 治安变化_城市数_修正 = true;   // 유저세력에 대해서 도시수에 비례하여 치안감소 패널티 (도시당 1% 증가)

	//---------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(101, pk::func101_t(callback));
		}

		int callback(pk::city@ city, const pk::detail::arrayptr<pk::person@>& in actors, int troops)
		{
			pk::list<pk::person@> actor_list;
			for (int i = 0; i < actors.length; i++)
			{
				actor_list.add(actors[i]);
			}
			
			return inter::get_recruit_order_dec(pk::city_to_building(city),actor_list,troops, 治安变化_城市数_修正);
			//以下内容均搬运到305全局函数内政，以方便其他cpp调用
			/*
			int n = 100;
			for (int i = 0; i < actors.length; i++)
			{
				pk::person@ actor = actors[i];

				if (pk::is_alive(actor))
					n = n + actor.stat[int(pk::core["recruit.stat"])];
			}
			n = -troops / n;




			// 유저_도시수_패널티 ('19.5.4)
			if (治安变化_城市数_修正 and city.is_player() and !pk::is_campaign())
			{
				pk::force@ force = pk::get_force(city.get_force_id());
				float force_city_count = float(pk::get_city_list(force).count);
				n = int(n * (1.f + (force_city_count * 0.01f)));
			}

			n = ch::get_order_change(city, n);
			return n;*/
		}
	}

	Main main;
}
// ## 2021/10/10 # 江东新风 # 人口相关设定 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2021/01/24 # 江东新风 # 将函数内容全搬运到305，以便其他cpp调用 ##
// ## 2020/10/23 #江东新风#同步马术书大神的更新，修复战役模式及势力无都督等情况的nullptr错误##
// ## 2020/08/24 # 氕氘氚 # 常量修改 ##
// ## 2020/08/16 #江东新风#has_skill函数替換##


/*
@수정자 : 기마책사
@Update: '18.10.14  // 수정내용: 세력의 보유 특기 또는 황군인 경우 징병량 버프
@Update: '19.04.26  // 수정내용: 대도시 징병량 버프 추가
@Update: '19.05.4   // 수정내용: 유저_도시수_패널티 추가
@Update: '20.8.25   // 수정내용: null point error 오류 수정
@Update: '20.8.29  // 수정내용: 캠페인에서는 커스텀 세팅 사용 불가하도록 수정
*/

namespace RECRUIT_TROOPS_CHANGE
{
	//---------------------------------------------------------------------------

	const bool 势力名声特技修正 = true;   // 군주,도독,태수가 명성 특기 보유 시 버프
	const float 名声特技_君主名声 = 1.10f;   // 군주 버프 계수
	const float 名声特技_都督名声 = 1.15f;  // 도독 버프 계수
	const float 名声特技_太守名声 = 1.20f;  // 태수 버프 계수	
	const bool 皇帝势力修正 = true;   // 후한 황제 옹립 세력 또는 군주가 황제인 경우 버프


	const bool 征兵数_城市数_修正 = true;   // 유저세력에 대해서 도시수에 비례하여 징병량 디버프 (도시당 1% 감소)

	//---------------------------------------------------------------------------

	class Main
	{
		Main()
		{
			pk::set_func(100, pk::func100_t(callback));
		}

		int callback(pk::building@ building, const pk::detail::arrayptr<pk::person@>& in actors,int type/*0执行，1仅计算，不影响兵役*/)
		{
			pk::list<pk::person@> actor_list;
			for (int i = 0; i < actors.length; i++)
			{
				actor_list.add(actors[i]);
			}
			//pk::trace(pk::format("征兵:类型：{}",type));
			//	int get_recruit_num(pk::building@ building, pk::list<pk::person@>actor_list, int near_city_id = - 1, bool 征兵数_城市数_修正 = false, bool 势力名声特技修正 = false, float 名声特技_君主名声 = 1.1f, float 名声特技_都督名声 = 1.15f, float 名声特技_太守名声 = 1.2f, bool 皇帝势力修正 = false)
			return inter::get_recruit_num(building, actor_list, type, 征兵数_城市数_修正, 势力名声特技修正, 名声特技_君主名声, 名声特技_都督名声, 名声特技_太守名声, 皇帝势力修正);
			//下面内容全搬运到305，以便其他cpp调用
			/*
			if (pk::is_alive(building) and actors[0] !is null)
			{
				pk::city@ city = pk::building_to_city(building);
				if (pk::is_alive(city))
				{
					int n = 0;
					int sum = 0;
					int mul = 100;
					for (int i = 0; i < actors.length; i++)
					{
						pk::person@ actor = actors[i];
						if (pk::is_alive(actor))
						{
							sum += actor.stat[int(pk::core["recruit.stat"])];
							// 명성 특기를 보유하고 있을 경우 50% 상승
							if (ch::has_skill(actor, int(pk::core["recruit.skill"])))
								mul += 名声征兵加成;
						}
					}
					n = int((1000 + (city.public_order + 20) * sum * 5 / 100) * mul / 100 * func_5c4600(city));
					// 특급 난이도 컴퓨터일 경우 2배
					if (pk::get_scenario().difficulty == 难易度_超级 and !city.is_player())
						n = n * 超级电脑征兵倍率 / 100;
					// 주변에 적 부대가 존재할 경우 반감
					if (pk::enemies_around(building))
						n = n * 周围敌部队征兵倍率 / 100;


					// -----------------------------------------------------------------------------

					// 세력
					pk::force@ force = pk::get_force(building.get_force_id());

					// 세력의 보유 특기 ('18.10.14)
					if (势力名声特技修正 and !pk::is_campaign())
					{
						// 君主持有名声군주가 특기_명성 보유 시 징병량 증가
						pk::person@ p1 = pk::get_person(pk::get_kunshu_id(building));
						if (pk::is_alive(p1))
						{
							if (ch::has_skill(p1, 특기_명성))
								n = int(n * 名声特技_君主名声);
						}
						// 都督持有名声도독이 특기_명성 보유 시 징병량 증가
						pk::person@ p2 = pk::get_person(pk::get_totoku_id(building));
						if (pk::is_alive(p2))
						{
							if (ch::has_skill(p2, 특기_명성))
								n = int(n * 名声特技_都督名声);
						}
						// 太守持有名声태수가 특기_명성 보유 시 징병량 증가
						pk::person@ p3 = pk::get_person(pk::get_taishu_id(building));
						if (pk::is_alive(p3))
						{
							if (ch::has_skill(p3, 특기_명성))
								n = int(n * 名声特技_太守名声);
						}
					}

					// 후한황제 보호세력 또는 군주가 황제인 세력 징병량 증가 ('18.10.14)
					if (皇帝势力修正 and !pk::is_campaign())
					{

						if (pk::is_protecting_the_emperor(force))
							n = int(n * 1.1f);
						else if (force.title == 爵位_皇帝)
							n = int(n * 1.1f);
					}

					// 대도시 징병량 버프 ('19.4.26)
					if (pk::is_large_city(city) and !pk::is_campaign())
						n = int(n * 大都市征兵倍率 / 100.f);
					else
					{
						n = int(n * 一般城市征兵倍率 / 100.f);
					}


					// 유저_도시수_패널티 ('19.5.4)
					if (征兵数_城市数_修正 and building.is_player() and !pk::is_campaign())
					{
						float force_city_count = float(pk::get_city_list(force).count);
						n = int(n * (1.f - (force_city_count * 0.01f)));
					}


					// -----------------------------------------------------------------------------

					return n;
				}
			}
			return 0;*/
		}
		/*
		float func_5c4600(pk::city@ city)
		{
			int level1 = 0, level2 = 0;
			for (int i = 0; i < int(city.max_devs); i++)
			{
				pk::building@ building = city.dev[i].building;
				if (pk::is_alive(building))
				{
					switch (building.facility)
					{
					case 시설_병영: building.completed ? level1++ : 0; break;
					case 시설_병영2단: building.completed ? level2++ : level1++; break;
					case 시설_병영3단: building.completed ? 0 : level2++; break;
					}
				}
			}
			if (int(city.barracks_counter) > level1 + level2)
				return 1.5f;
			if (int(city.barracks_counter) > level1)
				return 1.2f;
			return 1.f;
		}*/
	}

	Main main;
}
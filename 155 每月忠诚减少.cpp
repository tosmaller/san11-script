// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/09/21 #江东新风#修改仁政武将不在城市中，也能起效，与原版保持一致##
// ## 2020/08/19 # 江东新风 # 原版特技has_skill设定 ##
// ## 2020/07/26 ##
namespace LOYALTY_CHANGE
{
	//---------------------------------------------------------------------------------------
	const int 人心掌握下降机率 = 33;
	const int 忠诚下降上限 = 8;
	const int 忠诚下降基数 = 3;
	const int 符节台加成 = 2;
	const int 电脑治安下降倍率 = 50;
	const int 电脑忠诚下降倍率 = 50;
	//---------------------------------------------------------------------------------------
	class Main
	{
		Main()
		{
			pk::set_func(155, pk::func155_t(callback));
		}

		int callback(pk::building@ building, pk::person@ person)
		{
			if (person.mibun == 身份_君主)	//君主
				return 0;

			// 거점에 인정 특기 보유자가 있다면 일반 무장은 감소하지 않음
			if (person.mibun != 신분_포로 and ch::has_skill(building, 특기_인정, false))	//仁政
				return 0;
			//圣人所属据点武将忠诚不下降,和仁政分离了	
			if (person.mibun != 신분_포로 and ch::has_skill(building, 143, false))	//仁政
				return 0;

			int kunshu_id = pk::get_kunshu_id(person);
			pk::person@ kunshu = pk::get_person(kunshu_id);

			// 군주가 없다면 감소하지 않음(멸망한 세력의 포로)  灭亡势力?虏不掉忠
			if (!pk::is_alive(kunshu))
				return 0;

			// 일반 무장은 분기별 첫 달에만 감소  一般武将每季度掉忠
			if (person.mibun != 신분_포로 and !pk::is_first_month_of_quarter(pk::get_month()))
				return 0;

			// 군주를 친애하거나 부부, 의형제, 부모자식 관계일 경우 감소하지 않음	与君主亲爱、结婚、兄弟、血缘关系不掉忠
			if (pk::is_like(person, kunshu_id) or pk::is_fuufu(person, kunshu_id) or pk::is_gikyoudai(person, kunshu_id) or pk::is_oyako(person, kunshu_id))
				return 0;

			// 일반 무장이고 군주와의 궁합 25 이하(좋음)	与君主相性差小於25
			if (person.mibun != 신분_포로 and pk::get_aishou_distance(person, kunshu_id) <= 25)
			{
				// 의리가 보통 이상이거나 야망이 보통 이하	义理普通及以上，野心普通及以下
				if (person.giri >= 의리_보통 or person.ambition <= 야망_보통)
				{
					// 군주를 혐오하지 않으면 감소하지 않음	不厌恶君主
					if (!pk::is_dislike(person, kunshu_id))
						return 0;
				}
			}

			// 인심장악 기교가 연구된 경우 66% 확률로 감소하지 않음
			pk::force@ force = pk::get_force(person.get_force_id());
			if (pk::is_alive(force) and pk::has_tech(force, 기교_인심장악) and pk::rand_bool(100 - 人心掌握下降机率))
				return 0;

			int n = pk::rand(忠诚下降基数) * 2;
			pk::city@ city = pk::building_to_city(building);
			if (pk::is_alive(city) and pk::has_facility(city, 시설_부절태))
				n = n + 符节台加成;
			if (kunshu.giri == 의리_매우낮음 and kunshu.ambition == 야망_매우높음)	//君主义理极低and野心极高
				n = n + (의리_매우높음 - person.giri) / 2;


			//  电脑势力武将(非?虏)
			if (!person.is_player() and person.mibun != 신분_포로)
			{
				n = int(n * 电脑忠诚下降倍率 / 100.f);
			}

			n = pk::min(n, 忠诚下降上限);
			return -n;
		}
	}

	Main main;
}
// ## 2021/01/13 # 江东新风 # 修复空城叛乱错误 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ##2020/10/31 # 江东新风 # 同步honeybee大神的更新##
// ##2020/10/27 # 江东新风 # 新特技——内治##
// ##2020/08/19 # 江东新风 # 根据义理野心性格忠诚相性差綜合确定叛乱?率，总叛乱?率降低##
// ##2020/08/03 # 江东新风 # 解決单城叛乱bug, 增加判断所需基础城市数设定，太守君主所属相同时不叛乱 ##
// ##2020/07/31 # 氕氘氚 # 电脑可叛乱 ##
/*
@제작자: HoneyBee
@설명: 반란 이벤트
@기타: 군주와 상성이 차이가 크거나 충성도가 낮고 야망이 높은 태수가 일정확률로 반란

	 ===== 반란 조건 예시 =====
	 (1) 태수의 충성도가 95 미만 (필수)
	 (2) 태수와 군주의 상성차가 15보다 큰 경우 (조건)
	 (3) 태수의 야망이 높거나 매우 높은 경우 (조건)

	 [1,2]가 동시 충족 or
	 [1,3]이 동시 충족 or
	 [1,2,3]이 동시 충족될 경우 태수가 반란

	 (단, 태수와 군주가 [의형제, 부부, 친애무장]일 경우 위 조건이 충족되도 반란하지 않음)

*/

namespace 반란_이벤트
{

	// ================ CUSTOMIZE ================

	const int chance_ = 50;   // 반란 이벤트 발생 확률 (기본 50% 설정)
	const int loyalty_ = 90;  // 충성도 수치 미만일 경우 (기본 95 미만 설정)
	const int aishou_ = 30;   // 군주와 태수의 상성 차이 (기본 15보다 큰 경우 설정)
	const bool AI叛乱 = true;
	const int chance_AI = 30;  // AI叛乱机率
	const int 叛乱基础城市数 = 2; //低於此城市数不叛乱
	const array<int>  义理叛乱率 = { 10, 5, 0, -5, -10 };
	const array<int>  野心叛乱率 = { -10, -5, 0, 5, 10 };
	const array<int>  性格叛乱率 = { -10, -5, 5, 10 };
	/*
	   true일 경우 독립하는 무장과 같은 거점에 소속되어 있는 무장들만 배반하며,
	   false일 경우 기존 세력 전체에서 동조하는 무장들이 무작위로 배반합니다.
	*/
	const bool rise_scale = false;

	// ===========================================


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
			if (force.is_player() or AI叛乱)
			{
				@force_ = force;
				@kunshu_ = pk::get_person(force.kunshu);
				@city_ = pk::building_to_city(pk::get_building(kunshu_.service));

				if (city_ is null) return;

				if (pk::get_city_list(force).count <= 叛乱基础城市数) return;

				if ((force.is_player() and pk::rand_bool(chance_)) or (!force.is_player() and pk::rand_bool(chance_AI)))

					pk::scene(pk::scene_t(scene));
			}


		} // callback()


		void scene()
		{

			string kunshu_name = pk::decode(pk::get_name(kunshu_));
			int kunshu_id = pk::get_kunshu_id(kunshu_);

			auto city_list = pk::list_to_array(pk::get_city_list(force_));

			for (int i = 0; i < int(city_list.length); i++)
			{

				pk::city@ city_ = city_list[i];

				pk::building@ building_ = pk::city_to_building(city_);

				// if (!building_.is_player() and !AI叛乱)
				// 	continue;
				int taishu_id = pk::get_taishu_id(building_);
				if (!pk::is_valid_person_id(taishu_id)) continue;
				pk::person@ taishu = pk::get_person(taishu_id);
				//pk::trace(pk::format("叛乱判断：太守id：{}",taishu_id));
				string taishu_name = pk::decode(pk::get_name(taishu));

				if (pk::is_gikyoudai(taishu, kunshu_id) or pk::is_fuufu(taishu, kunshu_id) or pk::is_like(taishu, kunshu_id) or (taishu.service == kunshu_.service))
					continue;

				// if (pk::is_ketsuen(taishu, kunshu_id))
				// 	continue;

				if ((pk::is_alive(taishu) and taishu.loyalty < loyalty_ and taishu.mibun != 身份_君主) and (pk::get_aishou_distance(taishu, kunshu_id) > aishou_ or (taishu.ambition == 야망_매우높음) or (taishu.ambition == 야망_높음)))
				{
					int a = 义理叛乱率[taishu.giri] + 野心叛乱率[taishu.ambition] + 性格叛乱率[taishu.character] + (95 - taishu.loyalty) + (pk::get_aishou_distance(taishu, kunshu_.get_id()) / 5);
					if (ch::has_skill(force_, 特技_内治))
						a = int(a * 内治叛乱几率 / 100);
					if (pk::rand_bool(a))
					{
						pk::message_box(pk::encode(pk::format("不能再把\x1b[1x{}\x1b[0x这样的人作为主公了!", kunshu_name)), taishu);
						pk::hanran(taishu, rise_scale);
						pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x军独立了.", taishu_name)));
					}

				} // if

			} // for			

		} // void scene()


	} // class Main

	Main main;
}
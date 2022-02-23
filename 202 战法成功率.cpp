// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2020/10/29 #江东新风#同步马术书大神的更新##
// ## 2020/10/02 #氕氘氚# 适配KRE1.6.2适性属性自定义 ##
// ## 2020/08/16 #江东新风#has_skill函数替換##
// ## 2020/08/14 #江东新风#加入兵仙特技##
// ## 2020/07/26 ##
namespace TACTICS_CHANCE
{
	//---------------------------------------------------------------------------
	const int 战法成功率_突刺 = 70;
	const int 战法成功率_螺旋突 = 60;
	const int 战法成功率_二段突 = 65;
	const int 战法成功率_熊手 = 70;
	const int 战法成功率_橫扫 = 70;
	const int 战法成功率_旋风 = 65;
	const int 战法成功率_火矢 = 75;
	const int 战法成功率_貫箭 = 70;
	const int 战法成功率_乱射 = 65;
	const int 战法成功率_突击 = 70;
	const int 战法成功率_突破 = 60;
	const int 战法成功率_突进 = 65;
	const int 战法成功率_火箭 = 100;
	const int 战法成功率_破碎 = 100;
	const int 战法成功率_放射 = 100;
	const int 战法成功率_投石 = 100;
	const int 战法成功率_船火箭 = 100;
	const int 战法成功率_船猛撞 = 65;
	const int 战法成功率_船投石 = 100;

	const int 适性加成条件 = 1;  //0?C，依次增加
	const int 每级适性加成 = 5;
	const int 精妙增加成功率 = 10;
	const int 强运成功率下限 = 80;
	const int 洞察成功率上限 = 50;

	const int 战法成功率下限 = 10;
	const int 战法成功率上限 = 100;
	//---------------------------------------------------------------------------
	class Main
	{
		Main()
		{
			pk::set_func(202, pk::func202_t(callback));
		}

		int callback(pk::unit@ attacker, const pk::point& in pos, pk::hex_object@ target, int tactics_id)
		{
			pk::unit @ target_unit = pk::hex_object_to_unit(target);

			if (target.is_instance(pk::unit::type_id) and target_unit.status != 部队状态_通常)
				return 100;
			//兵仙判定
			if (attacker !is null and target_unit !is null)
			{
				if (ch::has_skill(attacker, 122))
				{
					if (ch::get_best_member_stat(attacker, 122, 武将能力_统率) > ch::get_best_member_stat(target_unit, 武将能力_统率))
						return 100;
				}
			}

			int n = 0;
			pk::point temp;

			// if (pk::get_tekisei(attacker) == 적성_A) n = 5; else if (pk::get_tekisei(attacker) == 적성_S) n = 10;
			n = int(pk::core["tekisei"][pk::get_tekisei(attacker)]["tactics_chance"]) + pk::core::tactics_chance(attacker.leader);
			// 新的适性加成公式  2020/7/17
			// int tekisei_ = pk::get_tekisei(attacker);
			// if (tekisei_ >= 适性加成条件)
			// 	n = int((tekisei_ - 适性加成条件) * 每级适性加成 + pk::core::tactics_chance(attacker.leader));

			switch (tactics_id)
			{
			case 전법_돌출:
				n = n + push_bonus(get_height_diff(attacker.get_pos(), target.get_pos())) + 战法成功率_突刺;
				break;
			case 전법_이단첨:
				n = n + push_bonus(get_height_diff(attacker.get_pos(), target.get_pos())) + 战法成功率_二段突;
				temp = pk::get_neighbor_pos(target.get_pos(), pk::get_direction(attacker.get_pos(), target.get_pos()));
				n = n + cutoff_diff(get_height_diff(target.get_pos(), temp));
				break;
			case 전법_나선첨:
				n = n + push_bonus(get_height_diff(attacker.get_pos(), target.get_pos())) + 战法成功率_螺旋突;
				break;

			case 전법_갈퀴:
				n = n + pull_bonus(get_height_diff(attacker.get_pos(), target.get_pos())) + 战法成功率_熊手;
				temp = pk::get_neighbor_pos(attacker.get_pos(), pk::get_direction(target.get_pos(), attacker.get_pos()));
				n = n + cutoff_diff(get_height_diff(attacker.get_pos(), temp));
				break;
			case 전법_횡치:
				n = n + 战法成功率_橫扫;
				break;
			case 전법_선풍:
				n = n + 战法成功率_旋风;
				break;

			case 전법_화시:
				n = n + hiya_terrain_bonus(pk::get_hex(target.get_pos()).terrain) + 战法成功率_火矢;
				break;
			case 전법_관시:
				n = n + 战法成功率_貫箭;
				break;
			case 전법_난사:
				n = n + 战法成功率_乱射;
				break;

			case 전법_돌격:
				n = n + push_bonus(get_height_diff(attacker.get_pos(), target.get_pos())) + 战法成功率_突击;
				break;
			case 전법_돌진:
				n = n + push_bonus(get_height_diff(attacker.get_pos(), target.get_pos())) + 战法成功率_突进;
				temp = pk::get_neighbor_pos(target.get_pos(), pk::get_direction(attacker.get_pos(), target.get_pos()));
				n = n + cutoff_diff(get_height_diff(target.get_pos(), temp));
				break;
			case 전법_돌파:
				n = n + push_bonus(get_height_diff(attacker.get_pos(), target.get_pos())) + 战法成功率_突破;
				break;

			case 전법_공성화시:	//火箭
				n = n + 战法成功率_火箭;
				break;
			case 전법_공성파쇄:	//破碎
				n = n + 战法成功率_破碎;
				break;
			case 전법_공성방사:	//放射
				n = n + 战法成功率_放射;
				break;
			case 전법_공성투석:	//投石
				n = n + 战法成功率_投石;
				break;

			case 전법_함선화시: //船火箭
				n = n + 战法成功率_船火箭;
				break;
			case 전법_함선격돌:	//船猛撞
				n = n + 战法成功率_船猛撞;
				break;
			case 전법_함선투석:	//船投石
				n = n + 战法成功率_船投石;
				break;

			default:
				return 100;
			}

			// 공격부대 병력수에 따라 전법 성공률 보정 ('18.10.13)
			if (attacker.troops <= 1000)
				n = int(n * pk::max(0.1f, attacker.troops / 1000.f));         // 소부대 시 전법성공률 1/10배 까지 감소


			// 공격/피격 부대의 특기 보유에 따른 전법 성공율 보정 ('18.10.18)
			if (target.is_instance(pk::unit::type_id) and target_unit.status == 部队状态_通常 and pk::is_valid_tactics_id(tactics_id))
			{
				// 공격부대가 강운 특기 보유 시 전법 성공율 최소 80% 
				if (ch::has_skill(attacker, 특기_강운))
					n = pk::max(强运成功率下限, n);
				// 피격부대가 통찰 특기 보유 시 전법 성공율 최대 50% : 공격전법 실패 시 통상공격이 되기 ?문에 불굴/금강처럼 일정 확률로 회피하는 것과는 다름 
				if (ch::has_skill(target_unit, 특기_통찰))
					n = pk::min(洞察成功率上限, n);
				// 공격부대가 정묘 특기 보유 시 전법 성공율 10% 추가
				if (ch::has_skill(attacker, 특기_정묘))
					n = n + 精妙增加成功率;
			}
			//兵神战法成功率设定
			if (ch::has_skill(target_unit, 121))
				n = pk::max(0, n - 50);

			n = pk::min(战法成功率上限, pk::max(战法成功率下限, n));

			return n;
		}





		// 5af5b0
		int get_height_diff(const pk::point& in src, const pk::point& in dst)
		{
			return pk::get_height_map(pk::hex_pos_to_height_map_pos(src) + 2).height - pk::get_height_map(pk::hex_pos_to_height_map_pos(dst) + 2).height;
		}

		// 5af6b0
		int push_bonus(int n)
		{
			return cutoff_diff(n) + 5;
		}

		// 5af6f0
		int pull_bonus(int n)
		{
			return 5 - cutoff_diff(n);
		}


		// 5af680
		int cutoff_diff(int n)
		{
			// 상대보다 높음
			if (n >= 10)
				return 10;
			// 상대보다 높음
			else if (n >= 5)
				return 5;
			// 평지
			else if (n >= -5)
				return 0;
			// 상대보다 낮음
			else if (n >= -10)
				return -5;
			// 상대보다 낮음
			else
				return -10;
		}


		// 849ae0 지형에 따른 화시 확률 보너스
		int hiya_terrain_bonus(int terrain)
		{
			switch (terrain)
			{
			case 지형_땅: return -5;
			case 지형_모래: return -5;
			case 지형_습지: return -15;
			case 지형_독천: return -15;
			case 지형_숲: return 10;
			case 지형_내: return -10;
			case 지형_강: return -10;
			case 지형_바다: return -10;
			case 지형_여울: return -10;
			case 지형_샛길: return -5;
			}
			return 0;
		}


	}

	Main main;
}
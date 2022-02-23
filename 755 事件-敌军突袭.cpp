// ## 2021/11/06 # 江东新风 # 尝试修复48714e,原因在于会生成无所属武将 ##
// ## 2021/01/09 # 江东新风 # 突袭重做，城市筛选优化 ##
// ## 2020/12/12 # 江东新风 # 修复Signed/Unsigned mismatch ##
// ## 2020/10/27 # 江东新风 # 新特技——戍防##
// ## 2020/09/30 # 氕氘氚 # 常量名简体化，难度调整 ##
// ## 2020/09/15 # 江东新风 # 突袭女武将名扩增至40种 ##
// ## 2020/08/05 # 氕氘氚 # 修复突袭部队被捕的bug ##
/*
@제작자: HoneyBee
@설명: 계절이 바뀔 때, 일정확률로 적군 기습부대 출현 (도시 근처에서 발생)
@기타: 난이도 상승 목적을 위해 제작한 SCRIPT

-----------------------------------------------------------------------
@수정자 : 기마책사
@수정일 : '18.11.10
@수정내용 : trigger 변경, 강대 적세력 검색함수 추가, 적군 병기 랜덤화
		 거점이 전투 중인지를 판단하여 적군 생성
		 발동 옵션 추가/변경
*/

namespace 事件_敌军突袭
{

	// ================ CUSTOMIZE ================

	const int 敌军奇袭_男武将部队数 = 2;		// 적군 男무장 부대 생성
	const int 敌军奇袭_女武将部队数 = 2;		// 적군 女무장 부대 생성
	const int min_stat = 60;        // 적무장의 최소 능력치를 설정 (기본 설정 - 60)
	const int max_stat = 85;        // 적무장의 최대 능력치를 설정 (기본 설정 - 85)

	const int 敌军奇袭_发动时期 = 1;        // 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일 默认 1
	const int 敌军奇袭_发动概率_基本 = 10;  // 基本的发动概率 默认20
	const int 敌军奇袭_发动概率_都市 = 20;    // 在发动的基础上，每座城市被袭击的概率30

	const int 敌军奇袭_保护月数 = 12;        // 게임 start 후 몇 개월 간 적군기습 발동 방지 默认12
	const int 敌军奇袭_最少城市 = 3;       // 설정 도시수 3개 이하 세력에게는 발동 안함 (도시 4개 ~ 6개 사이일 때 발생) 默认3
	const int 敌军奇袭_最多城市 = 7;       // 설정 도시수 7개 이상 세력에게는 발동 안함 (강제 종료 ERROR 방지를 위한 설정)

	const bool 敌军奇袭_仅对玩家 = true;   // true = 플레이어만 적용, false = AI도 적용
	const bool 敌军奇袭_特技保护 = true;    // true = 특기 적용, false = 특기 어드밴티지 없음

	const bool 自动姓名 = true;	         // 무장 이름을 자동으로 생성

	const int 敌军奇袭_最小距离 = 4;        // 적 부대 생성 위치
	const int 敌军奇袭_最大距离 = 7;        // 적 부대 생성 위치

	const int 兵力 = 8000;		 // 적 부대 병력 설정 (최대 14000 = 정동장군 관직과 동일)

	/*

		기습 무장 이름을 ARRAY로 미리 SETTING 하시기 바랍니다. (아래는 임시로 넣은 성명입니다.)
		아래 성명에서 RANDOM으로 출력하여 기습 무장의 이름이 정해집니다.

		EX) 조조, 조비, 조견, 유비, 유견, 유탁, 손견, 손탁, 손포 등..

	*/

	// 기습 男 무장 [성] (임시로 40개 설정)
	array<string> sei_0 = { "赵", "钱", "孙", "李", "郑", "王", "冯", "陈", "卫", "沈",
						   "韩", "杨", "朱", "秦", "许", "何", "吕", "施", "张", "孔",
						   "曹", "严", "华", "金", "魏", "陶", "姜", "戚", "支", "谢",
						   "邹", "水", "章", "云", "苏", "潘", "葛", "范", "彭", "鲁" };

	// 기습 男 무장 [이름] (임시로 40개로 설정)
	array<string> mei_0 = { "彬", "轩", "含", "蒲", "行", "亭", "伦", "蓝", "影", "韬",
						   "函", "克", "盛", "衡", "芝", "昊", "诗", "至", "涵", "时",
						   "鹰", "志", "永", "智", "嘉", "安", "世", "勇", "昂", "业",
						   "衡", "林", "言", "群", "书", "宣", "颁", "略", "沐", "风" };


	// EX) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 ... 39 } <- TOTAL 40개 (ARRAY에서 [성], [이름]을 늘릴 때마다 동일한 갯수로 설정하시면 됩니다.)
	int i = 0;   // 배열의 FIRST NUMBER 
	int j = 39;  // 배열의 LAST NUMBER


	// --------------------------------------------------------------------------------------------------------------


	// 기습 女 무장 [성] (임시로 50개 설정)
	array<string> sei_1 = { "赵", "钱", "孙", "李", "郑", "王", "冯", "陈", "卫", "沈",
						   "韩", "杨", "朱", "秦", "许", "何", "吕", "施", "张", "孔",
						   "曹", "严", "华", "金", "魏", "陶", "姜", "戚", "支", "谢",
						   "邹", "水", "章", "云", "苏", "潘", "葛", "范", "彭", "鲁",
						   "马", "苗", "花", "方", "任", "袁", "柳", "蔡", "乐", "上官" };

	// 기습 女 무장 [이름] (임시로 10개로 설정)
	array<string> mei_1 = { "双儿", "思", "慧", "环", "英", "静", "香", "宁", "秀", "婉儿",
						   "寻雁", "含烟", "语芙", "乐瑶", "碧瑶", "凌香", "谷丝", "南风", "秋白", "诗双" ,
						   "以山", "怀蝶", "冰兰", "元风", "怜云", "寒雁", "幻柏", "凡梦", "尔曼", "小萱" ,
						   "翠容", "春文", "平春", "秋灵", "白薇", "古雪", "水绿", "夜安", "寄凡", "千萍" };


	// EX) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 ... 49 } <- TOTAL 50개 (ARRAY에서 [성], [이름]을 늘릴 때마다 동일한 갯수로 설정하시면 됩니다.)
	int k = 0;   // 배열의 FIRST NUMBER [성]
	int l = 49;  // 배열의 LAST NUMBER  [성]

	int m = 39;  // 배열의 LAST NUMBER [이름]


// ===========================================


	class Main
	{
		int dst_force_id;
		int src_city_id;


		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}


		pk::random random(pk::rand());


		void callback()
		{

			if (敌军奇袭_发动时期 == 0)
				district_Set();  // 매턴
			else if (敌军奇袭_发动时期 == 1 and (pk::get_day() == 1))
				district_Set();  // 매월 1일
			else if (敌军奇袭_发动时期 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()))
				district_Set();  // 매분기 첫달이면서 1일
			else if (敌军奇袭_发动时期 == 3 and (pk::get_day() == 1) and pk::get_month() == 1)
				district_Set();  // 매년 1월 1일

		}

		//--------------------------------------------------------------------------------
		pk::int_int fun_获取AI_进攻及请求_城市_突袭(pk::force@ force)
		{
			pk::int_int result;
			result.first = -1;
			result.second = -1;
			array<pk::int_int> 目标数组();
			目标数组 = array<pk::int_int>();
			pk::list<pk::city@> List;
			pk::list<pk::city@> List0 = pk::get_city_list(force);
			if (List0.count < 1) return result;
			pk::list<pk::city@> List1 = pk::get_city_list();
			//打乱顺序，防止每次都相同的城市
			List1.shuffle();
			int n = 0;
			//按兵力和武将数排序
			List0.sort(function(a, b)
			{
				if (a.troops != a.troops)
					return a.troops < a.troops;
				pk::list<pk::person@> list_1 = pk::get_person_list(a.get_id(), pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
				pk::list<pk::person@> list_2 = pk::get_person_list(b.get_id(), pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
				if (list_1.count != list_2.count)
					return list_1.count < list_2.count;
				return a.get_id() < b.get_id(); // 先按兵力倒序，再按武将数排序，再按id排序
			});

			//获取兵力数量末位的城市，然后满足请求条件的城市取前三个
			for (int i = 0; i < List0.count; i++)
			{
				pk::city @ target_city = List0[i];
				pk::building@building0 = pk::city_to_building(target_city);
				if (ch::has_skill(building0, 特技_戍防)) continue;

				for (int j = 0; j < List1.count; j++)
				{
					pk::city @ ask_city = List1[j];
					int target_city_id = target_city.get_id();
					int ask_city_id = ask_city.get_id();
					//pk::trace(pk::format("ai提出请求城市id：{}, 目标城市id：{}",  ask_city.get_id(), target_city.get_id() ));

					if (ask_city.get_force_id() != target_city.get_force_id())
					{
						//pk::trace(pk::format("势力不同", target_city.get_id()));
						//pk::trace(pk::format("城市距离：{}, 是否敌人：{}", pk::get_building_distance(ask_city_id, target_city_id, target_city.get_force_id()) , pk::is_enemy(target_city, ask_city)));						
						if (pk::get_building_distance(ask_city_id, target_city_id, target_city.get_force_id()) <= 3 and pk::is_enemy(target_city, ask_city) and !pk::enemies_around(building0))
						{
							目标数组.insertLast(pk::int_int(ask_city_id, target_city_id));
							//目标数组[n+1].first = target_city_id;
							//目标数组[n+1].second = ask_city_id;
							n = n + 1;
							//pk::trace(pk::format("ai提出请求城市id：{}, 目标城市id：{},符合条件", ask_city_id, target_city_id));
							if (n > 2)
								break;
						}
					}
				}
			}
			if (目标数组.length > 0)
				result = 目标数组[pk::rand(目标数组.length - 1)];
			return result;
		}
		// 최강 적세력 검색 함수 추가 by 기마책사 ('18.11.10)
		int EnemyForceSel(pk::building @ building)
		{
			int force_sel = 0;
			int force_city_cnt = 0;

			auto force_list = pk::list_to_array(pk::get_force_list());
			for (int k = 0; k < int(force_list.length); k++)
			{

				if (building.is_player() and pk::is_enemy(force_list[k], building) and force_city_cnt < pk::get_city_count(force_list[k]))
				{

					force_city_cnt = pk::get_city_count(force_list[k]);
					force_sel = force_list[k].get_force_id();

				}

			}

			return force_sel;
		}
		//--------------------------------------------------------------------------------


		void district_Set()
		{
			if (!pk::rand_bool(敌军奇袭_发动概率_基本)) return;
			if (getEmptyPerson() is null) return;
			if (pk::get_elapsed_months() < 敌军奇袭_保护月数) return;
			//pk::trace(pk::format("城市id：{}，势力城市数量：{}", i, pk::get_city_count(pk::get_force(building_.get_force_id()))));

			for (int i = 0; i < 非贼势力_末; i++)
			{
				pk::force@force0 = pk::get_force(i);
				if (!pk::is_alive(force0))continue;
				if (敌军奇袭_仅对玩家 and !force0.is_player()) continue;
				if (敌军奇袭_最少城市 >= pk::get_city_count(force0)) continue;
				//if (敌军奇袭_最多城市 <= pk::get_city_count(force0) ) continue;
				if (pk::rand_bool(敌军奇袭_发动概率_都市))
				{
					pk::int_int dst_src = fun_获取AI_进攻及请求_城市_突袭(force0);
					if (!pk::is_valid_city_id(src_city_id))
					{
						pk::trace("异常城市id" + src_city_id);
						return;
					}
					if (dst_src.first != -1 and dst_src.second != -1)//尝试修复48714e,据说以前闪退发生在ai对ai突袭
					{
						src_city_id = dst_src.second;
						int dst_city_id = dst_src.first;
						pk::city@ src_city = pk::get_city(src_city_id);
						pk::city@ dst_city = pk::get_city(dst_city_id);
						dst_force_id = dst_city.get_force_id();
						if (src_city.is_player())
						{
							pk::person@ reporter_ = pk::get_person(무장_전령);
							string dst_force_name = pk::decode(pk::get_name(pk::get_force(dst_force_id)));
							string src_city_name = pk::decode(pk::get_name(src_city));
							//string dst_city_name = pk::decode(pk::get_name(dst_city));
							pk::message_box(pk::encode(pk::format("报告主公, \x1b[1x{}军\x1b[0x向我军的\x1b[2x{}\x1b[0x发动了突袭!", dst_force_name, src_city_name)), reporter_);
							//pk::move_screen(src_city.get_pos());
						}
						pk::scene(pk::scene_t(突袭执行));
					}
				}
			}

		} // district_Set()



		void 突袭执行()
		{

			pk::building@building_ = pk::get_building(src_city_id);//用city_id应该能直接获得建筑

			if (building_.is_player())pk::move_screen(building_.get_pos());

			pk::force@ force_ = pk::get_force(dst_force_id);
			int district_num = pk::get_district_id(force_, 1);
			// 男 무장 생성
			for (int j = 0; j < 敌军奇袭_男武将部队数; j++)
			{
				// 무장 설정
				pk::person@ man = getEmptyPerson();

				if (man is null) break;

				pk::set_district(man, district_num);

				int man_id = man.get_id();
				setTempPerson(man_id, 성별_남);

				int ground_weapon = 1 + pk::rand(병기_군마 - 1);
				man.tekisei[ground_weapon] = pk::rand(적성_S);

				// 능력치 반영.
				man.update();

				int gold = 2000;
				int food = int(兵力 * 1.3f);

				// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
				pk::point emptyPos = getEmptyPos(building_);

				if (!pk::is_valid_pos(emptyPos)) { pk::reset(man);	break; }

				pk::unit@ unit = pk::create_unit(building_, man, null, null, 兵力, ground_weapon, 兵器_走舸, gold, food, emptyPos);

				// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
				unit.energy = 100;

				// 부대 이동 불가 시 부대 및 무장 삭제 후 중단
				if (pk::get_movable_pos(unit).length == 0) { pk::kill(unit);	pk::reset(man);	break; }

				// 부대 생성 실패 시 부대 및 무장 삭제 후 중단
				if (!pk::is_alive(unit)) { pk::kill(unit); pk::reset(man); }

			} // for	

				// 女 무장 생성
			for (int k = 0; k < 敌军奇袭_女武将部队数; k++)
			{
				// 무장 설정
				pk::person@ woman = getEmptyPerson();

				if (woman is null) break;

				pk::set_district(woman, district_num);

				int woman_id = woman.get_id();
				setTempPerson(woman_id, 성별_여);

				int ground_weapon = 1 + pk::rand(병기_군마 - 1);
				woman.tekisei[ground_weapon] = pk::rand(적성_S);

				// 능력치 반영.
				woman.update();

				int gold = 2000;
				int food = int(兵力 * 1.3f);

				// 생성될 부대 포지션이 문제 있으면 부대를 생성하지 않는다.		
				pk::point emptyPos = getEmptyPos(building_);

				if (!pk::is_valid_pos(emptyPos) or !building_.is_player()) { pk::reset(woman);	break; }

				pk::unit@ unit = pk::create_unit(building_, woman, null, null, 兵力, ground_weapon, 兵器_走舸, gold, food, emptyPos);

				// 기력 설정(pk::create_unit 함수의 첫 번째 파라미터로 결정했다면 필요없음.)
				unit.energy = 100;

				// 부대 이동 불가 시 부대 및 무장 삭제 후 중단
				if (pk::get_movable_pos(unit).length == 0) { pk::kill(unit);	pk::reset(woman);	break; }

				// 부대 생성 실패 시 부대 및 무장 삭제 후 중단
				if (!pk::is_alive(unit)) { pk::kill(unit); pk::reset(woman); }

			} // for	

		} // void

		pk::point getEmptyPos(pk::building@ building_)
		{
			auto arr = pk::range(building_.get_pos(), 敌军奇袭_最小距离, 敌军奇袭_最大距离);
			for (int i = 0; i < int(arr.length); i++)
			{
				pk::hex@ hex = pk::get_hex(arr[i]);
				int terrain_id = hex.terrain;

				if (!hex.has_building and !hex.has_unit
					and pk::is_valid_terrain_id(terrain_id)
					and pk::is_enabled_terrain(terrain_id)
					and terrain_id != 지형_산)
					return arr[i];

			}
			return -1;
		}

		pk::person@ getEmptyPerson()
		{
			for (int i = 1000; i <= 1099; i++)
			{
				pk::person@ new_face = pk::get_person(i);

				switch (new_face.mibun)
				{
				case 신분_없음:
				case 신분_미등장:
				case 신분_미발견:
				case 신분_사망:
					return new_face;
				}
			}
			return null;
		}

		void setTempPerson(int new_person_id, int sex)
		{
			pk::person@ new_person = pk::get_person(new_person_id);
			new_person.name_read = pk::encode("突袭者");	//突袭者
			new_person.sex = sex;


			if (自动姓名)
			{
				new_person.sei = pk::get_family_name(random(성씨_끝 - 1)).name; // 성
				new_person.mei = pk::get_family_name(random(성씨_끝 - 1)).name; // 명
			}
			else
			{
				if (sex == 성별_남)
				{
					new_person.sei = pk::encode(sei_0[random(i, j)]); // 성
					new_person.mei = pk::encode(mei_0[random(i, j)]); // 명
				}
				else
				{
					new_person.sei = pk::encode(sei_1[random(k, l)]); // 성
					new_person.mei = pk::encode(sei_1[random(k, m)]); // 명
				}
			}
			new_person.service = 建筑_末 - 1;
			new_person.face = new_person.sex == 성별_남 ? random(2000, 2099) : random(2100, 2173);
			new_person.mibun = 身份_一般;
			new_person.loyalty = 100;
			new_person.birth = pk::get_year() - 30;
			new_person.death = pk::min(353, new_person.birth + 99);
			new_person.appearance = pk::get_year() - 10;
			new_person.aishou = random(1, 145);
			new_person.rank = 관직_정동장군;
			new_person.kouseki = 1000;

			new_person.birthplace = random(주_끝 - 1);
			new_person.skill = random(특기_끝) - 1;
			new_person.wadai = random(화제_끝 - 1);
			new_person.giri = random(의리_끝 - 1);
			new_person.ambition = random(야망_끝 - 1);
			new_person.promotion = random(기용_끝 - 1);
			new_person.character = random(성격_끝 - 1);
			new_person.voice = new_person.sex == 성별_남 ? random(음성_소심남, 음성_저돌남) : random(음성_냉정녀, 음성_대담녀);
			new_person.tone = new_person.sex == 성별_남 ? random(말투_장비, 말투_보통남) : random(말투_고대만족, 말투_보통녀);
			new_person.kanshitsu = random(한실_끝 - 1);
			new_person.strategic_tendency = random(전략경향_끝 - 1);
			new_person.local_affiliation = random(지역집착_끝 - 1);

			// 造型
			if (sex == 성별_남)
			{
				new_person.skeleton = 0;
				new_person.body[0] = 102;
				new_person.body[1] = 84;
				new_person.body[2] = 85;
				new_person.body[3] = 85;
				new_person.body[4] = 85;
				new_person.body[5] = 85;
			}
			else
			{
				new_person.skeleton = 1;
				new_person.body[0] = 77;
				new_person.body[1] = 78;
				new_person.body[2] = 78;
				new_person.body[3] = 78;
				new_person.body[4] = 113;
				new_person.body[5] = 113;
			}

			//	亲爱厌恶关系
			for (i = 0;i < 5;i++)
			{
				new_person.liked[i] = -1;
				new_person.disliked[i] = -1;
			}

			// 能力
			for (int i = 0; i < 武将能力_末; i++)
			{
				new_person.base_stat[i] = random(min_stat, max_stat);
				new_person.stat_aging[i] = random(능력성장_초지속형, 능력성장_장비형);
			}
			// 适性
			for (int i = 0; i < 병종_끝; i++)
			{
				new_person.tekisei[i] = random(适性_C, 적성_A);
			}

			//  話术
			new_person.wajutsu_daikatsu = pk::rand_bool(30);  //大喝
			new_person.wajutsu_kiben = pk::rand_bool(30);     //詭辯
			new_person.wajutsu_mushi = pk::rand_bool(30);     //无視
			new_person.wajutsu_chinsei = pk::rand_bool(30);   //镇静
			new_person.wajutsu_gyakujou = pk::rand_bool(30);  //憤怒

			new_person.update();
		}


	} // class Main

	Main main;

} // namespace
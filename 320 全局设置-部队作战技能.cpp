// ## 2022/01/02 # 江东新风 # 暴击逻辑修改 ##
// ## 2021/11/20 # 江东新风 # 0兵力部队会被战法干掉 ##
// ## 2021/11/15 # 江东新风 # 使用计略成功率计算方法来计算妙计成功率，加入失败动画 ##
// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/09/26 # 江东新风 # 添加作战特技的文字暴击图及动画 ##
// ## 2021/02/15 # 江东新风 # log势力错误 ##
// ## 2021/01/31 # 江东新风 # rand函数错误修复 ##
// ## 2021/01/05 # 江东新风 # AI释放神火计时判断是否划算，部队目标大于等于2或总目标大于等于3##
// ## 2021/01/01 # 江东新风 # 战神特技也能大杀四方,优化神火计动画效果，无部队也能点火 ##
// ## 2020/12/12 # 江东新风 # 修正参数类型错误 ##
// ## 2020/10/29 # 江东新风 # 新特技军略，谋略，韬略。及添加右下角消息提示##
// ## 2020/09/07 # 江东新风 # 限制部队战术等技能最高伤害量为现有兵力5倍 ##
// ## 2020/08/16 # 氕氘氚 # ##
// ## 2020/08/08 # 氕氘氚 # 平衡性修正 ##


//	===== 通用函数 =============================================================================================

//	气力消耗
uint8 global_func_最终气力消耗(pk::unit@ src_unit, int energy_cost)
{
	uint8 n = uint8(energy_cost);
	if (src_unit.has_skill(특기_백출))
	{
		if (百出耗气规则 >= 1)
			n /= 2; // 기력 소모량 절반 감소
		else
			n = 1;  // 기력 소모량 무조건 1로
	}

	//병력수에 따른 기력 소모량 차등 - 소부대 기력 소모 증가, 대부대 기력 소모 감소
	if (兵力影响耗气)
	{
		if (src_unit.troops <= 2000)
			n = uint8(n * pk::min(10.f, 2000.f / src_unit.troops));         // 최대 10배 소모
		else if (src_unit.troops >= 10000)
			n = uint8(n * pk::max(0.5f, (2 - src_unit.troops / 10000.f)));    // 최소 50% 소모
	}

	return n;
}

//	发动完成结算（?除气力、行动结束）
void global_func_发动完成结算(pk::unit@ src_unit, int energy_cost)
{
	// 실행 부대 기력 감소
	pk::add_energy(src_unit, -global_func_最终气力消耗(@src_unit, energy_cost), true);

	// 행동완료
	src_unit.action_done = true;
	if (int(pk::option["San11Option.EnableInfiniteAction"]) != 0)
		src_unit.action_done = false;
}


const bool 开启_作战技能暴击动画 = true; 
//	战略 =======================================================================================================
const bool 开启_部队战略_玩家 = true; 			// 부대전략메뉴_사용 설정 (true = on / false = off)
const bool 开启_士气高漲 = true; 			// 아군기력회복_사용 설정 (true = on / false = off)
const bool 开启_敌军动摇 = true; 			// 적군기력감소_사용 설정 (true = on / false = off)
const bool 开启_援军支援 = true; 			// 아군병력회복_사용 설정 (true = on / false = off)
const bool 开启_敌军煽动 = true; 			// 적군병력흡수_사용 설정 (true = on / false = off)

const int ENERGY_COST_士气高漲 = 40;     // 아군기력회복 기력 필요량
const int ENERGY_COST_敌军动摇 = 40;     // 적군기력감소 기력 필요량
const int ENERGY_COST_援军支援 = 40;     // 아군병력회복 기력 필요량
const int ENERGY_COST_敌军煽动 = 40;     // 적군병력흡수 기력 필요량

// 士气高漲
const int 士气高漲加气 = 10;

// 敌军动摇
const int 敌军动摇减气 = 10;

// 援军支援
const int 援军支援加兵 = 1000;
const int 援军支援加粮 = 4000;

// 敌军煽动
const int 敌军煽动减兵 = 1000;
const int 攻心吸收比率 = 25;

const int 部队战略_官职条件 = 관직_평북장군;			// 평북장군 이상의 관직
const int 部队战略_统率条件 = 80;      		// 최소 통솔 80 이상
const int 部队战略_智力条件 = 80;      		// 최소 지력 80 이상
const int 部队战略_兵力条件 = 5000;	    		// 전략메뉴를 실행할 수 있는 최소 병력 조건

const int 部队战略_暴击率 = 20;
const float 部队战略_智力暴击系数 = 0.2;
const float 部队战略_暴击效果 = 1.5;
const int 部队战略_成功率 = 60;	    			// 敌军智力高时

const int 部队战略_范围_min = 1;      			// 효과를 받는 최소 범위 
const int 部队战略_范围_max = 3; 				// 효과를 받는 최대 범위


bool func_部队战略_是否暴击(pk::person@ scr_leader)
{
	int chance = int(部队战略_暴击率 + scr_leader.stat[武将能力_智力] * 部队战略_智力暴击系数);
	return pk::rand_bool(chance);
}


void global_func_士气高漲(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	bool flag = false;
	bool crit = false;
	float rate = 1.f;
	array<pk::point> arr = pk::range(src_pos, 部队战略_范围_min, 部队战略_范围_max);

	if (src_unit.has_skill(特技_奏乐) or func_部队战略_是否暴击(@src_leader))
	{
		crit = true;
		rate = 部队战略_暴击效果;
	}

	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst = pk::get_unit(arr[j]);

		if (dst is null or pk::is_enemy(src_unit, dst)) continue;

		pk::add_energy(dst, int(士气高漲加气 * rate), true);
		flag = true;

	}

	if (flag && pk::is_in_screen(src_pos))
	{
		if (!crit)
		{
			if (开启_作战技能暴击动画)
			{
				pk::play_se(25);//使用暴击图音效
				pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/shiqigaozhang.png", 4);//填写暴击图位置
			}
		}
		else pk::face_cutin(src_unit);
		pk::play_voice(0x37, src_leader);
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		switch (pk::rand(2))
		{
		case 0: pk::say(pk::encode("一鼓作气!"), src_leader); break;
		case 1: pk::say(pk::encode("犒赏三军, 擂鼓助威!"), src_leader); break;
		}
		flag = false;
	}
	pk::history_log(src_pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x发动\x1b[28x士气高涨\x1b[0x，友方部队气力上升", pk::decode(pk::get_name(src_unit)))));

}


void global_func_敌军动摇(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	bool flag = false;
	bool crit = false;
	float rate = 1.f;
	array<pk::point> arr = pk::range(src_pos, 部队战略_范围_min, 部队战略_范围_max);

	if (src_unit.has_skill(특기_기략) or func_部队战略_是否暴击(@src_leader))
	{
		crit = true;
		rate = 部队战略_暴击效果;
	}

	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst = pk::get_unit(arr[j]);

		if (dst is null or !pk::is_enemy(src_unit, dst)) continue;

		if (ch::has_skill(dst, 特技_军略)) continue;

		pk::person@ dst_leader = pk::get_person(dst.leader);

		int chance = 部队战略_成功率 + 军略成功率上升;
		if (src_leader.stat[武将能力_智力] - dst_leader.stat[武将能力_智力] > 0)
			chance += (src_leader.stat[武将能力_智力] - dst_leader.stat[武将能力_智力]) * 2;

		float t = 1.f * src_leader.stat[武将能力_统率] / dst_leader.stat[武将能力_统率];
		t = ch::inRange(t, 0.5f, 1.5f);
		if (pk::rand_bool(pk::min(100, chance)))
		{
			pk::add_energy(dst, int(-敌军动摇减气 * rate * t), true);
			pk::history_log(src_pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x发动\x1b[28x敌军动摇\x1b[0x，\x1b[2x{}\x1b[0x产生动摇", pk::decode(pk::get_name(src_unit)), pk::decode(pk::get_name(dst)))));
			flag = true;
		}

	}

	if (flag && pk::is_in_screen(src_pos))
	{
		if (!crit)
		{
			if (开启_作战技能暴击动画)
			{
				pk::play_se(25);//使用暴击图音效
				pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/dongyaodijun.png", 4);//填写暴击图位置
			}
		}
		else pk::face_cutin(src_unit);
		pk::play_voice(0x37, src_leader);
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		switch (pk::rand(2))
		{
		case 0: pk::say(pk::encode("击溃敌军的士气!"), src_leader); break;
		case 1: pk::say(pk::encode("让敌军见识下我军的溃敌之策!"), src_leader); break;
		}
		flag = false;
	}

}


void global_func_援军支援(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	bool flag = false;
	bool crit = false;
	int t = 0, f = 0, count_t = 0, count_f = 0, unit_count = 0;
	float rate = 1.f;
	array<pk::point> arr = pk::range(src_pos, 部队战略_范围_min, 部队战略_范围_max);
	arr.insertLast(src_pos);

	if (src_unit.has_skill(특기_운반) or func_部队战略_是否暴击(@src_leader))
	{
		crit = true;
		rate = 部队战略_暴击效果;
	}

	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst = pk::get_unit(arr[j]);

		if (dst is null or pk::is_enemy(src_unit, dst)) continue;

		t = int(援军支援加兵 * rate);
		f = int(援军支援加粮 * rate);
		//ch::u8debug(pk::format("rate:{}, tooops:{}, food:{}",rate, t, f));
		count_t += t; count_f += f;
		t = int(t * ch::randint(80, 100) / 100.f);
		f = int(f * ch::randint(50, 75) / 100.f);
		//ch::u8debug(pk::format("rate:{}, tooops:{}, food:{}",rate, t, f));
		ch::add_troops(dst, t, true);
		pk::add_food(dst, f, true);
		flag = true;
		unit_count++;
		if (unit_count >= 6) break;
	}
	pk::building@ src_building = pk::get_building(src_leader.service);
	ch::add_troops(src_building, -count_t, true);
	pk::add_food(src_building, -count_f, true);
	pk::history_log(src_pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x召唤\x1b[28x援军支援\x1b[0x，威势大盛", pk::decode(pk::get_name(src_unit)))));

	if (flag && pk::is_in_screen(src_pos))
	{
		if (!crit)
		{
			if (开启_作战技能暴击动画)
			{
				pk::play_se(25);//使用暴击图音效
				pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/yuanjunzhiyuan.png", 4);//填写暴击图位置
			}
		}
		else pk::face_cutin(src_unit);
		pk::play_voice(0x37, src_leader);
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		switch (pk::rand(2))
		{
		case 0: pk::say(pk::encode("援兵已到,反攻的机会来了!"), src_leader); break;
		case 1: pk::say(pk::encode("养兵千日,用在一时!"), src_leader); break;
		}
		flag = false;
	}
}


void global_func_敌军煽动(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	bool flag = false;
	bool crit = false;
	float rate = 1.f;
	array<pk::point> arr = pk::range(src_pos, 部队战略_范围_min, 部队战略_范围_max);

	if (src_unit.has_skill(특기_심공) or func_部队战略_是否暴击(@src_leader))
	{
		crit = true;
		rate = 部队战略_暴击效果;
	}

	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst = pk::get_unit(arr[j]);

		if (dst is null or !pk::is_enemy(src_unit, dst)) continue;

		if (ch::has_skill(dst, 特技_军略)) continue;

		pk::person@ dst_leader = pk::get_person(dst.leader);

		int chance = 部队战略_成功率 + 军略成功率上升;
		if (src_leader.stat[武将能力_智力] - dst_leader.stat[武将能力_智力] > 0)
			chance += (src_leader.stat[武将能力_智力] - dst_leader.stat[武将能力_智力]) * 2;

		if (pk::rand_bool(pk::min(100, chance)))
		{
			float t = 1.f * src_leader.stat[武将能力_统率] / dst_leader.stat[武将能力_统率];
			t = ch::inRange(t, 0.5f, 1.2f);
			int n = pk::min(int(敌军煽动减兵 * ch::randfloat(0.8, 1.2) * rate * t), src_unit.troops * 5);
			ch::add_troops(dst, -n, true,100);
			ch::add_troops(src_unit, int(n * 攻心吸收比率 / 100.f), true);
			pk::history_log(src_pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x的士兵遭到\x1b[2x{}\x1b[0x的煽动，部分\x1b[28x叛逃\x1b[0x。", pk::decode(pk::get_name(dst)), pk::decode(pk::get_name(src_unit)))));
			flag = true;
		}
	}

	if (flag && pk::is_in_screen(src_pos))
	{
		if (!crit)
		{
			if (开启_作战技能暴击动画)
			{
				pk::play_se(25);//使用暴击图音效
				pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/shandongdijun.png", 4);//填写暴击图位置
			}
		}
		else pk::face_cutin(src_unit);
		pk::play_voice(0x37, src_leader);
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		switch (pk::rand(2))
		{
		case 0: pk::say(pk::encode("不战而屈人之兵,善之善者也!"), src_leader); break;
		case 1: pk::say(pk::encode("哈哈,敌军已出现溃逃投降之势!"), src_leader); break;
		}
		flag = false;
	}
}

//	======================================================================================================= 战略



//	战术 =======================================================================================================

	//昂扬奋战，威风屈敌，枪兵冲锋，戟兵破甲，骑兵突袭，大杀四方
const bool 开启_部队战术_玩家 = true; 			// 부대전술메뉴_사용 설정 (true = on / false = off)

const bool 开启_昂扬奋战 = true; 			// 부대전술메뉴_사용 설정 (true = on / false = off)
const bool 开启_威风屈敌 = true; 			// 부대전술메뉴_사용 설정 (true = on / false = off)
const bool 开启_枪兵冲锋 = true; 			// 부대전술메뉴_사용 설정 (true = on / false = off)
const bool 开启_戟兵破甲 = true; 			// 부대전술메뉴_사용 설정 (true = on / false = off)
const bool 开启_骑兵突袭 = true; 			// 부대전술메뉴_사용 설정 (true = on / false = off)
const bool 开启_大杀四方 = true; 			// 부대전술메뉴_사용 설정 (true = on / false = off)

const int ENERGY_COST_昂扬奋战 = 40;       // 앙양전술 기력 필요량
const int ENERGY_COST_威风屈敌 = 40;       // 위풍전술 기력 필요량
const int ENERGY_COST_枪兵冲锋 = 40;       // 창군전술 기력 필요량
const int ENERGY_COST_戟兵破甲 = 40;       // 극군전술 기력 필요량
const int ENERGY_COST_骑兵突袭 = 40;       // 기마전술 기력 필요량
const int ENERGY_COST_大杀四方 = 50;       // 용장전술 기력 필요량

const int 昂扬奋战减气 = 20;      		// 통솔 80대, 무력 80대
const int 昂扬奋战吸气比率 = 20; 			// <앙양> 특기 기력 흡수량 계산 [energy_감소 / 5]

const int 威风屈敌减气 = 20;      		// 통솔 80대, 무력 80대

const int 枪兵冲锋减兵 = 1600; 				// 통솔 80대, 무력 80대 (2000 + 0~500 = 실제 감소되는 병력 수치)
const int 枪兵冲锋减气 = 10;      		// 통솔 80대, 무력 80대

const int 戟兵破甲减兵 = 1600; 				// 통솔 80대, 무력 80대 (2000 + 0~500 = 실제 감소되는 병력 수치)
const int 戟兵破甲减气 = 10;      		// 통솔 80대, 무력 80대

const int 骑兵突袭减兵 = 1600; 				// 통솔 80대, 무력 80대 (2000 + 0~500 = 실제 감소되는 병력 수치)
const int 骑兵突袭减气 = 10;      		// 통솔 80대, 무력 80대

const int 大杀四方减兵 = 2000; 				// 통솔 80대, 무력 80대 (2500 + 0~500 = 실제 감소되는 병력 수치)
const int 大杀四方减气 = 20;      	// 통솔 80대, 무력 80대

const int 大杀四方眩晕率 = 30;
const int 大杀四方眩晕回合 = 2;

const int 部队战术_官职条件 = 관직_평북장군;			// 평북장군 이상의 관직
const int 部队战术_统率条件 = 80;      		// 최소 통솔 80 이상
const int 部队战术_武力条件 = 80;      		// 최소 지력 80 이상
const int 部队战术_兵力条件 = 5000;	    		// 전략메뉴를 실행할 수 있는 최소 병력 조건

const int 部队战术_暴击率 = 20;
const float 部队战术_统率暴击系数 = 0.1;
const float 部队战术_武力暴击系数 = 0.1;
const float 部队战术_暴击效果 = 1.5;
const int 部队战术_成功率 = 60;	    			// 敌军智力高时

const int 部队战术_范围_min = 1;      			// 효과를 받는 최소 범위 
const int 部队战术_范围_max = 1; 				// 효과를 받는 최대 범위


bool func_部队战术_是否暴击(pk::person@ scr_leader)
{
	int chance = int(部队妙计_暴击率 + scr_leader.stat[武将能力_统率] * 部队战术_统率暴击系数 + scr_leader.stat[武将能力_武力] * 部队战术_武力暴击系数);
	return pk::rand_bool(chance);
}


void global_func_昂扬奋战(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	bool flag = false;
	bool crit = false;
	float rate = 1.f;
	array<pk::point> arr = pk::range(src_pos, 部队战术_范围_min, 部队战术_范围_max);

	if (func_部队战术_是否暴击(@src_leader))
	{
		crit = true;
		rate = 部队战术_暴击效果;
	}

	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst_unit = pk::get_unit(arr[j]);

		if (dst_unit is null or !pk::is_enemy(src_unit, dst_unit)) continue;

		pk::person@ dst_leader = pk::get_person(dst_unit.leader);

		if (ch::has_skill(dst_unit, 특기_앙양)) continue;
		if (ch::has_skill(dst_unit, 特技_韬略)) continue;

		int chance = 部队战术_成功率 + 韬略成功率上升 + ch::minZero(src_unit.attr.stat[武将能力_武力] - dst_unit.attr.stat[武将能力_武力]);

		if (pk::rand_bool(pk::min(100, chance)))
		{
			float t = 1.f * src_leader.stat[武将能力_统率] / dst_leader.stat[武将能力_统率];	//统率修正
			t = ch::inRange(t, 0.5f, 1.25f);
			int 减气 = int(昂扬奋战减气 * t);
			pk::add_energy(dst_unit, -减气, true); //减气
			pk::add_energy(src_unit, int(减气 * 昂扬奋战吸气比率 / 100.f), true); //吸收 
			flag = true;
			pk::history_log(src_pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x发动昂扬奋战，\x1b[28x士气大盛\x1b[0x。", pk::decode(pk::get_name(src_unit)))));

		}
	}

	if (flag && pk::is_in_screen(src_pos))
	{
		if (!crit)
		{
			if (开启_作战技能暴击动画)
			{
				pk::play_se(25);//使用暴击图音效
				pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/angyangfenzhan.png", 4);//填写暴击图位置
			}
		}
		else pk::face_cutin(src_unit);
		pk::play_voice(0x37, src_leader);
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);
		pk::say(pk::encode("战意高昂,一鼓作气!"), src_leader);
	}
}


void global_func_威风屈敌(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	bool flag = false;
	bool crit = false;
	float rate = 1.f;
	array<pk::point> arr = pk::range(src_pos, 部队战术_范围_min, 部队战术_范围_max);

	if (func_部队战术_是否暴击(@src_leader))
	{
		crit = true;
		rate = 部队战术_暴击效果;
	}

	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst_unit = pk::get_unit(arr[j]);

		if (dst_unit is null or !pk::is_enemy(src_unit, dst_unit)) continue;

		pk::person@ dst_leader = pk::get_person(dst_unit.leader);

		if (ch::has_skill(dst_unit, 特技_威风)) continue;
		if (ch::has_skill(dst_unit, 特技_韬略)) continue;

		int chance = 部队战术_成功率 + 韬略成功率上升 + ch::minZero(src_unit.attr.stat[武将能力_武力] - dst_unit.attr.stat[武将能力_武力]);

		if (pk::rand_bool(pk::min(100, chance)))
		{
			float t = 1.f * src_leader.stat[武将能力_统率] / dst_leader.stat[武将能力_统率];	//统率修正
			t = ch::inRange(t, 0.5f, 1.25f);
			int 减气 = int(威风屈敌减气 * t * rate);
			pk::add_energy(dst_unit, -减气, true); //减气
			pk::history_log(src_pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("受到\x1b[2x{}\x1b[0x的\x1b[27x威风屈敌\x1b[0x战术影响，\x1b[2x{}\x1b[0x士气动摇。", pk::decode(pk::get_name(src_unit)), pk::decode(pk::get_name(dst_unit)))));
			flag = true;

			if (src_leader.stat[武将能力_武力] > dst_leader.stat[武将能力_武力])
			{
				if (pk::rand_bool(int(大杀四方眩晕率 * rate)) and dst_unit.status == 部队状态_通常)
				{
					pk::set_status(dst_unit, src_unit, 部队状态_混乱, 大杀四方眩晕回合, true);	//混乱
					pk::history_log(src_pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x的\x1b[27x威风屈敌\x1b[0x使\x1b[2x{}\x1b[0x陷入眩晕。", pk::decode(pk::get_name(src_unit)), pk::decode(pk::get_name(dst_unit)))));
				}
			}
		}
	}

	if (flag && pk::is_in_screen(src_pos))
	{
		if (!crit)
		{
			if (开启_作战技能暴击动画)
			{
				pk::play_se(25);//使用暴击图音效
				pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/weifengqudi.png", 4);//填写暴击图位置
			}
		}
		else pk::face_cutin(src_unit);
		pk::play_voice(0x37, src_leader);
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		pk::say(pk::encode("在我军的威风下颤抖!"), src_leader);
	}
}


void global_func_枪兵冲锋(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	bool flag = false;
	bool crit = false;
	float rate = 1.f;
	array<pk::point> arr = pk::range(src_pos, 部队战术_范围_min, 部队战术_范围_max);

	if (func_部队战术_是否暴击(@src_leader))
	{
		crit = true;
		rate = 部队战术_暴击效果;
	}

	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst_unit = pk::get_unit(arr[j]);

		if (dst_unit is null or !pk::is_enemy(src_unit, dst_unit)) continue;

		pk::person@ dst_leader = pk::get_person(dst_unit.leader);

		if (dst_unit.weapon != 병기_군마) continue;
		if (ch::has_skill(dst_unit, 特技_韬略)) continue;

		int chance = 部队战术_成功率 + 韬略成功率上升 + ch::minZero(src_unit.attr.stat[武将能力_武力] - dst_unit.attr.stat[武将能力_武力]);

		if (pk::rand_bool(pk::min(100, chance)))
		{
			float t = 1.f * src_leader.stat[武将能力_统率] / dst_leader.stat[武将能力_统率];	//统率修正
			t = ch::inRange(t, 0.5f, 1.5f);
			int 减兵 = pk::min(int(枪兵冲锋减兵 * t * rate), src_unit.troops * 5);
			int 减气 = int(枪兵冲锋减气 * t * rate);
			ch::add_troops(dst_unit, -减兵, true,100); // 병력 감소	
			pk::add_energy(dst_unit, -减气, true); // 기력 감소
			pk::history_log(src_pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("受到\x1b[2x{}\x1b[0x的\x1b[27x枪兵冲锋\x1b[0x战术影响，\x1b[2x{}\x1b[0x大溃，士气动摇。", pk::decode(pk::get_name(src_unit)), pk::decode(pk::get_name(dst_unit)))));
			flag = true;
			if (dst_unit.get_troops() == 0)
			{
				if (pk::is_in_screen(dst_unit.pos)) pk::create_death_effect(dst_unit, src_unit);
				pk::kill(dst_unit, src_unit);//放dst_unit调用的最后，防止空指针
			}
		}
	}

	if (flag && pk::is_in_screen(src_pos))
	{
		if (!crit)
		{
			if (开启_作战技能暴击动画)
			{
				pk::play_se(25);//使用暴击图音效
				pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/qiangbingchongfeng.png", 4);//填写暴击图位置
			}
		}
		else pk::face_cutin(src_unit);
		pk::play_voice(0x37, src_leader);
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		switch (pk::rand(2))
		{
		case 0: pk::say(pk::encode("给我消灭对面的骑兵部队!"), src_leader); break;  //적군 기병들이 두려워하는군
		case 1: pk::say(pk::encode("我的枪兵战无不胜!"), src_leader); break;  //아군의 창병 전술에 달아나는 꼴이라니..
		}
	}
}


void global_func_戟兵破甲(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	bool flag = false;
	bool crit = false;
	float rate = 1.f;
	array<pk::point> arr = pk::range(src_pos, 部队战术_范围_min, 部队战术_范围_max);

	if (func_部队战术_是否暴击(@src_leader))
	{
		crit = true;
		rate = 部队战术_暴击效果;
	}

	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst_unit = pk::get_unit(arr[j]);

		if (dst_unit is null or !pk::is_enemy(src_unit, dst_unit)) continue;

		pk::person@ dst_leader = pk::get_person(dst_unit.leader);

		if (dst_unit.weapon != 병기_노 && dst_unit.weapon != 병기_창) continue;
		if (ch::has_skill(dst_unit, 特技_韬略)) continue;

		int chance = 部队战术_成功率 + 韬略成功率上升 + ch::minZero(src_unit.attr.stat[武将能力_武力] - dst_unit.attr.stat[武将能力_武力]);

		if (pk::rand_bool(pk::min(100, chance)))
		{
			float t = 1.f * src_leader.stat[武将能力_统率] / dst_leader.stat[武将能力_统率];	//统率修正
			t = ch::inRange(t, 0.5f, 1.2f);
			int 减兵 = pk::min(int(戟兵破甲减兵 * t * rate), src_unit.troops * 5);
			int 减气 = int(戟兵破甲减气 * t * rate);
			ch::add_troops(dst_unit, -减兵, true, 100); // 병력 감소				
			pk::add_energy(dst_unit, -减气, true); // 기력 감소
			pk::history_log(src_pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("受到\x1b[2x{}\x1b[0x的\x1b[27x戟兵破甲\x1b[0x战术影响，\x1b[2x{}\x1b[0x大溃，士气动摇。", pk::decode(pk::get_name(src_unit)), pk::decode(pk::get_name(dst_unit)))));
			flag = true;
			if (dst_unit.get_troops() == 0)
			{
				if (pk::is_in_screen(dst_unit.pos)) pk::create_death_effect(dst_unit, src_unit);
				pk::kill(dst_unit, src_unit);//放dst_unit调用的最后，防止空指针
			}
		}
	}

	if (flag && pk::is_in_screen(src_pos))
	{
		if (!crit)
		{
			if (开启_作战技能暴击动画)
			{
				pk::play_se(25);//使用暴击图音效
				pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/jibingpojia.png", 4);//填写暴击图位置
			}
		}
		else pk::face_cutin(src_unit);
		pk::play_voice(0x37, src_leader);
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		switch (pk::rand(2))
		{
		case 0: pk::say(pk::encode("给我消灭对面的枪兵部队!"), src_leader); break;
		case 1: pk::say(pk::encode("我的戟兵战无不胜!"), src_leader); break;
		}
	}
}


void global_func_骑兵突袭(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	bool flag = false;
	bool crit = false;
	float rate = 1.f;
	array<pk::point> arr = pk::range(src_pos, 部队战术_范围_min, 部队战术_范围_max);

	if (func_部队战术_是否暴击(@src_leader))
	{
		crit = true;
		rate = 部队战术_暴击效果;
	}

	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst_unit = pk::get_unit(arr[j]);

		if (dst_unit is null or !pk::is_enemy(src_unit, dst_unit)) continue;

		pk::person@ dst_leader = pk::get_person(dst_unit.leader);

		if (dst_unit.weapon != 병기_극) continue;
		if (ch::has_skill(dst_unit, 特技_韬略)) continue;

		int chance = 部队战术_成功率 + 韬略成功率上升 + ch::minZero(src_unit.attr.stat[武将能力_武力] - dst_unit.attr.stat[武将能力_武力]);

		if (pk::rand_bool(pk::min(100, chance)))
		{
			float t = 1.f * src_leader.stat[武将能力_统率] / dst_leader.stat[武将能力_统率];	//统率修正
			t = ch::inRange(t, 0.5f, 1.2f);
			int 减兵 = pk::min(int(骑兵突袭减兵 * t * rate), src_unit.troops * 5);
			int 减气 = int(骑兵突袭减气 * t * rate);
			ch::add_troops(dst_unit, -减兵, true, 100); // 병력 감소				
			pk::add_energy(dst_unit, -减气, true); // 기력 감소
			pk::history_log(src_pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("受到\x1b[2x{}\x1b[0x的\x1b[27x骑兵突袭\x1b[0x战术影响，\x1b[2x{}\x1b[0x大溃，士气动摇。", pk::decode(pk::get_name(src_unit)), pk::decode(pk::get_name(dst_unit)))));
			flag = true;
			if (dst_unit.get_troops() == 0)
			{
				if (pk::is_in_screen(dst_unit.pos)) pk::create_death_effect(dst_unit, src_unit);
				pk::kill(dst_unit, src_unit);//放dst_unit调用的最后，防止空指针
			}
		}
	}

	if (flag && pk::is_in_screen(src_pos))
	{
		if (!crit)
		{
			if (开启_作战技能暴击动画)
			{
				pk::play_se(25);//使用暴击图音效
				pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/qibingtuxi.png", 4);//填写暴击图位置
			}
		}
		else pk::face_cutin(src_unit);

		pk::play_voice(0x37, src_leader);
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		switch (pk::rand(2))
		{
		case 0: pk::say(pk::encode("给我消灭对面的戟兵部队!"), src_leader); break;
		case 1: pk::say(pk::encode("我的骑兵战无不胜!"), src_leader); break;
		}
	}
}


void global_func_大杀四方(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	bool flag = false;
	bool crit = false;
	float rate = 1.f;
	array<pk::point> arr = pk::range(src_pos, 部队战术_范围_min, 部队战术_范围_max);
	//ch::u8debug(pk::format("dashasifang:{}  1", rate));
	if (func_部队战术_是否暴击(@src_leader))
	{
		crit = true;
		rate = 部队战术_暴击效果;
	}
	//ch::u8debug(pk::format("dashasifang:{}  2", rate));	
	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst_unit = pk::get_unit(arr[j]);
		//ch::u8debug(pk::format("j:{}  1",j));
		if (dst_unit is null or !pk::is_enemy(src_unit, dst_unit)) continue;
		pk::person@ dst_leader = pk::get_person(dst_unit.leader);

		if (ch::has_skill(dst_unit, 특기_패왕) or ch::has_skill(dst_unit, 특기_용장) or ch::has_skill(dst_unit, 특기_맹장)
			or ch::has_skill(dst_unit, 특기_신장) or ch::has_skill(dst_unit, 특기_비장) or ch::has_skill(dst_unit, 特技_战神)) continue;
		if (ch::has_skill(dst_unit, 特技_韬略)) continue;
		//ch::u8debug(pk::format("j:{}  3",j));
		int chance = 部队战术_成功率 + 韬略成功率上升 + ch::minZero(src_unit.attr.stat[武将能力_武力] - dst_unit.attr.stat[武将能力_武力]);
		//ch::u8debug(pk::format("chance:{}", chance));
		if (pk::rand_bool(pk::min(100, chance)))
		{
			//ch::u8debug("atk");
			float t = 1.f * src_leader.stat[武将能力_统率] / dst_leader.stat[武将能力_统率];	//统率修正
			t = ch::inRange(t, 0.5f, 1.2f);
			//ch::u8debug(pk::format("t:{}",t));
			int 减兵 = pk::min(int(大杀四方减兵 * t * rate), src_unit.troops * 5);
			int 减气 = int(大杀四方减气 * t * rate);
			//ch::u8debug(pk::format("troops:{}",减兵));
			ch::add_troops(dst_unit, -减兵, true, 100); // 병력 감소	
			pk::add_energy(dst_unit, -减气, true); // 기력 감소
			pk::history_log(src_pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("受到\x1b[2x{}\x1b[0x的\x1b[27x大杀四方\x1b[0x战术影响，\x1b[2x{}\x1b[0x大溃，士气动摇。", pk::decode(pk::get_name(src_unit)), pk::decode(pk::get_name(dst_unit)))));
			flag = true;

			if (pk::rand_bool(大杀四方眩晕率) and dst_unit.status == 部队状态_通常)
			{
				pk::set_status(dst_unit, src_unit, 部队状态_混乱, 大杀四方眩晕回合, true);
				pk::history_log(src_pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x的\x1b[27x大杀四方\x1b[0x使\x1b[2x{}\x1b[0x陷入眩晕。", pk::decode(pk::get_name(src_unit)), pk::decode(pk::get_name(dst_unit)))));
			}
			if (dst_unit.get_troops() == 0)
			{
				if (pk::is_in_screen(dst_unit.pos)) pk::create_death_effect(dst_unit, src_unit);
				pk::kill(dst_unit, src_unit);//放dst_unit调用的最后，防止空指针
			}
		}
	}

	if (flag && pk::is_in_screen(src_pos))
	{
		if (!crit)
		{
			if (开启_作战技能暴击动画)
			{
				pk::play_se(25);//使用暴击图音效
				pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/dashasifang.png", 4);//填写暴击图位置
			}
		}
		else pk::face_cutin(src_unit);
		//pk::critical_effect(2200);
		pk::play_voice(0x37, src_leader);
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		switch (pk::rand(2))
		{
		case 0: pk::say(pk::encode("众将士,随我杀他个人仰马翻"), src_leader); break;
		case 1: pk::say(pk::encode("看我杀杀杀!"), src_leader); break;
		}
	}

}


//	======================================================================================================= 战术



//	妙计 =======================================================================================================
	//范围混乱 范围伪报 神火计 八阵图 虚虚实实 速战固守
const bool 开启部队妙计_玩家 = true; 			// 묘책 사용 설정 (true = on / false = off)

const bool 开启_范围混乱 = true; 			// 묘책: 귀모계 사용 설정 (true = on / false = off)
const bool 开启_范围伪报 = true; 			// 묘책: 허보계 사용 설정 (true = on / false = off)
const bool 开启_神火计 = true; 			// 묘책: 신화계 사용 설정 (true = on / false = off)
const bool 开启_八阵图 = true; 			// 묘책: 팔진도 사용 설정 (true = on / false = off)
const bool 开启_虚虚实实 = true; 			// 묘책: 허허실실 사용 설정 (true = on / false = off)
const bool 开启_速战固守 = true; 			// 묘책: 속전고수 사용 설정 (true = on / false = off)

const int ENERGY_COST_范围混乱 = 50;        // 귀모계 기력 필요량
const int ENERGY_COST_范围伪报 = 50;        // 허보계 기력 필요량
const int ENERGY_COST_神火计 = 50;        // 신화계 기력 필요량
const int ENERGY_COST_八阵图 = 50;        // 팔진도 기력 필요량
const int ENERGY_COST_虚虚实实 = 50;       // 허허실실 기력 필요량
const int ENERGY_COST_速战固守 = 50;       // 속전고수 기력 필요량

const int 范围混乱回合 = 2;
const int 范围伪报回合 = 2;
const int 神火计减气 = 20;
const bool 神火计眩晕 = true;
const int 八阵图加气 = 20;
const int 八阵图减气 = 20;
const bool 八阵图重置行动 = false;
const int 八阵图混乱回合 = 2;
const int 虚虚实实控制回合 = 2;
const float 虚虚实实减兵系数 = 10.0;
const int 虚虚实实减兵比例 = 10;
const int 虚虚实实统率基准 = 60;
const int 速战固守加气 = 20;
const int 速战固守减气 = 20;
const float 速战固守减粮系数 = 10.0;
const int 速战固守减粮比例 = 30;
const int 速战固守统率基准 = 60;

const int 部队妙计_兵力条件 = 5000;	    		   // 묘책메뉴를 실행할 수 있는 최소 병력 조건
const int 部队妙计_官职条件 = 관직_평북장군;			// 평북장군 이상의 관직

const int 部队妙计_暴击率 = 20;
const float 部队妙计_暴击效果 = 1.5;
const int 部队妙计_成功率 = 60;	    			// 敌军智力高时

const int 部队妙计_范围_min = 1;      			// 효과를 받는 최소 범위 
const int 部队妙计_范围_max = 3; 				// 효과를 받는 최대 범위

const int 部队妙计_增加经验 = 5;

///下面内容只是为了防止kre报错，实际不起效，所以kre修改也不起效
const bool 开启_严防死守 = true; 			// 묘책: 속전고수 사용 설정 (true = on / false = off)
const int ENERGY_COST_严防死守 = 50;       // 속전고수 기력 필요량
const int 严防死守加气 = 20;
const int 严防死守减气 = 20;
const float 严防死守减粮系数 = 10.0;
const int 严防死守减粮比例 = 30;
const int 严防死守统率基准 = 60;

bool func_部队妙计_是否暴击(pk::person@ scr_leader)	//暫时沒有用到
{
	int chance = int(部队妙计_暴击率 + scr_leader.stat[武将能力_智力] / 5.f);
	return pk::rand_bool(chance);
}


void global_func_范围混乱(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	array<pk::point> arr = pk::range(src_pos, 部队妙计_范围_min, 部队妙计_范围_max);

	int count = 0;
	
	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst_unit = pk::get_unit(arr[j]);

		if (dst_unit is null or dst_unit.status != 部队状态_通常 or !pk::is_enemy(src_unit, dst_unit)) continue;

		//成功率里已经有对应特技效果了
		//if (dst_unit.has_skill(특기_침착) or dst_unit.has_skill(특기_명경) or dst_unit.has_skill(특기_통찰) or dst_unit.has_skill(특기_신산)) continue;

		pk::person@ dst_leader = pk::get_person(dst_unit.leader);
		
		//int chance = 部队妙计_成功率 + 谋略成功率上升 + ch::minZero(src_unit.attr.stat[武将能力_智力] - dst_unit.attr.stat[武将能力_智力]);
		pk::int_bool result0 = cast<pk::func206_t>(pk::get_func(206))(src_unit, src_pos, dst_unit.pos, 妙计_大混乱);//int_bool 成功率 是否被看破

		if (result0.second)//看破
		{
			if (pk::is_in_screen(dst_unit.pos))
			{
				pk::create_effect(84, dst_unit.pos);
				pk::play_se(95, dst_unit.pos);
				pk::play_voice(69, dst_leader);
			}
		}
		else if (pk::rand_bool(pk::min(100, result0.first)))//计策成功
		{
			count += 1;//影响部队技计数
			pk::set_status(dst_unit, src_unit, 部队状态_混乱, 范围混乱回合, true);		//混乱

			if (pk::is_in_screen(dst_unit.pos))
			{
				ch::say_message(7122, /*p0*/dst_leader, /*p1*/null, /*u0*/null, /*p0_u*/dst_unit);	//快點恢復正常			
			}
		}
		else//计策失败
		{
			if (pk::is_in_screen(dst_unit.pos))
			{				
				pk::play_voice(70, dst_leader);
				ch::say_message(7206, /*p0*/dst_leader, /*p1*/null, /*u0*/null, /*p0_u*/dst_unit);//沒用的	
			}
		}
	}

	if (count > 0 && pk::is_in_screen(src_pos))
	{
		//只有影响部队大于2只时才播放动画，说明暴击
		if (开启_作战技能暴击动画 and count > 2) pk::play_se(25);//使用暴击图音效
		if (开启_作战技能暴击动画 and count > 2) pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/dahunluan.png", 4);//填写暴击图位置
		pk::play_voice(0x37, src_leader);  // 음성 : 전투_진정
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		if (pk::is_in_screen(src_pos)) ch::say_message(7108, /*p0*/src_leader, /*p1*/null, /*u0*/null, /*p0_u*/src_unit);//直接调用原版扰乱台词
	}
}


void global_func_范围伪报(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	array<pk::point> arr = pk::range(src_pos, 部队妙计_范围_min, 部队妙计_范围_max);
	int count = 0;
	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst_unit = pk::get_unit(arr[j]);

		if (dst_unit is null or dst_unit.status != 部队状态_通常 or !pk::is_enemy(src_unit, dst_unit)) continue;

		//现在成功率里已经有对应特技效果了
		//if (dst_unit.has_skill(특기_침착) or dst_unit.has_skill(특기_명경) or dst_unit.has_skill(특기_통찰) or dst_unit.has_skill(특기_신산)) continue;

		pk::person@ dst_leader = pk::get_person(dst_unit.leader);
		
		//int chance = 部队妙计_成功率 + 谋略成功率上升 + ch::minZero(src_unit.attr.stat[武将能力_智力] - dst_unit.attr.stat[武将能力_智力]);
		pk::int_bool result0 = cast<pk::func206_t>(pk::get_func(206))(src_unit, src_pos, dst_unit.pos, 妙计_大伪报);//int_bool 成功率 是否被看破


		if (result0.second)//看破
		{
			if (pk::is_in_screen(dst_unit.pos))
			{
				pk::create_effect(84, dst_unit.pos);
				pk::play_se(95, dst_unit.pos);
				pk::play_voice(69, dst_leader);
			}
		}
		else if (pk::rand_bool(pk::min(100, result0.first)))//计策成功
		{
			count += 1;//影响部队技计数
			pk::set_status(dst_unit, src_unit, 部队状态_伪报, 范围伪报回合, true);		//混乱

			if (pk::is_in_screen(dst_unit.pos))
			{
				ch::say_message(5806, /*p0*/dst_leader, /*p1*/null, /*u0*/null, /*p0_u*/dst_unit);	//事到如今，撤退是唯一的手段。\n退回{}			
			}
		}
		else//计策失败
		{
			if (pk::is_in_screen(dst_unit.pos))
			{
				pk::play_voice(70, dst_leader);
				ch::say_message(7206, /*p0*/dst_leader, /*p1*/null, /*u0*/null, /*p0_u*/dst_unit);//沒用的	
			}
		}

	}

	if (count > 0 && pk::is_in_screen(src_pos))
	{
		if (count > 2 && 开启_作战技能暴击动画) pk::play_se(25);//使用暴击图音效
		if (count > 2 && 开启_作战技能暴击动画) pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/daweibao.png", 4);//填写暴击图位置
		pk::play_voice(0x37, src_leader);  // 음성 : 전투_진정
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		if (pk::is_in_screen(src_pos)) ch::say_message(7095, /*p0*/src_leader, /*p1*/null, /*u0*/null, /*p0_u*/src_unit);//直接调用原版伪报台词
	}
}


void global_func_神火计(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	array<pk::point> arr = pk::range(src_pos, 部队妙计_范围_min, 部队妙计_范围_max);
	int count = 0;
	//判断是否划算的运算，仅限ai
	if (!src_unit.is_player())
	{
		int count_unit = 0;
		int count_building = 0;
		for (int j = 0; j < int(arr.length); j++)
		{
			pk::unit@ dst_unit = pk::get_unit(arr[j]);
			pk::building@ dst_building = pk::get_building(arr[j]);
			if (dst_unit !is null and pk::is_enemy(src_unit, dst_unit))
			{
				count_unit = count_unit + 1;
			}
			if (dst_building !is null and pk::is_enemy(src_unit, dst_building))
			{
				count_building = count_building + 1;
			}
			if (count_unit > 1 or (count_unit + count_building) > 2) break;
		}
		if (count_unit < 2 and (count_unit + count_building) < 3) return;
	}

	pk::create_effect(0x19, src_pos);//25也不错			
	for (int j = 0; j < int(arr.length); j++)
	{
		auto hex = pk::get_hex(arr[j]);
		if (!hex.has_unit) continue;
		pk::unit@ dst_unit = pk::get_unit(arr[j]);

		if (!pk::is_enemy(src_unit, dst_unit)) continue;
		//现在成功率里已经有对应特技效果了
		//if (dst_unit.has_skill(특기_화공) or dst_unit.has_skill(특기_화신) or dst_unit.has_skill(특기_통찰) or dst_unit.has_skill(특기_신산)) continue;
		if (ch::has_skill(dst_unit, 特技_谋略)) continue;

		pk::person@ dst_leader = pk::get_person(dst_unit.leader);


		//int chance = 部队妙计_成功率 + 谋略成功率上升 + ch::minZero(src_unit.attr.stat[武将能力_智力] - dst_unit.attr.stat[武将能力_智力]);
		pk::int_bool result0 = cast<pk::func206_t>(pk::get_func(206))(src_unit, src_pos, dst_unit.pos, 妙计_神火计);//int_bool 成功率 是否被看破

		if (result0.second)//看破
		{
			if (pk::is_in_screen(dst_unit.pos))
			{
				pk::create_effect(84, dst_unit.pos);
				pk::play_se(95, dst_unit.pos);
				pk::play_voice(69, dst_leader);
			}
		}
		else if (pk::rand_bool(pk::min(100, result0.first)))//计策成功
		{
			count += 1;//影响部队技计数
			//pk::set_status(dst_unit, src_unit, 部队状态_伪报, 2, true);
			pk::create_fire(dst_unit.pos, pk::rand(2) + 2, dst_unit, true);	//火计	
			pk::add_energy(dst_unit, -神火计减气, true);	//减气
			if (神火计眩晕 && dst_unit.status == 部队状态_通常)
			{
				pk::set_status(dst_unit, src_unit, 部队状态_混乱, pk::rand(2), true);	//混乱
				if (!src_unit.is_player())
					pk::history_log(dst_unit.pos, pk::get_force(dst_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x中了\x1b[2x{}\x1b[0x的\x1b[29x神火计\x1b[0x，指挥系统彻底失灵，陷入混乱", pk::decode(pk::get_name(dst_unit)), pk::decode(pk::get_name(src_unit)))));
				else
					pk::history_log(dst_unit.pos, pk::get_force(dst_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x中了\x1b[2x{}\x1b[0x的\x1b[32x神火计\x1b[0x，指挥系统彻底失灵，陷入混乱", pk::decode(pk::get_name(dst_unit)), pk::decode(pk::get_name(src_unit)))));

			}

			if (pk::is_in_screen(dst_unit.pos))
			{
				ch::say_message(7394, /*p0*/dst_leader, /*p1*/null, /*u0*/null, /*p0_u*/dst_unit);	//好燙！\n你們幹得太過火了！		
			}
		}
		else//计策失败
		{
			if (pk::is_in_screen(dst_unit.pos))
			{
				pk::play_voice(70, dst_leader);
				ch::say_message(7206, /*p0*/dst_leader, /*p1*/null, /*u0*/null, /*p0_u*/dst_unit);//沒用的	
			}
		}

	}

	if (count > 0 && pk::is_in_screen(src_pos))
	{
		if (count > 2 && 开启_作战技能暴击动画) pk::play_se(25);//使用暴击图音效
		if (count > 2 && 开启_作战技能暴击动画) pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/shenhuoji.png", 4);//填写暴击图位置
		pk::play_voice(0x37, src_leader);  // 음성 : 전투_진정
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		switch (pk::rand(2))
		{
		case 0: pk::say(pk::encode("在我的计谋中烧成灰烬!"), src_leader); break;
		case 1: pk::say(pk::encode("尝尝这业火焚天的滋味吧!"), src_leader); break;
		}
	}
}


void global_func_八阵图(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	array<pk::point> arr = pk::range(src_pos, 部队妙计_范围_min, 部队妙计_范围_max);

	int count = 0;

	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst_unit = pk::get_unit(arr[j]);

		if (dst_unit is null) continue;

		pk::person@ dst_leader = pk::get_person(dst_unit.leader);

		if (!pk::is_enemy(src_unit, dst_unit))
		{
			pk::add_energy(dst_unit, 八阵图加气, true);		//加气
			if (dst_unit.status != 部队状态_通常)
			{
				pk::set_status(dst_unit, src_unit, 部队状态_通常, 2, true);	//恢复正常
			}
			if (八阵图重置行动 && dst_unit.action_done == true)
			{
				dst_unit.action_done = false;	//恢复行动
				dst_unit.update();
			}
			if (pk::is_in_screen(dst_unit.pos))
			{
				switch (pk::rand(2))
				{
				case 0: pk::say(pk::encode("八阵图果然厉害!"), dst_leader); break;
				case 1: pk::say(pk::encode("一鼓作气!提升士气!"), dst_leader); break;
				}
			}
		}
		else if (pk::is_enemy(src_unit, dst_unit))
		{
			if (dst_unit.has_skill(특기_통찰) or dst_unit.has_skill(특기_신산)) continue;
			if (ch::has_skill(dst_unit, 特技_谋略)) continue;

			//int chance = 部队妙计_成功率 + 谋略成功率上升 + ch::minZero(src_unit.attr.stat[武将能力_智力] - dst_unit.attr.stat[武将能力_智力]);
			pk::int_bool result0 = cast<pk::func206_t>(pk::get_func(206))(src_unit, src_pos, dst_unit.pos, 妙计_八阵图);//int_bool 成功率 是否被看破

			if (result0.second)//看破
			{
				if (pk::is_in_screen(dst_unit.pos))
				{
					pk::create_effect(84, dst_unit.pos);
					pk::play_se(95, dst_unit.pos);
					pk::play_voice(69, dst_leader);
				}
			}
			else if (pk::rand_bool(pk::min(100, result0.first)))//计策成功
			{
				count += 1;//影响部队技计数
				pk::set_status(dst_unit, src_unit, 部队状态_混乱, 八阵图混乱回合, true);	//混乱
				if (!src_unit.is_player())
					pk::history_log(dst_unit.pos, pk::get_force(dst_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x受困于\x1b[2x{}\x1b[0x施展的\x1b[29x八阵图\x1b[0x，陷入混乱", pk::decode(pk::get_name(dst_unit)), pk::decode(pk::get_name(src_unit)))));
				else
					pk::history_log(dst_unit.pos, pk::get_force(dst_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x受困于\x1b[2x{}\x1b[0x施展的\x1b[32x八阵图\x1b[0x，陷入混乱", pk::decode(pk::get_name(dst_unit)), pk::decode(pk::get_name(src_unit)))));

				if (pk::is_in_screen(dst_unit.pos))
				{
					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode("糟了.. 误入八阵图.."), dst_leader); break;
					case 1: pk::say(pk::encode("没想到世间竟有此等阵法.."), dst_leader); break;
					}
				}
			}
			else//计策失败
			{
				if (pk::is_in_screen(dst_unit.pos))
				{
					pk::play_voice(70, dst_leader);
					ch::say_message(7206, /*p0*/dst_leader, /*p1*/null, /*u0*/null, /*p0_u*/dst_unit);//沒用的	
				}
			}

		}
	}

	if (count > 0 && pk::is_in_screen(src_pos))
	{
		if (count > 2 && 开启_作战技能暴击动画) pk::play_se(25);//使用暴击图音效
		if (count > 2 && 开启_作战技能暴击动画) pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/bazhentu.png", 4);//填写暴击图位置
		pk::play_voice(0x37, src_leader);  // 음성 : 전투_진정
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		switch (pk::rand(2))
		{
		case 0: pk::say(pk::encode("开!休!生!伤!杜!景!死!惊"), src_leader); break;
		case 1: pk::say(pk::encode("无人能避开我的八阵图!"), src_leader); break;
		}
	}
}


void global_func_虚虚实实(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	array<pk::point> arr = pk::range(src_pos, 部队妙计_范围_min, 部队妙计_范围_max);

	int count = 0;

	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst_unit = pk::get_unit(arr[j]);

		if (dst_unit is null or !pk::is_enemy(src_unit, dst_unit)) continue;

		if (dst_unit.has_skill(특기_허실) or dst_unit.has_skill(특기_통찰) or dst_unit.has_skill(특기_신산)) continue;

		if (ch::has_skill(dst_unit, 特技_谋略)) continue;

		pk::person@ dst_leader = pk::get_person(dst_unit.leader);

		//int chance = 部队妙计_成功率 + 谋略成功率上升 + ch::minZero(src_unit.attr.stat[武将能力_智力] - dst_unit.attr.stat[武将能力_智力]);
		int chance = cast<pk::func206_t>(pk::get_func(206))(src_unit, src_pos, dst_unit.pos, 妙计_虚虚实实).first;//int_bool 成功率 是否被看破
		//pk::trace(pk::format("{}的虚虚实实发动几率为{}",pk::get_name(src_unit),chance));
		pk::int_bool result0 = cast<pk::func206_t>(pk::get_func(206))(src_unit, src_pos, dst_unit.pos, 妙计_虚虚实实);//int_bool 成功率 是否被看破

		if (result0.second)//看破
		{
			if (pk::is_in_screen(dst_unit.pos))
			{
				pk::create_effect(84, dst_unit.pos);
				pk::play_se(95, dst_unit.pos);
				pk::play_voice(69, dst_leader);
			}
		}
		else if (pk::rand_bool(pk::min(100, result0.first)))//计策成功
		{
			count += 1;//影响部队技计数
			int 减兵 = int(ch::minZero(src_unit.attr.stat[武将能力_智力] - dst_unit.attr.stat[武将能力_智力]) * 虚虚实实减兵系数);
			减兵 += int(dst_unit.troops * 虚虚实实减兵比例 / 100.f);
			float t = 1.f * 虚虚实实统率基准 / dst_leader.stat[武将能力_统率];	//统率影响兵力損失
			t = pk::min(1.2f, t);
			ch::add_troops(dst_unit, int(-减兵 * t), true, 100); // 减兵

			if (dst_unit.status == 部队状态_通常)
			{
				pk::set_status(dst_unit, src_unit, ch::randint(1, 2), 虚虚实实控制回合, true);	//混乱/伪报
				if (!src_unit.is_player())
					pk::history_log(dst_unit.pos, pk::get_force(dst_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x中了\x1b[2x{}\x1b[0x的妙计-\x1b[29x虚虚实实\x1b[0x，状态陷入异常，并有部分士兵溃逃", pk::decode(pk::get_name(dst_unit)), pk::decode(pk::get_name(src_unit)))));
				else
					pk::history_log(dst_unit.pos, pk::get_force(dst_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x中了\x1b[2x{}\x1b[0x的妙计-\x1b[32x虚虚实实\x1b[0x，状态陷入异常，并有部分士兵溃逃", pk::decode(pk::get_name(dst_unit)), pk::decode(pk::get_name(src_unit)))));

			}


			if (pk::is_in_screen(dst_unit.pos))
			{
				switch (pk::rand(2))
				{
				case 0: pk::say(pk::encode("无法弄清敌军意图.."), dst_leader); break;
				case 1: pk::say(pk::encode("指挥失灵了.."), dst_leader); break;
				}
			}
			if (dst_unit.get_troops() == 0)
			{
				if (pk::is_in_screen(dst_unit.pos)) pk::create_death_effect(dst_unit, src_unit);
				pk::kill(dst_unit, src_unit);//放dst_unit调用的最后，防止空指针
			}
		}
		else//计策失败
		{
			if (pk::is_in_screen(dst_unit.pos))
			{
				pk::play_voice(70, dst_leader);
				ch::say_message(7206, /*p0*/dst_leader, /*p1*/null, /*u0*/null, /*p0_u*/dst_unit);//沒用的	
			}
		}

	}

	if (count > 0 && pk::is_in_screen(src_pos))
	{
		if (count > 2 && 开启_作战技能暴击动画) pk::play_se(25);//使用暴击图音效
		if (count > 2 && 开启_作战技能暴击动画) pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/xuxushishi.png", 4);//填写暴击图位置
		pk::play_voice(0x37, src_leader);  // 음성 : 전투_진정
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		switch (pk::rand(2))
		{
		case 0: pk::say(pk::encode("虚虚实实!无从揣度!"), src_leader); break;
		case 1: pk::say(pk::encode("虚则实之,实则虚之!"), src_leader); break;
		}
	}
}


void global_func_速战固守(pk::point src_pos, pk::unit@ src_unit, pk::person@ src_leader)
{
	array<pk::point> arr = pk::range(src_pos, 部队妙计_范围_min, 部队妙计_范围_max);
	arr.insertLast(src_pos);

	int count = 0;

	for (int j = 0; j < int(arr.length); j++)
	{
		pk::unit@ dst_unit = pk::get_unit(arr[j]);

		if (dst_unit is null) continue;

		pk::person@ dst_leader = pk::get_person(dst_unit.leader);
		pk::hex@ dst_hex = pk::get_hex(arr[j]);

		if (!pk::is_enemy(src_unit, dst_unit))
		{
			pk::add_energy(dst_unit, 速战固守加气, true);		//加气
			if (dst_unit.status != 部队状态_通常)
				pk::set_status(dst_unit, src_unit, 部队状态_通常, 2, true);	//恢复正常
			if (dst_hex.fire_timer > 0)
				dst_hex.fire_timer = 0;		//灭火

			if (pk::is_in_screen(dst_unit.pos))
			{
				switch (pk::rand(2))
				{
				case 0: pk::say(pk::encode("等到敌军精疲力尽!"), dst_leader); break;
				case 1: pk::say(pk::encode("敌军兵粮快没了!"), dst_leader); break;
				}
			}
		}
		else if (pk::is_enemy(src_unit, dst_unit))
		{
			if (dst_unit.has_skill(특기_심모) or dst_unit.has_skill(특기_신산) or dst_unit.has_skill(특기_통찰)) continue;

			if (ch::has_skill(dst_unit, 特技_谋略)) continue;

			//int chance = 部队妙计_成功率 + 谋略成功率上升 + ch::minZero(src_unit.attr.stat[武将能力_智力] - dst_unit.attr.stat[武将能力_智力]);
			pk::int_bool result0 = cast<pk::func206_t>(pk::get_func(206))(src_unit, src_pos, dst_unit.pos, 妙计_速战固守);//int_bool 成功率 是否被看破

			if (result0.second)//看破
			{
				if (pk::is_in_screen(dst_unit.pos))
				{
					pk::create_effect(84, dst_unit.pos);
					pk::play_se(95, dst_unit.pos);
					pk::play_voice(69, dst_leader);
				}
			}
			else if (pk::rand_bool(pk::min(100, result0.first)))//计策成功
			{
				count += 1;//影响部队技计数
				int 减粮 = int(ch::minZero(src_unit.attr.stat[武将能力_智力] - dst_unit.attr.stat[武将能力_智力]) * 速战固守减粮系数);
				减粮 += int(dst_unit.food * 速战固守减粮比例 / 100.f);
				float t = 1.f * 速战固守统率基准 / dst_leader.stat[武将能力_统率];	//统率影响军粮損失
				t = pk::min(1.2f, t);
				pk::add_food(dst_unit, int(-减粮 * t), true); // 减粮
				pk::add_energy(dst_unit, -速战固守减气, true);	//减气
				if (!src_unit.is_player())
					pk::history_log(dst_unit.pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x发动了\x1b[29x速战固守\x1b[0x，\x1b[2x{}\x1b[0x粮道被截，气力下降", pk::decode(pk::get_name(src_unit)), pk::decode(pk::get_name(dst_unit)))));
				else
					pk::history_log(dst_unit.pos, pk::get_force(src_unit.get_force_id()).color, pk::encode(pk::format("\x1b[2x{}\x1b[0x发动了\x1b[32x速战固守\x1b[0x，\x1b[2x{}\x1b[0x粮道被截，气力下降", pk::decode(pk::get_name(src_unit)), pk::decode(pk::get_name(dst_unit)))));
				if (pk::is_in_screen(dst_unit.pos))
				{
					switch (pk::rand(2))
					{
					case 0: pk::say(pk::encode("防守果然严密.."), dst_leader); break;
					case 1: pk::say(pk::encode("糟了.. 兵粮快要用光了.."), dst_leader); break;
					}
				}
			}
			else//计策失败
			{
				if (pk::is_in_screen(dst_unit.pos))
				{
					pk::play_voice(70, dst_leader);
					ch::say_message(7206, /*p0*/dst_leader, /*p1*/null, /*u0*/null, /*p0_u*/dst_unit);//沒用的	
				}
			}
		}
	}

	if (count > 0 && pk::is_in_screen(src_pos))
	{
		if (count > 2 && 开启_作战技能暴击动画) pk::play_se(25);//使用暴击图音效
		if (count > 2 && 开启_作战技能暴击动画) pk::scenario_title_effect(pk::core_dir + "data/media/BaoJi/text/suzhangushou.png", 4);//填写暴击图位置
		pk::play_voice(0x37, src_leader);  // 음성 : 전투_진정
		pk::play_se(0x39, src_pos);
		pk::create_effect(0x3a, src_pos);

		switch (pk::rand(2))
		{
		case 0: pk::say(pk::encode("就是现在, 全军出动!"), src_leader); break;
		case 1: pk::say(pk::encode("持久战, 无人能敌!"), src_leader); break;
		}
	}
}


//	======================================================================================================= 妙计



//	阵法 =======================================================================================================

	//方圓 长蛇 回音 锋矢 八門金鎖 奇門八卦






//	======================================================================================================= 阵法


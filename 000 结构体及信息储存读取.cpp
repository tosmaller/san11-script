// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2022/02/06 # 江东新风 # 部分信息未正确储存 ##
// ## 2022/02/04 # 江东新风 # 尝试修复列传变一样错误 ##
// ## 2021/11/22 # 江东新风 # 加入事件相关记录，修复bug ##
// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/10/28 # 江东新风 # 单港关存活默认值 ##
// ## 2021/10/24 # 江东新风 # 部队伤兵参数 ##
// ## 2021/10/18 # 江东新风 # 事件 曹操南征参数 ##
// ## 2021/09/26 # 江东新风 # 在势力结构体添加协同请求相关参数,新人才选拔系统相关列传参数 ##
// ## 2021/03/21 # 江东新风 # 自动组队bug修复 ##
// ## 2021/03/20 # 江东新风 # 自动组队相关信息 ##
// ## 2021/03/07 # 江东新风 # 新增选拔人才相关参数 ##
// ## 2021/03/05 # 江东新风 # 为人才选拔加入列传信息记录 ##
// ## 2021/02/16 # 江东新风 # 修复默认战马武器异常bug ##
// ## 2021/02/07 # 江东新风 # bug修复 ##
// ## 2021/02/04 # 江东新风 # 武将结构体添加武器及马匹参数 ##
// ## 2021/01/29 # 江东新风 # 针对事件的结构体支持 ##
// ## 2021/01/23 # 江东新风 # 结构体设定内容增加，为金钱投资和请求军粮提供支持 ##
// ## 2021/01/22 # 江东新风 # 结构体相关设定，必须最早加载 ##
namespace 结构体读取与储存
{
	/// KEY index 的类型是 (uint16),所以最高支持65535,
	const int KEY_索引_追加_据点起始 = 0;					// 索引追加建筑结构体 uint32数量为30个，据点共86个。总计2580
	const int KEY_索引_追加_武将起始 = 5000;					// 索引追加武将结构体 uint32数量为10个，武将共1100个。总计11000
	const int KEY_索引_追加_部队起始 = 30000;					// 索引追加部队结构体 uint32数量为10个，部队共1000个。总计10000
	const int KEY_索引_追加_势力起始 = 50000;					// 索引追加势力结构体 uint32数量为20个，势力共46个。总计920
	const int KEY_索引_追加_地名起始 = 55000;					// 索引追加地名结构体 uint32数量为20个，地名共221个。总计4420
	const int KEY_索引_追加_组队起始 = 64000;					// 索引追加自动组队结构体 uint32数量为3个，记录数共300个。总计900
	const int KEY_索引_追加_设定起始 = 65400;					// 索引追加游戏基础设定结构体 uint32数量为135个
	//武将id*3 16*3+兵种8*2 + 频次16
	//使用3个unit32存，给100个位置，那就是300
	//是否有必要加入玩家自定义参数的储存--如果加入，那kre中的更改将失效，直到重开游戏或游戏中改设定
	

	const int KEY = pk::hash("结构体");

	class Main
	{
		Main()
		{
			pk::bind(102, -1, pk::trigger102_t(剧本初始化_结构体_信息读取));
			pk::bind(105, pk::trigger105_t(儲存_结构体_信息储存));
			pk::bind(111, pk::trigger111_t(onTurnStart));
		}
		/*array<array<uint32>> person_ex_info_temp(10, array<uint32>(武将_末, uint32(-1)));
		array<array<uint32>> base_ex_info_temp(10, array<uint32>(据点_末, uint32(-1)));*/
		void 儲存_结构体_信息储存(int file_id) //儲存
		{
			for (int i = 0; i < 据点_末; i++)
			{
				base_ex[i].update(i);
				for (int j = 0; j < (据点结构体_uint32数 - 1); j++)
					pk::store(KEY, (KEY_索引_追加_据点起始 + (i * 据点结构体_uint32数 + j)), base_ex_info_temp[j][i]);
			}
			for (int i = 0; i < 武将_末; i++)
			{
				person_ex[i].update(i);
				for (int j = 0; j < (武将结构体_uint32数 - 1); j++)
					pk::store(KEY, (KEY_索引_追加_武将起始 + (i * 武将结构体_uint32数 + j)), person_ex_info_temp[j][i]);
			}
			for (int i = 0; i < 部队_末; i++)
			{
<<<<<<< HEAD
=======
				//if (i== 1) pk::trace("unit_ex[i].movement_remain" + unit_ex[i].movement_remain + "pk::get_remain_movement(i)" + pk::get_remain_movement(i));
				//if (pk::is_fog_set()) unit_ex[i].movement_remain = pk::get_remain_movement(i);

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				unit_ex[i].update(i);
				for (int j = 0; j < (部队结构体_uint32数 - 1); j++)
					pk::store(KEY, (KEY_索引_追加_部队起始 + (i * 部队结构体_uint32数 + j)), unit_ex_info_temp[j][i]);
			}

			for (int i = 0; i < 势力_末; i++)
			{
				force_ex[i].update(i);
				for (int j = 0; j < (势力结构体_uint32数 - 1); j++)
					pk::store(KEY, (KEY_索引_追加_势力起始 + (i * 势力结构体_uint32数 + j)), force_ex_info_temp[j][i]);
			}

			for (int i = 0; i < 地名_末; i++)//地名数量221，所以到220结束
			{
				special_ex[i].update(i);
				for (int j = 0; j < (地名结构体_int32数 - 1); j++)
					pk::store(KEY, (KEY_索引_追加_地名起始 + (i * 地名结构体_int32数 + j)), special_info_temp[j][i]);
			}

			for (int i = 0; i < 300; i++)
			{
				autoarmy_ex[i].update(i);
				for (int j = 0; j < (组队结构体_int32数 - 1); j++)
					pk::store(KEY, (KEY_索引_追加_组队起始 + (i * 组队结构体_int32数 + j)), autoarmy_ex_info_temp[j][i]);
			}
<<<<<<< HEAD
			pk::trace("nanzheng1:" + setting_ex.eve_happened[5]);
			setting_ex.update();
			pk::trace("nanzheng2:"+ setting_ex.eve_happened[5]);
=======
			//pk::trace("nanzheng1:" + setting_ex.eve_happened[5]);
			setting_ex.update();
			//pk::trace("nanzheng2:"+ setting_ex.eve_happened[5]);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			for (int j = 0; j < (设定结构体_uint32数 - 1); j++)
			{
				pk::store(KEY, (KEY_索引_追加_设定起始 + (j)), setting_ex_info_temp[j]);
			}
				

		}

		void 剧本初始化_结构体_信息读取()
		{
			//重新开始游戏时，初始化数据
			if (!pk::get_scenario().loaded)
			{
				//pk::trace("first day");
				for (int i = 0; i < 据点_末; ++i)
				{
					//BaseInfo base_t(i);
					//base_ex[i].
					base_ex[i].mobilize_done = false;
					if (i >= 城市_末) base_ex[i].public_order = 85;
					if (i < 城市_末)
					{
<<<<<<< HEAD


=======
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
						//base_t.can_inspections = false;
						//base_t.can_drill = false;
						//base_t.can_recruit = false;
						//base_t.can_reward = false;
						//base_t.can_transport = false;
						base_ex[i].auto_sequence = 1;
						base_ex[i].inspections_std = 95;
						base_ex[i].drill_std = 95;
						base_ex[i].recruit_std_foodtroop = 150;
						base_ex[i].recruit_std_porder = 95;
						base_ex[i].recruit_std_gold = 2500;
						base_ex[i].reward_std = 90;
<<<<<<< HEAD
=======
						base_ex[i].harvest_bonus = 0;
						base_ex[i].revenue_bonus = 0;
						base_ex[i].recruit_person = -1;//征兵执行官
						base_ex[i].inspections_person = -1;//巡查执行官
						base_ex[i].drill_person = -1;//训练执行官
						base_ex[i].produce_person = -1;//生产执行官
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					}
					for (int j = 0; j < 势力_末; ++j)
					{
						base_ex[i].spy_lv[j] = 0;
						base_ex[i].send_spy[j] = false;
						base_ex[i].enhance_spy[j] = false;
						base_ex[i].can_build[j] = true;
					}

					//base_t.update(i);
					//if (i == 城市_北平) pk::trace(pk::format("曹操势力城市北平是否禁止拆建：{}", base_t.can_build[0]));
				}

				for (int i = 0; i < 武将_末; ++i)
				{
					//personinfo person_t(i);
					//personinfo@ person_t = @person_ex[i];
					person_ex[i].float_command = 0;
					person_ex[i].float_strength = 0;
					person_ex[i].float_wisdom = 0;
					person_ex[i].float_politic = 0;
					person_ex[i].float_charm = 0;
					person_ex[i].order = 255;
					person_ex[i].mission_time = 0;
					person_ex[i].mission_target1 = 65535;
					person_ex[i].mission_target2 = 65535;
					person_ex[i].horse_id = 255;
					person_ex[i].weapon_id = 255;
					person_ex[i].discovered_by = -1;
					person_ex[i].int_force = -1;
					person_ex[i].spec_id = -1;
					person_ex[i].select_history_person_id = 255;
					//person_t.update(i);
				}

				for (int i = 0; i < 地名_末; ++i)
				{
					//specialinfo spec_t(i);
					special_ex[i].person = -1;
					special_ex[i].troops = 15000;
					special_ex[i].gold = 15000;
					special_ex[i].food = 15000;
					//spec_t.update(i);
				}

				for (int i = 0; i < 势力_末; ++i)
				{
					force_ex[i].alliance_id = 255;
					force_ex[i].attack_delay = 255;
				}
				//剧本初始化时重置数据
//
				//settinginfo set0();
				setting_ex.set_port_gate_survive = false;
				setting_ex.enable_port_gate_survive = false;
				for (int i = 0; i < 128; ++i)
				{
					setting_ex.eve_happened[i] = false;
				}
				for (int i = 0; i < 4; ++i)
				{
					setting_ex.alliance_target_id[i] = 255;
					setting_ex.alliance_leader_id[i] = 255;
					for (int j = 0; j < 6; ++j) setting_ex.alliance_target_base[i][j] = 255;
				}
				//set0.update();
			}
			//pk::trace("读取结构体");
			//pk::get_eve(60);
			//pk::set_eve(60,true);
			if (pk::get_scenario().loaded)
			{
				for (int i = 0; i < 据点_末; i++)
				{
					for (int j = 0; j < (据点结构体_uint32数 - 1); j++)
						base_ex_info_temp[j][i] = uint32(pk::load(KEY, (KEY_索引_追加_据点起始 + (i * 据点结构体_uint32数 + j)), 0));
					BaseInfo base_t(i);
					base_ex[i] = base_t;
				}
				for (int i = 0; i < 武将_末; i++)
				{
					for (int j = 0; j < (武将结构体_uint32数 - 1); j++)
						person_ex_info_temp[j][i] = uint32(pk::load(KEY, (KEY_索引_追加_武将起始 + (i * 武将结构体_uint32数 + j)), 0));

					//借用位置写个武将装备马匹的设定,读档时设置
					personinfo person_t(i);
					person_ex[i] = person_t;
					//pk::trace(pk::format("武将id：{}，马：{}，武器：{}", i, person_t.weapon_id, person_t.horse_id));
					if (person_t.weapon_id != 255) pk::set_person_weapon(i, person_t.weapon_id);
					if (person_t.horse_id != 255) pk::set_person_horse(i, person_t.horse_id);

				}
				for (int i = 0; i < 部队_末; i++)
				{
					for (int j = 0; j < (部队结构体_uint32数 - 1); j++)
						unit_ex_info_temp[j][i] = uint32(pk::load(KEY, (KEY_索引_追加_部队起始 + (i * 部队结构体_uint32数 + j)), 0));
					unitinfo unit_t(i);
					unit_ex[i] = unit_t;
<<<<<<< HEAD
=======
					
					//if (pk::is_fog_set()) pk::set_remain_movement(i, unit_ex[i].movement_remain);
					//if (i == 1) pk::trace("load unit_ex[i].movement_remain" + unit_ex[i].movement_remain + "pk::get_remain_movement(i)" + pk::get_remain_movement(i));
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
				}
				for (int i = 0; i < 势力_末; i++)
				{
					for (int j = 0; j < (势力结构体_uint32数 - 1); j++)
						force_ex_info_temp[j][i] = uint32(pk::load(KEY, (KEY_索引_追加_势力起始 + (i * 势力结构体_uint32数 + j)), 0));
					forceinfo force_t(i);
					force_ex[i] = force_t;
					for (int info_index = 0; info_index < 3; ++info_index)
					{
						if (force_t.ask_base_id[info_index] != -1) pk::trace(pk::format("协同请求：我方据点id:{},目标城市id：{},异族势力:{},预计抵达天数:{}", pk::get_new_base_name(force_t.ask_base_id[info_index]), pk::get_new_base_name(force_t.aim_city_id[info_index]), force_t.yizu_force_id[info_index], force_t.wait_timer[info_index]));
					}
				}
				for (int i = 0; i < 地名_末; i++)
				{
					for (int j = 0; j < (地名结构体_int32数 - 1); j++)
						special_info_temp[j][i] = int32(pk::load(KEY, (KEY_索引_追加_地名起始 + (i * 地名结构体_int32数 + j)), 0));
					specialinfo special_t(i);
					special_ex[i] = special_t;
				}
				for (int i = 0; i < 300; i++)
				{
					for (int j = 0; j < (组队结构体_int32数 - 1); j++)
						autoarmy_ex_info_temp[j][i] = int32(pk::load(KEY, (KEY_索引_追加_组队起始 + (i * 组队结构体_int32数 + j)), 0));
					autoarmyinfo autoarmy_t(i);
					autoarmy_ex[i] = autoarmy_t;
				}
				for (int j = 0; j < (设定结构体_uint32数 - 1); j++)
				{
					setting_ex_info_temp[j] = uint32(pk::load(KEY, (KEY_索引_追加_设定起始 + (j)), 0));

				}
				settinginfo setting_t();
				setting_ex = setting_t;
				//setting_ex.eve_happened[5] = false;
					
			}


			//测试效果，记得删除
			//pk::set_person_weapon(武将_沙摩柯, 1);
			//pk::set_person_horse(武将_沙摩柯, 1);
		}

		void onTurnStart(pk::force@ force)
		{

			for (int i = 0; i < 据点_末; ++i)
			{
				int force_id = pk::get_building(i).get_force_id();
				if (force_id == force.get_id())
				{
					base_ex[i].mobilize_done = false;
					base_ex[i].repair_done = false;
					base_ex[i].enlist_done = false;
					base_ex[i].perturb_done = false;
					base_ex[i].misinform_done = false;
					base_ex[i].fire_done = false;
					base_ex[i].beguile_done = false;
					base_ex[i].raid_done = false;
					base_ex[i].farmer_count = 3;
					if (base_ex[i].provoke_cd > 0) base_ex[i].provoke_cd -= 1;
				}
			}
		}
	}


	Main main;
}
const int 地名_末 = 221;
const int 据点结构体_uint32数 = 30;
const int 武将结构体_uint32数 = 10;
const int 部队结构体_uint32数 = 10;
const int 势力结构体_uint32数 = 40;
const int 地名结构体_int32数 = 20;
const int 组队结构体_int32数 = 3;
const int 设定结构体_uint32数 = 135;
array<array<uint32>> base_ex_info_temp(据点结构体_uint32数, array<uint32>(据点_末, uint32(0)));
array<array<uint32>> person_ex_info_temp(武将结构体_uint32数, array<uint32>(武将_末, uint32(0)));
array<array<uint32>> unit_ex_info_temp(部队结构体_uint32数, array<uint32>(部队_末, uint32(0)));
array<array<uint32>> force_ex_info_temp(势力结构体_uint32数, array<uint32>(势力_末, uint32(0)));
array<array<int32>> special_info_temp(地名结构体_int32数, array<int32>(地名_末, int32(0)));
array<uint32> setting_ex_info_temp(设定结构体_uint32数, uint32(0));
array<array<int32>> autoarmy_ex_info_temp(组队结构体_int32数, array<int32>(300, int32(0)));

array<BaseInfo> base_ex(据点_末);
array<personinfo> person_ex(武将_末);
array<unitinfo> unit_ex(部队_末);
array<forceinfo> force_ex(武将_末);
array<specialinfo> special_ex(地名_末);
settinginfo setting_ex;
array<autoarmyinfo> autoarmy_ex(300);

//存档时将结构体转为数组再存
//读档时将数组转换为结构体读取

///struct 和 class没有本质区别，只是class默认类外不可访问。不过pk2.2class是可以类外访问的
//将多个信息组合成两个int64
class personinfo
{
	uint16 person_id_t;
	int8 float_command;    //统率浮动，实际用int4存，-8至+7--搞不定，还是就用int8存吧
	int8 float_strength;   //武力浮动，实际用int4存，-8至+7
	int8 float_wisdom;     //智力浮动，实际用int4存，-8至+7
	int8 float_politic;     //政治浮动，实际用int4存，-8至+7
	int8 float_charm;     //魅力浮动，实际用int4存，-8至+7
	uint8 order;     //任务种类，说明具体执行的是什么任务 0-255
	uint8 mission_time;     //执行某任务的计数器
	uint16 mission_target1; //任务目标的id，65535
	uint16 mission_target2; //任务目标的id，65535
	uint8 horse_id = 255;
	uint8 weapon_id = 255;

	uint8 career;//之前的职业

	uint8 person_type = 0;		//武将类型：0-一般，1-提拔，2-提拔(预留)，4-提拔(预留)，16-农民兵，32-异族
	uint8 talent_type = 0;			//（实际只需要4位来存）人才类型：帅才猛将之类的
	uint8 appeared_reason = 0;		//（实际只需要4位来存）登场原因：0-无，1-提拔，2-请求仕官，3-受赏识，4-后代，5-临时
	uint8 _flag_0;				//备用
	//uint8 employ_year = 0;		（这个可以用登场年表示）提拔时间--暂限于武将选拔
	int16 discovered_by = -1; 		//提拔者
	int8 int_force = -1;			//初次仕官势力
	int8 appeared_city = -1;		//出场城市/建筑
	int kill_num = 0;				//杀敌数---考虑到反复计算问题，还需要改函数
	int unit_kill_num = 0;			//所在部队杀敌数
	int16 spec_id = -1;             //封分的领地
	uint8 select_history_person_id;//标志选拔历史武将的id
	/*在300文件定义的暂存数组，读档时将存档加载入数组，
	array<array<uint32>> person_ex_info_temp(10, array<uint32>(武将_末, uint32(-1)));
	array<array<uint32>> base_ex_info_temp(10, array<uint32>(据点_末, uint32(-1)));
	*/
	//初始化
	personinfo(int person_id)
	{
		//pk::trace(pk::format("start initial,time:{}",pk::time()));
		person_id_t = person_id;
		//fromInt32_0(person_ex_info_temp[0][person_id]);
		//fromInt32_1(person_ex_info_temp[1][person_id]);
		//fromInt32_2(person_ex_info_temp[2][person_id]);
		get_info(person_id);
		//pk::trace(pk::format("initial finished,time:{}", pk::time()));
		//if (person_id == 289) pk::trace(pk::format("int,武将{}：统率{}，武力{}，智力{}，政治{}，魅力{}", person_id,float_command, float_strength, float_wisdom, float_politic, float_charm));
	}

	//下面这种写法用于定义全局变量，先声明，然后用get_info获取信息后使用
	personinfo() {}

	void get_info(int person_id)
	{
		fromInt32_0(person_ex_info_temp[0][person_id]);
		fromInt32_1(person_ex_info_temp[1][person_id]);
		fromInt32_2(person_ex_info_temp[2][person_id]);
		fromInt32_3(person_ex_info_temp[3][person_id]);
		fromInt32_4(person_ex_info_temp[4][person_id]);
		fromInt32_5(person_ex_info_temp[5][person_id]);
		fromInt32_6(person_ex_info_temp[6][person_id]);
	}

	void update(int person_id)
	{
		//pk::trace(pk::format("start update,time:{}", pk::time()));
		//if (person_id == 289)  pk::trace(pk::format("update before：武将{}。统率{}，武力{}，智力{}，政治{}，魅力{}", person_id, float_command, float_strength, float_wisdom, float_politic, float_charm));
		person_ex_info_temp[0][person_id] = toInt32_0();
		person_ex_info_temp[1][person_id] = toInt32_1();
		person_ex_info_temp[2][person_id] = toInt32_2();
		person_ex_info_temp[3][person_id] = toInt32_3();
		person_ex_info_temp[4][person_id] = toInt32_4();
		person_ex_info_temp[5][person_id] = toInt32_5();
		person_ex_info_temp[6][person_id] = toInt32_6();
		//pk::trace(pk::format("update finished,time:{}", pk::time()));
		//if (person_id == 289) pk::trace(pk::format("update after：武将{}。统率{}，武力{}，智力{}，政治{}，魅力{}", person_id, float_command, float_strength, float_wisdom, float_politic, float_charm));
	}

	//储存的时候应是PersonInfo.toInt32_0,作为一个单位储存
	uint32 toInt32_0(void)
	{
		/*
		uint8 float_command_t = int8(pk::max(pk::min(float_command, 7), -8));
		if (float_command < 0) float_command_t ^= 240;
		uint8 float_strength_t = int8(pk::max(pk::min(float_strength, 7), -8));
		if (float_strength < 0) float_strength_t ^= 240;
		uint8 float_wisdom_t = int8(pk::max(pk::min(float_wisdom, 7), -8));
		if (float_wisdom < 0) float_wisdom_t ^= 240;
		uint8 float_politic_t = int8(pk::max(pk::min(float_politic, 7), -8));
		if (float_politic < 0) float_politic_t ^= 240;
		uint8 float_charm_t = int8(pk::max(pk::min(float_charm, 7), -8));
		if (float_charm < 0) float_charm_t ^= 240;
		//if (person_id_t == 289)pk::trace(pk::format("float_command_t:{}", float_command_t));
		//pk::trace(pk::format("toInt32：统率{}，武力{}，智力{}，政治{}，魅力{}", float_command, float_strength, float_wisdom, float_politic, float_charm));
		uint32 x = 0;
		x = float_command_t + (float_strength_t << 4) + (float_wisdom_t << 8) + (float_politic_t << 12) + (float_charm_t << 16);*/
		//这是winapi支持的强行转换，将int8转成uint8类型，但实际储存的东西完全不变。如：1111 1111（-1）变成uint8后就是1111 1111（255)
		uint8 float_command_t = float_command;
		uint8 float_strength_t = float_strength;
		uint8 float_wisdom_t = float_wisdom;
		uint8 float_politic_t = float_politic;
		uint32 x = 0;
		x = float_command_t + (float_strength_t << 8) + (float_wisdom_t << 16) + (float_politic_t << 24);
		//if (person_id_t == 289) pk::trace(pk::format("toInt32,储存前x{}：武将：{},统率{}，武力{}，智力{}，政治{}，魅力{}", x, person_id_t, float_command, float_strength, float_wisdom, float_politic, float_charm));
		return x;
	}
	uint32 toInt32_1(void)
	{
		uint8 float_charm_t = float_charm;
		uint32 x = float_charm + (order << 8) + (mission_time << 16);
		return x;
	}
	uint32 toInt32_2(void)
	{
		uint32 x = mission_target1 + (mission_target2 << 16);
		return x;
	}
	uint32 toInt32_3(void)
	{
		uint32 x = horse_id + (weapon_id << 8) + (select_history_person_id << 16);
		return x;
	}

	uint32 toInt32_4(void)
	{
		uint32 x = discovered_by + (int_force << 16) + (appeared_city << 24);
		return x;
	}

	uint32 toInt32_5(void)
	{
		uint32 x = person_type + (talent_type << 8) + (appeared_reason << 16) + (_flag_0 << 24);
		return x;
	}

	uint32 toInt32_6(void)
	{
		uint32 x = spec_id;
		return x;
	}

	//读取后解析
	void fromInt32_0(uint32 x)
	{
		//pk::trace(pk::format("fromInt64：id_t：{}", id_t));
		//uint8 float_command_t = ((x << 28) >> 28) ;
		//uint8 float_strength_t = ((x << 24) >> 28);
		//uint8 float_wisdom_t = ((x << 20) >> 28);
		//uint8 float_politic_t = ((x << 16) >> 28);
		//uint8 float_charm_t = ((x << 12) >> 28);
		float_command = x;
		float_strength = ((x << 16) >> 24);
		float_wisdom = ((x << 8) >> 24);
		float_politic = x >> 24;
		//float_charm = ((x << 12) >> 28);
		//float_command = float_command_t;
		//float_strength = float_strength_t;
		//float_wisdom = float_wisdom_t;
		//if (person_id_t == 289 ) pk::trace(pk::format("fromInt32,x{}：武将：{},统率{}，武力{}，智力{}，政治{}，魅力{}", x,person_id_t, float_command, float_strength, float_wisdom, float_politic, float_charm));
	}

	void fromInt32_1(uint32 x)
	{
		float_charm = x;
		order = ((x << 16) >> 24);
		mission_time = ((x << 8) >> 24);
	}

	void fromInt32_2(uint32 x)
	{
		mission_target1 = ((x << 16) >> 16);
		mission_target2 = (x >> 16);
	}

	void fromInt32_3(uint32 x)
	{
		horse_id = x;
		weapon_id = ((x << 16) >> 24);
		select_history_person_id = ((x << 8) >> 24);
		// = ((x << 8) >> 24);
		//int_force = x >> 24;
	}

	void fromInt32_4(uint32 x)
	{
		if (x == 0)
		{
			discovered_by = -1;
			int_force = -1;
			appeared_city = -1;
		}
		else
		{
			discovered_by = x;
			int_force = ((x << 8) >> 24);
			appeared_city = ((x << 0) >> 24);
		}
	}

	void fromInt32_5(uint32 x)
	{
		person_type = x;
		talent_type = ((x << 16) >> 24);
		appeared_reason = ((x << 8) >> 24);
		_flag_0 = ((x << 0) >> 24);
	}

	void fromInt32_6(uint32 x)
	{
		spec_id = ((x << 16) >> 16);
	}
}

class BaseInfo
{
	
	uint32 population;//人口
	uint32 mil_pop_all;//总兵役人口
	uint32 mil_pop_av;//可用兵役人口
	uint32 wounded;//伤兵
	uint8 return_timer;//回归计时器--此设定需游戏中说明
	uint32 return_pop;//待返回人口
	bool return_buf;
	//bool can_build;//是否可拆建
	bool recruit_done;//是否已征兵-港关
	bool inspections_done;//是否已巡察-港关
	bool can_inspections;//巡察许可-目前限城市
	bool can_drill;//训练许可-目前限城市，可扩展
	bool can_recruit;//征兵许可-目前限城市
	bool can_reward;//褒奖许可-目前限城市，可扩展
	bool can_transport;//运输许可-目前限城市，可扩展
<<<<<<< HEAD
=======
	array<bool> can_produce(兵种_末, false);//枪戟弩马兵器水军
	int16 recruit_person;//征兵执行官
	int16 inspections_person;//巡查执行官
	int16 drill_person;//训练执行官
	int16 produce_person;//生产执行官

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	uint8 auto_sequence;//自动化顺序
	uint8 inspections_std;//巡察标准
	uint8 drill_std;//训练标准
	uint8 recruit_std_foodtroop;//征兵标准，粮兵比
	uint8 recruit_std_porder;//征兵标准，治安
	uint32 recruit_std_gold;//征兵标准，金
	uint8 reward_std;//褒赏标准，忠诚
	//array<bool> has_spy(势力_末, false);////判断是否有细作的专门数组，需要获取势力id，需要两个uint32
	//0代表无间谍，1-5逐渐开放信息？。或者用uint4？0代表无，1-15代表等级，占5.875个int32
	array<uint8> spy_lv(势力_末, 0);////如果用这个判断间谍等级的话，需要16个uint32，或者限制数字小于16，则本质上是uint4，这样只用8
	array<bool> send_spy(势力_末, false);//根据势力区分的是否向该城市派驻间谍
	array<bool> enhance_spy(势力_末, false);//根据势力区分的是否向该城市发布加强间谍信息
	array<bool> can_build(势力_末, false);//根据势力区分的是否可以拆建的信息
	uint8 public_order;//治安，也就港关有用
	uint8 anti_rumor;//防流言计数

	bool mobilize_done;//紧急动员完成
	bool repair_done;//城墙维修完成
	bool enlist_done;//募兵完成
	bool perturb_done;//据点计略扰乱
	bool misinform_done;//据点计略伪报
	bool fire_done;//据点计略火计
	bool beguile_done;//据点计略攻心
	bool raid_done;//据点计略攻心
	bool reinforce_done;//据点计略援军

	uint8 farmer_count;//一回合可以进行农民兵召唤的次数

	uint8 provoke_cd;//被挑衅的cd,每回合自动减少，被挑衅时设置cd，据点所属变化时清零
<<<<<<< HEAD

=======
	uint16 revenue_bonus;//金收入附加值--通过事件获得
	uint16 harvest_bonus;//粮收入附加值--通过事件获得
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	/*在300文件定义的暂存数组，读档时将存档加载入数组，
	array<array<uint32>> person_ex_info_temp(10, array<uint32>(武将_末, uint32(-1)));
	array<array<uint32>> base_ex_info_temp(10, array<uint32>(据点_末, uint32(-1)));
	*/
	//初始化
	BaseInfo(int base_id)
	{
		get_info(base_id);
	}

	BaseInfo(){}

	void get_info(int base_id)
	{
		fromInt32_0(base_ex_info_temp[0][base_id]);
		fromInt32_1(base_ex_info_temp[1][base_id]);
		fromInt32_2(base_ex_info_temp[2][base_id]);
		fromInt32_3(base_ex_info_temp[3][base_id]);
		fromInt32_4(base_ex_info_temp[4][base_id]);
		fromInt32_5(base_ex_info_temp[5][base_id]);
		fromInt32_6(base_ex_info_temp[6][base_id]);
		fromInt32_7(base_ex_info_temp[7][base_id]);
		fromInt32_8(base_ex_info_temp[8][base_id]);
		fromInt32_9(base_ex_info_temp[9][base_id]);
		fromInt32_10(base_ex_info_temp[10][base_id]);
		fromInt32_11(base_ex_info_temp[11][base_id]);
		fromInt32_12(base_ex_info_temp[12][base_id]);
		fromInt32_13(base_ex_info_temp[13][base_id]);
		fromInt32_14(base_ex_info_temp[14][base_id]);
		fromInt32_15(base_ex_info_temp[15][base_id]);
		fromInt32_16(base_ex_info_temp[16][base_id]);
		fromInt32_17(base_ex_info_temp[17][base_id]);
		fromInt32_18(base_ex_info_temp[18][base_id]);
		fromInt32_19(base_ex_info_temp[19][base_id]);
		fromInt32_20(base_ex_info_temp[20][base_id]);
		fromInt32_21(base_ex_info_temp[21][base_id]);
<<<<<<< HEAD
=======
		fromInt32_22(base_ex_info_temp[22][base_id]);
		fromInt32_23(base_ex_info_temp[23][base_id]);
		fromInt32_24(base_ex_info_temp[24][base_id]);
		fromInt32_25(base_ex_info_temp[25][base_id]);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	}

	//bool equal(const BaseInfo&, bool both = true);

	void update(int base_id)
	{
		base_ex_info_temp[0][base_id] = toInt32_0();
		base_ex_info_temp[1][base_id] = toInt32_1();
		base_ex_info_temp[2][base_id] = toInt32_2();
		base_ex_info_temp[3][base_id] = toInt32_3();
		base_ex_info_temp[4][base_id] = toInt32_4();
		base_ex_info_temp[5][base_id] = toInt32_5();
		base_ex_info_temp[6][base_id] = toInt32_6();
		base_ex_info_temp[7][base_id] = toInt32_7();
		base_ex_info_temp[8][base_id] = toInt32_8();
		base_ex_info_temp[9][base_id] = toInt32_9();
		base_ex_info_temp[10][base_id] = toInt32_10();
		base_ex_info_temp[11][base_id] = toInt32_11();
		base_ex_info_temp[12][base_id] = toInt32_12();
		base_ex_info_temp[13][base_id] = toInt32_13();
		base_ex_info_temp[14][base_id] = toInt32_14();
		base_ex_info_temp[15][base_id] = toInt32_15();
		base_ex_info_temp[16][base_id] = toInt32_16();
		base_ex_info_temp[17][base_id] = toInt32_17();
		base_ex_info_temp[18][base_id] = toInt32_18();
		base_ex_info_temp[19][base_id] = toInt32_19();
		base_ex_info_temp[20][base_id] = toInt32_20();
		base_ex_info_temp[21][base_id] = toInt32_21();
<<<<<<< HEAD
=======
		base_ex_info_temp[22][base_id] = toInt32_22();
		base_ex_info_temp[23][base_id] = toInt32_23();
		base_ex_info_temp[24][base_id] = toInt32_24();
		base_ex_info_temp[25][base_id] = toInt32_25();
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	}

	//储存的时候应是PersonInfo.toInt32_0,作为一个单位储存
	uint32 toInt32_0(void)
	{
		return population;
	}
	uint32 toInt32_1(void)
	{
		uint8 can_recruit_t = can_recruit ? 1 : 0;//改成按势力区分后，此位置暂时作废，以后有需要填入
		uint8 recruit_done_t = recruit_done ? 1 : 0;
		uint8 inspections_done_t = inspections_done ? 1 : 0;
		uint8 can_inspections_t = can_inspections ? 1 : 0;
		uint8 can_drill_t = can_drill ? 1 : 0;
		uint8 can_reward_t = can_reward ? 1 : 0;
		uint8 can_transport_t = can_transport ? 1 : 0;
		uint8 return_buf_t = return_buf ? 1 : 0;//人口返回buf
<<<<<<< HEAD
=======

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		//pk::trace(pk::format("内部，转为数组，巡察：{}，训练：{}，征兵{}，褒奖：{}，运输：{}", can_inspections, can_drill, can_recruit, can_reward, can_transport));
		uint32 x = can_recruit_t + (recruit_done_t << 1) + (inspections_done_t << 2) + (can_inspections_t << 3) + (can_drill_t << 4) + (can_reward_t << 5) + (can_transport_t << 6) + (return_buf_t << 7) + (auto_sequence << 8) + (inspections_std << 16) + (drill_std << 24);
		return x;
	}

	uint32 toInt32_2(void)
	{
		uint32 x = recruit_std_foodtroop + (recruit_std_porder << 8) + (reward_std << 16) + (return_timer << 24);//return_timer
		return x;
	}

	uint32 toInt32_3(void)
	{
		return recruit_std_gold;
	}

	uint32 toInt32_4(void)
	{
		uint32 x = 0;
		for (int force_id = 0; force_id < 8; ++force_id)
		{
			uint8 temp = spy_lv[force_id];
			if (temp > 15) temp = 15;
			x += temp << (force_id * 4);
		}
		return x;
	}

	uint32 toInt32_5(void)
	{
		uint32 x = 0;
		for (int force_id = 8; force_id < 16; ++force_id)
		{
			uint8 temp = spy_lv[force_id];
			if (temp > 15) temp = 15;
			x += temp << ((force_id - 8) * 4);
		}
		return x;
	}

	uint32 toInt32_6(void)
	{
		uint32 x = 0;
		for (int force_id = 16; force_id < 24; ++force_id)
		{
			uint8 temp = spy_lv[force_id];
			if (temp > 15) temp = 15;
			x += temp << ((force_id - 16) * 4);
		}
		return x;
	}

	uint32 toInt32_7(void)
	{
		uint32 x = 0;
		for (int force_id = 24; force_id < 32; ++force_id)
		{
			uint8 temp = spy_lv[force_id];
			if (temp > 15) temp = 15;
			x += temp << ((force_id - 24) * 4);
		}
		return x;
	}

	uint32 toInt32_8(void)
	{
		uint32 x = 0;
		for (int force_id = 32; force_id < 40; ++force_id)
		{
			uint8 temp = spy_lv[force_id];
			if (temp > 15) temp = 15;
			x += temp << ((force_id - 32) * 4);
		}
		return x;
	}

	uint32 toInt32_9(void)
	{
		uint32 x = 0;
		for (int force_id = 40; force_id < 势力_末; ++force_id)
		{
			uint8 temp = spy_lv[force_id];
			if (temp > 15) temp = 15;
			x += temp << ((force_id - 40) * 4);
		}
		return x;
	}
	uint32 toInt32_10(void)
	{
		uint32 x = 0;
		for (int force_id = 0; force_id < 32; ++force_id)
		{
			uint8 temp = can_build[force_id] ? 1 : 0;
			x += temp << force_id;
		}
		return x;
	}

	uint32 toInt32_11(void)
	{
		uint32 x = 0;
		for (int force_id = 32; force_id < 势力_末; ++force_id)
		{
			uint8 temp = can_build[force_id] ? 1 : 0;
			x += temp << (force_id - 32);
		}
		return x;
	}
	uint32 toInt32_12(void)
	{
		uint32 x = 0;
		for (int force_id = 0; force_id < 32; ++force_id)
		{
			uint8 temp = send_spy[force_id] ? 1 : 0;
			x += temp << force_id;
		}
		return x;
	}

	uint32 toInt32_13(void)
	{
		uint32 x = 0;
		for (int force_id = 32; force_id < 势力_末; ++force_id)
		{
			uint8 temp = send_spy[force_id] ? 1 : 0;
			x += temp << (force_id - 32);
		}
		return x;
	}
	uint32 toInt32_14(void)
	{
		uint32 x = 0;
		for (int force_id = 0; force_id < 32; ++force_id)
		{
			uint8 temp = enhance_spy[force_id] ? 1 : 0;
			x += temp << force_id;
		}
		return x;
	}

	uint32 toInt32_15(void)
	{
		uint32 x = 0;
		for (int force_id = 32; force_id < 势力_末; ++force_id)
		{
			uint8 temp = enhance_spy[force_id] ? 1 : 0;
			x += temp << (force_id - 32);
		}
		return x;
	}

	uint32 toInt32_16(void)
	{
		return mil_pop_all;
	}

	uint32 toInt32_17(void)
	{
		return mil_pop_av;
	}

	uint32 toInt32_18(void)
	{
		return wounded;
	}

	uint32 toInt32_19(void)
	{
		return return_pop;
	}

	uint32 toInt32_20(void)
	{
		uint32 x = public_order + (anti_rumor << 8) + (farmer_count << 16) + (provoke_cd << 24);//return_timer
		return x;
	}
	//mobilize_done
	uint32 toInt32_21(void)
	{
		uint8 mobilize_done_t = mobilize_done ? 1 : 0;//改成按势力区分后，此位置暂时作废，以后有需要填入
		uint8 repair_done_t = repair_done ? 1 : 0;
		uint8 enlist_done_t = enlist_done ? 1 : 0;
		uint8 perturb_done_t = perturb_done ? 1 : 0;
		uint8 misinform_done_t = misinform_done ? 1 : 0;
		uint8 fire_done_t = fire_done ? 1 : 0;
		uint8 beguile_done_t = beguile_done ? 1 : 0;
		uint8 raid_done_t = raid_done ? 1 : 0;
		uint8 reinforce_done_t = reinforce_done ? 1 : 0;
		uint32 x = mobilize_done_t + (repair_done_t << 1) + (enlist_done_t << 2) + (perturb_done_t << 3) + (misinform_done_t << 4) + (fire_done_t << 5) + (beguile_done_t << 6) + (raid_done_t << 7) + (mobilize_done_t << 8) + (reinforce_done_t << 9);
		return x;
	}
<<<<<<< HEAD
=======

	uint32 toInt32_22(void)
	{
		uint32 x = revenue_bonus + (harvest_bonus << 16);//
		return x;
	}

	uint32 toInt32_23(void)
	{
		array<uint8> can_produce_t(兵种_末, 0);
		for (int i = 0; i < 兵种_末; ++i)
		{
			can_produce_t[i] = can_produce[i] ? 1 : 0;
		}
		uint32 x = can_produce_t[0] + (can_produce_t[1] << 1) + (can_produce_t[2] << 2) + (can_produce_t[3] << 3) + (can_produce_t[4] << 4) + (can_produce_t[5] << 5);
		return x;
	}

	uint32 toInt32_24(void)
	{
		uint32 x = recruit_person + (inspections_person << 16);//
		return x;
	}

	uint32 toInt32_25(void)
	{
		uint32 x = drill_person + (produce_person << 16);//
		return x;
	}

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	//读取后解析
	void fromInt32_0(uint32 x)
	{
		population = x;
	}

	void fromInt32_1(uint32 x)
	{
		can_recruit = (((x << 31) >> 31) == 1);
		recruit_done = (((x << 30) >> 31) == 1);
		inspections_done = (((x << 29) >> 31) == 1);
		can_inspections = (((x << 28) >> 31) == 1);
		can_drill = (((x << 27) >> 31) == 1);
		can_reward = (((x << 26) >> 31) == 1);
		can_transport = (((x << 25) >> 31) == 1);
		return_buf = (((x << 24) >> 31) == 1);
		auto_sequence = x >> 8;
		inspections_std = x >> 16;
		drill_std = x >> 24;
		//pk::trace(pk::format("内部，数组转回，巡察：{}，训练：{}，征兵{}，褒奖：{}，运输：{}", can_inspections, can_drill, can_recruit, can_reward, can_transport));
		if (auto_sequence < 1 or auto_sequence > 6) auto_sequence = 1;		
		if (inspections_std < 0 or inspections_std > 100) inspections_std = 95;
		if (drill_std < 0 or drill_std > 100) drill_std = 95;
	}

	void fromInt32_2(uint32 x)
	{
		recruit_std_foodtroop = x;
		recruit_std_porder = x >> 8;
		reward_std = x >> 16;
		return_timer = x >> 24;
		if (reward_std < 0 or reward_std > 99) reward_std = 90;
		if (recruit_std_porder < 0 or recruit_std_porder > 100) recruit_std_porder = 95;
		if (recruit_std_foodtroop < 0 or recruit_std_foodtroop > 250) recruit_std_foodtroop = 150;
	}

	void fromInt32_3(uint32 x)
	{
		recruit_std_gold = x;
		if (recruit_std_gold < 0 or recruit_std_gold > 100000) recruit_std_gold = 2500;
	}

	void fromInt32_4(uint32 x)
	{
		for (int force_id = 0; force_id < 8; ++force_id)
		{
			spy_lv[force_id] = ((x << (28 - force_id*4)) >> 28);
		}
	}

	void fromInt32_5(uint32 x)
	{
		for (int force_id = 8; force_id < 16; ++force_id)
		{
			spy_lv[force_id] = ((x << (28 - (force_id - 8) * 4)) >> 28);
		}
	}

	void fromInt32_6(uint32 x)
	{
		for (int force_id = 16; force_id < 24; ++force_id)
		{
			spy_lv[force_id] = ((x << (28 - (force_id - 16)*4)) >> 28);
		}
	}

	void fromInt32_7(uint32 x)
	{
		for (int force_id = 24; force_id < 32; ++force_id)
		{
			spy_lv[force_id] = ((x << (28 - (force_id - 24) * 4)) >> 28);
		}
	}

	void fromInt32_8(uint32 x)
	{
		for (int force_id = 32; force_id < 40; ++force_id)
		{
			spy_lv[force_id] = ((x << (28 - (force_id - 32) * 4)) >> 28);
		}
	}

	void fromInt32_9(uint32 x)
	{
		for (int force_id = 40; force_id < 势力_末; ++force_id)
		{
			spy_lv[force_id] = ((x << (28 - (force_id - 8) * 4)) >> 28);
		}
	}

	void fromInt32_10(uint32 x)
	{
		for (int force_id = 0; force_id < 32; ++force_id)
		{
			can_build[force_id] = (((x << (31 - force_id)) >> 31) == 1);
		}
	}

	void fromInt32_11(uint32 x)
	{
		for (int force_id = 32; force_id < 势力_末; ++force_id)
		{
			can_build[force_id] = (((x << (63 - force_id)) >> 31) == 1);
		}
	}

	void fromInt32_12(uint32 x)
	{
		for (int force_id = 0; force_id < 32; ++force_id)
		{
			send_spy[force_id] = (((x << (31 - force_id)) >> 31) == 1);
		}
	}

	void fromInt32_13(uint32 x)
	{
		for (int force_id = 32; force_id < 势力_末; ++force_id)
		{
			send_spy[force_id] = (((x << (63 - force_id)) >> 31) == 1);
		}
	}

	void fromInt32_14(uint32 x)
	{
		for (int force_id = 0; force_id < 32; ++force_id)
		{
			enhance_spy[force_id] = (((x << (31 - force_id)) >> 31) == 1);
		}
	}

	void fromInt32_15(uint32 x)
	{
		for (int force_id = 32; force_id < 势力_末; ++force_id)
		{
			enhance_spy[force_id] = (((x << (63 - force_id)) >> 31) == 1);
		}
	}

	void fromInt32_16(uint32 x)
	{
		mil_pop_all = x;
	}

	void fromInt32_17(uint32 x)
	{
		mil_pop_av = x;
	}

	void fromInt32_18(uint32 x)
	{
		wounded = x;
	}

	void fromInt32_19(uint32 x)
	{
		return_pop = x;
	}

	void fromInt32_20(uint32 x)
	{
		public_order = x;
		anti_rumor = ((x << 16) >> 24);
		farmer_count = ((x << 8) >> 24);
		provoke_cd = x >> 24;
		//rumor_timer[4] = x;
		//rumor_timer[5] = ((x << 16) >> 24);
		//rumor_timer[6] = ((x << 8) >> 24);
		//rumor_timer[7] = x >> 24;
		//reward_std = x >> 16;
		//return_timer = x >> 24;
	}
	//mobilize_done

	void fromInt32_21(uint32 x)
	{
		mobilize_done = (((x << 31) >> 31) == 1);
		repair_done = (((x << 30) >> 31) == 1);
		enlist_done = (((x << 29) >> 31) == 1);
		perturb_done = (((x << 28) >> 31) == 1);
		misinform_done = (((x << 27) >> 31) == 1);
		fire_done = (((x << 26) >> 31) == 1);
		beguile_done = (((x << 25) >> 31) == 1);
		raid_done = (((x << 24) >> 31) == 1);
		reinforce_done = (((x << 23) >> 31) == 1);
		//enlist_done = (((x << 23) >> 31) == 1);
	}

<<<<<<< HEAD
=======
	void fromInt32_22(uint32 x)
	{
		revenue_bonus = x;
		harvest_bonus = x >> 16;
	}

	void fromInt32_23(uint32 x)
	{
		for (int i = 0; i < 兵种_末; ++i)
		{
			can_produce[i] = (((x << (31-i)) >> 31) == 1);
		}
	}

	void fromInt32_24(uint32 x)
	{
		recruit_person = x;
		inspections_person = x >> 16;
	}

	void fromInt32_25(uint32 x)
	{
		drill_person = x;
		produce_person = x >> 16;
	}

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	uint8 booltoint(bool t)
	{
		return t ? 1 : 0;
	}

	bool inttobool(int t)
	{
		return t == 1;
	}

}

class unitinfo//需要在部队生成的时候初始化，还是毁灭时？
{
	//int id;//结构初始化时获得，update时直接用？那嵌套还可以用吗
	bool has_contact;//和所有其他部队初次作战还是和玩家初次作战？
<<<<<<< HEAD
=======
	bool has_luezhen;//判断本回合是否被掠阵
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	uint8 cd_1;//各种特技cd
	uint8 cd_2;//各种特技cd
	uint8 cd_3;//各种特技cd
	uint8 cd_4;//各种特技cd
	uint8 cd_5;//各种特技cd
	uint8 cd_6;//各种特技cd
	uint8 cd_7;//各种特技cd
	uint8 cd_8;//各种特技cd
	uint16 wounded;//伤兵
<<<<<<< HEAD
=======
	uint8 movement_remain;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	//初始化
	unitinfo(int id)
	{
		get_info(id);
	}
	//因为没有需要id，所以如果写下面这种，会导致前面的被冲掉，所以就不用提供下面这张声明方式了
	//下面这种写法用于定义全局变量，先声明，然后用get_info获取信息后使用
	unitinfo() {}

	void get_info(int id)
	{
		//pk::trace(pk::format("id:{}",id));
		fromInt32_0(unit_ex_info_temp[0][id]);
		fromInt32_1(unit_ex_info_temp[1][id]);
		fromInt32_2(unit_ex_info_temp[2][id]);
		fromInt32_3(unit_ex_info_temp[3][id]);
<<<<<<< HEAD
=======
		fromInt32_4(unit_ex_info_temp[4][id]);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	}

	void update(int id)
	{
		unit_ex_info_temp[0][id] = toInt32_0();
		unit_ex_info_temp[1][id] = toInt32_1();
		unit_ex_info_temp[2][id] = toInt32_2();
		unit_ex_info_temp[3][id] = toInt32_3();
<<<<<<< HEAD
=======
		unit_ex_info_temp[4][id] = toInt32_4();
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	}

	uint32 toInt32_0(void)
	{
		uint8 has_contact_t = has_contact ? 1 : 0;//初次遭遇
<<<<<<< HEAD
		//uint8 recruit_done_t = recruit_done ? 1 : 0;
=======
		uint8 has_luezhen_t = has_luezhen ? 1 : 0;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		//uint8 inspections_done_t = inspections_done ? 1 : 0;
		//uint8 can_inspections_t = can_inspections ? 1 : 0;
		//uint8 can_drill_t = can_drill ? 1 : 0;
		//uint8 can_reward_t = can_reward ? 1 : 0;
		//uint8 can_transport_t = can_transport ? 1 : 0;
		//uint8 return_buf_t = return_buf ? 1 : 0;//人口返回buf
		//pk::trace(pk::format("内部，转为数组，巡察：{}，训练：{}，征兵{}，褒奖：{}，运输：{}", can_inspections, can_drill, can_recruit, can_reward, can_transport));
<<<<<<< HEAD
		uint32 x = has_contact_t + (has_contact_t << 1) + (has_contact_t << 2) + (has_contact_t << 3) + (has_contact_t << 4) + (has_contact_t << 5) + (has_contact_t << 6) + (has_contact_t << 7) + (has_contact_t << 8) + (has_contact_t << 9) + (has_contact_t << 10);
=======
		uint32 x = has_contact_t + (has_luezhen_t << 1) + (has_contact_t << 2) + (has_contact_t << 3) + (has_contact_t << 4) + (has_contact_t << 5) + (has_contact_t << 6) + (has_contact_t << 7) + (has_contact_t << 8) + (has_contact_t << 9) + (has_contact_t << 10);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		return x;
	}

	uint32 toInt32_1(void)
	{
		uint32 x = cd_1 + (cd_2 << 8) + (cd_3 << 16) + (cd_4 << 24);//return_timer
		return x;
	}

	uint32 toInt32_2(void)
	{
		uint32 x = cd_5 + (cd_6 << 8) + (cd_7 << 16) + (cd_8 << 24);//return_timer
		return x;
	}

	uint32 toInt32_3(void)
	{
		uint32 x = wounded + (wounded << 16);//return_timer
		return x;
	}
<<<<<<< HEAD
=======

	uint32 toInt32_4(void)
	{
		uint32 x = movement_remain + (movement_remain << 8) + (movement_remain << 16) + (movement_remain << 24);//return_timer
		return x;
	}
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	//读取后解析
	void fromInt32_0(uint32 x)
	{
		has_contact = (((x << 31) >> 31) == 1);
<<<<<<< HEAD
		//recruit_done = (((x << 30) >> 31) == 1);
=======
		has_luezhen = (((x << 30) >> 31) == 1);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		//inspections_done = (((x << 29) >> 31) == 1);
		//can_inspections = (((x << 28) >> 31) == 1);
		//can_drill = (((x << 27) >> 31) == 1);
		//can_reward = (((x << 26) >> 31) == 1);
		//can_transport = (((x << 25) >> 31) == 1);
		//return_buf = (((x << 24) >> 31) == 1);
		//auto_sequence = x >> 8;
		//inspections_std = x >> 16;
		//drill_std = x >> 24;
	}

	void fromInt32_1(uint32 x)
	{
		cd_1 = x;
		cd_2 = x >> 8;
		cd_3 = x >> 16;
		cd_4 = x >> 24;

	}

	void fromInt32_2(uint32 x)
	{
		cd_5 = x;
		cd_6 = x >> 8;
		cd_7 = x >> 16;
		cd_8 = x >> 24;
	}

	void fromInt32_3(uint32 x)
	{
		wounded = x;
	}
<<<<<<< HEAD
=======

	void fromInt32_4(uint32 x)
	{
		movement_remain = x;
		//cd_6 = x >> 8;
		//cd_7 = x >> 16;
		//cd_8 = x >> 24;
	}

>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
}

class forceinfo
{
	int8 good_evil;
	uint8 investment_time;
	uint16 investment_gold;
	uint8 disaster_relief_time;
	bool jisi_done;//是否执行过祭祀
	bool sangu_1;
	bool sangu_2;
	bool sangu_3;
	bool xuzhou;//三让徐州
<<<<<<< HEAD
=======
	bool all_ability_researched;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321

	uint16 sangu_1_turn_count;//触发三顾1时经过的旬数
	uint16 sangu_2_turn_count;//触发三顾2时经过的旬数
	//异族协同请求
	array<int8> ask_base_id(3, int8(-1));//长度为三的请求方据点id数组
	array<int8> aim_city_id(3, int8(-1));//长度为三的目标城市id数组
	array<int8> yizu_force_id(3, int8(-1));//长度为三的异族势力id数组
	array<int8> wait_timer(3, int8(-1));//长度为三的多少旬后出兵数组

	//被几号玩家流言次数
	array<uint8> rumor_timer(8, uint8(0));
	//int8 ask_base_id;//请求方据点id{ /*请求方据点id*/-1, /*目标城市id*/-1, /*异族势力id*/势力_盗贼, /*多少旬后出兵*/-1}
	//int8 aim_city_id; //目标城市id
	//int8 yizu_force_id;//异族势力id
	//int8 wait_timer;//多少旬后出兵
	//array<array<uint8>> ai异族请求信息(非贼势力_末, { /*请求方据点id*/-1, /*目标城市id*/-1, /*异族势力id*/势力_盗贼, /*多少旬后出兵*/-1 });

	uint8 alliance_id;
	uint8 attack_delay;//进攻延迟
	uint8 form_alliance_delay;
	//int8 alliance_target_id;
	//int8 alliance_leader_id;

	/*在300文件定义的暂存数组，读档时将存档加载入数组，
	array<array<uint32>> person_ex_info_temp(10, array<uint32>(武将_末, uint32(-1)));
	array<array<uint32>> base_ex_info_temp(10, array<uint32>(据点_末, uint32(-1)));
	*/
	//初始化
	forceinfo(int force_id)
	{
		get_info(force_id);
	}
	//下面这种写法用于定义全局变量，先声明，然后用get_info获取信息后使用
	forceinfo() {}

	void get_info(int force_id)
	{
		fromInt32_0(force_ex_info_temp[0][force_id]);
		fromInt32_1(force_ex_info_temp[1][force_id]);
		fromInt32_2(force_ex_info_temp[2][force_id]);
		fromInt32_3(force_ex_info_temp[3][force_id]);
		fromInt32_4(force_ex_info_temp[4][force_id]);//异族协同请求1
		fromInt32_5(force_ex_info_temp[5][force_id]);//异族协同请求2
		fromInt32_6(force_ex_info_temp[6][force_id]);//异族协同请求3
		fromInt32_7(force_ex_info_temp[7][force_id]);//被流言次数
		fromInt32_8(force_ex_info_temp[8][force_id]);//被流言次数2
		fromInt32_9(force_ex_info_temp[9][force_id]);//盟军相关
	}

	void update(int force_id)
	{
		//pk::trace(pk::format("start update,time:{}", pk::time()));
		//if (person_id == 289)  pk::trace(pk::format("update before：武将{}。统率{}，武力{}，智力{}，政治{}，魅力{}", person_id, float_command, float_strength, float_wisdom, float_politic, float_charm));
		force_ex_info_temp[0][force_id] = toInt32_0();
		force_ex_info_temp[1][force_id] = toInt32_1();
		force_ex_info_temp[2][force_id] = toInt32_2();
		force_ex_info_temp[3][force_id] = toInt32_3();
		force_ex_info_temp[4][force_id] = toInt32_4();
		force_ex_info_temp[5][force_id] = toInt32_5();
		force_ex_info_temp[6][force_id] = toInt32_6();
		force_ex_info_temp[7][force_id] = toInt32_7();
		force_ex_info_temp[8][force_id] = toInt32_8();
		force_ex_info_temp[9][force_id] = toInt32_9();
		//pk::trace(pk::format("update finished,time:{}", pk::time()));
		//if (person_id == 289) pk::trace(pk::format("update after：武将{}。统率{}，武力{}，智力{}，政治{}，魅力{}", person_id, float_command, float_strength, float_wisdom, float_politic, float_charm));
	}

	//储存的时候应是forceinfo.toInt32_0,作为一个单位储存
	uint32 toInt32_0(void)
	{
		//这是winapi支持的强行转换，将int8转成uint8类型，但实际储存的东西完全不变。如：1111 1111（-1）变成uint8后就是1111 1111（255)
		uint8 good_evil_t = good_evil;
		
		uint32 x = 0;
		x = good_evil_t + (investment_time << 8) + (disaster_relief_time << 16);// +(alliance_id << 24);
		//if (person_id_t == 289) pk::trace(pk::format("toInt32,储存前x{}：武将：{},统率{}，武力{}，智力{}，政治{}，魅力{}", x, person_id_t, float_command, float_strength, float_wisdom, float_politic, float_charm));
		return x;
	}
	uint32 toInt32_1(void)
	{
		//uint8 float_charm_t = float_charm;
		uint32 x = investment_gold;
		return x;
	}
	uint32 toInt32_2(void)
	{
		uint8 sangu_1_t = sangu_1 ? 1 : 0;//改成按势力区分后，此位置暂时作废，以后有需要填入
		uint8 sangu_2_t = sangu_2 ? 1 : 0;
		uint8 sangu_3_t = sangu_3 ? 1 : 0;
		uint8 xuzhou_t = xuzhou ? 1 : 0;
<<<<<<< HEAD
		//pk::trace(pk::format("内部，转为数组，巡察：{}，训练：{}，征兵{}，褒奖：{}，运输：{}", can_inspections, can_drill, can_recruit, can_reward, can_transport));
		uint32 x = sangu_1_t + (sangu_2_t << 1) + (sangu_3_t << 2) + (xuzhou_t << 3);
=======
		uint8 jisi_done_t = jisi_done ? 1 : 0;
		uint8 all_ability_researched_t = all_ability_researched ? 1 : 0;
		//pk::trace(pk::format("内部，转为数组，巡察：{}，训练：{}，征兵{}，褒奖：{}，运输：{}", can_inspections, can_drill, can_recruit, can_reward, can_transport));
		uint32 x = sangu_1_t + (sangu_2_t << 1) + (sangu_3_t << 2) + (xuzhou_t << 3) + (jisi_done_t << 4) + (all_ability_researched_t << 5);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		return x;
	}

	uint32 toInt32_3(void)
	{
		uint32 x = sangu_1_turn_count + (sangu_2_turn_count << 16);
		return x;
	}

	uint32 toInt32_4(void)
	{
		uint8 ask_base_id_t = ask_base_id[0];
		uint8 aim_city_id_t = aim_city_id[0];
		uint8 yizu_force_id_t = yizu_force_id[0];
		uint8 wait_timer_t = wait_timer[0];
		uint32 x = 0;
		x = ask_base_id_t + (aim_city_id_t << 8) + (yizu_force_id_t << 16) + (wait_timer_t << 24);
		return x;
	}

	uint32 toInt32_5(void)
	{
		uint8 ask_base_id_t = ask_base_id[1];
		uint8 aim_city_id_t = aim_city_id[1];
		uint8 yizu_force_id_t = yizu_force_id[1];
		uint8 wait_timer_t = wait_timer[1];
		uint32 x = 0;
		x = ask_base_id_t + (aim_city_id_t << 8) + (yizu_force_id_t << 16) + (wait_timer_t << 24);
		return x;
	}

	uint32 toInt32_6(void)
	{
		uint8 ask_base_id_t = ask_base_id[2];
		uint8 aim_city_id_t = aim_city_id[2];
		uint8 yizu_force_id_t = yizu_force_id[2];
		uint8 wait_timer_t = wait_timer[2];
		uint32 x = 0;
		x = ask_base_id_t + (aim_city_id_t << 8) + (yizu_force_id_t << 16) + (wait_timer_t << 24);
		return x;
	}

	uint32 toInt32_7(void)
	{
		uint32 x = 0;
		x = rumor_timer[0] + (rumor_timer[1] << 8) + (rumor_timer[2] << 16) + (rumor_timer[3] << 24);
		return x;
	}

	uint32 toInt32_8(void)
	{
		uint32 x = 0;
		x = rumor_timer[4] + (rumor_timer[5] << 8) + (rumor_timer[6] << 16) + (rumor_timer[7] << 24);
		return x;
	}

	uint32 toInt32_9(void)
	{
		//uint8 alliance_target_id_t = alliance_target_id;
		//uint8 alliance_leader_id_t = alliance_leader_id;
		uint32 x = 0;
		x = alliance_id + (attack_delay << 8) +(form_alliance_delay << 16);// + (alliance_id << 24);
		//x = alliance_target_id_t + (alliance_leader_id_t << 8);
		return x;
	}
	//读取后解析
	void fromInt32_0(uint32 x)
	{

		good_evil = x;
		investment_time = ((x << 16) >> 24);
		disaster_relief_time = ((x << 8) >> 24);
		//alliance_id = x >> 24;
		//float_politic = x >> 24;
	}

	void fromInt32_1(uint32 x)
	{
		investment_gold = x;
		//order = ((x << 16) >> 24);
		//mission_time = ((x << 8) >> 24);
	}

	void fromInt32_2(uint32 x)
	{
		sangu_1 = (((x << 31) >> 31) == 1);
		sangu_2 = (((x << 30) >> 31) == 1);
		sangu_3 = (((x << 29) >> 31) == 1);
		xuzhou = (((x << 28) >> 31) == 1);
<<<<<<< HEAD
=======
		jisi_done = (((x << 27) >> 31) == 1);
		all_ability_researched = (((x << 26) >> 31) == 1);
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
	}

	void fromInt32_3(uint32 x)
	{
		sangu_1_turn_count = x;
		sangu_2_turn_count = x >> 16;
	}


	void fromInt32_4(uint32 x)
	{
		//pk::trace(pk::format("fromInt32_4,temp:{},x:{}", force_ex_info_temp[4],x));
		if (x == 0)
		{
			ask_base_id[0] = -1;
			aim_city_id[0] = -1;
			yizu_force_id[0] = -1;
			wait_timer[0] = -1;
		}
		else
		{
			ask_base_id[0] = x;
			aim_city_id[0] = ((x << 16) >> 24);
			yizu_force_id[0] = ((x << 8) >> 24);
			wait_timer[0] = x >> 24;
		}

	}


	void fromInt32_5(uint32 x)
	{
		if (x == 0)
		{
			ask_base_id[1] = -1;
			aim_city_id[1] = -1;
			yizu_force_id[1] = -1;
			wait_timer[1] = -1;
		}
		else
		{
			ask_base_id[1] = x;
			aim_city_id[1] = ((x << 16) >> 24);
			yizu_force_id[1] = ((x << 8) >> 24);
			wait_timer[1] = x >> 24;
		}

	}

	void fromInt32_6(uint32 x)
	{
		if (x == 0)
		{
			ask_base_id[2] = -1;
			aim_city_id[2] = -1;
			yizu_force_id[2] = -1;
			wait_timer[2] = -1;
		}
		else
		{
			ask_base_id[2] = x;
			aim_city_id[2] = ((x << 16) >> 24);
			yizu_force_id[2] = ((x << 8) >> 24);
			wait_timer[2] = x >> 24;
		}

	}

	void fromInt32_7(uint32 x)
	{
		rumor_timer[0] = x;
		rumor_timer[1] = ((x << 16) >> 24);
		rumor_timer[2] = ((x << 8) >> 24);
		rumor_timer[3] = x >> 24;

	}

	void fromInt32_8(uint32 x)
	{
		rumor_timer[4] = x;
		rumor_timer[5] = ((x << 16) >> 24);
		rumor_timer[6] = ((x << 8) >> 24);
		rumor_timer[7] = x >> 24;
	}

	void fromInt32_9(uint32 x)
	{
		alliance_id = x;
		attack_delay = ((x << 16) >> 24);
		form_alliance_delay = ((x << 8) >> 24);
		//alliance_id = x >> 24;
		//float_politic = x >> 24;
	}
}

class specialinfo
{
	int16 person;//将军
	int16 building_id;
	int gold;
	int food;
	int troops;


	//初始化
	specialinfo(int id)
	{
		get_info(id);
	}
	//因为没有需要id，所以如果写下面这种，会导致前面的被冲掉，所以就不用提供下面这张声明方式了
	//下面这种写法用于定义全局变量，先声明，然后用get_info获取信息后使用
	specialinfo() {}

	void get_info(int id)
	{
		//pk::trace(pk::format("id:{}",id));
		fromInt32_0(special_info_temp[0][id]);
		fromInt32_1(special_info_temp[1][id]);
		fromInt32_2(special_info_temp[2][id]);
		fromInt32_3(special_info_temp[3][id]);
	}

	void update(int id)
	{
		special_info_temp[0][id] = toInt32_0();
		special_info_temp[1][id] = toInt32_1();
		special_info_temp[2][id] = toInt32_2();
		special_info_temp[3][id] = toInt32_3();
	}

	int32 toInt32_0(void)
	{
		int32 x = person + (building_id << 16);
		return x;
	}

	int32 toInt32_1(void)
	{
		int32 x = gold;
		return x;
	}

	int32 toInt32_2(void)
	{
		int32 x = food;
		return x;
	}

	int32 toInt32_3(void)
	{
		int32 x = troops;
		return x;
	}
	//读取后解析
	void fromInt32_0(int32 x)
	{
		person = x;
		building_id = x >> 16;
	}

	void fromInt32_1(int32 x)
	{
		gold = x;
	}

	void fromInt32_2(int32 x)
	{
		food = x;
	}

	void fromInt32_3(int32 x)
	{
		troops = x;
	}
}

class settinginfo
{
	bool set_port_gate_survive;//是否开启游戏中设定该参数
	bool enable_port_gate_survive;//该参数开启or关闭
	array<uint32> province_deserter(12, 0);
	bool nanzheng;//曹操南征
	array<bool> eve_happened(128, false);//就开关做成数组，需记录日期的额外做吧，一个日期需要1个int32太占地方了
	uint8 rumor_timer;

	uint16 sangu_1_turn_count;//触发三顾1时经过的旬数
	uint16 sangu_2_turn_count;//触发三顾2时经过的旬数
	//最多同时存在4个联盟好了
	array<uint8> alliance_target_id(4, 255);
	array<uint8> alliance_leader_id(4, 255);
	array<array<uint8>> alliance_target_base(4, array<uint8>(6, 255));//存联盟攻击目标的数组
	array<array<uint8>> alliance_member_id(4, array<uint8>(5, 255));//存组建中联盟成员的数组，无需存目标势力
	array<uint8> alliance_remain_time(4, 255);//联军剩余时间
	/*在300文件定义的暂存数组，读档时将存档加载入数组，
	array<array<uint32>> person_ex_info_temp(10, array<uint32>(武将_末, uint32(-1)));
	array<array<uint32>> base_ex_info_temp(10, array<uint32>(据点_末, uint32(-1)));
	*/
	//初始化
	settinginfo()
	{
		get_info();
	}
	//因为没有需要id，所以如果写下面这种，会导致前面的被冲掉，所以就不用提供下面这张声明方式了
	//下面这种写法用于定义全局变量，先声明，然后用get_info获取信息后使用
	//settinginfo() {}

	void get_info()
	{
		//pk::trace(pk::format("get_info,temp:{}", setting_ex_info_temp[0]));
		fromInt32_0(setting_ex_info_temp[0]);
		fromInt32_1(setting_ex_info_temp[1]);
		fromInt32_2(setting_ex_info_temp[2]);
		fromInt32_3(setting_ex_info_temp[3]);
		fromInt32_4(setting_ex_info_temp[4]);
		fromInt32_5(setting_ex_info_temp[5]);
		fromInt32_6(setting_ex_info_temp[6]);
		fromInt32_7(setting_ex_info_temp[7]);
		fromInt32_8(setting_ex_info_temp[8]);
		fromInt32_9(setting_ex_info_temp[9]);
		fromInt32_10(setting_ex_info_temp[10]);
		fromInt32_11(setting_ex_info_temp[11]);
		fromInt32_12(setting_ex_info_temp[12]);
		fromInt32_13(setting_ex_info_temp[13]);
		fromInt32_14(setting_ex_info_temp[14]);
		fromInt32_15(setting_ex_info_temp[15]);
		fromInt32_16(setting_ex_info_temp[16]);
		fromInt32_17(setting_ex_info_temp[17]);
		fromInt32_18(setting_ex_info_temp[18]);//18-25联盟相关信息
		fromInt32_19(setting_ex_info_temp[19]);
		fromInt32_20(setting_ex_info_temp[20]);
		fromInt32_21(setting_ex_info_temp[21]);
		fromInt32_22(setting_ex_info_temp[22]);
		fromInt32_23(setting_ex_info_temp[23]);
		fromInt32_24(setting_ex_info_temp[24]);
		fromInt32_25(setting_ex_info_temp[25]);
		fromInt32_26(setting_ex_info_temp[26]);//16-30联盟成员相关信息--组建中
		fromInt32_27(setting_ex_info_temp[27]);
		fromInt32_28(setting_ex_info_temp[28]);
		fromInt32_29(setting_ex_info_temp[29]);
		fromInt32_30(setting_ex_info_temp[30]);
		fromInt32_31(setting_ex_info_temp[31]);
		fromInt32_32(setting_ex_info_temp[32]);
	}

	void update()
	{
		//pk::trace(pk::format("start update,time:{}", pk::time()));
		//if (person_id == 289)  pk::trace(pk::format("update before：武将{}。统率{}，武力{}，智力{}，政治{}，魅力{}", person_id, float_command, float_strength, float_wisdom, float_politic, float_charm));
		setting_ex_info_temp[0] = toInt32_0();
		setting_ex_info_temp[1] = toInt32_1();
		setting_ex_info_temp[2] = toInt32_2();
		setting_ex_info_temp[3] = toInt32_3();
		setting_ex_info_temp[4] = toInt32_4();
		setting_ex_info_temp[5] = toInt32_5();
		setting_ex_info_temp[6] = toInt32_6();
		setting_ex_info_temp[7] = toInt32_7();
		setting_ex_info_temp[8] = toInt32_8();
		setting_ex_info_temp[9] = toInt32_9();
		setting_ex_info_temp[10] = toInt32_10();
		setting_ex_info_temp[11] = toInt32_11();
		setting_ex_info_temp[12] = toInt32_12();
		setting_ex_info_temp[13] = toInt32_13();
		setting_ex_info_temp[14] = toInt32_14();
		setting_ex_info_temp[15] = toInt32_15();
		setting_ex_info_temp[16] = toInt32_16();
		setting_ex_info_temp[17] = toInt32_17();
		setting_ex_info_temp[18] = toInt32_18();//18-25联盟相关信息
		setting_ex_info_temp[19] = toInt32_19();
		setting_ex_info_temp[20] = toInt32_20();
		setting_ex_info_temp[21] = toInt32_21();
		setting_ex_info_temp[22] = toInt32_22();
		setting_ex_info_temp[23] = toInt32_23();
		setting_ex_info_temp[24] = toInt32_24();
		setting_ex_info_temp[25] = toInt32_25();
		setting_ex_info_temp[26] = toInt32_26();//26-30联盟成员相关信息
		setting_ex_info_temp[27] = toInt32_27();
		setting_ex_info_temp[28] = toInt32_28();
		setting_ex_info_temp[29] = toInt32_29();
		setting_ex_info_temp[30] = toInt32_30();
		setting_ex_info_temp[31] = toInt32_31();
		setting_ex_info_temp[32] = toInt32_32();
	}

	//储存的时候应是forceinfo.toInt32_0,作为一个单位储存
	uint32 toInt32_0(void)
	{
		uint8 set_port_gate_survive_t = set_port_gate_survive ? 1 : 0;
		uint8 enable_port_gate_survive_t = enable_port_gate_survive ? 1 : 0;
		uint8 nanzheng_t = nanzheng ? 1 : 0;
		//pk::trace(pk::format("to,set:{},enable:{}", set_port_gate_survive_t, enable_port_gate_survive_t));
		uint32 x = set_port_gate_survive_t + (enable_port_gate_survive_t << 1) + (nanzheng_t << 2);
		return x;
	}

	uint32 toInt32_1(void)
	{
		return province_deserter[0];
	}

	uint32 toInt32_2(void)
	{
		return province_deserter[1];
	}

	uint32 toInt32_3(void)
	{
		return province_deserter[2];
	}

	uint32 toInt32_4(void)
	{
		return province_deserter[3];
	}

	uint32 toInt32_5(void)
	{
		return province_deserter[4];
	}

	uint32 toInt32_6(void)
	{
		return province_deserter[5];
	}

	uint32 toInt32_7(void)
	{
		return province_deserter[6];
	}

	uint32 toInt32_8(void)
	{
		return province_deserter[7];
	}

	uint32 toInt32_9(void)
	{
		return province_deserter[8];
	}

	uint32 toInt32_10(void)
	{
		return province_deserter[9];
	}

	uint32 toInt32_11(void)
	{
		return province_deserter[10];
	}

	uint32 toInt32_12(void)
	{
		return province_deserter[11];
	}

	uint32 toInt32_13(void)
	{
		uint32 x = rumor_timer + (rumor_timer << 8) + (rumor_timer << 16);
		return x;
	}

	uint32 toInt32_14(void)
	{
		uint32 x = 0;
		for (int i = 0; i < 32; ++i)
		{
			x = ch::set_bit(x,i, eve_happened[i]);
		}
		return x;
	}

	uint32 toInt32_15(void)
	{
		uint32 x = 0;
		for (int i = 0; i < 32; ++i)
		{
			x = ch::set_bit(x, i, eve_happened[i+32]);
		}
		return x;
	}

	uint32 toInt32_16(void)
	{
		uint32 x = 0;
		for (int i = 0; i < 32; ++i)
		{
			x = ch::set_bit(x, i, eve_happened[i+64]);
		}
		return x;
	}

	uint32 toInt32_17(void)
	{
		uint32 x = 0;
		for (int i = 0; i < 32; ++i)
		{
			x = ch::set_bit(x, i, eve_happened[i+96]);
		}
		return x;
	}

	uint32 toInt32_18(void)
	{
		uint32 x = 0;
		x = alliance_target_id[0] + (alliance_target_id[1] << 8) + (alliance_target_id[2] << 16) + (alliance_target_id[3] << 24);
		return x;
	}

	uint32 toInt32_19(void)
	{
		uint32 x = 0;
		x = alliance_leader_id[0] + (alliance_leader_id[1] << 8) + (alliance_leader_id[2] << 16) + (alliance_leader_id[3] << 24);
		return x;
	}

	uint32 toInt32_20(void)
	{
		uint32 x = 0;
		x = alliance_target_base[0][0] + (alliance_target_base[1][0] << 8) + (alliance_target_base[2][0] << 16) + (alliance_target_base[3][0] << 24);
		return x;
	}

	uint32 toInt32_21(void)
	{
		uint32 x = 0;
		x = alliance_target_base[0][1] + (alliance_target_base[1][1] << 8) + (alliance_target_base[2][1] << 16) + (alliance_target_base[3][1] << 24);
		return x;
	}

	uint32 toInt32_22(void)
	{
		uint32 x = 0;
		x = alliance_target_base[0][2] + (alliance_target_base[1][2] << 8) + (alliance_target_base[2][2] << 16) + (alliance_target_base[3][2] << 24);
		return x;
	}

	uint32 toInt32_23(void)
	{
		uint32 x = 0;
		x = alliance_target_base[0][3] + (alliance_target_base[1][3] << 8) + (alliance_target_base[2][3] << 16) + (alliance_target_base[3][3] << 24);
		return x;
	}

	uint32 toInt32_24(void)
	{
		uint32 x = 0;
		x = alliance_target_base[0][4] + (alliance_target_base[1][4] << 8) + (alliance_target_base[2][4] << 16) + (alliance_target_base[3][4] << 24);
		return x;
	}

	uint32 toInt32_25(void)
	{
		uint32 x = 0;
		x = alliance_target_base[0][5] + (alliance_target_base[1][5] << 8) + (alliance_target_base[2][5] << 16) + (alliance_target_base[3][5] << 24);
		return x;
	}

	uint32 toInt32_26(void)
	{
		uint32 x = 0;
		x = alliance_member_id[0][0] + (alliance_member_id[1][0] << 8) + (alliance_member_id[2][0] << 16) + (alliance_member_id[3][0] << 24);
		return x;
	}

	uint32 toInt32_27(void)
	{
		uint32 x = 0;
		x = alliance_member_id[0][1] + (alliance_member_id[1][1] << 8) + (alliance_member_id[2][1] << 16) + (alliance_member_id[3][1] << 24);
		return x;
	}

	uint32 toInt32_28(void)
	{
		uint32 x = 0;
		x = alliance_member_id[0][2] + (alliance_member_id[1][2] << 8) + (alliance_member_id[2][2] << 16) + (alliance_member_id[3][2] << 24);
		return x;
	}

	uint32 toInt32_29(void)
	{
		uint32 x = 0;
		x = alliance_member_id[0][3] + (alliance_member_id[1][3] << 8) + (alliance_member_id[2][3] << 16) + (alliance_member_id[3][3] << 24);
		return x;
	}

	uint32 toInt32_30(void)
	{
		uint32 x = 0;
		x = alliance_member_id[0][4] + (alliance_member_id[1][4] << 8) + (alliance_member_id[2][4] << 16) + (alliance_member_id[3][4] << 24);
		return x;
	}

	uint32 toInt32_31(void)
	{
		uint32 x = 0;
		x = alliance_remain_time[0] + (alliance_remain_time[1] << 8) + (alliance_remain_time[2] << 16) + (alliance_remain_time[3] << 24);
		return x;
	}

	uint32 toInt32_32(void)
	{
		uint32 x = 0;
		x = sangu_1_turn_count + (sangu_2_turn_count << 16);
		return x;
	}
	//读取后解析
	void fromInt32_0(uint32 x)
	{
		//pk::trace(pk::format("from,x:{}", setting_ex_info_temp[0]));
		set_port_gate_survive = (((x << 31) >> 31) == 1);
		enable_port_gate_survive = (((x << 30) >> 31) == 1);
		nanzheng = (((x << 29) >> 31) == 1);
		//pk::trace(pk::format("from,set:{},enable:{}", set_port_gate_survive, enable_port_gate_survive));
	}

	void fromInt32_1(uint32 x)
	{
		province_deserter[0] = x;
	}

	void fromInt32_2(uint32 x)
	{
		province_deserter[1] = x;
	}

	void fromInt32_3(uint32 x)
	{
		province_deserter[2] = x;
	}

	void fromInt32_4(uint32 x)
	{
		province_deserter[3] = x;
	}

	void fromInt32_5(uint32 x)
	{
		province_deserter[4] = x;
	}

	void fromInt32_6(uint32 x)
	{
		province_deserter[5] = x;
	}

	void fromInt32_7(uint32 x)
	{
		province_deserter[6] = x;
	}

	void fromInt32_8(uint32 x)
	{
		province_deserter[7] = x;
	}

	void fromInt32_9(uint32 x)
	{
		province_deserter[8] = x;
	}

	void fromInt32_10(uint32 x)
	{
		province_deserter[9] = x;
	}

	void fromInt32_11(uint32 x)
	{
		province_deserter[10] = x;
	}

	void fromInt32_12(uint32 x)
	{
		province_deserter[11] = x;
	}

	void fromInt32_13(uint32 x)
	{
		rumor_timer = x;
	}

	void fromInt32_14(uint32 x)
	{
<<<<<<< HEAD
		pk::trace("x:" + x);
=======
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		for (int i = 0; i < 32; ++i)
		{
			eve_happened[i] = ch::test_bit(x, i);
		}
	}

	void fromInt32_15(uint32 x)
	{
		for (int i = 0; i < 32; ++i)
		{
			eve_happened[i+32] = ch::test_bit(x, i);
		}
	}

	void fromInt32_16(uint32 x)
	{
		for (int i = 0; i < 32; ++i)
		{
			eve_happened[i+64] = ch::test_bit(x, i);
		}
	}

	void fromInt32_17(uint32 x)
	{
		for (int i = 0; i < 32; ++i)
		{
			eve_happened[i+96] = ch::test_bit(x, i);
		}
	}
	/*
	array<uint8> alliance_target_id(4, 255);
	array<uint8> alliance_leader_id(4, 255);
	array<array<uint8>> alliance_target_base(4, array<uint8>(6, 255));//存联盟攻击目标的数组
	*/
	void fromInt32_18(uint32 x)
	{
		alliance_target_id[0] = x;
		alliance_target_id[1] = ((x << 16) >> 24);
		alliance_target_id[2] = ((x << 8) >> 24);
		alliance_target_id[3] = x >> 24;
	}

	void fromInt32_19(uint32 x)
	{
		alliance_leader_id[0] = x;
		alliance_leader_id[1] = ((x << 16) >> 24);
		alliance_leader_id[2] = ((x << 8) >> 24);
		alliance_leader_id[3] = x >> 24;
	}

	void fromInt32_20(uint32 x)
	{
		alliance_target_base[0][0] = x;
		alliance_target_base[1][0] = ((x << 16) >> 24);
		alliance_target_base[2][0] = ((x << 8) >> 24);
		alliance_target_base[3][0] = x >> 24;
	}

	void fromInt32_21(uint32 x)
	{
		alliance_target_base[0][1] = x;
		alliance_target_base[1][1] = ((x << 16) >> 24);
		alliance_target_base[2][1] = ((x << 8) >> 24);
		alliance_target_base[3][1] = x >> 24;
	}

	void fromInt32_22(uint32 x)
	{
		alliance_target_base[0][2] = x;
		alliance_target_base[1][2] = ((x << 16) >> 24);
		alliance_target_base[2][2] = ((x << 8) >> 24);
		alliance_target_base[3][2] = x >> 24;
	}

	void fromInt32_23(uint32 x)
	{
		alliance_target_base[0][3] = x;
		alliance_target_base[1][3] = ((x << 16) >> 24);
		alliance_target_base[2][3] = ((x << 8) >> 24);
		alliance_target_base[3][3] = x >> 24;
	}

	void fromInt32_24(uint32 x)
	{
		alliance_target_base[0][4] = x;
		alliance_target_base[1][4] = ((x << 16) >> 24);
		alliance_target_base[2][4] = ((x << 8) >> 24);
		alliance_target_base[3][4] = x >> 24;
	}

	void fromInt32_25(uint32 x)
	{
		alliance_target_base[0][5] = x;
		alliance_target_base[1][5] = ((x << 16) >> 24);
		alliance_target_base[2][5] = ((x << 8) >> 24);
		alliance_target_base[3][5] = x >> 24;
	}

	void fromInt32_26(uint32 x)
	{
		alliance_member_id[0][0] = x;
		alliance_member_id[1][0] = ((x << 16) >> 24);
		alliance_member_id[2][0] = ((x << 8) >> 24);
		alliance_member_id[3][0] = x >> 24;
	}

	void fromInt32_27(uint32 x)
	{
		alliance_member_id[0][1] = x;
		alliance_member_id[1][1] = ((x << 16) >> 24);
		alliance_member_id[2][1] = ((x << 8) >> 24);
		alliance_member_id[3][1] = x >> 24;
	}

	void fromInt32_28(uint32 x)
	{
		alliance_member_id[0][2] = x;
		alliance_member_id[1][2] = ((x << 16) >> 24);
		alliance_member_id[2][2] = ((x << 8) >> 24);
		alliance_member_id[3][2] = x >> 24;
	}

	void fromInt32_29(uint32 x)
	{
		alliance_member_id[0][3] = x;
		alliance_member_id[1][3] = ((x << 16) >> 24);
		alliance_member_id[2][3] = ((x << 8) >> 24);
		alliance_member_id[3][3] = x >> 24;
	}

	void fromInt32_30(uint32 x)
	{
		alliance_member_id[0][4] = x;
		alliance_member_id[1][4] = ((x << 16) >> 24);
		alliance_member_id[2][4] = ((x << 8) >> 24);
		alliance_member_id[3][4] = x >> 24;
	}

	void fromInt32_31(uint32 x)
	{
		alliance_remain_time[0] = x;
		alliance_remain_time[1] = ((x << 16) >> 24);
		alliance_remain_time[2] = ((x << 8) >> 24);
		alliance_remain_time[3] = x >> 24;
	}

	void fromInt32_32(uint32 x)
	{
		sangu_1_turn_count = x;
		sangu_2_turn_count = x >> 16;
	}
}


class autoarmyinfo
{
	int16 member1;
	int16 member2;
	int16 member3;
	int8 ground_weapon;
	int8 sea_weapon;
	int16 freq;


	//初始化
	autoarmyinfo(int id)
	{
		get_info(id);
	}
	//因为没有需要id，所以如果写下面这种，会导致前面的被冲掉，所以就不用提供下面这张声明方式了
	//下面这种写法用于定义全局变量，先声明，然后用get_info获取信息后使用
	autoarmyinfo() {}

	void get_info(int id)
	{
		//pk::trace(pk::format("id:{}",id));
		fromInt32_0(autoarmy_ex_info_temp[0][id]);
		fromInt32_1(autoarmy_ex_info_temp[1][id]);
		fromInt32_2(autoarmy_ex_info_temp[2][id]);
	}

	void update(int id)
	{
		autoarmy_ex_info_temp[0][id] = toInt32_0();
		autoarmy_ex_info_temp[1][id] = toInt32_1();
		autoarmy_ex_info_temp[2][id] = toInt32_2();
	}

	int32 toInt32_0(void)
	{
		int32 x = member1 + (member2 << 16);
		return x;
	}

	int32 toInt32_1(void)
	{
		int32 x = member3 + (ground_weapon << 16) + (sea_weapon << 24);
		return x;
	}

	int32 toInt32_2(void)
	{
		int32 x = freq;
		return x;
	}

	//读取后解析
	void fromInt32_0(int32 x)
	{
		member1 = x;
		member2 = x >> 16;
	}

	void fromInt32_1(int32 x)
	{
		member3 = (x << 16) >> 16;
		ground_weapon = (x << 8) >> 24;
		sea_weapon = x >> 24;
	}

	void fromInt32_2(int32 x)
	{
		freq = (x << 16) >> 16;
	}

}


// ## 2021/10/29 # 江东新风 # 结构体存储调用方式改进 ##
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2021/01/21 # 江东新风 # 将能力浮动信息搬运到结构体 ##
// ## 2020/12/12 # 江东新风 # 修复trace参数报错 ##
// ## 2020/08/09 #江东新风#修复了能力浮动范围变成两倍的bug##
// ## 2020/08/08 #江东新风#修复了能力浮动沒有記錄原始能力的bug，加入了浮动达到上限的判定##
// ## 2020/07/26 ##
/*
@제작자: HoneyBee
@설명: 무장들의 능력 변동 SCRIPT

-> RANDOM한 확률로 무장들의 능력이 +@, -@로 변동됩니다. (일종의 컨디션(?) 효과)

   EX) 상황에 따라서 때로는 유능한 능력을 발휘하여 전쟁에서 승리하거나 간혹 무능한 면모를 보여 전쟁에서 패배하고, 삼국지연의에서는 일기토에서 승리하거나 패하는 등..
	   <正史> 및 <연의>에서 유추할 수 있는 무장들의 인간적인 면모를 참고하여 능력의 변동 요소를 첨가했습니다.

*/

namespace 能力浮动
{

	// ================ CUSTOMIZE ================
	const int KEY = pk::hash("能力浮动");
	const bool 仅玩家有效 = false;  // true =Player만 효과적용, false =유저와 AI 모두 효과 적용

	const int 发动时期 = 1;             // 0: 매턴,  1: 매월 1일,  2: 매분기 첫달1일,  3: 매년 1월1일,  4: 랜덤 턴
	const int 发动几率_旬 = 10;          // 发动时期를 랜덤으로 설정했을 경우: 턴별 발동 확률
	const int 发动几率_建筑 = 10;         // 발동 시 건물별로 발동 확률 설정
	const int 发动几率_部队 = 10;         // 발동 시 부대별로 발동 확률 설정
/*
	const int min_stat = 2;          // 최소 변동 능력치를 설정 (EX: -2)
	const int max_stat = 2;          // 최대 변동 능력치를 설정 (EX: +2)
*/
	const int stat_change_range = 2; //上下浮动2,此处范围是0~127
	const bool  Debug_Mode = false; //是否开?糾錯模式


// ===========================================


	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
			//pk::bind(102, pk::trigger102_t(剧本初始化_武将能力浮动_情报讀取));
			//pk::bind(105, pk::trigger105_t(儲存_武将能力浮动_情报儲存));
		}

		void callback()
		{
			//pk::trace(pk::format("start 能力浮动,time:{}", pk::time()));
			// 매턴
			if (发动时期 == 0 and pk::get_elapsed_days() != 0)
			{
				reset_stat();
				is_BuildingSet();
				is_UnitSet();
			}

			// 매월 1일
			else if (发动时期 == 1 and (pk::get_day() == 1) and pk::get_elapsed_days() != 0)
			{
				reset_stat();
				is_BuildingSet();
				is_UnitSet();
			}

			// 매분기 첫달이면서 1일
			else if (发动时期 == 2 and (pk::get_day() == 1) and pk::is_first_month_of_quarter(pk::get_month()) and pk::get_elapsed_days() != 0)
			{
				reset_stat();
				is_BuildingSet();
				is_UnitSet();
			}

			// 매년 1월 1일
			else if (发动时期 == 3 and (pk::get_day() == 1) and pk::get_month() == 1 and pk::get_elapsed_days() != 0)
			{
				reset_stat();
				is_BuildingSet();
				is_UnitSet();
			}

			// 랜덤 턴에서 발동
			else if (发动时期 == 4 and pk::rand_bool(发动几率_旬) and pk::get_elapsed_days() != 0)
			{
				reset_stat();
				is_BuildingSet();
				is_UnitSet();
			}
			//pk::trace(pk::format("finished 能力浮动,time:{}", pk::time()));
		}

		bool only_player_unit(pk::unit@ unit)
		{
			if (仅玩家有效 and !unit.is_player()) return false;
			return true;
		}

		bool only_player_building(pk::building@ building)
		{
			if (仅玩家有效 and !building.is_player()) return false;
			return true;
		}

		void reset_stat()
		{
			for (int i = 0; i < 武将_末; i++)
			{
				pk::person@ person_ = pk::get_person(i);
				if (is_target_mibun(person_))
				{
					personinfo@ person_t = @person_ex[i];

					if (Debug_Mode)
					{
						if (person_.get_id() == 289 or person_.get_id() == 432 or person_.get_id() == 660 or person_.get_id() == 919)
						{
							//ch::u8debug(pk::format("reset:{}'stat change, tong{}zhi{}zheng{}",person_.get_id(), person_t.float_command, person_t.float_strength, person_t.float_wisdom));
							pk::history_log(pk::get_current_turn_force_id(), pk::get_force(pk::get_current_turn_force_id()).color, pk::encode(pk::format("能力还原:\x1b[1x{}\x1b[0x的能力变化为统\x1b[1x{}\x1b[0x武\x1b[2x{}\x1b[0x智\x1b[2x{}\x1b[0x", pk::decode(pk::get_name(person_)), -person_t.float_command, -person_t.float_strength, -person_t.float_wisdom)));
						}
					}
					
					person_.base_stat[武将能力_统率] = person_.base_stat[武将能力_统率] - person_t.float_command;
					person_.base_stat[武将能力_武力] = person_.base_stat[武将能力_武力] - person_t.float_strength;
					person_.base_stat[武将能力_智力] = person_.base_stat[武将能力_智力] - person_t.float_wisdom;
					person_.update();
				}

			}
		}

		void is_BuildingSet()
		{
			pk::random random(pk::rand());
			int 最大能力 = int(pk::core["person.max_stat"]); // 获取设定的最大能力 
			for (int i = 0; i < 999; i++)
			{
				pk::person@ person_ = pk::get_person(i);
				personinfo@ person_t = @person_ex[i];
				if (is_target_mibun(person_) and (person_.location < 87) and (person_.location >= 0))
				{
					if (pk::rand_bool(发动几率_建筑))
					{
						
						person_t.float_command = random(-stat_change_range, stat_change_range);
						person_t.float_strength = random(-stat_change_range, stat_change_range);
						person_t.float_wisdom = random(-stat_change_range, stat_change_range);
						/*
						person_t.float_command = random(0, stat_change_range);
						person_t.float_strength = random(0, stat_change_range);
						person_t.float_wisdom = random(0, stat_change_range);
						*/
						if (person_t.float_command > 0 and (person_.base_stat[武将能力_统率] + person_t.float_command) > 最大能力)
							person_t.float_command = 最大能力 - person_.base_stat[武将能力_统率];
						if (person_t.float_strength > 0 and (person_.base_stat[武将能力_武力] + person_t.float_strength) > 最大能力)
							person_t.float_strength = 最大能力 - person_.base_stat[武将能力_武力];
						if (person_t.float_wisdom > 0 and (person_.base_stat[武将能力_智力] + person_t.float_wisdom) > 最大能力)
							person_t.float_wisdom = 最大能力 - person_.base_stat[武将能力_智力];

						if (person_t.float_command < 0 and (person_.base_stat[武将能力_统率] + person_t.float_command) < 1)
							person_t.float_command = 1 - person_.base_stat[武将能力_统率];
						if (person_t.float_strength < 0 and (person_.base_stat[武将能力_武力] + person_t.float_strength) < 1)
							person_t.float_strength = 1 - person_.base_stat[武将能力_武力];
						if (person_t.float_wisdom < 0 and (person_.base_stat[武将能力_智力] + person_t.float_wisdom) < 1)
							person_t.float_wisdom = 1 - person_.base_stat[武将能力_智力];

						if (Debug_Mode)
						{
							if (person_.get_id() == 289 or person_.get_id() == 432 or person_.get_id() == 660 or person_.get_id() == 919)
							{
								//ch::u8debug(pk::format("building:{}'stat change, tong{}zhi{}zheng{}",person_.get_id(), person_t.float_command, person_t.float_strength, person_t.float_wisdom));
								pk::history_log(pk::get_force(pk::get_current_turn_force_id()).pos, pk::get_force(pk::get_current_turn_force_id()).color, pk::encode(pk::format("据点中:\x1b[1x{}\x1b[0x的能力变化为统\x1b[1x{}\x1b[0x武\x1b[2x{}\x1b[0x智\x1b[2x{}\x1b[0x", pk::decode(pk::get_name(person_)), person_t.float_command, person_t.float_strength, person_t.float_wisdom)));
							}
						}

						person_.base_stat[武将能力_统率] = person_.base_stat[武将能力_统率] + person_t.float_command;
						person_.base_stat[武将能力_武力] = person_.base_stat[武将能力_武力] + person_t.float_strength;
						person_.base_stat[武将能力_智力] = person_.base_stat[武将能力_智力] + person_t.float_wisdom;
					}
					else
					{
						person_t.float_command = 0;
						person_t.float_strength = 0;
						person_t.float_wisdom = 0;
					}
				}
			}
		}
		void is_UnitSet()
		{
			pk::random random(pk::rand());
			int 最大能力 = int(pk::core["person.max_stat"]); // 获取设定的最大能力 
			for (int i = 0; i < 999; i++)
			{
				pk::person@ person_ = pk::get_person(i);
				personinfo@ person_t = @person_ex[i];
				if (is_target_mibun(person_) and (person_.location < 1087) and (person_.location > 86))
				{
					if (pk::rand_bool(发动几率_部队))
					{

						person_t.float_command = random(-stat_change_range, stat_change_range);
						person_t.float_strength = random(-stat_change_range, stat_change_range);
						person_t.float_wisdom = random(-stat_change_range, stat_change_range);
						/*
						person_t.float_command = random(0, stat_change_range);
						person_t.float_strength = random(0, stat_change_range);
						person_t.float_wisdom = random(0, stat_change_range);
						*/
						if ((person_.base_stat[武将能力_统率] + person_t.float_command) > 最大能力)
							person_t.float_command = 最大能力 - person_.base_stat[武将能力_统率];
						if ((person_.base_stat[武将能力_武力] + person_t.float_strength) > 最大能力)
							person_t.float_strength = 最大能力 - person_.base_stat[武将能力_武力];
						if ((person_.base_stat[武将能力_智力] + person_t.float_wisdom) > 最大能力)
							person_t.float_wisdom = 最大能力 - person_.base_stat[武将能力_智力];

						if ((person_.base_stat[武将能力_统率] + person_t.float_command) < 1)
							person_t.float_command = 1 - person_.base_stat[武将能力_统率];
						if ((person_.base_stat[武将能力_武力] + person_t.float_strength) < 1)
							person_t.float_strength = 1 - person_.base_stat[武将能力_武力];
						if ((person_.base_stat[武将能力_智力] + person_t.float_wisdom) < 1)
							person_t.float_wisdom = 1 - person_.base_stat[武将能力_智力];

						if (Debug_Mode)
						{
							if (person_.get_id() == 289 or person_.get_id() == 432 or person_.get_id() == 660 or person_.get_id() == 919)
							{
								//ch::u8debug(pk::format("unit:{}'stat change, tong{}zhi{}zheng{}",person_.get_id(), person_t.float_command, person_t.float_strength, person_t.float_wisdom));
								pk::history_log(pk::get_force(pk::get_current_turn_force_id()).pos, pk::get_force(pk::get_current_turn_force_id()).color, pk::encode(pk::format("部队中:\x1b[1x{}\x1b[0x的能力变化为统\x1b[1x{}\x1b[0x武\x1b[2x{}\x1b[0x智\x1b[2x{}\x1b[0x", pk::decode(pk::get_name(person_)), person_t.float_command, person_t.float_strength, person_t.float_wisdom)));
							}
						}

						person_.base_stat[武将能力_统率] = person_.base_stat[武将能力_统率] + person_t.float_command;
						person_.base_stat[武将能力_武力] = person_.base_stat[武将能力_武力] + person_t.float_strength;
						person_.base_stat[武将能力_智力] = person_.base_stat[武将能力_智力] + person_t.float_wisdom;
						person_.update();
					}
					else
					{
						person_t.float_command = 0;
						person_t.float_strength = 0;
						person_t.float_wisdom = 0;
					}
				}

			}
		}

		bool is_target_mibun(pk::person@ target)
		{
			if (target.mibun == 身分_君主 or target.mibun == 身分_君主 or target.mibun == 身分_都督 or target.mibun == 身分_太守 or target.mibun == 身分_一般 or target.mibun == 身分_在野 or target.mibun == 身分_捕虏)
				return true;
			return false;
		}
	} // class Main

	Main main;

} // namespace
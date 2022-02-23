// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/10/24 #江东新风#改写171trigger##
// ## 2020/11/17 #messi#优化语句，修正描述错误##
// ## 2020/07/26 ##
/*
	@제작 : masterpiecek


	[ 수송부대 수송량 제한 ]

	 * 수송부대가 출진 시 수송량이 제한됨 (조건에 따라 변동)
	 * 유저설정은 부대의 조작주체가 플레이어 1군단, 플레이어 위임군단, 컴퓨터인지에 따라 개별설정 가능

	1. 수송제한 항목 : 일반병기, 공성병기, 함선, 금, 병량, 병력
	 1) 항목별로 조건에 따라 조정된 제한비율만큼 최대 수송량이 제한됨
	  - 최대수송량 = 기존최대수송량 * 제한비율 / 100
	  - 제한비율 : 제한비율 기본값에 조건별 제한비율 조정값을 모두 곱하여 최종 제한비율 결정 (제한비율 최소값 이상 최대값 이하로 결정)
	 2) 메시지 표시 : 초과경고, 초과항목 안내, 최대수송량 정보

	2. 제한비율 조정조건
	 1) 계산조건 : 계산을 통해 조정값(배율)을 도출하는 조건 (각 조건의 최소값 이상 최대값 이하로 결정)
	  (1) 부대병력   : 부대의 병력에 비례하여 조정값 도출   (병력조건 조정값 = 부대병력 / 병력 기준값)
	   - 병력 기준값은 다른 조건들에 의해 결정된 최대 병력 수송량으로 결정
	  (2) 주장능력치 : 주장의 능력치에 비례하여 조정값 도출 (능력조건 조정값 = 주장의 적용능력 능력치 / 능력 기준값)
	  (3) 부대기력   : 부대의 기력에 비례하여 조정값 도출   (기력조건 조정값 = 부대기력 / 기력 기준값)
	 2) 지정조건 : 설정된 고정 조정값(배율)을 적용하는 조건 (각 조건의 조정값을 직접 설정)
	  - 특기, 기교, 계절 항목 추가 및 삭제 가능
	  (1) 무장특기 : 부대 소속 무장이 보유한 특기에 따라 조정값 차등 적용
	  (2) 세력기교 : 연구된 세력 기교에 따라 조정값 차등 적용
	  (3) 계절     : 현재 계절에 따라 조정값 차등 적용
*/

namespace 수송부대_수송량_제한
{

	///	=============================================  < 유저 설정 >  =====================================================

		/// 항목별 제한적용여부 설정
	const array<array<bool>> 限制输送量开关 =
	{
		/**兵?*/ {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false},
		/**兵器*/ {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false},
		/**?船*/ {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false},
		/** 金 */ {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false},
		/**兵粮*/ {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false},
		/**兵力*/ {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/false}
	};

	/// 최대 수송량 초과 시 메시지 표시 설정 超過最大输送量是报文显示设定  (컴퓨터 제외)
	const array<bool> 메시지표시_초과경고 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true };
	const array<bool> 메시지표시_초과항목 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true };
	const array<bool> 메시지표시_최대정보 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true };

	/// 항목별 제한비율 기본값 설정 各項限制比例默认（基准）値设定  (0~100%)
	const array<array<float>> 限制比例_基础量 =
	{
		/**일반병기*/ {/*플레이어 1군단*/20.0, /*플레이어 위임군단*/20.0, /*컴퓨터*/40.0},
		/**공성병기*/ {/*플레이어 1군단*/20.0, /*플레이어 위임군단*/20.0, /*컴퓨터*/40.0},
		/**  함선  */ {/*플레이어 1군단*/20.0, /*플레이어 위임군단*/20.0, /*컴퓨터*/40.0},
		/**   금   */ {/*플레이어 1군단*/20.0, /*플레이어 위임군단*/20.0, /*컴퓨터*/40.0},
		/**  병량  */ {/*플레이어 1군단*/20.0, /*플레이어 위임군단*/20.0, /*컴퓨터*/40.0},
		/**  병력  */ {/*플레이어 1군단*/20.0, /*플레이어 위임군단*/20.0, /*컴퓨터*/40.0}
	};

	/// 항목별 제한비율 최대값 설정 各項限制比例最大値 (최대最大 100%)
	const array<array<float>> 限制比例_最大量 =
	{
		/**일반병기*/ {/*플레이어 1군단*/40.0f, /*플레이어 위임군단*/40.0f, /*컴퓨터*/60.0f},
		/**공성병기*/ {/*플레이어 1군단*/40.0f, /*플레이어 위임군단*/40.0f, /*컴퓨터*/60.0f},
		/**  함선  */ {/*플레이어 1군단*/40.0f, /*플레이어 위임군단*/40.0f, /*컴퓨터*/60.0f},
		/**   금   */ {/*플레이어 1군단*/40.0f, /*플레이어 위임군단*/40.0f, /*컴퓨터*/60.0f},
		/**  병량  */ {/*플레이어 1군단*/40.0f, /*플레이어 위임군단*/40.0f, /*컴퓨터*/60.0f},
		/**  병력  */ {/*플레이어 1군단*/40.0f, /*플레이어 위임군단*/40.0f, /*컴퓨터*/60.0f}
	};

	/// 항목별 제한비율 최소값 설정 各項限制比例最小値  (최소最小 0%)
	const array<array<float>> 限制比例_最小量 =
	{
		/**일반병기*/ {/*플레이어 1군단*/5.0f, /*플레이어 위임군단*/5.0f, /*컴퓨터*/20.0f},
		/**공성병기*/ {/*플레이어 1군단*/5.0f, /*플레이어 위임군단*/5.0f, /*컴퓨터*/20.0f},
		/**  함선  */ {/*플레이어 1군단*/5.0f, /*플레이어 위임군단*/5.0f, /*컴퓨터*/20.0f},
		/**   금   */ {/*플레이어 1군단*/5.0f, /*플레이어 위임군단*/5.0f, /*컴퓨터*/20.0f},
		/**  병량  */ {/*플레이어 1군단*/5.0f, /*플레이어 위임군단*/5.0f, /*컴퓨터*/20.0f},
		/**  병력  */ {/*플레이어 1군단*/5.0f, /*플레이어 위임군단*/5.0f, /*컴퓨터*/20.0f}
	};

	/// 제한비율 조정조건 : 부대병력 (수송항목 중 병력을 제외하고 적용됨)
	// 병력조건 조정값 = 부대병력 / 병력 기준값
	// 위 식의 병력 기준값은 다른 조정조건이 다 반영된 상태의 최대 병력 수송량이 기준값이 됨
	// 限制比例調整条件：部队兵力
	// 兵力影响系数（調整値） = 部队兵力/兵力基准値  （下面各項系数计算公式类似）
	// 上式中的兵力基准値是在其他調整条件（参数）都考虑到的情況下成为兵力最大输送量的基准値
	const array<bool> 비율조정_병력조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<float> 비율조정_병력조건_최소값 = {/*플레이어 1군단*/0.5f, /*플레이어 위임군단*/0.5f, /*컴퓨터*/0.8f };
	const array<float> 비율조정_병력조건_최대값 = {/*플레이어 1군단*/1.5f, /*플레이어 위임군단*/1.5f, /*컴퓨터*/2.0f };

	/// 제한비율 조정조건 : 주장능력치
	// 능력조건 조정값 = 주장의 적용능력 능력치 / 능력 기준값
	// 主将能力値（统率）
	const array<bool> 비율조정_능력조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<float> 비율조정_능력조건_기준값 = {/*플레이어 1군단*/80.0f, /*플레이어 위임군단*/80.0f, /*컴퓨터*/50.f };
	const array<float> 비율조정_능력조건_최소값 = {/*플레이어 1군단*/0.5f, /*플레이어 위임군단*/0.5f, /*컴퓨터*/0.8f };
	const array<float> 비율조정_능력조건_최대값 = {/*플레이어 1군단*/1.5f, /*플레이어 위임군단*/1.5f, /*컴퓨터*/2.0f };
	const int 비율조정_능력조건_적용능력 = 무장능력_통솔;  //由统率決定

	/// 제한비율 조정조건 : 부대기력
	// 기력조건 조정값 = 부대기력 / 기력 기준값
	// 部队气力
	const array<bool> 비율조정_기력조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const array<float> 비율조정_기력조건_기준값 = {/*플레이어 1군단*/60.0f, /*플레이어 위임군단*/60.0f, /*컴퓨터*/30.f };
	const array<float> 비율조정_기력조건_최소값 = {/*플레이어 1군단*/0.5f, /*플레이어 위임군단*/0.5f, /*컴퓨터*/0.8f };
	const array<float> 비율조정_기력조건_최대값 = {/*플레이어 1군단*/1.5f, /*플레이어 위임군단*/1.5f, /*컴퓨터*/2.0f };

	/// 제한비율 조정조건 : 무장특기
	// 특기 추가 및 삭제 가능 : 동일한 형식으로 추가 (특기 이름은 'kr enums.cpp' 참고)
	// 武将特技
	const array<bool> 비율조정_특기조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 비율조정값_특기 =
	{
		{특기_운반, array<float> = {/*플레이어 1군단*/2.0f, /*플레이어 위임군단*/2.0f, /*컴퓨터*/2.5f} }
	};

	/// 제한비율 조정조건 : 세력기교
	// 기교 추가 및 삭제 가능 : 동일한 형식으로 추가 (기교 이름은 'kr enums.cpp' 참고)
	// 势力技巧
	const array<bool> 비율조정_기교조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 비율조정값_기교 =
	{
		{기교_목우유마, array<float> = {/*플레이어 1군단*/1.2f, /*플레이어 위임군단*/1.2f, /*컴퓨터*/1.4f} },
		{기교_숙련병,   array<float> = {/*플레이어 1군단*/1.1f, /*플레이어 위임군단*/1.1f, /*컴퓨터*/1.2f} }
	};

	/// 제한비율 조정조건 : 계절
	// 계절 추가 및 삭제 가능 : 동일한 형식으로 추가 (계절 이름은 'kr enums.cpp' 참고)
	// 季节影响
	const array<bool> 비율조정_계절조건_적용 = {/*플레이어 1군단*/true, /*플레이어 위임군단*/true, /*컴퓨터*/true };
	const dictionary 비율조정값_계절 =
	{
		{계절_봄,   array<float> = {/*플레이어 1군단*/1.0f, /*플레이어 위임군단*/1.0f, /*컴퓨터*/1.2f} },
		{계절_여름, array<float> = {/*플레이어 1군단*/0.8f, /*플레이어 위임군단*/0.8f, /*컴퓨터*/1.0f} },
		{계절_가을, array<float> = {/*플레이어 1군단*/1.0f, /*플레이어 위임군단*/1.0f, /*컴퓨터*/1.2f} },
		{계절_겨울, array<float> = {/*플레이어 1군단*/0.6f, /*플레이어 위임군단*/0.6f, /*컴퓨터*/0.8f} }
	};

	/// -------------------------------------------------------------------------------------------------------------------

		/// 소부대 수송 시
		// 소규모 수송부대가 관문, 항구에서 출진 시 수송량 제한 적용 여부 설정 (거점 3칸 이내 적부대 부재 시)
		// 小规模港?部队输送时是否也使用調整（避免AI港?运錢失敗）
	const array<bool> 允许小部队输送 = {/*플레이어 1군단*/false, /*플레이어_위임군단*/true, /*컴퓨터*/true };
	const int 小部队输送兵力 = 500;

	///	===================================================================================================================

	const int 항목_금 = 12, 항목_병량 = 13, 항목_병력 = 14, 항목_끝 = 15;

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

				// 항목 색인(item_index) : 0(검), 1(창), 2(극), 3(노), 4(군마), 5(충차), 6(정란), 7(투석), 8(목수), 9(주가), 10(누선), 11(투함), 12(금), 13(병량), 14(병력)
				// 분류 색인(category_index) : 0(일반병기), 1(공성병기), 2(함선), 3(병력), 4(금), 5(병량)
		array<array<int>> item_index_arr = { {병기_검, 병기_창, 병기_극, 병기_노, 병기_군마}, {병기_충차, 병기_정란, 병기_투석, 병기_목수}, {병기_주가, 병기_누선, 병기_투함}, {항목_금}, {항목_병량}, {항목_병력} };

		int unit_id = -1;
		int unit_energy = 0;
		array<float> 수송량_실제(항목_끝, 0.0f);  //实际输送量
		array<float> 수송량_최대(항목_끝, 0.0f);  //最大输送量
		array<float> 수송량_초과(항목_끝, 0.0f);  //输送超出量
		array<float> 거점복원정보(항목_끝, 0.0f);  //据点复原信息
		array<float> 기존최대수송량 = {/*일반병기*/100000, /*공성병기*/100, /*함선*/100, /*금*/100000, /*병량*/500000, /*병력*/60000 };  //原版最大输送量

///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			pk::bind(170, pk::trigger170_t(부대생성_수송량_제한));
			pk::bind(172, pk::trigger172_t(부대좌표변경_수송량_거점복원));
			pk::bind(171, pk::trigger171_t(부대제거_수송량_거점복원));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
				//	部队生成时输送量限制
		void 부대생성_수송량_제한(pk::unit@ unit, int type)	// type - 0 출진, 1 임의
		{
			func_정보_초기화();

			if (type != 0) return;
			if (unit.type != 부대종류_수송) return;

			pk::force@ force = pk::get_force(unit.get_force_id());
			if (!pk::is_alive(force)) return;
			if (!pk::is_normal_force(unit.get_force_id())) return;

			pk::building@ building = pk::get_building(pk::get_service(unit));
			if (!pk::is_alive(building)) return;

			int controller_index = get_controller_index(unit);
			if (func_소부대수송_조건충족(unit, building, controller_index)) return;

			// 确认输送量
			func_수송량_확인(unit, building, force, controller_index);
			if (!func_초과항목_존재여부()) return;

			// 調整输送量
			func_수송량_조정(unit);
			func_메시지_표시(unit, building, controller_index);
		}

		void 부대좌표변경_수송량_거점복원(pk::unit@ unit, const pk::point& in pos)	// pos - 부대의 이동 후 최종 위치
		{
			if (unit.type != 부대종류_수송) return;

			pk::building@ building = pk::get_building(pk::get_service(unit));
			if (!pk::is_alive(building)) return;
			if (building.pos != unit.pos) return;	// 출진 중인 부대가 아닌 경우 제외
			if (pos == building.pos) return;	// 부대가 이동하지 않은 경우 제외

			func_수송량_거점복원(unit, building);
		}

		/// 참고 : 부대가 출진 후 바로 제거되는 경우에는 부대의 위치를 출진거점으로 인식
		void 부대제거_수송량_거점복원(pk::unit@ unit, pk::hex_object@ dst, int type)	// type - 0 괴멸, 1 진입
		{
			if (type == 0 and unit.troops != 0) return;	// 출진취소로 부대가 제거(괴멸)되는 경우 제외
			if (unit.type != 부대종류_수송) return;

			pk::building@ building_unit_pos = pk::get_building(unit.pos);
			if (!pk::is_alive(building_unit_pos)) return;

			pk::building@ building_unit_service = pk::get_building(pk::get_service(unit));
			if (!pk::is_alive(building_unit_service)) return;
			if (@building_unit_pos != @building_unit_service) return;	// 출진 후 다른 거점으로 바로 진입하는 경우, 출진 후 바로 괴멸되는 경우 예외처리

			func_수송량_거점복원(unit, building_unit_service);
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		bool func_소부대수송_조건충족(pk::unit@ unit, pk::building@ building, int controller_index)
		{
			if (!允许小部队输送[controller_index]) return false;
			if (unit.troops >= 小部队输送兵力) return false;
			if (building.facility == 시설_도시) return false;
			if (pk::enemies_around(building)) return false;	// 건물 주변 3칸 이내

			return true;
		}

		// 确认输送量
		void func_수송량_확인(pk::unit@ unit, pk::building@ building, pk::force@ force, int controller_index)
		{
			for (int category_index = 5; category_index >= 0; category_index--)  //遍历檢cha各类資源兵装
			{
				if (!限制输送量开关[category_index][controller_index]) continue;  //如果设置不檢cha则跳過

				for (int i = 0; i < int(item_index_arr[category_index].length); i++)  //各大类下項目遍历檢察
				{
					int item_index = item_index_arr[category_index][i];
					if (category_index == /* 금 */3) 수송량_실제[item_index] = unit.gold;
					else if (category_index == /*병량*/4) 수송량_실제[item_index] = unit.food;
					else if (category_index == /*병력*/5) 수송량_실제[item_index] = unit.troops;
					else                                  수송량_실제[item_index] = unit.stock[item_index].amount;

					float limit_ratio = get_limit_ratio(unit, force, category_index, controller_index);
					수송량_최대[item_index] = 기존최대수송량[category_index] * limit_ratio;

					if (비율조정_병력조건_적용[controller_index] and item_index != 항목_병력)
						수송량_최대[item_index] *= pk::max(비율조정_병력조건_최소값[controller_index], pk::min(비율조정_병력조건_최대값[controller_index], unit.troops / 수송량_최대[항목_병력]));

					수송량_최대[item_index] = pk::min(기존최대수송량[category_index], 수송량_최대[item_index]);
					수송량_초과[item_index] = pk::max(0, int(수송량_실제[item_index]) - int(수송량_최대[item_index]));
				}
			}
		}

		float get_limit_ratio(pk::unit@ unit, pk::force@ force, int category_index, int controller_index)
		{
			float limit_ratio = 限制比例_基础量[category_index][controller_index];
			pk::person@ leader = pk::get_person(unit.leader);
			int leader_stat = leader.stat[비율조정_능력조건_적용능력];
			if (비율조정_능력조건_적용[controller_index]) limit_ratio *= pk::max(비율조정_능력조건_최소값[controller_index], pk::min(비율조정_능력조건_최대값[controller_index], (pk::max(1, leader_stat) / 비율조정_능력조건_기준값[controller_index])));
			if (비율조정_기력조건_적용[controller_index]) limit_ratio *= pk::max(비율조정_기력조건_최소값[controller_index], pk::min(비율조정_기력조건_최대값[controller_index], (pk::max(1, unit.energy) / 비율조정_기력조건_기준값[controller_index])));
			if (비율조정_특기조건_적용[controller_index]) limit_ratio *= get_dict_value(비율조정값_특기, unit, force, controller_index, /*특기*/0);
			if (비율조정_기교조건_적용[controller_index]) limit_ratio *= get_dict_value(비율조정값_기교, unit, force, controller_index, /*기교*/1);
			if (비율조정_계절조건_적용[controller_index]) limit_ratio *= get_dict_value(비율조정값_계절, unit, force, controller_index, /*계절*/2);

			limit_ratio = pk::min(限制比例_最大量[category_index][controller_index], limit_ratio);
			limit_ratio = pk::max(限制比例_最小量[category_index][controller_index], limit_ratio);
			return (limit_ratio / 100.f);
		}

		float get_dict_value(dictionary dict, pk::unit@ unit, pk::force@ force, int controller_index, int condition_index)
		{
			float n = 1.f;
			bool 조건충족여부 = false;
			array<string> index_name_arr = dict.getKeys();
			for (int i = 0; i < int(index_name_arr.length); i++)
			{
				int index_value = parseInt(index_name_arr[i]);
				if (condition_index == /*특기*/0) 조건충족여부 = unit.has_skill(index_value);
				else if (condition_index == /*기교*/1) 조건충족여부 = pk::has_tech(force, index_value);
				else if (condition_index == /*계절*/2) 조건충족여부 = (pk::get_season() == index_value);
				if (!조건충족여부) continue;

				array<float> dict_value_arr = array<float>(dict[index_name_arr[i]]);
				float dict_value = dict_value_arr[controller_index];
				n *= dict_value;
			}

			return n;
		}

		// 調整输送量
		void func_수송량_조정(pk::unit@ unit)
		{
			unit_id = unit.get_id();
			unit_energy = unit.energy;
			for (int item_index = 0; item_index < 항목_끝; item_index++)
			{
				if (수송량_초과[item_index] <= 0) continue;

				거점복원정보[item_index] = 수송량_초과[item_index];
				if (item_index < 병기_끝)    pk::set_weapon_amount(unit, item_index, item_index, int(수송량_최대[item_index]));
				else if (item_index == 항목_금)   pk::set_gold(unit, int(수송량_최대[item_index]));
				else if (item_index == 항목_병량) pk::set_food(unit, int(수송량_최대[item_index]));
				else if (item_index == 항목_병력) pk::set_troops(unit, int(수송량_최대[item_index]));
			}
		}

		// 显示消息报文
		void func_메시지_표시(pk::unit@ unit, pk::building@ building, int controller_index)
		{
			if (!unit.is_player()) return;

			string building_name = "\x1b[1x" + pk::decode(pk::get_name(building)) + "\x1b[0x";
			array<string> excess_item_arr(0);
			array<string> item_name_arr = { "剑", "枪", "戟", "弓", "军马", "冲车", "井栏", "投石", "木兽", "走舸", "楼船", "斗舰", "资金", "兵粮", "兵力" };
			for (int item_index = 0; item_index < 항목_끝; item_index++)
			{
				if (수송량_초과[item_index] > 0)
					excess_item_arr.insertLast((item_index < 병기_끝 ? "\x1b[1x" : "\x1b[2x") + item_name_arr[item_index]);
			}

			string warning_msg = "\x1b[0x运输部队的运输量\n\x1b[29x超出\x1b[0x了辎重水平!\x1b[0x(" + building_name + ")";
			string excess_info = "\x1b[0x超出项目:\n" + join(excess_item_arr, "\x1b[0x,");
			string max_info = "\x1b[0x<最大运输量(\x1b[2x兵力\x1b[0x" + unit.troops + "要求)>\n";
			max_info += "\x1b[1x普通兵器\x1b[0x" + int(수송량_최대[병기_창]) + ",";
			max_info += "\x1b[1x攻城兵器\x1b[0x" + int(수송량_최대[병기_충차]) + ",";
			max_info += "\x1b[1x舰船\x1b[0x" + int(수송량_최대[병기_누선]) + ",\n";
			max_info += "\x1b[2x资金\x1b[0x" + int(수송량_최대[항목_금]) + ",";
			max_info += "\x1b[2x兵粮\x1b[0x" + int(수송량_최대[항목_병량]);

			if (메시지표시_초과경고[controller_index]) pk::message_box(pk::encode(warning_msg), pk::get_person(unit.leader));
			if (메시지표시_초과항목[controller_index]) pk::message_box(pk::encode(excess_info), pk::get_person(unit.leader));
			if (메시지표시_최대정보[controller_index]) pk::message_box(pk::encode(max_info), pk::get_person(unit.leader));
		}

		// 是否显示附加項目
		bool func_초과항목_존재여부()
		{
			for (int item_index = 0; item_index < 항목_끝; item_index++)
			{
				if (수송량_초과[item_index] > 0) return true;
			}

			return false;
		}

		// 超出部分复原到据点
		void func_수송량_거점복원(pk::unit@ unit, pk::building@ building)
		{
			if (unit.get_id() != unit_id) return;

			for (int weapon_id = 0; weapon_id < 병기_끝; weapon_id++)
				pk::add_weapon_amount(building, weapon_id, int(거점복원정보[weapon_id]));
			pk::add_gold(building, int(거점복원정보[항목_금]));
			pk::add_food(building, int(거점복원정보[항목_병량]));
			ch::add_troops(building, int(거점복원정보[항목_병력]));
			pk::set_energy(building, unit_energy);	// 수송 병력이 거점 전체 병력인 경우 거점복원 시 기력이 0이 되는 버그 수정
		}

		// 情报初始化
		void func_정보_초기화()
		{
			unit_id = -1;
			unit_energy = 0;
			for (int item_index = 0; item_index < 항목_끝; item_index++)
				수송량_실제[item_index] = 수송량_최대[item_index] = 수송량_초과[item_index] = 거점복원정보[item_index] = 0.0f;
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		int get_controller_index(pk::unit@ unit)
		{
			return (pk::is_player_controlled(unit) ? /*플레이어 1군단*/0 : (unit.is_player() ? /*플레이어 위임군단*/1 : /*컴퓨터*/2));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	}

	Main main;
}
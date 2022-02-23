// ## 2021/10/24 # 江东新风 # 将pk::add_troops换成ch::add_troops以修正显示错误 ##
// ## 2021/10/24 #江东新风#改写171trigger##
// ## 2021/02/22 #江东新风#为登用概率添加调用类型##
// ## 2020/08/01 ##
/*
	@제작 : masterpiecek
	@참고 : 111, 168 함수 후킹 포함


	[ 급변사태 ]

	1. 거점 급변사태 : 적 세력 거점의 태수를 등용하여 해당 거점이 자세력 산하로 들어온 경우 급변사태 발생
	 * 등용에 성공한 무장이 플레이어 세력, 컴퓨터 세력인 경우에 대해 각각 설정 가능
	 1) 거점의 병력 및 물자(금, 병량, 병기) 손실
	 2) 도시인 경우 치안 재설정
	 3) 거점 소속 무장들의 의리에 따라 충성도 감소

	2. 부대 급변사태 : 적 세력 부대의 주장을 등용하여 해당 부대가 자세력으로 편입된 경우 급변사태 발생
	 * 등용에 성공한 무장이 플레이어 세력, 컴퓨터 세력인 경우에 대해 각각 설정 가능
	 1) 부대의 병력 및 물자(금, 병량) 손실
	 2) 일정 확률로 부대가 혼란상태에 빠짐
	 3) 부대 소속 무장들의 의리에 따라 충성도 감소
*/

namespace 급변사태
{

	/// ========================================= < 유저 설정 > =========================================

	/// [ 거점 급변사태 ]

		/// 거점 급변사태 전체 기능 적용여부 설정
	const array<bool> 거점_급변사태_전체_적용 = {/*플레이어*/true, /*컴퓨터*/false };

	/// 거점에서 급변사태 발생 시 효과(발생 시 화면이동, 발생 효과, 발생 효과음) 적용여부 설정
	const array<bool> 거점_급변사태_효과_적용 = {/*플레이어*/true, /*컴퓨터*/false };

	/// 거점에서 급변사태 발생 시 보고 메시지(메시지 박스, 로그 메시지) 표시여부 설정
	const array<bool> 거점_급변사태_메시지_표시 = {/*플레이어*/true, /*컴퓨터*/true };

	/// 거점에서 급변사태 발생 시 거점의 병력 및 물자 손실 비율 설정 (%)
	const array<int> 거점_급변사태_손실비율_병력 = {/*플레이어*/50, /*컴퓨터*/50 };
	const array<int> 거점_급변사태_손실비율_금 = {/*플레이어*/50, /*컴퓨터*/50 };
	const array<int> 거점_급변사태_손실비율_병량 = {/*플레이어*/50, /*컴퓨터*/50 };
	const array<int> 거점_급변사태_손실비율_병기 = {/*플레이어*/50, /*컴퓨터*/50 };

	/// 도시에서 거점 급변사태가 발생하는 경우 발생 후 도시의 치안값 설정
	const array<int> 거점_급변사태_발생후_도시치안 = {/*플레이어*/70, /*컴퓨터*/70 };

	/// 거점에서 급변사태 발생 시 거점 소속 무장들의 의리에 따른 충성도 하락 수치 설정
	const array<int> 거점_급변사태_충성도하락_의리_매우낮음 = {/*플레이어*/16, /*컴퓨터*/16 };
	const array<int> 거점_급변사태_충성도하락_의리_낮음 = {/*플레이어*/12, /*컴퓨터*/12 };
	const array<int> 거점_급변사태_충성도하락_의리_보통 = {/*플레이어*/8, /*컴퓨터*/8 };
	const array<int> 거점_급변사태_충성도하락_의리_높음 = {/*플레이어*/4, /*컴퓨터*/4 };
	const array<int> 거점_급변사태_충성도하락_의리_매우높음 = {/*플레이어*/0, /*컴퓨터*/0 };

	/// -------------------------------------------------------------------------------------------------

	/// [ 부대 급변사태 ]

		/// 부대 급변사태 전체 기능 적용여부 설정
	const array<bool> 부대_급변사태_전체_적용 = {/*플레이어*/true, /*컴퓨터*/false };

	/// 부대에서 급변사태 발생 시 효과(발생 시 화면이동, 발생 효과, 발생 효과음) 적용여부 설정
	const array<bool> 부대_급변사태_효과_적용 = {/*플레이어*/true, /*컴퓨터*/false };

	/// 부대에서 급변사태 발생 시 보고 메시지(메시지 박스, 로그 메시지) 표시여부 설정
	const array<bool> 부대_급변사태_메시지_표시 = {/*플레이어*/true, /*컴퓨터*/true };

	/// 부대에서 급변사태 발생 시 부대의 기력, 병력 및 물자 손실 비율 설정 (%)
	const array<int> 부대_급변사태_손실비율_기력 = {/*플레이어*/50, /*컴퓨터*/50 };
	const array<int> 부대_급변사태_손실비율_병력 = {/*플레이어*/50, /*컴퓨터*/50 };
	const array<int> 부대_급변사태_손실비율_금 = {/*플레이어*/50, /*컴퓨터*/50 };
	const array<int> 부대_급변사태_손실비율_병량 = {/*플레이어*/50, /*컴퓨터*/50 };

	/// 부대에서 급변사태가 발생 시 부대가 혼란 상태에 빠질 확률 설정
	const array<int> 부대_급변사태_부대혼란확률 = {/*플레이어*/20, /*컴퓨터*/20 };

	/// 부대에서 급변사태 발생 시 부대 소속 무장들의 의리에 따른 충성도 하락 수치 설정
	const array<int> 부대_급변사태_충성도하락_의리_매우낮음 = {/*플레이어*/16, /*컴퓨터*/16 };
	const array<int> 부대_급변사태_충성도하락_의리_낮음 = {/*플레이어*/12, /*컴퓨터*/12 };
	const array<int> 부대_급변사태_충성도하락_의리_보통 = {/*플레이어*/8, /*컴퓨터*/8 };
	const array<int> 부대_급변사태_충성도하락_의리_높음 = {/*플레이어*/4, /*컴퓨터*/4 };
	const array<int> 부대_급변사태_충성도하락_의리_매우높음 = {/*플레이어*/0, /*컴퓨터*/0 };

	/// =================================================================================================

	class Main
	{

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		pk::func168_t@ prev_callback_168;
		pk::func111_t@ prev_callback_111;

		array<bool> is_taishu(무장_끝, false);
		array<bool> is_leader(무장_끝, false);
		pk::point object_pos_for_screen_move;

		array<array<bool>> 급변사태_전체_적용 = { 거점_급변사태_전체_적용, 부대_급변사태_전체_적용 };
		array<array<bool>> 급변사태_효과_적용 = { 거점_급변사태_효과_적용, 부대_급변사태_효과_적용 };
		array<array<bool>> 급변사태_메시지_표시 = { 거점_급변사태_메시지_표시, 부대_급변사태_메시지_표시 };
		array<array<int>> 거점_급변사태_의리별_충성도하락 = { 거점_급변사태_충성도하락_의리_매우낮음, 거점_급변사태_충성도하락_의리_낮음, 거점_급변사태_충성도하락_의리_보통, 거점_급변사태_충성도하락_의리_높음, 거점_급변사태_충성도하락_의리_매우높음 };
		array<array<int>> 부대_급변사태_의리별_충성도하락 = { 부대_급변사태_충성도하락_의리_매우낮음, 부대_급변사태_충성도하락_의리_낮음, 부대_급변사태_충성도하락_의리_보통, 부대_급변사태_충성도하락_의리_높음, 부대_급변사태_충성도하락_의리_매우높음 };

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		Main()
		{
			@prev_callback_111 = cast<pk::func111_t@>(pk::get_func(111));
			@prev_callback_168 = cast<pk::func168_t@>(pk::get_func(168));
			pk::reset_func(111);
			pk::reset_func(168);
			pk::set_func(111, pk::func111_t(func111));
			pk::set_func(168, pk::func168_t(func168));

			pk::bind(103, pk::trigger103_t(시나리오시작_무장정보_갱신));
			pk::bind(111, pk::trigger111_t(턴시작_무장정보_갱신));
			pk::bind(112, pk::trigger112_t(턴종료_무장정보_갱신));
			pk::bind(170, pk::trigger170_t(부대생성_무장정보_갱신));
			pk::bind(171, pk::trigger171_t(부대제거_무장정보_갱신));

			pk::bind(152, pk::trigger152_t(무장세력변경_급변사태));
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		/*
			[ 거점 급변사태 ]

			1. 기본 동작원리
			 1) 무장의 세력이 변경되는 순간,               (무장세력변경 이벤트 트리거 사용)
			 2) 저장되어 있던 무장의 지위가 태수인 경우,   (거점별 태수정보를 저장하여 활용)
			 3) 무장의 소재거점이 자세력으로 인식되면 발생 (태수를 등용해도 거점이 자세력으로 편입되지 않는 경우가 있기 때문에 조건 추가 필요함)

			2. 참고 사항
			 1) 적 거점의 태수가 도독 신분인 경우 무장의 세력이 변경되는 순간 소속거점이 기존에 자신이 소속되어 있던 소속거점이 아닌 무장의 기존 세력의 군주 소속거점으로 인식됨
			  - 처리 : 이벤트가 발생하는 기준 거점 변수를 무장의 소속거점이 아닌 소재거점 사용
			 2) 적거점을 공격해서 제압(마지막 거점을 제압하여 멸망시키는 경우 포함)한 뒤 태수였던 무장이 포로가 된 후, 해당 턴(소요 일수 0일)에 바로 다른 아군 무장으로 해당 무장의 등용을 시도하는 경우에는 무장의 태수정보가 갱신되지 못함
			  - 처리 : 거점이 제압되는 경우 정보갱신 (168 함수 후킹)


			[ 부대 급변사태 ]

			1. 기본 동작원리
			 1) 무장의 세력이 변경되는 순간,               (무장세력변경 이벤트 트리거 사용)
			 2) 저장되어 있던 무장의 지위가 주장인 경우,   (부대별 주장정보를 저장하여 활용)
			 3) 무장의 소재부대가 자세력으로 인식되면 발생 (부대의 주장이 등용되면 자동으로 부대가 자세력으로 편입되기 때문에 조건 추가 필요 없음)

			2. 참고 사항
			 1) 적 부대를 괴멸시켜 주장이었던 무장을 포획한 후 해당 턴(소요 일수 0일)에 바로 다른 아군 무장으로 해당 무장의 등용을 시도하는 경우 무장의 주장정보가 갱신되지 못함
			  - 처리 : 부대가 제거되는 경우 정보갱신 (부대제거 이벤트 트리거 사용)
			 2) 적 부대를 괴멸시킴과 동시에 해당 부대의 주장을 포획한 후, (공격으로 괴멸시켜 포획하는 경우 및 일기토를 통해 적 부대 주장을 포획하는 경우) 포로처우 커맨드를 통해 등용을 시도하는 경우 무장의 주장정보가 갱신되지 못함
			  - 처리 : 포로처우 등용이 시도되는 경우 정보갱신 (111 함수 후킹)
		*/

		bool func111(pk::person@ target, pk::person@ actor, int type, int seed, int rettype)
		{
			bool return_value = prev_callback_111(target, actor, type, seed, rettype);

			// 부대 급변사태 예외 처리 : 포로처우 시
			if (target.location >= 据点_末 and actor.mibun == 身份_君主 and type == /*포로처우 등용*/1)
			{
				pk::unit@ unit = pk::get_unit(pk::get_unit_id(target));
				if (unit.leader == target.get_id()) is_leader[target.get_id()] = false;
			}

			return return_value;
		}

		void func168(pk::building@ base, pk::unit@ attacker)
		{
			prev_callback_168(base, attacker);

			// 거점 급변사태 예외 처리 : 거점 제압 시
			func_무장정보_갱신();
		}

		void 시나리오시작_무장정보_갱신() { func_무장정보_갱신(); }
		void 턴시작_무장정보_갱신(pk::force@ force) { func_무장정보_갱신(); }
		void 턴종료_무장정보_갱신(pk::force@ force) { func_무장정보_갱신(); }
		void 부대생성_무장정보_갱신(pk::unit@ unit, int type) { func_무장정보_갱신(); }
		void 부대제거_무장정보_갱신(pk::unit@ unit, pk::hex_object@ dst, int type) { func_무장정보_갱신(); }

		void 무장세력변경_급변사태(pk::person@ person, pk::force@ force)
		{
			pk::hex_object@ hex_object = pk::get_hex_object(person.location);
			if (!func_급변사태_적용가능여부(person, force, hex_object)) return;
			if (person.location < 据点_末)
			{
				if (!is_taishu[person.get_id()]) return;
				if (hex_object.get_force_id() != force.get_id()) return;
			}
			else if (!is_leader[person.get_id()]) return;

			func_급변사태_효과(person, force, hex_object);
			func_급변사태_메시지(person, force, hex_object);

			if (person.location < 据点_末)
			{
				pk::building@ building = pk::hex_object_to_building(hex_object);
				func_거점_급변사태_결과(force, building);
			}
			else
			{
				pk::unit@ unit = pk::hex_object_to_unit(hex_object);
				func_부대_급변사태_결과(force, unit);
			}
		}

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		bool func_급변사태_적용가능여부(pk::person@ person, pk::force@ force, pk::hex_object@ hex_object)
		{
			if (pk::is_campaign()) return false;
			if (pk::is_mission_running()) return false;
			if (person.location == -1) return false;
			if (person.get_id() >= 700 and person.get_id() <= 799) return false;	// 이벤트 무장
			if (person.get_id() >= 1000 and person.get_id() <= 1099) return false;	// 적장
			if (!급변사태_전체_적용[get_index(hex_object)][get_index(force)]) return false;

			return true;
		}

		void func_무장정보_갱신()
		{
			for (int person_id = 0; person_id < 무장_끝; person_id++)
			{
				pk::person@ person = pk::get_person(person_id);
				if (person.location == -1)
				{
					is_taishu[person_id] = false;
					is_leader[person_id] = false;
				}
				else if (person.location >= 0 and person.location < 据点_末)	// 0~86
				{
					int m = person.mibun;
					is_taishu[person_id] = (m == 身份_君主 or m == 身份_都督 or m == 身份_太守) ? true : false;
					is_leader[person_id] = false;
				}
				else if (person.location >= 据点_末 and person.location < 据点_末 + 부대_최대)	// 87~1086
				{
					pk::unit@ unit = pk::get_unit(pk::get_unit_id(person));
					is_taishu[person_id] = false;
					is_leader[person_id] = (person_id == unit.leader) ? true : false;
				}
			}
		}

		void func_급변사태_효과(pk::person@ person, pk::force@ force, pk::hex_object@ hex_object)
		{
			if (!급변사태_효과_적용[get_index(hex_object)][get_index(force)]) return;

			object_pos_for_screen_move = hex_object.get_pos();
			pk::scene(cast<pk::scene_t@>(function() { pk::move_screen(main.object_pos_for_screen_move, 500); }));	// 작은 숫자일수록 빠른 이동

			if (pk::is_in_screen(hex_object.get_pos()))
			{
				int effect_range = person.location < 据点_末 ? 1 : 0;
				array<pk::point> range_arr = pk::range(hex_object.get_pos(), 0, effect_range);
				for (int i = 0; i < int(range_arr.length); i++)
					pk::create_effect(/*요술 당함*/0x59, range_arr[i]);
				pk::play_se(11);
				string people_name = person.location < 据点_末 ? "百姓" : "士兵";
				pk::say(pk::encode("快点把这些" + people_name + "安顿下来!"), person, null);
			}
		}

		void func_급변사태_메시지(pk::person@ person, pk::force@ force, pk::hex_object@ hex_object)
		{
			if (!급변사태_메시지_표시[get_index(hex_object)][get_index(force)]) return;

			pk::person@ force_kunshu = pk::get_person(force.kunshu);
			pk::person@ force_gunshi = pk::get_person(force.gunshi);
			pk::person@ reporter = pk::is_alive(force_gunshi) ? force_gunshi : pk::get_person(무장_전령);
			pk::force@ person_force = pk::get_force(person.get_force_id());
			string kunshu_name = pk::decode(pk::get_name(force_kunshu));
			string object_name = pk::decode(pk::get_name(hex_object));
			string people_name = person.location < 据点_末 ? "百姓" : "士兵";

			string msg_for_box = "\x1b[1x" + kunshu_name + "\x1b[0x" + (force.is_player() ? "大人," : "军队");
			msg_for_box += "在\x1b[1x" + object_name + "\x1b[0x出现了\x1b[29x士兵哗变\x1b[0x";
			msg_for_box += "\n因此" + (force.is_player() ? "导致了" + people_name + "们的混乱和不安" : "已收到了确切的情报") + "!";
			string msg_for_log1 = "\x1b[2x" + object_name + "\x1b[0x被\x1b[2x" + kunshu_name + "军\x1b[0x收编";
			string msg_for_log2 = "\x1b[2x" + kunshu_name + "军\x1b[0x的\x1b[2x" + object_name + "\x1b[0x出现了\x1b[2x士兵哗变\x1b[0x";

			pk::message_box(pk::encode(msg_for_box), reporter);
			if (person.location >= 据点_末) pk::history_log(hex_object.get_pos(), person_force.color, pk::encode(msg_for_log1));
			pk::history_log(hex_object.get_pos(), force.color, pk::encode(msg_for_log2));
		}

		void func_거점_급변사태_결과(pk::force@ force, pk::building@ building)
		{
			ch::add_troops(building, -(pk::get_troops(building) * 거점_급변사태_손실비율_병력[get_index(force)] / 100), true, 102);
			pk::add_gold(building, -(pk::get_gold(building) * 거점_급변사태_손실비율_금[get_index(force)] / 100), true);
			pk::add_food(building, -(pk::get_food(building) * 거점_급변사태_손실비율_병량[get_index(force)] / 100), true);
			for (int i = 0; i < 병기_끝; i++)
				pk::add_weapon_amount(building, i, -(pk::get_weapon_amount(building, i) * 거점_급변사태_손실비율_병기[get_index(force)] / 100), true);

			pk::city@ city = pk::building_to_city(building);
			if (pk::is_alive(city)) pk::set_public_order(city, 거점_급변사태_발생후_도시치안[get_index(force)]);

			uint mibun_flags = pk::mibun_flags(身份_都督, 身份_太守, 身份_一般);
			array<pk::person@> person_arr = pk::list_to_array(pk::get_person_list(building, mibun_flags));
			for (int i = 0; i < int(person_arr.length); i++)
			{
				pk::person@ person = person_arr[i];
				pk::add_loyalty(person, -거점_급변사태_의리별_충성도하락[person.giri][get_index(force)]);
			}
		}

		void func_부대_급변사태_결과(pk::force@ force, pk::unit@ unit)
		{
			pk::add_energy(unit, -(unit.energy * 부대_급변사태_손실비율_기력[get_index(force)] / 100), true);
			ch::add_troops(unit, -(unit.troops * 부대_급변사태_손실비율_병력[get_index(force)] / 100), true, 102);
			pk::add_gold(unit, -(unit.gold * 부대_급변사태_손실비율_금[get_index(force)] / 100), true);
			pk::add_food(unit, -(unit.food * 부대_급변사태_손실비율_병량[get_index(force)] / 100), true);

			if (pk::rand_bool(부대_급변사태_부대혼란확률[get_index(force)])) unit.status = 部队状态_混乱;

			for (int i = 0; i < pk::get_member_count(unit); i++)
			{
				pk::person@ person = pk::get_person(unit.member[i]);
				pk::add_loyalty(person, -부대_급변사태_의리별_충성도하락[person.giri][get_index(force)]);
			}
		}

		int get_index(pk::hex_object@ hex_object) { return (pk::get_hex_object_id(hex_object) < 据点_末 ? /*거점*/0 : /*부대*/1); }
		int get_index(pk::force@ force) { return (force.is_player() ? /*플레이어*/0 : /*컴퓨터*/1); }

		///	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	}

	Main main;
}
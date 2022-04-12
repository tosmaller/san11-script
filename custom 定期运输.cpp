// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2020/11/10 # messi # 搬运naver可自定义一定时间点定期自动运输 ##
/*
정기배송 : 위임군단에서 정기적으로 골드, 병량, 무기를 지정도시로 배송
 - 만든이 : 버러풀
 - 버전 1.1
	- 도시에 남겨둘 무기 항목 추가
*/

namespace ori정기배송
{
	const int remain_gold = 1000; // 도시에 남겨둘 금
	const int remain_food = 10000; // 도시에 남겨둘 병량
	const int remain_troops = 5000; // 도시에 남겨둘 병력
	const int remain_weapon = 5000; // 도시에 남겨둘 무기(창,극,노,군마)
	const array<int> months = { 1, 4, 7, 10 }; // 실행할 달

	class Ori
	{
		// 생성자
		Ori()
		{
			// 생성자
			pk::bind(111, pk::trigger111_t(cbTurnStart));
		}

		// 턴 종료
		void cbTurnStart(pk::force@ force)
		{
			if (!force.is_player())
				return;
			if (months.find(pk::get_month()) >= 0 && pk::get_day() == 1) // 지정된 달
				start(force);
		}

		// 작업 시작
		void start(pk::force@ force)
		{
			pk::list<pk::district@> dlist = pk::get_district_list(force); // 군단 목록 읽기
			for (int i = 0; i < dlist.count; i++)
			{
				pk::district@ d = dlist[i];
				if (isCondition(d))// 조건?
					doIt(d);
			}
		}

		// 조건 확인
		bool isCondition(pk::district@ d)
		{
			return d.policy == 1 and !d.attack and d.transport_building > -1; // 위임 + 공격금지 + 수송도시 존재
		}

		// 배송 출발
		void doIt(pk::district@ d)
		{
			// 각 도시 루프
			pk::list<pk::city@> clist = pk::get_city_list(d);
			int pid;
			for (int i = 0; i < clist.count; i++)
			{
				pk::city@ c = clist[i];
				pk::building@ b = pk::city_to_building(c); // 건물

				// 장수가 존재하는지
				pid = getDeliveryMan(b);
				if (pid == -1) // 담당할 애가 없다
				{
					continue;
				}
				pk::person@ p = pk::get_person(pid);

				// 추르바르
				goDeilivery(b, pid, d.transport_building);
			}
		}

		// 배송하기
		bool goDeilivery(pk::building @ b, int person, int target)
		{
			int troops = pk::get_troops(b);
			if (troops <= remain_troops) // 무조건 병사는 남겨둘 병력보다 1명이라도 더 많아야한다
				return false;

			// 출진 명령 정보 생성.
			pk::com_deploy_cmd_info cmd;
			@cmd.base = @b;
			cmd.member[0] = person;
<<<<<<< HEAD
			cmd.type = 부대종류_수송;
=======
			cmd.type = 部队类型_运输;
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			cmd.order = 部队任务_移动;
			cmd.target_pos = pk::get_building(target).get_pos();

			// 배송 상품
			cmd.troops = troops - remain_troops;
			cmd.gold = 0;
			cmd.food = 0;

			int gold = pk::get_gold(b);
			if (remain_gold < gold)
				cmd.gold = gold - remain_gold;

			int food = pk::get_food(b);
			if (remain_gold < gold)
				cmd.food = food - remain_food;

			// 무기 전부
			int i = 0;
			for (int weapon_id = 0; weapon_id < 병기_끝; weapon_id++)
			{
				int weapon_amount = pk::get_weapon_amount(b, weapon_id);
				if (weapon_amount > 0)
				{
<<<<<<< HEAD
					if (weapon_id < 병기_창 or 병기_군마 < weapon_id)
=======
					if (weapon_id < 兵器_枪 or 兵器_战马 < weapon_id)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
					{
						cmd.weapon_id[i] = weapon_id;
						cmd.weapon_amount[i] = weapon_amount;
						i++;
					}
					else if (weapon_amount > remain_weapon)
					{
						cmd.weapon_id[i] = weapon_id;
						cmd.weapon_amount[i] = weapon_amount - remain_weapon;
						i++;
					}
				}
			}

			// 출진.
			int unit_id = pk::command(cmd);
			if (unit_id != -1)
				pk::get_unit(unit_id).action_done = true;
			return true;
		}

		// 택배기사 선정
		int getDeliveryMan(pk::building@ b)
		{
			int ret = -1;
			int min = 0;
			int min2;
			pk::list<pk::person@> list = pk::get_idle_person_list(b);
			for (int i = 0; i < list.count; i++)
			{
				pk::person@ p = list[i];

				// 운반 특기
				if (p.skill == 특기_운반)
					return p.get_id();

				// 지력+정치력
				min2 = p.stat[武将能力_智力] + p.stat[武将能力_政治];
				if (min == 0 or min > min2)
				{
					min = min2;
					ret = p.get_id();
				}
			}

			return ret;
		}
	}

	Ori ori;
}
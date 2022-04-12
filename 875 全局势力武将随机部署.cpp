<<<<<<< HEAD
﻿// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
=======
﻿// ## 2022/03/28 #江东新风# 默认关闭 ##
// ## 2022/03/16 #江东新风# 随机分配爵位bug ##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
// ## 2020/08/23 # messi # 统一了下选擇格式 ##
// ## 2020/08/01 ##
/*
개판만들기 : 세력 셔플
 - 만든이 : 버러풀
 - 버전 1.1
	- 1.1 : 세력재배치와 충돌하는 버그 개선
*/

namespace ori개판만들기
{

	// 시작 클래스
	class Ori
	{
		bool bNewGame = false;
		Ori()
		{
			// 생성자
<<<<<<< HEAD
			pk::bind(102, pk::trigger102_t(cbGameStart)); // 게임 첫시작
			pk::bind(111, pk::trigger111_t(cbTurnStart)); // 턴시작
=======
			//pk::bind(102, pk::trigger102_t(cbGameStart)); // 게임 첫시작
			//pk::bind(111, pk::trigger111_t(cbTurnStart)); // 턴시작
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		}

		// 게임 시작
		void cbGameStart()
		{
			// 초기화
			if (pk::get_scenario().loaded or pk::get_scenario().no != 7) // 로드 or 영웅집결 아닐시 패스
				return;
			//ch::u8debug(" - cbGameStart");
			bNewGame = true;
		}

		// 턴 시작
		void cbTurnStart(pk::force@ force)
		{
			if (!bNewGame) // 처음 시작시에만
				return;
			bNewGame = false;
			if (pk::choose({ pk::encode("  是  "), pk::encode("  否  ") }, pk::encode("是否要打乱分配势力和武将?"), pk::get_person(무장_강유)) == 0)
			{
				// 작업 시작
				Work work;
			}
		}
	}

	// 인물 고유설정
	class Ps
	{
		int no;
		int color; // 색
		int title; // 작위
		int kok; // 국호

		Ps() {}

<<<<<<< HEAD
		Ps(int n, int c = -1, int t = -1, int k = -1)
=======
		Ps(int n, int c = -1, int t = 9, int k = -1)
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
		{
			no = n;
			color = c;
			title = t;
			kok = k;
		}
	}

	// 세력 정보
	class Force
	{
		int id; // 세력 아이디
		int lord; // 군주
		int district; // 군단
		int location; // 현재 위치. service와 동일값

		Force() {}

		Force(pk::force@ f)
		{
			id = f.get_id();
			lord = f.kunshu;

			pk::person@ p = pk::get_person(lord);
			district = p.district;
			location = p.location;
		}

		// 새 인물로 군주 교체하기
		void makeLord(int person)
		{
			if (person == lord) // 혹시 모르지
			{
				return;
			}

			// 기존 군주
			pk::person@ op = pk::get_person(lord);
			if (op.location == location)
			{
				op.mibun = 身份_一般;
				op.rank = 官职_无;
				op.loyalty = 100;
			}

			pk::person@ np = pk::get_person(person); // 새 군주
			np.district = district;
			np.location = location;
			np.service = location;
			np.mibun = 身份_君主;
			np.rank = 官职_无;
			np.loyalty = 100;
			np.former_force = -1;

			// 세력 군주
			pk::force@ f = pk::get_force(id);
			f.kunshu = person;
			f.update();

			// 태수 변경
			pk::city@ c = pk::get_city(location);
			c.taishu = person;
			c.update();

			lord = person;
		}

		// 세력 비우기
		void clear()
		{
			pk::force@ f = pk::get_force(id);
			f.gunshi = -1; // 군사 제거
			f.kokugou = -1; // 국호 제거
			f.title = -1; // 관직 제거(혹시 모르니)

			// 일반 장수들 목록
			pk::list<pk::person@> ps = pk::get_person_list(f, pk::mibun_flags(身份_一般));
			for (int i = 0; i < ps.count; i++)
			{
				pk::person@ p = ps[i];
				fire(p);
			}

			// 기교 초기화
			for (int i = 0; i < 64; i++)
				f.tech[i] = false;
		}

		// 넌 해고야!
		void fire(pk::person@ p)
		{
			p.district = -1;
			p.mibun = 신분_미발견;
			p.rank = 官职_无;
			p.location = pk::rand(도시_끝); // 임의의 도시 소속으로
			p.service = p.location;
			p.loyalty = 0;
			p.former_force = -1;
		}

		// 세력 설정
		void set(Ps@ ps)
		{
			pk::force@ f = pk::get_force(id);
			f.color = ps.color;
			f.kokugou = ps.kok;
			f.title = ps.title;
		}
	}

	// 세력에 부하 설정 클라스
	class PersonSelector
	{
		Force mForce; // 대상 세력
		int mCount; // 부하 무장 수
		int mRemain; // 남은 자릿수
		array<int> marr; // 무장 저장

		PersonSelector(Force f, int count)
		{
			mForce = f;
			mCount = count;
		}

		// 부하 장수들 투입
		void putPeople()
		{
			mRemain = mCount - getPeopleCount();

			pk::list<pk::person@> ps = pk::get_person_list(pk::mibun_flags(신분_재야, 신분_미발견));
			pk::random ra(pk::rand());

			int pscnt = ps.count - 1;
			int idx, no;
			while (mRemain > 0 && pscnt > 0)
			{
				idx = ra(0, pscnt);
				pk::person@ p = ps[idx];
				no = p.get_id();
				if (!isOK(p) or marr.find(no) > -1)
				{
					no = getPinchHitter(ps, idx);
					if (no == -1) // 설마 이럴리는?
						break;
					put(no);
				}
				else
					put(p);

				// next
				mRemain--;
				pscnt--;

				putPartners(no);
			}

			// 초과시 자르기
			int pc = getPeopleCount();
			if (pc > mCount + 1) // 군주까지 +1
				cutOut();
		}

		// 초과 장수 잘라내기
		void cutOut()
		{
			pk::list<pk::person@> ps = pk::get_person_list(pk::get_force(mForce.id), pk::mibun_flags(身份_一般));
			int len = ps.count;

			int cnt = mCount - ps.count; // 잘라낼 수
			for (int i = 0; i < len; i++)
			{
				if (cnt < 1)
					break;
				if (isSingle(ps[i])) // 혼자야?
				{
					mForce.fire(ps[i]); // 자르기
					cnt--;
				}
			}
		}

		// 혼자인 것도 서러운데 잘려야하다니 ㅠㅠ
		bool isSingle(pk::person@ p)
		{
			if (p.spouse > -1)
				return false;

			pk::list<pk::person@> list = pk::get_gikyoudai_list(p);
			return list.count < 1;
		}

		// 현재 세력 소속
		int getPeopleCount()
		{
			pk::list<pk::person@> ps = pk::get_person_list(pk::get_force(mForce.id), pk::mibun_flags(身份_一般));
			return ps.count;
		}

		// 대타를 구해야한다
		int getPinchHitter(pk::list<pk::person@> list, int idx)
		{
			int i, no;

			// 뒤로 가기
			for (i = idx + 1; i < list.count; i++)
			{
				pk::person@ p = list[i];
				no = p.get_id();
				if (isOK(p) and marr.find(no) < 0)
					return no;
			}

			// 혹시 모르니 앞으로도 가기
			for (i = idx - 1; i >= 0; i--)
			{
				pk::person@ p = list[i];
				no = p.get_id();
				if (isOK(p) and marr.find(no) < 0)
					return no;
			}
			return -1;
		}

		// 반려/의형제 데려오기
		void putPartners(int no = -1)
		{
			pk::person@ p = pk::get_person(no > -1 ? no : mForce.lord); // 기본은 군주
			putPartners(p);
		}
		void putPartners(pk::person@ p)
		{
			// 반려
			if (p.spouse > -1)
			{
				pk::person@ pp = pk::get_person(p.spouse);
				if (isOK(pp))
				{
					put(pp);
					mRemain--;
					putPartners(pp);
				}
			}

			// 의형제
			pk::list<pk::person@> list = pk::get_gikyoudai_list(p);
			for (int i = 0; i < list.count; i++)
			{
				pk::person@ bro = list[i];
				if (isOK(bro))
				{
					put(bro);
					mRemain--;
					putPartners(bro);
				}
			}
		}

		// 부하로 편입하기
		void put(int no)
		{
			pk::person@ p = pk::get_person(no);
			put(p);
		}
		void put(pk::person@ p)
		{
			marr.insertLast(p.get_id());

			p.district = mForce.district;
			p.location = mForce.location;
			p.service = mForce.location;
			p.mibun = 身份_一般;
			p.rank = 官职_无;
			p.kouseki = 5000;
			p.loyalty = 200; // 개국공신특
			p.update();
		}

		// 부하 가능?
		bool isOK(int no)
		{
			pk::person@ p = pk::get_person(no);
			return isOK(p);
		}
		bool isOK(pk::person@ p)
		{
			return (p.mibun == 신분_재야 or p.mibun == 신분_미발견);
		}

	}

	// 작업 클래스
	class Work
	{
		// 작업 설정
		bool cfUserReset = false;
		int cfPerson;
		//int cfConcept;

		// 군주별 고유설정
		array<Ps> arrDef = {
			Ps(343,0), // 조조
			Ps(635,1), // 유비
			Ps(365,2), // 손견
			Ps(367,2), // 손권
			Ps(370,2), // 손책
			Ps(18,7), // 원소
			Ps(16,8), // 원술
			Ps(493,5), // 동탁
<<<<<<< HEAD
			Ps(660,10), // 여포
			Ps(403,28, 爵位_皇帝, 국호_황건), // 장각
=======
			Ps(660,10), // 여포吕布
			Ps(403,28, 爵位_皇帝, 국호_황건), // 장각张角
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
			Ps(517,3), // 마등
			Ps(180,11), // 공손찬
			Ps(225,15), // 사마의
			Ps(568,26) // 맹획
		};

		// data
		array<Force> marrForce; // 세력 정보
		array<Ps> marrLord; // 선발된 군주 목록
		array<int> marrLordNo; // 비교용 번호
		array<int> marrColor; // 세력색 체크용
		array<int> marrExceptCity; // 제외 도시

		Work()
		{
			// 현재는 기존 세력 교체밖에 못함
			// 설정
			pk::person@ p = pk::get_person(무장_강유);
			cfUserReset = pk::choose({ pk::encode(" 是 "), pk::encode(" 否 ") }, pk::encode("是否将玩家排除随机部署?"), p) == 1;
			cfPerson = pk::choose({ pk::encode("0名"), pk::encode("3名"), pk::encode("5名"), pk::encode("7名"), pk::encode("10名") }, pk::encode("请选择按势力分配的武将数量"), p);
			array<int> pc = { 0,3,5,7,10 };
			cfPerson = pc[cfPerson];

			// 작업 시작
			start();
		}

		void start()
		{
			// 작업 들어감

			// 현재 세력 수
			int nForceCount = readForces(); // 현재 세력 정보 읽기

			// 군주로 삼을 인물 뽑기
			selectLords(nForceCount);

			// 1. 군주 교체
			int lordCur = 0; // 군주 커서
			int i;
			for (i = 0; i < nForceCount; i++)
			{
				Force@ fc = marrForce[i];

				fc.makeLord(marrLordNo[lordCur]); // 군주 교체
				fc.clear(); // 세력 초기화
				fc.set(marrLord[lordCur]); // 이외 설정

				// next
				lordCur++;
			}

			if (cfPerson > 0)
			{
				// 2. 의형제, 부부 같은 세력 소속으로
				for (i = 0; i < nForceCount; i++)
				{
					Force@ fc = marrForce[i];
					PersonSelector sel(fc, cfPerson);
					sel.putPartners();
				}

				// 3. 부하 채워넣기
				for (i = 0; i < nForceCount; i++)
				{
					Force@ fc = marrForce[i];
					PersonSelector sel(fc, cfPerson);
					sel.putPeople();
				}
			}
		}

		// 현재 세력정보 읽기
		int readForces()
		{
			pk::list<pk::force@> list = pk::get_force_list();
			for (int i = 0; i < list.count; i++)
			{
				pk::force@ f = list[i];

				if (!cfUserReset and f.is_player()) // 플레이어 세력 제외
				{
					marrColor.insertLast(f.color); // 사용중인 세력색에 추가
					marrExceptCity.insertLast(getCity(f)); // 제외할 도시에 추가
					continue;
				}

				marrForce.insertLast(Force(f));
			}
			return marrForce.length();
		}

		// 플레이어 세력 유지 조건시 - 플레이어를 제외한 세력 수
		int getNonPlayerCount(pk::list<pk::force@> list)
		{
			int ret = 0;
			for (int i = 0; i < list.count; i++)
			{
				pk::force@ f = list[i];
				if (!f.is_player())
					ret++;
				else
				{
					marrColor.insertLast(f.color); // 사용중인 세력색에 추가
					marrExceptCity.insertLast(getCity(f)); // 제외할 도시에 추가
				}
			}

			return ret;
		}

		// 새 군주들 선발하기
		void selectLords(int cnt)
		{
			// 조건에 맞게

			// 는 개뿔 일단 무작위
			pk::random random(pk::rand());
			int n, d;
			Ps dps;
			for (int i = 0; i < cnt; i++)
			{
				n = random(0, 669);
				while (true)
				{
					if (marrLordNo.find(n) < 0 and canChange(n)) // 이미 선발되지도 않고 상태도 괜춘
						break;
					n = random(0, 669); // 괜찮아 질때까지 돌려
				}

				marrLordNo.insertLast(n); // 목록에 추가

				// 기본 세팅 적용
				d = findLordsDefault(n);
				if (d != -1)
					putDefLord(n, d);
				else
					marrLord.insertLast(Ps(n, putRandColor()));
			}
		}

		// 기본 세팅이 된 군주 후보군에 추가 - 지금은 세력색만 비교
		void putDefLord(int no, int def)
		{
			// 세팅이 겹치는눔 있나 확인
			Ps@ np = arrDef[def];

			int p1_def;
			for (int i = 0; i < int(marrLord.length()); i++)
			{
				Ps@ p1 = marrLord[i];
				if (p1.color == np.color)
				{
					// 색이 겹치면 우선순위 비교
					p1_def = findLordsDefault(p1.no); // 몇번이냐?
					if (p1_def >= 0 && p1_def < def)
					{
						// 내가 포기해야한다
						marrLord.insertLast(Ps(no, putRandColor(), np.title, np.kok));
					}
					else
					{
						// 기존놈꺼 뺏어야한다!
						marrLord[i].color = putRandColor();
						marrLord.insertLast(Ps(no, np.color, np.title, np.kok));
					}
					return; // 끄읏
				}

			}

			// 새로 생성할 세력중엔 겹치는게 없지만... 혹시 플레이어가 사용중인 세력색이라면?
			int color = marrColor.find(np.color) < 0 ? np.color : getRandColor(); // 그럼 별 수 엇이 포기다

			// 안겹치면 바로 넣기
			marrLord.insertLast(Ps(no, color, np.title, np.kok));
			marrColor.insertLast(color);
		}

		// 사용중이지 않은 랜덤색 리턴
		int getRandColor()
		{
			// 색 0~54
			int c = pk::rand(54 + 1);
			while (marrColor.find(c) >= 0)
				c = pk::rand(55);
			return c;
		}

		// 배열에 자동 입력하고 리턴
		int putRandColor()
		{
			int c = getRandColor();
			marrColor.insertLast(c);
			return c;
		}

		// 해당 군주의 기본 세팅이 있나 검사
		int findLordsDefault(int no)
		{
			for (int i = 0; i < int(arrDef.length()); i++)
			{
				if (arrDef[i].no == no)
					return i;
			}
			return -1;
		}

		// 변경 가능한 녀석이니?
		bool canChange(int no)
		{
			pk::person@ p = pk::get_person(no);
			if (!isOK(p))
				return false;
			if (cfUserReset) // 플레이어 세력도 작업하면 걍 다 허용
				return true;

			// 플레이어 세력 소속이 아닐것
			if ((p.mibun == 身份_君主 or p.mibun == 身份_一般) and marrExceptCity.find(p.location) > -1)
				return false;
			return true;
		}

		// 상태 괜춘?
		bool isOK(int no)
		{
			pk::person@ p = pk::get_person(no);
			return isOK(p);
		}
		bool isOK(pk::person@ p)
		{
			return (身份_君主 <= p.mibun and p.mibun <= 신분_재야) or p.mibun == 신분_미발견;
		}

		// 해당 세력의 도시 1개
		int getCity(pk::force@ force)
		{
			pk::list<pk::city@> clist = pk::get_city_list(force);
			return clist[0].get_id();
		}
	}

	Ori ori;
}
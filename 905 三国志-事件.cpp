// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ##2022/01/17 # 江东新风 # 根据梅神建议，稍微改进触发条件 ##
// ##2020/08/03 # messi # 修正对話 ##
// ##2020/07/31 # messi # 刪除单挑 ##
/*
@제작자: HoneyBee
@설명: 삼국지 이벤트를 추가

※ 이벤트 작성을 위한 별도 SCRIPT로 조금씩 UPDATE 할 예정입니다.

*/

namespace 삼국지_이벤트
{


	//===================== CUSTOMIZE ========================

	const bool 大兴山之战 = true; // (184년 2월 1일)
	const bool 大兴山之战_结果反映 = true; // 무장 사망, 공적 상승 반영 (false로 지정하면 반영되지 않음)

	const bool 南华老仙_来访 = true; // (184년 8월 1일)
	const bool 南华老仙_来访_结果反映 = true; // 태평요술서 빼앗김 반영 (false로 지정하면 반영되지 않음)

	const bool 汜水关之战 = true; // (190년 2월 1일)
	const bool 汜水关之战_结果反映 = true; // 무장 사망, 공적 상승 반영 (false로 지정하면 반영되지 않음)

	const bool 虎牢关之战 = true; // (190년 3월 1일)
	const bool 虎牢关之战_结果反映 = true; // 무장 사망, 공적 상승 반영 (false로 지정하면 반영되지 않음)

	const bool 左慈来访 = true; // (216년 10월 1일)

	const bool 高傲的美髯公 = true; // (217년 8월 1일) [미염공을 당하지 못하다.]
	const bool 高傲的美髯公_结果反映 = true; // 충성도 상승 반영 (false로 지정하면 반영되지 않음)


///下面内容只是为了防止kre报错，实际不起效，所以kre修改也不起效
	const bool 대흥산_전투 = true; // (184년 2월 1일)
	const bool 대흥산_전투_결과반영 = true; // 무장 사망, 공적 상승 반영 (false로 지정하면 반영되지 않음)

	const bool 남화노선_질책 = true; // (184년 8월 1일)
	const bool 남화노선_질책_결과반영 = true; // 태평요술서 빼앗김 반영 (false로 지정하면 반영되지 않음)

	const bool 사수관_전투 = true; // (190년 2월 1일)
	const bool 사수관_전투_결과반영 = true; // 무장 사망, 공적 상승 반영 (false로 지정하면 반영되지 않음)

	const bool 호뢰관_전투 = true; // (190년 3월 1일)
	const bool 호뢰관_전투_결과반영 = true; // 무장 사망, 공적 상승 반영 (false로 지정하면 반영되지 않음)

	const bool 좌자방문 = true; // (216년 10월 1일)

	const bool 미염공_마초 = true; // (217년 8월 1일) [미염공을 당하지 못하다.]
	const bool 미염공_마초_결과반영 = true; // 충성도 상승 반영 (false로 지정하면 반영되지 않음)

//========================================================


	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
		}

		void callback()
		{
			event_of_the_three_kingdoms();
		}

		pk::random random(pk::rand());

		// 아이템 변수
		pk::item@ item_태평요술서 = pk::get_item(보물_태평요술서);


		// 일기토 변수
		int 대흥산01_duel_result;
		int 대흥산02_duel_result;

		int 사수관01_duel_result;
		int 사수관02_duel_result;
		int 사수관03_duel_result;
		int 사수관04_duel_result;

		int 호뢰관01_duel_result;
		int 호뢰관02_duel_result;
		int 호뢰관03_duel_result;
		int 호뢰관04_duel_result;
		int 호뢰관05_duel_result;


		// 무장 변수
		pk::person@ 유비 = pk::get_person(무장_유비);
		pk::person@ 관우 = pk::get_person(무장_관우);
		pk::person@ 장비 = pk::get_person(무장_장비);
		pk::person@ 제갈량 = pk::get_person(무장_제갈량);
		pk::person@ 마초 = pk::get_person(무장_마초);

		pk::person@ 유언 = pk::get_person(무장_유언);
		pk::person@ 추정 = pk::get_person(무장_추정);

		pk::person@ 장각 = pk::get_person(무장_장각);
		pk::person@ 장보 = pk::get_person(무장_장보);
		pk::person@ 장량 = pk::get_person(무장_장량);

		pk::person@ 노옹 = pk::get_person(무장_노옹); // 남화노선
		pk::person@ 좌자 = pk::get_person(무장_좌자);

		pk::person@ 정원지 = pk::get_person(무장_정원지);
		pk::person@ 등무 = pk::get_person(무장_등무);

		pk::person@ 병사 = pk::get_person(무장_병사);

		pk::person@ 조조 = pk::get_person(무장_조조);

		pk::person@ 손견 = pk::get_person(무장_손견);
		pk::person@ 조무 = pk::get_person(무장_조무);

		pk::person@ 동탁 = pk::get_person(무장_동탁);
		pk::person@ 여포 = pk::get_person(무장_여포);
		pk::person@ 화웅 = pk::get_person(무장_화웅);

		pk::person@ 원소 = pk::get_person(무장_원소);
		pk::person@ 원술 = pk::get_person(무장_원술);
		pk::person@ 공손찬 = pk::get_person(무장_공손찬);
		pk::person@ 한복 = pk::get_person(무장_한복);
		pk::person@ 왕광 = pk::get_person(무장_왕광);

		pk::person@ 유섭 = pk::get_person(무장_유섭);
		pk::person@ 반봉 = pk::get_person(무장_반봉);

		pk::person@ 방열 = pk::get_person(무장_방열);
		pk::person@ 목순 = pk::get_person(무장_목순);
		pk::person@ 무안국 = pk::get_person(무장_무안국);


		void event_of_the_three_kingdoms()
		{
			// 대흥산 전투 (184년 2월 1일)大兴山之战
			if (check_大兴山之战()) pk::scene(pk::scene_t(scene_大兴山之战));

			// 南华老仙_来访 (184년 8월 1일)南华老仙
			if (check_南华老仙_来访()) pk::scene(pk::scene_t(scene_南华老仙_来访));

			// 사수관 전투 (190년 2월 1일)汜水关之战
			if (check_汜水关之战()) pk::scene(pk::scene_t(scene_汜水关之战));

			// 호뢰관 전투 (190년 3월 1일)虎牢关之战
			if (check_虎牢关之战()) pk::scene(pk::scene_t(scene_虎牢关之战));

			// 左慈来访 (216년 10월 1일) 左慈
			if (check_左慈来访()) pk::scene(pk::scene_t(scene_左慈来访));

			// 高傲的美髯公 (217년 8월 1일) 高傲的美?公
			if (check_高傲的美髯公()) pk::scene(pk::scene_t(scene_高傲的美髯公));

		}

		bool check_person_mibun(pk::person@person)
		{
			if (person.mibun == 身份_一般 or person.mibun == 身份_太守 or person.mibun == 身份_都督) return true;
			return false;
		}

		bool check_大兴山之战()
		{
			if (大兴山之战 and (pk::get_day() == 1) and (pk::get_month() == 2) and (pk::get_year() == 184))
			{
				//刘焉势力，刘焉为君主，刘备，关羽，张飞，邹靖在刘焉势力，且身份为一般或太守或都督（非君主，未死亡，俘虏，在野，未登场，未发现），程远志，邓茂为张角势力且身份为一般或太守或都督。
				pk::person@ 刘焉 = pk::get_person(武将_刘焉);
				int force_id0 = 刘焉.get_force_id();
				pk::person@ 刘备 = pk::get_person(武将_刘备);
				pk::person@ 关羽 = pk::get_person(武将_关羽);
				pk::person@ 张飞 = pk::get_person(武将_张飞);
				pk::person@ 邹靖 = pk::get_person(武将_邹靖);
				pk::person@ 程远志 = pk::get_person(武将_程远志);
				pk::person@ 邓茂 = pk::get_person(武将_邓茂);
				if (刘焉.mibun == 身份_君主 and check_person_mibun(刘备) and check_person_mibun(关羽) and check_person_mibun(张飞) and check_person_mibun(邹靖) and check_person_mibun(程远志) and check_person_mibun(邓茂))
				{
					if (刘备.get_force_id() == force_id0 and 关羽.get_force_id() == force_id0 and 张飞.get_force_id() == force_id0 and 邹靖.get_force_id() == force_id0)
					{
						int force_id1 = pk::get_person(武将_张角).get_force_id();
						if (程远志.get_force_id() == force_id1 and 邓茂.get_force_id() == force_id1)
						{
							return true;
						}
					}
				}
			}
			return false;
		}
		// -------------------------------------------------------------------------------------------------------------------------
		void scene_大兴山之战()
		{
			pk::fade(0);
			pk::sleep();

			pk::background(1);
			pk::fade(255);
			pk::message_box(pk::encode("玄德公,主公找你有急事,咱们一块过去吧."), 추정);
			pk::message_box(pk::encode("是,知道了."), 유비);

			pk::fade(0);
			pk::background(-1);

			pk::background(2);
			pk::fade(255);
			pk::message_box(pk::encode("太守大人,您找我吗?"), 유비);
			pk::message_box(pk::encode("哦!贤侄来了啊."), 유언);
			pk::message_box(pk::encode("其实我想拜托你一件事,所以叫你来了."), 유언);
			pk::message_box(pk::encode("我听说这件事很急我就赶过来了.."), 유비);
			pk::message_box(pk::encode("这附近的大兴山被盗贼占领了."), 유언);
			pk::message_box(pk::encode("希望你能跟邹靖将军一起出阵扫荡他们."), 유언);
			pk::message_box(pk::encode("啊,原来有这样的事啊.\n那就要抓紧时间了."), 유비);
			pk::message_box(pk::encode("云长,益德!赶紧准备出发."), 유비);
			pk::message_box(pk::encode("我明白了,大哥."), 관우);
			pk::message_box(pk::encode("哈哈哈!真是难得的第一次出阵啊!"), 장비);

			pk::fade(0);
			pk::background(-1);

			pk::background(10);
			pk::fade(255);
			pk::message_box(pk::encode("你们这帮土匪!胆敢在我大汉的疆土上放肆!"), 유비);
			pk::message_box(pk::encode("哼!乳臭未干的小毛孩真不知天高地厚.."), 정원지);
			pk::message_box(pk::encode("部将,你去把那家伙的脑袋拿过来!"), 정원지);
			pk::message_box(pk::encode("是,将军!"), 등무);
			pk::message_box(pk::encode("你这个大耳贼!看刀!"), 등무);
			pk::message_box(pk::encode("你这家伙!你小子接下我这丈八蛇矛吧!"), 장비);
			pk::message_box(pk::encode("呃啊...!"), 등무);
			pk::message_box(pk::encode("哼!连一回合都坚持不住..真无聊!"), 장비);
			pk::message_box(pk::encode("我来对付你这家伙!"), 정원지);
			pk::message_box(pk::encode("由我来取你项上人头."), 관우);
			pk::message_box(pk::encode("呃啊..我竟输给如此鼠辈.."), 정원지);
			pk::message_box(pk::encode("果然,\x1b[01x关羽\x1b[0x和\x1b[01x张飞\x1b[0x真厉害啊!"), 병사);
			pk::message_box(pk::encode("敌将死了!消灭剩下的黄巾贼!"), 유비);
			pk::message_box(pk::encode("哇啊啊啊!"), 병사);

			pk::fade(0);
			pk::background(-1);

			pk::background(22);
			pk::fade(255);
			pk::message_box(pk::encode("\x1b[01x刘备\x1b[0x在大兴山大获全胜."));

			if (大兴山之战_结果反映)
			{
				pk::message_box(pk::encode("\x1b[01x刘备\x1b[0x,\x1b[01x关羽\x1b[0x,\x1b[01x张飞\x1b[0x的功绩上升了."));
				pk::message_box(pk::encode("\x1b[01x程远志\x1b[0x和\x1b[01x邓茂\x1b[0x死亡."));


				유비.kouseki = 유비.kouseki + 500;
				관우.kouseki = 관우.kouseki + 500;
				장비.kouseki = 장비.kouseki + 500;

				pk::kill(등무, 장비, pk::get_hex_object(등무.location), null, 2);
				pk::kill(정원지, 관우, pk::get_hex_object(정원지.location), null, 2);

				유비.update();
				관우.update();
				장비.update();
			}

			pk::fade(0);
			pk::background(-1);

		}

		// -------------------------------------------------------------------------------------------------------------------------	

		bool check_南华老仙_来访()
		{
			if (南华老仙_来访 and (pk::get_day() == 1) and (pk::get_month() == 8) and (pk::get_year() == 184))
			{
				//张角势力存在，且张角张宝张梁在张角势力，并且张角为君主，张宝张梁为一般或太守或都督。太平要术为张角所属。
				pk::person@ 张角 = pk::get_person(武将_张角);
				int force_id0 = 张角.get_force_id();
				pk::person@ 张宝 = pk::get_person(武将_张宝);
				pk::person@ 张梁 = pk::get_person(武将_张梁);
				if (张角.mibun == 身份_君主 and pk::get_item(宝物_太平要术).owner == 武将_张角 and check_person_mibun(张宝) and check_person_mibun(张梁))
				{
					if (张宝.get_force_id() == force_id0 and 张梁.get_force_id() == force_id0)
					{
						return true;
					}
				}
			}
			return false;
		}

		void scene_南华老仙_来访()
		{
			pk::fade(0);
			pk::sleep();

			pk::background(5);
			pk::fade(255);
			pk::message_box(pk::encode("有一天在黄巾军首领\x1b[01x张角\x1b[0x的宅邸里.."));
			pk::message_box(pk::encode("大哥,如今也要跟起兵时一样,不能小看敌人掉以轻心.."), 장보);
			pk::message_box(pk::encode("一个王朝就像巨象一样.就是再怎么老也不是那么容易垮掉的."), 장각);
			pk::message_box(pk::encode("确实.....\n等等!是谁?"), 장량);
			pk::message_box(pk::encode("你这老家伙!\n像小偷一样悄无声息地走进大贤良师的房间中.\n是在做什么!"), 장량);
			pk::message_box(pk::encode("真是无礼!\n马上给我退下!"), 장보);
			pk::message_box(pk::encode("等等!你是....."), 장각);
			pk::message_box(pk::encode("....老朽就是\x1b[01x南华老仙\x1b[0x."), 노옹);
			pk::message_box(pk::encode("\x1b[01x张角\x1b[0x啊,好久不见啦...."), 노옹);
			pk::message_box(pk::encode("你为什么要造反,夺取天下呢?"), 노옹);
			pk::message_box(pk::encode("啊,不是这样的!\n我举兵是为了拯救全天下的百姓于水火之中啊."), 장각);
			pk::message_box(pk::encode("竟然说我是为了霸占天下,这是纵曲枉直啊."), 장각);
			pk::message_box(pk::encode("....我不想听什么辩解."), 노옹);
			pk::message_box(pk::encode("老朽不见黄巾之徒,而见万民侵苦...."), 노옹);
			pk::message_box(pk::encode("你被私欲蒙蔽,以赎罪的代价,我要将那本奇书带走了."), 노옹);
			pk::message_box(pk::encode("好好反省吧.\n知道了吗？..."), 노옹);
			pk::message_box(pk::encode("南华老仙!\n消失了."), 장각);
			pk::message_box(pk::encode("啊!太,太平要术彻底消失了."), 장각);
			pk::message_box(pk::encode("你说什么!\n不会是刚才那个老者干的吧..."), 장보);
			pk::message_box(pk::encode("嗯..真是怪事.."), 장량);
			pk::message_box(pk::encode("\x1b[01x张角\x1b[0x的\x1b[01x太平要术\x1b[0x丢失了."));

			pk::fade(0);
			pk::background(-1);

			if (南华老仙_来访_结果反映)
			{
				pk::message_box(pk::encode("\x1b[01x张角\x1b[0x所拥有的\x1b[01x太平要术\x1b[0x消失了."));

				item_태평요술서.location = random(도시_끝 - 1);
				item_태평요술서.owner = -1;
				item_태평요술서.update();
			}

			pk::fade(0);
			pk::background(-1);

		}

		// -------------------------------------------------------------------------------------------------------------------------		

		bool check_scenario()
		{
			if (pk::get_scenario().no == 1)
			{
				string scenario_name = pk::decode(pk::get_scenario().name);
				if (scenario_name == "反董卓联盟")
					return true;
			}
			return false;
		}

		bool check_汜水关之战()
		{
			if (汜水关之战 and (pk::get_day() == 1) and (pk::get_month() == 2) and (pk::get_year() == 190))
			{
				if (check_scenario())
				{
					/*
					汜水关之战：袁绍，袁术，孙坚，韩馥，孔融，刘岱，孔伷，张扬，马腾，陶谦，曹操，公孙瓒势力存在且互为盟友。此外与董卓未结盟。
孙坚势力，孙坚为君主，祖茂为孙坚所属，且身份为一般或太守或都督。
袁绍势力存在，袁绍为君主，公孙瓒势力存在，公孙瓒为君主。
刘备势力存在，刘备为君主，张飞关羽属于刘备势力，且身份为为一般或太守或都督。
孙坚势力存在，孙坚为君主，祖茂在孙坚势力且身份为一般或太守或都督。
袁术势力存在，袁术为君主，俞涉在袁术势力且身份为一般或太守或都督。
韩馥势力存在，韩馥为君主，潘凤在韩馥势力且身份为一般或太守或都督。
董卓势力存在，董卓为君主，华雄在董卓势力且身份为一般或太守或都督。

					*/
					pk::person@ 董卓 = pk::get_person(武将_董卓);
					int force_id0 = 董卓.get_force_id();
					pk::person@ 华雄 = pk::get_person(武将_华雄);
					pk::person@ 张飞 = pk::get_person(武将_张飞);
					pk::person@ 关羽 = pk::get_person(武将_关羽);
					pk::person@ 祖茂 = pk::get_person(武将_祖茂);
					pk::person@ 俞涉 = pk::get_person(武将_俞涉);
					pk::person@ 潘凤 = pk::get_person(武将_潘凤);

					if (董卓.mibun == 身份_君主 and 华雄.get_force_id() == force_id0)
					{
						if (check_person_mibun(张飞) and check_person_mibun(关羽) and check_person_mibun(华雄) and check_person_mibun(祖茂) and check_person_mibun(俞涉) and check_person_mibun(潘凤))
							return true;
					}
				}
			}
			return false;
		}

		void scene_汜水关之战()
		{
			pk::fade(0);
			pk::sleep();

			pk::background(11);
			pk::fade(255);
			pk::message_box(pk::encode("在反董卓联军中作为先锋的孙坚军因为军粮不足而士气低落,遭到了董卓军的夜袭."));
			pk::message_box(pk::encode("并且\x1b[01x孙坚\x1b[0x及其部下\x1b[01x祖茂\x1b[0x正处于其命运的岔路口.."));

			pk::fade(0);
			pk::background(-1);

			pk::background(3);
			pk::fade(255);
			pk::message_box(pk::encode("主公,让我戴上您的头巾.由我来代替主公引诱敌军!"), 조무);
			pk::message_box(pk::encode("啊.. \x1b[01x祖茂\x1b[0x..你这样做.."), 손견);
			pk::message_box(pk::encode("主公一定要完成未尽的大业啊."), 조무);
			pk::message_box(pk::encode("..........."), 손견);
			pk::message_box(pk::encode("你们这些家伙!我就是大名鼎鼎的\x1b[02x江东猛虎 \x1b[01x孙坚!"), 조무);
			pk::message_box(pk::encode("那家伙就是英勇善战的孙坚吗?!"), 병사);
			pk::message_box(pk::encode("尝尝我双刀的厉害吧!"), 조무);
			pk::message_box(pk::encode("哇哇啊..!"), 병사);
			pk::message_box(pk::encode("你这家伙!让我来对付你!"), 화웅);
			pk::message_box(pk::encode("咳咳...不能再和主公征战沙场了"), 조무);
			pk::message_box(pk::encode("哼,就凭这点实力也敢称\x1b[02x江东猛虎\x1b[0x吗?"), 화웅);
			pk::message_box(pk::encode("嗯..?等一下,这家伙不是\x1b[01x孙坚\x1b[0x.."), 화웅);
			pk::message_box(pk::encode("切,太可惜了..全军撤退吧!"), 화웅);
			pk::message_box(pk::encode("是,将军!"), 병사);

			pk::fade(0);
			pk::background(-1);

			pk::background(11);
			pk::fade(255);
			pk::message_box(pk::encode("另一方面,反董卓联军的大帐内.."));
			pk::message_box(pk::encode("盟主,听说孙坚的先锋部队在董卓军的夜袭下溃灭了."), 병사);
			pk::message_box(pk::encode("什么?!那勇猛的孙坚军也败北了吗..."), 원소);
			pk::message_box(pk::encode("......."), 유비);
			pk::message_box(pk::encode("嗯..对了,公孙太守后面的人是谁?"), 원소);
			pk::message_box(pk::encode("这位是小时候和我同门修学的\x1b[01x刘玄德\x1b[0x."), 공손찬);
			pk::message_box(pk::encode("\x1b[01x公孙瓒\x1b[0x向\x1b[01x袁绍\x1b[0x详细地介绍了关于\x1b[01x刘备\x1b[0x的功劳和来历."));
			pk::message_box(pk::encode("哦..果然不是凡夫俗子,原来是汉室宗亲."), 원소);
			pk::message_box(pk::encode("取座来,请坐."), 원소);
			pk::message_box(pk::encode("多谢盟主."), 유비);
			pk::message_box(pk::encode("急报!\x1b[01x华雄\x1b[0x来到寨前大骂搦战!"), 병사);
			pk::message_box(pk::encode("\x1b[01x华雄\x1b[0x这小子竟然敢来这里.."), 원소);
			pk::message_box(pk::encode("谁敢去取下\x1b[01x华雄\x1b[0x的项上人头?"), 원소);
			pk::message_box(pk::encode("末将愿往!"), 유섭);
			pk::message_box(pk::encode("祝将军武运昌盛,旗开得胜."), 원소);

			pk::fade(0);
			pk::background(-1);

			pk::background(10);
			pk::fade(255);
			pk::message_box(pk::encode("\x1b[01x华雄\x1b[0x何在!"), 유섭);
			pk::message_box(pk::encode("快出来跟我较量较量!"), 유섭);
			pk::message_box(pk::encode("哼!不知天高地厚!"), 화웅);
			pk::message_box(pk::encode("啊!"), 유섭);
			pk::message_box(pk::encode("连三回合都坚持不住,真是没用的家伙.."), 화웅);

			pk::fade(0);
			pk::background(-1);

			pk::background(11);
			pk::fade(255);
			pk::message_box(pk::encode("报!\x1b[01x俞涉\x1b[0x大人3回合就被\x1b[01x华雄\x1b[0x斩于马下."), 병사);
			pk::message_box(pk::encode("可恶..这次换谁去?"), 원소);
			pk::message_box(pk::encode("我有上将\x1b[01x潘凤\x1b[0x可斩\x1b[01x华雄\x1b[0x."), 한복);
			pk::message_box(pk::encode("可派他前去作为\x1b[01x华雄\x1b[0x的对手."), 한복);
			pk::message_box(pk::encode("我的大斧定会将\x1b[01x华雄\x1b[0x劈成两半!"), 반봉);
			pk::message_box(pk::encode("果然威风凛凛!"), 원소);

			pk::fade(0);
			pk::background(-1);

			pk::background(10);
			pk::fade(255);
			pk::message_box(pk::encode("\x1b[01x华雄\x1b[0x!尝尝我开山大斧的滋味!"), 반봉);
			pk::message_box(pk::encode("看起来还算那么回事..让我看看真正的实力吧?"), 화웅);
			pk::message_box(pk::encode("可恶!"), 반봉);
			pk::message_box(pk::encode("真是不堪一击啊.."), 화웅);

			pk::fade(0);
			pk::background(-1);

			pk::background(11);
			pk::fade(255);
			pk::message_box(pk::encode("\x1b[01x潘凤\x1b[0x又被\x1b[01x华雄\x1b[0x斩了."), 병사);
			pk::message_box(pk::encode("哎..可惜我之上将\x1b[01x颜良\x1b[0x,\x1b[01x文丑\x1b[0x未至.不然小小\x1b[01x华雄\x1b[0x何惧..."), 원소);
			pk::message_box(pk::encode("小将愿砍下\x1b[01x华雄\x1b[0x首级献于账下.."), 관우);
			pk::message_box(pk::encode("汝乃何人?"), 원소);
			pk::message_box(pk::encode("我是\x1b[01x刘玄德\x1b[0x之弟,弓马手\x1b[01x关羽\x1b[0x."), 관우);
			pk::message_box(pk::encode("汝欺吾众诸侯无大将耶?量一弓手,安敢乱言...!"), 원술);
			pk::message_box(pk::encode("我看这个人仪表不凡,倒不像个弓马手.\n如其不胜,责之不迟."), 조조);
			pk::message_box(pk::encode("如不胜,请斩某头."), 관우);
			pk::message_box(pk::encode("来,喝了这温酒,出去战个痛快."), 조조);
			pk::message_box(pk::encode("酒且斟下,某去便来."), 관우);
			pk::message_box(pk::encode("..........."), 조조);

			pk::fade(0);
			pk::background(-1);

			pk::background(10);
			pk::fade(255);
			pk::message_box(pk::encode("呵,哪里来的无名鼠辈!"), 화웅);
			pk::message_box(pk::encode("休要猖狂!汝可敢接下我的偃月刀!"), 관우);
			pk::message_box(pk::encode("咳呃...."), 화웅);
			pk::message_box(pk::encode("\x1b[01x华雄\x1b[0x的首级我便收下了!"), 관우);

			pk::fade(0);
			pk::background(-1);

			pk::background(11);
			pk::fade(255);
			pk::message_box(pk::encode("这便是\x1b[01x华雄\x1b[0x的首级!"), 관우);
			pk::message_box(pk::encode("(果然!此人不凡,吾甚爱之.)"), 조조);

			pk::fade(0);
			pk::background(-1);

			pk::background(22);
			pk::fade(255);
			pk::message_box(pk::encode("反董卓联军因\x1b[01x关羽\x1b[0x的勇武而斩下了\x1b[01x华雄\x1b[0x的头颅."));

			if (汜水关之战_结果反映)
			{
				pk::message_box(pk::encode("\x1b[01x关羽\x1b[0x的功绩上升了."));
				pk::message_box(pk::encode("\x1b[01x祖茂\x1b[0x,\x1b[01x俞涉\x1b[0x,\x1b[01x潘凤\x1b[0x,\x1b[01x华雄\x1b[0x死亡了."));

				관우.kouseki = 관우.kouseki + 1500;

				pk::kill(조무, 화웅, pk::get_hex_object(조무.location), null, 2);
				pk::kill(유섭, 화웅, pk::get_hex_object(유섭.location), null, 2);
				pk::kill(반봉, 화웅, pk::get_hex_object(반봉.location), null, 2);
				pk::kill(화웅, 관우, pk::get_hex_object(화웅.location), null, 2);

				관우.update();
			}

			pk::fade(0);
			pk::background(-1);
		}

		// -------------------------------------------------------------------------------------------------------------------------		

		bool check_虎牢关之战()
		{
			if (虎牢关之战 and (pk::get_day() == 1) and (pk::get_month() == 3) and (pk::get_year() == 190))
			{
				/*
				袁绍，袁术，孙坚，韩馥，孔融，刘岱，孔伷，张扬，马腾，陶谦，曹操，公孙瓒势力存在且互为盟友。此外与董卓未结盟。
董卓势力存在，董卓为君主，吕布在董卓势力且身份为一般或太守或都督。吕布在虎牢关。
袁绍势力存在，袁绍为君主。
张扬势力存在，张扬为君主，王匡，方悦，穆顺在张扬势力且身份为一般或太守或都督。
孔融势力存在，孔融为君主，武安国在孔融势力且身份为一般或太守或都督。
公孙瓒势力存在，公孙瓒为君主。
刘备势力存在，刘备为君主，关羽，张飞在刘备势力且身份为一般或太守或都督。
曹操势力存在，曹操为君主。

				*/
				if (check_scenario())
				{
					pk::person@ 董卓 = pk::get_person(武将_董卓);
					int force_id0 = 董卓.get_force_id();
					pk::person@ 吕布 = pk::get_person(武将_吕布);
					pk::person@ 王匡 = pk::get_person(武将_王匡);
					pk::person@ 方悦 = pk::get_person(武将_方悦);
					pk::person@ 穆顺 = pk::get_person(武将_穆顺);
					pk::person@ 武安国 = pk::get_person(武将_武安国);
					if (董卓.mibun == 身份_君主 and 吕布.get_force_id() == force_id0 and 吕布.service == 新据点_虎牢关)
					{
						if (check_person_mibun(吕布) and check_person_mibun(王匡) and check_person_mibun(方悦) and check_person_mibun(穆顺) and check_person_mibun(武安国))
							return true;
					}
					
				}
			}
			return false;
		}

		void scene_虎牢关之战()
		{

			pk::fade(0);
			pk::sleep();

			pk::background(36);
			pk::fade(255);
			pk::message_box(pk::encode("抵达虎牢关的反董卓联军,面对的将是当今最强的武将\x1b[01x吕布\x1b[0x."));
			pk::message_box(pk::encode("这群乌合之众竟敢来到这里,看大爷我杀他个人仰马翻!"), 여포);
			pk::message_box(pk::encode("这就是天下无双的\x1b[01x吕布\x1b[0x吗?"), 왕광);
			pk::message_box(pk::encode("谁敢出战,与那\x1b[01x吕布\x1b[0x较量一番?"), 왕광);
			pk::message_box(pk::encode("交给在下!"), 방열);
			pk::message_box(pk::encode("\x1b[01x吕布\x1b[0x匹夫,和我一决高下吧!"), 방열);
			pk::message_box(pk::encode("哼!真可笑!"), 여포);
			pk::message_box(pk::encode("好强啊!"), 방열);
			pk::message_box(pk::encode("竟然连5回合都坚持不下,杂鱼还敢叫阵."), 여포);
			pk::message_box(pk::encode("哎..那家伙是怪物吗?"), 왕광);
			pk::message_box(pk::encode("这次由我来对付你!"), 목순);
			pk::message_box(pk::encode("自寻死路."), 여포);
			pk::message_box(pk::encode("呜呃!"), 목순);
			pk::message_box(pk::encode("哈哈!联军那些家伙都那么弱吗?一个能打的都没有!"), 여포);
			pk::message_box(pk::encode("看我的厉害!看招!"), 무안국);
			pk::message_box(pk::encode("又来了一个送死的,真是麻烦!"), 여포);
			pk::message_box(pk::encode("啊啊啊!我的胳膊...!"), 무안국);
			pk::message_box(pk::encode("能撑得过我10回合还活着的家伙,运气不错啊!"), 여포);
			pk::message_box(pk::encode("不行了..我得逃跑.."), 무안국);
			pk::message_box(pk::encode("你这家伙!往哪儿跑!"), 여포);
			pk::message_box(pk::encode("\x1b[01x吕布\x1b[0x,可曾听过我的威名!"), 공손찬);
			pk::message_box(pk::encode("我就是被称为\x1b[02x白马将军\x1b[0x的\x1b[01x公孙瓒\x1b[0x."), 공손찬);
			pk::message_box(pk::encode("好哇!终于来了一位大人物!"), 여포);
			pk::message_box(pk::encode("呜..果然,天下无双并不是虚名."), 공손찬);
			pk::message_box(pk::encode("再打下去就危险了,先撤退吧.."), 공손찬);
			pk::message_box(pk::encode("这家伙!只比了几个回合就跑掉了,看我的!"), 여포);
			pk::message_box(pk::encode("可恶..这样下去会被抓到的.."), 공손찬);
			pk::message_box(pk::encode("交给我来!三姓家奴给俺听仔细了!俺乃\x1b[02x燕人\x1b[01x张翼德\x1b[0x是也!"), 장비);
			pk::message_box(pk::encode("不知死活的家伙!看戟!"), 여포);
			pk::message_box(pk::encode("不妙!对付这个环眼虎须大汉尽全力才能赢..又闯来了两个家伙..再打下去就吃亏了..撤退吧!"), 여포);
			pk::message_box(pk::encode("喝!\x1b[01x吕布\x1b[0x小儿,休要逃走!"), 장비);
			pk::message_box(pk::encode("哎!那\x1b[01x吕布\x1b[0x所骑乃名马\x1b[01x赤兔\x1b[0x."), 관우);
			pk::message_box(pk::encode("大哥,现在如要追上那\x1b[01x吕布\x1b[0x已是不可能了."), 관우);
			pk::message_box(pk::encode("虽然很可惜,还是按你所说吧.还是等日后再生擒\x1b[01x吕布\x1b[0x吧."), 유비);
			pk::message_box(pk::encode("(呼..\x1b[01x刘备\x1b[0x和他的兄弟真乃当时英杰.)"), 조조);
			pk::message_box(pk::encode("(特别是那个\x1b[01x关羽\x1b[0x,真想让此人为我效力啊..)"), 조조);
			pk::message_box(pk::encode("(之后再找机会吧..)"), 조조);

			pk::fade(0);
			pk::background(-1);

			pk::background(22);
			pk::fade(255);
			pk::message_box(pk::encode("以反董卓联军为对手,\x1b[01x吕布\x1b[0x显示了他天下无双的勇武."));
			pk::message_box(pk::encode("\x1b[01x刘关张\x1b[0x因此役活跃的表现,将绝世无双的\x1b[01x吕布\x1b[0x驱逐.联军由此士气大振,而三人也开始渐渐地崭露头角..."));

			if (虎牢关之战_结果反映)
			{
				pk::message_box(pk::encode("\x1b[01x刘关张\x1b[0x三兄弟的功绩上升了."));
				pk::message_box(pk::encode("\x1b[01x方悦\x1b[0x,\x1b[01x穆顺\x1b[0x死亡."));
				pk::message_box(pk::encode("\x1b[01x武安国\x1b[0x失去了一只手臂,武力-10."));

				유비.kouseki = 유비.kouseki + 1500;
				관우.kouseki = 관우.kouseki + 1500;
				장비.kouseki = 장비.kouseki + 1500;

				pk::kill(방열, 여포, pk::get_hex_object(방열.location), null, 2);
				pk::kill(목순, 여포, pk::get_hex_object(목순.location), null, 2);

				무안국.base_stat[武将能力_武力] = 무안국.base_stat[武将能力_武力] - 10;
				무안국.shoubyou = 상병_중증;

				유비.update();
				관우.update();
				장비.update();
				무안국.update();
			}

			pk::fade(0);
			pk::background(-1);

		}

		// -------------------------------------------------------------------------------------------------------------------------

		bool check_左慈来访()
		{
			if (左慈来访 and (pk::get_day() == 1) and (pk::get_month() == 8) and (pk::get_year() == 216))
			{
				pk::person@ 曹操 = pk::get_person(武将_曹操);
				pk::person@ 刘备 = pk::get_person(武将_刘备);
				if (刘备.mibun == 身份_君主 and 曹操.mibun == 身份_君主)
				{
					return true;
				}
			}
			return false;
		}

		void scene_左慈来访()
		{

			pk::fade(0);
			pk::sleep();

			pk::background(2);
			pk::fade(255);
			pk::message_box(pk::encode("\x1b[01x曹操\x1b[0x所在的城池内"));
			pk::message_box(pk::encode("禀报主公.\n一位名为\x1b[01x左慈\x1b[0x的道士求见.."), 병사);
			pk::message_box(pk::encode("道士?\n让我见见吧."), 조조);
			pk::message_box(pk::encode("初次见面.\n在下\x1b[01x左慈\x1b[0x."), 좌자);
			pk::message_box(pk::encode("阁下为何前来见我?"), 조조);
			pk::message_box(pk::encode("当今中原已定,阁下也已进爵为王..."), 좌자);
			pk::message_box(pk::encode("俗世已无依恋"), 좌자);
			pk::message_box(pk::encode("何不退步,跟贫道往峨眉山中修行,遁入仙道?"), 좌자);
			pk::message_box(pk::encode("所以说要孤一起进山仙人修行么."), 조조);
			pk::message_box(pk::encode("嗯,真是有趣的想法."), 조조);
			pk::message_box(pk::encode("但是..\n朝廷诸事不知该托付何人啊."), 조조);
			pk::message_box(pk::encode("\x1b[01x刘备\x1b[0x是忠义仁德之人,并且为帝室之胄."), 좌자);
			pk::message_box(pk::encode("何不让位给他,到时必天下归心."), 좌자);
			pk::message_box(pk::encode("此人是\x1b[01x刘备\x1b[0x派来的奸细!"), 조조);
			pk::message_box(pk::encode("来人!将此人拿下!"), 조조);
			pk::message_box(pk::encode("呵呵,在下掌握了仙术."), 좌자);
			pk::message_box(pk::encode("你认为能抓的到我吗?"), 좌자);
			pk::message_box(pk::encode("消失了!?"), 조조);
			pk::message_box(pk::encode("....竟发生了这种怪事!!"), 조조);

			pk::fade(0);
			pk::background(-1);

		}

		// -------------------------------------------------------------------------------------------------------------------------

		bool check_高傲的美髯公()
		{
			if (高傲的美髯公 and (pk::get_day() == 1) and (pk::get_month() == 8) and (pk::get_year() == 217))
			{
				//张角势力存在，且张角张宝张梁在张角势力，并且张角为君主，张宝张梁为一般或太守或都督。太平要术为张角所属。
				pk::person@ 刘备 = pk::get_person(武将_刘备);
				int force_id0 = 刘备.get_force_id();
				pk::person@ 诸葛亮 = pk::get_person(武将_诸葛亮);
				pk::person@ 关羽 = pk::get_person(武将_关羽);
				pk::person@ 马超 = pk::get_person(武将_马超);

				if (刘备.mibun == 身份_君主 and check_person_mibun(诸葛亮) and check_person_mibun(关羽) and check_person_mibun(马超))
				{
					if (关羽.location != 马超.location and 关羽.location!= 刘备.location)
					{
						return true;
					}
				}
			}
			return false;
		}

		void scene_高傲的美髯公() // 미염공을 당하지 못하다. 
		{
			pk::fade(0);
			pk::sleep();

			pk::background(5);
			pk::fade(255);
			pk::message_box(pk::encode("\x1b[01x关羽\x1b[0x所在的江陵城.."));
			pk::message_box(pk::encode("近日听闻在潼关杀得曹操割须弃袍的马孟起投靠了大哥..."), 관우);
			pk::message_box(pk::encode("是的,听说是一位和\x1b[01x张飞\x1b[0x大人比试不分胜负的优秀将领."), 병사);
			pk::message_box(pk::encode("他对自己的武艺非常自信."), 병사);
			pk::message_box(pk::encode("嗯...\n好的,我知道了."), 관우);

			pk::fade(0);
			pk::background(-1);

			pk::background(2);
			pk::fade(255);
			pk::message_box(pk::encode("\x1b[01x刘备\x1b[0x所在的城池.."));
			pk::message_box(pk::encode("主公,江陵的\x1b[01x关羽\x1b[0x将军送来书信."), 제갈량);
			pk::message_box(pk::encode("\x1b[01x云长\x1b[0x那里送来的书信..."), 유비);
			pk::message_box(pk::encode("难道荆州出了什么事情吗?"), 유비);
			pk::message_box(pk::encode("来,\x1b[01x孔明\x1b[0x,一并阅之."), 유비);
			pk::message_box(pk::encode("==近日新加入的\x1b[01x马孟起\x1b[0x武艺高超被谈及的传闻我也听说了.=="), 제갈량);
			pk::message_box(pk::encode("==为了和\x1b[01x马超\x1b[0x一较高下才无礼的写下这封信.=="), 제갈량);
			pk::message_box(pk::encode("==拜托了. 关云长=="), 제갈량);
			pk::message_box(pk::encode("如果\x1b[01x云长\x1b[0x与\x1b[01x孟起\x1b[0x决斗,两虎相争必有一伤."), 유비);
			pk::message_box(pk::encode("\x1b[01x云长\x1b[0x与\x1b[01x孟起\x1b[0x.对我来说都是不可失去的爱将啊."), 유비);
			pk::message_box(pk::encode("\x1b[01x孔明\x1b[0x,该如何是好."), 유비);
			pk::message_box(pk::encode("的确..\n主公勿忧,亮自作书回之."), 제갈량);

			pk::fade(0);
			pk::background(-1);

			pk::background(2);
			pk::fade(255);
			pk::message_box(pk::encode("这是\x1b[01x孔明\x1b[0x军师的书信,容某观之."), 관우);
			pk::message_box(pk::encode("==亮闻将军欲与孟起分别高下.=="), 관우);
			pk::message_box(pk::encode("==以亮度之：孟起虽雄烈过人,亦乃黥布,彭越之徒耳；当与翼德并驱争先,犹未及美髯公之绝伦超群也.=="), 관우);
			pk::message_box(pk::encode("==今公受任守荆州,不为不重；倘一入川,若荆州有失,罪莫大焉.惟冀明照=="), 관우);
			pk::message_box(pk::encode("果然\x1b[01x孔明\x1b[0x熟知我心也!"), 관우);
			pk::message_box(pk::encode("我的心结解开了!"), 관우);

			pk::fade(0);
			pk::background(-1);

			pk::background(5);
			pk::fade(255);
			pk::message_box(pk::encode("知道了\x1b[01x诸葛亮\x1b[0x回信一事,\x1b[01x马超\x1b[0x也吃了一惊."));
			pk::message_box(pk::encode("嗯...不管是\x1b[01x张飞\x1b[0x大人的武艺,还有在这之上的\x1b[01x关羽\x1b[0x大人..."), 마초);
			pk::message_box(pk::encode("聚集了如此多的人才,我切勿得意忘形."), 마초);

			pk::fade(0);
			pk::background(-1);

			if (高傲的美髯公_结果反映)
			{
				pk::message_box(pk::encode("\x1b[01x马超\x1b[0x的忠诚上升了."));

				마초.loyalty = 마초.loyalty + 10;
				마초.update();
			}

			pk::fade(0);
			pk::background(-1);
		}


	} // class Main

	Main main;

} // namespace
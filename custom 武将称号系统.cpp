<<<<<<< HEAD
﻿//## 2022/01/06 # 江东新风 # 头像范围重新设定 ##
=======
﻿//## 2022/02/15 # 江东新风 # 称号错误修复 ##
//## 2022/01/06 # 江东新风 # 头像范围重新设定 ##
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
// ## 2021/03/06 # 江东新风 # 修复xml复制黏贴导致不识别错误 ##
// ## 2021/01/14 # 江东新风 # 关掉trace ##
// ## 2020/12/25 # 江东新风 # 去掉测试内容 ##
// ## 2020/12/24 # 江东新风 # 修复trace参数报错 ##
// ## 2020/08/01 ##
/*
@제작자: HoneyBee
@설명: 특정 조건을 만족하면 무장들이 별명을 얻을 수 있는 SCRIPT


*/

namespace 별명
{

	// ================ CUSTOMIZE ================

	const int 공적비교_0 = 3000; 		  // 공적이 3000 이상 12000 미만인 경우 별명 획득 (특정 무장만)
	const int 공적비교_1 = 12000; 		  // 공적이 12000 이상인 경우 별명 획득

// ===========================================


	class Main
	{
		Main()
		{
			pk::bind(107, pk::trigger107_t(callback));
			pk::bind(106, pk::trigger106_t(callback2));
		}

		void callback()
		{
			reward_nickname_set();
		}

		void callback2(int file_id)
		{
			reward_nickname_set();
		}

		void reward_nickname_set()
		{
			pk::person@ 甘宁 = pk::get_person(118);
			if (甘宁.kouseki < 공적비교_0)
			{
				甘宁特技还原();
			}
			else if ((甘宁.kouseki >= 공적비교_0) and (甘宁.kouseki < 30000))
			{
				甘宁.name_read = pk::encode("锦帆贼");
				//甘宁.azana_read = "";
				甘宁特技还原();
			}
			else if (甘宁.kouseki >= 30000)
			{
				甘宁.name_read = pk::encode("江东新风");  //강좌호신
				//甘宁.azana_read = "";
				甘宁特技升级();
			}

			pk::person@ 陆逊 = pk::get_person(602);
			if (陆逊.kouseki < 공적비교_0)
			{
				陆逊特技还原();
			}
			else if ((陆逊.kouseki >= 공적비교_0) and (陆逊.kouseki < 50000))
			{
				陆逊.name_read = pk::encode("白面书生");
				//陆逊.azana_read = "";
				陆逊特技还原();
			}
			else if (陆逊.kouseki >= 50000)
			{
				陆逊.name_read = pk::encode("大都督");  //강좌호신
				//陆逊.azana_read = "";
				陆逊特技升级();
			}
			//pk::trace("开始测试特技xml读取");
			//特技xml读取测试();
			//pk::trace("完成特技xml读取");			


			auto building_list = pk::list_to_array(pk::get_building_list());

			for (int i = 0; i < int(building_list.length); i++)
			{
				pk::building@ building = building_list[i];

				pk::person@ taishu = pk::get_person(pk::get_taishu_id(building));

				auto mibun_list = pk::list_to_array(pk::get_person_list(building, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般)));

				if (pk::is_alive(taishu))
				{
					if (0 < mibun_list.length)
					{

						for (int j = 0; j < int(mibun_list.length); j++)
						{

							pk::person@ person_0 = mibun_list[j];

							// 무장이 출진 or 부재 상태이면 제외
							if (pk::is_unitize(person_0) or pk::is_absent(person_0)) continue;


							// 魏
							if (person_0.get_id() == 무장_조조)
							{
								if ((person_0.kouseki >= 공적비교_0) and (person_0.kouseki < 공적비교_1))
								{
									person_0.name_read = pk::encode("阿瞒");
								}
								else if (person_0.kouseki >= 공적비교_1)
								{
									person_0.name_read = pk::encode("乱世之奸雄");
								}
							}

							if (person_0.get_id() == 무장_조인)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("天人之勇");
							}

							if (person_0.get_id() == 무장_조홍)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("黄金头奎");
							}

							if (person_0.get_id() == 무장_하후돈)
							{
								if ((person_0.face == 1107)
									or (person_0.face == 1207)
									)
									person_0.name_read = pk::encode("盲夏侯");
							}

							if (person_0.get_id() == 무장_하후연)
							{
								if ((person_0.kouseki >= 공적비교_0) and (person_0.kouseki < 공적비교_1))
								{
									person_0.name_read = pk::encode("白地将军");
								}
								else if (person_0.kouseki >= 공적비교_1)
								{
									person_0.name_read = pk::encode("贵速将军");
								}
							}

							if (person_0.get_id() == 무장_장료)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("辽来来");
							}

							if (person_0.get_id() == 무장_허저)
							{
								if ((person_0.kouseki >= 공적비교_0) and (person_0.kouseki < 공적비교_1))
								{
									person_0.name_read = pk::encode("虎痴");
								}
								else if (person_0.kouseki >= 공적비교_1)
								{
									person_0.name_read = pk::encode("虎侯");
								}
							}

							if (person_0.get_id() == 무장_전위)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("恶来");
							}

							if (person_0.get_id() == 무장_순욱)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("王佐之才");
							}

							if (person_0.get_id() == 무장_조창)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("黄须儿");
							}

							if (person_0.get_id() == 무장_조식)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("七步之才");
							}

							if (person_0.get_id() == 무장_조휴)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("千里驹");
							}

							if (person_0.get_id() == 무장_서황)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("不败将军");
							}

							if (person_0.get_id() == 무장_방덕)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("白马将军");
							}

							if (person_0.get_id() == 무장_누규)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("梦梅居士");
							}


							// 蜀汉
							if (person_0.get_id() == 무장_유비)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("刘皇叔");
							}

							if (person_0.get_id() == 무장_관우)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("美髯公");
							}

							if (person_0.get_id() == 무장_장비)
							{
								if ((person_0.kouseki >= 공적비교_0) and (person_0.kouseki < 공적비교_1))
								{
									person_0.name_read = pk::encode("燕人张翼德");
								}
								else if (person_0.kouseki >= 공적비교_1)
								{
									person_0.name_read = pk::encode("环眼贼");
								}
							}

							if (person_0.get_id() == 무장_조운)
							{
								if ((person_0.kouseki >= 공적비교_0) and (person_0.kouseki < 공적비교_1))
								{
									person_0.name_read = pk::encode("常山赵子龙");  //쇄묘场뮴计
								}
								else if (person_0.kouseki >= 공적비교_1)
								{
									person_0.name_read = pk::encode("常胜将军");
								}
							}

							if (person_0.get_id() == 무장_마초)
							{
								if ((person_0.kouseki >= 공적비교_0) and (person_0.kouseki < 공적비교_1))
								{
									person_0.name_read = pk::encode("锦马超");
								}
								else if (person_0.kouseki >= 공적비교_1)
								{
									person_0.name_read = pk::encode("神威将军");
								}
							}

							if (person_0.get_id() == 무장_황충)
							{
								if ((person_0.kouseki >= 공적비교_1)
									and (person_0.face == 1023 or person_0.face == 949)
									)
									person_0.name_read = pk::encode("老当益壮");
							}

							if (person_0.get_id() == 무장_제갈량)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("卧龙");
							}

							if (person_0.get_id() == 무장_방통)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("凤雏");
							}

							if (person_0.get_id() == 무장_강유)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("麒麟儿");
							}

							if (person_0.get_id() == 무장_마량)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("白眉");
							}


							// 吳
							if (person_0.get_id() == 무장_손견)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("江东猛虎");
							}

							if (person_0.get_id() == 무장_손책)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("小霸王");
							}

							if (person_0.get_id() == 무장_손권)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("碧眼紫须");
							}

							if (person_0.get_id() == 무장_주유)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("美周郎");
							}

							if (person_0.get_id() == 무장_감녕)
							{
								if ((person_0.kouseki >= 공적비교_0) and (person_0.kouseki < 30000))
								{
									person_0.name_read = pk::encode("锦帆贼");
									//person_0.azana_read = "";
									甘宁特技还原();
								}
								else if (person_0.kouseki >= 30000)
								{
									person_0.name_read = pk::encode("江东新风");  //강좌호신
									//person_0.azana_read ="";
									甘宁特技升级();
								}
							}

							if (person_0.get_id() == 무장_대교)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("江东二乔");
							}

							if (person_0.get_id() == 무장_소교)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("江东二乔");
							}

							if ((person_0.get_id() == 무장_태사자) and (person_0.tekisei[병종_노병] >= 적성_S))
							{
								if ((person_0.kouseki >= 공적비교_1)
									and (person_0.tekisei[병종_노병] >= 적성_S)
									)
									person_0.name_read = pk::encode("神弓");
							}


							// 기타 (西晉 포함)
							if (person_0.get_id() == 무장_엄백호)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("东吴德王");
							}

							if (person_0.get_id() == 무장_원소)
							{
								if ((person_0.kouseki >= 공적비교_0) and (person_0.kouseki < 공적비교_1))
								{
									person_0.name_read = pk::encode("北方枭雄");
								}
								else if (person_0.kouseki >= 공적비교_1)
								{
									person_0.name_read = pk::encode("四世三公");
								}
							}

							if (person_0.get_id() == 무장_안량)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("河北双壁");
							}

							if (person_0.get_id() == 무장_문추)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("河北双壁");
							}

							if (person_0.get_id() == 무장_원술)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("蜂蜜皇帝");
							}

							if (person_0.get_id() == 무장_여포)
							{
								if ((person_0.kouseki >= 공적비교_0) and (person_0.kouseki < 공적비교_1))
								{
									person_0.name_read = pk::encode("飞将");
								}
								else if (person_0.kouseki >= 공적비교_1)
								{
									person_0.name_read = pk::encode("绝世无双");
								}
							}

							if (person_0.get_id() == 무장_여영기)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("飞将之女");
							}

							if (person_0.get_id() == 무장_두예)
							{
								if ((person_0.kouseki >= 공적비교_0) and (person_0.kouseki < 공적비교_1))
								{
									person_0.name_read = pk::encode("杜武库");
								}
								else if (person_0.kouseki >= 공적비교_1)
								{
									person_0.name_read = pk::encode("左传癖");
								}
							}

							if (person_0.get_id() == 무장_고순)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("陷阵营");
							}

							if (person_0.get_id() == 무장_초선)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("倾国之色");
							}

							if (person_0.get_id() == 무장_공손찬)
							{
								if (person_0.kouseki >= 공적비교_1)
<<<<<<< HEAD
									person_0.name_read = pk::encode("白马长史");
=======
									person_0.name_read = pk::encode("白马将军");
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
							}

							if (person_0.get_id() == 무장_장각)
							{
								if (person_0.kouseki >= 공적비교_1)
<<<<<<< HEAD
									person_0.name_read = pk::encode("白马长史");
=======
									person_0.name_read = pk::encode("天公将军");
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
							}

							if (person_0.get_id() == 무장_장보)
							{
								if (person_0.kouseki >= 공적비교_1)
<<<<<<< HEAD
									person_0.name_read = pk::encode("地马长史");
=======
									person_0.name_read = pk::encode("地公将军");
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
							}

							if (person_0.get_id() == 무장_장량)
							{
								if (person_0.kouseki >= 공적비교_1)
<<<<<<< HEAD
									person_0.name_read = pk::encode("人马长史");
=======
									person_0.name_read = pk::encode("人公将军");
>>>>>>> d4adedd2760ce1490eb9ba35d7c5e25622e8f321
							}

							if (person_0.get_id() == 무장_항적)
							{
								if (person_0.kouseki >= 공적비교_1)
									person_0.name_read = pk::encode("西楚霸王");
							}


						} // for 

					} // if 						

				} // if

			} // for 

		} // reward_nickname_set()
		//화신火神
		void 甘宁特技升级()
		{

			pk::load_xml("""
				<pk utf8 = "true" trace = "false" >
				<person id = "118">
				<hidden_skill value = "威风, 强行"/> 
				<威风 value = "25" />
				<强行 value = "15" />
				</person>
				</pk>
				""");				
		}//甘宁特技升级
		void 甘宁特技还原()
		{

			pk::load_xml("""
				<pk utf8 = "true" trace = "false" >
				<person id = "118">
				<hidden_skill value = "威风" />
				<威风 value = "20" />
				<强行 value = "0" />
				</person>
				</pk>
				""");				
		}//甘宁特技还原

		void 陆逊特技升级()
		{

			pk::load_xml("""
				<pk utf8 = "true" trace = "false" >
				<person id = "602">
				<hidden_skill value = "火神" />
				</person>
				</pk>
				""");				
		}//甘宁特技升级
		void 陆逊特技还原()
		{

			pk::load_xml("""
				<pk utf8 = "true" trace = "false" >
				<person id = "602">
				<hidden_skill value = "" />
				</person>
				</pk>
				""");				
		}//甘宁特技还原			
	} // class Main

	Main main;

} // namespace
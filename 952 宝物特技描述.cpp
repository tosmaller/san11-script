// ## 2021/08/30 # 江东新风 # 宝物名称改中文 ##
// ## 2021/08/30 # 氕氘氚 # 修改文字细节失误 ##
// ## 2020/12/03 # 氕氘氚 # 合并100个宝物 ##
// @@ 2020/07/26 @ HoneyBee @ 원본보물의 능력 설명을 작성하는 용도의 SCRIPT @@


namespace 원본보물_설명
{

	class Main
	{
		Main()
		{
			pk::bind(103, pk::trigger103_t(callback));
		}

		// 원본보물 명마 (Gold Edition Patch 기준 [이동력 상승/특기_강행])
		pk::item@ item_赤兎马 = pk::get_item(宝物_赤兎马);
		pk::item@ item_的卢 = pk::get_item(宝物_的卢);
		pk::item@ item_爪黄飞电 = pk::get_item(宝物_爪黄飞电);
		pk::item@ item_绝影 = pk::get_item(宝物_绝影);
		pk::item@ item_大宛马 = pk::get_item(宝物_大宛马);
		pk::item@ item_大宛马2 = pk::get_item(宝物_大宛马2);
		pk::item@ item_大宛马3 = pk::get_item(宝物_大宛马3);

		// 원본보물 무기 (Gold Edition Patch 기준 [무력 상승])
		pk::item@ item_七星宝刀 = pk::get_item(宝物_七星宝刀);
		pk::item@ item_倚天剑 = pk::get_item(宝物_倚天剑);
		pk::item@ item_青釭剑 = pk::get_item(宝物_青釭剑);
		pk::item@ item_雌雄一对剑 = pk::get_item(宝物_雌雄一对剑);
		pk::item@ item_方天画戟 = pk::get_item(宝物_方天画戟);
		pk::item@ item_青龙偃月刀 = pk::get_item(宝物_青龙偃月刀);
		pk::item@ item_蛇矛 = pk::get_item(宝物_蛇矛);
		pk::item@ item_流星锤 = pk::get_item(宝物_流星锤);
		pk::item@ item_飞刀 = pk::get_item(宝物_飞刀);
		pk::item@ item_飞刀2 = pk::get_item(宝物_飞刀2);
		pk::item@ item_短戟 = pk::get_item(宝物_短戟);
		pk::item@ item_短戟2 = pk::get_item(宝物_短戟2);
		pk::item@ item_手戟 = pk::get_item(宝物_手戟);
		pk::item@ item_手戟2 = pk::get_item(宝物_手戟2);
		pk::item@ item_铁锁 = pk::get_item(宝物_铁锁);
		pk::item@ item_养由基弓 = pk::get_item(宝物_养由基弓);
		pk::item@ item_李广弓 = pk::get_item(宝物_李广弓);
		pk::item@ item_檀弓 = pk::get_item(宝物_檀弓);
		pk::item@ item_檀弓2 = pk::get_item(宝物_檀弓2);
		pk::item@ item_东胡飞弓 = pk::get_item(宝物_东胡飞弓);
		pk::item@ item_东胡飞弓2 = pk::get_item(宝物_东胡飞弓2);
		pk::item@ item_貊弓 = pk::get_item(宝物_貊弓);
		pk::item@ item_貊弓2 = pk::get_item(宝物_貊弓2);

		// 원본보물 서책 (Gold Edition Patch 기준 [통솔 상승])
		pk::item@ item_孙子兵法 = pk::get_item(宝物_孙子兵法);
		pk::item@ item_兵法二十四篇 = pk::get_item(宝物_兵法二十四篇);
		pk::item@ item_孟徳新书 = pk::get_item(宝物_孟徳新书);
		pk::item@ item_六韬 = pk::get_item(宝物_六韬);
		pk::item@ item_三略 = pk::get_item(宝物_三略);
		pk::item@ item_吴子 = pk::get_item(宝物_吴子);
		pk::item@ item_尉缭子 = pk::get_item(宝物_尉缭子);
		pk::item@ item_司马法 = pk::get_item(宝物_司马法);
		pk::item@ item_墨子 = pk::get_item(宝物_墨子);

		// 원본보물 서책 (Gold Edition Patch 기준 [정치 상승])
		pk::item@ item_战国策 = pk::get_item(宝物_战国策);

		// 원본보물 서책 (Gold Edition Patch 기준 [지력 상승])
		pk::item@ item_太平要术 = pk::get_item(宝物_太平要术);

		// 원본보물 
		pk::item@ item_玉玺 = pk::get_item(宝物_玉玺);
		pk::item@ item_铜雀 = pk::get_item(宝物_铜雀);

		// =====================================================================================
		// 新宝物

		// Gold Edition Patch 추가보물 (명마) [이동력 상승/특기_강행]
		pk::item@ item_照夜玉狮子 = pk::get_item(宝物_照夜玉狮子); 			// 백룡
		pk::item@ item_卷毛赤兔马 = pk::get_item(51); 		// 권모적토마
		pk::item@ item_里沙飞 = pk::get_item(52); 			// 백마
		pk::item@ item_黄巾军兵符 = pk::get_item(53); 			// 백마
		pk::item@ item_西凉铁骑兵符 = pk::get_item(61); 			// 오환마
		pk::item@ item_无当飞军兵符 = pk::get_item(62); 			// 흉노마


		// Gold Edition Patch 추가보물 (무기) [무력 상승]
		pk::item@ item_双铁戟 = pk::get_item(54); 			// 쌍철극
		pk::item@ item_方天戟 = pk::get_item(55); 		// 방천화극
		pk::item@ item_铁脊蛇矛 = pk::get_item(56); 		// 铁脊蛇矛
		pk::item@ item_大戟士兵符 = pk::get_item(65); 			// 大戟士兵符
		pk::item@ item_三尖刀 = pk::get_item(67); 			// 三尖刀
		pk::item@ item_大斧 = pk::get_item(68); 			// 大斧
		pk::item@ item_元戎弩兵兵符 = pk::get_item(69); 			// 元戎弩兵兵符
		pk::item@ item_铁鞭 = pk::get_item(70); 			// 铁鞭
		pk::item@ item_铁蒺藜骨朶 = pk::get_item(71); 		// 铁蒺藜骨朶
		pk::item@ item_丹阳兵兵符 = pk::get_item(72); 			// 丹阳兵兵符
		pk::item@ item_锦帆军兵符 = pk::get_item(73); 			// 锦帆军兵符
		pk::item@ item_铁锤 = pk::get_item(74); 			// 铁锤
		pk::item@ item_白耳军兵符 = pk::get_item(75); 			// 白耳军兵符


		// Gold Edition Patch 추가보물 (서책) [통솔 상승]
		pk::item@ item_西蜀地形图 = pk::get_item(85); 		// 西蜀地形图
		pk::item@ item_平蛮指掌图 = pk::get_item(86); 		// 平蛮指掌图


		// Gold Edition Patch 추가보물 (서책) [지력 상승]
		pk::item@ item_论语集解 = pk::get_item(76); 		// 论语集解
		pk::item@ item_典论 = pk::get_item(77); 			// 典论
		pk::item@ item_博弈论 = pk::get_item(78); 			// 博弈论
		pk::item@ item_时要论 = pk::get_item(79); 			// 时要论
		pk::item@ item_治论 = pk::get_item(80); 			// 治论
		pk::item@ item_辩道论 = pk::get_item(81); 			// 辩道论
		pk::item@ item_乾象历注 = pk::get_item(82); 		// 乾象历注
		pk::item@ item_孝经传 = pk::get_item(83); 			// 孝经传
		pk::item@ item_仇国论 = pk::get_item(84); 			// 仇国论


		// Gold Edition Patch 추가보물 (특수병종) [특기 부여]
		pk::item@ item_东州兵兵符 = pk::get_item(57); 			// 东州兵兵符
		pk::item@ item_陷阵营兵符 = pk::get_item(58); 			// 陷阵营兵符
		pk::item@ item_飞熊兵兵符 = pk::get_item(59); 		// 飞熊兵兵符
		pk::item@ item_役兽部队兵符 = pk::get_item(66); 		// 맹수부대 (친만, 위풍, 질주)
		pk::item@ item_羌骑兵兵符 = pk::get_item(87); 			// 羌骑兵兵符 (친강, 기장)
		pk::item@ item_乌丸兵兵符 = pk::get_item(88); 			// 乌丸兵兵符 (친오, 백마)
		pk::item@ item_匈奴兵兵符 = pk::get_item(89); 			// 匈奴兵兵符 (기장, 백마)
		pk::item@ item_蛮族兵兵符 = pk::get_item(90); 			// 蛮族兵兵符 (친만, 극장, 해독)
		pk::item@ item_巨象兵兵符 = pk::get_item(91); 		// 巨象兵兵符 (친만, 위풍, 공성)
		pk::item@ item_藤甲兵兵符 = pk::get_item(92); 			// 藤甲兵兵符 (친만, 등갑, 금강)
		pk::item@ item_虎豹骑兵符 = pk::get_item(93); 			// 虎豹骑兵符 (기장, 질주)
		pk::item@ item_青州兵兵符 = pk::get_item(94); 			// 青州兵兵符 (창장, 불굴)
		pk::item@ item_山越兵兵符 = pk::get_item(95); 			// 山越兵兵符 (친월, 난전)
		pk::item@ item_五溪蛮族兵符 = pk::get_item(96); 		// 맹수부대 (친만, 위풍, 질주)
		pk::item@ item_白马义从兵符 = pk::get_item(97); 		// 白马义从兵符 (백마, 질주)
		pk::item@ item_铁车兵兵符 = pk::get_item(98); 			// 铁车兵兵符 (친강, 연전, 금강)


		// Gold Edition Patch 추가보물
		pk::item@ item_虎贲虎卫兵符 = pk::get_item(60); 			// 虎贲虎卫兵符
		pk::item@ item_解烦营兵符 = pk::get_item(63); 			// 解烦营兵符 (특기_맹장)
		pk::item@ item_先登营兵符 = pk::get_item(64); 			// 先登营兵符 (特技_攻城)

		// =====================================================================================


		void callback()
		{
			set_item_read();
		}

		void set_item_read()
		{


			// 원본보물 명마 (Gold Edition Patch 기준 [이동력 상승/특기_강행])
			item_赤兎马.name_read = pk::encode("强行, 武力 +1");
			item_赤兎马.update();

			item_的卢.name_read = pk::encode("强行");
			item_的卢.update();

			item_爪黄飞电.name_read = pk::encode("强行");
			item_爪黄飞电.update();

			item_绝影.name_read = pk::encode("强行");
			item_绝影.update();

			item_大宛马.name_read = pk::encode("强行");
			item_大宛马.update();

			item_大宛马2.name_read = pk::encode("强行");
			item_大宛马2.update();

			item_大宛马3.name_read = pk::encode("强行");
			item_大宛马3.update();


			//-----------------------------------------------------------------------------------
			//자웅일대검

			// 원본보물 무기 (Gold Edition Patch 기준 [무력 상승])
			item_七星宝刀.name_read = pk::encode("武力 +5");
			item_七星宝刀.update();

			item_倚天剑.name_read = pk::encode("武力 +6");
			item_倚天剑.update();

			item_青釭剑.name_read = pk::encode("武力 +5");
			item_青釭剑.update();

			item_雌雄一对剑.name_read = pk::encode("武力 +3");
			item_雌雄一对剑.update();

			item_方天画戟.name_read = pk::encode("武力 +8");
			item_方天画戟.update();

			item_青龙偃月刀.name_read = pk::encode("武力 +6");
			item_青龙偃月刀.update();

			item_蛇矛.name_read = pk::encode("武力 +6");
			item_蛇矛.update();

			item_流星锤.name_read = pk::encode("武力 +3");
			item_流星锤.update();

			item_飞刀.name_read = pk::encode("武力 +2");
			item_飞刀.update();

			item_飞刀2.name_read = pk::encode("武力 +2");
			item_飞刀2.update();

			item_短戟.name_read = pk::encode("武力 +2");
			item_短戟.update();

			item_短戟2.name_read = pk::encode("武力 +2");
			item_短戟2.update();

			item_手戟.name_read = pk::encode("武力 +2");
			item_手戟.update();

			item_手戟2.name_read = pk::encode("武力 +2");
			item_手戟2.update();

			item_铁锁.name_read = pk::encode("武力 +2");
			item_铁锁.update();

			item_养由基弓.name_read = pk::encode("武力 +4");
			item_养由基弓.update();

			item_李广弓.name_read = pk::encode("武力 +4");
			item_李广弓.update();

			item_檀弓.name_read = pk::encode("武力 +1");
			item_檀弓.update();

			item_檀弓2.name_read = pk::encode("武力 +1");
			item_檀弓2.update();

			item_东胡飞弓.name_read = pk::encode("武力 +2");
			item_东胡飞弓.update();

			item_东胡飞弓2.name_read = pk::encode("武力 +2");
			item_东胡飞弓2.update();

			item_貊弓.name_read = pk::encode("武力 +1");
			item_貊弓.update();

			item_貊弓2.name_read = pk::encode("武力 +1");
			item_貊弓2.update();


			//-----------------------------------------------------------------------------------


			// 원본보물 서책 (Gold Edition Patch 기준 [통솔 상승])
			item_孙子兵法.name_read = pk::encode("统率 +10");
			item_孙子兵法.update();

			item_兵法二十四篇.name_read = pk::encode("统率 +2");
			item_兵法二十四篇.update();

			item_孟徳新书.name_read = pk::encode("统率 +2");
			item_孟徳新书.update();

			item_六韬.name_read = pk::encode("统率 +8");
			item_六韬.update();

			item_三略.name_read = pk::encode("统率 +8");
			item_三略.update();

			item_吴子.name_read = pk::encode("统率 +6");
			item_吴子.update();

			item_尉缭子.name_read = pk::encode("统率 +5");
			item_尉缭子.update();

			item_司马法.name_read = pk::encode("统率 +7");
			item_司马法.update();

			item_墨子.name_read = pk::encode("统率 +4");
			item_墨子.update();


			//-----------------------------------------------------------------------------------


			// 원본보물 서책 (Gold Edition Patch 기준 [정치 상승])
			item_战国策.name_read = pk::encode("政治 +3");
			item_战国策.update();


			// 원본보물 서책 (Gold Edition Patch 기준 [지력 상승])
			item_太平要术.name_read = pk::encode("智力 +2");
			item_太平要术.update();


			// 원본보물 
			item_玉玺.name_read = pk::encode("魅力 +10");
			item_玉玺.update();

			//item_铜雀.name_read = pk::encode("");
			//item_铜雀.update();

			// =====================================================================================
			// 新宝物

			// Gold Edition Patch 추가보물 (명마) [이동력 상승/특기_강행]
			item_照夜玉狮子.name_read = pk::encode("强行");
			item_照夜玉狮子.update();

			item_卷毛赤兔马.name_read = pk::encode("强行");
			item_卷毛赤兔马.update();

			item_里沙飞.name_read = pk::encode("强行");
			item_里沙飞.update();

			item_黄巾军兵符.name_read = pk::encode("攻心, 妖术");
			item_黄巾军兵符.update();

			item_西凉铁骑兵符.name_read = pk::encode("亲羌, 长驱, 疾驰");
			item_西凉铁骑兵符.update();

			item_无当飞军兵符.name_read = pk::encode("强行, 踏破");
			item_无当飞军兵符.update();


			//-----------------------------------------------------------------------------------


			// Gold Edition Patch 추가보물 (무기) [무력 상승]
			item_双铁戟.name_read = pk::encode("武力 +1");
			item_双铁戟.update();

			item_方天戟.name_read = pk::encode("武力 +1");
			item_方天戟.update();

			item_铁脊蛇矛.name_read = pk::encode("武力 +1");
			item_铁脊蛇矛.update();

			item_大戟士兵符.name_read = pk::encode("戟将, 昂扬");
			item_大戟士兵符.update();

			item_三尖刀.name_read = pk::encode("武力 +3");
			item_三尖刀.update();

			item_大斧.name_read = pk::encode("武力 +2");
			item_大斧.update();

			item_元戎弩兵兵符.name_read = pk::encode("弓将, 射手");
			item_元戎弩兵兵符.update();

			item_铁鞭.name_read = pk::encode("武力 +1");
			item_铁鞭.update();

			item_铁蒺藜骨朶.name_read = pk::encode("武力 +2");
			item_铁蒺藜骨朶.update();

			item_丹阳兵兵符.name_read = pk::encode("戟将, 扫讨");
			item_丹阳兵兵符.update();

			item_锦帆军兵符.name_read = pk::encode("推进, 操舵, 水将");
			item_锦帆军兵符.update();

			item_铁锤.name_read = pk::encode("武力 +2");
			item_铁锤.update();

			item_白耳军兵符.name_read = pk::encode("枪将, 护卫, 血路");
			item_白耳军兵符.update();


			//-----------------------------------------------------------------------------------


			// Gold Edition Patch 추가보물 (서책) [통솔 상승]
			item_西蜀地形图.name_read = pk::encode("统率 +1");
			item_西蜀地形图.update();

			item_平蛮指掌图.name_read = pk::encode("统率 +1");
			item_平蛮指掌图.update();


			//-----------------------------------------------------------------------------------


			// Gold Edition Patch 추가보물 (서책) [지력 상승]
			item_论语集解.name_read = pk::encode("智力 +4");
			item_论语集解.update();

			item_典论.name_read = pk::encode("智力 +4");
			item_典论.update();

			item_博弈论.name_read = pk::encode("智力 +3");
			item_博弈论.update();

			item_时要论.name_read = pk::encode("智力 +3");
			item_时要论.update();

			item_治论.name_read = pk::encode("智力 +2");
			item_治论.update();

			item_辩道论.name_read = pk::encode("智力 +2");
			item_辩道论.update();

			item_乾象历注.name_read = pk::encode("智力 +1");
			item_乾象历注.update();

			item_孝经传.name_read = pk::encode("智力 +1");
			item_孝经传.update();

			item_仇国论.name_read = pk::encode("智力 +1");
			item_仇国论.update();


			//-----------------------------------------------------------------------------------


			// Gold Edition Patch 추가보물 (특수병종) [특기 부여]
			item_东州兵兵符.name_read = pk::encode("威压, 扫讨");
			item_东州兵兵符.update();

			item_陷阵营兵符.name_read = pk::encode("铁壁, 沉着");
			item_陷阵营兵符.update();

			item_飞熊兵兵符.name_read = pk::encode("强行, 骑将");
			item_飞熊兵兵符.update();

			item_役兽部队兵符.name_read = pk::encode("亲蛮, 威风, 疾驰");
			item_役兽部队兵符.update();

			item_羌骑兵兵符.name_read = pk::encode("亲羌, 骑将");
			item_羌骑兵兵符.update();

			item_乌丸兵兵符.name_read = pk::encode("亲乌, 白马");
			item_乌丸兵兵符.update();

			item_匈奴兵兵符.name_read = pk::encode("骑将, 白马");
			item_匈奴兵兵符.update();

			item_蛮族兵兵符.name_read = pk::encode("亲蛮, 戟将, 解毒");
			item_蛮族兵兵符.update();

			item_巨象兵兵符.name_read = pk::encode("亲蛮, 威风, 攻城");
			item_巨象兵兵符.update();

			item_藤甲兵兵符.name_read = pk::encode("亲蛮, 藤甲, 金刚");
			item_藤甲兵兵符.update();

			item_虎豹骑兵符.name_read = pk::encode("骑将, 疾驰");
			item_虎豹骑兵符.update();

			item_青州兵兵符.name_read = pk::encode("枪将, 不屈");
			item_青州兵兵符.update();

			item_山越兵兵符.name_read = pk::encode("亲越, 乱战");
			item_山越兵兵符.update();

			item_五溪蛮族兵符.name_read = pk::encode("亲蛮, 乱战");
			item_五溪蛮族兵符.update();

			item_白马义从兵符.name_read = pk::encode("白马, 疾驰");
			item_白马义从兵符.update();

			item_铁车兵兵符.name_read = pk::encode("亲羌, 连战, 金刚");
			item_铁车兵兵符.update();


			//-----------------------------------------------------------------------------------

			// Gold Edition Patch 추가보물
			item_解烦营兵符.name_read = pk::encode("枪将, 金刚");
			item_解烦营兵符.update();

			item_先登营兵符.name_read = pk::encode("攻城, 怒发");
			item_先登营兵符.update();

			item_虎贲虎卫兵符.name_read = pk::encode("猛者, 铁壁");
			item_虎贲虎卫兵符.update();



		} // set_item_desc()

	} // class Main

	Main main;

} // namespace
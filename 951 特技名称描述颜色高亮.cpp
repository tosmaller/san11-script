// ## 2021/09/26 # 江东新风 # 开始菜单触发，确保特技颜色正常 ##
// ## 2020/10/12 # 氕氘氚 # 配合KRE实现描述自定义、颜色高亮 ##
// ## 2020/09/21 # 江东新风 # 更改法治描述為下降减半，修正陷阵描述 ##
// ## 2020/08/30 # 江东新风 # 补充青山依旧逐鹿天下组合特技 ##
// ## 2020/08/24 # 氕氘氚 # 配合KRE和910实现自定义描述 ##
// @@ 2020/08/21 @ 氕氘氚 @@


/* * * * 請 勿 手 动 修 改 本 文 件 * * * */


namespace 特技名称描述
{
	class Main
	{

		Main()
		{
			pk::bind(102, 100, pk::trigger102_t(onGameInit));
			pk::bind(100, 100, pk::trigger100_t(onGameInit));//开始菜单
		}

		//---------------------------------------------------------------------------------------

		void onGameInit()
		{
			change_pk_skill_info();
		}

		//---------------------------------------------------------------------------------------
		// pk2.1 특기.xml 설정값을 특기 설명에 반영
		void change_pk_skill_info()
		{
			/*start*/
			set_skill_info(0, "\x1b[22x飞将", "无视陆上阻碍；敌部队武力低于己者，发动战法成功后暴击");
			set_skill_info(1, "\x1b[23x遁走", "无视陆上阻碍");
			set_skill_info(2, "\x1b[23x强行", "除兵器、输送队外的陆上部队移动力+{5}");
			set_skill_info(3, "\x1b[00x长驱", "骑兵部队移动力+{3}");
			set_skill_info(4, "\x1b[23x推进", "无视水上阻碍；擅长水战");
			set_skill_info(5, "\x1b[23x操舵", "水上部队移动力+{4}；擅长水战");
			set_skill_info(6, "\x1b[19x踏破", "栈道、山地、水岸通行时被害减少；受陷阱、火伤害为{10}％");
			set_skill_info(7, "\x1b[23x运输", "输送部队的移动力+{5}；辎重不会损失");
			set_skill_info(8, "\x1b[23x用毒", "[毒泉]地形伤害免疫；弓箭伤害增加（毒箭）");
			set_skill_info(9, "\x1b[00x扫讨", "攻击敌部队时使其气力减{5}点");
			set_skill_info(10, "\x1b[23x威风", "攻击敌部队时使其气力减{20}点");
			set_skill_info(11, "\x1b[23x昂扬", "击破敌部队后回复气力{10}点；攻击敌部队后回复气力{5}点");
			set_skill_info(12, "\x1b[19x连战", "普通攻击时，{50}%概率二次攻击");
			set_skill_info(13, "\x1b[23x急袭", "陆上攻击时，{50}%概率不受反击");
			set_skill_info(14, "\x1b[23x强袭", "水上攻击时，{50}%概率不受反击");
			set_skill_info(15, "\x1b[00x乱战", "在森林地形攻击时必暴击");
			set_skill_info(16, "\x1b[00x待伏", "[伏兵]计策成功时必暴击");
			set_skill_info(17, "\x1b[23x攻城", "对城市、设施、建设物攻击时发动暴击");
			set_skill_info(18, "\x1b[00x掎角", "一齐攻击时，{50}％概率使敌混乱");
			set_skill_info(19, "\x1b[19x擒拿", "击破敌方部队时，{100}％概率将敌方武将捕缚");
			set_skill_info(20, "\x1b[23x精妙", "击破敌部队获得双倍技巧值；增加战法成功率{10}%");
			set_skill_info(21, "\x1b[00x强夺", "敌部队击破时，{10}％概率夺取敌将宝物一件");
			set_skill_info(22, "\x1b[23x攻心", "近战攻击时，己方造成伤害的{15}％的敌士兵叛逃至己方阵营");
			set_skill_info(23, "\x1b[00x驱逐", "对武力低于己的敌部队普通攻击必暴击；攻击有概率不受反击");
			set_skill_info(24, "\x1b[23x射程", "井阑、投石车射程增加<1>格");
			set_skill_info(25, "\x1b[23x骑射", "技巧[骑射]未研究时便可使用骑射；骑射时暴击、伤害增加");
			set_skill_info(26, "\x1b[00x辅佐", "较之普通关系武将，多{30}%概率可发生支援效果（厌恶除外）");
			set_skill_info(27, "\x1b[23x不屈", "己方部队士兵小于{3000}时，{50}％概率抵抗对方攻击");
			set_skill_info(28, "\x1b[23x金刚", "敌部队伤害少于{500}时，{50}％概率抵抗对方攻击");
			set_skill_info(29, "\x1b[23x铁壁", "敌一齐攻击无效，视为一次普通攻击");
			set_skill_info(30, "\x1b[00x怒发", "被敌部队发动战法攻击后，己方气力回复{5}点");
			set_skill_info(31, "\x1b[19x藤甲", "除火伤害外、其他攻击伤害为{50}%，火计被害为{200}%");
			set_skill_info(32, "\x1b[22x幸运", "不会战死、被捕、负伤");
			set_skill_info(33, "\x1b[23x血路", "己方部队被击破后，同部队武将不会被捕缚");
			set_skill_info(34, "\x1b[00x枪将", "对武力低于己者，发动枪兵战法成功时暴击");
			set_skill_info(35, "\x1b[00x戟将", "对武力低于己者，发动戟兵战法成功时暴击");
			set_skill_info(36, "\x1b[00x弓将", "对武力低于己者，发动弩兵战法成功时暴击");
			set_skill_info(37, "\x1b[00x骑将", "对武力低于己者，发动骑兵战法成功时暴击");
			set_skill_info(38, "\x1b[00x水将", "对武力低于己者，发动水军战法成功时暴击");
			set_skill_info(39, "\x1b[19x勇将", "对武力低于己者，发动全部战法成功时暴击");
			set_skill_info(40, "\x1b[19x神将", "对武力低于己者，普通攻击或战法成功时暴击");
			set_skill_info(41, "\x1b[19x斗神", "发动枪兵、戟兵战法成功时暴击");
			set_skill_info(42, "\x1b[22x枪神", "发动枪兵战法成功时暴击+【要击】+【军魂】");
			set_skill_info(43, "\x1b[22x戟神", "发动戟兵战法成功时暴击+【布阵】+【军魂】");
			set_skill_info(44, "\x1b[22x弓神", "发动弩兵战法成功时暴击+【神臂】+【军魂】");
			set_skill_info(45, "\x1b[22x骑神", "发动骑兵战法成功时暴击+【疾驰】+【军魂】");
			set_skill_info(46, "\x1b[22x工神", "发动兵器战法成功时暴击+【射程】+【军魂】");
			set_skill_info(47, "\x1b[22x水神", "发动水军战法成功时暴击+【操舵】+【军魂】");
			set_skill_info(48, "\x1b[22x霸王", "全战法成功时暴击+【恫吓】");
			set_skill_info(49, "\x1b[19x疾驰", "对攻击力低于己的敌部队，发动骑兵战法成功时，{100}%概率使敌混乱");
			set_skill_info(50, "\x1b[23x射手", "对森林亦可发动弓兵战法");
			set_skill_info(51, "\x1b[23x猛者", "施放战法使目标队被迫移动时，{50}%概率使敌任一武将负伤");
			set_skill_info(52, "\x1b[23x护卫", "同部队武将不会战死、负伤；所属城市武将遇刺，有概率出战抵挡");
			set_skill_info(53, "\x1b[19x火攻", "对比己方智力低的敌部队「火计」必成功；火伤害增加、被害减少");
			set_skill_info(54, "\x1b[23x言毒", "对比己方智力低的敌部队「伪报」必成功");
			set_skill_info(55, "\x1b[23x机略", "对比己方智力低的敌部队「扰乱」必成功");
			set_skill_info(56, "\x1b[23x诡计", "对比己方智力低的敌部队「内讧」必成功");
			set_skill_info(57, "\x1b[19x虚实", "对智力低于己方的敌部队，全计略必成功");
			set_skill_info(58, "\x1b[23x妙计", "对比己方智力低的敌人，计略成功时暴击；计略气力消耗减少{3}点");
			set_skill_info(59, "\x1b[23x秘计", "对比己方智力高的敌人，计略成功时暴击；计略气力消耗减少{5}点");
			set_skill_info(60, "\x1b[19x看破", "比己方智力低的敌部队对己计略时必看破");
			set_skill_info(61, "\x1b[22x洞察", "敌部队计略必看破，全计略免疫；受战法成功攻击的概率降低");
			set_skill_info(62, "\x1b[22x火神", "智力低于己者火计必成、必暴；火伤害为{200}%；受到的火伤为{0}%");
			set_skill_info(63, "\x1b[22x神算", "对智力低于己的敌部队计略必成功、必暴击，计略全面优待");
			set_skill_info(64, "\x1b[19x百出", "所有计略的消耗气力{减半}");
			set_skill_info(65, "\x1b[19x鬼谋", "计略影响范围扩大<1>格");
			set_skill_info(66, "\x1b[19x连环", "对敌部队计略成功后,对敌相临的一支部队造成连锁伤害");
			set_skill_info(67, "\x1b[23x深谋", "对敌部队计略成功时必暴击");
			set_skill_info(68, "\x1b[19x反计", "看破敌计略后反作用于敌");
			set_skill_info(69, "\x1b[22x奇谋", "对敌部队计略成功率<2>倍");
			set_skill_info(70, "\x1b[19x妖术", "可使用终极魔法[妖术]");
			set_skill_info(71, "\x1b[22x鬼门", "可使用终极魔法[妖术、落雷]");
			set_skill_info(72, "\x1b[23x规律", "对敌部队计略[伪报]免疫");
			set_skill_info(73, "\x1b[23x沉着", "对敌部队计略[扰乱]免疫");
			set_skill_info(74, "\x1b[19x明镜", "对敌部队计略[伪报、扰乱]免疫");
			set_skill_info(75, "\x1b[23x奏乐", "己方部队每回合回复气力{5}点");
			set_skill_info(76, "\x1b[00x诗想", "军乐台气力回复效果<2>倍");
			set_skill_info(77, "\x1b[00x筑城", "建造军事设施时建造速度翻倍");
			set_skill_info(78, "\x1b[19x屯田", "驻扎在关所、港口时，兵士不消耗军粮；据点粮草消耗减少为{67}%");
			set_skill_info(79, "\x1b[23x名声", "征兵时，征兵量提升{50}％");
			set_skill_info(80, "\x1b[23x能吏", "枪、戟、弩等兵器生产量增加{50}％");
			set_skill_info(81, "\x1b[23x繁殖", "军马生产量增加{50}％");
			set_skill_info(82, "\x1b[00x发明", "兵装、兵器生产时，生产周期减为原先的{50}%");
			set_skill_info(83, "\x1b[00x造船", "舰船生产时，生产周期减为原先的{50}%");
			set_skill_info(84, "\x1b[23x指导", "技巧研究费用减半、经验两倍；部队作战经验亦两倍");
			set_skill_info(85, "\x1b[00x眼力", "人材探索时，必定发现城市在野人才；于人才选拔有优势");
			set_skill_info(86, "\x1b[23x论客", "外交[同盟、停战、俘虏交换]时，{100}%概率强制舌战（超级除外）");
			set_skill_info(87, "\x1b[19x积蓄", "所属都市每月金收入变为两倍");
			set_skill_info(88, "\x1b[19x劝农", "所属都市每季粮草收入变为两倍");
			set_skill_info(89, "\x1b[19x度支", "城市金收入改为每旬增加原每月金收入的一半");
			set_skill_info(90, "\x1b[19x平准", "城市粮草收入改为每月增加原每季粮食收入的一半");
			set_skill_info(91, "\x1b[19x亲乌", "所属城市不出[乌丸]（雇佣军除外）；强化骑兵伤害；可招募雇佣兵");
			set_skill_info(92, "\x1b[19x亲羌", "所属城市不出[羌]（雇佣军除外）；强化骑兵伤害；可招募雇佣兵");
			set_skill_info(93, "\x1b[19x亲越", "所属城市不出[山越]（雇佣军除外）；强化戟兵伤害；可招募雇佣兵");
			set_skill_info(94, "\x1b[19x亲蛮", "所属城市不出[南蛮]（雇佣军除外）；强化戟兵伤害；可招募雇佣兵");
			set_skill_info(95, "\x1b[19x威压", "所属城市领地内不易出贼、异族（雇佣军除外）；强化枪兵伤害");
			set_skill_info(96, "\x1b[19x仁政", "所属城市武将忠心不二");
			set_skill_info(97, "\x1b[00x风水", "所属城市[疫病、灾害]减少");
			set_skill_info(98, "\x1b[00x祈愿", "所属都市经常[丰收]");
			set_skill_info(99, "\x1b[00x内助", "结婚后，夫妻双方五维均+{1}");
			set_skill_info(100, "\x1b[19x八卦", "以{50}%的概率抵挡战法攻击");
			set_skill_info(101, "\x1b[00x窥探", "所在据点和部队的视野范围+{2}，使用[侦查]计略成功率上升");
			set_skill_info(102, "\x1b[23x内治", "势力内叛乱发生为一般情况下的{10}%");
			set_skill_info(103, "\x1b[19x戍防", "所在城市免疫黄巾，突袭");
			set_skill_info(104, "\x1b[23x韬略", "使用部队战术成功率上升{20}%，减少所受战术负面效果");
			set_skill_info(105, "\x1b[23x谋略", "使用部队妙计成功率上升{20}%，减少所受妙计负面效果");
			set_skill_info(106, "\x1b[23x军略", "使用部队战略成功率上升{20}%，减少所受战略负面效果");
			set_skill_info(107, "\x1b[23x强募", "所在据点每月自动招募少量士兵");
			set_skill_info(108, "\x1b[23x狙击", "弩兵类战法负伤率上升{20}%");
			set_skill_info(109, "\x1b[23x猛骑", "骑兵类战法负伤率上升{20}%");
			set_skill_info(110, "\x1b[23x猛卒", "步卒类战法负伤率上升{20}%");
			set_skill_info(111, "\x1b[23x严整", "部队辎重不会损失");
			set_skill_info(112, "\x1b[23x辎粮", "部队兵粮消耗减少为{50}%");
			set_skill_info(113, "\x1b[23x山战", "山地行军无损伤、战法必暴击");
			set_skill_info(114, "\x1b[23x背水", "水岸行军无损伤、战法必暴击");
			set_skill_info(115, "\x1b[23x清野", "内政设施被破坏时不会丢失资源");
			set_skill_info(116, "\x1b[19x拱戍", "在己方领土上作战时伤害为{150}%并同比例减少所受伤害");
			set_skill_info(117, "\x1b[19x布阵", "防止被战法暴击；规避兵种相克带来的负面影响");
			set_skill_info(118, "\x1b[23x要击", "对异常状态部队施展主动攻击伤害提升为{150}%");
			set_skill_info(119, "\x1b[23x商才", "善于讨价还价；可以以较低价购买兵装兵器；投资收益增加");
			set_skill_info(120, "\x1b[22x兵圣", "受到的物理伤害变为{50}%");
			set_skill_info(121, "\x1b[22x兵神", "敌方战法成功率和计略成功率大幅下降");
			set_skill_info(122, "\x1b[19x兵心", "对统率较低部队施展战法必成功");
			set_skill_info(123, "\x1b[22x战神", "攻击较高时施展战法会心一击，并对敌方形成较高伤害");
			set_skill_info(124, "\x1b[19x战将", "对防御力比己方攻击力低的敌军，施展战法成功会心一击");
			set_skill_info(125, "\x1b[19x巧变", "可无视地形限制施展战法");
			set_skill_info(126, "\x1b[23x游侠", "对兵力远多于己方的敌方部队，施展战法成功时会心一击");
			set_skill_info(127, "\x1b[19x百战", "部队战法的消费气力为<三分之一>");
			set_skill_info(128, "\x1b[23x蓄势", "战法气力消耗+{20}%，战法伤害+{20}%");
			set_skill_info(129, "\x1b[23x神臂", "使弩兵的射程增加<1>区格");
			set_skill_info(130, "\x1b[19x治军", "免疫物理攻击眩量效果");
			set_skill_info(131, "\x1b[19x坚城", "守城时城防和土兵所受伤害减少为{70}%");
			set_skill_info(132, "\x1b[19x料敌", "敌方部队所施展部队计略成功的机率大幅降低");
			set_skill_info(133, "\x1b[23x陷阵", "反击敌方施展的战法时，反击伤害为{150}%");
			set_skill_info(134, "\x1b[19x恫吓", "武力比自己低的邻接敌方部队有一定机率陷入眩晕");
			set_skill_info(135, "\x1b[19x破竹", "打倒敌方部队后，周围敌方部队也会减少气力");
			set_skill_info(136, "\x1b[23x安民", "攻破敌方城也可保留可全部内政设施");
			set_skill_info(137, "\x1b[23x循吏", "增加建设内政设施的速度");
			set_skill_info(138, "\x1b[00x贤哲", "所属势力每月增加{30}点技巧值");
			set_skill_info(139, "\x1b[19x变法", "每旬行动力恢复量增加；谷仓和造币厂作用提升为<2>倍");
			set_skill_info(140, "\x1b[23x法治", "所在城治安下降减半");
			set_skill_info(141, "\x1b[19x后勤", "军队耗费军粮由据点供给一半；向在城塞周围的部队补充兵力");
			set_skill_info(142, "\x1b[23x医者", "瘟疫时减少伤亡；治疗城市和部队中的武将");
			set_skill_info(143, "\x1b[19x扰敌", "智力比自己低的邻接敌方部队有一定机率陷入眩晕");
			set_skill_info(144, "\x1b[19x运筹", "如果使用计策成功，则有{50}%机率可再行动一次");
			set_skill_info(145, "\x1b[19x激励", "如果使用战法成功，则有{50}%机率可再行动一次");
			set_skill_info(146, "\x1b[23x伯乐", "月初会推荐相性相近的武将");
			set_skill_info(147, "\x1b[23x清谈", "可清谈登庸所属城市的无归属武将");
			set_skill_info(148, "\x1b[19x军魂", "统率光环带来的各种加成效果倍率为{200}%");
			set_skill_info(149, "\x1b[19x武干", "未达到基础光环要求的主帅获得基础光环；否则光环范围增加一格");
			set_skill_info(150, "\x1b[23x摧锋", "枪兵战法破防{40}%");
			set_skill_info(151, "\x1b[19x重甲", "戟兵防御增加{30}点、<50%>概率抵挡普通弓箭攻击、行动减缓");
			set_skill_info(152, "\x1b[23x轻甲", "弩兵近战攻防上升{25}%");
			set_skill_info(153, "\x1b[23x掠阵", "骑兵对途径部队造成伤害");
			set_skill_info(154, "\x1b[23x重器", "兵器防御力提高{40}%");
			set_skill_info(155, "\x1b[23x水师", "水军减伤{30}%；不受水流影响");
			set_skill_info(156, "\x1b[00x调练", "训练的效果变为{150}%");
			set_skill_info(157, "\x1b[00x巡查", "巡查的效果变为{150}%");
			set_skill_info(158, "\x1b[23x劫财", "攻击部队截取{3}%金钱");
			set_skill_info(159, "\x1b[23x截粮", "攻击部队截取{3}%粮食");
			set_skill_info(180, "\x1b[19x飞军", "施展弩兵、骑兵战法时，使敌将负伤几率增加；于西南行军有优势");
			set_skill_info(181, "\x1b[19x闪击", "于陆上时不考虑ZOC；陆上攻击不会受到损伤+【武干】");
			set_skill_info(182, "\x1b[23x步将", "对武力比自己低的敌方部队，施展枪兵和戟兵战法成时会心一击");
			set_skill_info(183, "\x1b[23x骁将", "对武力比自己低的敌方部队，施展骑兵和弩兵战法成时会心一击");
			set_skill_info(184, "\x1b[22x骁神", "弩兵战法与骑兵战法施展成功时会心一击+【军魂】");
			set_skill_info(185, "\x1b[23x毒师", "[毒泉]地形伤害免疫；弓箭伤害增加+【医者】");
			set_skill_info(186, "\x1b[23x骑驱", "对武力低的敌部队施展骑兵战法成功时会心一击；增加骑兵移动力");
			set_skill_info(187, "\x1b[22x卫戍", "【戍防】+【拱戍】+【调练】+【巡查】");
			set_skill_info(188, "\x1b[19x剽掠", "击破部队时有几率抢夺道具，攻击部队截取金钱、兵粮");
			set_skill_info(189, "\x1b[23x义从", "骑兵移动力增强；擅长支援友军");
			set_skill_info(190, "\x1b[22x天骄", "【昂扬】+【激励】+【军魂】");
			set_skill_info(191, "\x1b[22x谋圣", "对智力低于己的敌部队计略必成功；成功时波及邻接部队+【武干】");
			set_skill_info(192, "\x1b[19x才媛", "若结婚，属性增加；每回合自动回复气力；军乐台效果翻倍");
			set_skill_info(193, "\x1b[19x仙君", "城市易丰收、不易出现灾害；每月加技巧点+【医者】");
			set_skill_info(194, "\x1b[19x先师", "征兵数量增加；所属势力每月增加技巧值；研究费用减半");
			set_skill_info(195, "\x1b[19x圣王", "所在城武将不降忠；不容易出现盗贼或异族；征兵数增加");
			set_skill_info(196, "\x1b[22x王佐", "对于内政生产（生产、产马、造器、造船）有优势");
			set_skill_info(197, "\x1b[22x和戍", "所在城市不会有异族之进犯；强化戟兵、骑兵伤害；可以招募雇佣兵");
			set_skill_info(198, "\x1b[19x圣人", "【仁政】+【伯乐】+【清淡】");
			set_skill_info(199, "\x1b[19x统军", "部队辎重不会损失；训练效果提升+【军魂】");
			set_skill_info(200, "\x1b[22x魏武", "【虚实】+【治军】★（曹操）");
			set_skill_info(201, "\x1b[22x仁德", "【仁政】+【遁走】+【安民】★（刘备）");
			set_skill_info(202, "\x1b[22x吴侯", "【指导】+【内治】+【拱戍】★（孙权）");
			set_skill_info(203, "\x1b[22x黄天", "【鬼门】+【名声】+【韬略】★（张角）");
			set_skill_info(204, "\x1b[19x暴君", "【扫讨】+【积蓄】+【强募】★（董卓）");
			set_skill_info(205, "\x1b[22x名门", "【名声】+【后勤】★（袁绍）");
			set_skill_info(206, "\x1b[22x白马", "【骑射】+【破竹】★（公孙瓒）");
			set_skill_info(207, "\x1b[22x蛮王", "【藤甲】+【亲蛮】★（孟获）");
			set_skill_info(208, "\x1b[22x武冠", "全战法成功时暴击+【摧锋】★（孙策）");
			set_skill_info(209, "\x1b[22x无双", "【飞将】+【掠阵】★（吕布）");
			set_skill_info(210, "\x1b[22x武圣", "【兵圣】+【战神】★（关羽)");
			set_skill_info(211, "\x1b[22x咆哮", "【斗神】+【恫吓】★（张飞）");
			set_skill_info(212, "\x1b[22x龙骑", "【陷阵】+【洞察】★（赵云）");
			set_skill_info(213, "\x1b[22x固守", "【拱戍】+【坚城】★（姜维）");
			set_skill_info(214, "\x1b[22x奇袭", "【要击】+【威风】★（张辽）");
			set_skill_info(215, "\x1b[22x虎痴", "【蓄势】+【连战】★（许褚）");
			set_skill_info(216, "\x1b[22x恶来", "【重甲】+【护卫】★（典韦）");
			set_skill_info(217, "\x1b[22x啖睛", "【百战】+【猛骑】★（夏侯惇）");
			set_skill_info(218, "\x1b[22x稳固", "【屯田】+【遁走】★（邓艾）");
			set_skill_info(219, "\x1b[22x白衣", "【攻心】+【操舵】+【兵心】★（吕蒙）");
			set_skill_info(220, "\x1b[22x儒帅", "【火攻】+【连环】+【轻甲】★（陆逊）");
			set_skill_info(221, "\x1b[19x锦帆", "【推进】+【操舵】+【水师】★（甘宁）");
			set_skill_info(222, "\x1b[22x冢虎", "【深谋】+【料敌】★（司马懿）");
			set_skill_info(223, "\x1b[22x卧龙", "【神算】+【八卦】★（诸葛亮）");
			set_skill_info(224, "\x1b[22x周郎", "【火神】+【军略】★（周瑜）");
			set_skill_info(225, "\x1b[22x天妒", "【运筹】+【奇谋】★（郭嘉）");
			set_skill_info(226, "\x1b[22x斗智", "【反计】+【鬼谋】★（贾诩）");
			set_skill_info(227, "\x1b[22x凤雏", "【连环】+【谋略】★（庞统）");
			set_skill_info(228, "\x1b[22x令君", "【能吏】+【眼力】+【循吏】★（荀彧）");
			set_skill_info(229, "\x1b[22x门神", "【护卫】+【血路】★（周泰）");
			set_skill_info(230, "\x1b[19x破虏", "【驱逐】+【扫讨】+【昂扬】★（孙坚）");
			set_skill_info(231, "\x1b[22x人屠", "【战神】+【料敌】+【军魂】★(白起)");
			set_skill_info(232, "\x1b[22x霸王", "【霸王】+【破竹】+【恫吓】+【军魂】★(项藉)");
			set_skill_info(233, "\x1b[22x兵仙", "【斗神】+【兵心】+【武干】★(韩信)");
			set_skill_info(234, "\x1b[22x气机", "【戟神】+【激励】+【布阵】★(吴起)");
			set_skill_info(235, "\x1b[22x兵祖", "【落雷】+【运筹】+【深谋】★(姜尚)");
			set_skill_info(236, "\x1b[22x武穆", "【神将】+【百战】+【军魂】★(岳飞)");
			set_skill_info(237, "\x1b[22x谋圣", "【神算】+【运筹】+【鬼谋】★(张良)");
			set_skill_info(238, "\x1b[22x将星", "【攻城】+【拱戍】+【攻心】★(乐毅)");
			set_skill_info(239, "\x1b[22x信勇", "【戟神】+【弓将】+【射手】★(太史慈)");
			/*end*/

		}


		void set_skill_info(int skill_id, string name, string desc)
		{
			pk::skill@ skill = pk::get_skill(skill_id);
			skill.name = pk::encode(name);
			skill.desc = pk::encode(desc);
		}
		//-----------------------------------

	}

	Main main;
}


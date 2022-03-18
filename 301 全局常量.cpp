// ## 2021/11/03 # 江东新风 # 增长率放入301 ##
// ## 2021/11/03 # 江东新风 # 伤兵显示开关 ##
// ## 2021/02/15 # 江东新风 # 狼骑伤害系数调整 ##
// ## 2021/01/28 # messi # 优化部分特殊地名位置 ##
// ## 2021/01/05 # 江东新风 # 特殊地名设施可争夺常量移入此处，更新地名坐标 ##
// ## 2021/01/03 # 江东新风 # 特殊地名设施数组 ##
// ## 2020/12/13 # 江东新风 # 窥探常量 ##
// ## 2020/10/27 # 氕氘氚 # 删除特技名称 ##
// ## 2020/10/08 # 江东新风 # 特技_恫吓常量修改##
// ## 2020/09/28 # 氕氘氚 # 添加资源生产相关常量 ##
// ## 2020/09/23 # 江东新风 # 修改部分特技常量为简体(狙击，运筹，激励) ##
// ## 2020/08/30 # 江东新风 # 补充?山依旧逐鹿天下组合特技 ##
// ## 2020/08/24 # 氕氘氚 # 常量修改 ##
// ## 2020/08/19 # 江东新风 # 复合特技设定 ##
// ## 2020/08/16 # 氕氘氚 # 添加部分常量 ##
// ## 2020/08/14 # 江东新风 #加入光环系统常量##

const bool 特定地点特定设施可争夺 = true;
const array<int> 地名设施 = { 设施_阵, 设施_砦, 设施_城塞, 设施_弓橹, 设施_连弩橹 };
const array<int> 地名设施类型1 = { 设施_阵, 设施_砦, 设施_城塞};
const array<int> 地名设施类型2 = { 设施_弓橹, 设施_连弩橹 };

//人口测试
const bool 开启人口系统 = true;
const bool 开启伤兵系统 = true;
const bool 开启疑兵伏兵 = true;
const float 人口基础增长率 = 0.005f;//人口每次换季时增长，默认0.05，也就是每次换季人口增长5%,改成每旬涨的话，就是0.005f
const int 伏兵部队数 = 4;
const int 疑兵基础概率 = 3;
const int 减少疑兵概率智力下限 = 90;//攻击方
const int 增加疑兵概率智力下限 = 70;//被攻击方
// 资源生产相关
const int 玩家金收入倍率 = 170;//容易 120，普通100，史诗80，传说60
const int 电脑金收入倍率 = 200;//容易 120，普通130，史诗140，传说150
const int 玩家粮收入倍率 = 170;//容易 130，普通100，史诗80，传说60
const int 电脑粮收入倍率 = 200;//容易 120，普通130，史诗140，传说150
const bool 造币谷仓无需相邻 = true;
const bool 造币对鱼市大市生效 = true;
const bool 谷仓对军屯农生效 = true;
const int 军屯农单倍收入人数 = 15000; // 每产生一倍1级军粮收入所需的军队人数

const bool 港关使用固定收入 = true;
const int 港关固定金收入 = 1000;
const int 港关固定粮收入 = 12000;
const int 港关金收入倍率 = 150;  //相对原版收入
const bool 港口金收入加成 = true;
const int 港关粮收入倍率 = 150;  //相对原版收入
const bool 关隘粮收入加成 = true;

// 内政生产相关
const int 大都市征兵倍率 = 120;//开启人口时取消该加成
const int 一般城市征兵倍率 = 100;
const int 名声征兵加成 = 50;
const int 超级电脑征兵倍率 = 200;
const float 强募募兵系数 = 2.0;

const int 制造数量倍率 = 100;
const int 军马数量倍率 = 100;
const int 能吏生产加成 = 50;
const int 繁殖生产加成 = 50;
const int 超级电脑生产倍率 = 200;

const int 周围敌部队征兵倍率 = 50;
const int 生产时长倍率 = 100;
const int 发明造船倍率 = 50;
const int 生产时长上限 = 10;

const int 调练效果倍率 = 150;
const int 巡查效果倍率 = 150;

//  伤害倍率相关
const int 火神伤害倍率 = 200;
const int 火攻伤害倍率 = 150;
const int 火神火伤倍率 = 0;
const int 火攻火伤倍率 = 50;
const int 藤甲火伤倍率 = 200;
const int 藤甲受伤倍率 = 50;
const int 踏破陷伤倍率 = 10;

const int 拱戍攻防倍率 = 150;
const int 要击伤害倍率 = 150;
const int 兵圣受伤倍率 = 50;
const float 战神暴击系数 = 0.40; //1.0表示伤害加100%
const int 坚城受伤倍率 = 50;//60表示伤害为原伤害的60%
const int 陷阵伤害倍率 = 150;

const int 蓄势减少消耗 = 20;
const int 蓄势增加伤害 = 20;
const int 摧锋破甲比例 = 40;
const int 重甲防御提升量 = 30;
const int 重甲移动力减少 = 4;
const int 轻甲攻防提升比例 = 25;
const float 狼骑伤害系数 = 0.20;
const int 重器防御提升比例 = 40;
const int 水师减少伤害比例 = 30;

// 概率相关
const int 狙击击伤概率 = 20;
const int 猛骑击伤概率 = 20;
const int 猛卒击伤概率 = 20;
const int 韬略成功率上升 = 20;
const int 谋略成功率上升 = 20;
const int 军略成功率上升 = 20;
const int 八卦发动几率 = 50;
const int 内治叛乱几率 = 10;
const int 运筹触发几率 = 50;
const int 激励触发几率 = 50;
const int 军魂效果倍率 = 200;
const int 辎粮耗粮倍率 = 50;


// 其他 
const int 昂扬攻击加气 = 5;
const int 破竹气力减少 = 30;
const int 贤哲增加技巧 = 30;
const int 窥探视野增加 = 2;
const int 窥探调查成功率提升 = 15;
const float 劫财系数 = 3.0;
const float 截粮系数 = 3.0;



// 光环系统
const bool 基础光环 = true;//是否打开
const bool 战法暴击光环 = true;//是否打开
const bool 暴击伤害光环 = true;//是否打开
const bool 计策成功率光环 = true;//是否打开
const bool 计策识破率光环 = true;//是否打开
const bool 辅助攻击光环 = true;//是否打开
const bool 气力回复光环 = true;//是否打开

// 技巧设定 ----------------------------------------------------------------
const int 矢盾发动机率 = 30;
const int 大盾发动机率 = 30;



array<array<int>> 特技固定特技数组 =
{};

/*	    参考禁止拆建	
		array<uint32> enabled_(势力_末, uint64(-1));//长度为46的素组，每个元素初始化为-1
		// 下面这个表明有多少个uint32
		array<array<int>> min_(도시_끝, array<int>(시설_끝, -2));*/


//array<array<uint8>> ai异族请求信息(非贼势力_末, { /*请求方据点id*/-1, /*目标城市id*/-1, /*异族势力id*/势力_盗贼, /*多少旬后出兵*/-1 });


//目前数量221
const array<array<int>> 特殊地名设施 =
{
	/*金城*/ {/*设施类型*/设施_阵, /*x*/1, /*y*/48},
	/*陇西*/ {/*设施类型*/设施_阵, /*x*/4, /*y*/65},
	/*武都*/ {/*设施类型*/设施_阵, /*x*/8, /*y*/79},
	/*阴平*/ {/*设施类型*/设施_阵, /*x*/2, /*y*/91},
	/*五丈原*/ {/*设施类型*/设施_箭楼, /*x*/35, /*y*/71},
	/*涪陵*/ {/*设施类型*/设施_阵, /*x*/36, /*y*/156},
	/*牂牁*/ {/*设施类型*/设施_阵, /*x*/34, /*y*/169},
	/*华容*/ {/*设施类型*/设施_箭楼, /*x*/92, /*y*/134},
	/*河东*/ {/*设施类型*/设施_阵, /*x*/47, /*y*/50},
	/*乌巢*/ {/*设施类型*/设施_箭楼, /*x*/118, /*y*/68},
	/*野王*/ {/*设施类型*/设施_箭楼, /*x*/74, /*y*/59},
	/*易京*/ {/*设施类型*/设施_箭楼, /*x*/136, /*y*/26},
	/*雍丘*/ {/*设施类型*/设施_箭楼, /*x*/113, /*y*/85},
	/*陈*/ {/*设施类型*/设施_阵, /*x*/108, /*y*/100},
	/*临淄*/ {/*设施类型*/设施_阵, /*x*/160, /*y*/57},
	/*琅邪*/ {/*设施类型*/设施_阵, /*x*/158, /*y*/70},
	/*石亭*/ {/*设施类型*/设施_箭楼, /*x*/143, /*y*/111},
	/*巨鹿*/ {/*设施类型*/设施_阵, /*x*/112, /*y*/41},
	/*涪县*/ {/*设施类型*/设施_箭楼, /*x*/25, /*y*/117},
	/*巴邱*/ {/*设施类型*/设施_箭楼, /*x*/105, /*y*/152},
	/*彭城*/ {/*设施类型*/设施_阵, /*x*/148, /*y*/79},
	/*襄垣*/ {/*设施类型*/设施_箭楼, /*x*/78, /*y*/52},
	/*丸都*/ {/*设施类型*/设施_箭楼, /*x*/184, /*y*/5},
	/*昌黎*/ {/*设施类型*/设施_阵, /*x*/153, /*y*/2},
	/*渔阳*/ {/*设施类型*/设施_阵, /*x*/127, /*y*/5},
	/*涿*/ {/*设施类型*/设施_阵, /*x*/115, /*y*/26},
	/*河间*/ {/*设施类型*/设施_阵, /*x*/130, /*y*/31},
	/*乐陵*/ {/*设施类型*/设施_阵, /*x*/140, /*y*/44},
	/*清河*/ {/*设施类型*/设施_阵, /*x*/130, /*y*/47},
	/*雁门*/ {/*设施类型*/设施_阵, /*x*/90, /*y*/20},
	/*太原*/ {/*设施类型*/设施_阵, /*x*/84, /*y*/26},
	/*长子*/ {/*设施类型*/设施_箭楼, /*x*/90, /*y*/51},
	/*东莱*/ {/*设施类型*/设施_阵, /*x*/169, /*y*/50},
	/*即墨*/ {/*设施类型*/设施_箭楼, /*x*/164, /*y*/47},
	/*东武*/ {/*设施类型*/设施_箭楼, /*x*/157, /*y*/60},
	/*东阿*/ {/*设施类型*/设施_箭楼, /*x*/131, /*y*/67},
	/*定陶*/ {/*设施类型*/设施_箭楼, /*x*/124, /*y*/80},
	/*中牟*/ {/*设施类型*/设施_箭楼, /*x*/93, /*y*/83},
	/*颍川*/ {/*设施类型*/设施_阵, /*x*/92, /*y*/94},
	/*宋*/ {/*设施类型*/设施_阵, /*x*/122, /*y*/100},
	/*阳安*/ {/*设施类型*/设施_箭楼, /*x*/104, /*y*/101},
	/*朝歌*/ {/*设施类型*/设施_箭楼, /*x*/80, /*y*/62},
	/*平阳*/ {/*设施类型*/设施_阵, /*x*/67, /*y*/39},
	/*河南*/ {/*设施类型*/设施_阵, /*x*/75, /*y*/78},
	/*商*/ {/*设施类型*/设施_箭楼, /*x*/66, /*y*/85},
	/*郿*/ {/*设施类型*/设施_箭楼, /*x*/43, /*y*/74},
	/*赤岸*/ {/*设施类型*/设施_箭楼, /*x*/24, /*y*/77},
	/*榆中*/ {/*设施类型*/设施_箭楼, /*x*/10, /*y*/53},
	/*令居*/ {/*设施类型*/设施_箭楼, /*x*/19, /*y*/37},
	/*张掖*/ {/*设施类型*/设施_阵, /*x*/6, /*y*/33},
	/*酒泉*/ {/*设施类型*/设施_阵, /*x*/11, /*y*/28},
	/*寻阳*/ {/*设施类型*/设施_箭楼, /*x*/146, /*y*/118},
	/*阜陵*/ {/*设施类型*/设施_箭楼, /*x*/146, /*y*/97},
	/*丹阳*/ {/*设施类型*/设施_阵, /*x*/164, /*y*/124},
	/*武昌*/ {/*设施类型*/设施_阵, /*x*/109, /*y*/143},
	/*南昌*/ {/*设施类型*/设施_阵, /*x*/115, /*y*/141},
	/*穰县*/ {/*设施类型*/设施_箭楼, /*x*/68, /*y*/99},
	/*新城*/ {/*设施类型*/设施_阵, /*x*/68, /*y*/118},
	/*宜都*/ {/*设施类型*/设施_阵, /*x*/77, /*y*/121},
	/*安陆*/ {/*设施类型*/设施_箭楼, /*x*/110, /*y*/118},
	/*衡阳*/ {/*设施类型*/设施_箭楼, /*x*/80, /*y*/164},
	/*魏兴*/ {/*设施类型*/设施_阵, /*x*/57, /*y*/98},
	/*昆阳*/ {/*设施类型*/设施_箭楼, /*x*/99, /*y*/96},
	/*益阳*/ {/*设施类型*/设施_箭楼, /*x*/74, /*y*/161},
	/*安阳*/ {/*设施类型*/设施_箭楼, /*x*/47, /*y*/103},
	/*汉寿*/ {/*设施类型*/设施_箭楼, /*x*/27, /*y*/107},
	/*汉昌*/ {/*设施类型*/设施_箭楼, /*x*/36, /*y*/111},
	/*江油*/ {/*设施类型*/设施_箭楼, /*x*/14, /*y*/104},
	/*都安*/ {/*设施类型*/设施_箭楼, /*x*/1, /*y*/118},
	/*广汉*/ {/*设施类型*/设施_阵, /*x*/22, /*y*/132},
	/*临江*/ {/*设施类型*/设施_箭楼, /*x*/34, /*y*/136},
	/*江阳*/ {/*设施类型*/设施_阵, /*x*/19, /*y*/145},
	/*朱提*/ {/*设施类型*/设施_阵, /*x*/23, /*y*/160},
	/*且兰*/ {/*设施类型*/设施_箭楼, /*x*/51, /*y*/167},
	/*宛温*/ {/*设施类型*/设施_箭楼, /*x*/37, /*y*/185},
	/*台登*/ {/*设施类型*/设施_箭楼, /*x*/5, /*y*/144},
	/*章陵*/ {/*设施类型*/设施_箭楼, /*x*/105, /*y*/116},
	/*舞阴*/ {/*设施类型*/设施_箭楼, /*x*/98, /*y*/105},
	/*犍为*/ {/*设施类型*/设施_阵, /*x*/11, /*y*/139},
	/*义阳*/ {/*设施类型*/设施_阵, /*x*/93, /*y*/107},
	/*六安*/ {/*设施类型*/设施_箭楼, /*x*/135, /*y*/109},
	/*匡亭*/ {/*设施类型*/设施_箭楼, /*x*/109, /*y*/70},
	/*封丘*/ {/*设施类型*/设施_箭楼, /*x*/106, /*y*/74},
	/*耒阳*/ {/*设施类型*/设施_箭楼, /*x*/106, /*y*/180},
	/*临武*/ {/*设施类型*/设施_箭楼, /*x*/87, /*y*/186},
	/*建平*/ {/*设施类型*/设施_阵, /*x*/47, /*y*/133},
	/*邯郸*/ {/*设施类型*/设施_箭楼, /*x*/107, /*y*/48},
	/*庐陵*/ {/*设施类型*/设施_阵, /*x*/127, /*y*/180},
	/*戈阳*/ {/*设施类型*/设施_阵, /*x*/128, /*y*/104},
	/*梁*/ {/*设施类型*/设施_阵, /*x*/123, /*y*/87},
	/*武功*/ {/*设施类型*/设施_箭楼, /*x*/46, /*y*/68},
	/*荣阳*/ {/*设施类型*/设施_阵, /*x*/90, /*y*/76},
	/*上圭*/ {/*设施类型*/设施_箭楼, /*x*/19, /*y*/77},
	/*汉嘉*/ {/*设施类型*/设施_阵, /*x*/0, /*y*/137},
	/*扶风*/ {/*设施类型*/设施_阵, /*x*/54, /*y*/80},
	/*冯翔*/ {/*设施类型*/设施_阵, /*x*/49, /*y*/71},
	/*新平*/ {/*设施类型*/设施_阵, /*x*/36, /*y*/40},
	/*阴盘*/ {/*设施类型*/设施_箭楼, /*x*/29, /*y*/61},
	/*槐里*/ {/*设施类型*/设施_箭楼, /*x*/41, /*y*/69},
	/*阿阳*/ {/*设施类型*/设施_箭楼, /*x*/10, /*y*/60},
	/*乌枝*/ {/*设施类型*/设施_箭楼, /*x*/20, /*y*/55},
	/*滇池*/ {/*设施类型*/设施_阵, /*x*/17, /*y*/188},
	/*蕲春*/ {/*设施类型*/设施_箭楼, /*x*/122, /*y*/112},
	/*临沮*/ {/*设施类型*/设施_箭楼, /*x*/62, /*y*/118},
	/*秭归*/ {/*设施类型*/设施_箭楼, /*x*/60, /*y*/122},
	/*辰阳*/ {/*设施类型*/设施_箭楼, /*x*/66, /*y*/164},
	/*沅陵*/ {/*设施类型*/设施_箭楼, /*x*/75, /*y*/167},
	/*烝阳*/ {/*设施类型*/设施_箭楼, /*x*/76, /*y*/174},
	/*充*/ {/*设施类型*/设施_箭楼, /*x*/61, /*y*/151},
	/*始安*/ {/*设施类型*/设施_箭楼, /*x*/58, /*y*/182},
	/*都梁*/ {/*设施类型*/设施_箭楼, /*x*/61, /*y*/170},
	/*临烝*/ {/*设施类型*/设施_箭楼, /*x*/80, /*y*/186},
	/*汉宁*/ {/*设施类型*/设施_箭楼, /*x*/118, /*y*/181},
	/*曲汉*/ {/*设施类型*/设施_箭楼, /*x*/116, /*y*/193},
	/*临贺*/ {/*设施类型*/设施_阵, /*x*/104, /*y*/194},
	/*临湘*/ {/*设施类型*/设施_箭楼, /*x*/106, /*y*/171},
	/*南城*/ {/*设施类型*/设施_箭楼, /*x*/123, /*y*/174},
	/*云社*/ {/*设施类型*/设施_箭楼, /*x*/101, /*y*/112},
	/*溧阳*/ {/*设施类型*/设施_箭楼, /*x*/184, /*y*/125},
	/*句容*/ {/*设施类型*/设施_箭楼, /*x*/190, /*y*/122},
	/*海盐*/ {/*设施类型*/设施_箭楼, /*x*/157, /*y*/138},
	/*钱唐*/ {/*设施类型*/设施_箭楼, /*x*/155, /*y*/145},
	/*始新*/ {/*设施类型*/设施_箭楼, /*x*/151, /*y*/134},
	/*黟*/ {/*设施类型*/设施_箭楼, /*x*/161, /*y*/142},
	/*富春*/ {/*设施类型*/设施_箭楼, /*x*/147, /*y*/149},
	/*勤*/ {/*设施类型*/设施_箭楼, /*x*/155, /*y*/160},
	/*永宁*/ {/*设施类型*/设施_箭楼, /*x*/146, /*y*/165},
	/*太末*/ {/*设施类型*/设施_箭楼, /*x*/140, /*y*/159},
	/*育阳*/ {/*设施类型*/设施_箭楼, /*x*/79, /*y*/102},
	/*夏丘*/ {/*设施类型*/设施_箭楼, /*x*/156, /*y*/86},
	/*盱台*/ {/*设施类型*/设施_箭楼, /*x*/160, /*y*/81},
	/*郯*/ {/*设施类型*/设施_箭楼, /*x*/164, /*y*/71},
	/*东莞*/ {/*设施类型*/设施_阵, /*x*/152, /*y*/63},
	/*离狐*/ {/*设施类型*/设施_箭楼, /*x*/119, /*y*/75},
	/*燕*/ {/*设施类型*/设施_阵, /*x*/122, /*y*/72},
	/*鄄城*/ {/*设施类型*/设施_箭楼, /*x*/136, /*y*/69},
	/*昌邑*/ {/*设施类型*/设施_箭楼, /*x*/134, /*y*/77},
	/*鲁*/ {/*设施类型*/设施_阵, /*x*/146, /*y*/74},
	/*萧*/ {/*设施类型*/设施_箭楼, /*x*/141, /*y*/82},
	/*砀*/ {/*设施类型*/设施_箭楼, /*x*/136, /*y*/84},
	/*鄢陵*/ {/*设施类型*/设施_箭楼, /*x*/103, /*y*/87},
	/*酸枣*/ {/*设施类型*/设施_箭楼, /*x*/95, /*y*/76},
	/*尉氏*/ {/*设施类型*/设施_箭楼, /*x*/99, /*y*/81},
	/*围*/ {/*设施类型*/设施_箭楼, /*x*/107, /*y*/85},
	/*长社*/ {/*设施类型*/设施_箭楼, /*x*/93, /*y*/89},
	/*阳夏*/ {/*设施类型*/设施_箭楼, /*x*/112, /*y*/92},
	/*新汲*/ {/*设施类型*/设施_箭楼, /*x*/104, /*y*/96},
	/*阳翟*/ {/*设施类型*/设施_箭楼, /*x*/90, /*y*/87},
	/*召陵*/ {/*设施类型*/设施_箭楼, /*x*/109, /*y*/103},
	/*汝阴*/ {/*设施类型*/设施_阵, /*x*/118, /*y*/104},
	/*上蔡*/ {/*设施类型*/设施_箭楼, /*x*/104, /*y*/108},
	/*朗陵*/ {/*设施类型*/设施_箭楼, /*x*/115, /*y*/111},
	/*鲁阳*/ {/*设施类型*/设施_箭楼, /*x*/85, /*y*/88},
	/*顺阳*/ {/*设施类型*/设施_箭楼, /*x*/73, /*y*/91},
	/*棘阳*/ {/*设施类型*/设施_箭楼, /*x*/77, /*y*/97},
	/*筑阳*/ {/*设施类型*/设施_箭楼, /*x*/62, /*y*/99},
	/*锡*/ {/*设施类型*/设施_箭楼, /*x*/47, /*y*/98},
	/*汉丰*/ {/*设施类型*/设施_箭楼, /*x*/50, /*y*/126},
	/*宕渠*/ {/*设施类型*/设施_箭楼, /*x*/41, /*y*/125},
	/*固陵*/ {/*设施类型*/设施_箭楼, /*x*/41, /*y*/132},
	/*北井*/ {/*设施类型*/设施_箭楼, /*x*/54, /*y*/123},
	/*钟离*/ {/*设施类型*/设施_箭楼, /*x*/137, /*y*/88},
	/*当涂*/ {/*设施类型*/设施_箭楼, /*x*/141, /*y*/90},
	/*下蔡*/ {/*设施类型*/设施_箭楼, /*x*/133, /*y*/95},
	/*安丰*/ {/*设施类型*/设施_阵, /*x*/130, /*y*/100},
	/*成皋*/ {/*设施类型*/设施_箭楼, /*x*/82, /*y*/72},
	/*新郑*/ {/*设施类型*/设施_箭楼, /*x*/84, /*y*/79},
	/*侯氏*/ {/*设施类型*/设施_箭楼, /*x*/79, /*y*/79},
	/*平阴*/ {/*设施类型*/设施_箭楼, /*x*/74, /*y*/74},
	/*漆*/ {/*设施类型*/设施_箭楼, /*x*/37, /*y*/59},
	/*渝糜*/ {/*设施类型*/设施_箭楼, /*x*/24, /*y*/66},
	/*下辨*/ {/*设施类型*/设施_箭楼, /*x*/14, /*y*/75},
	/*兴势*/ {/*设施类型*/设施_箭楼, /*x*/35, /*y*/85},
	/*蓝田*/ {/*设施类型*/设施_箭楼, /*x*/49, /*y*/77},
	/*三水*/ {/*设施类型*/设施_箭楼, /*x*/24, /*y*/43},
	/*休屠*/ {/*设施类型*/设施_箭楼, /*x*/1, /*y*/21},
	/*仓松*/ {/*设施类型*/设施_箭楼, /*x*/9, /*y*/46},
	/*祖厉*/ {/*设施类型*/设施_箭楼, /*x*/15, /*y*/47},
	/*褒中*/ {/*设施类型*/设施_箭楼, /*x*/31, /*y*/83},
	/*成固*/ {/*设施类型*/设施_箭楼, /*x*/22, /*y*/90},
	/*安阳*/ {/*设施类型*/设施_箭楼, /*x*/32, /*y*/91},
	/*德阳*/ {/*设施类型*/设施_箭楼, /*x*/28, /*y*/130},
	/*焚道*/ {/*设施类型*/设施_箭楼, /*x*/5, /*y*/151},
	/*资中*/ {/*设施类型*/设施_箭楼, /*x*/12, /*y*/125},
	/*汉安*/ {/*设施类型*/设施_箭楼, /*x*/17, /*y*/129},
	/*武阳*/ {/*设施类型*/设施_箭楼, /*x*/4, /*y*/132},
	/*南安*/ {/*设施类型*/设施_箭楼, /*x*/0, /*y*/142},
	/*垫江*/ {/*设施类型*/设施_箭楼, /*x*/19, /*y*/136},
	/*符节*/ {/*设施类型*/设施_箭楼, /*x*/26, /*y*/140},
	/*会无*/ {/*设施类型*/设施_箭楼, /*x*/4, /*y*/169},
	/*平夷*/ {/*设施类型*/设施_箭楼, /*x*/22, /*y*/168},
	/*姑复*/ {/*设施类型*/设施_箭楼, /*x*/6, /*y*/178},
	/*弄栋*/ {/*设施类型*/设施_箭楼, /*x*/10, /*y*/183},
	/*双柏*/ {/*设施类型*/设施_箭楼, /*x*/23, /*y*/195},
	/*贲古*/ {/*设施类型*/设施_箭楼, /*x*/53, /*y*/195},
	/*安邑*/ {/*设施类型*/设施_箭楼, /*x*/66, /*y*/52},
	/*界休*/ {/*设施类型*/设施_箭楼, /*x*/91, /*y*/57},
	/*云中*/ {/*设施类型*/设施_阵, /*x*/81, /*y*/34},
	/*雍奴*/ {/*设施类型*/设施_箭楼, /*x*/131, /*y*/21},
	/*中山*/ {/*设施类型*/设施_阵, /*x*/116, /*y*/31},
	/*高阳*/ {/*设施类型*/设施_阵, /*x*/125, /*y*/26},
	/*婴陶*/ {/*设施类型*/设施_箭楼, /*x*/111, /*y*/34},
	/*房子*/ {/*设施类型*/设施_箭楼, /*x*/103, /*y*/43},
	/*荡阴*/ {/*设施类型*/设施_箭楼, /*x*/109, /*y*/58},
	/*阴安*/ {/*设施类型*/设施_箭楼, /*x*/117, /*y*/60},
	/*曲梁*/ {/*设施类型*/设施_箭楼, /*x*/114, /*y*/52},
	/*博平*/ {/*设施类型*/设施_箭楼, /*x*/125, /*y*/50},
	/*乐平*/ {/*设施类型*/设施_阵, /*x*/127, /*y*/59},
	/*西平昌*/ {/*设施类型*/设施_箭楼, /*x*/138, /*y*/48},
	/*东光*/ {/*设施类型*/设施_箭楼, /*x*/134, /*y*/41},
	/*信都*/ {/*设施类型*/设施_箭楼, /*x*/127, /*y*/40},
	/*豫章*/ {/*设施类型*/设施_阵, /*x*/122, /*y*/188},
	/*乐成*/ {/*设施类型*/设施_箭楼, /*x*/134, /*y*/34},
	/*章武*/ {/*设施类型*/设施_箭楼, /*x*/140, /*y*/31},
	/*徐无*/ {/*设施类型*/设施_箭楼, /*x*/136, /*y*/12},
	/*土垠*/ {/*设施类型*/设施_箭楼, /*x*/142, /*y*/22},
	/*令支*/ {/*设施类型*/设施_箭楼, /*x*/153, /*y*/12},
	/*辽阳*/ {/*设施类型*/设施_箭楼, /*x*/172, /*y*/3},
	/*三江口*/ {/*设施类型*/设施_箭楼, /*x*/123, /*y*/128},
	/*吴兴*/ {/*设施类型*/设施_阵, /*x*/140, /*y*/171},
	/*阳信*/ {/*设施类型*/设施_箭楼, /*x*/142, /*y*/39}
};

const array<string>特殊地名设施名称 =
{
	"金城",
	"陇西",
	"武都",
	"阴平",
	"五丈原",
	"涪陵",
	"牂牁",
	"华容",
	"河东",
	"乌巢",
	"野王",
	"易京",
	"雍丘",
	"陈",
	"临淄",
	"琅邪",
	"石亭",
	"巨鹿",
	"涪县",
	"巴邱",
	"彭城",
	"襄垣",
	"丸都",
	"昌黎",
	"渔阳",
	"涿",
	"河间",
	"乐陵",
	"清河",
	"雁门",
	"太原",
	"长子",
	"东莱",
	"即墨",
	"东武",
	"东阿",
	"定陶",
	"中牟",
	"颍川",
	"宋",
	"阳安",
	"朝歌",
	"平阳",
	"河南",
	"商",
	"郿",
	"赤岸",
	"榆中",
	"令居",
	"张掖",
	"酒泉",
	"寻阳",
	"阜陵",
	"丹阳",
	"武昌",
	"南昌",
	"穰县",
	"新城",
	"宜都",
	"安陆",
	"衡阳",
	"魏兴",
	"昆阳",
	"益阳",
	"安阳",
	"汉寿",
	"汉昌",
	"江油",
	"都安",
	"广汉",
	"临江",
	"江阳",
	"朱提",
	"且兰",
	"宛温",
	"台登",
	"章陵",
	"舞阴",
	"犍为",
	"义阳",
	"六安",
	"匡亭",
	"封丘",
	"耒阳",
	"临武",
	"建平",
	"邯郸",
	"庐陵",
	"戈阳",
	"梁",
	"武功",
	"荣阳",
	"上圭",
	"汉嘉",
	"扶风",
	"冯翔",
	"新平",
	"阴盘",
	"槐里",
	"阿阳",
	"乌枝",
	"滇池",
	"蕲春",
	"临沮",
	"秭归",
	"辰阳",
	"沅陵",
	"烝阳",
	"充",
	"始安",
	"都梁",
	"临烝",
	"汉宁",
	"曲汉",
	"临贺",
	"临湘",
	"南城",
	"云社",
	"溧阳",
	"句容",
	"海盐",
	"钱唐",
	"始新",
	"黟",
	"富春",
	"勤",
	"永宁",
	"太末",
	"育阳",
	"夏丘",
	"盱台",
	"郯",
	"东莞",
	"离狐",
	"燕",
	"鄄城",
	"昌邑",
	"鲁",
	"萧",
	"砀",
	"鄢陵",
	"酸枣",
	"尉氏",
	"围",
	"长社",
	"阳夏",
	"新汲",
	"阳翟",
	"召陵",
	"汝阴",
	"上蔡",
	"朗陵",
	"鲁阳",
	"顺阳",
	"棘阳",
	"筑阳",
	"锡",
	"汉丰",
	"宕渠",
	"固陵",
	"北井",
	"钟离",
	"当涂",
	"下蔡",
	"安丰",
	"成皋",
	"新郑",
	"侯氏",
	"平阴",
	"漆",
	"渝糜",
	"下辨",
	"兴势",
	"蓝田",
	"三水",
	"休屠",
	"仓松",
	"祖厉",
	"褒中",
	"成固",
	"安阳",
	"德阳",
	"焚道",
	"资中",
	"汉安",
	"武阳",
	"南安",
	"垫江",
	"符节",
	"会无",
	"平夷",
	"姑复",
	"弄栋",
	"双柏",
	"贲古",
	"安邑",
	"界休",
	"云中",
	"雍奴",
	"中山",
	"高阳",
	"婴陶",
	"房子",
	"荡阴",
	"阴安",
	"曲梁",
	"博平",
	"乐平",
	"西平昌",
	"东光",
	"信都",
	"豫章",
	"乐成",
	"章武",
	"徐无",
	"土垠",
	"令支",
	"辽阳",
	"三江口",
	"吴兴",
	"阳信"
};




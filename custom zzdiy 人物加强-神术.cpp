/***CUSTOM
@name zzdiy 人物加强-神术
@class 特技
@author 黑店小小二
@contact 813582133
@version 2.0
@date 2022/03/30
@desc pk2.2 版本要求，至少为2.4.220319！！
@notice
***/
/*
作者：黑店小小二
pk2.2 版本要求，至少为2.4.220319！！
神术效果具体描述 https://docs.qq.com/sheet/DWGpPdVpWak9SV3RH?tab=BB08J2

v2.0 2022/03/21 庞统新增神术【火凤连环】, 贾诩新增神术【乱武完杀】, 法正新增神术【奇谋诡策】,
                荀攸新增神术【帷幄奇策】, 陆逊新增神术【无间业火】，荀彧新增神术【握筹天下】
v1.9 2022/03/18 司马懿新增神术【狼顾权变】，修复郭嘉神术bug
v1.8 2022/03/01 诸葛亮新增神术【神鬼八阵】
v1.7 2022/02/28 袁术新增神术【妄尊仲帝】
v1.6 2022/02/25 郭嘉新增神术【神鬼奇谋】
v1.5 2022/02/22 调整写法，优化代码体积
v1.4 2022/02/19 修复【黄天泰平】劝降君主，都督后的处理
v1.3 2022/02/17 修复对城池耐久归0归属变更bug，张角新增神术【黄天泰平】,调整【神雷灭世】动画效果，曹操新增神术【天下归心】
v1.2 2022/02/14 修复措施耐久归0归属变更bug
v1.1 2022/02/13 修复周瑜神术对关卡跟港口无效bug，张角新增神术【神雷灭世】
v1.0 2022/02/12 新增周瑜神术【业火焚天】

描述：针对高智武将，设置了大招【神术】，旨在增加游戏娱乐性
 */

namespace 人物加强 {

  const int 计略气力消耗_通用 = 60;
  const int 兵力条件_通用 = 5000;

  const int 计略气力消耗_黄天泰平 = 100;
  const int 计略气力消耗_乱武完杀 = 100;

  const int 业火焚天_计略范围 = 3;
  const int 业火焚天_效果范围 = 11;

  const int 神雷灭世_计略范围 = 3;
  const int 神雷灭世_效果范围 = 9;

  const int 黄天泰平_计略范围 = 3;
  const int 黄天泰平_效果范围 = 3;

  const int 天下归心_计略范围 = 3;
  const int 天下归心_效果范围 = 3;

  const int 神鬼奇谋_已方数量 = 10;
  const int 神鬼奇谋_敌方数量 = 10;
  const bool 神鬼奇谋_盟军影响 = false;

  const int 神鬼八阵_效果范围 = 3;

  const int 狼顾权变_效果范围 = 3;
  const int 狼顾权变_技能失效时长 = 3;
  const bool 狼顾权变_盟军影响 = true;

  const int 火凤连环_计略范围 = 3;
  const int 火凤连环_效果范围 = 3;
  const bool 火凤连环_盟军影响 = true;

  const int 乱武完杀_分队数量 = 5;
  const int 乱武完杀_分队兵力 = 100;
  const bool 乱武完杀_盟军影响 = true;

  const int 奇谋诡策_计略范围 = 3;
  const int 奇谋诡策_效果范围 = 3;

  const int 帷幄奇策_计略范围 = 3;
  const int 帷幄奇策_效果范围 = 3;

  const int 无间业火_计略范围 = 3;
  const int 无间业火_火伤倍率 = 10;

  const int 握筹天下_计略范围 = 3;
  const int 握筹天下_效果范围 = 3;
  const bool 握筹天下_盟军影响 = true;

  pk::func209_t@ prev_callback_209;
  pk::func58_t@ prev_callback_58;
  pk::func205_t@ prev_callback_205;
  pk::func53_t@ prev_callback_53;
  pk::func202_t@ prev_callback_202;
  pk::func214_t@ prev_callback_214;
  pk::func169_t@ prev_callback_169;
  pk::func220_t@ prev_callback_220;

  pk::unit@ src_unit;
  pk::point src_pos_;
  int leader_id;

  class Main
  {
    pk::random random(pk::rand());

    int 临时武将起始 = 780;
    int 临时武将终止 = 799;
    dictionary 无间业火_影响部队 = {};
    array<array<int>> 握筹天下_影响部队;
    array<int> 握筹天下_效果部队;

    Main() {
      add_menu_unit_order();

      @prev_callback_53 = cast<pk::func53_t@>(pk::get_func(53));
      pk::reset_func(53);
      pk::set_func(53, pk::func53_t(func53));

      @prev_callback_58 = cast<pk::func58_t@>(pk::get_func(58));
      pk::reset_func(58);
      pk::set_func(58, pk::func58_t(func58));

      @prev_callback_169 = cast<pk::func169_t@>(pk::get_func(169));
      pk::reset_func(169);
      pk::set_func(169, pk::func169_t(func169));

      @prev_callback_202 = cast<pk::func202_t@>(pk::get_func(202));
      pk::reset_func(202);
      pk::set_func(202, pk::func202_t(func202));

      @prev_callback_205 = cast<pk::func205_t@>(pk::get_func(205));
      pk::reset_func(205);
      pk::set_func(205, pk::func205_t(func205));

      @prev_callback_209 = cast<pk::func209_t@>(pk::get_func(209));
      pk::reset_func(209);
      pk::set_func(209, pk::func209_t(func209));

      @prev_callback_214 = cast<pk::func214_t@>(pk::get_func(214));
      pk::reset_func(214);
      pk::set_func(214, pk::func214_t(func214));

      @prev_callback_220 = cast<pk::func220_t@>(pk::get_func(220));
      pk::reset_func(220);
      pk::set_func(220, pk::func220_t(func220));

      pk::bind(108, pk::trigger108_t(onMonthStart));
      pk::bind(111, pk::trigger111_t(onTurnStart));
      pk::bind(112, pk::trigger112_t(onTurnEnd));
      pk::bind(171, pk::trigger171_t(onUnitRemove));
      pk::bind(173, pk::trigger173_t(onUnitExpAdd));
      pk::bind(174, pk::trigger174_t(onUnitActionDone));
      pk::bind(175, pk::trigger175_t(onUnitDestroy));
      pk::bind(179, pk::trigger179_t(onUnitAfterAttack));
    }


    pk::person@ src_leader;
    pk::person@ person_周瑜;
    pk::person@ person_张角;
    pk::person@ person_张宝;
    pk::person@ person_张梁;
    pk::person@ person_曹操;
    pk::person@ person_郭嘉;
    pk::person@ person_袁术;
    pk::person@ person_诸葛亮;
    pk::person@ person_司马懿;
    pk::person@ person_庞统;
    pk::person@ person_贾诩;
    pk::person@ person_法正;
    pk::person@ person_荀攸;
    pk::person@ person_陆逊;
    pk::person@ person_荀彧;
    pk::force@ src_force;

    bool 开启_业火焚天 = true; // 周瑜大招
    bool 开启_神雷灭世 = true; // 张角大招
    bool 开启_黄天泰平 = true; // 张角三兄弟大招
    bool 开启_天下归心 = true; // 曹操大招
    bool 开启_神鬼奇谋 = true; // 郭嘉大招
    bool 开启_妄尊仲帝 = true; // 袁术大招
    bool 开启_神鬼八阵 = true; // 诸葛亮大招
    bool 开启_狼顾权变 = true; // 司马懿大招
    bool 开启_火凤连环 = true; // 庞统大招
    bool 开启_乱武完杀 = true; // 贾诩大招
    bool 开启_奇谋诡策 = true; // 法正大招
    bool 开启_帷幄奇策 = true; // 荀攸大招
    bool 开启_无间业火 = true; // 陆逊大招
    bool 开启_握筹天下 = true; // 荀彧大招

    bool 妄尊仲帝_启用状态 = true;
    bool 神鬼八阵_启用状态 = true;

    void add_menu_unit_order()
    {
      if (开启_业火焚天) {
        pk::menu_item 神术_业火焚天;
        神术_业火焚天.menu = 神术数据结构体::菜单_神术;
        神术_业火焚天.init = pk::unit_menu_item_init_t(init);
        神术_业火焚天.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称('业火焚天', 计略气力消耗_通用); });
        神术_业火焚天.get_desc = pk::menu_item_get_desc_t(getDesc_神术_业火焚天);
        神术_业火焚天.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称(武将_周瑜, leader_id); });
        神术_业火焚天.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_业火焚天);
        神术_业火焚天.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return 神术数据结构体::getTargets_神术_目标(业火焚天_计略范围, src_pos_); });
        神术_业火焚天.handler = pk::unit_menu_item_handler_t(handler_神术_业火焚天);
        pk::add_menu_item(神术_业火焚天);
      }
      if (开启_神雷灭世) {
        pk::menu_item 神术_神雷灭世;
        神术_神雷灭世.menu = 神术数据结构体::菜单_神术;
        神术_神雷灭世.init = pk::unit_menu_item_init_t(init);
        神术_神雷灭世.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称("神雷灭世", 计略气力消耗_通用); });
        神术_神雷灭世.get_desc = pk::menu_item_get_desc_t(getDesc_神术_神雷灭世);
        神术_神雷灭世.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称(武将_张角, leader_id); });
        神术_神雷灭世.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_神雷灭世);
        神术_神雷灭世.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return 神术数据结构体::getTargets_神术_目标(神雷灭世_计略范围, src_pos_); });
        神术_神雷灭世.handler = pk::unit_menu_item_handler_t(handler_神术_神雷灭世);
        pk::add_menu_item(神术_神雷灭世);
      }
      if (开启_黄天泰平) {
        pk::menu_item 神术_黄天泰平;
        神术_黄天泰平.menu = 神术数据结构体::菜单_神术;
        神术_黄天泰平.init = pk::unit_menu_item_init_t(init);
        神术_黄天泰平.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称("黄天泰平", 计略气力消耗_黄天泰平); });
        神术_黄天泰平.get_desc = pk::menu_item_get_desc_t(getDesc_神术_黄天泰平);
        神术_黄天泰平.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称({ 武将_张角, 武将_张梁, 武将_张宝 }, leader_id); });
        神术_黄天泰平.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_黄天泰平);
        神术_黄天泰平.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return 神术数据结构体::getTargets_神术_目标(黄天泰平_计略范围, src_pos_); });
        神术_黄天泰平.handler = pk::unit_menu_item_handler_t(handler_神术_黄天泰平);
        pk::add_menu_item(神术_黄天泰平);
      }
      if (开启_天下归心) {
        pk::menu_item 神术_天下归心;
        神术_天下归心.menu = 神术数据结构体::菜单_神术;
        神术_天下归心.init = pk::unit_menu_item_init_t(init);
        神术_天下归心.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称("天下归心", 计略气力消耗_通用); });
        神术_天下归心.get_desc = pk::menu_item_get_desc_t(getDesc_神术_天下归心);
        神术_天下归心.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称(武将_曹操, leader_id); });
        神术_天下归心.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_天下归心);
        神术_天下归心.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return 神术数据结构体::getTargets_神术_目标(天下归心_计略范围, src_pos_); });
        神术_天下归心.handler = pk::unit_menu_item_handler_t(handler_神术_天下归心);
        pk::add_menu_item(神术_天下归心);
      }
      if (开启_神鬼奇谋) {
        pk::menu_item 神术_神鬼奇谋;
        神术_神鬼奇谋.menu = 神术数据结构体::菜单_神术;
        神术_神鬼奇谋.init = pk::unit_menu_item_init_t(init);
        神术_神鬼奇谋.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称("神鬼奇谋", 计略气力消耗_通用); });
        神术_神鬼奇谋.get_desc = pk::menu_item_get_desc_t(getDesc_神术_神鬼奇谋);
        神术_神鬼奇谋.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称(武将_郭嘉, leader_id); });
        神术_神鬼奇谋.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_神鬼奇谋);
        神术_神鬼奇谋.handler = pk::unit_menu_item_handler_t(handler_神术_神鬼奇谋);
        pk::add_menu_item(神术_神鬼奇谋);
      }
      if (开启_妄尊仲帝) {
        pk::menu_item 神术_妄尊仲帝;
        神术_妄尊仲帝.menu = 神术数据结构体::菜单_神术;
        神术_妄尊仲帝.init = pk::unit_menu_item_init_t(init);
        神术_妄尊仲帝.get_text = cast<pk::menu_item_get_desc_t@>(function() { return main.getText_神术_妄尊仲帝(); });
        神术_妄尊仲帝.get_desc = pk::menu_item_get_desc_t(getDesc_神术_妄尊仲帝);
        神术_妄尊仲帝.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称(武将_袁术, leader_id); });
        神术_妄尊仲帝.handler = pk::unit_menu_item_handler_t(handler_神术_妄尊仲帝);
        pk::add_menu_item(神术_妄尊仲帝);
      }
      if (开启_神鬼八阵) {
        pk::menu_item 神术_神鬼八阵;
        神术_神鬼八阵.menu = 神术数据结构体::菜单_神术;
        神术_神鬼八阵.init = pk::unit_menu_item_init_t(init);
        神术_神鬼八阵.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称("神鬼八阵", 计略气力消耗_通用); });
        神术_神鬼八阵.get_desc = pk::menu_item_get_desc_t(getDesc_神术_神鬼八阵);
        神术_神鬼八阵.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称(武将_诸葛亮, leader_id); });
        神术_神鬼八阵.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_神鬼八阵);
        神术_神鬼八阵.handler = pk::unit_menu_item_handler_t(handler_神术_神鬼八阵);
        pk::add_menu_item(神术_神鬼八阵);
      }
      if (开启_狼顾权变) {
        pk::menu_item 神术_狼顾权变;
        神术_狼顾权变.menu = 神术数据结构体::菜单_神术;
        神术_狼顾权变.init = pk::unit_menu_item_init_t(init);
        神术_狼顾权变.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称("狼顾权变", 计略气力消耗_通用); });
        神术_狼顾权变.get_desc = pk::menu_item_get_desc_t(getDesc_神术_狼顾权变);
        神术_狼顾权变.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称(武将_司马懿, leader_id); });
        神术_狼顾权变.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_狼顾权变);
        神术_狼顾权变.handler = pk::unit_menu_item_handler_t(handler_神术_狼顾权变);
        pk::add_menu_item(神术_狼顾权变);
      }
      if (开启_火凤连环) {
        pk::menu_item 神术_火凤连环;
        神术_火凤连环.menu = 神术数据结构体::菜单_神术;
        神术_火凤连环.init = pk::unit_menu_item_init_t(init);
        神术_火凤连环.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称("火凤连环", 计略气力消耗_通用); });
        神术_火凤连环.get_desc = pk::menu_item_get_desc_t(getDesc_神术_火凤连环);
        神术_火凤连环.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称(武将_庞统, leader_id); });
        神术_火凤连环.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_火凤连环);
        神术_火凤连环.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return 神术数据结构体::getTargets_神术_目标(火凤连环_计略范围, src_pos_); });
        神术_火凤连环.handler = pk::unit_menu_item_handler_t(handler_神术_火凤连环);
        pk::add_menu_item(神术_火凤连环);
      }
      if (开启_乱武完杀) {
        pk::menu_item 神术_乱武完杀;
        神术_乱武完杀.menu = 神术数据结构体::菜单_神术;
        神术_乱武完杀.init = pk::unit_menu_item_init_t(init);
        神术_乱武完杀.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称("乱武完杀", 计略气力消耗_乱武完杀); });
        神术_乱武完杀.get_desc = pk::menu_item_get_desc_t(getDesc_神术_乱武完杀);
        神术_乱武完杀.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称({ 武将_贾诩 }, leader_id); });
        神术_乱武完杀.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_乱武完杀);
        神术_乱武完杀.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return 神术数据结构体::getTargets_神术_目标(3, src_pos_); });
        神术_乱武完杀.handler = pk::unit_menu_item_handler_t(handler_神术_乱武完杀);
        pk::add_menu_item(神术_乱武完杀);
      }
      if (开启_奇谋诡策) {
        pk::menu_item 神术_奇谋诡策;
        神术_奇谋诡策.menu = 神术数据结构体::菜单_神术;
        神术_奇谋诡策.init = pk::unit_menu_item_init_t(init);
        神术_奇谋诡策.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称("奇谋诡策", 计略气力消耗_通用); });
        神术_奇谋诡策.get_desc = pk::menu_item_get_desc_t(getDesc_神术_奇谋诡策);
        神术_奇谋诡策.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称(武将_法正, leader_id); });
        神术_奇谋诡策.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_奇谋诡策);
        神术_奇谋诡策.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return 神术数据结构体::getTargets_神术_目标(奇谋诡策_计略范围, src_pos_); });
        神术_奇谋诡策.handler = pk::unit_menu_item_handler_t(handler_神术_奇谋诡策);
        pk::add_menu_item(神术_奇谋诡策);
      }
      if (开启_帷幄奇策)
      {
        pk::menu_item 神术_帷幄奇策;
        神术_帷幄奇策.menu = 神术数据结构体::菜单_神术;
        神术_帷幄奇策.init = pk::unit_menu_item_init_t(init);
        神术_帷幄奇策.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称("帷幄奇策", 计略气力消耗_通用); });
        神术_帷幄奇策.get_desc = pk::menu_item_get_desc_t(getDesc_神术_帷幄奇策);
        神术_帷幄奇策.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称(武将_荀攸, leader_id); });
        神术_帷幄奇策.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_帷幄奇策);
        神术_帷幄奇策.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return 神术数据结构体::getTargets_神术_目标(帷幄奇策_计略范围, src_pos_); });
        神术_帷幄奇策.handler = pk::unit_menu_item_handler_t(handler_神术_帷幄奇策);
        pk::add_menu_item(神术_帷幄奇策);
      }
      if (开启_无间业火)
      {
        pk::menu_item 神术_无间业火;
        神术_无间业火.menu = 神术数据结构体::菜单_神术;
        神术_无间业火.init = pk::unit_menu_item_init_t(init);
        神术_无间业火.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称("无间业火", 计略气力消耗_通用); });
        神术_无间业火.get_desc = pk::menu_item_get_desc_t(getDesc_神术_无间业火);
        神术_无间业火.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称(武将_陆逊, leader_id); });
        神术_无间业火.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_无间业火);
        神术_无间业火.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return 神术数据结构体::getTargets_神术_目标部队位置(无间业火_计略范围, src_pos_); });
        神术_无间业火.handler = pk::unit_menu_item_handler_t(handler_神术_无间业火);
        pk::add_menu_item(神术_无间业火);
      }
      if (开启_握筹天下)
      {
        pk::menu_item 神术_握筹天下;
        神术_握筹天下.menu = 神术数据结构体::菜单_神术;
        神术_握筹天下.init = pk::unit_menu_item_init_t(init);
        神术_握筹天下.get_text = cast<pk::menu_item_get_desc_t@>(function() { return 神术数据结构体::getText_神术_名称("握筹天下", 计略气力消耗_通用); });
        神术_握筹天下.get_desc = pk::menu_item_get_desc_t(getDesc_神术_握筹天下);
        神术_握筹天下.is_visible = cast<pk::menu_item_is_visible_t@>(function() { return 神术数据结构体::isVisible_神术_名称(武将_荀彧, leader_id); });
        神术_握筹天下.is_enabled = pk::menu_item_is_enabled_t(isEnabled_神术_握筹天下);
        神术_握筹天下.get_targets = cast<pk::unit_menu_item_get_targets_t@>(function() { return 神术数据结构体::getTargets_神术_目标(握筹天下_计略范围, src_pos_); });
        神术_握筹天下.handler = pk::unit_menu_item_handler_t(handler_神术_握筹天下);
        pk::add_menu_item(神术_握筹天下);
      }
    }
    // ----------- 基础 ------------------
    void init(pk::unit@ unit, pk::point src_pos)
    {
      @src_unit = @unit;
      @src_leader = pk::get_person(src_unit.leader);
      @person_周瑜 = pk::get_person(武将_周瑜);
      @person_张角 = pk::get_person(武将_张角);
      @person_张宝 = pk::get_person(武将_张宝);
      @person_张梁 = pk::get_person(武将_张梁);
      @person_曹操 = pk::get_person(武将_曹操);
      @person_郭嘉 = pk::get_person(武将_郭嘉);
      @person_袁术 = pk::get_person(武将_袁术);
      @person_诸葛亮 = pk::get_person(武将_诸葛亮);
      @person_司马懿 = pk::get_person(武将_司马懿);
      @person_庞统 = pk::get_person(武将_庞统);
      @person_贾诩 = pk::get_person(武将_贾诩);
      @person_法正 = pk::get_person(武将_法正);
      @person_荀攸 = pk::get_person(武将_荀攸);
      @person_陆逊 = pk::get_person(武将_陆逊);
      @person_荀彧 = pk::get_person(武将_荀彧);
      @src_force = pk::get_force(unit.get_force_id());
      src_pos_ = src_pos;
      leader_id = src_unit.leader;
    }

    bool isVisible_神术()
    {
      if (pk::is_campaign()) return false;
      array<int> 神术技能武将= {
        person_周瑜.get_id(),
        person_张角.get_id(),
        person_张宝.get_id(),
        person_张梁.get_id(),
        person_曹操.get_id(),
        person_郭嘉.get_id(),
        person_袁术.get_id(),
        person_诸葛亮.get_id(),
        person_司马懿.get_id(),
        person_庞统.get_id(),
        person_贾诩.get_id(),
        person_法正.get_id(),
        person_荀攸.get_id(),
        person_陆逊.get_id(),
        person_荀彧.get_id()
      };
      if (神术技能武将.find(src_leader.get_id()) >= 0) return true;
      return false;
    }


    // ------------------------------------------------
    void onMonthStart()
    {
      for (int i = 0; i < 可用武将_末; i++)
      {
        sc_personinfo@ person_t = @person_sc[i];
        if (person_t.神鬼八阵_使用) person_t.神鬼八阵_使用 = false;
      }
    }
    void onTurnStart(pk::force@ force)
    {
      if (person_袁术 is null) @person_袁术 = pk::get_person(武将_袁术);
      if (force.get_id() == person_袁术.get_force_id()) 神术_妄尊仲帝_补兵处理();
    }
    void onTurnEnd(pk::force@ force)
    {
      // if (person_司马懿 is null)
      // {
      //   @person_司马懿 = pk::get_person(武将_司马懿);
      // }
      if (开启_火凤连环) 火凤连环清除();
      if (开启_奇谋诡策) 奇谋诡策清除();
      if (开启_狼顾权变) 神术_狼顾权变_特技处理();
      if (开启_帷幄奇策) 神术_帷幄奇策_特技处理();
    }
    void onUnitRemove(pk::unit@ unit, pk::hex_object@ dst, int type)
    {
      if (开启_神鬼八阵) 神鬼八阵部队溃灭处理(unit);
      if (开启_火凤连环) 火凤连环部队溃灭处理(unit);
      if (开启_乱武完杀) 乱武完杀部队溃灭处理(unit, dst, type);
      if (开启_帷幄奇策) 帷幄奇策部队溃灭处理(unit, dst, type);
      if (开启_握筹天下) 握筹天下部队溃灭处理(unit, dst, type);
    }

    void onUnitExpAdd(pk::unit@ unit, int type)
    {
      if (开启_握筹天下) 神术_握筹天下_攻击处理(unit, type);
    }

    void onUnitActionDone(pk::unit@ unit,int type)
    {
      sc_unitinfo@ sc_unit = @unit_sc[unit.get_id()];
      if (sc_unit.神鬼八阵效果) sc_unit.神鬼八阵效果 = false;
    }

    void onUnitAfterAttack(pk::unit@ dst_unit, pk::unit@ unit, int troops_damage, int critical, int tactics)
    {
      if (开启_奇谋诡策) 神术_奇谋诡策_伤害处理(dst_unit, unit);
    }

    void onUnitDestroy(pk::unit@ unit, pk::unit@ dst_unit)
    {
    }

    void func209(pk::damage_info& info, pk::unit@ attacker, int tactics_id, const pk::point& in target_pos, int type, int critical, bool ambush, int rettype)
    {
      prev_callback_209(info, attacker, tactics_id, target_pos, type, critical, ambush, rettype);
      if (开启_神鬼八阵) 神术_神鬼八阵_伤害处理(info, attacker, target_pos);
      // if (开启_握筹天下) 神术_握筹天下_伤害处理(info, attacker, target_pos, type);
    }

    int func58(pk::unit@ unit,int value, int rettype)
    {
      int troops = prev_callback_58(unit, value, rettype);
      if (unit.leader == 武将_袁术 and 妄尊仲帝_启用状态)
      {
        troops = 神术_妄尊仲帝_伤害处理(unit, troops, rettype);
      }
      if (开启_火凤连环) 火凤连环伤害(unit, value, rettype);
      if (开启_乱武完杀) 乱武完杀部队受伤处理(unit, value, rettype);
      return int(troops);
    }

    int func205(pk::unit@ src, int strategy_id)
    {
      int energy_cost = prev_callback_205(src, strategy_id);
      if (开启_帷幄奇策) return 帷幄奇谋_计略气力消耗(src, energy_cost);
      return energy_cost;
    }

    int func53(pk::unit@ unit, pk::tactics@ tactic, int type)
    {
      int energy_cost = prev_callback_53(unit, tactic, type);
      if (开启_帷幄奇策) return 帷幄奇谋_战法气力消耗(unit, energy_cost);
      return energy_cost;
    }

    int func202(pk::unit@ attacker, const pk::point& in pos, pk::hex_object@ target, int tactics_id)
    {
      int success_chance = prev_callback_202(attacker, pos, target, tactics_id);
      if (开启_帷幄奇策) return 帷幄奇谋_战法成功率(attacker, success_chance);
      return success_chance;
    }

    void func214(pk::damage_info& info, pk::unit@ attacker, pk::hex_object@ target, bool critical)
    {
      prev_callback_214(info, attacker, target, critical);
      if (开启_无间业火) 无间业火部队伤害处理(info, attacker, target, critical);
    }

    bool func169(pk::unit@ unit, int terrain_id)
    {
      bool result = prev_callback_169(unit, terrain_id);
      if ((贾诩小兵(unit)) and (terrain_id == 16 or terrain_id == 17 or terrain_id == 18)) return false;
      return result;
    }

    void func220(const pk::destroy_info& in info, pk::list<pk::person@>& captured, pk::list<pk::person@>& escaped, bool tactics_bonus)
    {
      prev_callback_220(info, captured, escaped, tactics_bonus);
      贾诩小兵俘虏处理(info, captured, escaped);
    }

    // ------------------- 业火焚天------------------

    string getDesc_神术_业火焚天()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("使用业火焚天.(气力至少 {} 以上)", 计略气力消耗_通用));
    }
    bool isEnabled_神术_业火焚天()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      else if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void func_业火焚天(pk::point src_pos, int range) {
      array<pk::point> arr = pk::range(src_pos, range, range);
      for (int l = 0; l < int(arr.length); l++)
      {

        pk::hex_object@ hex_obj = pk::get_hex_object(arr[l]);

        pk::unit@ dst = pk::get_unit(arr[l]);
        pk::building@ building = pk::get_building(arr[l]);

        if (dst !is null) {
          if (!pk::is_enemy(src_unit, dst)) continue;
          if (dst.has_skill(特技_火神)) {
            pk::create_effect(0x55, arr[l]);
            pk::say(pk::encode("火神在此，有何俱焉"), pk::get_person(dst.who_has_skill(特技_火神)));
            continue;
          }
          if (dst.has_skill(特技_洞察)) {
            pk::create_effect(0x55, arr[l]);
            pk::say(pk::encode("区区小计，也要丢人现眼吗"), pk::get_person(dst.who_has_skill(特技_洞察)));
            continue;
          }
          pk::set_status(dst, src_unit, 部队状态_伪报, pk::rand(2) + 3, true);
          神术数据结构体::历史日志(dst, '神术_业火焚天', '陷入伪报');
          pk::create_fire(arr[l], pk::rand(2) + 3, src_unit, true); //火计
          pk::add_energy(dst, -30, true); //减气
          int 最终伤害值 = random(500, 1500);
          ch::add_troops(dst, -最终伤害值, true); // 随机减兵500~1500
          神术数据结构体::设置部队伤害值(src_unit, 最终伤害值);
          神术数据结构体::历史日志(dst, '神术_业火焚天', '受到了巨大伤害');
          if (dst.troops <= 0) {
            pk::kill(dst, src_unit);
          }
          if (pk::is_in_screen(hex_obj.pos))
          {
            switch (pk::rand(4))
            {
            case 0: pk::say(pk::encode("哇哇哇.."), pk::get_person(武将_兵士), dst); break;
            case 1: pk::say(pk::encode("我的身体"), pk::get_person(武将_兵士), dst); break;
            case 2: pk::say(pk::encode("快去避火"), pk::get_person(武将_兵士), dst); break;
            case 3: pk::say(pk::encode("火. 着火了!"), pk::get_person(武将_兵士), dst); break;
            }
          }
        }

        if (building !is null) {
          if (building.get_force_id() == src_unit.get_force_id()) continue;
          if (building.has_skill(特技_火神) or building.has_skill(特技_洞察)) continue;
          int hp_damage = 建筑火伤_练成炸药附加 + pk::rand(建筑火伤_练成炸药附加) + pk::rand(2000);
          神术数据结构体::历史日志(building, '神术_业火焚天', '受到了巨大伤害');
          神术数据结构体::fun_处理城市(building, arr[l], hp_damage);

          building.update();
        }

        if (dst is null and building is null) {
          pk::create_fire(arr[l], pk::rand(2) + 3, src_unit, true);
        }
      }
    }

    bool handler_神术_业火焚天(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(122,1000); // 全火遮罩

      for (int l = 0; l < 业火焚天_效果范围; l++) {
        func_业火焚天(dst_pos, l);
      }

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 10);

      pk::say(pk::encode("感受下这红莲业火吧!"), person_周瑜);

      // pk::wait(100);

      pk::say(pk::encode("怎么感觉有点虚弱呢，难道是天谴吗。。。"), person_周瑜);

      神术数据结构体::减少武将寿命(src_leader, random(3, 10));
      src_leader.update();

      神术数据结构体::已影响城市.removeRange(0, 神术数据结构体::已影响城市.length() - 1); // 清除处理过的

      神术数据结构体::部队行动(src_unit);

      return true;
    }


    //-------------------------神雷灭世---------------
    string getDesc_神术_神雷灭世()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("使用神雷灭世.(气力至少 {} 以上)", 计略气力消耗_通用));
    }


    bool isEnabled_神术_神雷灭世()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      else if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }


    void func_神雷灭世(pk::point src_pos, int range) {
      array<pk::point> arr = pk::range(src_pos, range, range);
      for (int l = 0; l < int(arr.length); l++)
      {

        pk::create_effect(0x40, arr[l]); // 落雷动画

        pk::hex_object@ hex_obj = pk::get_hex_object(arr[l]);

        pk::unit@ dst = pk::get_unit(arr[l]);
        pk::building@ building = pk::get_building(arr[l]);

        if (dst !is null) {
          if (!pk::is_enemy(src_unit, dst)) continue;
          if (dst.has_skill(特技_鬼门)) {
            pk::create_effect(0x55, arr[l]);
            pk::say(pk::encode("区区雷电，不过尔尔"), pk::get_person(dst.who_has_skill(特技_鬼门)));
            continue;
          }
          pk::set_status(dst, src_unit, 部队状态_混乱, pk::rand(2) + 3, true); // 雷电麻痹
          神术数据结构体::历史日志(dst, '神术_神雷灭世', '陷入混乱');
          if (pk::rand_bool(50)) { // 50%概率着火
            pk::create_fire(arr[l], pk::rand(2), src_unit, true); //火计
          }
          pk::add_energy(dst, -30, true); //减气
          int 最终落雷伤害 = 部队落雷伤害 + pk::rand(部队落雷伤害_1) + pk::rand(1000);
          ch::add_troops(dst, -最终落雷伤害, true); // 在雷电伤害基础上随机加2000
          神术数据结构体::设置部队伤害值(src_unit, 最终落雷伤害);
          神术数据结构体::历史日志(dst, '神术_神雷灭世', '受到了巨大伤害');
          if (dst.troops <= 0)
          {
            pk::kill(dst, src_unit);
          }
          if (pk::is_in_screen(hex_obj.pos))
          {
            switch (pk::rand(4))
            {
            case 0: pk::say(pk::encode("哇哇哇.."), pk::get_person(武将_兵士), dst); break;
            case 1: pk::say(pk::encode("我的身体"), pk::get_person(武将_兵士), dst); break;
            case 2: pk::say(pk::encode("这是什么力量..."), pk::get_person(武将_兵士), dst); break;
            case 3: pk::say(pk::encode("难道苍天死了吗..."), pk::get_person(武将_兵士), dst); break;
            }
          }
        }

        if (building !is null) {
          if (!pk::is_enemy(src_unit, building)) continue;
          if (building.has_skill(特技_鬼门)) continue;
          int hp_damage = 建筑落雷伤害 + pk::rand(建筑落雷伤害_1) + pk::rand(2000);
          神术数据结构体::历史日志(building, '神术_神雷灭世', '受到了巨大伤害');
          神术数据结构体::fun_处理城市(building, arr[l], hp_damage);
          building.update();
        }

        if (dst is null and building is null) {
          if (pk::rand_bool(50)) { // 50%概率着火
            pk::create_fire(arr[l], pk::rand(2), src_unit, true); //火计
          }
        }
      }
    }

    bool handler_神术_神雷灭世(pk::point dst_pos)
    {
      pk::special_cutin(127,1000); // 雷电遮罩
      pk::play_se(121);

      for (int l = 0; l < 神雷灭世_效果范围; l++) {
        func_神雷灭世(dst_pos, l);
        if (l % 3 == 2)
        {
          pk::wait(1000);
        }
      }

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 10);

      pk::say(pk::encode("呼风唤雨，驱雷策电!道施所向，皆由我控!"), src_leader);

      // pk::wait(100);

      pk::say(pk::encode("黄天既覆，苍生何存......"), src_leader);
      神术数据结构体::减少武将寿命(src_leader, random(3, 10));
      src_leader.update();

      神术数据结构体::部队行动(src_unit);

      return true;
    }

    // ----------- 黄天泰平 ----------
    string getDesc_神术_黄天泰平()
    {
      if (src_unit.energy < 计略气力消耗_黄天泰平)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("消耗气力{}, 一定几率劝降{}格内部队", 计略气力消耗_黄天泰平, 黄天泰平_效果范围));
    }

    bool isEnabled_神术_黄天泰平()
    {
      if (src_unit.energy < 计略气力消耗_黄天泰平) return false;
      else if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void func_黄天泰平(pk::point src_pos, int range) {
      array<pk::point> arr = pk::range(src_pos, range, range);
      for (int l = 0; l < int(arr.length); l++)
      {

        pk::create_effect(0x3a, arr[l]); // 妖术动画

        pk::unit@ dst = pk::get_unit(arr[l]);
        if (dst !is null) {
          // if (!pk::is_enemy(src_unit, dst)) continue;
          if (src_unit.get_force_id() == dst.get_force_id()) continue;
          if (dst.has_skill(特技_仁政)) {
            pk::create_effect(0x55, arr[l]);
            pk::say(pk::encode("夫仁政，必自经界始"), pk::get_person(dst.who_has_skill(特技_仁政)));
            continue;
          }
          for (int m = 0; m < 3; m++)
          {
            if (pk::is_valid_person_id(dst.member[m]))
            {
              pk::person@ member_t = pk::get_person(dst.member[m]);  //隊伍中的武將
              if (member_t is null || !pk::is_alive(member_t)) continue;
              if (member_t.mibun == 身份_君主)
              {
                // 君主，万分之一的概率直接收编
                if (ch::rand_bool_10000(10000))
                {
                  pk::say(pk::encode("黄天有道，吾愿降。。。"), member_t);
                  pk::force@ king_force = pk::get_force(member_t.get_force_id());
                  pk::force@ attack_force = pk::get_force(src_leader.get_force_id());
                  pk::wait(1000);
                  pk::kill(king_force, true, attack_force);
                  king_force.update();
                  attack_force.update();
                }
                continue;
              }
              if (member_t.mibun == 身份_都督)
              {
                // 都督，千分之一的概率直接收编
                if (ch::rand_bool_1000(1))
                {
                  pk::say(pk::encode("黄天有道，吾愿降。。。"), member_t);
                  // 神术数据结构体::历史日志(dst, '神术_黄天泰平', '的蛊惑，投靠');
                  int member_district_id = member_t.get_district_id();
                  int attack_district_id = src_leader.get_district_id();

                  int member_force = member_t.get_force_id();
                  pk::district@ member_district = pk::get_district(member_district_id);

                  auto building_list = pk::get_building_list();
                  if (0 < building_list.count)
                  {
                    for (int j = 0; j < int(building_list.count); j++)
                    {
                      pk::building@ building_0 = building_list[j];
                      if (building_0.get_district_id() == member_district_id)
                      {
                        if (building_0.facility > 2)
                        {
                          building_0.init_force = src_leader.get_force_id();
                          building_0.update();
                        }
                        if (building_0.facility == 2)
                        {
                          pk::port@ port0 = pk::get_port(j - 52);
                          port0.district = attack_district_id;
                          port0.update();
                        }
                        if (building_0.facility == 1)
                        {
                          pk::gate@ gate0 = pk::get_gate(j - 42);
                          gate0.district = attack_district_id;
                          gate0.update();
                        }
                        if (building_0.facility == 0)
                        {
                          pk::city@ city0 = pk::get_city(j);
                          city0.district = attack_district_id;
                          city0.update();
                        }
                      }
                    }
                  }
                  auto person_list = pk::get_person_list(member_district, pk::mibun_flags(身份_都督, 身份_太守, 身份_一般));
                  if (0 < person_list.count)
                  {
                    for (int j = 0; j < int(person_list.count); j++)
                    {
                      pk::person@ person_0 = person_list[j];
                      // person_0.district = attack_district_id;
                      pk::set_district(person_0, attack_district_id);
                      if (person_0.mibun == 身份_都督)
                      {
                        person_0.mibun = 身份_一般;
                      }
                      if (person_0.order == 武将任务_亲善 or person_0.order == 武将任务_同盟 or person_0.order == 武将任务_摒弃 or person_0.order == 武将任务_停战协定 or person_0.order == 武将任务_劝降 or person_0.order == 武将任务_交换俘虏 or person_0.order == 武将任务_请求援军 or person_0.order == 武将任务_二虎竞食 or person_0.order == 武将任务_二虎竞食)
                      {
                        person_0.order = 武将任务_返回;
                        person_0.target[0] = person_0.service;
                      }
                      person_0.update();
                    }
                  }
                  auto unit_list = pk::get_unit_list(member_district);
                  if (0 < unit_list.count)
                  {
                    for (int j = 0; j < int(unit_list.count); j++)
                    {
                      pk::unit@ unit_0 = unit_list[j];
                      unit_0.update();
                    }
                  }

                }
                continue;
              }
              if (pk::rand_bool(5))
              { // 5%概率武将直接叛变
                神术数据结构体::历史日志(member_t, '神术_黄天泰平', '投靠别的势力了');
                pk::set_district(member_t, src_leader.get_district_id());
                member_t.mibun = 身份_一般;
              }
              else if (pk::rand_bool(20))
              {
                // 20% 概率下野
                神术数据结构体::历史日志(member_t, '神术_黄天泰平', '下野了');
                switch (pk::rand(4))
                {
                  case 0: pk::say(pk::encode("良禽折木而栖"), member_t); break;
                  case 1: pk::say(pk::encode("果然还是不适合"), member_t); break;
                  case 2: pk::say(pk::encode("再见了..."), member_t); break;
                  case 3: pk::say(pk::encode("世界那么大，我想去看看"), member_t); break;
                }

                if (m == 0) // 主将在野，部队解散
                {
                  pk::kill(dst);
                }
                pk::resign(member_t);
                member_t.update();
              }
              else if (pk::rand_bool(50))
              {
                // 50% 概率减忠1~3
                pk::add_loyalty(member_t, -random(1, 3));
                pk::say(pk::encode("君主大人好像也不是很好。。。"), member_t);
              }
            }
          }

        }
      }
    }

    bool handler_神术_黄天泰平(pk::point dst_pos)
    {

      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      for (int l = 0; l < 黄天泰平_效果范围; l++) {
        func_黄天泰平(dst_pos, l);
      }

      pk::add_energy(src_unit, -计略气力消耗_黄天泰平, true);

      pk::set_status(src_unit, null, 部队状态_混乱, 1, true); // 自己眩晕一回合

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("救苍生于水火，平天下于乱世!"), src_leader);

      神术数据结构体::减少武将寿命(src_leader, random(5, 10));
      src_leader.update();

      神术数据结构体::部队行动(src_unit);

      return true;
    }

    // ----------- 天下归心 ----------
    string getDesc_神术_天下归心()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("消耗气力{}，自身伪报，一定计略俘虏{}格内部队的女性武将", 计略气力消耗_通用, 天下归心_效果范围));
    }

    bool isEnabled_神术_天下归心()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      else if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void func_天下归心(pk::point src_pos, int range) {
      array<pk::point> arr = pk::range(src_pos, range, range);
      for (int l = 0; l < int(arr.length); l++)
      {

        pk::create_effect(0x3a, arr[l]); // 妖术动画

        pk::unit@ dst = pk::get_unit(arr[l]);
        if (dst !is null) {
          // if (!pk::is_enemy(src_unit, dst)) continue;
          if (src_unit.get_force_id() == dst.get_force_id()) continue;
          if (dst.has_skill(特技_内助)) {
            pk::create_effect(0x55, arr[l]);
            pk::say(pk::encode("内助，非细事也"), pk::get_person(dst.who_has_skill(特技_内助)));
            continue;
          }
          for (int m = 0; m < 3; m++)
          {
            if (pk::is_valid_person_id(dst.member[m]))
            {
              pk::person@ member_t = pk::get_person(dst.member[m]);  //隊伍中的武將
              if (member_t is null || !pk::is_alive(member_t) || member_t.mibun == 身份_君主) continue;

              int rang = 0;
              if (member_t.sex == 性别_女)
              {
                rang += 100;
                if (member_t.spouse != -1) { // 已有配偶
                  rang += 40;
                }
                if (pk::rand_bool(rang)) {
                  神术数据结构体::历史日志(member_t, '神术_天下归心', '被俘虏了');
                  member_t.former_force = member_t.get_force_id();
                  pk::set_district(member_t, (pk::get_person(pk::get_kunshu_id(member_t))).get_district_id());
                  member_t.mibun = 身份_俘虏;
                  member_t.location = src_leader.location;
                  pk::building@ building = pk::get_building(pk::get_service(src_unit));
                  pk::set_service(member_t, building.get_id());
                  pk::say(pk::encode("恶贼！"), member_t);
                  if (m == 0) // 主将被俘，部队解散
                  {
                    pk::kill(dst);
                  }
                  else
                  {
                    if (m == 1) {
                      dst.member[1] = dst.member[2];
                      dst.member[2] = -1;
                      m -= 1;
                    }
                    if (m == 2) {
                      dst.member[2] = -1;
                    }
                    pk::update_unit(dst);
                  }
                  if (member_t.spouse != -1) {
                    pk::person@ target = pk::get_person(member_t.spouse);
                    member_t.spouse = -1;
                    target.spouse = -1;

                    member_t.update();
                    target.update();

                    pk::say(pk::encode("曹贼我与你势不两立！"), target);
                    pk::add_dislike(target, person_曹操.get_id());
                  }
                }
              }
            }
          }

        }
      }
    }

    bool handler_神术_天下归心(pk::point dst_pos)
    {

      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      for (int l = 0; l < 天下归心_效果范围; l++) {
        func_天下归心(dst_pos, l);
        // pk::wait(1000);
      }

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::set_status(src_unit, null, 部队状态_伪报, 1, true); // 自己眩晕一回合

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("汝妻子吾自养之，汝勿虑也"), src_leader);

      // pk::wait(100);
      神术数据结构体::减少武将寿命(src_leader, random(5, 10));
      src_leader.update();

      神术数据结构体::部队行动(src_unit);

      return true;
    }

    // ----------------- 神鬼奇谋 ---------------
    string getDesc_神术_神鬼奇谋()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("消耗气力{}，距离最近{}只地方部队眩晕，最近{}只已方部队回复兵力", 计略气力消耗_通用, 神鬼奇谋_已方数量, 神鬼奇谋_敌方数量));
    }

    bool isEnabled_神术_神鬼奇谋()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      else if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void func_神鬼奇谋_处理敌方部队(int max_unit = 10)
    {
      array<pk::point> target_unit_list;
      array<pk::point> arr = pk::range(src_pos_, 1, 500);
      for (int i = 0; i < int(arr.length); i++)
      {
        if (int(target_unit_list.length) < max_unit)
        {
          pk::point dst_pos = arr[i];
          if (pk::is_valid_pos(dst_pos)) {
            pk::unit@ dst = pk::get_unit(dst_pos);
            // force_liubei.ally[force_sun.get_id()
            // src_force.ally[dst.get_force_id())
            if (dst !is null and dst.get_force_id() != src_unit.get_force_id())
            {
              bool ally_effect = 神鬼奇谋_盟军影响 or !src_force.ally[dst.get_force_id()];
              if (ally_effect) target_unit_list.insertLast(dst_pos);
            }
          }
        } else
        {
          break;
        }
      }

      for (int i = 0; i < int(target_unit_list.length); i++)
      {
        pk::point dst_pos = target_unit_list[i];
        if (pk::is_valid_pos(dst_pos)) {
          pk::unit@ dst = pk::get_unit(dst_pos);
          pk::set_status(dst, src_unit, 部队状态_混乱, 2 + pk::rand(3), true); // 混乱2~5回合
          神术数据结构体::历史日志(dst, '神术_神鬼奇谋', '陷入混乱');
          int 最终伤害值 = int(0.5 * pk::get_max_troops(dst));
          if (int(dst.troops) > 最终伤害值) { // 兵力超过最大兵力一半
            ch::add_troops(dst, -最终伤害值, true);
            神术数据结构体::设置部队伤害值(src_unit, 最终伤害值);
            神术数据结构体::历史日志(dst, '神术_神鬼奇谋', '受到了巨大伤害');
          }
          pk::say(pk::encode("这，这是发生什么"), pk::get_person(dst.leader));
        }
      }
    }

    void func_神鬼奇谋_处理已方部队(int max_unit = 10)
    {

      array<pk::point> target_unit_list;
      array<pk::point> arr = pk::range(src_pos_, 1, 500);
      for (int i = 0; i < int(arr.length); i++)
      {
        if (int(target_unit_list.length) < max_unit)
        {
          pk::point dst_pos = arr[i];
          if (pk::is_valid_pos(dst_pos)) {
            pk::unit@ dst = pk::get_unit(dst_pos);
            if (dst !is null and dst.get_force_id() == src_unit.get_force_id())
            {
              target_unit_list.insertLast(dst_pos);
            }
          }
        } else
        {
          break;
        }
      }

      for (int i = 0; i < int(target_unit_list.length); i++)
      {
        pk::point dst_pos = target_unit_list[i];
        if (pk::is_valid_pos(dst_pos)) {
          pk::unit@ dst = pk::get_unit(dst_pos);
          pk::add_energy(dst, 5 + pk::rand(10), true);
          if (int(dst.troops) < int(0.5 * pk::get_max_troops(dst))) { // 兵力小于最大兵力一半
            ch::add_troops(dst, int(0.5 * pk::get_max_troops(dst)), true);
            神术数据结构体::历史日志(dst, '神术_神鬼奇谋', '补充了大量兵士');
          }
          pk::say(pk::encode("先生真乃世之奇士也"), pk::get_person(dst.leader));
        }
      }
    }

    void func_神鬼奇谋()
    {
      func_神鬼奇谋_处理敌方部队(神鬼奇谋_敌方数量);
      func_神鬼奇谋_处理已方部队(神鬼奇谋_已方数量);
    }

    bool handler_神术_神鬼奇谋(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      func_神鬼奇谋();

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("策谋本天成，妙手偶得之!"), src_leader);

      神术数据结构体::减少武将寿命(src_leader, random(0, 1));
      src_leader.update();

      神术数据结构体::部队行动(src_unit);

      return true;
    }


    // --------------- 妄尊仲帝 ---------------
    string getText_神术_妄尊仲帝()
    {
      return pk::encode(pk::format("妄尊仲帝({})", 妄尊仲帝_启用状态 ? '已开启' : '已关闭'));
    }
    string getDesc_神术_妄尊仲帝()
    {
      return pk::encode("伤害减半，且可用金钱代替（可关闭）。每回合回兵势力数%");
    }

    bool isEnabled_神术_妄尊仲帝()
    {
      return true;
    }

    bool handler_神术_妄尊仲帝(pk::point dst_pos)
    {
      // pk::play_se(120);
      // pk::special_cutin(126,1000); // 妖术遮罩
      妄尊仲帝_启用状态 = !妄尊仲帝_启用状态;

      return true;
    }

    int 神术_妄尊仲帝_伤害处理(pk::unit@ unit, int value, int rettype)
    {
      int troops = value;
      if (troops < 0 and (rettype == 14 or rettype == 100))
      {
        if (unit.gold > 0)
        {
          troops = int(0.5f * troops);
          pk::add_gold(unit, troops, true);
          troops += unit.gold;
          troops = troops > 0 ? 0 : troops;
          // pk::wait(300);
          pk::say(pk::encode("大胆！冒犯天威，大逆不道！！"), person_袁术);
          神术数据结构体::历史日志(unit, '神术_妄尊仲帝', '减少了大量伤害');
        }
        else
        {
          pk::say(pk::encode("可恶！就差……一步了……"), person_袁术);
        }
      }
      return troops;
    }

    void 神术_妄尊仲帝_补兵处理()
    {
      pk::unit@ 袁术部队 = pk::get_unit(pk::hex_object_id_to_unit_id(person_袁术.location));
      if (袁术部队 !is null)
      {
        int troops = int(pk::get_force_list().count * 袁术部队.troops / 100);
        ch::add_troops(袁术部队, troops, true);
        pk::say(pk::encode("乱世之中，必出枭雄！哈哈哈"), person_袁术);
        神术数据结构体::历史日志(袁术部队, '神术_妄尊仲帝', '补充了大量兵士');
      }
    }

    // --------- 神鬼八阵 -----------
    string getDesc_神术_神鬼八阵()
    {
      sc_personinfo@ person_t = @person_sc[src_leader.get_id()];
      if (person_t.神鬼八阵_使用)
        return pk::encode("本月已经使用过.");
      else if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("每月限一次, 耗气{}，{}格内友军本旬获得一击必杀能力，敌军眩晕3~8旬", 计略气力消耗_通用, 神鬼八阵_效果范围));
    }

    bool isEnabled_神术_神鬼八阵()
    {
      sc_personinfo@ person_t = @person_sc[src_leader.get_id()];
      if (person_t.神鬼八阵_使用) return false;
      if (src_unit.energy < 计略气力消耗_通用) return false;
      if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void 神鬼八阵部队溃灭处理(pk::unit@ unit)
    {
      sc_unitinfo@ sc_unit = @unit_sc[unit.get_id()];
      if (sc_unit.神鬼八阵效果)
      {
        sc_unit.神鬼八阵效果 = false;
      }
    }

    bool handler_神术_神鬼八阵(pk::point dst_pos)
    {
      sc_personinfo@ person_t = @person_sc[src_leader.get_id()];
      person_t.神鬼八阵_使用 = true;
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      pk::force@ force = pk::get_force(src_unit.get_force_id());
      pk::list<pk::unit@> target_unit_list = 神术数据结构体::getTargets_神术_目标部队(神鬼八阵_效果范围, src_pos_);
      for (int i = 0; i < target_unit_list.count; i++)
      {
        pk::unit@ target_unit = target_unit_list[i];
        if (target_unit.get_force_id() == src_unit.get_force_id())
        {
          神术数据结构体::历史日志(target_unit, '神术_神鬼八阵', '获得一击必杀能力');
          pk::set_status(target_unit, src_unit, 部队状态_引诱, 1, true);
          sc_unitinfo@ sc_unit = @unit_sc[target_unit.get_id()];
          sc_unit.神鬼八阵效果 = true;
        }
        else
        {
          pk::set_status(target_unit, src_unit, 部队状态_混乱, 3 + pk::rand(5), true);
        }
      }
      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 50);

      pk::say(pk::encode("奇正相生，循环无端!"), src_leader);

      神术数据结构体::部队行动(src_unit);

      return true;

    }

    void 神术_神鬼八阵_伤害处理(pk::damage_info& info, pk::unit@ attacker, const pk::point& in target_pos)
    {
      sc_unitinfo@ sc_unit = @unit_sc[attacker.get_id()];
      if (sc_unit.神鬼八阵效果)
      {
        pk::building@ building = pk::get_building(target_pos);
        pk::unit@ unit = pk::get_unit(target_pos);
        if (building !is null and building.facility != 设施_城市 and building.facility != 设施_关隘 and building.facility != 设施_港口)
        {
          info.hp_damage = building.hp;
        }
        else if (unit !is null)
        {
          info.troops_damage = info.dst_troops;
        }
        info.critical = true;
      }
    }

    // ----------- 狼顾权变 --------------------
    string getDesc_神术_狼顾权变()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (getTargets_狼顾权变_目标部队位置(狼顾权变_效果范围).length == 0)
        return pk::encode("周围没有目标.");
      else
        return pk::encode(pk::format("耗气{},令自身{}格内敌方武将特技失效{}回合,并可令已方一名武将习得其中一个特技", 计略气力消耗_通用, 狼顾权变_效果范围, 狼顾权变_技能失效时长));
    }

    bool isEnabled_神术_狼顾权变()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      if (getTargets_狼顾权变_目标部队位置(狼顾权变_效果范围).length == 0) return false;
      return true;
    }

    pk::array<pk::point_int> getTargets_狼顾权变_目标部队位置(int range)
    {
        pk::array<pk::point_int> targets;
        array<pk::point> rings = pk::range(src_pos_, 1, range);
        for (int i = 0; i < int(rings.length); i++)
        {
          pk::point dst_pos = rings[i];
          pk::unit@ dst = pk::get_unit(dst_pos);
          if (dst !is null and dst.get_force_id() != src_unit.get_force_id())
          {
            bool ally_effect = 狼顾权变_盟军影响 || !src_force.ally[dst.get_force_id()];
            if (ally_effect)
            {
              array<string> skill_names = get_skill_name(dst);
              if (skill_names.length > 0) targets.insertLast(pk::point_int(dst_pos, 1));
            }
          }
        }

        return targets;
    }

    array<int> get_skill_id(pk::unit@ unit)
    {
      array<pk::point> rings = pk::range(unit.get_pos(), 1, 狼顾权变_效果范围);
      array<int> unit_ids;
      for (int i = 0; i < int(rings.length); i++)
      {
        pk::point target_pos = rings[i];
        pk::unit@ dst = pk::get_unit(target_pos);
        if (dst !is null and dst.get_force_id() != src_unit.get_force_id())
        {
          bool ally_effect = 狼顾权变_盟军影响 || !src_force.ally[dst.get_force_id()];
          if (ally_effect)
          {
            for (int m = 0; m < 3; m++)
            {
              if (pk::is_valid_person_id(dst.member[m]))
              {
                pk::person@ member_t = pk::get_person(dst.member[m]);
                if (member_t is null || !pk::is_alive(member_t)) continue;
                if (member_t.skill >= 0) unit_ids.insertLast(member_t.skill);
              }
            }
          }
        }
      }

      return unit_ids;
    }

    array<string> get_skill_name(pk::unit@ unit)
    {
      array<string> skill_names;
      for (int m = 0; m < 3; m++)
      {
        if (pk::is_valid_person_id(unit.member[m]))
        {
          pk::person@ member_t = pk::get_person(unit.member[m]);
          if (member_t is null || !pk::is_alive(member_t)) continue;
          if (member_t.skill >= 0)
          {
            pk::skill@ skill = pk::get_skill(member_t.skill);
            skill_names.insertLast(skill.name);
          }
        }
      }
      return skill_names;
    }

    void clear_skill(pk::unit@ unit)
    {
      array<pk::point> rings = pk::range(unit.get_pos(), 1, 狼顾权变_效果范围);
      for (int i = 0; i < int(rings.length); i++)
      {
        pk::point target_pos = rings[i];
        pk::unit@ dst = pk::get_unit(target_pos);

        if (dst !is null and dst.get_force_id() != src_unit.get_force_id())
        {
          bool ally_effect = 狼顾权变_盟军影响 || !src_force.ally[dst.get_force_id()];
          if (ally_effect)
          {
            for (int m = 0; m < 3; m++)
            {
              if (pk::is_valid_person_id(dst.member[m]))
              {
                pk::person@ member_t = pk::get_person(dst.member[m]);
                if (member_t is null || !pk::is_alive(member_t)) continue;
                if (member_t.skill >= 0)
                {
                  神术数据结构体::历史日志(member_t, '神术_狼顾权变', pk::format("失去特技【\x1b[1x{}\x1b[0x】", pk::decode(pk::get_skill(member_t.skill).name)));
                  sc_personinfo@ person_t = @person_sc[member_t.get_id()];
                  person_t.狼顾权变_失去技能回合 = pk::get_elapsed_days() + 10;
                  person_t.狼顾权变_技能失效编号 = member_t.skill;
                  pk::set_skill(member_t, -1);
                  pk::say(pk::encode("我，我这是怎么了！！!"), member_t);
                  member_t.update();
                }
              }
            }
          }
        }
      }
    }

    int choose_skill(array<int> skill_ids)
    {
        int num = int(skill_ids.length);
        array<string> skill_name_list;
        for (int i = 0; i < pk::min(5, num); ++i)
        {
          pk::skill@ skill = pk::get_skill(skill_ids[i]);
          skill_name_list.insertLast(skill.name);
        }

        if (num == 6) skill_name_list.insertLast(pk::get_skill(skill_ids[num]).name);
        else if (num > 6) skill_name_list.insertLast(pk::encode("下一页"));

        int n = pk::choose(pk::encode("请选择要获得的特技."), skill_name_list);
        if (n == 5 and num != 6) return choose_skill_mid(skill_ids, 1);
        return skill_ids[n];
    }

    int choose_skill_mid(array<int> skill_ids, int page)
    {
        int num = int(skill_ids.length);
        int num2 = (num - 1 - 4 * page);
        if (num2 <= 5)
        {
          //最后一页
          array<string> skill_name_list;
          for (int i = (1 + page * 4); i < (1 + page * 4 + num2); ++i)
          {
            skill_name_list.insertLast(pk::get_skill(skill_ids[i]).name);
          }
          skill_name_list.insertLast(pk::encode("上一页"));
          int n = pk::choose(pk::encode("请选择要获得的特技."), skill_name_list);

          if (n == 5 or n == int(skill_name_list.length - 1))
          {
            if (page == 1) return choose_skill(skill_ids);
            return choose_skill_mid(skill_ids, page - 1);//如何返回上一页
          }

          int t = n + (page - 1) * 4 + 5;

          return skill_ids[t];
        }
        else//不是最后一页
        {
            array<string> skill_name_list;
            for (int i = (1 + page * 4); i < (1 + page * 4 + 4); ++i)
            {
              skill_name_list.insertLast(pk::get_ability(skill_ids[num]).name);
            }
            skill_name_list.insertLast(pk::encode("上一页"));
            skill_name_list.insertLast(pk::encode("下一页"));

            int n = pk::choose(pk::encode("请选择要获得的特技."), skill_name_list);

            //非最后一页情况
            if (n == 4)
            {
              if (page != 1) return choose_skill_mid(skill_ids, page - 1);
              else return choose_skill(skill_ids);
            }
            if (n == 5) return choose_skill_mid(skill_ids, page + 1);

            int t = n + (page - 1) * 4 + 5;

            return skill_ids[t];
        }
    }

    void 神术_狼顾权变_特技处理()
    {
      int current = pk::get_elapsed_days();
      for (int i = 0; i < 可用武将_末; i++)
      {
        sc_personinfo@ person_t = @person_sc[i];
        int times = int(person_t.狼顾权变_失去技能回合 - 10);
        if (person_t.狼顾权变_失去技能回合 > 0 and person_t.狼顾权变_技能失效编号 >= 0 and times < 游戏最大时间)
        {

          if (current > times and current - times >= 狼顾权变_技能失效时长 * 10)
          {
            if (person_t.狼顾权变_技能失效编号 < 特技_末)
            {
              pk::set_skill(pk::get_person(i), person_t.狼顾权变_技能失效编号);
              pk::say(pk::encode("总算回复了，恐怖如斯啊！！!"), pk::get_person(i));
              神术数据结构体::历史日志(pk::get_person(i), '神术_狼顾权变', pk::format("回复特技【\x1b[1x{}\x1b[0x】", pk::decode(pk::get_skill(person_t.狼顾权变_技能失效编号).name)));
              person_t.狼顾权变_技能失效编号 = -1;
              person_t.狼顾权变_失去技能回合 = 游戏最大时间;
            }
          }
        }
      }
    }

    void func_狼顾权变(pk::point dst_pos)
    {

      pk::unit@ unit = pk::get_unit(dst_pos);
      array<int> skill_ids = get_skill_id(unit);
      // array<string> skill_names = get_skill_name(unit);
      clear_skill(unit);
      // pk::wait(1000);
      int skill_id = choose_skill(skill_ids);
      string desc = pk::encode(pk::format("请选择获得特技\x1b[1x{}\x1b的武将", pk::get_skill(skill_id).name));
      auto person_list = pk::get_person_list(src_force, pk::mibun_flags(身份_君主, 身份_都督, 身份_太守, 身份_一般));
      bool person_confirm = false;
      pk::person@ monster;
      string monster_name = "";
      while (!person_confirm)
      {
        // 무장 선택 창을 열어서 선택하기
        auto person_sel = pk::person_selector(pk::encode("选择武将"), desc, person_list, 1, 1);
      if (person_sel.count == 0) return;
        // 무장 선택
        @monster = @person_sel[0];

        // 선택된 무장의 이름
        monster_name = pk::decode(pk::get_name(monster));

        person_confirm = pk::yes_no(pk::encode(pk::format("是否让\x1b[1x{}\x1b[0x特技变为【\x1b[1x{}\x1b】?", monster_name, pk::decode(pk::get_skill(skill_id).name))));
      }

      pk::say(pk::encode("精晓奇谋，诡变万策!"), src_leader);
      pk::set_skill(monster, skill_id);
      monster.update();
      string mibun_text;
      switch(person_司马懿.mibun)
      {
        case 身份_君主:
          mibun_text = '主公';
        case 身份_都督:
          mibun_text = '都督';
        default:
           mibun_text = '仲达';
      }
      pk::say(pk::encode(pk::format("{}真乃世之奇才也", mibun_text)), monster);

      神术数据结构体::历史日志(monster, '神术_狼顾权变', pk::format("获得特技【\x1b[1x{}\x1b[0x】", pk::decode(pk::get_skill(skill_id).name)));
    }

    bool handler_神术_狼顾权变(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩
      pk::wait(1000);
      func_狼顾权变(dst_pos);

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      return true;
    }

    // --------- 神术_火凤连环
    string getDesc_神术_火凤连环()
    {
      sc_personinfo@ person_t = @person_sc[src_leader.get_id()];
      if (person_t.火凤连环_使用)
        return pk::encode("本回合已经使用过.");
      else if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else
        return pk::encode(pk::format("每回合限一次,耗气{},{}格内敌方任一部队受到部队伤害时,其他部队同样受伤。", 计略气力消耗_通用, 火凤连环_效果范围));
    }
    bool isEnabled_神术_火凤连环()
    {
      sc_personinfo@ person_t = @person_sc[src_leader.get_id()];
      if (person_t.火凤连环_使用) return false;
      if (src_unit.energy < 计略气力消耗_通用) return false;
      return true;
    }

    void 设置连环状态部队(pk::point dst_pos)
    {
      array<pk::point> rings = pk::range(dst_pos, 0, 火凤连环_效果范围);
      // array<int> unit_ids;
      bool has_unit_effect = false;
      for (int i = 0; i < int(rings.length); i++)
      {
        pk::point target_pos = rings[i];
        pk::unit@ dst = pk::get_unit(target_pos);
        if (dst !is null and dst.get_force_id() != src_unit.get_force_id())
        {
          bool ally_effect = 火凤连环_盟军影响 || !src_force.ally[dst.get_force_id()];
          if (ally_effect)
          {
            has_unit_effect = true;
            sc_unitinfo@ sc_unit = @unit_sc[dst.get_id()];
            sc_unit.火凤连环影响 = true;
            pk::set_status(dst, src_unit, 部队状态_混乱, pk::rand(2), true);
            神术数据结构体::历史日志(dst, '神术_火凤连环', '陷入混乱');
          }
        }
      }
      if (has_unit_effect)
      {
        sc_personinfo@ sc_person = @person_sc[src_leader.get_id()];
        sc_person.火凤连环_使用 = true;
      }
    }

    void 火凤连环伤害(pk::unit@ unit, int value, int rettype)
    {
      sc_unitinfo@ sc_unit = @unit_sc[unit.get_id()];
      if (sc_unit.火凤连环影响)
      {
        for (int i = 0; i < 部队_末; i++)
        {
          sc_unitinfo@ sc_unit_t = @unit_sc[i];
          if (unit.get_id() != i and sc_unit_t.火凤连环影响)
          {
            pk::unit@ dst = pk::get_unit(i);
            if (value < 0 and rettype == 14)
            {
              ch::add_troops(dst, value, true);
              神术数据结构体::设置部队伤害值(src_unit, value);
              神术数据结构体::历史日志(dst, '神术_火凤连环', '受到了巨大伤害');
              pk::say(pk::encode("该死的，这是怎么回事"), pk::get_person(dst.leader));
              if (dst.troops <= 0) {
                pk::kill(dst);
              }
            }
          }
        }
      }
    }

    void 火凤连环清除()
    {
      for (int i = 0; i < 部队_末; i++)
      {
        sc_unitinfo@ sc_unit = @unit_sc[i];
        sc_unit.火凤连环影响 = false;
      }

      for (int i = 0; i < 可用武将_末; i++)
      {
        sc_personinfo@ person_t = @person_sc[i];
        if (person_t.火凤连环_使用) person_t.火凤连环_使用 = false;
      }
    }

    void 火凤连环部队溃灭处理(pk::unit@ unit)
    {
      sc_unitinfo@ sc_unit = @unit_sc[unit.get_id()];
      if (sc_unit.火凤连环影响) sc_unit.火凤连环影响 = false;
    }

    bool handler_神术_火凤连环(pk::point dst_pos)
    {

      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩
      设置连环状态部队(dst_pos);

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

     神术数据结构体::部队行动(src_unit);
      return true;
    }

    // --------- 神术_乱武完杀 ------------
    string getDesc_神术_乱武完杀()
    {
      int unit_length = get_乱武完杀部队().length;
      if (getEmptyPerson() is null)
        return pk::encode("暂无多余部队.");
      else if (unit_length > 0)
        return pk::encode(pk::format("还有{}支小分队存在", unit_length));
      else if (src_unit.energy < 计略气力消耗_乱武完杀)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("耗气{}，指定位置分兵{}支{}兵力小队。小队被攻击或破坏时，对周围造成巨大伤害", 计略气力消耗_通用, 乱武完杀_分队数量, 乱武完杀_分队兵力));
    }

    bool isEnabled_神术_乱武完杀()
    {
      if (getEmptyPerson() is null) return false;
      if (get_乱武完杀部队().length > 0) return false;
      if (src_unit.energy < 计略气力消耗_乱武完杀) return false;
      if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    bool valid_pos(pk::point dst_pos)
    {
      pk::hex@ hex = pk::get_hex(dst_pos);
      int terrain_id = hex.terrain;
      return pk::is_valid_pos(dst_pos) and !hex.has_building and !hex.has_unit
        and pk::is_valid_terrain_id(terrain_id)
        and pk::is_enabled_terrain(terrain_id)
        and terrain_id != 地形_山 and terrain_id != 地形_岸 and terrain_id != 地形_川 and terrain_id != 地形_未定 and terrain_id < 地形_末;
    }

    pk::person@ getEmptyPerson()
    {
      for (int i = 临时武将起始; i <= 临时武将终止; i++)
      {
        pk::person@ new_face = pk::get_person(i);
        switch (new_face.mibun)
        {
        case 身分_无:
        case 身分_未登场:
        case 身分_死亡:
          return new_face;
        }
      }
      return null;
    }

    bool 贾诩小兵(pk::unit@ unit)
    {
      for (int j = 0; j < 3; j++)
      {
          pk::person@ member_t = pk::get_person(unit.member[j]);
          if (pk::is_alive(member_t))
          {
            if (member_t.name_read == pk::encode("贾诩小队")) return true;
          }
      }
      return false;
    }

    void 贾诩小兵俘虏处理(const pk::destroy_info& in info, pk::list<pk::person@>& captured, pk::list<pk::person@>& escaped)
    {
      pk::hex_object@ attacker = info.attacker;
      pk::hex_object@ target = info.target;
      pk::unit@ target_unit = target.get_type_id() == pk::unit::type_id ? target : null;
      pk::unit@ attacker_unit = attacker.get_type_id() == pk::unit::type_id ? attacker : null;
      pk::list<pk::person@> prisoner_list;

      for (int i = 0; i < captured.size; i++)
      {
        pk::person@ person = captured[i];
        if (pk::is_alive(person) and person.name_read == pk::encode("贾诩小队"))
        {
          captured.remove_at(i);
          escaped.push_back(person);
        }
      }
      if (pk::is_alive(attacker_unit) and pk::get_person(attacker_unit.leader).name_read == pk::encode("贾诩小队"))
      {
        captured.clear();
      }
    }

    void setTempPerson(int person_id, int index)
    {
      pk::person@ farmer = pk::get_person(person_id);
      // pk::set_district(farmer, person_贾诩.get_district_id());

      farmer.name_read = pk::encode("贾诩小队");
      farmer.sex = 性别_男;
      farmer.sei = pk::encode("贾");
      farmer.mei = pk::encode(pk::format("诩{}", index));
      farmer.service = 建筑_末 - 1;
      farmer.face = 武将_卫兵;
      farmer.mibun = 身份_一般;
      farmer.loyalty = 100;
      farmer.birth = pk::get_year() - 30;
      farmer.death = pk::min(353, farmer.birth + 99);
      farmer.appearance = pk::get_year() - 10;
      farmer.kouseki = 1000;
      farmer.birthplace = person_贾诩.birthplace;
      farmer.skill = -1;
      farmer.wadai = person_贾诩.wadai;
      farmer.giri = person_贾诩.giri;
      farmer.ambition = person_贾诩.ambition;
      farmer.promotion = person_贾诩.promotion;
      farmer.character = person_贾诩.character;
      farmer.voice = person_贾诩.voice;
      farmer.tone = person_贾诩.tone;
      farmer.kanshitsu = person_贾诩.kanshitsu;
      farmer.strategic_tendency = person_贾诩.strategic_tendency;
      farmer.local_affiliation = person_贾诩.local_affiliation;

      //  ?空亲爱厌恶关系
      for (int i = 0;i < 5;i++)
      {
        farmer.liked[i] = person_贾诩.liked[i];
        farmer.disliked[i] = person_贾诩.disliked[i];
      }

      // 能力?
      for (int i = 0; i < 武将能力_末; i++)
      {
        farmer.base_stat[i] = random(10, 30);
        farmer.stat_aging[i] = random(能力成长_超持续型, 能力成长_张飞型);
      }

      // 适性
      for (int i = 0; i < 兵种_末; i++)
      {
        farmer.tekisei[i] = person_贾诩.tekisei[i];
      }

      // 무장 조형
      farmer.skeleton = person_贾诩.skeleton;
      farmer.body[0] = person_贾诩.body[0];
      farmer.body[1] = person_贾诩.body[1];
      farmer.body[2] = person_贾诩.body[2];
      farmer.body[3] = person_贾诩.body[3];
      farmer.body[4] = person_贾诩.body[4];
      farmer.body[5] = person_贾诩.body[5];

      //  話术
      farmer.wajutsu_daikatsu = person_贾诩.wajutsu_daikatsu;  //大喝
      farmer.wajutsu_kiben = person_贾诩.wajutsu_kiben;     //詭辯
      farmer.wajutsu_mushi = person_贾诩.wajutsu_mushi;     //无視
      farmer.wajutsu_chinsei = person_贾诩.wajutsu_chinsei;   //?静
      farmer.wajutsu_gyakujou = person_贾诩.wajutsu_gyakujou;  //憤怒

      farmer.update();
    }

    array<int> get_乱武完杀部队()
    {
      array<int> unit_ids;
      for (int i = 0; i < 部队_末; i++)
      {
        sc_unitinfo@ sc_unit = @unit_sc[i];
        if (sc_unit.乱武完杀部队) unit_ids.insertLast(i);
      }
      return unit_ids;
    }

    void 乱武完杀处理(pk::point dst_pos, int max_unit)
    {
      array<pk::point> rings = pk::range(dst_pos, 0, 100);
      array<int> unit_ids;
      for (int i = 0; i < int(rings.length); i++)
      {
        if (int(unit_ids.length) >= max_unit) break;
        pk::point target_pos = rings[i];
        if (!valid_pos(target_pos)) continue;
        if (getEmptyPerson() is null) break;
        pk::person@ temp_person = getEmptyPerson();
        pk::set_district(temp_person, person_贾诩.get_district_id());
        setTempPerson(temp_person.get_id(), unit_ids.length + 1);
        pk::unit@ unit = pk::create_unit(pk::get_building(person_贾诩.service), temp_person, null, null, 乱武完杀_分队兵力, src_unit.weapon, 兵器_走舸, 0, 乱武完杀_分队兵力, target_pos);
        unit.energy = 10;
        unit_ids.insertLast(unit.get_id());

        sc_unitinfo@ sc_unit = @unit_sc[unit.get_id()];
        sc_unit.乱武完杀部队 = true;
      }
    }

    void 乱武完杀部队受伤处理(pk::unit@ unit, int value, int rettype)
    {
      if (value < 0)
      {
        if (rettype >= 0 and rettype != 13 and int(unit.troops) > -value)
        {
          array<int> 完杀小队 = get_乱武完杀部队();
          if (完杀小队.find(unit.get_id()) >= 0)
          {
            array<pk::point> rings = pk::range(unit.get_pos(), 1, 1);
            for (int i = 0; i < int(rings.length); i++)
            {
              pk::point dst_pos = rings[i];
              pk::unit@ dst = pk::get_unit(dst_pos);
              pk::force@ 贾诩势力 = pk::get_force(person_贾诩.get_force_id());
              if (dst !is null and dst.get_force_id() != person_贾诩.get_force_id())
              {
                bool ally_effect = 乱武完杀_盟军影响 || !贾诩势力.ally[dst.get_force_id()];
                if (ally_effect)
                {
                  int 最终伤害值 = int(random(1, 2) * value);
                  ch::add_troops(dst, 最终伤害值, true);
                  神术数据结构体::设置部队伤害值(src_unit, -最终伤害值);
                  神术数据结构体::历史日志(dst, '神术_乱武完杀', '受到了反伤');
                  pk::say(pk::encode("这是反伤吗？"), pk::get_person(dst.leader));
                  if (dst.troops <= 0) {
                    pk::kill(dst, src_unit);
                  }
                }
              }
            }
          }
        }
      }
    }

    void 乱武完杀部队溃灭处理(pk::unit@ unit, pk::hex_object@ hex_object, int type)
    {
      array<int> 完杀小队 = get_乱武完杀部队();
      if (完杀小队.find(unit.get_id()) >= 0)
      {
        if (type == 0)
        {
          array<pk::point> rings = pk::range(unit.get_pos(), 1, 1);
          for (int i = 0; i < int(rings.length); i++)
          {
            pk::point dst_pos = rings[i];
            pk::unit@ dst = pk::get_unit(dst_pos);
            pk::force@ 贾诩势力 = pk::get_force(person_贾诩.get_force_id());
            if (dst !is null and dst.get_force_id() != person_贾诩.get_force_id())
            {
              bool ally_effect = 乱武完杀_盟军影响 || !贾诩势力.ally[dst.get_force_id()];
              if (ally_effect)
              {
                int 最终伤害值 = int(random(10, 100) * 100);
                ch::add_troops(dst, -最终伤害值, true);
                神术数据结构体::设置部队伤害值(src_unit, 最终伤害值);
                神术数据结构体::历史日志(dst, '神术_乱武完杀', '受到了巨大伤害');
                pk::say(pk::encode("不好，中计了！"), pk::get_person(dst.leader));
                if (dst.troops <= 0) {
                  pk::kill(dst, src_unit);
                }
              }
            }
          }
        }
        sc_unitinfo@ sc_unit = unit_sc[unit.get_id()];
        sc_unit.乱武完杀部队 = false;
      }
    }
    bool handler_神术_乱武完杀(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      int max_unit = 乱武完杀_分队数量;
      乱武完杀处理(dst_pos, max_unit);
      ch::add_troops(src_unit, max_unit * -乱武完杀_分队兵力, true);

      pk::add_energy(src_unit, -计略气力消耗_乱武完杀, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("吾之所好，杀人诛心。"), src_leader);

      神术数据结构体::部队行动(src_unit);
      return true;
    }

    // ------------ 神术_奇谋诡策 -----------
    string getDesc_神术_奇谋诡策()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("耗气{}，令{}格内我方部队气力回满，兵力回一半，并且本回合攻击敌方部队时一定概率斩将", 计略气力消耗_通用, 奇谋诡策_效果范围));
    }
    bool isEnabled_神术_奇谋诡策()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void fun_奇谋诡策(pk::point dst_pos)
    {
      array<pk::point> rings = pk::range(dst_pos, 0, 奇谋诡策_效果范围);
      for (int i = 0; i < int(rings.length); i++)
      {
        pk::point target_pos = rings[i];
        if (!pk::is_valid_pos(target_pos)) continue;
        pk::unit@ dst = pk::get_unit(target_pos);
        if (dst !is null)
        {
          if (dst.get_force_id() == person_法正.get_force_id())
          {
            if (dst.get_id() == src_unit.get_id()) continue;
            sc_unitinfo@ sc_unit = unit_sc[dst.get_id()];
            sc_unit.奇谋诡策影响 = true;
            pk::add_energy(dst, int(pk::core["部队.熟练兵最大气力"]), true);
            ch::add_troops(dst, dst.troops, true);
            pk::say(pk::encode("多谢孝直大人"), pk::get_person(dst.leader));
            神术数据结构体::历史日志(dst, '神术_奇谋诡策', '回复了气力跟兵士');
          }
          else
          {
            pk::set_status(dst, src_unit, 部队状态_伪报, pk::rand(2), true);
            神术数据结构体::历史日志(dst, '神术_奇谋诡策', '陷入了伪报');
          }
        }
      }
    }

    void 神术_奇谋诡策_伤害处理(pk::unit@ dst_unit, pk::unit@ unit)
    {
      sc_unitinfo@ sc_unit = unit_sc[unit.get_id()];
      if (sc_unit.奇谋诡策影响 and dst_unit !is null)
      {
        for (int m = 0; m < 3; m++)
        {
          if (pk::is_valid_person_id(dst_unit.member[m]))
          {
            const int rand = random(0, 100);
            pk::person@ member_t = pk::get_person(dst_unit.member[m]);
            if (member_t is null || !pk::is_alive(member_t)) continue;
            if (rand <= 80) { // 80%受伤
              if (member_t.shoubyou != 3)
              {
                member_t.shoubyou += 1;
              }
              else if (pk::rand_bool(50))
              {
                pk::kill(member_t, pk::get_person(unit.leader));
                神术数据结构体::历史日志(member_t, '神术_奇谋诡策', '死了');
                if (m == 0) pk::kill(dst_unit, unit);
              }
            }
            else if (rand > 80 and rand <= 98 and member_t.mibun != 身份_君主) // 28% 受伤
            {
              pk::say(pk::encode("这是妖术！"), member_t);
              member_t.former_force = member_t.get_force_id();
              pk::set_district(member_t, (pk::get_person(pk::get_kunshu_id(member_t))).get_district_id());
              member_t.mibun = 身份_俘虏;
              member_t.location = pk::get_person(unit.leader).location;
              pk::building@ building = pk::get_building(pk::get_service(unit));
              pk::set_service(member_t, building.get_id());
              神术数据结构体::历史日志(member_t, '神术_奇谋诡策', '变为俘虏了');
              if (m == 0) // 主将被俘，部队解散
              {
                pk::kill(dst_unit, unit);
              }
              else
              {
                if (m == 1) {
                  dst_unit.member[1] = dst_unit.member[2];
                  dst_unit.member[2] = -1;
                  m -= 1;
                }
                if (m == 2) {
                  dst_unit.member[2] = -1;
                }
                pk::update_unit(dst_unit);
              }
            }
            else
            {
              pk::kill(member_t, pk::get_person(unit.leader));
              神术数据结构体::历史日志(member_t, '神术_奇谋诡策', '死亡');
              if (m == 0) pk::kill(dst_unit, unit);
            }
          }
        }
      }
    }

    void 奇谋诡策清除()
    {
      for (int i = 0; i < 部队_末; i++)
      {
        sc_unitinfo@ sc_unit = unit_sc[i];
        sc_unit.奇谋诡策影响 = false;
      }
    }

    bool handler_神术_奇谋诡策(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      fun_奇谋诡策(dst_pos);

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("虚实不定，避重就轻，以眩远近。"), src_leader);

      神术数据结构体::部队行动(src_unit);
      return true;
    }

    // ---- 神术_帷幄奇策
    string getDesc_神术_帷幄奇策()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("耗气{}，令{}格内我方部队满气，状态正常，获得特技攻心，昂扬，激励，巧变，百战", 计略气力消耗_通用, 帷幄奇策_效果范围));
    }
    bool isEnabled_神术_帷幄奇策()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    int 帷幄奇谋_计略气力消耗(pk::unit@ dst, int energy)
    {
      sc_unitinfo@ sc_unit = @unit_sc[dst.get_id()];
      const bool 部队受帷幄奇策影响 = (pk::get_elapsed_days() >= int(sc_unit.帷幄奇策_禁法回合 - 10) and pk::get_elapsed_days() - (sc_unit.帷幄奇策_禁法回合 - 10) <= 30) ;
      if (部队受帷幄奇策影响) return 200;
      return energy;
    }

    int 帷幄奇谋_战法气力消耗(pk::unit@ dst, int energy)
    {
      sc_unitinfo@ sc_unit = @unit_sc[dst.get_id()];
      const bool 部队受帷幄奇策影响 = (pk::get_elapsed_days() >= int(sc_unit.帷幄奇策_禁法回合 - 10) and pk::get_elapsed_days() - (sc_unit.帷幄奇策_禁法回合 - 10) <= 30) ;
      if (部队受帷幄奇策影响) return 200;
      return energy;
    }

    void fun_帷幄奇策(pk::point dst_pos)
    {
      array<pk::point> rings = pk::range(dst_pos, 0, 帷幄奇策_效果范围);
      for (int i = 0; i < int(rings.length); i++)
      {
        pk::point target_pos = rings[i];
        if (!pk::is_valid_pos(target_pos)) continue;
        pk::unit@ dst = pk::get_unit(target_pos);
        if (dst !is null)
        {

          if (dst.get_force_id() == src_leader.get_force_id())
          {
            if (dst.get_id() == src_unit.get_id()) continue;
            pk::set_status(dst, src_unit, 部队状态_通常, 0, true);
            pk::add_energy(dst, int(pk::core["部队.熟练兵最大气力"]), true);
            for (int m = 0; m < 3; m++)
            {
              if (pk::is_valid_person_id(dst.member[m]))
              {
                pk::person@ member_t = pk::get_person(dst.member[m]);  //隊伍中的武將
                if (member_t is null or !pk::is_alive(member_t)) continue;
                if (member_t.shoubyou > 伤病_健康)
                {
                  member_t.shoubyou = 伤病_健康;
                  神术数据结构体::历史日志(member_t, '神术_帷幄奇策', '回复健康了');
                }
              }
            }
            pk::person@ dst_leader = pk::get_person(dst.leader);
            pk::say(pk::encode("多谢公达大人"), dst_leader);
            pk::set_action_done(dst, false);
            sc_personinfo@ sc_person = @person_sc[dst.leader];
            array<string> skill_names;
            string msg;
            if (!ch::has_skill(dst_leader, 特技_霸王))
            {
              pk::core::add_hidden_skill(dst.leader, 特技_霸王);
              sc_person.特技_霸王 = true;
              sc_person.帷幄奇策_技能获得回合 = pk::get_elapsed_days() + 10;
              msg += pk::format("【\x1b[1x{}\x1b[0x】，", '霸王');
            }
            if (!ch::has_skill(dst_leader, 特技_百战))
            {
              pk::core::add_hidden_skill(dst.leader, 特技_百战);
              sc_person.特技_百战 = true;
              sc_person.帷幄奇策_技能获得回合 = pk::get_elapsed_days() + 10;
              msg += pk::format("【\x1b[1x{}\x1b[0x】，", '百战');
            }
            if (!ch::has_skill(dst_leader, 特技_巧变))
            {
              pk::core::add_hidden_skill(dst.leader, 特技_巧变);
              sc_person.特技_巧变 = true;
              sc_person.帷幄奇策_技能获得回合 = pk::get_elapsed_days() + 10;
              msg += pk::format("【\x1b[1x{}\x1b[0x】，", '巧变');
            }
            if (!ch::has_skill(dst_leader, 特技_激励))
            {
              pk::core::add_hidden_skill(dst.leader, 特技_激励);
              sc_person.特技_激励 = true;
              sc_person.帷幄奇策_技能获得回合 = pk::get_elapsed_days() + 10;
              msg += pk::format("【\x1b[1x{}\x1b[0x】", '激励');
            }

            神术数据结构体::历史日志(pk::get_person(dst.leader), '神术_帷幄奇策', pk::format("获得特技{}", msg));
          }
          else
          {
            sc_unitinfo@ sc_unit = @unit_sc[dst.get_id()];
            sc_unit.帷幄奇策_禁法回合 = pk::get_elapsed_days() + 10;
            pk::say(pk::encode("不好，用不出战法跟计策了"), pk::get_person(dst.leader));
          }
        }
      }
    }

    // void 神术_帷幄奇策_伤害处理(pk::unit@ dst_unit, pk::unit@ unit, int troops_damage)
    // {

    //   sc_personinfo@ sc_person = @person_sc[unit.leader];
    //   const bool 部队受帷幄奇策影响 = (pk::get_elapsed_days() >= (sc_person.帷幄奇策_技能获得回合 - 10)) and (pk::get_elapsed_days() - (sc_person.帷幄奇策_技能获得回合 - 10) <= 30);
    //   if (部队受帷幄奇策影响) ch::add_troops(unit, int(troops_damage * 2), true);
    // }

    int 帷幄奇谋_战法成功率(pk::unit@ unit, int success_chance)
    {
      sc_personinfo@ sc_person = @person_sc[unit.leader];
      const bool 部队受帷幄奇策影响 = (pk::get_elapsed_days() >= (sc_person.帷幄奇策_技能获得回合 - 10)) and (pk::get_elapsed_days() - (sc_person.帷幄奇策_技能获得回合 - 10) <= 30);
      if (部队受帷幄奇策影响) return 100;
      return success_chance;
    }

    void 帷幄奇策部队溃灭处理(pk::unit@ unit, pk::hex_object@ hex_object, int type)
    {
      sc_personinfo@ sc_person = @person_sc[unit.leader];
      sc_unitinfo@ sc_unit = @unit_sc[unit.get_id()];
      const bool 敌方部队受帷幄奇策影响 = (pk::get_elapsed_days() >= int(sc_unit.帷幄奇策_禁法回合 - 10)) and (pk::get_elapsed_days() - (sc_unit.帷幄奇策_禁法回合 - 10) <= 30) ;
      const bool 已方部队受帷幄奇策影响 = (pk::get_elapsed_days() >= int(sc_person.帷幄奇策_技能获得回合 - 10)) and (pk::get_elapsed_days() - (sc_person.帷幄奇策_技能获得回合 - 10) <= 30);

      if (已方部队受帷幄奇策影响) {
        if (sc_person.特技_霸王)
        {
          sc_person.特技_霸王 = false;
          sc_person.帷幄奇策_技能获得回合 = 游戏最大时间;
          pk::core::remove_hidden_skill(unit.leader, 特技_霸王);
        }
        if (sc_person.特技_百战)
        {
          sc_person.特技_百战 = false;
          sc_person.帷幄奇策_技能获得回合 = 游戏最大时间;
          pk::core::remove_hidden_skill(unit.leader, 特技_百战);
        }
        if (sc_person.特技_巧变)
        {
          sc_person.特技_巧变 = false;
          sc_person.帷幄奇策_技能获得回合 = 游戏最大时间;
          pk::core::remove_hidden_skill(unit.leader, 特技_巧变);
        }
        if (sc_person.特技_激励)
        {
          sc_person.特技_激励 = false;
          sc_person.帷幄奇策_技能获得回合 = 游戏最大时间;
          pk::core::remove_hidden_skill(unit.leader, 特技_激励);
        }
      }
      if (敌方部队受帷幄奇策影响)
      {
        sc_unit.帷幄奇策_禁法回合 = 游戏最大时间;
      }
    }

    void 神术_帷幄奇策_特技处理()
    {
      int current = pk::get_elapsed_days();
      for (int i = 0; i < 部队_末; i++)
      {
        pk::unit@ unit = pk::get_unit(i);
        if (unit !is null and unit.leader >= 0)
        {
          sc_personinfo@ sc_person = @person_sc[unit.leader];
          if (pk::get_elapsed_days() >= (sc_person.帷幄奇策_技能获得回合 - 10) and pk::get_elapsed_days() - (sc_person.帷幄奇策_技能获得回合 - 10) > 30)
          {
            string msg;
            if (sc_person.特技_霸王)
            {
              sc_person.特技_霸王 = false;
              sc_person.帷幄奇策_技能获得回合 = 游戏最大时间;
              pk::core::remove_hidden_skill(unit.leader, 特技_霸王);
              msg += pk::format("【\x1b[1x{}\x1b[0x】，", '霸王');
            }
            if (sc_person.特技_百战)
            {
              sc_person.特技_百战 = false;
              sc_person.帷幄奇策_技能获得回合 = 游戏最大时间;
              msg += pk::format("【\x1b[1x{}\x1b[0x】，", '百战');
              pk::core::remove_hidden_skill(unit.leader, 特技_百战);
            }
            if (sc_person.特技_巧变)
            {
              sc_person.特技_巧变 = false;
              sc_person.帷幄奇策_技能获得回合 = 游戏最大时间;
              msg += pk::format("【\x1b[1x{}\x1b[0x】，", '巧变');
              pk::core::remove_hidden_skill(unit.leader, 特技_巧变);
            }
            if (sc_person.特技_激励)
            {
              sc_person.特技_激励 = false;
              sc_person.帷幄奇策_技能获得回合 = 游戏最大时间;
              msg += pk::format("【\x1b[1x{}\x1b[0x】", '激励');
              pk::core::remove_hidden_skill(unit.leader, 特技_激励);
            }

            if (!msg.isEmpty())
            {
              神术数据结构体::历史日志(pk::get_person(unit.leader), '神术_帷幄奇策', pk::format("失去特技{}", msg));
            }

          }

          sc_unitinfo@ sc_unit = @unit_sc[i];
          if ((pk::get_elapsed_days() > int(sc_unit.帷幄奇策_禁法回合 - 10)) and (current - (sc_unit.帷幄奇策_禁法回合 - 10) > 30))
          {
            sc_unit.帷幄奇策_禁法回合 = 游戏最大时间;
          }
        }
      }
    }

    bool handler_神术_帷幄奇策(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      fun_帷幄奇策(dst_pos);

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("虚实不定，避重就轻，以眩远近。"), src_leader);

      神术数据结构体::部队行动(src_unit);
      return true;
    }

    // -------------- 神术_无间业火
    string getDesc_神术_无间业火()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
        return pk::encode(pk::format("耗气{}，对3格内任一部队使用，该部队跟相邻的部队跟相邻部队的相邻部队着火", 计略气力消耗_通用));
    }

    bool isEnabled_神术_无间业火()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void 无间业火部队伤害处理(pk::damage_info& info, pk::unit@ attacker, pk::hex_object@ target, bool critical)
    {
      bool has_effect = 无间业火_影响部队.exists(formatInt(target.get_id()));
      array<int> 武将列表 = { 武将_陆逊 };
      if (has_effect and 武将列表.find(attacker.leader) >= 0)
      {
        info.troops_damage *= 无间业火_火伤倍率;
      }
    }

    void func_无间业火(pk::point dst_pos)
    {
      pk::unit@ dst = pk::get_unit(dst_pos);

      if (!无间业火_影响部队.exists(formatInt(dst.get_id())))
      {
        无间业火_影响部队.set(formatInt(dst.get_id()), dictionary = {});
      }

      pk::create_fire(dst_pos, pk::rand(2) + 1, src_unit, true); //火计
      pk::wait(16);
      array<pk::point> rings = pk::range(dst_pos, 1, 1);
      for (int i = 0; i < int(rings.length); i++)
      {
        pk::point neighbor_pos = rings[i];
        if (!pk::is_valid_pos(neighbor_pos)) continue;
        pk::unit@ neighbor_unit = pk::get_unit(neighbor_pos);
        if (neighbor_unit !is null)
        {
          const bool has_effect = cast<dictionary>(无间业火_影响部队[formatInt(dst.get_id())]).exists(formatInt(i));
          if (!has_effect)
          {
            cast<dictionary>(无间业火_影响部队[formatInt(dst.get_id())]).set(formatInt(i), true);
            func_无间业火(neighbor_pos);
          }
        }
      }

      for (int m = 0; m < 3; m++)
      {
        if (pk::is_valid_person_id(dst.member[m]))
        {
          pk::person@ member_t = pk::get_person(dst.member[m]);  //隊伍中的武將
          if (member_t is null || !pk::is_alive(member_t)) continue;
          member_t.death -= random(1, 2);
          if (member_t.death - member_t.birth <= 0) { // 生命小于等于0，死亡预定
            member_t.estimated_death = true;
            if (m == 0)
            {
              pk::kill(dst);
            }
            member_t.update();
            pk::kill(member_t);
            dst.update();
          }
        }
      }
    }

    void 无间业火清除()
    {
      for (int i = 0; i < 部队_末; i++)
      {
        无间业火_影响部队.delete(formatInt(i));
      }
    }


    bool handler_神术_无间业火(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(122,1000);

      func_无间业火(dst_pos);

      无间业火清除();

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("生生不息，源源不绝!"), src_leader);

      神术数据结构体::部队行动(src_unit);
      return true;
    }

    // ----------- getDesc_神术_握筹天下

    string getDesc_神术_握筹天下()
    {
      if (src_unit.energy < 计略气力消耗_通用)
        return pk::encode("气力不足.");
      else if (src_unit.troops < 兵力条件_通用)
        return pk::encode(pk::format("兵力不足,兵力至少{}", 兵力条件_通用));
      else
         return pk::encode(pk::format("耗气{},令{}格内我方部队武将任一五维+1, 敌方武将相互攻击，溃灭则一定几率下野", 计略气力消耗_通用, 握筹天下_效果范围));
    }

    bool isEnabled_神术_握筹天下()
    {
      if (src_unit.energy < 计略气力消耗_通用) return false;
      if (src_unit.troops < 兵力条件_通用) return false;
      return true;
    }

    void func_握筹天下_部队攻击(pk::unit@ unit)
    {
      array<pk::point> rings = pk::range(unit.get_pos(), 1, 1);
      for (int i = 0; i < int(rings.length); i++)
      {
        // if (unit is null) break;
        pk::point neighbor_pos = rings[i];
        if (!pk::is_valid_pos(neighbor_pos)) continue;
        pk::unit@ neighbor_unit = pk::get_unit(neighbor_pos);
        if (neighbor_unit !is null and unit !is null and neighbor_unit.get_force_id() != src_leader.get_force_id() )
        {
          array<int> attack_obj = { unit.get_id(), neighbor_unit.get_id() };
          握筹天下_影响部队.insertLast(attack_obj);
          握筹天下_效果部队.insertLast(unit.get_id());
        }
      }
    }

    void 握筹天下部队溃灭处理(pk::unit@ unit, pk::hex_object@ hex_object, int type)
    {
      pk::unit@ attack = pk::hex_object_to_unit(hex_object);
      if (attack !is null)
      {
        // string attack_id = formatInt(attack.get_id());
        bool has_effect_unit = false;
        for (int i = 0; i < int(握筹天下_效果部队.length); i++)
        {
          if (握筹天下_效果部队[i] == attack.get_id()) has_effect_unit = true;
        }
        if (type == 0 and has_effect_unit)
        {
          for (int m = 0; m < 3; m++)
          {
            pk::person@ member_t = pk::get_person(attack.member[m]);
            if (member_t is null or !pk::is_alive(member_t)) continue;
            if (member_t.mibun == 身份_君主) continue;
            int chance = int((person_荀彧.base_stat[武将能力_智力] - member_t.base_stat[武将能力_智力]) * 2);
            if (pk::rand_bool(100))
            {
              if (m == 0) pk::kill(attack);
              pk::say(pk::encode(pk::format("我怎能对\x1b[1x{}\x1b[0x大人下如此狠手，无脸再见啊", pk::get_name(pk::get_person(unit.leader)))), member_t);
              pk::resign(member_t);
              member_t.banned_kunshu = pk::get_kunshu_id(attack);
              member_t.ban_timer = 3;
              member_t.update();
              神术数据结构体::历史日志(member_t, '神术_握筹天下', '下野了');
            }
          }
        }
      }
    }

    void 神术_握筹天下_伤害处理(pk::damage_info& info, pk::unit@ attacker, const pk::point& in target_pos, int type)
    {
      if (type == 1)
      {
        pk::unit@ base_attack = pk::get_unit(握筹天下_影响部队[0][0]);
        pk::unit@ base_target = pk::get_unit(握筹天下_影响部队[0][1]);

        pk::unit@ target = pk::get_unit(target_pos);

        if (target !is null and attacker.get_id() == base_attack.get_id() and target.get_id() == base_target.get_id())
        {
          握筹天下_影响部队.removeAt(0);
          for (int i = 0; i < int(握筹天下_影响部队.length); i++)
          {
            pk::unit@ next_attack = pk::get_unit(握筹天下_影响部队[i][0]);
            pk::unit@ next_target = pk::get_unit(握筹天下_影响部队[i][1]);
            if (next_attack !is null
              and next_target !is null
              and pk::is_alive(next_attack)
              and pk::is_alive(next_target)
              and next_attack.troops > 0
              and next_target.troops > 0
              )
            {
              pk::attack(next_attack, pk::get_hex_object(next_target.get_pos()));
              break;
            }
          }
        }
      }
    }

    void 神术_握筹天下_攻击处理(pk::unit@ unit, int type)
    {
      if (int(握筹天下_影响部队.length) > 0 and (type == 0 or type == 1))
      {
        pk::unit@ attack = pk::get_unit(握筹天下_影响部队[0][0]);
        // pk::unit@ target = pk::get_unit(握筹天下_影响部队[0][1]);

        if (attack.get_id() == unit.get_id())
        {
          握筹天下_影响部队.removeAt(0);

          if (int(握筹天下_影响部队.length) > 0)
          {
            for (int i = 0; i < int(握筹天下_影响部队.length); i++)
            {
              pk::unit@ next_attack = pk::get_unit(握筹天下_影响部队[i][0]);
              pk::unit@ next_target = pk::get_unit(握筹天下_影响部队[i][1]);
              if (next_attack !is null
                and next_target !is null
                and pk::is_alive(next_attack)
                and pk::is_alive(next_target)
                and next_attack.troops > 0
                and next_target.troops > 0
                )
              {
                pk::attack(next_attack, pk::get_hex_object(next_target.get_pos()));
                break;
              }
            }
          }
        }
      }
    }

    void func_握筹天下(pk::point dst_pos)
    {
      array<pk::point> rings = pk::range(dst_pos, 0, 握筹天下_效果范围);
      for (int i = 0; i < int(rings.length); i++)
      {
        pk::point target_pos = rings[i];
        if (!pk::is_valid_pos(target_pos)) continue;
        pk::unit@ dst = pk::get_unit(target_pos);
        if (dst !is null and dst.get_force_id() == src_leader.get_force_id())
        {
          array<string> stat_name = { '统率', '武力', '智力', '政治', '魅力' };
          if (dst.get_id() == src_unit.get_id()) continue;
          for (int m = 0; m < 3; m++)
          {
            if (pk::is_valid_person_id(dst.member[m]))
            {
              pk::person@ member_t = pk::get_person(dst.member[m]);  //隊伍中的武將
              if (member_t is null or !pk::is_alive(member_t)) continue;
              int stat = random(0, 武将能力_末 - 1);
              if (int(member_t.base_stat[stat]) < int(pk::core["person.max_stat"]))
              {
                member_t.base_stat[stat] = member_t.base_stat[stat] + 1;
                member_t.update();
                pk::say(pk::encode("多谢文若大人"), member_t);
                神术数据结构体::历史日志(member_t, '神术_握筹天下', pk::format("\x1b[1x{}\x1b[0x上升1", stat_name[stat]));
              }
            }
          }
        }
      }
      pk::wait(100);
      for (int i = 0; i < int(rings.length); i++)
      {
        pk::point target_pos = rings[i];
        if (!pk::is_valid_pos(target_pos)) continue;
        pk::unit@ dst = pk::get_unit(target_pos);
        if (dst !is null and dst.get_force_id() != src_leader.get_force_id())
        {
          bool ally_effect = 握筹天下_盟军影响 || !src_force.ally[dst.get_force_id()];
          if (ally_effect)
          {
            pk::say(pk::encode("不好，控制不住自己了"), pk::get_person(dst.leader));
            func_握筹天下_部队攻击(dst);
          }
        }
      }
      if (int(握筹天下_影响部队.length) > 0)
      {
        pk::unit@ attack = pk::get_unit(握筹天下_影响部队[0][0]);
        pk::unit@ target = pk::get_unit(握筹天下_影响部队[0][1]);
        pk::attack(attack, pk::get_hex_object(target.get_pos()));
      }
    }

    bool handler_神术_握筹天下(pk::point dst_pos)
    {
      pk::play_se(120);
      pk::special_cutin(126,1000); // 妖术遮罩

      func_握筹天下(dst_pos);

      pk::add_energy(src_unit, -计略气力消耗_通用, true);

      pk::add_stat_exp(src_unit, 武将能力_智力, 20);

      pk::say(pk::encode("看我，二桃杀三士。!"), src_leader);
      握筹天下_效果部队.removeRange(0, 握筹天下_效果部队.length() - 1);

      神术数据结构体::部队行动(src_unit);
      return true;
    }

  }
  Main main;
}

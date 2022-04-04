// ## 2020/12/12 # 江东新风 # 修正参数类型错误 ##
// ## 2020/08/16 # 氕氘氚 # 添加部分常量 ##

const int 部队火伤_火种 = 300;
const int 部队火伤_火球 = 300;
const int 部队火伤_火船 = 400;
const int 部队火伤_火焰种 = 400;
const int 部队火伤_火焰球 = 400;
const int 部队火伤_业火种 = 500;
const int 部队火伤_业火球 = 500;
const int 部队火伤_练成炸药附加 = 300;
const int 部队落石伤害 = 1200;

const int 建筑火伤_火种 = 100;
const int 建筑火伤_火球 = 200;
const int 建筑火伤_火船 = 200;
const int 建筑火伤_火焰种 = 200;
const int 建筑火伤_火焰球 = 300;
const int 建筑火伤_业火种 = 300;
const int 建筑火伤_业火球 = 400;
const int 建筑火伤_练成炸药附加 = 100;
const int 建筑落石伤害 = 1200;

const int 部队落雷伤害 = 1500;
const int 部队落雷伤害_1 = 1000;
const int 建筑落雷伤害 = 700;
const int 建筑落雷伤害_1 = 300;


const int 百出耗气规则 = 1;		// 0：始终1， 1：减半，   2：自定义
const bool 兵力影响耗气 = true;
const bool 计略特技减少耗气 = true;

const int 气力减少量_精妙 = 3;
const int 气力减少量_秘计 = 5;

const int 计略耗气_火计 = 10;
const int 计略耗气_灭火 = 10;
const int 计略耗气_伪报 = 15;
const int 计略耗气_扰乱 = 15;
const int 计略耗气_镇静 = 10;
const int 计略耗气_伏兵 = 10;
const int 计略耗气_内讧 = 15;
const int 计略耗气_妖术 = 30;
const int 计略耗气_落雷 = 40;

const int 百出耗气_火计 = 5;
const int 百出耗气_灭火 = 5;
const int 百出耗气_伪报 = 7;
const int 百出耗气_扰乱 = 7;
const int 百出耗气_镇静 = 5;
const int 百出耗气_伏兵 = 5;
const int 百出耗气_内讧 = 7;
const int 百出耗气_妖术 = 15;
const int 百出耗气_落雷 = 20;



namespace batt
{
    // ---------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------


    //  计略气力消耗
    uint8 get_strtegy_energy_cost(pk::unit@ src_unit, int energy_cost, bool baichu = true)
    {
        uint8 n = uint8(energy_cost);
        if (baichu && src_unit.has_skill(특기_백출))
        {
            if (百出耗气规则 >= 1)
                n /= 2;
            else
                n = 1;
        }

        if (兵力影响耗气)
        {
            if (src_unit.troops <= 2000)
                n = uint8(n * pk::min(10.f, 2000.f / src_unit.troops));
            else if (src_unit.troops >= 10000)
                n = uint8(n * pk::max(0.5f, (2 - src_unit.troops / 10000.f)));
        }

        return n;
    }

    // 修正：火攻特技         【火攻】【火神】
    int fire_atk_skill_impact(int troops_damage, pk::unit@ src_unit)
    {
        if (src_unit !is null && pk::is_alive(src_unit))    // 攻击部队必須仍然存活
        {
            if (ch::has_skill(src_unit, 특기_화신))  //火神
                troops_damage = int(troops_damage * (火神伤害倍率 / 100.f));
            else if (ch::has_skill(src_unit, 특기_화공))  //火攻
                troops_damage = int(troops_damage * (火攻伤害倍率 / 100.f));
            return troops_damage;
        }
        else
            return 0;
    }

    // 修正：火防特技         【火攻】【火神】【藤甲】
    int fire_def_skill_impact(int troops_damage, pk::unit@ dst_unit)
    {
        if (ch::has_skill(dst_unit, 특기_등갑))  //藤甲
            troops_damage = int(troops_damage * (藤甲火伤倍率 / 100.f));
        else if (ch::has_skill(dst_unit, 특기_화신))  //火神
            troops_damage = int(troops_damage * (火神火伤倍率 / 100.f));
        else if (ch::has_skill(dst_unit, 특기_화공))  //火攻
            troops_damage = int(troops_damage * (火攻火伤倍率 / 100.f));
        // if (ch::has_skill(dst_unit, 특기_답파)) //踏破
        //     troops_damage *= 踏破火伤倍率 / 100.f;

        return troops_damage;
    }

    // 修正：全部火特技
    int get_fire_final_damage(int troops_damage, pk::unit@ src_unit, pk::unit@ dst_unit)
    {
        troops_damage = fire_atk_skill_impact(troops_damage, src_unit);
        troops_damage = fire_def_skill_impact(troops_damage, dst_unit);

        return troops_damage;
    }

}
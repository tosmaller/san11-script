// @@ 2021/07/10 @ 白马叔叔 @@
/***CUSTOM
@name 买卖指令-买卖粮食
@class 买卖
@author 白马叔叔
@contact
@version 0.0.210710
@date 2021/07/10
@desc 仅限小城和关买卖粮食。买粮基础倍率1金=3粮，卖粮基础倍率7粮=1金。可与商人讨价还价，成功后倍率提升1。如果执行武将魅力值较高，有概率无需舌战即可提高。交易上限为30000。执行武将政治越高，交易效果越好。卖粮食会收取2成税收，因此实际所得金会略少一些。
@notice
***/
//仅限小城和关买卖粮食。
//买粮基础倍率1金=3粮，卖粮基础倍率7粮=1金。可与商人讨价还价，成功后倍率提升1。如果执行武将魅力值较高，有概率无需舌战即可提高。交易上限为30000。执行武将政治越高，交易效果越好。卖粮食会收取2成税收，因此实际所得金会略少一些。

namespace 粮食买卖
{
    //---------------------------------------------------------------------------------------
    const int 高魅力值 = 85;    //高魅力阈值，超过这个魅力值有小概率讨价还价时不用舌战
	const int 高魅力概率 = 30;  //高魅力阈值，有这么高概率讨价还价时不用舌战
	const int ACTION_COST = 20;    // 行动力花费
	const int exp_food_sell = 5;      //粮食买卖获得的经验
	const array<int> food_sale_num = {3000, 6000, 10000, 15000, 20000, 30000};    //买卖粮食的数字
	array<string> food_sale_list = {"3000", "6000", "10000", "15000", "20000", "30000"};       //买卖粮食的列表
	
    //---------------------------------------------------------------------------------------
	
    class Main
    {
		pk::building@ building_;
		pk::force@ force_;
		pk::district@ district;
		pk::person@ actor_;
		pk::person@ merchant;
		string actor_name;
		
        Main()
        {
			pk::menu_item item_粮食买卖;
			item_粮食买卖.menu = global_menu::菜单_买卖;
			//item_粮食买卖.pos = 5;
			item_粮食买卖.init = pk::building_menu_item_init_t(init);
			item_粮食买卖.is_visible = pk::menu_item_is_visible_t(isVisible);
			item_粮食买卖.is_enabled = pk::menu_item_is_enabled_t(isEnabled);
			item_粮食买卖.get_image_id = pk::menu_item_get_image_id_t(getImageID_1249);
			item_粮食买卖.get_text = pk::menu_item_get_text_t(getText);
			item_粮食买卖.get_desc = pk::menu_item_get_desc_t(getDesc);
			item_粮食买卖.handler = pk::menu_item_handler_t(handler_粮食买卖);
			pk::add_menu_item(item_粮食买卖);
		}
		
		void init(pk::building@ building)
		{
			@building_ = @building;
			@force_ = pk::get_force(building.get_force_id());
			@district = pk::get_district(pk::get_district_id(force_, 1));
		}
		
		bool isVisible()
        {
            if (pk::is_campaign()) return false;  //决战称霸不可见
            if (building_.get_id() < 城市_末) return false;  //城市不可见，仅限城关
            return true;
        }
		
		bool isEnabled()
		{
			if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST) return false;      //行动力不足
			else if (pk::enemies_around(building_)) return false;    //周围存在敌人
			else if (getActorList().count == 0) return false;      //没有可执行的武将
			return true;
		}
		
		int getImageID_1249()
		{
			return 1249;
		}

		string getText()
		{
			return pk::encode("粮食买卖");
		}
		
		string getDesc()
		{
			if (pk::get_district(pk::get_district_id(force_, 1)).ap < ACTION_COST)
				return pk::encode("行动力不足");
			else if (pk::enemies_around(building_))
                return pk::encode("周围存在敌人.无法执行");
			else if (getActorList().count == 0)
				return pk::encode("没有可执行的武将");
			else
				return pk::encode("与商人进行粮食买卖交易(行动力 20)");
		}
		
		bool handler_粮食买卖()
		{
			int answer_0 = pk::choose({ pk::encode("  买粮  "), pk::encode("  卖粮  "), pk::encode("  返回  ") }, pk::encode("是否要与商人进行粮食交易?"), pk::get_person(무장_문관)) ;
			if (answer_0 == 2) return false;
			
			pk::list<pk::person@> list_0 = getActorList();
			pk::list<pk::person@> person_sel = pk::person_selector(pk::encode("武将选择"), pk::encode("选择执行粮食买卖的武将."), list_0, 1, 1);
			if (person_sel.count == 0) return false; // 未选择或取消结束
			
			@actor_ = person_sel[0];
			actor_name = pk::decode(pk::get_name(actor_));
			@merchant = pk::get_person(무장_상인);
			
			if (answer_0 == 0) pk::scene(pk::scene_t(scene_buy_food));   //买粮场景
			if (answer_0 == 1) pk::scene(pk::scene_t(scene_sell_food));   //卖粮场景
			
			pk::add_ap(district, -ACTION_COST);
			actor_.action_done = true;
			actor_.update();
			
			return true;
		}
		
		//买粮场景
		void scene_buy_food()
		{
			int ratio = 3;  //兑换比例

			int percent = int((actor_.stat[3]-50)/4 + 100);  //政治修正%
			//pk::trace(pk::format("percent：{}", percent));
			
			pk::message_box(pk::encode("这位大人买粮吗?今年灾害连连颗粒无收,\n现在金1=粮3."), merchant);
			bool chaffer = pk::yes_no(pk::encode("是否还价?"));
			if (chaffer)
			{
				if (actor_.stat[4] >= 高魅力值 and pk::rand_bool(高魅力概率))  //高魅力几率免除舌战
				{
					pk::message_box(pk::encode(pk::format("原来是\x1b[1x{}\x1b[0x大人啊!!\n我愿为您奉献一份力, 金1=粮4.", actor_name)), merchant);
					ratio = 4;
				}
				else
				{
					merchant.shoubyou = 0;  //恢复健康
					merchant.update();
					int debate_result = pk::debate(actor_, merchant, pk::is_player_controlled(actor_), false, true, true).first;
					if (debate_result == 0)
					{
						pk::message_box(pk::encode("原来如此,\n我愿为您奉献一份力, 金1=粮4."), merchant);
						ratio = 4;
					}
					else pk::message_box(pk::encode("如何, 还算公道吧?（呵, 徒有虚名..）"), merchant);
					merchant.shoubyou = 0;  //恢复健康
					merchant.update();
				}
			}
			for (int i = 0; i < 3; i++)
			{
				int n = pk::choose(pk::encode("请选择要买入的粮食数."), food_sale_list);
				int gold_needed = food_sale_num[n] * 100 / ratio / percent;    //得到的金钱数=卖出粮食÷粮价÷交易效果
				//pk::trace(pk::format("gold_needed：{}", gold_needed));
				if (gold_needed <= pk::get_gold(building_))   //如果所需金小于总金
				{
					pk::add_gold(building_, -gold_needed, true);
					pk::add_food(building_, food_sale_num[n], true);
					pk::play_se(6);
					pk::add_stat_exp(actor_, 3, exp_food_sell);
					switch (pk::rand(5))
					{
						case 0: pk::say(pk::encode("因为粮食是士兵们维生的资源啊。"), actor_, building_); break;
						case 1: pk::say(pk::encode(pk::format("确确实实地购入了粮食{}。", food_sale_num[n])), actor_, building_);  break;
						case 2: pk::say(pk::encode("虽说是枯燥乏味,\n但却是必要的工作呢。"), actor_, building_); break;
						case 3: pk::say(pk::encode("粮食的购入等等之类的事,\n简直是太简单了。"), actor_, building_); break;
						case 4: pk::say(pk::encode("购入粮食。"), actor_, building_); break;
					}
					break;
				}
				else
				{
					pk::message_box(pk::encode("额呃...大人的金不足购入如此之多的粮食."), merchant);
					if (i == 2)
						pk::message_box(pk::encode("大人何不再考虑考虑?"), merchant);
				}
			}
		}
		
		//卖粮场景
		void scene_sell_food()
		{
			int ratio = 7;  //兑换比例

			int percent = int((actor_.stat[3]-50)/4 + 100);  //政治修正%
			//pk::trace(pk::format("percent：{}", percent));
			
			pk::message_box(pk::encode("这位大人卖粮吗?今年五谷丰登粮食充盈,\n现在粮7=金1."), merchant);
			bool chaffer = pk::yes_no(pk::encode("是否还价?"));
			if (chaffer)
			{
				if (actor_.stat[4] >= 高魅力值 and pk::rand_bool(高魅力概率))  //高魅力几率免除舌战
				{
					pk::message_box(pk::encode(pk::format("原来是\x1b[1x{}\x1b[0x大人啊!!\n我愿为您奉献一份力, 粮6=金1.", actor_name)), merchant);
					ratio = 6;
				}
				else
				{
					merchant.shoubyou = 0;  //恢复健康
					merchant.update();
					int debate_result = pk::debate(actor_, merchant, pk::is_player_controlled(actor_), false, true, true).first;
					if (debate_result == 0)
					{
						pk::message_box(pk::encode("原来如此,\n我愿为您奉献一份力, 粮6=金1."), merchant);
						ratio = 6;
					}
					else pk::message_box(pk::encode("如何, 还算公道吧?（呵, 徒有虚名..）"), merchant);
					merchant.shoubyou = 0;  //恢复健康
					merchant.update();
				}
			}
			for (int i = 0; i < 3; i++)
			{
				int n = pk::choose(pk::encode("请选择要卖出的粮食数."), food_sale_list);
				int gold_get = food_sale_num[n] / ratio * percent * 8 / 1000;   //卖粮所得金钱=粮食数÷粮价×交易效果×0.8
				//pk::trace(pk::format("gold_get：{}", gold_get));
				if (food_sale_num[n] <= pk::get_food(building_))   //如果卖粮小于总粮食
				{
					pk::add_gold(building_, gold_get, true);
					pk::add_food(building_, -food_sale_num[n], true);
					pk::play_se(6);
					pk::add_stat_exp(actor_, 3, exp_food_sell);
					switch (pk::rand(5))
					{
						case 0: pk::say(pk::encode("由于卖出了粮食, 因而充实了财库。"), actor_, building_); break;
						case 1: pk::say(pk::encode(pk::format("卖出粮食{}。", food_sale_num[n])), actor_, building_);  break;
						case 2: pk::say(pk::encode("确确实实地卖出了粮食!"), actor_, building_); break;
						case 3: pk::say(pk::encode("商人们也将会十分地高兴啊。"), actor_, building_); break;
						case 4: pk::say(pk::encode("卖粮食之类的工作,\n还真是轻松愉快呀。"), actor_, building_); break;
					}
					break;
				}
				else
				{
					pk::message_box(pk::encode("额呃...大人的粮不足, 不妨少卖些."), merchant);
					if (i == 2)
						pk::message_box(pk::encode("大人何不再考虑考虑?"), merchant);
				}
			}
		}
		
		pk::list<pk::person@> getActorList()
		{
			pk::list<pk::person@> person_list = pk::get_idle_person_list(building_);
			person_list.sort(function(a, b)
			{
				return a.stat[3] > b.stat[3];  // 按政排序
			});
			return person_list;
		}

    }

    Main main;
}
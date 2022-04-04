// ## 2022/02/14 # 江东新风 # 部分常量中文化 ##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/08/09 #江东新风#排除身分?fu虏的结婚以及和君主母亲的结婚##
// ## 2020/08/01 ##
namespace AI_혼인
{
	class Main
	{
		Main()
		{
			pk::bind(111, pk::trigger111_t(callbackTurnStart));
		}

		int best_woman_id;
		int best_woman_score;
		int best_man_id;
		int best_man_score;

		void callbackTurnStart(pk::force@ force)
		{
			best_woman_id = -1;
			best_man_id = -1;
			best_woman_score = 0;
			best_man_score = 0;

			if (pk::is_iminzoku(pk::get_person(force.kunshu)) or pk::is_bandit(pk::get_person(force.kunshu)))
				return;
			if (force.player >= 0)
				return;

			pk::person@ kunshu = pk::get_person(force.kunshu);

			for (int i = 0; i < 무장_끝; i++)
			{
				pk::person@ person = pk::get_person(i);

				if (person.sex == 1 && person.get_force_id() == force.get_id() && person.spouse < 0 && person.mibun != 身分_捕虏)
				{
					if (best_woman_id < 0)
						best_woman_id = person.get_id();

					if (checkBest(force.kunshu, person.get_id(), true))
						best_woman_id = person.get_id();
				}
			}

			if (best_woman_id >= 0)
			{
				for (int j = 0; j < 武将_末; j++)
				{
					pk::person@ person = pk::get_person(j);
					if (person.get_force_id() == force.get_id() && person.sex == 0 && person.spouse < 0 && person.mibun != 身分_捕虏)
					{
						if (best_man_id < 0)
							best_man_id = person.get_id();

						if (checkBest(force.kunshu, person.get_id(), false))
							best_man_id = person.get_id();
					}
				}
			}

			int random1 = pk::rand(500);
			int random2 = pk::rand(100);

			if (best_woman_id >= 0 && best_man_id >= 0 && int(force.tp) >= 500 && int(kunshu.stat[武将能力_智力] + kunshu.stat[武将能力_政治]) > random1 && best_man_score > random2)
			{
				pk::person@ final_woman = pk::get_person(best_woman_id);
				pk::person@ final_man = pk::get_person(best_man_id);

				pk::play_se(10);
				pk::set_spouse(final_man, final_woman.get_id());
				pk::set_spouse(final_woman, final_man.get_id());
				pk::history_log(pk::get_current_turn_force_id(), force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x势力的\x1b[2x{}\x1b[0x与\x1b[2x{}\x1b[0x共结连理", pk::decode(pk::get_name(kunshu)), pk::decode(pk::get_name(final_man)), pk::decode(pk::get_name(final_woman)))));
			}
		}

		//무장 수치비교값 계산공식 = 능력 + 충성 + 공적 + 혈연 + 의형제 + 친애무장 + 랜덤 武将数据比較公式 = 能力 + 忠诚 + 功績 + 血缘 + 义兄弟 + 亲爱武将 + 随机
		//該武将最大能力?*2+平均能力?*3+功績/100+忠诚+君主血亲300+君主义兄弟200+君主亲爱武将100+随机1~200，女性最大能力大於90加100，小於81减100
		bool checkBest(int kunshu_id, int person_id, bool is_woman)
		{
			bool result = false;

			pk::person@ kunshu = pk::get_person(kunshu_id);
			pk::person@ person = pk::get_person(person_id);

			int score = 0;
			int max_stat = 0;
			int avg_stat = 0;

			for (int i = 0; i < 武将能力_末; i++)
			{
				int s = person.stat[i];
				max_stat = pk::max(max_stat, s);
				avg_stat = avg_stat + s;
			}
			avg_stat = avg_stat / 武将能力_末;

			//該武将最大能力?*2+平均能力?*3+功績/100+忠诚+君主血亲(父母子女)300+君主义兄弟200+君主亲爱武将100+随机1~200
			score += (max_stat * 2) + (avg_stat * 3);
			score += (person.kouseki / 100) + person.loyalty;

			if (pk::is_oyako(kunshu, person_id)) score += 300;
			if (pk::is_gikyoudai(kunshu, person_id)) score += 200;
			if (pk::is_like(kunshu, person_id)) score += 100;
			score += pk::rand(200);

			if (is_woman)
			{
				if (best_woman_score == 0)
					best_woman_score = score + 1;

				best_woman_score = score < best_woman_score ? score : best_woman_score;
				result = score < best_woman_score;
			}
			else if (best_woman_id >= 0)
			{
				pk::person@ best_woman_person = pk::get_person(best_woman_id);

				if (max_stat > 90)
					score += 100;

				if (max_stat < 81)
					score -= 100;

				int n = score;
				int g = best_woman_score + 200;

				if (pk::is_dislike(best_woman_person, person_id))
					result = false;
				if (best_woman_person.mibun == 身份_君主)
					g += 100 * best_woman_person.ambition;
				if (person.birth - best_woman_person.birth >= 10)
					g += 500;
				if (person.birth - best_woman_person.birth <= -20)
					g += 500;

				g -= 75 - (pk::get_aishou_distance(best_woman_person, person_id) * 10);
				g -= pk::rand(100);

				if (pk::is_like(best_woman_person, person_id))
					g -= 100 * best_woman_person.giri;
				if (person.character == best_woman_person.character)
					g -= 100;
				if (person.kanshitsu == best_woman_person.kanshitsu)
					g -= 100;

				result = n > g && (n - g) > best_man_score;

				//父母子女及血缘关系排除，年龄相差大於40排除，江东新风增加：排除君主母亲，排除fu虏
				if (pk::is_oyako(person, best_woman_person.get_id()) or pk::is_ketsuen(person, best_woman_person.get_id()))
					result = false;
				if (abs(person.birth - best_woman_person.birth) >= 40)
					result = false;
				if (best_woman_person.get_id() == kunshu.mother)
					result = false;
				if (best_woman_person.mibun == 身分_捕虏)
					result = false;

				if (best_man_id == person_id && result == false)
					best_man_id = -1;

				if (result) best_man_score = (n - g);
			}

			return result;
		}
	}

	Main main;
}
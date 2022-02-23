// ## 2020/10/01 # keehl102 ##


namespace 난수시드초기화
{
	class Main
	{
		Main()
		{
			pk::bind(102, pk::trigger102_t(randomize_seed));
		}

		void randomize_seed()
		{
			uint value = pk::get_tick_count();
			pk::get_scenario().seed = value;
			pk::printf("New seed: {}\n", value);
		}
	}

	Main main;
}
// ## 2021/11/14 # 江东新风 # 俘虏在部队中也会逃跑 ##
// ## 2021/10/01 # 江东新风 # namespace的韩文改成英文 ##
// ## 2020/07/26 ##
namespace PRISONER_ESCAPE_CHANCE
{
	//---------------------------------------------------------------------------------------
	const int n回合内不逃脱 = 2;
	//---------------------------------------------------------------------------------------
	class Main
	{
		Main()
		{
			pk::set_func(157, pk::func157_t(callback));
		}

		int callback(pk::person@ prisoner)
		{
			// 此处决定了部队中的俘虏不逃脱
			if (prisoner.location > -1 and prisoner.location < 1087)
			{
				pk::hex_object@ hex_object = pk::get_hex_object(prisoner.location);

				if (hex_object is null or !pk::is_alive(hex_object))
					return 0;
			}
			/*
			pk::building@ building = pk::hex_object_to_building(pk::get_hex_object(prisoner.location));
			if (!pk::is_alive(building))
				return 0;*/

			int n = prisoner.prisoner_timer;

			// 포로가 된지 2턴 미만인 경우 탈출하지 않음
			if (n < n回合内不逃脱)
				return 0;

			n = n - n回合内不逃脱;
			if (n < 1)
				n = 1;
			n = n * n * pk::max(prisoner.stat[武将能力_智力], prisoner.stat[武将能力_武力], 30) / 166;

			return pk::max(n, 1);
		}
	}

	Main main;
}
// ## 2021/09/15 # 江东新风 # 更改pk::core[]函数为英文##
// ## 2020/12/17 #江东新风# 修复自动合并失效bug ##
// ## 2020/12/12 #江东新风# 修复trace参数类型错误 ##
// ## 2020/07/26 ##
/*
@만든이: Tester
@버전: 1.0.6

*/
namespace 设施_自动吸收合并
{
	///////////////////////////////////////////////////////////////////////////////
	/////////////////////////////				///////////////////////////////////
	/////////////////////////////	CUSTOMIZE	///////////////////////////////////
	/////////////////////////////				///////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	const bool is_player = true;
	///////////////////////////////////////////////////////////////////////////////


	class Main
	{
		array<int> 市场2合并需求 = { 设施_市场1级, 设施_市场1级, -1, -1, -1, -1, -1 };
		array<int> 市场3合并需求 = { 设施_市场2级, 设施_市场1级, -1, -1, -1, -1, -1 };
		array<int> 农场2合并需求 = { 设施_农场1级, 设施_农场1级, -1, -1, -1, -1, -1 };
		array<int> 农场3合并需求 = { 设施_农场2级, 设施_农场1级, -1, -1, -1, -1, -1 };
		array<int> 兵营2合并需求 = { 设施_兵营1级, 设施_兵营1级, -1, -1, -1, -1, -1 };
		array<int> 兵营3合并需求 = { 设施_兵营2级, 设施_兵营1级, -1, -1, -1, -1, -1 };
		array<int> 锻冶2合并需求 = { 设施_锻冶1级, 设施_锻冶1级, -1, -1, -1, -1, -1 };
		array<int> 锻冶3合并需求 = { 设施_锻冶2级, 设施_锻冶1级, -1, -1, -1, -1, -1 };
		array<int> 厩舍2合并需求 = { 设施_厩舍1级, 设施_厩舍1级, -1, -1, -1, -1, -1 };
		array<int> 厩舍3合并需求 = { 设施_厩舍2级, 设施_厩舍1级, -1, -1, -1, -1, -1 };

		int 相邻地格数 = 7;

		Main()
		{
			initialize();
		}

		void initialize()
		{
			pk::bind(102, pk::trigger102_t(onScenarioInit));
			pk::bind(107, pk::trigger107_t(onNewDay));
		}


		void onScenarioInit() {

			// pk::core["disable_merge"] = true;			
			makeIngredientDesc();
		}

		void onNewDay()
		{
			LoopBuildings();
			//pk::trace("start LoopBuildings");			
		}

		void makeIngredientDesc()
		{
			市场2合并需求.sortDesc();
			市场3合并需求.sortDesc();
			农场2合并需求.sortDesc();
			农场3合并需求.sortDesc();
			兵营2合并需求.sortDesc();
			兵营3合并需求.sortDesc();
			锻冶2合并需求.sortDesc();
			锻冶3合并需求.sortDesc();
			厩舍2合并需求.sortDesc();
			厩舍3合并需求.sortDesc();
		}



		bool isValidUpgrade(pk::building@ b0)
		{
			if (!pk::is_alive(b0))return false;
			if (is_player and !b0.is_player())return false;
			if (pk::get_facility_type(b0) != 设施类型_内政设施)return false;
			if (!b0.completed)return false;
			if (isUpgradeDone(getFacilityID(b0)))return false;

			return true;
		}

		// 업그레이드가 끝난 건물인가?
		bool isUpgradeDone(int facility)
		{
			if (facility == 设施_市场3级)return true;
			if (facility == 设施_农场3级)return true;
			if (facility == 设施_兵营3级)return true;
			if (facility == 设施_锻冶3级)return true;
			if (facility == 设施_厩舍3级)return true;
			return false;
		}

		void LoopBuildings()
		{
			pk::list<pk::building@> buildings = pk::get_building_list();
			for (int i = 0; i < buildings.count; i++)
			{
				pk::building@ building = buildings[i];
				//pk::trace("check valid");					
				if (!isValidUpgrade(building))continue;

				//pk::trace(pk::format("start upgrade id:{}",i));	
				//pk::trace(pk::encode("LoopBuildings pos=["+building.pos+"] facility=["+building.facility+"] model=["+building.model+"] completed=["+building.completed+"] \n"));

				UpgradeBuilding(building);

			} // end - for			
			//pk::printf(pk::encode("-------------------------------------------------------------------- \n"));
		}

		/*
		// get Empty pos
		pk::hex@ hex = pk::get_hex(arr[i]);
		if (!hex.has_building and !hex.has_unit)

		auto obj = pk::get_hex_object(range[i]);
		if (obj !is null and obj.get_force_id() != force_id)

		pk::building@ building = pk::get_building(arr[i]);
		if (pk::is_alive(building) // 빌딩 유효성

		pk::unit@ dst = pk::get_unit(arr[i]);
		if (dst is null
		*/

		bool UpgradeBuilding(pk::building@ building0)
		{

			auto arr = pk::range(building0.get_pos(), 0, 1);
			array<pk::building@> 相邻建筑(相邻地格数, null);

			// 인접 내정시설 탐색
			for (int i = 0; i < int(arr.length); i++)
			{
				// if (!pk::get_hex(arr[i]).has_building)continue;
				pk::building@ b = pk::get_building(arr[i]);
				if (!isValidUpgrade(b))continue;

				@相邻建筑[i] = b;

				//pk::trace(pk::encode("UpgradeBuilding range i=["+i+"] building0=["+building0.get_pos()+"] facility_type=["+b.facility+"] pos=["+b.get_pos()+"] \n"));				
			}

			相邻建筑.sort(function(a, b) { return main.getFacilityID(a) > main.getFacilityID(b); });

			array<array<pk::point>> 位置_一致建筑; // 0번째 인덱스는 건설여부

			位置_一致建筑.insertLast(MatchIngredients(相邻建筑, building0, 市场2合并需求));
			位置_一致建筑.insertLast(MatchIngredients(相邻建筑, building0, 市场3合并需求));
			位置_一致建筑.insertLast(MatchIngredients(相邻建筑, building0, 农场2合并需求));
			位置_一致建筑.insertLast(MatchIngredients(相邻建筑, building0, 农场3合并需求));
			位置_一致建筑.insertLast(MatchIngredients(相邻建筑, building0, 兵营2合并需求));
			位置_一致建筑.insertLast(MatchIngredients(相邻建筑, building0, 兵营3合并需求));
			位置_一致建筑.insertLast(MatchIngredients(相邻建筑, building0, 锻冶2合并需求));
			位置_一致建筑.insertLast(MatchIngredients(相邻建筑, building0, 锻冶3合并需求));
			位置_一致建筑.insertLast(MatchIngredients(相邻建筑, building0, 厩舍2合并需求));
			位置_一致建筑.insertLast(MatchIngredients(相邻建筑, building0, 厩舍3合并需求));

			pk::int_int 设施2_材料索引 = getFacilityBuild(位置_一致建筑);

			int facility2_id = 设施2_材料索引.first;
			//pk::trace("设施2_材料索引1=["+设施2_材料索引.first+"设施2_材料索引2"+设施2_材料索引.second);	
			if (facility2_id == -1)return false; // 일치하는 건물이 있는가	

			int b0_forceId = building0.get_force_id();
			pk::point 建设位置 = building0.get_pos(); // 원래 건물	
			int 建设_facilityId = getFacilityID(building0);	// 원래 시설			
			if (facility2_id == 建设_facilityId)return false; // 자기와 같은 건물인가		

			pk::facility@ facility2 = pk::get_facility(facility2_id); // 지어질 시설


			//pk::trace("facilityId=["+建设_facilityId+"] building0.get_pos() =["+building0.get_pos()+"]");	

			/// 재료건물 철거
			array<pk::point> 拆除位置 = 位置_一致建筑[设施2_材料索引.second];

			array<pk::building@> arr_拆除建筑;
			for (int i = 1; i < int(拆除位置.length); i++) {
				// if(拆除位置[i].equal(建设位置))continue;
				if (!pk::get_hex(拆除位置[i]).has_building)continue;

				/// set 建设位置, 시설.
				auto 拆除建筑 = pk::get_building(拆除位置[i]);
				int 拆除_facilityId = getFacilityID(拆除建筑);
				//pk::trace("x:" + 拆除位置[i].x + "y:" + 拆除位置[i].y);
				//pk::trace("Comp _Pos=["+建设位置+"] id=["+建设_facilityId+"] VS Destroy_Pos=["+拆除位置[i]+"] id=["+拆除_facilityId+"]");				

				if (建设_facilityId < 拆除_facilityId) {
					建设位置 = 拆除位置[i];
					建设_facilityId = 拆除_facilityId;
				}
				// prnt("Sel _Pos=["+建设位置+"]");
				arr_拆除建筑.insertLast(拆除建筑);
			}

			for (int i = 0; i < int(arr_拆除建筑.length); i++) {
				// prnt("Remove building.pos=["+arr_拆除建筑[i].pos+"] facility=["+getFacilityID(arr_拆除建筑[i])+"]");
				pk::remove(arr_拆除建筑[i]);
			}
			// prnt("---------------------Done_Pos=["+建设位置+"]------------------------");

			/// 업그레이드 건물 건설
			pk::building@ building2 = pk::create_building(facility2, 建设位置, b0_forceId);
			pk::complete(building2);
			/// Effect
			if (pk::is_in_screen(建设位置)) {
				pk::play_se(0x20, 建设位置);
				pk::create_effect(0x61, 建设位置);
			}


			//pk::trace(pk::encode("UpgradeBuilding facility2=["+facility2.get_id()+"] pos=["+building2.get_pos()+"] force_id=["+building2.get_force_id()+"] \n"));

			return true;
		}



		array<pk::point> MatchIngredients(array<pk::building@> 相邻建筑, pk::building@ building0, array<int> 材料排序) {

			array<pk::point> 合并建筑位置(1, -1); // 0번째:매치여부(1:매치), 매치되면, 일치하는 건물목록

			// 재료에 포함된 시설인가
			int facility0 = getFacilityID(building0);
			bool isIngredient = false;
			for (int k = 0; k < int(材料排序.length); k++)
			{
				if (facility0 > 材料排序[k]) { isIngredient = false;break; }
				if (facility0 == 材料排序[k]) { isIngredient = true;break; }
			}
			if (!isIngredient)return 合并建筑位置;



			int iStart = 0;
			// prnt("----------------------------------------");
			for (int j = 0; j < 相邻地格数; j++)
			{
				int jj = 材料排序[j];
				if (jj == -1)return 合并建筑位置;

				for (int i = iStart; i < int(相邻建筑.length); i++)
				{

					// prnt("kunshu=["+pk::get_force(building0.get_force_id()).kunshu+"] jj=["+jj+"] ii=["+getFacilityID(相邻建筑[i])+"]");

					if (!pk::is_alive(相邻建筑[i])) { 合并建筑位置[0] = -1;break; }
					pk::building@ iBuilding = 相邻建筑[i];
					int ii = getFacilityID(iBuilding);

					if (ii == jj) { iStart = i + 1;合并建筑位置[0] = 1;合并建筑位置.insertLast(iBuilding.get_pos());break; }
					if (ii < jj) { 合并建筑位置[0] = -1; return 合并建筑位置; }

				}
			}

			return 合并建筑位置;
		}




		// return (id, index)  //获取的是将要升级的目标设施类型，以及索引位置
		pk::int_int getFacilityBuild(array<array<pk::point>> 位置_一致建筑)
		{
			for (int i = 0; i < int(位置_一致建筑.length); i++)
			{
				//pk::trace("位置_一致建筑[i][0]为"+位置_一致建筑[i][0]);
				if (位置_一致建筑[i][0].x == 1)return pk::int_int(getFacility2ID(i), i); //如果可以建设，可返还建设摊位和建筑材料索引(第一批或第二批)。 건설가능하면, 건설할건물과 건설건물재료인덱스(첫번째 혹은 두번째)반환.
			}
			return  pk::int_int(-1, -1);
		}


		// index: 건설여부 인덱스 根据索引获取目标设施id
		int getFacility2ID(int index) {
			switch (index)
			{

			case 0: return 设施_市场2级;
			case 1: return 设施_市场3级;
			case 2: return 设施_农场2级;
			case 3: return 设施_农场3级;
			case 4: return 设施_兵营2级;
			case 5: return 设施_兵营3级;
			case 6: return 设施_锻冶2级;
			case 7: return 设施_锻冶3级;
			case 8: return 设施_厩舍2级;
			case 9: return 设施_厩舍3级;

			}

			return -1;
		}
		int getFacilityID(pk::building@ b) {
			// prnt("b id=["+b.get_id()+"] is_valid_building_id=["+pk::is_valid_building_id(b.get_id())+"] is null=["+(b is null)+"] is_alive=["+pk::is_alive(b)+"] pos=["+b.pos+"]");
			if (b is null)return -1;
			if (!pk::get_hex(b.pos).has_building)return -1;
			return pk::get_building(b.pos).facility;
		}

		void prnt(string str)
		{
			pk::printf(pk::encode(str + " \n"));
		}

	} // end - Main





	Main main;

} // end - namespace
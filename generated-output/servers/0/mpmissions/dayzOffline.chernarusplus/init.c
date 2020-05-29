void main () {
	//INIT WEATHER BEFORE ECONOMY INIT------------------------
	Weather weather = g_Game.GetWeather ();

	weather.MissionWeather (false); // false = use weather controller from Weather.c

	weather.GetOvercast ().Set (Math.RandomFloatInclusive (0.4, 0.6), 1, 0);
	weather.GetRain ().Set (0, 0, 1);
	weather.GetFog ().Set (Math.RandomFloatInclusive (0.05, 0.1), 1, 0);

	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive ();
	if (ce)
		ce.InitOffline ();

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 9, reset_day = 20;
	GetGame ().GetWorld ().GetDate (year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day)) {
		GetGame ().GetWorld ().SetDate (year, reset_month, reset_day, hour, minute);
	} else {
		if ((month == reset_month + 1) && (day > reset_day)) {
			GetGame ().GetWorld ().SetDate (year, reset_month, reset_day, hour, minute);
		} else {
			if ((month < reset_month) || (month > reset_month + 1)) {
				GetGame ().GetWorld ().SetDate (year, reset_month, reset_day, hour, minute);
			}
		}
	}
}

class CustomMission : MissionServer {

	override PlayerBase CreateCharacter (PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName) {
		Entity playerEnt;
		playerEnt = GetGame ().CreatePlayer (identity, characterName, pos, 0, "NONE"); //Creates random player
		Class.CastTo (m_player, playerEnt);

		GetGame ().SelectPlayer (identity, m_player);

		return m_player;
	}

	EntityAI AddInventory (PlayerBase player, string item) {
		EntityAI itemEnt = player.GetInventory ().CreateInInventory (item);
		ItemBase itemBs = ItemBase.Cast (itemEnt);
		return itemEnt;
	}

	EntityAI AddHands (PlayerBase player, string item) {
		EntityAI itemEnt = player.GetHumanInventory ().CreateInHands (item);
		ItemBase itemBs = ItemBase.Cast (itemEnt);
		return itemEnt;
	}

	void AddAttachment (EntityAI item, string attachment) {
		item.GetInventory ().CreateAttachment (attachment);
	}

	void AddMultiple (EntityAI container, string item, int count) {
		for (int i = 0; i < count; i++) {
			AddInventory (container, item);
		}
	}

	void AddM4 (PlayerBase player, int shortcut) {
		EntityAI gun = AddInventory (player, "M4A1");
		AddAttachment (gun, "M4_MPHndgrd");
		AddAttachment (gun, "M4_MPBttstck_Black");
		AddAttachment (gun, "M4_CarryHandleOptic");
		player.SetQuickBarEntityShortcut (gun, shortcut, true);
	}

	void AddSniper (PlayerBase player, int shortcut) {
		EntityAI sniper = AddInventory (player, "Winchester70");
		player.SetQuickBarEntityShortcut (sniper, shortcut, true);
	}

	void AddOptics (PlayerBase player) {
		AddInventory (player, "HuntingOptic");
		AddInventory (player, "PUScopeOptic");
		AddInventory (player, "ACOGOptic");
		AddInventory (player, "PSO11Optic");
	}

	void AddMilitaryClothing (PlayerBase player) {
		EntityAI helmet = AddInventory (player, "Mich2001Helmet");
		AddAttachment (helmet, "NVGoggles");
		AddInventory (player, "SportGlasses_Black");
		AddInventory (player, "USMCJacket_Woodland");
		AddInventory (player, "USMCPants_Woodland");
		AddInventory (player, "TacticalGloves_Beige");
		EntityAI belt = AddInventory (player, "MilitaryBelt");
		AddAttachment (belt, "Canteen");
		AddAttachment (belt, "NylonKnifeSheath");
		AddAttachment (belt, "PlateCarrierHolster");
		EntityAI boots = AddInventory (player, "MilitaryBoots_Beige");
		EntityAI vest = AddInventory (player, "PlateCarrierVest");
		AddAttachment (vest, "PlateCarrierPouches");
		AddAttachment (vest, "PlateCarrierHolster");
	}

	void AddFNX45 (PlayerBase player, int shortcut) {
		EntityAI gun = AddInventory (player, "FNX45");
		player.SetQuickBarEntityShortcut (gun, shortcut, true);
	}

	override void StartingEquipSetup (PlayerBase player, bool clothesChosen) {
		player.RemoveAllItems ();

		int shortcut = 0;

		AddM4 (player, shortcut++);
		AddSniper (player, shortcut++);
		AddMilitaryClothing (player);
		AddFNX45 (player, shortcut++);

		// Bags
		// EntityAI bag = AddInventory(player, "AliceBag_Camo");

		// Ammo
		AddMultiple (player, "Mag_STANAG_30Rnd", 2);
		AddMultiple (player, "Mag_FNX45_15Rnd", 2);
		AddMultiple (player, "Ammo_308Win", 2);

		// Optics
		AddOptics (player);

		// Equiptment
		AddMultiple (player, "Battery9V", 4);
		EntityAI bandage = AddInventory (player, "BandageDressing");
		player.SetQuickBarEntityShortcut (bandage, shortcut++);
		AddInventory (player, "BandageDressing");
		EntityAI knife = AddInventory (player, "CombatKnife");
		player.SetQuickBarEntityShortcut (knife, shortcut++);

		// Food
		EntityAI food = AddInventory (player, "TacticalBaconCan_Opened");
		player.SetQuickBarEntityShortcut (food, shortcut++);
		EntityAI water = AddInventory (player, "Canteen");
		player.SetQuickBarEntityShortcut (water, shortcut++);
	}
};

Mission CreateCustomMission (string path) {
	return new CustomMission ();
}
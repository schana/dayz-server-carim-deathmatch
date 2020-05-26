int FULL_EQUIP = 1;
int INVINCIBLE = 0;
int GIVE_GUNS = 0;
int SPAWN_CAR = 0;

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
		AddAttachment (gun, "ACOGOptic");
		AddAttachment (gun, "M4_Suppressor");
		player.SetQuickBarEntityShortcut (gun, shortcut, true);
	}

	void AddSniper (PlayerBase player, int shortcut) {
		EntityAI sniper = AddInventory (player, "Winchester70");
		AddAttachment (sniper, "HuntingOptic");
		//AddAttachment(sniper, "GhillieAtt_Woodland");
		player.SetQuickBarEntityShortcut (sniper, shortcut, true);
	}

	void AddAxe (PlayerBase player, int shortcut) {
		EntityAI axe = AddInventory (player, "WoodAxe");
		player.SetQuickBarEntityShortcut (axe, shortcut, true);
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
		AddAttachment (boots, "CombatKnife");
		EntityAI vest = AddInventory (player, "PlateCarrierVest");
		AddAttachment (vest, "PlateCarrierPouches");
		AddAttachment (vest, "PlateCarrierHolster");
		//AddInventory(player, "GhillieSuit_Woodland");
		//AddInventory(player, "GhillieHood_Woodland");
	}

	void AddFNX45 (PlayerBase player, int shortcut) {
		EntityAI gun = AddInventory (player, "FNX45");
		player.SetQuickBarEntityShortcut (gun, shortcut, true);
		// AddAttachment (gun, "FNP45_MRDSOptic");
		// AddAttachment (gun, "TLRLight");
	}

	override void StartingEquipSetup (PlayerBase player, bool clothesChosen) {
		player.RemoveAllItems ();

		int shortcut = 0;

		if (FULL_EQUIP) {
			AddM4 (player, shortcut++);
			AddSniper (player, shortcut++);
			AddMilitaryClothing (player);
			AddFNX45 (player, shortcut++);

			// Bags
			//EntityAI bag = AddInventory(player, "AliceBag_Camo");

			// Ammo
			AddMultiple (player, "Mag_STANAG_30Rnd", 2);
			AddMultiple (player, "Mag_FNX45_15Rnd", 2);
			//AddMultiple(player, "Ammo_556x45", 12);
			//AddMultiple(player, "Ammo_45ACP", 4);
			AddMultiple (player, "Ammo_308Win", 1);

			// Equiptment
			AddMultiple (player, "Battery9V", 2);
			//AddInventory(player, "OrienteeringCompass");
			AddMultiple (player, "BandageDressing", 2);
			AddInventory (player, "CombatKnife");

			// Food
			AddMultiple (player, "TacticalBaconCan_Opened", 2);
			AddInventory (player, "Canteen");
		} else {
			AddInventory (player, "AthleticShoes_Black");
			AddInventory (player, "TShirt_Grey");
			AddInventory (player, "Jeans_Blue");
			AddInventory (player, "Rag");
			AddInventory (player, "Apple");
			AddAxe (player, shortcut++);
		}

	}

	void AddIfOnlyEmptyExist (string type, PlayerBase player) {
		bool onlyEmpty = true;
		array<EntityAI> items = new array<EntityAI>;
		player.GetInventory ().EnumerateInventory (InventoryTraversalType.PREORDER, items);
		for (int i = 0; i < items.Count (); i++) {
			if (items.Get (i).GetType () == type) {
				Magazine mag = (Magazine) items.Get (i);
				if (mag.IsMagazine ()) {
					if (mag.GetAmmoCount () > 0) {
						onlyEmpty = false;
					} else {
						if (!((Weapon) mag.GetHierarchyParent ()).IsWeapon ()) {
							mag.Delete ();
						}
					}
				}
			}
		}
		if (onlyEmpty) {
			AddInventory (player, type);
		}
	}

	bool HasItemTypeInInventory (string type, PlayerBase player) {
		array<EntityAI> items = new array<EntityAI>;
		player.GetInventory ().EnumerateInventory (InventoryTraversalType.PREORDER, items);
		for (int i = 0; i < items.Count (); i++) {
			if (items.Get (i).GetType () == type) {
				return true;
			}
		}
		return false;
	}

	EntityAI GetItemTypeInInventory (string type, PlayerBase player) {
		array<EntityAI> items = new array<EntityAI>;
		player.GetInventory ().EnumerateInventory (InventoryTraversalType.PREORDER, items);
		for (int i = 0; i < items.Count (); i++) {
			if (items.Get (i).GetType () == type) {
				return items.Get (i);
			}
		}
		return NULL;
	}

	EntityAI AddIfNotAlreadyPresent (PlayerBase player, string type) {
		EntityAI item = GetItemTypeInInventory (type, player);
		if (item == NULL) {
			return player.GetInventory ().CreateInInventory (type);
		}
		return item;
	}

	override void TickScheduler (float timeslice) {
		GetGame ().GetWorld ().GetPlayerList (m_Players);
		if (m_Players.Count () == 0) return;
		for (int i = 0; i < SCHEDULER_PLAYERS_PER_TICK; i++) {
			if (m_currentPlayer >= m_Players.Count ()) {
				m_currentPlayer = 0;
			}

			PlayerBase currentPlayer = PlayerBase.Cast (m_Players.Get (m_currentPlayer));

			if (INVINCIBLE) {
				currentPlayer.SetHealth (currentPlayer.GetMaxHealth ("", ""));
				currentPlayer.SetHealth ("", "Blood", currentPlayer.GetMaxHealth ("", "Blood"));
				currentPlayer.SetHealth ("", "Shock", currentPlayer.GetMaxHealth ("", "Shock"));
				currentPlayer.GetStatStamina ().Set (GameConstants.STAMINA_MAX);

				array<EntityAI> items = new array<EntityAI>;
				currentPlayer.GetInventory ().EnumerateInventory (InventoryTraversalType.INORDER, items);
				foreach (EntityAI fixItem : items) {
					// only on server? client doesn't check?
					fixItem.SetHealth (fixItem.GetMaxHealth ("", ""));
				}
			}

			if (GIVE_GUNS) {
				AddIfNotAlreadyPresent (currentPlayer, "Matchbox");
				if (!HasItemTypeInInventory ("M4A1", currentPlayer)) {
					AddM4 (currentPlayer, 0);
				}
				if (!HasItemTypeInInventory ("FNX45", currentPlayer)) {
					AddFNX45 (currentPlayer, 0);
				}
				AddIfNotAlreadyPresent (currentPlayer, "Mag_FNX45_15Rnd");
				AddIfNotAlreadyPresent (currentPlayer, "Mag_STANAG_30Rnd");
				//AddIfNotAlreadyPresent(currentPlayer, "Canteen");
				//AddIfNotAlreadyPresent(currentPlayer, "CanisterGasoline");
				Weapon_Base weapon;
				EntityAI hands = currentPlayer.GetHumanInventory ().GetEntityInHands ();
				Class.CastTo (weapon, hands);
				if (weapon && weapon.CanProcessWeaponEvents ()) {
					Magazine mag = weapon.GetMagazine (weapon.GetCurrentMuzzle ());

					if (mag) {
						if (mag.GetAmmoCount () < 1) {
							AddIfOnlyEmptyExist (mag.GetType (), currentPlayer);
						}
					}
					ItemSuppressor surpressor = weapon.GetAttachedSuppressor ();
					if (surpressor) {
						surpressor.SetHealth (surpressor.GetMaxHealth ("", ""));
					}
					weapon.SetHealth (weapon.GetMaxHealth ("", ""));
				}
			}
			currentPlayer.OnTick ();
			m_currentPlayer++;
		}
	}

	EntityAI RemoveItemTypeFromInventory (string type, PlayerBase player) {
		array<EntityAI> items = new array<EntityAI>;
		player.GetInventory ().EnumerateInventory (InventoryTraversalType.PREORDER, items);
		for (int i = 0; i < items.Count (); i++) {
			if (items.Get (i).GetType () == type) {
				items.Get (i).Delete ();
			}
		}
		return NULL;
	}

	override void InvokeOnConnect (PlayerBase player, PlayerIdentity identity) {
		if (player) {
			player.OnConnect ();
			if (SPAWN_CAR) {
				vector position = player.GetPosition ();
				position[0] = position[0] + 2;
				position[2] = position[2] + 2;

				EntityAI oAWD = EntityAI.Cast (GetGame ().CreateObject ("OffroadHatchback", position, false, true));
				oAWD.GetInventory ().CreateAttachment ("HatchbackWheel");
				oAWD.GetInventory ().CreateAttachment ("HatchbackWheel");
				oAWD.GetInventory ().CreateAttachment ("HatchbackWheel");
				oAWD.GetInventory ().CreateAttachment ("HatchbackWheel");
				oAWD.GetInventory ().CreateAttachment ("HatchbackWheel");
				oAWD.GetInventory ().CreateAttachment ("HatchbackDoors_Driver");
				oAWD.GetInventory ().CreateAttachment ("HatchbackDoors_CoDriver");
				oAWD.GetInventory ().CreateAttachment ("HatchbackHood");
				oAWD.GetInventory ().CreateAttachment ("HatchbackTrunk");
				oAWD.GetInventory ().CreateAttachment ("SparkPlug");
				oAWD.GetInventory ().CreateAttachment ("EngineBelt");
				oAWD.GetInventory ().CreateAttachment ("CarRadiator");
				oAWD.GetInventory ().CreateAttachment ("CarBattery");
				oAWD.GetInventory ().CreateAttachment ("HeadlightH7");
				oAWD.GetInventory ().CreateAttachment ("HeadlightH7");
			}
		}
	}

};

Mission CreateCustomMission (string path) {
	return new CustomMission ();
}
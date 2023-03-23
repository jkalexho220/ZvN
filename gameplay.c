void removeCamTracks(int eventID = -1) {
	trackRemove();
	trUnblockAllSounds();
}

void hotkeyAbility(int ability = 0) {
	xsSetContextPlayer(0);
	int old = xGetPointer(dPlayerData);
	xSetPointer(dPlayerData, trCurrentPlayer());
	if (xGetBool(dPlayerData, xPlayerCanCast)) {
		trackInsert();
		trackAddWaypoint();
		trackPlay(1,EVENT_BUILD_AT_CURSOR);
		trBlockAllSounds(false);
		xUnitSelectByID(dPlayerData, xPlayerUnitID);
		if (trUnitIsSelected() == false) {
			uiFindType(xGetString(dPlayerData, xPlayerProto));
		}
		switch(ability)
		{
			case EVENT_BUILD_HOUSE:
			{
				uiSetProtoCursor("House", true);
			}
			case EVENT_BUILD_GRANARY:
			{
				uiSetProtoCursor("Granary", true);
			}
			case EVENT_BUILD_STOREHOUSE:
			{
				uiSetProtoCursor("Storehouse", true);
			}
		}
	}
	xSetPointer(dPlayerData, old);
}

void buildAtCursor(int eventID = -1) {
	uiBuildAtPointer();
	uiClearCursor();
	trackInsert();
	trackAddWaypoint();
	trackPlay(1,EVENT_REMOVE_CAM_TRACKS);
}

rule gameplay_setup
inactive
highFrequency
{
	xsDisableSelf();

	xsEnableRule("ysearch");

	trUIFadeToColor(0,0,0,1000,0,false);

	//xsEnableRule("gameplay_start");

	// spawn the hotkey units
	for(i=EVENT_BUILD_HOUSE; <= EVENT_BUILD_STOREHOUSE) {
		trEventSetHandler(i, "hotkeyAbility");
	}
	trEventSetHandler(EVENT_REMOVE_CAM_TRACKS, "removeCamTracks");
	trEventSetHandler(EVENT_BUILD_AT_CURSOR, "buildAtCursor");

	map("q", "game", "trackInsert(); trackAddWaypoint();trackPlay(-1,0);");
	map("w", "game", "trackInsert(); trackAddWaypoint();trackPlay(-1,1);");
	map("e", "game", "trackInsert(); trackAddWaypoint();trackPlay(-1,2);");

	// spawn players
	spawnPlayer(1, xsVectorSet(mapSize / 2 + 1, 0, mapSize / 2 + 1));
	spawnPlayer(2, xsVectorSet(mapSize * 3 / 2 + 1, 0, mapSize * 3 / 2 + 1));
	updateAbilities(1, true);
	updateAbilities(2, true);

	// select myself
	uiFindType(xGetString(dPlayerData, xPlayerProto, trCurrentPlayer()));

	xsEnableRule("the_game");
}

rule the_game
inactive
highFrequency
{
	// gameplay goes here
	int db = 0;
	int id = 0;
	int target = 0;
	int action = 0;
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	for(p=1; <= 2) {
		xDatabaseNext(dPlayerData);
		// update player pos vector
		xSetVector(dPlayerData, xPlayerPos, kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit)));
	}
	for(p=1; <= 2) {
		xSetPointer(dPlayerData, p);

		// abilities
		if (xGetBool(dPlayerData, xPlayerCanCast)) {
			if (xGetInt(dPlayerData, xPlayerButton) > 0) {
				db = xGetInt(dPlayerData, xPlayerAbilities);
				xSetPointer(db, xGetInt(dPlayerData, xPlayerButton));
				if (xGetInt(db, xAbilityType) != ABILITY_ON_COOLDOWN) {
					switch(xGetInt(db, xAbilityType))
					{
					case ZENO_BOUNCING:
						{
							zenoBouncingLaser(p);
						}
					}
				} else if (trCurrentPlayer() == p) {
					trSoundPlayFN("cantdothat.wav");
					trChatSend(0, "Your " + hotkeyName(xGetInt(dPlayerData, xPlayerButton)) + " is on cooldown!");
				}
			}
		}
		xSetInt(dPlayerData, xPlayerButton, BUTTON_NONE);

		// basic attacks
		id = xGetInt(dPlayerData, xPlayerUnitID);
		// gather information
		xsSetContextPlayer(p);
		action = kbUnitGetActionType(id);
		target = kbUnitGetTargetUnitID(id);
		xsSetContextPlayer(0);
		if (p == trCurrentPlayer()) {
			trSetCounterDisplay("Action: " + action + " | Target: " + target);
		}
		switch(xGetInt(dPlayerData, xPlayerAttackStep))
		{
		case ATTACK_NONE:
			{
				if (target >= 0) { // TARGET ACQUIRED
					xSetInt(dPlayerData, xPlayerAttackStep, ATTACK_WINDUP);
					xSetInt(dPlayerData, xPlayerAttackNext, trTimeMS() + xGetInt(dPlayerData, xPlayerStartDelay));
					xUnitSelectByID(dPlayerData, xPlayerUnitID);
					trUnitOverrideAnimation(xGetInt(dPlayerData, xPlayerAttackAnimation), 0, false, true, -1);
					dir = getUnitVector(xGetVector(dPlayerData, xPlayerPos), kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
					xSetVector(dPlayerData, xPlayerAttackDir, dir);
					trSetUnitOrientation(dir, vector(0,1,0), true);
				}
				break;
			}
		case ATTACK_WINDUP:
			{
				if (action == 11) {
					xSetInt(dPlayerData, xPlayerAttackStep, ATTACK_NONE);
					xUnitSelectByID(dPlayerData, xPlayerUnitID);
					trUnitOverrideAnimation(-1, 0, false, true, -1);
					debugLog("interrupt windup!");
				} else if (trTimeMS() >= xGetInt(dPlayerData, xPlayerAttackNext)) {
					xSetInt(dPlayerData, xPlayerAttackStep, ATTACK_WINDDOWN);
					xSetInt(dPlayerData, xPlayerAttackNext, xGetInt(dPlayerData, xPlayerEndDelay) + trTimeMS());
					dir = xGetVector(dPlayerData, xPlayerAttackDir);
					switch(xGetInt(dPlayerData, xPlayerAttackAnimation))
					{
					case 1: // zenophobia
						{
							shootLaser(p, xGetInt(dPlayerData, xPlayerSpawner), dir);
						}
					case 12: // nickonhawk
						{
							shootMissile(p, xGetVector(dPlayerData, xPlayerPos), dir);
						}
					}
				}
				break;
			}
		case ATTACK_WINDDOWN:
			{
				if (action == 11 || trTimeMS() > xGetInt(dPlayerData, xPlayerAttackNext)) {
					xSetInt(dPlayerData, xPlayerAttackStep, ATTACK_NONE);
					xUnitSelectByID(dPlayerData, xPlayerUnitID);
					trUnitOverrideAnimation(-1, 0, false, true, -1);
				}
				break;
			}
		}
	}
}
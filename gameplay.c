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

	map("q", "game", "trackInsert(); trackAddWaypoint();trackPlay(-1,"+BUTTON_Q+");");
	map("w", "game", "trackInsert(); trackAddWaypoint();trackPlay(-1,"+BUTTON_W+");");
	map("e", "game", "trackInsert(); trackAddWaypoint();trackPlay(-1,"+BUTTON_E+");");

	// spawn players
	spawnPlayer(1, vector(21,9,21));
	spawnPlayer(2, vector(43,9,43));
	
	xsEnableRule("gameplay_start");
}

rule gameplay_start
inactive
highFrequency
{
	xsDisableSelf();
	trCameraCut(vector(-15.519794,70.710701,-15.519794), vector(0.5,-0.707107,0.5), vector(0.5,0.707107,0.5), vector(0.707107,0,-0.707107));

	trPlayerSetDiplomacy(1, 2, "Enemy");
	trPlayerSetDiplomacy(2, 1, "Enemy");

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
	int p = 0;
	int db = 0;
	int id = 0;
	int target = 0;
	int action = 0;
	float scale = 0;
	float dist = 0;
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
					case ZENO_GRID:
						{
							zenoLaserGrid(p);
						}
					case ZENO_TURRET:
						{
							zenoTurret(p);
						}
					}
					xSetInt(db, xAbilityType, ABILITY_ON_COOLDOWN);
					xSetInt(db, xAbilityCooldown, trTimeMS() + 15000);
					updateAbilities(p, true);
				} else if (trCurrentPlayer() == p) {
					trSoundPlayFN("cantdothat.wav");
					trChatSend(0, "Your " + hotkeyName(xGetInt(dPlayerData, xPlayerButton)) + " is on cooldown!");
				}
			}
		}
		xSetInt(dPlayerData, xPlayerButton, BUTTON_NONE);
		updateAbilities(p);

		// speed boost
		if (trQuestVarGet("p"+p+"speed") == 1) {
			if (trTimeMS() > trQuestVarGet("p"+p+"speedTimeout")) {
				trQuestVarSet("p"+p+"speed", 0);
				trModifyProtounit("Hoplite", p, 1, -5);
			}
		}

		// basic attacks
		id = xGetInt(dPlayerData, xPlayerUnitID);
		// gather information
		xsSetContextPlayer(p);
		action = kbUnitGetActionType(id);
		target = kbUnitGetTargetUnitID(id);
		xsSetContextPlayer(0);
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

	// Lasers
	for(i=xGetDatabaseCount(dLasers); >0) {
		xDatabaseNext(dLasers);
		p = xGetInt(dLasers, xOwner);
		switch(xGetInt(dLasers, xLaserStep))
		{
		case LASER_WARN:
			{
				if (trTimeMS() > xGetInt(dLasers, xLaserTimeout)) {
					trSoundPlayFN("sky passage.wav");
					if (rayCollision(xGetVector(dPlayerData, xPlayerPos, 3 - p), xGetVector(dLasers, xLaserPos), 
						xGetVector(dLasers, xLaserDir), xGetFloat(dLasers, xLaserLength), 1.0)) {
						damagePlayer(3 - p, xGetInt(dLasers, xLaserDamage));
					}
					xSetInt(dLasers, xLaserTimeout, trTimeMS() + 500 + 100 * xGetInt(dLasers, xLaserDamage));
					xSetInt(dLasers, xLaserStep, LASER_FIRED);
					xUnitSelectByID(dLasers, xUnitID);
					trUnitHighlight(10.0, false);
				}
				break;
			}
		case LASER_FIRED:
			{
				xUnitSelectByID(dLasers, xUnitID);
				scale = 0.012 * (xGetInt(dLasers, xLaserTimeout) - trTimeMS());
				if (scale < 0) {
					trUnitDestroy();
					xFreeDatabaseBlock(dLasers);
				} else {
					trSetSelectedScale(scale, scale, xGetFloat(dLasers, xLaserLength) * 1.222222);
				}
			}
		}
	}

	// missiles
	for(i=xGetDatabaseCount(dMissiles); >0) {
		xDatabaseNext(dMissiles);
		p = xGetInt(dMissiles, xOwner);
		pos = xGetVector(dMissiles, xMissilePos) + xGetVector(dMissiles, xMissileDir) * timediff;
		dir = (pos - vector(31, 0, 31)) * 3.33;
		xUnitSelectByID(dMissiles, xUnitID);
		trSetSelectedUpVector(xsVectorGetX(dir),0.2,xsVectorGetZ(dir));
		xSetVector(dMissiles, xMissilePos, pos);
		dist = distanceBetweenVectors(pos, xGetVector(dMissiles, xMissilePrev));
		if (dist > 2.0) {
			pos = xGetVector(dMissiles, xMissilePrev);
			dist = xsSqrt(dist);
			if (rayCollision(xGetVector(dPlayerData, xPlayerPos, 3 - p), pos, xsVectorNormalize(xGetVector(dMissiles, xMissileDir)), dist + 1.0, 1.0)) {
				damagePlayer(3 - p, 3);
				xUnitSelectByID(dMissiles, xUnitID);
				trUnitDestroy();
				xFreeDatabaseBlock(dMissiles);
			} else {
				xSetVector(dMissiles, xMissilePrev, xGetVector(dMissiles, xMissilePos));
				if (vectorInMap(pos) == false) {
					xUnitSelectByID(dMissiles, xUnitID);
					trUnitDestroy();
					xFreeDatabaseBlock(dMissiles);
				}
			}
		}
	}

	// Turrets
	if (xGetDatabaseCount(dTurrets) > 0) {
		xDatabaseNext(dTurrets);
		p = xGetInt(dTurrets, xOwner);
		if (xGetInt(dTurrets, xTurretType) == NICK_ABILITIES) {
			pos = xGetVector(dTurrets, xTurretPos);
			dir = getUnitVector(pos, xGetVector(dPlayerData, xPlayerPos, 3 - p));
			xUnitSelectByID(dTurrets, xUnitID);
			trSetSelectedUpVector(xsVectorGetX(dir), -1.0, xsVectorGetZ(dir));
		}

		if (trTimeMS() > xGetInt(dTurrets, xTurretCooldown)) {
			dir = getUnitVector(xGetVector(dTurrets, xTurretPos), xGetVector(dPlayerData, xPlayerPos, 3 - p));
			xSetInt(dTurrets, xTurretCooldown, xGetInt(dTurrets, xTurretCooldown) + 5000);
			if (xGetInt(dTurrets, xTurretType) == ZENO_ABILITIES) {
				shootLaser(p, xGetInt(dTurrets, xUnitName), dir);
				xUnitSelectByID(dTurrets, xUnitID);
				trUnitChangeProtoUnit("Spy Eye");
				xUnitSelectByID(dTurrets, xUnitID);
				trMutateSelected(kbGetProtoUnitID("Tower Mirror"));
				trSetSelectedScale(0, 0.15, 0);
			} else if (xGetInt(dTurrets, xTurretType) == NICK_ABILITIES) {
				shootMissile(p, xGetVector(dTurrets, xTurretPos), dir);
			}
		}
	}
}
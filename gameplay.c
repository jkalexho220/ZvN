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
	spawnPlayer(1, vector(21,9,43));
	spawnPlayer(2, vector(43,9,21));
	
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

	trQuestVarSet("p1cooldowns", 15000);
	trQuestVarSet("p2cooldowns", 15000);

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
	vector next = vector(0,0,0);
	vector center = vector(0,0,0);
	vector dir = vector(0,0,0);
	vector firstDir = vector(0,0,0);
	vector nextDir = vector(0,0,0);
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
					case ZENO_SPEED:
						{
							zenoSpeed(p);
						}
					case ZENO_SHIELD:
						{
							zenoShield(p);
						}
					case ZENO_BARRAGE:
						{
							zenoBarrage(p);
						}
					case ZENO_CAROUSEL:
						{
							zenoCarousel(p);
						}
					case ZENO_WORLD_SPLITTER:
						{
							zenoWorldSplitter(p);
						}
					}
					xSetInt(db, xAbilityType, ABILITY_ON_COOLDOWN);
					xSetInt(db, xAbilityCooldown, trTimeMS() + trQuestVarGet("p"+p+"cooldowns"));
					trQuestVarSet("p1cooldowns", trQuestVarGet("p1cooldowns") * 0.99);
					trQuestVarSet("p2cooldowns", trQuestVarGet("p2cooldowns") * 0.99);
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

		// barrage
		if (trQuestVarGet("p"+p+"barrage") > 0) {
			if (trTimeMS() > trQuestVarGet("p"+p+"barrageNext")) {
				trQuestVarSet("p"+p+"barrageNext", trTimeMS() + 100);
				dir = rotationMatrix(trVectorQuestVarGet("p"+p+"barrageDir"), 0.965926, 0.258819);
				shootLaser(p, xGetInt(dPlayerData, xPlayerSpawner), dir, 40.0, 1000);
				trVectorQuestVarSet("p"+p+"barrageDir", dir);
				trQuestVarSet("p"+p+"barrage", trQuestVarGet("p"+p+"barrage") - 1);
				if (trQuestVarGet("p"+p+"barrage") == 0) {
					xUnitSelect(dPlayerData, xPlayerSphinx);
					trUnitOverrideAnimation(-1, 0, false, true, -1);
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				}
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
					trQuestVarSet("laserSound", 1);
					if (rayCollision(xGetVector(dPlayerData, xPlayerPos, 3 - p), xGetVector(dLasers, xLaserPos), 
						xGetVector(dLasers, xLaserDir), xGetFloat(dLasers, xLaserLength), 1.0)) {
						damagePlayer(3 - p, xGetInt(dLasers, xLaserDamage));
					}
					xSetInt(dLasers, xLaserTimeout, trTimeMS() + 300 + 100 * xGetInt(dLasers, xLaserDamage));
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

	if (trQuestVarGet("laserSound") == 1) {
		trSoundPlayFN("sky passage.wav");
		trQuestVarSet("laserSound", 0);
	}

	// deflector shields
	if (xGetDatabaseCount(dDeflectorShields) > 0) {
		xDatabaseNext(dDeflectorShields);
		switch(xGetInt(dDeflectorShields, xDeflectorShieldStep))
		{
		case 0:
			{
				// shield growing
				scale = 50.0 - 0.1 * (xGetInt(dDeflectorShields, xDeflectorShieldTimeout) - trTimeMS());
				if (scale >= 50.0) {
					scale = 50.0;
					xSetInt(dDeflectorShields, xDeflectorShieldStep, 1);
					xSetInt(dDeflectorShields, xDeflectorShieldTimeout, trTimeMS() + 6000);
				}
				xUnitSelect(dDeflectorShields, xDeflectorShieldLeft);
				trSetSelectedScale(1, scale, 5);
				xUnitSelect(dDeflectorShields, xDeflectorShieldRight);
				trSetSelectedScale(1, scale, 5);
			}
		case 1:
			{
				if (trTimeMS() > xGetInt(dDeflectorShields, xDeflectorShieldTimeout)) {
					xSetInt(dDeflectorShields, xDeflectorShieldStep, 2);
					xSetInt(dDeflectorShields, xDeflectorShieldTimeout, trTimeMS() + 500);
				} else {
					// collision detection with bullets
					center = xGetVector(dDeflectorShields, xDeflectorShieldPos);
					for(i=xGetDatabaseCount(dMissiles); >0) {
						xDatabaseNext(dMissiles);
						pos = xGetVector(dMissiles, xMissilePos);
						dir = xsVectorNormalize(xGetVector(dMissiles, xMissileDir));
						next = pos + dir;
						if (distanceBetweenVectors(pos, center) < 64.0 || distanceBetweenVectors(next, center) < 64.0) {
							firstDir = getUnitVector(center, pos);
							nextDir = getUnitVector(center, next);

							scale = dotProduct(firstDir, nextDir); // angle between the two positions
							dist = dotProduct(firstDir, xGetVector(dDeflectorShields, xDeflectorShieldDir));
							if (dist > scale) { // first angle is smaller
								dist = dotProduct(nextDir, xGetVector(dDeflectorShields, xDeflectorShieldDir));
								if (dist > scale) { // second angle is smaller too. we are in between the two
									firstDir = xGetVector(dDeflectorShields, xDeflectorShieldDir);
									nextDir = xGetVector(dMissiles, xMissileDir);
									dir = xGetVector(dDeflectorShields, xDeflectorShieldDir) * dotProduct(firstDir, nextDir);
									dir = xGetVector(dMissiles, xMissileDir) * (-1.0) + dir * 2.0;
									xSetVector(dMissiles, xMissileDir, dir);
									trQuestVarSetFromRand("sound", 1, 3, true);
									trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav");
								}
							}
						}
					}
				}
			}
		case 2:
			{
				scale = 0.1 * (xGetInt(dDeflectorShields, xDeflectorShieldTimeout) - trTimeMS());
				if (scale <= 0) {
					xUnitSelect(dDeflectorShields, xDeflectorShieldLeft);
					trUnitDestroy();
					xUnitSelect(dDeflectorShields, xDeflectorShieldRight);
					trUnitDestroy();
					xFreeDatabaseBlock(dDeflectorShields);
				} else {
					xUnitSelect(dDeflectorShields, xDeflectorShieldLeft);
					trSetSelectedScale(1, scale, 5);
					xUnitSelect(dDeflectorShields, xDeflectorShieldRight);
					trSetSelectedScale(1, scale, 5);
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

	// carousels
	for(i=xGetDatabaseCount(dCarousels); >0) {
		xDatabaseNext(dCarousels);
		p = xGetInt(dCarousels, xOwner);
		dir = xGetVector(dCarousels, xCarouselDir);
		dir = rotationMatrix(dir, xsCos(timediff * 0.4), xsSin(timediff * 0.4));
		xSetVector(dCarousels, xCarouselDir, dir);
		db = 0;
		if (trTimeMS() > xGetInt(dCarousels, xCarouselTimeout)) {
			xSetInt(dCarousels, xCarouselStep, xGetInt(dCarousels, xCarouselStep) + 1);
			switch(xGetInt(dCarousels, xCarouselStep))
			{
			case 1:
				{
					for(j=xGetInt(dCarousels, xCarouselStart); < xGetInt(dCarousels, xCarouselEnd)) {
						trUnitSelectClear();
						trUnitSelect(""+j, true);
						trSetSelectedScale(15.0, 15.0, 40.0);
						trUnitHighlight(50.0, false);
					}
					trSoundPlayFN("sonofosirisbolt.wav");
					xSetInt(dCarousels, xCarouselTimeout, trTimeMS() + 6000);
				}
			case 2:
				{
					xSetInt(dCarousels, xCarouselTimeout, trTimeMS() + 500);
				}
			case 3:
				{
					for(j=xGetInt(dCarousels, xCarouselStart); < xGetInt(dCarousels, xCarouselEnd)) {
						trUnitSelectClear();
						trUnitSelect(""+j, true);
						trUnitDestroy();
					}
					xFreeDatabaseBlock(dCarousels);
				}
			}
		}
		scale = 0.03 * (xGetInt(dCarousels, xCarouselTimeout) - trTimeMS());
		for(j=xGetInt(dCarousels, xCarouselStart); < xGetInt(dCarousels, xCarouselEnd)) {
			trUnitSelectClear();
			trUnitSelect(""+j, true);
			trSetUnitOrientation(dir, vector(0,1,0), true);
			if (xGetInt(dCarousels, xCarouselStep) == 2) {
				trSetSelectedScale(scale, scale, 40.0);
			}
			if ((db < 4) && (xGetInt(dCarousels, xCarouselStep) == 1)) { // hitboxes for the first four lasers
				db = db + 1;
				pos = xGetVector(dCarousels, xCarouselPos) - dir * 40.0;
				if (rayCollision(xGetVector(dPlayerData, xPlayerPos, 3 - p), pos, dir, 80.0, 4.0)) {
					damagePlayer(3 - p, timediff * 3.0);
					db = 100; // no need to check the rest
				}
			}
			dir = rotationMatrix(dir, 0.707107, 0.707107);
		}
	}
}
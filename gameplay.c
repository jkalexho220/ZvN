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
	for(i=1; <= 2) {
		p = xDatabaseNext(dPlayerData);
		// update player pos vector
		if (xGetBool(dPlayerData, xPlayerAlive)) {
			xUnitSelectByID(dPlayerData, xPlayerUnitID);
			if (trUnitAlive()) {
				xSetVector(dPlayerData, xPlayerPos, kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit)));
			} else {
				death(p);
			}
		} else if ((xGetInt(dPlayerData, xPlayerButton) != BUTTON_NONE) || (trTime() > trQuestVarGet("p"+p+"respawnTime"))) {
			if (trCurrentPlayer() == p) {
				trCounterAbort("respawn");
			}
			if (xGetInt(dPlayerData, xPlayerButton) == BUTTON_NONE) {
				xSetInt(dPlayerData, xPlayerButton, EVENT_BUILD_HOUSE);
			}
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"dwarf");
			trUnitDestroy();
			getUpgrade(p, 1*trQuestVarGet("p"+p+"upgrade"+xGetInt(dPlayerData, xPlayerButton)));
			trSoundPlayFN("herorevived.wav");
			pos = vector(32, 0, 32) * 2 - xGetVector(dPlayerData, xPlayerPos, 3 - p);
			spawnPlayer(p, pos);
			xSetInt(dPlayerData, xPlayerButton, BUTTON_NONE);
		}
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
							break;
						}
					case ZENO_TURRET:
						{
							zenoTurret(p);
							break;
						}
					case ZENO_SPEED:
						{
							zenoSpeed(p);
							break;
						}
					case ZENO_SHIELD:
						{
							zenoShield(p);
							break;
						}
					case ZENO_BARRAGE:
						{
							zenoBarrage(p);
							break;
						}
					case ZENO_CAROUSEL:
						{
							zenoCarousel(p);
							break;
						}
					case ZENO_WORLD_SPLITTER:
						{
							zenoWorldSplitter(p);
							break;
						}
					case NICK_VOLLEY:
						{
							nickVolley(p);
							break;
						}
					case NICK_TURRET:
						{
							nickTurret(p);
							break;
						}
					case NICK_TELEPORT:
						{
							nickTeleport(p);
							break;
						}
					case NICK_BOMB:
						{
							nickBomb(p);
							break;
						}
						/*
					case NICK_SPLIT:
						{
							nickSplit(p);
							break;
						}
						*/
					case NICK_MIRROR:
						{
							nickMirror(p);
							break;
						}
					case NICK_MISSILES:
						{
							nickMissiles(p);
							break;
						}
					case NICK_ON_HAWK:
						{
							nickOnHawk(p);
							break;
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
				trQuestVarSet("p"+p+"barrage", 0);
				xUnitSelect(dPlayerData, xPlayerSphinx);
				trUnitOverrideAnimation(-1, 0, false, true, -1);
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				/*
				trQuestVarSet("p"+p+"barrageNext", trTimeMS() + 300);
				dir = rotationMatrix(trVectorQuestVarGet("p"+p+"barrageDir"), 0.965926, 0.258819);
				shootLaser(p, xGetInt(dPlayerData, xPlayerSpawner), dir, 40.0, 1000);
				//trVectorQuestVarSet("p"+p+"barrageDir", dir);
				trQuestVarSet("p"+p+"barrage", trQuestVarGet("p"+p+"barrage") - 1);
				if (trQuestVarGet("p"+p+"barrage") == 0) {
					xUnitSelect(dPlayerData, xPlayerSphinx);
					trUnitOverrideAnimation(-1, 0, false, true, -1);
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				}
				*/
			}
		}

		// world splitter
		if (trQuestVarGet("p"+p+"worldSplitterActive") > 0) {
			switch(1*trQuestVarGet("p"+p+"worldSplitterActive"))
			{
			case 1:
				{
					if (trTimeMS() > trQuestVarGet("p"+p+"worldSplitterTimeout")) {
						zSetProtoUnitStat("Meteorite", p, 1, 1.0);
						trQuestVarSet("p"+p+"worldSplitterActive", 2);
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"meteorite");
						trMutateSelected(kbGetProtoUnitID("Meteorite"));
						trUnitOverrideAnimation(6, 0, true, false, -1); // EXPLOSION
						dir = trVectorQuestVarGet("p"+p+"worldSplitterDir");
						pos = vectorSetAsTargetVector(xGetVector(dPlayerData, xPlayerPos), dir, 999.0);
						trUnitMoveToPoint(xsVectorGetX(pos), 0, xsVectorGetZ(pos), -1, false);

						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"worldSplitterLaser");
						trSetSelectedScale(12.0,12.0,50.0);
						trUnitHighlight(50.0, false);
						trSetUnitOrientation(vector(0,1,0), rotationMatrix(dir, 1.0, 0), true);

						trSoundPlayFN("sonofosirisbolt.wav");
						trSoundPlayFN("phoenixattack.wav");
					}
				}
			case 2:
				{
					// calculate my speed
					pos = kbGetBlockPosition(""+1*trQuestVarGet("p"+p+"meteorite"));
					if (distanceBetweenVectors(pos, xGetVector(dPlayerData, xPlayerPos)) > 0.0) {
						//point the laser at the meteorite
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"worldSplitterLaser");
						trSetUnitOrientation(xsVectorNormalize(xGetVector(dPlayerData, xPlayerPos) - pos), rotationMatrix(trVectorQuestVarGet("p"+p+"worldSplitterDir"), 0.0, 1.0), true);

						dist = distanceBetweenVectors(pos, xGetVector(dPlayerData, xPlayerPos), false);
						dir = xsVectorSet(dist, 0, xsVectorGetY(xGetVector(dPlayerData, xPlayerPos)) - 9.0); // construct a vector representation of the 3d angle
						dir = rotationMatrix(dir, 0.9998, -0.019999); // the angle one second from now is 0.02 radians further
						dir = dir * ((xsVectorGetY(xGetVector(dPlayerData, xPlayerPos)) - 9.0) / xsVectorGetZ(dir)); // scale it back up

						scale = 60.0 * (xsVectorGetX(dir) - dist); // this is our speed if we need to reach that next spot in 1/60th of a second
						zSetProtoUnitStat("Meteorite", p, 1, scale);
					}
					if (positionInArena(pos) == false) {
						trQuestVarSet("p"+p+"worldSplitterActive", 3);
						trQuestVarSet("p"+p+"worldSplitterTimeout", trTimeMS() + 1000);
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"meteorite");
						trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					}
					next = pos;
					pos = trVectorQuestVarGet("p"+p+"worldSplitterPrev");
					dist = distanceBetweenVectors(next, pos);
					if (dist > 9.0) {
						dir = trVectorQuestVarGet("p"+p+"worldSplitterDir");
						dist = xsSqrt(dist);
						xUnitSelect(dPlayerData, xPlayerWorldSplitterLava);
						trUnitChangeProtoUnit("Transport Ship Greek");
						target = trGetNextUnitScenarioNameNumber();
						trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
						trUnitSelectClear();
						trUnitSelect(""+target, true);
						trImmediateUnitGarrison(""+xGetInt(dPlayerData, xPlayerWorldSplitterLava));
						trUnitChangeProtoUnit("Tartarian Gate flame");
						
						xUnitSelect(dPlayerData, xPlayerWorldSplitterLava);
						trUnitChangeProtoUnit("Cinematic Block");

						trUnitSelectClear();
						trUnitSelect(""+target, true);
						trSetSelectedScale(1.0, 1.0, dist * 0.3);
						if (rayCollision(xGetVector(dPlayerData, xPlayerPos, 3 - p), pos, dir, dist + 2.0, 4.0)) {
							damagePlayer(3 - p, dist * 10.0);
						}
						trVectorQuestVarSet("p"+p+"worldSplitterPrev", next);
					}
				}
			case 3:
				{
					scale = 0.012 * (trQuestVarGet("p"+p+"worldSplitterTimeout") - trTimeMS());
					if (scale > 0) {
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"worldSplitterLaser");
						trSetSelectedScale(scale, scale, 50.0);
					} else {
						xUnitSelectByID(dPlayerData, xPlayerUnitID);
						trUnitChangeProtoUnit(xGetString(dPlayerData, xPlayerProto));
						xSetBool(dPlayerData, xPlayerCanCast, true);
						trSoundPlayFN("godpowerfailed.wav");
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"phoenix");
						trUnitChangeProtoUnit("Cinematic Block");
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"worldSplitterLaser");
						trUnitChangeProtoUnit("Cinematic Block");

						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"floater");
						trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
						trQuestVarSet("p"+p+"worldSplitterActive", 0);
					}
				}
			}
		}

		// magic missiles
		if (trQuestVarGet("p"+p+"nickMissiles") > 0) {
			if (trTimeMS() > trQuestVarGet("p"+p+"nickMissilesNext")) {
				trQuestVarSet("p"+p+"nickMissiles", trQuestVarGet("p"+p+"nickMissiles") - 1);
				trQuestVarSet("p"+p+"nickMissilesNext", trQuestVarGet("p"+p+"nickMissilesNext") + 150);
				trQuestVarSetFromRand("rand", 1, 3, true);
				trSoundPlayFN("suckup"+1*trQuestVarGet("rand")+".wav");
				dir = rotationMatrix(trVectorQuestVarGet("p"+p+"nickMissilesDir"), -0.740544, -0.672008);
				trVectorQuestVarSet("p"+p+"nickMissilesDir", dir);
				shootMissile(p, xGetVector(dPlayerData, xPlayerPos), dir, 15.0, true);

				if (trQuestVarGet("p"+p+"nickMissiles") == 0) {
					xUnitSelect(dPlayerData, xPlayerSphinx);
					trUnitOverrideAnimation(-1, 0, false, true, -1);
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				}
			}
		}

		if (trQuestVarGet("p"+p+"hawkActive") > 0) {
			dir = getUnitVector(trVectorQuestVarGet("p"+p+"hawkPos"), xGetVector(dPlayerData, xPlayerPos, 3 - p));
			trVectorQuestVarSet("p"+p+"hawkPos", trVectorQuestVarGet("p"+p+"hawkPos") + dir * timediff * 3.0);
			// update the circle
			dir = vector(8,0,0);
			for(i=trQuestVarGet("p"+p+"hawkWarnStart"); < trQuestVarGet("p"+p+"hawkWarnEnd")) {
				pos = trVectorQuestVarGet("p"+p+"hawkPos") + dir - vector(31,0,31);
				trUnitSelectClear();
				trUnitSelect(""+i);
				trSetSelectedUpVector(xsVectorGetX(pos), -0.8, xsVectorGetZ(pos));
				dir = rotationMatrix(dir, 0.92388, 0.382683);
			}
			if (trTimeMS() > trQuestVarGet("p"+p+"hawkNext")) {
				trQuestVarSet("p"+p+"hawkNext", trQuestVarGet("p"+p+"hawkNext") + 200);
				trQuestVarSet("p"+p+"hawkActive", trQuestVarGet("p"+p+"hawkActive") - 1);

				trQuestVarSetFromRand("randx", -6, 6, false);
				scale = xsSqrt(36.0 - xsPow(trQuestVarGet("randx"), 2));
				trQuestVarSetFromRand("randz", 0.0 - scale, scale, false);
				nickHawkBarrage(p, closestAvailablePos(p, trVectorQuestVarGet("p"+p+"hawkPos") + xsVectorSet(trQuestVarGet("randx"),0,trQuestVarGet("randz"))));
				
				if (trQuestVarGet("p"+p+"hawkActive") == 0) {
					xSetPointer(dPlayerData, p);
					xSetBool(dPlayerData, xPlayerCanCast, true);
					xUnitSelectByID(dPlayerData, xPlayerUnitID);
					trUnitChangeProtoUnit("Dwarf");
					xUnitSelectByID(dPlayerData, xPlayerUnitID);
					trMutateSelected(kbGetProtoUnitID("Hero Greek Odysseus"));
					for(i=trQuestVarGet("p"+p+"hawkWarnStart"); < trQuestVarGet("p"+p+"hawkWarnEnd")) {
						trUnitSelectClear();
						trUnitSelect(""+i);
						trSetSelectedUpVector(0,-10,0);
					}
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"hawk");
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"floater");
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
				} else if (trTimeMS() >= xGetInt(dPlayerData, xPlayerAttackNext)) {
					xSetInt(dPlayerData, xPlayerAttackStep, ATTACK_WINDDOWN);
					xSetInt(dPlayerData, xPlayerAttackNext, xGetInt(dPlayerData, xPlayerEndDelay) + trTimeMS());
					dir = xGetVector(dPlayerData, xPlayerAttackDir);
					switch(xGetInt(dPlayerData, xPlayerAttackAnimation))
					{
					case 1: // zenophobia
						{
							shootLaser(p, xGetInt(dPlayerData, xPlayerSpawner), dir);
							if (trQuestVarGet("p"+p+"barrage") > 0) {
								trQuestVarSet("p"+p+"barrage", trQuestVarGet("p"+p+"barrage") - 1);
								shootLaser(p, xGetInt(dPlayerData, xPlayerSpawner), rotationMatrix(dir, 0.984808, 0.173648));
								shootLaser(p, xGetInt(dPlayerData, xPlayerSpawner), rotationMatrix(dir, 0.984808, -0.173648));
								if (trQuestVarGet("p"+p+"barrage") == 0) {
									xUnitSelect(dPlayerData, xPlayerSphinx);
									trUnitOverrideAnimation(-1, 0, false, true, -1);
									trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
								}
							}
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
						damagePlayer(3 - p, xGetFloat(dPlayerData, xPlayerAttack, p));
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

	// bubbles
	if (xGetDatabaseCount(dBubbles) > 0) {
		xDatabaseNext(dBubbles);
		if (trTimeMS() > xGetInt(dBubbles, xBubbleTimeout)) {
			for(i=xGetInt(dBubbles, xBubbleStart); < xGetInt(dBubbles, xBubbleEnd)) {
				trUnitSelectClear();
				trUnitSelect(""+i, true);
				trUnitDestroy();
			}
		} else {
			center = xGetVector(dBubbles, xBubbleCenter);
			for(i=xGetDatabaseCount(dMissiles); >0) {
				xDatabaseNext(dMissiles);
				pos = xGetVector(dMissiles, xMissilePos);
				if (distanceBetweenVectors(pos, center) < 49.0) {
					dir = xsVectorNormalize(xGetVector(dMissiles, xMissileDir));
					next = pos + dir * 2.0;
					if (distanceBetweenVectors(next, center) >= 49.0) {
						firstDir = rotationMatrix(getUnitVector(pos, center), 0.0, 1.0);
						nextDir = xGetVector(dMissiles, xMissileDir);
						dir = firstDir * dotProduct(firstDir, nextDir);
						dir = nextDir * (-1.0) + dir * 2.0;
						xSetVector(dMissiles, xMissileDir, dir);
						xSetBool(dMissiles, xMissileHoming, false);
						trQuestVarSetFromRand("sound", 1, 3, true);
						trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav");
					}
				}
			}
		}
	}

	// deflector shields
	if (xGetDatabaseCount(dDeflectorShields) > 0) {
		xDatabaseNext(dDeflectorShields);
		// collision detection with bullets
		center = xGetVector(dDeflectorShields, xDeflectorShieldPos);
		for(i=xGetDatabaseCount(dMissiles); >0) {
			xDatabaseNext(dMissiles);
			pos = xGetVector(dMissiles, xMissilePos);
			dir = xsVectorNormalize(xGetVector(dMissiles, xMissileDir));
			next = pos + dir * 2.0;
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
						dir = firstDir * dotProduct(firstDir, nextDir);
						dir = nextDir * (-1.0) + dir * 2.0;
						xSetVector(dMissiles, xMissileDir, dir);
						xSetBool(dMissiles, xMissileHoming, false);
						trQuestVarSetFromRand("sound", 1, 3, true);
						trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav");
					}
				}
			}
		}
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
		if (xGetBool(dMissiles, xMissileHoming)) {
			dir = xGetVector(dMissiles, xMissileDir) / (1.0 + timediff * xGetFloat(dPlayerData, xPlayerBulletSpeed, p));
			scale = timediff * 20.0 * xGetFloat(dPlayerData, xPlayerBulletSpeed, p);
			xSetVector(dMissiles, xMissileDir, dir + getUnitVector(xGetVector(dMissiles, xMissilePos), xGetVector(dPlayerData, xPlayerPos, 3 - p), scale));
		}
		pos = xGetVector(dMissiles, xMissilePos) + xGetVector(dMissiles, xMissileDir) * timediff;
		dir = (pos - vector(31, 0, 31)) * 3.33;
		xUnitSelectByID(dMissiles, xUnitID);
		trSetSelectedUpVector(xsVectorGetX(dir),0.2,xsVectorGetZ(dir));
		xSetVector(dMissiles, xMissilePos, pos);
		dist = distanceBetweenVectors(pos, xGetVector(dMissiles, xMissilePrev));
		if (dist > 2.0) {
			pos = xGetVector(dMissiles, xMissilePrev);
			dist = xsSqrt(dist);
			if (xGetBool(dMissiles, xMissileHoming)) {
				dir = xsVectorNormalize(xGetVector(dMissiles, xMissileDir));
				if ((8.0 * dotProduct(dir, getUnitVector(xGetVector(dMissiles, xMissilePos), xGetVector(dPlayerData, xPlayerPos, 3 - p)))) > 
					distanceBetweenVectors(xGetVector(dMissiles, xMissilePos), xGetVector(dPlayerData, xPlayerPos, 3 - p), false)) {
					xSetBool(dMissiles, xMissileHoming, false);
				}
			}
			if (rayCollision(xGetVector(dPlayerData, xPlayerPos, 3 - p), pos, xsVectorNormalize(xGetVector(dMissiles, xMissileDir)), dist + 1.0, 1.0)) {
				if (damagePlayer(3 - p, xGetFloat(dPlayerData, xPlayerAttack, p))) {
					xUnitSelectByID(dMissiles, xUnitID);
					trUnitDestroy();
					xFreeDatabaseBlock(dMissiles);
				} else {
					xSetVector(dMissiles, xMissilePrev, xGetVector(dMissiles, xMissilePos));
				}
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
		/*
		if (xGetInt(dTurrets, xTurretType) == NICK_ABILITIES) {
			pos = xGetVector(dTurrets, xTurretPos);
			dir = getUnitVector(pos, xGetVector(dPlayerData, xPlayerPos, 3 - p));
			xUnitSelectByID(dTurrets, xUnitID);
			trSetSelectedUpVector(xsVectorGetX(dir), -1.0, xsVectorGetZ(dir));
		}
		*/

		if (trTimeMS() > xGetInt(dTurrets, xTurretCooldown)) {
			if (xGetBool(dPlayerData, xPlayerAlive, 3 - p)) {
				xSetInt(dTurrets, xTurretCooldown, xGetInt(dTurrets, xTurretCooldown) + 5000 / xGetFloat(dPlayerData, xPlayerTurretSpeed, p));
				dir = getUnitVector(xGetVector(dTurrets, xTurretPos), xGetVector(dPlayerData, xPlayerPos, 3 - p));
				if (xGetInt(dTurrets, xTurretType) == ZENO_ABILITIES) {
					shootLaser(p, xGetInt(dTurrets, xUnitName), dir);
					xUnitSelectByID(dTurrets, xUnitID);
					trUnitChangeProtoUnit("Spy Eye");
					xUnitSelectByID(dTurrets, xUnitID);
					trMutateSelected(kbGetProtoUnitID("Tower Mirror"));
					trSetSelectedScale(0, 0.1, 0);
				} else if (xGetInt(dTurrets, xTurretType) == NICK_ABILITIES) {
					shootMissile(p, xGetVector(dTurrets, xTurretPos), dir);
				}
			} else {
				xSetInt(dTurrets, xTurretCooldown, trTimeMS());
			}
		}
	}

	// hawk barrages
	if (xGetDatabaseCount(dHawkBarrages) > 0) {
		xDatabaseNext(dHawkBarrages);
		if (trTimeMS() > xGetInt(dHawkBarrages, xHawkBarrageTimeout)) {
			p = xGetInt(dHawkBarrages, xOwner);
			pos = xGetVector(dHawkBarrages, xHawkBarragePos);
			if (distanceBetweenVectors(xGetVector(dPlayerData, xPlayerPos, 3 - p), pos) < 9.0) {
				damagePlayer(3 - p, 2.0 * xGetFloat(dPlayerData, xPlayerAttack, p));
			}
			xFreeDatabaseBlock(dHawkBarrages);
		}
	}

	if (xGetDatabaseCount(dHawkBombs) > 0) {
		xDatabaseNext(dHawkBombs);
		switch(xGetInt(dHawkBombs, xHawkBombStep))
		{
		case 0:
			{
				if (trTimeMS() > xGetInt(dHawkBombs, xHawkBombTimeout)) {
					xUnitSelectByID(dHawkBombs, xUnitID);
					trDamageUnitPercent(100);
					trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
					trSetSelectedScale(0,2,0);
					xSetInt(dHawkBombs, xHawkBombStep, 1);
					xSetInt(dHawkBombs, xHawkBombTimeout, trTimeMS() + 1500);
				}
			}
		case 1:
			{
				xSetInt(dHawkBombs, xHawkBombStep, 2);
				xUnitSelectByID(dHawkBombs, xUnitID);
				trMutateSelected(kbGetProtoUnitID("Lampades"));
				trUnitOverrideAnimation(18,0,false,false,-1);
			}
		case 2:
			{
				if (trTimeMS() > xGetInt(dHawkBombs, xHawkBombTimeout)) {
					p = xGetInt(dHawkBombs, xOwner);
					pos = xGetVector(dHawkBombs, xHawkBombPos);
					if (distanceBetweenVectors(xGetVector(dPlayerData, xPlayerPos, 3 - p), pos) < 9.0) {
						damagePlayer(3 - p, 3.0 * xGetFloat(dPlayerData, xPlayerAttack, p));
					}
					dir = vector(1,0,0);
					for(i=16; >0) {
						shootMissile(p, pos, dir);
						dir = rotationMatrix(dir, 0.92388, 0.382683);
					}

					trSoundPlayFN("meteordustcloud.wav");

					xUnitSelectByID(dHawkBombs, xUnitID);
					trUnitChangeProtoUnit("Implode Sphere Effect");

					xFreeDatabaseBlock(dHawkBombs);
				}
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
						trSetSelectedScale(12.0, 12.0, 40.0);
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
		scale = 0.024 * (xGetInt(dCarousels, xCarouselTimeout) - trTimeMS());
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
				if (rayCollision(xGetVector(dPlayerData, xPlayerPos, 3 - p), pos, dir, 80.0, 2.0)) {
					damagePlayer(3 - p, timediff * xGetFloat(dPlayerData, xPlayerAttack, p));
					db = 100; // no need to check the rest
				}
			}
			dir = rotationMatrix(dir, 0.707107, 0.707107);
		}
	}
}
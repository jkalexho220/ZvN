void nickVolley(int p = 0) {
	vector pos = xGetVector(dPlayerData, xPlayerPos);
	vector dir = getUnitVector(pos, xGetVector(dPlayerData, xPlayerCastPos));
	dir = rotationMatrix(dir, 0.866025, 0.5); // 30 degrees
	for(i=0; < 7) {
		shootMissile(p, pos, dir);
		dir = rotationMatrix(dir, 0.984808, -0.173648); // -10 degrees
	}
	trSoundPlayFN("manticorespecialattack.wav");
}

void nickTurret(int p = 0) {
	vector pos = closestAvailablePos(p, xGetVector(dPlayerData, xPlayerCastPos, p));
	xAddDatabaseBlock(dTurrets, true);
	xSetVector(dTurrets, xTurretPos, pos);
	xSetInt(dTurrets, xTurretCooldown, trTimeMS() + 1000);
	xSetInt(dTurrets, xTurretType, NICK_ABILITIES);
	xSetInt(dTurrets, xOwner, p);
	xSetInt(dTurrets, xUnitName, trGetNextUnitScenarioNameNumber());
	trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos), 0, xsVectorGetZ(pos), 0, true);
	xSetInt(dTurrets, xUnitID, kbGetBlockID(""+xGetInt(dTurrets, xUnitName), true));

	xUnitSelectByID(dTurrets, xUnitID);
	trUnitChangeProtoUnit("Spy Eye");
	xUnitSelectByID(dTurrets, xUnitID);
	trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));

	trSoundPlayFN("outpost.wav");
}

void nickTeleport(int p = 0) {
	vector pos = xGetVector(dPlayerData, xPlayerPos);
	int next = trGetNextUnitScenarioNameNumber();
	deployAtVector(p, "Dwarf", 1, pos);
	xUnitSelect(dPlayerData, xPlayerSpawner);
	trUnitChangeProtoUnit("Transport Ship Greek");
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trImmediateUnitGarrison(""+xGetInt(dPlayerData, xPlayerSpawner));
	trUnitChangeProtoUnit("Deconstruct Unit");
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trUnitOverrideAnimation(18, 0, false, true, -1);

	xUnitSelect(dPlayerData, xPlayerSpawner);
	trUnitChangeProtoUnit("Cinematic Block");

	pos = closestAvailablePos(p, xGetVector(dPlayerData, xPlayerCastPos));
	next = trGetNextUnitScenarioNameNumber();
	deployAtVector(p, "Dwarf", 1, pos);
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trUnitChangeProtoUnit("Transport Ship Greek");
	trSetUnitOrientation(getUnitVector(xGetVector(dPlayerData, xPlayerPos), pos), vector(0,1,0), true);

	xUnitSelectByID(dPlayerData, xPlayerUnitID);
	trImmediateUnitGarrison(""+next);
	trUnitChangeProtoUnit("Dwarf");
	xUnitSelectByID(dPlayerData, xPlayerUnitID);
	trMutateSelected(kbGetProtoUnitID("Hero Greek Odysseus"));

	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trUnitChangeProtoUnit("Arkantos God Out");
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trUnitSetAnimationPath("0,1,1,1");

	trSoundPlayFN("sphinxteleportout.wav");
}

void nickSplit(int p = 0) {
	trSoundPlayFN("changeunit.wav");
	vector dir = vector(0,0,0);
	float speed = 0;
	int index = 0;
	for(i=xGetDatabaseCount(dMissiles); >0) {
		index = xDatabaseNext(dMissiles, true);
		if (xGetInt(dMissiles, xOwner) == p) {
			dir = xGetVector(dMissiles, xMissileDir);
			xSetVector(dMissiles, xMissileDir, rotationMatrix(dir, 0.984808, 0.173648));
			/*
			shootMissile(p, xGetVector(dMissiles, xMissilePos), rotationMatrix(dir, 0.984808, 0.173648), 
				1.0 / xGetFloat(dPlayerData, xPlayerBulletSpeed, p), xGetBool(dMissiles, xMissileHoming));
			*/
			shootMissile(p, xGetVector(dMissiles, xMissilePos), rotationMatrix(dir, 0.984808, -0.173648), 
				1.0 / xGetFloat(dPlayerData, xPlayerBulletSpeed, p), xGetBool(dMissiles, xMissileHoming));
			xSetPointer(dMissiles, index);
		}
	}
}

void nickMirror(int p = 0) {
	trSoundPlayFN("sentinelbirth.wav");
	int next = 0;
	vector pos = closestAvailablePos(p, xGetVector(dPlayerData, xPlayerCastPos));
	vector dir = vector(1,0,0);
	vector truedir = vector(1,0,0);
	xAddDatabaseBlock(dBubbles, true);
	xSetVector(dBubbles, xBubbleCenter, pos);
	xSetInt(dBubbles, xBubbleStart, trGetNextUnitScenarioNameNumber());
	for(i=0; < 32) {
		next = trGetNextUnitScenarioNameNumber();
		trArmyDispatch("0,0","Dwarf",1,31,0,31,0,true);
		trUnitSelectClear();
		trUnitSelect(""+next, true);
		trMutateSelected(kbGetProtoUnitID("UI Range Indicator Norse SFX"));
		trSetUnitOrientation(dir, vector(0,1,0), true);
		truedir = pos - vector(31, 0, 31) + dir * 7.0;
		trSetSelectedUpVector(xsVectorGetX(truedir), 0, xsVectorGetZ(truedir));
		dir = rotationMatrix(dir, 0.980785, 0.19509); //11.25 degrees
	}
	xSetInt(dBubbles, xBubbleEnd, trGetNextUnitScenarioNameNumber());
	xSetInt(dBubbles, xBubbleTimeout, trTimeMS() + 9000);
}

void nickMissiles(int p = 0) {
	if (trQuestVarGet("p"+p+"nickMissiles") == 0) {
		trVectorQuestVarSet("p"+p+"nickMissilesDir", vector(1,0,0));
		trQuestVarSet("p"+p+"nickMissilesNext", trTimeMS());
		xUnitSelect(dPlayerData, xPlayerSphinx);
		trMutateSelected(kbGetProtoUnitID("Arkantos God"));
		trSetSelectedScale(0,0,0);
		trUnitOverrideAnimation(26, 0, false, false, -1);
	}
	trQuestVarSet("p"+p+"nickMissiles", trQuestVarGet("p"+p+"nickMissiles") + 10);
}

void nickOnHawk(int p = 0) {
	vector pos = closestAvailablePos(p, xGetVector(dPlayerData, xPlayerCastPos));
	vector dir = getUnitVector(xGetVector(dPlayerData, xPlayerPos), pos);
	trVectorQuestVarSet("p"+p+"hawkPos", pos);
	if (trQuestVarGet("p"+p+"hawk") == 0) {
		trQuestVarSet("p"+p+"hawk", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
		trQuestVarSet("p"+p+"floater", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
		trQuestVarSet("p"+p+"hawkWarnStart", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+p+",0","Dwarf",16,31,0,31,0,true);
		trArmySelect(""+p+",0");
		trMutateSelected(kbGetProtoUnitID("UI Range Indicator Norse SFX"));
		trQuestVarSet("p"+p+"hawkWarnEnd", trGetNextUnitScenarioNameNumber());
	}

	xUnitSelectByID(dPlayerData, xPlayerUnitID);
	trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
	trSetUnitOrientation(dir, vector(0,1,0), true);

	dir = vector(8,0,0);
	for(i=trQuestVarGet("p"+p+"hawkWarnStart"); < trQuestVarGet("p"+p+"hawkWarnEnd")) {
		pos = xGetVector(dPlayerData, xPlayerCastPos) + dir - vector(31,0,31);
		trUnitSelectClear();
		trUnitSelect(""+i);
		trSetSelectedUpVector(xsVectorGetX(pos), -0.8, xsVectorGetZ(pos));
		dir = rotationMatrix(dir, 0.92388, 0.382683);
	}

	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"floater");
	trMutateSelected(kbGetProtoUnitID("Dwarf"));
	trImmediateUnitGarrison(""+xGetInt(dPlayerData, xPlayerUnit));
	trUnitChangeProtoUnit("Stymphalian Bird");

	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"hawk");
	trUnitChangeProtoUnit("Stymphalian Bird");

	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"floater");
	trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));

	xUnitSelectByID(dPlayerData, xPlayerUnitID);
	trMutateSelected(kbGetProtoUnitID("Relic"));
	trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"floater"));
	trMutateSelected(kbGetProtoUnitID("Hero Greek Odysseus"));

	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"hawk");
	trMutateSelected(kbGetProtoUnitID("Relic"));
	trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"floater"));
	trMutateSelected(kbGetProtoUnitID("Stymphalian Bird"));

	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"floater");
	trMutateSelected(kbGetProtoUnitID("Wadjet Spit"));

	trQuestVarSet("p"+p+"hawkActive", 20);
	trQuestVarSet("p"+p+"hawkNext", trTimeMS());

	xSetBool(dPlayerData, xPlayerCanCast, false);

	trSoundPlayFN("attackwarning.wav");
	trSoundPlayFN("birdtitanbirth.wav");
}

void nickHawkBarrage(int p = 0, vector pos = vector(0,0,0)) {
	vector dir = getUnitVector(xGetVector(dPlayerData, xPlayerPos), trVectorQuestVarGet("p"+p+"hawkPos"));
	pos = closestAvailablePos(p, pos);
	xAddDatabaseBlock(dHawkBarrages, true);
	
	xSetInt(dHawkBarrages, xOwner, p);
	xSetInt(dHawkBarrages, xHawkBarrageTimeout, trTimeMS() + 500);
	xSetVector(dHawkBarrages, xHawkBarragePos, pos);

	trQuestVarSetFromRand("rand", 1, 3, true);
	trSoundPlayFN("rainofarrows"+1*trQuestVarGet("rand")+".wav");

	int next = trGetNextUnitScenarioNameNumber();
	deployAtVector(p, "Dwarf", 1, pos);
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trSetUnitOrientation(dir, vector(0,1,0), true);
	trUnitChangeProtoUnit("Barrage");

	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"floater");
	trSetUnitOrientation(dir, vector(0,1,0), true);
}

void nickBombAtPos(int p = 0, vector pos = vector(0,0,0)) {
	xAddDatabaseBlock(dHawkBombs, true);
	xSetInt(dHawkBombs, xOwner, p);
	xSetInt(dHawkBombs, xUnitName, trGetNextUnitScenarioNameNumber());
	deployAtVector(p, "Dwarf", 1, pos);
	xSetInt(dHawkBombs, xUnitID, kbGetBlockID(""+xGetInt(dHawkBombs, xUnitName), true));
	xSetInt(dHawkBombs, xHawkBombTimeout, trTimeMS() + 1000 / xGetFloat(dPlayerData, xPlayerBulletSpeed, p));
	xSetVector(dHawkBombs, xHawkBombPos, pos);

	xUnitSelectByID(dHawkBombs, xUnitID);
	trMutateSelected(kbGetProtoUnitID("UI Range Indicator Norse SFX"));
}

void nickBomb(int p = 0) {
	vector pos = closestAvailablePos(p, xGetVector(dPlayerData, xPlayerCastPos));
	nickBombAtPos(p, pos);

	trSoundPlayFN("catapultattack.wav");
}

void nickSingularity(int p = 0) {
	xSetBool(dPlayerData, xPlayerCanCast, false);
	xUnitSelectByID(dPlayerData, xPlayerUnitID);
	trUnitChangeProtoUnit("Cinematic Block");

	trSoundPlayFN("cinematics\15_in\gong.wav");
	trSoundPlayFN("godpower.wav");
	trSetLighting("night", 1.0);
	trOverlayText("Random Number Generator", 3.0);

	trQuestVarSet("singularityTimeout", trTimeMS() + 12000);
	trQuestVarSet("singularityNext", trTimeMS() + 1000);
	trQuestVarSet("singularityDelay", 8);
	trQuestVarSet("singularityPlayer", p);

	xsEnableRule("singularity_active");
}

rule singularity_active
inactive
highFrequency
{
	int p = trQuestVarGet("singularityPlayer");
	if (trTimeMS() > trQuestVarGet("singularityNext")) {
		trQuestVarSet("singularityNext", trQuestVarGet("singularityNext") + 8000 / trQuestVarGet("singularityDelay"));
		trQuestVarSet("singularityDelay", trQuestVarGet("singularityDelay") + 1);
		trQuestVarSetFromRand("randx", -16, 16, false);
		float scale = xsSqrt(256.0 - xsPow(trQuestVarGet("randx"), 2));
		trQuestVarSetFromRand("randz", 0.0 - scale, scale, false);
		nickBombAtPos(p, xsVectorSet(33 + trQuestVarGet("randx"), 0, 33 + trQuestVarGet("randz")));
	}

	if (trTimeMS() > trQuestVarGet("singularityTimeout")) {
		xsDisableSelf();
		ultimate = false;
		trSetLighting("default", 1.0);
		trSoundPlayFN("godpowerfailed.wav");
		xSetPointer(dPlayerData, p);
		xUnitSelectByID(dPlayerData, xPlayerUnitID);
		xSetBool(dPlayerData, xPlayerCanCast, true);
		trUnitChangeProtoUnit("Hero Greek Odysseus");
	}
}
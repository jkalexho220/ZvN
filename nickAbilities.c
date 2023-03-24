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
			shootMissile(p, xGetVector(dMissiles, xMissilePos), rotationMatrix(dir, 0.984808, -0.173648), 1.0, xGetBool(dMissiles, xMissileHoming));
			xSetPointer(dMissiles, index);
		}
	}
}

void nickMirror(int p = 0) {

}

void nickMissiles(int p = 0) {
	if (trQuestVarGet("p"+p+"nickMissiles") == 0) {
		trVectorQuestVarSet("p"+p+"nickMissilesDir", vector(1,0,0));
		trQuestVarSet("p"+p+"nickMissilesNext", trTimeMS());
	}
	trQuestVarSet("p"+p+"nickMissiles", trQuestVarGet("p"+p+"nickMissiles") + 12);
}

void nickOnHawk(int p = 0) {

}

void nickSingularity(int p = 0) {

}
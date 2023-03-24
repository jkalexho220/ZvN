void zenoLaserGrid(int p = 0) {
	vector pos = vectorSnapToGrid(xGetVector(dPlayerData, xPlayerCastPos, p));
	vector loc = vector(0,0,0);
	int x = 0;
	int z = 0;
	for(x=0; < 4) {
		loc = pos + xsVectorSet(x * 4 - 6, 0, -6);
		if (positionInArena(loc)) {
			shootLaserGround(p, loc, vector(0,0,1), 12.0);
		} else if (positionInArena(loc + vector(0,0,12))) {
			shootLaserGround(p, loc + vector(0,0,12), vector(0,0,-1), 12.0);
		}
	}

	for(z=0; < 4) {
		loc = pos + xsVectorSet(-6, 0, z * 4 - 6);
		if (positionInArena(loc)) {
			shootLaserGround(p, loc, vector(1,0,0), 12.0);
		} else if (positionInArena(loc + vector(12,0,0))) {
			shootLaserGround(p, loc + vector(12,0,0), vector(-1,0,0), 12.0);
		}
	}
	trSoundPlayFN("automatonspcbirth.wav");
}

void zenoTurret(int p = 0) {
	vector pos = vectorSnapToGrid(xGetVector(dPlayerData, xPlayerCastPos, p));
	if (positionInArena(pos) == false) {
		vector dir = getUnitVector(pos, xGetVector(dPlayerData, xPlayerPos, p), 2.0);
		for(i = distanceBetweenVectors(pos, xGetVector(dPlayerData, xPlayerPos, p), false) / 2; >0) {
			pos = pos + dir;
			if (positionInArena(pos)) {
				pos = vectorSnapToGrid(pos);
				break;
			}
		}
	}
	xAddDatabaseBlock(dTurrets, true);
	xSetVector(dTurrets, xTurretPos, pos);
	xSetInt(dTurrets, xTurretCooldown, trTimeMS() + 1000);
	xSetInt(dTurrets, xTurretType, ZENO_ABILITIES);
	xSetInt(dTurrets, xOwner, p);
	xSetInt(dTurrets, xUnitName, trGetNextUnitScenarioNameNumber());
	trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos), 0, xsVectorGetZ(pos), 0, true);
	xSetInt(dTurrets, xUnitID, kbGetBlockID(""+xGetInt(dTurrets, xUnitName), true));

	xUnitSelectByID(dTurrets, xUnitID);
	trUnitChangeProtoUnit("Spy Eye");
	xUnitSelectByID(dTurrets, xUnitID);
	trMutateSelected(kbGetProtoUnitID("Tower Mirror"));
	trSetSelectedScale(0, 0.15, 0);
}

void zenoSpeed(int p = 0) {
	if (trQuestVarGet("p"+p+"speed") == 0) {
		trQuestVarSet("p"+p+"speed", 1);
		trModifyProtounit("Hoplite", p, 1, 5);
	}
	trQuestVarSet("p"+p+"speedTimeout", trTimeMS() + 5000);
	trUnitSelectClear();
	trUnitSelectByID(xGetInt(dPlayerData, xPlayerUnitID));
	trUnitHighlight(5.0, false);
	trSoundPlayFN("vortexstart.wav");
}

void zenoShield(int p = 0) {
	vector pos = vectorSnapToGrid(xGetVector(dPlayerData, xPlayerCastPos, p));
	vector start = xGetVector(dPlayerData, xPlayerPos, p);
	vector dir = getUnitVector(start, pos);

	xAddDatabaseBlock(dDeflectorShields, true);
	xSetInt(dDeflectorShields, xDeflectorShieldLeft, trGetNextUnitScenarioNameNumber());
	trArmyDispatch("0,0", "Dwarf", 1, xsVectorGetX(pos), 0, xsVectorGetZ(pos), 0, true);
	xSetInt(dDeflectorShields, xDeflectorShieldRight, trGetNextUnitScenarioNameNumber());
	trArmyDispatch("0,0", "Dwarf", 1, xsVectorGetX(pos), 0, xsVectorGetZ(pos), 0, true);

	xUnitSelect(dDeflectorShields, xDeflectorShieldLeft);
	trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
	trSetUnitOrientation(rotationMatrix(dir, 0.0, 1.0));
	trSetSelectedScale(1.0, 1.0, 5.0);

	xUnitSelect(dDeflectorShields, xDeflectorShieldLeft);
	trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
	trSetUnitOrientation(rotationMatrix(dir, 0.0, -1.0));
	trSetSelectedScale(1.0, 1.0, 5.0);

	dir = rotationMatrix(dir, 0.0, 1.0);
	xSetVector(dDeflectorShields, xDeflectorShieldPos, pos - dir * 4.0);
	xSetVector(dDeflectorShields, xDeflectorShieldDir, dir);
}

void zenoBarrage(int p = 0) {

}

void zenoCarousel(int p = 0) {

}

void zenoWorldSplitter(int p = 0) {

}
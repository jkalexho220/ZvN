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
	vector pos = closestAvailablePos(p, xGetVector(dPlayerData, xPlayerCastPos, p));
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
	trSetSelectedScale(0, 0.1, 0);

	trSoundPlayFN("mirrortower.wav");
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
	trSetUnitOrientation(rotationMatrix(dir, 0.0, 1.0), vector(0,1,0), true);
	trSetSelectedScale(1.0, 1.0, 5.0);
	trUnitHighlight(10.0, false);

	xUnitSelect(dDeflectorShields, xDeflectorShieldRight);
	trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
	trSetUnitOrientation(rotationMatrix(dir, 0.0, -1.0), vector(0,1,0), true);
	trSetSelectedScale(1.0, 1.0, 5.0);
	trUnitHighlight(10.0, false);

	dir = rotationMatrix(dir, 0.0, 1.0);
	xSetVector(dDeflectorShields, xDeflectorShieldPos, pos - dir * 4.0);
	xSetVector(dDeflectorShields, xDeflectorShieldDir, dir);

	xSetInt(dDeflectorShields, xDeflectorShieldTimeout, trTimeMS() + 500);

	trSoundPlayFN("petsuchosattack.wav");
}

void zenoBarrage(int p = 0) {
	xUnitSelect(dPlayerData, xPlayerSphinx);
	trMutateSelected(kbGetProtoUnitID("Sphinx"));
	trSetSelectedScale(0,0,0);
	trUnitOverrideAnimation(39, 0, false, false, -1);
	if (trQuestVarGet("p"+p+"barrage") == 0) {
		trSoundPlayFN("lapadesconvert.wav");
		trQuestVarSet("p"+p+"barrageNext", trTimeMS() + 100);
		trVectorQuestVarSet("p"+p+"barrageDir", getUnitVector(xGetVector(dPlayerData, xPlayerPos, 3 - p), xGetVector(dPlayerData, xPlayerPos)));
	}
	trQuestVarSet("p"+p+"barrage", trQuestVarGet("p"+p+"barrage") + 24);
}

void zenoCarousel(int p = 0) {
	vector pos = closestAvailablePos(p, xGetVector(dPlayerData, xPlayerCastPos, p));
	xAddDatabaseBlock(dCarousels, true);
	xSetInt(dCarousels, xOwner, p);
	xSetVector(dCarousels, xCarouselPos, pos);
	xSetInt(dCarousels, xCarouselStart, trGetNextUnitScenarioNameNumber());
	trArmyDispatch("0,0","Dwarf",8,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
	xSetInt(dCarousels, xCarouselEnd, trGetNextUnitScenarioNameNumber());
	xSetInt(dCarousels, xCarouselTimeout, trTimeMS() + 2000);
	for(i=xGetInt(dCarousels, xCarouselStart); < xGetInt(dCarousels, xCarouselEnd)) {
		trUnitSelectClear();
		trUnitSelect(""+i, true);
		trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
		trSetSelectedScale(5.0, 5.0, 40.0);
	}
	trSoundPlayFN("attackwarning.wav");
	trSoundPlayFN("lapadesconvert.wav");
}

void zenoWorldSplitter(int p = 0) {
	xSetBool(dPlayerData, xPlayerCanCast, false, p); // there can only be one
	vector dir = getUnitVector(xGetVector(dPlayerData, xPlayerPos), xGetVector(dPlayerData, xPlayerCastPos));
	if (trQuestVarGet("p"+p+"phoenix") == 0) {
		trQuestVarSet("p"+p+"phoenix", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+p+",0","Phoenix",1,1,0,1,0,true);
		trQuestVarSet("p"+p+"floater", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+p+",0","Phoenix",1,1,0,1,0,true);

		trQuestVarSet("p"+p+"meteorite", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+p+",0", "Dwarf", 1, 1, 0, 1, 0, true);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"meteorite");
		spyEffect(kbGetProtoUnitID("Cinematic Block"), -1, xsVectorSet(dPlayerData, xPlayerWorldSplitterLava, p), vector(0,0,0));

		trQuestVarSet("p"+p+"worldSplitterLaser", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+p+",0", "Dwarf", 1, 1, 0, 1, 0, true);
	}

	zSetProtoUnitStat("Meteorite", p, 1, 1);

	xUnitSelectByID(dPlayerData, xPlayerUnitID);
	trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));

	// garrison floater and meteorite
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"floater", true);
	trMutateSelected(kbGetProtoUnitID("Dwarf"));
	trImmediateUnitGarrison(""+xGetInt(dPlayerData, xPlayerUnit));
	trUnitChangeProtoUnit("Phoenix");
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"meteorite", true);
	trMutateSelected(kbGetProtoUnitID("Dwarf"));
	trImmediateUnitGarrison(""+xGetInt(dPlayerData, xPlayerUnit));
	trUnitChangeProtoUnit("Dwarf");

	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"meteorite");
	trSetSelectedScale(0,0,0);
	trSetUnitOrientation(dir, vector(0,1,0), true);

	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"floater");
	trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));

	xUnitSelectByID(dPlayerData, xPlayerUnitID);
	trMutateSelected(kbGetProtoUnitID("Relic"));
	trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"floater"));
	trMutateSelected(kbGetProtoUnitID("Hoplite"));

	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"phoenix");
	trMutateSelected(kbGetProtoUnitID("Phoenix"));
	trUnitOverrideAnimation(2, 0, true, false, -1);
	trMutateSelected(kbGetProtoUnitID("Relic"));
	trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"floater"));
	trMutateSelected(kbGetProtoUnitID("Phoenix"));

	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"worldSplitterLaser");
	trMutateSelected(kbGetProtoUnitID("Relic"));
	trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"floater"));
	trUnitChangeProtoUnit("Phoenix");
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"worldSplitterLaser");
	trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));

	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"floater");
	trMutateSelected(kbGetProtoUnitID("Wadjet Spit"));
	trSetUnitOrientation(dir, vector(0,1,0), true);

	trQuestVarSet("p"+p+"worldSplitterActive", 1);

	trVectorQuestVarSet("p"+p+"worldSplitterDir", dir);
	trVectorQuestVarSet("p"+p+"worldSplitterPrev", xGetVector(dPlayerData, xPlayerPos));

	trQuestVarSet("p"+p+"worldSplitterTimeout", trTimeMS() + 1000);
	trQuestVarSet("p"+p+"worldSplitterHit", 0);

	trSoundPlayFN("phoenixselect2.wav");
	trSoundPlayFN("attackwarning.wav");
}
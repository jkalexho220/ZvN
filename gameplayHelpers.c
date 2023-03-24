int spyreset = 0;
int spysearch = 0;

void reselectMyself() {
	uiClearSelection();
	int p = trCurrentPlayer();
	string proto = xGetString(dPlayerData, xPlayerProto, p);
	trackInsert();
	trackAddWaypoint();
	trackAddWaypoint();
	trBlockAllSounds(false);
	uiFindType(proto);
	trackPlay(1,EVENT_REMOVE_CAM_TRACKS);
}

void damagePlayer(int p = 0, float amt = 0) {
	trUnitSelectClear();
	trUnitSelectByID(xGetInt(dPlayerData, xPlayerUnitID, p));
	trDamageUnit(amt);
	vector pos = xGetVector(dPlayerData, xPlayerPos, p);
	trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos), 0, xsVectorGetZ(pos),0,true);
	trArmySelect("0,0");
	//trDamageUnitPercent(100);
	trUnitChangeProtoUnit("Lightning Sparks");
	trQuestVarSetFromRand("sound", 1, 5, true);
	trSoundPlayFN("ui\lightning"+1*trQuestVarGet("sound")+".wav");
}

bool positionInArena(vector pos = vector(0,0,0)) {
	vector loc = vectorToGrid(pos);
	return(trGetTerrainSubType(xsVectorGetX(loc), xsVectorGetZ(loc)) == 53);
}

/*
Assumes that the target unit is already selected
*/
void spyEffect(int proto = 0, int anim = 0, vector dest = vector(0,0,0), vector scale = vector(1,1,1)) {
	int newest = xAddDatabaseBlock(dSpyRequests);
	xSetInt(dSpyRequests, xSpyRequestProto, proto, newest);
	xSetInt(dSpyRequests, xSpyRequestAnim, anim, newest);
	xSetVector(dSpyRequests, xSpyRequestDest, dest, newest);
	xSetVector(dSpyRequests, xSpyRequestScale, scale, newest);
	trTechInvokeGodPower(0, "spy", vector(0,0,0), vector(0,0,0));
}

bool rayCollision(vector pos = vector(0,0,0), vector start = vector(0,0,0), vector dir = vector(0,0,0), float dist = 0, float width = 0) {
	float current = distanceBetweenVectors(pos, start, false);
	if (current < dist) {
		vector hitbox = start + dir * current;
		if (distanceBetweenVectors(pos, hitbox, true) <= width) {
			return(true);
		}
	}
	return(false);
}

vector vectorSetAsTargetVector(vector from = vector(0,0,0), vector dir = vector(0,0,0), float dist = 40.0) {
	vector target = xsVectorSet((xsVectorGetX(dir) * dist) + xsVectorGetX(from), 0, (xsVectorGetZ(dir) * dist) + xsVectorGetZ(from));
	
	/*
	No out-of-bounds allowed
	*/
	float scale = 0;
	if (xsVectorGetX(target) < 0) {
		scale = xsVectorGetX(target) / (xsVectorGetX(target) - xsVectorGetX(from));
		target = xsVectorSet(0,0, xsVectorGetZ(target) + scale * (xsVectorGetZ(from) - xsVectorGetZ(target)));
	} else if (xsVectorGetX(target) > (mapSize * 2 + 1)) {
		scale = (xsVectorGetX(target) - (mapSize * 2 + 1)) / (xsVectorGetX(target) - xsVectorGetX(from));
		target = xsVectorSet(mapSize * 2 + 1,0,xsVectorGetZ(target) + scale * (xsVectorGetZ(from) - xsVectorGetZ(target)));
	}
	
	if (xsVectorGetZ(target) < 0) {
		scale = xsVectorGetZ(target) / (xsVectorGetZ(target) - xsVectorGetZ(from));
		target = xsVectorSet(xsVectorGetX(target) + scale * (xsVectorGetX(from) - xsVectorGetX(target)),0,0);
	} else if (xsVectorGetZ(target) > (mapSize * 2 + 1)) {
		scale = (xsVectorGetZ(target) - (mapSize * 2 + 1)) / (xsVectorGetZ(target) - xsVectorGetZ(from));
		target = xsVectorSet(xsVectorGetX(target) + scale * (xsVectorGetX(from) - xsVectorGetX(target)),0,mapSize * 2 + 1);
	}
	return(target);
}

bool vectorInMap(vector pos = vector(0,0,0)) {
	return((xsVectorGetX(pos) > 0) && (xsVectorGetZ(pos) > 0) && (xsVectorGetX(pos) < (2.0 * mapSize)) && (xsVectorGetZ(pos) < (2.0 * mapSize)));
}

void spawnPlayer(int p = 0, vector pos = vector(0,0,0)) {
	xSetPointer(dPlayerData, p);
	xSetInt(dPlayerData, xPlayerUnit, trGetNextUnitScenarioNameNumber());
	trArmyDispatch(""+p+",0", "Dwarf", 1, xsVectorGetX(pos), 0, xsVectorGetZ(pos), 0, true);
	xSetInt(dPlayerData, xPlayerUnitID, kbGetBlockID(""+xGetInt(dPlayerData, xPlayerUnit), true));
	
	xUnitSelectByID(dPlayerData, xPlayerUnitID);
	spyEffect(kbGetProtoUnitID("Cinematic Block"), -1, xsVectorSet(dPlayerData, xPlayerSpawner, p));
	spyEffect(kbGetProtoUnitID("Cinematic Block"), -1, xsVectorSet(dPlayerData, xPlayerSphinx, p));
	vector dir = getUnitVector(pos, xsVectorSet(mapSize, 0, mapSize), 1.0);
	trSetUnitOrientation(dir, vector(0,1,0), true);
	trUnitChangeProtoUnit(xGetString(dPlayerData, xPlayerProto));
	
	xSetVector(dPlayerData, xPlayerPos, pos);
}

void shootLaserGround(int p = 0, vector pos = vector(0,0,0), vector dir = vector(0,0,0), float length = 40.0, int delay = 1500) {
	int next = trGetNextUnitScenarioNameNumber();
	trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
	// change spawner into transport ship
	// change into laser
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
	trSetSelectedScale(3.0,0.0, length * 1.222222);
	trSetUnitOrientation(vector(0,0,0) - dir, vector(0,1,0), true);
	// add to database
	xAddDatabaseBlock(dLasers, true);
	xSetInt(dLasers, xOwner, p);
	xSetInt(dLasers, xUnitName, next);
	xSetInt(dLasers, xUnitID, kbGetBlockID(""+next, true));
	xSetVector(dLasers, xLaserPos, kbGetBlockPosition(""+next, true));
	xSetVector(dLasers, xLaserDir, dir);
	xSetInt(dLasers, xLaserTimeout, trTimeMS() + delay);
	xSetFloat(dLasers, xLaserLength, length);
}

void shootLaser(int p = 0, int spawner = 0, vector dir = vector(0,0,0), float length = 40.0, int delay = 1500) {
	int next = trGetNextUnitScenarioNameNumber();
	trArmyDispatch("0,0","Dwarf",1,1,0,1,0,true);
	// change spawner into transport ship
	trUnitSelectClear();
	trUnitSelect(""+spawner);
	trUnitChangeProtoUnit("Transport Ship Greek");
	// immediate garrison and ungarrison
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trImmediateUnitGarrison(""+spawner);
	trUnitChangeProtoUnit("Dwarf");
	// change spawner back to cinematic block
	trUnitSelectClear();
	trUnitSelect(""+spawner);
	trUnitChangeProtoUnit("Cinematic Block");
	// change into laser
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
	trSetSelectedScale(3.0,0.0, length * 1.222222);
	trSetUnitOrientation(vector(0,0,0) - dir, vector(0,1,0), true);
	// add to database
	xAddDatabaseBlock(dLasers, true);
	xSetInt(dLasers, xOwner, p);
	xSetInt(dLasers, xUnitName, next);
	xSetInt(dLasers, xUnitID, kbGetBlockID(""+next, true));
	xSetVector(dLasers, xLaserPos, kbGetBlockPosition(""+next, true));
	xSetVector(dLasers, xLaserDir, dir);
	xSetInt(dLasers, xLaserTimeout, trTimeMS() + delay);
	xSetFloat(dLasers, xLaserLength, length);
}

void shootMissile(int p = 0, vector pos = vector(0,0,0), vector dir = vector(0,0,0), float velocity = 15.0, bool homing = false) {
	int next = trGetNextUnitScenarioNameNumber();
	trArmyDispatch("0,0", "Dwarf", 1, 31, 0, 31, 0, true);
	// change the dwarf into a spy eye
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trUnitChangeProtoUnit("Spy Eye");

	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trMutateSelected(kbGetProtoUnitID("Outpost"));
	trSetSelectedScale(0,0,0);

	xAddDatabaseBlock(dMissiles, true);
	xSetInt(dMissiles, xOwner, p);
	xSetInt(dMissiles, xUnitName, next);
	xSetInt(dMissiles, xUnitID, kbGetBlockID(""+next, true));
	xSetVector(dMissiles, xMissilePos, pos);
	xSetVector(dMissiles, xMissilePrev, pos);
	xSetVector(dMissiles, xMissileDir, dir * velocity);
	xSetBool(dMissiles, xMissileHoming, homing);
}


rule ysearch
inactive
highFrequency
{
	int id = 0;
	int unittype = 0;
	int p = 0;
	vector scale = vector(0,0,0);
	vector dest = vector(0,0,0);
	for(i = spysearch; < trGetNextUnitScenarioNameNumber()) {
		id = kbGetBlockID(""+i, true);
		unittype = kbGetUnitBaseTypeID(id);
		switch(unittype)
		{
			case kbGetProtoUnitID("Spy Eye"):
			{
				if (xGetDatabaseCount(dSpyRequests) > 0) {
					scale = xGetVector(dSpyRequests, xSpyRequestScale);
					dest = xGetVector(dSpyRequests, xSpyRequestDest);
					trUnitSelectClear();
					trUnitSelectByID(id);
					trMutateSelected(xGetInt(dSpyRequests, xSpyRequestProto));
					trSetSelectedScale(xsVectorGetX(scale),xsVectorGetY(scale),xsVectorGetZ(scale));
					trUnitOverrideAnimation(xGetInt(dSpyRequests, xSpyRequestAnim),0,true,false,-1);
					if (aiPlanSetUserVariableInt(1*xsVectorGetX(dest),1*xsVectorGetY(dest),1*xsVectorGetZ(dest),i) == false) {
						debugLog("spy error N/A: " + 1*xsVectorGetX(dest) + "," + 1*xsVectorGetY(dest) + "," + 1*xsVectorGetZ(dest));
					}
					xFreeDatabaseBlock(dSpyRequests);
					spyreset = 0;
				} else {
					debugLog("Spy Buffer is empty");
				}
				break;
			}
			case kbGetProtoUnitID("House"):
			{
				p = kbUnitGetOwner(id);
				xSetVector(dPlayerData, xPlayerCastPos, kbGetBlockPosition(""+i, true), p);
				xSetInt(dPlayerData, xPlayerButton, EVENT_BUILD_HOUSE, p);
				trUnitSelectClear();
				trUnitSelectByID(id);
				trUnitDestroy();
				break;
			}
			case kbGetProtoUnitID("Storehouse"):
			{
				p = kbUnitGetOwner(id);
				xSetVector(dPlayerData, xPlayerCastPos, kbGetBlockPosition(""+i, true), p);
				xSetInt(dPlayerData, xPlayerButton, EVENT_BUILD_STOREHOUSE, p);
				trUnitSelectClear();
				trUnitSelectByID(id);
				trUnitDestroy();
				break;
			}
			case kbGetProtoUnitID("Granary"):
			{
				p = kbUnitGetOwner(id);
				xSetVector(dPlayerData, xPlayerCastPos, kbGetBlockPosition(""+i, true), p);
				xSetInt(dPlayerData, xPlayerButton, EVENT_BUILD_GRANARY, p);
				trUnitSelectClear();
				trUnitSelectByID(id);
				trUnitDestroy();
				break;
			}
		}
	}
	if (xGetDatabaseCount(dSpyRequests) > 0) {
		spyreset = spyreset + 1;
		if (spyreset >= 10) {
			debugLog("Spy reset. Failed requests: " + xGetDatabaseCount(dSpyRequests));
			xClearDatabase(dSpyRequests);
			spyreset = 0;
		}
	}
	spysearch = trGetNextUnitScenarioNameNumber();
}
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
		vector hitbox = xsVectorSet(xsVectorGetX(start) + current * xsVectorGetX(dir),0,
			xsVectorGetZ(start) + current * xsVectorGetZ(dir));
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

void spawnPlayer(int p = 0, vector pos = vector(0,0,0)) {
	xSetPointer(dPlayerData, p);
	xSetInt(dPlayerData, xPlayerUnit, trGetNextUnitScenarioNameNumber());
	trArmyDispatch(""+p+",0", "Dwarf", 1, xsVectorGetX(pos), 0, xsVectorGetZ(pos), 0, true);
	xSetInt(dPlayerData, xPlayerUnitID, kbGetBlockID(""+xGetInt(dPlayerData, xPlayerUnit), true));
	
	xUnitSelectByID(dPlayerData, xPlayerUnitID);
	spyEffect(kbGetProtoUnitID("Cinematic Block"), -1, xsVectorSet(dPlayerData, xPlayerSpawner, p));
	vector dir = getUnitVector(pos, xsVectorSet(mapSize, 0, mapSize), 1.0);
	trSetUnitOrientation(dir, vector(0,1,0), true);
	trUnitChangeProtoUnit(xGetString(dPlayerData, xPlayerProto));
	
	xSetVector(dPlayerData, xPlayerPos, pos);
}

void shootLaser(int p = 0, int spawner = 0, vector dir = vector(0,0,0), int delay = 1500) {
	trSoundPlayFN("skypassagein.wav");
}

void shootMissile(int p = 0, vector pos = vector(0,0,0), vector dir = vector(0,0,0), bool homing = false) {

}


rule ysearch
inactive
highFrequency
{
	int id = 0;
	int unittype = 0;
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
				xSetVector(dPlayerData, xPlayerCastPos, kbGetBlockPosition(""+i, true), kbUnitGetOwner(id));
				trUnitSelectClear();
				trUnitSelectByID(id);
				trUnitDestroy();
				xSetInt(dPlayerData, xPlayerButton, EVENT_BUILD_HOUSE);
				break;
			}
			case kbGetProtoUnitID("Storehouse"):
			{
				xSetVector(dPlayerData, xPlayerCastPos, kbGetBlockPosition(""+i, true), kbUnitGetOwner(id));
				trUnitSelectClear();
				trUnitSelectByID(id);
				trUnitDestroy();
				xSetInt(dPlayerData, xPlayerButton, EVENT_BUILD_STOREHOUSE);
				break;
			}
			case kbGetProtoUnitID("Granary"):
			{
				xSetVector(dPlayerData, xPlayerCastPos, kbGetBlockPosition(""+i, true), kbUnitGetOwner(id));
				trUnitSelectClear();
				trUnitSelectByID(id);
				trUnitDestroy();
				xSetInt(dPlayerData, xPlayerButton, EVENT_BUILD_GRANARY);
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
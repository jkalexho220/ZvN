
rule initialize_upgrades
active
highFrequency
{
	xsDisableSelf();
	int db = 0;
	for(p=1; <= 2) {
		db = zNewArray(mInt, UPGRADES_COUNT, "p"+p+"upgrades");
		trQuestVarSet("p"+p+"upgrades", db);
		for(i=0; < UPGRADES_COUNT) {
			zSetInt(db, i, i);
		}
	}

	trEventSetHandler(EVENT_CHOOSE_FIRST_UPGRADE, "chooseUpgrade");
	trEventSetHandler(EVENT_CHOOSE_SECOND_UPGRADE, "chooseUpgrade");
}

string upgradeName(int upgrade = -1) {
	string name = "N/A";
	switch(upgrade)
	{
	case UPGRADE_SPEED:
		{
			name = "+1 Speed";
			break;
		}
	case UPGRADE_ATTACK:
		{
			name = "+1 Attack";
			break;
		}
	case UPGRADE_BULLETS:
		{
			name = "+0.3x faster bullets/lasers";
			break;
		}
	case UPGRADE_HEALTH:
		{
			name = "+10 Health";
			break;
		}
	case UPGRADE_BASIC_ATTACK:
		{
			name = "Basic attack fires more bullets/lasers";
			break;
		}
	}
	return(name);
}

void getUpgrade(int p = 0, int upgrade = -1) {
	trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> has chosen " + upgradeName(upgrade));
	switch(upgrade)
	{
	case UPGRADE_SPEED:
		{
			trModifyProtounit(xGetString(dPlayerData, xPlayerProto, p), p, 1, 1);
			break;
		}
	case UPGRADE_ATTACK:
		{
			xSetFloat(dPlayerData, xPlayerAttack, xGetFloat(dPlayerData, xPlayerAttack) + 1);
			trModifyProtounit(xGetString(dPlayerData, xPlayerProto, p), p, 27, 1);
			trModifyProtounit(xGetString(dPlayerData, xPlayerProto, p), p, 31, 1);
			break;
		}
	case UPGRADE_BULLETS:
		{
			xSetFloat(dPlayerData, xPlayerBulletSpeed, xGetFloat(dPlayerData, xPlayerBulletSpeed) + 0.3);
			break;
		}
	case UPGRADE_HEALTH:
		{
			trModifyProtounit(xGetString(dPlayerData, xPlayerProto, p), p, 0, 10);
			break;
		}
	case UPGRADE_BASIC_ATTACK:
		{
			xSetInt(dPlayerData, xPlayerAttackLevel, xGetInt(dPlayerData, xPlayerAttackLevel) + 1);
			break;
		}
	}
}

void chooseUpgrade(int upgrade = -1) {
	trackInsert();
	trackAddWaypoint();
	trackPlay(1,EVENT_BUILD_AT_CURSOR);
	trBlockAllSounds(false);
	uiFindType("Dwarf");

	switch(upgrade)
	{
		case EVENT_CHOOSE_FIRST_UPGRADE:
		{
			uiSetProtoCursor("House", true);
		}
		case EVENT_CHOOSE_SECOND_UPGRADE:
		{
			uiSetProtoCursor("Granary", true);
		}
	}
}

void clearUnitDatabase(int db = 0) {
	for(i=xGetDatabaseCount(db); >0) {
		xDatabaseNext(db);
		xUnitSelectByID(db, xUnitID);
		trUnitDestroy();
	}
	xClearDatabase(db);
}

int getRandomUpgrade(int p = 0, int maximum = 4) {
	int db = trQuestVarGet("p"+p+"upgrades");
	trQuestVarSetFromRand("rand", 0, maximum, true);
	int val = zGetInt(db, 1*trQuestVarGet("rand"));
	// swap our choice with the last item in the array
	zSetInt(db, 1*trQuestVarGet("rand"), zGetInt(db, UPGRADES_COUNT - 1)); 
	zSetInt(db, UPGRADES_COUNT - 1, val);
	return(val);
}

void death(int p = 0) {
	clearUnitDatabase(dMissiles);
	clearUnitDatabase(dLasers);
	clearUnitDatabase(dTurrets);

	for(i=xGetDatabaseCount(dCarousels); >0) {
		xDatabaseNext(dCarousels);
		for(j=xGetInt(dCarousels, xCarouselStart); < xGetInt(dCarousels, xCarouselEnd)) {
			trUnitSelectClear();
			trUnitSelect(""+j, true);
			trUnitDestroy();
		}
	}
	xClearDatabase(dCarousels);

	xUnitSelect(dPlayerData, xPlayerSphinx);
	trUnitDestroy();

	trQuestVarSet("p"+p+"nickMissiles", 0);
	trQuestVarSet("p"+p+"barrage", 0);

	xUnitSelectByID(dPlayerData, xPlayerUnitID);
	trUnitChangeProtoUnit(xGetString(dPlayerData, xPlayerProto));

	xSetBool(dPlayerData, xPlayerAlive, false, p);
	xSetBool(dPlayerData, xPlayerCanCast, false, p);
	xSetInt(dPlayerData, xPlayerButton, BUTTON_NONE);

	xSetInt(dPlayerData, xPlayerLives, xGetInt(dPlayerData, xPlayerLives) - 1);

	trSoundPlayFN("townbell.wav");
	trQuestVarSetFromRand("sound", 1, 5, true);
	trSoundPlayFN("lightningstrike"+1*trQuestVarGet("sound")+".wav");
	trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> has died!");
	trChatSend(0, "" + xGetInt(dPlayerData, xPlayerLives) + " lives remaining!");

	if (xGetInt(dPlayerData, xPlayerLives) > 0) {
		// choose two upgrades at random
		int first = getRandomUpgrade(p, UPGRADES_COUNT - 1);
		int second = getRandomUpgrade(p, UPGRADES_COUNT - 2);
		trQuestVarSet("p"+p+"upgrade" + EVENT_BUILD_HOUSE, first);
		trQuestVarSet("p"+p+"upgrade" + EVENT_BUILD_GRANARY, second);
		if (trCurrentPlayer() == p) {
			trShowChoiceDialog("Choose an upgrade.", upgradeName(first), EVENT_CHOOSE_FIRST_UPGRADE, upgradeName(second), EVENT_CHOOSE_SECOND_UPGRADE);
		}
		trQuestVarSet("p"+p+"dwarf", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
		trArmySelect(""+p+",0");
		trSetSelectedScale(0,0,0);

		trQuestVarSet("p"+p+"respawnTime", trTime() + 10);
		if (trCurrentPlayer() == p) {
			trCounterAddTime("respawn", 10, 1, "Respawning...");
		}
	}
	trSetCounterDisplay("Zeno: " + xGetInt(dPlayerData, xPlayerLives, 1) + " | Nick: " + xGetInt(dPlayerData, xPlayerLives, 2));
}
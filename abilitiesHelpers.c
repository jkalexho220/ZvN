
int randomAbility(int p = 0) {
	//trQuestVarSetFromRand("temp", ZENO_ABILITIES + 1, NICK_ABILITIES + 1, true);
	trQuestVarSetFromRand("temp", 1, 7, true);
	trQuestVarSetFromRand("temp2", 1, 15, true);
	if (trQuestVarGet("temp2") < trQuestVarGet("temp")) {
		trQuestVarSet("temp", trQuestVarGet("temp2"));
	}
	if (trQuestVarGet("temp") == NICK_ABILITIES) {
		if (trCurrentPlayer() == p) {
			trSoundPlayFN("ui\thunder5.wav");
		}
	}
	return(xGetInt(dPlayerData, xPlayerAbilitiesStart, p) + trQuestVarGet("temp"));
}

string abilityName(int ability = 0) {
	string name = "N/A";
	switch(ability)
	{
	case ABILITY_ON_COOLDOWN:
		{
			name = "On Cooldown";
			break;
		}
	case ZENO_GRID:
		{
			name = "Laser Grid";
			break;
		}
	case ZENO_TURRET:
		{
			name = "Laser Turret";
			break;
		}
	case ZENO_SPEED:
		{
			name = "Light Speed";
			break;
		}
	case ZENO_SHIELD:
		{
			name = "Deflector Shield";
			break;
		}
	case ZENO_BARRAGE:
		{
			name = "Laser Barrage";
			break;
		}
	case ZENO_CAROUSEL:
		{
			name = "Laser Carousel";
			break;
		}
	case ZENO_WORLD_SPLITTER:
		{
			name = "World Splitter";
			break;
		}
	case ZENO_DANCE:
		{
			name = "Zenofinale (Ultimate)";
			break;
		}
	case NICK_VOLLEY:
		{
			name = "Scattershot";
			break;
		}
	case NICK_TURRET:
		{
			name = "Bullet Turret";
			break;
		}
	case NICK_TELEPORT:
		{
			name = "Teleport";
			break;
		}
	case NICK_SPLIT:
		{
			name = "Duplicate Bullets";
			break;
		}
	case NICK_MIRROR:
		{
			name = "Mirror Bubble";
			break;
		}
	case NICK_MISSILES:
		{
			name = "Homing Missiles";
			break;
		}
	case NICK_ON_HAWK:
		{
			name = "Nick on Hawk";
			break;
		}
	case NICK_SINGULARITY:
		{
			name = "Singularity (Ultimate)";
			break;
		}
	}
	return(name);
}

string hotkeyName(int hotkey = 0) {
	string name = "N/A";
	switch(hotkey)
	{
	case BUTTON_Q:
		{
			name = "(Q)";
			break;
		}
	case BUTTON_W:
		{
			name = "(W)";
			break;
		}
	case BUTTON_E:
		{
			name = "(E)";
			break;
		}
	}
	return(name);
}

/*
If an ability is empty, get a random ability.
Display abilities
*/
void updateAbilities(int p = 0, bool updateDisplay = false) {
	int db = xGetInt(dPlayerData, xPlayerAbilities, p);
	for(i = 1; <= 3) {
		xSetPointer(db, i);
		if (xGetInt(db, xAbilityType) == ABILITY_ON_COOLDOWN) {
			if (trTimeMS() > xGetInt(db, xAbilityCooldown)) {
				xSetInt(db, xAbilityType, randomAbility(p));
				updateDisplay = true;
			}
		}
	}

	if (updateDisplay && (trCurrentPlayer() == p)) {
		for(i = 3; >0) {
			trCounterAbort("countdown"+i);
		}
		for(i = 1; <= 3) {
			xSetPointer(db, i);
			int duration = (xGetInt(db, xAbilityCooldown) - trTimeMS()) / 1000;
			trCounterAddTime("countdown"+i, duration, -9999, hotkeyName(i) + " " + abilityName(xGetInt(db, xAbilityType)), -1);
		}
	}
}
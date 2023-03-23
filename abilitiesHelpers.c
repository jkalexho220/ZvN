
int randomAbility(int p = 0) {
	trQuestVarSetFromRand("temp", 1, 7, true);
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
	case ZENO_BOUNCING:
		{
			name = "Bouncing Laser";
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
	case ZENO_MIRROR:
		{
			name = "Mirror Circle";
			break;
		}
	case ZENO_SWORD:
		{
			name = "Laser Sword";
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
	case NICK_CONVERGE:
		{
			name = "Bullet Convergence";
			break;
		}
	case NICK_STORM:
		{
			name = "Bullet Storm";
			break;
		}
	case NICK_MISSILES:
		{
			name = "Missile Barrage";
			break;
		}
	case NICK_HELL:
		{
			name = "Portal to Space";
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
		trCounterAbort("countdown");
		for(i = 1; <= 3) {
			xSetPointer(db, i);
			int duration = (trTimeMS() - xGetInt(db, xAbilityCooldown)) / 1000;
			trCounterAddTime("countdown", duration, -9999, hotkeyName(i) + " " + abilityName(xGetInt(db, xAbilityType)), -1);
		}
	}
}
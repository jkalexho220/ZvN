int dPlayerData = 0;
int xPlayerUnit = 0;
int xPlayerUnitID = 0;
int xPlayerCastPos = 0;
int xPlayerProto = 0;
int xPlayerCanCast = 0;
int xPlayerSpawner = 0;
int xPlayerPos = 0;
int xPlayerButton = 0;
int xPlayerAbilities = 0;
int xPlayerAbilitiesStart = 0;

int xAbilityCooldown = 0;
int xAbilityType = 0;

int dSpyRequests = 0;
int xSpyRequestProto = 0;
int xSpyRequestAnim = 0;
int xSpyRequestDest = 0;
int xSpyRequestScale = 0;

int dMissiles = 0;
int xMissilePrev = 0;
int xMissileDir = 0;
int xMissilePos = 0;
int xMissileHoming = 0;

rule initialize_databases
active
highFrequency
{
	xsDisableSelf();

	dPlayerData = xInitDatabase("Player Data", 2);
	xPlayerUnit = xInitAddInt(dPlayerData, "unit");
	xPlayerUnitID = xInitAddInt(dPlayerData, "unitID");
	xPlayerProto = xInitAddString(dPlayerData, "proto");
	xPlayerCastPos = xInitAddVector(dPlayerData, "Cast Pos");
	xPlayerCanCast = xInitAddBool(dPlayerData, "can cast", true);
	xPlayerSpawner = xInitAddInt(dPlayerData, "spawner");
	xPlayerPos = xInitAddVector(dPlayerData, "position");
	xPlayerButton = xInitAddInt(dPlayerData, "button", BUTTON_NONE);
	xPlayerAbilities = xInitAddInt(dPlayerData, "abilities");
	xPlayerAbilitiesStart = xInitAddInt(dPlayerData, "abilitiesStart");

	dSpyRequests = xInitDatabase("Spy Requests");
	xSpyRequestProto = xInitAddInt(dSpyRequests, "proto");
	xSpyRequestAnim = xInitAddInt(dSpyRequests, "anim");
	xSpyRequestDest = xInitAddVector(dSpyRequests, "dest");
	xSpyRequestScale = xInitAddVector(dSpyRequests, "scale");

	xAddDatabaseBlock(dPlayerData);
	xAddDatabaseBlock(dPlayerData);
	xSetString(dPlayerData, xPlayerProto, "Hoplite", 1);
	xSetString(dPlayerData, xPlayerProto, "Hero Greek Odysseus", 2);
	xSetInt(dPlayerData, xPlayerAbilitiesStart, ZENO_ABILITIES, 1);
	xSetInt(dPlayerData, xPlayerAbilitiesStart, NICK_ABILITIES, 2);

	int db = 0;
	for(p=1; <= 2) {
		db = xInitDatabase("p"+p+"abilities", 3);
		xAbilityType = xInitAddInt(db, "type", ABILITY_ON_COOLDOWN);
		xAbilityCooldown = xInitAddInt(db, "cooldown", 0);
		for(i=1; <= 3) {
			xAddDatabaseBlock(db);
		}

		xSetPointer(dPlayerData, p);
		xSetInt(dPlayerData, xPlayerAbilities, db);
	}
}
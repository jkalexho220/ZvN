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
int xPlayerStartDelay = 0;
int xPlayerEndDelay = 0;
int xPlayerAttackAnimation = 0;
int xPlayerAttackStep = 0;
int xPlayerAttackNext = 0;
int xPlayerAttackDir = 0;

int xAbilityCooldown = 0;
int xAbilityType = 0;

int dSpyRequests = 0;
int xSpyRequestProto = 0;
int xSpyRequestAnim = 0;
int xSpyRequestDest = 0;
int xSpyRequestScale = 0;

int xOwner = 0;
int xUnitName = 0;
int xUnitID = 0;

int dMissiles = 0;
int xMissilePrev = 0;
int xMissileDir = 0;
int xMissilePos = 0;
int xMissileHoming = 0;

int dLasers = 0;
int xLaserPos = 0;
int xLaserDir = 0;
int xLaserStep = 0;
int xLaserTimeout = 0;
int xLaserLength = 0;
int xLaserDamage = 0;

int dTurrets = 0;
int xTurretCooldown = 0;
int xTurretType = 0;
int xTurretPos = 0;

int dDeflectorShields = 0;
int xDeflectorShieldLeft = 0;
int xDeflectorShieldRight = 0;
int xDeflectorShieldStep = 0;
int xDeflectorShieldTimeout = 0;
int xDeflectorShieldPos = 0;
int xDeflectorShieldDir = 0;

void setupZeno(int p = 0) {
	xSetPointer(dPlayerData, p);
	xSetString(dPlayerData, xPlayerProto, "Hoplite", p);
	xSetInt(dPlayerData, xPlayerAbilitiesStart, ZENO_ABILITIES, p);
	xSetInt(dPlayerData, xPlayerAttackAnimation, 1);
	xSetInt(dPlayerData, xPlayerStartDelay, 563);
	xSetInt(dPlayerData, xPlayerEndDelay, 1150 - xGetInt(dPlayerData, xPlayerStartDelay));
}

void setupNick(int p = 0) {
	xSetPointer(dPlayerData, p);
	xSetString(dPlayerData, xPlayerProto, "Hero Greek Odysseus", p);
	xSetInt(dPlayerData, xPlayerAbilitiesStart, NICK_ABILITIES, p);
	xSetInt(dPlayerData, xPlayerAttackAnimation, 12);
	xSetInt(dPlayerData, xPlayerStartDelay, 480);
	xSetInt(dPlayerData, xPlayerEndDelay, 1200 - xGetInt(dPlayerData, xPlayerStartDelay));
}

void OwnerNameID(int db = 0) {
	xOwner = xInitAddInt(db, "owner");
	xUnitName = xInitAddInt(db, "name");
	xUnitID = xInitAddInt(db, "id");
}

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

	// abilities
	xPlayerButton = xInitAddInt(dPlayerData, "button", BUTTON_NONE);
	xPlayerAbilities = xInitAddInt(dPlayerData, "abilities");
	xPlayerAbilitiesStart = xInitAddInt(dPlayerData, "abilitiesStart");

	// attacking
	xPlayerAttackAnimation = xInitAddInt(dPlayerData, "attackAnimation");
	xPlayerStartDelay = xInitAddInt(dPlayerData, "attackStartDelay");
	xPlayerEndDelay = xInitAddInt(dPlayerData, "attackEndDelay");
	xPlayerAttackStep = xInitAddInt(dPlayerData, "attackStep");
	xPlayerAttackNext = xInitAddInt(dPlayerData, "attackNext");
	xPlayerAttackDir = xInitAddVector(dPlayerData, "attackDir");

	// spy eyes
	dSpyRequests = xInitDatabase("Spy Requests");
	xSpyRequestProto = xInitAddInt(dSpyRequests, "proto");
	xSpyRequestAnim = xInitAddInt(dSpyRequests, "anim");
	xSpyRequestDest = xInitAddVector(dSpyRequests, "dest");
	xSpyRequestScale = xInitAddVector(dSpyRequests, "scale");

	// lasers
	dLasers = xInitDatabase("Lasers");
	OwnerNameID(dLasers);
	xLaserPos = xInitAddVector(dLasers, "pos");
	xLaserDir = xInitAddVector(dLasers, "dir");
	xLaserStep = xInitAddInt(dLasers, "step");
	xLaserTimeout = xInitAddInt(dLasers, "timeout");
	xLaserLength = xInitAddFloat(dLasers, "length", 30.0);
	xLaserDamage = xInitAddInt(dLasers, "damage", 2);

	// missiles
	dMissiles = xInitDatabase("missiles");
	OwnerNameID(dMissiles);
	xMissilePos = xInitAddVector(dMissiles, "pos");
	xMissileDir = xInitAddVector(dMissiles, "dir");
	xMissilePrev = xInitAddVector(dMissiles, "prev");
	xMissileHoming = xInitAddBool(dMissiles, "homing");

	// turrets
	dTurrets = xInitDatabase("turrets");
	OwnerNameID(dTurrets);
	xTurretType = xInitAddInt(dTurrets, "type", ZENO_ABILITIES);
	xTurretCooldown = xInitAddInt(dTurrets, "cooldown");
	xTurretPos = xInitAddVector(dTurrets, "pos");

	dDeflectorShields = xInitDatabase("Deflector Shields");
	xDeflectorShieldLeft = xInitAddInt(dDeflectorShields, "leftUnit");
	xDeflectorShieldRight = xInitAddInt(dDeflectorShields, "rightUnit");
	xDeflectorShieldStep = xInitAddInt(dDeflectorShields, "step", 0);
	xDeflectorShieldTimeout = xInitAddInt(dDeflectorShields, "timeout", 500);
	xDeflectorShieldPos = xInitAddVector(dDeflectorShields, "pos");
	xDeflectorShieldDir = xInitAddVector(dDeflectorShields, "dir");

	int db = 0;
	for(p=1; <= 2) {
		xAddDatabaseBlock(dPlayerData, true);
		db = xInitDatabase("p"+p+"abilities", 3);
		xAbilityType = xInitAddInt(db, "type", ABILITY_ON_COOLDOWN);
		xAbilityCooldown = xInitAddInt(db, "cooldown", 0);
		for(i=1; <= 3) {
			xAddDatabaseBlock(db);
		}

		xSetInt(dPlayerData, xPlayerAbilities, db);
	}

	setupZeno(1);
	setupNick(2);
}
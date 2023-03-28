int dPlayerData = 0;
int xPlayerUnit = 0;
int xPlayerUnitID = 0;
int xPlayerCastPos = 0;
int xPlayerProto = 0;
int xPlayerCharName = 0;
int xPlayerCanCast = 0;
int xPlayerAlive = 0;
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

int xPlayerSphinx = 0;
int xPlayerWorldSplitterLava = 0;

int xPlayerLives = 0;
int xPlayerAttack = 0;
int xPlayerAttackLevel = 0;
int xPlayerBulletSpeed = 0;

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

int dCarousels = 0;
int xCarouselStart = 0;
int xCarouselEnd = 0;
int xCarouselPos = 0;
int xCarouselDir = 0;
int xCarouselTimeout = 0;
int xCarouselStep = 0;

int dHawkBarrages = 0;
int xHawkBarragePos = 0;
int xHawkBarrageTimeout = 0;

int dHawkBombs = 0;
int xHawkBombPos = 0;
int xHawkBombStep = 0;
int xHawkBombTimeout = 0;

int dBubbles = 0;
int xBubbleStart = 0;
int xBubbleEnd = 0;
int xBubbleCenter = 0;
int xBubbleTimeout = 0;

int dCinematicSteps = 0;
int xCinematicSpeaker = 0;
int xCinematicAnim = 0;
int xCinematicText = 0;
int xCinematicDuration = 0;

void setupZeno(int p = 0) {
	xSetPointer(dPlayerData, p);
	xSetString(dPlayerData, xPlayerProto, "Hoplite", p);
	xSetString(dPlayerData, xPlayerCharName, "Zenophobia");
	xSetInt(dPlayerData, xPlayerAbilitiesStart, ZENO_ABILITIES, p);
	xSetInt(dPlayerData, xPlayerAttackAnimation, 1);
	xSetInt(dPlayerData, xPlayerStartDelay, 563);
	xSetInt(dPlayerData, xPlayerEndDelay, 1150 - xGetInt(dPlayerData, xPlayerStartDelay));
}

void setupNick(int p = 0) {
	xSetPointer(dPlayerData, p);
	xSetString(dPlayerData, xPlayerProto, "Hero Greek Odysseus", p);
	xSetString(dPlayerData, xPlayerCharName, "Nickonhawk");
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
	xPlayerAlive = xInitAddBool(dPlayerData, "alive", false);
	xPlayerCanCast = xInitAddBool(dPlayerData, "can cast", true);
	xPlayerPos = xInitAddVector(dPlayerData, "position");
	xPlayerCharName = xInitAddString(dPlayerData, "name");

	xPlayerSphinx = xInitAddInt(dPlayerData, "sphinxSFX");
	xPlayerSpawner = xInitAddInt(dPlayerData, "spawner");
	xPlayerWorldSplitterLava = xInitAddInt(dPlayerData, "lava");

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

	// upgraded stats
	xPlayerLives = xInitAddInt(dPlayerData, "lives", 5);
	xPlayerAttack = xInitAddFloat(dPlayerData, "attack", 2);
	xPlayerAttackLevel = xInitAddInt(dPlayerData, "attackLevel", 0);
	xPlayerBulletSpeed = xInitAddFloat(dPlayerData, "bulletSpeed", 1.0);

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
	xInitAddInt(dDeflectorShields, "owner");
	xDeflectorShieldLeft = xInitAddInt(dDeflectorShields, "leftUnit");
	xDeflectorShieldRight = xInitAddInt(dDeflectorShields, "rightUnit");
	xDeflectorShieldStep = xInitAddInt(dDeflectorShields, "step", 0);
	xDeflectorShieldTimeout = xInitAddInt(dDeflectorShields, "timeout", 500);
	xDeflectorShieldPos = xInitAddVector(dDeflectorShields, "pos");
	xDeflectorShieldDir = xInitAddVector(dDeflectorShields, "dir");

	dCarousels = xInitDatabase("Carousels");
	xInitAddInt(dCarousels, "owner");
	xCarouselStart = xInitAddInt(dCarousels, "start");
	xCarouselEnd = xInitAddInt(dCarousels, "end");
	xCarouselPos = xInitAddVector(dCarousels, "pos");
	xCarouselDir = xInitAddVector(dCarousels, "dir", vector(1,0,0));
	xCarouselTimeout = xInitAddInt(dCarousels, "timeout");
	xCarouselStep = xInitAddInt(dCarousels, "step", 0);

	dBubbles = xInitDatabase("bubble shields");
	xInitAddInt(dBubbles, "owner");
	xBubbleStart = xInitAddInt(dBubbles, "start");
	xBubbleEnd = xInitAddInt(dBubbles, "end");
	xBubbleCenter = xInitAddVector(dBubbles, "center");
	xBubbleTimeout = xInitAddInt(dBubbles, "timeout");

	dHawkBarrages = xInitDatabase("hawk barrages");
	xInitAddInt(dHawkBarrages, "owner");
	xHawkBarrageTimeout = xInitAddInt(dHawkBarrages, "timeout");
	xHawkBarragePos = xInitAddVector(dHawkBarrages, "pos");

	dHawkBombs = xInitDatabase("hawk bombs");
	OwnerNameID(dHawkBombs);
	xHawkBombPos = xInitAddVector(dHawkBombs, "pos");
	xHawkBombStep = xInitAddInt(dHawkBombs, "step", 0);
	xHawkBombTimeout = xInitAddInt(dHawkBombs, "timeout");

	dCinematicSteps = xInitDatabase("Cinematic Steps");
	xCinematicSpeaker = xInitAddInt(dCinematicSteps, "speaker");
	xCinematicAnim = xInitAddString(dCinematicSteps, "anim");
	xCinematicText = xInitAddString(dCinematicSteps, "text");
	xCinematicDuration = xInitAddInt(dCinematicSteps, "duration");

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
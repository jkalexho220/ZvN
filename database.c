int dPlayerData = 0;
int xPlayerUnit = 0;
int xPlayerUnitID = 0;
int xPlayerCastPos = 0;
int xPlayerProto = 0;
int xPlayerCanCast = 0;
int xPlayerSpawner = 0;

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
	xPlayerCanCast = xInitAddBool(dPlayerData, "can cast");
	xPlayerSpawner = xInitAddInt(dPlayerData, "spawner");

	dSpyRequests = xInitDatabase("Spy Requests");
	xSpyRequestProto = xInitAddInt(dSpyRequests, "proto");
	xSpyRequestAnim = xInitAddInt(dSpyRequests, "anim");
	xSpyRequestDest = xInitAddVector(dSpyRequests, "dest");
	xSpyRequestScale = xInitAddVector(dSpyRequests, "scale");
}
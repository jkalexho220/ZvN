int mapSize = 32;

void modifyPlayableProto(string proto = "", int p = 0) {
	// HP
	trModifyProtounit(proto, p, 0, 9999999999999999999.0);
	trModifyProtounit(proto, p, 0, -9999999999999999999.0);
	trModifyProtounit(proto, p, 0, 30.0);
	// speed
	trModifyProtounit(proto, p, 1, 9999999999999999999.0);
	trModifyProtounit(proto, p, 1, -9999999999999999999.0);
	trModifyProtounit(proto, p, 1, 5.0);
	// LOS
	trModifyProtounit(proto, p, 2, 9999999999999999999.0);
	trModifyProtounit(proto, p, 2, -9999999999999999999.0);
	trModifyProtounit(proto, p, 2, 3.0);
	// armor
	trModifyProtounit(proto, p, 24, -1);
	trModifyProtounit(proto, p, 25, -1);
	trModifyProtounit(proto, p, 26, -1);

}

void modifyBuildableProto(string proto = "", int p = 0) {
	trModifyProtounit(proto, p, 55, 4); // flying
	// LOS
	trModifyProtounit(proto, p, 2, 9999999999999999999.0);
	trModifyProtounit(proto, p, 2, -9999999999999999999.0);
	trModifyProtounit(proto, p, 2, 0.0);
	// cost gold/wood/food/favor
	for(i=16; <= 19) {
		trModifyProtounit(proto, p, i, 9999999999999999999.0);
		trModifyProtounit(proto, p, i, -9999999999999999999.0);
		trModifyProtounit(proto, p, i, 0.0);
	}
	// build points
	trModifyProtounit(proto, p, 4, -999);
}

rule setup
active
highFrequency
runImmediately
{
	xsDisableSelf();
	trSetFogAndBlackmap(false, false);
	trUIFadeToColor(0,0,0,0,0,true);
	if (aiIsMultiplayer() == false) {
		uiMessageBox("This map can only be played in multiplayer.","leaveGame()");
	}

	gadgetUnreal("ScoreDisplay");
	gadgetUnreal("GodPowers");
	gadgetUnreal("tributedlg-sendButton");
	gadgetUnreal("tributedlg-clearButton");
	configUndef("ErodeBuildingFoundations");

	trPlayerSetDiplomacy(1, 2, "Enemy");
	trPlayerSetDiplomacy(2, 1, "Enemy");

	for(p=1; <= 2) {
		trPlayerGrantResources(p, "Food", -999);
		trPlayerGrantResources(p, "Wood", -999);
		trPlayerGrantResources(p, "Gold", -999);
		trPlayerGrantResources(p, "Favor", -999);
		trPlayerSetDiplomacy(p, 0, "neutral");
		trPlayerSetDiplomacy(0, p, "Enemy");
		trPlayerKillAllGodPowers(p);
		trPlayerTechTreeEnabledGodPowers(p, false);
		trSetCivAndCulture(p, 1, 0);

		// heavy hoplite and iron all
		trTechSetStatus(p, 7, 4);
		trTechSetStatus(p, 476, 4);

		modifyPlayableProto("Hoplite", p);
		modifyPlayableProto("Hero Greek Odysseus", p);
		modifyBuildableProto("House", p);
		modifyBuildableProto("Storehouse", p);
		modifyBuildableProto("Granary", p);
	}
}
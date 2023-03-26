const int CHAR_ZENO = 0;
const int CHAR_NICK = 1;

string cinematicCharName(int character = 0) {
	string name = "N/A";
	switch(character)
	{
	case CHAR_ZENO:
		{
			name = "Zenophobia";
		}
	case CHAR_NICK:
		{
			name = "Nickonhawk";
		}
	}
	return(name);
}

string cinematicCharIcon(int character = 0) {
	string icon = "default";
	switch(character)
	{
	case CHAR_ZENO:
		{
			icon = "icons\infantry g hoplite icon 64";
		}
	case CHAR_NICK:
		{
			icon = "icons\hero g odysseus icon 64";
		}
	}
	return(icon);
}

void addCinematicStep(int speaker = 0, int duration = 0, string text = "", string anim = "") {
	xAddDatabaseBlock(dCinematicSteps, true);
	xSetInt(dCinematicSteps, xCinematicSpeaker, speaker);
	xSetString(dCinematicSteps, xCinematicText, text);
	xSetString(dCinematicSteps, xCinematicAnim, anim);
	xSetInt(dCinematicSteps, xCinematicDuration, duration);
}

rule intro_cinematic_start
inactive
highFrequency
{
	xsDisableSelf();
	trLetterBox(true);
	trQuestVarSet("cinematicNext", trTime() + 2);
	trUIFadeToColor(0,0,0,1000,1000,false);

	trRenderSky(true, "SkyStormy");
	trSoundPlayFN("cinematics\35_in\thunderstorm.mp3");

	trQuestVarSetFromRand("rand", 1, 4, true);

	switch(1*trQuestVarGet("rand"))
	{
	case 1:
		{
			addCinematicStep(CHAR_ZENO, 5, "Nick! Why haven't you finished the last part yet?!", "Hop_x03Theywere");
			addCinematicStep(CHAR_NICK, 5, "You don't understand, Zeno! Ingenuity takes time!", "Ody_aomsp92Ioklos");
			addCinematicStep(CHAR_ZENO, 5, "We don't HAVE time! There are so many people waiting for the finished product!", "Hop_08SirTheTroj");
			addCinematicStep(CHAR_NICK, 5, "Yeah, well in the meantime, they can watch me kick your ass!", "Ody_aomsp242thanks");
		}
	case 2:
		{
			addCinematicStep(CHAR_NICK, 5, "I've had it with your incessant nagging, Zeno!", "Ody_aomsp09GiantHorse");
			addCinematicStep(CHAR_ZENO, 5, "And I've had it with your incessant complaining, Nick!", "Hop_x03Theywere");
			addCinematicStep(CHAR_NICK, 5, "Perfect! Let's end this once and for all, then!", "Ody_aomsp92RepairBoats");
			addCinematicStep(CHAR_ZENO, 5, "For once, we agree on something!", "Hop_08SirTheTroj");
		}
	case 3:
		{
			addCinematicStep(CHAR_ZENO, 5, "If you won't finish the last story, then I'll do it myself!", "Hop_x03Theywere");
			addCinematicStep(CHAR_NICK, 5, "Over my dead body!", "Ody_aomsp92iwontbefarbehind");
			addCinematicStep(CHAR_ZENO, 5, "Prepare to die!", "Hop_x03believe");
			addCinematicStep(CHAR_NICK, 5, "Funny, I was going to say the exact same thing!", "Ody_aomsp09AjaxFunny");
		}
	case 4:
		{
			addCinematicStep(CHAR_NICK, 5, "Enough is enough, Zeno! Our partnership ends here!");
			addCinematicStep(CHAR_ZENO, 5, "Oh, so you've finally shown your true colors, huh Nick?", "Hop_x03Theywere");
			addCinematicStep(CHAR_NICK, 5, "That's right! I can't stand another minute of working with you!", "Ody_aomsp09GiantHorse");
			addCinematicStep(CHAR_ZENO, 5, "You traitorous scum! I'll teach you a lesson you won't forget!", "Hop_x03believe");
		}
	}

	trCameraCut(vector(31,10,5.5), vector(0, 0, 1.0), vector(0, 1.0, 0), vector(1.0, 0, 0));

	xsEnableRule("cinematic_play");
	xsEnableRule("intro_cinematic_end");
}

rule intro_cinematic_end
inactive
highFrequency
{
	if ((xGetDatabaseCount(dCinematicSteps) == 0) && (trTime() > trQuestVarGet("cinematicNext") + 1)) {
		xsDisableSelf();
		xsEnableRule("gameplay_start");
		trOverlayText("Press Q, W, and E to cast spells!", 5.0);
	}
}

rule cinematic_play
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinematicNext")) {
		if (xGetDatabaseCount(dCinematicSteps) == 0) {
			trUIFadeToColor(0,0,0,1000,0,true);
			xsEnableRule("cinematic_end");
			xsDisableSelf();
		} else {
			xDatabaseNext(dCinematicSteps);
			int character = xGetInt(dCinematicSteps, xCinematicSpeaker);
			trSoundPlayFN("default","1",-1,cinematicCharName(character) + ":" + xGetString(dCinematicSteps, xCinematicText), cinematicCharIcon(character));
			trUnitSelectClear();
			trUnitSelectByID(character);
			trUnitSetAnimation(xGetString(dCinematicSteps, xCinematicAnim), false, -1);
			trQuestVarSet("cinematicNext", trQuestVarGet("cinematicNext") + xGetInt(dCinematicSteps, xCinematicDuration));
			xFreeDatabaseBlock(dCinematicSteps);
		}
	}
}

rule cinematic_end
inactive
highFrequency
{
	if (trTime() > cActivationTime + 2) {
		xsDisableSelf();
		trLetterBox(false);
		trUIFadeToColor(0,0,0,1000,0,false);
		trBlockAllSounds(false);

		trUnitSelectClear();
		trUnitSelectByID(CHAR_ZENO);
		trUnitChangeProtoUnit("Cinematic Block");

		trUnitSelectClear();
		trUnitSelectByID(CHAR_NICK);
		trUnitChangeProtoUnit("Cinematic Block");

		uiFindType(xGetString(dPlayerData, xPlayerProto, trCurrentPlayer()));
		trCameraCut(vector(-15.519794,70.710701,-15.519794), vector(0.5,-0.707107,0.5), vector(0.5,0.707107,0.5), vector(0.707107,0,-0.707107));
		trEventFire(EVENT_REMOVE_CAM_TRACKS);
	}
}
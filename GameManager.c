#include <stdio.h>
#include "GameManager.h"

void GameManager_init(GameManager *thisGM) {
	State newState;
	State_init(&newState);
	GameManager_setStage(thisGM, &newState);

	Player newPlayer;
	Player_init(&newPlayer);
	GameManager_setPlayer(thisGM, &newPlayer);

	Rule newRule;
	Rule_init(&newRule);
	GameManager_setRule(thisGM, &newRule);
}

void GameManager_finish(GameManager *thisGM) {

}

void GameManager_run(GameManager *thisGM) {
	printf("X:%d, Y:%d\n", Rule_getMapSizeX(&(thisGM->rule)), Rule_getMapSizeY(&(thisGM->rule)));

	// main loop
	while (TRUE) {
		// gm---state-->player
		// gm<---act----player

		// state update
		
	}
}

void GameManager_setStage(GameManager *thisGM, State *s) {
	thisGM->state = *s;
}

void GameManager_setPlayer(GameManager *thisGM, Player *p) {
	thisGM->player = *p;
}

void GameManager_setRule(GameManager *thisGM, Rule *r) {
	thisGM->rule = *r;
}
#include <stdio.h>
#include "GameManager.h"

void GameManager_init(GameManager *thisGM) {
	printf("rule init...");
	Rule newRule;
	Rule_init(&newRule);
	GameManager_setRule(thisGM, &newRule);
	
	printf("state init...");
	State newState;
	State_init(&newState);
	GameManager_setStage(thisGM, &newState);

	State_finish(&newState);

	printf("player init...");
	Player newPlayer;
	Player_init(&newPlayer);
	GameManager_setPlayer(thisGM, &newPlayer);
}

void GameManager_finish(GameManager *thisGM) {

}

void GameManager_run(GameManager *thisGM) {
	printf("X:%d, Y:%d\n", Rule_getMapSizeX(&(thisGM->rule)), Rule_getMapSizeY(&(thisGM->rule)));

	
	// main loop
	while (TRUE) {
		//int act = Player_getAction(&(thisGM->player), &(thisGM->state));
		//Rule_transition(&(thisGM->rule), &(thisGM->state), act);

		//int act = Player_getAction(&(thisGM->state));
		//nextState = Rule_transition(&(thisGM->state), act);
		//State_update(nextState);
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
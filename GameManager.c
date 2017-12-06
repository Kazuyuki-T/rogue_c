#include <stdio.h>
#include "GameManager.h"

void GameManager_init(GameManager *thisGM) {
	printf("rule init...");
	Rule newRule;
	Rule_init(&newRule);
	GameManager_setRule(thisGM, &newRule);
	printf("complete\n");

	printf("state init...");
	State newState;
	State_init(&newState, &(thisGM->rule));
	GameManager_setStage(thisGM, &newState);
	printf("complete\n");

	// State内の配列の動的確保
	// Rule内の変数値に基づく，マップサイズ，敵数，...
	Rule_initState(&(thisGM->rule), &(thisGM->state));
	//State_output(&(thisGM->state));
	
	printf("player init...");
	Player newPlayer;
	Player_init(&newPlayer);
	GameManager_setPlayer(thisGM, &newPlayer);
	printf("complete\n");

	printf("init complete\n");
}

void GameManager_finish(GameManager *thisGM) {
	printf("player finish...");
	Player_finish(&(thisGM->player));
	printf("complete\n");

	// State内の動的確保した配列の解放
	Rule_finishState(&(thisGM->rule), &(thisGM->state));

	printf("state finish...");
	State_finish(&(thisGM->state));
	printf("complete\n");

	printf("rule finish...");
	Rule_finish(&(thisGM->rule));
	printf("complete\n");

	printf("finish complete\n");
}

void GameManager_run(GameManager *thisGM) {
	printf("Game start\n");
	printf("X:%d, Y:%d\n", Rule_getMapSizeX(&(thisGM->rule)), Rule_getMapSizeY(&(thisGM->rule)));
	
	// main loop
	while (TRUE) {
		// output current state
		
		//int act = Player_getAction(&(thisGM->player), &(thisGM->state));
		//Rule_transition(&(thisGM->rule), &(thisGM->state), act);

		//int act = Player_getAction(&(thisGM->state));
		//State nextState = Rule_transition(&(thisGM->state), act);
		//State_update(nextState);

		break;
	}
	printf("Game end\n");
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
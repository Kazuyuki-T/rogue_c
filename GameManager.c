#include <stdio.h>
#include "GameManager.h"

//#define DEBUG

#define TRUE 1
#define FALSE 0

void GameManager_init(GameManager *thisGM) {
	Rule newRule;
	Rule_init(&newRule);
	GameManager_setRule(thisGM, &newRule);
	
	State newState;
	State_init(&newState);
	GameManager_setStage(thisGM, &newState);


	// State内の配列の動的確保
	// Rule内の変数値に基づく，マップサイズ，敵数，...
	Rule_initArrayState(&(thisGM->rule), &(thisGM->state));
	// Ruleに基づきマップ等の配置
	Rule_setStateInfo(&(thisGM->rule), &(thisGM->state));



	//State_output(&(thisGM->state));
	

	Player newPlayer;
	Player_init(&newPlayer);
	GameManager_setPlayer(thisGM, &newPlayer);

	#ifdef DEBUG
	printf("init complete\n");
	#endif // DEBUG
}

void GameManager_finish(GameManager *thisGM) {
	Player_finish(&(thisGM->player));

	// State内の動的確保した配列の解放
	Rule_finishArrayState(&(thisGM->rule), &(thisGM->state));

	State_finish(&(thisGM->state));

	Rule_finish(&(thisGM->rule));

	#ifdef DEBUG
	printf("finish complete\n");
	#endif // DEBUG
}

void GameManager_run(GameManager *thisGM) {
	#ifdef DEBUG
	printf("Game start\n");
	#endif // DEBUG


	//printf("X:%d, Y:%d\n", Rule_getMapSizeX(&(thisGM->rule)), Rule_getMapSizeY(&(thisGM->rule)));
	printf("player:(%d, %d)\n", thisGM->state.x, thisGM->state.y);

	///////////////
	// main loop //
	///////////////
	while (TRUE) {
		// output current state
		
		int act = Player_getAction(&(thisGM->player), &(thisGM->state));
		Rule_transition(&(thisGM->rule), &(thisGM->state), act);

		printf("player:(%d, %d)\n", thisGM->state.x, thisGM->state.y);

		//int act = Player_getAction(&(thisGM->player), &(thisGM->state));
		//State *nextState = Rule_transition(&(thisGM->state), act);
		//State_update(&(thisGM->state), nextState);

		if(act == 9)	break;
	}


	#ifdef DEBUG
	printf("Game end\n");
	#endif // DEBUG
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
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "GameManager.h"

//#define DEBUG

#define TRUE 1
#define FALSE 0

void GameManager_init(GameManager *thisGM) {
	//srand((unsigned)time(NULL));

	Rule newRule;
	Rule_init(&newRule);
	GameManager_setRule(thisGM, &newRule);
	
	State newState;
	State_init(&newState);
	State_initArray(&newState, thisGM->rule.mapSizeX, thisGM->rule.mapSizeY, thisGM->rule.enemyNumber);
	GameManager_setStage(thisGM, &newState);


	// Ruleに基づきマップ等の配置
	Rule_setStateInfo(&(thisGM->rule), &(thisGM->state));


	//State_output(&(thisGM->state), Rule_getMapSizeX(&(thisGM->rule)), Rule_getMapSizeX(&(thisGM->rule)), Rule_getEnemyNumber(&(thisGM->rule)));


	Player newPlayer;
	Player_init(&newPlayer);
	GameManager_setPlayer(thisGM, &newPlayer);

	#ifdef DEBUG
	printf("init complete\n");
	#endif // DEBUG
}

void GameManager_finish(GameManager *thisGM) {
	Player_finish(&(thisGM->player));

	// 動的確保した配列の解放含む
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
	//printf("player:(%d, %d)\n", thisGM->state.x, thisGM->state.y);

	printf("\n"); // １行目の空欄分
	GameManager_output(thisGM);

	///////////////
	// main loop //
	///////////////
	while (TRUE) {
		printf("\r"); // カーソルを先頭へ
		printf("\x1b[1;1H"); // カーソルの位置を絶対座標1,1へ

		// プレイヤの行動決定
		int act = Player_getAction(&(thisGM->player), &(thisGM->state));
		Rule_transition(&(thisGM->rule), &(thisGM->state), act);
		//State *nextState = Rule_transition(&(thisGM->state), act);
		//State_update(&(thisGM->state), nextState);

		printf("player:(%d, %d)             \n", thisGM->state.x, thisGM->state.y);
		GameManager_output(thisGM); // 盤面の出力

		// ゲーム１試行の終了条件
		if (act == 0x1B) {
			printf("Esc\n");
			break;
		}
	}

	#ifdef DEBUG
	printf("Game end\n");
	#endif // DEBUG
}

void GameManager_output(GameManager *thisGM) {
	int mapx = Rule_getMapSizeX(&(thisGM->rule));
	int mapy = Rule_getMapSizeY(&(thisGM->rule));

	for (int y = 0; y < mapy; y++) {
		for (int x = 0; x < mapx; x++) {
			if (thisGM->state.map[y][x] == 1) {
				printf("# ");
			}
			else if (thisGM->state.map[y][x] == 0 && thisGM->state.y == y && thisGM->state.x == x) {
				printf("@ ");
			}
			else {
				printf(". ");
			}
		}
		printf("\n");
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
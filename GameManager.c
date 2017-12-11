﻿#include <stdio.h>
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
	// 配列の動的確保
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

	printf("\n"); // １行目の空欄分
	GameManager_output(thisGM);

	///////////////
	// main loop //
	///////////////
	while (TRUE) {
		//printf("\r"); // カーソルを先頭へ
		printf("\x1b[1;1H"); // カーソルの位置を絶対座標1,1へ

		// プレイヤの行動決定
		int act = Player_getAction(&(thisGM->player), &(thisGM->state));
		Rule_transition(&(thisGM->rule), &(thisGM->state), act);
		//State *nextState = Rule_transition(&(thisGM->state), act);
		//State_update(&(thisGM->state), nextState);

		printf("                                                 \n");
		GameManager_outputMap(thisGM); // 盤面の出力
		GameManager_outputPlayerState(thisGM); // aaa


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

void GameManager_outputMap(GameManager *thisGM) {
	int mapx = Rule_getMapSizeX(&(thisGM->rule));
	int mapy = Rule_getMapSizeY(&(thisGM->rule));

	for (int y = 0; y < mapy; y++) {
		for (int x = 0; x < mapx; x++) {
			// 描画の優先度に注意
			if (thisGM->state.y == y && thisGM->state.x == x) {
				printf("@ ");
			}
			else if (thisGM->state.map[y][x] == 2) {
				printf("%% ");
			}
			else if (thisGM->state.map[y][x] == 0) {
				printf(". ");
			}
			else if (thisGM->state.map[y][x] == 1) {
				printf("# ");
			}
			else {
				printf("_ ");
			}
		}
		printf("\n");
	}
	printf("\n");
}

void GameManager_outputPlayerState(GameManager *thisGM) {
	printf("player:(%d, %d)    \n", thisGM->state.x, thisGM->state.y);
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
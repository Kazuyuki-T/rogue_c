#include <stdio.h>
#include <stdlib.h>
#include "State.h"

#define TRUE 1
#define FALSE 0

void State_init(State *thisState) {
	thisState->gameTurn = 0;
	thisState->flr = 0;
	thisState->x = 0;
	thisState->y = 0;
}

void State_initArray(State *thisState, int mx, int my, int en) {
	// 配列の動的確保
	State_makeMapArray(&(thisState->map), mx, my, -1);
	State_makeMapArray(&(thisState->seem), mx, my, -1);
	State_makeMapArray(&(thisState->enemies), mx, my, -1);
	State_makeEnemyArray(&(thisState->killedEnemyTurn), en, 0);
	State_makeEnemyStArray(&(thisState->enemiesSt), en);
}

void State_finish(State *thisState) {
	State_finishArray(thisState);
}

void State_finishArray(State *thisState) {
	// 配列の要素数の計算
	int my = sizeof(thisState->map) / sizeof(thisState->map[0]);
	// 動的確保した配列の解放
	State_removeMapArray(&(thisState->map), my);
	State_removeMapArray(&(thisState->seem), my);
	State_removeMapArray(&(thisState->enemies), my);
	State_removeEnemyArray(&(thisState->killedEnemyTurn));
	State_removeEnemyStArray(&(thisState->enemiesSt));
}

void State_copy(State *thisState, State *copyState) {
	
}

void State_makeMapArray(int ***mapArray, int lengthX, int lengthY, int initVal) {
	*mapArray = (int**)malloc(sizeof(int) * lengthY);
	if (*mapArray == NULL) exit(1);

	for (int y = 0; y < lengthY; y++) {
		(*mapArray)[y] = (int*)malloc(sizeof(int) * lengthX);
		if ((*mapArray)[y] == NULL) exit(1);
	}

	// initValによる初期化
	for (int y = 0; y < lengthY; y++) {
		for (int x = 0; x < lengthX; x++) {
			(*mapArray)[y][x] = initVal;
		}
	}
}

void State_removeMapArray(int ***mapArray, int lengthY) {
	for (int y = 0; y < lengthY; y++) {
		free((*mapArray)[y]);
	}
	free(*mapArray);
}

void State_makeEnemyArray(int **enemyArray, int enemyLength, int initVal) {
	*enemyArray = (int*)malloc(sizeof(int) * enemyLength);
	if (*enemyArray == NULL) exit(1);

	// initValによる初期化
	for (int e = 0; e < enemyLength; e++) {
		(*enemyArray)[e] = initVal;
	}
}

void State_removeEnemyArray(int **enemyArray) {
	free(*enemyArray);
}

void State_makeEnemyStArray(Enemy **enemyStArray, int enemyLength) {
	*enemyStArray = (Enemy*)malloc(sizeof(Enemy) * enemyLength);
	if (*enemyStArray == NULL) exit(1);

	// 情報の初期化
	for (int e = 0; e < enemyLength; e++) {
		(*enemyStArray)[e].id = e;
		(*enemyStArray)[e].active = FALSE;
		(*enemyStArray)[e].x = 0;
		(*enemyStArray)[e].y = 0;
	}
}

void State_removeEnemyStArray(Enemy **enemyStArray) {
	free(*enemyStArray);
}
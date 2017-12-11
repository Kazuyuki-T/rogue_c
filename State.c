#include <stdio.h>
#include <stdlib.h>
#include "State.h"

void State_init(State *thisState) {
	
}

void State_initArray(State *thisState, int mx, int my, int en) {
	// 配列の動的確保
	State_makeMapArray(&(thisState->map), mx, my, 0);
	State_makeMapArray(&(thisState->seem), mx, my, 0);
	State_makeEnemyArray(&(thisState->enemies), en, 0);
	State_makeEnemyArray(&(thisState->killedEnemyTurn), en, 0);
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
	State_removeEnemyArray(&(thisState->enemies));
	State_removeEnemyArray(&(thisState->killedEnemyTurn));
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

void State_output(State *thisState, int mx, int my, int en) {
	printf("map\n");
	for (int y = 0; y < my; y++) {
		for (int x = 0; x < mx; x++) {
			printf("%d ", thisState->map[y][x]);
		}
		printf("\n");
	}
	printf("\n");

	printf("seem\n");
	for (int y = 0; y < my; y++) {
		for (int x = 0; x < mx; x++) {
			printf("%d ", thisState->seem[y][x]);
		}
		printf("\n");
	}
	printf("\n");
	
	printf("enemies\n");
	for (int e = 0; e < en; e++) {
		printf("%d ", thisState->enemies[e]);
	}
	printf("\n");
	
	printf("killed\n");
	for (int e = 0; e < en; e++) {
		printf("%d ", thisState->killedEnemyTurn[e]);
	}
	printf("\n");
}
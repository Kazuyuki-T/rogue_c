#include <stdio.h>
#include <stdlib.h>
#include "State.h"
#include "Rule.h"

void State_init(State *thisState, Rule *rule) {
	// Ruleから設定
	int mapX = rule->mapSizeX;
	int mapY = rule->mapSizeY;
	int enemyNum = rule->enemyNumber;
	
	// 配列の動的確保
	State_makeMapArray(&(thisState->map), mapX, mapY, 0);
	State_makeMapArray(&(thisState->seem), mapX, mapY, 1);
	State_makeEnemyArray(&(thisState->enemies), enemyNum, 10);
	State_makeEnemyArray(&(thisState->killedEnemyTurn), enemyNum, 100);
}

void State_finish(State *thisState) {
	// 動的確保した配列の解放
	State_removeMapArray(&(thisState->map), 5);
	State_removeMapArray(&(thisState->seem), 5);
	State_removeEnemyArray(&(thisState->enemies));
	State_removeEnemyArray(&(thisState->killedEnemyTurn));
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

void State_copy(State *thisState, State *copyState) {

}

void State_output(State *thisState) {
	printf("map\n");
	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 10; x++) {
			printf("%d ", thisState->map[y][x]);
		}
		printf("\n");
	}
	printf("\n");
	printf("seem\n");
	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 10; x++) {
			printf("%d ", thisState->seem[y][x]);
		}
		printf("\n");
	}
	printf("\n");
	printf("enemies\n");
	for (int e = 0; e < 4; e++) {
		printf("%d ", thisState->enemies[e]);
	}
	printf("\n");
	printf("killed\n");
	for (int e = 0; e < 4; e++) {
		printf("%d ", thisState->killedEnemyTurn[e]);
	}
	printf("\n");
}
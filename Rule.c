#include <stdio.h>
#include <stdlib.h>
#include "Rule.h"

void Rule_init(Rule *thisRule) {
	Rule_setMapSizeX(thisRule, 10);
	Rule_setMapSizeY(thisRule, 5);
	Rule_setEnemyNumber(thisRule, 4);
}

void Rule_finish(Rule *thisRule) {

}

void Rule_initState(Rule *thisRule, State *s) {
	int mapX = Rule_getMapSizeX(thisRule);
	int mapY = Rule_getMapSizeY(thisRule);
	int enemyNum = Rule_getEnemyNumber(thisRule);

	// 配列の動的確保
	Rule_makeMapArray(&(s->map), mapX, mapY, 0);
	Rule_makeMapArray(&(s->seem), mapX, mapY, 1);
	Rule_makeEnemyArray(&(s->enemies), enemyNum, 10);
	Rule_makeEnemyArray(&(s->killedEnemyTurn), enemyNum, 100);
}

void Rule_finishState(Rule *thisRule, State *s) {
	int mapY = Rule_getMapSizeY(thisRule);
	
	// 動的確保した配列の解放
	Rule_removeMapArray(&(s->map), mapY);
	Rule_removeMapArray(&(s->seem), mapY);
	Rule_removeEnemyArray(&(s->enemies));
	Rule_removeEnemyArray(&(s->killedEnemyTurn));
}

void Rule_transition(Rule *thisRule, State *currentState, int act) {
	// 遷移

	// player

	// enemy
}

void Rule_makeMapArray(int ***mapArray, int lengthX, int lengthY, int initVal) {
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

void Rule_removeMapArray(int ***mapArray, int lengthY) {
	for (int y = 0; y < lengthY; y++) {
		free((*mapArray)[y]);
	}
	free(*mapArray);
}

void Rule_makeEnemyArray(int **enemyArray, int enemyLength, int initVal) {
	*enemyArray = (int*)malloc(sizeof(int) * enemyLength);
	if (*enemyArray == NULL) exit(1);

	// initValによる初期化
	for (int e = 0; e < enemyLength; e++) {
		(*enemyArray)[e] = initVal;
	}
}

void Rule_removeEnemyArray(int **enemyArray) {
	free(*enemyArray);
}

void Rule_setMapSizeX(Rule *thisRule, int xsize) {
	thisRule->mapSizeX = xsize;
} 

int Rule_getMapSizeX(Rule *thisRule) {
	return thisRule->mapSizeX;
}

void Rule_setMapSizeY(Rule *thisRule, int ysize) {
	thisRule->mapSizeY = ysize;
}

int Rule_getMapSizeY(Rule *thisRule) {
	return thisRule->mapSizeY;
}

void Rule_setEnemyNumber(Rule *thisRule, int enemyNum) {
	thisRule->enemyNumber = enemyNum;
}

int Rule_getEnemyNumber(Rule *thisRule) {
	return thisRule->enemyNumber;
}
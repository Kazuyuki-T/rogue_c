#include <stdio.h>
#include <stdlib.h>
#include "State.h"

void State_init(State *thisState) {
	State_makeMapArray(thisState->map, 10, 5);
	State_makeMapArray(thisState->seem, 10, 5);
	State_makeEnemyArray(thisState->enemies, 4);
	State_makeEnemyArray(thisState->killedEnemyTurn, 4);
}

void State_finish(State *thisState) {
	State_removeMapArray(thisState->map, 10);
	State_removeMapArray(thisState->map, 10);
	State_removeEnemyArray(thisState->enemies);
	State_removeEnemyArray(thisState->killedEnemyTurn);
}

void State_makeMapArray(int **map, int sizex, int sizey) {
	map = (int**)malloc(sizeof(int) * sizey);
	if (map == NULL) exit(1);

	for (int x = 0; x < sizex; x++) {
		map[x] = (int*)malloc(sizeof(int)*sizex);
		if (map[x] == NULL) exit(1);
	}
}

void State_removeMapArray(int **map, int sizex) {
	for (int x = 0; x < sizex; x++) {
		free(map[x]);
	}
	free(map);
}

void State_makeEnemyArray(int *enemyArray, int enemySize) {
	enemyArray = (int*)malloc(sizeof(int) * enemySize);
	if (enemyArray == NULL) exit(1);
}

void State_removeEnemyArray(int *enemyArray) {
	free(enemyArray);
}
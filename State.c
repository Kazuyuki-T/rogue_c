#include <stdio.h>
#include <stdlib.h>
#include "State.h"

void State_init(State *thisState) {
	State_makeMapArray(&(thisState->map), 10, 5);
	State_makeMapArray(&(thisState->seem), 10, 5);
	State_makeEnemyArray(&(thisState->enemies), 4);
	State_makeEnemyArray(&(thisState->killedEnemyTurn), 4);

	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 10; x++) {
			thisState->map[y][x] = 0;
			thisState->seem[y][x] = 1;
		}
	}
	for (int e = 0; e < 4; e++) {
		thisState->enemies[e] = 10;
		thisState->killedEnemyTurn[e] = 100;
	}
}

void State_finish(State *thisState) {
	State_removeMapArray(&(thisState->map), 5);
	State_removeMapArray(&(thisState->seem), 5);
	State_removeEnemyArray(&(thisState->enemies));
	State_removeEnemyArray(&(thisState->killedEnemyTurn));
}

void State_makeMapArray(int ***map, int lengthX, int lengthY) {
	*map = (int**)malloc(sizeof(int) * lengthY);
	if (*map == NULL) exit(1);

	for (int y = 0; y < lengthY; y++) {
		(*map)[y] = (int*)malloc(sizeof(int) * lengthX);
		if ((*map)[y] == NULL) exit(1);
	}
}

void State_removeMapArray(int ***map, int lengthY) {
	for (int y = 0; y < lengthY; y++) {
		free((*map)[y]);
	}
	free(*map);
}

void State_makeEnemyArray(int **enemyArray, int enemySize) {
	*enemyArray = (int*)malloc(sizeof(int) * enemySize);
	if (*enemyArray == NULL) exit(1);
}

void State_removeEnemyArray(int **enemyArray) {
	free(*enemyArray);
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
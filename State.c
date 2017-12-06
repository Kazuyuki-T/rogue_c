#include <stdio.h>
#include "State.h"

void State_init(State *thisState) {
	
}

void State_finish(State *thisState) {
	
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
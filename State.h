#ifndef STATE_H
#define STATE_H


#include <stdio.h>
//#include "Rule.h"

typedef struct {
	int testState;

	// all OK
	int gameTurn;
	int flr;
	int hp, mhp, stm, lv, exp;
	int pt, fd, ar, st, itemNumber;
	int x, y;
	// NG or part of info
	int **map;
	int **seem;
	int *enemies;
	int *killedEnemyTurn;
} State;

// public
void State_init(State *thisState);

// public
void State_finish(State *thisState);

void State_makeMapArray(int ***map, int sizex, int sizey);

void State_removeMapArray(int ***map, int sizex);

void State_makeEnemyArray(int **enemyArray, int enemySize);

void State_removeEnemyArray(int **enemyArray);

void State_output(State *thisState);


#endif
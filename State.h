#ifndef STATE_H
#define STATE_H


#include <stdio.h>
#include "Rule.h"

typedef struct {
	int testState;

	// all OK
	int gameTurn;
	int flr;
	int hp, mhp, stm, lv, exp;
	int pt, fd, ar, st, itemNumber;
	int x, y;
	// NG or part
	int map[MAPSIZEY][MAPSIZEX];
	int seem[MAPSIZEY][MAPSIZEX];
	int enemies[ENEMYNUMBER];
	int killedEnemyTurn[ENEMYNUMBER];
} State;

// public
void State_init(State *thisState);

// public
void State_finish(State *thisState);


#endif
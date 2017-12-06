#ifndef STATE_H
#define STATE_H


#include <stdio.h>

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

// public，オブジェクトのコピー，thisState->copyState
void State_copy(State *thisState, State *copyState);

// public，テスト出力
void State_output(State *thisState);


#endif
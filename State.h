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

//
void State_initArray(State *thisState, int mx, int my, int en);

//
void State_finishArray(State *thisState);

// public，コピーの作成，thisState->copyState
void State_copy(State *thisState, State *copyState);

// private，マップ配列の動的確保
void State_makeMapArray(int ***mapArray, int lengthX, int lengthY, int initVal);

// private，動的確保したマップ配列の解放
void State_removeMapArray(int ***mapArray, int lengthY);

// private，敵配列の動的確保
void State_makeEnemyArray(int **enemyArray, int enemyLength, int initVal);

// private，動的確保した敵配列の解放
void State_removeEnemyArray(int **enemyArray);

// public，テスト出力
void State_output(State *thisState, int mx, int my, int en);


#endif
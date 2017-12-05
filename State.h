#ifndef STATE_H
#define STATE_H


#include <stdio.h>
//#include "Rule.h"

// 前方宣言，循環include error防止
struct Rule_t;
typedef struct Rule_t Rule;

//typedef struct {
//	int testState;
//
//	// all OK
//	int gameTurn;
//	int flr;
//	int hp, mhp, stm, lv, exp;
//	int pt, fd, ar, st, itemNumber;
//	int x, y;
//	// NG or part of info
//	int **map;
//	int **seem;
//	int *enemies;
//	int *killedEnemyTurn;
//} State;

struct State_t {
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
};
typedef struct State_t State;

// public
void State_init(State *thisState, Rule *rule);

// public
void State_finish(State *thisState);

// private，マップ配列の動的確保
void State_makeMapArray(int ***mapArray, int lengthX, int lengthY, int initVal);

// private，動的確保したマップ配列の解放
void State_removeMapArray(int ***mapArray, int lengthY);

// private，敵配列の動的確保
void State_makeEnemyArray(int **enemyArray, int enemyLength, int initVal);

// private，動的確保した敵配列の解放
void State_removeEnemyArray(int **enemyArray);

// public，オブジェクトのコピー，thisState->copyState
void State_copy(State *thisState, State *copyState);

// public，テスト出力
void State_output(State *thisState);


#endif
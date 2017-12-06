#ifndef RULE_H
#define RULE_H


#include <stdio.h>
#include "State.h"

typedef struct {
	int testRule;

	// private
	int mapSizeX;
	// private
	int mapSizeY;
	// private
	int enemyNumber;
} Rule;

// public
void Rule_init(Rule *thisRule);

// public
void Rule_finish(Rule *thisRule);

// public
void Rule_initArrayState(Rule *thisRule, State *s);

// public
void Rule_finishArrayState(Rule *thisRule, State *s);

// public，Stateの情報のセット，map，obj配置
void Rule_setStateInfo(Rule *thisRule, State *s);

// public
void Rule_transition(Rule *thisRule, State *currentState, int act);

//
int Rule_actionPlayer(Rule *thisRule, State *currentState, int act);

//
int Rule_convertActtoDir(int act);

// private，マップ配列の動的確保
void Rule_makeMapArray(int ***mapArray, int lengthX, int lengthY, int initVal);

// private，動的確保したマップ配列の解放
void Rule_removeMapArray(int ***mapArray, int lengthY);

// private，敵配列の動的確保
void Rule_makeEnemyArray(int **enemyArray, int enemyLength, int initVal);

// private，動的確保した敵配列の解放
void Rule_removeEnemyArray(int **enemyArray);

// private
void Rule_setMapSizeX(Rule *thisRule, int xsize);

// public
int Rule_getMapSizeX(Rule *thisRule);

// private
void Rule_setMapSizeY(Rule *thisRule, int ysize);

// public
int Rule_getMapSizeY(Rule *thisRule);

// private
void Rule_setEnemyNumber(Rule *thisRule, int enemyNum);

// public
int Rule_getEnemyNumber(Rule *thisRule);


#endif
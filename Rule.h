#ifndef RULE_H
#define RULE_H


#include <stdio.h>
#include "State.h"

#define MAPSIZEX 10
#define MAPSIZEY 10	
#define ENEMYNUMBER 4
#define TOPFLR 4

#define TRUE 1
#define FALSE 0

// actionPlayerからの戻り値
#define SUCCESS 1
#define FAILURE 0
#define NEXTFLR 2

#define GAME_CLEAR 1
#define GAME_OVER 2
#define GAME_PLAYING 0

typedef struct {
	int testRule;

	// private
	int mapSizeX;
	// private
	int mapSizeY;
	// private
	int enemyNumber;
	// private
	int topFlr;
} Rule;

State* Rule_init(void);

void Rule_finish(void);

State* Rule_getNextState(State* s, int act);

void Rule_makeArray(State* s);

void Rule_freeArray(State* s);

void Rule_copyState(State* cs, State* ns);

void actPlayerTest(State *s, int act);


// public，Stateの情報のセット，map，obj配置
void Rule_setStateInfo(State *s, int initFlag);

// 
int Rule_setState_setMap(State *s);

//
void Rule_setState_setStair(State *s, int* gridnum);

//
void Rule_setState_setItem(State *s, int* gridnum);

//
void Rule_setState_setEnemy(State *s, int* gridnum);

//
void Rule_setState_setEachEnemy(State *s, int* gridnum, int en);

//
void Rule_setState_setPlayer(State *s, int* gridnum);

//
void Rule_initPlayer(State *s);

//
void Rule_updateEnemyMap(State *s);

//
void Rule_updateSeemArea(State *s);

// public
int Rule_transition(State *currentState, int act);

// private
int Rule_actionPlayer(State *currentState, int act);

//
int Rule_judgeCollision(State *currentState, int nx, int ny);

//
void Rule_atkPlayer(State *currentState, int en, int atkDamage);

// private
void Rule_actionEnemy(State *currentState);

// private, act1~9 -> dir0~8
int Rule_convertActtoDir(int act);

// public
int Rule_getMapSizeX(Rule *thisRule);

// public
int Rule_getMapSizeY(Rule *thisRule);

// public
int Rule_getEnemyNumber(Rule *thisRule);

// private, min<=randval<=max
int Rule_getRandom(int min, int max);


void State_makeMapArray(int ***mapArray, int lengthX, int lengthY, int initVal);
void State_removeMapArray(int ***mapArray, int lengthY);
void State_makeEnemyArray(int **enemyArray, int enemyLength, int initVal);
void State_removeEnemyArray(int **enemyArray);
void State_makeEnemyStArray(Enemy **enemyStArray, int enemyLength);
void State_removeEnemyStArray(Enemy **enemyStArray);


#endif
#ifndef RULE_H
#define RULE_H


#include <stdio.h>
#include "State.h"

#define MAPSIZEX 10
#define MAPSIZEY 10

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

// public
void Rule_init(Rule *thisRule);

// public
void Rule_finish(Rule *thisRule);

// public，Stateの情報のセット，map，obj配置
void Rule_setStateInfo(Rule *thisRule, State *s);

// 
int Rule_setState_setMap(Rule *thisRule, State *s);

//
void Rule_setState_setStair(Rule *thisRule, State *s, int gridnum);

//
void Rule_setState_setItem(Rule *thisRule, State *s);

//
void Rule_setState_setEnemy(Rule *thisRule, State *s);

//
void Rule_setState_setPlayer(Rule *thisRule, State *s, int gridnum);

// public
int Rule_transition(Rule *thisRule, State *currentState, int act);

// private
int Rule_actionPlayer(Rule *thisRule, State *currentState, int act);

// private
void Rule_actionEnemy(Rule *thisRule, State *currentState);

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


#endif
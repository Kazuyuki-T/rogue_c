#ifndef RULE_H
#define RULE_H


#include <stdio.h>
#include "State.h"

#define MAPSIZEX 10
#define MAPSIZEY 10


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
void Rule_setState_setPlayer(Rule *thisRule, State *s);

// public
void Rule_transition(Rule *thisRule, State *currentState, int act);

// private
int Rule_actionPlayer(Rule *thisRule, State *currentState, int act);

// private
void Rule_actionEnemy(Rule *thisRule, State *currentState);

// private, act1~9 -> dir0~8
int Rule_convertActtoDir(int act);

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

// private, min<=randval<=max
int Rule_getRandom(int min, int max);


#endif
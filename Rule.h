#ifndef RULE_H
#define RULE_H


#include <stdio.h>
//#include "State.h"

struct State_t;
typedef struct State_t State;

//typedef struct {
//	int testRule;
//
//	// private
//	int mapSizeX;
//	// private
//	int mapSizeY;
//	// private
//	int enemyNumber;
//} Rule;

struct Rule_t {
	int testRule;

	// private
	int mapSizeX;
	// private
	int mapSizeY;
	// private
	int enemyNumber;
};
typedef struct Rule_t Rule;

// public
void Rule_init(Rule *thisRule);

// public
void Rule_finish(Rule *thisRule);

// public
void Rule_transition(Rule *thisRule, State *currentState, int act);

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
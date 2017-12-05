#ifndef RULE_H
#define RULE_H


#include <stdio.h>
#include "State.h"

typedef struct {
	int testRule;

	// private
	int mapsizex;
	// private
	int mapsizey;
	// private
	int enemyNumber;


} Rule;

// public
void Rule_init(Rule *thisRule);

// public
void Rule_finish(Rule *thisRule);

// private
void Rule_setMapSizeX(Rule *thisRule, int xsize);

// public
int Rule_getMapSizeX(Rule *thisRule);

// private
void Rule_setMapSizeY(Rule *thisRule, int ysize);

// public
int Rule_getMapSizeY(Rule *thisRule);

// public
void Rule_transition(Rule *thisRule, State *currentState, int act);


#endif
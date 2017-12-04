#ifndef RULE_H
#define RULE_H


#include <stdio.h>

#define MAPSIZEX 10
#define MAPSIZEY 10
#define ENEMYNUMBER 2

typedef struct {
	int testRule;

	// private
	int mapsizex;
	// private
	int mapsizey;


} Rule;



int aaa = 0;

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


#endif
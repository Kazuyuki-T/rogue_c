#include <stdio.h>
#include "Rule.h"

void Rule_init(Rule *thisRule) {
	Rule_setMapSizeX(thisRule, 50);
	Rule_setMapSizeY(thisRule, 30);
}

void Rule_finish(Rule *thisRule) {

}

void Rule_setMapSizeX(Rule *thisRule, int xsize) {
	thisRule->mapsizex = xsize;
}

int Rule_getMapSizeX(Rule *thisRule) {
	return thisRule->mapsizex;
}

void Rule_setMapSizeY(Rule *thisRule, int ysize) {
	thisRule->mapsizey = ysize;
}

int Rule_getMapSizeY(Rule *thisRule) {
	return thisRule->mapsizey;
}
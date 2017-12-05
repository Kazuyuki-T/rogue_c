#include <stdio.h>
#include "Rule.h"
#include "State.h"

void Rule_init(Rule *thisRule) {
	Rule_setMapSizeX(thisRule, 10);
	Rule_setMapSizeY(thisRule, 5);
	Rule_setEnemyNumber(thisRule, 4);
}

void Rule_finish(Rule *thisRule) {

}

void Rule_transition(Rule *thisRule, State *currentState, int act) {
	// 遷移

	// player

	// enemy
}

void Rule_setMapSizeX(Rule *thisRule, int xsize) {
	thisRule->mapSizeX = xsize;
} 

int Rule_getMapSizeX(Rule *thisRule) {
	return thisRule->mapSizeX;
}

void Rule_setMapSizeY(Rule *thisRule, int ysize) {
	thisRule->mapSizeY = ysize;
}

int Rule_getMapSizeY(Rule *thisRule) {
	return thisRule->mapSizeY;
}

void Rule_setEnemyNumber(Rule *thisRule, int enemyNum) {
	thisRule->enemyNumber = enemyNum;
}

int Rule_getEnemyNumber(Rule *thisRule) {
	return thisRule->enemyNumber;
}
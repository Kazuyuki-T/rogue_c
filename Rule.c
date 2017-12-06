#include <stdio.h>
#include <stdlib.h>
#include "Rule.h"

#define TRUE 1
#define FALSE 0

const int diffX[9] = { -1, 0, 1,-1, 0, 1,-1, 0, 1 };
const int diffY[9] = { 1, 1, 1, 0, 0, 0,-1,-1,-1 };

void Rule_init(Rule *thisRule) {
	Rule_setMapSizeX(thisRule, 10);
	Rule_setMapSizeY(thisRule, 5);
	Rule_setEnemyNumber(thisRule, 4);
}

void Rule_finish(Rule *thisRule) {

}

void Rule_initArrayState(Rule *thisRule, State *s) {
	// 配列の動的確保
	Rule_makeMapArray(&(s->map), thisRule->mapSizeX, thisRule->mapSizeY, 0);
	Rule_makeMapArray(&(s->seem), thisRule->mapSizeX, thisRule->mapSizeY, 0);
	Rule_makeEnemyArray(&(s->enemies), thisRule->enemyNumber, 0);
	Rule_makeEnemyArray(&(s->killedEnemyTurn), thisRule->enemyNumber, 0);
}

void Rule_finishArrayState(Rule *thisRule, State *s) {
	// 動的確保した配列の解放
	Rule_removeMapArray(&(s->map), thisRule->mapSizeY);
	Rule_removeMapArray(&(s->seem), thisRule->mapSizeY);
	Rule_removeEnemyArray(&(s->enemies));
	Rule_removeEnemyArray(&(s->killedEnemyTurn));
}

void Rule_setStateInfo(Rule *thisRule, State *s) {
	// マップの作製
	for (int y = 0; y < thisRule->mapSizeY; y++) {
		for (int x = 0; x < thisRule->mapSizeX; x++) {
			if (y == 0 || x == 0 || y == thisRule->mapSizeY - 1 || x == thisRule->mapSizeX - 1) {
				s->map[y][x] = 1;
			}
			else {
				s->map[y][x] = 0;
			}

			s->seem[y][x] = 1;
		}
	}

	// 階段

	// アイテム

	// 敵

	// プレイヤ
	s->x = 1;
	s->y = 1;
}

void Rule_transition(Rule *thisRule, State *currentState, int act) {
	// 遷移

	// プレイヤが行動したならば，敵を動かす
	int playerActionFlag = Rule_actionPlayer(thisRule, currentState, act);
	if (playerActionFlag == TRUE) {
		// enemy
	}
}

int Rule_actionPlayer(Rule *thisRule, State *currentState, int act) {
	//
	int dir = Rule_convertActtoDir(act);

	int nx = currentState->x + diffX[dir];
	int ny = currentState->y + diffY[dir];
	
	// マップの範囲外
	if(ny < 0 || thisRule->mapSizeY <= ny || nx < 0 || thisRule->mapSizeX <= nx){
		return FALSE;
	}

	// 進入禁止部分
	if(currentState->map[ny][nx] == 1){
		return FALSE;
	}

	// いずれにも当てはまらない->行動成功
	currentState->x += diffX[dir];
	currentState->y += diffY[dir];
	return TRUE;
}

int Rule_convertActtoDir(int act) {
	if (act == '1') {
		return 0;
	}
	else if (act == '2') {
		return 1;
	}
	else if (act == '3') {
		return 2;
	}
	else if (act == '4') {
		return 3;
	}
	else if (act == '5') {
		return 4;
	}
	else if (act == '6') {
		return 5;
	}
	else if (act == '7') {
		return 6;
	}
	else if (act == '8') {
		return 7;
	}
	else if (act == '9') {
		return 8;
	}
	else {
		return 4;
	}
}


void Rule_makeMapArray(int ***mapArray, int lengthX, int lengthY, int initVal) {
	*mapArray = (int**)malloc(sizeof(int) * lengthY);
	if (*mapArray == NULL) exit(1);

	for (int y = 0; y < lengthY; y++) {
		(*mapArray)[y] = (int*)malloc(sizeof(int) * lengthX);
		if ((*mapArray)[y] == NULL) exit(1);
	}

	// initValによる初期化
	for (int y = 0; y < lengthY; y++) {
		for (int x = 0; x < lengthX; x++) {
			(*mapArray)[y][x] = initVal;
		}
	}
}

void Rule_removeMapArray(int ***mapArray, int lengthY) {
	for (int y = 0; y < lengthY; y++) {
		free((*mapArray)[y]);
	}
	free(*mapArray);
}

void Rule_makeEnemyArray(int **enemyArray, int enemyLength, int initVal) {
	*enemyArray = (int*)malloc(sizeof(int) * enemyLength);
	if (*enemyArray == NULL) exit(1);

	// initValによる初期化
	for (int e = 0; e < enemyLength; e++) {
		(*enemyArray)[e] = initVal;
	}
}

void Rule_removeEnemyArray(int **enemyArray) {
	free(*enemyArray);
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
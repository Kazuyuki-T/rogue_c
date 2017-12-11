#include <stdio.h>
#include "Rule.h"

#define TRUE 1
#define FALSE 0

const int diffX[9] = { -1, 0, 1,-1, 0, 1,-1, 0, 1 };
const int diffY[9] = {  1, 1, 1, 0, 0, 0,-1,-1,-1 };

void Rule_init(Rule *thisRule) {
	//Rule_setMapSizeX(thisRule, 20);
	//Rule_setMapSizeY(thisRule, 20);
	//Rule_setEnemyNumber(thisRule, 4);

	thisRule->mapSizeX = 20;
	thisRule->mapSizeY = 20;
	thisRule->enemyNumber = 4;
}

void Rule_finish(Rule *thisRule) {

}

void Rule_setStateInfo(Rule *thisRule, State *s) {
	int roomGridNum;
	
	// マップの作製
	roomGridNum = Rule_setState_setMap(thisRule, s);
	// 階段
	Rule_setState_setStair(thisRule, s);
	// アイテム
	Rule_setState_setItem(thisRule, s);
	// 敵
	Rule_setState_setEnemy(thisRule, s);
	// プレイヤ
	Rule_setState_setPlayer(thisRule, s);
}

int Rule_setState_setMap(Rule *thisRule, State *s) {
	int count = 0;
	for (int y = 0; y < thisRule->mapSizeY; y++) {
		for (int x = 0; x < thisRule->mapSizeX; x++) {
			if (y == 0 || x == 0 || y == thisRule->mapSizeY - 1 || x == thisRule->mapSizeX - 1) {
				s->map[y][x] = 1;
			}
			else {
				s->map[y][x] = 0;
				count++;
			}
			s->seem[y][x] = 1;
		}
	}

	return count;
}

void Rule_setState_setStair(Rule *thisRule, State *s) {
	// 床の上に生成

}

void Rule_setState_setItem(Rule *thisRule, State *s) {

}

void Rule_setState_setEnemy(Rule *thisRule, State *s) {

}

void Rule_setState_setPlayer(Rule *thisRule, State *s) {
	s->x = 1;
	s->y = 1;
}

void Rule_transition(Rule *thisRule, State *currentState, int act) {
	// 遷移

	// プレイヤが行動したならば，敵を動かす
	int playerActionFlag = Rule_actionPlayer(thisRule, currentState, act);
	if (playerActionFlag == TRUE) {
		// enemy
		Rule_actionEnemy(thisRule, currentState);
	}
}

int Rule_actionPlayer(Rule *thisRule, State *currentState, int act) {
	// a:Arrow or a:Staff -> 数字入力
	// 1~9 or f:Food or p:Potion -> 直接行動


	// act1~9 -> dir0~8
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

void Rule_actionEnemy(Rule *thisRule, State *currentState) {

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
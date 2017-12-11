#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "Rule.h"

#define TRUE 1
#define FALSE 0

const int diffX[9] = { -1, 0, 1,-1, 0, 1,-1, 0, 1 };
const int diffY[9] = {  1, 1, 1, 0, 0, 0,-1,-1,-1 };

void Rule_init(Rule *thisRule) {
	srand((unsigned int)time(NULL));
	//srand(0);

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
	// マップの作製 -> obj配置できる床の数
	int roomGridNum = Rule_setState_setMap(thisRule, s);
	// 階段
	Rule_setState_setStair(thisRule, s, roomGridNum);
	// アイテム
	Rule_setState_setItem(thisRule, s);
	// プレイヤ
	Rule_setState_setPlayer(thisRule, s, roomGridNum);
	// 敵
	Rule_setState_setEnemy(thisRule, s);

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

void Rule_setState_setStair(Rule *thisRule, State *s, int gridnum) {
	// 床の上に生成
	int stairPos = Rule_getRandom(0, gridnum - 1);
	int count = 0;
	for (int y = 0; y < thisRule->mapSizeY; y++) {
		for (int x = 0; x < thisRule->mapSizeX; x++) {
			// 
			if (s->map[y][x] == 0) {
				if(count == stairPos){
					// 
					s->map[y][x] = 2;
					return;
				}
				count++;
			}
		}
	}
}

void Rule_setState_setItem(Rule *thisRule, State *s) {

}

void Rule_setState_setEnemy(Rule *thisRule, State *s) {

}

void Rule_setState_setPlayer(Rule *thisRule, State *s, int gridnum) {
	int playerPos = Rule_getRandom(0, gridnum - 1);
	int count = 0;
	for (int y = 0; y < thisRule->mapSizeY; y++) {
		for (int x = 0; x < thisRule->mapSizeX; x++) {
			// 
			if (s->map[y][x] == 0) {
				if (count == playerPos) {
					// 
					s->x = x;
					s->y = y;
					return;
				}
				count++;
			}
		}
	}
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
	// ↑↓←→ -> 直接行動
	if (act == 'a' || act == 's') {
		printf("                                                 \r");
		printf("direction : ");

		int actItem = _getch();
		if (actItem == 0 || actItem == 224)	actItem = _getch();
		int dir = Rule_convertActtoDir(actItem);
		if (dir != -1) {
			printf("                                                 \r");
			if (act == 'a') {
				printf("use arrrow %d", dir);
			}
			else if (act == 's') {
				printf("use staff %d", dir);
			}
		}
		else {
			printf("                                                 \r");
			printf("miss, ");
			return FALSE;
		}
	}
	else if (act == 'f' || act == 'p') {
		printf("                                                 \r");
		printf("f or p");
	}
	else {
		// act1~9 -> dir0~8
		int dir = Rule_convertActtoDir(act);

		int nx = currentState->x + diffX[dir];
		int ny = currentState->y + diffY[dir];

		// マップの範囲外
		if (ny < 0 || thisRule->mapSizeY <= ny || nx < 0 || thisRule->mapSizeX <= nx) {
			return FALSE;
		}

		// 進入禁止部分
		if (currentState->map[ny][nx] == 1) {
			return FALSE;
		}

		// いずれにも当てはまらない->行動成功
		currentState->x += diffX[dir];
		currentState->y += diffY[dir];
		return TRUE;
	}
}

void Rule_actionEnemy(Rule *thisRule, State *currentState) {

}

int Rule_convertActtoDir(int act) {
	if (act == '1') {
		return 0;
	}
	else if (act == '2' || act == 0x50) {
		return 1;
	}
	else if (act == '3') {
		return 2;
	}
	else if (act == '4' || act == 0x4b) {
		return 3;
	}
	else if (act == '5') {
		return 4;
	}
	else if (act == '6' || act == 0x4d) {
		return 5;
	}
	else if (act == '7') {
		return 6;
	}
	else if (act == '8' || act == 0x48) {
		return 7;
	}
	else if (act == '9') {
		return 8;
	}
	else {
		return -1;
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

int Rule_getRandom(int min, int max)
{
	return min + (int)(rand()*(max - min + 1.0) / (1.0 + RAND_MAX));
}
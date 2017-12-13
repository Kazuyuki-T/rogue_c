#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "Rule.h"

const int diffX[9] = { -1, 0, 1,-1, 0, 1,-1, 0, 1 };
const int diffY[9] = {  1, 1, 1, 0, 0, 0,-1,-1,-1 };

void Rule_init(Rule *thisRule) {
	srand((unsigned int)time(NULL));
	//srand(0);

	thisRule->mapSizeX = 20;
	thisRule->mapSizeY = 20;
	thisRule->enemyNumber = 4;
	thisRule->topFlr = 4;
}

void Rule_finish(Rule *thisRule) {

}

void Rule_setStateInfo(Rule *thisRule, State *s, int initFalg) {
	// obj配置できる床の数
	// この部分は連結リストで用意し，必要に応じて追加・削除したほうが良いか？
	// 最大数（マップのサイズ）が判明しているため，カーソルによる線形リストでも良いかもしれない

	// マップの作製->obj配置できる床の数
	int roomGridNum = Rule_setState_setMap(thisRule, s);
	// 階段
	Rule_setState_setStair(thisRule, s, &roomGridNum);
	// アイテム
	Rule_setState_setItem(thisRule, s, &roomGridNum);
	// プレイヤ
	Rule_setState_setPlayer(thisRule, s, &roomGridNum);
	// プレイヤの初期化を行うか否か
	// ゲームの初期化の際には行う
	// フロアの変更の際には行わない
	if(initFalg == TRUE)	Rule_initPlayer(thisRule, s);
	// 敵
	Rule_setState_setEnemy(thisRule, s, &roomGridNum);

	// map上の敵の座標を更新
	Rule_updateEnemyMap(thisRule, s);

	// プレイヤから見えている範囲を更新
	Rule_updateSeemArea(thisRule, s);
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

void Rule_setState_setStair(Rule *thisRule, State *s, int* gridnum) {
	// 床の上に生成
	int stairPos = Rule_getRandom(0, *gridnum - 1);
	(*gridnum)--;
	int count = 0;
	for (int y = 0; y < thisRule->mapSizeY; y++) {
		for (int x = 0; x < thisRule->mapSizeX; x++) {
			// 
			if (s->map[y][x] == 0) {
				if(count == stairPos){
					// mapに階段を設置
					s->map[y][x] = 2;
					return;
				}
				count++;
			}
		}
	}
}

void Rule_setState_setItem(Rule *thisRule, State *s, int* gridnum) {

}

void Rule_setState_setEnemy(Rule *thisRule, State *s, int* gridnum) {
	// 順番に生成
	for (int en = 0; en < thisRule->enemyNumber; en++) {
		Rule_setState_setEachEnemy(thisRule, s, gridnum, en);
	}
}

void Rule_setState_setEachEnemy(Rule *thisRule, State *s, int* gridnum, int en) {
	int enemyPos = Rule_getRandom(0, *gridnum - 1);
	(*gridnum)--;
	int count = 0;
	for (int y = 0; y < thisRule->mapSizeY; y++) {
		for (int x = 0; x < thisRule->mapSizeX; x++) {
			// 
			if (s->map[y][x] == 0) {
				if (count == enemyPos) {
					// 敵の初期座標の格納
					s->enemiesSt[en].x = x;
					s->enemiesSt[en].y = y;
					s->enemiesSt[en].active = TRUE;
					s->enemiesSt[en].mhp = 50;
					s->enemiesSt[en].hp = s->enemiesSt[en].mhp;
					return;
				}
				count++;
			}
		}
	}
}

void Rule_setState_setPlayer(Rule *thisRule, State *s, int* gridnum) {
	int playerPos = Rule_getRandom(0, *gridnum - 1);
	(*gridnum)--;
	int count = 0;
	for (int y = 0; y < thisRule->mapSizeY; y++) {
		for (int x = 0; x < thisRule->mapSizeX; x++) {
			// 
			if (s->map[y][x] == 0) {
				if (count == playerPos) {
					// プレイヤの初期座標の格納
					s->x = x;
					s->y = y;
					return;
				}
				count++;
			}
		}
	}
}

void Rule_initPlayer(Rule *thisRule, State *s) {
	s->mhp = 100;
	s->hp = s->mhp;
}

void Rule_updateEnemyMap(Rule *thisRule, State *s) {
	for (int y = 0; y < thisRule->mapSizeY; y++) {
		for (int x = 0; x < thisRule->mapSizeX; x++) {
			s->enemies[y][x] = -1;
		}
	}

	// 敵のいる座標：敵ID
	// その他：-1
	for (int en = 0; en < thisRule->enemyNumber; en++) {
		if (s->enemiesSt[en].active == TRUE) {
			int x = s->enemiesSt[en].x;
			int y = s->enemiesSt[en].y;
			s->enemies[y][x] = en;
		}
	}
}

void Rule_updateSeemArea(Rule *thisRule, State *s) {
	for (int y = 0; y < thisRule->mapSizeY; y++) {
		for (int x = 0; x < thisRule->mapSizeX; x++) {
			s->seem[y][x] = 1;
		}
	}
}

int Rule_countSetableObjGridNum(Rule *thisRule, State *s) {
	int count = 0;
	for (int y = 0; y < thisRule->mapSizeY; y++) {
		for (int x = 0; x < thisRule->mapSizeX; x++) {
			if (s->map[y][x] == 0) {
				count++;
			}
		}
	}

	return count;
}

int Rule_transition(Rule *thisRule, State *currentState, int act) {
	// 遷移

	// escのとき
	if (act == 0x1b)	return GAME_PLAYING;

	int playerAction = Rule_actionPlayer(thisRule, currentState, act);
	if (playerAction == SUCCESS) {
		// プレイヤが行動したならば，敵を動かす
		

		// ここで行うべきか？別の部分でまとめて行うべきでは？
		// map上の敵の座標を更新
		Rule_updateEnemyMap(thisRule, currentState);
		// プレイヤから見えている範囲を更新
		Rule_updateSeemArea(thisRule, currentState);


		// enemy
		Rule_actionEnemy(thisRule, currentState);

		return GAME_PLAYING;
	}
	else if (playerAction == FAILURE) {
		// プレイヤが行動しないとき

		return GAME_PLAYING;
	}
	else if (playerAction == NEXTFLR){
		// プレイヤの階層が変化したとき
		currentState->flr++;
		if (currentState->flr == thisRule->topFlr) {
			// 最上階到達 -> ゲームクリア
			return GAME_CLEAR;
		}
		else {
			// 途中 -> 新しい状況の生成
			Rule_setStateInfo(thisRule, currentState, FALSE);
			return GAME_PLAYING;
		}
	}
	else {
		// playerAction == ???
		return GAME_PLAYING;
	}
}

int Rule_actionPlayer(Rule *thisRule, State *currentState, int act) {
	// a:Arrow or a:Staff -> 数字入力
	// 1~9 or f:Food or p:Potion -> 直接行動
	// ↑↓←→ -> 直接行動

	// 矢or杖
	if (act == 'a' || act == 's') {
		printf("                                                 \r");
		printf("direction : "); // 方向の入力待ち

		int actItem = _getch();
		if (actItem == 0 || actItem == 224)	actItem = _getch();
		int dir = Rule_convertActtoDir(actItem);
		// -1のとき：0-8の方向に当てはまらない
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
			return FAILURE;
		}
	}
	// 食料 or ポーション
	else if (act == 'f' || act == 'p') {
		printf("                                                 \r");
		if (act == 'f') {
			printf("use food");
		}
		else if (act == 'p') {
			printf("use potion");
		}
	}
	// 階段降りるアクション，いらない？
	else if (act == 0x3c || act == 0x3e) {
		// もし階段が足元にあるのならば
	}
	// その他，移動・攻撃・待機
	else {
		// act1~9 -> dir0~8
		int dir = Rule_convertActtoDir(act);

		int nx = currentState->x + diffX[dir];
		int ny = currentState->y + diffY[dir];

		// マップの範囲外
		if (ny < 0 || thisRule->mapSizeY <= ny || nx < 0 || thisRule->mapSizeX <= nx) {
			return FAILURE;
		}
		// 進入禁止部分
		if (currentState->map[ny][nx] == 1) {
			return FAILURE;
		}
		// 斜め移動チェック
		//if (斜め攻撃の時) {
		//	return FAILURE;
		//}

		// いずれにも当てはまらない->行動成功
		// 攻撃or待機or移動

		// 敵との衝突判定
		int nextEnemy = Rule_judgeCollision(thisRule, currentState, nx, ny);
		if (nextEnemy != -1) {
			// 斜め攻撃判定 -> return
			Rule_atkPlayer(thisRule, currentState, nextEnemy, 10);
			printf("                                                 \r");
			printf("atk. ");
			for (int en = 0; en < thisRule->enemyNumber; en++) {
				printf("%d:%d-%d, ", en, currentState->enemiesSt[en].hp, currentState->enemiesSt[en].active);
			}
		}
		else {
			currentState->x += diffX[dir];
			currentState->y += diffY[dir];

			// 移動後の座標が階段だった場合
			if (currentState->map[currentState->y][currentState->x] == 2) {
				return NEXTFLR;
			}
		}
	}

	return SUCCESS;
}

int Rule_judgeCollision(Rule *thisRule, State *currentState, int nx, int ny) {
	for (int en = 0; en < thisRule->enemyNumber; en++) {
		if (currentState->enemiesSt[en].x == nx && currentState->enemiesSt[en].y == ny && currentState->enemiesSt[en].active == TRUE) {
			return en;
		}
	}
	return -1;
}

void Rule_atkPlayer(Rule *thisRule, State *currentState, int en, int atkDamage) {
	if (currentState->enemiesSt[en].hp - atkDamage <= 0) {
		// 敵の死亡処理
		currentState->enemiesSt[en].hp = 0;
		currentState->enemiesSt[en].active = FALSE;
		
		// 敵の再復活までのターンを設定

		// プレイヤに経験値を追加

	}
	else {
		currentState->enemiesSt[en].hp -= atkDamage;
	}
}

void Rule_actionEnemy(Rule *thisRule, State *currentState) {
	// enemyのアクション
	
	// enemymapの更新

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

int Rule_getMapSizeX(Rule *thisRule) {
	return thisRule->mapSizeX;
}

int Rule_getMapSizeY(Rule *thisRule) {
	return thisRule->mapSizeY;
}

int Rule_getEnemyNumber(Rule *thisRule) {
	return thisRule->enemyNumber;
}

int Rule_getRandom(int min, int max)
{
	return min + (int)(rand()*(max - min + 1.0) / (1.0 + RAND_MAX));
}
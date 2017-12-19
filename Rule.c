#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "Rule.h"

const int diffX[9] = { -1, 0, 1,-1, 0, 1,-1, 0, 1 };
const int diffY[9] = {  1, 1, 1, 0, 0, 0,-1,-1,-1 };

State currentState;
State nextState;

State* Rule_init(void) {
	srand((unsigned int)time(NULL));
	//srand(0);

	Rule_makeArray(&currentState);
	Rule_makeArray(&nextState);

	// obj配置できる床の数
	// この部分は連結リストで用意し，必要に応じて追加・削除したほうが良いか？
	// 最大数（マップのサイズ）が判明しているため，カーソルによる線形リストでも良いかもしれない

	// マップの作製->obj配置できる床の数
	int roomGridNum = Rule_setState_setMap(&currentState);
	// 階段
	Rule_setState_setStair(&currentState, &roomGridNum);
	// アイテム
	Rule_setState_setItem(&currentState, &roomGridNum);
	// プレイヤ
	Rule_setState_setPlayer(&currentState, &roomGridNum);
	// プレイヤの初期化
	Rule_initPlayer(&currentState);
	// 敵
	Rule_setState_setEnemy(&currentState, &roomGridNum);

	// map上の敵の座標を更新
	Rule_updateEnemyMap(&currentState);

	// プレイヤから見えている範囲を更新
	Rule_updateSeemArea(&currentState);

	return &currentState;
}

void Rule_finish() {
	Rule_freeArray(&currentState);
	Rule_freeArray(&nextState);
}

void Rule_makeArray(State* s) {
	// 配列の動的確保
	State_makeMapArray(&(s->map), MAPSIZEX, MAPSIZEY, -1);
	State_makeMapArray(&(s->seem), MAPSIZEX, MAPSIZEY, -1);
	State_makeMapArray(&(s->enemies), MAPSIZEX, MAPSIZEY, -1);
	State_makeEnemyArray(&(s->killedEnemyTurn), ENEMYNUMBER, 0);
	State_makeEnemyStArray(&(s->enemiesSt), ENEMYNUMBER);
}

void Rule_freeArray(State* s) {
	// 動的確保した配列の解放
	State_removeMapArray(&(s->map), MAPSIZEY);
	State_removeMapArray(&(s->seem), MAPSIZEY);
	State_removeMapArray(&(s->enemies), MAPSIZEY);
	State_removeEnemyArray(&(s->killedEnemyTurn));
	State_removeEnemyStArray(&(s->enemiesSt));
}

State* Rule_getNextState(State* s, int act) {
	// 状態+行動->新しい状態
	
	// 複製
	Rule_copyState(s, &nextState);


	// 
	printf("%d\n", act);

	return &nextState;
}

void Rule_copyState(State* cs, State* ns) {
	ns->gameTurn = cs->gameTurn;
	ns->gameFlag = cs->gameFlag;
	ns->flr = cs->flr;
	ns->hp = cs->hp;
	ns->mhp = cs->mhp;
	ns->stm = cs->stm;
	ns->lv = cs->lv;
	ns->exp = cs->exp;
	ns->pt = cs->pt;
	ns->fd = cs->fd;
	ns->ar = cs->ar;
	ns->st = cs->st;
	ns->itemNumber = cs->itemNumber;
	ns->x = cs->x;
	ns->y = cs->y;
	ns->testState = cs->testState;
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEY; x++) {
			ns->map[y][x] = cs->map[y][x];
			ns->seem[y][x] = cs->seem[y][x];
			ns->enemies[y][x] = cs->enemies[y][x];
		}
	}
	for (int en = 0; en < ENEMYNUMBER; en++) {
		ns->killedEnemyTurn[en] = cs->killedEnemyTurn[en];
		ns->enemiesSt[en].hp = cs->enemiesSt[en].hp;
		ns->enemiesSt[en].mhp = cs->enemiesSt[en].mhp;
		ns->enemiesSt[en].id = cs->enemiesSt[en].id;
		ns->enemiesSt[en].active = cs->enemiesSt[en].active;
		ns->enemiesSt[en].x = cs->enemiesSt[en].x;
		ns->enemiesSt[en].y = cs->enemiesSt[en].y;
		ns->enemiesSt[en].testEnemy = cs->enemiesSt[en].testEnemy;
	}
}

int Rule_setState_setMap(State *s) {
	int count = 0;
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			if (y == 0 || x == 0 || y == MAPSIZEY - 1 || x == MAPSIZEX - 1) {
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

void Rule_setState_setStair(State *s, int* gridnum) {
	// 床の上に生成
	int stairPos = Rule_getRandom(0, *gridnum - 1);
	(*gridnum)--;
	int count = 0;
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
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

void Rule_setState_setItem(State *s, int* gridnum) {

}

void Rule_setState_setEnemy(State *s, int* gridnum) {
	// 順番に生成
	for (int en = 0; en < ENEMYNUMBER; en++) {
		Rule_setState_setEachEnemy(s, gridnum, en);
	}
}

void Rule_setState_setEachEnemy(State *s, int* gridnum, int en) {
	int enemyPos = Rule_getRandom(0, *gridnum - 1);
	(*gridnum)--;
	int count = 0;
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
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

void Rule_setState_setPlayer(State *s, int* gridnum) {
	int playerPos = Rule_getRandom(0, *gridnum - 1);
	(*gridnum)--;
	int count = 0;
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
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

void Rule_initPlayer(State *s) {
	s->mhp = 100;
	s->hp = s->mhp;
}

void Rule_updateEnemyMap(State *s) {
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			s->enemies[y][x] = -1;
		}
	}

	// 敵のいる座標：敵ID
	// その他：-1
	for (int en = 0; en < ENEMYNUMBER; en++) {
		if (s->enemiesSt[en].active == TRUE) {
			int x = s->enemiesSt[en].x;
			int y = s->enemiesSt[en].y;
			s->enemies[y][x] = en;
		}
	}
}

void Rule_updateSeemArea(State *s) {
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			s->seem[y][x] = 1;
		}
	}
}












int Rule_countSetableObjGridNum(State *s) {
	int count = 0;
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			if (s->map[y][x] == 0) {
				count++;
			}
		}
	}

	return count;
}

int Rule_transition(State *currentState, int act) {
	// 遷移

	// escのとき
	if (act == 0x1b)	return GAME_PLAYING;

	int playerAction = Rule_actionPlayer(currentState, act);
	if (playerAction == SUCCESS) {
		// プレイヤが行動したならば，敵を動かす
		

		// ここで行うべきか？別の部分でまとめて行うべきでは？
		// map上の敵の座標を更新
		Rule_updateEnemyMap(currentState);
		// プレイヤから見えている範囲を更新
		Rule_updateSeemArea(currentState);


		// enemy
		Rule_actionEnemy(currentState);

		return GAME_PLAYING;
	}
	else if (playerAction == FAILURE) {
		// プレイヤが行動しないとき

		return GAME_PLAYING;
	}
	else if (playerAction == NEXTFLR){
		// プレイヤの階層が変化したとき
		currentState->flr++;
		if (currentState->flr == TOPFLR) {
			// 最上階到達 -> ゲームクリア
			return GAME_CLEAR;
		}
		else {
			// 途中 -> 新しい状況の生成
			Rule_setStateInfo(currentState, FALSE);
			return GAME_PLAYING;
		}
	}
	else {
		// playerAction == ???
		return GAME_PLAYING;
	}
}

void Rule_setStateInfo(State *s, int initFalg) {
	// obj配置できる床の数
	// この部分は連結リストで用意し，必要に応じて追加・削除したほうが良いか？
	// 最大数（マップのサイズ）が判明しているため，カーソルによる線形リストでも良いかもしれない

	// マップの作製->obj配置できる床の数
	int roomGridNum = Rule_setState_setMap(s);
	// 階段
	Rule_setState_setStair(s, &roomGridNum);
	// アイテム
	Rule_setState_setItem(s, &roomGridNum);
	// プレイヤ
	Rule_setState_setPlayer(s, &roomGridNum);
	// プレイヤの初期化を行うか否か
	// ゲームの初期化の際には行う
	// フロアの変更の際には行わない
	if (initFalg == TRUE)	Rule_initPlayer(s);
	// 敵
	Rule_setState_setEnemy(s, &roomGridNum);

	// map上の敵の座標を更新
	Rule_updateEnemyMap(s);

	// プレイヤから見えている範囲を更新
	Rule_updateSeemArea(s);
}

int Rule_actionPlayer(State *currentState, int act) {
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
		if (ny < 0 || MAPSIZEY <= ny || nx < 0 || MAPSIZEX <= nx) {
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
		int nextEnemy = Rule_judgeCollision(currentState, nx, ny);
		if (nextEnemy != -1) {
			// 斜め攻撃判定 -> return
			Rule_atkPlayer(currentState, nextEnemy, 10);
			printf("                                                 \r");
			printf("atk.");
			/*for (int en = 0; en < thisRule->enemyNumber; en++) {
				printf("%d:%d-%d, ", en, currentState->enemiesSt[en].hp, currentState->enemiesSt[en].active);
			}*/
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

int Rule_judgeCollision(State *currentState, int nx, int ny) {
	for (int en = 0; en < ENEMYNUMBER; en++) {
		if (currentState->enemiesSt[en].x == nx && currentState->enemiesSt[en].y == ny && currentState->enemiesSt[en].active == TRUE) {
			return en;
		}
	}
	return -1;
}

void Rule_atkPlayer(State *currentState, int en, int atkDamage) {
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

void Rule_actionEnemy(State *currentState) {
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

void State_makeMapArray(int ***mapArray, int lengthX, int lengthY, int initVal) {
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

void State_removeMapArray(int ***mapArray, int lengthY) {
	for (int y = 0; y < lengthY; y++) {
		free((*mapArray)[y]);
	}
	free(*mapArray);
}

void State_makeEnemyArray(int **enemyArray, int enemyLength, int initVal) {
	*enemyArray = (int*)malloc(sizeof(int) * enemyLength);
	if (*enemyArray == NULL) exit(1);

	// initValによる初期化
	for (int e = 0; e < enemyLength; e++) {
		(*enemyArray)[e] = initVal;
	}
}

void State_removeEnemyArray(int **enemyArray) {
	free(*enemyArray);
}

void State_makeEnemyStArray(Enemy **enemyStArray, int enemyLength) {
	*enemyStArray = (Enemy*)malloc(sizeof(Enemy) * enemyLength);
	if (*enemyStArray == NULL) exit(1);

	// 情報の初期化
	for (int e = 0; e < enemyLength; e++) {
		(*enemyStArray)[e].id = e;
		(*enemyStArray)[e].active = FALSE;
		(*enemyStArray)[e].x = 0;
		(*enemyStArray)[e].y = 0;
	}
}

void State_removeEnemyStArray(Enemy **enemyStArray) {
	free(*enemyStArray);
}
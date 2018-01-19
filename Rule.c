#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "Rule.h"


// 定数
#define MAPSIZEX 20
#define MAPSIZEY 20
#define ENEMY_NUMBER 4
#define ENEMY_REVTURN 50
#define ENEMY_MAXHP 50
#define ENEMY_POINT 100
#define TOPFLR 4
#define PLAYER_MAXHP 100
#define PLAYER_STM 100

// actionPlayerからの戻り値
#define SUCCESS 1
#define FAILURE 0
#define NEXTFLR 2

// 周囲8方向から求めるxy差分
const int diffX[9] = { -1, 0, 1,-1, 0, 1,-1, 0, 1 };
const int diffY[9] = {  1, 1, 1, 0, 0, 0,-1,-1,-1 };
// xy差分から求める周囲8方向
// 使用時には差分 -1~1 -> 0~2 に変換
const int dirNum[3][3] = { {6, 7, 8}, {3, 4, 5}, {0 ,1, 2} };

State currentState;
State nextState;

typedef struct {
	int testRule;

	// private
	int mapSizeX;
	// private
	int mapSizeY;
	// private
	int enemyNumber;
	// private
	int topFlr;
} Rule;

// 配列の動的確保
void Rule_makeArrays(State* s);
void Rule_reserveMapArray(int ***mapArray, int lengthX, int lengthY, int initVal);
void Rule_reserveEnemyArray(Enemy **enemyStArray, int enemyLength);
// 配列の解放
void Rule_removeArrays(State* s);
void Rule_freeMapArray(int ***mapArray, int lengthY);
void Rule_freeEnemyArray(Enemy **enemyStArray);
// Stateの情報のセット，map，obj配置
void Rule_setStateInfo(State *s, int initFlag);
int Rule_setMap(State *s);
void Rule_setStair(State *s, int* gridnum);
void Rule_setItems(State *s, int* gridnum);
void Rule_setEnemies(State *s, int* gridnum);
void Rule_setEachEnemy(State *s, int* gridnum, int en);
void Rule_setPlayer(State *s, int* gridnum);
// プレイヤの初期化
void Rule_initPlayer(State *s);
// 状態遷移の流れ
void Rule_transitState(State *s, int act);
// プレイヤの行動判定
int Rule_canActPlayer(State *s, int act);
// 敵との衝突判定
int Rule_getOrDefaultCollidedEnemyIndex(State *s, int nx, int ny);
// プレイヤの攻撃
void Rule_atkPlayer(State *s, int en, int atkDamage);
// 敵の行動
void Rule_actEnemies(State *s);
void Rule_actEachEnemy(State *s, int en);
// 敵の配置の更新
void Rule_updateEnemyMap(State *s);
// プレイヤの視界の更新
void Rule_updateSeemArea(State *s);
// act1~9 -> dir0~8
int Rule_convertActtoDir(int act);
// コピー，csの内容をnsにコピー
void Rule_copyState(State* cs, State* ns);
// min<=randval<=max
int Rule_getRandom(int min, int max);


int Rule_hasPlayerPosition(State *s, int en);
int Rule_canAttackPlayer(State *s, int en);

int Rule_canMoveUnit(State *s, int nx, int ny);

int Rule_convRangeM1to1(int);




State* Rule_init(void) {
	srand((unsigned int)time(NULL));
	//srand(0);

	Rule_makeArrays(&currentState);
	Rule_makeArrays(&nextState);

	// obj配置できる床の数
	// この部分は連結リストで用意し，必要に応じて追加・削除したほうが良いか？
	// 最大数（マップのサイズ）が判明しているため，カーソルによる線形リストでも良いかもしれない

	// 初期盤面の作成
	Rule_setStateInfo(&currentState, TRUE);

	return &currentState;
}

void Rule_destroy(void) {
	Rule_removeArrays(&currentState);
	Rule_removeArrays(&nextState);
}

void Rule_makeArrays(State* s) {
	// 配列の動的確保
	Rule_reserveMapArray(&(s->map), MAPSIZEX, MAPSIZEY, -1);
	Rule_reserveMapArray(&(s->seem), MAPSIZEX, MAPSIZEY, -1);
	Rule_reserveMapArray(&(s->enemies), MAPSIZEX, MAPSIZEY, -1);
	Rule_reserveEnemyArray(&(s->enemiesSt), ENEMY_NUMBER);
}

void Rule_reserveMapArray(int ***mapArray, int lengthX, int lengthY, int initVal) {
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

void Rule_reserveEnemyArray(Enemy **enemyStArray, int enemyLength) {
	*enemyStArray = (Enemy*)malloc(sizeof(Enemy) * enemyLength);
	if (*enemyStArray == NULL) exit(1);

	// 情報の初期化
	for (int e = 0; e < enemyLength; e++) {
		(*enemyStArray)[e].id = e;
		(*enemyStArray)[e].active = FALSE;
	}
}

void Rule_removeArrays(State* s) {
	// 動的確保した配列の解放
	Rule_freeMapArray(&(s->map), MAPSIZEY);
	Rule_freeMapArray(&(s->seem), MAPSIZEY);
	Rule_freeMapArray(&(s->enemies), MAPSIZEY);
	Rule_freeEnemyArray(&(s->enemiesSt));
}

void Rule_freeMapArray(int ***mapArray, int lengthY) {
	for (int y = 0; y < lengthY; y++) {
		free((*mapArray)[y]);
	}
	free(*mapArray);
}

void Rule_freeEnemyArray(Enemy **enemyStArray) {
	free(*enemyStArray);
}

void Rule_setStateInfo(State *s, int playerInitFalg) {
	// obj配置できる床の数
	// この部分は連結リストで用意し，必要に応じて追加・削除したほうが良いか？
	// 最大数（マップのサイズ）が判明しているため，カーソルによる線形リストでも良いかもしれない

	// マップの作製->obj配置できる床の数
	int roomGridNum = Rule_setMap(s);
	// 階段
	Rule_setStair(s, &roomGridNum);
	// アイテム
	Rule_setItems(s, &roomGridNum);
	// プレイヤ
	Rule_setPlayer(s, &roomGridNum);
	// プレイヤの初期化を行うか否か
	// ゲームの初期化の際には行う
	// フロアの変更の際には行わない
	if (playerInitFalg == TRUE) {
		Rule_initPlayer(s);
	}
	// 敵
	Rule_setEnemies(s, &roomGridNum);

	// map上の敵の座標を更新
	Rule_updateEnemyMap(s);

	// プレイヤから見えている範囲を更新
	Rule_updateSeemArea(s);

	// フラグを更新
	s->flrResetFlag = FALSE;
}

int Rule_setMap(State *s) {
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

void Rule_setStair(State *s, int* gridnum) {
	// 床の上に生成
	int stairPos = Rule_getRandom(0, (*gridnum) - 1);
	(*gridnum)--;
	int count = 0;
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			// 
			if (s->map[y][x] == 0) {
				if (count == stairPos) {
					// mapに階段を設置
					s->map[y][x] = 2;
					return;
				}
				count++;
			}
		}
	}
}

void Rule_setItems(State *s, int* gridnum) {

}

void Rule_setEnemies(State *s, int* gridnum) {
	// 順番に生成
	for (int en = 0; en < ENEMY_NUMBER; en++) {
		Rule_setEachEnemy(s, gridnum, en);
	}
}

void Rule_setEachEnemy(State *s, int* gridnum, int en) {
	int enemyPos = Rule_getRandom(0, (*gridnum) - 1);
	(*gridnum)--;
	int count = 0;
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			// 
			if (s->map[y][x] == 0 && (s->x != x || s->y != y) && s->enemies[y][x] == -1) {
				if (count == enemyPos) {
					// 敵の初期座標の格納
					s->enemies[y][x] = s->enemiesSt[en].id;
					s->enemiesSt[en].x = x;
					s->enemiesSt[en].y = y;
					s->enemiesSt[en].active = TRUE;
					s->enemiesSt[en].mhp = ENEMY_MAXHP;
					s->enemiesSt[en].hp = s->enemiesSt[en].mhp;
					s->enemiesSt[en].point = ENEMY_POINT;
					s->enemiesSt[en].killedEnemyTurn = 0;
					return;
				}
				count++;
			}
		}
	}
}

void Rule_setPlayer(State *s, int* gridnum) {
	int playerPos = Rule_getRandom(0, (*gridnum) - 1);
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
	s->mhp = PLAYER_MAXHP;
	s->hp = s->mhp;
	s->stm = PLAYER_STM;
	s->lv = 0;
	s->exp = 0;

	s->pt = 0;
	s->fd = 0;
	s->ar = 0;
	s->st = 0;
}

State* Rule_getNextState(State* s, int act) {
	// 状態 + 行動 -> 新しい状態
	
	// Stateの複製
	Rule_copyState(s, &nextState);
	// Stateの更新
	Rule_transitState(&nextState, act);

	return &nextState;
}

void Rule_transitState(State *s, int act) {
	// 遷移

	// escのとき
	if (act == 0x1b) {
		s->gameFlag = GAME_PLAYING;
	}
	else {
		int playerAction = Rule_canActPlayer(s, act);
		if (playerAction == TRUE) {
			// プレイヤが行動したならば，敵を動かす

			// enemy
			Rule_actEnemies(s);

			// ここで行うべきか？別の部分でまとめて行うべきでは？
			// map上の敵の座標を更新
			Rule_updateEnemyMap(s);
			// プレイヤから見えている範囲を更新
			Rule_updateSeemArea(s);

			(s->gameTurn)++;
			s->gameFlag = GAME_PLAYING;
		}
		else{
			// プレイヤが行動しないとき
			s->gameFlag = GAME_PLAYING;
		}
		
		// フロアの更新チェック
		if (s->flrResetFlag == TRUE) {
			// プレイヤの階層が変化したとき
			s->flrNum++;
			if (s->flrNum == TOPFLR) {
				// 最上階到達 -> ゲームクリア
				s->gameFlag = GAME_CLEAR;
			}
			else {
				// 途中 -> 新しい状況の生成
				Rule_setStateInfo(s, FALSE);
				(s->gameTurn)++;
				s->gameFlag = GAME_PLAYING;
			}
		}
	}
}

int Rule_canActPlayer(State *s, int act) {
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
			return FALSE;
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

		int nx = s->x + diffX[dir];
		int ny = s->y + diffY[dir];

		// State, xy座標
		if (Rule_canMoveUnit(s, nx, ny) == FALSE) {
			return FALSE;
		}

		// いずれにも当てはまらない->行動成功
		// 攻撃or待機or移動

		// 敵との衝突判定
		int nextEnemyIndex = Rule_getOrDefaultCollidedEnemyIndex(s, nx, ny);
		if (nextEnemyIndex != -1) {
			// 斜め攻撃判定 -> return
			Rule_atkPlayer(s, nextEnemyIndex, 10);
			printf("                                                 \r");
			printf("atk.");
			/*for (int en = 0; en < thisRule->enemyNumber; en++) {
			printf("%d:%d-%d, ", en, currentState->enemiesSt[en].hp, currentState->enemiesSt[en].active);
			}*/
		}
		else {
			s->x += diffX[dir];
			s->y += diffY[dir];

			// 移動後の座標が階段だった場合
			if (s->map[s->y][s->x] == 2) {
				s->flrResetFlag = TRUE;
			}
		}
	}

	return TRUE;
}

int Rule_canMoveUnit(State *s, int nx, int ny) {
	// マップの範囲外
	if (ny < 0 || MAPSIZEY <= ny || nx < 0 || MAPSIZEX <= nx) {
		return FALSE;
	}
	// 進入禁止部分
	if (s->map[ny][nx] == 1) {
		return FALSE;
	}
	// 斜め移動チェック
	//if (斜め移動の時) {
	//	return FALSE;
	//}

	return TRUE;
}

// (State, unitNextX, unitNextY)
int Rule_getOrDefaultCollidedEnemyIndex(State *s, int nx, int ny) {
	for (int en = 0; en < ENEMY_NUMBER; en++) {
		if (s->enemiesSt[en].x == nx && s->enemiesSt[en].y == ny && s->enemiesSt[en].active == TRUE) {
			return s->enemiesSt[en].id;
		}
	}
	return -1;
}

// (State, enemyNum)
int Rule_isCollidedPlayer(State *s, int en) {
	for(int dir = 0; dir < 9; dir++) {
		// 周囲８マスにいるか
		if (s->x == (s->enemiesSt[en].x + diffX[dir]) && s->y == (s->enemiesSt[en].y + diffY[dir])) {
			return TRUE;
		}
	}
	return FALSE;
}

void Rule_atkPlayer(State *s, int en, int atkDamage) {
	if (s->enemiesSt[en].hp - atkDamage <= 0) {
		// 敵の死亡処理
		s->enemiesSt[en].hp = 0;
		s->enemiesSt[en].active = FALSE;

		// 敵の再復活までのターンを設定
		s->enemiesSt[en].killedEnemyTurn = ENEMY_REVTURN;

		// プレイヤに経験値を追加
		s->exp += s->enemiesSt[en].point;
		//(s->exp) += 100;
		printf("%d\n", s->enemiesSt[en].point);
	}
	else {
		s->enemiesSt[en].hp -= atkDamage;
	}
}

void Rule_actEnemies(State *s) {
	// 順番に行動
	for (int en = 0; en < ENEMY_NUMBER; en++) {
		Rule_actEachEnemy(s, en);
	}
}

int Rule_convRangeM1to1(int dn) {
	if (dn > 1) {
		dn = 1;
	}
	else if(dn < -1) {
		dn = -1;
	}

	return dn;
}

void Rule_actEachEnemy(State *s, int en) {
	// 行動の選択

	// 生きているとき
	if (s->enemiesSt[en].active == TRUE) {
		// プレイヤを確認できる
		if (Rule_hasPlayerPosition(s, en) == TRUE) {
			// 攻撃できる
			if (Rule_canAttackPlayer(s, en) == TRUE) {

			}
			// 攻撃できない->最短距離で移動
			else {
				// enemy mapの座標初期化
				s->enemies[s->enemiesSt[en].y][s->enemiesSt[en].x] = -1;
				
				int ndiffx = Rule_convRangeM1to1((s->x) - (s->enemiesSt[en].x));
				int ndiffy = Rule_convRangeM1to1((s->y) - (s->enemiesSt[en].y));

				// 差分から次の移動方向を決定
				// (x, y) = (cx, cy) + (nx, ny)
				int nx = (s->enemiesSt[en].x) + ndiffx;
				int ny = (s->enemiesSt[en].y) + ndiffy;

				// (x, y)に移動できる
					// 移動
				// (x, y)に移動できない
					// (x, y)の左右２方向の座標確認
					// 移動できる
						// 移動
					// 移動できない
						// その場で待機


				// 移動可能（mapの進行不可領域に侵入しない）で，敵同士の衝突なし
				if (Rule_canMoveUnit(s, nx, ny) == TRUE && Rule_getOrDefaultCollidedEnemyIndex(s, nx, ny) == -1) {
					s->enemiesSt[en].x = nx;
					s->enemiesSt[en].y = ny;
				}
				else {

				}

				// map更新
				s->enemies[s->enemiesSt[en].y][s->enemiesSt[en].x] = s->enemiesSt[en].id;
			}
		}
		// プレイヤを確認できない
		else {
			// ランダムムーブ

			// 理想
			// 部屋->ランダムに次に向かう通路を決定
			// 通路->過去の履歴から戻らないように移動

			// 現実，とりあえず実装
			// ８方向の中からランダム
		}
	}
	// 死んでいるとき
	else {
		if (s->enemiesSt[en].killedEnemyTurn > 0) {
			s->enemiesSt[en].killedEnemyTurn--;
		}
		else {
			// 敵を復活
			int count = 0;
			for (int y = 0; y < MAPSIZEY; y++) {
				for (int x = 0; x < MAPSIZEX; x++) {
					if (s->map[y][x] == 0 && (s->x != x || s->y != y) && s->enemies[y][x] == -1) {
						count++;
					}
				}
			}

			Rule_setEachEnemy(s, &(count), en);
		}
	}
	
	// 行動したとき -> enemymapの更新
	Rule_updateEnemyMap(s);
}

int Rule_hasPlayerPosition(State *s, int en) {
	// 敵の位置からプレイヤが見えるか
	
	return TRUE;
}

int Rule_canAttackPlayer(State *s, int en) {
	// 周囲８マスにいて，攻撃が通る（斜め攻撃判定）
	if (Rule_isCollidedPlayer(s, s->enemiesSt[en].id) == TRUE) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void Rule_updateEnemyMap(State *s) {
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			s->enemies[y][x] = -1;
		}
	}

	// 敵のいる座標：敵ID
	// その他：-1
	for (int en = 0; en < ENEMY_NUMBER; en++) {
		if (s->enemiesSt[en].active == TRUE) {
			s->enemies[s->enemiesSt[en].y][s->enemiesSt[en].x] = s->enemiesSt[en].id;
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

void Rule_copyState(State* cs, State* ns) {
	ns->gameFlag = cs->gameFlag;
	ns->gameTurn = cs->gameTurn;

	ns->flrNum = cs->flrNum;
	ns->flrTurn = cs->flrTurn;
	ns->flrResetFlag = cs->flrResetFlag;
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
	for (int en = 0; en < ENEMY_NUMBER; en++) {
		ns->enemiesSt[en].hp = cs->enemiesSt[en].hp;
		ns->enemiesSt[en].mhp = cs->enemiesSt[en].mhp;
		ns->enemiesSt[en].id = cs->enemiesSt[en].id;
		ns->enemiesSt[en].active = cs->enemiesSt[en].active;
		ns->enemiesSt[en].x = cs->enemiesSt[en].x;
		ns->enemiesSt[en].y = cs->enemiesSt[en].y;
		ns->enemiesSt[en].point = cs->enemiesSt[en].point;
		ns->enemiesSt[en].killedEnemyTurn = cs->enemiesSt[en].killedEnemyTurn;
		
		ns->enemiesSt[en].testEnemy = cs->enemiesSt[en].testEnemy;
	}
}

int Rule_getRandom(int min, int max)
{
	return min + (int)(rand()*(max - min + 1.0) / (1.0 + RAND_MAX));
}

int Rule_getMapSizeX(void) {
	return MAPSIZEX;
}

int Rule_getMapSizeY(void) {
	return MAPSIZEY;
}

int Rule_getEnemyNum(void) {
	return ENEMY_NUMBER;
}


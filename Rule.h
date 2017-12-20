#ifndef RULE_H
#define RULE_H


#include <stdio.h>
#include "State.h"

#define MAPSIZEX 10
#define MAPSIZEY 10
#define ENEMY_NUMBER 4
#define ENEMY_REVTURN 50
#define TOPFLR 4
#define PLAYER_MAXHP 100
#define PLAYER_STM 100

// actionPlayerからの戻り値
#define SUCCESS 1
#define FAILURE 0
#define NEXTFLR 2

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

// 初期化関数
State* Rule_init(void);

// 終了関数，動的確保配列の解放
void Rule_finish(void);

// 配列の動的確保，解放
void Rule_makeArray(State* s);
void Rule_freeArray(State* s);
void Rule_makeMapArray(int ***mapArray, int lengthX, int lengthY, int initVal);
void Rule_removeMapArray(int ***mapArray, int lengthY);
void Rule_makeEnemyArray(int **enemyArray, int enemyLength, int initVal);
void Rule_removeEnemyArray(int **enemyArray);
void Rule_makeEnemyStArray(Enemy **enemyStArray, int enemyLength);
void Rule_removeEnemyStArray(Enemy **enemyStArray);

// Stateの情報のセット，map，obj配置
void Rule_setStateInfo(State *s, int initFlag);
int Rule_setMap(State *s);
void Rule_setStair(State *s, int* gridnum);
void Rule_setItem(State *s, int* gridnum);
void Rule_setEnemy(State *s, int* gridnum);
void Rule_setEachEnemy(State *s, int* gridnum, int en);
void Rule_setPlayer(State *s, int* gridnum);

// プレイヤの初期化
void Rule_initPlayer(State *s);

// 状態+行動->新しい状態
State* Rule_getNextState(State* s, int act);

// 状態遷移の流れ
void Rule_transitState(State *s, int act);

// プレイヤの行動判定
int Rule_actPlayer(State *s, int act);

// 敵との衝突判定
int Rule_judgeCollision(State *currentState, int nx, int ny);

// プレイヤの攻撃
void Rule_atkPlayer(State *currentState, int en, int atkDamage);

// 敵の行動
void Rule_actEnemy(State *currentState);
void Rule_actEachEnemy(State *currentState, int en);

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


#endif
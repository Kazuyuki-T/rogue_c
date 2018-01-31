#ifndef STATE_H
#define STATE_H


static const int TRUE = 1;
static const int FALSE = 0;

static const int GAME_CLEAR = 1;
static const int GAME_OVER = 2;
static const int GAME_PLAYING = 0;

typedef struct {
	int testEnemy;

	int id; // map更新時使用
	int active;
	int hp, mhp;
	int x, y;
	int point;
	int killedEnemyTurn;
} Enemy;

typedef struct {
	int id; // map
	int active;
	int itemid;
	int x;
	int y;
} Item;

typedef struct {
	int testState;
	
	// 公開
	int gameFlag;
	int gameTurn;
	int flrResetFlag;
	int flrNum;
	int flrTurn;
	int x, y;
	int hp, mhp, stm, lv, exp;
	
	int lvupExp, lvupExpSum;
	double autoHealVal;

	int pt, fd, ar, st, itemNumber;


	// 一部非公開
	int **map;
	int **seem;
	int **enemies;
	Enemy* enemiesSt;
} State;


#endif
#ifndef STATE_H
#define STATE_H


#define TRUE 1
#define FALSE 0

#define GAME_CLEAR 1
#define GAME_OVER 2
#define GAME_PLAYING 0

typedef struct {
	int testEnemy;

	int id;
	int active;
	int hp, mhp;
	int x, y;
	int point;
	int killedEnemyTurn;
} Enemy;

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
	int pt, fd, ar, st, itemNumber;
	
	// 一部非公開
	int **map;
	int **seem;
	int **enemies;
	Enemy* enemiesSt;
} State;


#endif
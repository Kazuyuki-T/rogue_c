#ifndef STATE_H
#define STATE_H


#include <stdio.h>

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
} Enemy;

typedef struct {
	int testState;
	int gameFlag;

	// all OK
	int gameTurn;
	int flr;
	int hp, mhp, stm, lv, exp;
	int pt, fd, ar, st, itemNumber;
	int x, y;
	// NG or part of info
	int **map;
	int **seem;
	int **enemies;
	int *killedEnemyTurn;

	Enemy* enemiesSt;
} State;


#endif
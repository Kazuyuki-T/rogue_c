#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H


#define TRUE 1
#define FALSE 0

#include <stdio.h>
//#include "State.h"
#include "Player.h"
//#include "Rule.h"

struct Rule_t;
typedef struct Rule_t Rule;
struct State_t;
typedef struct State_t State;
struct Player_t;
typedef struct Player_t Player;

typedef struct {
	// private
	State state;
	// private
	Player player;
	// private
	Rule rule;
} GameManager;

// public
void GameManager_init(GameManager *thisGM);

// public
void GameManager_finish(GameManager *thisGM);

// public
void GameManager_run(GameManager *thisGM);

// private
void GameManager_setStage(GameManager *thisGM, State *s);

// private
void GameManager_setPlayer(GameManager *thisGM, Player *p);

// private
void GameManager_setRule(GameManager *thisGM, Rule *r);


#endif
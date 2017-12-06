#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H


#include <stdio.h>
#include "Rule.h"
#include "State.h"
#include "Player.h"

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
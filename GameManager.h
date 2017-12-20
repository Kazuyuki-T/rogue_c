#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H


#include <stdio.h>
#include "Rule.h"
#include "State.h"
#include "Player.h"

// public
int GameManager_run(void);

void GameManager_outputMap(State* s);


#endif
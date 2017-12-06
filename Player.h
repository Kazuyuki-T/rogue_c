#ifndef PLAYER_H
#define PLAYER_H


#include <stdio.h>
#include <conio.h>
#include "State.h"

typedef struct {
	int testPlayer;
} Player;

// public
void Player_init(Player *thisPlayer);

// public
void Player_finish(Player *thisPlayer);

// public
int Player_getAction(Player *thisPlayer, State *currentState);


#endif
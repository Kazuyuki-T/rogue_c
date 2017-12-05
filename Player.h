#ifndef PLAYER_H
#define PLAYER_H


#include <stdio.h>
//#include "State.h"

struct State_t;
typedef struct State_t State;

//typedef struct {
//	int testPlayer;
//} Player;

struct Player_t {
	int testPlayer;
};
typedef struct Player_t Player;

// public
void Player_init(Player *thisPlayer);

// public
void Player_finish(Player *thisPlayer);

// public
int Player_getAction(Player *thisPlayer, State *currentState);


#endif
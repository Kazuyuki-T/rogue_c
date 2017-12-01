#ifndef PLAYER_H
#define PLAYER_H


#include <stdio.h>

typedef struct {
	int testPlayer;
} Player;

// public
void Player_init(Player *thisPlayer);

// public
void Player_finish(Player *thisPlayer);


#endif
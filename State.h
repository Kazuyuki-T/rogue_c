#ifndef STATE_H
#define STATE_H


#include <stdio.h>

typedef struct {
	int testState;
} State;

// public
void State_init(State *thisState);

// public
void State_finish(State *thisState);


#endif
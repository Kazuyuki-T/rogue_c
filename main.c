#include <stdio.h>
#include "GameManager.h"

int main(void)
{
	GameManager gm;
	GameManager_init(&gm);

	GameManager_run(&gm);

	// test

	GameManager_finish(&gm);

	return 0;
}
#include <stdio.h>
#include "GameManager.h"

int main(void)
{
	GameManager gm;
	GameManager_init(&gm);

	GameManager_run(&gm);

	return 0;
}
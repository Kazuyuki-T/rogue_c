#include <stdio.h>
#include "GameManager.h"

int main(void)
{
	GameManager gm;
	GameManager_init(&gm);

	// 実行
	GameManager_run(&gm);

	GameManager_finish(&gm);

	return 0;
}
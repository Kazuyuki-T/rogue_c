#include <stdio.h>
#include "GameManager.h"

int main(void)
{
	GameManager gm;
	GameManager_init(&gm); // 初期化

	GameManager_run(&gm); // 実行

	GameManager_finish(&gm); // 終了

	return 0;
}
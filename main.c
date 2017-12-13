#include <stdio.h>
#include "GameManager.h"

int main(void)
{
	GameManager gm;

	// GM_initとGM_finishはセット

	GameManager_init(&gm); // 初期化

	int result = GameManager_run(&gm); // 実行
	
	if (result == GAME_CLEAR) {
		printf("GAME CLEAR!!\n");
	}
	else if (result == GAME_OVER) {
		printf("GAME OVER...\n");
	}
	else {
		// 途中終了escとか
	}

	GameManager_finish(&gm); // 終了

	return 0;
}
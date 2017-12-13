#include <stdio.h>
#include "GameManager.h"

int main(void)
{
	GameManager gm;

	// GM_initとGM_finishはセット

	GameManager_init(&gm); // 初期化手続き

	int result = GameManager_run(&gm); // 実行
	
	if (result == GAME_CLEAR) {
		printf("GAME CLEAR!!\n");
	}
	else if (result == GAME_OVER) {
		printf("GAME OVER...\n");
	}
	else {
		// result == GAME_OVER での終了
		// 途中終了：escとか
	}

	GameManager_finish(&gm); // 終了手続き

	return 0;
}
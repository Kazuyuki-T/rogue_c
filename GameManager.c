#include <stdio.h>
#include "GameManager.h"

//#define DEBUG

int main(void)
{
	int result = GameManager_run(); // 実行

	if (result == GAME_CLEAR) {
		printf("GAME CLEAR!!\n");
	}
	else if (result == GAME_OVER) {
		printf("GAME OVER...\n");
	}
	else {
		// result == GAME_OVER での終了
		// 途中終了：escとか
		printf("途中終了\n");
	}

	return 0;
}

int GameManager_run() {
	#ifdef DEBUG
	printf("Game start\n");
	#endif // DEBUG

	State* cState = Rule_init();
	State* nState;

	///////////////
	// main loop //
	///////////////
	while (TRUE) {
		int act = Player_decideAction(cState);
		nState = Rule_getNextState(cState, act);
		cState = nState; // ポインタ付け替え

		if (cState->gameFlag == GAME_PLAYING) {
			// ゲームプレイ中
			// esc
			if (act == 0x1B) {
				break;
			}
		}
		else {
			// ゲームクリアorゲームオーバー
			break;
		}
	}

	int result = cState->gameFlag;
	Rule_finish();
	printf("test");
	return result;

	#ifdef DEBUG
	printf("Game end\n");
	#endif // DEBUG
}

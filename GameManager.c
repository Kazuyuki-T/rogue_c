#include <stdio.h>
#include <stdlib.h>
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

int GameManager_run(void) {
	#ifdef DEBUG
	printf("Game start\n");
	#endif // DEBUG

	// 現在・次状態を表すStateの
	State* cState = Rule_init(); // 現在のStateの初期化
	State* nState; // 次のStateを用意，配列確保等

	// テスト出力用
	GameManager_outputMap(cState);
	GameManager_outputPlayerInfo(cState);
	GameManager_outputEnemiesInfo(cState);

	///////////////
	// main loop //
	///////////////
	while (TRUE) {
		int act = Player_decideAction(cState);
		nState = Rule_getNextState(cState, act);
		cState = nState; // ポインタ付け替え
		
		GameManager_outputMap(cState);
		GameManager_outputPlayerInfo(cState);
		GameManager_outputEnemiesInfo(cState);

		if (cState->gameFlag == GAME_PLAYING) { // ゲームプレイ中
			// esc
			if (act == 0x1B) {
				break;
			}
		}
		else { // ゲームクリアorゲームオーバー
			break;
		}
	}

	int result = cState->gameFlag;
	Rule_destroy();
	return result;

	#ifdef DEBUG
	printf("Game end\n");
	#endif // DEBUG
}

void GameManager_outputMap(State* s) {
	system("cls");
	//gotoxy(1,1); // conio.hの関数，windows環境では使えない？
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			// 描画の優先度に注意
			if (s->y == y && s->x == x) {
				printf("@ ");
			}
			else if (s->enemies[y][x] != -1) {
				printf("$ ");
			}
			else if (s->map[y][x] == 2) {
				printf("%% "); // %%で%出力
			}
			else if (s->map[y][x] == 0) {
				printf(". ");
			}
			else if (s->map[y][x] == 1) {
				printf("# ");
			}
			else {
				printf("_ ");
			}
		}
		printf("\n");
	}
	printf("\n");
}

void GameManager_outputPlayerInfo(State* s) {
	printf("flr:%d, ", s->flr);
	printf("turn:%d, ", s->gameTurn);
	printf("(%d, %d), ", s->x, s->y);
	printf("\n");
	
	printf("Hp/maxHp:%d/%d, ", s->hp, s->mhp);
	printf("stm:%d, ", s->stm);
	printf("lv:%d, ", s->lv);
	printf("exp:%d, ", s->exp);
	printf("\n");
	
	printf("pt:%d, ", s->pt);
	printf("fd:%d, ", s->fd);
	printf("ar:%d, ", s->ar);
	printf("st:%d, ", s->st);
	printf("\n");
}

void GameManager_outputEnemiesInfo(State* s) {
	for (int e = 0; e < ENEMY_NUMBER; e++) {
		printf("%d : ", s->enemiesSt[e].id);
		printf("(%d, %d), ", s->enemiesSt[e].x, s->enemiesSt[e].y);
		printf("Hp/maxHp:%d/%d, ", s->enemiesSt[e].hp, s->enemiesSt[e].mhp);
		printf("point:%d, ", s->enemiesSt[e].point);
		printf("\n");
	}
}

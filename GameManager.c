#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "Rule.h"
#include "State.h"
#include "Player.h"


//#define DEBUG

// 1ゲームの実行，戻り値が勝敗
int GameManager_run(void);

// 出力
void GameManager_outputMap(State* s, int n);
void GameManager_outputPlayerInfo(State* s);
void GameManager_outputEnemiesInfo(State* s);


int main(void)
{
	int result = GameManager_run(); // 1試行の実行

	if (result == GAME_CLEAR) {
		printf("GAME CLEAR!!\n");
	}
	else if (result == GAME_OVER) {
		printf("GAME OVER...\n");
	}
	else {
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
	// 現在のStateの初期化（動的確保を含む）
	State* cState = Rule_init((unsigned int)time(NULL));
	//sState* cState = Rule_init(0);
	State* nState; // 次状態のStateポインタを用意
	State* cStateHidden; // 一部の情報が隠されたState

	// cStateの状態をコピー済み
	cStateHidden = Rule_getCurrentStateHidden();

	// テスト出力用
	GameManager_outputMap(cState, 0);
	//GameManager_outputMap(cState, 1);
	//GameManager_outputMap(cState, 2);
	GameManager_outputPlayerInfo(cState);
	GameManager_outputEnemiesInfo(cState);

	///////////////
	// main loop //
	///////////////
	while (TRUE) {
		// Stateそのままでなく，クローンを扱う必要あり
		// cStateの情報を一部隠したplayer用のState


		// stateファイル出力, player読み込み？
		// 要検討


		int act = Player_decideAction(cStateHidden);
		// 下の関数内でcStateHiddenへのコピーを行っている
		nState = Rule_getNextState(cState, act);
		
		/*GameManager_outputMap(cState, 0);
		_getch();

		GameManager_outputMap(nState, 0);
		_getch();*/
		
		cState = nState; // ポインタ付け替え
		
		GameManager_outputMap(cState, 0);
		//GameManager_outputMap(cState, 1);
		//GameManager_outputMap(cState, 2);
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

	#ifdef DEBUG
	printf("Game end\n");
	#endif // DEBUG

	int result = cState->gameFlag;
	Rule_destroy(); // 動的確保配列の解放
	return result;
}

void GameManager_outputMap(State* s, int n) {
	int mx = Rule_getMapSizeX();
	int my = Rule_getMapSizeY();

	if (n == 0) {
		system("cls");
		//gotoxy(1,1); // conio.hの関数，windows環境では使えない？
		
		printf("get item(food)\n");

		for (int y = 0; y < my; y++) {
			for (int x = 0; x < mx; x++) {
				if (s->seem[y][x] == 0) {
					printf("_ ");
					continue;
				}

				// 描画の優先度に注意
				if (s->y == y && s->x == x) {
					printf("@ ");
					continue;
				}
				else if (s->enemies[y][x] != -1 && s->seem[y][x] == 2) {
					printf("$ ");
					continue;
				}

				if (s->items[y][x] != -1) {
					printf("%c ", Rule_getItemIcon(s->items[y][x]));
					continue;
				}

				if (s->map[y][x] == 2) {
					printf("%% "); // %%で%出力
					continue;
				}

				if (s->map[y][x] == 0) {
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
	else if (n == 1) {
		for (int y = 0; y < my; y++) {
			for (int x = 0; x < mx; x++) {
				printf("%2d", s->enemies[y][x]);
			}
			printf("\n");
		}
		printf("\n");
	}
	else if (n == 2) {
		for (int y = 0; y < my; y++) {
			for (int x = 0; x < mx; x++) {
				printf("%2d", s->items[y][x]);
			}
			printf("\n");
		}
		printf("\n");
	}
}

void GameManager_outputPlayerInfo(State* s) {
	printf("%dF, ", s->flrNum+1);
	printf("turn:%d, ", s->gameTurn);
	printf("(%d, %d)", s->x, s->y);
	printf("\n");
	
	printf("Hp/maxHp:%d/%d, ", s->hp, s->mhp);
	printf("stm:%d, ", s->stm);
	printf("lv:%d, ", s->lv);
	//printf("exp:%d, ", s->exp);
	//printf("lvupexpsum:%d, ", s->lvupExpSum);
	printf("\n");

	for (int i = 0; i < Rule_getInvSize(); i++) {
		if (i == Rule_getInvSize() / 2) {
			printf("\n");
		}
		if (s->inv[i].itemID != -1) {
			printf("%d:[%c(%d)], ", i, Rule_getItemIcon(s->inv[i].itemID), s->inv[i].usageCount);
		}
	}
	printf("\n");
}

void GameManager_outputEnemiesInfo(State* s) {
	int en = Rule_getEnemyNum();
	for (int e = 0; e < en; e++) {
		printf("%d : ", s->enemiesSt[e].id);
		printf("(%d, %d), ", s->enemiesSt[e].x, s->enemiesSt[e].y);
		printf("Hp/maxHp:%d/%d, ", s->enemiesSt[e].hp, s->enemiesSt[e].mhp);
		printf("point:%d, ", s->enemiesSt[e].point);
		printf("ket:%d, ", s->enemiesSt[e].killedEnemyTurn);
		printf("\n");
	}
}

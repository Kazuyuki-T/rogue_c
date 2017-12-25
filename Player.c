#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "Player.h"

int Player_decideAction(State* s) {
	// キー入力

	int act;
	// 適切な文字が入力されるまでループ
	while (TRUE) {
		printf("\r"); // カーソルを先頭へ

		act = _getch(); // キー入力待ち
		if (act == '1') {
			break;
		}
		else if (act == '2') {
			break;
		}
		else if (act == '3') {
			break;
		}
		else if (act == '4') {
			break;
		}
		else if (act == '5') {
			break;
		}
		else if (act == '6') {
			break;
		}
		else if (act == '7') {
			break;
		}
		else if (act == '8') {
			break;
		}
		else if (act == '9') {
			break;
		}
		// arrow
		else if (act == 'a') {
			break;
		}
		// staff
		else if (act == 's') {
			break;
		}
		// potion
		else if (act == 'p') {
			break;
		}
		// food
		else if (act == 'f') {
			break;
		}
		// esc
		else if (act == 0x1B) {
			break;
		}
		// <
		else if (act == 0x3c) {
			break;
		}
		// >
		else if (act == 0x3e) {
			break;
		}
		// 方向キーによる入力，ほかのキーと異なり特殊
		// 方向キー入力 -> キー取得 -> どの方向キーでも0xe0
		// もう一度キー取得 -> いずれかの方向のキーコード
		else if (act == 0xe0) {
			int dflag = FALSE;
			act = _getch();
			switch (act) {
			case 0x48: // 上
			case 0x50: // 下
			case 0x4b: // 左
			case 0x4d: // 右
				dflag = TRUE;
				break;
			default:
				break;
			}

			if (dflag == TRUE) {
				break;
			}
		}
		else {
			printf("入力できません   ");
		}
	}
	//printf("%d", act);
	return act;
}

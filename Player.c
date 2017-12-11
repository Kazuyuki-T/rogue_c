#include <stdio.h>
#include <stdlib.h>
#include "Player.h"

#define TRUE 1
#define FALSE 0

void Player_init(Player *thisPlayer) {
	thisPlayer->testPlayer = 0;
}

void Player_finish(Player *thisPlayer) {

}

int Player_getAction(Player *thisPlayer, State *currentState) {
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
		else if (act == 0x1B) {
			break;
			//exit(1);
		}
		else {
			printf("入力できません   ");
		}
	}
	
	return act;
}
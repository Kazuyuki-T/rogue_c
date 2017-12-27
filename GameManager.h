#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H


#include <stdio.h>
#include "Rule.h"
#include "State.h"
#include "Player.h"

// 1ゲームの実行，戻り値が勝敗
int GameManager_run(void);

// 出力
void GameManager_outputMap(State* s);


void GameManager_outputPlayerInfo(State* s);


void GameManager_outputEnemiesInfo(State* s);


#endif
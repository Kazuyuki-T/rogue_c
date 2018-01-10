#ifndef RULE_H
#define RULE_H


#include <stdio.h>
#include "State.h"

#define MAPSIZEX 10
#define MAPSIZEY 10
#define ENEMY_NUMBER 4
#define ENEMY_REVTURN 50
#define ENEMY_MAXHP 50
#define ENEMY_POINT 100
#define TOPFLR 4
#define PLAYER_MAXHP 100
#define PLAYER_STM 100

// 初期化関数
State* Rule_init(void);

// 終了関数，動的確保配列の解放
void Rule_destroy(void);

// 状態+行動->新しい状態
State* Rule_getNextState(State* s, int act);


#endif
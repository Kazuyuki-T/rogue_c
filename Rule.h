#ifndef RULE_H
#define RULE_H


#include <stdio.h>
#include "State.h"

// 初期化関数
State* Rule_init(void);

// 終了関数，動的確保配列の解放
void Rule_destroy(void);

// 状態+行動->新しい状態
State* Rule_getNextState(State* s, int act);

// GMでの情報出力に必要，今後の修正により消去？
int Rule_getMapSizeX(void);
int Rule_getMapSizeY(void);
int Rule_getEnemyNum(void);


#endif
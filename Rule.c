#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "Rule.h"


// 定数
// #define, static const どっち？
#define MAPSIZEX 20
#define MAPSIZEY 20
//static const int MAPSIZEX = 20;
//static const int MAPSIZEY = 20;
static const int ENEMY_NUMBER = 2;
static const int ENEMY_REVTURN = 50;
static const int ENEMY_MAXHP = 50;
static const int ENEMY_POINT = 500;
static const int TOPFLR = 4;
static const int PLAYER_MAXHP = 100;
static const int PLAYER_STM = 100;
static const double PLAYER_AUTOHEALCOEF = 0.005; // -> 1/200
static const int PLAYER_INITLVUPEXP = 100; // 初期のレベルアップ必要経験値
static const double PLAYER_LVUPCOEF = 1.1; // レベルアップ必要経験値の係数
static const int PLAYER_INVENTORYSIZE = 10;
static const int ITEM_NUMBER = 10;
static const int PLAYER_STM_DICREASE_TURN = 10; // 10T1STM
static const int PLAYER_VIEW = 4;

// 周囲8方向から求めるxy差分
static const int diffX[9] = { -1, 0, 1,-1, 0, 1,-1, 0, 1 };
static const int diffY[9] = {  1, 1, 1, 0, 0, 0,-1,-1,-1 };
// xy差分から求める周囲8方向
// 自身はdirNum[1][1]=4の位置
// 使用時には差分 -1~1 -> 0~2 に変換
static const int dirNum[3][3] = { {6, 7, 8}, {3, 4, 5}, {0 ,1, 2} };

State currentState;
State nextState;
State currentStateHidden;


// そもそも，すべての引数にState必要ある？
// どうせこっちにStateの大本持ってるんだからいらなくない？
// どっちがわかりやすいか？
// 引数に用いたほうがわかりやすいか
// Ruleに持たせず，GM側でState本体を持てば？
// Ruleで初期化


typedef struct {
	int id;
	int type; // 0:food, 1;staff, 2:arrow
	char name[256];
	char icon;
	int maxUsageCount;
	int healHPval;
	int healSTval;
	int damHPval;
	int damSTval;
	int effect; // 0:nothing, 1:warp
} ItemList;

const ItemList itemList[4] = {
	{0, 0, "potion", 'p', 1, 10, 0, 0, 0, 0},
	{1, 0, "food",	 'f', 1, 0, 10, 0, 0, 0},
	{2, 1, "staff",	 's', 1, 0, 0, 0, 0, 0},
	{3, 2, "arrow",  'a', 3, 0, 0, 10, 0, 0}
};

typedef struct {
	int map[MAPSIZEY][MAPSIZEX];
	int squareNum;
	int roomNum;
	int room[8][2]; // {leftTop, rightBottom} y*mapSizeX+x
} MapType;

MapType mt; 
// 状態のセット等に用いる
// copy!!

//// maptypeその１
//MapType m1 = { 
//	{{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
//	1,
//	1,
//	{ { 51,  1448} }
//};
//// maptypeその２
//MapType m2 = {
//	{{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
//	 { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } },
//	8,
//	4,
//	{ { 210, 619 },{ 230, 639 },{ 910, 1319 },{ 930, 1339 },
//	  { 270, 279 },{ 1270, 1279 },{ 661, 861 },{ 688, 888 } }
//};

// maptypeその３
MapType m3 = {
	{{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	 { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	 { 1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1 },
	 { 1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
	 { 1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1 },
	 { 1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1 },
	 { 1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1 },
	 { 1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1 },
	 { 1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1 },
	 { 1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1 },
	 { 1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1 },
	 { 1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1 },
	 { 1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1 },
	 { 1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1 },
	 { 1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1 },
	 { 1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1 },
	 { 1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
	 { 1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1 }, 
	 { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	 { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } },
	8,
	4,
	{ { 42, 126 },{ 53, 137 },{ 262, 346 },{ 273, 357 },
	{ 67, 72 },{ 143, 243 },{ 156, 256 },{ 327, 332 } }
};



// 通路の数が変更あるときどうすんの？
// 




// 配列の動的確保
void Rule_makeArrays(State* s);
void Rule_reserveMapArray(int ***mapArray, int lengthX, int lengthY, int initVal);
void Rule_reserveEnemyArray(Enemy **enemyStArray, int enemyLength);
// 配列の解放
void Rule_removeArrays(State* s);
void Rule_freeMapArray(int ***mapArray, int lengthY);
void Rule_freeEnemyArray(Enemy **enemyStArray);
// Stateの情報のセット，map，obj配置
void Rule_setStateInfo(State *s, int initFlag);
void Rule_setMap(State *s);
void Rule_setStair(State *s, int* gridnum);
void Rule_setItems(State *s, int* gridnum);
void Rule_setEnemies(State *s, int* gridnum);
void Rule_setEachEnemy(State *s, int* gridnum, int en);
void Rule_setPlayer(State *s, int* gridnum);
// プレイヤの初期化
void Rule_initPlayer(State *s);
// 状態遷移の流れ
void Rule_transitState(State *s, int act);
// プレイヤの行動判定
int Rule_canActPlayer(State *s, int act);
// 敵との衝突判定
int Rule_getOrDefaultCollidedEnemyIndex(State *s, int nx, int ny);
// プレイヤの攻撃
void Rule_calcDamageFromPlayer(State *s, int en, int atkDamage);
// 敵の行動
void Rule_actEnemies(State *s);
void Rule_actEachEnemy(State *s, int en);
// 敵の配置の更新
void Rule_updateEnemyMap(State *s);
// プレイヤの視界の更新
void Rule_updateSeemArea(State *s);
// act1~9 -> dir0~8
int Rule_convertActtoDir(int act);
// コピー，csの内容をnsにコピー
void Rule_copyState(State* cs, State* ns);
// min<=randval<=max
int Rule_getRandom(int min, int max);



int Rule_hasPlayerPosition(State *s, int en);
int Rule_canAttackPlayer(State *s, int en);

int Rule_canMoveUnit(State *s, int nx, int ny);

int Rule_convRangeM1to1(int);

void Rule_levelupIfNeeded(State* s);

void Rule_setEachItem(State *s, int* gridnum, int in);

void Rule_reserveInventoryArray(Inventory **invStArray, int invLength);
void Rule_freeInventoryArray(Inventory **invStArray);

int Rule_canAddInvItem(Inventory *inv, int id);
int Rule_getInvItemNum(Inventory *inv);

int Rule_getOrDefaultInvArrayIndex(Inventory *inv, int id);

void Rule_decreaseInvItemUsageCount(Inventory *inv, int itemIndex);

void Rule_doItemEffect(State *s, int en, int effectIndex);


int Rule_getPosRoom(State *s, int roomID);

int Rule_convertOneDimValtoX(int oneDimVal);

int Rule_convertOneDimValtoY(int oneDimVal);

int Rule_convertXYtoOneDimVal(int x, int y);

int Rule_getOrDefaultPlayerRoomID(State *s);




State* Rule_init(unsigned int seed) {
	srand(seed);

	// 配列の動的確保
	Rule_makeArrays(&currentState);
	Rule_makeArrays(&nextState);
	Rule_makeArrays(&currentStateHidden);

	// 初期盤面の作成
	Rule_setStateInfo(&currentState, TRUE);

	// プレイヤ用
	Rule_copyState(&currentState, &currentStateHidden);

	return &currentState;
}

void Rule_destroy(void) {
	Rule_removeArrays(&currentState);
	Rule_removeArrays(&nextState);
	Rule_removeArrays(&currentStateHidden);
}

void Rule_makeArrays(State* s) {
	// 配列の動的確保
	Rule_reserveMapArray(&(s->map), MAPSIZEX, MAPSIZEY, -1);
	Rule_reserveMapArray(&(s->seem), MAPSIZEX, MAPSIZEY, 0);
	Rule_reserveMapArray(&(s->enemies), MAPSIZEX, MAPSIZEY, -1);
	Rule_reserveMapArray(&(s->items), MAPSIZEX, MAPSIZEY, -1);
	Rule_reserveEnemyArray(&(s->enemiesSt), ENEMY_NUMBER);
	Rule_reserveInventoryArray(&(s->inv), PLAYER_INVENTORYSIZE);
}

void Rule_reserveMapArray(int ***mapArray, int lengthX, int lengthY, int initVal) {
	*mapArray = (int**)malloc(sizeof(int) * lengthY);
	if (*mapArray == NULL) exit(1);

	for (int y = 0; y < lengthY; y++) {
		(*mapArray)[y] = (int*)malloc(sizeof(int) * lengthX);
		if ((*mapArray)[y] == NULL) exit(1);
	}

	// initValによる初期化
	for (int y = 0; y < lengthY; y++) {
		for (int x = 0; x < lengthX; x++) {
			(*mapArray)[y][x] = initVal;
		}
	}
}

void Rule_reserveEnemyArray(Enemy **enemyStArray, int enemyLength) {
	*enemyStArray = (Enemy*)malloc(sizeof(Enemy) * enemyLength);
	if (*enemyStArray == NULL) exit(1);

	// 情報の初期化
	for (int e = 0; e < enemyLength; e++) {
		(*enemyStArray)[e].id = e;
		(*enemyStArray)[e].active = FALSE;
	}
}

void Rule_reserveInventoryArray(Inventory **invStArray, int invLength) {
	*invStArray = (Inventory*)malloc(sizeof(Inventory) * invLength);
	if (*invStArray == NULL) exit(1);

	// 情報の初期化
	for (int i = 0; i < invLength; i++) {
		(*invStArray)[i].itemID = -1;
		(*invStArray)[i].usageCount = -1;
	}
}

void Rule_removeArrays(State* s) {
	// 動的確保した配列の解放
	Rule_freeMapArray(&(s->map), MAPSIZEY);
	Rule_freeMapArray(&(s->seem), MAPSIZEY);
	Rule_freeMapArray(&(s->enemies), MAPSIZEY);
	Rule_freeMapArray(&(s->items), MAPSIZEY);
	Rule_freeEnemyArray(&(s->enemiesSt));
	Rule_freeInventoryArray(&(s->inv));
}

void Rule_freeMapArray(int ***mapArray, int lengthY) {
	for (int y = 0; y < lengthY; y++) {
		free((*mapArray)[y]);
	}
	free(*mapArray);
}

void Rule_freeEnemyArray(Enemy **enemyStArray) {
	free(*enemyStArray);
}

void Rule_freeInventoryArray(Inventory **invStArray) {
	free(*invStArray);
}

void Rule_setStateInfo(State *s, int playerInitFalg) {
	// obj配置できる床の数
	// この部分は連結リストで用意し，必要に応じて追加・削除したほうが良いか？
	// 最大数（マップのサイズ）が判明しているため，カーソルによる線形リストでも良いかもしれない

	

	
	// どのmapテンプレを用いるか

	// ファイル読み込み
	// 








	//mt = m1;
	//for (int sn = 0; sn < m1.squareNum; sn++) {
	//	// 複数要素に対応してるの？
	//	mt.room[sn][0] = m1.room[sn][0];
	//	mt.room[sn][1] = m1.room[sn][1];
	//}
	mt = m3;
	for (int sn = 0; sn < m3.squareNum; sn++) {
		// 複数要素に対応してるの？
		mt.room[sn][0] = m3.room[sn][0];
		mt.room[sn][1] = m3.room[sn][1];
	}
	// 通路カットの有無

	// もろもろ確認
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			printf("%d ", mt.map[y][x]);
		}
		printf("\n");
	}
	printf("\n");
	printf("sNum:%d\n", mt.squareNum);
	printf("rNum:%d\n", mt.roomNum);
	for (int rn = 0; rn < mt.roomNum; rn++) {
		printf("(%d, %d)\n", mt.room[rn][0], mt.room[rn][1]);
	}
	_getch();


	int *randArray;
	randArray = (int*)malloc(sizeof(int) * mt.roomNum);
	for (int rn = 0; rn < mt.roomNum; rn++) {
		randArray[rn] = rn;
	}
	//　番号シャッフル
	

	// 各mapの初期化
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			s->seem[y][x] = 0;
			s->enemies[y][x] = -1;
			s->items[y][x] = -1;
		}
	}

	// マップの作製->obj配置できる床の数
	Rule_setMap(s);
	// 階段
	Rule_setStair(s, randArray);
	// プレイヤ
	Rule_setPlayer(s, randArray);
	// プレイヤの初期化を行うか否か
	// ゲームの初期化の際には行う
	// フロアの変更の際には行わない
	if (playerInitFalg == TRUE) {
		Rule_initPlayer(s);
	}
	// 敵
	Rule_setEnemies(s, randArray);
	// アイテム
	Rule_setItems(s, randArray);

	// map上の敵の座標を更新
	Rule_updateEnemyMap(s);

	// プレイヤから見えている範囲を更新
	Rule_updateSeemArea(s);

	// フラグを更新
	s->flrResetFlag = FALSE;

	free(randArray);
}

void Rule_setMap(State *s) {
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			s->map[y][x] = mt.map[y][x];
		}
	}
}

void Rule_setStair(State *s, int *randArray) {
	// 部屋番号のランダム選択
	int roomID = Rule_getRandom(0, mt.roomNum - 1);
	// 部屋の中の座標決定
	int setPos = Rule_getPosRoom(s, roomID);
	int setPosX = Rule_convertOneDimValtoX(setPos);
	int setPosY = Rule_convertOneDimValtoY(setPos);
	s->map[setPosY][setPosX] = 2;
}

int Rule_getPosRoom(State *s, int roomID) {
	int leftTopX = Rule_convertOneDimValtoX(mt.room[roomID][0]);
	int leftTopY = Rule_convertOneDimValtoY(mt.room[roomID][0]);
	int rightBottomX = Rule_convertOneDimValtoX(mt.room[roomID][1]);
	int rightBottomY = Rule_convertOneDimValtoY(mt.room[roomID][1]);

	int count = 0;
	for (int y = leftTopY; y < rightBottomY; y++) {
		for (int x = leftTopX; x < rightBottomX; x++) {
			// 配置可能なマスをカウント
			// 通路隣接除く
			if (s->map[y][x] == 0 && (s->x != x || s->y != y) && s->enemies[y][x] == -1) {
				count++;
			}
		}
	}
	
	// count数から座標を決定
	int randNum = Rule_getRandom(1, count); // １マス目～
	count = 0;
	int setPos = -1;
	for (int y = leftTopY; y < rightBottomY; y++) {
		for (int x = leftTopX; x < rightBottomX; x++) {
			// 配置可能なとき
			if (s->map[y][x] == 0 && (s->x != x || s->y != y) && s->enemies[y][x] == -1) {
				count++;
			}

			// 配置場所の時
			if (randNum == count) {
				setPos = Rule_convertXYtoOneDimVal(x, y);
				return setPos;
			}
		}
	}

	return setPos;
}

int Rule_convertOneDimValtoX(int oneDimVal) {
	return oneDimVal % MAPSIZEX;
}

int Rule_convertOneDimValtoY(int oneDimVal) {
	return oneDimVal / MAPSIZEX;
}

int Rule_convertXYtoOneDimVal(int x, int y) {
	return y * MAPSIZEX + x;
}

void Rule_setItems(State *s, int *randArray) {
	// アイテムマップの初期化
	/*for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			s->items[y][x] = -1;
		}
	}*/
	
	// 順番に生成
	for (int in = 0; in < ITEM_NUMBER; in++) {
		Rule_setEachItem(s, randArray, in);
	}
}

void Rule_setEachItem(State *s, int *randArray, int in) {
	// 部屋番号のランダム選択
	int roomID = Rule_getRandom(0, mt.roomNum - 1);
	// 部屋の中の座標決定
	int setPos = Rule_getPosRoom(s, roomID);
	int setPosX = Rule_convertOneDimValtoX(setPos);
	int setPosY = Rule_convertOneDimValtoY(setPos);

	// アイテムセット
	int randItem = Rule_getRandom(0, 3);
	s->items[setPosY][setPosX] = randItem;
	return;
}

void Rule_setEnemies(State *s, int *randArray) {
	// エネミーマップの初期化
	/*for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			s->enemies[y][x] = -1;
		}
	}*/
	
	// 順番に生成
	for (int en = 0; en < ENEMY_NUMBER; en++) {
		Rule_setEachEnemy(s, randArray, en);
	}
}

void Rule_setEachEnemy(State *s, int *randArray, int en) {
	// 部屋番号のランダム選択
	int roomID = Rule_getRandom(0, mt.roomNum - 1);

	printf("enemy roomid:%d\n", roomID);

	// 部屋の中の座標決定
	int setPos = Rule_getPosRoom(s, roomID);
	int setPosX = Rule_convertOneDimValtoX(setPos);
	int setPosY = Rule_convertOneDimValtoY(setPos);

	// 敵の初期座標の格納
	s->enemies[setPosY][setPosX] = s->enemiesSt[en].id;
	s->enemiesSt[en].x = setPosX;
	s->enemiesSt[en].y = setPosY;
	s->enemiesSt[en].active = TRUE;
	s->enemiesSt[en].mhp = ENEMY_MAXHP;
	s->enemiesSt[en].hp = s->enemiesSt[en].mhp;
	s->enemiesSt[en].point = ENEMY_POINT;
	s->enemiesSt[en].killedEnemyTurn = 0;
	return;
}

void Rule_setPlayer(State *s, int *randArray) {
	// 部屋番号のランダム選択
	int roomID = Rule_getRandom(0, mt.roomNum - 1);
	// 部屋の中の座標決定
	int setPos = Rule_getPosRoom(s, roomID);
	int setPosX = Rule_convertOneDimValtoX(setPos);
	int setPosY = Rule_convertOneDimValtoY(setPos);

	// プレイヤの初期座標の格納
	s->x = setPosX;
	s->y = setPosY;
	return;
}

void Rule_initPlayer(State *s) {
	s->mhp = PLAYER_MAXHP;
	s->hp = s->mhp;
	s->stm = PLAYER_STM;
	s->lv = 1;
	s->exp = 0;
	s->autoHealVal = 0.0;
	s->stmDicTurnCount = PLAYER_STM_DICREASE_TURN;

	s->lvupExp = PLAYER_INITLVUPEXP;
	s->lvupExpSum = s->lvupExp;

	s->view = PLAYER_VIEW;
}

State* Rule_getCurrentStateHidden(void) {
	return &currentStateHidden;
}

State* Rule_getNextState(State* s, int act) {
	// 状態 + 行動 -> 新しい状態

	/*for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			printf("%d ", nextState.map[y][x]);
		}
		printf("\n");
	}
	printf("\n");
	_getch();*/

	// Stateの複製
	Rule_copyState(s, &nextState);

	// Stateの更新
	Rule_transitState(&nextState, act);
	
	// プレイヤ用
	Rule_copyState(&nextState, &currentStateHidden);

	return &nextState;
}

void Rule_transitState(State *s, int act) {
	// 遷移

	// escのとき
	if (act == 0x1b) {
		s->gameFlag = GAME_PLAYING;
	}
	else {
		int playerAction = Rule_canActPlayer(s, act);
		if (playerAction == TRUE) {
			// プレイヤが行動したならば，敵を動かす

			// enemy
			Rule_actEnemies(s);

			// ここで行うべきか？別の部分でまとめて行うべきでは？
			// map上の敵の座標を更新
			Rule_updateEnemyMap(s);
			// プレイヤから見えている範囲を更新
			Rule_updateSeemArea(s);

			(s->gameTurn)++; // ターン経過処理

			// 敵の攻撃で死んだとき，スタミナ・自動回復処理をスキップ
			if (s->hp == 0) {
				s->gameFlag = GAME_OVER;
				return;
			}

			// stmの減少
			if (s->stm > 0) {
				s->stmDicTurnCount--;
				if (s->stmDicTurnCount == 0) {
					s->stm--;
					s->stmDicTurnCount = PLAYER_STM_DICREASE_TURN;
				}

				// ターン経過による自動回復
				s->autoHealVal += (s->hp * PLAYER_AUTOHEALCOEF);
				if (s->autoHealVal >= 1.0) {
					if (s->hp < PLAYER_MAXHP) {
						s->hp += 1;
					}
					s->autoHealVal -= 1.0;
				}
			}
			else {
				// stm0によるhp減少
				if (s->hp > 0) {
					s->hp--;
				}
			}

			if (s->hp == 0) {
				s->gameFlag = GAME_OVER;
				return;
			}
			else {
				s->gameFlag = GAME_PLAYING;
			}
		}
		else{
			// プレイヤが行動しないとき
			s->gameFlag = GAME_PLAYING;
		}
		
		// フロアの更新チェック
		if (s->flrResetFlag == TRUE) {
			// プレイヤの階層が変化したとき
			s->flrNum += 1;
			if (s->flrNum == TOPFLR) {
				// 最上階到達 -> ゲームクリア
				s->gameFlag = GAME_CLEAR;
			}
			else {
				// 途中 -> 新しい状況の生成
				Rule_setStateInfo(s, FALSE);
				(s->gameTurn)++;
				s->gameFlag = GAME_PLAYING;
			}
		}
	}
}

int Rule_canActPlayer(State *s, int act) {
	// a:Arrow or a:Staff -> 数字入力
	// 1~9 or f:Food or p:Potion -> 直接行動
	// ↑↓←→ -> 直接行動

	//printf("%d, %d\n", m1.room[0][0], m1.room[0][1]);
	//printf("%d, %d\n", m1.room[1][0], m1.room[1][1]);

	//_getch();

	// 矢
	if (act == 'a') {
		// インベントリに存在するか否か
		int invIndex = Rule_getOrDefaultInvArrayIndex(s->inv, 3);
		if (invIndex != -1) {
			printf("                                                 \r");
			printf("direction : "); // 方向の入力待ち

			int actItem = _getch();
			if (actItem == 0 || actItem == 224)	actItem = _getch();
			int dir = Rule_convertActtoDir(actItem);
			// -1のとき：0-8の方向に当てはまらない
			if (dir != -1) {
				printf("                                                 \r");
				printf("use arrrow %d", dir);

				// 敵がいるときダメージ処理
				int targetEnemyIndex = -1;
				int nx, ny;
				for (int distance = 1; distance <= 4; distance++) {
					nx = diffX[dir] * distance + s->x;
					ny = diffY[dir] * distance + s->y;
					// (nx,ny)が壁に衝突したとき
					if (s->map[ny][nx] == 1)	break;
					targetEnemyIndex = Rule_getOrDefaultCollidedEnemyIndex(s, nx, ny);
					// 敵に衝突したとき
					if (targetEnemyIndex != -1)	break;
				}
				if (targetEnemyIndex != -1) {
					// ダメージ処理
					Rule_calcDamageFromPlayer(s, targetEnemyIndex, itemList[3].damHPval);
					// effect処理（追加効果）
					if (itemList[3].effect != 0) {
						Rule_doItemEffect(s, targetEnemyIndex, itemList[3].effect);
					}
				}

				// 使用したインデックスのアイテム使用回数を減少
				Rule_decreaseInvItemUsageCount(s->inv, invIndex);
			}
			else {
				printf("                                                 \r");
				printf("miss, ");
				return FALSE;
			}
		}
		else {
			printf("                                                 \r");
			printf("No Item-arrow, ");
			return FALSE;
		}
	}
	// 杖
	else if (act == 's') {
		// インベントリに存在するか否か
		int invIndex = Rule_getOrDefaultInvArrayIndex(s->inv, 2);

		if (invIndex != -1) {
			printf("                                                 \r");
			printf("direction : "); // 方向の入力待ち

			int actItem = _getch();
			if (actItem == 0 || actItem == 224)	actItem = _getch();
			int dir = Rule_convertActtoDir(actItem);
			// -1のとき：0-8の方向に当てはまらない
			if (dir != -1) {
				printf("                                                 \r");
				printf("use staff %d", dir);

				// 敵がいるとき，ダメージ処理
				// 敵がいるときダメージ処理
				int targetEnemyIndex = -1;
				int nx, ny;
				for (int distance = 1; distance <= 4; distance++) {
					nx = diffX[dir] * distance + s->x;
					ny = diffY[dir] * distance + s->y;
					// (nx,ny)が壁に衝突したとき
					if (s->map[ny][nx] == 1)	break;
					targetEnemyIndex = Rule_getOrDefaultCollidedEnemyIndex(s, nx, ny);
					// 敵に衝突したとき
					if (targetEnemyIndex != -1)	break;
				}
				if (targetEnemyIndex != -1) {
					// ダメージ処理
					Rule_calcDamageFromPlayer(s, targetEnemyIndex, itemList[2].damHPval);
					// effect処理（追加効果）
					if (itemList[3].effect != 0) {
						Rule_doItemEffect(s, targetEnemyIndex, itemList[2].effect);
					}
				}

				// 使用したインデックスのアイテム使用回数を減少
				Rule_decreaseInvItemUsageCount(s->inv, invIndex);
			}
			else {
				printf("                                                 \r");
				printf("miss, ");
				return FALSE;
			}
		}
		else {
			printf("                                                 \r");
			printf("No Item-staff, ");
			return FALSE;
		}
	}
	// 食料
	else if (act == 'f') {
		// インベントリに存在するか否か
		int invIndex = Rule_getOrDefaultInvArrayIndex(s->inv, 1);

		if (invIndex != -1) {
			printf("                                                 \r");
			printf("use food");

			s->hp += itemList[1].healHPval;
			if (s->hp > PLAYER_MAXHP) {
				s->hp = PLAYER_MAXHP;
			}
			s->stm += itemList[1].healSTval;
			if (s->stm > PLAYER_STM) {
				s->stm = PLAYER_STM;
			}

			// 使用したインデックスのアイテム使用回数を減少
			Rule_decreaseInvItemUsageCount(s->inv, invIndex);
		}
		else {
			printf("                                                 \r");
			printf("No Item-food, ");
			return FALSE;
		}
	}
	//  ポーション
	else if (act == 'p') {
		// インベントリに存在するか否か
		int invIndex = Rule_getOrDefaultInvArrayIndex(s->inv, 0);

		if (invIndex != -1) {
			printf("                                                 \r");
			printf("use potion");

			s->hp += itemList[0].healHPval;
			s->stm += itemList[0].healSTval;

			// 使用したインデックスのアイテム使用回数を減少
			Rule_decreaseInvItemUsageCount(s->inv, invIndex);
		}
		else {
			printf("                                                 \r");
			printf("No Item-potion, ");
			return FALSE;
		}
	}
	// 階段降りるアクション，いらない？
	else if (act == 0x3c || act == 0x3e) {
		// もし階段が足元にあるのならば
	}
	// その他，移動・攻撃・待機
	else {
		// act1~9 -> dir0~8
		int dir = Rule_convertActtoDir(act);

		int nx = s->x + diffX[dir];
		int ny = s->y + diffY[dir];

		// State, xy座標
		if (Rule_canMoveUnit(s, nx, ny) == FALSE) {
			return FALSE;
		}

		// いずれにも当てはまらない->行動成功
		// 攻撃or待機or移動

		// 敵との衝突判定
		int nextEnemyIndex = Rule_getOrDefaultCollidedEnemyIndex(s, nx, ny);
		if (nextEnemyIndex != -1) {
			// 斜め攻撃判定 -> return


			Rule_calcDamageFromPlayer(s, nextEnemyIndex, 10);
			printf("                                                 \r");
			printf("atk.");
			/*for (int en = 0; en < thisRule->enemyNumber; en++) {
			printf("%d:%d-%d, ", en, currentState->enemiesSt[en].hp, currentState->enemiesSt[en].active);
			}*/
		}
		else {
			s->x += diffX[dir];
			s->y += diffY[dir];

			// 移動後の座標にアイテムがある場合
			if (s->items[s->y][s->x] != -1) {
				// アイテム追加
				// インベントリに余裕があるとき
				if (Rule_canAddInvItem(s->inv, s->items[s->y][s->x]) == TRUE) {
					s->items[s->y][s->x] = -1;
				}
			}

			// 移動後の座標が階段だった場合
			if (s->map[s->y][s->x] == 2) {
				s->flrResetFlag = TRUE;
			}
		}
	}

	return TRUE;
}

void Rule_doItemEffect(State *s, int en, int effectIndex) {
	if (effectIndex == 1) {
		// warp処理

	}
	else {
		// その他の処理
	}
}

int Rule_getOrDefaultInvArrayIndex(Inventory *inv, int id) {
	for (int i = 0; i < PLAYER_INVENTORYSIZE; i++) {
		if (inv[i].itemID == id) {
			return i;
		}
	}
	return -1;
}

int Rule_canAddInvItem(Inventory *inv, int id) {
	// 追加できるかできないか
	int invnum = Rule_getInvItemNum(inv);
	if (invnum < PLAYER_INVENTORYSIZE) {
		inv[invnum].itemID = id;
		inv[invnum].usageCount = itemList[id].maxUsageCount;
		return TRUE;
	}
	else{
		return FALSE;
	}
}

void Rule_decreaseInvItemUsageCount(Inventory *inv, int itemIndex) {
	// アイテム使用回数の減少
	inv[itemIndex].usageCount--;
	// 減少後0になったものは消去->配列の要素を詰める
	if (inv[itemIndex].usageCount == 0) {
		for (int i = itemIndex; i < PLAYER_INVENTORYSIZE - 1; i++) {
			inv[i].itemID = inv[i + 1].itemID;
			inv[i].usageCount = inv[i + 1].usageCount;
		}
		inv[PLAYER_INVENTORYSIZE - 1].itemID = -1;
		inv[PLAYER_INVENTORYSIZE - 1].usageCount = -1;
	}
}

int Rule_getInvItemNum(Inventory *inv) {
	for (int i = 0; i < PLAYER_INVENTORYSIZE; i++) {
		if (inv[i].itemID == -1) {
			return i;
		}
	}
	return PLAYER_INVENTORYSIZE;
}

int Rule_canMoveUnit(State *s, int nx, int ny) {
	// マップの範囲外
	if (ny < 0 || MAPSIZEY <= ny || nx < 0 || MAPSIZEX <= nx) {
		return FALSE;
	}
	// 進入禁止部分
	if (s->map[ny][nx] == 1) {
		return FALSE;
	}
	// 斜め移動チェック
	//if (斜め移動の時) {
	//	return FALSE;
	//}

	return TRUE;
}

// (State, unitNextX, unitNextY)
int Rule_getOrDefaultCollidedEnemyIndex(State *s, int nx, int ny) {
	for (int en = 0; en < ENEMY_NUMBER; en++) {
		if (s->enemiesSt[en].x == nx && s->enemiesSt[en].y == ny && s->enemiesSt[en].active == TRUE) {
			return en;
		}
	}
	return -1;
}

// (State, enemyNum)
int Rule_isCollidedPlayer(State *s, int en) {
	for(int dir = 0; dir < 9; dir++) {
		// 周囲８マスにいるか
		if (s->x == (s->enemiesSt[en].x + diffX[dir]) && s->y == (s->enemiesSt[en].y + diffY[dir])) {
			return TRUE;
		}
	}
	return FALSE;
}

void Rule_calcDamageFromPlayer(State *s, int en, int atkDamage) {
	if (s->enemiesSt[en].hp - atkDamage <= 0) {
		// 敵の死亡処理
		s->enemiesSt[en].hp = 0;
		s->enemiesSt[en].active = FALSE;

		// 敵の再復活までのターンを設定
		s->enemiesSt[en].killedEnemyTurn = ENEMY_REVTURN;

		// プレイヤに経験値を追加
		s->exp += s->enemiesSt[en].point;
		
		// レベルアップ判定
		Rule_levelupIfNeeded(s);

		printf("%d\n", s->enemiesSt[en].point);
	}
	else {
		s->enemiesSt[en].hp -= atkDamage;
	}
}

void Rule_levelupIfNeeded(State* s) {
	while (TRUE)
	{
		if (s->exp >= s->lvupExpSum) {
			s->lv++;
			s->lvupExp = (int)(s->lvupExp * PLAYER_LVUPCOEF);
			s->lvupExpSum += s->lvupExp;
		}
		else {
			break;
		}
	}
}

void Rule_actEnemies(State *s) {
	// 順番に行動
	for (int en = 0; en < ENEMY_NUMBER; en++) {
		Rule_actEachEnemy(s, en);
		// 敵の行動中にプレイヤが死んだとき
		if (s->hp == 0) {
			break;
		}
	}
}

int Rule_convRangeM1to1(int dn) {
	if (dn > 1) {
		dn = 1;
	}
	else if(dn < -1) {
		dn = -1;
	}

	return dn;
}

void Rule_calcDamageFromEnemy(State *s, int en, int atkDamage) {
	if (s->hp - atkDamage <= 0) {
		// プレイヤの死亡処理
		s->hp = 0;
	}
	else {
		s->hp -= atkDamage;
	}
}

void Rule_actEachEnemy(State *s, int en) {
	// 行動の選択

	// 生きているとき
	if (s->enemiesSt[en].active == TRUE) {
		// プレイヤを確認できる
		if (Rule_hasPlayerPosition(s, en) == TRUE) {
			// 攻撃できる
			if (Rule_canAttackPlayer(s, en) == TRUE) {
				Rule_calcDamageFromEnemy(s, en, 1);
			}
			// 攻撃できない->最短距離で移動
			else {
				// enemy mapの座標初期化
				s->enemies[s->enemiesSt[en].y][s->enemiesSt[en].x] = -1;
				
				int ndiffx = Rule_convRangeM1to1((s->x) - (s->enemiesSt[en].x));
				int ndiffy = Rule_convRangeM1to1((s->y) - (s->enemiesSt[en].y));

				// 差分から次の移動方向を決定
				// (x, y) = (cx, cy) + (nx, ny)
				int nx = (s->enemiesSt[en].x) + ndiffx;
				int ny = (s->enemiesSt[en].y) + ndiffy;

				// (x, y)に移動できる
					// 移動
				// (x, y)に移動できない
					// (x, y)の左右２方向の座標確認
					// 移動できる
						// 移動
					// 移動できない
						// その場で待機


				// 移動可能（mapの進行不可領域に侵入しない）で，敵同士の衝突なし
				if (Rule_canMoveUnit(s, nx, ny) == TRUE && Rule_getOrDefaultCollidedEnemyIndex(s, nx, ny) == -1) {
					s->enemiesSt[en].x = nx;
					s->enemiesSt[en].y = ny;
				}
				else {
					// 左右２方向確認

				}

				// map更新
				s->enemies[s->enemiesSt[en].y][s->enemiesSt[en].x] = s->enemiesSt[en].id;
			}
		}
		// プレイヤを確認できない
		else {
			// ランダムムーブ

			// 理想
			// 部屋->ランダムに次に向かう通路を決定
			// 通路->過去の履歴から戻らないように移動

			// 現実，とりあえず実装
			// ８方向の中からランダム
		}
	}
	// 死んでいるとき
	else {
		if (s->enemiesSt[en].killedEnemyTurn > 0) {
			s->enemiesSt[en].killedEnemyTurn--;
		}
		else {
			// 敵を復活
			int count = 0;
			for (int y = 0; y < MAPSIZEY; y++) {
				for (int x = 0; x < MAPSIZEX; x++) {
					if (s->map[y][x] == 0 && (s->x != x || s->y != y) && s->enemies[y][x] == -1) {
						count++;
					}
				}
			}

			Rule_setEachEnemy(s, &(count), en);
		}
	}
	
	// 行動したとき -> enemymapの更新
	Rule_updateEnemyMap(s);
}

int Rule_hasPlayerPosition(State *s, int en) {
	// 敵の位置からプレイヤが見えるか
	
	return TRUE;
}

int Rule_canAttackPlayer(State *s, int en) {
	// 周囲８マスにいて，攻撃が通る（斜め攻撃判定）
	if (Rule_isCollidedPlayer(s, en) == TRUE) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void Rule_updateEnemyMap(State *s) {
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			s->enemies[y][x] = -1;
		}
	}

	// 敵のいる座標：敵ID
	// その他：-1
	for (int en = 0; en < ENEMY_NUMBER; en++) {
		if (s->enemiesSt[en].active == TRUE) {
			s->enemies[s->enemiesSt[en].y][s->enemiesSt[en].x] = s->enemiesSt[en].id;
		}
	}
}

void Rule_updateSeemArea(State *s) {
	// 現在見えている部分(2)を見えた部分(1)に
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			if (s->seem[y][x] == 2) {
				s->seem[y][x] = 1;
			}
		}
	}
	
	int roomID = Rule_getOrDefaultPlayerRoomID(s);
	// プレイヤが部屋にいるとき
	if (roomID != -1) {
		// 部屋の周囲+1マス分を視界内に
		int leftTopX = Rule_convertOneDimValtoX(mt.room[roomID][0]);
		int leftTopY = Rule_convertOneDimValtoY(mt.room[roomID][0]);
		int rightBottomX = Rule_convertOneDimValtoX(mt.room[roomID][1]);
		int rightBottomY = Rule_convertOneDimValtoY(mt.room[roomID][1]);

		for (int y = leftTopY - 1; y <= rightBottomY + 1; y++) {
			for (int x = leftTopX - 1; x <= rightBottomX + 1; x++) {
				// 配列外でなければ
				if (0 <= y && y < MAPSIZEY && 0 <= x && x < MAPSIZEX) {
					s->seem[y][x] = 2;
				}
			}
		}
	}
	// 部屋にいないとき
	else {
		// プレイヤの視界範囲内を更新
		for (int y = s->y - s->view; y <= s->y + s->view; y++) {
			for (int x = s->x - s->view; x <= s->x + s->view; x++) {
				// 配列外でなければ
				if (0 <= y && y < MAPSIZEY && 0 <= x && x < MAPSIZEX) {
					s->seem[y][x] = 2;
				}
			}
		}
	}

		





	/*for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			s->seem[y][x] = 1;
		}
	}*/
}

int Rule_getOrDefaultPlayerRoomID(State *s) {
	int leftTopX, leftTopY;
	int rightBottomX, rightBottomY;
	for (int rn = 0; rn < mt.roomNum; rn++) {
		leftTopX = Rule_convertOneDimValtoX(mt.room[rn][0]);
		leftTopY = Rule_convertOneDimValtoY(mt.room[rn][0]);
		rightBottomX = Rule_convertOneDimValtoX(mt.room[rn][1]);
		rightBottomY = Rule_convertOneDimValtoY(mt.room[rn][1]);
		// 部屋の範囲内の時
		if (leftTopX <= s->x && s->x <= rightBottomX && leftTopY <= s->y && s->y <= rightBottomY) {
			return rn;
		}
	}
	return -1;
}

int Rule_convertActtoDir(int act) {
	if (act == '1') {
		return 0;
	}
	else if (act == '2' || act == 0x50) {
		return 1;
	}
	else if (act == '3') {
		return 2;
	}
	else if (act == '4' || act == 0x4b) {
		return 3;
	}
	else if (act == '5') {
		return 4;
	}
	else if (act == '6' || act == 0x4d) {
		return 5;
	}
	else if (act == '7') {
		return 6;
	}
	else if (act == '8' || act == 0x48) {
		return 7;
	}
	else if (act == '9') {
		return 8;
	}
	else {
		return -1;
	}
}

void Rule_copyState(State* s1, State* s2) {
	s2->gameFlag = s1->gameFlag;
	s2->gameTurn = s1->gameTurn;

	s2->flrNum = s1->flrNum;
	s2->flrTurn = s1->flrTurn;
	s2->flrResetFlag = s1->flrResetFlag;
	s2->hp = s1->hp;
	s2->mhp = s1->mhp;
	s2->stm = s1->stm;
	s2->autoHealVal = s1->autoHealVal;
	s2->lv = s1->lv;
	s2->exp = s1->exp;
	s2->lvupExp = s1->lvupExp;
	s2->lvupExpSum = s1->lvupExpSum;
	s2->x = s1->x;
	s2->y = s1->y;

	s2->stmDicTurnCount = s1->stmDicTurnCount;

	s2->view = s1->view;

	s2->testState = s1->testState;
	for (int y = 0; y < MAPSIZEY; y++) {
		for (int x = 0; x < MAPSIZEX; x++) {
			s2->map[y][x] = s1->map[y][x];
			s2->seem[y][x] = s1->seem[y][x];
			s2->enemies[y][x] = s1->enemies[y][x];
			s2->items[y][x] = s1->items[y][x];
		}
	}
	for (int en = 0; en < ENEMY_NUMBER; en++) {
		s2->enemiesSt[en].hp = s1->enemiesSt[en].hp;
		s2->enemiesSt[en].mhp = s1->enemiesSt[en].mhp;
		s2->enemiesSt[en].id = s1->enemiesSt[en].id;
		s2->enemiesSt[en].active = s1->enemiesSt[en].active;
		s2->enemiesSt[en].x = s1->enemiesSt[en].x;
		s2->enemiesSt[en].y = s1->enemiesSt[en].y;
		s2->enemiesSt[en].point = s1->enemiesSt[en].point;
		s2->enemiesSt[en].killedEnemyTurn = s1->enemiesSt[en].killedEnemyTurn;
		
		s2->enemiesSt[en].testEnemy = s1->enemiesSt[en].testEnemy;
	}
	for (int in = 0; in < PLAYER_INVENTORYSIZE; in++) {
		s2->inv[in].itemID = s1->inv[in].itemID;
		s2->inv[in].usageCount = s1->inv[in].usageCount;
	}
}

int Rule_getRandom(int min, int max)
{
	return min + (int)(rand()*(max - min + 1.0) / (1.0 + RAND_MAX));
}

int Rule_getMapSizeX(void) {
	return MAPSIZEX;
}

int Rule_getMapSizeY(void) {
	return MAPSIZEY;
}

int Rule_getEnemyNum(void) {
	return ENEMY_NUMBER;
}

int Rule_getInvSize(void) {
	return PLAYER_INVENTORYSIZE;
}

char Rule_getItemIcon(int itemID) {
	return itemList[itemID].icon;
}


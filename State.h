#ifndef STATE_H
#define STATE_H


static const int TRUE = 1;
static const int FALSE = 0;

static const int GAME_CLEAR = 1;
static const int GAME_OVER = 2;
static const int GAME_PLAYING = 0;

typedef struct {
	int testEnemy;

	int id; // 敵id，map更新時使用
	int active; // アクティブ，ノンアクティブ
	int hp, mhp; // ステータス
	int x, y; // 座標
	int point; // 得られる経験値
	int killedEnemyTurn; // 復活までのターン数
} Enemy;

typedef struct {
	int itemID; // アイテムid
	int usageCount; // 残り使用回数
} Inventory;

typedef struct {
	int testState;
	
	// 公開
	int gameFlag; // ゲームクリア:1，ゲームオーバー:2，プレイ中:0
	int gameTurn; // 経過ターン数
	int flrResetFlag; // フロアのリセットフラグ
	int flrNum; // フロア数
	int flrTurn; // フロアターン数
	int x, y; // プレイヤ座標
	int hp, mhp, stm, lv, exp; // プレイヤステータス
	
	int lvupExp; // lvupに必要な経験値
	int lvupExpSum; // 経験値の合計
	double autoHealVal; // 自然回復

	int stmDicTurnCount; // 腹減り

	int view; // 視界

	Inventory *inv; // インベントリ

	// 一部非公開
	int **map; // 0:通行可，1;通行不可，2:階段，(-100:未探索，プレイヤに渡すときのみ)
	int **seem; // 0:みえない，1:見た，2:現在みている
	int **enemies; // -1:なし，0~:id
	int **items; // -1:なし，0~:id
	Enemy* enemiesSt; // 敵
} State;


#endif
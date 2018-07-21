
//コンピュータオセロ ver1.0
 
#include <stdio.h>
 
/******************************* 根幹部分に関する記述  *******************************************/
//マスの状態を定義
#define NONE  0  //空きマス
#define BLACK 1  //黒駒
#define WHITE 2  //白駒
//ボード（左上を[0][0])
int board[8][8];
//マスの状態の数
int NONE_NUMBER;  //空きマス
int BLACK_NUMBER; //黒駒
int WHITE_NUMBER; //白駒
//ターン
int turn = 1; //1が先手、2が後手
//コンピュータ・人間のターン
int human, computer;
//方向ベクトルを定義
int vec_x[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
//方向ベクトルを定義
int vec_y[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
//変換関数のプロトタイプ宣言
int Trans_x(int place);                 //多次元配列のx要素に変換
int Trans_y(int place);                 //多次元配列のy要素に変換
int Trans_place(int x, int y);          //多次元配列のx,y要素を人間が入力する値に変換
int EnemyColor(int turn);               //相手の色を出力
int StretchVec(int a);                  //その方向のベクトルを１つ伸ばす(成分を入力すると、１つ伸ばした成分を出力する）
//重要関数のプロトタイプ宣言
void Initia(void);                      //初期化
void Disp(void);                        //現在のボードを描画
int CanGo(int place, int turn);         //有効な場所かどうかを判断（有効な場合は1、有効でない場合は0を返却)
int CanPut(int turn);                   //そのターンが駒を置けるかどうかを判別（置ける場合は1、置けない場合は0を返却）
void Count(const int ThinkBoard[8][8]); //空きマス、黒駒、白駒の数をカウント
void Flip(int place, int turn);         //駒を裏返す
void Change(void);                      //ターンを入れ替える
void Judge(void);                       //勝敗を決定
 
/* ***************************探索アルゴリズムに関する記述************************************* */
void BestPlace(int ThinkBoard[8][8], int turn);           //コンピューターの評価点を表示し最善の場所をグローバル変数bestplaceに保存
int minimax(int ThinkBoard[8][8], int turn, int depth);  //minimax法で評価点を出力
 
 
 
//探索アルゴリズムに関する設定
#define DEPTH 5     //読みの深さを設定
 
//グローバル宣言
int bestplace;              //最善の場所を保存する変数
int saveboard[8][8][DEPTH]; //局面を保存する変数
long count = 0;              //読み込んだ局面数を保存する変数
 
/* ***************************評価関数に関する記述********************************************* */
//評価関数のプロトタイプ宣言
void ChangeStage(const int ThinkBoard[8][8]);                        //局面を判断して評価関数のパラメーターを変化
int Evalu_Total(const int ThinkBoard[8][8], int turn);               //評価値の合計を計算（評価する局面とターンを値渡し）
int Evalu_HowMany(const int ThinkBoard[8][8], int turn);             //着手可能手数を計算して返却（評価する局面とター73ンを値渡し）
int Evalu_BoardScore(const int ThinkBoard[8][8], int turn);          //マスの評価値の合計を計算して数値を返却
int Evalu_HowManyStones(const int ThinkBoard[8][8], int turn);       //駒の数を計算して数値を返却
int Evalu_HowManyFixedStones(const int ThinkBoard[8][8], int turn);  //確定石を計算して数値を返却（辺上のみ）
int Evalu_Wing(const int ThinkBoard[8][8], int turn);                //ウィングの数を返却
int Evalu_Mountain(const int ThinkBoard[8][8], int turn);            //山の数を返却
#define EVALUATION_POINTS 6
 
//評価関数の設定に関する記述
//Evalu_BoardScore()のマスの得点を定義
const int CellScore[8][8] = {
 
	{ 50, -6, 7, 3, 3, 7, -6, 50 },
 
	{ -6, -10, -2, -2, -2, -2, -10, -6 },
 
	{ 7, -2, 1, 1, 1, 1, -2, 7 },
 
	{ 3, -2, 1, 3, 3, 1, -2, 3 },
 
	{ 3, -2, 1, 3, 3, 1, -2, 3 },
 
	{ 7, -2, 1, 1, 1, 1, -2, 7 },
 
	{ -6, -10, -2, -2, -2, -2, -10, -6 },
 
	{ 50, -6, 7, 3, 3, 7, -6, 50 },
 
};
 
//序盤、中盤、終盤を定義（その空きマスの数まで）
#define FIRST 30  //64 - 30マスまでが序盤
#define MIDDLE 10 //31 - 10マスまでが中盤

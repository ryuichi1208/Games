#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10      // 盤面サイズ 10 * 10
#define STONE_SPACE 0      // 盤面にある石 なし
#define STONE_BLACK 1      // 盤面にある石 黒
#define STONE_WHITE 2      // 盤面にある石 白

void inputPutPos(int board[][BOARD_SIZE], int which);
void changeTurn(int *which_turn);
int checkOutPos(int x, int y);
void gameInit(int board[][BOARD_SIZE], int *which_turn);
void boardInit(int board[][BOARD_SIZE]);
void boardPrint(int board[][BOARD_SIZE]);
int gameEndProcess(int board[][BOARD_SIZE]);
int lenCheck(int board[][BOARD_SIZE], int x, int y);

//=======================================================
// main
//=======================================================
int main()
{
    // 変数宣言
    int board[BOARD_SIZE][BOARD_SIZE];
    int which_turn;
    
    // 初期処理
    gameInit(board, &which_turn);
    boardPrint(board);
    
    //---- メインループ
    while (1) {
        //--- 入力処理
        inputPutPos(board, which_turn);

        //--- 演算処理
        changeTurn(&which_turn);
        
        //--- 出力処理
        boardPrint(board);
        
        //--- 終了判定
        if (gameEndProcess(board)) { break; }
    }
    return 0;
}

//-------------------------------------------------
// 置く場所入力
//-------------------------------------------------
void inputPutPos(int board[][BOARD_SIZE], int which)
{
    int pos_x, pos_y;
    
    printf("%s", (which == 1) ? "●" : "○");
    printf("の番です。どこに置きますか\n> ");
    while (1) {
        scanf("%d %d", &pos_x, &pos_y);
        if (checkOutPos(pos_x, pos_y) && board[pos_y][pos_x] == STONE_SPACE) { break; }
        printf("不正な入力です\n> ");
    }
    board[pos_y][pos_x] = which;
}

//-------------------------------------------------
// 手番交代処理
//-------------------------------------------------
void changeTurn(int *which_turn)
{
    *which_turn = (*which_turn == STONE_BLACK) ? STONE_WHITE : STONE_BLACK;
}

//-------------------------------------------------
// 範囲外チェック
//-------------------------------------------------
int checkOutPos(int x, int y)
{
    return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE);
}

//-------------------------------------------------
// ゲーム情報初期化
//-------------------------------------------------
void gameInit(int board[][BOARD_SIZE], int *which_turn)
{
    boardInit(board);
    *which_turn = STONE_BLACK;
}

//-------------------------------------------------
// 盤面初期化
//-------------------------------------------------
void boardInit(int board[][BOARD_SIZE])
{
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = STONE_SPACE;
        }
    }
}

//-------------------------------------------------
// 盤面出力
//-------------------------------------------------
void boardPrint(int board[][BOARD_SIZE])
{
    int i, j;
    
    printf("  ");
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
    }
    puts("");
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
        for (j = 0; j < BOARD_SIZE; j++) {
            switch (board[i][j]) {
            case STONE_SPACE: printf("・"); break;
            case STONE_BLACK: printf("●"); break;
            case STONE_WHITE: printf("○"); break;
            }
        }
        puts("");
    }
    puts("");
}

//-------------------------------------------------
// ゲーム終了処理
//-------------------------------------------------
int gameEndProcess(int board[][BOARD_SIZE])
{
    int i, j, len_flag;
    
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == STONE_SPACE) { continue; }
            if (lenCheck(board, j, i)) {
                printf("%sの勝ちです。\n", (board[i][j] == STONE_BLACK) ? "●" : "○");
                return 1;
            }
        }
    }
    return 0;
}

//-------------------------------------------------
// 5連確認
//-------------------------------------------------
int lenCheck(int board[][BOARD_SIZE], int x, int y)
{
    int i, j, len_flag;
    int dx[] = { 0, 1, 1 };
    int dy[] = { 1, 0, 1 };
    
    for (i = 0; i < 3; i++) {
        for (j = 1, len_flag = 1; j <= 4; j++) {
            if (board[y][x] != board[y+j*dy[i]][x+j*dx[i]]) {
                len_flag = 0;
                break;
            }
        }
        if (len_flag == 1) { return 1; }
    }
    return 0;
}

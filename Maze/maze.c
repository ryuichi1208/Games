#include <stdio.h>
#include "maze.h"

int MazePlayerInit(int *playerRow, int *playerColumn, MazeBlock maze[MAZE_ROW][MAZE_COLUMN]) {
	int i, j;

	for(i = 0; i < MAZE_ROW ; i++) {
		for(j = 0;  j < MAZE_COLUMN; j++) {
			if(maze[i][j].kind == START) {
				*playerRow    = i;
				*playerColumn = j;
				return 0;
			}
		}
	}

	//スタート地点がなければ、プレイヤーを設定できずここまでくる
	printf("スタートがありません\n");
	return -1;
}


//迷路表示
void MazeDraw(int playerRow, int playerColumn, MazeBlock maze[MAZE_ROW][MAZE_COLUMN]) {
	int i, j;

	for(i = 0; i < MAZE_ROW; i++) {
		for(j = 0; j < MAZE_COLUMN; j++ ) {
			if(i == playerRow && j == playerColumn) {
				printf("Ｐ");
			} else if(maze[i][j].flag == FALSE) {
				printf("？");
			} else {
				switch(maze[i][j].kind) {
					case WALL:
						printf("口"); break;
					case GOAL:
						printf("Ｇ"); break; 
					default:
						printf("　"); break;
				}
			}
		}
		printf("\n");
	}
}

enum MazeDirection{UP, DOWN, LEFT, RIGHT, Invalid}; 

void MazePlayerMove(int *playerRow, int *playerColumn, MazeBlock maze[MAZE_ROW][MAZE_COLUMN]) {
	char buf[100];
	int  direction = -1;

	printf("%d:上\n", UP);
	printf("%d:下\n", DOWN);
	printf("%d:左\n", LEFT);
	printf("%d:右\n", RIGHT);
	printf("数字を入力してください。：");

	fgets(buf, sizeof(buf), stdin);      //文字列で入力を読み込む
	sscanf(buf, "%d", &direction);        //数字にできる場合は変換(できなければ何もしない)

	while(direction < 0 || direction > (Invalid - 1)) {
		printf("入力が不正です。再入力してください:");
		fgets(buf, sizeof(buf), stdin);
		sscanf(buf, "%d", &direction);
	}


	switch(direction){
		case UP: {
			if(*playerRow - 1 >= 0) {
				maze[*playerRow - 1][*playerColumn].flag = TRUE;
					if(maze[*playerRow - 1][*playerColumn].kind != WALL) {
						*playerRow -= 1; //移動
						printf("\n上に移動しました。\n");
					} else {
						printf("\n壁です。\n");
					}
			} else {
				printf("\n範囲外です\n");
			}
		}
		break;

		//下移動
		case DOWN: {
			if(*playerRow + 1 < MAZE_ROW) {
				maze[*playerRow + 1][*playerColumn].flag = TRUE;
				if(maze[*playerRow + 1][*playerColumn].kind != WALL) {
					*playerRow += 1;
					printf("\n下に移動しました。\n");
				} else {
					printf("\n壁です。\n");
				}
			} else {
				printf("\n範囲外です\n");

			}
		}
		break;

		//左移動
		case LEFT: {
			if(*playerColumn - 1 >= 0) {
				maze[*playerRow][*playerColumn - 1].flag = TRUE;
				if(maze[*playerRow][*playerColumn - 1].kind != WALL) {
					*playerColumn -= 1;
					printf("\n左に移動しました。\n");
				} else {
					printf("\n壁です。\n");
				}
			} else {
				printf("\n範囲外です\n");

			}
		}
		break;

		//右移動
		case RIGHT: {
			if(*playerColumn + 1 < MAZE_COLUMN) {
				maze[*playerRow][*playerColumn + 1].flag = TRUE;
				if(maze[*playerRow][*playerColumn + 1].kind != WALL) {
					*playerColumn += 1;
					printf("\n右に移動しました。\n");
				} else {
					printf("\n壁です。\n");
				}
			} else {
				printf("\n範囲外です\n");
			}
		}
		break;
	}
}


//ゴール判定
int MazeGoalCheck(int playerRow, int playerColumn, MazeBlock maze[MAZE_ROW][MAZE_COLUMN])
{
  if(maze[playerRow][playerColumn].kind == GOAL) //プレイヤー位置がゴール地点に等しい
  {
    printf("ゴール!\n");
    return 1;
  }
  return 0;
}


//迷路ゲーム
void MazeGame(int stage)
{
  //プレイヤー
  MazePlayer player;

  //迷路
  MazeBlock maze[STAGE][MAZE_ROW][MAZE_COLUMN] = 
    {
      { //STAGE0
        { {START, TRUE } , {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE} },
        { {WALL , FALSE} , {WALL , FALSE}, {PATH , FALSE}, {WALL , FALSE}, {WALL , FALSE} },
        { {WALL , FALSE} , {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE} },
        { {PATH , FALSE} , {PATH , FALSE}, {WALL , FALSE}, {WALL , FALSE}, {WALL , FALSE} },
        { {WALL , FALSE} , {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {GOAL , TRUE } },
      },

      { //STAGE1
        { {PATH , FALSE} , {WALL , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE} },
        { {PATH , FALSE} , {WALL , FALSE}, {PATH , FALSE}, {WALL , FALSE}, {PATH , FALSE} },
        { {START, TRUE } , {PATH , FALSE}, {PATH , FALSE}, {WALL , FALSE}, {GOAL , TRUE } },
        { {PATH , FALSE} , {WALL , FALSE}, {WALL , FALSE}, {WALL , FALSE}, {WALL , FALSE} },
        { {PATH , FALSE} , {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE} },
      }
    };

  //プレイヤー初期化
  if(MazePlayerInit(&player.row, &player.column, maze[stage]) == -1)
  {
    //関数MazePlayerInitが-1を返すとき初期化に失敗している
    //よって、この時点でプログラムを終了し、迷路の表示は行わない
    return;
  }

  while(MazeGoalCheck(player.row, player.column, maze[stage]) != 1) //ゴールするまで移動を繰り返す
  {
    //迷路表示
    MazeDraw(player.row, player.column, maze[stage]);
    //プレイヤー移動
    MazePlayerMove(&player.row, &player.column, maze[stage]);
  }

  //迷路最終結果表示
  MazeDraw(player.row, player.column, maze[stage]);
}


//メニュー
enum MazeMenu {STAGE0, STAGE1, EXIT};

//タイトル
int MazeTitle()
{
  char buf[100];
  int menu = -1;

  printf("\n\n＊＊＊迷路ゲーム＊＊＊\n\n");

  printf("メニュー\n");
  printf("%d:ステージ0\n", STAGE0);
  printf("%d:ステージ1\n", STAGE1);
  printf("%d:終了\n", EXIT);

  printf("メニューを選んでください。：");

  fgets(buf, sizeof(buf), stdin);
  sscanf(buf, "%d", &menu);

  while(menu < 0 || menu > EXIT)
  {
    printf("入力が不正です。再入力してください:");
    fgets(buf, sizeof(buf), stdin);
    sscanf(buf, "%d", &menu);
  }

  return menu;
}


int main(void)
{
  int menu;
  int stage;

  while(1)
  {
    //メニュー選択      
    menu = MazeTitle();
    printf("\n");

    if(menu == EXIT) //EXITならwhileループを抜けて終了
    {
      break;
    }
    else
    {
      //ステージ設定
      switch(menu)
      {
      case STAGE0:
        stage = 0;
        break;

      case STAGE1:
        stage = 1;
        break;
      }
    }

    //迷路ゲーム
    MazeGame(stage);

    //ゲームが終わるとwhileループの先頭にもどる
  }

  return 0;
}

#define STAGE       2      //ステージ数
#define MAZE_ROW    5      //迷路の行数
#define MAZE_COLUMN 5      //迷路の列数

//プレイヤー
typedef struct
{
  int row;        //プレイヤー位置(行)
  int column;     //プレイヤー位置(列)
}MazePlayer;


//迷路の一ブロック
enum MazeKind {PATH, WALL, START, GOAL};    //ブロックの種類(道、壁、スタート、ゴール)
enum MazeFlag {FALSE, TRUE};                //ブロックが判明しているかどうか

typedef struct
{
  enum MazeKind kind;            //種類(道、壁、スタート、ゴール)
  enum MazeFlag flag;            //ブロックが判明しているかどうか
} MazeBlock;

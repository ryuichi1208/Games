#include "othello.h"

int main(void){
 
	//コンティニューするかどうかを保存する変数
	int retry;
 
	//リトライする時はここでループ
	while (1){
 
		//初期化
		Initia();
 
		//人間が先手・後手を決める
		while (1){
			printf("先手が黒、後手が白です。\n");
			printf("あなたは先手ですか？後手ですか？\n先手なら1、後手なら2を入力してください。\n");
			printf("入力："); scanf_s("%d", &human);
 
			if (human == 1){
				break;
			}
			else if (human == 2){
				break;
			}
			printf("無効な値です。\a\a\a\n\n");
		}
		if (human == 1){
			computer = 2;
		}
		else if (human == 2){
			computer = 1;
		}
		printf("\n\n");
 
		//空きマスがある・パスの時はここでループ
		while (1){
 
			//ボードを描画
			Disp();
			printf("\n");
 
			//人間の手
			if (turn == human){
				//有効な手であるまでループ
				while (1){
 
					//削除する表現
					BestPlace(board, human);
					printf("人間の予想場所：%d\n\n", bestplace);
					printf("コンピュータの確定石の数 = %d\n", Evalu_HowManyFixedStones(board, computer));
					printf("コンピュータのウィングの数 = %d\n", Evalu_Wing(board, computer));
					printf("コンピュータの山の数 = %d\n", Evalu_Mountain(board, computer));
					printf("人間の確定石の数 = %d\n", Evalu_HowManyFixedStones(board, human));
					printf("人間のウィングの数 = %d\n", Evalu_Wing(board, human));
					printf("人間の山の数 = %d\n\n", Evalu_Mountain(board, human));
 
 
					//場所を入力
					int human_place;
					printf("場所："); scanf_s("%d", &human_place);
					if (!(CanGo(human_place, human))){
						printf("無効な数値です。もう一度入力してください。\a\a\a\n\n");
						continue;
					}
					Flip(human_place, human);
					break;
				}
			}
 
			//コンピュータの手
			if (turn == computer){
				printf("思考中・・・\n\n\n");
				//コンピューターの評価点を表示し最善の場所をグローバル変数bestplaceに保存
				BestPlace(board, computer);
				printf("読み込んだ局面数：%d\n", count);
				printf("コンピュータは%dに置きました。\n\n", bestplace);
				//エラー判定
				if (!(CanGo(bestplace, computer))){
					printf("エラー3\n\n\a\a\a");
					return(0);
				}
 
				//裏返す
				Flip(bestplace, computer);
			}
 
			//黒駒・白駒・空きマスをカウント
			Count(board);
 
			//終了の時はループをブレイク
			if (NONE_NUMBER == 0){
				printf("空きマスが無くなったので終了です。\n\n");
				break;
			}
 
			//一色になったら終了
			if (BLACK_NUMBER == 0){
				printf("ボード上に白しかありません。\n\n");
				break;
			}
			else if (WHITE_NUMBER == 0){
				printf("ボード上に黒しかありません。\n\n");
				break;
			}
 
			//パスの場合はここでループ
			if (!(CanPut(EnemyColor(turn)))){
				if (EnemyColor(turn) == human){
					puts("人間は置く場所がありません。");
				}
				else if (EnemyColor(turn) == computer){
					puts("コンピューターは置く場所がありません。");
				}
				else{
					puts("エラー１");
				}
				continue;
			}
 
 
			//ターンチェンジ
			Change();
 
 
 
		}
 
		//ボードを描画
		Disp();
 
		//結果を出力
		Judge();
 
		printf("\n\n\n");
		puts("リトライするときは1、そうでない場合は2を入力してください。");
		printf("入力："); scanf_s("%d", &retry);
 
		if (retry == 1){
			continue;
		}
		else{
			break;
		}
 
	}
 
	return(0);
}
 
//評価値の合計を計算
int Evalu_Total(const int ThinkBoard[8][8], int turn){
 
	//評価値の合計
	int totalscore = 0;
 
	//評価関数のパラメーターを変化させる
	ChangeStage(ThinkBoard);
 
	int e[EVALUATION_POINTS] = { Evalu_HowMany(ThinkBoard, turn), Evalu_BoardScore(ThinkBoard, turn),
		Evalu_HowManyStones(ThinkBoard, turn), Evalu_HowManyFixedStones(ThinkBoard, turn), Evalu_Wing(ThinkBoard, turn),
		Evalu_Mountain(ThinkBoard, turn) };
 
	for (int i = 0; i < EVALUATION_POINTS; i++){
		totalscore += w[i] * e[i];
	}
 
	return(totalscore);
}
 
 
//着手可能手数を計算して返却（評価する局面とターンを値渡し）
int Evalu_HowMany(const int ThinkBoard[8][8], int turn){
 
	//着手可能手数を保存する変数
	int count = 0;
 
	//総当りで解析
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (CanGo(Trans_place(j, i), turn) == 1){
				count++;
			}
		}
	}
 
	return(count);
}
 
//マスの評価値の合計を計算して数値を返却
int Evalu_BoardScore(const int ThinkBoard[8][8], int turn){
 
	//評価値を保存する変数
	int score = 0;
 
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (ThinkBoard[i][j] == turn){
				score += CellScore[i][j];
			}
		}
	}
 
	return(score);
}
 
//自分の駒の数を計算して数値を返却
int Evalu_HowManyStones(const int ThinkBoard[8][8], int turn){
 
	//カウントする
	Count(ThinkBoard);
 
	if (turn == 1){
		return(BLACK_NUMBER);
	}
	else if (turn == 2){
		return(WHITE_NUMBER);
	}
 
	//エラー
	return(-10000);
}
 
//確定石を計算して数値を返却（辺のみ）
int Evalu_HowManyFixedStones(const int ThinkBoard[8][8], int turn){
 
	/*
 
	【確定石のアルゴリズム】
	(1)辺が全て埋まっている時の辺の駒
	(2)角から空きマスまでの駒が全て自分の時の駒
 
	*/
 
	int fixed = 0;  //確定石のカウント
	bool flag_up = true;
	bool flag_right = true;
	bool flag_down = true;
	bool flag_left = true;
 
 
	//角の駒（確定石）の数をカウント
	if (ThinkBoard[0][0] == turn) fixed++;
	if (ThinkBoard[0][7] == turn) fixed++;
	if (ThinkBoard[7][7] == turn) fixed++;
	if (ThinkBoard[7][0] == turn) fixed++;
 
 
	//辺が全て埋まっているかどうかを判定
	for (int i = 0; i < 8; i++){
		if (flag_up == true && ThinkBoard[0][i] == NONE)  flag_up = false;     //上の辺を探索
		if (flag_right == true && ThinkBoard[i][7] == NONE)  flag_right = false;  //右の辺を探索
		if (flag_down == true && ThinkBoard[7][i] == NONE)  flag_down = false;   //下の辺を探索
		if (flag_left == true && ThinkBoard[i][0] == NONE)  flag_left = false;   //左の辺を探索
	}
 
	//上の辺をカウント
	if (flag_up == true){  //上の辺が全て埋まっている場合
		for (int i = 1; i < 7; i++){
			if (ThinkBoard[0][i] == turn) fixed++;
		}
	}
	else if (flag_up == false) {  //上の辺に空きマスがある場合
		if (ThinkBoard[0][0] == turn){
			for (int i = 1; i < 7; i++){  //左から右へ探索
				if (ThinkBoard[0][i] == NONE || ThinkBoard[0][i] == EnemyColor(turn)) break;
				fixed++;
			}
		}
		if (ThinkBoard[0][7] == turn){  //右から左へ探索
			for (int i = 1; i < 7; i++){
				if (ThinkBoard[0][7 - i] == NONE || ThinkBoard[0][7 - i] == EnemyColor(turn)) break;
				fixed++;
			}
		}
	}
	else{
		printf("エラー６\n\n");
		return(-10000);
	}
 
 
	//右の辺をカウント
	if (flag_right == true){  //右の辺が全て埋まっている場合
		for (int i = 1; i < 7; i++){
			if (ThinkBoard[i][7] == turn) fixed++;
		}
	}
	else if (flag_right == false) {  //右の辺に空きマスがある場合
		if (ThinkBoard[0][7] == turn){
			for (int i = 1; i < 7; i++){  //上から下へ探索
				if (ThinkBoard[i][7] == NONE || ThinkBoard[i][7] == EnemyColor(turn)) break;
				fixed++;
			}
		}
		if (ThinkBoard[7][7] == turn){  //下から上へ探索
			for (int i = 1; i < 7; i++){
				if (ThinkBoard[7 - i][7] == NONE || ThinkBoard[7 - i][7] == EnemyColor(turn)) break;
				fixed++;
			}
		}
	}
	else{
		printf("エラー７\n\n");
		return(-10000);
	}
 
	//下の辺をカウント
	if (flag_down == true){  //下の辺が全て埋まっている場合
		for (int i = 1; i < 7; i++){
			if (ThinkBoard[7][i] == turn) fixed++;
		}
	}
	else if (flag_down == false) {  //下の辺に空きマスがある場合
		if (ThinkBoard[7][0] == turn){
			for (int i = 1; i < 7; i++){  //左から右へ探索
				if (ThinkBoard[7][i] == NONE || ThinkBoard[7][i] == EnemyColor(turn)) break;
				fixed++;
			}
		}
		if (ThinkBoard[7][7] == turn){  //右から左へ探索
			for (int i = 1; i < 7; i++){
				if (ThinkBoard[7][7 - i] == NONE || ThinkBoard[7][7 - i] == EnemyColor(turn)) break;
				fixed++;
			}
		}
	}
	else{
		printf("エラー８\n\n");
		return(-10000);
	}
 
	//左の辺をカウント
	if (flag_left == true){
		for (int i = 1; i < 7; i++){
			if (ThinkBoard[i][0] == turn) fixed++;
		}
	}
	else if (flag_left == false) {  //左の辺に空きマスがある場合
		if (ThinkBoard[0][0] == turn){
			for (int i = 1; i < 7; i++){  //上から下へ探索
				if (ThinkBoard[i][0] == NONE || ThinkBoard[i][0] == EnemyColor(turn)) break;
				fixed++;
			}
		}
		if (ThinkBoard[7][0] == turn){  //下から上へ探索
			for (int i = 1; i < 7; i++){
				if (ThinkBoard[7 - i][0] == NONE || ThinkBoard[7 - i][0] == EnemyColor(turn)) break;
				fixed++;
			}
		}
	}
	else{
		printf("エラー９\n\n");
		return(-10000);
	}
 
	return(fixed);
}
 
//ウィングの数を返却
int Evalu_Wing(const int ThinkBoard[8][8], int turn){
 
	int wing = 0;
 
	//上辺を判断
	if (ThinkBoard[0][0] == NONE && ThinkBoard[0][1] == turn && ThinkBoard[0][2] == turn && ThinkBoard[0][3] == turn &&
		ThinkBoard[0][4] == turn && ThinkBoard[0][5] == turn && ThinkBoard[0][6] == NONE && ThinkBoard[0][7] == NONE){
		wing++;
	}
	else if (ThinkBoard[0][0] == NONE && ThinkBoard[0][1] == NONE && ThinkBoard[0][2] == turn && ThinkBoard[0][3] == turn &&
		ThinkBoard[0][4] == turn && ThinkBoard[0][5] == turn && ThinkBoard[0][6] == turn && ThinkBoard[0][7] == NONE){
		wing++;
	}
 
	//右辺を判断
	if (ThinkBoard[0][7] == NONE && ThinkBoard[1][7] == turn && ThinkBoard[2][7] == turn && ThinkBoard[3][7] == turn &&
		ThinkBoard[4][7] == turn && ThinkBoard[5][7] == turn && ThinkBoard[6][7] == NONE && ThinkBoard[7][7] == NONE){
		wing++;
	}
	else if (ThinkBoard[0][7] == NONE && ThinkBoard[1][7] == NONE && ThinkBoard[2][7] == turn && ThinkBoard[3][7] == turn &&
		ThinkBoard[4][7] == turn && ThinkBoard[5][7] == turn && ThinkBoard[6][7] == turn && ThinkBoard[7][7] == NONE){
		wing++;
	}
 
	//下辺を判断
	if (ThinkBoard[7][0] == NONE && ThinkBoard[7][1] == turn && ThinkBoard[7][2] == turn && ThinkBoard[7][3] == turn &&
		ThinkBoard[7][4] == turn && ThinkBoard[7][5] == turn && ThinkBoard[7][6] == NONE && ThinkBoard[7][7] == NONE){
		wing++;
	}
	else if (ThinkBoard[7][0] == NONE && ThinkBoard[7][1] == NONE && ThinkBoard[7][2] == turn && ThinkBoard[7][3] == turn &&
		ThinkBoard[7][4] == turn && ThinkBoard[7][5] == turn && ThinkBoard[7][6] == turn && ThinkBoard[7][7] == NONE){
		wing++;
	}
 
	//左辺を判断
	if (ThinkBoard[0][0] == NONE && ThinkBoard[1][0] == turn && ThinkBoard[2][0] == turn && ThinkBoard[3][0] == turn &&
		ThinkBoard[4][0] == turn && ThinkBoard[5][0] == turn && ThinkBoard[6][0] == NONE && ThinkBoard[7][0] == NONE){
		wing++;
	}
	else if (ThinkBoard[0][0] == NONE && ThinkBoard[1][0] == NONE && ThinkBoard[2][0] == turn && ThinkBoard[3][0] == turn &&
		ThinkBoard[4][0] == turn && ThinkBoard[5][0] == turn && ThinkBoard[6][0] == turn && ThinkBoard[7][0] == NONE){
		wing++;
	}
 
	return(wing);
}
 
 
//山の数を返却
int Evalu_Mountain(const int ThinkBoard[8][8], int turn){
 
	int mountain = 0;
 
	//上辺を判断
	if (ThinkBoard[0][0] == NONE && ThinkBoard[0][1] == turn && ThinkBoard[0][2] == turn && ThinkBoard[0][3] == turn &&
		ThinkBoard[0][4] == turn && ThinkBoard[0][5] == turn && ThinkBoard[0][6] == turn && ThinkBoard[0][7] == NONE &&
		ThinkBoard[1][0] == NONE && ThinkBoard[1][1] == NONE && ThinkBoard[1][2] != NONE && ThinkBoard[1][3] != NONE &&
		ThinkBoard[1][4] != NONE && ThinkBoard[1][5] != NONE && ThinkBoard[1][6] == NONE && ThinkBoard[1][7] == NONE){
		mountain++;
	}
 
	//右辺を判断
	if (ThinkBoard[0][7] == NONE && ThinkBoard[1][7] == turn && ThinkBoard[2][7] == turn && ThinkBoard[3][7] == turn &&
		ThinkBoard[4][7] == turn && ThinkBoard[5][7] == turn && ThinkBoard[6][7] == turn && ThinkBoard[7][7] == NONE &&
		ThinkBoard[0][6] == NONE && ThinkBoard[1][6] == NONE && ThinkBoard[2][6] != NONE && ThinkBoard[3][6] != NONE &&
		ThinkBoard[4][6] != NONE && ThinkBoard[5][6] != NONE && ThinkBoard[6][6] == NONE && ThinkBoard[7][6] == NONE){
		mountain++;
	}
 
	//下辺を判断
	if (ThinkBoard[7][0] == NONE && ThinkBoard[7][1] == turn && ThinkBoard[7][2] == turn && ThinkBoard[7][3] == turn &&
		ThinkBoard[7][4] == turn && ThinkBoard[7][5] == turn && ThinkBoard[7][6] == turn && ThinkBoard[7][7] == NONE &&
		ThinkBoard[6][0] == NONE && ThinkBoard[6][1] == NONE && ThinkBoard[6][2] != NONE && ThinkBoard[6][3] != NONE &&
		ThinkBoard[6][4] != NONE && ThinkBoard[6][5] != NONE && ThinkBoard[6][6] == NONE && ThinkBoard[6][7] == NONE){
		mountain++;
	}
 
	//左辺を判断
	if (ThinkBoard[0][0] == NONE && ThinkBoard[1][0] == turn && ThinkBoard[2][0] == turn && ThinkBoard[3][0] == turn &&
		ThinkBoard[4][0] == turn && ThinkBoard[5][0] == turn && ThinkBoard[6][0] == turn && ThinkBoard[7][0] == NONE &&
		ThinkBoard[0][1] == NONE && ThinkBoard[1][1] == NONE && ThinkBoard[1][2] != NONE && ThinkBoard[1][3] != NONE &&
		ThinkBoard[1][4] != NONE && ThinkBoard[1][5] != NONE && ThinkBoard[1][6] == NONE && ThinkBoard[1][7] == NONE){
		mountain++;
	}
 
	return(mountain);
 
}
 
 
 
//局面を判断して評価関数のパラメーターを変化
void ChangeStage(const int ThinkBoard[8][8]){
 
	//左から【着手可能手数】、【マスの評価値の合計】、【自分の駒の数】、【相手の駒の数】
	//各々の評価関数のウェイトを書き換える
	if (NONE_NUMBER >= FIRST){  //【序盤】
		for (int i = 0; i < EVALUATION_POINTS; i++){
			w[i] = w_first[i];
		}
	}
	else if (FIRST > NONE_NUMBER && NONE_NUMBER >= MIDDLE){  //【中盤】
		for (int i = 0; i < EVALUATION_POINTS; i++){
			w[i] = w_middle[i];
		}
	}
	else if (MIDDLE > NONE_NUMBER && NONE_NUMBER >= FINAL){ //【終盤】
		for (int i = 0; i < EVALUATION_POINTS; i++){
			w[i] = w_final[i];
		}
	}
	else{
		for (int i = 0; i < EVALUATION_POINTS; i++){ //エラー
			w[i] = -10000;
		}
		printf("エラー２\n\n");
	}
 
}
 
//コンピューターの評価点を表示し最善の場所をグローバル変数bestplaceに保存
void BestPlace(int ThinkBoard[8][8], int turn){
 
	//局面数カウントをリセット
	count = 0;
 
	//minimax()で評価点を記録し最善手をグローバル変数bestplaceに記録
	int score = minimax(board, turn, DEPTH);
 
	//評価点を出力
	if (turn == human) printf("\n人間の評価点は%dです。\n", score);
	if (turn == computer) printf("\nコンピューターの評価点：%d\n", score);
 
	//ボード上のカウントを戻す
	Count(ThinkBoard);
 
}
 
//minimax法で評価点を出力
int minimax(int ThinkBoard[8][8], int ThinkTurn, int depth){
 
 
	//ボード上をカウント
	Count(ThinkBoard);
 
	//即死局面
	if (BLACK_NUMBER == 0 || WHITE_NUMBER == 0){
		if (ThinkTurn == turn)             return(-32768); //自分が即死局面
		if (ThinkTurn == EnemyColor(turn)) return(32768);  //相手が即死局面
	}
 
	//空きマスが無い場合
	if (NONE_NUMBER == 0){
		return(Evalu_Total(ThinkBoard, turn));
	}
 
	//リーフの場合は評価値を返却する
	if (depth == 0) return(Evalu_Total(ThinkBoard, turn));
 
	int score = -32768;
	int var;
 
	//局面を保存する
	for (int k = 0; k < 8; k++){
		for (int l = 0; l < 8; l++){
			saveboard[k][l][depth - 1] = ThinkBoard[k][l];
		}
	}
 
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
 
			if (CanGo(Trans_place(j, i), ThinkTurn)){
 
				//実際に裏返す
				Flip(Trans_place(j, i), ThinkTurn);
 
				//局面数をカウント
				count++;
 
				//再帰
				if (CanPut(EnemyColor(ThinkTurn)))    var = minimax(ThinkBoard, EnemyColor(ThinkTurn), depth - 1);
				if (!(CanPut(EnemyColor(ThinkTurn)))) var = minimax(ThinkBoard, ThinkTurn, depth - 1);
 
				//得点と最善手を更新
				if (ThinkTurn == turn && score <= var){
					score = var;
					if (depth == DEPTH){
						bestplace = Trans_place(j, i);
					}
				}
				if (ThinkTurn == EnemyColor(turn) && score <= -var){
					score = -var;
				}
 
				//局面を1手戻す
				for (int k = 0; k < 8; k++){
					for (int l = 0; l < 8; l++){
						ThinkBoard[k][l] = saveboard[k][l][depth - 1];
					}
				}
 
			}
		}
	}
 
	//最大値を返却
	if (ThinkTurn == turn){
		return(score);
	}
	//最小値を返却
	if (ThinkTurn == EnemyColor(turn)){
		return(-score);
	}
 
	//エラー判定
	printf("\a\a\aエラー４");
	return(99999);
 
 
}
 
//多次元配列のx要素に変換
int Trans_x(int place){
	return((place % 10) - 1);
}
 
//多次元配列のy要素に変換
int Trans_y(int place){
	return((place / 10) - 1);
}
 
//多次元配列のx,y要素を人間が入力する値に変換
int Trans_place(int x, int y){
	return((1 + y) * 10 + (x + 1));
}
 
 
//敵の色を出力
int EnemyColor(int turn){
	if (turn == 1){
		return(2);
	}
	else if (turn == 2){
		return(1);
	}
	//エラー
	return(-1);
}
 
//その方向のベクトルを１つ伸ばす
int StretchVec(int a){
 
	int b = a;
 
	if (a > 0){
		b++;
	}
	else if (a < 0){
		b--;
	}
	return(b);
}
 
//初期化
void Initia(void){
 
	//全てのマスを空きにする
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			board[i][j] = NONE;
		}
	}
 
	//最初の４マスを定義
	board[4][3] = BLACK;
	board[3][4] = BLACK;
	board[3][3] = WHITE;
	board[4][4] = WHITE;
 
}
 
//ボードを描画
void Disp(void){
 
	//行を描画
	printf("　");
	for (int i = 1; i <= 8; i++){
		printf("%2d", i);
	}
	putchar('\n');
	//マスを描画
	for (int i = 0; i < 8; i++){
		//列を描画
		printf("%2d", (i + 1) * 10);
		for (int j = 0; j < 8; j++){
			switch (board[i][j]){
			case NONE: printf("□"); break;
			case BLACK: printf("○"); break;
			case WHITE: printf("●"); break;
			default: printf("\a\a\aerror"); break;
			}
		}
		putchar('\n');
	}
}
 
//有効な場所かどうかを判断（有効な場合は1、有効でない場合は0を返却)
int CanGo(int place, int turn){
 
	//相手の色を定義
	int enemy = EnemyColor(turn);
	//x,yの要素を取り出す
	int x = Trans_x(place);
	int y = Trans_y(place);
 
	//指定された場所がボード上でない
	if (x < 0 || y < 0 || x > 7 || y > 7){
		return(0);
	}
 
	//指定された場所に既に駒がある場合
	if (board[y][x] != NONE){
		return(0);
	}
 
	for (int i = 0; i < 8; i++){
		//周りに相手の駒があるかどうかを判別
		if (board[y + vec_y[i]][x + vec_x[i]] == enemy){
 
			//ベクトルの成分をコピー
			int vec_xx = vec_x[i];
			int vec_yy = vec_y[i];
 
			//その方向を詳しく調べる
			while (1){
				//ベクトルを１つ伸ばす
				vec_xx = StretchVec(vec_xx);
				vec_yy = StretchVec(vec_yy);
 
				//探索ベクトルがボード外
				if ((x + vec_xx) < 0 || (y + vec_yy) < 0 || (x + vec_xx) > 7 || (y + vec_yy) > 7){
					break;
				}//空きマスに出た
				else if (board[y + vec_yy][x + vec_xx] == NONE){
					break;
				}//自分の駒に出た
				else if (board[y + vec_yy][x + vec_xx] == turn){
					return(1);
				}
			}
		}
	}
	return(0);
}
 
//駒を置けるかどうかを判別（置ける場合は1、置けない場合は0を返却）
int CanPut(int turn){
 
	//総当りで解析
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (CanGo((i + 1) * 10 + (j + 1), turn) == 1){
				return(1);
			}
		}
	}
	return(0);
}
 
//マスの状態の数
void Count(const int ThinkBoard[8][8]){
 
	//マスの状態の数をリセット
	NONE_NUMBER = 0;
	BLACK_NUMBER = 0;
	WHITE_NUMBER = 0;
 
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			switch (ThinkBoard[i][j]){
			case NONE: ++NONE_NUMBER; break;
			case BLACK: ++BLACK_NUMBER; break;
			case WHITE: ++WHITE_NUMBER; break;
			default: printf("\a\a\aerror"); break;
			}
		}
	}
}
 
//駒を裏返す
void Flip(int place, int turn){
 
	//相手の色を定義
	int enemy = EnemyColor(turn);
	//x,yの要素を取り出す
	int x = Trans_x(place);
	int y = Trans_y(place);
 
	//指定した場所に駒を置く
	board[y][x] = turn;
 
	for (int i = 0; i < 8; i++){
		//周りに相手の駒があるかどうかを判別
		if (board[y + vec_y[i]][x + vec_x[i]] == enemy){
 
			//ベクトルの成分をコピー
			int vec_xx = vec_x[i];
			int vec_yy = vec_y[i];
			//マスをカウントする変数
			int count = 0;
 
			//その方向を詳しく調べる
			while (1){
 
				//探索ベクトルがボード外
				if ((x + vec_xx) < 0 || (y + vec_yy) < 0 || (x + vec_xx) > 7 || (y + vec_yy) > 7){
					break;
				}//空きマスに出た
				else if (board[y + vec_yy][x + vec_xx] == NONE){
					break;
				}//自分の駒に出た
				else if (board[y + vec_yy][x + vec_xx] == turn){
					//ベクトルの成分をコピー
					int vec_xxx = vec_x[i];
					int vec_yyy = vec_y[i];
					//裏返す
					for (int j = 0; j < count; j++){
						//マスの書き換え
						board[y + vec_yyy][x + vec_xxx] = turn;
						//ベクトルを１つ伸ばす
						vec_xxx = StretchVec(vec_xxx);
						vec_yyy = StretchVec(vec_yyy);
					}
					break;
				}
				//ベクトルを１つ伸ばす
				vec_xx = StretchVec(vec_xx);
				vec_yy = StretchVec(vec_yy);
				//カウントアップ
				count++;
			}
		}
	}
}
 
//ターンを入れ替える
void Change(void){
 
	if (turn == 1){
		turn = 2;
	}
	else{
		turn = 1;
	}
}
 
//勝敗を決定
void Judge(void){
 
	//ボード上をカウント
	Count(board);
 
	printf("黒駒の数：%d個\n", BLACK_NUMBER);
	printf("白駒の数：%d個\n\n", WHITE_NUMBER);
 
	if (BLACK_NUMBER > WHITE_NUMBER){
		if (human == BLACK)    printf("人間の勝ちです。\n\n");
		if (computer == BLACK) printf("コンピュータの勝ちです。\n\n");
	}
	else if (WHITE_NUMBER > BLACK_NUMBER){
		if (human == WHITE)    printf("人間の勝ちです。\n\n");
		if (computer == WHITE) printf("コンピュータの勝ちです。\n\n");
	}
	else{
		printf("引き分けです。\n\n");
	}
 
}

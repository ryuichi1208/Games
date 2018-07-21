
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
88
89
90
91
92
93
94
95
96
97
98
99
100
101
102
103
104
105
106
107
108
109
110
111
112
113
114
115
116
117
118
119
120
121
122
123
124
125
126
127
128
129
130
131
132
133
134
135
136
137
138
139
140
141
142
143
144
145
146
147
148
149
150
151
152
153
154
155
156
157
158
159
160
161
162
163
164
165
166
167
168
169
170
171
172
173
174
175
176
177
178
179
180
181
182
183
184
185
186
187
188
189
190
191
192
193
194
195
196
197
198
199
200
201
202
203
204
205
206
207
208
209
210
211
212
213
214
215
216
217
218
219
220
221
222
223
224
225
226
227
228
229
230
231
232
233
234
235
236
237
238
239
240
241
242
243
244
245
246
247
248
249
250
251
252
253
254
255
256
257
258
259
260
261
262
263
264
265
266
267
268
269
270
271
272
273
274
275
276
277
278
279
280
281
282
283
284
285
286
287
288
289
290
291
292
293
294
295
296
297
298
299
300
301
302
303
304
305
306
307
308
309
310
311
312
313
314
315
316
317
318
319
320
321
322
323
324
325
326
327
328
329
330
331
332
333
334
335
336
337
338
339
340
341
342
343
344
345
346
347
348
349
350
351
352
353
354
355
356
357
358
359
360
361
362
363
364
365
366
367
368
369
370
371
372
373
374
375
376
377
378
379
380
381
382
383
384
385
386
387
388
389
390
391
392
393
394
395
396
397
398
399
400
401
402
403
404
405
406
407
408
409
410
411
412
413
414
415
416
417
418
419
420
421
422
423
424
425
426
427
428
429
430
431
432
433
434
435
436
437
438
439
440
441
442
443
444
445
446
447
448
449
450
451
452
453
454
455
456
457
458
459
460
461
462
463
464
465
466
467
468
469
470
471
472
473
474
475
476
477
478
479
480
481
482
483
484
485
486
487
488
489
490
491
492
493
494
495
496
497
498
499
500
501
502
503
504
505
506
507
508
509
510
511
512
513
514
515
516
517
518
519
520
521
522
523
524
525
526
527
528
529
530
531
532
533
534
535
536
537
538
539
540
541
542
543
544
545
546
547
548
549
550
551
552
553
554
555
556
557
558
559
560
561
562
563
564
565
566
567
568
569
570
571
572
573
574
575
576
577
578
579
580
581
582
583
584
585
586
587
588
589
590
591
592
593
594
595
596
597
598
599
600
601
602
603
604
605
606
607
608
609
610
611
612
613
614
615
616
617
618
619
620
621
622
623
624
625
626
627
628
629
630
631
632
633
634
635
636
637
638
639
640
641
642
643
644
645
646
647
648
649
650
651
652
653
654
655
656
657
658
659
660
661
662
663
664
665
666
667
668
669
670
671
672
673
674
675
676
677
678
679
680
681
682
683
684
685
686
687
688
689
690
691
692
693
694
695
696
697
698
699
700
701
702
703
704
705
706
707
708
709
710
711
712
713
714
715
716
717
718
719
720
721
722
723
724
725
726
727
728
729
730
731
732
733
734
735
736
737
738
739
740
741
742
743
744
745
746
747
748
749
750
751
752
753
754
755
756
757
758
759
760
761
762
763
764
765
766
767
768
769
770
771
772
773
774
775
776
777
778
779
780
781
782
783
784
785
786
787
788
789
790
791
792
793
794
795
796
797
798
799
800
801
802
803
804
805
806
807
808
809
810
811
812
813
814
815
816
817
818
819
820
821
822
823
824
825
826
827
828
829
830
831
832
833
834
835
836
837
838
839
840
841
842
843
844
845
846
847
848
849
850
851
852
853
854
855
856
857
858
859
860
861
862
863
864
865
866
867
868
869
870
871
872
873
874
875
876
877
878
879
880
881
882
883
884
885
886
887
888
889
890
891
892
893
894
895
896
897
898
899
900
901
902
903
904
905
906
907
908
909
910
911
912
913
914
915
916
917
918
919
920
921
922
923
924
925
926
927
928
929
930
931
932
933
934
935
936
937
938
939
940
941
942
943
944
945
946
947
948
949
950
951
952
953
954
955
956
957
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
#define FINAL 0   //11 - 0マスまでが終盤
 
//グローバル変数の定義
int w[EVALUATION_POINTS];                                //各々の評価関数のウェイト
//序盤、中盤、終盤の各々の評価関数のパラメーター
//左から【自分の着手可能手数】、【自分のマスの評価値の合計】、【自分の駒の数】、【自分の確定石の数】、【自分のウイングの数】、【自分の山の数】
const int w_first[EVALUATION_POINTS] = { 3, 8, 0, 5, -5, 10 };      //序盤
const int w_middle[EVALUATION_POINTS] = { 6, 10, 0, 15, -15, 15 };  //中盤
const int w_final[EVALUATION_POINTS] = { 3, 5, 10, 3, 0, 0 };       //終盤
 
 
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
 
 
 
 
 
/******************************************************************************** 評価関数 ******************************************************************************************/
 
//評価値の合計を計算
int Evalu_Total(const int ThinkBoard[8][8], int turn){
 
	//評価値の合計
	int totalscore = 0;
 
	//評価関数のパラメーターを変化させる
	ChangeStage(ThinkBoard);
 
	//各々の評価値
	//【自分の着手可能手数】、【相手の着手可能手数】、【自分のマスの評価値の合計】、【自分の駒の数】、【自分の確定石の数】、【自分のウイングの数】、【自分の山の数】
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
 
 
/******************************************************************************** 探索アルゴリズム ******************************************************************************************/
 
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
				if (CanPut(EnemyColor(ThinkTurn)))    var = minimax(ThinkBoard, EnemyColor(ThinkTurn), depth - 1); //着手可能な場所がある
				if (!(CanPut(EnemyColor(ThinkTurn)))) var = minimax(ThinkBoard, ThinkTurn, depth - 1);             //着手可能な場所がない
 
 
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
 
 
 
 
/********************************************************* 根幹部分の関数（基本的に改変する必要が無い） ******************************************************************************************/
 
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

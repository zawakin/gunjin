#include <stdio.h>
#include <string.h>


#define READDAT(X) fread(&X, sizeof(X), 1, dat)
#define WRITEDAT(X) fwrite(&X, sizeof(X), 1, dat)


typedef struct {
	char name[32] ;
	int direction[4] ;   /* right, front, left, back */
	int strength[32] ;
	int number ;
} opt ;


void initialdata(void) ;  /* 初期設定データの作成 */
int loadoption(int *classnum, int *height, int *width, opt option[], char datname[]) ;  /* 設定の読み込み */ /* return = -1: 読み込み失敗 */
void saveoption(int classnum, int height, int width, opt option[], char datname[]) ;  /* 設定の保存 */
void view(int classnum, int board[][16], int turn, int height, int width) ;  /* 盤面の表示 */
void setting(int classnum, int board[][16], int turn, int height, int width, opt option[]) ;  /* 配置 */
int movabledomain(int classnum, int pro[], int board[][16], int domain[][16], int turn, int height, int width, opt option[]) ;  /* 移動可能領域 */ /* return = -1: 無効な選択 */
void emphasizedview(int classnum, int board[][16], int domain[][16], int turn, int height, int width) ;  /* 移動可能領域の表示 */
int movement(int classnum, int pro[], int post[], int board[][16], int domain[][16], int height, int width, opt option[]) ;  /* 移動 */ /* return = -1: 無効な移動 */
void fight(int classnum, int pro[], int post[], int board[][16], int height, int width, opt option[]) ;  /* 戦闘 */
int victory(int classnum, int board[][16], int height, int width) ;  /* return = 1: 先攻の勝利, 2: 後攻の勝利, 3: 引き分け */
void allview(int classnum, int board[][16], int turn, int height, int width) ;  /* 全駒の表示 */





int main(void)
{
	char datname[64] ;
	char firstname[64] ;
	char secondname[64] ;
	int classnum ;
	int height, width ;
	opt option[32] ;
	int board[16][16] ;
	int domain[16][16] ;
	int turn ;
	int pro[2], post[2] ;
	int vic = 0 ;
	int error ;
	
	printf("先攻の名前を入力してください。\n\n") ;
	scanf_s_s("%s", firstname) ;
	printf("\n\n後攻の名前を入力してください。\n\n") ;
	scanf_s("%s", secondname) ;
	
	strcpy_s(datname, "initialoption.dat") ;
	
	loadoption(&classnum, &height, &width, option, datname) ;
	
	turn = 1 ;
	printf("\n\n%sさんは駒を配置してください。\n\n", firstname) ;
	setting(classnum, board, turn, height, width, option) ;
	turn = 2 ;
	printf("\n\n%sさんは駒を配置してください。\n\n", secondname) ;
	setting(classnum, board, turn, height, width, option) ;
	
	printf("ゲームを開始します。\n\n") ;
	turn = 1 ;
	do {
		if (turn % 2 == 1){
			printf("\n\n%sさんの番です。\n\n", firstname) ;
		} else {
			printf("\n\n%sさんの番です。\n\n", secondname) ;
		}	
		do {
			view(classnum, board, turn, height, width) ;			
			scanf_s("%d", &pro[0]) ;
			scanf_s("%d", &pro[1]) ;
			pro[0]-- ;
			pro[1]-- ;
			error = movabledomain(classnum, pro, board, domain, turn, height, width, option) ;
		} while (error == -1) ;
		do {
			emphasizedview(classnum, board, domain, turn, height, width) ;
			scanf_s("%d", &post[0]) ;
			scanf_s("%d", &post[1]) ;
			post[0]-- ;
			post[1]-- ;
			error = movement(classnum, pro, post, board, domain, height, width, option) ;
		} while (error == -1) ;
		vic = victory(classnum, board, height, width) ;
	} while (vic == 0) ;
	
	if (vic == 1){
		printf("\n\n%sさんの勝利です。\n\n", firstname) ;
	} else if (vic == 2){
		printf("\n\n%sさんの勝利です。\n\n", secondname) ;
	} else {
		printf("\n\n引き分けです。\n\n") ;
	}
	allview(classnum, board, turn, height, width) ;
	return 0;
}


int victory(int classnum, int board[][16], int height, int width)
{
	int first, second ;
	int i, j ;
	int vic = 0 ;
	
	if (board[width/2-1][0] >= 1 && board[width/2-1][0] <= 6){
		vic = 1 ;
	} else if (board[width/2][height-1] >= 1+classnum && board[width/2][height-1] <= 6+classnum){
		vic = 2 ;
	}
	
	first = 0 ;
	second = 0 ;
	for (i = 0 ; i < width ; i++){
		for (j = 0 ; j < height ; j++){
			if (board[i][j] > 0 && board[i][j] <= classnum){
				first++ ;
			} else if (board[i][j] > classnum){	
				second++ ;
			}	
		}
	}
	if (first > 0 && second == 0){
		vic = 1 ;
	}else if (first == 0 && second > 0){
		vic = 2 ;
	}else if (first == 0 && second == 0){
		vic = 3 ;
	}
	return vic ;
}


void allview(int classnum, int board[][16], int turn, int height, int width)
{
	int i, j ;
	
	if (turn % 2 == 1){
		for (i = 0 ; i < width ; i++){
			printf("  %2d  ", width-i) ;
		}
		printf("\n") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;	
		for (i = 0 ; i < width/2-1 ; i++){
			printf("| %2d  ", board[width-i-1][0]) ;
		}
		printf("|     %2d    ", board[width/2-1][0]) ;
		for (i = width/2+1 ; i < width ; i++){
			printf("| %2d  ", board[width-i-1][0]) ;
		}
		printf("| %2d\n", 1) ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		for (j = 1 ; j < height/2 ; j++){
			for (i = 0 ; i < width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;	
			for (i = 0 ; i < width ; i++){
				printf("| %2d  ", board[width-i-1][j]) ;
			}
			printf("| %2d\n", j+1) ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		}
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("|     |     |     ") ;
		for (i = 3 ; i < width-2 ; i++){
			printf("      ") ;
		}
		printf("|     |     |\n") ;
		for (j = height/2 ; j < height-1 ; j++){
			for (i = 0 ; i < width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;	
			for (i = 0 ; i < width ; i++){
				printf("| %2d  ", board[width-i-1][j]) ;
			}
			printf("| %2d\n", j+1) ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		}
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;	
		for (i = 0 ; i < width/2-1 ; i++){
			printf("| %2d  ", board[width-i-1][height-1]) ;
		}
		printf("|     %2d    ", board[width/2][height-1]) ;
		for (i = width/2+1 ; i < width ; i++){
			printf("| %2d  ", board[width-i-1][height-1]) ;
		}
		printf("| %2d\n", height) ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n\n") ;
	} else {
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		printf("%2d  ", height) ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (board[i][height-1] > classnum){
				printf("| %2d  ", board[i][height-1]-classnum) ;
			} else if (board[i][height-1] == 0){
				printf("|  0  ") ;
			} else {
				printf("| %2d  ", board[i][height-1]+classnum) ;
			}
		}
		if (board[width/2][height-1] > classnum){
			printf("|    %2d     ", board[width/2][height-1]-classnum) ;
		} else if (board[width/2][height-1] == 0){
			printf("|     0     ") ;
		} else {
			printf("|    %2d     ", board[width/2][height-1]+classnum) ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (board[i][height-1] > classnum){
				printf("| %2d  ", board[i][height-1]-classnum) ;
			} else if (board[i][height-1] == 0){
				printf("|  0  ") ;
			} else {
				printf("| %2d  ", board[i][height-1]+classnum) ;
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		for (j = 1 ; j < height/2 ; j++){
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
			printf("%2d  ", height-j) ;
			for (i = 0 ; i < width ; i++){
				if (board[i][height-j-1] > classnum){
					printf("| %2d  ", board[i][height-j-1]-classnum) ;
				} else if (board[i][height-j-1] == 0){
					printf("|  0  ") ;
				} else {
					printf("| %2d  ", board[i][height-j-1]+classnum) ;
				}
			}
			printf("|\n") ;
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		}
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		printf("|     |     |     ") ;
		for (i = 3 ; i < width-2 ; i++){
			printf("      ") ;
		}
		printf("|     |     |\n") ;		
		for (j = height/2 ; j < height-1 ; j++){
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
			printf("%2d  ", height-j) ;
			for (i = 0 ; i < width ; i++){
				if (board[i][height-j-1] > classnum){
					printf("| %2d  ", board[i][height-j-1]-classnum) ;
				} else if (board[i][height-j-1] == 0){
					printf("|  0  ") ;
				} else {
					printf("| %2d  ", board[i][height-j-1]+classnum) ;
				}
			}
			printf("|\n") ;
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;			
		}
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		printf("%2d  ", 1) ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (board[i][0] > classnum){
				printf("| %2d  ", board[i][0]-classnum) ;
			} else if (board[i][0] == 0){
				printf("|  0  ") ;
			} else {
				printf("| %2d  ", board[i][0]+classnum) ;
			}
		}
		if (board[width/2-1][0] > classnum){
			printf("|    %2d     ", board[width/2-1][0]-classnum) ;
		} else if (board[width/2-1][0] == 0){
			printf("|     0     ") ;
		} else {
			printf("|    %2d     ", board[width/2-1][0]+classnum) ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (board[i][0] > classnum){
				printf("| %2d  ", board[i][0]-classnum) ;
			} else if (board[i][0] == 0){
				printf("|  0  ") ;
			} else {
				printf("| %2d  ", board[i][0]+classnum) ;
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("  %2d  ", i+1) ;
		}
		printf("\n\n") ;
	}
	return ;
}


void view(int classnum, int board[][16], int turn, int height, int width)
{
	int i, j ;
	
	if (turn % 2 == 1){
		for (i = 0 ; i < width ; i++){
			printf("  %2d  ", width-i) ;
		}
		printf("\n") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;	
		for (i = 0 ; i < width/2-1 ; i++){
			if (board[width-i-1][0] <= classnum){
				printf("| %2d  ", board[width-i-1][0]) ;
			} else {
				printf("| ??? ") ;
			}
		}
		if (board[width/2-1][0] <= classnum){
			printf("|    %2d     ", board[width/2-1][0]) ;
		} else {
			printf("|    ???    ") ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (board[width-i-1][0] <= classnum){
				printf("| %2d  ", board[width-i-1][0]) ;
			} else {
				printf("| ??? ") ;
			}
		}
		printf("| %2d\n", 1) ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;	
		for (j = 1 ; j < height/2 ; j++){
			for (i = 0 ; i < width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;	
			for (i = 0 ; i < width ; i++){
				if (board[width-i-1][j] <= classnum){
					printf("| %2d  ", board[width-i-1][j]) ;
				} else {
					printf("| ??? ") ;
				}
			}
			printf("| %2d\n", j+1) ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		}
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("|     |     |     ") ;
		for (i = 3 ; i < width-2 ; i++){
			printf("      ") ;
		}
		printf("|     |     |\n") ;
		for (j = height/2 ; j < height-1 ; j++){
			for (i = 0 ; i < width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;	
			for (i = 0 ; i < width ; i++){
				if (board[width-i-1][j] <= classnum){
					printf("| %2d  ", board[width-i-1][j]) ;
				} else {
					printf("| ??? ") ;
				}
			}
			printf("| %2d\n", j+1) ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		}
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;	
		for (i = 0 ; i < width/2-1 ; i++){
			if (board[width-i-1][height-1] <= classnum){
				printf("| %2d  ", board[width-i-1][height-1]) ;
			} else {
				printf("| ??? ") ;
			}
		}
		if (board[width/2][height-1] <= classnum){
			printf("|    %2d     ", board[width/2][height-1]) ;
		} else {
			printf("|    ???    ") ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (board[width-i-1][height-1] <= classnum){
				printf("| %2d  ", board[width-i-1][height-1]) ;
			} else {
				printf("| ??? ") ;
			}
		}
		printf("| %2d\n", height) ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n\n") ;
	} else {
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		printf("%2d  ", height) ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (board[i][height-1] > classnum){
				printf("| %2d  ", board[i][height-1]-classnum) ;
			} else if (board[i][height-1] == 0){
				printf("|  0  ") ;
			} else {
				printf("| ??? ") ;
			}
		}
		if (board[width/2][height-1] > classnum){
			printf("|    %2d     ", board[width/2][height-1]-classnum) ;
		} else if (board[width/2][height-1] == 0){
			printf("|     0     ") ;
		} else {
			printf("|    ???    ") ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (board[i][height-1] > classnum){
				printf("| %2d  ", board[i][height-1]-classnum) ;
			} else if (board[i][height-1] == 0){
				printf("|  0  ") ;
			} else {
				printf("| ??? ") ;
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		for (j = 1 ; j < height/2 ; j++){
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
			printf("%2d  ", height-j) ;
			for (i = 0 ; i < width ; i++){
				if (board[i][height-j-1] > classnum){
					printf("| %2d  ", board[i][height-j-1]-classnum) ;
				} else if (board[i][height-j-1] == 0){
					printf("|  0  ") ;
				} else {
					printf("| ??? ") ;
				}
			}
			printf("|\n") ;
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		}
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		printf("|     |     |     ") ;
		for (i = 3 ; i < width-2 ; i++){
			printf("      ") ;
		}
		printf("|     |     |\n") ;		
		for (j = height/2 ; j < height-1 ; j++){
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
			printf("%2d  ", height-j) ;
			for (i = 0 ; i < width ; i++){
				if (board[i][height-j-1] > classnum){
					printf("| %2d  ", board[i][height-j-1]-classnum) ;
				} else if (board[i][height-j-1] == 0){	
					printf("|  0  ") ;
				} else {
					printf("| ??? ") ;
				}
			}
			printf("|\n") ;
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		}
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		printf("%2d  ", 1) ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (board[i][0] > classnum){
				printf("| %2d  ", board[i][0]-classnum) ;
			} else if (board[i][0] == 0){
				printf("|  0  ") ;
			} else {
				printf("| ??? ") ;
			}
		}
		if (board[width/2-1][0] > classnum){
			printf("|    %2d     ", board[width/2-1][0]-classnum) ;
		} else if (board[width/2-1][0] == 0){
			printf("|     0     ") ;
		} else {
			printf("|    ???    ") ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (board[i][0] > classnum){
				printf("| %2d  ", board[i][0]-classnum) ;
			} else if (board[i][0] == 0){
				printf("|  0  ") ;
			} else {
				printf("| ??? ") ;
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = width/2+1 ; i < width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("  %2d  ", i+1) ;
		}
		printf("\n\n") ;
	}
	return ;
}

void emphasizedview(int classnum, int board[][16], int domain[][16], int turn, int height, int width)
{
	int i, j ;
	
	if (turn % 2 == 1){
		for (i = 0 ; i < width ; i++){
			printf("  %2d  ", width-i) ;
		}
		printf("\n") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (domain[width-i-1][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[width/2-1][0] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (domain[width-i-1][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;	
		for (i = 0 ; i < width/2-1 ; i++){
			if (domain[width-i-1][0] == 1){
				if (board[width-i-1][0] <= classnum){
					printf("|*%2d**", board[width-i-1][0]) ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[width-i-1][0] <= classnum){
					printf("| %2d  ", board[width-i-1][0]) ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		if (domain[width/2-1][0] == 1){
			if (board[width/2-1][0] <= classnum){
				printf("|****%2d*****", board[width/2-1][0]) ;
			} else {
				printf("|****???****") ;
			}
		} else {
			if (board[width/2-1][0] <= classnum){
				printf("|    %2d     ", board[width/2-1][0]) ;
			} else {
				printf("|    ???    ") ;
			}
		}
		for (i = width/2+1 ; i < width ; i++){
			if (domain[width-i-1][0] == 1){
				if (board[width-i-1][0] <= classnum){
					printf("|*%2d**", board[width-i-1][0]) ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[width-i-1][0] <= classnum){
					printf("| %2d  ", board[width-i-1][0]) ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		printf("| %2d\n", 1) ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (domain[width-i-1][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[width/2-1][0] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (domain[width-i-1][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;
		for (j = 1 ; j < height/2 ; j++){
			for (i = 0 ; i < width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			for (i = 0 ; i < width ; i++){
				if (domain[width-i-1][j] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;	
			for (i = 0 ; i < width ; i++){
				if (domain[width-i-1][j] == 1){
					if (board[width-i-1][j] <= classnum){
						printf("|*%2d**", board[width-i-1][j]) ;
					} else {
						printf("|*???*") ;
					}
				} else {
					if (board[width-i-1][j] <= classnum){
						printf("| %2d  ", board[width-i-1][j]) ;
					} else {
						printf("| ??? ") ;
					}
				}
			}
			printf("| %2d\n", j+1) ;
			for (i = 0 ; i < width ; i++){
				if (domain[width-i-1][j] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;
		}
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("|     |     |     ") ;
		for (i = 3 ; i < width-2 ; i++){
			printf("      ") ;
		}
		printf("|     |     |\n") ;
		for (j = height/2 ; j < height-1 ; j++){
			for (i = 0 ; i < width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			for (i = 0 ; i < width ; i++){
				if (domain[width-i-1][j] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;	
			for (i = 0 ; i < width ; i++){
				if (domain[width-i-1][j] == 1){
					if (board[width-i-1][j] <= classnum){
						printf("|*%2d**", board[width-i-1][j]) ;
					} else {
						printf("|*???*") ;
					}
				} else {
					if (board[width-i-1][j] <= classnum){
						printf("| %2d  ", board[width-i-1][j]) ;
					} else {
						printf("| ??? ") ;
					}					
				}
			}
			printf("| %2d\n", j+1) ;
			for (i = 0 ; i < width ; i++){
				if (domain[width-i-1][j] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;	
		}
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (domain[width-i-1][height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[width/2][height-1] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (domain[width-i-1][height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;	
		for (i = 0 ; i < width/2-1 ; i++){
			if (domain[width-i-1][height-1] == 1){
				if (board[width-i-1][height-1] <= classnum){
					printf("|*%2d**", board[width-i-1][height-1]) ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[width-i-1][height-1] <= classnum){
					printf("| %2d  ", board[width-i-1][height-1]) ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		if (domain[width/2][height-1] == 1){
			if (board[width/2][height-1] <= classnum){
				printf("|****%2d*****", board[width/2][height-1]) ;
			} else {
				printf("|****???****") ;
			}
		} else {
			if (board[width/2][height-1] <= classnum){
				printf("|    %2d     ", board[width/2][height-1]) ;
			} else {
				printf("|    ???    ") ;
			}
		}
		for (i = width/2+1 ; i < width ; i++){
			if (domain[width-i-1][height-1] == 1){
				if (board[width-i-1][height-1] <= classnum){
					printf("|*%2d**", board[width-i-1][height-1]) ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[width-i-1][height-1] <= classnum){
					printf("| %2d  ", board[width-i-1][height-1]) ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		printf("| %2d\n", height) ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (domain[width-i-1][height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[width/2][height-1] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (domain[width-i-1][height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;	
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n\n") ;
	} else {
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (domain[i][height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[width/2][height-1] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (domain[i][height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;
		printf("%2d  ", height) ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (domain[i][height-1] == 1){
				if (board[i][height-1] > classnum){
					printf("|*%2d**", board[i][height-1]-classnum) ;
				} else if (board[i][height-1] == 0){
					printf("|**0**") ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[i][height-1] > classnum){
					printf("| %2d  ", board[i][height-1]-classnum) ;
				} else if (board[i][height-1] == 0){	
					printf("|  0  ") ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		if (domain[width/2][height-1] == 1){
			if (board[width/2][height-1] > classnum){
				printf("|*%2d**", board[width/2][height-1]-classnum) ;
			} else if (board[width/2][height-1] == 0){
				printf("|**0**") ;
			} else {
				printf("|*???*") ;
			}
		} else {
			if (board[width/2][height-1] > classnum){
				printf("| %2d  ", board[width/2][height-1]-classnum) ;
			} else if (board[width/2][height-1] == 0){	
				printf("|  0  ") ;
			} else {
				printf("| ??? ") ;
			}
		}
		for (j = width/2+1 ; j < width ; j++){
			if (domain[i][height-1] == 1){
				if (board[i][height-1] > classnum){
					printf("|*%2d**", board[i][height-1]-classnum) ;
				} else if (board[i][height-1] == 0){
					printf("|**0**") ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[i][height-1] > classnum){
					printf("| %2d  ", board[i][height-1]-classnum) ;
				} else if (board[i][height-1] == 0){	
					printf("|  0  ") ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (domain[i][height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[width/2][height-1] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (domain[i][height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;
		for (j = 1 ; j < height/2 ; j++){
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				if (domain[i][height-j-1] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;
			printf("%2d  ", height-j) ;
			for (j = 0 ; j < width ; j++){
				if (domain[i][height-j-1] == 1){
					if (board[i][height-j-1] > classnum){
						printf("|*%2d**", board[i][height-j-1]-classnum) ;
					} else if (board[i][height-j-1] == 0){
						printf("|**0**") ;
					} else {
						printf("|*???*") ;
					}
				} else {
					if (board[i][height-j-1] > classnum){
						printf("| %2d  ", board[i][height-j-1]-classnum) ;
					} else if (board[i][height-j-1] == 0){	
						printf("|  0  ") ;
					} else {
						printf("| ??? ") ;
					}
				}
			}
			printf("|\n") ;
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				if (domain[i][height-i-1] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;
		}
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		printf("|     |     |     ") ;
		for (i = 3 ; i < width-2 ; i++){
			printf("      ") ;
		}
		printf("|     |     |\n") ;		
		for (j = height/2 ; j < height-1 ; j++){
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				if (domain[i][height-j-1] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;
			printf("%2d  ", height-j) ;
			for (i = 0 ; i < width ; i++){
				if (domain[i][height-j-1] == 1){
					if (board[i][height-j-1] > classnum){
						printf("|*%2d**", board[i][height-j-1]-classnum) ;
					} else if (board[i][height-j-1] == 0){
						printf("|**0**") ;
					} else {
						printf("|*???*") ;
					}
				} else {
					if (board[i][height-j-1] > classnum){
						printf("| %2d  ", board[i][height-j-1]-classnum) ;
					} else if (board[i][height-j-1] == 0){	
						printf("|  0  ") ;
					} else {
						printf("| ??? ") ;
					}
				}
			}
			printf("|\n") ;
			printf("    ") ;
			for (i = 0 ; i < width ; i++){
				if (domain[i][height-j-1] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;
		}
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (domain[i][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[width/2-1][0] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (domain[i][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;
		printf("%2d  ", 1) ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (domain[i][0] == 1){
				if (board[i][0] > classnum){
					printf("|*%2d**", board[i][0]-classnum) ;
				} else if (board[i][0] == 0){
					printf("|**0**") ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[i][0] > classnum){
					printf("| %2d  ", board[i][0]-classnum) ;
				} else if (board[i][0] == 0){	
					printf("|  0  ") ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		if (domain[width/2-1][0] == 1){
			if (board[width/2-1][0] > classnum){
				printf("|*%2d**", board[width/2-1][0]-classnum) ;
			} else if (board[width/2-1][0] == 0){
				printf("|**0**") ;
			} else {
				printf("|*???*") ;
			}
		} else {
			if (board[width/2-1][0] > classnum){
				printf("| %2d  ", board[width/2-1][0]-classnum) ;
			} else if (board[width/2-1][0] == 0){	
				printf("|  0  ") ;
			} else {
				printf("| ??? ") ;
			}
		}
		for (i = width/2+1 ; i < width ; i++){
			if (domain[i][0] == 1){
				if (board[i][0] > classnum){
					printf("|*%2d**", board[i][0]-classnum) ;
				} else if (board[i][0] == 0){
					printf("|**0**") ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[i][0] > classnum){
					printf("| %2d  ", board[i][0]-classnum) ;
				} else if (board[i][0] == 0){	
					printf("|  0  ") ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < width/2-1 ; i++){
			if (domain[i][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[width/2-1][0] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = width/2+1 ; i < width ; i++){
			if (domain[i][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < width ; i++){
			printf("  %2d  ", i+1) ;
		}
		printf("\n\n") ;
	}
	return ;
}


void setting(int classnum, int board[][16], int turn, int height, int width, opt option[])
{
	int preboard[16][16] ;
	int i, j ;
	int k, l, m ;
	int list[128] ;
	int yes = 0 ;
	int pro, post[2] ;
	int num ;
	
	if (turn % 2 == 1){
		k = 1 ;
		l = 0 ;
		while (k <= classnum){
			m = 1 ;
			while (m <= option[k].number){
				list[l] = k ;
				m++ ;
				l++ ;
			}
			k++ ;
		}
		for (i = 0 ; i < width ; i++){
			for (j = 0 ; j < height/2 ; j++){
				preboard[i][j] = classnum+1 ;
			}
			for (j = height/2 ; j < height ; j++) {
				preboard[i][j] = 0 ;
			}
		}
		do {
			view(classnum, preboard, turn, height, width) ;
			printf("\n\n") ;
			for (k = 0 ; k < l ; k++){
				printf("| %2d  ", list[k]) ;
			}
			printf("|\n\n") ;
			scanf_s("%d", &pro) ;
			scanf_s("%d", &post[0]) ;
			scanf_s("%d", &post[1]) ;
			post[0]-- ;
			post[1]-- ;
			if (post[0] == width/2 && post[1] == 0){
				post[0] = width/2-1 ;
			} else if (post[0] == width/2-1 && post[1] == height-1) {
				post[0] = width/2 ;
			}
			if (((pro >= 0 && pro <= classnum) && ((post[0] >= 0 && post[0] < width) && (post[1] >= height/2 && post[1] < height))) && !((pro == 15 || pro == 16) && ((post[0] == 1 || post[0] == width-2) && post[1] == height/2))){
				for (k = 0 ; k < l ; k++){
					if (list[k] == pro){
						list[k] = preboard[post[0]][post[1]] ;
						preboard[post[0]][post[1]] = pro ;
						break ;
					}
				}
				num = 0 ;
				for (k = 0 ; k < l ; k++){
					if (list[k] > 0){
						num++ ;
					}
				}
				if (num == 0){
					view(classnum, preboard, turn, height, width) ;
					printf("この配置でよろしいですか？  yes: 1, no: 0\n\n") ;
					scanf_s("%d", &yes) ;
				}
			}
		} while (yes == 0) ;
		for (i = 0 ; i < width ; i++){
			for (j = height/2 ; j < height ; j++){
				board[i][j] = preboard[i][j] ;
			}
		}
	} else {
		k = 1 ;
		l = 0 ;
		while (k <= classnum){
			m = 1 ;
			while (m <= option[k].number){
				list[l] = k + classnum ;
				m++ ;
				l++ ;
			}
			k++ ;
		}
		for (i = 0 ; i < width ; i++){
			for (j = 0 ; j < height/2 ; j++){
				preboard[i][j] = 0 ;
			}
			for (j = height/2 ; j < height ; j++) {
				preboard[i][j] = 1 ;
			}
		}
		do {
			view(classnum, preboard, turn, height, width) ;
			printf("\n\n") ;
			for (k = 0 ; k < l ; k++){
				if (list[k] == 0){
					printf("| %2d  ", list[k]) ;
				} else {
					printf("| %2d  ", list[k]-classnum) ;
				}
			}
			printf("|\n\n") ;
			scanf_s("%d", &pro) ;
			scanf_s("%d", &post[0]) ;
			scanf_s("%d", &post[1]) ;
			post[0]-- ;
			post[1]-- ;
			if (post[0] == width/2 && post[1] == 0){	
				post[0] = width/2-1 ;
			} else if (post[0] == width/2-1 && post[1] == height-1) {
				post[0] = width/2 ;
			}			
			if (((pro >= 0 && pro <= classnum) && ((post[0] >= 0 && post[0] < width) && (post[1] >= 0 && post[1] < height/2))) && !((pro == 15 || pro == 16) && ((post[0] == 1 || post[0] == width-2) && post[1] == height/2-1))){
				for (k = 0 ; k < l ; k++){
					if (list[k] == pro+classnum){
						list[k] = preboard[post[0]][post[1]] ;
						preboard[post[0]][post[1]] = pro+classnum ;
						break ;
					} else if (list[k] == pro && pro == 0){
						list[k] = preboard[post[0]][post[1]] ;
						preboard[post[0]][post[1]] = pro ;
						break ;
					}
				}
				num = 0 ;
				for (k = 0 ; k < l ; k++){
					if (list[k] > 0){
						num++ ;
					}
				}
				if (num == 0){
					view(classnum, preboard, turn, height, width) ;
					printf("この配置でよろしいですか？  yes: 1, no: 0\n\n") ;
					scanf_s("%d", &yes) ;
				}
			}
		} while (yes == 0) ;
		for (i = 0 ; i < width ; i++){
			for (j = 0 ; j < height/2 ; j++){
				board[i][j] = preboard[i][j] ;
			}
		}		
	}
	return ;
}



int movabledomain(int classnum, int pro[], int board[][16], int domain[][16], int turn, int height, int width, opt option[])
{
	int i, j ;
	int k, l ;
	int error = -1 ;
	
	for (i = 0 ; i < width ; i++){
		for (j = 0 ; j < height ; j++){
			domain[i][j] = 0 ;
		}
	}	
	
	if ((pro[0] >= 0 && pro[0] < width) && (pro[1] >= 0 && pro[1] < height)){		
		if (pro[0] == width/2 && pro[1] == 0){	
			pro[0] = width/2-1 ;
		} else if (pro[0] == width/2-1 && pro[1] == height-1) {
			pro[0] = width/2 ;
		}
		if (turn % 2 == 1 && (board[pro[0]][pro[1]] > 0 && board[pro[0]][pro[1]] <= classnum)){
			error = 0 ;
			k = 1 ;
			l = 1 ;
			while (k <= option[board[pro[0]][pro[1]]].direction[0] && pro[0]-l >= 0){
				if (board[pro[0]][pro[1]] != 13){
					if ((pro[0]-l == width/2 && pro[1] == 0) || (pro[0]-l == width/2-1 && pro[1] == height-1)){
						l++ ;
					} else {
						if (board[pro[0]-l][pro[1]] == 0){
							domain[pro[0]-l][pro[1]] = 1 ;
						} else if (board[pro[0]-l][pro[1]] > classnum){ 
							domain[pro[0]-l][pro[1]] = 1 ;
							break ;
						} else {
							break ;
						}
						k++ ;
						l++ ;
					}
				} else {
					if ((pro[0]-l == width/2 && pro[1] == 0) || (pro[0]-l == width/2-1 && pro[1] == height-1)){
						l++ ;
					} else {
						if (board[pro[0]-l][pro[1]] = 0 || board[pro[0]-l][pro[1]] > classnum){
							domain[pro[0]-l][pro[1]] = 1 ;
						}
						k++ ;
						l++ ;
					}
				}
			}
			k = 1 ;
			l = 1 ;
			while (k <= option[board[pro[0]][pro[1]]].direction[2] && pro[0]+l < width){
				if (board[pro[0]][pro[1]] != 13){
					if ((pro[0]+l == width/2 && pro[1] == 0) || (pro[0]+l == width/2-1 && pro[1] == height-1)){
						l++ ;
					} else {
						if (board[pro[0]+l][pro[1]] == 0){
							domain[pro[0]+l][pro[1]] = 1 ;
						} else if (board[pro[0]+l][pro[1]] > classnum){ 
							domain[pro[0]+l][pro[1]] = 1 ;
							break ;
						} else {
							break ;
						}
						k++ ;
						l++ ;
					}
				} else {
					if ((pro[0]+l == width/2 && pro[1] == 0) || (pro[0]+l == width/2-1 && pro[1] == height-1)){
						l++ ;
					} else {
						if (board[pro[0]+l][pro[1]] == 0 || board[pro[0]+l][pro[1]] > classnum){
							domain[pro[0]+l][pro[1]] = 1 ;
						}
						k++ ;
						l++ ;
					}
				}
			}
			k = 1 ;
			while (k <= option[board[pro[0]][pro[1]]].direction[1] && pro[1]-k >= 0){
				if (board[pro[0]][pro[1]] != 13){
					if (k == 1 && (pro[0] == width/2 && pro[1] == height-1)){
						l = 1 ;
						while (l <= option[board[pro[0]][pro[1]]].direction[1] && pro[1]-l >= 0){
							if (board[pro[0]-1][pro[1]-l] == 0){
								domain[pro[0]-1][pro[1]-l] = 1 ;
							} else if (board[pro[0]-1][pro[1]-l] > classnum){
								domain[pro[0]-1][pro[1]-l] = 1 ;
								break ;
							} else {
								break ;
							}
							l++ ;
						}
					} else if (!((pro[0] != 1 && pro[0] != width-2) && pro[1]-k == height/2-1)){
						if (board[pro[0]][pro[1]-k] == 0){
							domain[pro[0]][pro[1]-k] = 1 ;
						} else if (board[pro[0]][pro[1]-k] > classnum){ 
							domain[pro[0]][pro[1]-k] = 1 ;
							break ;
						} else {
							break ;
						}
						k++ ;
					} else {
						break ;
					}
				} else {
					if (k == 1 && (pro[0] == width/2 && pro[1] == height-1)){
						l = 1 ;
						while (l <= option[board[pro[0]][pro[1]]].direction[1] && pro[1]-l >= 0){
							if (board[pro[0]-1][pro[1]-l] == 0 || board[pro[0]-1][pro[1]-l] > classnum){
								domain[pro[0]-1][pro[1]-l] = 1 ;
							}
							l++ ;
						}
					} else {
						if (board[pro[0]][pro[1]-k] == 0 || board[pro[0]][pro[1]-k] > classnum){
							domain[pro[0]][pro[1]-k] = 1 ;
						}
						k++ ;
					}
				}
			}
			k = 1 ;
			while (k <= option[board[pro[0]][pro[1]]].direction[3] && pro[1]+k < height){
				if (board[pro[0]][pro[1]] != 13){
					if (k == 1 && (pro[0] == width/2-1 && pro[1] == 0)){
						l = 1 ;
						while (l <= option[board[pro[0]][pro[1]]].direction[3] && pro[1]+l< height){
							if (board[pro[0]+1][pro[1]+l] == 0){
								domain[pro[0]+1][pro[1]+l] = 1 ;
							} else if (board[pro[0]+1][pro[1]+l] > classnum){
								domain[pro[0]+1][pro[1]+l] = 1 ;
								break ;
							} else {
								break ;
							}
							l++ ;
						}
					} else if (!((pro[0] != 1 && pro[0] != width-2) && pro[1]+k == height/2)){
						if (board[pro[0]][pro[1]+k] == 0){
							domain[pro[0]][pro[1]+k] = 1 ;
						} else if (board[pro[0]][pro[1]+k] > classnum){ 
							domain[pro[0]][pro[1]+k] = 1 ;
							break ;
						} else {
							break ;
						}
						k++ ;
					} else {
						break ;
					}
				} else {
					if (k == 1 && (pro[0] == width/2-1 && pro[1] == 0)){
						l = 1 ;
						while (l <= option[board[pro[0]][pro[1]]].direction[3] && pro[1]+l < height){
							if (board[pro[0]+1][pro[1]+l] == 0 || board[pro[0]+1][pro[1]+l] > classnum){
								domain[pro[0]+1][pro[1]+l] = 1 ;
							}
							l++ ;
						}
					} else {
						if (board[pro[0]][pro[1]+k] == 0 || board[pro[0]][pro[1]+k] > classnum){
							domain[pro[0]][pro[1]+k] = 1 ;
						}
						k++ ;
					}
				}
			}
		} else if (turn % 2 == 0 && board[pro[0]][pro[1]] > classnum){
			error = 0 ;
			k = 1 ;
			l = 1 ;
			while (k <= option[board[pro[0]][pro[1]]].direction[2] && pro[0]-l >= 0){
				if (board[pro[0]][pro[1]] != 13){
					if ((pro[0]-l == width/2 && pro[1] == 0) || (pro[0]-l == width/2-1 && pro[1] == height-1)){
						l++ ;
					} else {
						if (board[pro[0]-l][pro[1]] == 0){
							domain[pro[0]-l][pro[1]] = 1 ;
						} else if (board[pro[0]-l][pro[1]] > 0 && board[pro[0]-l][pro[1]] <= classnum){ 
							domain[pro[0]-l][pro[1]] = 1 ;
							break ;
						} else {
							break ;
						}
						k++ ;
						l++ ;
					}
				} else {
					if ((pro[0]-l == width/2 && pro[1] == 0) || (pro[0]-l == width/2-1 && pro[1] == height-1)){
						l++ ;
					} else {
						if (board[pro[0]-l][pro[1]] <= classnum){
							domain[pro[0]-l][pro[1]] = 1 ;
						}
						k++ ;
						l++ ;
					}
				}
			}
			k = 1 ;
			l = 1 ;
			while (k <= option[board[pro[0]][pro[1]]].direction[0] && pro[0]+l < width){
				if (board[pro[0]][pro[1]] != 13){
					if ((pro[0]+l == width/2 && pro[1] == 0) || (pro[0]+l == width/2-1 && pro[1] == height-1)){
						l++ ;
					} else {
						if (board[pro[0]+l][pro[1]] == 0){
							domain[pro[0]+l][pro[1]] = 1 ;
						} else if (board[pro[0]+l][pro[1]] > 0 && board[pro[0]+l][pro[1]] <= classnum){ 
							domain[pro[0]+l][pro[1]] = 1 ;
							break ;
						} else {
							break ;
						}
						k++ ;
						l++ ;
					}
				} else {
					if ((pro[0]+l == width/2 && pro[1] == 0) || (pro[0]+l == width/2-1 && pro[1] == height-1)){
						l++ ;
					} else {
						if (board[pro[0]+l][pro[1]] <= classnum){
							domain[pro[0]+l][pro[1]] = 1 ;
						}
						k++ ;
						l++ ;
					}
				}
			}
			k = 1 ;
			while (k <= option[board[pro[0]][pro[1]]].direction[3] && pro[1]-k >= 0){
				if (board[pro[0]][pro[1]] != 13){
					if (k == 1 && (pro[0] == width/2 && pro[1] == height-1)){
						l = 1 ;
						while (l <= option[board[pro[0]][pro[1]]].direction[3] && pro[1]-l >= 0){
							if (board[pro[0]-1][pro[1]-l] == 0){
								domain[pro[0]-1][pro[1]-l] = 1 ;
							} else if (board[pro[0]-1][pro[1]-l] > 0 && board[pro[0]-1][pro[1]-l] <= classnum){
								domain[pro[0]-1][pro[1]-l] = 1 ;
								break ;
							} else {
								break ;
							}
							l++ ;
						}
					} else if (!((pro[0] != 1 && pro[0] != width-2) && pro[1]-k == height/2-1)){
						if (board[pro[0]][pro[1]-k] == 0){
							domain[pro[0]][pro[1]-k] = 1 ;
						} else if (board[pro[0]][pro[1]-k] > 0 && board[pro[0]][pro[1]-k] <= classnum){ 
							domain[pro[0]][pro[1]-k] = 1 ;
							break ;
						} else {
							break ;
						}
						k++ ;
					} else {
						break ;
					}
				} else {
					if (k == 1 && (pro[0] == width/2 && pro[1] == height-1)){
						l = 1 ;
						while (l <= option[board[pro[0]][pro[1]]].direction[3] && pro[1]-l >= 0){
							if (board[pro[0]-1][pro[1]-l] <= classnum){
								domain[pro[0]-1][pro[1]-l] = 1 ;
							}
							l++ ;
						}
					} else {
						if (board[pro[0]][pro[1]-k] <= classnum){
							domain[pro[0]][pro[1]-k] = 1 ;
						}
						k++ ;
					}
				}
			}
			k = 1 ;
			while (k <= option[board[pro[0]][pro[1]]].direction[1] && pro[1]+k < height){
				if (board[pro[0]][pro[1]] != 13){
					if (k == 1 && (pro[0] == width/2-1 && pro[1] == 0)){
						l = 1 ;
						while (l <= option[board[pro[0]][pro[1]]].direction[1] && pro[1]+l < height){
							if (board[pro[0]+1][pro[1]+l] == 0){
								domain[pro[0]+1][pro[1]+l] = 1 ;
							} else if (board[pro[0]+1][pro[1]+l] > 0 && board[pro[0]+1][pro[1]+l] <= classnum){
								domain[pro[0]+1][pro[1]+l] = 1 ;
								break ;
							} else {
								break ;
							}
							l++ ;
						}
					} else if (!((pro[0] != 1 && pro[0] != width-2) && pro[1]+k == height/2)){
						if (board[pro[0]][pro[1]+k] == 0){
							domain[pro[0]][pro[1]+k] = 1 ;
						} else if (board[pro[0]][pro[1]+k] > 0 && board[pro[0]][pro[1]+k] <= classnum){ 
							domain[pro[0]][pro[1]+k] = 1 ;
							break ;
						} else {
							break ;
						}
						k++ ;
					} else {
						break ;
					}
				} else {
					if (k == 1 && (pro[0] == width/2-1 && pro[1] == 0)){
						l = 1 ;
						while (l <= option[board[pro[0]][pro[1]]].direction[1] && pro[1]+l < height){
							if (board[pro[0]+1][pro[1]+l] <= classnum){
								domain[pro[0]+1][pro[1]+l] = 1 ;
							}
							l++ ;
						}
					} else {
						if (board[pro[0]][pro[1]+k] <= classnum){
							domain[pro[0]][pro[1]+k] = 1 ;
						}
						k++ ;
					}
				}
			}
		}
	}
	return error ;
}





int movement(int classnum, int pro[], int post[], int board[][16], int domain[][16], int height, int width,  opt option[])
{
	int error = -1 ;
	
	if (pro[0] == width/2 && pro[1] == 0){	
		pro[0] = width/2-1 ;
	} else if (pro[0] == width/2-1 && pro[1] == height-1) {
		pro[0] = width/2 ;
	}
	if (post[0] == width/2 && post[1] == 0){	
		post[0] = width/2-1 ;
	} else if (post[0] == width/2-1 && post[1] == height-1) {
		post[0] = width/2 ;
	}
	if (domain[post[0]][post[1]] == 1){
		error = 0 ;
		fight(classnum, pro, post, board, height, width, option) ;
	}
	return error ;
}

void fight(int classnum, int pro[], int post[], int board[][16], int height, int width, opt option[])
{
	if (board[post[0]][post[1]] > classnum){
		if ((board[post[0]][post[1]] == 16+classnum && post[1] > 0) && board[post[0]][post[1]-1] > classnum){
			if (option[board[pro[0]][pro[1]]].strength[board[post[0]][post[1]-1]-classnum] == 1){
				board[post[0]][post[1]] = board[pro[0]][pro[1]] ;
			
			} else if (option[board[pro[0]][pro[1]]].strength[board[post[0]][post[1]-1]-classnum] == 0){
				board[post[0]][post[1]] = 0 ;
			
			}
		} else {
			if (option[board[pro[0]][pro[1]]].strength[board[post[0]][post[1]]-classnum] == 1){
				board[post[0]][post[1]] = board[pro[0]][pro[1]] ;
			
			} else if (option[board[pro[0]][pro[1]]].strength[board[post[0]][post[1]]-classnum] == 0){
				board[post[0]][post[1]] = 0 ;
			
			}
		}
	} else if (board[post[0]][post[1]] > 0 && board[post[0]][post[1]] <= classnum){
		if ((board[post[0]][post[1]] == 16 && post[1] < height-1) && (board[post[0]][post[1]+1] > 0 && board[post[0]][post[1]+1] <= classnum)){
			if (option[board[pro[0]][pro[1]]-classnum].strength[board[post[0]][post[1]+1]] == 1){
				board[post[0]][post[1]] = board[pro[0]][pro[1]] ;
			
			} else if (option[board[pro[0]][pro[1]]-classnum].strength[board[post[0]][post[1]+1]] == 0){
				board[post[0]][post[1]] = 0 ;
				
			}
		} else {
			if (option[board[pro[0]][pro[1]]-classnum].strength[board[post[0]][post[1]]] == 1){
				board[post[0]][post[1]] = board[pro[0]][pro[1]] ;
			
			} else if (option[board[pro[0]][pro[1]]-classnum].strength[board[post[0]][post[1]]] == 0){
				board[post[0]][post[1]] = 0 ;
				
			}
		}
	} else {
		board[post[0]][post[1]] = board[pro[0]][pro[1]] ;
		
	}
	board[pro[0]][pro[1]] = 0 ;
	
	return ;
}




int loadoption(int *classnum, int *height, int *width, opt option[], char datname[])
{
	FILE *dat ;
	int preclassnum, preheight, prewidth ;
	int error = -1 ;
	int n, i ;
	
	dat = fopen(datname, "rb") ;
	
	if (dat != NULL){
		error = 0 ;
		READDAT(preclassnum) ;
		READDAT(preheight) ;
		READDAT(prewidth) ;
		*classnum = preclassnum ;
		*height = preheight ;
		*width = prewidth ;
		
		for (n = 0 ; n <= preclassnum ; n++){
			READDAT(option[n].name) ;
		
			for (i = 0 ; i < 4 ; i++){
				READDAT(option[n].direction[i]) ;
			}
			for (i = 0 ; i <= preclassnum ; i++){
				READDAT(option[n].strength[i]) ;
			}
			
			READDAT(option[n].number) ;
		}
	
		fclose(dat) ;
	}
	return error ;
}

void saveoption(int classnum, int height, int width, opt option[], char datname[])
{
	FILE *dat ;
	
	int n, i ;
	
	dat = fopen(datname, "wb") ;
	
	WRITEDAT(classnum) ;
	WRITEDAT(height) ;
	WRITEDAT(width) ;
	
	for (n = 0 ; n <= classnum ; n++){
		WRITEDAT(option[n].name) ;
		
		for (i = 0 ; i < 4 ; i++){
			WRITEDAT(option[n].direction[i]) ;
		}
		for (i = 0 ; i <= classnum ; i++){
			WRITEDAT(option[n].strength[i]) ;
		}

		WRITEDAT(option[n].number) ;
	}
	
	fclose(dat) ;
	
	return ;
}

void initialdata(void)
{
	char datname[] = "initialoption.dat" ;
	int classnum = 16 ;
	int height = 8 ;
	int width = 6 ;

	opt iniopt[17] ; /* 16 + 1 */
	
	FILE *dat ;
	
	int n, i ;
	
	/* Null */
	strcpy_s(iniopt[0].name, "Null") ;
	
	iniopt[0].direction[0] = 0 ;
	iniopt[0].direction[1] = 0 ;
	iniopt[0].direction[2] = 0 ;
	iniopt[0].direction[3] = 0 ;
	
	iniopt[0].strength[0] = 0 ;
	iniopt[0].strength[1] = -1 ;
	iniopt[0].strength[2] = -1 ;
	iniopt[0].strength[3] = -1 ;
	iniopt[0].strength[4] = -1 ;
	iniopt[0].strength[5] = -1 ;
	iniopt[0].strength[6] = -1 ;
	iniopt[0].strength[7] = -1 ;
	iniopt[0].strength[8] = -1 ;
	iniopt[0].strength[9] = -1 ;
	iniopt[0].strength[10] = -1 ;
	iniopt[0].strength[11] = -1 ;
	iniopt[0].strength[12] = -1 ;
	iniopt[0].strength[13] = -1 ;
	iniopt[0].strength[14] = -1 ;
	iniopt[0].strength[15] = -1 ;
	iniopt[0].strength[16] = -1 ;
	
	iniopt[0].number = 0 ;
	
	/* 大将 */
	strcpy_s(iniopt[1].name, "Gengeral") ;
	
	iniopt[1].direction[0] = 1 ;
	iniopt[1].direction[1] = 1 ;
	iniopt[1].direction[2] = 1 ;
	iniopt[1].direction[3] = 1 ;
	
	iniopt[1].strength[0] = 1 ;
	iniopt[1].strength[1] = 0 ;
	iniopt[1].strength[2] = 1 ;
	iniopt[1].strength[3] = 1 ;
	iniopt[1].strength[4] = 1 ;
	iniopt[1].strength[5] = 1 ;
	iniopt[1].strength[6] = 1 ;
	iniopt[1].strength[7] = 1 ;
	iniopt[1].strength[8] = 1 ;
	iniopt[1].strength[9] = 1 ;
	iniopt[1].strength[10] = 1 ;
	iniopt[1].strength[11] = 1 ;
	iniopt[1].strength[12] = 1 ;
	iniopt[1].strength[13] = 1 ;
	iniopt[1].strength[14] = -1 ;
	iniopt[1].strength[15] = 0 ;
	iniopt[1].strength[16] = 1 ;

	iniopt[1].number = 1 ;	

	/* 中将 */
	strcpy_s(iniopt[2].name, "Lieutenant Gengeral") ;
	
	iniopt[2].direction[0] = 1 ;
	iniopt[2].direction[1] = 1 ;
	iniopt[2].direction[2] = 1 ;
	iniopt[2].direction[3] = 1 ;

	iniopt[2].strength[0] = 1 ;	
	iniopt[2].strength[1] = -1 ;
	iniopt[2].strength[2] = 0 ;
	iniopt[2].strength[3] = 1 ;
	iniopt[2].strength[4] = 1 ;
	iniopt[2].strength[5] = 1 ;
	iniopt[2].strength[6] = 1 ;
	iniopt[2].strength[7] = 1 ;
	iniopt[2].strength[8] = 1 ;
	iniopt[2].strength[9] = 1 ;
	iniopt[2].strength[10] = 1 ;
	iniopt[2].strength[11] = 1 ;
	iniopt[2].strength[12] = 1 ;
	iniopt[2].strength[13] = 1 ;
	iniopt[2].strength[14] = 1 ;
	iniopt[2].strength[15] = 0 ;
	iniopt[2].strength[16] = 1 ;

	iniopt[2].number = 1 ;	
	
	/* 少将 */
	strcpy_s(iniopt[3].name, "Major Gengeral") ;
	
	iniopt[3].direction[0] = 1 ;
	iniopt[3].direction[1] = 1 ;
	iniopt[3].direction[2] = 1 ;
	iniopt[3].direction[3] = 1 ;

	iniopt[3].strength[0] = 1 ;	
	iniopt[3].strength[1] = -1 ;
	iniopt[3].strength[2] = -1 ;
	iniopt[3].strength[3] = 0 ;
	iniopt[3].strength[4] = 1 ;
	iniopt[3].strength[5] = 1 ;
	iniopt[3].strength[6] = 1 ;
	iniopt[3].strength[7] = 1 ;
	iniopt[3].strength[8] = 1 ;
	iniopt[3].strength[9] = 1 ;
	iniopt[3].strength[10] = 1 ;
	iniopt[3].strength[11] = 1 ;
	iniopt[3].strength[12] = 1 ;
	iniopt[3].strength[13] = 1 ;
	iniopt[3].strength[14] = 1 ;
	iniopt[3].strength[15] = 0 ;
	iniopt[3].strength[16] = 1 ;

	iniopt[3].number = 1 ;	
	
	/* 大佐 */
	strcpy_s(iniopt[4].name, "Colonel") ;
	
	iniopt[4].direction[0] = 1 ;
	iniopt[4].direction[1] = 1 ;
	iniopt[4].direction[2] = 1 ;
	iniopt[4].direction[3] = 1 ;

	iniopt[4].strength[0] = 1 ;	
	iniopt[4].strength[1] = -1 ;
	iniopt[4].strength[2] = -1 ;
	iniopt[4].strength[3] = -1 ;
	iniopt[4].strength[4] = 0 ;
	iniopt[4].strength[5] = 1 ;
	iniopt[4].strength[6] = 1 ;
	iniopt[4].strength[7] = 1 ;
	iniopt[4].strength[8] = 1 ;
	iniopt[4].strength[9] = 1 ;
	iniopt[4].strength[10] = -1 ;
	iniopt[4].strength[11] = -1 ;
	iniopt[4].strength[12] = 1 ;
	iniopt[4].strength[13] = 1 ;
	iniopt[4].strength[14] = 1 ;
	iniopt[4].strength[15] = 0 ;
	iniopt[4].strength[16] = 1 ;

	iniopt[4].number = 1 ;
	
	/* 中佐 */
	strcpy_s(iniopt[5].name, "Lieutenant Colonel") ;
	
	iniopt[5].direction[0] = 1 ;
	iniopt[5].direction[1] = 1 ;
	iniopt[5].direction[2] = 1 ;
	iniopt[5].direction[3] = 1 ;

	iniopt[5].strength[0] = 1 ;	
	iniopt[5].strength[1] = -1 ;
	iniopt[5].strength[2] = -1 ;
	iniopt[5].strength[3] = -1 ;
	iniopt[5].strength[4] = -1 ;
	iniopt[5].strength[5] = 0 ;
	iniopt[5].strength[6] = 1 ;
	iniopt[5].strength[7] = 1 ;
	iniopt[5].strength[8] = 1 ;
	iniopt[5].strength[9] = 1 ;
	iniopt[5].strength[10] = -1 ;
	iniopt[5].strength[11] = -1 ;
	iniopt[5].strength[12] = 1 ;
	iniopt[5].strength[13] = 1 ;
	iniopt[5].strength[14] = 1 ;
	iniopt[5].strength[15] = 0 ;
	iniopt[5].strength[16] = 1 ;

	iniopt[5].number = 1 ;	
	
	/* 少佐 */
	strcpy_s(iniopt[6].name, "Major") ;
	
	iniopt[6].direction[0] = 1 ;
	iniopt[6].direction[1] = 1 ;
	iniopt[6].direction[2] = 1 ;
	iniopt[6].direction[3] = 1 ;

	iniopt[6].strength[0] = 1 ;
	iniopt[6].strength[1] = -1 ;
	iniopt[6].strength[2] = -1 ;
	iniopt[6].strength[3] = -1 ;
	iniopt[6].strength[4] = -1 ;
	iniopt[6].strength[5] = -1 ;
	iniopt[6].strength[6] = 0 ;
	iniopt[6].strength[7] = 1 ;
	iniopt[6].strength[8] = 1 ;
	iniopt[6].strength[9] = 1 ;
	iniopt[6].strength[10] = -1 ;
	iniopt[6].strength[11] = -1 ;
	iniopt[6].strength[12] = 1 ;
	iniopt[6].strength[13] = 1 ;
	iniopt[6].strength[14] = 1 ;
	iniopt[6].strength[15] = 0 ;
	iniopt[6].strength[16] = 1 ;

	iniopt[6].number = 1 ;	
	
	/* 大尉 */
	strcpy_s(iniopt[7].name, "Captain") ;
	
	iniopt[7].direction[0] = 1 ;
	iniopt[7].direction[1] = 1 ;
	iniopt[7].direction[2] = 1 ;
	iniopt[7].direction[3] = 1 ;

	iniopt[7].strength[0] = 1 ;	
	iniopt[7].strength[1] = -1 ;
	iniopt[7].strength[2] = -1 ;
	iniopt[7].strength[3] = -1 ;
	iniopt[7].strength[4] = -1 ;
	iniopt[7].strength[5] = -1 ;
	iniopt[7].strength[6] = -1 ;
	iniopt[7].strength[7] = 0 ;
	iniopt[7].strength[8] = 1 ;
	iniopt[7].strength[9] = 1 ;
	iniopt[7].strength[10] = -1 ;
	iniopt[7].strength[11] = -1 ;
	iniopt[7].strength[12] = 1 ;
	iniopt[7].strength[13] = 1 ;
	iniopt[7].strength[14] = 1 ;
	iniopt[7].strength[15] = 0 ;
	iniopt[7].strength[16] = 1 ;

	iniopt[7].number = 2 ;
	
	/* 中尉 */
	strcpy_s(iniopt[8].name, "Lieutenant") ;
	
	iniopt[8].direction[0] = 1 ;
	iniopt[8].direction[1] = 1 ;
	iniopt[8].direction[2] = 1 ;
	iniopt[8].direction[3] = 1 ;

	iniopt[8].strength[0] = 1 ;
	iniopt[8].strength[1] = -1 ;
	iniopt[8].strength[2] = -1 ;
	iniopt[8].strength[3] = -1 ;
	iniopt[8].strength[4] = -1 ;
	iniopt[8].strength[5] = -1 ;
	iniopt[8].strength[6] = -1 ;
	iniopt[8].strength[7] = -1 ;
	iniopt[8].strength[8] = 0 ;
	iniopt[8].strength[9] = 1 ;
	iniopt[8].strength[10] = -1 ;
	iniopt[8].strength[11] = -1 ;
	iniopt[8].strength[12] = 1 ;
	iniopt[8].strength[13] = 1 ;
	iniopt[8].strength[14] = 1 ;
	iniopt[8].strength[15] = 0 ;
	iniopt[8].strength[16] = 1 ;

	iniopt[8].number = 2 ;
	
	/* 少尉 */
	strcpy_s(iniopt[9].name, "Second Lieutenant") ;
	
	iniopt[9].direction[0] = 1 ;
	iniopt[9].direction[1] = 1 ;
	iniopt[9].direction[2] = 1 ;
	iniopt[9].direction[3] = 1 ;

	iniopt[9].strength[0] = 1 ;	
	iniopt[9].strength[1] = -1 ;
	iniopt[9].strength[2] = -1 ;
	iniopt[9].strength[3] = -1 ;
	iniopt[9].strength[4] = -1 ;
	iniopt[9].strength[5] = -1 ;
	iniopt[9].strength[6] = -1 ;
	iniopt[9].strength[7] = -1 ;
	iniopt[9].strength[8] = -1 ;
	iniopt[9].strength[9] = 0 ;
	iniopt[9].strength[10] = -1 ;
	iniopt[9].strength[11] = -1 ;
	iniopt[9].strength[12] = 1 ;
	iniopt[9].strength[13] = 1 ;
	iniopt[9].strength[14] = 1 ;
	iniopt[9].strength[15] = 0 ;
	iniopt[9].strength[16] = 1 ;

	iniopt[9].number = 2 ;
	
	/* 騎兵 */
	strcpy_s(iniopt[10].name, "Cavalry") ;
	
	iniopt[10].direction[0] = 1 ;
	iniopt[10].direction[1] = 2 ;
	iniopt[10].direction[2] = 1 ;
	iniopt[10].direction[3] = 1 ;

	iniopt[10].strength[0] = 1 ;	
	iniopt[10].strength[1] = -1 ;
	iniopt[10].strength[2] = -1 ;
	iniopt[10].strength[3] = -1 ;
	iniopt[10].strength[4] = -1 ;
	iniopt[10].strength[5] = -1 ;
	iniopt[10].strength[6] = -1 ;
	iniopt[10].strength[7] = -1 ;
	iniopt[10].strength[8] = -1 ;
	iniopt[10].strength[9] = -1 ;
	iniopt[10].strength[10] = -1 ;
	iniopt[10].strength[11] = -1 ;
	iniopt[10].strength[12] = 0 ;
	iniopt[10].strength[13] = 1 ;
	iniopt[10].strength[14] = 1 ;
	iniopt[10].strength[15] = 0 ;
	iniopt[10].strength[16] = 1 ;
	
	iniopt[10].number = 1 ;	
	
	/* 工兵 */
	strcpy_s(iniopt[11].name, "Sapper") ;
	
	iniopt[11].direction[0] = 15 ;
	iniopt[11].direction[1] = 15 ;
	iniopt[11].direction[2] = 15 ;
	iniopt[11].direction[3] = 15 ;

	iniopt[11].strength[0] = 1 ;	
	iniopt[11].strength[1] = -1 ;
	iniopt[11].strength[2] = -1 ;
	iniopt[11].strength[3] = -1 ;
	iniopt[11].strength[4] = -1 ;
	iniopt[11].strength[5] = -1 ;
	iniopt[11].strength[6] = -1 ;
	iniopt[11].strength[7] = -1 ;
	iniopt[11].strength[8] = -1 ;
	iniopt[11].strength[9] = -1 ;
	iniopt[11].strength[10] = -1 ;
	iniopt[11].strength[11] = 1 ;
	iniopt[11].strength[12] = -1 ;
	iniopt[11].strength[13] = 0 ;
	iniopt[11].strength[14] = 1 ;
	iniopt[11].strength[15] = 1 ;
	iniopt[11].strength[16] = 1 ;
	
	iniopt[11].number = 2 ;	
	
	/* 地雷 */
	strcpy_s(iniopt[12].name, "Lamdmine") ;
	
	iniopt[12].direction[0] = 0 ;
	iniopt[12].direction[1] = 0 ;
	iniopt[12].direction[2] = 0 ;
	iniopt[12].direction[3] = 0 ;

	iniopt[12].strength[0] = 1 ;	
	iniopt[12].strength[1] = 0 ;
	iniopt[12].strength[2] = 0 ;
	iniopt[12].strength[3] = 0 ;
	iniopt[12].strength[4] = 0 ;
	iniopt[12].strength[5] = 0 ;
	iniopt[12].strength[6] = 0 ;
	iniopt[12].strength[7] = 0 ;
	iniopt[12].strength[8] = 0 ;
	iniopt[12].strength[9] = 0 ;
	iniopt[12].strength[10] = -1 ;
	iniopt[12].strength[11] = 0 ;
	iniopt[12].strength[12] = 0 ;
	iniopt[12].strength[13] = -1 ;
	iniopt[12].strength[14] = 0 ;
	iniopt[12].strength[15] = 0 ;
	iniopt[12].strength[16] = 1 ;

	iniopt[12].number = 2 ;
	
	/* 飛行機 */
	strcpy_s(iniopt[13].name, "Airplane") ;
	
	iniopt[13].direction[0] = 1 ;
	iniopt[13].direction[1] = 15 ;
	iniopt[13].direction[2] = 1 ;
	iniopt[13].direction[3] = 15 ;

	iniopt[13].strength[0] = 1 ;	
	iniopt[13].strength[1] = -1 ;
	iniopt[13].strength[2] = -1 ;
	iniopt[13].strength[3] = -1 ;
	iniopt[13].strength[4] = 1 ;
	iniopt[13].strength[5] = 1 ;
	iniopt[13].strength[6] = 1 ;
	iniopt[13].strength[7] = 1 ;
	iniopt[13].strength[8] = 1 ;
	iniopt[13].strength[9] = 1 ;
	iniopt[13].strength[10] = 0 ;
	iniopt[13].strength[11] = 1 ;
	iniopt[13].strength[12] = 1 ;
	iniopt[13].strength[13] = 1 ;
	iniopt[13].strength[14] = 1 ;
	iniopt[13].strength[15] = 1 ;
	iniopt[13].strength[16] = 1 ;

	iniopt[13].number = 2 ;	
	
	/* タンク */
	strcpy_s(iniopt[14].name, "Tank") ;
	
	iniopt[14].direction[0] = 1 ;
	iniopt[14].direction[1] = 2 ;
	iniopt[14].direction[2] = 1 ;
	iniopt[14].direction[3] = 1 ;

	iniopt[14].strength[0] = 1 ;	
	iniopt[14].strength[1] = -1 ;
	iniopt[14].strength[2] = -1 ;
	iniopt[14].strength[3] = -1 ;
	iniopt[14].strength[4] = 1 ;
	iniopt[14].strength[5] = 1 ;
	iniopt[14].strength[6] = 1 ;
	iniopt[14].strength[7] = 1 ;
	iniopt[14].strength[8] = 1 ;
	iniopt[14].strength[9] = 1 ;
	iniopt[14].strength[10] = -1 ;
	iniopt[14].strength[11] = 0 ;
	iniopt[14].strength[12] = 1 ;
	iniopt[14].strength[13] = -1 ;
	iniopt[14].strength[14] = 1 ;
	iniopt[14].strength[15] = 0 ;
	iniopt[14].strength[16] = 1 ;

	iniopt[14].number = 2 ;	
	
	/* スパイ */
	strcpy_s(iniopt[15].name, "Agent") ;
	
	iniopt[15].direction[0] = 1 ;
	iniopt[15].direction[1] = 1 ;
	iniopt[15].direction[2] = 1 ;
	iniopt[15].direction[3] = 1 ;

	iniopt[15].strength[0] = 1 ;	
	iniopt[15].strength[1] = 1 ;
	iniopt[15].strength[2] = -1 ;
	iniopt[15].strength[3] = -1 ;
	iniopt[15].strength[4] = -1 ;
	iniopt[15].strength[5] = -1 ;
	iniopt[15].strength[6] = -1 ;
	iniopt[15].strength[7] = -1 ;
	iniopt[15].strength[8] = -1 ;
	iniopt[15].strength[9] = -1 ;
	iniopt[15].strength[10] = -1 ;
	iniopt[15].strength[11] = -1 ;
	iniopt[15].strength[12] = -1 ;
	iniopt[15].strength[13] = -1 ;
	iniopt[15].strength[14] = 0 ;
	iniopt[15].strength[15] = 0 ;
	iniopt[15].strength[16] = 1 ;

	iniopt[15].number = 1 ;
	
	/* 軍旗 */
	strcpy_s(iniopt[16].name, "War Flag") ;
	
	iniopt[16].direction[0] = 0 ;
	iniopt[16].direction[1] = 0 ;
	iniopt[16].direction[2] = 0 ;
	iniopt[16].direction[3] = 0 ;

	iniopt[16].strength[0] = 1 ;	
	iniopt[16].strength[1] = -1 ;
	iniopt[16].strength[2] = -1 ;
	iniopt[16].strength[3] = -1 ;
	iniopt[16].strength[4] = -1 ;
	iniopt[16].strength[5] = -1 ;
	iniopt[16].strength[6] = -1 ;
	iniopt[16].strength[7] = -1 ;
	iniopt[16].strength[8] = -1 ;
	iniopt[16].strength[9] = -1 ;
	iniopt[16].strength[10] = -1 ;
	iniopt[16].strength[11] = -1 ;
	iniopt[16].strength[12] = -1 ;
	iniopt[16].strength[13] = -1 ;
	iniopt[16].strength[14] = -1 ;
	iniopt[16].strength[15] = -1 ;
	iniopt[16].strength[16] = 0 ;

	iniopt[16].number = 1 ;
	
	dat = fopen(datname, "wb") ;
	
	WRITEDAT(classnum) ;
	WRITEDAT(height) ;
	WRITEDAT(width) ;
	
	for (n = 0 ; n <= classnum ; n++){
		WRITEDAT(iniopt[n].name) ;
		
		for (i = 0 ; i < 4 ; i++){
			WRITEDAT(iniopt[n].direction[i]) ;
		}
		for (i = 0 ; i <= classnum ; i++){
			WRITEDAT(iniopt[n].strength[i]) ;
		}
		
		WRITEDAT(iniopt[n].number) ;
	}
	
	fclose(dat) ;
	
	return ;
}

















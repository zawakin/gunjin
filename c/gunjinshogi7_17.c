#include <stdio.h>
#include <string.h>


#define READDAT(X) fread(&X, sizeof(X), 1, dat)
#define WRITEDAT(X) fwrite(&X, sizeof(X), 1, dat)


typedef struct {
	char name[32] ;
	int direction[4] ;   /* right, front, left, back */
	int strength[32] ;
	int number ;
} uni ;

typedef struct {
	int classnum ;
	int height ;
	int width ;
	uni unit[32] ;
} rul ;



void initialrule(void) ;  /* 初期設定データの作成 */
int loadrule(rul *rule, char datname[]) ;  /* 設定の読み込み */ /* return = -1: 読み込み失敗 */
void saverule(rul rule, char datname[]) ;  /* 設定の保存 */
void view(int board[][16], int turn, rul rule) ;  /* 盤面の表示 */
void setting(int board[][16], int turn, rul rule) ;  /* 配置 */
int movabledomain(int pro[], int board[][16], int domain[][16], int turn, rul rule) ;  /* 移動可能領域 */ /* return = -1: 無効な選択 */
void emphasizedview(int board[][16], int domain[][16], int turn, rul rule) ;  /* 移動可能領域の表示 */
int movement(int pro[], int post[], int board[][16], int domain[][16], rul rule) ;  /* 移動 */ /* return = -1: 無効な移動 */
void fight(int pro[], int post[], int board[][16], rul rule) ;  /* 戦闘 */
int victory(int board[][16], rul rule) ;  /* 勝利条件の判定 */ /* return = 1: 先攻の勝利, 2: 後攻の勝利, 3: 引き分け */
void allview(int board[][16], int turn, rul rule) ;  /* 全駒の表示 */



int main(void)
{
	char datname[64] ;
	char firstname[64] ;
	char secondname[64] ;
	rul rule ;
	int board[16][16] ;
	int domain[16][16] ;
	int turn ;
	int pro[2], post[2] ;
	int vic = 0 ;
	int error ;
	
	printf("先攻の名前を入力してください。\n\n") ;
	scanf("%s", firstname) ;
	printf("\n\n後攻の名前を入力してください。\n\n") ;
	scanf("%s", secondname) ;
	
	strcpy(datname, "initialrule.dat") ;
	
	loadrule(&rule, datname) ;
	
	turn = 1 ;
	printf("\n\n%sさんは駒を配置してください。\n\n", firstname) ;
	setting(board, turn, rule) ;
	turn = 2 ;
	printf("\n\n%sさんは駒を配置してください。\n\n", secondname) ;
	setting(board, turn, rule) ;
	
	printf("ゲームを開始します。\n\n") ;
	turn = 1 ;
	do {
		if (turn % 2 == 1){
			printf("\n\n%sさんの番です。\n\n", firstname) ;
		} else {
			printf("\n\n%sさんの番です。\n\n", secondname) ;
		}
		do {
			view(board, turn, rule) ;			
			scanf("%d", &pro[0]) ;
			scanf("%d", &pro[1]) ;
			pro[0]-- ;
			pro[1]-- ;
			error = movabledomain(pro, board, domain, turn, rule) ;
			if (error != -1) {
				emphasizedview(board, domain, turn, rule) ;
				scanf("%d", &post[0]) ;
				scanf("%d", &post[1]) ;
				post[0]-- ;
				post[1]-- ;
				error = movement(pro, post, board, domain, rule) ;
			}
		} while (error == -1) ;
		vic = victory(board, rule) ;
		if (vic == 0){
			turn++ ;
		}
	} while (vic == 0) ;
	
	if (vic == 1){
		printf("\n\n%sさんの勝利です。\n\n", firstname) ;
	} else if (vic == 2){
		printf("\n\n%sさんの勝利です。\n\n", secondname) ;
	} else {
		printf("\n\n引き分けです。\n\n") ;
	}
	allview(board, turn, rule) ;
	return 0;
}


int victory(int board[][16], rul rule)
{
	int first, second ;
	int i, j ;
	int vic = 0 ;
	
	if (board[rule.width/2-1][0] >= 1 && board[rule.width/2-1][0] <= 6){
		vic = 1 ;
	} else if (board[rule.width/2][rule.height-1] >= 1+rule.classnum && board[rule.width/2][rule.height-1] <= 6+rule.classnum){
		vic = 2 ;
	}
	
	first = 0 ;
	second = 0 ;
	for (i = 0 ; i < rule.width ; i++){
		for (j = 0 ; j < rule.height ; j++){
			if ((board[i][j] > 0 && board[i][j] <= rule.classnum) && !(board[i][j] == 12 || board[i][j] == 16)){
				first++ ;
			} else if (board[i][j] > rule.classnum && !(board[i][j]-rule.classnum == 12 || board[i][j]-rule.classnum == 16)){	
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


void allview(int board[][16], int turn, rul rule)
{
	int i, j ;
	
	if (turn % 2 == 1){
		for (i = 0 ; i < rule.width ; i++){
			printf("  %2d  ", rule.width-i) ;
		}
		printf("\n") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;	
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (board[rule.width-i-1][0] <= rule.classnum){
				printf("| %3s ", rule.unit[board[rule.width-i-1][0]].name) ;
			} else {
				printf("|*%3s*", rule.unit[board[rule.width-i-1][0]-rule.classnum].name) ;
			}
		}
		if (board[rule.width/2-1][0] <= rule.classnum){		
			printf("|    %3s    ", rule.unit[board[rule.width/2-1][0]].name) ;
		} else {
			printf("|   *%3s*   ", rule.unit[board[rule.width/2-1][0]-rule.classnum].name) ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (board[rule.width-i-1][0] <= rule.classnum){
				printf("| %3s ", rule.unit[board[rule.width-i-1][0]].name) ;
			} else {
				printf("|*%3s*", rule.unit[board[rule.width-i-1][0]-rule.classnum].name) ;
			}
		}
		printf("| %2d\n", 1) ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		for (j = 1 ; j < rule.height/2 ; j++){
			for (i = 0 ; i < rule.width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;	
			for (i = 0 ; i < rule.width ; i++){
				if (board[rule.width-i-1][j] <= rule.classnum){
					printf("| %3s ", rule.unit[board[rule.width-i-1][j]].name) ;
				} else {
					printf("|*%3s*", rule.unit[board[rule.width-i-1][j]-rule.classnum].name) ;
				}
			}
			printf("| %2d\n", j+1) ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		}
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("|     |     |     ") ;
		for (i = 3 ; i < rule.width-2 ; i++){
			printf("      ") ;
		}
		printf("|     |     |\n") ;
		for (j = rule.height/2 ; j < rule.height-1 ; j++){
			for (i = 0 ; i < rule.width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;	
			for (i = 0 ; i < rule.width ; i++){
				if (board[rule.width-i-1][j] <= rule.classnum){
					printf("| %3s ", rule.unit[board[rule.width-i-1][j]].name) ;
				} else {
					printf("|*%3s*", rule.unit[board[rule.width-i-1][j]-rule.classnum].name) ;
				}
			}
			printf("| %2d\n", j+1) ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		}
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;	
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (board[rule.width-i-1][rule.height-1] <= rule.classnum){
				printf("| %3s ", rule.unit[board[rule.width-i-1][rule.height-1]].name) ;
			} else {
				printf("|*%3s*", rule.unit[board[rule.width-i-1][rule.height-1]-rule.classnum].name) ;
			}
		}
		if (board[rule.width/2][rule.height-1] <= rule.classnum){
			printf("|    %3s    ", rule.unit[board[rule.width/2][rule.height-1]].name) ;
		} else {
			printf("|   *%3s*   ", rule.unit[board[rule.width/2][rule.height-1]-rule.classnum].name) ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (board[rule.width-i-1][rule.height-1] <= rule.classnum){
				printf("| %3s ", rule.unit[board[rule.width-i-1][rule.height-1]].name) ;
			} else {
				printf("|*%3s*", rule.unit[board[rule.width-i-1][rule.height-1]-rule.classnum].name) ;
			}
		}
		printf("| %2d\n", rule.height) ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n\n") ;
	} else {
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		printf("%2d  ", rule.height) ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (board[i][rule.height-1] > rule.classnum){
				printf("| %3s ", rule.unit[board[i][rule.height-1]-rule.classnum].name) ;
			} else if (board[i][rule.height-1] == 0){
				printf("|     ") ;
			} else {
				printf("|*%3s*", rule.unit[board[i][rule.height-1]].name) ;
			}
		}
		if (board[rule.width/2][rule.height-1] > rule.classnum){
			printf("|    %3s    ", rule.unit[board[rule.width/2][rule.height-1]-rule.classnum].name) ;
		} else if (board[rule.width/2][rule.height-1] == 0){
			printf("|           ") ;
		} else {
			printf("|   *%3s*   ", rule.unit[board[rule.width/2][rule.height-1]].name) ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (board[i][rule.height-1] > rule.classnum){
				printf("| %3s ", rule.unit[board[i][rule.height-1]-rule.classnum].name) ;
			} else if (board[i][rule.height-1] == 0){
				printf("|     ") ;
			} else {
				printf("|*%3s*", rule.unit[board[i][rule.height-1]].name) ;
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		for (j = 1 ; j < rule.height/2 ; j++){
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
			printf("%2d  ", rule.height-j) ;
			for (i = 0 ; i < rule.width ; i++){
				if (board[i][rule.height-j-1] > rule.classnum){
					printf("| %3s ", rule.unit[board[i][rule.height-j-1]-rule.classnum].name) ;
				} else if (board[i][rule.height-j-1] == 0){
					printf("|     ") ;
				} else {
					printf("|*%3s*", rule.unit[board[i][rule.height-j-1]].name) ;
				}
			}
			printf("|\n") ;
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		}
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		printf("|     |     |     ") ;
		for (i = 3 ; i < rule.width-2 ; i++){
			printf("      ") ;
		}
		printf("|     |     |\n") ;		
		for (j = rule.height/2 ; j < rule.height-1 ; j++){
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
			printf("%2d  ", rule.height-j) ;
			for (i = 0 ; i < rule.width ; i++){
				if (board[i][rule.height-j-1] > rule.classnum){
					printf("| %3s ", rule.unit[board[i][rule.height-j-1]-rule.classnum].name) ;
				} else if (board[i][rule.height-j-1] == 0){
					printf("|     ") ;
				} else {
					printf("|*%3s*", rule.unit[board[i][rule.height-j-1]].name) ;
				}
			}
			printf("|\n") ;
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;			
		}
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		printf("%2d  ", 1) ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (board[i][0] > rule.classnum){
				printf("| %3s ", rule.unit[board[i][0]-rule.classnum].name) ;
			} else if (board[i][0] == 0){
				printf("|     ") ;
			} else {
				printf("|*%3s*", rule.unit[board[i][0]].name) ;
			}
		}
		if (board[rule.width/2-1][0] > rule.classnum){
			printf("|    %3s    ", rule.unit[board[rule.width/2-1][0]-rule.classnum].name) ;
		} else if (board[rule.width/2-1][0] == 0){
			printf("|           ") ;
		} else {
			printf("|   *%3s*   ", rule.unit[board[rule.width/2-1][0]].name) ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (board[i][0] > rule.classnum){
				printf("| %3s ", rule.unit[board[i][0]-rule.classnum].name) ;
			} else if (board[i][0] == 0){
				printf("|     ") ;
			} else {
				printf("|*%3s*", rule.unit[board[i][0]].name) ;
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("  %2d  ", i+1) ;
		}
		printf("\n\n") ;
	}
	return ;
}


void view(int board[][16], int turn, rul rule)
{
	int i, j ;
	
	if (turn % 2 == 1){
		for (i = 0 ; i < rule.width ; i++){
			printf("  %2d  ", rule.width-i) ;
		}
		printf("\n") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;	
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (board[rule.width-i-1][0] <= rule.classnum){
				printf("| %3s ", rule.unit[board[rule.width-i-1][0]].name) ;
			} else {
				printf("| ??? ") ;
			}
		}
		if (board[rule.width/2-1][0] <= rule.classnum){
			printf("|    %3s    ", rule.unit[board[rule.width/2-1][0]].name) ;
		} else {
			printf("|    ???    ") ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (board[rule.width-i-1][0] <= rule.classnum){
				printf("| %3s ", rule.unit[board[rule.width-i-1][0]].name) ;
			} else {
				printf("| ??? ") ;
			}
		}
		printf("| %2d\n", 1) ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;	
		for (j = 1 ; j < rule.height/2 ; j++){
			for (i = 0 ; i < rule.width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;	
			for (i = 0 ; i < rule.width ; i++){
				if (board[rule.width-i-1][j] <= rule.classnum){
					printf("| %3s ", rule.unit[board[rule.width-i-1][j]].name) ;
				} else {
					printf("| ??? ") ;
				}
			}
			printf("| %2d\n", j+1) ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		}
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("|     |     |     ") ;
		for (i = 3 ; i < rule.width-2 ; i++){
			printf("      ") ;
		}
		printf("|     |     |\n") ;
		for (j = rule.height/2 ; j < rule.height-1 ; j++){
			for (i = 0 ; i < rule.width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;	
			for (i = 0 ; i < rule.width ; i++){
				if (board[rule.width-i-1][j] <= rule.classnum){
					printf("| %3s ", rule.unit[board[rule.width-i-1][j]].name) ;
				} else {
					printf("| ??? ") ;
				}
			}
			printf("| %2d\n", j+1) ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		}
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;	
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (board[rule.width-i-1][rule.height-1] <= rule.classnum){
				printf("| %3s ", rule.unit[board[rule.width-i-1][rule.height-1]].name) ;
			} else {
				printf("| ??? ") ;
			}
		}
		if (board[rule.width/2][rule.height-1] <= rule.classnum){
			printf("|    %3s    ", rule.unit[board[rule.width/2][rule.height-1]].name) ;
		} else {
			printf("|    ???    ") ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (board[rule.width-i-1][rule.height-1] <= rule.classnum){
				printf("| %3s ", rule.unit[board[rule.width-i-1][rule.height-1]].name) ;
			} else {
				printf("| ??? ") ;
			}
		}
		printf("| %2d\n", rule.height) ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n\n") ;
	} else {
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		printf("%2d  ", rule.height) ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (board[i][rule.height-1] > rule.classnum){
				printf("| %3s ", rule.unit[board[i][rule.height-1]-rule.classnum].name) ;
			} else if (board[i][rule.height-1] == 0){
				printf("|     ") ;
			} else {
				printf("| ??? ") ;
			}
		}
		if (board[rule.width/2][rule.height-1] > rule.classnum){
			printf("|    %3s    ", rule.unit[board[rule.width/2][rule.height-1]-rule.classnum].name) ;
		} else if (board[rule.width/2][rule.height-1] == 0){
			printf("|           ") ;
		} else {
			printf("|    ???    ") ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (board[i][rule.height-1] > rule.classnum){
				printf("| %3s ", rule.unit[board[i][rule.height-1]-rule.classnum].name) ;
			} else if (board[i][rule.height-1] == 0){
				printf("|     ") ;
			} else {
				printf("| ??? ") ;
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		for (j = 1 ; j < rule.height/2 ; j++){
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
			printf("%2d  ", rule.height-j) ;
			for (i = 0 ; i < rule.width ; i++){
				if (board[i][rule.height-j-1] > rule.classnum){
					printf("| %3s ", rule.unit[board[i][rule.height-j-1]-rule.classnum].name) ;
				} else if (board[i][rule.height-j-1] == 0){
					printf("|     ") ;
				} else {
					printf("| ??? ") ;
				}
			}
			printf("|\n") ;
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		}
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		printf("|     |     |     ") ;
		for (i = 3 ; i < rule.width-2 ; i++){
			printf("      ") ;
		}
		printf("|     |     |\n") ;		
		for (j = rule.height/2 ; j < rule.height-1 ; j++){
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
			printf("%2d  ", rule.height-j) ;
			for (i = 0 ; i < rule.width ; i++){
				if (board[i][rule.height-j-1] > rule.classnum){
					printf("| %3s ", rule.unit[board[i][rule.height-j-1]-rule.classnum].name) ;
				} else if (board[i][rule.height-j-1] == 0){	
					printf("|     ") ;
				} else {
					printf("| ??? ") ;
				}
			}
			printf("|\n") ;
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("|     ") ;
			}
			printf("|\n") ;
		}
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		printf("%2d  ", 1) ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (board[i][0] > rule.classnum){
				printf("| %3s ", rule.unit[board[i][0]-rule.classnum].name) ;
			} else if (board[i][0] == 0){
				printf("|     ") ;
			} else {
				printf("| ??? ") ;
			}
		}
		if (board[rule.width/2-1][0] > rule.classnum){
			printf("|    %3s    ", rule.unit[board[rule.width/2-1][0]-rule.classnum].name) ;
		} else if (board[rule.width/2-1][0] == 0){
			printf("|           ") ;
		} else {
			printf("|    ???    ") ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (board[i][0] > rule.classnum){
				printf("| %3s ", rule.unit[board[i][0]-rule.classnum].name) ;
			} else if (board[i][0] == 0){
				printf("|     ") ;
			} else {
				printf("| ??? ") ;
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			printf("|     ") ;
		}
		printf("|           ") ;
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			printf("|     ") ;
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("  %2d  ", i+1) ;
		}
		printf("\n\n") ;
	}
	return ;
}


void emphasizedview(int board[][16], int domain[][16], int turn, rul rule)
{
	int i, j ;
	
	if (turn % 2 == 1){
		for (i = 0 ; i < rule.width ; i++){
			printf("  %2d  ", rule.width-i) ;
		}
		printf("\n") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (domain[rule.width-i-1][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[rule.width/2-1][0] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (domain[rule.width-i-1][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;	
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (domain[rule.width-i-1][0] == 1){
				if (board[rule.width-i-1][0] <= rule.classnum){
					printf("|*%3s*", rule.unit[board[rule.width-i-1][0]].name) ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[rule.width-i-1][0] <= rule.classnum){
					printf("| %3s ", rule.unit[board[rule.width-i-1][0]].name) ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		if (domain[rule.width/2-1][0] == 1){
			if (board[rule.width/2-1][0] <= rule.classnum){
				printf("|****%3s****", rule.unit[board[rule.width/2-1][0]].name) ;
			} else {
				printf("|****???****") ;
			}
		} else {
			if (board[rule.width/2-1][0] <= rule.classnum){
				printf("|    %3s    ", rule.unit[board[rule.width/2-1][0]].name) ;
			} else {
				printf("|    ???    ") ;
			}
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (domain[rule.width-i-1][0] == 1){
				if (board[rule.width-i-1][0] <= rule.classnum){
					printf("|*%3s*", rule.unit[board[rule.width-i-1][0]].name) ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[rule.width-i-1][0] <= rule.classnum){
					printf("| %3s ", rule.unit[board[rule.width-i-1][0]].name) ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		printf("| %2d\n", 1) ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (domain[rule.width-i-1][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[rule.width/2-1][0] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (domain[rule.width-i-1][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;
		for (j = 1 ; j < rule.height/2 ; j++){
			for (i = 0 ; i < rule.width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			for (i = 0 ; i < rule.width ; i++){
				if (domain[rule.width-i-1][j] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;	
			for (i = 0 ; i < rule.width ; i++){
				if (domain[rule.width-i-1][j] == 1){
					if (board[rule.width-i-1][j] <= rule.classnum){
						printf("|*%3s*", rule.unit[board[rule.width-i-1][j]].name) ;
					} else {
						printf("|*???*") ;
					}
				} else {
					if (board[rule.width-i-1][j] <= rule.classnum){
						printf("| %3s ", rule.unit[board[rule.width-i-1][j]].name) ;
					} else {
						printf("| ??? ") ;
					}
				}
			}
			printf("| %2d\n", j+1) ;
			for (i = 0 ; i < rule.width ; i++){
				if (domain[rule.width-i-1][j] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;
		}
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("|     |     |     ") ;
		for (i = 3 ; i < rule.width-2 ; i++){
			printf("      ") ;
		}
		printf("|     |     |\n") ;
		for (j = rule.height/2 ; j < rule.height-1 ; j++){
			for (i = 0 ; i < rule.width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			for (i = 0 ; i < rule.width ; i++){
				if (domain[rule.width-i-1][j] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;	
			for (i = 0 ; i < rule.width ; i++){
				if (domain[rule.width-i-1][j] == 1){
					if (board[rule.width-i-1][j] <= rule.classnum){
						printf("|*%3s*", rule.unit[board[rule.width-i-1][j]].name) ;
					} else {
						printf("|*???*") ;
					}
				} else {
					if (board[rule.width-i-1][j] <= rule.classnum){
						printf("| %3s ", rule.unit[board[rule.width-i-1][j]].name) ;
					} else {
						printf("| ??? ") ;
					}					
				}
			}
			printf("| %2d\n", j+1) ;
			for (i = 0 ; i < rule.width ; i++){
				if (domain[rule.width-i-1][j] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;	
		}
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (domain[rule.width-i-1][rule.height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[rule.width/2][rule.height-1] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (domain[rule.width-i-1][rule.height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;	
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (domain[rule.width-i-1][rule.height-1] == 1){
				if (board[rule.width-i-1][rule.height-1] <= rule.classnum){
					printf("|*%3s*", rule.unit[board[rule.width-i-1][rule.height-1]].name) ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[rule.width-i-1][rule.height-1] <= rule.classnum){
					printf("| %3s ", rule.unit[board[rule.width-i-1][rule.height-1]].name) ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		if (domain[rule.width/2][rule.height-1] == 1){
			if (board[rule.width/2][rule.height-1] <= rule.classnum){
				printf("|****%3s****", rule.unit[board[rule.width/2][rule.height-1]].name) ;
			} else {
				printf("|****???****") ;
			}
		} else {
			if (board[rule.width/2][rule.height-1] <= rule.classnum){
				printf("|    %3s    ", rule.unit[board[rule.width/2][rule.height-1]].name) ;
			} else {
				printf("|    ???    ") ;
			}
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (domain[rule.width-i-1][rule.height-1] == 1){
				if (board[rule.width-i-1][rule.height-1] <= rule.classnum){
					printf("|*%3s*", rule.unit[board[rule.width-i-1][rule.height-1]].name) ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[rule.width-i-1][rule.height-1] <= rule.classnum){
					printf("| %3s ", rule.unit[board[rule.width-i-1][rule.height-1]].name) ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		printf("| %2d\n", rule.height) ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (domain[rule.width-i-1][rule.height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[rule.width/2][rule.height-1] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (domain[rule.width-i-1][rule.height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;	
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n\n") ;
	} else {
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (domain[i][rule.height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[rule.width/2][rule.height-1] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (domain[i][rule.height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;
		printf("%2d  ", rule.height) ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (domain[i][rule.height-1] == 1){
				if (board[i][rule.height-1] > rule.classnum){
					printf("|*%3s*", rule.unit[board[i][rule.height-1]-rule.classnum].name) ;
				} else if (board[i][rule.height-1] == 0){
					printf("|*   *") ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[i][rule.height-1] > rule.classnum){
					printf("| %3s ", rule.unit[board[i][rule.height-1]-rule.classnum].name) ;
				} else if (board[i][rule.height-1] == 0){	
					printf("|     ") ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		if (domain[rule.width/2][rule.height-1] == 1){
			if (board[rule.width/2][rule.height-1] > rule.classnum){
				printf("|****%3s****", rule.unit[board[rule.width/2][rule.height-1]-rule.classnum].name) ;
			} else if (board[rule.width/2][rule.height-1] == 0){
				printf("|****   ****") ;
			} else {
				printf("|****???****") ;
			}
		} else {
			if (board[rule.width/2][rule.height-1] > rule.classnum){
				printf("|    %3s    ", rule.unit[board[rule.width/2][rule.height-1]-rule.classnum].name) ;
			} else if (board[rule.width/2][rule.height-1] == 0){	
				printf("|           ") ;
			} else {
				printf("|    ???    ") ;
			}
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (domain[i][rule.height-1] == 1){
				if (board[i][rule.height-1] > rule.classnum){
					printf("|*%3s*", rule.unit[board[i][rule.height-1]-rule.classnum].name) ;
				} else if (board[i][rule.height-1] == 0){
					printf("|*   *") ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[i][rule.height-1] > rule.classnum){
					printf("| %3s ", rule.unit[board[i][rule.height-1]-rule.classnum].name) ;
				} else if (board[i][rule.height-1] == 0){	
					printf("|     ") ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (domain[i][rule.height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[rule.width/2][rule.height-1] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (domain[i][rule.height-1] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;
		for (j = 1 ; j < rule.height/2 ; j++){
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				if (domain[i][rule.height-j-1] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;
			printf("%2d  ", rule.height-j) ;
			for (i = 0 ; i < rule.width ; i++){
				if (domain[i][rule.height-j-1] == 1){
					if (board[i][rule.height-j-1] > rule.classnum){
						printf("|*%3s*", rule.unit[board[i][rule.height-j-1]-rule.classnum].name) ;
					} else if (board[i][rule.height-j-1] == 0){
						printf("|*   *") ;
					} else {
						printf("|*???*") ;
					}
				} else {
					if (board[i][rule.height-j-1] > rule.classnum){
						printf("| %3s ", rule.unit[board[i][rule.height-j-1]-rule.classnum].name) ;
					} else if (board[i][rule.height-j-1] == 0){	
						printf("|     ") ;
					} else {
						printf("| ??? ") ;
					}
				}
			}
			printf("|\n") ;
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				if (domain[i][rule.height-j-1] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;
		}
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		printf("|     |     |     ") ;
		for (i = 3 ; i < rule.width-2 ; i++){
			printf("      ") ;
		}
		printf("|     |     |\n") ;		
		for (j = rule.height/2 ; j < rule.height-1 ; j++){
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				printf("+-----") ;
			}
			printf("+\n") ;
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				if (domain[i][rule.height-j-1] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;
			printf("%2d  ", rule.height-j) ;
			for (i = 0 ; i < rule.width ; i++){
				if (domain[i][rule.height-j-1] == 1){
					if (board[i][rule.height-j-1] > rule.classnum){
						printf("|*%3s*", rule.unit[board[i][rule.height-j-1]-rule.classnum].name) ;
					} else if (board[i][rule.height-j-1] == 0){
						printf("|*   *") ;
					} else {
						printf("|*???*") ;
					}
				} else {
					if (board[i][rule.height-j-1] > rule.classnum){
						printf("| %3s ", rule.unit[board[i][rule.height-j-1]-rule.classnum].name) ;
					} else if (board[i][rule.height-j-1] == 0){	
						printf("|     ") ;
					} else {
						printf("| ??? ") ;
					}
				}
			}
			printf("|\n") ;
			printf("    ") ;
			for (i = 0 ; i < rule.width ; i++){
				if (domain[i][rule.height-j-1] == 1){
					printf("|*****") ;
				} else {
					printf("|     ") ;
				}
			}
			printf("|\n") ;
		}
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (domain[i][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[rule.width/2-1][0] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (domain[i][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;
		printf("%2d  ", 1) ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (domain[i][0] == 1){
				if (board[i][0] > rule.classnum){
					printf("|*%3s*", rule.unit[board[i][0]-rule.classnum].name) ;
				} else if (board[i][0] == 0){
					printf("|*   *") ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[i][0] > rule.classnum){
					printf("| %3s ", rule.unit[board[i][0]-rule.classnum].name) ;
				} else if (board[i][0] == 0){	
					printf("|     ") ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		if (domain[rule.width/2-1][0] == 1){
			if (board[rule.width/2-1][0] > rule.classnum){
				printf("|****%3s****", rule.unit[board[rule.width/2-1][0]-rule.classnum].name) ;
			} else if (board[rule.width/2-1][0] == 0){
				printf("|*         *") ;
			} else {
				printf("|****???****") ;
			}
		} else {
			if (board[rule.width/2-1][0] > rule.classnum){
				printf("|    %3s    ", rule.unit[board[rule.width/2-1][0]-rule.classnum].name) ;
			} else if (board[rule.width/2-1][0] == 0){	
				printf("|           ") ;
			} else {
				printf("|    ???    ") ;
			}
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (domain[i][0] == 1){
				if (board[i][0] > rule.classnum){
					printf("|*%3s*", rule.unit[board[i][0]-rule.classnum].name) ;
				} else if (board[i][0] == 0){
					printf("|*   *") ;
				} else {
					printf("|*???*") ;
				}
			} else {
				if (board[i][0] > rule.classnum){
					printf("| %3s ", rule.unit[board[i][0]-rule.classnum].name) ;
				} else if (board[i][0] == 0){	
					printf("|     ") ;
				} else {
					printf("| ??? ") ;
				}
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width/2-1 ; i++){
			if (domain[i][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		if (domain[rule.width/2-1][0] == 1){
			printf("|***********") ;
		} else {
			printf("|           ") ;
		}
		for (i = rule.width/2+1 ; i < rule.width ; i++){
			if (domain[i][0] == 1){
				printf("|*****") ;
			} else {
				printf("|     ") ;
			}
		}
		printf("|\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("+-----") ;
		}
		printf("+\n") ;
		printf("    ") ;
		for (i = 0 ; i < rule.width ; i++){
			printf("  %2d  ", i+1) ;
		}
		printf("\n\n") ;
	}
	return ;
}


void setting(int board[][16], int turn, rul rule)
{
	int preboard[16][16] ;
	int i, j ;
	int k, l, m ;
	int list[128] ;
	int yes = 0 ;
	char proname[32] ;
	int pronum, post[2] ;
	int num ;
	
	if (turn % 2 == 1){
		k = 1 ;
		l = 0 ;
		while (k <= rule.classnum){
			m = 1 ;
			while (m <= rule.unit[k].number){
				list[l] = k ;
				m++ ;
				l++ ;
			}
			k++ ;
		}
		for (i = 0 ; i < rule.width ; i++){
			for (j = 0 ; j < rule.height/2 ; j++){
				preboard[i][j] = rule.classnum+1 ;
			}
			for (j = rule.height/2 ; j < rule.height ; j++) {
				preboard[i][j] = 0 ;
			}
		}
		do {
			view(preboard, turn, rule) ;
			printf("\n\n") ;
			for (k = 0 ; k < l ; k++){
				if (list[k] == 0){
					printf("| Nul ") ;
				} else {
					printf("| %3s ", rule.unit[list[k]].name) ;
				}
			}
			printf("|\n\n") ;
			scanf("%s", proname) ;
			scanf("%d", &post[0]) ;
			scanf("%d", &post[1]) ;
			post[0]-- ;
			post[1]-- ;
			pronum = -1 ;
			if (strcmp(proname, "Nul") == 0){
				pronum = 0 ;
			}
			for (k = 1 ; k <= rule.classnum ; k++){
				if (strcmp(proname, rule.unit[k].name) == 0){
					pronum = k ;
				break ;
				}
			}
			if (post[0] == rule.width/2 && post[1] == 0){
				post[0] = rule.width/2-1 ;
			} else if (post[0] == rule.width/2-1 && post[1] == rule.height-1) {
				post[0] = rule.width/2 ;
			}
			if ((pronum >= 0 && ((post[0] >= 0 && post[0] < rule.width) && (post[1] >= rule.height/2 && post[1] < rule.height))) && !((pronum == 12 || pronum == 16) && ((post[0] == 1 || post[0] == rule.width-2) && post[1] == rule.height/2))){
				for (k = 0 ; k < l ; k++){
					if (list[k] == pronum){
						list[k] = preboard[post[0]][post[1]] ;
						preboard[post[0]][post[1]] = pronum ;
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
					view(preboard, turn, rule) ;
					printf("この配置でよろしいですか？  yes: 1, no: 0\n\n") ;
					scanf("%d", &yes) ;
				}
			}
		} while (yes == 0) ;
		for (i = 0 ; i < rule.width ; i++){
			for (j = rule.height/2 ; j < rule.height ; j++){
				board[i][j] = preboard[i][j] ;
			}
		}
	} else {
		k = 1 ;
		l = 0 ;
		while (k <= rule.classnum){
			m = 1 ;
			while (m <= rule.unit[k].number){
				list[l] = k + rule.classnum ;
				m++ ;
				l++ ;
			}
			k++ ;
		}
		for (i = 0 ; i < rule.width ; i++){
			for (j = 0 ; j < rule.height/2 ; j++){
				preboard[i][j] = 0 ;
			}
			for (j = rule.height/2 ; j < rule.height ; j++) {
				preboard[i][j] = 1 ;
			}
		}
		do {
			view(preboard, turn, rule) ;
			printf("\n\n") ;
			for (k = 0 ; k < l ; k++){
				if (list[k] == 0){
					printf("| Nul ") ;
				} else {
					printf("| %3s ", rule.unit[list[k]-rule.classnum].name) ;
				}
			}
			printf("|\n\n") ;
			scanf("%s", proname) ;
			scanf("%d", &post[0]) ;
			scanf("%d", &post[1]) ;
			post[0]-- ;
			post[1]-- ;
			pronum = -1 ;
			if (strcmp(proname, "Nul") == 0){
				pronum = 0 ;
			}
			for (k = 0 ; k <= rule.classnum ; k++){
				if (strcmp(proname, rule.unit[k].name) == 0){
					pronum = k ;
				break ;
				}
			}
			if (post[0] == rule.width/2 && post[1] == 0){	
				post[0] = rule.width/2-1 ;
			} else if (post[0] == rule.width/2-1 && post[1] == rule.height-1) {
				post[0] = rule.width/2 ;
			}			
			if ((pronum >= 0 && ((post[0] >= 0 && post[0] < rule.width) && (post[1] >= 0 && post[1] < rule.height/2))) && !((pronum == 12 || pronum == 16) && ((post[0] == 1 || post[0] == rule.width-2) && post[1] == rule.height/2-1))){
				for (k = 0 ; k < l ; k++){
					if (list[k] == pronum+rule.classnum){
						list[k] = preboard[post[0]][post[1]] ;
						preboard[post[0]][post[1]] = pronum+rule.classnum ;
						break ;
					} else if (list[k] == pronum && pronum == 0){
						list[k] = preboard[post[0]][post[1]] ;
						preboard[post[0]][post[1]] = pronum ;
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
					view(preboard, turn, rule) ;
					printf("この配置でよろしいですか？  yes: 1, no: 0\n\n") ;
					scanf("%d", &yes) ;
				}
			}
		} while (yes != 1) ;
		for (i = 0 ; i < rule.width ; i++){
			for (j = 0 ; j < rule.height/2 ; j++){
				board[i][j] = preboard[i][j] ;
			}
		}		
	}
	return ;
}


int movabledomain(int pro[], int board[][16], int domain[][16], int turn, rul rule)
{
	int i, j ;
	int k, l, m ;
	int error = -1 ;
	
	for (i = 0 ; i < rule.width ; i++){
		for (j = 0 ; j < rule.height ; j++){
			domain[i][j] = 0 ;
		}
	}	
	
	if ((pro[0] >= 0 && pro[0] < rule.width) && (pro[1] >= 0 && pro[1] < rule.height)){		
		if (pro[0] == rule.width/2 && pro[1] == 0){	
			pro[0] = rule.width/2-1 ;
		} else if (pro[0] == rule.width/2-1 && pro[1] == rule.height-1) {
			pro[0] = rule.width/2 ;
		}
		if (turn % 2 == 1 && (board[pro[0]][pro[1]] > 0 && board[pro[0]][pro[1]] <= rule.classnum)){
			error = 0 ;
			k = 1 ;
			l = 1 ;
			while (k <= rule.unit[board[pro[0]][pro[1]]].direction[0] && pro[0]-l >= 0){
				if (board[pro[0]][pro[1]] != 13){
					if ((pro[0]-l == rule.width/2 && pro[1] == 0) || (pro[0]-l == rule.width/2-1 && pro[1] == rule.height-1)){
						l++ ;
					} else {
						if (board[pro[0]-l][pro[1]] == 0){
							domain[pro[0]-l][pro[1]] = 1 ;
						} else if (board[pro[0]-l][pro[1]] > rule.classnum){ 
							domain[pro[0]-l][pro[1]] = 1 ;
							break ;
						} else {
							break ;
						}
						k++ ;
						l++ ;
					}
				} else {
					if ((pro[0]-l == rule.width/2 && pro[1] == 0) || (pro[0]-l == rule.width/2-1 && pro[1] == rule.height-1)){
						l++ ;
					} else {
						if (board[pro[0]-l][pro[1]] == 0 || board[pro[0]-l][pro[1]] > rule.classnum){
							domain[pro[0]-l][pro[1]] = 1 ;
						}
						k++ ;
						l++ ;
					}
				}
			}
			k = 1 ;
			l = 1 ;
			while (k <= rule.unit[board[pro[0]][pro[1]]].direction[2] && pro[0]+l < rule.width){
				if (board[pro[0]][pro[1]] != 13){
					if ((pro[0]+l == rule.width/2 && pro[1] == 0) || (pro[0]+l == rule.width/2-1 && pro[1] == rule.height-1)){
						l++ ;
					} else {
						if (board[pro[0]+l][pro[1]] == 0){
							domain[pro[0]+l][pro[1]] = 1 ;
						} else if (board[pro[0]+l][pro[1]] > rule.classnum){ 
							domain[pro[0]+l][pro[1]] = 1 ;
							break ;
						} else {
							break ;
						}
						k++ ;
						l++ ;
					}
				} else {
					if ((pro[0]+l == rule.width/2 && pro[1] == 0) || (pro[0]+l == rule.width/2-1 && pro[1] == rule.height-1)){
						l++ ;
					} else {
						if (board[pro[0]+l][pro[1]] == 0 || board[pro[0]+l][pro[1]] > rule.classnum){
							domain[pro[0]+l][pro[1]] = 1 ;
						}
						k++ ;
						l++ ;
					}
				}
			}
			k = 1 ;
			l = 1 ;
			m = 1 ;
			while (k <= rule.unit[board[pro[0]][pro[1]]].direction[1] && pro[1]-k >= 0){
				if (board[pro[0]][pro[1]] != 13){
					if (m == 1 && (pro[0] == rule.width/2 && pro[1] == rule.height-1)){
						while (l <= rule.unit[board[pro[0]][pro[1]]].direction[1] && pro[1]-l >= rule.height/2){
							if (board[pro[0]-1][pro[1]-l] == 0){
								domain[pro[0]-1][pro[1]-l] = 1 ;
							} else if (board[pro[0]-1][pro[1]-l] > rule.classnum){
								domain[pro[0]-1][pro[1]-l] = 1 ;
								break ;
							} else {
								break ;
							}
							l++ ;
						}
						m++ ;
					} else if (!((pro[0] != 1 && pro[0] != rule.width-2) && pro[1]-k == rule.height/2-1)){
						if (pro[0] == rule.width/2 && pro[1]-k == 0){
							if (board[pro[0]-1][pro[1]-k] == 0){
								domain[pro[0]-1][pro[1]-k] = 1 ;
							} else if (board[pro[0]-1][pro[1]-k] > rule.classnum){ 
								domain[pro[0]-1][pro[1]-k] = 1 ;
								break ;
							} else {
								break ;
							}
						} else {
							if (board[pro[0]][pro[1]-k] == 0){
								domain[pro[0]][pro[1]-k] = 1 ;
							} else if (board[pro[0]][pro[1]-k] > rule.classnum){ 
								domain[pro[0]][pro[1]-k] = 1 ;
								break ;
							} else {
								break ;
							}
						}
						k++ ;
					} else {
						break ;
					}
				} else {
					if (m == 1 && (pro[0] == rule.width/2 && pro[1] == rule.height-1)){
						while (l <= rule.unit[board[pro[0]][pro[1]]].direction[1] && pro[1]-l >= 0){
							if (board[pro[0]-1][pro[1]-l] == 0 || board[pro[0]-1][pro[1]-l] > rule.classnum){
								domain[pro[0]-1][pro[1]-l] = 1 ;
							}
							l++ ;
						}
						m++ ;
					} else {
						if ((pro[0] == rule.width/2 && pro[1]-k == 0) && (board[pro[0]-1][pro[1]-k] == 0 || board[pro[0]-1][pro[1]-k] > rule.classnum)){
							domain[pro[0]-1][pro[1]-k] = 1 ;
						} else if (!(pro[0] == rule.width/2 && pro[1]-k == 0) && (board[pro[0]][pro[1]-k] == 0 || board[pro[0]][pro[1]-k] > rule.classnum)){
							domain[pro[0]][pro[1]-k] = 1 ;
						}
						k++ ;
					}
				}
			}
			k = 1 ;
			l = 1 ;
			m = 1 ;
			while (k <= rule.unit[board[pro[0]][pro[1]]].direction[3] && pro[1]+k < rule.height){
				if (board[pro[0]][pro[1]] != 13){
					if (m == 1 && (pro[0] == rule.width/2-1 && pro[1] == 0)){
						while (l <= rule.unit[board[pro[0]][pro[1]]].direction[3] && pro[1]+l<= rule.height/2-1){
							if (board[pro[0]+1][pro[1]+l] == 0){
								domain[pro[0]+1][pro[1]+l] = 1 ;
							} else if (board[pro[0]+1][pro[1]+l] > rule.classnum){
								domain[pro[0]+1][pro[1]+l] = 1 ;
								break ;
							} else {
								break ;
							}
							l++ ;
						}
						m++ ;
					} else if (!((pro[0] != 1 && pro[0] != rule.width-2) && pro[1]+k == rule.height/2)){
						if (pro[0] == rule.width/2-1 && pro[1]+k == rule.height-1){
							if (board[pro[0]+1][pro[1]+k] == 0){
								domain[pro[0]+1][pro[1]+k] = 1 ;
							} else if (board[pro[0]+1][pro[1]+k] > rule.classnum){ 
								domain[pro[0]+1][pro[1]+k] = 1 ;
								break ;
							} else {
								break ;
							}
						} else {
							if (board[pro[0]][pro[1]+k] == 0){
								domain[pro[0]][pro[1]+k] = 1 ;
							} else if (board[pro[0]][pro[1]+k] > rule.classnum){ 
								domain[pro[0]][pro[1]+k] = 1 ;
								break ;
							} else {
								break ;
							}
						}
						k++ ;
					} else {
						break ;
					}
				} else {
					if (m == 1 && (pro[0] == rule.width/2-1 && pro[1] == 0)){
						while (l <= rule.unit[board[pro[0]][pro[1]]].direction[3] && pro[1]+l < rule.height){
							if (board[pro[0]+1][pro[1]+l] == 0 || board[pro[0]+1][pro[1]+l] > rule.classnum){
								domain[pro[0]+1][pro[1]+l] = 1 ;
							}
							l++ ;
						}
						m++ ;
					} else {
						if ((pro[0] == rule.width/2-1 && pro[1]+k == rule.height-1) && (board[pro[0]+1][pro[1]+k] == 0 || board[pro[0]+1][pro[1]+k] > rule.classnum)){
							domain[pro[0]+1][pro[1]+k] = 1 ;
						} else if (!(pro[0] == rule.width/2-1 && pro[1]+k == rule.height-1) && (board[pro[0]][pro[1]+k] == 0 || board[pro[0]][pro[1]+k] > rule.classnum)){
							domain[pro[0]][pro[1]+k] = 1 ;
						}
						k++ ;
					}
				}
			}
		} else if (turn % 2 == 0 && board[pro[0]][pro[1]] > rule.classnum){
			error = 0 ;
			k = 1 ;
			l = 1 ;
			while (k <= rule.unit[board[pro[0]][pro[1]]-rule.classnum].direction[2] && pro[0]-l >= 0){
				if (board[pro[0]][pro[1]]-rule.classnum != 13){
					if ((pro[0]-l == rule.width/2 && pro[1] == 0) || (pro[0]-l == rule.width/2-1 && pro[1] == rule.height-1)){
						l++ ;
					} else {
						if (board[pro[0]-l][pro[1]] == 0){
							domain[pro[0]-l][pro[1]] = 1 ;
						} else if (board[pro[0]-l][pro[1]] > 0 && board[pro[0]-l][pro[1]] <= rule.classnum){ 
							domain[pro[0]-l][pro[1]] = 1 ;
							break ;
						} else {
							break ;
						}
						k++ ;
						l++ ;
					}
				} else {
					if ((pro[0]-l == rule.width/2 && pro[1] == 0) || (pro[0]-l == rule.width/2-1 && pro[1] == rule.height-1)){
						l++ ;
					} else {
						if (board[pro[0]-l][pro[1]] <= rule.classnum){
							domain[pro[0]-l][pro[1]] = 1 ;
						}
						k++ ;
						l++ ;
					}
				}
			}
			k = 1 ;
			l = 1 ;
			while (k <= rule.unit[board[pro[0]][pro[1]]-rule.classnum].direction[0] && pro[0]+l < rule.width){
				if (board[pro[0]][pro[1]-rule.classnum] != 13){
					if ((pro[0]+l == rule.width/2 && pro[1] == 0) || (pro[0]+l == rule.width/2-1 && pro[1] == rule.height-1)){
						l++ ;
					} else {
						if (board[pro[0]+l][pro[1]] == 0){
							domain[pro[0]+l][pro[1]] = 1 ;
						} else if (board[pro[0]+l][pro[1]] > 0 && board[pro[0]+l][pro[1]] <= rule.classnum){ 
							domain[pro[0]+l][pro[1]] = 1 ;
							break ;
						} else {
							break ;
						}
						k++ ;
						l++ ;
					}
				} else {
					if ((pro[0]+l == rule.width/2 && pro[1] == 0) || (pro[0]+l == rule.width/2-1 && pro[1] == rule.height-1)){
						l++ ;
					} else {
						if (board[pro[0]+l][pro[1]] <= rule.classnum){
							domain[pro[0]+l][pro[1]] = 1 ;
						}
						k++ ;
						l++ ;
					}
				}
			}
			k = 1 ;
			l = 1 ;
			m = 1 ;
			while (k <= rule.unit[board[pro[0]][pro[1]]-rule.classnum].direction[3] && pro[1]-k >= 0){
				if (board[pro[0]][pro[1]]-rule.classnum != 13){
					if (m == 1 && (pro[0] == rule.width/2 && pro[1] == rule.height-1)){
						while (l <= rule.unit[board[pro[0]][pro[1]]-rule.classnum].direction[3] && pro[1]-l >= rule.height/2){
							if (board[pro[0]-1][pro[1]-l] == 0){
								domain[pro[0]-1][pro[1]-l] = 1 ;
							} else if (board[pro[0]-1][pro[1]-l] > 0 && board[pro[0]-1][pro[1]-l] <= rule.classnum){
								domain[pro[0]-1][pro[1]-l] = 1 ;
								break ;
							} else {
								break ;
							}
							l++ ;
						}
						m++ ;
					} else if (!((pro[0] != 1 && pro[0] != rule.width-2) && pro[1]-k == rule.height/2-1)){
						if (pro[0] == rule.width/2 && pro[1]-k == 0){
							if (board[pro[0]-1][pro[1]-k] == 0){
								domain[pro[0]-1][pro[1]-k] = 1 ;
							} else if (board[pro[0]-1][pro[1]-k] > 0 && board[pro[0]-1][pro[1]-k] <= rule.classnum){ 
								domain[pro[0]-1][pro[1]-k] = 1 ;
								break ;
							} else {
								break ;
							}
						} else {
							if (board[pro[0]][pro[1]-k] == 0){
								domain[pro[0]][pro[1]-k] = 1 ;
							} else if (board[pro[0]][pro[1]-k] > 0 && board[pro[0]][pro[1]-k] <= rule.classnum){ 
								domain[pro[0]][pro[1]-k] = 1 ;
								break ;
							} else {
								break ;
							}
						}
						k++ ;
					} else {
						break ;
					}
				} else {
					if (m == 1 && (pro[0] == rule.width/2 && pro[1] == rule.height-1)){
						while (l <= rule.unit[board[pro[0]][pro[1]]-rule.classnum].direction[3] && pro[1]-l >= 0){
							if (board[pro[0]-1][pro[1]-l] <= rule.classnum){
								domain[pro[0]-1][pro[1]-l] = 1 ;
							}
							l++ ;
						}
						m++ ;
					} else {
						if ((pro[0] == rule.width/2 && pro[1]-k == 0) && board[pro[0]-1][pro[1]-k] <= rule.classnum){
							domain[pro[0]-1][pro[1]-k] = 1 ;
						} else if (!(pro[0] == rule.width/2 && pro[1]-k == 0) && board[pro[0]][pro[1]-k] <= rule.classnum){
							domain[pro[0]][pro[1]-k] = 1 ;
						}
						k++ ;
					}
				}
			}
			k = 1 ;
			l = 1 ;
			m = 1 ;
			while (k <= rule.unit[board[pro[0]][pro[1]]-rule.classnum].direction[1] && pro[1]+k < rule.height){
				if (board[pro[0]][pro[1]]-rule.classnum != 13){
					if (m == 1 && (pro[0] == rule.width/2-1 && pro[1] == 0)){
						while (l <= rule.unit[board[pro[0]][pro[1]]-rule.classnum].direction[1] && pro[1]+l <= rule.height/2-1){
							if (board[pro[0]+1][pro[1]+l] == 0){
								domain[pro[0]+1][pro[1]+l] = 1 ;
							} else if (board[pro[0]+1][pro[1]+l] > 0 && board[pro[0]+1][pro[1]+l] <= rule.classnum){
								domain[pro[0]+1][pro[1]+l] = 1 ;
								break ;
							} else {
								break ;
							}
							l++ ;
						}
						m++ ;
					} else if (!((pro[0] != 1 && pro[0] != rule.width-2) && pro[1]+k == rule.height/2)){
						if (pro[0] == rule.width/2-1 && pro[1]+k == rule.height-1){
							if (board[pro[0]+1][pro[1]+k] == 0){
								domain[pro[0]+1][pro[1]+k] = 1 ;
							} else if (board[pro[0]+1][pro[1]+k] > 0 && board[pro[0]+1][pro[1]+k] <= rule.classnum){ 
								domain[pro[0]+1][pro[1]+k] = 1 ;
								break ;
							} else {
								break ;
							}
						} else {
							if (board[pro[0]][pro[1]+k] == 0){
								domain[pro[0]][pro[1]+k] = 1 ;
							} else if (board[pro[0]][pro[1]+k] > 0 && board[pro[0]][pro[1]+k] <= rule.classnum){ 
								domain[pro[0]][pro[1]+k] = 1 ;
								break ;
							} else {
								break ;
							}
						}
						k++ ;
					} else {
						break ;
					}
				} else {
					if (m == 1 && (pro[0] == rule.width/2-1 && pro[1] == 0)){
						while (l <= rule.unit[board[pro[0]][pro[1]]-rule.classnum].direction[1] && pro[1]+l < rule.height){
							if (board[pro[0]+1][pro[1]+l] <= rule.classnum){
								domain[pro[0]+1][pro[1]+l] = 1 ;
							}
							l++ ;
						}
						m++ ;
					} else {
						if ((pro[0] == rule.width/2-1 && pro[1]+k == rule.height-1) && board[pro[0]+1][pro[1]+k] <= rule.classnum){
							domain[pro[0]+1][pro[1]+k] = 1 ;
						} else if (!(pro[0] == rule.width/2-1 && pro[1]+k == rule.height-1) && board[pro[0]][pro[1]+k] <= rule.classnum){
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


int movement(int pro[], int post[], int board[][16], int domain[][16], rul rule)
{
	int error = -1 ;
	
	if (pro[0] == rule.width/2 && pro[1] == 0){	
		pro[0] = rule.width/2-1 ;
	} else if (pro[0] == rule.width/2-1 && pro[1] == rule.height-1) {
		pro[0] = rule.width/2 ;
	}
	if (post[0] == rule.width/2 && post[1] == 0){	
		post[0] = rule.width/2-1 ;
	} else if (post[0] == rule.width/2-1 && post[1] == rule.height-1) {
		post[0] = rule.width/2 ;
	}
	if (domain[post[0]][post[1]] == 1){
		error = 0 ;
		fight(pro, post, board, rule) ;
	}
	return error ;
}


void fight(int pro[], int post[], int board[][16], rul rule)
{
	if (board[post[0]][post[1]] > rule.classnum){
		if ((board[post[0]][post[1]] == 16+rule.classnum && post[1] > 0) && board[post[0]][post[1]-1] > rule.classnum){
			if (rule.unit[board[pro[0]][pro[1]]].strength[board[post[0]][post[1]-1]-rule.classnum] == 1){
				board[post[0]][post[1]] = board[pro[0]][pro[1]] ;
			
			} else if (rule.unit[board[pro[0]][pro[1]]].strength[board[post[0]][post[1]-1]-rule.classnum] == 0){
				board[post[0]][post[1]] = 0 ;
			
			}
		} else {
			if (rule.unit[board[pro[0]][pro[1]]].strength[board[post[0]][post[1]]-rule.classnum] == 1){
				board[post[0]][post[1]] = board[pro[0]][pro[1]] ;
			
			} else if (rule.unit[board[pro[0]][pro[1]]].strength[board[post[0]][post[1]]-rule.classnum] == 0){
				board[post[0]][post[1]] = 0 ;
			
			}
		}
	} else if (board[post[0]][post[1]] > 0 && board[post[0]][post[1]] <= rule.classnum){
		if ((board[post[0]][post[1]] == 16 && post[1] < rule.height-1) && (board[post[0]][post[1]+1] > 0 && board[post[0]][post[1]+1] <= rule.classnum)){
			if (rule.unit[board[pro[0]][pro[1]]-rule.classnum].strength[board[post[0]][post[1]+1]] == 1){
				board[post[0]][post[1]] = board[pro[0]][pro[1]] ;
			
			} else if (rule.unit[board[pro[0]][pro[1]]-rule.classnum].strength[board[post[0]][post[1]+1]] == 0){
				board[post[0]][post[1]] = 0 ;
				
			}
		} else {
			if (rule.unit[board[pro[0]][pro[1]]-rule.classnum].strength[board[post[0]][post[1]]] == 1){
				board[post[0]][post[1]] = board[pro[0]][pro[1]] ;
			
			} else if (rule.unit[board[pro[0]][pro[1]]-rule.classnum].strength[board[post[0]][post[1]]] == 0){
				board[post[0]][post[1]] = 0 ;
				
			}
		}
	} else {
		board[post[0]][post[1]] = board[pro[0]][pro[1]] ;
		
	}
	board[pro[0]][pro[1]] = 0 ;
	
	return ;
}


int loadrule(rul *rule, char datname[])
{
	FILE *dat ;
	int error = -1 ;
	int n, i ;
	
	dat = fopen(datname, "rb") ;
	
	if (dat != NULL){
		error = 0 ;
		READDAT(rule->classnum) ;
		READDAT(rule->height) ;
		READDAT(rule->width) ;
		
		for (n = 0 ; n <= rule->classnum ; n++){
			READDAT(rule->unit[n].name) ;
		
			for (i = 0 ; i < 4 ; i++){
				READDAT(rule->unit[n].direction[i]) ;
			}
			for (i = 0 ; i <= rule->classnum ; i++){
				READDAT(rule->unit[n].strength[i]) ;
			}
			
			READDAT(rule->unit[n].number) ;
		}
	
		fclose(dat) ;
	}
	return error ;
}


void saverule(rul rule, char datname[])
{
	FILE *dat ;
	int n, i ;
	
	dat = fopen(datname, "wb") ;
	
	WRITEDAT(rule.classnum) ;
	WRITEDAT(rule.height) ;
	WRITEDAT(rule.width) ;
	
	for (n = 0 ; n <= rule.classnum ; n++){
		WRITEDAT(rule.unit[n].name) ;
		
		for (i = 0 ; i < 4 ; i++){
			WRITEDAT(rule.unit[n].direction[i]) ;
		}
		for (i = 0 ; i <= rule.classnum ; i++){
			WRITEDAT(rule.unit[n].strength[i]) ;
		}

		WRITEDAT(rule.unit[n].number) ;
	}
	
	fclose(dat) ;
	
	return ;
}


void initialrule(void)
{
	char datname[] = "initialrule.dat" ;
	rul inirule ;

	FILE *dat ;
	int n, i ;
	
	inirule.classnum = 16 ;
	inirule.height = 8 ;
	inirule.width = 6 ;
	
	/* Null */
	strcpy(inirule.unit[0].name, "   ") ;
	
	inirule.unit[0].direction[0] = 0 ;
	inirule.unit[0].direction[1] = 0 ;
	inirule.unit[0].direction[2] = 0 ;
	inirule.unit[0].direction[3] = 0 ;
	
	inirule.unit[0].strength[0] = 0 ;
	inirule.unit[0].strength[1] = -1 ;
	inirule.unit[0].strength[2] = -1 ;
	inirule.unit[0].strength[3] = -1 ;
	inirule.unit[0].strength[4] = -1 ;
	inirule.unit[0].strength[5] = -1 ;
	inirule.unit[0].strength[6] = -1 ;
	inirule.unit[0].strength[7] = -1 ;
	inirule.unit[0].strength[8] = -1 ;
	inirule.unit[0].strength[9] = -1 ;
	inirule.unit[0].strength[10] = -1 ;
	inirule.unit[0].strength[11] = -1 ;
	inirule.unit[0].strength[12] = -1 ;
	inirule.unit[0].strength[13] = -1 ;
	inirule.unit[0].strength[14] = -1 ;
	inirule.unit[0].strength[15] = -1 ;
	inirule.unit[0].strength[16] = -1 ;
	
	inirule.unit[0].number = 0 ;
	
	/* 大将 */
	strcpy(inirule.unit[1].name, "TSh") ;
	
	inirule.unit[1].direction[0] = 1 ;
	inirule.unit[1].direction[1] = 1 ;
	inirule.unit[1].direction[2] = 1 ;
	inirule.unit[1].direction[3] = 1 ;
	
	inirule.unit[1].strength[0] = 1 ;
	inirule.unit[1].strength[1] = 0 ;
	inirule.unit[1].strength[2] = 1 ;
	inirule.unit[1].strength[3] = 1 ;
	inirule.unit[1].strength[4] = 1 ;
	inirule.unit[1].strength[5] = 1 ;
	inirule.unit[1].strength[6] = 1 ;
	inirule.unit[1].strength[7] = 1 ;
	inirule.unit[1].strength[8] = 1 ;
	inirule.unit[1].strength[9] = 1 ;
	inirule.unit[1].strength[10] = 1 ;
	inirule.unit[1].strength[11] = 1 ;
	inirule.unit[1].strength[12] = 0 ;
	inirule.unit[1].strength[13] = 1 ;
	inirule.unit[1].strength[14] = 1 ;
	inirule.unit[1].strength[15] = -1 ;
	inirule.unit[1].strength[16] = 1 ;

	inirule.unit[1].number = 1 ;	

	/* 中将 */
	strcpy(inirule.unit[2].name, "CJo") ;
	
	inirule.unit[2].direction[0] = 1 ;
	inirule.unit[2].direction[1] = 1 ;
	inirule.unit[2].direction[2] = 1 ;
	inirule.unit[2].direction[3] = 1 ;

	inirule.unit[2].strength[0] = 1 ;	
	inirule.unit[2].strength[1] = -1 ;
	inirule.unit[2].strength[2] = 0 ;
	inirule.unit[2].strength[3] = 1 ;
	inirule.unit[2].strength[4] = 1 ;
	inirule.unit[2].strength[5] = 1 ;
	inirule.unit[2].strength[6] = 1 ;
	inirule.unit[2].strength[7] = 1 ;
	inirule.unit[2].strength[8] = 1 ;
	inirule.unit[2].strength[9] = 1 ;
	inirule.unit[2].strength[10] = 1 ;
	inirule.unit[2].strength[11] = 1 ;
	inirule.unit[2].strength[12] = 0 ;
	inirule.unit[2].strength[13] = 1 ;
	inirule.unit[2].strength[14] = 1 ;
	inirule.unit[2].strength[15] = 1 ;
	inirule.unit[2].strength[16] = 1 ;

	inirule.unit[2].number = 1 ;	
	
	/* 少将 */
	strcpy(inirule.unit[3].name, "SSh") ;
	
	inirule.unit[3].direction[0] = 1 ;
	inirule.unit[3].direction[1] = 1 ;
	inirule.unit[3].direction[2] = 1 ;
	inirule.unit[3].direction[3] = 1 ;

	inirule.unit[3].strength[0] = 1 ;	
	inirule.unit[3].strength[1] = -1 ;
	inirule.unit[3].strength[2] = -1 ;
	inirule.unit[3].strength[3] = 0 ;
	inirule.unit[3].strength[4] = 1 ;
	inirule.unit[3].strength[5] = 1 ;
	inirule.unit[3].strength[6] = 1 ;
	inirule.unit[3].strength[7] = 1 ;
	inirule.unit[3].strength[8] = 1 ;
	inirule.unit[3].strength[9] = 1 ;
	inirule.unit[3].strength[10] = 1 ;
	inirule.unit[3].strength[11] = 1 ;
	inirule.unit[3].strength[12] = 0 ;
	inirule.unit[3].strength[13] = 1 ;
	inirule.unit[3].strength[14] = 1 ;
	inirule.unit[3].strength[15] = 1 ;
	inirule.unit[3].strength[16] = 1 ;

	inirule.unit[3].number = 1 ;	
	
	/* 大佐 */
	strcpy(inirule.unit[4].name, "TSa") ;
	
	inirule.unit[4].direction[0] = 1 ;
	inirule.unit[4].direction[1] = 1 ;
	inirule.unit[4].direction[2] = 1 ;
	inirule.unit[4].direction[3] = 1 ;

	inirule.unit[4].strength[0] = 1 ;	
	inirule.unit[4].strength[1] = -1 ;
	inirule.unit[4].strength[2] = -1 ;
	inirule.unit[4].strength[3] = -1 ;
	inirule.unit[4].strength[4] = 0 ;
	inirule.unit[4].strength[5] = 1 ;
	inirule.unit[4].strength[6] = 1 ;
	inirule.unit[4].strength[7] = 1 ;
	inirule.unit[4].strength[8] = 1 ;
	inirule.unit[4].strength[9] = 1 ;
	inirule.unit[4].strength[10] = 1 ;
	inirule.unit[4].strength[11] = 1 ;
	inirule.unit[4].strength[12] = 0 ;
	inirule.unit[4].strength[13] = -1 ;
	inirule.unit[4].strength[14] = -1 ;
	inirule.unit[4].strength[15] = 1 ;
	inirule.unit[4].strength[16] = 1 ;

	inirule.unit[4].number = 1 ;
	
	/* 中佐 */
	strcpy(inirule.unit[5].name, "CSa") ;
	
	inirule.unit[5].direction[0] = 1 ;
	inirule.unit[5].direction[1] = 1 ;
	inirule.unit[5].direction[2] = 1 ;
	inirule.unit[5].direction[3] = 1 ;

	inirule.unit[5].strength[0] = 1 ;	
	inirule.unit[5].strength[1] = -1 ;
	inirule.unit[5].strength[2] = -1 ;
	inirule.unit[5].strength[3] = -1 ;
	inirule.unit[5].strength[4] = -1 ;
	inirule.unit[5].strength[5] = 0 ;
	inirule.unit[5].strength[6] = 1 ;
	inirule.unit[5].strength[7] = 1 ;
	inirule.unit[5].strength[8] = 1 ;
	inirule.unit[5].strength[9] = 1 ;
	inirule.unit[5].strength[10] = 1 ;
	inirule.unit[5].strength[11] = 1 ;
	inirule.unit[5].strength[12] = 0 ;
	inirule.unit[5].strength[13] = -1 ;
	inirule.unit[5].strength[14] = -1 ;
	inirule.unit[5].strength[15] = 1 ;
	inirule.unit[5].strength[16] = 1 ;

	inirule.unit[5].number = 1 ;	
	
	/* 少佐 */
	strcpy(inirule.unit[6].name, "SSa") ;
	
	inirule.unit[6].direction[0] = 1 ;
	inirule.unit[6].direction[1] = 1 ;
	inirule.unit[6].direction[2] = 1 ;
	inirule.unit[6].direction[3] = 1 ;

	inirule.unit[6].strength[0] = 1 ;
	inirule.unit[6].strength[1] = -1 ;
	inirule.unit[6].strength[2] = -1 ;
	inirule.unit[6].strength[3] = -1 ;
	inirule.unit[6].strength[4] = -1 ;
	inirule.unit[6].strength[5] = -1 ;
	inirule.unit[6].strength[6] = 0 ;
	inirule.unit[6].strength[7] = 1 ;
	inirule.unit[6].strength[8] = 1 ;
	inirule.unit[6].strength[9] = 1 ;
	inirule.unit[6].strength[10] = 1 ;
	inirule.unit[6].strength[11] = 1 ;
	inirule.unit[6].strength[12] = 0 ;
	inirule.unit[6].strength[13] = -1 ;
	inirule.unit[6].strength[14] = -1 ;
	inirule.unit[6].strength[15] = 1 ;
	inirule.unit[6].strength[16] = 1 ;

	inirule.unit[6].number = 1 ;	
	
	/* 大尉 */
	strcpy(inirule.unit[7].name, "TaI") ;
	
	inirule.unit[7].direction[0] = 1 ;
	inirule.unit[7].direction[1] = 1 ;
	inirule.unit[7].direction[2] = 1 ;
	inirule.unit[7].direction[3] = 1 ;

	inirule.unit[7].strength[0] = 1 ;	
	inirule.unit[7].strength[1] = -1 ;
	inirule.unit[7].strength[2] = -1 ;
	inirule.unit[7].strength[3] = -1 ;
	inirule.unit[7].strength[4] = -1 ;
	inirule.unit[7].strength[5] = -1 ;
	inirule.unit[7].strength[6] = -1 ;
	inirule.unit[7].strength[7] = 0 ;
	inirule.unit[7].strength[8] = 1 ;
	inirule.unit[7].strength[9] = 1 ;
	inirule.unit[7].strength[10] = 1 ;
	inirule.unit[7].strength[11] = 1 ;
	inirule.unit[7].strength[12] = 0 ;
	inirule.unit[7].strength[13] = -1 ;
	inirule.unit[7].strength[14] = -1 ;
	inirule.unit[7].strength[15] = 1 ;
	inirule.unit[7].strength[16] = 1 ;

	inirule.unit[7].number = 2 ;
	
	/* 中尉 */
	strcpy(inirule.unit[8].name, "ChI") ;
	
	inirule.unit[8].direction[0] = 1 ;
	inirule.unit[8].direction[1] = 1 ;
	inirule.unit[8].direction[2] = 1 ;
	inirule.unit[8].direction[3] = 1 ;

	inirule.unit[8].strength[0] = 1 ;
	inirule.unit[8].strength[1] = -1 ;
	inirule.unit[8].strength[2] = -1 ;
	inirule.unit[8].strength[3] = -1 ;
	inirule.unit[8].strength[4] = -1 ;
	inirule.unit[8].strength[5] = -1 ;
	inirule.unit[8].strength[6] = -1 ;
	inirule.unit[8].strength[7] = -1 ;
	inirule.unit[8].strength[8] = 0 ;
	inirule.unit[8].strength[9] = 1 ;
	inirule.unit[8].strength[10] = 1 ;
	inirule.unit[8].strength[11] = 1 ;
	inirule.unit[8].strength[12] = 0 ;
	inirule.unit[8].strength[13] = -1 ;
	inirule.unit[8].strength[14] = -1 ;
	inirule.unit[8].strength[15] = 1 ;
	inirule.unit[8].strength[16] = 1 ;

	inirule.unit[8].number = 2 ;
	
	/* 少尉 */
	strcpy(inirule.unit[9].name, "ShI") ;
	
	inirule.unit[9].direction[0] = 1 ;
	inirule.unit[9].direction[1] = 1 ;
	inirule.unit[9].direction[2] = 1 ;
	inirule.unit[9].direction[3] = 1 ;

	inirule.unit[9].strength[0] = 1 ;	
	inirule.unit[9].strength[1] = -1 ;
	inirule.unit[9].strength[2] = -1 ;
	inirule.unit[9].strength[3] = -1 ;
	inirule.unit[9].strength[4] = -1 ;
	inirule.unit[9].strength[5] = -1 ;
	inirule.unit[9].strength[6] = -1 ;
	inirule.unit[9].strength[7] = -1 ;
	inirule.unit[9].strength[8] = -1 ;
	inirule.unit[9].strength[9] = 0 ;
	inirule.unit[9].strength[10] = 1 ;
	inirule.unit[9].strength[11] = 1 ;
	inirule.unit[9].strength[12] = 0 ;
	inirule.unit[9].strength[13] = -1 ;
	inirule.unit[9].strength[14] = -1 ;
	inirule.unit[9].strength[15] = 1 ;
	inirule.unit[9].strength[16] = 1 ;

	inirule.unit[9].number = 2 ;
	
	/* 騎兵 */
	strcpy(inirule.unit[10].name, "KiH") ;
	
	inirule.unit[10].direction[0] = 1 ;
	inirule.unit[10].direction[1] = 2 ;
	inirule.unit[10].direction[2] = 1 ;
	inirule.unit[10].direction[3] = 1 ;

	inirule.unit[10].strength[0] = 1 ;	
	inirule.unit[10].strength[1] = -1 ;
	inirule.unit[10].strength[2] = -1 ;
	inirule.unit[10].strength[3] = -1 ;
	inirule.unit[10].strength[4] = -1 ;
	inirule.unit[10].strength[5] = -1 ;
	inirule.unit[10].strength[6] = -1 ;
	inirule.unit[10].strength[7] = -1 ;
	inirule.unit[10].strength[8] = -1 ;
	inirule.unit[10].strength[9] = -1 ;
	inirule.unit[10].strength[10] = 0 ;
	inirule.unit[10].strength[11] = 1 ;
	inirule.unit[10].strength[12] = 0 ;
	inirule.unit[10].strength[13] = -1 ;
	inirule.unit[10].strength[14] = -1 ;
	inirule.unit[10].strength[15] = 1 ;
	inirule.unit[10].strength[16] = 1 ;
	
	inirule.unit[10].number = 1 ;	
	
	/* 工兵 */
	strcpy(inirule.unit[11].name, "KoH") ;
	
	inirule.unit[11].direction[0] = 15 ;
	inirule.unit[11].direction[1] = 15 ;
	inirule.unit[11].direction[2] = 15 ;
	inirule.unit[11].direction[3] = 15 ;

	inirule.unit[11].strength[0] = 1 ;	
	inirule.unit[11].strength[1] = -1 ;
	inirule.unit[11].strength[2] = -1 ;
	inirule.unit[11].strength[3] = -1 ;
	inirule.unit[11].strength[4] = -1 ;
	inirule.unit[11].strength[5] = -1 ;
	inirule.unit[11].strength[6] = -1 ;
	inirule.unit[11].strength[7] = -1 ;
	inirule.unit[11].strength[8] = -1 ;
	inirule.unit[11].strength[9] = -1 ;
	inirule.unit[11].strength[10] = -1 ;
	inirule.unit[11].strength[11] = 0 ;
	inirule.unit[11].strength[12] = 1 ;
	inirule.unit[11].strength[13] = -1 ;
	inirule.unit[11].strength[14] = 1 ;
	inirule.unit[11].strength[15] = 1 ;
	inirule.unit[11].strength[16] = 1 ;
	
	inirule.unit[11].number = 2 ;	
	
	/* 地雷 */
	strcpy(inirule.unit[12].name, "JiR") ;
	
	inirule.unit[12].direction[0] = 0 ;
	inirule.unit[12].direction[1] = 0 ;
	inirule.unit[12].direction[2] = 0 ;
	inirule.unit[12].direction[3] = 0 ;

	inirule.unit[12].strength[0] = 1 ;	
	inirule.unit[12].strength[1] = 0 ;
	inirule.unit[12].strength[2] = 0 ;
	inirule.unit[12].strength[3] = 0 ;
	inirule.unit[12].strength[4] = 0 ;
	inirule.unit[12].strength[5] = 0 ;
	inirule.unit[12].strength[6] = 0 ;
	inirule.unit[12].strength[7] = 0 ;
	inirule.unit[12].strength[8] = 0 ;
	inirule.unit[12].strength[9] = 0 ;
	inirule.unit[12].strength[10] = 0 ;
	inirule.unit[12].strength[11] = -1 ;
	inirule.unit[12].strength[12] = 0 ;
	inirule.unit[12].strength[13] = -1 ;
	inirule.unit[12].strength[14] = 0 ;
	inirule.unit[12].strength[15] = 0 ;
	inirule.unit[12].strength[16] = 1 ;

	inirule.unit[12].number = 2 ;
	
	/* 飛行機 */
	strcpy(inirule.unit[13].name, "HiK") ;
	
	inirule.unit[13].direction[0] = 1 ;
	inirule.unit[13].direction[1] = 15 ;
	inirule.unit[13].direction[2] = 1 ;
	inirule.unit[13].direction[3] = 15 ;

	inirule.unit[13].strength[0] = 1 ;	
	inirule.unit[13].strength[1] = -1 ;
	inirule.unit[13].strength[2] = -1 ;
	inirule.unit[13].strength[3] = -1 ;
	inirule.unit[13].strength[4] = 1 ;
	inirule.unit[13].strength[5] = 1 ;
	inirule.unit[13].strength[6] = 1 ;
	inirule.unit[13].strength[7] = 1 ;
	inirule.unit[13].strength[8] = 1 ;
	inirule.unit[13].strength[9] = 1 ;
	inirule.unit[13].strength[10] = 1 ;
	inirule.unit[13].strength[11] = 1 ;
	inirule.unit[13].strength[12] = 1 ;
	inirule.unit[13].strength[13] = 0 ;
	inirule.unit[13].strength[14] = 1 ;
	inirule.unit[13].strength[15] = 1 ;
	inirule.unit[13].strength[16] = 1 ;

	inirule.unit[13].number = 2 ;	
	
	/* タンク */
	strcpy(inirule.unit[14].name, "Tnk") ;
	
	inirule.unit[14].direction[0] = 1 ;
	inirule.unit[14].direction[1] = 2 ;
	inirule.unit[14].direction[2] = 1 ;
	inirule.unit[14].direction[3] = 1 ;

	inirule.unit[14].strength[0] = 1 ;	
	inirule.unit[14].strength[1] = -1 ;
	inirule.unit[14].strength[2] = -1 ;
	inirule.unit[14].strength[3] = -1 ;
	inirule.unit[14].strength[4] = 1 ;
	inirule.unit[14].strength[5] = 1 ;
	inirule.unit[14].strength[6] = 1 ;
	inirule.unit[14].strength[7] = 1 ;
	inirule.unit[14].strength[8] = 1 ;
	inirule.unit[14].strength[9] = 1 ;
	inirule.unit[14].strength[10] = 1 ;
	inirule.unit[14].strength[11] = -1 ;
	inirule.unit[14].strength[12] = 0 ;
	inirule.unit[14].strength[13] = -1 ;
	inirule.unit[14].strength[14] = 0 ;
	inirule.unit[14].strength[15] = 1 ;
	inirule.unit[14].strength[16] = 1 ;

	inirule.unit[14].number = 2 ;	
	
	/* スパイ */
	strcpy(inirule.unit[15].name, "Spy") ;
	
	inirule.unit[15].direction[0] = 1 ;
	inirule.unit[15].direction[1] = 1 ;
	inirule.unit[15].direction[2] = 1 ;
	inirule.unit[15].direction[3] = 1 ;

	inirule.unit[15].strength[0] = 1 ;	
	inirule.unit[15].strength[1] = 1 ;
	inirule.unit[15].strength[2] = -1 ;
	inirule.unit[15].strength[3] = -1 ;
	inirule.unit[15].strength[4] = -1 ;
	inirule.unit[15].strength[5] = -1 ;
	inirule.unit[15].strength[6] = -1 ;
	inirule.unit[15].strength[7] = -1 ;
	inirule.unit[15].strength[8] = -1 ;
	inirule.unit[15].strength[9] = -1 ;
	inirule.unit[15].strength[10] = -1 ;
	inirule.unit[15].strength[11] = -1 ;
	inirule.unit[15].strength[12] = -1 ;
	inirule.unit[15].strength[13] = -1 ;
	inirule.unit[15].strength[14] = -1 ;
	inirule.unit[15].strength[15] = 0 ;
	inirule.unit[15].strength[16] = 1 ;

	inirule.unit[15].number = 1 ;
	
	/* 軍旗 */
	strcpy(inirule.unit[16].name, "GnK") ;
	
	inirule.unit[16].direction[0] = 0 ;
	inirule.unit[16].direction[1] = 0 ;
	inirule.unit[16].direction[2] = 0 ;
	inirule.unit[16].direction[3] = 0 ;

	inirule.unit[16].strength[0] = 1 ;	
	inirule.unit[16].strength[1] = -1 ;
	inirule.unit[16].strength[2] = -1 ;
	inirule.unit[16].strength[3] = -1 ;
	inirule.unit[16].strength[4] = -1 ;
	inirule.unit[16].strength[5] = -1 ;
	inirule.unit[16].strength[6] = -1 ;
	inirule.unit[16].strength[7] = -1 ;
	inirule.unit[16].strength[8] = -1 ;
	inirule.unit[16].strength[9] = -1 ;
	inirule.unit[16].strength[10] = -1 ;
	inirule.unit[16].strength[11] = -1 ;
	inirule.unit[16].strength[12] = -1 ;
	inirule.unit[16].strength[13] = -1 ;
	inirule.unit[16].strength[14] = -1 ;
	inirule.unit[16].strength[15] = -1 ;
	inirule.unit[16].strength[16] = 0 ;

	inirule.unit[16].number = 1 ;
	
	dat = fopen(datname, "wb") ;
	
	WRITEDAT(inirule.classnum) ;
	WRITEDAT(inirule.height) ;
	WRITEDAT(inirule.width) ;
	
	for (n = 0 ; n <= inirule.classnum ; n++){
		WRITEDAT(inirule.unit[n].name) ;
		
		for (i = 0 ; i < 4 ; i++){
			WRITEDAT(inirule.unit[n].direction[i]) ;
		}
		for (i = 0 ; i <= inirule.classnum ; i++){
			WRITEDAT(inirule.unit[n].strength[i]) ;
		}
		
		WRITEDAT(inirule.unit[n].number) ;
	}
	
	fclose(dat) ;
	
	return ;
}


// #include <stdio.h>
#include "writePost.c"
#include "showPost.c"
#include "myPost.c"
#include <sys/wait.h>
#include <sys/types.h>
#include "sugang_doumi.h"

int main() {
	int input_num;
	
	while (1) {
		printf("----------------------------------------\n");
		printf("1. 게시글 보기\n");
		printf("2. 게시글 작성\n");
		printf("3. 내가 쓴 글 보기\n");
		printf("4. 메뉴 화면으로 돌아가기\n");
		printf("----------------------------------------\n");
	
		while (1) {
			printf(": ");
			if ((scanf("%d", &input_num) != 1) || (input_num > 4) || (input_num < 1)) {
				getchar();
				printf("잘못된 입력입니다. 1, 2, 3, 4 중 하나를 입력하세요.\n");
				continue;
			}
			else {
				getchar();
				break;
			}
		}
	
		switch(input_num) {
			case 1:
				clear_terminal();
				show_post();
				clear_terminal();
				continue;
			case 2:
				clear_terminal();
				write_post();
				sleep(1);
				clear_terminal();
				continue;
			case 3:
				clear_terminal();
				my_post();
				clear_terminal();
				continue;
			case 4:
				clear_terminal();
				return 0;
			default:
				clear_terminal();
				break;
		}
	
	}
	return 0;	
}
	
int clear_terminal() {
	int pid;

	pid = fork();
	if(pid == 0){
		execlp("clear", "clear", NULL);
	}
	wait(NULL);
}

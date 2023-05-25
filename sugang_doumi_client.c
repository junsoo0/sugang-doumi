#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <termios.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "sugang_doumi.h"

#define MAX_SIZE 100
#define oops(msg) {perror(msg); exit(1);}

enum initial_choice {
	LOGIN = 1, FIND_PASSWD, SIGN_UP, INITIAL_EXIT
};

enum main_choice {
	CALCULATE_GPA = 1, EVALUATE_LECTURE, MANAGE_SCHEDULE, CHANGE_PASSWD, LOGOUT, MAIN_EXIT
};

enum credit_choice {
	CREDIT_ADD = 1, CREDIT_REMOVE, CREDIT_EXIT
};

int initial_UI();
int main_UI();
int check_valid_input(char* input, int total_num);
int find_passwd();
int change_passwd();
int logout();
int manage_schedule();
int credit_UI();
int credit_add();
int credit_remove();
int credit_list();
int clear_terminal();
void inthandler(int s);
void quithandler(int s);
void *client_func(void *arg);

int main(int argc, char *argv[]) {
	int sock, input;
	struct sockaddr_in serv_addr;
	pthread_t recv_thread;
	void *thread_return;
	
	if(argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	 }
	
	signal(SIGINT, inthandler);
	signal(SIGQUIT, quithandler);

	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	  
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		oops("connect() error");

	pthread_create(&recv_thread, NULL, client_func, (void*)&sock);
	pthread_join(recv_thread, &thread_return);

	close(sock);
	printf("Client closed\n");

	return 0;
}

void *client_func(void *arg)   // read thread main
{
	int choice;
	int check = -1;
	int sock = *((int*)arg);

	while (check == -1) {
initial:	
		clear_terminal();
			
		choice = initial_UI();
		if (choice == INITIAL_EXIT)
			exit(0);
		write(sock, &choice, sizeof(choice));
		switch (choice) {
			case LOGIN:
				check = login_c(sock);
				break;
			case FIND_PASSWD:
				find_passwd_c(sock);
				break;
			case SIGN_UP:
				sign_up_c(sock);
				break;
		}
	}
	
	while (1) {
		clear_terminal();

		choice = main_UI(sock);
		switch (choice) {
			case CALCULATE_GPA:{
				int credit_condition = 1;
				while(credit_condition){
				clear_terminal();
				credit_list();
				int credit_choice = credit_UI();
				switch(credit_choice) {
					case CREDIT_ADD:
							credit_add();
							break;
					case CREDIT_REMOVE:
							credit_remove();
							break;
					case CREDIT_EXIT:
							credit_condition = 0;
							break;
				}
				}
				
				break;
				}
			case EVALUATE_LECTURE:
				break;
			case MANAGE_SCHEDULE:
				manage_schedule();
				break;
			case CHANGE_PASSWD:
				change_passwd();
				break;
			case LOGOUT: 
				if(logout()) goto initial;
				break;
			case MAIN_EXIT:
				break;
		}
	}
}

int initial_UI() {
	char input[INPUT_SIZE];

	while (1) {
		printf("========================================\n");
		printf("               수강 도우미              \n");
		printf("========================================\n");
		printf("[1] 로그인\n");
		printf("[2] 비밀번호 찾기\n");
		printf("[3] 회원가입\n");
		printf("[4] 종료\n");
		printf("----------------------------------------\n");
		printf("선택: [1 - 4] ");
		
		fgets(input, INPUT_SIZE, stdin);
		input[strlen(input) - 1] = '\0';
		if (check_valid_input(input, 4) == FALSE)
			continue;
		puts("");
		
		return input[0] - '0';
	}
}

int main_UI(int sock) {
	char input[INPUT_SIZE];

	printf("[메인 화면]\n");
	while (1) {
		show_schedule_c(sock, 3);
		printf("========================================\n");
		printf("[1] 학점 계산\n");
		printf("[2] 강의 평가\n");
		printf("[3] 일정 관리\n");
		printf("[4] 비밀번호 변경\n");
		printf("[5] 로그아웃\n");
		printf("[6] 종료\n");
		printf("----------------------------------------\n");
		printf("선택: [1 - 6] ");

		fgets(input, INPUT_SIZE, stdin);
		input[strlen(input) - 1] = '\0';
		if (check_valid_input(input, 6) == FALSE)
			continue;
		puts("");
		
		return input[0] - '0';
	}
}

int credit_UI() {
	char input[INPUT_SIZE];

	printf("[학점 계산 메뉴]\n");
	while (1) {
		printf("==============================================\n");
		printf("[1] 학점 등록\n");
		printf("[2] 학점 삭제\n");
		printf("[3] 돌아가기\n");
		printf("----------------------------------------------\n");
		printf("선택: [1 - 3] ");

		fgets(input, INPUT_SIZE, stdin);
		input[strlen(input) - 1] = '\0';
		if (check_valid_input(input, 3) == FALSE)
			continue;
		puts("");
		
		return input[0] - '0';
	}
}

int check_valid_input(char* input, int total_num) {
	if (strlen(input) != 1 || (input[0] - '0') < 1 || (input[0] - '0') > total_num) {
		puts("");
		printf("잘못된 입력입니다.\n");
		printf("다시 입력해주세요.\n");
		puts("");
		return FALSE;
	}
	return TRUE;
}

int clear_terminal() {
	int pid;

	pid = fork();
	if(pid == 0){
		execlp("clear", "clear", NULL);
	}
	wait(NULL);

	return 0;
}

void inthandler(int s){
	puts("");
	puts("");
	printf("Ctrl + C 입력은 허용되지 않습니다.\n");
	printf("종료를 원하시면 Ctrl + \\ 를 입력하세요.\n");	
	sleep(3);
}

void quithandler(int s){
	struct termios info;
	tcgetattr(0, &info);
	
	puts("");
	puts("");
	printf("프로그램을 종료합니다.\n");
	info.c_lflag |= ECHO;
	tcsetattr(0, TCSANOW, &info);
	exit(0);
	
}



#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <termios.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>
#include "sugang_doumi.h"

#define MAX_CLNT 256
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

void *handle_clnt(void *arg);
void server_func(int clnt_sock);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char *argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;

	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
  
	pthread_mutex_init(&mutx, NULL);
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET; 
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		oops("bind() error");

	if(listen(serv_sock, 5)==-1)
		oops("listen() error");
	
	printf("========================================\n");
	printf("            수강 도우미 서버            \n");
	printf("========================================\n");

	while(1)
	{
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);
		
		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutx);
	
		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id);
		printf("Connected client IP: %s, clnt_sock=%d\n", inet_ntoa(clnt_adr.sin_addr), clnt_sock);
	}
	
	close(serv_sock);
	pthread_mutex_destroy(&mutx);
	
	return 0;
}
void *handle_clnt(void *arg)
{
	int clnt_sock = *((int*)arg);
	int i;

	server_func(clnt_sock);
	pthread_mutex_lock(&mutx);
	for(i = 0; i < clnt_cnt; i++)   // remove disconnected client
	{
		if(clnt_sock == clnt_socks[i])
		{
			while(i < clnt_cnt)
			{
				clnt_socks[i] = clnt_socks[i + 1];
				i++;
			}
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);

	close(clnt_sock);
	printf("clnt_sock: %d closed.\n", clnt_sock);

	return NULL;
}

void server_func(int clnt_sock) {
	int choice;
	int check = -1;

	getcwd(home_path, INPUT_SIZE);
	while (check == -1) {
		read(clnt_sock, &choice, sizeof(choice));
		switch (choice) {
			case LOGIN:
				check = login_s(clnt_sock);
				break;
			case FIND_PASSWD:
				find_passwd_s(clnt_sock);
				break;
			case SIGN_UP:
				sign_up_s(clnt_sock);
				break;
		}
	}
	
	while (1) {
		choice = main_UI(clnt_sock);
		switch (choice) {
			case CALCULATE_GPA:{
				int credit_condition = 1;
				while(credit_condition){
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
				manage_schedule_s(clnt_sock);
				break;
			case CHANGE_PASSWD:
				change_passwd();
				break;
			case LOGOUT: 
				if(logout());
				break;
			case MAIN_EXIT:
				break;
		}
	}

	return 0;
}

int main_UI(int clnt_sock) {
	char input[INPUT_SIZE];

	while (1) {
		show_schedule_s(clnt_sock, 3);
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
	char ans;
	int check = 1;
	struct termios info;
	tcgetattr(0, &info);
	
	puts("");
	puts("");
	printf("프로그램을 종료합니다.\n");
	info.c_lflag |= ECHO;
	tcsetattr(0, TCSANOW, &info);
	exit(0);
	
}



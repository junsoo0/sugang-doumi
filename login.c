#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include "sugang_doumi.h" // FOLDER_PERMISSION, FILE_PERMMISION, hoem_path 포함

int login(){
	char buf[INPUT_SIZE];
	char login_id[INPUT_SIZE];
	char passwd[INPUT_SIZE];
	char ans;
	struct termios info;
	tcgetattr(0, &info);
	int pid = fork();
	if(pid == 0){
		execlp("clear", "clear", NULL);
	}
	wait(NULL);

	printf("ID를 입력하세요(학번): ");
	scanf("%s", login_id);
	getchar();

	if(chdir(login_id) == -1){
		printf("존재하지 않는 ID(학번)입니다.\n");
	        printf("3초 후 메뉴로 돌아갑니다.\n");
       		sleep(3);
			chdir(home_path);
		return -1;		
	}
	else{
label:
		puts("");
		printf("비밀번호를 입력하세요: ");
		info.c_lflag &= ~ECHO;
		tcsetattr(0, TCSANOW, &info);
		scanf("%s", passwd);
		getchar();

		info.c_lflag |= ECHO;
		tcsetattr(0, TCSANOW, &info);
		
		// 프로그램 재실행시 buf가 초기화가 안됨 그래서 초기화 해줘야함
		for(int i = 0; i < INPUT_SIZE; i++) buf[i] = '\0';
		int passwd_fd = open("passwd", O_RDONLY);
		read(passwd_fd, buf, INPUT_SIZE);
		close(passwd_fd);
	
		if(strcmp(buf, passwd) == 0){
			puts("");
			printf("로그인 성공.\n");
			// 무한루프 깨고 로그인 성공 후 화면으로 이동
			sprintf(user_path, "%s/%s", home_path, login_id);
			return 0;
		}
		else {
			while(1){
				puts("");
				printf("비밀번호가 일치하지 않습니다.\n");
				printf("메뉴로 돌아가시겠습니까? (Y/N) : ");
				ans = getchar();
				getchar();

				switch(ans){
					case 'Y' : 
					case 'y' :
						chdir(home_path);
						return -1;
					case 'N' : 
					case 'n' :
						goto label;
					default: break;	   
				}
			}
		}
	}

	return 0;
}

int sign_up(){
	char sign_up_id[INPUT_SIZE];
	char passwd[INPUT_SIZE];
	char name[INPUT_SIZE];
	char school[INPUT_SIZE];
	struct termios info;
	tcgetattr(0, &info);
	int pid = fork();
	if(pid == 0){
		execlp("clear", "clear", NULL);
	}
	wait(NULL);

	printf("회원가입 하실 ID를 입력하세요(학번): ");
	scanf("%s", sign_up_id);
	getchar();
	if(chdir(sign_up_id) != -1){
		printf("이미 존재하는 ID입니다.\n");
		printf("3초 후 메뉴로 돌아갑니다.\n");
		sleep(3);
		chdir(home_path);
		return -1;
	}
	else
		mkdir(sign_up_id, FOLDER_PERMISSION);

	chdir(sign_up_id);
	puts("");
	printf("비밀번호를 설정하세요: ");
	info.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &info);
	scanf("%s", passwd);
	getchar();

	info.c_lflag |= ECHO;
	tcsetattr(0, TCSANOW, &info);

	int passwd_fd = creat("passwd", FILE_PERMISSION);
	write(passwd_fd, passwd, strlen(passwd));

	puts("");
	puts("");
	printf("당신의 이름을 입력하세요: ");
	scanf("%s", name);
	getchar();
	int name_fd = creat("name", FILE_PERMISSION);
	write(name_fd, name, strlen(name));

	puts("");
	printf("당신의 출신 초등학교를 입력하세요(비밀번호 찾기 용도): ");
	scanf("%s", school);
	getchar();
	int school_fd = creat("school", FILE_PERMISSION);
	write(school_fd, school, strlen(school));
	
	puts("");
	printf("회원가입이 완료되었습니다. 3초 후 메뉴로 돌아갑니다.\n");
	chdir(home_path);
	sleep(3);

	close(passwd_fd);
	close(name_fd);
	close(school_fd);

	return -1;
}

int find_passwd(){

	;
}

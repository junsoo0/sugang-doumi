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
#include "sugang_doumi.h" // FOLDER_PERMISSION, FILE_PERMMISION, home_path 포함

char home_path[INPUT_SIZE];
char user_path[INPUT_SIZE];
char schedule_path[INPUT_SIZE];
char credit_path[INPUT_SIZE];
char id[INPUT_SIZE];

int login(){
	char buf[INPUT_SIZE];
	char passwd[INPUT_SIZE];
	char login_id[INPUT_SIZE];
	char ans;
	struct termios info;
	tcgetattr(0, &info);

	/* UI 변경 중 -> sugang_doumi.h에 함수로 따로 분리 
	int pid = fork();
	if(pid == 0){
		execlp("clear", "clear", NULL);
	}
	wait(NULL);
	*/

	printf("[로그인]\n");
	printf("ID를 입력하세요(학번): ");
	scanf("%s", login_id);
	getchar();

	if(chdir(login_id) == -1){
		printf("존재하지 않는 ID(학번)입니다.\n");
	        printf("메뉴로 돌아갑니다.\n");
       		sleep(1);
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
			sleep(1);
			// 무한루프 깨고 로그인 성공 후 화면으로 이동

			strcpy(id, login_id);
			// 각 폴더에 접근하기 위한 경로 설정
			strcpy(user_path, home_path);
			strcat(user_path, "/");
			strcat(user_path, login_id);
			strcpy(schedule_path, user_path);
			strcat(schedule_path, "/schedule");
			strcpy(credit_path, user_path);
			strcat(credit_path, "/credit");
			mkdir(credit_path, FOLDER_PERMISSION);
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
	char passwd1[INPUT_SIZE];
	char passwd2[INPUT_SIZE];
	char name[INPUT_SIZE];
	char school[INPUT_SIZE];
	struct termios info;
	tcgetattr(0, &info);

	/* UI 변경 중 -> sugang_doumi.h에 함수로 따로 분리
	int pid = fork();
	if(pid == 0){
		execlp("clear", "clear", NULL);
	}
	wait(NULL);
	*/

	printf("[회원가입]\n");
	printf("회원가입 하실 ID를 입력하세요(학번): ");
	scanf("%s", sign_up_id);
	getchar();
	if(chdir(sign_up_id) != -1){
		printf("이미 존재하는 ID입니다.\n");
		printf("메뉴로 돌아갑니다.\n");
		sleep(1);
		chdir(home_path);
		return -1;
	}
	else
		mkdir(sign_up_id, FOLDER_PERMISSION);

	chdir(sign_up_id);
	info.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &info);
	int passwd_check = 1;
	int passwd_fd;
	while(passwd_check){
		puts("");
		printf("비밀번호를 설정하세요: ");
		scanf("%s", passwd1);
		getchar();
	
		puts("");
		printf("비밀번호를 다시 입력하세요: ");
		scanf("%s", passwd2);
		getchar();
	
		if(strcmp(passwd1, passwd2) == 0){
			puts("");
			printf("비밀번호가 정상적으로 설정되었습니다.\n");
		
			passwd_fd = creat("passwd", FILE_PERMISSION);
			write(passwd_fd, passwd1, strlen(passwd1));

			passwd_check = 0;
		}
		else{
			puts("");
			printf("두 비밀번호가 일치하지 않습니다. \n");
		}
	}

	info.c_lflag |= ECHO;
	tcsetattr(0, TCSANOW, &info);


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
	printf("회원가입이 완료되었습니다.\n");
	chdir(home_path);
	sleep(1);

	close(passwd_fd);
	close(name_fd);
	close(school_fd);

	return -1;
}

int find_passwd(){
	char find_id[INPUT_SIZE];
	char school_name[INPUT_SIZE];
	char school_buf[INPUT_SIZE];
	char passwd_buf[INPUT_SIZE];
	/* UI 변경 중 -> sugang_doumi.h에 함수로 따로 분리
	int pid = fork();
	if(pid == 0){
		execlp("clear", "clear", NULL);
	}
	wait(NULL);
	*/

	printf("[비밀번호 찾기]\n");
	printf("ID를 입력하세요(학번): ");
       	scanf("%s", find_id);
       	getchar();
	
	if(chdir(find_id) == -1){
		printf("존재하지 않는 ID입니다.\n");
		printf("메뉴로 돌아갑니다.\n");
		sleep(1);
		chdir(home_path);
		return -1;
	}

	printf("당신의 출신 초등학교를 입력하세요: ");
	scanf("%s", school_name);
	getchar();

	
	for(int i = 0; i < INPUT_SIZE; i++) school_buf[i] = '\0';
	int school_fd = open("school", O_RDONLY);
	read(school_fd, school_buf, INPUT_SIZE);
	close(school_fd);

	if(strcmp(school_name, school_buf) == 0){
		for(int j = 0; j < INPUT_SIZE; j++) passwd_buf[j] = '\0';
		int passwd_fd = open("passwd", O_RDONLY);
		read(passwd_fd, passwd_buf, INPUT_SIZE);
		close(passwd_fd);
		printf("비밀번호는 %s 입니다.\n", passwd_buf);
		printf("메뉴로 돌아갑니다.\n");
		sleep(2);
		chdir(home_path);
		return -1;
	}
	else{
		printf("입력하신 정보가 회원정보와 일치하지 않습니다.\n");
		printf("메뉴로 돌아갑니다.\n");
		sleep(1);
		chdir(home_path);
		return -1;

	}

}




int change_passwd(){
	char buf[INPUT_SIZE];
	char old_passwd[INPUT_SIZE];
	char new_passwd1[INPUT_SIZE];
	char new_passwd2[INPUT_SIZE];
	struct termios info;
	tcgetattr(0, &info);
	
	for(int i = 0; i < INPUT_SIZE; i++) buf[i] = '\0';
	int passwd_fd = open("passwd", O_RDONLY);
	read(passwd_fd, buf, INPUT_SIZE);
	close(passwd_fd);
	
	puts("");
	printf("현재 비밀번호를 입력하세요: ");
	info.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &info);
	scanf("%s", old_passwd);
	getchar();

	if(strcmp(old_passwd, buf) == 0){
		puts("");
		printf("변경하실 비밀번호를 입력하세요: ");
		scanf("%s", new_passwd1);
		getchar();
		
		puts("");
		printf("변경하실 비밀번호를 다시 입력하세요: ");
		scanf("%s", new_passwd2);
		getchar();
		
		info.c_lflag |= ECHO;
		tcsetattr(0, TCSANOW, &info);
		
		if(strcmp(new_passwd1, new_passwd2) == 0){
			int new_passwd_fd = open("passwd", O_WRONLY | O_TRUNC);
			write(new_passwd_fd, new_passwd1, strlen(new_passwd1));
			close(new_passwd_fd);
			puts("");
			printf("비밀번호가 변경되었습니다.\n");
			sleep(2);
			chdir(home_path);
			return 1;
		}
		else{
			puts("");
			printf("두 비밀번호가 일치하지 않습니다.\n");
			sleep(2);
			chdir(home_path);
			return 0;
		}

	}
	else{
		puts("");
		printf("비밀번호가 일치하지 않습니다.\n");
		info.c_lflag |= ECHO;
		tcsetattr(0, TCSANOW, &info);
		sleep(2);
		return 0;
	}

	
}

int logout(){
	char ans;

	while(1){
	puts("");
 	printf("로그아웃 하시겠습니까? (Y/N): ");	
	ans = getchar();
	getchar();

	switch(ans){
		case 'Y' :
		case 'y' :
			chdir(home_path);
			for(int i = 0; i < INPUT_SIZE; i++)
				id[i] = '\0';
			return 1;
		case 'N' :
		case 'n' :	return 0;
		default : break;

	}

	}

}

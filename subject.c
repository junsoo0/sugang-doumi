#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "sugang_doumi.h"

char sub_name[INPUT_SIZE];
char prof_name[INPUT_SIZE];
char filename[INPUT_SIZE];
const char* folder_path = "../subject_lst";

int write_op();
int show_op();
int write_now(char* filename);

int evaluate_lecture() {
	int option;
	
	while (1) {
		clear_terminal();
		printf("\x1b[32m");
		printf("[강의 평가]\n");
		printf("\x1b[0m");
		printf("========================================\n");
		printf("[1] 강의평 보기\n");
		printf("[2] 강의평 입력\n");
		printf("[3] 돌아가기\n");
		printf("----------------------------------------\n\n");
		
		printf("선택: [1 - 3] ");
		scanf("%d", &option);
		getchar();
	
		if (option == 1 || option == 2) {
			if (option == 1) {
				clear_terminal();
				show_op();
				clear_terminal();
				continue;
			}
			
			if (option == 2) {
				clear_terminal();
				write_op();
				clear_terminal();
				continue;
			}
		}
		
		else if (option == 3) {
			clear_terminal();
			return 0;
		}
	
		
		else {
			printf("잘못된 입력입니다. 1, 2, 3 중 하나를 입력하세요.\n");
		}
	}
	
}		


int show_op()
{
	char new_text[INPUT_SIZE];
	struct stat st;    // 폴더 존재 확인, 없으면 생성 후 메뉴화면으로 복귀
	if (stat(folder_path, &st) != 0 || !S_ISDIR(st.st_mode)) {
		printf("조회할 강의평이 없습니다.\n");
		if (mkdir(folder_path, 0700) != 0) {
			perror("make folder fail");
			return 1;
		}
		return 1;
	}    
	
view:
	printf("<강의평 조회할 과목 입력>\n\n");
	printf("과목명 (띄어쓰기 없이 소문자로 입력) : ");
	fgets(sub_name, INPUT_SIZE, stdin);
	sub_name[strlen(sub_name) - 1] = '\0';    // 개행 제거
	
	printf("교수명 : ");
	fgets(prof_name, INPUT_SIZE, stdin);
	prof_name[strlen(prof_name) - 1] = '\0';    // 개행 제거
	
	printf("\n----------------------------------------\n");
	
	strcpy(filename, folder_path);
	strcat(filename, "/");
	strcat(filename, sub_name);
	strcat(filename, "_");
	strcat(filename, prof_name);
	strcat(filename, ".txt");
	
	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		printf("존재하지 않는 과목명 혹은 교수명입니다. 확인 후 다시 입력해주세요.\n\n");
		goto view;
	}
	
	ssize_t n;
	while ((n = read(fd, new_text, INPUT_SIZE)) > 0) {
		write(STDOUT_FILENO, new_text, n);
	}
	
	close(fd);
	
	int retry;
	printf("========================================\n\n");
	printf("[1] 현재 과목에 강의평 남기기\n");
	printf("[2] 다른 과목의 강의평 조회\n");
	printf("[3] 이전 화면으로 돌아가기\n");
	printf("----------------------------------------\n\n");
	printf("선택: [1 - 3] ");
	
	scanf("%d", &retry);
	getchar();
	
	if (retry == 1 || retry == 2 || retry == 3) {
		if (retry == 3) return 0;
		
		if (retry == 2) {
			clear_terminal();
			goto view;
		}
		
		if (retry == 1) {
			clear_terminal();
			write_now(filename);
			return 0;
		}
	}
	return 0;
}


int write_now(char* filename)
{
	// 강의평 작성할 파일은 당연히 존재하는 상태, 파일 열고 바로 작성만 하면 됨
	char new_text[INPUT_SIZE];
	FILE *fp;
	fp = fopen(filename, "a");  
	
	printf("내용 작성 (종료는 q): \n");
	while (1) {
		fgets(new_text, INPUT_SIZE, stdin);
		if (!strcmp(new_text, "q\n")) break;
		
		size_t len = strlen(new_text);    // 입력 끝의 개행 제거
        	if (len > 0 && new_text[len - 1] == '\n')
            	new_text[len - 1] = '\0';
            	
		fprintf(fp, "%s\n", new_text);
	}
	
	time_t t = time(NULL);    // 작성 시간 날짜까지만 출력
	struct tm* tm_info = localtime(&t);
	char time_str[20];
	
	strftime(time_str, sizeof(time_str), "%Y-%m-%d", tm_info);
	fprintf(fp, "%s\n\n", time_str);
	
	fclose(fp);
	
	return 0;
}


int write_op()
{
	char new_text[INPUT_SIZE];
	
	struct stat st;    // 폴더 존재 확인, 없으면 생성
	if (stat(folder_path, &st) != 0 || !S_ISDIR(st.st_mode)) {
		if (mkdir(folder_path, 0700) != 0) {
			perror("make folder fail");
			return 1;
		}
	}    
	
	printf("과목명 (띄어쓰기 없이 소문자로 입력) : ");
	fgets(sub_name, INPUT_SIZE, stdin);
	sub_name[strlen(sub_name) - 1] = '\0';    // 개행 제거
	
	printf("교수명 : ");
	fgets(prof_name, INPUT_SIZE, stdin);
	prof_name[strlen(prof_name) - 1] = '\0';    // 개행 제거
	
	// 파일 생성 or 오픈
	strcpy(filename, folder_path);
	strcat(filename, "/");
	strcat(filename, sub_name);
	strcat(filename, "_");
	strcat(filename, prof_name);
	strcat(filename, ".txt");
	
	// fd = open(filename, O_CREAT | O_WRONLY, 0666);
	FILE *fp;
	fp = fopen(filename, "a");  
	if (fp < 0) {
		perror("file open fail");
		exit(1);
	}
	
	// 강의평 입력
	printf("내용 작성 (종료는 q): \n");
	while (1) {
		fgets(new_text, INPUT_SIZE, stdin);
		if (!strcmp(new_text, "q\n")) break;
		
		size_t len = strlen(new_text);    // 입력 끝의 개행 제거
        	if (len > 0 && new_text[len - 1] == '\n')
            	new_text[len - 1] = '\0';
            	
		fprintf(fp, "%s\n", new_text);
		// write(fd, new_text, INPUT_SIZE);
	}
	
	/*
	time_t t = time(NULL); 
	fprintf(fp, "%s\n\n", ctime(&t));
	*/
	time_t t = time(NULL);    // 작성 시간 날짜까지만 출력
	struct tm* tm_info = localtime(&t);
	char time_str[20];
	
	strftime(time_str, sizeof(time_str), "%Y-%m-%d", tm_info);
	fprintf(fp, "%s\n\n", time_str);
	
	fclose(fp);
	
	return 0;
}

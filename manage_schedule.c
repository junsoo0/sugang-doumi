#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "sugang_doumi.h"

enum schedule_choice {
	ENTER_SCHEDULE = 1, CHANGE_SCHEDULE, SCHEDULE_BACK, SCHEDULE_EXIT
};

void show_schedule();
int enter_schedule();

void wrong_date();
char schedule_path[210];

int manage_schedule() {
	char input[INPUT_SIZE];

	strcpy(schedule_path, user_path);
	strcat(schedule_path, "/schedule");
	if (chdir(schedule_path) == -1) {
		mkdir(schedule_path, FOLDER_PERMISSION);
		chdir(schedule_path);
	}

	while (1) {
		show_schedule();
		printf("----------------------------------------\n");
		printf("[1] 일정 등록\n");
		printf("[2] 일정 수정\n");
		printf("[3] 뒤로가기\n");
		printf("[4] 종료\n");
		printf("----------------------------------------\n");
		printf("선택: [1 - 4] ");

		fgets(input, INPUT_SIZE, stdin);
		input[strlen(input) - 1] = '\0';
		if (check_valid_input(input, 5) == FALSE)
			continue;
		puts("");

		switch (input[0] - '0') {
			case ENTER_SCHEDULE:
				enter_schedule();
				break;
			case CHANGE_SCHEDULE:
				break;
			case SCHEDULE_BACK:
				break;
			case SCHEDULE_EXIT:
				break;
		}
	}
}

void show_schedule() {
	int i = 0;
	char str[INPUT_SIZE];
	FILE  *fp;
	DIR *dp;
	struct dirent *direntp;

	dp = opendir(".");
	while ((direntp = readdir(dp)) != NULL) {
		if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
			continue;

		printf("----------------------------------------\n");
		fp = fopen(direntp->d_name, "r");
		fgets(str, INPUT_SIZE, fp);
		printf("(%d) %s", ++i, str);
		fgets(str, INPUT_SIZE, fp);
		printf("날짜: %s", str);
		fgets(str, INPUT_SIZE, fp);
		printf("내용: %s", str);
		fclose(fp);
	}
	closedir(dp);

	if (i == 0) {
		printf("----------------------------------------\n");
		printf("등록된 일정이 없습니다.\n");
	}
}

int enter_schedule() {
	int year, month, day;
	char i = '1';
	char fname[INPUT_SIZE], title[INPUT_SIZE], due_date[INPUT_SIZE], parse[INPUT_SIZE], contents[INPUT_SIZE];
	FILE *fp, *tfp;
	DIR *dp;
	struct dirent *direntp;


	printf("일정 이름: ");
	fgets(title, INPUT_SIZE, stdin);

	while (1) {
		printf("마감 날짜: [yy-mm-dd] 8자리 ex) 23-06-01 ");
		fgets(due_date, INPUT_SIZE, stdin);
		strcpy(fname, due_date);
		fname[strlen(fname) - 1] = '\0';
		strcat(fname, "_1");
		strcpy(parse, due_date);
		if (parse[2] != '-' || parse[5] != '-') {
			wrong_date();
			continue;
		}

		parse[2] = '\0';
		parse[5] = '\0';
		year = atoi(&parse[0]);
		month = atoi(&parse[3]);
		day = atoi(&parse[6]);

		if (year <= 0 || year >= 100 || month <= 0 || month >= 13 || day <= 0 || day >= 32) {
			wrong_date();
			continue;
		}
		else if ((month == 4 || month == 6 || month == 9 || month == 11) && day >= 31) {
			wrong_date();
			continue;
		}
		else if (month == 2 && day >= 30) {
			wrong_date();
			continue;
		}

		break;
	}

	printf("내용: \n");
	fgets(contents, INPUT_SIZE, stdin);

	dp = opendir(".");
	while ((direntp = readdir(dp)) != NULL) {
		if (strcmp(direntp->d_name, fname) == 0) {
			fname[strlen(fname) - 1] = ++i;
		}
	}
	closedir(dp);

	fp = fopen(fname, "w");
	fwrite(title, strlen(title), sizeof(char), fp);
	fwrite(due_date, strlen(due_date), sizeof(char), fp);
	fwrite(contents, strlen(contents), sizeof(char), fp);
	fclose(fp);

	puts("");
	printf("일정 등록 성공\n");
	puts("");
	return TRUE;

	/*
	puts("");
	printf("일정 등록 실패\n");
	puts("");
	return FALSE;
	*/
}

int print_schedule() {
	
}

void wrong_date() {
	puts("");
	printf("잘못된 날짜형식입니다.\n");
	printf("다시 입력해주세요.\n");
	puts("");
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "sugang_doumi.h"

void wrong_date();
char schedule_path[210];

int manage_schedule() {
	int year, month, day;
	char i = '1';
	char fname[INPUT_SIZE], title[INPUT_SIZE], due_date[INPUT_SIZE], parse[INPUT_SIZE], contents[INPUT_SIZE];
	FILE *fp, *tfp;
	DIR *dp;
	struct dirent *direntp;
	struct stat info;

	sprintf(schedule_path, "%s/schedule", user_path);
	if (chdir(schedule_path) == -1) {
		mkdir(schedule_path, FOLDER_PERMISSION);
		chdir(schedule_path);
	}

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

#include <stdio.h>
#include <string.h>
#include "sugang_doumi.h"

char *lb = "\n";

int manage_schedule() {
	char title[INPUT_SIZE], due_date[INPUT_SIZE], contents[INPUT_SIZE];
	FILE *fp;

	// 일정 보기 등 구현 필요

	// 일정 등록
	printf("일정 이름: ");
	fgets(title, INPUT_SIZE, stdin);

	printf("마감 날짜: (yy-mm-dd)");
	fgets(due_date, INPUT_SIZE, stdin);

	printf("내용: \n");
	fgets(contents, INPUT_SIZE, stdin);

	fp = fopen("schedule.txt", "a");
	fwrite(title, strlen(title), sizeof(char), fp);
	fwrite(due_date, strlen(due_date), sizeof(char), fp);
	fwrite(contents, strlen(contents), sizeof(char), fp);
	fwrite(lb, strlen(lb), sizeof(char), fp);
	fclose(fp);

	puts("");
	printf("일정 등록 성공\n");
	puts("");
	return TRUE;

	puts("");
	printf("일정 등록 실패\n");
	puts("");
	return FALSE;
}

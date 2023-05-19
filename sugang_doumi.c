#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "sugang_doumi.h"

enum initial_choice {
	LOGIN = 1, FIND_PASSWD, SIGN_UP, INITIAL_EXIT
};

enum main_choice {
	CALCULATE_GPA = 1, EVALUATE_LECTURE, MANAGE_SCHEDULE, CHANGE_PASSWD, LOGOUT, MAIN_EXIT
};

int initial_UI();
int main_UI();
int check_valid_input(char* input, int total_num);
int login();
int sign_up();
int find_passwd();
int manage_schedule();

int main() {
	int choice;
	int check = -1;

	getcwd(home_path, INPUT_SIZE);

	while (check == -1) {
		choice = initial_UI();
		switch (choice) {
			case LOGIN:
				check = login();
				break;
			case FIND_PASSWD:
				find_passwd();
				break;
			case SIGN_UP:
				sign_up();
				break;
			case INITIAL_EXIT:
				break;
		}
	}
	
	while (1) {
		choice = main_UI();
		switch (choice) {
			case CALCULATE_GPA:
				break;
			case EVALUATE_LECTURE:
				break;
			case MANAGE_SCHEDULE:
				manage_schedule();
				break;
			case CHANGE_PASSWD:
				break;
			case LOGOUT:
				break;
			case MAIN_EXIT:
				break;
		}
	}

	return 0;
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

int main_UI() {
	char input[INPUT_SIZE];

	while (1) {
		show_schedule(3);
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

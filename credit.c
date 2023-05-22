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
#include <dirent.h>
#include "sugang_doumi.h" // FOLDER_PERMISSION, FILE_PERMMISION, home_path 포함

struct credit {
	char lecture[INPUT_SIZE];
	char lecture_type[INPUT_SIZE];
	int credit;
	char grade[INPUT_SIZE];

};

int valid_grade(char*);
void print_grade();
void print_credit_and_grade();
float grade_to_float(char*);

int credit_add(){
	// clear_terminal();

	int fd;
	FILE* fp;
	struct credit temp;
	int type_check = 1;
	int credit_check = 1;
	int grade_check = 1;
	char temp_type[INPUT_SIZE];
	int temp_credit;
	char temp_grade[INPUT_SIZE];
	
	chdir(credit_path);
	
	// 과목명 입력받기
	printf("과목명을 입력하세요: ");
	scanf("%s", temp.lecture);
	getchar();
	fd = creat(temp.lecture, FILE_PERMISSION);
	fp = fdopen(fd, "w");
	
	
	// 전공or교양 입력받기
	while(type_check){
		puts("");
		printf("과목구분을 입력하세요 (전공/교양): ");
		scanf("%s", temp_type);
		getchar();
	
		if(strcmp(temp_type, "전공") == 0 || strcmp(temp_type, "교양") == 0 ){
			strcpy(temp.lecture_type, temp_type);
			type_check = 0;
			break; 
		}	
		else{
			printf("잘못된 과목구분입니다.\n");
		}
	}
	
	
	// 이수학점 입력받기
	while(credit_check){
		puts("");
		printf("이수학점을 입력하세요 (1~9) : ");
		scanf("%d", &temp_credit);
		getchar();
	
		if(temp_credit < 1 || temp_credit > 9 ){
			printf("잘못된 입력입니다.\n");
		}
		else{
			temp.credit = temp_credit;
			credit_check = 0;
			break;
		}
	}
	
	// 성적 입력받기
	while(grade_check){
		puts("");
		printf("성적을 입력하세요 (F ~ A+) : ");
		scanf("%s", temp_grade);
		getchar();
		
		if(valid_grade(temp_grade)){
			strcpy(temp.grade, temp_grade);
			grade_check = 0;
			break;
		}
		else{
			printf("잘못된 입력입니다.\n");
		}
	
	}
	
	// 구조체에 담긴 것을 파일포인터로 출력
	fprintf(fp, "%s %s %d %s", temp.lecture, temp.lecture_type,
			                          temp.credit, temp.grade);

	printf("학점이 등록되었습니다.\n");
	sleep(1);
	
	chdir(home_path);
	fclose(fp);
	return 0;
}

int credit_remove(){
	//clear_terminal();
	
	DIR *dir_ptr;
	struct dirent *direntp;
	char remove_lecture[INPUT_SIZE];
	
	printf("삭제할 과목명을 입력하세요: ");
	scanf("%s", remove_lecture);
	getchar();
	
	chdir(credit_path);
	dir_ptr = opendir(credit_path);
	while((direntp = readdir(dir_ptr)) != NULL){
		if( strcmp(direntp->d_name, remove_lecture) == 0){
			unlink(remove_lecture);
			printf("과목 삭제가 완료되었습니다.\n");
			chdir(home_path);
			sleep(2);
			return 1;
		} 
	}
	
	printf("해당 과목이 존재하지 않습니다.\n");
	sleep(2);
	chdir(home_path);
	return 0;
}

int credit_list(){
	clear_terminal();
	
	DIR *dir_ptr;
	struct dirent *direntp;
	int list_num = 0;
	int fd;
	FILE* fp;
	struct credit temp;
	
	printf("==============================================\n");
	chdir(credit_path);
	dir_ptr = opendir(credit_path);
	while((direntp = readdir(dir_ptr)) != NULL){
		if( strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0 )
			continue;
		else{
			list_num++;
			fd = open(direntp->d_name, O_RDONLY);
			fp = fdopen(fd, "r");
			fscanf(fp, "%s %s %d %s", temp.lecture, temp.lecture_type,
			                          &temp.credit, temp.grade);
			printf(" %s / %s / %d학점 / %s\n", temp.lecture, temp.lecture_type,
			                          temp.credit, temp.grade);
			fclose(fp);
		
		
		
		} 
	}
	
	
	
	
	if(list_num == 0){
		printf("등록된 학점 목록이 없습니다.\n");
		sleep(2);
		chdir(home_path);
		return 0;
	}
		
		
	// 총 학점, 교양학점, 전공학점, 성적 출력
	print_credit_and_grade();
	
	puts("");	
	puts("");
	printf("메뉴로 돌아가려면 엔터를 두번 누르세요\n");
	getchar();
	getchar();
	chdir(home_path);
	return 0;
}



int valid_grade(char* input){
	if(strcmp(input, "A+") == 0 || strcmp(input, "A0") == 0 || strcmp(input, "A-") == 0 || 
	   strcmp(input, "B+") == 0 || strcmp(input, "B0") == 0 || strcmp(input, "B-") == 0 ||
	   strcmp(input, "C+") == 0 || strcmp(input, "C0") == 0 || strcmp(input, "C-") == 0 ||  
	   strcmp(input, "D+") == 0 || strcmp(input, "D0") == 0 || strcmp(input, "D-") == 0 || 
	   strcmp(input, "F") == 0) return 1; 

	return 0;
}


void print_credit_and_grade(){
	struct credit temp[INPUT_SIZE];
	DIR *dir_ptr;
	struct dirent *direntp;
	int fd;
	FILE* fp;
	int i = 0;
	int total_credit = 0;
	int major = 0;
	int culture = 0;
	float total_grade = 0;
	float avg_grade;

	
	chdir(credit_path);
	dir_ptr = opendir(credit_path);
	while((direntp = readdir(dir_ptr)) != NULL){
		if( strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0 )
			continue;
		else{
		
			fd = open(direntp->d_name, O_RDONLY);
			fp = fdopen(fd, "r");
			fscanf(fp, "%s %s %d %s", temp[i].lecture, temp[i].lecture_type,
			                          &temp[i].credit, temp[i].grade);
			i++;
			fclose(fp);
		
		} 
	}
	
	
	for(int j = 0; j < i; j++){
		total_credit += temp[j].credit;
		if(strcmp(temp[j].lecture_type, "전공") == 0){
			major += temp[j].credit;
		}
		total_grade += grade_to_float(temp[j].grade) * temp[j].credit;
	}
	
	
	culture = total_credit - major;
	avg_grade = total_grade / total_credit;
	
	
	printf("==============================================\n");
	printf("총 이수학점: %d학점 / 전공: %d학점 / 교양: %d학점\n", total_credit, major, culture);
	printf("평균평점: %.2f\n", avg_grade);
	printf("==============================================\n");
	
}




float grade_to_float(char* input){
	if(strcmp(input, "A+") == 0) return 4.3;
	else if(strcmp(input, "A0") == 0) return 4.0;
	else if(strcmp(input, "A-") == 0) return 3.7;
	else if(strcmp(input, "B+") == 0) return 3.3;
	else if(strcmp(input, "B0") == 0) return 3.0;
	else if(strcmp(input, "B-") == 0) return 2.7;
	else if(strcmp(input, "C+") == 0) return 2.3;
	else if(strcmp(input, "C0") == 0) return 2.0;
	else if(strcmp(input, "C-") == 0) return 1.7;
	else if(strcmp(input, "D+") == 0) return 1.3;
	else if(strcmp(input, "D0") == 0) return 1.0;
	else if(strcmp(input, "D-") == 0) return 0.7;
	else if(strcmp(input, "F") == 0) return 0.0;
	else return -1.0;
}


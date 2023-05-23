#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

//
const char* folderPath = "post_lst";
char filePath[100];
char line[100];
char quit;

// char userPath[100] = "./2021112563";

int my_post() {
	strcat(userPath, "\n");
	printf("나의 작성 목록 : \n\n");
	
	DIR* dir = opendir(folderPath);
	if (dir == NULL) {
		perror("DIR open fail");
		return 0;
	}
	
	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type == DT_REG) {
			sprintf(filePath, "%s/%s", folderPath, entry->d_name);
			
			FILE *fp = fopen(filePath, "r");
			if (fp != NULL) {
				fgets(line, sizeof(line), fp);
				fgets(line, sizeof(line), fp);    // 작성자 정보가 2번째줄에 있으므로 2번째 줄 가져오기
				if (!strcmp(line, userPath)) {    // 작성자 일치
					fseek(fp, 0, SEEK_SET);    // 파일포인터 처음으로 이동
					printf("Title: ");
					fgets(line, sizeof(line), fp);
					printf("%s", line);
					fgets(line, sizeof(line), fp);    // 작성자 정보 스킵
					
					// 내용 출력
					while (fgets(line, sizeof(line), fp) != NULL) 
						printf("%s", line);
						
					printf("\n-----------------------------------------------------\n\n");
					
				}
			}
			fclose(fp);
		}
	}
	
	closedir(dir);
	
do_quit:
	printf("q 입력 시 메뉴로 돌아갑니다 : ");
	scanf("%c", &quit);
	getchar();
	
	if (quit == 'q') return 0;
	else goto do_quit;
	
}

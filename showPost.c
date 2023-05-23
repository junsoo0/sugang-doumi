#include <stdio.h>
#include <dirent.h>

int show_post() {
	int input_num;
	const char* folderPath = "post_lst";
	char filePath[100];
	char line[100];
	DIR* dir = opendir(folderPath);
	if (dir == NULL) {
		perror("folder open fail");
		return 0;
	}
	
	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) {
	
		if (entry->d_type == DT_REG) {
			// char filePath[256];
			sprintf(filePath, "%s/%s", folderPath, entry->d_name);
			// snprintf(filePath, sizeof(filePath), "%s/%s", folderPath, entry->d_name);
			
			FILE* fp = fopen(filePath, "r");
			if (fp != NULL) {
				printf("%.*s", (int)(strlen(entry->d_name) - 4), entry->d_name);    // 맨 끝의 .txt 빼고 출력 (글 번호만 출력)
				
				// char line[50];
				if (fgets(line, sizeof(line), fp) != NULL) {
					printf(". %s\n", line);   // 제목 출력
				}
				
				fclose(fp);
			}
		
		}
		
	}
	
	closedir(dir);


	printf("==============================================\n");
	
print_post:
	printf("읽을 포스트의 번호 입력(이전으로 돌아가기는 0번) : ");
	if (scanf("%d", &input_num) != 1) {
		getchar();
		printf("잘못된 입력입니다. 숫자를 입력해주세요.\n\n");
		goto print_post;
	}
	
	getchar();
	if (input_num == 0) return 0;
	
	snprintf(filePath, sizeof(filePath), "%s/%d.txt", folderPath, input_num);
	
	FILE* fp = fopen(filePath, "r");
	if (fp == NULL) {
		printf("존재하지 않는 글입니다.\n\n");
		goto print_post;
	}
	else {
		printf("Title : ");
		fgets(line, sizeof(line), fp);
		printf("%s", line);
		fgets(line, sizeof(line), fp);    // 학번 줄 미출력
		
		while (fgets(line, sizeof(line), fp) != NULL) printf("%s", line);
		
		fclose(fp);
		printf("----------------------------------------------\n\n");
		
		goto print_post;
	}
	
	return 0;
}


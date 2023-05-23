// #define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "sugang_doumi.h"

typedef struct {
	int num;
	char title[50];
	char writer[30];
	char mtime[40];
} Post;

//
//char user_path[100] = "./2021112563";

int write_post()
{
	// 사용자가 작성을 선택
	char new_title[50];
	FILE *fp;
	char new_text[150];
	const char* folderPath = "post_lst";
	
	
	// post_lst 폴더에 총 몇개 post가 있는지 counting
	struct stat st;
	if (stat(folderPath, &st) != 0 || !S_ISDIR(st.st_mode)) {
		if (mkdir(folderPath, 0700) != 0) {
			perror("make folder fail");
			return 1;
		}
	}
	
	DIR* dir = opendir(folderPath);
	if (dir == NULL) {
		perror("open folder fail");
		return 1;
	}
	
	int count = 0;
	
	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type == DT_REG) count++;
	}
	
	closedir(dir);
	
	
	// 작성할 post 제목과 내용 입력받음
	printf("title : ");
	fgets(new_title, sizeof(new_title), stdin);
	
	
	char filename[20];    // file open
	sprintf(filename, "%s/%d.txt", folderPath, ++count);
	
	fp = fopen(filename, "w");    
	// fp = fopen(filename, O_CREAT | O_WRONLY);
	if (fp < 0) {
		perror("File open fail");
		exit(1);
	}

	fprintf(fp, "%s", new_title);
	fprintf(fp, "%s\n\n", user_path);
	printf("text (종료는 q): \n");	// file writing
	while (1) {
		fgets(new_text, sizeof(new_text), stdin);
		if (!strcmp(new_text, "q\n")) break;
		
		size_t len = strlen(new_text);    // 입력 끝의 개행 제거
        	if (len > 0 && new_text[len - 1] == '\n')
            	new_text[len - 1] = '\0';
            	
		fprintf(fp, "%s\n", new_text);
		// write(fp, new_text, sizeof(new_text));
	}
	fclose(fp);
	
	time_t t = time(NULL);    // make new_post
	Post new_post;
	new_post.num = count;
	strcpy(new_post.title, new_title);
	// strcpy(new_post.writer, "writer(student id)");
	strcpy(new_post.writer, user_path);
	strcpy(new_post.mtime, ctime(&t));
	
	// realloc(posts, (count+1) * sizeof(Post));
	// posts[count] = new_post;
	
	printf("\n작성이 완료되었습니다.\n");
	
		
	return 0;
}

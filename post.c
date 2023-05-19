// #define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
	int num;
	char title[50];
	char writer[30];
	char mtime[40];
} Post;

// int num = 1;

int write_post(Post* posts,int* num)
// int main() 
{
	// 사용자가 작성을 선택
	char new_title[50];
	FILE *fp;
	char new_text[150];
	/*
	Post* posts;
	posts = (Post*)malloc(0);
	*/
	
	printf("title : ");
	fgets(new_title, sizeof(new_title), stdin);
	
	
	char filename[10];    // file open
	sprintf(filename, "%d.txt", *num);
	
	fp = fopen(filename, "w");    
	if (fp == NULL) {
		perror("File open fail");
		exit(0);
	}

	printf("text (종료는 q): \n");	// file writing
	while (1) {
		fgets(new_text, sizeof(new_text), stdin);
		if (!strcmp(new_text, "q\n")) break;
		
		size_t len = strlen(new_text);    // 입력 끝의 개행 제거
        	if (len > 0 && new_text[len - 1] == '\n')
            	new_text[len - 1] = '\0';
            	
		fprintf(fp, "%s\n", new_text);
	}
	fclose(fp);
	
	time_t t = time(NULL);    // make new_post
	Post new_post;
	new_post.num = *num;
	strcpy(new_post.title, new_title);
	strcpy(new_post.writer, "writer(student id)");
	strcpy(new_post.mtime, ctime(&t));
	
	realloc(posts, ((*num)+1) * sizeof(Post));
	posts[*num++] = new_post;
	
	printf("\n작성이 완료되었습니다.\n");
	
		
	return 0;
}

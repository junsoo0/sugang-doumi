#define INPUT_SIZE 100

#define FOLDER_PERMISSION 0775
#define FILE_PERMISSION 0664

enum boolean {
	FALSE, TRUE
};

extern char home_path[INPUT_SIZE];
extern char user_path[INPUT_SIZE];
extern char schedule_path[INPUT_SIZE];

int check_valid_input(char* input, int total_num);
void show_schedule(int opt);

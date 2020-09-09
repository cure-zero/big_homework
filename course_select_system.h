#ifndef STUDENT_SELECT_CLASS_SYSTEM_COURSE_SELECT_SYSTEM_H
#define STUDENT_SELECT_CLASS_SYSTEM_COURSE_SELECT_SYSTEM_H

typedef struct {
    char name[100];
    int student_count;
    int limit;
}Course;

typedef struct {
    int identity;
    char name[100];
}course_select_system;

void menu();
int login(char *password, char *num);
int student_select_course();
int search_course();
void query_result(char *name);
int delete_course_student();
int manage_info();
void get_course_status();
int delete_course_teacher();
void get_stat();
int add_course();
int edit_course();
void init();
void exit_system();
void deal_raw_string(char* raw,int len_raw);
int parse_time(char* time_raw);
int parse_string(char *string);
void init_struct();
void print_by_limit(char *name,char *key);
void print_by_student_count(char *name,char *key);
int cmp_student_count(const void *a, const void *b);
int cmp_limit(const void *a, const void *b);
void deparse(char *string,char *new_string);
int check_valid_input(int l, int r);
int check_valid_email(char *email, int len);
int check_valid_phone_num(char *num);
#endif //STUDENT_SELECT_CLASS_SYSTEM_COURSE_SELECT_SYSTEM_H

#ifndef STUDENT_SELECT_CLASS_SYSTEM_COURSE_SELECT_SYSTEM_H
#define STUDENT_SELECT_CLASS_SYSTEM_COURSE_SELECT_SYSTEM_H

struct Student{
    char number[100];
    char faculty[100];
    char major[100];
    char name[100];
    char gender[100];
    char phone[100];
    char password[100];
    char email[100];
};

struct Teacher{
    char number[100];
    char faculty[100];
    char name[100];
    char password[100];
    char email[100];
};

typedef struct {
    char name[100];
    int student_count;
    int limit;
}Course;

typedef struct {
    int identity;
    char name[100];

    void (*login)();
    void (*student_select_course)();
    void (*search_course)();
    void (*query_result)(char *name);
    void (*delete_course_student)();
    void (*manage_info)();
    void (*get_course_status)();
    void (*delete_course_teacher)();
    void (*get_stat)();
    void (*add_course)();
    void (*edit_course)();
    void (*exit_system)();
    void (*print_by_limit)(char *name, char *key);
    void (*print_by_student_count)(char *name, char *key);
}course_select_system;

void login();
void student_select_course();
void search_course();
void query_result(char *name);
void delete_course_student();
void manage_info();
void get_course_status();
void delete_course_teacher();
void get_stat();
void add_course();
void edit_course();
void init();
void exit_system();
void deal_raw_string(char* raw,int len_raw);
int parse_time(char* time_raw);
int parse_string(char *string);
void init_struct();
void print_by_limit(char *name,char *key);
void print_by_student_count(char *name,char *key);
int cmp();
void deparse(char *string,char *new_string);
#endif //STUDENT_SELECT_CLASS_SYSTEM_COURSE_SELECT_SYSTEM_H

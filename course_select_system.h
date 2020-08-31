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
    int identity;

    int (*login)();
    void (*student_select_course)();
    void (*search_course)();
    void (*query_result)();
    void (*delete_course_student)();
    void (*manage_info)();
    void (*get_course_status)();
    void (*delete_course_teacher)();
    void (*get_stat)();
    void (*add_course)();
    void (*edit_course)();
}course_select_system;

int login();
void student_select_course();
void search_course();
void query_result();
void delete_course_student();
void manage_info();
void get_course_status();
void delete_course_teacher();
void get_stat();
void add_course();
void edit_course();
void init();

#endif //STUDENT_SELECT_CLASS_SYSTEM_COURSE_SELECT_SYSTEM_H

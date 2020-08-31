#include "course_select_system.h"
#include<stdio.h>
#include<stdlib.h>

extern course_select_system*  Course_select_system;

void init()
{
    Course_select_system = (course_select_system*)malloc(sizeof(course_select_system));
    Course_select_system->identity = 0;
    Course_select_system->login = login;
    Course_select_system->add_course=add_course;
    Course_select_system->delete_course_student=delete_course_student;
    Course_select_system->delete_course_teacher=delete_course_teacher;
    Course_select_system->edit_course=edit_course;
    Course_select_system->get_course_status=get_course_status;
    Course_select_system->search_course=search_course;
    Course_select_system->get_stat=get_stat;
    Course_select_system->manage_info=manage_info;
    Course_select_system->query_result=query_result;
    Course_select_system->student_select_course=student_select_course;
}

int login()
{
    printf("%d\n",Course_select_system->identity);
};







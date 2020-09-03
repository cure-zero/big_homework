#include "course_select_system.h"
#include<stdio.h>
#include<stdlib.h>
#include "cJSON.h"
#include<string.h>


extern course_select_system*  Course_select_system;
FILE *fp_teacher = NULL,*fp_student = NULL,*fp_course = NULL;
char *content_teacher, *content_student, *content_course;
int len_teacher,len_student,len_course;
int courses_size;
cJSON *teacher,*student,*course;
Course *courses;

void init()
{
    Course_select_system = (course_select_system*)malloc(sizeof(course_select_system));
    Course_select_system->identity = 0;
    memset(Course_select_system->name,0,sizeof(Course_select_system->name));
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
    Course_select_system->exit_system=exit_system;
    Course_select_system->print_by_student_count = print_by_student_count;
    Course_select_system->print_by_limit = print_by_limit;

    fp_teacher = fopen("../Teacher.json","rw+");
    fseek(fp_teacher,0,SEEK_END);
    len_teacher=ftell(fp_teacher);
    fseek(fp_teacher,0,SEEK_SET);
    content_teacher=(char*)malloc(len_teacher+1);
    fread(content_teacher,1,len_teacher,fp_teacher);
    teacher = cJSON_Parse(content_teacher);
    fseek(fp_teacher,0,SEEK_SET);
    fp_student = fopen("../Student.json","rw+");
    fseek(fp_student,0,SEEK_END);
    len_student=ftell(fp_student);
    fseek(fp_student,0,SEEK_SET);
    content_student=(char*)malloc(len_student+1);
    fread(content_student,1,len_student,fp_student);
    student = cJSON_Parse(content_student);
    fseek(fp_student,0,SEEK_SET);
    fp_course = fopen("../course.json","rw+");
    fseek(fp_course,0,SEEK_END);
    len_course=ftell(fp_course);
    fseek(fp_course,0,SEEK_SET);
    content_course=(char*)malloc(len_course+1);
    fread(content_course,1,len_course,fp_course);
    course = cJSON_Parse(content_course);
    fseek(fp_course,0,SEEK_SET);
    init_struct();
}

void exit_system()
{
    free(courses);
    fwrite(cJSON_Print(course),1,strlen(cJSON_Print(course)),fp_course);
    fwrite(cJSON_Print(teacher),1,strlen(cJSON_Print(teacher)),fp_teacher);
    fwrite(cJSON_Print(student),1,strlen(cJSON_Print(student)),fp_student);
    fclose(fp_student);
    fclose(fp_teacher);
    fclose(fp_course);
}

void deal_raw_string(char* raw, int len_raw)
{
    char a[100];
    int len=strlen(a);
    memset(a, 0, len);
    a[0]=(char)34;
    strcat(a,raw);
    memset(raw,0,len_raw);
    raw[0]=(char)34;
    strcat(a,raw);
    strcpy(raw,a);
}

void login()
{
    char num[100];
    char password[100];
    char name[100];
    memset(num,0,sizeof(num));
    memset(password,0,sizeof(password));
    memset(name,0,sizeof(name));
    retry_flag:

    puts("Please enter your number and password");
    scanf("%s%s",num,password);getchar();
    deal_raw_string(num,sizeof(num));
    deal_raw_string(password,sizeof(password));
    cJSON *i=NULL;
    cJSON_ArrayForEach(i,teacher)
    {
        if(strcmp(cJSON_Print(cJSON_GetArrayItem(i,4)),password) == 0&&strcmp(cJSON_Print(cJSON_GetArrayItem(i,0)),num) == 0)
        {
            Course_select_system->identity = 1;
            strcpy(name,cJSON_Print(cJSON_GetArrayItem(i,2)));
            strcpy(Course_select_system->name,name);
            puts("Login Success. Your identity is teacher");
            goto end_flag;
        }
    }
    i=NULL;
    cJSON_ArrayForEach(i,student)
    {
        if(strcmp(cJSON_Print(cJSON_GetArrayItem(i,6)),password) == 0&&strcmp(cJSON_Print(cJSON_GetArrayItem(i,0)),num) == 0)
        {
            Course_select_system->identity = 2;
            strcpy(name,cJSON_Print(cJSON_GetArrayItem(i,3)));
            strcpy(Course_select_system->name,name);
            puts("Login Success. Your identity is student");
            goto end_flag;
        }
    }
    end_flag:;
    if(Course_select_system->identity == 0)
    {
        puts("Login Failed. Please retry.");
        goto retry_flag;
    }
    int cmd = 0 ;

    while(1)
    {
        if(Course_select_system->identity == 1)
        {
            puts("Press 1 to get info of courses, 2 to delete your courses, 3 to get statistics of your courses, 4 to query courses, 5 to add courses, 6 to edit courses, 7 to edit your profile.");
            puts("If you want to exit, press 0");
            scanf("%d", &cmd);getchar();
            if(cmd == 0) { Course_select_system->exit_system(); break;}
            else if(cmd == 1)Course_select_system->get_course_status();
            else if(cmd == 2)Course_select_system->delete_course_teacher();
            else if(cmd == 3)Course_select_system->get_stat();
            else if(cmd == 4)Course_select_system->search_course();
            else if(cmd == 5)Course_select_system->add_course();
            else if(cmd == 6)Course_select_system->edit_course();
            else if(cmd == 7)Course_select_system->manage_info();
            else puts("Invalid input. Please retry.");
        }
        if(Course_select_system->identity == 2)
        {
            puts("Press 1 to select courses, 2 to search courses, 3 to query results, 4 to delete courses, and 5 to edit profile.");
            puts("If you want to exit, press 0");
            scanf("%d", &cmd);getchar();
            if(cmd == 0) { Course_select_system->exit_system(); break;}
            else if(cmd == 1)Course_select_system->student_select_course();
            else if(cmd == 2)Course_select_system->search_course();
            else if(cmd == 3)Course_select_system->query_result(Course_select_system->name);
            else if(cmd == 4)Course_select_system->delete_course_student();
            else if(cmd == 5)Course_select_system->manage_info();
            else puts("Invalid input. Please retry.");
        }
    }
}

void add_course()
{
    cJSON *new_course = cJSON_CreateObject();
    puts("Please input number");
    int course_time_int = 0;
    char num[100],name[100],credit[100],hours[100],course_type[100],teacher_name[100],begin_time[100],end_time[100],course_time[100],limit[100],brief[100],info[100];
    gets(num);
    puts("Please input name");gets(name);
    puts("Please input credit");gets(credit);
    puts("Please input hours");gets(hours);
    puts("Please input course type");gets(course_type);
    puts("Please input teacher name");gets(teacher_name);
    puts("Please input begin time");gets(begin_time);
    puts("Please input end time");gets(end_time);
    puts("Please input course time");gets(course_time);
    puts("Please input limit");gets(limit);
    puts("Please input brief introdution");gets(brief);
    puts("Please input textbook");gets(info);

    cJSON_AddItemToObject(new_course, "num", cJSON_CreateString(num));
    cJSON_AddItemToObject(new_course, "name", cJSON_CreateString(name));
    cJSON_AddItemToObject(new_course, "credit", cJSON_CreateString(credit));
    cJSON_AddItemToObject(new_course, "hours", cJSON_CreateString(hours));
    cJSON_AddItemToObject(new_course, "course_type", cJSON_CreateString(course_type));
    cJSON_AddItemToObject(new_course, "teacher_name", cJSON_CreateString(teacher_name));
    cJSON_AddItemToObject(new_course, "begin_time", cJSON_CreateString(begin_time));
    cJSON_AddItemToObject(new_course, "end_time", cJSON_CreateString(end_time));
    cJSON_AddItemToObject(new_course, "course_time", cJSON_CreateString(course_time));
    cJSON_AddItemToObject(new_course, "limit", cJSON_CreateString(limit));
    cJSON_AddItemToObject(new_course, "brief", cJSON_CreateString(brief));
    cJSON_AddItemToObject(new_course, "info", cJSON_CreateString(info));

    deal_raw_string(num,sizeof(num));
    deal_raw_string(name,sizeof(name));
    deal_raw_string(credit,sizeof(credit));
    deal_raw_string(hours,sizeof(hours));
    deal_raw_string(course_type,sizeof(course_type));
    deal_raw_string(teacher_name,sizeof(teacher_name));
    deal_raw_string(begin_time,sizeof(begin_time));
    deal_raw_string(end_time,sizeof(end_time));
    deal_raw_string(course_time,sizeof(course_time));
    deal_raw_string(limit,sizeof(limit));
    deal_raw_string(brief,sizeof(brief));
    deal_raw_string(info,sizeof(info));
    course_time_int=parse_time(course_time);

    int flag = 0;
    int course_count = 0;
    int opened_course = 0;
    cJSON *i = NULL;
    cJSON_ArrayForEach(i,course)
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,0)),num))
        {
            puts("Course number already exists!");
            flag = 1;
            goto goto_flag;
        }
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,1)),name))
        {
            puts("Course name already exists!");
            flag = 2;
            goto goto_flag;
        }
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,5)),teacher_name))
        {
            course_count++;
            opened_course = parse_time(cJSON_Print(cJSON_GetArrayItem(i,8)));
            if((opened_course&course_time_int) != 0)
            {
                puts("Conflict courses!");
                flag = 4;
                goto goto_flag;
            }
            if(course_count >= 2)
            {
                puts("Too much courses!");
                flag = 3;
                goto goto_flag;
            }
        }
    }
    cJSON_AddItemToArray(course,new_course);
    puts("Add success");

    goto_flag:;
}

int parse_time(char* time_raw)
{
    if(!strcmp(time_raw,"\"8:00-8:50\""))
        return 0b1000000000;
    if(!strcmp(time_raw,"\"8:00-9:50\""))
        return 0b1100000000;
    if(!strcmp(time_raw,"\"8:00-10:50\""))
        return 0b1110000000;
    if(!strcmp(time_raw,"\"8:00-11:50\""))
        return 0b1111000000;
    if(!strcmp(time_raw,"\"9:00-9:50\""))
        return 0b0100000000;
    if(!strcmp(time_raw,"\"9:00-10:50\""))
        return 0b0110000000;
    if(!strcmp(time_raw,"\"9:00-11:50\""))
        return 0b0111000000;
    if(!strcmp(time_raw,"\"10:00-10:50\""))
        return 0b0010000000;
    if(!strcmp(time_raw,"\"10:00-11:50\""))
        return 0b0011000000;
    if(!strcmp(time_raw,"\"13:30-14:20\""))
        return 0b0000100000;
    if(!strcmp(time_raw,"\"13:30-15:20\""))
        return 0b0000110000;
    if(!strcmp(time_raw,"\"13:30-16:20\""))
        return 0b0000111000;
    if(!strcmp(time_raw,"\"13:30-17:20\""))
        return 0b0000111100;
    if(!strcmp(time_raw,"\"14:30-15:20\""))
        return 0b0000010000;
    if(!strcmp(time_raw,"\"14:30-16:20\""))
        return 0b0000011000;
    if(!strcmp(time_raw,"\"14:30-17:20\""))
        return 0b0000011100;
    if(!strcmp(time_raw,"\"15:30-16:20\""))
        return 0b0000001000;
    if(!strcmp(time_raw,"\"15:30-17:20\""))
        return 0b0000001100;
    if(!strcmp(time_raw,"\"16:30-17:20\""))
        return 0b0000000100;
    if(!strcmp(time_raw,"\"18:30-19:20\""))
        return 0b0000000010;
    if(!strcmp(time_raw,"\"18:30-20:20\""))
        return 0b0000000011;
    if(!strcmp(time_raw,"\"19:30-20:20\""))
        return 0b0000000001;
}

void delete_course_teacher()
{
    puts("Please enter the number of the course.");
    char num[100];
    memset(num,0,sizeof(num));
    gets(num);
    deal_raw_string(num,sizeof(num));
    cJSON *i = NULL;
    int index = 0;
    cJSON_ArrayForEach(i,course)
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,0)),num))
        {
            if(strcmp(cJSON_Print(cJSON_GetArrayItem(i,5)),Course_select_system->name) != 0)
            {
                puts("Not your class. Permission Denied.");
                goto goto_flag;
            }
            if(cJSON_GetArraySize(cJSON_GetArrayItem(i,13)) != 0)
            {
                puts("The course has already been selected by students. Permission Denied.");
                goto goto_flag;
            }
            puts("Success.");
            cJSON_DeleteItemFromArray(course,index);
            break;
        }
        index++;
    }

    goto_flag:;
}

void manage_info()
{
    char target_string[100];
    back_flag:;
    memset(target_string,0,sizeof(target_string));
    cJSON *i;
    if(Course_select_system->identity == 1)
    cJSON_ArrayForEach(i,teacher)
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,2)),Course_select_system->name))
        {
            invalid_flag_teacher:;
            puts("What do you want to edit?");
            puts("press 1 to edit email address and 2 to edit password");
            int opt = 0;
            scanf("%d",&opt);getchar();
            if(opt > 2 || opt < 1)
            {
                puts("Invalid input!");
                goto invalid_flag_teacher;
            }
            if(opt == 1)
            {
                puts("Please enter new address");
                scanf("%s",target_string);getchar();
                cJSON_SetValuestring(cJSON_GetArrayItem(i,3),target_string);
                goto success_flag;
            }
            else if(opt == 2)
            {
                puts("Please enter new password");
                scanf("%s",target_string);getchar();
                cJSON_SetValuestring(cJSON_GetArrayItem(i,4),target_string);
                goto success_flag;
            }
        }
    }
    else if(Course_select_system->identity == 2)
    cJSON_ArrayForEach(i,student)
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,3)),Course_select_system->name))
        {
            invalid_flag_student:;
            puts("What do you want to edit?");
            puts("press 1 to edit email address, 2 to edit password and 3 to edit phone number");
            int opt = 0;
            scanf("%d",&opt);getchar();
            if(opt > 3 || opt < 1)
            {
                puts("Invalid input!");
                goto invalid_flag_student;
            }
            if(opt == 1)
            {
                puts("Please enter new address");
                scanf("%s",target_string);getchar();
                cJSON_SetValuestring(cJSON_GetArrayItem(i,7),target_string);
                goto success_flag;
            }
            else if(opt == 2)
            {
                puts("Please enter new password");
                scanf("%s",target_string);getchar();
                cJSON_SetValuestring(cJSON_GetArrayItem(i,6),target_string);
                goto success_flag;
            }
            else if(opt == 3)
            {
                puts("Please enter new phone number");
                gets(target_string);
                cJSON_SetValuestring(cJSON_GetArrayItem(i,5),target_string);
                goto success_flag;
            }
        }
    }
    goto back_flag;
    puts("Success.");
    success_flag:;
}

void edit_course()
{
    puts("Please enter the number of the course.");
    char num[100];
    memset(num,0,sizeof(num));
    gets(num);
    deal_raw_string(num,sizeof(num));
    cJSON *i = NULL;
    int index = 0;
    int mod = 0;
    cJSON_ArrayForEach(i,course)
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,0)),num))
        {
            back_flag:;
            puts("What do you want to edit?");
            puts("press 1 to edit textbook, 2 to edit info 3 to edit limit");
            int opt = 0;
            scanf("%d",&opt);getchar();
            if(opt > 3 || opt < 1)
            {
                puts("Invalid Input!");
                goto back_flag;
            }
            if(strcmp(cJSON_Print(cJSON_GetArrayItem(i,5)),Course_select_system->name) != 0)
            {
                puts("Not your class. Permission Denied.");
                goto goto_flag;
            }
            if(cJSON_GetArraySize(cJSON_GetArrayItem(i,13)) != 0 && opt != 3)
            {
                puts("The course has already been selected by students. Permission Denied.");
                goto goto_flag;
            }
            char target_string[100];
            if(opt == 1)
            {
                puts("Please input the new textbook");
                gets(target_string);
                cJSON_SetValuestring(cJSON_GetArrayItem(i,12),target_string);
            }
            if(opt == 2)
            {
                puts("Please input the new info");
                gets(target_string);
                cJSON_SetValuestring(cJSON_GetArrayItem(i,11),target_string);
            }
            if(opt == 3)
            {
                puts("PLease input the new limit");
                scanf("%s",target_string);getchar();
                cJSON_SetValuestring(cJSON_GetArrayItem(i,10),target_string);
            }
            puts("Success.");

            break;
        }
        index++;
    }

    goto_flag:;
}

void student_select_course()
{
    char num[100];
    char parsed_name[100];
    memset(parsed_name,0,sizeof(parsed_name));
    deparse(Course_select_system->name,parsed_name);

    cJSON *i;
    int select_count = 0;
    int course_times[100];
    cJSON *new_student = cJSON_CreateObject();
    cJSON_AddStringToObject(new_student,"name",parsed_name);

    retry_flag:;
    puts("Please enter the class number");

    memset(num,0,sizeof(num));
    memset(course_times,0,sizeof(course_times));
    gets(num);
    deal_raw_string(num,sizeof(num));
    cJSON_ArrayForEach(i,course)
    {
        cJSON *j;
        cJSON_ArrayForEach(j,cJSON_GetArrayItem(i,13))
        {
            if(!strcmp(cJSON_Print(j->child),Course_select_system->name))
            {
                select_count++;
                if(select_count >= 3)
                {
                    puts("You have selected enough courses. Permission denied.");
                    goto retry_flag;
                }
                course_times[select_count] = parse_time(cJSON_Print(cJSON_GetArrayItem(i,8)));
            }
        }
    }
    cJSON_ArrayForEach(i,course)
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,0)),num))
        {
            if(strlen(course_times) == 1)
            {
                if((course_times[1]&parse_time(cJSON_Print(cJSON_GetArrayItem(i,8)))) != 0)
                {
                    puts("Conflict courses. Please retry.");
                    goto retry_flag;
                }
            }
            if(strlen(course_times) == 2)
            {
                if((course_times[1]&parse_time(cJSON_Print(cJSON_GetArrayItem(i,8)))) != 0 || (course_times[2]&parse_time(cJSON_Print(cJSON_GetArrayItem(i,8)))) != 0)
                {
                    puts("Conflict courses. Please retry.");
                    goto retry_flag;
                }
            }
            if(parse_string(cJSON_Print(cJSON_GetArrayItem(i,10))) == cJSON_GetArraySize(cJSON_GetArrayItem(i,13)))
            {
                puts("The course is closed. Please retry.");
                goto retry_flag;
            }
            cJSON_AddItemToArray(cJSON_GetArrayItem(i,13),new_student);
            puts("Success.");
            break;
        }
    }
}

void delete_course_student()
{
    puts("Please enter the class number");
    char num[100];
    memset(num,0,sizeof(num));
    deal_raw_string(num,sizeof(num));
    cJSON *i;
    cJSON_ArrayForEach(i,course)
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,0)),num))
        {
            cJSON *j;
            int index = 0;
            cJSON_ArrayForEach(j,cJSON_GetArrayItem(i,13))
            {
                if(!strcmp(j->child,Course_select_system->name))
                {
                    break;
                }
                index++;
            }
            cJSON_DeleteItemFromArray(cJSON_GetArrayItem(i,13),index);
            break;
        }
    }
}
void query_result(char* name)
{
    puts("Course Lists:");
    cJSON *i;
    cJSON_ArrayForEach(i,course)
    {
        cJSON *j;
        cJSON_ArrayForEach(j,cJSON_GetArrayItem(i,13))
        {
            if(!strcmp(cJSON_Print(i->child),name))
            {
                printf("%s\t%s\n",cJSON_Print(cJSON_GetArrayItem(i,0)),cJSON_Print(cJSON_GetArrayItem(i,1)));
            }
        }
    }
}
void get_course_status()
{

    int opt = 0;
    cJSON *i;
    char name[100];
    memset(name,0,sizeof(name));
    invalid_flag:;


    puts("Press 1 to get the info of your courses, 2 to search courses of a student, and 3 to search the students info of a course.");
    scanf("%d",&opt);getchar();


    if(opt > 3 || opt < 1)
    {
        puts("Invalid input!");
        goto invalid_flag;
    }
    if(opt == 1)
    {
        puts("Course Lists:\n");
        cJSON_ArrayForEach(i,course)
        {
            if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,5)),Course_select_system->name))
            {
                puts(cJSON_Print(i));
            }
        }
    }
    else if(opt == 2)
    {
        puts("Please enter the name of the student.");
        gets(name);
        deal_raw_string(name,sizeof(name));
        query_result(name);
    }
    else if(opt == 3)
    {
        puts("Please enter the name of the course.");
        gets(name);
        deal_raw_string(name,sizeof(name));
        cJSON_ArrayForEach(i,course)
        {
            if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,1)),name))
            {
                puts("Student Lists:");
                puts(cJSON_Print(cJSON_GetArrayItem(i,13)));
            }
        }
    }
}
void search_course()
{
    int opt = 0 ;
    char name[100];
    memset(name,0,sizeof(name));
    cJSON *i;

    retry_flag:;
    puts("\nPress 1 to search by course, or press 2 to search by faculty.");
    scanf("%d",&opt);getchar();
    if(opt > 2 || opt < 1)
    {
        puts("Invalid input. Please retry");
        goto retry_flag;
    }
    if(opt == 1)
    {
        puts("Please enter course name.");
        gets(name);
        deal_raw_string(name,sizeof(name));
        cJSON_ArrayForEach(i,course)
        {
            if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,1)),name))
            {
                cJSON *j;
                cJSON_ArrayForEach(j,i)
                {
                    if(strcmp(j->string, "students") != 0)
                    {
                        printf("%s:%s\n",j->string,cJSON_Print(j));
                    }
                }
                break;
            }
        }
    }
    else if(opt == 2)
    {
        puts("Please enter faculty name.");
        gets(name);
        deal_raw_string(name,sizeof(name));
        retry_flag_1:;
        puts("Press 1 to sort by limit or press 2 to sort by student counts");
        int opt = 0;
        scanf("%d",&opt);getchar();
        if(opt == 1)
        print_by_limit(name,"faculty");
        else if(opt == 2)
            print_by_student_count(name,"faculty");
        else
        {
            puts("Invalid input. Please retry");
            goto retry_flag_1;
        }

        puts("\nTo get more info, please search by name");
    }
}
int parse_string(char *string)
{
    char new_string [100];
    for(int i = 0; i < strlen(string) - 2; i++)
    {
        new_string[i] = string[i+1];
    }
    return atoi(new_string);
}

void deparse(char *string,char *new_string)
{
    for(int i = 0; i < strlen(string) - 2; i++)
    {
        new_string[i] = string[i+1];
    }
}


void get_stat()
{
    cJSON *i;
    int index = 0;
    int course_array[10];
    int cnt = 0;
    cJSON_ArrayForEach(i,course)
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,5)),Course_select_system->name))
        {
            course_array[cnt++] = index;
        }
        index ++;
    }
    printf("You have %d courses.\n",cnt);
    if(cnt == 1 || cnt == 2)
    {
        puts("Course Lists:\n");
        if (cnt == 1)
        {
            printf("%s\t%d\n", cJSON_Print(cJSON_GetArrayItem(cJSON_GetArrayItem(course, course_array[0]), 1)),
                   cJSON_GetArraySize(cJSON_GetArrayItem(cJSON_GetArrayItem(course, course_array[0]),13)));
        }
        else if(cnt == 2)
        {
            if(cJSON_GetArraySize(cJSON_GetArrayItem(course, course_array[0])) > cJSON_GetArraySize(cJSON_GetArrayItem(course, course_array[1])))
            {
                printf("%s\t%d\n", cJSON_Print(cJSON_GetArrayItem(cJSON_GetArrayItem(course, course_array[0]), 1)),
                       cJSON_GetArraySize(cJSON_GetArrayItem(course, course_array[1])));
                printf("%s\t%d\n", cJSON_Print(cJSON_GetArrayItem(cJSON_GetArrayItem(course, course_array[0]), 1)),
                       cJSON_GetArraySize(cJSON_GetArrayItem(course, course_array[0])));
            }
            else
            {
                printf("%s\t%d\n", cJSON_Print(cJSON_GetArrayItem(cJSON_GetArrayItem(course, course_array[0]), 1)),
                       cJSON_GetArraySize(cJSON_GetArrayItem(course, course_array[0])));
                printf("%s\t%d\n", cJSON_Print(cJSON_GetArrayItem(cJSON_GetArrayItem(course, course_array[0]), 1)),
                       cJSON_GetArraySize(cJSON_GetArrayItem(course, course_array[1])));
            }
        }
    }

}
void init_struct()
{
    courses = (Course*)calloc(cJSON_GetArraySize(course),sizeof(Course));
  //  printf("%d\n",cJSON_GetArraySize(course));
    cJSON *i;

    cJSON_ArrayForEach(i,course)
    {
        strcpy(courses[courses_size].name,cJSON_Print(cJSON_GetArrayItem(i,1)));
        courses[courses_size].student_count = cJSON_GetArraySize(cJSON_GetArrayItem(i,13));
        courses[courses_size].limit = parse_string(cJSON_Print(cJSON_GetArrayItem(i,10)))-courses[courses_size].student_count;
        courses_size++;
    }
}
int cmp(const void *a, const void *b)
{
    return ((Course*)a)->student_count - ((Course*)b)->student_count;
}

void print_by_limit(char *name, char *key)
{
    qsort(courses,courses_size,sizeof(Course),cmp);
    for(int i = courses_size - 1; i >= 0; i--)
    {
        cJSON *j;
        cJSON_ArrayForEach(j,course)
        {
            if(!strcmp(cJSON_Print(cJSON_GetArrayItem(j,1)),courses[i].name))
            {
                cJSON *z;
                cJSON_ArrayForEach(z,j)
                {
                    if(!strcmp(z->string,key))
                    {
                        if(!strcmp(cJSON_Print(z),name))
                            printf("%s\t%d\n",courses[i].name,courses[i].limit);
                    }
                }
            }
        }

    }
}

void print_by_student_count(char *name, char *key)
{
    qsort(courses,courses_size,sizeof(Course),cmp);
    for(int i = 0; i < courses_size; i++)
    {
        cJSON *j;
        cJSON_ArrayForEach(j,course)
        {
            if(!strcmp(cJSON_Print(cJSON_GetArrayItem(j,1)),courses[i].name))
            {
                cJSON *z;
                cJSON_ArrayForEach(z,j)
                {
                    if(!strcmp(z->string,key))
                    {
                        if(!strcmp(cJSON_Print(z),name))
                            printf("%s\t%d\n",courses[i].name,courses[i].student_count);
                    }
                }
            }
        }

    }
}

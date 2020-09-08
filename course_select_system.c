#include "course_select_system.h"
#include<stdio.h>
#include<stdlib.h>
#include "cJSON.h"
#include<string.h>

extern course_select_system*  Course_select_system;//系统
FILE *fp_teacher = NULL,*fp_student = NULL,*fp_course = NULL;//文件指针
char *content_teacher, *content_student, *content_course;//文件内容
int len_teacher,len_student,len_course;//文件长度
int courses_size;//总课程数
cJSON *teacher,*student,*course;//解析出的三个cjson对象
Course *courses;//课程数组

void init()
{
    Course_select_system = (course_select_system*)malloc(sizeof(course_select_system));
    Course_select_system->identity = 0;
    memset(Course_select_system->name,0,sizeof(Course_select_system->name));//初始化结构体

    fp_teacher = fopen("../Teacher.json","rw+");
    fseek(fp_teacher,0,SEEK_END);
    len_teacher=ftell(fp_teacher);
    fseek(fp_teacher,0,SEEK_SET);
    content_teacher=(char*)malloc(len_teacher+1);
    fread(content_teacher,1,len_teacher,fp_teacher);
    teacher = cJSON_Parse(content_teacher);
    fseek(fp_teacher,0,SEEK_SET);//读入并解析教师数据

    fp_student = fopen("../Student.json","rw+");
    fseek(fp_student,0,SEEK_END);
    len_student=ftell(fp_student);
    fseek(fp_student,0,SEEK_SET);
    content_student=(char*)malloc(len_student+1);
    fread(content_student,1,len_student,fp_student);
    student = cJSON_Parse(content_student);
    fseek(fp_student,0,SEEK_SET);//读入并解析学生数据

    fp_course = fopen("../course.json","rw+");
    fseek(fp_course,0,SEEK_END);
    len_course=ftell(fp_course);
    fseek(fp_course,0,SEEK_SET);
    content_course=(char*)malloc(len_course+1);
    fread(content_course,1,len_course,fp_course);
    course = cJSON_Parse(content_course);
    fseek(fp_course,0,SEEK_SET);//读入并解析课程数据

    init_struct();
}

void exit_system()//退出系统，写入并关闭文件，释放内存
{
    free(courses);
    free(Course_select_system);
    fwrite(cJSON_Print(course),1,strlen(cJSON_Print(course)),fp_course);
    fwrite(cJSON_Print(teacher),1,strlen(cJSON_Print(teacher)),fp_teacher);
    fwrite(cJSON_Print(student),1,strlen(cJSON_Print(student)),fp_student);
    fclose(fp_student);
    fclose(fp_teacher);
    fclose(fp_course);
}

void deal_raw_string(char* raw, int len_raw)//将输入字符串加上双引号
{
    char a[100];
    int len=strlen(a);
    memset(a, 0, len);//清空数组a
    a[0]=(char)34;//给a首元素赋值为双引号
    strcat(a,raw);//拼接数组a和数组raw
    memset(raw,0,len_raw);//清空数组raw
    raw[0]=(char)34;//给raw首元素赋值为双引号
    strcat(a,raw);
    strcpy(raw,a);//拼接，复制，raw中此时为完整的带双引号字符串
}

int check_valid_input(int l, int r)//校验输入，如果输入不合法，重复获取直到合法为止
{
    int opt = 0;
    while(1)
    {
        scanf("%d", &opt);
        getchar();//过滤\n
        if (opt > r || opt < l) {
            puts("Invalid input!");
            continue;
        }
        break;
    }
    return opt;
}

int login(char *password, char *num)
{
    memset(num,0,sizeof(num));
    memset(password,0,sizeof(password));

    puts("Please enter your number and password");
    scanf("%s%s",num,password);getchar();
    deal_raw_string(num,sizeof(num));
    deal_raw_string(password,sizeof(password));//输入账号密码并处理

    cJSON *i=NULL;//定义cJSON循环变量i

    cJSON_ArrayForEach(i,teacher)//遍历teacher文件
    {
        //比较输入的账号密码是否在teacher数据库中
        if(strcmp(cJSON_Print(cJSON_GetArrayItem(i,4)),password) == 0&&strcmp(cJSON_Print(cJSON_GetArrayItem(i,0)),num) == 0)
        {
            Course_select_system->identity = 1;//将身份设置为老师
            strcpy(Course_select_system->name,cJSON_Print(cJSON_GetArrayItem(i,2)));
            puts("Login Success. Your identity is teacher");
            return 1;
        }
    }
    i=NULL;
    cJSON_ArrayForEach(i,student)
    {
        //比较输入的账号密码是否在student数据库中
        if(strcmp(cJSON_Print(cJSON_GetArrayItem(i,6)),password) == 0&&strcmp(cJSON_Print(cJSON_GetArrayItem(i,0)),num) == 0)
        {
            Course_select_system->identity = 2;//将身份设置为学生
            strcpy(Course_select_system->name,cJSON_Print(cJSON_GetArrayItem(i,3)));
            puts("Login Success. Your identity is student");
            return 1;
        }
    }
    if(Course_select_system->identity == 0)
    {
        puts("Login Failed. Please retry.");
        return 0;
    }
}

void add_side()//添加边框
{
    for (int i = 1; i < 50; i++)
    {
        printf("*");
    }
    printf("\n");
}
void menu()//菜单
{
    //验证是否读取数据成功，未成功退出程序
    if(fp_course == NULL || fp_teacher == NULL || fp_student == NULL || course->type == cJSON_Invalid || teacher->type == cJSON_Invalid || student->type == cJSON_Invalid )
    {
        puts("The files are deleted or damaged. Please fix and rerun.");
        exit_system();
    }

    char num[100];
    char password[100];

    while(1)
    {
        if(login(password,num))break;
    }

    int cmd = 0;
    int opt = 0;

    while(1)
    {
        if(Course_select_system->identity == 1)
        {
            add_side();
            puts("Teacher's functions");
            puts("(1) Get info of courses.");
            puts("(2) Delete your courses.");
            puts("(3) Get statistics of your courses.");
            puts("(4) Query courses.");
            puts("(5) Add courses.");
            puts("(6) Edit courses.");
            puts("(7) Edit your profile.");
            puts("If you want to exit, press 0.");
            add_side();

            scanf("%d", &cmd);getchar();

            if(cmd == 0)
            {
                exit_system();
                break;
            }
            else if(cmd == 1)get_course_status();
            else if(cmd == 2)
            {
                while(1)
                {
                    if(delete_course_teacher())break;
                    puts("To retry please press 1, or press other if you want to back to menu");
                    scanf("%d",&opt);getchar();
                    if(opt != 1)break;
                }
            }
            else if(cmd == 3)get_stat();
            else if(cmd == 4)
            {
                while(1)
                {
                    if(search_course())break;
                    puts("To retry please press 1, or press other if you want to back to menu");
                    scanf("%d",&opt);getchar();
                    if(opt != 1)break;
                }
            }
            else if(cmd == 5)
            {
                while(1)
                {
                    if(add_course())break;
                    puts("To retry please press 1, or press other if you want to back to menu");
                    scanf("%d",&opt);getchar();
                    if(opt != 1)break;
                }
            }
            else if(cmd == 6)
            {
                while(1)
                {
                    if(edit_course())break;
                    puts("To retry please press 1, or press other if you want to back to menu");
                    scanf("%d",&opt);getchar();
                    if(opt != 1)break;
                }
            }
            else if(cmd == 7)
            {
                while(1)
                {
                    if(manage_info())break;
                    puts("To retry please press 1, or press other if you want to back to menu");
                    scanf("%d",&opt);getchar();
                    if(opt != 1)break;
                }
            }
            else puts("Invalid input. Please retry.");
        }
        if(Course_select_system->identity == 2)
        {
            add_side();
            puts("Student's functions");
            puts( "(1) Select courses.");
            puts( "(2) Search courses. ");
            puts( "(3) Query results. ");
            puts( "(4) Delete courses.");
            puts( "(5) Edit profile.");
            puts("If you want to exit, press 0.");
            add_side();

            scanf("%d", &cmd);getchar();

            if(cmd == 0)
            {
                exit_system();
                break;
            }
            else if(cmd == 1)
            {
                while(1)
                {
                    if(student_select_course())break;
                    puts("To retry please press 1, or press other if you want to back to menu");
                    scanf("%d",&opt);getchar();
                    if(opt != 1)break;
                }
            }
            else if(cmd == 2)
            {
                while(1)
                {
                    if(search_course())break;
                    puts("To retry please press 1, or press other if you want to back to menu");
                    scanf("%d",&opt);getchar();
                    if(opt != 1)break;
                }
            }
            else if(cmd == 3)query_result(Course_select_system->name);
            else if(cmd == 4)
            {
                while(1)
                {
                    if(delete_course_student())break;
                    puts("To retry please press 1, or press other if you want to back to menu");
                    scanf("%d",&opt);getchar();
                    if(opt != 1)break;
                }
            }
            else if(cmd == 5)
            {
                while(1)
                {
                    if(manage_info())break;
                    puts("To retry please press 1, or press other if you want to back to menu");
                    scanf("%d",&opt);getchar();
                    if(opt != 1)break;
                }
            }
            else puts("Invalid input. Please retry.");
        }
    }
}

int add_course()//增加课程
{
    cJSON *new_course = cJSON_CreateObject();//新建一个课程作为cjson对象
    int course_time_int = 0;
    int course_count = 0;//课程计数
    int opened_course = 0;//
    char num[100],name[100],credit[100],hours[100],course_type[100],teacher_name[100],begin_time[100],end_time[100],course_time[100],limit[100],brief[100],info[100];

    puts("Please input number");gets(num);
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
    puts("Please input textbook");gets(info);//输入课程属性

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
    cJSON_AddItemToObject(new_course, "info", cJSON_CreateString(info));//把以上属性增加到新的课程里

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
    deal_raw_string(info,sizeof(info));//对以上数据进行字符串的格式化处理

    course_time_int=parse_time(course_time);

    cJSON *i = NULL;//定义cJSON循环变量i
    cJSON_ArrayForEach(i,course)//遍历course数据
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,0)),num))//判断课号是否存在
        {
            puts("Course number already exists!");
            return 0;
        }
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,1)),name))//判断名字是否存在
        {
            puts("Course name already exists!");
            return 0;
        }
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,5)),teacher_name))//判断教师名字是否存在
        {
            course_count++;//如果存在 计数器加一
            opened_course = parse_time(cJSON_Print(cJSON_GetArrayItem(i,8)));//解析当前课程

            if((opened_course&course_time_int) != 0)//判断当前课程和新增课程是否冲突
            {
                puts("Conflict courses!");
                return 0;
            }
            if(course_count >= 2)
            {
                puts("Too much courses!");
                return 0;
            }
        }
    }
    cJSON_AddItemToArray(course,new_course);//将新增的课程追加到课程列表的cjson对象中
    puts("Success.");
    return 1;
}

int parse_time(char* time_raw)
{
    if(!strcmp(time_raw,"\"8:00-8:50\""))//比较上课时间是否和此时间段一致
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
}//用不同的二进制数表示不同上课时间的课程，二进制位共十位，从高位开始，若第i位为真，代表第i节课有课，否则无课

int delete_course_teacher()
{
    puts("Please enter the number of the course.");

    char num[100];
    memset(num,0,sizeof(num));
    gets(num);
    deal_raw_string(num,sizeof(num));//输入并处理课号

    cJSON *i = NULL;
    int index = 0;//记录当前下标
    cJSON_ArrayForEach(i,course)
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,0)),num))//判断当前课程是否为输入课程
        {
            if(strcmp(cJSON_Print(cJSON_GetArrayItem(i,5)),Course_select_system->name) != 0)//判断开课老师是不是登录老师
            {
                puts("Not your class. Permission Denied.");
                return 0;
            }
            if(cJSON_GetArraySize(cJSON_GetArrayItem(i,13)) != 0)//判断是否有学生已经选课
            {
                puts("The course has already been selected by students. Permission Denied.");
                return 0;
            }
            puts("Success.");
            cJSON_DeleteItemFromArray(course,index);//从course对象中将该课程删除
            return 1;
        }
        index++;
    }
    puts("Number not found!");
    return 0;
}

int manage_info()
{
    char target_string[100];
    memset(target_string,0,sizeof(target_string));
    cJSON *i;

    if(Course_select_system->identity == 1)//身份为老师
    cJSON_ArrayForEach(i,teacher)//遍历teacher数据库
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,2)),Course_select_system->name))//判断当前老师是否为登录的老师
        {
            add_side();
            puts("What do you want to edit?");
            puts("(1) Edit email address.");
            puts("(2) Edit password.");
            add_side();

            int opt = check_valid_input(1,2);
            if(opt == 1)
            {
                while(1)//获取合法邮箱输入
                {
                    puts("Please enter new email address.");
                    gets(target_string);
                    if(check_valid_email(target_string,strlen(target_string)))break;
                    puts("Invalid input. Please retry.");
                    puts("Example: example@example.com");
                }
                cJSON_SetValuestring(cJSON_GetArrayItem(i,3),target_string);//将数据库中邮箱替换
                puts("Success.");
                return 1;
            }
            else if(opt == 2)
            {
                puts("Please input new password.");
                gets(target_string);
                cJSON_SetValuestring(cJSON_GetArrayItem(i,4),target_string);//将数据库中手机替换
                puts("Success.");
                return 1;
            }
        }
    }
    else if(Course_select_system->identity == 2)
    cJSON_ArrayForEach(i,student)
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,3)),Course_select_system->name))
        {
            invalid_flag_student:;
            add_side();
            puts("What do you want to edit?");
            puts("(1) Edit email address.");
            puts("(2) Edit password.");
            puts("(3) Edit phone number.");
            add_side();
            int opt = check_valid_input(1,3);
            if(opt == 1)
            {
                while(1)//获取合法邮箱输入
                {
                    puts("Please enter new email address.");
                    gets(target_string);
                    if(check_valid_email(target_string,strlen(target_string)))break;
                    puts("Invalid input. Please retry.");
                    puts("Example: example@example.com");
                }
                cJSON_SetValuestring(cJSON_GetArrayItem(i,7),target_string);
                puts("Success.");
                return 1;
            }
            else if(opt == 2)
            {
                puts("Please enter new password.");
                gets(target_string);
                cJSON_SetValuestring(cJSON_GetArrayItem(i,6),target_string);
                puts("Success.");
                return 1;
            }
            else if(opt == 3)
            {
                while (1)//获取合法电话号码输入
                {
                    puts("Please enter new phone number.");
                    gets(target_string);
                    if(check_valid_phone_num(target_string))break;
                    puts("Invalid input. Please retry.");
                    puts("Telephone number should be 11 characters and begin with 1.");
                }
                cJSON_SetValuestring(cJSON_GetArrayItem(i,5),target_string);
                puts("Success.");
                return 1;
            }
        }
    }
}

int edit_course()
{
    puts("Please enter the number of the course.");

    char num[100];
    memset(num,0,sizeof(num));
    gets(num);
    deal_raw_string(num,sizeof(num));//输入并处理课程编号

    cJSON *i = NULL;
    cJSON_ArrayForEach(i,course)//遍历course数据库
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,0)),num))//如果当前课程为输入课程
        {
            add_side();
            puts("What do you want to edit?");
            puts("(1) Edit textbook.");
            puts("(2) Edit info.");
            puts("(3) Edit limit.");
            add_side();

            int opt = check_valid_input(1,3);
            if(strcmp(cJSON_Print(cJSON_GetArrayItem(i,5)),Course_select_system->name) != 0)//该课程不是登录老师的
            {
                puts("Not your class. Permission Denied.");
                return 0;
            }
            if(cJSON_GetArraySize(cJSON_GetArrayItem(i,13)) != 0 && opt != 3)//该课程已有学生选择
            {
                puts("The course has already been selected by students. Permission Denied.");
                return 0;
            }
            char target_string[100];
            if(opt == 1)
            {
                puts("Please input the new textbook");
                gets(target_string);
                cJSON_SetValuestring(cJSON_GetArrayItem(i,12),target_string);//更新教材
            }
            if(opt == 2)
            {
                puts("Please input the new info");
                gets(target_string);
                cJSON_SetValuestring(cJSON_GetArrayItem(i,11),target_string);//更新简介
            }
            if(opt == 3)
            {
                int temp;
                while(1)
                {
                    puts("Please input the new limit");
                    if(temp = check_valid_input(1,100))//选课名额上限100人
                    {
                        break;
                    }
                    puts("Invalid input. Pleas retry.");
                }
                strcpy(target_string, itoa(temp,target_string,10));
                cJSON_SetValuestring(cJSON_GetArrayItem(i,10),target_string);//更新选课名额
            }
            puts("Success.");
            return 1;
        }
    }
    puts("Class not Found!");
    return 0;
}

int student_select_course()
{
    char num[100];
    cJSON *i;
    int select_count = 0;
    int course_times[100];
    char parsed_name[100];

    memset(num,0,sizeof(num));//初始化变量
    memset(course_times,0,sizeof(course_times));
    memset(parsed_name,0,sizeof(parsed_name));
    deparse(Course_select_system->name,parsed_name);//将脱掉引号的登录名字存储到parsed_name中
  
    cJSON *new_student = cJSON_CreateObject();
    cJSON_AddStringToObject(new_student,"name",parsed_name);//在new_student对象中添加name属性

    puts("Please enter the class number");
    gets(num);
    deal_raw_string(num,sizeof(num));//给输入的num加引号使其与文件中的数据格式一致

    cJSON_ArrayForEach(i,course)
    {
        cJSON *j;
        cJSON_ArrayForEach(j,cJSON_GetArrayItem(i,13))
        {
            if(!strcmp(cJSON_Print(j->child),Course_select_system->name))//判断学生是否选了当前课程
            {
                select_count++;
                if(select_count >= 3)//判断选课数是否大于三
                {
                    puts("You have selected enough courses. Permission denied.");
                    return 0;
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
                if((course_times[1]&parse_time(cJSON_Print(cJSON_GetArrayItem(i,8)))) != 0)//判断课程时间是否冲突
                {
                    puts("Conflict courses. Please retry.");
                    return 0;
                }
            }
            if(strlen(course_times) == 2)
            {
                if((course_times[1]&parse_time(cJSON_Print(cJSON_GetArrayItem(i,8)))) != 0 || (course_times[2]&parse_time(cJSON_Print(cJSON_GetArrayItem(i,8)))) != 0)
                {
                    puts("Conflict courses. Please retry.");
                    return 0;
                }
            }
            if(parse_string(cJSON_Print(cJSON_GetArrayItem(i,10))) == cJSON_GetArraySize(cJSON_GetArrayItem(i,13)))//判断课程人数是否满员
            {
                puts("The course is closed. Please retry.");
                return 0;
            }
            cJSON_AddItemToArray(cJSON_GetArrayItem(i,13),new_student);//添加成功并增加一位学生名额
            puts("Success.");
            return 1;
        }
    }
    puts("Course not found. Please retry.");
    return 0;
}

int delete_course_student()
{
    puts("Please enter the class number");

    char num[100];
    memset(num,0,sizeof(num));//初始化num
    gets(num);
    deal_raw_string(num,sizeof(num));//给输入的num加引号使其与文件中的数据格式一致

    cJSON *i;
    cJSON_ArrayForEach(i,course)
    {
        if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,0)),num))//比较course文件里的number与输入的num
        {
            cJSON *j;
            int index = 0;
            cJSON_ArrayForEach(j,cJSON_GetArrayItem(i,13))//遍历选课的学生名单
            {
                if(!strcmp(cJSON_Print(j->child),Course_select_system->name))//查找该选课中是否存在自己的信息
                {
                    cJSON_DeleteItemFromArray(cJSON_GetArrayItem(i,13),index);//从数据库中删除该课程
                    puts("Success.");
                    return 1;
                }
                index++;
            }
            puts("This is not your course. Please retry.");
            return 0;
        }
    }
    puts("Course not found. Please retry.");

}
void query_result(char* name)
{
    puts("Course Lists:");
    cJSON *i;
    cJSON_ArrayForEach(i,course)
    {
        cJSON *j;
        cJSON_ArrayForEach(j,cJSON_GetArrayItem(i,13))//遍历选课的学生名单
        {
            if(!strcmp(cJSON_Print(j->child),name))//查找该选课中是否存在自己的信息
            {
                printf("%s\t%s\n",cJSON_Print(cJSON_GetArrayItem(i,0)),cJSON_Print(cJSON_GetArrayItem(i,1)));
                //输出自己所有已选课程信息
            }
        }
    }
}
void get_course_status()
{
    cJSON *i;
    char name[100];
    memset(name,0,sizeof(name));

    add_side();
    puts("(1) Get the info of your courses.");
    puts("(2) Search courses of a student.");
    puts("(3) Search the students info of a course.");
    add_side();

    int opt = check_valid_input(1,3);
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
int search_course()
{
    char name[100];
    memset(name,0,sizeof(name));//初始化变量
    cJSON *i;
    add_side();//打星函数
    puts("Search methods:");
    puts("(1) Search by course.");
    puts("(2) Search by faculty.");
    add_side();
    int opt = check_valid_input(1,2);//过滤非法输入
    if(opt == 1)
    {
        puts("Please enter course name.");
        gets(name);
        deal_raw_string(name,sizeof(name));//给输入的name加引号使其与文件中的数据格式一致
        cJSON_ArrayForEach(i,course)
        {
            if(!strcmp(cJSON_Print(cJSON_GetArrayItem(i,1)),name))
            {
                puts("Success");
                cJSON *j;
                cJSON_ArrayForEach(j,i)//遍历i的所有元素
                {
                    if(strcmp(j->string, "students") != 0)
                    {
                        printf("%s:%s\n",j->string,cJSON_Print(j));
                    }
                }
                puts("To get more info, please search by name");
                return 1;
            }
        }
        puts("Course not found. Please retry.");
        return 0;
    }
    else if(opt == 2)
    {
        puts("Please enter faculty name.");
        gets(name);
        deal_raw_string(name,sizeof(name));
        add_side();
        puts("Sort method:");
        puts("(1) Sort by limit.");
        puts("(2) Sort by student counts");
        add_side();
        int opt_1 = check_valid_input(1,2);
        if(opt_1 == 1)
            print_by_limit(name,"faculty");//按限制人数排序
        else if(opt_1 == 2)
            print_by_student_count(name,"faculty");//按已选人数排序
        return 1;
    }
}
int parse_string(char *string)//将带双引号的整数字符串解析为int类型 例如将"123"转换为123
{
    char new_string [100];

    for(int i = 0; i < strlen(string) - 2; i++)//去除双引号
    {
        new_string[i] = string[i+1];
    }

    return atoi(new_string);//返回处理结果的整型数值
}

void deparse(char *string,char *new_string)//去除双引号
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

    cJSON_ArrayForEach(i,course)//遍历course查找自己选的课
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
    courses = (Course*)calloc(cJSON_GetArraySize(course),sizeof(Course));//初始化
    cJSON *i;

    cJSON_ArrayForEach(i,course)
    {
        strcpy(courses[courses_size].name,cJSON_Print(cJSON_GetArrayItem(i,1)));
        courses[courses_size].student_count = cJSON_GetArraySize(cJSON_GetArrayItem(i,13));
        courses[courses_size].limit = parse_string(cJSON_Print(cJSON_GetArrayItem(i,10)))-courses[courses_size].student_count;
        courses_size++;
    }
}

int cmp(const void *a, const void *b)//快排比较函数
{
    return ((Course*)a)->student_count - ((Course*)b)->student_count;
}

void print_by_limit(char *name, char *key)//根据选课上限排序
{
    int cnt = 0;//课程计数
    qsort(courses,courses_size,sizeof(Course),cmp);//快排
    for(int i = courses_size - 1; i >= 0; i--)//遍历排序后的课程
    {
        cJSON *j;
        cJSON_ArrayForEach(j,course)
        {
            if(!strcmp(cJSON_Print(cJSON_GetArrayItem(j,1)),courses[i].name))//在数据库中寻找当前课程
            {
                cJSON *z;
                cJSON_ArrayForEach(z,j)//遍历当前课程的元素
                {
                    if(!strcmp(z->string,key))//寻找筛选条件的元素
                    {
                        if(!strcmp(cJSON_Print(z),name))//如果满足筛选条件
                        {
                            cnt++;
                            printf("%s\t%d\n", courses[i].name, courses[i].limit);
                        }
                    }
                }
            }
        }

    }
    if(!cnt)
    {
        puts("Course not found. Please check and retry.");
    }
}

void print_by_student_count(char *name, char *key)//根据已选人数排序
{
    int cnt = 0;//课程计数
    qsort(courses,courses_size,sizeof(Course),cmp);
    for(int i = 0; i < courses_size; i++)//遍历所有课程
    {
        cJSON *j;
        cJSON_ArrayForEach(j,course)//遍历排序后的课程
        {
            if(!strcmp(cJSON_Print(cJSON_GetArrayItem(j,1)),courses[i].name))//在数据库中寻找当前课程
            {

                cJSON *z;
                cJSON_ArrayForEach(z,j)//遍历当前课程的元素
                {
                    if(!strcmp(z->string,key))//验证是否满足筛选条件
                    {
                        if(!strcmp(cJSON_Print(z),name))//输出除name外的信息
                        {
                            cnt++;
                            printf("%s\t%d\n", courses[i].name, courses[i].student_count);
                        }
                    }
                }
            }
        }

    }
    if(!cnt)
    {
        puts("Course not found. Please check and retry.");
    }
}

int check_valid_email(char *email, int len)
{
    int pos1 = 0, pos2 = 0;//pos1为@位置，pos2为.位置
    for(int i = 0 ; i < len ; i++)
    {
        if(email[i] != '@' && email[i] != '.' && (email[i] > '9' || email[i] < '0') && (email[i] > 'Z' || email[i] < 'A') && (email[i] > 'z' || email[i] < 'a'))
        {
            return 0;
        }
        if(email[i] == '@')
        {
            if(pos1 != 0)
                return 0;
            pos1 = i;
        }
        if(email[i] == '.')
        {
            if(pos2 != 0)
                return 0;
            pos2 = i;
        }
    }
    if(!pos1 || !pos2) return 0;
    if(pos2 - pos1 < 2)return 0;
    return 1;
}

int check_valid_phone_num(char *num)
{
    if(atoll(num) > 20000000000 || atoll(num) < 10000000000)
    {
        return 0;
    }
    return 1;
}
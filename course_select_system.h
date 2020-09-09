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

void menu();//菜单
int login(char *password, char *num);//登录
int student_select_course();//选课
int search_course();//查课
void query_result(char *name);//查询选课结果
int delete_course_student();//学生删除已选课程
int manage_info();//管理个人信息
void get_course_status();//获取课程状态
int delete_course_teacher();//教师删除课程
void get_stat();//教师获取自己的课程信息
int add_course();//教师加课
int edit_course();//教师修改课程
void init();//初始化
void exit_system();//退出系统
void deal_raw_string(char* raw,int len_raw);//为给定字符串加双引号
int parse_time(char* time_raw);//将上课时间解析为二进制数
int parse_string(char *string);//将给定带双引号的字符串解析为整数
void init_struct();//读取课程列表并存储在结构体中
void print_by_limit(char *name,char *key);//按选课名额输出课程列表
void print_by_student_count(char *name,char *key);//按选课人数输出课程列表
int cmp_student_count(const void *a, const void *b);//按选课人数的排序比较函数
int cmp_limit(const void *a, const void *b);//按选课名额的排序比较函数
void deparse(char *string,char *new_string);//脱去给定字符串的双引号
int check_valid_input(int l, int r);//重复获取输入直到输入合法为止
int check_valid_email(char *email, int len);//检查邮箱地址是否合法
int check_valid_phone_num(char *num);//检查电话号码是否合法
#endif //STUDENT_SELECT_CLASS_SYSTEM_COURSE_SELECT_SYSTEM_H

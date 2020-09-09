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

void menu();//�˵�
int login(char *password, char *num);//��¼
int student_select_course();//ѡ��
int search_course();//���
void query_result(char *name);//��ѯѡ�ν��
int delete_course_student();//ѧ��ɾ����ѡ�γ�
int manage_info();//���������Ϣ
void get_course_status();//��ȡ�γ�״̬
int delete_course_teacher();//��ʦɾ���γ�
void get_stat();//��ʦ��ȡ�Լ��Ŀγ���Ϣ
int add_course();//��ʦ�ӿ�
int edit_course();//��ʦ�޸Ŀγ�
void init();//��ʼ��
void exit_system();//�˳�ϵͳ
void deal_raw_string(char* raw,int len_raw);//Ϊ�����ַ�����˫����
int parse_time(char* time_raw);//���Ͽ�ʱ�����Ϊ��������
int parse_string(char *string);//��������˫���ŵ��ַ�������Ϊ����
void init_struct();//��ȡ�γ��б��洢�ڽṹ����
void print_by_limit(char *name,char *key);//��ѡ����������γ��б�
void print_by_student_count(char *name,char *key);//��ѡ����������γ��б�
int cmp_student_count(const void *a, const void *b);//��ѡ������������ȽϺ���
int cmp_limit(const void *a, const void *b);//��ѡ�����������ȽϺ���
void deparse(char *string,char *new_string);//��ȥ�����ַ�����˫����
int check_valid_input(int l, int r);//�ظ���ȡ����ֱ������Ϸ�Ϊֹ
int check_valid_email(char *email, int len);//��������ַ�Ƿ�Ϸ�
int check_valid_phone_num(char *num);//���绰�����Ƿ�Ϸ�
#endif //STUDENT_SELECT_CLASS_SYSTEM_COURSE_SELECT_SYSTEM_H

#ifndef _STU_H
#define _STU_H

#include "common.h"

#define FIND_BY_MATH 1
#define FIND_BY_CN 2
#define FIND_BY_EN 3
#define FIND_BY_TOTAL 4

#define PWD_SET 1  //�״�ʹ�����ʱ��������
#define PWD_ALTER 2  //�޸�����
#define PWD_CHECK 3  //ʹ��������޸�����ʱȷ������
#define PWD_ERROR_TIMES 4  //��������Ĵ������

//ѧ����Ϣ�ṹ��
typedef struct _STU{
	int id;  //ѧ��
	char name[20];  //����
	char sex[4];  //�Ա�
	int age;  //����
	float math;  //��ѧ�ɼ�
	float cn;  //���ĳɼ�
	float en;  //Ӣ��ɼ�
}STU;

//����ڵ�
typedef struct _NODE{
	STU data;
	struct _NODE *next;
}NODE;

//����ͷ���
struct{
	int num;  //ѧ������
	NODE *link;  //����ָ��
}header;

FILE *fp;  //�ļ�ָ��
FILE *fpTemp;  //��ʱ�ļ�
char pwdCurrent[PWD_LEN+1];  //�ļ�����
int pwdLen;  //���볤��

extern void init();  //��ʼ��
extern void updateFile();  //�����ļ�
extern void setPwd(int flag);  //��������
extern int checkPwd(int flag);  //У������

//ѧ����Ϣ��ɾ�Ĳ�
extern void addStuInfo();
extern void deleteStuInfo();
extern void alterStuInfo();
extern void findStuByID();
extern void findStuByName();
extern void findStuByScores(int flag);
extern void showAllStu();

//У��ѧ����Ϣ
extern int checkStuID(int stuID);
extern int checkStuName(char name[]);
extern int checkStuSex(char sex[]);
extern int checkStuAge(int age);
extern int checkStuMath(float math);
extern int checkStuCN(float cn);
extern int checkStuEN(float en);

//����ѧ����Ϣ
extern void getStuID(int *id);
extern void getStuName(char name[]);
extern void getStuSex(char sex[]);
extern void getStuAge(int *age);
extern void getStuMath(float *math);
extern void getStuCN(float *cn);
extern void getStuEN(float *en);

#endif
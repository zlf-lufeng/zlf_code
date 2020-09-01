#ifndef _STU_H
#define _STU_H

#include "common.h"

#define FIND_BY_MATH 1
#define FIND_BY_CN 2
#define FIND_BY_EN 3
#define FIND_BY_TOTAL 4

#define PWD_SET 1  //首次使用软件时设置密码
#define PWD_ALTER 2  //修改密码
#define PWD_CHECK 3  //使用软件、修改密码时确认密码
#define PWD_ERROR_TIMES 4  //密码输入的错误次数

//学生信息结构体
typedef struct _STU{
	int id;  //学号
	char name[20];  //姓名
	char sex[4];  //性别
	int age;  //年龄
	float math;  //数学成绩
	float cn;  //语文成绩
	float en;  //英语成绩
}STU;

//链表节点
typedef struct _NODE{
	STU data;
	struct _NODE *next;
}NODE;

//链表头结点
struct{
	int num;  //学生总数
	NODE *link;  //链表指针
}header;

FILE *fp;  //文件指针
FILE *fpTemp;  //临时文件
char pwdCurrent[PWD_LEN+1];  //文件密码
int pwdLen;  //密码长度

extern void init();  //初始化
extern void updateFile();  //更新文件
extern void setPwd(int flag);  //设置密码
extern int checkPwd(int flag);  //校验密码

//学生信息增删改查
extern void addStuInfo();
extern void deleteStuInfo();
extern void alterStuInfo();
extern void findStuByID();
extern void findStuByName();
extern void findStuByScores(int flag);
extern void showAllStu();

//校验学生信息
extern int checkStuID(int stuID);
extern int checkStuName(char name[]);
extern int checkStuSex(char sex[]);
extern int checkStuAge(int age);
extern int checkStuMath(float math);
extern int checkStuCN(float cn);
extern int checkStuEN(float en);

//输入学生信息
extern void getStuID(int *id);
extern void getStuName(char name[]);
extern void getStuSex(char sex[]);
extern void getStuAge(int *age);
extern void getStuMath(float *math);
extern void getStuCN(float *cn);
extern void getStuEN(float *en);

#endif
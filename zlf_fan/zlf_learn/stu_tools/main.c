#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "common.h"
#include "stu.h"

char printMainMenu();
char printSubMain();

int main(){
	int menuID;
	init();
	while(1){
		menuID = printMainMenu();
		switch(menuID){
			case '1': addStuInfo(); break;
			case '2': deleteStuInfo(); break;
			case '3': alterStuInfo(); break;
			case '4': 
				while(1){
					menuID = printSubMain();
					switch(menuID){
						case '1': findStuByID(); break;
						case '2': findStuByName(); break;
						case '3': findStuByScores(FIND_BY_MATH); break;
						case '4': findStuByScores(FIND_BY_CN); break;
						case '5': findStuByScores(FIND_BY_EN); break;
						case '6': findStuByScores(FIND_BY_TOTAL); break;
					}
					if(menuID=='0') break;
				}
				break;
			case '5': showAllStu(); break;
			case '6': setPwd(PWD_ALTER); break;
			case '0': exit(EXIT_SUCCESS);
		}
	}
	fclose(fp);
    return 0;
}

//��ӡ���˵�
char printMainMenu(){
	char menuID;
	system("cls");
	printf("***********��ӭʹ��ѧ����Ϣ����ϵͳ����ǰ����%02d��ѧ����***********\n", header.num);
	printf("*                                                                *\n");
	printf("*                ---------------------------------               *\n");
	printf("*                |   Powered By c.biancheng.net  |               *\n");
	printf("*                ---------------------------------               *\n");
	printf("*                                                                *\n");
	printf("*                1)���ѧ����Ϣ                                  *\n");
	printf("*                2)ɾ��ѧ����Ϣ(����ѧ��)                        *\n");
	printf("*                3)�޸�ѧ����Ϣ(����ѧ��)                        *\n");
	printf("*                4)��ѯѧ����Ϣ                                  *\n");
	printf("*                5)��ʾ����ѧ����Ϣ�Լ�ͳ����Ϣ                  *\n");
	printf("*                6)�޸�����                                      *\n");
	printf("*                0)�˳����                                      *\n");
	printf("*                                                                *\n");
	printf("******************************************************************\n");
	fflush(stdin);
	menuID = getch();
	return menuID;
}

//��ӡ�Ӳ˵�
char printSubMain(){
	char menuID;
	system("cls");
	printf("*****************��ѯѧ����Ϣ����ǰ����%02d��ѧ����*****************\n", header.num);
	printf("*                                                                *\n");
	printf("*                1)����ѧ�Ų�ѯ                                  *\n");
	printf("*                2)����������ѯ                                  *\n");
	printf("*                3)������ѧ�ɼ���ѯ                              *\n");
	printf("*                4)�������ĳɼ���ѯ                              *\n");
	printf("*                5)����Ӣ��ɼ���ѯ                              *\n");
	printf("*                6)�����ܳɼ���ѯ                                *\n");
	printf("*                0)�������˵�                                    *\n");
	printf("*                                                                *\n");
	printf("******************************************************************\n");
	fflush(stdin);
	menuID = getch();
	return menuID;
}
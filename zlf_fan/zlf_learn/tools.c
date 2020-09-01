#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <stdarg.h>
#include <ctype.h>
#include "tools.h"

//��ȡ�ļ���С
long getFileSize(FILE *fp){
	long fsize;
	fpos_t fpos;  //��ǰλ��
	fgetpos(fp, &fpos);  //��ȡ��ǰλ��
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	fsetpos(fp,&fpos);  //�ָ�֮ǰ��λ��

	return fsize;
}

//��ͣ����
void pause(const char *str, ...){
	va_list vl;
	char buf[500] = {0};
	va_start(vl, str);
	vsnprintf(buf, 500, str, vl);
	va_end(vl);
	printf(buf);
	getch();
	printf("\n");
}

/**
* ��ȡ�û����������
* @param  pwd     char*  ����������ڴ���׵�ַ
* @param  pwdlen  int    �������󳤶�
**/
void getpwd(char *pwd, int pwdlen){
    char ch = 0;
    int i = 0;
    while(i<pwdlen){
        ch = getch();

        if(ch == '\r'){  //�س���������
            printf("\n");
            break;
        }
       
        if(ch=='\b' && i>0){  //����ɾ����
            i--;
            printf("\b \b");
        }else if(isprint(ch)){  //����ɴ�ӡ�ַ�
            pwd[i] = ch;
            printf("*");
            i++;
        }
    }
    pwd[i] = 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <stdarg.h>
#include <ctype.h>
#include "tools.h"

//获取文件大小
long getFileSize(FILE *fp){
	long fsize;
	fpos_t fpos;  //当前位置
	fgetpos(fp, &fpos);  //获取当前位置
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	fsetpos(fp,&fpos);  //恢复之前的位置

	return fsize;
}

//暂停程序
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
* 获取用户输入的密码
* @param  pwd     char*  保存密码的内存的首地址
* @param  pwdlen  int    密码的最大长度
**/
void getpwd(char *pwd, int pwdlen){
    char ch = 0;
    int i = 0;
    while(i<pwdlen){
        ch = getch();

        if(ch == '\r'){  //回车结束输入
            printf("\n");
            break;
        }
       
        if(ch=='\b' && i>0){  //按下删除键
            i--;
            printf("\b \b");
        }else if(isprint(ch)){  //输入可打印字符
            pwd[i] = ch;
            printf("*");
            i++;
        }
    }
    pwd[i] = 0;
}
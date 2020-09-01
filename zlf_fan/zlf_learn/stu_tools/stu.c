#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "common.h"
#include "stu.h"

//初始化
void init(){
	STU stu;
	NODE *preNode = NULL, *thisNode = NULL;
	long fileSize;
	char buffer[32], pwdMD5[33];
	int readCount, i;

	//打开文件
	if( (fp=fopen(FILENAME, "rb+")) == NULL && (fp=fopen(FILENAME, "wb+")) == NULL ){
		pause("Error on open %s file!", FILENAME);
		exit(EXIT_FAILURE);
	}
	//获取文件大小
	fileSize = getFileSize(fp);

	if(fileSize==0){  //初次使用软件时需要设置密码
		setPwd(PWD_SET);
	}else if(!checkPwd(PWD_CHECK)){  //再次使用软件时需要确认密码
		exit(EXIT_SUCCESS);
	}

	if(getFileSize(fp)>32){  //如果文件中有数据，就构建链表
		//先将数据解密并保存到临时文件
		fpTemp = tmpfile();
		fseek(fp, 32, SEEK_SET);
		while( (readCount=fread(buffer, 1, pwdLen, fp)) > 0 ){
			for(i=0; i<readCount; i++){
				buffer[i] ^= pwdCurrent[i];
			}
			fwrite(buffer, 1, readCount, fpTemp);
		}
		fflush(fpTemp);

		//再通过临时文件中的数据构建链表
		fseek(fpTemp, 0, SEEK_SET);
		while(fread(&stu, 1, sizeof(STU), fpTemp)){
			thisNode = (NODE*)malloc(sizeof(NODE));
			thisNode->data = stu;
			thisNode->next = NULL;
			if(preNode==NULL){  //链表第一个节点
				header.link = thisNode;
			}else{  //不是第一个节点
				preNode->next = thisNode;
			}
			preNode = thisNode;
			header.num++;
		}
		fclose(fpTemp);
	}else{
		header.num = 0;
		header.link = NULL;
	}
}

//设置或修改密码
void setPwd(int flag){
	char pwdOld[PWD_LEN+1], pwdNew1[PWD_LEN+1], pwdNew2[PWD_LEN+1], pwdNewMD5[33];

	if(flag == PWD_SET){
		printf("为了保证数据安全，首次使用本软件请设置密码！\n");
	}else if(flag==PWD_ALTER && !checkPwd(PWD_ALTER)){  //校验原有密码失败
		exit(EXIT_SUCCESS);
	}

	do{
		if(flag == PWD_SET){
			printf("请输入密码：");
			getpwd(pwdNew1, PWD_LEN);
			printf("请确认密码：");
			getpwd(pwdNew2, PWD_LEN);
		}else if(flag == PWD_ALTER){
			printf("请输入新密码：");
			getpwd(pwdNew1, PWD_LEN);
			printf("请确认新密码：");
			getpwd(pwdNew2, PWD_LEN);
		}

		if(strcmp(pwdNew1, pwdNew2) == 0){
			MD5((unsigned char*)pwdNew1, (unsigned char*)pwdNewMD5);
			fseek(fp, 0, SEEK_SET);
			fwrite(pwdNewMD5, 32, 1, fp);
			if(flag == PWD_SET){
				pause("提示：密码设置成功！按任意键进入软件...");
				strcpy(pwdCurrent, pwdNew1);
				pwdLen = strlen(pwdCurrent);
			}else if(flag == PWD_ALTER){
				pause("提示：密码修改成功！按任意返回...");
				strcpy(pwdCurrent, pwdNew1);
				pwdLen = strlen(pwdCurrent);
				updateFile();
			}
			break;
		}else{
			pause("两次输入密码不一致，请重新输入...");
		}
	}while(1);
}

int checkPwd(int flag){
	char pwd[PWD_LEN+1], pwdFile[33] = {0}, pwdMD5[33];
	int i /*循环控制变量*/, n /*密码错误次数*/;

	for(i=0; i<PWD_ERROR_TIMES; i++){
		if(flag == PWD_CHECK){
			printf("请输入密码：");
		}else if(flag == PWD_ALTER){
			printf("请输入旧密码：");
		}
		getpwd(pwd, PWD_LEN);
		MD5((unsigned char*)pwd, (unsigned char*)pwdMD5);
		fseek(fp, 0, SEEK_SET);
		fread(pwdFile, 32, 1, fp);  //读取文件中的密码
		if(strcmp(pwdFile, pwdMD5)==0){
			strcpy(pwdCurrent, pwd);
			pwdLen = strlen(pwdCurrent);
			break;
		}else{
			n = PWD_ERROR_TIMES - i - 1;
			if(n == 0){
				pause("提示：密码错误次数过多，程序即将退出！按任意键继续...");
				return 0;
			}else{
				pause("提示：密码输入错误，您还有%d次机会！按任意键重新输入...", n);
			}
		}
	}

	return 1;
}

//显示所有学生信息
void showAllStu(){
	STU thisStu;
	NODE *thisNode;
	float mathTotal = 0, cnTotal = 0, enTotal = 0, ageTotal = 0, oneTotal = 0, allTotal = 0;
	int manTotal = 0, womanTotal = 0, n = header.num;

	if(!header.num){
		pause("还未添加任何学生信息，按任意键返回...");
		return;
	}
	system("cls");
	printf("-----------------------------------------------------------------------\n");
	printf("  学号  |  姓名  |  性别  |  年龄  |  数学  |  语文  |  英语  | 总成绩\n");
	printf("--------+--------+--------+--------+--------+--------+--------+--------\n");

	for(thisNode=header.link; thisNode!=NULL; thisNode=thisNode->next){
		thisStu = thisNode->data;
		oneTotal = thisStu.math + thisStu.cn + thisStu.en;
		allTotal += oneTotal;
		mathTotal += thisStu.math;
		cnTotal += thisStu.cn;
		enTotal += thisStu.en;
		ageTotal += thisStu.age;
		if(strcmp(thisStu.sex, "男")){
			womanTotal++;
		}else{
			manTotal++;
		}
		printf("   %.2d   | %-6s |   %-3s  |   %-3d  | %-6.2f | %-6.2f | %-6.2f | %-6.2f\n", thisStu.id, thisStu.name, thisStu.sex, thisStu.age, thisStu.math, thisStu.cn, thisStu.en, oneTotal);
	}
	printf("--------+--------+--------+--------+--------+--------+--------+--------\n");
	printf("   --   |   --   | %2d/%-2d  | %-6.2f | %-6.2f | %-6.2f | %-6.2f | %-6.2f\n", manTotal, womanTotal, ageTotal/n, mathTotal/n, cnTotal/n, enTotal/n, allTotal/n);
	printf("-----------------------------------------------------------------------\n");
	pause("\n共有%d条学生信息，按任意键返回...", n);
}

//添加学生信息
void addStuInfo(){
	STU stu;
	int id, isStuExist = 0;
	NODE *thisNode, *pnode = (NODE*)malloc(sizeof(NODE));
	getStuID(&stu.id);
	id = stu.id;

	if(header.num == 0){  //如果链表为空，直接插入
		header.link = pnode;
		pnode->next = NULL;
	}else{  //如果链表不为空，需要遍历链表确定插入位置
		for(thisNode=header.link; thisNode!=NULL; thisNode=thisNode->next){
			if(thisNode->data.id == id){  //该学生信息存在
				free(pnode);
				isStuExist = 1;
			}

			if(thisNode==header.link && thisNode->data.id>id){  //如果需要在链表开头插入（也就是在头结点和第一个节点之间插入）
				header.link = pnode;
				pnode->next = thisNode;
				break;
			}else if(thisNode->next==NULL && thisNode->data.id<id){  //如果需要在链表末尾插入
				thisNode->next = pnode;
				pnode->next = NULL;
				break;
			}else if(thisNode->data.id<id && thisNode->next->data.id>id ){  //在链表中间插入
				pnode->next = thisNode->next;
				thisNode->next = pnode;
				break;
			}
		}
	}

	if(isStuExist){  //该学生存在
		pause("错误：该学生信息已存在，无需重复添加！按任意键返回...");
	}else{  //该学生不存在
		getStuName(stu.name);
		getStuSex(stu.sex);
		getStuAge(&stu.age);
		getStuMath(&stu.math);
		getStuCN(&stu.cn);
		getStuEN(&stu.en);
		pnode->data = stu;
		updateFile();
		header.num++;
		pause("提示：添加成功！按任意键返回...");
	}
}

//删除学生信息
void deleteStuInfo(){
	STU stu;
	NODE *thisNode, *preNode;
	getStuID(&stu.id);

	for(thisNode=header.link, preNode=NULL; thisNode!=NULL; preNode=thisNode, thisNode=thisNode->next){
		if(thisNode->data.id == stu.id){
			if(preNode == NULL){  //如果是第一个节点
				header.link = thisNode->next;
			}else{  //如果不是第一个节点
				preNode->next = thisNode->next;
			}
			free(thisNode);
			updateFile();
			header.num--;
			pause("提示：删除成功，按任意键返回...");
			return;
		}else if(thisNode->data.id > stu.id){
			break;
		}
	}

	pause("错误：该学生信息不存在，删除失败！按任意键返回...");	
}

//修改学生信息
void alterStuInfo(){
	STU stu;
	NODE *thisNode;
	getStuID(&stu.id);

	for(thisNode=header.link; thisNode!=NULL; thisNode=thisNode->next){
		if(thisNode->data.id == stu.id){
			getStuName(stu.name);
			getStuSex(stu.sex);
			getStuAge(&stu.age);
			getStuMath(&stu.math);
			getStuCN(&stu.cn);
			getStuEN(&stu.en);
			thisNode->data = stu;
			updateFile();
			pause("提示：修改成功，按任意键返回...");
			return;
		}else if(thisNode->data.id > stu.id){
			break;
		}
	}

	pause("错误：该学生信息不存在，修改失败！按任意键返回...");	
}

//根据学号查询
void findStuByID(){
	STU stu;
	NODE *thisNode, *pnode = NULL;
	float total = 0;
	getStuID(&stu.id);

	for(thisNode=header.link; thisNode!=NULL; thisNode=thisNode->next){
		if(thisNode->data.id == stu.id){
			pnode = thisNode;
			break;
		}else if(thisNode->data.id > stu.id){
			break;
		}
	}

	if(pnode==NULL){
		pause("错误：该学生信息不存在，查询失败！按任意键返回...");
	}else{
		stu = pnode->data;
		total = stu.math + stu.cn + stu.en;
		printf("-----------------------------------------------------------------------\n");
		printf("  学号  |  姓名  |  性别  |  年龄  |  数学  |  语文  |  英语  | 总成绩\n");
		printf("--------+--------+--------+--------+--------+--------+--------+--------\n");
		printf("   %.2d   | %-6s |   %-3s  |   %-3d  | %-6.2f | %-6.2f | %-6.2f | %-6.2f\n", stu.id, stu.name, stu.sex, stu.age, stu.math, stu.cn, stu.en, total);
		printf("-----------------------------------------------------------------------\n");
		pause("\n按任意键返回...");
	}
}

//根据姓名查询学生信息
void findStuByName(){
	STU stu;
	NODE *thisNode = NULL;
	int n = 0;  //匹配到几条学生记录
	float total;
	char name[20];
	getStuName(name);

	for(thisNode=header.link; thisNode!=NULL; thisNode=thisNode->next){
		stu = thisNode->data;
		if(strstr(stu.name, name)){
			n++;
			total = stu.math + stu.cn + stu.en;
			if(n==1){
				printf("-----------------------------------------------------------------------\n");
				printf("  学号  |  姓名  |  性别  |  年龄  |  数学  |  语文  |  英语  | 总成绩\n");
				printf("--------+--------+--------+--------+--------+--------+--------+--------\n");
			}
			printf("   %.2d   | %-6s |   %-3s  |   %-3d  | %-6.2f | %-6.2f | %-6.2f | %-6.2f\n", stu.id, stu.name, stu.sex, stu.age, stu.math, stu.cn, stu.en, total);
		}
	}

	if(n>0){
		printf("-----------------------------------------------------------------------\n");
		pause("\n共查询到%d条记录。按任意键返回...", n);
	}else{
		pause("错误：没有查询到相关记录！按任意键返回...");
	}
}

//根据成绩查询学生信息
void findStuByScores(int flag){
	STU stu;
	int n = 0;  //匹配到几条学生记录
	float *scores = NULL;  //当前学生的成绩
	float min = 0, max = 0;  //用户输入的最高分和最低分
	int MAX = 0;  //common.h 中指定的成绩最高分
	char *courseName = NULL;  //科目名称
	float total = 0;  //当前学生总分
	NODE *thisNode = NULL;

	if(flag == FIND_BY_MATH){
		courseName = "数学成绩";
		MAX = MAX_STU_MATH;
		scores = &stu.math;
	}else if(flag == FIND_BY_CN){
		courseName = "语文成绩";
		MAX = MAX_STU_CN;
		scores = &stu.cn;
	}else if(flag == FIND_BY_EN){
		courseName = "英语成绩";
		MAX = MAX_STU_EN;
		scores = &stu.en;
	}else if(flag == FIND_BY_TOTAL){
		courseName = "总成绩";
		MAX = MAX_STU_MATH + MAX_STU_CN + MAX_STU_EN;
		scores = &total;
	}else{
		return;
	}

	while(1){
		printf("要查询的%s的范围：", courseName);
		fflush(stdin);
		scanf("%f %f", &min, &max);
		if(min<0 || min>MAX || max<0 || max>MAX){
			pause("错误：%s的取值范围为0~%d！按任意键重新输入...", courseName, MAX);
			continue;
		}
		if(min>max){
			pause("错误：最高分要高于最低分！按任意键重新输入...");
			continue;
		}
		break;
	}

	for(thisNode=header.link; thisNode!=NULL; thisNode=thisNode->next){
		stu = thisNode->data;
		total = stu.math + stu.cn + stu.en;
		if(min <= *scores && *scores <= max){
			n++;
			if(n==1){
				printf("-----------------------------------------------------------------------\n");
				printf("  学号  |  姓名  |  性别  |  年龄  |  数学  |  语文  |  英语  | 总成绩\n");
				printf("--------+--------+--------+--------+--------+--------+--------+--------\n");
			}
			printf("   %.2d   | %-6s |   %-3s  |   %-3d  | %-6.2f | %-6.2f | %-6.2f | %-6.2f\n", stu.id, stu.name, stu.sex, stu.age, stu.math, stu.cn, stu.en, total);
		}
	}

	if(n>0){
		printf("-----------------------------------------------------------------------\n");
		pause("\n共查询到%d条记录。按任意键返回...", n);
	}else{
		pause("错误：没有查询到相关记录！按任意键返回...");
	}
}

//更新文件
void updateFile(){
	FILE *fpTemp = tmpfile();
	NODE *thisNode;
	char buffer[32], pwdMD5[33];
	int readCount, i;

	//对密码进行MD5运算
	MD5((unsigned char*)pwdCurrent, (unsigned char*)pwdMD5);

	//先将学生信息写入临时文件
	for(thisNode=header.link; thisNode!=NULL; thisNode=thisNode->next){
		fwrite(&(thisNode->data), sizeof(STU), 1, fpTemp);
	}
	fflush(fpTemp);

	//再将临时文件中的数据加密并保存到数据文件（也即FILENAME）
	fseek(fpTemp, 0, SEEK_SET);  //重置临时文件指针
	freopen(FILENAME, "wb+", fp );  //清空数据文件
	fwrite(pwdMD5, 32, 1, fp);  //先将密码写入
	while( (readCount=fread(buffer, 1, pwdLen, fpTemp)) > 0 ){
		for(i=0; i<readCount; i++){
			buffer[i] ^= pwdCurrent[i];
		}
		fwrite(buffer, 1, readCount, fp);
	}

	fflush(fp);
	fclose(fpTemp);
}

//输入学生ID
void getStuID(int *id){
	while(1){
		printf("输入学号：");
		fflush(stdin);
		scanf("%d", id);
		if(checkStuID(*id)){
			break;
		}
	}
}

//输入学生姓名
void getStuName(char name[]){
	while(1){
		printf("输入姓名：");
		fflush(stdin);
		scanf("%s", name);
		if(checkStuName(name)){
			break;
		}
	}
}

//输入学生性别
void getStuSex(char sex[]){
	while(1){
		printf("输入性别：");
		fflush(stdin);
		scanf("%s", sex);
		if(checkStuSex(sex)){
			break;
		}
	}
}

//输入学生年龄
void getStuAge(int *age){
	while(1){
		printf("输入年龄：");
		fflush(stdin);
		scanf("%d", age);
		if(checkStuAge(*age)){
			break;
		}
	}
}

//输入数学成绩
void getStuMath(float *math){
	while(1){
		printf("数学成绩：");
		fflush(stdin);
		scanf("%f", math);
		if(checkStuMath(*math)){
			break;
		}
	}
}

//输入语文成绩
void getStuCN(float *cn){
	while(1){
		printf("语文成绩：");
		fflush(stdin);
		scanf("%f", cn);
		if(checkStuCN(*cn)){
			break;
		}
	}
}

//输入英语成绩
void getStuEN(float *en){
	while(1){
		printf("英语成绩：");
		fflush(stdin);
		scanf("%f", en);
		if(checkStuEN(*en)){
			break;
		}
	}
}

//检查学生ID是否正确
int checkStuID(int stuID){
	if(stuID <= 0 || stuID > MAX_STU_ID){
		printf("错误：学号必须是大于0且小于等于%d的整数！\n", MAX_STU_ID);
		return 0;
	}
	return 1;
}

//检查学生姓名是否正确
int checkStuName(char name[]){
	if(strlen(name) > MAX_STU_NAME){
		printf("错误：名字的最大长度不超过%d个字节！\n", MAX_STU_NAME);
		return 0;
	}
	return 1;
}

//检查学生性别是否正确
int checkStuSex(char sex[]){
	if(strcmp(sex, "男") && strcmp(sex, "女")){
		printf("错误：性别只能是男或女！\n");
		return 0;
	}else{
		return 1;
	}
}

//检查学生年龄是否正确
int checkStuAge(int age){
	if(age <=0 || age > MAX_STU_AGE){
		printf("错误：年龄的取值范围为1~%d！\n", MAX_STU_AGE);
		return 0;
	}
	return 1;
}

//检查数学成绩是否正确
int checkStuMath(float math){
	if(math < 0 || math > MAX_STU_MATH){
		printf("错误：数学成绩的取值范围为0~%d！\n", MAX_STU_MATH);
		return 0;
	}
	return 1;
}

//检查语文成绩是否正确
int checkStuCN(float cn){
	if(cn < 0 || cn > MAX_STU_CN){
		printf("错误：语文成绩的取值范围为0~%d！\n", MAX_STU_CN);
		return 0;
	}
	return 1;
}

//检查英语成绩是否正确
int checkStuEN(float en){
	if(en < 0 || en > MAX_STU_EN){
		printf("错误：英语成绩的取值范围为0~%d！\n", MAX_STU_EN);
		return 0;
	}
	return 1;
}
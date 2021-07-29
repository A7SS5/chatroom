#include<stdio.h>
#include "mysql/mysql.h"
#include<string.h>
#include<stdlib.h>
MYSQL a;
void my_err(const char *error_string, int line)
{
    fprintf(stderr, "line:%d",line);
    perror(error_string);
    
}
MYSQL accept_mysql(void)
{
	MYSQL               mysql;
	
	if(NULL == mysql_init(&mysql)){
		my_err("mysql_init", __LINE__);
	}

	//初始化数据库
	if(mysql_library_init(0, NULL, NULL) != 0){
		my_err("mysql_library_init", __LINE__);
	}

	//连接数据库
	if(NULL == mysql_real_connect(&mysql, "127.0.0.1", "root", "12345","etc", 0, NULL, 0)){
		my_err("mysql_real_connect", __LINE__);
	}

	//设置中文字符集
	if(mysql_set_character_set(&mysql, "utf8") < 0){
		my_err("mysql_set_character_set", __LINE__);
	}
	
	printf("连接mysql数据库成功!\n");
	return mysql;
}
struct people_status{
	char name[50];
	int id;
};
int use_mysql_4(int id,MYSQL mysql1)
{
	char string[50];
	sprintf(string,"select uid from 用户数据 where uid=%d;",id);
	int ret;
	MYSQL mysql=mysql1;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	ret=mysql_query(&mysql,string);
	if (!ret)
	{
		result = mysql_store_result(&mysql);
		if(result){
			if ((row=mysql_fetch_row(result)))
			{
				return 1;
			}

		}
		mysql_free_result(result);
	}
	else{
		printf("query fail\n");
		return -1;
	}
	return 0;

}
int use_mysql(const char *name)
{
	char string[50];
	sprintf(string,"select uid from 用户数据 where 用户名=\"%s\"",name);
	int                 ret;
	unsigned int        num_fields;
	MYSQL               mysql = a;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	MYSQL_FIELD         *field;
	mysql_query(&mysql,"use etc");
	printf("%s\n",string);
	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
			num_fields = mysql_num_fields(result);
			if (!(row = mysql_fetch_row(result)))
			{
				return 0;
			}
			return atoi(row[0]);
					
				}
				printf("\n");
		mysql_free_result(result);
	}
	else{
		printf("query fail\n");
		return -1;
	}
	return 0;
}
int use_mysql_3(int id,MYSQL mysql1)
{
		
	char string[120];
	sprintf(string,"select uid,用户名 FROM 用户数据,friend WHERE friend.sid=%d AND 用户数据.uid=friend.rid ORDER BY uid",id);
	printf("%s\n",string);
	int                 i;
	int                 ret;
	unsigned int        num_rows;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	mysql_query(&mysql,"use etc");
	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
			num_rows = mysql_num_rows(result);
			struct people_status arry[num_rows];
				int i=0;
					while((row = mysql_fetch_row(result))){
					arry[i].id=atoi(row[0]);
					strcpy(arry[i].name,row[1]);
					printf("id:%d name:%s\n",arry[i].id,arry[i].name);
					i++;
					}
				}
				printf("\n");		
		mysql_free_result(result);
	}
	else{
		printf("query fail\n");
		return -1;
	}
	return 0;
}
int close_mysql(MYSQL mysql)
{
	mysql_close(&mysql);
	mysql_library_end();
	printf("end\n");
	return 0;
}
void getmyfriend(int id)
{
		 MYSQL a;
    a=accept_mysql();
	int ret;
    ret=use_mysql_3(id,a);
    close_mysql(a);
}
int main()
{	/*
    a=accept_mysql();
	char name[10]="admsin";
	char password[10]="12345";
    if (use_mysql(name,password)==1)
	{
		printf("登陆成功\n");
	}
	else if (use_mysql(name,password)==0)
	{
		printf("无用户信息或密码错误\n");
	}
	else printf("mysql error\n");
    close_mysql(a);
	*/
	a=accept_mysql();
//	printf("\n%d\n",use_mysql("admin"));
	int id;
	while(1)
	{
		scanf("%d",&id);
	printf("zai:%d\n",use_mysql_4(id,a));
	}
	close_mysql(a);
}
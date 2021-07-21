#include"mysqlc.h"
#include"List.h"

extern people_list_t list;
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
	if(NULL == mysql_real_connect(&mysql, "127.0.0.1", "root", "12345", "etc", 0, NULL, 0)){
		my_err("mysql_real_connect", __LINE__);
	}

	//设置中文字符集
	if(mysql_set_character_set(&mysql, "utf8") < 0){
		my_err("mysql_set_character_set", __LINE__);
	}
	
	return mysql;
}

int getstatus(int id)
{
	people_node_t* p;
	List_ForEach(list,p)
	{
		if (p->data.id==id)
		return 1;
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
int use_mysql(const char *name,const char *password,MYSQL mysql1)
{
	
	char string[50];
	sprintf(string,"select*from 用户数据 where 用户名=\"%s\"",name);
	int                 i;
	int                 ret;
	unsigned int        num_fields;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	MYSQL_FIELD         *field;

	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
			num_fields = mysql_num_fields(result);
			row = mysql_fetch_row(result);			
			if (!row)
			{
				return 0;
			}
					if(row[2]){
						if (strcmp(password,row[2])==0)
						{
							return 1;
						}
						else {
							return 0;
						}
					//	printf("%-20s", row[1]);
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
int use_mysql_3(int id,MYSQL mysql1)
{
		
	char string[120];
	sprintf(string,"select uid,用户名 FROM 用户数据,friend WHERE friend.sid=%d AND 用户数据.uid=friend.rid ORDER BY uid",id);
	int                 i;
	int                 ret;
	unsigned int        num_fields;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	MYSQL_FIELD         *field;

	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
			num_fields = mysql_num_fields(result);
			row = mysql_fetch_row(result);			
			if (!row)
			{
				return 0;
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
int use_mysql_1(const char *name,const char *password,MYSQL mysql1) //注册，先看有没有，再插入
{
	
	char string[50];
	char string1[100];
	sprintf(string,"select*from 用户数据 where 用户名=\"%s\"",name);
	sprintf(string1,"insert into 用户数据 values(NULL,\"%s\",\"%s\")",name,password);
	int                 i;
	int                 ret;
	unsigned int        num_fields;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	MYSQL_FIELD         *field;
	int ret1;
	printf("%s\n",string1);
	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
			row = mysql_fetch_row(result);			
					if (!row)
					{
						ret1=mysql_query(&mysql,string1);
						if (!ret1)
						{
							return 1;
						}
						else return 0;
					}
					else{
						return 0;
					}
				}
				printf("sa\n");		
		mysql_free_result(result);
	}
	else{
		printf("query fail\n");
		return -1;
	}
	return 0;
}
int use_mysql_2(const char *name,MYSQL mysql1)
{
	char string[50];
	sprintf(string,"select uid from 用户数据 where 用户名=\"%s\"",name);
	int                 ret;
	unsigned int        num_fields;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	MYSQL_FIELD         *field;
	mysql_query(&mysql,"use etc");
	//printf("%s\n",string);
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
int judege(const char *name,const char *password)
{
  MYSQL a;
    a=accept_mysql();
    int ret=use_mysql(name,password,a);
    close_mysql(a);
    return ret;
}
int judegeon(const char *name,const char *password)
{
  MYSQL a;
    a=accept_mysql();
    int ret=use_mysql_1(name,password,a);
    close_mysql(a);
    return ret;
}
void getmyfriend(int id)
{
		 MYSQL a;
    a=accept_mysql();
	int ret;
    ret=use_mysql_3(id,a);
    close_mysql(a);
}
int find_byname(const char*name)
{
	 MYSQL a;
    a=accept_mysql();
    int ret=use_mysql_2(name,a);
    close_mysql(a);
    return ret;
}
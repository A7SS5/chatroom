#include"mysqlc.h"
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
	sprintf(string,"select*from 学生数据 where 帐号=\"%s\"",name);
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
					if(row[1]){
						if (strcmp(password,row[1])==0)
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
    int ret=use_mysql(name,password,a);
    close_mysql(a);
    return ret;
}
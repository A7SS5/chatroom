#include"mysqlc.h"
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
char* getname(int id)
{
	people_node_t* p;
	List_ForEach(list,p)
	{
		if (p->data.id==id)
		{
		return p->data.name;
		}
	}
	return NULL;
}
int getcfd(int id) //遍历链表查找在线人的fd
{
	people_node_t* p;
	List_ForEach(list,p)
	{
		if (p->data.id==id)
		{
		return p->data.fd;
		}
	}
	return 0;
}
int getstatus(int id)  //遍历链表并查找在线状态
{
	people_node_t* p;
	List_ForEach(list,p)
	{
		if (p->data.id==id)
		{
		return 1;
		}
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
int use_mysql_5(struct work temp,MYSQL mysql1)
{
	char string[50];
	sprintf(string,"insert into requst values(0,%d,%d,1)",temp.sid,temp.rid);
	MYSQL mysql=mysql1;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	int ret;
	ret=mysql_query(&mysql,string);
	if (!ret)
	{
		return 1;
	}
	else{
		printf("query error\n");
		return -1;
	}

}
char* use_mysql_7(int id,MYSQL mysql1)
{
	char string[50];
	sprintf(string,"select 用户名 from 用户数据 where uid=%d;",id);
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

				return row[0];
			}

		}
		mysql_free_result(result);
	}
	else{
		printf("query fail\n");
		return NULL;
	}
	return NULL;

}
int use_mysql_17(int id,MYSQL mysql1)
{
		
	char string[120];
	sprintf(string,"select pid,groupmates.gid,name,power from groupmates,grouplist where pid=%d and groupmates.gid=grouplist.gid",id);
	printf("%s\n",string);
	int                 ret;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	struct work temp={'p',0,0,"","",0,""};
	
//	mysql_query(&mysql,"use etc");
	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
				int send_fd;
				send_fd=getcfd(id);
					while((row = mysql_fetch_row(result))){
					temp.rid=atoi(row[1]);
					strcpy(temp.name,row[2]);
					temp.ret=atoi(row[3]);
					send(send_fd,&temp,sizeof(temp),0);
					}
					temp.rid=0;
					send(send_fd,&temp,sizeof(temp),0);
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
	struct work temp={'c',0,0,"","",0};
	
//	mysql_query(&mysql,"use etc");
	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
			num_rows = mysql_num_rows(result);
			
				int i=0;
				int send_fd;

					while((row = mysql_fetch_row(result))){
					temp.rid=atoi(row[0]);
					strcpy(temp.name,row[1]);
					temp.ret=getstatus(temp.rid);
					send_fd=getcfd(id);
					send(send_fd,&temp,sizeof(temp),0);
					i++;
					}
					temp.rid=0;
					send(send_fd,&temp,sizeof(temp),0);
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
int use_mysql_21(int gid,int cfd,MYSQL mysql1)
{
		
	char string[120];
	sprintf(string,"select distinct pid,power,用户名 from groupmates,用户数据 where pid=uid and gid=%d;",gid);
	printf("%s\n",string);
	int                 i;
	int                 ret;
	unsigned int        num_rows;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	struct work temp={'r',0,0,"","",0};
	
//	mysql_query(&mysql,"use etc");
	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
			num_rows = mysql_num_rows(result);
					while((row = mysql_fetch_row(result))){
					temp.rid=atoi(row[0]);
					strcpy(temp.name,row[2]);
					temp.ret=atoi(row[1]);
					send(cfd,&temp,sizeof(temp),0);
					}
					temp.rid=0;
					send(cfd,&temp,sizeof(temp),0);
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
int use_mysql_24(int gid,MYSQL mysql1) //还没完，把聊天记录也一删除
{
	char string[60];
	char string1[60];
	sprintf(string,"delete from groupmates where gid=%d",gid);
	sprintf(string,"delete from grouplist where gid=%d",gid);
	printf("%s\n",string);
	MYSQL mysql=mysql1;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	if (!mysql_query(&mysql,string))
	{
		if (!mysql_query(&mysql,string1))
		{
			
			return 1;
		}
	return 0;
	}
	return 0;
}
int use_mysql_23(struct work s1,MYSQL mysql1)
{
	char string[60];
	sprintf(string,"delete from groupmates where gid=%d and pid=%d",s1.sid,s1.rid);
	printf("%s\n",string);
	MYSQL mysql=mysql1;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	int ret;
	if (!mysql_query(&mysql,string))
	{
		return 1;
	}
	return 0;
}
int use_mysql_22(struct work s1,MYSQL mysql1)
{
	char string[60];
	sprintf(string," update groupmates set power=1 where pid=%d and gid=%d",s1.rid,s1.sid);
	printf("%s\n",string);
	MYSQL mysql=mysql1;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	int ret;
	if (!mysql_query(&mysql,string))
	{
		return 1;
	}
	return 0;
}
int use_mysql_6(int id,MYSQL mysql1)
{
		
	char string[150];
	sprintf(string,"select distinct sid,rid,type,用户数据.用户名 from requst,用户数据 where rid=%d and sid=uid;",id);
	int                 ret;
	unsigned int        num_rows;
	unsigned int        num_feids;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	struct work temp={'g',0,0,"","",0};
	
//	mysql_query(&mysql,"use etc");
	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
			num_rows = mysql_num_rows(result);
			num_feids=mysql_num_fields(result);
				int send_fd;

					while((row = mysql_fetch_row(result))){
					temp.rid=atoi(row[0]);
					temp.ret=atoi(row[2]);
					strcpy(temp.name,row[3]);
					temp.sid=1;
					send_fd=getcfd(id);
					send(send_fd,&temp,sizeof(temp),0);
					}
					temp.sid=0;
					send(send_fd,&temp,sizeof(temp),0);
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
int use_mysql_18(int gid,int sid,MYSQL mysql1)
{
		
	char string[150];
	sprintf(string,"select distinct sid,用户名,type from 用户数据,grequst where gid=%d and sid=uid;",gid);
	int                 ret;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	struct work temp={'q',0,0,"","",0};
	
//	mysql_query(&mysql,"use etc");
	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
				int send_fd;
					send_fd=getcfd(sid);
					while((row = mysql_fetch_row(result))){
					temp.ret=atoi(row[2]);
					strcpy(temp.name,row[1]);
					temp.sid=atoi(row[0]);
					send(send_fd,&temp,sizeof(temp),0);
					}
					temp.sid=0;
					send(send_fd,&temp,sizeof(temp),0);
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
int use_mysql_13(int id,MYSQL mysql1)
{
		
	char string[150];
	sprintf(string,"select sid,rid,mes from smes where sid = %d or rid =%d and type=1",id,id);
	printf("%s\n",string);
	int                 ret;
	unsigned int        num_rows;
	unsigned int        num_feids;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	struct work temp={'m',0,0,"","",0};
	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
				int send_fd;

					while((row = mysql_fetch_row(result))){
					temp.sid=atoi(row[0]);
					temp.rid=atoi(row[1]);
					strcpy(temp.mes,row[2]);
					send_fd=getcfd(id);
					send(send_fd,&temp,sizeof(temp),0);
					}
					temp.sid=0;
					send(send_fd,&temp,sizeof(temp),0);
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
int use_mysql_14(int id,MYSQL mysql1)
{
		
	char string[150];
	sprintf(string,"select sid,rid,mes from smes where rid =%d and type=0",id);
	printf("%s\n",string);
	int                 ret;
	unsigned int        num_rows;
	unsigned int        num_feids;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	struct work temp={'k',0,0,"","",0};
	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
				int send_fd;

					while((row = mysql_fetch_row(result))){
					temp.sid=atoi(row[0]);
					temp.rid=atoi(row[1]);
					strcpy(temp.mes,row[2]);
					send_fd=getcfd(id);
					send(send_fd,&temp,sizeof(temp),0);
					}
					temp.sid=0;
					send(send_fd,&temp,sizeof(temp),0);
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
int use_mysql_16(struct work temp,MYSQL mysql1)
{
		
	char string[150];
	sprintf(string,"select gid from grouplist where gid =%d",temp.rid);
	char string1[150];
	sprintf(string1,"insert into grequst values(0,%d,%d,1)",temp.sid,temp.rid);
	int                 ret;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	int send_fd=getcfd(temp.sid);
	struct work test={'o',0,0,"","",0,""};
	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
				row = mysql_fetch_row(result);
					if (row)
					{
						ret=mysql_query(&mysql,string1);
							if (!ret)
							{
								test.ret=1;
							}
							else {
								printf("error:%s",string1);
							}
						
					}
					else test.ret=0;
					send(send_fd,&test,sizeof(test),0);
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
int use_mysql_10(struct work temp,MYSQL mysql1) //未测试 
{
	char string[50];
	char string1[100];
	char string2[100];
	sprintf(string,"insert into requst values(0,%d,%d,%d)",temp.sid,temp.rid,temp.ret);
	sprintf(string1,"delete from friend where sid=%d and rid=%d",temp.sid,temp.rid);
	sprintf(string2,"delete from friend where sid=%d and rid=%d",temp.rid,temp.sid);
	printf("%s\n",string);
	MYSQL mysql=mysql1;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	int ret;
	if (!mysql_query(&mysql,string))
	{
		if (!mysql_query(&mysql,string1))
		{
			if (!mysql_query(&mysql,string2))
			{
				return 1;
			}
		}
	}
	return 0;
}
int use_mysql_9(struct work temp,MYSQL mysql1)
{
	char string[60];
	sprintf(string,"delete from requst  where sid=%d and rid=%d and type=%d",temp.sid,temp.rid,temp.ret);
	printf("%s\n",string);
	MYSQL mysql=mysql1;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	int ret;
	if (!mysql_query(&mysql,string))
	{
		return 1;
	}
	return 0;
}
int use_mysql_20(struct work temp,MYSQL mysql1)
{
	char string[60];
	sprintf(string,"delete from grequst  where sid=%d and gid=%d and type=%d",temp.sid,temp.rid,temp.ret);
	printf("%s\n",string);
	MYSQL mysql=mysql1;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	int ret;
	if (!mysql_query(&mysql,string))
	{
		return 1;
	}
	return 0;
}
int use_mysql_8(struct work temp,MYSQL mysql1)
{
	char string[50];
	char string1[50];
	char string2[60];
	sprintf(string,"insert into friend values(0,%d,%d)",temp.sid,temp.rid);
	sprintf(string1,"insert into friend values(0,%d,%d)",temp.rid,temp.sid);
	sprintf(string2,"delete from requst  where sid=%d and rid=%d and type=%d",temp.sid,temp.rid,temp.ret);
	MYSQL mysql=mysql1;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	int ret;
	if (!mysql_query(&mysql,string))
	{
		if (!mysql_query(&mysql,string1))
		{
			if (!mysql_query(&mysql,string2))
			{
				return 1;
			}
		}
	}
	return 0;
}
int use_mysql_12(struct work temp,MYSQL mysql1)
{
	char string[1200];
	sprintf(string,"update smes set type = 1 where sid=%d and rid=%d and mes=\"%s\"",temp.sid,temp.rid,temp.mes);
	printf("%s\n",string);
	MYSQL mysql=mysql1;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	int ret;
	ret=mysql_query(&mysql,string);
	if (!ret)
	{
		return 1;
	}
	else{
		printf("query error\n");
		return -1;
	}

}
int use_mysql_11(struct work temp,MYSQL mysql1)
{
	char string[1200];
	sprintf(string,"insert into smes values(0,%d,%d,\"%s\",%d)",temp.sid,temp.rid,temp.mes,temp.ret);
	printf("%s\n",string);
	MYSQL mysql=mysql1;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	int ret;
	ret=mysql_query(&mysql,string);
	if (!ret)
	{
		return 1;
	}
	else{
		printf("query error\n");
		return -1;
	}

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
int use_mysql_15(struct work temp,MYSQL mysql1)
{
		
	char string[150];
	char string1[150];
	char string2[150];
	sprintf(string,"select gid from grouplist where name=\"%s\"",temp.name);
	sprintf(string1,"insert into grouplist values(0,%d,\"%s\")",temp.sid,temp.name);
	int                 ret;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_RES           *result1 = NULL;
	MYSQL_ROW           row;
	int send_fd=getcfd(temp.sid);
	struct work test={'o',0,0,"","",0};
	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
				row=mysql_fetch_row(result);
				if (!row)
				{	mysql_free_result(result);
					ret=mysql_query(&mysql,string1);
					
						if (!ret)
						{
							ret=mysql_query(&mysql,string);
							if (!ret)
							{
								printf("string:%s\n",string);
								result1=mysql_store_result(&mysql);
								if (result1)
								{
									row=mysql_fetch_row(result1);
									temp.rid=atoi(row[0]);
									sprintf(string2,"insert into groupmates values(%d,%d,2)",temp.rid,temp.sid);
									ret=mysql_query(&mysql,string2);
									if (!ret)
									{
										test.ret=1;
										send(send_fd,&test,sizeof(test),0);
									}
									else printf("error %s\n",string2);
								}
									mysql_free_result(result1);
								
							}
						}
						else {
							printf("qurry fail%s",string1);
						}
					
				}
				else{	mysql_free_result(result);
					test.ret=0;
					send(send_fd,&test,sizeof(test),0);
				}
				}
				printf("\n");		
	}
	else{
		printf("query fail %s\n",string);
		return -1;
	}
	return 0;
}
int use_mysql_19(struct work temp,MYSQL mysql1)
{
	char string[50];
	char string2[60];
	sprintf(string,"insert into groupmates values(%d,%d,0)",temp.rid,temp.sid);
	sprintf(string2,"delete from grequst  where sid=%d and gid=%d and type=%d",temp.sid,temp.rid,temp.ret);
	MYSQL mysql=mysql1;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	int ret;
	if (!mysql_query(&mysql,string))
	{
			if (!mysql_query(&mysql,string2))
			{
				return 1;
			}
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
void getmyfriend(int id) //查找所有好友并返回
{
	MYSQL a;
    a=accept_mysql();
	int ret;
    ret=use_mysql_3(id,a);
    close_mysql(a);
}
void getmyrequst(int id)
{
	MYSQL a;
    a=accept_mysql();
	int ret;
    ret=use_mysql_6(id,a);
    close_mysql(a);
}
void getmygrequst(int gid,int sid)
{
	MYSQL a;
    a=accept_mysql();
	int ret;
    ret=use_mysql_18(gid,sid,a);
    close_mysql(a);
}
char *yourname(int id)
{
	MYSQL a;
	char *c;
    a=accept_mysql();
    c=use_mysql_7(id,a);
    close_mysql(a);
	return c;
}
int find_byname(const char*name) //通过用户名查找id
{
	 MYSQL a;
    a=accept_mysql();
    int ret=use_mysql_2(name,a);
    close_mysql(a);
    return ret;
}
void add_friends(struct work temp) //储存加好友信息
{
	MYSQL a;
    a=accept_mysql();
	int ret;
    ret=use_mysql_5(temp,a);
	if (getstatus(temp.rid))
	{
		int cfd=getcfd(temp.rid);
		printf("cfd:%d",cfd);
		strcpy(temp.name,getname(temp.sid));
		send(cfd,&temp,sizeof(temp),0);
	}
    close_mysql(a);
}
void agree(struct work temp)
{
	MYSQL a;
	int ret;
    a=accept_mysql();
    use_mysql_8(temp,a);
    close_mysql(a);
}
void disagree(struct work temp)
{
	int ret;
		MYSQL a;
    a=accept_mysql();
    ret=use_mysql_9(temp,a);
    close_mysql(a);
}
void agreeg(struct work temp)
{
	MYSQL a;
	int ret;
    a=accept_mysql();
    use_mysql_19(temp,a);
    close_mysql(a);
}
void disagreeg(struct work temp)
{
	int ret;
		MYSQL a;
    a=accept_mysql();
    ret=use_mysql_20(temp,a);
    close_mysql(a);
}
void delete_friend(struct work temp)
{
		MYSQL a;
    a=accept_mysql();
    use_mysql_10(temp,a);
    close_mysql(a);
}
void ssend_mes(struct work temp)
{
	MYSQL a;
    a=accept_mysql();
	int ret;
	if (getstatus(temp.rid))
	{
		int cfd=getcfd(temp.rid);
		send(cfd,&temp,sizeof(temp),0);
	}
	ret=use_mysql_11(temp,a);
    close_mysql(a);
}
void read_mes(struct work temp)
{
	MYSQL a;
    a=accept_mysql();
    use_mysql_12(temp,a);
    close_mysql(a);
}
void sendallmes(int id)

 {
	MYSQL a;
    a=accept_mysql();
    use_mysql_13(id,a);
    close_mysql(a);
 }
void getallnmes(int id)
{
	MYSQL a;
    a=accept_mysql();
    use_mysql_14(id,a);
    close_mysql(a);
}
void createg(struct work temp)
{
	MYSQL a;
    a=accept_mysql();
    use_mysql_15(temp,a);
    close_mysql(a);
}
void joingroups(struct work temp)
{
		MYSQL a;
    a=accept_mysql();
    use_mysql_16(temp,a);
    close_mysql(a);
}
void getmygroup(int id)
{
	MYSQL a;
    a=accept_mysql();
    use_mysql_17(id,a);
    close_mysql(a);
}
void getmates(int gid,int cfd)
{
	MYSQL a;
    a=accept_mysql();
	int ret;
    ret=use_mysql_21(gid,cfd,a);
    close_mysql(a);
}
void setadmin(struct work s1,int cfd)
{
	MYSQL a;
    a=accept_mysql();
    use_mysql_22(s1,a);
    close_mysql(a);
}
void delmate(struct work s1,int cfd)
{
	MYSQL a;
    a=accept_mysql();
    use_mysql_23(s1,a);
    close_mysql(a);
}
void killgroup(int gid)
{
	MYSQL a;
    a=accept_mysql();
    use_mysql_24(gid,a);
    close_mysql(a);
}
int ishe(int id,struct s1 *s) //判断是否存在id
{
	MYSQL a;
    a=accept_mysql();
    int ret=use_mysql_4(id,a);
    close_mysql(a);
	struct work ss;
	ss.tye='d';
	ss.ret=ret;
	send(s->conn_fd,&ss,sizeof(ss),0);
    return ret;
}
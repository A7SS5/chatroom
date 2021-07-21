#ifndef MYSQLC_H_
#define MYSQLC_H_
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <mysql/mysql.h>
struct s1 {
    int epfd,conn_fd;
};
struct work {
    char tye;
    int sid;
    char name[20];
    char password[20];
    int ret;
};
typedef struct {
    int id;
    char name[20];
    int fd;
}peple;
typedef struct people_node
{
    peple data;
    struct people_node*prev;
    struct people_node*next;
}people_node_t, *people_list_t;
void my_err(const char *error_string, int line);
MYSQL accept_mysql(void);
int close_mysql(MYSQL mysql);
int use_mysql(const char *name,const char *password,MYSQL mysql1);
int use_mysql_2(const char *name,MYSQL mysql1);
int use_mysql_1(const char *name,const char *password,MYSQL mysql1);
int judege(const char *name,const char *password);
int judegeon(const char *name,const char *password);
void getmyfriend(int id);
int getstatus(int id);
int find_byname(const char*name);
#endif
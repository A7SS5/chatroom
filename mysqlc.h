#ifndef MYSQLC_H_
#define MYSQLC_H_

#include <netinet/in.h>   
#include <arpa/inet.h>
#include <string.h>  
#include <stdio.h>
#include <stdlib.h>
#include"List.h"
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <pthread.h>

#include <mysql/mysql.h>
struct s1 {
    int epfd,conn_fd;
};
struct work {
    char tye;
    int sid;
    int rid;
    char name[20];
    char password[20];
    int ret;
    char mes[1000];
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
int use_mysql_3(int id,MYSQL mysql1);
int use_mysql_6(int id,MYSQL mysql1);
char* use_mysql_7(int id,MYSQL mysql1);
int use_mysql_4(int id,MYSQL mysql1);
int use_mysql_1(const char *name,const char *password,MYSQL mysql1);
int judege(const char *name,const char *password);
int judegeon(const char *name,const char *password);
void getmyrequst(int id);
void getmyfriend(int id);
char *yourname(int id);
int ishe(int id,struct s1 *s);
int getstatus(int id);
int getcfd(int id);
int find_byname(const char*name);
void add_friends(struct work temp);
#endif
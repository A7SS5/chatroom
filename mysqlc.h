#ifndef MYSQLC_H_
#define MYSQLC_H_

#include <netinet/in.h>   
#include <arpa/inet.h>
#include <string.h>  
#include <stdio.h>
#include <stdlib.h>
#include"List.h"
#include<sys/sendfile.h>
#include<sys/stat.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/signal.h>
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
int use_mysql_10(struct work temp,MYSQL mysql1);
char* use_mysql_7(int id,MYSQL mysql1);
int use_mysql_18(int gid,int sid,MYSQL mysql1);
int use_mysql_4(int id,MYSQL mysql1);
int use_mysql_21(int gid,int cfd,MYSQL mysql1);
int use_mysql_1(const char *name,const char *password,MYSQL mysql1);
int judege(const char *name,const char *password);
int judegeon(const char *name,const char *password);
void getmyrequst(int id);
void getmyfriend(int id);
char *yourname(int id);
int ishe(int id,struct s1 *s);
void getallngmes(int gid,int sid,int cfd);
int getstatus(int id);
int getcfd(int id);
int find_byname(const char*name);
void add_friends(struct work temp);
void disagree(struct work temp);
void agree(struct work temp);
void agreeg(struct work temp);
void disagreeg(struct work temp);
void delete_friend(struct work temp);
void ssend_mes(struct work temp);
void read_mes(struct work temp);
void read_gmes(struct work s1);
void getallnmes(int id);
void getallgmes(int gid,int sid,int cfd);
void getmates(int gid,int cfd);
void createg(struct work temp);
void sendallmes(int id);
void delmate(struct work s1,int cfd);
void setadmin(struct work s1,int cfd);
void joingroups(struct work temp);
void getmygroup(int id);
char* genRandomString(int length);
void savefile(struct work s1,char *filename);
int use_mysql_23(struct work s1,MYSQL mysql1);
void killgroup(int gid);
void delete_file(struct work s1);
void send_file(struct work s1);
void sendfilelist(struct work s1);
int use_mysql_24(int gid,MYSQL mysql1);
void getmygrequst(int gid,int sid);
void gsend_mes(struct work s1);
ssize_t						/* Read "n" bytes from a descriptor. */
readn(int fd, void *vptr, size_t n);
#endif
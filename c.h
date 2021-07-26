#ifndef _C_H
#define _C_H


#include "List.h"
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <termio.h>
#include <stdio.h>                                                                                                                  
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <strings.h>
#include <string.h>

typedef struct {
    int gid;
    char name[30];
    int power;
}group;
typedef struct group_node{
group data;
struct group_node*prev;
struct group_node*next;
}group_node_t,*group_list_t;
typedef struct {
    int xu;
    int sid;
    char name[20];
    int type;
}yan;
typedef struct {
int sid;
int rid;
char mes[1000];
}mes;
typedef struct mes_node{
    mes data;
    struct mes_node*prev;
    struct mes_node*next;
}mes_node_t,*mes_list_t;
typedef struct yan_node
{
    yan data;
    struct yan_node*prev;
    struct yan_node*next;
}yan_node_t, *yan_list_t;

typedef struct {
    int id;
    char name[20];
    int status;
}peple;
typedef struct people_node
{
    peple data;
    struct people_node*prev;
    struct people_node*next;
}people_node_t, *people_list_t;
struct work {
    char tye;
    int sid;
    int rid;
    char name[20];
    char password[20];
    int ret;
    char mes[1000];
};
int getch();
void *ralt(void* temp);
int ismyfriend(int id);
void yanzheng(int cfd);
void yanzhengg(int cfd,int gid);
void fetchallfriend(int cfd);
void fetchallmes(int cfd);
void delete_friend(int cfd);
void getgroupmates(int gid,int cfd);
void add_friend(int cfd);
void chatwithf(int cfd);
void getrequst(int cfd);
int isonline(int fd);
int logon(struct work temp,int cfd);
int login(struct work temp,int cfd);
int SysLogin(int efd);  // SL界面
int SysLogon(int efd);
void managefriend(int cfd);
void creategroup(int cfd);
void joingroup(int cfd);
void getgroup(int cfd);
void managegroup(int cfd);
void owner(group_node_t* temp,int cfd);
void admin(group_node_t* temp,int cfd);
void dog(group_node_t* temp,int cfd);
void getgrequst(group_node_t* temp,int cfd);
#endif
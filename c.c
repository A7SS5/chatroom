#include <stdio.h>                                                                                                                  
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <strings.h>
#include <string.h>
#include "List.h"
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <termio.h>
 int myid;
int getch()
{
   int c = 0;
   struct termios org_opts, new_opts;
   int res = 0;
   //-----  store old settings -----------
   res = tcgetattr(STDIN_FILENO, &org_opts);
   assert(res == 0);
   //---- set new terminal parms --------
   memcpy(&new_opts, &org_opts, sizeof(new_opts));
   new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
   tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
   c = getchar();
   //------  restore old settings ---------
   res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
   assert(res == 0);
   return c;
}

#define SERVER_ADDR     "127.0.0.1"
#define SERVER_PORT     9000
 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


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
people_list_t list=NULL;
struct work {
    char tye;
    int sid;
    int rid;
    char name[20];
    char password[20];
    int ret;
    
};
void *ralt(void* temp)
{
    int i=0;
    int cfd=*(int *)temp;
    struct work s1;
    people_node_t *new=NULL;
    while(1)
    {
        recv(cfd,&s1,sizeof(struct work),0);
        switch (s1.tye)
        {
        case 'c':
            if (s1.rid==0)
            {
                i=0;
                pthread_mutex_unlock(&mutex);
                printf("runlocked\n");
                break;
            }
                if (i==0)
                pthread_mutex_lock(&mutex);
                i++;
                printf("locked\n");
                new=(people_node_t*)malloc(sizeof(people_node_t));
                new->data.id=s1.rid;
                strcpy(new->data.name,s1.name);
                new->data.status=s1.ret;
  //              printf("%-20d%-20s%-20d\n",s1.rid,s1.name,s1.ret);
                List_AddTail(list,new);
                
        break;
        default:
            break;
        }
    }
}
void fetchallfriend(int cfd)
{
    struct work temp={'c',0,0,"","",0};
    temp.sid=myid;
    send(cfd,&temp,sizeof(struct work),0);
    printf("I'm waiting for data now\n");
    sleep(3);
    pthread_mutex_lock(&mutex);
    people_node_t *p;
    printf("i got locked");
    printf("%-20s%-20s%-20s\n","id","用户名","在线状态");
    List_ForEach(list,p)
    {
          printf("%-20d%-20s",p->data.id,p->data.name);
          if (p->data.status==1)
          printf("%-20s","在线");
          else printf("%-20s","不在线");
          printf("\n");
    }
    pthread_mutex_unlock(&mutex);
    printf("i'm unlocked now\n");
    
}
int logon(struct work temp,int cfd)
{
    struct work s1;
    temp.tye='b';
    send(cfd,&temp,sizeof(temp),0);
    recv(cfd,&s1,sizeof(s1),0);
    printf("ret :%d",s1.ret);
    if (s1.ret==0)
    return 0;
    else if (s1.ret==1)
    return 1;
}
int login(struct work temp,int cfd)
{
    struct work s1;
    temp.tye='s';
    send(cfd,&temp,sizeof(temp),0);
    recv(cfd,&s1,sizeof(s1),0);
    printf("ret :%d",s1.ret);
    if (s1.ret==0)
    return 0;
    else if (s1.ret==1)
    {
    myid=s1.sid;
    return 1;
    }
    else if (s1.ret==-1)
    return -1;
}
int SysLogin(int efd)  // SL界面
{
    struct work test={'a',0,0,"","",0};
    int i = 0;
    int j = 0;
    char n;
    char c;
    char usrname[30];
    char password[30];
    while (i <= 3)
    {
        printf("                                    登录界面\n");
    add_usr:
        printf("                              用户名:");
        j = 0;
        while ((n = getchar()) != '\n')
        {
            if (n == ' ' || j >= 29)
            {
                while ((n = getchar()) != '\n');
                
                printf("用户名不合规\n");
                j = 0;
                goto add_usr;
            }
            usrname[j++] = n;
        }
        if (n == '\n' && j == 0)
        {
            printf("用户名不为空\n");
            j = 0;
            goto add_usr;
        }
        usrname[j] = '\0';

    add_pass_1:
        printf("                              密码:");
        j = 0;
        while ((n = getch()) != '\n')
        {
            printf("*");
            if (n == ' ' || j >= 29)
            {
                while ((n = getch()) != '\n')
                    ;
                printf("\n密码不合规\n");
                j = 0;
                goto add_pass_1;
            }
            password[j++] = n;
        }
        if (n == '\n' && j == 0)
        {
            printf("\n密码不为空\n");
            j = 0;
            goto add_pass_1;
        }
        password[j] = '\0';
        i++;
        printf("\n");
        strcpy(test.name,usrname);
        strcpy(test.password,password);
        int ccc;
        if ((ccc=login(test,efd))==0)
        {
            printf("用户不存在或密码错误\n");
            printf("输入'0'来返回上层界面或'1'来继续尝试登陆\n");
            syc:
            fflush(stdin);
            printf("you choice:");
            scanf("%c",&c);
            getchar();
            if (c=='0')
            {
                return 0;
            }
            else if (c=='1')
           {}
           else {
               printf("%c不是一个合法选项请重新输入\n",c);
               goto syc;
           }
            if (i == 3)
            {
                printf("错误次数过多\n");
                return 0;
            }
        }
        else if (ccc==-1)
        {
            printf("该用户已经登陆！\n");
            return 0;
        }
        else
        {
            printf("Welcome!id:%d %s\n", myid,usrname);
            break;
        }
    }
        return 1;
}
int SysLogon(int efd)
{
        struct work test={'b',0,0,"","",0};
    int j = 0;
    char n;
    char c;
    char usrname[30];
    char password[30];
    while (1)
    {
        printf("                                    注册界面\n");
    add_usr:
        printf("                              用户名:");
        j = 0;
        while ((n = getchar()) != '\n')
        {
            if (n == ' ' || j >= 29)
            {
                while ((n = getchar()) != '\n');
                
                printf("用户名不合规\n");
                j = 0;
                goto add_usr;
            }
            usrname[j++] = n;
        }
        if (n == '\n' && j == 0)
        {
            printf("用户名不为空\n");
            j = 0;
            goto add_usr;
        }
        usrname[j] = '\0';

    add_pass_1:
        printf("                              密码:");
        j = 0;
        while ((n = getch()) != '\n')
        {
            printf("*");
            if (n == ' ' || j >= 29)
            {
                while ((n = getch()) != '\n')
                    ;
                printf("\n密码不合规\n");
                j = 0;
                goto add_pass_1;
            }
            password[j++] = n;
        }
        if (n == '\n' && j == 0)
        {
            printf("\n密码不为空\n");
            j = 0;
            goto add_pass_1;
        }
        password[j] = '\0';
        printf("\n");
        strcpy(test.name,usrname);
        strcpy(test.password,password);
        if (logon(test,efd) == 0)
        {
            printf("用户已存在或密码错误\n");
            printf("输入'0'来返回上层界面或'1'来继续尝试注册\n");
            syc:
            fflush(stdin);
            printf("you choice:");
            scanf("%c",&c);
            getchar();
            if (c=='0')
            {
                return 0;
            }
            else if (c=='1')
           {}
           else {
               printf("%c不是一个合法选项请重新输入\n",c);
               goto syc;
           }
        }
        else
        {
            printf("注册成功,请返回主页面登陆\n");
            break;
        }
    }
        return 1;
}
int main()
{
    List_Init(list,struct people_node);
    int cfd = 0;
    struct sockaddr_in saddr;
    char buf[BUFSIZ] = {0};
    int i = 0,n = 0;
 
    cfd = socket(AF_INET,SOCK_STREAM,0);
 
    bzero(&saddr,sizeof(saddr));
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET,SERVER_ADDR,&saddr.sin_addr.s_addr);
    saddr.sin_port = htons(SERVER_PORT);
 
    while ((connect(cfd,(struct sockaddr*)&saddr,sizeof(saddr)))!=0)
    {
        printf ("服务器未响应...........\n");
        sleep(3);
    }
    int t=5;
    struct work test={'a',0,0,"","",0};
    char op;
    int ret;
    printf("欢迎使用大鹏聊天室\n");
    while(1)
    {
        system("clear");
        ret=0;
        printf("输入'1'来登陆一个已注册帐号\n");
        printf("输入'2'来注册一个帐号\n");
        printf("输入'3'退出本聊天室\n");
        fflush(stdin);
        scanf("%c",&op);
        while(getchar()!='\n');
        switch(op){
        case '1':
        ret=SysLogin(cfd);
        break;
        case '2':
        SysLogon(cfd);
        break;
        case '3':
        close(cfd);
        return 0;
        break;
        default:
        printf("%c不是一个合法选项\n",op);
        }
        if (ret==1)
        {
            break;
        }
    }
    pthread_t pid1;
    int status;
    pthread_create(&pid1,NULL,ralt,&cfd);
    while(1)
    {
     //   system("clear");
        printf("欢迎使用大鹏聊天室\n");
        printf("输入'1'来查看好友列表(状态)\n");
        printf("输入'2'来开启一个私聊(输入id号)\n");
        printf("输入'3'来查看消息记录\n");
        printf("输入'4'来管理好友\n");
        printf("输入'5'来退出\n");
        
        fflush(stdin);
        scanf("%c",&op);
        while(getchar()!='\n');
        switch (op)
        {
        case '1':
            fetchallfriend(cfd);
        break;
        case '2':
        break;
        case '3':
        break;
        case '4':
        break;
        case '5':
        break;
        default:
            printf("不是一个合法的选择,请重新输入\n");
            break;
        }
    }
    close(cfd);
 
    return 0;
}
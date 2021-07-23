#include "c.h"

 int myid;
 int siliao=0;
 int allcansee=-1;
#define SERVER_ADDR     "127.0.0.1"
#define SERVER_PORT     9000
 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
yan_list_t list1=NULL;
people_list_t list=NULL;
int main()
{
    List_Init(list1,struct yan_node);
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
    struct work temp={'c',0,0,"","",0};
    temp.sid=myid;
    send(cfd,&temp,sizeof(struct work),0);
    temp.tye='g';
    send(cfd,&temp,sizeof(struct work),0);
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
        chatwithf(cfd);
        break;
        case '3':
        fetchallmes(cfd);
        break;
        case '4':
        managefriend(cfd);
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

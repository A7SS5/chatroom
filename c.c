#include "c.h"

 int myid;
 int siliao=0;
 int qunliao=0;
 int allcansee=-1;
#define SERVER_ADDR     "192.168.30.121"
#define SERVER_PORT     9000
 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex5 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex6 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex7 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex8 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex9 = PTHREAD_MUTEX_INITIALIZER;
yan_list_t list1=NULL;
people_list_t list=NULL;
people_list_t glist=NULL;
mes_list_t mes1=NULL;
mes_list_t mes2=NULL;
mes_list_t gmes1=NULL;
mes_list_t gmes2=NULL;
file_list_t flist=NULL;
group_list_t group1=NULL;
yan_list_t gyan=NULL;
int main()
{
     signal(SIGPIPE,SIG_IGN);
    List_Init(list1,struct yan_node);
    List_Init(flist,struct file_node);
    List_Init(gyan,struct yan_node);
    List_Init(list,struct people_node);
    List_Init(glist,struct people_node);
    List_Init(mes1,struct mes_node);
    List_Init(mes2,struct mes_node);
    List_Init(gmes1,struct mes_node);
    List_Init(gmes2,struct mes_node);
    List_Init(group1,struct group_node);
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
        system("clear");
    }
    int t=5;
    struct work test={'a',0,0,"","",0};
    char op;
    int ret;
    while(1)
    {
        system("clear");
        ret=0;
        printf("============================ 欢迎使用聊天室 ============================\n");
        printf("                        输入'1'来登陆一个已注册帐号\n");
        printf("                        输入'2'来注册一个帐号\n");
        printf("                        输入'3'退出本聊天室\n");
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
    int simple=0;
    pthread_create(&pid1,NULL,ralt,&cfd);
    struct work temp={'c',0,0,"","",0};
    temp.sid=myid;
    send(cfd,&temp,sizeof(struct work),0);
    temp.tye='g';
    send(cfd,&temp,sizeof(struct work),0);
    temp.tye='m';
     send(cfd,&temp,sizeof(struct work),0);
     temp.tye='n';
     send(cfd,&temp,sizeof(struct work),0);
     temp.tye='5';
     send(cfd,&temp,sizeof(struct work),0);
     temp.tye='q';
    send(cfd,&temp,sizeof(struct work),0);
    sleep(2);
    while(1)
    {
        system("clear");
        printf("============================ 欢迎使用聊天室 ============================\n");
        printf("ID:%d\n",myid);
        printf("                        输入'a'来查看好友列表(状态)\n");
        printf("                        输入'b'来开启一个私聊(输入id号)\n");
        printf("                        输入'c'来查看消息记录\n");
        printf("                        输入'd'来管理好友\n");
        printf("                        输入'e'来创建一个群聊\n");
        printf("                        输入'f'来加入一个群聊\n");
        printf("                        输入'g'来查看已加入的群组\n");
        printf("                        输入'h'来进行群组管理\n");
        printf("                        输入'i'来进入一个群聊\n");
        printf("                        输入'j'来查看群组历史记录\n");
        printf("                        输入'k'来传输文件\n");
        printf("                        输入'z'来退出\n");
        
        fflush(stdin);
        scanf("%c",&op);
        while(getchar()!='\n');
        switch (op)
        {
        case 'a':
        fetchallfriend(cfd);
        break;
        case 'b':
        chatwithf(cfd);
        break;
        case 'c':
        fetchallmes(cfd);
        break;
        case 'd':
        managefriend(cfd);
        break;
        case 'e':
        creategroup(cfd);
        break;
        case 'f':
        joingroup(cfd);
        break;
        case 'g':
        getgroup(cfd);
        break;
        case 'h':
        managegroup(cfd);
        break;
        case 'i':
        chatwithg(cfd);
        break;
        case 'j':
        grouphistory(cfd);
        break;
        case 'k':
        transfile(cfd);
        break;
        case 'z':
        simple=1;
        break;
        default:
            printf("不是一个合法的选择,请重新输入\n");
            printf("输入回车键继续\n");
            break;
        }
        if (simple==1)
        {
            break;
        }
    }
    close(cfd);
 
    return 0;
}

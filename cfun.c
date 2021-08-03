#include "c.h"
extern int myid;
extern int allcansee;
extern int siliao;
extern int qunliao;
extern yan_list_t list1;
extern people_list_t list;
extern people_list_t glist;
extern file_list_t flist;
extern mes_list_t mes1;
extern mes_list_t mes2;
extern mes_list_t gmes1;
extern mes_list_t gmes2;
extern group_list_t group1;
extern yan_list_t gyan;

extern pthread_mutex_t mutex;
extern pthread_mutex_t mutex1;
extern pthread_mutex_t mutex2;
extern pthread_mutex_t mutex3;
extern pthread_mutex_t mutex4;
extern pthread_mutex_t mutex5;
extern pthread_mutex_t mutex6;
extern pthread_mutex_t mutex7;
extern pthread_mutex_t mutex8;
extern pthread_mutex_t mutex9;
void getgroupmates(int gid,int cfd)
{
      char a;
     int simple=0;
    while(1)
    {
        system("clear");
        pthread_mutex_lock(&mutex6);
        people_node_t *p;
        printf("%-20s%-20s%-20s\n","id","用户名","身份");
        List_ForEach(glist,p)
        {
            printf("%-20d%-20s",p->data.id,p->data.name);
            if (p->data.status==2)
            printf("%-20s\n","群主");
            else if (p->data.status==1)
            printf("%-20s\n","管理员");
            else printf("%-20s\n","群成员");
            
        }
        pthread_mutex_unlock(&mutex6);
       
        printf("输入'1'来刷新状态\n");
        printf("输入'2'来退出查看\n");
        getgroupmates1:
        fflush(stdin);
        scanf("%c",&a);
        while(getchar()!='\n');
        switch(a)
        {
            case '1':
            List_Free(glist,people_node_t);
            struct work temp={'v',0,0,"","",0};
            temp.sid=gid;
            send(cfd,&temp,sizeof(struct work),0);
            printf("I'm waiting for data now\n");
            sleep(2);
            break;
            case '2':
            simple=1;
            break;
            default:
            printf("不是一个合法选项,请重新输入\n");
           goto getgroupmates1;
            break;
        }
        if (simple==1)
        break;
    }
    
}
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
void *ralt(void* temp)
{
    int i=0;
    int j=0;
    int k=0;
    int l=0;
    int m=0;
    int out;
    int n=0;
    int o=0;
    int p=0;
    int v=0;
    int q=0;
    int cfd=*(int *)temp;
    struct work s1;
    people_node_t *new=NULL;
    yan_node_t *old=NULL;
    file_node_t *file1=NULL;
    mes_node_t *nmes=NULL;
    mes_node_t *lmes=NULL;
    group_node_t *g=NULL;
    while(1)
    {
        recv(cfd,&s1,sizeof(struct work),0);
        switch (s1.tye)
        {
        case 'c':
            if (s1.rid==0)
            {
                i=0;
                allcansee=1;
                pthread_mutex_unlock(&mutex);
                break;
            }
                if (i==0)
                pthread_mutex_lock(&mutex);
                i++;
                new=(people_node_t*)malloc(sizeof(people_node_t));
                new->data.id=s1.rid;
                strcpy(new->data.name,s1.name);
                new->data.status=s1.ret;
                
  //              printf("%-20d%-20s%-20d\n",s1.rid,s1.name,s1.ret);
                List_AddTail(list,new);                
        break;
        case 'd':
            if (s1.ret==0)
            {
                allcansee=0;
            }
            else allcansee=1;
            break;
        case 'f': //'f'在后期考虑丢弃,因为此功能没啥用,验证消息不用单独来接
                old=(yan_node_t*)malloc(sizeof(yan_node_t));
                old->data.sid=s1.sid;
                strcpy(old->data.name,s1.name);
                old->data.type=1;
                old->data.xu=0;
                printf("您有一条新的验证消息\n");
 //               printf("%-20d%-20s%-20d\n",s1.sid,s1.name,1);
                List_AddTail(list1,old);      
            break;
        case 'g':
            if (s1.sid==0)
            {
                printf("你有%d条验证消息等待处理\n",j);
                j=0;
                pthread_mutex_unlock(&mutex1);
                break;
            }
                if (j==0)
                pthread_mutex_lock(&mutex1);
                j++;
                old=(yan_node_t*)malloc(sizeof(yan_node_t));
                old->data.xu=j;
                old->data.sid=s1.rid;
                strcpy(old->data.name,s1.name);
                old->data.type=s1.ret;
  //              printf("%-20d%-20s%-20d\n",s1.rid,s1.name,s1.ret);
                List_AddTail(list1,old);                
        break;
        case 'k':
        if (siliao!=0)
        {
        
            if (s1.sid==siliao)
            {
                printf("\n                                        %s:%d\n","id:",s1.sid);
                printf("                                        %-45s\n",s1.mes);
                   nmes=(mes_node_t*)malloc(sizeof(mes_node_t));
                nmes->data.sid=s1.sid;
                strcpy(nmes->data.mes,s1.mes);  
                List_AddTail(mes2,nmes);
                 s1.tye='l';

            send(cfd,&s1,sizeof(s1),0);
            break;
            }
            else printf("id:%d给您发了一条新的消息",s1.sid);
            
        }
        if (s1.sid==0) //发送者id不可能为0,表示结束并放开锁
            {
                k=0;
                sleep(1);
                printf("您有%d条未读消息\n",k);
                pthread_mutex_unlock(&mutex2);
                break;
            }
                if (k==0)
                pthread_mutex_lock(&mutex2);
                k++;
                nmes=(mes_node_t*)malloc(sizeof(mes_node_t));
                nmes->data.sid=s1.sid;
                strcpy(nmes->data.mes,s1.mes);  
                printf("id:%d给您发了一条新的消息\n",s1.sid);         
  //              printf("%-20d%-20s%-20d\n",s1.rid,s1.name,s1.ret);
                List_AddTail(mes1,nmes);                
        break;
        case 'm':
        if (s1.sid==0) //发送者id不可能为0,表示结束并放开锁
            {
                l=0;
                pthread_mutex_unlock(&mutex3);
                break;
            }
                if (l==0)
                pthread_mutex_lock(&mutex3);
                l++;
                lmes=(mes_node_t*)malloc(sizeof(mes_node_t));
                lmes->data.sid=s1.sid;
                lmes->data.rid=s1.rid;
                strcpy(lmes->data.mes,s1.mes);
                List_AddTail(mes2,lmes); 
        break;
        case 'o':
         if (s1.ret==0)
            {
                allcansee=0;
            }
            else allcansee=1;
            break;
        case 'p':
        if (s1.rid==0) //发送者id不可能为0,表示结束并放开锁
        {
            m=0;
            pthread_mutex_unlock(&mutex4);
            break;
        }
        if (m==0)
        pthread_mutex_lock(&mutex4);
        m++;
        g=(struct group_node*)malloc(sizeof(group_node_t));
        g->data.gid=s1.rid;
        strcpy(g->data.name,s1.name);
        g->data.power=s1.ret;
         List_AddTail(group1,g); 
        break;
        case 'q':
            if (s1.sid==0)
                {
                    n=0;
                    pthread_mutex_unlock(&mutex5);
                    break;
                }
                    if (n==0)
                    pthread_mutex_lock(&mutex5);
                    n++;
                    old=(yan_node_t*)malloc(sizeof(yan_node_t));
                    old->data.xu=n;
                    old->data.sid=s1.sid;
                    strcpy(old->data.name,s1.name);
                    old->data.type=s1.ret;
    //              printf("%-20d%-20s%-20d\n",s1.rid,s1.name,s1.ret);
                    List_AddTail(gyan,old);                
        break;
        case 'r':
         if (s1.rid==0)
            {
                o=0;
                pthread_mutex_unlock(&mutex6);
                break;
            }
                if (o==0)
                pthread_mutex_lock(&mutex6);
                o++;
                new=(people_node_t*)malloc(sizeof(people_node_t));
                new->data.id=s1.rid;
                strcpy(new->data.name,s1.name);
                new->data.status=s1.ret;
                
  //              printf("%-20d%-20s%-20d\n",s1.rid,s1.name,s1.ret);
                List_AddTail(glist,new);     
        break;
        case 't':
             if (qunliao!=0)
                {
                    if (s1.rid==qunliao)
                    {
                        printf("sid:%d:name:%s\n %s",s1.sid,s1.name,s1.mes);
                    }
                    s1.tye='1';
                    s1.ret=myid;
                    send(cfd,&s1,sizeof(s1),0);
                }
        if (s1.sid==0) //发送者id不可能为0,表示结束并放开锁
            {
                p=0;
                pthread_mutex_unlock(&mutex7);
                break;
            }
                if (p==0)
                pthread_mutex_lock(&mutex7);
                p++;
                nmes=(mes_node_t*)malloc(sizeof(mes_node_t));
                nmes->data.sid=s1.sid;  //发送者
                nmes->data.rid=s1.rid;  //群聊id
                strcpy(nmes->data.mes,s1.mes);           
  //              printf("%-20d%-20s%-20d\n",s1.rid,s1.name,s1.ret);
                List_AddTail(gmes1,nmes);  
        break;
        case 'u':
        if (s1.sid==0) //发送者id不可能为0,表示结束并放开锁
            {
                v=0;
                pthread_mutex_unlock(&mutex8);
                break;
            }
                if (v==0)
                pthread_mutex_lock(&mutex8);
                v++;
                lmes=(mes_node_t*)malloc(sizeof(mes_node_t));
                lmes->data.sid=s1.sid;
                strcpy(lmes->data.mes,s1.mes);
                List_AddTail(gmes2,lmes); 
        break;
        case 'v':
             if (s1.sid==0)
            {
                q=0;
                pthread_mutex_unlock(&mutex9);
                allcansee=1;
                break;
            }
                if (q==0)
                pthread_mutex_lock(&mutex9);
                q++;
                file1=(file_node_t*)malloc(sizeof(file_node_t));
                file1->data.id=q;
                file1->data.sid=s1.sid;
                strcpy(file1->data.name,s1.name);
                file1->data.status=s1.rid;
                file1->data.size=s1.ret;         
  //              printf("%-20d%-20s%-20d\n",s1.rid,s1.name,s1.ret);
                List_AddTail(flist,file1);     
        break;
        case 'w':
         out=creat(s1.name,0664);
        int len=s1.ret;
        char buf[1024];
        int num;
        while(len>0 ){
            num=recv(cfd,buf,1024,0); 
            len=len-num;
            write(out,buf,num);
        	memset(buf,'\0',sizeof(buf));
        }

        close(out);
        break;
        
        }
    
    }
}
void exitgroup(int cfd,int gid)
{
    char choice;
    exit1:
    printf("你真的要退出本群聊吗？输入'y'或'n'");
    fflush(stdin);
    while(scanf("%c",&choice)!=1)
    {
        printf("输入的不是一个字符,请重新输入\n");
         while(getchar()!='\n');
         goto exit1;
    }
    while(getchar()!='\n');
    if (choice=='y')
    {
        struct work temp;
        temp.tye='x';
        temp.sid=gid;
        temp.rid=myid;
        send(cfd,&temp,sizeof(temp),0);
        return;
    }
    else if (choice=='n')
    {
        return;
    }
    else {
        printf("输入了一个无效字符,请重新输入\n");
        goto exit1;
    }
    
}
void yanzhengg(int cfd,int gid)
{
    int id;
    yan_node_t *p;
    printf("输入操作的序号\n");
    fflush(stdin);
    if (scanf("%d",&id)!=1)
    {  
        printf("输入不合法\n");
        return;
    }
     while(getchar()!='\n');
    List_ForEach(gyan,p)
    {
         struct work temp;
        if (p->data.xu==id)
        {
             printf("%-20s%-20s%-20s%-20s\n","序号","id","用户名","种类");
            printf("%-20d%-20d%-20s",p->data.xu,p->data.sid,p->data.name);
             if (p->data.type==1)
            {
                printf("%-20s\n","申请");
            }
            else if (p->data.type==0)
            {
                printf("%-20s\n","删除");
            }
            char a;
            printf("输入'1'来同意此条申请\n");
            printf("输入'2'来拒绝此条申请\n");
            yanzheng2:
            fflush(stdin);
            printf("your choice:");
            scanf("%c",&a);
            while(getchar()!='\n');
            switch(a)
            {
                case '1':
                temp.tye='t';
                temp.rid=gid;
                temp.sid=p->data.sid;
                temp.ret=p->data.type;
                printf("已发送处理请求\n");
                send(cfd,&temp,sizeof(temp),0);
                break;
                case '2':
                temp.tye='u';
                temp.rid=gid;
                temp.sid=p->data.sid;
                temp.ret=p->data.type;
                printf("已发送处理请求\n");
                send(cfd,&temp,sizeof(temp),0);
                break;
                default:
                printf("不是一个合法选项，请重新输入\n");
                goto yanzheng2;
                break;
            }
            break;
        }
    }
    

}
void yanzheng(int cfd)
{
    int id;
    yan_node_t *p;
    printf("输入操作的序号\n");
    fflush(stdin);
    if (scanf("%d",&id)!=1)
    {  
        printf("输入不合法\n");
        return;
    }
     while(getchar()!='\n');
    List_ForEach(list1,p)
    {
         struct work temp;
        if (p->data.xu==id)
        {
             printf("                        %-20s%-20s%-20s%-20s\n","序号","id","用户名","种类");
            printf("                        %-20d%-20d%-20s",p->data.xu,p->data.sid,p->data.name);
             if (p->data.type==1)
            {
                printf("%-20s\n","申请");
            }
            else if (p->data.type==0)
            {
                printf("%-20s\n","删除");
            }
            char a;
            printf("输入'1'来同意此条申请\n");
            printf("输入'2'来拒绝此条申请\n");
            yanzheng1:
            fflush(stdin);
            printf("your choice:");
            scanf("%c",&a);
            while(getchar()!='\n');
            switch(a)
            {
                case '1':
                temp.tye='h';
                temp.rid=myid;
                temp.sid=p->data.sid;
                temp.ret=p->data.type;
                printf("已发送处理请求\n");
                send(cfd,&temp,sizeof(temp),0);
                return;
                break;
                case '2':
                temp.tye='i';
                temp.rid=myid;
                temp.sid=p->data.sid;
                temp.ret=p->data.type;
                printf("已发送处理请求\n");
                send(cfd,&temp,sizeof(temp),0);
                return;
                break;
                default:
                printf("不是一个合法选项，请重新输入\n");
                goto yanzheng1;
                break;
            }
            break;
        }
    }
    printf("无效的序号\n");

}
void fetchallfriend(int cfd)
{
     char a;
     int simple=0;
    while(1)
    {
        allcansee=-1;
        system("clear");
        pthread_mutex_lock(&mutex);
        people_node_t *p;
        printf("============================ 好友列表 ============================\n");
        printf("                 %-20s%-20s%-20s\n","id","用户名","在线状态");
        List_ForEach(list,p)
        {
            printf("                 %-20d%-20s",p->data.id,p->data.name);
            if (p->data.status==1)
            printf("%-20s","在线");
            else printf("%-20s","不在线");
            printf("\n");
        }
        pthread_mutex_unlock(&mutex);
       
        printf("输入'1'来刷新状态\n");
        printf("输入'2'来退出查看\n");
        getfriend1:
        fflush(stdin);
        scanf("%c",&a);
        while(getchar()!='\n');
        switch(a)
        {
            case '1':
            List_Free(list,people_node_t);
            struct work temp={'c',0,0,"","",0};
            temp.sid=myid;
            send(cfd,&temp,sizeof(struct work),0);
            while(1)
            {
                 pthread_mutex_lock(&mutex);
                
                if (allcansee==-1)
                {
                    printf("等待服务器响应\n");
                     pthread_mutex_unlock(&mutex);
                     sleep(1);
                }
                else {allcansee=-1;
                   pthread_mutex_unlock(&mutex);
                    break;
                   }
            }
            break;
            case '2':
            simple=1;
            break;
            default:
            printf("不是一个合法选项,请重新输入\n");
           goto getfriend1;
            break;
        }
        if (simple==1)
        break;
    }
    
}
void add_friend(int cfd)
{           allcansee=-1;
            int id;
            printf("请输入你想添加的id号\n");
            fflush(stdin);
            if(scanf("%d",&id)!=1)
            {
                printf("输入的不是一个数字！\n");
               
            }
            while(getchar()!='\n');
            if (ismyfriend(id))
            {
                printf("id:%d用户已经是您的好友!\n",id);
                return;
            }
            if (id==myid)
            {
                printf("不能加自己为好友!\n");
                return;
            }
            struct work ss;
            ss.tye='d';
            ss.rid=id;
            ss.sid=myid;
            int i=0;
            send(cfd,&ss,sizeof(ss),0);
            while (allcansee==-1)
            {
                printf("等待客户端响应\n");
                i++;
                if (i==5)
                 send(cfd,&ss,sizeof(ss),0);
                sleep(1);
            }
            if (allcansee==1)
            {struct work dd=ss;
                printf("id:%d 用户存在,已发出请求\n",id);
                dd.tye='f';
                send(cfd,&dd,sizeof(ss),0);
                sleep(1);
            }
            else if(allcansee==0)
            {
                printf("id:%d 用户不存在\n",id);
                 sleep(1);
            }
            
        

   
}
void delete_friend(int cfd)
{
    int id;
    printf("请输入你想删除好友的id号\n");
    fflush(stdin);
    if(scanf("%d",&id)!=1)
    {
        printf("输入的不是一个数字！\n");
        return;
    }
    while(getchar()!='\n');
    if (!ismyfriend(id))
    {
        printf("该用户不是您的好友!\n");
        return;
    }
    struct work ss;
    ss.tye='j';
    ss.rid=id;
    ss.sid=myid;
    ss.ret=0;
    send(cfd,&ss,sizeof(ss),0);
}
int ismyfriend(int id)
{
    people_node_t*p;
    List_ForEach(list,p)
    {
        if (p->data.id==id)
        return 1;

    }
    return 0;
}
int ismygroup(int gid)
{
     group_node_t*p;
    List_ForEach(group1,p)
    {
        if (p->data.gid==gid)
        return 1;

    }
    return 0;
}
char *getname(int id) //找到名字，未测试，就个遍历应该没问题
{
    people_node_t*p;
    List_ForEach(list,p)
    {
        if (p->data.id==id)
        return p->data.name;

    }
    printf("error\n");
    return NULL;
}
char *getgname(int id) //找到名字，未测试，就个遍历应该没问题
{
    group_node_t*p;
    List_ForEach(group1,p)
    {
        if (p->data.gid==id)
        return p->data.name;

    }
    printf("error\n");
    return NULL;
}
int isonline(int id) //判断是否在线，未测试应该没问题
{
    people_node_t*p;
    List_ForEach(list,p)
    {
        if (p->data.id==id)
        {
            if (p->data.status==1)
            {
                return 1;
            }
            return 0;
        }

    }
}
void getrequst(int cfd)
{
    char a;
    int simple=0;
    while(1)
    {
        system("clear");
         printf("============================ 好友管理 ============================\n");
        pthread_mutex_lock(&mutex1);
        yan_node_t *p;
        printf("                        %-20s%-20s%-20s%-20s\n","序号","id","用户名","种类");
        List_ForEach(list1,p)
        {
            printf("                        %-20d%-20d%-20s",p->data.xu,p->data.sid,p->data.name);
            if (p->data.type==1)
            {
                printf("%-20s\n","申请");
            }
            else if (p->data.type==0)
            {
                printf("%-20s\n","删除");
            }
        }
        pthread_mutex_unlock(&mutex1);
       
        printf("输入'1'来刷新验证消息表\n");
        printf("输入'2'来选择一条信息操作\n");
        printf("输入'3'来退出\n");
        getrequst1:
        fflush(stdin);
        scanf("%c",&a);
        while(getchar()!='\n');
        switch(a)
        {
            case '1':
            List_Free(list1,yan_node_t);
            struct work temp={'g',0,0,"","",0};
            temp.sid=myid;
            send(cfd,&temp,sizeof(struct work),0);
            printf("I'm waiting for data now\n");
            sleep(2);
            break;
            case '2':
            yanzheng(cfd);
            break;
            case '3':
            simple=1;
            break;
            default:
            printf("不是一个合法选项,请重新输入\n");
           goto getrequst1;
            break;
        }
        if (simple==1)
        break;
        printf("输入回车来继续\n");
        while(getchar()!='\n');
    }
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
void creategroup(int cfd)
{
    printf("请为您想创建的群组设置一个名字\n");
    struct work test;
    int j;
    char n;
    char usrname[30];
     add_group:
        printf("群组名:");
        j = 0;
        while ((n = getchar()) != '\n')
        {
            if (n == ' ' || j >= 29)
            {
                while ((n = getchar()) != '\n');
                
                printf("群组名不合规\n");
                j = 0;
                goto add_group;
            }
            usrname[j++] = n;
        }
        if (n == '\n' && j == 0)
        {
            printf("群组名不为空\n");
            j = 0;
            goto add_group;
        }
        usrname[j] = '\0';
    strcpy(test.name,usrname);
    test.tye='o';
    test.sid=myid;
     allcansee=-1;
    send(cfd,&test,sizeof(test),0);
    while(allcansee==-1)
    {
        sleep(1);
    }
    if (allcansee==0)
    {
        printf("该群组已经存在！\n");
    }
    else printf("已成功建立群组\n");
     allcansee=-1;
    printf("输入回车来返回上层界面\n");
    while(getchar()!='\n');
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
        add_usr:
        system("clear");
        printf("============================ 登陆界面 ============================\n");
   
        printf("                      用户名:");
        j = 0;
        while ((n = getchar()) != '\n')
        {
            if (n == ' ' || j >= 29)
            {
                while ((n = getchar()) != '\n');
                
                printf("用户名不合规,请在回车后继续\n");
                while(getchar()!='\n');
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
        printf("                      密码:");
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
         add_usr:
         system("clear");
        printf("============================ 注册界面 ============================\n");

        printf("                      用户名:");
        j = 0;
        while ((n = getchar()) != '\n')
        {
            if (n == ' ' || j >= 29)
            {
                while ((n = getchar()) != '\n');
                
                printf("用户名不合规,请在回车后继续\n");
                while(getchar()!='\n');
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
        printf("                      密码:");
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
void readsmes(int cfd)
{
   char *a;
   int id;
   char n;
     people_node_t *p;
    mes_node_t *q;
    int i=0;
   readsmes1:
   system("clear");
        printf("============================ 聊天记录 ============================\n");
        printf("                        %-20s%-20s%-20s\n","id","用户名","消息数");
        
        List_ForEach(list,p)
        {
            i=0;
            List_ForEach(mes2,q)
            {
                if (q->data.sid==p->data.id)
                i++;
            }
            printf("                        %-20d%-20s%-20d\n",p->data.id,p->data.name,i);
        }
    printf("请选择你要读取的用户(id)\n");
    fflush(stdin);
    while(scanf("%d",&id)!=1)
    {
        printf("输入的不是一个数字\n");
        while(getchar()!='\n');
    }
    while (getchar()!='\n');
    if (!ismyfriend(id))
    {
        printf("id:%d用户不是您的好友,输入回车继续\n",id);
        while(getchar()!='\n');
        return;
    }
    List_ForEach(mes2,q)
    {
        if (q->data.sid==id)
        {
            printf("                                                id:%d\n",id);
            printf("                                                %s",q->data.mes);
        }
        else if (q->data.rid==id)
        {
            printf("您发出:\n");
            printf("%s",q->data.mes);
        }
    }
    printf("\n");
    printf("输入'1'继续查询\n");
    printf("输入'2'退出查询\n");
     readsmes2:
    fflush(stdin);
    n=getchar();
    while (getchar()!='\n');
    switch(n)
    {
        case '1':
          goto readsmes1;
        break;
        case '2':
        break;
        default:
        printf("输入不合法,请重新输入\n");
        goto  readsmes2;
        break;
    }
    
} 
void readgmes(int cfd,int gid)
{
   mes_node_t *q;
   char *a;
   system("clear");
    a=getgname(gid);
    printf("群组id：%d 群组名:%s\n",gid,a);
    pthread_mutex_lock(&mutex8);
    List_ForEach(gmes2,q)
    {
            printf("id: %d\n",q->data.sid);
            printf("%s",q->data.mes);

    }
    pthread_mutex_unlock(&mutex8);
    printf("输入回车键返回上层界面\n");
    while(getchar()!='\n');
    
}  
void nreadsmes(int cfd)
{
    people_node_t *p;
    mes_node_t *q;
    int i=0;
    int id;
    char a;
    mesid2:
    system("clear");
    struct work test;
     test.tye='l';
      test.rid=myid;
      mes_node_t* trans;
       system("clear");
        printf("============================ 聊天记录 ============================\n");
    printf("                        %-20s%-20s%-20s\n","id","用户名","消息数");
    List_ForEach(list,p)
    {
        i=0;
        List_ForEach(mes1,q)
        {
            if (q->data.sid==p->data.id)
            i++;
        }
        printf("                        %-20d%-20s%-20d\n",p->data.id,p->data.name,i);
    }
    printf("请选择你要读取的用户(id)\n");
    mesid:
    fflush(stdin);
    printf("yourchoice:");
    if (scanf("%d",&id)!=1)
    {  
        printf("输入不合法\n");
        while(getchar()!='\n');
        goto mesid;
    }
     while(getchar()!='\n');
     if (!ismyfriend(id))
     {
         printf("id:%d的用户不是您的好友\n",id);
     }
      List_ForEach(mes1,q)
        {
            if (q->data.sid==id)
            {
                trans=q->prev;
                test.sid=q->data.sid;
                strcpy(test.mes,q->data.mes);
                send(cfd,&test,sizeof(test),0);
                 List_DelNode(q);
                List_AddTail(mes2,q);
                printf("id:%d 用户名:%s\n%s",id,getname(id),q->data.mes);
                q=trans;
            }
            
        }
        printf("输入'1'来继续查看\n");
        printf("输入'2'来退出查看\n");
          mesid1:
        fflush(stdin);
        printf("yourchoice:");
        if (scanf("%c",&a)!=1)
        {  
            printf("输入不合法\n");
            while(getchar()!='\n');
            goto mesid1;
        }
         while(getchar()!='\n');
        switch(a)
        {
            case'1':
            goto mesid2;
            break;
            case'2':
            break;
            default:
            printf("不是一个合法选项\n");
            sleep(1);
             goto mesid2;
        }

}
void nreadgmes(int cfd,int gid)
{
    group_node_t *p;
    mes_node_t *q;
    int i=0;
    system("clear");
    struct work test;
     test.tye='1';
      test.ret=myid;
    printf("%-20s%-20s%-20s\n","gid","群组名","消息数");
    List_ForEach(group1,p)
    {
        i=0;
        pthread_mutex_lock(&mutex7);
        List_ForEach(gmes1,q)
        {
            if (q->data.sid==p->data.gid)
            i++;
        }
        pthread_mutex_unlock(&mutex7);
        printf("%-20d%-20s%-20d\n",p->data.gid,getgname(p->data.gid),i);
    }
      List_ForEach(gmes1,q)
        {
            if (q->data.rid==gid)
            {
                test.sid=q->data.sid;
                test.rid=q->data.rid;
                strcpy(test.mes,q->data.mes);
                send(cfd,&test,sizeof(test),0);
                printf("id:%d:\n %s",q->data.sid,q->data.mes);
            }
        }
        printf("输入回车返回上层界面\n");
        while(getchar()!='\n');

}
void fetchallmes(int cfd)
{
    int simple=0;
     char a;
    while(1)
    {
         system("clear");
        printf("============================ 欢迎使用聊天室 ============================\n");
        printf("                        输入'1'来查看未读消息\n");
        printf("                        输入'2'来查看已读消息\n");
        printf("                        输入'3'来退出消息记录\n");
        fflush(stdin);
        scanf("%c",&a);
        while(getchar()!='\n');
        switch(a)
        {
            case '1':
            nreadsmes(cfd);
            break;
            case '2':
            readsmes(cfd);
            break;
            case '3':
            simple=1;
            break;
            default:
            printf("不是一个合法选项请重新输入\n");
            break;
        }
        if (simple==1)
            break;
    }
}
void chatwithf(int cfd)
{
    int id;
    printf("请输入你想私聊好友的id号\n");
    fflush(stdin);
    if(scanf("%d",&id)!=1)
    {
        printf("输入的不是一个数字！\n");
        printf("输入回车键继续\n");
         while(getchar()!='\n');
        return;
    }
    while(getchar()!='\n');
    if (!ismyfriend(id))
    {
        printf("该用户不是您的好友!\n");
         printf("输入回车键继续\n");
          while(getchar()!='\n');
        return;
    }
    char *name=getname(id);
    struct work mes;
    siliao=id;
    system("clear");
    mes.rid=id;mes.sid=myid;mes.tye='k';mes.ret=0; //k为发送消息包，并通过ret=1代表此消息对方未读.
    printf("                        %s正在与你私聊,输入\"/exit来退出\"\n",name);
    if (!isonline(id))
    {
        printf("当前对方不在线\n");
    }
     printf("your send: ");
    while(fgets(mes.mes,1000,stdin))
    {
 //       printf("your send:\n");
        if (strcmp(mes.mes,"/exit\n")==0)
        {
            break;
        }
        send(cfd,&mes,sizeof(struct work),0);
    }
    siliao=0;
}
void chatwithg(int cfd)
{
    int gid;
    printf("请输入你想进入的群组id号\n");
    fflush(stdin);
    if(scanf("%d",&gid)!=1)
    {
        printf("输入的不是一个数字！\n");
        return;
    }
    while(getchar()!='\n');
    if (!ismygroup(gid))
    {
        printf("您还未加入该群组!，输入回车键继续\n");
        while(getchar()!='\n');
        return;
    }
    char *name=getgname(gid);
    struct work mes;
    qunliao=gid;
    mes.rid=gid;mes.sid=myid;mes.tye='z';mes.ret=0; //k为发送消息包，并通过ret=0代表此消息对方未读.
    printf("你正在群聊id:%d %s中,输入exit来退出\n",gid,name);
    while(fgets(mes.mes,1000,stdin))
    {
        printf("your send:\n");
        if (strcmp(mes.mes,"exit\n")==0)
        {
            break;
        }
        send(cfd,&mes,sizeof(mes),0);
    }
    qunliao=0;
}
void fetchallgmes(int cfd,int gid)
{
    int simple=0;
    char a;
    while(1)
    {system("clear");
        printf("输入'1'来查看未读消息\n");
        printf("输入'2'来查看已读消息\n");
        printf("输入'3'来退出消息记录\n");
        fflush(stdin);
        scanf("%c",&a);
        while(getchar()!='\n');
        switch(a)
        {
            case '1':
            nreadgmes(cfd,gid);
            break;
            case '2':
            readgmes(cfd,gid);
            break;
            case '3':
            simple=1;
            break;
            default:
            printf("不是一个合法选项请重新输入\n");
            break;
        }
        if (simple==1)
            break;
    }
}
void joingroup(int cfd)
{
    int id;
    struct work temp;
    printf("请输入您想加入的群组id号\n");
    fflush(stdin);
    while (scanf("%d",&id)!=1)
    {   while(getchar()!='\n');
        printf("输入的不是一个数字！，请重新输入\n");
    }
    while(getchar()!='\n');
    if (ismygroup(id))
    {
        printf("你已经是id:%d的群组成员,输入回车继续",id);
        while(getchar()!='\n');
        return;
    }
    temp.sid=myid;
    temp.rid=id;
    temp.tye='p';
    send(cfd,&temp,sizeof(temp),0);
     allcansee=-1;
    while(allcansee==-1)
    {
        sleep(1);
    }
    if (allcansee==0)
    {
        printf("该群组不存在！\n");
    }
    else if (allcansee==1)
    {
        printf("已成功发出请求\n");
    }
    printf("按下回车键返回上层界面\n");
    while(getchar()!='\n');
}
void getgroup(int cfd)
{
    struct group_node* p;
    char a; struct work test;
    int simple=0;
    while(1)
    {
        system("clear");
        pthread_mutex_lock(&mutex4);
        printf("%-20s%-20s\n","群组id","群组名称");
        List_ForEach(group1,p)
        {
            printf("%-20d%-20s\n",p->data.gid,p->data.name);
        }
        pthread_mutex_unlock(&mutex4);
        printf("输入'1'来刷新加入的群组\n");
        printf("输入'2'来返回上层界面\n");
        fflush(stdin);
        while(scanf("%c",&a)!=1)
        {
            printf("输入的不是一个有效选项\n");
            while(getchar()!='\n');
        }
        while(getchar()!='\n');
        switch(a)
        {
            case '1':
            List_Free(group1,group_node_t);
            test.tye='q';
            test.sid=myid;
            send(cfd,&test,sizeof(test),0);
            printf("正在获取数据.....\n");
            sleep(1);
            break;
            case '2':
            simple=1;
            break;
            default:
            printf("输入的不是有效选项,请重新输入\n");
            break;
        }
        if (simple==1)
        {
            break;
        }
    }
}
void deletemate(int gid,int cfd)
{
        char a;
     int simple=0;
     int id;
     int find=0;
    while(1)
    {
        system("clear");
        pthread_mutex_lock(&mutex6);
        people_node_t *p;
        printf("%-20s%-20s\n","id","用户名");
        List_ForEach(glist,p)
        {
            if (p->data.status==0)
            printf("%-20d%-20s\n",p->data.id,p->data.name);         
        }
        pthread_mutex_unlock(&mutex6);
       
        printf("输入'1'来刷新状态\n");
        printf("输入'2'来选择一个群成员(id)\n");
        printf("输入'3'来退出查看\n");
        deletemate1:
        fflush(stdin);
        scanf("%c",&a);
        while(getchar()!='\n');
        switch(a)
        {
            case '1':
            List_Free(glist,people_node_t);
            struct work temp={'v',0,0,"","",0};
            temp.sid=gid;
            send(cfd,&temp,sizeof(struct work),0);
            printf("I'm waiting for data now\n");
            sleep(2);
            break;
            case '2':
            printf("id:");
            fflush(stdin);
            while(scanf("%d",&id)!=1)
            {
                printf("输入的不是一个数字！请重新输入\n");
                while(getchar()!='\n');
            }
            while(getchar()!='\n');
             pthread_mutex_lock(&mutex6);
             find=0;
            List_ForEach(glist,p)
            {
                if (p->data.id==id)
                {
                    find=1;
                    if (p->data.status>1)
                    {
                        printf("该用户是群主或管理员,不能删除!\n");
                    }
                    else{
                         struct work temp={'x',0,0,"","",0};
                         temp.sid=gid;
                         temp.rid=id;
                         send(cfd,&temp,sizeof(temp),0);
                    }
                    break;
                }    
            }
            pthread_mutex_unlock(&mutex6);
            if (find==0)
            printf("该用户不是群成员!\n");
            break;
            case '3':
            simple=1;
            break;
            default:
            printf("不是一个合法选项,请重新输入\n");
           goto deletemate1;
            break;
        }
        if (simple==1)
        break;
    }
}
void setadmin(int gid,int cfd)
{
        char a;
     int simple=0;
     int id;
     int find=0;
    while(1)
    {
        system("clear");
        pthread_mutex_lock(&mutex6);
        people_node_t *p;
        printf("%-20s%-20s\n","id","用户名");
        List_ForEach(glist,p)
        {
            if (p->data.status==0)
            printf("%-20d%-20s\n",p->data.id,p->data.name);         
        }
        pthread_mutex_unlock(&mutex6);
       
        printf("输入'1'来刷新状态\n");
        printf("输入'2'来选择一个群成员(id)\n");
        printf("输入'3'来退出查看\n");
        setadmin1:
        fflush(stdin);
        scanf("%c",&a);
        while(getchar()!='\n');
        switch(a)
        {
            case '1':
            List_Free(glist,people_node_t);
            struct work temp={'v',0,0,"","",0};
            temp.sid=gid;
            send(cfd,&temp,sizeof(struct work),0);
            printf("I'm waiting for data now\n");
            sleep(2);
            break;
            case '2':
            printf("id:");
            fflush(stdin);
            while(scanf("%d",&id)!=1)
            {
                printf("输入的不是一个数字！请重新输入\n");
                while(getchar()!='\n');
            }
            while(getchar()!='\n');
             pthread_mutex_lock(&mutex6);
             find=0;
            List_ForEach(glist,p)
            {
                if (p->data.id==id)
                {
                    find=1;
                    if (p->data.status>0)
                    {
                        printf("该用户已经是群主或管理员！\n");
                    }
                    else{
                         struct work temp={'w',0,0,"","",0};
                         temp.sid=gid;
                         temp.rid=id;
                         send(cfd,&temp,sizeof(temp),0);
                    }
                    break;
                }    
            }
            pthread_mutex_unlock(&mutex6);
            if (find==0)
            printf("该用户不是群成员!\n");
            break;
            case '3':
            simple=1;
            break;
            default:
            printf("不是一个合法选项,请重新输入\n");
           goto setadmin1;
            break;
        }
        if (simple==1)
        break;
    }
}
void managegroup(int cfd)
{
    struct group_node *p;
    int id;
    mangroup:
    system("clear");
    printf("============================ 群组管理 ============================\n");
    int i=0;
    printf("                  %-20s%-20s\n","群组id","群组名称");
    pthread_mutex_lock(&mutex4);
    List_ForEach(group1,p)
    {
        printf("                  %-20d%-20s\n",p->data.gid,p->data.name);
    }
    pthread_mutex_unlock(&mutex4);
    printf("请输入你要管理的群组id:\n");
    fflush(stdin);
    while(scanf("%d",&id)!=1)
    {
        printf("输入的不是有效的id!请重新输入\n");
        while(getchar()!='\n');
    }
    while(getchar()!='\n');
    List_ForEach(group1,p)
    {
       if (p->data.gid==id)
       {
           i=1;
           break;
       }
    }
    if (i==0)
    {
        printf("输入的不是有效的id!请重新输入\n");
        goto mangroup;
    }
    if (p->data.power==2)
    {
            owner(p,cfd);
    }
    else if (p->data.power==1)
    {
            admin(p,cfd);
    }
    else if (p->data.power==0)
    {
            dog(p,cfd);
    }

}
void getgrequst(group_node_t* temp,int cfd)   //未完成
{
    char a;
    int simple=0;
    while(1)
    {
        system("clear");
          printf("============================ 群组管理 ============================\n");
        pthread_mutex_lock(&mutex5);
        yan_node_t *p;
        printf("                    %-20s%-20s%-20s%-20s\n","序号","id","用户名","种类");
        List_ForEach(gyan,p)
        {
        printf("                    %-20d%-20d%-20s",p->data.xu,p->data.sid,p->data.name);
            if (p->data.type==1)
            {
                printf("%-20s\n","申请");
            }
            else if (p->data.type==0)
            {
                printf("%-20s\n","删除");
            }
        }
        pthread_mutex_unlock(&mutex5);
       
        printf("                        输入'1'来刷新验证消息表\n");
        printf("                        输入'2'来选择一条信息操作\n");
        printf("                        输入'3'来退出\n");
        getgrequst1:
        fflush(stdin);
        scanf("%c",&a);
        while(getchar()!='\n');
        switch(a)
        {
            case '1':
            List_Free(gyan,yan_node_t);
            struct work temp1={'r',0,0,"","",0};
            temp1.sid=myid;
            temp1.rid=temp->data.gid;
            send(cfd,&temp1,sizeof(struct work),0);
            printf("I'm waiting for data now\n");
            sleep(2);
            break;
            case '2':
            yanzhengg(cfd,temp->data.gid);
            break;
            case '3':
            simple=1;
            break;
            default:
            printf("不是一个合法选项,请重新输入\n");
           goto getgrequst1;
            break;
        }
        if (simple==1)
        break;
    }
}
void owner(group_node_t* temp,int cfd) //主人
{
    int simple=0;
    struct work temp1;
    temp1.tye='y';
    temp1.sid=temp->data.gid;
    while(1)
    {
        own:
        system("clear");
          printf("============================ 群组管理 ============================\n");
        char a;
        printf("欢迎你%-20s的群主\n",temp->data.name);
        printf("                        输入'1'来查看群验证信息\n");
        printf("                        输入'2'来设置一个管理员\n");
        printf("                        输入'3'来踢出一个群成员\n");
        printf("                        输入'4'来解散本群组\n");
        printf("                        输入'5'来查看群成员\n");
        printf("                        输入'6'来退出本界面\n");
        fflush(stdin);
        if (scanf("%c",&a)!=1)
        {
            printf("输入不合法,请重新输入\n");
            while(getchar()!='\n');
            goto own;
        }
        while(getchar()!='\n');
        switch(a)
        {
            case '1':
            getgrequst(temp,cfd);
            break;
            case '2':
            setadmin(temp->data.gid,cfd);
            break;
            case '3':
            deletemate(temp->data.gid,cfd);
            break;
            case '4':
            send(cfd,&temp1,sizeof(temp1),0);
            simple=1;
            break;
            case '5':
            getgroupmates(temp->data.gid,cfd);
            break;
            case '6':
            simple=1;
            break;
            default:
            printf("输入不合法,请重新输入\n");
            goto own;
            break;
        }
        if (simple==1)
        break;
    }
}
void admin(group_node_t* temp,int cfd)  //管理员
{
     int simple=0;
    while(1)
    {
       admin1:
        system("clear");
        printf("============================ 群组管理 ============================\n");
        char a;
        printf("欢迎你%-20s的管理员\n",temp->data.name);
        printf("                        输入'1'来查看群验证信息\n");
        printf("                        输入'2'来踢出一个群成员\n");
        printf("                        输入'3'来查看群成员\n");
        printf("                        输入'4'来退出本群聊\n");
        printf("                        输入'5'来退出本界面\n");
        fflush(stdin);
        if (scanf("%c",&a)!=1)
        {
            printf("输入不合法,请重新输入\n");
            while(getchar()!='\n');
            goto admin1;
        }
        while(getchar()!='\n');
        switch(a)
        {
            case '1':
            getgrequst(temp,cfd);
            break;
            case '2':
            deletemate(temp->data.gid,cfd);
            break;
            case '3':
            getgroupmates(temp->data.gid,cfd);
            break;
            case '4':
            exitgroup(cfd,temp->data.gid);
            break;
            case '5':
            simple=1;
            break;
            default:
            printf("输入不合法,请重新输入\n");
            goto admin1;
            break;
        }
        if (simple==1)
        break;
    }
}
void dog(group_node_t* temp,int cfd) //普通群员
{
     int simple=0;
    while(1)
    {
       dog1:
        system("clear");
        char a;
          printf("============================ 群组管理 ============================\n");
        printf("欢迎你%-20s的群成员\n",temp->data.name);
        printf("                        输入'1'来查看群成员\n");
        printf("                        输入'2'来退出本群聊\n");
        printf("                        输入'3'来退出本界面\n");
        fflush(stdin);
        if (scanf("%c",&a)!=1)
        {
            printf("输入不合法,请重新输入\n");
            while(getchar()!='\n');
            goto dog1;
        }
        while(getchar()!='\n');
        switch(a)
        {
            case '1':
            getgroupmates(temp->data.gid,cfd);
            break;
            case '2':
            exitgroup(cfd,temp->data.gid);
            break;
            case '3':
            simple=1;
            break;
            default:
            printf("输入不合法,请重新输入\n");
            goto dog1;
            break;
        }
        if (simple==1)
        break;
    }
}
void grouphistory(int cfd)
{
   struct group_node *p;
    int id;
    grouph:
    system("clear");
    int i=0;
    printf("                        %-20s%-20s\n","群组id","群组名称");
    pthread_mutex_lock(&mutex4);
    List_ForEach(group1,p)
    {
        printf("                        %-20d%-20s\n",p->data.gid,p->data.name);
    }
    pthread_mutex_unlock(&mutex4);
    printf("请输入你要查看记录的群组id:\n");
    fflush(stdin);
    while(scanf("%d",&id)!=1)
    {
        printf("输入的不是有效的id!请重新输入\n");
        while(getchar()!='\n');
        goto grouph;
    }
    while(getchar()!='\n');
    List_ForEach(group1,p)
    {
       if (p->data.gid==id)
       {
           i=1;
           break;
       }
    }
    if (i==0)
    {
        printf("输入的不是有效的id!请输入回车后继续\n");
        while(getchar()!='\n');
        return;
    }
    struct work temp;
    temp.tye='2';temp.sid=myid;temp.rid=id;send(cfd,&temp,sizeof(temp),0);
    temp.tye='3';send(cfd,&temp,sizeof(temp),0);
    fetchallgmes(cfd,id);
}
void transfile(int cfd)
{
    char choice;
    int simple=0;
    while(1)
    {
        system("clear");
        printf("============================ 传输文件 ============================\n");
        printf("                        输入'1'来查看待接收的文件\n");
        printf("                        输入'2'来发送一个文件\n");
        printf("                        输入'3'来退出出此界面\n");
        fflush(stdin);
        while(scanf("%c",&choice)!=1)
        {
            printf("输入不正确,请重新输入\n");
            while(getchar()!='\n');
        }
        while(getchar()!='\n');
        switch(choice)
        {
            case '1':
            rfile(cfd);
            break;
            case '2':
            sfile(cfd);
            break;
            case '3':
            simple=1;
            break;
            default:
             printf("输入不正确,请重新输入\n");
            break;
        }
        if (simple==1)
        break;
    }
}
void sfile(int cfd)
{
    int id;
    int fd;
    char filename[30];
    printf("输入你想传输的对象id号\n");
    if (scanf("%d",&id)!=1||!ismyfriend(id))
    {
        printf("无效的ID\n");
        while(getchar()!='\n');
        return;
    }getchar();
    
    printf("请输入正确的文件地址\n");
    scanf("%s",filename);getchar();
    struct work temp;
    temp.sid=myid;
    temp.rid=id;
    temp.tye='4';
    fd=open(filename,O_RDONLY,0664);
    if (fd==-1)
    {
        printf("open %s\n failed\n",filename);
        perror("open");
    }
    struct stat stat_buf;
    fstat(fd,&stat_buf);
    temp.ret=stat_buf.st_size;
    strcpy(temp.name,find_file_name(filename));
    send(cfd,&temp,sizeof(temp),0);
    sendfile(cfd,fd,NULL,stat_buf.st_size);
    allcansee=0;
    while(!allcansee)
    {
        printf("等待服务器响应\n");
        sleep(1);
    }
}
void rfile(int cfd)
{   file asdsa;
    struct file_node *q;
    int simple=0;
    char a;
    while(1)
    {
        system("clear");
    printf("============================ 传输文件 ============================\n");
    printf("                    %-20s%-20s%-20s%-20s%-20s\n","id","sid","文件名","大小","是否读取过");
    List_ForEach(flist,q)
    {
          printf("                    %-20d%-20d%-20s%-20d",q->data.id,q->data.sid,q->data.name,q->data.size);
          if (q->data.status)
          printf("%-20s\n","已下载");
          else printf("%-20s\n","未下载");
    }
    printf("                        输入'1'来选择一个文件下载\n");
    printf("                        输入'2'来在服务器上删除一个文件\n");
    printf("                        输入'3'来刷新数据\n");
    printf("                        输入'4'来退出本界面\n");
     rfile1:
        fflush(stdin);
        scanf("%c",&a);
        while(getchar()!='\n');
        switch(a)
        {
            case '3':
            List_Free(flist,file_node_t);
            struct work temp1={'5',0,0,"","",0};
            temp1.sid=myid;
            send(cfd,&temp1,sizeof(struct work),0);
            allcansee=0;
            while(!allcansee)
            {
                sleep(1);
            printf("I'm waiting for data now\n");
            }
            break;
            case '1':
            loadfile(cfd);
            break;
            case '2':
            delfile(cfd);
            break;
            case '4':
            simple=1;
            break;
            default:
            printf("不是一个合法选项,请重新输入\n");
           goto rfile1;
            break;
        }
        if (simple==1)
        {
            break;
        }
    }
    
}

char *find_file_name(char *name)
{
	char *name_start = NULL;
	int sep = '/';
	if (NULL == name) {
			printf("the path name is NULL\n");
	    return NULL;
	}
	name_start = strrchr(name, sep);
 
	return (NULL == name_start)?name:(name_start + 1);
}
void delfile(int cfd)
{   struct file_node *q;
        int id;
        system("clear");
        struct work temp;
        temp.tye='7';
        List_ForEach(flist,q)
        {
            printf("%-20d%-20d%-20s%-20d",q->data.id,q->data.sid,q->data.name,q->data.size);
            if (q->data.status)
            printf("%-20s\n","已下载");
            else printf("%-20s\n","未下载");
        }
        printf("请选择想要删除的文件id:\n");
        if (scanf("%d",&id)!=1)
        printf("输入的不是一个id!\n");
        while(getchar()!='\n');
        List_ForEach(flist,q)
        {
            if (q->data.id==id)
                {
                    temp.sid=q->data.sid;
                    temp.rid=myid;
                    strcpy(temp.name,q->data.name);
                    temp.ret=q->data.size;
                    send(cfd,&temp,sizeof(temp),0);
                    break;
                }
                printf("输入了无效的id号\n");
        }printf("输入回车继续\n");
        
        while(getchar()!='\n');

}
void loadfile(int cfd)
{
    struct file_node *q;
        int id;
        system("clear");
        struct work temp;
        temp.tye='6';
        List_ForEach(flist,q)
        {
            printf("%-20d%-20d%-20s%-20d",q->data.id,q->data.sid,q->data.name,q->data.size);
            if (q->data.status)
            printf("%-20s\n","已下载");
            else printf("%-20s\n","未下载");
        }
        printf("请选择想要下载文件id:\n");
        if (scanf("%d",&id)!=1)
        printf("输入的不是一个id!\n");
        while(getchar()!='\n');
        List_ForEach(flist,q)
        {
            if (q->data.id==id)
                {
                    temp.sid=q->data.sid;
                    temp.rid=myid;
                    strcpy(temp.name,q->data.name);
                    temp.ret=q->data.size;
                    send(cfd,&temp,sizeof(temp),0);
                    break;
                }
               
        }
         printf("输入了无效的id号\n");
        printf("输入回车继续\n");
        
        while(getchar()!='\n');
}
void managefriend(int cfd)
{
    int simple=0;
    while(1)
    {
        system("clear");
          printf("============================ 好友管理 ============================\n");
        char a;
        printf("                        输入'1'来添加一个好友\n");
        printf("                        输入'2'来删除一个好友\n");
        printf("                        输入'3'来查看验证消息\n");
        printf("                        输入'4'来返回上层界面\n");
        fflush(stdin);
        scanf("%c",&a);
        while(getchar()!='\n');
        switch(a)
        {
            case '1':
            add_friend(cfd);
            break;
            case '2':
            delete_friend(cfd);
            break;
            case '3':
            getrequst(cfd);
            break;
            case '4':
            simple=1;
            break;
            default:
            printf("不是一个合法选项\n");
            break;
        }
         printf("按回车后继续\n");
         while(getchar()!='\n');
        if (simple==1)
            break;
    }
}
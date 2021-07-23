#include "c.h"
extern int myid;
extern int allcansee;
extern yan_list_t list1;
extern people_list_t list;
 
extern pthread_mutex_t mutex ;
extern pthread_mutex_t mutex1 ;
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
    int i,j=0;
    int cfd=*(int *)temp;
    struct work s1;
    people_node_t *new=NULL;
    yan_node_t *old=NULL;
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
        case 'f':
                old=(yan_node_t*)malloc(sizeof(yan_node_t));
                old->data.sid=s1.sid;
                strcpy(old->data.name,s1.name);
                old->data.type=1;
                old->data.xu=0;
 //               printf("%-20d%-20s%-20d\n",s1.sid,s1.name,1);
                List_AddTail(list1,old);      
            break;
        case 'g':
            if (s1.sid==0)
            {
                j=0;
                pthread_mutex_unlock(&mutex);
                break;
            }
                if (j==0)
                pthread_mutex_lock(&mutex);
                j++;
                old=(yan_node_t*)malloc(sizeof(yan_node_t));
                old->data.xu=j;
                old->data.sid=s1.rid;
                strcpy(old->data.name,s1.name);
                old->data.type=s1.ret;
  //              printf("%-20d%-20s%-20d\n",s1.rid,s1.name,s1.ret);
                List_AddTail(list1,old);                
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
                break;
                case '2':
                temp.tye='i';
                temp.rid=myid;
                temp.sid=p->data.sid;
                temp.ret=p->data.type;
                printf("已发送处理请求\n");
                send(cfd,&temp,sizeof(temp),0);
                break;
                default:
                printf("不是一个合法选项，请重新输入\n");
                goto yanzheng1;
                break;
            }
            break;
        }
    }
    

}
void fetchallfriend(int cfd)
{
     char a;
     int simple=0;
    while(1)
    {
        system("clear");
        pthread_mutex_lock(&mutex);
        people_node_t *p;
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
            printf("I'm waiting for data now\n");
            sleep(2);
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
{
    int id;
    char a;
    int simple;
    while(1)
    {
    printf("输入'1'来添加\n");
    printf("输入'2'来退出查看\n");
    getfriend1:
    fflush(stdin);
    scanf("%c",&a);
    while(getchar()!='\n');
    switch(a)
        {
            case '1':
            printf("请输入你想添加的id号\n");
            fflush(stdin);
            if(scanf("%d",&id)!=1)
            {
                printf("输入的不是一个数字！\n");
                break;
            }
            while(getchar()!='\n');
            struct work ss;
            ss.tye='d';
            ss.rid=id;
            ss.sid=myid;
            send(cfd,&ss,sizeof(ss),0);
            allcansee=-1;
            if (allcansee==-1)
            {
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
            }
            break;
            case '2':
            simple=1;
            break;
            default:
            printf("%c不是一个合法选项,请重新输入\n",a);
            goto getfriend1;
                break;
        }
        if (simple==1)
        break;
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
void getrequst(int cfd)
{
    char a;
    int simple=0;
    while(1)
    {
   //     system("clear");
        pthread_mutex_lock(&mutex1);
        yan_node_t *p;
        printf("%-20s%-20s%-20s%-20s\n","序号","id","用户名","种类");
        List_ForEach(list1,p)
        {
            printf("%-20d%-20d%-20s",p->data.xu,p->data.sid,p->data.name);
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
void fetchallmes(int cfd)
{
    
}
void managefriend(int cfd)
{
    int simple=0;
    while(1)
    {
        char a;
        printf("输入'1'来添加一个好友\n");
        printf("输入'2'来删除一个好友\n");
        printf("输入'3'来查看验证消息\n");
        printf("输入'4'来返回上层界面\n");
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
            printf("不是一个合法选项请重新输入\n");
            break;
        }
        if (simple==1)
            break;
    }
}
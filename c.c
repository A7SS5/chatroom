#include <stdio.h>                                                                                                                  
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include    <assert.h>
 #include <termio.h>
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
 
struct work {
    char tye;
    char name[20];
    char password[20];
    int ret;
};
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
    return 1;
}
int SysLogin(int efd)  // SL界面
{
    struct work test={'a',"",""};
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
        if (login(test,efd) == 0)
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
        else
        {
            printf("Welcome!%s\n", usrname);
            break;
        }
    }
        return 1;
}
int SysLogon(int efd)
{
        struct work test={'b',"",""};
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
    int cfd = 0;
    struct sockaddr_in saddr;
    char buf[BUFSIZ] = {0};
    int i = 0,n = 0;
 
    cfd = socket(AF_INET,SOCK_STREAM,0);
 
    bzero(&saddr,sizeof(saddr));
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET,SERVER_ADDR,&saddr.sin_addr.s_addr);
    saddr.sin_port = htons(SERVER_PORT);
 
    if ((connect(cfd,(struct sockaddr*)&saddr,sizeof(saddr)))!=0)
    {
        printf ("connect error\n");
    }
    int t=5;
    struct work test={'a',"",""};
    char op;
    int ret;
    printf("欢迎使用大鹏聊天室\n");
    while(1)
    {
        printf("输入'1'来登陆一个已注册帐号\n");
        printf("输入'2'来注册一个帐号\n");
        printf("输入'3'退出本聊天室\n");
        fflush(stdin);
        scanf("%c",&op);
        getchar();
        switch(op){
        case '1':
        if ((ret=SysLogin(cfd)))
        {
            break;
            sleep(10);
        }
        break;
        case '2':
       if ((ret=SysLogon(cfd)))
        {
            sleep(5);
        }
        break;
        case '3':
        close(cfd);
        return 0;
        break;
        default:
        printf("%c不是一个合法选项\n",op);
        }
        if (ret==1)
        break;
    }
    close(cfd);
 
    return 0;
}
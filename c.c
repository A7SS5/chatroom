#include <stdio.h>                                                                                                                  
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
 
#define SERVER_ADDR     "127.0.0.1"
#define SERVER_PORT     9000
 
struct work {
    char tye;
    char name[20];
    char paaword[20];
};
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
    printf("已经成功发起了连接!\n");
    printf("name:");
    scanf("%s",test.name);
    printf("password:");
    scanf("%s",test.paaword);
    send(cfd,&test,sizeof(test),0);
    scanf("%c",&op);
    if (op=='y')
    {
        printf("success\n");
    }
    sleep(5);
    close(cfd);
 
    return 0;
}
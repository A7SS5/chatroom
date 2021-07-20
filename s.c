#include <netinet/in.h>   
#include <arpa/inet.h>
#include <string.h>  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <string.h>
#include <pthread.h>
#include "mysqlc.h"
#define EVENTS_MAX_SIZE 20
#define SERV_PORT 9000
#define MAX_CONTECT_SIZE 20
struct s1 {
    int epfd,conn_fd;
};
struct work {
    char tye;
    char name[20];
    char password[20];
    int ret;
};
void mylogin(struct work s1,struct s1* s)
{
    struct work rets;
     int c=judege(s1.name,s1.password);
    if (c==1)
	{
        rets.ret=1;
		printf("用户%s登陆成功\n",s1.name);
	}
	else if (c==0)
	{   rets.ret=0;
			printf("用户%s登陆失败\n",s1.name);
	}
	else printf("mysql error\n");
     send(s->conn_fd,&rets,sizeof(rets),0);
}
void mylogon(struct work s1,struct s1* s)
{
    struct work rets;
     int c=judegeon(s1.name,s1.password);
    if (c==1)
	{
        rets.ret=1;
		printf("用户%s注册成功\n",s1.name);
	}
	else if (c==0)
	{   rets.ret=0;
			printf("用户%s注册失败\n",s1.name);
	}
	else printf("mysql error\n");
     send(s->conn_fd,&rets,sizeof(rets),0);
}
void *solve(void* temp)
{
    struct work s1;
    pthread_detach(pthread_self());
    struct epoll_event ev;
    struct s1 *s=(struct s1*)temp;
    printf("efd:%d cfd:%d\n",s->epfd,s->conn_fd);
    int ret = recv(s->conn_fd,&s1,sizeof(s1),0);
    if (ret==0)
    {
        epoll_ctl(s->epfd,EPOLL_CTL_DEL,s->conn_fd,NULL);
        close(s->conn_fd);
        printf("对端已经关闭了");
        pthread_exit(0);
    }
  //  read(s->conn_fd,buf,sizeof(buf));
//    printf("%s %s",s1.name,s1.password);
    switch(s1.tye)
    {
        case 's':
        mylogin(s1,s);
        break;
        case 'b':
        mylogon(s1,s);
        break;
    }
    printf("end\n");
    pthread_exit(0);
    return 0;
}
int main()
{
    struct sockaddr_in cliaddr,servaddr;
    socklen_t cliaddr_len;
    struct s1* temp;
    pthread_t pth1;
    char str[100];
    temp=(struct s1*)malloc(sizeof(struct s1));
    int epfd,sock_fd,nfds,conn_fd;
    sock_fd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(SERV_PORT);

    int opt = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
    bind(sock_fd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    listen(sock_fd,MAX_CONTECT_SIZE);
    struct epoll_event ev,events[10];
    epfd=epoll_create(MAX_CONTECT_SIZE);
    ev.data.fd= sock_fd;
    int connect_size=0;
    ev.events =EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sock_fd,&ev);
    connect_size++;
    for(;;)
    {   
        nfds = epoll_wait(epfd,events,EVENTS_MAX_SIZE,-1);//等待可写事件
        for(int i=0;i<nfds;i++)
        {
            if(events[i].data.fd==sock_fd)       //服务器套接字接收到一个连接请求
            {
                if(connect_size>MAX_CONTECT_SIZE)
                {
                    printf("%d %d\n",connect_size,nfds);
                    sleep(3);
                    perror("到达最大连接数!\n");
                    break;
               //     continue;
                }
                cliaddr_len=sizeof(cliaddr);
                conn_fd=accept(events[i].data.fd,(struct sockaddr*)&cliaddr,&cliaddr_len);
                if(conn_fd<=0)
                {
                    perror("error in accept\n");
                    printf("%s\n",strerror(errno));
                 //   continue;
                 break;
                }
                 connect_size++;
                 printf("received from %s at PORT %d\n",inet_ntop(AF_INET,&cliaddr.sin_addr,str,sizeof(str)),ntohs(cliaddr.sin_port));
                
                ev.data.fd= conn_fd;
                ev.events =EPOLLIN|EPOLLRDHUP;
                epoll_ctl(epfd,EPOLL_CTL_ADD,conn_fd,&ev);

            }
/*            else if(events[i].events&EPOLLRDHUP)
            {              
                epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd,NULL);
                 printf("shutdown\n");
            }
*/
            else{
                temp->epfd=epfd;
                temp->conn_fd=events[i].data.fd;
                pth1=pthread_create(&pth1,NULL,solve,temp);
            }
        }
    }
    close(sock_fd);
}
#include "mysqlc.h"
#define EVENTS_MAX_SIZE 20
#define SERV_PORT 9000
#define MAX_CONTECT_SIZE 20
people_list_t list=NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
void mylogin(struct work s1,struct s1* s)
{
    struct work rets;
     int c=judege(s1.name,s1.password);
    if (c==1)
	{
    people_node_t *new=NULL;
    new=(people_node_t*)malloc(sizeof(people_node_t));
    peple a;
     new->data.id=find_byname(s1.name);
    strcpy(new->data.name,s1.name);
    new->data.fd=s->conn_fd;
    rets.sid=new->data.id;
    people_node_t *p;
    List_ForEach(list,p)
    {
        printf("id:%d name:%s fd:%d\n",p->data.id,p->data.name,p->data.fd);
            if (p->data.id==new->data.id)
            {
                close(p->data.fd);
                List_FreeNode(p);
                break;
            }
    }

     List_AddTail(list,new);
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
    int out;
    struct work s1;
    int num;
        struct s1 *s=(struct s1*)temp;
    struct epoll_event ev;
    ev.data.fd = s->conn_fd;
     char *filename;
       struct work sim;
    ev.events = EPOLLIN |EPOLLONESHOT;
    printf("efd:%d cfd:%d\n",s->epfd,s->conn_fd);
    int ret = recv(s->conn_fd,&s1,sizeof(struct work),0);
    if (ret==0)
    {
        people_node_t* p;
        pthread_mutex_lock(&mutex2);
        List_ForEach(list,p)
        {
            if (p->data.fd==s->conn_fd)
             {List_FreeNode(p);
            break;}
        }
         pthread_mutex_unlock(&mutex2);
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
        case 'c':
        getmyfriend(s1.sid);
        break;
        case 'e':
        break;
        case 'd': pthread_mutex_lock(&mutex);
        printf("id:%d 是 %d\n",s1.rid,ishe(s1.rid,s));
                pthread_mutex_unlock(&mutex);
        break;
        case 'f':
        add_friends(s1);
        break;
        case 'g':
        getmyrequst(s1.sid);
        break;
        case 'h':
        printf("i get a 'h\n");
        agree(s1);
        break;
        case 'i':
        printf("i get a 'i'\n");
        disagree(s1);
        break;
        case 'j':
        delete_friend(s1);
        break;
        case 'k':
       ev.events = EPOLLIN |EPOLLONESHOT;
        epoll_ctl(s->epfd,EPOLL_CTL_MOD,s->conn_fd,&ev);
         ssend_mes(s1);
        pthread_detach(pthread_self());
        pthread_exit(0);
        return 0;
        break;
        case 'l':
         ev.events = EPOLLIN |EPOLLONESHOT;
        epoll_ctl(s->epfd,EPOLL_CTL_MOD,s->conn_fd,&ev);
          read_mes(s1);
        pthread_detach(pthread_self());
        pthread_exit(0);
        break;
        case 'm':
        printf("m\n");
        sendallmes(s1.sid);
        break;
        case 'n':
        getallnmes(s1.sid);
        break;
        case 'o':
        createg(s1);
        break;
        case 'p':
        joingroups(s1);
        break;
        case 'q':
        printf("i got q\n");
        getmygroup(s1.sid);
        break;
        case 'r':
        getmygrequst(s1.rid,s1.sid);
        break;
        case 't':
        agreeg(s1);
        break;
        case 'u':
        disagreeg(s1);
        break;
        case 'v':
        getmates(s1.sid,s->conn_fd);
        break;
        case 'w':
        setadmin(s1,s->conn_fd);
        break;
        case 'x':
        delmate(s1,s->conn_fd);
        break;
        case 'y':
        killgroup(s1.sid);
        break;
        case 'z':
        ev.events = EPOLLIN |EPOLLONESHOT;
        epoll_ctl(s->epfd,EPOLL_CTL_MOD,s->conn_fd,&ev);
        gsend_mes(s1);
        pthread_detach(pthread_self());
        pthread_exit(0);
        return 0;
        break;
        case '1':
         ev.events = EPOLLIN |EPOLLONESHOT;
        epoll_ctl(s->epfd,EPOLL_CTL_MOD,s->conn_fd,&ev);
        read_gmes(s1);
        pthread_detach(pthread_self());
        pthread_exit(0);
        return 0;
        break;
        case '2':
        getallgmes(s1.rid,s1.sid,s->conn_fd);
        break;
        case '3':
        getallngmes(s1.rid,s1.sid,s->conn_fd);
        break;
        case '4':
    //    epoll_ctl(s->epfd,EPOLL_CTL_DEL,s->conn_fd,NULL);
        sim.tye='d';
        filename=genRandomString(4);
        out=creat(filename,0664);
        int len=s1.ret;
        char buf[5000];
        while(len>0 ){
            memset(buf,'\0',sizeof(buf));
            num=recv(s->conn_fd,buf,4096,0); 
            len=len-num;
            write(out,buf,num);
        }
        
   //       epoll_ctl(s->epfd,EPOLL_CTL_ADD,s->conn_fd,&ev);
        savefile(s1,filename);
        close(out);
        send(s->conn_fd,&sim,sizeof(struct work),0);
        break;
        case '5':
        sendfilelist(s1);
        break;
        case '6':
        send_file(s1);
        break;
        case '7':
        delete_file(s1);
        break;
        default:
        printf("not really:%c\n",s1.tye);
    }
    printf("%s结束\n",s1.mes);
    epoll_ctl(s->epfd,EPOLL_CTL_MOD,s->conn_fd,&ev);
    pthread_detach(pthread_self());
    pthread_exit(0);
    return 0;
}
int main()
{
    signal(SIGPIPE,SIG_IGN);
    List_Init(list,people_node_t);
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
                ev.events =EPOLLIN | EPOLLONESHOT | EPOLLRDHUP;
                epoll_ctl(epfd,EPOLL_CTL_ADD,conn_fd,&ev);

            }
/*       
*/
            else{
                temp->epfd=epfd;
                temp->conn_fd=events[i].data.fd;
                pth1=pthread_create(&pth1,NULL,solve,temp);
            }
        }
    }
    close(sock_fd);
    List_Destroy(list,people_node_t);
}
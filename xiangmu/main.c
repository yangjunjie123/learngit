#include"fun.h"
int main(int argc,char**argv)
{
	if(argc!=4)
	{
		printf("error args\n");
		return -1;
	}
	int count =atoi(argv[3]);//创建的进程数目
	pcdata p=(pcdata)calloc(count,sizeof(cdata));//为子进程管理
	make_child(p,count);
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	set_init(sfd,argv[1],argv[2]);
    listen(sfd,count);
	int epfd;
	epfd=epoll_create(1);
    struct epoll_event event,*evs;
	evs=(struct epoll_event*)calloc(count+1,sizeof(event));
    memset(&event,0,sizeof(event));
	event.events=EPOLLIN;
	event.data.fd=sfd;
	int ret;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	if(-1==ret)
	{
		perror("epoll_ctl");
		return -1;
	}
	int i;
	for(i=0;i<count;i++)
	{
		event.events=EPOLLIN;
		event.data.fd=p[i].fdw;
		epoll_ctl(epfd,EPOLL_CTL_ADD,p[i].fdw,&event);
	}
	int j,ret1,newfd;
    int flag=0;
	char recbuf[50]={0};
	memset(recbuf,0,sizeof(recbuf));
   	while(1)
	{   
		memset(evs,0,(count+1)*sizeof(event));
		ret=epoll_wait(epfd,evs,count+1,-1);
		if(ret>0)
		{
			for(i=0;i<ret;i++)
			{
				if(evs[i].data.fd==sfd) //如果有客服端连接
				{
                  newfd=accept(sfd,NULL,NULL);
				  for(j=0;j<count;j++)
				  {
					  if(p[j].busy==0)
					  {  send_fd(p[j].fdw,newfd);
				         p[j].busy=1;
						  break;
					  }
                  }
				  
				  close(newfd);//并不会断开连接


                 }
				for(j=0;j<count;j++)
				{
					if(evs[i].data.fd==p[j].fdw)
					{ read(p[j].fdw,&flag,sizeof(flag));
						printf("run successfully is 1 %d\n",flag);
						p[j].busy=0;
						printf("run ok\n");
					}
				}
			}
		}
	}



  close(sfd);
  close(epfd);
	
	return 0;
}

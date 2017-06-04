#include"fun.h"
void handle_request(int fd)
{
	int newfd;
	int flag=1;
	while(1)
	{
		recv_fd(fd,&newfd);
		printf("newfd=%d\n",newfd);
		choice(newfd);
		write(fd,&flag,sizeof(flag));//告诉父进程，干完活了

	}
}

void make_child(pcdata p, int count)
{
	int fds[2];
	int i;
	pid_t pid;
	for(i=0;i<count;i++)
	{
		socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		pid=fork();
		if(0==pid)
		{
			close(fds[1]);
			handle_request(fds[0]);
		}
		p[i].pid=pid;
		p[i].fdw=fds[1];
		p[i].busy=0;
		close(fds[0]);

	
	}   
}

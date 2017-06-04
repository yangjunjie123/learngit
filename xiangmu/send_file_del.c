#include"fun.h"
int send_file(int newfd)
{
	td t;
	memset(&t,0,sizeof(t));
	t.len=strlen(FILENAME);
	strcpy(t.buf,FILENAME);
	int ret;
	ret=send(newfd,&t,4+t.len,0);
    if(-1==ret)
	{
		printf("filename failed\n");
		return -1;
	}
	int fd;
	fd =open(FILENAME,O_RDONLY);
	if(-1==fd)
	{
		perror("open");
		return -1;
	}
	while( memset(&t,0,sizeof(t)),(t.len=read(fd,t.buf,sizeof(t.buf)))>0)
      
	{
		send_n(newfd,(char*)&t,4+t.len);    //有两种情况 一个是发的过快 接的过慢 一个是发的过慢 接的过快 造成缓冲区的问题

	}
    t.len=0;
	send(newfd,&t.len,4,0);


	close(newfd);//在这里close证明客户端真的关闭了
}

#include"fun.h"
int set_init(int fd,char*ip,char*port)
{
	struct sockaddr_in  xia;
	memset(&xia,0,sizeof(xia));
	xia.sin_family=AF_INET;
	xia.sin_port=htons(atoi(port));//port转网络字节序
	xia.sin_addr.s_addr=inet_addr(ip);//ip地址转网络字节序
	int ret;
	ret=bind(fd,(struct sockaddr*)&xia,sizeof(xia));//绑定ip地址和端口号
	if(-1==ret)
	{
		perror("bind");
		return -1;
	}
}

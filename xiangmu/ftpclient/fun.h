#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<string.h>
#include<signal.h>
#include<time.h>
#include<sys/time.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<sys/epoll.h>
#include<sys/uio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<shadow.h>
#include<errno.h>
#include<crypt.h>
#define FILENAME "file"
#define ROOT "/home/baifeng/ftpserver"
#define ROOTL "/home/baifeng/ftpserver/"
typedef struct childdata{
	pid_t pid;
	int fdw;
	short busy;
}cdata,*pcdata;
void make_child(pcdata,int);
void handle_request(int fd);
int set_init(int,char*,char*);
int  send_file(int );
typedef struct tdata{
	int len;
	
	char buf[1000];//不要大于64k
}td,*ptd;
void send_n(int fd ,char*buf,int len);
void recv_n(int fd ,char*buf,int len);
typedef struct mingling{
	int mingling;
	int len;
	char buf[100];
}ml,*pml;
void get_salt(char*,char*);

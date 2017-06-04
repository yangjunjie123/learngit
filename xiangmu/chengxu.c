#include"fun.h"
int choice(int fd)
{   int j=0;//循环计数
	ml t,s;//命令结构体
	td x,a,que,pass,con;//用户名 盐值
	char errorbuf[50]="输入错误，请重新输入\n";
	char mlbuf[50]={0};
	char home[50]={0};//根目录
	memset(home,0,sizeof(home));
	getcwd(home,sizeof(home));
	char salt[50]={0};
	char error1buf[50]="error1";
	char error2buf[50]="error2";
    char successbuf[50]="login success";
	int retu;
	struct spwd*sp;
	int logfd;
	logfd=open("log.txt",O_WRONLY|O_APPEND);
    close(1);
	int fd2=dup(logfd);
	time_t raw;
	struct tm *timeinfo;
	time(&raw);
		
		printf("-----------------------三次元分割线------------------------------\n");
		printf(" time:%s",ctime(&raw));
	void time()
	{
		/*time(&raw);
		
		printf(" time:%s",ctime(&raw));*/
	}
     
	while(1)
	
	{
label:		memset(&x,0,sizeof(x));
	recv_n(fd,(char*)&x.len,4);
	recv_n(fd,x.buf,x.len);//接受用户名
      time();	  
	  printf("用户%s试图登录\n",x.buf);
	  memset(salt,0,sizeof(salt));
	    if((sp=getspnam(x.buf))==NULL)
	{  memset(&a,0,sizeof(a));
	   a.len=strlen(error1buf);
	   strcpy(a.buf,error1buf);
	   send_n(fd,(char*)&a,4+a.len);
	   
	   goto label;
	}	
	  get_salt(salt,sp->sp_pwdp);
	 memset(&a,0,sizeof(a));
     a.len=strlen(salt);
	 strcpy(a.buf,salt);
	send_n(fd,(char*)&a,4+a.len);//发送盐值

    
     memset(&pass,0,sizeof(pass));
	 recv_n(fd,(char*)&pass.len,4);
	 recv_n(fd,pass.buf,pass.len);
	
	 if(strcmp(sp->sp_pwdp,pass.buf)==0)
	 {        memset(&con,0,sizeof(con));
	   con.len=strlen(successbuf);
	   strcpy(con.buf,successbuf);
	   send_n(fd,(char*)&con,con.len+4);
	   time();printf("用户%s登录成功\n",x.buf);
	
	 	 while(1)
	   { 
           memset(&t,0,sizeof(t));
		   memset(&s,0,sizeof(s));
		   memset(mlbuf,0,sizeof(mlbuf));
           recv_n(fd,(char*)&t.mingling,4);
		   recv_n(fd,(char*)&t.len,4);
		   recv_n(fd,t.buf,t.len);
		   

		   switch(t.mingling)
		   {
			 case 1:runcd(fd,t.buf,home);time();printf("cd  %s\n",t.buf);break;
			 case 2:runls(fd);time();printf("ls  \n");break;
		     case 3:runputs(fd,t.buf);time();printf("puts %s\n",t.buf);break;
			 case 4:runget(fd,t.buf);time();printf("gets %s\n",t.buf);break;
			 case 5:runremove(fd,t.buf);time();printf("remove %s \n",t.buf);break;
			 case 6:runpwd(fd,home);time();printf("pwd \n");break;

			 case 7:break;

	   
			 default:break;
		   };
	    }
      }else{
		   memset(&con,0,sizeof(con));
		   con.len=strlen(error2buf);
		   strcpy(con.buf,error2buf);
		   send_n(fd,(char*)&con,con.len+4);
		   printf("登录失败\n");
	  }
	}
}

void get_salt(char*salt,char*passwd)
{
	int i,j;
	//取出salt,i记录密码字符下标，j记录$出现次数
	for (i=0,j=0;passwd[i]&&j!=3;++i)
	{
		if(passwd[i]=='$')
			++j;
	}
	strncpy(salt,passwd,i-1);
}


int runpwd(int fd,char buf[])
{   
	td t,s;//传输结构体；
	char buf1[50]={0};
	char buf2[50]={0};
	memset(&t,0,sizeof(t));
	memset(&s,0,sizeof(s));
	memset(buf1,0,sizeof(buf1));
	;memset(buf2,0,sizeof(buf2));
	char buf3[50]="不能显示上级目录";
	int i;
	getcwd(buf1,sizeof(buf1));


	if(strcmp(buf1,buf)>0)
	{ 
		for(i=strlen(buf);i<strlen(buf1);i++)
		{
			buf2[i-strlen(buf)]=buf1[i];
		}
	    t.len=strlen(buf2);
		strcpy(t.buf,buf2);
		send_n(fd,(char*)&t.len,4+t.len);

	}else if(strcmp(buf1,buf)==0){//已经证明这个是这个是正确的
		s.len=1;
		strcpy(s.buf,"/");
		 send_n(fd,(char*)&s,4+s.len);
	}else{
	     t.len=strlen(buf3);
		 strcpy(t.buf,buf3);
		 send_n(fd,(char*)&t,4+t.len);
	}
	return 0;
}
int runget(int fd ,char buf[])//传入上方的第二个字符串

{	   
    
	
	
	td t,s;
	memset(&t,0,sizeof(t));
	memset(&s,0,sizeof(s));
	char buf3[50]="输入格式不对或无此文件\n";
	int i,openid; 

		 openid=open(buf,O_RDONLY);
		 if(-1==openid)
		 {
			 perror("open");
            
			
			 t.len=strlen(buf3);
			 strcpy(t.buf,buf3);
			 send_n(fd,(char*)&t,4+t.len);
			 return-1;
		 }else{
			 t.len=strlen(buf);
			 strcpy(t.buf,buf);
		     send_n(fd,(char*)&t,4+t.len);
		 }
        while(memset(&s,0,sizeof(s)),(s.len=read(openid,s.buf,sizeof(s.buf)))>0)
	     {
	 	   send_n(fd,(char*)&s,4+s.len);
	     }
		memset(&s,0,sizeof(s));
	     s.len=0;
	     send_n(fd,(char*)&s.len,4);
		 return 0;
}
int runcd(int fd,char buf[],char home[])
{  td t,s;
	memset(&t,0,sizeof(t));
	memset(&s,0,sizeof(t));
	char buf1[50]={0};
	char pbuf[50]={0};	char buf3[50]="输入目录格式不对\n";
	char buf2[50]="禁止访问服务器上级目录\n";
	char buf4[50]="切换成功\n";
	memset(buf1,0,sizeof(buf1));
	memset(pbuf,0,sizeof(pbuf));
	strcpy(pbuf,home);
	strcat(pbuf,"/..");
	int i,ret;
	getcwd(buf1,sizeof(buf1));


	   strcat(buf1,"/");
	   
	  strcat(buf1,buf);
 // runcd();
     if(!(strcmp(pbuf,buf1)==0))
       { ret= chdir(buf1);
	    if(-1==ret)
	    { t.len=strlen(buf3);
		  strcpy(t.buf,buf3);
		  send_n(fd,(char*)&t,4+t.len);
		  perror("chdir");
	      return -1;//缺少发送目录的参数
	   }else{
			t.len=strlen(buf4);
			strcpy(t.buf,buf4);
			send_n(fd,(char*)&t,4+t.len);
		}

	  }else{
		  s.len=strlen(buf2);
		strcpy(s.buf,buf2);
		send_n(fd,(char*)&s,4+s.len);
	
	  }
	 
	
}
int runls(int fd)
{
  td t,s;
  memset(&t,0,sizeof(t));
  memset(&s,0,sizeof(s));
  char buf1[50]={0};
  memset(buf1,0,sizeof(buf1));
  char recvbuf[50]={0};
  memset(recvbuf,0,sizeof(recvbuf));
  char errorbuf[50]="打开当前目录失败\n";
  struct stat buf;
  DIR*dir;
  getcwd(recvbuf,sizeof(recvbuf));
  dir=opendir(recvbuf);
  if(NULL==dir)
  {   t.len=strlen(errorbuf);
	  strcpy(t.buf,recvbuf);
	  send_n(fd,(char*)&t,4+t.len);
	  perror("opendir");
	  return -1;
  }
  struct dirent*p;
  int ret;
  char bigbuf[200]={0};
     while((p=readdir(dir))!=NULL)
	{
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,".."))
		{}else{
				memset(&buf,0,sizeof(buf));
				ret=stat((p->d_name),&buf);
				if(-1==ret)
				{
					perror("stat1");
				}
				// runcd();
			   char *a=ctime(&buf.st_mtime);
               int i;char b[25]={0};
               for(i=4;i<13;i++)
               {b[i-4]=*(a+i);
               } 
               char ab[]="rwxrwxrwx";
               char b2[10]={0};
               int j;
               int a1=buf.st_mode;
               for(j=9;j>0;j--)
               { if(a1&1<<j)
                 {b2[9-j]=ab[8-j];
                 }
                else{
                 b2[9-j]='_';
                 } 
               }
			   memset(bigbuf,0,sizeof(bigbuf));
             sprintf(bigbuf,"%s %d %8s %8s %7ld   %8s  %10s ",b2,buf.st_nlink,getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,buf.st_size,b,p->d_name);
			t.len=strlen(bigbuf);
			strcpy(t.buf,bigbuf);
			send_n(fd,(char*)&t,4+t.len);
                             
			  }
			}

       memset(&s,0,sizeof(s));
	   s.len=0;
	   send_n(fd,(char*)&s.len,4);
  return 0;
}

int runputs(int fd,char buf[])//后面的字符串
{
    td t,s;
	memset(&t,0,sizeof(t));
	memset(&s,0,sizeof(t));
		
	recv_n(fd,(char*)&t.len,4);
	recv_n(fd,t.buf,t.len);
	int putfd;
	putfd=open(buf,O_RDWR|O_CREAT,0666);
	if(-1==putfd)
	{
		perror("open");
		return -1;
	}
	while(1)
		
	{ 
	      recv_n(fd,(char*)&s.len,4);
		 memset(s.buf,0,sizeof(s.buf));
		 if(s.len>0)
		 {
			 recv_n(fd,s.buf,s.len);
			 write(putfd,s.buf,s.len);
		 }else{
			 break;
		 }

		
	     	    
	}	
	
	return 0;
}
int runremove(int fd,char*buf)//传第二个参数，进行拼接，只能删除文件；后面添加删除目录的选项
{
    td t,s;
	memset(&t,0,sizeof(t));
	memset(&s,0,sizeof(t));
	char buf1[50];
	char buf3[50]="删除失败\n";
	char buf2[50]="删除成功\n";
	memset(buf1,0,sizeof(buf1));
	
	int ret;
	
	      ret=unlink(buf);
	      if(-1==ret)
	      { 
		    perror("unlink");
			s.len=strlen(buf3);
			strcpy(s.buf,buf3);
			send_n(fd,(char*)&s,4+s.len);
		    return -1;//缺少发送目录的参数
	      }else{
		   s.len=strlen(buf2);
		   strcpy(s.buf,buf2);
		   send_n(fd,(char*)&s,4+s.len);
		  }
  }


   



   
			   
	 








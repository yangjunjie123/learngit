#include"fun.h"
int main(int argc, char**argv)
{
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
        int sfd;
	int ret;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in xia;
	memset(&xia,0,sizeof(xia));
	xia.sin_family=AF_INET;
    xia.sin_port=htons(atoi(argv[2]));
	xia.sin_addr.s_addr=inet_addr(argv[1]);//ip地址转网络字节序
    ret=connect(sfd,(struct sockaddr*)&xia,sizeof(xia));
	if(-1==ret)
	{
		perror("connect");
		return -1;
	}else{
		system("clear");
		printf("连接服务器成功\n");
	}
	ml t,s;//命令结构体
	td x,a,que,salt,inpass,con;//传输结构体
	
	char sendbuf[120]={0};
	char buf1[50]={0};
	char buf2[50]={0};
    int i,j,k;
	char errorbuf[50]="fail input\n";
        char account[50]={0};
        char inpasswd[50]={0};
        char onpasswd[50]={0};
		sleep(1);
		        while(1) 
       {
label:     system("clear");
		   printf("请输入用户名\n");
          memset(account,0,sizeof(account));
          read(0,account,sizeof(account));
                    memset(&que,0,sizeof(que));
          que.len=strlen(account)-1;
          strcpy(que.buf,account);
          send_n(sfd,(char*)&que,4+que.len);//发送用户名
		  
          recv_n(sfd,(char*)&salt.len,4);
          recv_n(sfd,salt.buf,salt.len);
		  if(strcmp(salt.buf,"error1")==0)
		  {printf("没有此用户，请重新输入\n");
			  sleep(1);
			  goto label;
		  }
         
          printf("请输入密码\n");
          memset(&onpasswd,0,sizeof(onpasswd));
          read(0,onpasswd,sizeof(onpasswd));
		
          
		  onpasswd[strlen(onpasswd)-1]='\0';
          memset(&inpasswd,0,sizeof(inpasswd));
          strcpy(inpasswd,crypt(onpasswd,salt.buf));
		 
          inpass.len=strlen(inpasswd);
          strcpy(inpass.buf,inpasswd);
          send_n(sfd,(char*)&inpass,4+inpass.len);//发送密文
          memset(&con,0,sizeof(con));
          recv_n(sfd,(char*)&con.len,4);
          recv_n(sfd,con.buf,con.len);
          if(strcmp(con.buf,"login success")==0)
          {  printf("登录成功\n"); 
		while(1)
	   {   int sum=0;
		int pos=0;
		memset(&t,0,sizeof(t));
	    memset(&s,0,sizeof(s));
		memset(buf1,0,sizeof(buf1));
		memset(buf2,0,sizeof(buf2));
		memset(sendbuf,0,sizeof(sendbuf));
		memset(&x,0,sizeof(x));
		memset(&a,0,sizeof(a));
		read(0,sendbuf,sizeof(sendbuf));
	    for(i=0;i<strlen(sendbuf);i++)
		{
			if(sendbuf[i]==' ')
			{
				sum++;
				pos=i;
			}
		}
		if(sum==1)
		{
			for(j=0;j<pos;j++)
            {
				buf1[j]=sendbuf[j];
			}
			for(k=pos+1;k<strlen(sendbuf)-1;k++)
			{   
                buf2[k-pos-1]=sendbuf[k];
			}
			}else if(sum==0)
		{
			memset(buf1,0,sizeof(buf1));
		 for(j=0;j<strlen(sendbuf)-1;j++)
			{
			    buf1[j]=sendbuf[j];
			}
		}


		if(!strcmp(buf1,"cd"))
		{    system("clear");
			t.mingling=1;
			t.len=strlen(buf2);
			strcpy(t.buf,buf2);
			send_n(sfd,(char*)&t,8+t.len);
			 recv_n(sfd,(char*)&x.len,4);
			recv_n(sfd,x.buf,x.len);
             printf("%s\n",x.buf);
			
	    }else if(!strcmp(buf1,"ls"))
		{    system("clear");
			t.mingling=2;
			t.len=strlen(buf2);
			strcpy(t.buf,buf2);
			send_n(sfd,(char*)&t,8+t.len);
			while(1)
		  {
				memset(&x,0,sizeof(x));
				recv_n(sfd,(char*)&x.len,4);
				if(x.len>0)
				{
					recv_n(sfd,x.buf,x.len);
					printf("%s\n",x.buf);
				}else{
					break;
				}
			}


		}else if(!strcmp(buf1,"puts"))
		{   system("clear");
			t.mingling=3;
			t.len=(strlen(buf2));
			strcpy(t.buf,buf2);
			send_n(sfd,(char*)&t,8+t.len);
		       
            	x.len=strlen(buf2);
			strcpy(x.buf,buf2);
			send_n(sfd,(char*)&x,4+t.len);//发送名字	
			int putpid;
			putpid=open(buf2,O_RDONLY);
			if(-1==putpid)
			{
				perror("open");
				return -1;
			}
		     while(memset(&a,0,sizeof(a)),(a.len=read(putpid,a.buf,sizeof(a.buf)))>0)
			{  
				
				send_n(sfd,(char*)&a,a.len+4);
			}
			memset(&a,0,sizeof(a));
			a.len=0;
			send_n(sfd,(char*)&a.len,4);
			printf("上传成功\n");

		}else if(!strcmp(buf1,"gets"))
		{   system("clear");
			t.mingling=4;
			t.len=strlen(buf2);
			strcpy(t.buf,buf2);
			send_n(sfd,(char*)&t,8+t.len);
			recv_n(sfd,(char*)&x.len,4);
			recv_n(sfd,x.buf,x.len);
			if(!strcmp(x.buf,buf2))
			{
				int recid;
				recid=open(x.buf,O_RDWR|O_CREAT,0666);
				if(-1==recid)
				{
					perror("");
					return -1;
				}
				while(1)
				{
					recv_n(sfd,(char*)&x.len,4);
					memset(x.buf,0,sizeof(x.buf));
					if(x.len>0)
					{
						recv_n(sfd,x.buf,x.len);
						write(recid,x.buf,x.len);
					}else
						printf("下载成功\n");
						break;
					}
		   }
			else{
				recv_n(sfd,(char*)&a.len,4);
				recv_n(sfd,(char*)&a,4+a.len);
				printf("%s\n",a.buf);
			}

				
		}else if(!strcmp(buf1,"remove"))
		{   
			t.mingling=5;
			t.len=strlen(buf2);
			strcpy(t.buf,buf2);
			send_n(sfd,(char*)&t,8+t.len);
			recv_n(sfd,(char*)&x.len,4);
			recv_n(sfd,x.buf,x.len);
			printf("%s\n",x.buf);
				}else if(!strcmp(buf1,"pwd"))
		{   system("clear");
			memset(&t,0,sizeof(t));
			memset(&x,0,sizeof(x));
			t.mingling=6;
			t.len=strlen(buf2);
			strcpy(t.buf,buf2);
			send_n(sfd,(char*)&t,8+t.len);
	        recv_n(sfd,(char*)&x.len,4);
			recv_n(sfd,x.buf,x.len);
			printf("当前目录地址为%s\n",x.buf);

		}else {
			t.mingling=7;
			t.len=strlen(errorbuf);
			strcpy(t.buf,errorbuf);
			send_n(sfd,(char*)&t,8+t.len);
			printf("请重新输入\n");
		}
	
	
         }
		  }else{
			    printf("登录失败,请重新输入\n");
			    sleep(1);
				goto label;
           }
	   }
	}
          


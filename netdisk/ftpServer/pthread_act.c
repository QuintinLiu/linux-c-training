#include"head.h"
int pthread_act(int new_fd,char *name,char *ip,int port,char *order1,char *order2)
{
	int len;
	int ret;
	char token[50]={0};
	char clitoken[50]={0};
	char act[30];
	//接收用户名
	ret=recv(new_fd,&len,4,0);
	error_check(-1,ret,"recv name error");
	ret=recv(new_fd,name,len,0);
	error_check(-1,ret,"recv name error");
	//查询token
	query_token(name,token);
	//接收token
	ret=recv(new_fd,&len,4,0);
	error_check(-1,ret,"recv token error");
	ret=recv(new_fd,clitoken,len,0);
	error_check(-1,ret,"recv token error");
	if(strcmp(token,clitoken)==0)
	{
		printf("varify success\n");
		len=1;
		ret=send(new_fd,&len,4,0);
		error_check(-1,ret,"send 1 error");
		if(strcmp(order1,"puts")==0)
		{
			recv_n(new_fd,(char *)&len,4);
			if(len>0)
			{
				recv_n(new_fd,order2,len);
				act_puts(new_fd,name,order2);
			}
			bzero(act,sizeof(act));
			sprintf(act,"%s%s%s",order1," ",order2);
			printf("%s\n",act);
			insert_log(ip,port,name,act);
		}
		else if(strcmp(order1,"gets")==0)
		{
			recv_n(new_fd,(char *)&len,4);
			recv_n(new_fd,order2,len);
			bzero(act,sizeof(act));
			sprintf(act,"%s%s%s",order1," ",order2);
			printf("%s\n",act);
			insert_log(ip,port,name,act);
			act_gets(new_fd,name,order2);
		}
		return 0;
	}
	else
	{
		len=0;
		ret=send(new_fd,&len,4,0);
		error_check(-1,ret,"send 0 error");
		return -1;
	}
}

#include"head.h"
int act_puts(int sfd,char *filename)
{
	data d;
	bzero(&d,sizeof(d));
	recv_n(sfd,(char *)&d.len,4);
	recv_n(sfd,(char *)&d.buf,d.len);
	if(strcmp(d.buf,"you have this filename")==0)
	{
		puts(d.buf);
		return 0;
	}
	else 
	{
		bzero(&d,sizeof(d));
		printf("initializing...\n");
		get_md5(filename,d.buf);
		d.len=strlen(d.buf);
		//发送md5
		send_n(sfd,(char *)&d,4+d.len);
		bzero(&d,sizeof(d));
		recv_n(sfd,(char *)&d.len,4);
		recv_n(sfd,(char *)&d.buf,d.len);
		if(strcmp(d.buf,"load")==0)
		{
			printf("send file start\n");
			send_file(sfd,filename);
			printf("send file done\n");
			bzero(&d,sizeof(d));
			recv_n(sfd,(char *)&d.len,4);
			recv_n(sfd,(char *)&d.buf,d.len);
			puts(d.buf);
		}
		else
		{
			puts(d.buf);
		}
	}
	return 0;
}
int act_sign(int sfd,char *name)
{
	int ret;
	char *passwd;
	data d;
	char salt[20]={0};
	//发送用户名
	printf("input the account name:");
	bzero(&d,sizeof(d));
	scanf("%s",d.buf);
	strcpy(name,d.buf);
	d.len=strlen(d.buf);
	ret=send_n(sfd,(char *)&d, d.len+4);
	if(ret==-1)
	{
		return -1;
	}
	//发送md5
	passwd=getpass("input the passwd");
	strcpy(salt,"$6$");
	produce_salt(salt+3,8);
	puts(salt);
	bzero(&d,sizeof(d));
	strcpy(d.buf,crypt(passwd,salt));
	puts(d.buf);
	d.len=strlen(d.buf);
	ret=send_n(sfd,(char *)&d,4+d.len);
	if(ret==-1)
	{
		return -1;
	}
	//接收结果
	bzero(&d,sizeof(d));
	ret=recv_n(sfd,(char *)&d.len,4);
	ret=recv_n(sfd,(char *)&d.buf,d.len);
	puts(d.buf);
	if(strcmp(d.buf,"sign successful")==0)
	{
		return 0;
	}
	else 
	{
		return -1;
	}
}
int act_login(int sfd,char *name)
{
	int ret;
	data d;
	char salt[50]={0};
	char *passwd;
	printf("input the account name:");
	bzero(&d,sizeof(d));
	scanf("%s",d.buf);
	strcpy(name,d.buf);
	passwd=getpass("input the password:");
	//发送用户名
	d.len=strlen(d.buf);
	ret=send_n(sfd,(char *)&d,4+d.len);
	if(ret==-1)
	{
		return -1;
	}
	//接收颜值
	ret=recv_n(sfd,(char *)&d.len, 4);
	ret=recv_n(sfd,(char *)salt,d.len);
	if(strcmp(salt,"wrong")==0)
	{
		printf("no such account\n");
		return -1;
	}
	bzero(&d,sizeof(d));
	strcpy(d.buf,crypt(passwd,salt));
	//发送md5
	d.len=strlen(d.buf);
	ret=send_n(sfd,(char *)&d,4+d.len);
	if(ret==-1)
	{
		return -1;
	}
	//接收验证结果
	bzero(&d,sizeof(d));
	ret=recv_n(sfd,(char *)&d.len,4);
	ret=recv_n(sfd,(char *)&d.buf,d.len);
	puts(d.buf);
	if(strcmp(d.buf,"login successful")==0)
	{
		return 0;
	}
	else 
	{
		return -1;
	}
}

#include"head.h"
char *key;
void *pthread_func1(void *r)
{
	int ret,len;
	lkinfo link;
	memcpy(&link,(lkinfo *)r,sizeof(lkinfo));
	ret=query_filedown(link.name,link.order2,link.offset,&link.point);
	if(ret==-1)
	{
		link.point=0;
		insert_filedown(link.name,link.order2,link.offset,link.size,link.point);
	}
	//连接
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_addr.s_addr=inet_addr(link.ip);
	ser.sin_port=htons(link.port);
	ret=connect(sfd,(struct sockaddr*)&ser,sizeof(struct sockaddr));
	if(ret<0)
	{
		perror("connect error");
		pthread_exit(NULL);
	}
	//发送link
	len=sizeof(lkinfo);
	send(sfd,&len,4,0);
	send_n(sfd,(char *)&link,len);
	//发送token
	char token[50]={0};
	pid_t pid=getpid();
	get_token(pid,key,token);
	len=strlen(token);
	send(sfd,&len,4,0);
	send_n(sfd,token,len);
	recv(sfd,&len,4,0);
	if(len==1)
	{
		//接收文件大小
		off_t file_size;
		off_t last=0;
		off_t recved=0;
		recv(sfd,&file_size,sizeof(off_t),0);
		int fd=open(link.order2,O_RDWR|O_CREAT,0660);
		error_check(-1,fd,"open error");
		ret=ftruncate(fd,file_size);
		printf("offset=%ld point=%ld start recv\n",link.offset,link.point);
		char *p=mmap(NULL,file_size,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
		error_check((char *)-1,p,"mmap error");
		char *q=p+link.offset+link.point;
		while(1)
		{
			ret=recv(sfd,&len,4,0);
			if(ret>0&&len>0)
			{
				ret=recv_n(sfd,q,len);
				if(ret<0)
				{
					break;
				}
				q+=len;
				recved+=len;
				if((recved-last)>=(1024*1024*20))
				{	
					update_filedown(link.name,link.order2,link.offset,recved+link.point);
					last=recved;
				}
			}
			else
			{
				break;
			}
		}
		if(recved==link.size-link.point)
		{
			update_filedown(link.name,link.order2,link.offset,link.size);
		}
		ret=munmap(p,file_size);
		error_check(-1,ret,"munmap error");
		printf("offset=%ld point=%ld recv done\n",link.offset,link.point);
	}
	else
	{
		printf("varify failed\n");
		pthread_exit(NULL);
	}
}
void *pthread_func(void *q)
{
	int ret;
	lkinfo *p=(lkinfo *)q;
	char ip[20]={0};
	short port;
	strcpy(ip,p->ip);
	port=p->port;
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_addr.s_addr=inet_addr(ip);
	ser.sin_port=htons(port);
	ret=connect(sfd,(struct sockaddr *)&ser,sizeof(struct sockaddr));
	error_check(-1,ret,"connect");

	int len;
	data d;
	pid_t pid;
	char order1[10]={0};
	strcpy(order1,p->order1);
	char order2[20]={0};
	strcpy(order2,p->order2);
	//发送order1
	len=strlen(order1);
	ret=send(sfd,&len,4,0);
	if(ret==-1||ret==0)
	{
		goto end;
	}
	ret=send(sfd,order1,len,0);
	if(ret==-1||ret==0)
	{
		goto end;
	}
	//发送用户名
	len=strlen(p->name);
	ret=send(sfd,&len,4,0);
	if(ret==-1||ret==0)
	{
		goto end;
	}
	ret=send(sfd,p->name,len,0);
	if(ret==-1||ret==0)
	{
		goto end;
	}
	//发送token
	char token[50]={0};
	pid=getpid();
	get_token(pid,key,token);
	len=strlen(token);
	ret=send(sfd,&len,4,0);
	if(ret==-1||ret==0)
	{
		goto end;
	}
	ret=send(sfd,token,len,0);
	if(ret==-1||ret==0)
	{
		goto end;
	}
	recv(sfd,&len,4,0);
	if(len==1)
	{
		if(strcmp(order1,"gets")==0)
		{
			len=strlen(order2);
			send(sfd,(char *)&len,4,0);
			send(sfd,order2,len,0);
			bzero(&d,sizeof(d));
			recv(sfd,(char *)&d.len,4,0);
			recv(sfd,(char *)d.buf,d.len,0);
			if(strcmp(d.buf,"download")==0)
			{
				printf("recv file ....\n");
				recv_file(sfd,order2);
				printf("recv file done\n");
			}
			else
			{
				puts(d.buf);
			}
		}
		else if(strcmp(order1,"puts")==0)
		{
			ret=open(order2,O_RDONLY);
			if(ret==-1)
			{
				perror("open error");
				len=0;
				send(sfd,(char *)&len,4,0);
				goto end;
			}
			close(ret);
			len=strlen(order2);
			ret=send(sfd,(char *)&len,4,0);
			ret=send(sfd,order2,len,0);
			act_puts(sfd,order2);
		}
	}
end:
	close(sfd);
	pthread_exit(NULL);
}
int main(int argc,char **argv)
{
	int ret;
	char ip[20]={0};
	char port[8]={0};
	FILE *fp;
	fp=fopen("../conf/server.conf","r");
	ret=fscanf(fp,"%s%s",ip,port);
	error_check(-1,ret,"fopen");
	fclose(fp);
	printf("%s %s\n",ip,port);

	int sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_addr.s_addr=inet_addr(ip);
	ser.sin_port=htons(atoi(port));
	ret=connect(sfd,(struct sockaddr *)&ser,sizeof(struct sockaddr));
	error_check(-1,ret,"connect");

	pthread_t pth_id;
	int len;
	data d;
	pid_t pid;
	key=(char *)calloc(1,50);
	char name[20]={0};
	char order1[10]={0};
	char order2[20]={0};
start:
	//system("clear");
	printf("1.login\n2.sign in\n");
	printf("select a number:");
	scanf("%d",&ret);
	bzero(&d,sizeof(d));
	if(ret==1)
	{
		strcpy(order1,"login");
		len=strlen(order1);
		ret=send(sfd,(char *)&len,4,0);
		ret=send_n(sfd,order1,len);
		if(ret==-1)
		{
			goto end;
		}
		ret=act_login(sfd,name);
		if(ret==-1)
		{
			goto start;
		}
		pid=getpid();
		send(sfd,(char *)&pid,sizeof(pid),0);
		recv(sfd,(char *)&len,4,0);
		recv(sfd,key,len,0);
	}
	else 
	{
		strcpy(order1,"sign_in");
		len=strlen(order1);
		ret=send_n(sfd,(char *)&len,4);
		ret=send_n(sfd,order1,len);
		if(ret==-1)
		{
			goto end;
		}
		ret=act_sign(sfd,name);
		if(ret==-1)
		{
			goto start;
		}
		pid=getpid();
		send(sfd,(char *)&pid,sizeof(pid),0);
		recv(sfd,(char *)&len,4,0);
		recv(sfd,key,len,0);
	}
	while(1)
	{
		bzero(order1,sizeof(order1));
		bzero(order2,sizeof(order2));
		bzero(&d,sizeof(d));
		scanf("%s",order1);
		if(strcmp(order1,"puts")==0)
		{
			scanf("%s",order2);
			lkinfo link;
			bzero(&link,sizeof(lkinfo));
			strcpy(link.name,name);
			strcpy(link.order1,order1);
			strcpy(link.order2,order2);
			strcpy(link.ip,ip);
			link.port=atoi(port);
			ret=pthread_create(&pth_id,NULL,pthread_func,&link);
			error_check(-1,ret,"pthread_create");
			continue;
		}
		len=strlen(order1);
		ret=send_n(sfd,(char *)&len,4);
		ret=send_n(sfd,order1,len);
		if(strcmp(order1,"pwd")==0)
		{
			ret=recv_n(sfd,(char *)&d.len,4);
			ret=recv_n(sfd,(char *)d.buf,d.len);
			if(ret==-1)
			{
				goto end;
			}
			puts(d.buf);
		}
		else if(strcmp(order1,"gets")==0)
		{
			pthread_t pth_id[4];
			int i;
			ctl each_info;
			lkinfo link[4];
			//读取并发送order2
			scanf("%s",order2);
			int fd=open(order2,O_RDONLY);
			if(fd==-1)
			{
				delete_filedown(name,order2);
			}
			else
			{
				close(fd);
			}
			len=strlen(order2);
			send(sfd,(char *)&len,4,0);
			send_n(sfd,order2,len);
			bzero(&d,sizeof(d));
			//接收反馈
			recv(sfd,(char *)&d.len,4,0);
			recv(sfd,(char *)d.buf,d.len,0);
			if(strcmp(d.buf,"download")==0)
			{
				for(i=0;i<4;i++)
				{
					bzero(&link[i],sizeof(lkinfo));
					strcpy(link[i].name,name);
					strcpy(link[i].order1,order1);
					strcpy(link[i].order2,order2);
					recv_n(sfd,(char *)&each_info,sizeof(each_info));
					strcpy(link[i].ip,each_info.ip);
					link[i].port=each_info.port;
					link[i].offset=each_info.offset;
					link[i].size=each_info.size;
					pthread_create(pth_id+i,NULL,pthread_func1,&link[i]);
				}
				recv(sfd,&len,4,0);
				printf("len=%d\n",len);
			}
			else
			{
				puts(d.buf);
			}
		}
		else if(strcmp(order1,"cd")==0)
		{
			scanf("%s",order2);
			len=strlen(order2);
			ret=send_n(sfd,(char *)&len,4);
			ret=send_n(sfd,order2,len);
			ret=recv_n(sfd,(char *)&d.len,4);
			ret=recv_n(sfd,(char *)d.buf,d.len);
			if(ret==-1)
			{
				goto end;
			}
			puts(d.buf);
		}
		else if(strcmp(order1,"ls")==0)
		{
			//system("clear");
			bzero(&d,sizeof(d));
			ret=recv_n(sfd,(char *)&d.len,4);
			ret=recv_n(sfd,(char *)d.buf,d.len);
			if(strcmp(d.buf,"empty")==0)
			{
				printf("its empty\n");
				continue;
			}
			printf("%-30s%-8s\n","FILENAME","TYPE");
			//off_t file_size;
			//int len;
			while(1)
			{
				bzero(&d,sizeof(d));
				ret=recv_n(sfd,(char *)&d.len,4);
				if(d.len>0)
				{
					ret=recv_n(sfd,(char *)d.buf,d.len);
					printf("%s\n",d.buf);
					/*recv_n(sfd,(char *)&len,4);
					  recv_n(sfd,(char *)&file_size,len);
					  printf("%10d\n",file_size);
					 */
				}
				else
				{
					break;
				}
			}
		}
		else if(strcmp(order1,"remove")==0)
		{
			scanf("%s",order2);
			len=strlen(order2);
			ret=send_n(sfd,(char *)&len,4);
			ret=send_n(sfd,order2,len);
			bzero(&d,sizeof(d));
			ret=recv_n(sfd,(char *)&d.len,4);
			ret=recv_n(sfd,(char *)d.buf,d.len);
			puts(d.buf);
		}
		else if(strcmp(order1,"mkdir")==0)
		{
			scanf("%s",order2);
			len=strlen(order2);
			ret=send_n(sfd,(char *)&len,4);
			ret=send_n(sfd,order2,len);
			bzero(&d,sizeof(d));
			ret=recv_n(sfd,(char *)&d.len,4);
			if(d.len>0)
			{
				ret=recv_n(sfd,(char *)d.buf,d.len);
				puts(d.buf);
			}
			else
			{
				printf("mkdir successful\n");
			}
		}
		else if(strcmp(order1,"quit")==0)
		{
			break;
		}
	}
end:
	close(sfd);
	return 0;
}

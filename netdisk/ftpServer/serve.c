#include"head.h"
#include"factory.h"
int serve(factory *fp,cliinfo *client,int *quit_flg)
{
	int ret;
	int len;
	char ip[20];
	int port;
	strcpy(ip,client->ip);
	port=client->port;
	pid_t pid;
	node *pnew;
	que_t *pque=&fp->que;
	char token[50]={0};
	char key[10]={0};
	char act[50]={0};
	char order2[30];
	char order1[10];
	strcpy(act,"connect");
	insert_log(ip,port,client->name,act);
	//接受order1
	bzero(order1,sizeof(order1));
	bzero(order2,sizeof(order2));
	ret=recv(client->new_fd,(char *)&len,4,0);
	if(ret==-1||ret==0)
	{
		*quit_flg=1;
		goto end;
	}
	ret=recv(client->new_fd,order1,len,0);
	if(ret==-1||ret==0)
	{
		*quit_flg=1;
		goto end;
	}
	if(strcmp(order1,"login")==0)
	{
		act_login(client->new_fd,client->name);
		//生成token并发送key
		recv(client->new_fd,(char *)&pid,sizeof(pid),0);
		get_md5(pid,key,token);
		len=strlen(key);
		send(client->new_fd,&len,4,0);
		send(client->new_fd,key,len,0);
		update_token(client->name,token);
		//记录日志
		bzero(act,sizeof(act));
		sprintf(act,"%s",order1);
		insert_log(ip,port,client->name,act);
	}
	else if(strcmp(order1,"puts")==0)
	{
		pnew=(node *)calloc(1,sizeof(node));
		pnew->new_fd=client->new_fd;
		strcpy(pnew->name,client->name);
		strcpy(pnew->ip,client->ip);
		pnew->port=client->port;
		strcpy(pnew->order1,order1);
		strcpy(pnew->order2,order2);
		pthread_mutex_lock(&pque->que_mutex);
		que_insert(pque,pnew);
		pthread_mutex_unlock(&pque->que_mutex);
		pthread_cond_signal(&fp->cond);
		*quit_flg=1;
	}
	else if(strcmp(order1,"gets")==0)
	{
		int ret,len;
		data d;
		char path[100]={0};
		strcpy(path,"../file_pool/");
		//接收order2
		recv(client->new_fd,&len,4,0);
		recv(client->new_fd,order2,len,0);
		printf("order2:%s\n",order2);
		ret=query_if_name_file(client->name,order2);//判断是否有这个文件或类型是否正确
		bzero(&d,sizeof(d));
		if(ret==-1)
		{
			strcpy(d.buf,"wrong order");
			d.len=strlen(d.buf);
			send_n(client->new_fd,(char *)&d,4+d.len);
		}
		else
		{
			int i;
			ctl each_info;
			off_t each_size;
			struct stat buf;
			strcpy(d.buf,"download");
			d.len=strlen(d.buf);
			send_n(client->new_fd,(char *)&d,4+d.len);
			//查询真正的文件名
			char md5[100]={0};
			query_md5(client->name,order2,md5);
			sprintf(path,"%s%s",path,md5);
			int fd=open(path,O_RDONLY);
			error_check(-1,fd,"open");
			fstat(fd,&buf);
			printf("total_size=%ld\n",buf.st_size);
			//发送每一段的控制信息
			each_size=buf.st_size/3;
			strcpy(each_info.ip,"192.168.176.128");
			each_info.port=1510;
			each_info.offset=0;
			each_info.size=each_size;
			len=sizeof(ctl);
			for(i=0;i<3;i++)
			{
				ret=send_n(client->new_fd,(char *)&each_info,len);
			printf("ip=%s  port=%u  offset=%ld  size=%ld\n",each_info.ip,each_info.port,each_info.offset,each_info.size);
				if(ret==-1)
				{
					printf("send failed\n");
					return -1;
				}
				each_info.port+=10;
				each_info.offset+=each_size;
			}
			each_info.size=buf.st_size%3;
			ret=send_n(client->new_fd,(char *)&each_info,len);
			printf("ip=%s  port=%u  offset=%ld  size=%ld\n",each_info.ip,each_info.port,each_info.offset,each_info.size);
				if(ret==-1)
				{
					printf("send failed\n");
					return -1;
				}
			len=0;
			send(client->new_fd,&len,4,0);
			printf("send each info over\n");
			close(fd);
		}
		return 0;
	}
	else if(strcmp(order1,"sign_in")==0)
	{
		act_sign(client->new_fd,client->name);
		//生成token并发送key
		recv(client->new_fd,(char *)&pid,sizeof(pid),0);
		get_md5(pid,key,token);
		len=strlen(key);
		send(client->new_fd,&len,4,0);
		send(client->new_fd,key,len,0);
		update_token(client->name,token);
		//记录日志
		bzero(act,sizeof(act));
		sprintf(act,"%s",order1);
		printf("%s\n",act);
		insert_log(ip,port,client->name,act);
	}
	else if(strcmp(order1,"pwd")==0)
	{
		bzero(act,sizeof(act));
		sprintf(act,"%s",order1);
		printf("%s\n",act);
		insert_log(ip,port,client->name,act);
		act_pwd(client->new_fd,client->name);
	}
	else if(strcmp(order1,"cd")==0)
	{
		ret=recv_n(client->new_fd,(char *)&len,4);
		ret=recv_n(client->new_fd,order2,len);
		bzero(act,sizeof(act));
		sprintf(act,"%s%s%s",order1," ",order2);
		printf("%s\n",act);
		insert_log(ip,port,client->name,act);
		act_cd(client->new_fd,client->name,order2);
	}
	else if(strcmp(order1,"ls")==0)
	{
		bzero(act,sizeof(act));
		sprintf(act,"%s%s%s",order1," ",order2);
		printf("%s\n",act);
		insert_log(ip,port,client->name,act);
		act_ls(client->new_fd,client->name);
	}
	else if(strcmp(order1,"remove")==0)
	{
		ret=recv_n(client->new_fd,(char *)&len,4);
		ret=recv_n(client->new_fd,order2,len);
		bzero(act,sizeof(act));
		sprintf(act,"%s%s%s",order1," ",order2);
		printf("%s\n",act);
		insert_log(ip,port,client->name,act);
		act_remove(client->new_fd,client->name,order2);
	}
	else if(strcmp(order1,"mkdir")==0)
	{
		ret=recv_n(client->new_fd,(char *)&len,4);
		ret=recv_n(client->new_fd,order2,len);
		bzero(act,sizeof(act));
		sprintf(act,"%s%s%s",order1," ",order2);
		printf("%s\n",act);
		insert_log(ip,port,client->name,act);
		act_mkdir(client->new_fd,client->name,order2);
	}
	else if(strcmp(order1,"quit")==0)
	{
		bzero(act,sizeof(act));
		sprintf(act,"%s",order1);
		printf("%s\n",act);
		insert_log(ip,port,client->name,act);
		*quit_flg=1;
	}
end:
	return 0;
}


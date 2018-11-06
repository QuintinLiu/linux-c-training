#include"head.h"
int act_mkdir(int new_fd,char *name,char *dirname)
{
	int ret;
	data d;
	ret=query_if_name_dir(name,dirname);
	bzero(&d,sizeof(d));
	if(ret==-1)
	{
		strcpy(d.buf,"you already have this dir");
		d.len=strlen(d.buf);
		send_n(new_fd,(char *)&d,4+d.len);
	}
	else
	{
		int curcode;
		query_curcode(name,&curcode);
		insert_dir(name,curcode,dirname);
		d.len=0;
		send_n(new_fd,(char *)&d.len,4);
	}
	return 0;
}
int act_gets(int new_fd,char *name,char *filename)
{
	int ret;
	data d;
	ret=query_if_name_file(name,filename);//判断是否有这个文件或类型是否正确
	bzero(&d,sizeof(d));
	if(ret==-1)
	{
		strcpy(d.buf,"wrong order");
		d.len=strlen(d.buf);
		send_n(new_fd,(char *)&d,4+d.len);
	}
	else
	{
		strcpy(d.buf,"download");
		d.len=strlen(d.buf);
		send_n(new_fd,(char *)&d,4+d.len);
		char md5[100]={0};
		query_md5(name,filename,md5);
		send_file(new_fd,md5);
	}
	return 0;
}
int act_puts(int new_fd,char *name,char *filename)
{
	int ret;
	int curcode;
	query_curcode(name,&curcode);
	data d;
	ret=query_if_file(name,filename);//判断是否有这个文件名
	bzero(&d,sizeof(d));
	if(ret==0)
	{
		strcpy(d.buf,"you have this filename");
		d.len=strlen(d.buf);
		send_n(new_fd,(char *)&d,4+d.len);
	}
	else 
	{
		//没有就传输md5
		strcpy(d.buf,"md5");
		d.len=strlen(d.buf);
		send_n(new_fd,(char *)&d,4+d.len);
		bzero(&d,sizeof(d));
		recv_n(new_fd,(char *)&d.len,4);
		recv_n(new_fd,d.buf,d.len);
		//判断文件池中是否有这个文件
		ret=search_file(d.buf);
		if(ret==0)
		{
			printf("search success\n");
			insert_file(name,curcode,filename,d.buf);
			bzero(&d,sizeof(d));
			strcpy(d.buf,"puts sucessful");
			d.len=strlen(d.buf);
			send_n(new_fd,(char *)&d,4+d.len);
		}
		else
		{
			char md5[100]={0};
			printf("search failed\n");
			strcpy(md5,d.buf);
			bzero(&d,sizeof(d));
			//没有就发送load使其传输
			strcpy(d.buf,"load");
			d.len=strlen(d.buf);
			send_n(new_fd,(char *)&d,4+d.len);
			//传输文件
			ret=recv_file(new_fd,md5);
			if(ret==-1)
			{
				printf("recv file failed\n");
				return -1;
			}
			printf("recv_file done\n");
			insert_file(name,curcode,filename,md5);
			bzero(&d,sizeof(d));
			strcpy(d.buf,"puts sucessful");
			d.len=strlen(d.buf);
			send_n(new_fd,(char *)&d,4+d.len);
		}
	}
	return 0;
}
int act_remove(int new_fd,char *name,char *filename)
{
	data d;
	int ret;
	ret=query_if_file(name,filename);
	bzero(&d,sizeof(d));
	if(ret==-1)
	{
		strcpy(d.buf,"no such filename");
		d.len=strlen(d.buf);
		send_n(new_fd,(char *)&d,4+d.len);
	}
	else 
	{
		ret=query_if_name_file(name,filename);
		if(ret==0)
		{
			delete_file(name,filename);
			strcpy(d.buf,"remove successful");
			d.len=strlen(d.buf);
			send_n(new_fd,(char *)&d,4+d.len);
		}
		else
		{
			int code;
			query_code(name,filename,&code);
			ret=query_if_dir_file(name,code);
			if(ret==-1)
			{
				strcpy(d.buf,"not empty");
				d.len=strlen(d.buf);
				send_n(new_fd,(char *)&d,4+d.len);
			}
			else
			{
				delete_file(name,filename);
				strcpy(d.buf,"remove successful");
				d.len=strlen(d.buf);
				send_n(new_fd,(char *)&d,4+d.len);
			}
		}
	}
	return 0;
}
int act_ls(int new_fd,char *name)
{
	int curcode;
	query_curcode(name,&curcode);
	query_file(new_fd,name,curcode);
	return 0;
}
int act_cd(int new_fd,char *name,char *filename)
{
	int ret,i;
	int len;
	int curcode,precode;
	char path[128]={0};
	if(strcmp(filename,"..")!=0)
	{
		ret=query_code(name,filename,&curcode);
		if(ret==-1)
		{
			strcpy(path,"cd failed");
			len=strlen(path);
			ret=send_n(new_fd,(char *)&len,4);
			ret=send_n(new_fd,path,len);
			return -1;
		}
		query_path(name,path);
		if(path[strlen(path)-1]=='/')
		{
			path[strlen(path)-1]='\0';
		}
		sprintf(path,"%s%s%s",path,"/",filename);
		ret=update_path_code(name,path,curcode);
	}
	else
	{
		query_curcode(name,&curcode);
		if(curcode!=0)
		{
			query_path(name,path);
			for(i=strlen(path)-1;path[i]!='/';i--)
			{
				path[i]='\0';
			}
			if(i!=0)
			{
				path[i]='\0';
			}
			query_precode(curcode,&precode);
			update_path_code(name,path,precode);
		}
	}
	bzero(path,sizeof(path));
	query_path(name,path);
	len=strlen(path);
	ret=send_n(new_fd,(char *)&len,4);
	ret=send_n(new_fd,path,len);
	return 0;
}
int act_pwd(int new_fd,char *name)
{
	int ret;
	data d;
	bzero(&d,sizeof(d));
	query_path(name,d.buf);
	d.len=strlen(d.buf);
	ret=send_n(new_fd,(char *)&d,4+d.len);
	if(ret==-1)
	{
		return -1;
	}
	return 0;
}
int act_sign(int new_fd,char *rename)
{
	int ret;
	data d;
	int len;
	char name[20]={0};
	char salt[20]={0};
	//接收用户名
	ret=recv_n(new_fd,(char *)&len,4);
	if(ret==-1)
	{
		goto end;
	}
	ret=recv_n(new_fd,name,len);
	if(ret==-1)
	{
		goto end;
	}
	strcpy(rename,name);
	//接收md5
	bzero(&d,sizeof(d));
	ret=recv_n(new_fd,(char *)&d.len,4);
	if(ret==-1)
	{
		goto end;
	}
	ret=recv_n(new_fd,d.buf,d.len);
	if(ret==-1)
	{
		goto end;
	}
	strncpy(salt,d.buf,11);
	ret=insert_account(name,salt,d.buf);
	if(ret==0)
	{
		bzero(&d,sizeof(d));
		strcpy(d.buf,"sign successful");
		d.len=strlen(d.buf);
		ret=send_n(new_fd,(char *)&d,4+d.len);
		if(ret==-1)
		{
			goto end;
		}
	}
	else
	{
		bzero(&d,sizeof(d));
		strcpy(d.buf,"sign failed");
		d.len=strlen(d.buf);
		ret=send_n(new_fd,(char *)&d,4+d.len);
		if(ret==-1)
		{
			goto end;
		}
	}

end:
	return 0;
}
int act_login(int new_fd,char *rename)
{	
	int ret;
	data d;
	char salt[50]={0};
	char md5[256]={0};
	//接收用户名
	bzero(&d,sizeof(d));
	ret=recv_n(new_fd,(char *)&d.len,4);
	if(ret==-1)
	{
		goto end;
	}
	ret=recv_n(new_fd,d.buf,d.len);
	if(ret==-1)
	{
		goto end;
	}
	strcpy(rename,d.buf);
	ret=query_if_account(rename);
	if(ret==-1)
	{
		bzero(&d,sizeof(d));
		strcpy(d.buf,"wrong");
		d.len=strlen(d.buf);
		send_n(new_fd,(char *)&d,4+d.len);
		return 0;
	}
	//获取颜值
	ret=query_salt(d.buf,salt,md5);
	if(ret==-1)
	{
		goto end;
	}
	//发送颜值
	bzero(&d,sizeof(d));
	strcpy(d.buf,salt);
	d.len=strlen(d.buf);
	ret=send_n(new_fd,(char *)&d,4+d.len);
	if(ret==-1)
	{
		goto end;
	}
	//接收md5
	bzero(&d,sizeof(d));
	ret=recv_n(new_fd,(char *)&d.len,4);
	if(ret==-1)
	{
		goto end;
	}
	ret=recv_n(new_fd,d.buf,d.len);
	if(ret==-1)
	{
		goto end;
	}
	//验证
	if(strcmp(d.buf,md5)==0)
	{
		update_path_code(rename,"/",0);
		bzero(&d,sizeof(d));
		strcpy(d.buf,"login successful");
		d.len=strlen(d.buf);
		ret=send_n(new_fd,(char *)&d,4+d.len);
		if(ret==-1)
		{
			goto end;
		}
	}
	else
	{
		bzero(&d,sizeof(d));
		strcpy(d.buf,"login failed");
		d.len=strlen(d.buf);
		ret=send_n(new_fd,(char *)&d,4+d.len);
		return -1;
	}
end:
	return 0;
}
/*
   int transfer(int new_fd)
   {
   int ret;
   data d;
   bzero(&d,sizeof(d));
   d.len=strlen(FILENAME);//发文件名
   strcpy(d.buf,FILENAME);
   ret=send_n(new_fd,(char *)&d,4+d.len);
   if(ret==-1)
   {
   goto end;
   }
   int fd=open(FILENAME,O_RDONLY);
   error_check(-1,fd,"open");
   struct stat buf;//发送文件总大小
   fstat(fd,&buf);
   d.len=sizeof(buf.st_size);
   memcpy(d.buf,&buf.st_size,sizeof(size_t));
   ret=send_n(new_fd,(char *)&d,d.len+4);
   if(ret==-1)
   {
   goto end;
   }
   while((d.len=read(fd,d.buf,sizeof(d.buf)))>0)
   {
   ret=send_n(new_fd,(char *)&d,4+d.len);
   if(ret==-1)
   {
   goto end;
   }
   }
   send_n(new_fd,(char *)&d,4+d.len);
end:
return 0;
}*/

#include"head.h"
int  send_n(int new_fd,char *p,int len)
{
	int ret;
	int total=0;
	while(total<len)
	{
		ret=send(new_fd,p+total,len-total,0);
		if(ret==-1)
		{
			return -1;
		}
		total+=ret;
	}
	return 0;
}
int recv_n(int new_fd,char *p,int len)
{
	int ret;
	int total=0;
	while(total<len)
	{
		ret=recv(new_fd,p+total,len-total,0);
		if(ret==-1)
		{
			return -1;
		}
		total+=ret;
	}
	return 0;
}
int search_file(char *filename)
{
	char path[100]="../file_pool";
	DIR *dir=opendir(path);
	struct dirent *info;
	while((info=readdir(dir))!=NULL)
	{
		if(strcmp(filename,info->d_name)==0)
		{
			return 0;
		}
	}
	return -1;
}
int send_file(int new_fd,char *filename)
{	
	int ret;
	data d;
	off_t offset;
	bzero(&d,sizeof(d));
	sprintf(d.buf,"%s%s","../file_pool/",filename);
	int fd=open(d.buf,O_RDONLY);
	error_check(-1,fd,"open");
	struct stat info;//发送文件总大小
	fstat(fd,&info);
	bzero(&d,sizeof(d));
	d.len=sizeof(info.st_size);
	memcpy(d.buf,&info.st_size,sizeof(size_t));
	ret=send_n(new_fd,(char *)&d,d.len+4);
	//接收断点
	recv(new_fd,&offset,sizeof(offset),0);
	if(offset==info.st_size)
	{
		return 0;
	}
	else
	{
		off_t left=info.st_size-offset;
		char *p=mmap(NULL,info.st_size,PROT_READ,MAP_SHARED,fd,0);
		error_check((char *)-1,p,"mmap error");
		char *q=p;
		off_t sended=0;
		int len=1024;
		q+=offset;
		while(left-sended>=1024)
		{
			ret=send(new_fd,&len,4,0);
			if(ret==-1)
			{
				printf("send failed\n");
				return 0;
			}
			ret=send(new_fd,q,len,0);
			if(ret==-1)
			{
				printf("send failed\n");
				return 0;
			}
			sended+=len;
			q+=len;
		}
		if(left-sended>0)
		{
			len=left-sended;
			ret=send(new_fd,&len,4,0);
			if(ret==-1)
			{
				printf("send failed\n");
				return 0;
			}
			ret=send(new_fd,q,len,0);
			if(ret==-1)
			{
				printf("send failed\n");
				return 0;
			}
		}
		len=0;
		send(new_fd,&len,4,0);
		printf("send file done\n");
	munmap(p,left);
	}
	return 0;
}

int recv_file(int new_fd,char *filename)
{
	int len;
	int ret;
	char buf[1024]={0};
	off_t file_size;
	double download=0;
	recv_n(new_fd,(char *)&len,4);//读文件大小
	recv_n(new_fd,(char *)&file_size,len);
	sprintf(buf,"%s%s","../file_pool/",filename);
	int fd=open(buf,O_WRONLY|O_CREAT,0660);
	error_check(-1,fd,"open");
	time_t start=time(NULL);
	time_t end=time(NULL);
	while(1)
	{
		ret=recv_n(new_fd,(char *)&len,4);
		if(ret!=-1&&len>0)
		{
			download+=len;
			ret=recv_n(new_fd,buf,len);
			write(fd,buf,len);
			end=time(NULL);
			if(end-start>=1)
			{
				printf("it has download %5.1f%c\r",download/file_size*100,'%');
				fflush(stdout);
				start=end;
			}
		}
		else
		{
			printf("it has download %5.1f%c\n",download/file_size*100,'%');
			break;
		}
	}
	close(fd);
	return 0;
}

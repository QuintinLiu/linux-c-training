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
		if(ret==0)
		{
			return -1;
		}
		total+=ret;
	}
	return 0;
}
int send_file(int new_fd,char *filename,off_t offset,off_t size,off_t point)
{	
	int ret,len;
	data d;
	bzero(&d,sizeof(d));
	sprintf(d.buf,"%s%s","../file_pool/",filename);
	int fd=open(d.buf,O_RDONLY);
	error_check(-1,fd,"open");
	struct stat info;
	fstat(fd,&info);
	send(new_fd,&info.st_size,sizeof(info.st_size),0);//发送总大小
	{
		off_t left=size-point;
		char *p=mmap(NULL,info.st_size,PROT_READ,MAP_SHARED,fd,0);
		error_check((char *)-1,p,"mmap error");
		char *q=p+offset+point;
		off_t sended=0;
		len=1024;
		while(left-sended>=1024)
		{
			ret=send(new_fd,&len,4,0);
			if(ret==-1)
			{
				printf("send failed\n");
				return 0;
			}
			ret=send_n(new_fd,q,len);
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
			ret=send_n(new_fd,q,len);
			if(ret==-1)
			{
				printf("send failed\n");
				return 0;
			}
		}
		len=0;
		send(new_fd,&len,4,0);
		ret=munmap(p,info.st_size);
		error_check(-1,ret,"munmap error");
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

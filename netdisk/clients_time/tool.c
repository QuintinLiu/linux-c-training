#include"head.h"
int produce_salt(char *salt,int n)
{
	char *charset="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int i;
	int len=strlen(charset);
	srand((unsigned int)time(NULL));
	for(i=0;i<n;i++)
	{
		salt[i]=charset[rand()%len];
	}
	return 0;
}	
int send_n(int sfd,char *p,int len)
{
	int ret;
	int total=0;
	while(total<len)
	{
		ret=send(sfd,p+total,len-total,0);
		if(ret==-1)
		{
			printf("send wrong\n");
			return -1;
		}
		total+=ret;
	}
	return 0;
}
int recv_n(int sfd,char *p,int len)
{
	int ret;
	int total=0;
	while(total<len)
	{
		ret=recv(sfd,p+total,len-total,0);
		if(ret==0)
		{
			printf("recv wrong\n");
			return -1;
		}
		total+=ret;
	}
	return 0;
}
int recv_file(int sfd,char *filename)
{	
	int len;
	int ret;
	char buf[1024]={0};
	off_t file_size;
	off_t offset;
	double download=0;
	recv_n(sfd,(char *)&len,4);//读文件大小
	recv_n(sfd,(char *)&file_size,len);
	int fd=open(filename,O_RDWR);
	if(fd==-1)
	{
		offset=0;
		send(sfd,&offset,sizeof(off_t),0);
		fd=open(filename,O_RDWR|O_CREAT,0660);
	}
	else
	{
		struct stat info;
		fstat(fd,&info);
		if(info.st_size==file_size)
		{
			offset=file_size;
			send(sfd,&offset,sizeof(off_t),0);
			return 0;
		}
		else
		{
			offset=info.st_size;
			send(sfd,&offset,sizeof(off_t),0);
			ret=lseek(fd,offset,SEEK_SET);
			error_check(-1,ret,"lseek");
		}
	}
	download+=offset;
	/*char *p=mmap(NULL,file_size,PROT_WRITE,MAP_SHARED,fd,0);
	error_check((char *)-1,p,"mmap error");
	char *q=p+offset;*/
	time_t start=time(NULL);
	time_t end=time(NULL);
	while(1)
	{
		ret=recv_n(sfd,(char *)&len,4);
		if(ret!=-1&&len>0)
		{
			download+=len;
			ret=recv_n(sfd,buf,len);
			if(ret==-1)
			{
				return -1;
			}
			write(fd,buf,len);
			/*offset+=len;
			ret=ftruncate(fd,offset);
			error_check(-1,ret,"ftruncate");
			q+=len;*/
			end=time(NULL);
			if(end-start>=1)
			{
				//printf("it has download %5.1f%c\r",download/file_size*100,'%');
				//fflush(stdout);
				start=end;
			}
		}
		else
		{
			printf("it has download %5.1f%c\n",download/file_size*100,'%');
			break;
		}
	}
	/*ret=munmap(p,file_size);
	error_check(-1,ret,"munmap error");*/
	close(fd);
	return 0;
}
int send_file(int sfd,char *filename)
{	
	int ret;
	data d;
	off_t file_size;
	bzero(&d,sizeof(d));
	int fd=open(filename,O_RDONLY);
	error_check(-1,fd,"open");
	struct stat buf;//发送文件总大小
	fstat(fd,&buf);
	d.len=sizeof(buf.st_size);
	memcpy(&file_size,&buf.st_size,sizeof(off_t));
	memcpy(d.buf,&buf.st_size,sizeof(off_t));
	ret=send_n(sfd,(char *)&d,d.len+4);

	char *p=mmap(NULL,buf.st_size,PROT_READ,MAP_SHARED,fd,0);
	error_check((char *)-1,p,"mmap error");
	int len=1024;
	char *q=p;
	double download=0;
	off_t sended=0;
	time_t start=time(NULL);
	time_t end=time(NULL);
	while(buf.st_size-sended>=1024)
	{
		download+=len;
		ret=send(sfd,&len,4,0);
		if(ret==-1)
		{
			perror("send len");
			return -1;
		}
		ret=send(sfd,q,len,0);
		if(ret==-1)
		{
			perror("send file");
			return -1;
		}
		sended+=len;
		q+=len;
		end=time(NULL);
		if(end-start>=1)
		{
			//printf("it has upload %5.1f%c\r",download/file_size*100,'%');
			//fflush(stdout);
			start=end;
		}
	}
	if(buf.st_size-sended>0)
	{
		len=buf.st_size-sended;
		ret=send(sfd,&len,4,0);
		if(ret==-1)
		{
			printf("send failed\n");
			return -1;
		}
		download+=len;
		ret=send(sfd,q,len,0);
		if(ret==-1)
		{
			printf("send failed\n");
			return -1;
		}
	}
	len=0;
	send(sfd,&len,4,0);
	printf("it has upload %5.1f%c\n",download/file_size*100,'%');
	ret=munmap(p,buf.st_size);
	error_check(-1,ret,"munmap error");
	close(fd);
	return 0;
}

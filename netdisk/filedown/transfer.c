#include"head.h"
#define FILENAME "file"
int act_gets(int new_fd,char *name,char *filename,off_t offset,off_t size,off_t point)
{
	data d;
	bzero(&d,sizeof(d));
	char md5[100]={0};
	query_md5(name,filename,md5);
	printf("offset=%ld,point=%ld start send file\n",offset,point);
	send_file(new_fd,md5,offset,size,point);
	printf("offset=%ld,point=%ld send file done\n",offset,point);
	return 0;
}
int transfer_file(int new_fd,char *ip,short port)
{
	int len;
	lkinfo link;
	char token[50]={0};
	char clitoken[50]={0};
	char act[30];
	//接收link
	bzero(&link,sizeof(lkinfo));
	recv(new_fd,&len,4,0);
	recv_n(new_fd,(char *)&link,len);
	//查询token
	query_token(link.name,token);
	//接收token
	recv(new_fd,&len,4,0);
	recv_n(new_fd,clitoken,len);
	if(strcmp(token,clitoken)==0)
	{
		printf("varify success\n");
		len=1;
		send(new_fd,&len,4,0);
		if(strcmp(link.order1,"gets")==0)
		{
			sprintf(act,"%s%s%s",link.order1," ",link.order2);
			printf("%s\n",act);
			insert_log(ip,port,link.name,act);
			act_gets(new_fd,link.name,link.order2,link.offset,link.size,link.point);
		}
		return 0;
	}
	else
	{
		len=-1;
		send(new_fd,&len,4,0);
	}
	return 0;
}
/*int ret;
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
close(new_fd);
return 0;
}*/

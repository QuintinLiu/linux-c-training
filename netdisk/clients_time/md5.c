#include"head.h"
  
int get_md5(char *filename,char *md5)
{
	int ret,i;
	char tmp[3]={0};
	int fd=open(filename,O_RDONLY);
	char buf[1024*1024]={0};
	MD5_CTX ctx;
	unsigned char outmd[16]={0};
	MD5_Init(&ctx);
	while((ret=read(fd,buf,sizeof(buf)))!=0)
	{
		MD5_Update(&ctx,buf,ret);
	}
	MD5_Final(outmd,&ctx);
	 
	for(i=0;i<16;i++)
	{
		sprintf(tmp,"%02x",outmd[i]);
		strcat(md5,tmp);
	}
	close(fd);
	return 0;
}

#include"head.h"
int get_token(pid_t pid,char *salt,char *md5)
{
	int i;
	char key[30]={0};
	char tmp[3]={0};
	unsigned char outmd[16]={0};
	sprintf(key,"%s%d",salt,pid);
	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx,key,strlen(key));
	MD5_Final(outmd,&ctx);
	 
	for(i=0;i<16;i++)
	{
		sprintf(tmp,"%02x",outmd[i]);
		strcat(md5,tmp);
	}
	return 0;
}

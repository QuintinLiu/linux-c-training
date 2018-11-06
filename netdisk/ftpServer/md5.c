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
int get_md5(pid_t pid,char *salt,char *md5)
{
	int i;
	char key[30]={0};
	char tmp[3]={0};
	unsigned char outmd[16]={0};
	produce_salt(salt,8);
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

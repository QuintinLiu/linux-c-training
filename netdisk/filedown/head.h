#ifndef __HEAD_H__
#define __HEAD_H__
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<signal.h>
#include<sys/time.h>
#include<time.h>
#include<pthread.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/epoll.h>
#include<sys/uio.h>
#include<mysql/mysql.h>
#include<sys/mman.h>
#include<crypt.h>
#include<openssl/md5.h>
typedef struct
{
	int len;
	char buf[1024];
}data;
typedef struct
{
	int new_fd;
	char name[20];
	char ip[20];
	short port;
}cliinfo;
typedef struct
{
	char name[20];
	char order1[10];
	char order2[20];
	char ip[20];
	short port;
	off_t offset;
	off_t size;
	off_t point;
}lkinfo;

int transfer_file(int,char *,short);
int send_n(int,char *,int);
int recv_n(int,char *,int);
int send_file(int,char *,off_t,off_t,off_t);
int query_token(char *,char *);
int query_md5(char *,char *,char *);
int insert_log(char *,short,char *,char *);

#define args_check(a,b) {if(a!=b) {printf("args error\n");return -1;}}
#define error_check(val,ret,func_name) {if(ret==val) {perror(func_name); return -1;}}
#define thread_check(ret,func_name) {if(ret!=0) {printf("%s failed:%d\n",func_name,ret); return -1;}}
#endif

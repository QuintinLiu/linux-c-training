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
#include<crypt.h>
#include <openssl/md5.h>
#include<sys/mman.h>
typedef struct
{
	int len;
	char buf[1024];
}data;
typedef struct
{
	char ip[20];
	short port;
	off_t offset;
	off_t size;
}ctl;
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

int transfer(int);
int send_n(int,char *,int);
int send_file(int,char *);
int recv_n(int,char *,int);
int recv_file(int,char *);
int act_login(int,char *);
int act_sign(int,char *); 
int act_puts(int,char *);
int produce_salt(char *,int);
int get_md5(char *,char *);
int get_token(pid_t,char *,char *);
int query_filedown(char *,char *,off_t,off_t *);
int insert_filedown(char *,char *,off_t,off_t,off_t);
int update_filedown(char *,char *,off_t,off_t);
int delete_filedown(char *,char *);

#define args_check(a,b) {if(a!=b) {printf("args error\n");return 0;}}
#define error_check(val,ret,func_name) {if(ret==val) {perror(func_name); return 0;}}
#define thread_check(ret,func_name) {if(ret!=0) {printf("%s failed:%d\n",func_name,ret); return 0;}}
#endif

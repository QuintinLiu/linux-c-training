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
#include <sys/mman.h>
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
	int slot;
	int site;
	time_t start;
}cliinfo;
typedef struct
{
	char ip[20];
	short port;
	off_t offset;
	off_t size;
}ctl;


int pthread_act(int,char *,char *,int,char *,char *);
int get_md5(pid_t,char *,char *);
int transfer(int);
int send_n(int,char *,int);
int send_file(int,char *);
int recv_n(int,char *,int);
int search_file(char *);
int recv_file(int,char *);
int query_salt(char *,char *,char *);
int query_path(char *,char *);
int query_token(char *,char *);
int query_code(char *,char *,int *);
int query_curcode(char *,int *);
int query_if_account(char *);
int query_precode(int,int *);
int query_file(int,char *,int);
int query_if_file(char *,char *);
int query_if_dir_file(char *,int);
int query_if_name_file(char *,char *);
int query_if_name_dir(char *,char *);
int query_md5(char *,char *,char *);
int update_path_code(char *,char *,int);
int update_token(char *,char *);
int insert_account(char *,char *,char *);
int insert_file(char *,int,char *,char *);
int insert_dir(char *,int,char *);
int insert_log(char *,short,char *,char *);
int delete_file(char *,char *);
int act_sign(int,char *);
int act_login(int,char *);
int act_pwd(int,char *);
int act_cd(int,char *,char *);
int act_ls(int,char *);
int act_remove(int,char *,char *);
int act_puts(int,char *,char *);
int act_gets(int,char *,char *);
int act_mkdir(int,char *,char *);
int act_remove(int,char *,char *);

#define args_check(a,b) {if(a!=b) {printf("args error\n");return -1;}}
#define error_check(val,ret,func_name) {if(ret==val) {perror(func_name); return -1;}}
#define thread_check(ret,func_name) {if(ret!=0) {printf("%s failed:%d\n",func_name,ret); return -1;}}
#endif

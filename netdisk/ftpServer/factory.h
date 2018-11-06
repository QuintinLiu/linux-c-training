#ifndef __FACTORY_H__
#define __FACTORY_H__
#include"head.h"
#include"work_que.h"

typedef void *(*pthread_func_t)(void *);

typedef struct
{
	pthread_t *pth_id;
	int pthread_num;
	que_t que;
	pthread_func_t pthread_func;
	pthread_cond_t cond;
	short start_flg;
}factory;

int serve(factory *,cliinfo *,int *);
void *pthread_func(void *);
int factory_init(factory *,int,int,pthread_func_t);
int factory_start(factory *);
int sock_bind(int *,char *,char *);

#endif

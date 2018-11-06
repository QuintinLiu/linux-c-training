#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include"head.h"

typedef struct list_node
{
	int new_fd;
	char name[20];
	char ip[20];
	short port;
	char order1[10];
	char order2[30];
	struct list_node *next;
}node;

typedef struct
{
	node *que_head,*que_tail;
	int que_size;
	int que_capacity;
	pthread_mutex_t que_mutex;
}que_t;

int que_insert(que_t *,node *);
int que_get(que_t *,node **);
int que_insert_exit(que_t*,node *);
#endif

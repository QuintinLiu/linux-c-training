#include"factory.h"

int factory_init(factory *fp,int pthread_num,int capacity,pthread_func_t pthread_func)
{
	int ret;
	fp->pth_id=(pthread_t *)calloc(pthread_num,sizeof(pthread_t));
	fp->pthread_num=pthread_num;
	fp->pthread_func=pthread_func;
	ret=pthread_cond_init(&fp->cond,NULL);
	error_check(-1,ret,"pthread_cond_init");
	fp->que.que_capacity=capacity;
	pthread_mutex_init(&fp->que.que_mutex,NULL);
	error_check(-1,ret,"pthread_mutex_init");
	return 0;
}

int factory_start(factory *fp)
{
	if(!fp->start_flg)
	{
		int i,ret;
		for(i=0;i<fp->pthread_num;i++)
		{
			ret=pthread_create(fp->pth_id+i,NULL,fp->pthread_func,fp);
			error_check(-1,ret,"pthread_create");
		}
		fp->start_flg=1;
	}
	return 0;
}

int sock_bind(int *psfd,char *ip,char *port)
{
	int ret;
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	int reuse=1;
	ret=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
	error_check(-1,ret,"setsockopt");
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(port));
	ser.sin_addr.s_addr=inet_addr(ip);
	ret=bind(sfd,(struct sockaddr *)&ser,sizeof(struct sockaddr));
	error_check(-1,ret,"bind");
	*psfd=sfd;
	return 0;
}

void *pthread_func(void *p)
{
	factory *fp=(factory *)p;
	node *pcur;
	que_t *pque=&fp->que;
	while(1)
	{
		pthread_mutex_lock(&pque->que_mutex);
		if(!pque->que_size)
		{
			pthread_cond_wait(&fp->cond,&pque->que_mutex);
		}
		que_get(pque,&pcur);
		pthread_mutex_unlock(&pque->que_mutex);
		if(pcur!=NULL)
		{
			if(pcur->new_fd!=-1)
			{
				printf("ready to serve\n");
				pthread_act(pcur->new_fd,pcur->name,pcur->ip,pcur->port,pcur->order1,pcur->order2);
				printf("serve done\n");
				close(pcur->new_fd);
				free(pcur);
			}
			else
			{
				pthread_exit(NULL);
			}
		}
	}
}

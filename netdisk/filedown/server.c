#include"factory.h"
int fds[2];
void handle_func(int s)
{
	char sig_exit='a';
	write(fds[1],&sig_exit,1);
	wait(NULL);
	exit(0);
}
int main(int argc,char **argv)
{
	args_check(argc,5);
	pipe(fds);
	if(fork())
	{
		printf("i am parent pid=%d\n",getpid());
		close(fds[0]);
		signal(SIGUSR1,handle_func);
		while(1)
		{
			sleep(10);
		}
	}
	int ret;
	printf("i am child pid=%d\n",getpid());
	close(fds[1]);
	factory f;
	bzero(&f,sizeof(f));
	int pthread_num=atoi(argv[3]);
	int capacity=atoi(argv[4]);
	factory_init(&f,pthread_num,capacity,pthread_func);
	factory_start(&f);
	int sfd;
	ret=sock_bind(&sfd,argv[1],argv[2]);
	if(-1==ret)
	{
		return -1;
	}

	int epfd=epoll_create(1);
	struct epoll_event event,evt[5];
	bzero(&event,sizeof(event));
	event.events=EPOLLIN;
	event.data.fd=sfd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	error_check(-1,ret,"epoll_ctl");
	event.data.fd=fds[0];
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fds[0],&event);
	error_check(-1,ret,"epoll_ctl");

	int ret_e,i;
	struct sockaddr_in cli;
	unsigned int addrlen=sizeof(struct sockaddr);
	int new_fd;
	node *pnew;
	que_t *pque=&f.que;
	listen(sfd,f.que.que_capacity);
	while(1)
	{
		ret_e=epoll_wait(epfd,evt,5,-1);	
		if(ret_e>0)
		{
			for(i=0;i<ret_e;i++)
			{
				if(evt[i].data.fd==sfd)
				{
					bzero(&cli,sizeof(cli));
					new_fd=accept(sfd,(struct sockaddr*)&cli,&addrlen);
					error_check(-1,new_fd,"accept error");
					pnew=(node *)calloc(1,sizeof(node));
					pnew->new_fd=new_fd;
					strcpy(pnew->ip,inet_ntoa(cli.sin_addr));
					pnew->port=ntohs(cli.sin_port);
					pthread_mutex_lock(&pque->que_mutex);
					que_insert(pque,pnew);
					pthread_mutex_unlock(&pque->que_mutex);
					pthread_cond_signal(&f.cond);
				}
				if(evt[i].data.fd==fds[0])
				{
					char exit_flg;
					read(fds[0],&exit_flg,1);
					printf("server will exit\n");
					close(sfd);
					pnew=(node *)calloc(1,sizeof(que_t));
					pnew->new_fd=-1;
					pthread_mutex_lock(&pque->que_mutex);
					que_insert_exit(pque,pnew);
					pthread_mutex_unlock(&pque->que_mutex);
					pthread_cond_broadcast(&f.cond);
					for(i=0;i<f.pthread_num;i++)
					{
						pthread_join(f.pth_id[i],NULL);
						printf("pthread %lu exit\n",f.pth_id[i]);
					}
					exit(0);
				}
			}
		}
	}
}

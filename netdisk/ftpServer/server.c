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
	printf("off_t:%d\n",sizeof(off_t));
	int ret;
	char ip[20]={0};
	char port[8]={0};
	int num,cap;
	FILE *fp;
	fp=fopen("../conf/server.conf","r");
	ret=fscanf(fp,"%s%s%d%d",ip,port,&num,&cap);
	error_check(-1,ret,"fopen");
	fclose(fp);
	printf("%s %s %d %d\n",ip,port,num,cap);
	close(fds[1]);
	factory f;
	bzero(&f,sizeof(f));
	int pthread_num=num;
	int capacity=cap;
	factory_init(&f,pthread_num,capacity,pthread_func);
	factory_start(&f);
	int sfd;
	ret=sock_bind(&sfd,ip,port);
	if(-1==ret)
	{
		return -1;
	}
	int epfd=epoll_create(1);
	struct epoll_event event,evt[12];
	bzero(&event,sizeof(event));
	event.events=EPOLLIN;
	event.data.fd=sfd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	error_check(-1,ret,"epoll_ctl");
	event.data.fd=fds[0];
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fds[0],&event);
	error_check(-1,ret,"epoll_ctl");
	int ret_e,i,j;
	int quit_flg=0;
	cliinfo *client;
	client=(cliinfo *)calloc(capacity,sizeof(cliinfo));
	for(i=0;i<capacity;i++)
	{
		client[i].new_fd=-1;
	}
	node *pnew;
	que_t *pque=&f.que;
	listen(sfd,f.que.que_capacity);
	struct sockaddr_in cli;
	unsigned int addrlen=sizeof(struct sockaddr);

	//循环数组
	int k;
	int timer[31][10];
	memset(timer,-1,sizeof(timer));
	int index=0;
	time_t last=time(NULL);
	time_t start=time(NULL);
	while(1)
	{
		ret_e=epoll_wait(epfd,evt,12,1000);	
		if(ret_e>0)
		{
			for(i=0;i<ret_e;i++)
			{
				for(j=0;j<capacity;j++)
				{
					if(evt[i].data.fd==client[j].new_fd)
					{
						quit_flg=0;
						serve(&f,&client[j],&quit_flg);
						client[j].start=time(NULL);
						timer[client[j].slot][client[j].site]=-1;
						if(index==0)
							ret=30;
						else
							ret=index-1;
						for(k=0;k<10;k++)//更新new_fd在timer中的位置
						{
							if(timer[ret][k]==-1)
							{
								timer[ret][k]=client[j].new_fd;
								client[j].slot=ret;
								client[j].site=k;
								break;
							}
						}
						if(quit_flg==1)
						{
							//解注册
							event.data.fd=client[j].new_fd;
							epoll_ctl(epfd,EPOLL_CTL_DEL,client[j].new_fd,&event);
							printf("new_fd %d bye bye\n",client[j].new_fd);
							client[j].new_fd=-1;
							timer[client[j].slot][client[j].site]=-1;
						}
						break;
					}
				}
				if(evt[i].data.fd==sfd)
				{
					bzero(&cli,sizeof(cli));
					for(j=0;j<capacity;j++)
					{
						if(client[j].new_fd==-1)
						{
							break;
						}
					}
					client[j].new_fd=accept(sfd,(struct sockaddr*)&cli,&addrlen);
					error_check(-1,client[j].new_fd,"accept");
					strcpy(client[j].ip,inet_ntoa(cli.sin_addr));
					client[j].port=ntohs(cli.sin_port);
					client[j].start=time(NULL);
					event.data.fd=client[j].new_fd;
					//加入监控
					ret=epoll_ctl(epfd,EPOLL_CTL_ADD,client[j].new_fd,&event);
					error_check(-1,ret,"epoll error");
					printf("new_fd=%d\n",client[j].new_fd);
					if(index==0)
						ret=30;
					else
						ret=index-1;
					for(k=0;k<10;k++)//添加进timer
					{
						if(timer[ret][k]==-1)
						{
							timer[ret][k]=client[j].new_fd;
							client[j].slot=ret;
							client[j].site=k;
							break;
						}
					}
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
			if(time(NULL)-last>=1)
			{
				printf("after %ld sec\n",time(NULL)-start);
				if((time(NULL)-start)>=30)
					start=time(NULL);
				for(k=0;k<10;k++)
				{
					if(timer[index][k]!=-1)
					{
						close(timer[index][k]);
						event.data.fd=timer[index][k];
						epoll_ctl(epfd,EPOLL_CTL_DEL,timer[index][k],&event);
						for(j=0;j<capacity;j++)
						{
							if(client[j].new_fd==timer[index][k])
								client[j].new_fd=-1;
						}
						printf("time over new_fd %d byebye\n",timer[index][k]);
						timer[index][k]=-1;
					}
				}
				index=(index+1)%31;
				last=time(NULL);
			}
		}
		else
		{
			printf("after %ld sec\n",time(NULL)-start);
			if((time(NULL)-start)>=30)
				start=time(NULL);
			for(k=0;k<10;k++)
			{
				if(timer[index][k]!=-1)
				{
					close(timer[index][k]);
					event.data.fd=timer[index][k];
					epoll_ctl(epfd,EPOLL_CTL_DEL,timer[index][k],&event);
					for(j=0;j<capacity;j++)
					{
						if(client[j].new_fd==timer[index][k])
						{
							printf("new_fd %d after %ld sec\n",client[j].new_fd,time(NULL)-client[j].start);
							client[j].new_fd=-1;
							break;
						}
					}
					printf("time over new_fd %d byebye\n",timer[index][k]);
					timer[index][k]=-1;
				}
			}
			index=(index+1)%31;
			last=time(NULL);
		}
	}
}

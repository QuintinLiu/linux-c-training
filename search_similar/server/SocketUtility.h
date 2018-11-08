 ///
 /// @file    SocketUtility.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-22 22:59:56
 ///

#ifndef __WD_SOCKETUTILITY_H__
#define __WD_SOCKETUTILITY_H__

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
using namespace wd;

inline void setNonBlock(int fd)
{
	int flg = ::fcntl(fd, F_GETFL, 0);
	flg |= O_NONBLOCK;
	::fcntl(fd, F_SETFL, 0);
}

inline size_t recvPeek(int fd, char * pbuf, size_t count)
{
	int nread = 0;
	do
	{
		nread = ::recv(fd, pbuf, count, MSG_PEEK);
	}while(nread == -1 && errno == EINTR);
	return nread;
}

inline int createSocketFd()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1)
	{
		perror("socket error");
	}
	return fd;
}

inline int createEpollFd()
{
	int efd = ::epoll_create1(0);
	if(efd == -1)
	{
		perror("epoll_create1 error");
		exit(EXIT_FAILURE);
	}
	return efd;
}

inline int createEventFd()
{
	int fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if(fd == -1)
	{
		perror("eventfd error");
	}
	return fd;
}

inline void addEpollFd(int efd, int listenfd)
{
	struct epoll_event evt;
	evt.data.fd = listenfd;
	evt.events = EPOLLIN;
	int ret = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &evt);
	if(ret == -1)
	{
		perror("epoll_ctl error");
		exit(EXIT_FAILURE);
	}
}

inline void delEpollFd(int efd, int peerfd)
{
	struct epoll_event evt;
	evt.data.fd = peerfd;
	int ret = epoll_ctl(efd, EPOLL_CTL_DEL, peerfd, &evt);
	if(ret == -1)
	{
		perror("epoll_ctl error");
		exit(EXIT_FAILURE);
	}
}

inline bool isConnectionClosed(int fd)
{
	char buf[1024] = {0};
	int ret = recvPeek(fd, buf, sizeof(buf));
	if(ret == -1)
	{
		perror("recvPeek error");
		return true;
	}
	return ret == 0;
}


#endif

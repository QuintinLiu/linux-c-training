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
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
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


#endif

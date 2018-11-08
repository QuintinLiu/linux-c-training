 ///
 /// @file    Socket.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-22 22:29:13
 ///

#include "Socket.h"
#include "SocketUtility.h"
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>

namespace wd
{

Socket::Socket(int fd)
:_fd(fd)
{}

Socket::~Socket()
{
	::close(_fd);
}

int Socket::fd()
{
	return _fd;
}

void Socket::shutDownWrite() const
{
	if(::shutdown(_fd, SHUT_WR) == -1)
	{
		::perror("shutdown error");
	}
}
void Socket::nonBlock() const
{
	setNonBlock(_fd);
}

InetAddress Socket::getLocalAddr(int fd)
{
	struct sockaddr_in addr;
	int len = sizeof(addr);
	if(::getsockname(fd, (struct sockaddr *)& addr, (socklen_t *)&len) == -1)
	{
		perror("getsockname error");
	}
	return InetAddress(addr);
}
InetAddress Socket::getPeerAddr(int fd)
{
	struct sockaddr_in addr;
	int len = sizeof(addr);
	if(::getpeername(fd, (struct sockaddr *)& addr, (socklen_t *)(&len)) == -1)
	{
		perror("getpeername error");
	}
	return InetAddress(addr);
}

};//end of namespace wd

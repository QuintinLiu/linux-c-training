 ///
 /// @file    Acceptor.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-23 21:06:48
 ///

#include "Acceptor.h"
#include "SocketUtility.h"

using namespace wd;

Acceptor::Acceptor(int fd, const InetAddress & addr)
:_addr(addr)
,_listenSock(fd)
{}

void Acceptor::setReuseAddr(bool flg)
{
	int on = flg ? 1 : 0;
	if(::setsockopt(_listenSock.fd(), 
					SOL_SOCKET, 
					SO_REUSEADDR, 
					&on, 
					static_cast<socklen_t>(sizeof(on))) == -1)
	{
		perror("setsockopt error");
		::close(_listenSock.fd());
		exit(EXIT_FAILURE);
	}
}

void Acceptor::setReusePort(bool flg)
{
#ifdef SO_REUSEPORT
	int on = flg ? 1 : 0;
	if(::setsockopt(_listenSock.fd(),
					SOL_SOCKET,
					SO_REUSEADDR,
					&on,
					static_cast<socklen_t>(sizeof(on))) == -1)
	{
		perror("setsockopt error");
		::close(_listenSock.fd());
		exit(EXIT_FAILURE);
	}
#else
	if(flg)
	{
		fprintf(stderr, "SO_REUSEPORT is not supported");
	}
#endif
}

void Acceptor::ready()
{
	setReuseAddr(true);
	setReusePort(true);
	bind();
	listen();
}

void Acceptor::bind()
{
	if(::bind(_listenSock.fd(), 
			  (struct sockaddr *)_addr.getInetAddressPtr(), 
			  sizeof(InetAddress)) == -1)
	{
		perror("bind error");
		::close(_listenSock.fd());
		exit(EXIT_FAILURE);
	}
}

void Acceptor::listen()
{
	if(::listen(_listenSock.fd(), 10))
	{
		perror("listen error");
		::close(_listenSock.fd());
		exit(EXIT_FAILURE);
	}
}

int Acceptor::accept()
{
	int ret = ::accept(_listenSock.fd(), NULL, 0);
	if(ret == -1)
	{
		perror("accept error");
	}
	return ret;
}

void Acceptor::connect()
{
	if(::connect(fd(),
	   (struct sockaddr *)_addr.getInetAddressPtr(),
	   sizeof(InetAddress)) == -1)
	{
		perror("connect error");
		::close(_listenSock.fd());
		exit(EXIT_FAILURE);
	}
}

int Acceptor::fd()
{ return _listenSock.fd(); }

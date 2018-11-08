 ///
 /// @file    SocketIO.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-22 23:24:58
 ///

#include "SocketIO.h"
#include "SocketUtility.h"

using namespace wd;

SocketIO::SocketIO(int fd)
:_fd(fd)
{}
size_t SocketIO::readn(char * pbuf, size_t count)
{
	size_t nleft = count;
	size_t nread = 0;
	while(nleft > 0)
	{
		nread = ::read(_fd, pbuf, nleft);
		if(nread == -1)
		{
			if(errno == EINTR)
				continue;
			return EXIT_FAILURE;
		}
		else if(nread == 0)
		{
			break;
		}
		pbuf += nread;
		nleft -= nread;
	}
	return count - nleft;
}
size_t SocketIO::writen(const char * pbuf, size_t count)
{
	int nwrite;
	int nleft = count;
	while(nleft > 0)
	{
		nwrite = ::write(_fd, pbuf, nleft);
		if(nwrite == -1)
		{
			if(errno == EINTR)
				continue;
			return EXIT_FAILURE;
		}
		pbuf += nwrite;
		nleft -= nwrite;
	}
	return count - nleft;
}
size_t SocketIO::readLine(char * pbuf, size_t max_count)
{
	int nleft = max_count - 1;
	int npeek= 0;
	int total = 0;
	while(nleft > 0)
	{
		npeek = recvPeek(_fd, pbuf, nleft);
		if(npeek <= 0)
			return npeek;
		for(int idx = 0; idx != npeek; ++idx)
		{
			if(pbuf[idx] == '\n')
			{
				int nread = idx + 1;
				if(readn(pbuf, nread) != nread)
					return EXIT_FAILURE;
				total += nread;
				pbuf += nread;
				*pbuf = 0;
				return total;
			}
		}
		if(readn(pbuf, npeek) != npeek)
			return EXIT_FAILURE;
		pbuf += npeek;
		nleft -= npeek;
		total += npeek;
	}
	return max_count - 1;
}

 ///
 /// @file    eventfdd.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-09-13 09:50:34
 ///
 
#include "Eventfd.h"
#include <poll.h>
#include <errno.h>
#include <stdio.h>
#include <sys/eventfd.h>
#include <unistd.h>
using namespace wd;
Eventfd::Eventfd(CallBack && cb)
:_evfd(createEventfd())
,_cb(std::move(cb))
,_isStart(false)
{}

void Eventfd::startListen()
{
	_isStart = true;
	struct pollfd pfd;
	pfd.fd = _evfd;
	pfd.events = POLLIN;
	while(_isStart)
	{
		int nready = ::poll(&pfd, 1, 5000);
		if(nready == -1 && errno == EINTR)
			continue;
		else if(nready == -1)
		{
			perror("poll error");
		}
		else if(nready == 0)
		{
			printf("poll time out\n");
		}
		else
		{
			if(pfd.revents & POLLIN)
			{
				handleRead();
				if(_cb)
				{
					_cb();
				}
			}
		}
	}
}

void Eventfd::handleRead()
{
	uint64_t ut;
	int ret = ::read(_evfd, &ut, sizeof(uint64_t));
	if(ret != sizeof(uint64_t))
	{
		perror("read error");
	}
}

void Eventfd::wakeUp()
{
	uint64_t ut = 1;
	int ret = ::write(_evfd, &ut, sizeof(uint64_t));
	if(ret != sizeof(uint64_t))
	{
		perror("write error");
	}
}

int Eventfd::createEventfd()
{
	int fd = eventfd(0, 0);
	if(fd == -1)
	{
		perror("eventfd error");
	}
	return fd;
}

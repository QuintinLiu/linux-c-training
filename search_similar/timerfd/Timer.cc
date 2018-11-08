 ///
 /// @file    Timer.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-09-14 20:45:02
 ///

#include "Timer.h"
#include <poll.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/timerfd.h>

using namespace wd;

Timer::Timer(int initTime, int intervalTime, CallBack && cb)
:_tfd(createTimerfd())
,_initTime(initTime)
,_intervalTime(intervalTime)
,_cb(std::move(cb))
,_isRun(false)
{}

void Timer::start()
{
	struct pollfd pfd;
	pfd.fd = _tfd;
	pfd.events = POLLIN;
	setTimer(_initTime, _intervalTime);
	_isRun = true;
	while(_isRun)
	{
		int ret = ::poll(&pfd, 1, 5000);
		if(ret == -1 && errno == EINTR)
		{ continue; }
		else if(ret == -1)
		{
			perror("poll error");
			exit(EXIT_FAILURE);
		}
		else if(ret == 0)
		{
			printf("poll time out\n");
		}
		else
		{
			handleRead();
			if(_cb)
				_cb();
		}
	}
}

void Timer::stop()
{
	if(_isRun)
	{
		setTimer(0, 0);
		_isRun = false;
	}
}

int Timer::createTimerfd()
{
	int fd = ::timerfd_create(CLOCK_REALTIME, 0);
	if(fd == -1)
	{
		perror("timer_create");
	}
	return fd;
}

void Timer::setTimer(int initTime, int intervalTime)
{
	struct itimerspec value;
	value.it_value.tv_sec = initTime;
	value.it_value.tv_nsec = 0;
	value.it_interval.tv_sec = intervalTime;
	value.it_interval.tv_nsec = 0;
	int ret = ::timerfd_settime(_tfd, 0, &value, NULL);
	if(ret == -1)
	{
		perror("timer_settime error");
	}
}

void Timer::handleRead()
{
	uint64_t ut;
	int ret = ::read(_tfd, &ut, sizeof(uint64_t));
	if(ret != sizeof(uint64_t))
	{
		perror("read error");
	}
}

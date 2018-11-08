 ///
 /// @file    thread.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-20 10:43:38
 ///

#include "Thread.h"
#include <iostream>

using namespace wd;

Thread::Thread(ThreadCallBack && cb)
:_pthid(0)
,_isRunning(false)
,_cb(std::move(cb))
{}
void Thread::start()
{
	pthread_create(&_pthid, NULL, threadFunc, this);
	_isRunning = true;
}
void * Thread::threadFunc(void * p)
{
	Thread * pthread = static_cast<Thread *>(p);
	if(pthread)
	{
		pthread->_cb();
	}
}
void Thread::join()
{
	if(_isRunning)
	{
		pthread_join(_pthid, NULL);
		_isRunning = false;
	}
}
pthread_t Thread::getThreadId()
{
	return _pthid;
}
Thread::~Thread()
{
	if(_isRunning)
	{
		pthread_detach(_pthid);
		_isRunning = false;
	}
	std::cout << "~Thread()" << std::endl;
}

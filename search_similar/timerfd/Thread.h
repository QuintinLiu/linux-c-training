 ///
 /// @file    thread.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-20 10:35:17
 ///

#ifndef __WD_THREAD_H__
#define __WD_THREAD_H__

#include "NonCopyAble.h"
#include <pthread.h>
#include <functional>

namespace wd
{

class Thread
: NonCopyAble
{
public:
	using ThreadCallBack = std::function<void()>;
	Thread(ThreadCallBack &&);
	~Thread();
	void start();
	void join();
	pthread_t getThreadId();
private:
	static void *threadFunc(void *);
private:
	pthread_t _pthid;
	bool _isRunning;
	ThreadCallBack _cb;
};


}//endof namespace

#endif

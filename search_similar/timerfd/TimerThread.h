 ///
 /// @file    TimerThread.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-09-14 20:23:22
 ///

#ifndef __WD_TIMERTHREAD_H__
#define __WD_TIMERTHREAD_H__

#include "Timer.h"
#include "Thread.h"
#include <functional>

namespace wd
{

class TimerThread
{
	using CallBack = std::function<void()>;
public:
	TimerThread(int initTime, int intervalTime, CallBack && cb)
	:_timer(initTime, intervalTime, std::move(cb))
	,_thread(std::bind(&Timer::start, &_timer))
	{}
	void start()
	{
		_thread.start();
	}
	void stop()
	{
		_timer.stop();
		_thread.join();
	}
private:
	Timer _timer;
	Thread _thread;
};

}//end of namespace std;

#endif

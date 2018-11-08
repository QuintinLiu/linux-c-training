 ///
 /// @file    Timer.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-09-14 20:34:40
 ///

#ifndef __WD_TIMER_H__
#define __WD_TIMER_H__

#include "NonCopyAble.h"
#include <functional>

namespace wd
{

class Timer
: NonCopyAble
{
public:
	using CallBack = std::function<void()>;
	Timer(int, int, CallBack &&);
	void start();
	void stop();
private:
	void setTimer(int, int);
	int createTimerfd();
	void handleRead();
private:
	int _tfd;
	int _initTime;
	int _intervalTime;
	CallBack _cb;
	bool _isRun;
};

}//end of namespace

#endif

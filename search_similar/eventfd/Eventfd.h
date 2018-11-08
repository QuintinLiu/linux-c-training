 ///
 /// @file    eventfd.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-09-13 09:36:29
 ///

#ifndef __WD_EVENTFD_H__
#define __WD_EVENTFD_H__

#include "NonCopyAble.h"
#include <functional>

namespace wd
{

class Eventfd
: NonCopyAble
{
public:
	using CallBack = std::function<void()>;
	Eventfd(CallBack &&);
	void startListen();
	void wakeUp();
private:
	void handleRead();
	int createEventfd();
private:
	int _evfd;
	CallBack _cb;
	bool _isStart;
};

}//end of namespace wd

#endif

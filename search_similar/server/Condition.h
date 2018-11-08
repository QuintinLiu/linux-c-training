 ///
 /// @file    Condition.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-20 20:48:28
 ///

#ifndef __WD_CONDITION_H__
#define __WD_CONDITION_H__

#include "NonCopyAble.h"
#include "MutexLock.h"
#include <pthread.h>

namespace wd
{

class Condition
: NonCopyAble
{
public:
	Condition(MutexLock &);
	~Condition();
	void wait();
	void notify();
	void notifyAll();
private:
	pthread_cond_t _cond;
	MutexLock & _mutexLock;
	
};

}

#endif

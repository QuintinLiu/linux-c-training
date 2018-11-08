 ///
 /// @file    MutexLock.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-20 20:31:46
 ///

#ifndef __WD_MUTEXLOCK_H__
#define __WD_MUTEXLOCK_H__

#include "NonCopyAble.h"
#include <pthread.h>

namespace wd
{

class MutexLock
: NonCopyAble
{
public:
	MutexLock();
	~MutexLock();
	void lock();
	void unlock();
	pthread_mutex_t * getMutexPtr();
private:
	pthread_mutex_t _mutex;
	bool _isLock;
};


class MutexLockGuard
{
public:
	MutexLockGuard(MutexLock &);
	~MutexLockGuard();
private:
	MutexLock & _mutexLock;
};

}//end of namespace wd

#endif

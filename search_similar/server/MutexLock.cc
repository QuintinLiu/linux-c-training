 ///
 /// @file    MutexLock.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-20 20:35:38
 ///

#include "MutexLock.h"
#include <iostream>

namespace wd
{

MutexLock::MutexLock()
:_isLock(false)
{
	pthread_mutex_init(&_mutex, NULL);
}

MutexLock::~MutexLock()
{
	pthread_mutex_destroy(&_mutex);
}

void MutexLock::lock()
{
	pthread_mutex_lock(&_mutex);
	_isLock = true;
}

void MutexLock::unlock()
{
	pthread_mutex_unlock(&_mutex);
	_isLock =false;
}

pthread_mutex_t * MutexLock::getMutexPtr()
{
	return &_mutex;
}


MutexLockGuard::MutexLockGuard(MutexLock & mutexLock)
:_mutexLock(mutexLock)
{
	_mutexLock.lock();
}
MutexLockGuard::~MutexLockGuard()
{
	_mutexLock.unlock();
}

}//end of namespace wd

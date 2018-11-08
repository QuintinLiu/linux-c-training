 ///
 /// @file    Condition.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-20 20:58:20
 ///

#include "Condition.h"

namespace wd
{

Condition::Condition(MutexLock & mutexlock)
:_mutexLock(mutexlock)
{
	pthread_cond_init(&_cond, NULL);
}

Condition::~Condition()
{
	pthread_cond_destroy(&_cond);
}

void Condition::wait()
{
	pthread_cond_wait(&_cond, _mutexLock.getMutexPtr());
}

void Condition::notify()
{
	pthread_cond_signal(&_cond);
}

void Condition::notifyAll()
{
	pthread_cond_broadcast(&_cond);
}


}//end of namespace wd

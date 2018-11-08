 ///
 /// @file    TaskQueue.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-20 21:29:48
 ///

#include "TaskQueue.h"

using namespace wd;

TaskQueue::TaskQueue(size_t size)
:_queSize(size)
,_mutex()
,_NotFull(_mutex)
,_NotEmpty(_mutex)
,_exitFlg(false)
{}

void TaskQueue::push(const ElemType & elem)
{
	MutexLockGuard autoLock(_mutex);
	while(isFull())
	{
		_NotFull.wait();
	}
	_que.push(elem);
	_NotEmpty.notify();
}

ElemType TaskQueue::pop()
{
	while(!_exitFlg && isEmpty())
	{
		_NotEmpty.wait();
	}
	if(!_exitFlg)
	{
		ElemType tmp = _que.front();
		_que.pop();
		_NotFull.notify();
		return tmp;
	}
	else
	{
		return NULL;
	}
}
void TaskQueue::wakeUp()
{
	_exitFlg = true;
	_NotEmpty.notifyAll();
}


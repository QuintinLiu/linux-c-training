 ///
 /// @file    TaskQueue.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-20 21:09:57
 ///

#ifndef __WD_TASKQUEUE_H__
#define __WD_TASKQUEUE_H__
#include "NonCopyAble.h"
#include "MutexLock.h"
#include "Condition.h" 
#include <queue>
#include <functional>
using std::queue;

namespace wd
{

using ElemType = std::function<void()>;

class TaskQueue
: NonCopyAble
{
public:
	TaskQueue(size_t);
	bool isFull() const { return _que.size() == _queSize; }
	bool isEmpty() const { return _que.size() == 0; }
	void push(const ElemType &);
	ElemType pop();
	void wakeUp();
private:
	size_t _queSize;
	queue<ElemType> _que;
	MutexLock _mutex;
	Condition _NotFull;
	Condition _NotEmpty;
	bool _exitFlg;
};

}//end of namespace wd

#endif

 ///
 /// @file    ThreadPool.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-20 23:29:08
 ///

#ifndef __WD_THREADPOOL_H__
#define __WD_THREADPOOL_H__

#include "TaskQueue.h"
#include "Thread.h"
#include <vector>
#include <memory>
#include <functional>
using std::vector;
using std::unique_ptr;

namespace wd
{
using ElemType = std::function<void()>;

class ThreadPool
{
public:
	ThreadPool(size_t, size_t);
	void start();
	void stop();
	void addTask(ElemType);
private:
	ElemType getTask();
	void allocateTask();
private:
	size_t _threadNum;
	size_t _taskNum;
	TaskQueue _taskQue;
	vector<unique_ptr<Thread>> _pths;
	bool _isExit;
};

}//end of namespace wd
#endif

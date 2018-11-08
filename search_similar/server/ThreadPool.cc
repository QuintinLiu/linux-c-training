 ///
 /// @file    TheadPool.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-20 23:44:35
 ///

#include "ThreadPool.h"
#include <utility>
#include <unistd.h>

namespace wd
{

ThreadPool::ThreadPool(size_t threadNum, size_t taskNum)
:_threadNum(threadNum)
,_taskNum(taskNum)
,_taskQue(_taskNum)
,_isExit(false)
{
	_pths.reserve(_threadNum);
}

void ThreadPool::start()
{
	for(int idx = 0; idx != _threadNum; ++idx)
	{
		unique_ptr<Thread> upt(new Thread(std::bind(&ThreadPool::allocateTask, this)));
		_pths.push_back(std::move(upt));
	}
	for(auto & mem : _pths)
	{
		mem->start();
	}
}

void ThreadPool::stop()
{
	if(!_isExit)
	{
		while(!_taskQue.isEmpty())
		{
			sleep(1);
		}
		_isExit = true;
		_taskQue.wakeUp();
		for(auto & mem : _pths)
		{
			mem->join();
		}
	}
}

void ThreadPool::addTask(ElemType ptask)
{
	_taskQue.push(ptask);
}

ElemType ThreadPool::getTask()
{
	return _taskQue.pop();
}

void ThreadPool::allocateTask()
{
	while(!_isExit)
	{
		ElemType cur = getTask();
		if(cur)
			cur();
	}
}

}//end of namespace wd

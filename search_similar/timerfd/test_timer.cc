 ///
 /// @file    test_timer.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-09-14 20:21:00
 ///

#include "TimerThread.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <functional>
using std::cout;
using std::endl;
using namespace wd;

struct Task
{
	void process()
	{
		::srand(time(NULL));
		cout << "task: " << rand()%100 << endl;
	}
};

void test()
{
	Task task;
	TimerThread timer(3, 2, std::bind(&Task::process, &task));
	timer.start();
	::sleep(20);
	timer.stop();
}

int main()
{
	test();
	return 0;
}

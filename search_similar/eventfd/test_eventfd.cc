 ///
 /// @file    test_eventfd.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-09-13 09:20:10
 ///

#include "Eventfd.h"
#include "Thread.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <functional>
using std::cout;
using std::endl;
using namespace wd;

void getNumber()
{
	::srand(time(NULL));
	cout << "subThread: " << pthread_self() << " " << ::rand()%100 << endl;
}

void test()
{
	cout << "mainThread: " << pthread_self() << endl;
	Eventfd ef(getNumber);
	Thread pth(std::bind(&Eventfd::startListen, &ef));
	pth.start();
	
	::sleep(1);
	for(int i = 0; i < 10; ++i)
	{
		ef.wakeUp();
		::sleep(1);
	}

	pth.join();
}

int main()
{
	test();
	return 0;
}

 ///
 /// @file    test_v1.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-23 23:17:33
 ///

#include "TcpServer.h"
#include "ThreadPool.h"
#include <iostream>
#include <functional>
using std::cout;
using std::endl;
using namespace wd;

class Task
{
public:
	Task(string data, TcpConnectionPtr pconn)
	:_data(data)
	,_pconn(pconn)
	{}
	void process()
	{
		cout << "subThread computing" << endl;
		_pconn -> sendInIO(_data);
	}
private:
	string _data;
	TcpConnectionPtr _pconn;
};

void onConnection(const TcpConnectionPtr & pconn)
{
	cout << pconn -> addrToString() << " has connected" << endl;
	pconn -> send("Welcome to char server\n");
}

ThreadPool * g_threadPool = NULL;

void onMessage(const TcpConnectionPtr & pconn)
{
	string msg = pconn -> receive();
	Task task(msg, pconn);

	g_threadPool -> addTask(std::bind(&Task::process, task));
}

void onClose(const TcpConnectionPtr & pconn)
{
	cout << pconn -> addrToString() << " has closed" << endl;
}

void test()
{
	ThreadPool tpool(4, 8);
	g_threadPool = &tpool;
	tpool.start();

	TcpServer tserver("192.168.176.128", 8888);
	tserver.setConnectionCb(onConnection);
	tserver.setMessageCb(onMessage);
	tserver.setCloseCb(onClose);
	cout << "tserver has start" << endl;
	tserver.start();
}
int main()
{
	test();
	return 0;
}

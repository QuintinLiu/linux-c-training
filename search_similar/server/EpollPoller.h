 ///
 /// @file    EpollPoller.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-09-09 10:30:34
 ///

#ifndef __WD_EPOLLPOLLER_H__
#define __WD_EPOLLPOLLER_H__

#include "NonCopyAble.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "MutexLock.h"
#include <sys/epoll.h>
#include <functional>
#include <vector>
#include <map>

namespace wd
{

class Acceptor;

class EpollPoller
: NonCopyAble
{
public:
using EpollCallBack = TcpConnection::TcpConnectionCallBack;
using Functor = std::function<void()>;
	EpollPoller(Acceptor &);
	~EpollPoller();
	void loop();
	void unloop();
	void addIOTasks(const Functor &&);
	void wakeUp();
	void setConnectionCb(EpollCallBack);
	void setMessageCb(EpollCallBack);
	void setCloseCb(EpollCallBack);
private:
	void handleFunctors();
	void epollWait();
	void handleRead();
	void handleConnection();
	void handleMessage(int);
private:
	int _efd;
	int _evfd;
	MutexLock _mutex;
	Acceptor & _acceptor;
	int _listenfd;
	bool _isLoop;
	std::vector<struct epoll_event> _events;
	std::map<int, TcpConnectionPtr> _connMap;
	std::vector<Functor> _pendingFunctors;
	EpollCallBack _connectionCb;
	EpollCallBack _messageCb;
	EpollCallBack _closeCb;
};


}//end of namespace wd

#endif

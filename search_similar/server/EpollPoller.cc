 ///
 /// @file    EpollPoller.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-09-09 15:38:53
 ///

#include "EpollPoller.h"
#include "SocketUtility.h"
#include <stdio.h>
using namespace wd;

EpollPoller::EpollPoller(Acceptor & acceptor)
:_efd(createEpollFd())
,_evfd(createEventFd())
,_acceptor(acceptor)
,_listenfd(_acceptor.fd())
,_isLoop(false)
,_events(1024)
{
	addEpollFd(_efd, _listenfd);
	addEpollFd(_efd, _evfd);
}
EpollPoller::~EpollPoller()
{
	::close(_efd);
}
void EpollPoller::loop()
{
	_isLoop = true;
	while(_isLoop)
	{
		epollWait();
	}
}
void EpollPoller::unloop()
{
	if(_isLoop)
	{
		_isLoop = false;
	}
}
void EpollPoller::setConnectionCb(EpollCallBack cb)
{
	_connectionCb = cb;
}
void EpollPoller::setMessageCb(EpollCallBack cb)
{
	_messageCb = cb;
}
void EpollPoller::setCloseCb(EpollCallBack cb)
{
	_closeCb = cb;
}
void EpollPoller::epollWait()
{
	int eret;
	do
	{
		eret = ::epoll_wait(_efd, &(*_events.begin()), _events.size(), 5000);
	}while(eret == -1 && errno == EINTR);

	if(eret == -1)
	{
		perror("epoll_wait error");
		exit(EXIT_FAILURE);
	}
	else if(eret == 0)
	{
		printf("epoll_wait timeout\n");
	}
	else
	{
		if(eret == _events.size())
		{
			_events.resize(_events.size() * 2);
		}
		for(int i = 0; i < eret; ++i)
		{
			if(_events[i].data.fd == _listenfd && _events[i].events == EPOLLIN)
			{
				handleConnection();
			}
			else if(_events[i].data.fd == _evfd && _events[i].events == EPOLLIN)
			{
				handleRead();
				handleFunctors();
			}
			else if(_events[i].events == EPOLLIN)
			{
				handleMessage(_events[i].data.fd);
			}
		}
	}
}

void EpollPoller::addIOTasks(const Functor && pconn)
{
	{
		MutexLockGuard mlg(_mutex);
		_pendingFunctors.push_back(std::move(pconn));
	}
	wakeUp();
}

void EpollPoller::wakeUp()
{
	uint64_t ut = 1;
	int ret = ::write(_evfd, &ut, sizeof(uint64_t));
	if(ret != sizeof(uint64_t))
	{
		perror("write error");
	}
}

void EpollPoller::handleRead()
{
	uint64_t ut;
	int ret = ::read(_evfd, &ut, sizeof(uint64_t));
	if(ret != sizeof(uint64_t))
	{
		perror("read error");
	}
}

void EpollPoller::handleFunctors()
{
	std::vector<Functor> tmp;
	{
		MutexLockGuard mlg(_mutex);
		tmp.swap(_pendingFunctors);
	}
	printf("handleFunctors\n");
	for(auto & func : tmp)
	{
		func();
	}
}

void EpollPoller::handleConnection()
{
	int newfd = _acceptor.accept();
	addEpollFd(_efd, newfd);

	TcpConnectionPtr pconn(new TcpConnection(newfd, this));
	pconn -> setConnectionCb(_connectionCb);
	pconn -> setMessageCb(_messageCb);
	pconn -> setCloseCb(_closeCb);

	std::pair<std::map<int, TcpConnectionPtr>::iterator, bool> ret;
	ret = _connMap.insert(std::make_pair(newfd, pconn));
	assert(ret.second == true);

	pconn->handleConnection();
}
void EpollPoller::handleMessage(int peerfd)
{
	bool isClosed = isConnectionClosed(peerfd);
	std::map<int, TcpConnectionPtr>::iterator ite = _connMap.find(peerfd);
	assert(ite != _connMap.end());
	if(isClosed)
	{
		ite -> second -> handleClose();
		delEpollFd(_efd, peerfd);
		_connMap.erase(ite);
	}
	else
	{
		ite -> second -> handleMessage();
	}
}

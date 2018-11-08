 ///
 /// @file    TcpServer.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-09-11 20:20:02
 ///

#ifndef __WD_TCPSERVER_H__
#define __WD_TCPSERVER_H__

#include "EpollPoller.h"
#include "Acceptor.h"

namespace wd
{

class TcpServer
: NonCopyAble
{
public:
using ServerCallBack = EpollPoller::EpollCallBack;
	TcpServer(const char *, unsigned short);
	void start();
	void setConnectionCb(ServerCallBack);
	void setMessageCb(ServerCallBack);
	void setCloseCb(ServerCallBack);
private:
	Acceptor _acceptor;
	EpollPoller _poller;
};

}//end of namespace wd

#endif

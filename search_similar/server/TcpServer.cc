 ///
 /// @file    TcpServer.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-09-11 20:29:50
 ///

#include "TcpServer.h"
#include "SocketUtility.h"

using namespace wd;

TcpServer::TcpServer(const char *ip, unsigned short port)
:_acceptor(createSocketFd(), InetAddress(ip, port))
,_poller(_acceptor)
{}
void TcpServer::setConnectionCb(ServerCallBack cb)
{
	_poller.setConnectionCb(cb);
}
void TcpServer::setMessageCb(ServerCallBack cb)
{
	_poller.setMessageCb(cb);
}
void TcpServer::setCloseCb(ServerCallBack cb)
{
	_poller.setCloseCb(cb);
}

void TcpServer::start()
{
	_acceptor.ready();
	_poller.loop();
}

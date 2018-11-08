 ///
 /// @file    TcpConnection.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-23 22:29:11
 ///

#include "TcpConnection.h"
#include "EpollPoller.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;

namespace wd
{

TcpConnection::TcpConnection(int fd, EpollPoller * pepoll)
:_socket(fd)
,_socketIO(fd)
,_localAddr(Socket::getLocalAddr(fd))
,_peerAddr(Socket::getPeerAddr(fd))
,_isShutDownWrite(false)
,_pepoll(pepoll)
{}

TcpConnection::~TcpConnection()
{
	shutDown();
}

void TcpConnection::peerAddr()
{
	cout << _peerAddr.ip() << " " << _peerAddr.port() << " has connected" << endl;
}

string TcpConnection::receive()
{
	char buf[65536] = {0};
	int ret = _socketIO.readLine(buf, sizeof(buf));
	if(ret <= 0)
	{
		return std::string();
	}
	else
	{
		return std::string(buf);
	}
}

void TcpConnection::send(const string & msg)
{
	_socketIO.writen(msg.c_str(), msg.size());
}

void TcpConnection::sendInIO(const string & msg)
{
	_pepoll -> addIOTasks(std::bind(&TcpConnection::send, this, msg));
}

void TcpConnection::shutDown()
{
	if(!_isShutDownWrite)
	{
		_isShutDownWrite = true;
		_socket.shutDownWrite();
	}
}

string TcpConnection::addrToString()
{
	return string(_peerAddr.ip()) + " " +  std::to_string(_peerAddr.port());
}

void TcpConnection::setConnectionCb(TcpConnectionCallBack cb)
{
	_connectionCb = cb;
}
void TcpConnection::setMessageCb(TcpConnectionCallBack cb)
{
	_messageCb = cb;
}
void TcpConnection::setCloseCb(TcpConnectionCallBack cb)
{
	_closeCb = cb;
}
void TcpConnection::handleConnection()
{
	if(_connectionCb)
	{
		_connectionCb(shared_from_this());
	}
}
void TcpConnection::handleMessage()
{
	if(_messageCb)
	{
		_messageCb(shared_from_this());
	}
}
void TcpConnection::handleClose()
{
	if(_closeCb)
	{
		_closeCb(shared_from_this());
	}
}

}//end of namespace wd

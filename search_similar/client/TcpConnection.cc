 ///
 /// @file    TcpConnection.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-23 22:29:11
 ///

#include "TcpConnection.h"
#include <string>

namespace wd
{

TcpConnection::TcpConnection(int fd)
:_socket(fd)
,_socketIO(fd)
,_localAddr(Socket::getLocalAddr(fd))
,_peerAddr(Socket::getPeerAddr(fd))
,_isShutDownWrite(false)
{}
TcpConnection::~TcpConnection()
{
	shutDown();
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
void TcpConnection::shutDown()
{
	if(!_isShutDownWrite)
	{
		_isShutDownWrite = true;
		_socket.shutDownWrite();
	}
}

}//end of namespace wd

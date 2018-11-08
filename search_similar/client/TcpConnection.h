 ///
 /// @file    TcpConnection.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-23 22:03:57
 ///

#ifndef __WD_TCPCONNECTION_H__
#define __WD_TCPCONNECTION_H__

#include "NonCopyAble.h"
#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"

namespace wd
{


class TcpConnection
: NonCopyAble
{
public:
	TcpConnection(int);
	string receive();
	void send(const string &);
	void shutDown();
	~TcpConnection();
private:
	Socket _socket;
	SocketIO _socketIO;
	InetAddress _localAddr;
	InetAddress _peerAddr;
	bool _isShutDownWrite;
};


};//end of namespace wd

#endif

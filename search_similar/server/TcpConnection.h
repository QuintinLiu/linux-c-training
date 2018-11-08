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
#include <memory>
#include <functional>

namespace wd
{

class EpollPoller;
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

class TcpConnection
: NonCopyAble
, public std::enable_shared_from_this<TcpConnection>
{
public:
using TcpConnectionCallBack = std::function<void(const TcpConnectionPtr &)>;

	TcpConnection(int, EpollPoller *);
	string receive();
	void send(const string &);
	void sendInIO(const string &);
	void shutDown();
	void peerAddr();
	~TcpConnection();
	string addrToString();
	void setConnectionCb(TcpConnectionCallBack);
	void setMessageCb(TcpConnectionCallBack);
	void setCloseCb(TcpConnectionCallBack);
	void handleConnection();
	void handleMessage();
	void handleClose();
private:
	Socket _socket;
	SocketIO _socketIO;
	InetAddress _localAddr;
	InetAddress _peerAddr;
	bool _isShutDownWrite;
	EpollPoller * _pepoll;
	TcpConnectionCallBack _connectionCb;
	TcpConnectionCallBack _messageCb;
	TcpConnectionCallBack _closeCb;
};


}//end of namespace wd

#endif

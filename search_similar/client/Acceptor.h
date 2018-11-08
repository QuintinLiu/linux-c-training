 ///
 /// @file    Acceptor.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-23 20:53:00
 ///

#ifndef __WD_ACCEPTOR_H__
#define __WD_ACCEPTOR_H__

#include "InetAddress.h"
#include "Socket.h"

namespace wd
{
class Acceptor
{
public:
	Acceptor(int, InetAddress &);
	void ready();
	void bind();
	void listen();
	int accept();
	void connect();
	int fd();
	void setReuseAddr(bool);
	void setReusePort(bool);
private:
	InetAddress _addr;
	Socket _listenSock;
};

}//end of namespace wd

#endif 

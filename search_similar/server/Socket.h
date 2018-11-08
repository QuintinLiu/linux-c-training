 ///
 /// @file    Socket.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-22 22:23:07
 ///

#ifndef __WD_SOCKET_H__
#define __WD_SOCKET_H__
#include "NonCopyAble.h"
#include "InetAddress.h"

namespace wd
{

class Socket
: NonCopyAble
{
public:
	Socket(int);
	~Socket();
	int fd();
	void shutDownWrite() const;
	void nonBlock() const;
	
	static InetAddress getLocalAddr(int);
	static InetAddress getPeerAddr(int);
private:
	int _fd;
};

}//end of namespace wd

#endif

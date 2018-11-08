 ///
 /// @file    InetAddress.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-22 21:59:18
 ///

#include "InetAddress.h"
#include <arpa/inet.h>
#include <string.h>

namespace wd
{

InetAddress::InetAddress(const char * addr, unsigned short port)
{
	::memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = inet_addr(addr);
}
InetAddress::InetAddress(unsigned short port)
{
	::memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = INADDR_ANY;
}
InetAddress::InetAddress(struct sockaddr_in addr)
:_addr(addr)
{}
string InetAddress::ip() const
{
	return string(inet_ntoa(_addr.sin_addr));
}
unsigned short InetAddress::port() const
{
	return ntohs(_addr.sin_port);
}
struct sockaddr_in * InetAddress::getInetAddressPtr()
{
	return &_addr;
}

}//end of namespace wd


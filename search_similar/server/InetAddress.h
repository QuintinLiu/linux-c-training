 ///
 /// @file    InetAddress.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-22 21:53:18
 ///

#ifndef __WD_INETADDRESS_H__
#define __WD_INETADDRESS_H__

#include <netinet/in.h>
#include <string>
using std::string;

namespace wd
{

class InetAddress
{
public:
	InetAddress(const char *, unsigned short);
	InetAddress(unsigned short);
	InetAddress(struct sockaddr_in);
	string ip() const;
	unsigned short port() const;
	struct sockaddr_in * getInetAddressPtr();
private:
	struct sockaddr_in _addr;
};

}//end of namespace wd

#endif

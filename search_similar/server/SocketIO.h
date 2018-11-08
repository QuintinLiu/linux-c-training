 ///
 /// @file    SocketIO.h
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-22 23:15:04
 ///

#ifndef __WD_SOCKETIO_H__
#define __WD_SOCKETIO_H__

#include <stdio.h>

namespace wd
{

class SocketIO
{
public:
	SocketIO(int);
	size_t readn(char * , size_t);
	size_t writen(const char * , size_t);
	size_t readLine(char * , size_t);
private:
	int _fd;
};

}//end of namespace wd

#endif

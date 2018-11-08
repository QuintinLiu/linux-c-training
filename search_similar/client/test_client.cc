 ///
 /// @file    client.cc
 /// @author  Quintin_Lew(carrot_lt@163.com)
 /// @date    2018-08-24 17:44:16
 ///

#include "Acceptor.h"
#include "SocketUtility.h"
#include "TcpConnection.h"
#include <iostream>
using std::cout;
using std::endl;
using namespace wd;

void test()
{
	InetAddress addr("192.168.176.128", 8888);
	int peerFd = createSocketFd();
	Acceptor acceptor(peerFd, addr);
	acceptor.connect();

	TcpConnection conn(peerFd);
	string recvmsg = conn.receive();
	cout << recvmsg;

	string sendmsg;
	while(std::getline(std::cin, sendmsg))
	{
		conn.send(sendmsg + '\n');
		recvmsg = conn.receive();
		cout << "receive: " << recvmsg << endl;
	}
}


int main()
{
	test();
	return 0;
}

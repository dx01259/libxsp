#ifndef NET_XSPSYNCSOCKET_H_
#define NET_XSPSYNCSOCKET_H_

#include<stdio.h>
#include"XSPSocket.h"

class CXSPSyncSocket:
		public CXSPSocket
{
public:
	CXSPSyncSocket();
	virtual ~CXSPSyncSocket();
public:
	bool InitSocket(int smethd, int stype, int ptype, const char *ip="127.0.0.1", const int port=8000);
	int CreateSocket();
	int SetoptSocket(const struct xsp_sockopt *sockopt);
	int ListenSocket();
	int AcceptSocket(struct sockaddr *addrp);
	int ConnectSocket(const struct sockaddr *addrp, socklen_t addrlen);
	int CloseSocket();
	int ReceiveData(void *data, const size_t size, struct sockaddr *addrp, size_t *addrlenp);
	int SendData(const void *data, const size_t size, const struct sockaddr *addrp, size_t addrlen);
};

#endif /* NET_XSPSYNCSOCKET_H_ */

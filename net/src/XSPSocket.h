#ifndef NET_XSPSOCKET_H_
#define NET_XSPSOCKET_H_

#include"xnet.h"
#include<stdio.h>

class CXSPSocket
{
public:
	CXSPSocket();
	virtual ~CXSPSocket();
public:
	virtual bool InitSocket(int smethd, int stype, int ptype, const char *ip="127.0.0.1", const int port=8000)
	{
		return false;
	}
	virtual int CreateSocket()
	{
		return 0;
	}
	virtual int SetoptSocket(const struct xsp_sockopt *sockopt)
	{
		return 0;
	}
	virtual int ListenSocket()
	{
		return 0;
	}
	virtual int AcceptSocket(struct sockaddr *addrp)
	{
		return 0;
	}
	virtual int ConnectSocket(const struct sockaddr *addrp, socklen_t addrlen)
	{
		return 0;
	}
	virtual int CloseSocket()
	{
		return 0;
	}
	string GetSocketError();
protected:
	inline bool isServiceSocket()
	{
		return (m_socket.sock_methd == SOCK_SERVICE);
	}

	inline int GetSocketID()
	{
		return m_socket.sock_fd;
	}
protected:
	struct xsp_socket m_socket;
};

#endif /* NET_XSPSOCKET_H_ */

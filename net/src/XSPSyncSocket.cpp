#include <net/XSPSyncSocket.h>

CXSPSyncSocket::CXSPSyncSocket()
{
	// TODO Auto-generated constructor stub

}

CXSPSyncSocket::~CXSPSyncSocket()
{
	// TODO Auto-generated destructor stub
}

bool CXSPSyncSocket::InitSocket(int smethd, int stype, int ptype, const char *ip, const int port)
{
	if(ip && port>0)
	{
		init_socket(&m_socket, smethd, stype, ptype, port, ip);
		return true;
	}
	return false;
}

int CXSPSyncSocket::CreateSocket()
{
	return create_socket(&m_socket);
}

int CXSPSyncSocket::SetoptSocket(const struct xsp_sockopt *sockopt)
{
	return set_socketopt(&m_socket, sockopt);
}

int CXSPSyncSocket::ListenSocket()
{
	return listen_socket(&m_socket);
}

int CXSPSyncSocket::AcceptSocket(struct sockaddr *addrp)
{
	return accept_socket(&m_socket, addrp);
}

int CXSPSyncSocket::ConnectSocket(const struct sockaddr *addrp, socklen_t addrlen)
{
	return connect_socket(&m_socket, addrp, addrlen);
}

int CXSPSyncSocket::CloseSocket()
{
	return close_socket(&m_socket);
}

int CXSPSyncSocket::ReceiveData(void *data, const size_t size, struct sockaddr *addrp, size_t *addrlenp)
{
	return recv_data(&m_socket, data, size, addrp, addrlenp);
}

int CXSPSyncSocket::SendData(const void *data, const size_t size, const struct sockaddr *addrp, size_t addrlen)
{
	return send_data(&m_socket, data, size, addrp, addrlen);
}

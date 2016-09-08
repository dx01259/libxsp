#include"XSPInetAddr.h"
#include<string.h>
#include<netdb.h>
#include<sys/socket.h>
#include<arpa/inet.h>

CXSPInetAddr::CXSPInetAddr(u_short usPort, const char *addr) {
	// TODO Auto-generated constructor stub
	m_socktype = -1;
	memset(&m_addr, 0, sizeof(m_addr));
	struct hostent *hp;
	if((hp = gethostbyname(addr)))
	{
		m_addr.sin_family = AF_INET;
		m_addr.sin_addr = *(struct in_addr *)hp->h_addr_list[0];
		m_addr.sin_port = usPort;
	}
}

CXSPInetAddr::CXSPInetAddr(const sockaddr_in addr)
{
	this->m_socktype = -1;
	this->m_addr = addr;
}

CXSPInetAddr::~CXSPInetAddr() {
	// TODO Auto-generated destructor stub
}

string CXSPInetAddr::GetIp() const
{
	return inet_ntoa(m_addr.sin_addr);
}
u_short CXSPInetAddr::GetPort() const
{
	return ntohs(m_addr.sin_port);
}

void CXSPInetAddr::SetSockType(const int socktype)
{
	this->m_socktype = socktype;
}

int  CXSPInetAddr::GetSockType() const
{
	return this->m_socktype;
}

void CXSPInetAddr::GetInetAddr(sockaddr_in &addr)
{
	addr = this->m_addr;
}

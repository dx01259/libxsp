#ifndef NET_XSPINETADDR_H_
#define NET_XSPINETADDR_H_

#include"comm/xdatatype.h"
#include<netinet/in.h>

#include<string>
using namespace std;


class CXSPInetAddr
{
public:
	CXSPInetAddr(u_short usPort, const char *addr);
	CXSPInetAddr(const sockaddr_in addr);
	virtual ~CXSPInetAddr();
public:
	string GetIp() const;
	u_short GetPort() const;
	void SetSockType(const int socktype);
	int  GetSockType() const;
	void GetInetAddr(sockaddr_in &addr);
private:
	sockaddr_in m_addr;
	int m_socktype;
};

#endif /* NET_XSPINETADDR_H_ */

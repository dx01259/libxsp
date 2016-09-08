#ifndef NET_XSPINETTABLE_H_
#define NET_XSPINETTABLE_H_

#include"XSPInetAddr.h"
#include"stl/XSPMap.h"

class CXSPInetTable
{
public:
	CXSPInetTable();
	virtual ~CXSPInetTable();
public:
	int GetIpAddr(const string &szID, CXSPInetAddr *addrp);
	void AddIpAddr(const string &szID,  const CXSPInetAddr *addrp);
	void ClearIpAddr();
private:
	CXSPMap<string, CXSPInetAddr *> m_iptables;
};

#endif /* NET_XSPINETTABLE_H_ */

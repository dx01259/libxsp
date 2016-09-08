#ifndef NET_XSPLOCALIPLIST_H_
#define NET_XSPLOCALIPLIST_H_

#include<string>
#include<vector>
using namespace std;

class CXSPLocalIpList
{
public:
	CXSPLocalIpList();
	virtual ~CXSPLocalIpList();
public:
	int GetLocalIpList();
	int GetLocalIpList(vector<string> &ips);
	bool islocalIp(const char *ip);
private:
	vector<string> m_iplist;
};

#endif /* NET_XSPLOCALIPLIST_H_ */

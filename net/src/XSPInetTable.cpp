#include"net/XSPInetTable.h"
#include"comm/xfun.h"

CXSPInetTable::CXSPInetTable()
{
	// TODO Auto-generated constructor stub
	this->m_iptables.Clear();
}

CXSPInetTable::~CXSPInetTable()
{
	// TODO Auto-generated destructor stub
	this->ClearIpAddr();
}

int CXSPInetTable::GetIpAddr(const string &szID, CXSPInetAddr *addrp)
{
	if(m_iptables.Find(szID, addrp) != 0)
	{
		return -1;
	}
	return 0;
}

void CXSPInetTable::AddIpAddr(const string &szID,  const CXSPInetAddr *addrp)
{
	CXSPInetAddr *newAddrp = NULL;
	if(m_iptables.Find(szID, newAddrp) != 0)
	{
		u_short uPort = addrp->GetPort();
		const char *ip = addrp->GetIp().c_str();
		newAddrp = new CXSPInetAddr(uPort, ip);
	}
	this->m_iptables.Set(szID, newAddrp);
}

void CXSPInetTable::ClearIpAddr()
{
	vector<CXSPInetAddr *> vInetAddrs;
	m_iptables.GetAllValues(vInetAddrs);
	for(size_t i=0; i < vInetAddrs.size(); ++i)
	{
		CXSPInetAddr *tmp = vInetAddrs[i];
		DELETE_MEMORY(tmp);
	}
	m_iptables.Clear();
}


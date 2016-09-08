
#include"XSPMutex.h"
#include<string.h>
#include"erro/xerro.h"

CXSPMutex::CXSPMutex()
{
	// TODO Auto-generated constructor stub
	mutex_init(m_mutex);
}

CXSPMutex::~CXSPMutex()
{
	// TODO Auto-generated destructor stub
	mutex_destroy(m_mutex);
}

int CXSPMutex::Lock()
{
	int ret = mutex_lock(m_mutex);
	if(0 != ret)
	{
		err_ret(GetLastError(ERROR_MUTEX_LOCK));
	}
	return ret;
}

int CXSPMutex::Unlock()
{
	int ret = mutex_unlock(m_mutex);
	if(0 != ret)
	{
		err_ret(GetLastError(ERROR_MUTEX_UNLOCK));
		return ret;
	}
	return 0;
}

int CXSPMutex::Trylock()
{
	int ret = mutex_trylock(m_mutex);
	if(0 != ret)
	{
		err_ret(GetLastError(ERROR_MUTEX_TRYLOCK));
		return ret;
	}
	return 0;
}

bool CXSPMutex::Islocked()
{
	return mutex_islocked(m_mutex);
}




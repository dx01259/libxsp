#include<mm/XSPIOBufferPtr.h>
#include"erro/xerro.h"

CXSPIOBufferPtr::CXSPIOBufferPtr(int size)
{
	// TODO Auto-generated constructor stub
	init_mmset(&m_mmset);
	set_mmset_resize(&m_mmset, size);
}

CXSPIOBufferPtr::~CXSPIOBufferPtr()
{
	// TODO Auto-generated destructor stub
	this->Clear();
}

int CXSPIOBufferPtr::Resize(int size)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = set_mmset_resize(&m_mmset, size);
	if(0 > ret)
	{
		err_msg(GetLastError(ERROR_RESIZE));
		return -1;
	}
	return ret;
}
int CXSPIOBufferPtr::GetSize()
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = get_mmset_size(&m_mmset);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::GetLength()
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = get_mmset_length(&m_mmset);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Read(char &value)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = read_mmset(&m_mmset, value);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Read(long &value)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = read_mmset(&m_mmset, value);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Read(string &value, int len)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = read_mmset(&m_mmset, value, len);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Read(int8 &value)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = read_mmset(&m_mmset, value);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Read(int16 &value)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = read_mmset(&m_mmset, value);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Read(int32 &value)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = read_mmset(&m_mmset, value);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Read(int64 &value)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = read_mmset(&m_mmset, value);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Write(const int8 value)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = write_mmset(&m_mmset, value);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Write(const int16 value)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = write_mmset(&m_mmset, value);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Write(const int32 value)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = write_mmset(&m_mmset, value);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Write(const int64 value)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = write_mmset(&m_mmset, value);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Write(const long value)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = write_mmset(&m_mmset, value);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Write(const char value)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = write_mmset(&m_mmset, value);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Write(const string value)
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = write_mmset(&m_mmset, value);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

int CXSPIOBufferPtr::Clear()
{
	AUTO_GUARD(g, THREAD_MUTEX, this->m_mutex);
	int ret = clear_mmset(&this->m_mmset);
	if(-1 == ret)
	{
		err_msg(GetLastError(ERROR_PARAMS_VALUE));
		return -1;
	}
	return ret;
}

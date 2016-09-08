/*
 * XSPLog.cpp
 *
 *  Created on: 2015年7月2日
 *      Author: dengxu
 */

#include <file/XSPLog.h>
#include "thread/XSPTime.h"
#include "erro/xerro.h"

CXSPLog::CXSPLog()
{
	m_logopen = false;
	pthread_attr_init(&this->m_attr);
	sem_init(&this->m_semt, 0, 0);
}

CXSPLog::~CXSPLog()
{
	pthread_attr_destroy(&this->m_attr);
	sem_destroy(&m_semt);
}

int CXSPLog::Open(const char *prefix, int level)
{
	init_log(&this->m_log, level, prefix);
	int ret = create_log(&this->m_log);
	if(0 == ret)
	{
		m_logopen = true;
		this->CreateThread(1, this->m_attr);
	}
	return ret;
}

void CXSPLog::WriteValist(const int iMaxLen, const char *fmt, va_list argp)
{
	try
	{
		if(iMaxLen < 2048)
		{
			char strTemp[2048];
			bzero(strTemp, sizeof(strTemp));
			vsnprintf(strTemp, sizeof(strTemp), fmt, argp);
			WriteString(&strTemp[0], strlen(strTemp));
		}
		else
		{
			vector<char> strTemp(iMaxLen);
			bzero(&strTemp[0], iMaxLen);
			vsnprintf(&strTemp[0], iMaxLen, fmt, argp);
			WriteString(&strTemp[0], strlen(&strTemp[0]));
		}
	}catch(...)
	{
		err_msg("WriteValist happen exceptions");
	}
}

bool CXSPLog::WriteBuff(const int8 level, const int iMaxLen, const char *fmt, ...)
{
	if(this->m_logopen &&
			m_log.xlevel >= level)
	{
		va_list argp;
		va_start(argp, fmt);
		WriteValist(iMaxLen, fmt, argp);
		va_end(argp);
		return true;
	}
	return false;
}

void CXSPLog::WriteBuff2File()
{
	if(this->m_logopen)
	{
		AUTO_GUARD(g, THREAD_MUTEX, m_mutex);
		while(!m_buff.empty())
		{
			std::deque<byte>::iterator iter2;
			std::deque<byte>::iterator iter1 = m_buff.begin();
			if(m_buff.size() > 1024000000)
			{
				iter2 = iter1+1024000000;
			}
			else
			{
				iter2 = m_buff.end();
			}
			std::deque<byte> tmpBuf;
			tmpBuf.insert(tmpBuf.begin(), iter1, iter2);
			m_buff.erase(iter1, iter2);
			fwrite_log(&this->m_log, (const char *)&tmpBuf[0], tmpBuf.size());
			tmpBuf.clear();
		}
	}
}

long CXSPLog::WriteString(const char *data, const size_t len)
{
	if(this->m_logopen)
	{
		if(NULL==data || len<=0)
		{
			return false;
		}
		AUTO_GUARD(g, THREAD_MUTEX, m_mutex);
		try
		{
			tLogTime now;
			const char *strTime = now.GetLocalTime();
			m_buff.insert(m_buff.end(), strTime, strTime+strlen(strTime));
			m_buff.insert(m_buff.end(), data, data+len);
			this->m_buff.push_back('\r');
			this->m_buff.push_back('\n');
			sem_post(&m_semt);
		}catch(std::bad_alloc &ba)
		{
			this->m_buff.clear();
			err_msg(ba.what());
		}catch(...)
		{
			err_msg("WriteString happen exception");
		}
	}
	return 0;
}

long CXSPLog::WriteBinHex(const byte *data, size_t len)
{
	return 0;
}

void CXSPLog::Char2Hex(const char *chr, const size_t len, vector<char> &hex)
{
	hex.reserve( len * 3 + 1 );
	char *p = &hex[0];
	for(uint32 i=0,j=0; i<len; i++,j+=3)
	{
		sprintf( p+j, "%02X ", ((byte *)chr)[i] );   /*转换为16进制显示*/
	}
}

void *CXSPLog::thread_proc()
{
	while(true)
	{
		CXSPTimeout cnt;
		u_long milliseconds = 300000;
		struct timespec spec;
		cnt.timeofNowTime(spec, milliseconds);
		int ret = sem_timedwait(&m_semt, &spec);
		if(0 != ret)
		{
			if(errno==EINTR || errno==EAGAIN)
				continue;
			if(ETIMEDOUT==errno)
			{
				//write data to file
				WriteBuff2File();
			}
		}
		else{
			//write data to file
			WriteBuff2File();
		}
		fflush(m_log.xpfd);
	}
	return (void *)0;
}

void CXSPLog::clean_func()
{

}











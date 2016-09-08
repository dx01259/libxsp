/*
 * XSPLog.h
 *
 *  Created on: 2015年7月2日
 *      Author: dengxu
 */

#ifndef FILE_XSPLOG_H_
#define FILE_XSPLOG_H_

#include"xlog.h"
#include"comm/xdatatype.h"
#include"thread/XSPThread.h"
#include<sys/time.h>

#include<deque>
#include<string>
using namespace std;

class CXSPLog:
		public CXSPThread
{
	struct tLogTime
	{
		const char *GetLocalTime()
		{
			struct timeval val;
			struct timezone zone;
			bzero(strTime, sizeof(strTime));
			gettimeofday(&val, &zone);
			struct tm *t = localtime(&val.tv_sec);
			snprintf(strTime, sizeof(strTime), "[%04u-%02u-%02u %02u:%02u:%02u:%03lu]\t",
					t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour,t->tm_min,t->tm_sec, val.tv_usec/1000);
			return strTime;
		}
		char strTime[30];
	};
public:
	CXSPLog();
	virtual ~CXSPLog();
public:
	int Open(const char *prefix, int level);
	bool WriteBuff(const int8 level, const int iMaxLen, const char *fmt, ...);
	void WriteBuff2File();
	void WriteValist(const int iMaxLen, const char *fmt, va_list argp);
	long WriteString(const char *data, const size_t len);
	long WriteBinHex(const byte *data, size_t len);
protected:
	void Char2Hex(const char *chr, const size_t len, vector<char> &hex);
protected:
	void *thread_proc();
	void clean_func();
private:
	xsp_log m_log;
	deque<byte> m_buff;
	pthread_attr_t m_attr;
	bool m_logopen;
	sem_t m_semt;
};

#endif /* FILE_XSPLOG_H_ */

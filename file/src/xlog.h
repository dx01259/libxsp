/*
 * xlog.h
 *
 *  Created on: 2015年4月23日
 *      Author: dengxu
 */

#ifndef FILE_XLOG_H_
#define FILE_XLOG_H_

#include<stdio.h>
#include<string.h>
#include<limits.h>
#include<dirent.h>
#include<stdarg.h>
#include<sys/stat.h>
#include<errno.h>
#include"erro/xerro.h"

#include<string>
using namespace std;

#ifndef MAX_LOG_SIZE
#define MAX_LOG_SIZE 1024*1024*10 //10M
#endif

enum{
	LOG_SYS=0,
	LOG_ERR,
	LOG_MSG,
	LOG_DEBUG,
	LOG_ALL
};

struct xsp_log
{
	FILE  *xpfd;              //pointer of log file
	int    xlevel;            //the level to write data
	string szName;            //the name of log file
	int    serrno;            //the system errors
	int    erroid;            //the programmer errors
	int    logid;             //the id of log file
	struct tm dtime;          //the time that create log file
};

extern int errno;

inline void set_log_error(struct xsp_log *plog, int erro);
inline string get_log_error(struct xsp_log *plog);

/**
 * init_log - initialize the structure xsp_log
 * @plog: pointer to xsp_log
 * @level: the level of log
 * @name:  the name of log file
 */
inline void init_log(xsp_log *plog, int level, const char *namep)
{
	if(plog)
	{
		plog->xpfd   = NULL;
		plog->xlevel = level;
		plog->serrno = 0;
		plog->erroid = -1;
		plog->szName = namep;
		plog->logid  = 1;
	}
}

/**
 * create_log - to create a log file
 * @plog: pointer to xsp_log
 */
inline int create_log(xsp_log *plog)
{
	if(plog)
	{
		string szPath;
		szPath.resize(PATH_MAX);
		time_t seconds;
		time(&seconds);
		struct tm *daytime = NULL;
		daytime = localtime(&seconds);
		if(NULL == opendir("./log"))
		{
			if(0 != mkdir("./log", S_IRWXU))
			{
				set_log_error(plog, ERROR_CREATE_DIRECTORY);
				return -2;
			}
		}
		snprintf(&szPath[0], PATH_MAX, "./log/%s-%04d-%02d-%02d-file%d.txt", plog->szName.c_str(),
					daytime->tm_year+1900, daytime->tm_mon+1, daytime->tm_mday, plog->logid);
	    plog->xpfd = fopen(szPath.c_str(), "a");
	    if(NULL == plog->xpfd)
	    {
	    	set_log_error(plog, ERROR_OPEN_FILE);
	    	return -3;
	    }
	    plog->dtime.tm_year = daytime->tm_year+1900;
	    plog->dtime.tm_mon  = daytime->tm_mon +1;
	    plog->dtime.tm_mday = daytime->tm_mday;
	    plog->dtime.tm_hour = daytime->tm_hour;
	    plog->dtime.tm_min  = daytime->tm_min;
	    plog->dtime.tm_sec  = daytime->tm_sec;
	    return 0;
	}
	return -1;
}

/**
 * fopen_log - to open a log file
 * @plog: pointer to a log file that to be opened
 */
inline int fopen_log(struct xsp_log *plog)
{
	if(plog)
	{
		if(plog->xpfd)
		{
			++plog->logid;
			return create_log(plog);
		}
	}
	return -1;
}

/**
 * is_next_day - judge that the now day whether
 *               is next day that the time create log file
 * @plog: pointer a effictive log file
 */
inline bool is_next_day(struct xsp_log *plog)
{
	if(plog)
	{
		time_t seconds;
		time(&seconds);
		struct tm *daytime = localtime(&seconds);
		if(
				daytime->tm_year+1900 == plog->dtime.tm_year
				&& daytime->tm_mon+1 == plog->dtime.tm_mon
				&& daytime->tm_mday == plog->dtime.tm_mday
			)
		{
			return false;
		}
		return true;
	}
	return false;
}

/**
 * fwrite_log - write data to log file that plog pointed
 * @plog: pointer to a log file
 * @pData: pointer to data that to writed
 * @len: the length of data to write
 */
inline long fwrite_log(struct xsp_log *plog, const char *pData, size_t len)
{
	if(plog)
	{
		int size = len;
		if(NULL != plog->xpfd)
		{
			if(MAX_LOG_SIZE == ftell(plog->xpfd)
					|| is_next_day(plog)
				)
			{
				int result;
				result = fopen_log(plog);
				if(0 != result) return result;
			}
			while(size > 0)
			{
				int wlen = fwrite(pData, size, 1, plog->xpfd);
				pData += wlen * size;
				size -= wlen*size;
			}
			return size;
		}
	}
	return -1;
}

/**
 * set_log_error - set the error when happen
 * @plog: pointer to the xsp_log object
 * @erro:   a id of erro that to see the head file of error.h
 */
inline void set_log_error(struct xsp_log *plog, int erro)
{
	if(plog)
	{
		plog->serrno = errno;
		plog->erroid = erro;
	}
}

/**
 * get_sock_error - get the error when happen
 * @sockp: pointer to the xsp_log object
 */
inline string get_log_error(struct xsp_log *plog)
{
	if(plog)
	{
		string eTmp = GetLastError(plog->erroid);
		if(plog->serrno != -1)
		{
			eTmp += ":";
			eTmp += strerror(plog->serrno);
		}
		return eTmp;
	}
	return NULL;
}

/**
 * close_log - to close a opened log file
 * @plog: pointer to a opened log file
 */
inline int close_log(struct xsp_log *plog)
{
	if(plog)
	{
		if(0 != fclose(plog->xpfd))
		{
			set_log_error(plog, ERROR_CLOSE_FILE);
			return -1;
		}
		return 0;
	}
	return -2;
}
#endif /* FILE_XLOG_H_ */

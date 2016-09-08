
#ifndef THREAD_XSPTIME_H_
#define THREAD_XSPTIME_H_

#include <comm/xdatatype.h>
#include<limits.h>
#include<sys/time.h>
#include <thread/xtime.h>


class CXSPTime
{
public:
	CXSPTime(time_t utc);
	CXSPTime(int year=1979, int mon=1, int day=1, int hour=0, int min=0, int sec=0);
	virtual ~CXSPTime();
public:
	CXSPTime(const CXSPTime &obj)
	{
		this->operator =(obj);
	}
	CXSPTime &operator=(const CXSPTime &obj)
	{
		if(&obj != this)
		{
			this->m_time = obj.m_time;
		}
		return *this;
	}
	bool SetTime(time_t utc);
	bool SetTime(int year, int mon, int day, int hour, int min, int sec);
	bool SetCurrentTime();
	void GetTime(time_t &utc);
	void GetTime(struct tm &stim);
	string ToString();

	bool IsValidTime() const
	{
		return is_valid_xtime(&this->m_time);
	}
private:
	struct xsp_time m_time;
};

/**
 * the class of CXSPTimeout is
 * count in milliseconds
 */
class CXSPTimeout
{
public:
	CXSPTimeout(const u_long to=0)
	{
		this->m_timeout = to;
		Reset();
	}
	~CXSPTimeout()
	{

	}
public:
	inline void Init(u_long out)
	{
		this->m_timeout = out;
		Reset();
	}
	inline void Reset()
	{
		this->m_stattime = timeGetTime();
	}

	inline u_long Start()
	{
		return (this->m_started=true, this->m_stattime=timeGetTime());
	}

	inline void SetTimeout(u_long out)
	{
		this->m_timeout = out;
	}

	inline bool IsTimeout() const
	{
		return (elapse()>this->m_timeout);
	}

	inline u_long elapse() const
	{
		u_long end = timeGetTime();
		return m_started ? ((m_stattime<=end)?(end-m_stattime):(INT_MAX-end)):0;
	}

	inline u_long timeleft()
	{
		return m_timeout>elapse() ? this->m_timeout-elapse():0;
	}

	inline u_long timeGetTime() const
	{
		u_long uptime = 0;
		struct timespec spec;
		if(clock_gettime(CLOCK_MONOTONIC, &spec) == 0)
		{
			uptime = spec.tv_sec*1000+spec.tv_nsec/1000000;
		}
		return uptime;
	}
	inline bool timeofNowTime(struct timespec &spec, const u_long minisec)
	{
		struct timeval tval;
		int ret = gettimeofday(&tval, NULL);
		spec.tv_sec = tval.tv_sec+minisec/1000;
		u_long minitmp = minisec % 1000;
		u_long usec = tval.tv_usec + minitmp*1000;
		spec.tv_sec += usec/(1000*1000);
		usec %= 1000000;
		spec.tv_nsec = usec*1000;
		spec.tv_sec += (spec.tv_nsec/(1000*1000*1000));
		spec.tv_nsec %= (1000*1000*1000);
		return 0==ret?true:false;
	}
private:
	bool   m_started;
	u_long m_stattime;
	u_long m_timeout;
};

#endif /* THREAD_XSPTIME_H_ */

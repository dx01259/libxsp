
#ifndef THREAD_XTIME_H_
#define THREAD_XTIME_H_

#include<time.h>
#include<string.h>
#include<stdio.h>

#include<string>
using namespace std;

struct xsp_time
{
	time_t    utc;
	struct tm stim;
};

inline bool init_xtime(struct xsp_time *tp, time_t utc)
{
	if(tp)
	{
		tp->utc = utc;
		gmtime_r(&utc, &tp->stim);
	}
	return false;
}

inline bool init_xtime(
		struct xsp_time *tp,
		int year,
		int month,
		int day,
		int hours,
		int minutes,
		int seconds)
{
	if(tp)
	{
		tp->stim.tm_year = year;
		tp->stim.tm_mon  = month;
		tp->stim.tm_mday = day;
		tp->stim.tm_hour = hours;
		tp->stim.tm_min  = minutes;
		tp->stim.tm_sec  = seconds;
		tp->utc = mktime(&tp->stim);
		return true;
	}
	return false;
}

inline time_t set_local_xtime(struct xsp_time *tp)
{
	if(tp)
	{
		time(&tp->utc);
		tp->stim = *localtime(&tp->utc);
		tp->stim.tm_mon +=1;
		tp->stim.tm_year+=1900;
		return tp->utc;
	}
	return 0;
}

inline bool is_valid_xtime(const struct xsp_time *tp)
{
	if(tp)
	{
		if(tp->stim.tm_sec<0 || tp->stim.tm_sec>59)
			return false;
		else if(tp->stim.tm_min<0 || tp->stim.tm_min>59)
			return false;
		else if(tp->stim.tm_hour<0 || tp->stim.tm_hour>23)
			return false;
		else if(tp->stim.tm_wday<0 || tp->stim.tm_mday>31)
			return false;
		else if(tp->stim.tm_mon<0 || tp->stim.tm_mon>12)
			return false;
		else if(tp->stim.tm_year<1900)
			return false;

		return true;

	}
	return false;
}

inline string string_xtime(struct xsp_time *tp)
{
	string szTime;
	if(tp)
	{
		szTime.resize(20);
		memset(&szTime[0], 0, 20);
		snprintf(&szTime[0], 20, "%04u-%02u-%02u %02u:%02u:%02u",
			tp->stim.tm_year, tp->stim.tm_mon, tp->stim.tm_mday,
			tp->stim.tm_hour, tp->stim.tm_min, tp->stim.tm_sec
		);
	}
	return szTime;
}


#endif /* THREAD_XTIME_H_ */

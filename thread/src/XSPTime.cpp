
#include <thread/XSPTime.h>

CXSPTime::CXSPTime(time_t utc)
{
	init_xtime(&this->m_time, utc);
}

CXSPTime::CXSPTime(int year, int mon, int day, int hour, int min, int sec)
{
	// TODO Auto-generated constructor stub
	init_xtime(&this->m_time, year, mon, day, hour, min, sec);
}

CXSPTime::~CXSPTime()
{
	// TODO Auto-generated destructor stub
}

bool CXSPTime::SetTime(time_t utc)
{
	return init_xtime(&m_time, utc);
}

bool CXSPTime::SetTime(int year, int mon, int day, int hour, int min, int sec)
{
	return init_xtime(&m_time, year, mon, day, hour, min, sec);
}

bool CXSPTime::SetCurrentTime()
{
	return set_local_xtime(&m_time) ? false:true;
}

void CXSPTime::GetTime(time_t &utc)
{
	utc = this->m_time.utc;
}

void CXSPTime::GetTime(struct tm &stim)
{
	stim = this->m_time.stim;
}

string CXSPTime::ToString()
{
	return string_xtime(&this->m_time);
}

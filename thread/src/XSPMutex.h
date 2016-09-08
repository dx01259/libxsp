
#ifndef THREAD_XSPMUTEX_H_
#define THREAD_XSPMUTEX_H_

#include <thread/xmutex.h>

#include<string>
using namespace std;

class CXSPMutex
{
public:
	CXSPMutex();
	virtual ~CXSPMutex();
	int Lock();
	int Unlock();
	int Trylock();
	bool Islocked();
private:
	struct xsp_mutex m_mutex;
};

template<class T>
class CXSPAutoGuard
{
public:
	CXSPAutoGuard(T &mtx) : m_mtx( mtx )
	{
		m_mtx.Lock();
	}
	~CXSPAutoGuard(void)
	{
		m_mtx.Unlock();
	}
protected:
	T &m_mtx;
};

#ifndef THREAD_MUTEX
#define THREAD_MUTEX CXSPMutex
#endif

//自动锁定义
//guard：CXSPAutoGuard的局部变量名称
//mutex: 锁的类型
//mtx：   锁的对象
#ifndef AUTO_GUARD
#define AUTO_GUARD(guard, mutex, mtx)\
	CXSPAutoGuard<mutex> guard(mtx)
#endif

#endif /* THREAD_XSPMUTEX_H_ */

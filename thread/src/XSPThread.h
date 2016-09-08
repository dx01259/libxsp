
#ifndef THREAD_XSPTHREAD_H_
#define THREAD_XSPTHREAD_H_

#include<semaphore.h>
#include <thread/xthread.h>
#include"XSPMutex.h"

class CXSPThread
{
public:
	CXSPThread();
	virtual ~CXSPThread();
public:
	void InitThread(
			const pthread_mutexattr_t *mutexattr,
			const pthread_condattr_t *condattr,
			const pthread_func func=NULL,
			const pthread_cleanup_func cfunc=NULL,
			const pthread_once_func ofunc=NULL,
			const pthread_key_func kfunc=NULL
		);
	int CreateThread(int numbers,
			pthread_attr_t attr,
			int scope=PTHREAD_SCOPE_PROCESS,
			int detach = PTHREAD_CREATE_JOINABLE,
			int prio=0
		);
	int WaitoutThread(u_long millisecond);
protected:
	virtual void *thread_proc();
	virtual void clean_func();
private:
	static void *thread_addr(void *arg);
	static void init_clean_func(void *arg);
protected:
	CXSPMutex m_mutex;
private:
	sem_t m_sem;
	struct xsp_thread m_thread;
};

#endif /* THREAD_XSPTHREAD_H_ */

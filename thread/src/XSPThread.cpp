
#include<thread/XSPThread.h>
#include"comm/xfun.h"
#include"XSPTime.h"
#include"erro/xerro.h"

CXSPThread::CXSPThread()
{
	sem_init(&this->m_sem, 0, 0);
	InitThread(NULL, NULL);
}

CXSPThread::~CXSPThread()
{
	destroy_thread(&m_thread);
	sem_destroy(&this->m_sem);
}

void CXSPThread::InitThread(
		const pthread_mutexattr_t *mutexattr,
		const pthread_condattr_t *condattr,
		const pthread_func func,
		const pthread_cleanup_func cfunc,
		const pthread_once_func ofunc,
		const pthread_key_func kfunc
	)
{
	pthread_func in_func = func ? func:thread_addr;
	pthread_cleanup_func in_cfunc = cfunc ? cfunc:init_clean_func;
	pthread_once_func in_ofunc = ofunc;
	pthread_key_func in_kfunc = kfunc;

	init_thread(&this->m_thread, mutexattr, condattr, in_func, in_cfunc, in_ofunc, in_kfunc);

}

int CXSPThread::CreateThread(int numbers, pthread_attr_t attr, int scope, int detach, int prio)
{
	int ret;
	this->m_thread.attr = attr;
	ret = attr_init_thread(&this->m_thread);
	if(ret != 0) return ret;
	ret = attr_setscope_thread(&this->m_thread, scope);
	if(ret != 0) return ret;
	ret = attr_setdetachstate_thread(&this->m_thread, detach);
	if(ret !=0 ) return ret;
	ret = attr_setschedparam_thread(&this->m_thread, prio);
	if(ret != 0) return ret;
	for(int i=0; i<numbers; ++i)
	{
		 create_thread(&this->m_thread, this);
		 err_msg("Create thread %lu", this->m_thread.pthrid[i]);
	}
	return 0;
}

int CXSPThread::WaitoutThread(u_long millisecond)
{
	switch(millisecond)
	{
	case -1:
		{
			while(true)
			{
				m_mutex.Lock();
				if(m_thread.pthrid.size()>0)
				{
					pthread_cancel(m_thread.pthrid[0]);
					m_mutex.Unlock();
				}
				else
				{
					m_mutex.Unlock();
					break;
				}
				sem_wait(&this->m_sem);
			}
		}
		break;
	default:
		{
			while(true)
			{
				this->m_mutex.Lock();
				if(m_thread.pthrid.size()>0)
				{
					pthread_cancel(m_thread.pthrid[0]);
					this->m_mutex.Unlock();
				}
				else{
					this->m_mutex.Unlock();
					break;
				}
				CXSPTimeout cnt;
				struct timespec tim;
				cnt.timeofNowTime(tim, millisecond);
				sem_timedwait(&this->m_sem, &tim);
			}
		}
		break;
	}

	return 0;
}

void *CXSPThread::thread_addr(void *arg)
{
	void *ret = NULL;
	CXSPThread *thrp =  (CXSPThread *)arg;
	struct xsp_thread *ptmp = &thrp->m_thread;
	cleanup_push_thread(ptmp, thrp);
	ret = thrp->thread_proc();
	cleanup_pop_thread(1);

	err_msg("thread_addr quit %lu", pthread_self());

	return ret;
}

void CXSPThread::init_clean_func(void *arg)
{
	CXSPThread *thrp =  (CXSPThread *)arg;
	thrp->m_mutex.Lock();
	remove_vector_data(thrp->m_thread.pthrid, pthread_self());
	sem_post(&thrp->m_sem);
	thrp->m_mutex.Unlock();
	thrp->clean_func();

	err_msg("init_clean_func quit %lu\n", pthread_self());
}

void *CXSPThread::thread_proc()
{
	return 0;
}

void CXSPThread::clean_func()
{

}





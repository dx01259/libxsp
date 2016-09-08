
#ifndef STL_XSPDEQUE_H_
#define STL_XSPDEQUE_H_

#include<semaphore.h>
#include<errno.h>
#include"thread/XSPMutex.h"
#include<bits/local_lim.h>
#include<stl/xdeque.h>
#include<thread/XSPTime.h>

#ifndef errno
extern int errno;
#endif

template<class V, class MUTEX=THREAD_MUTEX>
class CXSPDeque
{
public:
	CXSPDeque(const size_t maxSize=50000){
		m_bAbort  = false;
		m_bsNotep = false;
		m_bsNotfl = false;
		ResetInit(maxSize);
	}
	virtual ~CXSPDeque()
	{
		AbortWati();
		SafeClose();
	}
	size_t ResetInit(size_t maxSize)
	{
		maxSize = maxSize>SEM_VALUE_MAX ? SEM_VALUE_MAX-1:maxSize;
		if(this->m_bAbort)
		{
			this->m_bAbort = false;
			SafeClose();
		}
		int rete = sem_init(&m_sNotEmpty, 0, 0);
		int retf = sem_init(&m_sNotFull, 0, m_deque.maxsize);
		return ((rete==retf) && rete==0) ?
				this->m_bsNotep=true,this->m_bsNotfl=true,
				init_deque(&this->m_deque, maxSize):0;
	}

	inline void SetMaxQueueSize(size_t maxSize)
	{
		maxSize = maxSize>SEM_VALUE_MAX ? SEM_VALUE_MAX-1:maxSize;
		reset_deque(&this->m_deque, maxSize);
	}

	inline void SafeClose()
	{
		if(!this->m_bsNotep &&
				!this->m_bsNotfl)
		{
			sem_destroy(&m_sNotEmpty);
			sem_destroy(&m_sNotFull);
			m_bsNotep = false;
			m_bsNotfl = false;
		}
	}

	inline void AbortWati()
	{
		this->m_bAbort = false;
		sem_post(&this->m_sNotEmpty);
		sem_post(&this->m_sNotFull);

	}

	inline bool isFull(int offset=0)
	{
		return size_deque(&this->m_deque, offset)>=m_deque.maxsize ? 1:0;
	}

	inline bool isEmpty()
	{
		return empty_deque(&this->m_deque);
	}

	inline bool isAbort()
	{
		return this->m_bAbort;
	}

	bool Front(V &data, u_long out=0)
	{
		if(this->WaitforNotEmpty(out))
		{
			AUTO_GUARD(g, MUTEX, this->m_mutex);
			if(front_deque(&this->m_deque, data))
			{
				return true;
			}
			return false;
		}
		if(this->m_bAbort)
		{
			return false;
		}
		return false;
	}

	bool PopFront(V &data, u_long out=0)
	{
		if(!this->m_bAbort)
		{
			if(this->WaitforNotEmpty(out))
			{
				AUTO_GUARD(g, MUTEX, this->m_mutex);
				if(pop_front_deque(&this->m_deque, data))
				{
					sem_post(&this->m_sNotFull);
					return true;
				}
				return false;
			}
		}
		else
		{
			AUTO_GUARD(g, MUTEX, this->m_mutex);
			if(!this->isEmpty())
			{
				return pop_front_deque(&this->m_deque, data);
			}
			return false;
		}
		return false;
	}

	bool PushFront(const V &data, u_long out=0)
	{
		if(!this->m_bAbort)
		{
			if(this->WaitforNotFull(out))
			{
				AUTO_GUARD(g, MUTEX, this->m_mutex);
				if(!this->isFull())
				{
					if(push_front_deque(&this->m_deque, data))
					{
						sem_post(&this->m_sNotEmpty);
						return true;
					}
					return false;
				}
			}
		}
		return false;
	}

	bool Back(V &data, u_long out=0)
	{
		if(this->WaitforNotEmpty(out))
		{
			AUTO_GUARD(g, MUTEX, this->m_mutex);
			if(back_deque(&this->m_deque, data))
			{
				return true;
			}
			return false;
		}
		if(this->m_bAbort)
		{
			return false;
		}
		return false;
	}

	bool PopBack(V &data, u_long out=0)
	{
		if(!this->m_bAbort)
		{
			if(this->WaitforNotEmpty(out))
			{
				AUTO_GUARD(g, MUTEX, this->m_mutex);
				if(pop_back_deque(&this->m_deque, data))
				{
					sem_post(&this->m_sNotFull);
					return true;
				}
				return false;
			}
		}
		else
		{
			AUTO_GUARD(g, MUTEX, this->m_mutex);
			if(!empty_deque(&this->m_deque))
			{
				return pop_back_deque(&this->m_deque, data);
			}
			return false;
		}
		return false;

	}

	bool PushBack(const V &data)
	{
		if(!this->m_bAbort)
		{
			AUTO_GUARD(g, MUTEX, this->m_mutex);
			if(!this->isFull())
			{
				if(push_back_deque(&this->m_deque, data))
				{
					sem_post(&this->m_sNotEmpty);
					return true;
				}
			}
		}
		return false;
	}

	bool Insert(typename std::deque<V>::const_iterator where, V *first, V *last)
	{
		if(!this->m_bAbort)
		{
			AUTO_GUARD(g, MUTEX, this->m_mutex);
			int offset = last-first;
			if(offset<0) offset=-offset;
			if(!this->isFull(offset))
			{
				if(insert_deque(&this->m_deque, where, first, last))
				{
					sem_post(&this->m_sNotEmpty);
					return true;
				}
			}
		}
		return false;
	}

	inline size_t Size()
	{
		return size_deque(&this->m_deque);
	}

	void Clear()
	{
		AbortWati();
		SafeClose();
		clear_deque(&this->m_deque);
	}
protected:
	bool WaitforNotEmpty(u_long out = 0)
	{
		CXSPTimeout cnt;
		cnt.Init(out);
		while(!this->m_bAbort && this->m_bsNotep)
		{
			int ret = 0;
			u_long w = cnt.timeleft();
			switch(out)
			{
			case -1:
				ret = sem_wait(&this->m_sNotEmpty);
				break;
			default:
				struct timespec tim;
				cnt.timeofNowTime(tim, w);
				ret = sem_timedwait(&this->m_sNotEmpty, &tim);
				break;
			}
			if(0 == ret && !this->m_bAbort)
			{
				return true;
			}
			else if(EINTR == errno)
			{
				continue;
			}
			else{
				return false;
			}
		}
		return false;
	}

	bool WaitforNotFull(u_long out = 0)
	{
		CXSPTimeout cnt;
		cnt.Init(out);
		while(!this->m_bAbort && this->m_bsNotfl)
		{
			int ret = 0;
			u_long w = cnt.timeleft();
			switch(out)
			{
			case -1:
				ret = sem_wait(&this->m_sNotEmpty);
				break;
			default:
				struct timespec tim;
				cnt.timeofNowTime(tim, w);
				ret = sem_timedwait(&this->m_sNotEmpty, &tim);
				break;
			}
			if(0 == ret && !this->m_bAbort)
			{
				return true;
			}
			else if(EINTR == errno)
			{
				continue;
			}
			else{
				return false;
			}
		}
		return false;
	}

private:
	bool m_bAbort;
	MUTEX m_mutex;
	struct xsp_deque<V>  m_deque;
	sem_t m_sNotEmpty;
	sem_t m_sNotFull;
	bool  m_bsNotep;
	bool  m_bsNotfl;
};

#endif /* STL_XSPDEQUE_H_ */

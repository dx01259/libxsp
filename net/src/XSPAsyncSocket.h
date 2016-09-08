#ifndef NET_XSPASYNCSOCKET_H_
#define NET_XSPASYNCSOCKET_H_

#include"mm/xmm.h"
#include"thread/XSPMutex.h"
#include"thread/XSPThread.h"
#include"stl/XSPDeque.h"
#include"stl/XSPMap.h"
#include"xnet.h"

#include<vector>
using namespace std;

class CXSPAsyncSocket :
		public CXSPThread
{
	struct EpollWaitObject
	{
		EpollWaitObject()
		{
			Init();
		}
		int epollId;                  /* the object be create by call epoll_create1, and must be closed unused */
		EPOLLEVENT epollEvent;        /* the epoll event structure that base epoll event */
		SIGSETT    sigset;            /* the object of signal wheather be caught*/
		volatile int maxEvent;        /* how much I/O event*/
		vector<EPOLLEVENT> vEvents;   /* All events object that to be noticed*/

		void Init()
		{
			maxEvent = 0;
			vEvents.clear();
		}
	};
	enum ID_THEAD_TYPE
	{
		THEAD_ACCEPT=0,
		THEAD_RECEIVE,
		THEAD_SEND
	}
public:
	CXSPAsyncSocket();
	virtual ~CXSPAsyncSocket();
public:
	int Start(const char *ip="127.0.0.1", /*The IP Address of socket*/
			const int port=8000,          /*The Port of socket*/
			const int style=SOCK_STREAM,  /*The Type of socket*/
			const int lThrNum=1,          /*The numbers of threads to listen socket*/
			const int rThrNum=0,          /*The numbers of threads to receive data */
			const int wThrNum=0);         /*The numbers of threads to send data    */
	void Stop();
protected:
	int SetAsyncSocket(int &sockfd);
	int AsyncSocketCreate(const char *ip, const int port, const int style);
	int ListenSocket();
	int AcceptSocket(struct sockaddr *addrp);
	int CloseSocket();
public:
	void SetEpollEvent(int sockfd, const int event);
	int SocketEpollCreate();
	int SocketEpollCtl(const int eopt, int &sockfd);
	int SocketEpollAdd(int &sockfd, const int event);
	int SocketEpollModify(int &sockfd, const int event);
	int SocketEpollDel(const int sockfd, const int event);
	int SocketIOEpollWait(int timeout);
	int SetEpollResponseSocketID(void);
protected:
	inline void SafeIncreaseEvent()
	{
		m_mutex.Lock();
		pthread_t thrid = pthread_self();
		struct EpollWaitObject *pEwobj=NULL;
		if(m_ThreadId2Ewobj.Find(thrid, pEwobj) == 0)
		{
			++pEwobj->maxEvent;
			pEwobj->vEvents.resize(pEwobj->maxEvent);
		}
		else
		{
			pEwobj = new EpollWaitObject();
			pEwobj->maxEvent = 1;
			pEwobj->vEvents.resize(pEwobj->maxEvent);
		}
		m_ThreadId2Ewobj.Set(thrid, pEwobj);
		m_mutex.Unlock();
	}

	inline void SafeDecreaseEvent()
	{
		m_mutex.Lock();
		pthread_t thrid = pthread_self();
		struct EpollWaitObject *pEvojb=NULL;
		if(m_ThreadId2Ewobj.Find(thrid, pEvojb) == 0)
		{
			--pEvojb->maxEvent;
			pEvojb->vEvents.resize(pEvojb->maxEvent);
			if(pEvojb->maxEvent<=0)
			{
				m_ThreadId2Ewobj.Remove(thrid);
				delete pEvojb;
				pEvojb = NULL;
			}
			else{
				m_ThreadId2Ewobj.Set(thrid, pEvojb);
			}
		}
		m_mutex.Unlock();
	}
protected:
	void *thread_proc();
	void clean_func();
public:
	CXSPDeque<string> m_deque;
protected:
	struct xsp_socket m_socket;
	CXSPMutex m_mutex;
	pthread_attr_t m_thrAttr;
private:
	CXSPMap<int, struct xsp_thread *> m_Socket2Thread; /* The Thread object to create threads */
	CXSPMap<int, pthread_t> m_Epoll2ThreadID;                    /* The epoll ID to Thread ID*/
	CXSPMap<pthread_t, EpollWaitObject *> m_ThreadId2Ewobj;      /* The id from thread to structure of EpollWaitObject */
};

#endif /* NET_XSPASYNCSOCKET_H_ */

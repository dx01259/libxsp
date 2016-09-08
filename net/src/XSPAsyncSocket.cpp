#include"XSPAsyncSocket.h"
#include"erro/xerro.h"
#include<unistd.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include"comm/xfun.h"

CXSPAsyncSocket::CXSPAsyncSocket()
{
	// TODO Auto-generated constructor stub
	m_Epoll2ThreadID.Clear();
	m_ThreadId2Ewobj.Clear();
	pthread_attr_init(&m_thrAttr);
}

CXSPAsyncSocket::~CXSPAsyncSocket()
{
	// TODO Auto-generated destructor stub
	vector<EpollWaitObject *> vEwObj;
	m_ThreadId2Ewobj.GetAllValues(vEwObj);
	this->m_ThreadId2Ewobj.Clear();//it must be called before follow code
	for(size_t i=0; i<vEwObj.size(); ++i)
	{
		pthread_t thrid=0;
		EpollWaitObject *obj = vEwObj[i];
		if(m_Epoll2ThreadID.Find(obj->epollId, thrid)==0)
		{
			pthread_cancel(thrid);
			m_Epoll2ThreadID.Remove(obj->epollId);
		}
		close(obj->epollId);
		obj->vEvents.clear();
		DELETE_MEMORY(obj);
	}
	pthread_attr_destroy(&m_thrAttr);
}

int CXSPAsyncSocket::SetAsyncSocket(int &sockfd)
{
	if(sockfd > 0)
	{
		int flags = fcntl(sockfd, F_GETFL, 0);
		if(-1 == flags)
		{
			err_ret(GetLastError(ERROR_FCNTL_SOCKET));
			return -1;
		}
		flags |= SOCK_NONBLOCK;
		flags = fcntl(sockfd, F_SETFL, flags);
		if(-1 == flags)
		{
			err_ret(GetLastError(ERROR_FCNTL_SOCKET));
			return -1;
		}
	}
	return sockfd;
}

int CXSPAsyncSocket::AsyncSocketCreate(const char *ip, const int port, const int style)
{
	init_socket(&this->m_socket, SOCK_SERVICE, style, 0, port, ip);
	int sockfd = create_socket(&this->m_socket);
	if(sockfd >= 0)
	{
		SetAsyncSocket(sockfd);
	}
	return sockfd;
}

int CXSPAsyncSocket::ListenSocket()
{
	return listen_socket(&m_socket);
}

int CXSPAsyncSocket::AcceptSocket(struct sockaddr *addrp)
{
	return accept_socket(&m_socket, addrp);
}

int CXSPAsyncSocket::CloseSocket()
{
	return close_socket(&m_socket);
}

int CXSPAsyncSocket::Start(const char *ip,
		const int port,
		const int style,
		const int lThrNum,
		const int rThrNum,
		const int wThrNum)
{
	int sockid = AsyncSocketCreate(ip, port, style);
	if(sockid > 0)
	{
		ListenSocket();
		this->CreateThread(lThrNum, m_thrAttr, PTHREAD_SCOPE_SYSTEM, PTHREAD_CREATE_DETACHED);
	}
	return 0;
}

void CXSPAsyncSocket::Stop()
{
	WaitoutThread(3000);
}

void CXSPAsyncSocket::SetEpollEvent(int sockfd, const int event)
{
	pthread_t thrid = pthread_self();
	struct EpollWaitObject *pEwobj=NULL;
	if(this->m_ThreadId2Ewobj.Find(thrid, pEwobj) == 0)
	{
		pEwobj->epollEvent.data.fd = sockfd;
		pEwobj->epollEvent.events = event;
		m_ThreadId2Ewobj.Set(thrid, pEwobj);//it must be update data
	}
}

int CXSPAsyncSocket::SetEpollResponseSocketID()
{
	return this->m_socket.sock_fd;
}

int CXSPAsyncSocket::SocketEpollCreate()
{
	pthread_t thrid = pthread_self();
	struct EpollWaitObject *pEwobj = new EpollWaitObject();
	if(NULL != pEwobj)
	{
		pEwobj->epollId = epoll_create1(EPOLL_CLOEXEC);
		if(pEwobj->epollId < 0)
		{
			err_ret(GetLastError(ERROR_EPOLL_CREATE));
			return -1;
		}
		this->m_ThreadId2Ewobj.Set(thrid, pEwobj);
		return pEwobj->epollId;
	}
	return -2;
}

int CXSPAsyncSocket::SocketEpollCtl(const int eopt, int &sockfd)
{
	int ret = -1;
	pthread_t thrid = pthread_self();
	struct EpollWaitObject *pEwobj=NULL;
	if(this->m_ThreadId2Ewobj.Find(thrid, pEwobj) == 0)
	{
		if(pEwobj->epollId > 0)
		{
			if(EPOLL_CTL_ADD == eopt ||
					EPOLL_CTL_MOD == eopt)
			{
				//确保创建的套接子，是非阻塞的。
				SetAsyncSocket(sockfd);
			}
			ret = epoll_ctl(pEwobj->epollId, eopt, sockfd, &pEwobj->epollEvent);
			if(-1 == ret)
			{
				err_ret(GetLastError(ERROR_EPOLL_CTL));
				return -2;
			}
			m_ThreadId2Ewobj.Set(thrid, pEwobj);
			if(EPOLL_CTL_ADD == eopt)
			{
				SafeIncreaseEvent();
			}
		}
	}

	return ret;
}

int CXSPAsyncSocket::SocketEpollAdd(int &sockfd, const int event)
{
	SetEpollEvent(sockfd, event);
	return SocketEpollCtl(EPOLL_CTL_ADD, sockfd);
}

int CXSPAsyncSocket::SocketEpollModify(int &sockfd, const int event)
{
	SetEpollEvent(sockfd, event);
	return SocketEpollCtl(EPOLL_CTL_MOD, sockfd);
}

int CXSPAsyncSocket::SocketEpollDel(const int sockfd, const int event)
{
	EPOLLEVENT epollevent;
	epollevent.data.fd = sockfd;
	epollevent.events = event;
	pthread_t thrid = pthread_self();
	struct EpollWaitObject *pEwobj=NULL;
	if(m_ThreadId2Ewobj.Find(thrid, pEwobj) == 0)
	{
		int ret = epoll_ctl(pEwobj->epollId, EPOLL_CTL_DEL, sockfd, &epollevent);
		if(-1 == ret)
		{
			err_ret(GetLastError(ERROR_EPOLL_CTL));
			return -1;
		}
		m_ThreadId2Ewobj.Set(thrid, pEwobj);
		SafeDecreaseEvent();
		return ret;
	}
	return -2;
}

int CXSPAsyncSocket::SocketIOEpollWait(int timeout)
{
	while(true)
	{
		pthread_t thrid = pthread_self();
		EpollWaitObject *pEwobj=NULL;
		if(this->m_ThreadId2Ewobj.Find(thrid, pEwobj) != 0)
		{
			pthread_exit((void *)"the map without the thread data");
			break;
		}
		int &epollId = pEwobj->epollId;
		vector<EPOLLEVENT> &vEvents = pEwobj->vEvents;
		volatile int &maxEvent = pEwobj->maxEvent;
		SIGSETT &sigset = pEwobj->sigset;
		int ret = epoll_pwait(epollId, &vEvents[0], maxEvent, timeout, &sigset);
		if(0 == ret) continue;//timeout
		else if(0 > ret)
		{
			err_ret(GetLastError(ERROR_EPOLL_WAIT));
			if(EINTR == errno || EAGAIN == errno)
				continue;
			pthread_exit((void *)"epoll pwait error");
			return -1;
		}
		int &fds = ret;//监听套接子的个数
		for(int i=0; i<fds && i<maxEvent; ++i)
		{
			struct sockaddr addr;
			EPOLLEVENT &event = vEvents[i];
			if(m_socket.sock_fd == event.data.fd)
			{
				int clifd = 0;
				while( 0 < (clifd = AcceptSocket(&addr)) )
				{
					SetAsyncSocket(clifd);
					SocketEpollAdd(clifd, EPOLLET | EPOLLIN | EPOLLET);
#ifndef Debug
					printf("Add epoll event %d", clifd);
#endif
				}
				if(-1 == clifd && errno != EAGAIN &&
						ECONNABORTED !=errno && EPROTO!=errno && EINTR != errno)
				{
					err_ret(GetLastError(ERROR_ACCEPT_SOCK));
				}
				continue;
			}
			else if(EPOLLIN & event.events)//read data
			{
				int &fd = event.data.fd;
				string szRecvData;
				szRecvData.resize(4);
				int reLen = 0, offset=0;
				while((reLen=read(fd, &szRecvData[offset], 4)) > 0)
				{
					offset += reLen;
					szRecvData.resize(offset+1024);
				}
				if((reLen < 0 && errno==ECONNRESET)
						|| (0 == reLen) )
				{
					close(fd);
					SocketEpollDel(fd, event.events);
					continue;
				}
				else if(reLen < 0) continue;
				szRecvData.resize(offset);
				this->m_deque.PushBack(szRecvData);
#ifndef Debug
				printf("recv epoll data from %d", fd);
#endif
			}
			else if(EPOLLOUT & event.events )//write data
			{
				int &fd = event.data.fd;
				string szSendData = (char *)event.data.ptr;
				int datalen = szSendData.size(), sendlen=0;
				while(0 < datalen)
				{
					sendlen=write(fd, szSendData.c_str(), datalen);
					if(sendlen < 0)
					{
						if(-1 == sendlen && errno != EAGAIN)
						{
							err_ret(GetLastError(ERROR_SEND_SOCK));
						}
						break;
					}
					datalen -= sendlen;
				}
			}
		}
	}
	return 0;
}

void *CXSPAsyncSocket::thread_proc()
{
	int &sockid = m_socket.sock_fd;
	SocketEpollCreate();
	SocketEpollAdd(sockid, EPOLLOUT | EPOLLIN | EPOLLET);
	while(true)
	{
		SocketIOEpollWait(10000);
	}
	return NULL;
}

void CXSPAsyncSocket::clean_func()
{
	pthread_t thrid = pthread_self();
	EpollWaitObject *pEwobj=NULL;
	if(this->m_ThreadId2Ewobj.Find(thrid, pEwobj) == 0)
	{
		close(pEwobj->epollId);
		pEwobj->vEvents.clear();
		this->m_ThreadId2Ewobj.Remove(thrid);
		DELETE_MEMORY(pEwobj);
	}
}




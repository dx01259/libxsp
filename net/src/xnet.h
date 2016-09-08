#ifndef NET_XNET_H_
#define NET_XNET_H_

#include"xsocket.h"
#include<netdb.h>
#include<netinet/in.h>
#include<string.h>
#include<erro/xerro.h>

#include<string>

using namespace std;

enum{
	SOCK_SERVICE=0,
	SOCK_CLIENT =1,
};

struct xsp_socket;
struct xsp_sockopt;

struct xsp_socket
{
	int  sock_fd;       //The ID of socket
	int  sock_methd;    //This is SOCK_SERVICE or SOCK_CLIENT socket
	int  sock_type;     //what type of  socket
	int  prot_type;     //what protocol of socket
	int  sock_port;     //the port of socket
	char sock_addr[16]; //the address of socket
	xsp_sockopt *opt;   //the option of socket to be setted
};

struct xsp_sockopt
{
	int level;
	int optname;
	void *optval;
	socklen_t optlen;
};

/**
 * init_socket - initialize the structure xsp_socket
 * @sockp:  pointer to structure of xsp_socket
 * @smethd: the type of socket is SOCK_SERVICE or SOCK_CLIENT
 * @stype:  the type of socket
 * @ptype:  the protocol of socket
 * @port:   the port of socket
 * @addrp:  the address of socket
 *
 * the sockp is must be not null
 *
 * if pbase is null will set default socket
 * sockp->sock_methd= SOCK_SERVICE
 * sockp->sock_type = SOCK_DGRAM;
 * sockp->prot_type = 0;
 * sockp->sock_port = 8000;
 * sockp->sock_addr = "127.0.0.1";
 */
inline void init_socket(
		struct xsp_socket *sockp,
		int smethd=SOCK_SERVICE,
		int stype=SOCK_DGRAM,
		int ptype=0,
		int port=8000,
		const char *addrp="127.0.0.1")
{
	if (NULL != sockp)
	{
		sockp->sock_fd    = -1;
		sockp->sock_methd = smethd;
		sockp->sock_type  = stype;
		sockp->prot_type  = ptype;
		sockp->sock_port  = port;
		memset(sockp->sock_addr, 0, sizeof(sockp->sock_addr));
		memcpy(sockp->sock_addr, addrp, strlen(addrp));
	}
}

/**
 * create_socket - create the service socket to communication
 *			the SOCKET structure parameter of sock_methd must be SOCK_SERVICE
 * @sockp: pointer to the socket object will be created
 * @sockopt: callback the function that the pointer of sockopt to set option of socket
 *        @sockfd: pointer to a effective's socket object
 *        @opt:    the struct of options value to set socket
 */
inline int create_socket(struct xsp_socket *sockp)
{
	if(sockp)
	{
		switch(sockp->sock_methd)
		{
		case SOCK_SERVICE:
			{
				sockp->sock_fd = Socket(AF_INET, sockp->sock_type, 0);
				if(-1 == sockp->sock_fd)
				{
					return -2;
				}
				struct sockaddr_in servaddr;
				memset(&servaddr, 0, sizeof(servaddr));
				servaddr.sin_family = AF_INET;
				servaddr.sin_port = htons(sockp->sock_port);
				servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
				int ret = Bind(sockp->sock_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
				if (-1 == ret)
				{
					return -3;
				}
				return sockp->sock_fd;
			}
			break;
		case SOCK_CLIENT:
			{
				sockp->sock_fd = Socket(AF_INET, sockp->sock_type, 0);
				if(-1 == sockp->sock_fd)
				{
					return -2;
				}
				return sockp->sock_fd;
			}
			break;
		default:break;
		}
	}
	return -4;
}

inline int set_socketopt(
		struct xsp_socket *sockp,
		const struct xsp_sockopt *opt)
{
	if(sockp && opt)
	{
		return Setsockopt(sockp->sock_fd, opt->level, opt->optname, opt->optval, opt->optlen);
	}
	return -2;
}
/**
 * listen_socket - listen for the client's socket
 *			the SOCKET structure parameter of sockType
 *			must be SOCK_STREAM
 * @sockp: pointer to the a tcp service socket
 */
inline int listen_socket(struct xsp_socket *sockp)
{
	if(sockp)
	{
		if(sockp->sock_fd<0)
		{
			err_msg("The socket is invalid");
			return -1;
		}
		if(SOCK_STREAM==sockp->sock_type)
		{
			return Listen(sockp->sock_fd, SOMAXCONN);
		}
	}
	return -3;
}

/**
 * accept_socket - accept the client of socket
 *			the SOCKET structure parameter of proType
 *			must be SOCK_DGRAM or SOCK_STREAM
 * @sockp: pointer to the tcp service socket
 * @addrp: pointer to the sockaddr of tcp client socket
 */
inline int accept_socket(struct xsp_socket *sockp, struct sockaddr *addrp)
{
	if(sockp)
	{
		int stype = sockp->sock_type;
		switch(stype)
		{
		case SOCK_DGRAM:break;
		case SOCK_STREAM:
			{
				socklen_t addrlen;
				addrlen = sizeof(struct sockaddr);
				return Accept(sockp->sock_fd, addrp, &addrlen);
			}
			break;
		default:break;
		}
	}
	return -2;
}

/**
 * set_socket_addr - Set the socket address by IP address , realm name and port
 * @name: pointer to the Domain name
 * @port: the port of socket
 * @addrp: pointer to the sockaddr that the socket will be setted
 */
inline int set_socket_addr(const char *name, const int port, struct sockaddr_in *addrp)
{
	struct hostent *hp;
	if ((hp = gethostbyname(name))
			&& NULL != addrp)
	{
		addrp->sin_family = AF_INET;
		addrp->sin_addr = *(struct in_addr *) hp->h_addr_list[0];
		addrp->sin_port = htons(port);
		return 0;
	}
	return -1;
}

/**
 * connect_socket - To connect service socket by socket address
 *			the addrp is must be valid socket address of service
 * @sockp: pointer to a socket of tcp client
 * @addrp: pointer to sockaddr of tcp service socket
 * @addrlen: the length of structure of sockaddr
 */
inline int connect_socket(struct xsp_socket *sockp, const struct sockaddr *addrp, socklen_t addrlen)
{
	if(sockp && addrp)
	{
		if(SOCK_CLIENT == sockp->sock_methd
				&& SOCK_STREAM == sockp->sock_type)
		{
			return Connect(sockp->sock_fd, addrp, addrlen);
		}
	}
	return -2;
}

/**
 * close_socket - close the socket
 * @sockp: pointer to the socket that will be closed
 */
inline int close_socket(struct xsp_socket *sockp)
{
	if(sockp)
	{
		if(0 <= sockp->sock_fd)
		{
			return Close(sockp->sock_fd);
		}
	}
	return -2;
}

/**
 * recv_data - receive data from socket
 * @sockp: pointer to the structure of xsp_socket
 * @data: pointer to the data from the buffer
 * @size: the length of data buff
 * @addrp: pointer to structure of sockaddr
 * @addrlenp: pointer to the length of sockaddr
 */
inline ssize_t recv_data(struct xsp_socket *sockp,
		void *data,
		const size_t size,
		struct sockaddr *addrp,
		size_t *addrlenp
	)
{
	if(NULL == sockp)  return -1;
	int ret = sockp->sock_type;
	ssize_t datalen = 0;
	switch(ret)
	{
	case SOCK_DGRAM:
		if(NULL==addrp || NULL==addrlenp)
			return -2;
		datalen = Recvfrom(sockp->sock_fd, data, size, 0, addrp, addrlenp);
		break;
	case SOCK_STREAM:
		datalen = Recv(sockp->sock_fd, data, size, 0);
		break;
	default:break;
	}
	return datalen;
}

/*
 * send_data - send data to socket buffer
 * @sockp: pointer to the structure of xsp_socket
 * @data: the data to send to buffer
 * @size: the length of data to sended
 * @addrp: pointer to structure sockaddr
 * @addrlen: the length of sockaddr
 */
inline int send_data(struct xsp_socket *sockp,
		const void *data,
		const size_t size,
		const struct sockaddr *addrp,
		size_t addrlen)
{
	if(NULL == sockp)  return -1;
	int ret = sockp->sock_type;
	int datalen = 0;
	switch(ret)
	{
	case SOCK_DGRAM:
		datalen = Sendto(sockp->sock_fd, data, size, 0, addrp, addrlen);
		break;
	case SOCK_STREAM:
		datalen = Send(sockp->sock_fd, data, size, 0);
		break;
	default:break;
	}
	return datalen;
}

#endif /* NET_XNET_H_ */

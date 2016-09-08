#include<errno.h>
#include<erro/xerro.h>
#include<net/xsocket.h>
#include<unistd.h>

int Socket(int domain, int type, int protocol)
{
	int ret = socket(domain, type, protocol);
	if(-1 == ret)
	{
		err_ret("Create socket object is error");
	}
	return ret;
}

int Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
	int ret = setsockopt(fd, level, optname, optval, optlen);
	if(-1 == ret)
	{
		err_ret("Setsockopt socket of %d is error", fd);
	}
	return ret;
}

int Bind(int fd, const struct sockaddr * addrp, socklen_t len)
{
	int ret = bind(fd, addrp, len);
	if(-1 == ret)
	{
		err_ret("Bind socket of %d is error", fd);
	}
	return ret;
}

int Listen(int fd, int n)
{
	int ret = listen(fd, n);
	if(-1 == ret)
	{
		err_ret("Listen socket of %d is error", fd);
	}
	return 0;
}

int Accept(int fd, struct sockaddr *__restrict addrp, socklen_t *__restrict addr_len)
{
	int ret = accept(fd, addrp, addr_len);
	if(-1 == ret)
	{
		if(errno!=EAGAIN &&
			errno!=EWOULDBLOCK &&
			errno!=EINTR &&
			errno!=ECONNABORTED)
		{
			err_ret("Accept socket of %d is error", fd);
			return ret;
		}
		return -2;
	}
	return ret;
}

int Connect(int fd, const struct sockaddr * addrp, socklen_t len)
{
	int ret = connect(fd, addrp, len);
	if(-1 == ret)
	{
		if(errno!=EINTR &&
			errno!=EAGAIN)
		{
			err_ret("Connect socket of %d is error", fd);
			return ret;
		}
		return -2;
	}
	return ret;
}

int Close(int fd)
{
	int ret = close(fd);
	if(-1 == ret)
	{
		if(errno!=EINTR)
		{
			err_ret("Close socket of %d is error", fd);
			return ret;
		}
		return -2;
	}
	return ret;
}

ssize_t Recvfrom(int fd, void *__restrict buf, size_t n, int flags,
		struct sockaddr *__restrict addrp, socklen_t *__restrict addr_len)
{
	ssize_t ret = recvfrom(fd, buf, n, flags, addrp, addr_len);
	if(-1 == ret)
	{
		if(errno!=EAGAIN &&
			errno!=EWOULDBLOCK &&
			errno!=EINTR &&
			errno!=ECONNABORTED)
		{
			err_ret("Recvfrom datas from socket %d happen error", fd);
			return ret;
		}
		return -2;
	}
	return ret;
}

ssize_t Recv(int fd, void *buf, size_t n, int flags)
{
	ssize_t ret = recv(fd, buf, n, flags);
	if(-1 == ret)
	{
		if(errno!=EAGAIN &&
			errno!=EWOULDBLOCK &&
			errno!=EINTR &&
			errno!=ECONNABORTED)
		{
			err_ret("Recv datas from socket %d happen error", fd);
			return ret;
		}
		return -2;
	}
	return ret;
}

ssize_t Sendto(int fd, __const void *buf, size_t n, int flags, const struct sockaddr *addrp,
		       socklen_t addr_len)
{
	int ret = sendto(fd, buf, n, flags, addrp, addr_len);
	if(-1 == ret)
	{
		if(errno!=EAGAIN &&
			errno!=EWOULDBLOCK &&
			errno!=EINTR &&
			errno!=ECONNABORTED)
		{
			err_ret("Sendto datas to socket %d happen error", fd);
			return ret;
		}
		return -2;
	}
	return ret;
}

ssize_t Send(int fd, const void *buf, size_t n, int flags)
{
	int ret = send(fd, buf, n, flags);
	if(-1 == ret)
	{
		if(errno!=EAGAIN &&
			errno!=EWOULDBLOCK &&
			errno!=EINTR &&
			errno!=ECONNABORTED)
		{
			err_ret("Send datas to socket %d happen error", fd);
			return ret;
		}
		return -2;
	}
	return ret;
}



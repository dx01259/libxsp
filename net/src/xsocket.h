#ifndef NET_XSOCKET_H_
#define NET_XSOCKET_H_

#include<sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int Socket(int domain, int type, int protocol);

extern int Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen);

extern int Bind(int fd, const struct sockaddr * addrp, socklen_t len);

extern int Listen(int fd, int n);

extern int Accept(int fd, struct sockaddr *__restrict addrp, socklen_t *__restrict addr_len);

extern int Connect(int fd, const struct sockaddr * addrp, socklen_t len);

extern int Close(int fd);

extern ssize_t Recvfrom(int fd, void *__restrict buf, size_t n, int flags,
		struct sockaddr *__restrict addrp, socklen_t *__restrict addr_len);

extern ssize_t Recv(int fd, void *buf, size_t n, int flags);

extern ssize_t Sendto(int fd, __const void *buf, size_t n, int flags, const struct sockaddr *addrp,
		       socklen_t addr_len);

extern ssize_t Send(int fd, const void *buf, size_t n, int flags);

#ifdef __cplusplus
}
#endif

#endif /* NET_XSOCKET_H_ */

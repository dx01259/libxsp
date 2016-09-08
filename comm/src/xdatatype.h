#ifndef COMM_XDATATYPE_H_
#define COMM_XDATATYPE_H_

#include<limits.h>
#include<stdio.h>
#include<stdint.h>
#include<sys/types.h>
#include<bits/sigset.h>
#include<sys/epoll.h>

typedef unsigned char byte;
typedef unsigned short u_short;
typedef unsigned int   u_int;
typedef unsigned long  u_long;

typedef void * IO_HANDLE;

typedef __int8_t int8;
typedef __uint8_t uint8;

typedef __int16_t int16;
typedef __uint16_t uint16;

typedef __int32_t int32;
typedef __uint32_t uint32;

typedef __int64_t int64;
typedef __uint64_t uint64;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#ifndef INVALID_IO_HANDLE
#define INVALID_IO_HANDLE (IO_HANDLE)(-1L)
#endif

typedef epoll_event EPOLLEVENT;
typedef __sigset_t  SIGSETT;

#endif /* COMM_XDATATYPE_H_ */

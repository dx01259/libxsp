#ifndef ERRO_XERRO_H_
#define ERRO_XERRO_H_

#include<stdio.h>
#include"xdef.h"

#ifndef MAXLINE
#define MAXLINE	4096	/* max line length*/
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int log_to_stderr;
extern int err_to_stderr;

extern void err_dump(const char *, ...);
extern void err_msg(const char *, ...);
extern void err_quit(const char *, ...);
extern void err_exit(int, const char *, ...);
extern void err_ret(const char *, ...);
extern void err_sys(const char *, ...);

extern void	log_msg(const char *, ...);
extern void	log_open(const char *, int, int);
extern void	log_quit(const char *, ...);
extern void	log_ret(const char *, ...);
extern void	log_sys(const char *, ...);

#ifdef __cplusplus
}
#endif

#endif /* ERRO_XERRO_H_ */

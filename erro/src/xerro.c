#include<stdio.h>
#include<errno.h>
#include<erro/xerro.h>
#include<syslog.h>
#include<stdarg.h>
#include<stdlib.h>
#include<string.h>
/*
 * Caller must define and set this: nonzero if
 * interactive, zero if daemon
 */
int log_to_stderr=1;
int err_to_stderr=1;

/**********************************************************
 ***** the error from stdout and file process functions ***
***********************************************************/

static void err_doit(int, int, const char *, va_list);

/*
 * Nonfatal error related to a system call.
 * Print a message and return.
 */
void err_ret(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
}

/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void err_sys(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	exit(1);
}

/*
 * Fatal error unrelated to a system call.
 * Error code passed as explict parameter.
 * Print a message and terminate.
 */
void err_exit(int error, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_doit(1, error, fmt, ap);
	va_end(ap);
	exit(1);
}

/*
 * Fatal error related to a system call.
 * Print a message, dump core, and terminate.
 */
void err_dump(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	abort(); 		/* dump core and terminate */
	exit(1);		/* shouldn't get here*/
}

/*
 * Nonfatal error unrelated to a system call.
 * Print a message and return.
 */
void err_msg(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap);
}

/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 */
void err_quit(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap);
	exit(1);
}

/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag".
 */
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
	char	buf[MAXLINE];
	vsnprintf(buf, MAXLINE, fmt, ap);
	if(errnoflag)
	{
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf), ": %s", strerror(error));
	}
	if(err_to_stderr)
	{
		strcat(buf, "\n");
		fflush(stdout);		/* in case stdout and stderr are the same*/
		fputs(buf, stderr);
		fflush(NULL);		/* flushes all stdio ouput streams */
	}else{
		//调用打印日志信息的函数
	}
}



/**********************************************************
 *****    the error from daemon process functions       ***
***********************************************************/

static void log_doit(int, int, const char *, va_list ap);


/*
 * Initialize syslog(), if running as daemon.
 */
void log_open(const char *ident, int option, int facility)
{
	if(log_to_stderr == 0)
		openlog(ident, option, facility);
}

/*
 * Nofatal error related to a system call.
 * Print a message with the system's errno value and return.
 */
void log_ret(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	log_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
}

/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void log_sys(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	log_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}

/*
 * Nonfatal error unrelated to a system call.
 * Print a message and return.
 */
void log_msg(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
}

/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 */
void log_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}

/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag" and "priority".
 */
static void log_doit(int errnoflag, int priority, const char *fmt, va_list ap)
{
	int		errno_save;
	char	buf[MAXLINE];
	errno_save = errno; 		/* value caller might want printed */
	vsnprintf(buf, MAXLINE, fmt, ap);
	if(errnoflag)
	{
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf), ": %s", strerror(errno_save));
	}
	strcat(buf, "\n");
	if(log_to_stderr)
	{
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}else{
		syslog(priority, buf);
	}
}



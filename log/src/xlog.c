#include "xlog.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

volatile int g_level = 0;


void log_set(int level)
{
    g_level = level;
}

static void log_doit(int level, int errnoflag, 
    int error, const char *fmt, va_list arg)
{
    if(level > g_level)
    {
        char buff[MAXLINE]={0};
        vsnprintf(buff, MAXLINE, fmt, arg);
        if(errnoflag)
            snprintf(buff+strlen(buff), MAXLINE-strlen(buff), ":%s", strerror(error));
#ifdef RUM_RELEASE
        
#else
        if(XLOG_ERR == level)
        {
            fflush(stderr);
            fputs(buff, stderr);
            fflush(stderr);
        }
        else{
            fflush(stdout);
            fputs(buff, stdout);
            fflush(stdout);
        }
#endif
    }
}

void log_quit(int level, int error, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    log_doit(level, 1, error, fmt, arg);
    va_end(arg);
    exit(1);
}

void log_sys(int level, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    log_doit(level, 0, 0, fmt, arg);
    va_end(arg);
}

void log_err(int level, int error, const char *fmt, ...)
{   
    va_list arg;
    va_start(arg, fmt);
    log_doit(level, 1, error, fmt, arg);
    va_end(arg);
}

void log_msg(int level, const char *fmt, ...)
{   
    va_list arg;
    va_start(arg, fmt);
    log_doit(level, 0, 0, fmt, arg);
    va_end(arg);
}

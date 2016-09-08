#include<stdio.h>
#include<unistd.h>


#define MAXLINE 4096

extern volatile int g_level;

enum xlog_type
{
    XLOG_QUIT=0,
    XLOG_SYS,
    XLOG_ERR,
    XLOG_MSG,
    XLOG_DEBUG,
};

void log_set(int level);
void log_quit(int level, int error, const char *fmt, ...);
void log_sys(int level, const char *fmt, ...);
void log_err(int level, int error, const char *fmt, ...);
void log_msg(int level, const char *fmt, ...);

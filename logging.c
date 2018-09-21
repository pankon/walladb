/***************************************
 * logging.c
 * 
 * Walla Db
 * Pankonworks
 * Nathan Pankowsky
 * Logs to file
 * Tue Sep 17 09:21:00 2018
***************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdarg.h> /* va_list, va_args, va_end, vfprintf */
#include <time.h>   /* time_t, time, localtime_r, asctime_r (thread-safe) */
#include <pthread.h>

void Log(char *level, char *mesg, va_list args)
{
    static char *time_format = "%D %T";
    
    time_t current_time = {0};
    struct tm result = {0};
    char stime[32];
    int len = 0;
    pthread_t pid = NULL;

    pid = pthread_self();

    current_time = time(NULL);
    localtime_r(&current_time, &result);
    strftime(stime, 32, time_format, &result);

    fprintf(stderr, level, stime, (unsigned long)pid);
    vfprintf(stderr, mesg, args);
    fputc('\n', stderr);
}

void LogError(char *mesg, ...)
{
    static char *error = "[error (%s:%lx)] ";
    
    va_list args;
    va_start(args, mesg);

    Log(error, mesg, args);

    va_end(args);
}

void LogDebug(char *mesg, ...)
{
#ifdef LOG_DEBUG
    static char *debug = "[debug (%s:%lx)] ";

    va_list args;
    va_start(args, mesg);

    Log(debug, mesg, args);

    va_end(args);
#endif
}

void LogInfo(char *mesg, ...)
{
#ifdef LOG_INFO
    static char *info = "[info  (%s:%lx)] ";

    va_list args;
    va_start(args, mesg);

    Log(info, mesg, args);

    va_end(args);
#endif
}

void LogVerbose(char *mesg, ...)
{
#ifdef LOG_VERBOSE
    static char *info = "[verbose(%s:%lx)] ";

    va_list args;
    va_start(args, mesg);

    Log(info, mesg, args);

    va_end(args);
#endif
}
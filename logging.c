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
#include <stdlib.h>
#include <stdarg.h> /* va_list, va_args, va_end, vfprintf */
#include <time.h>   /* time_t, time, localtime_r, asctime_r (thread-safe) */
#include <pthread.h>

#include "logging.h"

log_t *LogStdErr(void)
{
    static log_t stderr_log = { 0, -1 };
    
    stderr_log.fp = stderr;
    
    return &stderr_log;
}

log_t *LogCreate(char *path)
{
    log_t *log = NULL;
    
    if (NULL == (log = malloc(sizeof(log_t))))
    {
        fprintf(stderr, "[LogCreate] malloc error\n");
        return (NULL);
    }
    
    log->fp = fopen(path, "a");
    if (NULL == log->fp)
    {
        fprintf(stderr, "[LogCreate] Could not open log\n");
        return (NULL);
    }
    
    log->is_builtin = 0;
    
    return (log);
}

log_t *LogCreateFromStdStream(FILE *fp)
{
    log_t *log = NULL;
    
    if (NULL == fp)
    {
        fprintf(stderr, "[LogCreate] Could not open stdstream"
                        " log\n");
        return (NULL);
    }
    
    if (NULL == (log = malloc(sizeof(log_t))))
    {
        fprintf(stderr, "[LogCreate] malloc error\n");
        return (NULL);
    }
        
    log->fp = fp;
    log->is_builtin = 1;
    
    return (log);
}

void LogDestroy(log_t *log)
{
    /* also closes log */
    if (NULL == log)
    {
        return;
    }
    
    if (log->is_builtin == -1)
    {
        /* is stderr static log */
        return;
    }
    
    if (!log->is_builtin)
    {
        fclose(log->fp);
        log->fp = NULL;
    }

    free(log);
    log = NULL;
}

void Log(log_t *log, char *level, char *mesg, va_list args)
{
    static char *time_format = "%D %T";
    
    time_t current_time = {0};
    struct tm result = {0};
    char stime[32];
    int len = 0;
    pthread_t pid = (pthread_t)0;
    
    if (NULL == log)
    {
        return;
    }
    
    if (NULL == log->fp)
    {
        return;
    }

    pid = pthread_self();

    current_time = time(NULL);
    localtime_r(&current_time, &result);
    strftime(stime, 32, time_format, &result);

    fprintf(log->fp, level, stime, (unsigned long)pid);
    vfprintf(log->fp, mesg, args);
    fputc('\n', log->fp);
}

void LogError(log_t *log, char *mesg, ...)
{
    static char *error = "[error (%s:%lx)] ";
    
    va_list args;
    va_start(args, mesg);

    Log(log, error, mesg, args);

    va_end(args);
}

void LogDebug(log_t *log, char *mesg, ...)
{
#ifdef LOG_DEBUG
    static char *debug = "[debug (%s:%lx)] ";

    va_list args;
    va_start(args, mesg);

    Log(log, debug, mesg, args);

    va_end(args);
#endif
}

void LogInfo(log_t *log, char *mesg, ...)
{
#ifdef LOG_INFO
    static char *info = "[info  (%s:%lx)] ";

    va_list args;
    va_start(args, mesg);

    Log(log, info, mesg, args);

    va_end(args);
#endif
}

void LogVerbose(log_t *log, char *mesg, ...)
{
#ifdef LOG_VERBOSE
    static char *info = "[verbose(%s:%lx)] ";

    va_list args;
    va_start(args, mesg);

    Log(log, info, mesg, args);

    va_end(args);
#endif
}

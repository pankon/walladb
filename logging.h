/***************************************
 * logging.h
 * 
 * Walla Db
 * Pankonworks
 * Nathan Pankowsky
 * Logs to file
 * Tue Sep 17 09:21:00 2018
***************************************/

#ifndef __LOGGING_H__
#define __LOGGING_H__

typedef struct log
{
    FILE *fp;
    int is_builtin;
} log_t;

log_t *LogCreate(char *path);
log_t *LogCreateFromStdStream(FILE *fp);
log_t *LogStdErr(void);
void LogDestroy(log_t *log); /* also closes log */

void LogError(log_t *log, char *mesg, ...);
void LogInfo(log_t *log, char *mesg, ...);
void LogDebug(log_t *log, char *mesg, ...);
void LogVerbose(log_t *log, char *mesg, ...);

#endif /* __LOGGING_H__ */

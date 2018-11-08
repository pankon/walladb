/**************************************
 * walla_db.c
 * 
 * Walla Db
 * Pankonworks
 * Nathan Pankowsky
 * Usage:
 *
 * Service features:
 * OctTree with circbufs
 * One thread per requester
 * 2+ threads per all sensors but one shmem segment
 * Specs:
 * Magic: WALLA-DB
 * Fri Sep 14 14:51:47 2018
***************************************/
#define _GNU_SOURCE
#include <stdio.h>  /* sprintf, asprintf    */
#include <stdlib.h> /* malloc, free         */
#include <string.h> /* strlen, strcpy       */

#include "json.h"
#include "logging.h"
#include "simple_mmap.h"
#include "walla_entry.h"
#include "walla_node.h"
#include "walla_db.h"

typedef struct WallaServer {
    int stub;
} WallaServer_t;

typedef struct WallaClient {
    int stub;
} WallaClient_t;

struct WallaDb {
    char *filename;
    log_t *log;
    FILE *fp;           /* initial stage */
    mmap_t *mmap;
    long length;
    long scale_factor;
    WallaNode_t root;
    long free_list_start;
    int is_server;
    union
    {
        WallaServer_t *server;
        WallaClient_t *client;
    } connection;
};

WallaDb_t *WallaDbCreate(char *filename, long length, long scale_factor, int is_server, int n_layers)
{
    WallaDb_t *walla_db = NULL;
    WallaPos_t *pos = NULL;
    log_t *log = NULL;
    size_t filename_len = 0;
    char *filename_copy = NULL;
    char *log_filename = NULL;

    if (NULL == filename)
    {
        LogError(LogStdErr(), "[WallaDbCreate] no filename passed");
        return (NULL);
    }

    filename_len = strlen(filename);
    if (NULL == (log_filename = malloc(filename_len + 5)))
    {
        LogError(LogStdErr(), "[WallaDbCreate] error in malloc"
                              " of log filename");
    }
    
    strcpy(log_filename, filename);
    strcpy(log_filename + filename_len, ".log");
    
    LogInfo(LogStdErr(), log_filename);
    
    if (NULL == (log = LogCreate(log_filename)))
    {
        LogError(LogStdErr(), "[WallaDbCreate] LogCreate"
                              " error");
        return (NULL);
    }
    
    free(log_filename);
    log_filename = NULL;
   
    if (NULL == (filename_copy = malloc(filename_len + 1)))
    {
        LogError(log, "[WallaDbCreate] error in malloc of str");
        LogDestroy(log);
        return (NULL);
    }

    if (NULL == (pos = WallaPosCreate(log, 0, 0, 0)))
    {
        LogError(log, "[WallaDbCreate] WallaPosCreate error");
        LogDestroy(log);
        return (NULL);
    }

    strcpy(filename_copy, filename);

    if (NULL == (walla_db = malloc(sizeof(WallaDb_t))))
    {
        LogError(log, "[WallaDbCreate] error in malloc");
        LogDestroy(log);
        WallaPosDestroy(log, pos);
        return (NULL);
    }

    walla_db->filename = filename_copy;
    walla_db->fp = NULL;
    walla_db->mmap = NULL;
    walla_db->length = length;
    walla_db->scale_factor = scale_factor;
    walla_db->free_list_start = 0;
    walla_db->log = log;
    
    WallaNodeInit(log, &(walla_db->root), 
                  NULL, 0, length, pos);
    WallaPosDestroy(log, pos);
    pos = NULL;

    walla_db->is_server = is_server;
    walla_db->connection.server = NULL;
    
    LogInfo(log, "[WallaDbCreate] : created new db at: %s\n",
            filename_copy);

    return (walla_db);
}

void WallaDbDestroy(WallaDb_t *walla_db)
{
    if (NULL == walla_db)
    {
        return;
    }

    free(walla_db->filename);
    walla_db->filename = NULL;
    
    SimpleMmapPleaseDisposeOfThisCharArray(walla_db->mmap);
    walla_db->mmap = NULL;
    
    if (NULL != walla_db->fp)
    {
        fclose(walla_db->fp);
        walla_db->fp = NULL;
    }
    
    LogDestroy(walla_db->log);
    walla_db->log = NULL;

    /* TODO setup/teardown of client/server */
    walla_db->connection.server = NULL;

    free(walla_db);
    walla_db = NULL;
}

/* 
 * Create a walladb server
 *    - If the file does not exist, the file will be created
 *      with default parameters
 *    - If the file does exist, the server processes will be
 *      restarted
 */ 
WallaDb_t *WallaDbCreateServer(char *filename)
{
    return (NULL);
}

/* 
 * Shutdown walladb server and dealloc all data
 */ 
WALLA_STATUS WallaDbShutdownServer(WallaDb_t *db)
{
    return (WALLA_NOT_IMPLEMENTED);
}

/* 
 * Connect to the database
 */ 
WallaDb_t *WallaDbConnect(char *filename)
{
    return (NULL);
}

/* 
 * Disconnect from database.
 *    - If the database is owned by this process,
 *      the thread workers are shut down
 *    - If the remote db is shut down,
 *      data can be lost - how do I deal with this?
 * Deallocs db handle
 */ 
WALLA_STATUS WallaDbDisconnect(WallaDb_t *db)
{
    return (WALLA_NOT_IMPLEMENTED);
}

/* 
 * Creates a db and thread workers
 */ 
WallaDb_t *WallaDbCreateDb(char *filename, long length, long scale_factor, int n_layers)
{
    FILE *fp = NULL;
    WallaDb_t *walla_db = NULL;
    WALLA_STATUS status = WALLA_SUCCESS;

    if (NULL == (walla_db = WallaDbCreate(filename, length, scale_factor, WALLA_SERVER, n_layers)))
    {
        LogError(LogStdErr(), "[WallaDbCreateDb] :"
                              " error in db creation");
        return (NULL);
    }

    walla_db->fp = fopen(filename, "w");

    WallaDbWriteMagic(walla_db);
    WallaDbSetBufLen(walla_db, length, scale_factor);

    fwrite(&(walla_db->root), sizeof(walla_db->root), 
           1, walla_db->fp);

    status = WallaDbSetupMemory(walla_db); /* TODO: check for success */
    
    /* TODO: make layers */

    fclose(walla_db->fp);
    walla_db->fp = NULL;

    return (walla_db);
}

/* 
 * Writes magic to new database
 */ 
WALLA_STATUS WallaDbWriteMagic(WallaDb_t *db)
{
    if (NULL == db)
    {
        LogError(LogStdErr(), "[WallaDbWriteMagic] :"
                              " you forgot to pass a db");
        return (WALLA_YOU_FORGOT_TO_PASS_ANYTHING);
    }

    if (NULL == db->fp)
    {
        LogError(db->log, "[WallaDbWriteMagic] no file open");
        return (WALLA_NO_FILE_OPENED);
    }

    fseek(db->fp, 0, SEEK_SET);
    fputs(walla_magic, db->fp);

    return (WALLA_SUCCESS);   
}

/* 
 * Sets universal buffer length
 * Recorded data is multiplied by scale_factor for each layer
 */ 
WALLA_STATUS WallaDbSetBufLen(WallaDb_t *db, long length, long scale_factor)
{
    if (NULL == db)
    {
        LogError(LogStdErr(), "[WallaDbSetBufLen] :"
                 " you forgot to pass a db");
        return (WALLA_YOU_FORGOT_TO_PASS_ANYTHING);
    }

    if (NULL == db->fp)
    {
        LogError(db->log, "[WallaDbSetBufLen] : no file open");
        return (WALLA_NO_FILE_OPENED);
    }

    db->length = length;
    db->scale_factor = scale_factor;

    /* print right after WALLA-DB magic */
    fseek(db->fp, 8, SEEK_SET); 
    fwrite(&length, sizeof(length), 1, db->fp);
    fwrite(&scale_factor, sizeof(scale_factor), 1, db->fp);

    /* TODO: resize existing buffers */

    return (WALLA_SUCCESS);
}

/* 
 * Sets root node
 * Allocates a bunch of blank nodes in the database
 *  as a singly linked list
 * Sets up pointer to incoming sensor data buffer area
 */ 
WALLA_STATUS WallaDbSetupMemory(WallaDb_t *db)
{
    return (WALLA_NOT_IMPLEMENTED);
}

/* 
 * Allows sensors to submit data
 */ 
WALLA_STATUS WallaDbSubmitEntry(WallaDb_t *db, WallaPos_t *pos, long epoch, double data)
{
    return (WALLA_NOT_IMPLEMENTED);
}

/* 
 * Queries the db and returns a json object
 * Should accept query like:
 *    {   min: (xmin, ymin, zmin, tmin)
 *        max: (xmax, ymax, zmax, tmax)
 *    }
 *  and return:
 *  "(x, y, z, t)": {
 *     "avg" : 0.324232
 *     "max" : 324.1232
 *     "min" : 0.010230
 *     "std" : 13.32459
 *  }
 */ 
char *WallaDbQuery(WallaDb_t *db, char *query)
{
    return (NULL);
}



/* 
 * Get raw WallaEntry_t struct by point
 * May retrieve closest point
 * 
 */ 
WallaEntry_t *WallaDbQueryInfoByPoint(WallaDb_t *db, WallaPos_t *pos)
{
    return (NULL);
}

/* 
 * Get closest WallaEntry_t struct by time
 * 
 */ 
WallaEntry_t *WallaDbQueryInfoByPointAndTime(WallaDb_t *db, WallaPos_t *pos, long epoch)
{
    return (NULL);
}

/* 
 * Get stddev by point
 */ 
double WallaDbQueryStdDevByPoint(WallaDb_t *db, WallaPos_t *pos)
{
    return (-1);
}

/* 
 * Get closest stddev by time and position
 */ 
WallaEntry_t *WallaDbQueryStdDevByPointAndTime(WallaDb_t *db, WallaPos_t *pos, long epoch)
{
    return (NULL);
}

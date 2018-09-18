/**************************************
 * walla_db.h
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

#ifndef __WALLA_DB_H__
#define __WALLA_DB_H__

#include "walla_entry.h"

typedef struct WallaPos {
    long x;
    long y;
    long z;
} WallaPos_t;

typedef struct WallaNodeInfo {
    WallaPos_t pos;
    long epoch_start;
    long epoch_end;
    double average;
    double max;
    double min;
    double stdev;
} WallaNodeInfo_t;

typedef struct WallaDb WallaDb_t;

typedef enum
{
    WALLA_SUCCESS,
    WALLA_OOPS,
    WALLA_BYE,
    WALLA_BAD_FORMAT,
    WALLA_NOT_IMPLEMENTED
} WALLA_STATUS;

/* 
 * Create a WallaDb_t server
 *    - If the file does not exist, the file will be created
 *      with default parameters
 *    - If the file does exist, the server processes will be
 *      restarted
 */ 
WallaDb_t *WallaDbCreateServer(char *filename);

/* 
 * Shutdown WallaDb_t server and dealloc all data
 */ 
WALLA_STATUS WallaDbShutdownServer(WallaDb_t *db);

/* 
 * Connect to the database
 */ 
WallaDb_t *WallaDbConnect(char *filename);

/* 
 * Disconnect from database.
 *    - If the database is owned by this process,
 *      the thread workers are shut down
 *    - If the remote db is shut down,
 *      data can be lost - how do I deal with this?
 * Deallocs db handle
 */ 
WALLA_STATUS WallaDbDisconnect(WallaDb_t *db);

/* 
 * Creates a db and thread workers
 */ 
WallaDb_t *WallaDbCreateDb(char *filename);

/* 
 * Writes magic to new database
 */ 
WALLA_STATUS WallaDbWriteMagic(WallaDb_t *db);

/* 
 * Sets universal buffer length
 * Recorded data is multiplied by scale_factor for each layer
 */ 
WALLA_STATUS WallaDbSetBufLen(WallaDb_t *db, long length, long scale_factor);

/* 
 * Sets root node
 * Allocates a bunch of blank nodes in the database
 *  as a singly linked list
 * Sets up pointer to incoming sensor data buffer area
 */ 
WALLA_STATUS WallaDbSetupMemory(WallaDb_t *db);

/* 
 * Allows sensors to submit data
 */ 
WALLA_STATUS WallaDbSubmitEntry(WallaDb_t *db, WallaPos_t *pos, long epoch, double data);

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
char *WallaDbQuery(WallaDb_t *db, char *query);

/* 
 * Turns WallaNodeInfo into a json string
 */
char *WallaNodeInfoToJson(WallaNodeInfo_t *walla_node_info);

/* 
 * Get raw WallaEntry_t struct by point
 * May retrieve closest point
 * 
 */ 
WallaEntry_t *WallaDbQueryInfoByPoint(WallaDb_t *db, WallaPos_t *pos);

/* 
 * Get closest WallaEntry_t struct by time
 * 
 */ 
WallaEntry_t *WallaDbQueryInfoByPointAndTime(WallaDb_t *db, WallaPos_t *pos, long epoch);

/* 
 * Get stddev by point
 */ 
double WallaDbQueryStdDevByPoint(WallaDb_t *db, WallaPos_t *pos);

/* 
 * Get closest stddev by time and position
 */ 
WallaEntry_t *WallaDbQueryStdDevByPointAndTime(WallaDb_t *db, WallaPos_t *pos, long epoch);

#endif /* __WALLA_DB_H__ */

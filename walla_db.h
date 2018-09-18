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

struct WallaPos {
    long x;
    long y;
    long z;
}

struct WallaNodeInfo {
    WallaPos pos;
    long epoch_start;
    long epoch_end;
    double average;
    double stdev;
}

typedef struct WallaEntry {
    long epoch;
    double value;
} WallaEntry_t;

struct WallaDb;

enum WALLA_STATUS
{
    WALLA_SUCCESS,
    WALLA_OOPS,
    WALLA_BYE,
    WALLA_BAD_FORMAT,
    WALLA_NOT_IMPLEMENTED,
};

/* 
 * Create a walladb server
 *    - If the file does not exist, the file will be created
 *      with default parameters
 *    - If the file does exist, the server processes will be
 *      restarted
 */ 
WallaDb *WallaDbCreateServer(char *filename);

/* 
 * Shutdown walladb server and dealloc all data
 */ 
WALLA_STATUS WallaDbShutdownServer(WallaDb *db);

/* 
 * Connect to the database
 */ 
WallaDb *WallaDbConnect(char *filename);

/* 
 * Disconnect from database.
 *    - If the database is owned by this process,
 *      the thread workers are shut down
 *    - If the remote db is shut down,
 *      data can be lost - how do I deal with this?
 * Deallocs db handle
 */ 
WALLA_STATUS WallaDbDisconnect(WallaDb *db);

/* 
 * Creates a db and thread workers
 */ 
WallaDb *WallaDbCreateDb(char *filename);

/* 
 * Writes magic to new database
 */ 
WALLA_STATUS WallaDbWriteMagic(WallaDb *db);

/* 
 * Sets universal buffer length
 * Recorded data is multiplied by scale_factor for each layer
 */ 
WALLA_STATUS WallaDbSetBufLen(WallaDb *db, long length, long scale_factor);

/* 
 * Sets root node
 * Allocates a bunch of blank nodes in the database
 *  as a singly linked list
 * Sets up pointer to incoming sensor data buffer area
 */ 
WALLA_STATUS WallaDbSetupMemory(WallaDb *db);

/* 
 * Allows sensors to submit data
 */ 
WALLA_STATUS WallaDbSubmitEntry(WallaDb *db, WallaPos *pos, long epoch, double data);

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
char *WallaDbQuery(WallaDb *db, char *query);

/* 
 * Turns WallaEntry into a json string
 */
char *WallaEntryToJson(WallaEntry *walla_entry);

/* 
 * Turns WallaNodeInfo into a json string
 */
char *WallaNodeInfoToJson(WallaEntry *walla_entry);

/* 
 * Get raw WallaEntry struct by point
 * May retrieve closest point
 * 
 */ 
WallaEntry *WallaDbQueryInfoByPoint(WallaDb *db, WallaPos *pos);

/* 
 * Get closest WallaEntry struct by time
 * 
 */ 
WallaEntry *WallaDbQueryInfoByPointAndTime(WallaDb *db, WallaPos *pos, long epoch);

/* 
 * Get stddev by point
 */ 
double WallaDbQueryStdDevByPoint(WallaDb *db, WallaPos *pos);

/* 
 * Get closest stddev by time and position
 */ 
WallaEntry *WallaDbQueryStdDevByPointAndTime(WallaDb *db, WallaPos *pos, long epoch);

#endif /* __WALLA_DB_H__ */

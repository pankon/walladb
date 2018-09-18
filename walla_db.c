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

#include <stdio.h>  /* sprintf, asprintf    */
#include <stdlib.h> /* malloc, free         */

#include "walla_db.h"

struct WallaNode {
    union meta {
        long depth;
        long next_free;
    };
    WallaNodeInfo info;
    WallaNode *parent;
    WallaNode **children;
    long n_entries;
    WallaEntry *circbuf_head;
    WallaEntry *circbuf_tail;
}

struct WallaServer {
}

struct WallaClient {
}

struct WallaDb {
    char *filename;
    char *data;
    long pos;
    long scale_factor;
    WallaNode root;
    long free_list_start;
    int is_server;
    union connection
    {
        WallaServer *server;
        WallaClient *client;
    };
}

static char *null_json = "{}";

/* 
 * Create a walladb server
 *    - If the file does not exist, the file will be created
 *      with default parameters
 *    - If the file does exist, the server processes will be
 *      restarted
 */ 
WallaDb *WallaDbCreateServer(char *filename)
{
    return (NULL);
}

/* 
 * Shutdown walladb server and dealloc all data
 */ 
WALLA_STATUS WallaDbShutdownServer(WallaDb *db)
{
    return (NULL);
}

/* 
 * Connect to the database
 */ 
WallaDb *WallaDbConnect(char *filename)
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
WALLA_STATUS WallaDbDisconnect(WallaDb *db)
{
    return (NULL);
}

/* 
 * Creates a db and thread workers
 */ 
WallaDb *WallaDbCreateDb(char *filename)
{
    return (NULL);
}

/* 
 * Writes magic to new database
 */ 
WALLA_STATUS WallaDbWriteMagic(WallaDb *db)
{
    return (WALLA_NOT_IMPLEMENTED);   
}

/* 
 * Sets universal buffer length
 * Recorded data is multiplied by scale_factor for each layer
 */ 
WALLA_STATUS WallaDbSetBufLen(WallaDb *db, long length, long scale_factor)
{
    return (WALLA_NOT_IMPLEMENTED);
}

/* 
 * Sets root node
 * Allocates a bunch of blank nodes in the database
 *  as a singly linked list
 * Sets up pointer to incoming sensor data buffer area
 */ 
WALLA_STATUS WallaDbSetupMemory(WallaDb *db)
{
    return (WALLA_NOT_IMPLEMENTED);
}

/* 
 * Allows sensors to submit data
 */ 
WALLA_STATUS WallaDbSubmitEntry(WallaDb *db, WallaPos *pos, long epoch, double data)
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
char *WallaDbQuery(WallaDb *db, char *query)
{
    return (NULL);
}

/* 
 * Turns WallaEntry into a json string
 */
char *WallaEntryToJson(WallaEntry *walla_entry)
{
    return (NULL);
}

WallaEntry_t *WallaEntryCreate(long epoch, double value)
{
    WallaEntry_t *walla_entry = NULL;

    if (NULL == (walla_entry = malloc(sizeof(WallaEntry_t))))
    {
        return (NULL);
    }

    walla_entry->epoch = epoch;
    wall_entry->value = value;

    return walla_entry;
}

void WallaEntryDestroy(WallaEntry *walla_entry)
{
    free(walla_entry);
    walla_entry = NULL;
}

/* 
 * Turns WallaNodeInfo into a json string
 * returns string: { epoch : epoch_val, value: double_value }
 */
char *WallaNodeInfoToJson(WallaEntry *walla_entry)
{
    static char *walla_node_to_json = "{ epoch = %lu, value = %10.10d }";
    char *json_string = NULL;
    int len = walla_node_to_json;

    if (walla_entry == null)
    {
        return null_json;
    }

    len = asprintf(&json_string, walla_node_to_json, walla_entry->epoch, walla_entry->value);
    if (-1 == value_len)
    {
        return (NULL);
    }
    
    return json_string;
}

/* 
 * Get raw WallaEntry struct by point
 * May retrieve closest point
 * 
 */ 
WallaEntry *WallaDbQueryInfoByPoint(WallaDb *db, WallaPos *pos)
{
    return (NULL);
}

/* 
 * Get closest WallaEntry struct by time
 * 
 */ 
WallaEntry *WallaDbQueryInfoByPointAndTime(WallaDb *db, WallaPos *pos, long epoch)
{
    return (NULL);
}

/* 
 * Get stddev by point
 */ 
double WallaDbQueryStdDevByPoint(WallaDb *db, WallaPos *pos)
{
    return (NULL);
}

/* 
 * Get closest stddev by time and position
 */ 
WallaEntry *WallaDbQueryStdDevByPointAndTime(WallaDb *db, WallaPos *pos, long epoch)
{
    return (NULL);
}

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

#include "json.h"
#include "walla_db.h"

typedef struct WallaNode {
    union {
        long depth;
        long next_free;
    } meta;
    WallaNodeInfo_t info;
    struct WallaNode *parent;
    struct WallaNode **children;
    long n_entries;
    WallaEntry_t *circbuf_head;
    WallaEntry_t *circbuf_tail;
} WallaNode_t;

typedef struct WallaServer {
    int stub;
} WallaServer_t;

typedef struct WallaClient {
    int stub;
} WallaClient_t;

struct WallaDb {
    char *filename;
    char *data;
    long pos;
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
WallaDb_t *WallaDbCreateDb(char *filename)
{
    return (NULL);
}

/* 
 * Writes magic to new database
 */ 
WALLA_STATUS WallaDbWriteMagic(WallaDb_t *db)
{
    return (WALLA_NOT_IMPLEMENTED);   
}

/* 
 * Sets universal buffer length
 * Recorded data is multiplied by scale_factor for each layer
 */ 
WALLA_STATUS WallaDbSetBufLen(WallaDb_t *db, long length, long scale_factor)
{
    return (WALLA_NOT_IMPLEMENTED);
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
 * Turns WallaNodeInfo into a json string
 * {"(x, y, z, t_start, t_end)": {
 *     "avg" : 0.324232
 *     "max" : 324.1232
 *     "min" : 0.010230
 *     "std" : 13.32459
 *  } }
 */
char *WallaNodeInfoToJson(WallaNodeInfo_t *walla_node_info)
{
    static char *walla_node_info_to_json = 
        "{\"(%lu, %lu, %lu, %lu, %lu)\":"
                "{\"avg\":"FLOAT_OUTPUT","
                " \"max\":"FLOAT_OUTPUT","
                " \"min\":"FLOAT_OUTPUT","
                " \"std\":"FLOAT_OUTPUT","
                "}"
        "}";
    char *json_string = NULL;
    int len = -1;

    if (walla_node_info == NULL)
    {
        return null_json;
    }
    
    len = asprintf(
        &json_string, walla_node_info_to_json, 
        walla_node_info->pos.x,
        walla_node_info->pos.x,
        walla_node_info->pos.x,
        walla_node_info->epoch_start,
        walla_node_info->epoch_end,
        walla_node_info->average,
        walla_node_info->max,
        walla_node_info->min,
        walla_node_info->stdev);
    if (-1 == len)
    {
        return (NULL);
    }
    
    return json_string;
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

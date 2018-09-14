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
 * Fri Sep 14 14:44:15 2018
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

struct WallaEntry {
    long epoch;
    double value;
}

struct WallaDb;

enum WALLA_STATUS
{
    WALLA_SUCCESS,
    WALLA_OOPS,
    WALLA_BYE,
    WALLA_BAD_FORMAT,
};

WallaDb *WallaDbCreateServer(char *filename);

WALLA_STATUS WallaDbShutdownServer(WallaDb *db);

WallaDb *WallaDbConnect(char *filename);

WALLA_STATUS WallaDbDisconnect(WallaDb *db);

WallaDb *WallaDbCreateDb(char *filename);

WALLA_STATUS WallaDbWriteMagic(WallaDb *db);

WALLA_STATUS WallaDbSetBufLen(WallaDb *db, long length, long scale_factor);

WALLA_STATUS WallaDbSetupMemory(WallaDb *db);

WALLA_STATUS WallaDbSubmitEntry(WallaDb *db, WallaPos *pos, long epoch, double data);

char *WallaDbQuery(WallaDb *db, char *query);

WallaEntry *WallaDbQueryInfoByPoint(WallaDb *db, WallaPos *pos);

WallaEntry *WallaDbQueryInfoByPointAndTime(WallaDb *db, WallaPos *pos, long epoch);

double WallaDbQueryStdDevByPoint(WallaDb *db, WallaPos *pos);

WallaEntry *WallaDbQueryStdDevByPointAndTime(WallaDb *db, WallaPos *pos, long epoch);

#endif /* __WALLA_DB_H__ */

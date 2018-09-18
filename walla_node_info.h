/**************************************
 * walla_node_info.h
 * 
 * Walla Entry
 * Pankonworks
 * 
 * Walla Node Info
 * Structure that holds all info on the walla node
 * This struct has no memory of its own, and thus has no constructors/destructors
 *
 * Fri Sep 14 14:51:47 2018
 ***************************************/

#ifndef __WALLA_NODE_INFO_H__
#define __WALLA_NODE_INFO_H__

#include "walla_status.h"
#include "walla_entry.h"

typedef struct WallaNodeInfo {
    WallaPos_t pos;
    unsigned long epoch_start;
    unsigned long epoch_end;
    double average;
    double max;
    double min;
    double stdev;
} WallaNodeInfo_t;

WALLA_STATUS WallaNodeInfoUpdateWithEntry(
    WallaNodeInfo_t *walla_node_info, 
    WallaEntry_t *walla_entry,
    long current_number_of_entries
);

/* 
 * Turns WallaNodeInfo into a json string
 */
char *WallaNodeInfoToJson(WallaNodeInfo_t *walla_node_info);

#endif /* __WALLA_NODE_INFO_H__ */
/**************************************
 * main.c
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

#include <stdio.h>  /* printf */
#include <stdlib.h> /* free */
#include "logging.h"
#include "walla_db.h"
#include "walla_pos.h"
#include "walla_entry.h"
#include "walla_node_info.h"
#include "walla_node.h"

void TestBasicDb(log_t *log);
void TestWallaEntry(log_t *log);
void TestWallaNode(log_t *log);
void TestWallaNodeUpdate(
    log_t *log, 
    WallaNode_t *walla_node, 
    WallaEntry_t *walla_entry, 
    unsigned long time, 
    double value);

int main(int argc, char **argv)
{
	log_t *log = NULL;
	
    log = LogStdErr();
	
    TestBasicDb(log);  
    
    LogDestroy(log);
    log = NULL;

    return (0);
}

void TestBasicDb(log_t *log)
{
    char *filename = "test.db";
    WallaDb_t *walla_db = NULL;

    walla_db = WallaDbCreateDb(filename, 10, 2, 4);

    WallaDbDestroy(walla_db);
    walla_db = NULL;
}

void TestWallaEntry(log_t *log)
{
	char *buffer = NULL;
    WallaEntry_t *walla_entry = NULL;
    
    walla_entry = WallaEntryCreate(1000, 0.2342);
    
    buffer = WallaEntryToJson(walla_entry);
    LogInfo(log, "entry: %s\n", buffer);
    free(buffer);
    
    WallaEntryDestroy(walla_entry);
    walla_entry = NULL;
}

void TestWallaNode(log_t *log)
{
	char *buffer = NULL;

    WallaNode_t *walla_node = NULL;
    WallaPos_t *walla_pos = NULL;
    WallaEntry_t *walla_entry = NULL;

    walla_pos = WallaPosCreate(log, 1, 3, 4);

    walla_node = WallaNodeCreate(log, NULL, 0, 0, walla_pos);
    
    if (NULL == walla_node)
    {
        LogError(log, "[WallaNodeCreate failed]\n");
        return;
    }

    buffer = WallaNodeToJson(log, walla_node);
    LogInfo(log, "node: %s", buffer);
    free(buffer);

    buffer = WallaNodeInfoToJson(log, &(walla_node->info));
    LogInfo(log, "node info: %s", buffer);
    free(buffer);

    walla_entry = WallaEntryCreate(1000, 0.2342);
    WallaNodeUpdateWithEntry(log, walla_node, walla_entry);

    buffer = WallaNodeInfoToJson(log, &(walla_node->info));
    LogInfo(log, "node info: %s", buffer);
    free(buffer);

    TestWallaNodeUpdate(log, walla_node, 
                        walla_entry, 100, 0.5);
    TestWallaNodeUpdate(log, walla_node, 
                        walla_entry, 1020, 0.6);
    TestWallaNodeUpdate(log, walla_node, 
                        walla_entry, 1034, 0.7);
    TestWallaNodeUpdate(log, walla_node, 
                        walla_entry, 1100, -1.343);
    
    WallaEntryDestroy(walla_entry);
    walla_entry = NULL;
    WallaNodeDestroy(log, walla_node);
    walla_node = NULL;
}

void TestWallaNodeUpdate(
    log_t *log, 
    WallaNode_t *walla_node, 
    WallaEntry_t *walla_entry, 
    unsigned long time, 
    double value)
{
    char *buffer = NULL;

    walla_entry = WallaEntryInit(walla_entry, 
                                 time, value);
    WallaNodeUpdateWithEntry(log, walla_node, walla_entry);

    buffer = WallaNodeInfoToJson(log, &(walla_node->info));
    LogInfo(log, "node info: %s", buffer);
    free(buffer);
    buffer = NULL;
}

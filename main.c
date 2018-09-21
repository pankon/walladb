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
#include "walla_pos.h"
#include "walla_entry.h"
#include "walla_node_info.h"
#include "walla_node.h"

void TestWallaEntry();
void TestWallaNode();
void TestWallaNodeUpdate(WallaNode_t *walla_node, WallaEntry_t *walla_entry, 
    unsigned long time, double value);

int main(int argc, char **argv)
{
    TestWallaNode();

    return (0);
}


void TestWallaEntry()
{
	char *buffer = NULL;
    WallaEntry_t *walla_entry = NULL;
    
    walla_entry = WallaEntryCreate(1000, 0.2342);
    
    buffer = WallaEntryToJson(walla_entry);
    printf("entry: %s\n", buffer);
    free(buffer);
    
    WallaEntryDestroy(walla_entry);
    walla_entry = NULL;
}

void TestWallaNode()
{
	char *buffer = NULL;
    WallaNode_t *walla_node = NULL;
    WallaPos_t *walla_pos = NULL;
    WallaEntry_t *walla_entry = NULL;

    walla_pos = WallaPosCreate(1, 3, 4);

    walla_node = WallaNodeCreate(NULL, 0, 0, walla_pos);
    
    if (NULL == walla_node)
    {
        LogError("[WallaNodeCreate failed]\n");
        return;
    }

    buffer = WallaNodeToJson(walla_node);
    LogInfo("node: %s", buffer);
    free(buffer);

    buffer = WallaNodeInfoToJson(&(walla_node->info));
    LogInfo("node info: %s", buffer);
    free(buffer);

    walla_entry = WallaEntryCreate(1000, 0.2342);
    WallaNodeUpdateWithEntry(walla_node, walla_entry);

    buffer = WallaNodeInfoToJson(&(walla_node->info));
    LogInfo("node info: %s", buffer);
    free(buffer);

    TestWallaNodeUpdate(walla_node, walla_entry, 1001, 0.5);
    TestWallaNodeUpdate(walla_node, walla_entry, 1001, 0.6);
    TestWallaNodeUpdate(walla_node, walla_entry, 1001, 0.7);
    TestWallaNodeUpdate(walla_node, walla_entry, 1001, -1.343);
    
    WallaEntryDestroy(walla_entry);
    walla_entry = NULL;
    WallaNodeDestroy(walla_node);
    walla_node = NULL;
}

void TestWallaNodeUpdate(WallaNode_t *walla_node, WallaEntry_t *walla_entry, 
    unsigned long time, double value)
{
    char *buffer = NULL;

    walla_entry = WallaEntryInit(walla_entry, time, value);
    WallaNodeUpdateWithEntry(walla_node, walla_entry);

    buffer = WallaNodeInfoToJson(&(walla_node->info));
    LogInfo("node info: %s", buffer);
    free(buffer);
    buffer = NULL;
}
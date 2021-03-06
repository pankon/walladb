/**************************************
 * walla_node.c
 * 
 * Walla Entry
 * Pankonworks
 * 
 * Walla Node
 * Main data structure
 *
 * Fri Sep 14 14:51:47 2018
 ***************************************/

#define _GNU_SOURCE
#include <stdio.h>  /* sprintf, asprintf    */
#include <stdlib.h> /* malloc, free         */

#include "logging.h"
#include "json.h"
#include "walla_node_info.h"
#include "walla_node.h"

/*
 * Alloc's and creates a walla node
 */
WallaNode_t *WallaNodeCreate(
    log_t *log, WallaNode_t *parent, 
    int depth, int n_entries, WallaPos_t *pos)
{
    WallaNode_t *walla_node = NULL;

    if (NULL == pos)
    {
        LogError(log, "[WallaNodeCreate] : Null pos passed\n");
        return (NULL);
    }

    if (NULL == (walla_node = malloc(sizeof(WallaNode_t))))
    {
        LogError(log, "[WallaNodeCreate] : Alloc failed\n");
        return (NULL);
    }

    return (WallaNodeInit(log, walla_node, parent, depth, n_entries, pos));
}

/*
 * Creates a walla node on existing memory
 * zeroes existing memory
 * Inits WallaNodeInfo_t
 */
WallaNode_t *WallaNodeInit(
    log_t *log, void *memory, 
    WallaNode_t *parent, 
    int depth, int n_entries, WallaPos_t *pos)
{
    WallaNode_t *walla_node = NULL;

    if (NULL == memory || NULL == pos)
    {
        LogError(log, "[WallaNodeInit] :"
                      " Null memory passed\n");
        return (NULL);
    }

    walla_node = (WallaNode_t *)memory;

    walla_node->parent = parent;
    walla_node->meta.depth = depth;
    walla_node->n_entries = n_entries;
    walla_node->children = NULL;
    
    /* TODO: circbuf */
    walla_node->circbuf_head = NULL;
    walla_node->circbuf_tail = NULL;
    
    WallaNodeInfoInit(log, &(walla_node->info), pos);

    return (walla_node);
}

void WallaNodeDestroy(log_t *log, WallaNode_t *walla_node)
{
    if (NULL == walla_node)
    {
        return;
    }

    WallaNodeZero(log, walla_node);

    free(walla_node);
}

void WallaNodeZero(log_t *log, WallaNode_t *walla_node)
{
    if (NULL == walla_node)
    {
        return;
    }

    WallaNodeDealloc(log, walla_node, 0);
}

void WallaNodeDealloc(
    log_t *log, 
    WallaNode_t *walla_node, 
    int next_free)
{
    if (NULL == walla_node)
    {
        return;
    }

    WallaNodeInfoZero(log, &walla_node->info);
    walla_node->parent = NULL;

    /* what about freeing the children? call some memory function which calls this again */
    walla_node->children = NULL;

    /* free circbuf : TODO */
    walla_node->n_entries = 0;
    walla_node->circbuf_head = NULL;
    walla_node->circbuf_tail = NULL;
}

WALLA_STATUS WallaNodeUpdateWithEntry(
    log_t *log,
    WallaNode_t *walla_node, 
    WallaEntry_t *walla_entry
)
{
    if (NULL == walla_node || NULL == walla_entry)
    {
        return (WALLA_YOU_FORGOT_TO_PASS_ANYTHING);
    }

    /* TODO: Get number of actual entries in circbuf */
    return WallaNodeInfoUpdateWithEntry(
            log, 
            &(walla_node->info), 
            walla_entry, 
            &(walla_node->n_entries));
}

/* 
 * Turns WallaNode into a json string
 * requires walking over circular buffer and constructing
 * json from the WallaEntry_t's.
 */
char *WallaNodeToJson(log_t *log, WallaNode_t *walla_node)
{
    /* TODO: Implement */
    return (JsonGetNull());
}


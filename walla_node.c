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
WallaNode_t *WallaNodeCreate(WallaNode_t *parent, int depth, int n_entries, WallaPos_t *pos)
{
    WallaNode_t *walla_node = NULL;

    if (NULL == pos)
    {
        LogError("Null pos passed\n");
        return (NULL);
    }

    if (NULL == (walla_node = malloc(sizeof(WallaNode_t))))
    {
        LogError("Alloc failed\n");
        return (NULL);
    }

    return (WallaNodeInit(walla_node, parent, depth, n_entries, pos));
}

/*
 * Creates a walla node on existing memory
 * zeroes existing memory
 * Inits WallaNodeInfo_t
 */
WallaNode_t *WallaNodeInit(void *memory, WallaNode_t *parent, int depth, int n_entries, WallaPos_t *pos)
{
    WallaNode_t *walla_node = NULL;

    if (NULL == memory || NULL == pos)
    {
        LogError("Null memory passed\n");
        return (NULL);
    }

    walla_node = (WallaNode_t *)memory;

    walla_node->parent = parent;
    walla_node->meta.depth = depth;
    walla_node->n_entries = n_entries;
    WallaNodeInfoInit(&(walla_node->info), pos);

    return (walla_node);
}

void WallaNodeDestroy(WallaNode_t *walla_node)
{
    if (NULL == walla_node)
    {
        return;
    }

    WallaNodeZero(walla_node);

    free(walla_node);
}

void WallaNodeZero(WallaNode_t *walla_node)
{
    if (NULL == walla_node)
    {
        return;
    }

    WallaNodeDealloc(walla_node, 0);
}

void WallaNodeDealloc(WallaNode_t *walla_node, int next_free)
{
    if (NULL == walla_node)
    {
        return;
    }

    WallaNodeInfoZero(&walla_node->info);
    walla_node->parent = NULL;

    /* what about freeing the children? call some memory function which calls this again */
    walla_node->children = NULL;

    /* free circbuf : TODO */
    walla_node->n_entries = 0;
    walla_node->circbuf_head = NULL;
    walla_node->circbuf_tail = NULL;
}

WALLA_STATUS WallaNodeUpdateWithEntry(
    WallaNode_t *walla_node, 
    WallaEntry_t *walla_entry
)
{
    if (NULL == walla_node || NULL == walla_entry)
    {
        return (WALLA_YOU_FORGOT_TO_PASS_ANYTHING);
    }

    /* TODO: Get number of actual entries in circbuf */
    return WallaNodeInfoUpdateWithEntry(&(walla_node->info), walla_entry, &(walla_node->n_entries));
}

/* 
 * Turns WallaNode into a json string
 * requires walking over circular buffer and constructing
 * json from the WallaEntry_t's.
 */
char *WallaNodeToJson(WallaNode_t *walla_node)
{
    /* TODO: Implement */
    return (JsonGetNull());
}


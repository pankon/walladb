/**************************************
 * walla_node.h
 * 
 * Walla Entry
 * Pankonworks
 * 
 * Walla Node
 * Main data structure
 *
 * Fri Sep 14 14:51:47 2018
 ***************************************/

#ifndef __WALLA_NODE_H__
#define __WALLA_NODE_H__

#include "logging.h"
#include "walla_pos.h"
#include "walla_node_info.h"

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

/*
 * Alloc's and creates a walla node
 */
WallaNode_t *WallaNodeCreate(
    log_t *log, WallaNode_t *parent, 
    int depth, int n_entries, WallaPos_t *pos);

/*
 * Creates a walla node on existing memory
 * zeroes existing memory
 * Inits WallaNodeInfo_t
 */
WallaNode_t *WallaNodeInit(
    log_t *log, void *memory, 
    WallaNode_t *parent, 
    int depth, int n_entries, WallaPos_t *pos);

void WallaNodeDestroy(log_t *log, WallaNode_t *walla_node);
void WallaNodeZero(log_t *log, WallaNode_t *walla_node);
void WallaNodeDealloc(
    log_t *log, 
    WallaNode_t *walla_node, 
    int next_free);

WALLA_STATUS WallaNodeUpdateWithEntry(
    log_t *log, 
    WallaNode_t *walla_node, 
    WallaEntry_t *walla_entry
);

/* 
 * Turns WallaNode into a json string
 */
char *WallaNodeToJson(log_t *log, WallaNode_t *walla_node);

#endif /* __WALLA_NODE_H__ */

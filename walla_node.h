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

typedef struct WallaPos {
    long x;
    long y;
    long z;
} WallaPos_t;

typedef struct WallaNodeInfo {
    WallaPos_t pos;
    long epoch_start;
    long epoch_end;
    double average;
    double max;
    double min;
    double stdev;
} WallaNodeInfo_t;

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

#endif /* __WALLA_NODE_H__ */
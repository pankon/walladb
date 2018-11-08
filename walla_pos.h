/**************************************
 * walla_pos.h
 * 
 * Walla Pos
 * Pankonworks
 * 
 * Walla Pos
 * provides utils for working in 3d space
 *
 * Fri Sep 14 14:51:47 2018
 ***************************************/

#ifndef __WALLA_POS_H__
#define __WALLA_POS_H__

#include "logging.h"

typedef struct WallaPos {
    long x;
    long y;
    long z;
} WallaPos_t;

WallaPos_t *WallaPosCreate(log_t *log, long x, long y, long z);
WallaPos_t *WallaPosInit(log_t *log, void *memory, 
                         long x, long y, long z);
void WallaPosZero(log_t *log, WallaPos_t *walla_pos);
void WallaPosDestroy(log_t *log, WallaPos_t *walla_pos);

WallaPos_t *WallaPosDistance(WallaPos_t *a, WallaPos_t *b);

#endif /* __WALLA_POS_H__ */

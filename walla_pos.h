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

typedef struct WallaPos {
    long x;
    long y;
    long z;
} WallaPos_t;

WallaPos_t *WallaPosCreate(long x, long y, long z);
WallaPos_t *WallaPosInit(void *memory, long x, long y, long z);
WallaPos_t *WallaPosZero(WallaPos_t *walla_pos);
WallaPos_t *WallaPosDestroy(WallaPos_t *walla_pos);

#endif /* __WALLA_POS_H__ */
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
void WallaPosZero(WallaPos_t *walla_pos);
void WallaPosDestroy(WallaPos_t *walla_pos);

WallaPos_t *WallaPosDistance(WallaPos_t *a, WallaPos_t *b);

#endif /* __WALLA_POS_H__ */
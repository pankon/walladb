/**************************************
 * walla_pos.c
 * 
 * Walla Pos
 * Pankonworks
 * 
 * Walla Pos
 * provides utils for working in 3d space
 *
 * Fri Sep 14 14:51:47 2018
 ***************************************/

#include <stdio.h> 	/* NULL 				*/
#include <stdlib.h> /* malloc, free         */
#include "logging.h"
#include "walla_pos.h"

WallaPos_t *WallaPosCreate(log_t *log, long x, long y, long z)
{
	WallaPos_t *walla_pos = NULL;

	if (NULL == (walla_pos = malloc(sizeof(WallaPos_t))))
	{
		LogError(log, "[WallaPosCreate] malloc failed");
		return (NULL);
	}

	return (WallaPosInit(log, walla_pos, x, y, z));
}

WallaPos_t *WallaPosInit(log_t *log, void *memory, 
                         long x, long y, long z)
{
	WallaPos_t *walla_pos = NULL;

	if (NULL == memory)
	{
		LogError(log, "[WallaPosCreate] null memory passed");
		return (NULL);
	}

	walla_pos = (WallaPos_t *)memory;

	walla_pos->x = x;
	walla_pos->y = y;
	walla_pos->z = z;

	return (walla_pos);
}

void WallaPosZero(log_t *log, WallaPos_t *walla_pos)
{
	WallaPosInit(log, walla_pos, 0, 0, 0);
}

void WallaPosDestroy(log_t *log, WallaPos_t *walla_pos)
{
	WallaPosZero(log, walla_pos);

	free(walla_pos);
	walla_pos = NULL;
}

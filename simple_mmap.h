/**************************************
 * simple_mmap.h
 * 
 * SimpleMmap
 * Pankonworks
 * Nathan Pankowsky
 * Usage:
 *
 * nu, just use it
 * Fri Sep 14 15:32:50 2018
***************************************/

#ifndef __SIMPLE_MMAP_H__
#define __SIMPLE_MMAP_H__

typedef struct 
{
    char *data;
    int handle;
    int mode;
    size_t size;
} mmap_t;

/* 
 * Get a char array, mmapped
 */ 
mmap_t *SimpleMmapGetMeACharArray(char *filename);

/* 
 * Get a rid of a mmapped char array
 */ 
void SimpleMmapPleaseDisposeOfThisCharArray(
    mmap_t *mmapped_data
);

mmap_t *SimpleMmapGetWriteableFixedSizeFile(char *filename);

#endif /* __SIMPLE_MMAP_H__ */
